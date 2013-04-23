////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Nicolas Gallerand
//
//  *** Quadrature encoder driver for STM32F4 ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "Cerberus_Hardware.h"
#include "Cerberus_Hardware_Cerberus_Hardware_QuadratureEncoder.h"
#include <tinyhal.h>
#include <cpu.h>
#include <stm32f4xx.h>



using namespace Cerberus::Hardware;

static CLR_RT_HeapBlock_NativeEventDispatcher *g_Context = NULL;
static UINT64 g_UserData = 0;


static void STM32F4_TIM2_Interrupt(void* param)
{
	GLOBAL_LOCK(irq);
	UINT32 count = 0;
	UINT32 source = 0;
	//reset interrupt flags
	if ((TIM2->SR & TIM_SR_CC3IF) != 0) {
		source = 1;
		count = TIM2->CCR3;
		if ((TIM2->SR & TIM_SR_CC3OF) != 0) {
			TIM2->SR &= ~TIM_SR_CC3OF;
			source |= 0x10;
		}
	}
	else if ((TIM2->SR & TIM_SR_CC4IF) != 0) {
		source = 2;
		count = TIM2->CCR4;
		if ((TIM2->SR & TIM_SR_CC4OF) != 0) {
			TIM2->SR &= ~TIM_SR_CC4OF;
			source |= 0x10;
		}
	}
	SaveNativeEventToHALQueue( g_Context, source, count);
};

// -- NativeEventDispatcher methods implementation --
static HRESULT Initialize( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
	debug_printf("Init Quad Encoder driver");
	g_Context  = pContext;
	g_UserData = userData;
	//Power port A (should be already done in bootstrap) and TIM2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//Reserve PA0 and PA1
	if (CPU_GPIO_ReservePin(0, true) == false) 
		return CLR_E_PIN_UNAVAILABLE;
	if (CPU_GPIO_ReservePin(1, true) == false)
		return CLR_E_PIN_UNAVAILABLE;
	//PA0 and PA1 mode alternate: 10b
	GPIOA->MODER &= ~(GPIO_MODER_MODER1 | GPIO_MODER_MODER0);
	GPIOA->MODER |= (GPIO_MODER_MODER1_1 | GPIO_MODER_MODER0_1);
	//Alternate function AF1 : TIM2 input for PA1 and PA0
	GPIOA->AFR[0] &= ~0x000000FF;
	GPIOA->AFR[0] |= 0x00000011;
	//pull up
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0);
	TIM2->CR2 = 0;
	//Encoder X1 count on PA0 edge
	TIM2->SMCR &= ~TIM_SMCR_SMS;
	TIM2->SMCR |= TIM_SMCR_ECE | TIM_SMCR_SMS_1;
	//filter N=8
	TIM2->SMCR &= ~TIM_SMCR_ETF;
	TIM2->SMCR |= TIM_SMCR_ETF_1 | TIM_SMCR_ETF_0;
	//no polarity invertion, rising edge
	TIM2->CCER = 0;
	//Count up to 32bits
	TIM2->ARR = 0xFFFFFFFF;

	//set capture to PA2 and PA3 rising edge to generate an interrupt at each count
	if (CPU_GPIO_ReservePin(2, true) == false) 
		return CLR_E_PIN_UNAVAILABLE;
	if (CPU_GPIO_ReservePin(3, true) == false)
		return CLR_E_PIN_UNAVAILABLE;
	//PA3 and PA2 mode alternate: 10b
	GPIOA->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER2);
	GPIOA->MODER |= (GPIO_MODER_MODER3_1 | GPIO_MODER_MODER2_1);
	//Alternate function AF1 : TIM2 input for PA3 and PA2
	GPIOA->AFR[0] &= ~0x0000FF00;
	GPIOA->AFR[0] |= 0x00001100;
	//pull up
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR3 | GPIO_PUPDR_PUPDR2);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR2_0);

	TIM2->CCMR2 &= ~(TIM_CCMR2_CC4S | TIM_CCMR2_CC3S);
	TIM2->CCMR2 |= (TIM_CCMR2_CC4S_0 | TIM_CCMR2_CC3S_0);
	//input filter N=8
	TIM2->CCMR2 &= ~(TIM_CCMR2_IC4F | TIM_CCMR2_IC3F);
	TIM2->CCMR2 |= TIM_CCMR2_IC4F_1 | TIM_CCMR2_IC4F_0 | TIM_CCMR2_IC3F_1 | TIM_CCMR2_IC3F_0;
	//no prescaler
	TIM2->CCMR2 &= ~(TIM_CCMR2_IC4PSC | TIM_CCMR2_IC3PSC);
	//non inverting/rising edge
	TIM2->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC3P);
	//no DMA
	TIM2->DIER &= ~(TIM_DIER_CC4DE | TIM_DIER_CC3DE);

	CPU_INTC_ActivateInterrupt(TIM2_IRQn, STM32F4_TIM2_Interrupt, 0);
	TIM2->CR1 |= TIM_CR1_CEN;
	//enable capture
	TIM2->CCER |= (TIM_CCER_CC4E | TIM_CCER_CC3E);
	return S_OK;
}

static HRESULT SetEnable( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
	GLOBAL_LOCK(irq);
	if (fEnable)
		//set interrupt
			TIM2->DIER |= (TIM_DIER_CC4IE | TIM_DIER_CC3IE);
	else
		//reset interrupt
		TIM2->DIER &= ~(TIM_DIER_CC4IE | TIM_DIER_CC3IE);

	return S_OK;
}

static HRESULT Cleanup( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
	GLOBAL_LOCK(irq);
	g_Context = NULL;
	g_UserData = 0;
	TIM2->CCR1 &= ~TIM_CR1_CEN;
	//no capture/compare selection
	TIM2->CCMR2 &= ~(TIM_CCMR2_CC4S | TIM_CCMR2_CC3S);
	//disable capture compare 
	TIM2->CCER &= ~(TIM_CCER_CC4E | TIM_CCER_CC3E);

	CPU_INTC_DeactivateInterrupt(TIM2_IRQn);
	CPU_GPIO_ReservePin(3, false);
	CPU_GPIO_ReservePin(2, false);
	CPU_GPIO_ReservePin(1, false);
	CPU_GPIO_ReservePin(0, false);
	CleanupNativeEventsFromHALQueue( pContext );
	return S_OK;
}
// -- end of NativeEventDispatcher methods implementation --

// -- Declarations required by NativeEventDispatcher --
static const CLR_RT_DriverInterruptMethods g_QuadratureEncoder_DriverInterruptMethods = 
{ 
	Initialize,
	SetEnable,
	Cleanup
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Cerberus_Hardware_QuadratureEncoder_Driver =
{
	"QuadratureEncoder_Driver", 
	DRIVER_INTERRUPT_METHODS_CHECKSUM,
	&g_QuadratureEncoder_DriverInterruptMethods
};
// -- end of declarations required by NativeEventDispatcher --


INT32 QuadratureEncoder::get_Count( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return TIM2->CNT;
}

void QuadratureEncoder::set_Count( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
	TIM2->CNT = param0;
}

INT8 QuadratureEncoder::get_InputCaptureEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if (TIM2->CCER & TIM_CCER_CC4E)
		return TRUE;
	if (TIM2->CCER & TIM_CCER_CC3E)
		return TRUE;

	return FALSE;
}

void QuadratureEncoder::set_InputCaptureEnabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr )
{
	if (param0 == TRUE) 
		TIM2->CCER |= (TIM_CCER_CC4E | TIM_CCER_CC3E);
	else
		TIM2->CCER &= ~(TIM_CCER_CC4E | TIM_CCER_CC3E);
}

INT8 QuadratureEncoder::get_CountEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if (TIM2->CR1  & TIM_CR1_CEN)
		return TRUE;
	return FALSE;
}

void QuadratureEncoder::set_CountEnabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr )
{
	if (param0 == FALSE)
		TIM2->CR1 &= ~TIM_CR1_CEN;
	else
		TIM2->CR1 |= TIM_CR1_CEN;
}

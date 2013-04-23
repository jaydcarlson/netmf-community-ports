////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Portions Copyright (c) GHI Electronics, LLC
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\stm32f4xx.h"

//--//
const int min = 0x0000;
const int max = 0x0FFF;

BOOL DA_Initialize( DA_CHANNEL channel, INT32 precisionInBits )
{
	if( channel < 1 || channel > 2)
		return FALSE;

	//GPIOA clock enable (to be used with DAC) 
	//Bit 0 GPIOAEN: IO port A clock enable
	RCC->AHB1ENR |= (1 << 0);
	//Bit 29 DACEN: DAC interface clock enable
	RCC->APB1ENR |= (1 << 29);

	//11b: Analog mode
	if (channel == 1)
	{
		//Pin 4 is analog
		GPIOA->MODER |= (3 << 8);

		//00: No pull-up, pull-down
		// Clear bits
		GPIOA->PUPDR &= ~(1 << 0);
	}
	else
	{
		//Pin 5 is analog
		GPIOA->MODER |= (3 << 10);

		GPIOA->PUPDR &= ~(1 << 1);
	}

	// Reset DAC
	DAC->CR = 0;

	if (channel == 1)
	{
		// Enable DAC channel 1
		DAC->CR |= (1 << 0);
	}
	else
	{
		// Enable DAC channel 2
		DAC->CR |= (1 << 16);
	}

	return TRUE;
}

void DA_Write( DA_CHANNEL channel, INT32 level )
{
	if(level > max)
		level = max;
	else if(level < min)
		level = min;

	if(channel == 1)
		DAC->DHR12R1 = level;
	else if(channel == 2)
		DAC->DHR12R2 = level;
}

UINT32 DA_DAChannels()
{
    return 2;
}

GPIO_PIN DA_GetPinForChannel( DA_CHANNEL channel )
{
	if(channel == 1)
		return (GPIO_PIN)((0 * 16) + 4);
	else if(channel == 2)
		return (GPIO_PIN)((0 * 16) + 5);
	else
		return GPIO_PIN_NONE;
}

BOOL DA_GetAvailablePrecisionsForChannel( DA_CHANNEL channel, INT32* precisions, UINT32& size )
{
    size = 0;
    if (precisions == NULL || (UINT32)channel > DA_DAChannels())
		return FALSE;
    precisions[0] = 12;
    size = 1;
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Implementation for STM32: Copyright (c) Oberon microsystems, Inc.
//
//  *** GPIO Driver ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <cores\arm\include\cpu.h>
#include "..\stm32f10x.h"

//--//

#define STM32_Gpio_MaxPorts 7  // A..G
#define STM32_Gpio_MaxPins (STM32_Gpio_MaxPorts * 16)
#define STM32_Gpio_MaxInt 16

#define STM32_GPIO_ALT_MODE_OUT    GPIO_ALT_MODE_1
#define STM32_GPIO_ALT_MODE_HIGH   GPIO_ALT_MODE_2
#define STM32_GPIO_ALT_MODE_OD     GPIO_ALT_MODE_3

// indexed port configuration access
#define Port(port) ((GPIO_TypeDef *) (GPIOA_BASE + (port << 10)))

static UINT32 g_debounceTicks;
static UINT16 g_pinReserved[STM32_Gpio_MaxPorts]; //  1 bit per pin

static GPIO_INTERRUPT_SERVICE_ROUTINE g_ISR[STM32_Gpio_MaxInt]; // interrupt handlers
static void* g_ISR_Param[STM32_Gpio_MaxInt]; // interrupt handler parameters
static UINT16 g_debounceFlags; // debounce flags
static HAL_COMPLETION g_completions[STM32_Gpio_MaxInt]; // debounce completions


/*
 * Debounce Completion Handler
 */
void STM32_GPIO_DebounceHandler (void* arg)
{
    UINT32 pin = (int)arg, num = pin & 0x0F;
    GPIO_INTERRUPT_SERVICE_ROUTINE isr = g_ISR[num];
    if (isr) {
        UINT32 port = pin >> 4, bit = 1 << num;
        UINT32 state = (Port(port)->IDR >> num) & 1; // actual pin state
        // if correct state or both edges active
        if ((state || (EXTI->FTSR & bit)) && (!state || (EXTI->RTSR & bit))) {
            isr(pin, state, g_ISR_Param[num]);
        }
    }
}

/*
 * Interrupt Handler
 */
void STM32_GPIO_ISR (int num)  // 0 <= num <= 15
{
    INTERRUPT_START
    
    GPIO_INTERRUPT_SERVICE_ROUTINE isr = g_ISR[num];
    if (isr) {
        UINT32 port = (AFIO->EXTICR[num >> 2] >> ((num & 0x3) << 2)) & 0xF;
        UINT32 pin = (port << 4) | num; // pin number
        UINT32 bit = 1 << num;
        g_completions[num].Abort();
        if (g_debounceFlags & bit) { // debounce
            g_completions[num].EnqueueTicks(HAL_Time_CurrentTicks() + g_debounceTicks);
        } else {
            UINT32 state = 0;
            if (EXTI->RTSR & bit) { // rising edge
                state = 1;
                if (EXTI->FTSR & bit) { // both edges
                    state = (Port(port)->IDR >> num) & 1; // actual pin state
                }
            }
            isr(pin, state, g_ISR_Param[num]);
        }
    }
    
    INTERRUPT_END
}

void STM32_GPIO_Interrupt0 (void* param) // EXTI0
{
    EXTI->PR = 1 << 0;   // reset pending bit
    STM32_GPIO_ISR(0);
}

void STM32_GPIO_Interrupt1 (void* param) // EXTI1
{
    EXTI->PR = 1 << 1;   // reset pending bit
    STM32_GPIO_ISR(1);
}

void STM32_GPIO_Interrupt2 (void* param) // EXTI2
{
    EXTI->PR = 1 << 2;   // reset pending bit
    STM32_GPIO_ISR(2);
}

void STM32_GPIO_Interrupt3 (void* param) // EXTI3
{
    EXTI->PR = 1 << 3;   // reset pending bit
    STM32_GPIO_ISR(3);
}

void STM32_GPIO_Interrupt4 (void* param) // EXTI4
{
    EXTI->PR = 1 << 4;   // reset pending bit
    STM32_GPIO_ISR(4);
}

void STM32_GPIO_Interrupt5 (void* param) // EXTI5 - EXTI9
{
    UINT32 pending = EXTI->PR & 0x03E0; // pending bits 5..9
    EXTI->PR = pending;    // reset pending bits 5..9
    pending &= EXTI->IMR;  // remove masked bits
    int num = 5; pending >>= 5;
    do {
        if (pending & 1) STM32_GPIO_ISR(num);
        num++; pending >>= 1;
    } while (pending);
}

void STM32_GPIO_Interrupt10 (void* param) // EXTI10 - EXTI15
{
    UINT32 pending = EXTI->PR & 0xFC00; // pending bits 10..15
    EXTI->PR = pending;    // reset pending bits 10..15
    pending &= EXTI->IMR;  // remove masked bits
    int num = 10; pending >>= 10;
    do {
        if (pending & 1) STM32_GPIO_ISR(num);
        num++; pending >>= 1;
    } while (pending);
}

BOOL STM32_GPIO_Set_Interrupt( UINT32 pin, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE mode, BOOL GlitchFilterEnable)
{
    UINT32 num = pin & 0x0F;
    UINT32 bit = 1 << num;
    UINT32 shift = (num & 0x3) << 2; // 4 bit fields
    UINT32 idx = num >> 2;
    UINT32 mask = 0xF << shift;
    UINT32 config = (pin >> 4) << shift; // port number configuration
    
    GLOBAL_LOCK(irq);
    
    if (ISR) {
        if ((AFIO->EXTICR[idx] & mask) != config) {
            if (EXTI->IMR & bit) return FALSE; // interrupt in use
            AFIO->EXTICR[idx] = AFIO->EXTICR[idx] & ~mask | config;
        }
        g_completions[num].Abort();
        if (GlitchFilterEnable) {
            g_completions[num].SetArgument((void*)pin);
            g_debounceFlags |= bit;
        } else {
            g_debounceFlags &= ~bit;
        }
        g_ISR[num] = ISR;
        g_ISR_Param[num] = ISR_Param;
        if (mode == GPIO_INT_EDGE_HIGH
         || mode == GPIO_INT_LEVEL_HIGH
         || mode == GPIO_INT_EDGE_BOTH) {
            EXTI->RTSR |= bit;
        } else {
            EXTI->RTSR &= ~bit;
        }
        if (mode == GPIO_INT_EDGE_LOW
         || mode == GPIO_INT_LEVEL_LOW
         || mode == GPIO_INT_EDGE_BOTH) {
            EXTI->FTSR |= bit;
        } else {
            EXTI->FTSR &= ~bit;
        }
        EXTI->PR = bit; // remove pending interrupt
        EXTI->IMR |= bit; // enable interrupt
        // check for level interrupts
        if (mode == GPIO_INT_LEVEL_HIGH && CPU_GPIO_GetPinState(pin)
         || mode == GPIO_INT_LEVEL_LOW && !CPU_GPIO_GetPinState(pin)) {
            EXTI->SWIER = bit; // force interrupt
        }
    } else if ((AFIO->EXTICR[idx] & mask) == config) {
        EXTI->IMR &= ~bit; // disable interrupt
        g_ISR[num] = NULL;
    }
    return TRUE;
}


void STM32_GPIO_Pin_Config( GPIO_PIN pin, UINT32 config )
{
    GPIO_TypeDef* port = Port(pin >> 4); // pointer to the actual port registers 
    UINT32 shift = (pin & 7) << 2; // 4 bits / pin
    config <<= shift;
    UINT32 mask = 0xF << shift;
    
    GLOBAL_LOCK(irq);
    
    if (pin & 0x08) { // bit 8 - 15
        port->CRH = port->CRH & ~mask | config;
    } else { // bit 0-7
        port->CRL = port->CRL & ~mask | config;
    }
    
}


BOOL CPU_GPIO_Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();

    CPU_GPIO_SetDebounce(20); // ???
    
    for (int i = 0; i < STM32_Gpio_MaxPorts; i++) {
        g_pinReserved[i] = 0;
    }
    
    for (int i = 0; i < STM32_Gpio_MaxInt; i++) {
        g_completions[i].InitializeForISR(&STM32_GPIO_DebounceHandler);
    }
    
    EXTI->IMR = 0; // disable all external interrups;
    CPU_INTC_ActivateInterrupt(EXTI0_IRQn, STM32_GPIO_Interrupt0, 0);
    CPU_INTC_ActivateInterrupt(EXTI1_IRQn, STM32_GPIO_Interrupt1, 0);
    CPU_INTC_ActivateInterrupt(EXTI2_IRQn, STM32_GPIO_Interrupt2, 0);
    CPU_INTC_ActivateInterrupt(EXTI3_IRQn, STM32_GPIO_Interrupt3, 0);
    CPU_INTC_ActivateInterrupt(EXTI4_IRQn, STM32_GPIO_Interrupt4, 0);
    CPU_INTC_ActivateInterrupt(EXTI9_5_IRQn, STM32_GPIO_Interrupt5, 0);
    CPU_INTC_ActivateInterrupt(EXTI15_10_IRQn, STM32_GPIO_Interrupt10, 0);

    return TRUE;
}

BOOL CPU_GPIO_Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();

    for (int i = 0; i < STM32_Gpio_MaxInt; i++) {
        g_completions[i].Abort();
    }
    
    EXTI->IMR = 0; // disable all external interrups;
    CPU_INTC_DeactivateInterrupt(EXTI0_IRQn);
    CPU_INTC_DeactivateInterrupt(EXTI1_IRQn);
    CPU_INTC_DeactivateInterrupt(EXTI2_IRQn);
    CPU_INTC_DeactivateInterrupt(EXTI3_IRQn);
    CPU_INTC_DeactivateInterrupt(EXTI4_IRQn);
    CPU_INTC_DeactivateInterrupt(EXTI9_5_IRQn);
    CPU_INTC_DeactivateInterrupt(EXTI15_10_IRQn);
    
    return TRUE;
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if(pin < STM32_Gpio_MaxPins) {
        return GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT;
    }
    return GPIO_ATTRIBUTE_NONE;
}


void CPU_GPIO_DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistor, UINT32 output, GPIO_ALT_MODE alternate )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin < STM32_Gpio_MaxPins) {
        UINT32 config;
        if (alternate == STM32_GPIO_ALT_MODE_OD) config = 0xD;            // alternate open drain
        else if (output) {
            if (alternate == STM32_GPIO_ALT_MODE_OUT) config = 0x9;       // alternate output
            else if (alternate == STM32_GPIO_ALT_MODE_HIGH) config = 0xB; // alternate high speed
            else config = 0x1;                                            // general purpose output
        } else {
            if (alternate) config = 0;                                    // analog mode
            else if (resistor) config = 0x8;                              // pull up/down
            else config = 0x4;                                            // general purpose input
        }
        if (resistor) {
            CPU_GPIO_SetPinState(pin, resistor == RESISTOR_PULLUP); // pull up or down
        }
        STM32_GPIO_Pin_Config(pin, config);
        STM32_GPIO_Set_Interrupt(pin, NULL, 0, GPIO_INT_NONE, FALSE); // disable interrupt
    }
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin < STM32_Gpio_MaxPins) {
        CPU_GPIO_SetPinState(pin, initialState);
        STM32_GPIO_Pin_Config(pin, 0x1); // general purpose output
        STM32_GPIO_Set_Interrupt(pin, NULL, 0, GPIO_INT_NONE, FALSE); // disable interrupt
    }
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, GPIO_INT_EDGE edge, GPIO_RESISTOR resistor )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return CPU_GPIO_EnableInputPin2( pin, GlitchFilterEnable, ISR, 0, edge, resistor );
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE edge, GPIO_RESISTOR resistor )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin >= STM32_Gpio_MaxPins) return FALSE;
    
    UINT32 config = 0x4; // general purpose input
    if (resistor) {
        config = 0x8;    // pull up/down
        CPU_GPIO_SetPinState(pin, resistor == RESISTOR_PULLUP);
    }
    STM32_GPIO_Pin_Config(pin, config);

    if (edge > GPIO_INT_EDGE_BOTH) return FALSE;
    return STM32_GPIO_Set_Interrupt(pin, ISR, ISR_Param, edge, GlitchFilterEnable);
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin >= STM32_Gpio_MaxPins) return FALSE;
    
    GPIO_TypeDef* port = Port(pin >> 4); // pointer to the actual port registers 
    return (port->IDR >> (pin & 0xF)) & 1;
}

void CPU_GPIO_SetPinState( GPIO_PIN pin, BOOL pinState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin < STM32_Gpio_MaxPins) {
        GPIO_TypeDef* port = Port(pin >> 4); // pointer to the actual port registers 
        UINT32 bit = 1 << (pin & 0x0F);
        if (!pinState) bit <<= 16; // reset bits
        port->BSRR = bit;
    }
}

//--//

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN pin )  // busy == reserved
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin >= STM32_Gpio_MaxPins) return FALSE;
    int port = pin >> 4, sh = pin & 0x0F;
    return (g_pinReserved[port] >> sh) & 1;
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (pin >= STM32_Gpio_MaxPins) return FALSE;
    int port = pin >> 4, bit = 1 << (pin & 0x0F);
    GLOBAL_LOCK(irq);
    if (fReserve) {
        if (g_pinReserved[port] & bit) return FALSE; // already reserved
        g_pinReserved[port] |= bit;
    } else {
        g_pinReserved[port] &= ~bit;
    }
    return TRUE;
}

UINT32 CPU_GPIO_GetDebounce()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return g_debounceTicks / (SLOW_CLOCKS_PER_SECOND / 1000); // ticks -> ms
}

BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if (debounceTimeMilliseconds > 0 && debounceTimeMilliseconds < 10000) {
        g_debounceTicks = CPU_MillisecondsToTicks((UINT32)debounceTimeMilliseconds);
        return TRUE;
    }
    return FALSE;
}

INT32 CPU_GPIO_GetPinCount()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return STM32_Gpio_MaxPins;
}

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    for (int i = 0; i < size && i < STM32_Gpio_MaxPins; i++) {
         pins[i] = GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT;
    }
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return (1 << RESISTOR_DISABLED) | (1 << RESISTOR_PULLUP) | (1 << RESISTOR_PULLDOWN);
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return (1 << GPIO_INT_EDGE_LOW) | (1 << GPIO_INT_EDGE_HIGH ) | (1 << GPIO_INT_EDGE_BOTH)
         | (1 << GPIO_INT_LEVEL_LOW) | (1 << GPIO_INT_LEVEL_HIGH );
}

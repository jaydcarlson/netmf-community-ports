////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  Portions Copyright (c) GHI Electronics, LLC.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

/***************************************************************************/

//#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
//#pragma arm section zidata = "g_AT91_SPI_Driver"
//#endif 

//AT91_SPI_Driver g_AT91_SPI_Driver;

//#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
//#pragma arm section zidata
//#endif 

#define MAX_PWM_PINS 3

#define PWM_MODE_REGISTER				(*(volatile unsigned long *)0xFFFC8000)
#define PWM_ENABLE_REGISTER				(*(volatile unsigned long *)0xFFFC8004)
#define PWM_DISABLE_REGISTER			(*(volatile unsigned long *)0xFFFC8008)
#define PWM_INTERUPT_ENABLE_REGISTER	(*(volatile unsigned long *)0xFFFC8010)
#define PWM_INTERUPT_DISABLE_REGISTER	(*(volatile unsigned long *)0xFFFC8014)

#define PWM_CMR0 (*(volatile unsigned long *)0xFFFC8200)
#define PWM_CMR1 (*(volatile unsigned long *)0xFFFC8220)
#define PWM_CMR2 (*(volatile unsigned long *)0xFFFC8240)


unsigned int PMW_PinTable[MAX_PWM_PINS] = {
	AT91_GPIO_Driver::PD14, 
	AT91_GPIO_Driver::PD15, 
	AT91_GPIO_Driver::PD16}; 

volatile unsigned long *PWM_ChannelModeRegister[MAX_PWM_PINS] = {
	(volatile unsigned long *)0xFFFC8200,	// PWM_CPRD0
	(volatile unsigned long *)0xFFFC8220,	// PWM_CPRD1
	(volatile unsigned long *)0xFFFC8240};	// PWM_CPRD2

volatile unsigned long *PWM_DutyRegister[MAX_PWM_PINS] = {
	(volatile unsigned long *)0xFFFC8204,	// PWM_CDTY0
	(volatile unsigned long *)0xFFFC8224,	// PWM_CDTY1
	(volatile unsigned long *)0xFFFC8244};	// PWM_CDTY2

volatile unsigned long *PWM_PeriodRegister[MAX_PWM_PINS] = {
	(volatile unsigned long *)0xFFFC8208,	// PWM_CPRD0
	(volatile unsigned long *)0xFFFC8228,	// PWM_CPRD1
	(volatile unsigned long *)0xFFFC8248};	// PWM_CPRD2

BOOL SetPinState(PWM_CHANNEL channel, BOOL state)
{
	if(channel > MAX_PWM_PINS)
		return FALSE;

	GPIO_PIN pin = AT91_PWM_Driver::GetPinForChannel(channel);
	
	CPU_GPIO_EnableOutputPin(pin, state);

    return TRUE;
}


BOOL AT91_PWM_Driver::Initialize  (PWM_CHANNEL channel)
{
	if(channel > MAX_PWM_PINS)
		return FALSE;

	AT91_PMC &pmc = AT91::PMC();
	pmc.EnablePeriphClock(AT91C_ID_PWMC);

	if(PWM_MODE_REGISTER != PWM_MODE_REGISTER & 0x100) // Checks if clock has been set
		PWM_MODE_REGISTER = (1 << 16);
	
	PWM_ENABLE_REGISTER |= (1 << channel);
	PWM_INTERUPT_ENABLE_REGISTER |= (1 << channel);

    return SetPinState(channel, FALSE);
}

BOOL AT91_PWM_Driver::Uninitialize(PWM_CHANNEL channel)
{
	if(channel < MAX_PWM_PINS && (channel != 0))
		CPU_GPIO_DisablePin(PMW_PinTable[channel], RESISTOR_PULLUP, 0, GPIO_ALT_PRIMARY);
	
	PWM_DISABLE_REGISTER |= (1 << channel);
	PWM_INTERUPT_DISABLE_REGISTER |= (1 << channel);
    
	return TRUE;
}

BOOL AT91_PWM_Driver::ApplyConfiguration(PWM_CHANNEL channel, GPIO_PIN pin, UINT32& period, UINT32& duration, PWM_SCALE_FACTOR& scale, BOOL invert)
{
	if(channel > MAX_PWM_PINS)
		return FALSE;

	UINT32 convertedPeriod = 0;
	UINT32 convertedDuration = 0;
	UINT32 pulseBeginsOnHighEdge = 1; // Default Pulse starts on High Edge.

	UINT32 divider = 4; // Sets the default period/duration clock divider to 4 --> (MCK / divider) // MCK = 100 MHz 
	UINT32 registerDividerFlag = 0x2; // Sets the default channel clock divider to 4 --> (MCK / divider) // MCK = 100 MHz

	switch(scale)
	{
	case PWM_MILLISECONDS:
		convertedPeriod = (period * 1000000);
		convertedDuration = (duration * 1000000);
		break;

	case PWM_MICROSECONDS:
		convertedPeriod = (period * 1000);
		convertedDuration = (duration * 1000);
		break;

	case PWM_NANOSECONDS:
		convertedPeriod = period;
		convertedDuration = duration;
		break;
	}

	if(convertedPeriod > 671078400)
	{
		return FALSE;
	}
	else if(convertedPeriod > 335539200)
	{
		divider = 1024;
		registerDividerFlag = 0xA;
	}
	else if(convertedPeriod > 167769600)
	{
		divider = 512;
		registerDividerFlag = 0x9;
	}
	else if(convertedPeriod > 83884800)
	{
		divider = 256;
		registerDividerFlag = 0x8;
	}
	else if(convertedPeriod > 41942400)
	{
		divider = 128;
		registerDividerFlag = 0x7;
	}
	else if(convertedPeriod > 20971200)
	{
		divider = 64;
		registerDividerFlag = 0x6;
	}
	else if(convertedPeriod > 10485600)
	{
		divider = 32;
		registerDividerFlag = 0x5;
	}
	else if(convertedPeriod > 5242800)
	{
		divider = 16;
		registerDividerFlag = 0x4;
	}
	else if(convertedPeriod > 2621400)
	{
		divider = 8;
		registerDividerFlag = 0x3;
	}
	else if(convertedPeriod > 1310700)
	{
		divider = 4;
		registerDividerFlag = 0x2;
	}
	else if(convertedPeriod > 655350)
	{
		divider = 2;
		registerDividerFlag = 0x1;
	}
	else if(convertedPeriod > 40) // Absoulute minimum for the PWM Counter
	{
		divider = 1;
		registerDividerFlag = 0x0;
	}
	else
	{
		return FALSE;
	}

	// Flips the pulse
	if(invert == 0)
		pulseBeginsOnHighEdge = 1;
	else
		pulseBeginsOnHighEdge = 0;

	*PWM_ChannelModeRegister[channel] = (volatile unsigned long)registerDividerFlag | (pulseBeginsOnHighEdge << 9);

	*PWM_PeriodRegister[channel] = (volatile unsigned long)(convertedPeriod / (divider * 10));
	*PWM_DutyRegister[channel] = (volatile unsigned long)(convertedDuration / (divider * 10));

    return TRUE;
}

BOOL AT91_PWM_Driver::Start(PWM_CHANNEL channel, GPIO_PIN pin)
{
	CPU_GPIO_DisablePin(PMW_PinTable[channel], RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
    return TRUE;
}

void AT91_PWM_Driver::Stop (PWM_CHANNEL channel, GPIO_PIN pin)
{
	SetPinState(channel, FALSE);
}

BOOL AT91_PWM_Driver::Start(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
    return TRUE;
}

void AT91_PWM_Driver::Stop (PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
}

UINT32 AT91_PWM_Driver::PWMChannels()
{
    return MAX_PWM_PINS;
}

GPIO_PIN AT91_PWM_Driver::GetPinForChannel( PWM_CHANNEL channel )
{
	if(channel < MAX_PWM_PINS)
		return PMW_PinTable[channel];

	return GPIO_PIN_NONE;
}

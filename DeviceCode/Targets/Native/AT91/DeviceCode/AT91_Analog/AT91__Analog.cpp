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

// Maximum Analog Pins/channels
#define ANALOG_MAX_PINS 6
#define MAX_AVERAGE_AMOUNT 5

//registers
#define TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_SELECT						(*(volatile unsigned long *)0xFFFD0010) // TSADCC_CHER
#define TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_DISABLE_REGISTER				(*(volatile unsigned long *)0xFFFD0014) // TSADCC_MR
#define TOUCHSCREEN_ADC_CONTROLLER_MODE_REGISTER						(*(volatile unsigned long *)0xFFFD0004) // TSADCC_MR
#define TOUCHSCREEN_ADC_CONTROLLER_TRIGGER_REGISTER						(*(volatile unsigned long *)0xFFFD0008) // TSADCC_TRGR
#define TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_DATA_REGISTER_BASE_ADDRESS	0xFFFD0030 // TSADCC_TRGR

// Data Registers
volatile unsigned long *Analog_DataRegister[ANALOG_MAX_PINS] = {
	(volatile unsigned long *)0xFFFD0030,	// ANALOG_0
	(volatile unsigned long *)0xFFFD0034,	// ANALOG_1
	(volatile unsigned long *)0xFFFD0038,	// ANALOG_2
	(volatile unsigned long *)0xFFFD003C,	// ANALOG_3
	(volatile unsigned long *)0xFFFD0040,	// ANALOG_4
	(volatile unsigned long *)0xFFFD0044};	// ANALOG_5

//// Pins
//unsigned int AnalogPinTable[ANALOG_MAX_PINS] = {
//	AT91_GPIO_Driver::PD6, 
//	AT91_GPIO_Driver::PA20, 
//	AT91_GPIO_Driver::PA18, 
//	AT91_GPIO_Driver::PD7, 
//	AT91_GPIO_Driver::PA19, 
//	AT91_GPIO_Driver::PA17};

// Pins
unsigned int AnalogPinTable[ANALOG_MAX_PINS] = {
	AT91_GPIO_Driver::PA17,
	AT91_GPIO_Driver::PA18,
	AT91_GPIO_Driver::PA19, 
	AT91_GPIO_Driver::PA20, 
	AT91_GPIO_Driver::PD6, 
	AT91_GPIO_Driver::PD7};

//// Channel Table
//unsigned int AnalogChannelTable[ANALOG_MAX_PINS] = {
//	4, 
//	3, 
//	1, 
//	5, 
//	2, 
//	0};

BOOL AT91_Analog_Driver::Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{
	if(channel >= ANALOG_MAX_PINS)
		return FALSE;

	AT91_PMC &pmc = AT91::PMC();
	pmc.EnablePeriphClock(AT91C_ID_TSADCC);
	if(channel >= 0 && channel <= ANALOG_MAX_PINS)
	{
		pmc.EnablePeriphClock(AT91C_ID_PIOA);
		pmc.EnablePeriphClock(AT91C_ID_PIOD);

		CPU_GPIO_DisablePin(AnalogPinTable[channel], RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
	}
	else
	{
		return CLR_E_PIN_UNAVAILABLE;
	}

	TOUCHSCREEN_ADC_CONTROLLER_CHANNEL_SELECT = (1 << channel);//AnalogChannelTable[channel]); // Selects the channel in the register
	// Explanation of the TOUCHSCREEN_ADC_CONTROLLER_MODE_REGISTER
	//
	// PENDET: Pen Detect Selection
	// 0: Disable the Touch screen pins as analog inputs
	// 1: enable the Touch screen pins as analog inputs
	//
	// PRESCAL: Prescaler Rate Selection
	//    ADCCLK = MCK / ( (PRESCAL+1) * 2 )   <-- MCK = 100 MHz
	//
	// SHTIM: Sample & Hold Time for ADC Channels
	//    Programming 0 for SHTIM gives a Sample & Hold Time equal to (2/ADCCLK).
	//       Sample & Hold Time = (SHTIM+1) / ADCCLK
	TOUCHSCREEN_ADC_CONTROLLER_MODE_REGISTER = (1 << 6) /*PENDET*/| (63 << 8) /*PRESCAL*/ | (0xf << 24) /*SHTIM*/;
	TOUCHSCREEN_ADC_CONTROLLER_TRIGGER_REGISTER = 6;
    
	return TRUE;
}

INT32 averageMiddleThreeValues(unsigned long *arrayToSort)
{
	unsigned long valueToReturn;
	int middleArrayPossition = MAX_AVERAGE_AMOUNT / 2;
	valueToReturn = arrayToSort[middleArrayPossition - 1];
	valueToReturn += arrayToSort[middleArrayPossition];
	valueToReturn += arrayToSort[middleArrayPossition + 1];
	return (valueToReturn / 3);
}

void swapArrayValues_Ascending(unsigned long *arrayToSort, int smallNumberArrayPosition, int largeNumberArrayPosition)
{
	unsigned long smallNumber = arrayToSort[smallNumberArrayPosition];
	unsigned long largeNumber = arrayToSort[largeNumberArrayPosition];
	arrayToSort[smallNumberArrayPosition] = largeNumber;
	arrayToSort[largeNumberArrayPosition] = smallNumber;
}

void sortArray(unsigned long *arrayToSort)
{
	bool isArraySorted = FALSE;
	int arrayCount;

	while(!isArraySorted)
	{
		arrayCount = 0;

		for(int x = 0; x < (MAX_AVERAGE_AMOUNT - 1); x++)
		{
			if(arrayToSort[x] < arrayToSort[x + 1])
				swapArrayValues_Ascending(arrayToSort, x, (x + 1));
			else
				arrayCount++;
		}

		if(arrayCount == (MAX_AVERAGE_AMOUNT - 1))
			isArraySorted = TRUE;
	}
}

INT32 AT91_Analog_Driver::Read( ANALOG_CHANNEL channel )
{
	INT32 total = 0;
	unsigned long arrayValuesToSort[MAX_AVERAGE_AMOUNT];
	unsigned long valueToStoreInArray;

	// get the values
	for(int i = 0; i < MAX_AVERAGE_AMOUNT; i++)
	{
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(5);
		//return *Analog_DataRegister[channel];
		//total += *Analog_DataRegister[channel];
		valueToStoreInArray = *Analog_DataRegister[channel];
		arrayValuesToSort[i] = valueToStoreInArray;
	}

	sortArray(arrayValuesToSort);

	return averageMiddleThreeValues(arrayValuesToSort);
	//total /= MAX_AVERAGE_AMOUNT;

	//return total;
}

UINT32 AT91_Analog_Driver::ADChannels()
{
    return ANALOG_MAX_PINS;
}

GPIO_PIN AT91_Analog_Driver::GetPinForChannel( ANALOG_CHANNEL channel )
{
	if(channel < ANALOG_MAX_PINS)
		return AnalogPinTable[channel];
	return GPIO_PIN_NONE;
}

BOOL AT91_Analog_Driver::GetAvailablePrecisionsForChannel( ANALOG_CHANNEL channel, INT32* precisions, UINT32& size )
{
	precisions[0] = 10;
	precisions[1] = 10;
	precisions[2] = 10;
	precisions[3] = 10;
	precisions[4] = 10;
	precisions[5] = 10;
	size = 6;
	return TRUE;
}

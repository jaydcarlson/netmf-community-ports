#ifndef _OSHW_CONFIGURATION_H_
#define _OSHW_CONFIGURATION_H_

#include "ThirdParty\GHI\include\GHI_OSHW_HAL.h"
// LCD //
struct OSHW_Configuration_LCD_Structure
{
	UINT32 Width;
    UINT32 Height;

	BOOL PriorityEnable; // Not implemented

	BOOL OutputEnableIsFixed;
	BOOL OutputEnablePolarity;

	BOOL HorizontalSyncPolarity;
	BOOL VerticalSyncPolarity;
	BOOL PixelPolarity;

    UINT8 HorizontalSyncPulseWidth;
    UINT8 HorizontalBackPorch;
    UINT8 HorizontalFrontPorch;
    UINT8 VerticalSyncPulseWidth;
    UINT8 VerticalBackPorch;
    UINT8 VerticalFrontPorch;

	UINT32 PixelClockRateKHz;

	UINT32 Magic; //If not 0xdeadbeef, configuration will not load

	UINT8 reserved;		// not currently used but keeps it aligned
	UINT8 reserved2;		// not currently used but keeps it aligned
};

#define OSHW_Configurations_Structure_SIZE	128

struct OSHW_Configurations_Structure
{
	HAL_DRIVER_CONFIG_HEADER header; // 4 Bytes

	OSHW_Configuration_LCD_Structure lcd; // 40 bytes 44 bytes with Magic // Now 48 with new PixelClockRateKHz and extra reserved byte.

	UINT8 resserved[76]; // substact from this when you add to keep at 128 // Was 80 without the added 4 bytes.
};

extern OSHW_Configurations_Structure		OSHW_Configurations;
extern OSHW_Configuration_LCD_Structure	OSHW_Configuration_LCD;
//////////

void OSHW_Configuration_Initialize();
BOOL OSHW_Configuration_Read();
BOOL OSHW_Configuration_Write();

GPAL_ERROR OSHW_Configuration_LCD_Set(OSHW_Configuration_LCD_Structure *config, BOOL *didChange);

#endif // _OSH_GPAL_CONFIG_H_
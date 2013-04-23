#include <tinyhal.h>
#include "ThirdParty\GHI\include\OSHW_Configuration.h"

#define OSHW_CONFIG_DRIVER_NAME		"OSHW_CONFIG"
#define DEFAULT_VALUE					0xFF

#ifdef FEZ_CERBERUS
OSHW_Configurations_Structure		OSHW_Configurations;
#endif

BOOL OSHW_Configuration_Read()
{
	return HAL_CONFIG_BLOCK::ApplyConfig(OSHW_CONFIG_DRIVER_NAME, &OSHW_Configurations, sizeof(OSHW_Configurations_Structure));
}

BOOL OSHW_Configuration_Write()
{
	return HAL_CONFIG_BLOCK::UpdateBlockWithName(OSHW_CONFIG_DRIVER_NAME, &OSHW_Configurations, sizeof(OSHW_Configurations_Structure), TRUE);
}

GPAL_ERROR OSHW_Configuration_LCD_Set(OSHW_Configuration_LCD_Structure *config, BOOL *didChange)
{
	//OSHW_GPAL_Configuration_LCD_Structure configToWrite = *config;
	*didChange = FALSE;

	//OSHW_Configuration_Read();
	if(OSHW_Configuration_Read() == FALSE) //OSHW_Configurations.lcd.reserved != 0xff)
	{
		memset(&OSHW_Configurations, 0xFF, sizeof(OSHW_Configurations_Structure));
		OSHW_Configurations.header.Enable = TRUE;
	//	OSHW_GPAL_Configurations.lcd.reserved = 0xff;
		OSHW_Configuration_Write();
	}

	//pointer_OSH_GPAL_Configurations = &OSH_GPAL_Configurations;
	//OSH_GPAL_Configurations.lcd = *config;

	//if(!GHAL_LCD_IsConfigOK(config))
	//	return CLR_E_INVALID_PARAMETER;
	//if(pointer_OSHW_GPAL_Configurations == NULL)
	//	OSHW_GPAL_Configuration_Write();
	//OSHW_GPAL_Configuration_Read();
	//if(!OSHW_GPAL_Configurations.lcd.PriorityEnable)
	//	OSHW_GPAL_Configuration_Write();


	if(!OSHW_Configuration_Read())
		return -1; //GHAL_E_SYS;

	// did it change?
	if(memcmp(config, &OSHW_Configurations.lcd, sizeof(OSHW_Configuration_LCD_Structure)))
	{
		memcpy(&OSHW_Configurations.lcd, config, sizeof(OSHW_Configuration_LCD_Structure));
		
		//OSHW_GPAL_Configurations.lcd.Width						= 0xff;//configToWrite.Width;
		//OSHW_GPAL_Configurations.lcd.Height						= 0xff;//configToWrite.Height;
		//OSHW_GPAL_Configurations.lcd.PixelPolarity				= 0xff;//configToWrite.PixelPolarity;
		//OSHW_GPAL_Configurations.lcd.VerticalSyncPolarity		= 0xff;//configToWrite.VerticalSyncPolarity;
		//OSHW_GPAL_Configurations.lcd.HorizontalSyncPolarity		= 0xff;//configToWrite.HorizontalSyncPolarity;
		//OSHW_GPAL_Configurations.lcd.OutputEnablePolarity		= 0xff;//configToWrite.OutputEnablePolarity;
		//OSHW_GPAL_Configurations.lcd.PixelClockDivider			= 0xff;//configToWrite.PixelClockDivider;
		//OSHW_GPAL_Configurations.lcd.HorizontalSyncPulseWidth	= 0xff;//configToWrite.HorizontalSyncPulseWidth;
		//OSHW_GPAL_Configurations.lcd.HorizontalBackPorch			= 0xff;//configToWrite.HorizontalBackPorch;
		//OSHW_GPAL_Configurations.lcd.HorizontalFrontPorch		= 0xff;//configToWrite.HorizontalFrontPorch;
		//OSHW_GPAL_Configurations.lcd.VerticalSyncPulseWidth		= 0xff;//configToWrite.VerticalSyncPulseWidth;
		//OSHW_GPAL_Configurations.lcd.VerticalBackPorch			= 0xff;//configToWrite.VerticalBackPorch;
		//OSHW_GPAL_Configurations.lcd.VerticalFrontPorch			= 0xff;//configToWrite.VerticalFrontPorch;
  //      //  OSHW_GPAL_Configurations.lcd = *config;
		//OSHW_GPAL_Configurations.header.Enable = TRUE;

		if(!OSHW_Configuration_Write())
			return -1; //GHAL_E_SYS;
		//OSHW_GPAL_Configuration_Read();		
		//LCD_Uninitialize();
		*didChange = TRUE;
		//LCD_Initialize();
	}

	return S_OK;
}

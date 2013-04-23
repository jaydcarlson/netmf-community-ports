//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "HAL.h"
#include "HAL_GHI_OSHW_Hardware_LCDController.h"
#include "ThirdParty\GHI\include\OSHW_Configuration.h"

using namespace GHI::OSHW::Hardware;

INT8 LCDController::Set( UINT32 Width, UINT32 Height, INT8 PriorityEnable, INT8 OutputEnableIsFixed, INT8 OutputEnablePolarity, INT8 HorizontalSyncPolarity, INT8 VerticalSyncPolarity, INT8 PixelPolarity, UINT8 HorizontalSyncPulseWidth, UINT8 HorizontalBackPorch, UINT8 HorizontalFrontPorch, UINT8 VerticalSyncPulseWidth, UINT8 VerticalBackPorch, UINT8 VerticalFrontPorch, UINT32 PixelClockRateKHz, HRESULT &hr )
{
	BOOL didChange;

	if (((Width * Height) > (800 * 600)) || (Width < 64) ||  (Height < 64))
	{
		hr = CLR_E_OUT_OF_RANGE;
		return FALSE;
	}
   


	OSHW_Configuration_LCD_Structure config;

	config.Width = Width;
    config.Height = Height;

	config.PriorityEnable = PriorityEnable; // Not implemented Used to determine if LCD Config is present in config sector

	config.OutputEnableIsFixed = OutputEnableIsFixed;
	config.OutputEnablePolarity = OutputEnablePolarity;

	config.HorizontalSyncPolarity = HorizontalSyncPolarity;
	config.VerticalSyncPolarity = VerticalSyncPolarity;
	config.PixelPolarity = PixelPolarity;

    config.HorizontalSyncPulseWidth = HorizontalSyncPulseWidth;
    config.HorizontalBackPorch = HorizontalBackPorch;
    config.HorizontalFrontPorch = HorizontalFrontPorch;
    config.VerticalSyncPulseWidth = VerticalSyncPulseWidth;
    config.VerticalBackPorch = VerticalBackPorch;
    config.VerticalFrontPorch = VerticalFrontPorch;

	config.PixelClockRateKHz = PixelClockRateKHz;

	config.Magic = 0xdeadbeef;

	config.reserved = 0x09;
	config.reserved2 = 0x15;

	hr = OSHW_Configuration_LCD_Set(&config, &didChange);

	return didChange;
}


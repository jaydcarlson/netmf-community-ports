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
#include "HAL_GHI_OSHW_Hardware_Util.h"

#include <ThirdParty\GHI\Include\GHI_OSHW_PAL.h>

using namespace GHI::OSHW::Hardware;
//UINT32 mask_channel=0;
//INT32 resMax=0;
#define ANALOG_OUT_SCALE_VALUE_MIN 0
#define ANALOG_OUT_SCALE_VALUE_MAX 0xFF
void Util::BitmapConvertBPP( CLR_RT_TypedArray_UINT8 bitmap, CLR_RT_TypedArray_UINT8 output, UINT8 bpp, HRESULT &hr )
{
	hr = OSHW_PAL_Bitmap_ConvertBPP(bitmap.GetBuffer(), bitmap.GetSize(), output.GetBuffer(), output.GetSize(), bpp);
}
void Util::PlayPCMAudio( UINT8 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, INT32 param3, INT32 param4, HRESULT &hr )
{
	int i; 
	UINT32 delay = 1000000 / param4 ;
  INT32 resMax=0;
	BYTE *buffer = param1.GetBuffer() + param2;
	INT32 value;
	DA_CHANNEL channel = (DA_CHANNEL)param0;	 
  //delay = delay+(delay/4);
	//if ((mask_channel & (1<<param0) )==0) // Initialize one time 
	{
		UINT32 size=0;
		INT32  precisions[1];
    
		if (DA_GetAvailablePrecisionsForChannel(channel,precisions,size)== FALSE)
		{
			hr = CLR_E_INVALID_OPERATION;
			return;
		}
		if (DA_Initialize(channel,precisions[0])== FALSE)
		{
			hr = CLR_E_INVALID_OPERATION;
			return;
		}
		for (i=0; i< precisions[0]; i++)
		{
			resMax = (resMax<<1) | 1;
		}   
		//mask_channel |= (1<<param0);
	}

	for(i = 0; i < param3; i++)
	{
		value = (resMax * (buffer[i] - ANALOG_OUT_SCALE_VALUE_MIN)) / (ANALOG_OUT_SCALE_VALUE_MAX - ANALOG_OUT_SCALE_VALUE_MIN);	
		DA_Write( channel, value );			
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(delay);
	}
}


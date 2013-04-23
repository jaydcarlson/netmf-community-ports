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
#include "HAL_GHI_Gameo_Display.h"
#include "..\..\..\Drivers\FSMC\FT1505_LCD\LCD.h"


using namespace GHI::Gameo;

#define PORTRAIT_MODE 0
#define LANDSCAPE_MODE 1
#define ORG_SCREEN_WIDTH 240
#define ORG_SCREEN_HEIGHT 320



UINT16 screen_width;
UINT16 screen_height;
UINT8 screen_mode=PORTRAIT_MODE;

#define PERIPH_BB_BASE        ((UINT32)0x42000000) /*!< Peripheral base address in the bit-band region */
#define PERIPH_BASE           ((UINT32)0x40000000) /*!< Peripheral base address in the alias region */
#define APB1PERIPH_BASE       PERIPH_BASE
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))
void PWR_WakeUpPinCmd(BOOL value)
{
  *(volatile UINT32 *) CSR_EWUP_BB = (UINT32)value;
}
INT8 Rotate90Degree( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT8* param3 )
{
	UINT16 w = param1*2;
	UINT16 h = param2;
	int i,j,cnt=0;  
	for (j = 0; j < w; j += 2)
	{
		for (i = (h - 1); i >= 0; i--)
		{
			param3[cnt++] =param0[i * w + j];
			param3[cnt++] = param0[i * w + j + 1];
		}
	}
    return TRUE;
}
INT8 Display::Initialize( UINT8 param0, UINT16 param1, UINT16 param2, HRESULT &hr )
{
	screen_mode = param0;
	screen_width = param1;
	screen_height =  param2;
	LCD_PinConfig();
	LCD_Init();
	PWR_WakeUpPinCmd(TRUE);
	return TRUE;
}

INT8 Display::WriteRaw( CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr )
{
	INT32 i;
  INT32 j=0;
	UINT16 data;
	INT32 len = param1 + param2;
	for (i=param1;i<len; i+=2)
	{

    data = ((param0[i]<<8) | (param0[i+1]));

		LCD_WriteData(data);
    
	}
	return TRUE;
}

INT8 Display::WriteRaw( CLR_RT_TypedArray_UINT16 param0, INT32 param1, INT32 param2, HRESULT &hr )
{
	INT32 i;
	INT32 len = param1 + param2;
	for (i=param1;i<len; i++)
	{
		LCD_WriteData(param0[i]);
	}
	return TRUE;
}

INT8 Display::WriteRegister( UINT16 param0, UINT16 param1, HRESULT &hr )
{
	LCD_WriteRegister(param0,param1);
	return TRUE;
}
INT8 Display::WriteRegister( UINT16 param0, HRESULT &hr )
{
   LCD_SetRegister(param0);
   return TRUE;
}
INT8 Display::FillRect( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT16 param3, UINT16 param4, UINT16 param5, HRESULT &hr )
{
	int i=0;
	int pixelCount = param3 * param4;
	int x = param1;
	int y = param2;
	int width = param3;
	int height = param4;
  UINT16 data;
	 // fill buffer
	param0[0] = (UINT8)(param5 >> 8);
	param0[1] = (UINT8)(param5);
	data = (UINT16)((param0[0]<<8) | (param0[1]));
	if (screen_mode!=PORTRAIT_MODE)
	{
		x = ORG_SCREEN_WIDTH-param2-param4; 
		y = param1;
		width = param4;
		height = param3;
	}
	
	LCD_SetDrawingWindow(x, y,width, height);
	for (i = 0; i< pixelCount; i++)
	{
		LCD_WriteData(data);
	}
	return TRUE;
}

INT8 Display::DrawImage( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT16 param3, UINT16 param4, UINT16 param5, UINT16 param6, UINT16 param7, UINT16 param8, HRESULT &hr )
{
	UINT16 x_des = param1;
	UINT16 y_des = param2;
	UINT16 width_org = param3;
	UINT16 height_org = param4;
	UINT16 x_src=param5;
	UINT16 y_src=param6;
	UINT16 width = param7;
	UINT16 height = param8;
	int len = (width*height)<<1; // RGB_565 format
	int i;
	int j;
	UINT16 data;
	
	if (screen_mode==PORTRAIT_MODE)
	{	
		// check if pos + offset is out of screen
		// if (x_des +width> screen_width ) return FALSE;
		// if (y_des + height > screen_height) return FALSE;   
		if (
			x_src == 0 && 
			y_src == 0 &&
			width_org == width &&
			height_org == height
			) // Draw full image
		{
			LCD_SetDrawingWindow(x_des, y_des, width,height);
			for (i=0; i< len; i+=2)
			{
				data = ((param0[i]<<8) | (param0[i+1]));
				LCD_WriteData(data);
			} 
			return TRUE;
		}
		else 
		{
			// Draw a piece of image
			if ((x_src +width) <= width_org && (y_src + height) <= height_org)
			{
				if (x_src%2!=0) x_src--;
				LCD_SetDrawingWindow(x_des,y_des,width,height);
				for (i=y_src; i<y_src+ height;i++)
				{
					//for(j=x_src*2;j<(x_src*2+width*2);j+=2)
					for(j=(x_src<<1);j<((x_src<<1)+(width<<1));j+=2)
					{

					  data = ((param0[i*width_org*2 +j]<<8) | (param0[i*width_org*2 +j+1]));
					  LCD_WriteData(data);
					}
				}
				return TRUE;
			}
			else 
			{
				return FALSE;
			}
		}
	}
	else 
	{
		UINT8 *img_90rotate = ( UINT8*) private_malloc (width_org*height_org*2);
		//if (img_90rotate!=null)
		{
		  Rotate90Degree(param0,width_org,height_org,img_90rotate);

		  // Update x, y, width, heigh follow new image
		  x_des = ORG_SCREEN_WIDTH- param2-(height);
		  y_des = param1;
		  width_org = param4;
		  height_org = param3;
		  width = param8;
		  height = param7;
		  y_src = param5;
		  x_src =width_org -param6- width;
		  // check if pos + offset is out of screen
		  // if (x_des +width> screen_height ) return FALSE;
		  // if (y_des + height > screen_width) return FALSE;

		  if (
		  x_src == 0 && 
		  y_src == 0 &&
		  width_org == width &&
		  height_org == height
		  ) // Draw full image
		  {
			LCD_SetDrawingWindow(x_des, y_des, width,height);
			for (i=0; i< len; i+=2)
			{
			  data = ((img_90rotate[i]<<8) | (img_90rotate[i+1])) ;
			  LCD_WriteData(data);
			}    		
		  }
		  else 
		  {
			// Draw a piece of image
			if ((x_src +width) <= width_org && (y_src + height) <= height_org)
			{
			  if (x_src%2!=0) x_src--;
			  LCD_SetDrawingWindow(x_des,y_des,width,height);
			  for (i=y_src; i<y_src+ height;i++)
			  {
				//for(j=x_src*2;j<(x_src*2+width*2);j+=2)
				for(j=(x_src<<1);j<((x_src<<1)+(width<<1));j+=2)
				{
				  data = ((img_90rotate[i*width_org*2 +j]<<8) | (img_90rotate[i*width_org*2 +j+1]))   ;
				  LCD_WriteData(data);
				}
			  }
			}
		  }
		}	
		private_free(img_90rotate); 
	}
	return TRUE;
}









/////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
/////////////////////////////////////////////
#include <GHI\Include\GHI_OSHW_HAL.h>
#include <tinyhal.h>

#define REGISTER  (*(UINT16*) 0x60000000)
#define COMMAND	(*(UINT16*) 0x60020000) 
#define REGISTER_WRITE_GRAM  0x22
#define REGISTER_HORIZONTAL_ADDRESS 0x20
#define REGISTER_VERTICAL_ADDRESS	0x21
#define REGISTER_HORIZONTAL_START_ADDRESS	0x50
#define REGISTER_HORIZONTAL_END_ADDRESS	0x51
#define REGISTER_VERTICAL_START_ADDRESS	0x52
#define REGISTER_VERTICAL_END_ADDRESS	0x53

void LCD_WriteData(UINT16 value);
void LCD_WriteRegister(UINT16 reg, UINT16 value);
void LCD_SetRegister(UINT16 reg);
void LCD_Init();
void LCD_PinConfig(void);
void LCD_SetPixelAddress(UINT16 x, UINT16 y);
void LCD_SetDrawingWindow(UINT16 x, UINT16 y, UINT16 width, UINT16 height);

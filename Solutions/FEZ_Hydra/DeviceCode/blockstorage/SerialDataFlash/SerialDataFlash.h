////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
//
// Portions Copyright (c) GHI Electronics, LLC.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SerialDataFlash_H_1
#define _SerialDataFlash_H_1   1

#define cmndAddress 0x40400000
#define addrAddress 0x40200000
#define dataAddress 0x40000000



/// Atmel DataFlash commands
#define COMMAND_READID							0x9F
#define COMMAND_READ_STATUS_REGISTER			0xD7
#define COMMAND_WRITE_BUFFER_1					0x84
#define COMMAND_WRITE_BUFFER_2					0x87
#define COMMAND_WRITE_BUFFER_1_TO_MEMORY		0x83
#define COMMAND_WRITE_BUFFER_2_TO_MEMORY		0x86
#define COMMAND_READ_FROM_MAIN_MEMORY_LEGACY	0xE8
#define COMMAND_READ_FROM_MAIN_MEMORY_DIRECT	0xD2
#define COMMAND_PAGE_ERASE						0x81

struct SerialDataFlash_BS_Driver
{
    typedef UINT8 CHIP_WORD;

    static BOOL ChipInitialize( void* context );

    static BOOL ChipUnInitialize( void* context );
    
    static BOOL ReadProductID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode );

    static const BlockDeviceInfo* GetDeviceInfo( void* context);
    
    static BOOL Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff );
    static BOOL Write( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite );
    static BOOL Memset( void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes );
    static BOOL GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* sectorMetadata);
    static BOOL IsBlockErased( void* context, ByteAddress BlockStartAddress, UINT32 BlockLength );
    static BOOL EraseBlock( void* context, ByteAddress Sector );
    static void SetPowerState( void* context, UINT32 State );
    static UINT32 MaxSectorWrite_uSec( void* context );
    static UINT32 MaxBlockErase_uSec( void* context );

private:

    static BOOL WriteX( void* context, ByteAddress StartSector, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr );
    static void WaitReady();
    
	/*static void EnableCE()
	{
		CPU_GPIO_SetPinState(AT91_NAND_CE, FALSE);
	};
    
	static void DisableCE()
	{
		CPU_GPIO_SetPinState(AT91_NAND_CE, TRUE);
	};
    
	static void WriteCommand(UINT8 command)
	{
		*((volatile UINT8 *)cmndAddress) = command;
	};
    
	static void WriteAddress(UINT8 Address)
	{
		*((volatile UINT8 *)addrAddress) = Address;
	};
    
	static void WriteData8(UINT8 Data)
	{
		*((volatile UINT8 *)dataAddress) = Data;
	};
    
	static UINT8 ReadData8()
	{
		return  (*((volatile UINT8 *)dataAddress));
	};
    
	static void ReadPage(ByteAddress StartSector, BYTE *outBuf, BYTE* inBuf, UINT32 size, UINT32 offset, UINT32 sectorSize, BOOL fIncrementDataPtr);
     */
	static unsigned char getSPI_Status();

	static BOOL writeSector(unsigned int pageNumber, BYTE *dataBuffer);
	static BOOL readSector(unsigned int pageNumber, unsigned int pageOffset, unsigned int numberOfBytesToRead);

    

};


#endif


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

#include <tinyhal.h>
#include "..\SerialDataFlash.h"


//****************************************************************************************************
//********************************** SPI_CONFIGURATION Reference *************************************
//****************************************************************************************************
// From CPU_SPI_decl.h
//struct SPI_CONFIGURATION
//{
//    GPIO_PIN       DeviceCS;
//    BOOL           CS_Active;             // False = LOW active,      TRUE = HIGH active
//    BOOL           MSK_IDLE;              // False = LOW during idle, TRUE = HIGH during idle
//    BOOL           MSK_SampleEdge;        // False = sample falling edge,  TRUE = samples on rising
//    BOOL           MD_16bits;
//    UINT32         Clock_RateKHz;
//    UINT32         CS_Setup_uSecs;
//    UINT32         CS_Hold_uSecs;
//    UINT32         SPI_mod;
//    GPIO_FLAG      BusyPin;
//};
//
//****************************************************************************************************
//************************************* GPIO_FLAG Reference ******************************************
//****************************************************************************************************
// From tinyhal.h
//struct GPIO_FLAG
//{
//    GPIO_PIN  Pin;
//    BOOL      ActiveState;
//};
//
//****************************************************************************************************

SPI_CONFIGURATION spiConfig = { 28, false, false, true, false, 20000, 0, 0, 0, {GPIO_PIN_NONE, FALSE} };

#define WRITE_COMMAND_SIZE						4
#define FLASH_MANUFACTURER_CODE                 0x1F
#define FLASH_DEVICE_CODE                       0x27 // For 4MB Chip

#define FLASH_BLOCK_COUNT                       8192

#define FLASH_BASE_ADDRESS                      0x00000000
#define FLASH_SECTOR_PER_BLOCK                  1 // 8 pages/sectors per block If full chip is used
#define FLASH_SECTOR_SIZE                       528 //512
#define FLASH_BLOCK_SIZE                        (FLASH_SECTOR_PER_BLOCK*FLASH_SECTOR_SIZE)
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC     15000 // <-Set for the time for a page/sector erase. || This is for block erase-> 45000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC    17000
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC         35000 // <-Set for the time for a page/sector erase. || This is for block erase-> 100000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC        40000

#define sectorFlashSize 16
//#define FLASH_SIZE								0x100000 // = 1 MB or 1024 KB *****  0x800000 8 MB or 8192 KB If full chip is used

#define TIMEOUT				10000

//--//



unsigned char DataflashTempBuffer[(FLASH_BLOCK_SIZE*16) + WRITE_COMMAND_SIZE];
unsigned char commandAndDataBuffer[FLASH_BLOCK_SIZE + WRITE_COMMAND_SIZE];//this should be deleted!
//unsigned char dataBuffer[FLASH_BLOCK_SIZE];

extern SerialDataFlash_BS_Driver g_SerialDataFlash_BS_Driver;

BOOL SerialDataFlash_BS_Driver::ChipInitialize( void* context )
{
     //lcd_printf( "ChipInitialize\r\n" );
     NATIVE_PROFILE_PAL_FLASH();
     
     unsigned char command[5];
     
     command[0] = COMMAND_READID;
     command[1] = 0x00;
     command[2] = 0x00;
     command[3] = 0x00;
     command[4] = 0x00;
     
     CPU_SPI_nWrite8_nRead8(spiConfig, command, 5, command, 5, 0);

     if (FLASH_MANUFACTURER_CODE != command[1])
     {
          lcd_printf( "Flash_ChipInitialize: ManufacturerCode failure!\r\n" );
          return FALSE;
     }
     
     if (FLASH_DEVICE_CODE != command[2])
     {
          lcd_printf( "Flash_ChipInitialize: DeviceCode failure!\r\n" );
          return FALSE;		
     }
     
     getSPI_Status();

	 return TRUE;
}

BOOL SerialDataFlash_BS_Driver::ChipUnInitialize( void* context )
{
    return TRUE;
}

const BlockDeviceInfo* SerialDataFlash_BS_Driver::GetDeviceInfo( void* context)
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;
    
    return config->BlockDeviceInformation;    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL SerialDataFlash_BS_Driver::Read( void* context, ByteAddress byteAddress, UINT32 NumberOfBytesToRead, BYTE * pointerToReadBuffer )
{
#if 0
memset (pointerToReadBuffer,0xff,NumberOfBytesToRead);
#else
	NATIVE_PROFILE_PAL_FLASH();
	GLOBAL_LOCK(irq);

	unsigned int pageNumber = 0;
	unsigned int pageOffset = 0;

	pageNumber = byteAddress / FLASH_BLOCK_SIZE;
	pageOffset = byteAddress % FLASH_BLOCK_SIZE;

	unsigned int temp = 0;
	temp = pageOffset;
	temp |= pageNumber << 10;

	unsigned char command[8];
	command[0] = COMMAND_READ_FROM_MAIN_MEMORY_LEGACY;
	command[1] = temp >> 16;
	command[2] = temp >> 8;
	command[3] = temp;
	command[4] = 0x00;
	command[5] = 0x00;
	command[6] = 0x00;
	command[7] = 0x00;

	CPU_SPI_nWrite8_nRead8(spiConfig, command, 8, pointerToReadBuffer, NumberOfBytesToRead, 8);

	for (int timeout = 0; timeout < TIMEOUT; timeout++)
	{
		if (getSPI_Status() & 0x80)
			return TRUE;
	}

	return FALSE;
/*	unsigned int pageNumber = 0;
	unsigned int pageOffset = 0;

	pageNumber = byteAddress / FLASH_BLOCK_SIZE;
	pageOffset = byteAddress % FLASH_BLOCK_SIZE;

	unsigned int beginningDataStubSize = 0;
	unsigned int totalComplete512BytePagesToRead = 0;
	unsigned int remainderOfBytesFromLastPage = 0;
	unsigned int endingDataStubSize = 0;

	beginningDataStubSize = FLASH_BLOCK_SIZE - pageOffset;
	totalComplete512BytePagesToRead = (NumberOfBytesToRead - beginningDataStubSize) / FLASH_BLOCK_SIZE;
	remainderOfBytesFromLastPage = (NumberOfBytesToRead - beginningDataStubSize) % FLASH_BLOCK_SIZE;
	endingDataStubSize = remainderOfBytesFromLastPage;

	unsigned int totalPagesToRead = 0;
	totalPagesToRead = totalComplete512BytePagesToRead;
	if(beginningDataStubSize > 0)
		totalPagesToRead++;
	if(endingDataStubSize > 0)
		totalPagesToRead++;

	unsigned int currentPageInBuffer = 0;
	unsigned int currentPageNumber = pageNumber;
	unsigned int currentPageOffset = pageOffset;

	if(beginningDataStubSize < FLASH_BLOCK_SIZE)
	{
		for(int xyz = 0; xyz < FLASH_BLOCK_SIZE; xyz++)
		{
			dataBuffer[xyz] = 0xff;
		}

		dataBuffer = readSector(currentPageNumber, currentPageOffset, beginningDataStubSize);

		memcpy(&pointerToReadBuffer[FLASH_BLOCK_SIZE * currentBufferPage++], dataBuffer, beginningDataStubSize);
		
		currentPageNumber++;

		for (int timeout = 0; timeout < TIMEOUT; timeout++)
		{
			if (getSPI_Status() & 0x80)
				return TRUE;
		}
	}

	for(currentPageNumber; currentPageNumber < totalComplete512BytePagesToRead + pageNumber; currentPageNumber++)
	{
		for(int xyz = 0; xyz < FLASH_BLOCK_SIZE; xyz++)
		{
			dataBuffer[xyz] = 0xff;
		}
		
		dataBuffer = readSector(currentPageNumber, currentPageOffset, FLASH_BLOCK_SIZE);

		memcpy(&pointerToReadBuffer[beginningDataNubSize + (FLASH_BLOCK_SIZE * currentPageInBuffer++)], dataBuffer, endingDataStubSize);

		currentPageNumber++;

		for (int timeout = 0; timeout < TIMEOUT; timeout++)
		{
			if (getSPI_Status() & 0x80)
				return TRUE;
		}
	}

	if(remainderOfBytesFromLastPage > 0)
	{
		for(int xyz = 0; xyz <= FLASH_BLOCK_SIZE; xyz++)
		{
			dataBuffer[xyz] = 0xff;
		}

		dataBuffer = readSector(currentPageNumber, currentPageOffset, beginningDataStubSize);

		memcpy(&pointerToReadBuffer[beginningDataNubSize + (FLASH_BLOCK_SIZE * currentPageInBuffer++)], dataBuffer, beginningDataStubSize);

	}

	for (int timeout = 0; timeout < TIMEOUT; timeout++)
	{
		if (getSPI_Status() & 0x80)
			return TRUE;
	}*/
///////////////
	
	//lcd_printf("R: %d\r\n", currentPageNumber);
 //         
 //         for (int timeout = 0; timeout < TIMEOUT; timeout++)
 //         {
 //              //df_timeout++;
 //              if (getSPI_Status() & 0x80)
 //                   break;
 //         }
 //    }

     //lcd_printf("R: %d\r\n", pageNumber);
 //    lcd_printf("Number Of Bytes To Read: %d\r\n", NumberOfBytesToRead);

 //    if(remainderOfBytesFromUnevenPages > 0)
 //    {
 //         temp = pageOffset;
 //         temp |= currentPageNumber << 10;
 //         command[0] = COMMAND_READ_FROM_MAIN_MEMORY_LEGACY;
 //         command[1] = temp >> 16;
 //         command[2] = temp >> 8;
 //         command[3] = temp;
 //         command[4] = 0x00;
 //         command[5] = 0x00;
 //         command[6] = 0x00;
 //         command[7] = 0x00;

 //         if(pageOffset > 0)
 //         {
 //              CPU_SPI_nWrite8_nRead8(spiConfig, command, 8, dataBuffer/*pointerToReadBuffer*/, NumberOfBytesToRead, 8);
 //              memcpy(pointerToReadBuffer, dataBuffer, NumberOfBytesToRead);
 //         }
 //         else
 //         {
 //              CPU_SPI_nWrite8_nRead8(spiConfig, command, 8, dataBuffer/*pointerToReadBuffer*/, remainderOfBytesFromUnevenPages, 8);
 //              memcpy(&pointerToReadBuffer[FLASH_BLOCK_SIZE * currentBufferPage++], dataBuffer, remainderOfBytesFromUnevenPages);
 //         }
 //         lcd_printf("R: %d\r\n", currentPageNumber);
 //    }
 //         
 //    for (int timeout = 0; timeout < TIMEOUT; timeout++)
 //    {
 //         //df_timeout++;
 //         if (getSPI_Status() & 0x80)
 //              return TRUE;
 //    }

	//return FALSE;
#endif
return TRUE;
}
/*
unsigned char tempReadDataBuffer[FLASH_BLOCK_SIZE];

BOOL SerialDataFlash_BS_Driver::readSector(unsigned int pageNumber, unsigned int pageOffset, unsigned int numberOfBytesToRead)
{
	unsigned int temp = 0;
	temp = pageOffset;
	temp |= pageNumber << 9;

	unsigned char command[8];
	command[0] = COMMAND_READ_FROM_MAIN_MEMORY_LEGACY;
	command[1] = temp >> 16;
	command[2] = temp >> 8;
	command[3] = temp;
	command[4] = 0x00;
	command[5] = 0x00;
	command[6] = 0x00;
	command[7] = 0x00;

	CPU_SPI_nWrite8_nRead8(spiConfig, command, 8, tempReadDataBuffer, numberOfBytesToRead, 8);

	return TRUE;
}
*/

BOOL SerialDataFlash_BS_Driver::Write( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
    NATIVE_PROFILE_PAL_FLASH();
    return WriteX( context, byteAddr, NumBytes, pSectorBuff, ReadModifyWrite, TRUE );
}

BOOL SerialDataFlash_BS_Driver::Memset( void* context, ByteAddress byteAddr, UINT8 Data, UINT32 NumBytes )
{
     lcd_printf( "SerialDataFlash_BS_Driver::Memset\r\n" );
     NATIVE_PROFILE_PAL_FLASH();
     
     CHIP_WORD chipData;
     
     memset(&chipData, Data, sizeof(CHIP_WORD));
     
     return WriteX( context, byteAddr, NumBytes, &chipData, TRUE, FALSE );
     return TRUE;
}


BOOL SerialDataFlash_BS_Driver::WriteX( void* context, ByteAddress byteAddress, UINT32 NumberOfBytesToWrite, BYTE * pointerToWriteBuffer, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
//return TRUE;
	NATIVE_PROFILE_PAL_FLASH();


    
	unsigned int pageNumber = 0;
	unsigned int pageOffset = 0;
	unsigned int current_index=0;
	unsigned int reamining_bytes = NumberOfBytesToWrite;
	unsigned int beginning_bytes = 0;
	unsigned int remaining_page_segment_size;


	pageNumber = byteAddress / FLASH_BLOCK_SIZE;
	pageOffset = byteAddress % FLASH_BLOCK_SIZE;
	beginning_bytes = FLASH_BLOCK_SIZE - pageOffset;
	remaining_page_segment_size = FLASH_BLOCK_SIZE - pageOffset; //To calculate the remaining sector segment write size
	// stage1.....ofsetted data?
	if(pageOffset)
	{
		memset(DataflashTempBuffer, 0xff, FLASH_BLOCK_SIZE);
		if(NumberOfBytesToWrite <= remaining_page_segment_size)
		{
			memcpy(&DataflashTempBuffer[pageOffset], &pointerToWriteBuffer[current_index], NumberOfBytesToWrite);
			reamining_bytes-=NumberOfBytesToWrite;
			//lcd_printf("WS: %d O: %d NB:%d\r\n", pageNumber, pageOffset, NumberOfBytesToWrite);
		}
		else
		{
			memcpy(&DataflashTempBuffer[pageOffset], &pointerToWriteBuffer[current_index], beginning_bytes);
			current_index+=beginning_bytes;
			reamining_bytes-=beginning_bytes;
			//lcd_printf("WS: %d O: %d BB:%d\r\n", pageNumber, pageOffset, beginning_bytes);
		}
		writeSector(pageNumber, DataflashTempBuffer);
		pageNumber++;
	}
	if(!reamining_bytes)
		return TRUE;

	//stage 2
	unsigned int sector_to_write = reamining_bytes/FLASH_BLOCK_SIZE;
	while(sector_to_write)
	{
		memset(DataflashTempBuffer, 0xff, FLASH_BLOCK_SIZE);
		memcpy(DataflashTempBuffer, &pointerToWriteBuffer[current_index], FLASH_BLOCK_SIZE);
		writeSector(pageNumber, DataflashTempBuffer);
		//lcd_printf("WS: %d O: %d FB:%d\r\n", pageNumber, pageOffset, FLASH_BLOCK_SIZE);


		current_index+=FLASH_BLOCK_SIZE;
		reamining_bytes-=FLASH_BLOCK_SIZE;
		sector_to_write--;
		pageNumber++;

	}
	//stage 3
	if(!reamining_bytes)
		return TRUE;
    else
    {
		memset(DataflashTempBuffer, 0xff, FLASH_BLOCK_SIZE);
		memcpy(DataflashTempBuffer, &pointerToWriteBuffer[current_index], reamining_bytes);
		writeSector(pageNumber, DataflashTempBuffer);
		//lcd_printf("WS: %d O: %d RB:%d\r\n", pageNumber, pageOffset, reamining_bytes);
    }

//     while(1);


	//unsigned int numberOfPagesToWrite = 0;
	//numberOfPagesToWrite = NumberOfBytesToWrite / FLASH_BLOCK_SIZE;
#if 0
	unsigned int beginningDataStubSize = 0;
	unsigned int totalComplete512BytePagesToWrite = 0;
	unsigned int remainderOfBytesFromLastPage = 0;
	unsigned int endingDataStubSize = 0;

	beginningDataStubSize = FLASH_BLOCK_SIZE - pageOffset;
	totalComplete512BytePagesToWrite = (NumberOfBytesToWrite - beginningDataStubSize) / FLASH_BLOCK_SIZE;
	remainderOfBytesFromLastPage = (NumberOfBytesToWrite - beginningDataStubSize) % FLASH_BLOCK_SIZE;
	endingDataStubSize = remainderOfBytesFromLastPage;

	unsigned int totalPagesToWrite = 0;
	totalPagesToWrite = totalComplete512BytePagesToWrite;
	if(beginningDataStubSize > 0)
		totalPagesToWrite++;
	if(endingDataStubSize > 0)
		totalPagesToWrite++;

	unsigned int currentPageInBuffer = 0;
	unsigned int currentPageNumber = pageNumber;
	
	if(beginningDataStubSize < FLASH_BLOCK_SIZE)
	{
		for(int xyz = 0; xyz < FLASH_BLOCK_SIZE; xyz++)
		{
			dataBuffer[xyz] = 0xff;
		}

		memcpy(&dataBuffer[pageOffset], &pointerToWriteBuffer[currentPageInBuffer++], beginningDataStubSize);
		
		if(!writeSector(currentPageNumber, dataBuffer))
			return FALSE;

		currentPageNumber++;
	}
    
	for(currentPageNumber; currentPageNumber < totalComplete512BytePagesToWrite + pageNumber; currentPageNumber++)
	{
		for(int xyz = 0; xyz < FLASH_BLOCK_SIZE; xyz++)
		{
			dataBuffer[xyz] = 0xff;
		}
		
		memcpy(dataBuffer, &pointerToWriteBuffer[beginningDataStubSize + (FLASH_BLOCK_SIZE * currentPageInBuffer++)], FLASH_BLOCK_SIZE);
		
		if(!writeSector(currentPageNumber, dataBuffer))
			return FALSE;

		currentPageNumber++;
	}

	if(remainderOfBytesFromLastPage > 0)
	{
		for(int xyz = 0; xyz <= FLASH_BLOCK_SIZE; xyz++)
		{
			dataBuffer[xyz] = 0xff;
		}

		memcpy(dataBuffer, &pointerToWriteBuffer[beginningDataStubSize + (FLASH_BLOCK_SIZE * currentPageInBuffer++)], endingDataStubSize);
		
		if(!writeSector(currentPageNumber, dataBuffer))
			return FALSE;

	}
#endif
	for (int timeout = 0; timeout < TIMEOUT; timeout++)
	{
		if (getSPI_Status() & 0x80)
			return TRUE;
	}

	return FALSE;

}


BOOL SerialDataFlash_BS_Driver::writeSector(unsigned int pageNumber, BYTE *dataBuffer)
{

     GLOBAL_LOCK(irq);

	commandAndDataBuffer[0] = COMMAND_WRITE_BUFFER_1;
	commandAndDataBuffer[1] = 0x00;
	commandAndDataBuffer[2] = 0x00;
	commandAndDataBuffer[3] = 0x00;
	
	memcpy(&commandAndDataBuffer[4], dataBuffer, FLASH_BLOCK_SIZE);

	CPU_SPI_nWrite8_nRead8(spiConfig, commandAndDataBuffer, WRITE_COMMAND_SIZE + FLASH_BLOCK_SIZE, commandAndDataBuffer, FLASH_BLOCK_SIZE + WRITE_COMMAND_SIZE, 0);

	unsigned int temp = pageNumber << 2;

	unsigned char command[4];

	command[0] = 0x88; //COMMAND_WRITE_BUFFER_1_TO_MEMORY;
	command[1] = temp >> 8;
	command[2] = temp;
	command[3] = 0x00;

	CPU_SPI_nWrite8_nRead8(spiConfig, command, WRITE_COMMAND_SIZE, command, WRITE_COMMAND_SIZE, 0);

	for (int timeout = 0; timeout < TIMEOUT; timeout++)
	{
		if (getSPI_Status() & 0x80)
		return TRUE;
	}

	return FALSE;
}

BOOL SerialDataFlash_BS_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
     lcd_printf( "GetSectorMetadata\r\n" );
	return FALSE;
}

BOOL SerialDataFlash_BS_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
     lcd_printf( "SetSectorMetadata\r\n" );
	return FALSE;
}


BOOL SerialDataFlash_BS_Driver::IsBlockErased( void* context, ByteAddress BlockStartAddress, UINT32 BlockLength  )
{

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
     GLOBAL_LOCK(irq);
	//unsigned int pageNumber, temp;
     //unsigned int currentPageNumber;
     unsigned int numberOfIterations;
     //int currentByte;
	//unsigned char commandAndDataBuffer[FLASH_BLOCK_SIZE + 8];

     //pageNumber = BlockStartAddress / FLASH_BLOCK_SIZE;
     numberOfIterations = BlockLength / (FLASH_BLOCK_SIZE*sectorFlashSize);

     //lcd_printf( "IsBlockErased: not implemented\r\n" );
     //lcd_printf( "Address Page: %d\r\n", pageNumber );
     //lcd_printf( "BlockLength: %d\r\n", BlockLength );

     for(int xyz = 0; xyz < numberOfIterations ; xyz++)
     {
     /////////////
          Read(context, BlockStartAddress, (FLASH_BLOCK_SIZE*sectorFlashSize), DataflashTempBuffer );
          int x;
     
          for( x = 0; x < (FLASH_BLOCK_SIZE*sectorFlashSize); x++)
          {
               if(DataflashTempBuffer[x] != 0xFF)
               {
                    break;
               }
          }
          // blank or not
          if(x != (FLASH_BLOCK_SIZE*sectorFlashSize))
          {
               // it is NOT blank
               //lcd_printf("ER\r\n");
               return FALSE;
          }
     //////////////
          BlockStartAddress += (FLASH_BLOCK_SIZE*sectorFlashSize);

     }
     
     
     return TRUE;
}

BOOL SerialDataFlash_BS_Driver::EraseBlock( void* context, ByteAddress byteAddress )
{

     if(IsBlockErased(context, byteAddress, (FLASH_BLOCK_SIZE*sectorFlashSize)))
     {
          //lcd_printf("ER\r\n");
          return TRUE;
     }
     /*
     // if erased then skip!
     Read(context, byteAddress, (FLASH_BLOCK_SIZE*2), DataflashTempBuffer );
     int x;

     for( x = 0; x < (FLASH_BLOCK_SIZE*2); x++)
     {
          if(DataflashTempBuffer[x] != 0xFF)
          {
         
               break;
          }
     }
     if(x == (FLASH_BLOCK_SIZE*2))
     {
          lcd_printf("ER\r\n");
          return TRUE;
     }
     */


     //lcd_printf( "EraseBlock\r\n" );
     NATIVE_PROFILE_HAL_DRIVERS_FLASH();
	GLOBAL_LOCK(irq);
     unsigned char command[4];

     unsigned int pageNumber, temp;

     pageNumber = byteAddress / FLASH_BLOCK_SIZE;

     //for(int x = 0; x < 100000; x++){}
     //lcd_printf("E: %d\r\n", pageNumber);     

     for(int i=0;i<sectorFlashSize;i++)
     {
          temp = pageNumber << 2;
          command[0] = COMMAND_PAGE_ERASE;
          command[1] = temp >> 8;
          command[2] = temp;
          command[3] = 0x00;

          CPU_SPI_nWrite8_nRead8(spiConfig, command, 4, command, 4, 0);
          int timeout;
          for (timeout = 0; timeout < TIMEOUT; timeout++)
          {
               //df_timeout++;
               if (getSPI_Status() & 0x80)
                    break;
                   // return TRUE;
          }
          if(timeout == TIMEOUT)
               return FALSE;
          pageNumber++;
     }


     // a hack fro now!
/*
     pageNumber++;
     temp = pageNumber << 2;
     command[0] = COMMAND_PAGE_ERASE;
     command[1] = temp >> 8;
     command[2] = temp;
     command[3] = 0x00;

     CPU_SPI_nWrite8_nRead8(spiConfig, command, 4, command, 4, 0);
     
     for (int timeout = 0; timeout < TIMEOUT; timeout++)
     {
          //df_timeout++;
          if (getSPI_Status() & 0x80)
               return TRUE;
     }*/


    return TRUE;

}

void SerialDataFlash_BS_Driver::SetPowerState( void* context, UINT32 State )
{
    // our flash driver is always Power ON
    return ;
}


UINT32 SerialDataFlash_BS_Driver::MaxSectorWrite_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;
    
    return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 SerialDataFlash_BS_Driver::MaxBlockErase_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();
    
    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;

    return config->BlockDeviceInformation->MaxBlockErase_uSec;

}

unsigned char SerialDataFlash_BS_Driver::getSPI_Status()
{
     unsigned char getStatusCommand[2] = { COMMAND_READ_STATUS_REGISTER, 0x00 };
     
     CPU_SPI_nWrite8_nRead8(spiConfig, getStatusCommand, 2, getStatusCommand, 2, 0);
     
     return getStatusCommand[1];
}

struct IBlockStorageDevice g_SerialDataFlash_BS_DeviceTable = 
{                          
    &SerialDataFlash_BS_Driver::ChipInitialize,
    &SerialDataFlash_BS_Driver::ChipUnInitialize,
    &SerialDataFlash_BS_Driver::GetDeviceInfo,
    
    &SerialDataFlash_BS_Driver::Read,    
    
    &SerialDataFlash_BS_Driver::Write,    
    &SerialDataFlash_BS_Driver::Memset,
    &SerialDataFlash_BS_Driver::GetSectorMetadata,
    &SerialDataFlash_BS_Driver::SetSectorMetadata,
    &SerialDataFlash_BS_Driver::IsBlockErased,
    &SerialDataFlash_BS_Driver::EraseBlock,
    &SerialDataFlash_BS_Driver::SetPowerState,
    &SerialDataFlash_BS_Driver::MaxSectorWrite_uSec,
    &SerialDataFlash_BS_Driver::MaxBlockErase_uSec,
      
};


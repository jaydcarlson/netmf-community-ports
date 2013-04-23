////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Portions Copyright (c) GHI Electronics, LLC.
// Portions Copyright (c) Atmel Corporation
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <tinyhal.h>
#include "SD_BL.h"
#include "mci.h"
#include "sdmmc_mci.h"

//--//

extern struct SD_BL_CONFIGURATION g_SD_BL_Config;

#define SD_PHYISCAL_BASE_ADDRESS 0

#define AT91C_PA0_MC_DA0	AT91_GPIO_Driver::PA0
#define AT91C_PA1_MC_CDA	AT91_GPIO_Driver::PA1
#define AT91C_PA2_MC_CK		AT91_GPIO_Driver::PA2
#define AT91C_PA3_MC_DA1	AT91_GPIO_Driver::PA3
#define AT91C_PA4_MC_DA2	AT91_GPIO_Driver::PA4
#define AT91C_PA5_MC_DA3	AT91_GPIO_Driver::PA5

static BYTE s_sectorBuff[SD_DATA_SIZE];
static BYTE s_cmdBuff[SD_CMD_SIZE];

unsigned char error;

// ***************************************************************************************************************************************
// From Atmel Code
// ***************************************************************************************************************************************

/// MCI driver instance.
static Mci mciDrv;

/// SDCard driver instance.
static SdCard sdDrv;

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

#define NB_MULTI_BLOCKS 5

unsigned char pBuffer[SD_BLOCK_SIZE * NB_MULTI_BLOCKS];

/// Base address of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_BASE   AT91C_BASE_MCI
/// Identifier of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_ID     AT91C_ID_MCI
//// MCI slot connected to the SD card.
#define BOARD_SD_SLOT       MCI_SD_SLOTA


#define SD_BLOCK_SIZE 512 // 512 bytes

BOOL IsSDCardHC = FALSE;

//// Command Erase block start    
unsigned char Cmd32(SdCard *pSd, unsigned short startsector);
//
//// Command Erase block end    
unsigned char Cmd33(SdCard *pSd, unsigned short endsector);
//
//// Command Erase 
unsigned char Cmd38(SdCard *pSd);

//--//

void MCI_Handler(Mci *pMci);
int g_MCI_clock_speed_value_khz = 10000000/4;

void ISR_Mci0(void*)
{
    MCI_Handler(&mciDrv);
}

BOOL SD_BS_Driver::ChipInitialize(void *context)
{
    SD_BLOCK_CONFIG *config = (SD_BLOCK_CONFIG*)context;

    if(!config || !config->BlockDeviceInformation)
    {
        return FALSE;
    }

    BlockDeviceInfo *pDevInfo = config->BlockDeviceInformation;

	AT91_PMC &pmc = AT91::PMC();
	pmc.EnablePeriphClock(AT91C_ID_PIOA); // PMC_PCER

	CPU_GPIO_DisablePin(AT91C_PA0_MC_DA0, RESISTOR_PULLUP, 0, GPIO_ALT_MODE_1);
	CPU_GPIO_DisablePin(AT91C_PA1_MC_CDA, RESISTOR_PULLUP, 0, GPIO_ALT_MODE_1);
	CPU_GPIO_DisablePin(AT91C_PA2_MC_CK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
	CPU_GPIO_DisablePin(AT91C_PA3_MC_DA1, RESISTOR_PULLUP, 0, GPIO_ALT_MODE_1);
	CPU_GPIO_DisablePin(AT91C_PA4_MC_DA2, RESISTOR_PULLUP, 0, GPIO_ALT_MODE_1);
	CPU_GPIO_DisablePin(AT91C_PA5_MC_DA3, RESISTOR_PULLUP, 0, GPIO_ALT_MODE_1);

	CPU_GPIO_ReservePin( AT91C_PA0_MC_DA0, TRUE );
	CPU_GPIO_ReservePin( AT91C_PA1_MC_CDA, TRUE );
	CPU_GPIO_ReservePin( AT91C_PA2_MC_CK,  TRUE );
	CPU_GPIO_ReservePin( AT91C_PA3_MC_DA1, TRUE );
	CPU_GPIO_ReservePin( AT91C_PA4_MC_DA2, TRUE );
	CPU_GPIO_ReservePin( AT91C_PA5_MC_DA3, TRUE );

	CPU_INTC_ActivateInterrupt( /*UINT32 Irq_Index*/BOARD_SD_MCI_ID, /*HAL_CALLBACK_FPN ISR*/ISR_Mci0, /*void* ISR_Param*/0 );

	// Initialize the MCI driver
	MCI_Init(&mciDrv, BOARD_SD_MCI_BASE, BOARD_SD_MCI_ID, BOARD_SD_SLOT);

	CPU_INTC_InterruptEnable( /*UINT32 Irq_Index*/BOARD_SD_MCI_ID );

    // Initialize the SD card driver
    if (SD_Init(&sdDrv, (SdDriver *)&mciDrv)) {

        //lcd_printf("-E- SD/MMC card initialization failed\n\r");
        return FALSE;
    }
//    else {
//
//        lcd_printf("SD/MMC card initialization successful\n\r");
//		lcd_printf("Card Version: %u\n\r", SD_CSD_STRUCTURE(&sdDrv));
//        lcd_printf("Card size: %u MB\n\r", SD_TOTAL_SIZE(&sdDrv)/(1024*1024));
//		lcd_printf("Num of Blocks: %u\n\r", SD_TOTAL_BLOCK(&sdDrv));
//        lcd_printf("Block size: %d Bytes\n\r", (SD_TOTAL_SIZE(&sdDrv) / SD_TOTAL_BLOCK(&sdDrv)) );
//    }
    MCI_SetSpeed(&mciDrv, g_MCI_clock_speed_value_khz);

//     memset(pBuffer, 0xFF, 512);
//     error = SD_ReadBlock(&sdDrv, 0, 1, pBuffer);
//     CPU_FlushCaches();
//     for(int xyz = 0; xyz < 128; xyz++)
//          lcd_printf("%c", pBuffer[xyz]);

	// Variables to setup SD Card Dynamically.
	//BYTE regCSD[16];   <-- Not used because we have better register access.

    BYTE C_SIZE_MULT = 0;

    BYTE TAAC, NSAC, MAX_RAN_SPEED, READ_BL_LEN, SECTOR_SIZE;

    BOOL ERASE_BL_EN;

    UINT32 C_SIZE;

    UINT32 MemCapacity = 0; //total memory size, in unit of byte

    UINT32 Max_Trans_Speed = 0; //in unit of Hz

    TAAC			= SD_CSD_TAAC(&sdDrv); // regCSD[1];
    NSAC			= SD_CSD_NSAC(&sdDrv); // regCSD[2];
	MAX_RAN_SPEED	= SD_CSD_TRAN_SPEED(&sdDrv); // regCSD[3];
    READ_BL_LEN		= SD_CSD_READ_BL_LEN(&sdDrv); // regCSD[5] &0x0F;

    if(MAX_RAN_SPEED == 0x32)
        Max_Trans_Speed = 25000000; // Normal mode
    else if(MAX_RAN_SPEED == 0x5A)
        Max_Trans_Speed = 50000000; // High-Speed mode

	// Checks to see if the SD card is Version 1.0: Standard or Version 2.0: High Capacity
	if(SD_CSD_STRUCTURE(&sdDrv) == 0x00)  /*if(regCSD[0] == 0x00)*/
	// SD Version1.0
	{
		C_SIZE = SD_CSD_C_SIZE(&sdDrv); // ((regCSD[6] &0x3) << 10) | (regCSD[7] << 2) | ((regCSD[8] &0xC0) >> 6);

		C_SIZE_MULT = SD_CSD_C_SIZE_MULT(&sdDrv); // ((regCSD[9] &0x03) << 1) | ((regCSD[10] &0x80) >> 7);

		ERASE_BL_EN = (SD_CSD_ERASE_BLK_EN(&sdDrv) == 0x00) ? FALSE : TRUE; // ((regCSD[10] &0x40) == 0x00) ? FALSE : TRUE;

		SECTOR_SIZE = SD_CSD_SECTOR_SIZE(&sdDrv); // ((regCSD[10] &0x3F) << 1) | ((regCSD[11] &0x80) >> 7);

		MemCapacity = (C_SIZE + 1) * (0x1 << (C_SIZE_MULT + 2)) * (0x1 << READ_BL_LEN);

		IsSDCardHC = FALSE;
	}
	else
	// SD Version2.0
	{
		C_SIZE = SD_CSD_C_SIZE_HC(&sdDrv); // ((regCSD[7] &0x3F) << 16) | (regCSD[8] << 8) | regCSD[9];

		ERASE_BL_EN = (SD_CSD_ERASE_BLK_EN(&sdDrv) == 0x00) ? FALSE : TRUE; // ((regCSD[10] &0x40) == 0x00) ? FALSE : TRUE;

		SECTOR_SIZE = SD_CSD_SECTOR_SIZE(&sdDrv); // ((regCSD[10] &0x3F) << 1) | ((regCSD[11] &0x80) >> 7);

		MemCapacity = (C_SIZE + 1) * 512 * 1024;

		IsSDCardHC = TRUE;
	}


    //Update SD config according to CSD register
    UINT32 SectorsPerBlock    = (ERASE_BL_EN == TRUE) ? 1 : (SECTOR_SIZE + 1);
    pDevInfo->BytesPerSector  = 512; // data bytes per sector is always 512
    pDevInfo->Size            = MemCapacity;

    BlockRegionInfo* pRegions = (BlockRegionInfo*)&pDevInfo->Regions[0];
    pRegions[0].BytesPerBlock = SectorsPerBlock * pDevInfo->BytesPerSector;
    pRegions[0].NumBlocks     = MemCapacity / pRegions[0].BytesPerBlock;
        
    BlockRange* pRanges   = (BlockRange*)&pRegions[0].BlockRanges[0];

    pRanges[0].StartBlock = 0;
    pRanges[0].EndBlock   = pRegions[0].NumBlocks-1;

    return TRUE;
}

BOOL SD_BS_Driver::ChipUnInitialize(void *context)
{    
	CPU_GPIO_ReservePin( AT91C_PA0_MC_DA0, FALSE );
	CPU_GPIO_ReservePin( AT91C_PA1_MC_CDA, FALSE );
	CPU_GPIO_ReservePin( AT91C_PA2_MC_CK,  FALSE );
	CPU_GPIO_ReservePin( AT91C_PA3_MC_DA1, FALSE );
	CPU_GPIO_ReservePin( AT91C_PA4_MC_DA2, FALSE );
	CPU_GPIO_ReservePin( AT91C_PA5_MC_DA3, FALSE );

    return TRUE;
}

const BlockDeviceInfo *SD_BS_Driver::GetDeviceInfo(void *context)
{
    SD_BLOCK_CONFIG *config = (SD_BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation;
}

BOOL SD_BS_Driver::Read(void *context, ByteAddress phyAddress, UINT32 NumBytes, BYTE *pSectorBuff)
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    UINT32 RangeIndex;
    UINT32 RegionIndex;
    UINT32 BytesPerSector;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
    
    if(pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddress, RegionIndex, RangeIndex))
    {
        ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( &pConfig->BlockDeviceInformation->Regions[RegionIndex], phyAddress);

        BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

        CHIP_WORD *pBuf = (CHIP_WORD*)pSectorBuff;

        UINT32 offset = phyAddress - (StartSector * pConfig->BlockDeviceInformation->BytesPerSector);

        UINT32 bytes  = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);
#if 1
		while(NumBytes > 0)
        {
            if(!ReadSector(StartSector, offset, bytes, pBuf, BytesPerSector))
            {
				//SD_Stop(SdCard *pSd, SdDriver *pSdDriver); // Prototype To Identify Vars
				//SD_Stop(&sdDrv, (SdDriver *)&mciDrv);

                return FALSE;
            }
            
            offset    = 0;
            pBuf      = (CHIP_WORD*)((UINT32)pBuf + bytes);
            NumBytes -= bytes;
            StartSector++;

            bytes = __min(BytesPerSector, NumBytes);
        }

        //SD_Stop(&sdDrv, (SdDriver *)&mciDrv);
#endif        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL SD_BS_Driver::ReadSector(SectorAddress Address, UINT32 Offset, UINT32 NumBytes, BYTE *pSectorBuff, UINT32 BytesPerSector)
{
	//error = SD_ReadBlock(&sdDrv, block, multiBlock, pBuffer);
	//if(!SD_ReadBlock(&sdDrv, (Address + Offset), (NumBytes/BytesPerSector), pSectorBuff))
     error = SD_ReadBlock(&sdDrv, (Address + Offset), (NumBytes/BytesPerSector), pSectorBuff);
	 CPU_FlushCaches();
	if(error == 0x00)
		return TRUE;
	else
		return FALSE;
}

BOOL SD_BS_Driver::Write(void *context, ByteAddress phyAddr, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite )
{
    NATIVE_PROFILE_PAL_FLASH();

    return WriteX( context, phyAddr, NumBytes, pSectorBuff, ReadModifyWrite, TRUE );
}

BOOL SD_BS_Driver::WriteX(void *context, ByteAddress phyAddr, UINT32 NumBytes, BYTE *pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
    NATIVE_PROFILE_PAL_FLASH();

    UINT32 RangeIndex;
    UINT32 RegionIndex;
    UINT32 BytesPerSector;
    UINT32 offset;
    UINT32 bytes;
    BYTE response;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    CHIP_WORD *pData, *pWrite;

    // find the corresponding region     
    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddr, RegionIndex, RangeIndex))
        return FALSE;

    ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( &pConfig->BlockDeviceInformation->Regions[RegionIndex], phyAddr);

    pData = (CHIP_WORD*)pSectorBuff;
    BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

    offset = phyAddr - (StartSector * BytesPerSector);

    bytes = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);

    //CPU_SPI_Xaction_Start(g_SD_BL_Config.SPI);

    while(NumBytes > 0)
    {
        // if we are using memset, or if the bytes written are less than the BytesPerSector then do read/modify/write
        if(!fIncrementDataPtr || (bytes != BytesPerSector))
        {   
            if(bytes != BytesPerSector)
            {
                if(!ReadSector(StartSector, 0, BytesPerSector, s_sectorBuff, BytesPerSector))
                {
                    //CPU_SPI_Xaction_Stop(g_SD_BL_Config.SPI);
                    return FALSE;
                }

            }
            
            pWrite = (CHIP_WORD*)&s_sectorBuff[0];

            if(fIncrementDataPtr)
            {
                memcpy(&pWrite[offset], pData, bytes);
            }
            else
            {
                memset(&pWrite[offset], *pData, bytes);
            }
        }
        else
        {
            pWrite = pData;
        }

//        // select SD CS
//        SD_CsSetLow();
//        
//        // send CMD24 --read single block data
//        response = SD_SendCmdWithR1Resp(SD_WRITE_SINGLE_BLOCK, StartSector << 9, 0xff, R1_IN_READY_STATUS);
//
//        if(response == R1_IN_READY_STATUS)
//        {
//            SPISendByte(SD_START_DATA_BLOCK_TOKEN); // send DATA_BLOCK_TOKEN
//
            // send data
            //SPISendCount(pWrite, BytesPerSector);
		CPU_FlushCaches();
			error = SD_WriteBlock(&sdDrv, StartSector, (NumBytes/BytesPerSector), pWrite);
//
//            // send CRC
//            SPISendByte(0xff);
//           SPISendByte(0xff);
//
//            // wait for end of write busy
//            response = SD_CheckBusy();
//        }
//
//        //disable SD card CS
//        SD_CsSetHigh();

        if(fIncrementDataPtr) pData = (CHIP_WORD*)((UINT32)pData + bytes);

        NumBytes   -= bytes;
        offset      = 0;
        StartSector++;
        bytes = __min(BytesPerSector, NumBytes);        
    }

//    CPU_SPI_Xaction_Stop(g_SD_BL_Config.SPI);

    return TRUE;

}

BOOL SD_BS_Driver::Memset(void *context, ByteAddress phyAddr, UINT8 Data, UINT32 NumBytes )
{
    NATIVE_PROFILE_PAL_FLASH();

    return WriteX( context, phyAddr, NumBytes, &Data, TRUE, FALSE );
}

BOOL SD_BS_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL SD_BS_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL SD_BS_Driver::IsBlockErased(void *context, ByteAddress phyAddress, UINT32 BlockLength)
{

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex;
    UINT32 RangeIndex;
    UINT32 SectorsPerBlock;
    UINT32 BytesPerSector;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    // this is static buffer, as the driver is current tailor for SD, a page size is 2048 bytes.
    BYTE *pSectorBuff = s_sectorBuff;


    BYTE state_After_Erase = g_SD_BL_Config.State_After_Erase ? 0xFF : 0x00;

    if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddress, RegionIndex, RangeIndex))
        return FALSE;

    ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( &pConfig->BlockDeviceInformation->Regions[RegionIndex], phyAddress);

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    // as the input arg Sector may not be the startSector address of a block,
    // we need to recalculate it.
    BytesPerSector  = pConfig->BlockDeviceInformation->BytesPerSector;
    SectorsPerBlock = (pRegion->BytesPerBlock / BytesPerSector);

    StartSector = (StartSector / SectorsPerBlock) * SectorsPerBlock;
    
    for(UINT32 i = 0; i < SectorsPerBlock; i++)
    {
        SD_BS_Driver::Read(context, StartSector, BytesPerSector, pSectorBuff);
        for(UINT32 j = 0; j < BytesPerSector; j++)
        {
            if(pSectorBuff[j] != state_After_Erase)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL SD_BS_Driver::EraseBlock(void *context, ByteAddress phyAddr)
{
//////	Commented out befor the 4.2.2.1 release
//////    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
//////
//////    UINT32 RangeIndex;
//////    UINT32 RegionIndex;
//////
//////    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
//////	
//////	const BlockRegionInfo* pRegion;
//////	ByteAddress StartSector;		// typedef UINT32 ByteAddress; 
//////	UINT32 SectorsPerBlock;
//////	SectorAddress SectorAddress;	// typedef UINT32 SectorAddress;
//////
//////	if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddr, RegionIndex, RangeIndex))
//////		return FALSE;
//////
//////	pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];
//////
//////
//////     StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, phyAddr );
//////
//////	SectorsPerBlock = pRegion->BytesPerBlock / pConfig->BlockDeviceInformation->BytesPerSector;
//////	SectorAddress = (StartSector / SectorsPerBlock) * SectorsPerBlock;
//////
//////
//////// TODO: this needs to be added if file system needs it
//////     lcd_printf("SD:EraseBlock: no one is home!");
     
    //CPU_SPI_Xaction_Start(g_SD_BL_Config.SPI);

   // EraseSectors(SectorAddress, SectorsPerBlock);

    //CPU_SPI_Xaction_Stop(g_SD_BL_Config.SPI);

    return TRUE;

}
/*
BOOL SD_BS_Driver::EraseSectors(SectorAddress Address, INT32 SectorCount)
{
    BYTE response;

    ////SD_CsSetLow(); // cs low

    //send ERASE_WR_BLK_START command
    //response = SD_SendCmdWithR1Resp(SD_ERASE_WR_BLK_START, Address << 9, 0xff, R1_IN_READY_STATUS);
	
	response = Cmd32(&sdDrv, Address);
    
	//if(response != R1_IN_READY_STATUS)
    //{
    //    SD_CsSetHigh();
    //    return FALSE;
    //}

    //send ERASE_WR_BLK_END command
    //response = SD_SendCmdWithR1Resp(SD_ERASE_WR_BLK_END, (Address + SectorCount - 1) << 9, 0xff, R1_IN_READY_STATUS);

	response = Cmd33(&sdDrv, (Address + SectorCount - 1));

    //if(response != R1_IN_READY_STATUS)
    //{
    //    SD_CsSetHigh();
    //    return FALSE;
    //}

    // send erase command
    //response = SD_SendCmdWithR1Resp(SD_ERASE, 0xffffffff, 0xff, R1_IN_READY_STATUS);

	response = Cmd38(&sdDrv);

    //if(response != R1_IN_READY_STATUS)
    //{
    //    SD_CsSetHigh();
    //    return FALSE;
    //}

    //// wait for IDLE
    //SD_CheckBusy();

    //SD_CsSetHigh();

    return TRUE;
}
*/
void SD_BS_Driver::SetPowerState(void *context, UINT32 State)
{
    // our flash driver is always Power ON
    return;
}

UINT32 SD_BS_Driver::MaxSectorWrite_uSec(void *context)
{
    NATIVE_PROFILE_PAL_FLASH();

    SD_BLOCK_CONFIG *config = (SD_BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 SD_BS_Driver::MaxBlockErase_uSec(void *context)
{
    NATIVE_PROFILE_PAL_FLASH();

    SD_BLOCK_CONFIG *config = (SD_BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

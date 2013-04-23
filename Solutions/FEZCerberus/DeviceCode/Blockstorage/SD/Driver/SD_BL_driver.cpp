////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Portions Copyright (c) GHI Electronics, LLC.
// Portions Copyright (c) STMicroelectronics
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <tinyhal.h>
#include "SD_BL.h"
#include "stm32f4xx_sdio_sd.h"
#include "stm32f4xx_sdio.h"

//--//

extern SD_CardInfo SDCardInfo;

extern struct SD_BL_CONFIGURATION g_SD_BL_Config;

#define SD_PHYISCAL_BASE_ADDRESS 0

#define PC8_SD_D0	((16 * 2) + 8)
#define PC9_SD_D1	((16 * 2) + 9)
#define PC10_SD_D2	((16 * 2) + 10)
#define PC11_SD_D3	((16 * 2) + 11)
#define PC12_SD_CLK	((16 * 2) + 12)
#define PD2_SD_CMD	((16 * 3) + 2)

static BYTE s_sectorBuff[SD_DATA_SIZE];
static BYTE s_cmdBuff[SD_CMD_SIZE];

unsigned char error;

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

#define NB_MULTI_BLOCKS 5
#define SD_BLOCK_SIZE 512 // 512 bytes
#define SD_INITIALIZE_TIMEOUT 0x2000 // 5secs
BOOL SD_GetStatus_WithTimeOut(int timeout);
unsigned char pBuffer[SD_BLOCK_SIZE * NB_MULTI_BLOCKS];

BOOL IsSDCardHC = FALSE;

//--//

//int g_MCI_clock_speed_value_khz = 10000000/4; // Needed to compile Firmware but not used in this file.
uint8_t g_MCI_clock_speed_div = 0;

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */

void SDIO_IRQHandler(void*)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

BOOL SD_GetStatus_WithTimeOut(int timeout)
{  
    //while(SD_GetStatus() != SD_TRANSFER_OK);
    for (;timeout>0; timeout--)
    {
      HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1);
      if (SD_GetStatus() == SD_TRANSFER_OK) 
        return TRUE;
    }
    return FALSE;
}
BOOL SD_BS_Driver::ChipInitialize(void *context)
{
    SD_BLOCK_CONFIG *config = (SD_BLOCK_CONFIG*)context;

    if(!config || !config->BlockDeviceInformation)
    {
        return FALSE;
    }

    BlockDeviceInfo *pDevInfo = config->BlockDeviceInformation;

	UINT32 alternate = 0x1C2;

	CPU_GPIO_DisablePin(PC8_SD_D0, RESISTOR_PULLUP, 0, (GPIO_ALT_MODE)alternate);
	CPU_GPIO_DisablePin(PC9_SD_D1, RESISTOR_PULLUP, 0, (GPIO_ALT_MODE)alternate);
	CPU_GPIO_DisablePin(PC10_SD_D2, RESISTOR_PULLUP, 0, (GPIO_ALT_MODE)alternate);
	CPU_GPIO_DisablePin(PC11_SD_D3, RESISTOR_PULLUP, 0, (GPIO_ALT_MODE)alternate);
	CPU_GPIO_DisablePin(PC12_SD_CLK, RESISTOR_DISABLED, 0, (GPIO_ALT_MODE)alternate);
	CPU_GPIO_DisablePin(PD2_SD_CMD, RESISTOR_PULLUP, 0, (GPIO_ALT_MODE)alternate);

	CPU_GPIO_ReservePin( PC8_SD_D0, TRUE );
	CPU_GPIO_ReservePin( PC9_SD_D1, TRUE );
	CPU_GPIO_ReservePin( PC10_SD_D2, TRUE );
	CPU_GPIO_ReservePin( PC11_SD_D3, TRUE );
	CPU_GPIO_ReservePin( PC12_SD_CLK, TRUE );
	CPU_GPIO_ReservePin( PD2_SD_CMD, TRUE );

	RCC->APB2ENR |= (1 << 11);
	CPU_INTC_ActivateInterrupt( /*UINT32 Irq_Index*/SDIO_IRQn, /*HAL_CALLBACK_FPN ISR*/SDIO_IRQHandler, /*void* ISR_Param*/0 );

	__IO SD_Error errorstatus = SD_OK;
	//errorstatus = SD_PowerON();
	errorstatus = SD_Init();

	CPU_INTC_InterruptEnable( /*UINT32 Irq_Index*/SDIO_IRQn );

	memset(pBuffer, 0xFF, 512);
	errorstatus = SD_ReadBlock(pBuffer, 0, 512);
	//errorstatus = SD_WaitReadOperation();
	//while(SD_GetStatus() != SD_TRANSFER_OK);
   if (SD_GetStatus() != SD_TRANSFER_OK)
   {
      if (SD_GetStatus_WithTimeOut(SD_INITIALIZE_TIMEOUT)== FALSE) 
        return FALSE;
   }


	//CPU_FlushCaches();

//	// Variables to setup SD Card Dynamically.
//	//BYTE regCSD[16];   <-- Not used because we have better register access.

    BYTE C_SIZE_MULT = 0;

    BYTE TAAC, NSAC, MAX_RAN_SPEED, READ_BL_LEN, SECTOR_SIZE;

    BOOL ERASE_BL_EN;

    UINT32 C_SIZE;

    UINT32 MemCapacity = 0; //total memory size, in unit of byte

    TAAC			= SDCardInfo.SD_csd.TAAC; // regCSD[1]; /* STM // Original */
    NSAC			= SDCardInfo.SD_csd.NSAC; // regCSD[2]; /* STM // Original */
	MAX_RAN_SPEED	= SDCardInfo.SD_csd.MaxBusClkFrec; // regCSD[3]; /* STM // Original */
    READ_BL_LEN		= SDCardInfo.SD_csd.RdBlockLen; // regCSD[5] &0x0F; /* STM // Original */

	// Checks to see if the SD card is Version 1.0: Standard or Version 2.0: High Capacity
	if(SDCardInfo.SD_csd.CSDStruct == 0x00)  /*if(regCSD[0] == 0x00)*/
	// SD Version1.0
	{
		C_SIZE = SDCardInfo.SD_csd.DeviceSize; // ((regCSD[6] &0x3) << 10) | (regCSD[7] << 2) | ((regCSD[8] &0xC0) >> 6); /* STM // Original */

		C_SIZE_MULT = SDCardInfo.SD_csd.DeviceSizeMul; // ((regCSD[9] &0x03) << 1) | ((regCSD[10] &0x80) >> 7); /* STM // Original */

		ERASE_BL_EN = (SDCardInfo.SD_csd.EraseGrSize == 0x00) ? FALSE : TRUE; // ((regCSD[10] &0x40) == 0x00) ? FALSE : TRUE; /* STM // Original */

		SECTOR_SIZE = SDCardInfo.SD_csd.EraseGrMul; // ((regCSD[10] &0x3F) << 1) | ((regCSD[11] &0x80) >> 7); /* STM // Original */

		MemCapacity = SDCardInfo.CardCapacity; // (C_SIZE + 1) * (0x1 << (C_SIZE_MULT + 2)) * (0x1 << READ_BL_LEN); /* STM // Original */

		IsSDCardHC = FALSE;
	}
	else
	// SD Version2.0
	{
		C_SIZE = SDCardInfo.SD_csd.DeviceSize; // ((regCSD[7] &0x3F) << 16) | (regCSD[8] << 8) | regCSD[9]; /* STM // Original */

		ERASE_BL_EN = (SDCardInfo.SD_csd.EraseGrSize == 0x00) ? FALSE : TRUE; // ((regCSD[10] &0x40) == 0x00) ? FALSE : TRUE; /* STM // Original */

		SECTOR_SIZE = SDCardInfo.SD_csd.EraseGrMul; // ((regCSD[10] &0x3F) << 1) | ((regCSD[11] &0x80) >> 7); /* STM // Original */

		MemCapacity = SDCardInfo.CardCapacity; // (C_SIZE + 1) * 512 * 1024; /* STM // Original */

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
	//CPU_GPIO_ReservePin( PC8_SD_D0, FALSE );
	//CPU_GPIO_ReservePin( PC9_SD_D1, FALSE );
	//CPU_GPIO_ReservePin( PC10_SD_D2, FALSE );
	//CPU_GPIO_ReservePin( PC11_SD_D3, FALSE );
	//CPU_GPIO_ReservePin( PC12_SD_CLK, FALSE );
	//CPU_GPIO_ReservePin( PD2_SD_CMD, FALSE );

    return TRUE;
}

const BlockDeviceInfo *SD_BS_Driver::GetDeviceInfo(void *context)
{
    SD_BLOCK_CONFIG *config = (SD_BLOCK_CONFIG*)context;

    return config->BlockDeviceInformation;
}

BOOL SD_BS_Driver::Read(void *context, ByteAddress phyAddress, UINT32 NumBytes, BYTE *pSectorBuff)
{
	__IO SD_Error errorstatus = SD_OK;
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

		while(NumBytes > 0)
        {
            if(!ReadSector(StartSector, offset, bytes, pBuf, BytesPerSector))
            {
				errorstatus = SD_StopTransfer();
				return FALSE;
            }
            
            offset    = 0;
            pBuf      = (CHIP_WORD*)((UINT32)pBuf + bytes);
            NumBytes -= bytes;
            StartSector++;

            bytes = __min(BytesPerSector, NumBytes);
        }

		return TRUE;
    }
    else
    {
        return FALSE;
    }
		return TRUE;

}

BOOL SD_BS_Driver::ReadSector(SectorAddress Address, UINT32 Offset, UINT32 NumBytes, BYTE *pSectorBuff, UINT32 BytesPerSector)
{
	__IO SD_Error errorstatus = SD_OK;

	errorstatus = SD_ReadBlock(pSectorBuff, ((Address * BytesPerSector) + Offset), BytesPerSector);

	while(SD_GetStatus() != SD_TRANSFER_OK);

	//CPU_FlushCaches();

	if(errorstatus == SD_OK)
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

	__IO SD_Error errorstatus = SD_OK;
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

    while(NumBytes > 0)
    {
        // if we are using memset, or if the bytes written are less than the BytesPerSector then do read/modify/write
        if(!fIncrementDataPtr || (bytes != BytesPerSector))
        {   
            if(bytes != BytesPerSector)
            {
                if(!ReadSector(StartSector, 0, BytesPerSector, s_sectorBuff, BytesPerSector))
                {
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

		CPU_FlushCaches();
		errorstatus = SD_WriteBlock(pWrite, (StartSector * BytesPerSector), BytesPerSector);
		while(SD_GetStatus() != SD_TRANSFER_OK);

        if(fIncrementDataPtr) pData = (CHIP_WORD*)((UINT32)pData + bytes);

        NumBytes   -= bytes;
        offset      = 0;
        StartSector++;
        bytes = __min(BytesPerSector, NumBytes);        
    }

	if(errorstatus == SD_OK)
		return TRUE;
	else
		return FALSE;
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
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RangeIndex;
    UINT32 RegionIndex;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
	
	const BlockRegionInfo* pRegion;
	ByteAddress StartSector;		// typedef UINT32 ByteAddress; 
	UINT32 SectorsPerBlock;
	SectorAddress SectorAddress;	// typedef UINT32 SectorAddress;

	if(!pConfig->BlockDeviceInformation->FindRegionFromAddress(phyAddr, RegionIndex, RangeIndex))
		return FALSE;

	pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];


     StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, phyAddr );

	SectorsPerBlock = pRegion->BytesPerBlock / pConfig->BlockDeviceInformation->BytesPerSector;
	SectorAddress = (StartSector / SectorsPerBlock) * SectorsPerBlock;


// TODO: this needs to be added if file system needs it
     lcd_printf("SD:EraseBlock: no one is home!");
     
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

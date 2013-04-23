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

//#include <Drivers\BlockStorage\WearLeveling\BS_WearLeveling.h>
//--//

//#define TINCLR_TEST         1

#define FLASH_MANUFACTURER_CODE                 0x1F
#define FLASH_DEVICE_CODE                       0x27

#define FLASH_BLOCK_COUNT                       512 //4096 //8192 //4096 //1024

#define FLASH_BASE_ADDRESS                      0x00000000
#define FLASH_SECTOR_PER_BLOCK                  16 // 8 pages/sectors per block If full chip is used
#define FLASH_SECTOR_SIZE                       528 //(528*2) // 512 for FEZ Hydra || 1024 on original SAM
#define FLASH_BLOCK_SIZE                        (FLASH_SECTOR_PER_BLOCK*FLASH_SECTOR_SIZE)
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC     15000 // <-Set for the time for a page/sector erase. || This is for block erase-> 45000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC    3000 // This is with erase included -> 17000
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC         35000 // <-Set for the time for a page/sector erase. || This is for block erase-> 100000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC        6000 // This is with erase included -> 40000

#define FLASH_SIZE								0x400000 // = 1 MB or 1024 KB *****  0x800000 8 MB or 8192 KB If full chip is used
#define SerialDataFlash_SIZE_IN_BYTES           FLASH_BLOCK_SIZE*FLASH_BLOCK_COUNT
#define SerialDataFlash_WP_GPIO_PIN             GPIO_PIN_NONE
#define SerialDataFlash_WP_ACTIVE               FALSE


// BlockDeviceInformation

#define BLOCK_DEVICE_INFO_REMOVEABLE            FALSE
#define BLOCK_DEVICE_INFO_SUPPORTXIP            FALSE
#define BLOCK_DEVICE_INFO_WRITEPROTECTED        FALSE
#define BLOCK_DEVICE_INFO_SUPPORTSCOPYBACK		FALSE
#define SerialDataFlash_NUMREGIONS              1


//--//
#ifdef MEMORY_USAGE_SPECIAL
#undef MEMORY_USAGE_SPECIAL
#endif

#ifdef MEMORY_USAGE_SPECIAL2
#undef MEMORY_USAGE_SPECIAL2
#endif

#ifdef MEMORY_USAGE_SPECIAL3
#undef MEMORY_USAGE_SPECIAL3
#endif

#if defined(MEMORY_USAGE_GCC_SPECIAL_CODE)
#undef MEMORY_USAGE_GCC_SPECIAL_CODE 
#endif

#ifdef __GNUC__
#define MEMORY_USAGE_GCC_SPECIAL_BOOTSTRAP  BlockStatus::USAGE_BOOTSTRAP
#define MEMORY_USAGE_GCC_SPECIAL_CONFIG     BlockStatus::USAGE_CONFIG

#else
#define MEMORY_USAGE_GCC_SPECIAL_BOOTSTRAP  BlockStatus::USAGE_CONFIG
#define MEMORY_USAGE_GCC_SPECIAL_CONFIG     BlockStatus::USAGE_CODE
#endif





//--//
const BlockRange g_SerialDataFlash_BlockStatus[] =
{
	//{ BlockRange::BLOCKTYPE_CODE		,    0,    3 }, // For GHI Loader
	{ BlockRange::BLOCKTYPE_BOOTSTRAP	,    0,    3 }, // For GHI Loader
	{ BlockRange::BLOCKTYPE_BOOTSTRAP	,    4,   31 }, // For TinyBooter
	{ BlockRange::BLOCKTYPE_RESERVED	,   32,   63 }, // reserved
    { BlockRange::BLOCKTYPE_CODE		,   64,  319 }, // 2 MB
	{ BlockRange::BLOCKTYPE_DEPLOYMENT	,  320,  478 }, // Aproximately 1.25 MB User Deployment
	{ BlockRange::BLOCKTYPE_STORAGE_A	,  479,  494 }, // 128 KB Extended Weak Reference
    { BlockRange::BLOCKTYPE_STORAGE_B	,  495,  510 }, // 128 KB Extended Weak Reference
	{ BlockRange::BLOCKTYPE_CONFIG		,  511,  511 }  // 1 Block = 8 KB (16 * 528)

	//{ BlockRange::BLOCKTYPE_BOOTSTRAP	,     0,    31 }, // For GHI Loader
	//{ BlockRange::BLOCKTYPE_BOOTSTRAP	,    32,   288 }, // For TinyBooter
	//{ BlockRange::BLOCKTYPE_RESERVED	,   288,   511 }, // reserved
 //   { BlockRange::BLOCKTYPE_CODE		,   512,  2559 }, // 2 MB
	//{ BlockRange::BLOCKTYPE_DEPLOYMENT	,  2560,  3831 }, // Aproximately 1.25 MB User Deployment
	//{ BlockRange::BLOCKTYPE_STORAGE_A	,  3832,  3959 }, // 128 KB Extended Weak Reference
 //   { BlockRange::BLOCKTYPE_STORAGE_B	,  3960,  4087 }, // 128 KB Extended Weak Reference
	//{ BlockRange::BLOCKTYPE_CONFIG		,  4088,  4095 }  // 8 KB (8 * 1056)

	//{ BlockRange::BLOCKTYPE_BOOTSTRAP	,     0,    31 }, // For GHI Loader
	//{ BlockRange::BLOCKTYPE_BOOTSTRAP	,    32,   287 }, // For TinyBooter
	//{ BlockRange::BLOCKTYPE_RESERVED	,   288,   511 }, // reserved
 //   { BlockRange::BLOCKTYPE_CODE		,   512,  2559 }, // 2 MB
	//{ BlockRange::BLOCKTYPE_DEPLOYMENT	,  2560,  3835 }, // Aproximately 1.25 MB User Deployment
	//{ BlockRange::BLOCKTYPE_STORAGE_A	,  3836,  3963 }, // 128 KB Extended Weak Reference
 //   { BlockRange::BLOCKTYPE_STORAGE_B	,  3964,  4091 }, // 128 KB Extended Weak Reference
	//{ BlockRange::BLOCKTYPE_CONFIG		,  4092,  4095 } // 4 KB (4 * 1056)

//	{ BlockRange::BLOCKTYPE_BOOTSTRAP	,     0,    63 }, // For GHI Loader
//	{ BlockRange::BLOCKTYPE_BOOTSTRAP	,    64,   575 }, // For TinyBooter
//	{ BlockRange::BLOCKTYPE_RESERVED	,   576,  1023 },
//    { BlockRange::BLOCKTYPE_CODE		,  1024,  5119 }, // 2 MB (4096 * 512)
//	{ BlockRange::BLOCKTYPE_DEPLOYMENT	,  5120,  6143 }, // .5 MB (1024 * 512)
//	{ BlockRange::BLOCKTYPE_CONFIG		,  6144,  6159 }, // 4 KB (16 * 512)
//	{ BlockRange::BLOCKTYPE_STORAGE_A	,  6160,  6161 },
//    { BlockRange::BLOCKTYPE_STORAGE_B	,  6162,  6163 },
//	{ BlockRange::BLOCKTYPE_RESERVED	,  6164,  8191 }

	//{ BlockRange::BLOCKTYPE_BOOTSTRAP	,     0,    63 }, // For GHI Loader
	//{ BlockRange::BLOCKTYPE_BOOTSTRAP	,    64,   575 }, // For TinyBooter
	//{ BlockRange::BLOCKTYPE_RESERVED	,   576,  1023 },
 //   { BlockRange::BLOCKTYPE_CODE		,  1024,  3071 },
 //   { BlockRange::BLOCKTYPE_DEPLOYMENT	,  3072,  4075 },  // original end sector was 4083
	//{ BlockRange::BLOCKTYPE_CONFIG		,  4076,  4091 }, // original start sector was 4084
 //   { BlockRange::BLOCKTYPE_STORAGE_A	,  4092,  4093 },
 //   { BlockRange::BLOCKTYPE_STORAGE_B	,  4094,  4095 },
	//{ BlockRange::BLOCKTYPE_RESERVED	,  4096,  8191 }

//	{ BlockRange::BLOCKTYPE_BOOTSTRAP ,    0,   31 }, // For GHI Loader
//	{ BlockRange::BLOCKTYPE_BOOTSTRAP ,   32,  287 }, // For TinyBooter
//	{ BlockRange::BLOCKTYPE_RESERVED  ,  288,  511 },
//    { BlockRange::BLOCKTYPE_CODE     ,  512, 1535 },
//    { BlockRange::BLOCKTYPE_DEPLOYMENT, 1536, 2041 },
//	{ BlockRange::BLOCKTYPE_CONFIG	  , 2042, 2045 },
//    { BlockRange::BLOCKTYPE_STORAGE_A , 2046, 2046 },
//    { BlockRange::BLOCKTYPE_STORAGE_B , 2047, 2047 }
};

const BlockRegionInfo  g_SerialDataFlash_BlkRegion[SerialDataFlash_NUMREGIONS] = 
{
    FLASH_BASE_ADDRESS,             // ByteAddress  Start;              // Starting Sector address
    FLASH_BLOCK_COUNT,              // UINT32       NumBlocks;          // total number of blocks in this region
    FLASH_BLOCK_SIZE,               // UINT32       BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)
	
	ARRAYSIZE_CONST_EXPR(g_SerialDataFlash_BlockStatus),  // NumBlockRanges
    g_SerialDataFlash_BlockStatus, // BlockRanges 
};

//--//

#undef MEMORY_USAGE_GCC_SPECIAL_CODE 

//--//


BlockDeviceInfo  g_SerialDataFlash_DeviceInfo=
{
    {  
        BLOCK_DEVICE_INFO_REMOVEABLE,
        BLOCK_DEVICE_INFO_SUPPORTXIP,
        BLOCK_DEVICE_INFO_WRITEPROTECTED,
		BLOCK_DEVICE_INFO_SUPPORTSCOPYBACK,
    },
    
    FLASH_SECTOR_WRITE_MAX_TIME_USEC,       // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_MAX_TIME_USEC,        // UINT32 Duration_Max_SectorErase_uSec;
	FLASH_SECTOR_SIZE,
	FLASH_SIZE,
	ARRAYSIZE_CONST_EXPR(g_SerialDataFlash_BlkRegion),  // UINT32 NumRegions;
    g_SerialDataFlash_BlkRegion,                    //const BlockRegionInfo* pRegions;
};


struct BLOCK_CONFIG SerialDataFlash_BS_Config =
{
    {
        SerialDataFlash_WP_GPIO_PIN,            // GPIO_PIN             Pin;
        SerialDataFlash_WP_ACTIVE,              // BOOL                 ActiveState;
    },

     &g_SerialDataFlash_DeviceInfo,             // BlockDeviceinfo
     

};


extern struct IBlockStorageDevice g_SerialDataFlash_BS_DeviceTable;

//struct BLOCK_CONFIG	g_SerialDataFlash_Config;

//struct BS_WearLeveling_Config g_SerialDataFlash_8_WearLeveling_Config =
//{
//    &SerialDataFlash_BS_Config,
//
//    &g_SerialDataFlash_BS_DeviceTable,
//
//    0,
//    FLASH_BLOCK_SIZE,
//
//    NULL,
//};
//--//



struct BlockStorageDevice g_SerialDataFlash_BS;


//--//


struct SerialDataFlash_BS_Driver g_SerialDataFlash_BS_Driver;
//--//


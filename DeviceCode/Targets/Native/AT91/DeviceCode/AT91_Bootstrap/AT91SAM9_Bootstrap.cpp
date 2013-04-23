////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Portions Copyright (c) GHI Electronics, LLC.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <Cores\arm\Include\cpu.h>

static const UINT32  c_Bootstrap_Register_Begin =   0xFFF00000;
static const UINT32  c_Bootstrap_Register_End   =   0xFFFFFFFF;
static const UINT32  c_Bootstrap_SDRAM_Begin    =   SDRAM_MEMORY_Base;
static const UINT32  c_Bootstrap_SDRAM_End      =   SDRAM_MEMORY_Base + SDRAM_MEMORY_Size - ARM9_MMU::c_TTB_size;
static const UINT32  c_Bootstrap_SRAM_Begin     =   SRAM_MEMORY_Base;
static const UINT32  c_Bootstrap_SRAM_End       =   SRAM_MEMORY_Base + SRAM_MEMORY_Size;
static const UINT32  c_Bootstrap_FLASH_Begin    =   FLASH_MEMORY_Base;
static const UINT32  c_Bootstrap_FLASH_End      =   FLASH_MEMORY_Base + FLASH_MEMORY_Size;
static UINT32* const c_Bootstrap_BaseOfTTBs     =   (UINT32*)(c_Bootstrap_SDRAM_End);

static const UINT32	 c_RLP_Physical_Address		=	0x20E00000; // Added for RLP Support of Memory MMU
static const UINT32  c_RLP_Virtual_Address_Cached =	0xA0000000; // Added for RLP Support of Memory MMU
static const UINT32  c_RLP_Virtual_Address_Uncached = 0xB0000000; // Added for RLP Support of Memory MMU


extern void AT91_SAM_SdramInit();
extern void AT91_SAM_ClockInit();

//  ARM BOOTSTRAPPING FOR ARM9 IS A COMMON METHOD "CPU_ARM9_BootstrapCode".
//void BootstrapCode_ARM()
//{
//    UINT32 reg;
//
//    //--//
//
//    //
//    // MMU                              : disabled
//    // Alignment fault checking         : disabled.
//    // Data Cache                       : disabled.
//    // Instruction Cache                : disabled.
//    // Write Buffer                     : enabled. It is always enabled.
//    // Exception handlers               : 32bits
//    // 26-bit address exception checking: disabled.
//    // Late Abort Model selected.
//    // Configured for little-endian memory system.
//    //
//    reg = 0x78;
//    __asm
//    {
//        mcr     p15, 0, reg, c1, c0, 0 // Control register.
//    }
//
//    //--//
//
//    reg = 0;
//
//    __asm
//    {
//        mcr     p15, 0, reg, c8, c7, 0  // Invalidate all TLBs.
//        mcr     p15, 0, reg, c7, c7, 0  // Invalidate all caches & BTB.
//        mcr     p15, 0, reg, c7, c10, 4 // Drain write buffers.
//    }
//
//    //--//
//
//    reg = 0xFFFFFFFF;
//    __asm
//    {
//        mcr     p15, 0, reg, c3, c0, 0  // Domain access control, set Manager access for 16 domains.
//    }
//}

void BootstrapCode_MMU()
{
    // Fill Translation table with faults.
    ARM9_MMU::InitializeL1(c_Bootstrap_BaseOfTTBs);

    // Direct map for the APB registers (0xFFF00000 ~ 0xFFFFFFFF)
     ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_Register_Begin,                             // mapped address
        c_Bootstrap_Register_Begin,                             // physical address
        c_Bootstrap_Register_End - c_Bootstrap_Register_Begin,  // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

    // Direct map SDRAM (cachable)
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_SDRAM_Begin,                                // mapped address
        c_Bootstrap_SDRAM_Begin,                                // physical address
        c_Bootstrap_SDRAM_End - c_Bootstrap_SDRAM_Begin,        // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        TRUE,                                                   // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended   

    // Remap SRAM @0x000000000 (cachable)
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        0x00000000,                                             // mapped address
        c_Bootstrap_SRAM_Begin,                                 // physical address
        c_Bootstrap_SRAM_End - c_Bootstrap_SRAM_Begin,          // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        TRUE,                                                   // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

    // Direct map SRAM (cachable)
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_SRAM_Begin,                                 // mapped address
        c_Bootstrap_SRAM_Begin,                                 // physical address
        c_Bootstrap_SRAM_End - c_Bootstrap_SRAM_Begin,          // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        TRUE,                                                   // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

    // Direct map for the LCD registers(0x00600000~0x006FFFFF)
    	ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        AT91C_BASE_LCDC,                                        // mapped address
        AT91C_BASE_LCDC,                                        // physical address
        ARM9_MMU::c_MMU_L1_size,                                // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

    // Direct map for the FLASH (uncachable)
    	ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_FLASH_Begin,                                // mapped address
        c_Bootstrap_FLASH_Begin,                                // physical address
        c_Bootstrap_FLASH_End - c_Bootstrap_FLASH_Begin,        // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

        // DM9000 DM9000 ETHERNET
    	ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        0x30000000,                                             // mapped address
        0x30000000,                                             // physical address
        ARM9_MMU::c_MMU_L1_size,                                // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

#ifdef PLATFORM_ARM_SAM9RL64_ANY

		// Direct map for the USBHS buffer registers(0x00600000~0x006FFFFF)
    	ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        AT91C_BASE_UDP_DMA,                                        // mapped address
        AT91C_BASE_UDP_DMA,                                        // physical address
        ARM9_MMU::c_MMU_L1_size,                 		      // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended
#endif


    // TODO: UNCOMMENT if FLASH is added on external bus
    //
    // Direct map for the FLASH (cachable)
    //	ARM9_MMU::GenerateL1_Sections( 
    //    c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
    //    c_Bootstrap_FLASH_Begin,                                // mapped address
    //    c_Bootstrap_FLASH_Begin,                                // physical address
    //    c_Bootstrap_FLASH_End - c_Bootstrap_FLASH_Begin,        // length to be mapped
    //    ARM9_MMU::c_AP__Manager,                                // AP
    //    0,                                                      // Domain
    //    TRUE,                                                  // Cacheable
    //    FALSE,                                                  // Buffered
    //    FALSE);                                                 // Extended

    // Direct map for the FLASH (uncachable)
    //	ARM9_MMU::GenerateL1_Sections( 
    //    c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
    //    CPU_GetUncachableAddress(c_Bootstrap_FLASH_Begin),      // mapped address
    //    c_Bootstrap_FLASH_Begin,                                // physical address
    //    c_Bootstrap_FLASH_End - c_Bootstrap_FLASH_Begin,        // length to be mapped
    //    ARM9_MMU::c_AP__Manager,                                // AP
    //    0,                                                      // Domain
    //    FALSE,                                                  // Cacheable
    //    FALSE,                                                  // Buffered
    //    FALSE);                                                 // Extended
    // 

	// Direct map for RLP in SDRAM (cachable)
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_RLP_Virtual_Address_Cached,                           // mapped address
        c_RLP_Physical_Address,									// physical address
        (1024 * 1024),											// length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        TRUE,                                                   // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended   

    // Direct map for RLP in SDRAM (uncachable)
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_RLP_Virtual_Address_Uncached,                         // mapped address
        c_RLP_Physical_Address,									// physical address
        (1024 * 1024),											// length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended   

    CPU_FlushCaches();
    CPU_EnableMMU( c_Bootstrap_BaseOfTTBs );
}


static void __section(SectionForBootstrapOperations) GPrepare_Copy( UINT32* src, UINT32* dst, UINT32 len )
{
    if(dst != src)
    {
        while(len)
        {
            *dst++ = *src++;

            len -= 4;
        }
    }
}

#define IMAGE_RAM_RO_BASE   Image$$ER_RAM_RO$$Base
extern UINT32 IMAGE_RAM_RO_BASE;

#define IMAGE_RAM_RO_LENGTH Image$$ER_RAM_RO$$Length
extern UINT32 IMAGE_RAM_RO_LENGTH;

void BootstrapCode ()
{

#ifdef PLATFORM_ARM_SAM9RL64_ANY
	//disable watchdog timer
	
	AT91_WATCHDOG& g_WDT = AT91::WTDG();
	
	g_WDT.WDT_MR = AT91_WATCHDOG::WMR_WDDIS;
	//disable watchdog timer
	//*((volatile UINT32*) 0xFFFFFD44) = 0x8000;
#endif 

    AT91_SAM_ClockInit();

    CPU_ARM9_BootstrapCode();

#if !defined(TARGETLOCATION_RAM)
    //AT91_SAM_SdramInit();
#endif

    BootstrapCode_MMU();    

    PrepareImageRegions();
//*

     UINT32* RObase = (UINT32*)&IMAGE_RAM_RO_BASE;
     UINT32  ROlen = (UINT32)&IMAGE_RAM_RO_LENGTH;
     if(RObase != 0)
     {
          UINT32* src = (UINT32*)RObase; //0x20000000;
          UINT32* dst = (UINT32*)0x000000; //0x000000;
          UINT32  len =  44; 
          
          GPrepare_Copy( src, dst, len );
     }
//*/

    CPU_EnableCaches();
}

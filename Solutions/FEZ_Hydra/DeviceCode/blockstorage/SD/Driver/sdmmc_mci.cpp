/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Portions Copyright (c) GHI Electronics, LLC.
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <tinyhal.h>
#include "mci.h"
#include "sdmmc_mci.h"
//#include "board.h"
//#include "utility/assert.h"
//#include "utility/trace.h"

#include <string.h>


//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------

// SD card operation states
#define SD_STATE_STBY     0
#define SD_STATE_DATA     1
#define SD_STATE_RCV      2

// Card type
#define UNKNOWN_CARD      0
#define CARD_SD           1
#define CARD_SDHC         2
#define CARD_MMC          3

// Delay between sending MMC commands
#define MMC_DELAY     0x4FF

#define SD_ADDRESS(pSd, address) (((pSd)->cardType == CARD_SDHC) ? \
                                 (address):((address) << SD_BLOCK_SIZE_BIT))


//-----------------------------------------------------------------------------
/// MMC/SD in SPI mode reports R1 status always, and R2 for SEND_STATUS
/// R1 is the low order byte; R2 is the next highest byte, when present.
//-----------------------------------------------------------------------------
#define R1_SPI_IDLE             (1 << 0)
#define R1_SPI_ERASE_RESET      (1 << 1)
#define R1_SPI_ILLEGAL_COMMAND  (1 << 2)
#define R1_SPI_COM_CRC          (1 << 3)
#define R1_SPI_ERASE_SEQ        (1 << 4)
#define R1_SPI_ADDRESS          (1 << 5)
#define R1_SPI_PARAMETER        (1 << 6)
// R1 bit 7 is always zero
#define R2_SPI_CARD_LOCKED      (1 << 0)
#define R2_SPI_WP_ERASE_SKIP    (1 << 1)
#define R2_SPI_LOCK_UNLOCK_FAIL R2_SPI_WP_ERASE_SKIP
#define R2_SPI_ERROR            (1 << 2)
#define R2_SPI_CC_ERROR         (1 << 3)
#define R2_SPI_CARD_ECC_ERROR   (1 << 4)
#define R2_SPI_WP_VIOLATION     (1 << 5)
#define R2_SPI_ERASE_PARAM      (1 << 6)
#define R2_SPI_OUT_OF_RANGE     (1 << 7)
#define R2_SPI_CSD_OVERWRITE    R2_SPI_OUT_OF_RANGE

// Status register constants
#define STATUS_READY_FOR_DATA   (1 << 8)
#define STATUS_IDLE             (0 << 9)
#define STATUS_READY            (1 << 9)
#define STATUS_IDENT            (2 << 9)
#define STATUS_STBY             (3 << 9)
#define STATUS_TRAN             (4 << 9)
#define STATUS_DATA             (5 << 9)
#define STATUS_RCV              (6 << 9)
#define STATUS_PRG              (7 << 9)
#define STATUS_DIS              (8 << 9)
#define STATUS_STATE          (0xF << 9)

//-----------------------------------------------------------------------------
/// OCR Register
//-----------------------------------------------------------------------------
#define AT91C_VDD_16_17          (1 << 4)
#define AT91C_VDD_17_18          (1 << 5)
#define AT91C_VDD_18_19          (1 << 6)
#define AT91C_VDD_19_20          (1 << 7)
#define AT91C_VDD_20_21          (1 << 8)
#define AT91C_VDD_21_22          (1 << 9)
#define AT91C_VDD_22_23          (1 << 10)
#define AT91C_VDD_23_24          (1 << 11)
#define AT91C_VDD_24_25          (1 << 12)
#define AT91C_VDD_25_26          (1 << 13)
#define AT91C_VDD_26_27          (1 << 14)
#define AT91C_VDD_27_28          (1 << 15)
#define AT91C_VDD_28_29          (1 << 16)
#define AT91C_VDD_29_30          (1 << 17)
#define AT91C_VDD_30_31          (1 << 18)
#define AT91C_VDD_31_32          (1 << 19)
#define AT91C_VDD_32_33          (1 << 20)
#define AT91C_VDD_33_34          (1 << 21)
#define AT91C_VDD_34_35          (1 << 22)
#define AT91C_VDD_35_36          (1 << 23)
#define AT91C_CARD_POWER_UP_BUSY (1 << 31)

#define AT91C_MMC_HOST_VOLTAGE_RANGE     (AT91C_VDD_27_28 +\
                                          AT91C_VDD_28_29 +\
                                          AT91C_VDD_29_30 +\
                                          AT91C_VDD_30_31 +\
                                          AT91C_VDD_31_32 +\
                                          AT91C_VDD_32_33)
#define AT91C_CCS    (1 << 30)

// MCI_CMD Register Value
#define AT91C_POWER_ON_INIT         (0  | AT91C_MCI_TRCMD_NO    | AT91C_MCI_SPCMD_INIT | AT91C_MCI_OPDCMD)

//-----------------------------------------------------------------------------
// Command Classes
//-----------------------------------------------------------------------------
//
// Class 0, 2, 4, 5, 7 and 8 are mandatory and shall be supported by all SD Memory Cards.
// Basic Commands (class 0)
//
// Cmd0 MCI + SPI
#define   AT91C_GO_IDLE_STATE_CMD     (0 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE )
// Cmd1 SPI
#define   AT91C_MMC_SEND_OP_COND_CMD  (1 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48 | AT91C_MCI_OPDCMD)
// Cmd2 MCI
#define   AT91C_ALL_SEND_CID_CMD      (2 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_136 )
// Cmd3 MCI
#define   AT91C_SET_RELATIVE_ADDR_CMD (3 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48 | AT91C_MCI_MAXLAT )
// Cmd4 MCI
//#define AT91C_SET_DSR_CMD             (4 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_NO | AT91C_MCI_MAXLAT )
// cmd7 MCI
#define   AT91C_SEL_DESEL_CARD_CMD    (7  | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48 | AT91C_MCI_MAXLAT )
// Cmd8 MCI + SPI
#define   AT91C_SEND_IF_COND          (8  | AT91C_MCI_TRCMD_NO    | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48  | AT91C_MCI_MAXLAT )
// Cmd9 MCI + SPI
#define   AT91C_SEND_CSD_CMD          (9  | AT91C_MCI_TRCMD_NO    | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_136 | AT91C_MCI_MAXLAT )
// Cmd10 MCI + SPI
#define   AT91C_SEND_CID_CMD          (10 | AT91C_MCI_TRCMD_NO    | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_136 | AT91C_MCI_MAXLAT )
// Cmd12 MCI + SPI
#define   AT91C_STOP_TRANSMISSION_CMD (12 | AT91C_MCI_TRCMD_STOP  | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48  | AT91C_MCI_MAXLAT )
// Cmd13 MCI + SPI
#define   AT91C_SEND_STATUS_CMD       (13 | AT91C_MCI_TRCMD_NO    | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48  | AT91C_MCI_MAXLAT )
// Cmd15 MCI
//#define AT91C_GO_INACTIVE_STATE_CMD   (15 | AT91C_MCI_RSPTYP_NO )
// Cmd58 SPI
#define   AT91C_READ_OCR_CMD          (58 | AT91C_MCI_RSPTYP_48   | AT91C_MCI_SPCMD_NONE | AT91C_MCI_MAXLAT )
// Cmd59 SPI
#define   AT91C_CRC_ON_OFF_CMD        (59 | AT91C_MCI_RSPTYP_48   | AT91C_MCI_SPCMD_NONE | AT91C_MCI_MAXLAT )
//#define AT91C_MMC_ALL_SEND_CID_CMD      (2 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_136| AT91C_MCI_OPDCMD)
//#define AT91C_MMC_SET_RELATIVE_ADDR_CMD (3 | AT91C_MCI_TRCMD_NO | AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48 | AT91C_MCI_MAXLAT | AT91C_MCI_OPDCMD)
//#define AT91C_MMC_READ_DAT_UNTIL_STOP_CMD (11 | AT91C_MCI_TRTYP_STREAM| AT91C_MCI_SPCMD_NONE | AT91C_MCI_RSPTYP_48  | AT91C_MCI_TRDIR  | AT91C_MCI_TRCMD_START | AT91C_MCI_MAXLAT )
//#define AT91C_STOP_TRANSMISSION_SYNC_CMD  (12 | AT91C_MCI_TRCMD_STOP  | AT91C_MCI_SPCMD_SYNC | AT91C_MCI_RSPTYP_48  | AT91C_MCI_MAXLAT )

//*------------------------------------------------
//* Class 2 commands: Block oriented Read commands
//*------------------------------------------------
// Cmd16
#define AT91C_SET_BLOCKLEN_CMD          (16 | AT91C_MCI_TRCMD_NO    | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_MAXLAT )
// Cmd17
#define AT91C_READ_SINGLE_BLOCK_CMD     (17 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_START | AT91C_MCI_TRTYP_BLOCK | AT91C_MCI_TRDIR   | AT91C_MCI_MAXLAT)
// Cmd18
#define AT91C_READ_MULTIPLE_BLOCK_CMD   (18 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_START | AT91C_MCI_TRTYP_MULTIPLE  | AT91C_MCI_TRDIR   | AT91C_MCI_MAXLAT)

//*------------------------------------------------
//* Class 4 commands: Block oriented write commands
//*------------------------------------------------
// Cmd24
#define AT91C_WRITE_BLOCK_CMD           (24 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_START | (AT91C_MCI_TRTYP_BLOCK    &  ~(AT91C_MCI_TRDIR))  | AT91C_MCI_MAXLAT)
// Cmd25
#define AT91C_WRITE_MULTIPLE_BLOCK_CMD  (25 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_START | (AT91C_MCI_TRTYP_MULTIPLE &  ~(AT91C_MCI_TRDIR))  | AT91C_MCI_MAXLAT)
// Cmd27
//#define AT91C_PROGRAM_CSD_CMD           (27 | AT91C_MCI_RSPTYP_48 )

//*----------------------------------------
//* Class 5 commands: Erase commands
//*----------------------------------------
// Cmd32
#define AT91C_TAG_SECTOR_START_CMD          (32 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)
// Cmd33
#define AT91C_TAG_SECTOR_END_CMD            (33 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)
// Cmd38
#define AT91C_ERASE_CMD                     (38 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT )

//*----------------------------------------
//* Class 7 commands: Lock commands
//*----------------------------------------
// Cmd42
//#define AT91C_LOCK_UNLOCK           (42 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT) // not tested

//*-----------------------------------------------
// Class 8 commands: Application specific commands
//*-----------------------------------------------
// Cmd55
#define AT91C_APP_CMD               (55 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO | AT91C_MCI_MAXLAT)
// cmd 56
//#define AT91C_GEN_CMD               (56 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO | AT91C_MCI_MAXLAT)    // not tested
// ACMD6
#define AT91C_SDCARD_SET_BUS_WIDTH_CMD          (6  | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)
// ACMD13
//#define AT91C_SDCARD_STATUS_CMD                 (13 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_START | AT91C_MCI_TRTYP_BLOCK | AT91C_MCI_TRDIR | AT91C_MCI_MAXLAT)
// ACMD22
//#define AT91C_SDCARD_SEND_NUM_WR_BLOCKS_CMD     (22 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)
// ACMD23
//#define AT91C_SDCARD_SET_WR_BLK_ERASE_COUNT_CMD (23 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)
// ACMD41
#define AT91C_SDCARD_APP_OP_COND_CMD            (41 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO )
// ACMD42
//#define AT91C_SDCARD_SET_CLR_CARD_DETECT_CMD    (42 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)
// ACMD51
#define AT91C_SDCARD_SEND_SCR_CMD               (51 | AT91C_MCI_SPCMD_NONE  | AT91C_MCI_RSPTYP_48   | AT91C_MCI_TRCMD_NO    | AT91C_MCI_MAXLAT)

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Sends the current SD card driver command to the card.
/// Returns 0 if successful; Otherwise, returns the transfer status code or
/// SD_ERROR_DRIVER if there was a problem with the SD transfer.
/// \param pSd  Pointer to a SdCard driver instance.
//------------------------------------------------------------------------------
static unsigned char SendCommand(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    SdDriver *pSdDriver = pSd->pSdDriver;
    unsigned char error;
    unsigned int i;

    // Send command
    error = MCI_SendCommand((Mci *)pSdDriver, (MciCmd *)pCommand);
    if (error) {
        TRACE_ERROR("MCI SendCommand: Failed to send command (%d)\n\r", error);
        return SD_ERROR_DRIVER;
    }

    // Wait for command to complete
    while (!MCI_IsTxComplete((MciCmd *)pCommand));

    if(pCommand->cmd == AT91C_STOP_TRANSMISSION_CMD) {
        while (MCI_CheckBusy((Mci *)pSdDriver) != 0);
    }

    // Delay between sending commands, only for MMC card test.
    if((pSd->cardType == CARD_MMC)
     ||(pSd->cardType == UNKNOWN_CARD)
     ||(pSd->cardType == CARD_SD)) {

        for(i=0; i < MMC_DELAY; i++);
    }

    return pCommand->status;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DecodeR1(unsigned char R1)
{
    if( (R1 & R1_SPI_IDLE)==R1_SPI_IDLE) {
        TRACE_DEBUG("R1_SPI_IDLE\n\r");
    }
    if( (R1 & R1_SPI_ERASE_RESET)==R1_SPI_ERASE_RESET) {
        TRACE_DEBUG("R1_SPI_ERASE_RESET\n\r");
    }
    if( (R1 & R1_SPI_ILLEGAL_COMMAND)==R1_SPI_ILLEGAL_COMMAND) {
        TRACE_DEBUG("R1_SPI_ILLEGAL_COMMAND\n\r");
    }
    if( (R1 & R1_SPI_COM_CRC)==R1_SPI_COM_CRC) {
        TRACE_DEBUG("R1_SPI_COM_CRC\n\r");
    }
    if( (R1 & R1_SPI_ERASE_SEQ)==R1_SPI_ERASE_SEQ) {
        TRACE_DEBUG("R1_SPI_ERASE_SEQ\n\r");
    }
    if( (R1 & R1_SPI_ADDRESS)==R1_SPI_ADDRESS) {
        TRACE_DEBUG("R1_SPI_ADDRESS\n\r");
    }
    if( (R1 & R1_SPI_PARAMETER)==R1_SPI_PARAMETER) {
        TRACE_DEBUG("R1_SPI_PARAMETER\n\r");
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DecodeR2(unsigned char R2)
{
    if( (R2 & R2_SPI_CARD_LOCKED)==R2_SPI_CARD_LOCKED) {
        TRACE_DEBUG("R2_SPI_CARD_LOCKED\n\r");
    }
    if( (R2 & R2_SPI_WP_ERASE_SKIP)==R2_SPI_WP_ERASE_SKIP) {
        TRACE_DEBUG("R2_SPI_WP_ERASE_SKIP/R2_SPI_LOCK_UNLOCK_FAIL\n\r");
    }
    if( (R2 & R2_SPI_ERROR)==R2_SPI_ERROR) {
        TRACE_DEBUG("R2_SPI_ERROR\n\r");
    }
    if( (R2 & R2_SPI_CC_ERROR)==R2_SPI_CC_ERROR) {
        TRACE_DEBUG("R2_SPI_CC_ERROR\n\r");
    }
    if( (R2 & R2_SPI_CARD_ECC_ERROR)==R2_SPI_CARD_ECC_ERROR) {
        TRACE_DEBUG("R2_SPI_CARD_ECC_ERROR\n\r");
    }
    if( (R2 & R2_SPI_WP_VIOLATION)==R2_SPI_WP_VIOLATION) {
        TRACE_DEBUG("R2_SPI_WP_VIOLATION\n\r");
    }
    if( (R2 & R2_SPI_ERASE_PARAM)==R2_SPI_ERASE_PARAM) {
        TRACE_DEBUG("R2_SPI_ERASE_PARAM\n\r");
    }
    if( (R2 & R2_SPI_OUT_OF_RANGE)==R2_SPI_OUT_OF_RANGE) {
        TRACE_DEBUG("R2_SPI_OUT_OF_RANGE/R2_SPI_CSD_OVERWRITE\n\r");
    }
}

//------------------------------------------------------------------------------
/// Initialization delay: The maximum of 1 msec, 74 clock cycles and supply ramp
/// up time.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SdCard driver instance.
//------------------------------------------------------------------------------
static unsigned char Pon(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned int response;
    unsigned char error;

    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_POWER_ON_INIT;
    pCommand->pResp = &response;

    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error =  SendCommand(pSd);
    return error;
}

//------------------------------------------------------------------------------
/// Resets all cards to idle state
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SdCard driver instance.
//------------------------------------------------------------------------------
static unsigned char Cmd0(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned int response;
    unsigned char error;

    TRACE_DEBUG("Cmd0()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_GO_IDLE_STATE_CMD;
    pCommand->pResp = &response;

    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // send command
    error =  SendCommand(pSd);
    return error;
}

//------------------------------------------------------------------------------
/// MMC send operation condition command.
/// Sends host capacity support information and activates the card's
/// initialization process.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SdCard driver instance.
//------------------------------------------------------------------------------
static unsigned char Cmd1(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd1()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_MMC_SEND_OP_COND_CMD;
    pCommand->arg = AT91C_MMC_HOST_VOLTAGE_RANGE;
    pCommand->resType = 3;
    pCommand->pResp = &response;

    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // send command
    error = SendCommand(pSd);
    if (error) {
        return error;
    }
    if ((response & AT91C_CARD_POWER_UP_BUSY) == AT91C_CARD_POWER_UP_BUSY) {
        return 0;
    }
    else {
        return SD_ERROR_DRIVER;
    }
}

//------------------------------------------------------------------------------
/// Asks any card to send the CID numbers
/// on the CMD line (any card that is
/// connected to the host will respond)
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param pCid  Buffer for storing the CID numbers.
//------------------------------------------------------------------------------
static unsigned char Cmd2(SdCard *pSd, unsigned int *pCid)
{
    SdCmd *pCommand = &(pSd->command);

    TRACE_DEBUG("Cmd2()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill the command information
    pCommand->cmd = AT91C_ALL_SEND_CID_CMD;
    pCommand->resType = 2;
    pCommand->pResp = pCid;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send the command
    return SendCommand(pSd);
}

//------------------------------------------------------------------------------
/// Ask the card to publish a new relative address (RCA)
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
//------------------------------------------------------------------------------
static unsigned char Cmd3(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned int cardAddress;
    unsigned char error;

    TRACE_DEBUG("Cmd3()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SET_RELATIVE_ADDR_CMD;
    // Assign relative address to MMC card
    if (pSd->cardType == CARD_MMC) {
        pCommand->arg = (0x1 << 16);
    }
    pCommand->resType = 1;
    pCommand->pResp = &cardAddress;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);
    if (error) {
        return error;
    }

    // Save card address in driver
    if (pSd->cardType != CARD_MMC) {
        pSd->cardAddress = (cardAddress >> 16) & 0xFFFF;
    }
    else {
        // Default MMC RCA is 0x0001
        pSd->cardAddress = 1;
    }

    return 0;
}

//------------------------------------------------------------------------------
/// Command toggles a card between the
/// stand-by and transfer states or between
/// the programming and disconnect states.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param address  Relative Card Address (0 deselects all).
//------------------------------------------------------------------------------
static unsigned char Cmd7(SdCard *pSd, unsigned int address)
{
    SdCmd *pCommand = &(pSd->command);

    TRACE_DEBUG("Cmd7()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SEL_DESEL_CARD_CMD;
    pCommand->arg = address << 16;
    pCommand->resType = 1;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    return SendCommand(pSd);
}

//------------------------------------------------------------------------------
/// Sends SD Memory Card interface
/// condition, which includes host supply
/// voltage information and asks the card
/// whether card supports voltage.
/// Returns 0 if successful; otherwise returns SD_ERROR_NORESPONSE if the card did
/// not answer the command, or SD_ERROR_DRIVER.
/// \param pSd  Pointer to a SD card driver instance.
/// \param supplyVoltage  Expected supply voltage.
//------------------------------------------------------------------------------
static unsigned char Cmd8(SdCard *pSd, unsigned char supplyVoltage)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned int response[2];
    unsigned char error;

    TRACE_DEBUG("Cmd8()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SEND_IF_COND;
    pCommand->arg = (supplyVoltage << 8) | (0xAA);
    pCommand->resType = 7;
    pCommand->pResp = &response[0];
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    TRACE_DEBUG("supplyVoltage: 0x%x\n\r", supplyVoltage);

    // Send command
    error = SendCommand(pSd);

    // Check result
    if (error == MCI_STATUS_NORESPONSE) {
        return SD_ERROR_NORESPONSE;
    }
    // SD_R7
    // Bit 0 - 7: check pattern
    // Bit 8 -11: voltage accepted
    else if (!error && ((response[0] & 0x00000FFF) == ((supplyVoltage << 8) | 0xAA))) {
        return 0;
    }
    else {
        return SD_ERROR_DRIVER;
    }
}

//------------------------------------------------------------------------------
/// Addressed card sends its card-specific
/// data (CSD) on the CMD line.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
//------------------------------------------------------------------------------
static unsigned char Cmd9(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;

    TRACE_DEBUG("Cmd9()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SEND_CSD_CMD;
    pCommand->arg = pSd->cardAddress << 16;
    pCommand->resType = 2;
    pCommand->pResp = pSd->csd;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);
    return error;
}


//------------------------------------------------------------------------------
/// Forces the card to stop transmission
/// \param pSd  Pointer to a SD card driver instance.
/// \param pStatus  Pointer to a status variable.
//------------------------------------------------------------------------------
static unsigned char Cmd12(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd12()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_STOP_TRANSMISSION_CMD;
    pCommand->conTrans = MCI_NEW_TRANSFER;
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

//------------------------------------------------------------------------------
/// Addressed card sends its status register.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param pStatus  Pointer to a status variable.
//------------------------------------------------------------------------------
static unsigned char Cmd13(SdCard *pSd, unsigned int *pStatus)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;

    TRACE_DEBUG("Cmd13()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SEND_STATUS_CMD;
    pCommand->arg = pSd->cardAddress << 16;
    pCommand->resType = 1;
    pCommand->pResp = pStatus;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

//------------------------------------------------------------------------------
/// In the case of a Standard Capacity SD Memory Card, this command sets the
/// block length (in bytes) for all following block commands (read, write, lock).
/// Default block length is fixed to 512 Bytes.
/// Set length is valid for memory access commands only if partial block read
/// operation are allowed in CSD.
/// In the case of a High Capacity SD Memory Card, block length set by CMD16
/// command does not affect the memory read and write commands. Always 512
/// Bytes fixed block length is used. This command is effective for LOCK_UNLOCK command.
/// In both cases, if block length is set larger than 512Bytes, the card sets the
/// BLOCK_LEN_ERROR bit.
/// \param pSd  Pointer to a SD card driver instance.
/// \param blockLength  Block length in bytes.
//------------------------------------------------------------------------------
static unsigned char Cmd16(SdCard *pSd, unsigned short blockLength)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd16()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SET_BLOCKLEN_CMD;
    pCommand->arg = blockLength;
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

// erase block start    
unsigned char Cmd32(SdCard *pSd, unsigned short startsector)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    //TRACE_DEBUG("Cmd16()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_TAG_SECTOR_START_CMD;
    pCommand->arg = startsector;
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

// erase block end    
unsigned char Cmd33(SdCard *pSd, unsigned short endsector)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    //TRACE_DEBUG("Cmd16()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_TAG_SECTOR_END_CMD;
    pCommand->arg = endsector;
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

// erase 
unsigned char Cmd38(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    //TRACE_DEBUG("Cmd16()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_ERASE_CMD;
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

//------------------------------------------------------------------------------
/// Continously transfers datablocks from card to host until interrupted by a
/// STOP_TRANSMISSION command.
/// \param pSd  Pointer to a SD card driver instance.
/// \param blockSize  Block size (shall be set to 512 in case of high capacity).
/// \param pData  Pointer to the application buffer to be filled.
/// \param address  SD card address.
//------------------------------------------------------------------------------
static unsigned char Cmd18(SdCard *pSd,
                           unsigned short nbBlock,
                           unsigned char *pData,
                           unsigned int address)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd18()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_READ_MULTIPLE_BLOCK_CMD;
    pCommand->arg = address;
    pCommand->blockSize = SD_BLOCK_SIZE;
    pCommand->nbBlock = nbBlock;
    pCommand->pData = pData;
    pCommand->isRead = 1;
    pCommand->conTrans = MCI_NEW_TRANSFER;
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_DATA;

    // Send command
    error = SendCommand(pSd);

    return error;
}

//------------------------------------------------------------------------------
/// Write block command
/// \param pSd  Pointer to a SD card driver instance.
/// \param blockSize  Block size (shall be set to 512 in case of high capacity).
/// \param pData  Pointer to the application buffer to be filled.
/// \param address  SD card address.
//------------------------------------------------------------------------------
static unsigned char Cmd25(SdCard *pSd,
                           unsigned short nbBlock,
                           unsigned char *pData,
                           unsigned int address)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd25()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_WRITE_MULTIPLE_BLOCK_CMD;
    pCommand->arg = address;
    pCommand->blockSize = SD_BLOCK_SIZE;
    pCommand->nbBlock = nbBlock;
    pCommand->pData = (unsigned char *) pData;
    pCommand->conTrans = MCI_NEW_TRANSFER;
    pCommand->resType = 1;
    pCommand->pResp = &response;

    // Set SD command state
    pSd->state = SD_STATE_RCV;

    // Send command
    //return SendCommand(pSd);
    error = SendCommand(pSd);
    return error;
}


//------------------------------------------------------------------------------
/// Initialization delay: The maximum of 1 msec, 74 clock cycles and supply
/// ramp up time.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
//------------------------------------------------------------------------------
static unsigned char Cmd55(SdCard *pSd)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd55()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_APP_CMD;
    pCommand->arg = (pSd->cardAddress << 16);
    pCommand->resType = 1;
    pCommand->pResp = &response;
    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    //return SendCommand(pSd);
    error = SendCommand(pSd);

    return error;
}

//------------------------------------------------------------------------------
/// SPI Mode, Reads the OCR register of a card
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param pOcr   OCR value of the card
//------------------------------------------------------------------------------
static unsigned char Cmd58(SdCard *pSd, unsigned int *pOcr)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response[2];

    TRACE_DEBUG("Cmd58()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_READ_OCR_CMD;
    pCommand->resType = 3;
    pCommand->pResp = &response[0];

    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);
    return error;
}

//------------------------------------------------------------------------------
/// SPI Mode, Set CRC option of a card
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param option  CRC option, 1 to turn on, 0 to trun off
//------------------------------------------------------------------------------
static unsigned char Cmd59(SdCard *pSd, unsigned char option)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Cmd59()\n\r");
    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_CRC_ON_OFF_CMD;
    pCommand->arg = (option & 0x1);
    pCommand->resType = 1;
    pCommand->pResp = &response;

    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    error = SendCommand(pSd);

    return error;
}

//------------------------------------------------------------------------------
/// Defines the data bus width (’00’=1bit or ’10’=4 bits bus) to be used for data transfer.
/// The allowed data bus widths are given in SCR register.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param busWidth  Bus width in bits.
//------------------------------------------------------------------------------
static unsigned char Acmd6(SdCard *pSd, unsigned char busWidth)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    TRACE_DEBUG("Acmd6()\n\r");
    // Delay
    error = Cmd55(pSd);
    if (error) {
        return error;
    }

    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->cmd = AT91C_SDCARD_SET_BUS_WIDTH_CMD;
    if (busWidth == 4) {
        pCommand->arg = SD_SCR_BUS_WIDTH_4BITS;
    }
    else {
        pCommand->arg = SD_SCR_BUS_WIDTH_1BIT;
    }
    pCommand->resType = 1;
    pCommand->pResp = &response;

    // Set SD command state
    pSd->state = SD_STATE_STBY;

    // Send command
    return SendCommand(pSd);
}

//------------------------------------------------------------------------------
/// Asks to all cards to send their operations conditions.
/// Returns the command transfer result (see SendCommand).
/// \param pSd  Pointer to a SD card driver instance.
/// \param hcs  Shall be true if Host support High capacity.
/// \param pCCS  Set the pointed flag to 1 if hcs != 0 and SD OCR CCS flag is set.
//------------------------------------------------------------------------------
static unsigned char Acmd41(SdCard *pSd, unsigned char hcs, unsigned char *pCCS)
{
    SdCmd *pCommand = &(pSd->command);
    unsigned char error;
    unsigned int response;

    do {
        error = Cmd55(pSd);
        if (error) {
            return error;
        }

        memset(pCommand, 0, sizeof(SdCmd));
        // Fill command information
        pCommand->cmd = AT91C_SDCARD_APP_OP_COND_CMD;
        pCommand->arg = AT91C_MMC_HOST_VOLTAGE_RANGE;
        if (hcs) {
            pCommand->arg |= AT91C_CCS;
        }

        pCommand->resType = 3;
        pCommand->pResp = &response;

        // Set SD command state
        pSd->state = SD_STATE_STBY;

        // Send command
        TRACE_DEBUG("Acmd41()\n\r");
        error = SendCommand(pSd);
        if (error) {
            return error;
        }
        *pCCS  = ((response & AT91C_CCS) != 0);
    }
    while ((response & AT91C_CARD_POWER_UP_BUSY) != AT91C_CARD_POWER_UP_BUSY);

    return 0;
}

//------------------------------------------------------------------------------
/// Continue to transfer datablocks from card to host until interrupted by a
/// STOP_TRANSMISSION command.
/// \param pSd  Pointer to a SD card driver instance.
/// \param blockSize  Block size (shall be set to 512 in case of high capacity).
/// \param pData  Pointer to the application buffer to be filled.
/// \param address  SD card address.
//------------------------------------------------------------------------------
static unsigned char ContinuousRead(SdCard *pSd,
                                    unsigned short nbBlock,
                                    unsigned char *pData,
                                    unsigned int address)
{
    SdCmd *pCommand = &(pSd->command);

    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->blockSize = SD_BLOCK_SIZE;
    pCommand->nbBlock = nbBlock;
    pCommand->pData = pData;
    pCommand->isRead = 1;
    pCommand->conTrans = MCI_CONTINUE_TRANSFER;
    // Set SD command state
    pSd->state = SD_STATE_DATA;

    // Send command
    return SendCommand(pSd);
}

//------------------------------------------------------------------------------
/// Continue to transfer datablocks from host to card until interrupted by a
/// STOP_TRANSMISSION command.
/// \param pSd  Pointer to a SD card driver instance.
/// \param blockSize  Block size (shall be set to 512 in case of high capacity).
/// \param pData  Pointer to the application buffer to be filled.
/// \param address  SD card address.
//------------------------------------------------------------------------------
static unsigned char ContinuousWrite(SdCard *pSd,
                                     unsigned short nbBlock,
                                     const unsigned char *pData,
                                     unsigned int address)
{
    SdCmd *pCommand = &(pSd->command);

    memset(pCommand, 0, sizeof(SdCmd));
    // Fill command information
    pCommand->blockSize = SD_BLOCK_SIZE;
    pCommand->nbBlock = nbBlock;
    pCommand->pData = (unsigned char *) pData;
    pCommand->isRead = 0;
    pCommand->conTrans = MCI_CONTINUE_TRANSFER;
    // Set SD command state
    pSd->state = SD_STATE_RCV;

    // Send command
    return SendCommand(pSd);
}

//------------------------------------------------------------------------------
/// Move SD card to transfer state. The buffer size must be at
/// least 512 byte long. This function checks the SD card status register and
/// address the card if required before sending the transfer command.
/// Returns 0 if successful; otherwise returns an code describing the error.
/// \param pSd  Pointer to a SD card driver instance.
/// \param address  Address of the block to transfer.
/// \param nbBlocks Number of blocks to be transfer.
/// \param pData  Data buffer whose size is at least the block size.
/// \param isRead 1 for read data and 0 for write data.
//------------------------------------------------------------------------------
static unsigned char MoveToTransferState(SdCard *pSd,
                                         unsigned int address,
                                         unsigned short nbBlocks,
                                         unsigned char *pData,
                                         unsigned char isRead)
{
    unsigned int status;
    unsigned char error;

    if((pSd->state == SD_STATE_DATA)
    || (pSd->state == SD_STATE_RCV)) {

        error = Cmd12(pSd);
        if (error) {
            return error;
        }
    }
    pSd->preBlock = address + (nbBlocks-1);

    if(isRead) {
        //TRACE_DEBUG("Read\n\r");
        // Wait for card to be ready for data transfers
        do {
            //TRACE_DEBUG("state = 0x%X\n\r", (status & STATUS_STATE) >> 9);
            error = Cmd13(pSd, &status);
            if (error) {
                TRACE_DEBUG("Pb MTTS cmd13\n\r");
                return error;
            }
            if( ((status & STATUS_STATE) == STATUS_IDLE)
              ||((status & STATUS_STATE) == STATUS_READY)
              ||((status & STATUS_STATE) == STATUS_IDENT)) {
                TRACE_DEBUG("Pb Card Identification mode\n\r");
                return SD_ERROR_NOT_INITIALIZED;
            }
            // If the SD card is in sending data state or in receivce data state
            if (((status & STATUS_STATE) == STATUS_RCV) ||
                ((status & STATUS_STATE) == STATUS_DATA) ){

                TRACE_DEBUG("state = 0x%X\n\r", (status & STATUS_STATE) >> 9);
            }
        }
        while (((status & STATUS_READY_FOR_DATA) == 0) ||
              ((status & STATUS_STATE) != STATUS_TRAN));
            ASSERT((status & STATUS_STATE) == STATUS_TRAN,
                "SD Card can't be configured in transfer state 0x%X\n\r", (status & STATUS_STATE)>>9);
        // Read data
        // Move to Sending data state
        error = Cmd18(pSd, nbBlocks, pData, SD_ADDRESS(pSd,address));
        if (error) {
            return error;
        }
    }
    else {
        //TRACE_DEBUG("Write\n\r");
        // Wait for card to be ready for data transfers
        do {
            error = Cmd13(pSd, &status);
            if (error) {
                TRACE_DEBUG("error cmd 13\n\r");
                return error;
            }
        }
        while ((status & STATUS_READY_FOR_DATA) == 0);

        // Move to Sending data state
        error = Cmd25(pSd, nbBlocks, pData, SD_ADDRESS(pSd,address));
        if (error) {
            TRACE_DEBUG("error cmd 25\n\r");
            return error;
        }
    }

    return error;
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Read Block of data in a buffer pointed by pData. The buffer size must be at
/// least 512 byte long. This function checks the SD card status register and
/// address the card if required before sending the read command.
/// Returns 0 if successful; otherwise returns an code describing the error.
/// \param pSd  Pointer to a SD card driver instance.
/// \param address  Address of the block to read.
/// \param nbBlocks Number of blocks to be read.
/// \param pData  Data buffer whose size is at least the block size.
//------------------------------------------------------------------------------
unsigned char SD_ReadBlock(SdCard *pSd,
                           unsigned int address,
                           unsigned short nbBlocks,
                           unsigned char *pData)
{
    unsigned char error;

    SANITY_CHECK(pSd);
    SANITY_CHECK(pData);
    SANITY_CHECK(nbBlocks);

    if((pSd->state == SD_STATE_DATA)
        && ((pSd->preBlock + 1) == address)) {

#if defined(at91rm9200)
        error = Cmd12(pSd);
        if (error) {
            return error;
        }
#else
        TRACE_DEBUG("SD_ReadBlock:ContinuousRead\n\r");
        error = ContinuousRead(pSd, nbBlocks, pData, SD_ADDRESS(pSd,address));
        pSd->preBlock = address + (nbBlocks-1);
#endif
    }
    else {
        error = MoveToTransferState(pSd, address, nbBlocks, pData, 1);
    }
    return error;
}

//------------------------------------------------------------------------------
/// Write Block of data pointed by pData. The buffer size must be at
/// least 512 byte long. This function checks the SD card status register and
/// address the card if required before sending the read command.
/// Returns 0 if successful; otherwise returns an SD_ERROR code.
/// \param pSd  Pointer to a SD card driver instance.
/// \param address  Address of block to write.
/// \param nbBlocks Number of blocks to be read
/// \param pData  Pointer to a 512 bytes buffer to be transfered
//------------------------------------------------------------------------------
unsigned char SD_WriteBlock(SdCard *pSd,
                            unsigned int address,
                            unsigned short nbBlocks,
                            const unsigned char *pData)
{
    unsigned char error;

    SANITY_CHECK(pSd);
    SANITY_CHECK(pData);
    SANITY_CHECK(nbBlocks);

    if((pSd->state == SD_STATE_RCV)
        && ((pSd->preBlock + 1) == address)) {

        TRACE_DEBUG("SD_WriteBlock:ContinuousWrite\n\r");
        error = ContinuousWrite(pSd, nbBlocks, pData, SD_ADDRESS(pSd,address));
        pSd->preBlock = address + (nbBlocks-1);
    }
    else {

        //TRACE_FATAL("SD_WriteBlock:MoveToTransferState\n\r");
        error = MoveToTransferState(pSd, address, nbBlocks,
                                    (unsigned char *)pData, 0);
    }
    return error;
}

//------------------------------------------------------------------------------
/// Run the SDcard SD Mode initialization sequence. This function runs the
/// initialisation procedure and the identification process, then it sets the
/// SD card in transfer state to set the block length and the bus width.
/// Returns 0 if successful; otherwise returns an SD_ERROR code.
/// \param pSd  Pointer to a SD card driver instance.
/// \param pSdDriver  Pointer to SD driver already initialized
//------------------------------------------------------------------------------
unsigned char SD_MCI_Init(SdCard *pSd, SdDriver *pSdDriver)
{
    unsigned int sdCid[4];
    unsigned char isCCSet;
    unsigned char error;
    unsigned int status;
    unsigned char cmd8Retries = 2;
    unsigned char cmd1Retries = 100;

    // The command GO_IDLE_STATE (CMD0) is the software reset command and sets card into Idle State
    // regardless of the current card state.
    error = Cmd0(pSd);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // CMD8 is newly added in the Physical Layer Specification Version 2.00 to support multiple voltage
    // ranges and used to check whether the card supports supplied voltage. The version 2.00 host shall
    // issue CMD8 and verify voltage before card initialization.
    // The host that does not support CMD8 shall supply high voltage range...
    TRACE_DEBUG("Cmd8()\n\r");
    do {
        error = Cmd8(pSd, 1);
    }
    while ((error == SD_ERROR_NORESPONSE) && (cmd8Retries-- > 0));

    if (error == SD_ERROR_NORESPONSE) {
        // No response : Ver2.00 or later SD Memory Card(voltage mismatch)
        // or Ver1.X SD Memory Card
        // or not SD Memory Card

        TRACE_DEBUG("No response to Cmd8\n\r");

        // ACMD41 is a synchronization command used to negotiate the operation voltage range and to poll the
        // cards until they are out of their power-up sequence.
        error = Acmd41(pSd, 0, &isCCSet);
        if (error) {
            // Acmd41 failed : MMC card or unknown card
            error = Cmd0(pSd);
            if (error) {
                TRACE_ERROR("Error during initialization (%d)\n\r", error);
                return error;
            }
            do {
                error = Cmd1(pSd);
            }
            while ((error) && (cmd1Retries-- > 0));
            if (error) {
                TRACE_ERROR("Error during initialization (%d)\n\r", error);
                return error;
            }
            else {
                TRACE_DEBUG("CARD MMC\n\r");
                pSd->cardType = CARD_MMC;
            }
        }
        else {
            if(isCCSet == 0) {
                TRACE_DEBUG("CARD SD\n\r");
                pSd->cardType = CARD_SD;
            }
        }
    }
    else if (!error) {

        // Valid response : Ver2.00 or later SD Memory Card
        error = Acmd41(pSd, 1, &isCCSet);
        if (error) {
            TRACE_ERROR("Error during initialization (%d)\n\r", error);
            return error;
        }
        if (isCCSet) {
            TRACE_DEBUG("CARD SDHC\n\r");
            pSd->cardType = CARD_SDHC;
        }
        else {
            TRACE_DEBUG("CARD SD\n\r");
            pSd->cardType = CARD_SD;
        }
    }
    else {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // The host then issues the command ALL_SEND_CID (CMD2) to the card to get its unique card identification (CID) number.
    // Card that is unidentified (i.e. which is in Ready State) sends its CID number as the response (on the CMD line).
    error = Cmd2(pSd, sdCid);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // Thereafter, the host issues CMD3 (SEND_RELATIVE_ADDR) asks the
    // card to publish a new relative card address (RCA), which is shorter than CID and which is used to
    // address the card in the future data transfer mode. Once the RCA is received the card state changes to
    // the Stand-by State. At this point, if the host wants to assign another RCA number, it can ask the card to
    // publish a new number by sending another CMD3 command to the card. The last published RCA is the
    // actual RCA number of the card.
    error = Cmd3(pSd);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // The host issues SEND_CSD (CMD9) to obtain the Card Specific Data (CSD register),
    // e.g. block length, card storage capacity, etc...
    error = Cmd9(pSd);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // At this stage the Initialization and identification process is achieved
    // The SD card is supposed to be in Stand-by State
    do {
        error = Cmd13(pSd, &status);
        if (error) {
            TRACE_ERROR("Error during initialization (%d)\n\r", error);
            return error;
        }
    }
    while ((status & STATUS_READY_FOR_DATA) == 0);

    // If the 4 bit bus transfer is supported switch to this mode
    // Select the current SD, goto transfer state
    error = Cmd7(pSd, pSd->cardAddress);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    if (pSd->cardType != CARD_MMC) {
        // Switch to 4 bits bus width (All SD Card shall support 1-bit, 4 bitswidth
        error = Acmd6(pSd, 4);
        if (error) {
            TRACE_ERROR("Error during initialization (%d)\n\r", error);
            return error;
        }
    }
    else {
        MCI_SetBusWidth((Mci *)pSdDriver, MCI_SDCBUS_1BIT);
    }
    return 0;
}

//------------------------------------------------------------------------------
/// Run the SDcard SPI Mode initialization sequence. This function runs the
/// initialisation procedure and the identification process, then it sets the SD
/// card in transfer state to set the block length.
/// Returns 0 if successful; otherwise returns an SD_ERROR code.
/// \param pSd  Pointer to a SD card driver instance.
/// \param pSdDriver  Pointer to SD driver already initialized.
//------------------------------------------------------------------------------
unsigned char SD_SPI_Init(SdCard *pSd, SdDriver *pSpi)
{
    unsigned char isCCSet;
    unsigned char error;
    unsigned char cmd8Retries = 2;
    unsigned char cmd1Retries = 1;
    unsigned int pOCR;

    // The command GO_IDLE_STATE (CMD0) is the software reset command and sets card into Idle State
    // regardless of the current card state.
    error = Cmd0(pSd);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // CMD8 is newly added in the Physical Layer Specification Version 2.00 to support multiple voltage
    // ranges and used to check whether the card supports supplied voltage. The version 2.00 host shall
    // issue CMD8 and verify voltage before card initialization.
    // The host that does not support CMD8 shall supply high voltage range...
    do {
        error = Cmd8(pSd, 1);
    }
    while ((error == SD_ERROR_NORESPONSE) && (cmd8Retries-- > 0));

    if (error == SD_ERROR_NORESPONSE) {
        // No response : Ver2.00 or later SD Memory Card(voltage mismatch)
        // or Ver1.X SD Memory Card
        // or not SD Memory Card

        TRACE_DEBUG("No response to Cmd8\n\r");

        // CMD58 ? !
        error = Cmd58(pSd, &pOCR);
        if (error) {
            TRACE_ERROR("Error during initialization (%d), 8\n\r", error);
            return error;
        }

        // ACMD41 is a synchronization command used to negotiate the operation voltage range and to poll the
        // cards until they are out of their power-up sequence.
        error = Acmd41(pSd, 0, &isCCSet);
        if (error) {
            // Acmd41 failed : MMC card or unknown card
            error = Cmd0(pSd);
            if (error) {
                TRACE_ERROR("Error during initialization (%d)\n\r", error);
                return error;
            }
            do {
                error = Cmd1(pSd);
            }
            while ((error) && (cmd1Retries-- > 0));

            if (error) {
                TRACE_ERROR("Error during initialization (%d)\n\r", error);
                return error;
            }
            else {
                pSd->cardType = CARD_MMC;
            }
        }
        else {
            if(isCCSet == 0) {
                TRACE_DEBUG("CARD SD\n\r");
                pSd->cardType = CARD_SD;
            }
        }
    }
    else if (!error) {
        error = Cmd58(pSd, &pOCR);
        if (error) {
            TRACE_ERROR("Error during initialization (%d), 8\n\r", error);
            return error;
        }

        // Valid response : Ver2.00 or later SD Memory Card
        error = Acmd41(pSd, 1, &isCCSet);
        if (error) {
            TRACE_ERROR("Error during initialization (%d)\n\r", error);
            return error;
        }
        error = Cmd58(pSd, &pOCR);
        if (error) {
            TRACE_ERROR("Error during initialization (%d), 8\n\r", error);
            return error;
        }
        if (isCCSet) {
            TRACE_DEBUG("CARD SDHC\n\r");
            pSd->cardType = CARD_SDHC;
        }
        else {
            TRACE_DEBUG("CARD SD\n\r");
            pSd->cardType = CARD_SD;
        }
    }
    else {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    if (pSd->cardType != CARD_MMC) {
        // The host issues CRC_ON_OFF (CMD59) to set data CRC on/off
        // The host can turn the CRC option on and off using the CRC_ON_OFF command (CMD59).
        // Host should enable CRC verification before issuing ACMD41.
        error = Cmd59(pSd,0);  // turn crc option OFF
        if (error) {

            TRACE_ERROR("Error during initialization (%d)\n\r, 59", error);
            return error;
        }
    }

    // The host issues SEND_CSD (CMD9) to obtain the Card Specific Data (CSD register),
    // e.g. block length, card storage capacity, etc...
    error = Cmd9(pSd);
    if (error) {

        TRACE_ERROR("Error during initialization (%d), 9\n\r", error);
        return error;
    }
    return 0;
}
//

//------------------------------------------------------------------------------
/// Run the SDcard initialization sequence. This function runs the initialisation
/// procedure and the identification process, then it sets the SD card in transfer
/// state to set the block length and the bus width.
/// Returns 0 if successful; otherwise returns an SD_ERROR code.
/// \param pSd  Pointer to a SD card driver instance.
/// \param pSdDriver  Pointer to SD driver already initialized.
//------------------------------------------------------------------------------
unsigned char SD_Init(SdCard *pSd, SdDriver *pSdDriver)
{
    unsigned char error;

    //TRACE_DEBUG("SD_Init()\n\r");

    // Initialize SdCard structure
    pSd->pSdDriver = pSdDriver;
    pSd->cardAddress = 0;
    pSd->preBlock = 0xffffffff;
    pSd->state = SD_STATE_STBY;
    pSd->cardType = UNKNOWN_CARD;
    memset(&(pSd->command), 0, sizeof(SdCmd));

    // Initialization delay: The maximum of 1 msec, 74 clock cycles and supply ramp up time
    // ‘Supply ramp up time’ provides the time that the power is built up to the operating level (the bus
    // master supply voltage) and the time to wait until the SD card can accept the first command

    // Power On Init Special Command
    //TRACE_DEBUG("Pon()\n\r");
    error = Pon(pSd);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // After power-on or CMD0, all cards’ CMD lines are in input mode, waiting for start bit of the next command.
    // The cards are initialized with a default relative card address (RCA=0x0000) and with a default
    // driver stage register setting (lowest speed, highest driving current capability).

    error = SD_MCI_Init(pSd, pSdDriver);
    if (error) {
        TRACE_ERROR("Error during initialization (%d)\n\r", error);
        return error;
    }

    // In the case of a Standard Capacity SD Memory Card, this command sets the
    // block length (in bytes) for all following block commands (read, write, lock).
    // Default block length is fixed to 512 Bytes.
    // Set length is valid for memory access commands only if partial block read
    // operation are allowed in CSD.
    // In the case of a High Capacity SD Memory Card, block length set by CMD16
    // command does not affect the memory read and write commands. Always 512
    // Bytes fixed block length is used. This command is effective for LOCK_UNLOCK command.
    // In both cases, if block length is set larger than 512Bytes, the card sets the
    // BLOCK_LEN_ERROR bit.
    if (pSd->cardType == CARD_SD) {
        error = Cmd16(pSd, SD_BLOCK_SIZE);
        if (error) {
            TRACE_ERROR("Error during initialization (%d)\n\r", error);
            return error;
        }
    }

    // If SD CSD v2.0
    if((pSd->cardType != CARD_MMC) && (SD_CSD_STRUCTURE(pSd) == 1)) {
        pSd->totalSize = SD_CSD_TOTAL_SIZE_HC(pSd);
        pSd->blockNr = SD_CSD_BLOCKNR_HC(pSd);
    }
    else {
        pSd->totalSize = SD_CSD_TOTAL_SIZE(pSd);
        pSd->blockNr = SD_CSD_BLOCKNR(pSd);
    }

    if (pSd->cardType == UNKNOWN_CARD) {
        return SD_ERROR_NOT_INITIALIZED;
    }
    else {
        return 0;
    }
}

//------------------------------------------------------------------------------
/// Stop the SDcard. This function stops all SD operations.
/// Returns 0 if successful; otherwise returns an SD_ERROR code.
/// \param pSd  Pointer to a SD card driver instance.
/// \param pSdDriver  Pointer to MCI driver already initialized.
//------------------------------------------------------------------------------
unsigned char SD_Stop(SdCard *pSd, SdDriver *pSdDriver)
{
    unsigned char error;
    SdCmd *pCommand = &(pSd->command);

    SANITY_CHECK(pSd);
    SANITY_CHECK(pSdDriver);

    if(pCommand->conTrans == MCI_CONTINUE_TRANSFER)
    {
        TRACE_DEBUG("SD_StopTransmission()\n\r");

        error = Cmd12(pSd);
        if(error) {
            return error;
        }
    }

    MCI_Close((Mci *)pSdDriver);
    return 0;
}

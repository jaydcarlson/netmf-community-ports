#include "CC1101.h"
#include "PacketRadio.h"
CC1101_CONFIG g_CC1101_Config;
void CC1101_Config_Initialize()
{
	/*
	struct SPI_CONFIGURATION
	{
	    GPIO_PIN       DeviceCS;
	    BOOL           CS_Active;             // False = LOW active,      TRUE = HIGH active
	    BOOL           MSK_IDLE;              // False = LOW during idle, TRUE = HIGH during idle
	    BOOL           MSK_SampleEdge;        // False = sample falling edge, TRUE = samples on rising
	    BOOL           MD_16bits;
	    UINT32         Clock_RateKHz;
	    UINT32         CS_Setup_uSecs;
	    UINT32         CS_Hold_uSecs;
	    UINT32         SPI_mod;
	    GPIO_FLAG      BusyPin;
	};
	*/

	// SPI settings
        // Angelos REV B
        // Cerberus with CC1101 gadgeteer module on socket 5
        g_CC1101_Config.SPI_Config.DeviceCS = 47; // PC15

        // Angelos REV A
        //g_CC1101_Config.SPI_Config.DeviceCS = 17; // PIN B1

        g_CC1101_Config.GDO0 = 46; // PC14
        g_CC1101_Config.GDO2 = 24; // PB8
	g_CC1101_Config.SPI_Config.CS_Active = FALSE;
	g_CC1101_Config.SPI_Config.MSK_IDLE  = FALSE;
	g_CC1101_Config.SPI_Config.MSK_SampleEdge = TRUE;
	g_CC1101_Config.SPI_Config.MD_16bits = FALSE;
	g_CC1101_Config.SPI_Config.Clock_RateKHz = 1000;
	g_CC1101_Config.SPI_Config.CS_Setup_uSecs = 30;
	g_CC1101_Config.SPI_Config.CS_Hold_uSecs = 30;
	g_CC1101_Config.SPI_Config.SPI_mod = 0;



}

UINT32 CC1101_Config_GetAddress()
{
	return *((UINT32*)0x1FFF7A10); // return the first four bytes of the STM32F4 Unique ID register
}

void CC1101_Config_ConfigureDevice()
{
	// Sync word qualifier mode = 30/32 sync word bits detected
	// CRC autoflush = false
	// Channel spacing = 199.951172
	// Data format = Normal mode
	// Data rate = 76.767
	// RX filter BW = 325.000000
	// PA ramping = false
	// Preamble count = 4
	// Whitening = false
	// Address config = No address check
	// Carrier frequency = 905.998993
	// Device address = 0
	// TX power = 0
	// Manchester enable = false
	// CRC enable = true
	// Deviation = 31.738281
	// Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word
	// Packet length = 255
	// Modulation format = GFSK
	// Base frequency = 901.999969
	// Modulated = true
	// Channel number = 20
	// PA table
	#define PA_TABLE {0x8e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}
	//
	// Rf settings for CC1101
	//
     CC1101_Driver::WriteReg(CC1101_IOCFG2, 0x06);  //GDO2 Output Pin Configuration
     CC1101_Driver::WriteReg(CC1101_IOCFG0, 0x07);  //GDO0 Output Pin Configuration
     CC1101_Driver::WriteReg(CC1101_FIFOTHR, 0x07); //RX FIFO and TX FIFO Thresholds
     CC1101_Driver::WriteReg(CC1101_PKTCTRL1, 0x0C);//Enable status byte appending and CRC autoflushing
     CC1101_Driver::WriteReg(CC1101_PKTCTRL0, 0x05);//Packet Automation Control
     CC1101_Driver::WriteReg(CC1101_CHANNR, 0x14);  //Channel Number
     CC1101_Driver::WriteReg(CC1101_FSCTRL1, 0x06); //Frequency Synthesizer Control
     CC1101_Driver::WriteReg(CC1101_FREQ2, 0x22);   //Frequency Control Word, High Byte
     CC1101_Driver::WriteReg(CC1101_FREQ1, 0xB1);   //Frequency Control Word, Middle Byte
     CC1101_Driver::WriteReg(CC1101_FREQ0, 0x3B);   //Frequency Control Word, Low Byte
     CC1101_Driver::WriteReg(CC1101_MDMCFG4, 0x6B); //Modem Configuration
     CC1101_Driver::WriteReg(CC1101_MDMCFG3, 0x83); //Modem Configuration
     CC1101_Driver::WriteReg(CC1101_MDMCFG2, 0x13); //Modem Configuration
     CC1101_Driver::WriteReg(CC1101_DEVIATN, 0x42); //Modem Deviation Setting
     CC1101_Driver::WriteReg(CC1101_MCSM1, 0x00);   //Main Radio Control State Machine Configuration
     CC1101_Driver::WriteReg(CC1101_MCSM0, 0x18);   //Main Radio Control State Machine Configuration
     CC1101_Driver::WriteReg(CC1101_FOCCFG, 0x16);  //Frequency Offset Compensation Configuration
     CC1101_Driver::WriteReg(CC1101_WORCTRL, 0xFB); //Wake On Radio Control
     CC1101_Driver::WriteReg(CC1101_FSCAL3, 0xEA);  //Frequency Synthesizer Calibration
     CC1101_Driver::WriteReg(CC1101_FSCAL2, 0x2A);  //Frequency Synthesizer Calibration
     CC1101_Driver::WriteReg(CC1101_FSCAL1, 0x00);  //Frequency Synthesizer Calibration
     CC1101_Driver::WriteReg(CC1101_FSCAL0, 0x1F);  //Frequency Synthesizer Calibration
     CC1101_Driver::WriteReg(CC1101_TEST2, 0x81);   //Various Test Settings
     CC1101_Driver::WriteReg(CC1101_TEST1, 0x35);   //Various Test Settings
     CC1101_Driver::WriteReg(CC1101_TEST0, 0x09);   //Various Test Settings

}

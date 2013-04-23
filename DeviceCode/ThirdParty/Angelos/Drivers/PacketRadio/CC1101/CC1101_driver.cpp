#include <tinyhal.h>
#include "CC1101.h"
#include "../PacketRadio.h"
// Uncomment that if you want to get the debugging messages on the uart 0
#define __CC1101_DEBUG__

// Maximum packet size set to 61 bytes
#define __CC1101_MAX_PACKET_SIZE__ 0x3D

// How many times should I try to send a packet :)
#define __CC1101_TX_RETRIES__ 255

// DELAY between the sending retries
#define __CC1101_TX_INTERVAL_US__ 27

namespace CC1101_Driver {
         UINT8 TXBuffer[67] = { 0 };
         UINT8 RXBuffer[67] = { 0 };
         volatile BOOL TxSucc = false;
         UINT8 State = 0;
         UINT8 BytesToRead = 0;
         UINT8 TxRetries = 0;
         UINT32 Address;



        void Initialize()
        {
          USART_Initialize( ConvertCOM_ComPort(USART_DEFAULT_PORT), HalSystemConfig.USART_DefaultBaudRate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE);
#ifdef __CC1101_DEBUG__
          USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "Initializing CC1101\n", 20);
#endif
                CC1101_Config_Initialize();
                ResetChip();
                CC1101_Config_ConfigureDevice();

                // We really need these interrupts to be wired properly for this driver to work, so go ahead and (possibly) override the user's IOCFG0 and IOCFG2 settings

                WriteReg(CC1101_IOCFG0, 0x07);  //GDO0 Output Pin Configuration

                /*
                 * Very important stuff
                 * CCA_MODE=2 - only  transmit when not receiving
                 * RXOFF_MODE = 3 - stay in RX after receiving
                 * TXOFF_MODE = 0 - stay in IDLE after transmitting
                */
                WriteReg(CC1101_MCSM1, 0x2c);   //Main Radio Control State Machine Configuration

                /*
                 * Set the maximum packet length to 61 bytes
                 */
                WriteReg(CC1101_PKTLEN, __CC1101_MAX_PACKET_SIZE__);
                WriteReg(CC1101_PKTCTRL0, 0x05);

                /**
                 * CRC_AUTOFLUSH = 1
                 * APPEND_STATUS = 1
                 * ADDR_CHECK = 0
                 */
                WriteReg(CC1101_PKTCTRL1, 0x0C);

               // CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO0, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO0_Interrupt), GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED );
               // CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO2, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO2_Interrupt), GPIO_INT_EDGE_LOW, RESISTOR_DISABLED );
                StartListening();
                HAL_Time_Sleep_MicroSeconds_InterruptEnabled( 2000 );
         }

        void StartListening()
        {
         // CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO0, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO0_Interrupt), GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED );
          CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO2, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO2_Interrupt), GPIO_INT_EDGE_LOW, RESISTOR_DISABLED );
          Strobe(CC1101_SRX);
        }

        void StopListening()
        {
          Strobe(CC1101_SIDLE);
         // CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO0, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO0_Interrupt), GPIO_INT_NONE, RESISTOR_DISABLED );
          CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO2, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO2_Interrupt), GPIO_INT_NONE, RESISTOR_DISABLED );
        }

        //----------------------------------------------------------------------------------
        //  void CC1101_Driver::ResetChip(void)
        //
        //  DESCRIPTION:
        //    Resets the chip using the procedure described in the datasheet.
        //----------------------------------------------------------------------------------
        void ResetChip(void)
        {

                GPIO_PIN msk, miso, mosi;
                CPU_SPI_GetPins(g_CC1101_Config.SPI_Config.SPI_mod, msk, miso, mosi);
                CPU_GPIO_EnableInputPin( miso, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
                GPIO_PIN cs = g_CC1101_Config.SPI_Config.DeviceCS;
                // Toggle chip select signal

                /* replace this with NETMF code
                HAL_SPI_CS_DEASSERT;
                halMcuWaitUs(30);
                HAL_SPI_CS_ASSERT;
                halMcuWaitUs(30);
                HAL_SPI_CS_DEASSERT;
                halMcuWaitUs(45);
                */

                CPU_GPIO_EnableOutputPin( cs, TRUE );   // HAL_SPI_CS_DEASSERT;
                HAL_Time_Sleep_MicroSeconds_InterruptEnabled( 30 );                                             // halMcuWaitUs(30);
                CPU_GPIO_SetPinState( cs, FALSE );              // HAL_SPI_CS_ASSERT;
                HAL_Time_Sleep_MicroSeconds_InterruptEnabled( 30 );                                             // halMcuWaitUs(30);
                CPU_GPIO_SetPinState( cs, TRUE );               // HAL_SPI_CS_DEASSERT;
                HAL_Time_Sleep_MicroSeconds_InterruptEnabled( 45 );                                             // halMcuWaitUs(45);



                // Send SRES command

                /* replace this with NETMF code
                HAL_SPI_CS_ASSERT;
                while(HAL_SPI_SOMI_VAL);
                HAL_SPI_TXBUF_SET(CC1101_SRES);
                HAL_SPI_WAIT_TXFIN;
                */

                CPU_GPIO_SetPinState(cs, FALSE );               // HAL_SPI_CS_ASSERT;
                while(CPU_GPIO_GetPinState(miso));                                                                              // while(HAL_SPI_SOMI_VAL);
                CC1101_Driver::Strobe(CC1101_SRES);                                                                             // HAL_SPI_TXBUF_SET(CC1101_SRES);


                // Wait for chip to finish internal reset
                /* replace with NETMF code
                while (HAL_SPI_SOMI_VAL);
                HAL_SPI_CS_DEASSERT;
                */
                CPU_GPIO_EnableInputPin( miso, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
                HAL_Time_Sleep_MicroSeconds_InterruptEnabled( 45 );
                CPU_GPIO_SetPinState( cs, TRUE );

        }

        /* asserts when packet is received with CRC OK */
        void GDO0_Interrupt(GPIO_PIN Pin, BOOL PinState, void* pArg)
        {
          /*
          BytesToRead = ReadReg(CC1101_RXFIFO) + 2;     // +2 for the RSSI and LQI
          ReadFifo(RXBuffer, BytesToRead);
          PacketRadio_OnDataReceived(RXBuffer, BytesToRead);

#ifdef __CC1101_DEBUG__
          USART_Write(0, "GDO0\n", 5);
          USART_Write(0, (const char*)&RXBuffer[10], 1);
          USART_Write(0, "\n", 1);
#endif
*/
        }

        /* GDO2 is wired to configuration 6, which is: asserts when sync word has been sent / received; deasserts at end of the packet.
        This interrupt is set to fire when the pin de-asserts, therefore, this will fire when the packet is done being transmitted.
        We'll use this interrupt to tell us when we're done transmitting so we can go into receive mode */
        void GDO2_Interrupt(GPIO_PIN Pin, BOOL PinState, void* pArg)
        {
#ifdef __CC1101_DEBUG__
          USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "GDO2\n", 5);
#endif
          HAL_Time_Sleep_MicroSeconds_InterruptEnabled(10);
          if(!CPU_GPIO_GetPinState(g_CC1101_Config.GDO0)) // we have autoflushing enabled, so we should never come here?
          {
#ifdef __CC1101_DEBUG__
              USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "BAD CRC. Flushing...\n", 21);
#endif
              StopListening();
              Strobe(CC1101_SFRX);
              Strobe(CC1101_SFTX);
              StartListening();
          } else { // good CRC
              BytesToRead = ReadReg(CC1101_RXFIFO) + 2;     // +2 for the RSSI and LQI
              ReadFifo(RXBuffer, BytesToRead);
              PacketRadio_OnDataReceived(RXBuffer, BytesToRead);
              StartListening();
              #ifdef __CC1101_DEBUG__
                USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "Packet Received\n", 16);
                USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), (const char*)&RXBuffer[10], 1);
                USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "\n", 1);
              #endif
          }

          /*
          State = Strobe(CC1101_SNOP) & CC1101_STATUS_STATE_BM;

            // Transmitted successfully
            if(State == CC1101_STATE_IDLE) {
                TxSucc = true;
            }else {
                HandleOverflow();
            }
            StartListening();
            */
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::GetChipId(void)
        //----------------------------------------------------------------------------------
        UINT8 GetChipId(void)
        {
                return(ReadStatusReg(CC1101_PARTNUM));
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::GetChipVer(void)
        //----------------------------------------------------------------------------------
        UINT8 GetChipVer(void)
        {
                return(ReadStatusReg(CC1101_VERSION));
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::Strobe(UINT8 cmd)
        //----------------------------------------------------------------------------------
        UINT8 Strobe(UINT8 cmd)
        {
                // return(halSpiStrobe(cmd));
                UINT8 retVal = 0;
                CPU_SPI_nWrite8_nRead8(g_CC1101_Config.SPI_Config, &cmd, 1, &retVal, 1, 0);
                return(retVal);
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::ReadStatusReg(UINT8 addr)
        //
        //  NOTE:
        //      When reading a status register over the SPI interface while the register
        //      is updated by the radio hardware, there is a small, but finite, probability
        //      that the result is corrupt. The CC1100 and CC1100 errata notes explain the
        //      problem and propose several workarounds.
        //
        //----------------------------------------------------------------------------------
        UINT8 ReadStatusReg(UINT8 addr)
        {
                UINT8 regVal;
                // halSpiRead(addr | CC1101_READ_BURST, &reg, 1);
                UINT8 send[] = {addr  | CC1101_READ_BURST, 0x00};
                UINT8 receive[] = { 0x00, 0x00};
                CPU_SPI_nWrite8_nRead8(g_CC1101_Config.SPI_Config, send, 2, receive, 2, 0);
                regVal = receive[1];
                return(regVal);
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::ReadReg(UINT8 addr)
        //----------------------------------------------------------------------------------
        UINT8 ReadReg(UINT8 addr)
        {
           // halSpiRead(addr | CC1101_READ_SINGLE, &reg, 1);
                TXBuffer[0] = addr  | CC1101_READ_SINGLE;
                TXBuffer[1] = 0x00;
                CPU_SPI_nWrite8_nRead8(g_CC1101_Config.SPI_Config, TXBuffer, 2, RXBuffer, 2, 0);
                return(RXBuffer[1]);
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::WriteReg(UINT8 addr, UINT8 data)
        //----------------------------------------------------------------------------------
        UINT8 WriteReg(UINT8 addr, UINT8 data)
        {
                // rc = halSpiWrite(addr, &data, 1);
                UINT8 send[] = {addr, data};
                UINT8 receive[] = {0, 0};
           // TXBuffer[0] = addr;
           // TXBuffer[1] = data;
                CPU_SPI_nWrite8_nRead8(g_CC1101_Config.SPI_Config, send, 2, receive, 2, 0);
                return(receive[1]);
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::WriteFifo(UINT8* data, UINT8 length)
        //----------------------------------------------------------------------------------
        UINT8 WriteFifo(UINT8* data, UINT8 length)
        {
          //  return(halSpiWrite(CC1101_TXFIFO | CC1101_WRITE_BURST, data, length));
                TXBuffer[0] = CC1101_TXFIFO | CC1101_WRITE_BURST;
                TXBuffer[1] = length;
                for(UINT32 i = 0; i<length; i++)
                {
                        TXBuffer[i+2] = data[i];
                }

                CPU_SPI_nWrite8_nRead8(g_CC1101_Config.SPI_Config, TXBuffer, length+2, RXBuffer, length+2, 0);
                UINT8 status = RXBuffer[length];
                return(status);
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::ReadFifo(UINT8* data, UINT8 length)
        //----------------------------------------------------------------------------------
        UINT8 ReadFifo(UINT8* data, UINT8 length)
        {
          //  return(halSpiRead(CC1101_RXFIFO | CC1101_READ_BURST, data, length));
          TXBuffer[0] = CC1101_RXFIFO | CC1101_READ_BURST;
          CPU_SPI_nWrite8_nRead8(g_CC1101_Config.SPI_Config, TXBuffer, length+1, data,  length, 1);
          return(0);
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::GetTxStatus(void)
        //
        //  DESCRIPTION:
        //      This function transmits a No Operation Strobe (SNOP) to get the status of
        //      the radio and the number of free bytes in the TX FIFO
        //
        //      Status byte:
        //
        //      ---------------------------------------------------------------------------
        //      |          |            |                                                 |
        //      | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (free bytes in the TX FIFO |
        //      |          |            |                                                 |
        //      ---------------------------------------------------------------------------
        //
        //  NOTE:
        //      When reading a status register over the SPI interface while the register
        //      is updated by the radio hardware, there is a small, but finite, probability
        //      that the result is corrupt. This also applies to the chip status byte. The
        //      CC1100 and CC1100 errata notes explain the problem and propose several
        //      workarounds.
        //
        //----------------------------------------------------------------------------------
        void HandleOverflow(void)
        {
          // Good, Old Jay's code ...
           // return(halSpiStrobe(CC1101_SNOP));
                State = Strobe(CC1101_SNOP) & CC1101_STATUS_STATE_BM;

                if(State == CC1101_STATE_TX_UNDERFLOW)
                            {
                #ifdef __CC1101_DEBUG__
                                USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "TXUF\n", 5);
                #endif

                                   Strobe(CC1101_SFTX);
                            }
                            else if(State == CC1101_STATE_RX_OVERFLOW)
                            {
                #ifdef __CC1101_DEBUG__
                                USART_Write(ConvertCOM_ComPort(USART_DEFAULT_PORT), "RXOF\n", 5);
                #endif
                                   Strobe(CC1101_SFRX);
                            }
        }

        //----------------------------------------------------------------------------------
        //  UINT8 CC1101_Driver::GetRxStatus(void)
        //
        //  DESCRIPTION:
        //      This function transmits a No Operation Strobe (SNOP) with the read bit set
        //      to get the status of the radio and the number of available bytes in the RX
        //      FIFO.
        //
        //      Status byte:
        //
        //      --------------------------------------------------------------------------------
        //      |          |            |                                                      |
        //      | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (available bytes in the RX FIFO |
        //      |          |            |                                                      |
        //      --------------------------------------------------------------------------------
        //
        //  NOTE:
        //      When reading a status register over the SPI interface while the register
        //      is updated by the radio hardware, there is a small, but finite, probability
        //      that the result is corrupt. This also applies to the chip status byte. The
        //      CC1100 and CC1100 errata notes explain the problem and propose several
        //      workarounds.
        //
        //----------------------------------------------------------------------------------
        UINT8 GetRxStatus(void)
        {
           // return(halSpiStrobe(CC1101_SNOP | CC1101_READ_SINGLE));
                return ((Strobe(CC1101_SNOP | CC1101_READ_SINGLE) & CC1101_STATUS_STATE_BM) == CC1101_STATE_RX);
        }

        UINT32 GetMaxPacketSize()
        {
                return __CC1101_MAX_PACKET_SIZE__;
        }

        void SetAddress(UINT32 address)
        {

        }

        /**
         * This function tries to send the data and returns the status
         * it relies on the interrupt setting the TxSucc flag to true when
         * there is a transition from TX->IDLE state on successful sending
         * @returns 1 when success, 0 when false
         */
        UINT8 TransmitData(UINT8* Data, size_t size)
        {
          TxRetries = __CC1101_TX_RETRIES__;
          do {
              if(!CPU_GPIO_GetPinState(g_CC1101_Config.GDO2)) {  // we're not currently receiving. Break the loop
                  break;
              }
              HAL_Time_Sleep_MicroSeconds_InterruptEnabled( __CC1101_TX_INTERVAL_US__ );        // wait a little longer
          } while(--TxRetries > 0);

          // We've waited long enough to receive a packet (or we timed out and got tired of waiting...)
          StopListening();
          // prep the transceiver for Tx

          Strobe(CC1101_SFRX);
          Strobe(CC1101_SFTX);

          // Flush in the data
          WriteFifo(Data, size);

          // Start transmitting
          Strobe(CC1101_STX);
          while(!CPU_GPIO_GetPinState(g_CC1101_Config.GDO2)); // wait for packet to start
          while(CPU_GPIO_GetPinState(g_CC1101_Config.GDO2)); // wait for packet to be sent

          // All done. Let's start listening.
          StartListening();

          return 1;








            /*************
          UINT8 retval = false;
          TxRetries = __CC1101_TX_RETRIES__;
          Strobe(CC1101_SIDLE);
          Strobe(CC1101_SFRX);
          Strobe(CC1101_SFTX);

          // DISABLE INTERRUPT
          CPU_GPIO_EnableInputPin( g_CC1101_Config.GDO0, FALSE, (GPIO_INTERRUPT_SERVICE_ROUTINE) &(CC1101_Driver::GDO0_Interrupt), GPIO_INT_NONE , RESISTOR_DISABLED );

          WriteFifo(Data, size);
          Strobe(CC1101_STX);

          // ENABLE


          do {
              if(CPU_GPIO_GetPinState(g_CC1101_Config.GDO2)) {

                  // wait for the pin to go down
                  while(CPU_GPIO_GetPinState(g_CC1101_Config.GDO2));



                  // successfully sent!
                  retval = true;
                  break;
              }
              HAL_Time_Sleep_MicroSeconds_InterruptEnabled( __CC1101_TX_INTERVAL_US__ );        // wait a little
          }
          while(--TxRetries > 0);

          #ifdef __CC1101_DEBUG__
            if(retval) {
                USART_Write(0, "sent\n", 5);
            } else {
                USART_Write(0, "not sent\n", 9);

            }
          #endif



          StartListening();
          return retval;


*****/
          /*
          TxRetries = __CC1101_TX_RETRIES__;

          WriteFifo(Data, size);
          TxSucc = false;

          do {
              UINT8 status = Strobe(CC1101_STX);
              UINT8 state =  status & CC1101_STATUS_STATE_BM;
              if(state != CC1101_STATE_RX)
                {
                  USART_Write(0, "Wrong state\n", 12);
                  return 0;
                }
              HAL_Time_Sleep_MicroSeconds_InterruptEnabled( __CC1101_TX_INTERVAL_US__ );        // wait a little
              if(TxSucc == true) {
             // if(Strobe(CC1101_SNOP) & CC1101_STATUS_STATE_BM == CC1101_STATE_IDLE) {
#ifdef __CC1101_DEBUG__
                  USART_Write(0, "sent\n", 5);
#endif
                  return 1;
              }
          }while(--TxRetries > 0);

          #ifdef __CC1101_DEBUG__
            USART_Write(0, "not sent\n", 9);
          #endif
            Strobe(CC1101_SIDLE);
            Strobe(CC1101_SFTX);
         // HandleOverflow();
          StartListening();     // come back to the RX mode

          return 0;
          */
        }

}

        //--//  g_CC1101_Config.GD0 = 46; // PC14  g_CC1101_Config.GD2 = 24; // PB8

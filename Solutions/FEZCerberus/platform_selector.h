////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Implementation for FEZCerberus board (STM32F4): Copyright (c) Oberon microsystems, Inc.
//
//  FEZCerberus specific definitions
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_FEZCerberus_SELECTOR_H_
#define _PLATFORM_FEZCerberus_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//

#if defined(PLATFORM_ARM_FEZCerberus)
#define HAL_SYSTEM_NAME                     "FEZCerberus"

#ifndef PLATFORM_ARM_STM32F4
#define PLATFORM_ARM_STM32F4  // STM32F405 cpu
#endif

#define USB_ALLOW_CONFIGURATION_OVERRIDE  1

#define RUNTIME_MEMORY_PROFILE__extrasmall 1


//
// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// Platform Select for shared OSHW drivers
//

#define FEZ_CERBERUS	1
    
//
// Platform Select for shared OSHW drivers
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// Solution info
//

#define GHI_VERSION_MAJOR 4
#define GHI_VERSION_MINOR 2
#define GHI_VERSION_BUILD 3
#define GHI_VERSION_REVISION 3
#define GHIOEMSTRING "Copyright (C) GHI Electronics, LLC"

//
// Solution info
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

// legal SYSCLK: 24, 30, 32, 36, 40, 42, 48, 54, 56, 60, 64, 72, 84, 96, 108, 120, 144, 168MHz
#define SYSTEM_CLOCK_HZ                 168000000  // 168MHz
#define SYSTEM_CYCLE_CLOCK_HZ           168000000  // 168MHz
#define SYSTEM_APB1_CLOCK_HZ            42000000   // 42MHz
#define SYSTEM_APB2_CLOCK_HZ            84000000   // 84MHz
#define SYSTEM_CRYSTAL_CLOCK_HZ         12000000   // 12MHz external clock
#define SUPPLY_VOLTAGE_MV               3300       // 3.3V supply

#define CLOCK_COMMON_FACTOR             1000000    // GCD(SYSTEM_CLOCK_HZ, 1M)

#define SLOW_CLOCKS_PER_SECOND          1000000    // 1MHz
#define SLOW_CLOCKS_TEN_MHZ_GCD         1000000    // GCD(SLOW_CLOCKS_PER_SECOND, 10M)
#define SLOW_CLOCKS_MILLISECOND_GCD     1000       // GCD(SLOW_CLOCKS_PER_SECOND, 1k)

#define FLASH_MEMORY_Base               0x08000000
#define FLASH_MEMORY_Size               0x00100000
#define SRAM1_MEMORY_Base               0x20000000
#define SRAM1_MEMORY_Size               0x00020000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define TOTAL_GPIO_PORT                 4 // PA - PD
#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#define TOTAL_USART_PORT                3
#define USART_DEFAULT_PORT              COM3
#define USART_DEFAULT_BAUDRATE          115200

#define DEBUG_TEXT_PORT                 USB1
#define STDIO                           USB1
#define DEBUGGER_PORT                   USB1
#define MESSAGING_PORT                  USB1

#define TOTAL_USB_CONTROLLER            1
#define USB_MAX_QUEUES                  4  // 4 endpoints (EP0 + 3)

#define TOTAL_SOCK_PORT                 0


// System Timer Configuration
#define STM32F4_32B_TIMER 5
#define STM32F4_16B_TIMER 12


// Pin Configuration
#define STM32F4_ADC 1     // A6,A2,A3,C0,C1,A4,C2,C3,A5,B0,B1
#define STM32F4_AD_CHANNELS { 6, 2, 3,10,11, 4,12,13, 5, 8, 9}

// Pin Configuration
//#define STM32F4_ADC 1     // A6,A2,A3,C0,C1,A4,C2,C3,A5
//#define STM32F4_AD_CHANNELS { 6, 2, 3,10,11, 4,12,13, 5}

//                         C6,A7,C7,A8,B0,B1,B5,B4,B3,B11,B10,A10,A9,A15, B8, B9
#define STM32F4_PWM_TIMER { 8,14, 8, 1, 3, 3, 3, 3, 2,  2,  2,  1, 1,  2,  4,  4}
#define STM32F4_PWM_CHNL  { 0, 0, 1, 0, 2, 3, 1, 0, 1,  3,  2,  2, 1,  0,  2,  3}
#define STM32F4_PWM_PINS  {38, 7,39, 8,16,17,21,20,19, 27, 26, 10, 9, 15, 24, 25}

//                         C6,A7,C7,A8,B0,B1,B5,B4,B3
//#define STM32F4_PWM_TIMER { 8,14, 8, 1, 3, 3, 3, 3, 2}
//#define STM32F4_PWM_CHNL  { 0, 0, 1, 0, 2, 3, 1, 0, 1}
//#define STM32F4_PWM_PINS  {38, 7,39, 8,16,17,21,20,19}

#define STM32F4_SPI_SCLK_PINS {19} // PB3
#define STM32F4_SPI_MISO_PINS {20} // PB4
#define STM32F4_SPI_MOSI_PINS {21} // PB5

#define STM32F4_I2C_PORT     1
#define STM32F4_I2C_SCL_PIN  22 // PB6
#define STM32F4_I2C_SDA_PIN  23 // PB7

//#define STM32F4_UART_RXD_PINS {23, 3, 27} // B7, A3, B11
//#define STM32F4_UART_TXD_PINS {22, 2, 26} // B6, A2, B10
//#define STM32F4_UART_CTS_PINS {11, 0, 29} // A11, A0, B13
//#define STM32F4_UART_RTS_PINS {12, 1, 30} // A12, A1, B14
#define STM32F4_UART_RXD_PINS {39, 3, 27} // C7, A3, B11
#define STM32F4_UART_TXD_PINS {38, 2, 26} // C6, A2, B10
#define STM32F4_UART_CTS_PINS {109, 0, 29} // G13, A0, B13
#define STM32F4_UART_RTS_PINS {108, 1, 30} // G12, A1, B14

//
// constants
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// global functions
//

//
// global functions
//
/////////////////////////////////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_ARM_FEZCerberus
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_FEZCerberus_SELECTOR_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

BOOL PWM_Initialize(PWM_CHANNEL channel)
{
    return AT91_PWM_Driver::Initialize(channel);
}

BOOL PWM_Uninitialize(PWM_CHANNEL channel)
{
    return AT91_PWM_Driver::Uninitialize(channel);
}

BOOL PWM_ApplyConfiguration(PWM_CHANNEL channel, GPIO_PIN pin, UINT32& period, UINT32& duration, PWM_SCALE_FACTOR& scale, BOOL invert)
{
    return AT91_PWM_Driver::ApplyConfiguration(channel, pin, period, duration, scale, invert);
}

BOOL PWM_Start(PWM_CHANNEL channel, GPIO_PIN pin)
{
    return AT91_PWM_Driver::Start(channel, pin);
}

void PWM_Stop(PWM_CHANNEL channel, GPIO_PIN pin)
{
	AT91_PWM_Driver::Stop(channel, pin);
}

BOOL PWM_Start(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
    return AT91_PWM_Driver::Start(channel, pin, count);
}

void PWM_Stop(PWM_CHANNEL* channel, GPIO_PIN* pin, UINT32 count)
{
	AT91_PWM_Driver::Stop(channel, pin, count);
}

UINT32 PWM_PWMChannels() 
{
    return AT91_PWM_Driver::PWMChannels();
}

GPIO_PIN PWM_GetPinForChannel( PWM_CHANNEL channel )
{
    return AT91_PWM_Driver::GetPinForChannel( channel );
}
//#include <cores\arm\include\cpu.h>
//
//BOOL CPU_SPI_Initialize()
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::Initialize();
//}
//
//void CPU_SPI_Uninitialize()
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    AT91_SPI_Driver::Uninitialize();
//}
//
//BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::nWrite16_nRead16( Configuration, Write16, WriteCount, Read16, ReadCount, ReadStartOffset );
//}
//
//BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::nWrite8_nRead8( Configuration, Write8, WriteCount, Read8, ReadCount, ReadStartOffset );
//}
//
//BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::Xaction_Start( Configuration );
//}
//
//BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::Xaction_Stop( Configuration );
//}
//
//BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::Xaction_nWrite16_nRead16( Transaction );
//}
//
//BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
//{
//    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
//    return AT91_SPI_Driver::Xaction_nWrite8_nRead8( Transaction );
//}
//
//UINT32 CPU_SPI_PortsCount()
//{
//    return AT91_SPI::c_MAX_SPI;
//}
//
//void CPU_SPI_GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
//{
//    return AT91_SPI_Driver::GetPins(spi_mod,msk,miso,mosi);
//}


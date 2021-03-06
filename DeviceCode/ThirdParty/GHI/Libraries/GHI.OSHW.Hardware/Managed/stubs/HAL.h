//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _HAL_H_
#define _HAL_H_

#include <TinyCLR_Interop.h>
struct Library_HAL_GHI_OSHW_Hardware_Util
{
    TINYCLR_NATIVE_DECLARE(BitmapConvertBPP___STATIC__VOID__SZARRAY_U1__SZARRAY_U1__GHIOSHWHardwareUtilBPPType);
    TINYCLR_NATIVE_DECLARE(PlayPCMAudio___STATIC__VOID__U1__SZARRAY_U1__I4__I4__I4);

    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_RTC__Cerb
{
    static const int FIELD_STATIC__SynchPrediv = 0;
    static const int FIELD_STATIC__AsynchPrediv = 1;
    static const int FIELD_STATIC__RCC_CFGR_RTCPRE = 2;
    static const int FIELD_STATIC__RTC_WeekDay = 3;
    static const int FIELD_STATIC__RTC_Date = 4;
    static const int FIELD_STATIC__RTC_Month = 5;
    static const int FIELD_STATIC__RTC_Year = 6;
    static const int FIELD_STATIC__RTC_Hours = 7;
    static const int FIELD_STATIC__RTC_Minutes = 8;
    static const int FIELD_STATIC__RTC_Seconds = 9;
    static const int FIELD_STATIC__RTC_H12 = 10;
    static const int FIELD_STATIC__RCC_APB1ENR = 11;
    static const int FIELD_STATIC__RCC_CSR = 12;
    static const int FIELD_STATIC__RCC_CFGR = 13;
    static const int FIELD_STATIC__RCC_CR = 14;
    static const int FIELD_STATIC__RCC_BDCR = 15;
    static const int FIELD_STATIC__RTC_WPR = 16;
    static const int FIELD_STATIC__RTC_ISR = 17;
    static const int FIELD_STATIC__RTC_CR_C = 18;
    static const int FIELD_STATIC__RTC_PRER = 19;
    static const int FIELD_STATIC__PWR_CR = 20;


    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_LCDController
{
    static const int FIELD_STATIC__HeadlessConfig = 21;

    TINYCLR_NATIVE_DECLARE(Set___STATIC__BOOLEAN__U4__U4__BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN__U1__U1__U1__U1__U1__U1__U4);

    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_LCDController__Configurations
{
    static const int FIELD__Width = 1;
    static const int FIELD__Height = 2;
    static const int FIELD__PriorityEnable = 3;
    static const int FIELD__OutputEnableIsFixed = 4;
    static const int FIELD__OutputEnablePolarity = 5;
    static const int FIELD__HorizontalSyncPolarity = 6;
    static const int FIELD__VerticalSyncPolarity = 7;
    static const int FIELD__PixelPolarity = 8;
    static const int FIELD__HorizontalSyncPulseWidth = 9;
    static const int FIELD__HorizontalBackPorch = 10;
    static const int FIELD__HorizontalFrontPorch = 11;
    static const int FIELD__VerticalSyncPulseWidth = 12;
    static const int FIELD__VerticalBackPorch = 13;
    static const int FIELD__VerticalFrontPorch = 14;
    static const int FIELD__PixelClockRateKHz = 15;


    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_LowLevel_AddressSpace
{
    TINYCLR_NATIVE_DECLARE(Write___STATIC__VOID__U4__SZARRAY_U1__I4__I4);
    TINYCLR_NATIVE_DECLARE(Read___STATIC__VOID__U4__SZARRAY_U1__I4__I4);

    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_LowLevel_Register
{
    static const int FIELD__address = 1;

    TINYCLR_NATIVE_DECLARE(_ctor___VOID__U4);
    TINYCLR_NATIVE_DECLARE(Write___VOID__U4);
    TINYCLR_NATIVE_DECLARE(Read___U4);
    TINYCLR_NATIVE_DECLARE(SetBits___VOID__U4);
    TINYCLR_NATIVE_DECLARE(ClearBits___VOID__U4);
    TINYCLR_NATIVE_DECLARE(ToggleBits___VOID__U4);

    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_SoftwareI2CBus
{
    static const int FIELD__disposed = 1;
    static const int FIELD__clockPin = 2;
    static const int FIELD__dataPin = 3;

    TINYCLR_NATIVE_DECLARE(NativeI2CWriteRead___STATIC__BOOLEAN__I4__I4__I4__U2__SZARRAY_U1__I4__I4__SZARRAY_U1__I4__I4__BYREF_I4__BYREF_I4);

    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_StorageDev
{
    TINYCLR_NATIVE_DECLARE(MountSDToSystem___STATIC__VOID);
    TINYCLR_NATIVE_DECLARE(MountSDToSystem___STATIC__VOID__U4);
    TINYCLR_NATIVE_DECLARE(UnmountSD___STATIC__VOID);

    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_RTC__Hydra__Hydra_PMC_Registers
{
    static const int FIELD_STATIC__PMC_MCKR = 22;
    static const int FIELD_STATIC__PMC_SR = 23;


    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_RTC__Hydra__Hydra_RTC_Registers
{
    static const int FIELD_STATIC__CR = 24;
    static const int FIELD_STATIC__TIMR = 25;
    static const int FIELD_STATIC__CALR = 26;
    static const int FIELD_STATIC__SR = 27;
    static const int FIELD_STATIC__SCCR = 28;
    static const int FIELD_STATIC__VER = 29;


    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_RTC__Hydra__Hydra_SCKCR_Registers
{
    static const int FIELD_STATIC__SCKCR = 30;
    static const int FIELD_STATIC__CKGR_PLLAR = 31;
    static const int FIELD_STATIC__CKGR_MOR = 32;


    //--//

};

struct Library_HAL_GHI_OSHW_Hardware_SoftwareI2CBus__I2CDevice
{
    static const int FIELD__bus = 1;
    static const int FIELD__address = 2;
    static const int FIELD__clockRateKHz = 3;


    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_GHI_OSHW_Hardware;

#endif  //_HAL_H_

//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#ifndef _HAL_GHI_OSHW_HARDWARE_SOFTWAREI2CBUS_H_
#define _HAL_GHI_OSHW_HARDWARE_SOFTWAREI2CBUS_H_

namespace GHI
{
    namespace OSHW
    {
        namespace Hardware
        {
            struct SoftwareI2CBus
            {
                // Helper Functions to access fields of managed object
                static INT8& Get_disposed( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_HAL_GHI_OSHW_Hardware_SoftwareI2CBus::FIELD__disposed ); }

                static UNSUPPORTED_TYPE& Get_clockPin( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_HAL_GHI_OSHW_Hardware_SoftwareI2CBus::FIELD__clockPin ); }

                static UNSUPPORTED_TYPE& Get_dataPin( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_HAL_GHI_OSHW_Hardware_SoftwareI2CBus::FIELD__dataPin ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 NativeI2CWriteRead( INT32 param0, INT32 param1, INT32 param2, UINT16 param3, CLR_RT_TypedArray_UINT8 param4, INT32 param5, INT32 param6, CLR_RT_TypedArray_UINT8 param7, INT32 param8, INT32 param9, INT32 * param10, INT32 * param11, HRESULT &hr );
            };
        }
    }
}
#endif  //_HAL_GHI_OSHW_HARDWARE_SOFTWAREI2CBUS_H_

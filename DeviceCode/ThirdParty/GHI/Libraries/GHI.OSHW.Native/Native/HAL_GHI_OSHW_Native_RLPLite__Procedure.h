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


#ifndef _HAL_GHI_OSHW_NATIVE_RLPLITE__PROCEDURE_H_
#define _HAL_GHI_OSHW_NATIVE_RLPLITE__PROCEDURE_H_

namespace GHI
{
    namespace OSHW
    {
        namespace Native
        {
            struct RLPLite_Procedure
            {
                // Helper Functions to access fields of managed object
                static UINT32& Get_address( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_HAL_GHI_OSHW_Native_RLPLite__Procedure::FIELD__address ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_float param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr );
                static INT32 Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr );
                static INT32 Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr );
                static INT32 Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr );
                static INT32 Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT8 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr );
            };
        }
    }
}
#endif  //_HAL_GHI_OSHW_NATIVE_RLPLITE__PROCEDURE_H_

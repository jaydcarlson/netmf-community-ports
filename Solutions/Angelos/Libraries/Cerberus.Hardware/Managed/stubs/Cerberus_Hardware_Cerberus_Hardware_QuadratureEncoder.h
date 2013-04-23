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


#ifndef _CERBERUS_HARDWARE_CERBERUS_HARDWARE_QUADRATUREENCODER_H_
#define _CERBERUS_HARDWARE_CERBERUS_HARDWARE_QUADRATUREENCODER_H_

namespace Cerberus
{
    namespace Hardware
    {
        struct QuadratureEncoder
        {
            // Helper Functions to access fields of managed object
            static UNSUPPORTED_TYPE& Get__dispatcher( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Cerberus_Hardware_Cerberus_Hardware_QuadratureEncoder::FIELD___dispatcher ); }

            static UNSUPPORTED_TYPE& Get__inputCapture( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Cerberus_Hardware_Cerberus_Hardware_QuadratureEncoder::FIELD___inputCapture ); }

            static INT32& Get_<Count>k__BackingField( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_Cerberus_Hardware_Cerberus_Hardware_QuadratureEncoder::FIELD__<Count>k__BackingField ); }

            static INT8& Get_<InputCaptureEnabled>k__BackingField( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_Cerberus_Hardware_Cerberus_Hardware_QuadratureEncoder::FIELD__<InputCaptureEnabled>k__BackingField ); }

            static INT8& Get_<CountEnabled>k__BackingField( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_Cerberus_Hardware_Cerberus_Hardware_QuadratureEncoder::FIELD__<CountEnabled>k__BackingField ); }

            // Declaration of stubs. These functions are implemented by Interop code developers
            static INT32 get_Count( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void set_Count( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
            static INT8 get_InputCaptureEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void set_InputCaptureEnabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr );
            static INT8 get_CountEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void set_CountEnabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr );
        };
    }
}
#endif  //_CERBERUS_HARDWARE_CERBERUS_HARDWARE_QUADRATUREENCODER_H_

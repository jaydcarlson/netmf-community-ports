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


#ifndef _HAL_GHI_OSHW_HARDWARE_LOWLEVEL_REGISTER_H_
#define _HAL_GHI_OSHW_HARDWARE_LOWLEVEL_REGISTER_H_

namespace GHI
{
    namespace OSHW
    {
        namespace Hardware
        {
            namespace LowLevel
            {
                struct Register
                {
                    // Helper Functions to access fields of managed object
                    static UINT32& Get_address( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_HAL_GHI_OSHW_Hardware_LowLevel_Register::FIELD__address ); }

                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static void _ctor( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                    static void Write( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                    static UINT32 Read( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static void SetBits( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                    static void ClearBits( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                    static void ToggleBits( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_HAL_GHI_OSHW_HARDWARE_LOWLEVEL_REGISTER_H_

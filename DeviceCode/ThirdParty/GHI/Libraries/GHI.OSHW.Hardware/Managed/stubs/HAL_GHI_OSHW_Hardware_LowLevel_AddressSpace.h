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


#ifndef _HAL_GHI_OSHW_HARDWARE_LOWLEVEL_ADDRESSSPACE_H_
#define _HAL_GHI_OSHW_HARDWARE_LOWLEVEL_ADDRESSSPACE_H_

namespace GHI
{
    namespace OSHW
    {
        namespace Hardware
        {
            namespace LowLevel
            {
                struct AddressSpace
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static void Write( UINT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, INT32 param3, HRESULT &hr );
                    static void Read( UINT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, INT32 param3, HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_HAL_GHI_OSHW_HARDWARE_LOWLEVEL_ADDRESSSPACE_H_

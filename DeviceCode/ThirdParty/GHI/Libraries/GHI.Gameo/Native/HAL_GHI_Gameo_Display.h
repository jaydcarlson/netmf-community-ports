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


#ifndef _HAL_GHI_GAMEO_DISPLAY_H_
#define _HAL_GHI_GAMEO_DISPLAY_H_

namespace GHI
{
    namespace Gameo
    {
        struct Display
        {
            // Helper Functions to access fields of managed object
            // Declaration of stubs. These functions are implemented by Interop code developers
            static INT8 Initialize( UINT8 param0, UINT16 param1, UINT16 param2, HRESULT &hr );
            static INT8 WriteRaw( CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr );
            static INT8 WriteRaw( CLR_RT_TypedArray_UINT16 param0, INT32 param1, INT32 param2, HRESULT &hr );
            static INT8 WriteRegister( UINT16 param0, UINT16 param1, HRESULT &hr );
            static INT8 WriteRegister( UINT16 param0, HRESULT &hr );
            static INT8 FillRect( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT16 param3, UINT16 param4, UINT16 param5, HRESULT &hr );
            static INT8 DrawImage( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT16 param3, UINT16 param4, UINT16 param5, UINT16 param6, UINT16 param7, UINT16 param8, HRESULT &hr );
        };
    }
}
#endif  //_HAL_GHI_GAMEO_DISPLAY_H_

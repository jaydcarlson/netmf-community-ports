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


#ifndef _ANGELOS_PACKETRADIO_ANGELOS_PACKETRADIO_H_
#define _ANGELOS_PACKETRADIO_ANGELOS_PACKETRADIO_H_

namespace Angelos
{
    struct PacketRadio
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static void Transmit( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
        static void GetPacket( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
        static void SetAddressFilteringMode( INT32 param0, HRESULT &hr );
        static void RadioInit( HRESULT &hr );
    };
}
#endif  //_ANGELOS_PACKETRADIO_ANGELOS_PACKETRADIO_H_

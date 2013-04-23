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


#include "Angelos_PacketRadio.h"
#include "Angelos_PacketRadio_Angelos_PacketRadio.h"
#include "Radio_PacketReceived.h"
#include "../../../Drivers/PacketRadio/PacketRadio.h"
using namespace Angelos;

void PacketRadio::Transmit( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
  PacketRadio_Send(param0.GetBuffer(), param0.GetSize());
}

void PacketRadio::GetPacket( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    int len = param0.GetSize();
    memcpy(param0.GetBuffer(), Radio_PacketReceived_RxPacket, len);
}

void PacketRadio::SetAddressFilteringMode( INT32 param0, HRESULT &hr )
{
}

void PacketRadio::RadioInit( HRESULT &hr )
{
PacketRadio_Initialize();
}


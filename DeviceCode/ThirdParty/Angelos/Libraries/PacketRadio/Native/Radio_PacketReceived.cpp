#include <TinyCLR_Interop.h>
#include "Radio_PacketReceived.h"
#include "../../../Drivers/PacketRadio/PacketRadio.h"
static bool g_TestInterruptEnalbed = false;
static CLR_RT_HeapBlock_NativeEventDispatcher *g_Context = NULL;
static UINT64 g_UserData = 0;
UINT8 Radio_PacketReceived_RxPacket[64];
UINT8 Radio_BufferLength = 0;
UINT8 callbackID;
static HRESULT InitializePacketReceivedDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_Context  = pContext;
   g_UserData = userData;

   return S_OK;
}


static HRESULT EnableDisablePacketReceivedDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
  callbackID = PacketRadio_RegisterOnReceivedDataCallback((PACKETRADIO_RECEIVED_DATA)&PacketRadio_PacketReceived );
   g_TestInterruptEnalbed = fEnable;
   return S_OK;
}


static HRESULT CleanupPacketReceivedDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    PacketRadio_UnregisterOnReceivedDataCallback(callbackID);
    g_Context = NULL;
    g_UserData = 0;
    CleanupNativeEventsFromHALQueue( pContext );
    return S_OK;
}

void PacketRadio_PacketReceived(UINT8* Data, UINT32 Length)
{
  if(g_TestInterruptEnalbed)
  {
    GLOBAL_LOCK(irq);
    memcpy(Radio_PacketReceived_RxPacket, Data, Length);
    Radio_BufferLength = Length;
    SaveNativeEventToHALQueue( g_Context, Length, 0 );
  }
}

static const CLR_RT_DriverInterruptMethods g_PacketReceivedDriverMethods =
{ 
    InitializePacketReceivedDriver,
    EnableDisablePacketReceivedDriver,
    CleanupPacketReceivedDriver
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Angelos_PacketReceived =
{
    "Angelos_PacketReceivedEvent",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_PacketReceivedDriverMethods
};

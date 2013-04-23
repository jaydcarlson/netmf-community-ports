#ifndef _PACKETHANDLER_PACKETRECEIVEDCALLBACK_H_
#define _PACKETHANDLER_PACKETRECEIVEDCALLBACK_H_

#include <TinyCLR_Interop.h>
#include <tinyhal.h>

void PacketRadio_PacketReceived(UINT8* Data, UINT32 Length);
extern UINT8 Radio_PacketReceived_RxPacket[64];
extern UINT8 Radio_BufferLength;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Angelos_PacketReceived;
#endif  //_PACKETHANDLER_PACKETRECEIVEDCALLBACK_H_

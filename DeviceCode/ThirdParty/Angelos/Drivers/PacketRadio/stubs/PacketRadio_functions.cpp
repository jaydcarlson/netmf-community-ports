////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "PacketRadio_decl.h"

#define PacketRadio_MAXPACKETSIZE 0

PACKETRADIO_RECEIVED_DATA PacketRadio_OnReceivedDataCallback[10];

int PacketRadio_RegisterOnReceivedDataCallback(PACKETRADIO_RECEIVED_DATA cb)
{

}

void PacketRadio_UnregisterOnReceivedDataCallback(int cbIdx)
{

}

void PacketRadio_OnDataReceived(UINT8* Data, size_t size)
{

}

BOOL PacketRadio_Initialize()
{
	return(1);
}

BOOL PacketRadio_Uninitialize()
{
    return TRUE;
}

int PacketRadio_GetMaxPacketSize()
{
	return 0;
}

int PacketRadio_Send(UINT8* Data, size_t size)
{
	return 0;
}

int PacketRadio_Read(UINT8* Data, size_t size )
{
    return 0;
}

BOOL PacketRadio_Flush()
{
    return TRUE;
}


int  PacketRadio_BytesInBuffer(BOOL fRx )
{
    return 0;
}

void PacketRadio_SetAddress(UINT32 address)
{

}




//--//



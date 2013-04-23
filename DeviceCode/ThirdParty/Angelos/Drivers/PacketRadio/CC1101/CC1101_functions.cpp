////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Portions copyright (c) Mobile Communications & Coding
// Portions copyright (c) 2007 Texas Instruments, Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "CC1101.h"
#include "../PacketRadio.h"

PACKETRADIO_RECEIVED_DATA PacketRadio_OnReceivedDataCallback[10];

int OnReceivedDataCallbackIdx = 0;

int PacketRadio_RegisterOnReceivedDataCallback(PACKETRADIO_RECEIVED_DATA cb)
{
  PacketRadio_OnReceivedDataCallback[OnReceivedDataCallbackIdx] = cb;
  return OnReceivedDataCallbackIdx++;
}

void PacketRadio_UnregisterOnReceivedDataCallback(int cbIdx)
{
  PacketRadio_OnReceivedDataCallback[cbIdx] = NULL;
}

void PacketRadio_OnDataReceived(UINT8* Data, size_t size)
{
  for(int i = 0; i<OnReceivedDataCallbackIdx; i++)
  {
    if(PacketRadio_OnReceivedDataCallback[i] != NULL)
    {
      PacketRadio_OnReceivedDataCallback[i](Data, size);
    }
  }
}

BOOL PacketRadio_Initialize()
{
	CC1101_Driver::Initialize();
	return(1);
}

BOOL PacketRadio_Uninitialize()
{
    return TRUE;
}

UINT32 PacketRadio_GetMaxPacketSize()
{
	return CC1101_Driver::GetMaxPacketSize();
}

/*
 * \brief Send an array of as a packet. Returns 1 on success or 0 on failure.
 *
 * This is a blocking function that only returns once the packet has been sent from the radio. Channel access is assumed to be implemented in the hardware
 */
int PacketRadio_Send(UINT8* Data, size_t size)
{
	CC1101_Driver::TransmitData(Data, size);
	return 0;
}

int PacketRadio_Read(UINT8* Data, size_t size )
{
    return 0;
}

void PacketRadio_SetAddress(UINT32 address)
{
	return CC1101_Driver::SetAddress(address);
}

BOOL PacketRadio_Flush()
{
    return TRUE;
}


int  PacketRadio_BytesInBuffer(BOOL fRx )
{
    return 0;
}

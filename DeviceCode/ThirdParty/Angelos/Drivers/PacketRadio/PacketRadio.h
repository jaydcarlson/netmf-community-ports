#ifndef _PacketRadio_DECL_H_
#define _PacketRadio_DECL_H_ 1

typedef void (* PACKETRADIO_RECEIVED_DATA)(UINT8* Data, size_t Length);
extern PACKETRADIO_RECEIVED_DATA PacketRadio_OnReceivedDataCallback[10];
int PacketRadio_RegisterOnReceivedDataCallback(PACKETRADIO_RECEIVED_DATA cb);
void PacketRadio_OnDataReceived(UINT8* Data, size_t size);
void PacketRadio_UnregisterOnReceivedDataCallback(int cbIdx);
BOOL PacketRadio_Initialize();
BOOL PacketRadio_Uninitialize();
UINT32 PacketRadio_GetMaxPacketSize();
int PacketRadio_Send(UINT8* Data, size_t size);
int PacketRadio_Read(UINT8* Data, size_t size );
BOOL PacketRadio_Flush();
int  PacketRadio_BytesInBuffer(BOOL fRx );
void PacketRadio_SetAddress(UINT32 address);
// PacketRadio_RECEIVED_DATA PacketRadio_OnReceivedDataCallback;
// void PacketRadio_Register_OnReceivedData(PacketRadio_RECEIVED_DATA cb);
#endif // _PacketRadio_DECL_H_

using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;
namespace Angelos
{
    public static class PacketRadio 
    {
       public delegate void DataReceivedEventHandler(object sender, byte[] Data);
       public static event DataReceivedEventHandler DataReceived;
       
       public enum AddressFilter { 
           /// <summary>
           /// Address filter disabled
           /// </summary>
           Disabled = 0, 
           /// <summary>
           /// Only accept packets starting with the radio's address
           /// </summary>
           MyAddress = 1, 
           /// <summary>
           /// Accept packets starting with 0xFF
           /// </summary>
           Broadcast = 2, 
           /// <summary>
           /// Accept packets starting with 0x00
           /// </summary>
           AddressZero = 4
       };
       private static NativeEventDispatcher m_evtDataEvent = null;

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void Transmit(byte[] bytes);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void GetPacket(byte[] packet);

        /// <summary>
        /// Sets the address filtering for the radio.
        /// </summary>
        /// <param name="mode"></param>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void SetAddressFilteringMode(AddressFilter mode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RadioInit();
          

        public static void EnableReceiveInterrupt()
        {
            m_evtDataEvent = new NativeEventDispatcher("Angelos_PacketReceivedEvent", 0);
            m_evtDataEvent.OnInterrupt += m_evtDataEvent_OnInterrupt;
        }

        static void m_evtDataEvent_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            byte[] packet = new byte[data1];
            GetPacket(packet);
            if (DataReceived != null)
            {
                DataReceived(null, packet);
            }
        }
    }
}

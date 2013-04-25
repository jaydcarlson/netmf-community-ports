using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;
namespace MC2.Hardware
{
    public static class SerialNumber 
    {
        public static byte[] Get()
        {
                byte[] serialNumber = new byte[GetCount()];
                Get(serialNumber);
                return serialNumber;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void Get(byte[] packet);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static UInt32 GetCount();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.CompilerServices;

namespace GHI.OSHW.Hardware.LowLevel
{
    /// <summary>
    /// Provides access to the address space.
    /// </summary>
    /// <remarks>
    /// </remarks>
    public static class AddressSpace
    {
        /// <summary>
        /// Writes a buffer to the target address space.
        /// </summary>
        /// <param name="address">Starting address.</param>
        /// <param name="buffer">Buffer.</param>
        /// <param name="offset">Offset.</param>
        /// <param name="count">Count.</param>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Write(uint address, byte[] buffer, int offset, int count);

        /// <summary>
        /// Reads the address space into a buffer.
        /// </summary>
        /// <param name="address">Starting address.</param>
        /// <param name="buffer">Buffer.</param>
        /// <param name="offset">Offset.</param>
        /// <param name="count">Count.</param>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Read(uint address, byte[] buffer, int offset, int count);
    }
}

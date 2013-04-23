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
    /// Provide register access.
    /// </summary>
    /// <remarks>
    /// </remarks>
    public class Register
    {
#pragma warning disable 0169, 0414
        readonly private uint address;
#pragma warning restore 0169, 0414

        /// <summary>
        /// Creates a new register.
        /// </summary>
        /// <param name="address">Address.</param>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Register(uint address);

        /// <summary>
        /// Writes a value to the register.
        /// </summary>
        /// <param name="value">Value to be written.</param>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void Write(uint value);

        /// <summary>
        /// Reads the register's value.
        /// </summary>
        /// <returns>Value of the register.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public uint Read();

        /// <summary>
        /// Sets certain bits in the register using a mask.
        /// </summary>
        /// <param name="mask">Register value mask.</param>
        /// <remarks>
        /// Bits of value 1 in mask are set in the register.<br/>
        /// Bits of value 0 in mask are not changed.<br/>
        /// </remarks>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void SetBits(uint mask);

        /// <summary>
        /// Clears certain bits in the register using a mask.
        /// </summary>
        /// <param name="mask">Register value mask.</param>
        /// <remarks>
        /// Bits of value 1 in mask are cleared in the register.<br/>
        /// Bits of value 0 in mask are not changed.<br/>
        /// </remarks>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void ClearBits(uint mask);

        /// <summary>
        /// Toggles certain bits in the register using a mask.
        /// </summary>
        /// <param name="mask">Register value mask.</param>
        /// <remarks>
        /// Bits of value 1 in mask are toggled in the register.<br/>
        /// Bits of value 0 in mask are not changed.<br/>
        /// </remarks>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void ToggleBits(uint mask);
    }
}

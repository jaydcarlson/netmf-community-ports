////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Threading;
using System.Runtime.CompilerServices;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// Allows SD Card blockstorage to be added in realtime after system is booted
    /// </summary>
    public static class StorageDev
    {
        static public void MountSD()
		{
            MountSDToSystem();
			Thread.Sleep(1000); // sleep one second
		}
		static public void MountSD(uint ClockSpeedinKHz)
		{
            MountSDToSystem(ClockSpeedinKHz);
			Thread.Sleep(1000); // sleep one second
		}
		/// <summary>
        /// Allows SD Card blockstorage to be added in realtime after system is booted
        /// </summary>
        /// <remarks>
        /// A failed mount will raise a CLR_E_FAIL exception<br/>
        /// </remarks>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static private void MountSDToSystem();

        /// <summary>
        /// Allows SD Card blockstorage to be added in realtime after system is booted, specifying clock frequency in KHz
        /// </summary>
        /// <param name="ClockSpeedinKHz">Specify the SD Card's clock frequency in KHz</param>
        /// <remarks>
        /// A failed mount will raise a CLR_E_FAIL exception<br/>
        /// </remarks>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static private void MountSDToSystem(uint ClockSpeedinKHz);

        /// <summary>
        /// Allows SD Card blockstorage to be removed in realtime without a system reboot
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void UnmountSD();
    }
}

using System;
using Microsoft.SPOT;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// An enum that lists GHI's OSHW boards.
    /// </summary>
    public enum DeviceID
    {
        /// <summary>
        /// Is used for any board in the Cerberus family, such as FEZ Cerberus, FEZ Cerbuino, and FEZ Cerb40.
        /// </summary>
        FEZ_CERB = 0,

        /// <summary>
        /// Is used for the FEZ Hydra
        /// </summary>
        FEZ_HYDRA = 1
    };

    /// <summary>
    /// A class that provide device specific info
    /// </summary>
    public static class DeviceInfo
    {
        /// <summary>
        /// Returns the DeviceID for this board. See the DeviceD enum for more info.
        /// </summary>
        /// <returns>The DeviceID for this board.</returns>
        public static DeviceID GetDeviceID()
        {
            uint sysclk = Microsoft.SPOT.Hardware.Cpu.SystemClock;

            if (168000000 == sysclk)
                return DeviceID.FEZ_CERB;
            else
                return DeviceID.FEZ_HYDRA;
        }
    }
}

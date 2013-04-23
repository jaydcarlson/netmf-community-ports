using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GHI.Gameo
{
    public static class PowerControl
    {
        
        // Main Power control
        public static void PowerOff()
        {
            Display.TurnBackLightOff();
            while (Key.IsKeyPowerMenuPressed) ;
            // Because any low to high signal is found on wakeup pin, GameO will be wake up.
            // So need to delay 200-300ms
            Thread.Sleep(300);
            Sound.Disable(); // put in standby mode
            Accelerometer.Disable(); // put in standby mode
            PowerState.Sleep(SleepLevel.Off, HardwareEvent.OEMReserved1);
        }
        
        
    }
}

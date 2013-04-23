using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
namespace GHI.Gameo
{
    public static class Sound
    {
        const Cpu.Pin AudioPowerControlPin = (Cpu.Pin)(0 * 16 + 10);//FEZCerberus.Pin.PA10; 

        static Boolean isSoundEnable = false;
        static OutputPort AudioPowerControl = new OutputPort(AudioPowerControlPin, isSoundEnable);
        // Sound Power control
        public static void Enable()
        {
            isSoundEnable = true;
            AudioPowerControl.Write(isSoundEnable);
        }
        public static void Disable()
        {
            isSoundEnable = false;
            AudioPowerControl.Write(isSoundEnable);
        }
        public static Boolean IsSoundEnable
        {
            get
            {
                return isSoundEnable;
            }
        }
    }
}

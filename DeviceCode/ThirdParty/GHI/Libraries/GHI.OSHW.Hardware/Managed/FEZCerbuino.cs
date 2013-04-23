////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// Exposes FEZ Cerbuino specific functionality.
    /// </summary>
    public static class FEZCerbuino
    {
        /// <summary>
        /// Provides Pin definitions for FEZ Cerbuino.
        /// </summary>
        public class Pin
        {
            /// <summary>
            /// Provides Pin definitions for FEZ Cerbuino's Arduino-styled I/O headers.
            /// </summary>
            public class Digital
            {
                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin A0 = FEZCerberus.Pin.PB1;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin A1 = FEZCerberus.Pin.PA5;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin A2 = FEZCerberus.Pin.PB0;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin A3 = FEZCerberus.Pin.PC3;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin A4 = FEZCerberus.Pin.PC1;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin A5 = FEZCerberus.Pin.PA4;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D0 = FEZCerberus.Pin.PB11;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D1 = FEZCerberus.Pin.PB10;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D2 = FEZCerberus.Pin.PB12;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D3 = FEZCerberus.Pin.PC14;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D4 = FEZCerberus.Pin.PC15;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D5 = FEZCerberus.Pin.PA8;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D6 = FEZCerberus.Pin.PA10;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D7 = FEZCerberus.Pin.PC4;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D8 = FEZCerberus.Pin.PB13;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D9 = FEZCerberus.Pin.PA9;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D10 = FEZCerberus.Pin.PA15;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D11 = FEZCerberus.Pin.PB5;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D12 = FEZCerberus.Pin.PB4;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin D13 = FEZCerberus.Pin.PB3;

                /// <summary>Digital I/O.</summary>
                public const Cpu.Pin LED1 = FEZCerberus.Pin.PB2;

            }

            /// <summary>
            /// Provides Pin definitions for FEZ Cerbuino's Gadgeteer sockets.
            /// </summary>
            public class Gadgeteer
            {
                /// <summary>
                /// Provides Pin definitions for FEZ Cerbuino's Socket 1.
                /// </summary>
                public class Socket1
                {
                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin3 = FEZCerberus.Pin.PA14;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin4 = FEZCerberus.Pin.PB10;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin5 = FEZCerberus.Pin.PB11;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin6 = FEZCerberus.Pin.PA13;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin7 = FEZCerberus.Pin.PB5;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin8 = FEZCerberus.Pin.PB4;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin9 = FEZCerberus.Pin.PB3;

                }

                /// <summary>
                /// Provides Pin definitions for FEZ Cerbuino's Socket 2.
                /// </summary>
                public class Socket2
                {
                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin3 = FEZCerberus.Pin.PA6;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin4 = FEZCerberus.Pin.PA2;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin5 = FEZCerberus.Pin.PA3;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin6 = FEZCerberus.Pin.PA1;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin7 = FEZCerberus.Pin.PA0;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin8 = FEZCerberus.Pin.PB7;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin9 = FEZCerberus.Pin.PB6;

                }

                /// <summary>
                /// Provides Pin definitions for FEZ Cerbuino's Socket 3.
                /// </summary>
                public class Socket3
                {
                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin3 = FEZCerberus.Pin.PC0;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin4 = FEZCerberus.Pin.PC1;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin5 = FEZCerberus.Pin.PA4;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin6 = FEZCerberus.Pin.PC5;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin7 = FEZCerberus.Pin.PB8;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin8 = FEZCerberus.Pin.PA7;

                    /// <summary>Digital I/O.</summary>
                    public const Cpu.Pin Pin9 = FEZCerberus.Pin.PB9;

                }

                
            }

            /// <summary>
            /// Provides Channel definitions for FEZ Cerbuino's PWM capable pins.
            /// </summary>
            public class PWM
            {
                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D5 = (Cpu.PWMChannel)3;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel A2 = (Cpu.PWMChannel)4;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel A0 = (Cpu.PWMChannel)5;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D11 = (Cpu.PWMChannel)6;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D12 = (Cpu.PWMChannel)7;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D13 = (Cpu.PWMChannel)8;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D0 = (Cpu.PWMChannel)9;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D1 = (Cpu.PWMChannel)10;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D6 = (Cpu.PWMChannel)11;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D9 = (Cpu.PWMChannel)12;

                /// <summary>PWM Output</summary>
                public const Cpu.PWMChannel D10 = (Cpu.PWMChannel)13;

            }

            /// <summary>
            /// Provides Channel definitions for FEZ Cerbuino's AnalogIn capable pins.
            /// </summary>
            public class AnalogIn
            {
                /// <summary>Analog Input</summary>
                public const Cpu.AnalogChannel A0 = (Cpu.AnalogChannel)10;

                /// <summary>Analog Input</summary>
                public const Cpu.AnalogChannel A1 = (Cpu.AnalogChannel)8;

                /// <summary>Analog Input</summary>
                public const Cpu.AnalogChannel A2 = (Cpu.AnalogChannel)9;

                /// <summary>Analog Input</summary>
                public const Cpu.AnalogChannel A3 = (Cpu.AnalogChannel)7;

                /// <summary>Analog Input</summary>
                public const Cpu.AnalogChannel A4 = (Cpu.AnalogChannel)4;

                /// <summary>Analog Input</summary>
                public const Cpu.AnalogChannel A5 = (Cpu.AnalogChannel)5;

            }

        }
    }
}

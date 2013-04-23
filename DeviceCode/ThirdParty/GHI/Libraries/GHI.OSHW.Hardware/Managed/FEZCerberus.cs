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
    /// Exposes FEZ Cerberus specific functionality.
    /// </summary>
    public static class FEZCerberus
    {
        /// <summary>
        /// Provides Pin definitions for FEZ Cerberus.
        /// </summary>
        public class Pin
        {
            /// <summary>A value indicating that no GPIO pin is specified.</summary>
            public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA0 = (Cpu.Pin)((0 * 16) + 0);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA1 = (Cpu.Pin)((0 * 16) + 1);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA2 = (Cpu.Pin)((0 * 16) + 2);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA3 = (Cpu.Pin)((0 * 16) + 3);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA4 = (Cpu.Pin)((0 * 16) + 4);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA5 = (Cpu.Pin)((0 * 16) + 5);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA6 = (Cpu.Pin)((0 * 16) + 6);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA7 = (Cpu.Pin)((0 * 16) + 7);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA8 = (Cpu.Pin)((0 * 16) + 8);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA9 = (Cpu.Pin)((0 * 16) + 9);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA10 = (Cpu.Pin)((0 * 16) + 10);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA11 = (Cpu.Pin)((0 * 16) + 11);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA12 = (Cpu.Pin)((0 * 16) + 12);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA13 = (Cpu.Pin)((0 * 16) + 13);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA14 = (Cpu.Pin)((0 * 16) + 14);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA15 = (Cpu.Pin)((0 * 16) + 15);


            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB0 = (Cpu.Pin)((1 * 16) + 0);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB1 = (Cpu.Pin)((1 * 16) + 1);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB2 = (Cpu.Pin)((1 * 16) + 2);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB3 = (Cpu.Pin)((1 * 16) + 3);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB4 = (Cpu.Pin)((1 * 16) + 4);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB5 = (Cpu.Pin)((1 * 16) + 5);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB6 = (Cpu.Pin)((1 * 16) + 6);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB7 = (Cpu.Pin)((1 * 16) + 7);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB8 = (Cpu.Pin)((1 * 16) + 8);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB9 = (Cpu.Pin)((1 * 16) + 9);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB10 = (Cpu.Pin)((1 * 16) + 10);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB11 = (Cpu.Pin)((1 * 16) + 11);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB12 = (Cpu.Pin)((1 * 16) + 12);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB13 = (Cpu.Pin)((1 * 16) + 13);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB14 = (Cpu.Pin)((1 * 16) + 14);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB15 = (Cpu.Pin)((1 * 16) + 15);



            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC0 = (Cpu.Pin)((2 * 16) + 0);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC1 = (Cpu.Pin)((2 * 16) + 1);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC2 = (Cpu.Pin)((2 * 16) + 2);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC3 = (Cpu.Pin)((2 * 16) + 3);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC4 = (Cpu.Pin)((2 * 16) + 4);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC5 = (Cpu.Pin)((2 * 16) + 5);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC6 = (Cpu.Pin)((2 * 16) + 6);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC7 = (Cpu.Pin)((2 * 16) + 7);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC8 = (Cpu.Pin)((2 * 16) + 8);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC9 = (Cpu.Pin)((2 * 16) + 9);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC10 = (Cpu.Pin)((2 * 16) + 10);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC11 = (Cpu.Pin)((2 * 16) + 11);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC12 = (Cpu.Pin)((2 * 16) + 12);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC13 = (Cpu.Pin)((2 * 16) + 13);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC14 = (Cpu.Pin)((2 * 16) + 14);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC15 = (Cpu.Pin)((2 * 16) + 15);



            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD0 = (Cpu.Pin)((3 * 16) + 0);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD1 = (Cpu.Pin)((3 * 16) + 1);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD2 = (Cpu.Pin)((3 * 16) + 2);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD3 = (Cpu.Pin)((3 * 16) + 3);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD4 = (Cpu.Pin)((3 * 16) + 4);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD5 = (Cpu.Pin)((3 * 16) + 5);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD6 = (Cpu.Pin)((3 * 16) + 6);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD7 = (Cpu.Pin)((3 * 16) + 7);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD8 = (Cpu.Pin)((3 * 16) + 8);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD9 = (Cpu.Pin)((3 * 16) + 9);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD10 = (Cpu.Pin)((3 * 16) + 10);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD11 = (Cpu.Pin)((3 * 16) + 11);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD12 = (Cpu.Pin)((3 * 16) + 12);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD13 = (Cpu.Pin)((3 * 16) + 13);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD14 = (Cpu.Pin)((3 * 16) + 14);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD15 = (Cpu.Pin)((3 * 16) + 15);

        }
    }
}

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
    /// Exposes FEZ Hydra specific functionality.
    /// </summary>
    public static class FEZHydra
    {
        /// <summary>
        /// Provides Pin definitions for FEZ Hydra.
        /// </summary>
        public class Pin
        {
            /// <summary>A value indicating that no GPIO pin is specified.</summary>
            public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA0 = (Cpu.Pin)(0);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA1 = (Cpu.Pin)(1);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA2 = (Cpu.Pin)(2);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA3 = (Cpu.Pin)(3);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA4 = (Cpu.Pin)(4);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA5 = (Cpu.Pin)(5);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA6 = (Cpu.Pin)(6);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA7 = (Cpu.Pin)(7);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA8 = (Cpu.Pin)(8);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA9 = (Cpu.Pin)(9);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA10 = (Cpu.Pin)(10);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA11 = (Cpu.Pin)(11);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA12 = (Cpu.Pin)(12);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA13 = (Cpu.Pin)(13);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA14 = (Cpu.Pin)(14);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA15 = (Cpu.Pin)(15);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA16 = (Cpu.Pin)(16);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA17 = (Cpu.Pin)(17);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA18 = (Cpu.Pin)(18);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA19 = (Cpu.Pin)(19);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA20 = (Cpu.Pin)(20);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA21 = (Cpu.Pin)(21);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA22 = (Cpu.Pin)(22);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA23 = (Cpu.Pin)(23);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA24 = (Cpu.Pin)(24);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA25 = (Cpu.Pin)(25);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA26 = (Cpu.Pin)(26);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA27 = (Cpu.Pin)(27);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA28 = (Cpu.Pin)(28);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA29 = (Cpu.Pin)(29);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA30 = (Cpu.Pin)(30);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PA31 = (Cpu.Pin)(31);


            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB0 = (Cpu.Pin)(0 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB1 = (Cpu.Pin)(1 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB2 = (Cpu.Pin)(2 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB3 = (Cpu.Pin)(3 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB4 = (Cpu.Pin)(4 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB5 = (Cpu.Pin)(5 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB6 = (Cpu.Pin)(6 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB7 = (Cpu.Pin)(7 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB8 = (Cpu.Pin)(8 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB9 = (Cpu.Pin)(9 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB10 = (Cpu.Pin)(10 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB11 = (Cpu.Pin)(11 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB12 = (Cpu.Pin)(12 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB13 = (Cpu.Pin)(13 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB14 = (Cpu.Pin)(14 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB15 = (Cpu.Pin)(15 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB16 = (Cpu.Pin)(16 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB17 = (Cpu.Pin)(17 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB18 = (Cpu.Pin)(18 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB19 = (Cpu.Pin)(19 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB20 = (Cpu.Pin)(20 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB21 = (Cpu.Pin)(21 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB22 = (Cpu.Pin)(22 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB23 = (Cpu.Pin)(23 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB24 = (Cpu.Pin)(24 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB25 = (Cpu.Pin)(25 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB26 = (Cpu.Pin)(26 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB27 = (Cpu.Pin)(27 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB28 = (Cpu.Pin)(28 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB29 = (Cpu.Pin)(29 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB30 = (Cpu.Pin)(30 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PB31 = (Cpu.Pin)(31 + 32);


            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC0 = (Cpu.Pin)(0 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC1 = (Cpu.Pin)(1 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC2 = (Cpu.Pin)(2 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC3 = (Cpu.Pin)(3 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC4 = (Cpu.Pin)(4 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC5 = (Cpu.Pin)(5 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC6 = (Cpu.Pin)(6 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC7 = (Cpu.Pin)(7 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC8 = (Cpu.Pin)(8 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC9 = (Cpu.Pin)(9 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC10 = (Cpu.Pin)(10 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC11 = (Cpu.Pin)(11 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC12 = (Cpu.Pin)(12 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC13 = (Cpu.Pin)(13 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC14 = (Cpu.Pin)(14 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC15 = (Cpu.Pin)(15 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC16 = (Cpu.Pin)(16 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC17 = (Cpu.Pin)(17 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC18 = (Cpu.Pin)(18 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC19 = (Cpu.Pin)(19 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC20 = (Cpu.Pin)(20 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC21 = (Cpu.Pin)(21 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC22 = (Cpu.Pin)(22 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC23 = (Cpu.Pin)(23 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC24 = (Cpu.Pin)(24 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC25 = (Cpu.Pin)(25 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC26 = (Cpu.Pin)(26 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC27 = (Cpu.Pin)(27 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC28 = (Cpu.Pin)(28 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC29 = (Cpu.Pin)(29 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC30 = (Cpu.Pin)(30 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PC31 = (Cpu.Pin)(31 + 32 + 32);


            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD0 = (Cpu.Pin)(0 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD1 = (Cpu.Pin)(1 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD2 = (Cpu.Pin)(2 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD3 = (Cpu.Pin)(3 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD4 = (Cpu.Pin)(4 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD5 = (Cpu.Pin)(5 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD6 = (Cpu.Pin)(6 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD7 = (Cpu.Pin)(7 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD8 = (Cpu.Pin)(8 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD9 = (Cpu.Pin)(9 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD10 = (Cpu.Pin)(10 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD11 = (Cpu.Pin)(11 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD12 = (Cpu.Pin)(12 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD13 = (Cpu.Pin)(13 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD14 = (Cpu.Pin)(14 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD15 = (Cpu.Pin)(15 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD16 = (Cpu.Pin)(16 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD17 = (Cpu.Pin)(17 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD18 = (Cpu.Pin)(18 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD19 = (Cpu.Pin)(19 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD20 = (Cpu.Pin)(20 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD21 = (Cpu.Pin)(21 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD22 = (Cpu.Pin)(22 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD23 = (Cpu.Pin)(23 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD24 = (Cpu.Pin)(24 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD25 = (Cpu.Pin)(25 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD26 = (Cpu.Pin)(26 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD27 = (Cpu.Pin)(27 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD28 = (Cpu.Pin)(28 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD29 = (Cpu.Pin)(29 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD30 = (Cpu.Pin)(30 + 32 + 32 + 32);

            /// <summary>Digital I/O.</summary>
            public const Cpu.Pin PD31 = (Cpu.Pin)(31 + 32 + 32 + 32);

        }
    }
}

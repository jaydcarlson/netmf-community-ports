using System;
using Microsoft.SPOT;
using System.Threading;
using GHI.OSHW.Hardware.LowLevel;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// Real Time Clock functionality
    /// </summary>
    public static class RTC
    {
        /// <summary>
        /// Initializes the Real Time Clock
        /// </summary>
        public static void Initialize()
        {
            if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_CERB)
                InitializeCerb();
            else if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_HYDRA)
                InitializeHydra();
        }

        private static void InitializeCerb()
        {
            Cerb.RTC_Config();
            Cerb.RTC_Init();
        }

        private static void InitializeHydra()
        {
            Hydra.InitializeCrystal_32768();
            Thread.Sleep(5000);
        }

        /// <summary>
        /// Returns the RTC time
        /// </summary>
        /// <returns>Time</returns>
        public static DateTime GetTime()
        {
            if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_CERB)
                return GetTimeCerb();
            else if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_HYDRA)
                return GetTimeHydra();
            else
                throw new NotImplementedException("Device not found");
        }

        /// <summary>
        /// Sets the RTC time.
        /// </summary>
        /// <param name="time">Time to set.</param>
        public static void SetTime(DateTime time)
        {
            if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_CERB)
                SetTimeCerb(time);
            else if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_HYDRA)
                SetTimeHydra(time);
            else 
                throw new NotImplementedException("Device not found");
        }

        private static DateTime GetTimeCerb()
        {
            Cerb.RTC_GetTime();
            Cerb.RTC_GetDate();

            return new DateTime((Cerb.RTC_Year + 1980), Cerb.RTC_Month, Cerb.RTC_Date, Cerb.RTC_Hours, Cerb.RTC_Minutes, Cerb.RTC_Seconds, Cerb.RTC_H12);
        }

        private static DateTime GetTimeHydra()
        {
            UInt32 year = 0;
            UInt32 day = 0;
            UInt32 month = 0;
            UInt32 hour = 0;
            UInt32 minute = 0;
            UInt32 second = 0;
            UInt32 milli = 0;

            Hydra.RealTimeClock_Get(ref year, ref month, ref day, ref hour, ref minute, ref second, ref milli);

            return new DateTime((int)year, (int)month, (int)day, (int)hour, (int)minute, (int)second, (int)milli);
        }

        private static void SetTimeCerb(DateTime time)
        {
            Cerb.RTC_SetDate(time.Year, (byte)time.Month, (byte)time.DayOfWeek, (byte)time.Day);
            Cerb.RTC_SetTime((byte)time.Hour, (byte)time.Minute, (byte)time.Second);
        }

        private static void SetTimeHydra(DateTime time)
        {
            Hydra.RealTimeClock_Set((uint)time.Year, (uint)time.Month, (uint)time.Day, (uint)time.DayOfWeek, (uint)time.Hour, (uint)time.Minute, (uint)time.Second, (uint)time.Millisecond);
        }

        #region Helper Functions
        private static void SafeToProceed(DeviceID id)
        {
            if (id == DeviceInfo.GetDeviceID())
                return;
            else
                throw new NotSupportedException();
        }
        #endregion

        private class Hydra
        {
            #region Hydra
            const UInt32 AT91_RTC_TIMR_AM = (0x0);
            const UInt32 AT91_RTC_TIMR_PM = (0x1);
            const int S_OK = 0x00;
            const int CLR_E_OUT_OF_RANGE = 0x01;
            const UInt32 AT91C_PMC_CSS = (0x3 << 0);

            // Real Time Clock Register
            const UInt32 AT91C_BASE_RTC = 0xFFFFFE00;//// Real TIme Clock (RTC) Base Address
            const UInt32 RTC_CR = (AT91C_BASE_RTC + 0x00);
            const UInt32 RTC_TIMR = (AT91C_BASE_RTC + 0x08);
            const UInt32 RTC_CALR = (AT91C_BASE_RTC + 0x0C);
            const UInt32 RTC_SR = (AT91C_BASE_RTC + 0x18);
            const UInt32 RTC_SCCR = (AT91C_BASE_RTC + 0x1C);
            const UInt32 RTC_VER = (AT91C_BASE_RTC + 0x2C);

            //Slow Clock Configuration Register
            const UInt32 AT91C_BASE_SCKCR = 0xFFFFFD50;//// Slow Clock Configuration Register (SCKCR) Base Address
            const UInt32 AT91C_BASE_CKGR = 0xFFFFFC1C;
            const UInt32 AT91C_PMC_CSS_SLOW_CLK = (0x0);// (PMC) Slow Clock is selected
            const UInt32 AT91C_PMC_PRES_CLK_64 = (0x6 << 2);// (PMC) Selected clock divided by 64
            const UInt32 AT91C_PMC_MDIV_2 = (0x1 << 8);// (PMC) The processor clock is twice as fast as the master clock
            const UInt32 AT91C_PMC_MCKRDY = (0x1 << 3);// (PMC) Master Clock Status/Enable/Disable/Mask
            const UInt32 AT91C_CKGR_MOSCEN = (0x1 << 0);// (PMC) Main Oscillator Enable
            const UInt32 TIMEOUT = 10000000;
            const UInt32 AT91C_CKGR_OSCOUNT = (0xFF << 8);// (PMC) Main Oscillator Start-up Time
            const UInt32 AT91C_PMC_LOCKA = (0x1 << 1);// (PMC) PLL A Status/Enable/Disable/Mask
            const UInt32 AT91C_SLCKSEL_OSCSEL = (0x1 << 3);// (SYS) OSC Selection

            //Power Manager Control Register
            const UInt32 AT91C_BASE_PMC = 0xFFFFFC00;//// Slow Clock Configuration Register (SCKCR) Base Address
            const UInt32 PMC = (AT91C_BASE_PMC + 0x0030);
            const UInt32 SR = (AT91C_BASE_PMC + 0x0068);

            public class Hydra_RTC_Registers
            {
                public static Register CR = new Register(RTC_CR);
                public static Register TIMR = new Register(RTC_TIMR);
                public static Register CALR = new Register(RTC_CALR);
                public static Register SR = new Register(RTC_SR);
                public static Register SCCR = new Register(RTC_SCCR);
                public static Register VER = new Register(RTC_VER);
            }
            public class Hydra_SCKCR_Registers
            {
                public static Register SCKCR = new Register(AT91C_BASE_SCKCR + 0x00);
                public static Register CKGR_PLLAR = new Register(AT91C_BASE_CKGR + 0x00000028);
                public static Register CKGR_MOR = new Register(AT91C_BASE_CKGR + 0x00000020);

            }
            public class Hydra_PMC_Registers
            {
                public static Register PMC_MCKR = new Register(PMC);
                public static Register PMC_SR = new Register(SR);
            }
            #endregion

            #region Hydra Functions
            static void BinaryCodedDecimalExtract(UInt32 ValueToConvert, ref UInt32 tens, ref UInt32 ones)
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                tens = ValueToConvert / 10;
                ones = ValueToConvert % 10;
            }

            static UInt32 BinaryCodedDecimalCombine(UInt32 tens, UInt32 ones)
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                UInt32 CombinedBinaryCodedDecimal = 0;
                CombinedBinaryCodedDecimal = tens * 10;
                CombinedBinaryCodedDecimal += ones;
                return CombinedBinaryCodedDecimal;
            }

            static Boolean RealTimeClock_IsTimeValid()
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                if (Hydra_RTC_Registers.VER.Read() > 0)
                    return false;
                return true;
            }

            /// <summary>
            /// Sets the Real Time Clock. Only used on Hydra.
            /// </summary>
            /// <param name="year">Year</param>
            /// <param name="month">Month</param>
            /// <param name="day">Day</param>
            /// <param name="dayOfWeek">Day of Week</param>
            /// <param name="hour">Hour</param>
            /// <param name="minute">Minute</param>
            /// <param name="second">Second</param>
            /// <param name="millisecond">Millisecond</param>
            /// <returns></returns>
            public static int RealTimeClock_Set(UInt32 year, UInt32 month, UInt32 day, UInt32 dayOfWeek, UInt32 hour, UInt32 minute, UInt32 second, UInt32 millisecond)
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                UInt32 CalenderRegister = 0;
                UInt32 TimeRegister = 0;
                UInt32 LowerHundredYears = 0;
                UInt32 Tens = 0;
                UInt32 Ones = 0;
                Hydra_RTC_Registers.CR.Write(0x2);
                while ((Hydra_RTC_Registers.SR.Read() & 0x1) == 0) ;
                while ((Hydra_RTC_Registers.SR.Read() & 0x4) == 0) ;
                if ((year < 1900) || (year > 2099) || (month < 1) || (month > 12) || (day < 1) || (day > 31))
                {
                    return CLR_E_OUT_OF_RANGE;
                }
                if (year < 2000)
                {
                    CalenderRegister |= ((0x1 << 4) | 0x9);
                    LowerHundredYears = year - 1900;
                }
                else
                {
                    CalenderRegister |= ((0x2 << 4) | 0x0);
                    LowerHundredYears = year - 2000;
                }
                // Add year
                BinaryCodedDecimalExtract(LowerHundredYears, ref Tens, ref Ones);
                CalenderRegister |= (UInt32)((Tens << 12) | (Ones << 8));
                // Add month
                BinaryCodedDecimalExtract(month, ref Tens, ref Ones);
                CalenderRegister |= (UInt32)((Tens << 20) | (Ones << 16));
                // Add dayOfWeek
                CalenderRegister |= (UInt32)((dayOfWeek << 21));
                // Add day
                BinaryCodedDecimalExtract(day, ref Tens, ref Ones);
                CalenderRegister |= (UInt32)((Tens << 28) | (Ones << 24));
                // Write Calender to register
                Hydra_RTC_Registers.CALR.Write(CalenderRegister);
                TimeRegister = 0;
                Hydra_RTC_Registers.CR.SetBits(1 << 0);
                while ((TimeRegister & 0x1) == 0)
                {
                    TimeRegister = Hydra_RTC_Registers.SR.Read();
                }
                while (TimeRegister != 0)
                {
                    TimeRegister = Hydra_RTC_Registers.TIMR.Read();
                    Hydra_RTC_Registers.TIMR.Write(0);
                }
                // Add hour
                BinaryCodedDecimalExtract(hour, ref Tens, ref Ones);
                TimeRegister = (UInt32)((AT91_RTC_TIMR_AM << 22) | (Tens << 20) | (Ones << 16));
                Hydra_RTC_Registers.TIMR.Write(TimeRegister);
                // Add minute
                BinaryCodedDecimalExtract(minute, ref Tens, ref Ones);
                TimeRegister = (UInt32)((Tens << 12) | (Ones << 8));
                Hydra_RTC_Registers.TIMR.SetBits(TimeRegister);
                // Add second
                BinaryCodedDecimalExtract(second, ref Tens, ref Ones);
                TimeRegister = (UInt32)((Tens << 4) | Ones);
                Hydra_RTC_Registers.TIMR.SetBits(TimeRegister);
                // Clear Status Register
                Hydra_RTC_Registers.SCCR.SetBits(1 << 2);
                Hydra_RTC_Registers.CR.ClearBits((1 << 0) | (1 << 1));
                return S_OK;

            }

            /// <summary>
            /// Gets the current time on the Real Time Clock. Only used on Hydra.
            /// </summary>
            /// <param name="year">Year</param>
            /// <param name="month">Month</param>
            /// <param name="day">Day</param>
            /// <param name="hour">Hour</param>
            /// <param name="minute">Minute</param>
            /// <param name="second">Second</param>
            /// <param name="millisecond">Millisecond</param>
            /// <returns></returns>
            public static int RealTimeClock_Get(ref UInt32 year, ref UInt32 month, ref  UInt32 day, ref UInt32 hour, ref UInt32 minute, ref UInt32 second, ref  UInt32 millisecond)
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                UInt32 CalenderRegister = 0;
                UInt32 TimeRegister = 0;
                UInt32 FullYear = 0;
                UInt32 HundredYear = 0;
                UInt32 Hour = 0;
                CalenderRegister = Hydra_RTC_Registers.CALR.Read();
                TimeRegister = Hydra_RTC_Registers.TIMR.Read();

                if (RealTimeClock_IsTimeValid() == false)
                {
                    year = 1977;
                    month = 1;
                    day = 1;
                    hour = 1;
                    minute = 1;
                    second = 1;
                    millisecond = 1;
                }
                else
                {
                    if ((CalenderRegister & 0x7F) == 0x19)
                        FullYear = 1900;
                    else if ((CalenderRegister & 0x7F) == 0x20)
                        FullYear = 2000;
                    HundredYear = BinaryCodedDecimalCombine((((CalenderRegister & (0xFF << 8)) >> 8) >> 4), (((CalenderRegister & (0xFF << 8)) >> 8) & 0xF));
                    year = (UInt32)(FullYear + HundredYear);
                    month = (UInt32)BinaryCodedDecimalCombine((((CalenderRegister & (0x1F << 16)) >> 16) >> 4), (((CalenderRegister & (0x1F << 16)) >> 16) & 0xF));
                    day = (UInt32)BinaryCodedDecimalCombine((((CalenderRegister & (0x3F << 24)) >> 24) >> 4), (((CalenderRegister & (0x3F << 24)) >> 24) & 0xF));
                    Hour = BinaryCodedDecimalCombine((((TimeRegister & (0x3F << 16)) >> 16) >> 4), (((TimeRegister & (0x3F << 16)) >> 16) & 0xF));

                    if (((TimeRegister & 0x400000) >> 22) == AT91_RTC_TIMR_PM)
                        hour = (UInt32)Hour + 12;
                    else
                        hour = (UInt32)Hour;

                    minute = (UInt32)BinaryCodedDecimalCombine((((TimeRegister & (0x7F << 8)) >> 8) >> 4), (((TimeRegister & (0x7F << 8)) >> 8) & 0xF));
                    second = (UInt32)BinaryCodedDecimalCombine(((TimeRegister & 0x7F) >> 4), ((TimeRegister & 0x7F) & 0xF));
                    millisecond = 1;
                }
                return S_OK;
            }

            /// <summary>
            /// Set clock mode. Only used on Hydra.
            /// </summary>
            public static void SLCK_SetSlowClockMode()
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                UInt32 oldPll;
                UInt32 oldMck;
                UInt32 timeout = 0;
                UInt32 timeInSlowClockMode = 10;
                oldPll = Hydra_SCKCR_Registers.CKGR_PLLAR.Read();
                oldMck = Hydra_PMC_Registers.PMC_MCKR.Read();
                timeout = 0;

                while (0 == (Hydra_PMC_Registers.PMC_SR.Read() & AT91C_PMC_MCKRDY) && timeout++ < TIMEOUT) ;
                // Stop PLL A
                // MULA: PLL A Multiplier 0 = The PLL A is deactivated.
                Hydra_SCKCR_Registers.CKGR_PLLAR.Write(0x00003f00);
                // Stop Main Oscillator
                Hydra_SCKCR_Registers.CKGR_MOR.Write(Hydra_SCKCR_Registers.CKGR_MOR.Read() & (~AT91C_CKGR_MOSCEN));
                // Wait a while. The clock is at 500Hz...
                while ((timeInSlowClockMode--) > 0) ;
                // End !  
                Hydra_SCKCR_Registers.CKGR_MOR.Write(Hydra_SCKCR_Registers.CKGR_MOR.Read() | (AT91C_CKGR_OSCOUNT & (0x32 << 8)));
                Hydra_SCKCR_Registers.CKGR_MOR.Write(Hydra_SCKCR_Registers.CKGR_MOR.Read() | AT91C_CKGR_MOSCEN);

                Hydra_SCKCR_Registers.CKGR_PLLAR.Write(oldPll);
                timeout = 0;
                while (0 == (Hydra_PMC_Registers.PMC_SR.Read() & AT91C_PMC_LOCKA) && timeout++ < TIMEOUT) ;
                Hydra_PMC_Registers.PMC_MCKR.Write(oldMck);
                timeout = 0;
                while (0 == (Hydra_PMC_Registers.PMC_SR.Read() & AT91C_PMC_MCKRDY) && timeout++ < TIMEOUT) ;
            }

            /// <summary>
            /// Only used on Hydra.
            /// </summary>
            /// <returns></returns>
            public static Boolean SLCK_Is32k()
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                return ((Hydra_SCKCR_Registers.SCKCR.Read() & AT91C_SLCKSEL_OSCSEL) != 0);
            }

            /// <summary>
            /// Only used on Hydra.
            /// </summary>
            /// <returns></returns>
            public static Boolean InitializeCrystal_32768()
            {
                SafeToProceed(DeviceID.FEZ_HYDRA);

                UInt32 tmp;
                tmp = Hydra_PMC_Registers.PMC_MCKR.Read();

                if ((tmp & AT91C_PMC_CSS) == 0)
                {
                    Debug.Print("The master clock use the slow clock. ");
                    return true;
                }

                if (SLCK_Is32k())
                {
                    Debug.Print("The slow clock is already the external 32.768kHz crystal. ");
                    return true;
                }
                tmp = Hydra_SCKCR_Registers.SCKCR.Read();
                //Page 270
                //Enable the 32,768 Hz oscillator by setting the bit OSC32EN to 1.
                Hydra_SCKCR_Registers.SCKCR.SetBits(1 << 1);
                //Wait 32,768 Hz Startup Time for clock stabilization (software loop).
                Thread.Sleep(100);
                //Switch from internal RC to 32,768 Hz oscillator by setting the bit OSCSEL to 1.
                Hydra_SCKCR_Registers.SCKCR.SetBits(1 << 3);
                //Wait 5 slow clock cycles for internal resynchronization.
                Thread.Sleep(100);
                //Disable the RC oscillator by setting the bit RCEN to 0.
                Hydra_SCKCR_Registers.SCKCR.ClearBits(1 << 0);

                tmp = Hydra_SCKCR_Registers.SCKCR.Read();
                tmp = Hydra_PMC_Registers.PMC_SR.Read();
                SLCK_SetSlowClockMode();
                return true;
            }
            #endregion
        }

        private class Cerb
        {
            #region Cerberus
            // RCC
            const UInt32 RCC_BASE = 0x40023800;
            //Periph
            const UInt32 PERIPH_BASE = 0x40000000;
            const UInt32 APB1PERIPH_BASE = PERIPH_BASE;
            /*!< Peripheral base address in the bit-band region */
            const UInt32 PERIPH_BB_BASE = 0x42000000;
            const UInt32 PWR_BASE = (APB1PERIPH_BASE + 0x7000);
            const UInt32 PWR_OFFSET = (PWR_BASE - PERIPH_BASE);
            /* Alias word address of DBP bit */
            const UInt32 CR_OFFSET = (PWR_OFFSET + 0x00);
            const UInt32 DBP_BitNumber = 0x08;
            const UInt32 CR_DBP_BB = (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4));
            /* CR register byte 3 (Bits[23:16]) base address */
            const UInt32 CR_BYTE3_ADDRESS = 0x40023802;
            const UInt32 RCC_FLAG_HSERDY = 0x31;
            /* RCC Flag Mask */
            const UInt32 FLAG_MASK = 0x1F;
            const UInt32 RTC_BASE = PERIPH_BASE + 0x2800;
            const UInt32 RTC_RSF_MASK = (0xFFFFFF5F);
            //const UInt32 RTC_ISR_OFFSET = 0x0C;
            const UInt32 RTC_ISR_RSF = (0x00000020);
            const UInt32 RCC_RTCCLKSource_HSE_Div12 = (0x000C0300);
            const UInt32 RCC_OFFSET = (RCC_BASE - PERIPH_BASE);
            const UInt32 BDCR_OFFSET = (RCC_OFFSET + 0x70);
            const UInt32 RTCEN_BitNumber = 0x0F;
            const UInt32 BDCR_RTCEN_BB = (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4));
            const UInt32 RCC_HSE_Bypass = 0x05;
            const UInt32 BDCR_ADDRESS = (PERIPH_BASE + BDCR_OFFSET);
            const UInt32 RCC_LSE_OFF = ((UInt32)0x00);
            const UInt32 RCC_LSE_ON = ((UInt32)0x01);
            const UInt32 RCC_LSE_Bypass = ((UInt32)0x04);
            const byte RCC_FLAG_LSERDY = ((byte)0x41);
            const UInt32 RCC_RTCCLKSource_LSE = ((UInt32)0x00000100);
            const UInt32 RTC_ISR_INITF = (0x00000040);
            const UInt32 RTC_INIT_MASK = (0xFFFFFFFF);
            const int INITMODE_TIMEOUT = (0x00010000);
            const UInt32 RTC_DR_RESERVED_MASK = ((UInt32)0x00FFFF3F);
            // Time out
            /*!< Time out for HSE start up */
            const int HSE_STARTUP_TIMEOUT = 0x0600;
            const UInt32 SYNCHRO_TIMEOUT = (0x00008000);
            // Define error code
            const int RESET = 0;
            const int SET = 1;
            const int DISABLE = 0;
            const int ENABLE = 1;
            const int ERROR = 0;
            const int SUCCESS = 1;
            // Date and Time
            const UInt32 RTC_TR_RESERVED_MASK = (0x007F7F7F);
            const UInt32 RTC_TR_PM = (0x00400000);
            const UInt32 RTC_TR_HT = (0x00300000);
            const UInt32 RTC_TR_HT_0 = (0x00100000);
            const UInt32 RTC_TR_HT_1 = (0x00200000);
            const UInt32 RTC_TR_HU = (0x000F0000);
            const UInt32 RTC_TR_HU_0 = (0x00010000);
            const UInt32 RTC_TR_HU_1 = (0x00020000);
            const UInt32 RTC_TR_HU_2 = (0x00040000);
            const UInt32 RTC_TR_HU_3 = (0x00080000);
            const UInt32 RTC_TR_MNT = (0x00007000);
            const UInt32 RTC_TR_MNT_0 = (0x00001000);
            const UInt32 RTC_TR_MNT_1 = (0x00002000);
            const UInt32 RTC_TR_MNT_2 = (0x00004000);
            const UInt32 RTC_TR_MNU = (0x00000F00);
            const UInt32 RTC_TR_MNU_0 = (0x00000100);
            const UInt32 RTC_TR_MNU_1 = (0x00000200);
            const UInt32 RTC_TR_MNU_2 = (0x00000400);
            const UInt32 RTC_TR_MNU_3 = (0x00000800);
            const UInt32 RTC_TR_ST = (0x00000070);
            const UInt32 RTC_TR_ST_0 = (0x00000010);
            const UInt32 RTC_TR_ST_1 = (0x00000020);
            const UInt32 RTC_TR_ST_2 = (0x00000040);
            const UInt32 RTC_TR_SU = (0x0000000F);
            const UInt32 RTC_TR_SU_0 = (0x00000001);
            const UInt32 RTC_TR_SU_1 = (0x00000002);
            const UInt32 RTC_TR_SU_2 = (0x00000004);
            const UInt32 RTC_TR_SU_3 = (0x00000008);

            /********************  Bits definition for RTC_DR register  *******************/
            const UInt32 RTC_DR_YT = (0x00F00000);
            const UInt32 RTC_DR_YT_0 = (0x00100000);
            const UInt32 RTC_DR_YT_1 = (0x00200000);
            const UInt32 RTC_DR_YT_2 = (0x00400000);
            const UInt32 RTC_DR_YT_3 = (0x00800000);
            const UInt32 RTC_DR_YU = (0x000F0000);
            const UInt32 RTC_DR_YU_0 = (0x00010000);
            const UInt32 RTC_DR_YU_1 = (0x00020000);
            const UInt32 RTC_DR_YU_2 = (0x00040000);
            const UInt32 RTC_DR_YU_3 = (0x00080000);
            const UInt32 RTC_DR_WDU = (0x0000E000);
            const UInt32 RTC_DR_WDU_0 = (0x00002000);
            const UInt32 RTC_DR_WDU_1 = (0x00004000);
            const UInt32 RTC_DR_WDU_2 = (0x00008000);
            const UInt32 RTC_DR_MT = (0x00001000);
            const UInt32 RTC_DR_MU = (0x00000F00);
            const UInt32 RTC_DR_MU_0 = (0x00000100);
            const UInt32 RTC_DR_MU_1 = (0x00000200);
            const UInt32 RTC_DR_MU_2 = (0x00000400);
            const UInt32 RTC_DR_MU_3 = (0x00000800);
            const UInt32 RTC_DR_DT = (0x00000030);
            const UInt32 RTC_DR_DT_0 = (0x00000010);
            const UInt32 RTC_DR_DT_1 = (0x00000020);
            const UInt32 RTC_DR_DU = (0x0000000F);
            const UInt32 RTC_DR_DU_0 = (0x00000001);
            const UInt32 RTC_DR_DU_1 = (0x00000002);
            const UInt32 RTC_DR_DU_2 = (0x00000004);
            const UInt32 RTC_DR_DU_3 = (0x00000008);


            static UInt32 SynchPrediv = 0x1F3F;
            static UInt32 AsynchPrediv = 0x7C;
            static UInt32 RCC_CFGR_RTCPRE = (0x001F0000);
            public static byte RTC_WeekDay;
            public static byte RTC_Date;
            public static byte RTC_Month;
            public static byte RTC_Year;
            public static byte RTC_Hours;
            public static byte RTC_Minutes;
            public static byte RTC_Seconds;
            public static byte RTC_H12;

            // RCC Register
            static Register RCC_APB1ENR = new Register(RCC_BASE + 0x40);
            static Register RCC_CSR = new Register(RCC_BASE + 0x74);
            static Register RCC_CFGR = new Register(RCC_BASE + 0x8);
            static Register RCC_CR = new Register(RCC_BASE + CR_OFFSET);
            static Register RCC_BDCR = new Register(RCC_BASE + 0x70);
            // RTC Register
            static Register RTC_WPR = new Register(RTC_BASE + 0x24);
            static Register RTC_ISR = new Register(RTC_BASE + 0x0C);
            static Register RTC_CR_C = new Register(RTC_BASE + 0x8);
            static Register RTC_PRER = new Register(RTC_BASE + 0x10);
            //PWR Register
            static Register PWR_CR = new Register(PWR_BASE + 0x00);
            #endregion

            #region Cerberus Functions
            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="RCC_FLAG"></param>
            /// <returns></returns>
            public static int RCC_GetFlagStatus(UInt32 RCC_FLAG)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                /* Get the RCC register index */
                UInt32 tmp = (RCC_FLAG >> 5);
                UInt32 statusreg;
                int bitstatus = RESET;
                if (tmp == 1)               /* The flag to check is in CR register */
                {
                    statusreg = RCC_CR.Read();// RCC_BASE + CR_OFFSET;
                }
                else if (tmp == 2)          /* The flag to check is in BDCR register */
                {
                    statusreg = RCC_BDCR.Read(); // RCC_BASE + RCC_BDCR_OFFSET;
                }
                else                       /* The flag to check is in CSR register */
                {
                    statusreg = RCC_CSR.Read(); // RCC_BASE + RCC_CSR_OFFSET; ;
                }
                tmp = RCC_FLAG & FLAG_MASK;

                if ((statusreg & ((UInt32)(1 << (int)tmp))) != RESET)
                {
                    bitstatus = SET;
                }
                else
                {
                    bitstatus = RESET;
                }
                return bitstatus;
            }

            /// <summary>
            /// Waits for synchronization. Only used on Cerb based boards.
            /// </summary>
            /// <returns></returns>
            public static int RTC_WaitForSynchro()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                int synchrocounter = 0;
                int status = ERROR;
                UInt32 synchrostatus = 0;
                /* Disable the write protection for RTC registers */
                RTC_WPR.Write(0xCA);
                RTC_WPR.Write(0x53);
                RTC_ISR.ClearBits(1 << 5);
                do
                {
                    synchrostatus = (RTC_ISR.Read() & RTC_ISR_RSF);
                    synchrocounter++;
                } while ((synchrocounter != SYNCHRO_TIMEOUT) && (synchrostatus == 0x00));

                if ((RTC_ISR.Read() & RTC_ISR_RSF) != RESET)
                {
                    status = SUCCESS;
                }
                else
                {
                    status = ERROR;
                }
                RTC_WPR.Write(0xFF);
                return status;
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="RCC_RTCCLKSource"></param>
            public static void RCC_RTCCLKConfig(UInt32 RCC_RTCCLKSource)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                UInt32 tmpreg = 0;
                if ((RCC_RTCCLKSource & 0x00000300) == 0x00000300)
                { /* If HSE is selected as RTC clock source, configure HSE division factor for RTC clock */
                    tmpreg = RCC_CFGR.Read();//RCC_BASE + RCC_CFGR_OFFSET;
                    /* Clear RTCPRE[4:0] bits */
                    tmpreg &= ~RCC_CFGR_RTCPRE;
                    /* Configure HSE division factor for RTC clock */
                    tmpreg |= (RCC_RTCCLKSource & 0xFFFFCFF);
                    /* Store the new value */
                    RCC_CFGR.Write(tmpreg);
                }
                /* Select the RTC clock source */
                RCC_BDCR.SetBits(RCC_RTCCLKSource & 0x00000FFF);

            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="NewState"></param>
            public static void RCC_RTCCLKCmdEnable(UInt32 NewState)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                //Bit 15 RTCEN: RTC clock enable
                new Register(BDCR_RTCEN_BB).Write(NewState);
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="NewState"></param>
            public static void PWR_BackupAccessCmd(UInt32 NewState)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                if (NewState == ENABLE)
                {
                    PWR_CR.SetBits(1 << 8);
                }
                else
                {
                    PWR_CR.ClearBits(1 << 8);
                }
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            public static void RCC_ResetConfig()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                //Bit 16 BDRST: Backup domain software reset
                RCC_BDCR.SetBits(0x10000);
                Thread.Sleep(10);
                RCC_BDCR.ClearBits(0x10000);
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="RCC_LSE"></param>
            public static void RCC_LSEConfig(UInt32 RCC_LSE)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                UInt32 tmp;
                tmp = RCC_BDCR.Read();
                /* Check the parameters */
                //  Bit 0 LSEON: External low-speed oscillator enable
                RCC_BDCR.ClearBits(1 << 0);
                /* Reset LSEBYP bit */
                //  Bit 2 LSEBYP: External low-speed oscillator bypass
                //*(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
                RCC_BDCR.ClearBits(1 << 0);
                tmp = RCC_BDCR.Read();
                /* Configure LSE (RCC_LSE_OFF is already covered by the code section above) */
                switch (RCC_LSE)
                {
                    case RCC_LSE_ON:
                        /* Set LSEON bit */

                        RCC_BDCR.SetBits(1 << 0);
                        break;
                    case RCC_LSE_Bypass:
                        /* Set LSEBYP and LSEON bits */
                        RCC_BDCR.SetBits(1 << 0);
                        RCC_BDCR.SetBits(1 << 2);
                        break;
                    default:
                        break;
                }
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            public static void RTC_Config()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                //Bit 28 PWRRST: Power interface reset
                /* Enable the PWR clock */
                RCC_APB1ENR.SetBits(1 << 28);
                /* Allow access to RTC */
                PWR_BackupAccessCmd(ENABLE);
                RCC_ResetConfig();
                RCC_LSEConfig(RCC_LSE_ON);
                while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) ;
                RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
                SynchPrediv = 0xFF;
                AsynchPrediv = 0x7F;
                /* Enable the RTC Clock */
                RCC_RTCCLKCmdEnable(ENABLE);
                RTC_WaitForSynchro();
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <returns></returns>
            public static int RTC_EnterInitMode()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                int initcounter = 0x00;
                int status = ERROR;
                UInt32 initstatus = 0x00;
                /* Check if the Initialization mode is set */
                if ((RTC_ISR.Read() & RTC_ISR_INITF) == (UInt32)RESET)
                {
                    /* Set the Initialization mode */
                    RTC_ISR.Write((UInt32)RTC_INIT_MASK);

                    /* Wait till RTC is in INIT state and if Time out is reached exit */
                    do
                    {
                        initstatus = (RTC_ISR.Read() & RTC_ISR_INITF);
                        initcounter++;
                    } while ((initcounter != INITMODE_TIMEOUT) && (initstatus == 0x00));

                    if ((RTC_ISR.Read() & RTC_ISR_INITF) != RESET)
                    {
                        status = SUCCESS;
                    }
                    else
                    {
                        status = ERROR;
                    }
                }
                else
                {
                    status = SUCCESS;
                }

                return (status);
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            public static void RTC_ExitInitMode()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                /* Exit Initialization mode */
                //Bit 7 INIT: Initialization mode
                RTC_ISR.ClearBits(1 << 7);
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <returns></returns>
            public static int RTC_Init()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                int status = ERROR;
                /* Disable the write protection for RTC registers */
                RTC_WPR.Write(0xCA);
                RTC_WPR.Write(0x53);
                if (RTC_EnterInitMode() == ERROR)
                {
                    status = ERROR;
                }
                else
                {
                    /* Clear RTC CR FMT Bit */
                    RTC_CR_C.ClearBits(1 << 6);
                    /* Set RTC_CR register */
                    // Set HourFormat
                    RTC_CR_C.SetBits(0x00000000);
                    /* Configure the RTC PRER */
                    RTC_PRER.Write(SynchPrediv);
                    RTC_PRER.SetBits((AsynchPrediv << 16));
                    /* Exit Initialization mode */
                    RTC_ExitInitMode();
                    status = SUCCESS;
                }
                RTC_WPR.Write(0xFF);
                return status;
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="Value"></param>
            /// <returns></returns>
            public static byte RTC_Bcd2ToByte(byte Value)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                byte tmp = 0;
                tmp = (byte)(((byte)(Value & (byte)0xF0) >> (byte)0x4) * 10);
                return (byte)(tmp + (Value & (byte)0x0F));
            }

            /// <summary>
            /// . Only used on Cerb based boards.
            /// </summary>
            /// <param name="Value"></param>
            /// <returns></returns>
            public static byte RTC_ByteToBcd2(byte Value)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                byte bcdhigh = 0;

                while (Value >= 10)
                {
                    bcdhigh++;
                    Value -= 10;
                }
                return (byte)((byte)(bcdhigh << 4) | Value);
            }

            /// <summary>
            /// Gets the time. Only used on Cerb based boards.
            /// </summary>
            public static void RTC_GetTime()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                Register RTC_TR = new Register(RTC_BASE + 0x00);
                UInt32 tmpreg;
                tmpreg = (UInt32)(RTC_TR.Read() & RTC_TR_RESERVED_MASK);
                RTC_Hours = (byte)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16);
                RTC_Minutes = (byte)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8);
                RTC_Seconds = (byte)(tmpreg & (RTC_TR_ST | RTC_TR_SU));
                RTC_H12 = (byte)((tmpreg & (RTC_TR_PM)) >> 22);
                /* Convert the structure parameters to Binary format */
                RTC_Hours = (byte)RTC_Bcd2ToByte(RTC_Hours);
                RTC_Minutes = (byte)RTC_Bcd2ToByte(RTC_Minutes);
                RTC_Seconds = (byte)RTC_Bcd2ToByte(RTC_Seconds);


            }

            /// <summary>
            ///  Gets the date. Only used on Cerb based boards.
            /// </summary>
            public static void RTC_GetDate()
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                Register RTC_DR = new Register(RTC_BASE + 0x04);
                UInt32 tmpreg = (UInt32)(RTC_DR.Read() & RTC_DR_RESERVED_MASK);
                /* Fill the structure fields with the read parameters */
                RTC_Year = (byte)((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16);
                RTC_Month = (byte)((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8);
                RTC_Date = (byte)(tmpreg & (RTC_DR_DT | RTC_DR_DU));
                RTC_WeekDay = (byte)((tmpreg & (RTC_DR_WDU)) >> 13);
                /* Convert the structure parameters to Binary format */
                RTC_Year = (byte)RTC_Bcd2ToByte(RTC_Year);
                RTC_Month = (byte)RTC_Bcd2ToByte(RTC_Month);
                RTC_Date = (byte)RTC_Bcd2ToByte(RTC_Date);
                //RTC_WeekDay = (byte)(RTC_DateStruct->RTC_WeekDay);

            }

            /// <summary>
            /// Sets the time. Only used on Cerb based boards.
            /// </summary>
            /// <param name="year">Year</param>
            /// <param name="month">Month</param>
            /// <param name="dayofweek">Day of Week</param>
            /// <param name="date">Date</param>
            /// <returns></returns>
            public static int RTC_SetDate(int year, byte month, byte dayofweek, byte date)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                int status = ERROR;
                UInt32 tmp = 0x10;
                UInt32 tmpreg = 0x00;
                RTC_Year = (byte)(year - 1980);
                RTC_Month = month;
                RTC_WeekDay = dayofweek;
                RTC_Date = date;
                if ((RTC_Month & 0x10) == 0x10)
                {
                    RTC_Month = (byte)((RTC_Month & (UInt32)~(tmp)) + 0x0A);
                }

                tmpreg = (((UInt32)RTC_ByteToBcd2(RTC_Year) << 16) |
                  ((UInt32)RTC_ByteToBcd2(RTC_Month) << 8) |
                  ((UInt32)RTC_ByteToBcd2(RTC_Date)) |
                  ((UInt32)RTC_WeekDay << 13));
                RTC_WPR.Write(0xCA);
                RTC_WPR.Write(0x53);
                if (RTC_EnterInitMode() == ERROR)
                {
                    status = ERROR;
                }
                else
                {
                    /* Set the RTC_DR register */
                    Register RTC_DR = new Register(RTC_BASE + 0x04);
                    RTC_DR.Write(tmpreg & RTC_DR_RESERVED_MASK);
                    /* Exit Initialization mode */
                    RTC_ExitInitMode();
                    if (RTC_WaitForSynchro() == ERROR)
                    {
                        status = ERROR;
                    }
                    else
                    {
                        status = SUCCESS;
                    }
                }
                /* Enable the write protection for RTC registers */
                RTC_WPR.Write(0xFF);
                return status;
            }

            /// <summary>
            /// Sets the time. Only used on Cerb based boards.
            /// </summary>
            /// <param name="hours">Hours</param>
            /// <param name="minutes">Minutes</param>
            /// <param name="seconds">Seconds</param>
            /// <returns></returns>
            public static int RTC_SetTime(byte hours, byte minutes, byte seconds)
            {
                SafeToProceed(DeviceID.FEZ_CERB);

                UInt32 tmpreg;
                RTC_H12 = 0;
                RTC_Hours = hours;
                RTC_Minutes = minutes;
                RTC_Seconds = seconds;
                tmpreg = (UInt32)(((UInt32)RTC_ByteToBcd2(RTC_Hours) << 16) |
                       ((UInt32)RTC_ByteToBcd2(RTC_Minutes) << 8) |
                       ((UInt32)RTC_ByteToBcd2(RTC_Seconds)) |
                       (((UInt32)RTC_H12) << 22));

                RTC_WPR.Write(0xCA);
                RTC_WPR.Write(0x53);
                int status = ERROR;
                if (RTC_EnterInitMode() == ERROR)
                {
                    status = ERROR;
                }
                else
                {
                    /* Set the RTC_TR register */
                    Register RTC_TR = new Register(RTC_BASE + 0x00);
                    RTC_TR.Write(tmpreg & RTC_TR_RESERVED_MASK);
                    /* Exit Initialization mode */
                    RTC_ExitInitMode();

                    if (RTC_WaitForSynchro() == ERROR)
                    {
                        status = ERROR;
                    }
                    else
                    {
                        status = SUCCESS;
                    }

                }
                /* Enable the write protection for RTC registers */
                RTC_WPR.Write(0xFF);
                return status;
            }
            #endregion
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Runtime.CompilerServices;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// Provides access to the LCD Screen Controller.
    /// </summary>
    public class LCDController
    {
        /// <summary>
        /// Configurations.
        /// </summary>
        public class Configurations
        {
            /// <summary>
            /// Constructor.
            /// </summary>
            public Configurations() { }

            /// <summary>
            /// Constructor.
            /// </summary>
            /// <param name="Width">Resolution width.</param>
            /// <param name="Height">Resolution height.</param>
            /// <param name="PriorityEnable">Internal LCD bus priority.</param>
            /// <param name="OutputEnableIsFixed"><c>true</c> if LCD enable is a fixed state. <c>false</c> if LCD enable is managed by LCD controller.</param>
            /// <param name="OutputEnablePolarity"><c>true</c> if active state is high.</param>
            /// <param name="HorizontalSyncPolarity"><c>true</c> if active state is high.</param>
            /// <param name="VerticalSyncPolarity"><c>true</c> if active state is high.</param>
            /// <param name="PixelPolarity"><c>true</c> if your LCD clocks data on the rising edge of the LCD clock.</param>
            /// <param name="HorizontalSyncPulseWidth">Horizontal Sync pulse width in clock cycles.</param>
            /// <param name="HorizontalBackPorch">Horizontal back porch in clock cycles.</param>
            /// <param name="HorizontalFrontPorch">Horizontal front porch in clock cycles.</param>
            /// <param name="VerticalSyncPulseWidth">Vertical Sync pulse width given by number of lines.</param>
            /// <param name="VerticalBackPorch">Vertical back porch given by number of lines.</param>
            /// <param name="VerticalFrontPorch">Vertical front porch given by number of lines.</param>
            /// <param name="PixelClockRateKHz">Pixel clock rate in KHz.</param>
            public Configurations(uint Width, uint Height,
            bool PriorityEnable,
            bool OutputEnableIsFixed, bool OutputEnablePolarity,
            bool HorizontalSyncPolarity, bool VerticalSyncPolarity, bool PixelPolarity,
            byte HorizontalSyncPulseWidth, byte HorizontalBackPorch, byte HorizontalFrontPorch,
            byte VerticalSyncPulseWidth, byte VerticalBackPorch, byte VerticalFrontPorch,
            uint PixelClockRateKHz)
            {
                this.Width = Width;
                this.Height = Height;

                this.PriorityEnable = PriorityEnable;

                this.OutputEnableIsFixed = OutputEnableIsFixed;
                this.OutputEnablePolarity = OutputEnablePolarity;

                this.HorizontalSyncPolarity = HorizontalSyncPolarity;
                this.VerticalSyncPolarity = VerticalSyncPolarity;
                this.PixelPolarity = PixelPolarity;

                this.HorizontalSyncPulseWidth = HorizontalSyncPulseWidth;
                this.HorizontalBackPorch = HorizontalBackPorch;
                this.HorizontalFrontPorch = HorizontalFrontPorch;
                this.VerticalSyncPulseWidth = VerticalSyncPulseWidth;
                this.VerticalBackPorch = VerticalBackPorch;
                this.VerticalFrontPorch = VerticalFrontPorch;

                this.PixelClockRateKHz = PixelClockRateKHz;
            }

            /// <summary>
            /// Resolution width.
            /// </summary>
            public uint Width;
            /// <summary>
            /// Resolution height.
            /// </summary>
            public uint Height;

            /// <summary>
            /// Internal LCD bus priority.
            /// </summary>
            /// <remarks>
            /// This is only supported on EMX. By setting this flag to true, the LCD controller will get the highest priority for the processor which will significantly reduce the general performance of the system for other processes. This feature is usually used for displays that require very high refresh rates such as VGA displays.
            /// </remarks>
            public bool PriorityEnable;

            /// <summary>
            /// <c>true</c> if LCD enable is a fixed state. <c>false</c> if LCD enable is managed by LCD controller.
            /// </summary>
            /// <remarks>
            /// For ChipworkX, LCD Enable is PB3. If Enable is fixed, no changes are done for PB3 and it can be used as a regular I/O. If Enable is not fixed, LCD Enable is handled by the LCD controller directly.
            /// </remarks>
            public bool OutputEnableIsFixed;
            /// <summary>
            /// <c>true</c> if active state is high.
            /// </summary>
            public bool OutputEnablePolarity;

            /// <summary>
            /// <c>true</c> if active state is high.
            /// </summary>
            public bool HorizontalSyncPolarity;
            /// <summary>
            /// <c>true</c> if active state is high.
            /// </summary>
            public bool VerticalSyncPolarity;
            /// <summary>
            /// <c>true</c> if your LCD clocks data on the rising edge of the LCD clock.
            /// </summary>
            public bool PixelPolarity;

            /// <summary>
            /// Horizontal Sync pulse width in clock cycles.
            /// </summary>
            public byte HorizontalSyncPulseWidth;
            /// <summary>
            /// Horizontal back porch in clock cycles.
            /// </summary>
            public byte HorizontalBackPorch;
            /// <summary>
            /// Horizontal front porch in clock cycles.
            /// </summary>
            public byte HorizontalFrontPorch;
            /// <summary>
            /// Vertical Sync pulse width given by number of lines.
            /// </summary>
            public byte VerticalSyncPulseWidth;
            /// <summary>
            /// Vertical back porch given by number of lines.
            /// </summary>
            public byte VerticalBackPorch;
            /// <summary>
            /// Vertical front porch given by number of lines.
            /// </summary>
            public byte VerticalFrontPorch;

            /// <summary>
            /// Pixel clock rate in KHz to calculate the pixel clock divider.
            /// </summary>
            public uint PixelClockRateKHz;

        }

        static LCDController()
        {
            HeadlessConfig = new Configurations();
            HeadlessConfig.Width = 0;
            HeadlessConfig.Height = 0;
        }

        /// <summary>
        /// Choose for headless LCD.
        /// </summary>
        public static Configurations HeadlessConfig;

        /// <summary>
        /// Sets LCD Configuration.
        /// </summary>
        /// <param name="config">LCD configuration stucture containing LCD configuration information to be set.</param>
        /// <returns><c>true</c> if configurations were updated. <c>false</c> if configurations did not change.</returns>
        public static bool Set(Configurations config)
        {
            if (DeviceInfo.GetDeviceID() != DeviceID.FEZ_HYDRA)
                throw new NotImplementedException("LCD Controller only available for FEZ Hydra at this time.");

            return Set(config.Width, config.Height,
            config.PriorityEnable,
            config.OutputEnableIsFixed, config.OutputEnablePolarity,
            config.HorizontalSyncPolarity, config.VerticalSyncPolarity, config.PixelPolarity,
            config.HorizontalSyncPulseWidth, config.HorizontalBackPorch, config.HorizontalFrontPorch,
            config.VerticalSyncPulseWidth, config.VerticalBackPorch, config.VerticalFrontPorch,
            config.PixelClockRateKHz);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool Set(uint Width, uint Height,
            bool PriorityEnable,
            bool OutputEnableIsFixed, bool OutputEnablePolarity,
            bool HorizontalSyncPolarity, bool VerticalSyncPolarity, bool PixelPolarity,
            byte HorizontalSyncPulseWidth, byte HorizontalBackPorch, byte HorizontalFrontPorch,
            byte VerticalSyncPulseWidth, byte VerticalBackPorch, byte VerticalFrontPorch,
            uint PixelClockRateKHz);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// Utilities.
    /// </summary>
    public static class Util
    {
        /// <summary>
        /// Bits per pixel conversion type.
        /// </summary>
        public enum BPP_Type : byte
        {
            /// <summary>
            /// 1 BPP for displays of 128 width and 128 or 64 height. The 1-bit pixel is set if the bitmap pixel has any color besides black.
            /// </summary>
            BPP1_x128,
            /// <summary>
            /// 8 BPP with RGB 3:3:2.
            /// </summary>
            BPP8_RGB,//3:3:2
            /// <summary>
            /// 8 BPP from red channel.
            /// </summary>
            BPP8_RED,
            /// <summary>
            /// 8 BPP from green channel.
            /// </summary>
            BPP8_GREEN,
            /// <summary>
            /// 8 BPP from blue channel.
            /// </summary>
            BPP8_BLUE,
            /// <summary>
            /// Big Endian 16 BPP with RGB 5:6:5. 
            /// </summary>
            BPP16_RGB_BE,//5:6:5
            /// <summary>
            /// Little Endian 16 BPP with RGB 5:6:5. 
            /// </summary>
            BPP16_RGB_LE,
            /// <summary>
            /// Big Endian 16 BPP with BGR 5:6:5. 
            /// </summary>
            BPP16_BGR_BE,
            /// <summary>
            /// Little Endian 16 BPP with BGR 5:6:5. 
            /// </summary>
            BPP16_BGR_LE,
        }

        /// <summary>
        /// Converts <c>Bitmap</c> pixel bytes into different BPP representation.
        /// </summary>
        /// <param name="bitmapBuffer">Bitmap bytes.</param>
        /// <param name="outputBuffer">Output buffer after conversion.</param>
        /// <param name="bpp">Target bits per pixel representation.</param>
        /// <remarks>
        /// GHI Electronics' devices support Bitmap and WPF graphics natively on color TFT displays. A design may require a small low-cost SPI-based displays. Different SPI LCDs can have different BPP requirements. This function converts the Bitmap bytes, where each pixel is represented as 4 bytes, into the needed BPP representation and then it can be transferred to the display.<br/>
        /// Start by doing all necessary graphics in NETMF on a Bitmap object. When you need to flush the results to the LCD:<br/>
        /// 1) Get the Bitmap bytes using <c>myBitmap.GetBitmap()</c>.<br/>
        /// 2) Use this conversion method to convert the buffer into the needed BPP.<br/>
        /// 3) Send output buffer to LCD over SPI.<br/>
        /// <br/>
        /// This drawing technique can be used also with Windows Presentation Foundation but it does not work on old non-TFT Embedded Master and it requires the following:<br/>
        /// 1) Set the display size to match your SPI-display using SetLCD. Use any values for timing and 0xFF for <c>PixelClockDivider</c> which will disable TFT LCD refresh.<br/>
        /// 2) Use the following code to fetch the Bitmap object from the WPF system.
        /// <code>
        /// WindowManager.Instance.PostRender += new PostRenderEventHandler(Instance_PostRender);
        /// // …
        /// void Instance_PostRender(DrawingContext dc)
        /// {
        ///    // get bitmap bytes
        ///    byte[] MyBitmapBytes = dc.Bitmap.GetBitmap();
        /// 
        ///    // do any necessary conversion
        ///    Util.ConvertBPP(MyBitmapBytes, output, Util.BPP_Type.BPP16_RGB_LE);
        /// 
        ///    // send over SPI
        ///    MySPIDisplay.FlushData(output);
        /// }
        /// </code>
        /// </remarks>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void BitmapConvertBPP(byte[] bitmapBuffer, byte[] outputBuffer, BPP_Type bpp);

        /// <summary>
        /// Allows .wav file playback through AnalogOut
        /// </summary>
        /// <param name="channel">AnalogOut Channel</param>
        /// <param name="data">.wav file buffer</param>
        /// <param name="offset">Array offset</param>
        /// <param name="count">Size of playback sample</param>
        /// <param name="dataRate">Frequency in Hz</param>
        public static void PlayPCMAudio(Cpu.AnalogOutputChannel channel, byte[] data, int offset, int count, int dataRate)
        {
            if (DeviceInfo.GetDeviceID() == DeviceID.FEZ_CERB)
                PlayPCMAudio((byte)channel, data, offset, count, dataRate);
            else
            {
                throw new NotSupportedException();
            }
            
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static void PlayPCMAudio(byte channel, byte[] data, int offset, int count, int dataRate);

    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using Microsoft.SPOT.Hardware;

using System;
using System.Runtime.CompilerServices;

namespace GHI.OSHW.Hardware
{
    /// <summary>
    /// Master I2C bus functionality implemented in software.
    /// </summary>
    /// <remarks>
    /// This class provides I2C functionality on any two digital pins. It is implemented by toggling the pins using software. It only supports one master on the same bus. Multiple masters on different buses work correctly.<br/>
    /// <br/>
    /// You must provide two pins: one for I2C clock and one for I2C data. The user must connect external pull-up resistors on the pins as required for I2C operation.<br/>
    /// <br/>
    /// Currently, only 7-bit I2C addresses are supported.<br/>
    /// Clock rate is currently ignored. Specify 0 for this field. This will make the rate as fast as possible.
    /// </remarks>
    public class SoftwareI2CBus : IDisposable
    {
        /// <summary>
        /// Represents an I2C device on a bus.
        /// </summary>
        public class I2CDevice
        {
            SoftwareI2CBus bus;
            ushort address;
            int clockRateKHz;

            internal I2CDevice(SoftwareI2CBus bus, ushort address, int clockRateKHz)
            {
                this.bus = bus;
                this.address = address;
                this.clockRateKHz = clockRateKHz;
            }

            /// <summary>
            /// Writes a data buffer to the I2C device.
            /// </summary>
            /// <param name="buffer">Buffer.</param>
            /// <param name="offset">Offset into the buffer.</param>
            /// <param name="count">Number of bytes to write.</param>
            /// <returns>Number of bytes sent successfully.</returns>
            public int Write(byte[] buffer, int offset, int count)
            {
                int numWritten, numRead;

                WriteRead(buffer, offset, count, buffer, 0, 0, out numWritten, out numRead);

                return numWritten;
            }

            /// <summary>
            /// Reads a data buffer from the I2C device.
            /// </summary>
            /// <param name="buffer">Buffer.</param>
            /// <param name="offset">Offset into the buffer.</param>
            /// <param name="count">Number of bytes to read.</param>
            /// <returns>Number of bytes read successfully.</returns>
            public int Read(byte[] buffer, int offset, int count)
            {
                int numWritten, numRead;

                WriteRead(buffer, 0, 0, buffer, offset, count, out numWritten, out numRead);

                return numRead;
            }

            /// <summary>
            /// Writes a data buffer then reads a data buffer from the I2C device.
            /// </summary>
            /// <param name="writeBuffer">Write buffer.</param>
            /// <param name="writeOffset">Offset into the write buffer.</param>
            /// <param name="writeLength">Number of bytes to write.</param>
            /// <param name="readBuffer">Read buffer.</param>
            /// <param name="readOffset">Offset into the read buffer.</param>
            /// <param name="readLength">Number of bytes to read.</param>
            /// <param name="numWritten">Number of bytes written successfully.</param>
            /// <param name="numRead">Number of bytes read successfully.</param>
            /// <returns><c>true</c> if all the required bytes are written and read successfully.</returns>
            public bool WriteRead(byte[] writeBuffer, int writeOffset, int writeLength, byte[] readBuffer, int readOffset, int readLength, out int numWritten, out int numRead)
            {
                lock (bus)
                {
                    if (bus.disposed)
                        throw new ObjectDisposedException();

                    return SoftwareI2CBus.NativeI2CWriteRead((int)bus.clockPin, (int)bus.dataPin, clockRateKHz, address, writeBuffer, writeOffset, writeLength, readBuffer, readOffset, readLength, out numWritten, out numRead);
                }
            }
        }

        bool disposed = false;
        Cpu.Pin clockPin;
        Cpu.Pin dataPin;

        /// <summary>
        /// Creates an I2C bus using the clock and data pins.
        /// </summary>
        /// <param name="clockPin">Clock pin.</param>
        /// <param name="dataPin">Data pin.</param>
        public SoftwareI2CBus(Cpu.Pin clockPin, Cpu.Pin dataPin)
        {
            if (Port.ReservePin(clockPin, true))
            {
                if (Port.ReservePin(dataPin, true) == false)
                {
                    Port.ReservePin(clockPin, false);
                    throw new InvalidOperationException();
                }
            }
            else
                throw new InvalidOperationException();

            this.clockPin = clockPin;
            this.dataPin = dataPin;
        }

        /// <summary>
        /// Creates an I2C device on the bus.
        /// </summary>
        /// <param name="address">Address of the device. This must be a 7 bit address.</param>
        /// <param name="clockRateKHz">Clock rate in KHz. Currently ignored. Specify 0 to go as fast as possible.</param>
        /// <returns>Created I2C device.</returns>
        public I2CDevice CreateI2CDevice(ushort address, int clockRateKHz)
        {
            return new I2CDevice(this, address, clockRateKHz);
        }

        /// <summary>
        /// Processes I2C directly without creating objects.
        /// </summary>
        /// <param name="clockPin">Clock pin.</param>
        /// <param name="dataPin">Data pin.</param>
        /// <param name="clockRateKHz">Clock rate in KHz.</param>
        /// <param name="address">Address of the device.</param>
        /// <param name="writeBuffer">Write buffer.</param>
        /// <param name="writeOffset">Offset into the write buffer.</param>
        /// <param name="writeLength">Number of bytes to write.</param>
        /// <param name="readBuffer">Read buffer.</param>
        /// <param name="readOffset">Offset into the read buffer.</param>
        /// <param name="readLength">Number of bytes to read.</param>
        /// <param name="numWritten">Number of bytes written successfully.</param>
        /// <param name="numRead">Number of bytes read successfully.</param>
        /// <returns><c>true</c> if all the required bytes are written and read successfully.</returns>
        /// <remarks>
        /// Do not use this method. It is intended for internal use only. See <c>CreateI2CDevice</c> instead.
        /// </remarks>
        // This is done as a static specifically to make it easier for Gadgeteer to access.
        public static bool DirectI2CWriteRead(Cpu.Pin clockPin, Cpu.Pin dataPin, int clockRateKHz, ushort address, byte[] writeBuffer, int writeOffset, int writeLength, byte[] readBuffer, int readOffset, int readLength, out int numWritten, out int numRead)
        {
            // this is because interops require non null arrays
            if (writeBuffer == null)
            {
                writeBuffer = readBuffer;
                writeLength = 0;
            }

            if (readBuffer == null)
            {
                readBuffer = writeBuffer;
                readLength = 0;
            }

            return NativeI2CWriteRead((int)clockPin, (int)dataPin, clockRateKHz, address, writeBuffer, writeOffset, writeLength, readBuffer, readOffset, readLength, out numWritten, out numRead);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static bool NativeI2CWriteRead(int clockPin, int dataPin, int clockRateKHz, ushort address, byte[] writeBuffer, int writeOffset, int writeLength, byte[] readBuffer, int readOffset, int readLength, out int numWritten, out int numRead);

        /// <summary>
        /// Disposes the I2C bus.
        /// </summary>
        public void Dispose()
        {
            lock (this)
            {
                disposed = true;

                Port.ReservePin(clockPin, false);
                Port.ReservePin(dataPin, false);
            }
        }

        /// <exclude/>
        ~SoftwareI2CBus()
        {
            Dispose();
        }
    }
}

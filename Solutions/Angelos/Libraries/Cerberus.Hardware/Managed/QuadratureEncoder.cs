using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Cerberus.Hardware
    {
    /// <summary>
    /// Event handler for Input capture
    /// </summary>
    /// <param name="source">Source pin</param>
    /// <param name="count">Count captured</param>
    /// <param name="time">Capture time</param>
    /// <param name="overCapture">Indicates whether one or more events was too close to be captured</param>
    public delegate void InputCaptureEventHandler(int source, int count, DateTime time, bool overCapture);

    /// <summary>
    /// Hardware quadrature encoder with 32 bits counter on pins PA0 and PA1
    /// <para>Allows independent capture of the counter value on falling edge of PA2 and PA3</para>
    /// </summary>
    public class QuadratureEncoder : IDisposable
        {
        private NativeEventDispatcher _dispatcher;

        /// <summary>
        /// Initialize a new <see cref="QuadratureEncoder"/>
        /// </summary>
        public QuadratureEncoder() {
            _dispatcher = new NativeEventDispatcher("QuadratureEncoder_Driver", 0);
            _dispatcher.OnInterrupt += Dispatcher_OnInterrupt;
            }
        /// <summary>
        /// Dispose native resources
        /// </summary>
        public void Dispose() {
            _dispatcher.OnInterrupt -= Dispatcher_OnInterrupt;
            _dispatcher.Dispose();
            _dispatcher = null;
            }

        /// <summary>
        /// Get or set the counter value
        /// </summary>
        public int Count {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
            }

        /// <summary>
        /// Gets or sets a value indicating whether a falling edge on an input captures the counter value
        /// </summary>
        public bool InputCaptureEnabled {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
            }

        //private bool _outputCompareEnabled;

        ///// <summary>
        ///// Gets or sets a value indicating whether the output is updated according the the counter value
        ///// </summary>
        //public bool OutputCompareEnabled {
        //    get { return _outputCompareEnabled; }
        //    set { _outputCompareEnabled = value; }
        //    }

        /// <summary>
        /// Gets or sets a value indicating whether the counter is updated according to the encoder inputs
        /// <para>When not enabled, the counter retains its value regardless of the encoder inputs</para>
        /// </summary>
        public bool CountEnabled {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
            }

        /// <summary>
        /// Relay native interrupt
        /// </summary>
        /// <param name="data1"></param>
        /// <param name="data2"></param>
        /// <param name="time"></param>
        private void Dispatcher_OnInterrupt(uint data1, uint data2, DateTime time) {
            if (_inputCapture != null) {
                int source = (int)(data1 & 0xF);
                int count = (int)data2;
                bool overCapture = (data1 & 0x10) == 0x10;
                _inputCapture(source, count, time, overCapture);
                }
            }

        private InputCaptureEventHandler _inputCapture;
        /// <summary>
        /// Event fired by input capture on pin PA2 or PA3
        /// <para>This event is not fired if <see cref="InputCaptureEnabled"/>is false</para>
        /// </summary>
        public event InputCaptureEventHandler InputCapture {
            [MethodImpl(MethodImplOptions.Synchronized)] 
            add {
                _inputCapture += value;
                }
            [MethodImpl(MethodImplOptions.Synchronized)]
            remove {
                _inputCapture -= value;
                if (_inputCapture == null)
                    InputCaptureEnabled = false;
                }
            }

        }
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Runtime.CompilerServices;

namespace GHI.OSHW.Native
{
    /// <summary>
    /// Runtime Loadable Procedures.
    /// </summary>
    /// <remarks>
    /// RLPLite allows developers to load/execute their own compiled C/Assembly procedures at runtime. RLPLite supports loading binary images using <c>GHIElectronics.NETMF.Hardware.LowLevel.AddressSpace</c> class. The user can load multiple images in the reserved RLP memory region.<br/>
    /// Compiled C/Assembly procedures are executed natively and can perform much faster than managed application. This is very useful for calculation intensive applications.<br/>
    /// <br/>
    /// Steps to execute RLPLite:
    /// <ul>
    /// <li>Create a C or Assembly program. Make sure the program uses the available address space for the RLPLite. This is set by the linker using the linker scripts.</li>
    /// <li>Make sure that the exposed C procedures (called from C#) are in the required form.</li>
    /// <li>Debug the program as needed.</li>
    /// <li>Include the program binary image in the managed application. For example, it can be a Resource, a file on an SD card, a file obtained over a network...etc.</li>
    /// <li>In the managed application, load the native program into the reserved region.</li>
    /// <li>You can now invoke (access) the native functions from the managed application.</li>
    /// </ul>
    /// <br/>
    /// <strong style="color:blue">Platform Support and RLPLite Memory Mapping</strong><br/>
    /// The user must make sure that the native programs use the assigned memory for program code and for variables.<br/>
    /// <br/>
    /// <strong>FEZ Hydra:</strong> by default, RLPLite is enabled and unlocked.<br/>
    /// RLP Address: 0xA0000000. RLP Size: 0x00100000.<br/>
    /// For advanced users, this region is a cached mapping of non-cached SDRAM region starting at 0xB0000000.<br/>
    /// <br/>
    /// To use RLPLite, you must load a binary image directly.<br/>
    /// For example:
    /// <code>
    /// AddressSpace.Write(loadAddress, binaryImage, 0, binaryImage.Length); // Load binary image
    /// RLPLite.Procedure Foo = new RLPLite.Procedure(procedureAddress); // Create a procedure with a known address
    /// Foo.Invoke(...); // Access native code
    /// </code>
    /// <strong style="color:blue">Calling Procedures from Managed Code (C#)</strong><br/>
    /// After loading the image as mentioned above, you call the native code directly and pass arguments. You need to get the <c>Procedure</c>(s) as mentioned above and then call <c>Procedure.Invoke(...)</c>.<br/>
    /// Procedures can be invoked in many ways:<br/>
    /// <c>Invoke(float[] VoidArray, Int32[] IntArray, Byte[] ByteArray)</c> is a call passing an array of floating point numbers, an array of integers, and an array of unsigned bytes.<br/>
    /// <c>Invoke(UInt32[] VoidArray, Int32[] IntArray, Byte[] ByteArray)</c> is a call passing an array of unsigned intergers, an array of integers, and an array of unsigned bytes.<br/>
    /// <c>Invoke(Int32[] VoidArray, Int32[] IntArray, Byte[] ByteArray)</c> is a call passing an array of signed intergers, an array of integers, and an array of unsigned bytes.<br/>
    /// <c>Invoke(Byte[] VoidArray, Int32[] IntArray, Byte[] ByteArray)</c> is a call passing an array of unsigned bytes, an array of integers, and an array of unsigned bytes.<br/>
    /// <c>Invoke(SByte[] VoidArray, Int32[] IntArray, Byte[] ByteArray)</c> is a call passing an array of signed bytes, an array of integers, and an array of unsigned bytes.<br/>
    /// Examples:
    /// <code>
    /// byte [] binfile = Resources.GetBytes(Resources.BinaryResources.RLPLite_Test);
    /// AddressSpace.Write(0xA0000000, binfile, 0, 0x00000328);
    ///
    /// RLPLite.Procedure RLP_MemSet = new RLPLite.Procedure(0xa00000c0);
    ///
    /// byte[] byteArray = new byte[50];
    /// int[] value = new int[1];
    /// value[0] = 1;
    /// byte[] length = new byte[1];
    /// length[0] = 50;
    ///
    /// int ret = 0;
    ///
    /// ret = RLP_MemSet.Invoke(byteArray, value, length);    
    /// </code>
    /// <strong style="color:blue">Native Procedures Implementation (C Language)</strong><br/>
    /// All managed overload methods endup invoking the following function through RLPLite/Invoke method.<br/>
    /// <c>int FunctionName(Type *voidArray, int* intArray, unsigned char* byteArray)</c><br/>
    /// <strong>voidArray, intArray, and byteArray</strong> is the same as managed C# array. Changing values in the native side will also change them in the managed application.<br/>
    /// The <c>Type</c> should be replaced appropriately. For example:<br/>
    /// C# is passing <c>byte[]</c> array. The corresponding C declaration is: <c>Type *voidArray</c> replaced by <c>unsigned char *voidArray</c>.<br/>
    /// C# is passing <c>float[]</c> array. The corresponding C declaration is: <c>Type *voidArray</c> replaced by <c>float *voidArray</c>.<br/>
    /// <br/>
    /// When some of these arguments are not used, they are not necessarily <c>NULL</c> and should not be accessed.<br/>
    /// <br/>
    /// <strong style="color:blue">Native Procedures Important Notes</strong>
    /// <ul>
    /// <li>Native function calls suspend all other C# threads and return only when the native procedure finishes.</li>
    /// <li>Hardware interrupts are always active unless they are explicitly stopped.</li>
    /// <li>You cannot pin arrays. When you receive pointers from managed code and use them in C/Assembly code, do not store them. After the native procedure returns, these pointers are invalid. When the garbage collector start working, it might change the arrays/pointers locations.</li> 
    /// </ul>
    /// </remarks>
    public static class RLPLite
    {
        static RLPLite()
        {

        }

        ///////////////////////////////////////////////

        /// <summary>
        /// Native Procedure. 
        /// </summary>
        public class Procedure
        {
            uint address;

            /// <summary>
            /// Creates a native procedure from a given address.
            /// </summary>
            /// <param name="address">Procedure address.</param>
            public Procedure(uint address)
            {
                this.address = address;
            }

            /// <summary>
            /// Calls a native procedure.
            /// </summary>
            /// <param name="VoidArray">Array of type float.</param>
            /// <param name="IntArray">Array of type int.</param>
            /// <param name="ByteArray">Array of type byte.</param>            
            /// <returns>Return value.</returns>
            [MethodImpl(MethodImplOptions.Synchronized)]
            public int Invoke(float[] VoidArray, Int32[] IntArray, Byte[] ByteArray)
            {
                int ret = 0;

                ret = Invoke_Helper(VoidArray, IntArray, ByteArray);

                return ret;
            }

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern private int Invoke_Helper(float[] VoidArray, Int32[] IntArray, Byte[] ByteArray);

            /// <summary>
            /// Calls a native procedure.
            /// </summary>
            /// <param name="VoidArray">Array of type unsigned int.</param>
            /// <param name="IntArray">Array of type int.</param>
            /// <param name="ByteArray">Array of type byte.</param>            
            /// <returns>Return value.</returns>
            [MethodImpl(MethodImplOptions.Synchronized)]
            public int Invoke(UInt32[] VoidArray, Int32[] IntArray, Byte[] ByteArray)
            {
                int ret = 0;

                ret = Invoke_Helper(VoidArray, IntArray, ByteArray);

                return ret;
            }

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern private int Invoke_Helper(UInt32[] VoidArray, Int32[] IntArray, Byte[] ByteArray);

            /// <summary>
            /// Calls a native procedure.
            /// </summary>
            /// <param name="VoidArray">Array of type int.</param>
            /// <param name="IntArray">Array of type int.</param>
            /// <param name="ByteArray">Array of type byte.</param>            
            /// <returns>Return value.</returns>
            [MethodImpl(MethodImplOptions.Synchronized)]
            public int Invoke(Int32[] VoidArray, Int32[] IntArray, Byte[] ByteArray)
            {
                int ret = 0;

                ret = Invoke_Helper(VoidArray, IntArray, ByteArray);

                return ret;
            }

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern private int Invoke_Helper(Int32[] VoidArray, Int32[] IntArray, Byte[] ByteArray);

            /// <summary>
            /// Calls a native procedure.
            /// </summary>
            /// <param name="VoidArray">Array of type byte.</param>
            /// <param name="IntArray">Array of type int.</param>
            /// <param name="ByteArray">Array of type byte.</param>            
            /// <returns>Return value.</returns>
            [MethodImpl(MethodImplOptions.Synchronized)]
            public int Invoke(Byte[] VoidArray, Int32[] IntArray, Byte[] ByteArray)
            {
                int ret = 0;

                ret = Invoke_Helper(VoidArray, IntArray, ByteArray);

                return ret;
            }

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern private int Invoke_Helper(Byte[] VoidArray, Int32[] IntArray, Byte[] ByteArray);

            /// <summary>
            /// Calls a native procedure.
            /// </summary>
            /// <param name="VoidArray">Array of type signed byte.</param>
            /// <param name="IntArray">Array of type int.</param>
            /// <param name="ByteArray">Array of type byte.</param>            
            /// <returns>Return value.</returns>
            [MethodImpl(MethodImplOptions.Synchronized)]
            public int Invoke(SByte[] VoidArray, Int32[] IntArray, Byte[] ByteArray)
            {
                int ret = 0;

                ret = Invoke_Helper(VoidArray, IntArray, ByteArray);

                return ret;
            }

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern private int Invoke_Helper(SByte[] VoidArray, Int32[] IntArray, Byte[] ByteArray);

        }// end class Procedure
    }// end class RLPLite
}

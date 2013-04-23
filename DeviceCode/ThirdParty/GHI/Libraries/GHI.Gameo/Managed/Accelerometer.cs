using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using GHI.OSHW.Hardware;

namespace GHI.Gameo
{
    public static class Accelerometer
    {
        static SoftwareI2CBus i2cbus= null;
        static SoftwareI2CBus.I2CDevice i2cdev = null;
        const Cpu.Pin i2c_clk = (Cpu.Pin)(1 * 16 + 6); //clk: PB6
        const Cpu.Pin i2c_dat = (Cpu.Pin)(1 * 16 + 7);  //data: PB7
        static Boolean isActived = false;
        //const String TEXT_EXCEPTION_00 = "Accelerometer needs to be enable by calling Enable() function";
        static public void Enable()
        {


            if (i2cbus == null)
            {
                i2cbus = new SoftwareI2CBus(i2c_clk, i2c_dat);
            }
            if (i2cdev == null)
            {
                i2cdev = i2cbus.CreateI2CDevice(0x1C, 400);
            }
               
           
           
            //bit 0 is 1 - active
            //bit 0 is 0 - standby
            //bit 1 = 1: 8 bit accurate - fast mode read
            //bit 1 = 0: 10 bit accurate - normal mode read
            
            isActived = true;
            WriteToRegister(0x2A, 1); //bit 0 is 1 - active
            
        }
        static public void Disable()
        {
            if (i2cbus != null && i2cdev != null)
            {
                WriteToRegister(0x2A, 0);//bit 0 is 0 - standby
                isActived = false;
                i2cdev = null;
                i2cbus.Dispose();
                i2cbus = null;
            }
            
            
        }
        static public Boolean IsActived
        {
            get
            {
                return isActived;
            }
        }
     
        static void WriteToRegister(byte reg, byte value)
        {

            byte[] RegisterNum = new byte[2] { reg, value };
            if (isActived)
            {
                i2cdev.Write(RegisterNum, 0, 2);
            }
            //else
            //{
            //    throw new Exception("Accelerometer needs to be enable by calling Enable() function");
            //}
        }        
        static byte[] ReadFromRegister(byte reg, int readcount)
        {
            byte[] RegisterNum = new byte[1] { reg };
            // create read buffer to read the register
            byte[] RegisterValue = new byte[readcount];
            int out_num_write = 0;
            int out_num_read = 0;
            if (isActived)
            {                
                i2cdev.WriteRead(RegisterNum, 0, 1, RegisterValue, 0, readcount, out out_num_write, out out_num_read);
            }
            //else
            //{
            //    throw new Exception("Accelerometer needs to be enable by calling Enable() function");
            //}
            return RegisterValue;
        }
        public static void GetXYZ(out int X, out int Y, out int Z)
        {
            byte[] test = ReadFromRegister(0x1, 6);
            X  = test[0] << 2 | test[1] >> 6 & 0x3F;
            Y = test[2] << 2 | test[3] >> 6 & 0x3F;
            Z = test[4] << 2 | test[5] >> 6 & 0x3F;
            if (X > 511) X -= 1024;
            if (Y > 511) Y -= 1024;
            if (Z > 511) Z -= 1024;           
        }
        public static int GetX()
        {
        
            byte[] test = ReadFromRegister(0x1, 2);
            int x = test[0] << 2 | test[1]>>6 & 0x3F;
            if (x > 511) x = x - 1024;
            return x;
           
          
        }
        public static int GetY()
        {
            byte[] test = ReadFromRegister(0x3, 2);
            int y = test[0] << 2 | test[1] >> 6 & 0x3F;
            if (y > 511) y = y - 1024;
            return y;
          
        }
        public static int GetZ()
        {
            byte[] test = ReadFromRegister(0x5, 2);
            int z = test[0] << 2 | test[1] >> 6 & 0x3F;
            if (z > 511) z = z - 1024;
            return z;
        }
    }
}

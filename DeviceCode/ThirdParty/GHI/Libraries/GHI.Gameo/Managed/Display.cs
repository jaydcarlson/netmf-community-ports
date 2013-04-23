using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Runtime.CompilerServices;
namespace GHI.Gameo
{
    public static class Display
    {
        const ushort LCD_SCREEN_WIDTH = 240;
        const ushort LCD_SCREEN_HEIGHT = 320;
        static OutputPort pinReset = new OutputPort((Cpu.Pin)(2*16+13), false); // PC13 
        static OutputPort pinBacklight = new OutputPort((Cpu.Pin)(0 * 16 + 7), true); //PA7        
        
        static ushort lcd_screen_width;
        static ushort lcd_screen_height;
        //const String TEXT_EXCEPTION_00 = "Invalid param.";
        static public Orientation CurrentOrientation
        {
            get { return GetScreenWidth == LCD_SCREEN_WIDTH ? Orientation.Portrait : Orientation.Landscape; }
            set { Initialize(value); }
        }
        static public ushort GetScreenWidth
        {
            get { return lcd_screen_width; }
              
        }
        static public ushort GetScreenHeight
        {
            get { return lcd_screen_height; }

        }
        public enum Orientation : byte
        {
            Portrait =0,
            Landscape
        }
        static public void Reset()
        {
            pinReset.Write(true);
            Thread.Sleep(200);
            pinReset.Write(false);
            Thread.Sleep(500);
            pinReset.Write(true);
            Thread.Sleep(500);
        }
        static public void TurnBackLightOn()
        {
            pinBacklight.Write(true);
        }
        static public void TurnBackLightOff()
        {
            pinBacklight.Write(false);
        }           
       
        static public void DrawString(Font font, String text, int x, int y, UInt32 text_color, UInt32 back_color)
        {
            int width = 0;
            for (int i = 0; i < text.Length; i++)
            {
                width+=font.CharWidth((char)(text[i]));
            }
            Bitmap bmp = new Bitmap(width, font.Height);
            bmp.DrawRectangle((Microsoft.SPOT.Presentation.Media.Color)back_color, 100, 0, 0, width, font.Height, 0, 0, (Microsoft.SPOT.Presentation.Media.Color)back_color, 0, 0, (Microsoft.SPOT.Presentation.Media.Color)back_color, 0, 0, 0);
            bmp.DrawText(text, font, (Microsoft.SPOT.Presentation.Media.Color)text_color, 0, 0);
            Image imge = new Image(bmp); // Note: Maybe out of memory here
            DrawImage(imge, x, y);
            bmp.Dispose();
            bmp = null;
            imge.Dispose();
            imge = null;
        }
        static public void DrawImage(Image image, int xDst, int yDst)
        {
            if (false == DrawImage(image.image_data, (ushort)xDst, (ushort)yDst, image.ImageWidth, image.ImageHeight, 0, 0, image.ImageWidth, image.ImageHeight))
            {
                throw new Exception("Invalid param.");
            }
        }
        static public void DrawImage(Image image, int xDst, int yDst, int xSrc, int ySrc, int width, int height)
        {
            if (false == DrawImage(image.image_data, (ushort)xDst, (ushort)yDst, image.ImageWidth, image.ImageHeight, (ushort)xSrc, (ushort)ySrc, (ushort)width, (ushort)height))
            {
                throw new Exception("Invalid param.");
            }
        }
        static public Boolean Initialize(Orientation screenmode)
        {
            if (screenmode == Orientation.Landscape)
            {
                lcd_screen_width = LCD_SCREEN_HEIGHT;
                lcd_screen_height = LCD_SCREEN_WIDTH;
            }
            else
            {
                lcd_screen_width = LCD_SCREEN_WIDTH;
                lcd_screen_height = LCD_SCREEN_HEIGHT;
            }
            return Initialize(screenmode, lcd_screen_width, lcd_screen_height);
            

        }
        static public void FillRect(int x, int y, int width, int height, ushort color)
        {
            FillRect(new byte[2], (ushort)x, (ushort)y, (ushort)width, (ushort)height, color);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static private Boolean Initialize(Orientation screenmode, ushort screenwidt, ushort screenheight);     

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Boolean WriteRaw(byte[] data, int offset, int count);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Boolean WriteRaw(ushort[] data, int offset, int count);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Boolean WriteRegister(ushort register, ushort value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Boolean WriteRegister(ushort register);
      
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static private Boolean FillRect(byte[] buffercache, ushort x, ushort y, ushort width, ushort height, ushort color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static private Boolean DrawImage(byte[] image, ushort xDst, ushort yDst, ushort originalWidth, ushort originalHeight, ushort xSrc, ushort ySrc, ushort width, ushort height);
        static public void SetDefaultConfiguration()
        {
            Display.WriteRegister(0x0000, 0x0001); Thread.Sleep(50);
            Display.WriteRegister(0x002b, 0x0003); Thread.Sleep(50);
            Display.WriteRegister(0x0007, 0x0000); Thread.Sleep(50);
            Display.WriteRegister(0x0012, 0x0000); Thread.Sleep(50);
            Display.WriteRegister(0x0060, 0xa700); Thread.Sleep(2);
            Display.WriteRegister(0x0008, 0x0405); Thread.Sleep(2);
            Display.WriteRegister(0x0030, 0x0002); Thread.Sleep(2);
            Display.WriteRegister(0x0031, 0x0307); Thread.Sleep(2);
            Display.WriteRegister(0x0032, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0035, 0x0606); Thread.Sleep(2);
            Display.WriteRegister(0x0036, 0x080e); Thread.Sleep(2);
            Display.WriteRegister(0x0037, 0x0204); Thread.Sleep(2);
            Display.WriteRegister(0x0038, 0x0203); Thread.Sleep(2);
            Display.WriteRegister(0x0039, 0x0707); Thread.Sleep(2);
            Display.WriteRegister(0x003c, 0x0400); Thread.Sleep(2);
            Display.WriteRegister(0x003d, 0x0005); Thread.Sleep(2);
            Display.WriteRegister(0x0010, 0x02c0); Thread.Sleep(2);
            Display.WriteRegister(0x0011, 0x0127); Thread.Sleep(30);
            Display.WriteRegister(0x0012, 0x011c); Thread.Sleep(30);
            Display.WriteRegister(0x0013, 0x1600); Thread.Sleep(2);
            Display.WriteRegister(0x0029, 0x0010); Thread.Sleep(30);
            Display.WriteRegister(0x0001, 0x0100); Thread.Sleep(2);
            Display.WriteRegister(0x0002, 0x0700); Thread.Sleep(2);
            Display.WriteRegister(0x0003, 0x0030); Thread.Sleep(2);
            Display.WriteRegister(0x0061, 0x0001); Thread.Sleep(2);
            Display.WriteRegister(0x0090, 0x001c); Thread.Sleep(2);
            Display.WriteRegister(0x0092, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0080, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0081, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0082, 0x013f); Thread.Sleep(2);
            Display.WriteRegister(0x0083, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0084, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0085, 0x013f); Thread.Sleep(2);
            Display.WriteRegister(0x0050, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0051, 0x00ef); Thread.Sleep(2);
            Display.WriteRegister(0x0052, 0x0000); Thread.Sleep(2);
            Display.WriteRegister(0x0053, 0x013f); Thread.Sleep(2);
            Display.WriteRegister(0x0091, 0x0300); Thread.Sleep(2);
            Display.WriteRegister(0x0098, 0x0303); Thread.Sleep(2);
            Display.WriteRegister(0x0010, 0x12C0); Thread.Sleep(20);
            Display.WriteRegister(0x0007, 0x0133); Thread.Sleep(20);            
        }

    }
}

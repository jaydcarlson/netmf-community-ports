using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Presentation;


namespace GHI.Gameo
{
    public class Image
    {
        internal byte[] image_data;
        readonly ushort image_width;
        readonly ushort image_height;

        public Image(Bitmap imageBMP)
        {
            image_data = new byte[imageBMP.Width * imageBMP.Height * 2];
            GHI.OSHW.Hardware.Util.BitmapConvertBPP(imageBMP.GetBitmap(), image_data, GHI.OSHW.Hardware.Util.BPP_Type.BPP16_RGB_BE);
            image_width = (ushort)imageBMP.Width;
            image_height = (ushort)imageBMP.Height;
            
        }
        public Image(byte[] data, ushort width, ushort height)
        {
            if (data.Length != width * height * 2)
                throw new Exception("Invalid param. Image data should be BPP16_RGB_BE.");
            image_data = data;
            image_width = width;
            image_height = height;
        }
        ~Image()
        {
            Dispose();
        }
        public void Dispose()
        {            
            image_data = null;
        }
        public ushort ImageWidth
        {
            get { return image_width; }

        }
        public ushort ImageHeight
        {
            get { return image_height; }

        }
    }
}

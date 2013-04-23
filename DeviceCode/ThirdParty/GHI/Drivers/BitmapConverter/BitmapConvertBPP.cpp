#include <ThirdParty\GHI\Include\GHI_OSHW_PAL.h>

enum BPP_Type
{
	BPP1_x128,
	BPP8_RGB,//3:3:2
	BPP8_RED,
	BPP8_GREEN,
	BPP8_BLUE,
	BPP16_RGB_BE,//5:6:5
	BPP16_RGB_LE,
	BPP16_BGR_BE,
	BPP16_BGR_LE,
};

OSHW_PAL_ERROR OSHW_PAL_Bitmap_ConvertBPP(BYTE* bimap, INT32 bimapSize, BYTE* output, INT32 outputSize, BYTE bpp)
{
	int i,z;
	int temp32=0;
	UINT32 * p32;
	UINT16 * p16;
	UINT8 * p8;
	UINT16 temp;
	UINT8 b;

	if(bimapSize%4)
		return CLR_E_INVALID_PARAMETER;

	switch(bpp)
	{
	case BPP1_x128:
		if(outputSize < bimapSize/4/8)
			return CLR_E_INDEX_OUT_OF_RANGE;
    
		p32 = (UINT32 *) bimap;
		//memset(output, 0, outputSize);

		for(i=0;i<bimapSize/4/128/8;i++)
		{
			for(z=0;z<128;z++)
			{
				b=0;
		
				if(p32[128*0])//0
					b|=1;
				if(p32[128*1])//1
					b|=2;
				if(p32[128*2])//2
					b|=4;
				if(p32[128*3])//3
					b|=8;
				if(p32[128*4])//4
					b|=0x10;
				if(p32[128*5])//5
					b|=0x20;
				if(p32[128*6])//6
					b|=0x40;
				if(p32[128*7])//7
					b|=0x80;

				*output=b;
				output++;
				p32++;
			}
			p32+=(1024-128);
		}
		break;
	case BPP8_BLUE:
		temp32++;
	case BPP8_GREEN:
		temp32++;
	case BPP8_RED:
  	if(outputSize < bimapSize/4)
			return CLR_E_INDEX_OUT_OF_RANGE;

		p8 = output;
		for(i=0 ; i<bimapSize ; i+=4)
		{
			*p8 = bimap[i+temp32] ;
			p8++;
		}
		break;
	case BPP8_RGB://8bit RGB 3:3:2
		if(outputSize < bimapSize/4)
			return CLR_E_INDEX_OUT_OF_RANGE;

		p8 = output;
		for(i=0 ; i<bimapSize ; i+=4)
		{
			*p8 = ((bimap[i+2] & 0xE0)) | ((bimap[i+1] & 0xE0)>>3 ) | (bimap[i] >>6) ;
			p8++;
		}
		break;
	case BPP16_RGB_BE://big endian RGB
		if(outputSize < bimapSize/2)
			return CLR_E_INDEX_OUT_OF_RANGE;

		//p32 = (UINT32 *) bimap;
		p16 = (UINT16 *) output;
		
		for(i=0;i<bimapSize;i+=4)
		{
			temp = (UINT16)((UINT16)(bimap[i+2] & 0xF8) << (5 + 6-3)) | ((UINT16)(bimap[i+1] & 0xFC)<< 5-2 ) | (bimap[i] >>3) ;
			//big endian systems
			*p16 = temp>>8;
			*p16 |= temp<<8;
			p16++;
		}
		break;
	case BPP16_BGR_BE://big endian BGR
		if(outputSize < bimapSize/2)
			return CLR_E_INDEX_OUT_OF_RANGE;

		//p32 = (UINT32 *) bimap;
		p16 = (UINT16 *) output;
		for(i=0;i<bimapSize;i+=4)
		{
			temp = (UINT16)((UINT16)(bimap[i] & 0xF8) << (5 + 6-3)) | ((UINT16)(bimap[i+1] & 0xFC)<< 5-2 ) | (bimap[i+2] >>3) ;
			//big endian systems
			*p16 = temp>>8;
			*p16 |= temp<<8;
			p16++;
		}
		break;
	case BPP16_RGB_LE://little endian RGB
		if(outputSize < bimapSize/2)
			return CLR_E_INDEX_OUT_OF_RANGE;

		//p32 = (UINT32 *) bimap;
		p16 = (UINT16 *) output;
		UINT16 temp;
		for(i=0;i<bimapSize;i+=4)
		{
			*p16 = (UINT16)((UINT16)(bimap[i+2] & 0xF8) << (5 + 6-3)) | ((UINT16)(bimap[i+1] & 0xFC)<< 5-2 ) | (bimap[i] >>3) ;
			p16++;
		}
		break;
	case BPP16_BGR_LE://little endian BGR
		if(outputSize < bimapSize/2)
			return CLR_E_INDEX_OUT_OF_RANGE;

		//p32 = (UINT32 *) bimap;
		p16 = (UINT16 *) output;
		for(i=0;i<bimapSize;i+=4)
		{
			*p16 = (UINT16)((UINT16)(bimap[i] & 0xF8) << (5 + 6-3)) | ((UINT16)(bimap[i+1] & 0xFC)<< 5-2 ) | (bimap[i+2] >>3) ;
			p16++;
		}
		break;
	default:
		return CLR_E_INVALID_PARAMETER;
	}
	return S_OK;
}

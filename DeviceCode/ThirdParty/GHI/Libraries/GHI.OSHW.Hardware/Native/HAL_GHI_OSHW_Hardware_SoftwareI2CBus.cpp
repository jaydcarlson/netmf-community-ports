//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "HAL.h"
#include "HAL_GHI_OSHW_Hardware_SoftwareI2CBus.h"

#include <ThirdParty\GHI\Include\GHI_OSHW_HAL.h>

using namespace GHI::OSHW::Hardware;

INT8 SoftwareI2CBus::NativeI2CWriteRead(  INT32 clockPin, INT32 dataPin, INT32 clockRateKhz, UINT16 address, CLR_RT_TypedArray_UINT8 writeBuffer, INT32 writeOffset, INT32 writeLength, CLR_RT_TypedArray_UINT8 readBuffer, INT32 readOffset, INT32 readLength, INT32 * numWritten, INT32 * numRead, HRESULT &hr )
{
	if( (writeOffset < 0) || (writeLength < 0) || ((writeOffset + writeLength) > writeBuffer.GetSize()) ||
		(readOffset < 0) || (readLength < 0) || ((readOffset + readLength) > readBuffer.GetSize()) 
		)
	{
		hr = CLR_E_INDEX_OUT_OF_RANGE;
		return FALSE;
	}

	GHI_OSHW_HAL_SoftwareI2C i2c;
	i2c.scl = clockPin;
	i2c.sda = dataPin;
	i2c.clockSpeed = clockRateKhz;
	i2c.address = address;

	return GHI_OSHW_HAL_SoftwareI2C_WriteRead(&i2c, writeBuffer.GetBuffer() + writeOffset, writeLength, readBuffer.GetBuffer() + readOffset, readLength, (UINT32*)numWritten, (UINT32*)numRead);
}


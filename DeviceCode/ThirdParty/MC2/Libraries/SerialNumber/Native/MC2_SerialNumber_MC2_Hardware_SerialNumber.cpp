#include "MC2_SerialNumber.h"
#include "MC2_SerialNumber_MC2_Hardware_SerialNumber.h"

using namespace MC2::Hardware;

UINT8 serialnum[64];

void SerialNumber::Get( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
	if(param0.GetSize() <= 64)
		memcpy(param0.GetBuffer(), serialnum, param0.GetSize());
}

UINT32 SerialNumber::GetCount( HRESULT &hr )
{
    return SerialNumber_Get(serialnum);
}


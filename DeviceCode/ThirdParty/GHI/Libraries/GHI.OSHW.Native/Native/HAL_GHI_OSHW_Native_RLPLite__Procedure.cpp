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
#include "HAL_GHI_OSHW_Native_RLPLite__Procedure.h"

using namespace GHI::OSHW::Native;

INT32 InvokeRLP(UINT32 address, void* param0, int* param1, unsigned char* param2);

INT32 RLPLite_Procedure::Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_float param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr )
{
	UINT32 address = Get_address( pMngObj );

	return InvokeRLP( address, param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer() );
}

INT32 RLPLite_Procedure::Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr )
{
	UINT32 address = Get_address( pMngObj );

	return InvokeRLP( address, param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer() );
}

INT32 RLPLite_Procedure::Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT32 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr )
{
	UINT32 address = Get_address( pMngObj );

	return InvokeRLP( address, param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer() );
}

INT32 RLPLite_Procedure::Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr )
{
	UINT32 address = Get_address( pMngObj );

	return InvokeRLP( address, param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer() );
}

INT32 RLPLite_Procedure::Invoke_Helper( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT8 param0, CLR_RT_TypedArray_INT32 param1, CLR_RT_TypedArray_UINT8 param2, HRESULT &hr )
{
	UINT32 address = Get_address( pMngObj );

	return InvokeRLP( address, param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer() );
}


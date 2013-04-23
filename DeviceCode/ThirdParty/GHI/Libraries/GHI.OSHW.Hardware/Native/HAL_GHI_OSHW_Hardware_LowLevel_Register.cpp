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
#include "HAL_GHI_OSHW_Hardware_LowLevel_Register.h"

using namespace GHI::OSHW::Hardware::LowLevel;

void Register::_ctor( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
	Get_address(pMngObj) = param0;
}

void Register::Write( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
	(*(volatile unsigned *)Get_address(pMngObj)) = param0;
}

UINT32 Register::Read( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return (*(volatile unsigned *)Get_address(pMngObj));
}

void Register::SetBits( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
	(*(volatile unsigned *)Get_address(pMngObj)) |= param0;
}

void Register::ClearBits( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
	(*(volatile unsigned *)Get_address(pMngObj)) &= ~param0;
}

void Register::ToggleBits( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
	(*(volatile unsigned *)Get_address(pMngObj)) ^= param0;
}


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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Portions Copyright (c) GHI Electronics, LLC.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "HAL.h"
#include "HAL_GHI_OSHW_Hardware_StorageDev.h"

#include <ThirdParty\GHI\Include\GHI_OSHW_HAL.h>

using namespace GHI::OSHW::Hardware;

void StorageDev::MountSDToSystem( HRESULT &hr )
{
//#ifdef FEZ_HYDRA
	hr = GHI_OSHW_Mount(-1); // Parameter is clock frequency in KHz; -1 represents default clock rate
//#endif
}

void StorageDev::MountSDToSystem( UINT32 param0, HRESULT &hr )
{
//#ifdef FEZ_HYDRA
	hr = GHI_OSHW_Mount(param0); // Parameter is clock frequency in KHz
//#endif
}

void StorageDev::UnmountSD( HRESULT &hr )
{
//#ifdef FEZ_HYDRA
	GHI_OSHW_Unmount();
//#endif
}


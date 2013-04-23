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


#ifndef _CERBERUS_HARDWARE_CERBERUS_HARDWARE_QUADRATUREENCODER_H_
#define _CERBERUS_HARDWARE_CERBERUS_HARDWARE_QUADRATUREENCODER_H_

namespace Cerberus
{
	namespace Hardware
	{
		struct QuadratureEncoder
		{
			// Declaration of stubs. These functions are implemented by Interop code developers
			static INT32 get_Count( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
			static void set_Count( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
			static INT8 get_InputCaptureEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
			static void set_InputCaptureEnabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr );
			static INT8 get_CountEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
			static void set_CountEnabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr );
		};
	}
}

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Cerberus_Hardware_QuadratureEncoder_Driver;

#endif  //_CERBERUS_HARDWARE_CERBERUS_HARDWARE_QUADRATUREENCODER_H_

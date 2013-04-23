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


#ifndef _HAL_GHI_OSHW_HARDWARE_STORAGEDEV_H_
#define _HAL_GHI_OSHW_HARDWARE_STORAGEDEV_H_

namespace GHI
{
    namespace OSHW
    {
        namespace Hardware
        {
            struct StorageDev
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static void MountSDToSystem( HRESULT &hr );
                static void MountSDToSystem( UINT32 param0, HRESULT &hr );
                static void UnmountSD( HRESULT &hr );
            };
        }
    }
}
#endif  //_HAL_GHI_OSHW_HARDWARE_STORAGEDEV_H_

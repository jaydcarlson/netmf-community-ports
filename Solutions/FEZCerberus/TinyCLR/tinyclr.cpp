////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

////////////////////////////////////////////////////////////////////////////////
void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;


    ClrStartup( clrSettings );

#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
#else
    ::CPU_Reset();
#endif
}

BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        GHI_VERSION_MAJOR /*VERSION_MAJOR*/, GHI_VERSION_MINOR /*VERSION_MINOR*/, GHI_VERSION_BUILD/*VERSION_BUILD*/, GHI_VERSION_REVISION/*VERSION_REVISION*/,
                        GHIOEMSTRING/*OEMSYSTEMINFOSTRING*/, hal_strlen_s(GHIOEMSTRING/*OEMSYSTEMINFOSTRING*/)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}


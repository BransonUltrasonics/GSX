/*-----------------------------------------------------------------------------
 * AtEmRasSrv.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              RAS-Server DLL for Windows XP
 *---------------------------------------------------------------------------*/

#if (defined WIN32)

/*-INCLUDES------------------------------------------------------------------*/
#include <EcOs.h>

#include <warn_dis.h>
#include <windows.h>
#include <warn_ena.h>

/*-GLOBALS-------------------------------------------------------------------*/
#ifdef INCLUDE_EC_INTERNAL_TSC_MEASUREMENT
extern "C" {
EC_T_TSC_MEAS_DESC G_TscMeasDesc;
EC_T_CHAR* G_aszMeasInfo[1] = 
{
    "NOTSUPPORTED"
};
}
#endif

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/

/*****************************************************************************/
/**
\brief  DLL Entry Point.

\return Always TRUE
*/
BOOL APIENTRY DllMain
(   HANDLE hModule,             /* unused Instance handle                    */
    DWORD  ul_reason_for_call,  /* call Reason                               */
    LPVOID lpReserved )         /* unused Parameter Pointer                  */
{
    EC_UNREFPARM(hModule);
    EC_UNREFPARM(lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        EC_T_OS_PARMS oOsInitDesc;
        OsMemset(&oOsInitDesc, 0, sizeof(EC_T_OS_PARMS));
        OsInit(&oOsInitDesc);
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#endif /* WIN32 */
 
/*-END OF SOURCE FILE--------------------------------------------------------*/

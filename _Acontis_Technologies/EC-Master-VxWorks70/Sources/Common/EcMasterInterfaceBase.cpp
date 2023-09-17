/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              Interface algorithms common to EcMaster and RAS
 *---------------------------------------------------------------------------*/

#include "AtEthercat.h"
#include "EcMasterInterfaceBase.h"

EC_T_DWORD CEcMasterInterfaceBase::RegisterClient(  
    EC_T_DWORD              dwInstanceID,  
    EC_PF_NOTIFY            pfnNotify,
    EC_T_VOID*              pCallerData,     
    EC_T_REGISTERRESULTS*   pRegResults)
{
    EC_T_IOCTLPARMS    oIoCtl       = {0};
    EC_T_REGISTERPARMS oReg         = {0};
    EC_T_DWORD         dwNumOutData = 0;

    /* wrap function parms in IoControl parms */
    oReg.pfnNotify       = pfnNotify;
    oReg.pCallerData     = pCallerData;
    oIoCtl.pbyInBuf      = (EC_T_BYTE*)&oReg;
    oIoCtl.dwInBufSize   = sizeof(EC_T_REGISTERPARMS);
    oIoCtl.pbyOutBuf     = (EC_T_BYTE*)pRegResults;
    oIoCtl.dwOutBufSize  = sizeof(EC_T_REGISTERRESULTS);
    oIoCtl.pdwNumOutData = &dwNumOutData;

    return IoControl(dwInstanceID, EC_IOCTL_REGISTERCLIENT, &oIoCtl);
}

EC_T_DWORD CEcMasterInterfaceBase::DeregisterClient(
    EC_T_DWORD dwInstanceID,
    EC_T_DWORD dwClntId)
{
    EC_T_IOCTLPARMS     oIoCtl          = {0};
    EC_T_DWORD          dwNumOutData    = 0;

    /* wrap function parms in IoControl parms */
    oIoCtl.dwInBufSize   = sizeof(EC_T_DWORD);
    oIoCtl.pbyInBuf      = (EC_T_BYTE*)&dwClntId;
    oIoCtl.pbyOutBuf     = EC_NULL;
    oIoCtl.dwOutBufSize  = 0;
    oIoCtl.pdwNumOutData = &dwNumOutData;
    
    return IoControl(dwInstanceID, EC_IOCTL_UNREGISTERCLIENT, &oIoCtl);
}


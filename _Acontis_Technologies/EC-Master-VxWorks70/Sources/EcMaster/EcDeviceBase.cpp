/*-----------------------------------------------------------------------------
 * EcDeviceBase.cpp         cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              implementation of the EtherCAT EcDeviceBase class.
 *---------------------------------------------------------------------------*/


/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include "EcHashTable.h"

#include <EthernetServices.h>
#include <EcLink.h>

#include <EcInterfaceCommon.h>
#include <EcInterface.h>
#include <EcDeviceBase.h>

#include <EcMasterCommon.h>
#include <EcEthSwitch.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcServices.h>
#include <EcDevice.h>

#include <EcMaster.h>

#if (defined INCLUDE_MASTER_OBD)
#include <EcSdoServ.h>
#endif

#include <AtEthercatPrivate.h>

/*-DEFINES-------------------------------------------------------------------*/
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x20)
#define RED_CHNG        ((EC_T_BYTE)0x00)

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg m_pMaster->m_poEcDevice->LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   m_pMaster->m_poEcDevice->LinkDbgMsg

/*-STATIC--------------------------------------------------------------------*/

/*-GLOBAL--------------------------------------------------------------------*/

/* trace masks */
extern "C" 
{
    EC_T_DWORD G_dwTraceMask       = 0;
    EC_T_DWORD G_dwMemTraceMask    = 0xffffffff;
}


/********************************************************************************/
/** \brief Set trace mask functions
*/
EC_T_DWORD ecatGetTraceMask(EC_T_VOID)               { return G_dwTraceMask; }
EC_T_DWORD ecatGetMemTraceMask(EC_T_VOID)            { return G_dwMemTraceMask; }
EC_T_VOID ecatSetTraceMask(EC_T_DWORD dwTraceMask)        { G_dwTraceMask    = dwTraceMask; }
EC_T_VOID ecatSetMemTraceMask(EC_T_DWORD dwMemTraceMask)  { G_dwMemTraceMask = dwMemTraceMask; }

#define MEMTRACE_SIZE   80000


EC_T_VOID EcMemLogAddMem(EC_T_MEMORY_LOGGER* pLog, 
#ifdef DEBUGTRACE
    EC_T_DWORD dwMask, 
    char* szLoc, EC_T_ADDRESS dwAddress, 
#endif
    size_t nSize)
{
    if (!pLog)
        return;

    pLog->nCurrUsage += nSize;
    if (pLog->nCurrUsage > pLog->nMaxUsage)
    {
        pLog->nMaxUsage = pLog->nCurrUsage;
    }

#ifdef DEBUGTRACE
    if ((dwMask & G_dwMemTraceMask) != 0)
    {
        /* Allocate tables for the first time */
        if (pLog->dwUsageCurrIndex == 0)
        {
            pLog->anUsage = EC_NEW(size_t[MEMTRACE_SIZE]);
            if (pLog->anUsage)
            {
                OsMemset(pLog->anUsage, 0, sizeof(size_t) * MEMTRACE_SIZE);
            }
            pLog->pMemTable = EC_NEW(CHashTableDyn)<EC_T_UINT64, size_t>(0, MEMTRACE_SIZE, EC_NULL);
        }

        OsTrcMsg("+MEM[%010ul]: %60s =%6d (addr = 0x%08x), curr/max = %6d/%6d\n", OsQueryMsecCount(), (szLoc), nSize, dwAddress, pLog->nCurrUsage, pLog->nMaxUsage);
        if (pLog->dwUsageCurrIndex < MEMTRACE_SIZE && pLog->anUsage && pLog->pMemTable)
        {
            CHashTableDyn<EC_T_UINT64, size_t>* pTable = (CHashTableDyn<EC_T_UINT64, size_t>*)pLog->pMemTable;
            pLog->anUsage[pLog->dwUsageCurrIndex] = pLog->nCurrUsage;
            pLog->dwUsageCurrIndex++;
            pTable->Add((EC_T_UINT64)dwAddress, nSize);
        }
        else
        {
            OsTrcMsg("memory trace array full!\n");
        }
    }
#endif
}

EC_T_VOID EcMemLogSubMem(EC_T_MEMORY_LOGGER* pLog, 
#ifdef DEBUGTRACE
    EC_T_DWORD dwMask, 
    char* szLoc, EC_T_ADDRESS dwAddress, 
#endif
    size_t nFreeSize)
{
    if (EC_NULL == pLog)
        return;

#ifdef DEBUGTRACE
    if (0 == dwAddress && 0 != nFreeSize)
        return;
#endif
    if(nFreeSize < pLog->nCurrUsage) 
    {
        pLog->nCurrUsage -= nFreeSize;
    }
    else
    {
        pLog->nCurrUsage = 0;
    }
#ifdef DEBUGTRACE
    if ((dwMask & G_dwMemTraceMask) != 0)
    {
        size_t nSize = (size_t)-1;
        if (dwAddress == 0 || !pLog || !pLog->pMemTable || !pLog->anUsage)
            return;

        CHashTableDyn<EC_T_UINT64, size_t>* pTable = (CHashTableDyn<EC_T_UINT64, size_t>*)pLog->pMemTable;
        if (pTable->Lookup((EC_T_UINT64)dwAddress, nSize))
        {
            if (pLog->dwUsageCurrIndex < MEMTRACE_SIZE)
            {
                pLog->anUsage[pLog->dwUsageCurrIndex] = pLog->nCurrUsage;
                pLog->dwUsageCurrIndex++;
            }

            pTable->Remove((EC_T_UINT64)dwAddress);

            OsTrcMsg("-MEM[%010ul]: %60s =%6d (addr = 0x%08x, curr/max = %6d/%6d\n", OsQueryMsecCount(), (szLoc), nSize, dwAddress, pLog->nCurrUsage, pLog->nMaxUsage);
            if (nFreeSize != nSize)
                OsTrcMsg("-MEM: %60s (Wrong size: addr = 0x%x, alloc size = %6d, freed size = %6d)\n", (szLoc), dwAddress, nSize, nFreeSize);
        }
        else
        {
            OsTrcMsg("-MEM: %60s (NOT FOUND: addr = 0x%x)\n", (szLoc), dwAddress);
        }
    }
#endif
}

EC_T_VOID EcMemLogFree(EC_T_MEMORY_LOGGER* pLog)
{
#ifdef DEBUGTRACE
    if (pLog->anUsage)
    {
        SafeDeleteArray(pLog->anUsage);
    }

    if (pLog->pMemTable)
    {
        CHashTableDyn<EC_T_UINT64, size_t>* pTable = (CHashTableDyn<EC_T_UINT64, size_t>*)pLog->pMemTable;
        {
            // report all not freed entries
            EC_T_UINT64 dwAddress; 
            size_t* pnSize = EC_NULL;
            EC_T_VOID*          pVoid = EC_NULL;
            while (pTable->GetNextEntry(dwAddress, pnSize, pVoid))
            {
                OsTrcMsg("-MEM_LEAK[%06ul]: %60s =%6d (addr = 0x%08x, curr/max = %6d/%6d\n", OsQueryMsecCount(), "NOT FREED", *pnSize, (EC_T_ADDRESS)dwAddress, pLog->nCurrUsage, pLog->nMaxUsage);
            }
        }

        SafeDelete(pTable);
        pLog->pMemTable = EC_NULL;
    }
#endif /* DEBUGTRACE */

    OsMemset(pLog, 0, sizeof(EC_T_MEMORY_LOGGER));
}


#ifdef FRAME_LOSS_SIMULATION
/*-FRAME LOSS SIMULATION----------------------------------------------------------------*/
CEcFrameLoss::CEcFrameLoss() 
{
    m_EFrameLossSimulationMode  = eFrameLossSim_OFF;
    m_dwNumGoodFramesAfterStart = 0;
    m_dwFrameLossLikelihoodPpm  = 0;
    m_dwFixedLossNumGoodFrames  = 0;
    m_dwFixedLossNumLostFrames  = 0;
    m_dwFrameCounter            = 0;
    m_dwLostFrameCounter        = 0;
}


CEcFrameLoss::~CEcFrameLoss()
{
}

/********************************************************************************/
/** \brief Set frame loss simulation parameters
*
* \return N/A
*/
EC_T_VOID CEcFrameLoss::SetFrameLossSimulation
(   EC_T_DWORD dwNumGoodFramesAfterStart,
    EC_T_DWORD dwFrameLossLikelihoodPpm,
    EC_T_DWORD dwFixedLossNumGoodFrames,
    EC_T_DWORD dwFixedLossNumLostFrames
)
{
    m_EFrameLossSimulationMode = eFrameLossSim_OFF;
    m_dwNumGoodFramesAfterStart = dwNumGoodFramesAfterStart;
    m_dwFrameLossLikelihoodPpm  = dwFrameLossLikelihoodPpm;
    m_dwFixedLossNumGoodFrames  = dwFixedLossNumGoodFrames;
    m_dwFixedLossNumLostFrames  = dwFixedLossNumLostFrames;
    if (dwFrameLossLikelihoodPpm > 0)
    {
        m_EFrameLossSimulationMode = eFrameLossSim_RANDOM;
    }
    if ((dwFixedLossNumLostFrames > 0) && (dwFixedLossNumGoodFrames > 0))
    {
        if (m_EFrameLossSimulationMode == eFrameLossSim_RANDOM)
        {
            m_EFrameLossSimulationMode = eFrameLossSim_MIXED;
        }
        else
        {
            m_EFrameLossSimulationMode = eFrameLossSim_FIXED;
        }
    }
    m_dwFrameCounter = 0;
    m_dwLostFrameCounter = 0;
}


/********************************************************************************/
/** \brief Check for frame loss
*
* \return N/A
*/
EC_T_BOOL CEcFrameLoss::IsThisFrameLost(EC_T_VOID)
{
EC_T_BOOL   bFrameLoss = EC_FALSE;

    if (m_EFrameLossSimulationMode != eFrameLossSim_OFF)
    {
        m_dwFrameCounter++;
        if (m_dwFrameCounter > m_dwNumGoodFramesAfterStart)
        {
        EC_T_UINT64 u64RandNumber;

            if ((m_EFrameLossSimulationMode == eFrameLossSim_RANDOM) || (m_EFrameLossSimulationMode == eFrameLossSim_MIXED))
            {
            EC_T_UINT64 u64Rand;
                u64Rand = (EC_T_UINT64)rand();
                u64RandNumber = (u64Rand * 1000000L) / (EC_T_UINT64)RAND_MAX;
                if (u64RandNumber < m_dwFrameLossLikelihoodPpm)
                {
                    bFrameLoss = EC_TRUE;
                }
            }
            if ((m_EFrameLossSimulationMode == eFrameLossSim_FIXED) || (m_EFrameLossSimulationMode == eFrameLossSim_MIXED))
            {
            EC_T_DWORD dwFrameCounterOffset;
                dwFrameCounterOffset = m_dwFrameCounter - m_dwNumGoodFramesAfterStart;
                dwFrameCounterOffset = dwFrameCounterOffset % (m_dwFixedLossNumGoodFrames + m_dwFixedLossNumLostFrames);
                if (dwFrameCounterOffset >= m_dwFixedLossNumGoodFrames)
                {
                    bFrameLoss = EC_TRUE;
                }
            }
            else
            {
                OsDbgAssert(m_EFrameLossSimulationMode==eFrameLossSim_RANDOM);
            }
            if (bFrameLoss)
            {
                m_dwLostFrameCounter++;
            }
        } /* m_dwFrameCounter > m_dwNumGoodFramesAfterStart */
    }
    return bFrameLoss;
}
#endif



/*-CONSTRUCTION/DESTRUCTION--------------------------------------------------*/
CEcDeviceBase::CEcDeviceBase(EC_T_BOOL bIsRedDevice)
{
#if (defined INCLUDE_RED_DEVICE)
    m_bIsRedDevice                  = bIsRedDevice;
#else
    EC_UNREFPARM(bIsRedDevice);
#endif
    m_bOpened                       = EC_FALSE;
    m_oLinkMacAddress               = NullEthernetAddress;
    m_ELinkStatus                   = eLinkStatus_UNDEFINED;
    m_ELinkStatusForced             = eLinkStatus_UNDEFINED;
    m_bLinkLayerFrameAllocSupported = EC_TRUE;
    m_bIsFramesTypeRequired         = EC_FALSE;
    m_bIsFlushFramesRequired        = EC_FALSE;
    m_eLinkMode                     = EcLinkMode_UNDEFINED;
    m_pvLinkInstance                = EC_NULL;
    m_pMaster                       = EC_NULL;
	
    OsMemset(&m_LinkDrvDesc, 0, sizeof(EC_T_LINK_DRV_DESC));

#if (defined INCLUDE_MASTER_OBD)
    m_pdwTxFrameCrt     = EC_NULL;
    m_pdwRxFrameCrt     = EC_NULL;
    m_pdwCycFrameCrt    = EC_NULL;
    m_pdwCycDgramCrt    = EC_NULL;
    m_pdwAcycFrameCrt   = EC_NULL;
    m_pdwAcycDgramCrt   = EC_NULL;
    m_pdwTxBytesSecondCrt  = EC_NULL;
    m_pdwTxBytesCycleCrt   = EC_NULL;
#endif

    m_bReceiveEnabled   = EC_TRUE;
    m_bSendEnabled      = EC_TRUE;

    /* initialization for completeness */
    m_wRes = 0;
}


CEcDeviceBase::~CEcDeviceBase()
{
}


/*-IECDEVICE-----------------------------------------------------------------*/


/********************************************************************************/
/** \brief Opens the selected network adapter
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcDeviceBase::OpenDeviceBase
(EC_T_VOID*                  pParms,                 /* [in]  link parameters */
 EC_T_VOID*                  pvContext,              /* [in]  context for link layer callbacks */
 EC_T_RECEIVEFRAMECALLBACK   pfReceiveFrameCallback  /* [in]  pointer to rx callback function */
)
{
EC_T_DWORD          dwRetVal        = EC_E_ERROR;
EC_T_DWORD          dwRes           = EC_E_ERROR;
EC_T_LINK_DRV_DESC* pRestoreParms   = &((EC_T_LINK_DEV_RESTORE_PARAM*)pParms)->oLinkDrv;
EC_T_LINK_PARMS*    pLinkParms      = (EC_T_LINK_PARMS*)pParms;
EC_PF_LLREGISTER    pfEmllRegister;
EC_T_BOOL           bLinkOpened     = EC_FALSE;
ETHERNET_ADDRESS    oEthernetAddress;
EC_T_LINK_IOCTLPARMS oLinkIoCtlParms = {0};

    /* check device state */
    if (m_bOpened)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* check if master exist */
    if (m_pMaster == EC_NULL)
    {
        dwRetVal = EC_E_OPENFAILED;
        goto Exit;
    }
   
    /* check if this is the ecatConfigureMaster() call */
    if (LINK_LAYER_DRV_DESC_PATTERN == pRestoreParms->dwValidationPattern)
    {
        /* Link layer is already opened, reuse the stored values */
        OsMemcpy(&m_LinkDrvDesc, pRestoreParms, sizeof(EC_T_LINK_DRV_DESC));
        OsMemcpy(m_szDriverIdent, ((EC_T_LINK_DEV_RESTORE_PARAM*)pParms)->szDriverIdent , MAX_DRIVER_IDENT_LEN);
        m_pvLinkInstance = m_LinkDrvDesc.pvLinkInstance;

        if (pfReceiveFrameCallback != EC_NULL)
        {
            EC_T_LINK_FRM_RECV_CLBK oFrmRecvClbk = {0};
            oFrmRecvClbk.pfFrameReceiveCallback = pfReceiveFrameCallback;
            oFrmRecvClbk.pvDevice = this; 

            OsMemset(&oLinkIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));
            oLinkIoCtlParms.dwInBufSize = sizeof(EC_T_LINK_FRM_RECV_CLBK);
            oLinkIoCtlParms.pbyInBuf    = (EC_T_PBYTE)&oFrmRecvClbk;
            LinkIoctl(EC_LINKIOCTL_REGISTER_FRAME_CLBK, &oLinkIoCtlParms);
        }
    }
    else 
    {
        if (pLinkParms == EC_NULL)
        {
            EC_DBGMSG( "CEcDeviceBase::OpenDeviceBase: missing device parameter \n");
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        if (EC_LINK_PARMS_SIGNATURE_PATTERN != (pLinkParms->dwSignature & 0xFFF00000))
        {
            EC_DBGMSG( "CEcDeviceBase::OpenDeviceBase: invalid device parameter pattern 0x%x instead of 0x%x\n", (pLinkParms->dwSignature & 0xFFF00000), EC_LINK_PARMS_SIGNATURE_PATTERN );
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        if (EC_LINK_PARMS_SIGNATURE_VERSION != (pLinkParms->dwSignature & 0x000F0000))
        {
            EC_DBGMSG( "CEcDeviceBase::OpenDeviceBase: invalid device parameter version 0x%x instead of 0x%x\n", (pLinkParms->dwSignature & 0x000F0000), EC_LINK_PARMS_SIGNATURE_VERSION );
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        }
        pfEmllRegister = OsGetLinkLayerRegFunc(pLinkParms->szDriverIdent);

        if (pfEmllRegister == EC_NULL)
        {
            EC_DBGMSG( "CEcDeviceBase::OpenDeviceBase: cannot find registration function for link layer '%s'\n", pLinkParms->szDriverIdent );
            dwRetVal = EC_E_OPENFAILED;
            goto Exit;
        }
        OsMemcpy(m_szDriverIdent, pLinkParms->szDriverIdent, MAX_DRIVER_IDENT_LEN);

        m_LinkDrvDesc.dwValidationPattern   = LINK_LAYER_DRV_DESC_PATTERN;
        m_LinkDrvDesc.dwInterfaceVersion    = LINK_LAYER_DRV_DESC_VERSION;
        m_LinkDrvDesc.pfOsDbgMsgHook        = LLDbgMsgHookVaArgs;
        dwRes = (*pfEmllRegister)( &m_LinkDrvDesc, sizeof(m_LinkDrvDesc) );
        
        if (dwRes != EC_E_NOERROR)
        {
            EC_ERRORMSGC(("CEcDeviceBase::Open() Failed to register link layer driver '%s'!\n", pLinkParms->szDriverIdent));
            dwRetVal = dwRes;
            goto Exit;
        }
        
        dwRes = m_LinkDrvDesc.pfEcLinkOpen(pLinkParms, pfReceiveFrameCallback, EC_NULL, pvContext, &m_pvLinkInstance);
        if (dwRes != EC_E_NOERROR)
        {
            EC_ERRORMSGC(("CEcDeviceBase::Open() Failed to open link layer!\n"));
            dwRetVal = dwRes;
            goto Exit;
        }
        m_LinkDrvDesc.pvLinkInstance = m_pvLinkInstance;
    }
    bLinkOpened = EC_TRUE;
    
    /* get Ethernet address */
    dwRes = m_LinkDrvDesc.pfEcLinkGetEthernetAddress(m_pvLinkInstance, (EC_T_BYTE*)&oEthernetAddress);
    if (dwRes == EC_E_NOERROR)
    {
        m_oLinkMacAddress = oEthernetAddress;
    }
    /* get link mode */
    m_eLinkMode  = m_LinkDrvDesc.pfEcLinkGetMode(m_pvLinkInstance);
    m_ELinkStatus = m_LinkDrvDesc.pfEcLinkGetStatus(m_pvLinkInstance);

    /* get IsFramesTypeRequired, IsFlushFramesRequired, disregarding return code because IOCTL is optional for link layers */
    OsMemset(&oLinkIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));
    oLinkIoCtlParms.dwOutBufSize = sizeof(EC_T_BOOL);
    oLinkIoCtlParms.pbyOutBuf    = (EC_T_PBYTE)&m_bIsFramesTypeRequired;
    LinkIoctl(EC_LINKIOCTL_IS_FRAMETYPE_REQUIRED, &oLinkIoCtlParms);
    oLinkIoCtlParms.pbyOutBuf    = (EC_T_PBYTE)&m_bIsFlushFramesRequired;
    LinkIoctl(EC_LINKIOCTL_IS_FLUSHFRAMES_REQUIRED, &oLinkIoCtlParms);

#if (defined INCLUDE_MASTER_OBD)
    m_pMaster->SDOSetLinkLayerParms(m_pMaster->GetInitMasterParms()->pLinkParms);
    m_pMaster->SDOSetLinkLayerPolling((EcLinkMode_POLLING == m_eLinkMode));
#if (defined INCLUDE_RED_DEVICE)
    m_pMaster->SDOSetMacAddress(m_bIsRedDevice, (oEthernetAddress.b));
#else
    m_pMaster->SDOSetMacAddress(EC_FALSE, (oEthernetAddress.b));
#endif
    m_pMaster->SDOSetCfgMacAddress();
    m_pMaster->m_pSDOService->EntrySet_SetLinkUp(IsLinkConnected());
#endif

    m_bOpened = EC_TRUE;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* cleanup */
    if (dwRetVal != EC_E_NOERROR)
    {
        if( bLinkOpened )
        {
            m_LinkDrvDesc.pfEcLinkClose(m_pvLinkInstance);
        }
    }
    
    return dwRetVal;
}   


/********************************************************************************/
/** \brief Closes the selected network adapter
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcDeviceBase::CloseDeviceBase(EC_T_BOOL bDeinitForConfiguration)
{
EC_T_DWORD           dwRetVal = EC_E_ERROR;

    /* check state */
    if (!m_bOpened)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
	/* close link
       see also "set the validation pattern to 0 avoid the de-initialisation of the LinkLayer" */
    if ((LINK_LAYER_DRV_DESC_PATTERN == m_LinkDrvDesc.dwValidationPattern) && (!bDeinitForConfiguration))
    {
        dwRetVal = m_LinkDrvDesc.pfEcLinkClose(m_pvLinkInstance);
    
        if (dwRetVal != EC_E_NOERROR)
        {
            EC_ERRORMSG(("EcLinkCloseCEcDevice::Close() Failed to close link layer!\n"));
        }
    }
    else
    {
        LinkIoctl(EC_LINKIOCTL_UNREGISTER_FRAME_CLBK, EC_NULL);
    }
    m_bOpened = EC_FALSE;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (!bDeinitForConfiguration)
    {
        OsReleaseLinkLayerRegFunc(m_szDriverIdent);
    }
    return dwRetVal;
}

/*-----------------------------------------------------------------*/

/********************************************************************************/
/** \brief Handle the polling of the link layer if needed
*
* \return Result of the call.
*/
EC_T_DWORD CEcDeviceBase::HandleLinkPolling(EC_T_VOID)
{
    EC_T_BOOL bFrameProcessed = EC_FALSE;
    EC_T_LINK_FRAMEDESC oLinkframeDesc;
    oLinkframeDesc.pfnTimeStamp = EC_NULL;

    OsDbgAssert(EcLinkMode_POLLING == m_eLinkMode);
    
    do
    {
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkRecvFrame);
        RecvFrame(&oLinkframeDesc);
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkRecvFrame);

        if (oLinkframeDesc.dwSize != 0)
        {
            ReceiveFrameCallback(&oLinkframeDesc, &bFrameProcessed);

            /* release the received frame */
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkFreeRecvFrame);
            FreeRecvFrame(&oLinkframeDesc);
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkFreeRecvFrame);
        }
    }
    while (0 != oLinkframeDesc.dwSize);

    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief Flush all received frames
*
* \return EC_E_NOERROR
*/
EC_T_DWORD CEcDeviceBase::DeviceFlushRecvFrames(EC_T_VOID)
{
EC_T_LINK_FRAMEDESC oLinkframeDesc;
    OsMemset(&oLinkframeDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));
    do
    {
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkRecvFrame);
        m_LinkDrvDesc.pfEcLinkRecvFrame(m_pvLinkInstance, &oLinkframeDesc);
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkRecvFrame);
        if (oLinkframeDesc.dwSize != 0)
        {
            /* release the received frame and do not process it */
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkFreeRecvFrame);
            FreeRecvFrame(&oLinkframeDesc);
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkFreeRecvFrame);
        }
    } 
    while (0 != oLinkframeDesc.dwSize);
    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief Sends a frame to the network adapter.
*
* Note: The frame will always be freed if it is allocated by the link layer.
*       If the frame is not allocated by the link layer it will only be freed if it 
*       is a acyclic frame because cyclic frames are pre-allocated in the master.
*                                                                    
* \return 
*/
EC_T_DWORD CEcDeviceBase::FrameSendAndFree(   
    EC_T_LINK_FRAMEDESC*  pLinkFrameDesc,    /* [in]  frame descriptor */ 
    EC_T_FRAME_TYPE       EFrameType,        /* [in]  cyclic or acyclic frame */
#if (defined INCLUDE_MASTER_OBD)
    EC_T_DWORD            dwNumCmdsInFrame,  /* [in]  number of commands in frame for statistic crts */
#endif
    EC_T_BOOL             bIsDbgFrame        /* [in]  EC_TRUE if this is a debug frame */
                                          )
{
EC_T_DWORD      dwRes             = EC_E_NOERROR;
EC_T_DWORD      dwCount = 0;
EC_T_BOOL       bFrameLoss = EC_FALSE;

    if (!bIsDbgFrame && !m_bSendEnabled) 
    {
        bFrameLoss = EC_TRUE;
    }

#ifdef FRAME_LOSS_SIMULATION
    if( !bIsDbgFrame && (m_oTxFrameLoss.m_EFrameLossSimulationMode != eFrameLossSim_OFF) )
    {
        if( m_oTxFrameLoss.IsThisFrameLost() )
        {
            bFrameLoss = EC_TRUE;
        }
    }
#else
    EC_UNREFPARM(bIsDbgFrame);
#endif
            
    /* send the frame */
    if (m_bLinkLayerFrameAllocSupported)
    {
#ifdef FRAME_LOSS_SIMULATION
        /* link layer allocated frame: frame has to be freed by link-layer */
        if (bFrameLoss)
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkFreeSendFrame);
            m_LinkDrvDesc.pfEcLinkFreeSendFrame(m_pvLinkInstance, pLinkFrameDesc);
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkFreeSendFrame);
        }
        else
#endif
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkSendAndFreeFrame);
            dwRes = m_LinkDrvDesc.pfEcLinkSendAndFreeFrame(m_pvLinkInstance, pLinkFrameDesc);
            if (EFrameType == eFrameType_ACYCLIC)
            {
                dwCount = 0;
                while ((EC_E_SENDFAILED == dwRes) && (dwCount < 2000))
                {
                    dwRes = m_LinkDrvDesc.pfEcLinkSendAndFreeFrame(m_pvLinkInstance, pLinkFrameDesc);
                    dwCount++;
                }
                OsDbgAssert(EC_E_NOERROR == dwRes);
            }
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkSendAndFreeFrame);
        }
    }
    else
    {
        if (!bFrameLoss)
        {
            PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkSendFrame);
            dwRes = m_LinkDrvDesc.pfEcLinkSendFrame(m_pvLinkInstance, pLinkFrameDesc);
            PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkSendFrame);
        }
    }
#if (defined INCLUDE_MASTER_OBD)
    if( (EC_E_NOERROR == dwRes) && (EC_NULL != m_pdwTxFrameCrt))
    {
        (*m_pdwTxFrameCrt) = ((*m_pdwTxFrameCrt)+1);

        /* count number of transmitted bytes */
        (*m_pdwTxBytesSecondCrt) = ((*m_pdwTxBytesSecondCrt)    +  pLinkFrameDesc->dwSize);
        (*m_pdwTxBytesCycleCrt)  = ((*m_pdwTxBytesCycleCrt)  +  pLinkFrameDesc->dwSize);
    }
#endif

#if (defined INCLUDE_MASTER_OBD)
    if( EC_E_NOERROR == dwRes  )
    {
        switch( EFrameType )
        {
        case eFrameType_CYCLIC:
            {
                if( EC_NULL != m_pdwCycFrameCrt )
                {
                    (*m_pdwCycFrameCrt) = ((*m_pdwCycFrameCrt)+1);
                }

                if( EC_NULL != m_pdwCycDgramCrt )
                {
                    (*m_pdwCycDgramCrt) = ((*m_pdwCycDgramCrt)+dwNumCmdsInFrame);
                }
            } break;
        case eFrameType_ACYCLIC:
            {
                if( EC_NULL != m_pdwAcycFrameCrt )
                {
                    (*m_pdwAcycFrameCrt) = ((*m_pdwAcycFrameCrt)+1);
                }
                if( EC_NULL != m_pdwAcycDgramCrt )
                {
                    (*m_pdwAcycDgramCrt) = ((*m_pdwAcycDgramCrt)+dwNumCmdsInFrame);
                }
            } break;
        case eFrameType_DEBUG:
        case eFrameType_UNKNOWN:
        default:
            {
                /* don't count */
            } break;
        }
    }
#else
    EC_UNREFPARM(EFrameType);
#endif

    return dwRes;
}


/********************************************************************************/
/** \brief Allocates memory for an Ethernet frame.
*  
* If the link layer supports frame allocation the frame buffer will always be allocated by the
* link layer without regard to the frame allocation type.
* If the link layer does not support frame allocation the frame buffer will only be locally allocated
* in case of dynamic frames.
*
* \return 
*/
EC_T_DWORD CEcDeviceBase::FrameAlloc
(   EC_T_LINK_FRAMEDESC* pLinkFrameDesc,    /* [out]  descriptor containing allocated frame */
    EC_T_DWORD dwSize                       /* [in]   frame size */
)
{
EC_T_DWORD  dwRes    = EC_E_NOERROR;
EC_T_DWORD  dwRetVal = EC_E_NOERROR;

    /* reset frame descriptor */
    OsMemset(pLinkFrameDesc, 0, sizeof(EC_T_LINK_FRAMEDESC));

    if (m_bLinkLayerFrameAllocSupported)
    {
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkAllocSendFrame);
        dwRes = m_LinkDrvDesc.pfEcLinkAllocSendFrame(m_pvLinkInstance, pLinkFrameDesc, dwSize);
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkAllocSendFrame);

        switch (dwRes)
        {
        case EC_E_NOERROR:
            /* nothing to do */
            break;
        case EC_E_NOMEMORY:
            EC_ERRORMSGC(("FrameAlloc::EcLinkAllocSendFrame() supported but reports no memory!!\n"));
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        case EC_E_NOTSUPPORTED:
            /* remember for further calls to save cpu time */
            m_bLinkLayerFrameAllocSupported = EC_FALSE;
#if (defined INCLUDE_MASTER_OBD)
            m_pMaster->SDOSetLinkLayerAllocSupport(EC_FALSE);
#endif
            break;
        case EC_E_INVALIDSTATE:
            dwRetVal = dwRes;
            goto Exit;
        default:
            OsDbgAssert(EC_FALSE);
            dwRetVal = dwRes;
            goto Exit;
        }
    }

    /* finalize frame descriptor */
    pLinkFrameDesc->dwSize = dwSize;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Sets the MAC address of the EtherCAT master.
*
* \return 
*/
EC_T_VOID CEcDeviceBase::SetMacAddress(ETHERNET_ADDRESS macAddress)
{
    m_oLinkMacAddress = macAddress;
}

EC_T_BOOL CEcDeviceBase::LLDbgMsgHook(const EC_T_CHAR* szFormat, ...)
{
    EC_T_VALIST vaList;
    EC_VASTART(vaList, szFormat);

    LLDbgMsgHookVaArgs(szFormat, vaList);

    EC_VAEND(vaList);
    return EC_FALSE;
}

/***************************************************************************************************/
/**
\brief  Hook for dbg messages from linklayer to application logging.

\return EC_FALSE to suppress screen prints
*/
EC_T_BOOL CEcDeviceBase::LLDbgMsgHookVaArgs(
    const 
    EC_T_CHAR*  szFormat, 
    EC_T_VALIST vaArgs
                                     )
{
    EC_PF_OSDBGMSGHK    pfDbgMsg = EC_NULL;
    
    pfDbgMsg = OsGetDbgMsgHook();

    if( EC_NULL != pfDbgMsg )
        return pfDbgMsg(szFormat, vaArgs);
    else
        return EC_TRUE;
}

/********************************************************************************/
/** \brief Update the link status of the network connection.
*
* \return link status
*/
EC_T_LINKSTATUS CEcDeviceBase::RefreshLinkStatus(EC_T_VOID)
{
    if (IsLinkStatusForced())
    {
        m_ELinkStatus = m_ELinkStatusForced;
    }
    else
    {
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_LinkGetStatus);
        m_ELinkStatus = m_LinkDrvDesc.pfEcLinkGetStatus(m_pvLinkInstance);
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_LinkGetStatus);
    }
    return m_ELinkStatus;
}

/********************************************************************************/
/** \brief Force link status of the network connection.
*
* The next call to RefreshLinkStatus() will return the forced value
*
* \return N/A
*/
EC_T_VOID CEcDeviceBase::ForceLinkStatus(EC_T_LINKSTATUS eLinkStatus)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+RED_CHNG), "CEcDeviceBase::ForceLinkStatus(): Force link status to %d\n", eLinkStatus));

    m_ELinkStatusForced = eLinkStatus;
}


/*-END OF SOURCE FILE--------------------------------------------------------*/

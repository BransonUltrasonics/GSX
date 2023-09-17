/*-----------------------------------------------------------------------------
 * EcDevice.cpp             cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              implementation of the EtherCATDev class.
 *---------------------------------------------------------------------------*/


/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include <EcMasterCommon.h>
#include <EcInterfaceCommon.h>
#include <EcLink.h>
#include <EcInterface.h>
#include <EcEthSwitch.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcServices.h>
#include <EcDevice.h>
#include <EcMaster.h>

#include <EcBusTopology.h>
#if (defined INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif
#if (defined INCLUDE_RED_DEVICE)
#include <EcRedDevice.h>
#endif

#if (defined ECWIN)
#define VMF_COMM_JOB_LICENSE_REPORT        99
#define VMF_COMM_JOB_LICENSE_QUERY         136
#if defined _CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#endif
#include <vmfInterface.h>
#if (defined ECWIN_CE)
#pragma comment(lib, "vmfInterfaceUserMode.lib")
#endif
#endif
/*-DEFINES-------------------------------------------------------------------*/
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x20)
#define RED_CHNG        ((EC_T_BYTE)0x00)

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   LinkDbgMsg

//#define LINK_DEBUG
#ifdef LINK_DEBUG
#define LINK_DBG_TRACE(trc,msg) LinkDbgMsg(2, 0xF8, (trc), (msg))
#else
#define LINK_DBG_TRACE(trc,msg)
#endif

/*-STATIC--------------------------------------------------------------------*/

/*-GLOBAL--------------------------------------------------------------------*/

#define MAX_DBGMSG_LEN    256

/*-CONSTRUCTION/DESTRUCTION--------------------------------------------------*/
CEcDevice::CEcDevice(struct _EC_DEVICE_CTOR_PARMS* pParms)
: CEcDeviceBase(EC_FALSE)
, m_pMLog(pParms->pMLog)
{
    EC_T_MASTER_CONFIG* pMasterConfig = pParms->pMasterConfig;
    m_pIOImage                   = EC_NULL;

    m_poFrameDescLock            = OsCreateLockTyped(eLockType_SPIN);
#ifdef DEBUGTRACE
    m_pDeviceFrameDescFifo = EC_NEW(CFiFoListDyn<ECAT_SLAVEFRAME_DESC*>(pMasterConfig->dwMaxQueuedEthFrames, m_poFrameDescLock, (EC_T_CHAR*)"FifoE88A4", EC_MEMTRACE_CORE_DEVICE));
#else
    m_pDeviceFrameDescFifo = EC_NEW(CFiFoListDyn<ECAT_SLAVEFRAME_DESC*>(pMasterConfig->dwMaxQueuedEthFrames, m_poFrameDescLock, (EC_T_CHAR*)"FifoE88A4"));
#endif
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::m_pDeviceFrameDescFifo", m_pDeviceFrameDescFifo, sizeof(CFiFoListDyn<ECAT_SLAVEFRAME_DESC*>));

    m_pbyDbgMsg                  = (EC_T_BYTE*)OsMalloc(ETHERNET_FRAME_LEN + MAX_DBGMSG_LEN);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::m_pbyDbgMsg", m_pbyDbgMsg, ETHERNET_FRAME_LEN + MAX_DBGMSG_LEN);

    m_ELinkStatusNotified        = eLinkStatus_UNDEFINED;

    #if (defined INCLUDE_RED_DEVICE)
    m_bRedConfigured             = EC_FALSE;
    m_bRedDeviceEnabled          = EC_FALSE;
    m_pRedDevice                 = EC_NULL;
    m_byRedFrameId               = 0;
    OsMemset(&m_scMainLineUnproc, 0, sizeof(m_scMainLineUnproc));
    m_dwMainFrameLossCnt         = 0;
    m_bMainFrameLoss             = EC_FALSE;
    OsMemset(&m_scRedLineUnproc,  0, sizeof(m_scRedLineUnproc));
    m_dwRedFrameLossCnt          = 0;
    m_bRedFrameLoss              = EC_FALSE;
    m_bLinkConnected             = EC_TRUE;
    m_scMainLineUnproc.pbyFrames = EC_NULL;
    m_scRedLineUnproc.pbyFrames  = EC_NULL;
#endif
#if (defined INCLUDE_EOE_SUPPORT)
    m_pSwitch                    = EC_NULL;
#endif
    m_bTimeRunning = EC_FALSE;
    m_tTimeout.Stop();

    m_bInvalidLicense = EC_FALSE;
    m_bChecked        = EC_FALSE;
#if (defined ECWIN)
    m_dwEcWinJobDone = EC_FALSE;  
	m_bEcWinChecked  = EC_FALSE;   
    m_dwEcWinTime    = 1000 * 33; 
#endif
    m_dwCheckCounter  = 0;
#if (defined EVAL_VERSION)
    m_bInvalidLicense = EC_TRUE;
#endif
}

CEcDevice::~CEcDevice()
{
    CloseDevice(EC_FALSE);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::~m_pbyDbgMsg", m_pbyDbgMsg, ETHERNET_FRAME_LEN + MAX_DBGMSG_LEN);
    SafeOsFree(m_pbyDbgMsg);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::~m_pDeviceFrameDescFifo", m_pDeviceFrameDescFifo, sizeof(CFiFoListDyn<ECAT_SLAVEFRAME_DESC*>));
    SafeDelete(m_pDeviceFrameDescFifo);
    OsDeleteLock(m_poFrameDescLock);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::~m_pMaster", m_pMaster, sizeof(CEcMaster));
    SafeDelete(m_pMaster);
    CEcIoImage::DestroyInstance(m_pIOImage, GetMemLog());
#if (defined INCLUDE_EOE_SUPPORT)
    if (EC_NULL != m_pSwitch)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::~m_pSwitch", m_pSwitch, sizeof(CEcEthSwitch));
        SafeDelete(m_pSwitch);
    }
#endif
#if (defined INCLUDE_RED_DEVICE)    
    if (EC_NULL != m_scMainLineUnproc.pbyFrames)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::~m_scMainLineUnproc.pbyFrames", m_scMainLineUnproc.pbyFrames, MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN);
        SafeDeleteArray(m_scMainLineUnproc.pbyFrames);
    }

    if (EC_NULL != m_scRedLineUnproc.pbyFrames)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::~m_scRedLineUnproc.pbyFrames", m_scRedLineUnproc.pbyFrames, MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN);
        SafeDeleteArray(m_scRedLineUnproc.pbyFrames);
    }
#endif

}


/*-IECDEVICE-----------------------------------------------------------------*/


/********************************************************************************/
/** \brief Opens the selected network adapter
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcDevice::OpenDevice(EC_T_VOID* pParms)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_DWORD dwRes       = EC_E_ERROR;

    /* call base class first */
    dwRes = OpenDeviceBase(pParms, this, ReceiveFrameCallbackGateway);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* prepare the master */
    m_oLinkMacAddress.b[0] &= ~0x2;
    m_pMaster->SetSrcMacAddress(m_oLinkMacAddress);
    m_bOpened = EC_TRUE;

#if (defined EVAL_VERSION)
    if (!m_bTimeRunning)
    {
        m_bTimeRunning = EC_TRUE;
        m_tTimeout.Stop();
#if (defined ECWIN)
    m_tTimeout.Start(m_dwEcWinTime, m_pMaster->GetMsecCounterPtr());
#else
    m_tTimeout.Start(1000 * m_pMaster->GetTimeLimit(), m_pMaster->GetMsecCounterPtr());
#endif
    }
#endif
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}   


/********************************************************************************/
/** \brief Closes the selected network adapter
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcDevice::CloseDevice(EC_T_BOOL bDeinitForConfiguration)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* check state */
    if( !m_bOpened )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if( m_bTimeRunning )
    {
        m_tTimeout.Stop();
        m_bTimeRunning = EC_FALSE;
    }

    /* call base class last */
    dwRetVal = CloseDeviceBase(bDeinitForConfiguration);
    if (dwRetVal != EC_E_NOERROR)
    {
        goto Exit;
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


#if (defined INCLUDE_RED_DEVICE)
/********************************************************************************/
/** \brief Enable the Redundancy interface
*
* \return EC_E_NOERROR or error code
*/
EC_T_DWORD CEcDevice::AddRedDevice(CEcRedDevice* poEcRedDevice)
{
EC_T_DWORD             dwRetVal = EC_E_NOERROR;

    OsDbgAssert(m_pRedDevice==EC_NULL);

    m_pRedDevice = poEcRedDevice;

    m_bRedDeviceEnabled = EC_TRUE;

    m_scMainLineUnproc.pbyFrames = EC_NEW(EC_T_BYTE[MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::m_scMainLineUnproc.pbyFrames", m_scMainLineUnproc.pbyFrames, MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN);
    if(m_scMainLineUnproc.pbyFrames != EC_NULL)
    {
        OsMemset(m_scMainLineUnproc.pbyFrames, 0, MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN);
    }
    else
    {
        dwRetVal = EC_E_NOMEMORY;
    }

    m_scRedLineUnproc.pbyFrames  = EC_NEW(EC_T_BYTE[MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN]);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::m_scRedLineUnproc.pbyFrames", m_scRedLineUnproc.pbyFrames, MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN);
    if(m_scRedLineUnproc.pbyFrames != EC_NULL)
    {
        OsMemset(m_scRedLineUnproc.pbyFrames, 0, MAX_UNPROC_FRAMES * ETHERNET_MAX_FRAMEBUF_LEN);
    }
    else
    {
        dwRetVal = EC_E_NOMEMORY;
    }

    return dwRetVal;
}
#endif


/********************************************************************************/
/** \brief Gets a pointer to the process data.
*
* \return EC_NULL or pointer to the desired area
*/
EC_T_BYTE* CEcDevice::ProcessDataPtr(EC_T_INT nInOut, EC_T_INT nOffs, EC_T_INT size)
{
EC_T_BYTE* pbyRetVal = EC_NULL;

    switch (nInOut)
    {
        case VG_IN:
        {
            {
                /* check process data */
                if (m_pIOImage == EC_NULL)
                {
                    goto Exit;
                }
                /* get pointer to input process data */
                pbyRetVal = m_pIOImage->GetInputPtr(nOffs, size);
            }
        } break;
        case VG_OUT:
        {
            /* check process data */
            if (m_pIOImage == EC_NULL)
            {
                goto Exit;
            }
            /* get pointer to output process data */
            pbyRetVal = m_pIOImage->GetOutputPtr(nOffs, size);
        } break;
        default:
            goto Exit;
    }
Exit:
    return pbyRetVal;
}


/********************************************************************************/
/** \brief Gets the size of the process data.
*
* \return -1 or GetImage()->GetOutputSize()
*/

EC_T_INT CEcDevice::ProcessDataSize(EC_T_INT nInOut)
{
    if ((m_pIOImage != EC_NULL) && nInOut == VG_IN)
    {
        return m_pIOImage->GetInputSize();
    }
    else if ((m_pIOImage != EC_NULL) && nInOut == VG_OUT)
    {
        return m_pIOImage->GetOutputSize();
    }
    else
    {
        return -1;
    }
}

/*-IEthDevice----------------------------------------------------*/


/********************************************************************************/
/** \brief Creates an EtherCAT device frame descriptor.
*
* Details:
* - link layer supports send frame allocation: 
*   ==> poLinkFrameDesc->pbyFrame allocated via link layer function EcLinkAllocSendFrame()
*   ==> OsMemcpy (poLinkFrameDesc->pbyFrame, pbyFrame, dwSize);  
* - link layer does not support send frame allocation:  
*   ==> use the pre-allocated frame buffer to save time
*   ==> poLinkFrameDesc->pbyFrame = pbyFrame;  
*
* \return status value.
*/
EC_T_DWORD CEcDevice::DeviceFrameDescCreate
(   EC_T_DEVICE_FRAMEDESC* pDeviceFrameDesc,    /* [out]  allocate frame for the EtherCAT device */
    EC_T_BOOL  bCopyFrame,                      /* [in] if EC_TRUE, copy pbyFrame into newly allocated frame */
    EC_T_BYTE* pbyFrame,                        /* [in]   pointer to frame data buffer */
#ifdef INCLUDE_RED_DEVICE
    EC_T_BYTE* pbyRedFrame,                     /* [in]   pointer to frame data buffer */
#endif
    EC_T_DWORD dwSize                           /* [in]   size of frame (number of byte in frame data buffer) */
) 
{ 
EC_T_DWORD dwRetVal = EC_E_NOERROR;
EC_T_DWORD dwRes    = EC_E_NOERROR;

    /* first allocate frame descriptor(s) */
    dwRes = FrameAlloc(&pDeviceFrameDesc->oLinkFrameDesc, dwSize);
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = dwRes;
        goto Exit;
    }
#ifdef INCLUDE_RED_DEVICE
    if (m_bRedDeviceEnabled)
    {
        dwRes = m_pRedDevice->FrameAlloc(&pDeviceFrameDesc->oRedLinkFrameDesc, dwSize);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
#endif
    /* reset commands counter */
    pDeviceFrameDesc->dwNumCmdsInFrame = 0;

    if (EC_NULL == pDeviceFrameDesc->oLinkFrameDesc.pbyFrame)
    {
        pDeviceFrameDesc->oLinkFrameDesc.pbyFrame = pbyFrame;
    }
    else
    {
        if (bCopyFrame)
        {
            /* round up to WORD boarder */
            EC_T_WORD wCopySize = (EC_T_WORD)dwSize;
            if ((wCopySize % sizeof(EC_T_DWORD)) != 0)
            {
                wCopySize = (EC_T_WORD)((dwSize / sizeof(EC_T_DWORD) + 1) * sizeof(EC_T_DWORD));
            }
            OsMemcpy(pDeviceFrameDesc->oLinkFrameDesc.pbyFrame, pbyFrame, wCopySize);
        }

    }
    pDeviceFrameDesc->oLinkFrameDesc.dwSize = dwSize;

#ifdef INCLUDE_RED_DEVICE
    if (m_bRedDeviceEnabled)
    {
        if (EC_NULL == pDeviceFrameDesc->oRedLinkFrameDesc.pbyFrame)
        {
            pDeviceFrameDesc->oRedLinkFrameDesc.pbyFrame = pbyRedFrame;
        }
        pDeviceFrameDesc->oRedLinkFrameDesc.dwSize = dwSize;
    }
#endif

Exit:
    return dwRetVal;
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
EC_T_DWORD CEcDevice::DeviceFrameSendAndFree
(   EC_T_DEVICE_FRAMEDESC*  pDeviceFrameDesc   /* [in]  device frame descriptor */ 
)
{
EC_T_DWORD dwRetVal        = EC_E_NOERROR;
#if (defined INCLUDE_FRAME_LOGGING)
EC_T_DWORD dwLogFrameFlags = 0;
#endif

    if( m_bInvalidLicense )
    {
        if(m_bChecked)
        {
            m_dwCheckCounter++;
            DeviceFrameFree(pDeviceFrameDesc);
            dwRetVal = EC_E_DUPLICATE;
            goto Exit;
        }
        else if( m_tTimeout.IsStopped() || m_tTimeout.IsElapsed())
        {
            EC_T_LINK_FRAMEDESC oMacFrame = {0};
            EC_T_BYTE           aSrcMac[8];
            EC_T_DWORD          dwRes;

#if (defined ECWIN)
			if (m_dwEcWinTime == m_tTimeout.GetDuration())
			{
                m_bEcWinChecked = EC_TRUE;
                m_tTimeout.Start(1000 * m_pMaster->GetTimeLimit(), m_pMaster->GetMsecCounterPtr());
		    }
			else
#endif
			{
				m_bChecked = EC_TRUE;
				m_tTimeout.Stop();
			}
            /* get MAC address */
            oMacFrame.pdwTimeStampLo = (EC_T_DWORD*)&aSrcMac[0]; oMacFrame.pdwTimeStampPostLo = (EC_T_DWORD*)&aSrcMac[4];
            dwRes = (m_LinkDrvDesc.pfEcLinkAllocSendFrame)(m_LinkDrvDesc.pvLinkInstance, &oMacFrame, 0x4154);

            if(dwRes == EC_E_NOERROR)
            {
                EC_T_BYTE*      pCheckPoint;
                EC_T_DWORD      dwLkCheckOffset;
                union
                {
                    EC_T_UINT64     qwLicenseKey;
                    EC_T_BYTE       b[8];
                } uKey;
                EC_T_BYTE       bySave;
                EC_T_DWORD      dwLicenseKey1, dwLicenseKey2;

                dwLkCheckOffset = OsQueryMsecCount() % (512/8);
                dwLkCheckOffset *= 8;
                pCheckPoint = &m_pMaster->m_pLkMemory[dwLkCheckOffset];
                OsMemcpy(uKey.b, pCheckPoint, sizeof(uKey.qwLicenseKey));

                bySave = uKey.b[0]; uKey.b[0] = uKey.b[5]; uKey.b[5] = bySave;
                bySave = uKey.b[1]; uKey.b[1] = uKey.b[7]; uKey.b[7] = bySave;
                bySave = uKey.b[2]; uKey.b[2] = uKey.b[4]; uKey.b[4] = bySave;
                bySave = uKey.b[3]; uKey.b[3] = uKey.b[6]; uKey.b[6] = bySave;

                dwLicenseKey1 = EC_HIDWORD(uKey.qwLicenseKey);
                dwLicenseKey1 /= 52639; 
                dwLicenseKey1 -= aSrcMac[0]; 
                if(EC_MAKEWORD(aSrcMac[2], aSrcMac[5]) == dwLicenseKey1)
                    m_bInvalidLicense = EC_FALSE;

                dwLicenseKey2 = EC_LODWORD(uKey.qwLicenseKey);
                dwLicenseKey2 /= 63599; 
                dwLicenseKey2 -= aSrcMac[1]; 
                if(EC_MAKEWORD(aSrcMac[3], aSrcMac[4]) != dwLicenseKey2)
                    m_bInvalidLicense = EC_TRUE;

#ifdef FRAME_LOSS_SIMULATION
                if (m_bInvalidLicense && m_bChecked)
                    SetRxFrameLossSimulation(0, 1000, 1, 1);
#endif
            }
            else
            {
                m_bInvalidLicense = EC_TRUE;
            }
        }
    }
#ifdef INCLUDE_RED_DEVICE
    if( m_bRedDeviceEnabled)
    {
        /* modify source mac */
        pDeviceFrameDesc->oLinkFrameDesc.pbyFrame[6] &= ~8;

        RedBreakFrameSplit(pDeviceFrameDesc);
    }
#endif
    if (IsSendEnabledOnMain())
    {
#if (defined INCLUDE_FRAME_LOGGING)
        if(m_pMaster->m_pvLogFrameCallBack != EC_NULL)
        {
            dwLogFrameFlags = m_pMaster->GetMasterLowestDeviceState();
            m_pMaster->m_pvLogFrameCallBack(m_pMaster->m_pvLogFrameCallBackContext, dwLogFrameFlags, pDeviceFrameDesc->oLinkFrameDesc.dwSize, pDeviceFrameDesc->oLinkFrameDesc.pbyFrame);
        }
#endif
		dwRetVal = FrameSendAndFree(
            &pDeviceFrameDesc->oLinkFrameDesc
            ,eFrameType_CYCLIC
#if (defined INCLUDE_MASTER_OBD)
            ,pDeviceFrameDesc->dwNumCmdsInFrame
#endif
            );
    }
    else
    {
        FrameFree(&pDeviceFrameDesc->oLinkFrameDesc);
    }
#ifdef INCLUDE_RED_DEVICE
    if (m_bRedDeviceEnabled)
    {
        if (IsSendEnabledOnRed())
        {
#if (defined INCLUDE_FRAME_LOGGING)
            if(m_pMaster->m_pvLogFrameCallBack != EC_NULL)
            {
                dwLogFrameFlags |= EC_LOG_FRAME_FLAG_RED_FRAME;
                m_pMaster->m_pvLogFrameCallBack(m_pMaster->m_pvLogFrameCallBackContext, dwLogFrameFlags, pDeviceFrameDesc->oRedLinkFrameDesc.dwSize, pDeviceFrameDesc->oRedLinkFrameDesc.pbyFrame);
            }
#endif
			/* only count datagrams in primary device not in redundancy device, therefore set param dwNumCmdsInFrame to 0 */
            dwRetVal = m_pRedDevice->FrameSendAndFree(
                &pDeviceFrameDesc->oRedLinkFrameDesc
                ,eFrameType_CYCLIC
#if (defined INCLUDE_MASTER_OBD)
                ,0
#endif
                );
        }
        else
        {
            m_pRedDevice->FrameFree(&pDeviceFrameDesc->oRedLinkFrameDesc);
        }
    }
#endif

Exit:
    return dwRetVal;
}



/********************************************************************************/
/** \brief Frees a frame for the EtherCAT device (two in case redundancy is enabled).
*
* \return status value.
*/
EC_T_VOID CEcDevice::DeviceFrameFree
(   EC_T_DEVICE_FRAMEDESC* pDeviceFrameDesc      /* [in]  frame to free */
) 
{ 
    /* call base class to send frame */
    FrameFree(&pDeviceFrameDesc->oLinkFrameDesc);
#ifdef INCLUDE_RED_DEVICE
    if (m_bRedDeviceEnabled)
    {
        m_pRedDevice->FrameFree(&pDeviceFrameDesc->oRedLinkFrameDesc);
    }
#endif
}

/********************************************************************************/
/** \brief Check if send is enabled on the main and red line
 * 
 * \return EC_TRUE or EC_FALSE
 */
EC_T_BOOL CEcDevice::IsSendEnabledOnMain(EC_T_VOID)
{
    return (IsMainLinkConnected() && !m_pMaster->m_pBusTopology->IsSendMaskedOnMain());
}
#ifdef INCLUDE_RED_DEVICE
EC_T_BOOL CEcDevice::IsSendEnabledOnRed(EC_T_VOID)
{
    return (IsRedLinkConnected() && !m_pMaster->m_pBusTopology->IsSendMaskedOnRed());
}
#endif


/********************************************************************************/
/** \brief State change debug message.
*
* \return N/A.
*/
EC_T_VOID CEcDevice::DbgMsg(
    EC_T_BOOL bPrintMsg, 
    EC_T_BOOL bSendMsgFrame, 
    EC_T_BYTE byEtherType0, 
    EC_T_BYTE byEtherType1, 
    const
    EC_T_CHAR* szFormat, 
    EC_T_VALIST vaArgs
)
{
EC_T_DWORD dwRes;
EC_T_LINK_FRAMEDESC oFrame;
EC_T_WORD wEtherType = EC_MAKEWORD(byEtherType1, byEtherType0);

    if (!IsSendEnabledOnMain())
    {
        bSendMsgFrame = EC_FALSE;
    }
    if (!bPrintMsg && !bSendMsgFrame)
    {
        return;
    }
    OsMemset(m_pbyDbgMsg, 0, ETHERNET_FRAME_LEN + MAX_DBGMSG_LEN);
    OsVsnprintf((EC_T_CHAR*)&m_pbyDbgMsg[ETHERNET_FRAME_LEN], MAX_DBGMSG_LEN, szFormat, vaArgs);
    
    /* send message as frame, validate given EtherType: >= 1536: EtherType (supported as parameter), <=1500: size of payload (not supported as parameter), 1501–1535: undefined */
    if (bSendMsgFrame && (wEtherType >= 1536))
    {
    EC_T_BYTE abyEthDbgHdr[ETHERNET_FRAME_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff,1,1,1,1,1,1,0,0};

        OsMemset(&oFrame, 0, sizeof(EC_T_LINK_FRAMEDESC));
        oFrame.dwSize = (EC_T_DWORD)EC_MAX(OsStrlen(&m_pbyDbgMsg[ETHERNET_FRAME_LEN]) + ETHERNET_FRAME_LEN + 1, ETHERNET_MIN_FRAME_LEN);
        dwRes = FrameAlloc(&oFrame, oFrame.dwSize);
        if (dwRes == EC_E_NOERROR)
        {
            if (EC_NULL == oFrame.pbyFrame)
            {
                oFrame.pbyFrame = m_pbyDbgMsg;
            }
            else
            {
                OsMemset(oFrame.pbyFrame, 0, ETHERNET_FRAME_LEN + MAX_DBGMSG_LEN);
                OsStrncpy(&oFrame.pbyFrame[ETHERNET_FRAME_LEN], &m_pbyDbgMsg[ETHERNET_FRAME_LEN], MAX_DBGMSG_LEN);
                oFrame.pbyFrame[ETHERNET_FRAME_LEN + MAX_DBGMSG_LEN - 1] = '\0';
            }

            EC_SET_FRM_WORD(&abyEthDbgHdr[12], wEtherType);
            OsMemcpy(oFrame.pbyFrame, abyEthDbgHdr, ETHERNET_FRAME_LEN);
            
#if (defined INCLUDE_FRAME_LOGGING)
            if (m_pMaster->m_pvLogFrameCallBack != EC_NULL)
            {
                m_pMaster->m_pvLogFrameCallBack(m_pMaster->m_pvLogFrameCallBackContext, EC_LOG_FRAME_FLAG_DBG_FRAME, oFrame.dwSize, oFrame.pbyFrame);
            }
#endif
            FrameSendAndFree( 
                &oFrame, 
                eFrameType_DEBUG, 
#if (defined INCLUDE_MASTER_OBD)
                0,                          /* DEBUG is not counted anyway */
#endif
                EC_TRUE 
                );
        }
    }
    if (bPrintMsg)
    {
        LLDbgMsgHook("%s", &m_pbyDbgMsg[ETHERNET_FRAME_LEN]);
    }
}


/********************************************************************************/
/** \brief Send error message to link layer.
*
* \return N/A.
*/
EC_T_VOID CEcDevice::LinkErrorMsg(const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;

    if( m_pMaster->m_MasterConfig.dwErrorMsgToLinkLayer )
    {
        EC_VASTART( vaArgs, szFormat );
        DbgMsg( EC_FALSE, EC_TRUE, 0xFF, 0xFE, szFormat, vaArgs );
        EC_VAEND(vaArgs);
    }
}


/********************************************************************************/
/** \brief Send state change message to link layer.
*
* \return N/A.
*/
EC_T_VOID CEcDevice::LinkStateChngMsg(EC_T_DWORD dwLvl, const EC_T_CHAR* szFormat, ...)
{
EC_T_VALIST vaArgs;
    EC_VASTART( vaArgs, szFormat );
    DbgMsg( (dwLvl & 1), (dwLvl & 2), 0xFF, 0xFF, szFormat, vaArgs );
    EC_VAEND(vaArgs);
}


/********************************************************************************/
/** \brief Send debug message to link layer.
*
* \return N/A.
*/
EC_T_VOID CEcDevice::LinkDbgMsg
(   EC_T_DWORD dwLvl, 
    EC_T_BYTE byEtherType0, 
    EC_T_BYTE byEtherType1, 
    const 
    EC_T_CHAR* szFormat, 
    ...)
{
EC_T_VALIST vaArgs;

    EC_VASTART(vaArgs, szFormat);
    DbgMsg((dwLvl & 1), (dwLvl & 2), byEtherType0, byEtherType1, szFormat, vaArgs);
    EC_VAEND(vaArgs);
}

#if (defined INCLUDE_EOE_SUPPORT)
/********************************************************************************/
/** \brief Gets a pointer to the Ethernet switch.
*
* \return static_cast<CEcEthSwitch*>(m_pSwitch)
*/
CEcEthSwitch* CEcDevice::EthernetSwitch()
{
    return static_cast<CEcEthSwitch*>(m_pSwitch);
}
#endif /* INCLUDE_EOE_SUPPORT */

/********************************************************************************/
/** \brief Creates an EtherCAT master.
*
* \return EC_NULL or m_pMaster
*/
CEcMaster* CEcDevice::DeviceCreateMaster(   
    PEcMasterDesc           pDesc,              /* [in] master descriptor */
    MBX_CALLBACK            pfMbCallback,       /* [in] mailbox callback */
    NOTIFY_CALLBACK         pfNotifyCallback,   /* [in] notification callback */
    struct _EC_T_MASTER_CONFIG* pMasterConfig,  /* [in] master configuration parameters */
    struct _EC_T_MASTER_CONFIG_EX* pMasterConfigEx, /* [in] extended master configuration parameters */
    struct _EC_T_INTFEATUREFIELD_DESC* pFeatures,/* [in] features to be configured */
    EC_T_DWORD              dwVersion           /* [in] application asked version */
	                              )
{
    if( m_pMaster )
        return EC_NULL;

    m_pMaster = EC_NEW(CEcMaster(static_cast<CEcDevice*>(this), pDesc, 
                            pfMbCallback, pfNotifyCallback, pMasterConfig, pMasterConfigEx, 
                            pFeatures, dwVersion));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::m_pMaster", m_pMaster, sizeof(CEcMaster));

#if (defined INCLUDE_MASTER_OBD)
    m_pdwTxFrameCrt     = m_pMaster->SDOGetTXFrameCrtPtr();
    m_pdwRxFrameCrt     = m_pMaster->SDOGetRXFrameCrtPtr();
    m_pdwCycFrameCrt    = m_pMaster->SDOGetCycFrameCrtPtr();
    m_pdwCycDgramCrt    = m_pMaster->SDOGetCycDgramCrtPtr();
    m_pdwAcycFrameCrt   = m_pMaster->SDOGetAcycFrameCrtPtr();
    m_pdwAcycDgramCrt   = m_pMaster->SDOGetAcycDgramCrtPtr();
    m_pdwTxBytesSecondCrt = m_pMaster->SDOGetBytesSecondCrtPtr();
    m_pdwTxBytesCycleCrt  = m_pMaster->SDOGetBytesCycleCrtPtr();
#endif

    return m_pMaster;
}

#if (defined INCLUDE_EOE_SUPPORT)
/********************************************************************************/
/** \brief Creates an Ethernet switch.
*
* \return EC_E_INVALIDSTATE or EC_E_INVALIDPARM
*/
EC_T_DWORD CEcDevice::CreateSwitch(EthernetCreateSwitch *pCreate)
{
    OsDbgAssert(m_pSwitch == EC_NULL);

    m_pSwitch = EC_NEW(CEcEthSwitch(pCreate->dwMaxPorts, pCreate->dwMaxFrames, pCreate->dwMaxMACs, GetMemLog()));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_DEVICE, "CEcDevice::m_pSwitch", m_pSwitch, sizeof(CEcEthSwitch));
    if (m_pSwitch == EC_NULL)
    {
        return EC_E_NOMEMORY;
    }
    else
    {
        return EC_E_NOERROR;
    }
}
#endif /* INCLUDE_EOE_SUPPORT */

/********************************************************************************/
/** \brief Creates an IO image.
*
* \return EC_E_NOERROR
*/
EC_T_DWORD CEcDevice::CreateImage(EC_T_DWORD nIn, EC_T_DWORD nOut)
{   
EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* delete actual io image if already exist */
    CEcIoImage::DestroyInstance(m_pIOImage, GetMemLog());
    m_pIOImage = EC_NULL;

#if (defined INCLUDE_TRACE_DATA)
    /* append trace data to output following the configuration rules */
    if (m_pMaster->m_wTraceDataSize > 0)
    {
        /* adjust copy offset if eCycFrameLayout_IN_DMA with no process data is detected */
        if ((m_pMaster->m_eCycFrameLayout == eCycFrameLayout_IN_DMA) && (0 == m_pMaster->GetCopyOffset()))
        {
            m_pMaster->SetConfiguratorOffsets(ETYPE_EC_CMD_HEADER_LEN);
        }
        /* insert trace data */
        m_pMaster->m_dwTraceDataOffset = nOut;
        nOut += m_pMaster->m_wTraceDataSize;

        /* insert command overhead if needed */
        if (0 != m_pMaster->GetCopyOffset())
        {
            m_pMaster->m_dwTraceDataOffset += ETYPE_EC_CMD_HEADER_LEN;
            nOut += ETYPE_EC_OVERHEAD;
        }
    }
#endif /* INCLUDE_TRACE_DATA */

    /* create io image */
    m_pIOImage = CEcIoImage::CreateInstance(nIn, nOut, GetMemLog());
    if (m_pIOImage == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    m_pMaster->SetPDInPointer(  m_pIOImage->GetInputPtr( 0, 1), m_pIOImage->GetInputSize() );
    m_pMaster->SetPDOutPointer( m_pIOImage->GetOutputPtr(0, 1), m_pIOImage->GetOutputSize());

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Sends the queued EtherCAT frames to the network adapter.
*
* Commands are queued using CEcMaster::QueueEcatCmdReq() into the current pending frame descriptor.
* Using CEcMaster::EcatFlushCurrPendingSlaveFrame() the current pending frame is added
* into CEcMaster::m_pDeviceFrameDescFifo.
* Finally all queued frames are sent here.
*
* This method is called by eUsrJob_SendAcycFrames and 
* \return 0
*/
EC_T_DWORD CEcDevice::SendQueuedFrames(EC_T_WORD* pwNumSendFrames)
{
EC_T_DWORD              dwRetVal = EC_E_NOERROR;
EC_T_DWORD              dwRes    = EC_E_NOERROR;
ECAT_SLAVEFRAME_DESC*   pSlvFrmDesc    = EC_NULL;
EC_T_WORD               wSentFramesCnt = 0;
EC_T_WORD               wSentFramesThrottleCnt = (EC_T_WORD)m_pMaster->m_MasterConfig.dwMaxSentQueuedFramesPerCyc;

    *pwNumSendFrames = 0;

    while (m_pDeviceFrameDescFifo->RemoveNoLock(pSlvFrmDesc))
    {
        EC_T_DEVICE_FRAMEDESC   oDeviceFrameDesc;
        EC_T_WORD               wFrameSize      = 0;
#if (defined INCLUDE_RED_DEVICE)
        EC_T_BOOL               bSentByRedOnly  = EC_FALSE;
#endif
#if (defined INCLUDE_FRAME_LOGGING)
        EC_T_DWORD              dwLogFrameFlags = m_pMaster->GetMasterLowestDeviceState();
#endif
        OsMemset(&oDeviceFrameDesc, 0, sizeof(EC_T_DEVICE_FRAMEDESC));

        /* fill frame size to ETHERNET_MIN_FRAME_LEN bytes */
        wFrameSize = (EC_T_WORD)SIZEOF_88A4_FRAME(pSlvFrmDesc->poEcSlaveFrame->m_p88A4Header);
#if (defined VLAN_FRAME_SUPPORT)
        if (m_pMaster->m_bVLANEnabled)
        {
            wFrameSize += ETYPE_VLAN_HEADER_LEN;
        }
#endif

        if (wFrameSize < ETHERNET_MIN_FRAME_LEN) wFrameSize = ETHERNET_MIN_FRAME_LEN;

        dwRes = DeviceFrameDescCreate(
            &oDeviceFrameDesc, EC_TRUE,
            (EC_T_BYTE*)&(pSlvFrmDesc->poEcSlaveFrame->m_EthernetFrame),
#if (defined INCLUDE_RED_DEVICE)
            m_pMaster->m_pbyRedFrame,
#endif
            wFrameSize);

        if (dwRes == EC_E_NOERROR)
        {
            oDeviceFrameDesc.dwNumCmdsInFrame = pSlvFrmDesc->poEcSlaveFrame->m_nNumCmdsInFrame;
            oDeviceFrameDesc.bIgnoreFrameInAcycLimits = pSlvFrmDesc->bIgnoreFrameInAcycLimits;

            /* check for registered DCM functions */
            if( EC_NULL != pSlvFrmDesc->pfnTimeStamp )
            {
                oDeviceFrameDesc.oLinkFrameDesc.pfnTimeStamp        = pSlvFrmDesc->pfnTimeStamp;
                oDeviceFrameDesc.oLinkFrameDesc.pdwTimeStampLo      = pSlvFrmDesc->pdwTimeStamp;
                oDeviceFrameDesc.oLinkFrameDesc.pdwTimeStampPostLo  = pSlvFrmDesc->pdwTimeStamp;
                oDeviceFrameDesc.oLinkFrameDesc.pvTimeStampCtxt     = pSlvFrmDesc->pvTimeStampCtxt;
#if (defined INCLUDE_DC_SUPPORT)
                oDeviceFrameDesc.oLinkFrameDesc.pdwLastTSResult     = m_pMaster->m_pDistributedClks->DcmLastTsResultPtr();
#else
                oDeviceFrameDesc.oLinkFrameDesc.pdwLastTSResult     = 0;
#endif
            }
            oDeviceFrameDesc.oLinkFrameDesc.wTimestampOffset = pSlvFrmDesc->wTimestampOffset;
            oDeviceFrameDesc.oLinkFrameDesc.wTimestampSize   = pSlvFrmDesc->wTimestampSize;
            oDeviceFrameDesc.oLinkFrameDesc.dwRepeatCnt      = pSlvFrmDesc->dwRepeatCnt;

            pSlvFrmDesc->poEcSlaveFrame->m_timeout.Start(m_pMaster->GetEcatCmdTimeout(), m_pMaster->GetMsecCounterPtr());

#ifdef INCLUDE_RED_DEVICE
            if (m_bRedDeviceEnabled)
            {
                RedBreakFrameSplit(&oDeviceFrameDesc);
            }
#endif
            /* send packet to network adapter */
            if (IsSendEnabledOnMain())
            {
                wSentFramesCnt = (EC_T_WORD)oDeviceFrameDesc.oLinkFrameDesc.dwRepeatCnt;
                if (0 == wSentFramesCnt)
                {
                    wSentFramesCnt++;
                }
#if (defined INCLUDE_FRAME_LOGGING)
                if (m_pMaster->m_pvLogFrameCallBack != EC_NULL)
                {
                    dwLogFrameFlags |= EC_LOG_FRAME_FLAG_ACYC_FRAME;
                    m_pMaster->m_pvLogFrameCallBack(m_pMaster->m_pvLogFrameCallBackContext, dwLogFrameFlags, oDeviceFrameDesc.oLinkFrameDesc.dwSize, oDeviceFrameDesc.oLinkFrameDesc.pbyFrame);
                }
#endif
                dwRetVal = FrameSendAndFree(
                    &oDeviceFrameDesc.oLinkFrameDesc,
                    eFrameType_ACYCLIC
#if (defined INCLUDE_MASTER_OBD)
                    , oDeviceFrameDesc.dwNumCmdsInFrame
#endif
                    );
                wSentFramesCnt = (EC_T_WORD)((EC_T_DWORD)wSentFramesCnt - oDeviceFrameDesc.oLinkFrameDesc.dwRepeatCnt);
            }
            else
            {
#ifdef INCLUDE_RED_DEVICE
    			if (m_bRedDeviceEnabled)
    			{
    				/* time stamping must be done by red device */
    				oDeviceFrameDesc.oRedLinkFrameDesc.pfnTimeStamp		  = oDeviceFrameDesc.oLinkFrameDesc.pfnTimeStamp;
    				oDeviceFrameDesc.oRedLinkFrameDesc.pvTimeStampCtxt	  = oDeviceFrameDesc.oLinkFrameDesc.pvTimeStampCtxt;
    				oDeviceFrameDesc.oRedLinkFrameDesc.pdwTimeStampLo	  = oDeviceFrameDesc.oLinkFrameDesc.pdwTimeStampLo;
    				oDeviceFrameDesc.oRedLinkFrameDesc.pdwTimeStampPostLo = oDeviceFrameDesc.oLinkFrameDesc.pdwTimeStampPostLo;
    				oDeviceFrameDesc.oRedLinkFrameDesc.pdwLastTSResult	  = oDeviceFrameDesc.oLinkFrameDesc.pdwLastTSResult;
                    oDeviceFrameDesc.oRedLinkFrameDesc.dwRepeatCnt        = oDeviceFrameDesc.oLinkFrameDesc.dwRepeatCnt;
                    bSentByRedOnly = EC_TRUE;
    			}
#endif
                FrameFree(&oDeviceFrameDesc.oLinkFrameDesc);
            }
#ifdef INCLUDE_RED_DEVICE
            if (m_bRedDeviceEnabled)
            {
                if (IsSendEnabledOnRed())
                {
                    if (bSentByRedOnly)
                    {
                        wSentFramesCnt = (EC_T_WORD)oDeviceFrameDesc.oRedLinkFrameDesc.dwRepeatCnt;
                        if (0 == wSentFramesCnt)
                        {
                            wSentFramesCnt++;
                        }
                    }
#if (defined INCLUDE_FRAME_LOGGING)
                    if (m_pMaster->m_pvLogFrameCallBack != EC_NULL)
                    {
                        dwLogFrameFlags |= EC_LOG_FRAME_FLAG_RED_FRAME;
                        m_pMaster->m_pvLogFrameCallBack(m_pMaster->m_pvLogFrameCallBackContext, dwLogFrameFlags, oDeviceFrameDesc.oRedLinkFrameDesc.dwSize, oDeviceFrameDesc.oRedLinkFrameDesc.pbyFrame);
                    }
#endif
					/* only count datagrams in primary device not in redundancy device, therefore set param dwNumCmdsInFrame to 0 */
                    dwRetVal = m_pRedDevice->FrameSendAndFree(
                        &oDeviceFrameDesc.oRedLinkFrameDesc,
                        eFrameType_ACYCLIC
#if (defined INCLUDE_MASTER_OBD)
                        , 0
#endif
                        );
                    if (bSentByRedOnly)
                    {
                        wSentFramesCnt = (EC_T_WORD)((EC_T_DWORD)wSentFramesCnt - oDeviceFrameDesc.oRedLinkFrameDesc.dwRepeatCnt);
                    }
                }
                else
                {
                    m_pRedDevice->FrameFree(&oDeviceFrameDesc.oRedLinkFrameDesc);
                }
            }
#endif
            /* count num of frames */
            *pwNumSendFrames = (EC_T_WORD)(*pwNumSendFrames + wSentFramesCnt);

            /* limit num of frames */
            if (wSentFramesThrottleCnt > 0)
            {
                /* is this frame counted for ACYC Limits ? */
                if (!oDeviceFrameDesc.bIgnoreFrameInAcycLimits)
                {
                    /* limit the number of queued frames to be sent */
                    wSentFramesThrottleCnt--;
                }
                if (0 == wSentFramesThrottleCnt)
                {
                    break;
                }
            }
        } /* if (dwRes == EC_E_NOERROR) */
    } 

    return dwRetVal;
}

/********************************************************************************/
/** \brief Cancel queued frames
*
* \return
*/
EC_T_VOID CEcDevice::CancelQueuedFrames(EC_T_DWORD dwCmdResult)
{
ECAT_SLAVEFRAME_DESC* pSlvFrmDesc = EC_NULL;

    while (m_pDeviceFrameDescFifo->RemoveNoLock(pSlvFrmDesc))
    {
        m_pMaster->CancelAcycFrame(pSlvFrmDesc->poEcSlaveFrame, dwCmdResult);
    }
}

/********************************************************************************/
/** \brief Process a received frame
*
* \return
*/
EC_T_VOID CEcDevice::ProcessFrame(EC_T_VOID* pvFrame, 
                                  EC_T_DWORD dwSize, 
                                  EC_T_BOOL  bIsRedFrame,
                                  EC_T_BOOL* pbFrameProcessed)
{
#if (defined INCLUDE_LOG_MESSAGES) && (defined INCLUDE_RED_DEVICE)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
PETHERNET_FRAME pFrame = EC_NULL;
EC_T_WORD* pwFrameType = EC_NULL;
#if (defined INCLUDE_FRAME_LOGGING)
EC_T_DWORD dwLogFrameFlags = 0;
#else
EC_UNREFPARM(dwSize);
#endif

#ifdef FRAME_LOSS_SIMULATION
    if (m_oRxFrameLoss.m_EFrameLossSimulationMode != eFrameLossSim_OFF)
    {
        if (m_oRxFrameLoss.IsThisFrameLost())
        {
            goto Exit;
        }
    }
#endif

#if (defined ECWIN)
	if (m_bEcWinChecked  && !m_dwEcWinJobDone) 
	{	
        EC_T_DWORD dwRes;

		if (!m_bInvalidLicense)
		{
            dwRes = vmfEventJobDone(VMF_COMM_JOB_LICENSE_REPORT, (VMF_HANDLE)(UINT_PTR)VMF_OSID_THISOS);
            if (dwRes == RTE_SUCCESS)
            {
                m_dwEcWinJobDone = EC_TRUE;
            }
        }
		else
		{
			EC_T_DWORD dwData = 0;

            dwRes = vmfEventJobDone(VMF_COMM_JOB_LICENSE_QUERY,(VMF_HANDLE)&dwData);
            if (dwRes == RTE_SUCCESS)
			{
			    if (0 != dwData)
				{
				    m_bInvalidLicense = EC_FALSE; 
				}
				m_dwEcWinJobDone = EC_TRUE;
			}
		}
    }
#endif

    /* process frame */
    pFrame      = (PETHERNET_FRAME)pvFrame;
    pwFrameType = FRAMETYPE_PTR(pFrame);

    switch (EC_ETHFRM_GET_FRAMETYPE(pvFrame))
    {
    case ETHERNET_FRAME_TYPE_BKHF:
        {               
            PETHERNET_88A4_HEADER p88A4 = (PETHERNET_88A4_HEADER)EC_ENDOF(pwFrameType);

            if (EC_AL_88A4HDR_GET_E88A4HDRTYPE(&(p88A4->E88A4Header)) == ETYPE_88A4_TYPE_ECAT)
            {
#if (defined INCLUDE_FRAME_LOGGING)
            PETYPE_EC_CMD_HEADER pEcCmdHeader = &p88A4->sETECAT.FirstEcCmdHeader;

                if ((m_pMaster->m_pvLogFrameCallBack != EC_NULL) && (EC_NULL != pFrame))
                {
                    dwLogFrameFlags = m_pMaster->GetMasterLowestDeviceState();
                    if (bIsRedFrame)
                    {
                        dwLogFrameFlags |= EC_LOG_FRAME_FLAG_RED_FRAME;
                    }
                    if (pEcCmdHeader->uCmdIdx.swCmdIdx.byIdx >= m_pMaster->m_byMinAcycCmdIdx)
                    {
                        dwLogFrameFlags |= EC_LOG_FRAME_FLAG_ACYC_FRAME;
                    }
                    dwLogFrameFlags |= EC_LOG_FRAME_FLAG_RX_FRAME;
                    m_pMaster->m_pvLogFrameCallBack(m_pMaster->m_pvLogFrameCallBackContext, dwLogFrameFlags, dwSize, (EC_T_BYTE*)pvFrame);
                }
#endif /* INCLUDE_FRAME_LOGGING */

#if (defined INCLUDE_RED_DEVICE)
                if (m_bRedDeviceEnabled)
                {
                EC_T_DWORD       dwRes              = EC_E_ERROR;
                EC_T_BOOL        bFrameProcessed    = EC_FALSE;
                EC_T_BOOL        bSendEnabledOnMain = IsSendEnabledOnMain();
                EC_T_BOOL        bSendEnabledOnRed  = IsSendEnabledOnRed();
                EC_T_BOOL        bFrameSendByRed    = EC_FALSE;
                EC_RED_UNPROC_T* pOwnUnprocData     = EC_NULL;
                EC_RED_UNPROC_T* pOtherUnprocData   = EC_NULL;

                    /* detect if frame was sent on the redundancy line */
                    if ((pFrame->Source.b[0] & 0x08) == 0x08)
                    {
                        bFrameSendByRed = EC_TRUE;
                    }
                    /* select unprocessed data queue and reset permanent frame loss flag */
                    if (bIsRedFrame)
                    {
                        pOwnUnprocData      = &m_scRedLineUnproc;
                        pOtherUnprocData    = &m_scMainLineUnproc;
                        m_dwRedFrameLossCnt = 0;
                        m_bRedFrameLoss     = EC_FALSE;
                        if (!bSendEnabledOnRed && !m_pRedDevice->IsLinkStatusForced())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+RED_CHNG), "EcDevice::ProcessFrame(): Receiving frames on red\n"));
                            m_pMaster->m_pBusTopology->MaskSendOnRed(EC_FALSE);
                            RefreshLinkStatus();
                            bSendEnabledOnRed = IsSendEnabledOnRed();
                        }
                    }
                    else
                    {
                        pOwnUnprocData       = &m_scMainLineUnproc;
                        pOtherUnprocData     = &m_scRedLineUnproc;
                        m_dwMainFrameLossCnt = 0;
                        m_bMainFrameLoss     = EC_FALSE;
                        if (!bSendEnabledOnMain && !IsLinkStatusForced())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+RED_CHNG), "EcDevice::ProcessFrame(): Receiving frames on main\n"));
                            m_pMaster->m_pBusTopology->MaskSendOnMain(EC_FALSE);
                            m_pMaster->m_pBusTopology->SetTopologyChanged();
                            RefreshLinkStatus();
                            bSendEnabledOnMain = IsSendEnabledOnMain();
                        }
                    }
                    /* flush the queues of disconnected lines. Use a trigger if both */ 
                    /* lines were disconnected and nobody could call this function   */
                    if (!m_bLinkConnected || (bIsRedFrame && bFrameSendByRed) || (!bIsRedFrame && !bFrameSendByRed))
                    {
                        if (!m_bLinkConnected || !bSendEnabledOnMain)
                        {
                            while (m_scMainLineUnproc.dwNum > 0)
                            {
                                /* refresh queue control */
                                m_scMainLineUnproc.abValid[m_scMainLineUnproc.dwTailIdx] = EC_FALSE;
                                m_scMainLineUnproc.dwTailIdx = (m_scMainLineUnproc.dwTailIdx+1)%MAX_UNPROC_FRAMES;
                                m_scMainLineUnproc.dwNum--;
                            }
                        }
                        if (!m_bLinkConnected || !bSendEnabledOnRed)
                        {
                            while (m_scRedLineUnproc.dwNum > 0)
                            {
                                /* refresh queue control */
                                m_scRedLineUnproc.abValid[m_scRedLineUnproc.dwTailIdx] = EC_FALSE;
                                m_scRedLineUnproc.dwTailIdx = (m_scRedLineUnproc.dwTailIdx+1)%MAX_UNPROC_FRAMES;
                                m_scRedLineUnproc.dwNum--;
                            }
                        }
                        m_bLinkConnected = EC_TRUE;
                    }
                    /* process frames sent on only one line */
                    if (ETH_ADDRESS_CABLE_RED_FRAME_ID_ONLY_ONE_LINE == pFrame->Source.b[5])
                    {
                        if (bIsRedFrame)
                        {
                            dwRes = m_pMaster->CheckFrame(EC_NULL, pFrame);
                        }
                        else
                        {
                            dwRes = m_pMaster->CheckFrame(pFrame, EC_NULL);
                        }
                        EC_UNREFPARM(dwRes);
                        bFrameProcessed = EC_TRUE;
                    }
                    /* search for matching frame in the unprocessed data queue of the other line */
                    if (!bFrameProcessed)
                    {
                        EC_T_DWORD      dwTailIdx   = pOtherUnprocData->dwTailIdx;
                        EC_T_DWORD      dwFrameNum  = pOtherUnprocData->dwNum;
                        PETHERNET_FRAME pOtherFrame = EC_NULL;

                        /* look for the frame in the queue */
                        while (0 != dwFrameNum)
                        {
                            if (pOtherUnprocData->abyIdx[dwTailIdx] == pFrame->Source.b[5])
                            {
                                break;
                            }
                            dwTailIdx = (dwTailIdx+1)%MAX_UNPROC_FRAMES;
                            dwFrameNum--;
                        }
                        if (0 != dwFrameNum)
                        {
                            /* drop all frames before and get the frame */
                            while (pOtherUnprocData->dwNum >= dwFrameNum)
                            {
                                /* get frame */
                                pOtherFrame = (PETHERNET_FRAME)&pOtherUnprocData->pbyFrames[pOtherUnprocData->dwTailIdx*ETHERNET_MAX_FRAMEBUF_LEN];

                                /* refresh queue control */
                                pOtherUnprocData->abValid[pOtherUnprocData->dwTailIdx] = EC_FALSE;
                                pOtherUnprocData->dwTailIdx = (pOtherUnprocData->dwTailIdx+1)%MAX_UNPROC_FRAMES;
                                pOtherUnprocData->dwNum--;
                            }
                            /* reset permanent frame loss monitoring */
                            m_oUnprocTimeout.Stop();

                            /* two matching frames are available, process them */
                            if (bIsRedFrame)
                            {
                                dwRes = m_pMaster->CheckFrame(pOtherFrame, pFrame);
                            }
                            else
                            {
                                dwRes = m_pMaster->CheckFrame(pFrame, pOtherFrame);
                            }
                            EC_UNREFPARM(dwRes);
                            bFrameProcessed = EC_TRUE;

                            /* flush the redundancy queue of the own line containing old frames */
                            while (pOwnUnprocData->dwNum != 0)
                            {
                                pOwnUnprocData->abValid[pOwnUnprocData->dwTailIdx] = EC_FALSE;
                                pOwnUnprocData->dwTailIdx = (pOwnUnprocData->dwTailIdx+1)%MAX_UNPROC_FRAMES;
                                pOwnUnprocData->dwNum--;
                            }
                        }
                    }
                    /* process the frame alone if the other line is disconnected or     */
                    /* permanent frame loss is detected. A threshold of 2 give a chance */
                    /* to the other side to receive before processing the frame alone   */
                    if (!bFrameProcessed)
                    {
                        EC_T_BOOL bFlushOtherLine = EC_FALSE;

                        if (bIsRedFrame)
                        {
                            if (!bSendEnabledOnMain || m_bMainFrameLoss)
                            {
                                /* first process the frames in the own queue */
                                RedProcessQueuedFrames(bIsRedFrame);

                                /* process current frame */
                                m_pMaster->CheckFrame(EC_NULL, pFrame);
                                bFrameProcessed = EC_TRUE;
                            }
                            else
                            {
                                if (m_oUnprocTimeout.IsElapsed())
                                {
                                    m_oUnprocTimeout.Stop();

                                    /* give a chance to the main line to receive */
                                    m_dwMainFrameLossCnt++;
                                    if (m_dwMainFrameLossCnt > 2)
                                    {
									    m_bMainFrameLoss = EC_TRUE;
									    bFlushOtherLine = EC_TRUE;
								    }
                                }
                            }
                        }
                        else
                        {
                            if (!bSendEnabledOnRed || m_bRedFrameLoss)
                            {
                                /* first process the frames in the own queue */
                                RedProcessQueuedFrames(bIsRedFrame);

                                /* process current frame */
                                m_pMaster->CheckFrame(pFrame, EC_NULL);
                                bFrameProcessed = EC_TRUE;
                            }
                            else
                            {
                                if (m_oUnprocTimeout.IsElapsed())
                                {
                                    m_oUnprocTimeout.Stop();

                                    /* give a chance to the red line to receive */
                                    m_dwRedFrameLossCnt++;
                                    if (m_dwRedFrameLossCnt > 2)
                                    {
									    m_bRedFrameLoss = EC_TRUE;
									    bFlushOtherLine = EC_TRUE;
								    }
                                }
                            }
                        }
                        if (bFlushOtherLine)
                        {
							/* flush the queue of the other line containing old frames */
							while (pOtherUnprocData->dwNum != 0)
							{
								pOtherUnprocData->abValid[pOtherUnprocData->dwTailIdx] = EC_FALSE;
								pOtherUnprocData->dwTailIdx = (pOtherUnprocData->dwTailIdx+1)%MAX_UNPROC_FRAMES;
								pOtherUnprocData->dwNum--;
							}
                        }
                    }
                    /* store this frame in the unprocessed data queue to process it later */
                    if (!bFrameProcessed)
                    {
                        /* check for queue overflow */
                        if (pOwnUnprocData->dwNum >= MAX_UNPROC_FRAMES)
                        {
                            /* drop oldest frame */
                            pOwnUnprocData->abValid[pOwnUnprocData->dwTailIdx] = EC_FALSE;
                            pOwnUnprocData->dwTailIdx = (pOwnUnprocData->dwTailIdx+1)%MAX_UNPROC_FRAMES;
                            pOwnUnprocData->dwNum--;
                        }
#if 0 /* performance increasing but doesn't work with HILSCHER slaves */

                        /* is this a processed frame ? */
                        if ((pFrame->Source.b[0] & 0x02) == 0x02)
                        {
                            /* save the whole frame */
                            OsMemcpy(&pOwnUnprocData->pbyFrames[pOwnUnprocData->dwHeadIdx*ETHERNET_MAX_FRAMEBUF_LEN], pFrame, dwSize);
                        }
                        else
                        {
                            /* save only the frame header */
                            OsMemcpy(&pOwnUnprocData->pbyFrames[pOwnUnprocData->dwHeadIdx*ETHERNET_MAX_FRAMEBUF_LEN], pFrame, sizeof(ETHERNET_FRAME));
                        }                            
#else
                        /* save the whole frame */
                        OsMemcpy(&pOwnUnprocData->pbyFrames[pOwnUnprocData->dwHeadIdx*ETHERNET_MAX_FRAMEBUF_LEN], pFrame, dwSize);
#endif
                        /* monitor permanent frame loss */
                        if (!m_oUnprocTimeout.IsStarted())
                        {
                            m_oUnprocTimeout.Start(EC_MAX(1, ((2 * m_pMaster->GetBusCycleTimeUsec()) / 1000)), m_pMaster->GetMsecCounterPtr());
                        }
                        /* used for identification */
                        pOwnUnprocData->abyIdx[pOwnUnprocData->dwHeadIdx] = pFrame->Source.b[5];

                        /* refresh queue control */
                        pOwnUnprocData->abValid[pOwnUnprocData->dwHeadIdx] = EC_TRUE;
                        pOwnUnprocData->dwHeadIdx = (pOwnUnprocData->dwHeadIdx + 1) % MAX_UNPROC_FRAMES;
                        pOwnUnprocData->dwNum++;
                    }
                } /* if (m_bRedDeviceEnabled) */
                else
#else
                EC_UNREFPARM(dwSize);
                EC_UNREFPARM(bIsRedFrame);
#endif /* defined INCLUDE_RED_DEVICE */
                {
                    m_pMaster->CheckFrame(pFrame, EC_NULL);
                }
            }
        } break;
    default:
        {
            LinkErrorMsg("Non Ethercat frame received\n");
        } break;
    }

#ifdef FRAME_LOSS_SIMULATION
    Exit:
#endif
    /* not supported sync modes */
    /* frames are currently always processed */
    if (pbFrameProcessed != EC_NULL)
    {
        *pbFrameProcessed = EC_TRUE;
    }
}



/********************************************************************************/
/** \brief This function is called by the link layer after a new frame was received.
*
* \return status code
*/
EC_T_VOID CEcDevice::ReceiveFrameCallback(EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed)
{

    /* incoming frames are currently handled directly */
#if (defined INCLUDE_MASTER_OBD)
    if( EC_NULL != m_pdwRxFrameCrt )
    {
        (*m_pdwRxFrameCrt) = ((*m_pdwRxFrameCrt)+1);
	        }
#endif

    ProcessFrame(pLinkFrameDesc->pbyFrame, pLinkFrameDesc->dwSize, EC_FALSE, pbFrameProcessed);
}

/********************************************************************************/
/** \brief This function is called by the link layer after a new frame was received.
*
* \return status code
*/
EC_T_DWORD CEcDevice::ReceiveFrameCallbackGateway(EC_T_VOID* pvContext, EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed)
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* incoming frames are currently handled directly */
    OsDbgAssert( pvContext != EC_NULL );
    if (pvContext != EC_NULL)
    {
        ((CEcDevice*)pvContext)->ReceiveFrameCallback(pLinkFrameDesc, pbFrameProcessed);
    }
    else
    {
        /* this frame will never be processed! */
        *pbFrameProcessed = EC_TRUE;
    }
    return dwRetVal;
}


/********************************************************************************/
/** \brief Update the link state of the network connection.
*
* \return EC_TRUE if Link / Cable connected, EC_FALSE otherwise
*/
EC_T_LINKSTATUS CEcDevice::GetLinkStatus(EC_T_VOID)
{
EC_T_LINKSTATUS eLinkStatus       = eLinkStatus_UNDEFINED;
#ifdef INCLUDE_RED_DEVICE
EC_T_LINKSTATUS eLinkStatusRed    = eLinkStatus_UNDEFINED;
#endif

    /* get link status */
    eLinkStatus = CEcDeviceBase::GetLinkStatus();

#ifdef INCLUDE_RED_DEVICE
    if (m_bRedDeviceEnabled)
    {
        /* get red link status */
        eLinkStatusRed = m_pRedDevice->GetLinkStatus();
    }
    /* if one link is connected, global link is connected */
    if ((eLinkStatus_OK == eLinkStatus) || (eLinkStatus_OK == eLinkStatusRed))
    {
        eLinkStatus = eLinkStatus_OK;
    }
    else
    {
        eLinkStatus = eLinkStatus_DISCONNECTED;
    }
#endif

    return eLinkStatus;
}


/********************************************************************************/
/** \brief Update the link state of the network connection.
*
* \return EC_TRUE if Link / Cable connected, EC_FALSE otherwise
*/
EC_T_LINKSTATUS CEcDevice::RefreshLinkStatus(EC_T_VOID)
{
#ifdef INCLUDE_RED_DEVICE
EC_T_LINKSTATUS eLinkStatusMainOld = eLinkStatus_UNDEFINED;
EC_T_LINKSTATUS eLinkStatusMain    = eLinkStatus_UNDEFINED;
EC_T_LINKSTATUS eLinkStatusRedOld  = eLinkStatus_UNDEFINED;
EC_T_LINKSTATUS eLinkStatusRed     = eLinkStatus_UNDEFINED;
#else
EC_T_LINKSTATUS eLinkStatusOld     = eLinkStatus_UNDEFINED;
#endif
EC_T_LINKSTATUS eLinkStatus        = eLinkStatus_UNDEFINED;
EC_T_NOTIFICATION_INTDESC* pNotification;

#ifdef INCLUDE_RED_DEVICE
    /* refresh (main) link status */
    eLinkStatusMainOld = CEcDeviceBase::GetLinkStatus();
    CEcDeviceBase::RefreshLinkStatus();
    eLinkStatusMain    = CEcDeviceBase::GetLinkStatus();
    if (m_bRedDeviceEnabled)
    {
        /* refresh red link status */
        eLinkStatusRedOld = m_pRedDevice->GetLinkStatus();
        m_pRedDevice->RefreshLinkStatus();
        eLinkStatusRed    = m_pRedDevice->GetLinkStatus();
    }
    /* refresh (global) link status */
    eLinkStatus = GetLinkStatus();
#else
    /* refresh link status */
    eLinkStatusOld = GetLinkStatus();
    CEcDeviceBase::RefreshLinkStatus();
    eLinkStatus    = GetLinkStatus();
#endif

    /* handle notification */
    if ((eLinkStatus_OK == eLinkStatus) && (eLinkStatus_OK != m_ELinkStatusNotified))
    {
        /* refresh cyclic frame manager */
        m_pMaster->ResetCyclicFrmMgr();

        /* notify the application (don't notify first) */
        if (eLinkStatus_UNDEFINED != m_ELinkStatusNotified)
        {
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_ETH_LINK_CONNECTED;
                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, sizeof(EC_T_DWORD));
            }
        }
    }
    else if ((eLinkStatus_OK != eLinkStatus) && ((eLinkStatus_OK == m_ELinkStatusNotified) || (eLinkStatus_UNDEFINED == m_ELinkStatusNotified)))
    {
#ifdef INCLUDE_RED_DEVICE
        /* set private flag for redundancy management */
        m_bLinkConnected = EC_FALSE;
#endif
        /* notify the application */
        if (m_pMaster->GetCycErrorNotifyMask() & EC_ERR_MASK_ETH_LINK_NOT_CONNECTED)
        {
            /* notify the application */
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_ETH_LINK_NOT_CONNECTED;
                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, sizeof(EC_T_DWORD));
    }
        }
    }
    /* inform the bus topology module about the link status change */
#if (defined INCLUDE_RED_DEVICE)
    if (eLinkStatusMainOld != eLinkStatusMain)
    {
        m_pMaster->m_pBusTopology->MainLinkStatusChanged(eLinkStatusMain, eLinkStatusRed);
    }
    if (eLinkStatusRedOld != eLinkStatusRed)
    {
        m_pMaster->m_pBusTopology->RedLinkStatusChanged(eLinkStatusMain, eLinkStatusRed);
    }
#else
    if (eLinkStatusOld != eLinkStatus)
    {
        m_pMaster->m_pBusTopology->LinkStatusChanged(eLinkStatus);
    }
#endif
    /* save last notified link status */
    m_ELinkStatusNotified = eLinkStatus;
    
    return eLinkStatus;
}


#if (defined INCLUDE_RED_DEVICE)
/********************************************************************************/
/** \brief Redundancy: In case of line break, adjust auto inc cmds
*
* Note: 
* Auto Inc Addresses
* 1. Slave -->  0x0000 -->  0 dez.
* 2. Slave -->  0xFFFF --> -1 dez.
* 3. Slave -->  0xFFFE --> -2 dez.

* Without break
* 0x0000(0)    0xFFFF(-1)    0xFFFE(-2)    0xFFFD(-3)    0xFFFC(-4)    0xFFFB(-5)    0xFFFA(-6)    0xFFF9(-7)
* Break between Slave 3(-2) and 4(-3) 
* number of slave on main: 3
* 0x0000(0)    0xFFFF(-1)    0xFFFE(-2)    0x0000( 0)    0xFFFF(-1)    0xFFFE(-2)    0xFFFD(-3)    0xFFFC(-4)
*                                                                    
* \return 
*/
EC_T_VOID CEcDevice::RedBreakFrameSplit
(   EC_T_DEVICE_FRAMEDESC*  pDeviceFrameDesc  /* [in]  device frame descriptor */ 
 )
{
EC_T_WORD  wCmdHdrLen  = 0;
EC_T_BYTE  byCmd       = 0;
EC_T_WORD  wAdp        = 0;
EC_T_SWORD sAdp        = 0;
PETYPE_EC_CMD_HEADER pMainCmdHeader = EC_NULL;
PETYPE_EC_CMD_HEADER pRedCmdHeader  = EC_NULL;

    /* source address first byte */
    pDeviceFrameDesc->oLinkFrameDesc.pbyFrame[6] &= ~8;

    /* source address last byte */
    if (IsSendEnabledOnMain() && IsSendEnabledOnRed())
    {
        m_byRedFrameId++;
        if (ETH_ADDRESS_CABLE_RED_FRAME_ID_LAST < m_byRedFrameId)
        {
            m_byRedFrameId = 0;
        }
        pDeviceFrameDesc->oLinkFrameDesc.pbyFrame[6+5] = m_byRedFrameId;
    }
    else
    {
        pDeviceFrameDesc->oLinkFrameDesc.pbyFrame[6+5] = ETH_ADDRESS_CABLE_RED_FRAME_ID_ONLY_ONE_LINE;
    }
    /* copy main frame into red frame */
    OsMemcpy(pDeviceFrameDesc->oRedLinkFrameDesc.pbyFrame, pDeviceFrameDesc->oLinkFrameDesc.pbyFrame, pDeviceFrameDesc->oLinkFrameDesc.dwSize);
    pDeviceFrameDesc->oRedLinkFrameDesc.dwSize = pDeviceFrameDesc->oLinkFrameDesc.dwSize;

    pDeviceFrameDesc->oRedLinkFrameDesc.pbyFrame[6] |=  8;

    /* process commands */
    pMainCmdHeader = &(((PETHERNET_88A4_HEADER)EC_ENDOF(FRAMETYPE_PTR(pDeviceFrameDesc->oLinkFrameDesc.pbyFrame)))->sETECAT.FirstEcCmdHeader);
    pRedCmdHeader  = &(((PETHERNET_88A4_HEADER)EC_ENDOF(FRAMETYPE_PTR(pDeviceFrameDesc->oRedLinkFrameDesc.pbyFrame)))->sETECAT.FirstEcCmdHeader);

    /* walk through the commands */
    while (EC_NULL != pMainCmdHeader)
    {
        /* get common information */
        byCmd      = pMainCmdHeader->uCmdIdx.swCmdIdx.byCmd;
        wCmdHdrLen = EC_CMDHDRLEN_GET_LEN(&(pMainCmdHeader->uLen));
        wAdp       = EC_ICMDHDR_GET_ADDR_ADP(pMainCmdHeader);
        sAdp       = (EC_T_SWORD)wAdp;

        /* common handling */
        switch (byCmd)
        {
        case EC_CMD_TYPE_APRD:
        case EC_CMD_TYPE_APWR:
        case EC_CMD_TYPE_APRW:
        case EC_CMD_TYPE_ARMW:
            {
                /* can this slave be read by the main line ? */
                if ((sAdp + m_pMaster->m_wRedNumSlavesOnMain) > 0)
                {
                    /* yes: slave on main line */
                    /* remove cmd from red frame */
                    pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
                }
                else
                {
                    /* no: slave on red line */
                    /* remove cmd from main frame */
                    pMainCmdHeader->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;

                    /* adjust auto inc address to match the desired slave */
                    wAdp =  EC_LOWORD(wAdp + m_pMaster->m_wRedNumSlavesOnMain);
                    EC_ICMDHDR_SET_ADDR_ADP(pRedCmdHeader, wAdp);
                }
            } break;
        default:
            break;
        }
#if (defined INCLUDE_DC_SUPPORT)
        /* handle DC system time distribution */
		if ((byCmd == EC_CMD_TYPE_FRMW) && (ECREG_SYSTEMTIME == EC_ICMDHDR_GET_ADDR_ADO(pMainCmdHeader)))
		{
            if (m_pMaster->m_bRedLineBreak)
            {
                if (  (m_pMaster->m_pDistributedClks->m_pBusSlaveRefClock == EC_NULL) 
                   || (m_pMaster->m_pDistributedClks->m_pBusSlaveRefClock->GetBusIndex() < m_pMaster->m_wRedNumSlavesOnMain))
                {
		            if (EC_NULL != m_pMaster->m_pDistributedClks->m_pRedBusSlaveRefClock)
                    {
                    EC_T_BOOL bBusTimeZero = EC_FALSE;

                        /* activate command */
                        EC_ICMDHDR_SET_ADDR_ADP(pRedCmdHeader, m_pMaster->m_pDistributedClks->m_pRedBusSlaveRefClock->GetFixedAddress());

                        /* apply the redundancy propagation delay on the estimated next bus time */
                        if (8 == wCmdHdrLen)
                        {
                        EC_T_UINT64 qwBusTime = EC_GET_FRM_QWORD((EC_T_PBYTE)EC_ENDOF(pRedCmdHeader));

                            if (0 != qwBusTime)
                            {
                                qwBusTime += m_pMaster->m_pDistributedClks->m_nRedPropagDelay;
	                            EC_SET_FRM_QWORD((EC_T_PBYTE)EC_ENDOF(pRedCmdHeader), qwBusTime);
                            }
                            else
                            {
                                bBusTimeZero = EC_TRUE;
                            }
                        }
                        else
                        {
                        EC_T_DWORD dwBusTime = EC_GET_FRM_DWORD((EC_T_PBYTE)EC_ENDOF(pRedCmdHeader));

                            if (0 != dwBusTime)
                            {
                                dwBusTime += m_pMaster->m_pDistributedClks->m_nRedPropagDelay;
	                            EC_SET_FRM_DWORD((EC_T_PBYTE)EC_ENDOF(pRedCmdHeader), dwBusTime);
                            }
                            else
                            {
                                bBusTimeZero = EC_TRUE;
                            }
                        }
                        if (bBusTimeZero)
                        {
                            m_pMaster->m_pDistributedClks->m_bRedSynchronizeBusTime = EC_FALSE;
                        }
                        else
                        {
                            m_pMaster->m_pDistributedClks->m_bRedSynchronizeBusTime = !m_pMaster->m_pDistributedClks->m_bRedSynchronizeBusTime;
                        }
                        if (m_pMaster->m_pDistributedClks->m_bRedSynchronizeBusTime)
                        {
                            pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
                        }
                        else
                        {
                            pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FRMW;
                        }
                    }
                    else
                    {
                        /* deactivate command */
                        pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd  = EC_CMD_TYPE_NOP;
                    }
                }
            }
            else
            {
                /* deactivate command */
                pRedCmdHeader->uCmdIdx.swCmdIdx.byCmd  = EC_CMD_TYPE_NOP;
            }
        }
#endif /* INCLUDE_DC_SUPPORT */
        /* get next command */
        if (0 != EC_CMDHDRLEN_GET_NEXT(&(pMainCmdHeader->uLen)))
        {
            pMainCmdHeader = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pMainCmdHeader + wCmdHdrLen + ETYPE_EC_OVERHEAD);
            pRedCmdHeader  = (PETYPE_EC_CMD_HEADER)((EC_T_BYTE*)pRedCmdHeader  + wCmdHdrLen + ETYPE_EC_OVERHEAD);
        }
        else
        {
            pMainCmdHeader = EC_NULL;
            pRedCmdHeader  = EC_NULL;
        }
    }
}


/********************************************************************************/
/** \brief Redundancy: Process queued frames
*
* \return 
*/
EC_T_VOID CEcDevice::RedProcessQueuedFrames
(EC_T_BOOL       bIsRedFrame
)
{
    EC_RED_UNPROC_T* pUnprocData = EC_NULL;

    /* select redundancy queue of the own line */
    if (bIsRedFrame)
    {
        pUnprocData = &m_scRedLineUnproc;
    }
    else
    {
        pUnprocData = &m_scMainLineUnproc;
    }
    /* handle all frames in the queue */
    while (pUnprocData->dwNum != 0)
    {
        PETHERNET_FRAME pUnprocFrame = (PETHERNET_FRAME)&pUnprocData->pbyFrames[pUnprocData->dwTailIdx*ETHERNET_MAX_FRAMEBUF_LEN];

        /* drop unprocessed frames */
        if ((pUnprocFrame->Source.b[0] & 0x2) == 0x2)
        {
            if (bIsRedFrame)
            {
                m_pMaster->CheckFrame(EC_NULL, pUnprocFrame);
            }
            else
            {
                m_pMaster->CheckFrame(pUnprocFrame, EC_NULL);
            }
        }        
        /* refresh queue control */
        pUnprocData->abValid[pUnprocData->dwTailIdx] = EC_FALSE;
        pUnprocData->dwTailIdx = (pUnprocData->dwTailIdx+1)%MAX_UNPROC_FRAMES;
        pUnprocData->dwNum--;
    }
}


#endif


/*-END OF SOURCE FILE--------------------------------------------------------*/

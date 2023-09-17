/*-----------------------------------------------------------------------------
 * EcDeviceBase.h           header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description				interface for the CEcDeviceBase class.
 *---------------------------------------------------------------------------*/

#ifndef INC_ECDEVICEBASE
#define INC_ECDEVICEBASE

/*-INCLUDES------------------------------------------------------------------*/
#include "EcList.h"

/*-FORWARD DECLARATIONS------------------------------------------------------*/
class CEcMaster;

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef CList<EC_T_BYTE*, EC_T_BYTE*>::CNode* PFRAMEBUF_NODE;

#ifdef FRAME_LOSS_SIMULATION
typedef enum _EC_T_FRAME_LOSS_SIMULATION_MODE
{
    eFrameLossSim_OFF = 0,      /*< frame loss simulation turned off */
    eFrameLossSim_RANDOM,       /*< random frame loss */
    eFrameLossSim_FIXED,        /*< fixed (pre-determined) frame loss */
    eFrameLossSim_MIXED,        /*< combination of fixed (pre-determined) and random frame loss (or-ed) */

    /* Borland C++ datatype alignment correction */
    eFrameLossSim_BCppDummy   = 0xFFFFFFFF
} EC_T_FRAME_LOSS_SIMULATION_MODE;

/*- CLASSES -----------------------------------------------------------------*/
/******************************************************************************
* frame loss simulation class
*/
class CEcFrameLoss
{
public:
    CEcFrameLoss();
    ~CEcFrameLoss();
    EC_T_VOID SetFrameLossSimulation(EC_T_DWORD dwNumGoodFramesAfterStart, EC_T_DWORD dwFrameLossLikelihoodPpm, EC_T_DWORD dwFixedLossNumGoodFrames, EC_T_DWORD dwFixedLossNumLostFrames);
    EC_T_BOOL IsThisFrameLost(EC_T_VOID);
    EC_T_FRAME_LOSS_SIMULATION_MODE m_EFrameLossSimulationMode;     /* frame loss simulation mode */

private:
    EC_T_DWORD                      m_dwNumGoodFramesAfterStart;    /* number of good frames before frame loss simulation starts */
    EC_T_DWORD                      m_dwFrameLossLikelihoodPpm;     /* random loss simulation: frame loss likelihood (ppm) */
    EC_T_DWORD                      m_dwFixedLossNumGoodFrames;     /* fixed loss simulation: number of good frames before frame loss */
    EC_T_DWORD                      m_dwFixedLossNumLostFrames;     /* fixed loss simulation: number of lost frames after processing the good ones */
    EC_T_DWORD                      m_dwFrameCounter;               /* frame counter */
    EC_T_DWORD                      m_dwLostFrameCounter;           /* lost frames counter */
};
#endif

/********************************************************************************/
/** \brief Base Class for communicating with a network adapter
*/
class CEcDeviceBase
{
/* friend classes for public inline functions referencing protected members */
friend class CEcDevice;
friend class CEcRedDevice;

public:
#if (defined INSTRUMENT_MASTER)
    CEcDeviceBase() {};
#endif
    CEcDeviceBase(EC_T_BOOL bIsRedDevice);
    virtual             ~CEcDeviceBase();
    
    /*---------------------------------------------------------------------------*/
    virtual EC_T_DWORD  OpenDevice(EC_T_VOID* pParms) = 0;
    virtual EC_T_DWORD  CloseDevice(EC_T_BOOL bDeinitForConfiguration) = 0;
    
    
    CEcMaster*          GetMaster() {return (CEcMaster*)m_pMaster;}
#if (defined INSTRUMENT_MASTER)
    EC_T_VOID           SetMaster(CEcMaster* pMaster) { m_pMaster = pMaster; }
#endif

    /* Returns the MAC address of the EtherCAT master. */
    ETHERNET_ADDRESS    GetMacAddress() {return m_oLinkMacAddress;}
    
    /* deprecated */
    CEcMaster*          GetMasterObject() {return m_pMaster;}

#ifdef FRAME_LOSS_SIMULATION
    EC_T_VOID           SetTxFrameLossSimulation(EC_T_DWORD dwNumGoodFramesAfterStart, EC_T_DWORD dwFrameLossLikelihoodPpm,
                                                 EC_T_DWORD dwFixedLossNumGoodFrames, EC_T_DWORD dwFixedLossNumLostFrames)
    {
        m_oTxFrameLoss.SetFrameLossSimulation(dwNumGoodFramesAfterStart, dwFrameLossLikelihoodPpm, dwFixedLossNumGoodFrames, dwFixedLossNumLostFrames);
    }
#endif

    virtual EC_T_LINKSTATUS     LinkGetStatus(EC_T_VOID) {return (m_bSendEnabled ? m_LinkDrvDesc.pfEcLinkGetStatus(m_pvLinkInstance) : eLinkStatus_DISCONNECTED);}
    virtual EC_T_DWORD          LinkGetSpeed(EC_T_VOID)  {return m_LinkDrvDesc.pfEcLinkGetSpeed(m_pvLinkInstance);}
    virtual EC_T_LINKMODE       LinkGetMode(EC_T_VOID)   {return m_LinkDrvDesc.pfEcLinkGetMode(m_pvLinkInstance);}
    EC_T_DWORD          LinkIoctl(EC_T_DWORD dwCode, EC_T_LINK_IOCTLPARMS* pParms)
    {
        /* common Link Layer IoCtl */
        if (EC_LINKIOCTL_SET_LINKENABLED == dwCode)
        {
            if (pParms->dwInBufSize < sizeof(EC_T_BYTE)) return EC_E_INVALIDPARM;

            switch (*pParms->pbyInBuf)
            {
            case EC_LINKENABLED_OFF: m_bReceiveEnabled = EC_FALSE; m_bSendEnabled = EC_FALSE; break;
            case EC_LINKENABLED_ON: m_bReceiveEnabled = EC_TRUE; m_bSendEnabled = EC_TRUE; break;
            case EC_LINKENABLED_ONLY_SEND: m_bReceiveEnabled = EC_FALSE; m_bSendEnabled = EC_TRUE; break;
            case EC_LINKENABLED_ONLY_RECEIVE: m_bReceiveEnabled = EC_TRUE; m_bSendEnabled = EC_FALSE; break;
            default: return EC_E_INVALIDPARM;
            }

            return EC_E_NOERROR;
        }
        if (EC_LINKIOCTL_GET_SPEED == dwCode)
        {
            EC_T_DWORD dwLinkSpeed = 0;
            if (pParms->dwOutBufSize < sizeof(EC_T_DWORD)) return EC_E_INVALIDPARM;

            dwLinkSpeed = LinkGetSpeed();
            EC_SETDWORD(pParms->pbyOutBuf, dwLinkSpeed);

            if (EC_NULL != pParms->pdwNumOutData) EC_SETDWORD(pParms->pdwNumOutData, sizeof(EC_T_DWORD));

            return EC_E_NOERROR;
        }

        /* dispatch IoCtl to Link Layer */
        return ((EC_NULL == m_LinkDrvDesc.pfEcLinkIoctl) ? EC_E_NOTFOUND : m_LinkDrvDesc.pfEcLinkIoctl(m_pvLinkInstance, dwCode, pParms));
    }
   
    /*-CEcDeviceBase--------------------------------------------------------------------*/
    virtual EC_T_DWORD  OpenDeviceBase(EC_T_VOID* pParms, EC_T_VOID* pvContext, EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback);
    EC_T_DWORD          CloseDeviceBase(EC_T_BOOL bDeinitForConfiguration);
    EC_T_VOID           SetMacAddress(ETHERNET_ADDRESS macAddress);
    static EC_T_BOOL    LLDbgMsgHook(const EC_T_CHAR* szFormat, ...);
    static EC_T_BOOL    LLDbgMsgHookVaArgs(const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs);

    virtual EC_T_LINKSTATUS RefreshLinkStatus(EC_T_VOID);
    EC_T_VOID           ForceLinkStatus(EC_T_LINKSTATUS eLinkStatus);
    EC_T_BOOL           IsLinkStatusForced(EC_T_VOID)
    {
        return  eLinkStatus_UNDEFINED != m_ELinkStatusForced;
    }
    EC_T_CHAR           m_szDriverIdent[MAX_DRIVER_IDENT_LEN];    /* driver identification string (zero terminated) */

    EC_T_DWORD          HandleLinkPolling(EC_T_VOID);
    virtual EC_T_DWORD  DeviceFlushRecvFrames(EC_T_VOID);
    virtual EC_T_DWORD  RecvFrame(EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
    {
        if (!m_bReceiveEnabled)
        {
            pLinkFrameDesc->dwSize = 0;
            return EC_E_NOERROR;
        }
        return m_LinkDrvDesc.pfEcLinkRecvFrame(m_pvLinkInstance, pLinkFrameDesc);
    }
    virtual EC_T_VOID   FreeRecvFrame(EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
    {
        m_LinkDrvDesc.pfEcLinkFreeRecvFrame(m_pvLinkInstance, pLinkFrameDesc);
    }
    EC_T_DWORD          FrameAlloc(EC_T_LINK_FRAMEDESC* pLinkframeDesc, EC_T_DWORD dwSize);
    EC_T_DWORD          FrameSendAndFree(EC_T_LINK_FRAMEDESC*    pLinkframeDesc, 
                                        EC_T_FRAME_TYPE         EFrameType, 
#if (defined INCLUDE_MASTER_OBD)
                                        EC_T_DWORD              dwNumCmdsInFrame, 
#endif
                                        EC_T_BOOL               bIsDbgFrame = EC_FALSE );
    virtual EC_T_VOID   FrameFree(EC_T_LINK_FRAMEDESC* pLinkFrameDesc)
    {
        if (m_bLinkLayerFrameAllocSupported) 
        {
            m_LinkDrvDesc.pfEcLinkFreeSendFrame(m_pvLinkInstance, pLinkFrameDesc);
        }
    }

    EC_T_VOID           SetFramesType(EC_T_BOOL bCyclic)
    {
        if (m_bIsFramesTypeRequired)
        {
            LinkIoctl((bCyclic ? EC_LINKIOCTL_SENDCYCLICFRAMES : EC_LINKIOCTL_SENDACYCLICFRAMES), EC_NULL);
        }
    }

    EC_T_VOID           FlushFrames()
    {
        if (m_bIsFlushFramesRequired)
        {
           LinkIoctl(EC_LINKIOCTL_FLUSHFRAMES, EC_NULL);
        }
    }

protected:
    virtual EC_T_VOID   ReceiveFrameCallback(EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed)=0;    

    EC_T_LINKSTATUS     GetLinkStatus(EC_T_VOID){return m_ELinkStatus;}
    EC_T_BOOL           IsLinkConnected(EC_T_VOID)
    {
        return m_bSendEnabled && (eLinkStatus_OK == m_ELinkStatus);
    }

protected:
    EC_T_BOOL           m_bOpened;
    
#if (defined INCLUDE_RED_DEVICE)
    EC_T_BOOL           m_bIsRedDevice;
#endif
    EC_T_VOID*          m_pvLinkInstance;
    
EC_INSTRUMENT_MOCKABLE_VAR
    CEcMaster*          m_pMaster;

    ETHERNET_ADDRESS    m_oLinkMacAddress;
    EC_T_WORD           m_wRes;                 /* ETHERNET_ADDRESS is 6 byte, add 2 bytes to assure 32-bit alignment */
    EC_T_BOOL           m_bLinkLayerFrameAllocSupported;
    EC_T_BOOL           m_bIsFramesTypeRequired;
    EC_T_BOOL           m_bIsFlushFramesRequired;
    EC_T_LINKMODE       m_eLinkMode;
    EC_T_LINKSTATUS     m_ELinkStatus;          /* link status */
    EC_T_LINKSTATUS     m_ELinkStatusForced;    /* forced link status, used for example in case of redundancy */
    EC_T_BOOL           m_bReceiveEnabled;
    EC_T_BOOL           m_bSendEnabled;

#if (defined INCLUDE_MASTER_OBD)
    EC_T_DWORD*         m_pdwTxFrameCrt;
    EC_T_DWORD*         m_pdwRxFrameCrt;
    EC_T_DWORD*         m_pdwCycFrameCrt;
    EC_T_DWORD*         m_pdwCycDgramCrt;
    EC_T_DWORD*         m_pdwAcycFrameCrt;
    EC_T_DWORD*         m_pdwAcycDgramCrt;
    EC_T_DWORD*         m_pdwTxBytesSecondCrt;
    EC_T_DWORD*         m_pdwTxBytesCycleCrt;
#endif

private:
#if (defined FRAME_LOSS_SIMULATION)
    CEcFrameLoss        m_oTxFrameLoss;         /* transmit frame loss object */
#endif
public:
    EC_T_LINK_DRV_DESC  m_LinkDrvDesc;          /* link layer driver descriptor */
};

#endif /* INC_ECDEVICEBASE */

/*-END OF SOURCE FILE--------------------------------------------------------*/

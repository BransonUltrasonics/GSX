/*-----------------------------------------------------------------------------
 * EcDevice.h               header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description				interface for the CEcDevice class.
 *---------------------------------------------------------------------------*/

#ifndef INC_ECDEVICE
#define INC_ECDEVICE

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECDEVICEBASE
#include "EcDeviceBase.h"
#endif

#if (defined INCLUDE_RED_DEVICE)
#ifndef INC_ECREDDEVICE
#include "EcRedDevice.h"
#endif
#endif

#ifndef INC_ECIOIMAGE
#include "EcIoImage.h"
#endif
#include "EcFiFo.h"
#ifndef INC_ECTIMER
#include "EcTimer.h"
#endif

/*-DEFINES/MACROS------------------------------------------------------------*/
#define MAX_UNPROC_FRAMES 64            /* 2 x 64 x 1,5 kB = 192 kB */


/*-TYPEDEFS/ENUMS------------------------------------------------------------*/

/* insert sdo class here for the moment*/
typedef struct TCANopenCmd
{
    ETHERCAT_CANOPEN_HEADER head;     /* head for the string command */
} CANopenCmd, *PCANopenCmd;


typedef struct  
{
    EC_T_DWORD      dwNum;            /* number of unprocessed frames */
    EC_T_DWORD      dwHeadIdx;        /* where to store the next received unprocessed frames */
    EC_T_DWORD      dwTailIdx;        /* which unprocessed frame shall be processed next */
    EC_T_BOOL       abValid[MAX_UNPROC_FRAMES];
    EC_T_BYTE       abyIdx[MAX_UNPROC_FRAMES];
    EC_T_BYTE       *pbyFrames;
} EC_RED_UNPROC_T;

/* mailbox callback function pointer type */
/*typedef EC_T_VOID (*MBX_CALLBACK)(EC_T_DWORD     dwInstanceID, PEcMailboxCmd);*/
typedef EC_T_DWORD (*MBX_CALLBACK)(
                                   EC_T_DWORD        dwInstanceID
                                  ,PEcMailboxCmd     pCmd
                                  ,EC_T_BYTE*        pbyMbxData
                                  ,EC_T_DWORD        dwMbxDataLength
                                  ,EC_T_DWORD        dwMbxResult         );

/* notification callback function pointer type */
typedef EC_T_DWORD (*NOTIFY_CALLBACK)(EC_T_DWORD  dwInstanceID, EC_T_DWORD dwClientID, EC_T_DWORD dwCode, PEC_T_NOTIFYPARMS pNotifyParams);

/* EcDevice constructor parameters */
typedef struct _EC_DEVICE_CTOR_PARMS
{
    struct _EC_T_MASTER_CONFIG* pMasterConfig;
    struct _EC_T_MEMORY_LOGGER* pMLog;
} EC_DEVICE_CTOR_PARMS;


/********************************************************************************/
/** \brief Class for communicating with a network adapter
*/
class CEcDevice : public CEcDeviceBase
{
public:
#if (defined INSTRUMENT_MASTER)
    CEcDevice() {}
#endif
    CEcDevice(struct _EC_DEVICE_CTOR_PARMS* pParms);
    virtual ~CEcDevice();

    /*-IEthDevice----------------------------------------------------------*/
    EC_T_VOID   LinkStateChngMsg(EC_T_DWORD dwLvl, const EC_T_CHAR* szFormat, ...);
    EC_T_VOID   LinkErrorMsg(const EC_T_CHAR* szFormat, ...);
    EC_T_VOID   LinkDbgMsg(EC_T_DWORD dwLvl, EC_T_BYTE byEthTyp0, EC_T_BYTE byEthTyp1, const EC_T_CHAR* szFormat, ...);
#if (defined INCLUDE_EOE_SUPPORT)
    CEcEthSwitch* EthernetSwitch();
#endif
    EC_T_BYTE*  ProcessDataPtr(EC_T_INT nInOut, EC_T_INT nOffs, EC_T_INT nSize);
    EC_T_INT    ProcessDataSize(EC_T_INT nInOut); 

    /*--------------------------------------------------------------------*/
    virtual EC_T_DWORD OpenDevice(EC_T_VOID* pParms);
    virtual EC_T_DWORD CloseDevice(EC_T_BOOL bDeinitForConfiguration);
    
#ifdef FRAME_LOSS_SIMULATION
    EC_T_VOID  SetRxFrameLossSimulation( EC_T_DWORD dwNumGoodFramesAfterStart, EC_T_DWORD dwFrameLossLikelihoodPpm, EC_T_DWORD dwFixedLossNumGoodFrames, EC_T_DWORD dwFixedLossNumLostFrames)
    {
        m_oRxFrameLoss.SetFrameLossSimulation(dwNumGoodFramesAfterStart, dwFrameLossLikelihoodPpm, dwFixedLossNumGoodFrames, dwFixedLossNumLostFrames);
    }
    EC_T_VOID  SetTxFrameLossSimulation( EC_T_DWORD dwNumGoodFramesAfterStart, EC_T_DWORD dwFrameLossLikelihoodPpm, EC_T_DWORD dwFixedLossNumGoodFrames, EC_T_DWORD dwFixedLossNumLostFrames)
    {
        m_oTxFrameLoss.SetFrameLossSimulation(dwNumGoodFramesAfterStart, dwFrameLossLikelihoodPpm, dwFixedLossNumGoodFrames, dwFixedLossNumLostFrames);
    }
#endif

#if (defined INCLUDE_EOE_SUPPORT)
    CEcEthSwitch* GetReferenceToSwitch(EC_T_VOID) { return m_pSwitch; }
#endif

    /*-IEcDevice----------------------------------------------------------*/

#if (defined INCLUDE_RED_DEVICE)
    EC_T_DWORD AddRedDevice(CEcRedDevice* poEcRedDevice);
#endif

    EC_T_DWORD DeviceFrameDescCreate(EC_T_DEVICE_FRAMEDESC* pDeviceFrameDesc,
                                     EC_T_BOOL  bCopyFrame,
                                     EC_T_BYTE* pbyFrame, 
#ifdef INCLUDE_RED_DEVICE
                                     EC_T_BYTE* pbyRedFrame,
#endif
                                     EC_T_DWORD dwSize);
    EC_T_BOOL  DeviceQueueFrame(struct _ECAT_SLAVEFRAME_DESC* pSlvFrmDesc) {return m_pDeviceFrameDescFifo->Add(pSlvFrmDesc); }
    EC_T_DWORD DeviceFrameSend(EC_T_LINK_FRAMEDESC* pLinkFrameDesc) 
    { 
        if (!m_bSendEnabled) return EC_E_NOERROR;
        return m_LinkDrvDesc.pfEcLinkSendFrame(m_pvLinkInstance, pLinkFrameDesc); 
    }
    EC_T_VOID  DeviceFrameFree(EC_T_DEVICE_FRAMEDESC* pDeviceFrameDesc);
    EC_T_DWORD DeviceFrameSendAndFree(EC_T_DEVICE_FRAMEDESC* pDeviceFrameDesc);
    EC_T_VOID  ProcessFrame(EC_T_VOID* pvFrame, EC_T_DWORD dwSize, EC_T_BOOL bIsRedFrame, EC_T_BOOL* pbFrameProcessed);
    
    /*-CEcDevice----------------------------------------------------------*/

    CEcMaster* DeviceCreateMaster(  PEcMasterDesc pDesc,
                                            MBX_CALLBACK pfMbCallback, NOTIFY_CALLBACK pfNotifyCallback, 
                                            struct _EC_T_MASTER_CONFIG* pMasterConfig, struct _EC_T_MASTER_CONFIG_EX* pMasterConfigEx, 
                                            struct _EC_T_INTFEATUREFIELD_DESC* pFeatures, EC_T_DWORD dwVersion  );
#if (defined INCLUDE_EOE_SUPPORT)
    EC_T_DWORD CreateSwitch(EthernetCreateSwitch *pCreate);
#endif
    EC_T_DWORD CreateImage(EC_T_DWORD nIn, EC_T_DWORD nOut);
    EC_T_VOID  DbgMsg(EC_T_BOOL bPrintMsg, EC_T_BOOL bSendMsgFrame, EC_T_BYTE byEthTyp0, EC_T_BYTE byEthTyp1, const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs);
    EC_T_DWORD SendQueuedFrames(EC_T_WORD* pwNumSendFrames);
    EC_T_VOID  CancelQueuedFrames(EC_T_DWORD dwCmdResult);
    EC_T_VOID  SetFramesType(EC_T_BOOL bCyclic)
    {
        CEcDeviceBase::SetFramesType(bCyclic);

#ifdef INCLUDE_RED_DEVICE
        if (m_bRedDeviceEnabled)
        {
            if (IsSendEnabledOnRed())
            {
                m_pRedDevice->SetFramesType(bCyclic);
            }
        }
#endif /* INCLUDE_RED_DEVICE */
    };

    EC_T_VOID  FlushFrames()
    {
        CEcDeviceBase::FlushFrames();

#ifdef INCLUDE_RED_DEVICE
        if (m_bRedDeviceEnabled)
        {
            if (IsSendEnabledOnRed())
            {
                m_pRedDevice->FlushFrames();
            }
        }
#endif /* INCLUDE_RED_DEVICE */
    };

    EC_T_LINKSTATUS RefreshLinkStatus(EC_T_VOID);
    EC_T_LINKSTATUS GetLinkStatus(EC_T_VOID);  
    EC_T_DWORD GetCheckCounter(EC_T_VOID)       {return m_dwCheckCounter;}
    EC_T_BOOL  IsBulkInLinkLayerSupported(EC_T_VOID)
    {
        return (EC_E_NOERROR == LinkIoctl(EC_LINKIOCTL_IS_REPEAT_CNT_SUPPORTED, EC_NULL));
    }
#if (defined INCLUDE_RED_DEVICE)
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_BOOL IsMainLinkConnected(EC_T_VOID){return CEcDeviceBase::IsLinkConnected();}
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_BOOL IsRedLinkConnected(EC_T_VOID) {return (m_bRedDeviceEnabled && m_pRedDevice->IsLinkConnected());}
    EC_T_BOOL  IsLinkConnected(EC_T_VOID)       {return (IsMainLinkConnected() || IsRedLinkConnected());}
    EC_T_BOOL  IsSendEnabledOnMain(EC_T_VOID);
    EC_T_BOOL  IsSendEnabledOnRed(EC_T_VOID);
    EC_T_BOOL  IsSendEnabled(EC_T_VOID)         {return (IsSendEnabledOnMain() || IsSendEnabledOnRed());}
#else
    EC_T_BOOL  IsMainLinkConnected(EC_T_VOID)   {return CEcDeviceBase::IsLinkConnected();}
    EC_T_BOOL  IsLinkConnected(EC_T_VOID)       {return IsMainLinkConnected();}
    EC_T_BOOL  IsSendEnabledOnMain(EC_T_VOID);
    EC_T_BOOL  IsSendEnabled(EC_T_VOID)         {return IsSendEnabledOnMain();}
#endif
    EC_T_BOOL  IsLicenseValid(EC_T_VOID)        {return !m_bInvalidLicense;};

    struct _EC_T_MEMORY_LOGGER* GetMemLog() { return m_pMLog; }
    
protected:
    virtual EC_T_VOID ReceiveFrameCallback(EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed);
    static  EC_T_DWORD ReceiveFrameCallbackGateway(EC_T_VOID* pvContext, EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed);
	
#if (defined INCLUDE_RED_DEVICE)
    EC_T_VOID  RedBreakFrameSplit( EC_T_DEVICE_FRAMEDESC*  pDeviceFrameDesc);
    EC_T_VOID  RedProcessQueuedFrames(EC_T_BOOL bIsRedFrame);
#endif

protected:
    CEcIoImage*         m_pIOImage;
    CFiFoListDyn<struct _ECAT_SLAVEFRAME_DESC*>* m_pDeviceFrameDescFifo;
    EC_T_BYTE*          m_pbyDbgMsg;
    EC_T_VOID*          m_poFrameDescLock;
#if (defined INCLUDE_EOE_SUPPORT)
    CEcEthSwitch*       m_pSwitch;
#endif
private:
    EC_T_LINKSTATUS     m_ELinkStatusNotified;          /* last notified link status */
#if (defined INCLUDE_RED_DEVICE)
public:
    EC_T_BOOL           m_bRedConfigured;
    EC_T_BOOL           m_bRedDeviceEnabled;
    CEcRedDevice*       m_pRedDevice;

private:
    EC_T_BYTE           m_byRedFrameId;
    EC_RED_UNPROC_T     m_scMainLineUnproc;
    EC_T_DWORD          m_dwMainFrameLossCnt;
    EC_T_BOOL           m_bMainFrameLoss;    
    EC_RED_UNPROC_T     m_scRedLineUnproc;
    EC_T_DWORD          m_dwRedFrameLossCnt;
    EC_T_BOOL           m_bRedFrameLoss;    
    CEcTimer            m_oUnprocTimeout;
    EC_T_BOOL           m_bLinkConnected;
#endif

#if (defined ECWIN)
	EC_T_BOOL           m_dwEcWinJobDone;
	EC_T_BOOL           m_bEcWinChecked;
	EC_T_DWORD			m_dwEcWinTime;
#endif

#ifdef FRAME_LOSS_SIMULATION
    CEcFrameLoss        m_oRxFrameLoss;                 /* receive frame loss object */
#endif

    CEcTimer            m_tTimeout;
    EC_T_BOOL           m_bTimeRunning;
    EC_T_BOOL           m_bInvalidLicense;
    EC_T_BOOL           m_bChecked;
    EC_T_DWORD          m_dwCheckCounter;

    struct _EC_T_MEMORY_LOGGER* m_pMLog;
};

/********************************************************************************/
/** \brief Class with stubbed communicating API as main device or red device
*/
template<typename ClassType, typename ConstructorParmType>
class CEcDeviceStubTemplate : public ClassType
{
public:
    CEcDeviceStubTemplate(ConstructorParmType* pConstructorParm)
        : ClassType(pConstructorParm)
	{
    	this->m_bReceiveEnabled = EC_FALSE;
    	this->m_bSendEnabled = EC_FALSE;
	}

    virtual ~CEcDeviceStubTemplate() {}

    virtual EC_T_DWORD          OpenDeviceBase(EC_T_VOID*, EC_T_VOID*, EC_T_RECEIVEFRAMECALLBACK) {return EC_E_NOERROR;}

    virtual EC_T_DWORD          RecvFrame(EC_T_LINK_FRAMEDESC* pFrame) {pFrame->dwSize = 0; return EC_E_NOERROR;}
    virtual EC_T_VOID           FrameFree(EC_T_LINK_FRAMEDESC* ) {}
    virtual EC_T_DWORD          DeviceFlushRecvFrames(EC_T_VOID) {return EC_E_NOERROR;}
    virtual EC_T_LINKSTATUS     RefreshLinkStatus(EC_T_VOID) { return eLinkStatus_DISCONNECTED; }

    virtual EC_T_LINKSTATUS     LinkGetStatus(EC_T_VOID) {return eLinkStatus_DISCONNECTED;}
    virtual EC_T_DWORD          LinkGetSpeed(EC_T_VOID)  {return 100;}
    virtual EC_T_LINKMODE       LinkGetMode(EC_T_VOID)   {return this->m_eLinkMode;}
};

/********************************************************************************/
/** \brief Class with stubbed communicating API as main device
*/
class CEcDeviceStub : public CEcDeviceStubTemplate<CEcDevice, struct _EC_DEVICE_CTOR_PARMS>
{
public:
    CEcDeviceStub(struct _EC_DEVICE_CTOR_PARMS* pConstructorParm) : CEcDeviceStubTemplate<CEcDevice, struct _EC_DEVICE_CTOR_PARMS>(pConstructorParm) {}
    virtual ~CEcDeviceStub() {}
};

#if (defined INCLUDE_RED_DEVICE)
/********************************************************************************/
/** \brief Class with stubbed communicating API as red device
*/
class CEcRedDeviceStub : public CEcDeviceStubTemplate<CEcRedDevice, CEcDeviceBase>
{
public:
    CEcRedDeviceStub(CEcDeviceBase* pConstructorParm) : CEcDeviceStubTemplate<CEcRedDevice, CEcDeviceBase>(pConstructorParm) {}
    virtual ~CEcRedDeviceStub() {}
};
#endif

#endif /* INC_ECDEVICE */

/*-END OF SOURCE FILE--------------------------------------------------------*/

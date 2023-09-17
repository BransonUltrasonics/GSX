/*-----------------------------------------------------------------------------
 * Copyright    acontis technologies GmbH, Weingarten, Germany
 * Response     Paul Bussmann
 * Description  Scan and Store topology of connected EtherCAT Bus
 *---------------------------------------------------------------------------*/

#ifndef INC_ECBUSTOPOLOGY
#define INC_ECBUSTOPOLOGY 1

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECLINK
#include "EcLink.h"
#endif
#ifndef INC_ECSERVICES
#include "EcServices.h"
#endif
#ifndef INC_ECTIMER
#include "EcTimer.h"
#endif

#if (defined INCLUDE_HOTCONNECT)
#ifndef INC_ECHOTCONNECT
#include "EcHotConnect.h"
#endif
#endif
#include "EcHashTable.h"
#include "EcList.h"

/*-DEFINES-------------------------------------------------------------------*/
/* for development purpose only */
#undef  ECBT_PRINTBUSSLAVESLIST
#undef  ECBT_PRINTBUSSLAVESTREE

#define ECBT_TOPOCHANGE_AUTOMODE            EC_TRUE
#define ECBT_TOPOCHANGE_THRESHOLD_SLAVE     ((EC_T_DWORD)1)
#define ECBT_TOPOCHANGE_THRESHOLD_FRAMELOSS ((EC_T_DWORD)64)
#define ECBT_TOPOCHANGE_DELAY               ((EC_T_DWORD)1000)

#define ECBT_SLAVELIST_PORT                 ((EC_T_DWORD)0)
#if (defined INCLUDE_HOTCONNECT)
#define ECBT_SLAVELIST_HC                   ((EC_T_DWORD)1)
#endif
#define ECBT_SLAVELIST_BUSSLAVE             ((EC_T_DWORD)2)

#define ECBT_LINKPORT_LINKSTATUS            ((EC_T_WORD)0x0001)
#define ECBT_LINKPORT_LOOPSTATUS            ((EC_T_WORD)0x0002)
#define ECBT_LINKPORT_SIGNALDETECTION       ((EC_T_WORD)0x0004)
#define ECBT_LINKPORT_SLAVECONNECTED        ((EC_T_WORD)0x0008)
#define ECBT_LINKPORT_STATUS_MASK           ((EC_T_WORD)0x000F)
#define ECBT_LINKPORT_CONFIGURED            ((EC_T_WORD)0x0010)
#define ECBT_LINKPORT_SMPROCESSED           ((EC_T_WORD)0x0020)
#define ECBT_LINKPORT_SMPROCESSEDRED        ((EC_T_WORD)0x0040)
#define ECBT_LINKPORT_SMPROCESSED_MASK      ((EC_T_WORD)0x0060)
#define ECBT_LINKPORT_EL9010CONNECTED       ((EC_T_WORD)0x0080)
#define ECBT_LINKPORT_JUNCTIONRED_LINEBREAK ((EC_T_WORD)0x0100)
#define ECBT_LINKPORT_JUNCTIONRED_CHANGE    ((EC_T_WORD)0x0200)
#define ECBT_LINKPORT_RECV_TIME_MISSING     ((EC_T_WORD)0x0400)
#define ECBT_LINKPORT_FRAMELOSS_AFTER_SLAVE ((EC_T_WORD)0x0800)
#define ECBT_LINKPORT_BACKUPVALID           ((EC_T_WORD)0x8000)

#define ECBT_SLAVEFLAG_ISNEW                ((EC_T_WORD)0x0001)
#define ECBT_SLAVEFLAG_DCSMPROCESSED        ((EC_T_WORD)0x0002)
#define ECBT_SLAVEFLAG_IS_BRANCHING         ((EC_T_WORD)0x0004)
#define ECBT_SLAVEFLAG_LINECROSSED          ((EC_T_WORD)0x0008)
#define ECBT_SLAVEFLAG_PORTANOTCONNECTED    ((EC_T_WORD)0x0010)
#define ECBT_SLAVEFLAG_RECVTIMES_COHERENT   ((EC_T_WORD)0x0020)
#define ECBT_SLAVEFLAG_DLSTATUSEVENT        ((EC_T_WORD)0x0080)
#define ECBT_SLAVEFLAG_PDIWDEXPIRED         ((EC_T_WORD)0x0100)
#define ECBT_SLAVEFLAG_LINECROSSEDNOTIFIED  ((EC_T_WORD)0x0200)
#define ECBT_SLAVEFLAG_ECATEVENTSUPPORTED   ((EC_T_WORD)0x0400)
#define ECBT_SLAVEFLAG_AUTOCLOSESUPPORTED   ((EC_T_WORD)0x0800)
#define ECBT_SLAVEFLAG_DEVICE_EMULATION     ((EC_T_WORD)0x1000)
#define ECBT_SLAVEFLAG_DCUNIT_SUPPORTED     ((EC_T_WORD)0x2000)
#define ECBT_SLAVEFLAG_RECVTIMES_SUPPORTED  ((EC_T_WORD)0x4000)
#define ECBT_SLAVEFLAG_BOOTSTRAPSUPPORTED   ((EC_T_WORD)0x8000)

#define ECBT_ADDRRANGE_AMOUNT               ((EC_T_DWORD)0x00000005)


/*-MACROS--------------------------------------------------------------------*/
#define READEEPROMENTRY_COUNT 11
#define READEEPROMENTRY_IDX_VENDORID      0 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_PRODUCTCODE   1 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_REVISIONNO    2 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_SERIALNO      3 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_ALIASADDRESS  4 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_BOOTRCVMBX    5 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_BOOTSNDMBX    6 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_STDRCVMBX     7 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_STDSNDMBX     8 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_MBXPROTOCOL   9 /* use broadcast addressing */
#define READEEPROMENTRY_IDX_CATEGORIES   10 /* use direct    addressing */


/*-TYPEDEF-------------------------------------------------------------------*/
class CAtEmInterface;
class CEcMaster;
class CEcSlave;
typedef CList<class CEcSlave*, class CEcSlave*> CEcSlaveList;
class CEcDevice;
struct TECAT_SLAVECMD_DESC;

typedef struct _ECBT_T_ADDRRANGE
{
    EC_T_BOOL bRangeUsed;     /**<    is this range used */
    EC_T_WORD wStartAddr;     /**<    first address in range */
    EC_T_WORD wEndAddr;       /**<    last address in range */
    EC_T_WORD wWorkAddr;      /**<    next un-used address in range */
    EC_T_WORD wRsvd;          /**<    alignment safety */
} ECBT_T_ADDRRANGE, *ECBT_PT_ADDRRANGE;

/*-CLASS---------------------------------------------------------------------*/
class CEcBusTopology;
class CEcBusSlaveGenEni;

class CEcBusSlave
{
public:
    CEcBusSlave() {}
    virtual ~CEcBusSlave(){}

     virtual CEcBusSlaveGenEni* GetBusSlaveGenEni(EC_T_VOID                           )
                         { return EC_NULL; };
     virtual EC_T_VOID       InitInstance(        EC_T_VOID                           );

     CEcBusSlave*    GetNext(                     EC_T_VOID                           )
					     { return m_pNext; };                                        
     EC_T_VOID       SetNext(                     CEcBusSlave* pN                     )
					     { m_pNext = pN; };                                          
     
     CEcBusSlave*    GetPrev(                     EC_T_VOID                           )
					     { return m_pPrev; };                                        
     EC_T_VOID       SetPrev(                     CEcBusSlave* pP                     )
					     { m_pPrev = pP; };
     
     CEcBusSlave*    GetParentSlave(              EC_T_VOID                           )
                        { return ((m_wParentPort==ESC_PORT_INVALID)?EC_NULL:m_apBusSlaveChildren[m_wParentPort]); };
     EC_T_WORD       GetPortAtParent(             EC_T_VOID                           )
                        { return m_wPortAtParent; };

     EC_T_DWORD      GetBusIndex(                EC_T_VOID                           )
                        { return m_dwBusIndex; };                                    
     EC_T_VOID       SetBusIndex(                EC_T_DWORD val                      )  
                        { m_dwBusIndex = val; };
     EC_T_WORD       GetAutoIncAddress(          EC_T_VOID                           )
                        { return (EC_T_WORD)(0-EC_LOWORD(m_dwBusIndex)); }

     /* Temporary Fixed address, used during BT operation */
     EC_T_WORD       GetTmpFixedAddress(         EC_T_VOID                           )
                        { return m_wSlaveTmpFixAddr; }
     EC_T_VOID       SetTmpFixedAddress(         EC_T_WORD   wAddr                   )
                        { m_wSlaveTmpFixAddr = wAddr; }
                                                                                     
     EC_T_BOOL       AreDlInfoEqual(             EC_T_BYTE*  pbyVal                  );
     EC_T_VOID       SetDlInfo(                  EC_T_BYTE*  pbyVal                  );
     EC_T_VOID       BackupPortInfos(            EC_T_VOID                           );

     EC_T_BYTE       GetESCType(                 EC_T_VOID                           )
                        { return m_byType; }
     EC_T_BYTE       GetESCRevision(             EC_T_VOID                           )
                        { return m_byRevision; }
     EC_T_WORD       GetESCBuild(                EC_T_VOID                           )
                        { return m_wBuild; }
     EC_T_VOID       SetAutoCloseSupported(      EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_AUTOCLOSESUPPORTED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_AUTOCLOSESUPPORTED:0x00); }
     EC_T_BOOL       GetAutoCloseSupported(      EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_AUTOCLOSESUPPORTED)==ECBT_SLAVEFLAG_AUTOCLOSESUPPORTED); }
     EC_T_VOID       SetEcatEventSupported(      EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_ECATEVENTSUPPORTED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_ECATEVENTSUPPORTED:0x00); }
     EC_T_BOOL       GetEcatEventSupported(      EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_ECATEVENTSUPPORTED)==ECBT_SLAVEFLAG_ECATEVENTSUPPORTED); }
     EC_T_BOOL       GetBootStrapSupported(      EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_BOOTSTRAPSUPPORTED)==ECBT_SLAVEFLAG_BOOTSTRAPSUPPORTED); }

     EC_T_BOOL      UseDcLoopControlStdValues(   EC_T_VOID                           )
                        {
                            if ((m_byType >= ESCTYPE_NETX100_500) && (m_byType <= ESCTYPE_HILSCHER_RES5))
                            {
                                /* 2016-10-07 New Hilscher ESC support 0x800 */

                                /* 2018-04-13 Old Hilscher ESC only support standard values */
                                /* see EtherCAT_Slave_limitations_netx.pdf "7.1.9 Distributed Clock" */
                                /* if (m_wBuild < 20) { return EC_TRUE; } */

                                /* 2018-05-24 Build 21 is not working properly */
                                /* see EtherCAT_Slave_netx_6_50_51_52_90_100_500_4000_HAL_05_EN.pdf */
                                /* Other values than 0x1000 not supported */
                                return EC_TRUE;
                            }
                            return EC_FALSE;
                        }
     EC_T_BYTE       GetSupFmmuEntities(         EC_T_VOID                           )
                        { return m_bySupportedFmmuEntities; }
     EC_T_BYTE       GetSupSyncManEntities(      EC_T_VOID                           )
                        { return m_bySupportedSyncManchannels; }
     EC_T_BYTE       GetRamSize(                 EC_T_VOID                           )
                        { return m_byRAMSize; }
     EC_T_VOID       SetPortDescriptor(          EC_T_BYTE   byPortDescriptor        );
     EC_T_BYTE       GetPortDescriptor(          EC_T_VOID                           )
                        { return m_byPortDescriptor; }
     EC_T_BYTE       GetPhysicPortX(             EC_T_WORD   wPort                   )
                        { return (EC_T_BYTE)(m_byPortDescriptor >> (2 * wPort) & 0x03); }
     EC_T_VOID       SetDlControl(               EC_T_DWORD  dwDlControlVal          )
                        { m_dwDlControl = dwDlControlVal; }
     EC_T_DWORD      GetDlControl(               EC_T_VOID                           )
                        { return m_dwDlControl; }
     EC_T_WORD       GetDlStatus(                EC_T_VOID                           )
                        { return m_wDlStatus; }
     EC_T_BYTE       PortLoopControlX(           EC_T_WORD   wPort                   );
     EC_T_VOID       SetBranchingFlag(           EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_IS_BRANCHING; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_IS_BRANCHING:0x00); }
     EC_T_BOOL       IsBranching(                EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_IS_BRANCHING)==ECBT_SLAVEFLAG_IS_BRANCHING); }
     EC_T_WORD       GetFeaturesSupported(       EC_T_VOID                           )
                        { return m_wFeaturesSupported; }
     EC_T_BOOL       IsFmmuBitOperationSupport(  EC_T_VOID                           )
                        { return ((m_wFeaturesSupported & ECM_DLI_FMMUBITSUPPORT)==ECM_DLI_FMMUBITSUPPORT); }
#if (defined INCLUDE_DC_SUPPORT)
     EC_T_VOID       SetDcUnitSupported(         EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_DCUNIT_SUPPORTED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_DCUNIT_SUPPORTED:0x00); }
     EC_T_BOOL       IsDcUnitSupported(          EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_DCUNIT_SUPPORTED)==ECBT_SLAVEFLAG_DCUNIT_SUPPORTED); }
     EC_T_BOOL       IsDcEnabled(                EC_T_VOID                           );
     EC_T_VOID       SetDcInitialized(           EC_T_BOOL   bVal                    )
                        { m_bDcInitialized = bVal; }
     EC_T_BOOL       IsDcInitialized(            EC_T_VOID                           )
                        { return m_bDcInitialized; }
#endif /* INCLUDE_DC_SUPPORT */
     EC_T_VOID       SetRecvTimesSupported(      EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_RECVTIMES_SUPPORTED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_RECVTIMES_SUPPORTED:0x00); }
     EC_T_BOOL       IsRecvTimesSupported(       EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_RECVTIMES_SUPPORTED)==ECBT_SLAVEFLAG_RECVTIMES_SUPPORTED); }
    EC_T_VOID        SetRecvTimesCoherent(       EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_RECVTIMES_COHERENT; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_RECVTIMES_COHERENT:0x00); }
     EC_T_BOOL       AreRecvTimesCoherent(       EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_RECVTIMES_COHERENT)==ECBT_SLAVEFLAG_RECVTIMES_COHERENT); }
     EC_T_VOID       SetRecvTimeMissingPortX(    EC_T_WORD wPort, EC_T_BOOL bVal)
                        { m_awLinkPort[wPort] &= ~ECBT_LINKPORT_RECV_TIME_MISSING; m_awLinkPort[wPort] |= (bVal ? ECBT_LINKPORT_RECV_TIME_MISSING : 0x00); }
     EC_T_BOOL       IsRecvTimeMissingPortX(EC_T_WORD wPort)
                        { return (m_awLinkPort[wPort] & ECBT_LINKPORT_RECV_TIME_MISSING) == ECBT_LINKPORT_RECV_TIME_MISSING; }
     EC_T_BOOL       IsRecvTimesMissing(EC_T_VOID)
                        { return IsRecvTimeMissingPortX(ESC_PORT_B) || IsRecvTimeMissingPortX(ESC_PORT_C) || IsRecvTimeMissingPortX(ESC_PORT_A) || IsRecvTimeMissingPortX(ESC_PORT_D); }
     EC_T_BOOL       IsDCSupport(                EC_T_VOID                           )
                        { return ((m_wFeaturesSupported & ECM_DLI_DCSUPPORT)==ECM_DLI_DCSUPPORT); }
     EC_T_BOOL       IsDC64Support(              EC_T_VOID                           )
                        { return ((m_wFeaturesSupported & ECM_DLI_DC64SUPPORT)==ECM_DLI_DC64SUPPORT); }
     EC_T_BOOL       IsDeviceEmulation(          EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_DEVICE_EMULATION)==ECBT_SLAVEFLAG_DEVICE_EMULATION); }
     EC_T_BOOL       PdiWdExpired(               EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_PDIWDEXPIRED)==ECBT_SLAVEFLAG_PDIWDEXPIRED); }
     EC_T_VOID       ResetPdiWdExpired(          EC_T_VOID                           )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_PDIWDEXPIRED; }

     EC_T_VOID       SetEventRegVal(             EC_T_WORD   wVal                    )
                        { 
                            m_wFlagField       &= ~ECBT_SLAVEFLAG_DLSTATUSEVENT;
                            if (wVal & ECM_ECATEVENT_DLSTATUS)
                            {
                                m_wFlagField   |= ECBT_SLAVEFLAG_DLSTATUSEVENT;
                            }
                        }
     EC_T_BOOL       IsDLStatusEvent(            EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_DLSTATUSEVENT)==ECBT_SLAVEFLAG_DLSTATUSEVENT); }
     EC_T_VOID       ResetDLStatusEvent(         EC_T_VOID                           )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_DLSTATUSEVENT; }

     EC_T_VOID       SetLineCrossed(      EC_T_BOOL   bVal                           )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_LINECROSSED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_LINECROSSED:0x00); }
     EC_T_BOOL       IsLineCrossed(       EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_LINECROSSED)==ECBT_SLAVEFLAG_LINECROSSED); }

     EC_T_VOID       SetLineCrossedNotified(      EC_T_BOOL   bVal                           )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_LINECROSSEDNOTIFIED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_LINECROSSEDNOTIFIED:0x00); }
     EC_T_BOOL       IsLineCrossedNotified(       EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_LINECROSSEDNOTIFIED)==ECBT_SLAVEFLAG_LINECROSSEDNOTIFIED); }

     EC_T_VOID       SetPortANotConnected(       EC_T_BOOL   bVal                    )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_PORTANOTCONNECTED; m_wFlagField |= (bVal?ECBT_SLAVEFLAG_PORTANOTCONNECTED:0x00); }
     EC_T_BOOL       IsPortANotConnected(       EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_PORTANOTCONNECTED)==ECBT_SLAVEFLAG_PORTANOTCONNECTED); }

     EC_T_WORD       GetFixedAddress(            EC_T_VOID                           )
                        { return m_wStationFixedAddress; }
     EC_T_VOID       SetFixedAddress(            EC_T_WORD   wAddr                   )
                        { m_wStationFixedAddress = wAddr; }
                                                         
     EC_T_WORD       GetAliasAddress(            EC_T_VOID                           )
                        { return m_wStationAliasAddress; }
     EC_T_VOID       SetAliasAddress(            EC_T_WORD wAddress                  )
                        { m_wStationAliasAddress = wAddress; }
     EC_T_VOID       GetSlaveProp(               EC_T_SLAVE_PROP* pSlaveProp         );

     EC_T_WORD       GetIdentifyData(EC_T_VOID)
                         { return m_wIdentificationValue; }
     EC_T_BOOL       IsLinkPortX(                EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_LINKSTATUS) != 0); }
     EC_T_BOOL       IsLoopPortX(                EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_LOOPSTATUS) != 0); }
     EC_T_BOOL       IsSignalPortX(              EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_SIGNALDETECTION) != 0); }
     EC_T_BOOL       IsSlaveConPortX(            EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_SLAVECONNECTED) != 0); }
     EC_T_BOOL       IsSlaveCfgPortX(            EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_CONFIGURED) != 0); }
     
     EC_T_BOOL       IsBuLinkPortX(              EC_T_WORD   wPort                   )
                        { return ((m_awLinkPortBackup[wPort] & ECBT_LINKPORT_LINKSTATUS) != 0); }
     EC_T_BOOL       IsBuLoopPortX(              EC_T_WORD   wPort                   )
                        { return ((m_awLinkPortBackup[wPort] & ECBT_LINKPORT_LOOPSTATUS) != 0); }
     EC_T_BOOL       IsBuSignalPortX(            EC_T_WORD   wPort                   )
                        { return ((m_awLinkPortBackup[wPort] & ECBT_LINKPORT_SIGNALDETECTION) != 0); }
     EC_T_BOOL       IsBuSlaveConPortX(          EC_T_WORD   wPort                   )
                        { return ((m_awLinkPortBackup[wPort] & ECBT_LINKPORT_SLAVECONNECTED) != 0); }

     EC_T_BOOL       IsBackupValid(              EC_T_VOID                           )
                        { return ((m_awLinkPortBackup[ESC_PORT_A] & ECBT_LINKPORT_BACKUPVALID) != 0); }

     EC_T_BOOL       DetectPortChanges      (EC_T_VOID);
     EC_T_BOOL       DetectNewPortConnection(CEcMaster* pMaster);

     EC_T_WORD       GetPortState(               EC_T_VOID                           );
     
     /* State machine processed flag */
     EC_T_VOID       ResetPortsSmProc(           EC_T_VOID                           )
                        { 
                             for (EC_T_WORD wPort = 0; wPort < ESC_PORT_COUNT; wPort++)
                             {
                                 m_awLinkPort[wPort] &= ~ECBT_LINKPORT_SMPROCESSED_MASK;
                             }
                        }
     EC_T_VOID       SetPortSmProc(             EC_T_WORD   wPort                   )
                        { 
                             m_awLinkPort[wPort] &= ~ECBT_LINKPORT_SMPROCESSED; 
                             m_awLinkPort[wPort] |=  ECBT_LINKPORT_SMPROCESSED; 
                        }
     EC_T_BOOL       IsPortSmProc(              EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_SMPROCESSED) != 0); }
#if (defined INCLUDE_RED_DEVICE)
     EC_T_VOID       SetPortSmProcRed(          EC_T_WORD   wPort                   )
                        { 
                             m_awLinkPort[wPort] &= ~ECBT_LINKPORT_SMPROCESSEDRED; 
                             m_awLinkPort[wPort] |=  ECBT_LINKPORT_SMPROCESSEDRED; 
                        }
     EC_T_BOOL       IsPortSmProcRed(           EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_SMPROCESSEDRED) != 0); }
#endif /* INCLUDE_RED_DEVICE */

     EC_T_VOID       SetEL9010ConnectedX(       EC_T_WORD   wPort,
                                                EC_T_BOOL   bVal                    )
                        { m_awLinkPort[wPort] &= ~ECBT_LINKPORT_EL9010CONNECTED; m_awLinkPort[wPort] |= (bVal?ECBT_LINKPORT_EL9010CONNECTED:0x00); }
     EC_T_VOID       ResetEL9010Connected(      EC_T_VOID                           )
                        { 
                             for (EC_T_WORD wPort = 0; wPort < ESC_PORT_COUNT; wPort++)
                             {
                                 SetEL9010ConnectedX(wPort, EC_FALSE);
                             }
                        }
     EC_T_BOOL       IsEL9010ConnectedX(        EC_T_WORD   wPort                   )
                        { return ((m_awLinkPort[wPort] & ECBT_LINKPORT_EL9010CONNECTED)==ECBT_LINKPORT_EL9010CONNECTED); }

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
     EC_T_VOID       SetJunctionRedLineBreak(   EC_T_BOOL   bVal                    )
                        { m_awLinkPort[ESC_PORT_A] &= ~ECBT_LINKPORT_JUNCTIONRED_LINEBREAK; m_awLinkPort[ESC_PORT_A] |= (bVal?ECBT_LINKPORT_JUNCTIONRED_LINEBREAK:0x00); }
     EC_T_BOOL       IsJunctionRedLineBreak(    EC_T_VOID                           )
                        { return ((m_awLinkPort[ESC_PORT_A] & ECBT_LINKPORT_JUNCTIONRED_LINEBREAK)==ECBT_LINKPORT_JUNCTIONRED_LINEBREAK); }
     EC_T_VOID       SetJunctionRedChange(      EC_T_BOOL   bVal                    )
                        { m_awLinkPort[ESC_PORT_A] &= ~ECBT_LINKPORT_JUNCTIONRED_CHANGE; m_awLinkPort[ESC_PORT_A] |= (bVal?ECBT_LINKPORT_JUNCTIONRED_CHANGE:0x00); }
     EC_T_BOOL       IsJunctionRedChange(       EC_T_VOID                           )
                        { return ((m_awLinkPort[ESC_PORT_A] & ECBT_LINKPORT_JUNCTIONRED_CHANGE)==ECBT_LINKPORT_JUNCTIONRED_CHANGE); }
#endif /* INCLUDE_JUNCTION_REDUNDANCY */
#if (defined INCLUDE_RESCUE_SCAN)
     EC_T_VOID       SetFramelossAfterSlave(    EC_T_WORD   wPort,
                                                EC_T_BOOL   bVal                    )
                        { if (bVal) m_awLinkPort[wPort] |= ECBT_LINKPORT_FRAMELOSS_AFTER_SLAVE; else m_awLinkPort[wPort] &= ~ECBT_LINKPORT_FRAMELOSS_AFTER_SLAVE; }
#endif /* INCLUDE_RESCUE_SCAN */
     EC_T_BOOL       IsFramelossAfterSlave(EC_T_WORD   wPort)
                        { return (0 != (m_awLinkPort[wPort] & ECBT_LINKPORT_FRAMELOSS_AFTER_SLAVE)); }

     EC_T_WORD       CreateLinkPortStatusFromDlStatusX(
                                                EC_T_WORD wPort,
                                                EC_T_WORD wVal                      );
     EC_T_VOID       SetPortSetting(            EC_T_WORD wVal                      );
     EC_T_VOID       SetPdiControl(             EC_T_WORD wVal                      );
    
     EC_T_VOID       SetEcatEventMask(          EC_T_WORD   wEcatEventMask          )
                        { m_wEcatEventMask = wEcatEventMask; }
     EC_T_WORD       GetEcatEventMask(          EC_T_VOID                           )
                        { return m_wEcatEventMask; }

     EC_T_VOID       ResetIsNew(                EC_T_VOID                           )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_ISNEW; }
     EC_T_BOOL       IsNew(                     EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_ISNEW) == ECBT_SLAVEFLAG_ISNEW); }

     EC_T_VOID       SetDCSmProc(               EC_T_VOID                           )
                        { m_wFlagField |=  ECBT_SLAVEFLAG_DCSMPROCESSED; }
     EC_T_VOID       ResetDCSmProc(             EC_T_VOID                           )
                        { m_wFlagField &= ~ECBT_SLAVEFLAG_DCSMPROCESSED; }
     EC_T_BOOL       IsDCSmProc(                EC_T_VOID                           )
                        { return ((m_wFlagField & ECBT_SLAVEFLAG_DCSMPROCESSED) == ECBT_SLAVEFLAG_DCSMPROCESSED); }

     EC_T_VOID       GetSlaveInfo(               EC_PT_SB_SLAVEINFO_DESC pInfo      );

     EC_T_DWORD      GetVendorId(                EC_T_VOID                          )
                        { return m_dwVendorId; }
     EC_T_VOID       SetVendorId(                EC_T_DWORD  dwVal                  )
                        { m_dwVendorId = dwVal; }
     EC_T_DWORD      GetProductCode(             EC_T_VOID                          )
                        { return m_dwProductCode; }
     EC_T_VOID       SetProductCode(             EC_T_DWORD  dwVal                  )
                        { m_dwProductCode = dwVal; }
     EC_T_DWORD      GetRevisionNo(              EC_T_VOID                          )
                        { return m_dwRevisionNumber; }
     EC_T_VOID       SetRevisionNo(              EC_T_DWORD  dwVal                  )
                        { m_dwRevisionNumber = dwVal; }
     EC_T_DWORD      GetSerialNo(                EC_T_VOID                          )
                        { return m_dwSerialNumber; }
     EC_T_VOID       SetSerialNo(                EC_T_DWORD  dwVal                  )
                        { m_dwSerialNumber = dwVal; }
     EC_T_WORD       GetBootRcvMbxOffset(        EC_T_VOID                          )
                        { return m_wBootRcvMbxOffset; }
     EC_T_WORD       GetBootRcvMbxSize(          EC_T_VOID                          )
                        { return m_wBootRcvMbxSize; }
     EC_T_DWORD      GetBootRcvMbx(              EC_T_VOID                          )
                        { return EC_MAKEDWORD(m_wBootRcvMbxSize, m_wBootRcvMbxOffset); }
     EC_T_VOID       SetBootRcvMbx(              EC_T_DWORD  dwVal                  )
                        { m_wBootRcvMbxSize = EC_HIWORD(dwVal); m_wBootRcvMbxOffset = EC_LOWORD(dwVal); }
     EC_T_WORD       GetBootSndMbxOffset(        EC_T_VOID                          )
                        { return m_wBootSndMbxOffset; }
     EC_T_WORD       GetBootSndMbxSize(          EC_T_VOID                          )
                        { return m_wBootSndMbxSize; }
     EC_T_DWORD      GetBootSndMbx(              EC_T_VOID                          )
                        { return EC_MAKEDWORD(m_wBootSndMbxSize, m_wBootSndMbxOffset); }
     EC_T_VOID       SetBootSndMbx(              EC_T_DWORD  dwVal                  )
                        { m_wBootSndMbxSize = EC_HIWORD(dwVal); m_wBootSndMbxOffset = EC_LOWORD(dwVal); }
     EC_T_WORD       GetStdRcvMbxOffset(         EC_T_VOID                          )
                        { return m_wStdRcvMbxOffset; }
     EC_T_WORD       GetStdRcvMbxSize(           EC_T_VOID                          )
                        { return m_wStdRcvMbxSize; }
     EC_T_DWORD      GetStdRcvMbx(               EC_T_VOID                          )
                        { return EC_MAKEDWORD(m_wStdRcvMbxSize, m_wStdRcvMbxOffset); }
     EC_T_VOID       SetStdRcvMbx(               EC_T_DWORD  dwVal                  )
                        { m_wStdRcvMbxSize = EC_HIWORD(dwVal); m_wStdRcvMbxOffset = EC_LOWORD(dwVal); }
     EC_T_WORD       GetStdSndMbxOffset(         EC_T_VOID                          )
                        { return m_wStdSndMbxOffset; }
     EC_T_WORD       GetStdSndMbxSize(           EC_T_VOID                          )
                        { return m_wStdSndMbxSize; }
     EC_T_DWORD      GetStdSndMbx(               EC_T_VOID                          )
                        { return EC_MAKEDWORD(m_wStdSndMbxSize, m_wStdSndMbxOffset); }
     EC_T_VOID       SetStdSndMbx(               EC_T_DWORD  dwVal                  )
                        { m_wStdSndMbxSize = EC_HIWORD(dwVal); m_wStdSndMbxOffset = EC_LOWORD(dwVal); }
     EC_T_WORD       GetMbxProtocols(            EC_T_VOID                          )
                        { return m_wMbxSupportedProtocols; }
     EC_T_VOID       SetMbxProtocols(            EC_T_WORD   wVal                   )
                        { m_wMbxSupportedProtocols = wVal; }

     EC_T_DWORD      GetScanBusStatus(           EC_T_VOID                          )
                        { return m_dwScanStatus; }
     EC_T_VOID       SetScanBusStatus(           EC_T_DWORD  dwVal                  )
                        { m_dwScanStatus = dwVal; }

     CEcSlave*       GetCfgSlave(                EC_T_VOID                          )
                        { return m_pCfgSlave; }
     EC_T_VOID       SetCfgSlave(                CEcSlave* pVal                     )
                        { m_pCfgSlave = pVal; }

#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
     EC_T_VOID       ResetRecvTimes(             EC_T_VOID                          );
     EC_T_VOID       SetRecvTimes(               EC_T_PBYTE pbyVal,
                                                 CEcMaster* poMaster                );
     EC_T_DWORD      GetRecvTimeX(               EC_T_WORD   wPort                  )
						{ return m_adwRecvTime[wPort]; }
     EC_T_UINT64     GetRecvTimeProcessingUnit(  EC_T_VOID                          )
                        { return m_qwRecvTimeProcessingUnit; }
     EC_T_VOID       ResetPortDelays(            EC_T_VOID                          )
                        {
                            m_adwPortDelay[ESC_PORT_A] = 0;
                            m_adwPortDelay[ESC_PORT_B] = 0;
                            m_adwPortDelay[ESC_PORT_C] = 0;
                            m_adwPortDelay[ESC_PORT_D] = 0;
                        }
     EC_T_VOID       CalculatePortDelays(        EC_T_INT nMeasCycles = 0           );

     EC_T_DWORD      GetSlaveDelay(              EC_T_VOID                          )
                        { return m_dwSlaveDelay; }
#endif /* INCLUDE_DC_SUPPORT || INCLUDE_LINE_CROSSED_DETECTION */
#if (defined INCLUDE_DC_SUPPORT)
     EC_T_VOID       ResetLineDelays    (            EC_T_VOID                      )
                        {
                            m_adwLineDelay[ESC_PORT_A] = 0;
                            m_adwLineDelay[ESC_PORT_B] = 0;
                            m_adwLineDelay[ESC_PORT_C] = 0;
                            m_adwLineDelay[ESC_PORT_D] = 0;
                        }
     EC_T_DWORD      GetPropagDelay(             EC_T_VOID                          )
                        { return m_dwPropagDelay; }
     EC_T_UINT64     GetSystemTimeOffset(        EC_T_VOID                          )
                        { return m_qwSystemTimeOffset; }
     EC_T_VOID       SetSystemTimeOffset(        EC_T_UINT64 qwVal                  )
                        { m_qwSystemTimeOffset = qwVal; }
#endif /* INCLUDE_DC_SUPPORT*/
     EC_T_WORD       GetAlStatus()
                        { return m_wAlStatus; }
     EC_T_VOID       SetAlStatus(EC_T_WORD wVal)
                        {
                             m_wAlStatus = wVal;
                             if (DEVICE_STATE_BOOTSTRAP == (m_wAlStatus & DEVICE_STATE_MASK))
                             {
                                m_wFlagField |= ECBT_SLAVEFLAG_BOOTSTRAPSUPPORTED;
                             }
                        }
     EC_T_WORD       GetAlStatusCode()
                        { return m_wAlStatusCode; }
     EC_T_VOID       SetAlStatusCode(EC_T_WORD wVal)
                        { 
                             m_wAlStatusCode = wVal; 
                             if (DEVICE_STATUSCODE_BOOTSTRAPNSUPP == m_wAlStatusCode)
                             {
                                 m_wFlagField &= (EC_T_WORD)(~ECBT_SLAVEFLAG_BOOTSTRAPSUPPORTED);
                             }
                        }
     EC_T_BOOL      IsAlStatusErrorPending() 
                        {
                             return (0 != (GetAlStatus() & DEVICE_STATE_ERROR)) && !IsDeviceEmulation();
                        }
     EC_T_DWORD     GetPortsInfo(EC_T_BUS_SLAVE_PORTS_INFO* pPortsInfo)
                        {
                            OsMemset(pPortsInfo, 0, sizeof(EC_T_BUS_SLAVE_PORTS_INFO));
                            pPortsInfo->byPortDescriptor = m_byPortDescriptor;
                            pPortsInfo->wDlStatus = m_wDlStatus;
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
                            for (EC_T_WORD wPort = 0; wPort < ESC_PORT_COUNT; wPort++)
                            {
                                pPortsInfo->adwRecvTime[wPort] = m_adwRecvTime[wPort];
                            }
                            pPortsInfo->qwRecvTimeProcessingUnit = m_qwRecvTimeProcessingUnit;
#endif
                            return EC_E_NOERROR;
                        }
     EC_T_VOID      SynchronizeWithDuplicate(CEcBusSlave* pBusSlaveDuplicate)
                        {
                        CEcBusSlave* pBusSlavePrev = m_pPrev; /* don't store these values in pBusSlaveDuplicate because */
                        CEcBusSlave* pBusSlaveNext = m_pNext; /* pBusSlaveDuplicate is linked in the duplicate list     */

                            pBusSlaveDuplicate->m_dwBusIndex = m_dwBusIndex;
                            OsMemcpy(m_apBusSlaveChildren, pBusSlaveDuplicate->m_apBusSlaveChildren, sizeof(m_apBusSlaveChildren));
                            pBusSlaveDuplicate->m_wInputPort = m_wInputPort;
                            pBusSlaveDuplicate->m_wParentPort = m_wParentPort;
                            pBusSlaveDuplicate->m_wPortAtParent = m_wPortAtParent;
                            OsMemcpy(m_awLinkPort, pBusSlaveDuplicate->m_awLinkPort, sizeof(m_awLinkPort));
                            pBusSlaveDuplicate->m_pBusSlaveBranchingLast = m_pBusSlaveBranchingLast;
                            pBusSlaveDuplicate->m_wPortBranchingLast = m_wPortBranchingLast;
                            pBusSlaveDuplicate->m_byConnectedPortCnt = m_byConnectedPortCnt;
                            pBusSlaveDuplicate->m_dwDlControl = m_dwDlControl;
                            pBusSlaveDuplicate->m_wDlStatus = m_wDlStatus;
                            pBusSlaveDuplicate->m_wAlStatus = m_wAlStatus;
                            pBusSlaveDuplicate->m_wAlStatusCode = m_wAlStatusCode;
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
                            OsMemcpy(m_adwRecvTime, pBusSlaveDuplicate->m_adwRecvTime, sizeof(m_adwRecvTime));
                            pBusSlaveDuplicate->m_qwRecvTimeProcessingUnit = m_qwRecvTimeProcessingUnit;
#endif
                            OsMemcpy(this, pBusSlaveDuplicate, sizeof(CEcBusSlave));
                            m_pPrev = pBusSlavePrev;
                            m_pNext = pBusSlaveNext;
                        }
private:
    /* internal management */
    CEcBusSlave*            m_pNext;
    CEcBusSlave*            m_pPrev;

public:
    /* topology information */
    EC_T_DWORD              m_dwBusIndex;                         /* index on the network */
    CEcBusSlave*            m_apBusSlaveChildren[ESC_PORT_COUNT]; /* children slave per ports (validated in the currentscan) */
    EC_T_WORD               m_wInputPort;                         /* port where the EtherCAT frames are coming from */
    EC_T_WORD               m_wParentPort;                        /* port closest to processing unit where the EtherCAT frames are coming from */
    EC_T_WORD               m_wPortAtParent;                      /* port of the parent slave connected to parent port of this slave */
    EC_T_WORD               m_awLinkPort[ESC_PORT_COUNT];         /* see ECBT_LINKPORT_LINKSTATUS */
    CEcBusSlave*            m_pBusSlaveBranchingLast;             /* last branching slave before current one */
    EC_T_WORD               m_wPortBranchingLast;                 /* branching port at last branching slave*/
    EC_T_BYTE               m_byConnectedPortCnt;

    /* link to config slave */
    CEcSlave*               m_pCfgSlave;

private:
    /* values of ESC regiters */
    EC_T_BYTE               m_byType;                       /* ESC 0x0000 */
    EC_T_BYTE               m_byRevision;                   /* ESC 0x0001 */
    EC_T_WORD               m_wBuild;                       /* ESC 0x0002 */
    EC_T_BYTE               m_bySupportedFmmuEntities;      /* ESC 0x0004 */
    EC_T_BYTE               m_bySupportedSyncManchannels;   /* ESC 0x0005 */
    EC_T_BYTE               m_byRAMSize;                    /* ESC 0x0006 */
    EC_T_BYTE               m_byPortDescriptor;             /* ESC 0x0007 */
    EC_T_WORD               m_wFeaturesSupported;           /* ESC 0x0008 */
    EC_T_WORD               m_wStationFixedAddress;         /* ESC 0x0010 */
    EC_T_WORD               m_wStationAliasAddress;         /* ESC 0x0012 */
    EC_T_DWORD              m_dwDlControl;                  /* ESC 0x0100 */
    EC_T_WORD               m_wDlStatus;                    /* ESC 0x0110 */
    EC_T_WORD               m_wAlStatus;                    /* ESC 0x0130 */
    EC_T_WORD               m_wAlStatusCode;                /* ESC 0x0134 */
    EC_T_BYTE               m_byPDI;                        /* ESC 0x0140 */
    EC_T_WORD               m_wEcatEventMask;               /* ESC 0x0200 */
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_DWORD              m_adwRecvTime[ESC_PORT_COUNT];  /* ESC 0x900 + y*4 y=0..3 */
    EC_T_UINT64             m_qwRecvTimeProcessingUnit;     /* ESC 0x0918 */
#endif
#if (defined INCLUDE_DC_SUPPORT)
public:
    EC_T_UINT64             m_qwSystemTimeOffset;           /* ESC 0x0920 */
    EC_T_DWORD              m_dwPropagDelay;                /* ESC 0x0928 */
    EC_T_DWORD              m_dwSystemTimeDifference;       /* ESC 0x092C */
#endif /* INCLUDE_DC_SUPPORT */

private:
    /* values from EEPROM */
    EC_T_DWORD              m_dwVendorId;                   /* EEPROM Offset 0x0008 */
    EC_T_DWORD              m_dwProductCode;                /* EEPROM Offset 0x000A */
    EC_T_DWORD              m_dwRevisionNumber;             /* EEPROM Offset 0x000C */
    EC_T_DWORD              m_dwSerialNumber;               /* EEPROM Offset 0x000E */
    EC_T_WORD               m_wBootRcvMbxOffset;            /* EEPROM Offset 0x0014 */
    EC_T_WORD               m_wBootRcvMbxSize;              /* EEPROM Offset 0x0015 */
    EC_T_WORD               m_wBootSndMbxOffset;            /* EEPROM Offset 0x0016 */
    EC_T_WORD               m_wBootSndMbxSize;              /* EEPROM Offset 0x0017 */
    EC_T_WORD               m_wStdRcvMbxOffset;             /* EEPROM Offset 0x0018 */
    EC_T_WORD               m_wStdRcvMbxSize;               /* EEPROM Offset 0x0019 */
    EC_T_WORD               m_wStdSndMbxOffset;             /* EEPROM Offset 0x001A */
    EC_T_WORD               m_wStdSndMbxSize;               /* EEPROM Offset 0x001B */
    EC_T_WORD               m_wMbxSupportedProtocols;       /* EEPROM Offset 0x001C */

    /* flag field */
    EC_T_WORD               m_wFlagField;                   /* see ECBT_SLAVEFLAG_xxx */

public:
    /* device identification */
    EC_T_WORD               m_wIdentificationAdo;           /* ado of last identification check */
    EC_T_WORD               m_wIdentificationValue;         /* value of last identification check */
    EC_T_WORD               m_wIdentificationPrevAdo;       /* ado of previous identification request */
    CEcSlave*               m_pCfgSlaveIdentMisMatch;       /* last identification candidate */
    CEcBusSlave*            m_pBusSlaveDuplicate;           /* Duplicate HC slave */

    /* LineCrossed Detecttion / DC support */
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_DWORD				m_dwSlaveDelay;                 /* Total slave delay measured by the previous slave */
    EC_T_DWORD				m_dwProcessingUnitDelay;        /* Time delay due to the processing unit */
    EC_T_DWORD              m_adwPortDelay[ESC_PORT_COUNT]; /* Time elapsed between send and receive on this port */
#endif
#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL               m_bDcInitialized;               /* DC initialized */
    EC_T_DWORD              m_adwLineDelay[ESC_PORT_COUNT]; /* Time elapsed between this port and the next slave */
#endif

    /* bus scan variables */
    EC_T_DWORD              m_dwScanStatus;
    EC_T_WORD               m_wSlaveTmpFixAddr;
    EC_T_WORD               m_awLinkPortBackup[ESC_PORT_COUNT]; /* same bit description as m_wLinkPort */
};
#if (defined INCLUDE_GEN_OP_ENI)
typedef struct _ECBT_T_PDOENTRY {
    EC_T_WORD               wIndex;                         /* ESC_SII_CAT_PDO_INDEX */
    EC_T_BYTE               bySubIndex;                     /* ESC_SII_CAT_PDO_SUBINDEX */
    EC_T_BYTE               byBitLen;                       /* ESC_SII_CAT_PDO_BITLEN */
    EC_T_WORD               wBitOffs;
} ECBT_T_PDOENTRY, *ECBT_PT_PDOENTRY;

typedef struct _ECBT_T_PDOLIST {
struct _ECBT_T_PDOLIST* pNext;
    EC_T_WORD               wIndex;                         /* ESC_SII_CAT_PDOLIST_INDEX */
    EC_T_BYTE               bySmIdx;                        /* ESC_SII_CAT_PDOLIST_SYNCM */
    EC_T_BYTE               byEntries;
    ECBT_PT_PDOENTRY        poEntries;
} ECBT_T_PDOLIST, *ECBT_PT_PDOLIST;

class CEcBusSlaveGenEni : public CEcBusSlave
{
public:
    CEcBusSlaveGenEni() {}
     virtual ~CEcBusSlaveGenEni() {}

    virtual CEcBusSlaveGenEni* GetBusSlaveGenEni(EC_T_VOID                          )
                         { return this; };
    virtual EC_T_VOID       InitInstance(        EC_T_VOID                          );

    /* storage */
    EC_T_BYTE m_abyFmmu[EC_CFG_SLAVE_PD_SECTIONS];/* Category FMMU     */
    EC_T_BYTE m_byLogicalMBoxState;           /* mailbox state is mapped into logical */
    
    EC_T_BYTE m_bySmOutEntries;               /* number of used sync managers for output */
    EC_T_BYTE m_bySmInpEntries;               /* number of used sync managers for input */

    struct {
    EC_T_WORD wAddr;                          /* ESC_SII_CAT_SYNCM_REG_STARTADDRESS */
    EC_T_WORD wMaxVarSize;                    /* Size of biggest variable in bits */
    EC_T_WORD wBitLen;                        /* Sum of all ESC_SII_CAT_PDO_REG_BITLEN */
    EC_T_BYTE byControl;                      /* ESC_SII_CAT_SYNCM_REG_CONTROL */
    EC_T_BYTE byEnable;                       /* ESC_SII_CAT_SYNCM_REG_ENABLE */
    EC_T_BYTE byIdx;                          /* sync manager index */
    } m_aSmOutEntries[EC_CFG_SLAVE_PD_SECTIONS], m_aSmInpEntries[EC_CFG_SLAVE_PD_SECTIONS];

    ECBT_PT_PDOLIST m_poTxPdoList, m_poRxPdoList;

    EC_T_DWORD m_dwDcSync0CycleTime;          /* ESC_SII_CAT_DC_SYNC0_CYCLE_TIME */
    EC_T_WORD  m_wDcActivation;               /* ESC_SII_CAT_DC_REG_ACTIVATION */
    struct {
    EC_T_DWORD dwShiftTime;                   /* ESC_SII_CAT_DC_SYNCx_SHIFT */
    EC_T_SWORD swCycleFactor;                 /* ESC_SII_CAT_DC_SYNCx_CYC_TIME_FACTOR */
    } m_aDcSync[2];

    /* state machine variables */
    EC_T_WORD m_wEEPROMAddrCur;
    EC_T_WORD m_wEEPROMCategoryBaseCur;
    EC_T_WORD m_wEEPROMCategorySizeCur;
    EC_T_WORD m_wEEPROMCategoryTypeCur;

    /* temp buffer */
    EC_T_BYTE m_abyEEPROMValCur[24];
    EC_T_BYTE m_byEEPROMValCntCur;

    ECBT_T_PDOLIST* m_poPdoListCur;
    EC_T_BYTE       m_byPdoCntCur;
};
#endif /* INCLUDE_GEN_OP_ENI */

class CEcBusTopology
{
public:
    CEcBusTopology(EC_T_VOID);
    virtual ~CEcBusTopology();
    EC_T_VOID InitInstance(CEcMaster* pMaster, CEcDevice* poEthDevice, EC_T_DWORD dwMaxSlaveElements);

    /* bus slaves memory pool */
    CEcBusSlave*       m_poBusSlavesPool;
    CEcBusSlaveGenEni* m_poBusSlavesGenEniPool;
    EC_T_DWORD         m_dwBusSlavesPoolSize;
    CHashTableDyn<EC_T_DWORD, CEcBusSlave*>* m_pBusSlavesByIndex;
    CHashTableDyn<EC_T_WORD,  CEcBusSlave*>* m_pBusSlavesByFixedAddress;
    CEcBusSlave*       m_poBusSlavesPoolFree;
    EC_T_BYTE*         m_pbyBusSlavesMem;
    EC_T_BYTE*         m_pbyBusSlavesMemCur;
    EC_T_DWORD         m_dwBusSlavesMemSize;
    EC_T_BOOL          m_bBusSlavesPoolReady;

    /* bus slaves memory pool */
    EC_T_DWORD      CreateBusSlavesPool (EC_T_DWORD dwMaxBusSlaves, EC_T_BOOL bGenerateENI);
    EC_T_DWORD      DeleteBusSlavesPool (EC_T_VOID);
    EC_T_DWORD      GetBusSlavesPoolSize(EC_T_VOID)
                        { return m_dwBusSlavesPoolSize; }
    EC_T_DWORD      RecreateBusSlavesPool (EC_T_DWORD dwMaxBusSlaves, EC_T_BOOL bGenerateENI);

    /* new/delete bus slave function */
    CEcBusSlave*    NewBusSlave     (EC_T_VOID);
    EC_T_BYTE*      AllocBusSlaveMem(EC_T_DWORD dwSize);
    EC_T_VOID       DeleteBusSlave  (CEcBusSlave* pBusSlave);
    EC_T_VOID       DeleteBusSlaves (EC_T_VOID);

    /* Config topology information */
    EC_T_DWORD  CreateCfgTopology   (EC_T_VOID);
    EC_T_DWORD  DeleteCfgTopology   (EC_T_VOID);

    /* Bus topology information */
    EC_T_DWORD  BuildBusTopologyPrepare   (EC_T_VOID);
    EC_T_VOID   BuildBusTopologyPrepareSlave (CEcBusSlave* pBusSlave);
    EC_T_WORD   GetLastConnectedPortAfterPort(CEcBusSlave* pBusSlave, EC_T_WORD wAfterPort);
    EC_T_VOID   LinkToParentBusSlave      (CEcBusSlave* pBusSlave, EC_T_WORD wParentPort, CEcBusSlave* pBusSlaveParent, EC_T_WORD wPortAtParent);
    EC_T_VOID   StartNewTopology          (CEcBusSlave* pBusSlave);
    EC_T_VOID   SetBusSlaveBranchingCur   (EC_T_BOOL bBack, CEcBusSlave* pBusSlaveBranchingCur, EC_T_WORD wPortBranchingCur, CEcBusSlave* pBusSlaveCur);
    EC_T_VOID   InsertBusSlaveInTopology  (CEcBusSlave* pBusSlave);
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_VOID   DetectLineCrossed         (CEcBusSlave* pBusSlave);
    EC_T_VOID   InsertLineCrossedBusSlaveInTopology(
                                           CEcBusSlave* pBusSlave);
#endif
    EC_T_VOID   LookForMainPortInTopology (EC_T_VOID);
#if (defined INCLUDE_RED_DEVICE)
    EC_T_VOID   LookForRedPortInTopology  (EC_T_VOID);
#endif
    EC_T_VOID   BuildBusTopology          (CEcBusSlave* pBusSlave);
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_VOID   PrintTopologyTest         (CEcBusSlave* pBusSlave);
#endif
    /* Operations */
    EC_T_DWORD  StartBusScan(                       EC_T_DWORD dwTimeout                );
    EC_T_DWORD  StartTopologyChange(                EC_T_DWORD dwTimeout                );
    EC_T_DWORD  StartAcceptTopologyChange(          EC_T_DWORD dwTimeout                );
    EC_T_DWORD  StartDlStatusInt(                   EC_T_DWORD dwTimeout                );
    EC_T_DWORD  StartAlStatusRefresh(               EC_T_DWORD dwTimeout                );
    EC_T_DWORD  WaitForBTStateMachine(              EC_T_VOID                           );
    EC_T_DWORD  ReleaseBTStateMachine(              EC_T_VOID                           );

    /* Frame Return Triggered by JobTask */
    EC_T_VOID   ProcessResult(                      EC_T_DWORD result, 
                                                    struct TECAT_SLAVECMD_DESC* pSlaveCmd,
                                                    PETYPE_EC_CMD_HEADER pEcCmdHeader   );

    EC_T_DWORD  GetConnectedSlaveCount(             EC_T_VOID                           )
                    { return m_dwSlavesAtStart; }
    EC_T_DWORD  GetDCSlaveCount(                    EC_T_VOID                           )
                    { return m_dwBusSlavesDC; }
    CEcBusSlave* GetBusSlaveList(                   EC_T_VOID                           )
                    { return m_pBusSlaveRoot; }

    EC_T_VOID   InsertBusSlaveInListBefore(         CEcBusSlave*    pBusSlaveNext
                                                   ,CEcBusSlave*    pBusSlave           )
                    {
                    CEcBusSlave* pBusSlavePrev = EC_NULL;

                        OsLock(m_poBusSlaveListLock);
                        if (EC_NULL != pBusSlaveNext) { pBusSlavePrev = pBusSlaveNext->GetPrev(); pBusSlaveNext->SetPrev(pBusSlave); }
                        if (EC_NULL != pBusSlavePrev)   pBusSlavePrev->SetNext(pBusSlave);
                        else 
                            m_pBusSlaveRoot = pBusSlave;
                        pBusSlave->SetPrev(pBusSlavePrev);
                        pBusSlave->SetNext(pBusSlaveNext);
                        m_dwBusSlaves++;
                        OsUnlock(m_poBusSlaveListLock);
                    }
    EC_T_VOID   InsertBusSlaveInListAfter(          CEcBusSlave*    pBusSlavePrev
                                                   ,CEcBusSlave*    pBusSlave           )
                    {
                    CEcBusSlave* pBusSlaveNext = EC_NULL;

                        OsLock(m_poBusSlaveListLock);
                        if (EC_NULL != pBusSlavePrev) { pBusSlaveNext = pBusSlavePrev->GetNext(); pBusSlavePrev->SetNext(pBusSlave); }
                        else
                            m_pBusSlaveRoot = pBusSlave;
                        if (EC_NULL != pBusSlaveNext) pBusSlaveNext->SetPrev(pBusSlave);
                        pBusSlave->SetPrev(pBusSlavePrev);
                        pBusSlave->SetNext(pBusSlaveNext);
                        m_dwBusSlaves++;
                        OsUnlock(m_poBusSlaveListLock);
                    }
    EC_T_VOID   RemoveBusSlaveFromList(             CEcBusSlave*    pBusSlave           )
                    {
                    CEcBusSlave* pBusSlavePrev = pBusSlave->GetPrev();
                    CEcBusSlave* pBusSlaveNext = pBusSlave->GetNext();

                        OsLock(m_poBusSlaveListLock);
                        if (EC_NULL != pBusSlavePrev) pBusSlavePrev->SetNext(pBusSlaveNext);
                        else 
                            m_pBusSlaveRoot = pBusSlaveNext;

                        if (EC_NULL != pBusSlaveNext) pBusSlaveNext->SetPrev(pBusSlavePrev);
                        m_dwBusSlaves--;
                        OsUnlock(m_poBusSlaveListLock);
                    }
    EC_T_VOID   DeleteUnexpectedBusSlaves(          EC_T_VOID                           ); //TODO Slave slicing
    EC_T_VOID   RemoveBusSlaveGenEniFromList(       EC_T_VOID                           ) //TODO Slave slicing
    {
        EC_T_DWORD dwBusIndex = 0;

        for (CEcBusSlave* poBusSlaveCur = GetBusSlaveList(); EC_NULL != poBusSlaveCur; poBusSlaveCur = poBusSlaveCur->GetNext())
        {
            m_pBusSlavesByIndex->Remove(poBusSlaveCur->GetBusIndex());

            if (EC_NULL != poBusSlaveCur->GetBusSlaveGenEni())
            {
                m_pBusSlavesByFixedAddress->Remove(poBusSlaveCur->GetFixedAddress());
                RemoveBusSlaveFromList(poBusSlaveCur);
            }
            else
            {
                m_pBusSlavesByIndex->Add(dwBusIndex++, poBusSlaveCur);
            }
        }
    }
    EC_T_DWORD  GetUnexpectedSlaveCount(EC_T_VOID)
                    { return m_dwBusSlavesUnexpected; }
   
    EC_T_DWORD  GetListSlaveCount(                  EC_T_VOID                           )
                    { return m_dwBusSlaves; }

    EC_T_DWORD  RecordBusSlavePrepare(              EC_T_VOID                           );
    EC_T_VOID   AssignTmpFixedAddress(              CEcBusSlave*    pBusSlave           );
    EC_T_DWORD  RecordBusSlave(                     EC_T_WORD       wAutoIncAddress,
                                                    EC_T_BYTE*      pbyData             );
    EC_T_DWORD  RemoveBusSlave(                     CEcBusSlave*    pBusSlave           );
    EC_T_VOID   RemoveBusSlaves(                    EC_T_VOID                           );
    EC_T_VOID   RefreshCfgSlavePresence(            CEcSlave*       pCfgSlave           );

    CEcBusSlave* GetBusSlaveMainPort(               EC_T_VOID                           )
                    { return m_pBusSlaveMainPort; }
    EC_T_WORD    GetMainPort(                       EC_T_VOID                           )
    { return (EC_T_WORD)((EC_NULL != m_pBusSlaveMainPort)?(m_pBusSlaveMainPort->m_wInputPort):(ESC_PORT_INVALID)); }
#if (defined INCLUDE_RED_DEVICE)
    CEcBusSlave* GetBusSlaveRedPort(                EC_T_VOID                           )
                    { return m_pBusSlaveRedPort; }
    EC_T_WORD    GetRedPort(                        EC_T_VOID                           )
                    { return m_wRedPort; }
    EC_T_BOOL    IsRedPort(                         CEcBusSlave*    pBusSlave
                                                   ,EC_T_WORD       wPort               )
    {
        return ((pBusSlave == m_pBusSlaveRedPort) && (wPort == m_wRedPort));
    }
#else
    EC_T_BOOL    IsRedPort(                         CEcBusSlave*    pBusSlave
                                                   ,EC_T_WORD       wPort               )
    {
        EC_UNREFPARM(pBusSlave);
        EC_UNREFPARM(wPort);
        return EC_FALSE;
    }
#endif /* INCLUDE_RED_DEVICE */
    CEcBusSlave*   GetBusSlaveByIndex(              EC_T_DWORD      dwIdx               )
                    { CEcBusSlave* pBusSlave = EC_NULL; if (m_pBusSlavesByIndex->Lookup(dwIdx, pBusSlave)) return pBusSlave; else return EC_NULL; }
    CEcBusSlave*   GetBusSlaveByAutoIncAddress(     EC_T_WORD       wAutoIncAddress     )
                    { return GetBusSlaveByIndex(EC_MAKEDWORD(0, (wAutoIncAddress*0xFFFF))); }
    CEcBusSlave*   GetBusSlaveByFixedAddress(       EC_T_WORD       wFixedAddress       )
                    { CEcBusSlave* pBusSlave = EC_NULL; if (m_pBusSlavesByFixedAddress->Lookup(wFixedAddress, pBusSlave)) return pBusSlave; else return EC_NULL; }

    EC_T_VOID      GetNextBusSlaveByPropagation(    CEcBusSlave*    pBusSlave,
                                                    EC_T_WORD       wPortIn,
                                                    EC_T_BOOL       bSkipBranching,
                                                    EC_T_WORD*      pwPortOut,
                                                    CEcBusSlave**   ppBusSlaveNext,
                                                    EC_T_WORD*      pwPortInNext        );

    EC_T_BOOL      GetVerificationActive(           EC_T_VOID                           )
                    { return m_bTopologyVerificationEnabled; }
    EC_T_VOID      SetVerificationActive(           EC_T_BOOL   bVerificationActive     )
                    { m_bTopologyVerificationEnabled = bVerificationActive; }
    EC_T_BOOL      GetNotifyUnexpectedBusSlaves(    EC_T_VOID                           )
                    { return m_bNotifyUnexpectedBusSlaves; }
    EC_T_VOID      SetNotifyUnexpectedBusSlaves(    EC_T_BOOL   bNotifyUnexpectedBusSlaves)
                    { m_bNotifyUnexpectedBusSlaves = bNotifyUnexpectedBusSlaves; }
    
    EC_T_VOID      InterLockList(                   EC_T_BOOL bLS                       );

    EC_T_VOID      SetPortOperationsEnabled(        EC_T_BOOL   bPortOperationsEnabled)
                    { m_bPortOperationsEnabled = bPortOperationsEnabled; }
    EC_T_BOOL      GetPortOperationsEnabled(        EC_T_VOID                           )
                    { return m_bPortOperationsEnabled; }

    EC_T_VOID      EnableAliasAddressing(           EC_T_BOOL bEnable                   )
                    { m_bSlaveAliasAddressing = bEnable; }
    EC_T_BOOL      IsAliasAddressingEnabled(        EC_T_VOID                           )
                    { return m_bSlaveAliasAddressing; }

    EC_T_VOID      SetTopologyChangeDelay(EC_T_DWORD dwDelay)
                    { m_dwTopologyChangeDelay = dwDelay; }
    EC_T_DWORD     GetTopologyChangeDelay(EC_T_VOID)
                    { return m_dwTopologyChangeDelay; }
#if (defined INCLUDE_RED_DEVICE)
    EC_T_VOID      MainLinkStatusChanged(EC_T_LINKSTATUS eLinkStatusMain, EC_T_LINKSTATUS eLinkStatusRed);
    EC_T_VOID      RedLinkStatusChanged(EC_T_LINKSTATUS eLinkStatusMain, EC_T_LINKSTATUS eLinkStatusRed);
#else
    EC_T_VOID      LinkStatusChanged(EC_T_LINKSTATUS eLinkStatus);
#endif
    EC_T_BOOL      SetTopologyChangedMasked(EC_T_BOOL bMasked) {EC_T_BOOL bMaskedOld = m_bTopologyChangedMasked; m_bTopologyChangedMasked = bMasked; return bMaskedOld;};
    EC_T_VOID      SetTopologyChanged(EC_T_VOID);
    EC_T_BOOL      TopologyChangedDetected(EC_T_VOID);
    EC_T_BOOL      TopologyIsChanging(EC_T_VOID);
    EC_T_VOID      TopologyChangeDone(EC_T_VOID)
                    { m_bTopologyChanged = EC_FALSE; }

    EC_T_BOOL      IsSendMaskedOnMain(EC_T_VOID)
                    { return m_bSendMaskedOnMain; }
	EC_T_VOID      MaskSendOnMain(EC_T_BOOL bMasked)
                    { m_bSendMaskedOnMain = bMasked; }
#if (defined INCLUDE_RED_DEVICE)    
    EC_T_BOOL      IsSendMaskedOnRed(EC_T_VOID)
                    { return m_bSendMaskedOnRed; }
	EC_T_VOID      MaskSendOnRed(EC_T_BOOL bMasked)
                    { m_bSendMaskedOnRed = bMasked; }
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_BOOL      GetRedEnhancedLineCrossedDetectionEnabled(EC_T_VOID)
                    { return m_bRedEnhancedLineCrossedDetectionEnabled; }
    EC_T_VOID      SetRedEnhancedLineCrossedDetectionEnabled(EC_T_BOOL bEnabled)
                    { m_bRedEnhancedLineCrossedDetectionEnabled = bEnabled; }
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
#endif /* INCLUDE_RED_DEVICE */
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_VOID      SetErrorOnLineCrossed( EC_T_BOOL bErrorOnLineCrossed )
                    { m_bErrorOnLineCrossed = bErrorOnLineCrossed; }
    EC_T_BOOL      GetErrorOnLineCrossed( EC_T_VOID ) { return m_bErrorOnLineCrossed; }
    EC_T_BOOL      GetNotifyNotConnectedPortA(EC_T_VOID)
                    { return m_bNotifyNotConnectedPortA; }
    EC_T_VOID      SetNotifyNotConnectedPortA(EC_T_BOOL bEnabled)
                    { m_bNotifyNotConnectedPortA = bEnabled; }
    EC_T_BOOL      GetNotifyUnexpectedConnectedPort(EC_T_VOID)
                    { return m_bNotifyUnexpectedConnectedPort; }
    EC_T_VOID      SetNotifyUnexpectedConnectedPort(EC_T_BOOL bEnabled)
                    { m_bNotifyUnexpectedConnectedPort = bEnabled; }
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    EC_T_BOOL      GetJunctionRedundancyEnabled(EC_T_VOID)
                    { return m_bJunctionRedundancyEnabled; }
    EC_T_VOID      SetJunctionRedundancyEnabled(EC_T_BOOL bEnabled);
#endif /* INCLUDE_JUNCTION_REDUNDANCY */
    EC_T_VOID      SetTopologyChangeAutoMode( EC_T_BOOL bAutoMode ) 
                    { m_bTopologyChangeAutoMode = bAutoMode; }
    EC_T_BOOL      GetTopologyChangeAutoMode( EC_T_VOID ) { return m_bTopologyChangeAutoMode; }

    EC_T_BOOL      ShallAckAlStatusError(CEcBusSlave* pBusSlave);

    static const EC_T_WORD c_awFrameProcessingPortOrder[];
    static const EC_T_WORD c_awBackwardPortOrder[];
    static const EC_T_WORD c_awCloseToProcessingUnitOrder[];

    EC_T_DWORD     SetReadEEPROMEntryByIoctl(EC_PT_SCANBUS_PROP_DESC pProp);
    EC_T_DWORD     SetCheckEEPROMEntryByIoctl(EC_PT_SCANBUS_PROP_DESC pProp);

    EC_T_VOID*     m_poBusSlaveListLock;            /* bus slave list mutex object */

    EC_T_VOID      SetNextBusScanGenEni(EC_T_VOID)
                    { m_bNextBusScanGenEni = EC_TRUE; }

public:
    EC_T_BOOL               m_abReadEEPROMEntry[READEEPROMENTRY_COUNT];
    EC_T_BOOL               m_abCheckEEPROMSetByIoctl[CHECKEEPROMENTRY_COUNT];
    EC_T_CHECKEEPROMENTRY   m_aeCheckEEPROMEntry[CHECKEEPROMENTRY_COUNT];
private:
EC_INSTRUMENT_MOCKABLE_VAR
    static const EC_T_WORD c_awReadEEPROMEntryOrder[READEEPROMENTRY_COUNT];
    EC_T_CHECKEEPROMENTRY  CheckEEPromContentEntryCriterion(CEcSlave* pCfgSlave, EC_T_DWORD dwEEPromEntryIdx);
    EC_T_DWORD             CheckEEPromContent(CEcBusSlave* pBusSlave, CEcSlave* pCfgSlave);

    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_BOOL TopologyChangedWhileScan(EC_T_VOID);

    EC_T_DWORD     IdentifyBusSlave(CEcBusSlave* pBusSlave, CEcSlaveList* pCfgSlaveList, CEcSlave** ppCfgSlave);
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_DWORD ReadSlaveIdentification(CEcBusSlave* pBusSlave, EC_T_WORD wIdentAdo);

#if (defined INCLUDE_HOTCONNECT)
    EC_T_DWORD     IdentifyHCGroup(CEcBusSlave* pBusSlave, CEcSlaveList* pCfgSlaveList, EC_T_HOTCONNECT_GROUP** ppHCGroup);
#endif
    EC_T_VOID      LinkSlaveDescriptors(CEcBusSlave* pBusSlave, CEcSlave* pCfgSlave);
    EC_T_VOID      UnlinkSlaveDescriptors(CEcBusSlave* pBusSlave);



    CEcMaster*                          m_pMaster;
    CEcDevice*                          m_poEthDevice;                  /* Ethernet device interface */

    EC_T_DWORD                          m_dwBTResult;                   /* result of the last bt state machine run */

    EC_T_BOOL                           m_bTopologyVerificationEnabled; /* default is EC_TRUE  means checking of
                                                                         * configuration is active 
                                                                         */
    EC_T_BOOL                           m_bNotifyUnexpectedBusSlaves;   /* default is EC_TRUE  means checking of configuration is active */
    EC_T_BOOL                           m_bPortOperationsEnabled;
    EC_T_BOOL                           m_bSlaveAliasAddressing;        /* Is slave alias addressing enabled ? */

    EC_T_DWORD                          m_dwEcatCmdPendingMax;          /* maximal number of pending ecat commands */

    /* topology changed management */
    EC_T_BOOL                           m_bTopologyChangeAutoMode;      /* Topology change automatically executed */
    EC_T_WORD                           m_wTopologyChangeSlaveCount;    /* Current monitored slave count before topology changes detected */
    EC_T_DWORD                          m_dwTopologyChangeCounter;      /* Counter to monitor topology changes before reaction */
    CEcTimer                            m_oTopologyChangeDelayTimeout;
    EC_T_DWORD                          m_dwTopologyChangeDelay;
    EC_T_BOOL                           m_bTopologyChangedMasked;
    EC_T_BOOL                           m_bTopologyChanged;
    EC_T_BOOL                           m_bLinkWasDisconnected;         /* trigger */    
    EC_T_BOOL                           m_bSendMaskedOnMain;
#if (defined INCLUDE_RED_DEVICE)
    EC_T_BOOL                           m_bSendMaskedOnRed;
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_BOOL                           m_bRedEnhancedLineCrossedDetectionEnabled;
    EC_T_BOOL                           m_bRedEnhancedLineCrossedDetectionRunning;
    CEcTimer                            m_oRedOpenPortTimeout;
#endif
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_BOOL                           m_bErrorOnLineCrossed;          /* Bus mismatch if IN and OUT connectors are swapped. */
    EC_T_BOOL                           m_bNotifyNotConnectedPortA;     /* Bus mismatch if port A is not connected */
    EC_T_BOOL                           m_bNotifyUnexpectedConnectedPort; /* Bus mismatch if unexpected connected port is detected */
    EC_T_BOOL                           m_bLineCrossedDetectionRunning; /* Signalize that line crossed detection is currently running */
    EC_T_BOOL                           m_bLineCrossedNotified;         /* Crossed line at any slave detected */
#endif
    CEcTimer                            m_oOpenClosedPortsTimeout;
    EC_T_BOOL                           m_bOpenClosedPorts;
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    EC_T_BOOL                           m_bJunctionRedundancyEnabled;
#endif
    /* topology change while bus scan management */
    EC_T_WORD                           m_wAcycBrdAlStatusWkc;          /* Acyclic AL status BRD */
    EC_T_DWORD                          m_dwSlavesAtStart;              /* Slaves count at start */
    EC_T_BOOL                           m_bLinkConnectedAtStart;        /* Link state at start */
#if (defined INCLUDE_RED_DEVICE)
    EC_T_BOOL                           m_bLineBreakAtStart;            /* Line break at start */
    EC_T_BOOL                           m_bIsSendEnabledOnMainAtStart;  /* Send enabled on main link at start */
    EC_T_BOOL                           m_bIsSendEnabledOnRedAtStart;   /* Send enabled on red link at start */
    EC_T_WORD                           m_wSlavesCntOnMainAtStart;      /* Slaves count on main at start */
    EC_T_WORD                           m_wSlavesCntOnRedAtStart;       /* Slaves count on red at start */
#endif

    /* bus slave list management */
    CEcBusSlave*                        m_pBusSlaveRoot;                /* Bus slave list ordered by bus index */
    EC_T_DWORD                          m_dwBusSlaves;                  /* Number of slaves in bus slave list */
    EC_T_DWORD                          m_dwBusSlavesDC;                /* Number of slaves in bus slave list supporting DC */
    EC_T_BOOL                           m_bPurgeBusSlavesAndNotify;
    CEcBusSlave*                        m_pBusSlaveDuplicate;           /* Bus slave list of pending duplicate slaves */
    EC_T_DWORD                          m_dwBusSlavesUnexpected;        /* Number of unexpected bus slaves */
    EC_T_DWORD                          m_dwBusSlavesUnexpectedCur;

    /* topology information */
    CEcBusSlave*                        m_pBusSlaveTopoMainRoot;        /* First slave of the topology (with biggest slave delay) */
    CEcBusSlave*                        m_pBusSlaveMainPort;            /* Bus slave where the main link is connected */
#if (defined INCLUDE_RED_DEVICE)
    CEcBusSlave*                        m_pBusSlaveTopoRedRoot;         /* First slave of the redundancy topology (with biggest slave delay) in case of line break */
    CEcBusSlave*                        m_pBusSlaveRedPort;             /* Bus slave where the redundancy link is connected */
    EC_T_WORD                           m_wRedPort;                     /* Port where the redundancy link is connected */
#endif

    /* topology detection */
    CEcSlaveList                        m_oFirstSlaveList;
#if (defined INCLUDE_HOTCONNECT)
    CEcSlaveList                        m_oHCSlavesList;
#endif
    CEcSlaveList                        m_oSingleSlaveListCur;

    /* generic state machine variables */
    EC_T_DWORD                          m_dwEcatCmdPending;             /* number of pending ecat commands */
    EC_T_DWORD                          m_dwEcatCmdSent;                /* number of pending ecat commands in a handling step */
    EC_T_DWORD                          m_dwErrorCur;                   /* currently detected error */
    EC_T_DWORD                          m_dwEcatCmdProcessed;           /* number of pending ecat commands in a handling step */
    EC_T_DWORD                          m_dwEcatCmdRetry;               /* number of pending ecat commands in a handling step */
    CEcBusSlave*                        m_pBusSlaveCur;                 /* currently evaluating bus slave */
    CEcBusSlave*                        m_pBusSlaveTopoCur;             /* currently evaluating bus slave */
    const EC_T_WORD*                    m_pawExpectedProcessingPortOrder; /* current expected processing port order */
    CEcBusSlave*                        m_pBusSlaveBranchingCur;        /* current branching slave */
    EC_T_WORD                           m_wPortBranchingCur;            /* branching port at current branching slave */
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    enum _EC_T_JUNCTIONREDSTATE
    {
        eJunctionRedStateUnknow,
        eJunctionRedStateLineFixed,
        eJunctionRedStateLineBreak,

        eJunctionRedState_BCppDummy = 0xFFFFFFFF
    }                                   m_eJunctionRedState;
#endif /* INCLUDE_JUNCTION_REDUNDANCY */
    EC_T_DWORD                          m_dwBusSlavesDCCur;             /* currently counting DC bus slaves */
    EC_T_DWORD                          m_dwCfgSlaveIdxCur;             /* currently evaluating config slave index */
    EC_T_DWORD                          m_dwAbsentIdxCur;               /* currently appending config slave index for Master OD object 0x3000 */

    /* EEPROM sub state machine variables */
    EC_T_DWORD                          m_dwCurEEPEntryIdx;             /* current EEPROM list entry */
    EC_T_BOOL                           m_bEEPChkSumErrorDetected;      /* Check sum error detected */

    /* topology check sub state machine variables */
    CEcBusSlave*                        m_pScanBusSlaveCur;             /* currently evaluated bus slave */
    EC_T_DWORD                          m_dwScanNextSlaveList;          /* next slave list to look in */
    enum {
        eResyncState_Idle,
        eResyncState_CheckGroupBegin,
        eResyncState_LookForNextGroup,
        eResyncState_CheckGroupEnd,
        eResyncState_ResyncOnNextSlave,
    } m_eResyncState;
    CEcBusSlave*                        m_pResyncBusSlave;              /* bus slave after the line break */
    
    EC_T_DWORD                          m_dwResyncCfgSlaveStartIdx;
    EC_T_DWORD                          m_dwResyncCfgSlavesCnt;
    EC_T_DWORD                          m_dwResyncCfgSlavesProcessed;

#if (defined INCLUDE_HOTCONNECT)
    EC_T_HOTCONNECT_GROUP*              m_pScanHCGroupCur;              /* Current HC group */
    EC_T_HOTCONNECT_GROUP*              m_pResyncHcGroupIncomplete;
#endif

    /* slave identification */
    CEcTimer                            m_oIdentifyBusSlaveTimeout;
    EC_T_BOOL                           m_bBusMismatchNotified;         /* flag to be sure that only 1 bus mismatch is notified during 1 bus scan */
    
    EC_T_BOOL                           m_bNextBusScanGenEni;           /* if EC_TRUE next bus scan read more information from slaves */

    EC_T_DWORD   StartTopologyDetection (EC_T_VOID);
    EC_T_DWORD   DoTopologyDetection    (EC_T_VOID);
    EC_T_DWORD   StartTopologyDetectionResync(CEcBusSlave* pBusSlaveResync);
    CEcBusSlave* GetNextBusSlaveToCheckForResync(EC_T_VOID);
    EC_T_DWORD   DoTopologyDetectionResync(EC_T_VOID);

#if (defined INCLUDE_MASTER_OBD)
    EC_T_VOID StorePresentSlave(CEcBusSlave* pBusSlave, EC_T_DWORD dwODIdx);
    EC_T_VOID StoreAbsentSlave(CEcSlave*     pCfgSlave, EC_T_DWORD dwODIdx);
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    EC_T_VOID NotifyLineCrossed(CEcBusSlave* pBusSlave);
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    EC_T_VOID NotifyJunctionRedChange(CEcBusSlave* pBusSlave);
#endif
    EC_T_VOID NotifyPdiWatchdog(CEcBusSlave* pBusSlave);
    EC_T_VOID NotifyUnexpectedLocation(CEcBusSlave* pBusSlave);
    EC_T_VOID NotifyBusMismatch(CEcBusSlave* pBusSlaveUnexpected, CEcSlave* pCfgSlaveMissing);

public:
    typedef enum _ECBTSM_T_ESTATES {
        ecbtsm_unknown              ,       /* unknown state, before state machine did start */

        ecbtsm_start                ,       /* start state, from here the state machine decides the way to go */
        ecbtsm_restart              ,
        ecbtsm_start_wait           ,
        ecbtsm_start_done           ,
        
        ecbtsm_collectinfo          ,       /* collect bus information, in this state slave objects are created and EEPROM are read from slave nodes */
        ecbtsm_collectinfo_wait     ,
        ecbtsm_collectinfo_done     ,

        ecbtsm_checktopo            ,       /* check network topology information against the ENI file */
        ecbtsm_checktopo_wait       ,
        ecbtsm_checktopo_done       ,

        ecbtsm_writefixaddr         ,       /* write fixed addresses from ENI file to slave nodes */
        ecbtsm_writefixaddr_wait    ,
        ecbtsm_writefixaddr_done    ,

        ecbtsm_refresh_slaves_presence,     /* renumber bus slaves, evaluates dirty flags and check group completion */
        ecbtsm_refresh_slaves_presence_wait,
        ecbtsm_refresh_slaves_presence_done,

        ecbtsm_notify_application,          /* send pending notifications */
        ecbtsm_notify_application_wait,
        ecbtsm_notify_application_done,

        ecbtsm_storeinfo            ,       /* store slave node information from topology to Master Object Dictionary */
        ecbtsm_storeinfo_wait       ,
        ecbtsm_storeinfo_done       ,

#if (defined INCLUDE_HOTCONNECT)
        ecbtsm_borderclose          ,       /* perform border close, if desired in HotConnect mode */
        ecbtsm_borderclose_wait     ,
        ecbtsm_borderclose_done     ,
#endif
        ecbtsm_aliasaddressing      ,       /* activate alias addressing on slave nodes, if desired */
        ecbtsm_aliasaddressing_wait ,
        ecbtsm_aliasaddressing_done ,

        ecbtsm_checkdlstatusint     ,       /* check interrupt source and set return value apropriate */
        ecbtsm_checkdlstatusint_wait,
        ecbtsm_checkdlstatusint_done,


        /* requestable states */
        ecbtsm_busscan_automatic    ,       /* automatic bus scan */
        ecbtsm_busscan_manual       ,       /* manual bus scan */
        ecbtsm_busscan_accepttopo   ,       /* accept topology change */
        ecbtsm_dlstatus_ist         ,       /* read DL Status from each slave to find slave generating the DL status interrupt */
        ecbtsm_alstatusrefresh      ,       /* read AL Status from each slave to find slave notifying AL status error or generating AL status interrupt */

        /* auxiliary states */
        ecbtsm_reqto_wait           ,       /* a timeout did occur, all requests shall be stopped from sub state machines */
        ecbtsm_reqto                ,       /* sub state machines did reach timeout state, release request */
        ecbtsm_stuck                ,       /* this state should never be reached, it means the state machine(s) did reach an undefined state. This state is
                                             * implemented to recover from fatal errors. */
        ecbtsm_idle                 ,       /* initial state, where state machine is not busy */
        ecbtsm_error                ,       /* error occured while scan */

        ecbtsm_BCppDummy            = 0xFFFFFFFF
    } ECBTSM_T_ESTATES;

    EC_T_CHAR* ESTATESText(ECBTSM_T_ESTATES eState);

    typedef enum _ECBTSM_T_ESUBSTATES {
        ecbtsms_unknown             ,       /* initial state machine state, before any state machine is started */

        ecbtsms_start               ,       /* initial state */
        
        ecbtsms_countslaves         ,       /* count slaves on bus */
        ecbtsms_countslaves_send    ,
        ecbtsms_countslaves_wait    ,
        ecbtsms_countslaves_done    ,

        ecbtsms_getbusslaves        ,       /* get information about slaves on bus */
        ecbtsms_getbusslaves_send   ,
        ecbtsms_getbusslaves_wait   ,
        ecbtsms_getbusslaves_done   ,

        ecbtsms_writetmpaddr        ,       /* write temporary slave fixed address to ensure consistency while performing BT data collection */
        ecbtsms_writetmpaddr_send   ,
        ecbtsms_writetmpaddr_wait   ,
        ecbtsms_writetmpaddr_done   ,

        ecbtsms_chkwrttmpaddr       ,       /* check if write of temporary slave fixed address was successful */
        ecbtsms_chkwrttmpaddr_send  ,
        ecbtsms_chkwrttmpaddr_wait  ,       /* wait until all sent datagrams are processed */
        ecbtsms_chkwrttmpaddr_done  ,

        ecbtsms_readalstatus        ,       /* read AL status */
        ecbtsms_readalstatus_send   ,
        ecbtsms_readalstatus_wait   ,
        ecbtsms_readalstatus_done   ,

        ecbtsms_ackslaveerror       ,       /* acknowledge slave error */
        ecbtsms_ackslaveerror_send  ,
        ecbtsms_ackslaveerror_wait  ,
        ecbtsms_ackslaveerror_done  ,

        ecbtsms_getportstatus       ,       /* Get Port State information per slave */
        ecbtsms_getportstatus_send  ,
        ecbtsms_getportstatus_wait  ,
        ecbtsms_getportstatus_done  ,

        ecbtsms_readportctrl        ,       /* read DL Control register to check how port's states are, to have information about topology */
        ecbtsms_readportctrl_send   ,
        ecbtsms_readportctrl_wait   ,
        ecbtsms_readportctrl_done   ,

        ecbtsms_writeportctrl       ,       /* write DL Control register to correct Port configuration to a valid mode */
        ecbtsms_writeportctrl_send  ,
        ecbtsms_writeportctrl_wait  ,   
        ecbtsms_writeportctrl_done  ,

        ecbtsms_refreshportstatus     ,     /* refresh port status after write port control */
        ecbtsms_refreshportstatus_send,
        ecbtsms_refreshportstatus_wait,
        ecbtsms_refreshportstatus_done,

        ecbtsms_detect_linecrossed,         /* start of line crossed detection */

#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
#if (defined INCLUDE_RED_DEVICE)
        ecbtsms_redcloseport        ,       /* close redundancy port */
        ecbtsms_redcloseport_wait   ,
        ecbtsms_redcloseport_done   ,
#endif /* INCLUDE_RED_DEVICE */

        ecbtsms_latchrecvtimes      ,       /* trigger port receive times latching */
        ecbtsms_latchrecvtimes_wait ,
        ecbtsms_latchrecvtimes_done ,

        ecbtsms_readrecvtimes       ,       /* read port receive times */
        ecbtsms_readrecvtimes_send  ,
        ecbtsms_readrecvtimes_wait  ,
        ecbtsms_readrecvtimes_done  ,

#if (defined INCLUDE_RED_DEVICE)
        ecbtsms_redopenport         ,      /* (re)open redundancy port */
        ecbtsms_redopenport_send    ,
        ecbtsms_redopenport_wait    ,
        ecbtsms_redopenportrefreshportstatus_send, /* refresh dl status of bus slave at redundancy port */
        ecbtsms_redopenportrefreshportstatus_wait,
        ecbtsms_redopenport_done    ,
#endif /* INCLUDE_RED_DEVICE */
#endif /* INCLUDE_DC_SUPPORT || INCLUDE_LINE_CROSSED_DETECTION */
        ecbtsms_detect_linecrossed_done,

        ecbtsms_readinterrupt       ,       /* Read Ecat Event Mask */
        ecbtsms_readinterrupt_send  ,
        ecbtsms_readinterrupt_wait  ,
        ecbtsms_readinterrupt_done  ,

        ecbtsms_cfginterrupt        ,       /* Configure Ecat Event */
        ecbtsms_cfginterrupt_send   ,
        ecbtsms_cfginterrupt_wait   ,
        ecbtsms_cfginterrupt_done   ,

        ecbtsms_readbusconfig       ,       /* Readout EEPROM Information from slave nodes SII interface */
        ecbtsms_readbusconfig_wait  ,
        ecbtsms_readbusconfig_done  ,

        ecbtsms_readpdicontrol      ,       /* Read out PDI Control to know about Power safe mode etc. of slave node */
        ecbtsms_readpdicontrol_send ,
        ecbtsms_readpdicontrol_wait ,
        ecbtsms_readpdicontrol_done ,

        ecbtsms_probedcunit         ,       /* probe DC unit */
#if (defined INCLUDE_DC_SUPPORT)
        ecbtsms_probedcunit_send    ,
        ecbtsms_probedcunit_wait    ,
#endif
        ecbtsms_probedcunit_done    ,

        ecbtsms_buildbustopology     ,      /* build bus topology and check if dl status of any port changed and port is not closed by application */
        ecbtsms_buildbustopology_wait,
        ecbtsms_buildbustopology_done,

        ecbtsms_parseports          ,       /* check ports to perform border close if required */
        ecbtsms_parseports_wait     ,
        ecbtsms_parseports_done     ,


        ecbtsms_findintsource       ,       /* find interrupting slave */
        ecbtsms_findintsource_send  ,
        ecbtsms_findintsource_wait  ,
        ecbtsms_findintsource_done  ,

        ecbtsms_ackinterrupt        ,       /* acknowledge found interrupt */
        ecbtsms_ackinterrupt_send   ,
        ecbtsms_ackinterrupt_wait   ,
        ecbtsms_ackinterrupt_done   ,

        ecbtsms_activatealias       ,       /* Send activation command to second station address for addressing */
        ecbtsms_activatealias_wait  ,
        ecbtsms_activatealias_done  ,

        ecbtsms_writefixaddr        ,       /* write slave fixed address to slaves (before performing border close) */
        ecbtsms_writefixaddr_send   ,
        ecbtsms_writefixaddr_wait   ,
        ecbtsms_writefixaddr_done   ,

        ecbtsms_checkfixedaddress   ,
        ecbtsms_checkfixedaddress_send,
        ecbtsms_checkfixedaddress_wait,     /* check slave fixed address */
        ecbtsms_checkfixedaddress_done,

        ecbtsms_collectbusscan      ,
#if (defined INCLUDE_HOTCONNECT)
        ecbtsms_borderclose         ,
#endif
        ecbtsms_collectdlstatus     ,       /* Collect DL Status Information for IST */
        ecbtsms_collectalstatus     ,       /* Collect AL status information */

        ecbtsms_enainterrupt        ,       /* enable DL Status Interrupt */
        ecbtsms_disinterrupt        ,       /* disable DL Status Interrupt */
        ecbtsms_aliasactive         ,
        ecbtsms_wrtfaddr            ,

        ecbtsms_timeout             ,       /* timeout occurred within top level state machine or here */
        ecbtsms_error               ,       /* sub state machine error state, is reached when something weird happens */

        ecbtsms_BCppDummy           = 0xFFFFFFFF
    } ECBTSM_T_ESUBSTATES;

    EC_T_CHAR* ESUBSTATESText(ECBTSM_T_ESUBSTATES eState);

    typedef enum _ECBTSM_T_EEEPSTATES
    {
        ecbtsme_unknown             ,       /* state machine initial state, if state machine did not run */
        ecbtsme_start               ,       /* start state, where state machine decides based on target state which way to go */

        ecbtsme_acq_eep_access      ,       /* Assign EEPROM of all slaves to EtherCAT Master (not PDI) */
        ecbtsme_acq_eep_access_wait ,
        ecbtsme_acq_eep_access_done ,

        ecbtsme_chkprebusy          ,       /* check and wait if EEPROM is busy before sending SII command */
        ecbtsme_chkprebusy_wait     ,
        ecbtsme_chkprebusy_done     ,

        ecbtsme_checksumerror       ,       /* look for slave(s) with checksum error */
        ecbtsme_checksumerror_send  ,
        ecbtsme_checksumerror_wait  ,
        ecbtsme_checksumerror_done  ,

        ecbtsme_writeaddr           ,       /* write EEPROM SII Address to all slaves */
        ecbtsme_writeaddr_send      ,
        ecbtsme_writeaddr_wait      ,
        ecbtsme_writeaddr_done      ,
        
        ecbtsme_chkbusy             ,       /* check and wait, whether slaves are done with processing the SII request */
        ecbtsme_chkbusy_wait        ,
        ecbtsme_chkbusy_done        ,

        ecbtsme_readvalue           ,       /* Read SII Data from each slave individually */
        ecbtsme_readvalue_send      ,
        ecbtsme_readvalue_wait      ,
        ecbtsme_readvalue_done      ,

        ecbtsme_done                ,

        ecbtsme_timeout             ,       /* this or upper level state machine did reach timeout condition */
        ecbtsme_error               ,       /* state machine hit an error condition, this state is used to recover from a fatal state error */

        ecbtsme_BCppDummy           = 0xFFFFFFFF
    } ECBTSM_T_EEEPSTATES;

    EC_T_CHAR* EEEPSTATESText(ECBTSM_T_EEEPSTATES eState);

    typedef enum _ECBTSMCHK_T_STATES
    {
        ecbtsmchk_unknown           ,       /* initial state, if state machine did not run */
        ecbtsmchk_idle              ,       /* state machine is idle / passive */
        ecbtsmchk_start             ,       /* state machine initial state, decision which way to go is done here, based on desired state */

        ecbtsmchk_process           ,       /* explore topology */
        ecbtsmchk_process_wait      ,       /* waiting for identification command result */

        ecbtsmchk_clean             ,       /* clean slaves on bus modified by topology check */
        ecbtsmchk_clean_send        ,
        ecbtsmchk_clean_wait        ,
        ecbtsmchk_clean_done        ,

        ecbtsmchk_done              ,       /* check performed target state */
        ecbtsmchk_error             ,       /* state machine did reached unexpected condition */

        ecbtsmchk_BCppDummy         = 0xFFFFFFFF
    } ECBTSMCHK_T_STATES;

    EC_T_CHAR* ECHKSTATESText(ECBTSMCHK_T_STATES eState);

    typedef struct _ECBTSM_REQ
    {
        ECBTSM_T_ESTATES    eState;
        CEcTimer            oTimeout;
        EC_T_BOOL           bUsed;
    } ECBTSM_REQ;

    EC_T_DWORD  RequestState(       ECBTSM_T_ESTATES    eState          
                                   ,EC_T_DWORD          dwTimeout       
                                   ,ECBTSM_REQ**        pHandle         );
    EC_T_VOID   ReleaseReq(         ECBTSM_REQ*         pHandle         );

#if (defined INCLUDE_HOTCONNECT)
    EC_T_DWORD  CountUnusedFreeHookElements( 
                                    EC_T_VOID                           );
#endif
    EC_T_DWORD  ChangePortAnalysis( CEcBusSlave*        pBusSlave       );

    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_VOID BTStateMachine(    EC_T_VOID  );
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_VOID BTSubStateMachine( EC_T_VOID  );
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_VOID BTEEPStateMachine( EC_T_VOID  );
    EC_INSTRUMENT_MOCKABLE_FUNC EC_T_VOID BTChkStateMachine( EC_T_VOID  );

    EC_T_DWORD  FreeRanges(         EC_T_VOID                           )
                    { return ECBT_ADDRRANGE_AMOUNT; }
    EC_T_VOID   FreeRangeSet(       EC_T_DWORD          dwIdx
                                   ,EC_T_WORD           wBegin
                                   ,EC_T_WORD           wEnd            );

    struct _EC_T_MEMORY_LOGGER* GetMemLog();

#if (defined INSTRUMENT_MASTER) || (defined ECBT_PRINTBUSSLAVESLIST)
    EC_T_VOID PrintBusSlaveList(CEcBusSlave* pBusSlave);
#endif
#if (defined INSTRUMENT_MASTER) || (defined ECBT_PRINTBUSSLAVESTREE)
    EC_T_VOID PrintBusSlaveTreeNode(CEcBusSlave* pBusSlave, EC_T_DWORD dwTabs);
    EC_T_VOID PrintBusSlaveTree(CEcBusSlave* pBusSlave, EC_T_BOOL bRedLine);
#endif

private:
EC_INSTRUMENT_MOCKABLE_VAR
    /* bus topology state machine */
    ECBTSM_REQ              m_oRequest[2];      /* request buffers */    
    ECBTSM_REQ*             m_pRequest;         /* queued request */
    ECBTSM_REQ*             m_pCurRequest;      /* current request */
    ECBTSM_T_ESTATES        m_eCurState;        /* current state */
    ECBTSM_T_ESTATES        m_eReqState;        /* requested state */

    /* sub state machine */
    ECBTSM_T_ESUBSTATES     m_eCurSubState;
    ECBTSM_T_ESUBSTATES     m_eReqSubState;
    EC_T_DWORD              m_dwSubResult;

    /* EEPROM sub state machine */
    ECBTSM_T_EEEPSTATES     m_eCurEEPState;
    ECBTSM_T_EEEPSTATES     m_eReqEEPState;
    EC_T_DWORD              m_dwEEPResult;

    /* topology check sub state machine */
    ECBTSMCHK_T_STATES      m_eCurChkState;
    ECBTSMCHK_T_STATES      m_eReqChkState;
    EC_T_DWORD              m_dwChkResult;

    /* handle to currently running Bus Scan (BS) operation */
    ECBTSM_REQ*             m_pBSStartReq;

    /* range of free addresses which can be used for temporary fixed addresses */
    ECBT_T_ADDRRANGE        m_abFreeAddresses[ECBT_ADDRRANGE_AMOUNT];

    EC_T_VOID   ReleaseTmpFixedAddress(EC_T_VOID);
    EC_T_WORD   TakeTmpFixedAddress( EC_T_VOID );
};

#if (defined INCLUDE_RESCUE_SCAN)
class CRescueScan
{
public:
    CRescueScan(CAtEmInterface* pInterface);

    EC_T_DWORD RescueScan(EC_T_DWORD dwTimeout);

private:
    EC_T_BOOL  GetNextPortToCheck(EC_T_WORD* pwAutoIncAddr, EC_T_WORD* pwPortIdx);
    EC_T_DWORD CheckPortForFrameLoss(EC_T_WORD wAutoIncAddr, EC_T_WORD wPortIdx);

    static const EC_T_WORD  c_awNextPortToCheckOrder[];
    static const EC_T_DWORD c_dwDlControlCloseAllButPort0;

    CAtEmInterface* m_pInterface;
    CEcMaster* m_pMaster;
    CEcTimer m_Timeout;
};
#endif /* INCLUDE_RESCUE_SCAN */

#endif /* INC_ECBUSTOPOLOGY */

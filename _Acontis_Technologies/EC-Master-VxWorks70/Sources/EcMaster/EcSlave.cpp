/*-----------------------------------------------------------------------------
 * EcSlave.cpp              cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT master: slave command management
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include <EcInterfaceCommon.h>
#include <EthernetServices.h>
#include <EcServices.h>
#include <EcText.h>

#include <EcLink.h>
#include <EcInterface.h>
#include <EcMasterCommon.h>
#include <EcEthSwitch.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcDevice.h>

#include <EcMaster.h>
#include <EcSlave.h>

#if (defined INCLUDE_MASTER_OBD)
  #include <EcSdoServ.h>
#endif

#include <EcConfig.h>
#include <AtEthercatPrivate.h>
#include <EcInvokeId.h>

#include <EcBusTopology.h>
#if (defined INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif

/*-COMPILER SETTINGS---------------------------------------------------------*/
#undef  LINK_DEBUG

/*-DEFINES-------------------------------------------------------------------*/
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x40)
#define DCAPROVAL       ((EC_T_BYTE)0x01)

#define MAILBOX_POLL_RCV_ONLY EC_TRUE   /* EC_TRUE: mailbox polling - read only mbx header
                                         * EC_FALSE: mailbox polling - read whole mbx
                                         */
#define INIT_CMD_DEFAULT_TIMEOUT        5000        /* 5000 ms */

/*-MACROS--------------------------------------------------------------------*/
#ifdef LINK_DEBUG               
#define LINK_DBG_TRACE(trc,msg) m_poEcDevice->LinkDbgMsg( 2, 0xF8, (trc), (msg))
#define LINK_DBG_TRACE1(trc,msg,arg1) m_poEcDevice->LinkDbgMsg( 2, 0xF8, (trc), (msg), (arg1))
#else
#define LINK_DBG_TRACE(trc,msg)
#define LINK_DBG_TRACE1(trc,msg,arg1) 
#endif

#ifdef INCLUDE_LOG_MESSAGES
static EC_T_CHAR* CoESdoCcsText(EC_T_BYTE ccs)
{
    switch (ccs)
    {
    case SDO_CCS_DOWNLOAD_SEGMENT:      return (EC_T_CHAR*)"DownSeg";
    case SDO_CCS_INITIATE_DOWNLOAD:     return (EC_T_CHAR*)"InitDown";
    case SDO_CCS_INITIATE_UPLOAD:       return (EC_T_CHAR*)"InitUp";
    case SDO_CCS_UPLOAD_SEGMENT:        return (EC_T_CHAR*)"UpSeg";
    case SDO_CCS_ABORT_TRANSFER:        return (EC_T_CHAR*)"Abort";
    }
    return (EC_T_CHAR*)"";
}
#endif

#ifdef INCLUDE_SOE_SUPPORT
#ifdef DEBUGTRACE
static EC_T_CHAR* SoEOpCodeText(EC_T_BYTE byOpCode)
{
    switch (byOpCode)
    {
    case ECAT_SOE_OPCODE_RRQ:       return (EC_T_CHAR*)"ReadReq";
    case ECAT_SOE_OPCODE_RRS:       return (EC_T_CHAR*)"ReadRes";
    case ECAT_SOE_OPCODE_WRQ:       return (EC_T_CHAR*)"WriteReq";
    case ECAT_SOE_OPCODE_WRS:       return (EC_T_CHAR*)"WriteRes";
    case ECAT_SOE_OPCODE_NOTIFY:    return (EC_T_CHAR*)"Notif";
    case ECAT_SOE_OPCODE_SLV_INFO:  return (EC_T_CHAR*)"Slave Info";
    }
    return (EC_T_CHAR*)"";
}
#endif
#endif

static EC_INLINESTART EC_T_WORD GetTransition(struct TEcInitCmdDesc* p) { return EC_ECINITCMDDESC_GET_TRANSITION(p); } EC_INLINESTOP
static EC_INLINESTART EC_T_WORD GetTransition(struct TEcMailboxCmdDesc* p) { return EC_ECMBXCMDDESC_GET_TRANSITION(p); } EC_INLINESTOP
template<typename T>
static T* FindNextInitCmd(T** ppMbxCmdList, EC_T_WORD* pwCurIdx, EC_T_WORD wCount, EC_T_WORD wTransition);

template <typename T> EC_T_VOID FreeMbxProtocolDesc(T** ppDesc, struct _EC_T_MEMORY_LOGGER* pMLog)
{
    EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_SLAVE, pMLog, "FreeMbxProtocolDesc", (*ppDesc)->pPendMailboxCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
    SafeDelete((*ppDesc)->pPendMailboxCmdFifo);
    EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_SLAVE, pMLog, "FreeMbxProtocolDesc", (*ppDesc), sizeof(T));
    SafeDelete((*ppDesc));
}

/*-CEcSlave------------------------------------------------------------------*/

/********************************************************************************/
/** \brief CEcSlave constructor
*
* \return
*/
CEcSlave::CEcSlave(
    CEcMaster*              pMaster, 
    EC_T_ENI_SLAVE*         pEniSlave, 
    CEcDevice*              poEcDevice
                  ) 
: m_pMaster(pMaster)
, m_poEcDevice(poEcDevice)
, m_pEniSlave(pEniSlave)
, m_wFixedAddr(pEniSlave->wPhysAddr)
, m_wAutoIncAddr(pEniSlave->wAutoIncAddr)
{
    EC_T_BOOL  bCheckRevisionValidateLength = EC_FALSE;
    EC_T_INT   nOmronSetPreopInitCmdNextIdx = -1;
    EC_T_INT   nOmronClearSmsInitCmdIdx = -1;

    m_dwSlaveId = INVALID_SLAVE_ID;

    /* simple slave without mailbox */
    m_wCurrStateMachDevState         = DEVICE_STATE_UNKNOWN;
    m_wReqStateMachDevState          = DEVICE_STATE_UNKNOWN;

    m_wAlStatusNotified              = DEVICE_STATE_UNKNOWN;
    m_wUnexpStateCurAlStatusNotified = DEVICE_STATE_UNKNOWN;
    m_wUnexpStateExpAlStatusNotified = DEVICE_STATE_UNKNOWN;

    m_bRawTferPending        = EC_FALSE;
    m_dwRawTferStatus        = EC_E_NOERROR;
    m_wRawTferDataLen        = 0;
    m_pbyRawTferData         = EC_NULL;
                             
    m_bInitCmdFailedError    = EC_FALSE;
    m_bEnableLinkMessages    = EC_FALSE;
                             
    m_wSyncManagerEnabled    = 0;
                             
    m_wActTransition         = 0;
    m_wInitCmdsRetries       = 0;
    m_wInitCmdsCurrent       = INITCMD_INACTIVE;
    m_wMasterInitCmdsCurrent = INITCMD_INACTIVE;
    m_bStateMachineStable    = EC_TRUE;
    m_bStandardInitCmdsFirst = EC_FALSE;
    m_bStandardInitCmdsDone  = EC_FALSE;
    m_wInitCmdsCount         = 0;
    m_ppInitCmds             = EC_NULL;

    m_aCycCmdWkcList        = EC_NULL;
    m_dwNumCycCmdWkc        = 0;
    m_bCycErrorNotifyMasked = EC_FALSE;

#if (defined INCLUDE_DC_SUPPORT)
    m_bDcSendSyncActivation = EC_FALSE;
    m_bDclSendConfiguration = EC_FALSE;
    m_bDcInitializationRequired = EC_FALSE;
#endif

#if (defined INCLUDE_WKCSTATE)    
    m_wWkcStateInEntries  = 0;
    OsMemset(m_awWkcStateDiagOffsIn,  0, sizeof(m_awWkcStateDiagOffsIn));
    m_wWkcStateOutEntries = 0;
    OsMemset(m_awWkcStateDiagOffsOut, 0, sizeof(m_awWkcStateDiagOffsOut));
#endif /* INCLUDE_WKCSTATE */

    m_poSlaveLock = OsCreateLockTyped(eLockType_SPIN);

    m_wMaxNumInitCmds = (EC_T_WORD)(pEniSlave->wNumEcatInitCmds + 1);/* one more to avoid index overflow, see also CEcSlave::StartInitCmds() */
    if (0 == m_wMaxNumInitCmds)
    {
        m_ppInitCmds = EC_NULL;
    }
    else
    {
        m_ppInitCmds = EC_NEW(PEcInitCmdDesc[m_wMaxNumInitCmds]); 
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::m_ppInitCmds", m_ppInitCmds, m_wMaxNumInitCmds * sizeof(PEcInitCmdDesc));
    }
    if (m_ppInitCmds != EC_NULL)
    {
        EcInitCmdDesc* pInitCmdDesc = EC_NULL;
        EC_T_WORD i = 0;

        for (i = 0; i < pEniSlave->wNumEcatInitCmds; i++)
        {
            pInitCmdDesc = pEniSlave->apPkdEcatInitCmdDesc[i];

            if ((EC_ICMDHDR_GET_CMDIDX_CMD(&(pInitCmdDesc->EcCmdHeader)) == EC_CMD_TYPE_APWR) && (EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)) == ECREG_STATION_ADDRESS))
            {
                /* filter broadcast write of configured station address */
                /* because master setup all the fixed address already in the bus scan */
                continue;
            }

#if (defined INCLUDE_DC_SUPPORT)
            if (pEniSlave->sParm.bDc)
            {
                if (!m_bDcSendSyncActivation)
                {
                    /* only check this things on the way up from Pre-OP to Op */
                    if ((ECAT_INITCMD_P_S == (EC_ECINITCMDDESC_GET_TRANSITION((pInitCmdDesc))&ECAT_INITCMD_P_S)) 
                     || (ECAT_INITCMD_S_O == (EC_ECINITCMDDESC_GET_TRANSITION((pInitCmdDesc))&ECAT_INITCMD_S_O)))
                    {
                        if ((EC_CMD_TYPE_FPWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECM_DCS_DC_ACTIVATION_REGISTER == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
                        {
                            m_bDcSendSyncActivation = EC_TRUE;
                        }

                        if ((EC_CMD_TYPE_FPWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECM_DCS_DC_CYCLETIME0 == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                         && (sizeof(EC_T_DWORD) <= EC_ICMDHDR_GET_LEN(&(pInitCmdDesc->EcCmdHeader))))
                        {
                            m_bDcSendSyncActivation = EC_TRUE;
                        }

                        if ((EC_CMD_TYPE_FPWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECM_DCS_DC_CYCLETIME1 == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                         && (sizeof(EC_T_DWORD) <= EC_ICMDHDR_GET_LEN(&(pInitCmdDesc->EcCmdHeader))))
                        {
                            m_bDcSendSyncActivation = EC_TRUE;
                        }
                    }
                }


                if (!m_bDclSendConfiguration)
                {
                    /* only check this things on the way up from Pre-OP to Op */
                    if ((ECAT_INITCMD_P_S == (EC_ECINITCMDDESC_GET_TRANSITION((pInitCmdDesc))&ECAT_INITCMD_P_S)) 
                     || (ECAT_INITCMD_S_O == (EC_ECINITCMDDESC_GET_TRANSITION((pInitCmdDesc))&ECAT_INITCMD_S_O)))
                    {
                        if ((EC_CMD_TYPE_FPWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECREG_DCL_CTRL_LATCH == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                         && (sizeof(EC_T_WORD) <= EC_ICMDHDR_GET_LEN(&(pInitCmdDesc->EcCmdHeader))))
                        {
                            m_bDclSendConfiguration = EC_TRUE;
                        }

                        if ((EC_CMD_TYPE_FPWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECREG_DCL_CTRL_LATCH1 == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
                        {
                            m_bDclSendConfiguration = EC_TRUE;
                        }
                    }
                }
            }
#endif /* INCLUDE_DC_SUPPORT */

            /* evaluate startup checking */
            if ((EC_CMD_TYPE_APWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
             && (ECM_SB_EEP_SLV_CTRLSTATUS == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
            {
                switch (EC_GET_FRM_WORD((EcInitCmdDescData(pInitCmdDesc)+0x2)))
                {
                case ESC_SII_REG_VENDORID:       pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_VENDORID]    = eCheckEEPROMEntry_EQUAL; break;
                case ESC_SII_REG_PRODUCTCODE:    pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_PRODUCTCODE] = eCheckEEPROMEntry_EQUAL; break;
                case ESC_SII_REG_REVISIONNUMBER_LO: 
                    if (pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] == eCheckEEPROMEntry_ANY)
                    {
                        pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] = eCheckEEPROMEntry_LWEQUAL;
                        bCheckRevisionValidateLength = EC_TRUE;
                    }
                    else 
                    {
                        pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] = eCheckEEPROMEntry_EQUAL;
                    }
                    break;
                case ESC_SII_REG_REVISIONNUMBER_HI: 
                    if (pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] == eCheckEEPROMEntry_ANY)
                    {
                        pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] = eCheckEEPROMEntry_HWEQUAL;
                        bCheckRevisionValidateLength = EC_TRUE;
                    } 
                    else 
                    {
                        pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] = eCheckEEPROMEntry_EQUAL;
                    }
                    break;
                case ESC_SII_REG_SERIALNUMBER:   pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_SERIALNO] = eCheckEEPROMEntry_EQUAL; break;
                default: break;
                }
            }

            /* detailed check how to handle revision check */
            if (bCheckRevisionValidateLength)
            {
                if ((EC_CMD_TYPE_APRD == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                 && (ECM_SB_EEP_SLV_EEPDATA == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
                {
                    bCheckRevisionValidateLength = EC_FALSE;
                    if (EC_ICMDHDR_GET_LEN(&(pInitCmdDesc->EcCmdHeader)) == 4)
                    {
                        pEniSlave->aeCheckEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO] = eCheckEEPROMEntry_EQUAL;
                    }
                }
            }

#if (defined ADD_AL_CONTROL_FORCE_ERROR_CLEAR)
            /* check whether InitCmd is write access to AL Control */
            if (
                (ECREG_AL_CONTROL >= EC_ICMDHDR_GET_ADDR_ADO(&(pPkdEcInitCmdDesc->EcCmdHeader)))
                &&  (ECREG_AL_CONTROL <= (EC_ICMDHDR_GET_ADDR_ADO(&(pPkdEcInitCmdDesc->EcCmdHeader)) + (EC_GETWORD(&(pPkdEcInitCmdDesc->EcCmdHeader.uLen.slength.len))&((1<<12)-1))))
                )
            {
                /* if is write access */
                if (
                    (EC_CMD_TYPE_APWR == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_APRW == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_FPWR == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_FPRW == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_BWR  == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_BRW  == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_LWR  == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    || (EC_CMD_TYPE_LRW  == pPkdEcInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                    )
                {
                    EC_T_PBYTE  pbyPtr  = EC_NULL;
                    pbyPtr = (EC_T_PBYTE)(EC_ENDOF(pPkdEcInitCmdDesc));
                    pbyPtr[(EC_ICMDHDR_GET_ADDR_ADO(&(pPkdEcInitCmdDesc->EcCmdHeader))-ECREG_AL_CONTROL)] |= ECR_ALCTRL_ACK_ERROR_IND;
                }
            }
#endif
            m_ppInitCmds[m_wInitCmdsCount++] = pInitCmdDesc;

            /* reorder InitCmd for OMRON slaves to avoid slave error during falling transition */
            if (0x00000083 == GetVendorId())
            {
                if ((ECAT_INITCMD_O_P == (EC_ECINITCMDDESC_GET_TRANSITION((pInitCmdDesc))&ECAT_INITCMD_O_P)) 
                 || (ECAT_INITCMD_S_P == (EC_ECINITCMDDESC_GET_TRANSITION((pInitCmdDesc))&ECAT_INITCMD_S_P)))
                {
                    /* reorder check device state for PREOP just after "set device state to PREOP" */
                    if (-1 == nOmronSetPreopInitCmdNextIdx)
                    {
                        /* look for "set device state to PREOP" */
                        if ((EC_CMD_TYPE_FPWR   == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECREG_AL_CONTROL   == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                         && (DEVICE_STATE_PREOP == (EC_GET_FRM_WORD(EcInitCmdDescData(pInitCmdDesc))&DEVICE_STATE_MASK)))
                        {
                            nOmronSetPreopInitCmdNextIdx = (EC_T_INT)m_wInitCmdsCount;
                        }
                    }
                    else
                    {
                        /* look for "check device state for PREOP" */
                        if ((EC_CMD_TYPE_FPRD   == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECREG_AL_STATUS    == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                         && (DEVICE_STATE_PREOP == (EC_GET_FRM_WORD(EcInitCmdDescVData(pInitCmdDesc))&DEVICE_STATE_MASK)))
                        {
                        EcInitCmdDesc* pInitCmdInserted = m_ppInitCmds[m_wInitCmdsCount-1];
                        EcInitCmdDesc* pInitCmdMoved    = EC_NULL;

                            for (EC_T_INT nInitCmdIdx = nOmronSetPreopInitCmdNextIdx; nInitCmdIdx < (EC_T_INT)m_wInitCmdsCount; nInitCmdIdx++)
                            {
                                pInitCmdMoved = m_ppInitCmds[nInitCmdIdx];
                                m_ppInitCmds[nInitCmdIdx] = pInitCmdInserted;
                                pInitCmdInserted = pInitCmdMoved;
                            }
                        }
                    }
                    /* reorder "clear fmmu" before "clear sms" */
                    if (-1 == nOmronClearSmsInitCmdIdx)
                    {
                        /* look for "clear sms" */
                        if (pEniSlave->sParm.bMailbox)
                        {
                            if ((EC_CMD_TYPE_FPWR    == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                             && (ECREG_SYNCMANAGER2  == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
                            {
                                nOmronClearSmsInitCmdIdx = (EC_T_INT)m_wInitCmdsCount - 1;
                            }
                        }
                        else
                        {
                            if ((EC_CMD_TYPE_FPWR         == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                             && (ECREG_SYNCMANAGER_CONFIG == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
                            {
                                nOmronClearSmsInitCmdIdx = (EC_T_INT)m_wInitCmdsCount - 1;
                            }
                        }
                    }
                    else
                    {
                        /* look for "clear fmmu" */
                        if ((EC_CMD_TYPE_FPWR   == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
                         && (ECREG_FMMU_CONFIG <= EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                         && ((ECREG_FMMU_CONFIG + ECREG_FMMU_MAX_NUMOF * 16) > EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
                        {
                        EcInitCmdDesc* pInitCmdInserted = m_ppInitCmds[m_wInitCmdsCount-1];
                        EcInitCmdDesc* pInitCmdMoved    = EC_NULL;

                            for (EC_T_INT nInitCmdIdx = nOmronClearSmsInitCmdIdx; nInitCmdIdx < (EC_T_INT)m_wInitCmdsCount; nInitCmdIdx++)
                            {
                                pInitCmdMoved = m_ppInitCmds[nInitCmdIdx];
                                m_ppInitCmds[nInitCmdIdx] = pInitCmdInserted;
                                pInitCmdInserted = pInitCmdMoved;
                            }
                            nOmronClearSmsInitCmdIdx++;
                        }
                    }
                }
            }
        }
    }
#if (defined INCLUDE_SLAVE_STATISTICS)
    OsMemset(m_wRxErrCounter, 0, (sizeof(EC_T_WORD)*4));
    OsMemset(m_byFwdRxErrCounter, 0, (sizeof(EC_T_BYTE)*4));
    m_byProcUnitErrCounter  = 0;
    m_byPDIErrCounter       = 0;
    OsMemset(m_byLostLinkCounter, 0, (sizeof(EC_T_BYTE)*4));
#endif
          
    if (0 == pEniSlave->wNumPreviousPorts)
    {
        m_pPreviousPort = EC_NULL;
    }
    else
    {
        m_pPreviousPort = pEniSlave->aoPreviousPort;
    }
    m_ePresenceOnBus    = eUNKNOWN;
    m_ePresenceNotified = eUNKNOWN;
    m_bIsPresenceValid  = EC_TRUE;
    m_bIgnoreAbsence    = EC_FALSE;
    m_eMaxState         = eEcatState_OP;
    m_bIsOptional       = EC_FALSE;
    m_bCancelStateMachine = EC_FALSE;
    m_bIsGHCGroupHead   = EC_FALSE;
    m_dwHCGroupId       = 0;
    m_pBusSlave         = EC_NULL;
    m_pBusSlaveCandidate = EC_NULL;

    m_bDeviceEmulation = EC_FALSE;

    /* initialization for completeness */
#if (defined INCLUDE_WKCSTATE)
    m_wWkcStateInEntries  = 0;
    m_wWkcStateOutEntries = 0;
#endif

#if (defined INCLUDE_DC_SUPPORT)    
    /* execute DC initialization for bridge slaves (primary) even if there is no DC initialization for them in the ENI file */
    if ((!m_bDcSendSyncActivation && !m_bDclSendConfiguration) && ((0 != m_pEniSlave->dwDcRegisterCycleTime0) || (0 != m_pEniSlave->dwDcRegisterCycleTime1)))
    {
        m_bDcInitializationRequired = EC_TRUE;
    }
#endif

#if (defined INCLUDE_CONFIG_EXTEND)
    m_bRemoveOnTimer = EC_FALSE;
#endif
}

/********************************************************************************/
/** \brief CEcSlave destructor
*
* \return
*/
CEcSlave::~CEcSlave()
{
    if (EC_NULL != m_aCycCmdWkcList)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_FACTORY, "CEcSlave::~m_aCycCmdWkcList", m_aCycCmdWkcList, m_dwNumCycCmdWkc * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        SafeOsFree(m_aCycCmdWkcList);
    }
    if (EC_NULL != m_ppInitCmds)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::~m_ppInitCmds", m_ppInitCmds, m_wMaxNumInitCmds * sizeof(PEcInitCmdDesc));
        SafeDeleteArray(m_ppInitCmds);
    }
    OsDeleteLock(m_poSlaveLock);
}

#if (defined INCLUDE_SLAVE_STATISTICS)

/***************************************************************************************************/
/**
\brief  Get Slave's statistics counter.

Counters are collected on a regularly base (default 60secs) and show errors on Ethernet Layer.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSlave::GetStatisticCounters(
    EC_T_SLVSTATISTICS_DESC*    pStatistics     /**< [in]   Buffer to carry statistics data */
                                         )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_BYTE   n           = 0;

    if (EC_NULL == pStatistics)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    for(n = 0; n < 4; n++ )
    {
        EC_SETWORD(&(pStatistics->wRxErrorCounter[n]), m_wRxErrCounter[n]);
        
        pStatistics->byFwdRxErrorCounter[n] = m_byFwdRxErrCounter[n];
        pStatistics->byLostLinkCounter[n]   = m_byLostLinkCounter[n];
    }

    pStatistics->byEcatProcUnitErrorCounter = m_byProcUnitErrCounter;
    pStatistics->byPDIErrorCounter          = m_byPDIErrCounter;

    pStatistics->wAlStatusCode              = GetAlStatusCode();

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
/***************************************************************************************************/
/**
\brief  Clear Slave's statistics counter.

Counters are collected on a regularly base (default 60secs) and show errors on Ethernet Layer.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CEcSlave::ClearStatisticCounters(EC_T_VOID) {
    EC_T_BYTE n = 0;

    for (n = 0; n < 4; n++)
    {
        m_wRxErrCounter[n]     = 0;        
        m_byFwdRxErrCounter[n] = 0;
        m_byLostLinkCounter[n] = 0;
    }

    m_byProcUnitErrCounter = 0;
    m_byPDIErrCounter = 0;
}
#endif

/********************************************************************************/
/** \brief Request EtherCAT slave state machine to change to the specified state.
*
* \return
*/
EC_T_VOID CEcSlave::RequestSmState(EC_T_STATE state)
{
    EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcSlave::RequestSmState() %s: %s (0x%x)\n", m_pEniSlave->szName, SlaveDevStateText((EC_T_WORD)state), (EC_T_WORD)state));

    if (!IsPresentOnBus())
    {
        /* skip absent slave */
        goto Exit;
    }
    if (state > GetMaxState())
    {
        goto Exit;
    }
    m_bInitCmdFailedError = EC_FALSE;
    m_bStandardInitCmdsDone = EC_FALSE;

#if (defined INCLUDE_MASTER_OBD)
    m_pMaster->m_pSDOService->UpdateReqSlaveState((EC_T_WORD)(0 - GetAutoIncAddress()), state);
#endif
    m_wReqStateMachDevState = (EC_T_WORD)state;
    if ((EC_T_WORD)state == GetDeviceState())
    {
        /* synchronize slave state machine */
        SetSmDeviceState(GetDeviceState());
    }
    if (m_wCurrStateMachDevState == m_wReqStateMachDevState)
    {
        /* synchronize slave state machine */
        SetSmDeviceState(GetDeviceState());
    }
    if (DEVICE_STATE_UNKNOWN == m_wAlStatusNotified)
    {
        m_pMaster->NotifySlaveStateChanged(this);
    }

    /* enable InitCmds for II- and PP-transition */
    m_bStateMachineStable = EC_FALSE;

    OsDbgAssert(!m_bCycErrorNotifyMasked);
    if (!m_bCycErrorNotifyMasked)
    {
        m_pMaster->IncCycErrorNotifyMaskedCnt();
        m_bCycErrorNotifyMasked = EC_TRUE;
    }

    m_pMaster->m_pSlavesStateMachineFifo->Add(this);

Exit:
    return;
}

/********************************************************************************/
/** \brief Reset the state machine of the EtherCAT slave.
*
* \return
*/
EC_T_VOID CEcSlave::ResetSlaveStateMachine(EC_T_VOID)
{
    m_wActTransition         = 0;
    m_bStateMachineStable    = EC_TRUE;
    m_wInitCmdsCurrent       = INITCMD_INACTIVE;
    m_wMasterInitCmdsCurrent = INITCMD_INACTIVE;
    m_wReqStateMachDevState  = DEVICE_STATE_UNKNOWN;
    m_wAlStatusNotified      = DEVICE_STATE_UNKNOWN;
    SetSmDeviceState(DEVICE_STATE_UNKNOWN);

    if (m_bCycErrorNotifyMasked)
    {
        m_bCycErrorNotifyMasked = EC_FALSE;
        m_pMaster->DecCycErrorNotifyMaskedCnt();
    }
}

/********************************************************************************/
/** \brief Stabilize the state machine of the EtherCAT slave.
*
* \return
*/
EC_T_VOID CEcSlave::StabilizeSlaveStateMachine(EC_T_VOID)
{
    m_wActTransition         = 0;
    m_bStateMachineStable    = EC_TRUE;
    m_wInitCmdsCurrent       = INITCMD_INACTIVE;
    m_wMasterInitCmdsCurrent = INITCMD_INACTIVE;
    m_wReqStateMachDevState  = DEVICE_STATE_UNKNOWN;
    if (DEVICE_STATE_INIT == GetDeviceState())
    {
        m_wSyncManagerEnabled = 0;
    }
    if (DEVICE_STATE_UNKNOWN == m_pMaster->GetMasterDeviceState())
    {
        /* SetMasterState() not called until now */
        SetSmDeviceState(DEVICE_STATE_UNKNOWN);
    }
    else
    {
        SetSmDeviceState(GetDeviceState());
    }
    if (m_bCycErrorNotifyMasked)
    {
        m_bCycErrorNotifyMasked = EC_FALSE;
        m_pMaster->DecCycErrorNotifyMaskedCnt();
    }
}

/** \brief Cancel the state machine of the EtherCAT slave.
*
* \return
*/
EC_T_VOID CEcSlave::CancelSlaveStateMachine(EC_T_VOID)
{
    m_wReqStateMachDevState = m_wCurrStateMachDevState;

    if (AreInitCmdsActive())
    {
        /* set trigger */
        m_bCancelStateMachine = EC_TRUE;
    }
    EC_TRACEMSG( EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcSlave::CancelSlaveStateMachine() %s (curr=0x%x, req=0x%x)\n", 
        m_pEniSlave->szName, m_wCurrStateMachDevState, m_wReqStateMachDevState));
}

/********************************************************************************/
/** \brief Executes the state machine of the EtherCAT slave.
*
* \return EC_TRUE in case the requested state matches the current device state.
*/
EC_T_DWORD  CEcSlave::SlaveStateMachine()
{
EC_T_DWORD dwRetVal = eStateMachRes_Unknown;
EC_T_BOOL bWaitForDc = EC_FALSE;

    if (!IsPresentOnBus())
    {
        dwRetVal = eStateMachRes_Done;
        goto Exit;         
    }
    /* must be checked before SlaveStateMachIsStable() because it returns EC_TRUE */
    if (TargetStateNotReachable())
    {
        dwRetVal = eStateMachRes_Error;
        goto Exit;
    }
    if (SlaveStateMachIsStable())
    {
        dwRetVal = eStateMachRes_Done;
        goto Exit;         
    }
    if (m_oInitCmdRetryTimeout.IsElapsed())
    {
        m_oInitCmdRetryTimeout.Stop();

        if (AreInitCmdsPending())
        {
            StartInitCmds(m_wActTransition, EC_TRUE);
        }
    }
    if (!AreInitCmdsActive())
    {
        EC_T_WORD wTransition = 0;

        /* determine current state */
        if (DEVICE_STATE_UNKNOWN == m_wCurrStateMachDevState)
        {
            m_wCurrStateMachDevState = (EC_T_WORD)GetDeviceState();
            OsDbgAssert(DEVICE_STATE_UNKNOWN != m_wCurrStateMachDevState);
        }

        /* determine current transition */
        switch (m_wCurrStateMachDevState)
        {
        case DEVICE_STATE_INIT:         
            switch (m_wReqStateMachDevState)
            {
            case DEVICE_STATE_INIT:
                wTransition = ECAT_INITCMD_I_I;
                break;
            case DEVICE_STATE_PREOP:
            case DEVICE_STATE_SAFEOP:
            case DEVICE_STATE_OP:
                wTransition = ECAT_INITCMD_I_P;
                break;
            case DEVICE_STATE_BOOTSTRAP:            
                wTransition = ECAT_INITCMD_I_B;
            }
            break;
        case DEVICE_STATE_PREOP:
            switch (m_wReqStateMachDevState)
            {
            case DEVICE_STATE_PREOP:
                wTransition = ECAT_INITCMD_P_P;
                break;
            case DEVICE_STATE_INIT:
            case DEVICE_STATE_BOOTSTRAP:    
                wTransition = ECAT_INITCMD_P_I;
                break;
            case DEVICE_STATE_SAFEOP:
            case DEVICE_STATE_OP:
#if (defined INCLUDE_DC_SUPPORT)
                if ((EC_NULL != m_pBusSlave) && m_pBusSlave->IsDcEnabled())
                {
                    if (!m_pBusSlave->IsDcInitialized() || !m_pMaster->m_pDistributedClks->m_bInSyncNotifyActive)
                    {
                        /* wait for DC initialization, and DC state machine */
                        bWaitForDc = EC_TRUE;
                    }
                }
#endif /* INCLUDE_DC_SUPPORT */
                wTransition = ECAT_INITCMD_P_S;
                break;
            }
            break;
        case DEVICE_STATE_BOOTSTRAP:
            switch (m_wReqStateMachDevState)
            {
            case DEVICE_STATE_INIT:
            case DEVICE_STATE_PREOP:
            case DEVICE_STATE_SAFEOP:
            case DEVICE_STATE_OP:
                wTransition = ECAT_INITCMD_B_I;
                break;
            }
            break;
        case DEVICE_STATE_SAFEOP:
            switch (m_wReqStateMachDevState)
            {
            case DEVICE_STATE_INIT:
            case DEVICE_STATE_BOOTSTRAP: 
                wTransition = ECAT_INITCMD_S_I;
                break;
            case DEVICE_STATE_PREOP:
                wTransition = ECAT_INITCMD_S_P;
                break;
            case DEVICE_STATE_OP:
                wTransition = ECAT_INITCMD_S_O;
                break;
            }
            break;
        case DEVICE_STATE_OP:
            switch (m_wReqStateMachDevState)
            {
            case DEVICE_STATE_INIT:
            case DEVICE_STATE_BOOTSTRAP:  
                wTransition = ECAT_INITCMD_O_I;
                break;
            case DEVICE_STATE_PREOP:
                wTransition = ECAT_INITCMD_O_P;
                break;
            case DEVICE_STATE_SAFEOP:
                wTransition = ECAT_INITCMD_O_S;
                break;
            }
            break;
        default:
            OsDbgAssert(EC_FALSE);
        }       
        if (0 == wTransition)
        {
            StabilizeSlaveStateMachine();
        }
        else
        {
            if (!bWaitForDc)
            {
                /* reset AlStatusCode on state change */
                if (EC_NULL != m_pBusSlave)
                {
                    m_pBusSlave->SetAlStatusCode(DEVICE_STATUSCODE_NOERROR);
                }
                /* store current transition */
                m_wActTransition = wTransition;

                /* send first init command of current transition */
                StartInitCmds(wTransition, EC_FALSE);
            }
        }
    }
    if (SlaveStateMachIsStable())
    {
        dwRetVal = eStateMachRes_Done;
    }
    else
    {
        dwRetVal = eStateMachRes_Pending;
    }

Exit:
    OsDbgAssert(dwRetVal != eStateMachRes_Unknown);
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set Slave Presence.
*/
EC_T_VOID CEcSlave::SetSlavePresence(
    EC_T_BOOL   bPresent    /**< [in]   EC_TRUE: slave is present on bus, EC_FALSE: it is not */
                                    )
{
    if (bPresent)
    {
        if (ePRESENT != m_ePresenceOnBus)
        {
            m_ePresenceOnBus = ePRESENT;

            /* don't notify mandatory slave the first time */
            if (!m_bIsOptional && (eUNKNOWN == m_ePresenceNotified))
            {
                m_ePresenceNotified = m_ePresenceOnBus;
            }
            /* synchronize and stabilize slave state machine */
            StabilizeSlaveStateMachine();
        }
        m_wAutoIncAddr = (EC_NULL != m_pBusSlave)?m_pBusSlave->GetAutoIncAddress():INVALID_AUTO_INC_ADDR;

#if (defined INCLUDE_MASTER_OBD)
        if (EC_NULL != m_pMaster->m_pSDOService)
        {
            m_pMaster->m_pSDOService->UpdateSlaveState((EC_T_WORD)(0-m_wAutoIncAddr), GetDeviceState());
        }
#endif
    }
    else
	{
        if (eABSENT != m_ePresenceOnBus)
        {
            m_ePresenceOnBus = eABSENT;

            /* don't notify mandatory slave the first time */
            if (!m_bIsOptional && (eUNKNOWN == m_ePresenceNotified) && m_pMaster->GetAllSlavesMustReachState())
            {
                m_ePresenceNotified = m_ePresenceOnBus;
            }
            /* synchronize and stabilize slave state machine */
            StabilizeSlaveStateMachine();
        }
        m_wAutoIncAddr = INVALID_AUTO_INC_ADDR;

#if (defined INCLUDE_MASTER_OBD)
        if (EC_NULL != m_pMaster->m_pSDOService)
        {
            m_pMaster->m_pSDOService->UpdateSlaveState((EC_T_WORD)(0-m_wAutoIncAddr), eEcatState_UNKNOWN);
        }
#endif
    }
    /* set valid flag */
    m_bIsPresenceValid = EC_TRUE;

    m_pMaster->RecalculateCycCmdWkc(this);

    return;
}

/***************************************************************************************************/
/**
\brief  Notify cfg slave presence.
*/
EC_T_VOID CEcSlave::NotifyPresence(EC_T_VOID)
{
    /* presence must be valid */
    if (!m_bIsPresenceValid)
    {
        return;
    }
    /* 1. notify on first bus scan all present optional slaves
       2. notify on first bus scan all absent mandatory slaves if not all slaves must reach master state on bus scan
       3. notify on following bus scan all changes*/
    if (  ((eUNKNOWN == m_ePresenceNotified) && (m_ePresenceOnBus == ePRESENT) &&  IsOptional())
       || ((eUNKNOWN == m_ePresenceNotified) && (m_ePresenceOnBus == eABSENT)  && !IsOptional() && !m_pMaster->GetAllSlavesMustReachState())
       || ((eUNKNOWN != m_ePresenceNotified) && (m_ePresenceOnBus != m_ePresenceNotified)))
    {
        m_pMaster->NotifySlavePresence(m_wFixedAddr, ePRESENT == m_ePresenceOnBus);
    }
    m_ePresenceNotified = m_ePresenceOnBus;

    /* check for unexpected state */
    if (ePRESENT == m_ePresenceOnBus)
    {
#if (defined INCLUDE_HOTCONNECT)
    EC_T_HOTCONNECT_GROUP* pHcGroup = GetHCGroup();

        if ((EC_NULL == pHcGroup) || (pHcGroup->bGroupTargStateSet))
#endif
        {
            EC_T_STATE expState = GetSmReqEcatState();
            if (eEcatState_UNKNOWN == expState)
            {
                expState = (DEVICE_STATE_UNKNOWN == m_wAlStatusNotified)?eEcatState_UNKNOWN:(EC_T_STATE)m_wAlStatusNotified;
            }
            m_pMaster->NotifySlaveUnexpectedState(this, expState);
        }
    }
}

/********************************************************************************/
/** \brief Determine slave target state.
*
* \return
*/
EC_T_WORD   GetTargetState(EC_T_WORD wTransition)
{
    switch (wTransition)
    {
    case ECAT_INITCMD_I_I:
    case ECAT_INITCMD_P_I:
    case ECAT_INITCMD_B_I:
    case ECAT_INITCMD_S_I:
    case ECAT_INITCMD_O_I:      return DEVICE_STATE_INIT;
    case ECAT_INITCMD_I_P:
    case ECAT_INITCMD_S_P:
    case ECAT_INITCMD_O_P:
    case ECAT_INITCMD_P_P:      return DEVICE_STATE_PREOP;
    case ECAT_INITCMD_P_S:
    case ECAT_INITCMD_O_S:      return DEVICE_STATE_SAFEOP;
    case ECAT_INITCMD_S_O:      return DEVICE_STATE_OP;
    case ECAT_INITCMD_I_B:      return DEVICE_STATE_BOOTSTRAP;
    }
    OsDbgAssert(EC_FALSE);
    return DEVICE_STATE_UNKNOWN;
}

/********************************************************************************/
/** \brief Add CoE init commands dynamically
*
* \return
*/
EC_T_DWORD CEcSlave::AddCoeInitCmds(EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmds, EC_T_WORD wCount)
{
    EC_UNREFPARM(pbCoeInitCmds);
    EC_UNREFPARM(wCount);

    return EC_E_NOTSUPPORTED;
}

/********************************************************************************/
/** \brief Converts Master InitCmd to Slave InitCmd
 * - MasterInitCmd must be issued with FPRD/FPWR
 *
 * \return EC_TRUE if InitCmd was modified
 */
EC_T_BOOL CEcSlave::ConvertMasterInitCmdToSlaveInitCmd(EcInitCmdDesc* pInitCmdDesc)
{
    EC_T_BOOL bModified = EC_FALSE;

    /* change InitCmd type for fixed addressing */
    switch (pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
    {
    case EC_CMD_TYPE_NOP: break;
    case EC_CMD_TYPE_APRD: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_APWR: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_APRW: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRW; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_FPRD: break;
    case EC_CMD_TYPE_FPWR: break;
    case EC_CMD_TYPE_FPRW: break;
    case EC_CMD_TYPE_BRD: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_BWR: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_BRW: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRW; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_LRD: break;
    case EC_CMD_TYPE_LWR: break;
    case EC_CMD_TYPE_LRW: break;
    case EC_CMD_TYPE_ARMW: pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FRMW; bModified = EC_TRUE; break;
    case EC_CMD_TYPE_FRMW: break;
    default: OsDbgAssert(EC_FALSE); break;
    }

    /* set slave address to InitCmd */
    if (bModified)
    {
        EC_ICMDHDR_SET_ADDR_ADP(&pInitCmdDesc->EcCmdHeader, GetFixedAddr());
    }

    return bModified;
}

/********************************************************************************/
/** \brief Processes init commands that should be sent in this transition.
*
* \return EC_TRUE: All InitCmds done
*/
EC_T_BOOL  CEcSlave::StartInitCmds(EC_T_WORD wTransition, EC_T_BOOL bRetry)
{
#if (defined INCLUDE_LOG_MESSAGES) && (defined INCLUDE_DC_SUPPORT)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_DWORD     dwRes         = EC_E_ERROR;
EC_T_BOOL      bStopInitCmds = EC_FALSE;
EcInitCmdDesc* pInitCmdDesc  = EC_NULL;
EcInitCmdDesc  MasterInitCmdDesc;
EC_T_BOOL      bRestoreMasterInitCmdDesc = EC_FALSE;
#if (defined INCLUDE_DC_SUPPORT)
EC_T_UINT64    qwDcStartTimeData = 0;
EC_T_BOOL      bRestoreDcStartTimeData = EC_FALSE;
#endif
    OsMemset(&MasterInitCmdDesc, 0, sizeof(EcInitCmdDesc));

    /* cancel state machine? */
    if (m_bCancelStateMachine || !IsPresentOnBus())
    {
        /* reset trigger */
        m_bCancelStateMachine = EC_FALSE;

        /* stop init commands */
        StopInitCmds(wTransition, wTransition);
        bStopInitCmds = EC_TRUE;
        goto Exit;
    }
    /* this method is called once by CEcSlave::StateMachine to send the first */
    /* command defined for this transition and is called by CEcSlave:ProcessCmdResult */
    /* for the following commands */
    EC_TRACEMSG(EC_TRACE_INITCMDS, ("-----\n"));
    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcSlave::StartInitCmds() %s: wTransition = 0x%x\n", m_pEniSlave->szName, (EC_T_DWORD)wTransition));

    /* get next init cmd */
    if (!bRetry)
    {
        /* look for next InitCmd of current transition */
        if (INITCMD_INACTIVE == m_wMasterInitCmdsCurrent)
        {
            /* we start state change, deactivate now the AL status interrupt to not disturb the init commands */
            ALStatusInterruptEnable(EC_FALSE);

#ifdef INCLUDE_LOG_MESSAGES
            if (m_pMaster->m_MasterConfig.dwStateChangeDebug & 3)
            {
                if (m_bEnableLinkMessages)
                {
                    m_poEcDevice->LinkStateChngMsg(m_pMaster->m_MasterConfig.dwStateChangeDebug, "Transition '%s': %s\n", GetStateChangeNameShort((EC_T_WORD)(wTransition & 0x7FF)), m_pEniSlave->szName);
                }
            }
#endif
        }
        /* start first / get next Master/Slave InitCmd */
        if (m_pMaster->MasterStateMachIsStable())
        {
            for (;;)
            {
            EC_T_BOOL bSendInitCmd = EC_TRUE;

                pInitCmdDesc = FindNextInitCmd(m_pMaster->m_ppInitCmds, &m_wMasterInitCmdsCurrent, m_pMaster->m_nInitCmdsCount, wTransition);
                if (EC_NULL == pInitCmdDesc)
                {
                    break;
                }
                /* skip some DC related InitCmd modifying register set in the DC state machine if master state is PREOP or above */
                if (DEVICE_STATE_PREOP <= m_pMaster->GetMasterDeviceState())
                {
                EC_T_WORD wAdo = EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader));

                    if ((ECM_DCS_SYSTEMTIME == wAdo) || (ECM_DCS_SPEEDCOUNT_START == wAdo) || (ECM_DCS_SYSTIMEDIFF_FILTERDEPTH == wAdo))
                    {
                        bSendInitCmd = EC_FALSE;
                    }
                }
                if (bSendInitCmd)
                {
                    break;
                }
            }
        }
        if (EC_NULL == pInitCmdDesc)
        {
            pInitCmdDesc = FindNextInitCmd(m_ppInitCmds, &m_wInitCmdsCurrent, m_wInitCmdsCount, wTransition);
        }
    }
    else
    {
        pInitCmdDesc = GetInitCmdsCurrent();
        OsDbgAssert(EC_NULL != pInitCmdDesc);
    }

    /* terminate on last init command */
    if (EC_NULL == pInitCmdDesc)
    {
        /* we reached the final state */
        EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcSlave::StartInitCmds(): %s - all Standard InitCmds for state %s processed\n", m_pEniSlave->szName, SlaveDevStateText(m_wReqStateMachDevState)));
        if (!m_bStandardInitCmdsFirst)
        {
            StopInitCmds(wTransition, wTransition);
        }
        bStopInitCmds = EC_TRUE;
        goto Exit;
    }

    /* save and convert MasterInitCmdDesc */
    if (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc))
    {
        OsMemcpy(&MasterInitCmdDesc, pInitCmdDesc, sizeof(EcInitCmdDesc));
        bRestoreMasterInitCmdDesc = ConvertMasterInitCmdToSlaveInitCmd(pInitCmdDesc);
    }

    /* send InitCmd */
    {
        EC_TRACEMSG(
            EC_TRACE_INITCMDS, 
            ("<----[%06d] CEcSlave::StartInitCmds(%s, cmdIdx=0x%x) send %s InitCmd[%d-%d] %s: %s,adp=%d,ado=0x%x\n", 
             OsQueryMsecCount(), 
             (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? "Master" : "Slave"),
             (EC_T_DWORD)EC_ICMDHDR_GET_CMDIDX(&(pInitCmdDesc->EcCmdHeader)), 
             m_pEniSlave->szName, (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), 
             (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? m_wMasterInitCmdsCurrent : m_wInitCmdsCurrent),
             GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)),
             EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd), 
             (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)), 
             (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader))
            )      );

#if (defined INCLUDE_DC_SUPPORT)
        if ((EC_CMD_TYPE_FPWR == pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) && (ECM_DCS_DC_WR_STARTCYCOP == EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader))))
        {
            if (m_pMaster->m_pDistributedClks->m_bDCInitialized)
            {
            EC_T_UINT64 qwBusTime     = m_pMaster->m_pDistributedClks->GetBusTime();
            EC_T_UINT64 qwDcStartTime = 0;

                /* get next DC start time, if in the past recalculate one */
                qwDcStartTime = m_pMaster->m_pDistributedClks->GetDcStartTimeNext();
                if (qwDcStartTime < (qwBusTime + 10 * (m_pMaster->GetBusCycleTimeUsec() * 1000)))
                {
                    m_pMaster->m_pDistributedClks->CalculateDcStartTime();
                    qwDcStartTime = m_pMaster->m_pDistributedClks->GetDcStartTimeNext();
                }
                /* adjust DC start time with SYNC shift time */
                qwDcStartTimeData = EC_GET_FRM_QWORD(EcInitCmdDescData(pInitCmdDesc));
                qwDcStartTime += qwDcStartTimeData;
                if (!m_pBusSlave->IsDC64Support())
                {
                    qwDcStartTime = EC_MAKEQWORD(0, EC_LODWORD(qwDcStartTime));
                }
                bRestoreDcStartTimeData = EC_TRUE;
                EC_SET_FRM_QWORD(EcInitCmdDescData(pInitCmdDesc), qwDcStartTime);
                {
                EC_T_UINT64 qwTmp;

                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "CEcSlave::StartInitCmds()\n"));
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "InitCmd Slave 0x%04X\n", m_wFixedAddr));
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "BusTime        = 0x%08X%08X\n", EC_HIDWORD(qwBusTime),     EC_LODWORD(qwBusTime)));
                    OsSystemTimeGet(&qwTmp);
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "HostTime       = 0x%08X%08X\n", EC_HIDWORD(qwTmp),         EC_LODWORD(qwTmp)));
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), " Diff          = %18d\n", (qwTmp - qwBusTime)));
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), "DcStartTime    = 0x%08X%08X\n", EC_HIDWORD(qwDcStartTime), EC_LODWORD(qwDcStartTime)));
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+DCAPROVAL), " Diff          = %18d\n", (qwDcStartTime - qwBusTime)));
                }
            }
        }
#endif /* INCLUDE_DC_SUPPORT */

        /* initialize retry counter */
        if (!bRetry)
        {
            m_wInitCmdsRetries = EC_ECINITCMDDESC_GET_RETRIES(pInitCmdDesc);
        }

        /* send init command to slave */
        dwRes = QueueEcatInitCmd(
                    wTransition, 
                    pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd, 
                    EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)), 
                    EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)), 
                    EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)), 
                    EcInitCmdDescData(pInitCmdDesc),
                    (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF) 
                    );
        if (EC_E_NOERROR != dwRes)
        {
            if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
            {
                /* start retry timeout to retry later. Don't decrement retry counter, because InitCmd was not sent */
                m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
            }
        }
        else
        {
            m_pMaster->LogInitCmd(pInitCmdDesc, wTransition);
            /* start timeout */
            {
                EC_T_WORD wInitCmdTimeout = 0;

                wInitCmdTimeout = EC_ECINITCMDDESC_GET_INITCMDTIMEOUT(pInitCmdDesc);
                if (0 == wInitCmdTimeout)
                {
                    wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                }
                m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
            }
            m_oInitCmdRetryTimeout.Stop();
        }
    }

Exit:
    if (bRestoreMasterInitCmdDesc)
    {
        /* save MasterInitCmdDesc */
        OsMemcpy(pInitCmdDesc, &MasterInitCmdDesc, sizeof(EcInitCmdDesc));
    }
#if (defined INCLUDE_DC_SUPPORT)
    if (bRestoreDcStartTimeData)
    {
        if (EC_NULL != pInitCmdDesc)
        {
            EC_SET_FRM_QWORD(EcInitCmdDescData(pInitCmdDesc), qwDcStartTimeData);
        }
        else
        {
            OsDbgAssert(EC_FALSE);
        }
        bRestoreDcStartTimeData = EC_FALSE;
    }
#endif
    return bStopInitCmds;
}

/********************************************************************************/
/** \brief Stops the processing of init commands
*
* \return
*/
EC_T_VOID   CEcSlave::StopInitCmds
(EC_T_WORD wTransition      /* [in]  transition or failure info */
,EC_T_WORD wCurrTransition  /* [in]  current transition in case of failure */
)
{
    EC_TRACEMSG(EC_TRACE_INITCMDS, ("++CEcSlave::StopInitCmds() %s: wCurrStateMachDevState = %s\n", m_pEniSlave->szName, SlaveDevStateText(m_wCurrStateMachDevState)));

    if (wTransition == ECAT_INITCMD_FAILURE)
    {
        m_bInitCmdFailedError = EC_TRUE;

        /* stabilize state machine */
        m_wReqStateMachDevState = m_wCurrStateMachDevState;

        /* generate notification */
        if (CEcSlave::AreInitCmdsPending())
        {
            EcInitCmdDesc* pInitCmdDesc = CEcSlave::GetInitCmdsCurrent();
            m_pMaster->NotifySlaveInitCmdResponseError(this, wCurrTransition, eInitCmdErr_FAILED, EcInitCmdDescComment(pInitCmdDesc), EC_ECINITCMDDESC_GET_CMTLEN(pInitCmdDesc));
        }
#ifdef INCLUDE_LOG_MESSAGES
        if ((m_pMaster->m_MasterConfig.dwStateChangeDebug&3) && m_bEnableLinkMessages)
        {
            m_poEcDevice->LinkStateChngMsg( m_pMaster->m_MasterConfig.dwStateChangeDebug, 
                "InitCmd failure '%s', transition %s: requested state = %s\n", 
                                             m_pEniSlave->szName, 
                                             GetStateChangeName(wCurrTransition),
                                             SlaveDevStateText(m_wReqStateMachDevState)
                                           );
        }
#endif
    }
    else
    {
        SetSmDeviceState(GetTargetState(wTransition));

#ifdef INCLUDE_LOG_MESSAGES
        if ((m_pMaster->m_MasterConfig.dwStateChangeDebug&3) && m_bEnableLinkMessages)
        {
            m_poEcDevice->LinkStateChngMsg( 
                m_pMaster->m_MasterConfig.dwStateChangeDebug, "New CurrStateMachDevState '%s' = %s\n", 
                m_pEniSlave->szName, SlaveDevStateText(m_wCurrStateMachDevState) 
                                           );
        }
#endif
    }
    ALStatusInterruptEnable(EC_TRUE);

    m_wInitCmdsCurrent = INITCMD_INACTIVE;
    m_wMasterInitCmdsCurrent = INITCMD_INACTIVE;
    if ((m_wCurrStateMachDevState == m_wReqStateMachDevState) || TargetStateNotReachable())
    {
        m_bStateMachineStable = EC_TRUE;

        if (m_bCycErrorNotifyMasked)
        {
            m_bCycErrorNotifyMasked = EC_FALSE;
            m_pMaster->DecCycErrorNotifyMaskedCnt();
        }
    }
    m_oInitCmdsTimeout.Stop();
    m_oInitCmdRetryTimeout.Stop();

    if (m_bCancelStateMachine)
    {
        m_bCancelStateMachine = EC_FALSE;
    }

    EC_TRACEMSG(EC_TRACE_INITCMDS, ("--CEcSlave::StopInitCmds() %s: wCurrStateMachDevState = %s\n", m_pEniSlave->szName, SlaveDevStateText(m_wCurrStateMachDevState)));
}

EC_T_VOID CEcSlave::SetSmDeviceState(EC_T_WORD wCurrStateMachDevState)
{
    m_wCurrStateMachDevState = wCurrStateMachDevState;

    if (ePRESENT == m_ePresenceNotified)
    {
        m_pMaster->NotifySlaveStateChanged(this);
    }
#if (defined INCLUDE_HOTCONNECT)
    if (IsHCGroupHead())
    {
        m_pMaster->m_oHotConnect.RefreshGroupActive(GetHCGroup());
        m_pMaster->m_oHotConnect.RecalculateWKC(EC_TRUE);
    }
#endif /* INCLUDE_HOTCONNECT */
}

/********************************************************************************/
/** \brief Return state change name string.
*
* \return
*/
EC_T_CHAR*   GetStateChangeName(EC_T_WORD wTransition)
{
#ifdef INCLUDE_LOG_MESSAGES
    if (ECAT_INITCMD_BEFORE & wTransition)
    {
        return (EC_T_CHAR*)"BEFORE";
    }

    switch (wTransition)
    {
    case ECAT_INITCMD_I_P:  return (EC_T_CHAR*)"INIT to PREOP";
    case ECAT_INITCMD_P_S:  return (EC_T_CHAR*)"PREOP to SAFEOP";
    case ECAT_INITCMD_P_I:  return (EC_T_CHAR*)"PREOP to INIT";
    case ECAT_INITCMD_S_P:  return (EC_T_CHAR*)"SAFEOP to PREOP";
    case ECAT_INITCMD_S_O:  return (EC_T_CHAR*)"SAFEOP to OP";
    case ECAT_INITCMD_S_I:  return (EC_T_CHAR*)"SAFEOP to INIT";
    case ECAT_INITCMD_O_S:  return (EC_T_CHAR*)"OP to SAFEOP";
    case ECAT_INITCMD_O_P:  return (EC_T_CHAR*)"OP to PREOP";
    case ECAT_INITCMD_O_I:  return (EC_T_CHAR*)"OP to INIT";
    case ECAT_INITCMD_B_I:  return (EC_T_CHAR*)"BOOT to INIT";  
    case ECAT_INITCMD_I_B:  return (EC_T_CHAR*)"INIT to BOOT";
    case ECAT_INITCMD_SI_OI:            return (EC_T_CHAR*)"SO to INIT";
    case ECAT_INITCMD_OSP_I:            return (EC_T_CHAR*)"OSP to INIT";
    case ECAT_INITCMD_OSP_I__I_BP:      return (EC_T_CHAR*)"OSP to INIT, INIT to BP";
    case ECAT_INITCMD_OSP_I__I_P:       return (EC_T_CHAR*)"OSP to INIT, INIT to PREOP";
    case ECAT_INITCMD_SP_SI_OP_OI:      return (EC_T_CHAR*)"SP to SI, OP to OI";
    case ECAT_INITCMD_IP_SP_SI_OP_OI:   return (EC_T_CHAR*)"ISO to PREOP, SO to INIT";
    case ECAT_INITCMD_I_PB:             return (EC_T_CHAR*)"INIT to PB";
    case ECAT_INITCMD_BACKTO_I:         return (EC_T_CHAR*)"BACKTO INIT";
    case ECAT_INITCMD_BACKTO_P:         return (EC_T_CHAR*)"BACKTO PREOP";
    case ECAT_INITCMD_ADR_ERROR:        return (EC_T_CHAR*)"ADDRESS ERROR";
    case ECAT_INITCMD_FAILURE:          return (EC_T_CHAR*)"INITCMD FAILURE";
    case ECAT_INITCMD_BI_IP:            return (EC_T_CHAR*)"BI+IP";
    case ECAT_INITCMD_IP_PS:            return (EC_T_CHAR*)"IP+PS";
    }
    OsDbgAssert(EC_FALSE);
    return (EC_T_CHAR*)"invalid transition";
#else
    EC_UNREFPARM(wTransition);
    return (EC_T_CHAR*)"TRANS?";
#endif
}

/********************************************************************************/
/** \brief Return state change short string.
*
* \return
*/
EC_T_CHAR*   GetStateChangeNameShort(EC_T_WORD wTransition)
{
#ifdef INCLUDE_LOG_MESSAGES
    if (ECAT_INITCMD_BEFORE & wTransition)
    {
        return (EC_T_CHAR*)"BEFORE";
    }

    switch (wTransition)
    {
    case ECAT_INITCMD_I_I:  return (EC_T_CHAR*)"II";
    case ECAT_INITCMD_I_P:  return (EC_T_CHAR*)"IP";
    case ECAT_INITCMD_P_P:  return (EC_T_CHAR*)"PP";
    case ECAT_INITCMD_P_S:  return (EC_T_CHAR*)"PS";
    case ECAT_INITCMD_P_I:  return (EC_T_CHAR*)"PI";
    case ECAT_INITCMD_S_P:  return (EC_T_CHAR*)"SP";
    case ECAT_INITCMD_S_O:  return (EC_T_CHAR*)"SO";
    case ECAT_INITCMD_S_I:  return (EC_T_CHAR*)"SI";
    case ECAT_INITCMD_O_S:  return (EC_T_CHAR*)"OS";
    case ECAT_INITCMD_O_P:  return (EC_T_CHAR*)"OP";
    case ECAT_INITCMD_O_I:  return (EC_T_CHAR*)"OI";
    case ECAT_INITCMD_B_I:  return (EC_T_CHAR*)"BI";
    case ECAT_INITCMD_I_B:  return (EC_T_CHAR*)"IB";
    case ECAT_INITCMD_SI_OI:            return (EC_T_CHAR*)"SO_I";
    case ECAT_INITCMD_OSP_I:            return (EC_T_CHAR*)"OSP_I";
    case ECAT_INITCMD_OSP_I__I_P:       return (EC_T_CHAR*)"OSP_I+I_P";
    case ECAT_INITCMD_SP_SI_OP_OI:      return (EC_T_CHAR*)"SP+SI+OP+OI";
    case ECAT_INITCMD_IP_SP_SI_OP_OI:   return (EC_T_CHAR*)"IP+SP+SI+OP+OI";        
    case ECAT_INITCMD_I_PB:             return (EC_T_CHAR*)"I_PB";
    case ECAT_INITCMD_BACKTO_I:         return (EC_T_CHAR*)"BACKTO_I";
    case ECAT_INITCMD_BACKTO_P:         return (EC_T_CHAR*)"BACKTO_P";
    case ECAT_INITCMD_ADR_ERROR:        return (EC_T_CHAR*)"ADR_ERR";
    case ECAT_INITCMD_FAILURE:          return (EC_T_CHAR*)"FAIL";
    case ECAT_INITCMD_BI_IP:            return (EC_T_CHAR*)"BI+IP";
    case ECAT_INITCMD_IP_PS:            return (EC_T_CHAR*)"IP+PS";
    case ECAT_INITCMD_OSP_I__I_BP:      return (EC_T_CHAR*)"OSP_I+I_BP";
    case ECAT_INITCMD_IP_PI_BI_SI_OI:   return (EC_T_CHAR*)"IP+PI+BI+SI+OI";
    }
    OsDbgAssert(EC_FALSE);
    return (EC_T_CHAR*)"TRANS?";
#else
    EC_UNREFPARM(wTransition);
    return (EC_T_CHAR*)"TRANS?";
#endif
}



/********************************************************************************/
/** \brief Queue an init command.
*
* \return
*/
EC_T_DWORD CEcSlave::QueueEcatInitCmd
(   EC_T_DWORD dwInvokeId, 
    EC_T_BYTE byCmd, 
    EC_T_WORD wAdp, 
    EC_T_WORD wAdo, 
    EC_T_WORD wLen, 
    EC_T_BYTE* abyData,
    EC_T_WORD wCurrTransition)
{
    EC_T_DWORD dwRetval = EC_E_ERROR;

    if (!IsPresentOnBus() || (EC_NULL == m_pBusSlave))
    {
        StopInitCmds(ECAT_INITCMD_FAILURE, wCurrTransition);
        return EC_E_SLAVE_NOT_PRESENT;
    }
    /* change addressing mode if necessary */
    if (byCmd == EC_CMD_TYPE_APRD)
    {
        byCmd = EC_CMD_TYPE_FPRD;
        wAdp  = m_pBusSlave->GetFixedAddress();
    }
    else if (byCmd == EC_CMD_TYPE_APWR)
    {
        byCmd = EC_CMD_TYPE_FPWR;
        wAdp  = m_pBusSlave->GetFixedAddress();
    }
    else if (byCmd == EC_CMD_TYPE_APRW)
    {
        byCmd = EC_CMD_TYPE_FPRW;
        wAdp  = m_pBusSlave->GetFixedAddress();
    }
    else if (byCmd == EC_CMD_TYPE_ARMW )
    {
        byCmd = EC_CMD_TYPE_FRMW;
        wAdp  = m_pBusSlave->GetFixedAddress();
    }
    /* See "10.1.2 Device Emulation" in ET1100 datasheet: 
     * "The master shoud not set the Error Indication Acknowledge bit for such slaves at all"
     * Added for compatibility: errornous ENIs where tolerated in V2.4, too. */
    if ((ECREG_AL_CONTROL == wAdo) && m_pBusSlave->IsDeviceEmulation())
    {
        if (1 == EC_GET_WORD_IN_BITFIELD(4, 1, abyData[0]))
        {
            EC_ERRORMSGC(("WARNING: InitCmd for Slave with Adp %d with Device Emulation contains Error Indication Acknowledge bit, removing!", wAdp));
            abyData[0] = (EC_T_BYTE)EC_RESET_WORD_IN_BITFIELD(abyData[0], 4, 1);
        }
    }

    dwRetval = m_pMaster->QueueRegisterCmdReq(this, dwInvokeId, byCmd, wAdp, wAdo, wLen, abyData);
    return dwRetval;
}

/********************************************************************************/
/** \brief Activate the AL Status Interrupt in 0x0200.
 *
 * \return EC_E_NOERROR, EC_E_ACYC_FRM_FREEQ_EMPTY
 */
EC_T_DWORD CEcSlave::ALStatusInterruptEnable(EC_T_BOOL bEnable)
{
EC_T_DWORD dwRetval = EC_E_ERROR;
EC_T_DWORD dwRes = EC_E_ERROR;
EC_T_WORD  wEcatEventMask = 0;

    if (EC_NULL == m_pBusSlave)
    {
        dwRetval = EC_E_NOTFOUND;
        goto Exit;
    }

    /* check current ECAT event mask */
    wEcatEventMask = m_pBusSlave->GetEcatEventMask();

    /* enable? */
    if (bEnable)
    {
        wEcatEventMask |= ECM_ECATEVENT_ALSTATUS;
    }
    else
    {
        /* disable AL status interrupt to monitor unexpected changes */
        wEcatEventMask &= ~ECM_ECATEVENT_ALSTATUS;
    }

    /* changed? */
    if (wEcatEventMask != m_pBusSlave->GetEcatEventMask())
    {
    EC_T_WORD wEcatEventMaskFrm  = 0;

        EC_SET_FRM_WORD(&wEcatEventMaskFrm, wEcatEventMask);

        /* queue EtherCAT command */
        dwRes = m_pMaster->QueueRegisterCmdReq(
            this, INVOKE_ID_WRITEEVENTMASK, EC_CMD_TYPE_FPWR, m_wFixedAddr,
            ECREG_SLV_ECATEVENTMASK, sizeof(EC_T_WORD), &wEcatEventMaskFrm
            );
        if (EC_E_NOERROR != dwRes)
        {
            dwRetval = dwRes;
            goto Exit;
        }
    }

    dwRetval = EC_E_NOERROR;
Exit:
    return dwRetval;
}

/********************************************************************************/
/** \brief Processes the response of an EtherCAT command.
*
* \return
*/
EC_T_VOID CEcSlave::ProcessCmdResult(EC_T_DWORD result, EC_T_DWORD dwInvokeId, PETYPE_EC_CMD_HEADER pEcCmdHeader)
{
EC_T_DWORD                  dwRes               = EC_E_ERROR;
EC_T_NOTIFICATION_INTDESC*  pNotification       = EC_NULL;
EC_T_WORD                   wEcCmdLen           = 0;
EC_T_PBYTE                  pbyData             = EC_NULL;
EC_T_WORD                   wWkc                = 0;
EcInitCmdDesc*              pInitCmdDesc        = EC_NULL;

    if (EC_NULL != pEcCmdHeader)
    {
        wEcCmdLen = EC_AL_CMDHDRLEN_GET_LEN(&(pEcCmdHeader->uLen));
        pbyData   = (EC_T_PBYTE)(EC_ENDOF(pEcCmdHeader));
        wWkc      = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
    }

    switch (dwInvokeId)
    {
    case ECAT_INITCMD_I_I:
    case ECAT_INITCMD_I_P:
    case ECAT_INITCMD_P_P:
    case ECAT_INITCMD_P_S:
    case ECAT_INITCMD_P_I:
    case ECAT_INITCMD_S_P:
    case ECAT_INITCMD_S_O:
    case ECAT_INITCMD_S_I:
    case ECAT_INITCMD_O_S:
    case ECAT_INITCMD_O_P:
    case ECAT_INITCMD_O_I:
    case ECAT_INITCMD_B_I:
    case ECAT_INITCMD_I_B:  
        {
            /* don't process unexpected invoke ids */
            if (CEcSlave::AreAllInitCmdsProcessed())
            {
                EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() %s: unprocessed dwInvokeId (0x%x)\n", m_pEniSlave->szName, (EC_T_INT)dwInvokeId));
                break;
            }

            EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcSlave::ProcessCmdResult() %s: Process dwInvokeId (0x%x)\n", m_pEniSlave->szName, (EC_T_INT)dwInvokeId));
            pInitCmdDesc = CEcSlave::GetInitCmdsCurrent();
            
            /* error result -> retry */
            if (EC_E_NOERROR != result)
            {   
                if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                {
                    if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                    {
                        m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        m_wInitCmdsRetries--;
                    }

                    EC_ERRORMSGC_L(
                        ("CEcSlave::ProcessCmdResult() Error result retry send %s InitCmd[%d-%d]: %s: %s,adp=%d,ado=0x%x\n",
                            (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? "Master" : "Slave"),
                            (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr),
                            (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? m_wMasterInitCmdsCurrent : m_wInitCmdsCurrent),
                            GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc) & 0x7FF)),
                            EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                            (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                            (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)))
                        );
                }
                else
                {
                    m_pMaster->NotifySlaveInitCmdResponseError(this, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc) & 0x7FF), eInitCmdErr_NO_RESPONSE,
                        EcInitCmdDescComment(pInitCmdDesc), EC_ECINITCMDDESC_GET_CMTLEN(pInitCmdDesc));

                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nCommunication Error '0x%x'.\n", m_pEniSlave->szName, m_wFixedAddr,
                        GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc) & 0x7FF)), EcInitCmdDescComment(pInitCmdDesc), result));

                    /* don't continue sending init commands */
                    StopInitCmds(ECAT_INITCMD_FAILURE, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc) & 0x7FF));
                }
                break;
            }

            EC_TRACEMSG(EC_TRACE_INITCMDS, ("---->[%06d] CEcSlave::ProcessCmdResult(cmdIdx=0x%x) %s: Test dwInvokeId = %s (0x%x)\n", 
                        OsQueryMsecCount(), (EC_T_DWORD)pEcCmdHeader->uCmdIdx.__wCmdIdx, m_pEniSlave->szName, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), (int)dwInvokeId));
            
            if (EC_ECINITCMDDESC_GET_CNT(pInitCmdDesc) == 0xffff || ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == EC_ECINITCMDDESC_GET_CNT(pInitCmdDesc))
            {   /* valid working counter */
                    
                EC_T_BOOL bNext = EC_TRUE;
                if (EC_ECINITCMDDESC_GET_VALIDATE(pInitCmdDesc) && EC_ICMDHDR_GET_LEN_LEN(pEcCmdHeader) == EC_ICMDHDR_GET_LEN_LEN((&pInitCmdDesc->EcCmdHeader)))
                {   /* initcmd with validation */
                        
                    EC_T_BYTE* pDataFrame = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);
                    EC_T_BYTE* pData      = EcInitCmdDescData(pInitCmdDesc);

                    OsMemcpy(pData, pDataFrame, wEcCmdLen);

                    if (EC_ECINITCMDDESC_GET_VALIDATEMASK(pInitCmdDesc))
                    {
                        EC_T_BYTE* pMask = (EC_T_BYTE*)EcInitCmdDescVMData(pInitCmdDesc);

                        for ( EC_T_INT i=0; i < wEcCmdLen; i++ )
                        {
                            pData[i] &= pMask[i];
                        }
                    }

                    if (OsMemcmp(pData, EcInitCmdDescVData(pInitCmdDesc), wEcCmdLen) != 0 )
                    {   /* validate failed -> retry */
                        if (m_oInitCmdsTimeout.IsElapsed() || m_bCancelStateMachine )
                        {
                            m_pMaster->NotifySlaveInitCmdResponseError(this, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF), 
                                eInitCmdErr_VALIDATION_ERR, EcInitCmdDescComment(pInitCmdDesc),
                                EC_ECINITCMDDESC_GET_CMTLEN(pInitCmdDesc));
                            if (wEcCmdLen == 2*sizeof(EC_T_DWORD))
                            {
                                if (EC_ECINITCMDDESC_GET_VALIDATEMASK(pInitCmdDesc))
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%08x %08x' read and '0x%08x %08x' with mask '0x%08x %08x' expected.\n", 
                                        m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        pDataFrame[1], pDataFrame[0], 
                                        EC_GETDWORD( (((EC_T_DWORD*)EcInitCmdDescVData(pInitCmdDesc))+1)), EC_GETDWORD( (((EC_T_DWORD*)EcInitCmdDescVData(pInitCmdDesc))+0)),
                                        EC_GETDWORD( (((EC_T_DWORD*)EcInitCmdDescVMData(pInitCmdDesc))+1)), EC_GETDWORD( (((EC_T_DWORD*)EcInitCmdDescVMData(pInitCmdDesc))+0))
                                                    ));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%08x %08x' read and '0x%08x %08x' expected.\n", 
                                        m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        EC_GETDWORD(pData+1), EC_GETDWORD(pData+0), 
                                        EC_GETDWORD( (((EC_T_DWORD*)EcInitCmdDescVData(pInitCmdDesc))+1)), EC_GETDWORD( (((EC_T_DWORD*)EcInitCmdDescVData(pInitCmdDesc))+0)) ));
                                }
                            }
                            else if (wEcCmdLen == sizeof(EC_T_DWORD))
                            {
                                if (EC_ECINITCMDDESC_GET_VALIDATEMASK(pInitCmdDesc))
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%08x' read and '0x%08x' with mask '0x%08x' expected.\n", 
                                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        pDataFrame[0], EC_GETDWORD(EcInitCmdDescVData(pInitCmdDesc)), 
                                        EC_GETDWORD(EcInitCmdDescVMData(pInitCmdDesc))));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%08x' read and '0x%08x' expected.\n", 
                                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        EC_GETDWORD(pData), EC_GETDWORD(EcInitCmdDescVData(pInitCmdDesc))));
                                }
                            }
                            else if (wEcCmdLen == sizeof(EC_T_WORD))
                            {
                                if (EC_ECINITCMDDESC_GET_VALIDATEMASK(pInitCmdDesc))
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%04x' read and '0x%04x' with mask '0x%04x' expected.\n", 
                                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        (EC_T_WORD)(pDataFrame[0]), EC_GETWORD(EcInitCmdDescVData(pInitCmdDesc)), 
                                        EC_GETWORD(EcInitCmdDescVMData(pInitCmdDesc))));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%04x' read and '0x%04x' expected.\n", 
                                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        EC_GETWORD(pData), EC_GETWORD(EcInitCmdDescVData(pInitCmdDesc))));
                                }
                            }
                            else if (wEcCmdLen == sizeof(EC_T_BYTE))
                            {
                                if (EC_ECINITCMDDESC_GET_VALIDATEMASK(pInitCmdDesc))
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%02x' read and '0x%02x' with mask '0x%02x' expected.\n", 
                                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        (EC_T_BYTE)(pDataFrame[0]), *(EC_T_BYTE*)EcInitCmdDescVData(pInitCmdDesc), 
                                        *(EC_T_BYTE*)EcInitCmdDescVMData(pInitCmdDesc)));
                                }
                                else
                                {
                                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'. \nValue '0x%02x' read and '0x%02x' expected.\n", 
                                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc), 
                                        *(EC_T_BYTE*)pData, *(EC_T_BYTE*)EcInitCmdDescVData(pInitCmdDesc)));
                                }
                            }
                            else
                            {
                                EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%s'.\n", 
                                        m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)), EcInitCmdDescComment(pInitCmdDesc)));
                            }
                                
                            EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcSlave::ProcessCmdResult() %s: inactivate init commands, m_wCurrStateMachDevState = %s\n", m_pEniSlave->szName, SlaveDevStateText(m_wCurrStateMachDevState)));
                                
                            /* don't continue sending init commands */
                            StopInitCmds( ECAT_INITCMD_FAILURE, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF));
                        } /* if (m_oInitCmdsTimeout.IsElapsed()) */
                        else
                        {
                            /* maybe the requested state is not reached in the slave device, continue waiting... */
                            dwRes = QueueEcatInitCmd(
                                        dwInvokeId,
                                        pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd,
                                        EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                                        EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)),
                                        EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader)),
                                        EcInitCmdDescData(pInitCmdDesc),
                                        (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF)
                                        );
                            if (EC_E_NOERROR != dwRes)
                            {
                                if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                                {
                                    /* start retry timeout to retry later. Don't decrement retry counter, because InitCmd was not sent */
                                    m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                                }
                            }
                        }
                        bNext = EC_FALSE;
                    }
                }
                /* check for AL status read */
                if ((EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader) == ECREG_AL_STATUS) && 
                    ((EC_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHeader) == EC_CMD_TYPE_APRD) || (EC_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHeader) == EC_CMD_TYPE_FPRD)))
                {
                    EC_T_WORD wAlStatus = EC_GET_FRM_WORD(pbyData);

                    /* refresh bus slave */
                    if (EC_NULL != m_pBusSlave)
                    {
                        m_pBusSlave->SetAlStatus(wAlStatus);
                    }
                    /* detect slave error */
                    if (wAlStatus & DEVICE_STATE_ERROR)
                    {
                        if (6 <= EC_ICMDHDR_GET_LEN_LEN(pEcCmdHeader))
                        {
                        EC_T_WORD wAlStatusCode = EC_GET_FRM_WORD((pbyData+(ECREG_AL_STATUSCODE_LO-ECREG_AL_STATUS_LO)));

                            /* refresh bus slave */
                            if (EC_NULL != m_pBusSlave)
                            {
                                m_pBusSlave->SetAlStatusCode(wAlStatusCode);
                            }
                            /* notify application */
                            m_pMaster->NotifySlaveError(this, wAlStatus, wAlStatusCode);
                        }
                        else
                        {
                            /* signalize slave error for further handling */
                            m_pMaster->SetSlaveErrorDetected(EC_TRUE);
                        }
                        /* current state machine request isn't achievable */
                        StopInitCmds(ECAT_INITCMD_FAILURE, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF));
                        bNext = EC_FALSE;
                    }
#if (defined INCLUDE_MASTER_OBD)
                    m_pMaster->m_pSDOService->UpdateSlaveState((EC_T_WORD)(0-m_wAutoIncAddr), wAlStatus);
#endif

                    m_pMaster->RecalculateCycCmdWkc(this);
                }
                if (bNext)
                {
                    /* check for SyncManager configuration */
                    {
                    EC_T_WORD  wInitCmdAdo   = EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader));
                    EC_T_WORD  wInitCmdLen   = EC_ICMDHDR_GET_LEN_LEN(&(pInitCmdDesc->EcCmdHeader));
                    EC_T_BYTE* pbyInitCmdDat = EcInitCmdDescData(pInitCmdDesc);

                        if ((ECREG_SYNCMANAGER_CONFIG <= wInitCmdAdo) && (wInitCmdAdo < (ECREG_SYNCMANAGER_CONFIG+(16*8))))
                        {
                        EC_T_WORD wSyncManChan     = 0;
                        EC_T_WORD wEstimatedSMAddr = 0;

                            do 
                            {
                                wEstimatedSMAddr = (EC_T_WORD)(ECREG_SYNCMANAGER_CONFIG + (wSyncManChan*8));
                                if (wEstimatedSMAddr < wInitCmdAdo )
                                {
                                    /* later sync Manager is addressed */
                                    wSyncManChan++;
                                    continue;
                                }
                                /* shift to relevant fields 4 = Type, 6 = Activation -> after shift: 0=type 2=activation */
                                pbyInitCmdDat = (pbyInitCmdDat + 4 - (wEstimatedSMAddr-wInitCmdAdo));
                                wInitCmdLen   = (EC_T_WORD)(wInitCmdLen - (4 - (wEstimatedSMAddr-wInitCmdAdo)));
                                break;
                            } while (wSyncManChan < 16);
                            do 
                            {
                                if (2 >= wInitCmdLen)
                                {
                                    break;
                                }
                                m_wSyncManagerEnabled &= ~(0x01<<wSyncManChan);
                                m_wSyncManagerEnabled |= ((pbyInitCmdDat[2]&0x01)<<(wSyncManChan));

                                if (8 >= wInitCmdLen)
                                {
                                    break;
                                }
                                pbyInitCmdDat += 8;
                                wInitCmdLen   -= 8;
                                wSyncManChan++;
                            } while (wSyncManChan < 16);
                        }
                    }
                    /* init command is valid -> process next init command */
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcSlave::ProcessCmdResult() %s: start next init command\n", m_pEniSlave->szName));
                    StartInitCmds((EC_T_WORD)dwInvokeId, EC_FALSE);
                }
            } /* if (pInitCmdDesc->cnt == 0xffff || ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == pInitCmdDesc->cnt ) */
            else
            {   /* invalid working counter -> retry */
                if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                {
                    if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                    {
                        m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        m_wInitCmdsRetries--;
                    }
                    pNotification = m_pMaster->AllocNotification();
                    if (EC_NULL != pNotification)
                    {
                        pNotification->uNotification.ErrorNotification.dwNotifyErrorCode  = EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR;
                        GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.SlaveProp));
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(&(pInitCmdDesc->EcCmdHeader));
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
                        pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = EC_ECINITCMDDESC_GET_CNT(pInitCmdDesc);
                            
                        m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR);
                    }

                    if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
                    {
                        EC_ERRORMSGC_L(
                            ("CEcSlave::ProcessCmdResult() Invalid working counter retry send %s InitCmd[%d-%d]: %s: %s,adp=%d,ado=0x%x\n",
                                (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? "Master" : "Slave"),
                                (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr),
                                (EC_ECINITCMDDESC_GET_MASTERINITCMD(pInitCmdDesc) ? m_wMasterInitCmdsCurrent : m_wInitCmdsCurrent),
                                GetStateChangeNameShort((EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc) & 0x7FF)),
                                EcatCmdShortText(pInitCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd),
                                (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADP(&(pInitCmdDesc->EcCmdHeader)),
                                (EC_T_INT)EC_ICMDHDR_GET_ADDR_ADO(&(pInitCmdDesc->EcCmdHeader)))
                            );
                    }
                }
                else
                {
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcSlave::ProcessCmdResult() %s: inactivate init commands, m_wCurrStateMachDevState = %s\n", m_pEniSlave->szName, SlaveDevStateText(m_wCurrStateMachDevState)));

                    /* don't continue sending init commands */
                    StopInitCmds( ECAT_INITCMD_FAILURE, (EC_T_WORD)(EC_ECINITCMDDESC_GET_TRANSITION(pInitCmdDesc)&0x7FF));
                }
            }
        } break;
    case INVOKE_ID_WRITEEVENTMASK:
        {
            if (result == EC_E_NOERROR)
            {
                if (wWkc == 1)
                {
                    if (EC_NULL != m_pBusSlave)
                    {
	                    m_pBusSlave->SetEcatEventMask(EC_GET_FRM_WORD(pbyData));
                    }
                }
            }
        } break;

    case INVOKE_ID_TFER_RAW_CMD:
        {
            OsLock(m_poSlaveLock);
            if (m_dwRawTferStatus == EC_E_BUSY)
            {
                OsDbgAssert(m_bRawTferPending);
                if (result == EC_E_NOERROR)
                {
                    if (ETYPE_EC_CMD_GETWKC(pEcCmdHeader) >= 1 )
                    {
                        /* slave did respond */
                        if (m_wRawTferDataLen > 0 )
                        {
                            /* copy data into user area */
                            EC_T_BYTE* pbyTferData;
                            pbyTferData = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);
                            OsDbgAssert(m_wRawTferDataLen <= MAX_EC_DATA_LEN );
                            OsDbgAssert(m_pbyRawTferData != EC_NULL);
                            OsMemcpy(m_pbyRawTferData, pbyTferData, m_wRawTferDataLen);
                        }
                        m_dwRawTferStatus = EC_E_NOERROR;
                    }
                    else
                    {
                        EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult(): invalid working counter when receiving raw command result\nSlave \"%s\"\n", m_pEniSlave->szName));
                        m_dwRawTferStatus = EC_E_NOTREADY;
                    }
                }
                else
                {
                    /* communication error */
                    EC_ERRORMSGC_L( ("CEcSlave::ProcessCmdResult() '%s' (0x%x): Communication Error '0x%x'.\n", m_pEniSlave->szName, m_wFixedAddr, result));
                    m_dwRawTferStatus = result;
                }
                OsDbgAssert(m_dwRawTferStatus != EC_E_BUSY );
            }
            OsUnlock(m_poSlaveLock);
        }
        break;
#if (defined INCLUDE_SLAVE_STATISTICS)
    case INVOKE_ID_GET_SLAVE_STATISTICS:
        {
            EC_T_BYTE* pbyDataParse = pbyData;
            EC_T_INT   nIdx         = 0;

            /* if no response then no error here */
            if (result == EC_E_TIMEOUT)
            {
                break;
            }

            /* no response */
            if (0 == ETYPE_EC_CMD_GETWKC(pEcCmdHeader)) 
            {
                break;
            }

            if ((1 < ETYPE_EC_CMD_GETWKC(pEcCmdHeader))  || (0x14 != wEcCmdLen))
            {
                EC_ERRORMSGC_L(
                    ("CEcSlave::ProcessCmdResult(): Malformed response to Slave statistic collect command WKC = 0x%x Len: 0x%lx, (%s) \n", 
                    ETYPE_EC_CMD_GETWKC(pEcCmdHeader),
                    wEcCmdLen,
                    m_pEniSlave->szName
                    ));
                break;
            }
            
            /* parse data to statistic counters */

#if (defined INCLUDE_MASTER_OBD)
            m_pMaster->m_pSDOService->UpdateErrorCounter((EC_T_WORD)(0-m_wAutoIncAddr), pbyDataParse);
#endif

            /* RX Error counter (0x300:0x307) */
            for( nIdx = 0; nIdx < 4; nIdx ++ )
            {
                m_wRxErrCounter[nIdx] = EC_GET_FRM_WORD(pbyDataParse);
                pbyDataParse += sizeof(EC_T_WORD);
            }

            /* Forwarded RX Error Counter (0x0308:0x030B) */
            for( nIdx = 0; nIdx < 4; nIdx ++ )
            {
                m_byFwdRxErrCounter[nIdx] = pbyDataParse[0];
                pbyDataParse += sizeof(EC_T_BYTE);
            }

            /* ECAT Processing Unit Error Counter (0x30C) */
            m_byProcUnitErrCounter = pbyDataParse[0];
            pbyDataParse += sizeof(EC_T_BYTE);

            /* PDI Error Counter (0x30D) */
            m_byPDIErrCounter = pbyDataParse[0];
            pbyDataParse += sizeof(EC_T_WORD)+sizeof(EC_T_BYTE);
            
            /* Lost Link Counter (0x0310:0x313) */
            for( nIdx = 0; nIdx < 4; nIdx ++ )
            {
                m_byLostLinkCounter[nIdx] = pbyDataParse[0];
                if (3 != nIdx )
                {
                    pbyDataParse += sizeof(EC_T_BYTE);
                }
            }
        }
        break;
#endif
    default:
        {
            if (dwInvokeId >= INVOKE_ID_CLNT_TFER_Q_RAW_CMD )
            {
                EC_T_WORD  wInvokeId   = EC_LOWORD(dwInvokeId);
                EC_T_BYTE* pbyTferData;
#ifdef DEBUG
                EC_T_DWORD dwNumNotify = 0;
#endif                
                pNotification = m_pMaster->AllocNotification();
                if (EC_NULL != pNotification)
                {
                   pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwInvokeId = EC_MAKEDWORD(0,wInvokeId);
                   pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwResult   = result;
                   if (EC_NULL != pEcCmdHeader )
                   {
                       pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwWkc      = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
                       pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwCmdIdx   = EC_MAKEDWORD(0, EC_ICMDHDR_GET_CMDIDX(pEcCmdHeader));
                       pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwAddr     = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
                       pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.dwLength   = EC_MAKEDWORD(0, wEcCmdLen);
                       
                       pbyTferData = (EC_T_BYTE*)EC_ENDOF(pEcCmdHeader);
                       pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.pbyData = pbyTferData;
                   }
                   if ((dwInvokeId & INVOKE_ID_TFER_Q_RAW_CMD) == INVOKE_ID_TFER_Q_RAW_CMD )
                   {
                       /* broadcast raw cmd */
                       pNotification->dwClientID = 0;
#ifdef DEBUG
                       dwNumNotify = m_pMaster->NotifyAndFree(EC_NOTIFY_RAWCMD_DONE, pNotification, sizeof(EC_T_RAWCMDRESPONSE_NTFY_DESC));
#else
                       m_pMaster->NotifyAndFree(EC_NOTIFY_RAWCMD_DONE, pNotification, sizeof(EC_T_RAWCMDRESPONSE_NTFY_DESC));
#endif
                   }
                   else
                   {
                       EC_T_BYTE byClntIdent;
   
                       /* client specific raw cmd */
                       byClntIdent = (EC_T_BYTE)((dwInvokeId >> 16) & 0xFF);
   
                       pNotification->dwClientID = IS_CLIENT_RAW_CMD | (EC_T_DWORD)byClntIdent;
#ifdef DEBUG
                       dwNumNotify = m_pMaster->NotifyAndFree(EC_NOTIFY_RAWCMD_DONE, pNotification, sizeof(EC_T_RAWCMDRESPONSE_NTFY_DESC));
                       OsDbgAssert(dwNumNotify == 1 );
#else
                       m_pMaster->NotifyAndFree(EC_NOTIFY_RAWCMD_DONE, pNotification, sizeof(EC_T_RAWCMDRESPONSE_NTFY_DESC));
#endif
                   }
                }
            }
            else
            {
                EC_TRACEMSG(EC_TRACE_INITCMDS, ("---->[%06d] CEcSlave::ProcessCmdResult(cmdIdx=0x%x) %s: Test dwInvokeId = 0x%x\n", 
                    OsQueryMsecCount(), (EC_T_DWORD)EC_ICMDHDR_GET_CMDIDX(pEcCmdHeader), m_pEniSlave->szName, (EC_T_INT)dwInvokeId));
                EC_ERRORMSG_L(("CEcSlave::ProcessCmdResult() %s: invalid dwInvokeId = 0x%x\n", m_pEniSlave->szName, (EC_T_INT)dwInvokeId));
            };
        };
    }
}

/***************************************************************************************************/
/**
\brief  Create Port Descriptor from Configuration.

\return Port Descriptor Value (BYTE).
*/
EC_T_BYTE CEcSlave::GetPhysicsDesc(EC_T_VOID)
{
    EC_T_BYTE   byPortDesc  = 0;

    for( EC_T_BYTE byCnt = 0; byCnt < 4; byCnt++ )
    {
        switch (m_pEniSlave->szPhysics[byCnt])
        {
        case 'K': /* E-Bus */
            {
                byPortDesc |= ((EC_T_BYTE)(0x2<<(byCnt*2))); 
            } break;
        case 'Y':
        case 'H': /* MII */
            {
                byPortDesc |= ((EC_T_BYTE)(0x3<<(byCnt*2))); 
            } break;
        case '\0': /* not configured */
        default: 
            {
                byPortDesc |= ((EC_T_BYTE)(0x0<<(byCnt*2))); 
            } break;
        }
    }

    return byPortDesc;
}

/********************************************************************************/
/** \brief Determine slave properties.
*
* \return  N/A.
*/
EC_T_VOID CEcSlave::GetSlaveProp
(   EC_T_SLAVE_PROP* pSlaveProp )   /* [out]  slave properties */
{
    SAFE_STRCPY( pSlaveProp->achName, m_pEniSlave->szName, MAX_STD_STRLEN );
    pSlaveProp->wAutoIncAddr = m_wAutoIncAddr;
    pSlaveProp->wStationAddress = m_wFixedAddr;
    return;
}

/********************************************************************************/
/** \brief Gets the name of the EtherCAT slave.
*
* \return The name of the EtherCAT slave.
*/
const EC_T_CHAR* CEcSlave::GetName( EC_T_VOID )
{
    return m_pEniSlave->szName;
}


/********************************************************************************/
/** \brief Transfer a raw EtherCAT command.
*
* \return  status value.
*/
EC_T_DWORD CEcSlave::EcTransferRawCmd
(   EC_T_DWORD dwUniqueClntId,      /* [in]  client id, 0 if broadcast */
    EC_T_WORD wInvokeId,            /* [in]  invoke id to be used for deferred transfers (timeout EC_NOWAIT) */
    EC_T_BYTE byCmd,                /* [in]  EtherCAT command */
    EC_T_DWORD dwMemoryAddress,     /* [in]  address inside the slave */
    EC_T_VOID* pvData,              /* [in/out]  data */
    EC_T_WORD wLen,                 /* [in]  size of data */
    EC_T_DWORD dwTimeout            /* [in]  Timeout value. If set to EC_NOWAIT the transfer is queued and the application will get a notification */
)
{
CEcTimer oTimeout;
EC_T_DWORD dwRetval = EC_E_NOERROR;
EC_T_WORD  wAdp, wAdo;
EC_T_BOOL  bReadData = EC_TRUE;
EC_T_BOOL  bInvalidateSingleRawTferRequest = EC_FALSE;
EC_T_BOOL  bSlaveLockTaken = EC_FALSE;
    
    if (wLen > MAX_EC_DATA_LEN )
    {
        dwRetval = EC_E_INVALIDSIZE;
        goto Exit;
    }
    switch( byCmd )
    {
    case EC_CMD_TYPE_APRD:
    case EC_CMD_TYPE_APWR:
    case EC_CMD_TYPE_APRW:
    case EC_CMD_TYPE_ARMW:
        wAdp = m_wAutoIncAddr;
        wAdo = EC_LOWORD(dwMemoryAddress);
        break;
    case EC_CMD_TYPE_FPRD:
    case EC_CMD_TYPE_FPWR:
    case EC_CMD_TYPE_FPRW:
    case EC_CMD_TYPE_FRMW:
        wAdp = m_wFixedAddr;
        wAdo = EC_LOWORD(dwMemoryAddress);
        break;
    case EC_CMD_TYPE_LRD:
    case EC_CMD_TYPE_LWR:
    case EC_CMD_TYPE_LRW:
        wAdp = EC_LOWORD(dwMemoryAddress);
        wAdo = EC_HIWORD(dwMemoryAddress);
        break;
    case EC_CMD_TYPE_BRD:
    case EC_CMD_TYPE_BWR:
    case EC_CMD_TYPE_BRW:
        wAdp = 0;
        wAdo = EC_LOWORD(dwMemoryAddress);
        break;
    default:
        /* command not supported */
        dwRetval = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (   (byCmd == EC_CMD_TYPE_APWR)
        || (byCmd == EC_CMD_TYPE_FPWR)
        || (byCmd == EC_CMD_TYPE_BWR)
        || (byCmd == EC_CMD_TYPE_LWR)
      )
    {
        bReadData = EC_FALSE;
    }

    if (dwTimeout == EC_NOWAIT )
    {
    EC_T_DWORD dwInvokeId;
    EC_T_DWORD dwClntIdent;
        if (dwUniqueClntId == 0 )
        {
            /* broadcast */
            dwInvokeId = (EC_T_DWORD)( INVOKE_ID_TFER_Q_RAW_CMD | wInvokeId);
        }
        else
        {
            /* client specific cmd */
            OsDbgAssert((INVOKE_ID_TFER_Q_RAW_CMD - (dwUniqueClntId&(~IS_CLIENT_RAW_CMD))) >= INVOKE_ID_CLNT_TFER_Q_RAW_CMD );
            OsDbgAssert((INVOKE_ID_TFER_Q_RAW_CMD - INVOKE_ID_CLNT_TFER_Q_RAW_CMD) == 0xFF0000 );
            OsDbgAssert(dwUniqueClntId & IS_CLIENT_RAW_CMD );
            dwClntIdent = (EC_T_DWORD)(((dwUniqueClntId & (~IS_CLIENT_RAW_CMD)) & 0xFF) << 16);
            dwInvokeId = (EC_T_DWORD)(INVOKE_ID_CLNT_TFER_Q_RAW_CMD | dwClntIdent | wInvokeId);
        }
        dwRetval = m_pMaster->QueueEcatCmdReq(EC_FALSE, this, dwInvokeId, byCmd, wAdp, wAdo, wLen, pvData );

    }
    else
    {
        OsDbgAssert(!(dwUniqueClntId & IS_CLIENT_RAW_CMD));
        OsLock(m_poSlaveLock); bSlaveLockTaken = EC_TRUE;
        if ((m_dwRawTferStatus == EC_E_BUSY) || m_bRawTferPending )
        {
            /* another request is pending (e.g. issued by another thread) */
            dwRetval = EC_E_BUSY;
            goto Exit;
        }
        /* issue a new raw transfer */
        m_dwRawTferStatus = EC_E_BUSY;
        m_bRawTferPending = EC_TRUE;
        bInvalidateSingleRawTferRequest = EC_TRUE;
        if (bReadData )
        {
            m_wRawTferDataLen = wLen;
            m_pbyRawTferData = (EC_T_BYTE*)pvData;
        }
        else
        {
            m_wRawTferDataLen = 0;
            m_pbyRawTferData = EC_NULL;
        }
        OsDbgAssert(bSlaveLockTaken); OsUnlock(m_poSlaveLock); bSlaveLockTaken = EC_FALSE;
        
        dwRetval = m_pMaster->QueueEcatCmdReq(EC_FALSE, this, INVOKE_ID_TFER_RAW_CMD, byCmd, wAdp, wAdo, wLen, pvData );
        
        if (dwRetval != EC_E_NOERROR )
        {
            goto Exit;
        }

        /* wait for response */
        oTimeout.Start(dwTimeout, m_pMaster->GetMsecCounterPtr());
        while ((m_dwRawTferStatus == EC_E_BUSY) && !oTimeout.IsElapsed())
        {
            OsSleep(1);
        }
        dwRetval = m_dwRawTferStatus;
        if ((dwRetval == EC_E_BUSY) && oTimeout.IsElapsed())
        {
            /* a timeout occurred */
            dwRetval = EC_E_TIMEOUT;
            goto Exit;
        }

        if (!m_poEcDevice->IsLinkConnected())
        {
            /* link cable disconnected */
            dwRetval = EC_E_LINK_DISCONNECTED;
            goto Exit;
        }
    } /* else if (dwTimeout == EC_NOWAIT ) */
    

Exit:
    if (bInvalidateSingleRawTferRequest )
    {
        /* invalidate request */
        if (!bSlaveLockTaken)
        {
            OsLock(m_poSlaveLock);
            bSlaveLockTaken=EC_TRUE;
        }
        if (dwRetval == EC_E_BUSY )
        {
            /* do not leave status in BUSY state! */
            m_dwRawTferStatus = EC_E_NOERROR;
        }
        else
        {
            m_dwRawTferStatus = dwRetval;
        }
        m_wRawTferDataLen = 0;
        m_pbyRawTferData = EC_NULL;
        m_bRawTferPending = EC_FALSE;
    }
    if (bSlaveLockTaken)
    {
        OsUnlock(m_poSlaveLock);
        bSlaveLockTaken = EC_FALSE;
    }
    return dwRetval;
}

/***************************************************************************************************/
/**
\brief  Retrieve a specific Configuration value by its "address".

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSlave::GetConfigValue(EC_T_WORD wAddr, EC_T_DWORD& dwVal)
{
    EC_T_DWORD dwRetVal = EC_E_NOTFOUND;

    switch( wAddr )
    {
    case ESC_SII_REG_VENDORID:
        {
            dwVal = m_pEniSlave->dwVendorId;
            dwRetVal = EC_E_NOERROR;
            break;
        }
    case ESC_SII_REG_PRODUCTCODE:
        {
            dwVal = m_pEniSlave->dwProductCode;
            dwRetVal = EC_E_NOERROR;
            break;
        }
    case ESC_SII_REG_REVISIONNUMBER:
        {
            dwVal = m_pEniSlave->dwRevisionNumber;
            dwRetVal = EC_E_NOERROR;
            break;
        }
    case ESC_SII_REG_SERIALNUMBER:
        {
            dwVal = m_pEniSlave->dwSerialNumber;
            dwRetVal = EC_E_NOERROR;
            break;
        }
    default:
        {
            /* generic handling */
            break;
        }
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Internally stored Slave Information.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSlave::GetSlaveInfo(
                                  EC_T_eINFOENTRY   eEntry,     /**< [in]   read entry index */
                                  EC_T_BYTE*        pbyData,    /**< [out]  value read */
                                  EC_T_DWORD*       pdwLen      /**< [out]  size of read value */
                                 )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwTmpVal = 0;

    if (EC_NULL == pbyData )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    switch( eEntry )
    {
    case eie_pdoffs_in:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDInOffset[0];

            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
                }
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_in:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDInSize[0]);
        } break;
    case eie_pdoffs_out:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDOutOffset[0];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
                }
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_out:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDOutSize[0]);
        } break;
    case eie_pdoffs_in2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDInOffset[1];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
                }
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_in2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDInSize[1]);
        } break;
    case eie_pdoffs_out2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDOutOffset[1];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
                }
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_out2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDOutSize[1]);
        } break;
    case eie_pdoffs_in3:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDInOffset[2];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
                }
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_in3:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDInSize[2]);
        } break;
    case eie_pdoffs_out3:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDOutOffset[2];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
                }
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_out3:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDOutSize[2]);
        } break;
    case eie_pdoffs_in4:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDInOffset[3];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
                }
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_in4:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDInSize[3]);
        } break;
    case eie_pdoffs_out4:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            /* XML Value */
            dwTmpVal = m_pEniSlave->adwPDOutOffset[3];
            
            if (m_pMaster->m_bPdOffsetCompMode)
            {
                if (((EC_T_DWORD)-1) != dwTmpVal)
                {
                    /* now norm value on behalf of configurator specialties */
                    dwTmpVal = (EC_T_DWORD)((dwTmpVal + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
                }
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, dwTmpVal);
        } break;
    case eie_pdsize_out4:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, m_pEniSlave->adwPDOutSize[3]);
        } break;
/*
    case eie_phys_address:
        {
            if (sizeof(EC_T_WORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }

            *pdwLen = sizeof(EC_T_WORD);
            EC_SETWORD(pbyData, m_wFixedAddr);
        } break;
*/
    case eie_cfgphy_address:
        {
            if (sizeof(EC_T_WORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_WORD);
            EC_SETWORD(pbyData, m_pEniSlave->wPhysAddr);
        } break;
    case eie_device_name:
        {
            if (ECAT_DEVICE_NAMESIZE > (*pdwLen))
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            *pdwLen = ECAT_DEVICE_NAMESIZE;
            OsMemcpy(pbyData, m_pEniSlave->szName, ECAT_DEVICE_NAMESIZE);
        } break;
    case eie_ismailbox_slave:
        {
            if (sizeof(EC_T_BOOL) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            *pdwLen = sizeof(EC_T_BOOL);
            EC_SETDWORD(pbyData, HasMailBox());
        } break;
    case eie_mbx_outsize:
    case eie_mbx_insize:
    case eie_mbx_outsize2:
    case eie_mbx_insize2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            if (!HasMailBox())
            {
                *pdwLen = sizeof(EC_T_DWORD);
                EC_SETDWORD(pbyData, 0);
            }
            else
            {   
                CEcMbSlave* pMbxSlave = (CEcMbSlave*)this;
                dwRetVal = pMbxSlave->GetMbSlaveInfo(eEntry, pbyData, pdwLen);
            }

        } break;
    case eie_isoptional:
        {
            if (sizeof(EC_T_BOOL) > (*pdwLen))
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            EC_SETDWORD(pbyData, m_bIsOptional);
        } break;
    case eie_ispresent:
        {
            if (sizeof(EC_T_BOOL) > (*pdwLen))
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            EC_SETDWORD(pbyData, IsPresentOnBus());
        } break;
    case eie_unknown:
    default:
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        };
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/*-CEcMbSlave----------------------------------------------------------------*/

#if (defined INCLUDE_RAWMBX_SUPPORT)
MBXPROTOCOL_DESC* CEcMbSlave::CreateMbxProtoDesc(EC_T_DWORD dwMaxQueuedCmds)
{
    ECMBSLAVE_RAWMBX_DESC* pRawMbxDesc = EC_NULL;

    pRawMbxDesc = (ECMBSLAVE_RAWMBX_DESC*)OsMalloc(sizeof(ECMBSLAVE_RAWMBX_DESC));
    if (EC_NULL == pRawMbxDesc)
    {
        return EC_NULL;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::CEcMbSlave", pRawMbxDesc, sizeof(ECMBSLAVE_RAWMBX_DESC));
    OsMemset(pRawMbxDesc, 0, sizeof(ECMBSLAVE_RAWMBX_DESC));
#ifdef DEBUGTRACE
    pRawMbxDesc->mbxDesc.pCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(dwMaxQueuedCmds, m_poSlaveLock, (EC_T_CHAR*)"QueuedMbxCmds", EC_MEMTRACE_CORE_SLAVE));
#else
    pRawMbxDesc->mbxDesc.pCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(dwMaxQueuedCmds, m_poSlaveLock, (EC_T_CHAR*)"QueuedMbxCmds"));
#endif
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::CEcMbSlave", pRawMbxDesc->mbxDesc.pCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
    if (pRawMbxDesc->mbxDesc.pCmdFifo == EC_NULL)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::CEcMbSlave", pRawMbxDesc->mbxDesc.pCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::CEcMbSlave", pRawMbxDesc, sizeof(ECMBSLAVE_RAWMBX_DESC));
        SafeDelete(pRawMbxDesc->mbxDesc.pCmdFifo);
        SafeDelete(pRawMbxDesc);
    }
    return (MBXPROTOCOL_DESC*)pRawMbxDesc;
}
EC_T_VOID CEcMbSlave::ClearMbxProtoDesc(MBXPROTOCOL_DESC* pMbxDesc)
{
    if (EC_NULL != pMbxDesc->cur.pCmd)
    {
        OsDbgAssert(pMbxDesc->cur.bIsValid);
    }
    OsMemset(&pMbxDesc->cur, 0, sizeof(pMbxDesc->cur));
}

EC_T_VOID CEcMbSlave::DeleteMbxProtoDesc(MBXPROTOCOL_DESC* pMbxDesc)
{
PEcMailboxCmd pCmd;

#if (defined DEBUG)
    if (pMbxDesc->cur.bIsValid)
    {
        OsDbgAssert(!pMbxDesc->cur.pCmd->bInUseByInterface);
    }
#endif
    ClearMbxProtoDesc(pMbxDesc);
    while (pMbxDesc->pCmdFifo->RemoveNoLock(pCmd));
    
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::~CEcMbSlave", pMbxDesc->pCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::~CEcMbSlave", pMbxDesc, sizeof(ECMBSLAVE_RAWMBX_DESC));
    SafeDelete(pMbxDesc->pCmdFifo);
    SafeOsFree(pMbxDesc);
}
#endif /* INCLUDE_RAWMBX_SUPPORT */

/********************************************************************************/
/** \brief CEcMbSlave constructor
*
* \return
*/
CEcMbSlave::CEcMbSlave(CEcMaster* pMaster, EC_T_ENI_SLAVE* pEniSlave, CEcDevice* poEthDevice, EC_T_DWORD dwMsecCounter ) : 
CEcSlave(pMaster, pEniSlave, poEthDevice)
{
    EC_T_DWORD              dwCmdIndex;
    EC_T_INT                nAllIndex;

    m_byNumMbxProtocol            = 0;
    m_wSupportedMbxProtocols      = 0;
    m_bMbxReadStateIsBusy         = EC_FALSE;
    m_wRetryAccessCount           = EC_MBX_RETRYACCESS_COUNT;
    m_wRetryAccessPeriod          = EC_MBX_RETRYACCESS_PERIOD;
    m_wEoeRetryAccessPeriod       = (EC_T_WORD)(pMaster->GetBusCycleTimeUsec() / 1000); /* starting with bus cycle time */
    memset(&m_mbxLayer, 0, sizeof(m_mbxLayer));

    m_mbxOStart[REGULAR_MAILBOX]  = pEniSlave->wMboxOutStart;
    m_mbxOLen[REGULAR_MAILBOX]    = pEniSlave->wMboxOutLen;
    m_mbxIStart[REGULAR_MAILBOX]  = pEniSlave->wMboxInStart;
    m_mbxILen[REGULAR_MAILBOX]    = pEniSlave->wMboxInLen;

    m_mbxOStart[BOOT_MAILBOX]     = pEniSlave->wMbxBootOutStart;
    m_mbxOLen[BOOT_MAILBOX]       = pEniSlave->wMbxBootOutLen;
    m_mbxIStart[BOOT_MAILBOX]     = pEniSlave->wMbxBootInStart;
    m_mbxILen[BOOT_MAILBOX]       = pEniSlave->wMbxBootInLen;
    
    m_sMboxFlags.mbxIdx           = REGULAR_MAILBOX;
    m_sMboxFlags.bMbxOutShortSend = pEniSlave->sParm.bMbxOutShortSend;

    m_wMailboxActionRequest       = iecs_mbx_idle;
    m_wMailboxActionState         = iecs_mbx_idle;
    m_bSerializeMbxProtocols      = EC_FALSE;
    m_bMbxCmdPending              = EC_FALSE;

    m_dwLastOnTimerCall           = dwMsecCounter;
    m_dwCurMbxPollingAge          = 0;

    /* according to the master sample documentation either PollTime or StatusBitAddr 
     * must be configured. Both should never be set */
    OsDbgAssert((pEniSlave->sParm.bCyclicPhysicalMBoxPolling != 0) || (pEniSlave->sParm.bLogicalStateMBoxPolling != 0));    /* at least one! */
    OsDbgAssert(!((pEniSlave->sParm.bCyclicPhysicalMBoxPolling == 0) && (pEniSlave->sParm.bLogicalStateMBoxPolling == 0))); /* not both! */
#if (defined INCLUDE_FOE_SUPPORT)
    if (pEniSlave->sParm.bFoeSupport )
    {
        OsDbgAssert((pEniSlave->sParm.bCyclicPhysicalMBoxPolling != 0) || (pEniSlave->sParm.bLogicalStateMBoxPolling != 0));    /* at least one! */
    }
    if (pEniSlave->sParm.bBootStrapSupport)
    {
        OsDbgAssert(!((pEniSlave->sParm.bCyclicPhysicalMBoxBootPolling == 0) && (pEniSlave->sParm.bLogicalStateMBoxBootPolling == 0))); /* not both! */
    }
#endif

    /* specifies how often the CMbSlave::OnTimer is called before the mailbox is checked for a new message */
    m_wCyclePhysicalMBoxPollingInterval = pEniSlave->wCyclePhysicalMBoxPollingTime;    
    if (0 == m_wCyclePhysicalMBoxPollingInterval)
    {
        /* use default value */
        m_wCyclePhysicalMBoxPollingInterval = (EC_T_WORD)pMaster->GetDefaultMbxPollingTime();
    }
    m_sMboxFlags.bCyclicPhysicalMBoxPolling   = pEniSlave->sParm.bCyclicPhysicalMBoxPolling && m_wCyclePhysicalMBoxPollingInterval > 0; 
    if ((m_sMboxFlags.bCyclicPhysicalMBoxPolling) && (m_wCyclePhysicalMBoxPollingInterval == 0))
    {
        /* mailbox state not mapped via FMMU and cyclically read by LRD: if no value has been specified use default polling time */
        m_wCyclePhysicalMBoxPollingInterval = (EC_T_WORD)pMaster->GetDefaultMbxPollingTime();
    }
    else if ((!m_sMboxFlags.bCyclicPhysicalMBoxPolling))
    {
        /* mailbox state mapped via FMMU and cyclically read by LRD: use default polling time if cyclic commands (LRD) are missing */
        OsDbgAssert(pEniSlave->wCyclePhysicalMBoxPollingTime == 0 );    /* there should no polling time be set */
        m_wCyclePhysicalMBoxPollingInterval = (EC_T_WORD)pMaster->GetDefaultMbxPollingTime();
    }
    m_sMboxFlags.bLogicalStateMBoxPolling = pEniSlave->sParm.bLogicalStateMBoxPolling;
    m_wSlaveLogicalAddressMBoxState       = pEniSlave->wSlaveLogicalAddressMBoxState;      

#if (defined INCLUDE_FOE_SUPPORT)
    m_wCyclePhysicalMBoxBootPollingInterval = pEniSlave->wCyclePhysicalMBoxBootPollingTime;
    if (0 == m_wCyclePhysicalMBoxBootPollingInterval)
    {
        /* use default value */
        m_wCyclePhysicalMBoxBootPollingInterval = (EC_T_WORD)pMaster->GetDefaultMbxPollingTime();
    }
    m_sMboxFlags.bCyclicPhysicalMBoxBootPolling = pEniSlave->sParm.bCyclicPhysicalMBoxBootPolling && m_wCyclePhysicalMBoxBootPollingInterval > 0; 
    if ((m_sMboxFlags.bCyclicPhysicalMBoxBootPolling) && (m_wCyclePhysicalMBoxBootPollingInterval == 0))
    {
        /* mailbox state not mapped via FMMU and cyclically read by LRD: if no value has been specified use default polling time */
        m_wCyclePhysicalMBoxBootPollingInterval = (EC_T_WORD)pMaster->GetDefaultMbxPollingTime();
    }
    else if ((!m_sMboxFlags.bCyclicPhysicalMBoxBootPolling))
    {
        /* mailbox state mapped via FMMU and cyclically read by LRD: use default polling time if cyclic commands (LRD) are missing */
        OsDbgAssert(pEniSlave->wCyclePhysicalMBoxBootPollingTime == 0 );    /* there should no polling time be set */
        m_wCyclePhysicalMBoxBootPollingInterval = (EC_T_WORD)pMaster->GetDefaultMbxPollingTime();
    }
    m_sMboxFlags.bLogicalStateMBoxBootPolling = pEniSlave->sParm.bLogicalStateMBoxBootPolling;
    m_wSlaveLogicalAddressMBoxBootState       = pEniSlave->wSlaveLogicalAddressMBoxBootState;      
#endif

    /* COE */
    m_pCoEDesc = EC_NULL;
    if (pEniSlave->sParm.bCoeSupport)
    {/* EtherCAT slaves supports the CANopen over EtherCAT protocol */
        m_pCoEDesc = EC_NEW(ECMBSLAVE_COE_DESC);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pCoEDesc", m_pCoEDesc, sizeof(ECMBSLAVE_COE_DESC));

        m_wSupportedMbxProtocols |= EC_MBX_PROTOCOL_COE;
        m_byNumMbxProtocol++;
        
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid = EC_FALSE;
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry = EC_FALSE;
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_FALSE;

        if (m_pCoEDesc != EC_NULL)
        {
            OsMemset(m_pCoEDesc, 0, sizeof(ECMBSLAVE_COE_DESC));
            
#ifdef DEBUGTRACE
            m_pCoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_COE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendCoeCmds", EC_MEMTRACE_CORE_SLAVE));
#else
            m_pCoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_COE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendCoeCmds"));
#endif
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::CEcMbSlave", m_pCoEDesc->pPendMailboxCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
            if (m_pCoEDesc->pPendMailboxCmdFifo == EC_NULL)
            {
                FreeMbxProtocolDesc(&m_pCoEDesc, GetMemLog());
            }
        }
    }
#if (defined INCLUDE_EOE_SUPPORT)
    m_pEoEDesc = EC_NULL;
    if (pEniSlave->sParm.bEoeSupport)
    {
        m_wSupportedMbxProtocols |= EC_MBX_PROTOCOL_EOE;
        m_byNumMbxProtocol++;
        CEcEthSwitch* ipSwitch = m_pMaster->EthernetSwitch();
        if (EC_NULL != ipSwitch)
        {
            m_pEoEDesc = EC_NEW(ECMBSLAVE_EOE_DESC);
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pEoEDesc", m_pEoEDesc, sizeof(ECMBSLAVE_EOE_DESC));

            if (m_pEoEDesc != EC_NULL)
            {
                OsMemset(m_pEoEDesc, 0, sizeof(ECMBSLAVE_EOE_DESC));
                m_pEoEDesc->pPort = EC_NEW(CEthernetPort(m_pMaster->GetInstanceId(), SendFrameToSlaveWrap, this, m_pEniSlave->szName));
                EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pEoEDesc->pPort", m_pEoEDesc->pPort, sizeof(CEthernetPort));

                if (m_pEoEDesc->pPort == EC_NULL)
                {
                    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::~m_pEoEDesc", m_pEoEDesc, sizeof(ECMBSLAVE_EOE_DESC));
                    SafeDelete(m_pEoEDesc);
                }
            }
        }
    }
#endif /* INCLUDE_EOE_SUPPORT */
#if (defined INCLUDE_FOE_SUPPORT)
    m_pFoEDesc = EC_NULL;
    m_sMboxFlags.bFoeSupportNotBS   =  pEniSlave->sParm.bFoeSupport;
    m_sMboxFlags.bBootStrapSupport  =  pEniSlave->sParm.bBootStrapSupport;
    if (pEniSlave->sParm.bFoeSupport || pEniSlave->sParm.bBootStrapSupport)
    {
        m_pFoEDesc = EC_NEW(ECMBSLAVE_FOE_DESC);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pFoEDesc", m_pFoEDesc, sizeof(ECMBSLAVE_FOE_DESC));

        m_wSupportedMbxProtocols |= EC_MBX_PROTOCOL_FOE;
        m_byNumMbxProtocol++;
        if (EC_NULL != m_pFoEDesc)
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid = EC_FALSE;
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending = EC_FALSE;
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry = EC_FALSE;

            OsMemset(m_pFoEDesc, 0, sizeof(ECMBSLAVE_FOE_DESC));
#ifdef DEBUGTRACE
            m_pFoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_FOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendFoeCmds", EC_MEMTRACE_CORE_SLAVE));
#else
            m_pFoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_FOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendFoeCmds"));
#endif
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pFoEDesc->pPendMailboxCmdFifo", m_pFoEDesc->pPendMailboxCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
            if (EC_NULL == m_pFoEDesc->pPendMailboxCmdFifo)
            {
                FreeMbxProtocolDesc(&m_pFoEDesc, GetMemLog());
            }
        }
    }
#endif /* INCLUDE_FOE_SUPPORT */
#ifdef INCLUDE_SOE_SUPPORT
    m_pSoEDesc = EC_NULL;
    if (pEniSlave->sParm.bSoeSupport)
    {/* EtherCAT slaves supports the Servo Profile over EtherCAT protocol */
        m_pSoEDesc = EC_NEW(ECMBSLAVE_SOE_DESC);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pSoEDesc", m_pSoEDesc, sizeof(ECMBSLAVE_SOE_DESC));

        m_wSupportedMbxProtocols |= EC_MBX_PROTOCOL_SOE;
        m_byNumMbxProtocol++;
        m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid = EC_FALSE;
        m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry = EC_FALSE;

        if (m_pSoEDesc != EC_NULL)
        {
            OsMemset(m_pSoEDesc, 0, sizeof(ECMBSLAVE_SOE_DESC));
#ifdef DEBUGTRACE
            m_pSoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_SOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendSoeCmds", EC_MEMTRACE_CORE_SLAVE));
#else
            m_pSoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_SOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendSoeCmds"));
#endif
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pSoEDesc->pPendMailboxCmdFifo", m_pSoEDesc->pPendMailboxCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
            if (m_pSoEDesc->pPendMailboxCmdFifo == EC_NULL)
            {
                FreeMbxProtocolDesc(&m_pSoEDesc, GetMemLog());
            }
        }
    }
#endif
#ifdef INCLUDE_VOE_SUPPORT
    m_pVoEDesc        = EC_NULL;
    m_pvVoeMbxTferObj = EC_NULL;
    if (pEniSlave->sParm.bVoeSupport)
    {/* EtherCAT slaves supports the Vendor over EtherCAT protocol */

        EC_T_MBXTFER_DESC oVoeMbxTferDesc;
        EC_T_MBXTFERP* poVoeMbxTferObj   = EC_NULL;

        m_pVoEDesc = EC_NEW(ECMBSLAVE_VOE_DESC);
        m_wSupportedMbxProtocols |= EC_MBX_PROTOCOL_VOE;
        m_byNumMbxProtocol++;
        m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid = EC_FALSE;
        m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry = EC_FALSE;
        
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pVoEDesc", m_pVoEDesc, sizeof(ECMBSLAVE_VOE_DESC));
        if (m_pVoEDesc != EC_NULL)
        {
            OsMemset(m_pVoEDesc, 0, sizeof(ECMBSLAVE_VOE_DESC));
#ifdef DEBUGTRACE
            m_pVoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_VOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendVoeCmds", EC_MEMTRACE_CORE_SLAVE));
#else
            m_pVoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_VOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendVoeCmds"));
#endif
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::CEcMbSlave", m_pVoEDesc->pPendMailboxCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
            if (m_pVoEDesc->pPendMailboxCmdFifo == EC_NULL)
            {
                FreeMbxProtocolDesc(&m_pVoEDesc, GetMemLog());
            }
        }

        /* create a voe mailbox transfer object which are responable for the incoming VoE data from the slave */
        /* initialize the VoE mailbox transfer descriptor */
        oVoeMbxTferDesc.dwMaxDataLen = m_mbxOLen[m_sMboxFlags.mbxIdx];
        oVoeMbxTferDesc.pbyMbxTferDescData  = (EC_T_BYTE*)OsMalloc(oVoeMbxTferDesc.dwMaxDataLen);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::oMbxTferDesc.pbyMbxTferDescData", oVoeMbxTferDesc.pbyMbxTferDescData, oVoeMbxTferDesc.dwMaxDataLen);

        /* check if memory allocation succeeded */
        if (EC_NULL != oVoeMbxTferDesc.pbyMbxTferDescData )
        {
            /* clear voe mailbox data buffer */
            OsMemset(oVoeMbxTferDesc.pbyMbxTferDescData, 0, oVoeMbxTferDesc.dwMaxDataLen);

            /* create the mailbox transfer object (the mailbox transfer descriptor will be copied */
            poVoeMbxTferObj = (EC_T_MBXTFERP*)CAtEmInterface::MbxTferCreate(&oVoeMbxTferDesc, GetMemLog());
            if (EC_NULL != poVoeMbxTferObj)
            {
                poVoeMbxTferObj->oEcMbxCmd.pvContext         = poVoeMbxTferObj;
                poVoeMbxTferObj->oEcMbxCmd.dwInvokeId        = poVoeMbxTferObj->MbxTfer.dwTferId;
                poVoeMbxTferObj->oEcMbxCmd.length            = poVoeMbxTferObj->MbxTfer.dwDataLen;
                poVoeMbxTferObj->oEcMbxCmd.pbyData           = poVoeMbxTferObj->MbxTfer.pbyMbxTferData;
                poVoeMbxTferObj->oEcMbxCmd.bInUseByInterface = EC_FALSE;
                poVoeMbxTferObj->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_VOE_READ;
                poVoeMbxTferObj->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
                poVoeMbxTferObj->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_VOE;
                poVoeMbxTferObj->MbxTfer.eTferStatus         = eMbxTferStatus_Idle;
                poVoeMbxTferObj->MbxTfer.dwErrorCode         = EC_E_NOERROR;
                poVoeMbxTferObj->MbxTfer.dwClntId            = 0;
                poVoeMbxTferObj->MbxTfer.dwTferId            = m_dwSlaveId; /* INVALID_SLAVE_ID, because SetSlaveId() is called after InitInstance */
                poVoeMbxTferObj->MbxTfer.eMbxTferType        = eMbxTferType_VOE_MBX_READ;
                poVoeMbxTferObj->dwSlaveId                   = 0;

                /* set the mailbox transfer object for this slave */
                m_pvVoeMbxTferObj = (EC_T_VOID*)poVoeMbxTferObj;
            }
            else
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::CEcMbSlave", oVoeMbxTferDesc.pbyMbxTferDescData, oVoeMbxTferDesc.dwMaxDataLen);
                SafeOsFree(oVoeMbxTferDesc.pbyMbxTferDescData);
                m_pvVoeMbxTferObj = EC_NULL;
            }
        }
        else 
        {
            /* mailbox transfer object could not be allocated */     
            m_pvVoeMbxTferObj  = EC_NULL;
        } 
    }
#endif
#ifdef INCLUDE_AOE_SUPPORT
    m_dwAoeInvokeId = 0;
    m_pAoEDesc      = EC_NULL;
    if (pEniSlave->sParm.bAoeSupport)
    {/* EtherCAT slaves supports the ADS over EtherCAT protocol */
        m_pAoEDesc = EC_NEW(ECMBSLAVE_AOE_DESC);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pAoEDesc", m_pAoEDesc, sizeof(ECMBSLAVE_AOE_DESC));

        m_wSupportedMbxProtocols |= EC_MBX_PROTOCOL_AOE;
        m_byNumMbxProtocol++;

        if (m_pAoEDesc != EC_NULL)
        {
            OsMemset(m_pAoEDesc, 0, sizeof(ECMBSLAVE_AOE_DESC));
            OsMemcpy(&m_pAoEDesc->oAoeNetId, pEniSlave->abyAoeNetId, sizeof(EC_T_AOE_NETID));
            m_pAoEDesc->dwAoeInvokeId = 1;
#ifdef DEBUGTRACE
            m_pAoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_AOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendAoeCmds", EC_MEMTRACE_CORE_SLAVE));
#else
            m_pAoEDesc->pPendMailboxCmdFifo = EC_NEW(CFiFoListDyn<PEcMailboxCmd>(MAX_QUEUED_AOE_CMDS, m_poSlaveLock, (EC_T_CHAR*)"PendAoeCmds"));
#endif
            EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pAoEDesc->pPendMailboxCmdFifo", m_pAoEDesc->pPendMailboxCmdFifo, sizeof(CFiFoListDyn<PEcMailboxCmd>));
            if (m_pAoEDesc->pPendMailboxCmdFifo == EC_NULL)
            {
                FreeMbxProtocolDesc(&m_pAoEDesc, GetMemLog());
            }
        }
    }
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    {
        m_pRawMbxDesc = (ECMBSLAVE_RAWMBX_DESC*)CreateMbxProtoDesc(MAX_QUEUED_RAWMBX_CMDS);

        m_pRawMbxDesc->dwClientId = 0;
    }
#endif /* INCLUDE_RAWMBX_SUPPORT */
         
    if (m_sMboxFlags.bLogicalStateMBoxPolling )
    {
        /* inform master that the mailbox state should be checked cyclically */
        m_pMaster->RegisterLogicalStateMBoxPolling(this, m_wSlaveLogicalAddressMBoxState, EC_FALSE);
    }
    /* MbxInitCmds */
    m_wMbxInitCmdsDataOffset = 0;
    m_wMbxInitCmdsCurrent    = INITCMD_INACTIVE;
    m_wCoeInitCmdsCurrent    = INITCMD_INACTIVE;
    m_bStateMachineStable    = EC_TRUE;
    m_ppMbxInitCmds          = EC_NULL;
    m_pCoeInitCmds           = EC_NULL;
    m_ppCoeInitCmds          = EC_NULL;
    m_wCoeInitCmdsCount      = 0;
    m_dwCoeInitCmdsSize      = 0;

    /* get total number mailbox initcmds */
    m_wMbxInitCmdsCount = (EC_T_WORD)(pEniSlave->wNumCoeInitCmds + pEniSlave->wNumEoeInitCmds);
#if (defined INCLUDE_AOE_SUPPORT)
    m_wMbxInitCmdsCount = (EC_T_WORD)(m_wMbxInitCmdsCount + pEniSlave->wNumAoeInitCmds);
#endif
#if (defined INCLUDE_SOE_SUPPORT)
    m_wMbxInitCmdsCount = (EC_T_WORD)(m_wMbxInitCmdsCount + pEniSlave->wNumSoeInitCmds);
#endif

    if (0 == m_wMbxInitCmdsCount)
    {
        m_ppMbxInitCmds = EC_NULL;
    }
    else
    {
        m_ppMbxInitCmds = EC_NEW(PEcMailboxCmdDesc[m_wMbxInitCmdsCount+1]); /* one more to avoid index overflow, see also CEcMbSlave::StartInitCmds() */
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_ppMbxInitCmds", m_ppMbxInitCmds, (m_wMbxInitCmdsCount+1) * sizeof(PEcMailboxCmdDesc));
    }
    if (m_ppMbxInitCmds != EC_NULL)
    {
        nAllIndex = 0;
        /* copy CoE initcmds */
        for(dwCmdIndex = 0; dwCmdIndex < pEniSlave->wNumCoeInitCmds; dwCmdIndex++)
        {
            m_ppMbxInitCmds[nAllIndex] = pEniSlave->apPkdCoeInitCmdDesc[dwCmdIndex];
            nAllIndex++;
        }
        /* copy EoE initcmds */
        for(dwCmdIndex = 0; dwCmdIndex < pEniSlave->wNumEoeInitCmds; dwCmdIndex++)
        {
            m_ppMbxInitCmds[nAllIndex] = pEniSlave->apPkdEoeInitCmdDesc[dwCmdIndex];
            nAllIndex++;
        }
#if (defined INCLUDE_AOE_SUPPORT)
        /* copy AoE initcmds */
        for(dwCmdIndex = 0; dwCmdIndex < pEniSlave->wNumAoeInitCmds; dwCmdIndex++)
        {
            m_ppMbxInitCmds[nAllIndex] = pEniSlave->apPkdAoeInitCmdDesc[dwCmdIndex];
            nAllIndex++;
        }
#endif
#if (defined INCLUDE_SOE_SUPPORT)
        /* copy SoE initcmds */
        for(dwCmdIndex = 0; dwCmdIndex < pEniSlave->wNumSoeInitCmds; dwCmdIndex++)
        {
            m_ppMbxInitCmds[nAllIndex] = pEniSlave->apPkdSoeInitCmdDesc[dwCmdIndex];
            nAllIndex++;
        }
#endif
        OsDbgAssert(m_wMbxInitCmdsCount == nAllIndex);
    }
    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::CEcMbSlave() m_wMbxInitCmdsCount = %d this = 0x%x\n", m_wMbxInitCmdsCount, this));
}

/********************************************************************************/
/** \brief CEcMbSlave destructor
*
* \return
*/
CEcMbSlave::~CEcMbSlave()
{
PEcMailboxCmd pCmd = EC_NULL;

    if (m_sMboxFlags.bLogicalStateMBoxPolling )
        m_pMaster->RegisterLogicalStateMBoxPolling(this, m_wSlaveLogicalAddressMBoxState, EC_TRUE);

    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_ppCoeInitCmds()", m_ppCoeInitCmds, m_wCoeInitCmdsCount * sizeof(PEcMailboxCmdDesc*));
    SafeOsFree(m_ppCoeInitCmds);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pCoeInitCmds()", m_pCoeInitCmds, m_dwCoeInitCmdsSize);
    SafeOsFree(m_pCoeInitCmds);
    if (m_ppMbxInitCmds != EC_NULL)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::~m_ppMbxInitCmds", m_ppMbxInitCmds, (m_wMbxInitCmdsCount+1) * sizeof(PEcMailboxCmdDesc));
        SafeDeleteArray(m_ppMbxInitCmds);
    }

#if (defined INCLUDE_FOE_SUPPORT)
    if (m_pFoEDesc != EC_NULL)
    {        
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
        {
            OsDbgAssert(!m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bInUseByInterface);
        }
        ClearCurrPendFoeMbxCmdDesc();
        while (m_pFoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd));
    
        FreeMbxProtocolDesc(&m_pFoEDesc, GetMemLog());
    }
#endif /* INCLUDE_FOE_SUPPORT */
#if (defined INCLUDE_EOE_SUPPORT)
    if (m_pEoEDesc != EC_NULL)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::~CEcMbSlave", m_pEoEDesc->pPort, sizeof(CEthernetPort));
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcSlave::~CEcMbSlave", m_pEoEDesc, sizeof(ECMBSLAVE_EOE_DESC));
        SafeDelete(m_pEoEDesc->pPort);
        SafeDelete(m_pEoEDesc);
    }
#endif /* INCLUDE_EOE_SUPPORT */
    if (m_pCoEDesc != EC_NULL)
    {   
#if (defined DEBUG)
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid )
        {
            OsDbgAssert(!m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bInUseByInterface );
        }
#endif
        ClearCurrPendCoeMbxCmdDesc();
        while ( m_pCoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd));
        
        FreeMbxProtocolDesc(&m_pCoEDesc, GetMemLog());
    }   
#if (defined INCLUDE_SOE_SUPPORT)
    if (m_pSoEDesc != EC_NULL)
    {   
        if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid )
        {
            OsDbgAssert(!m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bInUseByInterface );
        }
        ClearCurrPendSoeMbxCmdDesc();
/* rkl AutoRelease and MailboxFree not yet defined, commented out
 *
        while ( m_pSoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd))
        {
            if (pCmd->bAutoRelease )
            {
                m_pMaster->MailboxFree(pCmd);
            }
            OsDbgAssert(pCmd->bAutoRelease);    / * all non-auto-release mailbox commands should have been sent! * /
        }
*/
        FreeMbxProtocolDesc(&m_pSoEDesc, GetMemLog());
    }   
#endif /* INCLUDE_SOE_SUPPORT */
#if (defined INCLUDE_VOE_SUPPORT)
    if (m_pVoEDesc != EC_NULL)
    {       
        EC_T_MBXTFERP* poVoeMbxTferObj = EC_NULL;

        if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid )
        {
            OsDbgAssert(!m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bInUseByInterface );
        }
        ClearCurrPendVoeMbxCmdDesc();
        while ( m_pVoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd));
        
        FreeMbxProtocolDesc(&m_pVoEDesc, GetMemLog());
       
        if (m_pvVoeMbxTferObj != EC_NULL)
        {
            poVoeMbxTferObj = (EC_T_MBXTFERP*)(m_pvVoeMbxTferObj);

            while ( poVoeMbxTferObj->MbxTfer.eTferStatus != eMbxTferStatus_Idle 
                &&  poVoeMbxTferObj->MbxTfer.eTferStatus != eMbxTferStatus_TferDone)
            {
                /* waiting for master to return the mailbox transfer object */
                EC_DBGMSG( "ecatDeinitMasterEx(): Waiting for master to return the VoE notification mailbox transfer object...\n" );
                OsSleep(1);
            }

            /* the transfer status is possibly in state eMbxTferStatus_TferDone (that means there is a VoE message from the slave available)
               but we discard the received message to free the VoE mailbox transfer object. */
            poVoeMbxTferObj->MbxTfer.eTferStatus = eMbxTferStatus_Idle;
            
            /*  release the mailbox data buffer first */
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::~CEcMbSlave()", poVoeMbxTferObj->MbxTfer.pbyMbxTferData, poVoeMbxTferObj->MbxTfer.dwDataLen);
            SafeOsFree(poVoeMbxTferObj->MbxTfer.pbyMbxTferData);
           
            /* now release the VoE mailbox transfer object */
            CAtEmInterface::MbxTferDelete((EC_T_MBXTFER*)poVoeMbxTferObj, GetMemLog());
            poVoeMbxTferObj = EC_NULL;
        }
        
        m_pvVoeMbxTferObj = EC_NULL;
    }
#endif /* INCLUDE_VOE_SUPPORT */
#if (defined INCLUDE_AOE_SUPPORT)
    if (m_pAoEDesc != EC_NULL)
    {        
        if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
        {
            OsDbgAssert(!m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bInUseByInterface);
        }
        ClearCurrPendAoeMbxCmdDesc();
        while ( m_pAoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd));
    
        FreeMbxProtocolDesc(&m_pAoEDesc, GetMemLog());
    }
#endif /* INCLUDE_FOE_SUPPORT */
#if (defined INCLUDE_RAWMBX_SUPPORT)
    if (EC_NULL != m_pRawMbxDesc)
    {
        DeleteMbxProtoDesc(&m_pRawMbxDesc->mbxDesc);
        m_pRawMbxDesc = EC_NULL;
    }
#endif /* INCLUDE_RAWMBX_SUPPORT */
}

/********************************************************************************/
/** \brief Reset the state machine of the EtherCAT slave.
*
* \return
*/
EC_T_VOID CEcMbSlave::ResetSlaveStateMachine(EC_T_VOID)
{   
    m_wMbxInitCmdsCurrent = INITCMD_INACTIVE;
    m_wCoeInitCmdsCurrent = INITCMD_INACTIVE;
    CEcSlave::ResetSlaveStateMachine();
}

/********************************************************************************/
/** \brief Stabilize the state machine of the EtherCAT master.
*
* \return
*/
EC_T_VOID CEcMbSlave::StabilizeSlaveStateMachine(EC_T_VOID)
{
    m_wMbxInitCmdsCurrent = INITCMD_INACTIVE;
    m_wCoeInitCmdsCurrent = INITCMD_INACTIVE;
    CEcSlave::StabilizeSlaveStateMachine();
}

/********************************************************************************/
/** \brief Cancel the state machine of the EtherCAT slave.
*
* \return
*/
EC_T_VOID CEcMbSlave::CancelSlaveStateMachine(EC_T_VOID)
{
    CEcSlave::CancelSlaveStateMachine();
    if (AreInitCmdsActive())
    {
        /* set trigger */
        CEcSlave::m_bCancelStateMachine = EC_TRUE;
    }
}

/********************************************************************************/
/** \brief Executes the state machine of the EtherCAT slave.
*
* \return
*/
EC_T_DWORD  CEcMbSlave::SlaveStateMachine()
{
EC_T_DWORD dwRes = CEcSlave::SlaveStateMachine();

	if (AreAllMbxInitCmdsProcessed())
	{
		switch (m_wCurrStateMachDevState)
		{
		case DEVICE_STATE_INIT:
			/* reset MBX data link layer */
			OsMemset(&m_mbxLayer, 0, sizeof(m_mbxLayer));
			m_bMbxReadStateIsBusy = EC_FALSE;
			m_sMboxFlags.mbxIdx = REGULAR_MAILBOX;
			m_bStandardInitCmdsDone = EC_FALSE;
			break;
		case DEVICE_STATE_BOOTSTRAP:
			m_sMboxFlags.mbxIdx = BOOT_MAILBOX;
			break;
		default:
			m_sMboxFlags.mbxIdx = REGULAR_MAILBOX;
		}
	}    
    return dwRes;
}

/********************************************************************************/
/** \brief Add CoE init commands dynamically
*
* \return
*/
EC_T_DWORD CEcMbSlave::AddCoeInitCmds(EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmds, EC_T_WORD wCount)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_WORD  wIdx     = 0;

    /* check state */
    if (AreInitCmdsActive())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* free previously allocated memory memory */
    if (EC_NULL != m_pCoeInitCmds)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pCoeInitCmds()", m_pCoeInitCmds, m_dwCoeInitCmdsSize);
        SafeOsFree(m_pCoeInitCmds);
    }
    if (EC_NULL != m_ppCoeInitCmds)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_ppCoeInitCmds()", m_ppCoeInitCmds, m_wCoeInitCmdsCount * sizeof(PEcMailboxCmdDesc*));
        SafeOsFree(m_ppCoeInitCmds);
    }
    /* allocate CoeInitCmds */
    {
    EC_T_INT nCoeInitCmdsSize = 0;

        for (wIdx = 0; wIdx < wCount; wIdx++)
        {
        EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmd = &pbCoeInitCmds[wIdx];

            nCoeInitCmdsSize += sizeof(EcMailboxCmdDesc);
            nCoeInitCmdsSize += (EC_T_INT)pbCoeInitCmd->wDataLen;
            nCoeInitCmdsSize += (EC_T_INT)(OsStrlen(pbCoeInitCmd->szComment) + 1);
        }
        m_pCoeInitCmds = (PEcMailboxCmdDesc)OsMalloc(nCoeInitCmdsSize);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_pCoeInitCmds", m_pCoeInitCmds, nCoeInitCmdsSize);
        if (EC_NULL == m_pCoeInitCmds)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(m_pCoeInitCmds, 0, nCoeInitCmdsSize);
        m_dwCoeInitCmdsSize = nCoeInitCmdsSize;
        m_wCoeInitCmdsCount = wCount;
    }
    m_ppCoeInitCmds = (PEcMailboxCmdDesc*)OsMalloc(m_wCoeInitCmdsCount * sizeof(PEcMailboxCmdDesc*));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_SLAVE, "CEcMbSlave::m_ppCoeInitCmds", m_ppCoeInitCmds, (m_wCoeInitCmdsCount * sizeof(PEcMailboxCmdDesc*)));
    if (EC_NULL == m_ppCoeInitCmds)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* fill CoeInitCmds */
    {
    EC_T_BYTE* pbyCur = (EC_T_BYTE*)m_pCoeInitCmds;

        for (wIdx = 0; wIdx < m_wCoeInitCmdsCount; wIdx++)
        {
        PEcMailboxCmdDesc pEcMbxCmdDesc = (PEcMailboxCmdDesc)pbyCur;
        EC_T_ADD_COE_INITCMD_DESC_ENTRY* pbCoeInitCmd = &pbCoeInitCmds[wIdx];
        EC_T_WORD wCmtLen = (EC_T_WORD)(OsStrlen(pbCoeInitCmd->szComment) + 1);

            EC_ECMBXCMDDESC_SET_HANDLE(pEcMbxCmdDesc, pbCoeInitCmd->dwHandle);
            EC_ECMBXCMDDESC_SET_IGNOREFAILURE(pEcMbxCmdDesc, pbCoeInitCmd->bIgnoreFailure);
            EC_ECMBXCMDDESC_SET_FIXED(pEcMbxCmdDesc, EC_FALSE);
            EC_ECMBXCMDDESC_SET_TRANSITION(pEcMbxCmdDesc, pbCoeInitCmd->wTransition);
            EC_ECMBXCMDDESC_SET_MBXTIMEOUT(pEcMbxCmdDesc, (EC_T_WORD)pbCoeInitCmd->wTimeout );
            EC_ECMBXCMDDESC_SET_PROTOCOL(pEcMbxCmdDesc, ETHERCAT_MBOX_TYPE_CANOPEN);

            /* fill SDO header */
            EC_ECSDOHDR_SET_INDEX(&(pEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader), pbCoeInitCmd->wIndex);
            pEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex = pbCoeInitCmd->bySubIndex;
            pEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Ccs = pbCoeInitCmd->byCcs;
            pEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Complete = (EC_T_WORD)pbCoeInitCmd->bCompleteAccess;
            EC_ECSDOHDR_SET_SDODATA(&(pEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader), pbCoeInitCmd->wDataLen);

            /* copy data */
            EC_ECMBXCMDDESC_SET_DATALEN(pEcMbxCmdDesc, (sizeof(pEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader) + pbCoeInitCmd->wDataLen));
            if (0 != pbCoeInitCmd->wDataLen)
            {
                OsMemcpy(&pEcMbxCmdDesc->uMbxHdr.coe.data, pbCoeInitCmd->pbyData, pbCoeInitCmd->wDataLen);
            }
            /* copy InitCmd comment */
            EC_ECMBXCMDDESC_SET_CMTLEN(pEcMbxCmdDesc, wCmtLen);
            if (0 != wCmtLen)
            {
                OsStrcpy(EcMailboxCmdDescComment(pEcMbxCmdDesc), pbCoeInitCmd->szComment);
            }
            pbyCur = pbyCur + SIZEOF_EcMailboxCmdDesc(pEcMbxCmdDesc);

            /* store pointer InitCmd */
            m_ppCoeInitCmds[wIdx] = pEcMbxCmdDesc;
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief Processes init commands that should be sent in this transition.
*
* \return
*/
template<typename T> 
T* FindNextInitCmd(T** ppMbxCmdList, EC_T_WORD* pwCurIdx, EC_T_WORD wCount, EC_T_WORD wTransition)
{
    T* pRetVal = EC_NULL;
    EC_T_WORD wCurIdx = *pwCurIdx;

    if (INITCMD_INACTIVE == wCurIdx)
    {
        wCurIdx = 0;
    }
    else
    {
        wCurIdx++;
        if (wCurIdx >= wCount)
        {
            wCurIdx = wCount;
        }
    }
    for (; wCurIdx < wCount; wCurIdx++)
    {
        if (GetTransition(ppMbxCmdList[wCurIdx]) & wTransition)
        {
            pRetVal = ppMbxCmdList[wCurIdx];
            break;
        }
    }
    *pwCurIdx = wCurIdx;
    return pRetVal;
}
/********************************************************************************/
/** \brief Processes init commands that should be sent in this transition.
*
* \return EC_TRUE: All InitCmds done
*/
EC_T_BOOL   CEcMbSlave::StartInitCmds(EC_T_WORD wTransition, EC_T_BOOL bRetry)
{
EC_T_DWORD        dwRes            = EC_E_ERROR;
PEcMailboxCmdDesc pMbxCmdDesc      = EC_NULL;
EC_T_BOOL         bAllInitCmdsDone = EC_FALSE;

    /* start mailbox init commands, if no other mailbox transfer are pending */
    if (((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_SOE_SUPPORT
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_VOE_SUPPORT
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_AOE_SUPPORT
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bIsValid)
#endif
       )
    {
        goto Exit;
    }
    /* cancel state machine? */
    if (m_bCancelStateMachine || !IsPresentOnBus())
    {
        /* stop mailbox init commands */
        StopInitCmds(wTransition, wTransition);

        /* call start mailbox init commands, which will handle m_bCancelStateMachine and stop init commands */
        bAllInitCmdsDone = CEcSlave::StartInitCmds(wTransition, EC_FALSE);
        goto Exit;
    }
    /* if below PREOP perform "Standard" InitCmds first */
    if ((m_wCurrStateMachDevState < DEVICE_STATE_PREOP) && (!m_bStandardInitCmdsDone))
    {
        m_bStandardInitCmdsFirst = EC_TRUE;
        
        /* perform "Standard" InitCmds */
        m_bStandardInitCmdsDone = CEcSlave::StartInitCmds(wTransition, bRetry);
        if (!m_bStandardInitCmdsDone)
        {
            goto Exit;
        }
    }

    if (!bRetry)
    {
        pMbxCmdDesc = FindNextInitCmd(m_ppMbxInitCmds, &m_wMbxInitCmdsCurrent, m_wMbxInitCmdsCount, wTransition);
        if (EC_NULL == pMbxCmdDesc)
        {
            for (;;)
            {
            EC_T_BOOL bSendInitCmd = EC_TRUE;

                pMbxCmdDesc = FindNextInitCmd(m_ppCoeInitCmds, &m_wCoeInitCmdsCurrent, m_wCoeInitCmdsCount, wTransition);
                if (EC_NULL == pMbxCmdDesc)
                {
                    break;
                }
                /* call registered callback to eventually modify data or skip InitCmd */
                if (EC_NULL != m_pMaster->m_pfnSlaveCoeInitCmdCallback)
                {
                bSendInitCmd = m_pMaster->m_pfnSlaveCoeInitCmdCallback(m_pMaster->m_pvSlaveCoeInitCmdCallbackParm,
                                    GetSlaveId(), EC_ECMBXCMDDESC_GET_HANDLE(pMbxCmdDesc),
                                    EC_ECSDOHDR_GET_INDEX(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)), pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex,
                                    EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)), &pMbxCmdDesc->uMbxHdr.coe.data[0]);
                }
                if (bSendInitCmd)
                {
                    break;
                }
            }
        }
    }
    else
    {
        pMbxCmdDesc = GetMbxInitCmdsCurrent();
        OsDbgAssert(EC_NULL != pMbxCmdDesc);
    }
    if (EC_NULL != pMbxCmdDesc)
    {
        /* found a MbxInitCmd for this transition */
        /* EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::StartInitCmds() - %s: send MBX init command (%d<%d)\n", m_pEniSlave->szName, m_wMbxInitCmdsCurrent, m_wMbxInitCmdsCount)); */

        switch (EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc))
        {
        case ETHERCAT_MBOX_TYPE_CANOPEN:
            {
            EC_SDO_HDR EcSdoHeader;

                /* this is the amount of data that can be sent/received */
                EC_T_WORD nMaxData   = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SDO_MBOX_LEN);
                EC_T_WORD len        = GetMBoxOutCmdLength(ETHERCAT_MIN_SDO_MBOX_LEN);

                /* initialize ethercat mailbox header */
                ETHERCAT_MBOX_HEADER mbx;
                MBXHDR_CLR(&mbx);
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
                
                /* initialize CanOpen header */
                ETHERCAT_CANOPEN_HEADER can;
                OsMemset( &can, 0, sizeof(ETHERCAT_CANOPEN_HEADER));
                EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
                EC_ECCOEHDR_SET_COENUMBER(&can, 0);
                
                /* initialize Sdo header */
                SDOHDR_CLR(&EcSdoHeader);
                EC_T_VOID* pData = EC_NULL;     
                m_wMbxInitCmdsDataOffset = 0;

                /* use command description to fill Sdo header */
                EC_ECSDOHDR_SET_INDEX(&(EcSdoHeader), EC_ECSDOHDR_GET_INDEX(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));   
                EcSdoHeader.SubIndex            = pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex;
                EcSdoHeader.uHdr.Idq.Ccs        = pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Ccs;
                EcSdoHeader.uHdr.Idq.Complete   = pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Complete;
                
                switch (EcSdoHeader.uHdr.Idq.Ccs)
                {           
                case SDO_CCS_INITIATE_DOWNLOAD:                                         
                case SDO_CCS_INITIATE_UPLOAD:
                    if (EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)) > nMaxData )
                    {
                        /* the data doen't fit into one ethercat frame */
                        /* as a consequence one has to download 2 or more segments */
                        EcSdoHeader.uHdr.Idq.Expedited   = EC_FALSE;
                        EcSdoHeader.uHdr.Idq.SizeInd     = EC_TRUE;
                        EcSdoHeader.uHdr.Idq.Size        = 0;
                        EC_ECSDOHDR_SET_SDODATA(&(EcSdoHeader), EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));
                        pData                       = &pMbxCmdDesc->uMbxHdr.coe.data[0]; 
                        /* the actual amout of data without headers */
                        m_wMbxInitCmdsDataOffset         = nMaxData;
                        /* the actual amount of mailbox data to send in this ECAT-Frame including can and sdo header */
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + nMaxData));
                        /* adjust length */
                        len                         = GetMBoxOutCmdLength((EC_T_WORD)(len+nMaxData));
                    }
                    else if (EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)) > sizeof(EC_T_DWORD))
                    {
                        /* because length is greater than the maximal length in expedited mode */
                        EcSdoHeader.uHdr.Idq.Expedited   = EC_FALSE;
                        EcSdoHeader.uHdr.Idq.SizeInd     = EC_TRUE;
                        EcSdoHeader.uHdr.Idq.Size        = 0;
                        EC_ECSDOHDR_SET_SDODATA(&(EcSdoHeader), EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));
                        pData                       = &pMbxCmdDesc->uMbxHdr.coe.data[0]; 
                        m_wMbxInitCmdsDataOffset    = EC_LOWORD(EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader))));
                        len                         = GetMBoxOutCmdLength((EC_T_WORD)(len+EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader))));
                    }
                    else if (EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)) > 0 )
                    {               
                        EcSdoHeader.uHdr.Idq.Expedited   = EC_TRUE;
                        EcSdoHeader.uHdr.Idq.SizeInd     = EC_TRUE;              
                        EcSdoHeader.uHdr.Idq.Size        = (EC_T_BYTE)(sizeof(EC_T_DWORD)-EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));
                        OsMemcpy( EC_ECSDOHDR_SDODATA(&EcSdoHeader),   /* &EcSdoHeader.dwSdoData (avoid diab compiler warning)*/
                                  &pMbxCmdDesc->uMbxHdr.coe.data[0], 
                                  EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));
                        m_wMbxInitCmdsDataOffset = EC_LOWORD(EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)));
                    }
                    else
                    {   
                        EcSdoHeader.uHdr.Idq.Expedited   = EC_TRUE;
                        EcSdoHeader.uHdr.Idq.SizeInd     = EC_FALSE;
                        EcSdoHeader.uHdr.Idq.Size        = 0;
                        m_wMbxInitCmdsDataOffset         = 0;
                    }
                    /* initialize retry counter */
                    if (!bRetry)
                    {
                        m_wInitCmdsRetries = (EC_T_WORD)EC_MAX(m_wRetryAccessCount, EC_ECMBXCMDDESC_GET_RETRIES(pMbxCmdDesc));
                    }
                    /* send command to the slave */
                    dwRes = EcatMbxSendCmdReq(EC_TRUE, pData, &mbx, &can, &EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                ,EC_NULL
                                ,EC_NULL
#endif
                                );
                    if (EC_E_NOERROR != dwRes)
                    {
                        if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                        {
                            /* start retry timeout to retry later. Don't decrement retry counter, because InitCmd was not sent */
                            m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        }
                    }
                    else
                    {
                    EC_T_WORD wInitCmdTimeout = 0;

                        wInitCmdTimeout = EC_ECMBXCMDDESC_GET_MBXTIMEOUT(pMbxCmdDesc);
                        if (wInitCmdTimeout == 0)
                        {
                            wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                        }
                        m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
                        m_oInitCmdRetryTimeout.Stop();

                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("<====[%06d] CEcMbSlave::StartInitCmds() QuCmdMbx[%d-%d]: %s: %s - %s: id=0x%x, subid=%d\n", 
                                  OsQueryMsecCount(), (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                                  GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                                  StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)),
                                  CoESdoCcsText(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Ccs), (EC_T_INT)EC_ECSDOHDR_GET_INDEX(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)), pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex));
                    }
                    break;

                default:
                    m_pMaster->NotifySlaveInitCmdResponseError(this, m_wActTransition, eInitCmdErr_FAILED, 
                        EcMailboxCmdDescComment(m_ppMbxInitCmds[m_wMbxInitCmdsCurrent]), EC_ECMBXCMDDESC_GET_CMTLEN(m_ppMbxInitCmds[m_wMbxInitCmdsCurrent]));

                    EC_ERRORMSGC_L( ("CEcMbSlave::StartInitCmds(): CoE CCS 0x%x not supported for INIT Commands!\n",(EC_T_DWORD)EcSdoHeader.uHdr.Idq.Ccs));
                    OsDbgAssert(EC_FALSE);
                    break;      
                }
            }
            break;
#ifdef INCLUDE_SOE_SUPPORT
        case ETHERCAT_MBOX_TYPE_SOE:
            {
                /* this is the amount of data that can be sent/received */
                EC_T_WORD               nMaxData    = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SOE_MBOX_LEN);
                EC_T_WORD               len         = GetMBoxOutCmdLength(ETHERCAT_MIN_SOE_MBOX_LEN);
                EC_T_BYTE               byMbxCnt    = 0;
                ETHERCAT_MBOX_HEADER    mbx;
                EC_SOE_HDR              EcSoeHeader;
                EC_T_VOID*              pData       = EC_NULL;

                /* initialize ethercat mailbox header */
                MBXHDR_CLR(&mbx);
                byMbxCnt = IncCntMbxSend();
                EC_ECMBOXHDR_SET_COUNTER(&mbx, byMbxCnt);
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_SOE_HDR_LEN));
                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_SOE);
                
                /* initialize SoE header */
                OsMemset( &EcSoeHeader, 0, sizeof(EC_SOE_HDR));
                m_wMbxInitCmdsDataOffset = 0;

                /* use command description to fill Soe header */
                EcSoeHeader.OpCode = pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader.OpCode;
                EcSoeHeader.DriveNo = pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader.DriveNo;
                EcSoeHeader.byElements = pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader.byElements;
                EC_ECSOEHDR_SET_IDN(&(EcSoeHeader), EC_ECSOEHDR_GET_IDN(&(pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader)));   
                
                switch (EcSoeHeader.OpCode)
                {           
                case ECAT_SOE_OPCODE_WRQ:                                         
                    EC_T_DWORD dwDataLen; 
                    EC_T_INT nFragLeft;
                    dwDataLen = EC_ECMBXCMDDESC_GET_DATALEN(pMbxCmdDesc) - sizeof(EC_SOE_HDR);
                    if (dwDataLen > nMaxData )
                    {
                        /* the data doesn't fit into the mailbox */
                        /* as a consequence one has to write 2 or more fragments */
                        EcSoeHeader.Incomplete = EC_TRUE;
                        /* the actual amount of mailbox data to send in this ECAT-Frame including header */
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + nMaxData));
                        /* number of fragments left */
                        nFragLeft = (((EC_T_INT)dwDataLen + nMaxData - 1) / nMaxData) - 1;
                        EC_ECSOEHDR_SET_FRAGLEFT( &EcSoeHeader, (EC_T_WORD)nFragLeft );
                        /* the actual amount of data without headers */
                        m_wMbxInitCmdsDataOffset         = nMaxData;
                        /* adjust length */
                        len = GetMBoxOutCmdLength((EC_T_WORD)(len+nMaxData));
                    }
                    else
                    {
                        /* data fits into mailbox */
                        EcSoeHeader.Incomplete = EC_FALSE;
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + dwDataLen));
                        /* the actual amount of data without headers */
                        m_wMbxInitCmdsDataOffset         = (EC_T_WORD)dwDataLen;
                        /* adjust length */
                        len = GetMBoxOutCmdLength((EC_T_WORD)(len+dwDataLen));
                    }
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("<====[%06d] CEcMbSlave::StartInitCmds() QuCmdMbx[%d-%d]: %s: %s - %s: IDN=0x%x\n", 
                        OsQueryMsecCount(), (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                        GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                        StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)),
                        SoEOpCodeText(EcSoeHeader.OpCode), (EC_T_INT)EC_ECSOEHDR_GET_IDN(&(pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader))));
                    
                    pData = &pMbxCmdDesc->uMbxHdr.soe.data[0]; /* just pass the pointer to the data */

                    /* initialize retry counter */
                    if (!bRetry)
                    {
                        m_wInitCmdsRetries = (EC_T_WORD)EC_MAX(m_wRetryAccessCount, EC_ECMBXCMDDESC_GET_RETRIES(pMbxCmdDesc));
                    }
                    dwRes = m_pMaster->QueueEcatCmdReq(EC_FALSE, this, eMbxInvokeId_INITCMD_SEND_TO_SLAVE, EC_CMD_TYPE_FPWR, m_wFixedAddr, m_mbxOStart[m_sMboxFlags.mbxIdx], len,
                                pData, 0, &mbx, EC_NULL, EC_NULL, EC_NULL, EC_NULL, &EcSoeHeader);
                    if (EC_E_NOERROR != dwRes)
                    {
                        if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                        {
                            /* start retry timeout to retry later. Don't decrement retry counter, because InitCmd was not sent */
                            m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        }
                    }
                    else
                    {
                    EC_T_WORD wInitCmdTimeout = 0;

                        wInitCmdTimeout = EC_ECMBXCMDDESC_GET_MBXTIMEOUT(pMbxCmdDesc);
                        if (wInitCmdTimeout == 0)
                        {
                            wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                        }
                        m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
                        m_oInitCmdRetryTimeout.Stop();

                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("<====[%06d] CEcMbSlave::StartInitCmds() QuCmdMbx[%d-%d]: %s: %s - %s: IDN=0x%x\n", 
                            OsQueryMsecCount(), (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                            GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                            StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)),
                            SoEOpCodeText(EcSoeHeader.OpCode), (EC_T_INT)EC_ECSOEHDR_GET_IDN(&(pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader))));
                    }
                    break;
                case ECAT_SOE_OPCODE_RRQ:
                    EC_ERRORMSGC_L( ("CEcMbSlave::StartInitCmds(): SOE READ REQUEST not supported for INIT Commands!\n"));
                    OsDbgAssert(EC_FALSE);
                    break;      
                default:
                    EC_ERRORMSGC_L( ("CEcMbSlave::StartInitCmds(): SoE OpCode 0x%x not supported for INIT Commands!\n",(EC_T_DWORD)EcSoeHeader.OpCode));
                    OsDbgAssert(EC_FALSE);
                    break;      
                }
            }
            break;
#endif
#ifdef INCLUDE_AOE_SUPPORT
        case ETHERCAT_MBOX_TYPE_ADS:
            {
            EC_AOE_HDR EcAoeHeader;

                /* this is the amount of data that can be sent/received */
                EC_T_WORD               nMaxData    = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_AOE_MBOX_LEN);
                EC_T_WORD               len         = GetMBoxOutCmdLength(ETHERCAT_MIN_AOE_MBOX_LEN);
                EC_T_BYTE               byMbxCnt    = 0;
                ETHERCAT_MBOX_HEADER    mbx;
                EC_AOE_HDR*             pEcAoeHeader;
                EC_T_VOID*              pData       = EC_NULL;     

                /* initialize ethercat mailbox header */
                MBXHDR_CLR(&mbx);
                byMbxCnt = IncCntMbxSend();
                EC_ECMBOXHDR_SET_COUNTER(&mbx, byMbxCnt);
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_AOE_HDR_LEN));
                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_ADS);
                
                /* initialize Aoe header */
                AOEHDR_CLR(&EcAoeHeader);
                pEcAoeHeader = &pMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader;

                m_wMbxInitCmdsDataOffset = 0;

                OsMemcpy(EcAoeHeader.oTargetNetId, pEcAoeHeader->oTargetNetId, 6);
                EC_AOE_HDR_SET_TARGET_PORT(&EcAoeHeader, pEcAoeHeader->wTargetPort);
                OsMemcpy(EcAoeHeader.oSenderNetId, pEcAoeHeader->oSenderNetId, 6);
                EC_AOE_HDR_SET_SENDER_PORT(&EcAoeHeader, pEcAoeHeader->wSenderPort);
                EC_AOE_HDR_SET_CMD_ID(&EcAoeHeader, pEcAoeHeader->wCmdId);
                EC_AOE_HDR_SET_STATE_FLAGS(&EcAoeHeader, pEcAoeHeader->wStateFlags);
                EC_AOE_HDR_SET_DATA_SIZE(&EcAoeHeader, pEcAoeHeader->dwDataSize);
                EC_AOE_HDR_SET_ERROR_CODE(&EcAoeHeader, pEcAoeHeader->dwErrorCode);
                EC_AOE_HDR_SET_INVOKE_ID(&EcAoeHeader, pEcAoeHeader->dwAoeInvokeId);

                switch (pEcAoeHeader->wCmdId)
                {           
                case ECAT_AOEHDR_COMMANDID_READ:
                case ECAT_AOEHDR_COMMANDID_WRITE:
                case ECAT_AOEHDR_COMMANDID_READWRITE:
                    EC_T_DWORD dwDataLen; 
                    dwDataLen = EC_ECMBXCMDDESC_GET_DATALEN(pMbxCmdDesc) - sizeof(EC_AOE_HDR);
                    if (dwDataLen > nMaxData)
                    {
                        /* Not yet supported. Please increase the mailbox size. */
                        OsDbgAssert(EC_FALSE);
                    }
                    else
                    {
                        EcAoeHeader.dwAoeInvokeId = m_pAoEDesc->dwAoeInvokeId++;

                        /* data fits into mailbox */
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + dwDataLen));
                        /* the actual amount of data without headers */
                        m_wMbxInitCmdsDataOffset = (EC_T_WORD)dwDataLen;
                        /* adjust length */
                        len = GetMBoxOutCmdLength((EC_T_WORD)(len + dwDataLen));
                    }
                    /* send command to the slave */
                    pData = &pMbxCmdDesc->uMbxHdr.aoe.data[0];

                    /* initialize retry counter */
                    if (!bRetry)
                    {
                        m_wInitCmdsRetries = (EC_T_WORD)EC_MAX(m_wRetryAccessCount, EC_ECMBXCMDDESC_GET_RETRIES(pMbxCmdDesc));
                    }
                    dwRes = m_pMaster->QueueEcatCmdReq(EC_FALSE, this, eMbxInvokeId_INITCMD_SEND_TO_SLAVE, EC_CMD_TYPE_FPWR, m_wFixedAddr, m_mbxOStart[m_sMboxFlags.mbxIdx], len,
                                pData, 0, &mbx, EC_NULL, EC_NULL, EC_NULL
#ifdef INCLUDE_SOE_SUPPORT
                               ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                              ,EC_NULL, &EcAoeHeader
#endif
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                        {
                            /* start retry timeout to retry later. Don't decrement retry counter, because InitCmd was not sent */
                            m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        }
                    }
                    else
                    {
                    EC_T_WORD wInitCmdTimeout = 0;

                        wInitCmdTimeout = EC_ECMBXCMDDESC_GET_MBXTIMEOUT(pMbxCmdDesc);
                        if (wInitCmdTimeout == 0)
                        {
                            wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                        }
                        m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
                        m_oInitCmdRetryTimeout.Stop();

                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("<====[%06d] CEcMbSlave::StartInitCmds() QuCmdMbx[%d-%d]: %s: %s - %s\n", 
                                  OsQueryMsecCount(), (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                                  GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                                  StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc))));
                    }
                    break; 
                default:
                    EC_ERRORMSGC_L( ("CEcMbSlave::StartInitCmds(): AoE command id 0x%x not supported for INIT Commands!\n",(EC_T_DWORD)pEcAoeHeader->wCmdId));
                    OsDbgAssert(EC_FALSE);
                    break;      
                }
            }
            break;
#endif
#if (defined INCLUDE_EOE_SUPPORT)
        case ETHERCAT_MBOX_TYPE_ETHERNET:
            {
                /* initialize retry counter */
                if (!bRetry)
                {
                    m_wInitCmdsRetries = (EC_T_WORD)EC_MAX(m_wRetryAccessCount, EC_ECMBXCMDDESC_GET_RETRIES(pMbxCmdDesc));
                }
                /* start InitCmd timeout */
                EC_T_WORD wInitCmdTimeout = 0;

                wInitCmdTimeout = EC_ECMBXCMDDESC_GET_MBXTIMEOUT(pMbxCmdDesc);
                if (wInitCmdTimeout == 0)
                {
                    wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                }
                m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
                m_oInitCmdRetryTimeout.Stop();

                EC_TRACEMSG(EC_TRACE_INITCMDS, ("<====[%06d] CEcMbSlave::StartInitCmds() QuCmdMbx[%d-%d]: %s: %s - %s\n", 
                          OsQueryMsecCount(), (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                          GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                          StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc))));

                /* request send InitCmd (see SendQueuedEoECmds()) */
                m_pEoEDesc->bSendInitCmdReq = EC_TRUE;
            }
            break;
#endif /* INCLUDE_EOE_SUPPORT */
        }
    }
    else
    {
        EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::StartInitCmds() - %s: all MBX init commands sent (%d<%d)\n", m_pEniSlave->szName, m_wMbxInitCmdsCurrent, m_wMbxInitCmdsCount));

        /* "Standard" InitCmds already done ? */
        if (m_bStandardInitCmdsFirst)
        {
            /* all done */
            StopInitCmds(wTransition, wTransition);
            bAllInitCmdsDone = EC_TRUE;
            m_bStandardInitCmdsFirst = EC_FALSE;
        }
        else
        {
            bAllInitCmdsDone = CEcSlave::StartInitCmds(wTransition, bRetry);
        }
    }
Exit:
    return bAllInitCmdsDone;
}

/********************************************************************************/
/** \brief Stop init command handling.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopInitCmds
(EC_T_WORD wTransition
,EC_T_WORD wCurrTransition
)
{
EC_T_BOOL bIgnoreFailure = EC_FALSE;

    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::StopInitCmds() %s: m_wCurrStateMachDevState = %s\n", m_pEniSlave->szName, SlaveDevStateText(m_wCurrStateMachDevState)));

    if ((wTransition == ECAT_INITCMD_FAILURE))
    {
        /* generate notification */
        if (AreMbxInitCmdsPending())
        {
            bIgnoreFailure = (0 != EC_ECMBXCMDDESC_GET_IGNOREFAILURE(GetMbxInitCmdsCurrent()));
            if (!bIgnoreFailure)
            {
                m_pMaster->NotifySlaveInitCmdResponseError(this, wCurrTransition, eInitCmdErr_FAILED, 
                    EcMailboxCmdDescComment(GetMbxInitCmdsCurrent()), EC_ECMBXCMDDESC_GET_CMTLEN(GetMbxInitCmdsCurrent()));
            }            
        }
    }
    if (bIgnoreFailure)
    {
        StartInitCmds(m_wActTransition, EC_FALSE);
    }
    else
    {
        m_wMbxInitCmdsCurrent = INITCMD_INACTIVE;
        m_wCoeInitCmdsCurrent = INITCMD_INACTIVE;

        CEcSlave::StopInitCmds(wTransition, wCurrTransition);
    }
}

/********************************************************************************/
/** \brief Sends if necessary an Sdo abort command to the mailbox slave and sends an error response to the mailbox client.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopCurrCoEMbxRequest
(   EC_T_BOOL bIsInitCmd, 
    EC_T_DWORD result, 
    EC_T_DWORD abortCode, 
    EC_T_DWORD nData, 
    EC_T_VOID* pData )
{
    if (EC_NULL == m_pCoEDesc)
        return;

    /* Reset m_bCoeWKCretry flag */
    if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)
    {
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry = EC_FALSE;
    }

    if (m_mbxLayer.TOGGLED == m_mbxLayer.eState)
    {
        m_mbxLayer.oTimeout.Stop();
        m_mbxLayer.eState = m_mbxLayer.IDLE;
        m_bMbxReadStateIsBusy = EC_FALSE;
    }
    m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_FALSE;
    if (!m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
    {
        /* should never happen */
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
    m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Stop();
    if (0 == abortCode)
    {
        /* inform Mailbox client */
        if (EC_E_NOERROR != m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode)
        {
            result = m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode;
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOERROR;
        }

        MailboxRes(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, result, nData, pData);
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid = EC_FALSE;
    }
    else
    {
        ETHERCAT_MBOX_HEADER    mbx;
        ETHERCAT_CANOPEN_HEADER can;
        EC_SDO_HDR     EcSdoHeader;
        MBXHDR_CLR(&mbx);
        COEHDR_CLR(&can);
        SDOHDR_CLR(&EcSdoHeader);
        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
        EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
        EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
        EcSdoHeader.uHdr.Abt.Ccs = SDO_CCS_ABORT_TRANSFER;
        EC_ECSDOHDR_SET_SDODATA(&(EcSdoHeader), abortCode);

        /* abort SDO transfer  */
        EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, &can, &EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                         ,EC_NULL
                         ,EC_NULL
#endif
                         );
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Start(m_pMaster->GetMbxCmdTimeout(), m_pMaster->GetMsecCounterPtr());
    }
Exit:
    return;
}

#ifdef INCLUDE_VOE_SUPPORT
/********************************************************************************/
/** \brief Sends an error response to the mailbox client.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopCurrVoEMbxRequest
(   EC_T_DWORD result, 
    EC_T_DWORD nData, 
    EC_T_VOID* pData )
{
    if (m_pVoEDesc == EC_NULL)
        return;

    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED )
    {
        m_mbxLayer.oTimeout.Stop();
        m_mbxLayer.eState = m_mbxLayer.IDLE;
        m_bMbxReadStateIsBusy = EC_FALSE;
    }

    /* inform Mailbox client about error */
    OsDbgAssert(m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid);
    if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
    {
        m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Stop();
        MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, result, nData, pData);            
        ClearCurrPendVoeMbxCmdDesc();
    }
}
#endif


#ifdef INCLUDE_SOE_SUPPORT
/********************************************************************************/
/** \brief Sends an error response to the mailbox client.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopCurrSoEMbxRequest
(   EC_T_DWORD result, 
    EC_T_DWORD nData, 
    EC_T_VOID* pData )
{
    if (m_pSoEDesc == EC_NULL)
        return;

    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED )
    {
        m_mbxLayer.oTimeout.Stop();
        m_mbxLayer.eState = m_mbxLayer.IDLE;
        m_bMbxReadStateIsBusy = EC_FALSE;
    }

    /* inform Mailbox client about error */
    OsDbgAssert(m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid);
    if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
    {
        m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Stop();
        MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, result, nData, pData);            
        ClearCurrPendSoeMbxCmdDesc();
    }
}
#endif

#if (defined INCLUDE_FOE_SUPPORT)
/********************************************************************************/
/** \brief Stop FoE request.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopCurrFoEMbxRequest
(   EC_T_BOOL bIsInitCmd, 
    EC_T_DWORD result, 
    EC_T_DWORD abortCode, 
    EC_T_DWORD nData, 
    EC_T_VOID* pData )
{
    if (m_pFoEDesc == EC_NULL)
        return;
    /* Reset m_bFoeWKCretry flag */

    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry)
    {
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry = EC_FALSE;
    }

    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED)
    {
        m_mbxLayer.oTimeout.Stop();
        m_mbxLayer.eState = m_mbxLayer.IDLE;
        m_bMbxReadStateIsBusy = EC_FALSE;
    }

    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending = EC_FALSE;

    if (abortCode != 0)
    {
        ETHERCAT_MBOX_HEADER mbx;
        EC_FOE_HDR FoE;
        MBXHDR_CLR(&mbx);
        FOEHDR_CLR(&FoE);
        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);
        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + nData));
        EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_ERR);
        EC_ECFOEHDR_SET_ERRORCODE(&FoE, abortCode);
        EcatMbxSendCmdReq(bIsInitCmd, pData, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                         ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                         ,EC_NULL
                         ,EC_NULL
#endif
                         );
    }

    /* inform Mailbox client about error */
    OsDbgAssert(m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid);
    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
    {
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Stop();
        MailboxRes(m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd, result, nData, pData);            
        ClearCurrPendFoeMbxCmdDesc();
    }
}

#endif /* INCLUDE_FOE_SUPPORT */


#ifdef INCLUDE_AOE_SUPPORT
/********************************************************************************/
/** \brief Stops AoE requests.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopCurrAoEMbxRequest
(   EC_T_DWORD dwReturnCode,    
    EC_T_DWORD nData,           
    EC_T_VOID* pData )          
{
    if (m_pAoEDesc == EC_NULL)
        return;

    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED )
    {
        m_mbxLayer.oTimeout.Stop();
        m_mbxLayer.eState = m_mbxLayer.IDLE;
        m_bMbxReadStateIsBusy = EC_FALSE;
    }

    /* inform Mailbox client about error */
    OsDbgAssert(m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid);
    if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid )
    {
        m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Stop();
        MailboxRes(m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd, dwReturnCode, nData, pData);            
        ClearCurrPendAoeMbxCmdDesc();
    }
}
#endif /* #ifdef INCLUDE_AOE_SUPPORT */


#if (defined INCLUDE_EOE_SUPPORT)
/********************************************************************************/
/** \brief Send Ethernet frame to EoE slave.
*
* \return status value.
*/
EC_T_DWORD CEcMbSlave::SendFrameToSlave(EC_T_BOOL bIsInitCmd, PETHERNET_FRAME pData, EC_T_DWORD nData)
{
EC_T_DWORD dwRetVal = EC_S_UNSUCCESSFUL;
EC_T_DWORD dwRes    = EC_S_UNSUCCESSFUL;
ETHERCAT_MBOX_HEADER      mbx;
ETHERCAT_ETHERNET_HEADER  eth;
ETHERCAT_MBOX_HEADER*     pMbx = EC_NULL;
ETHERCAT_ETHERNET_HEADER* pEoe = EC_NULL;

    if ((m_pEoEDesc == EC_NULL) || (m_mbxOLen[m_sMboxFlags.mbxIdx] == 0) 
        /* see ETG.1000.6: in Bootstrap state the mailbox is active but restricted to the FoE protocol */
        || (DEVICE_STATE_BOOTSTRAP == m_wCurrStateMachDevState) || (DEVICE_STATE_BOOTSTRAP == m_wReqStateMachDevState))
    {
        dwRetVal = EC_S_UNSUCCESSFUL;
        goto Exit;
    }
    if (EC_NULL != m_pEoEDesc->pendSend.pData)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_S_UNSUCCESSFUL;
        goto Exit;
    }

    if (m_pEoEDesc->nMaxSendFragment == 0)
    {
        m_pEoEDesc->nMaxSendFragment = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MBOX_HEADER_LEN - ETHERCAT_ETHERNET_HEADER_LEN);
        m_pEoEDesc->nMaxSendFragment = (EC_T_WORD)((EC_T_DWORD)m_pEoEDesc->nMaxSendFragment & ETHERNET_FRAGMENT_MASK);
    }

    m_pEoEDesc->nSendFragNo = 0;
    m_pEoEDesc->nSendFrameNo++;

    MBXHDR_CLR(&mbx);
    EOEHDR_CLR(&eth);

    if (bIsInitCmd)
    {
        pMbx = &mbx;
        pEoe = (ETHERCAT_ETHERNET_HEADER*)pData;
        nData = nData - ETHERCAT_ETHERNET_HEADER_LEN;
        pData = (PETHERNET_FRAME)(((EC_T_BYTE*)pData) + ETHERCAT_ETHERNET_HEADER_LEN);

        if (EC_ECETHHDR_GET_FRAMETYPE(pEoe) != ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETIP_REQ)
        {
            /* EoE traffic, but not a InitCmd. Return busy to queue . */
            dwRetVal = EC_S_DEVICE_BUSY;
            goto Exit;
        }

        EC_ECMBOXHDR_SET_MBXTYPE(pMbx, ETHERCAT_MBOX_TYPE_ETHERNET);
        
        m_pEoEDesc->pendSend.pData  = (EC_T_BYTE*)pData;
        m_pEoEDesc->pendSend.nData  = (EC_T_WORD)nData;
        m_pEoEDesc->pendSend.nOffs  = 0;    

        m_pEoEDesc->retry = m_wRetryAccessCount;

        m_pEoEDesc->pendSend.nSend  = EC_MIN((EC_T_WORD)nData, m_pEoEDesc->nMaxSendFragment);
        EC_ECMBOXHDR_SET_LENGTH(pMbx, (EC_T_WORD)(ETHERCAT_ETHERNET_HEADER_LEN + m_pEoEDesc->pendSend.nSend));
    }
    else
    {
        pMbx = &mbx;
        pEoe = &eth;

        EC_ECMBOXHDR_SET_MBXTYPE(pMbx, ETHERCAT_MBOX_TYPE_ETHERNET);
        EC_ECETHHDR_SET_FRAGMENTNUMBER(pEoe, 0);
        EC_ECETHHDR_SET_FRAMENUMBER(pEoe, (EC_T_WORD)(m_pEoEDesc->nSendFrameNo&ETHERNET_FRAMENUMBER_MASK));
        EC_ECETHHDR_SET_OFFSETBUFFER(pEoe, (EC_T_WORD)ETHERNET_FRAGMENT_BUFFER(nData));

        m_pEoEDesc->pendSend.pData  = (EC_T_BYTE*)pData;
        m_pEoEDesc->pendSend.nData  = (EC_T_WORD)nData;
        m_pEoEDesc->pendSend.nOffs  = 0;    
        
        m_pEoEDesc->retry           = m_wRetryAccessCount;
        /* reset EoE retry access period to protect against saturation */
        if (EC_MBX_RETRYACCESS_PERIOD == m_wEoeRetryAccessPeriod)
        {
            m_wEoeRetryAccessPeriod = (EC_T_WORD)(m_pMaster->GetBusCycleTimeUsec() / 1000);
        }
        EC_ECETHHDR_SET_LASTFRAGMENT(pEoe, (nData <= m_pEoEDesc->nMaxSendFragment));
        m_pEoEDesc->pendSend.nSend  = EC_MIN((EC_T_WORD)nData, m_pEoEDesc->nMaxSendFragment);
        EC_ECMBOXHDR_SET_LENGTH(pMbx, (EC_T_WORD)(ETHERCAT_ETHERNET_HEADER_LEN + m_pEoEDesc->pendSend.nSend));
    }

    dwRes = EcatMbxSendCmdReq(bIsInitCmd, pData, pMbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                     ,pEoe
#endif
#ifdef INCLUDE_FOE_SUPPORT
                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                     ,EC_NULL
                     ,EC_NULL
#endif
                     );
    if (EC_E_NOERROR != dwRes)
    {
        dwRetVal = EC_S_DEVICE_BUSY;
        goto Exit;
    }
    /* no error */
    dwRetVal = EC_S_PENDING;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Sends the queued EoE commands to the slave
*
* \return
*/
EC_T_VOID CEcMbSlave::SendQueuedEoECmds()
{
EC_T_DWORD dwRes      = EC_E_ERROR;
EC_T_BOOL  bIsInitCmd = m_pEoEDesc->bIsInitCmd;
EC_T_BOOL  bCancelPendRecv = EC_FALSE;
EC_T_BOOL  bCancelPendSend = EC_FALSE;

    /* Mbx SyncManagers are not ready or slave is not present */
    if ((3 != (m_wSyncManagerEnabled & 3)) || !IsPresentOnBus())
    {
        bCancelPendRecv = EC_TRUE;
        bCancelPendSend = EC_TRUE;
        goto Exit;
    }
    /* retry current mailbox transfer */
    if (m_pEoEDesc->bRetrySendMbx && m_pEoEDesc->oRetryTimeout.IsElapsed())
    {
        if (m_pEoEDesc->retry == 0)
        {
            bCancelPendSend = EC_TRUE;
            goto Exit;
        }
        m_pEoEDesc->oRetryTimeout.Stop();
        m_pEoEDesc->bRetrySendMbx = EC_FALSE;
        m_pEoEDesc->retry--;

        /* resend last fragment of pending frame */
        ETHERCAT_MBOX_HEADER        mbx;
        ETHERCAT_ETHERNET_HEADER    eth;
        MBXHDR_CLR(&mbx);
        EOEHDR_CLR(&eth);
        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_ETHERNET);
        
        if (bIsInitCmd)
        {
            /* We assume the current init command is of the type ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETIP_REQ */
            EC_ECETHHDR_SET_FRAMETYPE(&eth, ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETIP_REQ);
            
            EC_T_WORD nLeft             = (EC_T_WORD)(m_pEoEDesc->pendSend.nData - m_pEoEDesc->pendSend.nOffs);
            m_pEoEDesc->pendSend.nSend  = (EC_T_WORD)EC_MIN(nLeft, m_pEoEDesc->nMaxSendFragment);
            EC_ECETHHDR_SET_LASTFRAGMENT(&eth, EC_FALSE);
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(ETHERCAT_ETHERNET_HEADER_LEN + m_pEoEDesc->pendSend.nSend));
        }
        else
        {
            EC_ECETHHDR_SET_FRAGMENTNUMBER(&eth, (m_pEoEDesc->nSendFragNo));
            EC_ECETHHDR_SET_FRAMENUMBER(&eth, (EC_T_WORD)(m_pEoEDesc->nSendFrameNo&ETHERNET_FRAMENUMBER_MASK));
            
            /* The sending of the first fragment went wrong so we have to set the buffer size instead of the offset*/ 
            if (m_pEoEDesc->nSendFragNo == 0)
            {
                EC_ECETHHDR_SET_OFFSETBUFFER(&eth, (EC_T_WORD)ETHERNET_FRAGMENT_BUFFER(m_pEoEDesc->pendSend.nData));
            }
            else
            {
                EC_ECETHHDR_SET_OFFSETBUFFER(&eth, (EC_T_WORD)(m_pEoEDesc->pendSend.nOffs/ETHERNET_FRAGMENT_GRANULAR));
            }
            
            EC_T_WORD nLeft             = (EC_T_WORD)(m_pEoEDesc->pendSend.nData - m_pEoEDesc->pendSend.nOffs);
            m_pEoEDesc->pendSend.nSend  = (EC_T_WORD)EC_MIN(nLeft, m_pEoEDesc->nMaxSendFragment);
            EC_ECETHHDR_SET_LASTFRAGMENT(&eth, (nLeft == m_pEoEDesc->pendSend.nSend));
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(ETHERCAT_ETHERNET_HEADER_LEN + m_pEoEDesc->pendSend.nSend));
        }


#if (defined INCLUDE_MAILBOX_STATISTICS)
        m_pMaster->MailboxStatisticsInc(ETHERCAT_MBOX_TYPE_ETHERNET, EC_FALSE, EC_ECMBOXHDR_GET_LENGTH(&mbx) - ETHERCAT_ETHERNET_HEADER_LEN);
#endif

        dwRes = EcatMbxSendCmdReq(bIsInitCmd, &m_pEoEDesc->pendSend.pData[m_pEoEDesc->pendSend.nOffs], &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                         ,&eth
#endif
#ifdef INCLUDE_FOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                         ,EC_NULL
                         ,EC_NULL
#endif
                         );
        if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
        {
            EC_ERRORMSGC_L(("CEcMbSlave::ProcessEoEReturningRequest() EoE: Retry Fragment %d/%d\n", EC_ECETHHDR_GET_FRAMENUMBER(&eth), EC_ECETHHDR_GET_FRAGMENTNUMBER(&eth)));
        }
        goto Exit;
    }
    /* ... or wait for pending mailbox transfer */
    if (EC_NULL != m_pEoEDesc->pendSend.pData)
    {
        goto Exit;
    }
    /* ... or send init command if requested */
    if (m_pEoEDesc->bSendInitCmdReq)
    {
    PEcMailboxCmdDesc pMbxCmdDesc;

        m_pEoEDesc->bSendInitCmdReq = EC_FALSE;

        if (m_wMbxInitCmdsCurrent < m_wMbxInitCmdsCount)
        {
            pMbxCmdDesc = GetMbxInitCmdsCurrent();
            if (ETHERCAT_MBOX_TYPE_ETHERNET == EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc))
            {
                dwRes = SendFrameToSlave(EC_TRUE, (PETHERNET_FRAME)&pMbxCmdDesc->uMbxHdr.eoe.data[0], EC_ECMBXCMDDESC_GET_DATALEN(pMbxCmdDesc));
            }
            else
            {
                OsDbgAssert(EC_FALSE);
            }
        }
        else
        {
            OsDbgAssert(EC_FALSE);
        }
        goto Exit;
    }
    /* ... or send queued frame */
    if (SlaveStateMachIsStable())
    {
        SWITCH_DATA_BUFFER* pDataBufFifo = m_pEoEDesc->pPort->GetQueuedSendFrame(EC_NULL);

        if (EC_NULL != pDataBufFifo)
        {
            dwRes = SendFrameToSlave(EC_FALSE, &pDataBufFifo->head, pDataBufFifo->nData);
        }
    }

Exit:
    if (bCancelPendRecv && (EC_NULL != m_pEoEDesc->pendRecv.pData))
    {
        m_pEoEDesc->pPort->FreeSendBuffer((PETHERNET_FRAME)m_pEoEDesc->pendRecv.pData);
        OsMemset(&m_pEoEDesc->pendRecv, 0, sizeof(m_pEoEDesc->pendRecv));
    }
    if (bCancelPendSend && (EC_NULL != m_pEoEDesc->pendSend.pData))
    {
    EC_T_BYTE* pData = m_pEoEDesc->pendSend.pData;

        /* reset pendsend management */
        OsMemset(&m_pEoEDesc->pendSend, 0, sizeof(m_pEoEDesc->pendSend));
        m_pEoEDesc->oRetryTimeout.Stop();
        m_pEoEDesc->bRetrySendMbx = EC_FALSE;

        /* signalize frame send complete */
        if (!bIsInitCmd)
        {
            dwRes = m_pEoEDesc->pPort->SendFrameComplete(bIsInitCmd, (PETHERNET_FRAME)pData);
        }
        else
        {
            m_pEoEDesc->bIsInitCmd = EC_FALSE;
        }
    }
    EC_UNREFPARM(dwRes);
    return;
}

/********************************************************************************/
/** \brief Process EoE mailbox send commands
*
* \return N/A.
*/
EC_T_VOID   CEcMbSlave::ProcessEoEReturningRequest(
    EC_T_BOOL               bIsInitCmd      /* Indicate if we process an init command  */
   ,EC_T_WORD               wWkc            /* Working counter of the returning frame */
   ,PETYPE_EC_CMD_HEADER    pEcCmdHeader    /* EtherCAT command header*/
   ,PETHERCAT_MBOX_HEADER   pMBox           /* Mailbox header */
                                                  )
{
EC_T_DWORD dwRes = EC_E_ERROR;
EC_T_BOOL bWkcNotify = EC_FALSE;
PEcMailboxCmdDesc pMbxCmdDesc = EC_NULL;

    EC_UNREFPARM(pMBox);

    if (m_pEoEDesc == EC_NULL)
        return;

    if (wWkc == 1 )
    {
        if (m_pEoEDesc->pendSend.pData != EC_NULL)
        {   
            /* bump send offset */
            m_pEoEDesc->pendSend.nOffs = (EC_T_WORD)(m_pEoEDesc->pendSend.nOffs + m_pEoEDesc->pendSend.nSend);

            /* check for last fragment */
            if (m_pEoEDesc->pendSend.nData == m_pEoEDesc->pendSend.nOffs)
            {
                if (bIsInitCmd)
                {
                    /* reset pendsend management */
                    m_pEoEDesc->pendSend.pData = EC_NULL;
                }
                else
                {
                EC_T_BYTE* pData = m_pEoEDesc->pendSend.pData;

                    /* reset pendsend management */
                    m_pEoEDesc->pendSend.pData = EC_NULL;

                    /* signalize frame send complete */
                    if (!bIsInitCmd)
                    {
                        dwRes = m_pEoEDesc->pPort->SendFrameComplete(bIsInitCmd, (PETHERNET_FRAME)pData);
                    }
                }
            }
            else
            {
                /* send next fragment of pending frame */
                ETHERCAT_MBOX_HEADER        mbx;
                ETHERCAT_ETHERNET_HEADER    eth;
                MBXHDR_CLR(&mbx);
                EOEHDR_CLR(&eth);
                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_ETHERNET);
                m_pEoEDesc->nSendFragNo++;
                EC_ECETHHDR_SET_FRAGMENTNUMBER(&eth, m_pEoEDesc->nSendFragNo);
                EC_ECETHHDR_SET_FRAMENUMBER(&eth, (EC_T_WORD)(m_pEoEDesc->nSendFrameNo&ETHERNET_FRAMENUMBER_MASK));
                EC_ECETHHDR_SET_OFFSETBUFFER(&eth, (EC_T_WORD)(m_pEoEDesc->pendSend.nOffs/ETHERNET_FRAGMENT_GRANULAR));

                EC_T_WORD nLeft                 = (EC_T_WORD)(m_pEoEDesc->pendSend.nData - m_pEoEDesc->pendSend.nOffs);
                m_pEoEDesc->pendSend.nSend      = (EC_T_WORD)EC_MIN(nLeft, m_pEoEDesc->nMaxSendFragment);
                EC_ECETHHDR_SET_LASTFRAGMENT(&eth, (nLeft == m_pEoEDesc->pendSend.nSend));
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(ETHERCAT_ETHERNET_HEADER_LEN + m_pEoEDesc->pendSend.nSend));

                m_pEoEDesc->retry = m_wRetryAccessCount;

                EcatMbxSendCmdReq(bIsInitCmd, &m_pEoEDesc->pendSend.pData[m_pEoEDesc->pendSend.nOffs], &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                 ,&eth
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,EC_NULL
                                 ,EC_NULL
#endif
                                 );
            }
        } /* if (m_pEoEDesc->pendSend.pData != EC_NULL) */
    } /* if (wWkc == 1 ) */
    else
    {
        if (bIsInitCmd)
        {
            if (AreMbxInitCmdsPending())
            {
                /* reset pendsend management */
                m_pEoEDesc->pendSend.pData = EC_NULL;

                if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                {
                    if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                    {
                        m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        m_wInitCmdsRetries--;
                    }
                    pMbxCmdDesc = GetMbxInitCmdsCurrent();
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessEoEReturningRequest() retry send slave mailbox InitCmd[%d-%d] fragment: %s: %s\n", 
                        (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                        GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                        StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)))
                        );
                }
                else
                {
                    /* give up */
                    bWkcNotify = EC_TRUE;
                }
            }
        }
        else
        {
            if ((EC_NULL != m_pEoEDesc->pendSend.pData) && (m_pEoEDesc->retry > 0))
            {
                /* adjust EoE retry access period dynamically starting from second retry */
                if (m_pEoEDesc->retry < m_wRetryAccessCount)
                {
                    m_wEoeRetryAccessPeriod = (EC_T_WORD)(EC_MIN(m_wEoeRetryAccessPeriod * 2, EC_MBX_RETRYACCESS_PERIOD));
                }
                m_pEoEDesc->oRetryTimeout.Start(m_wEoeRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
                m_pEoEDesc->bRetrySendMbx = EC_TRUE;
            }
            else
            {
            EC_T_BYTE* pData = m_pEoEDesc->pendSend.pData;

                EC_ERRORMSGC_L(("CEcMbSlave::ProcessEoEReturningRequest() EoE: Fragment %d/%d sent but returned with WKC = 0 -> No more retries!\n", (m_pEoEDesc->nSendFrameNo&ETHERNET_FRAMENUMBER_MASK), m_pEoEDesc->nSendFragNo));

                /* reset pendsend management */
                m_pEoEDesc->pendSend.pData = EC_NULL;

                /* signalize frame send complete */
                if (!bIsInitCmd)
                {
                    dwRes = m_pEoEDesc->pPort->SendFrameComplete(bIsInitCmd, (PETHERNET_FRAME)pData);
                }
                /* give up */
                bWkcNotify = EC_TRUE;
            }
        }
    }
    if (bIsInitCmd && bWkcNotify )
    {
        /* don't continue sending init commands */
        StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
    }
    if (bWkcNotify)
    {
        EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

        pNotification = m_pMaster->AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_EOE_MBXSND_WKC_ERROR;
            GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.SlaveProp));
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd  = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct  = wWkc;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet  = 1;
            m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
        }
    }
    EC_UNREFPARM(dwRes);
    EC_UNREFPARM(pMbxCmdDesc);
}

/********************************************************************************/
/** \brief Process an received EoE response
*
* \return N/A.
*/
EC_T_VOID   CEcMbSlave::ProcessEoEResponse
(   EC_T_BOOL bIsInitCmd, 
    PETHERCAT_MBOX_HEADER pMBox )
{

    if (m_pEoEDesc == EC_NULL)
    {        
        goto Exit;
    }
    if (
            (m_pEoEDesc->pPort) 
        &&  (m_mbxOLen[m_sMboxFlags.mbxIdx] > 0) 
        && (EC_ECMBOXHDR_GET_LENGTH(pMBox) >= ETHERCAT_ETHERNET_HEADER_LEN) 
       )
    {
        PETHERCAT_ETHERNET_HEADER   pEoeHdr  = (PETHERCAT_ETHERNET_HEADER)EC_ENDOF(pMBox);
    
#if (defined INCLUDE_MAILBOX_STATISTICS)
        m_pMaster->MailboxStatisticsInc(EC_MAILBOX_STATISTICS_IDX_EOE, EC_TRUE,  EC_ECMBOXHDR_GET_LENGTH(pMBox)-ETHERCAT_ETHERNET_HEADER_LEN);
#endif

        if (EC_ECETHHDR_GET_FRAMETYPE(pEoeHdr) == ETHERCAT_ETHERNET_FRAME_TYPE_EOE_FRAG_REQ)
        {
            bIsInitCmd = EC_FALSE;

            /* check if we expect to receive an additional fragment */
            if (m_pEoEDesc->pendRecv.pData )
            {   /* further fragments */
                if (m_pEoEDesc->pendRecv.nData == (EC_ECETHHDR_GET_OFFSETBUFFER(pEoeHdr)*ETHERNET_FRAGMENT_GRANULAR) &&
                    (EC_T_WORD)(m_pEoEDesc->pendRecv.nMax - m_pEoEDesc->pendRecv.nData) >= (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_ETHERNET_HEADER_LEN) &&
                    m_pEoEDesc->nLastFragNo+1 == EC_ECETHHDR_GET_FRAGMENTNUMBER(pEoeHdr))
                {   /* expected fragment -> copy data of fragment */
                    EC_T_WORD size = (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(pMBox)-ETHERCAT_ETHERNET_HEADER_LEN);
                    OsMemcpy(&m_pEoEDesc->pendRecv.pData[m_pEoEDesc->pendRecv.nData], EC_ENDOF(pEoeHdr), size);
                    m_pEoEDesc->pendRecv.nData  = (EC_T_WORD)(m_pEoEDesc->pendRecv.nData + size);
                    m_pEoEDesc->nLastFragNo++;
                    
                    if (EC_ECETHHDR_GET_LASTFRAGMENT(pEoeHdr))
                    {   /* last fragment -> switch whole frame */
                        m_pEoEDesc->pPort->SwitchFrame(bIsInitCmd, (PETHERNET_FRAME)m_pEoEDesc->pendRecv.pData,  m_pEoEDesc->pendRecv.nData, 0);
                        OsMemset(&m_pEoEDesc->pendRecv, 0, sizeof(m_pEoEDesc->pendRecv));
                    }
                }
                else
                {   /* error */
                    m_pEoEDesc->pPort->FreeSendBuffer((PETHERNET_FRAME)m_pEoEDesc->pendRecv.pData);
                    OsMemset(&m_pEoEDesc->pendRecv, 0, sizeof(m_pEoEDesc->pendRecv));
                    
                    if (bIsInitCmd)
                    {
                        /* don't continue sending init commands */
                        StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition);
                    }
                }
            }

            if (EC_ECETHHDR_GET_FRAGMENTNUMBER(pEoeHdr) == 0)
            { 
                m_pEoEDesc->nLastFragNo = 0;
                /* first fragment of new frame */
                if (EC_ECETHHDR_GET_LASTFRAGMENT(pEoeHdr))
                {   
                    /* frame is complete */
                    EC_T_PBYTE  pbyData     = (EC_T_PBYTE)EC_ENDOF(pEoeHdr);
                    EC_T_DWORD  dwDataLen   = (EC_ECMBOXHDR_GET_LENGTH(pMBox)-ETHERCAT_ETHERNET_HEADER_LEN);
                    EC_T_DWORD  dwTimeStamp = 0;

                    if (EC_ECETHHDR_GET_TIMEAPPENDED(pEoeHdr))
                    {
                        if (dwDataLen >= ETHERCAT_EOE_TIMESTAMP_LEN)
                        {
                            dwDataLen -= ETHERCAT_EOE_TIMESTAMP_LEN;
                            dwTimeStamp = EC_GET_FRM_DWORD((&(pbyData[dwDataLen])));
                        }
                    }

                    if (dwDataLen >= ETHERNET_FRAME_LEN)
                    {
                        m_pEoEDesc->pPort->SwitchFrame(
                            bIsInitCmd, 
                            (ETHERNET_FRAME*)pbyData,
                            dwDataLen, 
                            0, 
                            dwTimeStamp,
                            EC_ECETHHDR_GET_PORT(pEoeHdr)
                                                      );
                    }
                    
                }
                else
                {   /* more fragments follows -> alloc buffer from switch */
                    m_pEoEDesc->pendRecv.nMax   = (EC_T_WORD)(EC_ECETHHDR_GET_OFFSETBUFFER(pEoeHdr)*ETHERNET_FRAGMENT_GRANULAR);
                    if (m_pEoEDesc->pPort->AllocSendBuffer(m_pEoEDesc->pendRecv.nMax, (PETHERNET_FRAME*)(EC_T_VOID*)&m_pEoEDesc->pendRecv.pData) == EC_S_SUCCESS )
                    {   /* copy data of first fragment */
                        m_pEoEDesc->pendRecv.nData  = (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_ETHERNET_HEADER_LEN);
                        OsMemcpy(m_pEoEDesc->pendRecv.pData, EC_ENDOF(pEoeHdr), m_pEoEDesc->pendRecv.nData);
                    }
                }
            }
        } /* EC_ECETHHDR_GET_FRAMETYPE(pEoeHdr) == ETHERCAT_ETHERNET_FRAME_TYPE_EOE_FRAG_REQ */
        else if (EC_ECETHHDR_GET_FRAMETYPE(pEoeHdr) == ETHERCAT_ETHERNET_FRAME_TYPE_EOE_SETIP_RSP)
        {
            if (bIsInitCmd)
            {
                if (AreMbxInitCmdsActive())
                {
                    StartInitCmds(m_wActTransition, EC_FALSE);
                }
            }
        }
        else
        {
            EC_ERRORMSG_L(("CEcMbSlave::ProcessEoEResponse: frame type %d not supported!\n", (EC_T_INT)EC_ECETHHDR_GET_FRAMETYPE(pEoeHdr)));
        }
    }

Exit:
    return;
}
#endif /* INCLUDE_EOE_SUPPORT */

EC_T_DWORD CEcMbSlave::MailboxRes(PEcMailboxCmd pReq, EC_T_DWORD nResult, EC_T_DWORD cbLength, EC_T_VOID* pData)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;

    switch (pReq->wMbxCmdType)
    {
    case EC_MAILBOX_CMD_UPLOAD:
    case EC_MAILBOX_CMD_READWRITE:   
        m_pMaster->MailboxResponse(pReq, (EC_T_BYTE*)pData, cbLength, nResult);
        break;
                
    case EC_MAILBOX_CMD_DOWNLOAD:
        m_pMaster->MailboxResponse(pReq, pReq->pbyData, pReq->length, nResult);
        break;

    default:
        dwRetVal = EC_E_INVALIDPARM;
        break;
    }

    return dwRetVal;
}


/********************************************************************************/
/** \brief Process CoE response
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessCoEResponse
(   EC_T_BOOL bIsInitCmd,
    PETHERCAT_MBOX_HEADER pMBox )
{
PETHERCAT_CANOPEN_HEADER pCan;

    if (m_pCoEDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
    pCan = (PETHERCAT_CANOPEN_HEADER)EC_ENDOF(pMBox);

    /* check if the length filed of the mailbox header has a minimum size of (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN). */ 
    if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDOREQ || 
       EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDORES)
    {
        if (EC_ECMBOXHDR_GET_LENGTH(pMBox) < (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN))
        {
            /* mailbox header length does not match.*/
/*            EC_ERRORMSGC(("Warning: Invalid length in the mailbox header length field. (MailboxHeader.length(%d) < %d for Slave with fixed address %d)\n", EC_ECMBOXHDR_GET_LENGTH(pMBox), (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN), GetFixedAddr())); */
        }
    }
    else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDOINFO)
    {
        if (EC_ECMBOXHDR_GET_LENGTH(pMBox) < 8)
        {
            /* mailbox header length does not match.*/
/*            EC_ERRORMSGC(("Warning: Invalid length in the mailbox header length field. (MailboxHeader.length(%d) < %d for Slave with fixed address %d)\n", EC_ECMBOXHDR_GET_LENGTH(pMBox), 8, GetFixedAddr())); */
        }
    }
       
    if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDORES)
    {
        PEC_SDO_HDR pSdo = (PEC_SDO_HDR)EC_ENDOF(pCan);
        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() ETHERCAT_CANOPEN_TYPE_SDORES. m_wMbxInitCmdsCurrent = %d this = 0x%x\n", m_wMbxInitCmdsCurrent));

        if (bIsInitCmd)
        {
            if (AreMbxInitCmdsPending())
            {
            PEcMailboxCmdDesc pMbxCmdDesc = GetMbxInitCmdsCurrent();

                /* CANopen init command */
                switch (pSdo->uHdr.Ids.Scs)
                {
                case SDO_SCS_INITIATE_UPLOAD:
                    {
                    EC_T_BYTE* pbyData    = EC_NULL;
                    EC_T_WORD  wDataLen   = 0;
                    EC_T_BOOL  bDataValid = EC_FALSE;

                        /* get data and data length */
                        if (pSdo->uHdr.Ius.Expedited && pSdo->uHdr.Ius.SizeInd )
                        {
                            /* expedited transfer with size indicator */
                            pbyData  = EC_ECSDOHDR_SDODATA(pSdo);
                            wDataLen = (EC_T_WORD)((sizeof(EC_T_DWORD) - pSdo->uHdr.Ius.Size));
                        }
                        else if (pSdo->uHdr.Ius.Expedited && !pSdo->uHdr.Ius.SizeInd )
                        {
                            /* expedited transfer without size indicator */
                            pbyData = EC_ECSDOHDR_SDODATA(pSdo);
                            wDataLen = sizeof(EC_T_DWORD);
                        }                        
                        else if (!pSdo->uHdr.Ius.Expedited && pSdo->uHdr.Ius.SizeInd)
                        {
                            /* standard transfer */
                            pbyData  = (EC_T_BYTE*)EC_ENDOF(pSdo);
                            wDataLen = (EC_T_WORD)EC_MIN(EC_ECSDOHDR_GET_SDODATA(pSdo), (EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_CANOPEN_HEADER_LEN - EC_SDO_HDR_LEN));
                        }
                        /* validate if needed */
                        if (0 == pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.__dwSdoData)
                        {
                            bDataValid = EC_TRUE;
                        }
                        else if (pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.__dwSdoData >= wDataLen)
                        {   
                            if (OsMemcmp(pMbxCmdDesc->uMbxHdr.coe.data, pbyData, wDataLen) == 0)
                            {
                                bDataValid = EC_TRUE;
                            }
                            else
                            {
                                bDataValid = EC_FALSE;
                                EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d) '%s':verify INIT Command fails! \n", m_pEniSlave->szName, m_wFixedAddr, GetStateChangeNameShort(m_wActTransition)));
                            }
                        }
                        else
                        {
                            bDataValid = EC_FALSE;
                            EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d) '%s':slave returned more data than Mailbox client has expected! \n", m_pEniSlave->szName, m_wFixedAddr, GetStateChangeNameShort(m_wActTransition)));
                        }
                        if (bDataValid)
                        {
                            m_pMaster->NotifySlaveCoEInitCmd(this, m_wActTransition, GetMbxInitCmdsCurrent(), EC_E_NOERROR, pbyData, wDataLen);
         
                            /* upload complete -> next init cmd */
                            EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() %s: start next init command\n", m_pEniSlave->szName));
                            StartInitCmds(m_wActTransition, EC_FALSE);
                        }
                        else
                        {
                            m_pMaster->NotifySlaveCoEInitCmd(this, m_wActTransition, GetMbxInitCmdsCurrent(), EC_E_INVALIDDATA, pbyData, wDataLen);

                            m_pMaster->NotifySlaveInitCmdResponseError(this, m_wActTransition, eInitCmdErr_VALIDATION_ERR, 
                                EcMailboxCmdDescComment(GetMbxInitCmdsCurrent()), EC_ECMBXCMDDESC_GET_CMTLEN(GetMbxInitCmdsCurrent()));
                            
                            EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%d'. \n", m_pEniSlave->szName, m_wFixedAddr, 
                                GetStateChangeNameShort(m_wActTransition), EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR));

                            /* don't continue sending init commands */
                            StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
                         }
                    }
                    break;
                case SDO_SCS_INITIATE_DOWNLOAD:
                case SDO_SCS_DOWNLOAD_SEGMENT:
                    {
                        /* Check if the coe sdo transfer is complete or not  */
                        if (EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader))-m_wMbxInitCmdsDataOffset == 0 )
                        {
                            m_pMaster->NotifySlaveCoEInitCmd(this, m_wActTransition, GetMbxInitCmdsCurrent(), EC_E_NOERROR, &pMbxCmdDesc->uMbxHdr.coe.data[0], EC_LOWORD(EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader))));

                            /* transfer complete -> next init cmd */
                            EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() %s: start next init command\n", m_pEniSlave->szName));
                            StartInitCmds(m_wActTransition, EC_FALSE);
                        }
                        else
                        {   /* send next segment */
                            /* nMaxData fits in mailbox */
                            EC_T_WORD nMaxData  = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SDO_MBOX_LEN + SDO_DOWNLOAD_SEGMENT_MAX_DATA);
                            EC_T_WORD nSendData     = EC_MIN(nMaxData, (EC_T_WORD)(EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)) - m_wMbxInitCmdsDataOffset));                            
                            EC_T_WORD nRemainLength = EC_MAX(nMaxData, (EC_T_WORD)(EC_ECSDOHDR_GET_SDODATA(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)) - m_wMbxInitCmdsDataOffset));
                            
                            /* CoE Init command timeout */
                            EC_T_WORD wInitCmdTimeout = EC_ECMBXCMDDESC_GET_MBXTIMEOUT(pMbxCmdDesc);
                            if (0 == wInitCmdTimeout)
                            {
                                wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                            }
                            m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
                            m_oInitCmdRetryTimeout.Stop();

                            ETHERCAT_MBOX_HEADER    mbx;
                            ETHERCAT_CANOPEN_HEADER can;
                            EC_SDO_HDR     EcSdoHeader;
                            MBXHDR_CLR(&mbx);
                            COEHDR_CLR(&can);
                            SDOHDR_CLR(&EcSdoHeader);
                            EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
                            EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
                            EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
                            EcSdoHeader.uHdr.Dsq.Ccs = SDO_CCS_DOWNLOAD_SEGMENT;

                            /* we have to toggle the following bit with every Download SDO Segment Request*/
                            if (pSdo->uHdr.Ids.Scs == SDO_SCS_INITIATE_DOWNLOAD)   
                            {
                                /* the toggle bit starts with 0x00 */
                                pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Dsq.Toggle = 0;    
                            }
                            else                                
                            {
                                /* store the toggle bit for the next SDO Download Segment  */
                                pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Dsq.Toggle = !pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Dsq.Toggle;    /* last cmd was an initiate */
                            }

                            /* adjust the toggle bit for the next SDO Download Segment  */
                            EcSdoHeader.uHdr.Dsq.Toggle = pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Dsq.Toggle;

                            /* in case of an Download SDO Segment Request you can store 7 bytes in the SDO header */
                            if (nSendData < SDO_DOWNLOAD_SEGMENT_MAX_DATA)
                            {
                                /* set the segment data size and copy a corresponding number of bytes in the sdo header  */
                                EcSdoHeader.uHdr.Dsq.Size       = SDO_DOWNLOAD_SEGMENT_MAX_DATA - nSendData;

                                /* Copy the 1-6 bytes into the CoE sdo header */
#if defined WIN32
                                OsMemcpy( &(EcSdoHeader.__Index),    /* use this to prevent microsoft compiler error */
                                    &pMbxCmdDesc->uMbxHdr.coe.data[m_wMbxInitCmdsDataOffset], 
                                    nSendData );
#else
                                OsMemcpy( ((EC_T_BYTE*)&EcSdoHeader) + SDO_HDR_INDEX_OFFSET, /* &EcSdoHeader.Index to avoid diab compiler warning*/
                                    &pMbxCmdDesc->uMbxHdr.coe.data[m_wMbxInitCmdsDataOffset], 
                                    nSendData );
#endif
                            }
                            else
                            {
                                /* the complete data portion does not fit into the SDO header so we copy at first 7 bytes */
                                EcSdoHeader.uHdr.Dsq.Size = 0;
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + nSendData - SDO_DOWNLOAD_SEGMENT_MAX_DATA));
#if defined WIN32
                                OsMemcpy( (&EcSdoHeader.__Index),    /* use this to prevent microsoft compiler error */
                                    &pMbxCmdDesc->uMbxHdr.coe.data[m_wMbxInitCmdsDataOffset], 
                                    SDO_DOWNLOAD_SEGMENT_MAX_DATA );
#else
                                OsMemcpy( ((EC_T_BYTE*)&EcSdoHeader) + SDO_HDR_INDEX_OFFSET, /* &EcSdoHeader.Index to avoid diab compiler warning*/
                                    &pMbxCmdDesc->uMbxHdr.coe.data[m_wMbxInitCmdsDataOffset], 
                                    SDO_DOWNLOAD_SEGMENT_MAX_DATA );
#endif
                            }
         
                            /* set the last segment bit if required */
                            EcSdoHeader.uHdr.Dsq.LastSeg = (nRemainLength <= nMaxData);

                            EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() Send next slave mailbox InitCmd[%d-%d]: %s: %s - id=0x%x, subid=%d\n", 
                                      (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                                      GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                                      StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)),
                                      (EC_T_INT)EC_ECSDOHDR_GET_INDEX(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)), pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex));

                            /* send the CoE init command ether with data (if the data does not fit into the sdo header) or without data (expedited) */
                            if (nSendData > SDO_DOWNLOAD_SEGMENT_MAX_DATA )
                            {
                                EcatMbxSendCmdReq(bIsInitCmd, &pMbxCmdDesc->uMbxHdr.coe.data[SDO_DOWNLOAD_SEGMENT_MAX_DATA + m_wMbxInitCmdsDataOffset], &mbx, &can, &EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                                 ,EC_NULL
                                                 ,EC_NULL
#endif
                                                 );
                            }
                            else
                            {
                                EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, &can, &EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                                 ,EC_NULL
                                                 ,EC_NULL
#endif
                                                 );
                            }
                            /* adjust the current data offset for the next loop */
                            m_wMbxInitCmdsDataOffset = (EC_T_WORD)(m_wMbxInitCmdsDataOffset + nSendData);
                        }
                    }
                    break;
                default:
                    EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d) '%s':  wrong slave response pSdo->uHdr.Ids.Scs = 0x%x\n", m_pEniSlave->szName, m_wFixedAddr, GetStateChangeNameShort(m_wActTransition), (EC_T_DWORD)pSdo->uHdr.Ids.Scs));
                    /*OsDbgAssert(EC_FALSE); -> can happen in case of a bad line */
                    ;
                } /* switch ( pSdo->Ids.Scs ) */
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid )
        {   
            /* Check if the CoE Mailbox is as expected a response. */
            if ( (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType != ETHERCAT_CANOPEN_TYPE_SDORES)
               &&(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType != ETHERCAT_CANOPEN_TYPE_SDOREQ)
              )
            {
                /* A not expected datagram was received. (buggy slave) don't process this datagram */
                EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d):  A not expected datagram was received (type %d expected and %d read)! \n", m_pEniSlave->szName, m_wFixedAddr,
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType, ETHERCAT_CANOPEN_TYPE_SDORES));
                goto Exit;
            }
            /* Check if the response is the expected response */ 
            if (((m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->sIndex.index != EC_ECSDOHDR_GET_INDEX(pSdo))
                || (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->sIndex.subIndex != pSdo->SubIndex))
                && (pSdo->uHdr.Ids.Scs != SDO_SCS_UPLOAD_SEGMENT ) && (pSdo->uHdr.Ids.Scs != SDO_SCS_DOWNLOAD_SEGMENT)
                )
            {
                /* A not expected datagram was received. (buggy slave) don't process this datagram */
                EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d):  A not expected datagram was received (Index 0x%x,0x%x expected and 0x%x,0x%x read)! \n", m_pEniSlave->szName, m_wFixedAddr,
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->sIndex.index, 
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->sIndex.subIndex,
                    EC_ECSDOHDR_GET_INDEX(pSdo), pSdo->SubIndex
                    ));
                goto Exit;
            }

            /* queued CANopen Mailbox command */
            switch (pSdo->uHdr.Ids.Scs)
            {
            case SDO_SCS_INITIATE_UPLOAD:
                /* response of an initiate upload */
                if (pSdo->uHdr.Ius.Expedited && pSdo->uHdr.Ius.SizeInd )
                {                           
                    /* expedited transfer with size indicator */
                    if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= sizeof(EC_T_DWORD) - pSdo->uHdr.Ius.Size)
                    {   
                        MailboxRes
                        /*MailboxUploadRes*/( m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, 
                                          EC_E_NOERROR, 
                                          sizeof(EC_T_DWORD) - pSdo->uHdr.Ius.Size, 
                                          EC_ECSDOHDR_SDODATA(pSdo)
                                        );  /* &EC_ECSDOHDR_GET_SDODATA(pSdo) (avoid diab compiler warning)*/

                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid = EC_FALSE;
                    }
                    else
                    {   /* size mismatch (slave returned more data than Mailbox client has expected) */
                        EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d):  Size mismatch (slave returned more data than Mailbox client has expected) (max. %d expected and %d read)! \n", m_pEniSlave->szName, m_wFixedAddr,
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length, 
                            sizeof(EC_T_DWORD) - pSdo->uHdr.Ius.Size                            
                            ));
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE);
                    }
                }
                else if (pSdo->uHdr.Ius.Expedited && !pSdo->uHdr.Ius.SizeInd )
                {   /* expedited transfer without size indicator */
                    EC_T_DWORD length = EC_MIN(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length, sizeof(EC_T_DWORD));
                    
                    MailboxRes( m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, 
                                      EC_E_NOERROR, 
                                      length, 
                                      EC_ECSDOHDR_SDODATA(pSdo)
                                    );  /* &EC_ECSDOHDR_GET_SDODATA(pSdo) (avoid diab compiler warning)*/
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid = EC_FALSE;
                }
                else if (!pSdo->uHdr.Ius.Expedited && pSdo->uHdr.Ius.SizeInd )
                {
                    /*******************/
                    /* normal transfer */
                    /*******************/
                    EC_T_DWORD nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_CANOPEN_HEADER_LEN - EC_SDO_HDR_LEN;                   
                    if (nData == EC_ECSDOHDR_GET_SDODATA(pSdo))
                    {   
                        /* initiate upload response contains all data (behind the 8 byte sdo header) */
                        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= nData )
                        {   /* size of Mailbox response is OK */
                            MailboxRes(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_NOERROR, nData, EC_ENDOF(pSdo));
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid = EC_FALSE;
                        }
                        else
                        {   /* size mismatch (slave returned more data than Mailbox client has expected) */
                            StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE);
                        }
                    }
                    else if (nData < EC_ECSDOHDR_GET_SDODATA(pSdo))
                    {   
                        /* more data follows -> alloc Mailbox response */
                        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= EC_ECSDOHDR_GET_SDODATA(pSdo))
                        {   
                            /* size of Mailbox response is OK */
                            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData )
                            {   /* save returned data (if any) and request next segment */
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.nRetData   = EC_ECSDOHDR_GET_SDODATA(pSdo);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset    = 0;
                                if (nData > 0 )
                                {
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset    = nData;
                                    OsMemcpy(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData, EC_ENDOF(pSdo), nData);
                                }
                                /* request next segment */
                                ETHERCAT_MBOX_HEADER    mbx;
                                ETHERCAT_CANOPEN_HEADER can;
                                MBXHDR_CLR(&mbx);
                                COEHDR_CLR(&can);
                                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
                                EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;
                                /*m_pCoEDesc->CurrPendCoeMbxCmdDesc.oCoeTimeout.Start(m_pMaster->m_MasterConfig.dwCoeTimeout, m_pMaster->GetMsecCounterPtr());*/

                                OsMemset(&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader, 0, sizeof(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader));
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Usq.Ccs    = SDO_CCS_UPLOAD_SEGMENT;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Usq.Toggle = 0;

                                if (!(
#ifdef INCLUDE_FOE_SUPPORT
                                      (EC_NULL != m_pFoEDesc && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) || 
#endif
#ifdef INCLUDE_SOE_SUPPORT             
                                      (EC_NULL != m_pSoEDesc && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) || 
#endif
                                      m_bMbxCmdPending
                                     ))
                                {
                                    EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, &can, &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                                     ,EC_NULL
                                                     ,EC_NULL
#endif
                                                     );
                                }
                                else
                                {
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcCanHeader = can;
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcMbxHeader = mbx;
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_TRUE;
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePendingData = EC_FALSE;
                                }
                            }
                            else
                            {   /* memory allocation error -> send ABORT to the slave */
                                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_NOMEMORY, SDO_ABORTCODE_MEMORY);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOMEMORY;
                            }
                        }
                        else
                        {   /* size mismatch (slave returned more data than Mailbox client has expected) */
                            StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_DATA_SIZE1);
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                        }
                    }
                    else
                    {   /* size mismatch (more data in mailbox than specified in sdo header) */
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_DATA_SIZE);
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                    }
                }
                else
                {   /* invalid bits in sdo header */
                    StopCurrCoEMbxRequest(bIsInitCmd, EC_E_ERROR, SDO_ABORTCODE_GENERAL);
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_ERROR;
                }
                break;
            case SDO_SCS_UPLOAD_SEGMENT:
                if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData )
                {   /* memory of return cmd must be pre allocated! */
                    if (pSdo->uHdr.Uss.Toggle == m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Usq.Toggle )
                    {   /* toggle of segment is OK */
                        EC_T_DWORD nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_CANOPEN_HEADER_LEN - EC_SDO_HDR_LEN + SDO_DOWNLOAD_SEGMENT_MAX_DATA;
                        
                        if (nData == SDO_DOWNLOAD_SEGMENT_MAX_DATA )   /* if last segment size == 7 bytes */
                            nData -= pSdo->uHdr.Uss.Size;                    /* consider size indicator */
                        if (pSdo->uHdr.Uss.LastSeg )
                        {   /* last segment */
                            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset + nData == m_pCoEDesc->CurrPendCoeMbxCmdDesc.nRetData )
                            {   /* sumerized size is OK */
                                /* save data */

#if defined WIN32
                                OsMemcpy( 
                                          &(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset]), 
                                          &(pSdo->__Index),                       /* use this to prevent microsoft compiler error */
                                          nData );
#else
                                OsMemcpy( 
                                          &m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset], 
                                          ((EC_T_BYTE*)pSdo) + SDO_HDR_INDEX_OFFSET,        /* &pSdo->Index   to avoid diab compiler warning*/
                                          nData );
#endif

                                /* return to Mailbox client */
                                StopCurrCoEMbxRequest(
                                    bIsInitCmd, 
                                    EC_E_NOERROR, 
                                    0, 
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.nRetData, 
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData
                                                     );
                            }
                            else
                            {   /* size mismatch */
                                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_BLK_SIZE);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                            }
                        }
                        else
                        {   /* a segment in the middle */
                            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset + nData < m_pCoEDesc->CurrPendCoeMbxCmdDesc.nRetData )
                            {   /* summarized data fits in pre allocated buffer -> copy data to buffer */
#if defined WIN32
                                
                                OsMemcpy( 
                                          &m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset], 
                                          &(pSdo->__Index),                       /* use this to prevent microsoft compiler error */
                                          nData );
#else
                                OsMemcpy( 
                                          &m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset], 
                                          ((EC_T_BYTE*)pSdo) + SDO_HDR_INDEX_OFFSET,        /* &pSdo->Index   to avoid diab compiler warning*/
                                          nData );
#endif
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset        += nData;

                                /* request new segment */
                                ETHERCAT_MBOX_HEADER    mbx;
                                ETHERCAT_CANOPEN_HEADER can;
                                MBXHDR_CLR(&mbx);
                                COEHDR_CLR(&can);
                                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
                                EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;

                                /*m_pCoEDesc->CurrPendCoeMbxCmdDesc.oCoeTimeout.Start(m_pMaster->m_MasterConfig.dwCoeTimeout, m_pMaster->GetMsecCounterPtr());*/

                                OsMemset(&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader, 0, sizeof(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader));
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Usq.Ccs      = SDO_CCS_UPLOAD_SEGMENT;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Usq.Toggle   = !pSdo->uHdr.Uss.Toggle;

                                if (!(
#ifdef INCLUDE_FOE_SUPPORT
                                      (EC_NULL != m_pFoEDesc && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) || 
#endif
#ifdef INCLUDE_SOE_SUPPORT             
                                      (EC_NULL != m_pSoEDesc && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) || 
#endif
                                      m_bMbxCmdPending
                                     ))
                                {
                                    EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, &can, &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                                     ,EC_NULL
                                                     ,EC_NULL
#endif
                                                     );
                                }
                                else
                                {
                                   /* save inforamtions for resend */
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcCanHeader = can;
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcMbxHeader = mbx;
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_TRUE;
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePendingData = EC_FALSE;
                                }
                            }
                            else
                            {   /* buffer to small */
                                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_DATA_SIZE1);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                            }
                        }
                    }
                    else
                    {   /* toggle bit error */
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_SDO_ABORTCODE_TOGGLE, SDO_ABORTCODE_TOGGLE);
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_SDO_ABORTCODE_TOGGLE;
                    }
                }
                else
                {   /* invalid state of sdoSafe (should never occur) */
                    StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDDATA, SDO_ABORTCODE_GENERAL);
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDDATA;
                }
                break;
            case SDO_SCS_INITIATE_DOWNLOAD:
            case SDO_SCS_DOWNLOAD_SEGMENT:
                {
                    if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset < m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length )
                    {   /* more data to send */
                        EC_T_WORD nMaxData      = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SDO_MBOX_LEN + SDO_DOWNLOAD_SEGMENT_MAX_DATA);
                        EC_T_WORD nSendData     = EC_MIN(nMaxData, (EC_T_WORD)(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length - m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset));
                        EC_T_WORD nRemainLength = EC_MAX(nMaxData, (EC_T_WORD)(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length - m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset));
                        EC_T_BYTE byToggleSave = 0;
                        ETHERCAT_MBOX_HEADER    mbx;
                        ETHERCAT_CANOPEN_HEADER can;

                        MBXHDR_CLR(&mbx);
                        COEHDR_CLR(&can);
                        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
                        EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;
                        /*m_pCoEDesc->CurrPendCoeMbxCmdDesc.oCoeTimeout.Start(m_pMaster->m_MasterConfig.dwCoeTimeout, m_pMaster->GetMsecCounterPtr());*/
                        EC_T_BYTE* pData = &(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset]);

                        /* save toggle bit */
                        byToggleSave = m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Toggle;
                        OsMemset(&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader, 0, sizeof(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader));
                        /* resore it after memset */
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Toggle = byToggleSave;


                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Ccs        = SDO_CCS_DOWNLOAD_SEGMENT;
                        if (pSdo->uHdr.Ids.Scs == SDO_SCS_INITIATE_DOWNLOAD )
                        {
                            /* not necessary due to previous memset m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Toggle = 0;*/
                        }
                        else
                        {
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Toggle = !m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Toggle;
                        }

                        if (nSendData < SDO_DOWNLOAD_SEGMENT_MAX_DATA )
                        {
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Size       = SDO_DOWNLOAD_SEGMENT_MAX_DATA - nSendData;

#if defined WIN32
                            OsMemcpy( &(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.__Index),    /* use this to prevent microsoft compiler error */
                                      pData, 
                                      nSendData );
#else
                            OsMemcpy( ((EC_T_BYTE*)&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader) + SDO_HDR_INDEX_OFFSET, /* &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.Index to avoid diab compiler warning*/
                                      pData, 
                                      nSendData );
#endif
                        }
                        else
                        {
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.Size = 0;
                            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + nSendData - SDO_DOWNLOAD_SEGMENT_MAX_DATA));
#if defined WIN32
                            OsMemcpy( (&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.__Index),    /* use this to prevent microsoft compiler error */
                                      pData, 
                                      SDO_DOWNLOAD_SEGMENT_MAX_DATA );
#else
                            OsMemcpy( ((EC_T_BYTE*)&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader) + SDO_HDR_INDEX_OFFSET, /* &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.Index to avoid diab compiler warning*/
                                      pData, 
                                      SDO_DOWNLOAD_SEGMENT_MAX_DATA );
#endif
                        }
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Dsq.LastSeg    = nRemainLength <= nMaxData;

                        if (nSendData > SDO_DOWNLOAD_SEGMENT_MAX_DATA )
                        {
                            if (!(
#ifdef INCLUDE_FOE_SUPPORT
                                  (EC_NULL != m_pFoEDesc && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) || 
#endif
#ifdef INCLUDE_SOE_SUPPORT             
                                  (EC_NULL != m_pSoEDesc && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) || 
#endif
                                  m_bMbxCmdPending
                                 ))
                            {
                                EcatMbxSendCmdReq(bIsInitCmd, &pData[SDO_DOWNLOAD_SEGMENT_MAX_DATA], &mbx, &can, &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                                 ,EC_NULL
                                                 ,EC_NULL
#endif
                                                 );
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset += nSendData;
                            }
                            else
                            {
                                   /* save information for resend */
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcCanHeader = can;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcMbxHeader = mbx;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_TRUE;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePendingData = EC_TRUE;
                            }
                        }
                        else
                        {
                            if (!(
#ifdef INCLUDE_FOE_SUPPORT
                                  (EC_NULL != m_pFoEDesc && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) || 
#endif
#ifdef INCLUDE_SOE_SUPPORT             
                                  (EC_NULL != m_pSoEDesc && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) || 
#endif
                                  m_bMbxCmdPending
                                 ))
                            {
                                EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, &can, &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                                 ,EC_NULL
                                                 ,EC_NULL
#endif
                                                 );
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset += nSendData;
                            }
                            else
                            {
                                   /* save information for resend */
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcCanHeader = can;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcMbxHeader = mbx;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_TRUE;
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePendingData = EC_FALSE;
                            }
                        }
                    }
                    else
                    {   /* all data downloaded */
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_NOERROR);
                    }
                }
                break;
                case SDO_SCS_ABORT_TRANSFER:
                    {
                        EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse() '%s' (%d): WARNING: Got SDO_SCS_ABORT_TRANSFER as SDO Response! Expecting SDO Request with SDO Abort Code instead!\n", m_pEniSlave->szName, m_wFixedAddr));

                        /* release Mbx object with error code */
                        MailboxRes(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_SDO_ABORTCODE_UNKNOWN, 0, EC_NULL);

                        /* slave quit pending request, EC_E_NOERROR -> no abort request and no retries needed */
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_NOERROR);
                    }
                break;
                default:
                    OsDbgAssert(EC_FALSE);
                    break;
            }
        } /* else if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid ) */
    } /* if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDORES ) */
    else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDOREQ )
    {
        PEC_SDO_HDR pSdo = (PEC_SDO_HDR)EC_ENDOF(pCan);        
        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() ETHERCAT_CANOPEN_TYPE_SDOREQ. m_wMbxInitCmdsCurrent = %d this = 0x%x\n", m_wMbxInitCmdsCurrent, this));

        if (bIsInitCmd)
        {
            if (AreMbxInitCmdsPending())
            {
                switch (pSdo->uHdr.Idq.Ccs)
                {
                case SDO_CCS_ABORT_TRANSFER:
                    {   /* failed */

                        EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;

                        PEcMailboxCmdDesc pMbxCmdDesc = GetMbxInitCmdsCurrent();

                        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() '%s' (%d) '%s': CoE ('%s' 0x%04x:%d) - SDO Abort Code=0x%04x: '%s'.\n", 
                            m_pEniSlave->szName, m_wFixedAddr, GetStateChangeNameShort(m_wActTransition), CoESdoCcsText(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Ccs), EC_ECSDOHDR_GET_INDEX(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader)), 
                            pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex, EC_ECSDOHDR_GET_SDODATA(pSdo), EcMailboxCmdDescComment(pMbxCmdDesc)));

                        pNotification = m_pMaster->AllocNotification();
                        if (EC_NULL != pNotification)
                        {
                            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_MBSLAVE_COE_SDO_ABORT;
                            pNotification->uNotification.ErrorNotification.desc.SdoAbortDesc.dwErrorCode = EcConvertCoeErrorToEcError(EC_ECSDOHDR_GET_SDODATA(pSdo));
                            pNotification->uNotification.ErrorNotification.desc.SdoAbortDesc.wObjIndex   = EC_GET_FRM_WORD(&(pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.__Index));
                            pNotification->uNotification.ErrorNotification.desc.SdoAbortDesc.bySubIndex  = pMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex;

                            GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.SdoAbortDesc.SlaveProp));

                            m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_MBOX_SDO_ABORT);
                        }

                        m_pMaster->NotifySlaveCoEInitCmd(this, m_wActTransition, GetMbxInitCmdsCurrent(), EcConvertCoeErrorToEcError(EC_ECSDOHDR_GET_SDODATA(pSdo)), EC_NULL, 0);

                        /* don't continue sending init commands */
                        StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
                    }
                    break;
                case SDO_CCS_INITIATE_DOWNLOAD:
                case SDO_CCS_DOWNLOAD_SEGMENT:
                case SDO_CCS_INITIATE_UPLOAD:
                case SDO_CCS_UPLOAD_SEGMENT:

                    m_pMaster->NotifySlaveCoEInitCmd(this, m_wActTransition, GetMbxInitCmdsCurrent(), EC_E_NOERROR, EC_NULL, 0);

                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() %s: start next init command\n", m_pEniSlave->szName));
                    StartInitCmds(m_wActTransition, EC_FALSE);
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                } /* switch ( pSdo->Idq.Ccs ) */
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
        {
            EC_T_WORD wExpMbxSubType = m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType;
            EC_T_BYTE byActCcs = pSdo->uHdr.Idq.Ccs;

            /* Check if the CoE Mailbox is as expected a sdo response or sdo request or slave aborts current CoE cmd (e.g. SDO Info). */
            if (!((ETHERCAT_CANOPEN_TYPE_SDORES == wExpMbxSubType)
               || (ETHERCAT_CANOPEN_TYPE_SDOREQ == wExpMbxSubType)
               || (SDO_CCS_ABORT_TRANSFER == byActCcs)))
            {
                /* A not expected datagram was received. (e.g on frameloss) don't process this datagram */
                EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d):  A not expected datagram was received (type %d expected and %d read)! \n", m_pEniSlave->szName, m_wFixedAddr,
                    wExpMbxSubType, ETHERCAT_CANOPEN_TYPE_SDOREQ));
                goto Exit;
            }
            /* queued CANopen Mailbox command */
            if (SDO_CCS_ABORT_TRANSFER == byActCcs)
            {
                EC_T_DWORD result = EcConvertCoeErrorToEcError(EC_ECSDOHDR_GET_SDODATA(pSdo));
#ifdef DEBUGTRACE
                    EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() '%s' (%d): CoE ('%s' 0x%04x:%d) - SDO Abort Code=0x%04x.\n", 
                        m_pEniSlave->szName, m_wFixedAddr, CoESdoCcsText(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Ccs), 
                        EC_ECSDOHDR_GET_INDEX(&(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader)), m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.SubIndex, 
                        EC_ECSDOHDR_GET_SDODATA(pSdo)));
#endif
                StopCurrCoEMbxRequest(bIsInitCmd, result);
            }
        }
    } /* else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDOREQ) */
    else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDOINFO)
    {
        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() ETHERCAT_CANOPEN_TYPE_SDOINFO\n"));

        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
        {
            PETHERCAT_SDO_INFO_HEADER   pInfo = (PETHERCAT_SDO_INFO_HEADER)EC_ENDOF(pCan);
            EC_T_DWORD                  nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_CANOPEN_HEADER_LEN - EC_OFFSETOF(ETHERCAT_SDO_INFO_HEADER, uInfo.Data);          
            /* Check if the CoE Mailbox is the expected MbxSubType. */
            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType != EC_ECCOEHDR_GET_COETYPE(pCan))
            {
                EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEResponse()  '%s' (%d):  A not expected datagram was received (type %d expected and %d read)! \n", m_pEniSlave->szName, m_wFixedAddr,
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType, EC_ECCOEHDR_GET_COETYPE(pCan)));
                goto Exit;
            }
            else
            {
                if (pInfo->InComplete == EC_FALSE)
                {
                    /* last SDO information fragment received (not incomplete) */
                    switch ( pInfo->OpCode )
                    {
                    case ECAT_COE_INFO_OPCODE_LIST_S:
                    case ECAT_COE_INFO_OPCODE_OBJ_S:
                    case ECAT_COE_INFO_OPCODE_ENTRY_S:
                        if ( (EC_NULL != m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData))
                        {
                            if (nData + m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset <= m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length )
                            {
                                
                                OsMemcpy(
                                    &m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset], 
                                    &pInfo->uInfo.Data, 
                                    nData
                                        );

                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset += nData;
                                if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset <= m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length )
                                {
                                    StopCurrCoEMbxRequest(
                                        bIsInitCmd, 
                                        EC_E_NOERROR, 
                                        0, 
                                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset, 
                                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData
                                                         );
                                }
                                else
                                {
                                    StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_BLK_SIZE);
                                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                                    EC_ERRORMSGC(("ProcessCoEResponse() OpCode=%d EC_E_INVALIDSIZE: nOffset=%d > length=%d\n", 
                                        pInfo->OpCode, m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset, m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length));
                                }
                            }
                            else
                            {
                                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_BLK_SIZE);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                                    EC_ERRORMSGC(("ProcessCoEResponse() OpCode=%d EC_E_INVALIDSIZE: nOffset=%d > length=%d\n", 
                                        pInfo->OpCode, m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset, m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length));
                            }
                        }
                        else
                        {
                            if (nData <= m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length )
                            {
                                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_NOERROR, 0, nData, (EC_T_VOID*)&pInfo->uInfo.Data);
                            }
                            else
                            {
                                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_BLK_SIZE);
                                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                                EC_ERRORMSGC(("ProcessCoEResponse() OpCode=%d EC_E_INVALIDSIZE: nData=%d > length=%d\n", 
                                    nData, m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length));
                            }
                        }
                        break;
                    case ECAT_COE_INFO_OPCODE_ERROR_S:
                        {
                            EC_T_DWORD dwErrorCode = EcConvertCoeErrorToEcError(EC_SDOINFOERROR_GET_ERRORCODE(&pInfo->uInfo.Error));
                            StopCurrCoEMbxRequest(bIsInitCmd, dwErrorCode);
                        } break;
                    default:
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDPARM, SDO_ABORTCODE_CCS_SCS);
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDPARM;
                    }
                } /* if (pInfo->InComplete == EC_FALSE) */
                else
                {
                    /* SDO information fragments will follow (incomplete) */
                    if (EC_NULL != m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData )
                    {
                        if (nData + m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset <= m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length )
                        {
                            OsMemcpy(
                                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset], 
                                &pInfo->uInfo.Data, 
                                nData
                                );

                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset += nData;

                            /* trigger mailbox polling -> server may want to send more data */
                            AddMailboxActionRequestPoll();
                        }
                        else
                        {
                            StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE, SDO_ABORTCODE_BLK_SIZE);
                            m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_INVALIDSIZE;
                        }
                    }
                    else
                    {
                        StopCurrCoEMbxRequest(bIsInitCmd, EC_E_NOMEMORY, SDO_ABORTCODE_BLK_SIZE);
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOMEMORY;
                    }
                } /* else if (pInfo->InComplete == EC_FALSE) */
            } /* else if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType != EC_ECCOEHDR_GET_COETYPE(pCan)) */
        } /* if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid )  */
    } /* else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_SDOINFO ) */
#ifdef INCLUDE_COE_PDO_SUPPORT
    else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_TXPDO ) 
    {
    EC_T_BYTE*  pbyData = (EC_T_BYTE*)EC_ENDOF(pCan);
    EC_T_WORD   wLen = EC_ECMBOXHDR_GET_LENGTH(pMBox);
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    
        OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType == EC_ECCOEHDR_GET_COETYPE(pCan));
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType == EC_ECCOEHDR_GET_COETYPE(pCan))
        {
            if (wLen > MAX_EC_DATA_LEN )
            {
                wLen = MAX_EC_DATA_LEN;
            }
            wLen -= 2;  /* CoE header */
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.Notification.desc.TxPdoNtfyDesc.wPhysAddr = m_wFixedAddr;
                pNotification->uNotification.Notification.desc.TxPdoNtfyDesc.dwNumber = pCan->wCoeNumber;
                pNotification->uNotification.Notification.desc.TxPdoNtfyDesc.wLen = wLen;
                pNotification->uNotification.Notification.desc.RawCmdRespNtfyDesc.pbyData = pbyData;
                m_pMaster->NotifyAndFree( EC_NOTIFY_COE_TX_PDO, pNotification);
            }
        }
    } /* else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_TXPDO )  */
#endif
    else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_EMERGENCY ) 
    {
    PETHERCAT_EMERGENCY_HEADER  pEmgc = (PETHERCAT_EMERGENCY_HEADER)EC_ENDOF(pCan);

        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCoEResponse() '%s' (%d): CoE - Emergency (Hex: %04x, %02x, '%02x %02x %02x %02x %02x').\n", 
            m_pEniSlave->szName, m_wFixedAddr, EC_EMERGHDR_GET_ERRORCODE(pEmgc), pEmgc->ErrorRegister, pEmgc->Data[0], pEmgc->Data[1], pEmgc->Data[2], pEmgc->Data[3], pEmgc->Data[4]));

        PEcMailboxCmd pRes = m_pMaster->GetCoeEmergencyMbxCmd();
        EC_T_DWORD nData = sizeof(ETHERCAT_EMERGENCY_HEADER);
        
        if (EC_ECMBOXHDR_GET_LENGTH(pMBox) >= ETHERCAT_CANOPEN_HEADER_LEN)
        {
            nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_CANOPEN_HEADER_LEN;
        }

        if (pRes != EC_NULL)
        {
            pRes->dwMailboxFlags = 0;
            pRes->cmdId = 0;
            pRes->wMbxCmdType = EC_MAILBOX_CMD_UPLOAD;
            OsMemcpy(pRes->pbyData, pEmgc, sizeof(ETHERCAT_EMERGENCY_HEADER));
            pRes->dwInvokeId = m_wFixedAddr;
            m_pMaster->MailboxResponse(pRes, (EC_T_BYTE*)pEmgc, nData, EC_E_NOERROR); 
        }                   
    } /* else if (EC_ECCOEHDR_GET_COETYPE(pCan) == ETHERCAT_CANOPEN_TYPE_EMERGENCY )  */
    else
    {
        OsDbgAssert(EC_FALSE);
    }

Exit:
    return;
}

#ifdef INCLUDE_SOE_SUPPORT
/********************************************************************************/
/** \brief Process SoE response
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessSoEResponse
(   EC_T_BOOL bIsInitCmd,
    PETHERCAT_MBOX_HEADER pMBox )
{
    EC_SOE_HDR*    pSoeHeader   = EC_NULL;
    EcMailboxCmd*  pCmdRes      = EC_NULL;
    EC_T_DWORD              dwResult                  = 0;
    EC_T_MBXTFERP* pMbxTferPriv = EC_NULL;
    
    if (m_pSoEDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }

    pCmdRes = m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd;
    pSoeHeader = (PEC_SOE_HDR)EC_ENDOF(pMBox);
    
    if (EC_NULL != pCmdRes)
    {
        pMbxTferPriv = (EC_T_MBXTFERP*)(pCmdRes->pvContext);
    }

    if ((pSoeHeader->OpCode == ECAT_SOE_OPCODE_RRS) || (pSoeHeader->OpCode == ECAT_SOE_OPCODE_WRS))
    {
        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessSoEResponse() read/write response. m_wMbxInitCmdsCurrent = %d\n", m_wMbxInitCmdsCurrent));

        if (bIsInitCmd)
        {
            if (AreMbxInitCmdsPending())
            {
                /* SoE init command */
                switch (pSoeHeader->OpCode)
                {
                case ECAT_SOE_OPCODE_WRS:
                    {   
                    PEcMailboxCmdDesc pMbxCmdDesc = GetMbxInitCmdsCurrent();
                        
                        if (pSoeHeader->Error)
                        {
                            EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();
                            if (EC_NULL != pNotification)
                            {
                                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode  = EC_NOTIFY_SOE_WRITE_ERROR;
                                GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.SlaveProp));
                                SAFE_STRCPY( pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.achStateChangeName, 
                                    GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                                    MAX_SHORT_STRLEN );
                                pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.EErrorType = eInitCmdErr_FAILED;
                                SAFE_STRCPY(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.szComment, EcMailboxCmdDescComment(pMbxCmdDesc), EC_MIN(EC_ECMBXCMDDESC_GET_CMTLEN(pMbxCmdDesc), MAX_STD_STRLEN-1));
                                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR);
                            }
                            /* don't continue sending init commands */
                            StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
                        }
                        else
                        {
                            /* transfer complete -> next init cmd */
                            EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessSoEResponse() %s: start next init command\n", m_pEniSlave->szName));
                            StartInitCmds(m_wActTransition, EC_FALSE);
                        }
                    }
                    break;
                default:
                    break; /* can happen in case of a bad line */
                } /* switch( pSoeHeader->OpCode ) */
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid )
        {   
            /* The response is not a response to an init cmd, so check response according to the OpCode of the Request */
            switch (m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.OpCode)
            {
            case ECAT_SOE_OPCODE_WRQ: /* the previously sent request was a write request */
                {
                    /*Check if we received an response to the last write/fragment request*/
                    if (pSoeHeader->OpCode != ECAT_SOE_OPCODE_WRS)
                    {
                        /* This is not the expected OpCode. This can happen e.g. on FrameLoss and repeating mailbox communication */
                        goto Exit;
                    }
                    /* Check if the last wirte/fragment request was successful */
                    else if (pSoeHeader->Error) 
                    {
                        /* error is set in the response */
                        dwResult = EcConvertSoeErrorToEcError(EC_ECSOEHDR_GET_ERRORCODE(pSoeHeader));
                        StopCurrSoEMbxRequest(dwResult);
                    }
                    /* Check if the drive number is the same as the drive number in the request */
                    else if (pSoeHeader->DriveNo != pCmdRes->byDriveNo)
                    {
                        /* DriveNo not identical with the previous sent one */
                        dwResult = EC_E_SOE_ERRORCODE_DRIVE_NO;
                        StopCurrSoEMbxRequest(dwResult);
                        /* This should not happen if master and slave are iec conform. */
                        OsDbgAssert(EC_FALSE);
                    }
                    /* Check if the IDN is the same as the IDN in the request */
                    else if ((pSoeHeader->uIdnFragLeft.wIdn != pCmdRes->wIDN))
                    {
                        /* IDN not identical with the previous sent one */
                        dwResult = EC_E_SOE_ERRORCODE_IDN;
                        StopCurrSoEMbxRequest(dwResult);
                        /* This should not happen if master and slave are iec conform. */
                        goto Exit;
                    }
                    else
                    {
						if (EC_NULL != pMbxTferPriv)
						{
							pMbxTferPriv->MbxTfer.MbxData.SoE.byElementFlags = pSoeHeader->byElements;
						}
                        StopCurrSoEMbxRequest(EC_E_NOERROR);
                    }
                    break;
                }
            case ECAT_SOE_OPCODE_RRQ: /* the previously sent request was a read request */
                {
                    EC_T_BYTE*      pByte = pCmdRes->pbyData;
                    
                    /*Check if we received an response to the last read request*/
                    if (pSoeHeader->OpCode != ECAT_SOE_OPCODE_RRS)
                    {
                        /* This is not the expected OpCode. This can happen e.g. on FrameLoss and repeating mailbox communication */
                        goto Exit;
                    }
                    /* Check if the last read request was successful */
                    else if (pSoeHeader->Error) 
                    {
                        /* error is set in the response */
                        dwResult = EcConvertSoeErrorToEcError(EC_ECSOEHDR_GET_ERRORCODE(pSoeHeader));
                        StopCurrSoEMbxRequest(dwResult);
                    }
                    /* Check if the drive number is the same as the drive number in the request */
                    else if (pSoeHeader->DriveNo != pCmdRes->byDriveNo)
                    {
                        /* response drive number not identical with the requested drive number */
                        dwResult = EC_E_SOE_ERRORCODE_DRIVE_NO;
                        StopCurrSoEMbxRequest(dwResult);
                        /* This should not happen if master and slave are iec conform. */
                        OsDbgAssert(EC_FALSE);
                    }
                    /* Check if there is space in the Rx-Buffer */
                    else if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset >= pCmdRes->length)
                    {
                        /* Rx-buffer is full */
                        dwResult = EC_E_SOE_ERRORCODE_BUFFER_FULL;
                        StopCurrSoEMbxRequest(dwResult);
                        OsDbgAssert(EC_FALSE);
                    }
                    else
                    {
                        EC_T_DWORD nAct  = EC_MIN( EC_ECMBOXHDR_GET_LENGTH(pMBox) - EC_SOE_HDR_LEN, pCmdRes->length - m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset);
                     
                        /* Check if not last fragment */
                        if (pSoeHeader->Incomplete)
                        {
                            /* If this is the first fragment, the FragmentsLeft number is saved */
                            if (0 == m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft)
                            {
                                m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft = pSoeHeader->uIdnFragLeft.wFragmentsLeft;
                                OsMemcpy(&pByte[m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset], EC_ENDOF(pSoeHeader), nAct);
                                m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset += nAct;
                            }
                            /* Else we decrement the FragmentsLeft Counter */
                            else if ((pSoeHeader->uIdnFragLeft.wFragmentsLeft+1) == m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft)
                            {
                                m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft--;
                                OsMemcpy(&pByte[m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset], EC_ENDOF(pSoeHeader), nAct);
                                m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset += nAct;
                            }
                            /* Check fragments, if an fragment was lost or the last fragment was received an second time*/
                            else if (pSoeHeader->uIdnFragLeft.wFragmentsLeft != m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft )
                            {
                                dwResult = EC_E_SOE_ERRORCODE_FRAGMENT_LOST;
                                StopCurrSoEMbxRequest(dwResult);
                            }
                        }
                        else
                        {
                            /* Check fragments, if an fragment was lost */
                            if (1 < m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft)
                            {
                                dwResult = EC_E_SOE_ERRORCODE_FRAGMENT_LOST;
                                StopCurrSoEMbxRequest(dwResult);
                                goto Exit;
                            }
                            /* Check if the IDN is the same as the IDN in the request */
                            else if ((pSoeHeader->uIdnFragLeft.wIdn != pCmdRes->wIDN))
                            {
                                /* IDN not identical with the previous sent one */
                                dwResult = EC_E_SOE_ERRORCODE_IDN;
                                StopCurrSoEMbxRequest(dwResult);
                                /* This should not happen if master and slave are iec conform. */
                                OsDbgAssert(EC_FALSE);
                            }
                            else
                            {
                                /* Last fragment was received without errors */
                                OsMemcpy(&pByte[m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset], EC_ENDOF(pSoeHeader), nAct);
                                m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset += nAct;
								if (EC_NULL != pMbxTferPriv)
								{
									pMbxTferPriv->MbxTfer.MbxData.SoE.byElementFlags = pSoeHeader->byElements;
								}
                                StopCurrSoEMbxRequest(EC_E_NOERROR, m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset, pByte);
                            }
                        }
                    }
                }
                break;
            default:
                OsDbgAssert(EC_FALSE);    /* not yet implemented*/
                break;
            }
        } /* else if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid ) */
        
    } /* if ((pSoeHeader->OpCode == ECAT_SOE_OPCODE_RRS) || (pSoeHeader->OpCode == ECAT_SOE_OPCODE_WRS)) */
    
    /* Notify SSC Command */
    else if (pSoeHeader->OpCode == ECAT_SOE_OPCODE_NOTIFY) 
    {
        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessSoEResponse() '%s' (%d): SoE - Notification \n", 
            m_pEniSlave->szName, m_wFixedAddr ));
        
        PEcMailboxCmd pRes = m_pMaster->GetSoeNotificationMbxCmd();
        EC_T_DWORD nData = sizeof(ETHERCAT_SOE_NOTIFICATION_HEADER);
        
        if (EC_ECMBOXHDR_GET_LENGTH(pMBox) >= EC_SOE_HDR_LEN)
        {
            nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - EC_SOE_HDR_LEN;
        }
        
        if (pRes != EC_NULL)
        {
            pRes->dwMailboxFlags = 0;
            pRes->cmdId = 0;
            pRes->wMbxCmdType = EC_MAILBOX_CMD_UPLOAD;
            OsMemcpy(pRes->pbyData, pSoeHeader, sizeof(ETHERCAT_SOE_NOTIFICATION_HEADER));
            pRes->dwInvokeId = m_wFixedAddr;
            m_pMaster->MailboxResponse(pRes, (EC_T_BYTE*)pSoeHeader, nData, EC_E_NOERROR); 
        }                   
    }
    /* Notify Emergency */
    else if (pSoeHeader->OpCode == ECAT_SOE_OPCODE_SLV_INFO) 
    {
        EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessSoEResponse() '%s' (%d): SoE - Emergency \n", 
            m_pEniSlave->szName, m_wFixedAddr ));
        
        PEcMailboxCmd pRes = m_pMaster->GetSoeEmergencyMbxCmd();
        EC_T_DWORD nData = sizeof(ETHERCAT_SOE_EMERGENCY_HEADER);
        
        if (EC_ECMBOXHDR_GET_LENGTH(pMBox) >= EC_SOE_HDR_LEN)
        {
            nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - EC_SOE_HDR_LEN;
        }
        
        if (pRes != EC_NULL)
        {
            pRes->dwMailboxFlags = 0;
            pRes->cmdId = 0;
            pRes->wMbxCmdType = EC_MAILBOX_CMD_UPLOAD;
            OsMemcpy(pRes->pbyData, pSoeHeader, sizeof(ETHERCAT_SOE_EMERGENCY_HEADER));
            pRes->dwInvokeId = m_wFixedAddr;
            m_pMaster->MailboxResponse(pRes, (EC_T_BYTE*)pSoeHeader, nData, EC_E_NOERROR); 
        }                   
    }
    else
    {
        OsDbgAssert(EC_FALSE);
    }
    
Exit:
    return;
}
#endif

#if (defined INCLUDE_FOE_SUPPORT)
/********************************************************************************/
/** \brief Process FoE response
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessFoEResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox)
{
    if ((EC_NULL == m_pFoEDesc) || (!m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid))
        return;

    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bRead)
    {
        ProcessFoEReadResponse(bIsInitCmd, pMBox);
    }
    else
    {
        ProcessFoEWriteResponse(bIsInitCmd, pMBox);
    }
}

EC_T_VOID CEcMbSlave::SendFoEAckToSlave(EC_T_BOOL bIsInitCmd, EC_T_DWORD dwPacketNo)
{
    ETHERCAT_MBOX_HEADER mbx;
    EC_FOE_HDR           FoE;

    MBXHDR_CLR(&mbx);
    FOEHDR_CLR(&FoE);
    EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);
    EC_ECMBOXHDR_SET_LENGTH(&mbx, EC_FOE_HDR_LEN);
    EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_ACK);
    EC_ECFOEHDR_SET_PACKETNO(&FoE, dwPacketNo);
    m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;
    
    /* Sending ACK to slave */
    m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType = ECAT_FOE_OPCODE_ACK;
    if (!((EC_NULL != m_pCoEDesc && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry) || 
#ifdef INCLUDE_SOE_SUPPORT             
        (EC_NULL != m_pSoEDesc && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) || 
#endif
        m_bMbxCmdPending))

    {
        EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                         ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                         ,EC_NULL
                         ,EC_NULL
#endif
                         );
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsDefaultTimeout())
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Restart();
        }
    }
    else
    {
        /* save information for resend */
        OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
        OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending = EC_TRUE;
    }
}

EC_T_VOID CEcMbSlave::ProcessFoEReadResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox)
{
    EC_T_DWORD          dwRes = EC_E_ERROR;
    PEC_FOE_HDR         pFoE  = (PEC_FOE_HDR)EC_ENDOF(pMBox);
    PEcMailboxCmd       pCmdRes = m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd;
    EC_T_MBXTFERP*      pMbxTferPriv = EC_NULL;

    if (EC_NULL != pCmdRes)
    {
        pMbxTferPriv = (EC_T_MBXTFERP*)(pCmdRes->pvContext);
    }

    switch (pFoE->OpCode)
    {
    case ECAT_FOE_OPCODE_BUSY:
        /* no break because data will be sent a second time. */
    case ECAT_FOE_OPCODE_DATA:
        {
            EC_T_BOOL bReceivedNextPacket = (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber + 1 == EC_ECFOEHDR_GET_PACKETNO(pFoE)) && 
                    (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber == m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber) &&
                    (ECAT_FOE_OPCODE_DATA == pFoE->OpCode);

            EC_T_BOOL bReceivedLastPacketAgain = (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber == EC_ECFOEHDR_GET_PACKETNO(pFoE)) &&
                    (ECAT_FOE_OPCODE_DATA == pFoE->OpCode);

            /* process data or busy */
            if (bReceivedNextPacket || bReceivedLastPacketAgain || (ECAT_FOE_OPCODE_BUSY == pFoE->OpCode))
            {
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.nRetData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - EC_FOE_HDR_LEN;

                /* process new data */
                if (bReceivedNextPacket)
                {
                    /* read data and save it */
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber = EC_ECFOEHDR_GET_PACKETNO(pFoE);
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bLastRead = (m_pFoEDesc->CurrPendFoeMbxCmdDesc.nRetData < GetFoEDataSize(VG_IN));
                    
                    if (EC_NULL != pCmdRes)
                    {
                        EC_T_DWORD dwMemcpyLen = EC_MIN(m_pFoEDesc->CurrPendFoeMbxCmdDesc.nRetData, pCmdRes->length - m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset);
                        
                        OsMemcpy(&pCmdRes->pbyData[m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset], EC_ENDOF(pFoE), dwMemcpyLen);

                        pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes += dwMemcpyLen;
                        if (eMbxTferType_FOE_SEG_UPLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
                        {
                            pMbxTferPriv->MbxTfer.dwDataLen = dwMemcpyLen;
                        }
                        else
                        {
                            if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bLastRead)
                            {
                                pMbxTferPriv->MbxTfer.dwDataLen = pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes;
                            }
                        }

                        /* buffer exceeding handling */
                        if (dwMemcpyLen < m_pFoEDesc->CurrPendFoeMbxCmdDesc.nRetData)
                        {
                            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_ERRCODE_MAX_FILE_SIZE, ECAT_FOE_ERRCODE_DISKFULL);
                            break;
                        }

                        /* notify current transfer status. MbxResult EC_E_BUSY to keep MbxTfr pending for ACK. */
                        dwRes = m_pMaster->MailboxResponse(pCmdRes, EC_NULL, m_pFoEDesc->CurrPendFoeMbxCmdDesc.nRetData, EC_E_BUSY);
                        if (EC_E_CANCEL == dwRes)
                        {
                            StopCurrFoEMbxRequest(bIsInitCmd, dwRes, 0);
                            break;
                        }

                        m_pMaster->NotifyMbxTferProgress(pMbxTferPriv);

                        if (eMbxTferType_FOE_SEG_UPLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
                        {
                            FoeSegmentedUploadSetSegmentDone(pMbxTferPriv);
                        }

                        /* skip ahead in application's FoE buffer */
                        if (eMbxTferType_FOE_FILE_UPLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
                        {
                            OsDbgAssert(pMbxTferPriv->MbxTfer.eTferStatus == eMbxTferStatus_Pend);
                            if (!m_pFoEDesc->CurrPendFoeMbxCmdDesc.bLastRead)
                            {
                                m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset += dwMemcpyLen;
                            }
                        }
                    }
                }
                /* received last packet again */
                else if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber == EC_ECFOEHDR_GET_PACKETNO(pFoE) &&
                         ECAT_FOE_OPCODE_BUSY != pFoE->OpCode)
                {
                    EC_ERRORMSGC(("CEcMbSlave::ProcessFoEReadResponse() ECAT_FOE_OPCODE_DATA nPacketNo (%d) received twice!\n", 
                        m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber));
                }

                /* Send ACK for ECAT_FOE_OPCODE_DATA, ECAT_FOE_OPCODE_BUSY. Segmented upload handshaked with application! */
                if ((eMbxTferType_FOE_FILE_UPLOAD == pMbxTferPriv->MbxTfer.eMbxTferType) || (ECAT_FOE_OPCODE_BUSY == pFoE->OpCode))
                {
                    EC_T_DWORD dwPacketNo = EC_ECFOEHDR_GET_PACKETNO(pFoE);
                    if (ECAT_FOE_OPCODE_BUSY == pFoE->OpCode)
                    {
                        dwPacketNo = m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber;
                    }

                    SendFoEAckToSlave(bIsInitCmd, dwPacketNo);
                }
            }
            else
            {
                /* invalid packetno */
                StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_ERRCODE_PACKENO, ECAT_FOE_ERRCODE_PACKENO);
                break;
            }
        } break;

    case ECAT_FOE_OPCODE_ERR:
        {
            EC_T_DWORD                  result              = EC_E_ERROR;
            EC_T_NOTIFICATION_INTDESC*  pNotification       = EC_NULL;   
            EC_T_DWORD                  nData               = EC_ECMBOXHDR_GET_LENGTH(pMBox) - EC_FOE_HDR_LEN;

            /* Allocate notification memory and save notification data */
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_FOE_MBSLAVE_ERROR;
                pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.dwErrorCode = EC_ECFOEHDR_GET_ERRORCODE(pFoE);
                GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.SlaveProp));
                /* The sting must and with a null character */
                OsMemset(pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.achErrorString, 0, MAX_STD_STRLEN);
                /* Copy error string */
                OsStrncpy(pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.achErrorString, EC_ENDOF(pFoE), EC_MIN(nData, MAX_STD_STRLEN-1));
                /* Generate Notification */
                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_MBOX_FOE_ABORT);
            }

            result = EcConvertFoeErrorToEcError(EC_ECFOEHDR_GET_ERRORCODE(pFoE));
            StopCurrFoEMbxRequest(bIsInitCmd, result);
        } break;
    default:
        StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_ERRCODE_ILLEGAL, ECAT_FOE_ERRCODE_ILLEGAL);
    }
}

EC_T_VOID CEcMbSlave::ProcessFoEWriteResponse(EC_T_BOOL bIsInitCmd, PETHERCAT_MBOX_HEADER pMBox)
{
    EC_T_DWORD  dwRes = EC_E_ERROR;
    PEC_FOE_HDR pFoE  = (PEC_FOE_HDR)EC_ENDOF(pMBox);
    EC_T_MBXTFERP* pMbxTferPriv = (EC_T_MBXTFERP*)m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pvContext;
    EC_T_BOOL bLastChunkSent = EC_FALSE;

    EC_T_BOOL bCoeWkcRetry = EC_FALSE;
    EC_T_BOOL bSoeWkcRetry = EC_FALSE;

    if ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)
    {
        bCoeWkcRetry = EC_TRUE;
    }
#ifdef INCLUDE_SOE_SUPPORT             
    if ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry)
    {
        bSoeWkcRetry = EC_TRUE;
    }
#endif

    /* data len from last DATA REQ */
    EC_T_WORD wLastDataLen = 0;
    if (EC_ECMBOXHDR_GET_LENGTH(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader) > (EC_T_WORD)EC_FOE_HDR_LEN)
    {
        wLastDataLen = (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader) - EC_FOE_HDR_LEN);
    }
    bLastChunkSent = (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend && (wLastDataLen < GetFoEDataSize(VG_OUT)));

    switch (EC_ECFOEHDR_GET_OPCODE(pFoE))
    {
    case ECAT_FOE_OPCODE_BUSY:
        {
            if (!m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend)
            {
                EC_ERRORMSGC_L(("CEcMbSlave::ProcessFoEWriteResponse(): Got BUSY before DATA!\n"));
                StopCurrFoEMbxRequest(bIsInitCmd, EC_E_INVALIDDATA, ECAT_FOE_ERRCODE_PACKENO);
                break;
            }

            /* fill busy info from returning cmd */
            pMbxTferPriv->MbxTfer.MbxData.FoE.dwBusyDone = EC_ECFOEHDR_GET_STATUSDONE(pFoE);
            pMbxTferPriv->MbxTfer.MbxData.FoE.dwBusyEntire = EC_ECFOEHDR_GET_STATUSENTIRE(pFoE);
            OsStrncpy(pMbxTferPriv->MbxTfer.MbxData.FoE.szBusyComment, EC_ECFOEHDR_GET_BUSYCOMMENT(pFoE), EC_FOE_BUSY_COMMENT_SIZE - 1);
            pMbxTferPriv->MbxTfer.MbxData.FoE.szBusyComment[EC_FOE_BUSY_COMMENT_SIZE - 1] = '\0';

            if (pMbxTferPriv->bAbort)
            {
                StopCurrFoEMbxRequest(bIsInitCmd, EC_E_CANCEL);
                break;
            }

            /* send last data again */
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber = m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber;
            if (eMbxTferType_FOE_SEG_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
            {
                /* same as if new data was provided with Continue */
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled = EC_TRUE;
                break;
            }
        } /* no break */ /* no break because data will be sent a second time. */
    case ECAT_FOE_OPCODE_ACK:
        {
            if (ECAT_FOE_OPCODE_BUSY != pFoE->OpCode)
            {
                /* reset busy info */
                pMbxTferPriv->MbxTfer.MbxData.FoE.dwBusyDone = 0;
                pMbxTferPriv->MbxTfer.MbxData.FoE.dwBusyEntire = 0;
                OsMemset(pMbxTferPriv->MbxTfer.MbxData.FoE.szBusyComment, 0, EC_FOE_BUSY_COMMENT_SIZE);
            }

            /* can't be BUSY here for segmented download */
            if (eMbxTferType_FOE_SEG_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
            {
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber = m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber;
                if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend)
                {
                    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 
                        pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes + wLastDataLen;
                }

                m_pMaster->NotifyMbxTferProgress(pMbxTferPriv);
                if (bLastChunkSent)
                {
                    StopCurrFoEMbxRequest(bIsInitCmd, EC_E_NOERROR);
                    break;
                }

                FoeSegmentedDownloadSetSegmentDone(pMbxTferPriv);
                if (pMbxTferPriv->bAbort)
                {
                    StopCurrFoEMbxRequest(bIsInitCmd, EC_E_CANCEL);
                }
                break;
            }

            /* */
            if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend)
            {   /* coming from a DATA (not OPEN) command */
                if ((EC_ECFOEHDR_GET_PACKETNO(pFoE) == m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber) || (ECAT_FOE_OPCODE_BUSY == pFoE->OpCode))
                {
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend = EC_FALSE;

                    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 
                        m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset + wLastDataLen;

                    /* notify current transfer status */
                    dwRes = m_pMaster->MailboxResponse(m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_NULL, EC_ECMBOXHDR_GET_LENGTH(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader) - ETHERCAT_MBOX_HEADER_LEN, EC_E_BUSY);
                    if (EC_E_CANCEL == dwRes)
                    {
                        StopCurrFoEMbxRequest(bIsInitCmd, dwRes);
                        break;
                    }
                    else
                    {
                        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber == m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber + 1)
                        {
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset += GetFoEDataSize(VG_OUT);
                        }

                        /* send data */
                        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset)
                        {   /* send next data packet */
                            EC_T_BYTE*              pByte = m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData;
                            ETHERCAT_MBOX_HEADER    mbx;
                            EC_FOE_HDR     FoE;
                            MBXHDR_CLR(&mbx);
                            FOEHDR_CLR(&FoE);
                            EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);

                            if (ECAT_FOE_OPCODE_BUSY == pFoE->OpCode)
                            {
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, EC_ECMBOXHDR_GET_LENGTH(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader));
                            }
                            /* send full mailbox */
                            else if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= GetFoEDataSize(VG_OUT)+m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset )
                            {
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + GetFoEDataSize(VG_OUT)));
                            }
                            /* send less mailbox */
                            else if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length > m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset)
                            {
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length - m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset));
                            }
                            /* send zero mailbox */
                            else if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length == m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset)
                            {
                                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN));
                            }

                            EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_DATA);
                            EC_ECFOEHDR_SET_PACKETNO(&FoE, (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber + 1));
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend      = EC_TRUE;
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;
                            /*m_pFoEDesc->CurrPendFoeMbxCmdDesc.timeout.Start(m_pMaster->m_MasterConfig.dwFoeTimeout, m_pMaster->GetMsecCounterPtr());*/
                            OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                            OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));

                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType = ECAT_FOE_OPCODE_DATA;
                            if (!bCoeWkcRetry && !bSoeWkcRetry && !m_bMbxCmdPending)
                            {
                                dwRes = EcatMbxSendCmdReq(bIsInitCmd, &pByte[m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset], &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                             ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                             ,EC_NULL
                                             ,EC_NULL
#endif
                                             );
                                if (EC_E_NOERROR != dwRes)
                                {
                                    EC_ERRORMSGC_L(("CEcMbSlave::ProcessFoEWriteResponse(): ERROR send next data packet! Result is 0x%lx\n", dwRes));
                                }
                                if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsDefaultTimeout())
                                {
                                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Restart();
                                }
                            }
                            else
                            {
                                /* send later */
                                m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending = EC_TRUE;
                            }
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber = m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber;
                        }
                        /* all data send stop mailbox */
                        else
                        {
                            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_NOERROR, 0, m_pFoEDesc->CurrPendFoeMbxCmdDesc.nRetData, m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData);
                        }
                    }
                }
                else
                {
                    /* do nothing if received last packet number again (Repeating Mailbox Communication) */
                    if (EC_ECFOEHDR_GET_PACKETNO(pFoE) == (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber - 1))
                    {
                        break;
                    }

                    StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_ERRCODE_PACKENO, ECAT_FOE_ERRCODE_PACKENO);
                }
            }
            else /* !m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend */
            {   /* coming from an open command -> response with file handle */
                if (EC_ECFOEHDR_GET_PACKETNO(pFoE) == 0)
                {
                    /* send first segment */
                    ETHERCAT_MBOX_HEADER    mbx;
                    EC_FOE_HDR              FoE;
                    EC_T_BYTE*              pByte = m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData;
                    
                    MBXHDR_CLR(&mbx);
                    FOEHDR_CLR(&FoE);
                    {
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length - m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset));
                        /* if send data >= mailbox  */
                        if ( m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= GetFoEDataSize(VG_OUT)+m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset )
                        {
                            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + GetFoEDataSize(VG_OUT)));
                        }
                        else
                        {
                            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length - m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset));
                        }
                    }
                    EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);
                    EC_ECFOEHDR_SET_PACKETNO(&FoE, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber + 1);

                    EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_DATA);
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend = EC_TRUE;
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;

                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType = ECAT_FOE_OPCODE_DATA;
                    if (!bCoeWkcRetry && !bSoeWkcRetry && !m_bMbxCmdPending)
                    {
                        dwRes = EcatMbxSendCmdReq(bIsInitCmd, &pByte[0], &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,EC_NULL
                                     ,EC_NULL
#endif
                                         );
                        if (EC_E_NOERROR != dwRes)
                        {
                            EC_ERRORMSGC_L(("CEcMbSlave::ProcessFoEWriteResponse(): ERROR send next data packet! Result is 0x%lx\n", dwRes));
                        }
                        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsDefaultTimeout())
                        {
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Restart();
                        }
                    }
                    else
                    {
                        /* send later */
                        m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending = EC_TRUE;
                    }
                }
                else
                {
                    StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_ERRCODE_PACKENO, ECAT_FOE_ERRCODE_PACKENO);
                }
            }
        } break;
    case ECAT_FOE_OPCODE_ERR:
        {
            EC_T_DWORD                  result              = EC_E_ERROR;
            EC_T_NOTIFICATION_INTDESC*  pNotification       = EC_NULL;   
            EC_T_DWORD                  nData               = EC_ECMBOXHDR_GET_LENGTH(pMBox) - EC_FOE_HDR_LEN;

            EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::ProcessFoEWriteResponse() FoE ('%s': Server sends an ERROR response %d\n", m_pEniSlave->szName, EC_ECFOEHDR_GET_ERRORCODE(pFoE)));
            
            /* Allocate notification memory and save notification data */
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_FOE_MBSLAVE_ERROR;
                pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.dwErrorCode = EC_ECFOEHDR_GET_ERRORCODE(pFoE);
                GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.SlaveProp));
                /* The sting must and with a null character */
                OsMemset(pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.achErrorString, 0, MAX_STD_STRLEN);
                /* Copy error string */
                OsStrncpy(pNotification->uNotification.ErrorNotification.desc.FoeErrorDesc.achErrorString, EC_ENDOF(pFoE), EC_MIN(nData, MAX_STD_STRLEN-1));
                /* Generate Notification */
                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_MBOX_FOE_ABORT);
            }

            result = EcConvertFoeErrorToEcError(EC_ECFOEHDR_GET_ERRORCODE(pFoE));
            StopCurrFoEMbxRequest(bIsInitCmd, result);
        } break;
    default:
        {
            OsDbgAssert(EC_ECFOEHDR_GET_OPCODE(pFoE) != ECAT_FOE_OPCODE_WRQ);
            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_ERRCODE_ILLEGAL, ECAT_FOE_ERRCODE_ILLEGAL);
        }
    } /* switch (EC_ECFOEHDR_GET_OPCODE(pFoE)) */
}
#endif /* INCLUDE_FOE_SUPPORT */

#ifdef INCLUDE_AOE_SUPPORT

/********************************************************************************/
/** \brief Gets the AoE NetID
*
* \return
*/
EC_T_DWORD CEcMbSlave::GetNetId(EC_T_AOE_NETID* poAoeNetId)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_AOE_NETID oUnInitNetId = {{0,0,0,0,0,0}};

    if (poAoeNetId == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if (m_pAoEDesc != EC_NULL)
    {
        OsMemcpy(poAoeNetId, &m_pAoEDesc->oAoeNetId, sizeof(EC_T_AOE_NETID));
    }
    else
    {
        dwRetVal = EC_E_NO_AOE_SUPPORT;
        goto Exit;
    }

    if (OsMemcmp((EC_T_VOID*)m_pAoEDesc->oAoeNetId.aby, (EC_T_VOID*)oUnInitNetId.aby, sizeof(EC_T_AOE_NETID)) == 0 )
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;        
    }

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Process AoE response
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessAoEResponse
(   EC_T_BOOL bIsInitCmd,
    PETHERCAT_MBOX_HEADER pMBox )
{
    PEC_AOE_HDR    pAoeHeader;
    PEcMailboxCmd           pCmdRes;
    
    if (m_pAoEDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }

    pCmdRes = m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd;
    pAoeHeader = (PEC_AOE_HDR)EC_ENDOF(pMBox);

    if (!bIsInitCmd)
    {
        if (!m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
        {
            EC_ERRORMSGC_L(("CEcMbSlave::ProcessAoEResponse()  m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid == FALSE \n"));
            goto Exit;
        }
        /* check if this known AoE command */
        switch (EC_AOE_HDR_GET_CMD_ID(pAoeHeader))
        {
        case ECAT_AOEHDR_COMMANDID_READ:
        case ECAT_AOEHDR_COMMANDID_WRITE:
        case ECAT_AOEHDR_COMMANDID_WRITECONTROL:
        case ECAT_AOEHDR_COMMANDID_READWRITE:
            /* supported AoE command */
            break;
        case ECAT_AOEHDR_COMMANDID_FRAGMENTATION:
            {
                EC_ERRORMSGC(("Received AoE fragment discarded! AoE fragmentation is not yet supported!\n")); 
                StopCurrAoEMbxRequest(EC_E_NOTSUPPORTED);
                goto Exit;
            } /* no break due to goto Exit */
        default:
            {
                EC_ERRORMSGC(("Unsupported AoE command received! Invoke ID: %d, AoE command 0x%x\n", 
                    EC_AOE_HDR_GET_INVOKE_ID(pAoeHeader), EC_AOE_HDR_GET_CMD_ID(pAoeHeader))); 
                goto Exit;
            }
        }

        /* check if this is a expected response */
        if (!m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid || (EC_AOE_HDR_GET_INVOKE_ID(pAoeHeader) != m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader.dwAoeInvokeId))
        {
            EC_ERRORMSGC(("Unexpected AoE response received (discarded). Expected AoE invoke ID 0x%x / Received AoE invoke ID 0x%x\n", m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader.dwAoeInvokeId, 
                EC_AOE_HDR_GET_INVOKE_ID(pAoeHeader))); 
            goto Exit;
        }

        /* check if this is a AoE response from */
        if ((EC_AOE_HDR_GET_STATE_FLAGS(pAoeHeader) & EC_ECAOEHDR_STATEFLAG_RES) == 0)
        {
            EC_ERRORMSGC(("AoE header response flag not set! (AoE invoke ID = %d)\n", EC_AOE_HDR_GET_INVOKE_ID(pAoeHeader)));
            goto Exit;            
        }
    }
    
    EC_TRACEMSG(EC_TRACE_CORE | EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessAoEResponse() read/write response. m_wMbxInitCmdsCurrent = %d\n", m_wMbxInitCmdsCurrent));
    if (bIsInitCmd)
    {
        if (AreMbxInitCmdsPending())
        {
            if (EC_AOE_HDR_GET_CMD_ID(pAoeHeader) == ECAT_AOEHDR_COMMANDID_WRITE)
            {
                /* PETHERCAT_AOE_WRITE_RES_HEADER pWriteResHeader = (PETHERCAT_AOE_WRITE_RES_HEADER)EC_ENDOF(pAoeHeader); */

                /* check the size of the AoE response command */
                if (EC_AOE_HDR_GET_DATA_SIZE(pAoeHeader) != sizeof(ETHERCAT_AOE_WRITE_RES_HEADER))
                {
                    EC_ERRORMSGC(("Warning: Invalid AoE init command response data size (expected %d byte length, received %d byte length)!\n",sizeof(ETHERCAT_AOE_WRITE_RES_HEADER), 
                        EC_AOE_HDR_GET_DATA_SIZE(pAoeHeader)));

                    /* we warn the user but dont stop the init commands */
                    /* StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );  */
                }

                if (EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader) != 0 /*|| pWriteResHeader->dwResult != 0*/ )
                {
                    m_pMaster->NotifySlaveInitCmdResponseError(this, m_wActTransition, eInitCmdErr_VALIDATION_ERR, 
                        EcMailboxCmdDescComment(GetMbxInitCmdsCurrent()), EC_ECMBXCMDDESC_GET_CMTLEN(GetMbxInitCmdsCurrent()));

                    EC_ERRORMSGC_L(("CEcSlave::ProcessCmdResult() '%s' (%d): '%s' failed! Error: '%d'. \n", m_pEniSlave->szName, m_wFixedAddr, 
                        GetStateChangeNameShort(m_wActTransition), EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR));

                    /* don't continue sending init commands */
                    StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
                }
                else
                {
                    /* transfer complete -> next init cmd */
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessAoEResponse() %s: start next init command\n", m_pEniSlave->szName));
                    StartInitCmds(m_wActTransition, EC_FALSE);
                }
            }
            else
            {
                EC_ERRORMSGC(("ProcessAoEResponse: Init command read response received though not supported!\n"));
                OsDbgAssert(EC_FALSE);
            }
        }
        else
        {
            OsDbgAssert(EC_FALSE);
            StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
        }
    }
    else 
    /* !bIsInitCmd */
    {
        switch (EC_AOE_HDR_GET_CMD_ID(pAoeHeader))
        {
        case ECAT_AOEHDR_COMMANDID_WRITE:
        {
            EC_T_DWORD dwRetVal = EC_E_ERROR;
            PETHERCAT_AOE_WRITE_RES_HEADER pAoeWriteResHdr = (PETHERCAT_AOE_WRITE_RES_HEADER)EC_ENDOF(pAoeHeader);

            pCmdRes->dwAoeErrorCode = EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader);

            /* check the response error (AoE header) */
            if (EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader) == 0)
            {
                 /* check response size */
                if (EC_AOE_HDR_GET_DATA_SIZE(pAoeHeader) >= sizeof(ETHERCAT_AOE_WRITE_RES_HEADER))
                {
                    pCmdRes->dwAoeCmdResult = pAoeWriteResHdr->dwResult;
                    dwRetVal = EcConvertAdsErrorToEcError(pAoeWriteResHdr->dwResult);
                    StopCurrAoEMbxRequest(dwRetVal);
                }
                else
                {
                    StopCurrAoEMbxRequest(EC_E_AOE_INV_RESPONSE_SIZE);
                }
            }
            else
            {   /* response frame contains an error, therefore we dont consider the data */
                dwRetVal = EcConvertAdsErrorToEcError(EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader));
                StopCurrAoEMbxRequest(dwRetVal);
            }
        } break;
        case ECAT_AOEHDR_COMMANDID_READ:
        case ECAT_AOEHDR_COMMANDID_READWRITE:
        {
            /* same response for READ and READWRITE */
            EC_T_DWORD dwRetVal = EC_E_ERROR;
            ETHERCAT_AOE_READ_RES_HEADER* pAoeReadResHdr = (ETHERCAT_AOE_READ_RES_HEADER*)EC_ENDOF(pAoeHeader);
            EC_T_BYTE* pData = (EC_T_BYTE*)EC_ENDOF(pAoeReadResHdr);

            pCmdRes->dwAoeErrorCode = EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader);

            /* check the response error (AoE header) */
            if (EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader) == 0)
            {
                /* check response size */
                if (EC_AOE_HDR_GET_DATA_SIZE(pAoeHeader) == sizeof(ETHERCAT_AOE_READ_RES_HEADER) + pAoeReadResHdr->dwReadLength)
                {
                    pCmdRes->dwAoeCmdResult = pAoeReadResHdr->dwResult;

                    /* check the command result (AoE response header) */
                    if (pAoeReadResHdr->dwResult == 0)
                    {
                        if (EC_AOE_HDR_GET_CMD_ID(pAoeHeader) == ECAT_AOEHDR_COMMANDID_READWRITE)
                        {
                            pCmdRes->length = pAoeReadResHdr->dwReadLength;
                        }
                        /* the mbx data will be copied in CAtEmInterface::MboxNotify */
                        StopCurrAoEMbxRequest(EC_E_NOERROR, EC_MIN(pAoeReadResHdr->dwReadLength, pCmdRes->length), pData);
                    }
                    else
                    {   /* response frame contains an error, therefore we dont consider the data */
                        dwRetVal = EcConvertAdsErrorToEcError(pCmdRes->dwAoeCmdResult);
                        StopCurrAoEMbxRequest(dwRetVal);
                    }
                }
                else
                {   /* unexpected response size */
                    StopCurrAoEMbxRequest(EC_E_AOE_INV_RESPONSE_SIZE);
                }
            }
            else
            {   /* response frame contains an error, therefore we dont consider the data */
                dwRetVal = EcConvertAdsErrorToEcError(EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader));
                StopCurrAoEMbxRequest(dwRetVal);
            }
        } break;
        case ECAT_AOEHDR_COMMANDID_WRITECONTROL:
        {
            EC_T_DWORD dwRetVal = EC_E_ERROR;
            PETHERCAT_AOE_WRITECONTROL_RES_HEADER pAoeWriteResHdr = (PETHERCAT_AOE_WRITECONTROL_RES_HEADER)EC_ENDOF(pAoeHeader);

            pCmdRes->dwAoeErrorCode = EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader);

            /* check the response error (AoE header) */
            if (EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader) == 0)
            {
                 /* check response size */
                if (EC_AOE_HDR_GET_DATA_SIZE(pAoeHeader) >= sizeof(ETHERCAT_AOE_WRITECONTROL_RES_HEADER))
                {
                    pCmdRes->dwAoeCmdResult = pAoeWriteResHdr->dwResult;
                    dwRetVal = EcConvertAdsErrorToEcError(pAoeWriteResHdr->dwResult);
                    StopCurrAoEMbxRequest(dwRetVal);
                }
                else
                {
                    StopCurrAoEMbxRequest(EC_E_AOE_INV_RESPONSE_SIZE);
                }
            }
            else
            {   /* response frame contains an error, therefore we dont consider the data */
                dwRetVal = EcConvertAdsErrorToEcError(EC_AOE_HDR_GET_ERROR_CODE(pAoeHeader));
                StopCurrAoEMbxRequest(dwRetVal);
            }
        } break;
        default:
        {
            OsDbgAssert(EC_FALSE);
        } break;
        }
    }
    
Exit:
    return;
}
#endif

/** \brief Process received datagram of a CoE mailbox send commands
*
* \return N/A.
*/
EC_T_VOID   CEcMbSlave::ProcessCoEReturningRequest
(   EC_T_BOOL bIsInitCmd,
    EC_T_WORD wWkc, 
    PETYPE_EC_CMD_HEADER pEcCmdHeader, 
    PETHERCAT_MBOX_HEADER pMBox )
{
    EC_T_NOTIFICATION_INTDESC*  pNotification = EC_NULL;

#if !(defined INCLUDE_COE_PDO_SUPPORT)
    EC_UNREFPARM(pMBox);
#endif

    if (m_pCoEDesc == EC_NULL)
        return;
    
    if (wWkc == 1)
    {
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)
        {
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry = EC_FALSE;
        }
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid )
        {
            /* stop MBX repeat mailbox timeout */ 
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_FALSE;
        }

#ifdef INCLUDE_COE_PDO_SUPPORT
        if (pCan->wMbxType == ETHERCAT_CANOPEN_TYPE_RXPDO ) 
        {
            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid )
            {
            EC_T_DWORD nData = EC_ECMBOXHDR_GET_LENGTH(pMBox) - ETHERCAT_CANOPEN_HEADER_LEN;                   
                /* all data downloaded */
                if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length >= nData )
                {   /* size of Mailbox response is OK */
                    StopCurrCoEMbxRequest(bIsInitCmd, EC_E_NOERROR);
                }
                else
                {   /* size mismatch */
                    StopCurrCoEMbxRequest(bIsInitCmd, EC_E_INVALIDSIZE);
                }
            }
        }
        else
#endif
        {
            /* trigger mailbox polling -> server may want to respond */
            AddMailboxActionRequestPoll();
        }
    }
    else if ((wWkc == 0) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid && (m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter > 0))
    {
        /* Start oMbxTimeout and in SendQueuedCoECmds a Timeout will occur which starts the retry mechanism */ 
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Start(m_wRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_TRUE;

        /* Set m_bCoeWKCretry flag to avoid other mailbox protocols to send data*/
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry = EC_TRUE;

    }
    else if ((wWkc == 0) && bIsInitCmd && (m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
    {
        if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
        {
            m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
            m_wInitCmdsRetries--;
        }
    }
    else
    {
        /* Reset m_bCoeWKCretry flag */
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)
        {
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry = EC_FALSE;
        }

        EC_ERRORMSGC_L(("CEcMbSlave::ProcessCoEReturningRequest() WKC not correct. MBXSnd Slave '%s' WKC set/act=1/%d\n", 
            this->GetName(), ETYPE_EC_CMD_GETWKC(pEcCmdHeader)
            ));
        
        if (EC_E_NOERROR != m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode)
        {
            StopCurrCoEMbxRequest(bIsInitCmd, EC_E_COE_MBXSND_WKC_ERROR);
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOERROR;
        }
        else
        {
            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
            {
                StopCurrCoEMbxRequest(bIsInitCmd, EC_E_COE_MBXSND_WKC_ERROR, SDO_ABORTCODE_GENERAL);
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_COE_MBXSND_WKC_ERROR;
            }
        }
        
        pNotification = m_pMaster->AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_COE_MBXSND_WKC_ERROR;
            GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.SlaveProp));
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = wWkc;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = 1;
            m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
        }
        if (bIsInitCmd)
        {
            /* don't continue sending init commands */
            StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
        }
    }
}

#if (defined INCLUDE_FOE_SUPPORT)

/********************************************************************************/
/** \brief Process FoE slave's returning request
*
* \return
*/
EC_T_VOID   CEcMbSlave::ProcessFoEReturningRequest
(   EC_T_BOOL bIsInitCmd,
    EC_T_WORD wWkc, 
    PETYPE_EC_CMD_HEADER pEcCmdHeader, 
    PETHERCAT_MBOX_HEADER pMBox )
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    PEC_FOE_HDR pFoE = (PEC_FOE_HDR)EC_ENDOF(pMBox);

    if (m_pFoEDesc == EC_NULL)
        return;

    /* Check working counter */
    if (wWkc == 1)
    {
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry)
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry = EC_FALSE;
        }
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_FALSE;
        }

        /* Check if last segment was read successful */
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bLastRead && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid && (pFoE->OpCode == ECAT_FOE_OPCODE_ACK))
        {
            /* all data received stop mailbox */
            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_NOERROR, 0, 0, m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData);
        }

        /* validate/update PacketNumber */
        if (!bIsInitCmd)
        {   
            switch (pFoE->OpCode)
            {
                case ECAT_FOE_OPCODE_DATA:
                case ECAT_FOE_OPCODE_ACK:
                case ECAT_FOE_OPCODE_ERR:
                case ECAT_FOE_OPCODE_BUSY:
                {
                    /* Check packet Number, last packet again or next packet  */
                    if (EC_ECFOEHDR_GET_PACKETNO(pFoE) == m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber ||
                        EC_ECFOEHDR_GET_PACKETNO(pFoE) == m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber + 1)
                    {
                        m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber = EC_ECFOEHDR_GET_PACKETNO(pFoE);
                    }
                    /* Check if received first packet, else wrong packet number */
                    else if (0 != m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber)
                    {
                        EC_ERRORMSGC_L(("CEcMbSlave::ProcessFoEReturningRequest() PacketNo not correct. MBXSnd Slave '%s' PacketNo old/act=%d/%d\n",
                            this->GetName(), m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber, EC_ECFOEHDR_GET_PACKETNO(pFoE)
                            ));

                        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
                        {
                            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_MBX_WKC_ERROR, ECAT_FOE_ERRCODE_PACKENO);
                        }
                    }
                } break;

                /* __PacketNo not available at requests */
                case ECAT_FOE_OPCODE_RRQ:
                case ECAT_FOE_OPCODE_WRQ:
                default:
                    break;
            }
        }
        /* trigger mailbox polling -> server may want to respond */
        AddMailboxActionRequestPoll();
    }
    else if ((wWkc == 0) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid && (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter > 0))
    {
        /* Start oMbxTimeout and in SendQueuedFoECmds a Timeout will occur which starts the retry mechanism */ 
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Start(m_wRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_TRUE;

        /* Set m_bFoeWKCretry flag to avoid other mailbox protocols to send data*/
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry = EC_TRUE;
    }
    else
    {
        /* Reset m_bFoeWKCretry flag */
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry)
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry = EC_FALSE;
        }

        EC_ERRORMSGC(("CEcMbSlave::ProcessFoEReturningRequest() WKC not correct. MBXSnd Slave '%s' WKC set/act=1/%d\n", 
            this->GetName(), ETYPE_EC_CMD_GETWKC(pEcCmdHeader)
            ));

        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
        {
            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_FOE_MBX_WKC_ERROR, ECAT_FOE_ERRCODE_NOTDEFINED);
        }

        pNotification = m_pMaster->AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_FOE_MBXSND_WKC_ERROR;
            GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.SlaveProp));
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = wWkc;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = 1;
            m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
        }

        if (bIsInitCmd)
        {
            /* don't continue sending init commands */
            StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
        }
    }
}
#endif /* INCLUDE_FOE_SUPPORT */


#ifdef INCLUDE_SOE_SUPPORT
/********************************************************************************/
/** \brief Process SoE slave's returning request
*
* \return
*/
EC_T_VOID   CEcMbSlave::ProcessSoEReturningRequest
(   EC_T_BOOL bIsInitCmd,
    EC_T_WORD wWkc, 
    PETYPE_EC_CMD_HEADER pEcCmdHeader, 
    PETHERCAT_MBOX_HEADER pMBox )
{
    EC_T_DWORD dwDataLen;
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    EC_T_BOOL bWkcNotify = EC_FALSE;
    PEcMailboxCmdDesc pMbxCmdDesc = EC_NULL;

    if (m_pSoEDesc == EC_NULL)
        return;

    if (wWkc == 1 )
    {
       if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry)
        {
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry = EC_FALSE;
        }
        if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid )
        {
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry = 0;
        }
        if (bIsInitCmd || AreMbxInitCmdsActive())
        {     
            if (AreMbxInitCmdsPending())
            {
                pMbxCmdDesc = GetMbxInitCmdsCurrent();
                dwDataLen = EC_ECMBXCMDDESC_GET_DATALEN(pMbxCmdDesc) - sizeof(EC_SOE_HDR);
                if (dwDataLen < m_wMbxInitCmdsDataOffset )
                {
                    /* there should have not been sent more than requested!*/
                    OsDbgAssert(EC_FALSE);
                    dwDataLen = m_wMbxInitCmdsDataOffset;
                }
                if (dwDataLen-m_wMbxInitCmdsDataOffset == 0 )
                {   /* transfer complete -> next init cmd */
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessSoEReturningRequest() %s, complete\n", m_pEniSlave->szName));
                }
                else
                {   /* send next segment */
                    ETHERCAT_MBOX_HEADER    EcMbxHeader;
#ifdef  __TMS470__
                    EC_SOE_HDR     EcSoeHeader;
                    EC_SOE_HDR*    pEcSoeHeader;
                    pEcSoeHeader = (EC_SOE_HDR*)&pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader;
                    EcSoeHeader = *pEcSoeHeader;
                    OsDbgAssert(EC_FALSE);
#else
                    EC_SOE_HDR     EcSoeHeader = pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader;
#endif
                    EC_T_INT nFragLeft;

                    m_wInitCmdsRetries = (EC_T_WORD)EC_MAX( m_wRetryAccessCount, EC_ECMBXCMDDESC_GET_RETRIES(pMbxCmdDesc));

                    /* this is the amount of data that can be sent/received */
                    EC_T_WORD wMaxData  = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SOE_MBOX_LEN);
                    EC_T_WORD wSendData = (EC_T_WORD)EC_MIN(wMaxData, (EC_T_INT)dwDataLen - (EC_T_INT)m_wMbxInitCmdsDataOffset);
                    OsDbgAssert(dwDataLen >= m_wMbxInitCmdsDataOffset );
                    EC_T_WORD wRemainData = (EC_T_WORD)(dwDataLen - m_wMbxInitCmdsDataOffset);
                    
                    EC_T_WORD wInitCmdTimeout = EC_ECMBXCMDDESC_GET_MBXTIMEOUT(pMbxCmdDesc);
                    if (0 == wInitCmdTimeout)
                    {
                        wInitCmdTimeout = INIT_CMD_DEFAULT_TIMEOUT;
                    }
                    m_oInitCmdsTimeout.Start(wInitCmdTimeout, m_pMaster->GetMsecCounterPtr());
                    m_oInitCmdRetryTimeout.Stop();

                    MBXHDR_CLR(&EcMbxHeader);
                    EC_ECMBOXHDR_SET_MBXTYPE(&EcMbxHeader, ETHERCAT_MBOX_TYPE_SOE);
                    EC_ECMBOXHDR_SET_LENGTH(&EcMbxHeader, (EC_T_WORD)(EC_SOE_HDR_LEN + wSendData));
                    if (wRemainData > wMaxData )
                    {
                        /* the data doesn't fit into the mailbox */
                        EcSoeHeader.Incomplete = EC_TRUE;
                        /* number of fragments left */
                        nFragLeft = (((EC_T_INT)wRemainData + wMaxData - 1) / wMaxData) - 1;
                        EC_ECSOEHDR_SET_FRAGLEFT( &EcSoeHeader, (EC_T_WORD)nFragLeft );
                    }
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessSoEReturningRequest() Send next slave mailbox InitCmd[%d-%d] fragment: %s: %s - idn=0x%x\n", 
                        (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                        GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                        StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)),
                        (EC_T_INT)EC_ECSOEHDR_GET_IDN(&(pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader))) 
                        );
                    EcatMbxSendCmdReq(bIsInitCmd, &pMbxCmdDesc->uMbxHdr.soe.data[m_wMbxInitCmdsDataOffset], &EcMbxHeader, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,&EcSoeHeader
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,EC_NULL
                                     ,EC_NULL
#endif
                                     );
                    m_wMbxInitCmdsDataOffset = (EC_T_WORD)(m_wMbxInitCmdsDataOffset + wSendData);
                }
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid && (ECAT_SOE_OPCODE_WRQ == m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.OpCode))
        {   
            /* write fragmented SoE */
            if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete)
            {
                /* send next fragment of pending frame */

                ETHERCAT_MBOX_HEADER    mbx     = {0};
                EC_SOE_HDR     EcSoeHeader = {0};
                PEcMailboxCmd           pCmd = m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd;

                EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_SOE);

                /* fill the soe header with default values, some will be changed later */
                EcSoeHeader.Incomplete        = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete;
                EcSoeHeader.Error             = 0;
                EcSoeHeader.DriveNo           = pCmd->byDriveNo;
                EcSoeHeader.uIdnFragLeft.wIdn = pCmd->wIDN;
                EcSoeHeader.OpCode            = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.OpCode;  
                EcSoeHeader.byElements        = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.byElements;

                if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft > 1 )
                {
                    m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft--;
                    EC_ECSOEHDR_SET_FRAGLEFT( &EcSoeHeader, m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft );
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN + GetSoEDataSize(VG_OUT)));
                }
                else
                {
                    EcSoeHeader.uIdnFragLeft.wIdn = (unsigned short) pCmd->wIDN;
                    m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete = EcSoeHeader.Incomplete = EC_FALSE;
                     
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN + pCmd->length - m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset));
                }
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry = m_wRetryAccessCount;
                EcatMbxSendCmdReq(bIsInitCmd, &pCmd->pbyData[m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset], &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,&EcSoeHeader
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,EC_NULL
                                 ,EC_NULL
#endif
                                 );
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset	+= GetSoEDataSize(VG_OUT);
            }
        } 
        
        /* trigger mailbox polling -> server may want to respond */
        AddMailboxActionRequestPoll();
    } /* if (wWkc == 1) */
    else if (wWkc == 0)
    {
        EC_T_BYTE* pbyData = EC_NULL;
        PEC_SOE_HDR pSoE = EC_NULL;
		
        pSoE = (PEC_SOE_HDR)EC_ENDOF(pMBox);
        pbyData = (EC_T_BYTE*)EC_ENDOF(pSoE);
        EC_UNREFPARM(pSoE);
        EC_UNREFPARM(pbyData);
		
        EC_ERRORMSGC_L(("CEcMbSlave::ProcessSoEReturningRequest() WKC not correct. MBXSnd Slave '%s' WKC set/act=1/%d\n", 
            this->GetName(), ETYPE_EC_CMD_GETWKC(pEcCmdHeader)
            ));

        if (bIsInitCmd || AreMbxInitCmdsActive())
        {     
            if (AreMbxInitCmdsPending())
            {
                if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                {
                    if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                    {
                        m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        m_wInitCmdsRetries--;
                    }
                    pMbxCmdDesc = GetMbxInitCmdsCurrent();
                    EC_ERRORMSGC_L(("CEcMbSlave::ProcessSoEReturningRequest() retry send slave mailbox InitCmd[%d-%d] fragment: %s: %s - idn=0x%x\n", 
                        (EC_T_INT)(-(EC_T_WORD)m_wAutoIncAddr), m_wMbxInitCmdsCurrent, 
                        GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(pMbxCmdDesc)&0x7FF)), 
                        StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(pMbxCmdDesc)),
                        (EC_T_INT)EC_ECSOEHDR_GET_IDN(&(pMbxCmdDesc->uMbxHdr.soe.EcSoeHeader))) 
                        );
                }
                else
                {
                    /* give up*/
                    bWkcNotify = EC_TRUE;
                }
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else
        {
            if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid && (m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry > 0))
            {
                /* Start oMbxTimeout and in SendQueuedSoECmds a Timeout will occur which starts the retry mechanism */ 
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Start(m_wRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_TRUE;

                /* Set m_bSoeWKCretry flag to avoid other mailbox protocols to send data*/
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry = EC_TRUE;

            }
            else
            {
                /* Reset m_bSoeWKCretry flag */
                if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry)
                {
                    m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry = EC_FALSE;
                }

                /* give up*/
                bWkcNotify = EC_TRUE;
            }
        }
    }
    else
    {
        /* frame loss? */
        bWkcNotify = EC_TRUE;
    }

    if (bIsInitCmd && bWkcNotify )
    {
        /* don't continue sending init commands */
        StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
    }
    if (bWkcNotify )
    {
        pNotification = m_pMaster->AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_SOE_MBXSND_WKC_ERROR;
            GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.SlaveProp));
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = wWkc;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = 1;
            m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
        }
    }
}
#endif /* INCLUDE_SOE_SUPPORT */

#ifdef INCLUDE_VOE_SUPPORT
/********************************************************************************/
/** \brief Process VoE slave's returning request
*
* \return
*/
EC_T_VOID   CEcMbSlave::ProcessVoEReturningRequest
(   EC_T_WORD wWkc, 
    PETYPE_EC_CMD_HEADER pEcCmdHeader, 
    PETHERCAT_MBOX_HEADER pMBox )
{
    EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;
    EC_T_BOOL bWkcNotify = EC_FALSE;

    EC_UNREFPARM(pMBox);
    
    if (m_pVoEDesc == EC_NULL)
    {
        return;
    }

    /* Check working counter */
    if (wWkc == 1 )
    {
        m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry = EC_FALSE;

        if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid )
        {
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_FALSE;

            if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->cmdId == MAILBOXCMD_ECAT_VOE_WRITE)
            {
                /* data written stop mailbox */
                StopCurrVoEMbxRequest(EC_E_NOERROR);
            }
            else
            {
                OsDbgAssert(EC_FALSE);
            }
        }
        else
        {
            OsDbgAssert(EC_FALSE);
        }

         m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid = EC_FALSE;

    } /* if (wWkc == 1 )*/
    else if (wWkc == 0 )
    {
    /* In VoE WKC is no error! Depending on the VoE implementation it is a use case. */		
/*        EC_ERRORMSGC_L(("CEcMbSlave::ProcessVoEReturningRequest() WKC not correct. MBXSnd Slave '%s' WKC set/act=1/%d\n", 
            this->GetName(), ETYPE_EC_CMD_GETWKC(pEcCmdHeader)
            ));
*/
        if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid && (m_pVoEDesc->CurrPendVoeMbxCmdDesc.dwRetryCounter > 0))
        {
            /* Start oMbxTimeout and in SendQueuedVoECmds a Timeout will occur which starts the retry mechanism */ 
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Start(m_wRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_TRUE;

            /* Set m_bVoeWKCretry flag to avoid other mailbox protocols to send data*/
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry = EC_TRUE;
        }
        else
        {
            /* Reset m_bVoeWKCretry flag */
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry = EC_FALSE;
            
            /* give up*/
            bWkcNotify = EC_TRUE;

            /* return wkc error */
            StopCurrVoEMbxRequest(EC_E_VOE_MBX_WKC_ERROR);
        }
    }
    else
    {
        /* frame loss? */
        bWkcNotify = EC_TRUE;

        /* return wkc error */
        StopCurrVoEMbxRequest(EC_E_VOE_MBX_WKC_ERROR);
    }


    /* notify a voe working counter error */
    if (bWkcNotify )
    {
        pNotification = m_pMaster->AllocNotification();
        if (EC_NULL != pNotification)
        {
            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_VOE_MBXSND_WKC_ERROR;
            GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.SlaveProp));
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.byCmd   = pEcCmdHeader->uCmdIdx.swCmdIdx.byCmd;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.dwAddr  = EC_ICMDHDR_GET_ADDR(pEcCmdHeader);
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcAct = wWkc;
            pNotification->uNotification.ErrorNotification.desc.WkcErrDesc.wWkcSet = 1;
            m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_WKCERR);
        }
    }
}
#endif

#ifdef INCLUDE_AOE_SUPPORT
/********************************************************************************/
/** \brief Process AoE slave's returning request
*
* \return
*/
EC_T_VOID   CEcMbSlave::ProcessAoEReturningRequest
(   EC_T_BOOL bIsInitCmd,
    EC_T_WORD wWkc, 
    PETYPE_EC_CMD_HEADER pEcCmdHeader, 
    PETHERCAT_MBOX_HEADER pMBox )
{
    EC_T_DWORD dwDataLen;
    EC_T_BOOL bWkcNotify = EC_FALSE;
    PEcMailboxCmdDesc pMbxCmdDesc = EC_NULL;

#ifndef INCLUDE_LOG_MESSAGES
    EC_UNREFPARM(pEcCmdHeader);
#endif

    if (m_pAoEDesc == EC_NULL)
    {
        return;
    }

    if (wWkc == 1 )
    {
        if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry)
        {
            m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry = EC_FALSE;
        }
        if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid )
        {
            m_pAoEDesc->CurrPendAoeMbxCmdDesc.wRetryCounter = 0;
        }

        if (bIsInitCmd || AreMbxInitCmdsActive())
        {     
            if (AreMbxInitCmdsPending())
            {
                pMbxCmdDesc = GetMbxInitCmdsCurrent();
                dwDataLen = EC_ECMBXCMDDESC_GET_DATALEN(pMbxCmdDesc) - sizeof(EC_AOE_HDR);
                if (dwDataLen < m_wMbxInitCmdsDataOffset )
                {
                    /* there should have not been sent more than requested!*/
                    OsDbgAssert(EC_FALSE);
                    dwDataLen = m_wMbxInitCmdsDataOffset;
                }
                if (dwDataLen-m_wMbxInitCmdsDataOffset == 0 )
                {   /* transfer complete -> next init cmd */
                    EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessAoEReturningRequest() %s, complete\n", m_pEniSlave->szName));
                }
                else
                {   
                    /* fragmented init command not yet supported */
                    EC_ERRORMSGC(("ProcessAoEReturningRequest: Fragmented AoE init command sent though not supported!\n", m_wMbxInitCmdsCurrent, m_wMbxInitCmdsCurrent, m_wMbxInitCmdsCount));
                    OsDbgAssert(EC_FALSE);
                }
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader.wCmdId == ECAT_AOEHDR_COMMANDID_WRITE)
        {   
            /* nothing to do */
        }
        else if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader.wCmdId == ECAT_AOEHDR_COMMANDID_READ)
        {
            /* nothing to do */
        }
        else if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader.wCmdId == ECAT_AOEHDR_COMMANDID_READWRITE)
        {
            /* nothing to do */
        }
        else if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader.wCmdId == ECAT_AOEHDR_COMMANDID_WRITECONTROL)
        {
            /* nothing to do */
        }
        else
        {
            OsDbgAssert(EC_FALSE);
        }
        
        /* trigger mailbox polling -> server may want to respond */
        AddMailboxActionRequestPoll();

    } /* if (wWkc == 1) */
    else if (wWkc == 0)
    {
        EC_T_BYTE* pbyData = EC_NULL;
        PEC_AOE_HDR pAoE = EC_NULL;
		
        pAoE = (PEC_AOE_HDR)EC_ENDOF(pMBox);
        pbyData = (EC_T_BYTE*)EC_ENDOF(pAoE);
        EC_UNREFPARM(pAoE);
        EC_UNREFPARM(pbyData);
		
        EC_ERRORMSGC_L(("CEcMbSlave::ProcessAoEReturningRequest() WKC not correct. MBXSnd Slave '%s' WKC set/act=1/%d", 
            this->GetName(), ETYPE_EC_CMD_GETWKC(pEcCmdHeader)
            ));

        if (bIsInitCmd || AreMbxInitCmdsActive())
        {     
            if (AreMbxInitCmdsPending())
            {
                if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                {
                    if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                    {
                        m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                        m_wInitCmdsRetries--;
                    }
                }
                else
                {
                    /* give up*/
                    bWkcNotify = EC_TRUE;
                }
            }
            else
            {
                OsDbgAssert(EC_FALSE);
                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
            }
        }
        else
        {
            if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && (m_pAoEDesc->CurrPendAoeMbxCmdDesc.wRetryCounter > 0))
            {
                /* start oRetryTimeout and check this timeout in SendQueuedAoE commands  */ 
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Start(m_wRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_TRUE;

                /* Set m_bAoeWKCretry flag to avoid other mailbox protocols to send data*/
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry = EC_TRUE;
            }
            else
            {
                /* Reset m_bAoeWKCretry flag */
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry = EC_FALSE;
                
                /* give up */
                bWkcNotify = EC_TRUE;
            }
        }
    }
    else
    {
        /* frame loss? */
        bWkcNotify = EC_TRUE;
    }

    if (bIsInitCmd && bWkcNotify )
    {
        /* don't continue sending init commands */
        StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
    }
    if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && bWkcNotify )
    {
        StopCurrAoEMbxRequest(EC_E_MBX_CMD_WKC_ERROR);
    }
}
#endif



#ifdef INCLUDE_VOE_SUPPORT
/********************************************************************************/
/** \brief Process VoE response
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessVoEResponse(PETHERCAT_MBOX_HEADER pMBox )
{
    EC_T_WORD wReceivedVoeDataLen = 0;
    EC_T_MBXTFERP* poVoeMbxTferObj = EC_NULL;
    EC_T_BYTE* pReceivedVoeData = EC_NULL;
            
    /* parameter check */
    if (m_pVoEDesc == EC_NULL || m_pvVoeMbxTferObj == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
    
    wReceivedVoeDataLen = EC_ECMBOXHDR_GET_LENGTH(pMBox);
    pReceivedVoeData    = (EC_T_BYTE*)EC_ENDOF(pMBox);
    poVoeMbxTferObj     = (EC_T_MBXTFERP*)(m_pvVoeMbxTferObj);       
           
    /* check if the size of the received VoE mailbox matches */
    if ((wReceivedVoeDataLen + ETHERCAT_MBOX_HEADER_LEN) > m_mbxOLen[m_sMboxFlags.mbxIdx])
    {
        EC_ERRORMSGC_L(("Received VoE data greater then the available mailbox size\n"));
        wReceivedVoeDataLen = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] + ETHERCAT_MBOX_HEADER_LEN);
    }
    
    /* if the mailbox transfer status is idle the VoE mailbox object is free and can be written */
    if (poVoeMbxTferObj->MbxTfer.eTferStatus == eMbxTferStatus_Idle)
    {
        poVoeMbxTferObj->MbxTfer.dwTferId    = m_dwSlaveId;
        OsMemcpy(poVoeMbxTferObj->MbxTfer.pbyMbxTferData, pReceivedVoeData, wReceivedVoeDataLen);
        poVoeMbxTferObj->MbxTfer.dwDataLen   = wReceivedVoeDataLen;
        poVoeMbxTferObj->MbxTfer.dwErrorCode = EC_E_NOERROR;
        poVoeMbxTferObj->MbxTfer.eTferStatus = eMbxTferStatus_TferDone; 
        
        /* by calling MailboxRes a notification will be thrown that indecates that voe data are available and can be 
           retrieved by ecatVoeRead */
        MailboxRes(&(poVoeMbxTferObj->oEcMbxCmd), EC_E_NOERROR);
    } 
    else
    {
        /* the EtherCAT application must first call ecatVoeRead to retrieve the received VoE mailbox data */
        EC_ERRORMSGC_L(("VoE mailbox lost! Unretrieved VoE mailbox has blocked the incoming VoE mailbox\n"));
    }
    
Exit:
    return;
}

#endif


/***************************************************************************************************/
/**
\brief  Set Slave Presence.
*/
EC_T_VOID CEcMbSlave::SetSlavePresence(
    EC_T_BOOL   bPresent    /**< [in]   EC_TRUE: slave is present on bus, EC_FALSE: it is not */
                                    )
{
    if (!bPresent)
    {
        EC_T_DWORD dwResult = EC_E_SLAVE_NOT_PRESENT;

        m_wMbxInitCmdsCurrent = INITCMD_INACTIVE;
        DelMailboxActionPoll();
        DelMailboxActionRead();

        if ((m_pCoEDesc != EC_NULL) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
        {
            StopCurrCoEMbxRequest(EC_FALSE, dwResult);
        }
#ifdef INCLUDE_FOE_SUPPORT
        if ((m_pFoEDesc != EC_NULL) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
        {
            StopCurrFoEMbxRequest(EC_FALSE, dwResult);
        }
#endif
#ifdef INCLUDE_SOE_SUPPORT
        if ((m_pSoEDesc != EC_NULL) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
        {
            StopCurrSoEMbxRequest(dwResult);
        }
#endif
#ifdef INCLUDE_VOE_SUPPORT
        if ((m_pVoEDesc != EC_NULL) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
        {
            StopCurrVoEMbxRequest(dwResult);
        }
#endif
#ifdef INCLUDE_AOE_SUPPORT
        if ((m_pAoEDesc != EC_NULL) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
        {
            StopCurrAoEMbxRequest(dwResult);
        }
#endif

        m_bMbxCmdPending = EC_FALSE;
    }

    CEcSlave::SetSlavePresence(bPresent);
}

/********************************************************************************/
/** \brief Processes the response of an EtherCAT command.
*
* \return
*/
EC_T_VOID   CEcMbSlave::ProcessCmdResult
(   EC_T_DWORD result, 
    EC_T_DWORD dwInvokeId, 
    PETYPE_EC_CMD_HEADER pEcCmdHeader )
{
    EC_T_BOOL             bIsInitCmd            = EC_FALSE;
    EC_T_BOOL             bPolledRecv           = EC_FALSE;
    EC_T_BOOL             bDelMailboxActionPoll = EC_FALSE;
    EC_T_BOOL             bDelMailboxActionRead = EC_FALSE;
    EC_T_BOOL             bValidateMBox         = EC_FALSE;
    EC_T_WORD             wWkc                  = 0;
    PETHERCAT_MBOX_HEADER pMBox                 = EC_NULL;

    OsDbgAssert((result == EC_E_CMD_MISSING) || (pEcCmdHeader != EC_NULL));

    /* filter not mailbox invoke IDs */
    if (((EC_T_DWORD)eMbxInvokeId_FIRST > dwInvokeId) || ((EC_T_DWORD)eMbxInvokeId_LAST < dwInvokeId))
    {
        EC_TRACEMSG(EC_TRACE_INITCMDS, ("CEcMbSlave::ProcessCmdResult() %s: process dwInvokeId = 0x%x\n", m_pEniSlave->szName, (EC_T_INT)dwInvokeId));
        CEcSlave::ProcessCmdResult(result, dwInvokeId, pEcCmdHeader);
        goto Exit;
    }

    /* preprocess command */
    switch (dwInvokeId)
    {
    case eMbxInvokeId_RECV_FROM_SLAVE:
        bValidateMBox = EC_TRUE;
        bDelMailboxActionRead = EC_TRUE;
        break;
    case eMbxInvokeId_POLL_RECV_FROM_SLAVE:
        bPolledRecv   = EC_TRUE;
        bDelMailboxActionPoll = EC_TRUE;
        break;
    case eMbxInvokeId_SEND_TO_SLAVE:
        bValidateMBox = EC_TRUE;
        break;
    case eMbxInvokeId_INITCMD_RECV_FROM_SLAVE:
        bIsInitCmd    = EC_TRUE;
        bValidateMBox = EC_TRUE;
        bDelMailboxActionRead = EC_TRUE;
        break;
    case eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE:
        bIsInitCmd    = EC_TRUE;
        bPolledRecv   = EC_TRUE;
        bDelMailboxActionPoll = EC_TRUE;
        break;
    case eMbxInvokeId_INITCMD_SEND_TO_SLAVE:
        bIsInitCmd    = EC_TRUE;
        bValidateMBox = EC_TRUE;
        break;
    case eMbxInvokeId_INVALIDATED_RECV_FROM_SLAVE:
        break;
    case eMbxInvokeId_SEND_FILL_TO_SLAVE:
        break;
    case eMbxInvokeId_TOGGLE_REPEAT_REQ:
        break;
    case eMbxInvokeId_TOGGLE_REPEAT_RES:
        break;
    case eMbxInvokeId_INITCMD_TOGGLE_REPEAT_REQ:
        bIsInitCmd    = EC_TRUE;
        break;
    case eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES:
        bIsInitCmd    = EC_TRUE;
        break;
    case eMbxInvokeId_INVALIDATED_SEND_TO_SLAVE:
        break;
    default:        
        OsDbgAssert(EC_FALSE);
        break;
    }
    /* delete mailbox action if needed */
    if (bDelMailboxActionPoll)
    {
        DelMailboxActionPoll();
    }
    if (bDelMailboxActionRead)
    {
        DelMailboxActionRead();
    }
    /* validate mailbox data if needed */
    if (bValidateMBox && (result != EC_E_CMD_MISSING))
    {
        wWkc  = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
        pMBox = (PETHERCAT_MBOX_HEADER)EC_ENDOF(pEcCmdHeader);
        if (EC_ECMBOXHDR_GET_LENGTH(pMBox) > ETYPE_EC_CMD_GETLEN(pEcCmdHeader))
        {
            EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult(): mailbox data length wrong! Got %d, expected at most %d. result is 0x%lx\n",
                (EC_T_INT)EC_ECMBOXHDR_GET_LENGTH(pMBox), (EC_T_INT)ETYPE_EC_CMD_GETLEN(pEcCmdHeader), result));
            EC_T_NOTIFICATION_INTDESC* pNotification = EC_NULL;   
            m_bMbxReadStateIsBusy = EC_FALSE;

            /* allocate notification memory and fill notification data */
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_MBXRCV_INVALID_DATA;
                GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.MbxRcvInvalidDataDesc.SlaveProp));

                /* generate notification */
                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_MBXRCV_INVALID_DATA);
            }

            /* drop errornous data to not disturb other mailbox protocols */
            if (!m_mbxLayer.bRepeatRequested && (m_mbxLayer.byCntRecv != 0))
            {
                IncCntMbxRecv();
            }

            goto Exit;
        }
    }

    /* process mailbox command */
    switch (dwInvokeId)
    {
    case eMbxInvokeId_SEND_TO_SLAVE:
    case eMbxInvokeId_INITCMD_SEND_TO_SLAVE:
        /* process result after sending mailbox data to slave */
        if (result == EC_E_NOERROR )
        {
            if (wWkc == 0)
            {
                DecCntMbxSend();
            }
            EC_TRACEMSG(EC_TRACE_INITCMDS, ("%s[%06d] CEcMbSlave::ProcessCmdResult(cmdIdx=0x%x) %s: process dwInvokeId = eMbxInvokeId_SEND_TO_SLAVE (%d)\n", 
                bIsInitCmd?"====>":"++++>", OsQueryMsecCount(), (EC_T_DWORD)EC_ICMDHDR_GET_CMDIDX(pEcCmdHeader), m_pEniSlave->szName, 
                bIsInitCmd?"eMbxInvokeId_INITCMD_SEND_TO_SLAVE":"eMbxInvokeId_SEND_TO_SLAVE", (EC_T_INT)dwInvokeId));

#if (defined INCLUDE_RAWMBX_SUPPORT)
            if (RAWMBX_FIXEDADDRESS == EC_ECMBOXHDR_GET_ADDRESS(pMBox))
            {
                ProcessRawMbxReturningRequest(bIsInitCmd, wWkc, pEcCmdHeader, pMBox);
            }
            else
#endif /* INCLUDE_RAWMBX_SUPPORT */
            {            
                switch ( EC_ECMBOXHDR_GET_MBXTYPE(pMBox))
                {
#if (defined INCLUDE_EOE_SUPPORT)
                case ETHERCAT_MBOX_TYPE_ETHERNET:
                    ProcessEoEReturningRequest(bIsInitCmd, wWkc, pEcCmdHeader, pMBox);
                    break;
#endif
                case ETHERCAT_MBOX_TYPE_CANOPEN:
                    ProcessCoEReturningRequest(bIsInitCmd, wWkc, pEcCmdHeader, pMBox);
                    break;
#if (defined INCLUDE_FOE_SUPPORT)
                case ETHERCAT_MBOX_TYPE_FILEACCESS:
                    ProcessFoEReturningRequest(bIsInitCmd, wWkc, pEcCmdHeader, pMBox);
                    break;
#endif
#ifdef INCLUDE_SOE_SUPPORT
                case ETHERCAT_MBOX_TYPE_SOE:
                    ProcessSoEReturningRequest(bIsInitCmd, wWkc, pEcCmdHeader, pMBox);
                    break;
#endif
#ifdef INCLUDE_VOE_SUPPORT
                case ETHERCAT_MBOX_TYPE_VOE:
                    ProcessVoEReturningRequest(wWkc, pEcCmdHeader, pMBox);
                    break;
#endif
#ifdef INCLUDE_AOE_SUPPORT
                case ETHERCAT_MBOX_TYPE_ADS:
                    ProcessAoEReturningRequest(bIsInitCmd, wWkc, pEcCmdHeader, pMBox);
                    break;
#endif
                default:
                    EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Unsupported mbx send type %d received\n", (EC_T_INT)EC_ECMBOXHDR_GET_MBXTYPE(pMBox)));
                }
            }
        }
        else if (pMBox != EC_NULL)
        {
#if (defined INCLUDE_RAWMBX_SUPPORT)
            if (RAWMBX_FIXEDADDRESS == EC_ECMBOXHDR_GET_ADDRESS(pMBox))
            {
                StopCurrRawMbxRequest(bIsInitCmd, result);
            }
            else
#endif /* INCLUDE_RAWMBX_SUPPORT */
            {
                /* @@@ App Notify*/
                /* an error occurred while writing a mailbox */
                switch (EC_ECMBOXHDR_GET_MBXTYPE(pMBox))
                {
                case ETHERCAT_MBOX_TYPE_ETHERNET:
                    /* @@@: error handling, clear current pending command?*/
                    EC_ERRORMSGC_L( ("CEcMbSlave::ProcessCmdResult() Mailbox write error on slave '%s' (addr 0x%x): Error 0x%x!.\n", m_pEniSlave->szName, m_wFixedAddr, result));


                    if (bIsInitCmd || AreMbxInitCmdsActive())
                    {     
                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("%s[%06d] CEcMbSlave::ProcessCmdResult(ERROR - RESULT = 0x%x) %s: process dwInvokeId = eMbxInvokeId_SEND_TO_SLAVE (%d)\n", 
                            bIsInitCmd?"====>":"++++>", OsQueryMsecCount(), (EC_T_DWORD)result, m_pEniSlave->szName, 
                            bIsInitCmd?"eMbxInvokeId_INITCMD_SEND_TO_SLAVE":"eMbxInvokeId_SEND_TO_SLAVE", (EC_T_INT)dwInvokeId));

                        if (AreMbxInitCmdsPending())
                        {
#ifdef INCLUDE_LOG_MESSAGES
                            /* an error occurred while writing mailbox init commands */
                            PEcMailboxCmdDesc pMbxCmdDesc = GetMbxInitCmdsCurrent();
                            EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Mailbox write init command error on slave '%s' (addr 0x%x):\n'%s' Communication Error (0x%x)! Comment: '%s'.\n", 
                                m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName(m_wActTransition), result, EcMailboxCmdDescComment(pMbxCmdDesc)));
#endif
                            if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                            {
                                if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                                {
                                    m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                                    m_wInitCmdsRetries--;
                                }
                            }
                            else
                            {
                                /* don't continue sending init commands */
                                StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
                            }
                        }
                    }
                    break;
                case ETHERCAT_MBOX_TYPE_CANOPEN:
                    if (bIsInitCmd || AreMbxInitCmdsActive())
                    {     
                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("%s[%06d] CEcMbSlave::ProcessCmdResult(ERROR - RESULT = 0x%x) %s: process dwInvokeId = eMbxInvokeId_SEND_TO_SLAVE (%d)\n", 
                            bIsInitCmd?"====>":"++++>", OsQueryMsecCount(), (EC_T_DWORD)result, m_pEniSlave->szName, 
                            bIsInitCmd?"eMbxInvokeId_INITCMD_SEND_TO_SLAVE":"eMbxInvokeId_SEND_TO_SLAVE", (EC_T_INT)dwInvokeId));
                        if (AreMbxInitCmdsPending())
                        {
#ifdef INCLUDE_LOG_MESSAGES
                            /* an error occurred while writing mailbox init commands */
                            PEcMailboxCmdDesc pMbxCmdDesc = GetMbxInitCmdsCurrent();
                            EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Mailbox write init command error on slave '%s' (addr 0x%x):\n'%s' Communication Error (0x%x)! Comment: '%s'.\n", 
                                m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName(m_wActTransition), result, EcMailboxCmdDescComment(pMbxCmdDesc)));
#endif
                            if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                            {
                                if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                                {
                                    m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                                    m_wInitCmdsRetries--;
                                }
                            }
                            else
                            {
                                /* don't continue sending init commands */
                                StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
                            }
                        }
                    }
                    else if ((m_pCoEDesc != EC_NULL) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
                    {
                        StopCurrCoEMbxRequest(bIsInitCmd, result);
                    }
                    break;
#if (defined INCLUDE_FOE_SUPPORT)
                case ETHERCAT_MBOX_TYPE_FILEACCESS:
                    /* error handling, set flag for resending mailbox*/
                    if ((m_pFoEDesc != EC_NULL) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
                    {
                        StopCurrFoEMbxRequest(bIsInitCmd, result);
                    }
                    break;
#endif /* INCLUDE_FOE_SUPPORT */
#ifdef INCLUDE_SOE_SUPPORT
                case ETHERCAT_MBOX_TYPE_SOE:
                    if (bIsInitCmd || AreMbxInitCmdsActive())
                    {     
                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("%s[%06d] CEcMbSlave::ProcessCmdResult(ERROR - RESULT = 0x%x) %s: process dwInvokeId = eMbxInvokeId_SEND_TO_SLAVE (%d)\n", 
                            bIsInitCmd?"====>":"++++>", OsQueryMsecCount(), (EC_T_DWORD)result, m_pEniSlave->szName, 
                            bIsInitCmd?"eMbxInvokeId_INITCMD_SEND_TO_SLAVE":"eMbxInvokeId_SEND_TO_SLAVE", (EC_T_INT)dwInvokeId));
                        if (AreMbxInitCmdsPending())
                        {
#ifdef INCLUDE_LOG_MESSAGES
                            /* an error occurred while writing mailbox init commands */
                            PEcMailboxCmdDesc pMbxCmdDesc = GetMbxInitCmdsCurrent();
                            EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Mailbox write init command error on slave '%s' (addr 0x%x):\n'%s' Communication Error (0x%x)! Comment: '%s'.\n", 
                                m_pEniSlave->szName, m_wFixedAddr, GetStateChangeName(m_wActTransition), result, EcMailboxCmdDescComment(pMbxCmdDesc)));
#endif
                            if ((m_wInitCmdsRetries > 0) && !m_bCancelStateMachine)
                            {
                                if (m_oInitCmdRetryTimeout.IsElapsed() || !m_oInitCmdRetryTimeout.IsStarted())
                                {
                                    m_oInitCmdRetryTimeout.Start(m_pMaster->m_dwInitCmdRetryTimeout, m_pMaster->GetMsecCounterPtr());
                                    m_wInitCmdsRetries--;
                                }
                            }
                            else
                            {
                                /* don't continue sending init commands */
                                StopInitCmds( ECAT_INITCMD_FAILURE, m_wActTransition );
                            }
                        }
                    }
                    else if ((m_pSoEDesc != EC_NULL) && (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid))
                    {
                        StopCurrSoEMbxRequest(result);
                    }
                    break;
#endif
                default:
                    /* The default case should never happen */
                    if ((m_pCoEDesc != EC_NULL) && (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid))
                    {
                        StopCurrCoEMbxRequest(bIsInitCmd, result);
                    }
#ifdef INCLUDE_SOE_SUPPORT
                    if ((m_pSoEDesc != EC_NULL) && (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid))
                    {
                        StopCurrSoEMbxRequest(result);
                    }
#endif
#if (defined INCLUDE_FOE_SUPPORT)
                    if ((m_pFoEDesc != EC_NULL) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
                    {
                        StopCurrFoEMbxRequest(bIsInitCmd, result);
                    }
#endif /* INCLUDE_FOE_SUPPORT */
                    else
                    {
                        EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Unsupported mbx send type %d received\n", (EC_T_INT)EC_ECMBOXHDR_GET_MBXTYPE(pMBox)));
                    }
                }
            }
        }
        //reset m_bMbxCmdPending flag after process send data --> allow to queue new mbx command.
        m_bMbxCmdPending = EC_FALSE;
        break;
    
    case eMbxInvokeId_RECV_FROM_SLAVE:
    case eMbxInvokeId_POLL_RECV_FROM_SLAVE:
    case eMbxInvokeId_INITCMD_RECV_FROM_SLAVE:
    case eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE:
        /* process received mailbox data from slave */
        if (result == EC_E_NOERROR )
        {
            /* Mailbox returned --> set mailbox state to idle */
            m_bMbxReadStateIsBusy = EC_FALSE;
            EC_TRACEMSG(EC_TRACE_INITCMDS, ("%s[%06d] CEcMbSlave::ProcessCmdResult(cmdIdx=0x%x) %s: dwInvokeId = %s (0x%x)\n", 
                        bIsInitCmd?"====>":"++++>", OsQueryMsecCount(), (EC_T_DWORD)EC_ICMDHDR_GET_CMDIDX(pEcCmdHeader), m_pEniSlave->szName, 
                        bIsInitCmd?"eMbxInvokeId_INITCMD_(POLL)_RECV_FROM_SLAVE":"eMbxInvokeId_(POLL)_RECV_FROM_SLAVE", (EC_T_INT)dwInvokeId));
            if (ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 1)
            {
                if (bPolledRecv)
                {
                    /* received mailbox header only: now read the mailbox as it contains data */
                    AddMailboxActionRequestRead();
                }
                else
                {
                    if (0 == m_mbxLayer.byCntRecv)
                    {
                        m_mbxLayer.byCntRecv = EC_ECMBOXHDR_GET_COUNTER(pMBox);
                        
                        EC_TRACEMSG(EC_TRACE_INITCMDS, ("'%s' (%d): Recv Mbx Communication Info (0x%x)! MBX Data Link Layer initialized with %d\n", 
                            m_pEniSlave->szName, m_wFixedAddr, m_mbxLayer.byCntRecv, EC_ECMBOXHDR_GET_COUNTER(pMBox)));
                    }

                    if ((EC_ECMBOXHDR_GET_COUNTER(pMBox) == 0) || (m_mbxLayer.byCntRecv == EC_ECMBOXHDR_GET_COUNTER(pMBox)))
                    {
#if (defined INCLUDE_RAWMBX_SUPPORT)
                        if (RAWMBX_FIXEDADDRESS == EC_ECMBOXHDR_GET_ADDRESS(pMBox))
                        {
                            ProcessRawMbxResponse(pMBox);
                        }
                        else
#endif
                        {
                            /* received mailbox data: process response */
                            switch (EC_ECMBOXHDR_GET_MBXTYPE(pMBox))
                            {
#if (defined INCLUDE_EOE_SUPPORT)
                            case ETHERCAT_MBOX_TYPE_ETHERNET:
                                ProcessEoEResponse(bIsInitCmd, pMBox);
                                break;
#endif
                            case ETHERCAT_MBOX_TYPE_CANOPEN:
                                ProcessCoEResponse(bIsInitCmd, pMBox);
                                break;
#if (defined INCLUDE_FOE_SUPPORT)
                            case ETHERCAT_MBOX_TYPE_FILEACCESS:
                                ProcessFoEResponse(bIsInitCmd, pMBox);
                                break;
#endif
#ifdef INCLUDE_SOE_SUPPORT
                            case ETHERCAT_MBOX_TYPE_SOE:
                                ProcessSoEResponse(bIsInitCmd, pMBox);
                                break;
#endif
#ifdef INCLUDE_VOE_SUPPORT
                            case ETHERCAT_MBOX_TYPE_VOE:
                                ProcessVoEResponse(pMBox);
                                break;
#endif
#ifdef INCLUDE_AOE_SUPPORT
                            case ETHERCAT_MBOX_TYPE_ADS:
                                ProcessAoEResponse(bIsInitCmd, pMBox);
                                break;
#endif
                                /* mailbox service protocol */
                            case ETHERCAT_MBOX_TYPE_ERROR:
                                {
                                    EC_T_BYTE  *pbyData;
                                    EC_T_WORD  wMbxErrorCode = 0;
                                    EC_T_DWORD  dwRes = 0;
                                    
                                    pbyData = (EC_T_BYTE *)EC_ENDOF(pMBox);
                                    wMbxErrorCode = EC_GET_FRM_WORD(pbyData+2);
                                    
                                    EC_ERRORMSGC(("CEcMbSlave::ProcessCmdResult() Mailbox Error. MBXRcv Slave '%s' Error Code %d\n", 
                                        this->GetName(), wMbxErrorCode));

                                    /* determine the error code by the received mailbox error */
                                    /* see Table 30 - Error Reply Service Data in ETG1000_4_S_R_V1i0i3_EcatDLLProtocols.pdf */
                                    switch(wMbxErrorCode)
                                    {
                                    case 0x01:
                                        dwRes = EC_E_MBXERR_SYNTAX;
                                        break;
                                    case 0x02:
                                        dwRes = EC_E_MBXERR_UNSUPPORTEDPROTOCOL;
                                        break;
                                    case 0x03:
                                        dwRes = EC_E_MBXERR_INVALIDCHANNEL;
                                        break;
                                    case 0x04:
                                        dwRes = EC_E_MBXERR_SERVICENOTSUPPORTED;
                                        break;
                                    case 0x05:
                                        dwRes = EC_E_MBXERR_INVALIDHEADER;
                                        break;
                                    case 0x06:
                                        dwRes = EC_E_MBXERR_SIZETOOSHORT;
                                        break;
                                    case 0x07:
                                        dwRes = EC_E_MBXERR_NOMOREMEMORY;
                                        break;
                                    case 0x08:
                                        dwRes = EC_E_MBXERR_INVALIDSIZE;
                                        break;
                                    default:
                                        dwRes = EC_E_MBX_ERROR_TYPE;
                                    }
                                    StopCurrMbxRequest(bIsInitCmd, dwRes, pMBox);
                                }   
                                break;
                                
                            default:
                                EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() '%s' (%d) '%s':  Unsupported mbx recv type %d received\n", m_pEniSlave->szName, m_wFixedAddr, GetStateChangeNameShort(m_wActTransition), (EC_T_INT)EC_ECMBOXHDR_GET_MBXTYPE(pMBox)));
                            }
                        }
                        IncCntMbxRecv();
                    }
                    else
                    {
                        EC_T_BYTE byCnt = (EC_T_BYTE)EC_ECMBOXHDR_GET_COUNTER(pMBox);
                        EC_T_BYTE byPrevCntRecv = (EC_T_BYTE)(m_mbxLayer.byCntRecv - 1);
                        if (byPrevCntRecv == 0)
                        {
                            byPrevCntRecv = 7;
                        }
                        if (byPrevCntRecv == byCnt)
                        {
                            if (m_mbxLayer.bRepeatRequested)
                            {
                                EC_TRACEMSG(EC_TRACE_INITCMDS, ("'%s' (%d): Recv Mbx service counter %d again due to repeat request. Expecting next %d.\n", 
                                    m_pEniSlave->szName, m_wFixedAddr, byCnt, m_mbxLayer.byCntRecv));
                            }
                            else
                            {
                                EC_ERRORMSGC_L(("'%s' (%d): Recv Mbx Communication Warning (0x%x)! Duplicated response from the slave (Mbx service counter %d expected and %d read)!\n", 
                                    m_pEniSlave->szName, m_wFixedAddr, result, m_mbxLayer.byCntRecv, byCnt));

                                /* reset receive counter */
                                m_mbxLayer.byCntRecv = 0;
                            }
                        }
                        else
                        {
                            EC_ERRORMSGC_L(("'%s' (%d): Recv Mbx Communication Error (0x%x)! Unexpected response from the slave (Mbx service counter %d expected and %d read)!\n", 
                                m_pEniSlave->szName, m_wFixedAddr, result, m_mbxLayer.byCntRecv, byCnt));

                            /* reset receive counter */
                            m_mbxLayer.byCntRecv = 0;
                        }
                    }
                    /* got response from slave */
                    if (m_mbxLayer.bRepeatRequested)
                    {
                        EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult(): RepeatRequested=0\n"));
                    }
                    m_mbxLayer.bRepeatRequested = EC_FALSE;
                }
            } /* if (ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 1 ) */
            else
            {
                /* WKC not correct */
                /* nothing in the mail box*/
                if (bIsInitCmd )
                {
                    /* trigger mailbox polling -> server has to respond to previous mailbox init command!*/
                     AddMailboxActionRequestPoll();
                }
            } /* else if (ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 1 ) */
        } /* if (result == EC_E_NOERROR ) */

        /* Only start the repeating mailbox communication mechanism if slave is still present. No repeating for EoE */
        else if ((IsPresentOnBus()) && !bPolledRecv && (ETHERCAT_MBOX_TYPE_ETHERNET != EC_ECMBOXHDR_GET_MBXTYPE(pMBox)))
        {
            /* start the repeating mailbox communication mechanism */
            EC_TRACEMSG(EC_TRACE_INITCMDS, ("'%s' (%d): Recv Mbx Communication Warning (0x%x)! Request a repeat from the slave!\n",
                m_pEniSlave->szName, m_wFixedAddr, result));
            if (m_mbxLayer.eState == m_mbxLayer.IDLE && 
                eMbxInvokeId_POLL_RECV_FROM_SLAVE != dwInvokeId && 
                eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE != dwInvokeId &&
                m_poEcDevice->IsSendEnabled())
            {	
                /* toggle repeat request */
                m_mbxLayer.eState = m_mbxLayer.TOGGLED;
                m_mbxLayer.bToggleBit = !m_mbxLayer.bToggleBit;
                EC_T_BYTE byCtrl = (EC_T_BYTE)(ECREG_SYNCMANAGER_ENABLE | ((m_mbxLayer.bToggleBit & 0x01) << 1));
                
                /*EC_T_DWORD dwRetVal = 0;*/

                /* Master toggles repeat request bit and send to slave */
                /*dwRetVal =*/ m_pMaster->QueueRegisterCmdReq(this,
                    bIsInitCmd ? eMbxInvokeId_INITCMD_TOGGLE_REPEAT_REQ : eMbxInvokeId_TOGGLE_REPEAT_REQ,
                    EC_CMD_TYPE_FPWR, m_wFixedAddr, ECREG_SYNCMANAGER_MBX_IN+ECREG_SYNCMANAGER_ACTIVATE, 1, &byCtrl);
                /*This is the timeout for repeating mailbox communication*/
                m_mbxLayer.oTimeout.Start(1000, m_pMaster->GetMsecCounterPtr());
                m_mbxLayer.byIDX = pEcCmdHeader->uCmdIdx.swCmdIdx.byIdx;
            }
            else
            {
                m_bMbxReadStateIsBusy = EC_FALSE;
            }
        }
        else if (pMBox != EC_NULL)
        {
            StopCurrMbxRequest(bIsInitCmd, result, pMBox);
        }
        break;
    case eMbxInvokeId_TOGGLE_REPEAT_REQ:
    case eMbxInvokeId_INITCMD_TOGGLE_REPEAT_REQ:
      if (result == EC_E_NOERROR && pEcCmdHeader && ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 1 && m_mbxLayer.eState == m_mbxLayer.TOGGLED && m_poEcDevice->IsSendEnabled())
      {
          /* Master polls repeat request acknowledge bit */
          m_pMaster->QueueRegisterCmdReq(this,
              bIsInitCmd ? eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES : eMbxInvokeId_TOGGLE_REPEAT_RES, EC_CMD_TYPE_FPRD, 
              m_wFixedAddr, ECREG_SYNCMANAGER_MBX_IN + ECREG_SYNCMANAGER_UC_STATUS, 1, EC_NULL);
      }
      /* Check if the slave is still present.Otherwise we should stop the current pending mbx */
      else if (IsPresentOnBus() && m_mbxLayer.eState == m_mbxLayer.TOGGLED && m_poEcDevice->IsSendEnabled())
      {
          /* Retry to send TOGGLE_REPEAT_REQ */
          EC_T_BYTE byCtrl = (EC_T_BYTE)(ECREG_SYNCMANAGER_ENABLE | ((m_mbxLayer.bToggleBit & 0x01) << 1));
          m_pMaster->QueueRegisterCmdReq(this,
              bIsInitCmd ? eMbxInvokeId_INITCMD_TOGGLE_REPEAT_REQ : eMbxInvokeId_TOGGLE_REPEAT_REQ, 
              EC_CMD_TYPE_FPWR, m_wFixedAddr, ECREG_SYNCMANAGER_MBX_IN+ECREG_SYNCMANAGER_ACTIVATE, 1, &byCtrl);
      }
      else if (m_mbxLayer.eState == m_mbxLayer.TOGGLED)
      {
          m_mbxLayer.eState = m_mbxLayer.IDLE;
          m_bMbxReadStateIsBusy = EC_FALSE;
          StopCurrMbxRequest(bIsInitCmd, EC_E_FRAME_LOST, pMBox);
      }
      break;
    case eMbxInvokeId_TOGGLE_REPEAT_RES:
    case eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES:
      if (result == EC_E_NOERROR && pEcCmdHeader && ETYPE_EC_CMD_GETWKC(pEcCmdHeader) == 1 && m_mbxLayer.eState == m_mbxLayer.TOGGLED)
      {
          EC_T_BYTE byStatus = *(EC_T_PBYTE)EC_ENDOF(pEcCmdHeader);
          if ((byStatus & ECREG_SYNCMANAGER_MBX_TOGGLE) == (m_mbxLayer.bToggleBit << 1) && m_poEcDevice->IsSendEnabled())
          {
              EC_T_MBXINVOKEID    EMbxInvokeId    = eMbxInvokeId_UNKNOWN;
              EC_T_WORD           wMbxILen        = 0;       

              /* Master received toggled repeat request bit */
              /* Master sends mailbox read command again */
              EC_DBGMSG(("CEcMbSlave::ProcessCmdResult() repeating Mbx communication success.\n"));
              m_mbxLayer.oTimeout.Stop();

              m_mbxLayer.eState             = m_mbxLayer.IDLE;
              m_bMbxReadStateIsBusy         = EC_TRUE;
              if (!m_mbxLayer.bRepeatRequested)
              {
                  EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult(): RepeatRequested=1\n"));
              }
              m_mbxLayer.bRepeatRequested   = EC_TRUE;

              /* Repeat Request Acknowledge is set --> read the mailbox as it contains data */
              /* decision whether Current Cmd is InitCmd or not */
              bIsInitCmd = (INITCMD_INACTIVE!=m_wMbxInitCmdsCurrent);

              EMbxInvokeId = bIsInitCmd ? eMbxInvokeId_INITCMD_RECV_FROM_SLAVE : eMbxInvokeId_RECV_FROM_SLAVE;
              wMbxILen = m_mbxILen[m_sMboxFlags.mbxIdx];

              m_pMaster->QueueRegisterCmdReq(this, EMbxInvokeId, EC_CMD_TYPE_FPRD, m_wFixedAddr, 
                  m_mbxIStart[m_sMboxFlags.mbxIdx], wMbxILen, EC_NULL);

          }
          else if (!m_mbxLayer.oTimeout.IsElapsed() && m_poEcDevice->IsSendEnabled())
          {
              m_pMaster->QueueRegisterCmdReq(this,
                  bIsInitCmd ? eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES : eMbxInvokeId_TOGGLE_REPEAT_RES, EC_CMD_TYPE_FPRD, 
                  m_wFixedAddr, ECREG_SYNCMANAGER_MBX_IN + ECREG_SYNCMANAGER_UC_STATUS, 1, EC_NULL);
          }
          else
          {
              EC_DBGMSG(("CEcMbSlave::ProcessCmdResult() Timeout elapsed for repeating Mbx communication timeout.\n"));
              m_mbxLayer.eState             = m_mbxLayer.IDLE;
              m_bMbxReadStateIsBusy         = EC_FALSE;
              StopCurrMbxRequest(bIsInitCmd, EC_E_FRAME_LOST, pMBox);
          }
      }
      /* Check if the slave is still present. Otherwise we should stop the current pending mbx */
      else if (IsPresentOnBus() && m_mbxLayer.eState == m_mbxLayer.TOGGLED)
      {
          /* Retry to send TOGGLE_REPEAT_RES */
          if (!m_mbxLayer.oTimeout.IsElapsed() && m_poEcDevice->IsSendEnabled())
          {
              m_pMaster->QueueRegisterCmdReq(this,
                bIsInitCmd ? eMbxInvokeId_INITCMD_TOGGLE_REPEAT_RES : eMbxInvokeId_TOGGLE_REPEAT_RES, EC_CMD_TYPE_FPRD, 
                m_wFixedAddr, ECREG_SYNCMANAGER_MBX_IN + ECREG_SYNCMANAGER_UC_STATUS, 1, EC_NULL);
          }
          else
          {
              EC_DBGMSG(("CEcMbSlave::ProcessCmdResult() Timeout elapsed for repeating Mbx communication timeout.\n"));
              m_mbxLayer.eState             = m_mbxLayer.IDLE;
              m_bMbxReadStateIsBusy         = EC_FALSE;
              StopCurrMbxRequest(bIsInitCmd, EC_E_FRAME_LOST, pMBox);
          }
      }
      else if (m_mbxLayer.eState == m_mbxLayer.TOGGLED)
      {
          m_mbxLayer.eState = m_mbxLayer.IDLE;
          m_bMbxReadStateIsBusy = EC_FALSE;
          StopCurrMbxRequest(bIsInitCmd, EC_E_FRAME_LOST, pMBox);
      }
      break;    
    case eMbxInvokeId_INVALIDATED_RECV_FROM_SLAVE:
    case eMbxInvokeId_INVALIDATED_SEND_TO_SLAVE:
      /* nothing to do here, it was already handled when the cmd was thrown away */
      break;
  
    case eMbxInvokeId_SEND_FILL_TO_SLAVE:
      break;
  
    default:      
        OsDbgAssert(EC_FALSE);
    }
Exit:
    return;
}

/********************************************************************************/
/** \brief Determines if the EtherCAT slave has an mailbox.
*
* \return
*/
EC_T_BOOL   CEcMbSlave::HasMailBox()
{   
    return m_mbxOLen[m_sMboxFlags.mbxIdx] > 0 && m_mbxILen[m_sMboxFlags.mbxIdx] > 0;

}

/********************************************************************************/
/** \brief Determine mailbox output length.
*
* \return
*/
EC_T_WORD   CEcMbSlave::GetMBoxOutCmdLength(EC_T_WORD length)
{
    if (m_sMboxFlags.bMbxOutShortSend)
        return length;
    else
        return m_mbxOLen[m_sMboxFlags.mbxIdx];
}

/********************************************************************************/
/** \brief Sends the queued CoE commands to the slave
*
* \return
*/
EC_T_VOID   CEcMbSlave::SendQueuedCoECmds()
{
    /* Mbx SyncManagers are not ready or slave is in INIT or slave is not present */
    if ((3 != (m_wSyncManagerEnabled & 3)) || (DEVICE_STATE_INIT == GetDeviceState()) || !IsPresentOnBus())
    {
        /* clear all pending CoE requests */
        /* clear current pending CoE request */
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid )
        {
            MailboxRes(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, (IsPresentOnBus()?EC_E_INVALID_SLAVE_STATE:EC_E_SLAVE_NOT_PRESENT));
            ClearCurrPendCoeMbxCmdDesc();
        }
        /* clear pending FIFO */
        while ((m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid == EC_FALSE) 
            && m_pCoEDesc->pPendMailboxCmdFifo->RemoveNoLock((m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd)))
        {
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid   = EC_TRUE;

            MailboxRes(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd, (IsPresentOnBus()?EC_E_INVALID_SLAVE_STATE:EC_E_SLAVE_NOT_PRESENT));
            ClearCurrPendCoeMbxCmdDesc();
        }

        /* we're done */
        return;
    }

    if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
    {   /* still waiting for a response to a previous CoE mailbox */
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
        {/* timeout has elapsed */
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedCoECmds() m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
            
            /* send an abort cmd to the slave, but just in case this isn't the already the abort */
            if (EC_E_NOERROR == m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode)
            {
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_TIMEOUT, SDO_ABORTCODE_TIMEOUT);
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_TIMEOUT;
            }
            else
            {
                /* terminate mailbox request */
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_TIMEOUT);
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOERROR;
            }
            return;
        }
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx && m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.IsElapsed())
        {   /* timeout has elapsed */
            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter > 0 )
            {
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_FALSE;
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bSendEnabled   = EC_TRUE;
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter--; 
            }
            else
            {/* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pCoe->safe */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedCoECmds() m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_TIMEOUT, SDO_ABORTCODE_TIMEOUT);
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_TIMEOUT;
                return;
            }
        }
        if (((EC_T_MBXTFERP*)m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pvContext)->bAbort)
        {
            /* send an abort cmd to the slave, but just in case this isn't the already the abort */
            if (EC_E_NOERROR == m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode)
            {
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_TIMEOUT, SDO_ABORTCODE_TIMEOUT);
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_TIMEOUT;
            }
            else
            {
                /* terminate mailbox request */
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_TIMEOUT);
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOERROR;
            }
            return;
        }
    }

    /* return if repeating mailbox communication is running */
    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED)
        return;

    /* don't start mailbox transfer if master is in transition or InitCmd are active */
    if (!m_pMaster->m_bStateMachIsStable || !SlaveStateMachIsStable())
    {
        PEcMailboxCmd   pNextCmd = EC_NULL;
        
        /* check if timeout for next pending cmd is already elapsed */
        if (m_pCoEDesc->pPendMailboxCmdFifo->PeakNoLock(pNextCmd) && pNextCmd->oTimeout.IsElapsed())
        {
            /* remove cmd from queue */
            if (m_pCoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pNextCmd))
            {
                MailboxRes(pNextCmd, EC_E_TIMEOUT);
            }
        }
        return;
    }
    /* don't start mailbox transfer if another mailbox transfer is retrying */
    if (  ((EC_NULL == m_pCoEDesc))	/* protect against compiler error if no mailbox protocol is supported */
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry)
#endif
#ifdef INCLUDE_SOE_SUPPORT             
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) 
#endif
#ifdef INCLUDE_VOE_SUPPORT 
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry)
#endif
#ifdef INCLUDE_AOE_SUPPORT 
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry)
#endif
       )
    {
        return;
    }
    /* don't start mailbox transfer if serialization is active and another mailbox transfer is running */
    if (m_bSerializeMbxProtocols && (
          ((EC_NULL == m_pCoEDesc)) /* protect against compiler error if no mailbox protocol is supported */
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_SOE_SUPPORT             
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid) 
#endif
#ifdef INCLUDE_VOE_SUPPORT 
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_AOE_SUPPORT 
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
#endif
       ))
    {
        return;
    }
    /* Check if temp queued mailbox exist to send now */
    if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending )
    {
        /* Check if mailbox with more than 4 byte data is pending */
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePendingData)
        {
            EC_T_WORD nMaxData = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SDO_MBOX_LEN + SDO_DOWNLOAD_SEGMENT_MAX_DATA);
            EC_T_WORD nSendData = EC_MIN(nMaxData, (EC_T_WORD)(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->length - m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset));

            EC_T_BYTE* pData = &(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData[m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset]);
            EcatMbxSendCmdReq(
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, 
                &pData[SDO_DOWNLOAD_SEGMENT_MAX_DATA], 
                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcMbxHeader, 
                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcCanHeader, 
                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
               ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
               ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
               ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
               ,EC_NULL
               ,EC_NULL
#endif
               );
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset += nSendData;
        }
        else
        {
            EcatMbxSendCmdReq(
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, 
                EC_NULL, 
                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcMbxHeader, 
                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcCanHeader, 
                &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
               ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
               ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
               ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
               ,EC_NULL
               ,EC_NULL
#endif
               );
        }
        /* start timeout helper for queued mbx cmd */
        if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsDefaultTimeout())
        {
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Restart();
        }
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bFramePending = EC_FALSE;
    }
    if (!m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
    {
        ClearCurrPendCoeMbxCmdDesc();
        if (m_pCoEDesc->pPendMailboxCmdFifo->RemoveNoLock(m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd))
        {
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid       = EC_TRUE;
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bSendEnabled   = EC_TRUE;
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwSdoAbortErrorCode = EC_E_NOERROR;
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;

            m_dwLastOnTimerCall = 0;        /* force mbx polling */
            /* first check if timeout has elapsed */
            if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
            {/* timeout has elapsed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedCoECmds() m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_TIMEOUT);
                return;
            }
        }
    }

    if (m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bSendEnabled)
    {
        /* not waiting for any command responses and a pending command exists */
        EC_T_VOID* pData = EC_NULL;
        
        ETHERCAT_MBOX_HEADER    mbx;
        ETHERCAT_CANOPEN_HEADER can;
        MBXHDR_CLR(&mbx);
        COEHDR_CLR(&can);
        
        m_pCoEDesc->CurrPendCoeMbxCmdDesc.bSendEnabled = EC_FALSE;
        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_CANOPEN);
        
        PEcMailboxCmd pCmd = m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd;

        if (   (pCmd->dwInvokeId == eMbxInvokeId_INITCMD_RECV_FROM_SLAVE) 
            || (pCmd->dwInvokeId == eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE) 
            || (pCmd->dwInvokeId == eMbxInvokeId_INITCMD_SEND_TO_SLAVE) 
            )
        {
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd = EC_TRUE;

            /* deny sdo complete access to subindex for InitCmds */
            if ((1 == (pCmd->dwMailboxFlags & EC_MAILBOX_FLAG_SDO_COMPLETE)) && (0 != pCmd->sIndex.subIndex))
            {
                StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_INVALIDINDEX);
                return;
            }
        }
        OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd || (pCmd->wMbxType == EC_ECMBOXHDR_GET_MBXTYPE(&mbx)));
        
        if (pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO)
        {
            EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOREQ);
            OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd || (pCmd->wMbxSubType == ETHERCAT_CANOPEN_TYPE_SDOREQ) || (pCmd->wMbxSubType == ETHERCAT_CANOPEN_TYPE_SDORES));
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + EC_SDO_HDR_LEN));
            
            EC_ECSDOHDR_SET_INDEX(&(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader), (pCmd->sIndex.index));
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.SubIndex = pCmd->sIndex.subIndex;

            if (pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD)
            {
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Iuq.Ccs      = SDO_CCS_INITIATE_UPLOAD;
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Iuq.Complete = pCmd->dwMailboxFlags & EC_MAILBOX_FLAG_SDO_COMPLETE;/* combine flags and value info */
            }
            else if (pCmd->wMbxCmdType == EC_MAILBOX_CMD_DOWNLOAD)
            {
                EC_T_WORD nMaxData = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] - ETHERCAT_MIN_SDO_MBOX_LEN);
                
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Ccs      = SDO_CCS_INITIATE_DOWNLOAD;            
                m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Complete = pCmd->dwMailboxFlags & EC_MAILBOX_FLAG_SDO_COMPLETE;/* combine flags and value info */
                
                if (pCmd->length > nMaxData)
                {
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Expedited  = EC_FALSE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.SizeInd    = EC_TRUE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Size       = 0;
                    EC_ECSDOHDR_SET_SDODATA(&(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader), (pCmd->length));
                    pData                           = pCmd->pbyData;
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + nMaxData));
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset            = nMaxData;
                }
                else if (pCmd->length > sizeof(EC_T_DWORD)) /* pCmd->length > 4 */
                {
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Expedited  = EC_FALSE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.SizeInd    = EC_TRUE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Size       = 0;
                    EC_ECSDOHDR_SET_SDODATA(&(m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader), (pCmd->length)); 
                    pData                           = pCmd->pbyData;
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + pCmd->length));
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset            = (EC_T_WORD) pCmd->length;
                }
                else if (pCmd->length > 0 )
                {
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Expedited  = EC_TRUE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.SizeInd    = EC_TRUE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Size       = (EC_T_BYTE)(sizeof(EC_T_DWORD) - pCmd->length);
                    OsMemcpy( ((EC_T_BYTE*)&m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader) + SDO_HDR_DATA_OFFSET,   /* &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.dwSdoData (avoid diab compiler warning)*/
                        m_pCoEDesc->CurrPendCoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData,  
                        pCmd->length );
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset            = (EC_T_WORD)pCmd->length;
                }
                else
                {
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Expedited  = EC_TRUE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.SizeInd    = EC_FALSE;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader.uHdr.Idq.Size       = 0;
                    m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset            =  (EC_T_WORD)pCmd->length;
                }
            }
            else
            {
                OsDbgAssert(EC_FALSE);
            }

            /* send to slave */
            EcatMbxSendCmdReq(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, pData, &mbx, &can, &m_pCoEDesc->CurrPendCoeMbxCmdDesc.EcSdoHeader
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        else if (pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO_INFO_LIST && pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD )
        {
            EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOINFO);
            OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd || (pCmd->wMbxSubType == EC_ECCOEHDR_GET_COETYPE(&can)));
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + ETHERCAT_SDO_INFO_LISTREQ_LEN));
            
            ETHERCAT_SDO_INFO_HEADER info;
            SDOINFOHDR_CLR(&info);
            
            info.OpCode         = ECAT_COE_INFO_OPCODE_LIST_Q;
            EC_SDOINFOLIST_SET_LISTTYPE(&(info.uInfo.List), pCmd->sIndex.index);

            EcatMbxSendCmdReq(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, &info, &mbx, &can, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        else if (pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO_INFO_OBJ && pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD )
        {
            EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOINFO);
            OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd || (pCmd->wMbxSubType == EC_ECCOEHDR_GET_COETYPE(&can)));
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + ETHERCAT_SDO_INFO_OBJREQ_LEN));
            
            ETHERCAT_SDO_INFO_HEADER info;
            SDOINFOHDR_CLR(&info);
            
            info.OpCode         = ECAT_COE_INFO_OPCODE_OBJ_Q;
            EC_SDOINFOOBJ_SET_INDEX(&(info.uInfo.Obj), pCmd->sIndex.index);
            
            EcatMbxSendCmdReq(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, &info, &mbx, &can, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        else if (pCmd->cmdId == MAILBOXCMD_CANOPEN_SDO_INFO_ENTRY && pCmd->wMbxCmdType == EC_MAILBOX_CMD_UPLOAD )
        {
            EC_ECCOEHDR_SET_COETYPE(&can, ETHERCAT_CANOPEN_TYPE_SDOINFO);
            OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd || (pCmd->wMbxSubType == EC_ECCOEHDR_GET_COETYPE(&can)));
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (ETHERCAT_CANOPEN_HEADER_LEN + ETHERCAT_SDO_INFO_ENTRYREQ_LEN));
            ETHERCAT_SDO_INFO_HEADER info;
            
            SDOINFOHDR_CLR(&info);
            info.OpCode         = ECAT_COE_INFO_OPCODE_ENTRY_Q;
            EC_SDOINFOENTRY_SET_INDEX(&(info.uInfo.Entry), (pCmd->sIndex.index));
            info.uInfo.Entry.SubIdx   = pCmd->sIndex.subIndex;
            info.uInfo.Entry.ValueInfo= pCmd->sIndex.valueInfo;
            
            EcatMbxSendCmdReq(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, &info, &mbx, &can, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
#ifdef INCLUDE_COE_PDO_SUPPORT
        else if (pCmd->cmdId == MAILBOXCMD_CANOPEN_RX_PDO && pCmd->wMbxCmdType == EC_MAILBOX_CMD_DOWNLOAD )
        {
            can.wCoeType        = ETHERCAT_CANOPEN_TYPE_RXPDO;
            OsDbgAssert(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd || (pCmd->wMbxSubType == EC_ECCOEHDR_GET_COETYPE(&can)));
            EC_ECMBOXHDR_SET_LENGTH(&mbx, ETHERCAT_CANOPEN_HEADER_LEN);
            pData      = pCmd->pbyData;
            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_ECMBOXHDR_GET_LENGTH(&mbx) + pCmd->length));
            can.wCoeNumber = pCmd->dwNumber;
            m_pCoEDesc->CurrPendCoeMbxCmdDesc.nOffset = (EC_T_WORD) pCmd->length;
            
            EcatMbxSendCmdReq(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, &pData, &mbx, EC_NULL, &can);
        }
#endif
        else
        {
            OsDbgAssert(EC_FALSE);
            StopCurrCoEMbxRequest(m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsInitCmd, EC_E_INVALIDCMD);
        }
    } /* if not waiting for any command responses and a pending command exists */   
}

/********************************************************************************/
/** \brief Sends the queued FoE commands to the slave
*
* \return
*/
#if (defined INCLUDE_FOE_SUPPORT)
EC_T_VOID   CEcMbSlave::SendQueuedFoECmds()
{
EC_T_DWORD      dwRes      = EC_E_NOERROR;
EC_T_BOOL       bIsInitCmd = EC_FALSE;
EC_T_BOOL       bResend    = EC_FALSE;
PEcMailboxCmd   pCmd       = m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd;
EC_T_MBXTFERP*  pMbxTferPriv = EC_NULL;

    if (EC_NULL != pCmd)
    {
        pMbxTferPriv = (EC_T_MBXTFERP*)pCmd->pvContext;
    }

    /* Mbx SyncManagers are not ready or slave is in INIT or slave is not present */
    if (((3 != (m_wSyncManagerEnabled & 3)) || (DEVICE_STATE_INIT == GetDeviceState()) || !IsPresentOnBus())
        && (DEVICE_STATE_BOOTSTRAP != GetDeviceState()))
    {
        /* clear all pending FoE requests */
        /* clear current pending FoE request */
        if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
        {
            MailboxRes(pCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE: EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendFoeMbxCmdDesc();
        }
        /* clear pending FIFO */
        while (!m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid && m_pFoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd))
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd = pCmd;
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid = EC_TRUE;
            
            MailboxRes(pCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE: EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendFoeMbxCmdDesc();
        }
        
        /* we're done */
        return;
    }
    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid && !m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled)
    { /* still waiting for a response to a previous FoE mailbox */
        if (pCmd->oTimeout.IsElapsed())
        {   /* timeout has elapsed */
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedFoECmds() pCmd->oTimeout.IsElapsed() stop MbxRequest, send error to Mbx \n"));
            /* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pFoe->safe */
            StopCurrFoEMbxRequest(EC_FALSE, EC_E_TIMEOUT, ECAT_FOE_ERRCODE_NOTDEFINED);
            return;
        }
        if (pCmd->bRetrySendMbx && pCmd->oRetryTimeout.IsElapsed())
        {   /* timeout has elapsed */
            if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter > 0)
            {
                pCmd->oRetryTimeout.Stop();
                pCmd->bRetrySendMbx = EC_FALSE;
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled = EC_TRUE;
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter--; 
                bResend = EC_TRUE;
            }
            else
            {/* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pFoe->safe */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedFoECmds() pCmd->oTimeout.IsElapsed() RetryTimeout.IsElapsed! stop MbxRequest, send error to Mbx \n"));
                StopCurrFoEMbxRequest(EC_FALSE, EC_E_TIMEOUT, ECAT_FOE_ERRCODE_NOTDEFINED);
                return;
            }
        }
        if ((EC_NULL != pMbxTferPriv) && pMbxTferPriv->bAbort)
        {
            StopCurrFoEMbxRequest(bIsInitCmd, EC_E_CANCEL);
            return;
        }
    }
    /* don't start mailbox transfer if master is in transition or InitCmd are running */
    if (!m_pMaster->m_bStateMachIsStable || !SlaveStateMachIsStable())
    {
        PEcMailboxCmd   pNextCmd = EC_NULL;
        
        /* check if timeout for next pending cmd is already elapsed */
        if (m_pFoEDesc->pPendMailboxCmdFifo->PeakNoLock(pNextCmd) && pNextCmd->oTimeout.IsElapsed())
        {
            /* remove cmd from queue */
            if (m_pFoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pNextCmd))
            {
                MailboxRes(pNextCmd, EC_E_TIMEOUT);
            }
        }
        return;
    }
    /* don't start mailbox transfer if another mailbox transfer is retrying */
    if (  ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)
#ifdef INCLUDE_SOE_SUPPORT             
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry) 
#endif
#ifdef INCLUDE_VOE_SUPPORT 
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry)
#endif
#ifdef INCLUDE_AOE_SUPPORT 
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bRetry)
#endif
       )
    {
        return;
    }
    /* don't start mailbox transfer if serialization is active and another mailbox transfer is running */
    if (m_bSerializeMbxProtocols && (
        ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
#ifdef INCLUDE_SOE_SUPPORT             
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid) 
#endif
#ifdef INCLUDE_VOE_SUPPORT 
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_AOE_SUPPORT 
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bIsValid)
#endif
       ))
    {
        return;
    }
    /* Check if temp queued mailbox exist to send now */
    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending)
    {
        EC_T_BYTE* pByte = pCmd->pbyData;
        if (pCmd->wMbxSubType == ECAT_FOE_OPCODE_ACK)
        {
            dwRes = EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        else
        {
            dwRes = EcatMbxSendCmdReq(bIsInitCmd, &pByte[m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset], &m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        if (EC_E_NOERROR != dwRes)
        {
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedFoECmds(): ERROR send next data packet! Result is 0x%lx\n", dwRes));
        }
        else
        {
            /* start timeout helper for queued mbx cmd */
            if (pCmd->oTimeout.IsDefaultTimeout())
            {
                pCmd->oTimeout.Restart();
            }
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFramePending = EC_FALSE;
        }
    }
    /* get next Cmd from Fifo */
    if (!m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
    {
        if (m_pFoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pCmd))
        {
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd = pCmd;
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid = EC_TRUE;
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled = EC_TRUE;
            m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;

            m_dwLastOnTimerCall = 0;        /* force mbx polling */

            /* stop on timeout (and retry = 0) */
            if (pCmd->oTimeout.IsElapsed())
            {
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedFoECmds() pCmd->oTimeout.IsElapsed() stop MbxRequest, don't send error to Mbx \n"));
                StopCurrFoEMbxRequest(EC_FALSE, EC_E_TIMEOUT);
                return;
            }
        }
    }

    /* send Cmd */
    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled)
    {
        ETHERCAT_MBOX_HEADER    mbx;
        EC_FOE_HDR              FoE;
        EC_T_BYTE*              pByte = pCmd->pbyData;

        MBXHDR_CLR(&mbx);
        FOEHDR_CLR(&FoE);
        
        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);
        m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled = EC_FALSE;

        if (bResend)
        {
            switch (pCmd->wMbxSubType)
            {
            case ECAT_FOE_OPCODE_RRQ:
            case ECAT_FOE_OPCODE_WRQ:
                {
                    OsDbgAssert(bIsInitCmd || (pCmd->wMbxSubType == ECAT_FOE_OPCODE_RRQ) || (pCmd->wMbxSubType == ECAT_FOE_OPCODE_WRQ));
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + pCmd->FoE.dwFileNameLength));
                    EC_ECFOEHDR_SET_OPCODE(&FoE, (EC_T_BYTE)((pCmd->cmdId==MAILBOXCMD_ECAT_FOE_FOPENREAD) ? ECAT_FOE_OPCODE_RRQ : ECAT_FOE_OPCODE_WRQ));
                    EC_ECFOEHDR_SET_PASSWORD(&FoE, (pCmd->FoE.dwPassword));
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
                    
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bRead                         = (pCmd->cmdId == MAILBOXCMD_ECAT_FOE_FOPENREAD);
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber            = 0;
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber   = 0;
                    
                    /* send to slave */
                    EcatMbxSendCmdReq(bIsInitCmd, pCmd->FoE.achFileName, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,EC_NULL
                                     ,EC_NULL
#endif
                                     );
                } break;
                
            case ECAT_FOE_OPCODE_DATA:
                {
                    EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);
                    /* send full mailbox */
                    if (pCmd->length >= GetFoEDataSize(VG_OUT)+m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset)
                    {
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + GetFoEDataSize(VG_OUT)));
                    }
                    /* send less mailbox */
                    else if (pCmd->length > m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset)
                    {
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + pCmd->length - m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset));
                    }
                    /* send zero mailbox */
                    else if (pCmd->length == m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset)
                    {
                        EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN));
                    }
                    EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_DATA);
                    EC_ECFOEHDR_SET_PACKETNO(&FoE, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber + 1);
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
                    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend = EC_TRUE;
                    EcatMbxSendCmdReq(bIsInitCmd, &pByte[m_pFoEDesc->CurrPendFoeMbxCmdDesc.nOffset], &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,EC_NULL
                                 ,EC_NULL
#endif
                                 );
                } break;
                
            case ECAT_FOE_OPCODE_ACK:
                {
                    SendFoEAckToSlave(bIsInitCmd, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber);
                } break;
                
            case ECAT_FOE_OPCODE_ERR:
                {
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, EC_FOE_HDR_LEN);
                    EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_ERR);
                    EC_ECFOEHDR_SET_PACKETNO(&FoE, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber);
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                    OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
                    EcatMbxSendCmdReq(bIsInitCmd, EC_NULL, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,EC_NULL
                                 ,EC_NULL
#endif
                                 );
                } break;
                
            case  ECAT_FOE_OPCODE_BUSY:
            default:
                OsDbgAssert(EC_FALSE);
                break;
            }
        } /* bResend */
        else
        {
            switch (pCmd->cmdId)
            {
            case MAILBOXCMD_ECAT_FOE_FOPENREAD:
            case MAILBOXCMD_ECAT_FOE_FOPENWRITE:
                switch (pCmd->wMbxSubType)
                {
                case ECAT_FOE_OPCODE_RRQ:
                case ECAT_FOE_OPCODE_WRQ:
                    {
                OsDbgAssert(bIsInitCmd || (pCmd->wMbxSubType == ECAT_FOE_OPCODE_RRQ) || (pCmd->wMbxSubType == ECAT_FOE_OPCODE_WRQ));
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + pCmd->FoE.dwFileNameLength));
                EC_ECFOEHDR_SET_OPCODE(&FoE, (EC_T_BYTE)((MAILBOXCMD_ECAT_FOE_FOPENREAD == pCmd->cmdId) ? ECAT_FOE_OPCODE_RRQ : ECAT_FOE_OPCODE_WRQ));
                EC_ECFOEHDR_SET_PASSWORD(&FoE, pCmd->FoE.dwPassword);
                OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
                
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.bRead                         = (pCmd->cmdId == MAILBOXCMD_ECAT_FOE_FOPENREAD);
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastPacketNumber            = 0;
                m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber   = 0;

                /* send to slave */
                EcatMbxSendCmdReq(bIsInitCmd, pCmd->FoE.achFileName, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,EC_NULL
                                 ,EC_NULL
#endif
                                 );
                    } break;
                case ECAT_FOE_OPCODE_DATA:
                    {
                        /* send to slave */
                        if (eMbxTferType_FOE_SEG_DOWNLOAD == pMbxTferPriv->MbxTfer.eMbxTferType)
                        {
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;

                            EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_FILEACCESS);
                            EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_FOE_HDR_LEN + pMbxTferPriv->MbxTfer.dwDataLen));
                            EC_ECFOEHDR_SET_OPCODE(&FoE, ECAT_FOE_OPCODE_DATA);
                            EC_ECFOEHDR_SET_PACKETNO(&FoE, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber + 1);
                            OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcFoeHeader, &FoE, sizeof(EC_FOE_HDR));
                            OsMemcpy(&m_pFoEDesc->CurrPendFoeMbxCmdDesc.EcMbxHeader, &mbx, sizeof(ETHERCAT_MBOX_HEADER));
                            m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend = EC_TRUE;
                            EcatMbxSendCmdReq(bIsInitCmd, pByte, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,&FoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,EC_NULL
                                 ,EC_NULL
#endif
                                 );

                            if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsDefaultTimeout())
                            {
                                m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Restart();
                            }
                            else
                            {
                                m_pFoEDesc->CurrPendFoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.Start(m_pMaster->GetMbxCmdTimeout() | 0x80000000, m_pMaster->GetMsecCounterPtr());
                            }
                        }
                    } break;
                } break; /* switch (pMbxTferPriv->oEcMbxCmd.wMbxSubType) */
            default:
                OsDbgAssert(EC_FALSE);
            } /* switch (pCmd->cmdId) */
        } /* !bResend */
    }
}
#endif /* INCLUDE_FOE_SUPPORT */



/********************************************************************************/
/** \brief Sends the queued SoE commands to the slave
*
* \return
*/
#ifdef INCLUDE_SOE_SUPPORT
EC_T_VOID   CEcMbSlave::SendQueuedSoECmds()
{
    EC_T_BOOL   bIsInitCmd = EC_FALSE;
    EC_T_BOOL   bResend    = EC_FALSE;
    EC_T_WORD   wFragmentsLeft = 0;

    /* Mbx SyncManagers are not ready or slave is in INIT or slave is not present */
    if ((3 != (m_wSyncManagerEnabled & 3)) || (DEVICE_STATE_INIT == GetDeviceState()) || !IsPresentOnBus())
    {
        /* clear all pending SoE requests */
        /* clear current pending SoE request */
        if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
        {
            MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE : EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendSoeMbxCmdDesc();
        }
        /* clear pending FIFO */
        while (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid == EC_FALSE && m_pSoEDesc->pPendMailboxCmdFifo->RemoveNoLock((m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd)))
        {
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid   = EC_TRUE;
            
            MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE : EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendSoeMbxCmdDesc();
        }
        
        /* we're done */
        return;
    }
    
    if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid )
    { /* still waiting for a response to a previous SoE mailbox */
        if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
        {   /* timeout has elapsed */
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedSoECmds() m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
            MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
            ClearCurrPendSoeMbxCmdDesc();
            return;
        }
        if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx && m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.IsElapsed())
        {   /* timeout has elapsed */
            if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry > 0 )
            {
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSendEnabled = EC_TRUE;
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx  = EC_FALSE;
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry--; 
                bResend = EC_TRUE;
            }
            else

            {/* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pCoe->safe */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedSoECmds() m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
                ClearCurrPendSoeMbxCmdDesc();
                return;
            }
        }
    }
    /* don't start mailbox transfer if master is in transition or InitCmd are running */
    if (!m_pMaster->m_bStateMachIsStable || !SlaveStateMachIsStable())
    {
        PEcMailboxCmd   pNextCmd = EC_NULL;
        
        /* check if timeout for next pending cmd is already elapsed ? */
        if (m_pSoEDesc->pPendMailboxCmdFifo->PeakNoLock(pNextCmd) && pNextCmd->oTimeout.IsElapsed())
        {
            /* remove cmd from queue */
            if (m_pSoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pNextCmd))
            {
                MailboxRes(pNextCmd, EC_E_TIMEOUT);
            }
        }
        return;
    }
    /* don't start mailbox transfer if another mailbox transfer is retrying */
    if (  ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry) 
#ifdef INCLUDE_FOE_SUPPORT             
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) 
#endif
#ifdef INCLUDE_VOE_SUPPORT 
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry)
#endif
#ifdef INCLUDE_AOE_SUPPORT 
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bRetry)
#endif
       )
    {
        return;
    }
    /* don't start mailbox transfer if serialization is active and another mailbox transfer is running */
    if (m_bSerializeMbxProtocols && (
        ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid) 
#ifdef INCLUDE_FOE_SUPPORT             
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid) 
#endif
#ifdef INCLUDE_VOE_SUPPORT 
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_AOE_SUPPORT 
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bIsValid)
#endif
       ))
    {
        return;
    }
    if ((m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid == EC_FALSE))
    {
        if (m_pSoEDesc->pPendMailboxCmdFifo->RemoveNoLock((m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd)))
        {
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid      = EC_TRUE;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSendEnabled  = EC_TRUE;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry         = m_wRetryAccessCount;
            
            m_dwLastOnTimerCall = 0;        /* force mbx polling */
            
            /*first check if timeout has elapsed */
            if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
            {   /* timeout has elapsed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedSoECmds() m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
                ClearCurrPendSoeMbxCmdDesc();
                return;
            }
        }
    }

    if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid == EC_TRUE && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSendEnabled == EC_TRUE)
    {
        ETHERCAT_MBOX_HEADER    mbx;
        EC_SOE_HDR     soe;
        MBXHDR_CLR(&mbx);
        SOEHDR_CLR(&soe);
        
        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_SOE);
        
        /* store data also to CurrPendSoeMbxCmdDesc , will be needed later by ProcessSoEResponse() in order to check the response*/
        PEcMailboxCmd pCmd = m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd;
        m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSendEnabled  = EC_FALSE;

        /* fill the soe header with default values, some will be changed later */
        soe.Incomplete        = 0;                   
        soe.Error             = 0;                   
        soe.DriveNo           = pCmd->byDriveNo;     
        soe.uIdnFragLeft.wIdn = pCmd->wIDN;          
        soe.byElements        = *pCmd->pbyElementFlags;

        if (bResend && m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete)
        {
            /* resend last fragment */
            
            EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_SOE);
            if (m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft > 0 )
            {
                EC_ECSOEHDR_SET_FRAGLEFT( &soe, m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft );
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN + GetSoEDataSize(VG_OUT)));
            }
            else
            {
                soe.uIdnFragLeft.wIdn = (unsigned short) pCmd->wIDN;
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete = soe.Incomplete = EC_FALSE;
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN + pCmd->length - m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset));
            }
            EcatMbxSendCmdReq(bIsInitCmd, pCmd->pbyData + m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,&soe
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        /* not waiting for any command responses and a pending command is in the queue */
        else
        {
            if ((pCmd->cmdId != MAILBOXCMD_ECAT_SOE_READREQUEST) &&
                (pCmd->cmdId != MAILBOXCMD_ECAT_SOE_WRITEREQUEST) &&
                (pCmd->cmdId != MAILBOXCMD_ECAT_SOE_ABORT_COMMAND))
            {
                /* this cmdId (Service Channel request) is for a master not allowed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedSoECmds() cmdId 0x%X is not allowed for an EtherCAT master.\n",pCmd->cmdId));
                MailboxRes(m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
                ClearCurrPendSoeMbxCmdDesc();
                return;
            }
            
            /* common code for all cmdId */
            OsDbgAssert(bIsInitCmd || (pCmd->wMbxSubType == ECAT_SOE_OPCODE_RRQ) || (pCmd->wMbxSubType == ECAT_SOE_OPCODE_WRQ));

            /* fill the soe header with default values, some will be changed later */
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete        = soe.Incomplete        = 0;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Error             = soe.Error             = 0;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.DriveNo           = soe.DriveNo           = pCmd->byDriveNo;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.uIdnFragLeft.wIdn = soe.uIdnFragLeft.wIdn = pCmd->wIDN;

            /* element flags */
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.byElements        = soe.byElements        = *pCmd->pbyElementFlags;

            m_pSoEDesc->CurrPendSoeMbxCmdDesc.retry          = m_wRetryAccessCount;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft = 0;
            m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset        = 0;

            /* code is different for different cmdId */
            switch (pCmd->cmdId)
            {
            case MAILBOXCMD_ECAT_SOE_READREQUEST:

                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN));
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType = ECAT_SOE_OPCODE_RRQ;
                soe.OpCode = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.OpCode = ECAT_SOE_OPCODE_RRQ;
                break;
            case MAILBOXCMD_ECAT_SOE_WRITEREQUEST:

                if (pCmd->length <= GetSoEDataSize(VG_OUT))
                {
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN + pCmd->length ));
                }
                else
                {
                    EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN + GetSoEDataSize(VG_OUT)));
                    m_pSoEDesc->CurrPendSoeMbxCmdDesc.nOffset	+= GetSoEDataSize(VG_OUT);
                }

                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType = ECAT_SOE_OPCODE_WRQ;
                soe.OpCode = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.OpCode = ECAT_SOE_OPCODE_WRQ;
                /* Calculate the count of fragments */

                wFragmentsLeft = (EC_T_WORD)(pCmd->length / GetSoEDataSize(VG_OUT));
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft = wFragmentsLeft;
                 /* If we can send all data in one fragment transmit the IDN*/
                if (0 == wFragmentsLeft)
                {
                    m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete = soe.Incomplete = EC_FALSE;
                }
                /* We need fragmented data */
                else
                {
                    soe.uIdnFragLeft.wFragmentsLeft = wFragmentsLeft;                    
                    m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete = soe.Incomplete = EC_TRUE;
                }
                break;
            case MAILBOXCMD_ECAT_SOE_ABORT_COMMAND:
                /* OpCode = Write and Data = 0 indicates an abort command */
                EC_ECMBOXHDR_SET_LENGTH(&mbx, (EC_T_WORD)(EC_SOE_HDR_LEN));
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.pCurrPendQueuedMbxCmd->wMbxSubType = ECAT_SOE_OPCODE_WRQ;
                soe.OpCode = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.OpCode = ECAT_SOE_OPCODE_WRQ;
                soe.Error = m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Error = 0;
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.wFragmentsLeft = 0;
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.Incomplete        = soe.Incomplete        = EC_FALSE;
                m_pSoEDesc->CurrPendSoeMbxCmdDesc.EcSoeHeader.uIdnFragLeft.wIdn = soe.uIdnFragLeft.wIdn = pCmd->wIDN;
                break;
            }
            
            /* common code for all cmdId */
            /* send to slave */
            EcatMbxSendCmdReq(bIsInitCmd, pCmd->pbyData, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,&soe
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
    }
}
#endif

/********************************************************************************/
/** \brief Sends the queued VoE commands to the slave
*
* \return
*/
#if defined INCLUDE_VOE_SUPPORT
EC_T_VOID CEcMbSlave::SendQueuedVoECmds()
{
    EC_T_BOOL   bIsInitCmd = EC_FALSE;
    EC_T_BOOL   bResend    = EC_FALSE;

    /* Mbx SyncManagers are not ready or slave is in INIT or slave is not present */
    if ((3 != (m_wSyncManagerEnabled & 3)) || (DEVICE_STATE_INIT == GetDeviceState()) || !IsPresentOnBus())
    {
        /* clear current pending VoE request */
        if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid )
        {
            MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE : EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendVoeMbxCmdDesc();
        }
        /* iterate through the pending VoE command queue and clear all mailbox commands */
        while (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid == EC_FALSE && m_pVoEDesc->pPendMailboxCmdFifo->RemoveNoLock(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd))
        {
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid   = EC_TRUE;
            MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE : EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendVoeMbxCmdDesc();
        }

        /* we're done */
        return;
    }
    
    if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid )
    { /* still waiting for a response to a previous VoE mailbox */
        if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
        {   /* timeout has elapsed */
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedVoECmds() m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
            MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
            ClearCurrPendVoeMbxCmdDesc();
            return;
        }
        if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx && m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.IsElapsed())
        {   /* timeout has elapsed */
            if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.dwRetryCounter > 0 )
            {
                m_pVoEDesc->CurrPendVoeMbxCmdDesc.bSendEnabled = EC_TRUE;
                m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
                m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_FALSE;
                m_pVoEDesc->CurrPendVoeMbxCmdDesc.dwRetryCounter--; 
                bResend = EC_TRUE;
            }
            else
            {/* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pCoe->safe */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedVoECmds() m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_VOE_MBX_WKC_ERROR);
                ClearCurrPendVoeMbxCmdDesc();
                return;
            }
        }
    }
    /* don't start mailbox transfer if master is in transition or InitCmd are running */
    if (!m_pMaster->m_bStateMachIsStable || !SlaveStateMachIsStable())
    {
        PEcMailboxCmd   pNextCmd = EC_NULL;
        
        /* check if timeout for next pending cmd is already elapsed ? */
        if (m_pVoEDesc->pPendMailboxCmdFifo->PeakNoLock(pNextCmd) && pNextCmd->oTimeout.IsElapsed())
        {
            /* remove cmd from queue */
            if (m_pVoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pNextCmd))
            {
                MailboxRes(pNextCmd, EC_E_TIMEOUT);
            }
        }
        return;
    }
    /* don't start mailbox transfer if another mailbox transfer is retrying */
    if (  ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)  
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) 
#endif
#ifdef INCLUDE_SOE_SUPPORT
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry)
#endif
#ifdef INCLUDE_AOE_SUPPORT
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bRetry)
#endif
       )
    {
        return;
    }
    /* don't start mailbox transfer if serialization is active and another mailbox transfer is running */
    if (m_bSerializeMbxProtocols && (
        ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)  
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid) 
#endif
#ifdef INCLUDE_SOE_SUPPORT
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_AOE_SUPPORT
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bIsValid)
#endif
       ))
    {
        return;
    }
    if ((m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid == EC_FALSE))
    {
        if (m_pVoEDesc->pPendMailboxCmdFifo->RemoveNoLock((m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd)))
        {
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid       = EC_TRUE;
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.bSendEnabled   = EC_TRUE;
            m_pVoEDesc->CurrPendVoeMbxCmdDesc.dwRetryCounter = m_wRetryAccessCount;
            
            m_dwLastOnTimerCall = 0;        /* force mbx polling */
            /*first check timeout has elapsed */
            if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
            {   /* timeout has elapsed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedVoECmds() m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
                ClearCurrPendVoeMbxCmdDesc();
                return;
            }
        }
    }

    if (m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid == EC_TRUE && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bSendEnabled == EC_TRUE)
    {
        ETHERCAT_MBOX_HEADER    oMbx;
        MBXHDR_CLR(&oMbx);
                
        /* store data also to CurrPendVoeMbxCmdDesc , will be needed later by ProcessVoEResponse() in order to check the response*/
        PEcMailboxCmd pCmd = m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd;
        m_pVoEDesc->CurrPendVoeMbxCmdDesc.bSendEnabled  = EC_FALSE;

        EC_ECMBOXHDR_SET_MBXTYPE(&oMbx, ETHERCAT_MBOX_TYPE_VOE);
        
        if (bResend)
        {
            EC_ECMBOXHDR_SET_LENGTH(&oMbx, (EC_T_WORD)(pCmd->length));

            EcatMbxSendCmdReq(bIsInitCmd, pCmd->pbyData, &oMbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
        else /* not waiting for any command responses and a pending command is in the queue */
        {
            if ((pCmd->cmdId != MAILBOXCMD_ECAT_VOE_READ) &&
                (pCmd->cmdId != MAILBOXCMD_ECAT_VOE_WRITE))
            {
                /* this cmdId is for a master not allowed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedVoECmds() cmdId 0x%X is not allowed for an EtherCAT master.\n",pCmd->cmdId));
                MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_TIMEOUT);
                ClearCurrPendVoeMbxCmdDesc();
                return;
            }

            /* check whether the data fits into the mailbox buffer */
            if ((ETHERCAT_MBOX_HEADER_LEN + pCmd->length) <= m_mbxOLen[m_sMboxFlags.mbxIdx])
            {
                EC_ECMBOXHDR_SET_LENGTH(&oMbx, (EC_T_WORD)(pCmd->length));
            }
            else
            {
                /* the send data are larger then the mailbox size, dont send the mailbox data */
                MailboxRes(m_pVoEDesc->CurrPendVoeMbxCmdDesc.pCurrPendQueuedMbxCmd, EC_E_INVALIDSIZE);
                ClearCurrPendVoeMbxCmdDesc();
                return;
            }
            
            /* common code for all cmdId */
            /* send to slave */
            EcatMbxSendCmdReq(bIsInitCmd, pCmd->pbyData, &oMbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                             ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                             ,EC_NULL
                             ,EC_NULL
#endif
                             );
        }
    }
}
#endif



/********************************************************************************/
/** \brief Sends the queued AoE commands to the slave
*
* \return
*/
#if (defined INCLUDE_AOE_SUPPORT)
EC_T_VOID   CEcMbSlave::SendQueuedAoECmds()
{
EC_T_BOOL bResend    = EC_FALSE;

    /* Mbx SyncManagers are not ready or slave is in INIT or slave is not present */
    if ((3 != (m_wSyncManagerEnabled & 3)) || (DEVICE_STATE_INIT == GetDeviceState()) || !IsPresentOnBus())
    {
        /* clear all pending AoE requests */
        /* clear current pending AoE request */
        /* Stop all currently pending AoE commands */
        if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid )
        {
            MailboxRes(m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd, IsPresentOnBus() ? EC_E_TIMEOUT: EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendAoeMbxCmdDesc();
        }
        /* clear pending FIFO */
        while (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid == EC_FALSE && m_pAoEDesc->pPendMailboxCmdFifo->RemoveNoLock((m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd)))
        {
            m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid   = EC_TRUE;
            
            MailboxRes(m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd, IsPresentOnBus() ? EC_E_TIMEOUT: EC_E_SLAVE_NOT_PRESENT);
            ClearCurrPendAoeMbxCmdDesc();
        }
        
        /* we're done */
        return;
    }
    if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid )
    { /* still waiting for a response to a previous AoE mailbox */
        if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
        {   /* timeout has elapsed */
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedAoECmds() m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
            /* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pAoe->safe */
            StopCurrAoEMbxRequest(EC_E_TIMEOUT);
            return;
        }
        if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx && m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.IsElapsed())
        {   /* timeout has elapsed */
            if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.wRetryCounter > 0 )
            {
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oRetryTimeout.Stop();
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->bRetrySendMbx = EC_FALSE;
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.bSendEnabled   = EC_TRUE;
                m_pAoEDesc->CurrPendAoeMbxCmdDesc.wRetryCounter--; 
                bResend = EC_TRUE;
            }
            else
            {/* timeout has elapsed and retry == 0*/
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedAoECmds() m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                StopCurrAoEMbxRequest(EC_E_TIMEOUT);
                return;
            }
        }
    }
    /* don't start mailbox transfer if master is in transition or InitCmd are running */
    if (!m_pMaster->m_bStateMachIsStable || !SlaveStateMachIsStable())
    {
        PEcMailboxCmd   pNextCmd = EC_NULL;
        
        /* check if timeout for next pending cmd is already elapsed ? */
        if (m_pAoEDesc->pPendMailboxCmdFifo->PeakNoLock(pNextCmd) && pNextCmd->oTimeout.IsElapsed())
        {
            /* remove cmd from queue */
            if (m_pAoEDesc->pPendMailboxCmdFifo->RemoveNoLock(pNextCmd))
            {
                MailboxRes(pNextCmd, EC_E_TIMEOUT);
            }
        }
        return;
    }
    /* don't start mailbox transfer if another mailbox transfer is retrying */
    if (  ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)
#ifdef INCLUDE_SOE_SUPPORT             
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry)
#endif
#ifdef INCLUDE_FOE_SUPPORT             
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry)
#endif
#ifdef INCLUDE_VOE_SUPPORT             
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bVoeWkcRetry)
#endif
       )
    {
        return;
    }
    /* don't start mailbox transfer if serialization is active and another mailbox transfer is running */
    if (m_bSerializeMbxProtocols && (
        ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
#ifdef INCLUDE_SOE_SUPPORT             
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_FOE_SUPPORT             
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_VOE_SUPPORT             
       || ((EC_NULL != m_pVoEDesc) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
#endif
       ))
    {
        return;
    }
    /* get the next AoE request from the AoE mailbox fifo */
    if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid == EC_FALSE)
    {
        if (m_pAoEDesc->pPendMailboxCmdFifo->RemoveNoLock(m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd))
        {
            m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid      = EC_TRUE;
            m_pAoEDesc->CurrPendAoeMbxCmdDesc.bSendEnabled  = EC_TRUE;
            m_pAoEDesc->CurrPendAoeMbxCmdDesc.wRetryCounter = m_wRetryAccessCount;
            m_dwLastOnTimerCall = 0;        /* force mbx polling */           

            /* first check if timeout has elapsed */
            if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed())
            {/* timeout has elapsed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedAoECmds() m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->oTimeout.IsElapsed()\n"));
                StopCurrAoEMbxRequest(EC_E_TIMEOUT);
                return;
            }
        }
    }


    if (m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bSendEnabled)
    {
        ETHERCAT_MBOX_HEADER     mbx;    
        PEC_AOE_HDR     pEcAoeHeader  = &m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeHeader;
        EC_AOE_CMD_HDR* pEcAoeCmdHeader = &m_pAoEDesc->CurrPendAoeMbxCmdDesc.oEcAoeCmdHeader;
        EC_T_BYTE*               pEcAoeCmdData = m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd->pbyData;
        EcMailboxCmd*            pCmd  = m_pAoEDesc->CurrPendAoeMbxCmdDesc.pCurrPendQueuedMbxCmd;

        MBXHDR_CLR(&mbx);

        EC_ECMBOXHDR_SET_MBXTYPE(&mbx, ETHERCAT_MBOX_TYPE_ADS);

        /* fill the soe header with default values, some will be changed later */
        OsMemcpy(pEcAoeHeader->oTargetNetId, pCmd->oTargetNetId, 6);
        pEcAoeHeader->wTargetPort = pCmd->wTargetPort;
        OsMemcpy(pEcAoeHeader->oSenderNetId, &m_pMaster->m_oNetId, 6);
        pEcAoeHeader->wSenderPort = 0xFFFF;
        pEcAoeHeader->wCmdId = pCmd->wAoeCmdId;
        pEcAoeHeader->wStateFlags |= EC_ECAOEHDR_STATEFLAG_REQ;
        /* oAoe.dwDataSize = < depends on the AoE command and is therefore set below  */
        pEcAoeHeader->dwErrorCode = 0;

        /* a new invoke id is used once for each request */
        pEcAoeHeader->dwAoeInvokeId = m_pAoEDesc->dwAoeInvokeId++;

        /* prepare mailbox header */
        m_pAoEDesc->CurrPendAoeMbxCmdDesc.bSendEnabled = EC_FALSE;
        
        if (bResend)
        {
            switch (pCmd->cmdId)
            {
            case MAILBOXCMD_ECAT_AOE_WRITE:
            case MAILBOXCMD_ECAT_AOE_READ:
            case MAILBOXCMD_ECAT_AOE_READWRITE:
                mbx.wLength = (EC_T_WORD)(EC_AOE_HDR_LEN + pEcAoeHeader->dwDataSize);

                /* send to slave */
                EcatMbxSendCmdReq(EC_FALSE, pEcAoeCmdData, &mbx, EC_NULL, EC_NULL 
#ifdef INCLUDE_EOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                 ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                 ,pEcAoeHeader
                                 ,pEcAoeCmdHeader
#endif
                                 );
            break;

            default:
                OsDbgAssert(EC_FALSE);
                break;
            }
        } /* bResend */
        else
        {
            switch (pCmd->cmdId)
            {
            case MAILBOXCMD_ECAT_AOE_READ:
                {
                    /* set the length fields in the mailbox and the AoE header */
                    pEcAoeHeader->dwDataSize = ETHERCAT_AOE_READ_REQ_HEADER_LEN;
                    mbx.wLength = (EC_T_WORD)(EC_AOE_HDR_LEN + pEcAoeHeader->dwDataSize);

                    /* fragmented AoE is not yet supported */
                    if ((ETHERCAT_MBOX_HEADER_LEN + EC_AOE_HDR_LEN + pCmd->length) > m_mbxILen[m_sMboxFlags.mbxIdx]) 
                    {
                        EC_ERRORMSGC(("CEcMbSlave::SendQueuedAoECmds(): Fragmented AoE read not yet supported!\n"));
                        StopCurrAoEMbxRequest(EC_E_NOTSUPPORTED);
                        break;
                    }

                    pEcAoeCmdHeader->dwUsedLength = sizeof(pEcAoeCmdHeader->uCmd.oReadReqHdr);
                    pEcAoeCmdHeader->uCmd.oReadReqHdr.dwIndexGroup  = pCmd->dwIndexGroup;
                    pEcAoeCmdHeader->uCmd.oReadReqHdr.dwIndexOffset = pCmd->dwIndexOffset;
                    pEcAoeCmdHeader->uCmd.oReadReqHdr.dwReadLength  = pCmd->length;
                    
                    /* send to slave */
                    EcatMbxSendCmdReq(EC_FALSE, EC_NULL, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,pEcAoeHeader
                                     ,pEcAoeCmdHeader
#endif
                                     );
                }break;

            case MAILBOXCMD_ECAT_AOE_WRITE:
                {
                    /* set the length fields in the mailbox and the AoE header */
                    pEcAoeHeader->dwDataSize = ETHERCAT_AOE_WRITE_REQ_HEADER_LEN + pCmd->length;
                    mbx.wLength = (EC_T_WORD)(EC_AOE_HDR_LEN + pEcAoeHeader->dwDataSize);

                    /* fragmented AoE is not yet supported */
                    if ((ETHERCAT_MBOX_HEADER_LEN + mbx.wLength) > m_mbxOLen[m_sMboxFlags.mbxIdx]) 
                    {
                        EC_ERRORMSGC(("CEcMbSlave::SendQueuedAoECmds(): Fragmented AoE write not yet supported!\n"));
                        StopCurrAoEMbxRequest(EC_E_NOTSUPPORTED);
                        break;
                    }

                    pEcAoeCmdHeader->dwUsedLength = sizeof(pEcAoeCmdHeader->uCmd.oWriteReqHdr);
                    pEcAoeCmdHeader->uCmd.oWriteReqHdr.dwIndexGroup  = pCmd->dwIndexGroup;
                    pEcAoeCmdHeader->uCmd.oWriteReqHdr.dwIndexOffset = pCmd->dwIndexOffset;
                    pEcAoeCmdHeader->uCmd.oWriteReqHdr.dwWriteLength  = pCmd->length;

                    /* send to slave */
                    EcatMbxSendCmdReq(EC_FALSE, pEcAoeCmdData, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,pEcAoeHeader
                                     ,pEcAoeCmdHeader
#endif
                                     );
                }break;

            case MAILBOXCMD_ECAT_AOE_READWRITE:
                {
                    /* set the length fields in the mailbox and the AoE header */
                    pEcAoeHeader->dwDataSize = ETHERCAT_AOE_READWRITE_REQ_HEADER_LEN + pCmd->length;
                    mbx.wLength    = (EC_T_WORD)(EC_AOE_HDR_LEN + pEcAoeHeader->dwDataSize);

                    /* fragmented AoE is not yet supported */
                    if ((ETHERCAT_MBOX_HEADER_LEN + mbx.wLength) > m_mbxILen[m_sMboxFlags.mbxIdx]) 
                    {
                        EC_ERRORMSGC(("CEcMbSlave::SendQueuedAoECmds(): Fragmented AoE ReadWrite not yet supported!\n"));
                        StopCurrAoEMbxRequest(EC_E_NOTSUPPORTED);
                        break;
                    }

                    pEcAoeCmdHeader->dwUsedLength = sizeof(pEcAoeCmdHeader->uCmd.oReadWriteReqHdr);
                    pEcAoeCmdHeader->uCmd.oReadWriteReqHdr.dwIndexGroup  = pCmd->dwIndexGroup;
                    pEcAoeCmdHeader->uCmd.oReadWriteReqHdr.dwIndexOffset = pCmd->dwIndexOffset;
                    pEcAoeCmdHeader->uCmd.oReadWriteReqHdr.dwReadLength  = pCmd->dwAoeReadLength;
                    pEcAoeCmdHeader->uCmd.oReadWriteReqHdr.dwWriteLength  = pCmd->length;
                    
                    /* send to slave */
                    EcatMbxSendCmdReq(EC_FALSE, pEcAoeCmdData, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,pEcAoeHeader
                                     ,pEcAoeCmdHeader
#endif
                                     );
                }break;

            case MAILBOXCMD_ECAT_AOE_WRITECONTROL:
                {
                    /* set the length fields in the mailbox and the AoE header */
                    pEcAoeHeader->dwDataSize = ETHERCAT_AOE_WRITECONTROL_REQ_HEADER_LEN + pCmd->length;
                    mbx.wLength    = (EC_T_WORD)(EC_AOE_HDR_LEN + pEcAoeHeader->dwDataSize);

                    /* fragmented AoE is not yet supported */
                    if ((ETHERCAT_MBOX_HEADER_LEN + mbx.wLength) > m_mbxOLen[m_sMboxFlags.mbxIdx]) 
                    {
                        EC_ERRORMSGC(("CEcMbSlave::SendQueuedAoECmds(): Fragmented AoE write not yet supported!\n"));
                        StopCurrAoEMbxRequest(EC_E_NOTSUPPORTED);
                        break;
                    }

                    pEcAoeCmdHeader->dwUsedLength = sizeof(pEcAoeCmdHeader->uCmd.oWriteControlReqHdr);
                    pEcAoeCmdHeader->uCmd.oWriteControlReqHdr.wAoEState     = pCmd->wAoEState;
                    pEcAoeCmdHeader->uCmd.oWriteControlReqHdr.wDeviceState  = pCmd->wDeviceState;
                    pEcAoeCmdHeader->uCmd.oWriteControlReqHdr.dwWriteLength = pCmd->length;

                    /* send to slave */
                    EcatMbxSendCmdReq(EC_FALSE, pEcAoeCmdData, &mbx, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                     ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                     ,pEcAoeHeader
                                     ,pEcAoeCmdHeader
#endif
                                     );
                }break;

            default:
                {
                    /* This should never happen */
                    OsDbgAssert(EC_FALSE);
                }
                break;
            }
        } /* !bResend */
    }
}
#endif /* INCLUDE_AOE_SUPPORT */


#if (defined INCLUDE_RAWMBX_SUPPORT)
/********************************************************************************/
/** \brief Sends queued raw mailbox requests to slaves if present.
*   Data for not present slaves is dropped.
* \return
*/
EC_T_VOID CEcMbSlave::SendQueuedRawMbxCmds(EC_T_VOID)
{
EC_T_BOOL bIsInitCmd = EC_FALSE;

    /* Mbx SyncManagers are not ready or slave is in INIT or slave is not present */
    if ((3 != (m_wSyncManagerEnabled & 3)) || (DEVICE_STATE_INIT == GetDeviceState()) || !IsPresentOnBus())
    {
        /* clear current pending raw mailbox request request */
        if (m_pRawMbxDesc->mbxDesc.cur.bIsValid )
        {
            MailboxRes(m_pRawMbxDesc->mbxDesc.cur.pCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE : EC_E_SLAVE_NOT_PRESENT);
            ClearCurRawMbxDesc();
        }
        /* iterate through the pending raw mailbox command queue and clear all mailbox commands */
        while (m_pRawMbxDesc->mbxDesc.cur.bIsValid == EC_FALSE && m_pRawMbxDesc->mbxDesc.pCmdFifo->RemoveNoLock(m_pRawMbxDesc->mbxDesc.cur.pCmd))
        {
            m_pRawMbxDesc->mbxDesc.cur.bIsValid   = EC_TRUE;
            MailboxRes(m_pRawMbxDesc->mbxDesc.cur.pCmd, IsPresentOnBus() ? EC_E_INVALID_SLAVE_STATE : EC_E_SLAVE_NOT_PRESENT);
            ClearCurRawMbxDesc();
        }

        /* we're done */
        return;
    }
    
    if (m_pRawMbxDesc->mbxDesc.cur.bIsValid )
    { /* still waiting for a response to a previous raw mailbox request */
        if (m_pRawMbxDesc->mbxDesc.cur.pCmd->oTimeout.IsElapsed())
        {   /* timeout has elapsed */
            EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedRawMbxCmds() m_pRawMbxDesc->mbxDesc.cur.pCmd->oTimeout.IsElapsed()\n"));
            MailboxRes(m_pRawMbxDesc->mbxDesc.cur.pCmd, EC_E_TIMEOUT);
            ClearCurRawMbxDesc();
            return;
        }
        if (m_pRawMbxDesc->mbxDesc.cur.pCmd->bRetrySendMbx && m_pRawMbxDesc->mbxDesc.cur.pCmd->oRetryTimeout.IsElapsed())
        {   /* timeout has elapsed */
            if (m_pRawMbxDesc->mbxDesc.cur.wRetryCounter > 0 )
            {
                m_pRawMbxDesc->mbxDesc.cur.bSendEnabled = EC_TRUE;
                m_pRawMbxDesc->mbxDesc.cur.pCmd->oRetryTimeout.Stop();
                m_pRawMbxDesc->mbxDesc.cur.pCmd->bRetrySendMbx = EC_FALSE;
                m_pRawMbxDesc->mbxDesc.cur.wRetryCounter--; 
            }
            else
            {/* timeout has elapsed and retry = 0 -> send error response to mailbox and clear m_pCoe->safe */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedRawMbxCmds() m_pRawMbxDesc->mbxDesc.cur.pCmd->oTimeout.IsElapsed()\n"));
                MailboxRes(m_pRawMbxDesc->mbxDesc.cur.pCmd, EC_E_MBX_CMD_WKC_ERROR);
                ClearCurRawMbxDesc();
                return;
            }
        }
    }
    /* don't start mailbox transfer if master is in transition or InitCmd are running */
    if (!m_pMaster->m_bStateMachIsStable || !SlaveStateMachIsStable())
    {
        PEcMailboxCmd   pNextCmd = EC_NULL;
        
        /* check if timeout for next pending cmd is already elapsed ? */
        if (m_pRawMbxDesc->mbxDesc.pCmdFifo->PeakNoLock(pNextCmd) && pNextCmd->oTimeout.IsElapsed())
        {
            /* remove cmd from queue */
            if (m_pRawMbxDesc->mbxDesc.pCmdFifo->RemoveNoLock(pNextCmd))
            {
                MailboxRes(pNextCmd, EC_E_TIMEOUT);
            }
        }
        return;
    }
    /* don't start mailbox transfer if another mailbox transfer is retrying */
    if (  ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bCoeWkcRetry)  
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bFoeWkcRetry) 
#endif
#ifdef INCLUDE_SOE_SUPPORT
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bSoeWkcRetry)
#endif
#ifdef INCLUDE_AOE_SUPPORT
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bAoeWkcRetry)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bRetry)
#endif
       )
    {
        return;
    }
    /* don't start mailbox transfer if serialization is active and another mailbox transfer is running */
    if (m_bSerializeMbxProtocols && (
        ((EC_NULL != m_pCoEDesc) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)  
#ifdef INCLUDE_FOE_SUPPORT
       || ((EC_NULL != m_pFoEDesc) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid) 
#endif
#ifdef INCLUDE_SOE_SUPPORT
       || ((EC_NULL != m_pSoEDesc) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
#endif
#ifdef INCLUDE_AOE_SUPPORT
       || ((EC_NULL != m_pAoEDesc) && m_pAoEDesc->CurrPendAoeMbxCmdDesc.bIsValid)
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
       || ((EC_NULL != m_pRawMbxDesc) && m_pRawMbxDesc->mbxDesc.cur.bIsValid)
#endif
       ))
    {
        return;
    }
    if ((m_pRawMbxDesc->mbxDesc.cur.bIsValid == EC_FALSE))
    {
        if (m_pRawMbxDesc->mbxDesc.pCmdFifo->RemoveNoLock((m_pRawMbxDesc->mbxDesc.cur.pCmd)))
        {
            m_pRawMbxDesc->mbxDesc.cur.bIsValid      = EC_TRUE;
            m_pRawMbxDesc->mbxDesc.cur.bSendEnabled  = EC_TRUE;
            m_pRawMbxDesc->mbxDesc.cur.wRetryCounter = m_wRetryAccessCount;
            
            m_dwLastOnTimerCall = 0;        /* force mbx polling */
            /*first check timeout has elapsed */
            if (m_pRawMbxDesc->mbxDesc.cur.pCmd->oTimeout.IsElapsed())
            {   /* timeout has elapsed */
                EC_ERRORMSGC_L(("CEcMbSlave::SendQueuedRawMbxCmds() m_pRawMbxDesc->mbxDesc.cur.pCmd->oTimeout.IsElapsed()\n"));
                MailboxRes(m_pRawMbxDesc->mbxDesc.cur.pCmd, EC_E_TIMEOUT);
                ClearCurRawMbxDesc();
                return;
            }
        }
    }

    if (m_pRawMbxDesc->mbxDesc.cur.bIsValid == EC_TRUE && m_pRawMbxDesc->mbxDesc.cur.bSendEnabled == EC_TRUE)
    {
        /* store data also to CurrPendRawMbxMbxCmdDesc , will be needed later by ProcessRawMbxResponse() in order to check the response*/
        PEcMailboxCmd pCmd = m_pRawMbxDesc->mbxDesc.cur.pCmd;
        m_pRawMbxDesc->mbxDesc.cur.bSendEnabled = EC_FALSE;

        /* add client ID to raw mailbox mapping table */
        m_pRawMbxDesc->dwClientId = ((EC_T_MBXTFERP*)pCmd->pvContext)->MbxTfer.dwClntId;

        /* set client Id of mailbox transfer object to zero to not notify the application in ProcessRawMbxReturningRequest() */
        ((EC_T_MBXTFERP*)pCmd->pvContext)->MbxTfer.dwClntId = 0;

        /* set address field in mailbox header to raw mailbox slave ID */
        EC_ECMBOXHDR_SET_ADDRESS((ETHERCAT_MBOX_HEADER*)pCmd->pbyData, RAWMBX_FIXEDADDRESS);

        /* send to slave */
        EcatMbxSendCmdReq(bIsInitCmd, pCmd->pbyData + sizeof(ETHERCAT_MBOX_HEADER), (ETHERCAT_MBOX_HEADER*)pCmd->pbyData, EC_NULL, EC_NULL
#ifdef INCLUDE_EOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_FOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                         ,EC_NULL
#endif
#ifdef INCLUDE_AOE_SUPPORT
                         ,EC_NULL
                         ,EC_NULL
#endif
                         );
    }
}

/********************************************************************************/
/** \brief Stop current raw mailbox request
*
* \return
*/
EC_T_VOID CEcMbSlave::StopCurrRawMbxRequest(
    EC_T_BOOL  bIsInitCmd, 
    EC_T_DWORD dwResult,
    EC_T_DWORD dwData,
    EC_T_VOID* pvData
    )
{
    EC_UNREFPARM(bIsInitCmd);

    if (m_pRawMbxDesc == EC_NULL)
        return;

    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED )
    {
        m_mbxLayer.oTimeout.Stop();
        m_mbxLayer.eState = m_mbxLayer.IDLE;
        m_bMbxReadStateIsBusy = EC_FALSE;
    }

    /* inform Mailbox client about error */
    OsDbgAssert(m_pRawMbxDesc->mbxDesc.cur.bIsValid);
    if (m_pRawMbxDesc->mbxDesc.cur.bIsValid )
    {
        m_pRawMbxDesc->mbxDesc.cur.pCmd->oTimeout.Stop();
        MailboxRes(m_pRawMbxDesc->mbxDesc.cur.pCmd, dwResult, dwData, pvData);            
        ClearCurRawMbxDesc();
    }
    return;
}

/********************************************************************************/
/** \brief Process raw mailbox returning request
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessRawMbxReturningRequest(
    EC_T_BOOL             bIsInitCmd,
    EC_T_WORD             wWkc,
    ETYPE_EC_CMD_HEADER*  pEcCmdHeader,
    ETHERCAT_MBOX_HEADER* pMBox
    )
{
    EC_UNREFPARM(bIsInitCmd);
    EC_UNREFPARM(pEcCmdHeader);
    EC_UNREFPARM(pMBox);

    if (EC_NULL == m_pRawMbxDesc)
    {
        return;
    }

    if (1 == wWkc)
    {
        if (m_pRawMbxDesc->mbxDesc.cur.bRetry)
        {
            m_pRawMbxDesc->mbxDesc.cur.bRetry = EC_FALSE;
        }
        if (m_pRawMbxDesc->mbxDesc.cur.bIsValid )
        {
            m_pRawMbxDesc->mbxDesc.cur.wRetryCounter = 0;
        }
        /* trigger mailbox polling -> server may want to respond */
        AddMailboxActionRequestPoll();
    }
    else if (0 == wWkc)
    {
        if (m_pRawMbxDesc->mbxDesc.cur.bIsValid && (m_pRawMbxDesc->mbxDesc.cur.wRetryCounter > 0))
        {
            /* start oRetryTimeout and check this timeout in SendQueuedAoE commands  */ 
            m_pRawMbxDesc->mbxDesc.cur.pCmd->oRetryTimeout.Start(m_wRetryAccessPeriod, m_pMaster->GetMsecCounterPtr());
            m_pRawMbxDesc->mbxDesc.cur.pCmd->bRetrySendMbx = EC_TRUE;

            /* Set retry flag to avoid other mailbox protocols to send data*/
            m_pRawMbxDesc->mbxDesc.cur.bRetry = EC_TRUE;
        }
        else
        {
            /* Reset retry flag */
            m_pRawMbxDesc->mbxDesc.cur.bRetry = EC_FALSE;
            
            /* give up */
            StopCurrRawMbxRequest(EC_FALSE, EC_E_MBX_CMD_WKC_ERROR);
        }
    }
    else
    {
        StopCurrRawMbxRequest(EC_FALSE, EC_E_MBX_CMD_WKC_ERROR);
    }
}

/********************************************************************************/
/** \brief Process raw mailbox response
*
* \return
*/
EC_T_VOID CEcMbSlave::ProcessRawMbxResponse(
    ETHERCAT_MBOX_HEADER* pMBox
    )
{
EC_T_MBXTFERP     oMbxTferPriv;
EC_T_MBXTFERP*    pMbxTferPriv = &oMbxTferPriv;
EC_T_MBXTFER_DESC oMbxTferDesc;

    /* refresh command header */
    EC_ECMBOXHDR_SET_ADDRESS(pMBox, GetFixedAddr());

    /* fill mailbox transfer descriptor */
    oMbxTferDesc.dwMaxDataLen       = EC_ECMBOXHDR_GET_LENGTH(pMBox) + ETHERCAT_MBOX_HEADER_LEN;
    oMbxTferDesc.pbyMbxTferDescData = (EC_T_BYTE*)pMBox;

    /* check if the size of the received raw mailbox match */
    if (oMbxTferDesc.dwMaxDataLen > m_mbxOLen[m_sMboxFlags.mbxIdx])
    {
        EC_ERRORMSGC_L(("Received raw mailbox data greater then the available mailbox size\n"));
        oMbxTferDesc.dwMaxDataLen = (EC_T_WORD)(m_mbxOLen[m_sMboxFlags.mbxIdx] + ETHERCAT_MBOX_HEADER_LEN);
    }
    /* fill mailbox transfer private object */
    pMbxTferPriv->dwSlaveId = GetSlaveId();
    pMbxTferPriv->bAbort    = EC_FALSE;

    /* fill mailbox transfer object */
    OsMemset(pMbxTferPriv, 0, sizeof(EC_T_MBXTFERP));
    pMbxTferPriv->MbxTfer.MbxTferDesc    = oMbxTferDesc;
    pMbxTferPriv->MbxTfer.dwDataLen      = oMbxTferDesc.dwMaxDataLen;
    pMbxTferPriv->MbxTfer.pbyMbxTferData = oMbxTferDesc.pbyMbxTferDescData;
    pMbxTferPriv->MbxTfer.eTferStatus    = eMbxTferStatus_TferDone;
    pMbxTferPriv->MbxTfer.dwErrorCode    = EC_E_NOERROR;

    pMbxTferPriv->MbxTfer.dwClntId       = m_pRawMbxDesc->dwClientId;
    pMbxTferPriv->MbxTfer.dwTferId       = 0;
    pMbxTferPriv->MbxTfer.eMbxTferType   = eMbxTferType_RAWMBX;

    /* fill mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_RAWMBX;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_UPLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = EC_ECMBOXHDR_GET_MBXTYPE(pMBox);
    pMbxTferPriv->oEcMbxCmd.pvContext         = pMbxTferPriv;
    pMbxTferPriv->oEcMbxCmd.dwInvokeId        = pMbxTferPriv->MbxTfer.dwTferId;
    pMbxTferPriv->oEcMbxCmd.length            = pMbxTferPriv->MbxTfer.dwDataLen;
    pMbxTferPriv->oEcMbxCmd.pbyData           = pMbxTferPriv->MbxTfer.pbyMbxTferData;
    pMbxTferPriv->oEcMbxCmd.bInUseByInterface = EC_TRUE;

    /* notify application */
    MailboxRes(&pMbxTferPriv->oEcMbxCmd, EC_E_NOERROR, pMbxTferPriv->oEcMbxCmd.length, pMbxTferPriv->oEcMbxCmd.pbyData);

    StopCurrRawMbxRequest(EC_FALSE, EC_E_NOERROR);

    return;
}
#endif /* INCLUDE_RAWMBX_SUPPORT */

/********************************************************************************/
/** \brief Is called by the master to send a mailbox command to the slave.
*
* \return
*/
EC_T_DWORD CEcMbSlave::SendMailboxCmd(PEcMailboxCmd pCmd)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::SendMailboxCmd()\n"));
    
    if (0 == m_mbxOLen[m_sMboxFlags.mbxIdx])
    {
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }

    /* if slave is in UNKNOWN or INIT, reject all Mbx Commands */
    if ((DEVICE_STATE_UNKNOWN == GetDeviceState())
     || (DEVICE_STATE_INIT    == GetDeviceState()))
    {       
        dwRetVal = EC_E_INVALID_SLAVE_STATE;
        goto Exit;
    }

    if (EC_NULL == m_pBusSlave)
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    if (IsCoECmd(pCmd))
    {
        if (m_pCoEDesc == EC_NULL)
        {
            dwRetVal = EC_E_NO_COE_SUPPORT;
            goto Exit;
        }

        m_dwLastOnTimerCall = 0;        /* force mbx polling */

        if (!m_pCoEDesc->pPendMailboxCmdFifo->Add(pCmd))
        {
            EC_ERRORMSGC(("EC-Master configuration error: cannot add CoE command!\nParameter MAX_QUEUED_COE_CMDS too small!\n"));
            dwRetVal = EC_E_INSERTMAILBOX;
            goto Exit;
        }
    }
#if (defined INCLUDE_SOE_SUPPORT)
    else if (IsSoECmd(pCmd))
    {
        if (m_pSoEDesc == EC_NULL)
        {
            dwRetVal = EC_E_NO_SOE_SUPPORT;
            goto Exit;
        }
		
        m_dwLastOnTimerCall = 0;        /* force mbx polling */
		
        if (!m_pSoEDesc->pPendMailboxCmdFifo->Add(pCmd))
        {
            EC_ERRORMSGC(("EC-Master configuration error: cannot add SoE command!\nParameter MAX_QUEUED_SOE_CMDS too small!\n"));
            dwRetVal = EC_E_INSERTMAILBOX;
            goto Exit;
        }
    }
#endif
#if (defined INCLUDE_FOE_SUPPORT)
    else if (IsFoECmd(pCmd))
    {
        if (m_pFoEDesc == EC_NULL) 
        {
            dwRetVal = EC_E_NO_FOE_SUPPORT;
            goto Exit;
        }
        /* If FoE is only supported in Bootstrap and we are not in Bootstrap */
        if ((DEVICE_STATE_BOOTSTRAP != GetDeviceState()) && !m_sMboxFlags.bFoeSupportNotBS)
        {
            dwRetVal = EC_E_NO_FOE_SUPPORT;
            goto Exit;
        }
        /* If FoE is not supported in Bootstrap */
        if ((DEVICE_STATE_BOOTSTRAP == GetDeviceState()) && !m_sMboxFlags.bBootStrapSupport)
        {
            dwRetVal = EC_E_NO_FOE_SUPPORT_BS;
            goto Exit;
        }

        /* force mbx polling */
        m_dwLastOnTimerCall = 0;
        if (!m_pFoEDesc->pPendMailboxCmdFifo->Add(pCmd))
        {
            EC_ERRORMSGC(("EC-Master configuration error: cannot add FoE command!\nParameter MAX_QUEUED_FOE_CMDS too small!\n"));
            dwRetVal = EC_E_INSERTMAILBOX;
            goto Exit;
        }
    }
#endif /* INCLUDE_FOE_SUPPORT */
#if (defined INCLUDE_VOE_SUPPORT)
    else if (IsVoECmd(pCmd))
    {
        if (m_pVoEDesc == EC_NULL)
        {
            dwRetVal = EC_E_NO_VOE_SUPPORT;
            goto Exit;
        }
		
        m_dwLastOnTimerCall = 0;        /* force mbx polling */
		
        if (!m_pVoEDesc->pPendMailboxCmdFifo->Add(pCmd))
        {
            EC_ERRORMSGC(("EC-Master configuration error: cannot add VoE command!\nParameter MAX_QUEUED_VOE_CMDS too small!\n"));
            dwRetVal = EC_E_INSERTMAILBOX;
            goto Exit;
        }
    }
#endif /* INCLUDE_VOE_SUPPORT */
#if (defined INCLUDE_AOE_SUPPORT)
    else if (IsAoECmd(pCmd))
    {
        if (m_pAoEDesc == EC_NULL)
        {
            dwRetVal = EC_E_NO_AOE_SUPPORT;
            goto Exit;
        }
		
        m_dwLastOnTimerCall = 0;        /* force mbx polling */
		
        if (!m_pAoEDesc->pPendMailboxCmdFifo->Add(pCmd))
        {
            EC_ERRORMSGC(("EC-Master configuration error: cannot add AoE command!\nParameter MAX_QUEUED_AOE_CMDS too small!\n"));
            dwRetVal = EC_E_INSERTMAILBOX;
            goto Exit;
        }
    }
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    else if (IsRawMbxCmd(pCmd))
    {
        if (m_pRawMbxDesc == EC_NULL)
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_NOTSUPPORTED;
            goto Exit;
        }
		
        m_dwLastOnTimerCall = 0;        /* force mbx polling */

        if (!m_pRawMbxDesc->mbxDesc.pCmdFifo->Add(pCmd))
        {
            EC_ERRORMSGC(("EC-Master configuration error: cannot add AoE command!\nParameter MAX_QUEUED_AOE_CMDS too small!\n"));
            dwRetVal = EC_E_INSERTMAILBOX;
            goto Exit;
        }
    }
#endif
    else
    {
        EC_ERRORMSG(("CEcMbSlave::SendMailboxCmd(): unknown mailbox command\n"));
        dwRetVal = EC_E_UNKNOWN_MBX_PROTOCOL;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#ifdef INCLUDE_FOE_SUPPORT
EC_T_DWORD CEcMbSlave::FoeSegmentedUploadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szDstFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, 
                                                 EC_T_DWORD dwPassword)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (dwFileNameLen > MAX_FILE_NAME_SIZE)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_FOE_FOPENREAD;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_FILEACCESS;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_FOE_OPCODE_RRQ;
    pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength  = dwFileNameLen;
    OsMemset(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, 0, sizeof(EC_T_CHAR) * MAX_FILE_NAME_SIZE);
    if (EC_NULL != szDstFileName)
    {
        OsMemcpy(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, szDstFileName, pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength);
    }
    pMbxTferPriv->oEcMbxCmd.FoE.dwPassword = dwPassword;

    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_FOE_SEG_UPLOAD;
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 0;
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwRequestedBytes = dwFileSize;
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwFileSize = dwFileSize;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_VOID CEcMbSlave::FoeSegmentedUploadSetSegmentDone(EC_T_MBXTFERP* pMbxTferPriv)
{
    pMbxTferPriv->oEcMbxCmd.wMbxSubType = ECAT_FOE_OPCODE_DATA;
    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend = EC_FALSE;

    if (m_pFoEDesc->CurrPendFoeMbxCmdDesc.bLastRead)
    {
        SendFoEAckToSlave(EC_FALSE, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber);
    }
}

EC_T_VOID CEcMbSlave::FoeSegmentedUploadContinue(EC_T_MBXTFERP* pMbxTferPriv)
{
    pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_Pend;
    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled = EC_TRUE;

    SendFoEAckToSlave(EC_FALSE, m_pFoEDesc->CurrPendFoeMbxCmdDesc.dwLastProcessedPacketNumber);
}

EC_T_DWORD CEcMbSlave::FoeSegmentedDownloadStart(EC_T_MBXTFERP* pMbxTferPriv, EC_T_CHAR* szDstFileName, EC_T_DWORD dwFileNameLen, EC_T_DWORD dwFileSize, 
                                                 EC_T_DWORD dwPassword)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (dwFileNameLen > MAX_FILE_NAME_SIZE)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* prepare the mailbox command descriptor */
    OsMemset(&pMbxTferPriv->oEcMbxCmd, 0, sizeof(EcMailboxCmd));
    pMbxTferPriv->oEcMbxCmd.cmdId             = MAILBOXCMD_ECAT_FOE_FOPENWRITE;
    pMbxTferPriv->oEcMbxCmd.wMbxCmdType       = EC_MAILBOX_CMD_DOWNLOAD;
    pMbxTferPriv->oEcMbxCmd.wMbxType          = ETHERCAT_MBOX_TYPE_FILEACCESS;
    pMbxTferPriv->oEcMbxCmd.wMbxSubType       = ECAT_FOE_OPCODE_WRQ;
    pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength  = dwFileNameLen;
    OsMemset(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, 0, sizeof(EC_T_CHAR) * MAX_FILE_NAME_SIZE);
    if (EC_NULL != szDstFileName)
    {
        OsMemcpy(pMbxTferPriv->oEcMbxCmd.FoE.achFileName, szDstFileName, pMbxTferPriv->oEcMbxCmd.FoE.dwFileNameLength);
    }
    pMbxTferPriv->oEcMbxCmd.FoE.dwPassword = dwPassword;

    pMbxTferPriv->MbxTfer.eMbxTferType = eMbxTferType_FOE_SEG_DOWNLOAD;
    pMbxTferPriv->MbxTfer.MbxData.FoE.dwTransferredBytes = 0;

    pMbxTferPriv->MbxTfer.MbxData.FoE.dwRequestedBytes = GetFoEDataSize(VG_OUT);
    if (EC_MIN(pMbxTferPriv->MbxTfer.MbxData.FoE.dwRequestedBytes, dwFileSize) > pMbxTferPriv->MbxTfer.MbxTferDesc.dwMaxDataLen)
    {
        EC_ERRORMSGC(("FoeSegmentedDownloadStart: MbxTfer buffer too small!\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    pMbxTferPriv->MbxTfer.MbxData.FoE.dwFileSize = dwFileSize;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

EC_T_VOID CEcMbSlave::FoeSegmentedDownloadSetSegmentDone(EC_T_MBXTFERP* pMbxTferPriv)
{
    pMbxTferPriv->oEcMbxCmd.wMbxSubType = ECAT_FOE_OPCODE_DATA;
    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bDataPend = EC_FALSE;

    pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_TferWaitingForContinue;
    m_pMaster->NotifyMbxRcv(pMbxTferPriv);
}

EC_T_VOID CEcMbSlave::FoeSegmentedDownloadContinue(EC_T_MBXTFERP* pMbxTferPriv)
{
    pMbxTferPriv->MbxTfer.eTferStatus = eMbxTferStatus_Pend;
    m_pFoEDesc->CurrPendFoeMbxCmdDesc.bSendEnabled = EC_TRUE;
}
#endif /* INCLUDE_FOE_SUPPORT */

/********************************************************************************/
/** \brief Send mailbox command to slave.
*
* \return
*/
EC_T_DWORD  CEcMbSlave::EcatMbxSendCmdReq
(EC_T_BOOL bIsInitCmd
,EC_T_VOID* pData
,PETHERCAT_MBOX_HEADER pMbox
,PETHERCAT_CANOPEN_HEADER  pCANopen
,PEC_SDO_HDR      pSDO
#ifdef INCLUDE_EOE_SUPPORT
,PETHERCAT_ETHERNET_HEADER pEthernet
#endif
#ifdef INCLUDE_FOE_SUPPORT
,PEC_FOE_HDR      pFoE
#endif
#ifdef INCLUDE_SOE_SUPPORT
,PEC_SOE_HDR      pSoE
#endif
#ifdef INCLUDE_AOE_SUPPORT
,PEC_AOE_HDR      pAoE
,PEC_AOE_CMD_HDR  pAoeCmd
#endif
)
{
EC_T_MBXINVOKEID EMbxInvokeId;
EC_T_DWORD  dwRetVal;
EC_T_DWORD  dwRes;
EC_T_WORD   wLen;
EC_T_BYTE   byMbxCnt = 0;
EC_T_BOOL   bSplitDatagram = EC_FALSE;
EC_T_BOOL   bPendingFrameLocked = EC_FALSE;
EC_T_WORD   wNumOfFillBytes = 2;

    if (bIsInitCmd)
    {
        EMbxInvokeId = eMbxInvokeId_INITCMD_SEND_TO_SLAVE;
    }
    else
    {
        EMbxInvokeId = eMbxInvokeId_SEND_TO_SLAVE;
    }

    if (!IsPresentOnBus())
    {
        /* StopInitCmds(ECAT_INITCMD_FAILURE, wCurrTransition); */
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }

    wLen = (EC_T_WORD)(ETHERCAT_MBOX_HEADER_LEN + EC_ECMBOXHDR_GET_LENGTH(pMbox));

    /* split into two datagrams */
    if (m_mbxOLen[m_sMboxFlags.mbxIdx] > wLen + 14 + 2)
    {
        bSplitDatagram = EC_TRUE;

        m_pMaster->EcatLockCurrPendingSlaveFrame();
        bPendingFrameLocked = EC_TRUE;

        /* prevent that the two datagrams are in different frames */
        dwRes = m_pMaster->PrepareFreeFrameSpace((EC_T_WORD)(wLen + ETYPE_EC_OVERHEAD + wNumOfFillBytes));
        if (EC_E_NOERROR != dwRes)
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    else
    {
        wLen = GetMBoxOutCmdLength(wLen);
    }
    byMbxCnt = IncCntMbxSend();
    EC_ECMBOXHDR_SET_COUNTER(pMbox, byMbxCnt);
    m_bMbxCmdPending = EC_TRUE;

    dwRes = m_pMaster->QueueEcatCmdReq( EC_FALSE, this
                                       ,EMbxInvokeId
                                       ,EC_CMD_TYPE_FPWR
                                       ,m_wFixedAddr
                                       ,m_mbxOStart[m_sMboxFlags.mbxIdx]
                                       ,wLen
                                       ,pData
                                       ,0
                                       ,pMbox
#ifdef INCLUDE_EOE_SUPPORT
                                       ,pEthernet
#else
                                       ,EC_NULL
#endif
                                       ,pCANopen
                                       ,pSDO
#ifdef INCLUDE_FOE_SUPPORT
                                       ,pFoE
#else
                                       ,EC_NULL
#endif
#ifdef INCLUDE_SOE_SUPPORT
                                       ,pSoE
#endif
#ifdef INCLUDE_AOE_SUPPORT
                                       ,pAoE
                                       ,pAoeCmd
#endif
										);
    if (EC_E_NOERROR != dwRes)
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = dwRes;
        goto Exit;
    }
    /* write end of mailbox */
    if (bSplitDatagram)
    {
        dwRes = m_pMaster->QueueEcatCmdReq(
            EC_FALSE, this,
            eMbxInvokeId_SEND_FILL_TO_SLAVE,
            EC_CMD_TYPE_FPWR,
            m_wFixedAddr,
            (EC_T_WORD)(m_mbxOStart[m_sMboxFlags.mbxIdx] + m_mbxOLen[m_sMboxFlags.mbxIdx] - wNumOfFillBytes), 
            wNumOfFillBytes, EC_NULL, 0, EC_NULL, EC_NULL, EC_NULL, EC_NULL, EC_NULL
#ifdef INCLUDE_SOE_SUPPORT
            ,EC_NULL
#endif
            );
        if (EC_E_NOERROR != dwRes)
        {
            OsDbgAssert(EC_FALSE);
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    dwRetVal = EC_E_NOERROR;
Exit:
    if (bPendingFrameLocked)
    {
       m_pMaster->EcatUnlockCurrPendingSlaveFrame();
    }
    return dwRetVal;
}

/********************************************************************************/
/** \brief Sends an EtherCAT command request to read out the mailbox of the slave.
*
* \return N/A.
*/
EC_T_VOID   CEcMbSlave::MBoxReadFromSlave(EC_T_VOID)
{
    EC_T_DWORD          dwRes           = EC_E_NOERROR;
    EC_T_MBXINVOKEID    EMbxInvokeId    = eMbxInvokeId_UNKNOWN;
    EC_T_WORD           wMbxILen        = 0;       
    EC_T_BOOL           bIsInitCmd      = EC_FALSE;
    EC_T_BOOL           bPhyscalMbxPollingEnabled = EC_FALSE;
    EC_T_DWORD          dwMailboxAction = 0;
    EC_T_BOOL           bMbxReadStateIsBusy = EC_FALSE;

    /* if a mailbox command is queued but not yet send and returned, don't poll the slave  */
    if (m_bMbxCmdPending)
    {
         goto Exit;
    }
    /* if last mailbox read did not return or repeating mailbox communication failed */
    if (m_bMbxReadStateIsBusy)
    { 
         goto Exit;
    }
    /* if repeating mailbox communication is running */
    if (m_mbxLayer.eState == m_mbxLayer.TOGGLED)
    {
        goto Exit;
    }
    /* if send is disabeld */
    if (!m_poEcDevice->IsSendEnabled())
    {
        goto Exit;
    }
    /* Slave is not present */
    if (!IsPresentOnBus())
    {
        goto Exit;
    }
    /* Master is in (or commanded to) INIT or slave is in (or commanded to) INIT or Mailbox Syncmanagers are not ready (MbxInitCmds can be configured in IP transition) */
    if (((DEVICE_STATE_INIT == m_pMaster->GetMasterReqDeviceState()) || (DEVICE_STATE_INIT == GetDeviceState()) || (DEVICE_STATE_INIT == GetSmReqDeviceState()) || (3 != (m_wSyncManagerEnabled & 3)))
        && (DEVICE_STATE_BOOTSTRAP != GetDeviceState()))
    {
        goto Exit;
    }
    /* enable physical mailbox polling.
       physical mailbox polling must be active for init commands which will be sent in the IP (init -> pre-op) transition */
    bPhyscalMbxPollingEnabled = (m_sMboxFlags.bCyclicPhysicalMBoxPolling || m_wActTransition == ECAT_INITCMD_I_P);

    /* decide about mailbox action */
    dwMailboxAction |= (((m_wMailboxActionRequest&iecs_mbx_poll) && !(m_wMailboxActionState&iecs_mbx_poll))?iecs_mbx_poll:0);
    dwMailboxAction |= (((m_wMailboxActionRequest&iecs_mbx_read) && !(m_wMailboxActionState&iecs_mbx_read))?iecs_mbx_read:0);
    if ((iecs_mbx_idle == dwMailboxAction) 
     && (  (bPhyscalMbxPollingEnabled && (m_dwCurMbxPollingAge >= m_wCyclePhysicalMBoxPollingInterval)) 
#if (defined INCLUDE_FOE_SUPPORT)
         || (m_sMboxFlags.bCyclicPhysicalMBoxBootPolling && (m_dwCurMbxPollingAge >= m_wCyclePhysicalMBoxBootPollingInterval)
             && (DEVICE_STATE_BOOTSTRAP == GetDeviceState()))
#endif
        ) 
      )
    {
        /* dwMailboxAction |= Cyclic Timeouts */
        dwMailboxAction |= ((CycleMBoxPolling())?iecs_mbx_poll:iecs_mbx_idle);
        m_dwCurMbxPollingAge = 0;
    }

    if (iecs_mbx_idle == dwMailboxAction )
    {
        goto Exit;
    }
    /* prepare mailbox cmd */
    bIsInitCmd = (INITCMD_INACTIVE!=m_wMbxInitCmdsCurrent);
    if (0 != (iecs_mbx_read & dwMailboxAction))
    {
        bMbxReadStateIsBusy = EC_TRUE;
        EMbxInvokeId = bIsInitCmd ? eMbxInvokeId_INITCMD_RECV_FROM_SLAVE : eMbxInvokeId_RECV_FROM_SLAVE;
        wMbxILen = m_mbxILen[m_sMboxFlags.mbxIdx];
    }
    else if (0 != (dwMailboxAction & iecs_mbx_poll))
    {
        EMbxInvokeId = bIsInitCmd ? eMbxInvokeId_INITCMD_POLL_RECV_FROM_SLAVE : eMbxInvokeId_POLL_RECV_FROM_SLAVE; 
        wMbxILen = 2;       /* just read the first two bytes to figure out if mailbox is filled */
    }
    else
    {
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }
    /* queue the mailbox cmd */
    dwRes = m_pMaster->QueueRegisterCmdReq(this, EMbxInvokeId, EC_CMD_TYPE_FPRD, m_wFixedAddr, 
        m_mbxIStart[m_sMboxFlags.mbxIdx], wMbxILen, EC_NULL);
    if (EC_E_NOERROR != dwRes)
    {
        goto Exit;
    }
    /* refresh flags */
    m_bMbxReadStateIsBusy    = bMbxReadStateIsBusy;
    m_wMailboxActionState   |= dwMailboxAction;
    m_wMailboxActionRequest &= ~dwMailboxAction;
    
Exit:
    return;
}


/********************************************************************************/
/** \brief Determine if mailbox shall be polled
*
* \return EC_TRUE if mailbox shall be polled.
*/
EC_T_BOOL   CEcMbSlave::CycleMBoxPolling()
{
    EC_T_BOOL bPoll;

    /*bPoll is true if:
    *   1. m_sMboxFlags.cyclicPhysicalMBoxPolling is set to true
    *   or if state polling  is used and
    *   2. the master is in init or in a transition IP(Init to Pre-Operational), SI or OI. 
    *       This is necessary because the cyclic commands are only sent from the state Pre-Operational on
    *       and mailbox commands might be sent for instance during the transition from init to Pre-Operational.         
    *   or
    *   4. The slave is not in a stable state, and CoE Initcmds must be sent to reach the new state
    */

    if (DEVICE_STATE_BOOTSTRAP == GetDeviceState())
    {
        bPoll = m_sMboxFlags.bCyclicPhysicalMBoxBootPolling;
    }
    else
    {
        bPoll = m_sMboxFlags.bCyclicPhysicalMBoxPolling;
    }

    if (!bPoll)
    {
        EC_T_WORD wMasterState = m_pMaster->GetMasterLowestDeviceState();
        bPoll = wMasterState == DEVICE_STATE_UNKNOWN || wMasterState == DEVICE_STATE_INIT;
    }

    if (!bPoll)
    {
        bPoll = !SlaveStateMachIsStable() && (m_wMbxInitCmdsCurrent != INITCMD_INACTIVE);
    }

    /* don't poll if the current state of the slave device is in init and no mailbox init commands should be sent, */
    bPoll &= (DEVICE_STATE_INIT != GetDeviceState() || m_wActTransition == ECAT_INITCMD_I_P);

    EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::CycleMBoxPolling() bPoll = %s\n", bPoll?"TRUE":"FALSE"));

    return bPoll;
}


/********************************************************************************/
/** \brief This method is called cyclically by CEcMaster::OnTimer.
*
* \return
*/
EC_T_VOID   CEcMbSlave::OnTimer(   
    EC_T_DWORD  dwCurMsecCounter    /**< [in] Current absolute Msec counter */
                               )
{           
    EC_T_NOTIFICATION_INTDESC*  pNotification       = EC_NULL;

    /* evaluate time since last call */
    EC_T_DWORD                  dwCallDelta         = 0;

    dwCallDelta         = dwCurMsecCounter - m_dwLastOnTimerCall;

    m_dwLastOnTimerCall = dwCurMsecCounter;

	CEcSlave::OnTimer(dwCurMsecCounter);

    /* now age poll request mark by delta */
    m_dwCurMbxPollingAge += dwCallDelta;

    /* EC_TRACEMSG(EC_TRACE_CORE, ("CEcMbSlave::OnTimer() %d\n", dwTickNow)); */    
    MBoxReadFromSlave();
    
    if (AreMbxInitCmdsPending())
    {
        if (m_oInitCmdsTimeout.IsElapsed())
        {
            pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT;
                GetSlaveProp(&(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.SlaveProp));
                SAFE_STRCPY( pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.achStateChangeName, 
                           GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(GetMbxInitCmdsCurrent())&0x7FF)), 
                           MAX_SHORT_STRLEN );
                pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.EErrorType = eInitCmdErr_NO_RESPONSE;
                SAFE_STRCPY(pNotification->uNotification.ErrorNotification.desc.InitCmdErrDesc.szComment, EcMailboxCmdDescComment(GetMbxInitCmdsCurrent()), EC_MIN(EC_ECMBXCMDDESC_GET_CMTLEN(GetMbxInitCmdsCurrent()), MAX_STD_STRLEN-1));
                m_pMaster->NotifyAndFree(pNotification->uNotification.ErrorNotification.dwNotifyErrorCode, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_INITCMD_RESPONSE_ERROR);
            }

            EC_ERRORMSGC_L( ("CEcMbSlave::OnTimer() '%s' (%d): Timeout: '%s'. Mbx InitCmd [Num %d of %d]: %s: %s - %s: id=0x%x, subid=%d\n", 
                         m_pEniSlave->szName, m_wFixedAddr, EcMailboxCmdDescComment(GetMbxInitCmdsCurrent()),
                         m_wMbxInitCmdsCurrent, m_wMbxInitCmdsCount,
                         GetStateChangeNameShort((EC_T_WORD)(EC_ECMBXCMDDESC_GET_TRANSITION(GetMbxInitCmdsCurrent())&0x7FF)), 
                         StrMbxTypeText(EC_ECMBXCMDDESC_GET_PROTOCOL(GetMbxInitCmdsCurrent())),
                         CoESdoCcsText(GetMbxInitCmdsCurrent()->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Ccs), 
                         (EC_T_INT)EC_ECSDOHDR_GET_INDEX(&(GetMbxInitCmdsCurrent()->uMbxHdr.coe.EcSdoHeader)), GetMbxInitCmdsCurrent()->uMbxHdr.coe.EcSdoHeader.SubIndex)
                       );

            /* don't continue sending init commands */
            StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
        }
    }

    if (m_pCoEDesc && !m_bMbxCmdPending)
    {
        SendQueuedCoECmds();
    }
#if (defined INCLUDE_FOE_SUPPORT)
    if (m_pFoEDesc && !m_bMbxCmdPending)
    {
        SendQueuedFoECmds();
    }
#endif
    
#if (defined INCLUDE_SOE_SUPPORT)
    if (m_pSoEDesc && !m_bMbxCmdPending)
    {
        SendQueuedSoECmds();
    }
#endif

#if (defined INCLUDE_EOE_SUPPORT)
    if (m_pEoEDesc && !m_bMbxCmdPending)
    {
        SendQueuedEoECmds();
    }
#endif

#if (defined INCLUDE_VOE_SUPPORT)
    if (m_pVoEDesc && !m_bMbxCmdPending)
    {
        SendQueuedVoECmds();
    }
#endif
#if (defined INCLUDE_AOE_SUPPORT)
    if (m_pAoEDesc && !m_bMbxCmdPending)
    {
        SendQueuedAoECmds();
    }
#endif
#if (defined INCLUDE_RAWMBX_SUPPORT)
    if (m_pRawMbxDesc && !m_bMbxCmdPending)
    {
        SendQueuedRawMbxCmds();
    }
#endif
}

/***************************************************************************************************/
/**
\brief  Get Internally stored Slave Information.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMbSlave::GetMbSlaveInfo(
                                  EC_T_eINFOENTRY   eEntry,     /**< [in]   read entry index */
                                  EC_T_BYTE*        pbyData,    /**< [out]  value read */
                                  EC_T_DWORD*       pdwLen      /**< [out]  size of read value */
                                 )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL == pbyData )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    switch( eEntry )
    {
    case eie_mbx_outsize:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }

            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, (EC_T_DWORD)m_mbxOLen[0]);
        } break;
    case eie_mbx_insize:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, (EC_T_DWORD)m_mbxILen[0]);
        } break;
    case eie_mbx_outsize2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, (EC_T_DWORD)m_mbxOLen[1]);
        } break;
    case eie_mbx_insize2:
        {
            if (sizeof(EC_T_DWORD) > *pdwLen )
            {
                dwRetVal = EC_E_INVALIDSIZE;
                goto Exit;
            }
            
            *pdwLen = sizeof(EC_T_DWORD);
            EC_SETDWORD(pbyData, (EC_T_DWORD)m_mbxILen[1]);
        } break;

    case eie_unknown:
    default:
        {
            dwRetVal = EC_E_INVALIDPARM;
            goto Exit;
        };
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get All Internally stored Mailbox Slave Information.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcMbSlave::GetMbSlaveInfoAll(
                                EC_T_GET_SLAVE_INFO* pSlaveInfo     /**< [out]  slave information */
                                 )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    
    /* mbx_outsize */
    pSlaveInfo->dwMbxOutSize = m_mbxOLen[0];
    /* mbx_insize */
    pSlaveInfo->dwMbxInSize = m_mbxILen[0];
    /* mbx_outsize2 */
    pSlaveInfo->dwMbxOutSize2 = m_mbxOLen[1];
    /* mbx_insize2 */
    pSlaveInfo->dwMbxInSize2 = m_mbxILen[1];

    pSlaveInfo->wSupportedMbxProtocols = m_wSupportedMbxProtocols;

    dwRetVal = EC_E_NOERROR;
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get All Internally stored Slave Information.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSlave::GetSlaveInfoAll(
    EC_T_GET_SLAVE_INFO*    pSlaveInfo  /**< [out]  slave information */
                                 )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    
    
    pSlaveInfo->dwSlaveId = m_dwSlaveId;

    /* pdoffs_in */
    /* XML Value */
    pSlaveInfo->dwPdOffsIn = m_pEniSlave->adwPDInOffset[0];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsIn)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsIn = (EC_T_DWORD)((pSlaveInfo->dwPdOffsIn + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
        }
    }
    
    /* pdsize_in */
    pSlaveInfo->dwPdSizeIn = m_pEniSlave->adwPDInSize[0];
    
    /* pdoffs_out */
    /* XML Value */
    pSlaveInfo->dwPdOffsOut = m_pEniSlave->adwPDOutOffset[0];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsOut)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsOut = (EC_T_DWORD)((pSlaveInfo->dwPdOffsOut + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
        }
    }
    
    /* pdsize_out */
    pSlaveInfo->dwPdSizeOut = m_pEniSlave->adwPDOutSize[0];
    
    /* pdoffs_in2 */
    /* XML Value */
    pSlaveInfo->dwPdOffsIn2 = m_pEniSlave->adwPDInOffset[1];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsIn2)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsIn2 = (EC_T_DWORD)((pSlaveInfo->dwPdOffsIn2 + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
        }
    }

    /* pdsize_in */
    pSlaveInfo->dwPdSizeIn2 = m_pEniSlave->adwPDInSize[1];
    
    /* pdoffs_out2 */
    /* XML Value */
    pSlaveInfo->dwPdOffsOut2 = m_pEniSlave->adwPDOutOffset[1];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsOut2)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsOut2 = (EC_T_DWORD)((pSlaveInfo->dwPdOffsOut2 + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
        }
    }
    
    /* pdsize_out2 */
    pSlaveInfo->dwPdSizeOut2 = m_pEniSlave->adwPDOutSize[1];
    
    /* pdoffs_in3 */
    /* XML Value */
    pSlaveInfo->dwPdOffsIn3 = m_pEniSlave->adwPDInOffset[2];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsIn3)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsIn3 = (EC_T_DWORD)((pSlaveInfo->dwPdOffsIn3 + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
        }
    }
    
    /* pdsize_in3 */
    pSlaveInfo->dwPdSizeIn3 = m_pEniSlave->adwPDInSize[2];
    
    /* pdoffs_out3 */
    /* XML Value */
    pSlaveInfo->dwPdOffsOut3 = m_pEniSlave->adwPDOutOffset[2];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsOut3)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsOut3 = (EC_T_DWORD)((pSlaveInfo->dwPdOffsOut3 + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
        }
    }

    /* pdsize_out3 */
    pSlaveInfo->dwPdSizeOut3 = m_pEniSlave->adwPDOutSize[2];

    /* pdoffs_in4 */
    /* XML Value */
    pSlaveInfo->dwPdOffsIn4 = m_pEniSlave->adwPDInOffset[3];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsIn4)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsIn4 = (EC_T_DWORD)((pSlaveInfo->dwPdOffsIn4 + ((m_pMaster->m_dwMinCycInOffset) * 8)) - (m_pMaster->m_dwMinSend));
        }
    }

    /* pdsize_in4 */
    pSlaveInfo->dwPdSizeIn4 = m_pEniSlave->adwPDInSize[3];
    
    /* pdoffs_out4 */
    /* XML Value */
    pSlaveInfo->dwPdOffsOut4 = m_pEniSlave->adwPDOutOffset[3];
    
    if (m_pMaster->m_bPdOffsetCompMode)
    {
        if (((EC_T_DWORD)-1) != pSlaveInfo->dwPdOffsOut4)
        {
            /* now norm value on behalf of configurator specialties */
            pSlaveInfo->dwPdOffsOut4 = (EC_T_DWORD)((pSlaveInfo->dwPdOffsOut4 + ((m_pMaster->m_dwMinCycOutOffset) * 8)) - (m_pMaster->m_dwMinRecv));
        }
    }

    /* pdsize_out4 */
    pSlaveInfo->dwPdSizeOut4 = m_pEniSlave->adwPDOutSize[3];
    
    /* cfgphy_address */
    pSlaveInfo->wCfgPhyAddress  = m_pEniSlave->wPhysAddr;

    /* device_name */
    OsMemcpy(pSlaveInfo->abyDeviceName, m_pEniSlave->szName, ECAT_DEVICE_NAMESIZE);

    /* ismailbox_slave */
    pSlaveInfo->bIsMailboxSlave = HasMailBox();
    
    /* hot connect infos */
    pSlaveInfo->bIsOptional     = m_bIsOptional;

    /* hot connect infos */
    pSlaveInfo->bIsPresent      = IsPresentOnBus();

    /* AL Status Infos from last Update Slave State */
    pSlaveInfo->wAlStatusValue  = GetAlStatus();
    pSlaveInfo->wAlStatusCode   = GetAlStatusCode();

    /* if no valid vendor id (BusSlave doesn't exists) use values from ConfigSlave */
    if (pSlaveInfo->dwVendorId == 0)
    {
        pSlaveInfo->dwVendorId       = m_pEniSlave->dwVendorId;
        pSlaveInfo->dwProductCode    = m_pEniSlave->dwProductCode;
        pSlaveInfo->dwRevisionNumber = m_pEniSlave->dwRevisionNumber;
        pSlaveInfo->dwSerialNumber   = m_pEniSlave->dwSerialNumber;
    }

#if (defined INCLUDE_VARREAD)
    /* Number of input and output process variables */
    pSlaveInfo->wNumProcessVarsInp  = m_pEniSlave->wNumProcessVarsInp;
    pSlaveInfo->wNumProcessVarsOutp = m_pEniSlave->wNumProcessVarsOutp;
#endif
    
    if (!HasMailBox())
    {
        pSlaveInfo->dwMbxInSize     = 0;
        pSlaveInfo->dwMbxOutSize    = 0;
        pSlaveInfo->dwMbxInSize2    = 0;
        pSlaveInfo->dwMbxOutSize2   = 0;
    }
    else
    {   

        CEcMbSlave* pMbxSlave = (CEcMbSlave*)this;
        dwRetVal = pMbxSlave->GetMbSlaveInfoAll(pSlaveInfo);
    }

    dwRetVal = EC_E_NOERROR;
    
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get All Internally stored Mailbox Slave Information.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CEcMbSlave::GetCfgSlaveInfo(
    EC_T_CFG_SLAVE_INFO*    pSlaveInfo  /**< [out]  slave information */
    )
{
    pSlaveInfo->dwMbxOutSize            = m_mbxOLen[0];
    pSlaveInfo->dwMbxInSize             = m_mbxILen[0];
    pSlaveInfo->dwMbxOutSize2           = m_mbxOLen[1];
    pSlaveInfo->dwMbxInSize2            = m_mbxILen[1];
    pSlaveInfo->dwMbxSupportedProtocols = (EC_T_DWORD)m_wSupportedMbxProtocols;
    return;
}

/***************************************************************************************************/
/**
\brief  Get config slave information

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CEcSlave::GetCfgSlaveInfo(
    EC_T_CFG_SLAVE_INFO*    pSlaveInfo  /**< [out]  slave information */
    )
{
EC_T_HOTCONNECT_GROUP* pHCGroup = m_pMaster->m_oHotConnect.GetGroup(m_dwHCGroupId);

    OsMemset(pSlaveInfo, 0, sizeof(EC_T_CFG_SLAVE_INFO));

    pSlaveInfo->dwSlaveId           = m_dwSlaveId;
    OsMemcpy(pSlaveInfo->abyDeviceName, m_pEniSlave->szName, ECAT_DEVICE_NAMESIZE);
    pSlaveInfo->dwHCGroupIdx        = m_dwHCGroupId;
    pSlaveInfo->bIsPresent          = IsPresentOnBus();
    if (EC_NULL != pHCGroup)
    {
        pSlaveInfo->bIsHCGroupPresent = pHCGroup->bGroupPresent;
    }
    pSlaveInfo->dwVendorId          = m_pEniSlave->dwVendorId;
    pSlaveInfo->dwProductCode       = m_pEniSlave->dwProductCode;
    pSlaveInfo->dwRevisionNumber    = m_pEniSlave->dwRevisionNumber;
    pSlaveInfo->dwSerialNumber      = m_pEniSlave->dwSerialNumber;

    pSlaveInfo->wStationAddress     = m_pEniSlave->wPhysAddr;
    pSlaveInfo->wAutoIncAddress     = m_pEniSlave->wAutoIncAddr;

    pSlaveInfo->dwPdOffsIn          = m_pEniSlave->adwPDInOffset[0];
    pSlaveInfo->dwPdSizeIn          = m_pEniSlave->adwPDInSize[0];
    pSlaveInfo->dwPdOffsOut         = m_pEniSlave->adwPDOutOffset[0];
    pSlaveInfo->dwPdSizeOut         = m_pEniSlave->adwPDOutSize[0];
    pSlaveInfo->dwPdOffsIn2         = m_pEniSlave->adwPDInOffset[1];
    pSlaveInfo->dwPdSizeIn2         = m_pEniSlave->adwPDInSize[1];
    pSlaveInfo->dwPdOffsOut2        = m_pEniSlave->adwPDOutOffset[1];
    pSlaveInfo->dwPdSizeOut2        = m_pEniSlave->adwPDOutSize[1];
    pSlaveInfo->dwPdOffsIn3         = m_pEniSlave->adwPDInOffset[2];
    pSlaveInfo->dwPdSizeIn3         = m_pEniSlave->adwPDInSize[2];
    pSlaveInfo->dwPdOffsOut3        = m_pEniSlave->adwPDOutOffset[2];
    pSlaveInfo->dwPdSizeOut3        = m_pEniSlave->adwPDOutSize[2];
    pSlaveInfo->dwPdOffsIn4         = m_pEniSlave->adwPDInOffset[3];
    pSlaveInfo->dwPdSizeIn4         = m_pEniSlave->adwPDInSize[3];
    pSlaveInfo->dwPdOffsOut4        = m_pEniSlave->adwPDOutOffset[3];
    pSlaveInfo->dwPdSizeOut4        = m_pEniSlave->adwPDOutSize[3];
    if (m_pMaster->m_bPdOffsetCompMode)
    {
    EC_T_DWORD dwCompVal = (m_pMaster->m_dwMinCycInOffset * 8) - m_pMaster->m_dwMinSend;

        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsIn)
        {
            pSlaveInfo->dwPdOffsIn  += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsOut)
        {
            pSlaveInfo->dwPdOffsOut  += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsIn2)
        {
            pSlaveInfo->dwPdOffsIn2  += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsOut2)
        {
            pSlaveInfo->dwPdOffsOut2 += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsIn3)
        {
            pSlaveInfo->dwPdOffsIn3  += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsOut3)
        {
            pSlaveInfo->dwPdOffsOut3 += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsIn4)
        {
            pSlaveInfo->dwPdOffsIn4  += dwCompVal;
        }
        if ((EC_T_DWORD)-1 != pSlaveInfo->dwPdOffsOut4)
        {
            pSlaveInfo->dwPdOffsOut4 += dwCompVal;
        }
    }
    if (HasMailBox())
    {
        ((CEcMbSlave*)this)->GetCfgSlaveInfo(pSlaveInfo);
    }
#if (defined INCLUDE_DC_SUPPORT)
    pSlaveInfo->bDcSupport          = m_bDcSendSyncActivation || m_bDclSendConfiguration;
#else
    pSlaveInfo->bDcSupport          = EC_FALSE;
#endif
#if (defined INCLUDE_VARREAD)
    pSlaveInfo->wNumProcessVarsInp  = m_pEniSlave->wNumProcessVarsInp;
    pSlaveInfo->wNumProcessVarsOutp = m_pEniSlave->wNumProcessVarsOutp;
#endif
    if (0 != m_pEniSlave->wNumPreviousPorts)
    {
        pSlaveInfo->wPrevStationAddress = m_pPreviousPort[0].wSlaveAddress;
        pSlaveInfo->wPrevPort           = m_pPreviousPort[0].wPortNumber;
    }
    else
    {
        pSlaveInfo->wPrevStationAddress = INVALID_FIXED_ADDR;
        pSlaveInfo->wPrevPort           = ESC_PORT_INVALID;
    }

    GetIdentification(&pSlaveInfo->wIdentifyAdo, &pSlaveInfo->wIdentifyData);

    pSlaveInfo->byPortDescriptor        = GetPhysicsDesc();

#if (defined INCLUDE_WKCSTATE)
    pSlaveInfo->wWkcStateDiagOffsIn[0]  = m_awWkcStateDiagOffsIn[0];
    pSlaveInfo->wWkcStateDiagOffsIn[1]  = m_awWkcStateDiagOffsIn[1];
    pSlaveInfo->wWkcStateDiagOffsIn[2]  = m_awWkcStateDiagOffsIn[2];
    pSlaveInfo->wWkcStateDiagOffsIn[3]  = m_awWkcStateDiagOffsIn[3];
    pSlaveInfo->wWkcStateDiagOffsOut[0] = m_awWkcStateDiagOffsOut[0];
    pSlaveInfo->wWkcStateDiagOffsOut[1] = m_awWkcStateDiagOffsOut[1];
    pSlaveInfo->wWkcStateDiagOffsOut[2] = m_awWkcStateDiagOffsOut[2];
    pSlaveInfo->wWkcStateDiagOffsOut[3] = m_awWkcStateDiagOffsOut[3];
#endif /* INCLUDE_WKCSTATE */

    return;
}

/********************************************************************************/
/** \brief Get slave identification.
*
* \return EC_FALSE if slave has no identification
*/
EC_T_BOOL CEcSlave::GetIdentification(EC_T_WORD* pwIdentAdo, EC_T_WORD* pwValue)
{
    if (EC_NULL != pwIdentAdo)
        *pwIdentAdo   = m_pEniSlave->wIdentificationAdo;
    if (EC_NULL != pwValue)
        *pwValue = m_pEniSlave->wIdentificationValue;

    return (0 != m_pEniSlave->wIdentificationAdo) && (0 != m_pEniSlave->wIdentificationValue);
}

/********************************************************************************/
/** \brief Sends an error response to the mailbox client.
*
* \return
*/
EC_T_VOID   CEcMbSlave::StopCurrMbxRequest
(   EC_T_BOOL bIsInitCmd, 
    EC_T_DWORD result, 
    PETHERCAT_MBOX_HEADER pMBox )
{
    m_mbxLayer.eState = m_mbxLayer.IDLE;
    m_bMbxReadStateIsBusy = EC_FALSE;

    if (bIsInitCmd)
    {
        if (AreMbxInitCmdsPending())
        {
#ifdef INCLUDE_LOG_MESSAGES
            EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Mailbox write init command error on slave '%s' (addr 0x%x):\nCommunication Error (0x%x)! Error: '%s'.\n", 
                m_pEniSlave->szName, m_wFixedAddr, result, EcMailboxCmdDescComment(GetMbxInitCmdsCurrent())));
#endif
            StopInitCmds(ECAT_INITCMD_FAILURE, m_wActTransition);
        }
    }
    else if (pMBox != EC_NULL)
    {
        if ((m_pCoEDesc != EC_NULL) && m_pCoEDesc->CurrPendCoeMbxCmdDesc.bIsValid)
        {
            StopCurrCoEMbxRequest(bIsInitCmd, result);
        }
#ifdef INCLUDE_SOE_SUPPORT
        if ((m_pSoEDesc != EC_NULL) && m_pSoEDesc->CurrPendSoeMbxCmdDesc.bIsValid)
        {
            StopCurrSoEMbxRequest(result);
        }
#endif
#if (defined INCLUDE_FOE_SUPPORT)
        if ((m_pFoEDesc != EC_NULL) && m_pFoEDesc->CurrPendFoeMbxCmdDesc.bIsValid)
        {
            StopCurrFoEMbxRequest(bIsInitCmd, result);
        }
#endif /* INCLUDE_FOE_SUPPORT */
#if (defined INCLUDE_VOE_SUPPORT)
        if ((m_pVoEDesc != EC_NULL) && m_pVoEDesc->CurrPendVoeMbxCmdDesc.bIsValid)
        {
            StopCurrVoEMbxRequest(bIsInitCmd, result);
        }
#endif /* INCLUDE_VOE_SUPPORT */

        EC_ERRORMSGC_L(("CEcMbSlave::ProcessCmdResult() Mailbox read error on slave '%s' (addr 0x%x): Error 0x%x!.\n", m_pEniSlave->szName, m_wFixedAddr, result));
    }
}

EC_T_DWORD CEcSlave::SetDeviceState(EC_T_WORD wDeviceState)
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    /* parameter check */
    if (   wDeviceState != DEVICE_STATE_INIT 
        && wDeviceState != DEVICE_STATE_PREOP 
        && wDeviceState != DEVICE_STATE_BOOTSTRAP
        && wDeviceState != DEVICE_STATE_SAFEOP
        && wDeviceState != DEVICE_STATE_OP)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (!m_pMaster->MasterStateMachIsStable())
    {
        /* a single slave state can only be set if the master state is stable */
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }
    if (!IsPresentOnBus())
    {
        dwRetVal = EC_E_SLAVE_NOT_PRESENT;
        goto Exit;
    }
    if ((EC_T_STATE)wDeviceState > GetMaxState())
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* from BOOTSTRAP the only valid transition is to INIT */
    if (DEVICE_STATE_BOOTSTRAP == GetDeviceState())
    {
        if (wDeviceState != DEVICE_STATE_INIT)
        {
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        }
    }

    /* deny higher states than Master State */
    switch (m_pMaster->GetMasterDeviceState())
    {
    case DEVICE_STATE_INIT:
        switch (wDeviceState)
        {
        case DEVICE_STATE_PREOP:
        case DEVICE_STATE_SAFEOP:
        case DEVICE_STATE_OP:
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        default: break;
        } break;
    case DEVICE_STATE_PREOP:
        switch (wDeviceState)
        {
        case DEVICE_STATE_SAFEOP:
        case DEVICE_STATE_OP:
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        default: break;
        } break;
    case DEVICE_STATE_SAFEOP:
        switch (wDeviceState)
        {
        case DEVICE_STATE_OP:
            dwRetVal = EC_E_INVALIDSTATE;
            goto Exit;
        default: break;
        } break;
    case DEVICE_STATE_OP:
        break;
    case DEVICE_STATE_UNKNOWN:
    default:
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    if (!SlaveStateMachIsStable() && (wDeviceState > (EC_T_WORD)GetSmReqDeviceState()))
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    /* Request EtherCAT slave state machine to change to the specified state */
    RequestSmState((EC_T_STATE)wDeviceState);

    dwRetVal = EC_E_NOERROR;

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);

    return OsSetLastError(dwRetVal);
}

/*-END OF SOURCE FILE--------------------------------------------------------*/

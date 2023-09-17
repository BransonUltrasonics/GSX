/*-----------------------------------------------------------------------------
 * EcHotConnect.cpp         file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description
 * Date                     2008/11/19::10:01
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include <EcMasterCommon.h>

#include <EcLink.h>

#include <EcInterfaceCommon.h>
#include <EcInterface.h>
#include <EcEthSwitch.h>
#include <EcServices.h>
#include "EcFiFo.h"
#include <EcIoImage.h>
#include <EcDevice.h>
#include <EcMaster.h>

#include <EcInvokeId.h>

#include <EcSlave.h>

#include <EcConfig.h>
#include <AtEthercatPrivate.h>

#include <EcBusTopology.h>
#if defined (INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif
#include <EcHotConnect.h>

/*-DEFINES-------------------------------------------------------------------*/
/* for development purpose only */
#undef  ECHC_PRINTRECALCULATEDWKC

/* debug message type IDs for link layer debug messages */
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x00)
#define STATE_CHNG      ((EC_T_BYTE)0x00)
#define EEP_STATE_CHNG  ((EC_T_BYTE)0x01)
#define FIXED_ADDR      ((EC_T_BYTE)0x02)

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/

/*-EC_T_HCPORTLISTPORTOP-----------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Create Empty Port List.
  
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD EC_T_HCPORTLISTPORTOP::CreatePortList(
    EC_T_DWORD  dwMaxPortEntries    /**< [in]   size of list */
  , struct _EC_T_MEMORY_LOGGER* pMLog   )
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    
    if( EC_NULL != m_poPorts )
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "EC_T_HCPORTLISTPORTOP:~m_poPorts", m_poPorts, m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX));
        OsFree(m_poPorts);
        m_poPorts = EC_NULL;
    }
    
    m_dwPortEntries     = 0;
    m_dwMaxPortEntries  = dwMaxPortEntries;
    if( m_dwMaxPortEntries == 0 )
    {
        m_poPorts = EC_NULL;
    }
    else
    {
        m_poPorts = (EC_T_SLAVE_PORT_DESC_EX*)OsMalloc(m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX));
        EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "EC_T_HCPORTLISTPORTOP:m_poPorts", m_poPorts, m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX));
        if( EC_NULL == m_poPorts )
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        
        OsMemset(m_poPorts, 0, (m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX)));
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        if( EC_NULL != m_poPorts)
        {
            EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "EC_T_HCPORTLISTPORTOP:~m_poPorts", m_poPorts, m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX));
            OsFree(m_poPorts);
        }
    }

    return dwRetVal;    
}

/***************************************************************************************************/
/**
\brief  Free PortList Memory.
*/
EC_T_VOID EC_T_HCPORTLISTPORTOP::DeletePortList(EC_T_MEMORY_LOGGER* pMLog)
{
    if( EC_NULL != m_poPorts )
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "EC_T_HCPORTLISTPORTOP:~m_poPorts", m_poPorts, m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX));
        OsFree(m_poPorts);
        m_poPorts = EC_NULL;
    }
    m_dwMaxPortEntries = 0;
    m_dwPortEntries = 0;
}

/***************************************************************************************************/
/**
\brief  Clear Port List.
*/
EC_T_VOID EC_T_HCPORTLISTPORTOP::ResetPortList(EC_T_VOID)
{
    OsMemset(m_poPorts, 0, (m_dwMaxPortEntries*sizeof(EC_T_SLAVE_PORT_DESC_EX))); 
    m_dwPortEntries = 0; 
}

/***************************************************************************************************/
/**
\brief  Find Port List entry.

\return Port List entry.
*/
EC_T_SLAVE_PORT_DESC_EX* EC_T_HCPORTLISTPORTOP::FindEntry(
    EC_T_SLAVE_PORT_DESC_EX*    salt            /**< [in]   Start search from, if EC_NULL start from head of list */
   ,EC_T_WORD                   wSlaveAddr      /**< [in]   Slave Fixed Address */
                                                          )
{
    EC_T_SLAVE_PORT_DESC_EX*    pReturn     = EC_NULL;
    EC_T_DWORD                  dwIdx       = 0;
    EC_T_DWORD                  dwInitIdx   = 0;
    
    if( EC_NULL == salt )
    {
        salt = m_poPorts;
    }
    else
    {
        dwInitIdx = (EC_T_DWORD)(salt - &(m_poPorts[0]))+1;
    }
    
    for( dwIdx = dwInitIdx; dwIdx < m_dwPortEntries; dwIdx++ )
    {
        if( (m_poPorts[dwIdx].m_wSlaveAddress == wSlaveAddr) )
        {
            pReturn = &(m_poPorts[dwIdx]);
            
            break;
        }
    }
    
    return pReturn;
}

/***************************************************************************************************/
/**
\brief  Check whether specific port entry is in list.

\return EC_TRUE if in list, EC_FALSE otherwise.
*/
EC_T_BOOL EC_T_HCPORTLISTPORTOP::IsPortInList(
    EC_T_WORD   wSlaveAddr      /**< [in]   Slave fixed address */
   ,EC_T_WORD   wPort           /**< [in]   Port Index 0..3 */
                                             )
{
    EC_T_BOOL                   bInList = EC_FALSE;
    EC_T_SLAVE_PORT_DESC_EX*    pEntry  = EC_NULL;

    while( EC_NULL != (pEntry = FindEntry(pEntry, wSlaveAddr)) )
    {
        if( (EC_T_WORD(-1)) == wPort )
        {
            bInList = EC_TRUE;
        }
        else
        {
            if( 0 != pEntry->m_wFlags[wPort] )
            {
                bInList = EC_TRUE;
                break;
            }
        }
    }
    
    return bInList;
}

/***************************************************************************************************/
/**
\brief  Add Port Entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD EC_T_HCPORTLISTPORTOP::AddPort(
    EC_T_WORD   wSlaveAddr      /**< [in]   Slave fixed Address */
   ,EC_T_WORD   wPort           /**< [in]   Slave PORT (0,1,2,3 = A,B,C,D) */
   ,EC_T_WORD   wFlags          /**< [in]   Port Operation Flags */
                                         )
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    EC_T_SLAVE_PORT_DESC_EX*    pEntry      = EC_NULL;
        
    if( EC_NULL == m_poPorts )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pEntry = FindEntry(pEntry, wSlaveAddr);
    if( EC_NULL == pEntry )
    {
        if( !(m_dwMaxPortEntries> m_dwPortEntries) )
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        pEntry = & m_poPorts[m_dwPortEntries];
        m_dwPortEntries++;
        OsMemset(pEntry, 0, sizeof(EC_T_SLAVE_PORT_DESC_EX));
        pEntry->m_wSlaveAddress = wSlaveAddr;
#if (defined INCLUDE_RED_DEVICE)
        pEntry->m_wPortBehind[0] = ESC_PORT_INVALID;
        pEntry->m_wPortBehind[1] = ESC_PORT_INVALID;
        pEntry->m_wPortBehind[2] = ESC_PORT_INVALID;
        pEntry->m_wPortBehind[3] = ESC_PORT_INVALID;
#endif /* INCLUDE_RED_DEVICE */
    }
    pEntry->m_wFlags[wPort] = wFlags;
    
    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

EC_T_DWORD EC_T_HCPORTLISTPORTOP::AddPort(EC_T_SLAVE_PORT_DESC_EX* pEntrySrc, EC_T_WORD wPort)
{
    EC_T_DWORD                  dwRetVal    = EC_E_ERROR;
    EC_T_SLAVE_PORT_DESC_EX*    pEntry      = EC_NULL;

    if( EC_NULL == m_poPorts )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    pEntry = FindEntry(pEntry, pEntrySrc->m_wSlaveAddress);
    if( EC_NULL == pEntry )
    {
        if( !(m_dwMaxPortEntries> m_dwPortEntries) )
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        pEntry = &m_poPorts[m_dwPortEntries];
        m_dwPortEntries++;
        pEntry->m_wSlaveAddress = pEntrySrc->m_wSlaveAddress;
    }
    pEntry->m_wFlags[wPort]           = pEntrySrc->m_wFlags[wPort];
#if (defined INCLUDE_RED_DEVICE)
    pEntry->m_wFixedAddrBehind[wPort] = pEntrySrc->m_wFixedAddrBehind[wPort];
    pEntry->m_wPortBehind[wPort]      = pEntrySrc->m_wPortBehind[wPort];
#endif /* INCLUDE_RED_DEVICE */

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Add Port Entry.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD EC_T_HCPORTLISTPORTOP::RemovePort( 
    EC_T_WORD   wSlaveAddr      /**< [in]   Slave fixed Address */
   ,EC_T_WORD   wPort           /**< [in]   Port index 0..3 */
                                            )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_DWORD      dwIdx       = 0;
    
    if( EC_NULL == m_poPorts )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    for( dwIdx = 0; dwIdx < m_dwPortEntries; dwIdx++ )
    {
        if( (m_poPorts[dwIdx].m_wSlaveAddress == wSlaveAddr) )
        {
            if( ((EC_T_WORD)-1) == wPort )
            {
                OsMemcpy(&(m_poPorts[dwIdx]), &(m_poPorts[dwIdx+1]), ((m_dwPortEntries-dwIdx-1) * sizeof(EC_T_SLAVE_PORT_DESC_EX)) );
                OsMemset(&(m_poPorts[m_dwPortEntries-1]), 0, sizeof(EC_T_SLAVE_PORT_DESC_EX));
                m_dwPortEntries--;
            }
            else
            {
                EC_T_BOOL bAllZero = EC_TRUE;

                m_poPorts[dwIdx].m_wFlags[wPort] = 0;
                for (EC_T_WORD wPIdx = 0; wPIdx < 4; wPIdx++ )
                {
                    if( 0 != m_poPorts[dwIdx].m_wFlags[wPIdx] )
                    {
                        bAllZero = EC_FALSE;
                        break;
                    }
                }

                if( bAllZero )
                {
                    OsMemcpy(&(m_poPorts[dwIdx]), &(m_poPorts[dwIdx+1]), ((m_dwPortEntries-dwIdx-1) * sizeof(EC_T_SLAVE_PORT_DESC_EX)) );
                    OsMemset(&(m_poPorts[m_dwPortEntries-1]), 0, sizeof(EC_T_SLAVE_PORT_DESC_EX));
                    m_dwPortEntries--;
                }
            }

            if( 0 == m_dwPortEntries )
            {
                OsMemset(&m_poPorts[0], 0, sizeof(EC_T_SLAVE_PORT_DESC_EX));
            }
            
            break;
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

/*-/EC_T_HCPORTLISTPORTOP----------------------------------------------------*/

/*-CECHotConnect-------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default Constructor
*/
CEcHotConnect::CEcHotConnect()
{
    m_dwHotConGroupListLen  = 0;
    m_pHotConnectGroupList  = EC_NULL;

    m_dwCmdDescListLen      = 0;
    m_dwCmdsInList          = 0;
    m_apCmdDescList         = EC_NULL;
    m_pMaster               = EC_NULL;
    m_poEcDevice			= EC_NULL;
    m_dwOutStandingPOCalls  = 0;

    OsMemset(&m_oClosedPorts, 0, sizeof(EC_T_HCPORTLISTPORTOP));
    OsMemset(&m_oPortsToChange, 0, sizeof(EC_T_HCPORTLISTPORTOP));
    m_dwNumSucceedPortOperations = 0;
    m_bPortOperationsFailed      = EC_FALSE;

    m_dwHCCurGroupIdx       = 0;

    m_oHCMode               = EC_HC_DFLT_HCMODE;
    m_bWaitingForManualContinue = EC_FALSE;

    /* state machine */
    OsMemset(&m_oRequest, 0, sizeof(m_oRequest));
    m_pRequest              = EC_NULL;
    m_pCurRequest           = EC_NULL;
    
    m_eCurState             = echcsm_unknown;
    m_eReqState             = echcsm_unknown;

    m_pCurrentHCReq         = EC_NULL;

#if (defined INCLUDE_PORT_OPERATION)
    m_eCurPOCmdState        = echcsmc_idle;
    
    /* Port Operation State Machine */
    OsMemset(&m_oPortOpRequest, 0, sizeof(m_oPortOpRequest));
    m_pPortOpRequest        = EC_NULL;
    m_pCurPortOpRequest     = EC_NULL;

    m_eCurPortOpState       = echcsmpo_unknown;
    m_eReqPortOpState       = echcsmpo_unknown;
    
    m_pCurrentHCPortOpReq   = EC_NULL;
#endif /* INCLUDE_PORT_OPERATION */

    m_bSMRestart            = EC_FALSE;
    m_bSMTimeout    		= EC_FALSE;
    m_dwHCSmResult			= 0;

	m_dwSlavesAtStart       = 0;
	m_bLinkConnectedAtStart = EC_FALSE;
}

/***************************************************************************************************/
/**
\brief  Destructor
*/
CEcHotConnect::~CEcHotConnect()
{
    if( EC_NULL != m_pHotConnectGroupList )
    {
        EC_T_DWORD  dwIdx = 0;
        for( dwIdx = 0; dwIdx < m_dwHotConGroupListLen; dwIdx++ )
        {
        EC_T_HOTCONNECT_GROUP* pGroup = &m_pHotConnectGroupList[dwIdx];

            if (EC_NULL != pGroup->aCycCmdWkcList )
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:~aCycCmdWkcList", pGroup->aCycCmdWkcList, 
                    pGroup->dwMaxNumCycCmdWkcList * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
                OsFree(pGroup->aCycCmdWkcList);
                pGroup->aCycCmdWkcList=EC_NULL;
            }
            if (EC_NULL != pGroup->ppGroupMembers )
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:~ppGroupMembers", pGroup->ppGroupMembers, 
                    pGroup->dwGrpMembers * sizeof(EC_T_VOID*));
                OsFree(pGroup->ppGroupMembers);
                pGroup->ppGroupMembers=EC_NULL;
            }

            if(EC_NULL != pGroup->paIdentCmds)
            {
                EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:~pGroup->paIdentCmds", pGroup->paIdentCmds, 
                    pGroup->dwIdentCmdsCount * sizeof(EcInitCmdDesc*));
                OsFree(pGroup->paIdentCmds);
            }
        }
        
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:~m_pHotConnectGroupList", m_pHotConnectGroupList,
            (m_dwHotConGroupListLen)*sizeof(EC_T_HOTCONNECT_GROUP));
        OsFree(m_pHotConnectGroupList);
    }

    if( EC_NULL != m_apCmdDescList )
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:~m_apCmdDescList", m_apCmdDescList,
            m_dwCmdDescListLen * sizeof(EcCycCmdConfigDesc*));
        OsFree(m_apCmdDescList);
    }

    m_oClosedPorts.DeletePortList(GetMemLog());
    m_oPortsToChange.DeletePortList(GetMemLog());
}

/***************************************************************************************************/
/**
\brief  Create HotConnect Group Entries.

Number of created HC groups = number of configured (optional) HC groups + 1 (the mandatory group)

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::CreateHCGroups(
    EC_T_DWORD  dwGroupCount    /**< [in]   amount of configured HC Groups */
  )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    m_dwHotConGroupListLen = (dwGroupCount+1);
    m_pHotConnectGroupList = (EC_T_HOTCONNECT_GROUP*)OsMalloc((m_dwHotConGroupListLen)*sizeof(EC_T_HOTCONNECT_GROUP));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:m_pHotConnectGroupList", m_pHotConnectGroupList, (m_dwHotConGroupListLen)*sizeof(EC_T_HOTCONNECT_GROUP));

    if( EC_NULL == m_pHotConnectGroupList )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    OsMemset(m_pHotConnectGroupList, 0, ((dwGroupCount+1)*sizeof(EC_T_HOTCONNECT_GROUP)));

    /* List Element 0 == Mandatory Default */
    m_pHotConnectGroupList->oHCGroupType =  hcgrouptype_mandatory;

    /* Mandatory group is always present, but this is set in XPAT Config Layer  !!!! */
    m_pHotConnectGroupList->bGroupComplete = EC_FALSE;
    m_pHotConnectGroupList->bGroupPresent  = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
  
/***************************************************************************************************/
/**
\brief  Get HC Group by Index.

\return Group at Index x.
*/
EC_T_HOTCONNECT_GROUP* CEcHotConnect::GetGroup(
    EC_T_DWORD  dwIndex         /**< [in]   Group Index */
                                              )
{ 
    EC_T_HOTCONNECT_GROUP*  pRet    = EC_NULL; 

    if( ! (dwIndex < m_dwHotConGroupListLen) ) 
    {
        pRet = EC_NULL; 
    }
    else 
    {
        pRet = &(m_pHotConnectGroupList[dwIndex]); 
    }

    return pRet;
}

/***************************************************************************************************/
/**
\brief  Recalculate expected Working Counters, based on HC state.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::RecalculateWKC(EC_T_BOOL bAuto)
{
EC_T_DWORD dwGroupIdx = 0;
EC_T_DWORD dwCmdIdx   = 0;

    if (!bAuto || (!m_pMaster->GetAutoAdjustCycCmdWkc() && (1 < m_dwHotConGroupListLen)))
    {
        /* set all WKC values to 0 */
        for (dwCmdIdx = 0; dwCmdIdx < m_dwCmdsInList; dwCmdIdx++)
        {
            ((EcCycCmdConfigDesc*)(m_apCmdDescList[dwCmdIdx]))->wExpectedWkc = 0;
        }
        /* increment all WKC values according HC groups */
        for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
        {
        EC_T_HOTCONNECT_GROUP*          pHCGroup      = &(m_pHotConnectGroupList[dwGroupIdx]);
        EC_T_CYC_CMD_WKC_DESC_ENTRY*    pTopocalEntry = EC_NULL;

            for (pTopocalEntry = &pHCGroup->aCycCmdWkcList[0]; (EC_NULL != pTopocalEntry) && (EC_NULL != pTopocalEntry->pCycCmdCfgDesc); pTopocalEntry++)
            {
            EcCycCmdConfigDesc* pCycCmdConfigDesc = (EcCycCmdConfigDesc*)pTopocalEntry->pCycCmdCfgDesc;

                if (!pTopocalEntry->bOptional || pTopocalEntry->bActive)
                {
                    pCycCmdConfigDesc->wExpectedWkc = (EC_T_WORD) (pCycCmdConfigDesc->wExpectedWkc + pTopocalEntry->wWkcIncrement);
                }
            }
        }
#if (defined ECHC_PRINTRECALCULATEDWKC)
        EC_DBGMSG("\nA--------\n");
        for( dwScanCmd  =0; dwScanCmd < m_dwCmdsInList; dwScanCmd++ )
        {
            pCurCmdDesc = (EcCycCmdConfigDesc*)m_apCmdDescList[dwScanCmd];;
            if( EC_NULL == pCurCmdDesc )
                continue;

            EC_DBGMSG("CYC Cmd: %s (%02d) 0x%08lx (0x%04x/0x%04x) : XWKC : %02d \n", 
                EcatCmdShortText(EC_AL_ICMDHDR_GET_CMDIDX_CMD(&(pCurCmdDesc->EcCmdHeader))), 
                EC_AL_ICMDHDR_GET_CMDIDX_CMD(&(pCurCmdDesc->EcCmdHeader)), 
                EC_AL_ICMDHDR_GET_ADDR(&(pCurCmdDesc->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR_ADP(&(pCurCmdDesc->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR_ADO(&(pCurCmdDesc->EcCmdHeader)),
                pCurCmdDesc->cntRecv
                    );
        }
        EC_DBGMSG("\nA--------\n");
#endif /* ECHC_PRINTRECALCULATEDWKC */
    }
    return EC_E_NOERROR;
}

#if (defined INCLUDE_HOTCONNECT)
/***************************************************************************************************/
/**
\brief  Handle notification in state GroupAdded
*/
EC_T_VOID CEcHotConnect::HCSmHandleGroupAddedNotification(EC_T_VOID)
{
    EC_T_DWORD dwGroupIdx        = 0;
    EC_T_DWORD dwMaskIdx         = 0;
    EC_T_DWORD dwBitIdx          = 0;
    EC_T_DWORD dwGroupAppear     = 0;
    EC_T_DWORD dwGroupDisappear  = 0;
    EC_T_NOTIFICATION_INTDESC*        pNotification      = EC_NULL;
    EC_T_HC_DETECTALLGROUP_NTFY_DESC* pHCDetectGroupDesc = EC_NULL;

    pNotification = m_pMaster->AllocNotification();
    if (EC_NULL == pNotification)
        return;

    pHCDetectGroupDesc = &(pNotification->uNotification.Notification.desc.HCDetAllGrps);

    /* reset notification descriptor */
    OsMemset(pHCDetectGroupDesc, 0, sizeof(EC_T_HC_DETECTALLGROUP_NTFY_DESC));

    /* walk through hot connect group list, starting at index 1 because index 0 is the mandatory group */
    for (dwGroupIdx = 1; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
    {
    EC_T_HOTCONNECT_GROUP* pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);

        /* handle presence notification */
        if (pHCGroup->bNotifyPresence)
        {
            /* reset notification  flag */
            pHCGroup->bNotifyPresence = EC_FALSE;

            /* count appeared / disappeared slaves  */
            if (pHCGroup->bGroupPresent)
            {
                dwGroupAppear++;
            }
            else
            {
                dwGroupDisappear++;
            }
        }
        /* handle present groups */
        if (pHCGroup->bGroupPresent)
        {
            /* count present groups */
            pHCDetectGroupDesc->dwGroupsPresent++;

            /* fill present bit mask */
            dwMaskIdx = dwGroupIdx/32;
            dwBitIdx  = dwGroupIdx%32;
            if (0 == dwMaskIdx)
            {
                pHCDetectGroupDesc->dwGroupMask |= (1<<dwBitIdx);
            }
            if (99 < dwMaskIdx)
            {
                dwMaskIdx = 99;
            }
            pHCDetectGroupDesc->adwGroupMask[dwMaskIdx] |= (1<<dwBitIdx);
        }
    }
    if ((0 == dwGroupAppear) && (0 == dwGroupDisappear))
    {
        m_pMaster->FreeNotification(pNotification);

        if (echcsm_TopoChangeManual == m_eReqState)
        {
            m_pMaster->NotifyTopologyChangeDone((EC_E_BUSY == m_dwHCSmResult)?(EC_E_NOERROR):(m_dwHCSmResult));
        }
    }
    else
    {
        pHCDetectGroupDesc->dwResultCode = EC_E_NOERROR;
        pHCDetectGroupDesc->dwGroupCount = m_dwHotConGroupListLen;
        if (0 == dwGroupAppear)
        {
            m_pMaster->NotifyAndFree(EC_NOTIFY_HC_PROBEALLGROUPS,  pNotification, sizeof(EC_T_HC_DETECTALLGROUP_NTFY_DESC));
        }
        else
        {
            m_pMaster->NotifyAndFree(EC_NOTIFY_HC_DETECTADDGROUPS, pNotification, sizeof(EC_T_HC_DETECTALLGROUP_NTFY_DESC));
        }
    }
    return;
}
#endif /* INCLUDE_HOTCONNECT */

/***************************************************************************************************/
/**
\brief  Prepare Data storage for cyclic cmds.
*/
EC_T_VOID CEcHotConnect::CreateCycCmd(
    EC_T_DWORD  dwNumCmds   /**< [in]   Total amount of cyclic datagrams */
    )
{
    m_apCmdDescList = (EC_T_VOID**)OsMalloc(dwNumCmds*sizeof(EcCycCmdConfigDesc*));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcHotConnect:m_apCmdDescList", m_apCmdDescList, dwNumCmds*sizeof(EcCycCmdConfigDesc*));
    if (EC_NULL == m_apCmdDescList)
    {
        return;
    }
    m_dwCmdDescListLen = dwNumCmds;
    OsMemset(m_apCmdDescList, 0, (sizeof(EcCycCmdConfigDesc*)*dwNumCmds));
}

/***************************************************************************************************/
/**
\brief  Add Cyclic command entry.
*/
EC_T_VOID CEcHotConnect::AddCycCmd(
    EC_T_VOID* pvCycCmdCfgDesc /**< [in]   Cyclic command to add */
                                  )
{
EcCycCmdConfigDesc* pEcCycCmdConfigDesc = (EcCycCmdConfigDesc*)pvCycCmdCfgDesc;

    if ((EC_NULL != m_apCmdDescList) && (m_dwCmdsInList < m_dwCmdDescListLen))
    {
        m_apCmdDescList[m_dwCmdsInList++] = pEcCycCmdConfigDesc;
    }
}

/***************************************************************************************************/
/**
\brief  Create List of Ports.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::CreatePortList(
    EC_T_DWORD  dwSlaves    /**< [in]   Slaves configured */
                                        )
{
EC_T_DWORD dwRetVal = EC_E_ERROR;

    dwRetVal = m_oClosedPorts.CreatePortList(dwSlaves, GetMemLog());
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }
    dwRetVal = m_oPortsToChange.CreatePortList(dwSlaves, GetMemLog());
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        m_oClosedPorts.DeletePortList(GetMemLog());
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Datagram return processor.
*/
EC_T_VOID CEcHotConnect::ProcessResult(
    EC_T_DWORD              result,         /**< [in] result */
    EC_T_DWORD              invokeId,       /**< [in] invokeId */
    PETYPE_EC_CMD_HEADER    pEcCmdHeader    /**< [in] frame header */
                                      )
{
#if !(defined INCLUDE_PORT_OPERATION)
    EC_UNREFPARM(result);
    EC_UNREFPARM(invokeId);
    EC_UNREFPARM(pEcCmdHeader);
    OsDbgAssert(EC_FALSE);
#else
    EC_T_WORD  wWorkingCnt = 0;
    EC_T_PBYTE pbyData     = EC_NULL;

    if (EC_NULL == pEcCmdHeader)
    {
        goto Exit;
    }    
    wWorkingCnt = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
    pbyData     = (EC_T_PBYTE)EC_ENDOF(pEcCmdHeader);

    switch(invokeId)
    {
    case INVOKE_ID_PORTOP_RDDLCTRLCL:
        {
            if( 0 != m_dwOutStandingPOCalls )
            {
                m_dwOutStandingPOCalls--;
            }

            if(result != EC_E_NOERROR)
            {
                // retry
                OsDbgAssert( (EC_E_TIMEOUT == result) 
                    || (EC_E_FRAME_LOST == result) );    /* only timeout/frame loss errors are expected! */
                
                /* only re-trigger state machine, if operation is still ongoing */
                if( (m_dwOutStandingPOCalls == 0) )
                {
                    /* trigger retry */
                    m_eCurPOCmdState = echcsmc_retry;
                }
                goto Exit;
            }

            if( 1 != wWorkingCnt )
            {
                /* topo changed */
                m_eCurPOCmdState = echcsmc_retry;
                goto Exit;
            }
    
            if( echcsmpo_readdlcontrol_wait == m_eCurPortOpState )
            {
                EC_T_WORD                wAddr     = 0;
                CEcBusSlave*             pBusSlave = EC_NULL;
                EC_T_SLAVE_PORT_DESC_EX* pEntry    = EC_NULL;
                EC_T_BYTE                byCmd     = 0;

                /* get bus slave */
                byCmd = EC_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHeader);
                if( EC_CMD_TYPE_FPRD == byCmd )
                {
                    wAddr = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader));
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wAddr);
                }
                else
                {
                    wAddr = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - m_pMaster->m_pBusTopology->GetConnectedSlaveCount());
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wAddr);
                }
                /* get slave port entry */
                if( EC_NULL != pBusSlave )
                {
	                pEntry = m_oPortsToChange.FindEntry(pEntry, pBusSlave->GetFixedAddress());
                }
                /* handle port changes */
                if( EC_NULL != pEntry )
                {
                    pEntry->m_dwDLControlValue = EC_GET_FRM_DWORD(pbyData);
                }
                else
                {
                    OsDbgAssert(EC_FALSE);
                }
                if( 0 >= m_dwOutStandingPOCalls )
                {
                    m_dwOutStandingPOCalls = 0;
                    m_eCurPOCmdState = echcsmc_idle;
                };
            }   
            else
            {
                OsDbgAssert(m_eCurPortOpState == echcsmpo_reqto);
            }
        } break;
    case INVOKE_ID_PORTOP_CHANGEPORT:
        {
            /* we don't expect this command to be returned */
        } break;
    case INVOKE_ID_PORTOP_CHECKDLCTRLCL:
        {
            if (0 != m_dwOutStandingPOCalls)
            {
                m_dwOutStandingPOCalls--;
            }

            if (result != EC_E_NOERROR)
            {
                // retry
                OsDbgAssert((EC_E_TIMEOUT == result) || (EC_E_FRAME_LOST == result) || (EC_E_LINK_DISCONNECTED == result));   

                /* only re-trigger state machine, if operation is still ongoing */
                if (m_dwOutStandingPOCalls == 0)
                {
                    /* trigger retry */
                    m_eCurPOCmdState = echcsmc_retry;
                }
                goto Exit;
            }

            if (echcsmpo_checkdlctrl_wait == m_eCurPortOpState)
            {
                EC_T_WORD                wAddr     = 0;
                CEcBusSlave*             pBusSlave = EC_NULL;                
                EC_T_SLAVE_PORT_DESC_EX* pEntry    = EC_NULL;
                EC_T_BYTE                byCmd     = 0;

                /* get bus slave */
                byCmd = EC_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHeader);
                if( EC_CMD_TYPE_FPRD == byCmd )
                {
                    wAddr = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader));
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wAddr);
                }
                else
                {
                    wAddr = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - m_pMaster->m_pBusTopology->GetConnectedSlaveCount());
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wAddr);
                }
                /* get slave port entry */
                if( EC_NULL != pBusSlave )
                {
                    pEntry = m_oPortsToChange.FindEntry(pEntry, pBusSlave->GetFixedAddress());
                }
                if (EC_NULL != pEntry)
                {
                    if (wWorkingCnt == 0)
                    {
                        /* since we have possibly exclude a slave by a previous port operation
                        we possibly can not reach the current slave. 
                        unfortunately we can not distinguish between a slave that does not react and a slave that is 
                        not on the bus, so we ignore the current wkc error and pretend that the port operation 
                        was successful.
                        */
                        m_oPortsToChange.RemovePort(pEntry->m_wSlaveAddress);
                    }
                    /* if the port operations was indeed successful we adjust the closed port list */
                    if (wWorkingCnt == 1)
                    {
                        if (pEntry->m_dwDLControlValue == EC_GET_FRM_DWORD(pbyData))
                        {
                            for (EC_T_WORD wPIdx=0; wPIdx<4; wPIdx++)
                            {
                                if( PORT_CLOSE_FLAG == (pEntry->m_wFlags[wPIdx]&PORT_MODIFY_MASK) )
                                {
                                    m_oClosedPorts.AddPort(pEntry, wPIdx);
                                    m_dwNumSucceedPortOperations++;
                                }
                                else if( PORT_OPEN_FLAG == (pEntry->m_wFlags[wPIdx]&PORT_MODIFY_MASK) )
                                {
                                    m_oClosedPorts.RemovePort(pEntry->m_wSlaveAddress, wPIdx);
                                    m_dwNumSucceedPortOperations++;
                                }
                                else
                                {
                                    /* no operation, do not Remove Port from Closed Ports */
                                }
                            }
	                        m_oPortsToChange.RemovePort(pEntry->m_wSlaveAddress);
                        }
                        else
                        {
                            m_bPortOperationsFailed = EC_TRUE;
                        }
                    }
                }
                if (0 == m_dwOutStandingPOCalls)
                {
                    m_dwOutStandingPOCalls = 0;
                    m_eCurPOCmdState = echcsmc_idle;
                }
            }
            else
            {
                OsDbgAssert(m_eCurPortOpState == echcsmpo_reqto);
            }
        }break;
    default:
        {
            OsDbgAssert(EC_FALSE);
        } break;
    }

Exit:
#endif
    return;
}

/***************************************************************************************************/
/**
\brief Refresh HC group active flag and influence list
*/
EC_T_VOID CEcHotConnect::RefreshGroupActive(EC_T_HOTCONNECT_GROUP* pHCGroup)
{
CEcSlave* pHeadSlave   = pHCGroup->ppGroupMembers[0];
EC_T_BOOL bGroupActive;

#if (defined INCLUDE_HOTCONNECT)
    if ((m_pMaster->GetHCMode() & echm_automan_mask) == echm_fullmanual)
    {
        if (DEVICE_STATE_UNKNOWN == pHeadSlave->GetSmReqDeviceState())
        {
            bGroupActive = EC_FALSE;
        }
        else
        {
            switch(pHeadSlave->GetSmDeviceState())
            {
            case DEVICE_STATE_SAFEOP:
            case DEVICE_STATE_OP:
                bGroupActive = EC_TRUE;
                break;
            default:
                bGroupActive = EC_FALSE;
                break;
            }
        }
    }
    else
#endif /* INCLUDE_HOTCONNECT */
    {
        bGroupActive = pHeadSlave->IsPresentOnBus();
    }
    pHCGroup->bGroupActive = bGroupActive;

    /* refresh WKC influence list */
    if (EC_NULL != pHCGroup->aCycCmdWkcList)
    {
        for (EC_T_DWORD dwIdx = 0; EC_NULL != pHCGroup->aCycCmdWkcList[dwIdx].pCycCmdCfgDesc; dwIdx++)
        {
            pHCGroup->aCycCmdWkcList[dwIdx].bActive = pHCGroup->bGroupActive;
        }
    }
}

/***************************************************************************************************/
/**
\brief Set HC group presence
*/
EC_T_VOID CEcHotConnect::SetGroupPresence(EC_T_HOTCONNECT_GROUP* pHCGroup, EC_T_BOOL bPresent)
{
    if (pHCGroup->bGroupPresent == bPresent)
    {
        /* nothing to do */
        return;
    }
    /* refresh group information */
    pHCGroup->bGroupPresent   = bPresent;
    pHCGroup->bNotifyPresence = EC_TRUE;
    if (!bPresent)
    {
        pHCGroup->bGroupTargStateSet = EC_FALSE;
    }
    RefreshGroupActive(pHCGroup);
}

/***************************************************************************************************/
/**
\brief  Refresh config slave presence
*/
EC_T_VOID CEcHotConnect::RefreshCfgSlavePresence(CEcSlave* pCfgSlave)
{
EC_T_DWORD dwHCGroupIdx = pCfgSlave->GetHCGroupId();
EC_T_HOTCONNECT_GROUP* pHCGroup = m_pMaster->m_oHotConnect.GetGroup(dwHCGroupIdx);

    /* activate/deactivate HC groups, detect missing slaves */
    if (pCfgSlave->IsHCGroupHead())
    {
        if (pCfgSlave->IsPresentOnBus())
        {
            /* assume group complete */
            pHCGroup->bGroupComplete = EC_TRUE;
        }
        else
        {
            SetGroupPresence(pHCGroup, EC_FALSE);
        }
    }
    if (!pCfgSlave->IsPresentOnBus())
    {
        /* group incomplete */
        pHCGroup->bGroupComplete = EC_FALSE;
    }
    if (pCfgSlave->IsHCGroupTail())
    {
        if ((0 != dwHCGroupIdx) && !pHCGroup->bGroupPresent)
        {
            if (!pHCGroup->ppGroupMembers[0]->IsPresentOnBus())
            {
                /* mark absent group to perform startup in next detection */
                /* cycle this is needed because all groups are considered */
                /* present on start --see DeviceFactoryInitHotConnect()-- */
                pHCGroup->bGroupTargStateSet = EC_FALSE;
            }
            else
            {
                /* activate group if all slaves present and group not activated yet */
                /* never activate mandatory group                                   */
                if (pHCGroup->bGroupComplete || !m_pMaster->GetAllSlavesMustReachState())
                {
                    SetGroupPresence(pHCGroup, EC_TRUE);
                }
            }
        }
    }
}

/***************************************************************************************************/
/**
\brief  Place Order to HC State Machine, to perform topology change.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::StartTopologyChange(EC_T_VOID)
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (EC_NULL == m_pCurrentHCReq)
    {
        /* handle hot connect mode */
        switch (m_oHCMode & echm_automan_mask)
        {
        case echm_automatic:
            /* start request */
            dwRetVal = RequestState(echcsm_TopoChangeAutomatic, &m_pCurrentHCReq);
            break;
        case echm_manual:
        case echm_fullmanual:
            /* start request */
            dwRetVal = RequestState(echcsm_TopoChangeManual, &m_pCurrentHCReq);
            break;
        default:
            OsDbgAssert(EC_FALSE);
            dwRetVal = EC_E_NOTSUPPORTED;
            break;
        }
    }
    return dwRetVal;
}
/***************************************************************************************************/
/**
\brief  Continue operation in manual mode.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::StartManualContinue(EC_T_VOID)
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    /* check for HC manual mode */
    if (  ((m_oHCMode & echm_automan_mask) != echm_manual)
       && ((m_oHCMode & echm_automan_mask) != echm_fullmanual)
       )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if (EC_NULL == m_pCurrentHCReq)
    {
        /* start request */
        dwRetVal = RequestState(echcsm_TopoChangeManualContinue, &m_pCurrentHCReq);
    }
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Poll state of topology change progress.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::WaitForHCStateMachine(EC_T_VOID)
{
    if (EC_NULL != m_pCurrentHCReq)
    {
        if (m_eCurState == m_eReqState)
        {
            return m_dwHCSmResult;
        }
        else
        {
            return EC_E_BUSY;
        }
    }
    else
    {
        OsDbgAssert(EC_FALSE);
        return EC_E_NOTREADY;
    }
}

/***************************************************************************************************/
/**
\brief  Release HC state machine

\return Result
*/
EC_T_DWORD CEcHotConnect::ReleaseHCStateMachine(EC_T_VOID)
{
    if (EC_NULL != m_pCurrentHCReq)
    {
        ReleaseReq(m_pCurrentHCReq);
        m_pCurrentHCReq = EC_NULL;
        return m_dwHCSmResult;
    }
    else
    {
        OsDbgAssert(EC_FALSE);
        return EC_E_NOTREADY;
    }
}

/***************************************************************************************************/
/**
\brief  Topology changed while HC in progress?.

\return EC_TRUE if topology changed, EC_FALSE otherwise.
*/
EC_T_BOOL CEcHotConnect::TopologyChangedWhileHC(EC_T_VOID)
{
EC_T_BOOL  bTopologyChanged = EC_FALSE;
EC_T_WORD  wSlaveCount      = m_pMaster->GetBrdAlStatusWkc();
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE  byDbgLvl         = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif

    /* topology changed if working counter changed */
    if ((EC_AL_STATUS_INIT_VALUE != wSlaveCount) && (m_dwSlavesAtStart != wSlaveCount) && (0 != wSlaveCount))
    {
        bTopologyChanged = EC_TRUE;

#ifdef INCLUDE_LOG_MESSAGES
        if (byDbgLvl)
        {
            m_pMaster->m_poEcDevice->LinkDbgMsg( byDbgLvl, ETHTYPE1, (ETHTYPE0+FIXED_ADDR),"TopologyChangedWhileHC due to WKC\n");
        }
#endif
    }
    /* topology changed if link disconnected */
    if (m_bLinkConnectedAtStart != m_pMaster->m_poEcDevice->IsLinkConnected())
    {
        bTopologyChanged = EC_TRUE;

#ifdef INCLUDE_LOG_MESSAGES
        if( bTopologyChanged && byDbgLvl )
        {
            m_pMaster->m_poEcDevice->LinkDbgMsg( 
                byDbgLvl, ETHTYPE1, (ETHTYPE0+FIXED_ADDR),
                "TopologyChangedWhileHC due to link disconnected\n");
        }
#endif
    }
    return bTopologyChanged;
}

/***************************************************************************************************/
/**
\brief  Determine number of slaves that belong to a specific group.

\return 0: group does not exist. >0: number of slaves.
*/
EC_T_DWORD CEcHotConnect::GetNumGroupMembers( 
    EC_T_DWORD dwGroupIndex )       /**< [in]  group index, first group (=0) is the mandatory group */
{
    EC_T_HOTCONNECT_GROUP* pGroup;
    EC_T_DWORD dwNumSlaves = 0;
    
    pGroup = GetGroup( dwGroupIndex );
    if( pGroup == EC_NULL )
    {
        // group does not exist
        goto Exit;
    }
    dwNumSlaves = pGroup->dwGrpMembers;

Exit:
    return dwNumSlaves;
}



/***************************************************************************************************/
/**
\brief  Determine slave IDs of a given HotConnect group.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::GetSlaveIdsOfGroup(
     EC_T_DWORD  dwGroupIndex,       /**< [in]  group index, first group (=0) is the mandatory group */
     EC_T_DWORD* adwSlaveId,         /**< [out] slave IDs of the group members */
     EC_T_DWORD  dwMaxNumSlaveIds )  /**< [in]  maximum number of slave ids to be stored in adwSlaveId */
{
    EC_T_DWORD dwResult = EC_E_NOERROR;
    EC_T_HOTCONNECT_GROUP* pGroup;
    EC_T_DWORD dwCnt;
    EC_T_DWORD dwMaxSlaveIds;
    CEcSlave* poSlave;

    for( dwCnt = 0; dwCnt < dwMaxNumSlaveIds; dwCnt++ )
    {
        adwSlaveId[dwCnt] = (EC_T_DWORD)INVALID_SLAVE_ID;
    }
    pGroup = GetGroup( dwGroupIndex );
    if( pGroup == EC_NULL )
    {
        // group does not exist
        dwResult = EC_E_INVALIDPARM;
        goto Exit;
    }
    dwMaxSlaveIds = EC_MIN( dwMaxNumSlaveIds, pGroup->dwGrpMembers );
    for( dwCnt = 0; dwCnt < dwMaxSlaveIds; dwCnt++ )
    {
        poSlave = pGroup->ppGroupMembers[dwCnt];
        adwSlaveId[dwCnt] = poSlave->GetSlaveId();
    }
    
Exit:
    return dwResult;
}

/***************************************************************************************************/
/**
\brief  Configure HotConnect Mode.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::SetMode(
    EC_T_EHOTCONNECTMODE    oMode   /**< [in]   Mode to configure */
                                 )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if (  ((oMode & echm_automan_mask) == echm_automatic)
       || ((oMode & echm_automan_mask) == echm_manual)
       || ((oMode & echm_automan_mask) == echm_fullmanual)
       )
    {
        m_oHCMode = oMode;
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_INVALIDPARM;
    }

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  string conversion routines.

\return string.
*/
EC_T_CHAR* CEcHotConnect::EcHlSmStateString( ECHC_T_ESTATE EHcState )
{
    switch( EHcState )
    {
    case echc_eIdle:            return (EC_T_CHAR*)"echc_eIdle";
    case echc_eWaitStart:       return (EC_T_CHAR*)"echc_eWaitStart";
    case echc_eSBEnhance:       return (EC_T_CHAR*)"echc_eSBEnhance";
    case echc_eScanGrps:        return (EC_T_CHAR*)"echc_eScanGrps";
    case echc_eInitStateWait:   return (EC_T_CHAR*)"echc_eInitStateWait";
    case echc_eReqStateWait:    return (EC_T_CHAR*)"echc_eReqStateWait";
    default:                    return (EC_T_CHAR*)"XXX - unknown state";
    }
}

/***************************************************************************************************/
/**
\brief  Master state String.

\return Master state string.
*/
EC_T_CHAR* CEcHotConnect::MasterStateString( EC_T_WORD  wMasterState )
{
    switch (wMasterState)
    {
    case DEVICE_STATE_UNKNOWN: return (EC_T_CHAR*)"UNKNOWN";
    case DEVICE_STATE_INIT:    return (EC_T_CHAR*)"INIT";
    case DEVICE_STATE_PREOP:   return (EC_T_CHAR*)"PREOP";
    case DEVICE_STATE_SAFEOP:  return (EC_T_CHAR*)"SAFEOP";
    case DEVICE_STATE_OP:      return (EC_T_CHAR*)"OP";
    default:                   return (EC_T_CHAR*)"XXX - unknown state"; 
    }
}

#if (defined INCLUDE_PORT_OPERATION)
/***************************************************************************************************/
/**
\brief  Create Port Change Flags.

\return Flags value for port change.
*/
EC_T_WORD CEcHotConnect::CreatePortChangeFlags(
    EC_T_BOOL   bClose      /**< [in]   EC_TRUE: CLOSE Port, EC_FALSE: OPEN Port */
   ,EC_T_BOOL   bForce      /**< [in]   EC_TRUE: Force Close/Open, EC_FALSE don't force */
   ,EC_T_BYTE   bySource    /**< [in]   Port Change source */
                                              )
{
    EC_T_WORD   wFlags = 0;

    if( bClose )
    {
        wFlags = PORT_CLOSE_FLAG;
        
        if( bForce )
        {
            wFlags |= PORT_MODE_CLOSE;
        }
        else
        {
            wFlags |= PORT_MODE_AUTOCL;
        }
    }
    else
    {
        wFlags = PORT_OPEN_FLAG;
        
        if( bForce )
        {
            wFlags |= PORT_MODE_OPEN;
        }
        else
        {
            wFlags |= PORT_MODE_AUTOCL;
        }
    }
    wFlags |= (bySource<<8);

    return wFlags;
}

/***************************************************************************************************/
/**
\brief  Queue Change port request.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::QueueChangePortRequest(
    EC_T_WORD   wSlaveAddr      /**< [in]   Slaves fixed address */
   ,EC_T_WORD   wPort           /**< [in]   Port Identifier (x%4) */
   ,EC_T_WORD   wFlags          /**< [in]   Port operation flags */
                                                )
{
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE                byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug >> 4) & 3);
#endif
EC_T_DWORD               dwRetVal = EC_E_ERROR;
EC_T_DWORD               dwRes    = EC_E_ERROR;
EC_T_SLAVE_PORT_DESC_EX* pEntry   = EC_NULL;
    
    if (EC_NULL == m_pMaster)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    if( m_eCurPortOpState != m_eReqPortOpState )
    {
        dwRetVal  = EC_E_BUSY;
        goto Exit;
    }
    if (!((PORT_CLOSE_FLAG == (wFlags & PORT_CLOSE_FLAG)) || (PORT_OPEN_FLAG  == (wFlags & PORT_OPEN_FLAG))))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check if port was already closed by another source */ 
    pEntry = m_oClosedPorts.FindEntry(EC_NULL, wSlaveAddr);
    if( EC_NULL != pEntry)
    {
        if(pEntry->m_wFlags[wPort] != 0 && (pEntry->m_wFlags[wPort] >> 8) != (wFlags>> 8))
        {
            dwRetVal =  EC_E_INVALIDSTATE;
            goto Exit;
        }
    }
    /* */
    dwRes = m_oPortsToChange.AddPort(wSlaveAddr, wPort, wFlags);
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "QueueChangePortRequest(): Slave %d, Port %d %s (0x%x)\n", 
        wSlaveAddr, wPort, ((wFlags & PORT_CLOSE_FLAG) ? "close" : ((wFlags & PORT_OPEN_FLAG) ? "open" : "unknown")), dwRetVal));

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Start performing queued Port Open / Close operations.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcHotConnect::StartChangePort(
    EC_T_DWORD  dwTimeout       /**< [in]   Port operation timeout */
                                         )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if( EC_NOWAIT == dwTimeout )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRetVal = RequestPortOpState(echcsmpo_portoperation, dwTimeout, &m_pCurrentHCPortOpReq);

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Poll progress of queued port Open/Close operations.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcHotConnect::PollChangePort(EC_T_VOID)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;

    if( EC_NULL == m_pCurrentHCPortOpReq )
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    dwRetVal = PollPortOpState(m_pCurrentHCPortOpReq);
    if( EC_E_BUSY != dwRetVal && EC_E_NOTREADY != dwRetVal )
    {
        ReleasePortOpReq(m_pCurrentHCPortOpReq);
        m_pCurrentHCPortOpReq = EC_NULL;
    }
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Queue Port Open Request by Source.

\return EC_E_NOERROR on success.
*/
EC_T_DWORD CEcHotConnect::QueuePortsBySource(
    EC_T_BYTE   bySource    /**< [in]   Source value */
                                            )
{
EC_T_DWORD   dwChgPorts = 0;
EC_T_DWORD   dwEntryIdx = 0;
EC_T_WORD    wPortIdx   = 0;
CEcBusSlave* pBusSlave  = EC_NULL;

    if (EC_NULL == m_pMaster)
    {
        dwChgPorts = EC_E_INVALIDSTATE;
        goto Exit;
    }
    for( dwEntryIdx = 0; dwEntryIdx < m_oClosedPorts.m_dwPortEntries; dwEntryIdx++ )
    {
        pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(m_oClosedPorts.m_poPorts[dwEntryIdx].m_wSlaveAddress);
        if( EC_NULL != pBusSlave )
        {
            for( wPortIdx = 0; wPortIdx < 4; wPortIdx++ )
            {
                if( ((m_oClosedPorts.m_poPorts[dwEntryIdx].m_wFlags[wPortIdx]) >> 8) == bySource )
                {
                    if( EC_E_NOERROR == QueueChangePortRequest(
                        pBusSlave->GetFixedAddress(), 
                        wPortIdx,
                        CreatePortChangeFlags(EC_FALSE, EC_FALSE, bySource)
                        ) )
                    {
                        dwChgPorts++;
                    }
                }    
            }
        }
    }

Exit:
    return dwChgPorts;
}

/***************************************************************************************************/
/**
\brief  Open all ports closed by a specific source.

\return Amount of ports queued to change.
*/
EC_T_DWORD CEcHotConnect::OpenPortsBySource(
    EC_T_BYTE   bySource    /**< [in]   Source value */
   ,EC_T_DWORD  dwTimeout   /**< [in]   Operation Timeout */
                                           )
{
    EC_T_DWORD                  dwChgPorts  = 0;

    dwChgPorts = QueuePortsBySource(bySource);

    if( 0 < dwChgPorts )
    {
        if( EC_E_NOERROR != RequestPortOpState(echcsmpo_portoperation, dwTimeout, &m_pCurrentHCPortOpReq) )
        {
            dwChgPorts = 0;
        }
    }

    return dwChgPorts;
}

/***************************************************************************************************/
/**
\brief  Open all ports closed by a specific source.

\return Amount of ports queued to change.
*/
EC_T_DWORD CEcHotConnect::CountClosedPortsBySource(
    EC_T_BYTE   bySource    /**< [in]   Source value */
                                                  )
{
EC_T_DWORD   dwPortCount = 0;
EC_T_DWORD   dwEntryIdx  = 0;
EC_T_WORD    wPortIdx    = 0;    
CEcBusSlave* pBusSlave   = EC_NULL;

    for( dwEntryIdx = 0; dwEntryIdx < m_oClosedPorts.m_dwMaxPortEntries; dwEntryIdx++ )
    {
        pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(m_oClosedPorts.m_poPorts[dwEntryIdx].m_wSlaveAddress);
        if( EC_NULL != pBusSlave )
        {
            for( wPortIdx = 0; wPortIdx < 4; wPortIdx++ )
            {
                if( ((m_oClosedPorts.m_poPorts[dwEntryIdx].m_wFlags[wPortIdx]) >> 8) == bySource )
                {
                    dwPortCount++;
                }    
            }
        }
    }

    return dwPortCount;
}


/***************************************************************************************************/
/**
\brief  Return whether request port is in state closed (by opinion of stack).

\return EC_TRUE: port is in closed list, EC_FALSE: port is not in closed list.
*/
EC_T_BOOL CEcHotConnect::IsPortClosed(
    EC_T_WORD   wSlaveAddr      /**< [in]   Slave address carrying requested port */
   ,EC_T_WORD   wPort           /**< [in]   Requested Port (0..3) */
                                     )
{
    EC_T_BOOL                   bPortClosed = EC_FALSE;

    bPortClosed = m_oClosedPorts.IsPortInList(wSlaveAddr, wPort);

    return bPortClosed;
}
#endif /* INCLUDE_PORT_OPERATION */

EC_T_MEMORY_LOGGER* CEcHotConnect::GetMemLog() 
{ 
    return m_pMaster ? m_pMaster->GetMemLog() : EC_NULL;
}

/***************************************************************************************************/
/**
\brief  State Machine for HotConnect operations.
*/
EC_T_VOID CEcHotConnect::HCStateMachine( EC_T_VOID )
{
#if( defined INCLUDE_DC_SUPPORT )
EC_T_DWORD              dwRes       = EC_E_ERROR;
#endif /* INCLUDE_DC_SUPPORT */
EC_T_BOOL               bContinueSM = EC_FALSE;
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE               byDbgLvl    = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
ECHCSM_T_ESTATEMACHINE  eOldState   = echcsm_unknown;
EC_T_DWORD              dwResSB     = EC_E_ERROR;
#if (defined INCLUDE_HOTCONNECT)
EC_T_DWORD              dwGroupIdx  = 0;
EC_T_HOTCONNECT_GROUP*  pHCGroup    = EC_NULL;
EC_T_DWORD              dwSlvIdx    = 0;
CEcSlave*               pCfgSlave   = EC_NULL;
#endif

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_HCSM);

    /* check whether state machine is idle */
    if( (m_eCurState == m_eReqState) && (EC_NULL == m_pCurRequest) )
    {
        /* stable, check for queued request */
        if( EC_NULL != m_pRequest )
        {
            /* Fetch new Request */
            m_pCurRequest   = m_pRequest;
            /* free Request queue */
            m_pRequest      = EC_NULL;

            /* store current request as target for sm */
            m_eReqState     = m_pCurRequest->eState;

            /* start state Machine */            
            m_eCurState     = echcsm_start;            
            m_dwHCSmResult  = EC_E_BUSY;
            m_bSMTimeout    = EC_FALSE;
            m_bSMRestart    = EC_FALSE;
            bContinueSM     = EC_TRUE;
			m_oTimeout.Stop();
        }
        else
        {
            /* no new request leave SM */
            bContinueSM             = EC_FALSE;
        }
    }
    else
    {
        /* request still pending, go into SM */
        bContinueSM                 = EC_TRUE;
    }

    /* state is stable, request not yet thrown out */
    if( (m_eCurState == m_eReqState) && (EC_NULL != m_pCurRequest) )
    {
        m_pCurRequest = EC_NULL;
        bContinueSM = EC_FALSE;
    }

#if (defined INCLUDE_PORT_OPERATION)
    /* Trigger Port Operation State Machine */
    HCPortOperationStateMachine();
#endif

    while( bContinueSM )
    {
        /* only do a single run of SM per default */
        bContinueSM = EC_FALSE;

        eOldState = m_eCurState;

        /* if Current Request is missing, and SM is not stable -> stuck */
        if( EC_NULL == m_pCurRequest && m_eCurState != m_eReqState && m_eCurState != echcsm_idle )
        {
            OsDbgAssert(EC_FALSE);
            m_eCurState = m_eReqState = echcsm_stuck;
            continue;
        }
        /* handle timeout */
        if (m_oTimeout.IsElapsed())
        {
            m_oTimeout.Stop();
            m_bSMTimeout   = EC_TRUE;
            m_dwHCSmResult = EC_E_TIMEOUT;
            continue;
        }
        /* handle topology changed while HC in progress */
        if ((echcsm_GroupInitState <= m_eCurState) && !m_bSMRestart && TopologyChangedWhileHC())
        {
            /* set trigger, it should be reset in echcsm_start where the */
            /* conditions of TopologyChangedWhileHC will be reset to     */
            m_bSMRestart = EC_TRUE;
        }
        switch( m_eCurState )
        {
        case echcsm_idle:
            {
                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* from idle, decide which way to go */
                switch(m_eReqState)
                {
                case echcsm_TopoChangeManual:
                case echcsm_TopoChangeManualContinue:
                case echcsm_TopoChangeAutomatic:
                    {
                        m_eCurState = echcsm_start;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = echcsm_stuck;
                        bContinueSM = EC_TRUE;
                    } break;
                }
            } break;
        case echcsm_start:
            {
                m_dwHCSmResult = EC_E_BUSY;
                m_bSMRestart   = EC_FALSE;

                /* reset conditions for topology changed while HC in progress */
                m_dwSlavesAtStart       = m_pMaster->GetBrdAlStatusWkc();
                m_bLinkConnectedAtStart = m_pMaster->m_poEcDevice->IsLinkConnected();
                
                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* wait for scan bus */
                dwResSB = m_pMaster->GetBusScanResult();
                if (EC_E_BUSY == dwResSB)
                {                
                    m_eCurState = echcsm_start;
                }
                else
                {
                    /* determine next step */
                    switch(m_eReqState)
                    {
                    case echcsm_TopoChangeManual:
                    case echcsm_TopoChangeAutomatic:
                        {
                            m_eCurState = echcsm_BusScan;
                            bContinueSM = EC_TRUE;
                        } break;
                    case echcsm_TopoChangeManualContinue:
                        {
                            m_eCurState = echcsm_GroupMasterState;
                            bContinueSM = EC_TRUE;
                        } break;
                    default:
                        {
                            OsDbgAssert(EC_FALSE);
                            m_eCurState = m_eReqState = echcsm_stuck;
                            bContinueSM = EC_TRUE;
                        } break;
                    }
                }
            } break;
        case echcsm_restart:
            {
                /* determine next state */
                m_eCurState = echcsm_idle;
                bContinueSM = EC_TRUE;
            } break;
        case echcsm_BusScan:
            {
                dwResSB = m_pMaster->StartBTTopologyChange();
                if( EC_E_NOERROR == dwResSB )
                {
                    m_eCurState = echcsm_BusScan_wait;
                }
            } break;
        case echcsm_BusScan_wait:
            {
                dwResSB = m_pMaster->WaitForBTStateMachine();
                if ((EC_E_BUSY == dwResSB) || (EC_E_NOTREADY == dwResSB))
                {
                    break;
                }
                m_pMaster->ReleaseBTStateMachine(dwResSB);
                if (EC_E_BUSCONFIG_TOPOCHANGE == dwResSB)
                {
                    m_bSMRestart = EC_TRUE;
                }
                else
                {
                    m_pMaster->NotifyScanBusStatus(m_pMaster->GetBusScanResult(), m_pMaster->GetScanBusSlaveCount());
                    if (EC_E_NOERROR != dwResSB)
                    {
                        if (m_pMaster->GetAllSlavesMustReachState() || (dwResSB != EC_E_BUSCONFIG_MISMATCH))
                        {
                            /* DC state machine should always process topology changes. Error handling is done later */
	                        m_dwHCSmResult = dwResSB;
                        }
                    }
                }
                m_eCurState  = echcsm_BusScan_done;
                bContinueSM  = EC_TRUE;
            } break;
        case echcsm_BusScan_done:
            {
                /* during the bus scan, new slaves can appear refresh the topology changed while HC monitoring */
                m_dwSlavesAtStart = m_pMaster->GetBrdAlStatusWkc();
                
                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_GroupAdded;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* handle restart */
                if (m_bSMRestart)
                {
                    m_eCurState = echcsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* determine next state */
#if (defined INCLUDE_DC_SUPPORT)
                m_eCurState = echcsm_DCActivation;
#else
                /* handle error */
                if (EC_E_BUSY != m_dwHCSmResult)
                {
                    m_eCurState = m_eReqState;
                    bContinueSM = EC_TRUE;
                    break;
                }
                switch(m_eReqState)
                {
                case echcsm_TopoChangeManual:
#if (defined INCLUDE_HOTCONNECT)
                    if ((m_pMaster->GetHCMode() & echm_automan_mask) == echm_fullmanual)
                    {
                        m_eCurState = echcsm_GroupAdded;
                        break;
                    }
#endif
                    /* no break */
                case echcsm_TopoChangeAutomatic:
                    m_eCurState = echcsm_GroupInitState;
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = echcsm_stuck;
                    break;
                } 
#endif
                bContinueSM = EC_TRUE;
            } break;

#if (defined INCLUDE_DC_SUPPORT)
        case echcsm_DCActivation:
            {
                if (!m_pMaster->GetDcSupportEnabled() || (m_pMaster->m_wReqMasterDeviceState == DEVICE_STATE_INIT))
                {
                    m_eCurState = echcsm_DCActivation_done;
                    break;
                }
                if( m_pMaster->m_wReqMasterDeviceState == DEVICE_STATE_PREOP )
                {
                    dwRes = m_pMaster->m_pDistributedClks->StartInitDC();
                }
                else
                {
                    dwRes = m_pMaster->m_pDistributedClks->StartTopologyChanged();
                }
                if( EC_E_NOERROR == dwRes )
                {
                    m_eCurState = echcsm_DCActivation_wait;
                }
            } break;
        case echcsm_DCActivation_wait:
            {
                EC_T_DWORD  dwRetVal    = EC_E_ERROR;

                /* check for restart */
                if( m_bSMRestart )
                {
                    dwRetVal = m_pMaster->m_pDistributedClks->CancelRequest();
                    if( EC_E_BUSY == dwRetVal )
                    {
                        /* DC State Machine needs to become stable */
                        bContinueSM = EC_FALSE;
                    }
                }
                
                dwRetVal = m_pMaster->m_pDistributedClks->WaitForStateMachine();
                switch( dwRetVal )
                {
                case EC_E_BUSY:
                    break;
                    
                case EC_E_NOERROR:
                    m_eCurState = echcsm_DCActivation_done;
                    bContinueSM = EC_TRUE;
                    break;

                case EC_E_CANCEL:
                    if( m_bSMRestart )
                    {
                        m_eCurState  = echcsm_restart;
                        break;
                    }
                    else
                    {
	                    m_dwHCSmResult = dwRetVal;
                        m_eCurState = m_eReqState;
	                    bContinueSM = EC_TRUE;
                    }
                    break;
                case EC_E_TIMEOUT:
                case EC_E_LINK_DISCONNECTED:
                case EC_E_DC_REF_CLOCK_NOT_FOUND:
                case EC_E_DC_REF_CLOCK_SYNC_OUT_UNIT_DISABLED:
                case EC_E_DC_SLAVES_BEFORE_REF_CLOCK:
                    m_dwHCSmResult = dwRetVal;
                    m_eCurState = m_eReqState;
                    bContinueSM = EC_TRUE;
                    break;
                    
                case EC_E_NOTREADY:
                    EC_DBGMSG( "HCStateMachine(echcsm_DCActivation_wait): Internal Error - GetDCStatus() returned EC_E_NOTREADY\n" );
                    m_eCurState = m_eReqState = echcsm_stuck;
                    OsDbgAssert(EC_FALSE);
                    break;
                    
                default:
                    EC_DBGMSG( "HCStateMachine(echcsm_DCActivation_wait): Internal Error - GetDCStatus() returned 0x%x\n", dwRetVal );
                    m_eCurState = m_eReqState = echcsm_stuck;
                    OsDbgAssert(EC_FALSE);
                    break;
                }
                if( ((EC_E_BUSY != dwRetVal) && !m_bSMRestart) || (echcsm_stuck == m_eCurState))
                {
                    m_pMaster->NotifyDcStatus(dwRetVal);
                }
            } break;
        case echcsm_DCActivation_done:
            {
                /* handle error */
                if (EC_E_BUSY != m_dwHCSmResult)
                {
                    m_eCurState = m_eReqState;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_GroupAdded;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* handle restart */
                if (m_bSMRestart)
                {
                    m_eCurState = echcsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* determine next state */
                switch(m_eReqState)
                {
                case echcsm_TopoChangeManual:
#if (defined INCLUDE_HOTCONNECT)
                    if ((m_pMaster->GetHCMode() & echm_automan_mask) == echm_fullmanual)
#endif
                    {
                        m_eCurState = echcsm_GroupAdded;
                        break;
                    }
                    /* no break */
                case echcsm_TopoChangeAutomatic:
                    m_eCurState = echcsm_GroupInitState;
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = echcsm_stuck;
                    break;
                } 
                bContinueSM = EC_TRUE;
            } break;
#endif /* INCLUDE_DC_SUPPORT */

        case echcsm_GroupInitState:
            {
                /* apply timeout to state change */
                m_oTimeout.Start(m_pMaster->m_dwHCDetectionTimeout, m_pMaster->GetMsecCounterPtr());

#if (defined INCLUDE_HOTCONNECT)
                /* trigger Recalc WKC */
                RecalculateWKC(EC_TRUE);

                /* set each additionally found group member first into INIT and then into the same OP state as master state */
                for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
                {
                    pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);
                    
                    if( pHCGroup->bGroupTargStateSet || !pHCGroup->bGroupPresent)
                    {
                        /* group already in target state or absent */
                        continue;
                    }
                    
                    /* now set each group member slave to state INIT */
                    for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ )
                    {
                        pCfgSlave = pHCGroup->ppGroupMembers[dwSlvIdx];
                        if( EC_NULL == pCfgSlave )
                        {
                            OsDbgAssert(EC_FALSE);
                            continue;
                        }
                        pCfgSlave->ResetSlaveStateMachine();

                        /* request INIT state */
                        m_pMaster->RequestSlaveSmState(pCfgSlave, eEcatState_INIT);
                        
#ifdef INCLUDE_LOG_MESSAGES
                        if( byDbgLvl > 0 )
                        {
                            m_poEcDevice->LinkDbgMsg( 
                                byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                "HCStateMachine: set slave to INIT: '%s', CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                pCfgSlave->GetName(), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                                    );
                        }
#endif
                    }
                }
                m_eCurState = echcsm_GroupInitState_wait;
#else
                m_eCurState = echcsm_GroupInitState_done;
#endif /* INCLUDE_HOTCONNECT */
            } break;
#if (defined INCLUDE_HOTCONNECT)
        case echcsm_GroupInitState_wait:
            {
            EC_T_BOOL bAllSlavesInReqState = EC_TRUE;
            EC_T_BOOL bAllGroupsInReqState = EC_TRUE;

                /* handle timeout/restart */
                if (m_bSMTimeout || m_bSMRestart)
                {
                    m_eCurState = echcsm_GroupInitState_done;
                    break;
                }
                /* Probe for state reached */
                for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
                {
                    pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);

                    if( pHCGroup->bGroupTargStateSet || !pHCGroup->bGroupPresent)
                    {
                        /* group already in target state or absent */
                        continue;
                    }
                    bAllSlavesInReqState = EC_TRUE;
                
                    /* now set each group member slave to state INIT */
                    for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ )
                    {
                        pCfgSlave = pHCGroup->ppGroupMembers[dwSlvIdx];
                        if( EC_NULL == pCfgSlave )
                        {
                            OsDbgAssert(EC_FALSE);
                            continue;
                        }
                        if( (pCfgSlave->GetSmDeviceState() == DEVICE_STATE_INIT) && pCfgSlave->SlaveStateMachIsStable())
                        {
                            if (DEVICE_STATE_INIT != (pCfgSlave->GetAlStatus() & DEVICE_STATE_MASK))
                            {
                                /* should not happen, just initiate INIT state again */
                                OsDbgAssert( EC_FALSE );

                                pCfgSlave->ResetSlaveStateMachine();
                            
                                /* request INIT state */
                                m_pMaster->RequestSlaveSmState(pCfgSlave, eEcatState_INIT);
#ifdef INCLUDE_LOG_MESSAGES
                                if( byDbgLvl > 0 )
                                {
                                    m_poEcDevice->LinkDbgMsg( 
                                        byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                        "HCStateMachine: FATAL ERROR - set slave to INIT again: '%s', AL-STATUS = 0x%x, CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                        pCfgSlave->GetName(), pCfgSlave->GetAlStatus(), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                                            );
                                }
#endif
                                bAllSlavesInReqState = EC_FALSE;
                                break;
                            }
                        }
                        else if (!m_pMaster->GetAllSlavesMustReachState() && (!pCfgSlave->IsPresentOnBus() || pCfgSlave->TargetStateNotReachable()))
                        {
                            /* consider slave in requested state */
                        }
                        else
                        {
                            bAllSlavesInReqState = EC_FALSE;
                            break;
                        }
                    } /* for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ ) */
                    if (!bAllSlavesInReqState)
                    {
                        bAllGroupsInReqState = EC_FALSE;
                    }
                }
                /* determine next state */
                if (bAllGroupsInReqState)
                {
                    m_eCurState = echcsm_GroupInitState_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
#endif /* INCLUDE_HOTCONNECT */
        case echcsm_GroupInitState_done:
            {
                m_oTimeout.Stop();

                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_GroupAdded;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* handle restart */
                if (m_bSMRestart)
                {
                    m_eCurState = echcsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* determine next state */
                switch(m_eReqState)
                {
                case echcsm_TopoChangeManual:
                case echcsm_TopoChangeAutomatic:
                    switch (m_pMaster->m_wReqMasterDeviceState)
                    {
                    case DEVICE_STATE_PREOP:
                    case DEVICE_STATE_SAFEOP:
                    case DEVICE_STATE_OP:
                        m_eCurState = echcsm_GroupPreopState;
                        break;
                    case DEVICE_STATE_UNKNOWN:
                    case DEVICE_STATE_INIT:
                        m_eCurState = echcsm_GroupAdded;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState;
                        break;
                    }
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = echcsm_stuck;
                    break;
                } 
                bContinueSM = EC_TRUE;
            } break;
        case echcsm_GroupPreopState:
            {
                /* apply timeout to state change */
                m_oTimeout.Start(m_pMaster->m_dwHCDetectionTimeout, m_pMaster->GetMsecCounterPtr());

#if (defined INCLUDE_HOTCONNECT)
                /* set each additionally found group member into PREOP state */
                for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
                {
                    pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);
                    
                    if( pHCGroup->bGroupTargStateSet || !pHCGroup->bGroupPresent)
                    {
                        /* group already in target state or absent */
                        continue;
                    }
                    
                    /* now set each group member slave to state PREOP */
                    for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ )
                    {
                        pCfgSlave = pHCGroup->ppGroupMembers[dwSlvIdx];
                        if( EC_NULL == pCfgSlave )
                        {
                            OsDbgAssert(EC_FALSE);
                            continue;
                        }
                        /* request PREOP state */
                        m_pMaster->RequestSlaveSmState(pCfgSlave, eEcatState_PREOP);

#ifdef INCLUDE_LOG_MESSAGES
                        if( byDbgLvl > 0 )
                        {
                            m_poEcDevice->LinkDbgMsg( 
                                byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                "HCStateMachine: set slave to PREOP: '%s', CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                pCfgSlave->GetName(), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                                    );
                        }
#endif
                    }
                }
                m_eCurState = echcsm_GroupPreopState_wait;
#else
                m_eCurState = echcsm_GroupPreopState_done;
#endif /* INCLUDE_HOTCONNECT */
            } break;
#if (defined INCLUDE_HOTCONNECT)
        case echcsm_GroupPreopState_wait:
            {
            EC_T_BOOL bAllSlavesInReqState = EC_TRUE;
            EC_T_BOOL bAllGroupsInReqState = EC_TRUE;

                /* handle timeout/restart */
                if (m_bSMTimeout || m_bSMRestart)
                {
                    m_eCurState = echcsm_GroupPreopState_done;
                    break;
                }
                /* Probe for state reached */
                for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
                {
                    pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);
                
                    if( pHCGroup->bGroupTargStateSet || !pHCGroup->bGroupPresent)
                    {
                        /* group already in target state or absent */
                        continue;
                    }
                
                    /* now set each group member slave to state PREOP */
                    for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ )
                    {
                        pCfgSlave = pHCGroup->ppGroupMembers[dwSlvIdx];
                        if( EC_NULL == pCfgSlave )
                        {
                            OsDbgAssert(EC_FALSE);
                            continue;
                        }
                        bAllSlavesInReqState = EC_TRUE;
                    
                        if( (pCfgSlave->GetSmDeviceState() == DEVICE_STATE_PREOP) && pCfgSlave->SlaveStateMachIsStable() )
                        {
                            if (eEcatState_PREOP != pCfgSlave->GetDeviceState())
                            {
                                /* should not happen, just initiate PREOP state */
                                OsDbgAssert( EC_FALSE );

                                pCfgSlave->ResetSlaveStateMachine();
                            
                                /* request PREOP state */
                                m_pMaster->RequestSlaveSmState(pCfgSlave, eEcatState_PREOP);
#ifdef INCLUDE_LOG_MESSAGES
                                if( byDbgLvl > 0 )
                                {
                                    m_poEcDevice->LinkDbgMsg( 
                                        byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                        "HCStateMachine: FATAL ERROR - set slave to PREOP : '%s', AL-STATUS = 0x%x, CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                        pCfgSlave->GetName(), pCfgSlave->GetAlStatus(), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                                            );
                                }
#endif
                                bAllSlavesInReqState = EC_FALSE;
                                break;
                            }
                        }
                        else if (!m_pMaster->GetAllSlavesMustReachState() && (!pCfgSlave->IsPresentOnBus() || pCfgSlave->TargetStateNotReachable()))
                        {
                            /* consider slave in requested state */
                        }
                        else
                        {
                            bAllSlavesInReqState = EC_FALSE;
                            break;
                        }
                    } /* for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ ) */
                    if (!bAllSlavesInReqState)
                    {
                        bAllGroupsInReqState = EC_FALSE;
                    }
                }
                /* determine next state */
                if (bAllGroupsInReqState)
                {
                    m_eCurState = echcsm_GroupPreopState_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
#endif /* INCLUDE_HOTCONNECT */
        case echcsm_GroupPreopState_done:
            {
                m_oTimeout.Stop();

                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_GroupAdded;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* handle restart */
                if (m_bSMRestart)
                {
                    m_eCurState = echcsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* determine next state */
                switch(m_eReqState)
                {
                case echcsm_TopoChangeManual:
                case echcsm_TopoChangeAutomatic:
                    m_eCurState = echcsm_GroupAdded;
                    bContinueSM = EC_TRUE;
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = echcsm_stuck;
                    break;
                } 
                bContinueSM = EC_TRUE;
            } break;


        case echcsm_GroupAdded:
            {
#if (defined INCLUDE_HOTCONNECT)
                HCSmHandleGroupAddedNotification();
#endif /* INCLUDE_HOTCONNECT */
                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* determine next state */
                switch(m_eReqState)
                {
                case echcsm_TopoChangeManual:
                    m_eCurState = echcsm_TopoChangeManual;
                    bContinueSM = EC_TRUE;
                    break;
                case echcsm_TopoChangeAutomatic:
                    switch (m_pMaster->m_wReqMasterDeviceState)
                    {
                    case DEVICE_STATE_SAFEOP:
                    case DEVICE_STATE_OP:
                        m_eCurState = echcsm_GroupMasterState;
                        break;
                    case DEVICE_STATE_UNKNOWN:
                    case DEVICE_STATE_INIT:
                    case DEVICE_STATE_PREOP:
                        m_eCurState = echcsm_GroupMasterState_done;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = echcsm_GroupMasterState_done;
                        break;
                    }
                    bContinueSM = EC_TRUE;
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = echcsm_stuck;
                    break;
                }
            } break;

        case echcsm_GroupMasterState:
            {
                /* apply timeout to state change */
                m_oTimeout.Start(m_pMaster->m_dwHCDetectionTimeout, m_pMaster->GetMsecCounterPtr());

#if (defined INCLUDE_HOTCONNECT)
                /* since a master transition may be in progress, while Topo Change is detected,
                * try to set newly detected nodes to Requested master state, not the state, the master has
                * reached
                */
                EC_T_WORD wTargetState = m_pMaster->m_wReqMasterDeviceState;

                for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
                {
                    pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);
                    
                    if( pHCGroup->bGroupTargStateSet || !pHCGroup->bGroupPresent)
                    {
                        /* group already in target state or absent */
                        continue;
                    }
                    
                    /* now set each group member slave to the same OP state as the master state */
                    for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ )
                    {
                        pCfgSlave = pHCGroup->ppGroupMembers[dwSlvIdx];
                        if( EC_NULL == pCfgSlave )
                        {
                            OsDbgAssert(EC_FALSE);
                            continue;
                        }
                        if (wTargetState != (EC_T_WORD)pCfgSlave->GetDeviceState())
                        {
                            m_pMaster->RequestSlaveSmState(pCfgSlave, wTargetState);
#ifdef INCLUDE_LOG_MESSAGES
                            if( byDbgLvl > 0 )
                            {
                                m_poEcDevice->LinkDbgMsg( 
                                    byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                    "HCStateMachine: set slave to '%s': '%s', CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                    MasterStateString(wTargetState), pCfgSlave->GetName(), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                    );
                            }
#endif
                        }
#ifdef INCLUDE_LOG_MESSAGES
                        else if( byDbgLvl > 0 )
                        {
                            m_poEcDevice->LinkDbgMsg( 
                                byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                "HCStateMachine: slave '%s' is already in state '%s', CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                pCfgSlave->GetName(), MasterStateString(wTargetState), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                );
                        }
#endif
                    }
                }
                m_eCurState = echcsm_GroupMasterState_wait;
#else
                m_eCurState = echcsm_GroupMasterState_done;
#endif /* INCLUDE_HOTCONNECT */
            } break;
#if (defined INCLUDE_HOTCONNECT)
        case echcsm_GroupMasterState_wait:
            {
            EC_T_BOOL bAllSlavesInReqState = EC_TRUE;
            EC_T_BOOL bAllGroupsInReqState = EC_TRUE;

                /* handle timeout/restart */
                if (m_bSMTimeout || m_bSMRestart)
                {
                    m_eCurState = echcsm_GroupMasterState_done;
                    break;
                }
                /* Probe for state reached */
                for (dwGroupIdx = 0; dwGroupIdx < m_dwHotConGroupListLen; dwGroupIdx++)
                {
                    pHCGroup = &(m_pHotConnectGroupList[dwGroupIdx]);
                    
                    if( pHCGroup->bGroupTargStateSet || !pHCGroup->bGroupPresent)
                    {
                        /* group already in target state or absent */
                        continue;
                    }
                    
                    /* now set each group member slave to state INIT */
                    for (dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++)
                    {
                        pCfgSlave = pHCGroup->ppGroupMembers[dwSlvIdx];
                        if( EC_NULL == pCfgSlave )
                        {
                            OsDbgAssert(EC_FALSE);
                            continue;
                        }
                        if ((pCfgSlave->GetSmDeviceState() == pCfgSlave->GetSmReqDeviceState()) && pCfgSlave->SlaveStateMachIsStable())
                        {
                            if (pCfgSlave->GetSmReqDeviceState() != pCfgSlave->GetDeviceState())
                            {
                                /* should not happen, just initiate to enter the requested state again */
                                OsDbgAssert( EC_FALSE );
                                
                                pCfgSlave->ResetSlaveStateMachine();
                                
                                /* request final state */
                                m_pMaster->RequestSlaveSmState(pCfgSlave, (EC_T_WORD)pCfgSlave->GetSmReqEcatState());
#ifdef INCLUDE_LOG_MESSAGES
                                if( byDbgLvl > 0 )
                                {
                                    m_poEcDevice->LinkDbgMsg( 
                                        byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_HL,
                                        "HCStateMachine: FATAL ERROR - set slave to 0x%x again: '%s', AL-STATUS = 0x%x, CfgAddr/CurrAddr = 0x%x/0x%x\n",
                                        pCfgSlave->GetSmReqDeviceState(), pCfgSlave->GetName(), pCfgSlave->GetAlStatus(), pCfgSlave->GetCfgFixedAddr(), pCfgSlave->GetFixedAddr()
                                        );
                                }
#endif
                                bAllSlavesInReqState = EC_FALSE;
                                break;
                            }
                        }
                        else if (!m_pMaster->GetAllSlavesMustReachState() && (!pCfgSlave->IsPresentOnBus() || pCfgSlave->TargetStateNotReachable()))
                        {
                            /* consider slave in requested state */
                        }
                        else
                        {
                            bAllSlavesInReqState = EC_FALSE;
                            break;
                        }
                    } /* for( dwSlvIdx = 0; dwSlvIdx < pHCGroup->dwGrpMembers; dwSlvIdx++ ) */
                    if (bAllSlavesInReqState)
                    {
                        pHCGroup->bGroupTargStateSet = EC_TRUE;
                    }
                    else
                    {
                        bAllGroupsInReqState = EC_FALSE;
                    }
                }
                /* determine next state */
                if (bAllGroupsInReqState)
                {
                    m_eCurState = echcsm_GroupMasterState_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
#endif /* INCLUDE_HOTCONNECT */
        case echcsm_GroupMasterState_done:
            {
                m_oTimeout.Stop();

                /* handle timeout */
                if (m_bSMTimeout)
                {
                    m_eCurState = echcsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* handle restart */
                if (m_bSMRestart)
                {
                    m_eCurState = echcsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* determine next state */
                switch( m_eReqState )
                {
                case echcsm_TopoChangeManualContinue:
                case echcsm_TopoChangeAutomatic:
                    m_eCurState = m_eReqState;
                    break;
                default: 
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = echcsm_stuck;
                }
                bContinueSM = EC_TRUE;
            } break;
        case echcsm_reqto_wait:
            {
                m_eCurState = m_eReqState;
                bContinueSM = EC_TRUE;
            } break;
        case echcsm_TopoChangeManualContinue:
        case echcsm_TopoChangeAutomatic:
            {
                /* initial scan bus error code not changed while scan -> no error occurred */ 
                if (EC_E_BUSY == m_dwHCSmResult)
                {
                    m_dwHCSmResult = EC_E_NOERROR;
                }
                /* handle notification */
                m_pMaster->NotifyTopologyChangeDone(m_dwHCSmResult);

                m_pMaster->m_pBusTopology->TopologyChangeDone();
                m_bWaitingForManualContinue = EC_FALSE;

                /* store result and throw out request */
                m_pCurRequest->dwResult = m_dwHCSmResult;
                m_pCurRequest = EC_NULL;

                /* determine next state */
                m_eCurState = m_eReqState = echcsm_idle;
            } break;
        case echcsm_TopoChangeManual:
            {
                /* initial scan bus error code not changed while scan -> no error occurred */
                if (EC_E_BUSY == m_dwHCSmResult)
                {
                    m_dwHCSmResult = EC_E_NOERROR;
                }
                m_pMaster->m_pBusTopology->TopologyChangeDone();
                m_bWaitingForManualContinue = EC_TRUE;

                /* store result and throw out request */
                m_pCurRequest->dwResult = m_dwHCSmResult;
                m_pCurRequest = EC_NULL;

                /* determine next state */
                m_eCurState = m_eReqState = echcsm_idle;
            } break;
        case echcsm_stuck:
            {
                m_pCurRequest->dwResult = m_dwHCSmResult;

                m_eCurState = echcsm_idle;
                bContinueSM = EC_TRUE;
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_eCurState = m_eReqState = echcsm_stuck;
                bContinueSM = EC_FALSE;
            } /* default */
        } /* switch m_eCurState */

#ifdef INCLUDE_LOG_MESSAGES
        if( (m_eCurState != eOldState) && byDbgLvl )
        {
            m_poEcDevice->LinkDbgMsg( 
                byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_SC,
                "HCStateMachine: %s->%s (%s, 0x%08X)\n", EHCSTATESText(eOldState), EHCSTATESText(m_eCurState), EHCSTATESText(m_eReqState), m_dwHCSmResult);
        }
#endif
    } /* while( bContinueSM)  */

    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_HCSM);

    return;
}

/***************************************************************************************************/
/**
\brief  Enqueue new Request for HotConnect Instance.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcHotConnect::RequestState(
    ECHCSM_T_ESTATEMACHINE  eState          /**< [in]   Desired state */
   ,ECHCSM_REQ**            pHandle         /**< [out]  Handle to Request if EC_E_NOERROR is returned */
                                       )
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
    
    if( EC_NULL == pHandle )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( (m_oRequest[0].bUsed) && (m_oRequest[1].bUsed) )
    {
        dwRetVal = EC_E_BUSY;
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }

    OsDbgAssert( EC_NULL == m_pRequest );
    if( EC_NULL == m_pRequest )
    {
        if( m_oRequest[0].bUsed )
        {
            (*pHandle) = &(m_oRequest[1]);
        }
        else
        {
            (*pHandle) = &(m_oRequest[0]);
        }
        (*pHandle)->bUsed       = EC_TRUE;
        (*pHandle)->eState      = eState;
        (*pHandle)->dwResult    = EC_E_BUSY;
         m_pRequest = (*pHandle);

        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_BUSY;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Release handle after reading result code.

\return error code.
*/
EC_T_DWORD CEcHotConnect::ReleaseReq(
    ECHCSM_REQ*     pHandle     /**< [in]   desired request to release */
                                     )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if( (pHandle != &(m_oRequest[0])) && (pHandle != &(m_oRequest[1])) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    if( pHandle == m_pRequest )
    {
        m_pRequest = EC_NULL;
    }

    if( pHandle == m_pCurRequest )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    pHandle->bUsed = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#if (defined INCLUDE_PORT_OPERATION)

EC_T_DWORD CEcHotConnect::s_dwClosePortReserved = 0;
EC_T_DWORD CEcHotConnect::ClosePortAtMainLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved)
{
CEcMaster* pMaster = (CEcMaster*)pCallerData;
    
    EC_UNREFPARM(pdwReserved);
    pMaster->m_pBusTopology->MaskSendOnMain(EC_TRUE);
    return EC_E_NOERROR;
}
EC_T_DWORD CEcHotConnect::OpenPortAtMainLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved)
{
CEcMaster* pMaster = (CEcMaster*)pCallerData;
    
    EC_UNREFPARM(pdwReserved);
    pMaster->m_pBusTopology->MaskSendOnMain(EC_FALSE);
    return EC_E_NOERROR;
}
#if (defined INCLUDE_RED_DEVICE)
EC_T_DWORD CEcHotConnect::ClosePortAtRedLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved)
{
CEcMaster* pMaster = (CEcMaster*)pCallerData;
    
    EC_UNREFPARM(pdwReserved);
    pMaster->m_pBusTopology->MaskSendOnRed(EC_TRUE);
    return EC_E_NOERROR;
}
EC_T_DWORD CEcHotConnect::OpenPortAtRedLine(EC_T_PVOID pCallerData, EC_T_DWORD* pdwReserved)
{
CEcMaster* pMaster = (CEcMaster*)pCallerData;
    
    EC_UNREFPARM(pdwReserved);
    pMaster->m_pBusTopology->MaskSendOnRed(EC_FALSE);
    return EC_E_NOERROR;
}
#endif /* INCLUDE_RED_DEVICE */

/***************************************************************************************************/
/**
\brief  State Machine for HotConnect operations.
*/
EC_T_VOID CEcHotConnect::HCPortOperationStateMachine(EC_T_VOID)
{
EC_T_DWORD              dwRes       = EC_E_ERROR;
EC_T_BOOL               bContinueSM = EC_FALSE;
#ifdef INCLUDE_LOG_MESSAGES
EC_T_BYTE               byDbgLvl    = 0;
#endif
ECHCSM_T_EPORTOPERATION eOldState   = echcsmpo_unknown;
EC_T_DWORD              dwResSB     = EC_E_ERROR;

    if (EC_NULL == m_pMaster)
    {
        goto Exit;
    }
#ifdef INCLUDE_LOG_MESSAGES
    byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif

    /* check whether state machine is idle */
    if( (m_eCurPortOpState == m_eReqPortOpState) && (EC_NULL == m_pCurPortOpRequest) )
    {
        /* stable, check for queued request */
        if( EC_NULL != m_pPortOpRequest )
        {
            /* Fetch new Request */
            m_pCurPortOpRequest     = m_pPortOpRequest;
            /* free Request queue */
            m_pPortOpRequest        = EC_NULL;

            /* store current request as target for sm */
            m_eReqPortOpState       = m_pCurPortOpRequest->eState;

            /* start state Machine */
            m_eCurPortOpState       = echcsmpo_start;
            bContinueSM             = EC_TRUE;
        }
        else
        {
            /* no new request leave SM */
            bContinueSM             = EC_FALSE;
        }
    }
    else
    {
        /* request still pending, go into SM */
        bContinueSM                 = EC_TRUE;
    }

    /* state is stable, request not yet thrown out */
    if( (m_eCurPortOpState == m_eReqPortOpState) && (EC_NULL != m_pCurPortOpRequest) )
    {
        m_pCurPortOpRequest->oTimeout.Stop();
        
        m_pCurPortOpRequest = EC_NULL;
        bContinueSM = EC_FALSE;
    }
    
    while( bContinueSM )
    {
        /* only do a single run of SM per default */
        bContinueSM = EC_FALSE;

        eOldState = m_eCurPortOpState;

        /* if Current Request is missing, and SM is not stable -> stuck */
        if( EC_NULL == m_pCurPortOpRequest && m_eCurPortOpState != m_eReqPortOpState && m_eCurPortOpState != echcsmpo_idle )
        {
            OsDbgAssert(EC_FALSE);
            m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
            continue;
        }



        switch( m_eCurPortOpState )
        {
            /* a timeout did occur, all requests are stopped from sub state machines */
        case echcsmpo_reqto_wait:
            {
                /* determine next state */
                m_eCurPortOpState = echcsmpo_reqto;
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_reqto:
            {
                m_oPortsToChange.ResetPortList();

                m_pCurPortOpRequest->dwResult = EC_E_TIMEOUT;
                m_pCurPortOpRequest->oTimeout.Stop();
                m_pCurPortOpRequest = EC_NULL;

                m_eCurPortOpState = m_eReqPortOpState = echcsmpo_idle;
            } break;
        case echcsmpo_stuck:
            {
                /* wait for outstanding commands returned (ProcResult) which is always the case (don't care if frameloss e.g.) 
                 * and return to idle */
                if( m_eCurPOCmdState != echcsmc_pending ) /* if no DGram calls are pending */
                {
                    m_oPortsToChange.ResetPortList();

					m_eReqPortOpState = m_eCurPortOpState = echcsmpo_idle;
					bContinueSM = EC_FALSE;					
                }
            } break;
        case echcsmpo_idle:
            {
                /* from idle, decide which way to go */
                switch( m_eReqPortOpState )
                {
                case echcsmpo_portoperation:
                    m_eCurPortOpState = echcsmpo_start;
                    bContinueSM = EC_TRUE;
                    break;
                case echcsmpo_reqto:
                    m_eCurPortOpState = echcsmpo_reqto_wait;
                    bContinueSM = EC_TRUE;
                    break;
                default: OsDbgAssert(EC_FALSE); m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                }
                
            } break;
        case echcsmpo_start:
            {
                switch( m_eReqPortOpState )
                {
                case echcsmpo_portoperation:
                    m_eCurPortOpState = echcsmpo_readdlcontrol;
	                bContinueSM = EC_TRUE;
                    break;
                case echcsmpo_reqto:
                    m_eCurPortOpState = echcsmpo_reqto_wait;
                    bContinueSM = EC_TRUE;
                    break;
                default: OsDbgAssert(EC_FALSE); m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                }
            } break;
        case echcsmpo_readdlcontrol:
            {
                /* go through list of requests, and send DL Ctrl Reads */
                EC_T_DWORD                  dwListLength    = m_oPortsToChange.m_dwPortEntries;
                EC_T_DWORD                  dwIdx           = 0;
                EC_T_SLAVE_PORT_DESC_EX*    pCurPortOp      = EC_NULL;
                CEcBusSlave*                pBusSlave       = EC_NULL;

                OsDbgAssert(m_eCurPortOpState != echcsmpo_reqto_wait);

                m_dwOutStandingPOCalls = 0;
                for( dwIdx = 0; dwIdx < dwListLength; dwIdx++ )
                {
                    pCurPortOp = &(m_oPortsToChange.m_poPorts[dwIdx]);

                    /* API always is using Fixed Addr */
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(pCurPortOp->m_wSlaveAddress);
                    if( EC_NULL == pBusSlave )
                    {
                        continue;
                    }
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_PORTOP_RDDLCTRLCL, EC_CMD_TYPE_FPRD, pBusSlave->GetFixedAddress(),
                        ECREG_DL_CONTROL, 4, EC_NULL);
                    if( dwRes != EC_E_NOERROR ) 
                    {
                        EC_ERRORMSGC_L(
                            ("CEcHotConnect::ChangePort : error 0x%x in m_pMaster->QueueRegisterCmdReq\n", dwRes) );                                                                 
                        continue;
                    }
                    m_dwOutStandingPOCalls++;
                } /* for( dwIdx = 0; dwIdx < dwListLength; dwIdx++ ) */

                if( 0 == m_dwOutStandingPOCalls )
                {
                    m_eCurPortOpState = echcsmpo_readdlcontrol_done;
                }
                else
                {
                    m_eCurPOCmdState = echcsmc_pending;
                    m_eCurPortOpState = echcsmpo_readdlcontrol_wait;
                }

            } break;
        case echcsmpo_readdlcontrol_wait:
            {
                if( echcsmc_idle == m_eCurPOCmdState )
                {
                    m_eCurPortOpState = echcsmpo_readdlcontrol_done;
                    bContinueSM = EC_TRUE;
                }
                else if( m_eCurPOCmdState != echcsmc_pending )
                {
                    if( echcsmpo_reqto == m_eReqPortOpState )
                    {
                        m_eCurPortOpState = echcsmpo_reqto_wait;
                    }
                    else
                    {
                        m_eCurPortOpState = echcsmpo_readdlcontrol;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case echcsmpo_readdlcontrol_done:
            {
                switch(m_eReqPortOpState )
                {
                case echcsmpo_portoperation: m_eCurPortOpState = echcsmpo_writedlctrl; break;
                case echcsmpo_reqto:         m_eCurPortOpState = echcsmpo_reqto_wait; break;
                default: 
                    OsDbgAssert(EC_FALSE); 
                    m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                    break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_writedlctrl:
            {
                EC_T_DWORD   dwListLength         = m_oPortsToChange.m_dwPortEntries;
                EC_T_DWORD   dwIdx                = 0;
                EC_T_SLAVE_PORT_DESC_EX* pCurPortOp = EC_NULL;                
                CEcBusSlave* pBusSlave            = EC_NULL;
                EC_T_WORD    wPortIdx             = 0;
                EC_T_DWORD   dwPortShift          = 0;
                EC_T_DWORD   dwDLControlFrm       = 0;                
#if (defined INCLUDE_RED_DEVICE)
                EC_T_WORD    wFixedAddrBehind     = INVALID_FIXED_ADDR;
                EC_T_WORD    wPortBehind          = ESC_PORT_INVALID;
                CEcBusSlave* pBusSlaveBehind      = EC_NULL;
                EC_T_DWORD   dwDLControlBehind    = 0xFFFFFFFF;
                EC_T_DWORD   dwDLControlBehindFrm = 0;
#endif /* INCLUDE_RED_DEVICE */
                EC_PF_TIMESTAMP pfSetPortAtLine   = EC_NULL;
                
                m_dwOutStandingPOCalls = 0;
                for (dwIdx = 0; dwIdx < dwListLength; dwIdx++)
                {
                    /* get port operation */
                    pCurPortOp = &(m_oPortsToChange.m_poPorts[dwIdx]);

                    /* get bus slave */
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(pCurPortOp->m_wSlaveAddress);
                    if( EC_NULL == pBusSlave )
                    {
                        continue;
                    }
                    /* handle every port */
                    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
                    {
                        /* prepare shift value */
                        switch (wPortIdx)
                        {
                        case 0: dwPortShift = PORT0_BITOFFSET; break;
                        case 1: dwPortShift = PORT1_BITOFFSET; break;
                        case 2: dwPortShift = PORT2_BITOFFSET; break;
                        case 3: dwPortShift = PORT3_BITOFFSET; break;
                        default: OsDbgAssert(EC_FALSE); break;
                        }
                        switch(pCurPortOp->m_wFlags[wPortIdx] & PORT_MODIFY_MASK)
                        {
                        case PORT_CLOSE_FLAG:
                            if (EC_NULL != pBusSlave)
                            {
                                pCurPortOp->m_dwDLControlValue &= ~(PORT_MODE_MASK<<dwPortShift);
                                pCurPortOp->m_dwDLControlValue |= (((pCurPortOp->m_wFlags[wPortIdx])&(PORT_MODE_MASK))<<dwPortShift);
#if (defined INCLUDE_RED_DEVICE)
                                /* in case of redundancy handle the slave behind the closed ports */
                                if (m_pMaster->IsRedConfigured())
                                {
                                    if (EC_NULL != pBusSlave->m_apBusSlaveChildren[wPortIdx])
                                    {
                                        /* get behind information if existing */
                                        pBusSlaveBehind = (CEcBusSlave*)pBusSlave->m_apBusSlaveChildren[wPortIdx];
                                        for (wPortBehind = 0; wPortBehind < ESC_PORT_COUNT; wPortBehind++)
                                        {
                                            if (pBusSlaveBehind->m_apBusSlaveChildren[wPortBehind] == pBusSlave)
                                            {
                                                break;
                                            }
                                        }
                                        if (ESC_PORT_COUNT <= wPortBehind)
                                        {
                                            wPortBehind = ESC_PORT_INVALID;
                                        }
                                        if ((EC_NULL != pBusSlaveBehind) && (ESC_PORT_INVALID != wPortBehind))
                                        {
                                            /* store behind information */
                                            pCurPortOp->m_wFixedAddrBehind[wPortIdx] = pBusSlaveBehind->GetFixedAddress();
                                            pCurPortOp->m_wPortBehind[wPortIdx]      = wPortBehind;

                                            /* add this port to the closed port list to protect for unexpected open during bus scan */
                                            m_oClosedPorts.AddPort(pBusSlaveBehind->GetFixedAddress(), wPortBehind, PORT_CLOSE_FLAG);

                                            /* prepare command */
                                            dwDLControlBehind = pBusSlaveBehind->GetDlControl() & ~ECM_DLCTRL_LOOP_PORTX_MASK(wPortBehind);
                                            dwDLControlBehind = dwDLControlBehind | (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wPortBehind));
                                        }
                                    }
                                    else
                                    {
                                        if (pBusSlave->IsSignalPortX(wPortIdx))
                                        {
                                            if (0 == pBusSlave->GetBusIndex())
                                            {
                                                pfSetPortAtLine = &ClosePortAtMainLine;
                                            }
                                            else
                                            {
                                                pfSetPortAtLine = &ClosePortAtRedLine;
                                            }
                                        }
                                    }
                                }
#endif /* INCLUDE_RED_DEVICE */
                            }
                            break;
                        case PORT_OPEN_FLAG:
                            pCurPortOp->m_dwDLControlValue &= ~(PORT_MODE_MASK<<dwPortShift);
                            pCurPortOp->m_dwDLControlValue |= (((pCurPortOp->m_wFlags[wPortIdx])&(PORT_MODE_MASK))<<dwPortShift);

#if (defined INCLUDE_RED_DEVICE)
                            /* */
                            if (EC_NULL == pBusSlave->m_apBusSlaveChildren[wPortIdx])
                            {
                                if (pBusSlave->IsSignalPortX(wPortIdx))
                                {
                                    if (0 == pBusSlave->GetBusIndex())
                                    {
                                        pfSetPortAtLine = &OpenPortAtMainLine;
                                    }
                                    else
                                    {
                                        pfSetPortAtLine = &OpenPortAtRedLine;
                                    }
                                }
                            }
                            /* in case of redundancy handle the slave behind the closed ports */
                            if (m_pMaster->IsRedConfigured())
                            {
                            EC_T_SLAVE_PORT_DESC_EX* pClosedPortEntry = m_oClosedPorts.FindEntry(EC_NULL, pCurPortOp->m_wSlaveAddress);

                                if (EC_NULL != pClosedPortEntry)
                                {
                                    wFixedAddrBehind = pClosedPortEntry->m_wFixedAddrBehind[wPortIdx];
                                    wPortBehind      = pClosedPortEntry->m_wPortBehind[wPortIdx];
                                    if ((INVALID_FIXED_ADDR != wFixedAddrBehind) && (ESC_PORT_INVALID != wPortBehind))
                                    {
                                        /* reset behind information */
                                        pCurPortOp->m_wFixedAddrBehind[wPortIdx] = INVALID_FIXED_ADDR;
                                        pCurPortOp->m_wPortBehind[wPortIdx]      = ESC_PORT_INVALID;

                                        /* remove this port to the closed port list */
                                        m_oClosedPorts.RemovePort(wFixedAddrBehind, wPortBehind);

                                        /* get bus slave behind */
                                        pBusSlaveBehind = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(wFixedAddrBehind);
                                        if (EC_NULL != pBusSlaveBehind)
                                        {
                                            /* prepare command */
                                            dwDLControlBehind = pBusSlaveBehind->GetDlControl() & ~ECM_DLCTRL_LOOP_PORTX_MASK(wPortBehind);
                                            dwDLControlBehind = dwDLControlBehind | (ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wPortBehind));
                                        }
                                    }
                                }
                            }
#endif /* INCLUDE_RED_DEVICE */
                            break;
                        default:
                            /* nothing to do */
                            continue;
                        }
                        /* flush current pending frames */
                        m_pMaster->EcatFlushCurrPendingSlaveFrame();

                        /* queue EtherCAT command */
                        if (EC_NULL != pBusSlave)
                        {
                            EC_SET_FRM_DWORD((EC_T_BYTE*)&dwDLControlFrm, pCurPortOp->m_dwDLControlValue);
                            dwRes = m_pMaster->QueueEcatCmdReq(
                                EC_FALSE, EC_NULL, INVOKE_ID_PORTOP_CHANGEPORT, EC_CMD_TYPE_FPWR, pBusSlave->GetFixedAddress(),
                                ECREG_DL_CONTROL, sizeof(EC_T_DWORD), &dwDLControlFrm);
                            if (EC_E_NOERROR == dwRes)
                            {
                                m_dwOutStandingPOCalls++;
                            }
                        }
                        else
                        {
                            m_oClosedPorts.RemovePort(pCurPortOp->m_wSlaveAddress);
                            m_oPortsToChange.RemovePort(pCurPortOp->m_wSlaveAddress);
                        }
#if (defined INCLUDE_RED_DEVICE)
                        if (EC_NULL != pBusSlaveBehind)
                        {
                            EC_SET_FRM_DWORD((EC_T_BYTE*)&dwDLControlBehindFrm, dwDLControlBehind);
                            dwRes = m_pMaster->QueueEcatCmdReq(
                                EC_FALSE, EC_NULL, INVOKE_ID_PORTOP_CHANGEPORT, EC_CMD_TYPE_FPWR, pBusSlaveBehind->GetFixedAddress(),
                                ECREG_DL_CONTROL, sizeof(EC_T_DWORD), &dwDLControlBehindFrm);
                            if (EC_E_NOERROR == dwRes)
                            {
                                m_dwOutStandingPOCalls++;
                            }
                        }
#endif /* INCLUDE_RED_DEVICE */
                    }
                    m_pMaster->EcatFlushCurrPendingSlaveFrameStamp(pfSetPortAtLine, m_pMaster, &s_dwClosePortReserved, 0, 0);
                }
                if( 0 == m_dwOutStandingPOCalls )
                {
                    m_eCurPortOpState = echcsmpo_portoperation;   
                }
                else
                {
                    m_eCurPOCmdState = echcsmc_pending;
                    m_eCurPortOpState = echcsmpo_writedlctrl_wait;
                }
            } break;
        case echcsmpo_writedlctrl_wait:
            {
                /* since the send frame does possible not return we go on with the next step */
                m_eCurPortOpState = echcsmpo_writedlctrl_done;
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_writedlctrl_done:
            {
                /* determine the next step */
                switch(m_eReqPortOpState )
                {
                case echcsmpo_portoperation: m_eCurPortOpState = echcsmpo_checkdlctrl; break;
                case echcsmpo_reqto: m_eCurPortOpState = echcsmpo_reqto_wait; break;
                default: 
                    OsDbgAssert(EC_FALSE); 
                    m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                    break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_checkdlctrl:
            {
                /* go through list of requests, and send DL Ctrl Reads */
                EC_T_DWORD                  dwListLength    = m_oPortsToChange.m_dwPortEntries;
                EC_T_DWORD                  dwIdx           = 0;
                EC_T_SLAVE_PORT_DESC_EX*    pCurPortOp      = EC_NULL;
                CEcBusSlave*                pBusSlave       = EC_NULL;

                OsDbgAssert(m_eCurPortOpState != echcsmpo_reqto_wait);

                m_dwOutStandingPOCalls = 0;
                m_dwNumSucceedPortOperations = 0;
                m_bPortOperationsFailed = EC_FALSE;

                for( dwIdx = 0; dwIdx < dwListLength; dwIdx++ )
                {
                    pCurPortOp = &(m_oPortsToChange.m_poPorts[dwIdx]);

                    /* API always is using Fixed Addr */
                    pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByFixedAddress(pCurPortOp->m_wSlaveAddress);
                    if( EC_NULL == pBusSlave )
                    {
                        continue;
                    }
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_PORTOP_CHECKDLCTRLCL, EC_CMD_TYPE_FPRD, pBusSlave->GetFixedAddress(),
                        ECREG_DL_CONTROL, 4, EC_NULL);
                    if( dwRes != EC_E_NOERROR ) 
                    {
                        EC_ERRORMSGC_L(
                            ("CEcHotConnect::ChangePort : error 0x%x in m_pMaster->QueueRegisterCmdReq\n", dwRes) );                                                                 
                        continue;
                    }

                    m_dwOutStandingPOCalls++;
                } /* for( dwIdx = 0; dwIdx < dwListLength; dwIdx++ ) */

                if( 0 == m_dwOutStandingPOCalls )
                {
                    m_eCurPortOpState = echcsmpo_portoperation;   
                }
                else
                {
                    m_eCurPOCmdState = echcsmc_pending;
                    m_eCurPortOpState = echcsmpo_checkdlctrl_wait;
                }
            } break;
        case echcsmpo_checkdlctrl_wait:
            {
                /* wait for sent frames to be processed */
                if(m_eCurPOCmdState != echcsmc_pending)
                {
                    m_eCurPortOpState = echcsmpo_checkdlctrl_done;
                    bContinueSM = EC_TRUE;
                }
               
            } break;
        case echcsmpo_checkdlctrl_done:
            {
                OsDbgAssert(m_eCurPOCmdState != echcsmc_pending);

                /* handle timeout case */
                if(m_eReqPortOpState == echcsmpo_reqto)
                {
                    m_eCurPortOpState = echcsmpo_reqto_wait;
                    break;
                }

                /* determine the next step */
                if(m_bPortOperationsFailed)
                {
                    /* re-execute the port operation */
                    m_eCurPortOpState = echcsmpo_start;
                    break;
                }
                else if(m_eCurPOCmdState == echcsmc_retry)
                {
                    /* retry the last step */
                    m_eCurPortOpState = echcsmpo_checkdlctrl;
                    break;
                }
                /* determine next state */
                switch(m_eReqPortOpState )
                {
                case echcsmpo_portoperation: m_eCurPortOpState = echcsmpo_BTEnhanceScan; break;
                case echcsmpo_reqto: m_eCurPortOpState = echcsmpo_reqto_wait; break;
                default: 
                    OsDbgAssert(EC_FALSE); 
                    m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                    break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_BTEnhanceScan:
            {
                if (EC_E_BUSY != m_pMaster->GetBusScanResult())
                {
                    dwRes = m_pMaster->StartBTTopologyChange();
                    if( EC_E_NOERROR == dwRes )
                    {
                        m_eCurPortOpState = echcsmpo_BTEnhanceScan_wait;
                    }
                    else
                    {
                        m_eCurPortOpState = echcsmpo_BTEnhanceScan_done;
                    }
                }
                else
                {
                    m_eCurPortOpState = echcsmpo_BTEnhanceScan_done;
                }
            } break;
        case echcsmpo_BTEnhanceScan_wait:
            {
                dwResSB = m_pMaster->WaitForBTStateMachine();
                if ((EC_E_BUSY == dwResSB) || (EC_E_NOTREADY == dwResSB))
                {
                    break;
                }
                m_pMaster->ReleaseBTStateMachine(dwResSB);
                
                m_eCurPortOpState = echcsmpo_BTEnhanceScan_done;
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_BTEnhanceScan_done:
            {
                /* determine next state */
                switch(m_eReqPortOpState )
                {
                case echcsmpo_portoperation: m_eCurPortOpState = echcsmpo_portoperation; break;
                case echcsmpo_reqto: m_eCurPortOpState = echcsmpo_reqto_wait; break;
                default: 
                    OsDbgAssert(EC_FALSE); 
                    m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                    break;
                }
                bContinueSM = EC_TRUE;
            } break;
        case echcsmpo_portoperation: 
            {
                m_pCurPortOpRequest->dwResult = EC_E_NOERROR;
                m_pCurPortOpRequest->oTimeout.Stop();
                m_pCurPortOpRequest = EC_NULL;
                m_eCurPortOpState = m_eReqPortOpState = echcsmpo_idle;
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_eCurPortOpState = m_eReqPortOpState = echcsmpo_stuck;
                bContinueSM = EC_FALSE;
            } /* default */
        } /* switch m_eCurPortOpState */
        
        /* current request did time out */
        if( EC_NULL != m_pCurPortOpRequest && m_pCurPortOpRequest->oTimeout.IsElapsed() )
        {
            m_eReqPortOpState = echcsmpo_reqto;
        }

#ifdef INCLUDE_LOG_MESSAGES
        if( (m_eCurPortOpState != eOldState) && byDbgLvl )
        {
            m_poEcDevice->LinkDbgMsg( 
                byDbgLvl, ETHTYP0_HCSM, ETHTYP1_HCSM_SC,
                "HCPortOperationStateMachine: %s->%s (%s)\n", EHCPOSTATESText(eOldState), EHCPOSTATESText(m_eCurPortOpState), EHCPOSTATESText(m_eReqPortOpState)
                );
        }
#endif
    } /* while( bContinueSM)  */
    
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Enqueue new Request for Port Operation Instance.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcHotConnect::RequestPortOpState(
    ECHCSM_T_EPORTOPERATION eState          /**< [in]   Desired state */
   ,EC_T_DWORD              dwTimeout       /**< [in]   Request Timeout */
   ,ECHCSMPO_REQ**          pHandle         /**< [out]  Handle to Request if EC_E_NOERROR is returned */
                                            )
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
    
    if( EC_NULL == pHandle )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    switch(eState)
    {
    case echcsmpo_portoperation: break;
    default:
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( (m_oPortOpRequest[0].bUsed) && (m_oPortOpRequest[1].bUsed) )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    if( EC_NULL == m_pPortOpRequest )
    {
        if( m_oPortOpRequest[0].bUsed )
        {
            (*pHandle) = &(m_oPortOpRequest[1]);
        }
        else
        {
            (*pHandle) = &(m_oPortOpRequest[0]);
        }
        (*pHandle)->bUsed       = EC_TRUE;
        (*pHandle)->eState      = eState;
        (*pHandle)->dwResult    = EC_E_BUSY;
        (*pHandle)->oTimeout.Start(dwTimeout, m_pMaster->GetMsecCounterPtr());
         m_pPortOpRequest = (*pHandle);
        
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        dwRetVal = EC_E_BUSY;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Poll success of requested state.

\return error code.
*/
EC_T_DWORD CEcHotConnect::PollPortOpState(
    ECHCSMPO_REQ*     pHandle     /**< [in]   desired request to wait for */
                                         )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if( m_pPortOpRequest == pHandle )
    {
        /* request not fetched for execution by state machine */
        dwRetVal = EC_E_NOTREADY;
        goto Exit;
    }
    
    if( m_pCurPortOpRequest == pHandle )
    {
        /* request fetched, result is updated by state machine */
        dwRetVal = m_pCurPortOpRequest->dwResult;
    }
    else
    {
        /* already out of SM, waits for result read */
        dwRetVal = EC_E_NOERROR;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Release handle after reading result code.

\return error code.
*/
EC_T_DWORD CEcHotConnect::ReleasePortOpReq(
    ECHCSMPO_REQ*   pHandle     /**< [in]   desired request to release */
                                          )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if( (pHandle != &(m_oPortOpRequest[0])) && (pHandle != &(m_oPortOpRequest[1])) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    /* check if a port operations is queued but not yet started */
    if( pHandle == m_pPortOpRequest )
    {
        m_pPortOpRequest = EC_NULL;
    }

    /* check if a port operation is currently running */
    if( EC_NULL != m_pCurPortOpRequest )
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    /* free queued handle */
    pHandle->bUsed = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /* INCLUDE_PORT_OPERATION */

/***************************************************************************************************/
/**
\brief  Return Enum Text for HC StateMachine.

\return State Text.
*/
EC_T_CHAR* CEcHotConnect::EHCSTATESText(
    ECHCSM_T_ESTATEMACHINE  eState  /**< [in]   Statea to retrieve text to */
                                       )
{
    EC_T_CHAR* pszRet = (EC_T_CHAR*)"Unknown state";

#ifdef INCLUDE_LOG_MESSAGES
    switch( eState )
    {
    case echcsm_idle: pszRet = (EC_T_CHAR*)"echcsm_idle"; break;
    case echcsm_start: pszRet = (EC_T_CHAR*)"echcsm_start"; break;
    case echcsm_restart: pszRet = (EC_T_CHAR*)"echcsm_restart"; break;

    case echcsm_BusScan:                pszRet = (EC_T_CHAR*)"echcsm_BusScan"; break;
    case echcsm_BusScan_wait:           pszRet = (EC_T_CHAR*)"echcsm_BusScan_wait"; break;
    case echcsm_BusScan_done:           pszRet = (EC_T_CHAR*)"echcsm_BusScan_done"; break;
    case echcsm_GroupInitState:         pszRet = (EC_T_CHAR*)"echcsm_GroupInitState"; break;
    case echcsm_GroupInitState_wait:    pszRet = (EC_T_CHAR*)"echcsm_GroupInitState_wait"; break;
    case echcsm_GroupInitState_done:    pszRet = (EC_T_CHAR*)"echcsm_GroupInitState_done"; break;
    case echcsm_GroupPreopState:        pszRet = (EC_T_CHAR*)"echcsm_GroupPreopState"; break;
    case echcsm_GroupPreopState_wait:   pszRet = (EC_T_CHAR*)"echcsm_GroupPreopState_wait"; break;
    case echcsm_GroupPreopState_done:   pszRet = (EC_T_CHAR*)"echcsm_GroupPreopState_done"; break;
    case echcsm_DCActivation:           pszRet = (EC_T_CHAR*)"echcsm_DCActivation"; break;
    case echcsm_DCActivation_wait:      pszRet = (EC_T_CHAR*)"echcsm_DCActivation_wait"; break;
    case echcsm_DCActivation_done:      pszRet = (EC_T_CHAR*)"echcsm_DCActivation_done"; break;
    case echcsm_GroupAdded:             pszRet = (EC_T_CHAR*)"echcsm_GroupAdded"; break;
    case echcsm_GroupMasterState:       pszRet = (EC_T_CHAR*)"echcsm_GroupMasterState"; break;
    case echcsm_GroupMasterState_wait:  pszRet = (EC_T_CHAR*)"echcsm_GroupMasterState_wait"; break;
    case echcsm_GroupMasterState_done:  pszRet = (EC_T_CHAR*)"echcsm_GroupMasterState_done"; break;
    case echcsm_TopoChangeManual:       pszRet = (EC_T_CHAR*)"echcsm_TopoChangeManual"; break;
    case echcsm_TopoChangeManualContinue: pszRet = (EC_T_CHAR*)"echcsm_TopoChangeManualContinue"; break;
    case echcsm_TopoChangeAutomatic:    pszRet = (EC_T_CHAR*)"echcsm_TopoChangeAutomatic"; break;

    case echcsm_reqto_wait:             pszRet = (EC_T_CHAR*)"echcsm_reqto_wait"; break;
    case echcsm_stuck:                  pszRet = (EC_T_CHAR*)"echcsm_stuck"; break;
    case echcsm_unknown:                pszRet = (EC_T_CHAR*)"echcsm_unknown"; break;
    default:                            OsSnprintf(pszRet, (EC_T_INT)(OsStrlen(pszRet)+1),"0x%lx", eState); break;
    }
#else
    EC_UNREFPARM(eState);
#endif
    return pszRet;
}

#if (defined INCLUDE_PORT_OPERATION)
/***************************************************************************************************/
/**
\brief  Return Enum Text for HC StateMachine.

\return State Text.
*/
EC_T_CHAR* CEcHotConnect::EHCPOSTATESText(
    ECHCSM_T_EPORTOPERATION     eState  /**< [in]   State to retrieve text to */
                                         )
{
    EC_T_CHAR* pszRet = (EC_T_CHAR*)"Unknown state";

#ifdef INCLUDE_LOG_MESSAGES
    switch( eState )
    {
    case echcsmpo_unknown:              pszRet = (EC_T_CHAR*)"echcsmpo_unknown"; break;
    case echcsmpo_idle:                 pszRet = (EC_T_CHAR*)"echcsmpo_idle"; break;
    case echcsmpo_start:                pszRet = (EC_T_CHAR*)"echcsmpo_start"; break;
    case echcsmpo_readdlcontrol:        pszRet = (EC_T_CHAR*)"echcsmpo_readdlcontrol"; break;
    case echcsmpo_readdlcontrol_wait:   pszRet = (EC_T_CHAR*)"echcsmpo_readdlcontrol_wait"; break;
    case echcsmpo_readdlcontrol_done:   pszRet = (EC_T_CHAR*)"echcsmpo_readdlcontrol_done"; break;
    case echcsmpo_writedlctrl:          pszRet = (EC_T_CHAR*)"echcsmpo_writedlctrl"; break;
    case echcsmpo_writedlctrl_wait:     pszRet = (EC_T_CHAR*)"echcsmpo_writedlctrl_wait"; break;
    case echcsmpo_writedlctrl_done:     pszRet = (EC_T_CHAR*)"echcsmpo_writedlctrl_done"; break;
    case echcsmpo_checkdlctrl:          pszRet = (EC_T_CHAR*)"echcsmpo_checkdlctrl"; break;
    case echcsmpo_checkdlctrl_wait:     pszRet = (EC_T_CHAR*)"echcsmpo_checkdlctrl_wait"; break;
    case echcsmpo_checkdlctrl_done:     pszRet = (EC_T_CHAR*)"echcsmpo_checkdlctrl_done"; break;
    case echcsmpo_BTEnhanceScan:        pszRet = (EC_T_CHAR*)"echcsmpo_BTEnhanceScan"; break;
    case echcsmpo_BTEnhanceScan_wait:   pszRet = (EC_T_CHAR*)"echcsmpo_BTEnhanceScan_wait"; break;
    case echcsmpo_BTEnhanceScan_done:   pszRet = (EC_T_CHAR*)"echcsmpo_BTEnhanceScan_done"; break;
    case echcsmpo_portoperation:        pszRet = (EC_T_CHAR*)"echcsmpo_portoperation"; break;
    case echcsmpo_reqto_wait:           pszRet = (EC_T_CHAR*)"echcsmpo_reqto_wait"; break;
    case echcsmpo_reqto:                pszRet = (EC_T_CHAR*)"echcsmpo_reqto"; break;
    default:                            OsSnprintf(pszRet, (EC_T_INT)(OsStrlen(pszRet)+1), "0x%lx", eState); break;
    }
#else
    EC_UNREFPARM(eState);
#endif
    return pszRet;
}
#endif /* INCLUDE_PORT_OPERATION */

/*-/CECHotConnect------------------------------------------------------------*/


/*-END OF SOURCE FILE--------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * EcBusTopology.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Cyril Eyssautier
 * Description              Scan and Store topology of connected EtherCAT Bus
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcCommon.h"
#include "EcCommonPrivate.h"

#include "EcInterfaceCommon.h"
#include "EthernetServices.h"
#include "EcBusTopology.h"

#include "EcMasterCommon.h"
#include "EcInterface.h"
#include "EcEthSwitch.h"
#include "EcIoImage.h"
#include "EcFiFo.h"
#include "EcDevice.h"
#include "EcMaster.h"
#include "EcSlave.h"
#include "EcInvokeId.h"

#if( defined INCLUDE_DC_SUPPORT )
#include "EcDistributedClocks.h"
#endif

#include "EcText.h"

/*-DEFINES-------------------------------------------------------------------*/

/* debug message type IDs for link layer debug messages */
#define ETHTYPE1        ((EC_T_BYTE)0xFF)
#define ETHTYPE0        ((EC_T_BYTE)0x00)
#define STATE_CHNG      ((EC_T_BYTE)0x00)
#define EEP_STATE_CHNG  ((EC_T_BYTE)0x01)
#define FIXED_ADDR      ((EC_T_BYTE)0x02)
#define TOPO_CHNG       ((EC_T_BYTE)0x03)

#define IDENTIFY_BUSSLAVE_TIMEOUT   5000

/*-MACROS--------------------------------------------------------------------*/
#undef  EcLinkErrorMsg
#define EcLinkErrorMsg m_poEthDevice->LinkErrorMsg
#undef  EcLinkDbgMsg
#define EcLinkDbgMsg   m_poEthDevice->LinkDbgMsg

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Allocate Memory for Memory Management in BT Instance to avoid OsMalloc during runtime.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcBusTopology::CreateBusSlavesPool(EC_T_DWORD dwMaxBusSlaves, EC_T_BOOL bGenerateENI)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_DWORD dwIdx = 0;

    /* create bus slaves pool */
#if (defined INCLUDE_GEN_OP_ENI)
    if (bGenerateENI)
    {
    EC_T_DWORD dwBusSlavesMemSize = dwMaxBusSlaves * 0x400;

        m_poBusSlavesGenEniPool = EC_NEW(CEcBusSlaveGenEni[dwMaxBusSlaves]);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_poBusSlavesGenEniPool", m_poBusSlavesGenEniPool, sizeof(CEcBusSlaveGenEni)*dwMaxBusSlaves);
        if (EC_NULL == m_poBusSlavesGenEniPool)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        /* alloc memory for BusSlave memory */
        m_pbyBusSlavesMem = (EC_T_BYTE*)OsMalloc(dwBusSlavesMemSize);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_pbyBusSlavesMem", m_pbyBusSlavesMem, dwBusSlavesMemSize);
        if (EC_NULL == m_pbyBusSlavesMem)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        m_dwBusSlavesMemSize = dwBusSlavesMemSize;
        m_pbyBusSlavesMemCur = m_pbyBusSlavesMem;
    }
    else
#else
    EC_UNREFPARM(bGenerateENI);
#endif /* INCLUDE_GEN_OP_ENI */
    {
        m_poBusSlavesPool = EC_NEW(CEcBusSlave[dwMaxBusSlaves]);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_poBusSlavesPool", m_poBusSlavesPool, sizeof(CEcBusSlave)*dwMaxBusSlaves);
        if (EC_NULL == m_poBusSlavesPool)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }
    /* store current pool size */
    m_dwBusSlavesPoolSize = dwMaxBusSlaves;

    /* create hash tables */
    m_pBusSlavesByIndex = EC_NEW(CHashTableDyn)<EC_T_DWORD, CEcBusSlave*>(EC_MEMTRACE_CORE_MASTER, dwMaxBusSlaves, GetMemLog());
    if (EC_NULL == m_pBusSlavesByIndex)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_pBusSlavesByIndex", m_pBusSlavesByIndex, sizeof(CHashTableDyn<EC_T_DWORD, CEcBusSlave*>));

    m_pBusSlavesByFixedAddress = EC_NEW(CHashTableDyn)<EC_T_WORD, CEcBusSlave*>(EC_MEMTRACE_CORE_MASTER, dwMaxBusSlaves, GetMemLog());
    if (EC_NULL == m_pBusSlavesByFixedAddress)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_pBusSlavesByFixedAddress", m_pBusSlavesByFixedAddress, sizeof(CHashTableDyn<EC_T_WORD, CEcBusSlave*>));

    /* initialize free list */
    for (dwIdx = 0; dwIdx < m_dwBusSlavesPoolSize; dwIdx++)
    {
#if (defined INCLUDE_GEN_OP_ENI)
    CEcBusSlave* pBusSlave = (bGenerateENI)?&m_poBusSlavesGenEniPool[dwIdx]:&m_poBusSlavesPool[dwIdx]; /* CEcBusSlaveGenEni and CEcBusSlave have different size! */
#else
    CEcBusSlave* pBusSlave = &m_poBusSlavesPool[dwIdx];
#endif
        pBusSlave->SetNext(m_poBusSlavesPoolFree);
        m_poBusSlavesPoolFree = pBusSlave;
    }

    m_bBusSlavesPoolReady = EC_TRUE;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "CreateBusSlavesPool(%d, %d) returns 0x%04X\n", dwMaxBusSlaves, bGenerateENI, dwRetVal));
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Delete bus slaves pool

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcBusTopology::DeleteBusSlavesPool(EC_T_VOID)
{
    m_bBusSlavesPoolReady = EC_FALSE;

    /* remove bus slaves */
    RemoveBusSlaves();

    /* delete hash tables */
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_pBusSlavesByFixedAddress", m_pBusSlavesByFixedAddress, sizeof(CHashTableDyn<EC_T_WORD, CEcBusSlave*>));
    SafeDelete(m_pBusSlavesByFixedAddress);
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_pBusSlavesByIndex", m_pBusSlavesByIndex, sizeof(CHashTableDyn<EC_T_DWORD, CEcBusSlave*>));
    SafeDelete(m_pBusSlavesByIndex);

    /* delete bus slaves pool */
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_poBusSlavesPool", m_poBusSlavesPool, sizeof(CEcBusSlave)*m_dwBusSlavesPoolSize);
    SafeDeleteArray(m_poBusSlavesPool);
#if (defined INCLUDE_GEN_OP_ENI)
    if (EC_NULL != m_poBusSlavesGenEniPool)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_poBusSlavesGenEniPool", m_poBusSlavesGenEniPool, sizeof(CEcBusSlaveGenEni)*m_dwBusSlavesPoolSize);
        SafeDeleteArray(m_poBusSlavesGenEniPool);
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcBusTopology:m_pbyBusSlavesMem", m_pbyBusSlavesMem, m_dwBusSlavesMemSize);
    SafeOsFree(m_pbyBusSlavesMem);
    m_dwBusSlavesMemSize = 0;
    m_pbyBusSlavesMemCur = EC_NULL;
#endif
    m_dwBusSlavesPoolSize = 0;
    m_poBusSlavesPoolFree = EC_NULL;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Recreate bus slaves pool

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::RecreateBusSlavesPool(EC_T_DWORD dwMaxBusSlaves, EC_T_BOOL bGenerateENI)
{
    DeleteBusSlavesPool();
    return CreateBusSlavesPool(dwMaxBusSlaves, bGenerateENI);
}

/*-class CEcBusSlave---------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Get BusSlave from pool and initialize it (like new without malloc)

\return Pointer to BusSlave object or EC_NULL on error
*/
CEcBusSlave* CEcBusTopology::NewBusSlave(EC_T_VOID)
{
CEcBusSlave* pBusSlave = m_poBusSlavesPoolFree;

    if (EC_NULL != pBusSlave)
    {
        m_poBusSlavesPoolFree = pBusSlave->GetNext();

        /* initialize bus slave */
        pBusSlave->InitInstance();
    }
    return pBusSlave;
}

/***************************************************************************************************/
/**
\brief  Alloc memory for bus slave information
*/
EC_T_BYTE* CEcBusTopology::AllocBusSlaveMem(EC_T_DWORD dwSize)
{
EC_T_BYTE* pbyBusSlavesMemCur  = m_pbyBusSlavesMemCur;
EC_T_BYTE* pbyBusSlavesMemNext =   pbyBusSlavesMemCur + ((dwSize+3)/4)*4; /* round up to DWORD to protect against missalignment */
EC_T_BYTE* pbyBusSlavesMemLast = m_pbyBusSlavesMem    + m_dwBusSlavesMemSize;

    if (pbyBusSlavesMemNext < pbyBusSlavesMemLast)
    {
        m_pbyBusSlavesMemCur = pbyBusSlavesMemNext;
        OsMemset(m_pbyBusSlavesMemCur, 0, dwSize);
        return pbyBusSlavesMemCur;
    }
    else
    {
        return EC_NULL;
    }
}

/***************************************************************************************************/
/**
\brief  Return BusSlave to pool
*/
EC_T_VOID CEcBusTopology::DeleteBusSlave(CEcBusSlave* pBusSlave)
{
    pBusSlave->SetNext(m_poBusSlavesPoolFree);
    m_poBusSlavesPoolFree = pBusSlave;
}

/***************************************************************************************************/
/**
\brief  Delete bus slaves list.
*/
EC_T_VOID CEcBusTopology::DeleteBusSlaves()
{
CEcBusSlave* pBusSlaveNext = EC_NULL;

    OsLock(m_poBusSlaveListLock);
    while (EC_NULL != m_pBusSlaveRoot)
    {
        pBusSlaveNext = m_pBusSlaveRoot->GetNext();

        /* free memory */
        DeleteBusSlave(m_pBusSlaveRoot);

        m_pBusSlaveRoot = pBusSlaveNext;
    }
    m_dwBusSlaves   = 0;
    m_dwBusSlavesDC = 0;

    /* release bus slaves memory */
    m_pbyBusSlavesMemCur = m_pbyBusSlavesMem;
    OsUnlock(m_poBusSlaveListLock);
}

/***************************************************************************************************/
/**
\brief  Delete bus slaves without config slaves.
*
*/
EC_T_VOID CEcBusTopology::DeleteUnexpectedBusSlaves(EC_T_VOID)
{
    EC_T_DWORD dwBusIndex = 0;
    CEcBusSlave* poBusSlaveCur = GetBusSlaveList();
    CEcBusSlave* poBusSlaveNext = EC_NULL;

    OsLock(m_poBusSlaveListLock);
    while (EC_NULL != poBusSlaveCur)
    {
        poBusSlaveNext = poBusSlaveCur->GetNext();
        m_pBusSlavesByIndex->Remove(poBusSlaveCur->GetBusIndex());

        if (EC_NULL == poBusSlaveCur->GetCfgSlave())
        {
            m_pBusSlavesByFixedAddress->Remove(poBusSlaveCur->GetFixedAddress());
            RemoveBusSlaveFromList(poBusSlaveCur);
            DeleteBusSlave(poBusSlaveCur);
        }
        else
        {
            m_pBusSlavesByIndex->Add(dwBusIndex++, poBusSlaveCur);
        }
        
        poBusSlaveCur = poBusSlaveNext;        
    }
    OsUnlock(m_poBusSlaveListLock);
}

/***************************************************************************************************/
/**
\brief  Default constructor.
*/
EC_T_VOID CEcBusSlave::InitInstance(EC_T_VOID)
{
    /* internal management */
    m_pNext                         = EC_NULL;
    m_pPrev                         = EC_NULL;

    /* values of ESC regiters */
    m_byType                        = 0;
    m_byRevision                    = 0;
    m_wBuild                        = 0;
    m_bySupportedFmmuEntities       = 0;
    m_bySupportedSyncManchannels    = 0;
    m_byRAMSize                     = 0;
    m_byPortDescriptor              = 0;
    m_wFeaturesSupported            = 0;
    m_wStationFixedAddress          = 0;
    m_wStationAliasAddress          = 0;
    m_dwDlControl                   = 0;
    m_wDlStatus                     = 0;
    m_wAlStatus                     = 0;
    m_wAlStatusCode                 = DEVICE_STATUSCODE_NOERROR;
    m_byPDI                         = 0;
    m_wEcatEventMask                = 0;
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
    ResetRecvTimes();
#endif
#if (defined INCLUDE_DC_SUPPORT)
    m_qwSystemTimeOffset            = 0;
    m_dwPropagDelay                 = 0;
    m_dwSystemTimeDifference        = 0;
#endif

    /* values from EEPROM */
    m_dwVendorId                    = (EC_T_DWORD)-1;
    m_dwProductCode                 = (EC_T_DWORD)-1;
    m_dwRevisionNumber              = (EC_T_DWORD)-1;
    m_dwSerialNumber                = (EC_T_DWORD)-1;
    m_wBootRcvMbxOffset             = 0;
    m_wBootRcvMbxSize               = 0;
    m_wBootSndMbxOffset             = 0;
    m_wBootSndMbxSize               = 0;
    m_wStdRcvMbxOffset              = 0;
    m_wStdRcvMbxSize                = 0;
    m_wStdSndMbxOffset              = 0;
    m_wStdSndMbxSize                = 0;
    m_wMbxSupportedProtocols        = 0;

    /* index on the network */
    m_dwBusIndex                    = INVALID_BUS_INDEX;

    /* flag field */
    m_wFlagField                    = ECBT_SLAVEFLAG_ISNEW | ECBT_SLAVEFLAG_RECVTIMES_SUPPORTED | ECBT_SLAVEFLAG_BOOTSTRAPSUPPORTED;

    /* topology information */
    OsMemset(&m_awLinkPort[0], 0, sizeof(m_awLinkPort));
    OsMemset(&m_apBusSlaveChildren[0], 0, sizeof(m_apBusSlaveChildren));
    m_wInputPort                    = ESC_PORT_A;
    m_wParentPort                   = ESC_PORT_INVALID;
    m_wPortAtParent                 = ESC_PORT_INVALID;
    m_pBusSlaveBranchingLast        = EC_NULL;
    m_wPortBranchingLast            = 0;
    m_byConnectedPortCnt            = 0;

    /* slave identififcation */
    m_wIdentificationAdo            = 0;
    m_wIdentificationValue          = 0;
    m_wIdentificationPrevAdo        = 0;
    m_pCfgSlaveIdentMisMatch        = EC_NULL;
    m_pBusSlaveDuplicate            = EC_NULL;

    /* link to config slave */
    m_pCfgSlave                     = EC_NULL;

#if (defined INCLUDE_DC_SUPPORT)
    SetDcInitialized(EC_FALSE);
    m_dwProcessingUnitDelay         = 10;
    m_dwSlaveDelay                  = 0;
    OsMemset(&m_adwPortDelay[0], 0, sizeof(m_adwPortDelay));
    OsMemset(&m_adwLineDelay[0], 0, sizeof(m_adwLineDelay));
#endif /* defined INCLUDE_DC_SUPPORT */

    /* bus scan variables */
    m_dwScanStatus                  = EC_E_NOTREADY;
    m_wSlaveTmpFixAddr              = 0;
    OsMemset(&m_awLinkPortBackup[0], 0, sizeof(m_awLinkPortBackup));
}

/***************************************************************************************************/
/**
\brief  Compare DL info against bus slave
*/
EC_T_BOOL CEcBusSlave::AreDlInfoEqual(EC_T_BYTE* pbyVal)
{
    if (    (m_byType     != pbyVal[0])
        ||  (m_byRevision != pbyVal[1])
        ||  (m_wBuild     != EC_MAKEWORD(pbyVal[3], pbyVal[2]))
        ||  (m_bySupportedFmmuEntities != pbyVal[4])
        ||  (m_bySupportedSyncManchannels != pbyVal[5])
        ||  (m_byRAMSize  != pbyVal[6])
        || ((m_byPortDescriptor != pbyVal[7]) && (0 != pbyVal[7])) /* by some slaves the port descriptor register returns always 0 */
        ||  (m_wFeaturesSupported != EC_MAKEWORD(pbyVal[9], pbyVal[8]))
        ||  (m_wStationAliasAddress != EC_MAKEWORD(pbyVal[19], pbyVal[18]))
        )
    {
        return EC_FALSE;
    }
    return EC_TRUE;
}

/***************************************************************************************************/
/**
\brief  Set DL Info
*/
EC_T_VOID CEcBusSlave::SetDlInfo(EC_T_BYTE* pbyVal)
{
    /* Type of EtherCAT controller (0x0000) */
    m_byType                            = pbyVal[0x0000];
    /* Revision of EtherCAT controller. IP Core: major version X (0x0001) */
    m_byRevision                        = pbyVal[0x0001];
    /* Actual build of EtherCAT controller. IP Core: [7:4] = minor version Y, [3:0] = maintenance version Z (0x0002) */
    m_wBuild                            = EC_GET_FRM_WORD(&(pbyVal[0x0002]));
    /* Number of supported FMMU channels (or entities) of the EtherCAT Slave Controller. (0x0004) */
    m_bySupportedFmmuEntities           = pbyVal[0x0004];
    /* Number of supported SyncManager channels (or entities) of the EtherCAT Slave Controller (0x0005) */
    m_bySupportedSyncManchannels        = pbyVal[0x0005];
    /* Process Data RAM size supported by the EtherCAT Slave Controller in KByte (0x0006) */
    m_byRAMSize                         = pbyVal[0x0006];

    /* by some slaves the port descriptor register returns always 0 */
    /* in this case the physics description from the ENI file should be used */
    if ((0 != pbyVal[0x0007]) || (EC_NULL == m_pCfgSlave))
    {
        /* Port configuration: 00: Not implemented 01: Not configured (ESI EEPROM) 10: EBUS 11: MII / RMII (0x0007)*/
        SetPortDescriptor(pbyVal[0x0007]);
    }
    /* ESC Features supported (0x0008) */
    m_wFeaturesSupported                = EC_GET_FRM_WORD(&(pbyVal[0x0008]));

    /* Configured Station Address (0x0010) */
    m_wStationFixedAddress = EC_GET_FRM_WORD(&(pbyVal[0x0010]));
    /* Configured Station Alias (0x0012) */
    m_wStationAliasAddress = EC_GET_FRM_WORD(&(pbyVal[0x0012]));

    /* check if Ecat Event supported */
    switch (GetESCType())
    {
    case ESCTYPE_BKHF_ELOLD:
    case ESCTYPE_ESC10_20:
    case 5:
        SetEcatEventSupported(EC_FALSE);
        break;
    default:
        SetEcatEventSupported(EC_TRUE);
        break;
    }
    /* check if autoclose supported */
    switch (GetESCType())
    {
    case ESCTYPE_NETX100_500:
    case ESCTYPE_NETX50:
    case ESCTYPE_NETX5:
        if ((1 <= m_byRevision) && (8 <= m_wBuild))
        {
            /* Hilscher supports auto close properly first by build 8 */
            SetAutoCloseSupported(EC_TRUE);
        }
        else
        {
            SetAutoCloseSupported(EC_FALSE);
        }
        break;
    case ESCTYPE_NETX51_52:
    default:
        SetAutoCloseSupported(EC_TRUE);
        break;
    }
}

#if (defined INCLUDE_DC_SUPPORT)
/***************************************************************************************************/
/**
\brief  DC is enabled if DC sync unit is supported and DC must be initialized
*/
EC_T_BOOL CEcBusSlave::IsDcEnabled(EC_T_VOID)
{
    return (IsDcUnitSupported() && (EC_NULL != m_pCfgSlave) && m_pCfgSlave->IsDcInitializationRequired());
}
#endif /* INCLUDE_DC_SUPPORT */

/***************************************************************************************************/
/**
\brief  Backup Slave Information concerning Ports for DL status IRQ processing.
*/
EC_T_VOID CEcBusSlave::BackupPortInfos(EC_T_VOID)
{
EC_T_WORD wPortIdx = 0;

    if ((0 == m_awLinkPort[0]) && (0 == m_awLinkPort[1]) && (0 == m_awLinkPort[2]) && (0 == m_awLinkPort[3]))
    {
        /* port state not read yet, new slave */
        return;
    }
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
        m_awLinkPortBackup[wPortIdx] = (EC_T_WORD)(m_awLinkPort[wPortIdx] | ECBT_LINKPORT_BACKUPVALID);
    }
}

/***************************************************************************************************/
/**
\brief  Detect port status changes
*/
EC_T_BOOL CEcBusSlave::DetectPortChanges(EC_T_VOID)
{
EC_T_BOOL bPortChangesDetected = EC_FALSE;

    OsDbgAssert(IsBackupValid());

    for (EC_T_WORD wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
        if (!IsSlaveCfgPortX(wPortIdx))
        {
            continue;
        }
        if ((IsLoopPortX(wPortIdx)   != IsBuLoopPortX(wPortIdx))
         || (IsLinkPortX(wPortIdx)   != IsBuLinkPortX(wPortIdx))
         || (IsSignalPortX(wPortIdx) != IsBuSignalPortX(wPortIdx))
        )
        {
            bPortChangesDetected = EC_TRUE;
        }
    }
    return bPortChangesDetected;
}

/***************************************************************************************************/
/**
\brief  Detect new connection
*/
EC_T_BOOL CEcBusSlave::DetectNewPortConnection(CEcMaster* pMaster)
{
EC_T_BOOL bNewPortConnectionDetected = EC_FALSE;
#if (defined INCLUDE_PORT_OPERATION)
    CEcHotConnect* pHotConnect = &pMaster->m_oHotConnect;
#else
    EC_UNREFPARM(pMaster);
#endif
    for (EC_T_WORD wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
        if (!IsSlaveCfgPortX(wPortIdx))
        {
            continue;
        }
		if (IsLoopPortX(wPortIdx) && IsSignalPortX(wPortIdx) && !IsFramelossAfterSlave(wPortIdx))
		{
#if (defined INCLUDE_PORT_OPERATION)
            if (!pHotConnect->IsPortClosed(GetFixedAddress(), wPortIdx))
#endif
	        {
				bNewPortConnectionDetected = EC_TRUE;
		    }
        }
    }
    return bNewPortConnectionDetected;
}

/***************************************************************************************************/
/**
\brief  Loop Control State Port X.

\return Loop control value.
*/
EC_T_BYTE CEcBusSlave::PortLoopControlX(
    EC_T_WORD   wPort   /**< [in]   Port identifier */
                                                       )
{
    EC_T_BYTE   byPortLoopControl   = 0;

    /* Register ESC DL Control (0x0100) */
    byPortLoopControl = (EC_T_BYTE)((m_dwDlControl>>ECM_DLCTRL_LOOP_PORTX_SHIFT(wPort))&ECM_DLCTRL_LOOP_PORTS_MASK);

    return byPortLoopControl;
}

#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
/***************************************************************************************************/
/**
\brief  Reset receive times
*/
EC_T_VOID CEcBusSlave::ResetRecvTimes( EC_T_VOID )
{
    for (EC_T_WORD wPort = 0; wPort < ESC_PORT_COUNT; wPort++)
    {
        SetRecvTimeMissingPortX(wPort, EC_FALSE);
    }
    SetRecvTimesCoherent(EC_FALSE);
    OsMemset(&m_adwRecvTime[0], 0, sizeof(m_adwRecvTime));
#if (defined INCLUDE_DC_SUPPORT)
    m_qwRecvTimeProcessingUnit = 0;
    m_dwSlaveDelay = 0;
    OsMemset(&m_adwPortDelay[0], 0, sizeof(m_adwPortDelay));
#endif
}

/***************************************************************************************************/
/**
\brief  Set receive times
*/
EC_T_VOID CEcBusSlave::SetRecvTimes(
    EC_T_PBYTE pbyVal,               /**< [in] value from incoming packet */
    CEcMaster* poMaster
)
{
    EC_T_WORD  wPort = 0;
    EC_T_DWORD dwMaxDelay = 0;
    EC_T_BOOL  bRecvTimesCoherent = EC_TRUE;
    EC_T_UINT64 qwRecvTimeProcessingUnit = EC_GET_FRM_QWORD((pbyVal + 0x18));

    /* determine max delay value to validate port receive times, e.g.  */
    /* in case of junction redundancy, time information is not latched */
    /* at all ports see JUNCTIONRED in EcBusTopology_tests             */
    dwMaxDelay = poMaster->GetBusCycleTimeUsec() * 1000;
    if (0 == dwMaxDelay)
    {
        dwMaxDelay = 1000000;
    }

    for (wPort = 0; wPort < ESC_PORT_COUNT; wPort++)
    {
        if (IsSlaveConPortX(wPort))
        {
            m_adwRecvTime[wPort] = EC_GET_FRM_DWORD((pbyVal + (wPort * sizeof(EC_T_DWORD))));

            /* detect missing port receive time */
            if ((0 != qwRecvTimeProcessingUnit) &&
                (abs((EC_T_INT)EC_LODWORD(qwRecvTimeProcessingUnit) - (EC_T_INT)m_adwRecvTime[wPort]) > (EC_T_INT)dwMaxDelay))
            {
                SetRecvTimeMissingPortX(wPort, EC_TRUE);
                bRecvTimesCoherent = EC_FALSE;
            }
        }
    }
#if (defined INCLUDE_DC_SUPPORT)
    m_qwRecvTimeProcessingUnit = qwRecvTimeProcessingUnit;
#endif
    SetRecvTimesCoherent(bRecvTimesCoherent);
}

/***************************************************************************************************/
/**
\brief  Calculate port delays
*/
EC_T_VOID CEcBusSlave::CalculatePortDelays(EC_T_INT nMeasCycles)
{
EC_T_WORD  wPortIdx      = 0;
EC_T_WORD  wPortIdxStart = 0;
EC_T_WORD  wPortPrev     = 0;
EC_T_DWORD dwDelay       = 0;

    /* skip if RecvTimes not supported */
    if (!IsRecvTimesSupported())
    {
        return;
    }
    /* get port index of input port */
    for (wPortIdxStart = 0; wPortIdxStart < ESC_PORT_COUNT; wPortIdxStart++)
    {
        if (m_wInputPort == CEcBusTopology::c_awFrameProcessingPortOrder[wPortIdxStart])
        {
            break;
        }
    }
    /* cumulate port delays between connected ports, loop one more to handle input port */
    for (wPortIdx = 0, wPortPrev = 0xFFFF; wPortIdx < (ESC_PORT_COUNT+1); wPortIdx++)
    {
    EC_T_WORD wPortCur = CEcBusTopology::c_awFrameProcessingPortOrder[(EC_T_DWORD)(wPortIdxStart + wPortIdx) % ESC_PORT_COUNT];

        if (IsSlaveConPortX(wPortCur) && !IsRecvTimeMissingPortX(wPortCur))
        {
            if ((0xFFFF != wPortPrev) && (wPortCur != wPortPrev))
            {
                /* receive time difference is negative at "input port" */
                if (GetRecvTimeX(wPortCur) < GetRecvTimeX(wPortPrev))
                {
                    dwDelay = GetRecvTimeX(wPortPrev) - GetRecvTimeX(wPortCur);

                    m_wInputPort = wPortCur;

                    /* cumulate slave delay */
                    m_dwSlaveDelay += dwDelay;

                    /* during last measurement cycle, calculate slave delay */
                    if (0 != nMeasCycles)
                    {
                        m_dwSlaveDelay /= nMeasCycles;
                    }
                }
                else if (GetRecvTimeX(wPortCur) > GetRecvTimeX(wPortPrev))
                {
                    dwDelay = GetRecvTimeX(wPortCur) - GetRecvTimeX(wPortPrev);
                    
                    /* consider processing unit after port A */
                    if (ESC_PORT_A == wPortPrev)
                    {
                        dwDelay -= m_dwProcessingUnitDelay;
                    }
                    /* cumulate port delay */
                    m_adwPortDelay[wPortCur] += dwDelay;

                    /* during last measurement cycle, calculate port delay */
                    if (0 != nMeasCycles)
                    {
                        m_adwPortDelay[wPortCur] /= nMeasCycles;
                    }
                }
            }
            wPortPrev = wPortCur;
        }
    }
    return;
}
#endif /* INCLUDE_DC_SUPPORT || INCLUDE_LINE_CROSSED_DETECTION */

EC_T_VOID CEcBusSlave::GetSlaveProp(EC_T_SLAVE_PROP* pSlaveProp)
{ 
    if (EC_NULL != m_pCfgSlave)
    {
        m_pCfgSlave->GetSlaveProp(pSlaveProp);
    }
    else
    {
        OsStrcpy(pSlaveProp->achName, "Unknown");
        pSlaveProp->wAutoIncAddr    = GetAutoIncAddress();
        pSlaveProp->wStationAddress = GetFixedAddress();
    }
}

EC_T_WORD CEcBusSlave::CreateLinkPortStatusFromDlStatusX(EC_T_WORD wPort, EC_T_WORD wDlStatusVal)
{
EC_T_WORD wLinkPortStatus = 0;

    /* by some slaves the port descriptor register returns always 0 */
    /* in this case the physics description from the ENI file should be used */
    /* but the current function can be called before reading the configuration */
    if ((0 == GetPortDescriptor()) || IsSlaveCfgPortX(wPort))
    {
        /* Physical link on Port 0..3 Bit 4..7 */
        if( (ECM_DLS_PDI_PHYSICAL_LINK_ON_PORT(wPort)) == (wDlStatusVal&(ECM_DLS_PDI_PHYSICAL_LINK_ON_PORT(wPort))) )
        {
            wLinkPortStatus |= ECBT_LINKPORT_LINKSTATUS;
        }

        /* Loop Port 0..3 Bit 8, 10, 12, 14 */
        if( (ECM_DLS_PDI_LOOP_PORT(wPort)) == (wDlStatusVal & (ECM_DLS_PDI_LOOP_PORT(wPort))) )
        {
            wLinkPortStatus |= ECBT_LINKPORT_LOOPSTATUS;
        }

        /* Communication on Port 0..3 Bit 9, 11, 13, 15 */
        if( (ECM_DLS_PDI_COM_ON_PORT(wPort)) == (wDlStatusVal & (ECM_DLS_PDI_COM_ON_PORT(wPort))) )
        {
            wLinkPortStatus |= ECBT_LINKPORT_SIGNALDETECTION;
        }

        /* determine if slave is connected at current port, according DL status information */
        {
        EC_T_BOOL bSlaveConnected = EC_FALSE;

            /* |#|Comm. established | Loop closed | Link Detected || Slave present || Comment
             * | |  SIGNALDETECTION |  LOOPSTATUS |    LINKSTATUS ||               ||
             * |-|------------------|-------------|---------------||---------------||----------------------------------
             * |0|                0 |           0 |             0 ||             0 || Shall not occur, only if port is not configured
             * |1|                0 |           0 |             1 ||             1 ||
             * |2|                0 |           1 |             0 ||             0 || Standard
             * |3|                0 |           1 |             1 ||             0 || EK1100 old
             * |4|                1 |           0 |             0 ||             1 || EK1100 old, EL2004 old, EL4132 (old), EK1122!
             * |5|                1 |           0 |             1 ||             1 || Standard
             * |6|                1 |           1 |             0 ||             0 ||
             * |7|                1 |           1 |             1 ||             0 || EK1122 with closed port
             */
            switch (wLinkPortStatus & ECBT_LINKPORT_STATUS_MASK)
            {
            /* |0| */ case (0                                                                                  ): bSlaveConnected = EC_FALSE; break; /* ESCTYPE_BKHF_EKOLD */
            /* |1| */ case (                                                           ECBT_LINKPORT_LINKSTATUS): bSlaveConnected = EC_TRUE;  OsDbgAssert(EC_FALSE); break;
            /* |2| */ case (                                ECBT_LINKPORT_LOOPSTATUS                           ): bSlaveConnected = EC_FALSE; break;
            /* |3| */ case (                                ECBT_LINKPORT_LOOPSTATUS | ECBT_LINKPORT_LINKSTATUS): bSlaveConnected = EC_FALSE; break;
            /* |4| */ case (ECBT_LINKPORT_SIGNALDETECTION                                                      ): bSlaveConnected = EC_TRUE;  break;
            /* |5| */ case (ECBT_LINKPORT_SIGNALDETECTION |                            ECBT_LINKPORT_LINKSTATUS): bSlaveConnected = EC_TRUE;  break;
            /* |6| */ case (ECBT_LINKPORT_SIGNALDETECTION | ECBT_LINKPORT_LOOPSTATUS                           ): bSlaveConnected = EC_FALSE; break;
            /* |7| */ case (ECBT_LINKPORT_SIGNALDETECTION | ECBT_LINKPORT_LOOPSTATUS | ECBT_LINKPORT_LINKSTATUS): bSlaveConnected = EC_FALSE; break;
            default: OsDbgAssert(EC_FALSE); break;
            }
            if (bSlaveConnected)
            {
                wLinkPortStatus |= ECBT_LINKPORT_SLAVECONNECTED;
            }
        }
    }    
    return wLinkPortStatus;
}

/***************************************************************************************************/
/**
\brief  Store Link Port Information.

*/
EC_T_VOID CEcBusSlave::SetPortSetting(
    EC_T_WORD wVal       /**< [in] value from incoming packet */
                                                     )
{
EC_T_WORD wPortIdx = 0;

    /* store the complete ESC DL Status word (0x0110) */
    m_wDlStatus = wVal;

    /* Check if PDI watchdog expired */
    if (ECM_DLS_PDI_WATCHDOG_STATUS != (wVal&ECM_DLS_PDI_WATCHDOG_STATUS))
    {
        m_wFlagField |= ECBT_SLAVEFLAG_PDIWDEXPIRED;
    }
    /* store per port settings, which are bits following up 4 times */
    m_byConnectedPortCnt = 0;
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
        m_awLinkPort[wPortIdx] &= ~ECBT_LINKPORT_STATUS_MASK;
        m_awLinkPort[wPortIdx] |= CreateLinkPortStatusFromDlStatusX(wPortIdx, wVal);

        if (IsSlaveConPortX(wPortIdx))
        {
#if (defined INCLUDE_RESCUE_SCAN)
            SetFramelossAfterSlave(wPortIdx, EC_FALSE);
#endif
            m_byConnectedPortCnt++;
        }
#if (defined INCLUDE_RESCUE_SCAN)
        if (!IsSignalPortX(wPortIdx))
        {
            SetFramelossAfterSlave(wPortIdx, EC_FALSE);
        }
#endif
    }
    /* bus slave is branching if more than 2 ports are connected */
    if (2 < m_byConnectedPortCnt)
    {
        SetBranchingFlag(EC_TRUE);
    }
    else
    {
        SetBranchingFlag(EC_FALSE);
    }
}


/***************************************************************************************************/
/**
\brief  Store Link Port Information.

*/
EC_T_VOID CEcBusSlave::SetPdiControl(
    EC_T_WORD wVal       /**< [in] value from incoming packet */
                                                     )
{
    /* PDI Control (0x0140) */
    m_byPDI = EC_LOBYTE((wVal));

    /* Device emulation (control of AL status) Bit 8 */
    if (wVal & ECM_PDICTRL_DEVICE_EMULATION)
    {
        m_wFlagField   |= ECBT_SLAVEFLAG_DEVICE_EMULATION;
    }
}

/***************************************************************************************************/
/**
\brief  Is Link configured.

\return EC_TRUE if configured, EC_FALSE otherwise.
*/
EC_T_VOID CEcBusSlave::SetPortDescriptor(          
    EC_T_BYTE   byPortDescriptor
    )
{
EC_T_WORD wPortIdx = 0;

    m_byPortDescriptor = byPortDescriptor;

    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
        EC_T_BYTE byPortConfig    = (EC_T_BYTE)((byPortDescriptor >> (2 * wPortIdx)) & ECM_PORT_MASK);
        EC_T_BOOL bPortConfigured = (byPortConfig > ECM_PORT_NOTCONFIGURED);

        if (bPortConfigured)
        {
            m_awLinkPort[wPortIdx] = (EC_T_WORD)(m_awLinkPort[wPortIdx] |  ECBT_LINKPORT_CONFIGURED);
        }
        else
        {
            m_awLinkPort[wPortIdx] = (EC_T_WORD)(m_awLinkPort[wPortIdx] & ~ECBT_LINKPORT_CONFIGURED);
        }
    }
}

/***************************************************************************************************/
/**
\brief  Fill Slave Information Descriptor.
*/
EC_T_VOID CEcBusSlave::GetSlaveInfo(
    EC_PT_SB_SLAVEINFO_DESC pInfo   /**< [out]  Slave information descriptor */
                                                   )
{
    if( EC_NULL != pInfo )
    {
        pInfo->dwScanBusStatus  = m_dwScanStatus;
        pInfo->dwVendorId       = m_dwVendorId;
        pInfo->dwProductCode    = m_dwProductCode;
        pInfo->dwRevisionNumber = m_dwRevisionNumber;
        pInfo->dwSerialNumber   = m_dwSerialNumber;
    }
}

/***************************************************************************************************/
/**
\brief  Get port state
*/
EC_T_WORD CEcBusSlave::GetPortState(EC_T_VOID)
{
EC_T_WORD wPortState = 0;

    /* by some slaves the port descriptor register returns always 0 */
    /* in this case the physics description from the ENI file should be used */
    /* but the current function can be called before reading the configuration */
    if ((0 == GetPortDescriptor()) || IsSlaveCfgPortX(ESC_PORT_A))
    {
        wPortState |= ((EC_T_WORD)(((IsSlaveConPortX(ESC_PORT_A))?1:0) <<0x0));
        wPortState |= ((EC_T_WORD)(((IsLinkPortX(ESC_PORT_A))?1:0)     <<0x4));
        wPortState |= ((EC_T_WORD)(((IsLoopPortX(ESC_PORT_A))?1:0)     <<0x8));
        wPortState |= ((EC_T_WORD)(((IsSignalPortX(ESC_PORT_A))?1:0)   <<0xc));
    }
    if ((0 == GetPortDescriptor()) || IsSlaveCfgPortX(ESC_PORT_B))
    {
        wPortState |= ((EC_T_WORD)(((IsSlaveConPortX(ESC_PORT_B))?1:0)  <<0x1));
        wPortState |= ((EC_T_WORD)(((IsLinkPortX(ESC_PORT_B))?1:0)      <<0x5));
        wPortState |= ((EC_T_WORD)(((IsLoopPortX(ESC_PORT_B))?1:0)      <<0x9));
        wPortState |= ((EC_T_WORD)(((IsSignalPortX(ESC_PORT_B))?1:0)    <<0xd));
    }
    if ((0 == GetPortDescriptor()) || IsSlaveCfgPortX(ESC_PORT_C))
    {
        wPortState |= ((EC_T_WORD)(((IsSlaveConPortX(ESC_PORT_C))?1:0)  <<0x2));
        wPortState |= ((EC_T_WORD)(((IsLinkPortX(ESC_PORT_C))?1:0)      <<0x6));
        wPortState |= ((EC_T_WORD)(((IsLoopPortX(ESC_PORT_C))?1:0)      <<0xa));
        wPortState |= ((EC_T_WORD)(((IsSignalPortX(ESC_PORT_C))?1:0)    <<0xe));
    }
    if ((0 == GetPortDescriptor()) || IsSlaveCfgPortX(ESC_PORT_D))
    {
        wPortState |= ((EC_T_WORD)(((IsSlaveConPortX(ESC_PORT_D))?1:0)  <<0x3));
        wPortState |= ((EC_T_WORD)(((IsLinkPortX(ESC_PORT_D))?1:0)      <<0x7));
        wPortState |= ((EC_T_WORD)(((IsLoopPortX(ESC_PORT_D))?1:0)      <<0xb));
        wPortState |= ((EC_T_WORD)(((IsSignalPortX(ESC_PORT_D))?1:0)    <<0xf));
    }
    return wPortState;
}

/*-/class CEcBusSlave--------------------------------------------------------*/

#if (defined INCLUDE_GEN_OP_ENI)
/*-class CEcBusSlaveGenEni------------------------------------------------------*/
EC_T_VOID CEcBusSlaveGenEni::InitInstance(EC_T_VOID)
{
    m_wEEPROMAddrCur         = 0;
    m_wEEPROMCategoryBaseCur = 0;
    m_wEEPROMCategorySizeCur = 0;
    m_wEEPROMCategoryTypeCur = 0;

    OsMemset(m_abyEEPROMValCur, 0, sizeof(m_abyEEPROMValCur));
    m_byEEPROMValCntCur      = 0;

    m_poPdoListCur           = EC_NULL;
    m_byPdoCntCur            = 0;

    m_bySmOutEntries         = 0;
    m_bySmInpEntries         = 0;
    m_byLogicalMBoxState     = 0;
    OsMemset(&m_abyFmmu[0],       0, sizeof(m_abyFmmu));
    OsMemset(&m_aSmOutEntries[0], 0, sizeof(m_aSmOutEntries));
    OsMemset(&m_aSmInpEntries[0], 0, sizeof(m_aSmInpEntries));
    for (EC_T_DWORD dwIdx = 0; dwIdx < EC_CFG_SLAVE_PD_SECTIONS; dwIdx++)
    {
        m_aSmOutEntries[dwIdx].byIdx = 0xFF;
        m_aSmInpEntries[dwIdx].byIdx = 0xFF;
    }
    m_poTxPdoList            = EC_NULL;
    m_poRxPdoList            = EC_NULL;

    m_wDcActivation          = 0;
    m_dwDcSync0CycleTime     = 0;
    OsMemset(m_aDcSync, 0, sizeof(m_aDcSync));

    CEcBusSlave::InitInstance();
}
/*-/class CEcBusSlaveGenEni-----------------------------------------------------*/
#endif /* INCLUDE_GEN_OP_ENI */

/*-class CEcBusTopology------------------------------------------------------*/

/*-STATIC CLASS-MEMBERS------------------------------------------------------*/

/* SII offset for READEEPROMENTRY_IDX_... */
const EC_T_WORD CEcBusTopology::c_awReadEEPROMEntryOrder[READEEPROMENTRY_COUNT] =
    { ESC_SII_REG_VENDORID,      ESC_SII_REG_PRODUCTCODE,   ESC_SII_REG_REVISIONNUMBER, ESC_SII_REG_SERIALNUMBER,
      0, /* READEEPROMENTRY_IDX_ALIASADDRESS - Alias address is not read from the EEPROM */
      ESC_SII_REG_BOOT_RECV_MBX, ESC_SII_REG_BOOT_SEND_MBX, ESC_SII_REG_STD_RECV_MBX,   ESC_SII_REG_STD_SEND_MBX,
      ESC_SII_REG_MBX_PROTOCOL,
      0  /* READEEPROMENTRY_IDX_CATEGORIES - Categories are read indirectly */ };

const EC_T_WORD CEcBusTopology::c_awFrameProcessingPortOrder[] =
    { ESC_PORT_A, ESC_PORT_D, ESC_PORT_B, ESC_PORT_C };

const EC_T_WORD CEcBusTopology::c_awBackwardPortOrder[] =
    { ESC_PORT_C, ESC_PORT_B, ESC_PORT_D, ESC_PORT_A };

const EC_T_WORD CEcBusTopology::c_awCloseToProcessingUnitOrder[] =
    { ESC_PORT_A, ESC_PORT_C, ESC_PORT_B, ESC_PORT_D };

/*-CLASS-METHODS-------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Default Constructor
*/
CEcBusTopology::CEcBusTopology(EC_T_VOID)
{
    m_pMaster               = EC_NULL;
    m_poEthDevice           = EC_NULL;

    /* number of pending ecat commands */
    m_dwEcatCmdPendingMax   = 0xFFFFFFFF;
    m_dwEcatCmdPending      = 0;

    /* last bus topology state machine result code */
    m_dwBTResult            = EC_E_NOTREADY;

    /* EEPROM management */
    m_dwCurEEPEntryIdx        = READEEPROMENTRY_COUNT;
    m_bEEPChkSumErrorDetected = EC_FALSE;

    /* Verification is turned on by default */
    m_bTopologyVerificationEnabled = EC_TRUE;
    m_bNotifyUnexpectedBusSlaves   = EC_TRUE;
    
    /* Spin lock, which protects list of slave objects */
    m_poBusSlaveListLock    = OsCreateLockTyped(eLockType_SPIN);

    m_bPortOperationsEnabled = EC_TRUE;

    /* by default alias addressing is disabled */
    m_bSlaveAliasAddressing = EC_FALSE;

    /* pre-configure, which EEPROM values are read and checked against ENI configuration */
    OsMemset(m_abReadEEPROMEntry, 0, READEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_VENDORID]    = BT_CHECK_EEPENTRY_VENDORID;
    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_PRODUCTCODE] = BT_CHECK_EEPENTRY_PRODUCTCODE;
    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_REVISIONNO]  = BT_CHECK_EEPENTRY_REVISIONNO;
    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_SERIALNO]    = BT_CHECK_EEPENTRY_SERIALNO;

    OsMemset(m_abCheckEEPROMSetByIoctl, 0, CHECKEEPROMENTRY_COUNT*sizeof(EC_T_BOOL));
    OsMemset(m_aeCheckEEPROMEntry, 0, CHECKEEPROMENTRY_COUNT*sizeof(EC_T_CHECKEEPROMENTRY));

    /* Initialize State Machines */
    OsMemset(&m_oRequest, 0, sizeof(m_oRequest));
    m_pRequest          = EC_NULL;
    m_pCurRequest       = EC_NULL;

    /* top level state machine */
    m_eCurState         = ecbtsm_unknown;
    m_eReqState         = ecbtsm_unknown;

    /* state machine, for data collection */
    m_eCurSubState      = ecbtsms_unknown;
    m_eReqSubState      = ecbtsms_unknown;
    m_dwSubResult       = EC_E_ERROR;

    /* state machine for EEPROM readout */
    m_eCurEEPState      = ecbtsme_unknown;
    m_eReqEEPState      = ecbtsme_unknown;
    m_dwEEPResult       = EC_E_ERROR;

    /* state machine for topology detection */
    m_eCurChkState      = ecbtsmchk_unknown;
    m_eReqChkState      = ecbtsmchk_unknown;
    m_dwChkResult       = EC_E_ERROR;

    /* topology check sub state machine variables */
    m_pScanBusSlaveCur = EC_NULL;
    m_dwScanNextSlaveList = ECBT_SLAVELIST_PORT;
    m_eResyncState = eResyncState_Idle;
    m_pResyncBusSlave = EC_NULL;
    m_dwResyncCfgSlaveStartIdx = 0;
    m_dwResyncCfgSlavesCnt = 0;
    m_dwResyncCfgSlavesProcessed = 0;

#if (defined INCLUDE_HOTCONNECT)
    m_pScanHCGroupCur = EC_NULL;
    m_pResyncHcGroupIncomplete = EC_NULL;
#endif
    m_bBusMismatchNotified      = EC_FALSE;
    m_bNextBusScanGenEni        = EC_FALSE;

    /* Handle storage for integrated state machine queue access by functions: StartBusScan, StartTopologyChange, StartDlStatusInt, WaitForBTStateMachine */
    m_pBSStartReq               = EC_NULL;

    /* memory pool */
    m_poBusSlavesPool           = EC_NULL;
    m_poBusSlavesGenEniPool     = EC_NULL;
    m_dwBusSlavesPoolSize       = 0;
    m_pBusSlavesByIndex         = EC_NULL;
    m_pBusSlavesByFixedAddress  = EC_NULL;
    m_poBusSlavesPoolFree       = 0;
    m_pbyBusSlavesMem           = EC_NULL;
    m_dwBusSlavesMemSize        = 0;
    m_pbyBusSlavesMemCur        = EC_NULL;
    m_bBusSlavesPoolReady       = EC_FALSE;

    /* free address pool, filled from ENI (XPAT Configuration) and used to generate Temporary Fixed addresses during Scan Bus operation */
    OsMemset(m_abFreeAddresses, 0, sizeof(m_abFreeAddresses));
    /* initially configure all addresses as free range. After ENI File is read (ecatConfigureMaster) those values are updated to real free
     * address spaces. */
    m_abFreeAddresses[0].bRangeUsed = EC_FALSE;
    m_abFreeAddresses[0].wStartAddr = 1;
    m_abFreeAddresses[0].wEndAddr   = ((EC_T_WORD)-1);
    m_abFreeAddresses[0].wWorkAddr  = 0;

    /* topology changed management */
    m_bTopologyChangeAutoMode   = ECBT_TOPOCHANGE_AUTOMODE;
    m_wTopologyChangeSlaveCount = EC_AL_STATUS_INIT_VALUE;
    m_dwTopologyChangeCounter   = 0;
    m_dwTopologyChangeDelay     = ECBT_TOPOCHANGE_DELAY;
    m_bTopologyChangedMasked    = EC_FALSE;
    m_bTopologyChanged          = EC_FALSE;
    m_bLinkWasDisconnected      = EC_FALSE;
    m_bSendMaskedOnMain         = EC_FALSE;
#if (defined INCLUDE_RED_DEVICE)
    m_bSendMaskedOnRed          = EC_FALSE;
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    m_bRedEnhancedLineCrossedDetectionEnabled = EC_FALSE;
    m_bRedEnhancedLineCrossedDetectionRunning = EC_FALSE;
#endif
#endif
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    m_bErrorOnLineCrossed            = EC_TRUE;
    m_bNotifyNotConnectedPortA       = EC_TRUE;
    m_bNotifyUnexpectedConnectedPort = EC_FALSE;
    m_bLineCrossedNotified           = EC_FALSE;
    m_bLineCrossedDetectionRunning   = EC_FALSE;
#endif
    m_bOpenClosedPorts            = EC_FALSE;
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    m_bJunctionRedundancyEnabled  = EC_FALSE;
#endif
    /* topology changed while bus scan management */
    m_wAcycBrdAlStatusWkc         = 0;
    m_dwSlavesAtStart             = 0;
    m_bLinkConnectedAtStart       = EC_FALSE;
#if (defined INCLUDE_RED_DEVICE)
    m_bLineBreakAtStart           = EC_FALSE;
    m_bIsSendEnabledOnMainAtStart = EC_FALSE;
    m_bIsSendEnabledOnRedAtStart  = EC_FALSE;
    m_wSlavesCntOnMainAtStart     = 0;
    m_wSlavesCntOnRedAtStart      = 0;
#endif

    /* topology information */
    m_pBusSlaveTopoMainRoot     = EC_NULL;
    m_pBusSlaveMainPort         = EC_NULL;
#if (defined INCLUDE_RED_DEVICE)
    m_pBusSlaveTopoRedRoot      = EC_NULL;
    m_pBusSlaveRedPort          = EC_NULL;
    m_wRedPort                  = ESC_PORT_INVALID;
#endif
    /* bus slave list management */
    m_pBusSlaveRoot             = EC_NULL;
    m_dwBusSlaves               = 0;
    m_dwBusSlavesDC             = 0;
    m_bPurgeBusSlavesAndNotify  = EC_FALSE;
    m_pBusSlaveDuplicate        = EC_NULL;

    /* generic state machine variables */
    m_dwEcatCmdPending     = 0;
    m_dwEcatCmdSent        = 0;
    m_dwErrorCur           = EC_E_NOERROR;
    m_dwEcatCmdProcessed   = 0;
    m_dwEcatCmdRetry       = 0;
    m_pBusSlaveCur         = EC_NULL;
    m_pBusSlaveTopoCur     = EC_NULL;
    m_pawExpectedProcessingPortOrder = c_awFrameProcessingPortOrder;
    m_pBusSlaveBranchingCur = EC_NULL;
    m_wPortBranchingCur     = ESC_PORT_INVALID;
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    m_eJunctionRedState    = eJunctionRedStateUnknow;
#endif
    m_dwBusSlavesDCCur     = 0;
    m_dwCfgSlaveIdxCur     = 0;
    m_dwAbsentIdxCur       = 0;

    m_dwBusSlavesUnexpected    = 0;
    m_dwBusSlavesUnexpectedCur = 0;
}

/***************************************************************************************************/
/**
\brief  Destructor
*/
CEcBusTopology::~CEcBusTopology()
{
    DeleteBusSlavesPool();
    DeleteCfgTopology();
    OsDeleteLock(m_poBusSlaveListLock);
}

/***************************************************************************************************/
/**
\brief  Initialize Instance
*/
EC_T_VOID CEcBusTopology::InitInstance(
    CEcMaster*  pMaster             /**< [in] Master Instance handle */
   ,CEcDevice*  poEthDevice         /**< [in] Ethernet device interface */
   ,EC_T_DWORD  dwMaxSlaveElements  /**< [in] Maximum number of slave elements to detect */
                                        )
{
    m_pMaster     = pMaster;
    m_poEthDevice = poEthDevice;
    m_oSingleSlaveListCur.AddHead(EC_NULL);
    CreateBusSlavesPool(dwMaxSlaveElements, EC_FALSE);
}

 /***************************************************************************************************/
/**
\brief  Create topology information based on config slaves
*/
EC_T_DWORD CEcBusTopology::CreateCfgTopology(EC_T_VOID)
{
EC_T_DWORD dwSlaveIdx = 0;
EC_T_DWORD dwSlaveCnt = m_pMaster->GetCfgSlaveCount();
CEcSlave*  pCfgSlave  = EC_NULL;

    /* generate information from the config slaves */
    for (dwSlaveIdx = 0; dwSlaveIdx < dwSlaveCnt; dwSlaveIdx++)
    {
    EC_T_DWORD dwPrevPortIdx = 0;

        /* get config slave */
        pCfgSlave = m_pMaster->GetSlaveByIndex(dwSlaveIdx);
        if (EC_NULL == pCfgSlave)
        {
            OsDbgAssert(EC_FALSE);
            continue;
        }
        /* look for first slave considering Superset-ENI */
        if ((0 == dwSlaveIdx) || ((0 == pCfgSlave->GetNumPreviousPorts()) && (!pCfgSlave->IsHCGroupHead())))
        {
            while (!m_oFirstSlaveList.IsEmpty())
            {
                m_oFirstSlaveList.RemoveTail();
            }
            m_oFirstSlaveList.AddTail(pCfgSlave);
        }
        /* walk through each previous port entry */
        for (dwPrevPortIdx = 0; dwPrevPortIdx < pCfgSlave->GetNumPreviousPorts(); dwPrevPortIdx++)
        {
        EC_T_SLAVE_PORT_DESC* pPortDesc = &(pCfgSlave->m_pPreviousPort[dwPrevPortIdx]);

            /* previous port information found, add current slave to its list */
            if (0 == (pPortDesc->wPortNumber & IGNORE_PREV_PORT_FLAG))
            {
                /* insert current slave into the possible children at port list */
                {
                CEcSlave* pCfgSlavePrev = m_pMaster->GetSlaveByCfgFixedAddr(pPortDesc->wSlaveAddress);

                    if (EC_NULL != pCfgSlavePrev)
                    {
                        pCfgSlavePrev->m_aoPossiblePortChildrenList[pPortDesc->wPortNumber].AddTail(pCfgSlave);
                    }
                }
            }
            else
            {
                /* port number is marked (0x8000), insert current slave into the possible bus child of previous slave */
                if (0 != dwSlaveIdx)
                {
                CEcSlave* pCfgSlavePrev = m_pMaster->GetSlaveByIndex(dwSlaveIdx-1);

                    if (EC_NULL != pCfgSlavePrev)
                    {
                        pCfgSlavePrev->m_oPossibleBusChildrenList.AddTail(pCfgSlave);
                    }
                }
            }
        }
#if (defined INCLUDE_HOTCONNECT)
        /* store HC group head slaves */
        if (pCfgSlave->IsHCGroupHead())
        {
            m_oHCSlavesList.AddTail(pCfgSlave);
        }
#endif /* INCLUDE_HOTCONNECT */
    }
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Delete topology information based on config slaves
*/
EC_T_DWORD CEcBusTopology::DeleteCfgTopology(EC_T_VOID)
{
EC_T_DWORD dwSlaveIdx = 0;
EC_T_DWORD dwSlaveCnt = (EC_NULL == m_pMaster)?0:m_pMaster->GetCfgSlaveCount();
CEcSlave*  pCfgSlave  = EC_NULL;

    /* delete information in the config slaves */
    for (dwSlaveIdx = 0; dwSlaveIdx < dwSlaveCnt; dwSlaveIdx++)
    {
    EC_T_WORD wPortIdx = 0;

        /* get config slave */
        pCfgSlave = m_pMaster->GetSlaveByIndex(dwSlaveIdx);
        if (EC_NULL == pCfgSlave)
        {
            OsDbgAssert(EC_FALSE);
            continue;
        }
        /* delete possible children list entries */
        for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
        {
            while (!pCfgSlave->m_aoPossiblePortChildrenList[wPortIdx].IsEmpty())
            {
                pCfgSlave->m_aoPossiblePortChildrenList[wPortIdx].RemoveTail();
            }
        }
        /* delete bus children list entries */
        while (!pCfgSlave->m_oPossibleBusChildrenList.IsEmpty())
        {
            pCfgSlave->m_oPossibleBusChildrenList.RemoveTail();
        }
    }
#if (defined INCLUDE_HOTCONNECT)
    /* delete HC list entries */
    while (!m_oHCSlavesList.IsEmpty())
    {
        m_oHCSlavesList.RemoveTail();
    }
#endif /* INCLUDE_HOTCONNECT */

    /* delete first slave list */
    while (!m_oFirstSlaveList.IsEmpty())
    {
        m_oFirstSlaveList.RemoveTail();
    }
    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Trigger Scan of Bus for current Topology.

\return EC_E_NOERROR if start of scan passed, error code Otherwise.
*/
EC_T_DWORD CEcBusTopology::StartBusScan(
    EC_T_DWORD dwTimeout        /**< [in] Communication Timeout */
                                       )
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (EC_NULL == m_pBSStartReq)
    {
        dwRetVal = RequestState(ecbtsm_busscan_automatic, dwTimeout, &m_pBSStartReq);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Trigger Scan of Bus for current Topology.

\return EC_E_NOERROR if start of scan passed, error code Otherwise.
*/
EC_T_DWORD CEcBusTopology::StartDlStatusInt(
    EC_T_DWORD dwTimeout        /**< [in] Communication Timeout */
                                       )
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (EC_NULL == m_pBSStartReq)
    {
        dwRetVal = RequestState(ecbtsm_dlstatus_ist, dwTimeout, &m_pBSStartReq);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Trigger Scan of Bus for current Topology.

\return EC_E_NOERROR if start of scan passed, error code Otherwise.
*/
EC_T_DWORD CEcBusTopology::StartAlStatusRefresh(
    EC_T_DWORD dwTimeout        /**< [in] Communication Timeout */
                                       )
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (EC_NULL == m_pBSStartReq)
    {
        dwRetVal = RequestState(ecbtsm_alstatusrefresh, dwTimeout, &m_pBSStartReq);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Start topology change handling

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::StartTopologyChange(
    EC_T_DWORD  dwTimeout       /**< [in]   Operation timeout in Msec */
                                              )
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (EC_NULL == m_pBSStartReq)
    {
        if (m_bTopologyChangeAutoMode)
        {
            dwRetVal = RequestState(ecbtsm_busscan_automatic, dwTimeout, &m_pBSStartReq);
        }
        else
        {
            dwRetVal = RequestState(ecbtsm_busscan_manual, dwTimeout, &m_pBSStartReq);
        }
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Start accept topology change handling

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::StartAcceptTopologyChange(
    EC_T_DWORD  dwTimeout       /**< [in]   Operation timeout in Msec */
                                              )
{
EC_T_DWORD dwRetVal = EC_E_NOERROR;

    if (EC_NULL == m_pBSStartReq)
    {
        dwRetVal = RequestState(ecbtsm_busscan_accepttopo, dwTimeout, &m_pBSStartReq);
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Check state of previously started bus scan.
*
\return EC_E_NOERROR on successful completion of scan, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::WaitForBTStateMachine()
{
    if (EC_NULL != m_pBSStartReq)
    {
        if (m_eCurState == m_eReqState)
        {
            return m_dwBTResult;
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
\brief  Release BT state machine
*
\return Result
*/
EC_T_DWORD CEcBusTopology::ReleaseBTStateMachine()
{
    if (EC_NULL != m_pBSStartReq)
    {
        ReleaseReq(m_pBSStartReq);
        m_pBSStartReq = EC_NULL;
        return m_dwBTResult;
    }
    else
    {
        OsDbgAssert(EC_FALSE);
        return EC_E_NOTREADY;
    }
}

/***************************************************************************************************/
/**
\brief  Processes the response of an EtherCAT ScanBus command.

DCS Invoke ID's are redirected to here.
*/
EC_T_VOID CEcBusTopology::ProcessResult(
    EC_T_DWORD dwResult,                /**< [in] result */
    PECAT_SLAVECMD_DESC pSlaveCmd,      /**< [in] invokeId */
    PETYPE_EC_CMD_HEADER pEcCmdHeader   /**< [in] frame header */
    )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
    EC_T_DWORD   dwRes;
    EC_T_PBYTE   pbyData            = EC_NULL;
    EC_T_WORD    wWorkingCnt;
    EC_T_WORD    wAutoIncAddress;
    EC_T_WORD    wFixedAddress;
    EC_T_WORD    wTmpFixedAddress;
    CEcBusSlave* pBusSlave;
    CEcSlave*    pCfgSlave;
    EC_T_DWORD   dwInvokeId         = pSlaveCmd->invokeId;
#ifndef DEBUG
    /* don't initialize in debug to track unintialized usage in state */
    dwRes             = EC_E_ERROR;
    wWorkingCnt       = 0;
    wAutoIncAddress   = 0;
    wFixedAddress     = 0;
    wTmpFixedAddress  = 0;
    pBusSlave         = EC_NULL;
    pCfgSlave         = EC_NULL;
#endif

    /* handle error case */
    if (dwResult != EC_E_NOERROR)
    {
        switch (dwInvokeId)
        {
        case INVOKE_ID_SB_COUNT:
        case INVOKE_ID_SB_ACYCALSTATUSBRD:
            break;
        default:
            goto Exit;
        }
    }
    /* get command information */
    if (EC_NULL != pEcCmdHeader)
    {
        wWorkingCnt = ETYPE_EC_CMD_GETWKC(pEcCmdHeader);
        pbyData     = (EC_T_PBYTE)EC_ENDOF(pEcCmdHeader);
    }
    else
    {
        switch (dwInvokeId)
        {
        case INVOKE_ID_SB_COUNT:
        case INVOKE_ID_SB_OPENCLOSEDPORTA:
        case INVOKE_ID_SB_ACYCALSTATUSBRD:
            wWorkingCnt = 0;
            break;
        default:
            OsDbgAssert(EC_FALSE);
            goto Exit;
        }
    }
    if (!m_bBusSlavesPoolReady)
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "CEcBusTopology::ProcessResult: Bus slaves pool not ready in %s\n", ESTATESText(m_eCurState)));

        /* should never happen */
        OsDbgAssert(EC_FALSE);
        m_dwSubResult  = EC_E_NOMEMORY;
        m_dwEEPResult  = EC_E_NOMEMORY;
        m_dwChkResult  = EC_E_NOMEMORY;
        m_eCurSubState = m_eReqSubState = ecbtsms_error;
        m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
        m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
        goto Exit;
    }
    /* dispatch the frame */
    switch (dwInvokeId)
    {
    /* count slaves on bus */
    case INVOKE_ID_SB_COUNT:
        {
            if (m_eCurSubState == ecbtsms_countslaves_wait)
            {
                if (1 <= wWorkingCnt)
                {
                    m_dwSlavesAtStart = wWorkingCnt;

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
                else
                {
                    if (0 == m_dwEcatCmdRetry)
                    {
                        m_dwSlavesAtStart = 0;

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* get information about slaves on bus */
    case INVOKE_ID_SB_GETBUSSLAVES:
        {
            if (ecbtsms_getbusslaves_wait == m_eCurSubState)
            {
                if (1 == wWorkingCnt)
                {
                    /* validate autoincrement address */
                    if ((EC_T_WORD)((EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - (EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pSlaveCmd->pEcCmdHeader)) != (EC_T_WORD)m_dwSlavesAtStart)
                    {
                        break;
                    }
                    /* get autoincrement address from header */
                    wAutoIncAddress = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - m_dwSlavesAtStart);

                    /* record bus slave */
                    dwRes = RecordBusSlave(wAutoIncAddress, pbyData);
                    if (EC_E_NOERROR != dwRes)
                    {
                        m_dwErrorCur = dwRes;
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult || EC_E_NOTREADY == m_dwBTResult);
            }
        } break;
    /* Get Port State information per slave */
    case INVOKE_ID_SB_PORTSTATUS:
        {
            if (  (ecbtsms_getportstatus_wait == m_eCurSubState) || (ecbtsms_refreshportstatus_wait == m_eCurSubState)
               || (ecbtsms_ackinterrupt_wait  == m_eCurSubState)
#if (defined INCLUDE_RED_DEVICE)
               || (ecbtsms_readrecvtimes_wait == m_eCurSubState) || (ecbtsms_redopenportrefreshportstatus_wait == m_eCurSubState)
#endif
                )
            {
                if (1 == wWorkingCnt)
                {
                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
#if (defined INCLUDE_RED_DEVICE)
                        /* look for redundancy port */
                        if (ecbtsms_readrecvtimes_wait == m_eCurSubState)
                        {
                            for (EC_T_WORD wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
                            {
                            EC_T_WORD wPortLinkStatus = pBusSlave->CreateLinkPortStatusFromDlStatusX(wPortIdx, EC_GET_FRM_WORD(pbyData));
                            EC_T_BOOL bIsSignalPort   = (0 != (wPortLinkStatus & ECBT_LINKPORT_SIGNALDETECTION));

                                if (!bIsSignalPort && (bIsSignalPort != pBusSlave->IsSignalPortX(wPortIdx)))
                                {
                                    if (EC_NULL == m_pBusSlaveRedPort)
                                    {
                                        m_pBusSlaveRedPort = pBusSlave;
                                        m_wRedPort         = wPortIdx;
                                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "INVOKE_ID_SB_PORTSTATUS Red link detected at AutoInc 0x%04X Port %d\n", m_pBusSlaveRedPort->GetAutoIncAddress(), m_wRedPort));
                                    }
                                    else
                                    {
                                        /* several ports disconnected, signalize topology change */
                                        m_dwErrorCur = EC_E_BUSCONFIG_TOPOCHANGE;
                                    }
                                }
                            }
                        }
#endif /* INCLUDE_RED_DEVICE */
                        /* store current value */
                        pBusSlave->SetPortSetting(EC_GET_FRM_WORD(pbyData));
                        pBusSlave->ResetDLStatusEvent();

                        /* backup current information for next comparaison */
                        if (!pBusSlave->IsBackupValid())
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "INVOKE_ID_SB_PORTSTATUS BackupPortInfos() AutoInc 0x%04X DlStatus 0x%04X\n", pBusSlave->GetAutoIncAddress(), pBusSlave->GetDlStatus()));
                            pBusSlave->BackupPortInfos();
                        }
                        /* check for new connection since ecbtsms_getportstatus and  */
                        /* consider them as topology change to restart state machine */
                        /* topology change applied in ecbtsms_writeportctrl_send     */
                        if (ecbtsms_refreshportstatus_wait == m_eCurSubState)
                        {
                            if (pBusSlave->DetectNewPortConnection(m_pMaster))
                            {
                                m_dwErrorCur = EC_E_BUSCONFIG_TOPOCHANGE;
                            }
                        }
                        /* dump some register values for EcBusTopology_tests */
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                            "{0x%02X, 0x%02X}, /* AutoInc 0x%04X ESC:0x0110 */\n", 
                            pbyData[0],  pbyData[1],
                            pBusSlave->GetAutoIncAddress()));

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* Assign EEPROM of all slaves to EtherCAT Master (not PDI) */
    case INVOKE_ID_SB_EEPACCACQUIRE:
        {
            OsDbgAssert(1 == m_dwEcatCmdPending);

            if (m_eCurSubState == ecbtsms_readbusconfig_wait && m_eCurEEPState == ecbtsme_acq_eep_access_wait)
            {
                if (1 <= wWorkingCnt)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* check and wait if EEPROM is busy before sending SII command */
    case INVOKE_ID_SB_EEPPREBUSY:
        {
            OsDbgAssert(1 == m_dwEcatCmdPending);

            if (m_eCurSubState == ecbtsms_readbusconfig_wait && m_eCurEEPState == ecbtsme_chkprebusy_wait)
            {
                if (1 <= wWorkingCnt)
                {
                EC_T_WORD wCtrlStatus = EC_GET_FRM_WORD(pbyData);

                    if (0 != (wCtrlStatus & ECM_SB_EEP_CTRLSTATUS_ERR_CHKSUM))
                    {
                        /* checksum error detected */
                        m_bEEPChkSumErrorDetected = EC_TRUE;

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                    else if (0 != (wCtrlStatus & (ECM_SB_EEP_CTRLSTATUS_BUSY | ECM_SB_EEP_CTRLSTATUS_LOADING_STATUS)))
                    {
                        /* must retry the same EcatCmd */
                    }
                    else
                    {
                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* look for slave(s) with EEPROM checksum error */
    case INVOKE_ID_SB_EEPCHECKSUMERROR:
        {
            if (m_eCurSubState == ecbtsms_readbusconfig_wait && m_eCurEEPState == ecbtsme_checksumerror_wait)
            {
                if (1 <= wWorkingCnt)
                {
                EC_T_WORD wCtrlStatus = EC_GET_FRM_WORD(pbyData);

                    if (0 != (wCtrlStatus & ECM_SB_EEP_CTRLSTATUS_ERR_CHKSUM))
                    {
                        /* get bus slave */
                        wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                        pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                        if (EC_NULL != pBusSlave)
                        {
                            EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();
                            if (EC_NULL != pNotification)
                            {
                                EC_T_EEPROM_CHECKSUM_ERROR_DESC* pEEPROMChecksumError = &(pNotification->uNotification.ErrorNotification.desc.EEPROMChecksumErrorDesc);
                                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_EEPROM_CHECKSUM_ERROR;

                                pBusSlave->GetSlaveProp(&pEEPROMChecksumError->SlaveProp);

                                m_pMaster->NotifyAndFree(EC_NOTIFY_EEPROM_CHECKSUM_ERROR, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_EEPROM_CHECKSUM_ERROR);
                            }
                        }
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* write EEPROM SII Address to all slaves */
    case INVOKE_ID_SB_EEPADDR:
        {
            if (m_eCurSubState == ecbtsms_readbusconfig_wait && m_eCurEEPState == ecbtsme_writeaddr_wait)
            {
                if (1 <= wWorkingCnt)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* check and wait, whether slaves are done with processing the SII request */
    case INVOKE_ID_SB_EEPBUSY:
        {
            OsDbgAssert(1 == m_dwEcatCmdPending);

            if (m_eCurSubState == ecbtsms_readbusconfig_wait && m_eCurEEPState == ecbtsme_chkbusy_wait)
            {
                if (1 <= wWorkingCnt)
                {
                    EC_T_WORD wCtrlStatus = EC_GET_FRM_WORD(pbyData);

                    if (ECM_SB_EEP_CTRLSTATUS_BUSY == (wCtrlStatus&ECM_SB_EEP_CTRLSTATUS_BUSY))
                    {
                        /* must retry */
                    }
                    else
                    {
                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* Read SII Data from each slave individually */
    case INVOKE_ID_SB_EEPREAD:
        {
            if (m_eCurSubState == ecbtsms_readbusconfig_wait && m_eCurEEPState == ecbtsme_readvalue_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        if (m_dwCurEEPEntryIdx < READEEPROMENTRY_IDX_CATEGORIES)
                        {
                        EC_T_DWORD dwEEPVal = EC_GET_FRM_DWORD(pbyData);

                            switch(m_dwCurEEPEntryIdx)
                            {
                            case READEEPROMENTRY_IDX_VENDORID:    pBusSlave->SetVendorId(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_PRODUCTCODE: pBusSlave->SetProductCode(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_REVISIONNO:  pBusSlave->SetRevisionNo(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_SERIALNO:    pBusSlave->SetSerialNo(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_BOOTRCVMBX:  pBusSlave->SetBootRcvMbx(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_BOOTSNDMBX:  pBusSlave->SetBootSndMbx(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_STDRCVMBX:   pBusSlave->SetStdRcvMbx(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_STDSNDMBX:   pBusSlave->SetStdSndMbx(dwEEPVal); break;
                            case READEEPROMENTRY_IDX_MBXPROTOCOL: pBusSlave->SetMbxProtocols(EC_LOWORD(dwEEPVal)); break;
                            default: OsDbgAssert(EC_FALSE); break;
                            }
                        }
#if (defined INCLUDE_GEN_OP_ENI)
                        else
                        {
                        CEcBusSlaveGenEni* pBusSlaveGenEni = pBusSlave->GetBusSlaveGenEni();

                            if (EC_NULL != pBusSlaveGenEni)
                            {
                            EC_T_WORD wEEPROMAddrFirst  = (EC_T_WORD)(pBusSlaveGenEni->m_wEEPROMCategoryBaseCur + ESC_SII_CAT_HDRSIZE);  /* WORD address! */
                            EC_T_WORD wEEPROMAddrLast   = (EC_T_WORD)(wEEPROMAddrFirst + pBusSlaveGenEni->m_wEEPROMCategorySizeCur - 1); /* WORD address! */
                            EC_T_BOOL bReadNextCategory = EC_FALSE;

                                if (pBusSlaveGenEni->m_wEEPROMAddrCur == pBusSlaveGenEni->m_wEEPROMCategoryBaseCur)
                                {
                                    pBusSlaveGenEni->m_wEEPROMAddrCur         = wEEPROMAddrFirst;
                                    pBusSlaveGenEni->m_wEEPROMCategoryTypeCur = EC_GET_FRM_WORD(pbyData);
                                    pBusSlaveGenEni->m_wEEPROMCategorySizeCur = EC_GET_FRM_WORD(pbyData + 2);

                                    OsMemset(&pBusSlaveGenEni->m_abyEEPROMValCur[0], 0, sizeof(pBusSlaveGenEni->m_abyEEPROMValCur));
                                    pBusSlaveGenEni->m_byEEPROMValCntCur = 0;
                                }
                                else
                                {
                                    switch (pBusSlaveGenEni->m_wEEPROMCategoryTypeCur)
                                    {
                                    case ESC_SII_CAT_FMMU:
                                    {
                                    EC_T_DWORD dwFmmuIdx = (pBusSlaveGenEni->m_wEEPROMAddrCur - wEEPROMAddrFirst) * 2;

                                        /* store FMMU entries */
                                        if ((dwFmmuIdx + 1 < EC_CFG_SLAVE_PD_SECTIONS) && (pBusSlaveGenEni->m_wEEPROMAddrCur     <= wEEPROMAddrLast))
                                        {
                                            pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx]   = pbyData[0];                                        
                                            pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx+1] = pbyData[1];

                                            if ((ESC_SII_CAT_FMMU_MAILBOX == pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx]) || (ESC_SII_CAT_FMMU_MAILBOX == pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx+1]))
                                            {
                                                pBusSlaveGenEni->m_byLogicalMBoxState = 1;
                                            }
                                        }
                                        if ((dwFmmuIdx + 3 < EC_CFG_SLAVE_PD_SECTIONS) && (pBusSlaveGenEni->m_wEEPROMAddrCur + 1 <= wEEPROMAddrLast))
                                        {
                                            pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx+2] = pbyData[2];
                                            pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx+3] = pbyData[3];

                                            if ((ESC_SII_CAT_FMMU_MAILBOX == pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx+2]) || (ESC_SII_CAT_FMMU_MAILBOX == pBusSlaveGenEni->m_abyFmmu[dwFmmuIdx+3]))
                                            {
                                                pBusSlaveGenEni->m_byLogicalMBoxState = 1;
                                            }
                                        }
                                        /* read next FMMU entries */
                                        pBusSlaveGenEni->m_wEEPROMAddrCur = (EC_T_WORD)(pBusSlaveGenEni->m_wEEPROMAddrCur + 2); /* DWORDwise */
                                    } break;
                                    case ESC_SII_CAT_SYNCM:
                                    {
                                        /* store current val */
                                        OsMemcpy(&pBusSlaveGenEni->m_abyEEPROMValCur[pBusSlaveGenEni->m_byEEPROMValCntCur], pbyData, 4);
                                        pBusSlaveGenEni->m_byEEPROMValCntCur = (EC_T_BYTE)(pBusSlaveGenEni->m_byEEPROMValCntCur + 4);

                                        /* read complete */
                                        if (8 == pBusSlaveGenEni->m_byEEPROMValCntCur)
                                        {
                                            /* store SYNCM entry */
                                            if (ESC_SII_CAT_SYNCM_TYPE_PDOUT == pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_TYPE])
                                            {
                                                pBusSlaveGenEni->m_aSmOutEntries[pBusSlaveGenEni->m_bySmOutEntries].wAddr        = EC_GET_FRM_WORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_STARTADDRESS]);
                                                pBusSlaveGenEni->m_aSmOutEntries[pBusSlaveGenEni->m_bySmOutEntries].byControl    = pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_CONTROL];
                                                pBusSlaveGenEni->m_aSmOutEntries[pBusSlaveGenEni->m_bySmOutEntries].byEnable     = pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_ENABLE];
                                                pBusSlaveGenEni->m_aSmOutEntries[pBusSlaveGenEni->m_bySmOutEntries].byIdx        = (EC_T_BYTE)(pBusSlaveGenEni->m_bySmOutEntries + pBusSlaveGenEni->m_bySmInpEntries);
                                                if(0 != pBusSlaveGenEni->GetMbxProtocols()) pBusSlaveGenEni->m_aSmOutEntries[pBusSlaveGenEni->m_bySmOutEntries].byIdx += 2;
                                                pBusSlaveGenEni->m_bySmOutEntries++;
                                            }
                                            else if (ESC_SII_CAT_SYNCM_TYPE_PDIN == pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_TYPE])
                                            {
                                                pBusSlaveGenEni->m_aSmInpEntries[pBusSlaveGenEni->m_bySmInpEntries].wAddr        = EC_GET_FRM_WORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_STARTADDRESS]);
                                                pBusSlaveGenEni->m_aSmInpEntries[pBusSlaveGenEni->m_bySmInpEntries].byControl    = pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_CONTROL];
                                                pBusSlaveGenEni->m_aSmInpEntries[pBusSlaveGenEni->m_bySmInpEntries].byEnable     = pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_SYNCM_REG_ENABLE];
                                                pBusSlaveGenEni->m_aSmInpEntries[pBusSlaveGenEni->m_bySmInpEntries].byIdx        = (EC_T_BYTE)(pBusSlaveGenEni->m_bySmOutEntries + pBusSlaveGenEni->m_bySmInpEntries);
                                                if(0 != pBusSlaveGenEni->GetMbxProtocols()) pBusSlaveGenEni->m_aSmInpEntries[pBusSlaveGenEni->m_bySmInpEntries].byIdx += 2;
                                                pBusSlaveGenEni->m_bySmInpEntries++;
                                            }
                                            OsMemset(&pBusSlaveGenEni->m_abyEEPROMValCur[0], 0, sizeof(pBusSlaveGenEni->m_abyEEPROMValCur));
                                            pBusSlaveGenEni->m_byEEPROMValCntCur = 0;
                                        }
                                        /* read next PDO mapping entries */
                                        pBusSlaveGenEni->m_wEEPROMAddrCur = (EC_T_WORD)(pBusSlaveGenEni->m_wEEPROMAddrCur + 2); /* DWORDwise */
                                    } break;
                                    case ESC_SII_CAT_TXPDO:
                                    case ESC_SII_CAT_RXPDO:
                                    {
                                        if (0 == pBusSlaveGenEni->m_byPdoCntCur) /* PDO list */
                                        {
                                        ECBT_PT_PDOLIST poPdoListCur = (ECBT_PT_PDOLIST)AllocBusSlaveMem(sizeof(ECBT_T_PDOLIST));

                                            if (EC_NULL == poPdoListCur)
                                            {
                                                m_dwErrorCur = EC_E_NOMEMORY;
                                                goto Exit;
                                            }
                                            /* fill PDO list */
                                            OsMemset(poPdoListCur, 0, sizeof(ECBT_T_PDOLIST));
                                            poPdoListCur->wIndex  = EC_GET_FRM_WORD((EC_T_WORD*)&pbyData[ESC_SII_CAT_PDOLIST_INDEX]);
                                            poPdoListCur->bySmIdx = 0xFF;
                                            if (ESC_SII_CAT_TXPDO == pBusSlaveGenEni->m_wEEPROMCategoryTypeCur)
                                            {
                                                for (EC_T_BYTE bySmIdx = 0; bySmIdx < EC_CFG_SLAVE_PD_SECTIONS; bySmIdx++)
                                                {
                                                    if ((0xFF != pbyData[ESC_SII_CAT_PDOLIST_SYNCM]) && (pBusSlaveGenEni->m_aSmInpEntries[bySmIdx].byIdx == pbyData[ESC_SII_CAT_PDOLIST_SYNCM]))
                                                    {
                                                        poPdoListCur->bySmIdx = bySmIdx;
                                                        break;
                                                    }
                                                }
                                                if (EC_NULL == pBusSlaveGenEni->m_poTxPdoList)
                                                {
                                                    pBusSlaveGenEni->m_poTxPdoList = poPdoListCur;
                                                }
                                            }
                                            else
                                            {
                                                for (EC_T_BYTE bySmIdx = 0; bySmIdx < EC_CFG_SLAVE_PD_SECTIONS; bySmIdx++)
                                                {
                                                    if ((0xFF != pbyData[ESC_SII_CAT_PDOLIST_SYNCM]) && (pBusSlaveGenEni->m_aSmOutEntries[bySmIdx].byIdx == pbyData[ESC_SII_CAT_PDOLIST_SYNCM]))
                                                    {
                                                        poPdoListCur->bySmIdx = bySmIdx;
                                                        break;
                                                    }
                                                }
                                                if (EC_NULL == pBusSlaveGenEni->m_poRxPdoList)
                                                {
                                                    pBusSlaveGenEni->m_poRxPdoList = poPdoListCur;
                                                }
                                            }
                                            /* store current PDO list */
                                            if (EC_NULL != pBusSlaveGenEni->m_poPdoListCur)
                                            {
                                                pBusSlaveGenEni->m_poPdoListCur->pNext = poPdoListCur;
                                            }
                                            pBusSlaveGenEni->m_poPdoListCur = poPdoListCur;

                                            /* create PDO entries for current PDO list */
                                            pBusSlaveGenEni->m_byPdoCntCur = pbyData[ESC_SII_CAT_PDOLIST_NUMOFENTRIES];
                                            poPdoListCur->byEntries = 0;
                                            poPdoListCur->poEntries = (ECBT_PT_PDOENTRY)AllocBusSlaveMem(pBusSlaveGenEni->m_byPdoCntCur*sizeof(ECBT_T_PDOENTRY));
                                            if (EC_NULL == poPdoListCur->poEntries)
                                            {
                                                m_dwErrorCur = EC_E_NOMEMORY;
                                                goto Exit;
                                            }
                                            OsMemset(poPdoListCur->poEntries, 0, pBusSlaveGenEni->m_byPdoCntCur*sizeof(ECBT_T_PDOENTRY));

                                            /* read first PDO entry */
                                            pBusSlaveGenEni->m_wEEPROMAddrCur = (EC_T_WORD)(pBusSlaveGenEni->m_wEEPROMAddrCur + (ESC_SII_CAT_PDOLIST_SIZE / 2)); /* skip current header */

                                            OsMemset(&pBusSlaveGenEni->m_abyEEPROMValCur[0], 0, sizeof(pBusSlaveGenEni->m_abyEEPROMValCur));
                                            pBusSlaveGenEni->m_byEEPROMValCntCur = 0;
                                        }
                                        else /* PDO entry */
                                        {
                                        ECBT_PT_PDOLIST poPdoListCur = pBusSlaveGenEni->m_poPdoListCur;

                                            /* store current val */
                                            OsMemcpy(&pBusSlaveGenEni->m_abyEEPROMValCur[pBusSlaveGenEni->m_byEEPROMValCntCur], pbyData, 4);
                                            pBusSlaveGenEni->m_byEEPROMValCntCur = (EC_T_BYTE)(pBusSlaveGenEni->m_byEEPROMValCntCur + 4);

                                            if (8 == pBusSlaveGenEni->m_byEEPROMValCntCur)
                                            {
                                            EC_T_BYTE byBitLen = pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_PDO_BITLEN];

                                                /* refresh sync manager information */
                                                if (0xFF != poPdoListCur->bySmIdx)
                                                {
                                                    if (ESC_SII_CAT_TXPDO == pBusSlaveGenEni->m_wEEPROMCategoryTypeCur)
                                                    {
                                                        pBusSlaveGenEni->m_aSmInpEntries[poPdoListCur->bySmIdx].wBitLen     = (EC_T_WORD)(pBusSlaveGenEni->m_aSmInpEntries[poPdoListCur->bySmIdx].wBitLen + byBitLen);
                                                        pBusSlaveGenEni->m_aSmInpEntries[poPdoListCur->bySmIdx].wMaxVarSize = (EC_T_WORD)EC_MAX(pBusSlaveGenEni->m_aSmInpEntries[poPdoListCur->bySmIdx].wMaxVarSize, byBitLen);
                                                    }
                                                    else
                                                    {
                                                        pBusSlaveGenEni->m_aSmOutEntries[poPdoListCur->bySmIdx].wBitLen     = (EC_T_WORD)(pBusSlaveGenEni->m_aSmOutEntries[poPdoListCur->bySmIdx].wBitLen + byBitLen);
                                                        pBusSlaveGenEni->m_aSmOutEntries[poPdoListCur->bySmIdx].wMaxVarSize = (EC_T_WORD)EC_MAX(pBusSlaveGenEni->m_aSmOutEntries[poPdoListCur->bySmIdx].wMaxVarSize, byBitLen);
                                                    }
                                                }
                                                /* fill PDO entry */
                                                {
                                                ECBT_PT_PDOENTRY poPdoEntryCur = &pBusSlaveGenEni->m_poPdoListCur->poEntries[pBusSlaveGenEni->m_poPdoListCur->byEntries];

                                                    poPdoEntryCur->wIndex     = EC_GET_FRM_WORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_PDO_INDEX]);
                                                    poPdoEntryCur->bySubIndex = pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_PDO_SUBINDEX];
                                                    poPdoEntryCur->byBitLen   = byBitLen;
                                                    poPdoListCur->byEntries++;
                                                }
                                                pBusSlaveGenEni->m_byPdoCntCur = (EC_T_BYTE)(pBusSlaveGenEni->m_byPdoCntCur - 1);

                                                OsMemset(&pBusSlaveGenEni->m_abyEEPROMValCur[0], 0, sizeof(pBusSlaveGenEni->m_abyEEPROMValCur));
                                                pBusSlaveGenEni->m_byEEPROMValCntCur = 0;
                                            }
                                            /* read next entry */
                                            pBusSlaveGenEni->m_wEEPROMAddrCur = (EC_T_WORD)(pBusSlaveGenEni->m_wEEPROMAddrCur + 2); /* DWORDwise */
                                        }
                                    } break;
                                    case ESC_SII_CAT_DC:
                                    {
                                        /* store current val */
                                        OsMemcpy(&pBusSlaveGenEni->m_abyEEPROMValCur[pBusSlaveGenEni->m_byEEPROMValCntCur], pbyData, 4);
                                        pBusSlaveGenEni->m_byEEPROMValCntCur = (EC_T_BYTE)(pBusSlaveGenEni->m_byEEPROMValCntCur + 4);

                                        if (24 == pBusSlaveGenEni->m_byEEPROMValCntCur)
                                        {
                                            pBusSlaveGenEni->m_wDcActivation = EC_GET_FRM_WORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_DC_REG_ACTIVATION]);
                                            
                                            pBusSlaveGenEni->m_dwDcSync0CycleTime       =  EC_GET_FRM_DWORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_DC_SYNC0_CYCLE_TIME]);

                                            pBusSlaveGenEni->m_aDcSync[0].dwShiftTime   =  EC_GET_FRM_DWORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_DC_SYNC0_SHIFT_TIME]);
                                            pBusSlaveGenEni->m_aDcSync[0].swCycleFactor = (EC_T_SWORD)EC_GET_FRM_WORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_DC_SYNC0_CYCLE_FACTOR]);

                                            pBusSlaveGenEni->m_aDcSync[1].dwShiftTime   = EC_GET_FRM_DWORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_DC_SYNC1_SHIFT_TIME]);
                                            pBusSlaveGenEni->m_aDcSync[1].swCycleFactor = (EC_T_SWORD)EC_GET_FRM_WORD((EC_T_WORD*)&pBusSlaveGenEni->m_abyEEPROMValCur[ESC_SII_CAT_DC_SYNC1_CYCLE_FACTOR]);

                                            OsMemset(&pBusSlaveGenEni->m_abyEEPROMValCur[0], 0, sizeof(pBusSlaveGenEni->m_abyEEPROMValCur));
                                            pBusSlaveGenEni->m_byEEPROMValCntCur = 0;

                                            /* read only first DC entry */
                                            bReadNextCategory = EC_TRUE;
                                        }
                                        /* read next entry */
                                        pBusSlaveGenEni->m_wEEPROMAddrCur = (EC_T_WORD)(pBusSlaveGenEni->m_wEEPROMAddrCur + 2); /* DWORDwise */
                                    } break;
                                    default:
                                        bReadNextCategory = EC_TRUE;
                                        break;
                                    }
                                    if (pBusSlaveGenEni->m_wEEPROMAddrCur > wEEPROMAddrLast)
                                    {
                                        bReadNextCategory = EC_TRUE;
                                    }
                                }
                                if (bReadNextCategory)
                                {
                                    if (ESC_SII_CAT_END == pBusSlaveGenEni->m_wEEPROMCategoryTypeCur)
                                    {
                                        /* done */
                                        pBusSlaveGenEni->m_wEEPROMAddrCur = 0;
                                    }
                                    else if (pBusSlaveGenEni->m_wEEPROMCategoryTypeCur >= 0x1000)
                                    {
                                        EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();
                                        if (EC_NULL != pNotification)
                                        {
                                            EC_T_SLAVE_NOTSUPPORTED_DESC* pSlaveNotSupported = &(pNotification->uNotification.ErrorNotification.desc.SlaveNotSupportedDesc);
                                            pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_SLAVE_NOTSUPPORTED;

                                            pBusSlave->GetSlaveProp(&pSlaveNotSupported->SlaveProp);

                                            m_pMaster->NotifyAndFree(EC_NOTIFY_SLAVE_NOTSUPPORTED, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_SLAVE_NOTSUPPORTED);

                                            EC_ERRORMSGC(("Slave not supported - Slave %s: - EtherCAT address=%d, please update EEPROM", pSlaveNotSupported->SlaveProp.achName, pSlaveNotSupported->SlaveProp.wStationAddress));
                                        }
                                        /* done */
                                        pBusSlaveGenEni->m_wEEPROMAddrCur = 0;
                                    }
                                    else
                                    {
                                        /* move to next category */                                        
                                        pBusSlaveGenEni->m_wEEPROMAddrCur         = (EC_T_WORD)(wEEPROMAddrFirst + pBusSlaveGenEni->m_wEEPROMCategorySizeCur);
                                        pBusSlaveGenEni->m_wEEPROMCategoryBaseCur = pBusSlaveGenEni->m_wEEPROMAddrCur;
                                    }
                                }
                            }
                            else
                            {
                                OsDbgAssert(EC_FALSE);
                            }
                        }
#endif /* INCLUDE_GEN_OP_ENI */
                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* Send activation command to second station address for addressing */
    case INVOKE_ID_SB_ACTIVATEALIAS:
        {
            OsDbgAssert(1 == m_dwEcatCmdPending);

            if (m_eCurSubState == ecbtsms_activatealias_wait)
            {
                if (1 <= wWorkingCnt)
                {
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert((m_dwBTResult == EC_E_TIMEOUT) || (m_dwBTResult == EC_E_LINK_DISCONNECTED));
            }
        } break;
    /* send identify command to detect next slave node on bus */
    case INVOKE_ID_SB_IDENTIFYSLAVE:
        {
            if (ecbtsm_checktopo_wait == m_eCurState && ecbtsmchk_process_wait == m_eCurChkState)
            {
                /* get bus slave */
                wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);

                /* set helper to recognize id request was processed */
                pBusSlave->m_wIdentificationAdo = EC_AL_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader);

                /* store read value in bus slave */
                pBusSlave->m_wIdentificationValue = 0;

                if (1 == wWorkingCnt)
                {
                    if (EC_CMD_TYPE_FPRD == EC_AL_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHeader))
                    {
                        if (ECREG_STATION_ADDRESS_ALIAS == EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader))
                        {
                            pBusSlave->SetAliasAddress(EC_GET_FRM_WORD(pbyData));
                            pBusSlave->m_wIdentificationAdo = EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader);
                            pBusSlave->m_wIdentificationValue = EC_GET_FRM_WORD(pbyData);
                        }
                        else if (ECREG_AL_STATUS == EC_AL_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader)
                            && 6 == EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHeader))
                        {
                            pBusSlave->SetAlStatus(EC_GET_FRM_WORD(pbyData));
                            pBusSlave->SetAlStatusCode(EC_GET_FRM_WORD(pbyData + 4));

                            /* explicit device identification handling */
                            if (0 != (EC_GET_FRM_WORD(pbyData) & DEVICE_STATE_IDREQUEST))
                            {
                                /* set identification ado and value for evaluation in IdentifySlave() */
                                pBusSlave->m_wIdentificationAdo = ECREG_AL_STATUSCODE;
                                pBusSlave->m_wIdentificationValue = EC_GET_FRM_WORD(pbyData + 4);
                                m_oIdentifyBusSlaveTimeout.Stop();
                            }
                            else
                            {
                                if (m_oIdentifyBusSlaveTimeout.IsElapsed())
                                {
                                    /* timeout while reading id value */
                                    pBusSlave->m_wIdentificationAdo = ECREG_AL_STATUSCODE;
                                    pBusSlave->m_wIdentificationValue = 0; /* id value = 0 is invalid */
                                    m_oIdentifyBusSlaveTimeout.Stop();
                                }
                                else
                                {
                                    /* try to read register again */
                                    pBusSlave->m_wIdentificationAdo = ECREG_AL_STATUS;
                                }
                            }
                        }
                        else
                        {
                            /* set identification ado and value for evaluation in IdentifySlave() */
                            pBusSlave->m_wIdentificationAdo = EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader);
                            pBusSlave->m_wIdentificationValue = EC_GET_FRM_WORD(pbyData);
                        }
                    }
                }
                else
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "INVOKE_ID_SB_IDENTIFYSLAVE WKC = %d", wWorkingCnt));
                }
                /* mark command as processed */
                m_dwEcatCmdProcessed++;
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult || EC_E_NOTREADY == m_dwBTResult);
            }
        } break;
    /* Read out PDI Control to know about Power safe mode etc. of slave node */
    case INVOKE_ID_SB_PDICONTROL:
        {
            if (m_eCurSubState == ecbtsms_readpdicontrol_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        pBusSlave->SetPdiControl(EC_GET_FRM_WORD(pbyData));

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;

#if (defined INCLUDE_DC_SUPPORT)
    /* probe DC unit */
    case INVOKE_ID_SB_PROBEDCUNIT:
        {
            if (m_eCurSubState == ecbtsms_probedcunit_wait)
            {
                /* get bus slave */
                wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                if (EC_NULL != pBusSlave)
                {
                    pBusSlave->SetDcUnitSupported(0 != wWorkingCnt);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert( EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult );
            }
        } break;
#endif /* INCLUDE_DC_SUPPORT */

    /* write temporary slave fixed address to ensure consistency while performing BT data collection */
    case INVOKE_ID_SB_WRTTMPSLVADDR:
        {
            if (m_eCurSubState == ecbtsms_writetmpaddr_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* get autoincrement address from header */
                    wAutoIncAddress = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - m_dwSlavesAtStart);

                    /* get fixed address */
                    wFixedAddress = EC_GET_FRM_WORD(pbyData);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* check if write of temporary slave fixed address was successful */
    case INVOKE_ID_SB_CHECKTMPADDRESS:
        {
            if (ecbtsms_chkwrttmpaddr_wait == m_eCurSubState)
            {
                if (0 == wWorkingCnt)
                {
                    m_dwErrorCur = EC_E_SLAVE_NOT_ADDRESSABLE;
                    break;
                }
                if (wWorkingCnt > 1)
                {
                    OsDbgAssert(EC_FALSE);
                    m_dwErrorCur = EC_E_DUPLICATE;
                    break;
                }
                /* validate autoincrement address */
                if ((EC_T_WORD)((EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - (EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pSlaveCmd->pEcCmdHeader)) != (EC_T_WORD)m_dwSlavesAtStart)
                {
                    break;
                }
                /* get autoincrement and temporary fixed address from header */
                wAutoIncAddress  = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - m_dwSlavesAtStart);
                wTmpFixedAddress = EC_GET_FRM_WORD(pbyData);

                /* get bus slave */
                pBusSlave = GetBusSlaveByAutoIncAddress(wAutoIncAddress);
                if (EC_NULL == pBusSlave)
                {
                    m_dwErrorCur = EC_E_NOTFOUND;
                    break;
                }
                /* mark command as processed */
                m_dwEcatCmdProcessed++;

                /* validate tmp fixed address for bus slave (could be wrong if autoinc address change -> wrong bus slave) */
                if (wTmpFixedAddress != pBusSlave->GetTmpFixedAddress())
                {
                    m_dwErrorCur = EC_E_BUSCONFIG_TOPOCHANGE;
                    break;
                }
                /* store (temporary) fixed address */
                pBusSlave->SetFixedAddress(wTmpFixedAddress);
                m_pBusSlavesByFixedAddress->Add(pBusSlave->GetFixedAddress(), pBusSlave);
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;

    case INVOKE_ID_SB_CHECKDUPLICATE:
        {
            if ((ecbtsms_chkwrttmpaddr_wait == m_eCurSubState) || (ecbtsms_checkfixedaddress_wait == m_eCurSubState))
            {
                /* check working counter */
                if (0 == wWorkingCnt)
                {
                    m_dwErrorCur = EC_E_SLAVE_NOT_ADDRESSABLE;
                    break;
                }
                if (wWorkingCnt > 1)
                {
                    m_dwErrorCur = EC_E_DUPLICATE;
                    break;
                }
                /* mark command as processed */
                m_dwEcatCmdProcessed++;
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* read DL Control register to check how port's states are, to have information about topology */
    case INVOKE_ID_SB_RDPORTCONTROL:
        {
            if (m_eCurSubState == ecbtsms_readportctrl_wait)
            {
                if (1 == wWorkingCnt)
                {
                EC_T_DWORD dwDlControl = EC_GET_FRM_DWORD(pbyData);

                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        /* store DL Control value */
                        pBusSlave->SetDlControl(dwDlControl);

                        /* dump some register values for EcBusTopology_tests */
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                            "{0x%02X, 0x%02X, 0x%02X, 0x%02X}, /* AutoInc 0x%04X ESC:0x0100 */\n", 
                            pbyData[0],  pbyData[1], pbyData[2], pbyData[3],
                            pBusSlave->GetAutoIncAddress()));

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* write DL Control register to correct Port configuration to a valid mode */
    case INVOKE_ID_SB_WRTPORTCONTROL:
        {
            if (m_eCurSubState == ecbtsms_writeportctrl_wait)
            {
                if (1 == wWorkingCnt)
                {
                EC_T_DWORD dwDlControl = EC_GET_FRM_DWORD(pbyData);

                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        /* store DL Control value */
                        pBusSlave->SetDlControl(dwDlControl);

                        /* dump some register values for EcBusTopology_tests */
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                            "{0x%02X, 0x%02X, 0x%02X, 0x%02X}, /* AutoInc 0x%04X ESC:0x0100 */\n", 
                            pbyData[0],  pbyData[1], pbyData[2], pbyData[3],
                            pBusSlave->GetAutoIncAddress()));

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* send a NOP command in a single frame to open an eventually closed port 0 */
    case INVOKE_ID_SB_OPENCLOSEDPORTA:
        {
            /* nothing to do */
        } break;
    /* write slave fixed address to slaves (before performing border close) */
    case INVOKE_ID_SB_WRTSLVFIXADDR:
        {
            if (m_eCurSubState == ecbtsms_writefixaddr_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* get temporary fixed address from header */
                    wTmpFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);

                    /* get fixed address */
                    wFixedAddress = EC_GET_FRM_WORD(pbyData);

                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* check if write of slave fixed address was successful */
    case INVOKE_ID_SB_CHECKFIXEDADDRESS:
        {
            if (m_eCurSubState == ecbtsms_checkfixedaddress_wait)
            {
                if (0 == wWorkingCnt)
                {
                    m_dwErrorCur = EC_E_SLAVE_NOT_ADDRESSABLE;
                    break;
                }
                /* validate autoincrement address */
                if ((EC_T_WORD)((EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - (EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pSlaveCmd->pEcCmdHeader)) != (EC_T_WORD)m_dwSlavesAtStart)
                {
                    break;
                }
                /* get autoincrement and fixed address from header */
                wAutoIncAddress = (EC_T_WORD)(EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader) - m_dwSlavesAtStart);
                wFixedAddress   = EC_GET_FRM_WORD(pbyData);

                /* get bus slave */
                pBusSlave = GetBusSlaveByAutoIncAddress(wAutoIncAddress);
                if (EC_NULL == pBusSlave)
                {
                    m_dwErrorCur = EC_E_NOTFOUND;
                    break;
                }
                /* mark command as processed */
                m_dwEcatCmdProcessed++;

                /* validate tmp fixed address for bus slave (could be wrong if autoinc address change -> wrong bus slave) */
                pCfgSlave = pBusSlave->GetCfgSlave();
                if (EC_NULL == pCfgSlave)
                {
                    m_dwErrorCur = EC_E_BUSCONFIG_TOPOCHANGE;
                    break;
                }
                /* validate tmp fixed address for bus slave (could be wrong if autoinc address change -> wrong bus slave) */
                if (wFixedAddress != pCfgSlave->GetCfgFixedAddr())
                {
                    m_dwErrorCur = EC_E_BUSCONFIG_TOPOCHANGE;
                    break;
                }
                /* store fixed address, set temporary fixed address to same value because temporary fixed address should not be used anymore */
                m_pBusSlavesByFixedAddress->Remove(pBusSlave->GetTmpFixedAddress());
                pBusSlave->SetFixedAddress(wFixedAddress);
                pBusSlave->SetTmpFixedAddress(wFixedAddress);
                m_pBusSlavesByFixedAddress->Add(pBusSlave->GetFixedAddress(), pBusSlave);
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;

    case INVOKE_ID_SB_READEVENTREGISTER:
        {
            if (m_eCurSubState == ecbtsms_findintsource_wait)
            {
                /* get bus slave */
                wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                if (EC_NULL != pBusSlave)
                {
                    if      (1 == wWorkingCnt)
                    {
                        /* if EtherCAT event not supported read AL_STATUS instead */
                        if (ECREG_SLV_ECATEVENTREQUEST == EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader))
                        {
                            /* refresh EtherCAT event information */
                            pBusSlave->SetEventRegVal(EC_GET_FRM_WORD(pbyData));
                        }
                        /* validate slave presence */
                        pCfgSlave = pBusSlave->GetCfgSlave();
                        if (EC_NULL != pCfgSlave)
                        {
                            pCfgSlave->SetSlavePresence(EC_TRUE);
                        }
                    }
                    else if (0 == wWorkingCnt)
                    {
                        /* reset DL status event to protect for endless loop in ecbtsms_ackinterrupt  */
                        /* preventing scan bus timeout condition                                      */
                        pBusSlave->ResetDLStatusEvent();

                        /* set topology changed flag */
                        SetTopologyChanged();
                    }
                    else
                    {
                        /* a new slave has been connected to a slave not supporting autoclose  */
                        /* and his physical address already exist on the bus, trigger bus scan */
                        SetTopologyChanged();
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;

    case INVOKE_ID_SB_READEVENTMASK:
        {
            if (m_eCurSubState == ecbtsms_readinterrupt_wait)
            {
                if      (1 == wWorkingCnt)
                {
                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        pBusSlave->SetEcatEventMask(EC_GET_FRM_WORD(pbyData));

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
                else if (0 == wWorkingCnt)
                {
                    if (0 == m_dwEcatCmdRetry)
                    {
                        /* get bus slave */
                        wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                        pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                        if (EC_NULL != pBusSlave)
                        {
                            pBusSlave->SetEcatEventSupported(EC_FALSE);

                            /* mark command as processed */
                            m_dwEcatCmdProcessed++;
                        }
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;

    case INVOKE_ID_SB_WRITEEVENTMASK:
        {
            if (m_eCurSubState == ecbtsms_cfginterrupt_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* get bus slave */
                    wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        if (ECREG_SLV_ECATEVENTMASK == EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHeader))
                        {
                            pBusSlave->SetEcatEventMask(EC_GET_FRM_WORD(pbyData));
                        }
                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;

    case INVOKE_ID_SB_READALSTATUS:
        {
            if (m_eCurSubState == ecbtsms_readalstatus_wait)
            {
                if (1 == wWorkingCnt)
                {
                EC_T_WORD wAlStatus = EC_GET_FRM_WORD(pbyData);
                EC_T_WORD wAlStatusCode = EC_GET_FRM_WORD((pbyData+(ECREG_AL_STATUSCODE_LO-ECREG_AL_STATUS_LO)));

                    /* get bus slave */
                    wFixedAddress = (EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        /* refresh bus slave */
                        pBusSlave->SetAlStatus(wAlStatus);
                        if (DEVICE_STATE_ERROR & wAlStatus)
                        {
                            /* set AlStatusCode only in case of error to not overwrite the last code */
                            /* AlStatusCode will be reset on state change by the slave state machine */
                            pBusSlave->SetAlStatusCode(wAlStatusCode);
                        }

                        /* get config slave */
                        pCfgSlave = pBusSlave->GetCfgSlave();
                        if (EC_NULL != pCfgSlave)
                        {
                            /* validate slave presence */
                            pCfgSlave->SetSlavePresence(EC_TRUE);

                            /* notify application if slave state request pending or finished */
                            if (pCfgSlave->GetSmReqDeviceState() != DEVICE_STATE_UNKNOWN)
                            {
                                if (DEVICE_STATE_ERROR & wAlStatus)
                                {
                                    m_pMaster->NotifySlaveError(pCfgSlave, wAlStatus, wAlStatusCode);
                                }
                                else
                                {
                                    if (pCfgSlave->GetSmReqDeviceState() != pCfgSlave->GetDeviceState())
                                    {
                                        m_pMaster->NotifySlaveUnexpectedState(pCfgSlave, pCfgSlave->GetSmReqEcatState());
                                    }
                                }
                            }
                            m_pMaster->RecalculateCycCmdWkc(pCfgSlave);
                        }
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* acknowledge slave error */
    case INVOKE_ID_SB_ACKSLVERROR:
        {
            if (m_eCurSubState == ecbtsms_ackslaveerror_wait)
            {
                if (1 == wWorkingCnt)
                {
                EC_T_WORD wAlStatus = EC_GET_FRM_WORD(pbyData);

                    /* get bus slave */
                    wFixedAddress = (EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        /* refresh AL Status (error bit is cleared) */
                        pBusSlave->SetAlStatus((EC_T_WORD)(wAlStatus & ~DEVICE_STATE_ERROR));
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert((EC_E_TIMEOUT == m_dwBTResult) || (EC_E_NOMEMORY == m_dwBTResult));
            }
        } break;
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
    /* Latch receive times on all slaves to detect crossed lines */
    case INVOKE_ID_SB_LATCHRECVTIMES:
        {
            OsDbgAssert(1 == m_dwEcatCmdPending);

            if (ecbtsms_latchrecvtimes_wait == m_eCurSubState)
            {
                /* always mark command as processed in case no slave supports RecvTimes */
                m_dwEcatCmdProcessed++;
            }
            else
            {
                OsDbgAssert((m_dwBTResult == EC_E_TIMEOUT) || (m_dwBTResult == EC_E_LINK_DISCONNECTED));
            }
        } break;
    /* read receive times from slaves */
    case INVOKE_ID_SB_READRECVTIMES:
        {
            if (m_eCurSubState == ecbtsms_readrecvtimes_wait)
            {
                /* get bus slave */
                wFixedAddress = EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                if (EC_NULL != pBusSlave)
                {
                    if (0 == wWorkingCnt)
                    {
                        if (0 == m_dwEcatCmdRetry)
                        {
                            pBusSlave->SetRecvTimesSupported(EC_FALSE);

                            /* mark command as processed */
                            m_dwEcatCmdProcessed++;
                        }
                    }
                    else if (1 == wWorkingCnt)
                    {
                        pBusSlave->SetRecvTimesSupported(EC_TRUE);
                        pBusSlave->SetRecvTimes(pbyData, m_pMaster);

                        /* dump some register values for EcBusTopology_tests */
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                            "{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n"\
                            " 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}, /* AutoInc 0x%04X ESC:0x0900 */\n", 
                            pbyData[0],  pbyData[1], pbyData[2], pbyData[3], pbyData[4], pbyData[5], pbyData[6], pbyData[7], pbyData[8], pbyData[9], pbyData[10], pbyData[11], pbyData[12], pbyData[13], pbyData[14], pbyData[15], 
                            pbyData[16], pbyData[17], pbyData[18], pbyData[19], pbyData[20], pbyData[21], pbyData[22], pbyData[23], pbyData[24], pbyData[25], pbyData[26], pbyData[27], pbyData[28], pbyData[29], pbyData[30], pbyData[31],
                            pBusSlave->GetAutoIncAddress()));

                        /* mark command as processed */
                        m_dwEcatCmdProcessed++;
                    }                    
                }
            }
            else
            {
                OsDbgAssert((EC_E_TIMEOUT == m_dwBTResult) || (EC_E_NOMEMORY == m_dwBTResult));
            }
        } break;
#endif /* INCLUDE_DC_SUPPORT || INCLUDE_LINE_CROSSED_DETECTION */

    /* clean pending explicit device ID handshake */
    case INVOKE_ID_SB_CHKCLEAN:
        {
            if( m_eCurChkState == ecbtsmchk_clean_wait)
            {
                if (1 == wWorkingCnt)
                {
                    /* get bus slave */
                    wFixedAddress = (EC_T_WORD)EC_ICMDHDR_GET_ADDR_ADP(pEcCmdHeader);
                    pBusSlave = GetBusSlaveByFixedAddress(wFixedAddress);
                    if (EC_NULL != pBusSlave)
                    {
                        pBusSlave->SetAlStatus((EC_T_WORD)(pBusSlave->GetAlStatus() & ~DEVICE_STATE_IDREQUEST));
                    }
                    /* mark command as processed */
                    m_dwEcatCmdProcessed++;
                }
            }
            else
            {
                OsDbgAssert(EC_E_TIMEOUT == m_dwBTResult || EC_E_NOMEMORY == m_dwBTResult);
            }
        } break;
    /* acyclic AL status BRD if no cyclic information is available */
    case INVOKE_ID_SB_ACYCALSTATUSBRD:
        {
            m_wAcycBrdAlStatusWkc = wWorkingCnt;
        } break;
    /* dummy invoke id to mark highest invoke id reserved for BT Scan */
    case INVOKE_ID_SB_MAX:
        {
            /* shall not be used right now */
            OsDbgAssert(EC_FALSE);
        } break;
    /* default value, if unknown invoke id is received */
    default:
        {
            OsDbgAssert(EC_FALSE);
        } break;
    }

Exit:
    switch (dwInvokeId)
    {
    /* these commands are not monitored */
    case INVOKE_ID_SB_OPENCLOSEDPORTA:
    case INVOKE_ID_SB_ACYCALSTATUSBRD:
        break;
    default:
    {
        /* decrement pending commands counter */
        OsDbgAssert(m_dwEcatCmdProcessed <= m_dwEcatCmdSent);
        OsDbgAssert(0 != m_dwEcatCmdPending);
        if (0 != m_dwEcatCmdPending)
        {
            m_dwEcatCmdPending--;
        }
    } break;
    }
    return;
}

/***************************************************************************************************/
/**
\brief  Record bus slaves
        AssignTmpFixedAddress() are sub functions of RecordBusSlave()

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::RecordBusSlavePrepare(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES) && (defined INCLUDE_RED_DEVICE)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
    m_pBusSlaveCur = m_pBusSlaveRoot;

#if (defined INCLUDE_RED_DEVICE)
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "/* m_dwSlavesAtStart = %d, m_bLineBreakAtStart = %d, m_wSlavesCntOnMainAtStart = %d, m_wSlavesCntOnRedAtStart = %d */\n", m_dwSlavesAtStart, m_bLineBreakAtStart, m_wSlavesCntOnMainAtStart, m_wSlavesCntOnRedAtStart));
#endif
    return EC_E_NOERROR;
}

EC_T_VOID  CEcBusTopology::AssignTmpFixedAddress(CEcBusSlave* pBusSlave)
{
EC_T_WORD wFixedAddress    = pBusSlave->GetFixedAddress();
EC_T_WORD wTmpFixedAddress = TakeTmpFixedAddress();

    pBusSlave->SetTmpFixedAddress(wTmpFixedAddress);
    if (wFixedAddress == wTmpFixedAddress)
    {
        m_pBusSlavesByFixedAddress->Remove(wFixedAddress);
        m_pBusSlavesByFixedAddress->Add(wFixedAddress, pBusSlave);
    }
}

EC_T_DWORD CEcBusTopology::RecordBusSlave(EC_T_WORD wAutoIncAddress, EC_T_BYTE* pbyData)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_DWORD   dwRetVal      = EC_E_ERROR;
EC_T_WORD    wFixedAddress = EC_GET_FRM_WORD(&(pbyData[0x10]));
EC_T_DWORD   dwBusIndex    = EC_MAKEDWORD(0, ((EC_T_WORD)0-wAutoIncAddress));
CEcBusSlave* pBusSlave     = GetBusSlaveByFixedAddress(wFixedAddress);
CEcBusSlave* pBusSlaveDuplicate = EC_NULL;
CEcSlave*    pCfgSlave     = EC_NULL;

    /* dump some register values for EcBusTopology_tests */
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
        "{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}, /* AutoInc 0x%04X ESC:0x0000 */\n", 
        pbyData[0],  pbyData[1], pbyData[2], pbyData[3], pbyData[4], pbyData[5], pbyData[6], pbyData[7], pbyData[8], pbyData[9],
        pbyData[10], pbyData[11], pbyData[12], pbyData[13], pbyData[14], pbyData[15], pbyData[16], pbyData[17], pbyData[18], pbyData[19],
        wAutoIncAddress));

    /* check if bus slave match */
    if ((EC_NULL != pBusSlave) && (EC_NULL != pBusSlave->GetCfgSlave()) && pBusSlave->AreDlInfoEqual(pbyData))
    {
        /* detect bus slave duplicate */
        if (EC_E_INVALIDDATA != pBusSlave->GetScanBusStatus())
        {
            /* check if duplicate was already detected */
            pBusSlaveDuplicate = pBusSlave->m_pBusSlaveDuplicate;
            if (EC_NULL == pBusSlaveDuplicate)
            {
            CEcBusSlave* pBusSlavePrev = pBusSlave->GetPrev();

                /* remove first instance */
                pBusSlaveDuplicate = pBusSlave;
                pBusSlaveDuplicate->m_pBusSlaveDuplicate = pBusSlaveDuplicate;
                RemoveBusSlaveFromList(pBusSlaveDuplicate);

                /* add to pending duplicates */
                pBusSlaveDuplicate->SetNext(m_pBusSlaveDuplicate);
                m_pBusSlaveDuplicate = pBusSlaveDuplicate;

                /* create new bus slave */
                pBusSlave = NewBusSlave();
                if (EC_NULL == pBusSlave)
                {
                    OsDbgAssert(EC_FALSE);
                    dwRetVal = EC_E_MAX_BUS_SLAVES_EXCEEDED;
                    goto Exit;
                }
                pBusSlave->m_pBusSlaveDuplicate = pBusSlaveDuplicate;

                /* substitute first instance */
                InsertBusSlaveInListAfter(pBusSlavePrev, pBusSlave);

                /* refresh bus index */
                pBusSlave->SetBusIndex(pBusSlaveDuplicate->GetBusIndex());
                m_pBusSlavesByIndex->Remove(pBusSlaveDuplicate->GetBusIndex());
                m_pBusSlavesByIndex->Add(pBusSlaveDuplicate->GetBusIndex(), pBusSlave);

                /* assign temp adress */
                AssignTmpFixedAddress(pBusSlave);
            }
            /* request new bus slave */
            pBusSlave = EC_NULL;
        }
    }
    else
    {
        /* request new bus slave */
        pBusSlave = EC_NULL;
    }
    /* create new bus slave */
    if (EC_NULL == pBusSlave)
    {
        /* create new bus slave */
        pBusSlave = NewBusSlave();
        if (EC_NULL == pBusSlave)
        {
            dwRetVal = EC_E_MAX_BUS_SLAVES_EXCEEDED;
            goto Exit;
        }
        pBusSlave->m_pBusSlaveDuplicate = pBusSlaveDuplicate;

        if (EC_NULL == m_pBusSlaveCur)
        {
        CEcBusSlave* pBusSlavePrev = EC_NULL;

            m_pBusSlavesByIndex->Lookup((m_dwBusSlaves - 1), pBusSlavePrev);
            InsertBusSlaveInListAfter(pBusSlavePrev, pBusSlave);
        }
        else
        {
            InsertBusSlaveInListBefore(m_pBusSlaveCur, pBusSlave);
        }
    }
    /* consider current bus slave */
    m_pBusSlaveCur = pBusSlave;

    /* refresh bus index */
    if (m_pBusSlavesByIndex->Lookup(dwBusIndex, pBusSlave) && (m_pBusSlaveCur != pBusSlave))
    {
        /* invalidate bus index of the "moved" bus slave */
        OsDbgAssert(EC_E_INVALIDDATA == pBusSlave->GetScanBusStatus());
        pBusSlave->SetBusIndex(INVALID_BUS_INDEX);
    }
    m_pBusSlavesByIndex->Remove(m_pBusSlaveCur->GetBusIndex());
    m_pBusSlavesByIndex->Add(dwBusIndex, m_pBusSlaveCur);
    m_pBusSlaveCur->SetBusIndex(dwBusIndex);

    /* validate slave presence */
    pCfgSlave = m_pBusSlaveCur->GetCfgSlave();
    if (EC_NULL != pCfgSlave)
    {
        pCfgSlave->SetSlavePresence(EC_TRUE);
    }
    /* assign temporary fixed addresses to unknown slave or if configured fixed address was not set */
    if ((EC_NULL == pCfgSlave) || (m_pBusSlaveCur->GetFixedAddress() != pCfgSlave->GetFixedAddr()))
    {
        AssignTmpFixedAddress(m_pBusSlaveCur);
    }
    /* refresh bus slave information */
    m_pBusSlaveCur->SetDlInfo(pbyData);

    /* refresh scan bus status */
    m_pBusSlaveCur->SetScanBusStatus(EC_E_NOTREADY);

    /* expect next bus slave */
    m_pBusSlaveCur = m_pBusSlaveCur->GetNext();

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Build topology information based on bus slaves
        CleanDisconnectedPorts() ... LookForParentBusSlave() are sub functions of BuildBusTopology()

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::BuildBusTopologyPrepare(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif

    m_pBusSlaveCur               = m_pBusSlaveRoot;
    m_pBusSlaveTopoCur           = EC_NULL;
    SetBusSlaveBranchingCur(EC_FALSE, EC_NULL, ESC_PORT_INVALID, m_pBusSlaveCur);

#if (defined INCLUDE_RED_DEVICE)
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BuildBusTopologyPrepare() m_dwSlavesAtStart = %d, m_bLineBreakAtStart = %d, m_wSlavesCntOnMainAtStart = %d, m_wSlavesCntOnRedAtStart = %d, m_bIsSendEnabledOnMainAtStart = %d m_bIsSendEnabledOnRedAtStart = %d\n", m_dwSlavesAtStart, m_bLineBreakAtStart, m_wSlavesCntOnMainAtStart, m_wSlavesCntOnRedAtStart, m_bIsSendEnabledOnMainAtStart, m_bIsSendEnabledOnRedAtStart));
    if (m_bLineBreakAtStart)
    {
        m_pBusSlaveTopoMainRoot = (m_wSlavesCntOnMainAtStart == 0)?EC_NULL:m_pBusSlaveRoot;
        m_pBusSlaveTopoRedRoot  = (m_wSlavesCntOnMainAtStart == (EC_T_WORD)m_dwSlavesAtStart)?EC_NULL:GetBusSlaveByIndex(m_wSlavesCntOnMainAtStart);
    }
    else
    {
        m_pBusSlaveTopoMainRoot = m_pBusSlaveRoot;
        m_pBusSlaveTopoRedRoot  = EC_NULL;
    }
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    if (!m_bRedEnhancedLineCrossedDetectionEnabled || m_bLineBreakAtStart)
#endif
    {
        /* redundancy port determined in BuildBusTopology() */
        m_pBusSlaveRedPort      = EC_NULL;
        m_wRedPort              = ESC_PORT_INVALID;
    }
#else
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BuildBusTopologyPrepare() m_dwSlavesAtStart = %d", m_dwSlavesAtStart));
    m_pBusSlaveTopoMainRoot = m_pBusSlaveRoot;
#endif /* INCLUDE_RED_DEVICE */

    return EC_E_NOERROR;
}
EC_T_VOID CEcBusTopology::BuildBusTopologyPrepareSlave(CEcBusSlave* pBusSlave)
{
EC_T_WORD wPortIdx = 0;

    /* reset processed ports flags */
    pBusSlave->ResetPortsSmProc();

    /* reset topology information */
    pBusSlave->m_wInputPort    = ESC_PORT_INVALID;
    pBusSlave->m_wParentPort   = ESC_PORT_INVALID;
    pBusSlave->m_wPortAtParent = ESC_PORT_INVALID;
    OsMemset(&pBusSlave->m_apBusSlaveChildren[0], 0, (sizeof(CEcBusSlave*) * ESC_PORT_COUNT));
    pBusSlave->SetLineCrossed(EC_FALSE);
    pBusSlave->SetPortANotConnected(EC_FALSE);
    pBusSlave->ResetEL9010Connected();

#if (defined INCLUDE_RED_DEVICE)
    if (pBusSlave == m_pBusSlaveTopoRedRoot)
    {
        m_pawExpectedProcessingPortOrder = c_awBackwardPortOrder;

        /* bus slave at line break is branching if more than 1 ports are connected and port A not */
        if (!pBusSlave->IsSlaveConPortX(ESC_PORT_A))
        {
            if (1 < pBusSlave->m_byConnectedPortCnt)
            {
                pBusSlave->SetBranchingFlag(EC_TRUE);
            }
        }
    }
    if (pBusSlave == m_pBusSlaveRedPort)
    {
        /* prevent to link bus slave to redundancy port */
        pBusSlave->SetPortSmProc(m_wRedPort);
    }
#endif /* INCLUDE_RED_DEVICE */

    /* mark not connected ports as processed and initialize input port    */
    /* initialize input port to first connected port in processing order  */
    /* line crossed detection assume this order and can modify input port */
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_WORD wPortCur = m_pawExpectedProcessingPortOrder[wPortIdx];

        if (pBusSlave->IsSlaveConPortX(wPortCur))
        {
            if ((ESC_PORT_INVALID == pBusSlave->m_wInputPort) || IsRedPort(pBusSlave, pBusSlave->m_wInputPort))
            {
                pBusSlave->m_wInputPort = wPortCur;
            }
        }
        else
        {
            /* mark port as processed */
            pBusSlave->SetPortSmProc(wPortCur);
        }
    }
    OsDbgAssert(ESC_PORT_INVALID != pBusSlave->m_wInputPort);

    /* initialized parent port */
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_WORD wPortCur = c_awCloseToProcessingUnitOrder[wPortIdx];

        if (pBusSlave->IsSlaveConPortX(wPortCur) && !IsRedPort(pBusSlave, wPortCur))
        {
            /* initialize parent port to connected port close to processing unit  */
            /* line crossed detection assume this order and can modify input port */
            pBusSlave->m_wParentPort = wPortCur;
            break;
        }
    }
    OsDbgAssert(ESC_PORT_INVALID != pBusSlave->m_wParentPort);
}
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
EC_T_VOID CEcBusTopology::DetectLineCrossed(CEcBusSlave* pBusSlave)
{
EC_T_WORD wPortIdx = 0;
EC_T_DWORD dwMaxDelay = 0;

    /* determine max delay value to validate port receive times, e.g.  */
    /* in case of junction redundancy, time information is not latched */
    /* at all ports see JUNCTIONRED in EcBusTopology_tests             */
    dwMaxDelay = m_pMaster->GetBusCycleTimeUsec() * 1000;
    if (0 == dwMaxDelay)
    {
        dwMaxDelay = 1000000;
    }
    if (pBusSlave->IsRecvTimesSupported())
    {
        /* calculate port delays and adjust eventually input port */
        pBusSlave->ResetPortDelays();
        pBusSlave->CalculatePortDelays();

#if (defined INCLUDE_RED_DEVICE)
        if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart <= pBusSlave->GetBusIndex()))
        {
            if (EC_NULL != m_pBusSlaveBranchingCur)
            {
                if (pBusSlave->GetSlaveDelay() > m_pBusSlaveBranchingCur->GetSlaveDelay())
                {
                    SetBusSlaveBranchingCur(EC_FALSE, EC_NULL, ESC_PORT_INVALID, pBusSlave);
                }
            }
        }
#endif /* INCLUDE_RED_DEVICE */

        /* detect line crossed */
        for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
        {
        EC_T_WORD wPortCur;

            if (!pBusSlave->IsSlaveConPortX(ESC_PORT_A))
            {
                wPortCur = c_awCloseToProcessingUnitOrder[wPortIdx];
            }
            else
            {
                wPortCur = m_pawExpectedProcessingPortOrder[wPortIdx];
            }
            if (pBusSlave->IsSlaveConPortX(wPortCur))
            {
                /* line crossed if input port is not the expected one */
                if (wPortCur != pBusSlave->m_wInputPort)
                {
                    pBusSlave->SetLineCrossed(EC_TRUE);
                }
                break;
            }
        }
        /* detect connection to EL9010 */
        for (wPortIdx = 1; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
        {
        EC_T_WORD wPortCur = m_pawExpectedProcessingPortOrder[wPortIdx];

            if (pBusSlave->IsSlaveConPortX(wPortCur))
            {
                /* 150 measured against EL9010, 300 against ELxxxx */
                if ((ECM_PORT_EBUS == pBusSlave->GetPhysicPortX(wPortCur)) && (0 < pBusSlave->m_adwPortDelay[wPortCur]) && (200 > pBusSlave->m_adwPortDelay[wPortCur]))
                {
                    pBusSlave->SetEL9010ConnectedX(wPortCur, EC_TRUE);
                    pBusSlave->SetPortSmProc(wPortCur);
                }
            }
        }
    }
    /* check for not connected port A */
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_WORD wPortCur = m_pawExpectedProcessingPortOrder[wPortIdx];

        if (pBusSlave->IsSlaveCfgPortX(wPortCur))
        {
            if  (!pBusSlave->IsSlaveConPortX(wPortCur))
            {
                if (m_bNotifyNotConnectedPortA)
                {
                    pBusSlave->SetPortANotConnected(EC_TRUE);
                }
            }
            break;
        }
    }
}
#endif /* INCLUDE_LINE_CROSSED_DETECTION */

EC_T_WORD CEcBusTopology::GetLastConnectedPortAfterPort(CEcBusSlave* pBusSlave, EC_T_WORD wAfterPort)
{
EC_T_WORD wPortIdx      = 0;
EC_T_WORD wPortIdxStart = 0;
EC_T_WORD wLastConnectedPortAfterPort = wAfterPort;

    /* get port index of parent port */
    for (wPortIdxStart = 0; wPortIdxStart < ESC_PORT_COUNT; wPortIdxStart++)
    {
        if (wAfterPort == CEcBusTopology::c_awFrameProcessingPortOrder[wPortIdxStart])
        {
            break;
        }
    }
    /* go through ports following the expected propagation order starting with parent port */
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_WORD wPortCur = CEcBusTopology::c_awFrameProcessingPortOrder[(EC_T_DWORD)(wPortIdxStart + wPortIdx) % ESC_PORT_COUNT];

        if (pBusSlave->IsSlaveConPortX(wPortCur) && !IsRedPort(pBusSlave, wPortCur))
        {
            wLastConnectedPortAfterPort = wPortCur;
        }
    }
    return wLastConnectedPortAfterPort;
}
EC_T_VOID CEcBusTopology::LinkToParentBusSlave(CEcBusSlave* pBusSlave, EC_T_WORD wParentPort, CEcBusSlave* pBusSlaveParent, EC_T_WORD wPortAtParent)
{
    pBusSlave->m_wParentPort = wParentPort;

    if (ESC_PORT_INVALID != wParentPort)
    {
        pBusSlave->m_apBusSlaveChildren[wParentPort] = pBusSlaveParent;
        pBusSlave->SetPortSmProc(wParentPort);
    }
    if (EC_NULL != pBusSlaveParent)
    {
        if (ESC_PORT_INVALID != wPortAtParent)
        {
            pBusSlaveParent->m_apBusSlaveChildren[wPortAtParent] = pBusSlave;
            pBusSlaveParent->SetPortSmProc(wPortAtParent);
        }
        else
        {
            /* should never happen */
            OsDbgAssert(EC_FALSE);
        }
    }
    else
    {
        wPortAtParent = ESC_PORT_INVALID;
    }
    pBusSlave->m_wPortAtParent = wPortAtParent;
}
EC_T_VOID CEcBusTopology::StartNewTopology(CEcBusSlave* pBusSlave)
{
    m_pBusSlaveTopoCur = pBusSlave;

    if (m_pBusSlaveTopoCur->IsBranching())
    {
    EC_T_WORD wPortBranchingCur = ESC_PORT_INVALID;

        GetNextBusSlaveByPropagation(m_pBusSlaveTopoCur, m_pBusSlaveTopoCur->m_wInputPort, EC_FALSE, &wPortBranchingCur, EC_NULL, EC_NULL);
        SetBusSlaveBranchingCur(EC_FALSE, m_pBusSlaveTopoCur, wPortBranchingCur, EC_NULL);
    }
    else
    {
        SetBusSlaveBranchingCur(EC_FALSE, EC_NULL, ESC_PORT_INVALID, m_pBusSlaveTopoCur);
    }
}
EC_T_VOID CEcBusTopology::SetBusSlaveBranchingCur(EC_T_BOOL bBack, CEcBusSlave* pBusSlaveBranchingCur, EC_T_WORD wPortBranchingCur, CEcBusSlave* pBusSlaveCur)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
    /* set new branching slave */
    if (EC_NULL != m_pBusSlaveBranchingCur)
    {
        if (bBack)
        {
            m_wPortBranchingCur     = m_pBusSlaveBranchingCur->m_wPortBranchingLast;
            m_pBusSlaveBranchingCur = m_pBusSlaveBranchingCur->m_pBusSlaveBranchingLast;
        }
        else
        {
            if (EC_NULL != pBusSlaveBranchingCur)
            {
                pBusSlaveBranchingCur->m_wPortBranchingLast     = m_wPortBranchingCur;
                pBusSlaveBranchingCur->m_pBusSlaveBranchingLast = m_pBusSlaveBranchingCur;
            }
        }
    }
    m_pBusSlaveBranchingCur = pBusSlaveBranchingCur;
    m_wPortBranchingCur     = wPortBranchingCur;

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    m_eJunctionRedState = eJunctionRedStateUnknow;
#endif
    /* determine processing port order */
    if ((EC_NULL != m_pBusSlaveBranchingCur) && (ESC_PORT_INVALID!= m_wPortBranchingCur))
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "SetBusSlaveBranchingCur() Branching slave at AutoInc 0x%04X (Alias %d) Port %d\n", m_pBusSlaveBranchingCur->GetAutoIncAddress(), m_pBusSlaveBranchingCur->GetAliasAddress(), m_wPortBranchingCur));

#if (defined INCLUDE_LINE_CROSSED_DETECTION) && (defined INCLUDE_RED_DEVICE)
        m_pawExpectedProcessingPortOrder = c_awFrameProcessingPortOrder;
    }
    else
    {
        if (EC_NULL != pBusSlaveCur)
        {
            if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart <= pBusSlaveCur->GetBusIndex()))
            {
                m_pawExpectedProcessingPortOrder = c_awBackwardPortOrder;
            }
            else
            {
                m_pawExpectedProcessingPortOrder = c_awFrameProcessingPortOrder;
            }
        }
    }
#else
        EC_UNREFPARM(pBusSlaveCur);
    }
#endif /* INCLUDE_LINE_CROSSED_DETECTION && INCLUDE_RED_DEVICE */
}
EC_T_VOID CEcBusTopology::InsertBusSlaveInTopology(CEcBusSlave* pBusSlave)
{
EC_T_BYTE  byPortType = ECM_PORT_NOTIMPLEMENTED;
EC_T_DWORD dwUnsupportedTopologyCnt = m_dwSlavesAtStart * ESC_PORT_COUNT * 100;

#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    /* if no receive times are available physic must match (simple EL9010 detection) */
    if (!pBusSlave->AreRecvTimesCoherent() || pBusSlave->IsRecvTimesMissing())
#endif
    {
        byPortType = pBusSlave->GetPhysicPortX(pBusSlave->m_wParentPort);
    }
    for (; EC_NULL != m_pBusSlaveTopoCur; m_pBusSlaveTopoCur = m_pBusSlaveTopoCur->GetParentSlave())
    {
    EC_T_WORD wPortIdx      = 0;
    EC_T_WORD wPortIdxStart = 0;

        /* track last branching bus slave */
        if ((EC_NULL == m_pBusSlaveBranchingCur) && m_pBusSlaveTopoCur->IsBranching())
        {
            SetBusSlaveBranchingCur(EC_FALSE, m_pBusSlaveTopoCur, ESC_PORT_INVALID, EC_NULL);
        }
        /* get port index of input port */
        for (wPortIdxStart = 0; wPortIdxStart < ESC_PORT_COUNT; wPortIdxStart++)
        {
            if (m_pBusSlaveTopoCur->m_wInputPort == CEcBusTopology::c_awFrameProcessingPortOrder[wPortIdxStart])
            {
                break;
            }
        }
        /* go through ports following the expected propagation order starting after input port */
        /* considering slave with only one port connected */
        for (wPortIdx = 0, wPortIdxStart++; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
        {
        EC_T_WORD wPortCur = CEcBusTopology::c_awFrameProcessingPortOrder[(EC_T_DWORD)(wPortIdxStart + wPortIdx) % ESC_PORT_COUNT];

            /* skip already processed ports */
            if (m_pBusSlaveTopoCur->IsPortSmProc(wPortCur))
            {
                continue;
            }
            /* ignore port where physic doesn't match */
            if (  ECM_PORT_NOTIMPLEMENTED != byPortType
               && ECM_PORT_NOTCONFIGURED  != byPortType
               && ECM_PORT_NOTIMPLEMENTED != m_pBusSlaveTopoCur->GetPhysicPortX(wPortCur)
               && ECM_PORT_NOTCONFIGURED  != m_pBusSlaveTopoCur->GetPhysicPortX(wPortCur)
               && (m_pBusSlaveTopoCur->GetPhysicPortX(wPortCur) != byPortType)
               )
            {
                continue;
            }
            /* track last branching bus slave */
            if (m_pBusSlaveTopoCur == m_pBusSlaveBranchingCur)
            {
                if (wPortCur == m_pBusSlaveBranchingCur->m_wInputPort)
                {
	                SetBusSlaveBranchingCur(EC_TRUE, EC_NULL, ESC_PORT_INVALID, EC_NULL);
                }
                else
                {
                    SetBusSlaveBranchingCur(EC_FALSE, m_pBusSlaveBranchingCur, wPortCur, EC_NULL);
                }
            }
            /* found */
            LinkToParentBusSlave(pBusSlave, pBusSlave->m_wParentPort, m_pBusSlaveTopoCur, wPortCur);
            m_pBusSlaveTopoCur = pBusSlave;
            goto Exit;
        }
        if (m_pBusSlaveTopoCur == m_pBusSlaveBranchingCur)
        {
            SetBusSlaveBranchingCur(EC_TRUE, EC_NULL, ESC_PORT_INVALID, EC_NULL);
        }
        /* check for unsupported topology */
        dwUnsupportedTopologyCnt--;
        if (0 == dwUnsupportedTopologyCnt)
        {
            EC_ERRORMSGC(("Unsupported topology!"));
            goto Exit;
        }
    }
    /* unsupported topology */
    EC_ERRORMSGC(("Unsupported topology!"));

Exit:
    if (EC_NULL != m_pBusSlaveTopoCur)
    {
        if (m_pBusSlaveTopoCur->IsBranching())
        {
        EC_T_WORD wPortBranchingCur = ESC_PORT_INVALID;

            GetNextBusSlaveByPropagation(m_pBusSlaveTopoCur, m_pBusSlaveTopoCur->m_wInputPort, EC_FALSE, &wPortBranchingCur, EC_NULL, EC_NULL);
            SetBusSlaveBranchingCur(EC_FALSE, m_pBusSlaveTopoCur, wPortBranchingCur, EC_NULL);
        }
    }
    OsDbgAssert((EC_NULL == m_pBusSlaveBranchingCur) || (ESC_PORT_INVALID != m_wPortBranchingCur));
    return;
}
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
EC_T_VOID CEcBusTopology::InsertLineCrossedBusSlaveInTopology(CEcBusSlave* pBusSlave)
{
CEcBusSlave* pBusSlaveStartAt = EC_NULL;
EC_T_WORD    wPortToParent    = ESC_PORT_INVALID;
CEcBusSlave* pBusSlaveParent  = EC_NULL;
EC_T_WORD    wPortAtParent    = ESC_PORT_INVALID;
CEcBusSlave* pBusSlaveChild   = EC_NULL;
EC_T_WORD    wPortAtChild     = ESC_PORT_INVALID;
EC_T_BOOL    bLookForChild    = EC_TRUE;
EC_T_DWORD   dwUnsupportedTopologyCnt = m_dwSlavesAtStart * ESC_PORT_COUNT * 100;

    if (pBusSlave->IsPortSmProc(pBusSlave->m_wParentPort))
    {
        pBusSlave->m_wParentPort = pBusSlave->m_wInputPort;
    }
    if (EC_NULL == m_pBusSlaveBranchingCur)
    {
#if (defined INCLUDE_RED_DEVICE)
        if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart <= pBusSlave->GetBusIndex()))
        {
            pBusSlaveParent = m_pBusSlaveTopoRedRoot;
        }
        else
#endif /* INCLUDE_RED_DEVICE */
        {
            pBusSlaveParent = m_pBusSlaveTopoMainRoot;
        }
        GetNextBusSlaveByPropagation(pBusSlaveParent, pBusSlaveParent->m_wInputPort, EC_TRUE, &wPortAtParent, EC_NULL, EC_NULL);
    }
    else
    {
        pBusSlaveParent = m_pBusSlaveBranchingCur;
        wPortAtParent   = m_wPortBranchingCur;
    }
    pBusSlaveStartAt = pBusSlaveParent;
    wPortToParent    = pBusSlave->m_wParentPort;
    if (ESC_PORT_INVALID != wPortAtParent)
    {
        pBusSlaveChild   = pBusSlaveParent->m_apBusSlaveChildren[wPortAtParent];
        wPortAtChild     = (EC_NULL != pBusSlaveChild)?pBusSlaveChild->m_wInputPort:ESC_PORT_INVALID;
    }
    if (pBusSlave->GetSlaveDelay() > pBusSlaveParent->GetSlaveDelay())
    {
        wPortAtParent = pBusSlaveParent->m_wInputPort;
        if (EC_NULL != pBusSlaveParent->m_apBusSlaveChildren[wPortAtParent])
        {
            pBusSlaveChild = pBusSlaveParent->m_apBusSlaveChildren[wPortAtParent];
            wPortAtChild   = pBusSlaveChild->m_wInputPort;
            if (pBusSlave->IsBranching())
            {
                GetNextBusSlaveByPropagation(pBusSlave, pBusSlave->m_wInputPort, EC_FALSE, &wPortToParent, EC_NULL, EC_NULL);
                bLookForChild = EC_FALSE;
            }
            else
            {
                pBusSlaveParent = EC_NULL;
            }
        }
        else
        {
            /* insert bus slave at beginning */
            pBusSlaveChild = EC_NULL;
            bLookForChild  = EC_FALSE;
        }
    }
    if (bLookForChild)
    {
        for (; EC_NULL != pBusSlaveChild;)
        {
            if (pBusSlave->GetSlaveDelay() > pBusSlaveChild->GetSlaveDelay())
            {
                /* insert bus slave in middle */
                wPortToParent = GetLastConnectedPortAfterPort(pBusSlave, pBusSlave->m_wParentPort);
                break;
            }
            /* move to next bus slave */
            pBusSlaveParent = pBusSlaveChild;
            wPortAtParent   = wPortAtChild;
            GetNextBusSlaveByPropagation(pBusSlaveParent, wPortAtParent, EC_TRUE, &wPortAtParent, &pBusSlaveChild, &wPortAtChild);
            if (EC_NULL == pBusSlaveChild)
            {
                break;
            }
            /* check for unsupported topology */
            dwUnsupportedTopologyCnt--;
            if ((pBusSlaveParent->GetSlaveDelay() < pBusSlaveChild->GetSlaveDelay()) || (pBusSlaveParent == pBusSlaveStartAt) || (0 == dwUnsupportedTopologyCnt))
            {
                EC_ERRORMSGC(("Unsupported topology!"));
                pBusSlaveParent = EC_NULL;
                pBusSlaveChild  = EC_NULL;
                break;
            }
        }
    }
    /* link bus slave to child */
    if (EC_NULL != pBusSlaveChild)
    {
        LinkToParentBusSlave(pBusSlaveChild, wPortAtChild, pBusSlave, pBusSlave->m_wParentPort);
    }
    /* link bus slave to parent */
    if (EC_NULL != pBusSlaveParent)
    {
        LinkToParentBusSlave(pBusSlave, wPortToParent, pBusSlaveParent, wPortAtParent);
    }
    /* move to next */
    if (EC_NULL == m_pBusSlaveBranchingCur)
    {
        m_pBusSlaveTopoCur = pBusSlave;

#if (defined INCLUDE_RED_DEVICE)
        if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart <= m_pBusSlaveTopoCur->GetBusIndex()))
        {
            if ((EC_NULL != m_pBusSlaveBranchingCur) || m_pBusSlaveTopoCur->IsBranching())
            {
                m_pawExpectedProcessingPortOrder = c_awFrameProcessingPortOrder;
            }
            else
            {
                m_pawExpectedProcessingPortOrder = c_awBackwardPortOrder;
            }
        }
#endif /* INCLUDE_RED_DEVICE */
    }
    else
    {
        if ((pBusSlaveParent == m_pBusSlaveBranchingCur) && (EC_NULL == pBusSlaveChild))
        {
            m_pBusSlaveTopoCur = pBusSlave;
        }
        else
        {
            m_pBusSlaveTopoCur = m_pBusSlaveBranchingCur;
        }
    }
}
#endif /* INCLUDE_LINE_CROSSED_DETECTION */

EC_T_VOID CEcBusTopology::LookForMainPortInTopology(EC_T_VOID)
{
EC_T_DWORD dwUnsupportedTopologyCnt = m_dwSlavesAtStart * ESC_PORT_COUNT * 100;

    for (m_pBusSlaveMainPort = m_pBusSlaveTopoMainRoot; EC_NULL != m_pBusSlaveMainPort; m_pBusSlaveMainPort = m_pBusSlaveMainPort->GetParentSlave())
    {
        if (!m_pBusSlaveMainPort->IsPortSmProc(m_pBusSlaveMainPort->m_wInputPort))
        {
            m_pBusSlaveMainPort->SetPortSmProc(m_pBusSlaveMainPort->m_wInputPort);
            break;
        }
        /* check for unsupported topology */
        dwUnsupportedTopologyCnt--;
        if (0 == dwUnsupportedTopologyCnt)
        {
            EC_ERRORMSGC(("Unsupported topology!"));
            return;
        }
    }
}

#if (defined INCLUDE_RED_DEVICE)
/* look for remaining connected port according InsertBusSlaveInTopology() */
/* and consider it as redundancy port */
EC_T_VOID CEcBusTopology::LookForRedPortInTopology(EC_T_VOID)
{
EC_T_DWORD dwUnsupportedTopologyCnt = m_dwSlavesAtStart * ESC_PORT_COUNT * 100;

    for (; EC_NULL != m_pBusSlaveTopoCur; m_pBusSlaveTopoCur = m_pBusSlaveTopoCur->GetParentSlave())
    {
    EC_T_WORD wPortIdx      = 0;
    EC_T_WORD wPortIdxStart = 0;

        for (wPortIdxStart = 0; wPortIdxStart < ESC_PORT_COUNT; wPortIdxStart++)
        {
            if (m_pBusSlaveTopoCur->m_wInputPort == CEcBusTopology::c_awFrameProcessingPortOrder[wPortIdxStart])
            {
                break;
            }
        }
        for (wPortIdx = 0, wPortIdxStart++; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
        {
        EC_T_WORD wPortCur = CEcBusTopology::c_awFrameProcessingPortOrder[(EC_T_DWORD)(wPortIdxStart + wPortIdx) % ESC_PORT_COUNT];

            if (m_pBusSlaveTopoCur->IsPortSmProc(wPortCur))
            {
                continue;
            }
            /* found */
            m_pBusSlaveRedPort = m_pBusSlaveTopoCur;
            m_wRedPort = wPortCur;
            return;
        }
        if (wPortIdx < ESC_PORT_COUNT)
        {
            break;
        }
        /* check for unsupported topology */
        dwUnsupportedTopologyCnt--;
        if (0 == dwUnsupportedTopologyCnt)
        {
            EC_ERRORMSGC(("Unsupported topology!"));
            return;
        }
    }
}
#endif /* INCLUDE_RED_DEVICE */

EC_T_VOID CEcBusTopology::BuildBusTopology(CEcBusSlave* pBusSlave)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
CEcBusSlave* pBusSlaveTopoMainRoot = m_pBusSlaveTopoMainRoot;
#if (defined INCLUDE_RED_DEVICE)
CEcBusSlave* pBusSlaveTopoRedRoot  = m_pBusSlaveTopoRedRoot;
#endif
    /* prepare bus slave */
    BuildBusTopologyPrepareSlave(pBusSlave);

#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    /* detect line crossed and adjust eventually input port */
    DetectLineCrossed(pBusSlave);

    /* track topology root */
#if (defined INCLUDE_RED_DEVICE)
    if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart <= pBusSlave->GetBusIndex()))
    {
        if (pBusSlave->GetSlaveDelay() < m_pBusSlaveTopoRedRoot->GetSlaveDelay())
        {
            pBusSlaveTopoRedRoot = pBusSlave;
        }
    }
    else
#endif /* INCLUDE_RED_DEVICE */
    {
        if (pBusSlave->GetSlaveDelay() > m_pBusSlaveTopoMainRoot->GetSlaveDelay())
        {
            pBusSlaveTopoMainRoot = pBusSlave;
        }
    }
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    if (m_bJunctionRedundancyEnabled)
    {
        if ((eJunctionRedStateLineFixed == m_eJunctionRedState) && !pBusSlave->IsRecvTimesMissing())
        {
            /* first slave behind closed junction redundancy */
            m_pBusSlaveTopoCur = m_pBusSlaveBranchingCur;
            SetBusSlaveBranchingCur(EC_FALSE, EC_NULL, ESC_PORT_INVALID, EC_NULL);
        }
    }
#endif /* INCLUDE_JUNCTION_REDUNDANCY */
#endif /* INCLUDE_LINE_CROSSED_DETECTION */

    /* insert current bus slave in topology  */
    if (   (pBusSlave == m_pBusSlaveTopoMainRoot)
#if (defined INCLUDE_RED_DEVICE)
        || (pBusSlave == m_pBusSlaveTopoRedRoot)
#endif
       )
    {
        StartNewTopology(pBusSlave);
    }
    else
    {
        /* link to parent */
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
        if (pBusSlave->IsLineCrossed())
        {
            InsertLineCrossedBusSlaveInTopology(pBusSlave);
        }
        else
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
        {
            InsertBusSlaveInTopology(pBusSlave);
        }
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
        if (m_bJunctionRedundancyEnabled)
        {
            if (pBusSlave->IsSlaveConPortX(ESC_PORT_A))
            {
                if (pBusSlave->IsJunctionRedLineBreak())
                {
                    /* junction redundancy line fixed */
                    pBusSlave->SetJunctionRedLineBreak(EC_FALSE);
                    pBusSlave->SetJunctionRedChange(EC_TRUE);
                }
            }
            else
            {
                if ((EC_NULL != m_pBusSlaveBranchingCur) && (ESC_PORT_B == m_wPortBranchingCur))
                {
                    /* junction redundancy line break */
                    pBusSlave->SetJunctionRedLineBreak(EC_TRUE);
                    pBusSlave->SetJunctionRedChange(EC_TRUE);

                    m_eJunctionRedState = eJunctionRedStateLineBreak;
                }
            }
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
            if (eJunctionRedStateLineBreak == m_eJunctionRedState)
            {
                /* reset line crossed information for all slaves at */
                /* port B (this was needed to build the topology)   */
                pBusSlave->SetLineCrossed(EC_FALSE);
            }
            if (pBusSlave->IsRecvTimesMissing() && (EC_NULL != m_pBusSlaveBranchingCur) && m_pBusSlaveBranchingCur->IsSlaveConPortX(ESC_PORT_B))
            {
                /* junction redundancy line fixed detected */
                m_eJunctionRedState = eJunctionRedStateLineFixed;

                /* prevent to link bus slave to junction redundancy port */
                m_pBusSlaveBranchingCur->SetPortSmProc(ESC_PORT_B);
            }
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
        }
#endif /* INCLUDE_JUNCTION_REDUNDANCY */
    }
    /* store topology root */
    m_pBusSlaveTopoMainRoot = pBusSlaveTopoMainRoot;
#if (defined INCLUDE_RED_DEVICE)
    m_pBusSlaveTopoRedRoot = pBusSlaveTopoRedRoot;
#endif
    /* solve duplicate */
    {
    CEcBusSlave* pBusSlaveDuplicate = pBusSlave->m_pBusSlaveDuplicate;

        if ((EC_NULL != pBusSlaveDuplicate) && (EC_NULL != pBusSlaveDuplicate->m_pBusSlaveDuplicate))
        {
            if ((pBusSlave->GetParentSlave() == pBusSlaveDuplicate->GetParentSlave()) && (pBusSlaveDuplicate->GetPortAtParent() == pBusSlave->GetPortAtParent()))
            {
                /* synchronize with duplicate */
                pBusSlave->SynchronizeWithDuplicate(pBusSlaveDuplicate);

                /* duplicate solved */
                pBusSlaveDuplicate->m_pBusSlaveDuplicate = EC_NULL;
            }
        }
        pBusSlave->m_pBusSlaveDuplicate = EC_NULL;
    }
    /* consider last slave */
    if ((pBusSlave->GetBusIndex() == (m_dwSlavesAtStart - 1)))
    {
#if (defined INCLUDE_RED_DEVICE)
        if (m_bIsSendEnabledOnMainAtStart)
#endif
        {
            LookForMainPortInTopology();
        }
        if (EC_NULL != m_pBusSlaveMainPort)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BuildBusTopology() Main link detected at AutoInc 0x%04X Port %d\n", m_pBusSlaveMainPort->GetAutoIncAddress(), m_pBusSlaveMainPort->m_wInputPort));
        }
        else
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BuildBusTopology() No main link detected\n"));
        }
#if (defined INCLUDE_RED_DEVICE)
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
        if (!m_bRedEnhancedLineCrossedDetectionEnabled || m_bLineBreakAtStart)
#endif
        {
            if (m_bIsSendEnabledOnRedAtStart)
            {
                LookForRedPortInTopology();
            }
            if (EC_NULL != m_pBusSlaveRedPort)
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BuildBusTopology() Red link detected at AutoInc 0x%04X Port %d\n", m_pBusSlaveRedPort->GetAutoIncAddress(), m_wRedPort));
            }
            else
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BuildBusTopology() No red link detected\n"));
            }
        }
#endif /* INCLUDE_RED_DEVICE */

        /* release pending duplicates */
        while (EC_NULL != m_pBusSlaveDuplicate)
        {
        CEcBusSlave* pBusSlaveNext = m_pBusSlaveDuplicate->GetNext();

            OsDbgAssert(EC_NULL == pBusSlave->m_pBusSlaveDuplicate);
            DeleteBusSlave(m_pBusSlaveDuplicate);
            m_pBusSlaveDuplicate = pBusSlaveNext;
        }
    }
    return;
}

/***************************************************************************************************/
/**
\brief  Remove a bus slave from the list

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::RemoveBusSlave(
    CEcBusSlave* pBusSlave           /**< [in] Slave to remove */
                                      )
{
    EC_T_DWORD   dwRetVal      = EC_E_ERROR;
    EC_T_BOOL    bLocked       = EC_FALSE;

    /* check parameter */
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* lock the list */
    OsLock(m_poBusSlaveListLock);
    bLocked = EC_TRUE;

#if (defined INCLUDE_DC_SUPPORT)
    if (EC_NULL != m_pMaster->m_pDistributedClks)
    {
        m_pMaster->m_pDistributedClks->RemoveBusSlave(pBusSlave);
    }
#endif
    RemoveBusSlaveFromList(pBusSlave);

#if (defined INCLUDE_DC_SUPPORT)
    if (pBusSlave->IsDcEnabled() && (0 < m_dwBusSlavesDC))
    {
        m_dwBusSlavesDC--;
    }
#endif
    /* unlink slave descriptors related the removed bus slave */
    UnlinkSlaveDescriptors(pBusSlave);

    /* refresh hash table */
    {
    CEcBusSlave* pBusSlaveTmp = EC_NULL;

        if (EC_NULL != m_pBusSlavesByFixedAddress)
        {
            if (m_pBusSlavesByFixedAddress->Lookup(pBusSlave->GetFixedAddress(), pBusSlaveTmp) && (pBusSlave == pBusSlaveTmp))
            {
                m_pBusSlavesByFixedAddress->Remove(pBusSlave->GetFixedAddress());
            }
        }
        if (EC_NULL != m_pBusSlavesByIndex)
        {
            if (m_pBusSlavesByIndex->Lookup(pBusSlave->GetBusIndex(), pBusSlaveTmp) && (pBusSlave == pBusSlaveTmp))
            {
                m_pBusSlavesByIndex->Remove(pBusSlave->GetBusIndex());
            }
        }
    }
    /* release resources */
    DeleteBusSlave(pBusSlave);

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    /* unlock the list */
    if (bLocked)
    {
        OsUnlock(m_poBusSlaveListLock);
        bLocked = EC_FALSE;
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Remove bus slaves
*/
EC_T_VOID CEcBusTopology::RemoveBusSlaves()
{
    while (EC_NULL != m_pBusSlaveRoot)
    {
        RemoveBusSlave(m_pBusSlaveRoot);
    }
}

/***************************************************************************************************/
/**
\brief  Refresh config slave presence
*/
EC_T_VOID CEcBusTopology::RefreshCfgSlavePresence(CEcSlave* pCfgSlave)
{
    if (!pCfgSlave->IsPresenceValid())
    {
        pCfgSlave->SetSlavePresence(EC_FALSE);
    }
}

/***************************************************************************************************/
/**
\brief  Retrieve next bus slave connected to current one following the propagation order starting by
        wPortIn
*/
EC_T_VOID CEcBusTopology::GetNextBusSlaveByPropagation(CEcBusSlave* pBusSlave, EC_T_WORD wPortIn, EC_T_BOOL bSkipBranching, EC_T_WORD* pwPortOut, CEcBusSlave** ppBusSlaveNext, EC_T_WORD* pwPortInNext)
{
EC_T_WORD    wPortIdx      = 0;
EC_T_WORD    wPortIdxStart = 0;
EC_T_WORD    wPortOut      = wPortIn;
CEcBusSlave* pBusSlaveNext = EC_NULL;
EC_T_WORD    wPortInNext   = ESC_PORT_INVALID;
const EC_T_WORD* awPortOrder = EC_NULL;

    if (bSkipBranching)
    {
        awPortOrder = c_awBackwardPortOrder;
    }
    else
    {
        awPortOrder = c_awFrameProcessingPortOrder;
    }
    /* get port index of input port */
    for (wPortIdxStart = 0; wPortIdxStart < ESC_PORT_COUNT; wPortIdxStart++)
    {
        if (wPortIn == awPortOrder[wPortIdxStart])
        {
            break;
        }
    }
    /* go through ports following the expected propagation order starting after input port */
    for (wPortIdx = 0, wPortIdxStart++; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_WORD wPortCur = awPortOrder[(EC_T_DWORD)(wPortIdxStart + wPortIdx) % ESC_PORT_COUNT];

        /* look for connected port */
        if (pBusSlave->IsSlaveConPortX(wPortCur))
        {
            /* look for connected bus slave */
            if (EC_NULL != pBusSlave->m_apBusSlaveChildren[wPortCur])
            {
                /* return connected bus slave */
                pBusSlaveNext = pBusSlave->m_apBusSlaveChildren[wPortCur];

                /* look for IN port of the next slave */
                for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
                {
                    if (pBusSlave == pBusSlaveNext->m_apBusSlaveChildren[wPortIdx])
                    {
                        /* return IN port of the next slave */
                        wPortInNext = wPortIdx;
                        break;
                    }
                }
            }
            /* return connected port */
            wPortOut = wPortCur;
            break;
        }
    }
    if (EC_NULL != pwPortOut)
    {
        *pwPortOut = wPortOut;
    }
    if (EC_NULL != ppBusSlaveNext)
    {
        *ppBusSlaveNext = pBusSlaveNext;
    }
    if (EC_NULL != pwPortInNext)
    {
        *pwPortInNext = wPortInNext;
    }
}

/***************************************************************************************************/
/**
\brief  Lock / Unlock list access.
*/
EC_T_VOID CEcBusTopology::InterLockList(
    EC_T_BOOL bLS   /**< [in]   EC_TRUE if lock is requested, EC_FALSE if unlock */
                                       )
{
    if(bLS)
    {
        OsLock(m_poBusSlaveListLock);
    }
    else
    {
        OsUnlock(m_poBusSlaveListLock);
    }
}

/***************************************************************************************************/
/**
*/
EC_T_DWORD CEcBusTopology::SetReadEEPROMEntryByIoctl(EC_PT_SCANBUS_PROP_DESC pProp)
{
EC_T_DWORD dwRetVal   = EC_E_ERROR;
EC_T_DWORD dwEntryIdx = 0;

    switch(pProp->eEEPROMEntry)
    {
    case eEEP_VendorId:       dwEntryIdx = READEEPROMENTRY_IDX_VENDORID;     break;
    case eEEP_ProductCode:    dwEntryIdx = READEEPROMENTRY_IDX_PRODUCTCODE;  break;
    case eEEP_RevisionNumber: dwEntryIdx = READEEPROMENTRY_IDX_REVISIONNO;   break;
    case eEEP_SerialNumber:   dwEntryIdx = READEEPROMENTRY_IDX_SERIALNO;     break;
    case eEEP_AliasAddress:   dwEntryIdx = READEEPROMENTRY_IDX_ALIASADDRESS; break;
    case eEEP_BootRcvMbx:     dwEntryIdx = READEEPROMENTRY_IDX_BOOTRCVMBX;   break;
    case eEEP_BootSndMbx:     dwEntryIdx = READEEPROMENTRY_IDX_BOOTSNDMBX;   break;
    case eEEP_StdRcvMbx:      dwEntryIdx = READEEPROMENTRY_IDX_STDRCVMBX;    break;
    case eEEP_StdSndMbx:      dwEntryIdx = READEEPROMENTRY_IDX_STDSNDMBX;    break;
    case eEEP_MbxProtocol:    dwEntryIdx = READEEPROMENTRY_IDX_MBXPROTOCOL;  break;
    default: OsDbgAssert(EC_FALSE); goto Exit; break;
    }
    m_abReadEEPROMEntry[dwEntryIdx] = EC_TRUE;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
*/
EC_T_DWORD CEcBusTopology::SetCheckEEPROMEntryByIoctl(EC_PT_SCANBUS_PROP_DESC pProp)
{
EC_T_DWORD dwRetVal   = EC_E_ERROR;
EC_T_DWORD dwRes      = EC_E_ERROR;
EC_T_DWORD dwEntryIdx = EC_E_ERROR;

    if (0 != pProp->dwVerify)
    {
        dwRes = SetReadEEPROMEntryByIoctl(pProp);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    switch(pProp->eEEPROMEntry)
    {
    case eEEP_VendorId:       dwEntryIdx = READEEPROMENTRY_IDX_VENDORID;     break;
    case eEEP_ProductCode:    dwEntryIdx = READEEPROMENTRY_IDX_PRODUCTCODE;  break;
    case eEEP_RevisionNumber: dwEntryIdx = READEEPROMENTRY_IDX_REVISIONNO;   break;
    case eEEP_SerialNumber:   dwEntryIdx = READEEPROMENTRY_IDX_SERIALNO;     break;
    case eEEP_AliasAddress:   dwEntryIdx = READEEPROMENTRY_IDX_ALIASADDRESS; break;
    default: OsDbgAssert(EC_FALSE); goto Exit; break;
    }
    if (0 != pProp->dwVerify)
    {
        m_aeCheckEEPROMEntry[dwEntryIdx] = eCheckEEPROMEntry_EQUAL;
    }
    else
    {
        m_aeCheckEEPROMEntry[dwEntryIdx] = eCheckEEPROMEntry_ANY;
    }
    m_abCheckEEPROMSetByIoctl[dwEntryIdx] = EC_TRUE;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}
EC_T_CHECKEEPROMENTRY CEcBusTopology::CheckEEPromContentEntryCriterion(CEcSlave* pCfgSlave, EC_T_DWORD dwEEPromEntryIdx)
{
    EC_T_CHECKEEPROMENTRY eRetVal = eCheckEEPROMEntry_ANY;
    if (m_abCheckEEPROMSetByIoctl[dwEEPromEntryIdx])
    {
        eRetVal = m_aeCheckEEPROMEntry[dwEEPromEntryIdx];
    }
    else
    {
        eRetVal = pCfgSlave->GetCheckEEPromEntryCriterion(dwEEPromEntryIdx);
    }
    return eRetVal;
}

/***************************************************************************************************/
/**
\brief  Check Configuration match of single Slave Entry.

\return EC_E_NOERROR on match, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::CheckEEPromContent(
    CEcBusSlave*    pBusSlave       /**< [in]   Bus slave */
   ,CEcSlave*       pCfgSlave       /**< [in]   Config slave */
)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRevisionCheckMask = 0;

    /* check parameter */
    if (EC_NULL == pBusSlave)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* if config slave not given, do internal comparison within current slave list object */
    if (EC_NULL == pCfgSlave)
    {
        pCfgSlave = pBusSlave->GetCfgSlave();
        if (EC_NULL == pCfgSlave)
        {
            dwRetVal = EC_E_BUSCONFIG_MISMATCH;
            goto Exit;
        }
    }
    /* check vendor ID */
    if ((CheckEEPromContentEntryCriterion(pCfgSlave, READEEPROMENTRY_IDX_VENDORID) != eCheckEEPROMEntry_ANY)
        && (pCfgSlave->GetVendorId() != pBusSlave->GetVendorId()))
    {
        dwRetVal = EC_E_BUSCONFIG_MISMATCH;
        goto Exit;
    }
    /* check product code */
    if ((CheckEEPromContentEntryCriterion(pCfgSlave, READEEPROMENTRY_IDX_PRODUCTCODE) != eCheckEEPROMEntry_ANY)
        && (pCfgSlave->GetProductCode() != pBusSlave->GetProductCode()))
    {
        dwRetVal = EC_E_BUSCONFIG_MISMATCH;
        goto Exit;
    }
    /* check revision number using InitCmds */
    switch(CheckEEPromContentEntryCriterion(pCfgSlave, READEEPROMENTRY_IDX_REVISIONNO))
    {
    case eCheckEEPROMEntry_EQUAL:
        dwRevisionCheckMask = 0xFFFFFFFF;   /* don't mask anything */
        break;
    case eCheckEEPROMEntry_LWEQUAL:
        dwRevisionCheckMask = 0x0000FFFF;   /* mask high, just low word has to be equal */
        break;
    case eCheckEEPROMEntry_HWEQUAL:
        dwRevisionCheckMask = 0xFFFF0000;   /* mask low, just high word has to be equal */
        break;
    default:
        break;
    }

    if ((CheckEEPromContentEntryCriterion(pCfgSlave, READEEPROMENTRY_IDX_REVISIONNO) != eCheckEEPROMEntry_ANY) 
        && ((pCfgSlave->GetRevisionNumber() & dwRevisionCheckMask) != (pBusSlave->GetRevisionNo() & dwRevisionCheckMask)))
    {
        dwRetVal = EC_E_BUSCONFIG_MISMATCH;
        goto Exit;
    }
    /* check serial number using InitCmds */
    if ((CheckEEPromContentEntryCriterion(pCfgSlave, READEEPROMENTRY_IDX_SERIALNO) != eCheckEEPROMEntry_ANY)
        && (pCfgSlave->GetSerialNumber() != pBusSlave->GetSerialNo()))
    {
        dwRetVal = EC_E_BUSCONFIG_MISMATCH;
        goto Exit;
    }
    /* check alias address */
    if ((CheckEEPromContentEntryCriterion(pCfgSlave, READEEPROMENTRY_IDX_ALIASADDRESS) != eCheckEEPROMEntry_ANY)
        && (pBusSlave->GetAliasAddress() != 0) /* check only slave with Alias Address configured in EEPROM */
        && (pBusSlave->GetAliasAddress() != pCfgSlave->GetCfgFixedAddr()))
    {
        dwRetVal = EC_E_BUSCONFIG_MISMATCH;
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_RED_DEVICE)
EC_T_VOID CEcBusTopology::MainLinkStatusChanged(EC_T_LINKSTATUS eLinkStatusMain, EC_T_LINKSTATUS eLinkStatusRed)
#else
EC_T_VOID CEcBusTopology::LinkStatusChanged(EC_T_LINKSTATUS eLinkStatus)
#endif
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
#if (defined INCLUDE_RED_DEVICE)
EC_T_BOOL bMainConnected = (eLinkStatus_OK == eLinkStatusMain);
EC_T_BOOL bRedConnected  = (eLinkStatus_OK == eLinkStatusRed);
#else
EC_T_BOOL bMainConnected = (eLinkStatus_OK == eLinkStatus);
EC_T_BOOL bRedConnected  = EC_TRUE;
#endif
EC_T_BOOL bApplyTopologyChangeDelay = EC_FALSE;

    /* if topology change in  manual mode or topology change delay configured, apply send mask */
    if (!m_bTopologyChangeAutoMode || (0 != m_dwTopologyChangeDelay))
    {
        if (!bMainConnected)
        {
            /* mask send as soon as disconnection is detected */
            m_bSendMaskedOnMain = EC_TRUE;

            m_pBusSlaveMainPort = EC_NULL;
        }
        else if (m_bTopologyChangeAutoMode)
        {
            /* apply topology change delay */
            bApplyTopologyChangeDelay = EC_TRUE;
        }
        else if (m_bLinkWasDisconnected)
        {
            /* unmask send if link was disconnected before */
            m_bSendMaskedOnMain = EC_FALSE;
        }
#if (defined INCLUDE_RED_DEVICE)
        /* detect link disconnected */
        if (m_bSendMaskedOnMain && m_bSendMaskedOnRed)
#else
        /* detect link disconnected */
        if (m_bSendMaskedOnMain)
#endif
        {
            m_bLinkWasDisconnected = EC_TRUE;
        }
    }
    else
    {
        /* detect link disconnected */
        if (!bMainConnected && !bRedConnected)
        {
            m_bLinkWasDisconnected = EC_TRUE;
        }
    }
#if (defined INCLUDE_RED_DEVICE)
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "MainLinkStatusChanged() m_bSendMaskedOnMain = %d, m_bSendMaskedOnRed = %d\n", m_bSendMaskedOnMain, m_bSendMaskedOnRed));
#else
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "MainLinkStatusChanged() m_bSendMaskedOnMain = %d\n", m_bSendMaskedOnMain));
#endif
    if (bApplyTopologyChangeDelay)
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "MainLinkStatusChanged() Start %d ms Topology change delay\n", m_dwTopologyChangeDelay));
        m_oTopologyChangeDelayTimeout.Start(m_dwTopologyChangeDelay, m_pMaster->GetMsecCounterPtr());
    }
    else
    {
        if (m_bTopologyChangeAutoMode)
        {
            SetTopologyChanged();
        }
    }
}
#if (defined INCLUDE_RED_DEVICE)
EC_T_VOID CEcBusTopology::RedLinkStatusChanged(EC_T_LINKSTATUS eLinkStatusMain, EC_T_LINKSTATUS eLinkStatusRed)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_BOOL bMainConnected = (eLinkStatus_OK == eLinkStatusMain);
EC_T_BOOL bRedConnected  = (eLinkStatus_OK == eLinkStatusRed);
EC_T_BOOL bApplyTopologyChangeDelay = EC_FALSE;

    /* if topology change in  manual mode or topology change delay configured, apply send mask */
    if (!m_bTopologyChangeAutoMode || (0 != m_dwTopologyChangeDelay))
    {
        if (!bRedConnected)
        {
            /* mask send as soon as disconnection is detected */
            m_bSendMaskedOnRed = EC_TRUE;

            m_pBusSlaveRedPort = EC_NULL;
            m_wRedPort         = ESC_PORT_INVALID;
        }
        else if (m_bTopologyChangeAutoMode)
        {
            /* apply topology change delay */
            bApplyTopologyChangeDelay = EC_TRUE;
        }
        else if (m_bLinkWasDisconnected)
        {
            /* unmask send if link was disconnected before */
            m_bSendMaskedOnRed = EC_FALSE;
        }
        /* detect link disconnected */
        if (m_bSendMaskedOnMain && m_bSendMaskedOnRed)
        {
            m_bLinkWasDisconnected = EC_TRUE;
        }
    }
    else
    {
        /* detect link disconnected */
        if (!bMainConnected && !bRedConnected)
        {
            m_bLinkWasDisconnected = EC_TRUE;
        }
    }
#if (defined INCLUDE_RED_DEVICE)
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "RedLinkStatusChanged() m_bSendMaskedOnMain = %d, m_bSendMaskedOnRed = %d\n", m_bSendMaskedOnMain, m_bSendMaskedOnRed));
#endif
    if (bApplyTopologyChangeDelay)
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "RedLinkStatusChanged() Start %d ms Topology change delay\n", m_dwTopologyChangeDelay));
        m_oTopologyChangeDelayTimeout.Start(m_dwTopologyChangeDelay, m_pMaster->GetMsecCounterPtr());
    }
    else
    {
        if (m_bTopologyChangeAutoMode)
        {
            SetTopologyChanged();
        }
    }
}
#endif /* INCLUDE_RED_DEVICE */

/***************************************************************************************************/
/**
\brief  Set topology changed flag

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CEcBusTopology::SetTopologyChanged(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>4) &3);
#endif
EC_T_WORD wSlaveCount = 0;

    /* get current slave count */
    wSlaveCount = m_pMaster->GetBrdAlStatusWkc();
    if (EC_AL_STATUS_INIT_VALUE == wSlaveCount)
    {
        wSlaveCount = m_wAcycBrdAlStatusWkc;
    }
    if (m_bTopologyChangedMasked)
    {
        m_oTopologyChangeDelayTimeout.Stop();
        return;
    }
    if (m_bTopologyChanged)
    {
        return;
    }
    m_oTopologyChangeDelayTimeout.Stop();
    m_bTopologyChanged = EC_TRUE;

    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG),
        "SetTopologyChanged() Slave count = %d, BusSlave count = %d, Expected WKC = %d\n",
        (EC_T_INT)wSlaveCount, (EC_T_INT)m_dwBusSlaves, (EC_T_INT)m_pMaster->m_wALStatusExpectedWkc));
}

/***************************************************************************************************/
/**
\brief  Check if topology changes occured

\return EC_TRUE if topology has changed, EC_FALSE if not.
*/
EC_T_BOOL CEcBusTopology::TopologyChangedDetected(EC_T_VOID)
{
EC_T_BOOL bMasterConfigured = (0 < m_pMaster->GetCfgSlaveCount())?EC_TRUE:EC_FALSE;
EC_T_WORD wSlaveCount       = m_pMaster->GetBrdAlStatusWkc();

    /* return immediately if master is not configured */
    if (!bMasterConfigured)
    {
        goto Exit;
    }
    /* return immediately if topology changes already detected */
    if (m_bTopologyChanged)
    {
        goto Exit;
    }
    /* return immediately if slave count monitoring is impossible (current slave count is unknown) */
    if (EC_AL_STATUS_INIT_VALUE == wSlaveCount)
    {
        goto Exit;
    }
    /* handle slave count monitoring to detect topology changes */
    if (m_dwBusSlaves != wSlaveCount)
    {
        if (0 == m_dwBusSlaves)
        {
            /* recovering for permanent frameloss */
            SetTopologyChanged();
        }
        else
        {
            /* wait for slaves count stabilization */
            if (m_wTopologyChangeSlaveCount != wSlaveCount)
            {
                m_wTopologyChangeSlaveCount = wSlaveCount;
                if (0 == m_wTopologyChangeSlaveCount)
                {
                    m_dwTopologyChangeCounter = ECBT_TOPOCHANGE_THRESHOLD_FRAMELOSS;
                }
                else
                {
                    m_dwTopologyChangeCounter = ECBT_TOPOCHANGE_THRESHOLD_SLAVE;
                }
            }
            else
            {
                m_dwTopologyChangeCounter--;
                if (0 == m_dwTopologyChangeCounter)
                {
                    SetTopologyChanged();
                }
            }
        }
    }
    else
    {
        /* reset monitoring */
        m_wTopologyChangeSlaveCount = EC_AL_STATUS_INIT_VALUE;
    }

Exit:
    return m_bTopologyChanged;
}

/***************************************************************************************************/
/**
\brief  Check if topology is stable

\return EC_TRUE if topology might be changing.
*/
EC_T_BOOL CEcBusTopology::TopologyIsChanging(EC_T_VOID)
{
EC_T_WORD wSlaveCount = m_pMaster->GetBrdAlStatusWkc();

    return ((EC_AL_STATUS_INIT_VALUE != wSlaveCount) && (wSlaveCount != m_dwBusSlaves));
}

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
/***************************************************************************************************/
/**
\brief  

\return N/A
*/
EC_T_VOID CEcBusTopology::SetJunctionRedundancyEnabled(EC_T_BOOL bEnabled)
{
    m_bJunctionRedundancyEnabled = bEnabled;
    if (m_bJunctionRedundancyEnabled)
    {
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
        SetNotifyNotConnectedPortA(EC_FALSE);
        SetNotifyUnexpectedConnectedPort(EC_FALSE);
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
    }
}
#endif /* INCLUDE_JUNCTION_REDUNDANCY */

/***************************************************************************************************/
/**
\brief  Topology Changed while ScanBus ?.

\return EC_TRUE if topology changed, EC_FALSE otherwise.
*/
EC_T_BOOL CEcBusTopology::TopologyChangedWhileScan(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
EC_T_WORD wSlaveCount      = 0;
EC_T_BOOL bTopologyChanged = EC_FALSE;

    /* get current slave count */
    wSlaveCount = m_pMaster->GetBrdAlStatusWkc();
    if (EC_AL_STATUS_INIT_VALUE == wSlaveCount)
    {
        wSlaveCount = m_wAcycBrdAlStatusWkc;
    }
    /* topology changed if working counter changed */
    if ((m_dwSlavesAtStart != wSlaveCount) && (EC_AL_STATUS_INIT_VALUE != wSlaveCount))
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG),"TopologyChangedWhileScan() WKC change detected at start %d, now %d \n", m_dwSlavesAtStart, wSlaveCount));
        bTopologyChanged = EC_TRUE;
    }
    /* topology changed if link connected/disconnected */
    if (m_bLinkConnectedAtStart != m_poEthDevice->IsLinkConnected())
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "TopologyChangedWhileScan() Link dis-/connection detected\n"));
        bTopologyChanged = EC_TRUE;
    }
#if (defined INCLUDE_RED_DEVICE)
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    if (!m_bRedEnhancedLineCrossedDetectionRunning)
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
    {
        if ( (m_bLineBreakAtStart       != m_pMaster->m_bRedLineBreak)
          || (m_wSlavesCntOnMainAtStart != m_pMaster->m_wRedNumSlavesOnMain)
          || (m_wSlavesCntOnRedAtStart  != m_pMaster->m_wRedNumSlavesOnRed))
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG),"TopologyChangedWhileScan() Line break change detected\n"));
            bTopologyChanged = EC_TRUE;
        }
        if ( (m_bIsSendEnabledOnMainAtStart != m_poEthDevice->IsSendEnabledOnMain())
          || (m_bIsSendEnabledOnRedAtStart  != m_poEthDevice->IsSendEnabledOnRed()))
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "TopologyChangedWhileScan() Main/Red link dis-/connection detected\n"));
            bTopologyChanged = EC_TRUE;
        }
    }
#endif /* INCLUDE_RED_DEVICE */
    return bTopologyChanged;
}

/***************************************************************************************************/
/**
\brief  Link slave descriptors
*/
EC_T_VOID CEcBusTopology::LinkSlaveDescriptors(CEcBusSlave* pBusSlave, CEcSlave* pCfgSlave)
{

    if ((pCfgSlave == pBusSlave->GetCfgSlave()) && (pBusSlave == pCfgSlave->m_pBusSlave))
    {
        /* nothing to do */
        return;
    }
    OsDbgAssert(EC_NULL == pBusSlave->GetCfgSlave());
    OsDbgAssert(EC_NULL == pCfgSlave->m_pBusSlave);

    /* refresh bus slave information */
    pBusSlave->SetCfgSlave(pCfgSlave);

    /* by some slaves the port descriptor register returns always 0 */
    /* in this case the physics description from the ENI file should be used */
    if ((0 == pBusSlave->GetPortDescriptor()) && (EC_NULL != pCfgSlave))
    {
        pBusSlave->SetPortDescriptor(pCfgSlave->GetPhysicsDesc());
        pBusSlave->SetPortSetting(pBusSlave->GetDlStatus());
    }
    /* refresh config slave information */
    if (EC_NULL != pCfgSlave)
    {
        pCfgSlave->m_wAutoIncAddr     = pBusSlave->GetAutoIncAddress();
        pCfgSlave->m_bDeviceEmulation = pBusSlave->IsDeviceEmulation();
        pCfgSlave->m_pBusSlave        = pBusSlave;
        pCfgSlave->SetSlavePresence(EC_TRUE);
    }
}

/***************************************************************************************************/
/**
\brief  Unlink slave descriptors
*/
EC_T_VOID CEcBusTopology::UnlinkSlaveDescriptors(CEcBusSlave* pBusSlave)
{
EC_T_WORD   wPortIdx         = 0;
CEcSlave*    pCfgSlave       = pBusSlave->GetCfgSlave();
CEcBusSlave* pBusSlaveParent = pBusSlave->GetParentSlave();

    /* detach parent slave */
    if ((EC_NULL != pBusSlaveParent) && (ESC_PORT_INVALID != pBusSlave->m_wPortAtParent))
    {
        pBusSlaveParent->m_apBusSlaveChildren[pBusSlave->m_wPortAtParent] = EC_NULL;
    };
    /* reset children slaves information */
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
        pBusSlave->m_apBusSlaveChildren[wPortIdx] = EC_NULL;
    }
    pBusSlave->m_wPortAtParent = ESC_PORT_INVALID;

    /* reset config slave information */
    if (EC_NULL != pCfgSlave)
    {
        pCfgSlave->SetSlavePresence(EC_FALSE);
        pCfgSlave->m_pBusSlave     = EC_NULL;
    }
}

/***************************************************************************************************/
/**
\brief  Reads slave identification to a bus slave
*       This function reads the slave indentification with the direct id or the requesting id mechanism.
*
\return EC_E_NOERROR if value is read succesfully, EC_T_BUSY if identification is in progess, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::ReadSlaveIdentification(
    CEcBusSlave*    pBusSlave,  /* <[in/out] Bus slave, result is stored in wIdentificationAdo/Value */
    EC_T_WORD       wIdentAdo   /* <[in]     Offset of identification value */
    )
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug >> 2) & 3);
#endif
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_WORD  wAdo = 0;
    EC_T_BYTE  byCmd = 0;
    EC_T_BYTE  abyDataFrm[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 
    EC_T_WORD  wDataLen = 0; /* Data identification length */

    /* this function is called a several times with different pBusSlave->wIdentificationAdo values */
    /* use wIdentificationAdo of the bus slave to determine next action */
    switch (pBusSlave->m_wIdentificationAdo)
    {
    case 0: /* No device identification was invoked before */
        {           
            if (ECREG_AL_STATUSCODE == wIdentAdo)
            {
                /* request id mechanism */
                wAdo = ECREG_AL_CONTROL;
                byCmd = EC_CMD_TYPE_FPWR;
                EC_SET_FRM_WORD(abyDataFrm, (EC_T_WORD)(DEVICE_STATE_IDREQUEST | (pBusSlave->GetAlStatus() & DEVICE_STATE_MASK)));
                wDataLen = 2;
            }
            else
            {
                /* direct id mechanism */
                wAdo = wIdentAdo;
                byCmd = EC_CMD_TYPE_FPRD;
                wDataLen = 2;
            }
        } break;
    case ECREG_AL_CONTROL: /* request id send before, now read AL_STATUS + AL_STATUS_CODE */
    {
        m_oIdentifyBusSlaveTimeout.Start(IDENTIFY_BUSSLAVE_TIMEOUT, m_pMaster->GetMsecCounterPtr());
    }
    /* no break */
    case ECREG_AL_STATUS: /* wait for identification value */
    {
        wAdo = ECREG_AL_STATUS;
        byCmd = EC_CMD_TYPE_FPRD;
        EC_SET_FRM_WORD(abyDataFrm, (EC_T_WORD)(pBusSlave->GetAlStatus() & DEVICE_STATE_MASK));
        wDataLen = 6;
    } break;
    default: /* read id finished, ecbtsmchk_clean will clean AL_CONTROL register if necessary */
        {    
            /* result is stored in pBusSlave */
            dwRetVal = EC_E_NOERROR;
            goto Exit;
        } break;
    }

    /* queue EtherCAT command */    
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "IdentifyBusSlave() Try to identify AutoInc 0x%04X\n", pBusSlave->GetAutoIncAddress()));
    dwRetVal = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, INVOKE_ID_SB_IDENTIFYSLAVE, byCmd,
                    pBusSlave->GetFixedAddress(), wAdo, wDataLen, abyDataFrm
                    );
    if (EC_E_NOERROR != dwRetVal)
    {
        /* try again next cycle */
        goto Exit;
    }
    m_dwEcatCmdPending++;
    m_dwEcatCmdSent++;

    dwRetVal = EC_E_BUSY;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Helper function to check previous address of a cfg slave candidate

\return EC_TRUE if prev address matches or check disabled otherwise EC_FALSE.
*/
static EC_T_BOOL IsPrevAddrMatching(CEcBusSlave* pBusSlave, CEcSlave* pCfgSlave)
{
#if (defined INCLUDE_HOTCONNECT)
    EC_T_BOOL bMatch = EC_FALSE;

    /* check previous address */
    if (INVALID_FIXED_ADDR != pCfgSlave->m_pEniSlave->wHCIdentifyAddrPrev)
    {
        CEcBusSlave* pBusSlavePrev = pBusSlave->GetPrev();
        if (EC_NULL != pBusSlavePrev)
        {
            CEcSlave* pCfgSlavePrev = pBusSlavePrev->GetCfgSlave();
            if (EC_NULL != pCfgSlavePrev
                && pCfgSlavePrev->GetFixedAddr() == pCfgSlave->m_pEniSlave->wHCIdentifyAddrPrev)
            {
                bMatch = EC_TRUE;
            }
        }
    }
    else
    {
        bMatch = EC_TRUE;
    }

    return bMatch;
#else
    EC_UNREFPARM(pBusSlave);
    EC_UNREFPARM(pCfgSlave);
    return EC_TRUE;
#endif
}

/***************************************************************************************************/
/**
\brief  Identify a BusSlave in a CfgSlave list

\return EC_E_NOERROR if slave found, EC_E_NOTFOUND if not, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::IdentifyBusSlave(CEcBusSlave* pBusSlave, CEcSlaveList* pCfgSlaveList, CEcSlave** ppCfgSlave)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
EC_T_DWORD       dwRetVal           = EC_E_ERROR;
EC_T_DWORD       dwRes              = EC_E_ERROR;
CEcSlaveListNode pNode              = EC_NULL;
CEcSlave*        pCfgSlave          = EC_NULL;
CEcSlave*        pCfgSlaveMatch     = EC_NULL;
CEcSlave*        pCfgSlaveCandidate = EC_NULL;

    /* check parameters */
    if ((EC_NULL == pBusSlave) || (EC_NULL == pCfgSlaveList) || (EC_NULL == ppCfgSlave))
    {
        OsDbgAssert(EC_FALSE);
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* look for matching slaves in the list */
    for (pNode = pCfgSlaveList->GetFirstNode(); EC_NULL != pNode; pCfgSlaveList->GetNext(pNode))
    {
        pCfgSlave = pCfgSlaveList->GetAt(pNode);

#if (defined INCLUDE_CONFIG_EXTEND)
        OsDbgAssert(pCfgSlave->m_dwSlaveId < m_pMaster->GetCfgSlaveCount());
#endif
        /* config slave already assigned */
        if (EC_NULL != pCfgSlave->m_pBusSlave)
        {
            continue;
        }
        /* EEProm contain must tmatch */
        dwRes = CheckEEPromContent(pBusSlave, pCfgSlave);
        if (EC_E_NOERROR == dwRes)
        {
            pCfgSlaveCandidate = pCfgSlave;
        }
        /* if found CfgSlave is not optional, stop search */
        if (!pCfgSlave->IsOptional())
        {
            break;
        }
    }
    if (EC_NULL == pCfgSlaveCandidate)
    {
        dwRetVal = EC_E_NOTFOUND;
        goto Exit;
    }
    /* slave identification, cfg slave with tag or hc group head */
    if (pCfgSlaveCandidate->HasIdentificationCheck())
    {
        EC_T_WORD wIdentificationAdo = 0;
        EC_T_WORD wIdentificationValue = 0;

        pCfgSlaveCandidate->GetIdentification(&wIdentificationAdo, &wIdentificationValue);

        dwRes = ReadSlaveIdentification(pBusSlave, wIdentificationAdo);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }

        /* check identification */
        if (wIdentificationAdo == pBusSlave->m_wIdentificationAdo
            && wIdentificationValue == pBusSlave->m_wIdentificationValue
            && IsPrevAddrMatching(pBusSlave, pCfgSlaveCandidate))
        {
            pCfgSlaveMatch = pCfgSlaveCandidate;
        }
#if (defined INCLUDE_HOTCONNECT)
        else if (pCfgSlaveCandidate->IsHCGroupHead())
        {
            EC_T_WORD wNextAdo = 0xFFFF;
            CEcSlave* pCfgSlvNextCandidate = EC_NULL;

            /* iterate throw slave list to find a matching hot connect head */
            /* if no matching slave is found the identification is started again with another ado from the slave list */
            for (pNode = pCfgSlaveList->GetFirstNode(); EC_NULL != pNode; pCfgSlaveList->GetNext(pNode))
            {
                pCfgSlave = pCfgSlaveList->GetAt(pNode);

                /* skip non hot connect group heads */
                if (!pCfgSlave->IsHCGroupHead())
                    continue;

                /* skip previously tested slave */
                if (pCfgSlaveCandidate == pCfgSlave)
                    continue;

                /* skip non matching slave */
                if (EC_E_NOERROR != CheckEEPromContent(pBusSlave, pCfgSlave))
                    continue;

                /* skip matching slave with different Ado */
                if (pBusSlave->m_wIdentificationAdo != pCfgSlave->m_pEniSlave->wIdentificationAdo)
                {
                    /* determine the next possible ado(increase order) and next slave respectively */
                    if (pCfgSlave->m_pEniSlave->wIdentificationAdo > pBusSlave->m_wIdentificationPrevAdo
                        && pCfgSlave->m_pEniSlave->wIdentificationAdo < wNextAdo)
                    {
                        wNextAdo = pCfgSlave->m_pEniSlave->wIdentificationAdo;
                        pCfgSlvNextCandidate = pCfgSlave;
                    }
                    continue;
                }
                /* check identification value */
                if (pBusSlave->m_wIdentificationAdo == pCfgSlave->m_pEniSlave->wIdentificationAdo
                    && pBusSlave->m_wIdentificationValue == pCfgSlave->m_pEniSlave->wIdentificationValue
                    && IsPrevAddrMatching(pBusSlave, pCfgSlave))
                {
                    /* slave matches but is already configured => duplicate */
                    if (EC_NULL != pCfgSlave->m_pBusSlave)
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), 
                            "IdentifyBusSlave() Slave with AutoInc 0x%04X identified as duplicated %d\n", 
                            pBusSlave->GetAutoIncAddress(), pCfgSlave->m_pBusSlave->GetFixedAddress()));

                        pBusSlave->m_pBusSlaveDuplicate = pCfgSlave->m_pBusSlave;
                        pBusSlave->m_pCfgSlaveIdentMisMatch = pCfgSlaveCandidate;

                        dwRetVal = EC_E_DUPLICATE;
                        goto Exit;
                    }
                    else
                    {
                        /* matching slave found */
                        pCfgSlaveMatch = pCfgSlave;
                        break;
                    }
                }
            }
            if (EC_NULL == pCfgSlaveMatch)
            {
                /* complete list with different ados processed but no slave found */
                if (0xFFFF == wNextAdo || pBusSlave->m_wIdentificationPrevAdo == wNextAdo)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), 
                        "IdentifyBusSlave() Identification HC group failed for AutoInc 0x%04X\n", 
                        pBusSlave->GetAutoIncAddress()));

                    /* notice candidate for bus mismatch notification */
                    if (EC_NULL == pBusSlave->m_pCfgSlaveIdentMisMatch)
                        pBusSlave->m_pCfgSlaveIdentMisMatch = pCfgSlaveCandidate;

                    dwRetVal = EC_E_NOTFOUND;
                    goto Exit;
                }
                /* notice next candidate for bus mismatch notification */
                pBusSlave->m_pCfgSlaveIdentMisMatch = pCfgSlvNextCandidate;

                /* try another id request with a possible ado */
                pBusSlave->m_wIdentificationPrevAdo = wNextAdo;
                pBusSlave->m_wIdentificationAdo   = 0;
                pBusSlave->m_wIdentificationValue = 0;
                dwRes = ReadSlaveIdentification(pBusSlave, wNextAdo);
                if (EC_E_NOERROR != dwRes)
                {
                    dwRetVal = dwRes;
                    goto Exit;
                }
            }
        }
#endif
        else
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), 
                "IdentifyBusSlave() Identification failed for AutoInc 0x%04X\n", 
                pBusSlave->GetAutoIncAddress()));

            /* notice candidate for bus mismatch notification */
            pBusSlave->m_pCfgSlaveIdentMisMatch = pCfgSlaveCandidate;

            dwRetVal = EC_E_IDENTIFICATIONFAILED;
            goto Exit;
        }
    }
    else if (IsPrevAddrMatching(pBusSlave, pCfgSlaveCandidate)) /* slaves without identification always matching previous address */
    {
        pCfgSlaveMatch = pCfgSlaveCandidate;
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "IdentifyBusSlave() Slave with AutoInc 0x%04X identified as %d\n", pBusSlave->GetAutoIncAddress(), pCfgSlaveMatch->GetFixedAddr()));

    /* no errors */
    *ppCfgSlave = pCfgSlaveMatch;
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Start the topology detection resync (after a line break or a bus mismatch)

\return EC_E_NOERROR on bus match, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::StartTopologyDetectionResync(CEcBusSlave* pBusSlaveResync)
{
    m_pResyncBusSlave = pBusSlaveResync;
    m_pScanBusSlaveCur = pBusSlaveResync;
    m_dwResyncCfgSlaveStartIdx = 0;
    m_dwResyncCfgSlavesCnt = 0;
    m_dwResyncCfgSlavesProcessed = 0;

    /* determine the last incomplete group and count present slaves */
#if (defined INCLUDE_HOTCONNECT)
    for (m_pResyncHcGroupIncomplete = m_pScanHCGroupCur; EC_NULL != m_pResyncHcGroupIncomplete; m_pResyncHcGroupIncomplete = m_pResyncHcGroupIncomplete->pPrevGroup)
    {
        m_dwResyncCfgSlavesCnt = m_pResyncHcGroupIncomplete->dwGrpMembers;
#else
    {
        m_dwResyncCfgSlavesCnt = m_pMaster->GetCfgSlaveCount();
#endif
        m_dwResyncCfgSlaveStartIdx = 0;
        for (EC_T_DWORD dwCfgSlaveIdx = 0; dwCfgSlaveIdx < m_dwResyncCfgSlavesCnt; dwCfgSlaveIdx++)
        {
#if (defined INCLUDE_HOTCONNECT)
            CEcSlave* pCfgSlave = m_pResyncHcGroupIncomplete->ppGroupMembers[dwCfgSlaveIdx];
#else
            CEcSlave* pCfgSlave = m_pMaster->m_ppEcSlave[dwCfgSlaveIdx];
#endif
            if (!pCfgSlave->IsPresentOnBus())
            {
                break;
            }
            m_dwResyncCfgSlaveStartIdx++;
        }
#if (defined INCLUDE_HOTCONNECT)
        if (m_dwResyncCfgSlaveStartIdx < m_dwResyncCfgSlavesCnt)
        {
            break;
        }
#endif
    }
    /* determine next state */
#if (defined INCLUDE_HOTCONNECT)
    if (EC_NULL == m_pResyncHcGroupIncomplete)
    {
        m_eResyncState = eResyncState_LookForNextGroup;
    }
    else
#endif
    {
        m_eResyncState = eResyncState_CheckGroupBegin;
    }
    return EC_E_BUSY;
}

/***************************************************************************************************/
/**
\brief  Resynchronize topology detection (after a line break or a bus mismatch)

\return EC_E_NOERROR on bus match, error code otherwise.
*/
CEcBusSlave* CEcBusTopology::GetNextBusSlaveToCheckForResync(EC_T_VOID)
{
    CEcBusSlave* pRetVal       = EC_NULL;
    EC_T_DWORD   dwCfgSlaveIdx = 0;

    dwCfgSlaveIdx = m_dwResyncCfgSlaveStartIdx + m_dwResyncCfgSlavesProcessed + 1;
    if (dwCfgSlaveIdx < m_dwResyncCfgSlavesCnt)
    {
#if (defined INCLUDE_HOTCONNECT)
        CEcSlave*  pCfgSlaveCur  = m_pResyncHcGroupIncomplete->ppGroupMembers[dwCfgSlaveIdx - 1];
        CEcSlave*  pCfgSlaveNext = m_pResyncHcGroupIncomplete->ppGroupMembers[dwCfgSlaveIdx];
#else
        CEcSlave*  pCfgSlaveCur  = m_pMaster->m_ppEcSlave[dwCfgSlaveIdx - 1];
        CEcSlave*  pCfgSlaveNext = m_pMaster->m_ppEcSlave[dwCfgSlaveIdx];
#endif
        EC_T_DWORD dwPrevPortIdx = 0;

        for (dwPrevPortIdx = 0; dwPrevPortIdx < pCfgSlaveNext->GetNumPreviousPorts(); dwPrevPortIdx++)
        {
            EC_T_SLAVE_PORT_DESC* pPrevPortDesc     = &pCfgSlaveNext->m_pPreviousPort[dwPrevPortIdx];
            EC_T_WORD             wPortPrev         = pPrevPortDesc->wPortNumber;
            EC_T_BOOL             bIgnorePrevPort   = (0 != (wPortPrev & IGNORE_PREV_PORT_FLAG));
            CEcSlave*             pCfgSlavePrev     = EC_NULL;
            CEcBusSlave*          pScanBusSlavePrev = EC_NULL;

            if (EC_NULL == pPrevPortDesc)
            {
                continue;
            }
            if (bIgnorePrevPort)
            {
                pCfgSlavePrev = pCfgSlaveCur;
            }
            else
            {
                pCfgSlavePrev = m_pMaster->GetSlaveByCfgFixedAddr(pPrevPortDesc->wSlaveAddress);                
            }
            if (EC_NULL != pCfgSlavePrev)
            {
                pScanBusSlavePrev = (EC_NULL != pCfgSlavePrev->m_pBusSlave) ? pCfgSlavePrev->m_pBusSlave : pCfgSlavePrev->m_pBusSlaveCandidate;
            }
            if (EC_NULL != pScanBusSlavePrev)
            {
                if (bIgnorePrevPort)
                {
                    pRetVal = pScanBusSlavePrev->GetNext();
                }
                else
                {
                    pRetVal = pScanBusSlavePrev->m_apBusSlaveChildren[wPortPrev];
                }
            }
            break;
        }
    }
    return pRetVal;
}

EC_T_DWORD CEcBusTopology::DoTopologyDetectionResync(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
    EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug >> 2) & 3); EC_UNREFPARM(byDbgLvl);
#endif
    EC_T_DWORD dwRetVal         = EC_E_ERROR;
    EC_T_DWORD dwRes            = EC_E_ERROR;
    EC_T_DWORD dwCfgSlaveIdx    = 0;
    CEcSlave*  pCfgSlave        = EC_NULL;
    EC_T_BOOL  bResynchronized  = EC_FALSE;

    OsDbgAssert(ECBT_SLAVELIST_PORT == m_dwScanNextSlaveList);

    /* slave already identified (e.g. at line break), resync on next slave */
    if ((EC_NULL != m_pScanBusSlaveCur) && (EC_NULL != m_pScanBusSlaveCur->GetCfgSlave()))
    {
        m_pScanBusSlaveCur = m_pScanBusSlaveCur->GetNext();
        bResynchronized = EC_TRUE;
        goto Exit;
    }
    if (eResyncState_CheckGroupBegin == m_eResyncState)
    {
        EC_T_BOOL bMismatchDetected = EC_FALSE;

        for (dwCfgSlaveIdx = m_dwResyncCfgSlaveStartIdx + m_dwResyncCfgSlavesProcessed; dwCfgSlaveIdx < m_dwResyncCfgSlavesCnt; dwCfgSlaveIdx++, m_dwResyncCfgSlavesProcessed++)
        {
#if (defined INCLUDE_HOTCONNECT)
            pCfgSlave = m_pResyncHcGroupIncomplete->ppGroupMembers[dwCfgSlaveIdx];
#else
            pCfgSlave = m_pMaster->m_ppEcSlave[dwCfgSlaveIdx];
#endif
            if (EC_NULL == pCfgSlave->m_pBusSlave)
            {
                m_oSingleSlaveListCur.GetFirstNode()->data = pCfgSlave;
                dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, &m_oSingleSlaveListCur, &pCfgSlave);
#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_SLAVE_IDENTIFICATION)
                if (pCfgSlave->HasIdentificationCheck() && (EC_E_NOERROR == dwRes))
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetectionResync() Slave with AutoInc 0x%04X identified as %d\n",
                        m_pScanBusSlaveCur->GetAutoIncAddress(), pCfgSlave->GetFixedAddr()));
                    LinkSlaveDescriptors(m_pScanBusSlaveCur, pCfgSlave);
                }
#endif
            }
            else
            {
                /* config slave has been already identified */
                dwRes = EC_E_NOERROR;
            }
            switch (dwRes)
            {
            case EC_E_NOERROR:
                pCfgSlave->m_pBusSlaveCandidate = m_pScanBusSlaveCur;

                /* determine next BusSlave */
                m_pScanBusSlaveCur = GetNextBusSlaveToCheckForResync();
                break;
            case EC_E_BUSY:
                /* process the result next time this function will be called */
                dwRetVal = dwRes;
                goto Exit;
            default:
                bMismatchDetected = EC_TRUE;
                break;
            }
            if ((EC_NULL == m_pScanBusSlaveCur) || bMismatchDetected)
            {
                if (dwCfgSlaveIdx + 1 < m_dwResyncCfgSlavesCnt)
                {
                    bMismatchDetected = EC_TRUE;
                }
                break;
            }
        }
        if (!bMismatchDetected)
        {
#if (defined INCLUDE_HOTCONNECT)
            pCfgSlave = m_pResyncHcGroupIncomplete->ppGroupMembers[m_dwResyncCfgSlaveStartIdx];
#else
            pCfgSlave = m_pMaster->m_ppEcSlave[m_dwResyncCfgSlaveStartIdx];
#endif
            if (EC_NULL == pCfgSlave->m_pBusSlave)
            {
                LinkSlaveDescriptors(m_pResyncBusSlave, pCfgSlave);
            }
            m_pScanBusSlaveCur = m_pResyncBusSlave->GetNext();
            bResynchronized = EC_TRUE;
            goto Exit;
        }
        /* determine next state */
        m_pScanBusSlaveCur = m_pResyncBusSlave;
        m_eResyncState = eResyncState_LookForNextGroup;
    }
    if (eResyncState_LookForNextGroup == m_eResyncState)
    {
        EC_T_BOOL bGroupFound = EC_FALSE;

        for (; !bGroupFound && (EC_NULL != m_pScanBusSlaveCur); m_pScanBusSlaveCur = m_pScanBusSlaveCur->GetNext())
        {
#if (defined INCLUDE_HOTCONNECT)
            if (EC_NULL != m_pScanBusSlaveCur->GetCfgSlave())
            {
                pCfgSlave = m_pScanBusSlaveCur->GetCfgSlave();
            }
            else
            {
                /* search only in the HC list */
                dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, &m_oHCSlavesList, &pCfgSlave);
                switch (dwRes)
                {
                case EC_E_NOERROR:
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetectionResync() Slave with AutoInc 0x%04X identified as %d\n",
                        m_pScanBusSlaveCur->GetAutoIncAddress(), pCfgSlave->GetFixedAddr()));
                    LinkSlaveDescriptors(m_pScanBusSlaveCur, pCfgSlave);
                    if (EC_NULL == m_pResyncHcGroupIncomplete)
                    {
                        m_pScanBusSlaveCur = m_pScanBusSlaveCur->GetNext();
                        bResynchronized = EC_TRUE;
                        goto Exit;
                    }
                    bGroupFound = EC_TRUE;
                    break;
                case EC_E_BUSY:
                    /* process the result next time this function will be called */
                    dwRetVal = dwRes;
                    goto Exit;
                case EC_E_NOTFOUND:
                default:
                    break;
                }
            }
#endif /* INCLUDE_HOTCONNECT */
        }
        /* determine next state */
#if (defined INCLUDE_HOTCONNECT)
        if (EC_NULL == m_pResyncHcGroupIncomplete)
        {
            m_eResyncState = eResyncState_ResyncOnNextSlave;
        }
        else
#endif
        {
            EC_T_DWORD dwBusSlavesLeft = m_dwBusSlaves - m_pResyncBusSlave->GetBusIndex();
            if (m_dwResyncCfgSlavesCnt > dwBusSlavesLeft)
            {
                m_dwResyncCfgSlaveStartIdx = m_dwResyncCfgSlavesCnt - dwBusSlavesLeft;
                m_dwResyncCfgSlavesProcessed = 0;
            }
            m_pScanBusSlaveCur = m_pResyncBusSlave;
            m_eResyncState = eResyncState_CheckGroupEnd;
        }
    }
    if (eResyncState_CheckGroupEnd == m_eResyncState)
    {
        EC_T_BOOL bCandidateFound = EC_FALSE;
        EC_T_BOOL bMismatchDetected = EC_FALSE;

        for (dwCfgSlaveIdx = m_dwResyncCfgSlaveStartIdx + m_dwResyncCfgSlavesProcessed; dwCfgSlaveIdx < m_dwResyncCfgSlavesCnt; dwCfgSlaveIdx++, m_dwResyncCfgSlavesProcessed++)
        {
#if (defined INCLUDE_HOTCONNECT)
            pCfgSlave = m_pResyncHcGroupIncomplete->ppGroupMembers[dwCfgSlaveIdx];
#else
            pCfgSlave = m_pMaster->m_ppEcSlave[dwCfgSlaveIdx];
#endif
            if (EC_NULL == pCfgSlave->m_pBusSlave)
            {
                m_oSingleSlaveListCur.GetFirstNode()->data = pCfgSlave;
                dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, &m_oSingleSlaveListCur, &pCfgSlave);
#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_SLAVE_IDENTIFICATION)
                if (pCfgSlave->HasIdentificationCheck() && (EC_E_NOERROR == dwRes))
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetectionResync() Slave with AutoInc 0x%04X identified as %d\n",
                        m_pScanBusSlaveCur->GetAutoIncAddress(), pCfgSlave->GetFixedAddr()));
                    LinkSlaveDescriptors(m_pScanBusSlaveCur, pCfgSlave);
                }
#endif
            }
            else
            {
                /* config slave has been already identified */
                dwRes = EC_E_NOERROR;
            }
            switch (dwRes)
            {
            case EC_E_NOERROR:
                if (!bCandidateFound)
                {
                    m_dwResyncCfgSlaveStartIdx = m_dwResyncCfgSlaveStartIdx + m_dwResyncCfgSlavesProcessed;
                    bCandidateFound = EC_TRUE;
                }
                pCfgSlave->m_pBusSlaveCandidate = m_pScanBusSlaveCur;

                /* determine next BusSlave */
                m_pScanBusSlaveCur = GetNextBusSlaveToCheckForResync();
                break;
            case EC_E_BUSY:
                /* process the result next time this function will be called */
                dwRetVal = dwRes;
                goto Exit;
            default:
                if (bCandidateFound)
                {
                    bMismatchDetected = EC_TRUE;
                }
                break;
            }
            if ((EC_NULL == m_pScanBusSlaveCur) || bMismatchDetected)
            {
                if (dwCfgSlaveIdx + 1 < m_dwResyncCfgSlavesCnt)
                {
                    bMismatchDetected = EC_TRUE;
                }
                break;
            }
        }
        if (bCandidateFound && !bMismatchDetected)
        {
#if (defined INCLUDE_HOTCONNECT)
            pCfgSlave = m_pResyncHcGroupIncomplete->ppGroupMembers[m_dwResyncCfgSlaveStartIdx];
#else
            pCfgSlave = m_pMaster->m_ppEcSlave[m_dwResyncCfgSlaveStartIdx];
#endif
            if (EC_NULL == pCfgSlave->m_pBusSlave)
            {
                LinkSlaveDescriptors(m_pResyncBusSlave, pCfgSlave);
            }
            m_pScanBusSlaveCur = m_pResyncBusSlave->GetNext();
            bResynchronized = EC_TRUE;
            goto Exit;
        }
        /* determine next state */
        m_eResyncState = eResyncState_ResyncOnNextSlave;
    }
    OsDbgAssert(eResyncState_ResyncOnNextSlave == m_eResyncState);
    m_pScanBusSlaveCur = m_pResyncBusSlave->GetNext();
    bResynchronized = EC_TRUE;

Exit:
    /* resynchronization done */
    if (bResynchronized)
    {
        m_eResyncState = eResyncState_Idle;
        dwRetVal = EC_E_NOERROR;
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Start topology detection

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::StartTopologyDetection(EC_T_VOID)
{
    /* reset topology check sub state machine variables */
    m_pScanBusSlaveCur          = EC_NULL;
    m_dwScanNextSlaveList       = ECBT_SLAVELIST_PORT;
    m_eResyncState              = eResyncState_Idle;
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
    m_bLineCrossedNotified      = EC_FALSE;
#endif
    m_bBusMismatchNotified      = EC_FALSE;

#if (defined INCLUDE_HOTCONNECT)
    {
        CEcBusSlave* pBusSlave = EC_NULL;
        for (pBusSlave = m_pBusSlaveRoot; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
        {
            pBusSlave->m_pBusSlaveDuplicate = EC_NULL;
        }
        m_pScanHCGroupCur = m_pMaster->HCGetGroup(0);
    }
#endif /* INCLUDE_HOTCONNECT */

    /* start scan bus process at begin of lists */
    m_pScanBusSlaveCur = m_pBusSlaveRoot;

    return EC_E_NOERROR;
}

/***************************************************************************************************/
/**
\brief  Do topology detection

\return EC_E_NOERROR on bus match, error code otherwise.
*/
EC_T_DWORD CEcBusTopology::DoTopologyDetection(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3); EC_UNREFPARM(byDbgLvl);
#endif
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_T_DWORD dwRes    = EC_E_ERROR;

#if (defined INCLUDE_RED_DEVICE)
    /* resync once if main link is disconnected */
    if (!m_bIsSendEnabledOnMainAtStart && (eResyncState_Idle == m_eResyncState) && (EC_NULL != m_pScanBusSlaveCur) && (0 == m_pScanBusSlaveCur->GetBusIndex()))
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because main link is disconnected\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
        dwRes = StartTopologyDetectionResync(m_pScanBusSlaveCur);
    }
#endif
    /* handle resync (after a line break or a bus mismatch) */
    if (eResyncState_Idle != m_eResyncState)
    {
        dwRes = DoTopologyDetectionResync();
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    /* consecutively validate slaves */
    for (; EC_NULL != m_pScanBusSlaveCur; m_pScanBusSlaveCur = m_pScanBusSlaveCur->GetNext())
    {
    CEcBusSlave* pParentBusSlave = EC_NULL;
    CEcSlave*    pParentCfgSlave = EC_NULL;;
    EC_T_WORD    wPortAtParent   = ESC_PORT_INVALID;
    CEcSlave*    pScanCfgSlave   = EC_NULL;

        /* skip already identified slaves */
        if (EC_NULL != m_pScanBusSlaveCur->GetCfgSlave())
        {
            continue;
        }
        /* identify root slave */
        if (m_pBusSlaveRoot == m_pScanBusSlaveCur)
        {
            CEcSlaveList* pCfgSlaveList = &m_oFirstSlaveList;

#if (defined INCLUDE_HOTCONNECT)
            if (pCfgSlaveList->IsEmpty() || m_pMaster->GetSlaveByIndex(0)->IsOptional())
            {
                pCfgSlaveList = &m_oHCSlavesList;
            }
#endif
            dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, pCfgSlaveList, &pScanCfgSlave);
            switch (dwRes)
            {
            case EC_E_NOERROR:
                break;
            case EC_E_BUSY:
                dwRetVal = dwRes;
                goto Exit;
            default:
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because slave could not be identified\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
                goto Exit;
            }
        }
        else
        {
#if (defined INCLUDE_RED_DEVICE)
            if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart == m_pScanBusSlaveCur->GetBusIndex()))
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because slave at line break\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
                goto Exit;
            }
#endif /* INCLUDE_RED_DEVICE */
            /* get parent bus slave and port */
            pParentBusSlave = m_pScanBusSlaveCur->GetParentSlave();
            if (EC_NULL == pParentBusSlave)
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because slave has no parent\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
                goto Exit;
            }
            /* get parent config slave */
            pParentCfgSlave = pParentBusSlave->GetCfgSlave();
            if (EC_NULL == pParentCfgSlave)
            {
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because slave parent wasn't identified\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
                goto Exit;
            }
            wPortAtParent = m_pScanBusSlaveCur->m_wPortAtParent;

            /* search first ... */
            switch (m_dwScanNextSlaveList)
            {
            case ECBT_SLAVELIST_PORT:     /* ... in the possible children list (mandatory or hot connect with previous port entry), */
                if (ESC_PORT_INVALID != wPortAtParent)
                {
                    dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, &pParentCfgSlave->m_aoPossiblePortChildrenList[wPortAtParent], &pScanCfgSlave);
                }
                else
                {
                    dwRes = EC_E_NOTFOUND;
                }
                break;
#if (defined INCLUDE_HOTCONNECT)
            case ECBT_SLAVELIST_HC: /* ... in the (hot connect) list */
                dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, &m_oHCSlavesList, &pScanCfgSlave);
                break;
#endif /* INCLUDE_HOTCONNECT */
            case ECBT_SLAVELIST_BUSSLAVE: /* ... in the bus children list (Superset-ENI) */
                if (EC_NULL != m_pScanBusSlaveCur->GetPrev()->GetCfgSlave())
                {
                    dwRes = IdentifyBusSlave(m_pScanBusSlaveCur, &m_pScanBusSlaveCur->GetPrev()->GetCfgSlave()->m_oPossibleBusChildrenList, &pScanCfgSlave);
                }
                else
                {
                    dwRes = EC_E_NOTFOUND;
                }
                break;
            default:
                OsDbgAssert(EC_FALSE);
                goto Exit;
            }
            switch (dwRes)
            {
            case EC_E_NOERROR: /* slave successfully identified */
            {
                /* start next search in the port list */
                m_dwScanNextSlaveList = ECBT_SLAVELIST_PORT;
                dwRetVal = dwRes;
            } break;
            case EC_E_BUSY: /* identification commands sent - wait for result */
            {
                dwRetVal = dwRes;
            } goto Exit;
            case EC_E_NOTFOUND: /* slave was not identified, look in the next list */
            {
                switch (m_dwScanNextSlaveList)
                {
                case ECBT_SLAVELIST_PORT:
#if (defined INCLUDE_HOTCONNECT)
                    m_dwScanNextSlaveList = ECBT_SLAVELIST_HC;
                    dwRetVal = EC_E_BUSY;
                    break;
                case ECBT_SLAVELIST_HC:
#endif /* INCLUDE_HOTCONNECT */
                    m_dwScanNextSlaveList = ECBT_SLAVELIST_BUSSLAVE;
                    dwRetVal = EC_E_BUSY;
                    break;
                case ECBT_SLAVELIST_BUSSLAVE:
                    m_dwScanNextSlaveList = ECBT_SLAVELIST_PORT;
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because slave wasn't identified\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                    dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                }
            } goto Exit;
            case EC_E_IDENTIFICATIONFAILED: /* identification command failed */
            {
                /* start next search in the port list */
                m_dwScanNextSlaveList = ECBT_SLAVELIST_PORT;
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because slave identification failed\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
            } goto Exit;
            case EC_E_DUPLICATE: /* duplicate HC slave found */
            {
                /* start next search in the port list */
                m_dwScanNextSlaveList = ECBT_SLAVELIST_PORT;
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "DoTopologyDetection() Resync at AutoInc 0x%04X because duplicated slave found\n", m_pScanBusSlaveCur->GetAutoIncAddress()));
                dwRetVal = StartTopologyDetectionResync(m_pScanBusSlaveCur);
            } goto Exit;
            default:
            {
                OsDbgAssert(EC_FALSE);
            } goto Exit;
            }
        }
#if (defined INCLUDE_HOTCONNECT)
        /* track found HC group */
        if (pScanCfgSlave->IsHCGroupHead())
        {
            EC_T_HOTCONNECT_GROUP* pHCGroupCur = m_pMaster->m_oHotConnect.GetGroup(pScanCfgSlave->GetHCGroupId());

            pHCGroupCur->pPrevGroup = m_pScanHCGroupCur;
            m_pScanHCGroupCur       = pHCGroupCur;
        }
#endif /* INCLUDE_HOTCONNECT */

        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "DoTopologyDetection() Slave with AutoInc 0x%04X identified as %d\n", m_pScanBusSlaveCur->GetAutoIncAddress(), pScanCfgSlave->GetFixedAddr()));

        /* link the different slave descriptors together */
        LinkSlaveDescriptors(m_pScanBusSlaveCur, pScanCfgSlave);
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_MASTER_OBD)
/***************************************************************************************************/
/**
\brief  Store configuration data in master object dictionary
*/
EC_T_VOID CEcBusTopology::StorePresentSlave(CEcBusSlave* pBusSlave, EC_T_DWORD dwODIdx)
{
CEcSlave* pCfgSlave     = EC_NULL;
EC_T_WORD wMbxProtocols = 0;

    /* get config slave */
    pCfgSlave = pBusSlave->GetCfgSlave();
    if ((EC_NULL != pCfgSlave) && pCfgSlave->HasMailBox())
    {
        wMbxProtocols = ((CEcMbSlave*)pCfgSlave)->GetMbxProtocols();
    }
    else
    {
        wMbxProtocols = pBusSlave->GetMbxProtocols();
    }
    /* create slave object in directory */
    m_pMaster->SDOAddSlave(
        dwODIdx,
        pBusSlave->GetVendorId(),
        pBusSlave->GetProductCode(),
        pBusSlave->GetRevisionNo(),
        pBusSlave->GetSerialNo(),
        wMbxProtocols,

        pBusSlave->GetAutoIncAddress(),
        pBusSlave->GetFixedAddress(),
        pBusSlave->GetAliasAddress(),

        pBusSlave->GetPortState(),
        pBusSlave->IsDCSupport(),
        pBusSlave->IsDC64Support(),

        pBusSlave->GetScanBusStatus(),

        pBusSlave->GetSupFmmuEntities(),
        pBusSlave->GetSupSyncManEntities(),
        pBusSlave->GetRamSize(),
        pBusSlave->GetPortDescriptor(),
        pBusSlave->GetESCType(),
        pBusSlave->GetCfgSlave()
        );

    /* Create object dictionary entries for slave. -> "Modular Device Profiles" */
    m_pMaster->SDOAddConnectedSlave(pBusSlave);
}

/***************************************************************************************************/
/**
\brief  Store configuration data in master object dictionary
*/
EC_T_VOID CEcBusTopology::StoreAbsentSlave(CEcSlave* pCfgSlave, EC_T_DWORD dwODIdx)
{
    /* add slave in the master object directory */
    m_pMaster->SDOAddSlave(
        dwODIdx,
        pCfgSlave->GetVendorId(),
        pCfgSlave->GetProductCode(),
        pCfgSlave->GetRevisionNumber(),
        pCfgSlave->GetSerialNumber(),
        pCfgSlave->HasMailBox()?((CEcMbSlave*)(pCfgSlave))->GetMbxProtocols():(EC_T_WORD)0,
        pCfgSlave->m_wAutoIncAddr,
        pCfgSlave->GetCfgFixedAddr(),
        pCfgSlave->m_wFixedAddr,
        0,
        EC_FALSE,
        EC_FALSE,
        (EC_T_DWORD)EC_E_SLAVE_NOT_PRESENT,
        0,
        0,
        0,
        pCfgSlave->GetPhysicsDesc(),
        0,
        pCfgSlave
        );
}
#endif /* INCLUDE_MASTER_OBD */

#if (defined INCLUDE_LINE_CROSSED_DETECTION)
EC_T_VOID CEcBusTopology::NotifyLineCrossed(CEcBusSlave* pBusSlave)
{
EC_T_BOOL bLineCrossedNotified = EC_FALSE;

    if (pBusSlave->IsLineCrossed() || pBusSlave->IsPortANotConnected())
    {
    EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();

        if (EC_NULL != pNotification)
        {
            pBusSlave->GetSlaveProp(&pNotification->uNotification.Notification.desc.CrossedLineDesc.SlaveProp);
            pNotification->uNotification.Notification.desc.CrossedLineDesc.wInputPort = pBusSlave->m_wInputPort;
            m_pMaster->NotifyAndFree(EC_NOTIFY_LINE_CROSSED, pNotification, sizeof(EC_T_LINE_CROSSED_DESC));
        }
        bLineCrossedNotified = EC_TRUE;
    }
    else
    {
        if ( m_bNotifyUnexpectedConnectedPort
#if (defined INCLUDE_RED_DEVICE)
            && (!m_pMaster->IsRedConfigured() || m_bRedEnhancedLineCrossedDetectionEnabled || m_bLineBreakAtStart)
#endif
            )
        {
            /* detect connected port without slave connection */
            for (EC_T_WORD wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
            {
                if (!pBusSlave->IsSlaveConPortX(wPortIdx))
                {
                    continue; /* skip not connected port */
                }
                if (EC_NULL != pBusSlave->m_apBusSlaveChildren[wPortIdx])
                {
                    continue; /* skip port connected to another slave */
                }
                if ((pBusSlave == GetBusSlaveMainPort()) && (GetMainPort() == wPortIdx))
                {
                    continue; /* skip port connected to main link */
                }
#if (defined INCLUDE_RED_DEVICE)
                if ((pBusSlave == GetBusSlaveRedPort()) && (GetRedPort() == wPortIdx))
                {
                    continue; /* skip port connected to red link */
                }
#endif
                if (pBusSlave->IsEL9010ConnectedX(wPortIdx))
                {
                    continue; /* skip port connected to EL9010 */
                }
                {
                EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();

                    if (EC_NULL != pNotification)
                    {
                        pBusSlave->GetSlaveProp(&pNotification->uNotification.Notification.desc.CrossedLineDesc.SlaveProp);
                        pNotification->uNotification.Notification.desc.CrossedLineDesc.wInputPort = wPortIdx;
                        m_pMaster->NotifyAndFree(EC_NOTIFY_LINE_CROSSED, pNotification, sizeof(EC_T_LINE_CROSSED_DESC));
                    }
                    bLineCrossedNotified   = EC_TRUE;
                }
            }
        }
    }
    pBusSlave->SetLineCrossedNotified(bLineCrossedNotified);
    if (bLineCrossedNotified)
    {
        m_bLineCrossedNotified = EC_TRUE;
    }
}
#endif /* INCLUDE_LINE_CROSSED_DETECTION */

#if (defined INCLUDE_JUNCTION_REDUNDANCY)
EC_T_VOID CEcBusTopology::NotifyJunctionRedChange(CEcBusSlave* pBusSlave)
{
    if (pBusSlave->IsJunctionRedChange())
    {
        pBusSlave->SetJunctionRedChange(EC_FALSE);
        m_pMaster->NotifyJunctionRedChange(pBusSlave);
    }
    return;
}
#endif /* INCLUDE_JUNCTION_REDUNDANCY */

EC_T_VOID CEcBusTopology::NotifyPdiWatchdog(CEcBusSlave* pBusSlave)
{
    if (pBusSlave->PdiWdExpired())
    {
        if (!pBusSlave->IsDeviceEmulation())
        {
            EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();
            if (EC_NULL != pNotification)
            {
                EC_T_PDIWATCHDOG_DESC* pPdiWatchdog = &(pNotification->uNotification.ErrorNotification.desc.PdiWatchdogDesc);
                pNotification->uNotification.ErrorNotification.dwNotifyErrorCode = EC_NOTIFY_PDIWATCHDOG;

                pBusSlave->GetSlaveProp(&pPdiWatchdog->SlaveProp);

                m_pMaster->NotifyAndFree(EC_NOTIFY_PDIWATCHDOG, pNotification, SIZEOF_EC_T_ERROR_NOTIFICATION_PDIWATCHDOG);
            }
        }
        pBusSlave->ResetPdiWdExpired();
    }
}
EC_T_VOID CEcBusTopology::NotifyUnexpectedLocation(CEcBusSlave* pBusSlave)
{
    CEcSlave*    pCfgSlave         = pBusSlave->GetCfgSlave();
    CEcBusSlave* pBusSlavePrev     = EC_NULL;
    EC_T_WORD    wFixedAddressPrev = ESC_PORT_INVALID;
    EC_T_WORD    wPortPrev         = ESC_PORT_INVALID;

    OsDbgAssert(EC_NULL != pCfgSlave);

#if (defined INCLUDE_RED_DEVICE)
    if (m_bLineBreakAtStart && (m_wSlavesCntOnMainAtStart == pBusSlave->GetBusIndex()))
    {
        /* slave is at line break, don't check previous port */
        goto Exit;
    }
#endif /* INCLUDE_RED_DEVICE */
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
    if (pBusSlave->IsJunctionRedLineBreak())
    {
        /* slave is at junction red line break, don't check previous port */
        goto Exit;
    }
#endif
    /* get bus previous port information */
    pBusSlavePrev = pBusSlave->m_apBusSlaveChildren[ESC_PORT_A];
    if (EC_NULL != pBusSlavePrev)
    {
        /* look for previous port */
        for (EC_T_WORD wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
        {
            if (pBusSlavePrev->m_apBusSlaveChildren[wPortIdx] == pBusSlave)
            {
                wFixedAddressPrev = pBusSlavePrev->GetFixedAddress();
                wPortPrev = wPortIdx;
                break;
            }
        }
    }
    /* check if previous port information are matching */
    {
        EC_T_BOOL bMismatchDetected = EC_FALSE;

        for (EC_T_DWORD dwPrevPortIdx = 0; dwPrevPortIdx < pCfgSlave->GetNumPreviousPorts(); dwPrevPortIdx++)
        {
            EC_T_SLAVE_PORT_DESC* pPortDesc = &(pCfgSlave->m_pPreviousPort[dwPrevPortIdx]);
            CEcSlave* pPrevCfgSlave = EC_NULL;

            if (  (pPortDesc->wSlaveAddress == wFixedAddressPrev)
                && (pPortDesc->wPortNumber   == wPortPrev))
            {
                /* matching entry found, nothing to do */
                continue;
            }
            if (0 != (pPortDesc->wPortNumber & IGNORE_PREV_PORT_FLAG))
            {
                /* previous port information should be ignored, nothing to do */
                continue;
            }
            pPrevCfgSlave = m_pMaster->GetSlaveByCfgFixedAddr(pPortDesc->wSlaveAddress);
            if ((EC_NULL != pPrevCfgSlave) && pPrevCfgSlave->IgnoreAbsence())
            {
                /* absence should be ignored, nothing to do */
                continue;
            }
            bMismatchDetected = EC_TRUE;
        }
        if (!bMismatchDetected)
        {
            /* no mismatch detected, nothing to do */
            goto Exit;
        }
    }
    pBusSlave->SetScanBusStatus(EC_E_BUSCONFIG_MISMATCH);

    /* notify only 1 bus mismatch during 1 bus scan */
    if (!m_bBusMismatchNotified)
    {
        m_pMaster->NotifyScanBusMismatch(pBusSlave, pBusSlavePrev, wPortPrev);
        m_bBusMismatchNotified = EC_TRUE;
    }

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Notify application about bus mismatch

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_VOID CEcBusTopology::NotifyBusMismatch(CEcBusSlave* pBusSlaveUnexpected, CEcSlave* pCfgSlaveMissing)
{
CEcBusSlave* pBusSlavePrev      = EC_NULL;
EC_T_WORD    wPortPrev          = ESC_PORT_INVALID;
EC_T_BOOL    bBusMismatchNotify = !m_bBusMismatchNotified && GetNotifyUnexpectedBusSlaves();

    /* process unexpected bus slave */
    if (EC_NULL != pBusSlaveUnexpected)
    {
        /* get information about location of the mismatch */
        if (EC_NULL == pBusSlavePrev)
        {
            pBusSlavePrev = pBusSlaveUnexpected->GetParentSlave();
            wPortPrev     = pBusSlaveUnexpected->GetPortAtParent();
        }
        /* check if unexpected bus slave at location of missing config slave */
        if (EC_NULL == pCfgSlaveMissing)
        {
            /* determine eventually missing config slave */
            if (EC_NULL == pBusSlavePrev)
            {
                if (pBusSlaveUnexpected == m_pBusSlaveRoot)
                {
                    pCfgSlaveMissing = m_pMaster->GetSlaveByIndex(0);
                }
                else
                {
                    /* redundancy? */
                }
            }
            else
            {
                if ((EC_NULL != pBusSlavePrev->m_pCfgSlave) && (ESC_PORT_INVALID != wPortPrev))
                {
                CEcSlaveList* pCfgSlaveList = &pBusSlavePrev->m_pCfgSlave->m_aoPossiblePortChildrenList[wPortPrev];

                    if (!pCfgSlaveList->IsEmpty())
                    {
                    CEcSlaveListNode pNode = pCfgSlaveList->GetFirstNode();

                        pCfgSlaveMissing = pCfgSlaveList->GetAt(pNode);
                    }
                }
            }
            /* is config slave really missing? */
            if ((EC_NULL != pCfgSlaveMissing) && (pCfgSlaveMissing->IsPresentOnBus() || pCfgSlaveMissing->IsOptional()))
            {
                /* config slave is not missing */
                pCfgSlaveMissing = EC_NULL;
            }
        }
    }
    /* process missing config slave */
    if (EC_NULL != pCfgSlaveMissing)
    {
        if (EC_NULL == pBusSlaveUnexpected)
        {
        CEcSlave* pCfgSlavePrev = EC_NULL;

            /* get information about location of the mismatch */
			if (EC_NULL == pBusSlavePrev)
			{
				if (0 != pCfgSlaveMissing->GetNumPreviousPorts())
				{
					EC_T_SLAVE_PORT_DESC* pPrevPortDesc = pCfgSlaveMissing->m_pPreviousPort;

					if (0 == (pPrevPortDesc->wPortNumber & IGNORE_PREV_PORT_FLAG))
					{
						pCfgSlavePrev = m_pMaster->GetSlaveByCfgFixedAddr(pPrevPortDesc->wSlaveAddress);
						wPortPrev = pPrevPortDesc->wPortNumber;
					}
					else
					{
						/* port number is marked (0x8000), insert current slave into the possible bus child of previous slave */
						pCfgSlavePrev = m_pMaster->GetSlaveByIndex(pCfgSlaveMissing->m_dwSlaveId - 1);
						wPortPrev = ESC_PORT_INVALID;
					}
					if (EC_NULL != pCfgSlavePrev)
					{
						pBusSlavePrev = pCfgSlavePrev->m_pBusSlave;
					}
				}
			}
            else
            {
                pCfgSlavePrev = pBusSlavePrev->GetCfgSlave();
            }
            /* check if unexpected bus slave at location of missing config slave */
            if (EC_NULL == pBusSlavePrev)
            {
                if (pCfgSlaveMissing == m_pMaster->GetSlaveByIndex(0))
                {
                    pBusSlaveUnexpected = m_pBusSlaveRoot;
                }
                else
                {
                    /* redundancy? */
                }
            }
            else
            {
                if (ESC_PORT_INVALID != wPortPrev)
                {
                    pBusSlaveUnexpected = pBusSlavePrev->m_apBusSlaveChildren[wPortPrev];
                }
            }
            /* is bus slave really unexpected? */
            if ((EC_NULL != pBusSlaveUnexpected) && (EC_NULL != pBusSlaveUnexpected->GetCfgSlave()))
            {
                /* bus slave is not unexpected */
                pBusSlaveUnexpected = EC_NULL;
            }
        }
    }
#if (defined INCLUDE_HOTCONNECT)
    /* fill information about duplicate HC slave and notify application */
    if ((EC_NULL != pBusSlaveUnexpected) && (EC_NULL != pBusSlaveUnexpected->m_pBusSlaveDuplicate))
    {
        pBusSlaveUnexpected->SetScanBusStatus(EC_E_DUPLICATE);

        if (bBusMismatchNotify)
        {
            pCfgSlaveMissing = pBusSlaveUnexpected->m_pBusSlaveDuplicate->GetCfgSlave();
            m_pMaster->NotifyScanBusDuplicateHcNode(pBusSlaveUnexpected, pBusSlavePrev, wPortPrev, pCfgSlaveMissing);
        }
    }
    else
#endif
    {
        if (EC_NULL != pBusSlaveUnexpected)
        {
            pBusSlaveUnexpected->SetScanBusStatus(EC_E_BUSCONFIG_MISMATCH);
        }
        if (bBusMismatchNotify)
        {
            m_pMaster->NotifyScanBusMismatch(pBusSlaveUnexpected, pBusSlavePrev, wPortPrev, pCfgSlaveMissing);
        }
    }
    if (bBusMismatchNotify)
    {
        m_bBusMismatchNotified = EC_TRUE;
    }
    return;
}

/***************************************************************************************************/
/**
\brief  Enqueue new Request for BusScan Instance.

\return EC_E_NOERROR on success error code otherwise.
*/
EC_T_DWORD CEcBusTopology::RequestState(
    ECBTSM_T_ESTATES    eState          /**< [in]   Desired state */
   ,EC_T_DWORD          dwTimeout       /**< [in]   Request Timeout */
   ,ECBTSM_REQ**        pHandle         /**< [out]  Handle to Request if EC_E_NOERROR is returned */
                                       )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    if (EC_NULL == pHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check whether state requested matches one of the valid target states */
    switch (eState)
    {
    case ecbtsm_busscan_automatic:
    case ecbtsm_busscan_manual:
    case ecbtsm_busscan_accepttopo:
    case ecbtsm_dlstatus_ist:
    case ecbtsm_alstatusrefresh:
        break;
    default:
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* check whether request "queue" has a free position */
    if ((m_oRequest[0].bUsed) && (m_oRequest[1].bUsed))
    {
        dwRetVal = EC_E_BUSY;
        goto Exit;
    }

    OsDbgAssert(EC_NULL == m_pRequest);
    if (EC_NULL == m_pRequest)
    {
        /* allocate a request */
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
        (*pHandle)->oTimeout.Start(dwTimeout, m_pMaster->GetMsecCounterPtr());
         m_pRequest = (*pHandle);
        dwRetVal = EC_E_NOERROR;
    }
    else
    {
        /* if a request is still placed into waiting position, respond busy */
        dwRetVal = EC_E_BUSY;
    }

Exit:
    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Release handle after reading result code.
*/
EC_T_VOID CEcBusTopology::ReleaseReq(
    ECBTSM_REQ*     pHandle     /**< [in]   desired request to release */
                                     )
{
    /* is handle one of two possible ? */
    OsDbgAssert(( pHandle == &(m_oRequest[0])) || (pHandle == &(m_oRequest[1])) );

    OsDbgAssert(pHandle != m_pCurRequest );
    /* handle in correct state, mark it as free */
    pHandle->bUsed = EC_FALSE;
}

#if (defined INCLUDE_HOTCONNECT)
/***************************************************************************************************/
/**
\brief  (BorderClose) Count Free Hook List elements not used within this Run.

\return Free Hook List element counter.
*/
EC_T_DWORD CEcBusTopology::CountUnusedFreeHookElements(EC_T_VOID)
{
EC_T_DWORD       dwUnusedFreeHookElements = 0;
CEcSlaveListNode pNode     = EC_NULL;
CEcSlave*        pCfgSlave = EC_NULL;

    /* filter head slave with slave list if required */
    for (pNode = m_oHCSlavesList.GetFirstNode(); EC_NULL != pNode; m_oHCSlavesList.GetNext(pNode))
    {
        pCfgSlave = m_oHCSlavesList.GetAt(pNode);
        if ((0 == pCfgSlave->GetNumPreviousPorts()) && (EC_NULL == pCfgSlave->m_pBusSlave))
        {
            dwUnusedFreeHookElements++;
        }
    }
    return dwUnusedFreeHookElements;
}
#endif /* INCLUDE_HOTCONNECT */

/***************************************************************************************************/
/**
\brief  (BorderClose) Determine whether a slave port state change is required.
        DWORD: BYTE:BYTE:BYTE:BYTE = P3:P2:P1:P0

\return 0: No Op, PORT_OPEN_FLAG: Open Port, PORT_CLOSE_FLAG: Close Port, each byte carries one Port !.
*/
EC_T_DWORD CEcBusTopology::ChangePortAnalysis(CEcBusSlave* pBusSlave)
{
EC_T_DWORD dwPortOperation = 0;
EC_T_WORD  wPortIdx        = ESC_PORT_INVALID;

    /* now check each down-link port */
    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_BYTE byPortOperation = 0;

        if (ESC_PORT_A == wPortIdx)
        {
            /* skip Port A */
            continue;
        }
        /* change only not connected ports */
        if (EC_NULL == pBusSlave->m_apBusSlaveChildren[wPortIdx])
        {
        CEcSlave*     pCfgSlave     = EC_NULL;
        CEcSlaveList* pCfgSlaveList = EC_NULL;

            pCfgSlave = pBusSlave->GetCfgSlave();
            if (EC_NULL != pCfgSlave)
            {
                pCfgSlaveList = &pCfgSlave->m_aoPossiblePortChildrenList[wPortIdx];
                if (!pCfgSlaveList->IsEmpty())
                {
                CEcSlaveListNode pNode = EC_NULL;
                    pCfgSlave = EC_NULL;

                    /* check if the expected slaves at this port are connected somewhere else */
                    for (pNode = pCfgSlaveList->GetFirstNode(); EC_NULL != pNode; pCfgSlaveList->GetNext(pNode))
                    {
                        pCfgSlave = pCfgSlaveList->GetAt(pNode);
                        if (EC_NULL == pCfgSlave->m_pBusSlave)
                        {
                            break;
                        }
                    }
                    if (EC_NULL == pNode)
                    {
                        /* all expected slaves at this port are connected */
                        byPortOperation = PORT_CLOSE_FLAG;
                    }
                    else
                    {
                        /* one expected slave is not connected */
                        byPortOperation = PORT_OPEN_FLAG;
                    }
                }
                else
                {
                    /* close port if no slaves are expected at this port */
                    byPortOperation = PORT_CLOSE_FLAG;
                }
            }
            else
            {
                /* close port if no slaves are expected at this port */
                byPortOperation = PORT_CLOSE_FLAG;
            }
        }
        /* store result value */
        dwPortOperation |= (EC_T_DWORD)(byPortOperation << (8*wPortIdx));
    }
    return dwPortOperation;
}

/***************************************************************************************************/
/**
\brief  Top Level ScanBus State Machine.
*/
EC_T_VOID CEcBusTopology::BTStateMachine( EC_T_VOID )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
EC_T_DWORD   dwRes            = EC_E_ERROR;
EC_T_BOOL    bContinueSM      = EC_FALSE;
EC_T_INT     nSlaveSliceCount = 0;
CEcSlave*    pCfgSlave        = EC_NULL;
EC_T_DWORD   dwEcatCmdPending = m_dwEcatCmdPending;
ECBTSM_T_ESTATES eOldState    = ecbtsm_unknown;

    /* handle topology change delay */    
    if (m_oTopologyChangeDelayTimeout.IsStarted())
    {
        if (m_oTopologyChangeDelayTimeout.IsElapsed())
        {
            m_oTopologyChangeDelayTimeout.Stop();
            SetTopologyChanged();
        }
    }
    /* refresh send masks here if send is disabled and topology change    */
    /* delay is not running. In case of cable redundancy line break, they */
    /* will be refreshed in frame handler in CEcDevice::ProcessFrame()    */
    if (!m_poEthDevice->IsSendEnabled() && !m_oTopologyChangeDelayTimeout.IsStarted())
    {
        if (m_bSendMaskedOnMain && m_poEthDevice->IsMainLinkConnected())
        {
            m_bSendMaskedOnMain = EC_FALSE;
        }
#if (defined INCLUDE_RED_DEVICE)
        if (m_bSendMaskedOnRed && m_poEthDevice->IsRedLinkConnected())
        {
            m_bSendMaskedOnRed = EC_FALSE;
        }
#endif
    }
    /* check if state machine is idle */
    if( (m_eCurState == m_eReqState) && (EC_NULL == m_pCurRequest) )
    {
        /* stable, check for queued request */
        if( EC_NULL != m_pRequest )
        {
            /* Fetch new Request */
            m_pCurRequest = m_pRequest;
            /* free Request queue */
            m_pRequest    = EC_NULL;

            /* store current request as target for sm */
            m_eReqState   = m_pCurRequest->eState;

            /* start state Machine */
            m_eCurState   = ecbtsm_start;
            bContinueSM   = EC_TRUE;
        }
        else
        {
            /* no new request leave SM */
            bContinueSM   = EC_FALSE;
        }
    }
    else
    {
        /* request still pending, go into SM */
        bContinueSM       = EC_TRUE;
    }

    /* if requested state is reached, store result and throw out request */
    if( (m_eCurState == m_eReqState) && (EC_NULL != m_pCurRequest) )
    {
        m_pCurRequest->oTimeout.Stop();
        m_pCurRequest = EC_NULL;
        bContinueSM = EC_FALSE;
    }

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_BTSM);
    while (bContinueSM)
    {
        /* only do a single run of SM per default */
        bContinueSM = EC_FALSE;

        /* store previous SM state to send StateChange Debug Messages */
        eOldState = m_eCurState;

        /* if current request is missing, and state machine is not stable -> stuck */
        if( EC_NULL == m_pCurRequest && m_eCurState != m_eReqState && m_eCurState != ecbtsm_idle )
        {
            OsDbgAssert(EC_FALSE);
            m_eCurState = m_eReqState = ecbtsm_stuck;
            continue;
        }
        /* execute sub state machine */
        BTSubStateMachine();

        /* execute EEPROM sub state machine */
        PerfMeasStart(&G_TscMeasDesc, EC_MSMT_BTEEPSM);
        BTEEPStateMachine();
        PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_BTEEPSM);

        /* execute state machine */
        switch( m_eCurState )
        {
        /* reset state machine variables and slaves variables, according requested state */
        case ecbtsm_start:
            {
                /* reset state machine variables */
                m_dwEcatCmdPending = 0;
                m_dwBTResult = EC_E_BUSY;
                m_bBusMismatchNotified = EC_FALSE;

                /* determine next state */
                m_eCurState = ecbtsm_restart;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_restart:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* release temporary fixed address */
                ReleaseTmpFixedAddress();
                
                /* request specific reset */
                switch(m_eReqState)
                {
                case ecbtsm_busscan_accepttopo:
                    SetTopologyChanged();
                    /* no break */

                case ecbtsm_busscan_automatic:
                case ecbtsm_busscan_manual:

                    /* if master is not configured, rescan the bus completely */
                    if (!m_pMaster->IsConfigured() && (0 != m_dwBusSlaves))
                    {
                        m_bPurgeBusSlavesAndNotify = EC_TRUE;
                    }
#if (defined INCLUDE_MASTER_OBD)
                    /* clear slave list in OBD */
                    m_pMaster->SDOClearSlaves((EC_T_WORD)m_dwBusSlaves);
#endif
                    /* If master is not configured read more information from */
                    /* slaves. Needed to eventually generate ENI file. */
                    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_BOOTRCVMBX]  = (!m_pMaster->IsConfigured() || m_bNextBusScanGenEni);
                    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_BOOTSNDMBX]  = (!m_pMaster->IsConfigured() || m_bNextBusScanGenEni);
                    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_STDRCVMBX]   = (!m_pMaster->IsConfigured() || m_bNextBusScanGenEni);
                    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_STDSNDMBX]   = (!m_pMaster->IsConfigured() || m_bNextBusScanGenEni);
                    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_MBXPROTOCOL] = (!m_pMaster->IsConfigured() || m_bNextBusScanGenEni);
#if (defined INCLUDE_GEN_OP_ENI)
                    m_abReadEEPROMEntry[READEEPROMENTRY_IDX_CATEGORIES]  = (!m_pMaster->IsConfigured() || m_bNextBusScanGenEni);
#endif
                    m_bNextBusScanGenEni = EC_FALSE;
                    break;
                case ecbtsm_dlstatus_ist:
                case ecbtsm_alstatusrefresh:
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = ecbtsm_stuck;
                    break;
                }
                /* handle link disconnection/reconnection */
                {
                    m_bLinkConnectedAtStart       = m_poEthDevice->IsLinkConnected();
#if (defined INCLUDE_RED_DEVICE)
                    m_bIsSendEnabledOnMainAtStart = m_poEthDevice->IsSendEnabledOnMain();
                    m_bIsSendEnabledOnRedAtStart  = m_poEthDevice->IsSendEnabledOnRed();
                    m_bLineBreakAtStart           = m_pMaster->m_bRedLineBreak;
                    m_wSlavesCntOnMainAtStart     = m_pMaster->m_wRedNumSlavesOnMain;
                    m_wSlavesCntOnRedAtStart      = m_pMaster->m_wRedNumSlavesOnRed;
#endif
                    if (m_bLinkWasDisconnected || !m_bLinkConnectedAtStart)
                    {
                        m_bLinkWasDisconnected = EC_FALSE;
                        if (0 != m_dwBusSlaves)
                        {
                            m_bPurgeBusSlavesAndNotify = EC_TRUE;
                        }
                        if (!m_bLinkConnectedAtStart)
                        {
#if (defined INCLUDE_HOTCONNECT)
                        EC_T_HOTCONNECT_GROUP* pHCGroup = m_pMaster->m_oHotConnect.GetGroup(0);

                            if (!m_pMaster->IsConfigured() || ((EC_NULL != pHCGroup) && (pHCGroup->dwGrpMembers != 0)))
                            {
                                m_dwBTResult = EC_E_LINK_DISCONNECTED;
                            }
#else
                            m_dwBTResult = EC_E_LINK_DISCONNECTED;
#endif /* INCLUDE_HOTCONNECT */
                        }
                    }
                }
                /* prepare next state */
                m_pBusSlaveCur = m_pBusSlaveRoot;
                m_dwCfgSlaveIdxCur = 0;

                /* determine next state */
                m_eCurState = ecbtsm_start_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_start_wait:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    /* purge or remove new bus slaves */
                    for (;
                        (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_bPurgeBusSlavesAndNotify || m_pBusSlaveCur->IsNew());
                         nSlaveSliceCount--)
                    {
                        CEcBusSlave* pBusSlaveNext = m_pBusSlaveCur->GetNext();

                        RemoveBusSlave(m_pBusSlaveCur);
                        m_pBusSlaveCur = pBusSlaveNext;
                    }
                    if ((nSlaveSliceCount <= 0) || (m_pBusSlaveCur == EC_NULL))
                    {
                        break;
                    }
                    /* invalidate bus slave (to remove them later in ecbtsms_writetmpaddr) */
                    m_pBusSlaveCur->SetScanBusStatus(EC_E_INVALIDDATA);
                        
                    /* request specific reset */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
                        m_pBusSlaveCur->ResetRecvTimes();
#endif
                        break;
                    case ecbtsm_dlstatus_ist:
                    case ecbtsm_alstatusrefresh:
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = ecbtsm_stuck;
                        break;
                    }
                }
                /* invalidate config slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_dwCfgSlaveIdxCur < m_pMaster->GetCfgSlaveCount());
                     nSlaveSliceCount--,       m_dwCfgSlaveIdxCur++)
                {
                    pCfgSlave = m_pMaster->GetSlaveByIndex(m_dwCfgSlaveIdxCur);
                    if (EC_NULL == pCfgSlave)
                    {
                        OsDbgAssert(EC_FALSE);
                        continue;
                    }
                    if (m_bPurgeBusSlavesAndNotify)
                    {
                        pCfgSlave->SetSlavePresence(EC_FALSE);
                    }
                    else
                    {
                        pCfgSlave->InvalidatePresence();
                    }
                }
                /* determine next state */
                if ((EC_NULL == m_pBusSlaveCur) && (m_dwCfgSlaveIdxCur >= m_pMaster->GetCfgSlaveCount()))
                {
                    m_eCurState = ecbtsm_start_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_start_done:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* determine next state */
                if (m_bPurgeBusSlavesAndNotify)
                {
                    OsDbgAssert(0 == m_dwBusSlaves);

                    /* would be normally done in collect info */
                    m_dwSlavesAtStart = 0;

                    if (!m_pMaster->IsConfigured())
                    {
                        m_bPurgeBusSlavesAndNotify = EC_FALSE;
                        m_eCurState = ecbtsm_restart;
                    }
                    else
                    {
                        /* m_bPurgeBusSlavesAndNotify reset in ecbtsm_notify_application_done */
                        m_eCurState = ecbtsm_refresh_slaves_presence;
                    }
                    bContinueSM = EC_TRUE;
                    break;
                }
                if (!m_bLinkConnectedAtStart)
                {
                    m_eCurState = ecbtsm_refresh_slaves_presence;
                    bContinueSM = EC_TRUE;
                    break;
                }
                m_eCurState = ecbtsm_collectinfo;
                bContinueSM = EC_TRUE;
            } break;

        /* collect bus information, in this state slave objects are created and EEPROM are read from slave nodes */
        case ecbtsm_collectinfo:
            {
                /* start sub state machine */
                switch(m_eReqState)
                {
                case ecbtsm_busscan_automatic:
                case ecbtsm_busscan_manual:
                case ecbtsm_busscan_accepttopo:
                {
                    m_eReqSubState = ecbtsms_collectbusscan;
                } break;
                case ecbtsm_dlstatus_ist:
                {
                    m_eReqSubState = ecbtsms_collectdlstatus;
                } break;
                case ecbtsm_alstatusrefresh:
                {
                    m_eReqSubState = ecbtsms_collectalstatus;
                } break;
                default:
                    break;
                }
                /* wait for sub state machine */
                m_eCurState = ecbtsm_collectinfo_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_collectinfo_wait:
            {
                /* wait for sub state machine */
                if (m_eCurSubState == m_eReqSubState)
                {
                    /* make sub state machine stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;

                    /* determine next state */
                    m_eCurState = ecbtsm_collectinfo_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_collectinfo_done:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    /* topology changes are expected during DLSTATUS handling, don't restart */
                    if ((m_eReqState != ecbtsm_dlstatus_ist))
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Topology changed while scan in %s\n", ESTATESText(m_eCurState)));
                        m_eCurState = ecbtsm_restart;
                        bContinueSM = EC_TRUE;
                        break;
                    }
                }
                /* evaluate sub state machine result */
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Sub state machine returns 0x%04X in %s\n", m_dwSubResult, ESTATESText(m_eCurState)));
                switch (m_dwSubResult)
                {
                case EC_E_NOERROR:
                {
                    /* determine next state */
                    switch (m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
                        if (m_pMaster->IsConfigured())
                        {
                            m_eCurState = ecbtsm_checktopo;
                        }
                        else
                        {
                            m_eCurState = ecbtsm_refresh_slaves_presence;
                        }
                        bContinueSM = EC_TRUE;
                        break;
                    case ecbtsm_dlstatus_ist:
                        m_eCurState = ecbtsm_checkdlstatusint;
                        bContinueSM = EC_TRUE;
                        break;
                    case ecbtsm_alstatusrefresh:
                        m_eCurState = ecbtsm_refresh_slaves_presence;
                        bContinueSM = EC_TRUE;
                        break;
                    case ecbtsm_reqto:
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                        m_eCurState = ecbtsm_reqto_wait;
                        bContinueSM = EC_TRUE;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = ecbtsm_stuck;
                        break;
                    }
                } break;
                case EC_E_BUSCONFIG_TOPOCHANGE:
                {
                    /* determine next state */
                    switch (m_eReqState)
                    {
                    case ecbtsm_busscan_manual:
                        if (m_pMaster->IsConfigured())
                        {
                            m_eCurState = ecbtsm_checktopo;
                        }
                        else
                        {
                            m_eCurState = ecbtsm_refresh_slaves_presence;
                        }
                        break;
                    case ecbtsm_dlstatus_ist:
                        SetTopologyChanged();

                        /* topology changes are expected during DLSTATUS handling, don't restart */
                        m_eCurState = ecbtsm_checkdlstatusint;
                        break;
                    case ecbtsm_alstatusrefresh:
                        SetTopologyChanged();
                    default:
                        /* restart bus scan */
                        m_eCurState = ecbtsm_restart;
                        break;
                    }
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_LINK_DISCONNECTED:
                {
                    /* determine next state */
                    if (0 != m_dwBusSlaves)
                    {
                        switch (m_eReqState)
                        {
                        case ecbtsm_dlstatus_ist:
                        case ecbtsm_alstatusrefresh:
                            SetTopologyChanged();
                            break;
                        default:
                            /* nothing to do */
                            break;
                        }
                        m_bPurgeBusSlavesAndNotify = EC_TRUE;
                        m_eCurState = ecbtsm_restart;
                    }
                    else
                    {
                        m_eCurState = ecbtsm_notify_application;
                    }
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_MAX_BUS_SLAVES_EXCEEDED:
                case EC_E_NOMEMORY:
                {
                    m_dwBTResult = m_dwSubResult;
                    m_eCurState = ecbtsm_error;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_TIMEOUT:
                {
                    m_pCurRequest->oTimeout.Stop();
                    m_eReqState = ecbtsm_reqto;
                    m_eCurState = ecbtsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_NOTSUPPORTED:
                default:
                {
                    /* this should never happen and is only to recover from fatal error */
                    OsDbgAssert(EC_FALSE);

                    /* make sub state machines stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* restart state machine */
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                }
            } break;

        /* check network topology information against the ENI file */
        case ecbtsm_checktopo:
            {
                /* start topology check sub state machine */
                m_eReqChkState = ecbtsmchk_done;

                /* wait for topology check sub state machine */
                m_eCurState = ecbtsm_checktopo_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_checktopo_wait:
            {
                /* trigger topology check sub state machine */
                BTChkStateMachine();

                /* wait for topology check sub state machine */
                if (m_eCurChkState == m_eReqChkState )
                {
                    /* make check state machine stable and idle */
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* determine next state */
                    m_eCurState = ecbtsm_checktopo_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_checktopo_done:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Topology changed while scan in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
#if (defined INCLUDE_LOG_MESSAGES)
                if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
                {
#if (defined ECBT_PRINTBUSSLAVESTREE)
#if (defined INCLUDE_RED_DEVICE)
                    if (m_pMaster->m_bRedLineBreak)
                    {
                        CEcSlave* pCfgSlave = m_pMaster->GetSlaveByIndex(m_wSlavesCntOnMainAtStart);

                        if (0 != m_wSlavesCntOnMainAtStart)
                        {
                            PrintBusSlaveTree(m_pBusSlaveRoot, EC_FALSE);
                        }
                        if ((0 != m_wSlavesCntOnRedAtStart) && (EC_NULL != pCfgSlave))
                        {
                            PrintBusSlaveTree(pCfgSlave->m_pBusSlave, EC_TRUE);
                        }
                    }
                    else
#endif /* INCLUDE_RED_DEVICE */
                    {
                        PrintBusSlaveTree(m_pBusSlaveRoot, EC_FALSE);
                    }
#endif
                    PrintTopologyTest(m_pBusSlaveRoot);
                }
#endif
                /* evaluate topology check state machine result */
                switch (m_dwChkResult)
                {
                case EC_E_NOERROR:
                case EC_E_BUSCONFIG_MISMATCH:
                case EC_E_LINE_CROSSED:
                {
                    /* determine next state */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
                        m_eCurState = ecbtsm_writefixaddr;
                        bContinueSM = EC_TRUE;
                        break;
                    case ecbtsm_reqto:
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                        m_eCurState = ecbtsm_reqto_wait;
                        bContinueSM = EC_TRUE;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = ecbtsm_stuck;
                        break;
                    }
                } break;                
                case EC_E_BUSCONFIG_TOPOCHANGE:
                {
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_TIMEOUT:
                {
                    OsDbgAssert(m_eReqState == ecbtsm_reqto);
                    m_pCurRequest->oTimeout.Stop();
                    m_eReqState = ecbtsm_reqto;
                    m_eCurState = ecbtsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_NOTSUPPORTED:
                default:
                {
                    /* this should never happen and is only to recover from fatal error */
                    OsDbgAssert(EC_FALSE);

                    /* make sub state machines stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* restart state machine */
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                }
            }break;

        /* write fixed addresses from ENI file to slave nodes */
        case ecbtsm_writefixaddr:
            {
                /* start sub state machine */
                m_eReqSubState = ecbtsms_wrtfaddr;

                /* wait for sub state machine */
                m_eCurState = ecbtsm_writefixaddr_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_writefixaddr_wait:
            {
                /* wait for sub state machine */
                if (m_eCurSubState == m_eReqSubState )
                {
                    /* make sub state machine stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;

                    /* determine next state */
                    m_eCurState = ecbtsm_writefixaddr_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_writefixaddr_done:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Topology changed while scan in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* evaluate sub state machine result */
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Sub state machine returns 0x%04X in %s\n", m_dwSubResult, ESTATESText(m_eCurState)));
                switch (m_dwSubResult)
                {
                case EC_E_NOERROR:
                {
                    /* determine next state */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
#if (defined INCLUDE_HOTCONNECT) && (defined INCLUDE_PORT_OPERATION)
                        if ((EC_E_BUSCONFIG_MISMATCH != m_dwBTResult) && (m_pMaster->m_oHotConnect.IsBorderCloseActive()))
                        {
                            m_eCurState = ecbtsm_borderclose;
                            bContinueSM = EC_TRUE;
                        }
                        else
#endif /* INCLUDE_HOTCONNECT && INCLUDE_PORT_OPERATION */
                        {
                            m_eCurState = ecbtsm_refresh_slaves_presence;
                            bContinueSM = EC_TRUE;
                        }
                        break;
                    case ecbtsm_reqto:
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                        m_eCurState = ecbtsm_reqto_wait;
                        bContinueSM = EC_TRUE;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = ecbtsm_stuck;
                        break;
                    }
                } break;
                case EC_E_BUSCONFIG_TOPOCHANGE:
                {
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_TIMEOUT:
                {
                    OsDbgAssert(m_eReqState == ecbtsm_reqto);
                    m_pCurRequest->oTimeout.Stop();
                    m_eReqState = ecbtsm_reqto;
                    m_eCurState = ecbtsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_NOTSUPPORTED:
                default:
                {
                    /* this should never happen and is only to recover from fatal error */
                    OsDbgAssert(EC_FALSE);

                    /* make sub state machines stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* restart state machine */
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                }
            } break;

        /* perform border close, if desired in HotConnect mode */
#if (defined INCLUDE_HOTCONNECT)
        case ecbtsm_borderclose:
            {
                /* start sub state machine */
                m_eReqSubState = ecbtsms_borderclose;

                /* wait for sub state machine */
                m_eCurState = ecbtsm_borderclose_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_borderclose_wait:
            {
                /* wait for sub state machine */
                if (m_eCurSubState == m_eReqSubState)
                {
                    /* make sub state machine stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;

                    /* determine next state */
                    m_eCurState = ecbtsm_borderclose_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_borderclose_done:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Topology changed while scan in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* evaluate sub state machine result */
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Sub state machine returns 0x%04X in %s\n", m_dwSubResult, ESTATESText(m_eCurState)));
                switch (m_dwSubResult)
                {
                case EC_E_NOERROR:
                {
                    /* determine next state */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
                        if( m_bSlaveAliasAddressing )
                        {
                            m_eCurState = ecbtsm_aliasaddressing;
                        }
                        else
                        {
                            m_eCurState = ecbtsm_refresh_slaves_presence;
                        }
                        bContinueSM = EC_TRUE;
                        break;
                    case ecbtsm_reqto:
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                        m_eCurState = ecbtsm_reqto_wait;
                        bContinueSM = EC_TRUE;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = ecbtsm_stuck;
                        break;
                    }
                } break;
                case EC_E_BUSCONFIG_TOPOCHANGE:
                {
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_TIMEOUT:
                {
                    OsDbgAssert(m_eReqState == ecbtsm_reqto);
                    m_pCurRequest->oTimeout.Stop();
                    m_eReqState = ecbtsm_reqto;
                    m_eCurState = ecbtsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_NOTSUPPORTED:
                default:
                {
                    /* this should never happen and is only to recover from fatal error */
                    OsDbgAssert(EC_FALSE);

                    /* make sub state machines stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* restart state machine */
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                }
            } break;
#endif /* INCLUDE_HOTCONNECT */

        /* activate alias addressing on slave nodes, if desired */
        case ecbtsm_aliasaddressing:
            {
                /* start sub state machine */
                m_eReqSubState = ecbtsms_aliasactive;

                /* wait for sub state machine */
                m_eCurState = ecbtsm_aliasaddressing_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_aliasaddressing_wait:
            {
                /* wait for sub state machine */
                if (m_eCurSubState == m_eReqSubState)
                {
                    /* make sub state machine stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;

                    /* determine next state */
                    m_eCurState = ecbtsm_aliasaddressing_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_aliasaddressing_done:
            {
                /* handle timeout request */
                if (m_eReqState == ecbtsm_reqto)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_reqto_wait;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Topology changed while scan in %s\n", ESTATESText(m_eCurState)));
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* evaluate sub state machine result */
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Sub state machine returns 0x%04X in %s\n", m_dwSubResult, ESTATESText(m_eCurState)));
                switch (m_dwSubResult)
                {
                case EC_E_NOERROR:
                {
                    /* determine next state */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
                        m_eCurState = ecbtsm_refresh_slaves_presence;
                        bContinueSM = EC_TRUE;
                        break;
                    case ecbtsm_reqto:
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Timeout in %s\n", ESTATESText(m_eCurState)));
                        m_eCurState = ecbtsm_reqto_wait;
                        bContinueSM = EC_TRUE;
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_eCurState = m_eReqState = ecbtsm_stuck;
                        break;
                    }
                } break;
                case EC_E_BUSCONFIG_TOPOCHANGE:
                {
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_TIMEOUT:
                {
                    OsDbgAssert(m_eReqState == ecbtsm_reqto);
                    m_pCurRequest->oTimeout.Stop();
                    m_eReqState = ecbtsm_reqto;
                    m_eCurState = ecbtsm_reqto_wait;
                    bContinueSM = EC_TRUE;
                } break;
                case EC_E_NOTSUPPORTED:
                default:
                {
                    /* this should never happen and is only to recover from fatal error */
                    OsDbgAssert(EC_FALSE);

                    /* make sub state machines stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* restart state machine */
                    m_eCurState = ecbtsm_restart;
                    bContinueSM = EC_TRUE;
                } break;
                }
            } break;

        /* check interrupt source and set return value apropriate */
        case ecbtsm_checkdlstatusint:
            {
                /* prepare next state */
                m_pBusSlaveCur = m_pBusSlaveRoot;
                m_dwCfgSlaveIdxCur = 0;

                /* determine next state */
                m_eCurState = ecbtsm_checkdlstatusint_wait;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsm_checkdlstatusint_wait:
            {
                /* set scan bus status for all slaves from invalid to no error */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    /* check for new port connection */
                    if (m_pBusSlaveCur->DetectNewPortConnection(m_pMaster))
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + TOPO_CHNG), "DetectNewPortConnection() AutoInc 0x%04X new connection detected\n", m_pBusSlaveCur->GetAutoIncAddress()));

                        /* apply topology change delay */
                        if (m_bTopologyChangeAutoMode && (0 != m_dwTopologyChangeDelay))
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "Start %d ms Topology change delay\n", m_dwTopologyChangeDelay));
                            m_oTopologyChangeDelayTimeout.Start(m_dwTopologyChangeDelay, m_pMaster->GetMsecCounterPtr());
                        }
                        m_dwBTResult = EC_E_DLSTATUS_IRQ_TOPOCHANGED;
                    }
                    /* check for port disconnection */
                    if (m_pBusSlaveCur->DetectPortChanges() && !m_oTopologyChangeDelayTimeout.IsStarted())
                    {
                        m_dwBTResult = EC_E_DLSTATUS_IRQ_TOPOCHANGED;
                    }
                    /* backup current information for next comparaison */
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BackupPortInfos() AutoInc 0x%04X DlStatus 0x%04X\n", m_pBusSlaveCur->GetAutoIncAddress(), m_pBusSlaveCur->GetDlStatus()));
                    m_pBusSlaveCur->BackupPortInfos();
                }
                /* determine next state */
                if (EC_NULL == m_pBusSlaveCur)
                {
                    m_eCurState = ecbtsm_checkdlstatusint_done;
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecbtsm_checkdlstatusint_done:
            {
                if (EC_E_DLSTATUS_IRQ_TOPOCHANGED == m_dwBTResult)
                {
                    if (m_oTopologyChangeDelayTimeout.IsStarted())
                    {
                        m_dwBTResult = EC_E_BUSY;
                    }
                    else
                    {
                        SetTopologyChanged();
                    }
                }
                m_eCurState = ecbtsm_refresh_slaves_presence;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsm_refresh_slaves_presence:
            {
                /* prepare next state */
                m_pBusSlaveCur     = m_pBusSlaveRoot;
                m_dwCfgSlaveIdxCur = 0;
                m_dwBusSlavesDCCur = 0;

                /* determine next state */
                m_eCurState = ecbtsm_refresh_slaves_presence_wait;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsm_refresh_slaves_presence_wait:
            {
                /* renumber and set valid scan bus status for slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    /* scan bus state is valid (maybe error) */
                    if (EC_E_NOTREADY == m_pBusSlaveCur->GetScanBusStatus())
                    {
                        m_pBusSlaveCur->SetScanBusStatus(EC_E_NOERROR);
                    }
#if (defined INCLUDE_DC_SUPPORT)
                    /* count DC bus slaves */
                    if (m_pBusSlaveCur->IsDcEnabled())
                    {
                        m_dwBusSlavesDCCur++;
                    }
#endif /* INCLUDE_DC_SUPPORT */
                }
                /* refresh slaves presence */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_dwCfgSlaveIdxCur < m_pMaster->GetCfgSlaveCount());
                     nSlaveSliceCount--,       m_dwCfgSlaveIdxCur++)
                {
                    pCfgSlave = m_pMaster->GetSlaveByIndex(m_dwCfgSlaveIdxCur);
                    if (EC_NULL == pCfgSlave)
                    {
                        OsDbgAssert(EC_FALSE);
                        continue;
                    }
                    RefreshCfgSlavePresence(pCfgSlave);
#if (defined INCLUDE_HOTCONNECT)
                    m_pMaster->m_oHotConnect.RefreshCfgSlavePresence(pCfgSlave);
#endif
                }
                /* determine next state */
                if ((EC_NULL == m_pBusSlaveCur) && (m_dwCfgSlaveIdxCur >= m_pMaster->GetCfgSlaveCount()))
                {
                    m_eCurState = ecbtsm_refresh_slaves_presence_done;
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecbtsm_refresh_slaves_presence_done:
            {
#if (defined INCLUDE_DC_SUPPORT)
                m_dwBusSlavesDC = m_dwBusSlavesDCCur;
#endif
#if (defined INCLUDE_HOTCONNECT)
                /* recalculate WKC */
                m_pMaster->m_oHotConnect.RecalculateWKC(EC_TRUE);

                /* trigger cyclic frames adjustment */
                m_pMaster->TriggerCycFramesAdjustment();
#endif /* INCLUDE_HOTCONNECT */

                /* determine next state */
                m_eCurState = ecbtsm_storeinfo;
                bContinueSM = EC_TRUE;
            } break;

        /* Store slave node information from topology to Master Object Dictionary */
        case ecbtsm_storeinfo:
            {
#if (defined INCLUDE_MASTER_OBD)
                /* store slave amounts in OBD */
                m_pMaster->SDOSetSlaveAmount(m_dwBusSlaves, m_dwBusSlavesDC);

                /* prepare next state */
                m_pBusSlaveCur = m_pBusSlaveRoot;
                m_dwCfgSlaveIdxCur = 0;
                m_dwAbsentIdxCur = GetConnectedSlaveCount(); /* after last BUS Slave, we start with absent slaves */

                /* determine next state */
                m_eCurState = ecbtsm_storeinfo_wait;
#else
                /* determine next state */
                m_eCurState = ecbtsm_storeinfo_done;
#endif
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsm_storeinfo_wait:
            {
#if (defined INCLUDE_MASTER_OBD)
                /* store present slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    StorePresentSlave(m_pBusSlaveCur, m_pBusSlaveCur->GetBusIndex());
                }
                /* store absent slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_dwCfgSlaveIdxCur < m_pMaster->GetCfgSlaveCount());
                     nSlaveSliceCount--,       m_dwCfgSlaveIdxCur++)
                {
                    pCfgSlave = m_pMaster->GetSlaveByIndex(m_dwCfgSlaveIdxCur);
                    if (EC_NULL == pCfgSlave)
                    {
                        OsDbgAssert(EC_FALSE);
                        continue;
                    }
                    if (EC_NULL == pCfgSlave->m_pBusSlave)
                    {
                        StoreAbsentSlave(pCfgSlave, m_dwAbsentIdxCur);
                        m_dwAbsentIdxCur++;
                    }
                }
                /* determine next state */
                if ((EC_NULL == m_pBusSlaveCur) && (m_dwCfgSlaveIdxCur >= m_pMaster->GetCfgSlaveCount()))
#endif
                {
                    m_eCurState = ecbtsm_storeinfo_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsm_storeinfo_done:
            {
                /* notify application after storeinfo so Master OD is actual */
                m_eCurState = ecbtsm_notify_application;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsm_notify_application:
            {
                /* prepare next state */
                m_pBusSlaveCur = m_pBusSlaveRoot;
                m_dwCfgSlaveIdxCur = 0;
                m_dwBusSlavesUnexpectedCur = 0;

                /* determine next state */
                m_eCurState = ecbtsm_notify_application_wait;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsm_notify_application_wait:
            {
                /* notify config slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_dwCfgSlaveIdxCur < m_pMaster->GetCfgSlaveCount());
                     nSlaveSliceCount--,       m_dwCfgSlaveIdxCur++)
                {
                    pCfgSlave = m_pMaster->GetSlaveByIndex(m_dwCfgSlaveIdxCur);
                    if (EC_NULL == pCfgSlave)
                    {
                        OsDbgAssert(EC_FALSE);
                        continue;
                    }
                    /* slave presence notified as soon as possible */
                    pCfgSlave->NotifyPresence();

                    /* mismatch related notifications only generated in topology change related bus scan */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
                        /* notify application about missing config slave */
                        if (!pCfgSlave->IsPresentOnBus() && !pCfgSlave->IgnoreAbsence())
                        {
#if (defined INCLUDE_HOTCONNECT)
                        EC_T_HOTCONNECT_GROUP* pHCGroup = m_pMaster->m_oHotConnect.GetGroup(pCfgSlave->GetHCGroupId());

                            if ((EC_NULL != pHCGroup) && pHCGroup->bGroupActive)
#endif /* INCLUDE_HOTCONNECT */
                            {
                                NotifyBusMismatch(EC_NULL, pCfgSlave);
                            }
                        } break;
                    case ecbtsm_dlstatus_ist:
                    case ecbtsm_alstatusrefresh:
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        break;
                    }
                }
                /* notify bus slaves */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    /* flooding and mismatch related notifications only generated in topology change related bus scan */
                    switch(m_eReqState)
                    {
                    case ecbtsm_busscan_automatic:
                    case ecbtsm_busscan_manual:
                    case ecbtsm_busscan_accepttopo:
                    
                        m_pBusSlaveCur->ResetIsNew();
                    
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
                        NotifyLineCrossed(m_pBusSlaveCur);
#endif
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
                        NotifyJunctionRedChange(m_pBusSlaveCur);
#endif
                        if (m_pMaster->IsConfigured() && m_bTopologyVerificationEnabled)
                        {
                            if (EC_NULL == m_pBusSlaveCur->GetCfgSlave())
                            {
                                /* handle unexpected bus slaves */
                                m_dwBusSlavesUnexpectedCur++;
                                NotifyBusMismatch(m_pBusSlaveCur, EC_NULL);
                            }
                            else
                            {
                                /* validate bus slaves location */
                                NotifyUnexpectedLocation(m_pBusSlaveCur);
                            }
                        }
                        break;
                    case ecbtsm_dlstatus_ist:
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
                        NotifyJunctionRedChange(m_pBusSlaveCur);
#endif
                        NotifyPdiWatchdog(m_pBusSlaveCur);
                        break;
                    case ecbtsm_alstatusrefresh:
#if (defined INCLUDE_JUNCTION_REDUNDANCY)
                        NotifyJunctionRedChange(m_pBusSlaveCur);
#endif
                        break;
                    default:
                        OsDbgAssert(EC_FALSE);
                        break;
                    }
                }
                /* determine next state */
                if ((EC_NULL == m_pBusSlaveCur) && (m_dwCfgSlaveIdxCur >= m_pMaster->GetCfgSlaveCount()))
                {
                    m_eCurState = ecbtsm_notify_application_done;
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecbtsm_notify_application_done:
            {
                m_dwBusSlavesUnexpected = m_dwBusSlavesUnexpectedCur;

#if (defined INCLUDE_LINE_CROSSED_DETECTION) && (defined INCLUDE_RED_DEVICE)
                /* if line crossed was detected in RedBreakFrameMerge() but still not notified */
                /* because linklayer doesn't support EC_LINKIOCTL_SET_FORCEDISCONNECTION,      */
                /* generate generic notification without detailed information                  */
                if (m_pMaster->IsRedConfigured() && !m_bLineCrossedNotified && m_pMaster->m_bRedLineCrossed)
                {
                EC_T_NOTIFICATION_INTDESC* pNotification = m_pMaster->AllocNotification();

                    if (EC_NULL != pNotification)
                    {
                        pNotification->uNotification.Notification.desc.CrossedLineDesc.SlaveProp.wStationAddress = INVALID_FIXED_ADDR;
                        pNotification->uNotification.Notification.desc.CrossedLineDesc.SlaveProp.wAutoIncAddr    = INVALID_AUTO_INC_ADDR;
                        OsStrcpy(pNotification->uNotification.Notification.desc.CrossedLineDesc.SlaveProp.achName, "Unknown");
                        pNotification->uNotification.Notification.desc.CrossedLineDesc.wInputPort = ESC_PORT_INVALID;
                        m_pMaster->NotifyAndFree(EC_NOTIFY_LINE_CROSSED, pNotification, sizeof(EC_T_LINE_CROSSED_DESC));
                    }
                    m_bLineCrossedNotified = EC_TRUE;
                }
#endif /* INCLUDE_LINE_CROSSED_DETECTION && INCLUDE_RED_DEVICE */
                if (m_bPurgeBusSlavesAndNotify)
                {
                    m_bPurgeBusSlavesAndNotify = EC_FALSE;

                    if (m_bLinkConnectedAtStart)
                    {
                        m_eCurState = ecbtsm_restart;
                        bContinueSM = EC_TRUE;
                        break;
                    }
                }
                if (m_bBusMismatchNotified && (EC_E_BUSY == m_dwBTResult))
                {
                    m_dwBTResult = EC_E_BUSCONFIG_MISMATCH;
                }
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
                if (m_bLineCrossedNotified && m_bErrorOnLineCrossed && (EC_E_BUSY == m_dwBTResult))
                {
                    m_dwBTResult = EC_E_LINE_CROSSED;
                }
#endif
                switch(m_eReqState)
                {
                case ecbtsm_busscan_automatic:
                case ecbtsm_busscan_manual:
                case ecbtsm_busscan_accepttopo:
                case ecbtsm_dlstatus_ist:
                case ecbtsm_alstatusrefresh:
                    m_eCurState = m_eReqState;
                    bContinueSM = EC_TRUE;
                    break;
                default:
                    OsDbgAssert(EC_FALSE);
                    m_eCurState = m_eReqState = ecbtsm_stuck;
                    break;
                }
            } break;

        /* bus scan */
        case ecbtsm_busscan_automatic:
        case ecbtsm_busscan_manual:
        case ecbtsm_busscan_accepttopo:
            {
                if (EC_E_BUSY == m_dwBTResult)
                {
                    m_dwBTResult = EC_E_NOERROR;
                }
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest = EC_NULL;
                m_eCurState = m_eReqState = ecbtsm_idle;
            } break;

        /* auxiliary states */
        case ecbtsm_dlstatus_ist:
        case ecbtsm_alstatusrefresh:
            {
                if (EC_E_BUSY == m_dwBTResult)
                {
                    m_dwBTResult = EC_E_NOERROR;
                }
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest = EC_NULL;
                m_eCurState = m_eReqState = ecbtsm_idle;
            } break;

        case ecbtsm_error:
            {
                OsDbgAssert(m_dwBTResult != EC_E_NOERROR);
                m_pCurRequest->oTimeout.Stop();
                m_pCurRequest = EC_NULL;
                m_eCurState = m_eReqState = ecbtsm_idle;
            } break;
        /* a timeout did occur, all requests are stopped from sub state machines */
        case ecbtsm_reqto_wait:
            {
                /* wait for sub state machines */
                if ( (m_eCurSubState == m_eReqSubState)
                  && (m_eCurEEPState == m_eReqEEPState)
                  && (m_eCurChkState == m_eReqChkState) )
                {
                    /* make sub state machines stable and idle */
                    m_eCurSubState = m_eReqSubState = ecbtsms_unknown;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_unknown;

                    /* determine next state */
                    m_eCurState = ecbtsm_reqto;
                    bContinueSM = EC_TRUE;
                }
            } break;
        /* sub state machines did reach timeout state, release request */
        case ecbtsm_reqto:
            {
                m_dwBTResult = EC_E_TIMEOUT;

                m_pCurRequest = EC_NULL;
                m_eCurState = m_eReqState = ecbtsm_idle;
            } break;
        /* this state should never be reached, it means the state machine(s) did reach an undefined state. This state is
         * implemented to recover from fatal errors. */
        case ecbtsm_stuck:
            {
                OsDbgAssert(EC_FALSE);

                /* wait for outstanding commands returned (ProcResult) which is always the case (don't care if frameloss e.g.)
                 * and return to idle */
                if (m_eCurSubState == m_eReqSubState) /* if no DGram calls are pending */
                {
                    m_eCurState = ecbtsm_idle;
                    bContinueSM = EC_TRUE;
                }
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_eCurState = ecbtsm_stuck;
                bContinueSM = EC_FALSE;
            } /* default */
        } /* switch m_eCurState */

        /* current request did time out */
        if( (EC_NULL != m_pCurRequest) && m_pCurRequest->oTimeout.IsElapsed() )
        {
            m_pCurRequest->oTimeout.Stop();
            m_eReqState = ecbtsm_reqto;
            m_eReqSubState = ecbtsms_timeout;
            m_eReqEEPState = ecbtsme_timeout;
/*          m_eReqChkState = ecbtsmchk_timeout; */
        }

#if (defined INCLUDE_LOG_MESSAGES)
        if (m_eCurState != eOldState)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                "BTStateMachine %s->%s (%s, 0x%08X)\n",
                ESTATESText(eOldState), ESTATESText(m_eCurState), ESTATESText(m_eReqState), m_dwBTResult));
        }
#endif
    } /* while( bContinueSM ) */


    /* send acyclic AL status BRD if no cyclic one available to detect topology change while bus scan */
    /* skip it in ecbtsms_latchrecvtimes to not disturb port receive times latching */
    if ((EC_AL_STATUS_INIT_VALUE == m_pMaster->GetBrdAlStatusWkc()) && (dwEcatCmdPending != m_dwEcatCmdPending))
    {
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)
        if (ecbtsms_latchrecvtimes != m_eCurSubState)
#endif
        {
            /* queue EtherCAT command */
            dwRes = m_pMaster->QueueRegisterCmdReq(
                EC_NULL, INVOKE_ID_SB_ACYCALSTATUSBRD, EC_CMD_TYPE_BRD, (EC_T_WORD)(0),
                ECREG_AL_STATUS, sizeof(EC_T_WORD), EC_NULL
                );
#if (defined DEBUG)
            OsDbgAssert(EC_E_NOERROR == dwRes);
#else
            EC_UNREFPARM(dwRes);
#endif
        }
    }
#if (defined INCLUDE_RED_DEVICE)
    /* frame loss are expected in the following states */
    if ((ecbtsms_redcloseport_wait == m_eCurSubState) || (ecbtsms_redopenport_wait == m_eCurSubState))
    {
        /* this command is not monitored */
        m_pMaster->EcatFlushCurrPendingSlaveFrameNoMonitoring();
    }
#endif /* INCLUDE_RED_DEVICE */

    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_BTSM);
}

/***************************************************************************************************/
/**
\brief  Action / Command State machine.
*/
EC_T_VOID CEcBusTopology::BTSubStateMachine(EC_T_VOID)
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
EC_T_DWORD   dwRes            = EC_E_ERROR;
EC_T_BOOL    bContinueSM      = EC_FALSE;
EC_T_INT     nSlaveSliceCount = 0;
CEcBusSlave* pBusSlaveCur     = EC_NULL;
CEcSlave*    pCfgSlaveCur     = EC_NULL;
EC_T_WORD    wAutoIncAddress  = 0;
EC_T_WORD    wCfgAddress      = 0;
EC_T_WORD    wFixedAddress    = 0;
EC_T_WORD    wTmpFixedAddress = 0;
ECBTSM_T_ESUBSTATES eOldState = ecbtsms_unknown;

    PerfMeasStart(&G_TscMeasDesc, EC_MSMT_BTSSM);

    /* if request is pending, do something */
    if (m_eCurSubState != m_eReqSubState)
    {
        bContinueSM = EC_TRUE;
    }

    while (bContinueSM)
    {
        bContinueSM = EC_FALSE;

        eOldState = m_eCurSubState;

        switch (m_eCurSubState)
        {
        /* initial state machine state, before any state machine is started */
        case ecbtsms_unknown:
            {
                m_eCurSubState = ecbtsms_start;
                bContinueSM = EC_TRUE;
            } break;
        /* initial state */
        case ecbtsms_start:
            {
                m_dwSubResult = EC_E_BUSY;

                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                case ecbtsms_collectdlstatus:
                case ecbtsms_collectalstatus:
                    {
                        /* all bus slave should be marked as invalid */
                        OsDbgAssert((EC_NULL == m_pBusSlaveRoot) || (EC_E_INVALIDDATA == m_pBusSlaveRoot->GetScanBusStatus()));

                        m_eCurSubState = ecbtsms_countslaves;
                        bContinueSM = EC_TRUE;
                    } break;
#if (defined INCLUDE_HOTCONNECT)
                case ecbtsms_borderclose:
                    {
                        /* check if master is configured */
                        if (m_pMaster->IsConfigured())
                        {
                            m_eCurSubState = ecbtsms_parseports;
                        }
                        else
                        {
                            m_eCurSubState = ecbtsms_borderclose;
                        }
                        bContinueSM = EC_TRUE;
                    } break;
#endif /* INCLUDE_HOTCONNECT */
                case ecbtsms_aliasactive:
                    {
                        /* check if master is configured */
                        if (m_pMaster->IsConfigured())
                        {
                            m_eCurSubState = ecbtsms_activatealias;
                        }
                        else
                        {
                            m_eCurSubState = ecbtsms_aliasactive;
                        }
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_wrtfaddr:
                    {
                        /* check if master is configured */
                        if (m_pMaster->IsConfigured())
                        {
                            m_eCurSubState = ecbtsms_writefixaddr;
                        }
                        else
                        {
                            m_eCurSubState = ecbtsms_wrtfaddr;
                        }
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_enainterrupt:
                case ecbtsms_disinterrupt:
                    {
                        m_eCurSubState = ecbtsms_readinterrupt;
                        bContinueSM    = EC_TRUE;
                    } break;
                case ecbtsms_timeout:
                    {
                        if (0 == m_dwEcatCmdPending)
                        {
                            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                            m_dwSubResult  = EC_E_TIMEOUT;
                            m_eCurSubState = m_eReqSubState = ecbtsms_error;
                            break;
                        }
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;
        /* count slaves on bus */
        case ecbtsms_countslaves:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_dwEcatCmdRetry     = 3;

                /* determine next state */
                m_eCurSubState = ecbtsms_countslaves_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_countslaves_send:
            {
                /* queue EtherCAT command */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, INVOKE_ID_SB_COUNT, EC_CMD_TYPE_BRD, (EC_T_WORD)(0),
                    ECREG_AL_STATUS, sizeof(EC_T_WORD), EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurSubState = ecbtsms_countslaves_wait;
            } break;
        case ecbtsms_countslaves_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurSubState = ecbtsms_countslaves_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_countslaves_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if ((m_dwEcatCmdProcessed != m_dwEcatCmdSent) && (0 != m_dwEcatCmdRetry))
                {
                    /* prepare state */
                    m_dwErrorCur         = EC_E_NOERROR;
                    m_dwEcatCmdPending   = 0;
                    m_dwEcatCmdSent      = 0;
                    m_dwEcatCmdProcessed = 0;
                    m_dwEcatCmdRetry--;

                    m_eCurSubState = ecbtsms_countslaves_send;
                    bContinueSM = EC_TRUE;
                    break;
                }
#if (defined INCLUDE_RED_DEVICE)
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "/* m_dwSlavesAtStart = %d, m_bLineBreakAtStart = %d, m_wSlavesCntOnMainAtStart = %d, m_wSlavesCntOnRedAtStart = %d */\n", m_dwSlavesAtStart, m_bLineBreakAtStart, m_wSlavesCntOnMainAtStart, m_wSlavesCntOnRedAtStart));
#else
                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "/* m_dwSlavesAtStart = %d */\n", m_dwSlavesAtStart));
#endif
                /* calculate here the maximum pending commands count:          */
                /* maximum 10% of the the slaves can be handle at one time,    */
                /* the value is rounded up to maximum slave commands per frame */
                {
                EC_T_DWORD dwMaxSlaveCmdPerFrame = m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame;

                    m_dwEcatCmdPendingMax = (m_dwSlavesAtStart*10)/100;
                    m_dwEcatCmdPendingMax = (m_dwEcatCmdPendingMax/dwMaxSlaveCmdPerFrame+1)*dwMaxSlaveCmdPerFrame;
                }
                /* determine next state */
                if (0 == m_dwSlavesAtStart)
                {
                    m_dwSubResult  = EC_E_LINK_DISCONNECTED;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    bContinueSM = EC_TRUE;
                }
                else
                {
                    /* slaves are configured and found, continue bus scan */
                    switch (m_eReqSubState)
                    {
                    case ecbtsms_collectbusscan:
                        {
                            m_eCurSubState = ecbtsms_getbusslaves;
                            bContinueSM = EC_TRUE;
                        } break;
                    case ecbtsms_collectdlstatus:
                        {
                            if (m_dwSlavesAtStart != m_dwBusSlaves)
                            {
                                m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                                m_eCurSubState = ecbtsms_getbusslaves;
                            }
                            else
                            {
                                m_eCurSubState = ecbtsms_findintsource;
                            }
                            bContinueSM = EC_TRUE;
                        } break;
                    case ecbtsms_collectalstatus:
                        {
                            if (m_dwSlavesAtStart != m_dwBusSlaves)
                            {
                                m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                                m_eCurSubState = ecbtsms_getbusslaves;
                            }
                            else
                            {
                                m_eCurSubState = ecbtsms_readalstatus;
                            }
                            bContinueSM = EC_TRUE;
                        } break;
                    default:
                        {
                            OsDbgAssert(EC_FALSE);
                            m_dwSubResult  = EC_E_NOTSUPPORTED;
                            m_eCurSubState = m_eReqSubState = ecbtsms_error;
                        } break;
                    }
                }
            } break;
        /* get information about slaves on bus */
        case ecbtsms_getbusslaves:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                RecordBusSlavePrepare();

                /* determine next state */
                m_eCurSubState = ecbtsms_getbusslaves_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_getbusslaves_send:
            {
                /* walk through bus slave list */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_dwEcatCmdSent < m_dwSlavesAtStart) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--)
                {
                    /* get slave address */
                    wAutoIncAddress = (EC_T_WORD)(0-m_dwEcatCmdSent);

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_GETBUSSLAVES, EC_CMD_TYPE_APRD, wAutoIncAddress,
                        ECREG_SC_TYPE, 20, EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_getbusslaves_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_getbusslaves_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (m_dwEcatCmdSent < m_dwSlavesAtStart)
                    {
                        m_eCurSubState = ecbtsms_getbusslaves_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_getbusslaves_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_getbusslaves_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* determine next state */
                if (EC_E_NOERROR != m_dwErrorCur)
                {
                    m_dwSubResult  = m_dwErrorCur;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Not all commands processed in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                case ecbtsms_collectdlstatus:
                case ecbtsms_collectalstatus:
                    {
                        m_eCurSubState = ecbtsms_writetmpaddr;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
#if (defined ECBT_PRINTBUSSLAVESLIST)
                PrintBusSlaveList(m_pBusSlaveRoot);
#endif
            } break;
        /* write temporary slave fixed address to ensure consistency while performing BT data collection */
        case ecbtsms_writetmpaddr:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_writetmpaddr_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_writetmpaddr_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* remove invalid bus slaves */
                    if (EC_E_INVALIDDATA == m_pBusSlaveCur->GetScanBusStatus())
                    {
                        for (;
                             (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (EC_E_INVALIDDATA == m_pBusSlaveCur->GetScanBusStatus());
                             nSlaveSliceCount--)
                        {
                        CEcBusSlave* pBusSlaveNext = m_pBusSlaveCur->GetNext();
                        CEcSlave*    pCfgSlave     = m_pBusSlaveCur->GetCfgSlave();

                            RemoveBusSlave(m_pBusSlaveCur);
                            m_pBusSlaveCur = pBusSlaveNext;

                            /* notify as soon as possible disconnected slaves */
                            if (EC_NULL != pCfgSlave)
                            {
                                pCfgSlave->NotifyPresence();
                            }
                        }
                        if ((nSlaveSliceCount <= 0) || (m_pBusSlaveCur == EC_NULL))
                        {
                            break;
                        }
                    }
                    /* get slave address */
                    wAutoIncAddress   = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress     = m_pBusSlaveCur->GetFixedAddress();
                    wTmpFixedAddress  = m_pBusSlaveCur->GetTmpFixedAddress();
                    OsDbgAssert(wTmpFixedAddress != 0);

                    /* check if Bus set Fixed Address differs from Temporary Address */
                    if (wFixedAddress != wTmpFixedAddress)
                    {
                    EC_T_WORD wTmpFixedAddressFrm = 0;

                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + FIXED_ADDR),
                            "ecbtsms_writetmpaddr: Slave 0x%04X : Fixed = 0x%04X, Alias = 0x%04X, Temp:= 0x%04X\n",
                            wAutoIncAddress, wFixedAddress, m_pBusSlaveCur->GetAliasAddress(), wTmpFixedAddress));

                        /* configure slave node with temporary Fixed Address */
                        /* !!WARNING!! station address should always be set  */
                        /* using autoinc to prevent multiple set             */

                        EC_SET_FRM_WORD((EC_T_BYTE*)&wTmpFixedAddressFrm, wTmpFixedAddress);

                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_WRTTMPSLVADDR, EC_CMD_TYPE_APWR, wAutoIncAddress,
                            ECREG_STATION_ADDRESS, sizeof(EC_T_WORD), &wTmpFixedAddressFrm
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                    else
                    {
                        OsDbgAssert(0 != wFixedAddress);
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_writetmpaddr_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_writetmpaddr_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_writetmpaddr_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_writetmpaddr_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_writetmpaddr_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_writetmpaddr;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                case ecbtsms_collectdlstatus:
                case ecbtsms_collectalstatus:
                    {
                        m_eCurSubState = ecbtsms_chkwrttmpaddr;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;
        /* check if write of temporary slave fixed address was successful */
        case ecbtsms_chkwrttmpaddr:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_chkwrttmpaddr_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_chkwrttmpaddr_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wAutoIncAddress   = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress     = m_pBusSlaveCur->GetFixedAddress();
                    wTmpFixedAddress  = m_pBusSlaveCur->GetTmpFixedAddress();

                    /* queue EC_CMD_TYPE_APRD EtherCAT command to ensure correct autoinc <-> fixed address */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_CHECKTMPADDRESS, EC_CMD_TYPE_APRD, wAutoIncAddress,
                        ECREG_STATION_ADDRESS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;

                    /* queue EC_CMD_TYPE_FPRD EtherCAT command to detect fixed address duplicates */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_CHECKDUPLICATE, EC_CMD_TYPE_FPRD, wTmpFixedAddress,
                        ECREG_STATION_ADDRESS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_chkwrttmpaddr_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_chkwrttmpaddr_wait: 
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_chkwrttmpaddr_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_chkwrttmpaddr_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_chkwrttmpaddr_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* determine next state */
                if (EC_E_NOERROR != m_dwErrorCur)
                {
                    /* something gone wrong, e.g. validation failed, retry */
                    m_eCurSubState = ecbtsms_countslaves;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_chkwrttmpaddr;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                case ecbtsms_collectdlstatus:
                case ecbtsms_collectalstatus:
                    {
                        m_eCurSubState = ecbtsms_readalstatus;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;
        /* read AL Status */
        case ecbtsms_readalstatus:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_readalstatus_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_readalstatus_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle(); 
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax);
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave fixed address */
                    wFixedAddress = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_READALSTATUS, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECREG_AL_STATUS, 6, EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_readalstatus_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readalstatus_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_readalstatus_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_readalstatus_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readalstatus_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_readalstatus;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_ackslaveerror;
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_collectdlstatus:
                    {
                        m_eCurSubState = ecbtsms_findintsource;
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_collectalstatus:
                    {
                        m_eCurSubState = ecbtsms_ackslaveerror;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* acknowledge slave error */
        case ecbtsms_ackslaveerror:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_ackslaveerror_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_ackslaveerror_send:
            {
            EC_T_WORD wAlStatus     = 0;
            EC_T_WORD wAlControlFrm = 0;

                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    wAlStatus = m_pBusSlaveCur->GetAlStatus();

                    if (m_pBusSlaveCur->IsNew() && (DEVICE_STATE_BOOTSTRAP == (wAlStatus & DEVICE_STATE_MASK)))
                    {
                        /* slave identification isn´t always possible in bootstrap */
                        wAlStatus &= ~DEVICE_STATE_MASK;
                        wAlStatus |= DEVICE_STATE_INIT;
                    }
                    else if (!ShallAckAlStatusError(m_pBusSlaveCur))
                    {
                        continue;
                    }
                    /* get slave fixed address */
                    wFixedAddress = m_pBusSlaveCur->GetFixedAddress();

                    EC_SET_FRM_WORD((EC_T_BYTE*)&wAlControlFrm, wAlStatus);

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_ACKSLVERROR, EC_CMD_TYPE_FPWR, wFixedAddress,
                        ECREG_AL_CONTROL, sizeof(EC_T_WORD), &wAlControlFrm
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_ackslaveerror_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_ackslaveerror_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_ackslaveerror_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_ackslaveerror_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_ackslaveerror_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_ackslaveerror;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_getportstatus;
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_collectalstatus:
                    {
                        m_eCurSubState = m_eReqSubState;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* get port state information per slave */
        case ecbtsms_getportstatus:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_getportstatus_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_getportstatus_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_PORTSTATUS, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_getportstatus_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_getportstatus_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_getportstatus_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_getportstatus_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_getportstatus_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_getportstatus;
                    break;
                }
                /* reset eventually detected DL status interrupt */
                /* because the read access to ECREG_DL_STATUS    */
                /* acknowledged the interrupt in the slaves      */
                m_pMaster->DLStatusInterruptDone();

                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_readportctrl;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* read DL Control register to check how port's states are, to have information about topology */
        case ecbtsms_readportctrl:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_readportctrl_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_readportctrl_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();
                    OsDbgAssert(wFixedAddress != 0);

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_RDPORTCONTROL, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECREG_DL_CONTROL, sizeof(EC_T_DWORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_readportctrl_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readportctrl_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_readportctrl_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_readportctrl_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readportctrl_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_readportctrl;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_writeportctrl;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* write DL Control register to correct Port configuration to a valid mode */
        case ecbtsms_writeportctrl:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;
                m_oOpenClosedPortsTimeout.Stop();
                m_bOpenClosedPorts   = EC_FALSE;

                /* determine next state */
                m_eCurSubState = (m_bPortOperationsEnabled ? ecbtsms_writeportctrl_send : ecbtsms_writeportctrl_done);
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_writeportctrl_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                EC_T_BOOL  bSkipCurrentSlave = EC_FALSE;
                EC_T_WORD  wPortIdx          = 0;
                EC_T_DWORD dwDlControl       = 0;
                EC_T_BOOL  bUpdateDLControl  = EC_FALSE;
                EC_T_BOOL  bFlushRequired    = EC_FALSE;

                    if (m_eReqState != ecbtsm_busscan_accepttopo)
                    {
                        if (m_pBusSlaveCur->IsNew())
                        {
                            /* apply the topology change delay if new slave with link established on a closed port is found */
                            for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
                            {
                                if (m_pBusSlaveCur->IsLoopPortX(wPortIdx) && m_pBusSlaveCur->IsSignalPortX(wPortIdx) && (0 != m_dwTopologyChangeDelay))
                                {
                                    if (!m_bOpenClosedPorts)
                                    {
                                        m_oOpenClosedPortsTimeout.Start(m_dwTopologyChangeDelay, m_pMaster->GetMsecCounterPtr());
                                        bSkipCurrentSlave = EC_TRUE;
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* skip previously found slaves if topology in manual mode */
                            if (!m_bTopologyChangeAutoMode)
                            {
                                bSkipCurrentSlave = EC_TRUE;
                            }
                        }
                        if (bSkipCurrentSlave)
                        {
                            continue;
                        }
                    }
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* get current DL control */
                    dwDlControl = m_pBusSlaveCur->GetDlControl();

                    /* check each port configuration */
                    for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
                    {
                    EC_T_BYTE  byPortConfig   = 0;
                    EC_T_BOOL  bShallBeClosed = EC_FALSE;

                        /* handle only implemented ports */
                        if (!m_pBusSlaveCur->IsSlaveCfgPortX(wPortIdx))
                        {
                            continue;
                        }
                        /* get port configuration */
                        byPortConfig   = m_pBusSlaveCur->PortLoopControlX(wPortIdx);
#if (defined INCLUDE_PORT_OPERATION)
                        bShallBeClosed = m_pMaster->m_oHotConnect.IsPortClosed(m_pBusSlaveCur->GetFixedAddress(), wPortIdx) || m_pBusSlaveCur->IsFramelossAfterSlave(wPortIdx);
#endif
                        /* determine new DL control value */
                        if (bShallBeClosed)
                        {
                            if (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED != byPortConfig)
                            {
                                /* close this port */
                                dwDlControl &= ~ECM_DLCTRL_LOOP_PORTX_MASK(wPortIdx);
                                dwDlControl |= (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wPortIdx));
                                bUpdateDLControl = EC_TRUE;
                                bFlushRequired = EC_TRUE;
                            }
                        }
                        else
                        {
                            if (m_pBusSlaveCur->GetAutoCloseSupported() && m_pBusSlaveCur->GetEcatEventSupported())
                            {
                                if ((ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE != byPortConfig)
                                ||  (m_pBusSlaveCur->IsLoopPortX(wPortIdx) && m_pBusSlaveCur->IsSignalPortX(wPortIdx))
                                  )
                                {
                                    dwDlControl &= ~ECM_DLCTRL_LOOP_PORTX_MASK(wPortIdx);
                                    dwDlControl |= (ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wPortIdx));
                                    bUpdateDLControl = EC_TRUE;
                                }
                            }
                            else
                            {
                                /* if auto-close or ecat event are not supported, prevent from closing ports */
                                if ((ECM_DLCTRL_LOOP_PORT_VAL_AUTO != byPortConfig)
                                ||  (m_pBusSlaveCur->IsLoopPortX(wPortIdx) && m_pBusSlaveCur->IsSignalPortX(wPortIdx))
                                  )
                                {
                                    dwDlControl &= ~ECM_DLCTRL_LOOP_PORTX_MASK(wPortIdx);
                                    dwDlControl |= (ECM_DLCTRL_LOOP_PORT_VAL_AUTO<<ECM_DLCTRL_LOOP_PORTX_SHIFT(wPortIdx));
                                    bUpdateDLControl = EC_TRUE;
                                }
                            }
                            if (m_pBusSlaveCur->IsLoopPortX(wPortIdx) && m_pBusSlaveCur->IsSignalPortX(wPortIdx))
                            {
                                bFlushRequired = EC_TRUE;
                            }
                        }
                    }
                    /* update DL control if needed */
                    if (bUpdateDLControl)
                    {
                    EC_T_DWORD dwDlControlFrm = 0;

                        EC_SET_FRM_DWORD((EC_T_BYTE*)&dwDlControlFrm, dwDlControl);

                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_WRTPORTCONTROL, EC_CMD_TYPE_FPWR, wFixedAddress,
                            ECREG_DL_CONTROL, sizeof(EC_T_DWORD), &dwDlControlFrm
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;

                        /* only in case of opening ports */
                        if (bFlushRequired)
                        {
                            /* send a NOP command in a single frame to open an possibly closed port 0 behind the opened port */
                            m_pMaster->EcatFlushCurrPendingSlaveFrame();
                            dwRes = m_pMaster->QueueRegisterCmdReq(
                                EC_NULL, INVOKE_ID_SB_OPENCLOSEDPORTA, EC_CMD_TYPE_NOP, 0,
                                0, 0, EC_NULL
                                );
                            OsDbgAssert(EC_E_NOERROR == dwRes);

                            /* this command is not monitored */
                            m_pMaster->EcatFlushCurrPendingSlaveFrameNoMonitoring();
                        }
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_writeportctrl_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_writeportctrl_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_writeportctrl_send;
                        bContinueSM = EC_TRUE;
                    }
                    else
                    {
                        if (m_oOpenClosedPortsTimeout.IsStarted())
                        {
                            if (m_oOpenClosedPortsTimeout.IsElapsed())
                            {
                                /* prepare state */
                                m_dwErrorCur         = EC_E_NOERROR;
                                m_dwEcatCmdPending   = 0;
                                m_dwEcatCmdSent      = 0;
                                m_dwEcatCmdProcessed = 0;
                                m_pBusSlaveCur       = m_pBusSlaveRoot;
                                m_oOpenClosedPortsTimeout.Stop();
                                m_bOpenClosedPorts   = EC_TRUE;

                                m_eCurSubState = ecbtsms_writeportctrl_send;
                                bContinueSM = EC_TRUE;
                            }
                        }
                        else
                        {
                            m_eCurSubState = ecbtsms_writeportctrl_done;
							/* wait next cycle to detect new slaves behind opened ports */
							/* this reduce the amount of bus scan iteration             */
                            /* bContinueSM = EC_TRUE; */
                        }
                    }
                }
            } break;
        case ecbtsms_writeportctrl_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_writeportctrl;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_refreshportstatus; 
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* refresh port status */
        case ecbtsms_refreshportstatus:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_refreshportstatus_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_refreshportstatus_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    if (m_bPortOperationsEnabled)
                    {
                        /* get slave address */
                        wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                        wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_PORTSTATUS, EC_CMD_TYPE_FPRD, wFixedAddress,
                            ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                    else
                    {
                        m_pBusSlaveCur->BackupPortInfos();
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_refreshportstatus_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_refreshportstatus_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_refreshportstatus_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_refreshportstatus_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_refreshportstatus_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
#if (defined INCLUDE_RED_DEVICE)
                if (m_pMaster->IsRedConfigured() && !m_oTopologyChangeDelayTimeout.IsStarted())
                {
                    /* during ecbtsms_writeportctrl send would be unmasked if a frame   */
                    /* reached the other line after opening a port in a controlled way. */
                    /* Unmask send here to reconnect slaves if line break in middle     */

                    EC_T_BOOL bUnmasked = EC_FALSE;

                    if (m_bSendMaskedOnMain && m_poEthDevice->IsMainLinkConnected())
                    {
                        m_bSendMaskedOnMain = EC_FALSE;
                        bUnmasked = EC_TRUE;
                    }
                    if (m_bSendMaskedOnRed && m_poEthDevice->IsRedLinkConnected())
                    {
                        m_bSendMaskedOnRed = EC_FALSE;
                        bUnmasked = EC_TRUE;
                    }
                    if (bUnmasked)
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTStateMachine: Send NOP frame ans restart\n"));

                        /* send a NOP command in a single frame to open an possibly closed port 0 behind the opened port */
                        m_pMaster->EcatFlushCurrPendingSlaveFrame();
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_OPENCLOSEDPORTA, EC_CMD_TYPE_NOP, 0,
                            0, 0, EC_NULL
                            );
                        OsDbgAssert(EC_E_NOERROR == dwRes);

                        /* this command is not monitored */
                        m_pMaster->EcatFlushCurrPendingSlaveFrameNoMonitoring();

                        /* signalize topology change */
                        m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                        break;
                    }
                }
#endif /* INCLUDE_RED_DEVICE */
                /* check for topology changed */
                if (TopologyChangedWhileScan() || (EC_E_BUSCONFIG_TOPOCHANGE == m_dwErrorCur))
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_refreshportstatus;
                    break;
                }
                /* reset eventually detected DL status interrupt */
                /* because the read access to ECREG_DL_STATUS    */
                /* acknowledged the interrupt in the slaves      */
                m_pMaster->DLStatusInterruptDone();

                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_detect_linecrossed;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* start of line crossed detection */
        case ecbtsms_detect_linecrossed:
            {
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
                /* signalize line crossed detection is running to TopologyChangedWhileScan() */
                m_bLineCrossedDetectionRunning = EC_TRUE;

                /* determine next state */
#if (defined INCLUDE_RED_DEVICE)
                if (m_pMaster->IsRedConfigured() && !m_bLineBreakAtStart)
                {
                    if (m_bRedEnhancedLineCrossedDetectionEnabled)
                    {
                        m_eCurSubState = ecbtsms_redcloseport;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_detect_linecrossed_done;
                    }
                }
                else
#endif /* INCLUDE_RED_DEVICE */
                {
                    m_eCurSubState = ecbtsms_latchrecvtimes;
                }
#else
                m_eCurSubState = ecbtsms_detect_linecrossed_done;
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
                bContinueSM = EC_TRUE;
            } break;

#if (defined INCLUDE_LINE_CROSSED_DETECTION)
#if (defined INCLUDE_RED_DEVICE)
        /* close redundancy port */
        case ecbtsms_redcloseport:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* redundancy port determined in ecbtsms_readrecvtimes_wait() */
                m_pBusSlaveRedPort = EC_NULL;
                m_wRedPort         = ESC_PORT_INVALID;

                /* signalize enhanced line crossed detection is running to TopologyChangedWhileScan() */
                m_bRedEnhancedLineCrossedDetectionRunning = EC_TRUE;

                /* mask DL status interrupt and topology changed detection to protect against endless topology change */
                SetTopologyChangedMasked(EC_TRUE);
                m_pMaster->SetDLStatusInterruptMasked(EC_TRUE);

                /* close redundancy port */
                {
                EC_T_LINK_IOCTLPARMS oLinkIoCtlParms = {0};
                EC_T_BOOL bForceDisconnection = EC_TRUE;

                    OsMemset(&oLinkIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));
                    oLinkIoCtlParms.dwInBufSize = sizeof(EC_T_BOOL);
                    oLinkIoCtlParms.pbyInBuf    = (EC_T_PBYTE)&bForceDisconnection;
                    dwRes = m_poEthDevice->m_pRedDevice->LinkIoctl(EC_LINKIOCTL_SET_FORCEDISCONNECTION, &oLinkIoCtlParms);
                    if (EC_E_NOERROR == dwRes)
                    {
                        m_eCurSubState = ecbtsms_redcloseport_wait;
                    }
                    else
                    {
                        /* IOCTL not supported, skip enhanced line crossed detection */
                        m_eCurSubState = ecbtsms_redopenport_done;
                    }
                }
            } break;
        case ecbtsms_redcloseport_wait:
            {
                /* monitor timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    m_eCurSubState = ecbtsms_redcloseport_done;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    m_dwEcatCmdPending = 0;

                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = ecbtsms_redopenport;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* increment to force acyclic AL status BRD to detect line break */
                m_dwEcatCmdPending++;

                /* wait for line break */
                if (m_pMaster->m_bRedLineBreak)
                {
                    m_dwEcatCmdPending = 0;

                    /* determine next state */
                    m_eCurSubState = ecbtsms_redcloseport_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_redcloseport_done:
            {
                m_eCurSubState = ecbtsms_latchrecvtimes;
                bContinueSM = EC_TRUE;
            } break;
#endif /* INCLUDE_RED_DEVICE */

        /* trigger port receive times latching */
        case ecbtsms_latchrecvtimes:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* trigger receive times latching using acyclic command if cyclic  */
                /* communication disabled, otherwise set the request flag and let  */
                /* trigger the receive times latching using patched cyclic command */
#if (defined INCLUDE_HOTCONNECT)
                /* If hot connect is enable the cyclic communication is already    */
                /* enabled in INIT state                                           */
                if (!m_pMaster->IsConfigured() || (DEVICE_STATE_UNKNOWN == m_pMaster->GetMasterDeviceState()))
#else
                if (!m_pMaster->IsConfigured() || (DEVICE_STATE_UNKNOWN == m_pMaster->GetMasterDeviceState()) || (DEVICE_STATE_INIT == m_pMaster->GetMasterDeviceState()))
#endif
                {
                EC_T_UINT64 qwSysTime = 0;

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_LATCHRECVTIMES, EC_CMD_TYPE_BWR, 0,
                        ECM_DCS_REC_TIMEPORT0, sizeof( EC_T_UINT64 ), ( EC_T_VOID* )&qwSysTime
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                else
                {
                    /* request receive times latching */
                    m_pMaster->m_bLatchReceiveTimesRequest = EC_TRUE;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_latchrecvtimes_wait;
            } break;
        case ecbtsms_latchrecvtimes_wait:
            {
                /* monitor timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
                    if (m_bRedEnhancedLineCrossedDetectionRunning)
                    {
                        m_eCurSubState = ecbtsms_redopenport;
                    }
                    else
#endif
                    {
                        m_eCurSubState = ecbtsms_detect_linecrossed_done;
                    }
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* wait for receive times latching done */
                if (!m_pMaster->m_bLatchReceiveTimesRequest && (0 == m_dwEcatCmdPending))
                {
                    /* determine next state */
                    m_eCurSubState = ecbtsms_latchrecvtimes_done;
                    bContinueSM = EC_TRUE;
                    break;
                }
            } break;
        case ecbtsms_latchrecvtimes_done:
            {
                m_eCurSubState = ecbtsms_readrecvtimes;
                bContinueSM = EC_TRUE;
            } break;

        /* read port receive times */
        case ecbtsms_readrecvtimes:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;
                m_dwEcatCmdRetry     = 3;

                m_eCurSubState = ecbtsms_readrecvtimes_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_readrecvtimes_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax);
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
                    if (m_bRedEnhancedLineCrossedDetectionRunning)
                    {
                        /* read DL status to acknowledge interrupt and detect redundancy port */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_PORTSTATUS, EC_CMD_TYPE_FPRD, m_pBusSlaveCur->GetFixedAddress(),
                            ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
#endif /* INCLUDE_RED_DEVICE */
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_READRECVTIMES, EC_CMD_TYPE_FPRD, m_pBusSlaveCur->GetFixedAddress(),
                        ECM_DCS_REC_TIMEPORT0, 32, EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_readrecvtimes_wait;
                
                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readrecvtimes_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur )
                    {
                        m_eCurSubState = ecbtsms_readrecvtimes_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_readrecvtimes_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readrecvtimes_done:
            {
                /* monitor timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
                    if (m_bRedEnhancedLineCrossedDetectionRunning)
                    {
                        m_eCurSubState = ecbtsms_redopenport;
                    }
                    else
#endif
                    {
                        m_eCurSubState = ecbtsms_detect_linecrossed_done;
                    }
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan() || (EC_E_BUSCONFIG_TOPOCHANGE == m_dwErrorCur))
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;

#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
                    if (m_bRedEnhancedLineCrossedDetectionRunning)
                    {
                        m_eCurSubState = ecbtsms_redopenport;
                    }
                    else
#endif
                    {
                        m_eCurSubState = ecbtsms_detect_linecrossed_done;
                    }
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* check if all commands processed */
                if ((m_dwEcatCmdProcessed != m_dwEcatCmdSent) && (0 != m_dwEcatCmdRetry))
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + STATE_CHNG), "ecbtsms_readrecvtimes: m_dwEcatCmdProcessed = %d,  m_dwEcatCmdSent = %d, m_dwEcatCmdRetry = %d\n", m_dwEcatCmdProcessed, m_dwEcatCmdSent, m_dwEcatCmdRetry));

                    /* prepare state */
                    m_dwErrorCur         = EC_E_NOERROR;
                    m_dwEcatCmdPending   = 0;
                    m_dwEcatCmdSent      = 0;
                    m_dwEcatCmdProcessed = 0;
                    m_dwEcatCmdRetry--;
                    m_pBusSlaveCur       = m_pBusSlaveRoot;

                    /* determine next state */
                    m_eCurSubState = ecbtsms_readrecvtimes_send;
                    break;
                }
                /* determine next state */
#if (defined INCLUDE_RED_DEVICE) && (defined INCLUDE_LINE_CROSSED_DETECTION)
                if (m_bRedEnhancedLineCrossedDetectionRunning)
                {
                    m_eCurSubState = ecbtsms_redopenport;
                }
                else
#endif
                {
                    m_eCurSubState = ecbtsms_detect_linecrossed_done;
                }
                bContinueSM = EC_TRUE;
            } break;

#if (defined INCLUDE_RED_DEVICE)
        /* (re)open redundancy port */
        case ecbtsms_redopenport:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* (re)open redundancy port */
                m_oRedOpenPortTimeout.Start((m_pCurRequest->oTimeout.GetDuration() / 2), m_pMaster->GetMsecCounterPtr());
                {
                EC_T_LINK_IOCTLPARMS oLinkIoCtlParms = {0};
                EC_T_BOOL bForceDisconnection = EC_FALSE;

                    OsMemset(&oLinkIoCtlParms, 0, sizeof(EC_T_LINK_IOCTLPARMS));
                    oLinkIoCtlParms.dwInBufSize = sizeof(EC_T_BOOL);
                    oLinkIoCtlParms.pbyInBuf    = (EC_T_PBYTE)&bForceDisconnection;
                    dwRes = m_poEthDevice->m_pRedDevice->LinkIoctl(EC_LINKIOCTL_SET_FORCEDISCONNECTION, &oLinkIoCtlParms);
                    if (EC_E_NOERROR == dwRes)
                    {
                        m_eCurSubState = ecbtsms_redopenport_wait;
                    }
                    else
                    {
                        OsDbgAssert(EC_FALSE);
                        m_eCurSubState = ecbtsms_redopenport_done;
                    }
                }
            } break;
        case ecbtsms_redopenport_wait:
            {
                /* wait for topology change delay to protect against endless looping bus scan */
                if (m_oTopologyChangeDelayTimeout.IsStarted())
                {
                    /* stop open redundancy port timeout */
                    m_oRedOpenPortTimeout.Stop();
                    break;
                }
                /* check for open redundancy port timeout */
                if (m_oRedOpenPortTimeout.IsElapsed())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Open redundancy port timeout (%dms) elapsed in %s\n", m_oRedOpenPortTimeout.GetDuration(), ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    /* don't break */
                }
                /* monitor timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    m_eCurSubState = ecbtsms_redopenport_done;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    /* don't break */
                }
                /* check for topology changed */
                if (EC_E_BUSCONFIG_TOPOCHANGE == m_dwSubResult)
                {
                    m_dwEcatCmdPending = 0;

                    m_eCurSubState = ecbtsms_redopenport_done;
                    bContinueSM = EC_TRUE;
                    break;
                }
                /* increment to force acyclic AL status BRD to detect line break */
                m_dwEcatCmdPending++;

                /* wait for line fixed and send unmasked on redundancy line to protect against endless looping bus scan */
                if (!m_pMaster->m_bRedLineBreak)
                {
                    m_dwEcatCmdPending = 0;

                    /* determine next state */
                    m_eCurSubState = ecbtsms_redopenportrefreshportstatus_send;
                    bContinueSM = EC_TRUE;
                }
            } break;

        /* refresh dl status of bus slave at redundancy port */
        case ecbtsms_redopenportrefreshportstatus_send:
            {
                if (EC_NULL != m_pBusSlaveRedPort)
                {
                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_PORTSTATUS, EC_CMD_TYPE_FPRD, m_pBusSlaveRedPort->GetFixedAddress(),
                        ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdSent++;
                    m_dwEcatCmdPending++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_redopenportrefreshportstatus_wait;
            } break;
        case ecbtsms_redopenportrefreshportstatus_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurSubState = ecbtsms_redopenport_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_redopenport_done:
            {
                /* signalize enhanced line crossed detection is done to TopologyChangedWhileScan() */
                m_bRedEnhancedLineCrossedDetectionRunning = EC_FALSE;

                /* unmask DL status interrupt and topology changed */
                SetTopologyChangedMasked(EC_FALSE);
                m_pMaster->SetDLStatusInterruptMasked(EC_FALSE);

                if (EC_NULL != m_pBusSlaveRedPort)
                {
                    /* check for new port connection at bus slave at redundancy port during redport close */
                    if (m_pBusSlaveRedPort->DetectNewPortConnection(m_pMaster))
                    {
                        if (m_bTopologyChangeAutoMode)
                        {
                            if (0 == m_dwTopologyChangeDelay)
                            {
                                SetTopologyChanged();
                            }
                            else
                            {
                                EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "Start %d ms Topology change delay\n", m_dwTopologyChangeDelay));
                                m_oTopologyChangeDelayTimeout.Start(m_dwTopologyChangeDelay, m_pMaster->GetMsecCounterPtr());
                            }
                        }
                        else
                        {
                            m_dwSubResult = EC_E_BUSCONFIG_TOPOCHANGE;
                        }
                    }
                    /* backup current information for next comparaison */
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+TOPO_CHNG), "BackupPortInfos() AutoInc 0x%04X DlStatus 0x%04X\n", m_pBusSlaveRedPort->GetAutoIncAddress(), m_pBusSlaveRedPort->GetDlStatus()));
                    m_pBusSlaveRedPort->BackupPortInfos();
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_detect_linecrossed_done;
                bContinueSM = EC_TRUE;
            } break;
#endif /* INCLUDE_RED_DEVICE */
#endif /* INCLUDE_LINE_CROSSED_DETECTION */

        case ecbtsms_detect_linecrossed_done:
            {
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
                /* signalize line crossed detection is done to TopologyChangedWhileScan() */
                m_bLineCrossedDetectionRunning = EC_FALSE;
#endif
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (EC_E_BUSCONFIG_TOPOCHANGE == m_dwSubResult)
                {
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_readinterrupt;
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_timeout:
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                        m_dwSubResult  = EC_E_TIMEOUT;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;
        /* Read Ecat Event Mask */
        case ecbtsms_readinterrupt:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;
                m_dwEcatCmdRetry     = 3;

                /* determine next state */
                m_eCurSubState = ecbtsms_readinterrupt_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_readinterrupt_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* Ecat Event Mask register should be supported */
                    if (!m_pBusSlaveCur->GetEcatEventSupported())
                    {
                        continue;
                    }
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_READEVENTMASK, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECREG_SLV_ECATEVENTMASK, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_readinterrupt_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readinterrupt_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_readinterrupt_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_readinterrupt_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readinterrupt_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if ((m_dwEcatCmdProcessed != m_dwEcatCmdSent) && (0 != m_dwEcatCmdRetry))
                {
                    /* prepare state */
                    m_dwErrorCur         = EC_E_NOERROR;
                    m_dwEcatCmdPending   = 0;
                    m_dwEcatCmdSent      = 0;
                    m_dwEcatCmdProcessed = 0;
                    m_dwEcatCmdRetry--;
                    m_pBusSlaveCur       = m_pBusSlaveRoot;

                    /* determine next state */
                    m_eCurSubState = ecbtsms_readinterrupt_send;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                case ecbtsms_enainterrupt:
                case ecbtsms_disinterrupt:
                    {
                        m_eCurSubState = ecbtsms_cfginterrupt;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* Configure Ecat Event */
        case ecbtsms_cfginterrupt:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_cfginterrupt_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_cfginterrupt_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                EC_T_WORD wEcatEventMask = 0;

                    /* Ecat Event Mask register should be supported */
                    if (!m_pBusSlaveCur->GetEcatEventSupported())
                    {
                        continue;
                    }
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* disable PDI watchdog by slaves with device emulation to protect */
                    /* against DL status interrupt storm (saw by ET1100 based slaves)  */
                    if (m_pBusSlaveCur->IsDeviceEmulation())
                    {
                    EC_T_WORD wWatchDogTimePdiFrm = 0;

                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_WRITEEVENTMASK, EC_CMD_TYPE_FPWR, wFixedAddress,
                            ECREG_WATCHDOG_TIME_PDI, sizeof(EC_T_WORD), &wWatchDogTimePdiFrm
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                    /* prepare interrupt configuration values */
                    wEcatEventMask = m_pBusSlaveCur->GetEcatEventMask();
                    switch (m_eReqSubState)
                    {
                    case ecbtsms_collectbusscan:
                    case ecbtsms_enainterrupt:
                        wEcatEventMask |= (ECM_ECATEVENT_DLSTATUS);
                        break;
                    case ecbtsms_disinterrupt:
                        wEcatEventMask &= ~(ECM_ECATEVENT_DLSTATUS);
                        break;
                    case ecbtsms_timeout:
                        m_pBusSlaveCur = m_pBusSlaveCur->GetNext();
                        continue;
                    default:
                        OsDbgAssert(EC_FALSE);
                        m_pBusSlaveCur = m_pBusSlaveCur->GetNext();
                        continue;
                    }
                    if  (wEcatEventMask != m_pBusSlaveCur->GetEcatEventMask())
                    {
                    EC_T_WORD wEcatEventMaskFrm = 0;

                        EC_SET_FRM_WORD(&wEcatEventMaskFrm, wEcatEventMask);

                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_WRITEEVENTMASK, EC_CMD_TYPE_FPWR, wFixedAddress,
                            ECREG_SLV_ECATEVENTMASK, sizeof(EC_T_WORD), &wEcatEventMaskFrm
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_cfginterrupt_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_cfginterrupt_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_cfginterrupt_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_cfginterrupt_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_cfginterrupt_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_cfginterrupt;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_readbusconfig;
                    } break;
                case ecbtsms_enainterrupt:
                    {
                        m_eCurSubState = ecbtsms_enainterrupt;
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_disinterrupt:
                    {
                        m_eCurSubState = ecbtsms_disinterrupt;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* read EEPROM Information from slave nodes SII interface */
        case ecbtsms_readbusconfig:
            {
                /* start EEPROM sub state machine */
                m_eReqEEPState = ecbtsme_done;

                /* wait for EEPROM sub state machine */
                m_eCurSubState = ecbtsms_readbusconfig_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_readbusconfig_wait:
            {
                /* wait for EEPROM sub state machine */
                if (m_eCurEEPState == m_eReqEEPState)
                {
                    /* make EEPROM sub state machine idle, stable */
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_unknown;

                    /* determine next state */
                    m_eCurSubState = ecbtsms_readbusconfig_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readbusconfig_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* evaluate EEPROM sub state machine result */
                switch (m_dwEEPResult)
                {
                case EC_E_NOERROR:
                {
                    m_eCurSubState = ecbtsms_readpdicontrol;
                } break;
                case EC_E_NOMEMORY:
                case EC_E_BUSCONFIG_TOPOCHANGE:
                case EC_E_TIMEOUT:
                {
                    m_dwSubResult  = m_dwEEPResult;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    bContinueSM = EC_TRUE;
                } break;
                default:
                {
                    OsDbgAssert(EC_FALSE);
                    m_dwSubResult  = m_dwEEPResult;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                } break;
                }
            } break;

        /* read PDI control to know about power safe mode etc. of slave node */
        case ecbtsms_readpdicontrol:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_readpdicontrol_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_readpdicontrol_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_PDICONTROL, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECREG_PDI_CONTROL, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_readpdicontrol_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_readpdicontrol_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_readpdicontrol_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_readpdicontrol_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecbtsms_readpdicontrol_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_readpdicontrol;
                    break;
                }

                /* determine next state */
                m_eCurSubState = ecbtsms_probedcunit;
                bContinueSM = EC_TRUE;
            } break;

        /* probe DC unit */
        case ecbtsms_probedcunit:
            {
#if (defined INCLUDE_DC_SUPPORT)
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_probedcunit_send;
#else
                m_eCurSubState = ecbtsms_probedcunit_done;
#endif /* INCLUDE_DC_SUPPORT */
                bContinueSM = EC_TRUE;
            } break;

#if (defined INCLUDE_DC_SUPPORT)
        case ecbtsms_probedcunit_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_PROBEDCUNIT, EC_CMD_TYPE_FPRD, wFixedAddress, 
                        ECM_DCS_SYSTEMTIME, sizeof(EC_T_DWORD), EC_NULL
                        );                    
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_probedcunit_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_probedcunit_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_probedcunit_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_probedcunit_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
#endif /* INCLUDE_DC_SUPPORT */

        case ecbtsms_probedcunit_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_probedcunit;
                    break;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_buildbustopology;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsms_buildbustopology:
            {
                /* prepare state */
                BuildBusTopologyPrepare();

                m_eCurSubState = ecbtsms_buildbustopology_wait;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_buildbustopology_wait:
            {
                /* set scan bus status for all slaves from invalid to no error */
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL);
                     nSlaveSliceCount--,       m_pBusSlaveCur  = m_pBusSlaveCur->GetNext())
                {
                    BuildBusTopology(m_pBusSlaveCur);
                }
                /* determine next state */
                if (EC_NULL == m_pBusSlaveCur)
                {
                    m_eCurSubState = ecbtsms_buildbustopology_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_buildbustopology_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* restart scan if topology changed */
                if ((EC_E_DLSTATUS_IRQ_TOPOCHANGED == m_dwSubResult) && !m_oTopologyChangeDelayTimeout.IsStarted())
                {
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectbusscan:
                    {
                        m_eCurSubState = ecbtsms_collectbusscan;
                        bContinueSM = EC_TRUE;
                    } break;
                case ecbtsms_timeout:
                    {
                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                        m_dwSubResult  = EC_E_TIMEOUT;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

#if (defined INCLUDE_HOTCONNECT) && (defined INCLUDE_PORT_OPERATION)
        /* check ports to perform border close if required */
        case ecbtsms_parseports:
            {
            EC_T_DWORD dwChangesPending = 0;

                /* border close should be done if all HC group are present on the network */
                if (0 == CountUnusedFreeHookElements())
                {
                    for (pBusSlaveCur = m_pBusSlaveRoot; EC_NULL != pBusSlaveCur; pBusSlaveCur = pBusSlaveCur->GetNext())
                    {
                        EC_T_DWORD dwPortOperation = ChangePortAnalysis(pBusSlaveCur);

                        if (0 != dwPortOperation)
                        {
                            EC_T_WORD wPortIdx = 0;

                            for (wPortIdx = 0; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
                            {
                                EC_T_WORD wPortFlags = (EC_T_WORD)((dwPortOperation >> (8 * wPortIdx)) & PORT_MODIFY_MASK); 

                                switch (wPortFlags)
                                {
                                case 0: continue; /* nothing to do */
                                case PORT_CLOSE_FLAG: wPortFlags = m_pMaster->m_oHotConnect.CreatePortChangeFlags(EC_TRUE,  EC_TRUE,  PORTCHG_SOURCE_BORDERCLOSE); break;
                                case PORT_OPEN_FLAG:  wPortFlags = m_pMaster->m_oHotConnect.CreatePortChangeFlags(EC_FALSE, EC_FALSE, PORTCHG_SOURCE_BORDERCLOSE); break;
                                default: OsDbgAssert(EC_FALSE); continue;
                                }
                                dwRes = m_pMaster->m_oHotConnect.QueueChangePortRequest(pBusSlaveCur->GetFixedAddress(), wPortIdx, wPortFlags);
                                if (EC_E_NOERROR == dwRes)
                                {
                                    dwChangesPending++;
                                }
                            }
                        }
                    }
                }
                else
                {
                    /* check if there are some ports to open */
                    if (0 < m_pMaster->m_oHotConnect.OpenPortsBySource(PORTCHG_SOURCE_BORDERCLOSE, m_pMaster->m_dwSBTimeout))
                    {
                        dwChangesPending++;
                    }
                }
                if (0 < dwChangesPending)
                {
                    m_pMaster->m_oHotConnect.StartChangePort(m_pMaster->m_dwSBTimeout);
                    m_eCurSubState = ecbtsms_parseports_wait;
                }
                else
                {
                    m_eCurSubState = ecbtsms_parseports_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_parseports_wait:
            {
                dwRes = m_pMaster->m_oHotConnect.PollChangePort();
                if ((EC_E_BUSY != dwRes) && (EC_E_NOTREADY != dwRes))
                {
                    if (EC_E_NOERROR != dwRes)
                    {
                        OsDbgAssert(EC_FALSE);
                    }
                    else
                    {

                        EC_T_DWORD dwUnusedFreeHookElements = CountUnusedFreeHookElements();

                        /* check for all ports opened */
                        if ((0 != dwUnusedFreeHookElements) && (0 != m_pMaster->m_oHotConnect.CountClosedPortsBySource(PORTCHG_SOURCE_BORDERCLOSE)))
                        {
                            m_eCurSubState = ecbtsms_parseports;
                            bContinueSM = EC_TRUE;
                        }
                        else
                        {
                            m_eCurSubState = ecbtsms_parseports_done;
                            bContinueSM = EC_TRUE;
                        }
                    }
                }
            } break;
        case ecbtsms_parseports_done:
            {
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                m_eCurSubState = ecbtsms_borderclose;
                bContinueSM = EC_TRUE;
            } break;
#endif /* INCLUDE_HOTCONNECT && INCLUDE_PORT_OPERATION */

        /* */
        case ecbtsms_findintsource:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_findintsource_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_findintsource_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* if EtherCAT event not supported read AL_STATUS instead */
                    if (m_pBusSlaveCur->GetEcatEventSupported())
                    {
                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_READEVENTREGISTER, EC_CMD_TYPE_FPRD, wFixedAddress,
                            ECREG_SLV_ECATEVENTREQUEST, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                    else
                    {
                        /* queue EtherCAT command */                        
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_READEVENTREGISTER, EC_CMD_TYPE_FPRD, wFixedAddress,
                            ECREG_AL_STATUS, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_findintsource_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_findintsource_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_findintsource_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_findintsource_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_findintsource_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* don't check for topology changed       */
                /* don't check if all commands processed  */
                /* don't break the sub state machine here */
                /* just refresh and acknowledge all the slaves we can reach on the bus */

                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectdlstatus:
                    {
                        m_eCurSubState = ecbtsms_ackinterrupt;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;
        case ecbtsms_ackinterrupt:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_ackinterrupt_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_ackinterrupt_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();

                    /* check if DL status interrupt is pending */
                    if (m_pBusSlaveCur->IsDLStatusEvent() || !m_pBusSlaveCur->GetEcatEventSupported())
                    {
                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_PORTSTATUS, EC_CMD_TYPE_FPRD, wFixedAddress,
                            ECREG_DL_STATUS, sizeof(EC_T_WORD), EC_NULL
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_ackinterrupt_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_ackinterrupt_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_ackinterrupt_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_ackinterrupt_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_ackinterrupt_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* don't check for topology changed       */
                /* don't check if all commands processed  */
                /* don't break the sub state machine here */
                /* just refresh and acknowledge all the slaves we can reach on the bus */

                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_collectdlstatus:
                    {
                        m_eCurSubState = m_eReqSubState;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* send activation command to second station address for addressing */
        case ecbtsms_activatealias:
            {
            EC_T_BYTE    byData         = 0;
            EC_T_DWORD   dwScanCfgSlv   = 0;
            EC_T_DWORD   dwConfigSlaves = m_pMaster->GetCfgSlaveCount();
            CEcSlave*    pCurSlave      = EC_NULL;
            CEcSlave*    pCfgSlave      = EC_NULL;

                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* walk through bus slave list */
                for (pBusSlaveCur = m_pBusSlaveRoot; EC_NULL != pBusSlaveCur; pBusSlaveCur = pBusSlaveCur->GetNext())
                {
                    /* now search for slave in Master Config */
                    pCfgSlave->m_pBusSlave = EC_NULL;
                    pCfgSlave = EC_NULL;

                    pBusSlaveCur->SetCfgSlave(EC_NULL);
                    for (dwScanCfgSlv = 0; dwScanCfgSlv < dwConfigSlaves; dwScanCfgSlv++)
                    {
                        pCurSlave = m_pMaster->GetSlaveByIndex(dwScanCfgSlv);

                        if (EC_NULL == pCurSlave )
                            continue;

                        if (pCurSlave->m_wAutoIncAddr == pBusSlaveCur->GetAutoIncAddress() )
                        {
                            /* found slave in config */
                            pCfgSlave = pCurSlave;

                            pBusSlaveCur->SetCfgSlave(pCfgSlave);
                            pCfgSlave->m_pBusSlave = pBusSlaveCur;
                            pCfgSlave->m_bDeviceEmulation = pBusSlaveCur->IsDeviceEmulation();

                            /* leave for */
                            break;
                        }
                    }
                    if ((EC_NULL != pCfgSlave) && (0 != pBusSlaveCur->GetAliasAddress()) )
                    {
                        pCfgSlave->SetFixedAddr(pBusSlaveCur->GetAliasAddress());
                    }
                }
                /* set activation bit */
                byData |= 0x1;

                /* queue EtherCAT command */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, INVOKE_ID_SB_ACTIVATEALIAS, EC_CMD_TYPE_BWR, 0,
                    ECREG_DL_CONTROL3, sizeof(EC_T_BYTE), (EC_T_PVOID)(&byData)
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurSubState = ecbtsms_activatealias_wait;
            } break;
        case ecbtsms_activatealias_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurSubState = ecbtsms_activatealias_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_activatealias_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_activatealias;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_aliasactive:
                    {
                        m_eCurSubState = ecbtsms_aliasactive;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        /* write slave fixed address to slaves (before performing border close) */
        case ecbtsms_writefixaddr:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_writefixaddr_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_writefixaddr_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get config slave */
                    pCfgSlaveCur = m_pBusSlaveCur->GetCfgSlave();
                    if (EC_NULL == pCfgSlaveCur)
                    {
                        continue;
                    }
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();
                    wCfgAddress     = pCfgSlaveCur->GetFixedAddr();

                    /* if configured fixed address from ENI file differs from temporary fixed address, write it to the corresponding slave node */
                    if (wFixedAddress != wCfgAddress)
                    {
                    EC_T_WORD wCfgAddressFrm = 0;

                        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0 + FIXED_ADDR),
                            "ecbtsms_writefixaddr: Slave 0x%04X : Fixed = 0x%04X, Alias = 0x%04X, Temp = 0x%04X, Cfg:= 0x%04X\n",
                            wAutoIncAddress, wFixedAddress, m_pBusSlaveCur->GetAliasAddress(), m_pBusSlaveCur->GetTmpFixedAddress(), wCfgAddress));

                        EC_SET_FRM_WORD((EC_T_BYTE*)&wCfgAddressFrm, wCfgAddress);

                        /* queue EtherCAT command */
                        dwRes = m_pMaster->QueueRegisterCmdReq(
                            EC_NULL, INVOKE_ID_SB_WRTSLVFIXADDR, EC_CMD_TYPE_FPWR, wFixedAddress,
                            ECREG_STATION_ADDRESS, sizeof(EC_T_WORD), &wCfgAddressFrm
                            );
                        if (EC_E_NOERROR != dwRes)
                        {
                            /* try again next cycle */
                            goto Exit;
                        }
                        m_dwEcatCmdPending++;
                        m_dwEcatCmdSent++;
                    }
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_writefixaddr_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_writefixaddr_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_writefixaddr_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_writefixaddr_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_writefixaddr_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_writefixaddr;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_wrtfaddr:
                    {
                        m_eCurSubState = ecbtsms_checkfixedaddress;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;
        /* check if write of slave fixed address was successful */
        case ecbtsms_checkfixedaddress:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurSubState = ecbtsms_checkfixedaddress_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsms_checkfixedaddress_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get config slave */
                    pCfgSlaveCur = m_pBusSlaveCur->GetCfgSlave();
                    if (EC_NULL == pCfgSlaveCur)
                    {
                        continue;
                    }
                    /* get slave address */
                    wAutoIncAddress = m_pBusSlaveCur->GetAutoIncAddress();
                    wFixedAddress   = m_pBusSlaveCur->GetFixedAddress();
                    wCfgAddress     = pCfgSlaveCur->GetCfgFixedAddr();

                    /* queue EC_CMD_TYPE_APRD EtherCAT command to ensure correct autoinc <-> fixed address */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_CHECKFIXEDADDRESS, EC_CMD_TYPE_APRD, wAutoIncAddress,
                        ECREG_STATION_ADDRESS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;

                    /* queue EC_CMD_TYPE_FPRD EtherCAT command to detect fixed address duplicates */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_CHECKDUPLICATE, EC_CMD_TYPE_FPRD, wCfgAddress,
                        ECREG_STATION_ADDRESS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurSubState = ecbtsms_checkfixedaddress_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_checkfixedaddress_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurSubState = ecbtsms_checkfixedaddress_send;
                    }
                    else
                    {
                        m_eCurSubState = ecbtsms_checkfixedaddress_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsms_checkfixedaddress_done:
            {
                /* handle timeout request */
                if (m_eReqSubState == ecbtsms_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Timeout in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_TIMEOUT;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Topology changed while scan in %s\n", ESUBSTATESText(m_eCurSubState)));
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if validation succeeded */
                if (EC_E_NOERROR != m_dwErrorCur)
                {
                    m_dwSubResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTSubStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ESUBSTATESText(m_eCurSubState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurSubState = ecbtsms_checkfixedaddress;
                    break;
                }
                /* determine next state */
                switch (m_eReqSubState)
                {
                case ecbtsms_wrtfaddr:
                    {
                        m_eCurSubState = ecbtsms_wrtfaddr;
                        bContinueSM = EC_TRUE;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwSubResult  = EC_E_NOTSUPPORTED;
                        m_eCurSubState = m_eReqSubState = ecbtsms_error;
                    } break;
                }
            } break;

        case ecbtsms_collectbusscan:
        case ecbtsms_collectdlstatus:
        case ecbtsms_collectalstatus:
#if (defined INCLUDE_HOTCONNECT)
        case ecbtsms_borderclose:
#endif
        case ecbtsms_aliasactive:
        case ecbtsms_wrtfaddr:
        case ecbtsms_enainterrupt:
        case ecbtsms_disinterrupt:
            {
                if (EC_E_BUSY == m_dwSubResult)
                {
                    m_dwSubResult = EC_E_NOERROR;
                }
                /* done */
            } break;
        case ecbtsms_timeout:
            {
                m_dwSubResult = EC_E_TIMEOUT;

                /* done */
            } break;
        case ecbtsms_error:
            {
             /* m_dwSubResult set from error place */

                /* done */
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_dwSubResult  = EC_E_NOTSUPPORTED;
                m_eCurSubState = m_eReqSubState = ecbtsms_error;
            } break;
        }
#if (defined INCLUDE_LOG_MESSAGES)
        if (m_eCurSubState != eOldState)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                "BTSubStateMachine %s->%s (%s, 0x%08X)\n",
                ESUBSTATESText(eOldState), ESUBSTATESText(m_eCurSubState), ESUBSTATESText(m_eReqSubState), m_dwSubResult));
        }
#endif
    } /*while( bContinueSM )*/

Exit:
    PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_BTSSM);
}

/***************************************************************************************************/
/**
\brief  EEPROM sub state machine.
*/
EC_T_VOID CEcBusTopology::BTEEPStateMachine(EC_T_VOID )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
EC_T_DWORD   dwRes            = EC_E_ERROR;
EC_T_BOOL    bContinueSM      = EC_FALSE;
EC_T_INT     nSlaveSliceCount = 0;
CEcBusSlave* pBusSlaveCur     = EC_NULL;
EC_T_WORD    wFixedAddress    = 0;
ECBTSM_T_EEEPSTATES eOldState = ecbtsme_unknown;

    /* if request is pending, do something */
    if (m_eCurEEPState != m_eReqEEPState)
    {
        bContinueSM = EC_TRUE;
    }

    while( bContinueSM )
    {
        bContinueSM = EC_FALSE;

        eOldState = m_eCurEEPState;

        switch (m_eCurEEPState)
        {
        /* state machine initial state, if state machine did not run */
        case ecbtsme_unknown:
            {
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    m_eCurEEPState = ecbtsme_timeout;
                }
                else
                {
                    m_eCurEEPState = ecbtsme_start;
                }
                bContinueSM = EC_TRUE;
            } break;
        /* start state, where state machine decides based on target state which way to go */
        case ecbtsme_start:
            {
            EC_T_BOOL bReadEEPROM = EC_FALSE;

                /* reset state machine variables */
                m_dwEEPResult      = EC_E_BUSY;
                m_dwCurEEPEntryIdx = READEEPROMENTRY_COUNT;
                for (pBusSlaveCur = m_pBusSlaveRoot; EC_NULL != pBusSlaveCur; pBusSlaveCur = pBusSlaveCur->GetNext())
                {
#if (defined INCLUDE_GEN_OP_ENI)
                CEcBusSlaveGenEni* pBusSlaveGenEniCur = pBusSlaveCur->GetBusSlaveGenEni();

                    if (EC_NULL != pBusSlaveGenEniCur)
                    {
                        pBusSlaveGenEniCur->m_wEEPROMCategoryBaseCur = ESC_SII_REG_FIRSTCATEGORYHDR;
                        pBusSlaveGenEniCur->m_wEEPROMCategoryTypeCur = 0;
                        pBusSlaveGenEniCur->m_wEEPROMCategorySizeCur = 0;
                        pBusSlaveGenEniCur->m_wEEPROMAddrCur         = pBusSlaveGenEniCur->m_wEEPROMCategoryBaseCur;
                    }
#endif /* INCLUDE_GEN_OP_ENI */

                    /* check if there is something to do */
                    if (!m_pMaster->IsConfigured() || pBusSlaveCur->IsNew())
                    {
                         bReadEEPROM = EC_TRUE;;
                    }
                }
                if (bReadEEPROM)
                {
                    /* look for first EEPROM entry to read */
                    m_dwCurEEPEntryIdx = 0;
                    while (m_dwCurEEPEntryIdx < READEEPROMENTRY_COUNT)
                    {
                        if (m_abReadEEPROMEntry[m_dwCurEEPEntryIdx])
                        {
                            break;
                        }
                        m_dwCurEEPEntryIdx++;
                    }
                }
                /* determine next state */
                if (m_dwCurEEPEntryIdx < READEEPROMENTRY_COUNT)
                {
                    m_eCurEEPState = ecbtsme_acq_eep_access;
                    bContinueSM = EC_TRUE;
                }
                else
                {
                    m_eCurEEPState = ecbtsme_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        /* Assign EEPROM of all slaves to EtherCAT Master (not PDI) */
        case ecbtsme_acq_eep_access:
            {
            EC_T_BYTE byData = 0;

                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* queue EtherCAT command */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, INVOKE_ID_SB_EEPACCACQUIRE, EC_CMD_TYPE_BWR, 0,
                    ECM_SB_EEP_SLV_PDIACCSTATE, sizeof(EC_T_BYTE), &byData
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurEEPState = ecbtsme_acq_eep_access_wait;
            } break;
        case ecbtsme_acq_eep_access_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurEEPState = ecbtsme_acq_eep_access_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_acq_eep_access_done:
            {
                /* handle timeout request */
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Timeout in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_eCurEEPState = ecbtsme_timeout;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Topology changed while scan in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_dwEEPResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", EEEPSTATESText(m_eCurEEPState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurEEPState = ecbtsme_acq_eep_access;
                    break;
                }
                /* determine next state */
                m_eCurEEPState = ecbtsme_chkprebusy;
                bContinueSM = EC_TRUE;
            } break;
        /* check and wait if EEPROM is busy before sending SII command */
        case ecbtsme_chkprebusy:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* queue EtherCAT command */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, INVOKE_ID_SB_EEPPREBUSY, EC_CMD_TYPE_BRD, (EC_T_WORD)(0),
                    ECM_SB_EEP_SLV_CTRLSTATUS, sizeof(EC_T_WORD), EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurEEPState = ecbtsme_chkprebusy_wait;
            } break;
        case ecbtsme_chkprebusy_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurEEPState = ecbtsme_chkprebusy_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_chkprebusy_done:
            {
                /* handle timeout request */
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Timeout in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_eCurEEPState = ecbtsme_timeout;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Topology changed while scan in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_dwEEPResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", EEEPSTATESText(m_eCurEEPState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurEEPState = ecbtsme_chkprebusy;
                    break;
                }
                /* determine next state */
                if (m_bEEPChkSumErrorDetected)
                {
                    m_eCurEEPState = ecbtsme_checksumerror;
                }
                else
                {
                    m_eCurEEPState = ecbtsme_writeaddr;
                }
                bContinueSM = EC_TRUE;
            } break;
        /* look for slave(s) with EEPROM checksum error */
        case ecbtsme_checksumerror:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurEEPState = ecbtsme_checksumerror_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsme_checksumerror_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* get slave address */
                    wFixedAddress = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_EEPCHECKSUMERROR, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECM_SB_EEP_SLV_CTRLSTATUS, sizeof(EC_T_WORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurEEPState = ecbtsme_checksumerror_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_checksumerror_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurEEPState = ecbtsme_checksumerror_send;
                    }
                    else
                    {
                        m_eCurEEPState = ecbtsme_checksumerror_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_checksumerror_done:
            {
                /* handle timeout request */
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Timeout in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_eCurEEPState = ecbtsme_timeout;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Topology changed while scan in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_dwEEPResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", EEEPSTATESText(m_eCurEEPState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurEEPState = ecbtsme_checksumerror;
                    break;
                }
                /* determine next state */
                m_eCurEEPState = ecbtsme_writeaddr;
                bContinueSM = EC_TRUE;
            } break;
        /* write EEPROM SII Address to all slaves */
        case ecbtsme_writeaddr:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurEEPState = ecbtsme_writeaddr_send;
                bContinueSM = EC_TRUE;
            } break;

        case ecbtsme_writeaddr_send:
            {
            EC_T_SB_EEP_REGS oRegFrm = {0};

                if (m_dwCurEEPEntryIdx < READEEPROMENTRY_IDX_CATEGORIES)
                {
                    /* use broadcast addressing */
                    EC_SET_FRM_WORD(&oRegFrm.wCtrlStatus, ECM_SB_EEP_CTRLSTATUS_WRITE_READACCESS);
                    EC_SET_FRM_DWORD(&oRegFrm.dwEEPAddress, c_awReadEEPROMEntryOrder[m_dwCurEEPEntryIdx]);

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_EEPADDR, EC_CMD_TYPE_BWR, 0,
                        ECM_SB_EEP_SLV_CTRLSTATUS, 6, (EC_T_PVOID)&oRegFrm
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;

                    /* don't walk through bus slave list */
                    m_pBusSlaveCur = EC_NULL;
                }
#if (defined INCLUDE_GEN_OP_ENI)
                else
                {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                    {
                    CEcBusSlaveGenEni* pBusSlaveGenEniCur = m_pBusSlaveCur->GetBusSlaveGenEni();

                        if (EC_NULL != pBusSlaveGenEniCur)
                        {
                            /* don't read from already found slave */
                            if (m_pMaster->IsConfigured() && !m_pBusSlaveCur->IsNew())
                            {
                                continue;
                            }
                            /* nothing to read */
                            if (0 == pBusSlaveGenEniCur->m_wEEPROMAddrCur)
                            {
                                continue;
                            }
                            /* get slave address */
                            wFixedAddress = m_pBusSlaveCur->GetFixedAddress();

                            EC_SET_FRM_WORD(&oRegFrm.wCtrlStatus, ECM_SB_EEP_CTRLSTATUS_WRITE_READACCESS);
                            EC_SET_FRM_DWORD(&oRegFrm.dwEEPAddress, pBusSlaveGenEniCur->m_wEEPROMAddrCur);

                            /* queue EtherCAT command */
                            dwRes = m_pMaster->QueueRegisterCmdReq(
                                EC_NULL, INVOKE_ID_SB_EEPADDR, EC_CMD_TYPE_FPWR, wFixedAddress,
                                ECM_SB_EEP_SLV_CTRLSTATUS, 6, (EC_T_PVOID)&oRegFrm
                                );
                            if (EC_E_NOERROR != dwRes)
                            {
                                /* try again next cycle */
                                goto Exit;
                            }
                            m_dwEcatCmdPending++;
                            m_dwEcatCmdSent++;
                        }
                    }
                }
#endif /* INCLUDE_GEN_OP_ENI */
                /* determine next state */
                m_eCurEEPState = ecbtsme_writeaddr_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_writeaddr_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurEEPState = ecbtsme_writeaddr_send;
                    }
                    else
                    {
                        m_eCurEEPState = ecbtsme_writeaddr_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_writeaddr_done:
            {
                /* handle timeout request */
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Timeout in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_eCurEEPState = ecbtsme_timeout;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Topology changed while scan in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_dwEEPResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", EEEPSTATESText(m_eCurEEPState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurEEPState = ecbtsme_writeaddr;
                    break;
                }
                /* determine next state */
                m_eCurEEPState = ecbtsme_chkbusy;
                bContinueSM = EC_TRUE;
            } break;
        /* check and wait, whether slaves are done with processing the SII request */
        case ecbtsme_chkbusy:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* queue EtherCAT command */
                dwRes = m_pMaster->QueueRegisterCmdReq(
                    EC_NULL, INVOKE_ID_SB_EEPBUSY, EC_CMD_TYPE_BRD, (EC_T_WORD)(0),
                    ECM_SB_EEP_SLV_CTRLSTATUS, sizeof(EC_T_WORD), EC_NULL
                    );
                if (EC_E_NOERROR != dwRes)
                {
                    /* try again next cycle */
                    goto Exit;
                }
                m_dwEcatCmdPending++;
                m_dwEcatCmdSent++;

                /* determine next state */
                m_eCurEEPState = ecbtsme_chkbusy_wait;
            } break;
        case ecbtsme_chkbusy_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurEEPState = ecbtsme_chkbusy_done;
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_chkbusy_done:
            {
                /* handle timeout request */
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Timeout in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_eCurEEPState = ecbtsme_timeout;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Topology changed while scan in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_dwEEPResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", EEEPSTATESText(m_eCurEEPState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurEEPState = ecbtsme_chkbusy;
                    break;
                }
                /* determine next state */
                m_eCurEEPState = ecbtsme_readvalue;
                bContinueSM = EC_TRUE;
            } break;
        /* read SII sata from each slave individually */
        case ecbtsme_readvalue:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurEEPState = ecbtsme_readvalue_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsme_readvalue_send:
            {
                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* don't read from already found slave */
                    if (m_pMaster->IsConfigured() && !m_pBusSlaveCur->IsNew())
                    {
                        continue;
                    }
#if (defined INCLUDE_GEN_OP_ENI)
                    if (m_dwCurEEPEntryIdx == READEEPROMENTRY_IDX_CATEGORIES)
                    {
                    CEcBusSlaveGenEni* pBusSlaveGenEniCur = m_pBusSlaveCur->GetBusSlaveGenEni();

                        if ((EC_NULL == pBusSlaveGenEniCur) || (0 == pBusSlaveGenEniCur->m_wEEPROMAddrCur))
                        {
                            continue;
                        }
                    }
#endif /* INCLUDE_GEN_OP_ENI */

                    /* get slave address */
                    wFixedAddress = m_pBusSlaveCur->GetFixedAddress();

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_EEPREAD, EC_CMD_TYPE_FPRD, wFixedAddress,
                        ECM_SB_EEP_SLV_EEPDATA, sizeof(EC_T_DWORD), EC_NULL
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurEEPState = ecbtsme_readvalue_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_readvalue_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_E_NOERROR != m_dwErrorCur)
                    {
                        m_dwEEPResult = m_dwErrorCur;
                        m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                        break;
                    }
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurEEPState = ecbtsme_readvalue_send;
                    }
                    else
                    {
                        m_eCurEEPState = ecbtsme_readvalue_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsme_readvalue_done:
            {
                /* handle timeout request */
                if (m_eReqEEPState == ecbtsme_timeout)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Timeout in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_eCurEEPState = ecbtsme_timeout;
                    break;
                }
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Topology changed while scan in %s\n", EEEPSTATESText(m_eCurEEPState)));
                    m_dwEEPResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTEEPStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", EEEPSTATESText(m_eCurEEPState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurEEPState = ecbtsme_readvalue;
                    break;
                }                
                /* look for next EEPROM entry to read */
#if (defined INCLUDE_GEN_OP_ENI)
                if ((m_dwCurEEPEntryIdx == READEEPROMENTRY_IDX_CATEGORIES) && (0 != m_dwEcatCmdSent))
                {
                    m_eCurEEPState = ecbtsme_chkprebusy;
                    bContinueSM = EC_TRUE;
                    break;
                }
#endif /* INCLUDE_GEN_OP_ENI */
                m_dwCurEEPEntryIdx++;
                while (m_dwCurEEPEntryIdx < READEEPROMENTRY_COUNT)
                {
                    if (m_abReadEEPROMEntry[m_dwCurEEPEntryIdx])
                    {
                        break;
                    }
                    m_dwCurEEPEntryIdx++;
                }
                /* determine next state */
                if (m_dwCurEEPEntryIdx < READEEPROMENTRY_COUNT)
                {
                    m_eCurEEPState = ecbtsme_chkprebusy;
                }
                else
                {
                    m_eCurEEPState = ecbtsme_done;
                }
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsme_done:
            {
                if (EC_E_BUSY == m_dwEEPResult)
                {
                    m_dwEEPResult = EC_E_NOERROR;
                }
                /* done */
            } break;
        case ecbtsme_timeout:
            {
                m_dwEEPResult = EC_E_TIMEOUT;

                /* done */
            } break;
        case ecbtsme_error:
            {
             /* m_dwEEPResult set from error place */

                /* done */
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_dwEEPResult  = EC_E_NOTSUPPORTED;
                m_eCurEEPState = m_eReqEEPState = ecbtsme_error;
            } break;
        }
#if (defined INCLUDE_LOG_MESSAGES)
        if (m_eCurEEPState != eOldState)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                "BTEEPStateMachine %s->%s (%s, 0x%08X)\n",
                EEEPSTATESText(eOldState), EEEPSTATESText(m_eCurEEPState), EEEPSTATESText(m_eReqEEPState), m_dwEEPResult));
        }
#endif
    } /* while (bContinueSM) */
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Topology check sub state machine.
*/
EC_T_VOID CEcBusTopology::BTChkStateMachine(EC_T_VOID )
{
#if (defined INCLUDE_LOG_MESSAGES)
EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);
#endif
EC_T_DWORD   dwRes            = EC_E_ERROR;
EC_T_BOOL    bContinueSM      = EC_FALSE;
EC_T_INT     nSlaveSliceCount = 0;
ECBTSMCHK_T_STATES eOldState  = ecbtsmchk_unknown;

    /* if request is pending, do something */
    if (m_eCurChkState != m_eReqChkState)
    {
        bContinueSM = EC_TRUE;
    }

    while (bContinueSM)
    {
        bContinueSM = EC_FALSE;

        eOldState = m_eCurChkState;

        switch (m_eCurChkState)
        {
        case ecbtsmchk_unknown:
            {
                m_eCurChkState = ecbtsmchk_idle;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsmchk_idle:
            {
                m_eCurChkState = ecbtsmchk_start;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsmchk_start:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;

                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTChkStateMachine: Topology changed while scan in %s\n", ECHKSTATESText(m_eCurChkState)));
                    m_dwChkResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
                    break;
                }
                /* reset topology check sub state machine result */
                m_dwChkResult = EC_E_BUSY;

                /* initialize member variables for check config */
                PerfMeasStart(&G_TscMeasDesc, EC_MSMT_BT_INIT_CHECK_CONFIG);
                dwRes = StartTopologyDetection();
                PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_BT_INIT_CHECK_CONFIG);
                if (EC_E_NOERROR != dwRes)
                {
                    OsDbgAssert(EC_FALSE);
                    m_dwChkResult  = dwRes;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
                }
                /* determine next state */
                m_eCurChkState = ecbtsmchk_process;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsmchk_process:
            {
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTChkStateMachine: Topology changed while scan in %s\n", ECHKSTATESText(m_eCurChkState)));
                    m_dwChkResult  = EC_E_BUSCONFIG_TOPOCHANGE;                    
                    m_eCurChkState = ecbtsmchk_clean;
                    bContinueSM = EC_TRUE;
                    break;
                }
                PerfMeasStart(&G_TscMeasDesc, EC_MSMT_BT_CHECK_CONFIG);
                dwRes = DoTopologyDetection();
                PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_BT_CHECK_CONFIG);
                switch (dwRes)
                {
                case EC_E_NOERROR:
                    {
                        m_eCurChkState = ecbtsmchk_clean;
                        bContinueSM = EC_TRUE;
                    } break;
                case EC_E_BUSCONFIG_MISMATCH:
                    {
                        m_dwChkResult  = dwRes;
                        m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
                    } break;
                case EC_E_BUSY:
                    {
                        m_eCurChkState = ecbtsmchk_process_wait;
                    } break;
                default:
                    {
                        OsDbgAssert(EC_FALSE);
                        m_dwChkResult  = dwRes;
                        m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
                    } break;
                }
            } break;
        case ecbtsmchk_process_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    m_eCurChkState = ecbtsmchk_process;
                    bContinueSM = EC_TRUE;
                }
            } break;

        case ecbtsmchk_clean:
            {
                /* prepare state */
                OsDbgAssert(0 == m_dwEcatCmdPending);
                m_dwErrorCur         = EC_E_NOERROR;
                m_dwEcatCmdPending   = 0;
                m_dwEcatCmdSent      = 0;
                m_dwEcatCmdProcessed = 0;
                m_pBusSlaveCur       = m_pBusSlaveRoot;

                /* determine next state */
                m_eCurChkState = ecbtsmchk_clean_send;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsmchk_clean_send:
            {
            EC_T_WORD wAlStatus     = 0;
            EC_T_WORD wAlControlFrm = 0;
            EC_T_WORD wFixedAddress = 0;

                for (nSlaveSliceCount = m_pMaster->GetMaxSlaveProcessedPerCycle();
                    (nSlaveSliceCount > 0) && (m_pBusSlaveCur != EC_NULL) && (m_dwEcatCmdPending < m_dwEcatCmdPendingMax); 
                     nSlaveSliceCount--,       m_pBusSlaveCur = m_pBusSlaveCur->GetNext())
                {
                    /* only clean slaves with DEVICE_STATE_IDREQUEST set */
                    wAlStatus = m_pBusSlaveCur->GetAlStatus();
                    if (0 == (wAlStatus & DEVICE_STATE_IDREQUEST))
                    {
                        continue;
                    }
                    /* get slave fixed address */
                    wFixedAddress  = m_pBusSlaveCur->GetFixedAddress();

                    EC_SET_FRM_WORD((EC_T_BYTE*)&wAlControlFrm, (EC_T_WORD)(wAlStatus & ~DEVICE_STATE_IDREQUEST));

                    /* queue EtherCAT command */
                    dwRes = m_pMaster->QueueRegisterCmdReq(
                        EC_NULL, INVOKE_ID_SB_CHKCLEAN, EC_CMD_TYPE_FPWR, wFixedAddress,
                        ECREG_AL_CONTROL, sizeof(EC_T_WORD), &wAlControlFrm
                        );
                    if (EC_E_NOERROR != dwRes)
                    {
                        /* try again next cycle */
                        goto Exit;
                    }
                    m_dwEcatCmdPending++;
                    m_dwEcatCmdSent++;
                }
                /* determine next state */
                m_eCurChkState = ecbtsmchk_clean_wait;

                /* nothing sent and slave slicing for this state not exceeded */
                if ((0 == m_dwEcatCmdPending) && (nSlaveSliceCount > 0))
                {
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsmchk_clean_wait:
            {
                if (0 == m_dwEcatCmdPending)
                {
                    /* determine next state */
                    if (EC_NULL != m_pBusSlaveCur)
                    {
                        m_eCurChkState = ecbtsmchk_clean_send;
                    }
                    else
                    {
                        m_eCurChkState = ecbtsmchk_clean_done;
                    }
                    bContinueSM = EC_TRUE;
                }
            } break;
        case ecbtsmchk_clean_done:
            {
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTChkStateMachine: Topology changed while scan in %s\n", ECHKSTATESText(m_eCurChkState)));
                    m_dwChkResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
                    break;
                }
                /* check if all commands processed */
                if (m_dwEcatCmdProcessed != m_dwEcatCmdSent)
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTChkStateMachine: Retry in %s (Processed = %d, Sent = %d)\n", ECHKSTATESText(m_eCurChkState), m_dwEcatCmdProcessed, m_dwEcatCmdSent));
                    m_eCurChkState = ecbtsmchk_clean;
                    break;
                }
                /* determine next state */
                m_eCurChkState = ecbtsmchk_done;
                bContinueSM = EC_TRUE;
            } break;
        case ecbtsmchk_done:
            {
                /* check for topology changed */
                if (TopologyChangedWhileScan())
                {
                    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "BTChkStateMachine: Topology changed while scan in %s\n", ECHKSTATESText(m_eCurChkState)));
                    m_dwChkResult  = EC_E_BUSCONFIG_TOPOCHANGE;
                    m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
                    break;
                }
                if (EC_E_BUSY == m_dwChkResult)
                {
                    m_dwChkResult = EC_E_NOERROR;
                }
                /* done */
            } break;
        case ecbtsmchk_error:
            {
                /* m_dwChkResult set from error place */

                /* done */
            } break;
        default:
            {
                OsDbgAssert(EC_FALSE);
                m_dwChkResult = EC_E_NOTSUPPORTED;
                m_eCurChkState = m_eReqChkState = ecbtsmchk_error;
            } break;
        }
#if (defined INCLUDE_LOG_MESSAGES)
        if (m_eCurChkState != eOldState)
        {
            EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
                "BTChkStateMachine %s->%s (%s, 0x%08X)\n",
                ECHKSTATESText(eOldState), ECHKSTATESText(m_eCurChkState), ECHKSTATESText(m_eReqChkState), m_dwChkResult));
        }
#endif
    } /* while( bContinueSM ) */
Exit:
    return;
}


/***************************************************************************************************/
/**
\brief  Generate State Text.

\return State Text.
*/
EC_T_CHAR* CEcBusTopology::ESTATESText(
    ECBTSM_T_ESTATES    eState  /**< [in]   State to get text to*/
                                      )
{
    EC_T_CHAR* szRet = EC_NULL;

    switch (eState)
    {
#if (defined INCLUDE_LOG_MESSAGES)
    case ecbtsm_start:                  szRet = (EC_T_CHAR*)"ecbtsm_start"; break;
    case ecbtsm_restart:                szRet = (EC_T_CHAR*)"ecbtsm_restart"; break;
    case ecbtsm_start_wait:             szRet = (EC_T_CHAR*)"ecbtsm_start_wait"; break;
    case ecbtsm_start_done:             szRet = (EC_T_CHAR*)"ecbtsm_start_done"; break;
    case ecbtsm_collectinfo:            szRet = (EC_T_CHAR*)"ecbtsm_collectinfo"; break;
    case ecbtsm_collectinfo_wait:       szRet = (EC_T_CHAR*)"ecbtsm_collectinfo_wait"; break;
    case ecbtsm_collectinfo_done:       szRet = (EC_T_CHAR*)"ecbtsm_collectinfo_done"; break;
    case ecbtsm_checktopo:              szRet = (EC_T_CHAR*)"ecbtsm_checktopo"; break;
    case ecbtsm_checktopo_wait:         szRet = (EC_T_CHAR*)"ecbtsm_checktopo_wait"; break;
    case ecbtsm_checktopo_done:         szRet = (EC_T_CHAR*)"ecbtsm_checktopo_done"; break;
    case ecbtsm_writefixaddr:           szRet = (EC_T_CHAR*)"ecbtsm_writefixaddr"; break;
    case ecbtsm_writefixaddr_wait:      szRet = (EC_T_CHAR*)"ecbtsm_writefixaddr_wait"; break;
    case ecbtsm_writefixaddr_done:      szRet = (EC_T_CHAR*)"ecbtsm_writefixaddr_done"; break;
    case ecbtsm_storeinfo:              szRet = (EC_T_CHAR*)"ecbtsm_storeinfo"; break;
    case ecbtsm_storeinfo_wait:         szRet = (EC_T_CHAR*)"ecbtsm_storeinfo_wait"; break;
    case ecbtsm_storeinfo_done:         szRet = (EC_T_CHAR*)"ecbtsm_storeinfo_done"; break;
#if (defined INCLUDE_HOTCONNECT)
    case ecbtsm_borderclose:            szRet = (EC_T_CHAR*)"ecbtsm_borderclose"; break;
    case ecbtsm_borderclose_wait:       szRet = (EC_T_CHAR*)"ecbtsm_borderclose_wait"; break;
    case ecbtsm_borderclose_done:       szRet = (EC_T_CHAR*)"ecbtsm_borderclose_done"; break;
#endif
    case ecbtsm_aliasaddressing:        szRet = (EC_T_CHAR*)"ecbtsm_aliasaddressing"; break;
    case ecbtsm_aliasaddressing_wait:   szRet = (EC_T_CHAR*)"ecbtsm_aliasaddressing_wait"; break;
    case ecbtsm_aliasaddressing_done:   szRet = (EC_T_CHAR*)"ecbtsm_aliasaddressing_done"; break;

    case ecbtsm_checkdlstatusint:       szRet = (EC_T_CHAR*)"ecbtsm_checkdlstatusint"; break;
    case ecbtsm_checkdlstatusint_wait:  szRet = (EC_T_CHAR*)"ecbtsm_checkdlstatusint_wait"; break;
    case ecbtsm_checkdlstatusint_done:  szRet = (EC_T_CHAR*)"ecbtsm_checkdlstatusint_done"; break;

    case ecbtsm_refresh_slaves_presence:        szRet = (EC_T_CHAR*)"ecbtsm_refresh_slaves_presence"; break;
    case ecbtsm_refresh_slaves_presence_wait:   szRet = (EC_T_CHAR*)"ecbtsm_refresh_slaves_presence_wait"; break;
    case ecbtsm_refresh_slaves_presence_done:   szRet = (EC_T_CHAR*)"ecbtsm_refresh_slaves_presence_done"; break;

    case ecbtsm_notify_application:     szRet = (EC_T_CHAR*)"ecbtsm_notify_application"; break;
    case ecbtsm_notify_application_wait:szRet = (EC_T_CHAR*)"ecbtsm_notify_application_wait"; break;
    case ecbtsm_notify_application_done:szRet = (EC_T_CHAR*)"ecbtsm_notify_application_done"; break;
    
    case ecbtsm_busscan_automatic:      szRet = (EC_T_CHAR*)"ecbtsm_busscan_automatic"; break;
    case ecbtsm_busscan_manual:         szRet = (EC_T_CHAR*)"ecbtsm_busscan_manual"; break;
    case ecbtsm_busscan_accepttopo:     szRet = (EC_T_CHAR*)"ecbtsm_busscan_accepttopo"; break;
    case ecbtsm_dlstatus_ist:           szRet = (EC_T_CHAR*)"ecbtsm_dlstatus_ist"; break;
    case ecbtsm_reqto_wait:             szRet = (EC_T_CHAR*)"ecbtsm_reqto_wait"; break;
    case ecbtsm_reqto:                  szRet = (EC_T_CHAR*)"ecbtsm_reqto"; break;
    case ecbtsm_stuck:                  szRet = (EC_T_CHAR*)"ecbtsm_stuck"; break;
    case ecbtsm_idle:                   szRet = (EC_T_CHAR*)"ecbtsm_idle"; break;
    case ecbtsm_unknown:                szRet = (EC_T_CHAR*)"ecbtsm_unknown"; break;
#else
    case 0: /* against compiler warning */
#endif /* INCLUDE_LOG_MESSAGES */

    default:                            szRet = (EC_T_CHAR*)"Unknown"; break;
    }

    return szRet;
}

/***************************************************************************************************/
/**
\brief  Generate State Text.

\return State Text.
*/
EC_T_CHAR* CEcBusTopology::ESUBSTATESText(
    ECBTSM_T_ESUBSTATES eState  /**< [in]   State to get text to*/
    )
{
    EC_T_CHAR* szRet = EC_NULL;

    switch (eState)
    {
#if (defined INCLUDE_LOG_MESSAGES)
    case ecbtsms_unknown:                   szRet = (EC_T_CHAR*)"ecbtsms_unknown"; break;
    case ecbtsms_start:                     szRet = (EC_T_CHAR*)"ecbtsms_start"; break;
    case ecbtsms_countslaves:               szRet = (EC_T_CHAR*)"ecbtsms_countslaves"; break;
    case ecbtsms_countslaves_send:          szRet = (EC_T_CHAR*)"ecbtsms_countslaves_send"; break;
    case ecbtsms_countslaves_wait:          szRet = (EC_T_CHAR*)"ecbtsms_countslaves_wait"; break;
    case ecbtsms_countslaves_done:          szRet = (EC_T_CHAR*)"ecbtsms_countslaves_done"; break;
    case ecbtsms_getbusslaves:              szRet = (EC_T_CHAR*)"ecbtsms_getbusslaves"; break;
    case ecbtsms_getbusslaves_send:         szRet = (EC_T_CHAR*)"ecbtsms_getbusslaves_send"; break;
    case ecbtsms_getbusslaves_wait:         szRet = (EC_T_CHAR*)"ecbtsms_getbusslaves_wait"; break;
    case ecbtsms_getbusslaves_done:         szRet = (EC_T_CHAR*)"ecbtsms_getbusslaves_done"; break;
    case ecbtsms_writetmpaddr:              szRet = (EC_T_CHAR*)"ecbtsms_writetmpaddr"; break;
    case ecbtsms_writetmpaddr_send:         szRet = (EC_T_CHAR*)"ecbtsms_writetmpaddr_send"; break;
    case ecbtsms_writetmpaddr_wait:         szRet = (EC_T_CHAR*)"ecbtsms_writetmpaddr_wait"; break;
    case ecbtsms_writetmpaddr_done:         szRet = (EC_T_CHAR*)"ecbtsms_writetmpaddr_done"; break;
    case ecbtsms_chkwrttmpaddr:             szRet = (EC_T_CHAR*)"ecbtsms_chkwrttmpaddr"; break;
    case ecbtsms_chkwrttmpaddr_send:        szRet = (EC_T_CHAR*)"ecbtsms_chkwrttmpaddr_send"; break;
    case ecbtsms_chkwrttmpaddr_wait:        szRet = (EC_T_CHAR*)"ecbtsms_chkwrttmpaddr_wait"; break;
    case ecbtsms_chkwrttmpaddr_done:        szRet = (EC_T_CHAR*)"ecbtsms_chkwrttmpaddr_done"; break;
    case ecbtsms_readalstatus:              szRet = (EC_T_CHAR*)"ecbtsms_readalstatus"; break;
    case ecbtsms_readalstatus_send:         szRet = (EC_T_CHAR*)"ecbtsms_readalstatus_send"; break;
    case ecbtsms_readalstatus_wait:         szRet = (EC_T_CHAR*)"ecbtsms_readalstatus_wait"; break;
    case ecbtsms_readalstatus_done:         szRet = (EC_T_CHAR*)"ecbtsms_readalstatus_done"; break;
    case ecbtsms_ackslaveerror:             szRet = (EC_T_CHAR*)"ecbtsms_ackslaveerror"; break;
    case ecbtsms_ackslaveerror_send:        szRet = (EC_T_CHAR*)"ecbtsms_ackslaveerror_send"; break;
    case ecbtsms_ackslaveerror_wait:        szRet = (EC_T_CHAR*)"ecbtsms_ackslaveerror_wait"; break;
    case ecbtsms_ackslaveerror_done:        szRet = (EC_T_CHAR*)"ecbtsms_ackslaveerror_done"; break;
    case ecbtsms_getportstatus:             szRet = (EC_T_CHAR*)"ecbtsms_getportstatus"; break;
    case ecbtsms_getportstatus_send:        szRet = (EC_T_CHAR*)"ecbtsms_getportstatus_send"; break;
    case ecbtsms_getportstatus_wait:        szRet = (EC_T_CHAR*)"ecbtsms_getportstatus_wait"; break;
    case ecbtsms_getportstatus_done:        szRet = (EC_T_CHAR*)"ecbtsms_getportstatus_done"; break;
    case ecbtsms_readportctrl:              szRet = (EC_T_CHAR*)"ecbtsms_readportctrl"; break;
    case ecbtsms_readportctrl_send:         szRet = (EC_T_CHAR*)"ecbtsms_readportctrl_send"; break;
    case ecbtsms_readportctrl_wait:         szRet = (EC_T_CHAR*)"ecbtsms_readportctrl_wait"; break;
    case ecbtsms_readportctrl_done:         szRet = (EC_T_CHAR*)"ecbtsms_readportctrl_done"; break;
    case ecbtsms_writeportctrl:             szRet = (EC_T_CHAR*)"ecbtsms_writeportctrl"; break;
    case ecbtsms_writeportctrl_send:        szRet = (EC_T_CHAR*)"ecbtsms_writeportctrl_send"; break;
    case ecbtsms_writeportctrl_wait:        szRet = (EC_T_CHAR*)"ecbtsms_writeportctrl_wait"; break;
    case ecbtsms_writeportctrl_done:        szRet = (EC_T_CHAR*)"ecbtsms_writeportctrl_done"; break;
    case ecbtsms_refreshportstatus:         szRet = (EC_T_CHAR*)"ecbtsms_refreshportstatus"; break;
    case ecbtsms_refreshportstatus_send:    szRet = (EC_T_CHAR*)"ecbtsms_refreshportstatus_send"; break;
    case ecbtsms_refreshportstatus_wait:    szRet = (EC_T_CHAR*)"ecbtsms_refreshportstatus_wait"; break;
    case ecbtsms_refreshportstatus_done:    szRet = (EC_T_CHAR*)"ecbtsms_refreshportstatus_done"; break;
    case ecbtsms_detect_linecrossed:        szRet = (EC_T_CHAR*)"ecbtsms_detect_linecrossed"; break;
#if (defined INCLUDE_LINE_CROSSED_DETECTION)
#if (defined INCLUDE_RED_DEVICE)
    case ecbtsms_redcloseport:              szRet = (EC_T_CHAR*)"ecbtsms_redcloseport"; break;
    case ecbtsms_redcloseport_wait:         szRet = (EC_T_CHAR*)"ecbtsms_redcloseport_wait"; break;
    case ecbtsms_redcloseport_done:         szRet = (EC_T_CHAR*)"ecbtsms_redcloseport_done"; break;
#endif /* INCLUDE_RED_DEVICE */
    case ecbtsms_latchrecvtimes:            szRet = (EC_T_CHAR*)"ecbtsms_latchrecvtimes"; break;
    case ecbtsms_latchrecvtimes_wait:       szRet = (EC_T_CHAR*)"ecbtsms_latchrecvtimes_wait"; break;
    case ecbtsms_latchrecvtimes_done:       szRet = (EC_T_CHAR*)"ecbtsms_latchrecvtimes_done"; break;
    case ecbtsms_readrecvtimes:             szRet = (EC_T_CHAR*)"ecbtsms_readrecvtimes"; break;
    case ecbtsms_readrecvtimes_send:        szRet = (EC_T_CHAR*)"ecbtsms_readrecvtimes_send"; break;
    case ecbtsms_readrecvtimes_wait:        szRet = (EC_T_CHAR*)"ecbtsms_readrecvtimes_wait"; break;
    case ecbtsms_readrecvtimes_done:        szRet = (EC_T_CHAR*)"ecbtsms_readrecvtimes_done"; break;
#if (defined INCLUDE_RED_DEVICE)
    case ecbtsms_redopenport:               szRet = (EC_T_CHAR*)"ecbtsms_redopenport"; break;
    case ecbtsms_redopenport_send:          szRet = (EC_T_CHAR*)"ecbtsms_redopenport_send"; break;
    case ecbtsms_redopenport_wait:          szRet = (EC_T_CHAR*)"ecbtsms_redopenport_wait"; break;
    case ecbtsms_redopenportrefreshportstatus_send: szRet = (EC_T_CHAR*)"ecbtsms_redopenportrefreshportstatus_send"; break;
    case ecbtsms_redopenportrefreshportstatus_wait: szRet = (EC_T_CHAR*)"ecbtsms_redopenportrefreshportstatus_wait"; break;
    case ecbtsms_redopenport_done:          szRet = (EC_T_CHAR*)"ecbtsms_redopenport_done"; break;
#endif /* INCLUDE_RED_DEVICE */
#endif /* INCLUDE_LINE_CROSSED_DETECTION */
    case ecbtsms_detect_linecrossed_done:   szRet = (EC_T_CHAR*)"ecbtsms_detect_linecrossed_done"; break;
    case ecbtsms_readinterrupt:             szRet = (EC_T_CHAR*)"ecbtsms_readinterrupt"; break;
    case ecbtsms_readinterrupt_send:        szRet = (EC_T_CHAR*)"ecbtsms_readinterrupt_send"; break;
    case ecbtsms_readinterrupt_wait:        szRet = (EC_T_CHAR*)"ecbtsms_readinterrupt_wait"; break;
    case ecbtsms_readinterrupt_done:        szRet = (EC_T_CHAR*)"ecbtsms_readinterrupt_done"; break;
    case ecbtsms_cfginterrupt:              szRet = (EC_T_CHAR*)"ecbtsms_cfginterrupt"; break;
    case ecbtsms_cfginterrupt_send:         szRet = (EC_T_CHAR*)"ecbtsms_cfginterrupt_send"; break;
    case ecbtsms_cfginterrupt_wait:         szRet = (EC_T_CHAR*)"ecbtsms_cfginterrupt_wait"; break;
    case ecbtsms_cfginterrupt_done:         szRet = (EC_T_CHAR*)"ecbtsms_cfginterrupt_done"; break;
    case ecbtsms_readbusconfig:             szRet = (EC_T_CHAR*)"ecbtsms_readbusconfig"; break;
    case ecbtsms_readbusconfig_wait:        szRet = (EC_T_CHAR*)"ecbtsms_readbusconfig_wait"; break;
    case ecbtsms_readbusconfig_done:        szRet = (EC_T_CHAR*)"ecbtsms_readbusconfig_done"; break;
    case ecbtsms_readpdicontrol:            szRet = (EC_T_CHAR*)"ecbtsms_readpdicontrol"; break;
    case ecbtsms_readpdicontrol_send:       szRet = (EC_T_CHAR*)"ecbtsms_readpdicontrol_send"; break;
    case ecbtsms_readpdicontrol_wait:       szRet = (EC_T_CHAR*)"ecbtsms_readpdicontrol_wait"; break;
    case ecbtsms_readpdicontrol_done:       szRet = (EC_T_CHAR*)"ecbtsms_readpdicontrol_done"; break;
    case ecbtsms_probedcunit:               szRet = (EC_T_CHAR*)"ecbtsms_probedcunit"; break;
#if (defined INCLUDE_DC_SUPPORT)
    case ecbtsms_probedcunit_send:          szRet = (EC_T_CHAR*)"ecbtsms_probedcunit_send"; break;
    case ecbtsms_probedcunit_wait:          szRet = (EC_T_CHAR*)"ecbtsms_probedcunit_wait"; break;
#endif
    case ecbtsms_probedcunit_done:          szRet = (EC_T_CHAR*)"ecbtsms_probedcunit_done"; break;
    case ecbtsms_buildbustopology:          szRet = (EC_T_CHAR*)"ecbtsms_buildbustopology"; break;
    case ecbtsms_buildbustopology_wait:     szRet = (EC_T_CHAR*)"ecbtsms_buildbustopology_wait"; break;
    case ecbtsms_buildbustopology_done:     szRet = (EC_T_CHAR*)"ecbtsms_buildbustopology_done"; break;    

    case ecbtsms_parseports:                szRet = (EC_T_CHAR*)"ecbtsms_parseports"; break;
    case ecbtsms_parseports_wait:           szRet = (EC_T_CHAR*)"ecbtsms_parseports_wait"; break;
    case ecbtsms_parseports_done:           szRet = (EC_T_CHAR*)"ecbtsms_parseports_done"; break;

    case ecbtsms_findintsource:             szRet = (EC_T_CHAR*)"ecbtsms_findintsource"; break;
    case ecbtsms_findintsource_send:        szRet = (EC_T_CHAR*)"ecbtsms_findintsource_send"; break;
    case ecbtsms_findintsource_wait:        szRet = (EC_T_CHAR*)"ecbtsms_findintsource_wait"; break;
    case ecbtsms_findintsource_done:        szRet = (EC_T_CHAR*)"ecbtsms_findintsource_done"; break;
    case ecbtsms_ackinterrupt:              szRet = (EC_T_CHAR*)"ecbtsms_ackinterrupt"; break;
    case ecbtsms_ackinterrupt_send:         szRet = (EC_T_CHAR*)"ecbtsms_ackinterrupt_send"; break;
    case ecbtsms_ackinterrupt_wait:         szRet = (EC_T_CHAR*)"ecbtsms_ackinterrupt_wait"; break;
    case ecbtsms_ackinterrupt_done:         szRet = (EC_T_CHAR*)"ecbtsms_ackinterrupt_done"; break;

    case ecbtsms_activatealias:             szRet = (EC_T_CHAR*)"ecbtsms_activatealias"; break;
    case ecbtsms_activatealias_wait:        szRet = (EC_T_CHAR*)"ecbtsms_activatealias_wait"; break;
    case ecbtsms_activatealias_done:        szRet = (EC_T_CHAR*)"ecbtsms_activatealias_done"; break;

    case ecbtsms_writefixaddr:              szRet = (EC_T_CHAR*)"ecbtsms_writefixaddr"; break;
    case ecbtsms_writefixaddr_send:         szRet = (EC_T_CHAR*)"ecbtsms_writefixaddr_send"; break;
    case ecbtsms_writefixaddr_wait:         szRet = (EC_T_CHAR*)"ecbtsms_writefixaddr_wait"; break;
    case ecbtsms_writefixaddr_done:         szRet = (EC_T_CHAR*)"ecbtsms_writefixaddr_done"; break;
    case ecbtsms_checkfixedaddress:         szRet = (EC_T_CHAR*)"ecbtsms_checkfixedaddress"; break;
    case ecbtsms_checkfixedaddress_send:    szRet = (EC_T_CHAR*)"ecbtsms_checkfixedaddress_send"; break;
    case ecbtsms_checkfixedaddress_wait:    szRet = (EC_T_CHAR*)"ecbtsms_checkfixedaddress_wait"; break;
    case ecbtsms_checkfixedaddress_done:    szRet = (EC_T_CHAR*)"ecbtsms_checkfixedaddress_done"; break;

    case ecbtsms_collectbusscan:            szRet = (EC_T_CHAR*)"ecbtsms_collectbusscan"; break;
#if (defined INCLUDE_HOTCONNECT)
    case ecbtsms_borderclose:               szRet = (EC_T_CHAR*)"ecbtsms_borderclose"; break;
#endif
    case ecbtsms_collectdlstatus:           szRet = (EC_T_CHAR*)"ecbtsms_collectdlstatus"; break;
    case ecbtsms_collectalstatus:           szRet = (EC_T_CHAR*)"ecbtsms_collectalstatus"; break;
    case ecbtsms_enainterrupt:              szRet = (EC_T_CHAR*)"ecbtsms_enainterrupt"; break;
    case ecbtsms_disinterrupt:              szRet = (EC_T_CHAR*)"ecbtsms_disinterrupt"; break;
    case ecbtsms_aliasactive:               szRet = (EC_T_CHAR*)"ecbtsms_aliasactive"; break;
    case ecbtsms_wrtfaddr:                  szRet = (EC_T_CHAR*)"ecbtsms_wrtfaddr"; break;
    case ecbtsms_timeout:                   szRet = (EC_T_CHAR*)"ecbtsms_timeout"; break;
    case ecbtsms_error:                     szRet = (EC_T_CHAR*)"ecbtsms_error"; break;
#else
    case 0: /* against compiler warning */
#endif /* INCLUDE_LOG_MESSAGES */
    default:                                szRet = (EC_T_CHAR*)"Unknown"; break;
    }

    return szRet;
}

/***************************************************************************************************/
/**
\brief  Generate State Text.

\return State Text.
*/
EC_T_CHAR* CEcBusTopology::EEEPSTATESText(
    ECBTSM_T_EEEPSTATES eState  /**< [in]   State to get text to*/
                                       )
{
    EC_T_CHAR* szRet = EC_NULL;

    switch (eState)
    {
#if (defined INCLUDE_LOG_MESSAGES)
    case ecbtsme_unknown:               szRet = (EC_T_CHAR*)"ecbtsme_unknown"; break;
    case ecbtsme_start:                 szRet = (EC_T_CHAR*)"ecbtsme_start"; break;
    case ecbtsme_acq_eep_access:        szRet = (EC_T_CHAR*)"ecbtsme_acq_eep_access"; break;
    case ecbtsme_acq_eep_access_wait:   szRet = (EC_T_CHAR*)"ecbtsme_acq_eep_access_wait"; break;
    case ecbtsme_acq_eep_access_done:   szRet = (EC_T_CHAR*)"ecbtsme_acq_eep_access_done"; break;
    case ecbtsme_chkprebusy:            szRet = (EC_T_CHAR*)"ecbtsme_chkprebusy"; break;
    case ecbtsme_chkprebusy_wait:       szRet = (EC_T_CHAR*)"ecbtsme_chkprebusy_wait"; break;
    case ecbtsme_chkprebusy_done:       szRet = (EC_T_CHAR*)"ecbtsme_chkprebusy_done"; break;
    case ecbtsme_writeaddr:             szRet = (EC_T_CHAR*)"ecbtsme_writeaddr"; break;
    case ecbtsme_writeaddr_send:        szRet = (EC_T_CHAR*)"ecbtsme_writeaddr_send"; break;
    case ecbtsme_writeaddr_wait:        szRet = (EC_T_CHAR*)"ecbtsme_writeaddr_wait"; break;
    case ecbtsme_writeaddr_done:        szRet = (EC_T_CHAR*)"ecbtsme_writeaddr_done"; break;
    case ecbtsme_chkbusy:               szRet = (EC_T_CHAR*)"ecbtsme_chkbusy"; break;
    case ecbtsme_chkbusy_wait:          szRet = (EC_T_CHAR*)"ecbtsme_chkbusy_wait"; break;
    case ecbtsme_chkbusy_done:          szRet = (EC_T_CHAR*)"ecbtsme_chkbusy_done"; break;
    case ecbtsme_readvalue:             szRet = (EC_T_CHAR*)"ecbtsme_readvalue"; break;
    case ecbtsme_readvalue_send:        szRet = (EC_T_CHAR*)"ecbtsme_readvalue_send"; break;
    case ecbtsme_readvalue_wait:        szRet = (EC_T_CHAR*)"ecbtsme_readvalue_wait"; break;
    case ecbtsme_readvalue_done:        szRet = (EC_T_CHAR*)"ecbtsme_readvalue_done"; break;
    case ecbtsme_done:                  szRet = (EC_T_CHAR*)"ecbtsme_done"; break;
    case ecbtsme_timeout:               szRet = (EC_T_CHAR*)"ecbtsme_timeout"; break;
    case ecbtsme_error:                 szRet = (EC_T_CHAR*)"ecbtsme_error"; break;
#else
    case 0: /* against compiler warning */
#endif /* INCLUDE_LOG_MESSAGES */
    default:                            szRet = (EC_T_CHAR*)"Unknown"; break;
    }

    return szRet;
}

/***************************************************************************************************/
/**
\brief  Generate State Text.

\return State Text.
*/
EC_T_CHAR* CEcBusTopology::ECHKSTATESText(
    ECBTSMCHK_T_STATES eState  /**< [in]   State to get text to*/
    )
{
    EC_T_CHAR* szRet = EC_NULL;

    switch (eState)
    {
#if (defined INCLUDE_LOG_MESSAGES)
    case ecbtsmchk_unknown:       szRet = (EC_T_CHAR*)"ecbtsmchk_unknown"; break;
    case ecbtsmchk_idle:          szRet = (EC_T_CHAR*)"ecbtsmchk_idle"; break;
    case ecbtsmchk_start:         szRet = (EC_T_CHAR*)"ecbtsmchk_start"; break;
    case ecbtsmchk_process:       szRet = (EC_T_CHAR*)"ecbtsmchk_process"; break;
    case ecbtsmchk_process_wait:  szRet = (EC_T_CHAR*)"ecbtsmchk_process_wait"; break;
    case ecbtsmchk_done:          szRet = (EC_T_CHAR*)"ecbtsmchk_done"; break;
    case ecbtsmchk_error:         szRet = (EC_T_CHAR*)"ecbtsmchk_error"; break;
#else
    case 0: /* against compiler warning */
#endif /* INCLUDE_LOG_MESSAGES */
    default:                      szRet = (EC_T_CHAR*)"Unknown"; break;
    }

    return szRet;
}


/***************************************************************************************************/
/**
\brief  Add free address range, which may be used.
*/
EC_T_VOID CEcBusTopology::FreeRangeSet(
    EC_T_DWORD  dwIdx   /**< [in]   Range index */
   ,EC_T_WORD   wBegin  /**< [in]   Range begin */
   ,EC_T_WORD   wEnd    /**< [in]   Range end */
                                      )
{
    ECBT_T_ADDRRANGE*   pRange = EC_NULL;
    if (dwIdx > ECBT_ADDRRANGE_AMOUNT)
    {
        goto Exit;
    }

    pRange = &(m_abFreeAddresses[dwIdx]);
    pRange->wStartAddr  = wBegin;
    pRange->wEndAddr    = wEnd;
Exit:
    return;
}


/***************************************************************************************************/
/**
\brief  Release all the temporary fixed address
*/
EC_T_VOID CEcBusTopology::ReleaseTmpFixedAddress(EC_T_VOID)
{
    EC_T_DWORD dwIdx = 0;

    for( dwIdx = 0; dwIdx < (sizeof(m_abFreeAddresses)/sizeof(ECBT_T_ADDRRANGE)); dwIdx++ )
    {
        m_abFreeAddresses[dwIdx].bRangeUsed = EC_FALSE;
    }
    return;
}


/***************************************************************************************************/
/**
\brief  Seek for next unused Slave address, which can be used as temporary address.

\return Temporary Slave address or 0 on error.
*/
EC_T_WORD CEcBusTopology::TakeTmpFixedAddress(EC_T_VOID)
{
    EC_T_WORD   wTmpAddress = 0;
    EC_T_DWORD  dwIdx       = 0;

    /* find range with free space */
    for (dwIdx = 0; dwIdx < (sizeof(m_abFreeAddresses)/sizeof(ECBT_T_ADDRRANGE)); dwIdx++)
    {
        /* if free address entry not used yet, mark it and initialize iterator */
        if (!m_abFreeAddresses[dwIdx].bRangeUsed)
        {
            m_abFreeAddresses[dwIdx].bRangeUsed = EC_TRUE;
            m_abFreeAddresses[dwIdx].wWorkAddr  = m_abFreeAddresses[dwIdx].wStartAddr;
        }
        /* no space in range, mark as used and skip */
        if (!((m_abFreeAddresses[dwIdx].wStartAddr)<m_abFreeAddresses[dwIdx].wEndAddr))
        {
            m_abFreeAddresses[dwIdx].bRangeUsed = EC_TRUE;
            continue;
        }
        /* use next free address in range */
        if (m_abFreeAddresses[dwIdx].wWorkAddr <= m_abFreeAddresses[dwIdx].wEndAddr)
        {
            wTmpAddress = m_abFreeAddresses[dwIdx].wWorkAddr++;
            break;
        }
    }
    return wTmpAddress;
}

/***************************************************************************************************/
/**
\brief  Determine if Al Status ERROR at slave shall be acknowledged now

\return Al Status Error shall be acknowledged
*/
EC_T_BOOL CEcBusTopology::ShallAckAlStatusError(CEcBusSlave* pBusSlave)
{
    EC_T_BOOL bAutoAckAlStatusError = EC_FALSE;

    if (!pBusSlave->IsAlStatusErrorPending())
    {
        bAutoAckAlStatusError = EC_FALSE;
        goto Exit;
    }

    if (EC_NULL == pBusSlave->GetCfgSlave())
    {
        bAutoAckAlStatusError = EC_TRUE;
        goto Exit;
    }

    if (m_pMaster->GetAutoAckAlStatusError())
    {
        bAutoAckAlStatusError = EC_TRUE;
        goto Exit;
    }

    bAutoAckAlStatusError = EC_FALSE;
Exit:
    return bAutoAckAlStatusError;
}

/*-/class CEcBusTopology-----------------------------------------------------*/


#if (defined INSTRUMENT_MASTER) || (defined ECBT_PRINTBUSSLAVESLIST)
EC_T_VOID CEcBusTopology::PrintBusSlaveList(CEcBusSlave* pBusSlaveList)
{
EC_T_INT     nBusSlaveIdx = 0;
EC_T_INT     nPortIdx     = 0;
CEcBusSlave* pBusSlave    = EC_NULL;

    if (EC_NULL == pBusSlaveList)
    {
        EC_DBGMSG("Bus slave list is empty\n");
        EC_DBGMSG("**************************************************\n");
        goto Exit;
    }
    for (pBusSlave = pBusSlaveList, nBusSlaveIdx = 0; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext(), nBusSlaveIdx++)
    {
        EC_DBGMSG("\n");
        EC_DBGMSG("Bus slave %d:\n", nBusSlaveIdx);
        EC_DBGMSG("**************************************************\n");

        EC_DBGMSG("VendorId : 0x%08X; ProductCode : 0x%08X\n", pBusSlave->GetVendorId(), pBusSlave->GetProductCode());

        EC_DBGMSG("FixedAddress : %d; DL Status: 0x%x\n", pBusSlave->GetFixedAddress(), EC_HIBYTE(pBusSlave->GetDlStatus()));

        for (nPortIdx = 0; nPortIdx < ESC_PORT_COUNT; nPortIdx++)
        {
        EC_T_WORD  wPortCur          =  CEcBusTopology::c_awFrameProcessingPortOrder[nPortIdx];
        EC_T_DWORD dwPortRecvTimeCur = pBusSlave->GetRecvTimeX(wPortCur);

            EC_DBGMSG("RecvTime[%d] = 0x%08X\n", wPortCur, dwPortRecvTimeCur);
        }
        for (nPortIdx = 0; nPortIdx < ESC_PORT_COUNT; nPortIdx++)
        {
        EC_T_WORD    wPortCur       =  CEcBusTopology::c_awFrameProcessingPortOrder[nPortIdx];
        CEcBusSlave* pBusSlaveChild = pBusSlave->m_apBusSlaveChildren[wPortCur];

            EC_DBGMSG("ConnectedTo[%d] = 0x%08X\n", wPortCur, ((EC_NULL == pBusSlaveChild)?0:pBusSlaveChild->GetFixedAddress()));
        }
        EC_DBGMSG("\n");
    }
Exit:
    return;
}
#endif /* INSTRUMENT_MASTER || ECBT_PRINTBUSSLAVESLIST */

#if (defined INSTRUMENT_MASTER) || (defined ECBT_PRINTBUSSLAVESTREE)
EC_T_VOID CEcBusTopology::PrintBusSlaveTreeNode(CEcBusSlave* pBusSlave, EC_T_DWORD dwTabs)
{
EC_T_WORD wPortIdxStart = 0;
EC_T_WORD wPortIdx      = 0;
CEcSlave* pCfgSlave     = EC_NULL;

    /* check bus slave */
    if (EC_NULL == pBusSlave)
    {
        return;
    }
    /* print information about the slave */
    pCfgSlave = pBusSlave->GetCfgSlave();
    if (EC_NULL == pCfgSlave)
    {
        EC_DBGMSG("Slave 0x%08X:0x%08X\r\n", pBusSlave->GetVendorId(), pBusSlave->GetProductCode());
    }
    else
    {
        EC_DBGMSG("Slave %d: %s\r\n", pCfgSlave->GetSlaveId(), pCfgSlave->GetName());
    }

    /* get port index of input port */
    for (wPortIdxStart = 0; wPortIdxStart < ESC_PORT_COUNT; wPortIdxStart++)
    {
        if (pBusSlave->m_wInputPort == CEcBusTopology::c_awFrameProcessingPortOrder[wPortIdxStart])
        {
            break;
        }
    }
    /* go through ports following the expected propagation order starting after input port */
    for (wPortIdx = 1; wPortIdx < ESC_PORT_COUNT; wPortIdx++)
    {
    EC_T_WORD    wPortCur       = CEcBusTopology::c_awFrameProcessingPortOrder[(EC_T_DWORD)(wPortIdxStart + wPortIdx) % ESC_PORT_COUNT];
    CEcBusSlave* pBusSlaveChild = pBusSlave->m_apBusSlaveChildren[wPortCur];

        if (EC_NULL != pBusSlaveChild)
        {
            for (EC_T_WORD wLoop = 0; wLoop < dwTabs; wLoop++)
            {
                EC_DBGMSG(" ");
            }
            EC_DBGMSG("+%d:", wPortCur);

            /* recursive call for children nodes */
            PrintBusSlaveTreeNode(pBusSlaveChild, dwTabs+3);
        }
    }
}
EC_T_VOID CEcBusTopology::PrintBusSlaveTree(CEcBusSlave* pBusSlave, EC_T_BOOL bRedLine)
{
    EC_DBGMSG("\nBus slaves tree");
    if (bRedLine)
    {
        EC_DBGMSG(" (redundancy line)\r\n");
    }
    else
    {
        EC_DBGMSG(" (main line)\r\n");
    }
    EC_DBGMSG("**************************************************\n");
    PrintBusSlaveTreeNode(pBusSlave, 0);
}
#endif /* INSTRUMENT_MASTER || ECBT_PRINTBUSSLAVESTREE */

#if (defined INCLUDE_LOG_MESSAGES)
EC_T_VOID CEcBusTopology::PrintTopologyTest(CEcBusSlave* pBusSlave)
{
#if (defined INCLUDE_DC_SUPPORT) || (defined INCLUDE_LINE_CROSSED_DETECTION)

EC_T_BYTE byDbgLvl = (EC_T_BYTE)((m_pMaster->m_MasterConfig.dwStateChangeDebug>>2) &3);

    /* dump some register values according EcBusTopology_tests::CTestSlave */
    for (; EC_NULL != pBusSlave; pBusSlave = pBusSlave->GetNext())
    {
        EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG),
            "{0x%02X, 0x%04X, {0x%08X, 0x%08X, 0x%08X, 0x%08X}, 0x%08X%08X, {%4d, %4d, %4d, %4d}, %d}, /* %s */\n", 
            pBusSlave->GetPortDescriptor(),
            pBusSlave->GetDlStatus(), 
            pBusSlave->GetRecvTimeX(ESC_PORT_A), pBusSlave->GetRecvTimeX(ESC_PORT_B), pBusSlave->GetRecvTimeX(ESC_PORT_C), pBusSlave->GetRecvTimeX(ESC_PORT_D), 
            EC_HIDWORD(pBusSlave->GetRecvTimeProcessingUnit()), EC_LODWORD(pBusSlave->GetRecvTimeProcessingUnit()),
            ((EC_NULL == pBusSlave->m_apBusSlaveChildren[ESC_PORT_A])?INVALID_FIXED_ADDR:(pBusSlave->m_apBusSlaveChildren[ESC_PORT_A]->GetFixedAddress())),
            ((EC_NULL == pBusSlave->m_apBusSlaveChildren[ESC_PORT_D])?INVALID_FIXED_ADDR:(pBusSlave->m_apBusSlaveChildren[ESC_PORT_D]->GetFixedAddress())),
            ((EC_NULL == pBusSlave->m_apBusSlaveChildren[ESC_PORT_B])?INVALID_FIXED_ADDR:(pBusSlave->m_apBusSlaveChildren[ESC_PORT_B]->GetFixedAddress())),
            ((EC_NULL == pBusSlave->m_apBusSlaveChildren[ESC_PORT_C])?INVALID_FIXED_ADDR:(pBusSlave->m_apBusSlaveChildren[ESC_PORT_C]->GetFixedAddress())),
            pBusSlave->IsLineCrossed(),
            ((EC_NULL == pBusSlave->GetCfgSlave())?"unknown slave":pBusSlave->GetCfgSlave()->GetName())));
    }
    EC_DBGMSGL((byDbgLvl, ETHTYPE1, (ETHTYPE0+STATE_CHNG), "**************************************************\n"));
#else
    EC_UNREFPARM(pBusSlave);
#endif /* defined INCLUDE_DC_SUPPORT || INCLUDE_LINE_CROSSED_DETECTION */
}
#endif /* INCLUDE_LOG_MESSAGES */

EC_T_MEMORY_LOGGER* CEcBusTopology::GetMemLog()
{ 
    return m_pMaster->GetMemLog(); 
}

#if (defined INCLUDE_RESCUE_SCAN)
/* c_awNextPortToCheckOrder[ESC_PORT_A] == ESC_PORT_D
   c_awNextPortToCheckOrder[ESC_PORT_D] == ESC_PORT_B
   c_awNextPortToCheckOrder[ESC_PORT_B] == ESC_PORT_C
   c_awNextPortToCheckOrder[ESC_PORT_C] == ESC_PORT_A */
const EC_T_WORD CRescueScan::c_awNextPortToCheckOrder[] =
{ ESC_PORT_D, ESC_PORT_C, ESC_PORT_A, ESC_PORT_B };

const EC_T_DWORD CRescueScan::c_dwDlControlCloseAllButPort0 = ECM_DLCTRL_NONEC_FORWARDING |
    (ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE<<ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_A)) |
    (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED<<ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_B)) |
    (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED<<ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_C)) |
    (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED<<ECM_DLCTRL_LOOP_PORTX_SHIFT(ESC_PORT_D)) |
    (ECM_DLCTRL_RXFIFOSIZE_MASK           <<ECM_DLCTRL_RXFIFOSIZE_SHIFT) |
    (ECM_DLCTRL_USEALIAS & 0);

CRescueScan::CRescueScan(CAtEmInterface* pInterface)
{
    m_pInterface = pInterface;
    m_pMaster = m_pInterface->m_oAtEcatDesc.poMaster;
}

EC_T_BOOL CRescueScan::GetNextPortToCheck(EC_T_WORD* pwAutoIncAddr, EC_T_WORD* pwPort)
{
    EC_T_WORD wAutoIncAddr = *pwAutoIncAddr;
    EC_T_WORD wPort = *pwPort;
    EC_T_BOOL bAscend = EC_FALSE;

    CEcBusSlave* pBusSlaveCur = m_pMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wAutoIncAddr);

    if ((m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE) && (EC_NULL != pBusSlaveCur))
    {
        OsDbgMsg("RescueScanGetNextPortToCheck: %d, %d, %d\n", wAutoIncAddr, pBusSlaveCur->GetFixedAddress(), wPort);
    }
    while ((EC_NULL != pBusSlaveCur) && !m_Timeout.IsElapsed())
    {
        /* decent */
        CEcBusSlave* pSlaveConnectedAtPort = pBusSlaveCur->m_apBusSlaveChildren[wPort];
        if ((EC_NULL != pSlaveConnectedAtPort) && !bAscend)
        {
            wPort = pSlaveConnectedAtPort->m_wParentPort; /* should be ESC_PORT_A */
            pBusSlaveCur = pSlaveConnectedAtPort;
        }
        bAscend = EC_FALSE;

        /* turn left */
        wPort = c_awNextPortToCheckOrder[wPort];

        /* ascend */
        if (ESC_PORT_A == wPort)
        {
            /* turn left */
            wPort = pBusSlaveCur->GetPortAtParent();
            pBusSlaveCur = pBusSlaveCur->GetParentSlave();
            bAscend = EC_TRUE;
            continue;
        }
        /* DetectNewPortConnection */
        if (!pBusSlaveCur->IsSlaveCfgPortX(wPort))
        {
            continue;
        }
        if (pBusSlaveCur->IsLoopPortX(wPort) && pBusSlaveCur->IsSignalPortX(wPort))
        {
            /* NewPortConnectionDetected */
#if (defined INCLUDE_PORT_OPERATION)
            if (!m_pMaster->m_oHotConnect.IsPortClosed(pBusSlaveCur->GetFixedAddress(), wPort))
#endif
            {
                wAutoIncAddr = pBusSlaveCur->GetAutoIncAddress();
                if ((m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE) && (EC_NULL != pBusSlaveCur))
                {
                    OsDbgMsg("RescueScanGetNextPortToCheck: %d, %d, %d, %d\n", *pwAutoIncAddr, *pwPort, wAutoIncAddr, wPort);
                }
                *pwAutoIncAddr = wAutoIncAddr;
                *pwPort = wPort;
                return EC_TRUE;
            }
        }
    }
    return EC_FALSE;
}

EC_T_DWORD CRescueScan::CheckPortForFrameLoss(EC_T_WORD wAutoIncAddr, EC_T_WORD wPort)
{
EC_T_DWORD dwRetVal           = EC_E_ERROR;
EC_T_DWORD dwRes              = EC_E_ERROR;
EC_T_BOOL  bCheckFirstPort    = EC_FALSE;
EC_T_WORD  wFixedAddressCur   = INVALID_FIXED_ADDR;
EC_T_WORD  wAutoIncAddressCur = INVALID_AUTO_INC_ADDR;
EC_T_DWORD dwDlControlCur     = 0;
EC_T_DWORD dwDlControl        = 0;
EC_T_DWORD dwDlControlFrm     = 0;

    if ((0 == wAutoIncAddr) && (ESC_PORT_A == wPort))
    {
        bCheckFirstPort = EC_TRUE;
    }
    else
    {
    CEcBusSlave* pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(wAutoIncAddr);

        wFixedAddressCur   = pBusSlave->GetFixedAddress();
        wAutoIncAddressCur = pBusSlave->GetAutoIncAddress();
        dwDlControlCur     = pBusSlave->GetDlControl();
    }
    if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
    {
        OsDbgMsg("RescueScanCheckPortForFrameLoss: %d, %d, %d...\n", wAutoIncAddressCur, wFixedAddressCur, wPort);
    }
    if (bCheckFirstPort)
    {
        /* close all ports, but port 0 of first slave */
        {
            EC_SET_FRM_DWORD(&dwDlControlFrm, c_dwDlControlCloseAllButPort0);
            dwRes = m_pMaster->EcTransferRawCmd(
                0, 0, EC_CMD_TYPE_APWR,
                (((EC_T_DWORD)wFixedAddressCur) << 16) | ECREG_DL_CONTROL,
                (EC_T_BYTE*)&dwDlControlFrm, sizeof(EC_T_DWORD), m_pMaster->GetEcatCmdTimeout()
                );
            if ((EC_E_TIMEOUT != dwRes) && (EC_E_NOERROR != dwRes))
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
    }
    else
    {
        /* open current port of current slave */
        {
            dwDlControl  = dwDlControlCur & ~(ECM_DLCTRL_LOOP_PORTX_MASK(wPort));
            dwDlControl |= (ECM_DLCTRL_LOOP_PORT_VAL_AUTOCLOSE << ECM_DLCTRL_LOOP_PORTX_SHIFT(wPort));
            EC_SET_FRM_DWORD(&dwDlControlFrm, dwDlControl);
            dwRes = m_pMaster->EcTransferRawCmd(
                EC_NULL, 0, EC_CMD_TYPE_FPWR,
                (((EC_T_DWORD)wFixedAddressCur) << 16) | ECREG_DL_CONTROL,
                (EC_T_BYTE*)&dwDlControlFrm, sizeof(EC_T_DWORD), m_pMaster->GetEcatCmdTimeout()
                );
            if ((EC_E_TIMEOUT != dwRes) && (EC_E_NOERROR != dwRes))
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
        /* apply topology change delay */
        OsSleep(m_pMaster->m_pBusTopology->GetTopologyChangeDelay());

        /* close ports of next slave */
        {
        EC_T_WORD wAutoIncAddrNextSlave = (EC_T_WORD)(0 - m_pInterface->GetNumConnectedSlaves());

            EC_SET_FRM_DWORD(&dwDlControlFrm, c_dwDlControlCloseAllButPort0);
            dwRes = m_pMaster->EcTransferRawCmd(
                EC_NULL, 0, EC_CMD_TYPE_APWR,
                (((EC_T_DWORD)wAutoIncAddrNextSlave) << 16) | ECREG_DL_CONTROL,
                (EC_T_BYTE*)&dwDlControlFrm, sizeof(EC_T_DWORD), m_pMaster->GetEcatCmdTimeout()
                );
            if ((EC_E_TIMEOUT != dwRes) && (EC_E_NOERROR != dwRes))
            {
                dwRetVal = dwRes;
                goto Exit;
            }
        }
    }
    /* trigger a scan bus and check for frameloss */
    dwRes = m_pInterface->ScanBus(m_Timeout.GetDuration());
    switch (dwRes)
    {
    case EC_E_NOERROR:
    case EC_E_BUSCONFIG_MISMATCH:
        break;
    default:
        dwRetVal = dwRes;
        goto Exit;
        /* no break */
    }
    if (0 == m_pInterface->GetNumConnectedSlaves())
    {
        /* frame loss detected at current port */
        if (!bCheckFirstPort)
        {
            /* close ports of current slave again */
            {
                dwDlControl  = dwDlControlCur & ~(ECM_DLCTRL_LOOP_PORTX_MASK(wPort));
                dwDlControl |= (ECM_DLCTRL_LOOP_PORT_VAL_ALWAYSCLOSED << ECM_DLCTRL_LOOP_PORTX_SHIFT(wPort));
                EC_SET_FRM_DWORD(&dwDlControlFrm, dwDlControl);
                dwRes = m_pMaster->EcTransferRawCmd(
                    EC_NULL, 0, EC_CMD_TYPE_FPWR,
                    (((EC_T_DWORD)wFixedAddressCur) << 16) | ECREG_DL_CONTROL,
                    (EC_T_BYTE*)&dwDlControlFrm, sizeof(EC_T_DWORD), m_pMaster->GetEcatCmdTimeout()
                    );
                if ((EC_E_TIMEOUT != dwRes) && (EC_E_NOERROR != dwRes))
                {
                    dwRetVal = dwRes;
                    goto Exit;
                }
            }
            /* trigger a scan bus to prepare next step */
            dwRes = m_pInterface->ScanBus(m_Timeout.GetDuration());
            switch (dwRes)
            {
            case EC_E_NOERROR:
            case EC_E_BUSCONFIG_MISMATCH:
                break;
            default:
                dwRetVal = dwRes;
                goto Exit;
                /* no break */
            }
            OsDbgAssert(0 != m_pInterface->GetNumConnectedSlaves());
        }
        dwRetVal = EC_E_FRAMELOSS_AFTER_SLAVE;
        goto Exit;
    }
    /* no error */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
    {
        OsDbgMsg("RescueScanCheckPortForFrameLoss: %d, %d, %d: %s (0x%x)\n", wFixedAddressCur, wFixedAddressCur, wPort, EcErrorText(dwRetVal), dwRetVal);
    }
    return dwRetVal;
}

typedef struct _EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION
{
    EC_T_WORD wAutoIncAddr;
    EC_T_WORD wPort;
} EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION;

/***************************************************************************************************/
/** \brief Scans all connected slaves, handling frame loss after slaves
\return EC_E_NOERROR if it succeeds or error code if it fails
*/
EC_T_DWORD CRescueScan::RescueScan(EC_T_DWORD dwTimeout /**< [in] Time-out in ms */)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwRes = EC_E_ERROR;
    CFiFoListDyn<EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION> FramelossAfterSlaveLocationsFifo(m_pMaster->GetMaxBusSlaves() * ESC_PORT_COUNT, EC_NULL, EC_NULL, EC_MEMTRACE_INTERFACE);

    /* disable notifications */
    m_pMaster->m_pBusTopology->SetPortOperationsEnabled(EC_FALSE);
    m_pMaster->IncFrameResponseNotifyMaskedCnt();
    m_pMaster->IncNotifySBStatusMaskedCnt();
    m_pMaster->IncNotifySBMismatchMaskedCnt();

    /* start timeout if needed */
    if (EC_WAITINFINITE != dwTimeout)
    {
        m_Timeout.Start(dwTimeout);
    }
    /* check every connected port for causing frame loss */
    {
    EC_T_BOOL bContinue    = EC_TRUE;
    EC_T_WORD wAutoIncAddr = 0;
    EC_T_WORD wPort        = ESC_PORT_A;

        for (; bContinue && !m_Timeout.IsElapsed(); bContinue = GetNextPortToCheck(&wAutoIncAddr, &wPort))
        {
            dwRes = CheckPortForFrameLoss(wAutoIncAddr, wPort);
            switch (dwRes)
            {
            case EC_E_NOERROR:
                break;
            case EC_E_FRAMELOSS_AFTER_SLAVE:
            {
                EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION FramelossAfterSlaveLocation;
                OsMemset(&FramelossAfterSlaveLocation, 0, sizeof(EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION));

                FramelossAfterSlaveLocation.wAutoIncAddr = wAutoIncAddr;
                FramelossAfterSlaveLocation.wPort = wPort;
                FramelossAfterSlaveLocationsFifo.Add(FramelossAfterSlaveLocation);
            } break;
            default:
                dwRetVal = dwRes;
                goto Exit;
                /* no break */
            }
            /* terminate on permanent frame loss */
            if (0 == m_pInterface->GetNumConnectedSlaves())
            {
                if (m_pInterface->m_oAtEcatDesc.poEcDevice->IsLinkConnected())
                {
                    dwRetVal = EC_E_FRAME_LOST;
                    goto Exit;
                }
                else
                {
                    dwRetVal = EC_E_LINK_DISCONNECTED;
                    goto Exit;
                }
            }
        }
        /* check for timeout */
        if (!bContinue && m_Timeout.IsElapsed())
        {
            dwRetVal = EC_E_TIMEOUT;
            goto Exit;
        }
    }
    /* done */
    dwRetVal = ((0 == FramelossAfterSlaveLocationsFifo.GetCount()) ? EC_E_NOERROR : EC_E_FRAMELOSS_AFTER_SLAVE);

Exit:
    /* store frame loss information in bus slave and notifiy application */
    {
        EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION FramelossAfterSlaveLocation;
        OsMemset(&FramelossAfterSlaveLocation, 0, sizeof(EC_T_FRAMELOSS_AFTER_SLAVE_LOCATION));

        while (FramelossAfterSlaveLocationsFifo.RemoveNoLock(FramelossAfterSlaveLocation))
        {
        CEcBusSlave* pBusSlave = m_pMaster->m_pBusTopology->GetBusSlaveByAutoIncAddress(FramelossAfterSlaveLocation.wAutoIncAddr);

            if (EC_NULL != pBusSlave)
            {
                pBusSlave->SetFramelossAfterSlave(FramelossAfterSlaveLocation.wPort, EC_TRUE);
                m_pMaster->NotifyFramelossAfterSlave(pBusSlave, FramelossAfterSlaveLocation.wPort);
            }
        }
    }
    /* (re)enable notifications */
    m_pMaster->DecFrameResponseNotifyMaskedCnt();
    m_pMaster->DecNotifySBMismatchMaskedCnt();
    m_pMaster->DecNotifySBStatusMaskedCnt();
    m_pMaster->m_pBusTopology->SetPortOperationsEnabled(EC_TRUE);
    m_pMaster->NotifyTopologyChangeDone(dwRetVal);

    /* resynchronize bus slaves and state machines */
    m_pMaster->m_pBusTopology->SetTopologyChanged();
    while (m_pMaster->m_pBusTopology->TopologyChangedDetected() && !m_Timeout.IsElapsed())
    {
        OsSleep(10);
    }
    if (!m_pMaster->m_pBusTopology->TopologyChangedDetected() && m_Timeout.IsElapsed())
    {
        dwRetVal = EC_E_TIMEOUT;
    }
    if (m_pMaster->m_MasterConfig.dwLogLevel >= EC_LOG_LEVEL_VERBOSE)
    {
        OsDbgMsg("RescueScan: %s (0x%x)\n", EcErrorText(dwRetVal), dwRetVal);
    }
    return dwRetVal;
}
#endif /* INCLUDE_RESCUE_SCAN */

/*-END OF SOURCE FILE--------------------------------------------------------*/

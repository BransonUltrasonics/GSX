/*-----------------------------------------------------------------------------
 * EcDeviceFactory.cpp      cpp file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#include <EcInterfaceCommon.h>
#include <EthernetServices.h>
#include <EcServices.h>
#include <EcLink.h>
#include <EcInterface.h>
#include <EcMasterCommon.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcEthSwitch.h>
#include <EcDeviceFactory.h>
#include <EcObjDef.h>

#include <EcMaster.h>
#include <EcSlave.h>
#if (defined INCLUDE_HOTCONNECT)
#include <EcHotConnect.h>
#endif

#include <EcBusTopology.h>
#if (defined INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif

#if (defined INCLUDE_XPAT)
#include <EcConfigXpat.h>
#endif


/*- DEFINES ------------------------------------------------------------------*/
#define MAX_FMMU_FRAME_LENGTH ETHERNET_MAX_FRAME_LEN

#if (defined EVAL_VERSION)
static EC_T_BOOL S_bMsg1Shown = EC_FALSE;
#endif

/*-CEcDeviceFactory-----------------------------------------------------------*/
/******************************************************************************/
/** \brief Constructor
*
* \return
*/
CEcDeviceFactory::CEcDeviceFactory(CEcConfigData* poEcConfigData)
{
    m_poEcConfigData = poEcConfigData;
    m_bConfigModified = EC_FALSE;
}

/******************************************************************************/
/** \brief Destructor
*
* \return
*/
CEcDeviceFactory::~CEcDeviceFactory()
{
}


EC_T_MEMORY_LOGGER* CEcDeviceFactory::GetMemLog()
{ 
    return m_poEcConfigData->GetMemLog(); 
}

#if (defined INCLUDE_OEM)
#define CP_ID_TO_OEM_KEY                  0xAB2175B
#define CP_ID_TO_MANUFACTURER_SIGNATURE   0xAB21761
#define CP_ID_TO_ENI_KEY                  0xAB21775
#endif
/******************************************************************************/
/** \brief Creates an EtherCAT device
*
* \return status code
*/
EC_T_DWORD CEcDeviceFactory::CreateDevice
(   struct _EC_T_DEVICE_FACTORY_PARMS* pParms, 
    CEcDevice**         ppEcDevice
)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    CEcDevice*  pDevice  = EC_NULL;
    CEcMaster*  pMaster  = EC_NULL;
    EC_DEVICE_CTOR_PARMS oDevParms;

    /* check parameters */
    if (EC_NULL == pParms || EC_NULL == ppEcDevice || EC_NULL == pParms->pMasterConfig)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    /* check config data */
    if (EC_NULL == m_poEcConfigData)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* create link layer device */
    oDevParms.pMasterConfig = pParms->pMasterConfig;
    oDevParms.pMLog = pParms->pMLog;
    if (EC_NULL == pParms->pInitMasterParms->pLinkParms)
    {
        pDevice = EC_NEW(CEcDeviceStub(&oDevParms));
    }
    else
    {
        pDevice = EC_NEW(CEcDevice(&oDevParms));
    }
    if (EC_NULL == pDevice)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::CreateDevice", pDevice, sizeof(CEcDevice));
#if (defined INCLUDE_RED_DEVICE)
    pDevice->m_bRedConfigured = pParms->pMasterConfigEx->bUseRedundancy;
#endif

    /* parse eni file */
    if (!pParms->bCreateTemporary &&
        (  eCnfType_Filename == pParms->pConfigParms->eCnfType
        || eCnfType_Data == pParms->pConfigParms->eCnfType
        || eCnfType_Datadiag == pParms->pConfigParms->eCnfType)
        )
    {
#if(defined INCLUDE_XPAT)
        /* create xml parser */
        CEcConfigXpat* poEcConfigXpat = EC_NEW(CEcConfigXpat(m_poEcConfigData));
        if (EC_NULL == poEcConfigXpat)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::poEcConfigXpat", poEcConfigXpat, sizeof(CEcConfigXpat));
        /* parse eni file and fill config data */
        dwRetVal = poEcConfigXpat->Init(pParms->pConfigParms);
        if (poEcConfigXpat->IsErrorHit())
        {
            dwRetVal = poEcConfigXpat->LastError();
        }
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::~poEcConfigXpat", poEcConfigXpat, sizeof(CEcConfigXpat));
        SafeDelete(poEcConfigXpat);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
#else
        EC_UNREFPARM(pParms);
        EC_ERRORMSG(("CEcDeviceFactory::CreateDevice(): Eni file parsing is not supported!\n"));
        dwRetVal = EC_E_NOTSUPPORTED;
        goto Exit;
#endif
    }

#if (defined INCLUDE_OEM)
    {
        EC_T_DWORD dwCpId = (EC_T_DWORD)((pParms->pFeatures->qwOemKey / CP_ID_TO_OEM_KEY) & 0xFFFFFFFF);
        EC_T_DWORD dwSetSignature = (EC_T_DWORD)((dwCpId * CP_ID_TO_MANUFACTURER_SIGNATURE) & 0xFFFFFFFF);
        EC_T_DWORD dwActSignature = m_poEcConfigData->GetCfgMasterDesc()->dwManufacturerSignature;

        /* encrypted ENI needs OEM key */
        if ((0 == pParms->pFeatures->qwOemKey) && (m_poEcConfigData->GetCfgMasterDesc()->dwEncryptionVersion > 0))
        {
            dwRetVal = EC_E_ENI_ENCRYPTED;
            goto Exit;
        }

        /* validate signature */
        if (((0 != dwActSignature) && (dwSetSignature != dwActSignature))
            || (((EC_T_UINT64)dwActSignature == pParms->pFeatures->qwOemKey) && (0 != pParms->pFeatures->qwOemKey)))
        {
            dwRetVal = EC_E_OEM_SIGNATURE_MISMATCH;
            goto Exit;
        }
    }
#endif

#if (defined INCLUDE_VARREAD)
    if (pParms->pFeatures->bAddVarsForSpecificDataTypes)
    {
        dwRetVal = m_poEcConfigData->AddVarsForSpecificDataTypes();
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
    }
#endif    

    /* create master from the information stored in the tag <Config><Master>. */
    dwRetVal = DeviceFactoryCreateMaster( 
        pDevice, pParms->pfMbCallback, pParms->pfNotifyCallback, 
        pParms->pMasterConfig, pParms->pMasterConfigEx, 
        pParms->pFeatures, &pMaster, pParms->dwVersion 
                                        );
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }
    /* no errors */
#if (defined EVAL_VERSION)
    if (!pParms->bCreateTemporary && pMaster->GetLicenseKey() == 0)
    {
        if (!S_bMsg1Shown && !pParms->bCreateTemporary )
        {
            EC_DBGMSG("Protected version, stop sending ethernet frames after 60 minutes if not licensed!\n");
            S_bMsg1Shown = EC_TRUE;
        }
    }
#endif
    *ppEcDevice = pDevice;
        
    dwRetVal = EC_E_NOERROR;
    
Exit:
    /* cleanup */
    if (EC_E_NOERROR != dwRetVal)
    {
        if (dwRetVal == EC_E_PRODKEY_INVALID)
        {
            EC_DBGMSG ("\n\n*** invalid or missing product key! ***\n\n\n");
        }
        if (pDevice != EC_NULL )
        {   
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::DeleteDevice", pDevice, sizeof(CEcDevice));
            SafeDelete(pDevice);
        }
    }
    return dwRetVal;
}

EC_T_DWORD CEcDeviceFactory::ConfigExcludeSlave(CEcDevice* pEcDevice, EC_T_WORD wPhysAddress)
{
EC_T_DWORD dwRetVal  = EC_E_NOTFOUND;
EC_T_INT   nSlaveIdx = 0;

    EC_UNREFPARM(pEcDevice);

    m_bConfigModified = EC_TRUE;

    for ( nSlaveIdx = 0; nSlaveIdx < m_poEcConfigData->GetNumSlaves(); nSlaveIdx++ )
    {
        if ((0x0000 == wPhysAddress) || (wPhysAddress == m_poEcConfigData->GetSlaveFixedAddr(nSlaveIdx)))
        {
            m_poEcConfigData->SetSlaveExcludedFlag(nSlaveIdx, EC_TRUE);
            dwRetVal = EC_E_NOERROR;
        }
    }
    return dwRetVal;
}

EC_T_DWORD CEcDeviceFactory::ConfigIncludeSlave(CEcDevice* pEcDevice, EC_T_WORD wPhysAddress)
{
EC_T_DWORD dwRetVal  = EC_E_NOTFOUND;
EC_T_INT   nSlaveIdx = 0;

    EC_UNREFPARM(pEcDevice);

    m_bConfigModified = EC_TRUE;

    for ( nSlaveIdx = 0; nSlaveIdx < m_poEcConfigData->GetNumSlaves(); nSlaveIdx++ )
    {
        if ((0x0000 == wPhysAddress) || (wPhysAddress == m_poEcConfigData->GetSlaveFixedAddr(nSlaveIdx)))
        {
            m_poEcConfigData->SetSlaveExcludedFlag(nSlaveIdx, EC_FALSE);
            dwRetVal = EC_E_NOERROR;
        }
    }
    return dwRetVal;
}

EC_T_DWORD CEcDeviceFactory::ConfigSetPreviousPort(CEcDevice* pEcDevice, EC_T_WORD wPhysAddress, EC_T_WORD wPhysAddressPrev, EC_T_WORD wPortPrev)
{
EC_T_DWORD dwRetVal  = EC_E_NOTFOUND;
EC_T_INT   nSlaveIdx = 0;

    EC_UNREFPARM(pEcDevice);

    m_bConfigModified = EC_TRUE;

    for (nSlaveIdx = 0; nSlaveIdx < m_poEcConfigData->GetNumSlaves(); nSlaveIdx++)
    {
        if (wPhysAddress == m_poEcConfigData->GetSlaveFixedAddr(nSlaveIdx))
        {
            m_poEcConfigData->SetSlavePreviousPort(nSlaveIdx, wPhysAddressPrev, wPortPrev);
            dwRetVal = EC_E_NOERROR;
        }
    }
    return dwRetVal;
}

/******************************************************************************/
/** \brief Apply configuration on EtherCAT device
*
* \return status code
*/
EC_T_DWORD CEcDeviceFactory::ConfigApply(CEcDevice* pEcDevice, EC_T_BOOL bCreateTemporary)
{
EC_T_DWORD  dwRetVal     = EC_E_ERROR;
CEcMaster*  pMaster      = pEcDevice->GetMaster();
EC_T_DWORD  dwSlaveCnt   = 0;
EC_T_DWORD  dwSlaveIdx   = 0;
EC_T_DWORD  dwIn         = 0;
EC_T_DWORD  dwOut        = 0;
EC_T_DWORD  dwCopyOffset = 0;
EC_T_INT    nCopyOffset  = 0;
EC_T_CYC_CMD_WKC_DESC_ENTRY* aCycCmdWkcListTmp = EC_NULL;

    OsDbgAssert(pMaster != EC_NULL);

    if (!bCreateTemporary)
    {
#if (defined INCLUDE_MASTER_OBD)
        pMaster->SDOSetConfigCRC32ChkSum(m_poEcConfigData->GetConfigCheckSum());
#endif
#if (defined INCLUDE_VARREAD)
        /* create process variables info entries */
        /* must be called before DeviceFactoryCreateSlaves() */
        DeviceFactoryCreateProcessVarInfoEntries(pMaster);
#endif
        /* create config slaves */
        dwRetVal = DeviceFactoryCreateSlaves(pMaster);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }

        /* master properties (settings) */
        pMaster->m_dwMasterPropNumEntries = m_poEcConfigData->GetNumMasterProps();
        pMaster->m_aMasterPropEntries     = m_poEcConfigData->GetMasterPropArray();
        pMaster->m_dwMasterPropArraySize  = m_poEcConfigData->GetMasterPropArraySize();

        /* prepare offsets in BT instance for "old" compat. mode */
        pMaster->SetConfiguratorOffsets(
            m_poEcConfigData->m_dwLowestSlaveRecv,
            m_poEcConfigData->m_dwLowestOutOffset,
            m_poEcConfigData->m_dwLowestSlaveSend,
            m_poEcConfigData->m_dwLowestInOffset
                                   );
        /* calculate copy offset */
        if (m_poEcConfigData->m_dwLowestSlaveSend != (EC_T_DWORD)-1)
        {
            nCopyOffset = (m_poEcConfigData->m_dwLowestSlaveSend/8) - m_poEcConfigData->m_dwLowestOutOffset;
            if (m_poEcConfigData->m_dwLowestSlaveRecv != (EC_T_DWORD)-1)
            {
                nCopyOffset = EC_MIN(nCopyOffset, (EC_T_INT)((m_poEcConfigData->m_dwLowestSlaveRecv/8) - m_poEcConfigData->m_dwLowestInOffset));
            }
        }
        else if (m_poEcConfigData->m_dwLowestSlaveRecv != (EC_T_DWORD)-1)
        {
            nCopyOffset = (EC_T_INT)((m_poEcConfigData->m_dwLowestSlaveRecv/8) - m_poEcConfigData->m_dwLowestInOffset);
        }
        dwCopyOffset = EC_MAX(0, nCopyOffset);
        pMaster->SetConfiguratorOffsets(dwCopyOffset);
    }
    /* create process data images */
    if (pMaster->IsOnlyProcDataInImage())
    {
        dwIn  = m_poEcConfigData->m_dwHighestInByte;
        dwOut = m_poEcConfigData->m_dwHighestOutByte;
    }
    else
    {
         m_poEcConfigData->GetProcessImageParams(&dwIn, &dwOut);
    }
    dwRetVal = pEcDevice->CreateImage(dwIn, dwOut);
    if (EC_E_NOERROR != dwRetVal)
    {
        goto Exit;
    }

    if (!bCreateTemporary)
    {
#if (defined INCLUDE_WKCSTATE)
        /* create diagnosis image */
        dwRetVal = pMaster->CreateDiagnosisImage(m_poEcConfigData->GetNumCyclicCmds());
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
#endif
        /* create cyclic commands */
        dwRetVal = CreateCyclicCmds(pMaster);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }

        /* check cmds from ENI not exceeding image */
        dwRetVal = pMaster->IsAnyCmdExceedingImage();
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }

        /* set CycCmdWkcList at slaves for cyclic commands */
        aCycCmdWkcListTmp = (EC_T_CYC_CMD_WKC_DESC_ENTRY*)OsMalloc(m_poEcConfigData->GetNumCyclicCmds() * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        if (EC_NULL == aCycCmdWkcListTmp)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::ConfigApply() aCycCmdWkcListTmp", aCycCmdWkcListTmp, m_poEcConfigData->GetNumCyclicCmds() * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));

        dwSlaveCnt = pMaster->GetCfgSlaveCount();
        for (dwSlaveIdx = 0; dwSlaveIdx < dwSlaveCnt; dwSlaveIdx++)
        {
            OsMemset(aCycCmdWkcListTmp, 0, m_poEcConfigData->GetNumCyclicCmds() * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
            dwRetVal = DeviceFactoryCreateCycCmdWkcList(pMaster, pMaster->GetSlaveByIndex(dwSlaveIdx), aCycCmdWkcListTmp);
            if (EC_E_NOERROR != dwRetVal)
            {
                goto Exit;
            }
        }

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
        /* create HC groups */
        dwRetVal = DeviceFactoryInitHotConnect(pMaster);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
#endif
        /* create port list */
        pMaster->CreatePortList();

        /* create configured topology information */
        pMaster->SBCreateCfgTopology();
    }

    /* determine free fixed address ranges */
    {  
        EC_T_DWORD dwIdx = 0;

        for (dwIdx = 0; dwIdx < pMaster->SBFreeRanges(); dwIdx++)
        {
            ECX_T_ADDRRANGE* poRange = m_poEcConfigData->GetAddressRange(dwIdx);
            if (EC_NULL != poRange)
            {
                pMaster->SBSetFreeRange(dwIdx, poRange->wBegin, poRange->wEnd);
            }
            else
            {
                pMaster->SBSetFreeRange(dwIdx, 0, 0);
            }
        }
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    if (EC_NULL != aCycCmdWkcListTmp)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcDeviceFactory::ConfigApply() aCycCmdWkcListTmp", aCycCmdWkcListTmp, m_poEcConfigData->GetNumCyclicCmds() * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        SafeOsFree(aCycCmdWkcListTmp);
    }
    return dwRetVal;
}

/******************************************************************************/
/** \brief Remove a cyclic command entry from a cyclic frame 
*
* \return Removed cyclic command
*/
EcCycCmdConfigDesc* RemoveCycCmdEntry(EcCycFrameDesc* pCycFrame, EC_T_INT nCycCmdIdx)
{
    EC_T_INT nIdx = 0;
    EcCycCmdConfigDesc* pCycCmd = pCycFrame->apCycCmd[nCycCmdIdx];

    /* overwrite entry at position nCycCmdIdx with his successors */
    for (nIdx = nCycCmdIdx; nIdx < (pCycFrame->wCmdCnt - 1); nIdx++)
    {
        pCycFrame->apCycCmd[nIdx] = pCycFrame->apCycCmd[nIdx + 1];
    }
    pCycFrame->wCmdCnt--;
    pCycFrame->apCycCmd[pCycFrame->wCmdCnt] = EC_NULL; /* delete last entry because its doubled */
    

    return pCycCmd;
}
/******************************************************************************/
/** \brief Insert a cyclic command entry in a cyclic frame at a specific position
*
* \return EC_TRUE on success
*/
EC_T_BOOL InsertCycCmdEntry(EcCycFrameDesc* pCycFrame, EcCycCmdConfigDesc* pCycCmd, EC_T_INT nPos)
{
    EC_T_BOOL bOk = EC_TRUE;
    EC_T_INT nIdx = 0;

    OsDbgAssert(nPos >= 0);
    if (MAX_NUM_CMD_PER_FRAME < nPos || MAX_NUM_CMD_PER_FRAME < pCycFrame->wCmdCnt+1)
    {
        bOk = EC_FALSE;
        goto Exit;
    }

    /* move entries to create space for the new one */
    for (nIdx = pCycFrame->wCmdCnt; nIdx > nPos; nIdx--)
    {
        pCycFrame->apCycCmd[nIdx] = pCycFrame->apCycCmd[nIdx-1];
    }

    pCycFrame->apCycCmd[nPos] = pCycCmd;
    pCycFrame->wCmdCnt++;
Exit:
    return bOk;
}


/******************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD CEcDeviceFactory::CreateCyclicCmds(CEcMaster* pMaster)
{
#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL  bGotDCBrd      = EC_FALSE;
    EC_T_BOOL  bGotDCRecvTime = EC_FALSE;
#endif
    EC_T_DWORD dwRetVal       = EC_E_NOERROR;
    EC_T_INT   nNumCycEntries = 0;
    EC_T_INT   nCycEntryIndex = 0;
    EC_T_WORD  wHeaderLen = ETHERNET_88A4_FRAME_LEN;
#if (defined VLAN_FRAME_SUPPORT)
    if (pMaster->m_bVLANEnabled)
    {
        wHeaderLen += ETYPE_VLAN_HEADER_LEN;
    }
#endif
#if (defined INCLUDE_WKCSTATE)
    /* WkcStateDiagOffs will be incremented for each cyclic command created in pMaster->CreateCyclicCmds()  */
    /* start with 1 because EC_T_CFG_SLAVE_INFO doesn't return m_wWkcStateInEntries / m_wWkcStateOutEntries */
    /* unused wWkcStateDiagOffsIn / wWkcStateDiagOffsOut entries return 0                                   */
    EC_T_WORD  wWkcStateDiagOffs = 1;
#endif
#if (defined INCLUDE_VARREAD)
    EC_T_BOOL bSlaveStateProcVarCreated = EC_FALSE;
#endif
    
    nNumCycEntries = m_poEcConfigData->GetNumCyclicEntries();
    if (0 == nNumCycEntries)
    {
        EC_DBGMSG("CEcDeviceFactory::CreateCyclicCmds(): no cyclic entries found in XML configuration file!\n");
        dwRetVal = EC_E_XML_CYCCMDS_MISSING;
        goto Exit;
    }
    for (nCycEntryIndex = 0; (nCycEntryIndex < nNumCycEntries) && (dwRetVal == EC_E_NOERROR); nCycEntryIndex++)
    {
        EC_T_INT   nFrameIdx            = 0;
        EC_T_INT   nFrameLen            = 0;
        EC_T_INT   nFreeSpaceFirstFrame = ETHERNET_MAX_FRAME_LEN - wHeaderLen;

        EcCycCmdConfigDesc* pBrdCmd = EC_NULL;

        /* check frame count */
        if (0 == m_poEcConfigData->GetNumCyclicFrames(nCycEntryIndex))
        {
            EC_DBGMSG("CEcDeviceFactory::CreateCyclicCmds(): no cyclic frames found in XML configuration file!\n");
            dwRetVal = EC_E_XML_CYCCMDS_MISSING;
            goto Exit;
        }
        /* check how to manage BRD AL status command
        Goal: BRD AL status command is the first command in the first cyclic frame
        Case 1: BRD is first command in first frame --> no action
        Case 2: BRD is last command in first frame --> move to first command
        Case 3: No BRD found --> create command in first frame
                a) BRD fits into first frame
                b) BRD doesn't fit, because frame is full. --> Create an extra frame
        Case 4: BRD is last command in other frame --> move to first frame 
                a) BRD fits into first frame
                b) BRD doesn't fit, because frame is full. --> Create an extra frame */
        
        {
        EC_T_BOOL bAlStatusBrdFound    = EC_FALSE;
        EC_T_INT  nAlStatusBrdFrameIdx = 0;
        EC_T_INT  nAlStatusBrdCmdIdx = 0;

            /* process all frames */
            for (nFrameIdx = 0; (nFrameIdx < m_poEcConfigData->GetNumCyclicFrames(nCycEntryIndex)) && !bAlStatusBrdFound; nFrameIdx++)
            {
            EC_T_INT nNumCycCmdsInFrame = 0;
            EC_T_INT nCmdIdx            = 0;

                /* read commands */
                nNumCycCmdsInFrame = m_poEcConfigData->GetNumCyclicCmds(nCycEntryIndex, nFrameIdx);  
                for (nCmdIdx = 0; nCmdIdx < nNumCycCmdsInFrame; nCmdIdx++)
                {
                    dwRetVal = m_poEcConfigData->ReadCyclicCmd(&pBrdCmd, nCycEntryIndex, nFrameIdx, nCmdIdx);
                    if (dwRetVal != EC_E_NOERROR)
                    {
                        goto Exit;
                    }

                    /* calc free space in first frame */
                    if (nFrameIdx == 0)
                    {
                        nFreeSpaceFirstFrame -= ETYPE_EC_OVERHEAD;
                        nFreeSpaceFirstFrame -= EC_ICMDHDR_GET_LEN_LEN(&(pBrdCmd->EcCmdHeader));
                    }
                    /* look for BRD 0x130 */
                    if ((EC_CMD_TYPE_BRD == pBrdCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) && 
                        (ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(&(pBrdCmd->EcCmdHeader))))
                    {
                        bAlStatusBrdFound    = EC_TRUE;
                        nAlStatusBrdFrameIdx = nFrameIdx;
                        nAlStatusBrdCmdIdx = nCmdIdx;

                        break;
                    }
                }
            }/*for (nFrameIdx = 0; (nFrameIdx < nNumCycFrameCount) && !bAlStatusBrdFound; nFrameIdx++)*/

            /* for fixed layout it is not allowed to create command */
            if (!bAlStatusBrdFound && ((pMaster->m_eCycFrameLayout == eCycFrameLayout_FIXED) || (pMaster->m_eCycFrameLayout == eCycFrameLayout_IN_DMA)))
            {
                EC_DBGMSG("CEcDeviceFactory::CreateCyclicCmds(): missing AL_STATUS read for state\n");
                dwRetVal = EC_E_XML_ALSTATUS_READ_MISSING;
                goto Exit;
            }

            /* always process BRD first if not FIXED layout */
            if ((pMaster->m_eCycFrameLayout != eCycFrameLayout_FIXED) && (pMaster->m_eCycFrameLayout != eCycFrameLayout_IN_DMA))
            {
                EcCycFrameDesc* pCycFrame = EC_NULL;

                if (bAlStatusBrdFound && (nAlStatusBrdFrameIdx == 0))   /* BRD in first frame? */
                {
                    /* Case 1: BRD is first command --> no action */
                    if (nAlStatusBrdCmdIdx != 0)
                    {
                        /* Case 2: BRD is not first command --> move to first command */
                        pCycFrame = m_poEcConfigData->GetCyclicFrame(nCycEntryIndex, nAlStatusBrdFrameIdx);
                        RemoveCycCmdEntry(pCycFrame, nAlStatusBrdCmdIdx);
                        InsertCycCmdEntry(pCycFrame, pBrdCmd, 0);
                    }
                }
                else
                {
                    /* no BRD in first frame */
                    /* Case 3: No BRD found --> create command */
                    if (!bAlStatusBrdFound)
                    {
                        /* create new BRD command */
                        EC_T_BOOL bOk = EC_TRUE;
                        EC_T_CYCLIC_CMD_CFG_DESC oBrdCmdDesc;
                        OsMemset(&oBrdCmdDesc, 0, sizeof(EC_T_CYCLIC_CMD_CFG_DESC));
                        
                        oBrdCmdDesc.oEcCycCmdDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BRD;
                        EC_ICMDHDR_SET_ADDR_ADP(&(oBrdCmdDesc.oEcCycCmdDesc.EcCmdHeader), 0);    /* Broadcast */
                        oBrdCmdDesc.bAdpValid = EC_TRUE;
                        EC_ICMDHDR_SET_ADDR_ADO(&(oBrdCmdDesc.oEcCycCmdDesc.EcCmdHeader), ECREG_AL_STATUS);
                        oBrdCmdDesc.bAdoValid = EC_TRUE;
                        EC_ICMDHDR_SET_LEN_LEN(&(oBrdCmdDesc.oEcCycCmdDesc.EcCmdHeader), sizeof(EC_T_WORD));
                        oBrdCmdDesc.oEcCycCmdDesc.wConfOpStatesMask = (DEVICE_STATE_INIT | DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP);
                        oBrdCmdDesc.oEcCycCmdDesc.bCheckWkc = EC_FALSE;
                        oBrdCmdDesc.oEcCycCmdDesc.wConfiguredWkc = (EC_T_WORD)pMaster->GetCfgSlaveCount();
                        oBrdCmdDesc.oEcCycCmdDesc.wExpectedWkc = (EC_T_WORD)pMaster->GetCfgSlaveCount();
                        oBrdCmdDesc.bInputOffsValid = EC_TRUE;
                        
                        
                        bOk = m_poEcConfigData->CreateCyclicFrameCmd(EC_NULL, &oBrdCmdDesc, &pBrdCmd);
                        if (!bOk)
                        {
                            dwRetVal = EC_E_NOMEMORY;
                            goto Exit;
                        }
                        pBrdCmd->bCopyInputs = EC_FALSE;
                        pBrdCmd->dwInImageSize = 0;
                        pBrdCmd->dwInOffset = 0;
                    }
                    else
                    {
                        /* Remove existing BRD from frame */
                        pCycFrame = m_poEcConfigData->GetCyclicFrame(nCycEntryIndex, nAlStatusBrdFrameIdx);
                        pBrdCmd = RemoveCycCmdEntry(pCycFrame, nAlStatusBrdCmdIdx);
                    }
                    
                    /* Case 3: No BRD found --> create command in first frame &
                       Case 4: BRD is last command in other frame --> move to first frame */
                    if (nFreeSpaceFirstFrame < (EC_T_INT)(ETYPE_EC_OVERHEAD + sizeof(EC_T_WORD)))
                    {
                        /* b) BRD doesn't fit, because frame is full. --> Create an extra frame as first frame */
                        dwRetVal = m_poEcConfigData->CreateCyclicFrame(m_poEcConfigData->GetCyclicEntry(nCycEntryIndex), &pCycFrame, 0);
                        if (dwRetVal != EC_E_NOERROR)
                        {
                            goto Exit;
                        }

#if (defined INCLUDE_HOTCONNECT)
                        if (ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(&(pBrdCmd->EcCmdHeader)))
                        {
                            pBrdCmd->wConfOpStatesMask |= DEVICE_STATE_INIT;
                        }
#endif
                        pCycFrame->apCycCmd[pCycFrame->wCmdCnt++] = pBrdCmd;
                    }
                    else
                    {
                        /* a) BRD fits into first frame -> move to first frame */
                        pCycFrame = m_poEcConfigData->GetCyclicFrame(nCycEntryIndex, 0);
                        InsertCycCmdEntry(pCycFrame, pBrdCmd, 0);
                    }
                }
            }
#if (defined INCLUDE_VARREAD)
            if (!bSlaveStateProcVarCreated)
            {
                EC_T_DWORD dwRes = EC_E_NOERROR;
                EC_T_PD_VAR_INFO_INTERN oProcessVar;
                EC_T_PD_VAR_INFO_INTERN* pProcessVar = EC_NULL;
                OsMemset(&oProcessVar, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                /* create variable containing slaves state */
                OsSnprintf(oProcessVar.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "Inputs.DevicesState");
                oProcessVar.wFixedAddr = 0;
                oProcessVar.nBitOffs = 8 * (pBrdCmd->dwInOffset + pMaster->GetCopyOffset());
                oProcessVar.nBitSize = 8 * (pBrdCmd->dwInImageSize);
                oProcessVar.wDataType = DEFTYPE_UNSIGNED16;
                dwRes = m_poEcConfigData->CreateProcessVarInfoEntry(&oProcessVar, EC_TRUE, &pProcessVar);
                if (EC_E_NOERROR != dwRes)
                {
                    EC_ERRORMSG(("EcDeviceFactory::CreateCyclicCmds() - no memory for a process image inputs var. entry\n"));
                    dwRetVal = EC_E_NOMEMORY;
                    goto Exit;
                }
                pMaster->AddProcessVarInfoEntry(pProcessVar, pProcessVar->bIsInputData);

                bSlaveStateProcVarCreated = EC_TRUE;
            }
#endif /* INCLUDE_VARREAD */
        }

        /* create descriptor */
        EcCycCmdConfig oCycCmdConfig;
        m_poEcConfigData->GetCyclicEntryParameters(nCycEntryIndex, &oCycCmdConfig.dwCycleTime, &oCycCmdConfig.dwPriority, &oCycCmdConfig.dwTaskId);
        m_poEcConfigData->GetMacAddrDestination(&oCycCmdConfig.macTarget);
        oCycCmdConfig.wNumFrames = 0;
        oCycCmdConfig.wTotalNumCmds = 0;

        /* process cyclic frames */
        for (nFrameIdx = 0; nFrameIdx < m_poEcConfigData->GetNumCyclicFrames(nCycEntryIndex); nFrameIdx++)
        {
            EC_T_INT nCmdIdx = 0;
            /* insert cyclic frame */
            oCycCmdConfig.apCycFrameDesc[oCycCmdConfig.wNumFrames++] = m_poEcConfigData->GetCyclicFrame(nCycEntryIndex, nFrameIdx);

            nFrameLen = MAX_FMMU_FRAME_LENGTH;
            if (nFrameLen < (EC_T_INT)ETHERNET_88A4_FRAME_LEN)
            {
                dwRetVal = EC_E_XML_CYCCMDS_SIZEMISMATCH;
                goto Exit;
            }
            nFrameLen -= ETHERNET_88A4_FRAME_LEN;

            /* process commands */
            for (nCmdIdx = 0; nCmdIdx < m_poEcConfigData->GetNumCyclicCmds(nCycEntryIndex, nFrameIdx); nCmdIdx++)
            {
                EcCycCmdConfigDesc* pCurCmd = EC_NULL;

                if (nFrameLen < (EC_T_INT)ETYPE_EC_OVERHEAD)
                {
                    dwRetVal = EC_E_XML_CYCCMDS_SIZEMISMATCH;
                    goto Exit;
                }                
                nFrameLen -= ETYPE_EC_OVERHEAD;

                dwRetVal = m_poEcConfigData->ReadCyclicCmd(&pCurCmd, nCycEntryIndex, nFrameIdx, nCmdIdx);
                if (EC_E_NOERROR != dwRetVal)
                {
                    goto Exit;
                }            

                pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byIdx = 0;        /* IDX values are determined in CEcMaster::CreateCyclicCmds() */
                EC_ICMDHDR_SET_LEN_NEXT(&(pCurCmd->EcCmdHeader), EC_TRUE);

                /* mailbox cmd */
                if ((pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd == EC_CMD_TYPE_LRD) && 
                    (EC_ICMDHDR_GET_ADDR(&(pCurCmd->EcCmdHeader)) >= pMaster->GetLogAddressMBoxStates()) && 
                    (EC_ICMDHDR_GET_ADDR(&(pCurCmd->EcCmdHeader)) <  (pMaster->GetLogAddressMBoxStates() + (pMaster->GetSizeOfAddressMBoxStates()+7)/8)))
                {
                    pCurCmd->bIsLogicalMBoxStateCmd = EC_TRUE;
                }
                else
                {
                    pCurCmd->bIsLogicalMBoxStateCmd = EC_FALSE;
                }
#if (defined INCLUDE_DC_SUPPORT)
                /* check for command accessing Receive Time Port0 */
                if ((EC_CMD_TYPE_NOP == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) && 
                    (ECM_DCS_REC_TIMEPORT0 == EC_ICMDHDR_GET_ADDR_ADO(&(pCurCmd->EcCmdHeader))))
                {
                    bGotDCRecvTime = EC_TRUE;
                }

                if (((EC_CMD_TYPE_ARMW == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) || (EC_CMD_TYPE_FRMW == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)) 
                    && (ECREG_SYSTEMTIME == EC_ICMDHDR_GET_ADDR_ADO(&(pCurCmd->EcCmdHeader)))
                  )
                {
                    /* DC is enabled in configuration */
                    pMaster->SetDcSupportEnabled(EC_TRUE, 0);

                    /* some configurators don't set "ReferenceClock". Identify the DC reference clock slave here */
                    if (EC_NULL == pMaster->m_pDistributedClks->m_pCfgSlaveRefClock)
                    {
                        if ((EC_CMD_TYPE_ARMW == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd))
                        {
                            pMaster->m_pDistributedClks->SetRefClockAutoIncAddr(EC_ICMDHDR_GET_ADDR_ADP(&(pCurCmd->EcCmdHeader)));
                        }
                        else if ((EC_CMD_TYPE_FRMW == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd))
                        {
                            pMaster->m_pDistributedClks->SetRefClockFixedAddr(EC_ICMDHDR_GET_ADDR_ADP(&(pCurCmd->EcCmdHeader)));
                        }
                    }
#if (defined INCLUDE_VARREAD)
                    EC_T_DWORD dwRes = EC_E_NOERROR;
                    EC_T_PD_VAR_INFO_INTERN oProcessVar;
                    EC_T_PD_VAR_INFO_INTERN* pProcessVar = EC_NULL;
                    OsMemset(&oProcessVar, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    /* create variable containing bus time */
                    OsSnprintf(oProcessVar.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "Inputs.BusTime");
                    oProcessVar.wFixedAddr = 0;
                    oProcessVar.nBitOffs = 8 * (pCurCmd->dwInOffset + pMaster->GetCopyOffset());
                    oProcessVar.nBitSize = 8 * (pCurCmd->dwInImageSize);
                    oProcessVar.wDataType = (pCurCmd->dwInImageSize == 4 ? DEFTYPE_UNSIGNED32 : DEFTYPE_UNSIGNED64);

                    dwRes = m_poEcConfigData->CreateProcessVarInfoEntry(&oProcessVar, EC_TRUE, &pProcessVar);
                    if (EC_E_NOERROR != dwRes)
                    {
                        EC_ERRORMSG(("EcDeviceFactory::CreateCyclicCmds() - no memory for a process input var. entry\n"));
                        dwRetVal = dwRes;
                        goto Exit;
                    }
                    pMaster->AddProcessVarInfoEntry(pProcessVar, pProcessVar->bIsInputData);                    
#endif /* INCLUDE_VARREAD */
                }
                /* check if "Sync Window Monitoring" is active in cyclic frame */
                if ((EC_CMD_TYPE_BRD == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) && 
                    (ECREG_SYSTIME_DIFF == EC_ICMDHDR_GET_ADDR_ADO(&(pCurCmd->EcCmdHeader))))
                {
                    bGotDCBrd = EC_TRUE;
                    if (pMaster->GetAddBrdSyncWindowMonitoring())
                    {
                        /* force state to be compatible with IOCTL */
                        pCurCmd->wConfOpStatesMask = (DEVICE_STATE_PREOP|DEVICE_STATE_SAFEOP|DEVICE_STATE_OP);
                    }
                }
                /* datagram to shift the reference clock? */
                if ((EC_CMD_TYPE_APWR == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) && 
                    (ECM_DCS_SYSTEMTIME == EC_ICMDHDR_GET_ADDR_ADO(&(pCurCmd->EcCmdHeader))))
                {
                    pMaster->m_pDistributedClks->DcmSetBusShiftConfigured();
                }
#if (defined INCLUDE_DCX)
                /* determine external clock slave with NOP cmd created from EC-Engineer */
                if ((EC_CMD_TYPE_NOP == pCurCmd->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd) &&
                    (0x950 == EC_ICMDHDR_GET_ADDR_ADO(&(pCurCmd->EcCmdHeader))))
                {
                    EC_T_WORD wAutoIncAddr = EC_ICMDHDR_GET_ADDR_ADP(&(pCurCmd->EcCmdHeader));
                    pMaster->m_pDistributedClks->DcxSetExtClock(EC_FALSE, wAutoIncAddr);
                }
#endif /*INCLUDE_DCX*/
#endif /* INCLUDE_DC_SUPPORT */
#if (defined INCLUDE_HOTCONNECT)
                if (ECREG_AL_STATUS == EC_ICMDHDR_GET_ADDR_ADO(&(pCurCmd->EcCmdHeader)))
                {
                    pCurCmd->wConfOpStatesMask |= DEVICE_STATE_INIT;
                }
#endif /* INCLUDE_HOTCONNECT */
                /* cyclic command MUST be related to one of the operational states */
                OsDbgAssert (pCurCmd->wConfOpStatesMask & (DEVICE_STATE_INIT|DEVICE_STATE_PREOP|DEVICE_STATE_SAFEOP|DEVICE_STATE_OP));

                /* count command */
                oCycCmdConfig.wTotalNumCmds++;

                /* monitor frame length */
                if (nFrameLen < EC_ICMDHDR_GET_LEN_LEN(&(pCurCmd->EcCmdHeader)))
                {
                    /* Datagram oversized! */
                    dwRetVal = EC_E_XML_CYCCMDS_SIZEMISMATCH;
                    goto Exit;
                }
                nFrameLen -= EC_ICMDHDR_GET_LEN_LEN(&(pCurCmd->EcCmdHeader));
            } /* for ( nCmdIdx = 0; nCmdIdx < nNumCycCmdCountInFrm; nCmdIdx++ ) */
        } /* for ( nFrmIdx = 0; nFrmIdx < nNumCycFrameCount; nFrmIdx++ ) */

#if (defined INCLUDE_DC_SUPPORT)
        if (pMaster->GetDcSupportEnabled() && !bGotDCRecvTime)
        {
            EC_DBGMSG("CEcDeviceFactory::CreateCyclicCmds(): DC is active but there is no command accessing to Receive Time Port0!\n");
            dwRetVal = EC_E_XML_DC_CYCCMDS_MISSING;
            goto Exit;
        }
        /* insert DC window monitoring in last frame if needed */
        if (((pMaster->m_eCycFrameLayout == eCycFrameLayout_STANDARD) || (pMaster->m_eCycFrameLayout == eCycFrameLayout_DYNAMIC))
            && pMaster->GetDcSupportEnabled()
            && !bGotDCBrd
            && pMaster->GetAddBrdSyncWindowMonitoring())
        {
            EC_T_BOOL bOk = EC_TRUE;
            EcCycFrameDesc* pFrameDesc = EC_NULL;
            EcCycCmdConfigDesc* pCycCmdDC = EC_NULL;
            EC_T_CYCLIC_CMD_CFG_DESC oCycCmdDC;
            OsMemset(&oCycCmdDC, 0, sizeof(EC_T_CYCLIC_CMD_CFG_DESC));

            EC_ICMDHDR_SET_LEN_NEXT(&(oCycCmdDC.oEcCycCmdDesc.EcCmdHeader), EC_TRUE);
            oCycCmdDC.oEcCycCmdDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_BRD;
            EC_ICMDHDR_SET_ADDR_ADP(&(oCycCmdDC.oEcCycCmdDesc.EcCmdHeader), 0);        /* Broadcast */
            oCycCmdDC.bAdpValid = EC_TRUE;
            EC_ICMDHDR_SET_ADDR_ADO(&(oCycCmdDC.oEcCycCmdDesc.EcCmdHeader), ECREG_SYSTIME_DIFF);
            oCycCmdDC.bAdoValid = EC_TRUE;
            EC_ICMDHDR_SET_LEN_LEN(&(oCycCmdDC.oEcCycCmdDesc.EcCmdHeader), sizeof(EC_T_DWORD));
            oCycCmdDC.oEcCycCmdDesc.wConfOpStatesMask = (DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP);
            oCycCmdDC.oEcCycCmdDesc.bCheckWkc = EC_FALSE;
            oCycCmdDC.oEcCycCmdDesc.wConfiguredWkc = 0;
            oCycCmdDC.oEcCycCmdDesc.wExpectedWkc = 0;
            oCycCmdDC.bInputOffsValid = EC_TRUE; /* needs to be true to pass CreateCycFrameCmd */

            /* process data cmd */
            if (nFrameLen < EC_ICMDHDR_GET_LEN_LEN(&(oCycCmdDC.oEcCycCmdDesc.EcCmdHeader)))
            {
                dwRetVal = EC_E_XML_CYCCMDS_SIZEMISMATCH;
                goto Exit;
            }
            nFrameLen -= EC_ICMDHDR_GET_LEN_LEN(&(oCycCmdDC.oEcCycCmdDesc.EcCmdHeader));

            /* add command as last one in last frame */
            pFrameDesc = oCycCmdConfig.apCycFrameDesc[oCycCmdConfig.wNumFrames - 1];
            bOk = m_poEcConfigData->CreateCyclicFrameCmd(pFrameDesc, &oCycCmdDC, &pCycCmdDC);
            if (!bOk)
            {
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }
            pCycCmdDC->bCopyInputs = EC_FALSE;

            /* cyclic command MUST be related to one of the operational states */
            OsDbgAssert (pCycCmdDC->wConfOpStatesMask & (DEVICE_STATE_PREOP|DEVICE_STATE_SAFEOP|DEVICE_STATE_OP));
        
            oCycCmdConfig.wTotalNumCmds++;
        } /* bAddDCBrd */
#endif /* INCLUDE_DC_SUPPORT */

#if (defined INCLUDE_TRACE_DATA)
        /* Trace Data */
        if ((pMaster->m_wTraceDataSize > 0) && (nCycEntryIndex == 0))
        {
            OsDbgAssert(eCycFrameLayout_FIXED != pMaster->m_eCycFrameLayout);
            EC_T_BOOL bOk = EC_TRUE;
            EcCycFrameDesc* pFrameDesc = EC_NULL;
            EcCycCmdConfigDesc* pCycCmdTrace = EC_NULL;
            EC_T_CYCLIC_CMD_CFG_DESC oCycCmdTrace;
            OsMemset(&oCycCmdTrace, 0, sizeof(EC_T_CYCLIC_CMD_CFG_DESC));

            oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byIdx = 0;        /* IDX values are determined in CEcMaster::CreateCyclicCmds() */
            EC_ICMDHDR_SET_LEN_NEXT(&(oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader), EC_TRUE);
            oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_NOP;
            EC_ICMDHDR_SET_ADDR_ADP(&(oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader), 0);        /* Master */
            oCycCmdTrace.bAdpValid = EC_TRUE;
            EC_ICMDHDR_SET_ADDR_ADO(&(oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader), TRACE_DATA_CMD_ADO); /* 0x4154 */
            oCycCmdTrace.bAdoValid = EC_TRUE;
            EC_ICMDHDR_SET_LEN_LEN(&(oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader), pMaster->m_wTraceDataSize);
            oCycCmdTrace.oEcCycCmdDesc.wConfOpStatesMask = (DEVICE_STATE_PREOP|DEVICE_STATE_SAFEOP|DEVICE_STATE_OP);
            oCycCmdTrace.oEcCycCmdDesc.bCheckWkc = EC_FALSE;
            oCycCmdTrace.oEcCycCmdDesc.wConfiguredWkc = 0;
            oCycCmdTrace.oEcCycCmdDesc.wExpectedWkc = 0;
            oCycCmdTrace.dwOutputOffs = (EC_T_WORD)(pMaster->m_dwTraceDataOffset - pMaster->GetCopyOffset());
            oCycCmdTrace.bOutputOffsValid = EC_TRUE;          

            /* process data cmd */
            if (nFrameLen < EC_ICMDHDR_GET_LEN_LEN(&(oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader)))
            {
                dwRetVal = EC_E_XML_CYCCMDS_SIZEMISMATCH;
                goto Exit;
            }
            nFrameLen -= EC_ICMDHDR_GET_LEN_LEN(&(oCycCmdTrace.oEcCycCmdDesc.EcCmdHeader));
        
            /* add command as last one in last frame */
            pFrameDesc = oCycCmdConfig.apCycFrameDesc[oCycCmdConfig.wNumFrames - 1];
            bOk = m_poEcConfigData->CreateCyclicFrameCmd(pFrameDesc, &oCycCmdTrace, &pCycCmdTrace);
            if (!bOk)
            {
                dwRetVal = EC_E_NOMEMORY;
                goto Exit;
            }

            /* cyclic command MUST be related to one of the operational states */
            OsDbgAssert(pCycCmdTrace->wConfOpStatesMask & (DEVICE_STATE_PREOP | DEVICE_STATE_SAFEOP | DEVICE_STATE_OP));
            
            oCycCmdConfig.wTotalNumCmds++;

#if (defined INCLUDE_TRACE_DATA_VARINFO) && (defined INCLUDE_VARREAD)
            {
                /* create variable containing trace data */
                EC_T_DWORD dwRes = EC_E_NOERROR;
                EC_T_PD_VAR_INFO_INTERN oProcessVar;
                EC_T_PD_VAR_INFO_INTERN* pProcessVar = EC_NULL;
                OsMemset(&oProcessVar, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                
                OsSnprintf(oProcessVar.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "Outputs.TraceData");
                oProcessVar.wFixedAddr = 0;
                oProcessVar.nBitOffs = 8 * (pCycCmdTrace->dwOutOffset + pMaster->GetCopyOffset());
                oProcessVar.nBitSize = 8 * (pCycCmdTrace->dwOutImageSize);
                oProcessVar.wDataType = DEFTYPE_NULL;
                dwRes = m_poEcConfigData->CreateProcessVarInfoEntry(&oProcessVar, EC_FALSE, &pProcessVar);
                if (EC_E_NOERROR != dwRes)
                {
                    EC_ERRORMSG(("EcDeviceFactory::CreateCyclicCmds() - no memory for a process image outputs var. entry\n"));
                    dwRetVal = dwRes;
                    goto Exit;
                }
                pMaster->AddProcessVarInfoEntry(pProcessVar, pProcessVar->bIsInputData);
            }
#endif /* (defined INCLUDE_TRACE_DATA_VARINFO) && (defined INCLUDE_VARREAD) */
        }
#endif /* INCLUDE_TRACE_DATA */

#if (defined INCLUDE_VARREAD)
        /* swap information */
        oCycCmdConfig.wNumOfSwapInfos = m_poEcConfigData->GetNumSwapEntries();
        oCycCmdConfig.pSwapInfo = m_poEcConfigData->GetSwapInfo();
#endif /* INCLUDE_VARREAD */

        /* Copy information for Slave-to-Slave communication */
        oCycCmdConfig.wNumOfCopyInfos = m_poEcConfigData->GetNumCopyInfoEntries();
        oCycCmdConfig.pCopyInfo = m_poEcConfigData->GetCopyInfo();

        dwRetVal = pMaster->CreateCyclicCmds(nNumCycEntries, nCycEntryIndex, &oCycCmdConfig
#if (defined INCLUDE_WKCSTATE)
                                             ,&wWkcStateDiagOffs
#endif
                                            );
    } /* for (nCycEntryIndex = 0; nCycEntryIndex < nNumCycEntries; nCycEntryIndex++) */

Exit:
    OsDbgAssert(EC_E_ERROR != dwRetVal);
    return dwRetVal;
}


/******************************************************************************/
/** \brief 
*
* \return 
*/
EC_T_DWORD CEcDeviceFactory::DeviceFactoryCreateMaster
(   CEcDevice*              pEcDevice,          /* [in]  device instance */
    MBX_CALLBACK            pfMbCallback,       /* [in]  mailbox callback */
    NOTIFY_CALLBACK         pfNotifyCallback,   /* [in]  notification callback */
    struct _EC_T_MASTER_CONFIG* pMasterConfig,  /* [in]  master layer parameters */
    struct _EC_T_MASTER_CONFIG_EX* pMasterConfigEx, /* [in]  extended master layer parameters */
    struct _EC_T_INTFEATUREFIELD_DESC* pFeatures, /* [in]  preset feature list */
    CEcMaster**             ppCreMaster,        /* [out] Created Master Pointer */
    EC_T_DWORD              dwVersion           /* [in]  Application asked version */
)
{ 
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    PEcMasterDesc   pMasterDesc = EC_NULL;
    CEcMaster*      pEcMaster   = EC_NULL;
    
    if (EC_NULL == ppCreMaster)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pMasterDesc = m_poEcConfigData->GetCfgMasterDesc();

    /* read the MAC address of the EtherCAT master device */
    pEcDevice->SetMacAddress(pMasterDesc->oMacSrc);

#if (defined INCLUDE_EOE_SUPPORT)
    {
    EthernetCreateSwitch createSwitch;

        /* read master switch parameters */
        createSwitch.dwMaxFrames  = pMasterDesc->dwMaxFrames;
        createSwitch.dwMaxMACs    = pMasterDesc->dwMaxMACs;
        createSwitch.dwMaxPorts   = pMasterDesc->dwMaxPorts;
        createSwitch.bSwitchToNIC = EC_FALSE;

        createSwitch.dwMaxPorts += pMasterConfig->dwAdditionalEoEEndpoints;
        if (0 != createSwitch.dwMaxPorts )
        {
            pEcDevice->CreateSwitch(&createSwitch);
        }
    }
#endif /* INCLUDE_EOE_SUPPORT */

    pMasterDesc->wMaxNumSlaves = (EC_T_WORD)EC_MAX(1, (EC_T_WORD)m_poEcConfigData->GetNumSlaves());  /* at least one! */

#if (defined EVAL_VERSION)
    pMasterDesc->dwTimeLimit = 62 * 60; /* run 62 minutes */
#else
    pMasterDesc->dwTimeLimit = 0;
#endif

#if (defined INCLUDE_VARREAD) 
    /* Gets the number of input/output process variables */
    pMasterDesc->dwProcessVarInfoNumEntriesInp  = m_poEcConfigData->GetNumInpProcessVarInfo();
    pMasterDesc->dwProcessVarInfoNumEntriesOutp = m_poEcConfigData->GetNumOutpProcessVarInfo();
#endif /* INCLUDE_VARREAD */

    /* create EtherCAT master */
    pEcMaster = pEcDevice->DeviceCreateMaster(
        pMasterDesc, pfMbCallback, pfNotifyCallback, pMasterConfig, pMasterConfigEx, pFeatures, dwVersion);

    /* no errors */
    *ppCreMaster = pEcMaster;

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

#if (defined INCLUDE_VARREAD)
/***************************************************************************************************/
/**
\brief Creates the process variables info entries.
*/
EC_T_VOID CEcDeviceFactory::DeviceFactoryCreateProcessVarInfoEntries(CEcMaster* pMaster)
{
    EC_T_DWORD dwRetVal                       = EC_E_NOERROR;
    EC_T_DWORD dwProcessVarInfosIdx           = 0;
    EC_T_PD_VAR_INFO_INTERN* poProcessVarInfo = EC_NULL;
    EC_T_DWORD dwNumInpProcessVarInfos        = m_poEcConfigData->GetNumInpProcessVarInfo();
    EC_T_DWORD dwNumOutpProcessVarInfos       = m_poEcConfigData->GetNumOutpProcessVarInfo();

    /* Iterate through all INPUT variables */
    for (dwProcessVarInfosIdx = 0; dwProcessVarInfosIdx < dwNumInpProcessVarInfos; dwProcessVarInfosIdx++)
    {
        const EC_T_CHAR szProcessVarInfoInputToggle[] = ".WcState.InputToggle";
        const EC_T_CHAR szWcState[] = "WcState";

        dwRetVal = m_poEcConfigData->GetProcessVarInfoEntry(&poProcessVarInfo, dwProcessVarInfosIdx, EC_TRUE);
        if (EC_E_NOERROR != dwRetVal)
        {
            OsDbgAssert(EC_FALSE);
            continue;
        }

        /* Remove ".WcState.InputToggle" BIT-Variables from TwinCAT
           check if poProcessVarInfo->szName ends with ".WcState.InputToggle" */
        if ((OsStrlen(poProcessVarInfo->szName) >= OsStrlen(szProcessVarInfoInputToggle)) 
            && (0 == OsStrcmp(poProcessVarInfo->szName + OsStrlen(poProcessVarInfo->szName) - OsStrlen(szProcessVarInfoInputToggle), szProcessVarInfoInputToggle)))
            continue;

        /* Remove ...".WcState.WcState" ..."Frm0WcState"  and ..."Frm1WcState" Variables 
           check if poProcessVarInfo->szName ends with "WcState" */
        if ((OsStrlen(poProcessVarInfo->szName) >= OsStrlen(szWcState)) 
            && (0 == OsStrcmp(poProcessVarInfo->szName + OsStrlen(poProcessVarInfo->szName) - OsStrlen(szWcState), szWcState)))
        {
            continue;
        }

        pMaster->AddProcessVarInfoEntry(poProcessVarInfo, EC_TRUE);
    }
    
    /* Iterate through all OUTPUT variables */
    for (dwProcessVarInfosIdx = 0; dwProcessVarInfosIdx < dwNumOutpProcessVarInfos; dwProcessVarInfosIdx++)
    {
        /* Get the process variable by an index */
        dwRetVal = m_poEcConfigData->GetProcessVarInfoEntry(&poProcessVarInfo, dwProcessVarInfosIdx, EC_FALSE);
        if (EC_E_NOERROR != dwRetVal)
        {
            OsDbgAssert(EC_FALSE);
            continue;
        }
        pMaster->AddProcessVarInfoEntry(poProcessVarInfo, EC_FALSE);
    }
}
#endif

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
/***************************************************************************************************/
/**
\brief  Create HotConnect Groups.
*/
EC_T_DWORD CEcDeviceFactory::DeviceFactoryInitHotConnect( CEcMaster* pEcMaster )
{
    EC_T_DWORD              dwRes           = EC_E_ERROR;
    EC_T_DWORD              dwRetVal        = EC_E_ERROR;
    EC_T_DWORD              dwGroupIndex    = 0;
    EC_T_DWORD              dwIdx           = 0;
    EC_T_DWORD              dwHCGroupAmount = 0;
    EC_T_HOTCONNECT_GROUP*  pHcGroupCur     = EC_NULL;

    dwHCGroupAmount = m_poEcConfigData->GetNumHCGroups();    /* configured HotConnect Groups + 1st Mandatory Group */
        
    dwRes = pEcMaster->HCCreateGroups(dwHCGroupAmount);
    if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

    for (dwGroupIndex = 0; dwGroupIndex < (dwHCGroupAmount+1); dwGroupIndex++)
    {
        pHcGroupCur = pEcMaster->HCGetGroup(dwGroupIndex);

        dwRes = m_poEcConfigData->ReadHCGroup(pHcGroupCur, pEcMaster, dwGroupIndex);
        if (EC_E_NOERROR != dwRes) { dwRetVal = dwRes; goto Exit; }

        if (0 == dwGroupIndex)
        {
            pHcGroupCur->bGroupComplete = EC_TRUE;
            pHcGroupCur->bGroupPresent  = EC_TRUE;
            pHcGroupCur->bGroupActive   = EC_TRUE;
        }
        /* mark all groups as target state set because the first set */
        /* state is done by the master API and not HC state machine  */
        /* --see RefreshSlavesAndGroupsPresence()--                  */
        pHcGroupCur->bGroupTargStateSet = EC_TRUE;

#if (defined INCLUDE_HOTCONNECT)
        /* check for duplicate identify values, considering Superset-ENI */
        if (EC_NULL != pHcGroupCur->ppGroupMembers)
        {
            CEcSlave* pCfgSlaveCur = pHcGroupCur->ppGroupMembers[0];
            if (EC_NULL != pCfgSlaveCur)
            {                
                for (dwIdx = dwGroupIndex; dwIdx > 1; dwIdx--)
                {
                    EC_T_HOTCONNECT_GROUP* pHcGroupTmp = pEcMaster->HCGetGroup(dwIdx - 1);
                    if ((EC_NULL != pHcGroupTmp) && (EC_NULL != pHcGroupTmp->ppGroupMembers))
                    {
                        CEcSlave* pCfgSlaveTmp = pHcGroupTmp->ppGroupMembers[0];
                        if (EC_NULL != pCfgSlaveTmp)
                        {
                            EC_T_ENI_SLAVE* pEniSlaveCur = pCfgSlaveCur->m_pEniSlave;
                            EC_T_ENI_SLAVE* pEniSlaveTmp = pCfgSlaveTmp->m_pEniSlave;

                            if ((pEniSlaveCur->wIdentificationAdo == pEniSlaveTmp->wIdentificationAdo)
                                 && (pEniSlaveCur->wIdentificationValue == pEniSlaveTmp->wIdentificationValue))
                            {
                                if ((INVALID_FIXED_ADDR == pEniSlaveCur->wHCIdentifyAddrPrev) 
                                     || (pEniSlaveCur->wHCIdentifyAddrPrev == pEniSlaveTmp->wHCIdentifyAddrPrev))
                                {
                                    EC_ERRORMSGC(("CEcDeviceFactory: HC identifcation of Slave %d and Slave %d duplicated in ENI\n", pEniSlaveCur->wPhysAddr, pEniSlaveTmp->wPhysAddr));
                                    dwRetVal = EC_E_INVALIDCMD;
                                    goto Exit;
                                }
                            }
                        }
                    }
                }
            }
        }
#else
        EC_UNREFPARM(dwIdx);
#endif
    }
    /* recalculate WKCs if needed */
    if (m_bConfigModified || (0 < dwHCGroupAmount) || pEcMaster->GetAutoAdjustCycCmdWkc())
    {
        dwRes = pEcMaster->RecalculateHCWKC(EC_FALSE);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}
#endif /* INCLUDE_HOTCONNECT || INCLUDE_WKCSTATE */

#if (defined INCLUDE_OEM)
EC_T_VOID SimplePolyalphabeticCipher(EC_T_BYTE* pBuffer, EC_T_DWORD dwPattern, EC_T_INT nBufLen)
{
    volatile EC_T_DWORD dwCur = 0;
    EC_T_DWORD dwLastBlock = 0;

    /* XOR complete blocks at beginning */
    for (; nBufLen >= 4; pBuffer = pBuffer + 4, nBufLen = nBufLen - 4, dwPattern++)
    {
        dwCur = EC_GET_FRM_DWORD(pBuffer);
        dwCur = dwCur ^ dwPattern;
        EC_SET_FRM_DWORD(pBuffer, dwCur);
    }
    /* XOR incomplete block at ending */
    if (nBufLen > 0)
    {
        OsMemcpy((EC_T_BYTE*)&dwLastBlock, pBuffer, nBufLen);
        dwCur = EC_GET_FRM_DWORD(&dwLastBlock);
        dwCur = dwCur ^ dwPattern;
        EC_SET_FRM_DWORD(&dwLastBlock, dwCur);
        OsMemcpy(pBuffer, (EC_T_BYTE*)&dwLastBlock, nBufLen);
    }
}

EC_T_DWORD CEcDeviceFactory::DeviceFactoryDecryptCfgSlave(CEcMaster* pEcMaster, CEcSlave* pSlave)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    PEcInitCmdDesc pInitCmd = EC_NULL;
    EC_T_UINT64 qwOemKey = pEcMaster->GetOemKey();
    EC_T_DWORD dwCpId = (EC_T_DWORD)((qwOemKey / CP_ID_TO_OEM_KEY) & 0xFFFFFFFF);
    EC_T_DWORD dwEniKey = (EC_T_DWORD)((dwCpId * CP_ID_TO_ENI_KEY) & 0xFFFFFFFF);
    EC_T_DWORD dwInitCmdIdx = 0;

    switch (m_poEcConfigData->GetCfgMasterDesc()->dwEncryptionVersion)
    {
    /* not encrypted */
    case 0: break;

    /* decrypt only FMMU init cmd data from slave */
    case 1:
        {
            for (dwInitCmdIdx = 0; dwInitCmdIdx < pSlave->m_wInitCmdsCount; dwInitCmdIdx++)
            {
                pInitCmd = pSlave->m_ppInitCmds[dwInitCmdIdx];

                if ((EC_CMD_TYPE_FPWR == EC_ICMDHDR_GET_CMDIDX_CMD(&pInitCmd->EcCmdHeader))
                     && (EC_ICMDHDR_GET_ADDR_ADO(&pInitCmd->EcCmdHeader) >= ECREG_FMMU_CONFIG)
                     && (EC_ICMDHDR_GET_ADDR_ADO(&pInitCmd->EcCmdHeader) < ECREG_SYNCMANAGER_CONFIG)
                     && (ECAT_INITCMD_P_S == EC_ECINITCMDDESC_GET_TRANSITION(pInitCmd)))
                {
                    /* decrypt FMMU init cmd data */
                    EC_T_BYTE* pInitCmdData = EcInitCmdDescData(pInitCmd);
                    EC_T_WORD wInitCmdDataLen = EC_ICMDHDR_GET_LEN_LEN(&pInitCmd->EcCmdHeader);
                    EC_T_DWORD dwPattern = 0;

                    /* set pattern considering EC_T_DWORD overflow */
                    if (dwEniKey < (EC_T_DWORD)(0xffffffff - pSlave->GetFixedAddr()))
                    {
                        dwPattern = dwEniKey + pSlave->GetFixedAddr();
                    }
                    else
                    {
                        dwPattern = dwEniKey - pSlave->GetFixedAddr();
                    }

                    SimplePolyalphabeticCipher(pInitCmdData, dwPattern, wInitCmdDataLen);
                }
            }
        } break;

    /* ENI encryption algorithm version not supported */
    default:
        dwRetVal = EC_E_ENI_ENCRYPTION_WRONG_VERSION;
        goto Exit;
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /* INCLUDE_OEM */

EC_T_DWORD  CEcDeviceFactory::DeviceFactoryCreateCycCmdWkcList(CEcMaster* poEcMaster, CEcSlave* pCfgSlave, EC_T_CYC_CMD_WKC_DESC_ENTRY* aCycCmdWkcListTmp)
{
    EC_T_DWORD           dwRetVal = EC_E_ERROR;
    EC_T_INT             nNumCycCmds = 0;
    EC_T_DWORD           dwCycEntryIndex = 0;
    EC_T_WORD            wWkcIncrement = 0;
    EcCycCmdConfigDesc*  pCmdDesc = EC_NULL;

    EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::DeviceFactoryCreateCycCmdWkcList", pCfgSlave->m_aCycCmdWkcList, pCfgSlave->m_dwNumCycCmdWkc * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
    SafeOsFree(pCfgSlave->m_aCycCmdWkcList);
    pCfgSlave->m_dwNumCycCmdWkc = 0;

    /* fill aCycCmdWkcListTmp counting nNumCycCmds */
#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
    for (dwCycEntryIndex = 0; dwCycEntryIndex < poEcMaster->GetNumCycConfigEntries(); dwCycEntryIndex++)
#else
    dwCycEntryIndex = 0;
#endif
    {
        CYCLIC_ENTRY_MGMT_DESC* pCycEntry = poEcMaster->GetCycEntryDesc((EC_T_INT)dwCycEntryIndex);
        EC_T_WORD wCmdsInThisEntry = pCycEntry->wTotalNumCmds;

        for (EC_T_WORD wCmdIdx = 0; wCmdIdx < wCmdsInThisEntry; wCmdIdx++)
        {
            pCmdDesc = pCycEntry->apEcAllCycCmdConfigDesc[wCmdIdx];

            wWkcIncrement = m_poEcConfigData->GetCycCmdWkcIncrement(pCfgSlave, pCmdDesc);
            
            if (wWkcIncrement > 0)
            {
                aCycCmdWkcListTmp[nNumCycCmds].bActive = EC_TRUE;
                aCycCmdWkcListTmp[nNumCycCmds].pCycCmdCfgDesc = pCmdDesc;
                aCycCmdWkcListTmp[nNumCycCmds].wWkcIncrement = wWkcIncrement;
                nNumCycCmds++;
            }
        }
    }

    /* latch aCycCmdWkcListTmp to pCfgSlave */
    if (nNumCycCmds > 0)
    {
        pCfgSlave->m_aCycCmdWkcList = (EC_T_CYC_CMD_WKC_DESC_ENTRY*)OsMalloc(nNumCycCmds * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_FACTORY, "CEcDeviceFactory::DeviceFactoryCreateCycCmdWkcList", pCfgSlave->m_aCycCmdWkcList, nNumCycCmds * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        if (EC_NULL == pCfgSlave->m_aCycCmdWkcList)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemcpy(pCfgSlave->m_aCycCmdWkcList, aCycCmdWkcListTmp, nNumCycCmds * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
	    pCfgSlave->m_dwNumCycCmdWkc = (EC_T_DWORD)nNumCycCmds;
    }

    /* no errors */
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/******************************************************************************/
/** \brief 
*
* \return 
*/
EC_T_DWORD CEcDeviceFactory::DeviceFactoryCreateSlaves(CEcMaster* pEcMaster)
{
EC_T_DWORD dwRetVal       = EC_E_ERROR;
EC_T_DWORD dwRes          = EC_E_ERROR;
EC_T_WORD  wAutoIncAddr   = 0;               
EC_T_ENI_SLAVE* pEniSlave = EC_NULL;
CEcSlave* pSlave = EC_NULL;
EC_T_INT nSlaveIndex = 0;

    for (nSlaveIndex = 0; nSlaveIndex < m_poEcConfigData->GetNumSlaves(); nSlaveIndex++)
    {
        pEniSlave = m_poEcConfigData->GetEniSlave(nSlaveIndex);
        if (EC_NULL == pEniSlave)
        {
            continue;
        }
        if (0 == pEniSlave->wPhysAddr)
        {
            /* skip slaves with no physical addr like EK0000 (inserted by ET9000 between CCAT adapter and first E-Bus slave) or EL9010 or EL9011 */
            continue;
        }
        pEniSlave->wAutoIncAddr = wAutoIncAddr;    /* for FP Superset ENI in case of excluded slaves ! */

#if (defined INCLUDE_VARREAD)
        pEcMaster->GetSlaveProcVarInfoNumOf(EC_TRUE, pEniSlave->wPhysAddr, &(pEniSlave->wNumProcessVarsInp), 
            &(pEniSlave->wProcessVarsInpIndexFirst), &(pEniSlave->wProcessVarsInpIndexLast), EC_TRUE);
        pEcMaster->GetSlaveProcVarInfoNumOf(EC_TRUE, pEniSlave->wPhysAddr, &(pEniSlave->wNumProcessVarsOutp),
            &(pEniSlave->wProcessVarsOutIndexFirst), &(pEniSlave->wProcessVarsOutIndexLast), EC_FALSE);
#endif
        /* create EtherCAT slave */
        pSlave = pEcMaster->CreateSlave(pEniSlave);
        dwRes = pEcMaster->AddSlaveToList(pSlave);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }

#if (defined INCLUDE_OEM)
        /* decrypt ENI data for slave */
        dwRes = DeviceFactoryDecryptCfgSlave(pEcMaster, pSlave);
        if (EC_E_NOERROR != dwRes)
        {
            dwRetVal = dwRes;
            goto Exit;
        }
#else
        EC_UNREFPARM(pSlave);
#endif /* INCLUDE_OEM */

#if (defined INCLUDE_DC_SUPPORT)
        /* identify DC reference clock */
        if (pEniSlave->bDcReferenceClock)
        {
            if (EC_NULL == pEcMaster->m_pDistributedClks->m_pCfgSlaveRefClock)
            {
                pEcMaster->m_pDistributedClks->SetRefClockFixedAddr(pEniSlave->wPhysAddr);
            }
        }
#endif
        /* next auto increment address */
        wAutoIncAddr--;
    }

    /* sanity check: slaves at previous port entry must be available */
    {
    EC_T_DWORD dwSlaveIdx = 0;
    EC_T_DWORD dwSlaveCnt = pEcMaster->GetCfgSlaveCount();
    CEcSlave*  pCfgSlave  = EC_NULL;

        for (dwSlaveIdx = 0; dwSlaveIdx < dwSlaveCnt; dwSlaveIdx++)
        {
        EC_T_DWORD dwPrevPortIdx = 0;

            /* get config slave */
            pCfgSlave = pEcMaster->GetSlaveByIndex(dwSlaveIdx);
            if (EC_NULL == pCfgSlave)
            {
                OsDbgAssert(EC_FALSE);
                continue;
            }

            /* walk through each previous port entry */
            for (dwPrevPortIdx = 0; dwPrevPortIdx < pCfgSlave->GetNumPreviousPorts(); dwPrevPortIdx++)
            {
            EC_T_SLAVE_PORT_DESC* pPortCur = &(pCfgSlave->m_pPreviousPort[dwPrevPortIdx]);

                if ((0 != pPortCur->wSlaveAddress) && (0 == (pPortCur->wPortNumber & IGNORE_PREV_PORT_FLAG)))
                {
                CEcSlave* pCfgSlavePrev = pEcMaster->GetSlaveByCfgFixedAddr(pPortCur->wSlaveAddress);

                    if (EC_NULL == pCfgSlavePrev)
                    {
                        EC_ERRORMSGC(("Slave with EtherCAT address=%d referenced at slave with EtherCAT address=%d missing!\n", pPortCur->wSlaveAddress, pCfgSlave->GetCfgFixedAddr()));
                        dwRetVal = EC_E_WRONG_FORMAT;
                        goto Exit;
                    }
                }
            }
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/* CEcConfigData */
CEcConfigData::CEcConfigData(EC_T_DWORD dwInstanceID, struct _EC_T_MEMORY_LOGGER* pMLog)
: m_dwMasterInstanceID(dwInstanceID)
, m_pMLog(pMLog)
{
    
    InitializeConfigData();
}

/* Memory Managment */
#if(defined INCLUDE_MEM_POOL)
#define ConfigDataAllocMem(dwSize) OsConfigDataMemPoolAlloc(m_dwMasterInstanceID, (dwSize))
#else
#define ConfigDataAllocMem OsMalloc
#endif

#if(defined INCLUDE_MEM_POOL)
#define ConfigDataReallocMem(pvMem, dwSize) OsConfigDataMemPoolRealloc(m_dwMasterInstanceID, (pvMem), (dwSize))
#else
#define ConfigDataReallocMem OsRealloc
#endif

#if(defined INCLUDE_MEM_POOL)
#define ConfigDataFreeMem(pvMem) { OsConfigDataMemPoolFree(m_dwMasterInstanceID, (pvMem)); (pvMem) = EC_NULL; }
#else
#define ConfigDataFreeMem SafeOsFree
#endif

/********************************************************************************/
/** \brief Initialize members of CEcConfigData
*/
EC_T_VOID CEcConfigData::InitializeConfigData()
{
    CreateUninitializedString(m_strProductKey);
    m_dwProductKeyStringLen = 0;

    m_dwConfigCheckSum = 0;

    OsMemset(&m_oCfgMaster, 0, sizeof(EcMasterDesc));

    /* Info */
    OsMemset(m_oCfgMaster.oMacDest.b, 0xFF, 6);
    OsMemset(m_oCfgMaster.oMacSrc.b, 0xFF, 6);
    m_oCfgMaster.abyEthType[0] = 0xA4;
    m_oCfgMaster.abyEthType[1] = 0x88;

    /* EoE */
    m_oCfgMaster.bEoE = 0;
    m_oCfgMaster.dwMaxPorts = 0;
    m_oCfgMaster.dwMaxFrames = 0;
    m_oCfgMaster.dwMaxMACs = 0;

    m_dwNumMasterPropEntries = 0;
    m_dwMasterPropArraySize = 0;
    m_aMasterProps = EC_NULL;

    /* reset ranges and initialize range 0 to most possible amount of slaves (1-65535) it is then split by configuration into hugest areas */
    OsMemset(m_abHugeRanges, 0, sizeof(m_abHugeRanges));

    m_abHugeRanges[0].wBegin = 1;
    m_abHugeRanges[0].wEnd = ((EC_T_WORD)-1);

    m_dwNumCfgDataSlaves = 0;
    m_dwCfgDataSlaveArraySize = 0;
    m_apoCfgDataSlave = EC_NULL;

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    m_dwHCGroupArraySize = 0;
    m_dwNumHCGroups = 0;
    m_apoHotConnectGroup = EC_NULL;
#endif

    m_dwCfgCyclicArraySize = 0;
    m_dwNumCyclicEntries = 0;
    m_apoCfgCyclic = EC_NULL;

#if (defined INCLUDE_VARREAD)
    m_dwProcessInputVarInfoArraySize = 0;
    m_dwNumProcessInputVarInfoEntries = 0;
    m_apoProcessInputVarInfo = EC_NULL;

    m_dwProcessOutputVarInfoArraySize = 0;
    m_dwNumProcessOutputVarInfoEntries = 0;
    m_apoProcessOutputVarInfo = EC_NULL;

    m_wNumSwapInfosAllocated = 0;
    m_wNumSwapInfos = 0;
    m_pSwapInfo = EC_NULL;
#endif

    m_wNumCopyInfosAllocated = 0;
    m_wNumCopyInfos = 0;
    m_pCopyInfo = EC_NULL;

    m_dwInputByteSize = 32;
    m_dwOutputByteSize = 32;

    m_dwLowestSlaveSend = (EC_T_DWORD)-1;
    m_dwHighestSlaveSend = 0;
    m_dwLowestSlaveRecv = (EC_T_DWORD)-1;
    m_dwHighestSlaveRecv = 0;
    m_dwLowestOutOffset = (EC_T_DWORD)-1;
    m_dwHighestOutByte = 0;
    m_dwLowestInOffset = (EC_T_DWORD)-1;
    m_dwHighestInByte = 0;

#if (defined INCLUDE_DC_SUPPORT)
    m_bDcEnabled = EC_FALSE;
#endif
    m_wLogicalOutpWkc = 0;
    m_wLogicalInpWkc  = 0;

#if (defined INCLUDE_MEM_POOL)
    OsConfigDataMemPoolInitialize(m_dwMasterInstanceID);
#endif
}

/* Calculates Initialization command descriptor size */
static EC_T_DWORD CalcInitCmdDescSize(EcInitCmdDesc* pInitCmdDesc)
{
    ETYPE_EC_CMD_HEADER* pHeader = &pInitCmdDesc->EcCmdHeader;
    EC_T_DWORD dwDataLength = EC_ICMDHDR_GET_LEN_LEN(pHeader);
    EC_T_DWORD dwDescSize = sizeof(EcInitCmdDesc);
    
    /* data consist of data, validate data and validate mask data */
    dwDescSize += dwDataLength;
    if (EC_ECINITCMDDESC_GET_VALIDATE(pInitCmdDesc))
    {
        dwDescSize += dwDataLength;
        if (EC_ECINITCMDDESC_GET_VALIDATEMASK(pInitCmdDesc))
            dwDescSize += dwDataLength;
    }

    dwDescSize += EC_ECINITCMDDESC_GET_CMTLEN(pInitCmdDesc);
    dwDescSize += 1;

    return dwDescSize;
}

/* Calculate size of pointer array allocated with Granularity */
static EC_T_DWORD CalcGranArraySize(EC_T_DWORD dwCurrentCount, EC_T_DWORD dwGranularity)
{
    EC_T_DWORD dwAllocatedCount = dwGranularity * (dwCurrentCount / dwGranularity);
    if (0 != (dwCurrentCount % dwGranularity))
        dwAllocatedCount += dwGranularity;

    return dwAllocatedCount * sizeof(EC_T_PVOID);
}

/********************************************************************************/
/** \brief Destroys init command descriptor
*/
EC_T_VOID CEcConfigData::DestroyEcatInitCmdsDesc(EcInitCmdDesc** apPkdEcatInitCmdDesc, EC_T_WORD wNumEcatInitCmds)
{
    EC_T_DWORD dwIdx;

    if (EC_NULL == apPkdEcatInitCmdDesc)
    {
        /* nothing to do */
        return;
    }
    for (dwIdx = 0; dwIdx < wNumEcatInitCmds; dwIdx++)
    {
        if (EC_NULL != apPkdEcatInitCmdDesc[dwIdx])
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyEcatInitCmdsDesc", apPkdEcatInitCmdDesc[dwIdx], CalcInitCmdDescSize(apPkdEcatInitCmdDesc[dwIdx]));
            ConfigDataFreeMem(apPkdEcatInitCmdDesc[dwIdx]);
	    }
    }

    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyEcatInitCmdsDesc", apPkdEcatInitCmdDesc, 
        CalcGranArraySize(wNumEcatInitCmds, C_dwInitCmdAllocGranularity));
    ConfigDataFreeMem(apPkdEcatInitCmdDesc);
}

/* Calculates Mailboc command descriptor size */
static EC_T_DWORD _CalcMbxCmdDescSize(EcMailboxCmdDesc* pCmdDesc)
{
    EC_T_DWORD dwDescSize = sizeof(EcMailboxCmdDesc);
    dwDescSize += EC_ECMBXCMDDESC_GET_DATALEN(pCmdDesc);
    dwDescSize += EC_ECMBXCMDDESC_GET_CMTLEN(pCmdDesc);
    dwDescSize += 1;

    return dwDescSize;
}

#if (defined INCLUDE_VARREAD)
static EC_T_DWORD _CalcPDVarInfoDescSize(EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo)
{
    EC_T_DWORD dwDescSize = sizeof(EC_T_PD_VAR_INFO_INTERN);
    
    if (EC_NULL != pProcessVarInfo->szSpecificDataType)
    {
        dwDescSize += (EC_T_DWORD)(OsStrlen(pProcessVarInfo->szSpecificDataType) + 1);
    }
    return dwDescSize;
}
#endif

#if (defined INCLUDE_AOE_SUPPORT)
/********************************************************************************/
/** \brief Destroys AoE init command descriptor
*/
EC_T_VOID CEcConfigData::DestroyAoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave)
{
    EC_T_DWORD dwIdx;

    if (EC_NULL == pEniSlave->apPkdAoeInitCmdDesc)
    {
        /* nothing to do */
        return;
    }

    for (dwIdx = 0; dwIdx < pEniSlave->wNumAoeInitCmds; dwIdx++)
    {
        if (EC_NULL != pEniSlave->apPkdAoeInitCmdDesc[dwIdx])
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyAoeEcatInitCmdsDesc", pEniSlave->apPkdAoeInitCmdDesc[dwIdx], _CalcMbxCmdDescSize(pEniSlave->apPkdAoeInitCmdDesc[dwIdx]));
            ConfigDataFreeMem(pEniSlave->apPkdAoeInitCmdDesc[dwIdx]);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyAoeEcatInitCmdsDesc", pEniSlave->apPkdAoeInitCmdDesc, 
        CalcGranArraySize(pEniSlave->wNumAoeInitCmds, C_dwMbxInitCmdAllocGranularity));
    ConfigDataFreeMem(pEniSlave->apPkdAoeInitCmdDesc);
    pEniSlave->wNumAoeInitCmds = 0;
}
#endif

/********************************************************************************/
/** \brief Destroys CoE init command descriptor
*/
EC_T_VOID CEcConfigData::DestroyCoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave)
{
    EC_T_DWORD dwIdx;

    if (EC_NULL == pEniSlave->apPkdCoeInitCmdDesc)
    {
        /* nothing to do */
        return;
    }

    for (dwIdx = 0; dwIdx < pEniSlave->wNumCoeInitCmds; dwIdx++)
    {
        if (EC_NULL != pEniSlave->apPkdCoeInitCmdDesc[dwIdx])
        {
            EC_T_DWORD dwDescSize = _CalcMbxCmdDescSize(pEniSlave->apPkdCoeInitCmdDesc[dwIdx]) - sizeof(EC_SDO_HDR);
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyCoeEcatInitCmdsDesc", pEniSlave->apPkdCoeInitCmdDesc[dwIdx], dwDescSize);
            ConfigDataFreeMem(pEniSlave->apPkdCoeInitCmdDesc[dwIdx]);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyCoeEcatInitCmdsDesc", pEniSlave->apPkdCoeInitCmdDesc,
        CalcGranArraySize(pEniSlave->wNumCoeInitCmds, C_dwMbxInitCmdAllocGranularity));
    pEniSlave->wNumCoeInitCmds = 0;
    ConfigDataFreeMem(pEniSlave->apPkdCoeInitCmdDesc);
}

/********************************************************************************/
/** \brief Destroys EoE init command descriptor
*/
EC_T_VOID CEcConfigData::DestroyEoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave)
{
    EC_T_DWORD dwIdx = 0;

    if (EC_NULL == pEniSlave->apPkdEoeInitCmdDesc)
    {
        /* nothing to do */
        return;
    }

    for (dwIdx = 0; dwIdx < pEniSlave->wNumEoeInitCmds; dwIdx++)
    {
        if (EC_NULL != pEniSlave->apPkdEoeInitCmdDesc[dwIdx])
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyEoeEcatInitCmdsDesc", pEniSlave->apPkdEoeInitCmdDesc[dwIdx], _CalcMbxCmdDescSize(pEniSlave->apPkdEoeInitCmdDesc[dwIdx]));
            ConfigDataFreeMem(pEniSlave->apPkdEoeInitCmdDesc[dwIdx]);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroyEoeEcatInitCmdsDesc", pEniSlave->apPkdEoeInitCmdDesc, 
        CalcGranArraySize(pEniSlave->wNumEoeInitCmds, C_dwMbxInitCmdAllocGranularity));
    pEniSlave->wNumEoeInitCmds = 0;
    ConfigDataFreeMem(pEniSlave->apPkdEoeInitCmdDesc);
}

#if (defined INCLUDE_SOE_SUPPORT)
/********************************************************************************/
/** \brief Destroys SoE init command descriptor
*/
EC_T_VOID CEcConfigData::DestroySoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave)
{
    EC_T_DWORD dwIdx;

    if (EC_NULL == pEniSlave->apPkdSoeInitCmdDesc)
    {
        /* nothing to do */
        return;
    }

    for (dwIdx = 0; dwIdx < pEniSlave->wNumSoeInitCmds; dwIdx++)
    {
        if (EC_NULL != pEniSlave->apPkdSoeInitCmdDesc[dwIdx])
        {
            EC_T_DWORD dwDescSize = _CalcMbxCmdDescSize(pEniSlave->apPkdSoeInitCmdDesc[dwIdx]) - sizeof(EC_SOE_HDR);
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroySoeEcatInitCmdsDesc", pEniSlave->apPkdSoeInitCmdDesc[dwIdx], dwDescSize);
            ConfigDataFreeMem(pEniSlave->apPkdSoeInitCmdDesc[dwIdx]);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "DestroySoeEcatInitCmdsDesc", pEniSlave->apPkdSoeInitCmdDesc, 
        CalcGranArraySize(pEniSlave->wNumSoeInitCmds, C_dwMbxInitCmdAllocGranularity));
    pEniSlave->wNumSoeInitCmds = 0;
    ConfigDataFreeMem(pEniSlave->apPkdSoeInitCmdDesc);
}
#endif

/********************************************************************************/
/** \brief Destroys the EC_T_CFG_DATA_SLAVE object
*/
EC_T_VOID CEcConfigData::DeleteCfgDataSlave(EC_T_CONFIG_DATA_SLAVE* pCfgSlave)
{
    EC_T_ENI_SLAVE* pEniSlave = pCfgSlave->pEniSlave;

    DestroyCoeEcatInitCmdsDesc(pEniSlave);
    DestroyEoeEcatInitCmdsDesc(pEniSlave);
#if (defined INCLUDE_SOE_SUPPORT)
    DestroySoeEcatInitCmdsDesc(pEniSlave);
#endif
#if (defined INCLUDE_AOE_SUPPORT)
    DestroyAoeEcatInitCmdsDesc(pEniSlave);
#endif
    DestroyEcatInitCmdsDesc(pEniSlave->apPkdEcatInitCmdDesc, pEniSlave->wNumEcatInitCmds);
    pEniSlave->apPkdEcatInitCmdDesc = EC_NULL;
    pEniSlave->wNumEcatInitCmds = 0;

    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::DeleteEniSlave", pEniSlave, sizeof(EC_T_ENI_SLAVE));
    ConfigDataFreeMem(pCfgSlave->pEniSlave);
}


CEcConfigData::~CEcConfigData() /* this destructor can be called outside delete call */
{
    EC_T_DWORD dwIdx = 0;

#if (defined INCLUDE_VARREAD)
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_pSwapInfo", m_pSwapInfo, sizeof(EC_T_CYC_SWAP_INFO) * m_wNumSwapInfosAllocated);
    ConfigDataFreeMem(m_pSwapInfo);
    m_wNumSwapInfosAllocated = 0;
#endif
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "~CEcConfigData::m_pCopyInfo", m_pCopyInfo, sizeof(EC_T_CYC_COPY_INFO) * m_wNumCopyInfosAllocated);
    ConfigDataFreeMem(m_pCopyInfo);
    m_wNumCopyInfosAllocated = 0;

    for (dwIdx = 0; dwIdx < m_dwNumCfgDataSlaves; dwIdx++)
    {
		if (EC_NULL != m_apoCfgDataSlave[dwIdx])
		{
			DeleteCfgDataSlave(m_apoCfgDataSlave[dwIdx]);
			EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::DeleteCfgDataSlave", m_apoCfgDataSlave[dwIdx], sizeof(EC_T_CONFIG_DATA_SLAVE));
			ConfigDataFreeMem(m_apoCfgDataSlave[dwIdx]);
		}
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_apoCfgDataSlave", m_apoCfgDataSlave,
        CalcGranArraySize(m_dwCfgDataSlaveArraySize, C_dwCfgDataSlaveAllocGranularity));
    m_dwCfgDataSlaveArraySize = 0;
    m_dwNumCfgDataSlaves = 0;
    ConfigDataFreeMem(m_apoCfgDataSlave);

    /* Destroy cyclic entries, frames and commands */
    for (dwIdx = 0; (EC_NULL != m_apoCfgCyclic) && (dwIdx < m_dwNumCyclicEntries); dwIdx++)
    {
        if (EC_NULL != m_apoCfgCyclic[dwIdx])
        {
            EC_T_DWORD dwFrmIdx = 0;
            EcCycFrameDesc** apCyclicFrameDesc = m_apoCfgCyclic[dwIdx]->apCyclicFrameDesc;
            for (dwFrmIdx = 0; (EC_NULL != apCyclicFrameDesc) && (dwFrmIdx < m_apoCfgCyclic[dwIdx]->dwNumCyclicFrames); dwFrmIdx++)
            {
                EC_T_DWORD dwCmdsIndex;
                if (apCyclicFrameDesc[dwFrmIdx] != EC_NULL)
                {
                    for (dwCmdsIndex = 0; dwCmdsIndex < apCyclicFrameDesc[dwFrmIdx]->wCmdCnt; dwCmdsIndex++)
                    {
                        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::~CyclicFrameCmd",
                            apCyclicFrameDesc[dwFrmIdx]->apCycCmd[dwCmdsIndex], sizeof(EcCycCmdConfigDesc));
                        ConfigDataFreeMem(apCyclicFrameDesc[dwFrmIdx]->apCycCmd[dwCmdsIndex]);
                    }
                    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::~CyclicFrameDesc", apCyclicFrameDesc[dwFrmIdx], sizeof(EcCycFrameDesc));
                    apCyclicFrameDesc[dwFrmIdx]->wCmdCnt = 0;
                    ConfigDataFreeMem(m_apoCfgCyclic[dwIdx]->apCyclicFrameDesc[dwFrmIdx]);
                }
            }
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_DATA, "CCfgCyclic::~apCyclicFrameDesc", apCyclicFrameDesc,
                CalcGranArraySize(m_apoCfgCyclic[dwIdx]->dwNumCyclicFrames, C_dwCfgCyclicFrameAllocGranularity));
            m_apoCfgCyclic[dwIdx]->dwNumCyclicFrames = 0;
            ConfigDataFreeMem(m_apoCfgCyclic[dwIdx]->apCyclicFrameDesc);
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::~CyclicEntry", m_apoCfgCyclic[dwIdx],
                sizeof(EC_T_CYCLIC_ENTRY_CFG_DESC));
            ConfigDataFreeMem(m_apoCfgCyclic[dwIdx]);
        }
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_apoCfgCyclic", m_apoCfgCyclic, m_dwCfgCyclicArraySize * sizeof(EC_T_PVOID));
    m_dwNumCyclicEntries = 0;
    m_dwCfgCyclicArraySize = 0;
    ConfigDataFreeMem(m_apoCfgCyclic);
  
#if (defined INCLUDE_VARREAD)
    for (dwIdx = 0; dwIdx < m_dwNumProcessInputVarInfoEntries; dwIdx++)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~ProcessInputVarInfo", m_apoProcessInputVarInfo[dwIdx],
            _CalcPDVarInfoDescSize(m_apoProcessInputVarInfo[dwIdx]));
        ConfigDataFreeMem(m_apoProcessInputVarInfo[dwIdx]);
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_apoProcessInputVarInfo", m_apoProcessInputVarInfo,
        m_dwProcessInputVarInfoArraySize * sizeof(EC_T_PVOID));
    m_dwNumProcessInputVarInfoEntries = 0;
    m_dwProcessInputVarInfoArraySize = 0;
    ConfigDataFreeMem(m_apoProcessInputVarInfo);

    for (dwIdx = 0; dwIdx < m_dwNumProcessOutputVarInfoEntries; dwIdx++)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~ProcessImageOutputVarInfo", m_apoProcessOutputVarInfo[dwIdx],
            _CalcPDVarInfoDescSize(m_apoProcessOutputVarInfo[dwIdx]));
        ConfigDataFreeMem(m_apoProcessOutputVarInfo[dwIdx]);
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_apoProcessOutputVarInfo", m_apoProcessOutputVarInfo,
        m_dwProcessOutputVarInfoArraySize * sizeof(EC_T_PVOID));
    m_dwNumProcessOutputVarInfoEntries = 0;
    m_dwProcessOutputVarInfoArraySize = 0;
    ConfigDataFreeMem(m_apoProcessOutputVarInfo);
#endif
    DeleteStatString(m_strProductKey, GetMemLog());

    DestroyEcatInitCmdsDesc(m_oCfgMaster.apPkdEcatInitCmdDesc, m_oCfgMaster.wNumEcatInitCmds);
    m_oCfgMaster.apPkdEcatInitCmdDesc = EC_NULL;
    m_oCfgMaster.wNumEcatInitCmds = 0;

#if (defined INCLUDE_HOTCONNECT)
    for (dwIdx = 0; dwIdx < m_dwNumHCGroups; dwIdx++)
    {
        EC_T_HC_GROUP_CFG_DESC* pHCGroup = m_apoHotConnectGroup[dwIdx];
        DestroyEcatInitCmdsDesc(pHCGroup->apPkdEcatIdentifyCmdDesc, pHCGroup->wNumEcatIdentifyCmds);
        pHCGroup->apPkdEcatIdentifyCmdDesc = EC_NULL;
        pHCGroup->wNumEcatIdentifyCmds = 0;

        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::~apHCGroupMember", pHCGroup->apGroupMember, 
            sizeof(EC_T_ENI_SLAVE*) * pHCGroup->dwNumGroupMembersListed);
        pHCGroup->dwNumGroupMembersListed = 0;
        pHCGroup->dwNumGroupMembers = 0;
        ConfigDataFreeMem(pHCGroup->apGroupMember);
        
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_apoHotConnectGroup[dwHCGrpIdx]", m_apoHotConnectGroup[dwIdx],
            sizeof(EC_T_HC_GROUP_CFG_DESC));
        ConfigDataFreeMem(m_apoHotConnectGroup[dwIdx]);
    }
    EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_apoHotConnectGroup", m_apoHotConnectGroup, 
        m_dwHCGroupArraySize * sizeof(EC_T_PVOID));
    m_dwNumHCGroups = 0;
    m_dwHCGroupArraySize = 0;
    ConfigDataFreeMem(m_apoHotConnectGroup);
#endif
}

/********************************************************************************/
/** \brief Re-initialize CEcConfigData.
*
*/
EC_T_VOID CEcConfigData::Initialize(EC_T_VOID)
{
#if (!defined INCLUDE_MEM_POOL)
    this->~CEcConfigData();
#endif
    InitializeConfigData();
}

/********************************************************************************/
/** \brief Set product key.
*
* \return EC_T_NOERROR on success
*/
EC_T_DWORD CEcConfigData::SetProductKey(const EC_T_CHAR* pszProductKey, const EC_T_DWORD dwLen)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    dwRetVal = InitString(m_strProductKey, pszProductKey, dwLen, GetMemLog());
    m_dwProductKeyStringLen = (EC_T_DWORD)GetStringLen(&m_strProductKey);

    return dwRetVal;
}


/********************************************************************************/
/** \brief Get EtherCAT MAC address.
*
* \return
*/
EC_T_DWORD CEcConfigData::GetMacAddrDestination(ETHERNET_ADDRESS* pMacAddr)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;

    OsMemcpy(pMacAddr, &m_oCfgMaster.oMacDest, sizeof(ETHERNET_ADDRESS));

    /* no errors */
    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/********************************************************************************/
/** \brief Get EtherCAT MAC address.
*
* \return
*/
EC_T_DWORD CEcConfigData::GetMacAddrSource(ETHERNET_ADDRESS* pMacAddr)
{
    EC_T_DWORD      dwRetVal = EC_E_ERROR;

    OsMemcpy(pMacAddr, &m_oCfgMaster.oMacSrc, sizeof(ETHERNET_ADDRESS));

    /* no errors */
    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/********************************************************************************/
/** \brief Create an new ENI slave and adds it into internal storage
*
* \return Pointer to the created slave
*/
EC_T_DWORD CEcConfigData::CreateEniSlave(
    EC_T_ENI_SLAVE** ppEniSlave) /**< [out] ENI slave configuration object (optional) */
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_ENI_SLAVE* pEniSlave = EC_NULL;

    /* create and initialize eni slave */
    pEniSlave = (EC_T_ENI_SLAVE*)ConfigDataAllocMem(sizeof(EC_T_ENI_SLAVE));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::CreateEniSlave", pEniSlave, sizeof(EC_T_ENI_SLAVE));
    if (pEniSlave == EC_NULL)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pEniSlave, 0, sizeof(EC_T_ENI_SLAVE));
    pEniSlave->dwVendorId = (EC_T_DWORD)-1;
    pEniSlave->dwProductCode = (EC_T_DWORD)-1;
    pEniSlave->dwRevisionNumber = (EC_T_DWORD)-1;
    pEniSlave->dwSerialNumber = (EC_T_DWORD)-1;
    OsMemset(pEniSlave->adwPDOutOffset, 0xff, sizeof(pEniSlave->adwPDOutOffset));
    OsMemset(pEniSlave->adwPDOutSize,      0, sizeof(pEniSlave->adwPDOutSize));
    OsMemset(pEniSlave->adwPDInOffset,  0xff, sizeof(pEniSlave->adwPDInOffset));
    OsMemset(pEniSlave->adwPDInSize,       0, sizeof(pEniSlave->adwPDInSize));

    dwRetVal = AddEniSlave(pEniSlave);
    if (EC_E_NOERROR != dwRetVal)
        goto Exit;

    if (EC_NULL != ppEniSlave)
        *ppEniSlave = pEniSlave;

Exit:
    if (EC_E_NOERROR != dwRetVal)
    {
        /* clean up */
        if (EC_NULL != pEniSlave)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::CreateEniSlave", pEniSlave, sizeof(EC_T_ENI_SLAVE));
            ConfigDataFreeMem(pEniSlave);
        }
    }
    return dwRetVal;
}

/********************************************************************************/
/** \brief Adds an eni slave to the internal storage
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::AddEniSlave(EC_T_ENI_SLAVE* pEniSlave)
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_BOOL bOK = EC_TRUE;
    EC_T_CONFIG_DATA_SLAVE* pCfgSlave = EC_NULL;

    m_dwNumCfgDataSlaves++;
    if (m_dwNumCfgDataSlaves > m_dwCfgDataSlaveArraySize)
    {
        bOK = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&m_apoCfgDataSlave, &m_dwCfgDataSlaveArraySize, C_dwCfgDataSlaveAllocGranularity);
        if (!bOK)
        {
            m_dwNumCfgDataSlaves--;
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }
    
    OsDbgAssert(m_apoCfgDataSlave != EC_NULL);

    /* create a new CONFIG_DATA_SLAVE for internal use */
    pCfgSlave = (EC_T_CONFIG_DATA_SLAVE*)ConfigDataAllocMem(sizeof(EC_T_CONFIG_DATA_SLAVE));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::AddEniSlave", pCfgSlave, sizeof(EC_T_CONFIG_DATA_SLAVE));
    if (pCfgSlave == EC_NULL)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pCfgSlave, 0, sizeof(EC_T_CONFIG_DATA_SLAVE));
    pCfgSlave->pEniSlave = pEniSlave;

    m_apoCfgDataSlave[m_dwNumCfgDataSlaves - 1] = pCfgSlave;
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief
*
* \return
*/
EC_T_DWORD CEcConfigData::SetSlaveExcludedFlag(
    EC_T_INT nSlaveIndex, 
    EC_T_BOOL bExcluded
    )
{
    OsDbgAssert(m_apoCfgDataSlave != EC_NULL);
    EC_T_CONFIG_DATA_SLAVE* pCfgSlave = m_apoCfgDataSlave[nSlaveIndex];
    EC_T_WORD  wPhysAddr = pCfgSlave->pEniSlave->wPhysAddr;
    EC_T_INT   nSlaveIdx = 0;
    EC_T_INT   nSlaves   = (EC_T_INT)m_dwNumCfgDataSlaves;
    EC_T_DWORD dwRetVal  = EC_E_ERROR;

    /* mark slaves connected to config slave */
    for (nSlaveIdx = 0; nSlaveIdx < nSlaves; nSlaveIdx++)
    {
        EC_T_SLAVE_PORT_DESC* pPrevPortDesc = &m_apoCfgDataSlave[nSlaveIdx]->pEniSlave->aoPreviousPort[0];

        if (wPhysAddr == pPrevPortDesc->wSlaveAddress)
        {
            if (bExcluded)
            {
                pPrevPortDesc->wPortNumber |= IGNORE_PREV_PORT_FLAG;
            }
            else
            {
                pPrevPortDesc->wPortNumber &= ~IGNORE_PREV_PORT_FLAG;
            }
        }
    }
    /* mark the slave as excluded */
    pCfgSlave->bExcluded = bExcluded;

    /* no errors */
    dwRetVal = EC_E_NOERROR;

    return dwRetVal;
}

/********************************************************************************/
/** \brief Set previous port address in slave
*
* \return
*/
EC_T_DWORD CEcConfigData::SetSlavePreviousPort(
    EC_T_INT nSlaveIndex,       /**<[IN] Slave index */
    EC_T_WORD wPhysAddressPrev, /**<[IN] Physical port address of previous slave */
    EC_T_WORD wPortPrev         /**<[IN] Prevoius port */
    )
{
    OsDbgAssert(m_apoCfgDataSlave != EC_NULL);
    EC_T_ENI_SLAVE* pEniSlave = m_apoCfgDataSlave[nSlaveIndex]->pEniSlave;

    if ((ESC_PORT_INVALID == wPortPrev) || (INVALID_FIXED_ADDR == wPhysAddressPrev))
    {
        OsDbgAssert((INVALID_FIXED_ADDR == wPhysAddressPrev) && (INVALID_FIXED_ADDR == wPhysAddressPrev));
        pEniSlave->wNumPreviousPorts = 0;
    }
    else
    {
        pEniSlave->wNumPreviousPorts = 1;
    }
    pEniSlave->aoPreviousPort[0].wSlaveAddress = wPhysAddressPrev;
    pEniSlave->aoPreviousPort[0].wPortNumber = wPortPrev;

    return EC_E_NOERROR;
}

/********************************************************************************/
/** \brief Read ECat master create slave parameters.
*
* \return
*/
EC_T_ENI_SLAVE* CEcConfigData::GetEniSlave(EC_T_INT nSlaveIndex)
{
    EC_T_ENI_SLAVE* pEniSlave = EC_NULL;

    if (nSlaveIndex >= (EC_T_INT)m_dwNumCfgDataSlaves)
    {
        /* Should never happen */
        OsDbgAssert(EC_FALSE);
        goto Exit;
    }

    if (!m_apoCfgDataSlave[nSlaveIndex]->bExcluded)
    {
        pEniSlave = m_apoCfgDataSlave[nSlaveIndex]->pEniSlave;
    }

Exit:
    return pEniSlave;
}

#if (defined INCLUDE_AOE_SUPPORT)
/********************************************************************************/
/** \brief Get the AoE slave net id array.
*
* \return EC_E_NOERROR on success
*/
EC_T_BYTE* CEcConfigData::GetAoeSlaveNetId(EC_T_INT nSlaveIndex)
{
    EC_T_ENI_SLAVE* pEniSlave = EC_NULL;

    OsDbgAssert(m_apoCfgDataSlave != EC_NULL);
    pEniSlave = m_apoCfgDataSlave[nSlaveIndex]->pEniSlave;

    return pEniSlave->abyAoeNetId;
}
#endif /*INCLUDE_AOE_SUPPORT*/

/********************************************************************************/
/** \brief Get process image size parameters.
*
* \return
*/
EC_T_VOID CEcConfigData::GetProcessImageParams(
    EC_T_DWORD* pdwIn, /**<[Out] Byte size of input process image */
    EC_T_DWORD* pdwOut /**<[Out] Byte size of input process image */
    )
{
    *pdwIn = m_dwInputByteSize;
    *pdwOut = m_dwOutputByteSize;
}

/********************************************************************************/
/** \brief Create a configure cyclic entry and store it in internal storage.
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::CreateCyclicEntry(
    EC_T_CYCLIC_ENTRY_CFG_DESC** ppCreatedCyc) /**< [out] Created cyclic entry configuration object (optional) */
{
    EC_T_CYCLIC_ENTRY_CFG_DESC* pNewCycEntry = EC_NULL;
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_BOOL bOk = EC_TRUE;

    /* new cyclic entry */
    if (m_dwNumCyclicEntries + 1 > m_dwCfgCyclicArraySize)
    {
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&m_apoCfgCyclic, &m_dwCfgCyclicArraySize, C_dwCfgCyclicAllocGranularity);
        if (!bOk)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    pNewCycEntry = (EC_T_CYCLIC_ENTRY_CFG_DESC*)ConfigDataAllocMem(sizeof(EC_T_CYCLIC_ENTRY_CFG_DESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::CreateCyclicEntry", 
        pNewCycEntry, sizeof(EC_T_CYCLIC_ENTRY_CFG_DESC));
    if (EC_NULL == pNewCycEntry)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pNewCycEntry, 0, sizeof(EC_T_CYCLIC_ENTRY_CFG_DESC));

    m_apoCfgCyclic[m_dwNumCyclicEntries++] = pNewCycEntry;

    if (EC_NULL != ppCreatedCyc)
        *ppCreatedCyc = pNewCycEntry;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Create a cyclic frame and add it to the cyclic entry
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::CreateCyclicFrame(
    EC_T_CYCLIC_ENTRY_CFG_DESC* poCyclicEntry,  /**< [in] Cyclic configuration object */
    EcCycFrameDesc** ppCreatedFrame,            /**< [out] Created cyclic frame description (optional) */
    EC_T_INT nPos                               /**< [in] Position of the new frame in cyclic entry */
    )
{
    EcCycFrameDesc* pNewCycFrameDesc = EC_NULL;
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_BOOL bOk = EC_TRUE;
    EC_T_INT nIdx = 0;

    OsDbgAssert(poCyclicEntry != EC_NULL);

    poCyclicEntry->dwNumCyclicFrames++;
    if (poCyclicEntry->dwNumCyclicFrames > poCyclicEntry->dwCyclicFrameArraySize)
    {
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&poCyclicEntry->apCyclicFrameDesc, &poCyclicEntry->dwCyclicFrameArraySize, C_dwCfgCyclicFrameAllocGranularity);
        if (!bOk)
        {
            poCyclicEntry->dwNumCyclicFrames--;
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }
    /* Create new frame description */
    pNewCycFrameDesc = (EcCycFrameDesc*)ConfigDataAllocMem(sizeof(EcCycFrameDesc));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::CreateCyclicFrame()", pNewCycFrameDesc, sizeof(EcCycFrameDesc));
    if (EC_NULL == pNewCycFrameDesc)
    {
        poCyclicEntry->dwNumCyclicFrames--;
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pNewCycFrameDesc, 0, sizeof(EcCycFrameDesc));

    /* if necessary move the existing entries to add the new one at his desired position */
    for (nIdx = poCyclicEntry->dwNumCyclicFrames - 1; nIdx > nPos; nIdx--)
    {
        poCyclicEntry->apCyclicFrameDesc[nIdx] = poCyclicEntry->apCyclicFrameDesc[nIdx-1];
    }
    poCyclicEntry->apCyclicFrameDesc[nPos] = pNewCycFrameDesc;

    if (EC_NULL != ppCreatedFrame)
        *ppCreatedFrame = pNewCycFrameDesc;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Creates a new cyclic frame command and adds it to a cyclic frame.
*
* \return N/A.
*/
EC_T_BOOL CEcConfigData::CreateCyclicFrameCmd(
    EcCycFrameDesc* pCyclicFrameDesc,         /**< [in] Cyclic frame description (optional) */
    EC_T_CYCLIC_CMD_CFG_DESC* pCycCmdCfgDesc, /**< [in] Temporarily cyclic command configuration description */
    EcCycCmdConfigDesc** ppCreatedCmd         /**< [out] Cyclic command configuration description (optional) */
    )
{
    EC_T_BOOL            bOk = EC_TRUE;
    EC_T_BOOL            bReadCmd = EC_FALSE;
    EC_T_BOOL            bWriteCmd = EC_FALSE;
    EC_T_BOOL            bNopCmd = EC_FALSE;
    EcCycCmdConfigDesc*  pEcCycCmdDesc;

    /* only store cyclic commands with effect ! */
    if (0 == EC_ICMDHDR_GET_LEN_LEN(&(pCycCmdCfgDesc->oEcCycCmdDesc.EcCmdHeader)))
    {
        goto Exit;
    }

    pEcCycCmdDesc = (EcCycCmdConfigDesc*)ConfigDataAllocMem(sizeof(EcCycCmdConfigDesc));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_DATA,"CEcConfigData::CreateCyclicFrameCmd", pEcCycCmdDesc, sizeof(EcCycCmdConfigDesc));
    OsMemcpy(pEcCycCmdDesc, &pCycCmdCfgDesc->oEcCycCmdDesc,sizeof(EcCycCmdConfigDesc));

    switch (pEcCycCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
    {
    case EC_CMD_TYPE_LRD:
    case EC_CMD_TYPE_LWR:
    case EC_CMD_TYPE_LRW:
        /* logical address should be valid */
        if (!pCycCmdCfgDesc->bAddrValid)
        {
            EC_DBGMSG("CEcConfigData::CreateCyclicFrameCmd(): no logic address for LRD/LRW/LWR cyclic command\n");
        }
        break;
    default:
        /* physical address should be valid */
        if (!pCycCmdCfgDesc->bAdpValid)
        {
            EC_DBGMSG("CEcConfigData::CreateCyclicFrameCmd(): no ADP address for non-LRD/LRW/LWR cyclic command\n");
        }
        if (!pCycCmdCfgDesc->bAdoValid)
        {
            EC_DBGMSG("CEcConfigData::CreateCyclicFrameCmd(): no ADO address for non-LRD/LRW/LWR cyclic command\n");
        }
        break;
    }

    pEcCycCmdDesc->cmdSize = (EC_T_WORD)ETYPE_EC_CMD_GETLEN(&(pEcCycCmdDesc->EcCmdHeader));
    pEcCycCmdDesc->bCopyInputs = EC_FALSE;
    pEcCycCmdDesc->bCopyOutputs = EC_FALSE;

    /* determine correct IN/OUT Direction of EtherCAT Datagrams, because TwinCAT generated XML File does not !!! */
    bReadCmd = bWriteCmd = EC_FALSE;
    switch (pEcCycCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd)
    {
    case EC_CMD_TYPE_NOP:   bReadCmd = EC_FALSE;    bWriteCmd = EC_FALSE;   bNopCmd = EC_TRUE; break;

    case EC_CMD_TYPE_FPRD:
    case EC_CMD_TYPE_BRD:
    case EC_CMD_TYPE_LRD:
    case EC_CMD_TYPE_APRD:  bReadCmd = EC_TRUE;     bWriteCmd = EC_FALSE;   bNopCmd = EC_FALSE; break;

    case EC_CMD_TYPE_FPWR:
    case EC_CMD_TYPE_BWR:
    case EC_CMD_TYPE_LWR:
    case EC_CMD_TYPE_APWR:  bReadCmd = EC_FALSE;    bWriteCmd = EC_TRUE;    bNopCmd = EC_FALSE; break;

    case EC_CMD_TYPE_FPRW:
    case EC_CMD_TYPE_BRW:
    case EC_CMD_TYPE_LRW:
    case EC_CMD_TYPE_APRW:  bReadCmd = EC_TRUE;     bWriteCmd = EC_TRUE;    bNopCmd = EC_FALSE; break;

        /* if user application shall write out bytes to auto increment frames at begin,
        * this shall be configured anywhere other !!! */
    case EC_CMD_TYPE_FRMW:
    case EC_CMD_TYPE_ARMW:  bReadCmd = EC_TRUE;     bWriteCmd = EC_FALSE;   bNopCmd = EC_FALSE; break;

        /* not defined in IEC 61158, suppose an extended command is controlled correctly,
        * by means of OutputOffs and InputOffs in XML File
        */
    case EC_CMD_TYPE_EXT:   bReadCmd = EC_TRUE;     bWriteCmd = EC_TRUE;    bNopCmd = EC_FALSE; break;
    default:
    {
        EC_ERRORMSGC(("CEcConfigData::CreateCyclicFrameCmd(): Unknown cyclic cmd in Configuration: 0x%x\n", pEcCycCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd));
        bOk = EC_FALSE;
        break;
    }
    }

    if (bReadCmd && (0 != EC_ICMDHDR_GET_LEN_LEN(&(pEcCycCmdDesc->EcCmdHeader))))
    {
        if (pCycCmdCfgDesc->bInputOffsValid)
        {
            /* this command is responsible for reading the input process data (LRD) */
            pEcCycCmdDesc->bCopyInputs = EC_TRUE;
            pEcCycCmdDesc->dwInImageSize = EC_ICMDHDR_GET_LEN_LEN(&(pEcCycCmdDesc->EcCmdHeader));
            pEcCycCmdDesc->dwInOffset = (EC_T_WORD)pCycCmdCfgDesc->dwInputOffs;
        }
        else
        {
            EC_ERRORMSGC(("CEcConfigData::CreateCyclicFrameCmd(): missing input offset in cyclic cmd in Configuration\n"));
            bOk = EC_FALSE;
            goto Exit;
        }
    }
    if ((bWriteCmd && (0 != EC_ICMDHDR_GET_LEN_LEN(&(pEcCycCmdDesc->EcCmdHeader))))
#if (defined INCLUDE_TRACE_DATA)
        || (bNopCmd && (TRACE_DATA_CMD_ADO == EC_ICMDHDR_GET_ADDR_ADO(&(pEcCycCmdDesc->EcCmdHeader))
        && (0 == EC_ICMDHDR_GET_ADDR_ADP(&(pEcCycCmdDesc->EcCmdHeader)))))
#endif
       )
    {
        if (pCycCmdCfgDesc->bOutputOffsValid)
        {
            /* this command is responsible for writing the output process data (LWR) */
            pEcCycCmdDesc->bCopyOutputs = EC_TRUE;
            pEcCycCmdDesc->dwOutImageSize = EC_ICMDHDR_GET_LEN_LEN(&(pEcCycCmdDesc->EcCmdHeader));
            pEcCycCmdDesc->dwOutOffset = (EC_T_WORD)pCycCmdCfgDesc->dwOutputOffs;
        }
        else
        {
            EC_ERRORMSGC(("CEcConfigData::CreateCyclicFrameCmd(): missing output offset in cyclic cmd in Configuration\n"));
            bOk = EC_FALSE;
            goto Exit;
        }
    }

    if (EC_NULL != pCyclicFrameDesc)
    {
        pCyclicFrameDesc->wCmdCnt++;
        if (pCyclicFrameDesc->wCmdCnt > MAX_NUM_CMD_PER_FRAME)
        {
            EC_ERRORMSG(("CEcConfigData::CreateCyclicFrameCmd() - Too many cyclic commands in frame\n"));
            bOk = EC_FALSE;
            goto Exit;
        }
        pCyclicFrameDesc->apCycCmd[pCyclicFrameDesc->wCmdCnt - 1] = pEcCycCmdDesc;
    }

    if (EC_NULL != ppCreatedCmd)
        *ppCreatedCmd = pEcCycCmdDesc;
Exit:
#if (!defined INCLUDE_TRACE_DATA)
	EC_UNREFPARM(bNopCmd);
#endif
    return bOk;
}

/********************************************************************************/
/** \brief Get cyclic entry parameters.
*
* \return N/A
*/
EC_T_VOID CEcConfigData::GetCyclicEntryParameters(
    EC_T_INT    nCycEntryIndex,  /* [in]  Cyclic entry index in XML configuration file */
    EC_T_DWORD* pdwCycleTime,    /* [out] cycle time */
    EC_T_DWORD* pdwPriority,     /* [out] priority */
    EC_T_DWORD* pdwTaskId        /* [out] task id */
    )
{
    EC_T_CYCLIC_ENTRY_CFG_DESC* poCfgCyclic = EC_NULL;

    OsDbgAssert(m_apoCfgCyclic != EC_NULL);

    poCfgCyclic = m_apoCfgCyclic[nCycEntryIndex];
    if (poCfgCyclic == EC_NULL)
    {
        /* should never happen */
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::GetCyclicEntryParameters() - missing cyclic entry index %d\n", nCycEntryIndex);
        goto Exit;
    }

    *pdwCycleTime = poCfgCyclic->dwCycleTime;
    *pdwPriority = poCfgCyclic->dwPriority;
    *pdwTaskId = poCfgCyclic->dwTaskId;

Exit:
    return;
}
/********************************************************************************/
/** \brief Get number of cyclic frames.
*
* \return
*/
EC_T_INT CEcConfigData::GetNumCyclicFrames(
    EC_T_INT nCycEntryIndex        /* [in]  Cyclic entry index in XML configuration file */
    )
{
    EC_T_INT    nFrames = 0;
    EC_T_CYCLIC_ENTRY_CFG_DESC* poCfgCyclic;

    OsDbgAssert(m_apoCfgCyclic != EC_NULL);
    poCfgCyclic = m_apoCfgCyclic[nCycEntryIndex];
    if (poCfgCyclic == EC_NULL)
    {
        /* should never happen */
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::GetNumCyclicFrames() - missing cyclic entry index %d\n", nCycEntryIndex);
        goto Exit;
    }
    nFrames = (EC_T_INT)poCfgCyclic->dwNumCyclicFrames;

Exit:
    return nFrames;
}

/********************************************************************************/
/** \brief Get total number of cyclic commands.
*
* \return
*/
EC_T_INT CEcConfigData::GetNumCyclicCmds()
{
    EC_T_INT nCmds = 0;
    EC_T_INT nFrameIdx = 0;
    EC_T_INT nCycEntryIdx = 0;

    /* iterate through all cyclic entries */
    for (nCycEntryIdx = 0; nCycEntryIdx < (EC_T_INT)m_dwNumCyclicEntries; nCycEntryIdx++)
    {
        for (nFrameIdx = 0; nFrameIdx < GetNumCyclicFrames(nCycEntryIdx); nFrameIdx++)
        {
            nCmds += GetNumCyclicCmds(nCycEntryIdx, nFrameIdx);
        }        
    }

    return nCmds;
}


/********************************************************************************/
/** \brief Get number of cyclic commands in selected frame.
*
* \return
*/
EC_T_INT CEcConfigData::GetNumCyclicCmds(
    EC_T_INT nCycEntryIndex,    /**< [in]   Cyclic entry index */
    EC_T_INT nFrameIndex        /**< [in]   Frame to parse Index */
    )
{
    EC_T_CYCLIC_ENTRY_CFG_DESC* poCfgCyclic;
    EcCycFrameDesc* pCyclicFrameDesc;
    EC_T_INT nCmds = -1;

    OsDbgAssert(m_apoCfgCyclic != EC_NULL);

    poCfgCyclic = m_apoCfgCyclic[nCycEntryIndex];
    if (poCfgCyclic == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::GetNumCyclicCmds() - missing cyclic entry index %d\n", nCycEntryIndex);
        goto Exit;
    }

    pCyclicFrameDesc = poCfgCyclic->apCyclicFrameDesc[nFrameIndex];
    if (pCyclicFrameDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::GetNumCyclicCmds() - missing frame index %d in cyclic entry index %d\n", nFrameIndex, nCycEntryIndex);
        goto Exit;
    }
    nCmds = (EC_T_INT)pCyclicFrameDesc->wCmdCnt;

Exit:
    return nCmds;
}

/********************************************************************************/
/** \brief Read cyclic command.
*
* \return
*/
EC_T_DWORD CEcConfigData::ReadCyclicCmd(
    EcCycCmdConfigDesc** ppCmd,           /**< [out]  Pointer to cyclic command description */
    EC_T_INT    nCycEntryIndex,  /**< [in]   Cyclic entry Index */
    EC_T_INT    nFrameIndex,     /**< [in]   Frame Index */
    EC_T_INT    nCmdIndex        /**< [in]   Command index in frame */
)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_CYCLIC_ENTRY_CFG_DESC* poCfgCyclic = EC_NULL;
    EcCycFrameDesc* pCyclicFrameDesc = EC_NULL;
    EcCycCmdConfigDesc*  pEcCycCmdDesc = EC_NULL;
    

    OsDbgAssert(m_apoCfgCyclic != EC_NULL);
    
    poCfgCyclic = m_apoCfgCyclic[nCycEntryIndex];
    if (poCfgCyclic == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::ReadCyclicCmd() - missing cyclic entry index %d\n", nCycEntryIndex);
        goto Exit;
    }

    pCyclicFrameDesc = poCfgCyclic->apCyclicFrameDesc[nFrameIndex];
    if (pCyclicFrameDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::ReadCyclicCmd() - missing frame index %d in cyclic entry index %d\n", nFrameIndex, nCycEntryIndex);
        goto Exit;
    }

    pEcCycCmdDesc = pCyclicFrameDesc->apCycCmd[nCmdIndex];
    if (pEcCycCmdDesc == EC_NULL)
    {
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::ReadCyclicCmd() - missing cmd index %d in frame index %d in cyclic entry index %d\n", nCmdIndex, nFrameIndex, nCycEntryIndex);
        goto Exit;
    }

    *ppCmd = pEcCycCmdDesc;
    //OsMemcpy(pCmd, pEcCmdDesc, sizeof(EcCmdDesc));
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}

EC_T_VOID CEcConfigData::SetInOutputOffset(EcCycCmdConfigDesc* pCmdDesc, EC_T_DWORD dwOffs, EC_T_BOOL bIsInput)
{
    EC_T_BYTE byCmd = pCmdDesc->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd;
    /* only check logical commands here */
    if (m_oCfgMaster.dwLogAddressMBoxStates != EC_ICMDHDR_GET_ADDR(&(pCmdDesc->EcCmdHeader)))
    {
        if ((byCmd == EC_CMD_TYPE_LRD) || (byCmd == EC_CMD_TYPE_LWR) || (byCmd == EC_CMD_TYPE_LRW))
        {
            if (bIsInput)
            {
                m_dwLowestInOffset = EC_MIN(m_dwLowestInOffset, dwOffs);
            }
            else
            {
                m_dwLowestOutOffset = EC_MIN(m_dwLowestOutOffset, dwOffs);
            }
        }
        if ((byCmd == EC_CMD_TYPE_LRD) || (byCmd == EC_CMD_TYPE_LRW))
        {
            m_dwHighestInByte = EC_MAX(m_dwHighestInByte, (dwOffs + EC_ICMDHDR_GET_LEN_LEN(&(pCmdDesc->EcCmdHeader))));
        }
        if ((byCmd == EC_CMD_TYPE_LWR) || (byCmd == EC_CMD_TYPE_LRW))
        {
            m_dwHighestOutByte = EC_MAX(m_dwHighestOutByte, (dwOffs + EC_ICMDHDR_GET_LEN_LEN(&(pCmdDesc->EcCmdHeader))));
        }
    }
}

/********************************************************************************/
/** \brief Create copy info entry and add to the internal storage
*
* \return Pointer to copy info entry
*/
EC_T_CYC_COPY_INFO* CEcConfigData::CreateCopyInfoEntry(EC_T_VOID)
{
    EC_T_CYC_COPY_INFO* pCopyInfoEntry = EC_NULL;

    m_wNumCopyInfos++;
    /* allocate entries if needed */
    if (m_wNumCopyInfos > m_wNumCopyInfosAllocated)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_pCopyInfo", m_pCopyInfo, sizeof(EC_T_CYC_COPY_INFO) * m_wNumCopyInfosAllocated);
        m_wNumCopyInfosAllocated = (EC_T_WORD)(m_wNumCopyInfosAllocated + COPY_ENTRIES_ALLOC_COUNT);
        m_pCopyInfo = (EC_T_CYC_COPY_INFO*)ConfigDataReallocMem(m_pCopyInfo, sizeof(EC_T_CYC_COPY_INFO) * m_wNumCopyInfosAllocated);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::m_pCopyInfo", m_pCopyInfo, sizeof(EC_T_CYC_COPY_INFO) * m_wNumCopyInfosAllocated);
    }
    if (m_pCopyInfo != EC_NULL)
    {
        pCopyInfoEntry = &m_pCopyInfo[m_wNumCopyInfos - 1];
        OsMemset(pCopyInfoEntry, 0, sizeof(EC_T_CYC_COPY_INFO));
    }

    return pCopyInfoEntry;
}

#if (defined INCLUDE_VARREAD)
/********************************************************************************/
/** \brief Create swap info entry and add to the internal storage
*
* \return Pointer to swap info entry
*/
EC_T_CYC_SWAP_INFO* CEcConfigData::CreateSwapInfoEntry(EC_T_VOID)
{
    EC_T_CYC_SWAP_INFO* pSwapInfoEntry = EC_NULL;

    m_wNumSwapInfos++;
    /* allocate entries if needed */
    if (m_wNumSwapInfos > m_wNumSwapInfosAllocated)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::~m_pSwapInfo", m_pSwapInfo, sizeof(EC_T_CYC_SWAP_INFO) * m_wNumSwapInfosAllocated);
        m_wNumSwapInfosAllocated = (EC_T_WORD)(m_wNumSwapInfosAllocated + SWAP_ENTRIES_ALLOC_COUNT);
        m_pSwapInfo = (EC_T_CYC_SWAP_INFO*)ConfigDataReallocMem(m_pSwapInfo, sizeof(EC_T_CYC_SWAP_INFO) * m_wNumSwapInfosAllocated);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::m_pSwapInfo", m_pSwapInfo, sizeof(EC_T_CYC_SWAP_INFO) * m_wNumSwapInfosAllocated);
    }
    if (m_pSwapInfo != EC_NULL)
    {
        pSwapInfoEntry = &m_pSwapInfo[m_wNumSwapInfos - 1];
        OsMemset(pSwapInfoEntry, 0, sizeof(EC_T_CYC_SWAP_INFO));
    }

    return pSwapInfoEntry;
}
#endif /* INCLUDE_VARREAD */

/********************************************************************************/
/* Copied from ProbeMemberLAddr. TODO: merge ProbeMemberLAddr! */
EC_T_WORD GetCycCmdWkcIncrementLAddr(
    CEcSlave* pCfgSlave,
    EcCycCmdConfigDesc* pCmdConfigDesc,
    EC_T_BOOL bRead,
    EC_T_BOOL bWrite
    )
{
    EC_T_WORD                   wWkcIncrement = 0;
    EcInitCmdDesc*              pInitCmd = EC_NULL;
    EC_T_DWORD                  dwInitCmdIdx = 0;
    ETYPE_EC_CMD_HEADER*        pEcCmdHdr = EC_NULL;
    ETYPE_EC_T_FMMU_CFG_CMD*    pFmmuCmd = 0;

    ETYPE_EC_CMD_HEADER*        pEcCmdHdrParm = &(pCmdConfigDesc->EcCmdHeader);
    EC_T_DWORD                  dwLogicalAddr = EC_AL_ICMDHDR_GET_ADDR(pEcCmdHdrParm);
    EC_T_DWORD                  dwLength = EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHdrParm);

    for (dwInitCmdIdx = 0; dwInitCmdIdx < pCfgSlave->m_wInitCmdsCount; dwInitCmdIdx++)
    {
        pInitCmd = pCfgSlave->m_ppInitCmds[dwInitCmdIdx];
        pEcCmdHdr = &(pInitCmd->EcCmdHeader);

        /* FMMU write is always FPWR */
        if (EC_CMD_TYPE_FPWR == EC_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHdr))
        {

            /* write is to one of FMMU channels */
            EC_T_WORD wAdo = EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHdr);

            /* FMMU configuration within range of FMMU entries and at start of FMMU entry */
            if (((wAdo & 0x0600) == 0x0600) && ((wAdo & 0x000f) == 0))
            {
                pFmmuCmd = (ETYPE_EC_T_FMMU_CFG_CMD*)EcInitCmdDescData(pInitCmd);
                EC_T_WORD wFmmuLength = EC_ECFMMUCFGCMD_GETLENGTH(pFmmuCmd);
                EC_T_DWORD dwFmmuLogStartAddr = EC_ECFMMUCFGCMD_GETLOGSTARTADDR(pFmmuCmd);

                /* FMMU at given CycCmd */
                if ((wFmmuLength != 0) && !EC_IS_RANGE_OUT_OF_BOUNDS(dwFmmuLogStartAddr, dwFmmuLogStartAddr + wFmmuLength, dwLogicalAddr, dwLogicalAddr + dwLength))
                {
                    if ((bRead && ((pFmmuCmd->byType & FMMU_CFG_CMD_TYPE_READ) == FMMU_CFG_CMD_TYPE_READ)))
                    {
                        wWkcIncrement = 1;
                        break;
                    }
                    if ((bWrite && ((pFmmuCmd->byType & FMMU_CFG_CMD_TYPE_WRITE) == FMMU_CFG_CMD_TYPE_WRITE)))
                    {
                        wWkcIncrement = 1;
                        break;
                    }
                }
            }
        }
    }

    return wWkcIncrement;
}

EC_T_WORD CEcConfigData::GetCycCmdWkcIncrement(CEcSlave* pCfgSlave, EcCycCmdConfigDesc* pCmdDesc)
{
    EC_T_WORD wCycCmdWkcIncrement = 0;

    ETYPE_EC_CMD_HEADER* pEcCmdHdr = &(pCmdDesc->EcCmdHeader);

    switch (EC_AL_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHdr))
    {
    case EC_CMD_TYPE_APRD:
    case EC_CMD_TYPE_APWR:
    {
        /* Single counter increment if match */
        if (pCfgSlave->m_wAutoIncAddr == EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr))
        {
            wCycCmdWkcIncrement = 1;
        }
    } break;
    case EC_CMD_TYPE_APRW:
    {
        /* Counter increment by 3 if match */
        if (pCfgSlave->m_wAutoIncAddr == EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr))
        {
            wCycCmdWkcIncrement = 3;
        }
    } break;
    case EC_CMD_TYPE_FPRD:
    case EC_CMD_TYPE_FPWR:
    {
        /* Single counter increment if match */
        if (pCfgSlave->m_wFixedAddr == EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr))
        {
            wCycCmdWkcIncrement = 1;
        }
    } break;
    case EC_CMD_TYPE_FPRW:
    {
        /* Counter increment by 3 if match */
        if (pCfgSlave->m_wFixedAddr == EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr))
        {
            /* make entry */
            wCycCmdWkcIncrement = 3;
        }
    } break;
    case EC_CMD_TYPE_BRD:
    case EC_CMD_TYPE_BWR:
    {
        /* Single counter increment */
        wCycCmdWkcIncrement = /* pHCGroup->dwGrpMembers * */ 1;
    } break;
    case EC_CMD_TYPE_BRW:
    {
        /* Counter increment by 3 */
        wCycCmdWkcIncrement = /* pHCGroup->dwGrpMembers * */ 3;
    } break;
    case EC_CMD_TYPE_LRD:
    {
        /* Single counter increment if match */
        wCycCmdWkcIncrement = GetCycCmdWkcIncrementLAddr(pCfgSlave, pCmdDesc, EC_TRUE, EC_FALSE);
    } break;
    case EC_CMD_TYPE_LWR:
    {
        /* Single counter increment if match */
        wCycCmdWkcIncrement = GetCycCmdWkcIncrementLAddr(pCfgSlave, pCmdDesc, EC_FALSE, EC_TRUE);
    } break;
    case EC_CMD_TYPE_LRW:
    {
        /* Counter increment by 3 if match */
        wCycCmdWkcIncrement = GetCycCmdWkcIncrementLAddr(pCfgSlave, pCmdDesc, EC_TRUE, EC_FALSE);
        wCycCmdWkcIncrement = (EC_T_WORD)(wCycCmdWkcIncrement + GetCycCmdWkcIncrementLAddr(pCfgSlave, pCmdDesc, EC_FALSE, EC_TRUE) * 2);
    } break;
    case EC_CMD_TYPE_ARMW:
    {
        /* Single counter increment if match */
    } break;
    case EC_CMD_TYPE_FRMW:
    {
        /* Single counter increment if match */
    } break;
    case EC_CMD_TYPE_NOP: /* no entry */ break;
    case EC_CMD_TYPE_EXT:
    default:
    {
        /* don't know what to do ... ignore datagram */
        OsDbgAssert(EC_FALSE);
    } break;
    }

    return wCycCmdWkcIncrement;
}

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
/********************************************************************************/
/** \brief Create a configure hot connect group entry and add it into internal storage.
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::CreateHCGroup(
    EC_T_HC_GROUP_CFG_DESC** ppHCGroup) /**< [out] Hot connect group configuration object (optional) */
{
    EC_T_HC_GROUP_CFG_DESC* pNewHCGroup = EC_NULL;
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_BOOL bOk = EC_TRUE;

    /* new hot connect group entry */
    bOk = EC_TRUE;
    m_dwNumHCGroups++;
    if (m_dwNumHCGroups > m_dwHCGroupArraySize)
    {
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&m_apoHotConnectGroup, &m_dwHCGroupArraySize, C_dwCfgHcGroupAllocGranularity);
        if (!bOk)
        {
            m_dwNumHCGroups--;
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }
    /* create new hot connect group */
    pNewHCGroup = (EC_T_HC_GROUP_CFG_DESC*)ConfigDataAllocMem(sizeof(EC_T_HC_GROUP_CFG_DESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::CreateHCGroup", pNewHCGroup, sizeof(EC_T_HC_GROUP_CFG_DESC));
    if (EC_NULL == pNewHCGroup)
    {
        m_dwNumHCGroups--;
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pNewHCGroup, 0, sizeof(EC_T_HC_GROUP_CFG_DESC));

    m_apoHotConnectGroup[m_dwNumHCGroups - 1] = pNewHCGroup;
   
    if (EC_NULL != ppHCGroup)
        *ppHCGroup = pNewHCGroup;

Exit:
    return dwRetVal;
}
/********************************************************************************/
/** \brief Add a Slave to a Hot Connect Group and link slave with HC Group.
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::AddHCGroupMember(
    EC_T_HC_GROUP_CFG_DESC* pHCGroup,   /** <[in] Hot connect group configuration description */
    EC_T_ENI_SLAVE* pEniSlave)          /** <[in] Eni slave description */
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_DWORD dwCfgSlaveIdx = 0;
    EC_T_CONFIG_DATA_SLAVE* pCurCfgSlave = EC_NULL;

    OsDbgAssert(pHCGroup->dwNumGroupMembersListed > 0);
    OsDbgAssert(pHCGroup->dwNumGroupMembers <= pHCGroup->dwNumGroupMembersListed);

    if (0 == pHCGroup->dwNumGroupMembers)
    {
        pHCGroup->apGroupMember = (EC_T_ENI_SLAVE**)ConfigDataAllocMem(sizeof(EC_T_ENI_SLAVE*) * pHCGroup->dwNumGroupMembersListed);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_DATA, "CEcConfigData::AddHCGroupMember",
            pHCGroup->apGroupMember, sizeof(EC_T_ENI_SLAVE*) * pHCGroup->dwNumGroupMembersListed);
        if (EC_NULL == pHCGroup->apGroupMember)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    pHCGroup->apGroupMember[pHCGroup->dwNumGroupMembers++] = pEniSlave;

    if (pHCGroup->dwNumGroupMembers == pHCGroup->dwNumGroupMembersListed)
        pHCGroup->bIsGroupComplete = EC_TRUE;

    /* link hot connect group to slave */
    for (dwCfgSlaveIdx = m_dwNumCfgDataSlaves - 1; dwCfgSlaveIdx >= 0; dwCfgSlaveIdx--)
    {
        pCurCfgSlave = m_apoCfgDataSlave[dwCfgSlaveIdx];

        if (pEniSlave == pCurCfgSlave->pEniSlave)
        {
            pCurCfgSlave->pHotConnectGroupLink = pEniSlave;
            dwRetVal = EC_E_NOERROR;
            break;
        }
    }

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief ProbeMemberAInc. Helper for ReadHCGroup
*
* \return
*/
EC_T_DWORD ProbeMemberAInc(
    EC_T_HOTCONNECT_GROUP* pGroup,  /**< [in] Pointer to hot connect group */
    EC_T_WORD wAIncAddr             /**< [in] */
    )
{
    EC_T_DWORD  dwEntryMatches = 0;
    EC_T_DWORD  dwScanEntries = 0;
    CEcSlave*   poCurSlave = EC_NULL;

    for (dwScanEntries = 0; dwScanEntries < pGroup->dwGrpMembers; dwScanEntries++)
    {
        poCurSlave = pGroup->ppGroupMembers[dwScanEntries];
        if (poCurSlave->m_wAutoIncAddr == wAIncAddr)
        {
            dwEntryMatches++;
        }
    }

    return dwEntryMatches;
}
/********************************************************************************/
/** \brief ProbeMemberFAddr. Helper for ReadHCGroup
*
* \return
*/

EC_T_DWORD ProbeMemberFAddr(
    EC_T_HOTCONNECT_GROUP* pGroup,  /**< [in] Pointer to hot connect group */
    EC_T_WORD wFixedAddr            /**< [in] */
    )
{
    EC_T_DWORD  dwEntryMatches  = 0;
    EC_T_DWORD  dwScanEntries   = 0;
    CEcSlave*   poCurSlave      = EC_NULL;

    for (dwScanEntries = 0; dwScanEntries < pGroup->dwGrpMembers; dwScanEntries++)
    {
        poCurSlave = pGroup->ppGroupMembers[dwScanEntries];
        if (poCurSlave->m_wFixedAddr == wFixedAddr)
        {
            dwEntryMatches++;
        }
    }

    return dwEntryMatches;
}

/********************************************************************************/
/** \brief ProbeMemberLAddr. Helper for ReadHCGroup
*
* \return
*/
EC_T_DWORD ProbeMemberLAddr(
    EC_T_HOTCONNECT_GROUP* pGroup,      /**< [in] Pointer to hot connect group */
    EcCycCmdConfigDesc* pCmdConfigDesc, /**< [in] Pointer to cyclic command config */
    EC_T_DWORD dwLogicalAddr,           /**< [in] */
    EC_T_DWORD dwLength,                /**< [in] */
    EC_T_BOOL bRead,                    /**< [in] */
    EC_T_BOOL bWrite                    /**< [in] */
    )
{
    EC_T_DWORD                  dwEntryMatches = 0;
    EC_T_DWORD                  dwScanEntries = 0;
    CEcSlave*                   poCurSlave = EC_NULL;
    PEcInitCmdDesc              pInitCmd = EC_NULL;
    EC_T_DWORD                  dwInitCmdIdx = 0;
    PETYPE_EC_CMD_HEADER        pEcCmdHdr       = EC_NULL;
    ETYPE_EC_T_FMMU_CFG_CMD*    pCurFmmuCmd = 0;
    EC_T_BOOL                   bThisSlaveDone = EC_FALSE;

    for (dwScanEntries = 0; dwScanEntries < pGroup->dwGrpMembers; dwScanEntries++)
    {
        poCurSlave = pGroup->ppGroupMembers[dwScanEntries];
        if (EC_NULL == poCurSlave)
        {
            /* TODO: ASSERT slave excluded by emConfigExcludeSlave() */
            continue;
        }

        bThisSlaveDone = EC_FALSE;
        for (dwInitCmdIdx = 0; (dwInitCmdIdx < poCurSlave->m_wInitCmdsCount) && !bThisSlaveDone; dwInitCmdIdx++)
        {
            pInitCmd = poCurSlave->m_ppInitCmds[dwInitCmdIdx];

            pEcCmdHdr = &(pInitCmd->EcCmdHeader);

            /* FMMU write is always FPWR */
            if (EC_CMD_TYPE_FPWR == EC_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHdr))
            {
                /* write is to one of FMMU channels */
                EC_T_WORD wAdo = EC_ICMDHDR_GET_ADDR_ADO(pEcCmdHdr);

                if (
                    ((wAdo & 0x0600) == 0x0600)   /* is in range of FMMU entries */
                    && ((wAdo & 0x000f) == 0)        /* start of FMMU entry */
                    )
                {
                    /* FMMU configuration entry found */
                    pCurFmmuCmd = (ETYPE_EC_T_FMMU_CFG_CMD*)EcInitCmdDescData(pInitCmd);
                    EC_T_WORD wFmmuLength = EC_ECFMMUCFGCMD_GETLENGTH(pCurFmmuCmd);
                    EC_T_DWORD dwFmmuLogStartAddr = EC_ECFMMUCFGCMD_GETLOGSTARTADDR(pCurFmmuCmd);

                    /* entry match if FMMU is within or overlapping dwLogicalAddr + dwLength */
                    if ((wFmmuLength != 0) && !EC_IS_RANGE_OUT_OF_BOUNDS(dwFmmuLogStartAddr, dwFmmuLogStartAddr + wFmmuLength, dwLogicalAddr, dwLogicalAddr + dwLength))
                    {
                        if ((bRead && ((pCurFmmuCmd->byType & FMMU_CFG_CMD_TYPE_READ) == FMMU_CFG_CMD_TYPE_READ)))
                        {
#if (defined INCLUDE_WKCSTATE)
                            if (EC_CFG_SLAVE_PD_SECTIONS > poCurSlave->m_wWkcStateInEntries)
                            {
                                poCurSlave->m_awWkcStateDiagOffsIn[poCurSlave->m_wWkcStateInEntries] = pCmdConfigDesc->wWkcStateDiagOffs;
                                poCurSlave->m_wWkcStateInEntries++;
                            }
                            else
                            {
                                OsDbgAssert(EC_FALSE);
                            }
#else
                            EC_UNREFPARM(pCmdConfigDesc);
#endif
                            dwEntryMatches++;
                            bThisSlaveDone = EC_TRUE;
                        }
                        if ((bWrite && ((pCurFmmuCmd->byType & FMMU_CFG_CMD_TYPE_WRITE) == FMMU_CFG_CMD_TYPE_WRITE)))
                        {
#if (defined INCLUDE_WKCSTATE)
                            if (EC_CFG_SLAVE_PD_SECTIONS > poCurSlave->m_wWkcStateOutEntries)
                            {
                                poCurSlave->m_awWkcStateDiagOffsOut[poCurSlave->m_wWkcStateOutEntries] = pCmdConfigDesc->wWkcStateDiagOffs;
                                poCurSlave->m_wWkcStateOutEntries++;
                            }
                            else
                            {
                                OsDbgAssert(EC_FALSE);
                            }
#endif
                            dwEntryMatches++;
                            bThisSlaveDone = EC_TRUE;
                        }
                    } /* matches range */
                } /* is FMMU entry */
            } /* is write */
        } /* through each initcmd of slave */
    } /* through all group members */

    return dwEntryMatches;
}

#if (defined INCLUDE_HOTCONNECT)
/***************************************************************************************************/
/**
\brief  Helper to parse identification value and Ado from hot connect identify commands
*
\return EC_E_NOERROR on success, error code otherwise.
*/
static EC_T_DWORD ParseHCIdentifyCmds(EC_T_HOTCONNECT_GROUP* pHCGroup, EC_T_ENI_SLAVE* pEniSlave)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_DWORD dwIdx = 0;
    EC_T_BOOL  bALCtlFound = EC_FALSE;

    for (dwIdx = 0; dwIdx < pHCGroup->dwIdentCmdsCount && EC_E_NOERROR == dwRetVal; dwIdx++)
    {
        EcInitCmdDesc* pIdentCmd = pHCGroup->paIdentCmds[dwIdx];

        if (EC_NULL == pIdentCmd)
        {
            OsDbgAssert(EC_FALSE);
            continue;
        }
        /* parse cmd */
        switch (EC_ICMDHDR_GET_ADDR_ADO(&pIdentCmd->EcCmdHeader))
        {
            case ECREG_AL_CONTROL:
            {
                bALCtlFound = EC_TRUE;
            } break;
            case ECREG_AL_STATUS:
            {
                if (bALCtlFound)
                {
                    /* request id mechanism, ado = 0x134 */
                    pEniSlave->wIdentificationAdo = ECREG_AL_STATUSCODE;
                    pEniSlave->wIdentificationValue = EC_GET_FRM_WORD(EcInitCmdDescVData(pIdentCmd) + 4);
                }
                else
                {
                    EC_ERRORMSG(("CEcDeviceFactory: hot connect request device identify command missing\n"));
                    dwRetVal = EC_E_INVALIDCMD;
                    goto Exit;
                }
            } break;
            case ECREG_STATION_ADDRESS:
            {
                /* check previous address */
                pEniSlave->wHCIdentifyAddrPrev = EC_GET_FRM_WORD(EcInitCmdDescVData(pIdentCmd));
            } break;
            default:
            {
                if (0 != ((EC_CMD_TYPE_APRD | EC_CMD_TYPE_FPRD) & EC_ICMDHDR_GET_CMDIDX_CMD(&pIdentCmd->EcCmdHeader)))
                {
                    /* direct id mechanism, ado = 0x12, 0x1000 ... */
                    pEniSlave->wIdentificationAdo = EC_ICMDHDR_GET_ADDR_ADO(&pIdentCmd->EcCmdHeader);
                    pEniSlave->wIdentificationValue = EC_GET_FRM_WORD(EcInitCmdDescVData(pIdentCmd));
                }
                else
                {
                    EC_ERRORMSG(("CEcDeviceFactory: Unknown hot connect identify command\n"));
                    dwRetVal = EC_E_NOTSUPPORTED;
                    goto Exit;
                }
            }break;
        }
    }

Exit:
    return dwRetVal;
}
#endif

/***************************************************************************************************/
/**
\brief  Read HC Group Configuration to HC Group Structure
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcConfigData::ReadHCGroup(
    EC_T_VOID*  pvGroup,      /**< [in]   Buffer holding HC Group element */
    EC_T_VOID*  pvMaster,     /**< [in]   Pointer to Master */
    EC_T_DWORD  dwGrpIdx      /**< [in]   Index of HC Group to read to pGroup */
    )
{
EC_T_DWORD              dwRetVal     = EC_E_ERROR;
#if (defined INCLUDE_HOTCONNECT)
EC_T_DWORD              dwRes        = EC_E_ERROR;
#endif /* INCLUDE_HOTCONNECT */
EC_T_HOTCONNECT_GROUP*  pHCGroup     = (EC_T_HOTCONNECT_GROUP*)pvGroup;
EC_T_HC_GROUP_CFG_DESC* pCfgHCGroup  = EC_NULL;
EC_T_BOOL               bIsMandatory = (0 == dwGrpIdx);
CEcMaster*              poEcMaster   = ((CEcMaster*)pvMaster);

    if (EC_NULL == pvGroup || EC_NULL == poEcMaster)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if (dwGrpIdx > m_dwNumHCGroups)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    /* select config data for mandatory or optional group */
    if (!bIsMandatory)
    {
        pCfgHCGroup = m_apoHotConnectGroup[dwGrpIdx - 1];
    }
    /* determine group type */
    pHCGroup->oHCGroupType = (bIsMandatory?hcgrouptype_mandatory:hcgrouptype_optional);

    /* determine number of group members */
    {
        if (bIsMandatory)
        {
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < m_dwNumCfgDataSlaves; dwSlaveIdx++)
            {
            EC_T_CONFIG_DATA_SLAVE* pCfgDataSlave = GetCfgDataSlaveByIndex(dwSlaveIdx);

                if ((0 == pCfgDataSlave->pEniSlave->wPhysAddr) || pCfgDataSlave->bExcluded)
                {
                    continue;
                }
                if (EC_NULL != pCfgDataSlave->pHotConnectGroupLink)
                {
                    continue;
                }
                pHCGroup->dwGrpMembers++;
            }
        }
        else
        {
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < pCfgHCGroup->dwNumGroupMembers; dwSlaveIdx++)
            {
            EC_T_CONFIG_DATA_SLAVE* pCfgDataSlave = GetCfgDataSlaveByFixedAddr(pCfgHCGroup->apGroupMember[dwSlaveIdx]->wPhysAddr);

                if ((0 == pCfgDataSlave->pEniSlave->wPhysAddr) || pCfgDataSlave->bExcluded)
                {
                    continue;
                }
                pHCGroup->dwGrpMembers++;
            }
        }
    }
    /* create group members list */
    if (0 != pHCGroup->dwGrpMembers)
    {
    CEcSlave*  poSlave       = EC_NULL;
    EC_T_DWORD dwMemberIndex = 0;

        /* allocate group members list */
        pHCGroup->ppGroupMembers = (CEcSlave**)OsMalloc(sizeof(EC_T_VOID*)*pHCGroup->dwGrpMembers);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcDeviceFactory::pHCGroup->ppGroupMembers", pHCGroup->ppGroupMembers, sizeof(EC_T_VOID*)*pHCGroup->dwGrpMembers);
        if (EC_NULL == pHCGroup->ppGroupMembers)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(pHCGroup->ppGroupMembers, 0, (sizeof(EC_T_VOID*)*pHCGroup->dwGrpMembers));

        /* fill group members list */
        if (bIsMandatory)
        {
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < m_dwNumCfgDataSlaves; dwSlaveIdx++)
            {
            EC_T_CONFIG_DATA_SLAVE* pCfgDataSlave = GetCfgDataSlaveByIndex(dwSlaveIdx);

                if ((0 == pCfgDataSlave->pEniSlave->wPhysAddr) || pCfgDataSlave->bExcluded)
                {
                    continue;
                }
                if (EC_NULL != pCfgDataSlave->pHotConnectGroupLink)
                {
                    continue;
                }
                poSlave = poEcMaster->GetSlaveByAddr(pCfgDataSlave->pEniSlave->wPhysAddr);
                OsDbgAssert(EC_NULL != poSlave);
                if (poSlave != EC_NULL)
                {
                    poSlave->m_bIsOptional = EC_FALSE;
                    poSlave->m_dwHCGroupId = 0;
                    poSlave->m_bIsGHCGroupHead = EC_FALSE;
                    pHCGroup->ppGroupMembers[dwMemberIndex++] = poSlave;
                }
            }
        }
        else
        {
            for (EC_T_DWORD dwSlaveIdx = 0; dwSlaveIdx < pCfgHCGroup->dwNumGroupMembers; dwSlaveIdx++)
            {
            EC_T_CONFIG_DATA_SLAVE* pCfgDataSlave = GetCfgDataSlaveByFixedAddr(pCfgHCGroup->apGroupMember[dwSlaveIdx]->wPhysAddr);

                if ((0 == pCfgDataSlave->pEniSlave->wPhysAddr) || pCfgDataSlave->bExcluded)
                {
                    continue;
                }
                poSlave = poEcMaster->GetSlaveByAddr(pCfgDataSlave->pEniSlave->wPhysAddr);
                if (poSlave != EC_NULL)
                {
                    poSlave->m_bIsOptional = EC_TRUE;
                    poSlave->m_dwHCGroupId = dwGrpIdx;
                    poSlave->m_bIsGHCGroupHead = (0 == dwMemberIndex);
                    pHCGroup->ppGroupMembers[dwMemberIndex++] = poSlave;
                }
            }
            /* get PreviousPort entry from slave */
            if ((0 != pCfgHCGroup->dwNumGroupMembers) && (0 != pCfgHCGroup->apGroupMember[0]->wNumPreviousPorts))
            {
                pHCGroup->dwNumPreviousPorts = pCfgHCGroup->apGroupMember[0]->wNumPreviousPorts;
                pHCGroup->apoPreviousPort = pCfgHCGroup->apGroupMember[0]->aoPreviousPort;
            }
        }
    }
    /* Allocate memory for CycCmdWkcList */
    {
    EC_T_WORD wNumCmds = poEcMaster->GetCycConfigEntriesNumCmds();
    
        if (bIsMandatory)
        {
            /* only store dgrams within first, mandatory group */
            poEcMaster->CreateCycCmdHC(wNumCmds);
        }
        /* allocate memory for CycCmdWkcList */
        OsDbgAssert(pHCGroup->aCycCmdWkcList == EC_NULL);
        pHCGroup->dwMaxNumCycCmdWkcList = wNumCmds + 1;
        pHCGroup->aCycCmdWkcList = (EC_T_CYC_CMD_WKC_DESC_ENTRY*)OsMalloc(pHCGroup->dwMaxNumCycCmdWkcList * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcDeviceFactory::pHCGroup->aCycCmdWkcList", pHCGroup->aCycCmdWkcList, 
            pHCGroup->dwMaxNumCycCmdWkcList * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY));
        if (EC_NULL == pHCGroup->aCycCmdWkcList)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(pHCGroup->aCycCmdWkcList, 0, (pHCGroup->dwMaxNumCycCmdWkcList * sizeof(EC_T_CYC_CMD_WKC_DESC_ENTRY)));
    }
    /* now go through cylic commands and check for assignment within this group */
    {
    EC_T_DWORD           dwCycEntryIndex = 0;
    EcCycCmdConfigDesc*  pCmdConfigDesc = EC_NULL;
    ETYPE_EC_CMD_HEADER* pEcCmdHdr = EC_NULL;
    EC_T_DWORD           dwWkcIncrement = 0;
    EC_T_DWORD           dwGrpCycCmdWkcEntryCnt = 0;

#ifdef INCLUDE_MULTIPLE_CYC_ENTRIES
        for (dwCycEntryIndex = 0; dwCycEntryIndex < poEcMaster->GetNumCycConfigEntries(); dwCycEntryIndex++)
#else
        dwCycEntryIndex = 0;
#endif
        {
            CYCLIC_ENTRY_MGMT_DESC* pCycEntry = poEcMaster->GetCycEntryDesc((EC_T_INT)dwCycEntryIndex);
            EC_T_WORD wCmdsInThisEntry = pCycEntry->wTotalNumCmds;

            for (EC_T_WORD wCmdIdx = 0; wCmdIdx < wCmdsInThisEntry; wCmdIdx++)
            {
                pCmdConfigDesc = pCycEntry->apEcAllCycCmdConfigDesc[wCmdIdx];

                dwWkcIncrement = 0;

                pEcCmdHdr = &(pCmdConfigDesc->EcCmdHeader);

                /* only store dgrams within first, mandatory group */
                if (bIsMandatory)
                {
                    poEcMaster->AddCycCmdHC(pCmdConfigDesc);
                }

                switch (EC_AL_ICMDHDR_GET_CMDIDX_CMD(pEcCmdHdr))
                {
                case EC_CMD_TYPE_APRD:
                case EC_CMD_TYPE_APWR:
                {
                    /* Single counter increment if match */
                    if (0 < ProbeMemberAInc(pHCGroup, EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr)))
                    {
                        /* make entry */
                        dwWkcIncrement = 1;
                    }
                } break;
                case EC_CMD_TYPE_APRW:
                {
                    /* Counter increment by 3 if match */
                    if (0 < ProbeMemberAInc(pHCGroup, EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr)))
                    {
                        /* make entry */
                        dwWkcIncrement = 3;
                    }
                } break;
                case EC_CMD_TYPE_FPRD:
                case EC_CMD_TYPE_FPWR:
                {
                    /* Single counter increment if match */
                    if (0 < ProbeMemberFAddr(pHCGroup, EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr)))
                    {
                        /* make entry */
                        dwWkcIncrement = 1;
                    }
                } break;
                case EC_CMD_TYPE_FPRW:
                {
                    /* Counter increment by 3 if match */
                    if (0 < ProbeMemberFAddr(pHCGroup, EC_AL_ICMDHDR_GET_ADDR_ADP(pEcCmdHdr)))
                    {
                        /* make entry */
                        dwWkcIncrement = 3;
                    }
                } break;
                case EC_CMD_TYPE_BRD:
                case EC_CMD_TYPE_BWR:
                {
                    /* Single counter increment */
                    dwWkcIncrement = pHCGroup->dwGrpMembers * 1;
                } break;
                case EC_CMD_TYPE_BRW:
                {
                    /* Counter increment by 3 */
                    dwWkcIncrement = pHCGroup->dwGrpMembers * 3;
                } break;
                case EC_CMD_TYPE_LRD:
                {
                    /* Single counter increment if match */
                    EC_T_DWORD dwLogicalAddr = EC_AL_ICMDHDR_GET_ADDR(pEcCmdHdr);
                    EC_T_DWORD dwLength = EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHdr);
                    dwWkcIncrement = ProbeMemberLAddr(pHCGroup, pCmdConfigDesc, dwLogicalAddr, dwLength, EC_TRUE, EC_FALSE) * 1;
                } break;
                case EC_CMD_TYPE_LWR:
                {
                    /* Single counter increment if match */
                    dwWkcIncrement = ProbeMemberLAddr(pHCGroup, pCmdConfigDesc, EC_AL_ICMDHDR_GET_ADDR(pEcCmdHdr), EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHdr), EC_FALSE, EC_TRUE) * 1;
                } break;
                case EC_CMD_TYPE_LRW:
                {
                    /* Counter increment by 3 if match */
                    dwWkcIncrement = ProbeMemberLAddr(pHCGroup, pCmdConfigDesc, EC_AL_ICMDHDR_GET_ADDR(pEcCmdHdr), EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHdr), EC_TRUE, EC_FALSE) * 1;
                    dwWkcIncrement += ProbeMemberLAddr(pHCGroup, pCmdConfigDesc, EC_AL_ICMDHDR_GET_ADDR(pEcCmdHdr), EC_AL_ICMDHDR_GET_LEN_LEN(pEcCmdHdr), EC_FALSE, EC_TRUE) * 2;
                } break;
                case EC_CMD_TYPE_ARMW:
                {
                    /* Single counter increment if match */
                } break;
                case EC_CMD_TYPE_FRMW:
                {
                    /* Single counter increment if match */
                } break;
                case EC_CMD_TYPE_NOP: /* no entry */ break;
                case EC_CMD_TYPE_EXT:
                default:
                {
                    /* don't know what to do ... ignore datagram */
                    OsDbgAssert(EC_FALSE);
                } break;
                }

                /* Add CycCmdWkcList entry if WkcIncrement != 0 */
                if (0 != dwWkcIncrement)
                {
                    pHCGroup->aCycCmdWkcList[dwGrpCycCmdWkcEntryCnt].pCycCmdCfgDesc = pCmdConfigDesc;
                    pHCGroup->aCycCmdWkcList[dwGrpCycCmdWkcEntryCnt].wWkcIncrement = EC_LOWORD(dwWkcIncrement);

                    if ((pHCGroup->oHCGroupType) != hcgrouptype_mandatory)
                    {
                        pCmdConfigDesc->bIsHcSlaveInfluence = EC_TRUE;
                    }

                    /* mandatory group is not optional :-) */
                    pHCGroup->aCycCmdWkcList[dwGrpCycCmdWkcEntryCnt].bOptional = (((pHCGroup->oHCGroupType) != hcgrouptype_mandatory) ? EC_TRUE : EC_FALSE);

                    /* assume all slaves from mandatory groups are present = active, because Active Flag is discarded anyway, optional
                    * modules are assumed to be absent by default */
                    pHCGroup->aCycCmdWkcList[dwGrpCycCmdWkcEntryCnt].bActive = (((pHCGroup->oHCGroupType) == hcgrouptype_mandatory) ? EC_TRUE : EC_FALSE);

                    dwGrpCycCmdWkcEntryCnt++;
                }
            } /* for( EC_T_WORD wCmdIdx = 0; wCmdIdx < wCmdsInThisEntry; wCmdIdx++ ) */
        } /* for(dwCycEntryIndex=0; dwCycEntryIndex < poEcMaster->m_dwNumCycConfigEntries; dwCycEntryIndex++) */
    }

    /* Group check cmd */
    /* no group ident for default group */
    if (!bIsMandatory)
    {
        /* get identify pointer array */
        pHCGroup->paIdentCmds = (EcInitCmdDesc**)OsMalloc((pCfgHCGroup->wNumEcatIdentifyCmds) * sizeof(EcInitCmdDesc*));
        EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcDeviceFactory::pHCGroup->paIdentCmds", pHCGroup->paIdentCmds, (pCfgHCGroup->wNumEcatIdentifyCmds) * sizeof(EcInitCmdDesc*));
        if (EC_NULL == pHCGroup->paIdentCmds)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
        OsMemset(pHCGroup->paIdentCmds, 0, (pCfgHCGroup->wNumEcatIdentifyCmds) * sizeof(EcInitCmdDesc*));
        pHCGroup->dwIdentCmdsCount = pCfgHCGroup->wNumEcatIdentifyCmds;

        for (EC_T_DWORD nIdentCmdIdx = 0; nIdentCmdIdx < pCfgHCGroup->wNumEcatIdentifyCmds; nIdentCmdIdx++)
        {
            /* place init command in slave command list */
            pHCGroup->paIdentCmds[nIdentCmdIdx] = pCfgHCGroup->apPkdEcatIdentifyCmdDesc[nIdentCmdIdx];;

            /* always use fixed addressing */
            switch (EC_ICMDHDR_GET_CMDIDX_CMD(&(pHCGroup->paIdentCmds[nIdentCmdIdx]->EcCmdHeader)))
            {
            case EC_CMD_TYPE_APRD: /* change to fixed addressing -> EC_CMD_TYPE_FPRD */
            case EC_CMD_TYPE_FPRD:
            {
                pHCGroup->paIdentCmds[nIdentCmdIdx]->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPRD;
            } break;
            case EC_CMD_TYPE_APWR: /* change to fixed addressing -> EC_CMD_TYPE_FPWR */
            case EC_CMD_TYPE_FPWR:
            {
                pHCGroup->paIdentCmds[nIdentCmdIdx]->EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = EC_CMD_TYPE_FPWR;
            } break;
            default:
            {
                /* don't know what to do, leave untouched */
                OsDbgAssert(EC_FALSE);
            } break;
            }
        }
#if (defined INCLUDE_HOTCONNECT)
        /* parse identify commands and set identification-ado, -value in eni slave */
	   if ((EC_NULL != pHCGroup->ppGroupMembers) && (EC_NULL != pHCGroup->ppGroupMembers[0]))
        {
			dwRes = ParseHCIdentifyCmds(pHCGroup, pHCGroup->ppGroupMembers[0]->m_pEniSlave);
			if (EC_E_NOERROR != dwRes)
            {
				dwRetVal = dwRes;
                goto Exit;
            }
        }
#endif
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:

#if defined VERBOSE_DEBUG
    {
        EC_T_DWORD dwScan = 0;

        EC_DBGMSG("HC Group : %d, Members : %d\n", dwGrpIdx, pHCGroup->dwGrpMembers);
        if (EC_NULL != pHCGroup->pEcHcGroupIdentCmd)
        {
            EC_DBGMSG("Group Ident: %s (%d) ADP/ADO:0x%04x/0x%04x Len: %d\n",
                EcatCmdShortText(EC_AL_ICMDHDR_GET_CMDIDX_CMD(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader))),
                EC_AL_ICMDHDR_GET_CMDIDX_CMD(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR_ADP(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR_ADO(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_LEN_LEN(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader))
                );
            if (EC_ECINITCMDDESC_GET_VALIDATE((pHCGroup->pEcHcGroupIdentCmd)))
            {
                EC_DBGMSG("\t");
                for (EC_T_DWORD dwValIdx = 0; dwValIdx < EC_AL_ICMDHDR_GET_LEN_LEN(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader)); dwValIdx++)
                {
                    EC_DBGMSG("%02X ", EcInitCmdDescVData(pHCGroup->pEcHcGroupIdentCmd)[dwValIdx]);
                    if ((0 != dwValIdx) && (0 == (dwValIdx % 8))) EC_DBGMSG(" ");
                    if ((0 != dwValIdx) && (0 == (dwValIdx % 16))) EC_DBGMSG("\n\t");
                }
                EC_DBGMSG("\n");
            }

            if (EC_ECINITCMDDESC_GET_VALIDATEMASK((pHCGroup->pEcHcGroupIdentCmd)))
            {
                EC_DBGMSG("&\t");
                for (EC_T_DWORD dwMaskIdx = 0; dwMaskIdx < EC_AL_ICMDHDR_GET_LEN_LEN(&(pHCGroup->pEcHcGroupIdentCmd->EcCmdHeader)); dwMaskIdx++)
                {
                    EC_DBGMSG("%02X ", EcInitCmdDescVMData(pHCGroup->pEcHcGroupIdentCmd)[dwMaskIdx]);
                    if ((0 != dwMaskIdx) && (0 == (dwMaskIdx % 8))) EC_DBGMSG(" ");
                    if ((0 != dwMaskIdx) && (0 == (dwMaskIdx % 16))) EC_DBGMSG("\n\t");
                }
                EC_DBGMSG("\n");
            }
        }

        for (dwScan = 0; dwScan < pHCGroup->dwGrpMembers; dwScan++)
        {
            EC_DBGMSG("Group Member: 0x%lx = %s / %d\n",
                pHCGroup->ppGroupMembers[dwScan],
                pHCGroup->ppGroupMembers[dwScan]->GetName(),
                pHCGroup->ppGroupMembers[dwScan]->m_wFixedAddr
                );
        }
        EC_DBGMSG("WKC Influence:\n");
        dwScan = 0;
        while (EC_NULL != pHCGroup->aCycCmdWkcList[dwScan].pCmd)
        {
            EC_DBGMSG("Cmd: %s (%02d) 0x%08lx (0x%04x/0x%04x) : Opt=%01d, Act=%01d; WKCI : %02d \n",
                EcatCmdShortText(EC_AL_ICMDHDR_GET_CMDIDX_CMD(&(pHCGroup->aCycCmdWkcList[dwScan].pCmd->EcCmdHeader))),
                EC_AL_ICMDHDR_GET_CMDIDX_CMD(&(pHCGroup->aCycCmdWkcList[dwScan].pCmd->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR(&(pHCGroup->aCycCmdWkcList[dwScan].pCmd->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR_ADP(&(pHCGroup->aCycCmdWkcList[dwScan].pCmd->EcCmdHeader)),
                EC_AL_ICMDHDR_GET_ADDR_ADO(&(pHCGroup->aCycCmdWkcList[dwScan].pCmd->EcCmdHeader)),
                pHCGroup->aCycCmdWkcList[dwScan].bOptional,
                pHCGroup->aCycCmdWkcList[dwScan].bActive,
                pHCGroup->aCycCmdWkcList[dwScan].wWkcIncrement
                );
            dwScan++;
        }
    }
#endif

    return dwRetVal;
}
#endif /*(defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)*/

/***************************************************************************************************/
/**
\brief  Return evaluated Free Address Ranges.
\return Pointer to address range on success, EC_NULL otherwise.
*/
ECX_T_ADDRRANGE* CEcConfigData::GetAddressRange(
    EC_T_DWORD          dwIdx  /**< [in]   Get Address range at index */
    )
{
    ECX_T_ADDRRANGE* pRange = EC_NULL;

    if (dwIdx > ECX_ADDRRANGE_AMOUNT)
    {
        goto Exit;
    }

    pRange = &(m_abHugeRanges[dwIdx]);

Exit:
    return pRange;
}

/***************************************************************************************************/
/**
\brief  Remove used slave address from range of free usable addresses.
*/
EC_T_VOID CEcConfigData::RemoveAddressFromRange(
    EC_T_WORD   wAddress    /**< [in]   Slave address, which is in use by ENI configuration */
    )
{
    EC_T_DWORD  dwRangeIdx = 0;

    /* search for a fitting range */
    for (dwRangeIdx = 0; dwRangeIdx < ECX_ADDRRANGE_AMOUNT; dwRangeIdx++)
    {
        /* address lies within this range mark ? */
        if (m_abHugeRanges[dwRangeIdx].wBegin <= wAddress && m_abHugeRanges[dwRangeIdx].wEnd >= wAddress)
        {
            if (m_abHugeRanges[dwRangeIdx].wBegin == wAddress)
            {
                /* is begin */
                m_abHugeRanges[dwRangeIdx].wBegin++;

                if (m_abHugeRanges[dwRangeIdx].wBegin > m_abHugeRanges[dwRangeIdx].wEnd)
                {
                    /* range is empty, kill it */
                    OsMemset(&(m_abHugeRanges[dwRangeIdx]), 0, sizeof(ECX_T_ADDRRANGE));
                }
            }
            else if (m_abHugeRanges[dwRangeIdx].wEnd == wAddress)
            {
                /* is end */
                m_abHugeRanges[dwRangeIdx].wEnd--;

                if (m_abHugeRanges[dwRangeIdx].wBegin > m_abHugeRanges[dwRangeIdx].wEnd)
                {
                    /* range is empty, kill it */
                    OsMemset(&(m_abHugeRanges[dwRangeIdx]), 0, sizeof(ECX_T_ADDRRANGE));
                }
            }
            else
            {
                EC_T_WORD   wBegin1 = 0;
                EC_T_WORD   wBegin2 = 0;
                EC_T_WORD   wEnd1 = 0;
                EC_T_WORD   wEnd2 = 0;
                EC_T_DWORD  dwScanIdx = 0;
                EC_T_DWORD  dwStoreIdx = 0;
                EC_T_WORD   wTmpB = 0;
                EC_T_WORD   wTmpE = 0;

                /* is in middle */

                /* split ranges */
                wBegin1 = m_abHugeRanges[dwRangeIdx].wBegin;
                wEnd1 = (EC_T_WORD)(wAddress - 1);
                wBegin2 = (EC_T_WORD)(wAddress + 1);
                wEnd2 = m_abHugeRanges[dwRangeIdx].wEnd;

                /* clear ranges, if they are empty */
                if (wEnd1 < wBegin1)
                {
                    wEnd1 = wBegin1 = 0;
                }

                if (wEnd2 < wBegin2)
                {
                    wEnd2 = wBegin2 = 0;
                }

                if ((wEnd1 - wBegin1) < (wEnd2 - wBegin2))
                {
                    wTmpB = wBegin2;
                    wTmpE = wEnd2;
                    wBegin2 = wBegin1;
                    wEnd2 = wEnd1;
                    wBegin1 = wTmpB;
                    wEnd1 = wTmpE;
                }

#define WDELTA1     ((wEnd1)-(wBegin1))
#define WDELTA2     ((wEnd2)-(wBegin2))
#define WDELTASCAN  ((m_abHugeRanges[dwScanIdx].wEnd)-(m_abHugeRanges[dwScanIdx].wBegin))
#define WDELTASTORE ((m_abHugeRanges[dwStoreIdx].wEnd)-(m_abHugeRanges[dwStoreIdx].wBegin))

                dwStoreIdx = ((EC_T_DWORD)-1);

                /* seek if create ranges are larger than the smallest stored entry, and replace the smallest one */
                for (dwScanIdx = 0; dwScanIdx < ECX_ADDRRANGE_AMOUNT; dwScanIdx++)
                {
                    /* do not overwrite currently changed entry here */
                    if (dwScanIdx == dwRangeIdx)
                        continue;

                    if (WDELTASCAN < WDELTA1)
                    {
                        if (((EC_T_DWORD)-1) == dwStoreIdx)
                        {
                            dwStoreIdx = dwScanIdx;
                        }
                        else if (WDELTASTORE < WDELTASCAN)
                        {
                            dwStoreIdx = dwScanIdx;
                        }
                    }
                }

                /* dwStoreIdx should store smallest entry, which is the one to overwrite */
                if (((EC_T_DWORD)-1) == dwStoreIdx)
                {
                    dwStoreIdx = dwRangeIdx;
                }

                /* clean old entry */
                OsMemset(&(m_abHugeRanges[dwRangeIdx]), 0, sizeof(ECX_T_ADDRRANGE));

                m_abHugeRanges[dwStoreIdx].wBegin = wBegin1;
                m_abHugeRanges[dwStoreIdx].wEnd = wEnd1;

                dwStoreIdx = ((EC_T_DWORD)-1);

                /* seek if create ranges are larger than the smallest stored entry, and replace the smallest one */
                for (dwScanIdx = 0; dwScanIdx < ECX_ADDRRANGE_AMOUNT; dwScanIdx++)
                {
                    if (WDELTASCAN < WDELTA2)
                    {
                        if (((EC_T_DWORD)-1) == dwStoreIdx)
                        {
                            dwStoreIdx = dwScanIdx;
                        }
                        else if (WDELTASTORE < WDELTASCAN)
                        {
                            dwStoreIdx = dwScanIdx;
                        }
                    }
                }

                /* dwStoreIdx should store smallest entry, which is the one to overwrite */
                if (((EC_T_DWORD)-1) != dwStoreIdx)
                {
                    m_abHugeRanges[dwStoreIdx].wBegin = wBegin2;
                    m_abHugeRanges[dwStoreIdx].wEnd = wEnd2;
                }
            }

            /* if zones where split, leave this scope */
            break;
        } /* address is in a stored zone */
    } /* scan zones */

    /* if no range was found, address is already outside one of the outside ranges, no action needed */

    /* sort ranges */
    SortAddressRanges();
}

/***************************************************************************************************/
/**
\brief  Sort Free Address Ranges by size desc.
*/
EC_T_VOID CEcConfigData::SortAddressRanges(EC_T_VOID)
{
    ECX_T_ADDRRANGE     oTmp = { 0 };
    EC_T_DWORD          dwIdx = 0;
    EC_T_DWORD          dwIdx2 = 0;
    EC_T_DWORD          dwKey = 0;

    /* INSERTSORT Algorithm

    for(dwIdx = 2; dwIdx < ECX_ADDRRANGE_AMOUNT; dwIdx ++ )
    {
    key = Value[dwIdx] = [dwidx].end-[dwidx].begin;
    dwIdx2 = dwIdx-1;
    while( dwIdx2 > 0 && Value[dwIdx2] < key <=> (([dwidx2].end-[dwidx2].begin) < key))
    {
    Value[dwIdx2+1] = Value[dwIdx2];
    dwIdx2--;
    }

    Value[dwIdx2+1]=key;
    }
    */

#define DELTA(x)    ((EC_T_DWORD)((x).wEnd - (x).wBegin))

    for (dwIdx = 2; dwIdx < (ECX_ADDRRANGE_AMOUNT + 1); dwIdx++)
    {
        dwKey = DELTA(m_abHugeRanges[(dwIdx - 1)]);
        OsMemcpy(&oTmp, &(m_abHugeRanges[(dwIdx - 1)]), sizeof(ECX_T_ADDRRANGE));

        dwIdx2 = (dwIdx - 1);

        while ((dwIdx2 > 0) && (DELTA(m_abHugeRanges[(dwIdx2 - 1)]) < dwKey))
        {
            OsMemcpy(&(m_abHugeRanges[(dwIdx2 - 1) + 1]), &(m_abHugeRanges[(dwIdx2 - 1)]), sizeof(ECX_T_ADDRRANGE));
            dwIdx2--;
        }

        OsMemcpy(&(m_abHugeRanges[(dwIdx2 - 1) + 1]), &oTmp, sizeof(ECX_T_ADDRRANGE));

    } /* for( dwIdx = 1; dwIdx < ECX_ADDRRANGE_AMOUNT; dwIdx++ ) */
}


/********************************************************************************/
/** \brief Expand configuration data pointer array (array management).
*
* \return EC_TRUE if array could be expanded.
*/
EC_T_BOOL CEcConfigData::ExpandPointerArray(
    EC_T_VOID*** pppvArray,     /* [in]  pointer to the array */
    EC_T_DWORD*  pdwCurSize,   /* [in,out]  Current size */
    EC_T_DWORD   dwExpandSize  /* [in]  expand size */
    )
{
    EC_T_VOID** ppvNewArray;
    EC_T_DWORD  dwIndex;
    EC_T_BOOL   bOk = EC_TRUE;

    ppvNewArray = (EC_T_VOID**)ConfigDataAllocMem(sizeof(EC_T_VOID*) * (*pdwCurSize+dwExpandSize));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::ExpandPointerArray", ppvNewArray, (*pdwCurSize + dwExpandSize) * sizeof(EC_T_PVOID));
    if (ppvNewArray == EC_NULL)
    {
        EC_ERRORMSG(("CEcConfigData::ExpandPointerArray() - no memory while expanding pointer array\n"));
        bOk = EC_FALSE;
    }
    else
    {
        OsMemset(ppvNewArray, 0, (*pdwCurSize + dwExpandSize) * sizeof(EC_T_VOID*));

        /* save old array pointer values */
        for (dwIndex = 0; dwIndex < *pdwCurSize; dwIndex++)
        {
            ppvNewArray[dwIndex] = (*pppvArray)[dwIndex];
        }
        /* delete old array pointer values */
        EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::ExpandPointerArray", *pppvArray, (*pdwCurSize) * sizeof(EC_T_PVOID));
        ConfigDataFreeMem(*pppvArray);
        /* return new values */
        *pppvArray = ppvNewArray;
        *pdwCurSize += dwExpandSize;
    }
    return bOk;
}

#if (defined INCLUDE_VARREAD)
/***************************************************************************************************/
/**
\brief  Adds a new process variable to internal storage.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcConfigData::AddProcessVarInfoEntry(
    EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo, /**< [out] Pointer to ProcessImageVar */
    EC_T_BOOL bIsInputData                  /**< [in]  Selects the corresponding process var array Input/Output */
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_BOOL bOk = EC_TRUE;
    EC_T_PD_VAR_INFO_INTERN*** papoProcessVarInfo = EC_NULL;
    EC_T_DWORD* pdwArraySize = EC_NULL;
    EC_T_DWORD* pdwNumEntries = EC_NULL;

    /* select process var array */
    if (bIsInputData)
    {
        papoProcessVarInfo = &m_apoProcessInputVarInfo;
        pdwArraySize  = &m_dwProcessInputVarInfoArraySize;
        pdwNumEntries = &m_dwNumProcessInputVarInfoEntries;
    }
    else
    {
        papoProcessVarInfo = &m_apoProcessOutputVarInfo;
        pdwArraySize  = &m_dwProcessOutputVarInfoArraySize;
        pdwNumEntries = &m_dwNumProcessOutputVarInfoEntries;
    }

    /* expand process var array */
    (*pdwNumEntries)++;
    if (*pdwNumEntries > *pdwArraySize)
    {
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)papoProcessVarInfo, pdwArraySize, C_dwCfgProcessVarAllocGranularity);
        if (!bOk)
        {
            (*pdwNumEntries)--;
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }
    pProcessVarInfo->bIsInputData = bIsInputData;
    /* add new process variable */
    (*papoProcessVarInfo)[(*pdwNumEntries) - 1] = pProcessVarInfo;

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Gets an process variable info entry by index.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcConfigData::GetProcessVarInfoEntry(
    EC_T_PD_VAR_INFO_INTERN** ppProcessVarInfoEntry, /**< [out] Pointer to ProcessVarInfo entry */
    EC_T_DWORD dwProcessVarInfosIdx,                 /**< [in]  Index to access the process variable info array */
    EC_T_BOOL bIsInputData                           /**< [in]  Selects the corresponding process var array Input/Output */
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo = EC_NULL;

    /* Get the process variable information entry with the given index */
    if (bIsInputData)
    {
        pProcessVarInfo = m_apoProcessInputVarInfo[dwProcessVarInfosIdx];
    }
    else
    {
        pProcessVarInfo = m_apoProcessOutputVarInfo[dwProcessVarInfosIdx];
    }
    if (pProcessVarInfo == EC_NULL)
    {
        /* should never happen */
        OsDbgAssert(EC_FALSE);
        EC_DBGMSG("CEcConfigData::GetProcessVarInfoEntry() - missing entry index %d\n", dwProcessVarInfosIdx);
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    *ppProcessVarInfoEntry = pProcessVarInfo;

Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Creates a process image variable a adds it to the corresponding process
*          var array.
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::CreateProcessVarInfoEntry(
    EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo, /**< [In] Temporary process variable */
    EC_T_BOOL bIsInputData,                   /**< [In] Selects the corresponding process Input/Output */
    EC_T_PD_VAR_INFO_INTERN** ppCreatedVar    /**< [Out] Created process image variable(optional) */
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    EC_T_DWORD dwDescSize = _CalcPDVarInfoDescSize(pProcessVarInfo);
    EC_T_PD_VAR_INFO_INTERN* pPDVarInfo = EC_NULL;

    pPDVarInfo = (EC_T_PD_VAR_INFO_INTERN*)ConfigDataAllocMem(dwDescSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::CreateProcessVarInfo", pPDVarInfo, dwDescSize);
    if (pPDVarInfo == EC_NULL)
    {
        EC_ERRORMSG(("CEcConfigData::CreateProcessVarInfo() - no memory for process var. entry\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pPDVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
    /* Copy Parameters to ProcessVarInfo */
    OsMemcpy(pPDVarInfo, pProcessVarInfo, sizeof(EC_T_PD_VAR_INFO_INTERN));
    pPDVarInfo->bIsInputData = bIsInputData;

    if (EC_NULL != pProcessVarInfo->szSpecificDataType)
    {
        OsStrcpy(EC_ENDOF(pPDVarInfo), pProcessVarInfo->szSpecificDataType);
        pPDVarInfo->szSpecificDataType = (EC_T_CHAR*)EC_ENDOF(pPDVarInfo);
    }

    /* Add new process image variable */
    dwRetVal = AddProcessVarInfoEntry(pPDVarInfo, bIsInputData);
    if (EC_E_NOERROR != dwRetVal)
        goto Exit;

    if (EC_NULL != ppCreatedVar)
        *ppCreatedVar = pPDVarInfo;

Exit:
    if (dwRetVal != EC_E_NOERROR)
    {
        /* clean up */
        if (pPDVarInfo != EC_NULL)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CONFIG, "CEcConfigData::CreateProcessVarInfo", pPDVarInfo, dwDescSize);
            ConfigDataFreeMem(pPDVarInfo);            
        }
    }
    return dwRetVal;
}
/********************************************************************************/
/** \brief Adds process image variables for known complex data types
*
* \return EC_E_NOERROR on success
*/
EC_T_DWORD CEcConfigData::AddVarsForSpecificDataTypes()
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;
    EC_T_PD_VAR_INFO_INTERN oProcessVarInfo;

    /* eCfgState_EcCfg_Cfg_ProcessImage_Inputs */
    {
        EC_T_DWORD dwProcessVarEntryIdx = 0;

        /* iterate through the complete list of process variables and search for known complex data types */
        for (dwProcessVarEntryIdx = 0; dwProcessVarEntryIdx < m_dwNumProcessInputVarInfoEntries; dwProcessVarEntryIdx++)
        {
            EC_T_INT nChNumber = 1;
            EC_T_INT nChannelBitOffset = 0;

            /* get the next data type to check */
            EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo = m_apoProcessInputVarInfo[dwProcessVarEntryIdx];

            /* check for a special known data type */
            if ((pProcessVarInfo->wDataType == DEFTYPE_NULL) && (pProcessVarInfo->szSpecificDataType != EC_NULL))
            {
                OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));

                if ((OsStrcmp(pProcessVarInfo->szSpecificDataType, "FSOE_4098") == 0))
                {
                    EC_T_CHAR strNameTmp[MAX_PROCESS_VAR_NAME_LEN_EX - 26] = { 0 };
                    OsSnprintf(strNameTmp, (MAX_PROCESS_VAR_NAME_LEN_EX - 26), "%s[..]", pProcessVarInfo->szName);

                    for (nChNumber = 1; nChNumber <= 4; nChNumber++, nChannelBitOffset++)
                    {
                        OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                        /* add channel 1 - 4 */
                        OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].InputChannel%d", strNameTmp, nChNumber);
                        oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                        oProcessVarInfo.nBitOffs = (pProcessVarInfo->nBitOffs + 8 + nChannelBitOffset);
                        oProcessVarInfo.nBitSize = 1;
                        oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                        oProcessVarInfo.bIsInputData = EC_TRUE;

                        if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                        {
                            dwRetVal = EC_E_NOMEMORY;
                            goto Exit;
                        }
                    }
                }
                else if (OsStrcmp(pProcessVarInfo->szSpecificDataType, "FB Info 1") == 0)
                {
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s.Map State", pProcessVarInfo->szName);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs;
                    oProcessVarInfo.nBitSize = 8;
                    oProcessVarInfo.wDataType = DEFTYPE_UNSIGNED8;
                    oProcessVarInfo.bIsInputData = EC_TRUE;

                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }
                }
                else if (OsStrcmp(pProcessVarInfo->szSpecificDataType, "FB Info 3") == 0)
                {
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s.Map State", pProcessVarInfo->szName);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs;
                    oProcessVarInfo.nBitSize = 8;
                    oProcessVarInfo.wDataType = DEFTYPE_UNSIGNED8;
                    oProcessVarInfo.bIsInputData = EC_TRUE;
                    
                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }
                 
                    OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s.Diag State", pProcessVarInfo->szName);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 8;
                    oProcessVarInfo.nBitSize = 16;
                    oProcessVarInfo.wDataType = DEFTYPE_UNSIGNED16;
                    oProcessVarInfo.bIsInputData = EC_TRUE;

                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }
                }
                else if (OsStrcmp(pProcessVarInfo->szSpecificDataType, "FSOE_4096") == 0)
                {
                    EC_T_CHAR strNameTmp[MAX_PROCESS_VAR_NAME_LEN_EX - 26] = { 0 };
                    
                    OsSnprintf(strNameTmp, (MAX_PROCESS_VAR_NAME_LEN_EX - 26), "%s[..]", pProcessVarInfo->szName);

                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].Safety Project State", strNameTmp);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs;
                    oProcessVarInfo.nBitSize = 3;
                    oProcessVarInfo.wDataType = DEFTYPE_BIT3;
                    oProcessVarInfo.bIsInputData = EC_TRUE;

                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }

                    OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].Login State", strNameTmp);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 7;
                    oProcessVarInfo.nBitSize = 1;
                    oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                    oProcessVarInfo.bIsInputData = EC_TRUE;

                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }

                    OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].Input Size Mismatch", strNameTmp);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 8;
                    oProcessVarInfo.nBitSize = 1;
                    oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                    oProcessVarInfo.bIsInputData = EC_TRUE;
                    
                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }

                    OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].Output Size Mismatch", strNameTmp);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 9;
                    oProcessVarInfo.nBitSize = 1;
                    oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                    oProcessVarInfo.bIsInputData = EC_TRUE;

                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }

                    OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].TxPDO Toggle", strNameTmp);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 14;
                    oProcessVarInfo.nBitSize = 1;
                    oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                    oProcessVarInfo.bIsInputData = EC_TRUE;
                    
                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }

                    
                    OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                    OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].TxPDO State", strNameTmp);
                    oProcessVarInfo.wFixedAddr = pProcessVarInfo->wFixedAddr;
                    oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 15;
                    oProcessVarInfo.nBitSize = 1;
                    oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                    oProcessVarInfo.bIsInputData = EC_TRUE;

                    if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_TRUE, EC_NULL) != EC_E_NOERROR)
                    {
                        dwRetVal = EC_E_NOMEMORY;
                        goto Exit;
                    }
                }
            }
        }
    }

    /* eCfgState_EcCfg_Cfg_ProcessImage_Outputs */
    {
        EC_T_DWORD dwProcessVarEntryIdx = 0;

        /* iterate through the complete list of process variables and search for known complext data types */
        for (dwProcessVarEntryIdx = 0; dwProcessVarEntryIdx < m_dwNumProcessOutputVarInfoEntries; dwProcessVarEntryIdx++)
        {
            EC_T_INT nChNumber = 1;
            EC_T_INT nChannelBitOffset = 0;

            /* get the next data type to check */
            EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo = m_apoProcessOutputVarInfo[dwProcessVarEntryIdx];

            /* check for a special known data type */
            if ((pProcessVarInfo->wDataType == DEFTYPE_NULL) && (pProcessVarInfo->szSpecificDataType != EC_NULL))
            {
                if ((OsStrcmp(pProcessVarInfo->szSpecificDataType, "FSOE_4099") == 0))
                {
                    EC_T_CHAR strNameTmp[MAX_PROCESS_VAR_NAME_LEN_EX - 26] = { 0 };

                    for (nChNumber = 1; nChNumber <= 4; nChNumber++, nChannelBitOffset++)
                    {
                        OsMemset(&oProcessVarInfo, 0, sizeof(EC_T_PD_VAR_INFO_INTERN));
                        /* add channel 1 - 4 */
                        OsSnprintf(strNameTmp, (MAX_PROCESS_VAR_NAME_LEN_EX - 26), "%s[..]", pProcessVarInfo->szName);
                        OsSnprintf(oProcessVarInfo.szName, MAX_PROCESS_VAR_NAME_LEN_EX, "*%s[..].OutputChannel%d", strNameTmp, nChNumber);
                        oProcessVarInfo.wFixedAddr = 0;
                        oProcessVarInfo.nBitOffs = pProcessVarInfo->nBitOffs + 8 + nChannelBitOffset;
                        oProcessVarInfo.nBitSize = 1;
                        oProcessVarInfo.wDataType = DEFTYPE_BOOLEAN;
                        oProcessVarInfo.bIsInputData = EC_FALSE;

                        if (CreateProcessVarInfoEntry(&oProcessVarInfo, EC_FALSE, EC_NULL) != EC_E_NOERROR)
                        {
                            dwRetVal = EC_E_NOMEMORY;
                            goto Exit;
                        }
                    }
                }
            }
        }
    }
    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}
#endif /*(defined INCLUDE_VARREAD)*/

/********************************************************************************/
/** \brief Create new generic EtherCat command entry
*
* \return Packed init command description
*/
PEcInitCmdDesc CEcConfigData::CreateECatCmd(
    EC_T_ECAT_INIT_CMDS_DESC* pInitCmdDesc  /**< [in]  Temporary init command description */
    )
{
    EC_T_BOOL       bOk = EC_TRUE;
    EcInitCmdDesc*  pPkdEcInitCmdDesc = EC_NULL;
    EC_T_DWORD      dwDescSize;
    EC_T_INT        nMult = 1;

    EC_ICMDHDR_SET_LEN_LEN(&(pInitCmdDesc->EcInitCmdsDesc.EcCmdHeader), pInitCmdDesc->wDataLen);

    /* determine total size of configuration descriptor */
    if (pInitCmdDesc->wValidateDataLen > 0)
    {
        if (pInitCmdDesc->wValidateDataLen == pInitCmdDesc->wDataLen)
        {
            nMult++;
        }
        else
        {
            EC_ERRORMSG(("CEcConfigData::CreateECatCmd() - init command data len != validate data len\n"));
        }
        /* Add Mask size if exists */
        if (pInitCmdDesc->wValidateDataMaskLen > 0)
        {
            if (pInitCmdDesc->wValidateDataMaskLen == pInitCmdDesc->wDataLen)
            {
                nMult++;
            }
            else
            {
                EC_ERRORMSG(("CEcConfigData::CreateECatCmd() - init command data len != validate data mask len\n"));
            }
        }
    }
    dwDescSize = sizeof(EcInitCmdDesc) + nMult*pInitCmdDesc->wDataLen + pInitCmdDesc->dwCommentLen + 1;
    /* allocate memory for configuration descriptor and copy basic descriptor into final one */
    pPkdEcInitCmdDesc = (EcInitCmdDesc*)ConfigDataAllocMem(dwDescSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigData::CreateECatCmd", pPkdEcInitCmdDesc, dwDescSize);
    if (pPkdEcInitCmdDesc == EC_NULL)
    {
        EC_ERRORMSG(("CEcConfigData::CreateECatCmd() - no memory for EcInitCmdDesc* entry\n"));
        bOk = EC_FALSE;
        goto Exit;
    }
    OsMemcpy(pPkdEcInitCmdDesc, &pInitCmdDesc->EcInitCmdsDesc, sizeof(EcInitCmdDesc));

    /* append data to the end of the basic descriptor */
    if (pInitCmdDesc->bOnlyDataLen)
    {
        OsMemset(EC_ENDOF(pPkdEcInitCmdDesc), 0, pInitCmdDesc->wDataLen);
    }
    else
    {
        OsMemcpy(EC_ENDOF(pPkdEcInitCmdDesc), pInitCmdDesc->pbyData, pInitCmdDesc->wDataLen);
    }

    if (pInitCmdDesc->wValidateDataLen && pInitCmdDesc->wValidateDataMaskLen)
    {
        EC_ECINITCMDDESC_SET_VALIDATE(pPkdEcInitCmdDesc, EC_TRUE);
        EC_ECINITCMDDESC_SET_VALIDATEMASK(pPkdEcInitCmdDesc, EC_TRUE);
        EC_ECINITCMDDESC_SET_INITCMDTIMEOUT(pPkdEcInitCmdDesc, pInitCmdDesc->wValidateTimeout);
        OsMemcpy(&((EC_T_CHAR*)pPkdEcInitCmdDesc)[sizeof(EcInitCmdDesc) + 1 * pInitCmdDesc->wDataLen], pInitCmdDesc->pbyValidateData, pInitCmdDesc->wDataLen);
        OsMemcpy(&((EC_T_CHAR*)pPkdEcInitCmdDesc)[sizeof(EcInitCmdDesc) + 2 * pInitCmdDesc->wDataLen], pInitCmdDesc->pbyValidateDataMask, pInitCmdDesc->wDataLen);
        
    }
    else if (pInitCmdDesc->wValidateDataLen)
    {
        EC_ECINITCMDDESC_SET_VALIDATE(pPkdEcInitCmdDesc, EC_TRUE);
        EC_ECINITCMDDESC_SET_INITCMDTIMEOUT(pPkdEcInitCmdDesc, pInitCmdDesc->wValidateTimeout);
        OsMemcpy(&((EC_T_CHAR*)pPkdEcInitCmdDesc)[sizeof(EcInitCmdDesc) + 1 * pInitCmdDesc->wDataLen], 
            pInitCmdDesc->pbyValidateData, pInitCmdDesc->wDataLen);
    }

    if (pInitCmdDesc->dwCommentLen != 0)
    {
        EC_ECINITCMDDESC_SET_CMTLEN(pPkdEcInitCmdDesc, (EC_T_WORD)pInitCmdDesc->dwCommentLen);
        OsStrcpy(&((EC_T_CHAR*)pPkdEcInitCmdDesc)[sizeof(EcInitCmdDesc) + (pInitCmdDesc->wDataLen * nMult)], pInitCmdDesc->chCommentData);
    }

    EC_ECINITCMDDESC_SET_MASTERINITCMD(pPkdEcInitCmdDesc, pInitCmdDesc->bMasterInitCmd);

Exit:
    EC_UNREFPARM(bOk);

    return pPkdEcInitCmdDesc;
}

/********************************************************************************/
/** \brief Create a new packed EtherCAT init command entry and add to internal storage.
*          The packed init command will created from a init command description.
*
* \return N/A.
*/
EC_T_DWORD CEcConfigData::CreateECatInitCmd(
    EcInitCmdDesc*** papInitCmdDesc,    /** <[in] Pointer to target array */
    EC_T_WORD* pwNumInitCmds,           /** <[in/out] Current size */
    EC_T_ECAT_INIT_CMDS_DESC* pCmdsDesc /**< [in] Temporary init command description */
    )
{
    EC_T_BOOL       bOk = EC_TRUE;
    EC_T_DWORD      dwRetVal = EC_E_NOERROR;
    EcInitCmdDesc*  pNewInitCmdDesc = EC_NULL;

    if (0 == ((*pwNumInitCmds) % C_dwInitCmdAllocGranularity))
    {
        EC_T_DWORD dwArraySize = *pwNumInitCmds;
        bOk = ExpandPointerArray((EC_T_VOID***)papInitCmdDesc, &dwArraySize, C_dwInitCmdAllocGranularity);
        if (!bOk)
        {
            EC_ERRORMSG(("CEcConfigData::CreateECatInitCmd - no memory for init command entry\n"));
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    /* store packed descriptor */
    pNewInitCmdDesc = CreateECatCmd(pCmdsDesc);
    if (pNewInitCmdDesc == EC_NULL)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    
    (*papInitCmdDesc)[(*pwNumInitCmds)++] = pNewInitCmdDesc;
    
Exit:
    return dwRetVal;
}

/********************************************************************************/
/** \brief Create new packed EtherCAT CoE init command entry and add to ENI slave.
*
* \return N/A.
*/
EC_T_DWORD CEcConfigData::CreateECatMbxCoeCmd(
    EC_T_ENI_SLAVE* pEniSlave, 
    EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    PEcMailboxCmdDesc pPkdEcMbxCmdDesc = EC_NULL;
    EC_T_DWORD        dwDescSize = 0;
    
    if (0 == (pEniSlave->wNumCoeInitCmds % C_dwMbxInitCmdAllocGranularity))
    {
        EC_T_BOOL bOk = EC_TRUE;
        EC_T_DWORD dwArraySize = pEniSlave->wNumCoeInitCmds;
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&pEniSlave->apPkdCoeInitCmdDesc, &dwArraySize, C_dwMbxInitCmdAllocGranularity);
        if (!bOk)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    /* determine total size of init command entry */
    dwDescSize = sizeof(EcMailboxCmdDesc) + pCmdDesc->wDataLen + pCmdDesc->dwCommentLen + 1;

    /* allocate memory for init command entry and copy basic descriptor into final one */
    pPkdEcMbxCmdDesc = (PEcMailboxCmdDesc)ConfigDataAllocMem(dwDescSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigData::CreateECatMbxCoeCmd", pPkdEcMbxCmdDesc, dwDescSize);
    if (pPkdEcMbxCmdDesc == EC_NULL)
    {
        //EC_ERRORMSG(("CEcConfigData::AddECatMbxCoeCmd() - no memory\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pPkdEcMbxCmdDesc, 0, sizeof(EcMailboxCmdDesc));

    /* fill generic information  */
    EC_ECMBXCMDDESC_SET_FIXED(pPkdEcMbxCmdDesc, pCmdDesc->bFixed);
    EC_ECMBXCMDDESC_SET_TRANSITION(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTransition);
    EC_ECMBXCMDDESC_SET_MBXTIMEOUT(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTimeout);
    EC_ECMBXCMDDESC_SET_PROTOCOL(pPkdEcMbxCmdDesc, ETHERCAT_MBOX_TYPE_CANOPEN);

    /* fill SDO header */
    EC_ECSDOHDR_SET_INDEX(&(pPkdEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader), (EC_T_WORD)pCmdDesc->uMbx.coe.dwIndex);
    pPkdEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.SubIndex = (EC_T_BYTE)pCmdDesc->uMbx.coe.dwSubIndex;
    /* command type 1 = SDO initiate upload ; 2 = SDO initiate download */
    pPkdEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Ccs = (EC_T_BYTE)pCmdDesc->uMbx.coe.dwCcs;
    pPkdEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader.uHdr.Idq.Complete = (EC_T_WORD)pCmdDesc->uMbx.coe.bCompleteAccess;
    EC_ECSDOHDR_SET_SDODATA(&(pPkdEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader), pCmdDesc->wDataLen);

    /* copy data */
    EC_ECMBXCMDDESC_SET_DATALEN(pPkdEcMbxCmdDesc, (sizeof(pPkdEcMbxCmdDesc->uMbxHdr.coe.EcSdoHeader) + pCmdDesc->wDataLen));
    if (pCmdDesc->wDataLen != 0)
    {
        OsMemcpy(&pPkdEcMbxCmdDesc->uMbxHdr.coe.data, pCmdDesc->pbyData, pCmdDesc->wDataLen);
    }
    else
    {
        OsMemset(&pPkdEcMbxCmdDesc->uMbxHdr.coe.data, 0, pCmdDesc->wDataLen);
    }
    /* copy InitCmd comment */
    EC_ECMBXCMDDESC_SET_CMTLEN(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwCommentLen);
    if (pCmdDesc->dwCommentLen != 0)
    {
        OsStrcpy(EcMailboxCmdDescComment(pPkdEcMbxCmdDesc), pCmdDesc->chCommentData);
    }
    /* store packed descriptor */
    OsDbgAssert(pEniSlave->apPkdCoeInitCmdDesc != EC_NULL);
    OsDbgAssert(pEniSlave->apPkdCoeInitCmdDesc[pEniSlave->wNumCoeInitCmds] == EC_NULL);
    pEniSlave->apPkdCoeInitCmdDesc[pEniSlave->wNumCoeInitCmds] = pPkdEcMbxCmdDesc;
    pEniSlave->wNumCoeInitCmds++;

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_SOE_SUPPORT)
/********************************************************************************/
/** \brief Create new packed EtherCAT SoE init command entry and add to ENI slave.
*
* \return N/A.
*/
EC_T_DWORD CEcConfigData::CreateECatMbxSoeCmd(
    EC_T_ENI_SLAVE* pEniSlave, 
    EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc
    )
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    PEcMailboxCmdDesc pPkdEcMbxCmdDesc = EC_NULL;
    EC_T_DWORD        dwDescSize = 0;

    if (0 == (pEniSlave->wNumSoeInitCmds % C_dwMbxInitCmdAllocGranularity))
    {
        EC_T_BOOL bOk = EC_TRUE;
        EC_T_DWORD dwArraySize = pEniSlave->wNumSoeInitCmds;
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&pEniSlave->apPkdSoeInitCmdDesc, &dwArraySize, C_dwMbxInitCmdAllocGranularity);
        if (!bOk)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    /* determine total size of init command entry */
    dwDescSize = sizeof(EcMailboxCmdDesc) + pCmdDesc->wDataLen + pCmdDesc->dwCommentLen + 1;

    /* allocate memory for init command entry and copy basic descriptor into final one */
    pPkdEcMbxCmdDesc = (PEcMailboxCmdDesc)ConfigDataAllocMem(dwDescSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigData::CreateECatMbxSoeCmd", pPkdEcMbxCmdDesc, dwDescSize);
    if (pPkdEcMbxCmdDesc == EC_NULL)
    {
        //EC_ERRORMSG(("CEcConfigData::AddECatMbxSoeCmd() - no memory\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pPkdEcMbxCmdDesc, 0, sizeof(EcMailboxCmdDesc));

    /* set transitions during which this command should be sent */
    EC_ECMBXCMDDESC_SET_TRANSITION(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTransition);
    EC_ECMBXCMDDESC_SET_MBXTIMEOUT(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTimeout);
    EC_ECMBXCMDDESC_SET_FIXED(pPkdEcMbxCmdDesc, pCmdDesc->bFixed);

    /* set protocol */
    EC_ECMBXCMDDESC_SET_PROTOCOL(pPkdEcMbxCmdDesc, ETHERCAT_MBOX_TYPE_SOE);

    /* fill SoE header */
    pPkdEcMbxCmdDesc->uMbxHdr.soe.EcSoeHeader.OpCode = (EC_T_BYTE)(pCmdDesc->uMbx.soe.dwOpCode & 0x7);
    pPkdEcMbxCmdDesc->uMbxHdr.soe.EcSoeHeader.DriveNo = (EC_T_BYTE)(pCmdDesc->uMbx.soe.dwDriveNo & 0x7);
    pPkdEcMbxCmdDesc->uMbxHdr.soe.EcSoeHeader.byElements = (EC_T_BYTE)(pCmdDesc->uMbx.soe.dwElements);
    EC_ECSOEHDR_SET_IDN(&(pPkdEcMbxCmdDesc->uMbxHdr.soe.EcSoeHeader), (EC_T_WORD)pCmdDesc->uMbx.soe.dwIDN);

    /* copy data */
    EC_ECMBXCMDDESC_SET_DATALEN(pPkdEcMbxCmdDesc, (sizeof(pPkdEcMbxCmdDesc->uMbxHdr.soe.EcSoeHeader) + pCmdDesc->wDataLen));
    if (pCmdDesc->wDataLen != EC_NULL)
    {
        OsMemcpy(&pPkdEcMbxCmdDesc->uMbxHdr.soe.data, pCmdDesc->pbyData, pCmdDesc->wDataLen);
    }
    else
    {

        OsMemset(&pPkdEcMbxCmdDesc->uMbxHdr.soe.data, 0, pCmdDesc->wDataLen);
    }
    /* copy InitCmd comment */
    EC_ECMBXCMDDESC_SET_CMTLEN(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwCommentLen);
    if (pCmdDesc->dwCommentLen != 0)
    {
        OsStrcpy(EcMailboxCmdDescComment(pPkdEcMbxCmdDesc), pCmdDesc->chCommentData);
    }
    /* store packed descriptor */
    OsDbgAssert(pEniSlave->apPkdSoeInitCmdDesc != EC_NULL);
    OsDbgAssert(pEniSlave->apPkdSoeInitCmdDesc[pEniSlave->wNumSoeInitCmds] == EC_NULL);
    pEniSlave->apPkdSoeInitCmdDesc[pEniSlave->wNumSoeInitCmds] = pPkdEcMbxCmdDesc;
    pEniSlave->wNumSoeInitCmds++;

Exit:
    return dwRetVal;
}
#endif


/********************************************************************************/
/** \brief Create new packed EtherCAT EoE init command entry and add to ENI slave.
*
* \return N/A.
*/
EC_T_DWORD CEcConfigData::CreateECatMbxEoeCmd(EC_T_ENI_SLAVE* pEniSlave, EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    PEcMailboxCmdDesc pPkdEcMbxCmdDesc = EC_NULL;
    EC_T_DWORD        dwDescSize = 0;

    if (0 == (pEniSlave->wNumEoeInitCmds % C_dwMbxInitCmdAllocGranularity))
    {
        EC_T_BOOL bOk = EC_TRUE;
        EC_T_DWORD dwArraySize = pEniSlave->wNumEoeInitCmds;
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&pEniSlave->apPkdEoeInitCmdDesc, &dwArraySize, C_dwMbxInitCmdAllocGranularity);
        if (!bOk)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    /* determine total size of init command entry */
    dwDescSize = sizeof(EcMailboxCmdDesc) + pCmdDesc->wDataLen + pCmdDesc->dwCommentLen + 1;

    /* allocate memory for init command entry and copy basic descriptor into final one */
    pPkdEcMbxCmdDesc = (PEcMailboxCmdDesc)ConfigDataAllocMem(dwDescSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigData::CreateECatMbxEoeCmd", pPkdEcMbxCmdDesc, dwDescSize);
    if (pPkdEcMbxCmdDesc == EC_NULL)
    {
        //EC_ERRORMSG(("CEcConfigData::AddECatMbxEoeCmd() - no memory\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pPkdEcMbxCmdDesc, 0, sizeof(EcMailboxCmdDesc));

    /* set transitions during which this command should be sent */
    EC_ECMBXCMDDESC_SET_TRANSITION(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTransition);
    EC_ECMBXCMDDESC_SET_MBXTIMEOUT(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTimeout);
    EC_ECMBXCMDDESC_SET_FIXED(pPkdEcMbxCmdDesc, pCmdDesc->bFixed);

    /* set protocol */
    EC_ECMBXCMDDESC_SET_PROTOCOL(pPkdEcMbxCmdDesc, ETHERCAT_MBOX_TYPE_ETHERNET);

    /* The EoE header data will be set on CEcMbSlave::StartInitCmds! */

    /* copy data */
    EC_ECMBXCMDDESC_SET_DATALEN(pPkdEcMbxCmdDesc, pCmdDesc->wDataLen);
    if (pCmdDesc->wDataLen != 0)
    {
        OsMemcpy(&pPkdEcMbxCmdDesc->uMbxHdr.eoe.data, pCmdDesc->pbyData, pCmdDesc->wDataLen);
    }
    else
    {
        OsMemset(&pPkdEcMbxCmdDesc->uMbxHdr.eoe.data, 0, pCmdDesc->wDataLen);
    }
    /* copy InitCmd comment */
    EC_ECMBXCMDDESC_SET_CMTLEN(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwCommentLen);
    if (pCmdDesc->dwCommentLen != 0)
    {
        OsStrcpy(EcMailboxCmdDescComment(pPkdEcMbxCmdDesc), pCmdDesc->chCommentData);
    }
    /* store packed descriptor */
    OsDbgAssert(pEniSlave->apPkdEoeInitCmdDesc != EC_NULL);
    OsDbgAssert(pEniSlave->apPkdEoeInitCmdDesc[pEniSlave->wNumEoeInitCmds] == EC_NULL);
    pEniSlave->apPkdEoeInitCmdDesc[pEniSlave->wNumEoeInitCmds] = pPkdEcMbxCmdDesc;
    pEniSlave->wNumEoeInitCmds++;

Exit:
    return dwRetVal;
}

#if (defined INCLUDE_AOE_SUPPORT)

/********************************************************************************/
/** \brief Create new packed EtherCAT AoE init command entry and add to ENI slave.
*
* \return N/A.
*/
EC_T_DWORD CEcConfigData::CreateECatMbxAoeCmd(EC_T_ENI_SLAVE* pEniSlave, EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc)
{
    EC_T_DWORD dwRetVal = EC_E_NOERROR;
    PEcMailboxCmdDesc pPkdEcMbxCmdDesc = EC_NULL;
    EC_T_DWORD        dwDescSize = 0;

    if (0 == (pEniSlave->wNumAoeInitCmds % C_dwMbxInitCmdAllocGranularity))
    {
        EC_T_BOOL bOk = EC_TRUE;
        EC_T_DWORD dwArraySize = pEniSlave->wNumAoeInitCmds;
        bOk = ExpandPointerArray((EC_T_VOID***)(EC_T_VOID*)&pEniSlave->apPkdAoeInitCmdDesc, &dwArraySize, C_dwMbxInitCmdAllocGranularity);
        if (!bOk)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }

    /* determine total size of init command entry */
    dwDescSize = sizeof(EcMailboxCmdDesc) + pCmdDesc->wDataLen + pCmdDesc->dwCommentLen + 1;

    /* allocate memory for init command entry and copy basic descriptor into final one */
    pPkdEcMbxCmdDesc = (PEcMailboxCmdDesc)ConfigDataAllocMem(dwDescSize);
    EC_TRACE_ADDMEM(EC_MEMTRACE_CONFIG_CMDDESC, "CEcConfigData::AddECatMbxAoeCmd", pPkdEcMbxCmdDesc, dwDescSize);
    if (pPkdEcMbxCmdDesc == EC_NULL)
    {
        //EC_ERRORMSG(("CEcConfigData::AddECatMbxAoeCmd() - no memory\n"));
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemset(pPkdEcMbxCmdDesc, 0, sizeof(EcMailboxCmdDesc));

    /* set transitions during which this command should be sent */
    EC_ECMBXCMDDESC_SET_TRANSITION(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTransition);
    EC_ECMBXCMDDESC_SET_MBXTIMEOUT(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwTimeout);
    EC_ECMBXCMDDESC_SET_FIXED(pPkdEcMbxCmdDesc, pCmdDesc->bFixed);

    /* set protocol */
    EC_ECMBXCMDDESC_SET_PROTOCOL(pPkdEcMbxCmdDesc, ETHERCAT_MBOX_TYPE_ADS);

    /* copy data (AoE header is included) */
    EC_ECMBXCMDDESC_SET_DATALEN(pPkdEcMbxCmdDesc, (sizeof(pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader) + pCmdDesc->wDataLen));
    if (pCmdDesc->wDataLen != 0)
    {
        OsMemcpy(pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.oTargetNetId, ((EC_AOE_HDR*)pCmdDesc->pbyData)->oTargetNetId, 6);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.wTargetPort = EC_AOE_HDR_GET_TARGET_PORT((EC_AOE_HDR*)pCmdDesc->pbyData);
        OsMemcpy(pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.oSenderNetId, ((EC_AOE_HDR*)pCmdDesc->pbyData)->oSenderNetId, 6);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.wSenderPort = EC_AOE_HDR_GET_SENDER_PORT((EC_AOE_HDR*)pCmdDesc->pbyData);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.wCmdId = EC_AOE_HDR_GET_CMD_ID((EC_AOE_HDR*)pCmdDesc->pbyData);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.wStateFlags = EC_AOE_HDR_GET_STATE_FLAGS((EC_AOE_HDR*)pCmdDesc->pbyData);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.dwDataSize = EC_AOE_HDR_GET_DATA_SIZE((EC_AOE_HDR*)pCmdDesc->pbyData);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.dwErrorCode = EC_AOE_HDR_GET_ERROR_CODE((EC_AOE_HDR*)pCmdDesc->pbyData);
        pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.dwAoeInvokeId = EC_AOE_HDR_GET_INVOKE_ID((EC_AOE_HDR*)pCmdDesc->pbyData);
        OsMemcpy(&pPkdEcMbxCmdDesc->uMbxHdr.aoe.data[0], (pCmdDesc->pbyData + sizeof(EC_AOE_HDR)), pPkdEcMbxCmdDesc->uMbxHdr.aoe.EcAoeHeader.dwDataSize);
    }
    else
    {
        OsMemset(&pPkdEcMbxCmdDesc->uMbxHdr.aoe, 0, pCmdDesc->wDataLen);
    }
    /* copy InitCmd comment */
    EC_ECMBXCMDDESC_SET_CMTLEN(pPkdEcMbxCmdDesc, (EC_T_WORD)pCmdDesc->dwCommentLen);
    if (pCmdDesc->dwCommentLen != 0)
    {
        OsStrcpy(EcMailboxCmdDescComment(pPkdEcMbxCmdDesc), pCmdDesc->chCommentData);
    }
    /* store packed descriptor */
    OsDbgAssert(pEniSlave->apPkdAoeInitCmdDesc != EC_NULL);
    OsDbgAssert(pEniSlave->apPkdAoeInitCmdDesc[pEniSlave->wNumAoeInitCmds] == EC_NULL);
    pEniSlave->apPkdAoeInitCmdDesc[pEniSlave->wNumAoeInitCmds] = pPkdEcMbxCmdDesc;
    pEniSlave->wNumAoeInitCmds++;

Exit:
    return dwRetVal;
}
#endif /*INCLUDE_AOE_SUPPORT*/

#if (defined INCLUDE_GEN_OP_ENI)
/********************************************************************************/
/** \brief Creates a new cyclic frame command from several parameters.
*          After creation the new command will be added to the internal storage.
*
* \return N/A.
*/
EC_T_DWORD CEcConfigData::GenerateCyclicFrameCmd(
    EcCycFrameDesc* pCyclicFrameDesc,
    EC_T_BYTE byState,
    EC_CMD_TYPE eCmdType,
    EC_T_WORD wAdp, EC_T_WORD wAdo, EC_T_DWORD dwAddr,
    EC_T_WORD wDataLen,
    EC_T_WORD wCnt,
    EC_T_WORD wInputOffs, EC_T_WORD wOutputOffs)
{
    EC_T_BOOL bOk = EC_TRUE;
    EC_T_CYCLIC_CMD_CFG_DESC oCycCmdCfgDesc;
    OsMemset(&oCycCmdCfgDesc, 0, sizeof(EC_T_CYCLIC_CMD_CFG_DESC));

    oCycCmdCfgDesc.oEcCycCmdDesc.wConfOpStatesMask = byState;
    oCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader.uCmdIdx.swCmdIdx.byCmd = (EC_T_BYTE)eCmdType;
    if (dwAddr > 0)
    {
        oCycCmdCfgDesc.bAddrValid = EC_TRUE;
        EC_ICMDHDR_SET_ADDR(&(oCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), dwAddr);
    }
    else
    {
        oCycCmdCfgDesc.bAdoValid = EC_TRUE;
        oCycCmdCfgDesc.bAdpValid = EC_TRUE;
        EC_ICMDHDR_SET_ADDR_ADO(&(oCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), wAdo);
        EC_ICMDHDR_SET_ADDR_ADP(&(oCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), wAdp);
    }

    EC_ICMDHDR_SET_LEN_LEN(&(oCycCmdCfgDesc.oEcCycCmdDesc.EcCmdHeader), wDataLen);

    oCycCmdCfgDesc.oEcCycCmdDesc.wConfiguredWkc = wCnt;
    oCycCmdCfgDesc.oEcCycCmdDesc.wExpectedWkc = wCnt;
    oCycCmdCfgDesc.oEcCycCmdDesc.bCheckWkc = (wCnt > 0) ? EC_TRUE : EC_FALSE;

    if (wInputOffs > 0)
    {
        oCycCmdCfgDesc.bInputOffsValid = EC_TRUE;
        oCycCmdCfgDesc.dwInputOffs = wInputOffs;
        SetInputOffset(&oCycCmdCfgDesc.oEcCycCmdDesc, wInputOffs);
    }
    if (wOutputOffs > 0)
    {
        oCycCmdCfgDesc.bOutputOffsValid = EC_TRUE;
        oCycCmdCfgDesc.dwOutputOffs = wOutputOffs;
        SetOutputOffset(&oCycCmdCfgDesc.oEcCycCmdDesc, wOutputOffs);
    }

    bOk = CreateCyclicFrameCmd(pCyclicFrameDesc, &oCycCmdCfgDesc, EC_NULL);

    return bOk ? EC_E_NOERROR : EC_E_ERROR;
}
#endif

EC_T_MEMORY_LOGGER* CEcConfigData::GetMemLog() 
{ 
    return m_pMLog; 
}

/*-END OF SOURCE FILE--------------------------------------------------------*/

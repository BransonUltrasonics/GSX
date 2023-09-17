/*-----------------------------------------------------------------------------
 * EcDeviceFactory.h        header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description				interface for the CEcDevice class.
 *---------------------------------------------------------------------------*/


#ifndef INC_ECDEVICEFACTORY
#define INC_ECDEVICEFACTORY


/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECCONFIG
  #include <EcConfig.h>
#endif

#ifndef INC_ECDEVICE
  #include <EcDevice.h>
#endif

#ifndef INC_ECMASTER
  #include <EcMaster.h>
#endif

/*-DEFINES/MACROS------------------------------------------------------------*/
#define ECX_ADDRRANGE_AMOUNT    5       /* number of hugest address ranges to store */

#define SWAP_ENTRIES_ALLOC_COUNT 64
#define COPY_ENTRIES_ALLOC_COUNT 64
#define	ECAT_WCOUNT_DONT_CHECK	0xffff

const EC_T_DWORD C_dwInitCmdAllocGranularity = 40;
const EC_T_DWORD C_dwMbxInitCmdAllocGranularity = 10;
const EC_T_DWORD C_dwCfgDataSlaveAllocGranularity = 10;
const EC_T_DWORD C_dwCfgCyclicAllocGranularity = 5;
const EC_T_DWORD C_dwCfgCyclicFrameAllocGranularity = 5;
const EC_T_DWORD C_dwCfgCyclicCmdAllocGranularity = 10;
#if (defined INCLUDE_VARREAD)
const EC_T_DWORD C_dwCfgProcessVarAllocGranularity = 40;
#endif
#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
const EC_T_DWORD C_dwCfgHcGroupAllocGranularity = 10;
#endif

/*-FORWARD DECLARATIONS------------------------------------------------------*/
class CEcDevice;
class CEcMaster;
class CEcSlave;
class CEcConfigData;
#if (defined INCLUDE_XPAT)
class CEcConfigXpat;
#endif
struct TEcInitCmdDesc;
struct TEcCANopenCmdDesc;
struct _EC_T_CONFIGPARM_DESC;

typedef struct _EC_T_DEVICE_FACTORY_PARMS
{
    struct _EC_T_CONFIGPARM_DESC*   pConfigParms;       /* [in]  config layer parameters */
    MBX_CALLBACK                    pfMbCallback;       /* [in]  mailbox callback */
    NOTIFY_CALLBACK                 pfNotifyCallback;   /* [in]  notification callback */
    struct _EC_T_MASTER_CONFIG*     pMasterConfig;      /* [in]  master layer parameters */
    struct _EC_T_MASTER_CONFIG_EX*  pMasterConfigEx;    /* [in]  extended master layer parameters */
    EC_T_DWORD                      dwVersion;          /* [in]  ECM Version of calling instance */
    EC_T_BOOL                       bCreateTemporary;   /* [in]  Create Temporary instance (TRUE), create real instance (FALSE) */
    struct _EC_T_INTFEATUREFIELD_DESC* pFeatures;       /* [in]  Optional Feature field from previous master instance */
    EC_T_INIT_MASTER_PARMS*         pInitMasterParms;
    struct _EC_T_MEMORY_LOGGER*     pMLog;
} EC_T_DEVICE_FACTORY_PARMS;


/********************************************************************************/
/** \brief Class factory for creating an EtherCAT device.
*
* This classed is used by the application to create an EhterCAT device. An EtherCAT 
* Configuration file(XML) is passed to the CreateDevice method. Here the file is parsed
* and one master and one or more slave objects are created according to the information
* found in the XML file.
*
* \return 
*/
class CEcDeviceFactory
{
public:	
    /* brief constructor of CEcDeviceFactory */
    CEcDeviceFactory(CEcConfigData* poEcConfigData);

    /* brief destructor of CEcDeviceFactory */
    ~CEcDeviceFactory();

    /* brief Creates an EtherCAT device.
       param pConfigParms Configuration parameters (name of the xml Configuration File).
       param type Type of the Device. At the moment the only supported value is ECAT_CAPTURE_TYPE_NPF.
       param pMbCallback Pointer to a callback interface for mailbox communication.
       param ppEcDevice Address of IEcDevice* pointer variable that receives the interface pointer to the new EhterCAT device object. 
       return Result of the call. */

    EC_T_DWORD  CreateDevice(       struct _EC_T_DEVICE_FACTORY_PARMS* pParms, 
                                    CEcDevice**                     ppEcDevice);

    EC_T_DWORD ConfigExcludeSlave(  CEcDevice*              pEcDevice,
                                    EC_T_WORD               wPhysAddress);

    EC_T_DWORD ConfigIncludeSlave(  CEcDevice*              pEcDevice,
                                    EC_T_WORD               wPhysAddress);

    EC_T_DWORD ConfigSetPreviousPort(
                                    CEcDevice*              pEcDevice,
                                    EC_T_WORD               wPhysAddress,
                                    EC_T_WORD               wPhysAddressPrev,
                                    EC_T_WORD               wPortPrev);

    EC_T_DWORD ConfigApply(         CEcDevice*              pEcDevice,
                                    EC_T_BOOL               bCreateTemporary);

    struct _EC_T_MEMORY_LOGGER* GetMemLog();

protected:	
    EC_T_DWORD  CreateCyclicCmds(   CEcMaster*              pEcMaster               );
    EC_T_DWORD  DeviceFactoryCreateMaster(       
                                    CEcDevice*              pEcDevice, 
                                    MBX_CALLBACK            pfMbCallback, 
                                    NOTIFY_CALLBACK         pfNotifyCallback, 
                                    struct _EC_T_MASTER_CONFIG* pMasterConfig, 
                                    struct _EC_T_MASTER_CONFIG_EX* pMasterConfigEx, 
                                    struct _EC_T_INTFEATUREFIELD_DESC* pFeatures,
                                    CEcMaster**             ppCreMaster,
                                    EC_T_DWORD              dwVersion               );
    EC_T_DWORD  DeviceFactoryCreateSlaves(
                                    CEcMaster*              pEcDevice               );
#if (defined INCLUDE_OEM)
    EC_T_DWORD  DeviceFactoryDecryptCfgSlave( /* TODO decrypt EC_T_ENI_SLAVE* in DeviceFactoryDecryptCfg(dwEniKey) */
                                    CEcMaster*              pEcDevice,
                                    CEcSlave*               pSlave                  );
#endif /* INCLUDE_OEM */

    EC_T_DWORD  DeviceFactoryCreateCycCmdWkcList(CEcMaster* poMaster, CEcSlave* pCfgSlave, EC_T_CYC_CMD_WKC_DESC_ENTRY* aCycCmdWkcListTmp);

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    EC_T_DWORD  DeviceFactoryInitHotConnect(       
                                    CEcMaster*              pEcDevice               );
#endif
#if (defined INCLUDE_VARREAD)
    EC_T_VOID  DeviceFactoryCreateProcessVarInfoEntries(CEcMaster* pMaster);
#endif

private:
    /* brief confguration data */
    CEcConfigData*  m_poEcConfigData;
    EC_T_BOOL       m_bConfigModified;
};

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
typedef struct _EC_T_HC_GROUP_CFG_DESC
{
    EC_T_DWORD                  dwNumGroupMembers;
    EC_T_DWORD                  dwNumGroupMembersListed;
    EC_T_ENI_SLAVE**            apGroupMember;
    EC_T_BOOL                   bIsGroupComplete;

    EC_T_WORD                   wNumEcatIdentifyCmds;   
    EcInitCmdDesc**             apPkdEcatIdentifyCmdDesc;
}EC_T_HC_GROUP_CFG_DESC;
#endif

typedef struct _EC_T_ECAT_INIT_CMDS_DESC
{
    EcInitCmdDesc   EcInitCmdsDesc;             /* current init command */

    EC_T_BYTE*      pbyData;
    EC_T_WORD       wDataLen;
    EC_T_BOOL       bOnlyDataLen;

    EC_T_BYTE*      pbyValidateData;
    EC_T_WORD       wValidateDataLen;

    EC_T_BYTE*      pbyValidateDataMask;
    EC_T_WORD       wValidateDataMaskLen;
    EC_T_WORD       wValidateTimeout;

    EC_T_DWORD      dwCommentLen;
#define MAX_INITCMD_COMMENT_LEN             256
    EC_T_CHAR       chCommentData[MAX_INITCMD_COMMENT_LEN];

    EC_T_BOOL       bMasterInitCmd;
} EC_T_ECAT_INIT_CMDS_DESC;

typedef struct _EC_T_SLAVE_MBX_INIT_CMD_DESC
{
    /* generic mailbox init command entries */
    EC_T_BYTE*  		pbyData;
    EC_T_WORD   		wDataLen;

    EC_T_DWORD          dwCommentLen;
#define MAX_INITCMD_COMMENT_LEN             256
    EC_T_CHAR   chCommentData[MAX_INITCMD_COMMENT_LEN];

    EC_T_BOOL           bFixed;
    EC_T_DWORD          dwTransition;
    EC_T_DWORD          dwTimeout;
    /* Mailbox specific entries */
    union
    {
        struct
        {
            /* CoE specific entries */
            EC_T_BOOL           bCompleteAccess;
            EC_T_DWORD          dwCcs;
            EC_T_DWORD          dwIndex;
            EC_T_DWORD          dwSubIndex;
            EC_T_BOOL           bDisabled;
        } coe;
#if (defined INCLUDE_SOE_SUPPORT)
        struct
        {
            /* SoE specific entries */
            EC_T_DWORD          dwOpCode;
            EC_T_DWORD          dwDriveNo;
            EC_T_DWORD          dwIDN;
            EC_T_DWORD          dwElements;
            EC_T_DWORD          dwAttribute;
            EC_T_BOOL           bDisabled;
        } soe;
#endif
    } uMbx;
} EC_T_SLAVE_MBX_INIT_CMD_DESC;

typedef struct _EC_T_CONFIG_DATA_SLAVE
{
    EC_T_ENI_SLAVE*     pEniSlave;
#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    EC_T_VOID*          pHotConnectGroupLink;
#endif
    EC_T_BOOL           bExcluded;
} EC_T_CONFIG_DATA_SLAVE;

typedef struct _EC_T_CYCLIC_CMD_CFG_DESC
{
    EcCycCmdConfigDesc  oEcCycCmdDesc;
    EC_T_BOOL           bAddrValid;
    EC_T_BOOL           bAdpValid;
    EC_T_BOOL           bAdoValid;
    EC_T_BOOL           bInputOffsValid;
    EC_T_DWORD          dwInputOffs;
    EC_T_BOOL           bOutputOffsValid;
    EC_T_DWORD          dwOutputOffs;
} EC_T_CYCLIC_CMD_CFG_DESC;

typedef struct _EC_T_CYCLIC_ENTRY_CFG_DESC
{
    EC_T_DWORD          dwTaskId;                   /* task id (value from XML file) */
    EC_T_DWORD          dwPriority;                 /* priority (value from XML file) */
    EC_T_DWORD          dwCycleTime;                /* cycle time (value from XML file) */

    EC_T_DWORD          dwNumCyclicFrames;          /* number of cyclic frames */
    EC_T_DWORD          dwCyclicFrameArraySize;     /* size of the apCyclicFrameDesc[] array */
    EcCycFrameDesc**    apCyclicFrameDesc;          /* array of cyclic frame descriptors */
} EC_T_CYCLIC_ENTRY_CFG_DESC;

/********************************************************************************/
/** \brief  Class for EtherCAT configuration.
*
* This class is used by the application to configure EtherCAT.
*
* \return
*/
class CEcConfigData
{
#if (defined INCLUDE_GEN_OP_ENI)
    friend class CEcMaster;
#endif
    /*-MEMBERS-------------------------------------------------------------------*/
private:
    EC_T_DWORD              m_dwMasterInstanceID;
    struct _EC_T_MEMORY_LOGGER* m_pMLog;

    TYPECSTRING             m_strProductKey;                        /* product key */
    EC_T_DWORD              m_dwProductKeyStringLen;

    EC_T_DWORD              m_dwConfigCheckSum;

    EcMasterDesc            m_oCfgMaster;

    EC_T_CONFIG_DATA_SLAVE**        m_apoCfgDataSlave;              /* array of slave configuration objects */
    EC_T_DWORD                      m_dwCfgDataSlaveArraySize;      /* size of the m_apoCfgSlave[] array */
    EC_T_DWORD                      m_dwNumCfgDataSlaves;           /* total number of slaves */

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    EC_T_HC_GROUP_CFG_DESC**        m_apoHotConnectGroup;
    EC_T_DWORD                      m_dwHCGroupArraySize;           /* size of m_apoHotConnectGroup[] array */
    EC_T_DWORD                      m_dwNumHCGroups;                /* total number of HC Groups */
#endif

    EC_T_CYCLIC_ENTRY_CFG_DESC**    m_apoCfgCyclic;                 /* array of cyclic configuration objects */
    EC_T_DWORD                      m_dwCfgCyclicArraySize;         /* size of the m_apoCfgCyclic[] array */
    EC_T_DWORD                      m_dwNumCyclicEntries;           /* total number of cyclic entries */
   
#if (defined INCLUDE_VARREAD)
    EC_T_PD_VAR_INFO_INTERN**   m_apoProcessInputVarInfo;           /* array of process image input var. objects */
    EC_T_DWORD                  m_dwProcessInputVarInfoArraySize;   /* size of the m_apoCfgProcessImageInputVar[] array */
    EC_T_DWORD                  m_dwNumProcessInputVarInfoEntries;  /* total number of variable entries */

    EC_T_PD_VAR_INFO_INTERN**   m_apoProcessOutputVarInfo;          /* array of process image output var. objects */
    EC_T_DWORD                  m_dwProcessOutputVarInfoArraySize;  /* size of the m_apoCfgProcessImageOutputVar[] array */
    EC_T_DWORD                  m_dwNumProcessOutputVarInfoEntries; /* total number of variable entries */
    
    /* Swap information */
    EC_T_CYC_SWAP_INFO*     m_pSwapInfo;                            /* swap info */
    EC_T_WORD               m_wNumSwapInfos;                        /* number of swap info entries */
    EC_T_WORD               m_wNumSwapInfosAllocated;
#endif /* INCLUDE_VARREAD */

    /* Copy information for Slave-to-Slave communication */
    EC_T_CYC_COPY_INFO*     m_pCopyInfo;                            /* copy info */
    EC_T_WORD               m_wNumCopyInfos;                        /* number of copy info entries */
    EC_T_WORD               m_wNumCopyInfosAllocated;

public:
    /* parameter and data storage to configure Free usable address ranges for BT Temporary Addresses */
    ECX_T_ADDRRANGE         m_abHugeRanges[ECX_ADDRRANGE_AMOUNT];

    /* master settings (properties) */
    EC_T_MASTER_PROP_DESC*  m_aMasterProps;                         /* list entries */
    EC_T_DWORD              m_dwNumMasterPropEntries;               /* number of list entries */
    EC_T_DWORD              m_dwMasterPropArraySize;                /* size of the m_aMasterProps[] array */

    EC_T_DWORD              m_dwInputByteSize;
    EC_T_DWORD              m_dwOutputByteSize;

    /* Store ProcessData Offsets */
    EC_T_DWORD              m_dwLowestSlaveSend;
    EC_T_DWORD              m_dwHighestSlaveSend;
    EC_T_DWORD              m_dwLowestSlaveRecv;
    EC_T_DWORD              m_dwHighestSlaveRecv;
    EC_T_DWORD              m_dwLowestOutOffset;
    EC_T_DWORD              m_dwHighestOutByte;
    EC_T_DWORD              m_dwLowestInOffset;
    EC_T_DWORD              m_dwHighestInByte;

    /* Helper for CEcMaster::ConfigGenerate() */
#if (defined INCLUDE_DC_SUPPORT)
    EC_T_BOOL               m_bDcEnabled;
#endif
    EC_T_WORD               m_wLogicalOutpWkc;
    EC_T_WORD               m_wLogicalInpWkc;

    /*-FUNCTION DECLARATION------------------------------------------------------*/
private:
    EC_T_VOID       DeleteCfgDataSlave(EC_T_CONFIG_DATA_SLAVE* pCfgSlave);
    EC_T_VOID       DestroyEoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave);
    EC_T_VOID       DestroySoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave);
    EC_T_VOID       DestroyCoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave);
    EC_T_VOID       DestroyAoeEcatInitCmdsDesc(EC_T_ENI_SLAVE* pEniSlave);
    EC_T_VOID       DestroyEcatInitCmdsDesc(EcInitCmdDesc** apPkdEcatInitCmdDesc, EC_T_WORD wNumEcatInitCmds);

    EC_T_VOID       SetInOutputOffset(EcCycCmdConfigDesc* pCmdDesc, EC_T_DWORD dwOffs, EC_T_BOOL bIsInput);
    EC_T_VOID       SortAddressRanges(EC_T_VOID);
    PEcInitCmdDesc  CreateECatCmd(EC_T_ECAT_INIT_CMDS_DESC* pInitCmdDesc);
    EC_T_DWORD      AddEniSlave(EC_T_ENI_SLAVE* pEniSlave);
#if(defined INCLUDE_VARREAD)
    EC_T_DWORD      AddProcessVarInfoEntry(EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo,
                                           EC_T_BOOL bIsInputData);
#endif

public:
    CEcConfigData(EC_T_DWORD dwInstanceID, struct _EC_T_MEMORY_LOGGER* pMLog);
    virtual ~CEcConfigData();
    EC_T_VOID       InitializeConfigData(EC_T_VOID);
    EC_T_VOID       Initialize(EC_T_VOID);
    
    EC_T_DWORD      SetProductKey(const EC_T_CHAR* pszProductKey, EC_T_DWORD dwLen);
    EC_T_STRING     GetProductKey(EC_T_VOID) 
                    { 
                        return m_strProductKey; 
                    }

    EC_T_DWORD      GetMacAddrDestination(ETHERNET_ADDRESS* pMacAddr);
    EC_T_DWORD      GetMacAddrSource(ETHERNET_ADDRESS* pMacAddr);

    EC_T_VOID       SetConfigCheckSum(EC_T_DWORD dwChkSum)
                    {
                        m_dwConfigCheckSum = dwChkSum;
                    }
    EC_T_DWORD      GetConfigCheckSum(EC_T_VOID)
                    {
                        return m_dwConfigCheckSum;
                    }

    EC_T_CONFIG_DATA_SLAVE* GetCfgDataSlaveByIndex(EC_T_DWORD dwSlaveIndex)
                    {
                        return m_apoCfgDataSlave[dwSlaveIndex];
                    }
    EC_T_CONFIG_DATA_SLAVE* GetCfgDataSlaveByFixedAddr(EC_T_WORD wFixedAddress)
                    {
                        for (EC_T_DWORD dwSlaveIndex = 0; dwSlaveIndex < m_dwNumCfgDataSlaves; dwSlaveIndex++)
                        {
                            if (m_apoCfgDataSlave[dwSlaveIndex]->pEniSlave->wPhysAddr == wFixedAddress)
                            {
                                return m_apoCfgDataSlave[dwSlaveIndex];
                            }
                        }
                        return EC_NULL;
                    }
    EC_T_WORD       GetSlaveFixedAddr(EC_T_DWORD dwSlaveIndex)
                    {
                        return m_apoCfgDataSlave[dwSlaveIndex]->pEniSlave->wPhysAddr;
                    }
    EC_T_DWORD      SetSlaveExcludedFlag(EC_T_INT nSlaveIndex, EC_T_BOOL bExcluded);
    EC_T_DWORD      SetSlavePreviousPort(EC_T_INT nSlaveIndex, EC_T_WORD wPhysAddressPrev, EC_T_WORD wPortPrev);
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_BYTE*      GetAoeSlaveNetId(EC_T_INT nSlaveIndex);
#endif

    PEcMasterDesc   GetCfgMasterDesc(EC_T_VOID)
                    {
                        return &m_oCfgMaster; 
                    }
    /* master settings (properties) */
    EC_T_DWORD      GetNumMasterProps()
                    {
                        return m_dwNumMasterPropEntries;
                    }
    EC_T_DWORD      GetMasterPropArraySize() { return m_dwMasterPropArraySize; }

    EC_T_MASTER_PROP_DESC*  GetMasterPropArray()
                    {
                        return m_aMasterProps;
                    }

    EC_T_DWORD      CreateEniSlave(EC_T_ENI_SLAVE** ppEniSlave);
    EC_T_ENI_SLAVE* GetEniSlave(EC_T_INT nSlaveIndex);

    EC_T_VOID       GetProcessImageParams(EC_T_DWORD* pdwIn, EC_T_DWORD* pdwOut);

    EC_T_DWORD      CreateCyclicEntry(EC_T_CYCLIC_ENTRY_CFG_DESC** ppCreatedCyc);
    EC_T_CYCLIC_ENTRY_CFG_DESC*     GetCyclicEntry(EC_T_INT nCycEntryIdx)
                    {
                        return m_apoCfgCyclic[nCycEntryIdx];
                    }
    EC_T_INT        GetNumCyclicEntries(EC_T_VOID)
                    {
                        return (EC_T_INT)m_dwNumCyclicEntries;
                    }

    EC_T_DWORD      CreateCyclicFrame(EC_T_CYCLIC_ENTRY_CFG_DESC* poCyclicEntry, EcCycFrameDesc** ppCreatedFrame, EC_T_INT nPos);
    EC_T_DWORD      CreateCyclicFrame(EC_T_CYCLIC_ENTRY_CFG_DESC* poCyclicEntry, EcCycFrameDesc** ppCreatedFrame)
                    {
                        return CreateCyclicFrame(poCyclicEntry, ppCreatedFrame, poCyclicEntry->dwNumCyclicFrames);
                    }
    EcCycFrameDesc* GetCyclicFrame(EC_T_INT nCycEntryIdx, EC_T_INT nFrameIdx)
                    {
                        return m_apoCfgCyclic[nCycEntryIdx]->apCyclicFrameDesc[nFrameIdx];
                    }
    EC_T_INT        GetNumCyclicFrames(EC_T_INT nCycEntryIndex);

    EC_T_BOOL       CreateCyclicFrameCmd(EcCycFrameDesc* pCyclicFrameDesc, 
                                         EC_T_CYCLIC_CMD_CFG_DESC* pCycCmdCfgDesc,
                                         EcCycCmdConfigDesc** ppCreatedCmd);
    EC_T_DWORD      ReadCyclicCmd(EcCycCmdConfigDesc** ppCmd, EC_T_INT nCycEntryIndex,
                                    EC_T_INT nFrameIndex, EC_T_INT nCmdIndex);
    EC_T_INT        GetNumCyclicCmds(EC_T_VOID);
    EC_T_INT        GetNumCyclicCmds(EC_T_INT nCycEntryIndex, EC_T_INT nFrameIndex);

    EC_T_VOID       GetCyclicEntryParameters(EC_T_INT nCycEntryIndex, EC_T_DWORD* pdwCycleTime,
        EC_T_DWORD* pdwPriority, EC_T_DWORD* pdwTaskId);
    
    EC_T_VOID       SetInputOffset(EcCycCmdConfigDesc* pCmdDesc, EC_T_DWORD dwInputOffs)
                    {
                        SetInOutputOffset(pCmdDesc, dwInputOffs, EC_TRUE);
                    }
    EC_T_VOID       SetOutputOffset(EcCycCmdConfigDesc* pCmdDesc, EC_T_DWORD dwOutputOffs)
                    {
                        SetInOutputOffset(pCmdDesc, dwOutputOffs, EC_FALSE);
                    }

    EC_T_INT        GetNumSlaves(EC_T_VOID)
                    {
                        return (EC_T_INT)m_dwNumCfgDataSlaves;
                    }

    EC_T_WORD       GetCycCmdWkcIncrement(CEcSlave* pCfgSlave, EcCycCmdConfigDesc* pCmdDesc);

#if (defined INCLUDE_HOTCONNECT) || (defined INCLUDE_WKCSTATE)
    EC_T_DWORD      CreateHCGroup(EC_T_HC_GROUP_CFG_DESC** ppHCGroup);
    EC_T_DWORD      AddHCGroupMember(EC_T_HC_GROUP_CFG_DESC* pHCGroup, EC_T_ENI_SLAVE* pEniSlave);
    EC_T_DWORD      ReadHCGroup(EC_T_VOID* pvGroup, EC_T_VOID* pvMaster, EC_T_DWORD dwGrpIdx);
    EC_T_DWORD      GetNumHCGroups(EC_T_VOID) 
                    { 
                        return m_dwNumHCGroups; 
                    }
#endif
    EC_T_VOID        RemoveAddressFromRange(EC_T_WORD wAddress);
    ECX_T_ADDRRANGE* GetAddressRange(EC_T_DWORD dwIdx);

    EC_T_CYC_COPY_INFO* CreateCopyInfoEntry(EC_T_VOID);
    EC_T_CYC_COPY_INFO* GetCopyInfo(EC_T_VOID)
                    {
                        return m_pCopyInfo;
                    }
    EC_T_WORD       GetNumCopyInfoEntries(EC_T_VOID) 
                    { 
                        return m_wNumCopyInfos; 
                    }

#if (defined INCLUDE_VARREAD)
    EC_T_DWORD      CreateProcessVarInfoEntry(EC_T_PD_VAR_INFO_INTERN* pProcessVarInfo, EC_T_BOOL bIsInputData, 
                                              EC_T_PD_VAR_INFO_INTERN** ppCreatedVar);
    EC_T_DWORD      GetNumInpProcessVarInfo(EC_T_VOID)
                    {
                        return m_dwNumProcessInputVarInfoEntries;
                    }
    EC_T_DWORD      GetNumOutpProcessVarInfo(EC_T_VOID)
                    {
                        return m_dwNumProcessOutputVarInfoEntries;
                    }

    EC_T_DWORD      GetProcessVarInfoEntry(EC_T_PD_VAR_INFO_INTERN** ppProcessVarInfoEntry,
                                           EC_T_DWORD dwProcessVarInfosIdx,
                                           EC_T_BOOL bIsInputData);
    EC_T_DWORD      AddVarsForSpecificDataTypes();

    EC_T_CYC_SWAP_INFO* CreateSwapInfoEntry(EC_T_VOID);
    EC_T_CYC_SWAP_INFO* GetSwapInfo(EC_T_VOID)
                        {
                            return m_pSwapInfo;
                        }
    EC_T_WORD       GetNumSwapEntries(EC_T_VOID)
                    {
                        return m_wNumSwapInfos;
                    }
#endif /* INCLUDE_VARREAD */

   EC_T_BOOL       ExpandPointerArray(EC_T_VOID*** pppvArray, 
                                      EC_T_DWORD* pdwCurSize, 
                                      EC_T_DWORD dwExpandSize);
   EC_T_DWORD      CreateECatInitCmd(EcInitCmdDesc*** papInitCmdDesc, EC_T_WORD* pwNumInitCmds, EC_T_ECAT_INIT_CMDS_DESC* pCmdsDesc);

   EC_T_DWORD      CreateECatMbxCoeCmd(EC_T_ENI_SLAVE* pEniSlave, EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc);
   EC_T_DWORD      CreateECatMbxEoeCmd(EC_T_ENI_SLAVE* pEniSlave, EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc);
#if (defined INCLUDE_SOE_SUPPORT)
   EC_T_DWORD      CreateECatMbxSoeCmd(EC_T_ENI_SLAVE* pEniSlave, EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc);
#endif
#if (defined INCLUDE_AOE_SUPPORT)
   EC_T_DWORD      CreateECatMbxAoeCmd(EC_T_ENI_SLAVE* pEniSlave, EC_T_SLAVE_MBX_INIT_CMD_DESC* pCmdDesc);
   
#endif
   
#if (defined INCLUDE_GEN_OP_ENI)
   /* Helper for CEcMaster::ConfigGenerate() */
   EC_T_DWORD      GenerateCyclicFrameCmd(EcCycFrameDesc* pCyclicFrameDesc,
                                          EC_T_BYTE byState,
                                          EC_CMD_TYPE eCmdType,
                                          EC_T_WORD wAdp, EC_T_WORD wAdo, EC_T_DWORD dwAddr,
                                          EC_T_WORD wDataLen, 
                                          EC_T_WORD wCnt,
                                          EC_T_WORD wInputOffs, EC_T_WORD wOutputOffs);
#endif

   struct _EC_T_MEMORY_LOGGER* GetMemLog();
}; /* class CEcConfigData */

#endif /* INC_ECDEVICEFACTORY */ 
/*-END OF SOURCE FILE--------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * EcSdoServ.cpp
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Cyril Eyssautier
 * Description              Master Object Dictionary implementation
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include <EcCommon.h>
#include <EcCommonPrivate.h>

#if (defined INCLUDE_MASTER_OBD)

#include <EcLink.h>
#include <EcInterfaceCommon.h>
#include <EcInterface.h>
#include <EcSdoServ.h>

#include <EcObjDefPrivate.h>

#include "EthernetServices.h"
#include <EcServices.h>
#include <EcMasterCommon.h>

#include <EcEthSwitch.h>
#include <EcIoImage.h>
#include "EcFiFo.h"
#include <EcDevice.h>

#include <EcMaster.h>
#include <EcSlave.h>

#include <EcBusTopology.h>
#if (defined INCLUDE_DC_SUPPORT)
#include <EcDistributedClocks.h>
#endif

#include "EcText.h"

/* #define PRINT_DC_DBG_MESSAGES        */

/*-DEFINES-------------------------------------------------------------------*/
#define INFO_LIST_TYPE_LENGTH   0
#define INFO_LIST_TYPE_ALL      1
#define INFO_LIST_TYPE_RXPDO    2
#define INFO_LIST_TYPE_TXPDO    3
#define INFO_LIST_TYPE_BACKUP   4
#define INFO_LIST_TYPE_SET      5
#define INFO_LIST_TYPE_MAX      5

#define OBJGETNEXTSTR(p) \
    ((const EC_T_CHAR*)((p) + (EC_T_DWORD)OsStrlen((const EC_T_CHAR*)(p)) + 1))

#ifndef BIT2BYTE
    #define BIT2BYTE(x) \
        (((x)+7)>>3)
#endif

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-FUNCTIONS-----------------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Constructor.
*/
CEcSdoServ::CEcSdoServ(
    CEcMaster* pMaster,       /**< [in]   Master Instance */
    CEcDevice* pDevice,       /**< [in]   Device Instance */
    EC_T_WORD  wMaxSlaveObjs  /**< [in]   Max slave objects */
                      )
: m_pMaster(pMaster)
, m_pDevice(pDevice)
{
    m_pSdoCurInfoObjEntry       = EC_NULL;
    m_pObjectDict               = EC_NULL;
    m_wNumAllocObjectDict       = 0;
    m_pObject3000               = EC_NULL;              /* base ptr to objects 3000ff. */
    m_pObject8000               = EC_NULL;              /* base ptr to objects 8000ff. */
    m_pObject9000               = EC_NULL;              /* base ptr to objects 9000ff. */
    m_pObjectA000               = EC_NULL;              /* base ptr to objects A000ff. */
    m_pObjectF000               = EC_NULL;              /* base ptr to objects F000ff. */

    m_apSlaveObjects            = EC_NULL;
    m_apMasterSlave             = EC_NULL;
    m_apCfgSlaveObjects         = EC_NULL;
    m_apConSlaveObjects         = EC_NULL;
    m_apDiagSlaveObjects        = EC_NULL;
    m_apCfgAddressListObjects   = EC_NULL;
    m_apConAddressListObjects   = EC_NULL;

    m_wMaxSlavesObjs            = wMaxSlaveObjs;

    m_qwMailboxStatisticsClearCounters = 0;

    InitObjectDir(m_wMaxSlavesObjs);

    m_dwPrevMsecCounter         = 0;
    OsSystemTimeGet(&m_qwSystemTime);
}

/***************************************************************************************************/
/**
\brief  Destructor.
*/
CEcSdoServ::~CEcSdoServ()
{
    DeInitObjectDir();
}

/***************************************************************************************************/
/**
\brief  Allocates memory and initializes the array elements in object directory
*/
EC_T_DWORD CEcSdoServ::InitObjectDir(EC_T_WORD wMaxSlavesObjs)
{
    EC_T_DWORD  dwRetVal = EC_E_ERROR;
    EC_T_WORD   wScan    = 0;
    EC_T_WORD   wLength  = 0;

    const EC_T_SDOINFOENTRYDESC EntryDesc0x1008Default =
    {
        DEFTYPE_VISIBLESTRING, 0, ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE         /* SubIndex: 0 */
    };
    const EC_T_SDOINFOENTRYDESC EntryDesc0x1009Default =
    {
        /* "Vx.x.x.xx\0" */
        DEFTYPE_VISIBLESTRING, 0, ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE         /* SubIndex: 0 */
    };
    const EC_T_SDOINFOENTRYDESC EntryDesc0x100ADefault =
    {
        /* "Vx.x.x.xx\0" */
        DEFTYPE_VISIBLESTRING, 0, ACCESS_READ | OBJACCESS_NOPDOMAPPING | OBJACCESS_FORCE         /* SubIndex: 0 */
    };
    const EC_T_SDOINFOENTRYDESC DiagMessageDefault =
    {
        DEFTYPE_OCTETSTRING, (sizeof(EC_T_OBJ10F3_DIAGMSG)*8), ACCESS_READ | OBJACCESS_NOPDOMAPPING
    };
    const EC_T_SDOINFOENTRYDESC NotifyCounterDefault =
    {
        DEFTYPE_OCTETSTRING, (sizeof(EC_T_DWORD)*8), ACCESS_READ | OBJACCESS_NOPDOMAPPING
    };

    EC_T_OBJECTDICT*        pEntry              = EC_NULL;
    EC_T_WORD               dwSubObjCnt;
    EC_T_DWORD              subIdx;

    EC_T_WORD wAddrListObjCnt = (EC_T_WORD)((wMaxSlavesObjs + 254) / 255);

    if ((EC_NULL != m_apSlaveObjects) || (EC_NULL != m_apMasterSlave)
        || (EC_NULL != m_apCfgSlaveObjects) || (EC_NULL != m_apConSlaveObjects)
        || (EC_NULL != m_apDiagSlaveObjects) || (EC_NULL != m_apCfgAddressListObjects)
        || (EC_NULL != m_apConAddressListObjects) || (EC_NULL != m_pObjectDict))
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    m_pEntry0x1008 = EC_NULL;
    m_pEntry0x1009 = EC_NULL;
    m_pEntry0x100A = EC_NULL;
    m_pEntry0x10F3 = EC_NULL;
    m_pEntry0x2004 = EC_NULL;

    m_wMaxSlavesObjs          = wMaxSlavesObjs;

    m_apSlaveObjects          = (EC_T_OBJ3XXX_ENTRY*) OsMalloc(wMaxSlavesObjs*sizeof(EC_T_OBJ3XXX_ENTRY));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apSlaveObjects", m_apSlaveObjects, wMaxSlavesObjs*sizeof(EC_T_OBJ3XXX_ENTRY));

    m_apMasterSlave           = (CEcSlave**) OsMalloc(wMaxSlavesObjs*sizeof(CEcSlave*));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apMasterSlave", m_apMasterSlave, wMaxSlavesObjs*sizeof(EC_T_VOID*));

    m_apCfgSlaveObjects       = (EC_T_OBJ8XXX*) OsMalloc(wMaxSlavesObjs*sizeof(EC_T_OBJ8XXX));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apCfgSlaveObjects", m_apCfgSlaveObjects, wMaxSlavesObjs*sizeof(EC_T_OBJ8XXX));

    m_apConSlaveObjects       = (EC_T_OBJ9XXX_ENTRY*) OsMalloc(wMaxSlavesObjs*sizeof(EC_T_OBJ9XXX_ENTRY));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apConSlaveObjects", m_apConSlaveObjects, wMaxSlavesObjs*sizeof(EC_T_OBJ9XXX_ENTRY));

    /* CoE Slave Diagnosis Data Objects */
    m_apDiagSlaveObjects      = (EC_T_OBJAXXX*) OsMalloc(wMaxSlavesObjs*sizeof(EC_T_OBJAXXX));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apDiagSlaveObjects", m_apDiagSlaveObjects, wMaxSlavesObjs*sizeof(EC_T_OBJAXXX));

    m_apCfgAddressListObjects = (EC_T_OBJF02X*)OsMalloc(wAddrListObjCnt*sizeof(EC_T_OBJF02X));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apCfgAddressListObjects", m_apCfgAddressListObjects, wAddrListObjCnt*sizeof(EC_T_OBJF02X));

    m_apConAddressListObjects = (EC_T_OBJF04X*)OsMalloc(wAddrListObjCnt*sizeof(EC_T_OBJF04X));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apConAddressListObjects", m_apConAddressListObjects, wAddrListObjCnt*sizeof(EC_T_OBJF04X));

    wLength  = 0;

    /* 0x0000 - 0x0FFF Data Type Area */
    wLength += 1;                   /* Object 0x0800  ENUM */

    /* 0x1000 - 0x1FFF Communication Area */
    wLength += 1;                   /* Object 0x1000  Device Type */
    wLength += 1;                   /* Object 0x1008  Manufacturer Device Name */
    wLength += 1;                   /* Object 0x1009  Manufacturer Hardware Version */
    wLength += 1;                   /* Object 0x100A  Manufacturer Software Version */
    wLength += 1;                   /* Object 0x1018  Identity Object */
    wLength += 1;                   /* Object 0x10F3  History Object */

    /* 0x2000 - 0x5FFF Manufacturer Specific Area */
    wLength += 1;                   /* Object 0x2000  Master State change command */
    wLength += 1;                   /* Object 0x2001  Master State Summary */
    wLength += 1;                   /* Object 0x2002  Bus Diagnosis Object */
#ifdef INCLUDE_RED_DEVICE
    wLength += 1;                   /* Object 0x2003  Redundancy Diagnosis Object */
#endif
    wLength += 1;                   /* Object 0x2004  Notification Counter Object */
    wLength += 1;                   /* Object 0x2005  MAC Address Object */
#if (defined INCLUDE_MAILBOX_STATISTICS)
    wLength += 1;                   /* Object 0x2006  Mailbox Statistics Object */
#endif
    wLength += 1;                   /* Object 0x2010  Debug Register */
    wLength += 1;                   /* Object 0x2020  Master Initialization Parameters */
    wLength += 1;                   /* Object 0x2100  DC Deviation Limit */
    wLength += 1;                   /* Object 0x2101  DC Current Deviation */
    wLength += 1;                   /* Object 0x2102  DCM Bus Shift */
    wLength += 1;                   /* Object 0x2200  Bus Load Base */
    wLength = (EC_T_WORD)(wLength + wMaxSlavesObjs); /* 0x3000
                                      -0x3fff Slave Objects */

    /* ------------------------------------------------ */
    /* EtherCAT-Master - Modular device profile objects */
    /* ------------------------------------------------ */

    /* 0x8000 - 0x8FFF Configuration Area */
    wLength = (EC_T_WORD)(wLength + wMaxSlavesObjs); /* 0x8000
                                      -0x8fff Slave Objects (configured slaves) */

    /* 0x9000 - 0x9FFF Information Area */
    wLength = (EC_T_WORD)(wLength + wMaxSlavesObjs); /* 0x9000
                                      -0x9fff Slave Objects (connected slaves) */

    /* 0xA000 - 0xAFFF Diagnosis Area */
    wLength = (EC_T_WORD)(wLength + wMaxSlavesObjs); /* 0xA000
                                      -0xAFFF Slave Objects (slave diagnosis data) */

    /* 0xF000 - 0xFFFF Device Area */
    wLength += 1;                   /* 0xF000 Modular device profile object */
#if 0
    wLength += 1;                   /* 0xF002  Detect modules command object */
#endif
    wLength = (EC_T_WORD)(wLength + wAddrListObjCnt);    /* 0xF020
                                      -0xF02F Configured address list */
    wLength = (EC_T_WORD)(wLength + wAddrListObjCnt);    /* 0xF040
                                      -0xF04F Detected address list */

    wLength += 1;   /* EOL */

    /* create storage */
    m_pObjectDict   = (EC_T_OBJECTDICT*)OsMalloc(wLength * sizeof(EC_T_OBJECTDICT));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_pObjectDict", m_pObjectDict, wLength * sizeof(EC_T_OBJECTDICT));

    if ((EC_NULL == m_apSlaveObjects) || (EC_NULL == m_apMasterSlave)
        || (EC_NULL == m_apCfgSlaveObjects) || (EC_NULL == m_apConSlaveObjects)
        || (EC_NULL == m_apDiagSlaveObjects) || (EC_NULL == m_apCfgAddressListObjects)
        || (EC_NULL == m_apConAddressListObjects) || (EC_NULL == m_pObjectDict))
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    /* Zero out all entries */
    OsMemset(m_pObjectDict, 0, (wLength*sizeof(EC_T_OBJECTDICT)));
    m_wNumAllocObjectDict = wLength;

    pEntry = m_pObjectDict;

    /* initialize static info parts */
    /* Enum 0x0800 */
    pEntry->wIndex              = COEOBJID_0x800;
    pEntry->oObjDesc.wDataType  = DEFTYPE_ENUM;
    pEntry->oObjDesc.wObjFlags  = (0x02 | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          = S_aoEntryDesc0x0800;
    pEntry->pName               = EC_NULL;
    pEntry->pVarPtr             = apszEnum0800;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = EC_NULL;

    pEntry++;

    /* Object 0x1000  Device Type */
    pEntry->wIndex              = COEOBJID_DEVICE_TYPE;
    pEntry->oObjDesc.wDataType  = DEFTYPE_UNSIGNED32;
    pEntry->oObjDesc.wObjFlags  = (0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          = &S_oEntryDesc0x1000;
    pEntry->pName               = S_abyName0x1000;
    pEntry->pVarPtr             = &S_dwDevicetype;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = EC_NULL;

    pEntry++;

    /* Object 0x1008  Manufacturer Device Name */
    m_pEntry0x1008              = pEntry;
    pEntry->wIndex              = COEOBJID_MANF_DEVICE_NAME;
    pEntry->oObjDesc.wDataType  = DEFTYPE_VISIBLESTRING;
    pEntry->oObjDesc.wObjFlags  = (0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          = EC_NULL;
    pEntry->pName               = S_abyName0x1008;
    pEntry->pVarPtr             = EC_NULL;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = ReplaceAllocatedString;
    pEntry->pEntryDesc          = (EC_T_SDOINFOENTRYDESC*)OsMalloc(sizeof(EC_T_SDOINFOENTRYDESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "m_pEntry0x1008->pEntryDesc", pEntry->pEntryDesc, sizeof(EC_T_SDOINFOENTRYDESC));
    if (EC_NULL == pEntry->pEntryDesc)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemcpy(pEntry->pEntryDesc, &EntryDesc0x1008Default, sizeof(EC_T_SDOINFOENTRYDESC));
    pEntry++;

    /* Object 0x1009  Manufacturer Hardware Version */
    m_pEntry0x1009              = pEntry;
    pEntry->wIndex              = COEOBJID_MANF_HW_VERSION;
    pEntry->oObjDesc.wDataType  = DEFTYPE_VISIBLESTRING;
    pEntry->oObjDesc.wObjFlags  = (0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          = EC_NULL;
    pEntry->pName               = S_abyName0x1009;
    pEntry->pVarPtr             = EC_NULL;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = ReplaceAllocatedString;
    pEntry->pEntryDesc          = (EC_T_SDOINFOENTRYDESC*)OsMalloc(sizeof(EC_T_SDOINFOENTRYDESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "m_pEntry0x1009->pEntryDesc", pEntry->pEntryDesc, sizeof(EC_T_SDOINFOENTRYDESC));
    if (EC_NULL == pEntry->pEntryDesc)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemcpy(pEntry->pEntryDesc, &EntryDesc0x1009Default, sizeof(EC_T_SDOINFOENTRYDESC));
    pEntry++;

    /* Object 0x100A  Manufacturer Software Version */
    m_pEntry0x100A              = pEntry;
    pEntry->wIndex              = COEOBJID_MANF_SW_VERSION;
    pEntry->oObjDesc.wDataType  = DEFTYPE_VISIBLESTRING;
    pEntry->oObjDesc.wObjFlags  = (0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          = EC_NULL;
    pEntry->pName               = S_abyName0x100A;
    pEntry->pVarPtr             = EC_NULL;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = ReplaceAllocatedString;
    pEntry->pEntryDesc          = (EC_T_SDOINFOENTRYDESC*)OsMalloc(sizeof(EC_T_SDOINFOENTRYDESC));
    EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "m_pEntry0x100A->pEntryDesc", pEntry->pEntryDesc, sizeof(EC_T_SDOINFOENTRYDESC));
    if (EC_NULL == pEntry->pEntryDesc)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    OsMemcpy(pEntry->pEntryDesc, &EntryDesc0x100ADefault, sizeof(EC_T_SDOINFOENTRYDESC));
    pEntry++;

    /* Object 0x1018  Identity Object */
    pEntry->wIndex              =   COEOBJID_IDENTITY_OBJECT;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_IDENTITY;
    pEntry->oObjDesc.wObjFlags  =   (4 | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x1018;
    pEntry->pName               =   S_abyName0x1018;
    pEntry->pVarPtr             =   &S_oIdentity;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

    pEntry++;

    /* Object 0x10F3  History Object */
    m_pEntry0x10F3              =   pEntry;
    pEntry->wIndex              =   COEOBJID_HISTORY_OBJECT;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_HISTORY;
    pEntry->oObjDesc.wObjFlags  =   (OBJ10F3_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x10F3;
    pEntry->pName               =   S_abyName0x10F3;
    pEntry->pVarPtr             =   &m_oHistoryObject;
    pEntry->pfRead              =   ReadWrapper;
    pEntry->pfWrite             =   WriteWrapper;
    m_byLastDiagMsgRead         =   0;

    /* initialize history object entries */
    m_oHistoryObject.wSubIndex0             = 5;
    m_pEntry0x10F3->oObjDesc.wObjFlags      = (EC_T_WORD)(m_oHistoryObject.wSubIndex0 | (OBJCODE_REC << 8));
    m_oHistoryObject.byMaxDiagMessages      = MAX_DIAG_MSG;
    m_oHistoryObject.byNewestMessage        = 0;
    m_oHistoryObject.byNewestAckMessage     = 0;
    m_oHistoryObject.byNewDiagMessages      = EC_FALSE;
    m_oHistoryObject.wFlags                 = 0x0000;

    for (wScan = 6; wScan <= OBJ10F3_MAX_SUBINDEX; wScan++ )
    {
        OsMemcpy(&(S_aoEntryDesc0x10F3[wScan]), &DiagMessageDefault, sizeof(EC_T_SDOINFOENTRYDESC));
    }

    /* allocate memory for messages */
    OsMemset(m_aDiagMessages, 0, sizeof(m_aDiagMessages));
    for (wScan = 0; wScan < MAX_DIAG_MSG; wScan++)
    {
        m_aDiagMessages[wScan].pMessage = (EC_T_OBJ10F3_DIAGMSG*)OsMalloc(HISTORY_OBJECT_DIAGELE_SIZE);
        EC_TRACE_ADDMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_aDiagMessages", m_aDiagMessages[wScan].pMessage, HISTORY_OBJECT_DIAGELE_SIZE);
        if (m_aDiagMessages[wScan].pMessage == EC_NULL)
        {
            dwRetVal = EC_E_NOMEMORY;
            goto Exit;
        }
    }
    pEntry++;

    /* Object 0x2000  Master State change command */
    pEntry->wIndex              =   COEOBJID_MAST_STATECHNG;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_UNSIGNED32;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          =   &S_oEntryDesc0x2000;
    pEntry->pName               =   S_abyName0x2000;
    pEntry->pVarPtr             =   &m_dwMasterStateChng;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   WriteWrapper;

    m_dwMasterStateChng         =   0;

    pEntry++;

    /* Object 0x2001  Master State Summary */
    pEntry->wIndex              =   COEOBJID_MAST_STATESUMMARY;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_UNSIGNED32;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          =   &S_oEntryDesc0x2001;
    pEntry->pName               =   S_abyName0x2001;
    pEntry->pVarPtr             =   &m_dwMasterStateSummary;
    pEntry->pfRead              =   ReadWrapper;
    pEntry->pfWrite             =   EC_NULL;

    m_dwMasterStateSummary      =   0;

    pEntry++;

    /* Object 0x2002  Bus Diagnosis Object */
    pEntry->wIndex              =   COEOBJID_BUS_DIAGNOSIS;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_BUSDIAGNOSTIC;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(OBJ2002_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x2002;
    pEntry->pName               =   S_abyName0x2002;
    pEntry->pVarPtr             =   &m_oBusDiagnosisObject;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   WriteWrapper;

    OsMemset(&m_oBusDiagnosisObject, 0, sizeof(EC_T_OBJ2002));
    m_oBusDiagnosisObject.wSubIndex0 = OBJ2002_MAX_SUBINDEX;

    pEntry++;

#ifdef INCLUDE_RED_DEVICE
    /* Object 0x2003  Redundancy Diagnosis Object */
    pEntry->wIndex              =   COEOBJID_REDUNDANCY;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_REDUNDANCY;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(OBJ2003_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x2003;
    pEntry->pName               =   S_abyName0x2003;
    pEntry->pVarPtr             =   &m_oRedundancyObject;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

    OsMemset(&m_oRedundancyObject, 0, sizeof(EC_T_OBJ2003));
    m_oRedundancyObject.wSubIndex0 = OBJ2003_MAX_SUBINDEX;

    pEntry++;
#endif

    /* Object 0x2004  Notification Counter Object */
    m_pEntry0x2004              =   pEntry;
    pEntry->wIndex              =   COEOBJID_NOTIFY_COUNTER;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_NOTIFY_COUNTER;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(OBJ2004_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x2004;
    pEntry->pName               =   S_abyName0x2004;
    pEntry->pVarPtr             =   &m_oNotifyCounterObject;
    pEntry->pfRead              =   ReadWrapper;
    pEntry->pfWrite             =   WriteWrapper;
    m_byLastDiagMsgRead         =   0;

    /* initialize notification counter entries */
    m_oNotifyCounterObject.wSubIndex0     = NOTIFICATION_MEMBER_COUNT + (MAX_NOTIFICATIONS*2);
    m_pEntry0x2004->oObjDesc.wObjFlags    = (EC_T_WORD)(m_oNotifyCounterObject.wSubIndex0 | (OBJCODE_REC << 8));
    m_oNotifyCounterObject.byMaxMessages  = MAX_NOTIFICATIONS;
    m_oNotifyCounterObject.byMessageCount = 0;
    m_oNotifyCounterObject.byFlags        = 0;

    for (wScan = NOTIFICATION_MEMBER_COUNT; wScan <= OBJ2004_MAX_SUBINDEX; wScan++ )
    {
        OsMemcpy(&(S_aoEntryDesc0x2004[wScan]), &NotifyCounterDefault, sizeof(EC_T_SDOINFOENTRYDESC));
    }

    /* get memory for messages */
    OsMemset(m_aNotifyMessages, 0, sizeof(m_aNotifyMessages));
    pEntry++;

    /* Object 0x2005  MAC Address Object */
    pEntry->wIndex              = COEOBJID_MAC_ADDRESS;
    pEntry->oObjDesc.wDataType  = DEFTYPE_MACADDRESS;
    pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJ2005_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          = S_aoEntryDesc0x2005;
    pEntry->pName               = S_abyName0x2005;
    pEntry->pVarPtr             = &m_oMacAddressObject;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = EC_NULL;

    OsMemset(&m_oMacAddressObject, 0, sizeof(EC_T_OBJ2005));
    m_oMacAddressObject.wSubIndex0 = OBJ2005_MAX_SUBINDEX;

    pEntry++;

#if (defined INCLUDE_MAILBOX_STATISTICS)
    /* Object 0x2006  Mailbox Statistics Object */
    pEntry->wIndex = COEOBJID_MAILBOX_STATISTICS;
    pEntry->oObjDesc.wDataType = DEFTYPE_MAILBOX_STATISTICS;
    pEntry->oObjDesc.wObjFlags = (EC_T_WORD)(OBJ2006_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc = S_aoEntryDesc0x2006;
    pEntry->pName = S_abyName0x2006;
    pEntry->pVarPtr = EC_NULL;
    pEntry->pfRead = ReadWrapper;
    pEntry->pfWrite = WriteWrapper;

    pEntry++;
#endif /* INCLUDE_MAILBOX_STATISTICS */

    /* Object 0x2010  Debug Register */
    pEntry->wIndex              =   COEOBJID_DEBUG_REGISTER;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_UNSIGNED64;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          =   &S_oEntryDesc0x2010;
    pEntry->pName               =   S_abyName0x2010;
    pEntry->pVarPtr             =   EC_NULL;
    pEntry->pfRead              =   ReadWrapper;
    pEntry->pfWrite             =   WriteWrapper;

    pEntry++;

    /* Object 0x2020  Master Initialization parameters */
    pEntry->wIndex              =   COEOBJID_MASTER_INIT_PARM;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_MASTERINITPARM;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(OBJ2020_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x2020;
    pEntry->pName               =   S_abyName0x2020;
    pEntry->pVarPtr             =   &m_oMasterInitParmObject;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

    OsMemset(&m_oMasterInitParmObject, 0, sizeof(EC_T_OBJ2020));

    /* read data to structure */
    m_oMasterInitParmObject.wSubIndex0                   = OBJ2020_MAX_SUBINDEX;
    m_oMasterInitParmObject.dwApplicationVersion         = m_pMaster->GetApplicationVersion();
    m_oMasterInitParmObject.dwMasterVersion              = ATECAT_VERSION;
    m_oMasterInitParmObject.dwMaxSlavesProcessedPerCycle = m_pMaster->m_MasterConfig.dwMaxSlavesProcessedPerCycle;
    m_oMasterInitParmObject.dwEcatCmdTimeout             = m_pMaster->m_MasterConfig.dwEcatCmdTimeout;
    m_oMasterInitParmObject.dwEcatCmdMaxRetries          = m_pMaster->m_MasterConfig.dwEcatCmdMaxRetries;
    m_oMasterInitParmObject.dwBusCycleTimeUsec           = m_pMaster->m_MasterConfig.dwBusCycleTimeUsec;
    m_oMasterInitParmObject.dwEoeTimeout                 = m_pMaster->m_MasterConfig.dwEoeTimeout;
    m_oMasterInitParmObject.dwFoeBusyTimeout             = 0;
    m_oMasterInitParmObject.dwMaxQueuedEthFrames         = m_pMaster->m_MasterConfig.dwMaxQueuedEthFrames;
    m_oMasterInitParmObject.dwMaxSlaveCmdPerFrame        = m_pMaster->m_MasterConfig.dwMaxSlaveCmdPerFrame;
    m_oMasterInitParmObject.dwMaxBusSlave                = m_pMaster->GetMaxBusSlaves();
    m_oMasterInitParmObject.dwReserved2                  = 0;
    m_oMasterInitParmObject.dwStateChangeDebug           = m_pMaster->m_MasterConfig.dwStateChangeDebug;
/*  m_oMasterInitParmObject.szDriverIdent                = Filled by SetLinkLayerParms();*/
/*  m_oMasterInitParmObject.bPollingModeActive           = Filled by SetLinkLayerParms();*/
    m_oMasterInitParmObject.bAllocSendFrameActive        = EC_TRUE;  /* default to true if not DeviceBase tells us
                                                                      * through SetLinkLayerAllocSupport()
                                                                      */
    pEntry++;

    /* Object 0x2100  DC Deviation Limit*/
    pEntry->wIndex              =   COEOBJID_DC_DEVIATION_LIMIT;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_UNSIGNED32;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          =   &S_oEntryDesc0x2100;
    pEntry->pName               =   S_abyName0x2100;
    pEntry->pVarPtr             =   &m_dwDCDeviationLimit;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

#if (defined INCLUDE_DC_SUPPORT)
    if (m_pMaster->GetDcSupportEnabled())
    {
        m_dwDCDeviationLimit    =   m_pMaster->m_pDistributedClks->GetDeviationLimit();
    }
    else
#endif
    {
        m_dwDCDeviationLimit    =   0;
    }
    m_bDCInSync = EC_FALSE;
    m_bDCMInSync = EC_FALSE;

    pEntry++;

    /* Object 0x2101  DC Current deviation */
    pEntry->wIndex              =   COEOBJID_DC_CURDEVIATION;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_INTEGER32;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(0 | (OBJCODE_VAR << 8));
    pEntry->pEntryDesc          =   &S_oEntryDesc0x2101;
    pEntry->pName               =   S_abyName0x2101;
    pEntry->pVarPtr             =   EC_NULL;
    pEntry->pfRead              =   ReadWrapper;
    pEntry->pfWrite             =   EC_NULL;

    pEntry++;

    /* Object 0x2102  DCM Bus Shift */
    pEntry->wIndex              =   COEOBJID_DCM_BUSSHIFT;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_DCM_BUS_SHIFT;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(OBJ2102_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x2102;
    pEntry->pName               =   S_abyName0x2102;
    pEntry->pVarPtr             =   &m_oDcmBusShiftObject;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

    OsMemset(&m_oDcmBusShiftObject, 0, sizeof(m_oDcmBusShiftObject));
    m_oDcmBusShiftObject.wSubIndex0 = OBJ2102_MAX_SUBINDEX;

    pEntry++;

    /* Object 0x2200  Bus Load Base Object */
    pEntry->wIndex              =   COEOBJID_BUSLOAD_BASE;
    pEntry->oObjDesc.wDataType  =   DEFTYPE_BUSLOADBASE;
    pEntry->oObjDesc.wObjFlags  =   (EC_T_WORD)(OBJ2200_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          =   S_aoEntryDesc0x2200;
    pEntry->pName               =   S_abyName0x2200;
    pEntry->pVarPtr             =   &m_oBusLoadBaseObject;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

    OsMemset(&m_oBusLoadBaseObject, 0, sizeof(EC_T_OBJ2200));
    m_oBusLoadBaseObject.wSubIndex0 = OBJ2200_MAX_SUBINDEX;

    pEntry++;

    /* Object 0x3000 - 0x3FFF Slave Objects */
    m_pObject3000 = pEntry;
    for (wScan = 0; wScan < wMaxSlavesObjs; wScan++)
    {
        pEntry->wIndex              = (EC_T_WORD)(COEOBJID_SLAVECFGINFOBASE+wScan);
        pEntry->oObjDesc.wDataType  = DEFTYPE_SLAVECFGINFO;
        pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJ3XXX_MAX_SUBINDEX | (OBJCODE_REC << 8));
        pEntry->pEntryDesc          = S_aoEntryDesc0x3xxx;
        pEntry->pName               = S_abyName0x3xxx;
        pEntry->pVarPtr             = &(m_apSlaveObjects[wScan]);
        pEntry->pfRead              = EC_NULL;
        pEntry->pfWrite             = WriteWrapper;

        OsMemset(&(m_apSlaveObjects[wScan]), 0, sizeof(EC_T_OBJ3XXX));

        EC_SETWORD(&m_apSlaveObjects[wScan].wSubIndex0, OBJ3XXX_MAX_SUBINDEX);

        pEntry++;
    }

    /* Modular Device Profiles: EtherCAT Master
     * Object 0x8000 - 0x8FFF Slave Objects (configured slaves) */
    m_pObject8000 = pEntry;
    for (wScan = 0; wScan < wMaxSlavesObjs; wScan++)
    {
        pEntry->wIndex              = (EC_T_WORD)(COEOBJID_SLAVECFGBASE + wScan);
        pEntry->oObjDesc.wDataType  = DEFTYPE_SLAVECFG;
        pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJ8XXX_MAX_SUBINDEX | (OBJCODE_REC << 8));
        pEntry->pEntryDesc          = S_aoEntryDesc0x8xxx;
        pEntry->pName               = S_abyName0x8xxx;
        pEntry->pVarPtr             = &(m_apCfgSlaveObjects[wScan]);
        pEntry->pfRead              = EC_NULL;
        pEntry->pfWrite             = EC_NULL;

        OsMemset(&(m_apCfgSlaveObjects[wScan]), 0, sizeof(EC_T_OBJ8XXX));

        EC_SETWORD(&m_apCfgSlaveObjects[wScan].wSubIndex0, OBJ8XXX_MAX_SUBINDEX); /* last subindex */

        pEntry++;
    }

    /* Modular Device Profiles: EtherCAT Master
     * Object 0x9000 - 0x9FFF Slave Objects (connected slaves) */
    m_pObject9000 = pEntry;
    for (wScan = 0; wScan < wMaxSlavesObjs; wScan++)
    {
        pEntry->wIndex              = (EC_T_WORD)(COEOBJID_SLAVEINFBASE+wScan);
        pEntry->oObjDesc.wDataType  = DEFTYPE_SLAVEINF;
        pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJ9XXX_MAX_SUBINDEX | (OBJCODE_REC << 8));
        pEntry->pEntryDesc          = S_aoEntryDesc0x9xxx;
        pEntry->pName               = S_abyName0x9xxx;
        pEntry->pVarPtr             = &(m_apConSlaveObjects[wScan]);
        pEntry->pfRead              = EC_NULL;
        pEntry->pfWrite             = EC_NULL;

        OsMemset(&(m_apConSlaveObjects[wScan]), 0, sizeof(EC_T_OBJ9XXX));

        EC_SETWORD(&m_apConSlaveObjects[wScan].wSubIndex0, OBJ9XXX_MAX_SUBINDEX); /* last subindex */

        pEntry++;
    }

    /* Modular Device Profiles: EtherCAT Master
     * Object 0xA000 - 0xAFFF Diagnosis Data */
    m_pObjectA000 = pEntry;
    for (wScan = 0; wScan < wMaxSlavesObjs; wScan++)
    {
        pEntry->wIndex              = (EC_T_WORD)(COEOBJID_SLAVEDIAGBASE+wScan);
        pEntry->oObjDesc.wDataType  = DEFTYPE_SLAVEDIAG;
        pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJAXXX_MAX_SUBINDEX | (OBJCODE_REC << 8));
        pEntry->pEntryDesc          = S_aoEntryDesc0xAxxx;
        pEntry->pName               = S_abyName0xAxxx;
        pEntry->pVarPtr             = &(m_apDiagSlaveObjects[wScan]);
        pEntry->pfRead              = EC_NULL;
        pEntry->pfWrite             = WriteWrapper; /* AL control write */

        OsMemset(&(m_apDiagSlaveObjects[wScan]), 0, sizeof(EC_T_OBJAXXX));

        EC_SETWORD(&m_apDiagSlaveObjects[wScan].wSubIndex0, OBJAXXX_MAX_SUBINDEX); /* last subindex */

        pEntry++;
    }

    /* Modular Device Profiles: EtherCAT Master
     * Object 0xF000 Modular device profile object */
    m_pObjectF000               = pEntry;
    pEntry->wIndex              = COEOBJID_DEVICEPROFILE;
    pEntry->oObjDesc.wDataType  = DEFTYPE_DEVICEPROFILE;
    pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJF000_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          = S_aoEntryDesc0xF000;
    pEntry->pName               = S_abyName0xF000;
    pEntry->pVarPtr             = &m_oModularDeviceProfileObject;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = EC_NULL;

    OsMemset(&m_oModularDeviceProfileObject, 0, sizeof(EC_T_OBJF000));
    m_oModularDeviceProfileObject.wSubIndex0 = OBJF000_MAX_SUBINDEX; /* last subindex */

    pEntry++;

#if 0
    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0xF002  Detect Modules Command
     */
    pEntry->wIndex              = COEOBJID_DETECTMODCMD;
    pEntry->oObjDesc.wDataType  = DEFTYPE_DETECTMODCMD;
    pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(OBJF002_MAX_SUBINDEX | (OBJCODE_REC << 8));
    pEntry->pEntryDesc          = S_aoEntryDesc0xF002;
    pEntry->pName               = S_abyName0xF002;
    pEntry->pVarPtr             = &m_oScanCmdObject;
    pEntry->pfRead              = EC_NULL;
    pEntry->pfWrite             = WriteWrapper; /* Command Request */

    OsMemset(&m_oScanCmdObject, 0, sizeof(EC_T_OBJF002));
    m_oScanCmdObject.wSubIndex0 = OBJF002_MAX_SUBINDEX; /* last subindex */

    pEntry++;
#endif

    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0xF02x  Configured address list
     */
    for (wScan = 0; wScan < wAddrListObjCnt; ++wScan)
    {
#if (defined  _MSC_VER) /* GNU */
#pragma warning (disable:4127)
#endif
        if (   (wScan == (wAddrListObjCnt - 1))
            && ((wMaxSlavesObjs % 256) != 0)
           )
        {
            dwSubObjCnt = (EC_T_WORD)((wMaxSlavesObjs % 256) + 1);
        }
        else
        {
            dwSubObjCnt = 256;
        }
#if (defined  _MSC_VER) /* GNU */
#pragma warning (default:4127)
#endif

        pEntry->wIndex              = (EC_T_WORD)(COEOBJID_CONFADDRLISTBASE+wScan);
        pEntry->oObjDesc.wDataType  = DEFTYPE_CONFADDRLIST;
        pEntry->pEntryDesc          = S_aoEntryDesc0xF02x;
        pEntry->pName               = S_abyName0xF02x; /* Each empty string expands to "SubIndex XXX" */
        pEntry->pVarPtr             = &m_apCfgAddressListObjects[wScan];
        pEntry->pfRead              = EC_NULL;
        pEntry->pfWrite             = EC_NULL;

        /* Name strings (as empty strings) */
        OsMemset(pEntry->pName, 0, sizeof(C_abyAddrListSubidxDesc1));
        OsMemcpy(pEntry->pName, C_abyAddrListSubidxDesc1, sizeof(C_abyAddrListSubidxDesc1));

        /* Data buffer */
        OsMemset(pEntry->pVarPtr, 0, sizeof(EC_T_OBJF02X));

        /* Entry descriptors */
        OsMemset(pEntry->pEntryDesc, 0, sizeof(S_aoEntryDesc0xF02x));
        OsMemcpy(pEntry->pEntryDesc, &S_oSubindex0EntryDesc, sizeof(S_oSubindex0EntryDesc)); /* Subindex 0 */
        for (subIdx = 1; subIdx < dwSubObjCnt; ++subIdx)
        {
           pEntry->pEntryDesc[subIdx].wDataType = DEFTYPE_UNSIGNED16;
           pEntry->pEntryDesc[subIdx].wBitLength = 0x10;
           pEntry->pEntryDesc[subIdx].wObjAccess = ACCESS_READ | OBJACCESS_NOPDOMAPPING;
        }

        pEntry++;
    }

    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0xF04x  Detected address list
     */
    for (wScan = 0; wScan < wAddrListObjCnt; ++wScan)
    {
#if (defined  _MSC_VER) /* GNU */
#pragma warning (disable:4127)
#endif
        if ((wScan == (wAddrListObjCnt - 1)) && ((wMaxSlavesObjs % 256) != 0))
        {
            dwSubObjCnt = (EC_T_WORD)((wMaxSlavesObjs % 256) + 1);
        }
        else
        {
            dwSubObjCnt = 256;
        }
#if (defined  _MSC_VER) /* GNU */
#pragma warning (default:4127)
#endif

        pEntry->wIndex              =   (EC_T_WORD)(COEOBJID_CONNADDRLISTBASE+wScan);
        pEntry->oObjDesc.wDataType  =   DEFTYPE_CONNADDRLIST;
        pEntry->pEntryDesc          =   S_aoEntryDesc0xF04x;
        pEntry->pName               =   S_abyName0xF04x; /* Each empty string expands to "SubIndex XXX" */
        pEntry->pVarPtr             =   &m_apConAddressListObjects[wScan];
        pEntry->pfRead              =   EC_NULL;
        pEntry->pfWrite             =   EC_NULL;

        /* Name strings (as empty strings) */
        OsMemset(pEntry->pName, 0, sizeof(C_abyAddrListSubidxDesc2));
        OsMemcpy(pEntry->pName, C_abyAddrListSubidxDesc2, sizeof(C_abyAddrListSubidxDesc2));

        /* Data buffer */
        OsMemset(pEntry->pVarPtr, 0, sizeof(EC_T_OBJF04X));

        /* Entry descriptors */
        OsMemset(pEntry->pEntryDesc, 0, sizeof(EC_T_SDOINFOENTRYDESC) * dwSubObjCnt);
        OsMemcpy(pEntry->pEntryDesc, &S_oSubindex0EntryDesc, sizeof(S_oSubindex0EntryDesc)); /* Subindex 0 */
        for (subIdx = 1; subIdx < dwSubObjCnt; ++subIdx)
        {
           pEntry->pEntryDesc[subIdx].wDataType = DEFTYPE_UNSIGNED16;
           pEntry->pEntryDesc[subIdx].wBitLength = 0x10;
           pEntry->pEntryDesc[subIdx].wObjAccess = ACCESS_READ | OBJACCESS_NOPDOMAPPING;
        }

        pEntry++;
    }

    /* end of dictionary */
    pEntry->wIndex              =   COEOBJID_ENDLIST;
    pEntry->oObjDesc.wDataType  =   0;
    pEntry->oObjDesc.wObjFlags  =   0;
    pEntry->pEntryDesc          =   EC_NULL;
    pEntry->pName               =   EC_NULL;
    pEntry->pVarPtr             =   EC_NULL;
    pEntry->pfRead              =   EC_NULL;
    pEntry->pfWrite             =   EC_NULL;

    m_bIsAlStatusWkcOk = EC_FALSE;

    dwRetVal = EC_E_NOERROR;
Exit:
    if (EC_E_NOMEMORY == dwRetVal)
    {
        DeInitObjectDir();
    }
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  The function returns the Entry-Desc of a subindex to allow the application
        to define the object dictionary independent of the sdoserv-files.

\return Pointer to the EntryDesc of the Subindex.
*/
EC_T_SDOINFOENTRYDESC* CEcSdoServ::GetEntryDesc(
    EC_T_OBJECTDICT*    pObjEntry,      /**< [in]   handle to the dictionary object returned by
                                          *         GetObjectHandle which was called before
                                          */
            EC_T_BYTE   bySubindex      /**< [in]   subindex of the requested object */
                                               )
{
    EC_T_SDOINFOENTRYDESC*  pEntry      = EC_NULL;
    EC_T_BYTE               byObjCode   = 0;

    if( EC_NULL == pObjEntry )
    {
        pEntry = EC_NULL;
        goto Exit;
    }

    byObjCode = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT);

    if ( byObjCode == OBJCODE_ARR )
    {
        /* object is an array */
        if ( bySubindex == 0 )
        {
            /* subindex 0 has a description */
            pEntry = &pObjEntry->pEntryDesc[0];
        }
        else
        {
            /* and all other elements have the same description */
            pEntry = &pObjEntry->pEntryDesc[1];
        }
    }
    else
    {
        /* object is a variable or a record return the corresponding entry */
        pEntry = &pObjEntry->pEntryDesc[bySubindex];
    }

Exit:
    return pEntry;
}

/***************************************************************************************************/
/**
\brief  Frees memory of the array elements in object directory
*/
EC_T_VOID CEcSdoServ::DeInitObjectDir()
{
    EC_T_INT nIndex = 0;
    EC_T_WORD wAddrListObjCnt = (EC_T_WORD)((m_wMaxSlavesObjs + 254) / 255);

    if (EC_NULL != m_apSlaveObjects)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apSlaveObjects", m_apSlaveObjects, m_wMaxSlavesObjs*sizeof(EC_T_OBJ3XXX_ENTRY));
        SafeOsFree(m_apSlaveObjects);
    }

    if (EC_NULL != m_apMasterSlave)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apMasterSlave", m_apMasterSlave, m_wMaxSlavesObjs*sizeof(EC_T_VOID*));
        SafeOsFree(m_apMasterSlave);
    }

    if (EC_NULL != m_apCfgSlaveObjects)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apCfgSlaveObjects", m_apCfgSlaveObjects, m_wMaxSlavesObjs*sizeof(EC_T_OBJ8XXX));
        SafeOsFree(m_apCfgSlaveObjects);
    }

    if (EC_NULL != m_apConSlaveObjects)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apConSlaveObjects", m_apConSlaveObjects, m_wMaxSlavesObjs*sizeof(EC_T_OBJ9XXX_ENTRY));
        SafeOsFree(m_apConSlaveObjects);
    }

    if (EC_NULL != m_apDiagSlaveObjects)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apDiagSlaveObjects", m_apDiagSlaveObjects, m_wMaxSlavesObjs*sizeof(EC_T_OBJAXXX));
        SafeOsFree(m_apDiagSlaveObjects);
    }

    if (EC_NULL != m_apCfgAddressListObjects)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apCfgAddressListObjects", m_apCfgAddressListObjects, wAddrListObjCnt*sizeof(EC_T_OBJF02X));
        SafeOsFree(m_apCfgAddressListObjects);
    }

    if (EC_NULL != m_apConAddressListObjects)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_apConAddressListObjects", m_apConAddressListObjects, wAddrListObjCnt*sizeof(EC_T_OBJF04X));
        SafeOsFree(m_apConAddressListObjects);
    }

    FreeEntries0x1XXX();

    if (EC_NULL != m_pObjectDict)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_pObjectDict", m_pObjectDict, m_wNumAllocObjectDict * sizeof(EC_T_OBJECTDICT));
        SafeOsFree(m_pObjectDict);
    }

    for (nIndex = 0; nIndex < MAX_DIAG_MSG; nIndex++)
    {
        if (EC_NULL != m_aDiagMessages[nIndex].pMessage)
        {
            EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "CEcSdoServ:m_aDiagMessages", m_aDiagMessages[nIndex].pMessage, HISTORY_OBJECT_DIAGELE_SIZE);
            SafeOsFree(m_aDiagMessages[nIndex].pMessage);
            m_aDiagMessages[nIndex].pMessage = EC_NULL;
        }
    }

    m_pEntry0x1008 = EC_NULL;
    m_pEntry0x1009 = EC_NULL;
    m_pEntry0x100A = EC_NULL;
    m_wMaxSlavesObjs = 0;
}


/***************************************************************************************************/
/**
\brief  The function counts the number of objects of the requested list type.

\return Number of objects of the requested list type.
*/
EC_T_WORD CEcSdoServ::GetNoOfObjects(
    EC_T_BYTE byListType    /**< [in]   listType requested listType (0=all objects, 1=RxPDO mappable objects,
                              *         2=TxPDO mappable objects, 3=backup objects, 4=setting objects
                              */
                                    )
{
    /* the variable listFlags contains the mask used for the ObjAccess in the Entry-Desc
       see the structure TSDOINFOENTRYDESC in sdoserv.h, listType = 0 indictes that
       all objects has to be counted */

    EC_T_WORD           wListFlags  = (EC_T_WORD)(0x0020 << byListType);

    /* set pObjEntry to the beginning of the object dictionary */
    EC_T_OBJECTDICT*    pObjEntry   = m_pObjectDict;
    EC_T_WORD           n           = 0;

    if( EC_NULL == pObjEntry )
    {
        n = 0;
        goto Exit;
    }

    while (pObjEntry->wIndex != 0xFFFF)
    {
        /* count the objects of the requested list type */
        if ( pObjEntry->wIndex >= COEOBJID_DEVICE_TYPE )
        {
            EC_T_BYTE   t = byListType;
            if ( t )
            {
                EC_T_BYTE               byMaxSubindex   = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT);
                EC_T_BYTE               i               = 0;
                EC_T_SDOINFOENTRYDESC*  pEntry          = EC_NULL;

                while ( t && i <= byMaxSubindex )
                {
                    pEntry = GetEntryDesc(pObjEntry, i);

                    if ( (EC_NULL != pEntry) && (pEntry->wObjAccess & wListFlags) )
                    {
                        t = 0;
                    }
                    i++;
                }
            }

            if ( !t )
            {
                /* object from listType found */
                n++;
            }
        }

        /* next object in object dictionary */
        pObjEntry++;
    }

Exit:
    return n;
}

/***************************************************************************************************/
/**
\brief  The function copies (the part of) the object list in the mailbox buffer.

\return size of the available mailbox buffer which was not copied to.
*/
EC_T_DWORD CEcSdoServ::GetObjectList(
    EC_T_WORD   wListType,  /**< [in]   requested listType (0=all objects, 1=RxPDO mappable objects,
                              *         2=TxPDO mappable objects, 3=backup objects, 4=setting objects)
                              */
    EC_T_WORD*  pwIndex,    /**< [in]   next Index of the object list to copied in the mailbox buffer,
                              *         has to adapted at the end of the function
                              */
    EC_T_DWORD  dwSize,     /**< [in]   size of the available mailbox buffer */
    EC_T_WORD*  pwData      /**< [in]   pointer to the mailbox buffer where (the part of) the object list requested listType (0=all objects, 1=RxPDO mappable objects,
                              *         2=TxPDO mappable objects, 3=backup objects, 4=setting objects)
                              */
                                   )
{
    /* the variable listFlags contains the mask used for the ObjAccess in the Entry-Desc
       see the structure TSDOINFOENTRYDESC in sdoserv.h, listType = 0 indictes that
       all objects has to be counted */
    EC_T_WORD           wListFlags   = (EC_T_WORD)(0x0020 << wListType);
    EC_T_OBJECTDICT*    pObjEntry    = EC_NULL;
    EC_T_DWORD          dwNumObj3000 = EC_MIN((m_oBusDiagnosisObject.dwNumCfgSlaves+m_oBusDiagnosisObject.dwNumSlavesFound), m_wMaxSlavesObjs);

    if (pwIndex[0] == COEOBJID_DEVICE_TYPE)
    {
        /* beginning of object list, set pObjEntry to the beginning of the object dictionary */
        pObjEntry = m_pObjectDict;
    }
    else
    {
        /* next fragment, the next object to be handled was stored in pSdoInfoObjEntry */
        pObjEntry = m_pSdoCurInfoObjEntry;
    }

    if (EC_NULL == pObjEntry)
    {
        goto Exit;
    }

    /* master has no eODListType_RxPdoMap and no eODListType_TxPdoMap */
    if ((OBJACCESS_RXPDOMAPPING == wListFlags)
     || (OBJACCESS_TXPDOMAPPING == wListFlags))
    {
        goto Exit;
    }

    for (; (pObjEntry->wIndex != COEOBJID_ENDLIST) && (dwSize > 1); pObjEntry++)
    {
        /* get the next index of the requested object list if there is enough space in the mailbox buffer */
        if ( (pObjEntry->wIndex >= COEOBJID_DEVICE_TYPE) &&
           !((pObjEntry > (m_pObject3000 + (dwNumObj3000-1)))                           && (pObjEntry < (m_pObject3000 + m_wMaxSlavesObjs))) &&
           !((pObjEntry > (m_pObject8000 + (m_oBusDiagnosisObject.dwNumCfgSlaves-1)))   && (pObjEntry < (m_pObject8000 + m_wMaxSlavesObjs))) &&
           !((pObjEntry > (m_pObject9000 + (m_oBusDiagnosisObject.dwNumSlavesFound-1))) && (pObjEntry < (m_pObject9000 + m_wMaxSlavesObjs))) &&
           !((pObjEntry > (m_pObjectA000 + (m_oBusDiagnosisObject.dwNumSlavesFound-1))) && (pObjEntry < (m_pObjectA000 + m_wMaxSlavesObjs))))
        {
            EC_T_BYTE t = EC_LOBYTE(wListType);

            if (t)
            {
                EC_T_BYTE               byMaxSubindex   = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT);
                EC_T_BYTE               i               = 0;
                EC_T_SDOINFOENTRYDESC*  pEntry          = EC_NULL;

                for (i = 0; t && (i <= byMaxSubindex); i++)
                {
                    pEntry = GetEntryDesc(pObjEntry, i);
                    if ((EC_NULL != pEntry) && (pEntry->wObjAccess & wListFlags))
                    {
                        t = 0;
                    }
                }
            }
            if (!t)
            {
                /* store the index in the mailbox buffer */
                EC_SET_FRM_WORD(pwData, (pObjEntry->wIndex));
                pwData++;
                dwSize -= sizeof(EC_T_WORD);
            }
        }
    }

    /* return the next Index to be handled */
    pwIndex[0] = pObjEntry->wIndex;

    /* store object description pointer and index for next fragment */
    m_pSdoCurInfoObjEntry = pObjEntry;

Exit:
    /* return the size of the available mailbox buffer which was not copied to */
    return dwSize;
}

/***************************************************************************************************/
/**
\brief  Get Object Dictionary List.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::GetObjDictList(
    PEcMailboxCmd pCmd  /**< [in]   Mailbox cmd to store data into */
                                     )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;
    EC_T_WORD   wListType   = pCmd->sIndex.index;
    EC_T_WORD*  pwData      = EC_NULL;

    /* an object list is requested, check if the list type is supported */
    if ( !(wListType <= INFO_LIST_TYPE_MAX) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* first clear data */
    OsMemset(pCmd->pbyData, 0, pCmd->length);

    pwData = ((EC_T_WORD*)pCmd->pbyData);

    /* set type of List in response */
    EC_SET_FRM_WORD(pwData, wListType );

    if ( wListType-- == 0 )
    {
        /* List-Type 0: length of the lists */
        EC_T_BYTE i = 0;

        /* the needed mailbox size for List-Type 0 response is just 24 bytes, the mailbox has always
           to be at least 24 bytes to support the SDO Information service */

        for (i = 0; i < INFO_LIST_TYPE_MAX; i++)
        {
            EC_T_WORD n = GetNoOfObjects(i);

            /* copy the number of objects of the list type in the SDO Information response */

            EC_SET_FRM_WORD((&(pwData[1+i])), n);
        }

        /* size of the mailbox service response */
        pCmd->length = (INFO_LIST_TYPE_MAX * sizeof(EC_T_WORD))+sizeof(EC_T_WORD);
    }
    else
    {
        /* object list with indexes is requested */
        EC_T_WORD wStartIdx = COEOBJID_DEVICE_TYPE;

        /* get the requested object list */
        pCmd->length = pCmd->length - GetObjectList(wListType, &wStartIdx, pCmd->length, &(pwData[1]));

        pCmd->length = pCmd->length + sizeof(EC_T_WORD);
    }

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  The function looks in all objects of the dictionary after the indicated index and
        returns a handle if found..
\return A handle to an object of the requested index or EC_NULL if not found.
*/
EC_T_OBJECTDICT* CEcSdoServ::GetObjectHandle(
    EC_T_WORD   wIndex  /**< [in]   Indicates the index of the dictionary object. */
                                            )
{
EC_T_OBJECTDICT* pRetVal   = EC_NULL;
EC_T_OBJECTDICT* pObjEntry = EC_NULL;

    if (   (EC_NULL == m_pObjectDict)
        || (EC_NULL == m_apSlaveObjects)
        || (EC_NULL == m_apMasterSlave)
        || (EC_NULL == m_apCfgSlaveObjects)
        || (EC_NULL == m_apConSlaveObjects)
        || (EC_NULL == m_apDiagSlaveObjects)
        || (EC_NULL == m_apCfgAddressListObjects)
        || (EC_NULL == m_apConAddressListObjects))
    {
        goto Exit;
    }
    if (wIndex < COEOBJID_SLAVECFGINFOBASE)
    {
        pObjEntry = m_pObjectDict;
    }
    else if ((wIndex >= COEOBJID_SLAVECFGINFOBASE) && (wIndex < COEOBJID_SLAVECFGINFOBASE + m_wMaxSlavesObjs))
    {
        pObjEntry = m_pObject3000 + (wIndex - COEOBJID_SLAVECFGINFOBASE);
    }
    else if ((wIndex >= COEOBJID_SLAVECFGBASE) && (wIndex < COEOBJID_SLAVECFGBASE + m_wMaxSlavesObjs))
    {
        pObjEntry = m_pObject8000 + (wIndex - COEOBJID_SLAVECFGBASE);
    }
    else if ((wIndex >= COEOBJID_SLAVEINFBASE) && (wIndex < COEOBJID_SLAVEINFBASE + m_wMaxSlavesObjs))
    {
        pObjEntry = m_pObject9000 + (wIndex - COEOBJID_SLAVEINFBASE);
    }
    else if ((wIndex >= COEOBJID_SLAVEDIAGBASE) && (wIndex < COEOBJID_SLAVEDIAGBASE + m_wMaxSlavesObjs))
    {
        pObjEntry = m_pObjectA000 + (wIndex - COEOBJID_SLAVEDIAGBASE);
    }
    else if (wIndex >= COEOBJID_DEVICEPROFILE)
    {
        pObjEntry = m_pObjectF000;
    }
    if (EC_NULL == pObjEntry)
    {
        goto Exit;
    }
    for (; pObjEntry->wIndex != COEOBJID_ENDLIST; pObjEntry++)
    {
        if (pObjEntry->wIndex == wIndex)
        {
            pRetVal = pObjEntry;
            break;
        }
    }
Exit:
    return pRetVal;
}

/***************************************************************************************************/
/**
\brief  The function copies the Number in the string pStr, which shall be initialized with 000.

The detailed description and the brief descriptions are separated by on blank line.
This function appears only in the detailed documentation.
*/
EC_T_VOID CEcSdoServ::CopyNumberToString(
    EC_T_BYTE*  pbyStr,     /**< [in]   Pointer to a string.*/
    EC_T_BYTE   byNumber    /**< [in]   Number to be included in the string */
                                        )
{
    EC_T_BYTE   byModulo    = 0;

    if( EC_NULL == pbyStr )
    {
        goto Exit;
    }

    pbyStr[2]   = '0';
    pbyStr[0]   = (EC_T_BYTE)((pbyStr[0])+(byNumber / 100));
    byModulo    = (EC_T_BYTE)(byNumber % 100);
    pbyStr[1]   = (EC_T_BYTE)((pbyStr[1])+(byModulo / 10));
    pbyStr[2]   = (EC_T_BYTE)((pbyStr[2])+(byModulo % 10));

Exit:
    return;
}


/***************************************************************************************************/
/**
\brief  The function returns size and description string of the requested entry.

  Its possible to define all description strings of one entry ( including the name
  of the object ) in one structure: <br>
  { <br>
  name_of_object with index,    <br>
  description_of_subindex0, <br>
  . <br>
  description_of_subindexN, <br>
  0xFF  <br>
  }
  \return The size in bytes of the description string (without null termination byte ).
          0 will be returned if a description for the indicated entry was not found.
*/
EC_T_WORD CEcSdoServ::GetDesc(
    EC_T_WORD           wIndex,
    EC_T_WORD           bySubindex,     /**< [in]   Indicates the subindex of the dictionary object.
                                          *         Subindex 0xffff returns the description of the whole object ( the name of
                                          *         the object ).
                                          *         Subindex 0x00 returns the description of the subindex 0 and so on.
                                          */
    EC_T_OBJECTDICT*    pObjEntry,      /**< [in]   Is a handle to the dictionary object ( for faster access ) or EC_NULL */
    EC_T_WORD*          pwData          /**< [in]   Is the memory field for the description string or EC_NULL ( if the size of
                                          *         string is unknown )
                                          */
                             )
{
    EC_T_WORD           wStrSize    = 0;
    EC_T_BYTE*          pbyDesc     = EC_NULL;
    EC_T_BYTE           byObjCode   = 0;
    EC_T_BOOL           bUseSubDesc = EC_TRUE;

    if( EC_NULL == pObjEntry )
    {
        wStrSize = 0;
        goto Exit;
    }

    pbyDesc = pObjEntry->pName;

    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    byObjCode = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT);

    if ( (bySubindex == 0xFFFF) || (byObjCode == OBJCODE_VAR) )
    {
        /* Get object description length */
        wStrSize = (EC_T_WORD)OsStrlen( (const EC_T_CHAR*) pbyDesc );

        /* If there is a pointer given, copy data: */
        if ( pwData )
        {
            OsMemcpy( pwData, pbyDesc, wStrSize );
        }
    }
    else
    {
        if ( bySubindex == 0 )
        {
            wStrSize = sizeof(C_abySubindexDesc);
            if ( pwData )
            {
                OsMemcpy( pwData, C_abySubindexDesc, wStrSize );
            }
        }
        else if ( byObjCode == OBJCODE_REC )
        {
            /* get pointer to description of subindex 1 : */
            EC_T_WORD i = 1;
            EC_T_WORD tmpSubindex = bySubindex;

            if(wIndex == 0x10F3 && bySubindex > 6)
            {
                /* all diagnosis message have the same entry description name */
                tmpSubindex = 6;
            }

            if(wIndex == COEOBJID_NOTIFY_COUNTER && bySubindex >= NOTIFICATION_MEMBER_COUNT)
            {
              /* all notification counter messages have the same entry description name */
              if (bySubindex % 2 == 0)
              {
                wStrSize = (EC_T_WORD)OsStrlen( NOTIFICATION_TEXT_CODE );
                if (pwData)
                  OsSnprintf((EC_T_CHAR*)pwData, (EC_T_INT)OsStrlen(NOTIFICATION_TEXT_CODE) + 1, NOTIFICATION_TEXT_CODE, (bySubindex - NOTIFICATION_MEMBER_COUNT)/2);
              }
              else
              {
                wStrSize = (EC_T_WORD)OsStrlen( NOTIFICATION_TEXT_COUNT );
                if (pwData)
                  OsSnprintf((EC_T_CHAR*)pwData, (EC_T_INT)OsStrlen(NOTIFICATION_TEXT_COUNT) + 1, NOTIFICATION_TEXT_COUNT, (bySubindex - NOTIFICATION_MEMBER_COUNT)/2);
              }

              bUseSubDesc = EC_FALSE;
            }

            const EC_T_BYTE* c_pbySubDesc = (const EC_T_BYTE*) OBJGETNEXTSTR( pbyDesc );

            if ( bUseSubDesc && c_pbySubDesc[0] != 0xFF && c_pbySubDesc[0] != 0xFE )
            {
                while (i <= tmpSubindex)
                {
                    if ( i == tmpSubindex )
                    {
                        wStrSize = (EC_T_WORD)OsStrlen( (const EC_T_CHAR*) c_pbySubDesc );
                        if ( pwData && wStrSize )
                        {
                            OsMemcpy( pwData, c_pbySubDesc, wStrSize );
                        }
                        break;
                    }
                    else
                    {
                        i++;
                        c_pbySubDesc = (const EC_T_BYTE*) OBJGETNEXTSTR( c_pbySubDesc );
                    }
                }
            }
        }

        if (0 == wStrSize)
        {
            /* no string found for subindex x -> name is Subindex x */
            wStrSize = sizeof(C_abySubindexDesc) - 1;

            if (EC_NULL != pwData)
            {
                OsMemcpy(pwData, C_abySubindexDesc, sizeof(C_abySubindexDesc));
                CopyNumberToString(&((EC_T_BYTE*)pwData)[9], (EC_T_BYTE)bySubindex);
            }
        }
    }

Exit:
    return wStrSize;
}

/***************************************************************************************************/
/**
\brief  Get Object description.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::GetObjDescription(
    PEcMailboxCmd   pCmd
                                        )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;

    EC_T_OBJECTDICT*        pObjEntry   = EC_NULL;
    EC_T_WORD               wSize       = 0;
    EC_T_BYTE*              pbyData     = EC_NULL;

    /* get the object handle of the requested index */
    pObjEntry = GetObjectHandle( pCmd->sIndex.index );

    if ( EC_NULL == pObjEntry )
    {
        dwRetVal = EC_E_SDO_ABORTCODE_INDEX;
        goto Exit;
    }

    /* first clear data */
    OsMemset((pCmd->pbyData), 0, (pCmd->length));

    pbyData = pCmd->pbyData;

    EC_SET_FRM_WORD(&(pbyData[0]), pCmd->sIndex.index);

    /* object exists */
    /* object description is requested */
#if (defined EC_BIG_ENDIAN)
    EC_SET_FRM_WORD(&(pbyData[2]), pObjEntry->oObjDesc.wDataType);
    EC_SET_FRM_WORD(&(pbyData[4]), pObjEntry->oObjDesc.wObjFlags);
#else
    OsMemcpy(&(pbyData[2]), &(pObjEntry->oObjDesc), sizeof(EC_T_SDOINFOOBJDESC));
#endif

    /* the mailbox should be big enough that the maximum object description
       fits in the mailbox (the fragmentation is not done in the sample code),
       so it will be checked only if the object description fits */
    wSize = (EC_T_WORD)(GetDesc(pCmd->sIndex.index, 0xFFFF, pObjEntry, EC_NULL) + sizeof(EC_T_SDOINFOOBJDESC));

    if ( wSize > (pCmd->length-SDO_HDR_DATA_OFFSET) )
    {
        dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
        goto Exit;
    }

    /* object description fits in the mailbox, get the name of the object */
    wSize = GetDesc(pCmd->sIndex.index, 0xFFFF, pObjEntry, (EC_T_WORD*)(&(pbyData[6])));

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Get Entry description.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::GetEntryDescription(
    PEcMailboxCmd   pCmd
                                        )
{
    EC_T_DWORD              dwRetVal        = EC_E_ERROR;
    EC_T_OBJECTDICT*        pObjEntry       = EC_NULL;
    EC_T_WORD               wSize           = 0;
    EC_T_BYTE*              pbyData         = EC_NULL;
    EC_T_BYTE               bySubindex      = 0;
    EC_T_BYTE               byMaxSubindex   = 0;
    EC_T_SDOINFOENTRYDESC*  pInfoEntryDesc  = EC_NULL;

    /* get the object handle of the requested index */
    pObjEntry = GetObjectHandle( pCmd->sIndex.index );

    if ( EC_NULL == pObjEntry )
    {
        dwRetVal = EC_E_SDO_ABORTCODE_INDEX;
        goto Exit;
    }

    /* first clear data */
    OsMemset(pCmd->pbyData, 0, pCmd->length);

    /* entry description is requested, get the requested subindex */
    bySubindex = pCmd->sIndex.subIndex;

    /* get the maximum subindex */
    byMaxSubindex = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT);

    if (bySubindex <= byMaxSubindex)
    {
        /* requested subindex is not too great */
        pbyData = pCmd->pbyData;

        EC_SET_FRM_WORD(&(pbyData[0]), pCmd->sIndex.index);
        pbyData[2] = bySubindex;
        pbyData[3] = 0;       /* value Info */

        /* object exists */
        /* get the entry description of the requested entry */
        pInfoEntryDesc = GetEntryDesc(pObjEntry, bySubindex);
        if( EC_NULL == pInfoEntryDesc)
        {
            dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
            goto Exit;
        }

        if(pCmd->sIndex.index == COEOBJID_HISTORY_OBJECT && bySubindex >= 6)
        {
            pInfoEntryDesc->wBitLength = (EC_T_WORD)(m_aDiagMessages[(bySubindex-6)].wMessageLength * 8);  /* length in bits */
        }

        if(pCmd->sIndex.index == COEOBJID_NOTIFY_COUNTER && bySubindex >= NOTIFICATION_MEMBER_COUNT)
        {
          pInfoEntryDesc->wBitLength = sizeof(EC_T_DWORD)*8;
        }

#if (defined EC_BIG_ENDIAN)
        EC_SET_FRM_WORD(&(pbyData[4]), pInfoEntryDesc->wDataType);
        EC_SET_FRM_WORD(&(pbyData[6]), pInfoEntryDesc->wBitLength);
        EC_SET_FRM_WORD(&(pbyData[8]), pInfoEntryDesc->wObjAccess);
#else
        OsMemcpy(&(pbyData[4]), pInfoEntryDesc, sizeof(EC_T_SDOINFOENTRYDESC));
#endif

        /* the mailbox should be big enough that the maximum entry description
           fits in the mailbox (the fragmentation is not done in the sample code),
           so it will be checked only if the entry description fits */
        wSize = (EC_T_WORD)(GetDesc(pCmd->sIndex.index, bySubindex, pObjEntry, EC_NULL)  + sizeof(EC_T_SDOINFOENTRYDESC));

        if ( wSize > (pCmd->length-SDO_HDR_DATA_OFFSET) )
        {
            dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
            goto Exit;
        }

        /* object description fits in the mailbox, get the name of the entry */
        wSize = GetDesc(pCmd->sIndex.index, bySubindex, pObjEntry, (EC_T_WORD*)(&(pbyData[10])));
    }
    else
    {
        dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

EC_T_DWORD CEcSdoServ::GetEntryBitLength(
    EC_T_WORD               wIndex,
    EC_T_BYTE               bySubindex,
    EC_T_WORD*              pwBitLength
                                      )
{
    EC_T_DWORD              dwRetVal    = EC_E_ERROR;
    EC_T_OBJECTDICT*        pObjEntry   = EC_NULL;
    EC_T_SDOINFOENTRYDESC*  pEntry      = EC_NULL;

    if (EC_NULL == pwBitLength)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pObjEntry = GetObjectHandle(wIndex);
    if (EC_NULL == pObjEntry)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    pEntry = GetEntryDesc(pObjEntry, bySubindex);
    if (EC_NULL == pObjEntry)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }

    *pwBitLength = pEntry->wBitLength;

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

/***************************************************************************************************/
/**
 \param    index              Index of the requested object.
 \param    subindex           Subindex of the requested object.
 \param    dataSize           Size of the requested object data, calculated with OBJ_GetObjectLength
 \param    pData                    Pointer to the buffer where the data can be copied to
 \param    bCompleteAccess    Indicates if a complete read of all subindices of the
                                     object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief    This function reads the object 0x10F3
*/

EC_T_DWORD CEcSdoServ::Read0x10F3( EC_T_BYTE subindex, EC_T_DWORD dataSize, EC_T_BYTE* pData, EC_T_BOOL bCompleteAccess )
{
    if ( bCompleteAccess )
        /* Complete Access is not supported for object 0x10F3 */
        return EC_E_SDO_ABORTCODE_ACCESS;

    if(subindex == 0)
    {
        *pData = (EC_T_BYTE)m_oHistoryObject.wSubIndex0;
    }
    else if (subindex == 1)
    {
        /* Maximum Messages*/
        *pData = m_oHistoryObject.byMaxDiagMessages;
    }
    else if (subindex == 2)
    {
        /* Newest Message*/
        *pData = m_oHistoryObject.byNewestMessage;
    }
    else if (subindex == 3)
    {
        /* Newest Acknowledged Message*/
        *pData = m_oHistoryObject.byNewestAckMessage;
    }
    else if (subindex == 4)
    {
        /* New Message Available*/
        *pData = (EC_T_BYTE)m_oHistoryObject.byNewDiagMessages;
    }
    else if (subindex == 5)
    {
        /* Flags*/
        *((EC_T_WORD*)pData) = m_oHistoryObject.wFlags;
    }
    else if (subindex <= m_oHistoryObject.wSubIndex0)
    {
        m_byLastDiagMsgRead = subindex;

        if (m_byLastDiagMsgRead == m_oHistoryObject.byNewestMessage)
            m_oHistoryObject.byNewDiagMessages = 0;

        if (m_aDiagMessages[(subindex-6)].wMessageLength > 0)
        {
            dataSize = EC_MIN(dataSize, m_aDiagMessages[(subindex-6)].wMessageLength);
            OsMemcpy(pData, m_aDiagMessages[(subindex-6)].pMessage, dataSize);
        }
    }
    else
    {
        return EC_E_SDO_ABORTCODE_OFFSET;
    }

    return 0;
}


/***************************************************************************************************/
/**
 \param    index              Index of the requested object.
 \param    subindex           Subindex of the requested object.
 \param    dataSize           Size of the requested object data, calculated with OBJ_GetObjectLength
 \param    pData                    Pointer to the buffer where the data can be copied to
 \param    bCompleteAccess    Indicates if a complete read of all subindices of the
                                     object shall be done or not

 \return    result of the read operation (0 (success) or an abort code (ABORTIDX_.... defined in
            sdosrv.h))

 \brief    This function reads the object 0x10F3
*/

EC_T_DWORD CEcSdoServ::Read0x2004(EC_T_BYTE subindex, EC_T_DWORD dataSize, EC_T_BYTE* pData, EC_T_BOOL bCompleteAccess)
{
    if (bCompleteAccess)
        /* Complete Access is not supported for object 0x10F3 */
        return EC_E_SDO_ABORTCODE_ACCESS;

    if (subindex == 0)
    {
        *pData = (EC_T_BYTE)m_oNotifyCounterObject.wSubIndex0;
    }
    else if (subindex == 1)
    {
        /* Maximum Messages */
        *pData = m_oNotifyCounterObject.byMaxMessages;
    }
    else if (subindex == 2)
    {
        /* Message Count */
        *pData = m_oNotifyCounterObject.byMessageCount;
    }
    else if (subindex == 3)
    {
      /* Flags */
      *pData = m_oNotifyCounterObject.byFlags;
    }
    else if (subindex <= m_oNotifyCounterObject.wSubIndex0)
    {
        dataSize = EC_MIN(dataSize, sizeof(EC_T_DWORD));

        if (subindex % 2 == 0)
          OsMemcpy(pData, &m_aNotifyMessages[(subindex-NOTIFICATION_MEMBER_COUNT)/2].dwCode, dataSize);
        else
          OsMemcpy(pData, &m_aNotifyMessages[(subindex-NOTIFICATION_MEMBER_COUNT)/2].dwCount, dataSize);
    }
    else
    {
        return EC_E_SDO_ABORTCODE_OFFSET;
    }

    return 0;
}

#if (defined INCLUDE_MAILBOX_STATISTICS)
EC_T_DWORD CEcSdoServ::Read0x2006(EC_T_BYTE bySubindex, EC_T_DWORD dwSize, EC_T_BYTE* pData, EC_T_BOOL bCompleteAccess)
{
    if (bySubindex > 65)
    {
        return EC_E_SDO_ABORTCODE_OFFSET;
    }

    if (bCompleteAccess)
    {
        EC_T_BYTE byStatisticsIdx = 0;

        /* Complete Access is not supported for subindex > 1 */
        if (bySubindex > 1)
            return EC_E_SDO_ABORTCODE_ACCESS;

        /* Complete Access for bySubindex 0 (not 1) */
        if (bySubindex == 0)
        {
            Read0x2006(0, sizeof(EC_T_WORD), pData, EC_FALSE);
            pData = pData + sizeof(EC_T_WORD);
        }

        for (byStatisticsIdx = 0; byStatisticsIdx < 64; byStatisticsIdx++)
        {
            Read0x2006((EC_T_BYTE)(1 + byStatisticsIdx), sizeof(EC_T_DWORD), pData, EC_FALSE);
            pData = pData + sizeof(EC_T_DWORD);
        }

        Read0x2006(65, sizeof(EC_T_UINT64), pData, EC_FALSE);
        return EC_E_NOERROR;
    }

    switch (bySubindex)
    {
    case 0:
        *pData = 65;
        return EC_E_NOERROR;
        /* no break */
    case 65:
        if (dwSize < sizeof(EC_T_UINT64))
        {
            return EC_E_SDO_ABORTCODE_DATA_SIZE;
        }
        EC_SET_FRM_QWORD((EC_T_UINT64*)pData, m_qwMailboxStatisticsClearCounters);
        return EC_E_NOERROR;
        /* no break */
    }

    switch ((bySubindex - 1) % 8)
    {
    case 0: /* Total Read Transfer Count */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Read.Cnt.dwTotal);
        break;
    case 1: /* Read Transfer Count Last Second */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Read.Cnt.dwLast);
        break;
    case 2: /* Total Bytes Read */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Read.Bytes.dwTotal);
        break;
    case 3: /* Bytes Read Last Second */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Read.Bytes.dwLast);
        break;
    case 4: /* Total Write Transfer Count */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Write.Cnt.dwTotal);
        break;
    case 5: /* Write Transfer Count Last Second */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Write.Cnt.dwLast);
        break;
    case 6: /* Total Bytes Write */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Write.Bytes.dwTotal);
        break;
    case 7: /* Bytes Write Last Second */
        EC_SET_FRM_DWORD((EC_T_DWORD*)pData, m_pMaster->m_MailboxStatistics[(bySubindex - 1) / 8].Write.Bytes.dwLast);
        break;
    }

    return EC_E_NOERROR;
}
#endif /* INCLUDE_MAILBOX_STATISTICS */

/***************************************************************************************************/
/**
\brief  Read value from OD.

Reads value from OD, where standard read is not possible.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::ActiveRead(
    EC_T_WORD   wIndex,         /**< [in]   OD Index */
    EC_T_BYTE   bySubindex,     /**< [in]   OD SubIndex */
    EC_T_DWORD  dwSize,         /**< [in]   Data Size */
    EC_T_WORD*  pwData,         /**< [in]   Data Buffer */
    EC_T_BOOL   bCompleteAccess /**< [in]   Complete access requested ? */
                                 )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    switch (wIndex)
    {
    case COEOBJID_HISTORY_OBJECT:
        dwRetVal = Read0x10F3(bySubindex, dwSize, (EC_T_BYTE*)pwData, bCompleteAccess);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
        break;

    case COEOBJID_NOTIFY_COUNTER:
        dwRetVal = Read0x2004(bySubindex, dwSize, (EC_T_BYTE*)pwData, bCompleteAccess);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
        break;

    case COEOBJID_MAST_STATESUMMARY:
        {
            EC_T_BOOL   bMasterOkFlag   = EC_FALSE;
            EC_T_BOOL   bSlavesInReqSt  = EC_FALSE;
            EC_T_WORD   wMasterState    = DEVICE_STATE_UNKNOWN;

            if (dwSize < sizeof(EC_T_DWORD))
            {
                dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                goto Exit;
            }

            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }

            /* reset OK flag */
            m_dwMasterStateChng &= ~1;

            wMasterState = m_pMaster->GetMasterDeviceState();

            /* determine if slaves are not in master state */
            switch (wMasterState)
            {
            case DEVICE_STATE_INIT:         if( (m_wAlStatusBrdValue & ~DEVICE_STATE_INIT)    == 0 ){ bSlavesInReqSt = EC_TRUE;} break;
            case DEVICE_STATE_PREOP:        if( (m_wAlStatusBrdValue & ~DEVICE_STATE_PREOP)   == 0 ){ bSlavesInReqSt = EC_TRUE;} break;
            case DEVICE_STATE_SAFEOP:       if( (m_wAlStatusBrdValue & ~DEVICE_STATE_SAFEOP)  == 0 ){ bSlavesInReqSt = EC_TRUE;} break;
            case DEVICE_STATE_OP:           if( (m_wAlStatusBrdValue & ~DEVICE_STATE_OP)      == 0 ){ bSlavesInReqSt = EC_TRUE;} break;
            case DEVICE_STATE_UNKNOWN:
            default:
                {
                    bSlavesInReqSt = EC_FALSE;
                } break;
            }

            /* set the "Slaves in requested State" bit */
            if (bSlavesInReqSt && m_bIsAlStatusWkcOk)
            {
                m_dwMasterStateSummary |= OBJ2001_STATE_SUM_SLAVE_REQ;
            }
            else
            {
                m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_SLAVE_REQ;
            }

            /* evaluate ok flag */

            if (((m_dwMasterStateSummary & OBJ2001_STATE_SUM_MASK1) == OBJ2001_STATE_SUM_VALUE1))
            {
                if (((m_dwMasterStateSummary & OBJ2001_STATE_SUM_DC_ENA) == OBJ2001_STATE_SUM_DC_ENA))  /* if dc enabled check DC values */
                {
                    if (((m_dwMasterStateSummary & OBJ2001_STATE_SUM_MASK2) == OBJ2001_STATE_SUM_VALUE2))
                    {
                        bMasterOkFlag = EC_TRUE;
                    }
                }
                else
                {
                    bMasterOkFlag = EC_TRUE;
                }
            }

            /* set OK Flag */
            if (bMasterOkFlag)
            {
                m_dwMasterStateSummary |= OBJ2001_STATE_SUM_MASTER_OK;
            }
            else
            {
                m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_MASTER_OK;
            }
            /* store value */
            EC_SET_FRM_DWORD(&(pwData[0]), m_dwMasterStateSummary);
        } break;
#if (defined INCLUDE_MAILBOX_STATISTICS)
    case COEOBJID_MAILBOX_STATISTICS:
        dwRetVal = Read0x2006(bySubindex, dwSize, (EC_T_BYTE*)pwData, bCompleteAccess);
        if (EC_E_NOERROR != dwRetVal)
        {
            goto Exit;
        }
        break;
#endif /* INCLUDE_MAILBOX_STATISTICS */
    case COEOBJID_DEBUG_REGISTER:
        {
            if (dwSize < (2 * sizeof(EC_T_DWORD)))
            {
                dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                goto Exit;
            }

            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }

            EC_SET_FRM_DWORD(&(pwData[0]), (m_pMaster->m_MasterConfig.dwStateChangeDebug));
            EC_SET_FRM_DWORD(&(pwData[2]), (m_pMaster->m_MasterConfig.dwErrorMsgToLinkLayer));
        } break;

#if (defined INCLUDE_DC_SUPPORT)
    case COEOBJID_DC_CURDEVIATION:
        {
            if( dwSize < sizeof(EC_T_DWORD) )
            {
                dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                goto Exit;
            }

            if( bCompleteAccess )
            {
                dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                goto Exit;
            }
            if( m_pMaster->GetDcSupportEnabled() )
            {
                EC_T_DC_SYNC_NTFY_DESC* pSlaveSync  = EC_NULL;
                EC_T_DWORD              dwValue     = 0;

                pSlaveSync = m_pMaster->m_pDistributedClks->GetSlaveSyncNotification();

                dwValue = pSlaveSync->dwDeviation;
                if( pSlaveSync->IsNegative )
                {
                    EC_SET_FRM_DWORD(&(pwData[0]), (EC_T_DWORD)(-1*((EC_T_INT)dwValue)));
                }
                else
                {
                    EC_SET_FRM_DWORD(&(pwData[0]), dwValue);
                }
            }
            else
            {
                EC_SET_FRM_DWORD(&(pwData[0]), EC_E_FEATURE_DISABLED);
            }
        } break;
#endif /* INCLUDE_DC_SUPPORT */
    default:
        {
            EC_UNREFPARM(wIndex);
            EC_UNREFPARM(bySubindex);
            EC_UNREFPARM(dwSize);
            EC_UNREFPARM(pwData);
            EC_UNREFPARM(bCompleteAccess);
            dwRetVal = EC_E_SDO_ABORTCODE_INDEX;
            goto Exit;
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}


EC_T_VOID CEcSdoServ::ClearBusDiagnosisCounters(EC_T_DWORD dwClear)
{
    m_oBusDiagnosisObject.dwClearCounters = dwClear;

    if ((dwClear & 0x01) == 0x01)
    {
        m_oBusDiagnosisObject.dwTXFrames = 0;
        m_oBusDiagnosisObject.dwRXFrames = 0;
        m_oBusDiagnosisObject.dwLostFrames = 0;
        m_oBusDiagnosisObject.dwCyclicFrames = 0;
        m_oBusDiagnosisObject.dwCyclicDatagrams = 0;
        m_oBusDiagnosisObject.dwAcyclicFrames = 0;
        m_oBusDiagnosisObject.dwAcyclicDatagrams = 0;
    }
    else if ((dwClear & 0x02) == 0x02)
    {
        m_oBusDiagnosisObject.dwTXFrames = 0;
    }
    else if ((dwClear & 0x04) == 0x04)
    {
        m_oBusDiagnosisObject.dwRXFrames = 0;
    }
    else if ((dwClear & 0x08) == 0x08)
    {
        m_oBusDiagnosisObject.dwLostFrames = 0;
    }
    else if ((dwClear & 0x10) == 0x10)
    {
        m_oBusDiagnosisObject.dwCyclicFrames = 0;
    }
    else if ((dwClear & 0x20) == 0x20)
    {
        m_oBusDiagnosisObject.dwCyclicDatagrams = 0;
    }
    else if ((dwClear & 0x40) == 0x40)
    {
        m_oBusDiagnosisObject.dwAcyclicFrames = 0;
    }
    else if ((dwClear & 0x80) == 0x80)
    {
        m_oBusDiagnosisObject.dwAcyclicDatagrams = 0;
    }
}

/**************************************************************************************************
 brief    This functions clears the message history.
 In overwrite mode all message are deleted in acknowledge mode only acknowledged message are deleted
*/
EC_T_DWORD CEcSdoServ::ClearMessageHistory(EC_T_VOID)
{
    EC_T_DWORD dwRetVal = 0;

    if((!(m_oHistoryObject.wFlags & DIAG_OPERATION_MODE))
        || (m_oHistoryObject.byNewDiagMessages == 0 ))
    {
        /*Operation Mode Bit (0x10F3.5 bit4) is 0 => Overwrite Mode is active
         OR no non acknowledged message available (SI4 = 0)*/
        EC_T_WORD wCnt = 0;

        for(wCnt = 0; wCnt < MAX_DIAG_MSG; wCnt++)
        {
            m_aDiagMessages[wCnt].wMessageLength = 0;
        }
        m_oHistoryObject.wSubIndex0 = 5;
        m_pEntry0x10F3->oObjDesc.wObjFlags = (EC_T_WORD)(m_oHistoryObject.wSubIndex0 | (OBJCODE_REC << 8));

        /*set status entries to startup values*/
        m_oHistoryObject.byNewestMessage = 0;
        m_oHistoryObject.byNewDiagMessages = 0;
    }
    else
    {
        /*Acknowledge Mode is active and at least one non acknowledged message is available*/
        EC_T_BYTE byMessageLeft = 0; /* indicates how many messages are left in the message history queue */

        if (m_oHistoryObject.byNewestAckMessage < m_oHistoryObject.byNewestMessage)
        {
            /* messages from SI6 to "Newest Acknowledged Message" will be deleted
               messages from "Newest Acknowledged Message" to "Newest Message" will be shifted to the beginning of the message history queue*/
            EC_T_WORD wCnt = 0;
            EC_T_WORD wShiftCnt = 0;

            /* delete messages */
            for (wCnt = 0; wCnt < (m_oHistoryObject.byNewestAckMessage - 6) ; wCnt++)
            {
                m_aDiagMessages[wCnt].wMessageLength =0;
            }

            /* shift messages */
            wCnt = 0;
            for (wShiftCnt = (EC_T_WORD)(m_oHistoryObject.byNewestAckMessage - 6); wShiftCnt < (EC_T_WORD)(m_oHistoryObject.byNewestMessage - 6); wShiftCnt++)
            {
                m_aDiagMessages[wCnt].wMessageLength = m_aDiagMessages[wShiftCnt].wMessageLength;
                m_aDiagMessages[wCnt].pMessage = m_aDiagMessages[wShiftCnt].pMessage;
                wCnt++;

                m_aDiagMessages[wShiftCnt].wMessageLength = 0;
            }

            byMessageLeft = (EC_T_BYTE)(m_oHistoryObject.byNewestAckMessage - m_oHistoryObject.byNewestMessage);
        }
        else
        {
           /* messages from SI6 to "Newest Message" and from "Newest Acknowledged Message" to "Subindex0" will be deleted
              messages from "Newest Message" to "Newest Acknowledged Message" will be  shifted to the beginning of the message history queue*/
            EC_T_WORD wCnt = 0;
            EC_T_WORD wShiftCnt = 0;

            /*delete messages SI6 to "Newest Message"*/
            for (wCnt = 0; wCnt < (m_oHistoryObject.byNewestMessage - 6) ; wCnt++)
            {
                m_aDiagMessages[wCnt].wMessageLength = 0;
            }

            /*delete messages "Newest Acknowledged Message" to "Subindex0"*/
            for (wCnt = (EC_T_WORD)(m_oHistoryObject.byNewestAckMessage - 6); wCnt < (EC_T_WORD)MAX_DIAG_MSG; wCnt++)
            {
                m_aDiagMessages[wCnt].wMessageLength = 0;
            }

            /*shift messages between "Newest Message" and "Newest Acknowledged Message" to the beginning of the message history queue*/
            wCnt = 0;
            for (wShiftCnt = (EC_T_WORD)(m_oHistoryObject.byNewestMessage - 6); wShiftCnt < (EC_T_WORD)(m_oHistoryObject.byNewestAckMessage - 6); wShiftCnt++)
            {
                m_aDiagMessages[wCnt].wMessageLength = m_aDiagMessages[wShiftCnt].wMessageLength;
                m_aDiagMessages[wCnt].pMessage = m_aDiagMessages[wShiftCnt].pMessage;
                wCnt++;

                m_aDiagMessages[wShiftCnt].wMessageLength = 0;
            }

            byMessageLeft = (EC_T_BYTE)(m_oHistoryObject.byNewestMessage - m_oHistoryObject.byNewestAckMessage);
        }

        m_oHistoryObject.wSubIndex0 = (EC_T_WORD)(5 + byMessageLeft);
        m_pEntry0x10F3->oObjDesc.wObjFlags = (EC_T_WORD)(m_oHistoryObject.wSubIndex0 | (OBJCODE_REC << 8));
        m_oHistoryObject.byNewestMessage = 6;
    }

    /*Reset Overwrite/Discard Indication*/
    m_oHistoryObject.wFlags &= ~((EC_T_WORD)DIAG_OVERWRITE_DISCARD);

    return dwRetVal;
}


/***************************************************************************************************/
/**
\brief  Write value to OD.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::ActiveWrite(
    EC_T_WORD   wIndex,         /**< [in]   OD Index */
    EC_T_BYTE   bySubindex,     /**< [in]   OD SubIndex */
    EC_T_DWORD  dwSize,         /**< [in]   Data Size */
    EC_T_WORD*  pwData,         /**< [in]   Data Buffer */
    EC_T_BOOL   bCompleteAccess /**< [in]   Complete access requested ? */
                                  )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    switch (wIndex)
    {
    case COEOBJID_HISTORY_OBJECT:
        {
            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }

            switch (bySubindex)
            {
            case 3: /* SubIndexMessageAck */
                {
                    /*Write "Newest Acknowledged Message" (Subindex 3) */
                    EC_T_BYTE NewValueSi3 = ((EC_T_BYTE*)pwData)[0];

                    if(NewValueSi3 == 0)
                    {
                        /*New value for subindex3 is 0 => Clear Message history*/
                        dwRetVal = ClearMessageHistory();
                        m_oHistoryObject.byNewestAckMessage = 0;
                    }
                    else if (NewValueSi3 >= 6 && NewValueSi3 <= (EC_T_BYTE)m_oHistoryObject.wSubIndex0)
                    {
                        /*Messages need to be acknowledged*/
                        m_oHistoryObject.byNewestAckMessage = NewValueSi3;

                        /*Reset overwrite status bit*/
                        m_oHistoryObject.wFlags &= ~((EC_T_WORD)DIAG_OVERWRITE_DISCARD);

                        if(m_oHistoryObject.wFlags & DIAG_OPERATION_MODE)
                        {
                            /*Update "New Messages Available" Indication (only in acknowledge mode); in overwrite mode "New Message Available" is updated when diagnosis messages are read*/
                            if(m_oHistoryObject.byNewestMessage == m_oHistoryObject.byNewestAckMessage)
                                m_oHistoryObject.byNewDiagMessages = 0;
                        }
                    }
                    else
                    {
                        dwRetVal = EC_E_SDO_ABORTCODE_DATA_RANGE;
                        goto Exit;
                    }
                } break;
            case 5:
                {
                    /*
                    * Write "Flags"
                    * Only Bit 1, Bit 2 and Bit 3 are writeable
                    */
                    EC_T_WORD ReadOnlyMask = (EC_T_WORD)~(DIAG_DISABLE_INFO_MSG | DIAG_DISABLE_WARNING_MSG | DIAG_DISABLE_ERROR_MSG);
                    EC_T_BYTE Value = (EC_T_BYTE) ((EC_T_BYTE*)pwData)[0];

                    if(((EC_T_WORD)Value & ReadOnlyMask) == (m_oHistoryObject.wFlags & ReadOnlyMask))
                    {
                        /*Readonly bits are equal => write new flags value*/
                        m_oHistoryObject.wFlags = (EC_T_WORD)Value;
                    }
                    else
                    {
                        /*One or more read only bits changed => return error code (Value exceed)*/
                        dwRetVal = EC_E_SDO_ABORTCODE_DATA_RANGE;
                        goto Exit;
                    }
                } break;
            default:
                {
                    dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
                    goto Exit;
                }
            }
        } break;

    case COEOBJID_NOTIFY_COUNTER:
        {
            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }

            switch (bySubindex)
            {
            case 3:
                {
                    /*
                    * Write "Flags"
                    */
                    EC_T_BYTE Value = (EC_T_BYTE) ((EC_T_BYTE*)pwData)[0];
                    if((Value & NOTIFICATION_FLAGS_CLEAR) != 0)
                    {
                      ClearNotifications();
                    }
                    else
                    {
                        /*One or more read only bits changed => return error code (Value exceed)*/
                        dwRetVal = EC_E_SDO_ABORTCODE_DATA_RANGE;
                    }
                } break;
            default:
                {
                    dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
                    goto Exit;
                }
            }
        } break;

    case COEOBJID_MAST_STATECHNG:
        {
            /* need hook to CAtemInterface ?!!! */
            if (0 != bySubindex)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }

            m_dwMasterStateChng = EC_GET_FRM_DWORD(pwData);

            switch (m_dwMasterStateChng & 0x0f)
            {
            case eMastStChng_init:
                {
                    if (EC_MASTER_STATE_INIT == m_pMaster->GetCurMasterState())
                    {
                        dwRetVal = EC_E_NOERROR;
                        goto Exit;
                    }
#if (defined INCLUDE_MASTER_OBD)
                    if (0xffff != m_pMaster->GetMasterStChngRequest())
                    {
                        dwRetVal = EC_E_BUSY;
                        goto Exit;
                    }
                    m_pMaster->SetMasterStChngRequest(DEVICE_STATE_INIT);
#endif
                } break;
            case eMastStChng_preop:
                {
                    if (EC_MASTER_STATE_PREOP == m_pMaster->GetCurMasterState())
                    {
                        dwRetVal = EC_E_NOERROR;
                        goto Exit;
                    }
#if (defined INCLUDE_MASTER_OBD)
                    if (0xffff != m_pMaster->GetMasterStChngRequest())
                    {
                        dwRetVal = EC_E_BUSY;
                        goto Exit;
                    }
                    m_pMaster->SetMasterStChngRequest(DEVICE_STATE_PREOP);
#endif
                } break;
            case eMastStChng_safeop:
                {
                    if( EC_MASTER_STATE_SAFEOP == m_pMaster->GetCurMasterState() )
                    {
                        dwRetVal = EC_E_NOERROR;
                        goto Exit;
                    }
#if (defined INCLUDE_MASTER_OBD)
                    if (0xffff != m_pMaster->GetMasterStChngRequest())
                    {
                        dwRetVal = EC_E_BUSY;
                        goto Exit;
                    }
                    m_pMaster->SetMasterStChngRequest(DEVICE_STATE_SAFEOP);
#endif
                } break;
            case eMastStChng_op:
                {
                    if (EC_MASTER_STATE_OP == m_pMaster->GetCurMasterState())
                    {
                        dwRetVal = EC_E_NOERROR;
                        goto Exit;
                    }
#if (defined INCLUDE_MASTER_OBD)
                    if (0xffff != m_pMaster->GetMasterStChngRequest())
                    {
                        dwRetVal = EC_E_BUSY;
                        goto Exit;
                    }
                    m_pMaster->SetMasterStChngRequest(DEVICE_STATE_OP);
#endif
                } break;
#if 0 /* glagow likes to have no error on provisioning 0 here */
            case 0:
                {
                    /* do nothing */
                } break;
#endif
            default:
                {
                    dwRetVal = EC_E_SDO_ABORTCODE_GENERAL;
                    goto Exit;
                }
            }
        } break;
    case COEOBJID_DEBUG_REGISTER:
        {
            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }
            if (dwSize < 2 * sizeof(EC_T_DWORD))
            {
                dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                goto Exit;
            }

            m_pMaster->m_MasterConfig.dwStateChangeDebug        = EC_GET_FRM_DWORD(&(pwData[0]));
            m_pMaster->m_MasterConfig.dwErrorMsgToLinkLayer     = EC_GET_FRM_DWORD(&(pwData[2]));
        } break;
    case COEOBJID_BUS_DIAGNOSIS:
        {
            EC_T_DWORD dwClear = 0;

            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }
            if (14 != bySubindex)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_READONLY;
                goto Exit;
            }

            /* Subindex 014 Clear Counters */
            dwClear = EC_GET_FRM_DWORD(&(pwData[0]));
            ClearBusDiagnosisCounters(dwClear);
            dwRetVal = EC_E_NOERROR;
    } break;
#if (defined INCLUDE_MAILBOX_STATISTICS)
    case COEOBJID_MAILBOX_STATISTICS:
        {
            EC_T_UINT64 qwClear = 0;

            if (bCompleteAccess)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
                goto Exit;
            }
            if (65 != bySubindex)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_READONLY;
                goto Exit;
            }

            /* Subindex 065 Clear Counters */
            qwClear = EC_GET_FRM_QWORD(&(pwData[0]));
            m_pMaster->ClearMailboxStatisticsCounters(qwClear);
            m_qwMailboxStatisticsClearCounters = qwClear;
            dwRetVal = EC_E_NOERROR;
        } break;
#endif /* INCLUDE_MAILBOX_STATISTICS */
    case COEOBJID_DETECTMODCMD:
        {
            /* not implemented yet */
            dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
            goto Exit;
        } /* no break */
    default:
        {
            if ((COEOBJID_SLAVECFGINFOBASE <= wIndex)
             && (COEOBJID_SLAVECFGINFOBASE + m_wMaxSlavesObjs > wIndex))
            {
                CEcSlave *pCfgSlave = m_apMasterSlave[wIndex - COEOBJID_SLAVECFGINFOBASE];
                EC_T_OBJ3XXX* pObjSlave = &m_apSlaveObjects[wIndex - COEOBJID_SLAVECFGINFOBASE];

                if( dwSize != sizeof(EC_T_DWORD) )
                {
                    dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                    goto Exit;
                }

                switch (bySubindex)
                {
                case 15:
                    {
                        if (!EC_GETBOOL(&pObjSlave->bEntryValid))
                        {
                            break;
                        }

                        pObjSlave->dwReqState = EC_GET_FRM_DWORD(&(pwData[0]));
                        if (EC_NULL != pCfgSlave)
                        {
                            dwRetVal = pCfgSlave->SetDeviceState(EC_LOWORD(pObjSlave->dwReqState));
                            if (EC_E_NOERROR != dwRetVal)
                            {
                                goto Exit;
                            }
                        }
                    } break;
                case 18:
                    {
                        if (!EC_GETBOOL(&pObjSlave->bEntryValid))
                        {
                            break;
                        }

                        pObjSlave->bEnableLinkMsgs = EC_GET_FRM_DWORD(&(pwData[0]));
                        if (EC_NULL != pCfgSlave)
                        {
                            pCfgSlave->LinkMessages(pObjSlave->bEnableLinkMsgs);
                        }
                    } break;
                default:
                    {
                        dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
                        goto Exit;
                    }
                }
            }
            else if ((COEOBJID_SLAVEDIAGBASE <= wIndex)
                  && (COEOBJID_SLAVEDIAGBASE + m_wMaxSlavesObjs > wIndex))
            {
                CEcSlave *pCfgSlave = (CEcSlave *) m_apMasterSlave[wIndex - COEOBJID_SLAVEDIAGBASE];
                EC_T_OBJAXXX* pObjSlave = &m_apDiagSlaveObjects[wIndex - COEOBJID_SLAVEDIAGBASE];

                if (dwSize != sizeof(EC_T_WORD))
                {
                    dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
                    goto Exit;
                }

                switch (bySubindex)
                {
                case 2: /* AL Control */
                    {
                        pObjSlave->wALControl = EC_GET_FRM_WORD(&pwData[0]);
                        if (EC_NULL != pCfgSlave)
                        {
                            dwRetVal = pCfgSlave->SetDeviceState(EC_LOWORD(pObjSlave->wALControl));
                            if (EC_E_NOERROR != dwRetVal)
                            {
                                goto Exit;
                            }
                        }
                        break;
                    }
                default:
                    {
                        dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
                        goto Exit;
                    }
                }
            }
            else
            {
                dwRetVal = EC_E_SDO_ABORTCODE_INDEX;
                goto Exit;
            }
        }
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  This function returns the size of the requested object.

\return The size of the object entry in bytes (!).
*/
EC_T_WORD CEcSdoServ::GetObjectLength(
    EC_T_WORD           wIndex,         /**< [in]   index of the requested object */
    EC_T_BYTE           bySubindex,     /**< [in]   subindex of the requested object */
    EC_T_OBJECTDICT*    pObjEntry,      /**< [in]   handle to the dictionary object returned by
                                          *         GetObjectHandle which was called before
                                          */
    EC_T_BOOL       bCompleteAccess     /**< [in]   Indicates if a complete read of all subindices of the
                                          *         object shall be done or not
                                          */
                                     )
{
    EC_T_WORD   wLength         = 0;
    EC_T_BYTE   byObjCode       = 0;
    EC_T_BYTE   byMaxSubindex   = 0;
    EC_T_WORD   wSize           = 0;

    if (EC_NULL == pObjEntry)
    {
        wLength = 0;
        goto Exit;
    }

    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    byObjCode = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT);

    byMaxSubindex = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT);

    if (bCompleteAccess)
    {
        if (OBJCODE_VAR == byObjCode)
        {
            wLength = 0;
            goto Exit;
        }
        else if (OBJCODE_ARR == byObjCode)
        {
            /* we have to get the maxSubindex from the actual value of subindex 0,
               which is stored as EC_T_WORD at the beginning of the object's variable */
            byMaxSubindex = EC_LOBYTE(((EC_T_WORD*)(pObjEntry->pVarPtr))[0]);
            wSize = (EC_T_WORD)(((pObjEntry->pEntryDesc[0].wBitLength + 7) >> 3)*byMaxSubindex);

            if ( bySubindex == 0 )
            {
                /* add size for subindex 0 (is transmitted as EC_T_WORD) */
                wSize += 2;
            }
            wLength = wSize;
        }
        else
        {
            EC_T_BYTE i;

            /* add the sizes of all entries */
            for (i = 1; i <= byMaxSubindex; i++)
            {
                wSize = (EC_T_WORD)(wSize + pObjEntry->pEntryDesc[i].wBitLength);

                if (0xff == i)
                    break;
            }
            /* we have to convert the size in bytes */
            wSize = (EC_T_WORD)BIT2BYTE(wSize);

            if (0 == bySubindex)
            {
                /* add size for subindex 0 (is transmitted as EC_T_WORD) */
                wSize += 2;
            }
            wLength = wSize;
        }
    }
    else
    {
        if (OBJCODE_VAR == byObjCode)
        {
            wLength = (EC_T_WORD)(BIT2BYTE(pObjEntry->pEntryDesc->wBitLength));
        }
        else if (0 == bySubindex)
        {
            /* for single access subindex 0 is transmitted as EC_T_BYTE */
            wLength = sizeof(EC_T_BYTE);
        }
        else if (OBJCODE_ARR == byObjCode)
        {
            wLength = (EC_T_WORD)(BIT2BYTE(pObjEntry->pEntryDesc[1].wBitLength));
        }
        else
        {
            if ((COEOBJID_HISTORY_OBJECT == wIndex) && (bySubindex >= 6))
            {
                wLength = m_aDiagMessages[bySubindex - 6].wMessageLength;
            }
            else if ((COEOBJID_NOTIFY_COUNTER == wIndex) && (bySubindex >= NOTIFICATION_MEMBER_COUNT))
            {
                wLength = sizeof(EC_T_DWORD);
            }
            else
            {
                wLength = (EC_T_WORD)(BIT2BYTE(pObjEntry->pEntryDesc[bySubindex].wBitLength));
            }
        }
    }
Exit:
    return wLength;
}

/***************************************************************************************************/
/**
\brief  SDO Upload.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::SDOUpload(
    PEcMailboxCmd   pCmd
                                )
{
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;
    EC_T_OBJECTDICT*    pObjEntry       = EC_NULL;
    EC_T_DWORD          dwObjLength     = 0;
    EC_T_BOOL           bCompleteAccess = EC_FALSE;

    /* get the object handle of the requested index */
    pObjEntry = GetObjectHandle(pCmd->sIndex.index);

    if (EC_NULL == pObjEntry)
    {
        dwRetVal = EC_E_SDO_ABORTCODE_INDEX;
        goto Exit;
    }

    /* first clear data */
    OsMemset(pCmd->pbyData, 0, pCmd->length);

    /* transferType contains the information if the SDO Download Request or the SDO Upload Response
       can be an expedited service (SDO data length <= 4, that means the data is stored in the
       SDO-Header completely */

    /* pData is the pointer to the received (SDO-Download) or sent (SDO-Upload) SDO data in the mailbox */
    bCompleteAccess = ((pCmd->dwMailboxFlags & EC_MAILBOX_FLAG_SDO_COMPLETE) == EC_MAILBOX_FLAG_SDO_COMPLETE);

    dwObjLength = GetObjectLength(pCmd->sIndex.index, pCmd->sIndex.subIndex, pObjEntry, bCompleteAccess);

    /* SDO Upload */

    /* distinguish between expedited and normal upload response within the length of the response data */
    if (dwObjLength > pCmd->length)
    {
        dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
        goto Exit;
    }

    dwRetVal = Read(pCmd->sIndex.index, pCmd->sIndex.subIndex, pCmd->length, pObjEntry, (EC_T_WORD*)pCmd->pbyData, bCompleteAccess);
    if (EC_E_NOERROR == dwRetVal)
    {
        pCmd->length = dwObjLength;
    }

Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  This function calculates the bit offset of the entry in the object's variable.

\return bit offset of the entry in the variable.
*/
EC_T_DWORD CEcSdoServ::GetEntryOffset(
    EC_T_BYTE           bySubindex, /**< [in]   subindex of the entry */
    EC_T_OBJECTDICT*    pObjEntry   /**< [in]   handle to the dictionary object returned by GetObjectHandle
                                      *         which was called before
                                      */
                                    )
{
    EC_T_WORD               i           = 0;
    EC_T_DWORD              dwBitOffset  = 0;
    EC_T_BYTE               byObjCode   = 0;
    EC_T_SDOINFOENTRYDESC*  pEntry      = EC_NULL;

    if( EC_NULL == pObjEntry )
    {
        dwBitOffset = 0;
        goto Exit;
    }

    /* bitOffset will be initialized with the bit offset of subindex 1 */
    dwBitOffset  = 16;
    byObjCode   = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT);

    if ( byObjCode == OBJCODE_VAR )
    {
        dwBitOffset = 0;
        goto Exit;
    }

    for (i = 1; i <= bySubindex; i++)
    {
        /* get the entry description */
        if (byObjCode == OBJCODE_ARR)
        {
            pEntry = &pObjEntry->pEntryDesc[1];
        }
        else
        {
            pEntry = &pObjEntry->pEntryDesc[i];
        }

        switch (pEntry->wDataType)
        {
        case DEFTYPE_INTEGER16:
        case DEFTYPE_UNSIGNED16:
            {
            /* the 16-bit variables in the structure are word-aligned,
                align the actual bitOffset to a word */
                /*wBitOffset = (EC_T_WORD)((wBitOffset+15) & 0xFFF0);*/

                if (i < bySubindex)
                {
                    dwBitOffset += 16;
                }
            } break;
        case DEFTYPE_UNSIGNED32:
        case DEFTYPE_INTEGER32:
        case DEFTYPE_REAL32:
            {
            /* the 32-bit variables in the structure are dword-aligned,
                align the actual bitOffset to a dword */
                /*wBitOffset = ((EC_T_WORD)((wBitOffset+31) & 0xFFE0));*/

                if (i < bySubindex)
                {
                    dwBitOffset += 32;
                }
            } break;
        case        DEFTYPE_UNSIGNED8:
        case        DEFTYPE_INTEGER8:
            {
                if( i < bySubindex )
                {
                    dwBitOffset += 8;
                }
            } break;
        default:
            {
                /* align the actual bitOffset to a byte */
                if (i < bySubindex)
                {
                    dwBitOffset = (EC_T_DWORD)(dwBitOffset + pEntry->wBitLength);
                }
            } break;
        }
    }

Exit:
    return dwBitOffset;
}

/***************************************************************************************************/
/**
\brief  This function reads the requested object.

\return dwRetVal of the read operation (EC_E_NOERROR on success, error code otherwise).
*/
EC_T_DWORD CEcSdoServ::Read(
    EC_T_WORD           wIndex,         /**< [in]   index of the requested object */
    EC_T_BYTE           bySubindex,     /**< [in]   subindex of the requested object */
    EC_T_DWORD          dwObjSize,      /**< [in]   Size of the object, returned by the function GetObjectLength
                                          *         which was called before
                                          */
    EC_T_OBJECTDICT*    pObjEntry,      /**< [in]   handle to the dictionary object returned by GetObjectHandle
                                          *         which was called before
                                          */
    EC_T_WORD*          pwData,         /**< [in]   Pointer to the buffer where the read data shall be copied to */
    EC_T_BOOL           bCompleteAccess /**< [in]   Indicates if a complete read of all subindices of the object
                                          *         shall be done or not
                                          */
                           )
{
    EC_T_DWORD              dwRetVal        = EC_E_ERROR;
    EC_T_WORD               i               = bySubindex;
    EC_T_BYTE               byObjCode       = 0;
    EC_T_BYTE               byMaxSubindex   = 0;
    EC_T_SDOINFOENTRYDESC*  pEntry          = EC_NULL;
    EC_T_BYTE               byLastSubindex  = bySubindex;
    EC_T_BYTE*              pbyData         = (EC_T_BYTE*)pwData;
    EC_PF_ACCESSFUNC        pfReader        = EC_NULL;
    EC_T_DWORD              dwUsedBytes     = 0;

    if (EC_NULL == pObjEntry)
    {
        pObjEntry = GetObjectHandle(wIndex);
    }
    if (EC_NULL == pObjEntry)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    pfReader = pObjEntry->pfRead;

    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    byObjCode = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT);
    byMaxSubindex = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT);

    /* lastSubindex is used for complete access to make loop over the requested entries
       to be read, we initialize this variable with the requested subindex that only
       one loop will be done for a single access */
    byLastSubindex = bySubindex;

    if ((byObjCode != OBJCODE_VAR) && (wIndex >= COEOBJID_DEVICE_TYPE))
    {
        /* if the object is an array or record we have to get the maxSubindex from the
           actual value of subindex 0, which is stored as EC_T_WORD at the beginning of the
           object's variable */
        if (EC_NULL != pfReader)
        {
            EC_T_WORD wSubIndex0 = 0;
            pfReader(this, wIndex, 0, 16, &wSubIndex0, EC_FALSE);
            byMaxSubindex = (EC_T_BYTE)wSubIndex0;
        }
        else
        {
            OsDbgAssert(EC_NULL != pObjEntry->pVarPtr);
            byMaxSubindex = EC_LOBYTE(EC_GETWORD((EC_T_WORD*)pObjEntry->pVarPtr)); /* ARM safe */
        }
    }

    if (bCompleteAccess)
    {
        if ( byObjCode == OBJCODE_VAR || wIndex < COEOBJID_DEVICE_TYPE )
        {
            /* complete access is not supported with simple objects or ENUM descriptions */
            dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
            goto Exit;
        }

        /* we read until the maximum subindex */
        byLastSubindex = (EC_T_BYTE)(byMaxSubindex);
    }
    else if (bySubindex > byMaxSubindex)
    {
        dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
        goto Exit;
    }

    if (EC_NULL != pfReader)
    {
        /* Read function is defined, we call the object specific read function */
        dwRetVal = pfReader(
            this,
            wIndex,
            bySubindex,
            dwObjSize,
            pwData,
            bCompleteAccess
                           );
        goto Exit;

    }
    /* in this example all entries are readable at any state, so we don't check the access rights here */
    else if ((wIndex < COEOBJID_DEVICE_TYPE) && (0 != bySubindex))
    {
        /* an ENUM description is read */
        EC_T_WORD   wSize   = 0;
        EC_T_WORD*  pVarPtr = (EC_T_WORD*)pObjEntry->pVarPtr;
        EC_T_CHAR** p;

        /* we get the corresponding entry description */
        pEntry = GetEntryDesc(pObjEntry, bySubindex);

        if( EC_NULL == pEntry )
        {
            dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
            goto Exit;
        }

        wSize    = (EC_T_WORD)BIT2BYTE(pEntry->wBitLength);

        p = (EC_T_CHAR **) pVarPtr;

        OsMemcpy(pbyData, p[bySubindex-1], wSize);
    }
    else /*( wIndex < COEOBJID_DEVICE_TYPE && bySubindex != 0 )*/
    {
        /* a variable object is read */
        for (i = bySubindex; i <= byLastSubindex; i++)
        {
            /* if only a single entry is requested, this loop will only be done once */
            EC_T_BYTE*  pbyVarPtr   = (EC_T_BYTE*)pObjEntry->pVarPtr;
            EC_T_DWORD  dwBitOffset  = 0;

/*
            if( 0x2fff < wIndex && 46 == bySubindex )
            {
                EC_DBGMSG("0x300x 0x%x PortDesc = 0x%x / 0x%x BP:0x%lx\n",
                    wIndex, EC_LOBYTE(pwData[0]), ((EC_T_BYTE*)pObjEntry->pVarPtr)[183],
                    (EC_T_DWORD)(pObjEntry->pVarPtr)
                    );
            }
*/

            if (i == 0)
            {
                /* subindex 0 is requested, the entry's data is at the beginning of the object's variable */
            }
            else if ( wIndex >= COEOBJID_DEVICE_TYPE )
            {
                /* subindex 1-n of an variable object is requested, we get the offset of the variable here */
                dwBitOffset = GetEntryOffset((EC_T_BYTE)i, pObjEntry);
            }

            /* we increment the variable pointer to the corresponding word address */
            pbyVarPtr += BIT2BYTE(dwBitOffset);

            /* get the corresponding entry description */
            pEntry = GetEntryDesc(pObjEntry, (EC_T_BYTE)i);

            if( EC_NULL == pEntry )
            {
                dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
                goto Exit;
            }

            if (
                (i == bySubindex)                       /* requested entry */
             || (bCompleteAccess && i >= bySubindex)     /* complete access and entry should be read */
               )
            {
/*
                EC_T_WORD wBitMask = 0;
*/

                /* we have to copy the entry */
                if ( i == 0 && byObjCode != OBJCODE_VAR )
                {
                    /* we read subindex 0 of an array or record */
                    if( 1 == dwObjSize )
                    {
                        pbyData[0] = byMaxSubindex;
                    }
                    else
                    {
                        EC_SET_FRM_WORD(pbyData, (EC_T_WORD)byMaxSubindex);
                        if( sizeof(EC_T_WORD) < (dwObjSize-dwUsedBytes) )
                        {
                        /* we increment the destination pointer by 2 because the subindex 0 will be
                            transmitted as EC_T_WORD for a complete access */
                            pbyData+=sizeof(EC_T_WORD);
                            dwUsedBytes += sizeof(EC_T_WORD);
                        }
                    }
                }
                else /*( i == 0 && byObjCode != OBJCODE_VAR )*/
                {
                    EC_T_WORD wDataType = pEntry->wDataType;

                    if (pEntry->wDataType >= 0x700)
                    {
                        /* the ENUM data types are defined from index 0x700 in this example
                           convert in standard data type for the read access */
                        if ( pEntry->wBitLength <= 8 )
                        {
                            wDataType = (EC_T_WORD)(DEFTYPE_BIT1 - 1 + pEntry->wBitLength);
                        }
                        else if ( pEntry->wBitLength == 16 )
                        {
                            wDataType = DEFTYPE_UNSIGNED16;
                        }
                        else if ( pEntry->wBitLength == 32 )
                        {
                            wDataType = DEFTYPE_UNSIGNED32;
                        }
                    }

                    switch (wDataType)
                    {
                    case  DEFTYPE_BOOLEAN:
                    case  DEFTYPE_BIT1:
                    case  DEFTYPE_BIT2:
                    case  DEFTYPE_BIT3:
                    case  DEFTYPE_BIT4:
                    case  DEFTYPE_BIT5:
                    case  DEFTYPE_BIT6:
                    case  DEFTYPE_BIT7:
                    case  DEFTYPE_BIT8:
                                /* in this example the objects are defined in that way,
                                   that the bit types are always inside a 16-bit field,
                                   starting at an even byte offset
                                 */
                    case DEFTYPE_INTEGER8:
                    case DEFTYPE_UNSIGNED8:
                        {
                            pbyData[0] = pbyVarPtr[0];
                            if( sizeof(EC_T_BYTE) < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += sizeof(EC_T_BYTE);
                                dwUsedBytes += sizeof(EC_T_BYTE);
                            }
                        } break;
                    case DEFTYPE_INTEGER16:
                    case DEFTYPE_UNSIGNED16:
                        {
                            /* in this example the objects are defined in that way,
                               that the 16 bit type are always starting at an even byte offset */
                            EC_SET_FRM_WORD(pbyData, EC_GETWORD(pbyVarPtr));
                            if( sizeof(EC_T_WORD) < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData+=sizeof(EC_T_WORD);
                                dwUsedBytes += sizeof(EC_T_WORD);
                            }
                        } break;
                    case      DEFTYPE_UNSIGNED24:
                        {
#if defined EC_BIG_ENDIAN
                            pbyData[0] = pbyVarPtr[2];
                            pbyData[1] = pbyVarPtr[1];
                            pbyData[2] = pbyVarPtr[0];
#else
                            OsMemcpy(pbyData, pbyVarPtr, 3);
#endif
                            if( 3 < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += 3;
                                dwUsedBytes += 3;
                            }
                        } break;
                    case DEFTYPE_UNSIGNED32:
                    case DEFTYPE_INTEGER32:
                    case DEFTYPE_REAL32:
                        {
                            /* in this example the objects are defined in that way,
                               that the 32 bit type are always starting at an even byte offset */
                            EC_SET_FRM_DWORD(pbyData, EC_GETDWORD(pbyVarPtr));
                            if( sizeof(EC_T_DWORD) < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += sizeof(EC_T_DWORD);
                                dwUsedBytes += sizeof(EC_T_DWORD);
                            }
                        } break;
                    case      DEFTYPE_UNSIGNED48:
                        {
#if defined EC_BIG_ENDIAN
                            pbyData[0] = pbyVarPtr[5];
                            pbyData[1] = pbyVarPtr[4];
                            pbyData[2] = pbyVarPtr[3];
                            pbyData[3] = pbyVarPtr[2];
                            pbyData[4] = pbyVarPtr[1];
                            pbyData[5] = pbyVarPtr[0];
#else
                            OsMemcpy(pbyData, pbyVarPtr, 6);
#endif
                            if( 6 < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += 6;
                                dwUsedBytes += 6;
                            }
                        } break;
                    case      DEFTYPE_UNSIGNED56:
                        {
#if defined EC_BIG_ENDIAN
                            pbyData[0] = pbyVarPtr[6];
                            pbyData[1] = pbyVarPtr[5];
                            pbyData[2] = pbyVarPtr[4];
                            pbyData[3] = pbyVarPtr[3];
                            pbyData[4] = pbyVarPtr[2];
                            pbyData[5] = pbyVarPtr[1];
                            pbyData[6] = pbyVarPtr[0];
#else
                            OsMemcpy(pbyData, pbyVarPtr, 7);
#endif
                            if( 7 < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += 7;
                                dwUsedBytes += 7;
                            }
                        } break;
                    case  DEFTYPE_REAL64:
                    case DEFTYPE_UNSIGNED64:
                        {
                        /* in this example the objects are defined in that way,
                           that the 64 bit type are always starting at an even byte offset */
                            EC_SET_FRM_QWORD(pbyData, EC_GETQWORD(pbyVarPtr));
                            if( sizeof(EC_T_UINT64) < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += sizeof(EC_T_UINT64);
                                dwUsedBytes += sizeof(EC_T_UINT64);
                            }
                        } break;
                    case DEFTYPE_OCTETSTRING:
                    case DEFTYPE_VISIBLESTRING:
                        {
                            /* in this example the objects are defined in that way,
                            that these types are always starting at an even byte offset */
                            OsMemcpy(pbyData, pbyVarPtr, BIT2BYTE(pEntry->wBitLength));
                            if( ((EC_T_DWORD)BIT2BYTE(pEntry->wBitLength)) < (dwObjSize-dwUsedBytes) )
                            {
                                pbyData += BIT2BYTE(pEntry->wBitLength);
                                dwUsedBytes += BIT2BYTE(pEntry->wBitLength);
                            }
                        } break;
                    case DEFTYPE_NULL: /* Empty type, fill */
                        {
                            /* continue with next loop */
                        } break;
                    default:
                        {
                            dwRetVal = EC_E_SDO_ABORTCODE_GENERAL;
                            goto Exit;
                        }
                    } /*switch (wDataType)*/
                } /*else ( i == 0 && byObjCode != OBJCODE_VAR )*/
            } /*((i == bySubindex)||(bCompleteAccess && i >= bySubindex))*/
        } /*for (i = bySubindex; i <= byLastSubindex; i++)*/
    } /*else ( wIndex < COEOBJID_DEVICE_TYPE && bySubindex != 0 )*/

/*
    if( 0x2fff < wIndex && 46 == bySubindex )
    {
        EC_DBGMSG("0x300x 0x%x PortDesc = 0x%x / 0x%x BP:0x%lx\n",
            wIndex, EC_LOBYTE(pwData[0]), ((EC_T_BYTE*)pObjEntry->pVarPtr)[183],
            (EC_T_DWORD)(pObjEntry->pVarPtr)
                );
    }
*/

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  This function writes the requested object.

\return dwRetVal of the write operation (EC_E_NOERROR on success, error code otherwise).
*/
EC_T_DWORD CEcSdoServ::Write(
    EC_T_WORD           wIndex,             /**< [in]   index of the requested object */
    EC_T_BYTE           bySubindex,         /**< [in]   subindex of the requested object */
    EC_T_DWORD          dwDataSize,         /**< [in]   received data size of the SDO Download */
    EC_T_OBJECTDICT*    pObjEntry,          /**< [in]   handle to the dictionary object returned by
                                             *          GetObjectHandle which was called before */
    EC_T_WORD*          pwData,             /**< [in]   Pointer to the buffer where the written data can be copied from */
    EC_T_BOOL           bCompleteAccess,    /**< [in]   Indicates if a complete write of all subindices of the
                                             *          object shall be done or not */
    EC_T_BOOL           bForce              /**< [in]   Indicates if access to e.g. identity object */
                            )
{
    EC_T_DWORD              dwRetVal        = EC_E_ERROR;
    EC_T_BYTE               i               = bySubindex;
    EC_T_BYTE               byObjCode       = 0;
    EC_T_BYTE               byMaxSubindex   = 0;
    EC_T_SDOINFOENTRYDESC*  pEntry          = EC_NULL;
    EC_T_BYTE               byLastSubindex  = 0;
    EC_T_DWORD              dwCopyLen       = 0;
    EC_T_BOOL               bWritten        = 0;
    EC_T_BYTE*              pbyData         = (EC_T_BYTE*)pwData;
    EC_PF_ACCESSFUNC        pfWriter        = EC_NULL;
    EC_T_DWORD              dwUsedBytes     = 0;

    if ((EC_NULL == pbyData) || (0 == dwDataSize))
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    if ((EC_NULL == pObjEntry) && bForce)
    {
        pObjEntry = GetObjectHandle(wIndex);
    }
    if (EC_NULL == pObjEntry)
    {
        dwRetVal = EC_E_INVALIDINDEX;
        goto Exit;
    }
    /* get the information of ObjCode and MaxSubindex in local variables to support different types of microcontroller */
    byObjCode       = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT);
    byMaxSubindex   = (EC_T_BYTE)((pObjEntry->oObjDesc.wObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT);

    byLastSubindex = bySubindex;

    if ((byObjCode != OBJCODE_VAR) && (EC_NULL != pObjEntry->pVarPtr))
    {
        /* if the object is an array or record we have to get the maxSubindex from the
           actual value of subindex 0, which is stored as EC_T_WORD at the beginning of the
            object's variable */
        byMaxSubindex = EC_LOBYTE(((EC_T_WORD*) (pObjEntry->pVarPtr))[0]);
    }

    if (bCompleteAccess)
    {
        if (byObjCode == OBJCODE_VAR)
        {
            /* complete access is not supported with simple objects */
            dwRetVal = EC_E_SDO_ABORTCODE_ACCESS;
            goto Exit;
        }

        if ( bySubindex == 0 )
        {
            /* we change the subindex 0 */
            byMaxSubindex = EC_LOBYTE(pwData[0]);
        }
        /* we write until the maximum subindex */
        byLastSubindex = (EC_T_BYTE)(byMaxSubindex);
    }
    else /*( bCompleteAccess )*/
    {
        if ( bySubindex > byMaxSubindex )
        {
            /* the maximum subindex is reached */
            dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
            goto Exit;
        }
        else /*( bySubindex > byMaxSubindex )*/
        {
            /* we check the write access for single accesses here, a complete write access
               is allowed if at least one entry is writable (in this case the values for the
               read only entries shall be ignored) */
            /* we get the corresponding entry description */
            pEntry = GetEntryDesc(pObjEntry, bySubindex);

            if (EC_NULL == pEntry)
            {
                dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
                goto Exit;
            }

            /* check if we have write access (bits 3-5 (PREOP, SAFEOP, OP) of ObjAccess)
               by comparing with the actual state (bits 1-3 (PREOP, SAFEOP, OP) of AL Status) */
            if(
                !(
                (EC_MASTER_STATE_INIT == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_PREOP))
              ||(EC_MASTER_STATE_PREOP == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_PREOP))
              ||(EC_MASTER_STATE_SAFEOP == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_SAFEOP))
              ||(EC_MASTER_STATE_OP == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_OP))
              ||(bForce && (pEntry->wObjAccess&OBJACCESS_FORCE))
                 )
              )
            {
                OsDbgAssert(EC_FALSE);
                dwRetVal = EC_E_SDO_ABORTCODE_READONLY;
                goto Exit;
            }
        } /*else ( bySubindex > byMaxSubindex )*/
    } /*else ( bCompleteAccess )*/

    pfWriter = pObjEntry->pfWrite;
    if ( pfWriter != EC_NULL )
    {
        /* Write function is defined, we call the object specific write function */
        dwRetVal = pfWriter(this, wIndex, bySubindex, dwDataSize, pwData, bCompleteAccess);
        goto Exit;
    }

    /* we use the standard write function */
    for (i = bySubindex; i <= byLastSubindex; i++)
    {
        /* if only a single entry is requested, this loop will only be done once */
        EC_T_BYTE*  pbyVarPtr     = (EC_T_BYTE*)pObjEntry->pVarPtr;
        EC_T_DWORD   dwBitOffset  = 0;

        /* we get the corresponding entry description */
        pEntry = GetEntryDesc(pObjEntry, bySubindex);

        if( EC_NULL == pEntry )
        {
            dwRetVal = EC_E_SDO_ABORTCODE_OFFSET;
            goto Exit;
        }

        if(
            !(
                (EC_MASTER_STATE_INIT == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_PREOP))
              ||(EC_MASTER_STATE_PREOP == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_PREOP))
              ||(EC_MASTER_STATE_SAFEOP == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_SAFEOP))
              ||(EC_MASTER_STATE_OP == m_pMaster->GetCurMasterState() && (pEntry->wObjAccess&ACCESS_WRITE_OP))
              ||(bForce && (pEntry->wObjAccess&OBJACCESS_FORCE))
             )
          )
        {
            dwRetVal = EC_E_SDO_ABORTCODE_READONLY;
            goto Exit;
        }
        /* we have write access for this entry */
        if (i != 0)
        {
            /* subindex 1-n of an variable object is requested, we get the offset of the variable here */
            dwBitOffset = GetEntryOffset(i, pObjEntry);

            /* we increment the variable pointer to the corresponding word address */
            pbyVarPtr += BIT2BYTE(dwBitOffset);
        }

        if (
            (i == bySubindex)           /* requested entry */
         || (bCompleteAccess && i >= bySubindex) )     /* complete access and entry should be read */
        {
            /* we have to copy the entry */
            if (i == 0 && byObjCode != OBJCODE_VAR)
            {
                /* subindex 0 of an array or record shall be written */
                EC_SET_FRM_WORD(pbyVarPtr, EC_GETWORD(pbyData));

                if( sizeof(EC_T_WORD) < (dwDataSize-dwUsedBytes) )
                {
                /* we increment the destination pointer by 2 because the subindex 0 will be
                    transmitted as EC_T_WORD for a complete access */
                    pbyData +=sizeof(EC_T_WORD);
                    dwUsedBytes += sizeof(EC_T_WORD);
                }
            }
            else /*(i == 0 && byObjCode != OBJCODE_VAR)*/
            {
                EC_T_WORD wDataType = pEntry->wDataType;
                if (pEntry->wDataType >= 0x700)
                {
                    /* the ENUM data types are defined from index 0x700 in this example
                       convert in standard data type for the write access */
                    if ( pEntry->wBitLength <= 8 )
                    {
                        wDataType = (EC_T_WORD)(DEFTYPE_BIT1 - 1 + pEntry->wBitLength);
                    }
                    else if ( pEntry->wBitLength == 16 )
                    {
                        wDataType = DEFTYPE_UNSIGNED16;
                    }
                    else if ( pEntry->wBitLength == 32 )
                    {
                        wDataType = DEFTYPE_UNSIGNED32;
                    }
                }

                switch (wDataType)
                {
                case    DEFTYPE_BOOLEAN:
                case    DEFTYPE_BIT1:
                case    DEFTYPE_BIT2:
                case    DEFTYPE_BIT3:
                case    DEFTYPE_BIT4:
                case    DEFTYPE_BIT5:
                case    DEFTYPE_BIT6:
                case    DEFTYPE_BIT7:
                case    DEFTYPE_BIT8:
                    /* in this example the objects are defined in that way,
                       that the bit types are always inside a 16-bit field,
                       starting at an even byte offset */
                case    DEFTYPE_INTEGER8:
                case    DEFTYPE_UNSIGNED8:
                    {
                        pbyVarPtr[0] = pbyData[0];

                        if( sizeof(EC_T_BYTE) < (dwDataSize-dwUsedBytes) )
                        {
                            pbyData += sizeof(EC_T_BYTE);
                            dwUsedBytes += sizeof(EC_T_BYTE);
                        }
                    } break;
                case    DEFTYPE_INTEGER16:
                case    DEFTYPE_UNSIGNED16:
                    {
                        /* in this example the objects are defined in that way,
                           that the 16 bit type are always starting at an even byte offset */
                        EC_SET_FRM_WORD(pbyVarPtr, EC_GETWORD(pbyData));
                        if( sizeof(EC_T_WORD) < (dwDataSize-dwUsedBytes) )
                        {
                            pbyData += sizeof(EC_T_WORD);
                            dwUsedBytes += sizeof(EC_T_WORD);
                        }
                    } break;
                case    DEFTYPE_UNSIGNED32:
                case    DEFTYPE_INTEGER32:
                case    DEFTYPE_REAL32:
                    {
                        /* in this example the objects are defined in that way,
                           that the 32 bit type are always starting at an even byte offset */
                        EC_SET_FRM_DWORD(pbyVarPtr, EC_GETDWORD(pbyData));
                        if( sizeof(EC_T_DWORD) < (dwDataSize-dwUsedBytes) )
                        {
                            pbyData += sizeof(EC_T_DWORD);
                            dwUsedBytes += sizeof(EC_T_DWORD);
                        }
                    } break;
                case    DEFTYPE_REAL64:
                case    DEFTYPE_UNSIGNED64:
                    {
                        /* in this example the objects are defined in that way,
                           that the 64 bit type are always starting at an even byte offset */
                        EC_SET_FRM_QWORD(pbyVarPtr, EC_GETQWORD(pbyData));
                        if( sizeof(EC_T_UINT64) < (dwDataSize-dwUsedBytes) )
                        {
                            pbyData += sizeof(EC_T_UINT64);
                            dwUsedBytes += sizeof(EC_T_UINT64);
                        }
                    } break;
                case    DEFTYPE_VISIBLESTRING:
                case    DEFTYPE_OCTETSTRING:
                    {
                        /* in this example the objects are defined in that way,
                           that the other types are always starting at an even byte offset */
                        dwCopyLen = EC_MIN(((EC_T_DWORD)BIT2BYTE(pEntry->wBitLength)), dwDataSize);
                        OsMemcpy(pbyVarPtr, pbyData, dwCopyLen);
                        if( ((EC_T_DWORD)BIT2BYTE(pEntry->wBitLength)) < (dwDataSize-dwUsedBytes) )
                        {
                            pbyData += BIT2BYTE(pEntry->wBitLength);
                            dwUsedBytes += BIT2BYTE(pEntry->wBitLength);
                        }
                    } break;
                default:
                    {
                        /* other data types are not supported from this example */
                        dwRetVal = EC_E_SDO_ABORTCODE_GENERAL;
                        goto Exit;
                    }
                } /*switch (wDataType)*/
            } /*else (i == 0 && byObjCode != OBJCODE_VAR)*/

            /* set flag */
            bWritten = 1;
        } /*(i == bySubindex) || (bCompleteAccess && i >= bySubindex)*/
    } /*for (i = bySubindex; i <= byLastSubindex; i++)*/

    if (bWritten == 0)
    {
        /* we didn't write anything, so we have to return the stored error code */
        goto Exit;
    }

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  SDO Download.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::SDODownload(
    PEcMailboxCmd   pCmd
                                  )
{
    EC_T_DWORD          dwRetVal        = EC_E_ERROR;
    EC_T_OBJECTDICT*    pObjEntry       = EC_NULL;
    EC_T_DWORD          dwDataSize      = 0;
    EC_T_DWORD          dwObjLength     = 0;
    EC_T_BOOL           bCompleteAccess = EC_FALSE;

    /* get the object handle of the requested index */
    pObjEntry = GetObjectHandle( pCmd->sIndex.index );

    if (EC_NULL == pObjEntry)
    {
        dwRetVal = EC_E_SDO_ABORTCODE_INDEX;
        goto Exit;
    }

    /* transferType contains the information if the SDO Download Request or the SDO Upload Response
       can be an expedited service (SDO data length <= 4, that means the data is stored in the
       SDO-Header completely */

    /* pData is the pointer to the received (SDO-Download) or sent (SDO-Upload) SDO data in the mailbox */
    bCompleteAccess = ((pCmd->dwMailboxFlags & EC_MAILBOX_FLAG_SDO_COMPLETE) == EC_MAILBOX_FLAG_SDO_COMPLETE);

    dwDataSize = dwObjLength = GetObjectLength(pCmd->sIndex.index, pCmd->sIndex.subIndex, pObjEntry, bCompleteAccess);

    /* SDO Download */
    if (dwDataSize != pCmd->length)
    {
        dwRetVal = EC_E_SDO_ABORTCODE_DATA_SIZE;
        goto Exit;
    }

    dwRetVal = Write(pCmd->sIndex.index, pCmd->sIndex.subIndex, dwObjLength, pObjEntry, (EC_T_WORD*)&(pCmd->pbyData[0]), bCompleteAccess, EC_FALSE);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Update master state.
*/
EC_T_VOID CEcSdoServ::EntrySetMasterState_State(
    EC_T_DWORD  dwMasterState    /**< [in]   Current master state */
                                               )
{
/* OBJ2001_STATE_SUM_MASTER_STATE 0x000000F0 Bit 4-7: Master State */
    m_dwMasterStateSummary &= (EC_T_DWORD)(~(0xf<<4));

    switch( dwMasterState )
    {
    case DEVICE_STATE_INIT:      m_dwMasterStateSummary |= ((EC_T_DWORD)(0x1<<4));   break;
    case DEVICE_STATE_PREOP:     m_dwMasterStateSummary |= ((EC_T_DWORD)(0x2<<4));   break;
    case DEVICE_STATE_SAFEOP:    m_dwMasterStateSummary |= ((EC_T_DWORD)(0x4<<4));   break;
    case DEVICE_STATE_OP:        m_dwMasterStateSummary |= ((EC_T_DWORD)(0x8<<4));   break;
    default:                                                                         break;
    }
}

/***************************************************************************************************/
/**
\brief  Master in Requested state ?!.
*/
EC_T_VOID CEcSdoServ::EntrySet_MastInReqState(
    EC_T_BOOL   bInReqState     /**< [in]   State */
                                             )
{
    if(bInReqState)
    {
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_MASTER_REQ;
    }
    else
    {
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_MASTER_REQ;
    }

}

/***************************************************************************************************/
/**
\brief  Bus Topology Match
*/
EC_T_VOID CEcSdoServ::EntrySet_BusScanMatch(
    EC_T_BOOL   bMatch  /**< [in]   Match */
                                           )
{
    if(bMatch)
    {
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_BUS_MATCH;
    }
    else
    {
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_BUS_MATCH;
    }
}

/***************************************************************************************************/
/**
\brief  Is DC enabled.
*/
EC_T_VOID CEcSdoServ::EntrySet_SetDCEnabled(
    EC_T_BOOL   bEnabled    /**< [in]   DC Enabled */
                                           )
{
    if(bEnabled)
    {
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_DC_ENA;
    }
    else
    {
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_DC_ENA;
    }

}

/***************************************************************************************************/
/**
\brief  DC In Sync
*/
EC_T_VOID CEcSdoServ::EntrySet_SetDCInSync(
    EC_T_BOOL   bDCInSync   /**< [in]   DC In Sync */
                                          )
{
#ifdef PRINT_DC_DBG_MESSAGES
    if(m_bDCInSync != bDCInSync)
    {
        m_pMaster->EthDevice()->LinkDbgMsg(3, 0xF1, 0xF1, "EntrySet_SetDCInSync: m_bDCInSync is now: %d\n", bDCInSync);
    }
#endif

    m_bDCInSync = bDCInSync;
    if(m_bDCInSync && m_bDCMInSync)
    {
#ifdef PRINT_DC_DBG_MESSAGES
        if(!(m_dwMasterStateSummary & OBJ2001_STATE_SUM_DC_SYNC))
        {
            m_pMaster->EthDevice()->LinkDbgMsg(3, 0xF1, 0xF1, "EntrySet_SetDCInSync: Set DC-Sync (Bit 13) in object 0x2001\n");
        }
#endif
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_DC_SYNC;
    }
    else
    {
#ifdef PRINT_DC_DBG_MESSAGES
        if((m_dwMasterStateSummary & OBJ2001_STATE_SUM_DC_SYNC))
        {
            m_pMaster->EthDevice()->LinkDbgMsg(3, 0xF1, 0xF1, "EntrySet_SetDCInSync: Clear DC-Sync (Bit 13) in object 0x2001\n");
        }
#endif
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_DC_SYNC;
    }
}


/***************************************************************************************************/
/**
\brief  DCM In Sync
*/
EC_T_VOID CEcSdoServ::EntrySet_SetDCMInSync(
    EC_T_BOOL   bDCMInSync   /**< [in]   DCM In Sync */
    )
{

#ifdef PRINT_DC_DBG_MESSAGES
    if(m_bDCMInSync != bDCMInSync)
    {
        m_pMaster->EthDevice()->LinkDbgMsg(3, 0xF1, 0xF1, "EntrySet_SetDCMInSync: m_bDCMInSync is now: %d\n", bDCMInSync);
    }
#endif

    m_bDCMInSync = bDCMInSync;

    if(m_bDCInSync && m_bDCMInSync)
    {
#ifdef PRINT_DC_DBG_MESSAGES
        if(!(m_dwMasterStateSummary & OBJ2001_STATE_SUM_DC_SYNC))
        {
            m_pMaster->EthDevice()->LinkDbgMsg(3, 0xF1, 0xF1, "EntrySet_SetDCMInSync: Set DC-Sync (Bit 13) in object 0x2001\n");
        }
#endif
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_DC_SYNC;
    }
    else
    {
#ifdef PRINT_DC_DBG_MESSAGES
        if((m_dwMasterStateSummary & OBJ2001_STATE_SUM_DC_SYNC))
        {
            m_pMaster->EthDevice()->LinkDbgMsg(3, 0xF1, 0xF1, "EntrySet_SetDCMInSync: Clear DC-Sync (Bit 13) in object 0x2001\n");
        }
#endif
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_DC_SYNC;
    }
}


/***************************************************************************************************/
/**
\brief  DC is busy
*/
EC_T_VOID CEcSdoServ::EntrySet_SetDCBusy(
    EC_T_BOOL   bDCBusy     /**< [in]   DC Instance Busy */
                                        )
{
    if(bDCBusy)
    {
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_DC_BUSY;
    }
    else
    {
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_DC_BUSY;
    }

}

/***************************************************************************************************/
/**
\brief  Link Up
*/
EC_T_VOID CEcSdoServ::EntrySet_SetLinkUp(
    EC_T_BOOL   bLinkUp     /**< [in]   Link UP */
                                        )
{
    if(bLinkUp)
    {
        m_dwMasterStateSummary |= OBJ2001_STATE_SUM_LINK_UP;
    }
    else
    {
        m_dwMasterStateSummary &= ~OBJ2001_STATE_SUM_LINK_UP;
    }

}

/***************************************************************************************************/
/**
\brief  Set LinkLayer parameters in ODL.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD CEcSdoServ::SetLinkLayerParms(
    EC_T_LINK_PARMS*    pLinkParms      /**< [in]   Current LL Parameterset */
                                        )
{
    EC_T_DWORD  dwRetVal    = EC_E_ERROR;

    if( EC_NULL == pLinkParms )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    OsStrcpy(m_oMasterInitParmObject.szDriverIdent, pLinkParms->szDriverIdent);
    m_oMasterInitParmObject.bPollingModeActive = (EcLinkMode_POLLING == pLinkParms->eLinkMode);

    dwRetVal = EC_E_NOERROR;
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Set Link Layer Polling in ODL.
*/
EC_T_VOID CEcSdoServ::SetLinkLayerPolling(
    EC_T_BOOL   bActive
                                         )
{
    m_oMasterInitParmObject.bPollingModeActive = bActive;
}

/***************************************************************************************************/
/**
\brief  Set Link Layer Alloc support in ODL.
*/
EC_T_VOID CEcSdoServ::SetLinkLayerAllocSupport(
    EC_T_BOOL   bActive
                                              )
{
    m_oMasterInitParmObject.bAllocSendFrameActive = bActive;
}

/***************************************************************************************************/
/**
\brief  Set DC Slave Deviation limit in ODL
*/
EC_T_VOID CEcSdoServ::SetDCSlaveDevLimit(
    EC_T_DWORD  dwLimitNsec     /**< [in]   Limit in nsecs */
                                        )
{
    m_dwDCDeviationLimit = dwLimitNsec;
}

/***************************************************************************************************/
/**
\brief  Store Hardware address
*/
EC_T_VOID CEcSdoServ::SetMacAddress(
    EC_T_BOOL bRedIf,   /**< [in]   Is Redundant IF Addr if True */
    EC_T_BYTE byMac[6]  /**< [in]   MAC Address */
                                   )
{
    if (bRedIf)
    {
        OsMemcpy(m_oMacAddressObject.abyRedHardware, byMac, 6);
    }
    else
    {
        OsMemcpy(m_oMacAddressObject.abyHardware, byMac, 6);
    }
}

/***************************************************************************************************/
/**
\brief  Store Hardware address
*/
EC_T_VOID CEcSdoServ::SetCfgMacAddress(
    EC_T_BOOL bSrc,     /**< [in]   Is Source address if True */
    EC_T_BYTE byMac[6]  /**< [in]   MAC Address */
                                   )
{
    if (bSrc)
    {
        OsMemcpy(m_oMacAddressObject.abyCfgSource, byMac, 6);
    }
    else
    {
        OsMemcpy(m_oMacAddressObject.abyCfgDestination, byMac, 6);
    }
}

/***************************************************************************************************/
/**
\brief  Clean stored slave information.

Issued before storing SB Results.
*/
EC_T_VOID CEcSdoServ::CleanSlaves(EC_T_WORD wNumOfSlavesToClean)
{
    EC_T_DWORD dwScan   = 0;

    EntrySet_BusScanMatch(EC_FALSE);

    wNumOfSlavesToClean = EC_MIN(wNumOfSlavesToClean, m_wMaxSlavesObjs);

    OsMemset(m_apSlaveObjects, 0, wNumOfSlavesToClean * sizeof(EC_T_OBJ3XXX));

    for( dwScan = 0; dwScan < wNumOfSlavesToClean; dwScan++ )
    {
        EC_SETWORD(&m_apSlaveObjects[dwScan].wSubIndex0, OBJ3XXX_MAX_SUBINDEX);
    }
}

/***************************************************************************************************/
/**
\brief  Add Slave from ESI Configuration.
*/
EC_T_VOID CEcSdoServ::AddConfiguredSlave(
    EC_T_DWORD  dwSlaveIdx          /**< [in]   Slave Array Index */
   ,CEcSlave*   pCfgSlave           /**< [in]   Slave configuration Instance */
                                        )
{
    EC_T_WORD           wIndex      = 0;
    EC_T_OBJECTDICT*    pEntry      = EC_NULL;
    EC_T_OBJ8XXX*       pSlaveInfo  = EC_NULL;
    CEcMbSlave*         pMbSlave    = EC_NULL;
    EC_T_DWORD          dwVal = 0, dwLen = 0;
    EC_T_DWORD          dwObjIdx    = 0;
    EC_T_DWORD          dwSubObjIdx = 0;
    EC_T_WORD*          pwFixedAddr = 0;

    if( dwSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD Range */
        goto Exit;
    }

    if ( pCfgSlave->HasMailBox() ) pMbSlave = (CEcMbSlave *) pCfgSlave;

    wIndex = (EC_T_WORD) (COEOBJID_SLAVECFGBASE + dwSlaveIdx);

    pEntry = GetObjectHandle(wIndex);
    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }


    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0x8000 - 0x8FFF Slave Objects (configured slaves)
     */
    pSlaveInfo = (EC_T_OBJ8XXX*) pEntry->pVarPtr;
    if( EC_NULL == pSlaveInfo )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo->wFixedStationAddr       = pCfgSlave->GetFixedAddr();

    OsStrcpy(pSlaveInfo->szType, "EtherCAT Slave");
    OsStrcpy(pSlaveInfo->szName, pCfgSlave->GetName());

    pSlaveInfo->dwDeviceType            = DEVICETYPE_ETHERCAT_SLAVE;
    pSlaveInfo->dwVendorID              = pCfgSlave->GetVendorId();
    pSlaveInfo->dwProductCode           = pCfgSlave->GetProductCode();
    pSlaveInfo->dwRevision              = pCfgSlave->GetRevisionNumber();
    pSlaveInfo->dwSerial                = pCfgSlave->GetSerialNumber();

    if (pSlaveInfo->dwVendorID == 2)
    {
        if (pSlaveInfo->dwProductCode == 0x07a82c52)
            OsStrcpy(pSlaveInfo->szType, "EK1960"); 
        else if (pSlaveInfo->dwProductCode == 0x1af43052)
            OsStrcpy(pSlaveInfo->szType, "EL6900");
        else if (pSlaveInfo->dwProductCode == 0x1afe3052)
            OsStrcpy(pSlaveInfo->szType, "EL6910");
        else if (pSlaveInfo->dwProductCode == 0x1b123052)
            OsStrcpy(pSlaveInfo->szType, "EL6930");
    }

    if (pMbSlave)
    {
       dwLen = sizeof(dwVal);
       pMbSlave->GetMbSlaveInfo(eie_mbx_outsize, (EC_T_BYTE *) &dwVal, &dwLen);
       pSlaveInfo->wMailboxOutSize = (EC_T_WORD) dwVal;

       dwLen = sizeof(dwVal);
       pMbSlave->GetMbSlaveInfo(eie_mbx_insize, (EC_T_BYTE *) &dwVal, &dwLen);
       pSlaveInfo->wMailboxInSize = (EC_T_WORD) dwVal;
    }

    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0xF02x  Configured address list
     */
    dwObjIdx = dwSlaveIdx / 256;
    dwSubObjIdx = dwSlaveIdx % 256;
    dwSubObjIdx++; /* First Slave is at subindex 1 */

    wIndex = (EC_T_WORD) (COEOBJID_CONFADDRLISTBASE + dwObjIdx);

    pEntry = GetObjectHandle(wIndex);
    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    /* Store fixed station address */
    pwFixedAddr = (EC_T_WORD *) pEntry->pVarPtr;
    EC_SETWORD(&pwFixedAddr[dwSubObjIdx], pCfgSlave->GetCfgFixedAddr()); /* ARM safe */

    /* increment number of configured slaves */
    {
    EC_T_WORD wNumOfSlaves = (EC_T_WORD)(m_apCfgAddressListObjects[dwObjIdx].wSubIndex0 + 1);

        EC_SETWORD(&m_apCfgAddressListObjects[dwObjIdx].wSubIndex0, wNumOfSlaves);
        pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(wNumOfSlaves | (OBJCODE_REC << 8));
    }
Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Add configuration of a slave which is found during bus scan.
*/
EC_T_VOID CEcSdoServ::AddConnectedSlave(
   CEcBusSlave*   pCfgBusSlave       /**< [in]   CEcBusSlave configuration Instance */
                                       )
{
    EC_T_WORD           wIndex          = 0;
    EC_T_OBJECTDICT*    pEntry          = EC_NULL;
    EC_T_OBJ9XXX*       pSlaveInfo9xxx  = EC_NULL;
    EC_T_DWORD          dwSlaveIdx      = pCfgBusSlave->GetBusIndex();
    EC_T_DWORD          dwObjIdx        = 0;
    EC_T_DWORD          dwSubObjIdx     = 0;
    EC_T_WORD*          pwFixedAddr     = 0;

    if( dwSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD Range */
        goto Exit;
    }

    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0x9000 - 0x9FFF Slave Objects (connected slaves)
     */

    wIndex = (EC_T_WORD) (COEOBJID_SLAVEINFBASE + dwSlaveIdx);

    pEntry = GetObjectHandle(wIndex);
    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo9xxx = (EC_T_OBJ9XXX *) pEntry->pVarPtr;
    if( EC_NULL == pSlaveInfo9xxx )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo9xxx->wFixedStationAddr       = pCfgBusSlave->GetFixedAddress();
    pSlaveInfo9xxx->dwVendorID              = pCfgBusSlave->GetVendorId();
    pSlaveInfo9xxx->dwProductCode           = pCfgBusSlave->GetProductCode();
    pSlaveInfo9xxx->dwRevision              = pCfgBusSlave->GetRevisionNo();
    pSlaveInfo9xxx->dwSerial                = pCfgBusSlave->GetSerialNo();
    pSlaveInfo9xxx->wDLStatus               = pCfgBusSlave->GetDlStatus();

    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0xF04x  Detected address list
     */

    dwObjIdx = dwSlaveIdx / 256;
    dwSubObjIdx = dwSlaveIdx % 256;
    dwSubObjIdx++; /* First Slave is at subindex 1 */

    wIndex = (EC_T_WORD) (COEOBJID_CONNADDRLISTBASE + dwObjIdx);

    pEntry = GetObjectHandle(wIndex);
    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    /* Store fixed station address */
    pwFixedAddr = (EC_T_WORD *) pEntry->pVarPtr;
    EC_SETWORD(&pwFixedAddr[dwSubObjIdx], pCfgBusSlave->GetFixedAddress()); /* ARM safe */

    /* set number of connected slaves */
    {
    EC_T_WORD wNumOfSlaves = (EC_T_WORD)EC_MAX(dwSubObjIdx, m_apConAddressListObjects[dwObjIdx].wSubIndex0);
    
        EC_SETWORD(&m_apConAddressListObjects[dwObjIdx].wSubIndex0, wNumOfSlaves);
        pEntry->oObjDesc.wObjFlags  = (EC_T_WORD)(wNumOfSlaves | (OBJCODE_REC << 8));
    }

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Add Slave from BusTopology / ENI.
Bus slaves first followed by slaves not currently present.

This is the old ATEM API (0x3000-0x3FFF). The new methods AddConfiguredSlave (0x8000-0x8FFF)
and AddConnectedSlave (0x9000-0x9FFF) were added to support the "EtherCAT Modular Device Profiles" standard.

*/
EC_T_VOID CEcSdoServ::AddSlave(
    EC_T_DWORD  dwSDOSlaveIdx       /**< [in]   Master OD 0x3000 Slave Array Index */
   ,EC_T_DWORD  dwVendorId          /**< [in]   Vendor Code */
   ,EC_T_DWORD  dwProdCode          /**< [in]   Product Code */
   ,EC_T_DWORD  dwRevisionNo        /**< [in]   Revision No */
   ,EC_T_DWORD  dwSerialNo          /**< [in]   Serial No*/
   ,EC_T_WORD   wMbxSupportedProtocols /**< [in]   Supported Mailbox Protokolls */
   ,EC_T_WORD   wAutoIncAddr        /**< [in]   Auto Increment Address*/
   ,EC_T_WORD   wFixedAddr          /**< [in]   Slave Programmed fixed Address */
   ,EC_T_WORD   wAliasAddr          /**< [in]   Slave Programmed Alias Address */
   ,EC_T_WORD   wPortState          /**< [in]   Slave Port state */
   ,EC_T_BOOL   bDCSupport          /**< [in]   DC Support */
   ,EC_T_BOOL   bDC64Support        /**< [in]   DC 64Bit Support */
   ,EC_T_DWORD  dwSbErrorCode       /**< [in]   Slave scanbus check status */
   ,EC_T_BYTE   byFmmusSupported    /**< [in]   FMMU's supported */
   ,EC_T_BYTE   bySyncManagersSupd  /**< [in]   Sync Managers supported */
   ,EC_T_BYTE   byRamSizeKb         /**< [in]   RAM Size in kB */
   ,EC_T_BYTE   byPortDescriptor    /**< [in]   Port Descriptor */
   ,EC_T_BYTE   byESCType           /**< [in]   ESC type */
   ,CEcSlave*   pCfgSlave           /**< [in]   Hooked Slave configuration Instance if existent */
                              )
{
    EC_T_WORD           wIndex      = 0;
    EC_T_OBJECTDICT*    pEntry      = EC_NULL;
    EC_T_OBJ3XXX*  pSlaveInfo  = EC_NULL;
    EC_T_DWORD dwTmp = 0;

    if( dwSDOSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD range */
        goto Exit;
    }

    m_apMasterSlave[dwSDOSlaveIdx]          = pCfgSlave;

    wIndex = (EC_T_WORD)(COEOBJID_SLAVECFGINFOBASE+dwSDOSlaveIdx);

    pEntry = GetObjectHandle(wIndex);

    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo = (EC_T_OBJ3XXX*)pEntry->pVarPtr;

    if( EC_NULL == pSlaveInfo )
    {
        /* entry not found */
        goto Exit;
    }

    EC_SETDWORD(&pSlaveInfo->bEntryValid,    EC_TRUE);
    EC_SETDWORD(&pSlaveInfo->dwVendorID,    dwVendorId);
    EC_SETDWORD(&pSlaveInfo->dwProductCode, dwProdCode);
    EC_SETDWORD(&pSlaveInfo->dwRevisionNo,  dwRevisionNo);
    EC_SETDWORD(&pSlaveInfo->dwSerialNo,    dwSerialNo);
    pSlaveInfo->wMbxSupportedProtocols  = wMbxSupportedProtocols;

    pSlaveInfo->wAutoIncAddr            = wAutoIncAddr;
    pSlaveInfo->wPhysAddr               = wFixedAddr;
    pSlaveInfo->wAliasAddr              = wAliasAddr;
    pSlaveInfo->wPortState              = wPortState;
    pSlaveInfo->bDCSupport              = bDCSupport;
    pSlaveInfo->bDC64Support            = bDC64Support;
    pSlaveInfo->dwSBErrorCode           = dwSbErrorCode;

    pSlaveInfo->byFmmusSupported        = byFmmusSupported;
    pSlaveInfo->bySyncManagersSupported = bySyncManagersSupd;
    pSlaveInfo->byRamSizeKb             = byRamSizeKb;
    pSlaveInfo->byPortDescriptor        = byPortDescriptor;
    pSlaveInfo->byESCType               = byESCType;
    if(dwSbErrorCode != EC_E_SLAVE_NOT_PRESENT)
    {
        EC_SETDWORD(&pSlaveInfo->bSlaveIsPresent, EC_TRUE);
    }

    if( EC_NULL == pCfgSlave )
    {
        /* no configuration yet */
        goto Exit;
    }

    OsStrcpy(pSlaveInfo->szDeviceName, ((CEcSlave*)pCfgSlave)->GetName());
    pSlaveInfo->wConfigPhysAddr         = ((CEcSlave*)pCfgSlave)->GetCfgFixedAddr();
    pSlaveInfo->bMailboxSupport         = ((CEcSlave*)pCfgSlave)->HasMailBox();
    pSlaveInfo->dwCurState              = ((CEcSlave*)pCfgSlave)->GetEcatState();
    pSlaveInfo->dwReqState              = ((CEcSlave*)pCfgSlave)->GetSmReqEcatState();
    pSlaveInfo->bErrFlagSet             = (((CEcSlave*)pCfgSlave)->GetAlStatus() & DEVICE_STATE_ERROR) ? EC_TRUE : EC_FALSE;
    pSlaveInfo->bEnableLinkMsgs         = ((CEcSlave*)pCfgSlave)->GetLinkMessagesEnabled();
    pSlaveInfo->dwErrorCode             = ((CEcSlave*)pCfgSlave)->GetAlStatusCode();
#if (defined INCLUDE_DC_SUPPORT)
    pSlaveInfo->bSyncPulseActive        = ((CEcSlave*)pCfgSlave)->GetDcSendSyncActivation();
    pSlaveInfo->dwDCSync0Period         = ((CEcSlave*)pCfgSlave)->GetDcRegisterCycleTime0();
    pSlaveInfo->dwDCSync1Period         = ((CEcSlave*)pCfgSlave)->GetDcRegisterCycleTime1();
#else
    pSlaveInfo->bSyncPulseActive        = EC_FALSE;
    pSlaveInfo->dwDCSync0Period         = 0;
    pSlaveInfo->dwDCSync1Period         = 0;
#endif /* INCLUDE_DC_SUPPORT */
    EC_SETDWORD(&pSlaveInfo->bSlaveIsOptional, ((CEcSlave*)pCfgSlave)->IsOptional());
    EC_SETDWORD(&pSlaveInfo->bSlaveIsPresent,  ((CEcSlave*)pCfgSlave)->IsPresentOnBus());
    EC_SETDWORD(&pSlaveInfo->dwHotConnectGroupId, ((CEcSlave*)pCfgSlave)->m_dwHCGroupId);
#if (defined INCLUDE_DC_SUPPORT)
    if (EC_NULL != ((CEcSlave*)pCfgSlave)->m_pBusSlave)
    {
        EC_SETDWORD(&pSlaveInfo->dwSystemTimeDifference, ((CEcSlave*)pCfgSlave)->m_pBusSlave->m_dwSystemTimeDifference);
    }
    else
#endif
    {
        EC_SETDWORD(&pSlaveInfo->dwSystemTimeDifference, 0);
    }

    if (pCfgSlave->m_pEniSlave->adwPDInOffset[0] == 0xffffffff)
    {
        EC_SETDWORD(&pSlaveInfo->dwPdOffsIn, 0);
    }
    else
    {
        EC_SETDWORD(&pSlaveInfo->dwPdOffsIn, pCfgSlave->m_pEniSlave->adwPDInOffset[0]);
    }

    EC_SETDWORD(&pSlaveInfo->dwPdSizeIn, pCfgSlave->m_pEniSlave->adwPDInSize[0]);

    if (pCfgSlave->m_pEniSlave->adwPDOutOffset[0] == 0xffffffff)
    {
        EC_SETDWORD(&pSlaveInfo->dwPdOffsOut, 0);
    }
    else
    {
        EC_SETDWORD(&pSlaveInfo->dwPdOffsOut, dwTmp);
    }

    EC_SETDWORD(&pSlaveInfo->dwPdSizeOut, pCfgSlave->m_pEniSlave->adwPDOutSize[0]);

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Set Slave error state.
*/
EC_T_VOID CEcSdoServ::SetSlaveError(
    EC_T_DWORD dwSDOSlaveIdx    /**< [in]   Master OD 0x3000 Slave Array Index */
    ,EC_T_DWORD dwALStatus      /**< [in]   Slave error status */
    ,EC_T_DWORD dwStatusCode    /**< [in]   Status code */
                                   )
{
    EC_T_WORD           wIndex      = 0;
    EC_T_OBJECTDICT*    pEntry      = EC_NULL;
    EC_T_OBJ3XXX*       pSlaveInfo  = EC_NULL;

    if( dwSDOSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD Range */
        goto Exit;
    }

    wIndex = (EC_T_WORD)(COEOBJID_SLAVECFGINFOBASE+dwSDOSlaveIdx);

    pEntry = GetObjectHandle(wIndex);

    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo = (EC_T_OBJ3XXX*)pEntry->pVarPtr;

    if( EC_NULL == pSlaveInfo )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo->dwCurState  =  dwALStatus & DEVICE_STATE_MASK;
    pSlaveInfo->bErrFlagSet = (dwALStatus & DEVICE_STATE_ERROR) ? EC_TRUE : EC_FALSE;
    pSlaveInfo->dwErrorCode = dwStatusCode;

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Update slave state in OD.
*/
EC_T_VOID CEcSdoServ::UpdateSlaveState(
    EC_T_DWORD dwSDOSlaveIdx    /**< [in]   Master OD 0x3000 Slave Array Index */
    ,EC_T_WORD wALStatus        /**< [in]   Slave state */
                                      )
{
    EC_T_WORD           wIndex          = 0;
    EC_T_OBJECTDICT*    pEntry          = EC_NULL;
    EC_T_OBJ3XXX*       pSlaveInfo3xxx  = EC_NULL;
    EC_T_OBJAXXX*       pSlaveInfoAxxx  = EC_NULL;

    if( dwSDOSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD Range */
        goto Exit;
    }

    wIndex = (EC_T_WORD)(COEOBJID_SLAVECFGINFOBASE+dwSDOSlaveIdx);

    pEntry = GetObjectHandle(wIndex);

    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo3xxx = (EC_T_OBJ3XXX*)pEntry->pVarPtr;

    if( EC_NULL == pSlaveInfo3xxx )
    {
        /* entry not found */
        goto Exit;
    }

    EC_SET_FRM_DWORD(&(pSlaveInfo3xxx->dwCurState), wALStatus & DEVICE_STATE_MASK);
    pSlaveInfo3xxx->bErrFlagSet = (wALStatus & DEVICE_STATE_ERROR) ? EC_TRUE : EC_FALSE;


    /*
     * Modular Device Profiles: EtherCAT Master
     * Object 0xA000 - 0xAFFF Slave Objects (slave diagnosis)
     */

    wIndex = (EC_T_WORD) (COEOBJID_SLAVEDIAGBASE + dwSDOSlaveIdx);

    pEntry = GetObjectHandle(wIndex);
    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfoAxxx = (EC_T_OBJAXXX *) pEntry->pVarPtr;
    if( EC_NULL == pSlaveInfoAxxx )
    {
        /* entry not found */
        goto Exit;
    }
    pSlaveInfoAxxx->wALStatus = wALStatus;


Exit:
    return;
}


/***************************************************************************************************/
/**
\brief  Update slave state in OD.
*/
EC_T_VOID CEcSdoServ::UpdateReqSlaveState(
    EC_T_DWORD  dwSDOSlaveIdx   /**< [in]   Master OD 0x3000 Slave Array Index */
    ,EC_T_STATE eReqState       /**< [in]   Slave state */
                                         )
{
    EC_T_WORD           wIndex      = 0;
    EC_T_OBJECTDICT*    pEntry      = EC_NULL;
    EC_T_OBJ3XXX*       pSlaveInfo  = EC_NULL;

    if( dwSDOSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD Range */
        goto Exit;
    }

    wIndex = (EC_T_WORD)(COEOBJID_SLAVECFGINFOBASE+dwSDOSlaveIdx);

    pEntry = GetObjectHandle(wIndex);

    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo = (EC_T_OBJ3XXX*)pEntry->pVarPtr;

    if( EC_NULL == pSlaveInfo )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo->dwReqState = (EC_T_WORD) eReqState;

Exit:
    return;
}

/***************************************************************************************************/
/**
\brief  Set Slave Counters.
*/
EC_T_VOID CEcSdoServ::SetSlaveAmount(
    EC_T_DWORD dwSBSlaves,      /**< [in]   SB Slaves */
    EC_T_DWORD dwSBDCSlaves,    /**< [in]   SB DC Slaves */
    EC_T_DWORD dwCfgSlaves,     /**< [in]   Cfg Slaves */
    EC_T_DWORD dwCfgMbxSlaves   /**< [in]   Mbx Slaves */
                                    )
{
    m_oBusDiagnosisObject.dwNumSlavesFound      = dwSBSlaves;
    m_oBusDiagnosisObject.dwNumDCSlavesFound    = dwSBDCSlaves;
    m_oBusDiagnosisObject.dwNumCfgSlaves        = dwCfgSlaves;
    m_oBusDiagnosisObject.dwNumMbxSlaves        = dwCfgMbxSlaves;

    /* Modular Device Profiles  */
    m_oModularDeviceProfileObject.wIndexDistance = 1;
    m_oModularDeviceProfileObject.wMaxModuleCnt  = m_wMaxSlavesObjs;
    EC_SETDWORD(&m_oModularDeviceProfileObject.dwGeneralInfo, dwSBSlaves);
    EC_SETDWORD(&m_oModularDeviceProfileObject.dwGeneralCfg, dwCfgSlaves);
}


#ifdef INCLUDE_RED_DEVICE
/***************************************************************************************************/
/**
\brief  Update Redundancy
*/
EC_T_VOID CEcSdoServ::UpdateRedundancy(
                                     EC_T_BYTE  byRedEnabled,       /**< [in]   Red enabled */
                                     EC_T_WORD  wNumOfMainSlaves,   /**< [in]   Number of active slaves on main line */
                                     EC_T_WORD  wNumOfRedSlaves,    /**< [in]   Number of active slaves on red line */
                                     EC_T_BYTE  byLineBreak         /**< [in]   Line is broken */
                                     )
{
  m_oRedundancyObject.byRedEnabled        = byRedEnabled;
  m_oRedundancyObject.wNumOfMainSlaves    = wNumOfMainSlaves;
  m_oRedundancyObject.wNumOfRedSlaves     = wNumOfRedSlaves;
  m_oRedundancyObject.byLineBreak         = byLineBreak;
}

#endif


/***************************************************************************************************/
/**
\brief  Add Notification
*/
EC_T_VOID CEcSdoServ::AddNotification(
                                     EC_T_DWORD  dwCode   /**< [in]   Notification code */
                                     )
{
  EC_T_DWORD dwMsgIdx = 0;

  /* Search message code and increment counter */
  for (dwMsgIdx=0;dwMsgIdx<MAX_NOTIFICATIONS;dwMsgIdx++)
  {
    if (m_aNotifyMessages[dwMsgIdx].dwCode == 0)
      break;
    if (m_aNotifyMessages[dwMsgIdx].dwCode != dwCode)
      continue;

    m_aNotifyMessages[dwMsgIdx].dwCount++;
    return;
  }

  /* Message code not found -> Add message code */
  for (dwMsgIdx=0;dwMsgIdx<MAX_NOTIFICATIONS;dwMsgIdx++)
  {
    if (m_aNotifyMessages[dwMsgIdx].dwCode != 0)
      continue;

    m_aNotifyMessages[dwMsgIdx].dwCode = dwCode;
    m_aNotifyMessages[dwMsgIdx].dwCount = 1;
    m_oNotifyCounterObject.byMessageCount++;
    m_oNotifyCounterObject.wSubIndex0 = (EC_T_WORD)(NOTIFICATION_MEMBER_COUNT - 1 + (m_oNotifyCounterObject.byMessageCount*2));
    m_pEntry0x2004->oObjDesc.wObjFlags = (EC_T_WORD)(m_oNotifyCounterObject.wSubIndex0 | (OBJCODE_REC << 8));
    return;
  }
}


/***************************************************************************************************/
/**
\brief  Clear Notifications
*/
EC_T_VOID CEcSdoServ::ClearNotifications()
{
  m_oNotifyCounterObject.byMessageCount         = 0;
  m_oNotifyCounterObject.byFlags                = 0;
  OsMemset(m_aNotifyMessages, 0, sizeof(m_aNotifyMessages));
}


/***************************************************************************************************/
/**
\brief  Update Error Counters

\return Pointer to counter variable.
*/
EC_T_VOID CEcSdoServ::UpdateErrorCounter(
    EC_T_DWORD  dwSDOSlaveIdx   /**< [in]   Master OD 0x3000 Slave Array Index */
   ,EC_T_BYTE*  pbyDataErrReg   /**< [in]   Slave Error Registers 0x300-0x313 */
                                          )
{
    EC_T_WORD           wIndex          = 0;
    EC_T_OBJECTDICT*    pEntry          = EC_NULL;
    EC_T_OBJ3XXX*       pSlaveInfo3xxx  = EC_NULL;

    if( dwSDOSlaveIdx >= m_wMaxSlavesObjs )
    {
        /* out of OD Range */
        goto Exit;
    }

    wIndex = (EC_T_WORD)(COEOBJID_SLAVECFGINFOBASE+dwSDOSlaveIdx);

    pEntry = GetObjectHandle(wIndex);

    if( EC_NULL == pEntry )
    {
        /* entry not found */
        goto Exit;
    }

    pSlaveInfo3xxx = (EC_T_OBJ3XXX*)pEntry->pVarPtr;

    if( EC_NULL == pSlaveInfo3xxx )
    {
        /* entry not found */
        goto Exit;
    }

    /* RX Error counter (0x300:0x307) */
    pSlaveInfo3xxx->wRxErrorCounter0 = EC_GET_FRM_WORD(pbyDataErrReg);
    pbyDataErrReg += sizeof(EC_T_WORD);

    pSlaveInfo3xxx->wRxErrorCounter1 = EC_GET_FRM_WORD(pbyDataErrReg);
    pbyDataErrReg += sizeof(EC_T_WORD);

    pSlaveInfo3xxx->wRxErrorCounter2 = EC_GET_FRM_WORD(pbyDataErrReg);
    pbyDataErrReg += sizeof(EC_T_WORD);

    pSlaveInfo3xxx->wRxErrorCounter3 = EC_GET_FRM_WORD(pbyDataErrReg);
    pbyDataErrReg += sizeof(EC_T_WORD);

    /* Forwarded RX Error Counter (0x0308:0x030B) */
    pSlaveInfo3xxx->byFwdRxErrorCounter0 = pbyDataErrReg[0];
    pSlaveInfo3xxx->byFwdRxErrorCounter1 = pbyDataErrReg[1];
    pSlaveInfo3xxx->byFwdRxErrorCounter2 = pbyDataErrReg[2];
    pSlaveInfo3xxx->byFwdRxErrorCounter3 = pbyDataErrReg[3];
    pbyDataErrReg += 4;

    /* ECAT Processing Unit Error Counter (0x30C) */
    pSlaveInfo3xxx->byEcatProcUnitErrorCounter = pbyDataErrReg[0];
    pbyDataErrReg += sizeof(EC_T_BYTE);

    /* PDI Error Counter (0x30D) */
    pSlaveInfo3xxx->byPDIErrorCounter = pbyDataErrReg[0];
    pbyDataErrReg += sizeof(EC_T_WORD)+sizeof(EC_T_BYTE);

    /* Lost Link Counter (0x0310:0x313) */
    pSlaveInfo3xxx->byLostLinkCounter0 = pbyDataErrReg[0];
    pSlaveInfo3xxx->byLostLinkCounter1 = pbyDataErrReg[1];
    pSlaveInfo3xxx->byLostLinkCounter2 = pbyDataErrReg[2];
    pSlaveInfo3xxx->byLostLinkCounter3 = pbyDataErrReg[3];

Exit:
    return;
}



/***************************************************************************************************/
/**
\brief  Store Config File CRC32
*/
EC_T_VOID CEcSdoServ::SetConfigCRC32ChkSum(
    EC_T_DWORD  dwCRC32Sum      /**< [in]   Config file CRC32 Hash */
                                          )
{
    m_oBusDiagnosisObject.dwCRC32ConfigCheckSum = dwCRC32Sum;
}

/***************************************************************************************************/
/**
 \param     dwDiagCode          Diagnosis code according to ETG.1020
 \param     byType              Type of the diagnosis message
                                0 = Info
                                1 = Warning
                                2 = Error
 \param     wTextID             Device unique textID defined in the device description (ETG.2000)
 \param     byNumParam          Number of parameters which should be added to the message
 \param     pParam              Pointer to the first parameter information
                                The information contains parameter flags according to ETG.1020 and
                                a pointer to local memory which should be added as parameter
                                (Note: the parameter information passed to this function shall be located in continuous memory)

 \brief    This function creates a new diagnosis message
*/
EC_T_VOID CEcSdoServ::Diag_CreateNewMessage
    (EC_T_DWORD     dwDiagCode
    ,EC_T_BYTE      byType
    ,EC_T_WORD      wTextID
    ,EC_T_BYTE      byNumParam
    ,EC_T_DIAGMSGPARAMINFO *pParam)
{
    EC_T_BYTE MaskedType = (EC_T_BYTE)(byType & DIAG_MSG_TYPE_MASK);

    /*Check if Diagnosis Message shall be stored in Diagnosis Message queue*/

    if(((m_oHistoryObject.wFlags & DIAG_OVERWRITE_DISCARD) == 0 || (m_oHistoryObject.wFlags & DIAG_OPERATION_MODE) == 0) &&
        (((MaskedType == DIAG_MSG_TYPE_INFO) && ((m_oHistoryObject.wFlags & DIAG_DISABLE_INFO_MSG) == 0))
        || ((MaskedType == DIAG_MSG_TYPE_WARNING) && ((m_oHistoryObject.wFlags & DIAG_DISABLE_WARNING_MSG) == 0))
        || ((MaskedType == DIAG_MSG_TYPE_ERROR) && ((m_oHistoryObject.wFlags & DIAG_DISABLE_ERROR_MSG) == 0))))
    {
        /*message buffer is not full or message history is working in Overwrite mode */
        EC_T_BYTE byNextBuffer = 0;
        EC_T_OBJ10F3_DIAGMSG *pNewDiagMsg = NULL;
        EC_T_BYTE wCnt = 0;
        EC_T_DIAGMSGPARAMINFO *pParamWorking = pParam;

        /* get next buffer if a diagnosis message was already stored */
        if(m_oHistoryObject.byNewestMessage > 5)
        {
            /*Get current array index*/
            byNextBuffer = (EC_T_BYTE)(m_oHistoryObject.byNewestMessage - 6);

            byNextBuffer++;

            /*Wrap around if last buffer was used*/
            if(byNextBuffer >= MAX_DIAG_MSG)
            {
                byNextBuffer = 0;

                if(m_oHistoryObject.byNewestAckMessage == 0)
                {
                    /*no message was acknowledged yet => set Overwrite/Discard Flag*/
                    m_oHistoryObject.wFlags |= (EC_T_WORD)DIAG_OVERWRITE_DISCARD;
                }
            }

            /*Overrun flag need to be set if the next Message index would be equal to newest acknowledged Message */
            if(m_oHistoryObject.byNewestAckMessage == (byNextBuffer + 6))
            {
                m_oHistoryObject.wFlags |= (EC_T_WORD)DIAG_OVERWRITE_DISCARD;

                if ((m_oHistoryObject.wFlags & DIAG_OPERATION_MODE) == 0)
                {
                    /* message history is working in overwrite mode => reset SI3 ("Newest acknowledged Message") and local "LastReadIndex" indication */
                    m_oHistoryObject.byNewestAckMessage = 0;
                    m_byLastDiagMsgRead = 0;
                }
            }
        }


        /* Continue if Overwrite/Discard flag is 0 or teh message history is handled in Overwrite Mode */
        if ((m_oHistoryObject.wFlags & DIAG_OVERWRITE_DISCARD) == 0 || (m_oHistoryObject.wFlags & DIAG_OPERATION_MODE) == 0)
        {
            m_aDiagMessages[byNextBuffer].wMessageLength = DIAG_MSG_DEFAULT_LEN;

            if (byNumParam > 0 && pParam != NULL)
            {
                EC_T_WORD wParamLength = 0;
                EC_T_BYTE ParamType = 0;
                /* Calculate additional buffer length for parameters */
                for (wCnt = 0; wCnt < byNumParam ; wCnt++)
                {
                    wParamLength = 2; /* add length for parameter wFlags */
                    ParamType = (EC_T_BYTE)(pParamWorking->wParamFlags >> DIAG_MSG_PARAM_TYPE_OFFSET);

                    if (ParamType == DIAG_MSG_PARAM_TYPE_DATA)
                    {
                        if (((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_INTEGER8)
                            ||((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_UNSIGNED8))
                        {
                            wParamLength += 1;
                        }
                        else if(((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_INTEGER16)
                            ||((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_UNSIGNED16))
                        {
                            wParamLength += 2;
                        }
                        else if(((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_INTEGER32)
                            ||((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_UNSIGNED32))
                        {
                            wParamLength += 4;
                        }
                    }
                    else if((ParamType == DIAG_MSG_PARAM_TYPE_B_ARRY)
                        || (ParamType == DIAG_MSG_PARAM_TYPE_ASCII)
                        || (ParamType == DIAG_MSG_PARAM_TYPE_UNICODE))
                    {
                        /*The length for Byte arrays, ASCII and UNICODE ist stored in bit 0 - 11*/
                        wParamLength = (EC_T_WORD)(wParamLength + (pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK));
                    }
                    /*parameter types 4 -15 reserved for future standardization*/

                    m_aDiagMessages[byNextBuffer].wMessageLength = (EC_T_WORD)(m_aDiagMessages[byNextBuffer].wMessageLength + wParamLength);

                    /*Get next parameter*/
                    pParamWorking++;
                }
            } /* Parameters available */

            m_aDiagMessages[byNextBuffer].wMessageLength = (EC_T_WORD)EC_MIN(m_aDiagMessages[byNextBuffer].wMessageLength, HISTORY_OBJECT_DIAGELE_SIZE-1);

            if (m_aDiagMessages[byNextBuffer].pMessage != NULL)
            {
                OsMemset(m_aDiagMessages[byNextBuffer].pMessage,0x00,m_aDiagMessages[byNextBuffer].wMessageLength);

                /* Initialize new allocated buffer */
                pNewDiagMsg = (EC_T_OBJ10F3_DIAGMSG *)m_aDiagMessages[byNextBuffer].pMessage;

                EC_SET_FRM_DWORD(&pNewDiagMsg->dwDiagNumber, dwDiagCode);
                EC_SET_FRM_WORD(&pNewDiagMsg->wFlags, (EC_T_WORD)((((EC_T_WORD)byNumParam) << 8) | byType));
                EC_SET_FRM_WORD(&pNewDiagMsg->wTextId, wTextID);

#if (defined INCLUDE_DC_SUPPORT)
                if (m_pMaster->m_pDistributedClks->GetBusTime() != 0)
                {
#ifdef  __TMS470__
                    EC_T_UINT64 qwTimeStamp = m_pMaster->m_pDistributedClks->GetBusTime();
                    pNewDiagMsg->dwTimeStampLo = EC_LODWORD(qwTimeStamp);
                    pNewDiagMsg->dwTimeStampHi = EC_HIDWORD(qwTimeStamp);
#else
                    pNewDiagMsg->qwTimeStamp = m_pMaster->m_pDistributedClks->GetBusTime();
#endif
                }
                else
#endif
                {
                    if (m_pMaster->m_dwMsecCounter != m_dwPrevMsecCounter)
                    {
                        OsSystemTimeGet(&m_qwSystemTime);
                        m_dwPrevMsecCounter = m_pMaster->m_dwMsecCounter;
                    }
#ifdef  __TMS470__
                    pNewDiagMsg->dwTimeStampLo = EC_LODWORD(m_qwSystemTime);
                    pNewDiagMsg->dwTimeStampHi = EC_HIDWORD(m_qwSystemTime);
#else
                    pNewDiagMsg->qwTimeStamp = m_qwSystemTime;
#endif
                }

                /*copy parameters to message buffer*/
                if(byNumParam > 0 && pParam != NULL)
                {
                    EC_T_BYTE *pMsgParamBuffer = (EC_T_BYTE *)&pNewDiagMsg->oParameter;
                    EC_T_WORD wLengthToCopy = 0;
                    EC_T_BYTE ParamType;
                    pParamWorking = pParam;

                    for(wCnt = 0; wCnt < byNumParam ; wCnt++)
                    {
                        EC_T_WORD u16PFlag;

                        EC_SET_FRM_WORD(&u16PFlag, pParamWorking->wParamFlags);

                        OsMemcpy(pMsgParamBuffer, &u16PFlag, 2);
                        pMsgParamBuffer+=2;
                        ParamType = (EC_T_BYTE)(pParamWorking->wParamFlags >> DIAG_MSG_PARAM_TYPE_OFFSET);

                        if(ParamType == DIAG_MSG_PARAM_TYPE_DATA)
                        {
                            if(((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_INTEGER8)
                                ||((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_UNSIGNED8))
                            {
                                wLengthToCopy = 1;
                            }
                            else if(((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_INTEGER16)
                                ||((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_UNSIGNED16))
                            {
                                wLengthToCopy = 2;
                            }
                            else if(((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_INTEGER32)
                                ||((pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK) == DEFTYPE_UNSIGNED32))
                            {
                                wLengthToCopy = 4;
                            }
                        }
                        else if((ParamType == DIAG_MSG_PARAM_TYPE_B_ARRY)
                            || (ParamType == DIAG_MSG_PARAM_TYPE_ASCII)
                            || (ParamType == DIAG_MSG_PARAM_TYPE_UNICODE))
                        {
                            /*The length for Byte arrays, ASCII and UNICODE ist stored in bit 0 - 11*/
                            wLengthToCopy = (EC_T_WORD)(pParamWorking->wParamFlags & ~DIAG_MSG_PARAM_TYPE_MASK);
                        }
                        /*parameter types 4 -15 reserved for future standardization*/

                        /*Copy Parameter Value to message buffer*/
                        if(wLengthToCopy > 0 )
                        {
                            OsMemcpy(pMsgParamBuffer, pParamWorking->pvAddress, wLengthToCopy);
                            pMsgParamBuffer += wLengthToCopy;
                            wLengthToCopy = 0;
                        }
                        /*Get next parameter*/
                        pParamWorking++;
                    }
                }

                /*Update newest message index */
                m_oHistoryObject.byNewestMessage = (EC_T_BYTE)(byNextBuffer + 6);

                /*indicate new DiagMessage*/
                m_oHistoryObject.byNewDiagMessages = 1;

                 /*increment subindex 0 until all message entries used at least once*/
                if (m_oHistoryObject.wSubIndex0 < (MAX_DIAG_MSG +5))
                {
                    m_oHistoryObject.wSubIndex0++;
                    m_pEntry0x10F3->oObjDesc.wObjFlags = (EC_T_WORD)(m_oHistoryObject.wSubIndex0 | (OBJCODE_REC << 8));
                }
            }
            else
            {
                /*No valid buffer allocated*/
                m_aDiagMessages[byNextBuffer].wMessageLength = 0;
            }
        } /* no Overwrite/Discard flag or message history is working in overwrite mode */
    }
}


/***************************************************************************************************/
/**
\brief  Add History Entry from Notification.
*/
EC_T_VOID CEcSdoServ::AddHistoryNotification(
    EC_T_DWORD  dwCode,         /**< [in]   Notification code */
    EC_T_NOTIFICATION_DATA*  pNotificationData   /**< [in]   Notification data */
                                            )
{
    AddNotification(dwCode);

    /* PerfMeasStart(&G_TscMeasDesc, EC_MSMT_AddHistoryNotificationCore); */
    switch( dwCode )
    {
    case EC_NOTIFY_STATECHANGED:
        {
            EC_T_STATECHANGE*   pStChange   = &(pNotificationData->Notification.desc.StatusChngNtfyDesc);
            EC_T_DIAGMSGPARAMINFO oDiagPara[2];

            /* EC_SZTXT_TXT_MASTER_STATE_CHANGE        "Master state changed from <%s> to <%s>" */
            oDiagPara[0].pvAddress = (EC_T_VOID*)ecatDeviceStateText(pStChange->oldState);
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            oDiagPara[1].pvAddress = (EC_T_VOID*)ecatDeviceStateText(pStChange->newState);
            oDiagPara[1].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[1].pvAddress));

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_INFO, EC_TXT_MASTER_STATE_CHANGE, 2, oDiagPara);

            EntrySetMasterState_State((EC_T_DWORD)(pStChange->newState));
        } break;
    case EC_NOTIFY_ETH_LINK_CONNECTED:
        {
            /* EC_SZTXT_TXT_CABLE_CONNECTED            "Ethernet cable connected" */
            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_INFO, EC_TXT_CABLE_CONNECTED, 0, EC_NULL);

            EntrySet_SetLinkUp(EC_TRUE);
        } break;
    case EC_NOTIFY_SB_STATUS:
        {
            EC_T_SB_STATUS_NTFY_DESC*   pSbStNot    = &(pNotificationData->Notification.desc.ScanBusNtfyDesc);

            if( EC_E_NOERROR == pSbStNot->dwResultCode )
            {
                EC_T_DIAGMSGPARAMINFO oDiagPara;

                /* EC_SZTXT_TXT_SB_RESULT_OK               "Scan Bus Succeeded, found %d slaves" */
                oDiagPara.wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET) | DEFTYPE_UNSIGNED32);
				EC_T_DWORD dwSlaveCount = EC_GET_FRM_DWORD(&pSbStNot->dwSlaveCount);
                oDiagPara.pvAddress = (EC_T_VOID*)&dwSlaveCount;

                Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_INFO, EC_TXT_SB_RESULT_OK, 1, &oDiagPara);

                EntrySet_BusScanMatch(EC_TRUE);
            }
            else if( EC_E_BUSCONFIG_MISMATCH == pSbStNot->dwResultCode )
            {
                EC_T_DIAGMSGPARAMINFO oDiagPara[3];

                /* EC_SZTXT_TXT_SB_RESULT_ERROR            "Bus scan error '%s (0x%x)', %d slaves found" */

                oDiagPara[0].pvAddress = (EC_T_VOID*)EcErrorText(pSbStNot->dwResultCode);
                oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

                EC_T_DWORD dwResultCode = EC_GET_FRM_DWORD(&pSbStNot->dwResultCode);
				oDiagPara[1].pvAddress = (EC_T_VOID*)&dwResultCode;
                oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

                EC_T_DWORD dwSlaveCount = EC_GET_FRM_DWORD(&pSbStNot->dwSlaveCount);
				oDiagPara[2].pvAddress = (EC_T_VOID*)&dwSlaveCount;
                oDiagPara[2].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

                Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_SB_RESULT_ERROR, 3, oDiagPara);

                EntrySet_BusScanMatch(EC_FALSE);
            }
        } break;
    case EC_NOTIFY_DC_STATUS:
        {
            EC_T_DIAGMSGPARAMINFO   oDiagPara[2];
            EC_T_BYTE               byType = 0;
            EC_T_DWORD              dwDcStatus  = pNotificationData->Notification.desc.StatusCode;

            /* EC_SZTXT_TXT_DC_STATUS                  "Distributed clocks - status %s (0x%x)" */
            switch( dwDcStatus )
            {
            case EC_E_NOERROR:  byType          = DIAG_MSG_TYPE_INFO; break;
            case EC_E_BUSY:     byType          = DIAG_MSG_TYPE_WARNING; break;
            case EC_E_NOTREADY: byType          = DIAG_MSG_TYPE_WARNING; break;
            default:            byType          = DIAG_MSG_TYPE_ERROR; break;
            }

            oDiagPara[0].pvAddress = (EC_T_VOID*)EcErrorText(dwDcStatus);
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            dwDcStatus = EC_GET_FRM_DWORD(&dwDcStatus);
			oDiagPara[1].pvAddress = &dwDcStatus;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            Diag_CreateNewMessage(dwCode, byType, EC_TXT_DC_STATUS, 2, oDiagPara);

        } break;
    case EC_NOTIFY_DC_SLV_SYNC:
        {
            EC_T_DIAGMSGPARAMINFO   oDiagPara[2];
            EC_T_BYTE               byType = 0;
            EC_T_WORD               wTextId;
            EC_T_DC_SYNC_NTFY_DESC* pSlvSyncNot = &(pNotificationData->Notification.desc.SyncNtfyDesc);

            if( pSlvSyncNot->IsInSync )
            {
                byType = DIAG_MSG_TYPE_INFO;
                wTextId = EC_TXT_DCSLVSYNC_INSYNC;
                /* EC_SZTXT_TXT_DCSLVSYNC_INSYNC           "DC slaves are \"in-sync\" - Deviation: 0x%x" */
                EntrySet_SetDCInSync(EC_TRUE);
            }
            else
            {
                byType = DIAG_MSG_TYPE_WARNING;
                wTextId = EC_TXT_DCSLVSYNC_OUTOFSYNC;
                /* EC_SZTXT_TXT_DCSLVSYNC_OUTOFSYNC        "DC slaves are \"out-of-sync\" - Deviation: 0x%x" */
                EntrySet_SetDCInSync(EC_FALSE);
            }

            EC_T_DWORD dwDeviation = EC_GET_FRM_DWORD(&pSlvSyncNot->dwDeviation);
			oDiagPara[0].pvAddress = (EC_T_VOID*)&dwDeviation;
            oDiagPara[0].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            Diag_CreateNewMessage(dwCode, byType, wTextId, 1, oDiagPara);

        } break;
    case EC_NOTIFY_DCL_STATUS:
        {
            EC_T_DIAGMSGPARAMINFO   oDiagPara[2];
            EC_T_BYTE               byType = 0;
            EC_T_DWORD              dwDclStatus  = pNotificationData->Notification.desc.StatusCode;

            /* EC_SZTXT_TXT_DCL_STATUS                 "Distributed clocks - latching status %s (0x%x)" */
            switch( dwDclStatus )
            {
            case EC_E_NOERROR:  byType          = DIAG_MSG_TYPE_INFO; break;
            case EC_E_BUSY:     byType          = DIAG_MSG_TYPE_WARNING; break;
            case EC_E_NOTREADY: byType          = DIAG_MSG_TYPE_WARNING; break;
            default:            byType          = DIAG_MSG_TYPE_ERROR; break;
            }

            oDiagPara[0].pvAddress = (EC_T_VOID*)EcErrorText(dwDclStatus);
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            dwDclStatus = EC_GET_FRM_DWORD(&dwDclStatus);
			oDiagPara[1].pvAddress = &dwDclStatus;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            Diag_CreateNewMessage(dwCode, byType, EC_TXT_DCL_STATUS, 2, oDiagPara);

        } break;
    case EC_NOTIFY_CYCCMD_WKC_ERROR:
    case EC_NOTIFY_MASTER_INITCMD_WKC_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO oDiagPara[4];

            EC_T_WKCERR_DESC* pWkcErr = &(pNotificationData->ErrorNotification.desc.WkcErrDesc);

            /* EC_SZTXT_TXT_MASTINITCMD_WKC_ERROR      "Master init command working counter error - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            /* EC_SZTXT_TXT_CYCCMD_WKC_ERROR           "     Cyclic command working counter error - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d"  */
            oDiagPara[0].pvAddress = (EC_T_VOID*)EcatCmdShortText(pWkcErr->byCmd);
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_DWORD	dwAddr = EC_GET_FRM_DWORD(&pWkcErr->dwAddr);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&dwAddr;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            EC_T_WORD wWkcAct = EC_GET_FRM_WORD(&pWkcErr->wWkcAct);
			oDiagPara[2].pvAddress = (EC_T_VOID*)&wWkcAct;
            oDiagPara[2].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            EC_T_WORD wWkcSet = EC_GET_FRM_WORD(&pWkcErr->wWkcSet);
			oDiagPara[3].pvAddress = (EC_T_VOID*)&wWkcSet;
            oDiagPara[3].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            if(dwCode == EC_NOTIFY_MASTER_INITCMD_WKC_ERROR)
            {
                Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_MASTINITCMD_WKC_ERROR, 4, oDiagPara);
            }
            else
            {
                Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_CYCCMD_WKC_ERROR, 4, oDiagPara);
            }
        } break;
    case EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR:
    case EC_NOTIFY_EOE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_COE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_FOE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_SOE_MBXSND_WKC_ERROR:
    case EC_NOTIFY_VOE_MBXSND_WKC_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO oDiagPara[6];
            EC_T_WORD             wTextId;
            EC_T_WKCERR_DESC*     pWkcErr     = &(pNotificationData->ErrorNotification.desc.WkcErrDesc);

            switch( dwCode )
            {
            case EC_NOTIFY_EOE_MBXSND_WKC_ERROR:
                wTextId = EC_TXT_EOEMBXSND_WKC_ERROR;  break;
                /* EC_SZTXT_TXT_EOEMBXSND_WKC_ERROR        "EoE mbox send    working counter error - Properties for slave  \"%s\": - Physical station address=%d - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            case EC_NOTIFY_COE_MBXSND_WKC_ERROR:
                wTextId = EC_TXT_COEMBXSND_WKC_ERROR;  break;
                /* EC_SZTXT_TXT_COEMBXSND_WKC_ERROR        "CoE mbox send    working counter error - Properties for slave  \"%s\": - Physical station address=%d - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            case EC_NOTIFY_FOE_MBXSND_WKC_ERROR:
                wTextId = EC_TXT_FOEMBXSND_WKC_ERROR;  break;
                /* EC_SZTXT_TXT_FOEMBXSND_WKC_ERROR        "FoE mbox send    working counter error - Properties for slave  \"%s\": - Physical station address=%d - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            case EC_NOTIFY_SOE_MBXSND_WKC_ERROR:
                wTextId = EC_TXT_SOEMBXSND_WKC_ERROR;  break;
                /* EC_SZTXT_TXT_SOEMBXSND_WKC_ERROR        "SoE mbox send    working counter error - Properties for slave  \"%s\": - Physical station address=%d - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            case EC_NOTIFY_VOE_MBXSND_WKC_ERROR:
                wTextId = EC_TXT_VOEMBXSND_WKC_ERROR;  break;
                /* EC_SZTXT_TXT_VOEMBXSND_WKC_ERROR        "VoE mbox send working counter error - Slave  \"%s\": - EtherCAT address=%d - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            case EC_NOTIFY_SLAVE_INITCMD_WKC_ERROR:
            default:
                wTextId = EC_TXT_SLVINITCMD_WKC_ERROR; break;
                /* EC_SZTXT_TXT_SLVINITCMD_WKC_ERROR       "Slave init command working counter error - Properties for slave  \"%s\": - Physical station address=%d - Command: %s - Logical/Physical address: 0x%x, WKC act/set=%d/%d" */
            }

            oDiagPara[0].pvAddress = pWkcErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pWkcErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = (EC_T_VOID*)EcatCmdShortText(pWkcErr->byCmd);
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            EC_T_DWORD dwAddr = EC_GET_FRM_DWORD(&pWkcErr->dwAddr);
			oDiagPara[3].pvAddress = (EC_T_VOID*)&dwAddr;
            oDiagPara[3].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            EC_T_WORD wWkcAct = EC_GET_FRM_WORD(&pWkcErr->wWkcAct);
			oDiagPara[4].pvAddress = (EC_T_VOID*)&wWkcAct;
            oDiagPara[4].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            EC_T_WORD wWkcSet = EC_GET_FRM_WORD(&pWkcErr->wWkcSet);
			oDiagPara[5].pvAddress = (EC_T_VOID*)&wWkcSet;
            oDiagPara[5].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, wTextId, 6, oDiagPara);

        } break;
    case EC_NOTIFY_FRAME_RESPONSE_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO       oDiagPara[2];
            EC_T_FRAME_RSPERR_DESC*     pFrmRspErr  = &(pNotificationData->ErrorNotification.desc.FrameRspErrDesc);

            /* EC_SZTXT_TXT_FRMRESP_NORETRY            "%s response on %s Ethernet frame" */
            switch( pFrmRspErr->EErrorType )
            {
            case eRspErr_NO_RESPONSE:   oDiagPara[0].pvAddress = (EC_T_VOID*)ecatGetText(EC_TXT_FRAME_RESPONSE_ERRTYPE_NO);          break;
            case eRspErr_WRONG_IDX:     oDiagPara[0].pvAddress = (EC_T_VOID*)ecatGetText(EC_TXT_FRAME_RESPONSE_ERRTYPE_WRONG);       break;
            case eRspErr_UNEXPECTED:    oDiagPara[0].pvAddress = (EC_T_VOID*)ecatGetText(EC_TXT_FRAME_RESPONSE_ERRTYPE_UNEXPECTED);  break;
            case eRspErr_RETRY_FAIL:    oDiagPara[0].pvAddress = (EC_T_VOID*)ecatGetText(EC_TXT_FRAME_RESPONSE_ERRTYPE_RETRY_FAIL);  break;
            default:                    oDiagPara[0].pvAddress = (EC_T_CHAR*)"@@internal error@@";  break;
            }
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            oDiagPara[1].pvAddress = (EC_T_VOID*)((pFrmRspErr->bIsCyclicFrame)?ecatGetText(EC_TXT_FRAME_TYPE_CYCLIC): ecatGetText(EC_TXT_FRAME_TYPE_ACYCLIC));
            oDiagPara[1].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[1].pvAddress));

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_FRMRESP_NORETRY, 2, oDiagPara);
        } break;
    case EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR:
    case EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT:
        {
            EC_T_DIAGMSGPARAMINFO oDiagPara[3];
            EC_T_WORD             wTextId;
            EC_T_INITCMD_ERR_DESC*  pInitErr    = &(pNotificationData->ErrorNotification.desc.InitCmdErrDesc);

            switch( dwCode )
            {
            case EC_NOTIFY_MBSLAVE_INITCMD_TIMEOUT:
                wTextId = EC_TXT_MBSLV_INITCMDTO; break;
                /* EC_SZTXT_TXT_MBSLV_INITCMDTO            "\"Mailbox init command\" time-out - Slave  \"%s\": - EtherCAT address=%d - Current State change of slave=\"%s\"" */
            case EC_NOTIFY_SLAVE_INITCMD_RESPONSE_ERROR:
            default:
                {
                    switch( pInitErr->EErrorType )
                    {
                    case eInitCmdErr_NO_RESPONSE:
                        wTextId = EC_TXT_SLVINITCMDRSPERR_NR; break;    /* EC_SZTXT_TXT_SLVINITCMDRSPERR_NR        "        Slave init command response error - Properties for slave  \"%s\": - Physical station address=%d - Current State change of         slave = \"%s\" No Response, is there a slave at this position?" */
                    case eInitCmdErr_VALIDATION_ERR:
                        wTextId = EC_TXT_SLVINITCMDRSPERR_VE; break;    /* EC_SZTXT_TXT_SLVINITCMDRSPERR_VE        "        Slave init command response error - Properties for slave  \"%s\": - Physical station address=%d - Current State change of         slave = \"%s\" Validation error, is the correct slave at this position?" */
                    case eInitCmdErr_FAILED:
                    default:
                        wTextId = EC_TXT_SLVINITCMDRSPERR_FLD; break;   /* EC_SZTXT_TXT_SLVINITCMDRSPERR_FLD       "        Slave init command response error - Properties for slave  \"%s\": - Physical station address=%d - Current State change of         slave = \"%s\" target state could not be reached, is the correct slave at this position?"*/
                    }
                } break;
            }

            oDiagPara[0].pvAddress = pInitErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pInitErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = pInitErr->achStateChangeName;
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, wTextId, 3, oDiagPara);
        } break;

    case EC_NOTIFY_MASTER_INITCMD_RESPONSE_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO   oDiagPara[2];
            EC_T_WORD               wTextId;
            EC_T_INITCMD_ERR_DESC*  pInitErr    = &(pNotificationData->ErrorNotification.desc.InitCmdErrDesc);

            switch( pInitErr->EErrorType )
            {
            case eInitCmdErr_VALIDATION_ERR:
                wTextId = EC_TXT_MASTINITCMDRSPERR_VE; break;
            /* EC_SZTXT_TXT_MASTINITCMDRSPERR_VE       "Master init command response error - Current State change of master=\"%s\" Validation error, are the correct slaves connected?" */
            case eInitCmdErr_NO_RESPONSE:
            default:
                wTextId = EC_TXT_MASTINITCMDRSPERR_NR; break;
            /* EC_SZTXT_TXT_MASTINITCMDRSPERR_NR       "Master init command response error - Current State change of master=\"%s\" No Response, is there anything connected?" */
            }

            oDiagPara[0].pvAddress = pInitErr->achStateChangeName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, wTextId, 1, oDiagPara);
        } break;
    case EC_NOTIFY_NOT_ALL_DEVICES_OPERATIONAL:
        {
            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_NOT_ALL_DEVS_OP, 0, EC_NULL);
        } break;
    case EC_NOTIFY_ETH_LINK_NOT_CONNECTED:
        {
            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_CABLE_NOT_CONNECTED, 0, EC_NULL);
            EntrySet_SetLinkUp(EC_FALSE);
        } break;
    case EC_NOTIFY_RED_LINEBRK:
        {
            EC_T_DIAGMSGPARAMINFO   oDiagPara[2];
            EC_T_WORD               wHelp;
            EC_T_RED_CHANGE_DESC*   pRedChange    = &(pNotificationData->ErrorNotification.desc.RedChangeDesc);

            /* EC_SZTXT_TXT_REDLINEBREAK               "ERROR: Redundancy Line break between slave %d and %d" */
            EC_T_WORD wNumOfSlavesMain = EC_GET_FRM_WORD(&pRedChange->wNumOfSlavesMain);
			oDiagPara[0].pvAddress = (EC_T_VOID*)&wNumOfSlavesMain;
            oDiagPara[0].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            wHelp = (EC_T_WORD)(pRedChange->wNumOfSlavesMain + 1);
			wHelp = EC_GET_FRM_WORD(&wHelp);
            oDiagPara[1].pvAddress = &wHelp;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_WARNING, EC_TXT_REDLINEBREAK, 2, oDiagPara);
        } break;
    case EC_NOTIFY_STATUS_SLAVE_ERROR:
        {
            /* EC_SZTXT_TXT_SLVERR_DETECTED            "ERROR: At least one slave is in error status!" */
            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_WARNING, EC_TXT_SLVERR_DETECTED, 0, EC_NULL);
        } break;
    case EC_NOTIFY_SLAVE_ERROR_STATUS_INFO:
        {
            EC_T_DIAGMSGPARAMINFO oDiagPara[7];
            EC_T_SLAVE_ERROR_INFO_DESC* pSlaveErr   = &(pNotificationData->ErrorNotification.desc.SlaveErrInfoDesc);

            /* EC_SZTXT_TXT_SLVERR_INFO                "Slave error \"%s\": - EtherCAT address=%d - State <%s%s>(0x%x), control status <%s>(0x%x)" */
            oDiagPara[0].pvAddress = pSlaveErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pSlaveErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = (EC_T_VOID*)ecatSlaveStateText(pSlaveErr->wStatus);
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            oDiagPara[3].pvAddress = (EC_T_VOID*)("");
            oDiagPara[3].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[3].pvAddress));

            EC_T_WORD wStatus = EC_GET_FRM_WORD(&pSlaveErr->wStatus);
			oDiagPara[4].pvAddress = (EC_T_VOID*)&wStatus;
            oDiagPara[4].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[5].pvAddress = (EC_T_VOID*)SlaveDevStatusCodeText(pSlaveErr->wStatusCode);
            oDiagPara[5].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[5].pvAddress));

            EC_T_WORD wStatusCode = EC_GET_FRM_WORD(&pSlaveErr->wStatusCode);
			oDiagPara[6].pvAddress = (EC_T_VOID*)&wStatusCode;
            oDiagPara[6].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_SLVERR_INFO, 7, oDiagPara);

        } break;
    case EC_NOTIFY_SLAVE_NOT_ADDRESSABLE:
        {
            EC_T_DIAGMSGPARAMINFO oDiagPara[2];
            EC_T_WKCERR_DESC*   pWkcErr     = &(pNotificationData->ErrorNotification.desc.WkcErrDesc);

            /* EC_SZTXT_TXT_SLV_NOT_ADDRABLE           "Unable to address slave \"%s\": - EtherCAT address=%d" */
            oDiagPara[0].pvAddress = pWkcErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pWkcErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_SLV_NOT_ADDRABLE, 2, oDiagPara);

        } break;

    case EC_NOTIFY_SOE_WRITE_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO   oDiagPara[3];
            EC_T_INITCMD_ERR_DESC*  pInitErr    = &(pNotificationData->ErrorNotification.desc.InitCmdErrDesc);

            /* EC_SZTXT_TXT_SOEMBX_WRITE_ERROR         "SoE init command mbox write error - Slave  \"%s\": - EtherCAT address=%d - Current State change of slave=\"%s\"" */
            oDiagPara[0].pvAddress = pInitErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pInitErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = pInitErr->achStateChangeName;
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_SOEMBX_WRITE_ERROR, 3, oDiagPara);
        } break;
    case EC_NOTIFY_MBSLAVE_COE_SDO_ABORT:
        {
            EC_T_DIAGMSGPARAMINFO       oDiagPara[6];
            EC_T_MBOX_SDO_ABORT_DESC*   pMbxSdoAbort    = &(pNotificationData->ErrorNotification.desc.SdoAbortDesc);

            /* EC_SZTXT_TXT_MBSLV_SDO_ABORT            "SDO abort - Slave  \"%s\": - EtherCAT address=%d - %s (0x%x) - Index=0x%x SubIndex=0x%x" */

            oDiagPara[0].pvAddress = pMbxSdoAbort->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

			EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pMbxSdoAbort->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = (EC_T_VOID*)EcErrorText(pMbxSdoAbort->dwErrorCode);
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            EC_T_DWORD dwErrorCode = EC_GET_FRM_DWORD(&pMbxSdoAbort->dwErrorCode);
			oDiagPara[3].pvAddress = (EC_T_VOID*)&dwErrorCode;
            oDiagPara[3].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            EC_T_WORD wObjIndex = EC_GET_FRM_WORD(&pMbxSdoAbort->wObjIndex);
			oDiagPara[4].pvAddress = (EC_T_VOID*)&wObjIndex;
            oDiagPara[4].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[5].pvAddress = &pMbxSdoAbort->bySubIndex;
            oDiagPara[5].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED8);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_MBSLV_SDO_ABORT, 6, oDiagPara);
        } break;

    case EC_NOTIFY_RED_LINEFIXED:
        {
            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_INFO, EC_TXT_REDLINEFIXED, 0, EC_NULL);
        } break;

    case EC_NOTIFY_FOE_MBSLAVE_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO       oDiagPara[4];
            EC_T_MBOX_FOE_ABORT_DESC* pFoeErr = &(pNotificationData->ErrorNotification.desc.FoeErrorDesc);

            /* EC_SZTXT_TXT_MBSLV_FOE_ABORT            "FoE abort - Slave  \"%s\": - EtherCAT address=%d - %s (0x%x)" */

            oDiagPara[0].pvAddress = pFoeErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pFoeErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = pFoeErr->achErrorString;
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            EC_T_DWORD dwErrorCode = EC_GET_FRM_DWORD(&pFoeErr->dwErrorCode);
			oDiagPara[3].pvAddress = (EC_T_VOID*)&dwErrorCode;
            oDiagPara[3].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_MBSLV_FOE_ABORT, 4, oDiagPara);

        } break;

    case EC_NOTIFY_MBXRCV_INVALID_DATA:
        {
            EC_T_DIAGMSGPARAMINFO oDiagPara[2];
            EC_T_MBXRCV_INVALID_DATA_DESC* pMbxRcvInvalidData = &(pNotificationData->ErrorNotification.desc.MbxRcvInvalidDataDesc);

            /* EC_SZTXT_TXT_MBXRCV_INVALID_DATA        "Invalid mailbox data received - Slave  \"%s\": - EtherCAT address=%d" */
            oDiagPara[0].pvAddress = pMbxRcvInvalidData->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pMbxRcvInvalidData->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_MBXRCV_INVALID_DATA, 2, oDiagPara);
        } break;

    case EC_NOTIFY_PDIWATCHDOG:
        {
            EC_T_DIAGMSGPARAMINFO           oDiagPara[2];
            EC_T_PDIWATCHDOG_DESC*          pErr = &(pNotificationData->ErrorNotification.desc.PdiWatchdogDesc);

            /* EC_SZTXT_TXT_PDIWATCHDOG                "PDI Watchdog expired - Slave  \"%s\": - EtherCAT address=%d" */
            oDiagPara[0].pvAddress = pErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

			EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pErr->SlaveProp.wStationAddress);
            oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_PDIWATCHDOG, 2, oDiagPara);
        } break;

    case EC_NOTIFY_SLAVE_NOTSUPPORTED:
        {
            EC_T_DIAGMSGPARAMINFO           oDiagPara[2];
            EC_T_SLAVE_NOTSUPPORTED_DESC*   pErr = &(pNotificationData->ErrorNotification.desc.SlaveNotSupportedDesc);

            /* EC_SZTXT_TXT_SLAVE_NOTSUPPORTED         "Slave not supported - Slave  \"%s\": - EtherCAT address=%d" */
            oDiagPara[0].pvAddress = pErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_SLAVE_NOTSUPPORTED, 2, oDiagPara);
        } break;

    case EC_NOTIFY_SLAVE_UNEXPECTED_STATE:
        {
            EC_T_DIAGMSGPARAMINFO           oDiagPara[4];
            EC_T_SLAVE_UNEXPECTED_STATE_DESC*   pErr = &(pNotificationData->ErrorNotification.desc.SlaveUnexpectedStateDesc);

            /* EC_SZTXT_TXT_SLAVE_UNEXPECTED_STATE     "Unexpected state - Slave  \"%s\": - EtherCAT address=%d - Current state=<%s> - Expected=<%s>" */
            oDiagPara[0].pvAddress = pErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            oDiagPara[2].pvAddress = (EC_T_VOID*)ecatDeviceStateText(pErr->curState);
            oDiagPara[2].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[2].pvAddress));

            oDiagPara[3].pvAddress = (EC_T_VOID*)ecatDeviceStateText(pErr->expState);
            oDiagPara[3].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[3].pvAddress));

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_SLAVE_UNEXPECTED_STATE, 4, oDiagPara);
        } break;

    case EC_NOTIFY_ALL_DEVICES_OPERATIONAL:
        {
            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_INFO, EC_TXT_ALL_DEVS_OP, 0, EC_NULL);
        } break;

    case EC_NOTIFY_EEPROM_CHECKSUM_ERROR:
        {
            EC_T_DIAGMSGPARAMINFO           oDiagPara[2];
            EC_T_EEPROM_CHECKSUM_ERROR_DESC*   pErr = &(pNotificationData->ErrorNotification.desc.EEPROMChecksumErrorDesc);

            /* EC_SZTXT_TXT_EEPROM_CHECKSUM_ERROR      "EEPROM checksum error - Slave  \"%s\": - EtherCAT address=%d" */
            oDiagPara[0].pvAddress = pErr->SlaveProp.achName;
            oDiagPara[0].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[0].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pErr->SlaveProp.wStationAddress);
			oDiagPara[1].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[1].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_EEPROM_CHECKSUM_ERROR, 2, oDiagPara);
        } break;
    case EC_NOTIFY_LINE_CROSSED:
        {
        } break;
    case EC_NOTIFY_JUNCTION_RED_CHANGE:
        {
            EC_T_DIAGMSGPARAMINFO           oDiagPara[3];
            EC_T_JUNCTION_RED_CHANGE_DESC*   pErr = &(pNotificationData->ErrorNotification.desc.JunctionRedChangeDesc);

            /* EC_SZTXT_TXT_JUNCTION_RED_CHANGE        "Junction redundancy change (Line break = %d) - Slave  \"%s\": - EtherCAT address=%d" */
            EC_T_DWORD bLineBreak = EC_GET_FRM_DWORD(&pErr->bLineBreak);
			oDiagPara[0].pvAddress = (EC_T_VOID*)&bLineBreak;
            oDiagPara[0].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED32);

            oDiagPara[1].pvAddress = pErr->SlaveProp.achName;
            oDiagPara[1].wParamFlags = (EC_T_WORD)((DIAG_MSG_PARAM_TYPE_ASCII<<DIAG_MSG_PARAM_TYPE_OFFSET) | (EC_T_WORD)OsStrlen(oDiagPara[1].pvAddress));

            EC_T_WORD wStationAddress = EC_GET_FRM_WORD(&pErr->SlaveProp.wStationAddress);
			oDiagPara[2].pvAddress = (EC_T_VOID*)&wStationAddress;
            oDiagPara[2].wParamFlags = ((DIAG_MSG_PARAM_TYPE_DATA<<DIAG_MSG_PARAM_TYPE_OFFSET)| DEFTYPE_UNSIGNED16);

            Diag_CreateNewMessage(dwCode, DIAG_MSG_TYPE_ERROR, EC_TXT_JUNCTION_RED_CHANGE, 3, oDiagPara);
        } break;
    default:
        {
            /* do nothing */
            goto Exit;
        }
    }

    /* PerfMeasEnd(&G_TscMeasDesc, EC_MSMT_AddHistoryNotificationCore); */

Exit:
    return;
}

EC_T_DWORD CEcSdoServ::ReplaceAllocatedString(  CEcSdoServ* pInst,
                                                EC_T_WORD   wIndex,
                                                EC_T_BYTE   bySubindex,
                                                EC_T_DWORD  dwSize,
                                                EC_T_WORD*  pwData,
                                                EC_T_BOOL   bCompleteAccess )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_DWORD      dwAllocSize = EC_MIN(dwSize, (EC_T_DWORD)OsStrlen((EC_T_CHAR*)pwData)) + 1;
    EC_T_CHAR*      szBuffer    = (EC_T_CHAR*)OsMalloc(dwAllocSize);
    EC_T_OBJECTDICT*pObjEntry   = pInst->GetObjectHandle(wIndex);
    struct _EC_T_MEMORY_LOGGER* pMLog   = pInst->GetMemLog();
    EC_UNREFPARM(bySubindex);
    EC_UNREFPARM(bCompleteAccess);
    OsDbgAssert(EC_NULL != pObjEntry);

    if (EC_NULL == szBuffer)
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_CORE_MASTER, pMLog, "pObjEntry->pVarPtr", szBuffer, dwAllocSize);

    OsStrncpy(szBuffer, (EC_T_BYTE*)pwData, dwAllocSize);
    szBuffer[dwAllocSize - 1] = '\0';
    
    /* Free old VarPtr if exists */
    pInst->FreeEntry0x1xxxVarPtr(pObjEntry);

    pObjEntry->pEntryDesc->wBitLength = (EC_T_WORD)((dwAllocSize - 1) * 8);
    pObjEntry->pVarPtr = szBuffer;

    dwRetVal = EC_E_NOERROR;
Exit:

    return dwRetVal;
}

EC_T_VOID CEcSdoServ::FreeEntry0x1xxxDesc(EC_T_OBJECTDICT* pObjEntry)
{
    if (EC_NULL != pObjEntry && EC_NULL != pObjEntry->pEntryDesc)
    {
        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "pObjEntry->pEntryDesc", pObjEntry->pEntryDesc, sizeof(EC_T_SDOINFOENTRYDESC));
        SafeOsFree(pObjEntry->pEntryDesc);
        pObjEntry->pEntryDesc = EC_NULL;
    }
}

EC_T_VOID CEcSdoServ::FreeEntry0x1xxxVarPtr(EC_T_OBJECTDICT* pObjEntry)
{
    if (EC_NULL != pObjEntry && EC_NULL != pObjEntry->pVarPtr && EC_NULL != pObjEntry->pEntryDesc)
    {
        EC_T_DWORD dwAllocSize = (pObjEntry->pEntryDesc->wBitLength / 8) + 1;

        EC_TRACE_SUBMEM(EC_MEMTRACE_CORE_MASTER, "pObjEntry->pEntryDesc", pObjEntry->pVarPtr, dwAllocSize);
        SafeOsFree(pObjEntry->pVarPtr);
        pObjEntry->pVarPtr = EC_NULL;
    }
}

EC_T_VOID CEcSdoServ::FreeEntries0x1XXX()
{
    FreeEntry0x1xxxVarPtr(m_pEntry0x1008);
    FreeEntry0x1xxxDesc(m_pEntry0x1008);

    FreeEntry0x1xxxVarPtr(m_pEntry0x1009);
    FreeEntry0x1xxxDesc(m_pEntry0x1009);

    FreeEntry0x1xxxVarPtr(m_pEntry0x100A);
    FreeEntry0x1xxxDesc(m_pEntry0x100A);
}

#endif /* INCLUDE_MASTER_OBD */

/*-END OF SOURCE FILE--------------------------------------------------------*/

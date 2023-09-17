/*-----------------------------------------------------------------------------
 * EcServices.h             header file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description
 *---------------------------------------------------------------------------*/

#ifndef INC_ECSERVICES
#define INC_ECSERVICES

#include "EcString.h"

/*-DEFINES/MACROS------------------------------------------------------------*/

#define ECAT_DEVICE_PHYSICSSIZE             8
#define EC_MAX_NUMOF_PREV_PORT              6               /* maximum number of previous port entries for hot connect */

/*-TYPEDEFS/ENUMS------------------------------------------------------------*/
typedef struct TEcMasterDesc
{
    EC_T_WORD           wMaxNumSlaves;
    
    /* MailboxStates */
    EC_T_WORD           wSizeAddressMBoxStates;
    EC_T_DWORD          dwLogAddressMBoxStates;

    /* EoE */
    EC_T_BOOL           bEoE;
    EC_T_DWORD          dwMaxPorts;
    EC_T_DWORD          dwMaxFrames;
    EC_T_DWORD          dwMaxMACs;

    /* Info */
    ETHERNET_ADDRESS    oMacDest;
    ETHERNET_ADDRESS    oMacSrc;
    EC_T_BYTE           abyEthType[2];

#if(defined INCLUDE_VARREAD)
    EC_T_DWORD          dwProcessVarInfoNumEntriesInp;  /* Number of INPUT process variable information entries*/
    EC_T_DWORD          dwProcessVarInfoNumEntriesOutp; /* Number of OUTPUT process variable information entries*/
    EC_T_DWORD          reserved2;
#else 
    EC_T_DWORD          reserved2[3];
#endif

    EC_T_DWORD          dwTimeLimit;
    /* InitCmds */
    EC_T_WORD           wNumEcatInitCmds;       
    EcInitCmdDesc**     apPkdEcatInitCmdDesc;           /* packed ecat init commands */

#if (defined INCLUDE_OEM)
    EC_T_DWORD          dwEncryptionVersion;
    EC_T_DWORD          dwManufacturerSignature;
    EC_T_DWORD          reserved3;
#else 
    EC_T_DWORD          reserved3[3];
#endif /* INCLUDE_OEM */

    EC_T_DWORD          reserved4[2];
} EcMasterDesc, *PEcMasterDesc;
#define SIZEOF_EcMasterDesc(p)          (sizeof(EcMasterDesc)+((PEcMasterDesc)p)->initcmdLen)

typedef struct TEcSlaveTopologyDesc
{
    EC_T_WORD           physAddr[4];            /* self, portA, portB, portC */
    EC_T_DWORD          portDelay[2];           /* portB, portC */
} EcSlaveTopologyDesc, *PEcSlaveTopologyDesc;

#define CHECKEEPROMENTRY_COUNT 5
typedef enum _EC_T_CHECKEEPROMENTRY
{
    eCheckEEPROMEntry_ANY     = 0,
    eCheckEEPROMEntry_EQUAL   = 1,
    eCheckEEPROMEntry_ABOVE   = 2, /* not supported (comment "check revision number (>=)") */   
    eCheckEEPROMEntry_LWEQUAL = 3,
    eCheckEEPROMEntry_HWABOVE = 4, /* not supported (several InitCmds, comments "check revision number (lo word)" and "check revision number (hi word >=)") */
    eCheckEEPROMEntry_HWEQUAL = 5,
    eCheckEEPROMEntry_LWABOVE = 6, /* not supported (several InitCmds, comments "check revision number (lo word >=)" and "check revision number (hi word)") */

    eCheckEEPROMEntry_BCppDummy = 0xffffffff    
} EC_T_CHECKEEPROMENTRY;

typedef struct
{
    EC_T_DWORD          dwVendorId;
    EC_T_DWORD          dwProductCode;
    EC_T_DWORD          dwRevisionNumber;
    EC_T_DWORD          dwSerialNumber;

    EC_T_WORD           wAutoIncAddr;
    EC_T_WORD           wPhysAddr;
    struct
    {
        EC_T_WORD       bMailbox                        : 1;
        EC_T_WORD       bMbxOutShortSend                : 1;
        EC_T_WORD       bCyclicPhysicalMBoxPolling      : 1;    /* set if per slave physical mailbox polling is active */
        EC_T_WORD       bLogicalStateMBoxPolling        : 1;    /* set if logical mailbox state polling is active */
        EC_T_WORD       bEoeSupport                     : 1;
        EC_T_WORD       bCoeSupport                     : 1;
        EC_T_WORD       bFoeSupport                     : 1;
        EC_T_WORD       bSoeSupport                     : 1;
        EC_T_WORD       bCyclicPhysicalMBoxBootPolling  : 1;    /* set if per slave physical mailbox polling is active in BootStrap */
        EC_T_WORD       bLogicalStateMBoxBootPolling    : 1;    /* set if logical mailbox state polling is active in BootStrap */
        EC_T_WORD       bBootStrapSupport               : 1;
        EC_T_WORD       bVoeSupport                     : 1;
        EC_T_WORD       bAoeSupport                     : 1;
        EC_T_WORD       bDc                             : 1;
        EC_T_WORD       reserved1                       : 2;
    } sParm;
    EC_T_CHAR           szName[ECAT_DEVICE_NAMESIZE]; 
    EC_T_CHAR           szPhysics[ECAT_DEVICE_PHYSICSSIZE];
    EC_T_WORD           wIdentificationValue;
    EC_T_WORD           wIdentificationAdo;
    EC_T_WORD           wMboxOutStart;
    EC_T_WORD           wMboxOutLen;
    EC_T_WORD           wMboxInStart;
    EC_T_WORD           wMboxInLen;
    EC_T_WORD           wSlaveLogicalAddressMBoxState;          /* bit offset in logical area for logical mailbox state polling */
    EC_T_WORD           wCyclePhysicalMBoxPollingTime;          /* cycle time for MBox polling */
    EC_T_WORD           wMbxBootOutStart;
    EC_T_WORD           wMbxBootOutLen;
    EC_T_WORD           wMbxBootInStart;
    EC_T_WORD           wMbxBootInLen;
    EC_T_WORD           wSlaveLogicalAddressMBoxBootState;       /* bit offset in logical area for logical mailbox state polling */
    EC_T_WORD           wCyclePhysicalMBoxBootPollingTime;      /* cycle time for MBox polling */

    EC_T_WORD           wPDInEntries;                      /* Slave Info Ex infos */
    EC_T_WORD           wPDOutEntries;
    EC_T_DWORD          adwPDInOffset[EC_CFG_SLAVE_PD_SECTIONS];
    EC_T_DWORD          adwPDInSize[EC_CFG_SLAVE_PD_SECTIONS];
    EC_T_DWORD          adwPDOutOffset[EC_CFG_SLAVE_PD_SECTIONS];
    EC_T_DWORD          adwPDOutSize[EC_CFG_SLAVE_PD_SECTIONS];

    EC_T_BOOL           bDcReferenceClock;
    EC_T_BOOL           bDcPotentialRefClock;
    EC_T_DWORD          dwDcRegisterCycleTime0;
    EC_T_DWORD          dwDcRegisterCycleTime1;

    EC_T_WORD           wNumPreviousPorts;                  /* PP is configured */
    EC_T_SLAVE_PORT_DESC    aoPreviousPort[EC_MAX_NUMOF_PREV_PORT];    /* Previous Port Settings */
#if (defined INCLUDE_VARREAD)
    EC_T_WORD           wNumProcessVarsOutp;                /* Number of OUTPUT process variables */
    EC_T_WORD           wNumProcessVarsInp;                 /* Number of INPUT  process variables */

    EC_T_WORD           wProcessVarsInpIndexFirst;
    EC_T_WORD           wProcessVarsInpIndexLast;
    
    EC_T_WORD           wProcessVarsOutIndexFirst;
    EC_T_WORD           wProcessVarsOutIndexLast;

#else
    EC_T_DWORD          reserved3;
#endif

    EC_T_BYTE           abyAoeNetId[6];                     /* slave AoE NetID */
    EC_T_CHECKEEPROMENTRY aeCheckEEPROMEntry[CHECKEEPROMENTRY_COUNT];

    EC_T_WORD                       wNumEcatInitCmds;   
    EcInitCmdDesc**                 apPkdEcatInitCmdDesc;   /* init commands descriptor */

    EC_T_WORD                       wNumCoeInitCmds;
    EcMailboxCmdDesc**              apPkdCoeInitCmdDesc;
    
    EC_T_WORD                       wNumEoeInitCmds;
    EcMailboxCmdDesc**              apPkdEoeInitCmdDesc;
#if (defined INCLUDE_AOE_SUPPORT)
    EC_T_WORD                       wNumAoeInitCmds;
    EcMailboxCmdDesc**              apPkdAoeInitCmdDesc;
#endif
#if (defined INCLUDE_SOE_SUPPORT)
    EC_T_WORD                       wNumSoeInitCmds;
    EcMailboxCmdDesc**              apPkdSoeInitCmdDesc;
#endif
#if (defined INCLUDE_HOTCONNECT)
    EC_T_WORD           wHCIdentifyAddrPrev;
#endif
} EC_T_ENI_SLAVE;

#define SIZEOF_EcMasterCreateSlave(p)           (sizeof(EcMasterCreateSlave)+((PEcMasterCreateSlave)p)->initcmdLen+((PEcMasterCreateSlave)p)->mboxCmdLen)

typedef struct TEthernetCreateSwitch
{
    EC_T_DWORD      dwMaxPorts;
    EC_T_DWORD      dwMaxFrames;
    EC_T_DWORD      dwMaxMACs;
    EC_T_BOOL       bSwitchToNIC;
} EthernetCreateSwitch, *PEthernetCreateSwitch;

#endif /* ECSERVICES_INC */


/*-END OF SOURCE FILE--------------------------------------------------------*/

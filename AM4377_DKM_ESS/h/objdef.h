/**
 * \addtogroup CoE CAN Application Profile over EtherCAT
 * @{
 */

/**
\file objdef.h
\author EthercatSSC@beckhoff.com
\brief CAN application profile over EtherCAT object dictionary

\version 5.11

<br>Changes to version V5.10:<br>
V5.11 COE2: update synctype supported flag for freerun<br>
V5.11 COE3: change 0x10F3.2 (Sync Error limit) from ESS_UINT32 to ESS_UINT16 (according to the ETG.1020)<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
<br>Changes to version V5.01:<br>
V5.10 COE1: Define one entry description for all 0x1C3x objects and change data type of SI11,12,13 to ESS_UINT16 (according ETG.1020)<br>
V5.10 COE2: Change return value of Get ObjectSize to ESS_UINT32<br>
            Change object size to ESS_UINT32<br>
V5.10 ECAT13: Update Synchronisation handling (FreeRun,SM Sync, Sync0, Sync1)<br>
              Compare DC UINT configuration (by ESC Config data) vs. DC activation register settings<br>
              Update 0x1C3x entries<br>
V5.10 ECAT8: Update Datatype defines according ETG.1020<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */

#ifndef _OBJDEF_H_
#define _OBJDEF_H_


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include <sdoserv.h>

int getpwm();
/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/*---------------------------------------------
-    DataTypes
-----------------------------------------------*/
#define     DEFTYPE_NULL                0x0000 /**< \brief Null*/
#define     DEFTYPE_BOOLEAN             0x0001 /**< \brief BOOLEAN*/
#define     DEFTYPE_INTEGER8            0x0002 /**< \brief INTEGER8*/
#define     DEFTYPE_INTEGER16           0x0003 /**< \brief INTEGER16*/
#define     DEFTYPE_INTEGER32           0x0004 /**< \brief INTEGER32*/
#define     DEFTYPE_UNSIGNED8           0x0005 /**< \brief UNSIGNED8*/
#define     DEFTYPE_UNSIGNED16          0x0006 /**< \brief UNSIGNED16*/
#define     DEFTYPE_UNSIGNED32          0x0007 /**< \brief UNSIGNED32*/
#define     DEFTYPE_REAL32              0x0008 /**< \brief REAL32*/
#define     DEFTYPE_VISIBLESTRING       0x0009 /**< \brief VISIBLE_STRING*/
#define     DEFTYPE_OCTETSTRING         0x000A /**< \brief OCTET_STRING*/
#define     DEFTYPE_UNICODE_STRING      0x000B /**< \brief UNICODE_STRING*/
#define     DEFTYPE_TIME_OF_DAY         0x000C /**< \brief TIME_OF_DAY*/
#define     DEFTYPE_TIME_DIFFERENCE     0x000D /**< \brief TIME_DIFFERENCE*/
#define     DEFTYPE_INTEGER24           0x0010 /**< \brief INTEGER24*/
#define     DEFTYPE_REAL64              0x0011 /**< \brief REAL64*/
#define     DEFTYPE_INTEGER40           0x0012 /**< \brief INTEGER40*/
#define     DEFTYPE_INTEGER48           0x0013 /**< \brief INTEGER48*/
#define     DEFTYPE_INTEGER56           0x0014 /**< \brief INTEGER56*/
#define     DEFTYPE_INTEGER64           0x0015 /**< \brief INTEGER64*/
#define     DEFTYPE_UNSIGNED24          0x0016 /**< \brief UNSIGNED24*/
#define     DEFTYPE_UNSIGNED40          0x0018 /**< \brief UNSIGNED40*/
#define     DEFTYPE_UNSIGNED48          0x0019 /**< \brief UNSIGNED48*/
#define     DEFTYPE_UNSIGNED56          0x001A /**< \brief UNSIGNED56*/
#define     DEFTYPE_UNSIGNED64          0x001B /**< \brief UNSIGNED64*/
#define     DEFTYPE_GUID                0x001D /**< \brief DEFTYPE_GUID*/
#define     DEFTYPE_BYTE                0x001E /**< \brief DEFTYPE_BYTE*/
#define     DEFTYPE_WORD                0x001F /**< \brief DEFTYPE_WORD*/
#define     DEFTYPE_DWORD               0x0020 /**< \brief DEFTYPE_DWORD*/
#define     DEFTYPE_PDOMAPPING          0x0021 /**< \brief PDO_MAPPING*/
#define     DEFTYPE_IDENTITY            0x0023 /**< \brief IDENTITY*/
#define     DEFTYPE_COMMAND             0x0025 /**< \brief COMMAND_PAR*/
#define     DEFTYPE_PDOCOMPAR           0x0027 /**< \brief PDO_PARAMETER*/
#define     DEFTYPE_ENUM                0x0028 /**< \brief DEFTYPE_ENUM */
#define     DEFTYPE_SMPAR               0x0029 /**< \brief SM_SYNCHRONISATION*/ 
#define     DEFTYPE_RECORD              0x002A /**< \brief DEFTYPE_RECORD */
#define     DEFTYPE_BACKUP              0x002B /**< \brief BACKUP_PARAMETER*/
#define     DEFTYPE_MDP                 0x002C /**< \brief MODULAR_DEVICE_PROFILE*/
#define     DEFTYPE_BITARR8             0x002D /**< \brief BITARR8*/
#define     DEFTYPE_BITARR16            0x002E /**< \brief BITARR16*/
#define     DEFTYPE_BITARR32            0x002F /**< \brief BITARR32*/
#define     DEFTYPE_BIT1                0x0030 /**< \brief BIT1*/
#define     DEFTYPE_BIT2                0x0031 /**< \brief BIT2*/
#define     DEFTYPE_BIT3                0x0032 /**< \brief BIT3*/
#define     DEFTYPE_BIT4                0x0033 /**< \brief BIT4*/
#define     DEFTYPE_BIT5                0x0034 /**< \brief BIT5*/
#define     DEFTYPE_BIT6                0x0035 /**< \brief BIT6*/
#define     DEFTYPE_BIT7                0x0036 /**< \brief BIT7*/
#define     DEFTYPE_BIT8                0x0037 /**< \brief BIT8*/
#define     DEFTYPE_ARRAY_OF_INT        0x0260 /**< \brief DEFTYPE_ARRAY_OF_INT*/
#define     DEFTYPE_ARRAY_OF_SINT       0x0261 /**< \brief DEFTYPE_ARRAY_OF_SINT*/
#define     DEFTYPE_ARRAY_OF_DINT       0x0262 /**< \brief DEFTYPE_ARRAY_OF_DINT*/
#define     DEFTYPE_ARRAY_OF_UDINT      0x0263 /**< \brief DEFTYPE_ARRAY_OF_UDINT*/
#define     DEFTYPE_ERRORHANDLING       0x0281 /**< \brief DEFTYPE_ERRORHANDLING*/
#define     DEFTYPE_DIAGHISTORY         0x0282 /**< \brief DEFTYPE_DIAGHISTORY*/
#define     DEFTYPE_SYNCSTATUS          0x0283 /**< \brief DEFTYPE_SYNCSTATUS*/
#define     DEFTYPE_SYNCSETTINGS        0x0284 /**< \brief DEFTYPE_SYNCSETTINGS*/
#define     DEFTYPE_FSOEFRAME           0x0285 /**< \brief DEFTYPE_FSOEFRAME*/
#define     DEFTYPE_FSOECOMMPAR         0x0286 /**< \brief DEFTYPE_FSOECOMMPAR*/


/*---------------------------------------------
-    Flags for supported Synctypes (0x1C32.4 / 0x1C33.4)
-----------------------------------------------*/

/*ECATCHANGE_START(V5.11) COE2*/
#define     SYNCTYPE_FREERUNSUPP        0x0001 /**< \brief FreeRun supported*/
/*ECATCHANGE_END(V5.11) COE2*/
#define     SYNCTYPE_SYNCHRONSUPP       0x0002 /**< \brief SyncManager synchron supported*/
#define     SYNCTYPE_DCSYNC0SUPP        0x0004 /**< \brief Sync0 synchron supported*/
#define     SYNCTYPE_DCSYNC1SUPP        0x0008 /**< \brief Sync1 synchron supported*/
#define     SYNCTYPE_SUBCYCLESUPP       0x0010 /**< \brief Subordinated Application with fixed Sync0 supported*/
#define     SYNCTYPE_LOCALSHIFTSUPP     0x0020 /**< \brief Event shift with local timer supported. Output shift : 0x1C32.4 ; Input shift : 0x1C33.4 (not supported by the default SSC)*/
#define     SYNCTYPE_SHIFTBYSYNC1SUPP   0x0040 /**< \brief Event shift with Sync1 event supported. Output shift : 0x1C32.4 ; Input shift : 0x1C33.4*/
#define     SYNCTYPE_MEASURE_DELAYSUPP  0x0200 /**< \brief Delay times should be measured (not supported by the default SSC, only the cycle time can be measured (1ms resolution)*/
#define     SYNCTYPE_FIXED_DELAYSUPP    0x0400 /**< \brief Delay times are fixed*/
#define     SYNCTYPE_TIMESVARIABLE      0x4000 /**< \brief Dynamic Cycle Times supported*/


#define     IS_PDO_ASSIGN(x)            ((x >= 0x1C10) && (x <= 0x1C2F)) /**< \brief Marco to check if object index is SyncManager assign object*/
#define     IS_RX_PDO(x)                (((x) >= 0x1600) && ((x) <= 0x17FF)) /**< \brief Marco to check if object index RxPDO mapping object*/
#define     IS_TX_PDO(x)                (((x) >= 0x1A00) && ((x) <= 0x1BFF)) /**< \brief Marco to check if object index TxPDO mapping object*/


/**
 * \brief Object dictionary entry structure
 */
typedef struct OBJ_ENTRY
{
#if !STATIC_OBJECT_DIC
    struct OBJ_ENTRY                      *pPrev; /**< \brief Previous entry(object) in the object dictionary list*/
    struct OBJ_ENTRY                      *pNext; /**< \brief Next entry(object) in the object dictionary list*/
#endif //#if !STATIC_OBJECT_DIC

    ESS_UINT16                                Index; /**< \brief Object index*/
    TSDOINFOOBJDESC                       ObjDesc; /**< \brief Object access, type and code*/
    OBJCONST TSDOINFOENTRYDESC OBJMEM     *pEntryDesc; /**< \brief pointer to object entry descriptions*/
    OBJCONST ESS_UCHAR OBJMEM                 *pName; /**< \brief Pointer to object and entry names*/
    void MBXMEM                           *pVarPtr; /**< \brief Pointer to object buffer*/
    ESS_UINT8 (* Read)( ESS_UINT16 Index, ESS_UINT8 Subindex, ESS_UINT32 Size, ESS_UINT16 MBXMEM * pData, ESS_UINT8 bCompleteAccess ); /**< \brief Function pointer to read function (if NULL default read function will be used)*/
    ESS_UINT8 (* Write)( ESS_UINT16 Index, ESS_UINT8 Subindex, ESS_UINT32 Size, ESS_UINT16 MBXMEM * pData, ESS_UINT8 bCompleteAccess ); /**< \brief Function pointer to write function (if NULL default write function will be used)*/
    ESS_UINT16                                 NonVolatileOffset; /**< \brief Offset within the non volatile memory (need to be defined for backup objects)*/
}
TOBJECT;


/**
 * Object 0x1C3x (SyncManager Parameter) data structure
 */
typedef struct OBJ_STRUCT_PACKED_START
{
    ESS_UINT16    subindex0;/**< \brief SubIndex0*/
    ESS_UINT16    u16SyncType; /**< \brief SunbIndex 001: Sync type*/
    ESS_UINT32    u32CycleTime;/**< \brief SunbIndex 002: Cycle time*/
    ESS_UINT32    u32ShiftTime;/**< \brief SunbIndex 003: Shift time (not supported, variable is only used as a place holder)*/
    ESS_UINT16    u16SyncTypesSupported;/**< \brief SunbIndex 004: Supported Sync types*/
    ESS_UINT32    u32MinCycleTime;/**< \brief SunbIndex 005: Min cycle time*/
    ESS_UINT32    u32CalcAndCopyTime;/**< \brief SunbIndex 006: Calc and Copy time*/
    ESS_UINT32    u32Si7Reserved;/**< \brief SunbIndex 007: 32Bit Reserved*/
    ESS_UINT16    u16GetCycleTime;/**< \brief SunbIndex 008: Get cycle time*/
    ESS_UINT32    u32DelayTime; /**< \brief SunbIndex 009: Delay time*/
    ESS_UINT32    u32Sync0CycleTime; /**< \brief SunbIndex 010: Sync0 cycle time*/
    ESS_UINT16    u16SmEventMissedCounter; /**< \brief SunbIndex 011: SyncManager event missed counter*/
    ESS_UINT16    u16CycleExceededCounter; /**< \brief SunbIndex 012: Cycle exceed counter*/
    ESS_UINT16    u16Si13Reserved; /**< \brief SunbIndex 013: Shift too short (not supported, only padding)*/
    ESS_UINT16    u16Si14Reserved; /**< \brief SubIndex14 not supported*/
    ESS_UINT32    u32Si15Reserved; /**< \brief SubIndex15 not supported*/
    ESS_UINT32    u32Si16Reserved; /**< \brief SubIndex16 not supported*/
    ESS_UINT32    u32Si17Reserved; /**< \brief SubIndex17 not supported*/
    ESS_UINT32    u32Si18Reserved; /**< \brief SubIndex18 not supported*/
    ESS_UINT8    u8SyncError; /**< \brief Sync Error*/
}OBJ_STRUCT_PACKED_END
TSYNCMANPAR;

/**
 * Cycle diagnosis
 */
typedef struct OBJ_STRUCT_PACKED_START
{
    ESS_UINT16    syncFailedCounter; /**< \brief Sync Failed Counter*/
}OBJ_STRUCT_PACKED_END
TCYCLEDIAG;


#if !_PIC18
/**
 * Object 0x10F1 (Error Settings Object) data structure
 */
typedef struct OBJ_STRUCT_PACKED_START {
   ESS_UINT16   u16SubIndex0; /**< \brief SubIndex0*/
   ESS_UINT32   u32LocalErrorReaction; /**< \brief Local error reaction*/
/*ECATCHANGE_START(V5.11) COE3*/
   ESS_UINT16   u16SyncErrorCounterLimit; /**< \brief Sync error counter limit*/
/*ECATCHANGE_END(V5.11) COE3*/
} OBJ_STRUCT_PACKED_END
TOBJ10F1;
#endif

#endif //_OBJDEF_H_


/*-----------------------------------------------------------------------------------------
------
------    Global variables
------
-----------------------------------------------------------------------------------------*/
/* ECATCHANGE_START(V5.11) ECAT10*/
#if defined(_OBJDEF_) && (_OBJDEF_ == 1)
/* ECATCHANGE_END(V5.11) ECAT10*/
    #define PROTO
#else
    #define PROTO extern
#endif

/**
 * Indicates if the synchronisation was written by the user on startup (by writing 0x1C3x.1).
 * If it was not written the sync mode will be set based on the DC registers 0x980:0x981
 */
PROTO ESS_BOOL bSyncSetByUser;

/**
  * \brief Cycle diagnosis variable
  */
PROTO TCYCLEDIAG sCycleDiag;


#if MAX_PD_OUTPUT_SIZE > 0
/**
  * \brief Object 0x1C32 (SyncManager 2 Parameter) object variable
  */
PROTO TSYNCMANPAR MBXMEM sSyncManOutPar;
#endif


#if MAX_PD_INPUT_SIZE > 0
/**
  * \brief Object 0x1C33 (SyncManager 3 Parameter) object variable
  */
PROTO TSYNCMANPAR MBXMEM sSyncManInPar;
#endif


#if !_PIC18
/**
 * \brief Object 0x10F1 (Error Settings) object variable
 *
 * SubIndex0 : 2
 * Local error reaction : 1
 * Sync error counter limit : MAX_SM_EVENT_MISSED
 */
PROTO TOBJ10F1 sErrorSettings
/* ECATCHANGE_START(V5.11) ECAT10*/
#if defined(_OBJDEF_) && (_OBJDEF_ == 1)
/* ECATCHANGE_END(V5.11) ECAT10*/
= {2, 0x01, MAX_SM_EVENT_MISSED}
#endif
;
#endif


/**
 * \brief Default entry name "SubIndex 000"
 */
PROTO    ESS_CHAR OBJMEM         aSubindexDesc[13]
/* ECATCHANGE_START(V5.11) ECAT10*/
#if defined(_OBJDEF_) && (_OBJDEF_ == 1)
/* ECATCHANGE_END(V5.11) ECAT10*/
 = { "SubIndex 000" }
#endif
;


/*-----------------------------------------------------------------------------------------
------
------    Global functions
------
-----------------------------------------------------------------------------------------*/

PROTO    OBJCONST TOBJECT OBJMEM *  OBJ_GetObjectHandle( ESS_UINT16 index );
PROTO    ESS_UINT32  OBJ_GetObjectLength( ESS_UINT16 index, ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT8 bCompleteAccess);
PROTO    ESS_UINT16  OBJ_GetNoOfObjects(ESS_UINT8 listType);
PROTO    ESS_UINT16  OBJ_GetObjectList(ESS_UINT16 listType, ESS_UINT16 *pIndex, ESS_UINT16 size, ESS_UINT16 MBXMEM *pData,ESS_UINT8 *pAbort);
PROTO    ESS_UINT16  OBJ_GetDesc( ESS_UINT16 index, ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT16 MBXMEM * pData );
PROTO    OBJCONST TSDOINFOENTRYDESC OBJMEM * OBJ_GetEntryDesc(OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT8 Subindex);
PROTO    OBJCONST TSDOINFOOBJDESC OBJMEM * OBJ_GetObjDesc(OBJCONST TOBJECT OBJMEM * pObjEntry);
PROTO    ESS_UINT16  OBJ_GetEntryOffset(ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);
PROTO    ESS_UINT8   CheckSyncTypeValue(ESS_UINT16 index, ESS_UINT16 NewSyncType);
PROTO    ESS_UINT8   OBJ_Read(ESS_UINT16 index, ESS_UINT8 subindex, ESS_UINT32 objSize, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT16 MBXMEM * pData, ESS_UINT8 bCompleteAccess);
PROTO    ESS_UINT8   OBJ_Write(ESS_UINT16 index, ESS_UINT8 subindex, ESS_UINT32 dataSize, OBJCONST TOBJECT OBJMEM * pObjEntry, ESS_UINT16 MBXMEM * pData, ESS_UINT8 bCompleteAccess);
PROTO    void    COE_WriteBackupEntry(ESS_UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);

#undef PROTO
/** @}*/

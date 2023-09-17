/* rcm_snmp.h */
/* Copyright (c) 2004-2005, 2010,2012 Wind River Systems, Inc.
*
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01c,12mar10,lan  VxWorks 64-bit support.(WIND00193649)
01b,12oct05,wdz  '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history
*/

/*----------------------------------------------------------------------
 *
 * NAME CHANGE NOTICE:
 *
 * On May 11th, 1999, Rapid Logic changed its corporate naming scheme.
 * The changes are as follows:
 *
 *      OLD NAME                        NEW NAME
 *
 *      OpenControl                     RapidControl
 *      WebControl                      RapidControl for Web
 *      JavaControl                     RapidControl for Applets
 *      MIBway                          MIBway for RapidControl
 *
 *      OpenControl Backplane (OCB)     RapidControl Backplane (RCB)
 *      OpenControl Protocol (OCP)      RapidControl Protocol (RCP)
 *      MagicMarkup                     RapidMark
 *
 * The source code portion of our product family -- of which this file
 * is a member -- will fully reflect this new naming scheme in an upcoming
 * release.
 *
 *
 * RapidControl, RapidControl for Web, RapidControl Backplane,
 * RapidControl for Applets, MIBway, RapidControl Protocol, and
 * RapidMark are trademarks of Rapid Logic, Inc.  All rights reserved.
 *
 */
/*
$History: rcm_snmp.h $
 *
 * *****************  Version 26  *****************
 * User: Pstuart      Date: 3/29/01    Time: 11:43a
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * added protos for community string access
 *
 * *****************  Version 25  *****************
 * User: Pstuart      Date: 3/08/01    Time: 2:56p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * moved SNMP_DATA_GET_M def from .c to .h
 *
 * *****************  Version 24  *****************
 * User: Pstuart      Date: 3/07/01    Time: 9:20p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * move macro from rcm_envoy.h to rcm_snmp.h
 *
 * *****************  Version 23  *****************
 * User: Pstuart      Date: 2/26/01    Time: 11:10a
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * use new envoy macro
 *
 * *****************  Version 22  *****************
 * User: James        Date: 12/21/00   Time: 2:34p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added support for multi-host. JAB
 *
 * *****************  Version 21  *****************
 * User: James        Date: 12/18/00   Time: 9:00p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Cleaned up the CMU code, and fixed non-reentrancy issues. JAB
 *
 * *****************  Version 20  *****************
 * User: James        Date: 7/31/00    Time: 11:52a
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added additional community string support.
 *
 * *****************  Version 19  *****************
 * User: Pstuart      Date: 7/06/00    Time: 3:09p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added protos for: OCSNMP_GetCommunityString, OCSNMP_SetCommunityString,
 * OCSNMP_Commit
 *
 * *****************  Version 18  *****************
 * User: James        Date: 6/20/00    Time: 2:28p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Cleaned up banner info.
 *
 * *****************  Version 17  *****************
 * User: Leech        Date: 6/16/00    Time: 1:58p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added flags to change prototype of OCSNMP_DestructSnmpEnviron depending
 * on available SNMP agent (Epilogue uses new version, SRI uses old)
 *
 * User: James        Date: 6/05/00    Time: 1:58p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added get entire table row support. JAB
 *
 * User: Leech        Date: 5/24/00    Time: 6:00p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added OCSNMP_DestructSnmpEnviron() prototype
 *
 * User: James        Date: 5/18/00    Time: 5:48p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added support for dynamic MIB lookup tables. JAB
 *
 * User: Epeterson    Date: 4/25/00    Time: 2:25p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Include history and enable auto archiving feature from VSS


*/
#ifndef __OCSNMP_HEADER__
#define __OCSNMP_HEADER__

#include "rcm_envoy.h"
#include "rcm_ev_cnv.h"
#include "wmbstub/wmmwmbErrors.h"
#ifdef __cplusplus 
#include "wmLinkListLib.h"
#endif /* __cplusplus */

#ifdef __cplusplus 
extern "C" {
#endif /* __cplusplus */

#ifdef __MIBWAY_GET_BULK_ROW__
#define GET_BULK_ROW_RETRY_COUNT    3
#endif

/* generic snmp interface constants */
#define kMaxMibLength           64
#define kMaxExpResultLen        255
#define kMaxQueryStringLen      128
#define kMaxSnmpInstance        128
#define kMaxStringInteger       10
#define kMaxCommString          128

/* outer expression delims              ex. {[aaa.[bbb].[ccc.[ddd]]]} */
#define kInlineStartDelim       '{'
#define kInlineEndDelim         '}'

/* inner expression delims */
#define kInlineExpStartDelim    '['
#define kInlineExpEndDelim      ']'

/* for filter support */
#define kMaxInstanceLen         128

#define kFilterMatchUnknown     0
#define kFilterMatchInclude     1
#define kFilterMatchExclude     2
#define kFilterMatchIgnore      3

#define kInstanceWidthFixed     0
#define kInstanceWidthVariable  1

/* map WebControl to SNMPs datatype type identities */
#define kU32DataType            kDTuinteger
#define kS32DataType            kDTinteger

#ifdef __RLI_SNMP_DEBUG_MESSAGES__
#  define SNMP_DEBUG_MSG_M(x)        wmLogPrintf(x)
#  define SNMP_DEBUG_MSG1_M(x,y)     wmLogPrintf(x,y)
#else
#  define SNMP_DEBUG_MSG_M(x)
#  define SNMP_DEBUG_MSG1_M(x,y)
#endif

#ifndef __MACRO_REPEAT_NEST_FULL__
#  define SNMP_DATA_GET_M(x)    x->pSnmpData
#else
#  define SNMP_DATA_GET_M(x)    x->p_MacroSnmpData[x->Macro_NestDepth]
#endif  /* __MACRO_REPEAT_NEST_FULL__ */

/*-----------------------------------------------------------------------*/

#ifndef __RLI_OID_DEFINED__
#define __RLI_OID_DEFINED__

typedef struct
{
    sbyte4* pObjectId;          /* an array of OID components */
    Length  ObjectLen;          /* size of the above array */

} RLI_OID;

#endif



/*-----------------------------------------------------------------------*/

typedef struct
{
    sbyte   ColumnName[kMaxMibLength];

#ifdef __MIBWAY_GET_BULK_ROW__
    void*   pMibObjValueDescr;                  /* pointer into OIDTracker::pMibObjValueDescrArray */

#ifdef ENVOY_STACK_K
    void*       pEnvoyOID;
#endif

#if (defined(__GENERIC_SNMP_V1__) || defined(__CMU_SNMP_V1__))
    RLI_OID*    pCmuOID;
#endif

#endif /* __MIBWAY_GET_BULK_ROW__ */

} ColumnDescriptorStruct;



/*-----------------------------------------------------------------------*/

#if (defined(__GENERIC_SNMP_V1__) || defined(__CMU_SNMP_V1__))
struct MibObjValueDescr;
#endif

typedef struct
    {

    /* table data */

    sbyte   MibTableName[kMaxMibLength+1];      /* name of the MIB table */
    RLI_OID*   pTableOid;                          /* OID for the MIB table */
    BOOL boolMoreData;                       /* end of table not reached? */
    BOOL boolFilterData;                     /* do we want to filter table? */

    /* current instance data */

    sbyte   InstanceString[kMaxMibLength+1];    /* instance string */
    sbyte4  InstanceLength;                     /* length of the instance */
    sbyte4  InstanceWidthType;                  /* fixed or variable length instance */

#if (defined(__GENERIC_SNMP_V1__) || defined(__CMU_SNMP_V1__) || defined(ENVOY_STACK_K))
    MibObjValueDescr*   pCurrentVarBindRow;                 /* keeps track of the current row */
#endif
    int     NumTimesIndexed;                    /* useful for UI display */

    /* columns */

    WM_LIST_T *   p_lstMibColumnNames;                /* list of columns attached to this table */

    /* filter support */

    ubyte4  filterLowRange[kMaxInstanceLen];
    ubyte4  filterHighRange[kMaxInstanceLen];
    sbyte4  filterMatchType;                    /* include/exclude the filter rule */
    Length  numInstanceComponents;

#ifdef __MIBWAY_GET_BULK_ROW__
    MibObjValueDescr*   pMibObjValueDescrArray; /* performance optimization, holds an entire row of data */
#endif

    } OIDTracker;       /* used for keeping track of a indexable table !!!!!!!!! rename it!!!*/



/*-----------------------------------------------------------------------*/

#define kMibwayMaxHostNameLength 32

typedef struct RLI_CustomSnmpEnviron
    {
    void*   p_lstOidTrackers;
    void*   p_envVar;
    sbyte   CommunityString[kMaxCommString];
    sbyte   CommunityStringSet[kMaxCommString];
    BOOL IsPost;
    void*   pSetList;
    void*   pIntSetSnmpPkt;

#ifdef __RLI_DYNAMIC_MIB_LOOKUP_SUPPORT__
    void*   pMibLookupList;
#endif

#if (defined(__GENERIC_SNMP_V1__) || defined(__CMU_SNMP_V1__))
    void*   pCmuSessions;

    char    defaultHost[kMibwayMaxHostNameLength];
    int     snmpPort;
#endif /* #if (defined(__GENERIC_SNMP_V1__) || defined(__CMU_SNMP_V1__)) */

    } RLI_CustomSnmpEnviron;    /* interface to a SNMP */

/*-----------------------------------------------------------------------*/

typedef struct
    {
    void*   (*pFunc)(WMB_SESSION_T_WMM *, short, sbyte *, void *);
    short   DataType;
    void*   pSetOid;
    sbyte*  pString;
    ssize_t  length;
    BOOL native;
    } setElement;


/*-----------------------------------------------------------------------*/

typedef struct
    {
    sbyte4 error_index;
    sbyte4 error_status;
    sbyte  error_text[64];
    } snmpStatus;

/*-----------------------------------------------------------------------*/

/* prototypes */

void OCSNMP_ParseInlineExpression(WMB_SESSION_T_WMM *p_envVar, sbyte *p_inlineExpString,
                             sbyte *pOutputString, Length OutputLen);

RLI_OID *OCSNMP_CreateRLI_Oid();

RLI_OID *RLI_OidFromEnglish(WMB_SESSION_T_WMM *p_envVar, sbyte *pEnglishMibName);

MibObjValueDescr *RLI_GetExactSnmpObject(WMB_SESSION_T_WMM *p_envVar, RLI_OID *pRliOid);

STATUS RLI_SnmpCommitSet(WMB_SESSION_T_WMM *p_envVar, sbyte4 listIndex);

MibObjValueDescr *RLI_GetNextSnmpObject(WMB_SESSION_T_WMM *p_envVar, RLI_OID *pRliOid, RLI_OID *pBaseTableOid);

void RLI_FreeMibObjValueDescr(MibObjValueDescr *pMibObjValueDescr);

STATUS RLI_ConstructSnmpEnviron(WMB_SESSION_T_WMM *p_envVar);

int RLI_SetMibObject(WMB_SESSION_T_WMM *p_envVar, DATATYPE_T wmType,
                     void *pHtmlInputBuf,
                     void *pFullMibName, void *pBaseMibName,
                     ConvertHtmlToBin p_funcConvertHtmlToBin);

int RLI_MibObjectType(void *pFullMibName);

void OCSNMP_FreeRLI_Oid(RLI_OID *pRLI_OID);

void DestructAllTableObjects(void **pp_lstTableDescriptors);

BOOL OCSNMP_IsOffspring(RLI_OID *pParent, RLI_OID *pChild);

void OCSNMP_SetCommunityString(WMB_SESSION_T_WMM *p_envVar, sbyte *pCommString);

STATUS OCSNMP_Commit(WMB_SESSION_T_WMM *p_envVar, sbyte4 stage, snmpStatus** pSnmpStatus);

void OCSNMP_AssignGetCommunityString(WMB_SESSION_T_WMM *p_envVar, sbyte *pCommString);

void OCSNMP_AssignSetCommunityString(WMB_SESSION_T_WMM *p_envVar, sbyte *pCommString);


#ifdef __MIBWAY_GET_BULK_ROW__
void RLI_FreeMibObjValueDescrArray(OIDTracker *pTableDescriptor);
STATUS RLI_GetTableRow(WMB_SESSION_T_WMM *p_envVar, RLI_CustomSnmpEnviron *pSnmpData, OIDTracker *pTableDescriptor);
#endif

#ifdef ENVOY_STACK_K
STATUS OCSNMP_DestructSnmpEnviron(WMB_SESSION_T_WMM *p_envVar, snmpStatus *pSnmpStatus);
#else
void OCSNMP_DestructSnmpEnviron(WMB_SESSION_T_WMM *p_envVar);
#endif
#ifdef __cplusplus
}
#endif /* __cpluysplus */
#endif /* __OCSNMP_HEADER__ */



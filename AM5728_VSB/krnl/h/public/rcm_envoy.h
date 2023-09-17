/* rcm_envoy.h  -  (OpenControl to Epilogue/ISI Envoy Interface Code) */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,12mar10,lan  VxWorks 64-bit support.(WIND00193649)
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
$History: rcm_envoy.h $
 *
 * *****************  Version 18  *****************
 * User: Pstuart      Date: 3/07/01    Time: 9:20p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * move macro from rcm_envoy.h to rcm_snmp.h
 *
 * *****************  Version 17  *****************
 * User: Pstuart      Date: 2/26/01    Time: 11:09a
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * add macros
 *
 * *****************  Version 16  *****************
 * User: James        Date: 10/26/00   Time: 5:15p
 * Updated in $/Rapid Logic/Mediation/rli_code/mibway/include
 * Added a header file, if SNMPv2 is enabled.
 *
 * *****************  Version 15  *****************
 * User: Pstuart      Date: 8/21/00    Time: 4:02p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added __64_BIT_COUNTERS__
 *
 * *****************  Version 14  *****************
 * User: James        Date: 7/31/00    Time: 11:52a
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added additional community string support.
 *
 * *****************  Version 13  *****************
 * User: James        Date: 6/20/00    Time: 2:28p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Cleaned up banner info.
 *
 * *****************  Version 12  *****************
 * User: James        Date: 6/05/00    Time: 1:58p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added get entire table row support.
 *
 * User: James        Date: 4/26/00    Time: 12:17p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Added support for AgentX.
 *
 * User: Epeterson    Date: 4/25/00    Time: 2:25p
 * Updated in $/Rapid Logic/Code Line/rli_code/mibway/include
 * Include history and enable auto archiving feature from VSS


*/
#ifndef __OC_ENVOY_HEADER__
#define __OC_ENVOY_HEADER__


/*!!!!!!!!!!!!!!!!!! stab at abstraction layer code */
#define RLI_CreateOID               EPILOGUE_CreateOID
#define RLI_OidFromEnglish          EPILOGUE_OidFromEnglish
#define RLI_DestroyOID              EPILOGUE_DestroyOID

#define RLI_GetExactSnmpObject      EPILOGUE_GetExactSnmpObject
#define RLI_GetNextSnmpObject       EPILOGUE_GetNextSnmpObject
#define RLI_FreeMibObjValueDescr    EPILOGUE_FreeMibObjValueDescr

#define RLI_GetOidDigitFromLeft     OCSNMP_GetOidDigitFromLeft
#define RLI_GetOidDigitFromRight    OCSNMP_GetOidDigitFromRight
#define RLI_GetOidLength            OCSNMP_GetOidLength

#define RLI_MibObjectType           EPILOGUE_MibObjectType
#define RLI_SetMibObject            EPILOGUE_SetMibObject

#define RLI_CloneMibObjValueDescr   EPILOGUE_CloneMibObjValueDescr

#define RLI_GetBinToHtmlMethod      OCEV_CNV_GetBinToHtmlMethod
#define RLI_GetHtmlToBinMethod      OCEV_CNV_GetHtmlToBinMethod

#define RLI_ConstructSnmpEnviron    EPILOGUE_ConstructSnmpEnviron

#ifdef __MIBWAY_GET_BULK_ROW__
#define RLI_FreeMibObjValueDescrArray   ENVOY_FreeMibObjValueDescrArray
#define RLI_GetTableRow                 ENVOY_GetTableRow
#endif

#ifdef __RLI_ENABLE_V3__
#  define SNMP_VERSION_K    SNMP_VERSION_3
#endif

#ifdef __RLI_ENABLE_V2__
#  define SNMP_VERSION_K    SNMP_VERSION_2
#endif

#ifndef SNMP_VERSION_K
#  define SNMP_VERSION_K    SNMP_VERSION_1
#endif

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

#if ( defined(INSTALL_ENVOY_SNMP_VERSION_2)    || \
      defined(INSTALL_ENVOY_SNMP_USE_V2_TYPES) || \
      defined(__RLI_ENABLE_V2__) )

#define __64_BIT_COUNTERS__
#include <wrn/wm/snmp/engine/ui64.h>

#endif

typedef struct
{
    RLI_OID*     OidForThisObject;
    sbyte4       DataType;           /* have it match our internal datatype */
    ssize_t       stringLength;

    union
    {
        /* for maintence reasons: use Envoy's internal types. JAB */
        sbyte4          v_number;
        ubyte4          v_counter;
        sbyte*          v_string;
        RLI_OID*        v_object;
        unsigned char   v_network_address[4];
#ifdef __64_BIT_COUNTERS__
        UINT_64_T       v_counter64;
#endif

    } value_u;

#ifdef __WM_MASTER_SUBAGENT_SNMP_STACK__
    sbyte4 snmpMutexIndex;
#endif

#ifdef __MIBWAY_GET_BULK_ROW__
    BOOL      arrayMember;
#endif
    /* EJA */
    INT32 errorIndex;      /* error index */
    INT32 sessionID;       /* unique session ID */
    void* errorCallback;   /* Callback used to set error status */
    void* messageCallback; /* Callback used to parse message */
} MibObjValueDescr;



/*-----------------------------------------------------------------------*/

/* prototypes */

#endif




/* wmmErrorLog.h - Contains public error store parameters */

/* Copyright 2002 Wind River Systems, Inc. */

/*
modification history
--------------------
01e 09jul10,m_z  remove compile warning
01d,28oct08,y_z  remove MAX_OID_LEN redefined warning
01c,28mar06,ps  add ifdefs around vxWorks includes for linux build
01b,15sep03,jws  Add snmpdefs path info
01a,22mar02,eja  Created.
*/

/*
DESCRIPTION

SEE ALSO
wmmErrorLogP.h
*/

#ifndef INCwmmErrorLogh
#define INCwmmErrorLogh

#define __WEBCLI_OSCONF_NEED_RINGLIB__
#include <webcli_osconf.h>

#include "wrn/wm/snmp/engine/snmpdefs.h"


#define MAX_OPEN_LOGS  (10)          /* Max number of open logs */
#define IGNORE_ERR_CODE (0x55555555) /* Flag to ignore error code */
#ifndef MAX_OID_LEN
#define MAX_OID_LEN     (25)         /* The maximum length of the oid */
#endif /*MAX_OID_LEN*/
#define MAX_VAR_BINDS   (10)         /* Max number of varbinds */
#define NMSGS           (50)         /* Max number of messages in ring buf */

typedef struct wmm_obj_id_t
    {
    /* # of object id components    */
    /* Note: this is the number of  */
    /* components in the object id, */
    /* which is one more than the # */
    /* of subidentifiers in an      */
    /* encoded object id.           */
    INT32    numComponents;
    OIDC_T   componentList [MAX_OID_LEN];
    } WMM_OBJ_ID_T, *PWMM_OBJ_ID_T;

typedef struct wmm_simple_vb_t
    {
    WMM_OBJ_ID_T vbOid;        /* Oid of varibind that had exception */
    INT32        vbException;  /* Type of exception */
    INT8*        vbExcDesc;    /* Exception description */
    INT32        vbIndex;      /* location of varbind in list */
    } WMM_SVB_T, *PWMM_SVB_T;

typedef struct wmm_snmp_error_t
    {
    WMM_SVB_T vbList [MAX_VAR_BINDS]; /* List of varbind exceptions */
    INT32     sessionID;              /* session ID */
    INT32     pduType;                /* Protocol data unit type */
    INT32     errorCode;              /* error code used by error handler - ie envoy internal error*/
    INT32     errorIndex;             /* Instance of obj that error occured */
    INT8*     errorDesc;              /* error description - string describing snmp error status */
    INT32     errorStatus;            /* type of error that occured - ie snmp error code */
    INT32     numExceptions;          /* Number of exceptions that occured */
    INT32     genericTrap;            /* Trap type (analogous to errorStatus) */
    } WMM_SNMP_ERROR_T, *PWMM_SNMP_ERROR_T;

/* Public methods */


/* starts logging SNMP packets from a given session */
STATUS
    wmmErrorLogStart
    (
    INT32 sessionID  /* Unique ID used to attach to log buffer */
    );

/* ends logging SNMP packets from a given session */
STATUS
    wmmErrorLogEnd
    (
    INT32 sessionID     /* Unique session Id used to access message buffer */
    );

/* returns the number of lost error messages (overwritten) */
#ifndef _WRS_CONFIG_LP64
INT32
#else
INT64
#endif
    wmmErrorLostMsgsGet
    (
    INT32     /* Unique session Id used to access message buffer */
    );

/* returns the number of unread snmp response messages */
#ifndef _WRS_CONFIG_LP64
IMPORT INT32
#else
IMPORT INT64
#endif
    wmmErrorNumMsgsGet
    (
    INT32   /* Unique snmp Id used to access message buffer */
    );

/* deletes all messages from a log buffer */
STATUS
    wmmErrorMsgPurge
    (
    INT32   /* Unique Id used to access the message buffer */
    );

/* returns snmp response data from current transaction */
#ifndef _WRS_CONFIG_LP64
IMPORT INT32
#else
IMPORT INT64
#endif
    wmmErrorMsgGet
    (
    INT8*,             /* Buffer to place data */
#ifndef _WRS_CONFIG_LP64
    INT32,             /* Buffer size  */
    INT32,             /* Number of messages to return */
#else
    INT64,             /* Buffer size  */
    INT64,             /* Number of messages to return */
#endif
    INT32              /* Unique Id used to access the message buffer */
    );

/* Places snmp response data into a connection based ring buffer */
IMPORT void
    wmmErrorMsgLog
    (
    SNMPADDR_T*, /* Source IP address */
    SNMPADDR_T*, /* Destination IP address */
    PTR_T,       /* generic packet pointer */
    ALENGTH_T,   /* need */
    PTR_T        /* generic cookie */
    );

/* Places snmp response data into a connection based ring buffer */
IMPORT void
    wmmErrorLog
    (
    SNMPADDR_T*, /* Source IP address */
    SNMPADDR_T*, /* Destination IP address */
    int,         /* error code set by the snmp agent */
    PTR_T        /* generic cookie */
    );

#endif /* INCwmmErrorLogh */

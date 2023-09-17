/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/axapi.h,v 1.5 2002/04/04 15:20:35 andre Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: axapi.h,v $
 * Revision 1.5  2002/04/04 15:20:35  andre
 * Change enums to #defines; add default user-exit
 *
 * Revision 1.3  2002/03/18 20:20:04  tneale
 * Changed type of argument tcount to conform to correct type
 *
 * Revision 1.2  2002/02/26 18:41:49  josh
 * update and nits
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if !defined(_RFC2742API_H_)
#define _RFC2742API_H_

/* Macros / symbolic constant definitions */

#if !defined(TRUE)
#define TRUE  1
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(ENVOY_AX_CONN_ENTRY)
/* Define ENVOY_AX_CONN_ENTRY for non-VxWorks builds */
#define ENVOY_AX_CONN_ENTRY(INDEX, MATCH_TYPE, P_CONNENT) 0
#endif

#if !defined(ENVOY_AX_GET_CONNID)
/* Define ENVOY_AX_GET_CONNID for non-VxWorks builds */
#define ENVOY_AX_GET_CONNID(PCE) 1
#endif

#define  agentxConnectionEntry_INSTANCE_LEN    1
#define  agentxSessionEntry_INSTANCE_LEN       2
#define  agentxRegistrationEntry_INSTANCE_LEN  3

/* Internal data retrieval definitions, typedefs and structures */

#define  AX_MATCH_GET   1
#define  AX_MATCH_NEXT  2

#define  AX_GEN_DEFTO       1
#define  AX_GEN_VER         2
#define  AX_CONN_LASTCHG    3
#define  AX_SESS_LASTCHG    4
#define  AX_REG_LASTCHG     5

/* GENERAL, CONNECTION, SESSION and REGISTRATION group structures */

typedef struct AX_GEN_S
{
    bits32_t  agentxDefaultTimeout;
    bits32_t  agentxMasterAgentXVer;
} AX_GEN_T;

typedef struct AX_CONNENT_S
{
    bits32_t  agentxConnIndex;
    bits32_t  agentxConnTransportTime;
    OBJ_ID_T  agentxConnTransportDomain;
    EBUFFER_T agentxConnTransportAddress;
} AX_CONNENT_T;

typedef struct AX_SESSENT_S
{
    bits32_t  agentxConnIndex;
    bits32_t  agentxSessIndex;
    OBJ_ID_T  agentxSessObjectID;
    EBUFFER_T agentxSessDescr;
    bits32_t  agentxSessAdminStatus;
    bits32_t  agentxSessOpenTime;
    bits32_t  agentxSessAgentXVer;
    bits32_t  agentxSessTimeout;
} AX_SESSENT_T;

typedef struct AX_REGENT_S
{
    bits32_t  agentxConnIndex;
    bits32_t  agentxSessIndex;
    bits32_t  agentxRegIndex;
    EBUFFER_T agentxRegContext;
    OBJ_ID_T  agentxRegStart;
    bits32_t  agentxRegRangeSubId;
    bits32_t  agentxRegUpperBound;
    bits32_t  agentxRegPriority;
    bits32_t  agentxRegTimeout;
    bits32_t  agentxRegInstance;
} AX_REGENT_T;

/* API prototypes */

boolean_t     AxGetScalar (int scalarCode, bits32_t *value);

AX_CONNENT_T *AxConnGetEntry (int tcount, OIDC_T *tlist);
AX_CONNENT_T *AxConnGetNext  (int tcount, OIDC_T *tlist);

AX_SESSENT_T *AxSessGetEntry (int tcount, OIDC_T *tlist);
AX_SESSENT_T *AxSessGetNext  (int tcount, OIDC_T *tlist);

AX_REGENT_T  *AxRegGetEntry (int tcount, OIDC_T *tlist);
AX_REGENT_T  *AxRegGetNext  (int tcount, OIDC_T *tlist);

/* axmth.c prototypes:  GENERAL group */

void agentxGeneral_get  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxGeneral_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);

/* axmth.c prototypes:  CONNECTION group */

void agentxConnectionEntry_get (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
AX_CONNENT_T *agentxConnectionEntry_first (void);
void agentxConnectionEntry_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
bits32_t agentxConnectionEntry_build_instance (AX_CONNENT_T *axConnEntry, OIDC_T *tempInst);
void agentxConnection_get  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxConnection_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void setAxConnTblLastChg (bits32_t timeStamp);
void agentxFreeConnEntry (ptr_t pConnEntry);

/* axmth.c prototypes:  SESSION group */

void agentxSessionEntry_get  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxSessionEntry_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxSessionEntry_test (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxSessionEntry_set  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);

void agentxSession_get  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxSession_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);

/* axmth.c prototypes:  REGISTRATION group */

void agentxRegistrationEntry_get  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxRegistrationEntry_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxRegistration_get  (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);
void agentxRegistration_next (OIDC_T lastmatch, int tcount, OIDC_T *tlist, SNMP_PKT_T *pktp, VB_T *vbp);

/* mibLeaf.h definitions moved here for convenience */

#define LEAF_agentxDefaultTimeout 1
#define MIN_agentxDefaultTimeout 0L
#define MAX__agentxDefaultTimeout 255L
#define LEAF_agentxMasterAgentXVer 2
#define MIN_agentxMasterAgentXVer 1L
#define MAX__agentxMasterAgentXVer 255L
#define LEAF_agentxConnTableLastChange 1
#define LEAF_agentxConnIndex 1
#define MIN_agentxConnIndex 1L
#define MAX__agentxConnIndex 4294967295L
#define LEAF_agentxConnTransportTime 2
#define LEAF_agentxConnTransportDomain 3
#define LEAF_agentxConnTransportAddress 4
#define MINSIZE_agentxConnTransportAddress 0L
#define MAX_SIZE_agentxConnTransportAddress 255L
#define LEAF_agentxSessionTableLastChange 1
#define LEAF_agentxSessionIndex 1
#define MIN_agentxSessionIndex 0L
#define MAX__agentxSessionIndex 4294967295L
#define LEAF_agentxSessionObjectID 2
#define LEAF_agentxSessionDescr 3
#define MINSIZE_agentxSessionDescr 0L
#define MAX_SIZE_agentxSessionDescr 255L
#define LEAF_agentxSessionAdminStatus 4
#define VAL_agentxSessionAdminStatus_up 1L
#define VAL_agentxSessionAdminStatus_down 2L
#define LEAF_agentxSessionOpenTime 5
#define LEAF_agentxSessionAgentXVer 6
#define MIN_agentxSessionAgentXVer 1L
#define MAX__agentxSessionAgentXVer 255L
#define LEAF_agentxSessionTimeout 7
#define MIN_agentxSessionTimeout 0L
#define MAX__agentxSessionTimeout 255L
#define LEAF_agentxRegistrationTableLastChange 1
#define LEAF_agentxRegIndex 1
#define MIN_agentxRegIndex 1L
#define MAX__agentxRegIndex 4294967295L
#define LEAF_agentxRegContext 2
#define LEAF_agentxRegStart 3
#define LEAF_agentxRegRangeSubId 4
#define LEAF_agentxRegUpperBound 5
#define LEAF_agentxRegPriority 6
#define LEAF_agentxRegTimeout 7
#define MIN_agentxRegTimeout 0L
#define MAX__agentxRegTimeout 255L
#define LEAF_agentxRegInstance 8
#define VAL_agentxRegInstance_true 1L
#define VAL_agentxRegInstance_false 2L

#endif /* _RFC2742API_H_ */

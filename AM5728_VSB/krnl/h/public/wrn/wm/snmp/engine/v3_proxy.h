/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/v3_proxy.h,v 1.2 2001/11/06 21:35:07 josh Exp $ */

/* v3_proxy.h - definitions for RFC 2573 Proxy code */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  All rights reserved.
 ****************************************************************************/

/*
modification history
--------------------
01d,15sep09,y_t  fix for WIND00181371
01c,19apr05,job  update copyright notices
01b,25nov03,ps  copyright update
01a,25aug03,job  fixing a prototype mismatch
$Log: v3_proxy.h,v $
Revision 1.2  2001/11/06 21:35:07  josh
new revised path structure, first pass.  Also removing useless
shell include files.

Revision 1.1.1.1  2001/11/05 17:47:24  tneale
Tornado shuffle

Revision 9.4.2.4  2001/09/21 19:16:07  josh
updating and expanding structures for use with coexistence

Revision 9.4.2.3  2001/08/31 17:02:20  josh
clean up a few nits

Revision 9.4.2.2  2001/08/24 14:31:24  josh
adding function prototypes for the proxy processing routines

Revision 9.4.2.1  2001/06/28 18:22:57  josh
Seeding the Cormorant branch with the code from the old
cormorant branch

Revision 9.3.2.1  2001/04/11 19:35:41  josh
moving cormorant-specific files onto the special
cormorant branch so they don't get in kingfisher's way

Revision 9.3  2001/01/19 22:22:12  paul
Update copyright.

Revision 9.2  2000/07/12 18:42:27  josh
definitions for proxy control block and associated flags and
structures

Revision 9.1  2000/06/09 14:54:13  josh
modifications due to new installation options, new proxy code
moved some definitions from sendntfy.h into v3_trgt.h so they'd
be built with the target code


*/

#if (!defined(__INCv3Proxyh))
#define __INCv3Proxyh

#ifdef __cplusplus
extern"C" {
#endif /* __cplusplus */

#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/buffer.h>
#include <wrn/wm/snmp/engine/smi.h>
#include <wrn/wm/snmp/engine/etimer.h>

#define ETC_PROXY_NAME_MAX   32

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct SNMP_PROXY_S
    {
    struct SNMP_PROXY_S *next;             /* next proxy object in list    */
    struct EBUFFER_S  proxy_name;          /* the name of the proxy object */
    struct EBUFFER_S  context_engine_id;   /* engine id of this proxy      */
    struct EBUFFER_S  context_name;        /* context name of proxy        */
    struct EBUFFER_S  target_params_in;    /* for endpoint matching        */
    struct EBUFFER_S  single_target_out;   /* for forwarding of requests   */
    struct EBUFFER_S  multiple_target_out; /* for forwarding of notifies   */
    ETC_STORAGE_T     storage;             /* storage type of this object  */
    ETC_STATUS_T      status;              /* row status of this object    */
    bits8_t           proxy_type;          /* read, write, trap or inform  */ 
    } SNMP_PROXY_T;

/* global root pointer for target params list */
extern SNMP_PROXY_T *root_proxy;

#define SNMP_Proxy_Set_Defaults(P)                     \
      (MEMSET((P), 0, sizeof(SNMP_PROXY_T)),           \
       EBufferInitialize(&((P)->proxy_name)),          \
       EBufferInitialize(&((P)->context_engine_id)),   \
       EBufferInitialize(&((P)->context_name)),        \
       EBufferInitialize(&((P)->target_params_in)),    \
       EBufferInitialize(&((P)->single_target_out)),   \
       EBufferInitialize(&((P)->multiple_target_out)), \
       (P)->storage = ETC_STO_NONVOL,                  \
       (P)->status = ETC_RS_NREADY,                     \
       (P)->proxy_type = PROXY_TYPE_READ)
          
/* values for the proxy_type field */
#define PROXY_TYPE_READ   1
#define PROXY_TYPE_WRITE  2
#define PROXY_TYPE_TRAP   3
#define PROXY_TYPE_INFORM 4

extern SNMP_PROXY_T * SNMP_Proxy_Create  __((void));
extern void SNMP_Proxy_Destroy __((SNMP_PROXY_T * proxy));
extern envoy_err_t  SNMP_Proxy_Install __((SNMP_PROXY_T * proxy,
                                           bits8_t *      proxy_name,
                                           ALENGTH_T      proxy_name_len));
extern void SNMP_Proxy_Deinstall __((SNMP_PROXY_T * proxy));
extern SNMP_PROXY_T * SNMP_Proxy_Lookup __((bits8_t * proxy_name,
                                            ALENGTH_T proxy_name_len));
extern SNMP_PROXY_T * SNMP_Proxy_Next __((int      tcount,
                                          OIDC_T * tlist));
extern SNMP_PROXY_T * SNMP_Proxy_Next_Proxy __((SNMP_PROXY_T * proxy));
extern void SNMP_Proxy_Name __((SNMP_PROXY_T * proxy,
                                bits8_t *      proxy_name,
                                ALENGTH_T *    proxy_name_len));

#define SNMP_Proxy_Get_Proxy_Name(P)         (EBufferStart(&((P)->proxy_name)))
#define SNMP_Proxy_Get_Proxy_Name_Len(P)     (EBufferUsed(&((P)->proxy_name)))

#define SNMP_Proxy_Get_Proxy_Type(P)         ((P)->proxy_type)
#define SNMP_Proxy_Set_Proxy_Type(P, V)      ((P)->proxy_type = (bits16_t)(V))

#define SNMP_Proxy_Get_Context_Engine_ID(P)  \
(EBufferStart(&((P)->context_engine_id)))
#define SNMP_Proxy_Get_Context_Engine_ID_Len(P) \
        (EBufferUsed(&((P)->context_engine_id)))
#define SNMP_Proxy_Set_Context_Engine_ID(P, S, L, F) \
        (EBufferAllocateLoad((F), &((P)->context_engine_id), (S), (L)))

#define SNMP_Proxy_Get_Context_Name(P) (EBufferStart(&((P)->context_name)))
#define SNMP_Proxy_Get_Context_Name_Len(P) (EBufferUsed(&((P)->context_name)))
#define SNMP_Proxy_Set_Context_Name(P, S, L, F) \
        (EBufferAllocateLoad((F), &((P)->context_name), (S), (L)))

#define SNMP_Proxy_Get_Target_Params_In(P)  \
        (EBufferStart(&((P)->target_params_in)))
#define SNMP_Proxy_Get_Target_Params_In_Len(P) \
        (EBufferUsed(&((P)->target_params_in)))
#define SNMP_Proxy_Set_Target_Params_In(P, S, L, F) \
        (EBufferAllocateLoad((F), &((P)->target_params_in), (S), (L)))

#define SNMP_Proxy_Get_Single_Target_Out(P)  \
        (EBufferStart(&((P)->single_target_out)))
#define SNMP_Proxy_Get_Single_Target_Out_Len(P) \
        (EBufferUsed(&((P)->single_target_out)))
#define SNMP_Proxy_Set_Single_Target_Out(P, S, L, F) \
        (EBufferAllocateLoad((F), &((P)->single_target_out), (S), (L)))

#define SNMP_Proxy_Get_Multiple_Target_Out(P)  \
        (EBufferStart(&((P)->multiple_target_out)))
#define SNMP_Proxy_Get_Multiple_Target_Out_Len(P) \
        (EBufferUsed(&((P)->multiple_target_out)))
#define SNMP_Proxy_Set_Multiple_Target_Out(P, S, L, F) \
        (EBufferAllocateLoad((F), &((P)->multiple_target_out), (S), (L)))

#define SNMP_Proxy_Get_Storage(P)     ((P)->storage)
#define SNMP_Proxy_Set_Storage(P, V)  ((P)->storage = (bits16_t)(V))

#define SNMP_Proxy_Get_Status(P)      ((P)->status)
#define SNMP_Proxy_Set_Status(P, V)   ((P)->status = (bits16_t)(V))

#define SNMP_Proxy_Get_Flags(P)       ((P)->flags)
#define SNMP_Proxy_Set_Flags(P, V)    ((P)->flags = (bits16_t)(V))

/* User-definable stuff */
#ifndef PROXY_SWEEP_TIME
#define PROXY_SWEEP_TIME 3000
#endif    
    
#ifndef ENVOY_SNMP_SEND_PROXY
#define ENVOY_SNMP_SEND_PROXY(FOR, LOC, PKT, SIZ)
#endif

/* structure definition for proxy control block */
struct TARGET_LIST_S;

typedef struct SNMP_PROXY_CB_S
    {
    struct SNMP_PROXY_CB_S * next;
    struct TARGET_LIST_S   * targetList;
    SNMP_PKT_T             * originalPkt;
    EBUFFER_T                contextEngineID;
    EBUFFER_T                contextName;
    EBUFFER_T                secName;
    EBUFFER_T                communityString;
    bits32_t                 msgID;
    bits32_t                 expires;
    bits8_t                  flags;
    } SNMP_PROXY_CB_T;

#define SNMP_PROXY_FLAGS_EXPIRED          0x01
#define SNMP_PROXY_FLAGS_RESPONDED        0x02
#define SNMP_PROXY_FLAGS_VBLIST_COPIED    0x04
    
typedef struct SNMP_PROXY_CB_ROOT_S
    {
    SNMP_PROXY_CB_T        * pcb;
    bits32_t               * nextMsgID;
    ENVOY_TIMER_T            timer;
    } SNMP_PROXY_CB_ROOT_T;

extern SNMP_PROXY_CB_ROOT_T root_proxy_block;

extern envoy_err_t snmpProxyV3Request __((SNMP_PKT_T *));

extern envoy_err_t snmpProxyV3Notify __((SNMP_PKT_T *));

extern envoy_err_t snmpProxyV3Response __((SNMP_PKT_T *));

extern void snmpProxyEntry_get  __((OIDC_T, int, OIDC_T *,
				    SNMP_PKT_T *, VB_T *));

extern void snmpProxyEntry_next  __((OIDC_T, int, OIDC_T *,
				     SNMP_PKT_T *, VB_T *));

extern void snmpProxyEntry_test  __((OIDC_T, int, OIDC_T *,
				     SNMP_PKT_T *, VB_T *));

extern void snmpProxyEntry_set  __((OIDC_T, int, OIDC_T *,
				    SNMP_PKT_T *, VB_T *));

/* Definitions of leaf nodes for MIB support */

#define LEAF_snmpProxyName	1
#define MINSIZE_snmpProxyName	1L
#define MAXSIZE_snmpProxyName	32L
#define LEAF_snmpProxyType	2
#define VAL_snmpProxyType_read	1L
#define VAL_snmpProxyType_write	2L
#define VAL_snmpProxyType_trap	3L
#define VAL_snmpProxyType_inform	4L
#define LEAF_snmpProxyContextEngineID	3
#define MINSIZE_snmpProxyContextEngineID	5L
#define MAXSIZE_snmpProxyContextEngineID	32L
#define LEAF_snmpProxyContextName	4
#define MINSIZE_snmpProxyContextName	0L
#define MAXSIZE_snmpProxyContextName	255L
#define LEAF_snmpProxyTargetParamsIn	5
#define MINSIZE_snmpProxyTargetParamsIn	0L
#define MAXSIZE_snmpProxyTargetParamsIn	255L
#define LEAF_snmpProxySingleTargetOut	6
#define MINSIZE_snmpProxySingleTargetOut	0L
#define MAXSIZE_snmpProxySingleTargetOut	255L
#define LEAF_snmpProxyMultipleTargetOut	7
#define MINSIZE_snmpProxyMultipleTargetOut	0L
#define MAXSIZE_snmpProxyMultipleTargetOut	255L
#define LEAF_snmpProxyStorageType	8
#define VAL_snmpProxyStorageType_other	1L
#define VAL_snmpProxyStorageType_volatile	2L
#define VAL_snmpProxyStorageType_nonVolatile	3L
#define VAL_snmpProxyStorageType_permanent	4L
#define VAL_snmpProxyStorageType_readOnly	5L
#define LEAF_snmpProxyRowStatus	9
#define VAL_snmpProxyRowStatus_active	1L
#define VAL_snmpProxyRowStatus_notInService	2L
#define VAL_snmpProxyRowStatus_notReady	3L
#define VAL_snmpProxyRowStatus_createAndGo	4L
#define VAL_snmpProxyRowStatus_createAndWait	5L
#define VAL_snmpProxyRowStatus_destroy	6L

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCv3Proxyh */

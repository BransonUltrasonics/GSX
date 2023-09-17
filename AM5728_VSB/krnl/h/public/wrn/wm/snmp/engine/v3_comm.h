/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/v3_comm.h,v 1.2 2001/11/06 21:35:05 josh Exp $ */

/*
 * Copyright (C) 1999-2005, 2013 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: v3_comm.h,v $
 * Revision 1.2  2001/11/06 21:35:05  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.3  2001/09/21 19:16:07  josh
 * updating and expanding structures for use with coexistence
 *
 * Revision 1.1.2.2  2001/08/07 18:33:57  josh
 * adding support for SNMPv1/v2c/v3 coexistence to SNMP Notify code
 *
 * Revision 1.1.2.1  2001/07/16 18:02:12  josh
 * a first-round commit adding infrastructure support
 * for the SNMPv3 Coexistence RFC (2576)
 *
 *
 */

/* [clearcase]
modification history
-------------------
01b,06aug13,bbz  clear the med impact warning from prevent.pl(WIND00429583)
01a,19apr05,job  update copyright notices
*/


#if (!defined(v3_comm_inc))
#define v3_comm_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(buffer_inc))
#include <wrn/wm/snmp/engine/buffer.h>
#endif

#if (!defined(smi_inc))
#include <wrn/wm/snmp/engine/smi.h>
#endif

#define ETC_COMMUNITY_MAX   32

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct SNMP_COMMUNITY_S
        {
	struct SNMP_COMMUNITY_S *next; 
        EBUFFER_T  comm_index;      /* the name of this entry */
        EBUFFER_T  comm_name;       /* the community name */
        EBUFFER_T  comm_sec_name;   /* security name */
        EBUFFER_T  comm_con_id;     /* context engine id */
        EBUFFER_T  comm_con_name;   /* context name */
        EBUFFER_T  comm_tag;        /* transport tag */
        bits16_t   storage;
        bits16_t   status;
        bits16_t   flags;
        } SNMP_COMMUNITY_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

#define ETC_COMM_NAME      0x01
#define ETC_COMM_SEC_NAME  0x02
#define ETC_COMM_ALL_FLAGS 0x03

/* global root pointer for target params list */
extern SNMP_COMMUNITY_T *root_community;

#define SNMP_Community_Set_Defaults(C)                              \
      (MEMSET((C), 0, sizeof(SNMP_COMMUNITY_T)),                    \
       EBufferInitialize(&((C)->comm_index)),                       \
       EBufferInitialize(&((C)->comm_name)),                        \
       EBufferInitialize(&((C)->comm_sec_name)),                    \
       EBufferInitialize(&((C)->comm_con_id)),                      \
       EBufferAllocateLoad(BFL_IS_ALLOC, &((C)->comm_con_id),       \
                           v3_my_engine->id, v3_my_engine->id_len), \
       EBufferInitialize(&((C)->comm_con_name)),                    \
       EBufferInitialize(&((C)->comm_tag)),                         \
       (C)->storage = ETC_STO_NONVOL,                               \
       (C)->status = ETC_RS_NREADY,                                 \
       (C)->flags = 0)
          
extern SNMP_COMMUNITY_T * SNMP_Community_Create  __((void));
extern void SNMP_Community_Destroy __((SNMP_COMMUNITY_T *));
extern envoy_err_t  SNMP_Community_Install __((SNMP_COMMUNITY_T *, bits8_t *, 
                                               ALENGTH_T));
extern void SNMP_Community_Deinstall __((SNMP_COMMUNITY_T *));
extern SNMP_COMMUNITY_T * SNMP_Community_Lookup __((bits8_t *, ALENGTH_T));
extern SNMP_COMMUNITY_T * SNMP_Community_Next __((int, OIDC_T *));
extern SNMP_COMMUNITY_T * SNMP_Community_Next_Community
    __((SNMP_COMMUNITY_T *));
extern void SNMP_Community_Index __((SNMP_COMMUNITY_T *, bits8_t *, 
                                     ALENGTH_T *));

#define SNMP_Community_Get_Index(C)        (EBufferStart(&((C)->comm_index)))
#define SNMP_Community_Get_Index_Len(C)    (EBufferUsed(&((C)->comm_index)))

#define SNMP_Community_Get_Name(C)        (EBufferStart(&((C)->comm_name)))
#define SNMP_Community_Get_Name_Len(C)    (EBufferUsed(&((C)->comm_name)))
#define SNMP_Community_Set_Name(C, S, L, F) \
        (EBufferAllocateLoad((F), &((C)->comm_name), (S), (L)))

#define SNMP_Community_Get_Sec_Name(C)    (EBufferStart(&((C)->comm_sec_name)))
#define SNMP_Community_Get_Sec_Name_Len(C) (EBufferUsed(&((C)->comm_sec_name)))
#define SNMP_Community_Set_Sec_Name(C, S, L, F) \
        (EBufferAllocateLoad((F), &((C)->comm_sec_name), (S), (L)))

#define SNMP_Community_Get_Con_ID(C)        (EBufferStart(&((C)->comm_con_id)))
#define SNMP_Community_Get_Con_ID_Len(C)    (EBufferUsed(&((C)->comm_con_id)))
#define SNMP_Community_Set_Con_ID(C, S, L, F) \
        (EBufferAllocateLoad((F), &((C)->comm_con_id), (S), (L)))

#define SNMP_Community_Get_Con_Name(C)    (EBufferStart(&((C)->comm_con_name)))
#define SNMP_Community_Get_Con_Name_Len(C) (EBufferUsed(&((C)->comm_con_name)))
#define SNMP_Community_Set_Con_Name(C, S, L, F) \
        (EBufferAllocateLoad((F), &((C)->comm_con_name), (S), (L)))

#define SNMP_Community_Get_Tag(C)         (EBufferStart(&((C)->comm_tag)))
#define SNMP_Community_Get_Tag_Len(C)     (EBufferUsed(&((C)->comm_tag)))
#define SNMP_Community_Set_Tag(C, S, L, F) \
        (EBufferAllocateLoad((F), &((C)->comm_tag), (S), (L)))

#define SNMP_Community_Get_Storage(C)     ((C)->storage)
#define SNMP_Community_Set_Storage(C, V)  ((C)->storage = (bits16_t)(V))

#define SNMP_Community_Get_Status(C)     ((C)->status)
#define SNMP_Community_Set_Status(C, V)  ((C)->status = (bits16_t)(V))

#define SNMP_Community_Get_Flags(C)     ((C)->flags)
#define SNMP_Community_Set_Flags(C, V)  ((C)->flags = (bits16_t)(V))

extern void snmpCommunityEntry_get  __((OIDC_T, int, OIDC_T *,
					 SNMP_PKT_T *, VB_T *));

extern void snmpCommunityEntry_next  __((OIDC_T, int, OIDC_T *,
					  SNMP_PKT_T *, VB_T *));

extern void snmpCommunityEntry_test  __((OIDC_T, int, OIDC_T *,
					  SNMP_PKT_T *, VB_T *));

extern void snmpCommunityEntry_set  __((OIDC_T, int, OIDC_T *,
					 SNMP_PKT_T *, VB_T *));

extern struct SNMP_TARGET_ADDR_S *find_target_by_community __((EBUFFER_T *,
                                                               OBJ_ID_T *,
                                                               EBUFFER_T *));

#if (INSTALL_ENVOY_SNMP_V3_NOTIFY) || (INSTALL_ENVOY_SNMP_V3_PROXY)
/* Some well-known OIDs that will be necessary to deal with traps
 * and proxies
 */
extern OIDC_T COEX_SNMPTRAPOID[];   
extern OIDC_T COEX_TRAPENTERPRISE[];
extern OIDC_T COEX_SNMPTRAPS[];   
extern OIDC_T COEX_COLDSTART[];   
extern OIDC_T COEX_WARMSTART[];   
extern OIDC_T COEX_LINKDOWN[];    
extern OIDC_T COEX_LINKUP[];      
extern OIDC_T COEX_AUTHFAILURE[]; 
extern OIDC_T COEX_NEIGHBORLOSS[];
#endif

/* The leaf definitions for the method routines */
#define LEAF_snmpCommunityIndex	1
#define MINSIZE_snmpCommunityIndex	1L
#define MAXSIZE_snmpCommunityIndex	32L
#define LEAF_snmpCommunityName	2
#define LEAF_snmpCommunitySecurityName	3
#define MINSIZE_snmpCommunitySecurityName	1L
#define MAXSIZE_snmpCommunitySecurityName	32L
#define LEAF_snmpCommunityContextEngineID	4
#define MINSIZE_snmpCommunityContextEngineID	5L
#define MAXSIZE_snmpCommunityContextEngineID	32L
#define LEAF_snmpCommunityContextName	5
#define MINSIZE_snmpCommunityContextName	0L
#define MAXSIZE_snmpCommunityContextName	32L
#define LEAF_snmpCommunityTransportTag	6
#define MINSIZE_snmpCommunityTransportTag	0L
#define MAXSIZE_snmpCommunityTransportTag	255L
#define LEAF_snmpCommunityStorageType	7
#define VAL_snmpCommunityStorageType_other	1L
#define VAL_snmpCommunityStorageType_volatile	2L
#define VAL_snmpCommunityStorageType_nonVolatile	3L
#define VAL_snmpCommunityStorageType_permanent	4L
#define VAL_snmpCommunityStorageType_readOnly	5L
#define LEAF_snmpCommunityStatus	8
#define VAL_snmpCommunityStatus_active	1L
#define VAL_snmpCommunityStatus_notInService	2L
#define VAL_snmpCommunityStatus_notReady	3L
#define VAL_snmpCommunityStatus_createAndGo	4L
#define VAL_snmpCommunityStatus_createAndWait	5L
#define VAL_snmpCommunityStatus_destroy	6L
#define LEAF_snmpTargetAddrTMask	1
#define MINSIZE_snmpTargetAddrTMask	0L
#define MAXSIZE_snmpTargetAddrTMask	255L
#define LEAF_snmpTargetAddrMMS	2
#define MIN_snmpTargetAddrMMS	484L
#define MAX_snmpTargetAddrMMS	2147483646L
#define LEAF_snmpTrapAddress	3
#define LEAF_snmpTrapCommunity	4

#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(v3_comm_inc))*/

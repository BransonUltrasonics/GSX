/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/entlogic.h,v 1.1 2002/05/23 18:47:30 josh Exp $ */

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
 * $Log: entlogic.h,v $
 * Revision 1.1  2002/05/23 18:47:30  josh
 * support for the entLogicalTable from RFC 2737
 *
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(ent_logical_inc))
#define ent_logical_inc

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

#define ETC_ENT_LOGICAL_MAX_INDEX  0x7fffffff
#define ETC_ENT_LOGICAL_MAX   255

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct ENT_LOGICAL_S
        {
	struct ENT_LOGICAL_S *next; 
        bits32_t              entLogicalIndex;
        EBUFFER_T             entLogicalDescr;
        OBJ_ID_T              entLogicalType;
        EBUFFER_T             entLogicalCommunity;
        EBUFFER_T             entLogicalTAddress;
        OBJ_ID_T              entLogicalTDomain;
        EBUFFER_T             entLogicalContextEngineID;
        EBUFFER_T             entLogicalContextName;
        bits16_t              flags;
        } ENT_LOGICAL_T;

/* global root pointer for target params list */
extern ENT_LOGICAL_T *root_ent_logical;

#define Ent_Logical_Set_Defaults(L)                          \
      (MEMSET((L), 0, sizeof(ENT_LOGICAL_T)),                \
       EBufferInitialize(&((L)->entLogicalDescr)),           \
       EBufferInitialize(&((L)->entLogicalCommunity)),       \
       EBufferInitialize(&((L)->entLogicalTAddress)),        \
       EBufferInitialize(&((L)->entLogicalContextEngineID)), \
       EBufferInitialize(&((L)->entLogicalContextName)),     \
       (L)->flags = 0)
          
extern ENT_LOGICAL_T * Ent_Logical_Create  __((void));
extern void Ent_Logical_Destroy __((ENT_LOGICAL_T *));
extern int  Ent_Logical_Install __((ENT_LOGICAL_T *, bits32_t));
extern void Ent_Logical_Deinstall __((ENT_LOGICAL_T *));
extern ENT_LOGICAL_T * Ent_Logical_Lookup __((bits32_t));
extern ENT_LOGICAL_T * Ent_Logical_Next __((int, OIDC_T *));
extern ENT_LOGICAL_T * Ent_Logical_Next_Ent __((ENT_LOGICAL_T *));

#define Ent_Logical_Get_Index(L)       ((L)->entLogicalIndex)
#define Ent_Logical_Set_Index(L, V)    ((L)->entLogicalIndex = (V))
    
#define Ent_Logical_Get_Descr(L)        (EBufferStart(&((L)->entLogicalDescr)))
#define Ent_Logical_Get_Descr_Len(L)    (EBufferUsed(&((L)->entLogicalDescr)))
#define Ent_Logical_Set_Descr(L, S, N, F) \
        (EBufferAllocateLoad((F), &((L)->entLogicalDescr), (S), (N)))
    
#define Ent_Logical_Get_Type(L)    (&((L)->entLogicalType))
#define Ent_Logical_Set_Type(L, D, N) \
        (build_object_id((N), (D), (&((L)->entLogicalType))))

#define Ent_Logical_Get_Community(L) \
        (EBufferStart(&((L)->entLogicalCommunity)))
#define Ent_Logical_Get_Community_Len(L) \
        (EBufferUsed(&((L)->entLogicalCommunity)))
#define Ent_Logical_Set_Community(L, S, N, F) \
        (EBufferAllocateLoad((F), &((L)->entLogicalCommunity), (S), (N)))
    
#define Ent_Logical_Get_TAddress(L) \
        (EBufferStart(&((L)->entLogicalTAddress)))
#define Ent_Logical_Get_TAddress_Len(L) \
        (EBufferUsed(&((L)->entLogicalTAddress)))
#define Ent_Logical_Set_TAddress(L, S, N, F) \
        (EBufferAllocateLoad((F), &((L)->entLogicalTAddress), (S), (N)))
    
#define Ent_Logical_Get_TDomain(L)    (&((L)->entLogicalTDomain))
#define Ent_Logical_Set_TDomain(L, D, N) \
        (build_object_id((N), (D), (&((L)->entLogicalTDomain))))

#define Ent_Logical_Get_Context_Engine_ID(L) \
        (EBufferStart(&((L)->entLogicalContextEngineID)))
#define Ent_Logical_Get_Context_Engine_ID_Len(L) \
        (EBufferUsed(&((L)->entLogicalContextEngineID)))
#define Ent_Logical_Set_Context_Engine_ID(L, S, N, F) \
        (EBufferAllocateLoad((F), &((L)->entLogicalContextEngineID), (S), (N)))
    
#define Ent_Logical_Get_Context_Name(L) \
        (EBufferStart(&((L)->entLogicalContextName)))
#define Ent_Logical_Get_Context_Name_Len(L) \
        (EBufferUsed(&((L)->entLogicalContextName)))
#define Ent_Logical_Set_Context_Name(L, S, N, F) \
        (EBufferAllocateLoad((F), &((L)->entLogicalContextName), (S), (N)))
    
#define Ent_Logical_Get_Flags(L)     ((L)->flags)
#define Ent_Logical_Set_Flags(L, V)  ((L)->flags = (bits16_t)(V))

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct ENT_GENERAL_S
    {
    bits32_t entLastChangeTime;
    } ENT_GENERAL_T;


/* Global pointer for the entityGeneral group */
extern ENT_GENERAL_T root_ent_general;

#define Ent_General_Set_Defaults(G) \
       (MEMSET((G), 0, sizeof(ENT_GENERAL_T)))

#define Ent_General_Group_Lookup() (&root_ent_general)

#define Ent_General_Get_Last_Change_Time(G) ((G)->entLastChangeTime)
#define Ent_General_Set_Last_Change_Time(G, V) \
        ((G)->entLastChangeTime = (bits32_t) V)
    
/* Constants for entLogicalTable */
#define LEAF_entLogicalIndex	1
#define MIN_entLogicalIndex	1L
#define MAX_entLogicalIndex	2147483647L
#define LEAF_entLogicalDescr	2
#define MINSIZE_entLogicalDescr	0L
#define MAXSIZE_entLogicalDescr	255L
#define LEAF_entLogicalType	3
#define LEAF_entLogicalCommunity	4
#define MINSIZE_entLogicalCommunity	0L
#define MAXSIZE_entLogicalCommunity	255L
#define LEAF_entLogicalTAddress	5
#define MINSIZE_entLogicalTAddress	1L
#define MAXSIZE_entLogicalTAddress	255L
#define LEAF_entLogicalTDomain	6
#define LEAF_entLogicalContextEngineID	7
#define MINSIZE_entLogicalContextEngineID	0L
#define MAXSIZE_entLogicalContextEngineID	32L
#define LEAF_entLogicalContextName	8
#define MINSIZE_entLogicalContextName	0L
#define MAXSIZE_entLogicalContextName	255L

/* Constants for entGeneral group */
#define LEAF_entLastChangeTime	1

extern void entLogicalEntry_get __((OIDC_T, int, OIDC_T *,
                                    SNMP_PKT_T *, VB_T *));

extern void entLogicalEntry_next __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));

extern void entityGeneral_get __((OIDC_T, int, OIDC_T *,
                                  SNMP_PKT_T *, VB_T *));

extern void entityGeneral_next __((OIDC_T, int, OIDC_T *,
                                   SNMP_PKT_T *, VB_T *));
    
#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(ent_logical_inc))*/

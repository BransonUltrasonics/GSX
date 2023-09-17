/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/v3_ntfy.h,v 1.2 2001/11/06 21:35:06 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: v3_ntfy.h,v $
 * Revision 1.2  2001/11/06 21:35:06  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:24  tneale
 * Tornado shuffle
 *
 * Revision 9.6  2001/01/19 22:22:12  paul
 * Update copyright.
 *
 * Revision 9.5  2000/03/17 19:48:46  meister
 * Update copyright notice
 *
 * Revision 9.4  2000/03/09 20:58:36  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.3  2000/03/09 17:06:27  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.2  2000/02/04 21:54:24  josh
 * prototyping functions that may need to be called by other modules
 * this is done primarily to make the vxWorks compiler happy.
 * Warning:  the prototypes added may be duplicated by a mib_hand.h
 * file generated by Emissary.
 *
 * Revision 9.1  2000/01/02 22:55:56  josh
 * patching up a memory leak here, a broken compare there...per
 * sar's comments
 *
 * Revision 9.0  1999/10/21 20:43:36  josh
 * updating version stamps
 *
 * Revision 1.7  1999/10/21 19:13:08  josh
 * adding flags for flags fields in various objects to help with row
 * creation
 *
 * Revision 1.6  1999/10/15 17:47:27  josh
 * included MIB leaf definitions and cleaned up useless macros
 *
 * Revision 1.5  1999/09/21 21:31:26  josh
 * Adding a limit constant for the NotifyFilterMask
 *
 * Revision 1.4  1999/09/14 19:35:26  josh
 * updating default initialization per RFC 2573
 *
 * Revision 1.3  1999/09/11 02:15:19  josh
 * added definitions for filter excluded and included
 *
 * Revision 1.2  1999/09/01 20:50:02  josh
 * updates to reach consistency with v3_ntfy.c
 *
 * Revision 1.1  1999/06/16 17:04:22  josh
 * First step of adding RFC2573 Notify support.
 *
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(v3_ntfy_inc))
#define v3_ntfy_inc

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

#define ETC_NOTIFY_MAX                 32
#define ETC_NOTIFY_FILTER_PROFILE_MAX  32
#define ETC_NOTIFY_FILTER_MAX          32
#define ETC_NOTIFY_FILTER_MASK_MAX     16

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct SNMP_NOTIFY_S
        {
        struct SNMP_NOTIFY_S *next;
        EBUFFER_T  notify_name;   /* Name of the notify */
        EBUFFER_T  notify_tag;    /* Tag notify is associated with */
        bits16_t   notify_type;   /* Trap or Inform? */
#define SNMP_NOTIFY_TRAP   1
#define SNMP_NOTIFY_INFORM 2
        bits16_t   storage;    
        bits16_t   status;
        bits16_t   flags;
        } SNMP_NOTIFY_T;

/* global root pointer for notify list */
extern SNMP_NOTIFY_T *root_notify;

#define SNMP_Notify_Set_Defaults(T)                \
      (MEMSET((T), 0, sizeof(SNMP_NOTIFY_T)),      \
       EBufferInitialize(&((T)->notify_name)),     \
       EBufferInitialize(&((T)->notify_tag)),      \
       (T)->notify_type = SNMP_NOTIFY_TRAP,        \
       (T)->storage = ETC_STO_NONVOL,              \
       (T)->status = ETC_RS_NREADY,                \
       (T)->flags = 0)
          
extern SNMP_NOTIFY_T * SNMP_Notify_Create  __((void));
extern void SNMP_Notify_Destroy __((SNMP_NOTIFY_T *));
extern int  SNMP_Notify_Install __((SNMP_NOTIFY_T *, bits8_t *,
                                    ALENGTH_T));
extern void SNMP_Notify_Deinstall __((SNMP_NOTIFY_T *));
extern SNMP_NOTIFY_T * SNMP_Notify_Lookup __((bits8_t *, ALENGTH_T));
extern SNMP_NOTIFY_T * SNMP_Notify_Next __((int, OIDC_T *));
extern SNMP_NOTIFY_T * SNMP_Notify_Next_Notify __((SNMP_NOTIFY_T *));
extern void SNMP_Notify_Name __((SNMP_NOTIFY_T *, bits8_t *, 
				 ALENGTH_T *));

#define SNMP_Notify_Get_Name(N)       (EBufferStart(&((N)->notify_name)))
#define SNMP_Notify_Get_Name_Len(N)   (EBufferUsed(&((N)->notify_name)))

#define SNMP_Notify_Get_Tag(N)       (EBufferStart(&((N)->notify_tag)))
#define SNMP_Notify_Get_Tag_Len(N)   (EBufferUsed(&((N)->notify_tag)))
#define SNMP_Notify_Set_Tag(N, S, L, F) \
        (EBufferAllocateLoad((F), &((N)->notify_tag), (S), (L)))

#define SNMP_Notify_Get_Type(N)      ((N)->notify_type)
#define SNMP_Notify_Set_Type(N, V)   ((N)->notify_type = (bits16_t)(V))

#define SNMP_Notify_Get_Storage(N)      ((N)->storage)
#define SNMP_Notify_Set_Storage(N, V)   ((N)->storage = (bits16_t)(V))

#define SNMP_Notify_Get_Status(N)      ((N)->status)
#define SNMP_Notify_Set_Status(N, V)   ((N)->status = (bits16_t)(V))

#define SNMP_Notify_Get_Flags(N)      ((N)->flags)
#define SNMP_Notify_Set_Flags(N, V)   ((N)->flags = (bits16_t)(V))

typedef struct SNMP_NOTIFY_FILTER_PROFILE_S
        {
        struct SNMP_NOTIFY_FILTER_PROFILE_S *next;
        EBUFFER_T  params_name;   /* From snmpTargetParamsTable */
        EBUFFER_T  profile_name;  /* Name of filter profile */
        bits16_t   storage;
        bits16_t   status;
        bits16_t   flags;
        } SNMP_NOTIFY_FILTER_PROFILE_T;

#define ETC_NPROF_PROFILE   0x01
#define ETC_NPROF_ALL_FLAGS 0x01

/* global root pointer for notify filter profile list */
extern SNMP_NOTIFY_FILTER_PROFILE_T *root_notify_filter_profile;

#define SNMP_Notify_Profile_Set_Defaults(T)           \
      (MEMSET((T), 0, sizeof(SNMP_NOTIFY_FILTER_PROFILE_T)), \
       EBufferInitialize(&((T)->params_name)),               \
       EBufferInitialize(&((T)->profile_name)),              \
       (T)->storage = ETC_STO_NONVOL,                        \
       (T)->status = ETC_RS_NREADY,                          \
       (T)->flags = 0)
          
extern SNMP_NOTIFY_FILTER_PROFILE_T * SNMP_Notify_Profile_Create  
      __((void));
extern void SNMP_Notify_Profile_Destroy 
      __((SNMP_NOTIFY_FILTER_PROFILE_T *));
extern int  SNMP_Notify_Profile_Install 
      __((SNMP_NOTIFY_FILTER_PROFILE_T *, bits8_t *, ALENGTH_T));
extern void SNMP_Notify_Profile_Deinstall 
      __((SNMP_NOTIFY_FILTER_PROFILE_T *));
extern SNMP_NOTIFY_FILTER_PROFILE_T * SNMP_Notify_Profile_Lookup 
      __((bits8_t *, ALENGTH_T));
extern SNMP_NOTIFY_FILTER_PROFILE_T * SNMP_Notify_Profile_Next 
      __((int, OIDC_T *));
extern SNMP_NOTIFY_FILTER_PROFILE_T * SNMP_Notify_Profile_Next_Profile
      __((SNMP_NOTIFY_FILTER_PROFILE_T *));
extern void SNMP_Notify_Profile_Name 
      __((SNMP_NOTIFY_FILTER_PROFILE_T *, bits8_t *, ALENGTH_T *));

#define SNMP_Notify_Profile_Get_Params(N)  \
        (EBufferStart(&((N)->params_name)))
#define SNMP_Notify_Profile_Get_Params_Len(N) \
        (EBufferUsed(&((N)->params_name)))

#define SNMP_Notify_Profile_Get_Profile(N)  \
        (EBufferStart(&((N)->profile_name)))
#define SNMP_Notify_Profile_Get_Profile_Len(N) \
        (EBufferUsed(&((N)->profile_name)))
#define SNMP_Notify_Profile_Set_Profile(N, S, L, F) \
        (EBufferAllocateLoad((F), &((N)->profile_name), (S), (L)))

#define SNMP_Notify_Profile_Get_Storage(N)      ((N)->storage)
#define SNMP_Notify_Profile_Set_Storage(N, V)   ((N)->storage = (bits16_t)(V))

#define SNMP_Notify_Profile_Get_Status(N)      ((N)->status)
#define SNMP_Notify_Profile_Set_Status(N, V)   ((N)->status = (bits16_t)(V))

#define SNMP_Notify_Profile_Get_Flags(N)      ((N)->flags)
#define SNMP_Notify_Profile_Set_Flags(N, V)   ((N)->flags = (bits16_t)(V))

typedef struct SNMP_NOTIFY_FILTER_S
        {
        struct SNMP_NOTIFY_FILTER_S *next;
        EBUFFER_T  profile_name;  /* From snmpNotifyFilterProfileTable */
        OBJ_ID_T   subtree;       /* MIB subtree for this filter */
        EBUFFER_T  filter_mask;   /* Mask for subtree */
        bits16_t   filter_type;   /* include or exclude filter */
#define SNMP_NOTIFY_FILTER_INCLUDED 1
#define SNMP_NOTIFY_FILTER_EXCLUDED 2
        bits16_t   storage;
        bits16_t   status;
        bits16_t   flags;
        } SNMP_NOTIFY_FILTER_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

/* global root pointer for notify filter list */
extern SNMP_NOTIFY_FILTER_T *root_notify_filter;

#define SNMP_Notify_Filter_Set_Defaults(T)             \
      (MEMSET((T), 0, sizeof(SNMP_NOTIFY_FILTER_T)),   \
       EBufferInitialize(&((T)->profile_name)) ,       \
       EBufferInitialize(&((T)->filter_mask)),         \
       (T)->filter_type = SNMP_NOTIFY_FILTER_INCLUDED, \
       (T)->storage = ETC_STO_NONVOL,                  \
       (T)->status = ETC_RS_NREADY,                    \
       (T)->flags = 0)
          
extern SNMP_NOTIFY_FILTER_T * SNMP_Notify_Filter_Create __((void));
extern void SNMP_Notify_Filter_Destroy __((SNMP_NOTIFY_FILTER_T *));
extern int  SNMP_Notify_Filter_Install __((SNMP_NOTIFY_FILTER_T *, bits8_t *, 
					   ALENGTH_T, OBJ_ID_T *));
extern void SNMP_Notify_Filter_Deinstall __((SNMP_NOTIFY_FILTER_T *));
extern SNMP_NOTIFY_FILTER_T * SNMP_Notify_Filter_Lookup 
      __((bits8_t *, ALENGTH_T, OBJ_ID_T *));
extern SNMP_NOTIFY_FILTER_T * SNMP_Notify_Filter_Next __((int, OIDC_T *));
extern SNMP_NOTIFY_FILTER_T * SNMP_Notify_Filter_Next_Filter
      __((SNMP_NOTIFY_FILTER_T *));
extern void SNMP_Notify_Filter_Name __((SNMP_NOTIFY_FILTER_T *, 
					bits8_t *, ALENGTH_T *,
					OIDC_T **, int *));

#define SNMP_Notify_Filter_Get_Profile(N) \
        (EBufferStart(&((N)->profile_name)))
#define SNMP_Notify_Filter_Get_Profile_Len(N) \
        (EBufferUsed(&((N)->profile_name)))

#define SNMP_Notify_Filter_Get_Subtree(N) (&((N)->subtree))

#define SNMP_Notify_Filter_Get_Mask(N) \
        (EBufferStart(&((N)->filter_mask)))
#define SNMP_Notify_Filter_Get_Mask_Len(N) \
        (EBufferUsed(&((N)->filter_mask)))
#define SNMP_Notify_Filter_Set_Mask(N, S, L, F) \
        (EBufferAllocateLoad((F), &((N)->filter_mask), (S), (L)))

#define SNMP_Notify_Filter_Get_Type(N)     ((N)->filter_type)
#define SNMP_Notify_Filter_Set_Type(N, V)  ((N)->filter_type = (bits16_t)(V))

#define SNMP_Notify_Filter_Get_Storage(N)      ((N)->storage)
#define SNMP_Notify_Filter_Set_Storage(N, V)   ((N)->storage = (bits16_t)(V))

#define SNMP_Notify_Filter_Get_Status(N)      ((N)->status)
#define SNMP_Notify_Filter_Set_Status(N, V)   ((N)->status = (bits16_t)(V))

#define SNMP_Notify_Filter_Get_Flags(N)      ((N)->flags)
#define SNMP_Notify_Filter_Set_Flags(N, V)   ((N)->flags = (bits16_t)(V))

extern void snmpNotifyEntry_get    __((OIDC_T, int, OIDC_T *,
				       SNMP_PKT_T *, VB_T *));

extern void snmpNotifyEntry_next    __((OIDC_T, int, OIDC_T *,
					SNMP_PKT_T *, VB_T *));

extern void snmpNotifyEntry_test    __((OIDC_T, int, OIDC_T *,
					SNMP_PKT_T *, VB_T *));

extern void snmpNotifyEntry_set    __((OIDC_T, int, OIDC_T *,
				       SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterProfileEntry_get    __((OIDC_T, int, OIDC_T *,
						    SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterProfileEntry_next    __((OIDC_T, int, OIDC_T *,
						    SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterProfileEntry_test    __((OIDC_T, int, OIDC_T *,
						     SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterProfileEntry_set    __((OIDC_T, int, OIDC_T *,
						    SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterEntry_get    __((OIDC_T, int, OIDC_T *,
					     SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterEntry_next    __((OIDC_T, int, OIDC_T *,
					      SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterEntry_test    __((OIDC_T, int, OIDC_T *,
					      SNMP_PKT_T *, VB_T *));

extern void snmpNotifyFilterEntry_set    __((OIDC_T, int, OIDC_T *,
					     SNMP_PKT_T *, VB_T *));

/* The leaf definitions for the method routines */
#define LEAF_snmpNotifyName	1
#define MINSIZE_snmpNotifyName	1L
#define MAXSIZE_snmpNotifyName	32L
#define LEAF_snmpNotifyTag	2
#define MINSIZE_snmpNotifyTag	0L
#define MAXSIZE_snmpNotifyTag	255L
#define LEAF_snmpNotifyType	3
#define VAL_snmpNotifyType_trap	1L
#define VAL_snmpNotifyType_inform	2L
#define LEAF_snmpNotifyStorageType	4
#define VAL_snmpNotifyStorageType_other	1L
#define VAL_snmpNotifyStorageType_volatile	2L
#define VAL_snmpNotifyStorageType_nonVolatile	3L
#define VAL_snmpNotifyStorageType_permanent	4L
#define VAL_snmpNotifyStorageType_readOnly	5L
#define LEAF_snmpNotifyRowStatus	5
#define VAL_snmpNotifyRowStatus_active	1L
#define VAL_snmpNotifyRowStatus_notInService	2L
#define VAL_snmpNotifyRowStatus_notReady	3L
#define VAL_snmpNotifyRowStatus_createAndGo	4L
#define VAL_snmpNotifyRowStatus_createAndWait	5L
#define VAL_snmpNotifyRowStatus_destroy	6L
#define LEAF_snmpNotifyFilterProfileName	1
#define MINSIZE_snmpNotifyFilterProfileName	1L
#define MAXSIZE_snmpNotifyFilterProfileName	32L
#define LEAF_snmpNotifyFilterProfileStorType	2
#define VAL_snmpNotifyFilterProfileStorType_other	1L
#define VAL_snmpNotifyFilterProfileStorType_volatile	2L
#define VAL_snmpNotifyFilterProfileStorType_nonVolatile	3L
#define VAL_snmpNotifyFilterProfileStorType_permanent	4L
#define VAL_snmpNotifyFilterProfileStorType_readOnly	5L
#define LEAF_snmpNotifyFilterProfileRowStatus	3
#define VAL_snmpNotifyFilterProfileRowStatus_active	1L
#define VAL_snmpNotifyFilterProfileRowStatus_notInService	2L
#define VAL_snmpNotifyFilterProfileRowStatus_notReady	3L
#define VAL_snmpNotifyFilterProfileRowStatus_createAndGo	4L
#define VAL_snmpNotifyFilterProfileRowStatus_createAndWait	5L
#define VAL_snmpNotifyFilterProfileRowStatus_destroy	6L
#define LEAF_snmpNotifyFilterSubtree	1
#define LEAF_snmpNotifyFilterMask	2
#define MINSIZE_snmpNotifyFilterMask	0L
#define MAXSIZE_snmpNotifyFilterMask	16L
#define LEAF_snmpNotifyFilterType	3
#define VAL_snmpNotifyFilterType_included	1L
#define VAL_snmpNotifyFilterType_excluded	2L
#define LEAF_snmpNotifyFilterStorageType	4
#define VAL_snmpNotifyFilterStorageType_other	1L
#define VAL_snmpNotifyFilterStorageType_volatile	2L
#define VAL_snmpNotifyFilterStorageType_nonVolatile	3L
#define VAL_snmpNotifyFilterStorageType_permanent	4L
#define VAL_snmpNotifyFilterStorageType_readOnly	5L
#define LEAF_snmpNotifyFilterRowStatus	5
#define VAL_snmpNotifyFilterRowStatus_active	1L
#define VAL_snmpNotifyFilterRowStatus_notInService	2L
#define VAL_snmpNotifyFilterRowStatus_notReady	3L
#define VAL_snmpNotifyFilterRowStatus_createAndGo	4L
#define VAL_snmpNotifyFilterRowStatus_createAndWait	5L
#define VAL_snmpNotifyFilterRowStatus_destroy	6L

#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(v3_ntfy_inc))*/
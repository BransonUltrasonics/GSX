/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/view.h,v 1.2 2001/11/06 21:35:08 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1995-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: view.h,v $
 * Revision 1.2  2001/11/06 21:35:08  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:25  tneale
 * Tornado shuffle
 *
 * Revision 9.5  2001/04/25 16:40:05  josh
 * make sure the instance is thoroughly checked in the
 * vacmViewTreeFamilyTable before trying to create a
 * new row
 *
 * Revision 9.4  2001/01/19 22:22:13  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:49  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:37  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 17:14:33  tneale
 * Added #ifdef for C++ to extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:31  sar
 * Update version stamp to match release
 *
 * Revision 8.4  1998/06/20 17:49:44  sar
 * Modified the names and calling sequences for snmp_rfc2275 functions
 *
 * Revision 8.3  1998/06/18 04:44:20  sar
 * Update view index to directly point to the name instead of using
 * an ebuffer
 *
 * Revision 8.2  1998/05/23 20:20:08  sar
 * added code for rfc2275, updated types
 *
 * Revision 8.1  1998/02/25 04:50:57  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:33  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:27  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 1.4  1995/11/14  22:42:26  sar
 * Added the prototype defs for the view method routines.
 *
 * Revision 1.3  1995/11/11  00:05:52  sar
 * removed use of rfc1445_view_routines
 *
 * Revision 1.2  1995/11/01  00:54:33  sar
 * removed casts of 0
 * changed the default row status to be nis instead of rsne
 *
 * Revision 1.1  1995/10/20  22:53:35  sar
 * Initial revision
 *
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(view_inc))
#define view_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#if (!defined(smi_inc))
#include <wrn/wm/snmp/engine/smi.h>
#endif

#define STO_OTHER  1
#define STO_VOL    2
#define STO_NONVOL 3
#define STO_PERM   4
#define STO_RONLY  5

#define RS_RSNE    0
#define RS_ACTIVE  1
#define RS_NIS     2
#define RS_NREADY  3
#define RS_CAGO    4
#define RS_CAWAIT  5
#define RS_DESTROY 6

/* define the view structures */
#define VIEW_INCLUDED 1
#define VIEW_EXCLUDED 2
#define VIEW_INDETERMINATE 3
#define VIEW_MAXMASKLEN 16
#define VIEW_MAXINDEX  65535
#define VIEW_MAXINDEXL 65535L

#define ETC_VIEW_NAME_MIN 1
#define ETC_VIEW_NAME_MAX 32

/* options for SNMP_View_Check */
#define VIEW_CHECK_DET   0
#define VIEW_CHECK_IND   1

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct VIEWLEAF_S
	{
	struct VIEWINDEX_S *parent;	/* backpointer to parent, 0 if
					   this view isn't installed */
	bits16_t        index;          /* index of the view table,
					   this is the same as it's parents
					   view index used for admin */
	OBJ_ID_T	subtree;	/* the oid for this subtree */
	EBUFFER_T	mask;		/* wildcard string */
	int		type;		/* is this entry included or exc */
	int		status;		/* status of this entry */
	int		storage;	/* type of storage of this entry */
	struct VIEWLEAF_S *  lexinext;	/* list of leaves in lexi order */
	struct VIEWLEAF_S *  masknext;	/* list of leaves in mask order */
	} VIEWLEAF_T;

typedef struct VIEWINDEX_S
	{
#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
	bits16_t	index;		/* index of the view table */
#endif

#if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
	bits8_t	    *	name;		/* name of the view table */
	ALENGTH_T	name_len;	/* length of name */
#endif

	struct VIEWINDEX_S *	next;	/* next index in the chain */
	VIEWLEAF_T  *	lexilist;	/* list of leaves in lexi order */
	VIEWLEAF_T  *	masklist;	/* list of leaves in mask order */
	} VIEWINDEX_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif


#if (INSTALL_ENVOY_SNMP_DYNAMIC_VIEWS)
extern int SNMP_View_Find_Family  __((SNMP_PKT_T *));
extern int SNMP_View_Family_Check __((SNMP_PKT_T *, OIDC_T *, int, int));
extern int SNMP_View_Check	  __((SNMP_PKT_T *, OIDC_T *, int, int));

#define SNMP_View_Get_Type(V)           ((V)->type)
#define SNMP_View_Set_Type(V, W)        ((V)->type = (bits16_t)W)
#define SNMP_View_Get_Mask(V)		(EBufferStart(&(V)->mask))
#define SNMP_View_Get_MaskLen(V)	(EBufferUsed(&(V)->mask))
#define SNMP_View_Set_StorageType(V, W) ((V)->storage = (bits16_t)W)
#define SNMP_View_Get_StorageType(V)    ((V)->storage)
#define SNMP_View_Set_Status(V, W)      ((V)->status = (bits16_t)W)
#define SNMP_View_Get_Status(V)         ((V)->status)
#define SNMP_View_Get_Subtree(V)        ((V)->subtree.component_list)
#define SNMP_View_Get_Subtree_Len(V)    ((V)->subtree.num_components)

#define SNMP_View_Set_Defaults(V) ((V)->parent    = 0, \
				   EBufferInitialize(&(V)->mask), \
				   (V)->masknext = 0, \
				   (V)->lexinext = 0, \
				   (V)->type     = VIEW_INCLUDED, \
				   (V)->storage  = STO_NONVOL, \
				   (V)->status   = RS_NIS)


extern VIEWLEAF_T *SNMP_View_Create        __((OIDC_T *, int));
extern void        SNMP_View_Delete        __((VIEWLEAF_T *));
extern VIEWLEAF_T *SNMP_View_Next	   __((VIEWLEAF_T *));
extern int         SNMP_View_Set_Mask      __((VIEWLEAF_T *, bits8_t *, int));

#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
#define SNMP_View_Get_Index(V)   ((V)->index)

extern int         SNMP_View_Install       __((bits16_t, VIEWLEAF_T *)); 
extern int         SNMP_View_Name	   __((VIEWLEAF_T *, bits16_t *,
					       OIDC_T *, int));
extern int         SNMP_View_Deinstall     __((bits16_t, OIDC_T *, int));
extern VIEWLEAF_T *SNMP_View_Lookup	   __((bits16_t, OIDC_T *, int));
#endif /* #if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */

#if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
/*#define SNMP_View_2275_Name(V)     ((V)->parent ? (V)->parent->name : 0)
#define SNMP_View_2275_Name_Len(V) ((V)->parent ? (V)->parent->name_len : 0)*/

extern void 	   SNMP_View_2275_Name       __((VIEWLEAF_T *, bits8_t *,
						 ALENGTH_T *));
extern int         SNMP_View_2275_Install    __((VIEWLEAF_T *, bits8_t *,
						 ALENGTH_T));
extern int	   SNMP_View_2275_Full_Name  __((VIEWLEAF_T *, OIDC_T *, int));
extern void        SNMP_View_2275_Deinstall  __((VIEWLEAF_T *));
extern VIEWLEAF_T *SNMP_View_2275_Lookup     __((bits8_t *, ALENGTH_T,
						 OIDC_T *, int)); 
extern sbits32_t   v3_vacmViewSpinLock;
#endif /* #if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS) */


extern void	   viewtable_test	   __((OIDC_T, int, OIDC_T *,
					       SNMP_PKT_T *, VB_T *));

extern void	   viewtable_get	   __((OIDC_T, int, OIDC_T *,
					       SNMP_PKT_T *, VB_T *));

extern void	   viewtable_next	   __((OIDC_T, int, OIDC_T *,
					       SNMP_PKT_T *, VB_T *));

extern void	   viewtable_set	   __((OIDC_T, int, OIDC_T *,
					       SNMP_PKT_T *, VB_T *));

#endif /* (INSTALL_ENVOY_SNMP_DYNAMIC_VIEWS) */

extern VIEWINDEX_T *viewroot;

#ifdef __cplusplus
}
#endif

#endif /* view_inc */


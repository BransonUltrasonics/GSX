/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/tree.h,v 1.2 2001/11/06 21:35:04 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: tree.h,v $
 * Revision 1.2  2001/11/06 21:35:04  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.4  2001/01/19 22:22:10  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:42  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:35  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 16:59:00  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:21  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:51  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:31  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:21  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:55  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/09/29  18:31:12  sar
 * Minor cleanup, adding other include files.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.1  1994/05/10  20:17:19  sar
 * Removed the #if that restricted it for use with v2 only
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.8  1993/06/02  23:05:55  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 1.7  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 1.6  1993/04/30  22:58:26  sar
 * Added mechanism for minimal proxies and coarse grained locks.
 *
 * Revision 1.5  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 1.4  1993/04/17  00:58:50  sar
 * changed def of TREE_Add to allow the user to determine what node the leaf
 * was attatched to.
 *
 * Revision 1.3  1993/03/05  03:56:25  dab
 * fixed up types
 *
 * Revision 1.2  1993/02/24  00:59:50  sar
 * Removed the name objid from the partyleaf and put a copy of it in the
 * pkt.  Also Updated many of the SNMP_(Party, Context, ACL)_* files
 * to use a OIDC_T/length instead of a OBJ_ID_T so we don't have to
 * allocate an OBJ_ID_T when we don't have one.
 *
 * Revision 1.1  1993/02/17  20:55:28  sar
 * Initial revision
 *
 * Revision 3.0  92/09/09  sar
*/

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#include <wrn/wm/snmp/engine/asn1.h>

#if (!defined(tree_inc))
#define tree_inc

#ifdef __cplusplus
extern"C" {
#endif

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

/* Define a generic tree structure */
typedef struct TREENODE_S
	{
	INT_32_T		status;		/* flag and status word */
	OIDC_T			name;		/* subid for this level */
	struct TREENODE_S *	parent;		/* ptr to one level up  */
	struct TREENODE_S *	sibling;	/* ptr to this level */
	PTR_T      		child;		/* ptr to one level down */
						/* or to the data block  */
	} TREENODE_T;
	
#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

#define TREESTATIC  0x1		/* Node may not be deleted, ptrs may change */
#define TREETWIN    0x2		/* Next node contains a leaf pointer */
#define TREECHILD   0x4		/* Node contains a child pointer */
#define TREELEAF    0x8		/* Node contains a leaf pointer */

extern int	TREE_Add	    __((TREENODE_T **root, OIDC_T *oidc,
					int oidclen, PTR_T leaf,
					TREENODE_T **retnode));
extern int	TREE_Delete	    __((TREENODE_T **root, TREENODE_T *node));
extern TREENODE_T *TREE_Get	    __((TREENODE_T *root, OIDC_T *oidc, 
					int oidclen, int *length));
extern TREENODE_T *TREE_GetPrev	    __((TREENODE_T *root, OIDC_T *oidc,
					int oidclen));
extern TREENODE_T *TREE_GetNext     __((TREENODE_T *root, TREENODE_T *node));
extern int	TREE_Extract     __((TREENODE_T *node, PTR_T *leaf));
extern int	TREE_Install     __((TREENODE_T *node, PTR_T leaf));
extern int	TREE_Name        __((TREENODE_T *node, OIDC_T *oidc, int len));

#ifdef __cplusplus
}
#endif

#endif /* tree_inc */

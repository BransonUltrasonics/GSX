/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/mib.h,v 1.2 2001/11/06 21:35:02 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: mib.h,v $
 * Revision 1.2  2001/11/06 21:35:02  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.9.4.1  2001/08/24 14:11:25  josh
 * make sure that standard declaration of NODE_NT zeroes all fields
 *
 * Revision 9.9  2001/04/11 20:12:00  josh
 * merging changes from the kingfisher branch back onto
 * the trunk
 *
 * Revision 9.8  2001/01/19 22:22:05  paul
 * Update copyright.
 *
 * Revision 9.7.2.2  2001/03/12 22:07:54  tneale
 * Updated copyright
 *
 * Revision 9.7.2.1  2000/10/13 21:17:33  josh
 * function prototypes and static declarations to eliminate warnings
 * from the Tornado compiler
 *
 * Revision 9.7  2000/03/17 19:48:35  meister
 * Update copyright notice
 *
 * Revision 9.6  2000/03/09 20:58:33  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.5  2000/03/09 16:56:42  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.4  1999/12/23 14:47:26  josh
 * cleaning up code per Shawn's comments -- removing extraneous
 * comments and doing proxy check before notify check
 *
 * Revision 9.3  1999/11/12 23:20:28  josh
 * fixing synchronous function prototypes
 *
 * Revision 9.2  1999/10/26 20:07:33  josh
 * fix indeterminate function prototyping with forward declarations
 *
 * Revision 9.1  1999/05/24 20:12:11  josh
 * 'compc' and 'compl' have been replaced by 'tcount' and 'tlist'
 * to avoid C++ compiler errors.
 *
 * Revision 9.0  1998/10/16 22:10:15  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:40  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:26  sar
 * Updated revision to 8.0
 *
 * Revision 7.6  1997/10/29 01:33:28  sar
 * Only define the two character macros ("NO", "RW" etc) if the user
 * asks for them via an installation option.
 *
 * Revision 7.5  1997/10/22 03:07:29  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.4  1997/10/16 00:25:17  sar
 * Primarily this set of modifications is for the agentx protocol.
 * Most of the work was in the agentx.h and ax_mth.h files.  Most
 * other files (snmpdefs.h, vbdefs.h and mib.h) received updates
 * to their structures and snmp got some new lock macros.
 *
 * One other change of note was the modification of continue
 * processing.  Previously we called a specific continue routine
 * directly, now we store a pointer to the corret continue
 * function in the snmp packet structure and call that.  We include
 * macros for backwards compatibility.
 *
 * Revision 7.3  1997/09/16 19:19:23  sar
 * Added the NT macros for leaves and nodes, these add a flag
 * word to allow the mib compiler to set some flags in the mib
 * tree structure.  Emissary 7.0 can make use of these macros.
 *
 * Revision 7.2  1997/03/20 06:48:13  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:58  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/03  21:01:22  sar
 * Added CREATE_ACCESS & NOTIFY_ACCESS
 *
 * Revision 5.3  1995/03/24  17:35:34  sar
 * Added scalar/tabular arg to the leaf_asy and leaf_com macros, currently
 * we just throw it away.
 *
 * Revision 5.2  1995/03/20  23:11:33  sar
 * Added the ASY_*PROC_T method routines
 * Updated the MIBLEAF_T structure to use the async method routine typedefs
 * Added a compatibility structure for backwards compatibility
 * Added a new series of macros to populate the mibleaf and compat structure.
 *
 * Revision 5.1  1994/08/04  22:05:29  sar
 * Added multiple mib tree capability, including macros to translate the
 * old apis into the new api's
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.3  1994/04/29  20:06:04  sar
 * Added LEAF_INACTIVE flag and modified calling sequence for Find_Node.
 *
 * Revision 4.2  1994/02/01  23:37:07  sar
 * Added flags for use with the Find, Add and Remove_Node routines.
 *
 * Revision 4.1  1993/09/30  19:32:16  sar
 * Added a definition for TEST_CREATE in the list of defines for
 * the form parameter to the testproc routine.  TEST_CREATE specified
 * that this call is to check if the given object can be created.
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.2  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 3.1  1993/03/02  18:16:06  dab
 * Removed __() macro since this is defined in <types.h>
 *
 * Revision 3.0  1992/04/03  19:52:27  dab
 * Release 3.0
 *
 * Revision 2.103  92/01/30  14:00:17  dab
 * Added function parameter prototypes for the test, get, set, and next
 * functions.  Also added casts inthe LEAF and LEAFM macros to cast the
 * routines to those prototypes.
 * 
 * Revision 2.102  91/12/05  15:02:59  dab
 * LEAF macro initializes mib view masks to 0xFFFFFFFF instead of just 0xFF.
 * 
 * Revision 2.101  91/11/07  09:37:23  dab
 * Made the view and write masks be 32 bits.
 * 
 * Revision 2.100  91/08/09  14:09:47  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:28  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:24
 * Release 2.00
 * 
 *    Rev 1.8   24 Sep 1989 22:06:34
 * Renamed mib_root to mib_root_node to support the MIB compiler.
 * 
 *    Rev 1.7   11 Jan 1989 11:59:48
 * Updated copyright dates.
 * 
 *    Rev 1.6   11 Jan 1989 11:09:20
 * Added definitions of find_object_node() and find_next_object() for non-
 * ANSI compilers.
 * 
 *    Rev 1.5   10 Oct 1988 21:47:00
 * Reorganized source modules
 * 
 *    Rev 1.4   21 Sep 1988 16:34:00
 * Revised spelling to conform to the usage "MGMT" in the MIB.
 * 
 *    Rev 1.3   20 Sep 1988 15:50:10
 * Revised END_OF_ARC macro to avoid an improper type cast.
 * 
 *    Rev 1.2   19 Sep 1988 19:56:06
 * Revised MIBARC structure and ARC macro to avoid casting structure
 * pointers to a (char *) which, on machines like the Cray, could
 * destroy the meaning of the pointer.
 * 
 *    Rev 1.1   19 Sep 1988 17:27:06
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:18
 * Initial revision.
*/

/* [clearcase]
modification history
-------------------
30may14,bbz  resolve snmp engine's dependence on snmp agent(V7MAN-82)
19apr05,job  update copyright notices
*/


#if (!defined(mib_inc))
#define mib_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

/* Definitions for "form" parameter to the "testproc" found in leaf nodes */
#define	TEST_GET	0
#define	TEST_SET	1
#define TEST_CREATE	2

/* Definitions for node_type in MIBNODE_T and MIBLEAF_T */
#define NODE_TYPE	0x01
#define	INNER_NODE	0x00
#define	LEAF_NODE	0x01
#define REMOVABLE	0x02
#define REPLACEABLE	0x04
#define SYSTEM_OWNED	0x08
#define NODE_DYNAMIC	0x10
#define LEAF_DYNAMIC	0x10
#define ARC_DYNAMIC	0x20
#define COOKIE_DYNAMIC	0x20
#define LEAF_INACTIVE	0x40
#define AGENTX_LEAF     0x80

struct SNMP_PKT_S;
struct VB_S;
typedef void ASY_TESTPROC_T __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				struct SNMP_PKT_S *pktp, struct VB_S *vbp));
typedef void ASY_GETPROC_T  __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				struct SNMP_PKT_S *pktp, struct VB_S *vbp));
typedef void ASY_SETPROC_T  __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				struct SNMP_PKT_S *pktp, struct VB_S *vbp));
typedef void ASY_NEXTPROC_T __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				struct SNMP_PKT_S *pktp, struct VB_S *vbp));

typedef int	 TESTPROC_T __((int form, OIDC_T lastmatch, int tcount,
				OIDC_T *tlist, char *cookie, 
				struct SNMP_PKT_S *pktp, int indx));
typedef INT_32_T GETPROC_T  __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				char *cookie,
				struct SNMP_PKT_S *pktp));
typedef void	 SETPROC_T  __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				char *cookie, INT_32_T value,
				struct SNMP_PKT_S *pktp, int vb_index));
typedef int	 NEXTPROC_T __((OIDC_T lastmatch, int tcount, OIDC_T *tlist,
				OIDC_T *rlist, char *cookie,
				struct SNMP_PKT_S *pktp));

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

/* Define a MIB leaf node */
typedef	struct	MIBLEAF_S
	{
	unsigned short	node_type;	/* See above			*/
	OCTET_T		expected_tag;	/* VT_xxx from snmp.h		*/
	OCTET_T		access_type;	/* See below			*/

	ASY_TESTPROC_T *testproc;	/* Routine to check whether the	*/
					/* indicated data object exists */
	ASY_GETPROC_T  *getproc; 	/* Routine to get the data	*/
	ASY_SETPROC_T  *setproc;	/* Routine to set the data	*/
	ASY_NEXTPROC_T *nextproc;	/* Routine to locate "next" obj id  */
	char *		user_cookie;	/* Value to pass to unchanged	    */
	UINT_16_T	locator;	/* The user can define any value for*/
					/*  this field.  It is useful to    */
					/*  give easy to handle names to    */
					/*  leaves.			    */
	UINT_32_T	view_mask;	/* A mask of the views of which this*/
					/* leaf is a part.		    */
	UINT_32_T	write_mask;	/* That subset of view_mask in which*/
					/* is potentially writable.	    */
#if INSTALL_ENVOY_AGENTX_MASTER
	bits32_t        session_id;     /* session for this object */
	bits16_t        ref_count;      /* reference count              */
	bits8_t         timeout;        /* timeout period               */
#endif

#if defined(DEBUG)
	char *		object_name;	/* FOR DEBUGGING		    */
#endif
	} MIBLEAF_T;

/* Bit values for access_type */
#define	READ_ACCESS	0x01
#define	WRITE_ACCESS	0x02
#define CREATE_ACCESS	0x04
#define NOTIFY_ACCESS	0x08
#define READ_WRITE_ACCESS (READ_ACCESS | WRITE_ACCESS)
#define READ_CREATE_ACCESS (READ_ACCESS | WRITE_ACCESS | CREATE_ACCESS)

#if INSTALL_ENVOY_TWO_CHAR_MACROS
#define	RO	READ_ACCESS			/* READ ONLY ACCESS	*/
#define	WO	WRITE_ACCESS			/* WRITE ONLY ACCESS	*/
#define	RW	(READ_ACCESS | WRITE_ACCESS)	/* READ/WRITE ACCESS	*/
#define RC	(READ_ACCESS | WRITE_ACCESS | CREATE_ACCESS) /* READ/CREATE */
#define NO	NOTIFY_ACCESS			/* NOTIFY ONLY ACCESS   */
#endif

/* Bit values for scalar vs tabular type */
#define SCALAR  0x80
#define TABULAR 0x00

/* maximum size for a reference count */
#define AX_MAX_REF_COUNT 0xFFFF

/* Define an MIB internal (or root) node */
typedef	struct	MIBNODE_S
	{
	unsigned short	node_type;	/* See above			    */
	struct MIBARC_S	*arcs;		/* Descendents from this node	    */
#if INSTALL_ENVOY_AGENTX_MASTER
	MIBLEAF_T       *leaf;		/* leaf for this node */
#endif
	} MIBNODE_T;

/* Define a pointer from a node to subsidiary node or a leaf	*/
/* These structures are aggregated into an array.		*/
/* THE LAST ELEMENT IN SUCH AN ARRAY MUST HAVE A NULL POINTER.	*/
typedef	struct	MIBARC_S
	{
	OIDC_T		id;		/* Object identifier component	*/
	struct MIBARC_S *nodep;		/* The descendent node/leaf	*/
	} MIBARC_T;

/* Define a cookie structure to hold old style method routines for use with
   the backwards compatibility method routines */
typedef struct COMPAT_ROUTINES_S 
	{
	TESTPROC_T *	testproc;
	NEXTPROC_T *	nextproc;
	GETPROC_T  *	getproc;
	SETPROC_T  *	setproc;
	char	   *	cookie;
	} COMPAT_ROUTINES_T;

#define	MIB_ISO		1	/* Level 0 - ISO *IS* #1 by fiat (!!)	*/
#define	MIB_CCITT	2	/* Level 0				*/
#define	MIB_JOINT_ISO_CCITT	3	/* Level 0			*/
#define	MIB_ORG		3	/* Level 1 - {ISO 3}			*/
#define	MIB_DOD		6	/* Level 2 - {ORG 6}			*/
#define	MIB_INTERNET	1	/* Level 3 - {DOD 1}			*/


#define	MIB_DIRECTORY	1	/* Level 4 - {INTERNET 1}		*/
#define	MIB_MGMT	2	/* Level 4 - {INTERNET 2}		*/
#define	MIB_MIB2	1	/* Level 5 - {MGMT 1}			*/
#define	MIB_MIB1	1	/* Level 5 - {MGMT 1}			*/
#define	MIB_SYSTEM	1	/* Level 6 - {MIB 1}			*/
#define	MIB_INTERFACES	2	/* Level 6 - {MIB 2}			*/
#define	MIB_AT		3	/* Level 6 - {MIB 3}			*/
#define	MIB_IP		4	/* Level 6 - {MIB 4}			*/
#define	MIB_ICMP	5	/* Level 6 - {MIB 5}			*/
#define	MIB_TCP		6	/* Level 6 - {MIB 6}			*/
#define	MIB_UDP		7	/* Level 6 - {MIB 7}			*/
#define	MIB_EGP		8	/* Level 6 - {MIB 8}			*/

#define	MIB_EXPERIMENTAL	3	/* Level 4 - {INTERNET 3}	*/
#define	MIB_IETF	1	/* Level 5 - {EXPERIMENTAL 1}		*/ 
#define	MIB_PRIVATE	4	/* Level 4 - {INTERNET 4}		*/
#define	MIB_ENTERPRISE	1	/* Level 5 - {PRIVATE 1}		*/


#define ARC(ID,NODE)		   {ID, (struct MIBARC_S *)&NODE}
#define END_OF_ARC_LIST		   {0, (struct MIBARC_S *)0}
#define NODE(NAME, ARCLIST)	   {INNER_NODE, ARCLIST}
#if INSTALL_ENVOY_AGENTX_MASTER
#define NODE_NT(NAME, NT, ARCLIST) {(INNER_NODE | NT), ARCLIST, 0}
#else
#define NODE_NT(NAME, NT, ARCLIST) {(INNER_NODE | NT), ARCLIST}
#endif
#define SNMP_NULLPROC		   (int (*)())0

/* The LEAF macro is for release 1 mib.c and MIB Compiler */
#define	LEAF(NAME, VT, ACCESS, TESTP, GETP, SETP, NXTP, COOKIE)	\
				{LEAF_NODE, VT, ACCESS,		\
				(TESTPROC_T *)TESTP, (GETPROC_T *)GETP, \
				(SETPROC_T *)SETP, (NEXTPROC_T *)NXTP,	\
				(char *)COOKIE, 0, 0xFFFFFFFFL, 0xFFFFFFFFL}

/* The LEAFM macro is for release 2 mib.c and MIB Compiler */
#define	LEAFM(NM, VT, ACC, TSTP, GETP, SETP, NXTP, CKE, LOC, VMSK, WMSK)  \
				{LEAF_NODE, VT, ACC,			\
				(TESTPROC_T *)TSTP, (GETPROC_T *)GETP, \
				(SETPROC_T *)SETP, (NEXTPROC_T *)NXTP,	\
				(char *)CKE, LOC, VMSK, WMSK }

/* The LEAF_ASYS macro is for release envoy release 6 and is for use with
   the async routines, it tags the leaf as static */
#define LEAF_ASYS(LN, NM, VT, AC, TB, TST, GET, SET, NEX, CKE, LOC, VMK, WMK) \
STATIC MIBLEAF_T	 LN = {LEAF_NODE, VT, AC,			    \
			       (ASY_TESTPROC_T *)TST, (ASY_GETPROC_T *)GET, \
			       (ASY_SETPROC_T *)SET, (ASY_NEXTPROC_T *)NEX, \
			       (char *)CKE, LOC, VMK, WMK }

/* The LEAF_ASY macro is for release envoy release 6 and is for use with
   the async routines */
#define LEAF_ASY(LN, NM, VT, AC, TB, TST, GET, SET, NEX, CKE, LOC, VMK, WMK) \
MIBLEAF_T	 LN = {LEAF_NODE, VT, AC,			    \
		       (ASY_TESTPROC_T *)TST, (ASY_GETPROC_T *)GET, \
		       (ASY_SETPROC_T *)SET, (ASY_NEXTPROC_T *)NEX, \
		       (char *)CKE, LOC, VMK, WMK }

/* The LEAFCOMS macro is for envoy release 6 and is for use with the async
   compatibility routines.  The compat routines allow older non-async method
   routines to be run from a newer async engine */
#define LEAF_COMS(LN, CN, NM, VT, AC, TB, TS, GT, ST, NX, CKE, LOC, VMK, WMK) \
STATIC COMPAT_ROUTINES_T CN = {(TESTPROC_T *)TS, (NEXTPROC_T *)NX,	\
			       (GETPROC_T *)GT,  (SETPROC_T *)ST,	\
			       (char *)CKE };				\
STATIC MIBLEAF_T	 LN = {LEAF_NODE, VT, AC,			\
			       (ASY_TESTPROC_T *)async_compat_test,	\
			       (ASY_GETPROC_T *)async_compat_get,	\
			       (ASY_SETPROC_T *)async_compat_set,	\
			       (ASY_NEXTPROC_T *)async_compat_next,	\
			       (char *)&CN, LOC, VMK, WMK }

#define LEAF_COM(LN, CN, NM, VT, AC, TB, TS, GT, ST, NX, CKE, LOC, VMK, WMK) \
STATIC COMPAT_ROUTINES_T CN = {(TESTPROC_T *)TS, (NEXTPROC_T *)NX,	\
			       (GETPROC_T *)GT,  (SETPROC_T *)ST,	\
			       (char *)CKE };				\
MIBLEAF_T		 LN = {LEAF_NODE, VT, AC,			\
			       (ASY_TESTPROC_T *)async_compat_test,	\
			       (ASY_GETPROC_T *)async_compat_get,	\
			       (ASY_SETPROC_T *)async_compat_set,	\
			       (ASY_NEXTPROC_T *)async_compat_next,	\
			       (char *)&CN, LOC, VMK, WMK }

/* The LEAF NT macros are for emissary release 7 and include a flag
   argument to allow setting of the removable class flags */
#define LEAF_NT_ASYS(LN, NT, NM, VT, AC, TB, TS, GT, ST, NX, CK, LC, VM, WM) \
STATIC MIBLEAF_T	 LN = {(LEAF_NODE | NT), VT, AC,		    \
			       (ASY_TESTPROC_T *)TS, (ASY_GETPROC_T *)GT, \
			       (ASY_SETPROC_T *)ST, (ASY_NEXTPROC_T *)NX, \
			       (char *)CK, LC, VM, WM }

#define LEAF_NT_ASY(LN, NT, NM, VT, AC, TB, TS, GT, ST, NX, CK, LC, VM, WM) \
MIBLEAF_T	 LN = {(LEAF_NODE | NT), VT, AC,			    \
		       (ASY_TESTPROC_T *)TS, (ASY_GETPROC_T *)GT, \
		       (ASY_SETPROC_T *)ST, (ASY_NEXTPROC_T *)NX, \
		       (char *)CK, LC, VM, WM }

/* The COM macros are for use with the async compatibility routines.
   The compat routines allow older non-async method routines to
   be run from a newer async engine */
#define LEAF_NT_COMS(LN, CN, NT, NM, VT, AC, TB, TS, GT, ST, NX, CK, LC, VM, WM) \
STATIC COMPAT_ROUTINES_T CN = {(TESTPROC_T *)TS, (NEXTPROC_T *)NX,	\
			       (GETPROC_T *)GT,  (SETPROC_T *)ST,	\
			       (char *)CK };				\
STATIC MIBLEAF_T	 LN = {(LEAF_NODE | NT), VT, AC,		\
			       (ASY_TESTPROC_T *)async_compat_test,	\
			       (ASY_GETPROC_T *)async_compat_get,	\
			       (ASY_SETPROC_T *)async_compat_set,	\
			       (ASY_NEXTPROC_T *)async_compat_next,	\
			       (char *)&CN, LC, VM, WM }

#define LEAF_NT_COM(LN, CN, NT, NM, VT, AC, TB, TS, GT, ST, NX, CK, LC, VM, WM) \
STATIC COMPAT_ROUTINES_T CN = {(TESTPROC_T *)TS, (NEXTPROC_T *)NX,	\
			       (GETPROC_T *)GT,  (SETPROC_T *)ST,	\
			       (char *)CK };				\
MIBLEAF_T		 LN = {(LEAF_NODE | NT), VT, AC,		\
			       (ASY_TESTPROC_T *)async_compat_test,	\
			       (ASY_GETPROC_T *)async_compat_get,	\
			       (ASY_SETPROC_T *)async_compat_set,	\
			       (ASY_NEXTPROC_T *)async_compat_next,	\
			       (char *)&CN, LC, VM, WM }



typedef	struct	MIBLOC_S
	{
	UINT_16_T		ml_flags;
	OIDC_T			ml_last_match;
	/* The following item splits out the "instance" part of the	*/
	/* full object identifier.  The number of components in the	*/
	/* "base" part (i.e. the path to the leaf may be derived by	*/
	/* vb_obj_id.num_components - ml_base_objid.num_components and	*/
	/* the list of components being taken directly from		*/
	/* vb_obj_id.component_list.					*/
	OBJ_ID_T		ml_remaining_objid;
	union {
	      MIBLEAF_T		*mlleaf_u;
	      MIBNODE_T		*mlnode_u;
	      } mbl_u;
	} MIBLOC_T;
#define	ml_leaf		mbl_u.mlleaf_u
#define	ml_node		mbl_u.mlnode_u

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

/* Values for ml_flags */
#define	ML_IS_LEAF	0x01

extern  MIBNODE_T*	    p_mib_root_node;

extern	INT_32_T	Find_Node_From_Root   __((MIBNODE_T *root,
						  OBJ_ID_T  *objp,
						  MIBNODE_T **npp));

extern	int		Add_Node_From_Root    __((MIBNODE_T *root,
						  OBJ_ID_T  *objp,
						  MIBNODE_T *anode,
						  MIBNODE_T **rnode));

extern  MIBNODE_T *	Remove_Node_From_Root __((MIBNODE_T *root,
						  OBJ_ID_T  *objp));

#if (INSTALL_ENVOY_AGENTX_MASTER)
extern sbits32_t        Find_Leaf_From_Root   __((MIBNODE_T *mibroot,
                                                  OBJ_ID_T  *objid,
                                                  MIBLEAF_T **mibleaf));

extern int              AX_Add_Leaf_From_Root __((MIBNODE_T *mibroot,
                                                  OBJ_ID_T  *objp,
                                                  MIBLEAF_T *add_leaf,
                                                  MIBLEAF_T **rem_leaf));

extern MIBLEAF_T *      AX_Remove_Leaf_From_Root __((MIBNODE_T *mibroot,
                                                     OBJ_ID_T  *objid));

#endif /* #if (INSTALL_ENVOY_AGENTX_MASTER) */

#define Find_Node(O, N)   Find_Node_From_Root   (0, O, N)

#define	Add_Node(O, A, R) Add_Node_From_Root    (0, O, A, R)

#define Remove_Node(O)	  Remove_Node_From_Root (0, O)

#ifdef __cplusplus
}
#endif

#endif	/* mib_inc */



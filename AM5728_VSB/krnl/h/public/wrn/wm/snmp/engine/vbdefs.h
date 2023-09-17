/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/vbdefs.h,v 1.3 2002/05/13 20:46:19 josh Exp $ */

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
 * $Log: vbdefs.h,v $
 * Revision 1.3  2002/05/13 20:46:19  josh
 * encoding process now checks for 16-bit overflows in
 * calculating packet sizes
 *
 * Revision 1.2  2001/11/06 21:35:07  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:24  tneale
 * Tornado shuffle
 *
 * Revision 9.7  2001/01/19 22:22:13  paul
 * Update copyright.
 *
 * Revision 9.6  2000/03/17 19:48:48  meister
 * Update copyright notice
 *
 * Revision 9.5  2000/03/09 20:58:37  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.4  2000/03/09 17:14:20  tneale
 * Added #ifdef for C++ to extern C if needed
 *
 * Revision 9.3  2000/02/02 17:35:28  josh
 * adding VBList_Free() API call
 *
 * Revision 9.2  1999/12/23 14:47:28  josh
 * cleaning up code per Shawn's comments -- removing extraneous
 * comments and doing proxy check before notify check
 *
 * Revision 9.1  1999/05/24 20:12:12  josh
 * 'compc' and 'compl' have been replaced by 'tcount' and 'tlist'
 * to avoid C++ compiler errors.
 *
 * Revision 9.0  1998/10/16 22:10:28  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:54  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:33  sar
 * Updated revision to 8.0
 *
 * Revision 7.6  1997/10/26 01:44:43  sar
 * Always define the 64 bit counter and include it in the vb defintion
 * instead of forgetting about it if we don't have v2 or v2 types installed.
 *
 * Revision 7.5  1997/10/22 03:07:28  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.4  1997/10/16 23:52:27  sar
 * We changed the macro VB_GET_IP_ADDRESS to provide a more consistent
 * type signature from compilers.  Previously it could have been either
 * a pointer to an array of bytes or a pointer to a byte.  It should
 * now resolve to a pointer to an array of bytes.
 *
 * Revision 7.3  1997/10/16 00:25:19  sar
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
 * Revision 7.2  1997/03/20 06:48:25  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.2  1995/10/20  22:53:35  sar
 * removed bitstring include file & macro
 * removed nsapaddress
 * added VT_BITS & VT_UNSIGNED32
 *
 * Revision 6.1  1995/06/05  20:57:49  sar
 * Added clean_vb_data.
 *
 * Revision 6.0  1995/05/31  21:46:57  sra
 * Release 6.0.
 *
 * Revision 1.1  1995/03/20  23:07:47  sar
 * Initial revision
 *
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/

#if (!defined(vb_inc))
#define vb_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(buffer_inc))
#include <wrn/wm/snmp/engine/buffer.h>
#endif

#if (!defined(mib_inc))
#include <wrn/wm/snmp/engine/mib.h>
#endif

#if (!defined(ui64_inc))
#include <wrn/wm/snmp/engine/ui64.h>
#endif

/* Definition for routine to free private vb space */
typedef void VBPRIVPROC_T __((PTR_T));

/* Definition for routine to undo sets */
struct VB_S;
typedef void UNDOPROC_T __((OIDC_T lastmatch, int tcount, OIDC_T *tlist, 
			    struct SNMP_PKT_S *pktp, struct VB_S *vbp));

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

/* Define the internal shape of a VarBind	*/
typedef	struct VB_S
	{
	struct VB_S *	vb_link;	  /* For the user to use, usually   */
					  /*  to build a list of related    */
					  /*  VB_Ts.			    */
	ALENGTH_T	vb_seq_size;	  /* Overall length of the VarBind  */
					  /* sequence when encoded.	    */
	OBJ_ID_T	vb_obj_id;	  /* Object id for this VarBind	    */
	OCTET_T		vb_data_flags_n_type;  /* Class form & type of data */
	OCTET_T		vb_flags;	  /* See below			    */
	ALENGTH_T	vb_data_length;	  /* Length of the data when encode */
	MIBLOC_T	vb_ml;		  /* The mib-leaf associated with   */
					  /* this VarBind.		    */
	union	{
		INT_32_T	v_number;	/* Integer kinds of things  */
		UINT_32_T	v_counter;	/* Unsigned int things	    */
		EBUFFER_T	v_string;	/* Octetstring things	    */
		OBJ_ID_T	v_object;	/* Object id things	    */
		unsigned char	v_network_address[4];
		UINT_64_T	v_counter64;	/* 64 bit unsigned things   */
		} value_u;

	union   {
	        ptr_t    p_ptr;           /* storage space for moving info  */
	        bits32_t p_int;           /* between test and set routines  */
	        } priv;

	VBPRIVPROC_T *  vb_free_priv;	  /* pointer to routine to free the */
					  /* private storage space	    */
	UNDOPROC_T   *	undoproc;	  /* pointer to routine to undo sets*/
#if (INSTALL_ENVOY_AGENTX)
	OBJ_ID_T        ax_search_end;
	OCTET_T         ax_flags;
#endif
	} VB_T;
#define	v_gauge		v_counter
#define	v_timeticks	v_counter
#define v_uinteger32	v_counter
#define	v_opaque	v_string

#define vb_priv         priv.p_ptr
#define vb_int          priv.p_int

/* Values for vb_data_flags_n_type	*/
#define	VT_NUMBER	A_INTEGER
#define	VT_STRING	A_OCTETSTRING
#define VT_BITS		A_OCTETSTRING		/* same as a string */
#define	VT_OBJECT	A_OBJECTID
#define	VT_EMPTY	A_NULL
#define	VT_IPADDRESS	(A_APPLICATION | 0)
#define	VT_COUNTER	(A_APPLICATION | 1)
#define	VT_GAUGE	(A_APPLICATION | 2)
#define	VT_UNSIGNED32	(A_APPLICATION | 2)	/* same as a guage */
#define	VT_TIMETICKS	(A_APPLICATION | 3)
#define	VT_OPAQUE	(A_APPLICATION | 4)
#define	VT_COUNTER64	(A_APPLICATION | 6)
#define VT_UINTEGER32	(A_APPLICATION | 7)

/* values for exception data types */
#define VT_NOSUCHOBJ    (A_CONTEXT | 0)
#define VT_NOSUCHINS    (A_CONTEXT | 1)
#define VT_ENDOFMIB     (A_CONTEXT | 2)

/* Values for vb_flags	*/
#define	VFLAG_ALREADY_TEST	0x01
#define	VFLAG_ALREADY_SET	0x02
#define VFLAG_ALREADY_GET	0x02
#define VFLAG_SA_SENDIT		0x04
#define VFLAG_SA_DOIT		0x08

/* more values for vb_flags some of them need to be alligned with the
   already_* flags, the rest are put in as we have space */

#define VFLAG_TEST_STARTED	0x10
#define VFLAG_TEST_DONE		0x01
#define VFLAG_SET_STARTED	0x20
#define VFLAG_SET_DONE		0x02
#define VFLAG_UNDO_STARTED	0x40
#define VFLAG_UNDO_DONE		0x80
#define VFLAG_UNDO_BOTH		(VFLAG_UNDO_STARTED | VFLAG_UNDO_DONE)

#define VFLAG_GET_STARTED	0x20
#define VFLAG_GET_DONE		0x02

#define VFLAG_NEXT_STARTED	0x20
#define VFLAG_NEXT_DONE		0x02
#define VFLAG_NEXT_REDO		0x10
#define VFLAG_NEXT_VCREQ	0x40
#define VFLAG_NEXT_VCDONE	0x80
#define VFLAG_NEXT_INSTANCE	0x01
#define VFLAG_NEXT_VCBITS	(VFLAG_NEXT_VCREQ | VFLAG_NEXT_VCDONE)
#define VFLAG_NEXT_FLAGS	0xF3

#define ENVOY_AX_FLAGS_INCLUDE     0x01
#define ENVOY_AX_FLAGS_CLEAN_LEAF  0x02
#define ENVOY_AX_FLAGS_CLEAN_INDEX 0x04

/* Define the shape of the VarBindList */
typedef	struct VBL_S
	{
	ALENGTH_T	vbl_length;	/* Length of the VarBindList seq */
	int		vbl_count;	/* Number of Var Bind items	 */
	VB_T		*vblist;
	struct VBL_S    *vblp;          /* for get bulk chaining purposes */
	} VBL_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif


/* macros to get the data from a vbp */
#define VB_GET_INT32(V)		 ((V)->value_u.v_number)
#define VB_GET_UINT32(V)	 ((V)->value_u.v_counter)
#define VB_GET_IP_ADDRESS(V)	 ((V)->value_u.v_network_address)
#define VB_GET_STRING(V)	&((V)->value_u.v_string)
#define VB_GET_OBJECT_ID(V)	&((V)->value_u.v_object)
#define VB_GET_UINT64(V)	&((V)->value_u.v_counter64)

/* macro to get a pointer to a cookie, so that the user
   doesn't have to understand what a vb looks like */
#define VB_TO_COOKIE(vbp)	(vbp)->vb_ml.ml_leaf->user_cookie

/* various vb utility routines */
extern	VB_T *		VarBindList_Allocate __((int));
extern	VBL_T *		VBList_Allocate      __((int));
extern  bits32_t        vbsize               __((VB_T *));
extern  void            Clean_vb_data        __((VB_T *));
extern  void            Clean_vb             __((VB_T *));
extern	void		Clean_vb_list	     __((VBL_T *));
extern  void            VBList_Free          __((VBL_T *));

#ifdef __cplusplus
}
#endif

#endif /* (!defined(vb_inc)) */

/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/v3_priv.h,v 1.2 2001/11/06 21:35:06 josh Exp $ */

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
 * $Log: v3_priv.h,v $
 * Revision 1.2  2001/11/06 21:35:06  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:24  tneale
 * Tornado shuffle
 *
 * Revision 9.4  2001/01/19 22:22:12  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:46  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:36  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 17:06:57  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:26  sar
 * Update version stamp to match release
 *
 * Revision 1.3  1998/08/12 04:52:17  sar
 * revamp the initialization routines to limit the amount of
 * code that might get pulled in.
 *
 * Revision 1.2  1998/05/27 22:53:53  sar
 * Add routines to manipulate the auth and priv lists (add and find),
 * as well as some specifics adds (md5, sha)
 *
 * Revision 1.1  1998/05/23 19:49:40  sar
 * Include files for use with v3 code.  They define the structures
 * and entry points for the various pieces.
 * acc: access and group information
 * auth & priv: authentication and privacy
 * con: context
 * eng: engine ids, both this agents and any others it knows
 * mth: method routines, for use with mib control files
 * user: user info
 *
 */

/* [clearcase]
modification history
-------------------
01b,20apr09,y_t  update for requirement WIND00000579
01a,19apr05,job  update copyright notices
*/


#if (!defined(v3_priv_inc))
#define v3_priv_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

typedef ALENGTH_T (V3_PRIV_SIZE_T)   __((ptr_t, ALENGTH_T));
typedef int       (V3_PRIV_ENCODE_T) __((bits8_t *, ALENGTH_T, sbits32_t, sbits32_t,
					 bits8_t *, ALENGTH_T,
					 bits8_t *, bits8_t *));
typedef int	  (V3_PRIV_DECODE_T) __((bits8_t *, ALENGTH_T,sbits32_t, sbits32_t,
					 bits8_t *, ALENGTH_T,
					 bits8_t *, bits8_t *));

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct SNMP_PRIV_S
	{
	struct SNMP_PRIV_S *next;
	OBJ_ID_T            name;	/* name of the priv protocol */
	V3_PRIV_SIZE_T     *size;	/* routine to calculate the size */
	V3_PRIV_ENCODE_T   *encode;	/* routine to encode */
	V3_PRIV_DECODE_T   *decode;	/* routine to decode */
	ALENGTH_T 	    need;	/* space required for priv stuff */
	ALENGTH_T keysize;		/* space required for the key */
	} SNMP_PRIV_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif


/* global root pointer for the auth list */
extern SNMP_PRIV_T *v3_priv_root;

extern SNMP_PRIV_T * SNMP_Priv_Find    __((int, OIDC_T *));
extern int           SNMP_Priv_Add     __((SNMP_PRIV_T *));
extern int           SNMP_Priv_Add_DES __((void));
extern int           SNMP_Priv_Add_AESCFB128 __((void));

#define SNMP_Priv_Size(P, PKT, SL)	    (((P)->size)((PKT), (SL)))
#define SNMP_Priv_Encode(P, K, KL, BT, ET, B, BL, SP, OP) \
	(((P)->encode)((K), (KL), (BT),(ET), (B), (BL), (SP), (OP)))
#define SNMP_Priv_Decode(P, K, KL, BT, ET, B, BL, SP, OP) \
	(((P)->decode)((K), (KL), (BT), (ET), (B), (BL), (SP), (OP)))

#define SNMP_Priv_Get_KeySize(P) ((P)->keysize)
#define SNMP_Priv_Get_Need(P)    ((P)->need)

#define SNMP_Priv_Get_Name(P)    (&((P)->name))

extern OIDC_T      v3_priv_nopriv[];
extern int         v3_priv_nopriv_size;


extern SNMP_PRIV_T v3_priv_des;
extern SNMP_PRIV_T v3_priv_aescfb128;

#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(v3_priv_inc)) */

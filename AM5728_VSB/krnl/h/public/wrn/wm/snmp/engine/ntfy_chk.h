/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/ntfy_chk.h,v 1.2 2001/11/06 21:35:03 josh Exp $ */

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
 * $Log: ntfy_chk.h,v $
 * Revision 1.2  2001/11/06 21:35:03  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.6.4.2  2001/08/07 18:33:56  josh
 * adding support for SNMPv1/v2c/v3 coexistence to SNMP Notify code
 *
 * Revision 9.6.4.1  2001/06/28 18:22:56  josh
 * Seeding the Cormorant branch with the code from the old
 * cormorant branch
 *
 * Revision 9.6  2001/04/11 21:01:49  josh
 * we are overwriting the trunk's copy of this file with
 * the file from the kingfisher branch
 *
 * Revision 9.5  2001/01/19 22:22:05  paul
 * Update copyright.
 *
 * Revision 9.4  2000/07/12 18:36:41  josh
 * additional flags and prototypes
 *
 * Revision 9.3.4.1  2001/04/11 20:48:30  josh
 * moving cormorant-specific changes off to the cormorant
 * branch
 *
 * Revision 9.3.2.3  2001/03/12 22:07:55  tneale
 * Updated copyright
 *
 * Revision 9.3.2.2  2000/12/13 20:51:07  josh
 * SNMP_Send_Notify_Name() now obsoletes SNMP_Send_Notify()
 * logic of SNMP_Send_Notify_Name() is improved to help prevent nesting of
 * locks and generally smooth out flow of data
 *
 * Revision 9.3.2.1  2000/09/21 21:14:53  josh
 * bringing branch include files in line with root
 *
 * Revision 9.3  2000/03/17 19:48:35  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:34  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 16:57:00  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1999/10/21 20:43:34  josh
 * updating version stamps
 *
 * Revision 1.5  1999/10/18 22:49:52  josh
 * patches to method routines and related code
 *
 * Revision 1.4  1999/09/30 22:07:57  josh
 * cleaning up and changing prototypes to match new cookie scheme
 *
 * Revision 1.3  1999/09/27 21:11:44  josh
 * fixing nits, rewriting engine id <--> address code, adding installation
 * option
 *
 * Revision 1.2  1999/09/14 19:41:02  josh
 * completed code for sending v3 notifies
 *
 * Revision 1.1  1999/09/08 18:10:51  josh
 * functions to build a target list for a given notify
 *
 *
 */

/* [clearcase]
modification history
-------------------
01b,15sep09,y_t  fix for WIND00181371
01a,19apr05,job  update copyright notices
*/


#if (!defined(ntfy_chk_inc))
#define ntfy_chk_inc

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

#if (!defined(v3_trgt_inc))
#include <wrn/wm/snmp/engine/v3_trgt.h>
#endif

#if (!defined(v3_ntfy_inc))
#include <wrn/wm/snmp/engine/v3_ntfy.h>
#endif

#define tag_whitespace(TL) \
        ((((TL) == ' ') || ((TL) == '\x9') || \
        ((TL) == '\xd') || ((TL) == '\xa')) ? 1 : 0)

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct TARGET_LIST_S
        {
	struct TARGET_LIST_S *next;
        SNMP_TARGET_ADDR_T   *target;
        /* information passed in by the caller                */
	SNMP_PKT_T           *pktp;
	VBL_T                *vblp;
	EBUFFER_T             context;
        /* destination address                                */
	SNMPADDR_T            for_addr;
        /* information pulled out of the target_addr object   */
	OBJ_ID_T              tdomain;
	EBUFFER_T             taddress;
        bits32_t              timeout;
	bits16_t              retry_count;
        bits32_t              mms;
        /* information pulled out of the target_params object */
	int                   ptype;
	int                   mp_model;
        int                   sec_model;
	EBUFFER_T             sec_name;
	int                   sec_level;
        bits16_t              flags;
#define SNMP_TARGET_LIST_SEND_NOTIFY      0x0001
#define SNMP_TARGET_LIST_SEND_PROBE       0x0002
#define SNMP_TARGET_LIST_SEND_INFORM      0x0004
#define SNMP_TARGET_LIST_SEND_PROXY_PROBE 0x0008
#define SNMP_TARGET_LIST_INFOK            0x0010
    } TARGET_LIST_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

extern void init_target_list __((TARGET_LIST_T *));
extern void clean_target_list __((TARGET_LIST_T *));
extern TARGET_LIST_T *build_target_list __((SNMP_NOTIFY_T *, VBL_T *));
extern int taglist_check __((EBUFFER_T *, EBUFFER_T *));

#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(ntfy_chk_inc))*/


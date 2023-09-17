/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/sendntfy.h,v 1.2 2001/11/06 21:35:03 josh Exp $ */

/*
 * Copyright (C) 1999-2005, 2008, 2012 Wind River Systems, Inc.
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
 * Portions of this file are subject to additional licensing terms found in
 * the COPYING-SNMP.txt file that is included with this distribution.
 */

/*
 * $Log: sendntfy.h,v $
 * Revision 1.2  2001/11/06 21:35:03  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.9  2001/04/11 20:42:56  josh
 * merge from the kingfisher branch
 *
 * Revision 9.8  2001/01/19 22:22:06  paul
 * Update copyright.
 *
 * Revision 9.7  2000/07/12 18:39:31  josh
 * moving SNMP_Find_Matching_Engine_ID() to v3_eng.c -- appropriately
 * updating headers to correspond
 *
 * Revision 9.6  2000/06/09 14:54:12  josh
 * modifications due to new installation options, new proxy code
 * moved some definitions from sendntfy.h into v3_trgt.h so they'd
 * be built with the target code
 *
 * Revision 9.5.2.3  2001/03/12 22:07:56  tneale
 * Updated copyright
 *
 * Revision 9.5.2.2  2001/01/19 21:32:24  josh
 * prototype for SNMP_Send_Notify_Name().  retrans_timer_exp() redefined
 * to static
 *
 * Revision 9.5.2.1  2000/09/21 21:14:53  josh
 * bringing branch include files in line with root
 *
 * Revision 9.5  2000/03/17 19:48:38  meister
 * Update copyright notice
 *
 * Revision 9.4  2000/03/09 20:58:34  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.3  2000/03/09 16:58:01  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.2  2000/02/04 21:54:16  josh
 * prototyping functions that may need to be called by other modules
 * this is done primarily to make the vxWorks compiler happy.
 * Warning:  the prototypes added may be duplicated by a mib_hand.h
 * file generated by Emissary.
 *
 * Revision 9.1  2000/01/05 21:15:28  josh
 * moving code around to make sure probe checks occur as long as an
 * engine ID is found
 *
 * Revision 9.0  1999/10/21 20:43:35  josh
 * updating version stamps
 *
 * Revision 1.8  1999/10/07 23:38:24  josh
 * added definition for snmpaddr to taddress conversion routine
 *
 * Revision 1.7  1999/10/03 00:18:08  josh
 * fixing retramit timer code -- setting handler correctly,
 * cleaning up some double-free situations, and putting locking in
 * where needed
 *
 * Revision 1.6  1999/09/30 22:07:58  josh
 * cleaning up and changing prototypes to match new cookie scheme
 *
 * Revision 1.5  1999/09/27 21:11:44  josh
 * fixing nits, rewriting engine id <--> address code, adding installation
 * option
 *
 * Revision 1.4  1999/09/15 16:17:39  josh
 * Linking the Retransmit Lock to the Retransmit list turns out to
 * be a lot more trouble than it's worth.  We'll just refer to
 * it directly, a la SNMP_CoarseLock.
 *
 * Revision 1.3  1999/09/14 19:41:02  josh
 * completed code for sending v3 notifies
 *
 * Revision 1.2  1999/09/11 02:14:06  josh
 * updating header file -- created retrans block structures
 *
 * Revision 1.1  1999/09/01 21:56:04  josh
 * Code to do actual sending of Notifies
 *
 *
 */

/* [clearcase]
modification history
-------------------
01c,20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support.
01b,15jul08,y_z  add ack,timeout callback in the RETRANS_COOKIE_T
01a,19apr05,job  update copyright notices
*/


#if (!defined(sendntfy_inc))
#define sendntfy_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#if (!defined(buffer_inc))
#include <wrn/wm/snmp/engine/buffer.h>
#endif

#if (!defined(smi_inc))
#include <wrn/wm/snmp/engine/smi.h>
#endif

#if (!defined(etimer_inc))
#include <wrn/wm/snmp/engine/etimer.h>
#endif

#if (!defined(v3_trgt_inc))
#include <wrn/wm/snmp/engine/v3_trgt.h>
#endif

#if (!defined(v3_ntfy_inc))
#include <wrn/wm/snmp/engine/v3_ntfy.h>
#endif

#if (!defined(ntfy_chk_inc))
#include <wrn/wm/snmp/engine/ntfy_chk.h>
#endif

#ifndef ENVOY_RETRANS_CLEANUP_INTERVAL
#define ENVOY_RETRANS_CLEANUP_INTERVAL 500
#endif

#ifndef ENVOY_SET_RETRANS_ID
#define ENVOY_SET_RETRANS_ID(RET) \
        (RET)->retrans_id = root_retrans_block.next_retrans_id++
#endif

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct RETRANS_COOKIE_S
        {
	struct RETRANS_COOKIE_S *next;
	int                     ref_cnt;
        RETRANS_CLEANUP_T      *cleanup_rtn;
		ACK_CALLBACK_T         *ack_rtn;
		TIMEOUT_CALLBACK_T     *timeout_rtn;
	PTR_T                   cookie;
        } RETRANS_COOKIE_T;

typedef struct SNMP_RETRANS_S 
        {
	struct SNMP_RETRANS_S *next;
        SNMP_PKT_T            *pktp;
        SNMPADDR_T             for_addr;
        SNMPADDR_T             loc_addr;
        bits16_t               retry_count;
        bits32_t               timeout;
        IO_COMPLETE_T         *io_complete;
        ERR_COMPLETE_T        *error_complete;
        RETRANS_COOKIE_T       *cookie;
        bits32_t               retrans_id;
	bits32_t               time_sent;
	bits16_t               flags;
#define SNMP_RETRANS_FLAGS_EXPIRED       0x01
#define SNMP_RETRANS_FLAGS_ACKNOWLEDGED  0x02
#define SNMP_RETRANS_FLAGS_SENDING_PROBE 0x04
        bits16_t               ref_cnt;
	TARGET_LIST_T         *tlp;
        } SNMP_RETRANS_T;

typedef struct SNMP_RETRANS_ROOT_S
        {
	SNMP_RETRANS_T        *retrans;
        bits32_t               next_retrans_id;
        ENVOY_TIMER_T          timer;
        } SNMP_RETRANS_ROOT_T;
    
#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif


extern RETRANS_COOKIE_T *root_retrans_cookie;
extern SNMP_RETRANS_ROOT_T root_retrans_block;

extern int  SNMP_Probe_Check __((SNMP_RETRANS_T *));
extern void SNMP_Check_Retrans_List __((SNMP_PKT_T *));
extern void SNMP_Send_Packet_Retransmit __((SNMP_PKT_T *, SNMPADDR_T *,
					    SNMPADDR_T *, bits16_t, 
					    bits32_t, IO_COMPLETE_T *,
					    ERR_COMPLETE_T *, 
					    RETRANS_COOKIE_T *));
extern void SNMP_Send_Packet_Probe __((TARGET_LIST_T *, 
				       SNMPADDR_T *, 
				       IO_COMPLETE_T *, 
				       ERR_COMPLETE_T *,
				       RETRANS_COOKIE_T *));
extern void SNMP_Send_Notify __((SNMP_NOTIFY_T *, VBL_T *, SNMPADDR_T *, 
				 bits8_t *, ALENGTH_T, IO_COMPLETE_T *, 
				 ERR_COMPLETE_T *, RETRANS_CLEANUP_T *,
				 PTR_T, void *ptransport));
extern void SNMP_Send_Notify_Name_Ack __((bits8_t *, ALENGTH_T, VBL_T *,
                                      SNMPADDR_T *, bits8_t *, ALENGTH_T,
                                      IO_COMPLETE_T *, ERR_COMPLETE_T *,
                                      RETRANS_CLEANUP_T *, PTR_T,ACK_CALLBACK_T *,TIMEOUT_CALLBACK_T *, void *ptransport));
extern void SNMP_Send_Notify_Name __((bits8_t *, ALENGTH_T, VBL_T *,
                                      SNMPADDR_T *, bits8_t *, ALENGTH_T,
                                      IO_COMPLETE_T *, ERR_COMPLETE_T *,
                                      RETRANS_CLEANUP_T *, PTR_T, void *ptransport));


#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(sendntfy_inc))*/

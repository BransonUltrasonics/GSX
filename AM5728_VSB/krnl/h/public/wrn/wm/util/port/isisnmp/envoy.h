/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/isisnmp/envoy.h,v 1.2 2001/11/09 17:45:08 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
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
 * $Log: envoy.h,v $
 * Revision 1.2  2001/11/09 17:45:08  josh
 * pSOS-related path fixes
 *
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.13.4.1  2001/09/21 18:30:06  josh
 * updating porting files with new information for proxy and
 * coexistence
 *
 * Revision 1.13  2001/01/19 22:22:57  paul
 * Update copyright.
 *
 * Revision 1.12  2000/06/09 14:57:35  josh
 * updating header files to correspond with new installation options and
 * new code
 *
 * Revision 1.11  2000/04/05 19:40:51  josh
 * clearing up some additional warnings for pSOS+ 3.0
 *
 * Revision 1.10  2000/03/17 00:10:01  meister
 * Update copyright message
 *
 * Revision 1.9  1999/11/10 22:15:31  josh
 * adding support for the envoy_ax_translate_context userexit
 *
 * Revision 1.8  1999/11/10 18:28:05  josh
 * adding notification support to pSOS Envoy and fixing a few bugs
 *
 * Revision 1.7  1998/09/22 03:28:35  sar
 * correct a typo in a type
 *
 * Revision 1.6  1998/09/16 19:54:30  sar
 * Fixed up some user exits to allow binary customers a standard api
 *
 * Revision 1.5  1998/08/19 20:03:16  josh
 * removing extraneous () from things
 *
 * Revision 1.4  1998/08/13 22:56:05  josh
 * bringing in line with latest code changes
 *
 * Revision 1.3  1998/08/13 20:11:31  josh
 * updates for pSOS
 *
 * Revision 1.2  1998/06/28 21:18:28  josh
 * updating/bugfixing for SNMp
 *
 * Revision 1.1  1998/06/19 16:09:05  josh
 * port files for building SNMp
 *
 */

/* [clearcase]
modification history
-------------------
*/


#if INSTALL_ON_ISIX86
#ifndef NVRAM_START
#define NVRAM_START 0xf000
#endif
#ifndef P386
#define P386 1
#endif
#endif

#ifdef CONFIG_PREAMBLE

#include <string.h>

extern void * SNMP_memory_alloc(int );
extern void SNMP_memory_free(void *);
/* pSOS port */
extern int max_packet_size;
extern int max_oid_count;
extern int protect_critical_section;

/*----------------------------------------------------------------------*/
/* To tailor snmp for different applications                            */
/*----------------------------------------------------------------------*/
typedef struct snmp_cfg
  {
  /*--------------------------------------------------------------------*/
  /* Maximum snmp packet size this entity allows.                       */
  /*--------------------------------------------------------------------*/
  int max_packet_size;
  /*--------------------------------------------------------------------*/
  /* Maximum number of oid components allowed in a valid object id.     */
  /*--------------------------------------------------------------------*/
  int max_oid_count;
  /*--------------------------------------------------------------------*/
  /* snmp enters critical section before the testproc and leaves it     */
  /* after it has returned from the setproc/getproc.  Application should*/
  /* indicate whether to turn off task preemption globally when entering*/
  /* the critical section and to turn it back on at exit.  If this is   */
  /* set to FALSE, then the testproc/setproc/getproc for each individual*/
  /* mib object should properly implement the necessary locking         */
  /* mechanism to ensure the atomicity of the GET/SET operations.       */
  /*--------------------------------------------------------------------*/
  int protect_critical_section; /* TRUE/FALSE */
  } snmp_cfg;

extern void SNMP_Init(snmp_cfg *);

/* pSOS port */
/*----------------------------------------------------------------------*/
/* locks for local databases access                                     */
/*----------------------------------------------------------------------*/
typedef struct
  {
  unsigned char type;         /* READLOCK or WRITELOCK */
#define NO_LOCK   0x0
#define READLOCK  0x1
#define WRITELOCK 0x2
  unsigned long sema;         /* Semaphore id */
  int count;                  /* how many readlocks obtained */
  } LOCK_T;

#define ENVOY_LOCK_T LOCK_T

#else

/* This first should be the same as DECORUM_MAX_PACKET_SIZE in decorum.h */
#define	SNMP_MAX_PACKET_SIZE		max_packet_size
#define	SNMP_MIN_PACKET_SIZE		484

#define	MAX_OID_COUNT			max_oid_count

#define	validate_SNMP_version(x)		((x) == SNMP_VERSION_1)

extern int validate_SNMP_community
  __((SNMP_PKT_T *, SNMPADDR_T *, SNMPADDR_T *));

/*----------------------------------------------------------------
 SNMP_validate_community -- Check an operation against the community name.
         
Parameters:
        SNMP_PKT_T *pkt         The received packet (decoded format)
        SNMPADDR_T *src         Source of the packet
        SNMPADDR_T *dst         Destination of the packet (most likely
                                the address of the machine on which this
                                code is running.)

Returns:
        This procedure should return 0 if happy with the community and
        1 if not.

        This routine should fill in the packet structure with the mib
        view mask associated with the community string.
        This routine may hang additional data onto the "private" field of
        the packet structure.  The user will be given the opportinity to
        release that memory via SNMP_release_private().

 ----------------------------------------------------------------*/
#define SNMP_validate_community(x, y, z)	validate_SNMP_community(x,y,z)

/*----------------------------------------------------------------
 SNMP_release_private -- Get rid of a any private data attached to the packet
                    structure.  May be left undefined in which case nothing
                    is done with the private field when the SNMP packet
                    structure is freed.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/
extern void release_private __((SNMP_PKT_T *));
#define SNMP_release_private(pkt) release_private(pkt)

/*----------------------------------------------------------------
 SNMP_user_get_encode_buffer -- Allows the user the opportunity to get the
                        buffer that the SNMP packet will be encoded into.
                        If not used then leave undefined.

 Parameters:
        SNMP_PKT_T      *pkt    The packet being encoded.
        unsigned int    need    The amount of storage needed.
        EBUFFER_T       *ebuffp The ebuffer to fill in with the actual
                                buffer.

 Returns:
        0   successful
        1   not successful
 ----------------------------------------------------------------*/
extern int user_get_encode_buffer(SNMP_PKT_T *, unsigned int, EBUFFER_T *);
#define SNMP_user_get_encode_buffer(pkt, need, ebuffp) \
  user_get_encode_buffer(pkt, need, ebuffp)


/*----------------------------------------------------------------
 SNMP_trace_input
 SNMP_trace_output
                -- for implementing packet tracing through the
                   SNMP code.  May not be left undefined but may
                   instead be defined as nothing.

 Parameters:
        SNMP_PKT_T *pkt         The packet being traced

 Returns: Nothing
 ----------------------------------------------------------------*/
extern void TRACE_INPUT(SNMP_PKT_T *pkt);
extern void TRACE_OUTPUT(SNMP_PKT_T *pkt);
#define SNMP_TRACE_INPUT(pkt) TRACE_INPUT(pkt)
#define SNMP_TRACE_OUTPUT(pkt) TRACE_OUTPUT(pkt)

/*****************************************************************
 * The next series of macros are hooks into the processing of SET PDUs.
 * For most uses you should be able to leave these undefined.
 *****************************************************************/

/*----------------------------------------------------------------
 SNMP_validate_set_pdu -- Perform a global validation of a SET PDU.
         
 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns:
         -1 If the PDU is bad and should be rejected with a GEN_ERR.
          0 If the PDU is good and normal handling should proceed.
         +1 If the PDU is good and this routine has performed all of
            the set operations internally.
 ----------------------------------------------------------------*/
extern int validate_set_pdu(SNMP_PKT_T *pkt);
#define SNMP_validate_set_pdu(pkt) validate_set_pdu(pkt)

/*----------------------------------------------------------------
 SNMP_user_pre_set -- Perform a global validation of a SET PDU after all of the
                 test procedures have been called and given the "go ahead".
         
 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns:
         -1 If the PDU is bad and should be rejected with a GEN_ERR.
          0 If the PDU is good and normal handling should proceed.
         +1 If the PDU is good and this routine has performed all of
            the set operations internally.
 ----------------------------------------------------------------*/
extern int user_pre_set(SNMP_PKT_T *pkt);
#define SNMP_user_pre_set(pkt) user_pre_set(pkt)

/*----------------------------------------------------------------
 SNMP_user_post_set -- Perform any final activities after all of the set
                  procedures have been called.

 Parameters:
        SNMP_PKT_T *pkt         The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/
extern void user_post_set(SNMP_PKT_T *pkt);
#define SNMP_user_post_set(pkt) user_post_set(pkt)

/*****************************************************************
 The following are a series of macros that will connect the method
 routines we supplied for the RFC1445 or RFC2275 view schemes to
 your non-volatile storage areas.  You should consult the manuals
 for more information about these macros.
*****************************************************************/
#if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS)
#include <wrn/wm/demo/nvutils.h>

#define SNMP_VIEW_CREATE_TEST(PKT, NEW)		SNMP_NV_View_Add_Mod(NEW, 0)
#define SNMP_VIEW_UPDATE_TEST(PKT, OLD, NEW)	SNMP_NV_View_Add_Mod(NEW, 0)
#define SNMP_VIEW_DESTROY_TEST(PKT, OLD)	SNMP_NV_View_Add_Mod(OLD, 1)
#define SNMP_VIEW_BACKOUT_HOOK(NEW)		SNMP_NV_Clean()
#define SNMP_VIEW_CREATE_UNDO(NEW)		(SNMP_NV_Clean(), 0)
#define SNMP_VIEW_UPDATE_UNDO(OLD, NEW)		(SNMP_NV_Clean(), 0)
#define SNMP_VIEW_DESTROY_UNDO(OLD)		(SNMP_NV_Clean(), 0)
#define SNMP_VIEW_DESTROY_BACKOUT(OLD)		SNMP_NV_Clean()
#define SNMP_VIEW_FINISHED			SNMP_NV_Doit()

#endif /* #if (INSTALL_ENVOY_SNMP_RFC1445_VIEWS) */

#if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS)
#include <wrn/wm/demo/nvutils.h>

#define SNMP_VIEW_2275_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_view_2275_test(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_view_2275_test(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_view_2275_test(PKT, VBP, CUR, NEW)

#define SNMP_VIEW_2275_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_view_2275_set(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_view_2275_set(PKT, VBP, CUR, NEW)
#define SNMP_VIEW_2275_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_view_2275_set(PKT, VBP, CUR, NEW)

#define SNMP_VIEW_2275_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_view_2275_backout(CUR, NEW)
#define SNMP_VIEW_2275_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_view_2275_backout(CUR, NEW)
#define SNMP_VIEW_2275_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_view_2275_backout(CUR, NEW)

#define SNMP_VIEW_2275_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_view_2275_undo(PKT, VBP, CUR, SAV)
#define SNMP_VIEW_2275_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_view_2275_undo(PKT, VBP, CUR, SAV)
#define SNMP_VIEW_2275_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_view_2275_undo(PKT, VBP, CUR, SAV)

#define SNMP_VIEW_2275_FINISHED() snmp_view_2275_finished()

#endif /* #if (INSTALL_ENVOY_SNMP_RFC2275_VIEWS) */


/*****************************************************************
 If version 3 is installed we may need the following macros.

 ENVOY_TIME - returns a clock with a granularity of seconds
  PARAMETERS: none
  returns:    bits32_t

 SNMP_validate_address
  This routine gives the user a chance to examine the address
  information as well as most of the packet itself before a v3
  packet is processed.  The routine must attach the address
  information to the packet.  It may attach a non-default mib
  tree if desired.  Note: in some error cases the packet will
  not be decoded when this routine is called.

  Parameters:
	SNMP_PKT_T *pkt		The received packet (decoded format)
	SNMPADDR_T *src		Source of the packet
	SNMPADDR_T *dst		Destination of the packet (most likely
				the address of the machine on which this
				code is running.)
  returns:
	0 - ok
	1 - failure, drop the packet


 Lastly are are a series of macros that will connect the method
 routines we supplied for the v3 tables to your non-volatile
 storage areas.  You should consult the manuals for more information
 about these macros.
*****************************************************************/
#if INSTALL_ENVOY_SNMP_VERSION_3
#include <wrn/wm/demo/nvutils.h>

extern bits32_t get_curtime(void);
#define ENVOY_TIME() get_curtime()

extern int validate_SNMP_address __((SNMP_PKT_T *,
				     SNMPADDR_T *,
				     SNMPADDR_T *));
#define SNMP_validate_address(p, s, d) validate_SNMP_address(p, s, d)

#if INSTALL_ENVOY_SNMP_V3_TARGET

extern int envoy_snmpaddr_to_taddress __((SNMPADDR_T *, OBJ_ID_T *, 
					  EBUFFER_T *));
extern int envoy_taddress_to_snmpaddr __((SNMPADDR_T *, OBJ_ID_T *, 
					  EBUFFER_T *));

#define ENVOY_TADDRESS_TO_SNMPADDR(A, D, T) \
        envoy_taddress_to_snmpaddr(A, D, T)
#define ENVOY_SNMPADDR_TO_TADDRESS(A, D, T) \
        envoy_snmpaddr_to_taddress(A, D, T)
#endif

#define SNMP_V3_GROUP_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_group_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_GROUP_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_group_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_GROUP_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_group_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_GROUP_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_group_backout(CUR, NEW)
#define SNMP_V3_GROUP_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_group_backout(CUR, NEW)
#define SNMP_V3_GROUP_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_group_backout(CUR, NEW)

#define SNMP_V3_GROUP_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_group_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_GROUP_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_group_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_GROUP_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_group_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_GROUP_FINISHED() snmp_v3_group_finished()


#define SNMP_V3_ACCESS_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_access_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_ACCESS_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_access_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_ACCESS_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_access_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_ACCESS_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_access_backout(CUR, NEW)
#define SNMP_V3_ACCESS_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_access_backout(CUR, NEW)
#define SNMP_V3_ACCESS_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_access_backout(CUR, NEW)

#define SNMP_V3_ACCESS_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_access_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_ACCESS_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_access_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_ACCESS_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_access_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_ACCESS_FINISHED() snmp_v3_access_finished()


#define SNMP_V3_USER_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_user_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_USER_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_user_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_USER_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_user_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_USER_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_user_backout(CUR, NEW)
#define SNMP_V3_USER_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_user_backout(CUR, NEW)
#define SNMP_V3_USER_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_user_backout(CUR, NEW)

#define SNMP_V3_USER_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_user_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_USER_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_user_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_USER_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_user_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_USER_FINISHED() snmp_v3_user_finished()

#define SNMP_V3_TADDR_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_taddr_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_TADDR_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_taddr_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TADDR_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_taddr_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_TADDR_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_taddr_backout(CUR, NEW)
#define SNMP_V3_TADDR_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_taddr_backout(CUR, NEW)
#define SNMP_V3_TADDR_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_taddr_backout(CUR, NEW)

#define SNMP_V3_TADDR_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_taddr_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TADDR_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_taddr_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TADDR_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_taddr_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_TADDR_FINISHED() snmp_v3_taddr_finished()

#define SNMP_V3_TPARAM_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_tparam_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_TPARAM_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_tparam_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_TPARAM_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_tparam_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_TPARAM_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_tparam_backout(CUR, NEW)
#define SNMP_V3_TPARAM_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_tparam_backout(CUR, NEW)
#define SNMP_V3_TPARAM_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_tparam_backout(CUR, NEW)

#define SNMP_V3_TPARAM_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_tparam_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TPARAM_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_tparam_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_TPARAM_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_tparam_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_TPARAM_FINISHED() snmp_v3_tparam_finished()

#define SNMP_V3_NOTIFY_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_notify_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_NOTIFY_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_notify_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NOTIFY_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_notify_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_NOTIFY_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_notify_backout(CUR, NEW)
#define SNMP_V3_NOTIFY_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_notify_backout(CUR, NEW)
#define SNMP_V3_NOTIFY_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_notify_backout(CUR, NEW)

#define SNMP_V3_NOTIFY_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_notify_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NOTIFY_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_notify_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NOTIFY_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_notify_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_NOTIFY_FINISHED() snmp_v3_notify_finished()

#define SNMP_V3_NPROF_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nprof_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_NPROF_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nprof_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NPROF_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nprof_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_NPROF_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_nprof_backout(CUR, NEW)
#define SNMP_V3_NPROF_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nprof_backout(CUR, NEW)
#define SNMP_V3_NPROF_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nprof_backout(CUR, NEW)

#define SNMP_V3_NPROF_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nprof_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NPROF_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nprof_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NPROF_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nprof_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_NPROF_FINISHED() snmp_v3_nprof_finished()

#define SNMP_V3_NFILT_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nfilt_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_NFILT_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_nfilt_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_NFILT_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_nfilt_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_NFILT_DESTROY_BACKOUT(CUR, NEW)	 \
			snmp_v3_nfilt_backout(CUR, NEW)
#define SNMP_V3_NFILT_CREATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nfilt_backout(CUR, NEW)
#define SNMP_V3_NFILT_UPDATE_BACKOUT(CUR, NEW)		 \
			snmp_v3_nfilt_backout(CUR, NEW)

#define SNMP_V3_NFILT_CREATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nfilt_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NFILT_UPDATE_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nfilt_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_NFILT_DESTROY_UNDO(PKT, VBP, CUR, SAV)	 \
			snmp_v3_nfilt_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_NFILT_FINISHED() snmp_v3_nfilt_finished()

#define SNMP_V3_PROXY_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_v3_proxy_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_PROXY_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_v3_proxy_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_PROXY_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_v3_proxy_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_PROXY_DESTROY_BACKOUT(CUR, NEW)                         \
                        snmp_v3_proxy_backout(CUR, NEW)
#define SNMP_V3_PROXY_CREATE_BACKOUT(CUR, NEW)	                        \
                        snmp_v3_proxy_backout(CUR, NEW)
#define SNMP_V3_PROXY_UPDATE_BACKOUT(CUR, NEW)	                        \
                        snmp_v3_proxy_backout(CUR, NEW)

#define SNMP_V3_PROXY_CREATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_v3_proxy_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_PROXY_UPDATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_v3_proxy_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_PROXY_DESTROY_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_v3_proxy_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_PROXY_FINISHED()      snmp_v3_proxy_finished()

#define SNMP_V3_COMM_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			snmp_community_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_community_test(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			snmp_community_test(PKT, VBP, CUR, NEW)

#define SNMP_V3_COMM_DESTROY_SET(PKT, VBP, CUR, NEW)                   \
			snmp_community_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_CREATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_community_set(PKT, VBP, CUR, NEW)
#define SNMP_V3_COMM_UPDATE_SET(PKT, VBP, CUR, NEW)                    \
			snmp_community_set(PKT, VBP, CUR, NEW)

#define SNMP_V3_COMM_DESTROY_BACKOUT(CUR, NEW)                         \
                        snmp_community_backout(CUR, NEW)
#define SNMP_V3_COMM_CREATE_BACKOUT(CUR, NEW)	                        \
                        snmp_community_backout(CUR, NEW)
#define SNMP_V3_COMM_UPDATE_BACKOUT(CUR, NEW)	                        \
                        snmp_community_backout(CUR, NEW)

#define SNMP_V3_COMM_CREATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_community_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_COMM_UPDATE_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_community_undo(PKT, VBP, CUR, SAV)
#define SNMP_V3_COMM_DESTROY_UNDO(PKT, VBP, CUR, SAV)	                \
                        snmp_community_undo(PKT, VBP, CUR, SAV)

#define SNMP_V3_COMM_FINISHED()      snmp_community_finished()

#endif /* #if INSTALL_ENVOY_SNMP_VERSION_3 */

#if (INSTALL_ENVOY_EPI_MASTER)
#define SNMP_RESETSYSUPTIME
#endif

/*****************************************************************
 AgentX routines and macros for the master

 Defaults are fine for the following macros:
 #define ENVOY_AX_DEFAULT_TIMEOUT 1

 #define ENVOY_AX_FIND_MIB(CONTEXT) (EBufferUsed(CONTEXT) ? 0 : &mib_root_node)

 #define ENVOY_AX_MA_AC_ADD(PKTP, MIBROOT)    GEN_ERR
 #define ENVOY_AX_MA_AC_REMOVE(PKTP, MIBROOT) GEN_ERR
 #define ENVOY_AX_MA_AC_CLEAN(SESS_ID)
*****************************************************************/
#if INSTALL_ENVOY_AGENTX_MASTER


#if 0
extern bits32_t  envoy_get_sysuptime();
extern sbits32_t envoy_ax_transport_type(ptr_t);
extern int       envoy_ax_transport_string(ptr_t, ALENGTH_T *,
					   bits8_t **, int *);

#define ENVOY_GET_SYSUPTIME(CON)        envoy_get_sysuptime()
#define ENVOY_AX_TRANSPORT_TYPE(COOKIE) envoy_ax_transport_type(COOKIE)
#define ENVOY_AX_TRANSPORT_STRING(COOKIE, NEED, BUF, DYN) \
        envoy_ax_transport_string(COOKIE, NEED, BUF, DYN)
#define ENVOY_AX_TRANSPORTS_SUPPORTED    {0}
#endif

extern bits32_t  ENVOY_GET_SYSUPTIME(EBUFFER_T *);
extern sbits32_t ENVOY_AX_TRANSPORT_TYPE(ptr_t);
extern int       ENVOY_AX_TRANSPORT_STRING(ptr_t, ALENGTH_T *,
                                           bits8_t **, int *);
#define ENVOY_AX_TRANSPORTS_SUPPORTED    {0}

extern struct MIBNODE_S * ENVOY_AX_FIND_MIB(EBUFFER_T *);
extern bits16_t    ENVOY_AX_MA_AC_ADD(struct ENVOY_AX_PKT_S *,
				      struct MIBNODE_S *);
extern bits16_t    ENVOY_AX_MA_AC_REMOVE(struct ENVOY_AX_PKT_S *,
					 struct MIBNODE_S *);
extern void        ENVOY_AX_MA_AC_CLEAN(bits32_t);

extern void       envoy_ax_translate_context(SNMP_PKT_T *,
			                     struct ENVOY_AX_PKT_S *query_pkt);
#define ENVOY_AX_TRANSLATE_CONTEXT(PKT, AX_PKT) \
        envoy_ax_translate_context((PKT), (AX_PKT))
#endif /* #if INSTALL_ENVOY_AGENTX_MASTER */ 

/*****************************************************************
 AgentX routines and macros for the sub agent

 Defaults are fine for the following macros:
 #define ENVOY_AX_SA_MIB_ROOT_NODE(CKE, PKT) 0
 #define ENVOY_AX_SA_TIMEOUT 1000
*****************************************************************/


#if (INSTALL_ENVOY_SNMP_LOCK)
/* Implemented as user exit routines */
extern int get_read_lock(LOCK_T *);
extern void release_lock(LOCK_T *, unsigned char );
extern int get_write_lock(LOCK_T *);
extern int init_rw_lock(LOCK_T *, char *);


/* Don't use the lock stuff if the user doesn't define it */
#define ENVOY_SNMP_LOCK(lock)   LOCK_T  lock = {0,0,0}

#define ENVOY_SNMP_LOCK_EXTERN(lock)  extern LOCK_T lock

#define ENVOY_SNMP_INIT_LOCK_TAG(lock,str) init_rw_lock(&lock, str)

#define ENVOY_SNMP_GET_READ_LOCK(lock) get_read_lock(&lock)

#define ENVOY_SNMP_RELEASE_READ_LOCK(lock) release_lock(&lock, READLOCK)

#define ENVOY_SNMP_GET_WRITE_LOCK(lock) get_write_lock(&lock)

#define ENVOY_SNMP_RELEASE_WRITE_LOCK(lock) release_lock(&lock, WRITELOCK)

#define ENVOY_SNMP_LOCK_AND_1ARGS(get, lock, com1) \
        if (get(lock)) { com1; }

#define ENVOY_SNMP_LOCK_AND_2ARGS(get, lock, com1, com2) \
        if (get(lock)) { com1; com2; }

#define ENVOY_SNMP_LOCK_AND_3ARGS(get, lock, com1, com2, com3) \
        if (get(lock)) { com1; com2; com3; }

#define ENVOY_SNMP_LOCK_AND_4ARGS(get, lock, com1, com2, com3, com4) \
        if (get(lock)) { com1; com2; com3; com4; }

/*
 * declare all Lock externs here
 */
ENVOY_SNMP_LOCK_EXTERN(SNMP_CoarseLock);
ENVOY_SNMP_LOCK_EXTERN(SNMP_SubagentLock);

#if INSTALL_ENVOY_SNMP_VERSION_2
ENVOY_SNMP_LOCK_EXTERN(SNMP_TimeStampLock);
#endif
#if INSTALL_ENVOY_SNMP_PROXY
ENVOY_SNMP_LOCK_EXTERN(SNMP_ProxyLock);
#endif
ENVOY_SNMP_LOCK_EXTERN(SNMP_TimerLock);

#endif /* (INSTALL_ENVOY_SNMP_LOCK) */

/* Prototypes for these functions, just to keep things clear. */

extern bits32_t ENVOY_NOW(void);
extern void     ENVOY_CALL_TIMER(bits32_t when, void (*what)());

/* If SC_SOCAKDDR hasn't been defined by now, define it. */
#ifndef SC_SOCKADDR
#define SC_SOCKADDR sockaddr_in
#endif


#endif /* CONFIG_PREAMBLE */

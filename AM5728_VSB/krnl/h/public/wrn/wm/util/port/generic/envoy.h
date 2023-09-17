/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/generic/envoy.h,v 1.3 2002/05/23 18:51:11 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
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
 * $Log: envoy.h,v $
 * Revision 1.3  2002/05/23 18:51:11  josh
 * code modifications to support the entity MIB
 *
 * Revision 1.2  2001/11/07 15:58:50  tneale
 * Update for newest file layout
 *
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.10.4.3  2001/09/21 18:30:05  josh
 * updating porting files with new information for proxy and
 * coexistence
 *
 * Revision 1.10.4.2  2001/08/24 15:35:19  josh
 * add nonvolatile access definitions for proxy and community tables
 *
 * Revision 1.10.4.1  2001/08/24 14:59:45  josh
 * definition for generic (unix) proxy send routine
 *
 * Revision 1.10  2001/01/19 22:22:57  paul
 * Update copyright.
 *
 * Revision 1.9  2000/06/09 14:57:34  josh
 * updating header files to correspond with new installation options and
 * new code
 *
 * Revision 1.8  2000/03/17 00:09:56  meister
 * Update copyright message
 *
 * Revision 1.7  1999/11/10 22:16:33  josh
 * adding prototype for envoy_ax_translate_context()
 *
 * Revision 1.6  1999/10/19 15:15:28  josh
 * added NV macros for Notify code
 *
 * Revision 1.5  1999/05/27 16:16:30  josh
 * added context translation userexit, ENVOY_AX_TRANSLATE_CONTEXT
 *
 * Revision 1.4  1998/07/20 01:49:37  sar
 * Add () to the FINISHED and ENVOY_TIME macros
 *
 * Revision 1.3  1998/07/06 01:06:53  sar
 * Add a void to make compilers happy
 *
 * Revision 1.2  1998/07/01 17:06:55  josh
 * include snark/h/bug_conf.h when appropriate
 *
 * Revision 1.1  1998/06/21 21:48:47  sar
 * first revision
 *
 */

/* [clearcase]
modification history
-------------------
*/



/*
 * Fun with our new run-time debugging facility.
 * This is still experimental, and defaults to uninstalled.
 */
#if defined(INSTALL_SNARK_BUG_CONF) && INSTALL_SNARK_BUG_CONF
#include <wrn/wm/demo/bug_conf.h>
#endif

/*****************************************************************

  This is a generic version of envoy.h for use with the example
  code.  It contains all the macros and definitions that are
  common to all of the demo code.  Any platform specific information
  should be placed in the platforme specific files.  The platform
  specific files should include this file twice, once when the
  preamble flag is on and once when it isn't.

  In general customers will have one envoy.h file per platform
  that combines this file and the platform specific information,
  we use two files as there is a lot of shared code between our
  demos.
  
*****************************************************************/

#ifdef CONFIG_PREAMBLE

/*****************************************************************
 
  System Includes:
 
  Envoy uses memcpy(), memset(), and strlen().  The appropriate
  include file should be put here to see that these routines are
  properly declared.  If no such include file exists on the system
  then declare memcpy() and memset() here directly.  Envoy also
  uses min() and max().  Include the system's definition or use the
  macros supplied below.

  If necessary these should be in the platform specific files
*****************************************************************/


/*****************************************************************
  Memory allocation routines.  Envoy needs to allocate memory to
  decode the packet into and also to store the response data

  Make sure that the routines used by the SNMP_memory_alloc() and
  SNMP_memory_free() macros are declared.  If system memory routines
  are used (e.g. malloc() and free()) then include the appropriate
  system header file here.

  If necessary these should be defined in the platform specific files.
*****************************************************************/


/*****************************************************************
  Size limitations.
  Define the maximum packet size this implementation will accept.
  There is no hard upper limit.  SNMP_MAX_PACKET_SIZE should not
  be reduced below the value of SNMP_MIN_PACKET_SIZE, and
  SNMP_MIN_PACKET_SIZE shouldn't be changed.
  Also define the maximum number of components for an object identifer
  which we should be able to handle, including those in the instance.

 The first, MAX, should be the same as DECORUM_MAX_PACKET_SIZE in decorum.h
*****************************************************************/

#define	SNMP_MAX_PACKET_SIZE		8192
#define	SNMP_MIN_PACKET_SIZE		484
#define	MAX_OID_COUNT			32

/*****************************************************************
  Demo lock structure.  This lock is for testing purposes only.
  You will probably want to change this to suit your system.
*****************************************************************/
#if (INSTALL_ENVOY_SNMP_LOCK)
typedef struct ENVOY_LOCK_S
	{
	int readers;
	int writers;
	} ENVOY_LOCK_T;
#endif

#else /* config preamble */

/*****************************************************************
  The following macro definitions allow access to the user configurable
  part of the SNMP code.
  Include any declarations needed for the routines specified below
  by the macros.  Don't put <snmp.h> here because this file is
  included by that one.
  We start with the more generic macros and then get into macros
  required by specific options.
*****************************************************************/

/****************************************************************
 SNMP_release_private 
 Get rid of a any private data attached to the packet
 structure.  May be left undefined in which case nothing
 is done with the private field when the SNMP packet
 structure is freed.

 proxy_release_private is part of the code for demoing
 proxies
 
 Parameters: SNMP_PKT_T *pkt		The packet itself

 Returns:    Nothing

 #define SNMP_release_private(pkt) release_private(pkt) 
*****************************************************************/

#if (INSTALL_ENVOY_SNMP_PROXY)
extern void proxy_release_private __((SNMP_PKT_T *));
#define SNMP_release_private(pkt) proxy_release_private(pkt)
#endif

/*****************************************************************
 SNMP_user_get_encode_buffer
  Allows the user the opportunity to get the buffer that the SNMP
  packet will be encoded into.  If not used then leave undefined.

 Parameters:
	SNMP_PKT_T	*pkt	The packet being encoded.
	unsigned int	need	The amount of storage needed.
	EBUFFER_T	*ebuffp	The ebuffer to fill in with the actual
				buffer.

 Returns:
	0   successful
	1   not successful

 #define SNMP_user_get_encode_buffer(pkt, need, ebuffp) 0 
****************************************************************/


/*****************************************************************
 SNMP_trace_input
 SNMP_trace_output
   for implementing packet tracing through the SNMP code.
   May not be left undefined but may instead be defined as nothing.

 Parameters:
	SNMP_PKT_T *pkt		The packet being traced

 Returns: Nothing
****************************************************************/
#define SNMP_TRACE_INPUT(pkt)
#define SNMP_TRACE_OUTPUT(pkt)


/*****************************************************************
 The next series of macros are hooks into the processing of SET PDUs.
 For most uses you should be able to leave these undefined.

 SNMP_validate_set_pdu -- Perform a global validation of a SET PDU.
 SNMP_user_pre_set     -- Perform a global validation of a SET PDU
			  after all of the test procedures have been
			  called and given the "go ahead".	 
 SNMP_user_post_set    -- Perform any final activities after all
			  the set procedures have been called.

 Parameters: SNMP_PKT_T *pkt	The packet itself

 Returns for SNMP_Validate_set_pdu and SNMP_user_pre_set:
	 -1 If the PDU is bad and should be rejected with a GEN_ERR.
	  0 If the PDU is good and normal handling should proceed.
	 +1 If the PDU is good and this routine has performed all of
	    the set operations internally.

 Returns for SNMP_user_post_set: nothing

 #define SNMP_validate_set_pdu(pkt) validate_set_pdu(pkt) 
 #define SNMP_user_pre_set(pkt)     user_pre_set(pkt) 
 #define SNMP_user_post_set(pkt)    user_post_set(pkt)
*****************************************************************/

/*****************************************************************
 Timer routines.  These connect the Envoy timer routines to the
 system.  These are required for some of the options.

 ENVOY_CALL_TIMER - this routine should cause the system to call
		    the handler routine in the specified number
		    of milliseconds.

 ENVOY_NOW        - this routine returns a clock with millisecond
		    granularity.  If attache is installed it can
		    be defined to be glue_now.  We don't do that
		    here so that this file can be used by multiple
		    demos.  Instead we define envoy_now() which
		    may be the real function or may simply be a shim.
*****************************************************************/
extern void	envoy_call_timer __((bits32_t, void (*)(void)));
#define ENVOY_CALL_TIMER(T, H)	     envoy_call_timer(T, H)

extern  bits32_t    envoy_now  __((void));
#define ENVOY_NOW() envoy_now()


/******************************************************************
 SNMP_validate_community
  Check an operation against the community name.  If the community
  is accepted this routine must set the view mask, number or name
  (depending on the view scheme in use) and attach the source and
  destination addresses to the packet.  It may attach proxy information
  to the packet or choose a non-default mib tree for use with the
  give community.

  This routine may hang additional data onto the "private" field of
  the packet structure.  The user will be given the opportinity to
  release that memory via SNMP_release_private().

Parameters:
	SNMP_PKT_T *pkt		The received packet (decoded format)
	SNMPADDR_T *src		Source of the packet
	SNMPADDR_T *dst		Destination of the packet (most likely
				the address of the machine on which this
				code is running.)

Returns:
	0 - community is ok and processing should continue
	1 - community isn't okay, drop the packet, any stats have been updated
	2 - community isn't okay, drop the packet, update BadCommunityNames
	3 - community isn't okay, drop the packet, update BadCommunityUses
*****************************************************************/
extern int validate_SNMP_community __((SNMP_PKT_T *,
				       SNMPADDR_T *,
				       SNMPADDR_T *));
#define SNMP_validate_community(p, s, d) validate_SNMP_community(p, s, d)


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
			SNMP_NV_View_2275_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_VIEW_2275_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_View_2275_Add_Mod(CUR, NEW, 0)
#define SNMP_VIEW_2275_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_View_2275_Add_Mod(CUR, NEW, 0)

#define SNMP_VIEW_2275_DESTROY_BACKOUT(CUR, NEW)	SNMP_NV_Clean()
#define SNMP_VIEW_2275_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_VIEW_2275_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_VIEW_2275_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_VIEW_2275_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_VIEW_2275_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_VIEW_2275_FINISHED()		SNMP_NV_Doit()

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

extern long time(long *);
#define ENVOY_TIME() (time((long *) 0))

extern int validate_SNMP_V3_address __((SNMP_PKT_T *,
					SNMPADDR_T *,
					SNMPADDR_T *));
#define SNMP_validate_address(p, s, d) validate_SNMP_V3_address(p, s, d)

#define SNMP_V3_GROUP_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_Group_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_GROUP_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Group_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_GROUP_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Group_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_GROUP_DESTROY_BACKOUT(CUR, NEW)	        SNMP_NV_Clean()
#define SNMP_V3_GROUP_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_GROUP_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_GROUP_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_GROUP_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_GROUP_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_GROUP_FINISHED()		        SNMP_NV_Doit()


#define SNMP_V3_ACCESS_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_Access_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_ACCESS_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Access_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_ACCESS_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Access_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_ACCESS_DESTROY_BACKOUT(CUR, NEW)	SNMP_NV_Clean()
#define SNMP_V3_ACCESS_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_ACCESS_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_ACCESS_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_ACCESS_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_ACCESS_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_ACCESS_FINISHED()		        SNMP_NV_Doit()

#define SNMP_V3_USER_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_User_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_USER_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_User_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_USER_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_User_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_USER_DESTROY_BACKOUT(CUR, NEW)	SNMP_NV_Clean()
#define SNMP_V3_USER_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_USER_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_USER_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_USER_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_USER_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_USER_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_TADDR_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_TAddr_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_TADDR_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_TAddr_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_TADDR_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_TAddr_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_TADDR_DESTROY_BACKOUT(CUR, NEW)         SNMP_NV_Clean()
#define SNMP_V3_TADDR_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_TADDR_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_TADDR_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_TADDR_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_TADDR_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_TADDR_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_TPARAM_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_TParam_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_TPARAM_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_TParam_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_TPARAM_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_TParam_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_TPARAM_DESTROY_BACKOUT(CUR, NEW)        SNMP_NV_Clean()
#define SNMP_V3_TPARAM_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_TPARAM_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_TPARAM_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_TPARAM_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_TPARAM_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_TPARAM_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_NOTIFY_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_Notify_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_NOTIFY_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Notify_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_NOTIFY_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Notify_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_NOTIFY_DESTROY_BACKOUT(CUR, NEW)         SNMP_NV_Clean()
#define SNMP_V3_NOTIFY_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_NOTIFY_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_NOTIFY_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_NOTIFY_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_NOTIFY_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_NOTIFY_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_NPROF_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_NProf_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_NPROF_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_NProf_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_NPROF_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_NProf_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_NPROF_DESTROY_BACKOUT(CUR, NEW)        SNMP_NV_Clean()
#define SNMP_V3_NPROF_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_NPROF_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_NPROF_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_NPROF_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_NPROF_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_NPROF_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_NFILT_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_NFilt_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_NFILT_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_NFilt_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_NFILT_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_NFilt_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_NFILT_DESTROY_BACKOUT(CUR, NEW)        SNMP_NV_Clean()
#define SNMP_V3_NFILT_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_NFILT_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_NFILT_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_NFILT_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_NFILT_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_NFILT_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_PROXY_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_V3_Proxy_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_PROXY_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Proxy_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_PROXY_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_V3_Proxy_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_PROXY_DESTROY_BACKOUT(CUR, NEW)         SNMP_NV_Clean()
#define SNMP_V3_PROXY_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_PROXY_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_PROXY_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_PROXY_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_PROXY_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_PROXY_FINISHED()			SNMP_NV_Doit()

#define SNMP_V3_COMM_DESTROY_TEST(PKT, VBP, CUR, NEW)                   \
			SNMP_NV_Community_Add_Mod(CUR, NEW, NV_MOD_DESTROY)
#define SNMP_V3_COMM_CREATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_Community_Add_Mod(CUR, NEW, 0)
#define SNMP_V3_COMM_UPDATE_TEST(PKT, VBP, CUR, NEW)                    \
			SNMP_NV_Community_Add_Mod(CUR, NEW, 0)

#define SNMP_V3_COMM_DESTROY_BACKOUT(CUR, NEW)         SNMP_NV_Clean()
#define SNMP_V3_COMM_CREATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()
#define SNMP_V3_COMM_UPDATE_BACKOUT(CUR, NEW)		SNMP_NV_Clean()

#define SNMP_V3_COMM_CREATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_COMM_UPDATE_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)
#define SNMP_V3_COMM_DESTROY_UNDO(PKT, VBP, CUR, SAV)	(SNMP_NV_Clean(), 0)

#define SNMP_V3_COMM_FINISHED()			SNMP_NV_Doit()

#if INSTALL_ENVOY_SNMP_V3_PROXY
#if INSTALL_ATTACHE
void envoy_snmp_send_proxy __((SNMPADDR_T *, SNMPADDR_T *,
                               PTR_T, ALENGTH_T));

#define ENVOY_SNMP_SEND_PROXY(FOR, LOC, PKT, SIZ) \
    envoy_snmp_send_proxy((FOR), (LOC), (PKT), (SIZ))
#endif
#endif /* #if INSTALL_ENVOY_SNMP_V3_PROXY */
#endif /* #if INSTALL_ENVOY_SNMP_VERSION_3 */

/*****************************************************************
 If usec is installed we may need the following macros.
 NOTE WELL: usec was an experimental protocol for providing security
 to snmp.  It has been superseeded by v3.  It should not be used and
 will be removed in a future release.

 SNMP_USECTIME - returns a clock with a granularity of seconds
*****************************************************************/
#if (INSTALL_ENVOY_SNMP_VERSION_USEC)
typedef long USECTIME_T;
extern long time(long *);
#define SNMP_USECTIME ((USECTIME_T)time((long *)0))
#endif /* INSTALL_ENVOY_SNMP_VERSION_USEC */

/*****************************************************************
A macro for use with the epilogue master agent.
*****************************************************************/
#if (INSTALL_ENVOY_EPI_MASTER)
#define SNMP_RESETSYSUPTIME
#endif

#if (INSTALL_ENVOY_AGENTX_MASTER) || (INSTALL_ENVOY_ENTITY_MIB)
extern bits32_t  envoy_get_sysuptime(void);

#define ENVOY_GET_SYSUPTIME(CON)         envoy_get_sysuptime()
#endif /* #if (INSTALL_ENVOY_AGENTX_MASTER) || (INSTALL_ENVOY_ENTITY_MIB) */

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
extern sbits32_t envoy_ax_transport_type(ptr_t);
extern int       envoy_ax_transport_string(ptr_t, ALENGTH_T *,
					   bits8_t **, int *);
extern void      envoy_ax_translate_context(SNMP_PKT_T            *pktp,
		                            struct ENVOY_AX_PKT_S *query_pkt);

#define ENVOY_AX_TRANSPORT_TYPE(COOKIE) envoy_ax_transport_type(COOKIE)
#define ENVOY_AX_TRANSPORT_STRING(COOKIE, NEED, BUF, DYN) \
        envoy_ax_transport_string(COOKIE, NEED, BUF, DYN)
#define ENVOY_AX_TRANSPORTS_SUPPORTED    {0}
#define ENVOY_AX_TRANSLATE_CONTEXT(PKT, AX_PKT) \
        envoy_ax_translate_context(PKT, AX_PKT)
#endif /* #if INSTALL_ENVOY_AGENTX_MASTER */ 

/*****************************************************************
 AgentX routines and macros for the sub agent

 Defaults are fine for the following macros:
 #define ENVOY_AX_SA_MIB_ROOT_NODE(CKE, PKT) 0
 #define ENVOY_AX_SA_TIMEOUT 1000
*****************************************************************/

/*****************************************************************
 Lock Test Code:
  The following macros define a simple example to test the locking
  code.  NOTE WELL: this code doesn't block and so is not acceptable
  for use as the locking code, it is only here for test purposes.

  A lock is 2 uints one for reading and one for writing.  When a
  routine gets a lock the value is incremented by 1, when a routine
  frees a lock it is decremented by 1.  We are allowing multiple
  readers and one writer.  As this code can't block we simply return
  an error if we can't get a lock, real locking code must block in
  that case.
*****************************************************************/
#if (INSTALL_ENVOY_SNMP_LOCK)
#define ENVOY_SNMP_LOCK(name)			ENVOY_LOCK_T name = {0, 0}
#define ENVOY_SNMP_LOCK_EXTERN(name)	extern	ENVOY_LOCK_T name

#define ENVOY_SNMP_INIT_LOCK(name)   ((name).readers = 0, (name).writers = 0)

#define ENVOY_SNMP_GET_READ_LOCK(name) \
	(((name).writers) ? 1 : ((name).readers++, 0))
#define ENVOY_SNMP_RELEASE_READ_LOCK(name)  ((name).readers--)

#define ENVOY_SNMP_GET_WRITE_LOCK(name) \
	(((name).readers || (name).writers) ? 1 : ((name).writers++, 0))
#define ENVOY_SNMP_RELEASE_WRITE_LOCK(name) ((name).writers--)
#endif /* (INSTALL_ENVOY_SNMP_LOCK) */

#endif /* CONFIG_PREAMBLE */

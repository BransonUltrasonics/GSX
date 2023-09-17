/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/envtest/envoy.h,v 1.1.1.1 2001/11/05 17:47:09 tneale Exp $ */

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
 * Configuration for Envoy on MSDOS with Attache.
 */

/*
 * $Log: envoy.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:22:56  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:09:53  meister
 * Update copyright message
 *
 * Revision 1.2  1998/02/25 04:56:50  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/10/29 01:31:28  sar
 * Updated the master agent installation option.
 *
 * Revision 1.0  1997/08/08 22:11:15  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:58:09  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/28 19:57:20  sar
 * Initial Version
 *
 * Revision 7.0  1996/03/18  20:25:57  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/11/06  20:05:58  sar
 * removed validate_community's dependency on install v1
 *
 * Revision 6.0  1995/05/31  22:00:34  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/22  19:39:49  sar
 * Modified the lock code to add some new macros.
 *
 * Revision 5.3  1995/05/05  22:30:05  sar
 * Added some default macros for the timer code.
 *
 * Revision 5.2  1995/05/01  21:57:42  sar
 * Put the example lock macros in an if def block.
 *
 * Revision 5.1  1994/10/25  20:03:13  sar
 * Added some simple lock code for testing and as an example, also
 * added a definition for snmpresetsysuptime.
 *
 * Revision 5.0  1994/05/16  17:24:02  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:47:40  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 1.1  1993/06/09  20:37:06  sar
 * Initial revision
 */

/* [clearcase]
modification history
-------------------
*/


#if defined(CONFIG_PREAMBLE)

/*****************************************************************
 *
 * System Includes:
 *
 *     The SNMP uses memcpy(), memset(), and strlen().  The appropriate
 * include file should be put here to see that these routines are
 * properly declared.  If no such include file exists on the system
 * then declare memcpy() and memset() here directly.
 *****************************************************************/
/* zortech, watcom and intel 960 don't have a memory.h so away it goes */
#if (!defined(INSTALL_on_ztc) && !defined(INSTALL_on_watcom) && !defined(INSTALL_on_i960))
#include <memory.h>
#endif /* INSTALL_on_ztc */

#include <string.h>

/*****************************************************************
 * Memory allocation routines.  The SNMP needs to allocate memory to
 * decode the packet into and also to store the response data for GET's
 * and GET-NEXT's.
 *****************************************************************/

/* Fix up mach 386's improper declaration of the mem* routines */
#if defined(INSTALL_on_mach386)
#define MEMCPY(dst, src, n) memcpy((char *)(dst), (char *)(src), n)
#define MEMSET(dst, val, n) memset((char *)(dst), val, n)
#define MEMCMP(dst, src, n) memcmp((char *)(dst), (char *)(src), n)
#endif

/*----------------------------------------------------------------
 * Make sure that the routines used by the SNMP_memory_alloc() and
 * SNMP_memory_free() macros are declared.  If system memory routines
 * are used (e.g. malloc() and free()) then include the appropriate
 * system header file here.
 ----------------------------------------------------------------*/

/* The memory allocation macros */
#if defined(INSTALL_on_unix)
extern void *malloc __((unsigned));
extern void free __((void *));
#endif
#if defined(INSTALL_on_msdos)
#include <stdlib.h>
#endif

#define SNMP_memory_alloc(need)	malloc(need)
#define SNMP_memory_free(buf)	free(buf)

/* Define the maximum packet size this implementation will accept.	*/
/* There is no hard upper limit.  SNMP_MAX_PACKET_SIZE should not be	*/
/* reduced below the value of SNMP_MIN_PACKET_SIZE.			*/
#define	SNMP_MAX_PACKET_SIZE		1400
#define	SNMP_MIN_PACKET_SIZE		484 /* shouldn't change */

/* Say what is the maximum number of components of an object identifer	*/
/* which we can handle, including those in the instance.		*/
#define	MAX_OID_COUNT			32

#if (INSTALL_ENVOY_SNMP_LOCK)
typedef struct ENVOY_LOCK_S 
	{
	int readers;
	int writers;
	} ENVOY_LOCK_T;
#endif

#else /* CONFIG_PREAMBLE */

/*
 * Here goes all the user exit macro definitions and function declarations
 * to match.
 */

/*****************************************************************
 *
 * The following macro definitions allow access to the user configurable
 * part of the SNMP code.
 *
 *****************************************************************/

/*****************************************************************
 * Include any declarations needed for the routines specified below
 * by the macros.
 *****************************************************************/

extern int validate_SNMP_community __((SNMP_PKT_T *, SNMPADDR_T *,
				       SNMPADDR_T *));
/*----------------------------------------------------------------
 SNMP_validate_community -- Check an operation against the community name.
	 
Parameters:
	SNMP_PKT_T *pkt		The received packet (decoded format)
	SNMPADDR_T *src		Source of the packet
	SNMPADDR_T *dst		Destination of the packet (most likely
				the address of the machine on which this
				code is running.)

Returns:
	This procedure should return 0 if happy with the community string
	and 1 if not.

	This routine should fill in the packet structure with the mib
	view mask associated with the community string.
	This routine may hang additional data onto the "private" field of
	the packet structure.  The user will be given the opportinity to
	release that memory via SNMP_release_private().

 ----------------------------------------------------------------*/
#define SNMP_validate_community(pkt, src, dst) \
	 validate_SNMP_community(pkt, src, dst)

#if defined(INSTALL_ENVOY_SNMP_VERSION_2)
/* The time routine, this must be defined */

#define SNMP_PARTYTIME (UINT_32_T)0L

#endif /* INSTALL_ENVOY_SNMP_VERSION_2 */

#if (INSTALL_ENVOY_EPI_MASTER)
#define SNMP_RESETSYSUPTIME
#endif

/*----------------------------------------------------------------
 SNMP_release_private -- Get rid of a any private data attached to the packet
		    structure.  May be left undefined in which case nothing
		    is done with the private field when the SNMP packet
		    structure is freed.

 Parameters:
	SNMP_PKT_T *pkt		The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/
/* #define SNMP_release_private(pkt) release_private(pkt) */


/*----------------------------------------------------------------
 SNMP_user_get_encode_buffer -- Allows the user the opportunity to get the
			buffer that the SNMP packet will be encoded into.
			If not used then leave undefined.

 Parameters:
	SNMP_PKT_T	*pkt	The packet being encoded.
	unsigned int	need	The amount of storage needed.
	EBUFFER_T	*ebuffp	The ebuffer to fill in with the actual
				buffer.

 Returns:
	0   successful
	1   not successful
 ----------------------------------------------------------------*/
/* #define SNMP_user_get_encode_buffer(pkt, need, ebuffp) 0 */


/*----------------------------------------------------------------
 SNMP_trace_input
 SNMP_trace_output
		-- for implementing packet tracing through the
		   SNMP code.  If not used then define as nothing.

 Parameters:
	SNMP_PKT_T *pkt		The packet being traced

 Returns: Nothing
 ----------------------------------------------------------------*/
#define SNMP_TRACE_INPUT(pkt)
#define SNMP_TRACE_OUTPUT(pkt)

/*****************************************************************
 * The next series of macros are hooks into the processing of SET PDUs.
 * For most uses you should be able to leave these undefined.
 *****************************************************************/

/*----------------------------------------------------------------
 SNMP_validate_set_pdu -- Perform a global validation of a SET PDU.
			May be left undefined.
	 
 Parameters:
	SNMP_PKT_T *pkt		The packet itself

 Returns:
	 -1 If the PDU is bad and should be rejected with a GEN_ERR.
	  0 If the PDU is good and normal handling should proceed.
	 +1 If the PDU is good and this routine has performed all of
	    the set operations internally.
 ----------------------------------------------------------------*/
/* #define SNMP_validate_set_pdu(pkt) validate_set_pdu(pkt) */

/*----------------------------------------------------------------
 SNMP_user_pre_set -- Perform a global validation of a SET PDU after all of the
		 test procedures have been called and given the "go ahead".
		 May be left undefined.
	 
 Parameters:
	SNMP_PKT_T *pkt		The packet itself

 Returns:
	 -1 If the PDU is bad and should be rejected with a GEN_ERR.
	  0 If the PDU is good and normal handling should proceed.
	 +1 If the PDU is good and this routine has performed all of
	    the set operations internally.
 ----------------------------------------------------------------*/
/* #define SNMP_user_pre_set(pkt) user_pre_set(pkt) */

/*----------------------------------------------------------------
 SNMP_user_post_set -- Perform any final activities after all of the set
		  procedures have been called.  May be left undefined.

 Parameters:
	SNMP_PKT_T *pkt		The packet itself

 Returns: Nothing
 ----------------------------------------------------------------*/
/* #define SNMP_user_post_set(pkt) user_post_set(pkt) */

#if (INSTALL_ENVOY_SNMP_LOCK)
/* Here's a simple example to test the locking code, a lock is 2 uints
   one for reading, one for writing.  When a routine gets a lock the
   value is incremented by 1, when a routine frees a lock it is decremented
   by 1.  We are allowed multiple readers and 1 writer.  This code
   won't block so if a lock can't be an error is returned */

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

#define ENVOY_CALL_TIMER(T, H)
#define ENVOY_NOW() 0

#endif /* CONFIG_PREAMBLE */

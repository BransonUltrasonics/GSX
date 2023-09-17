/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/snmp.h,v 1.4 2003/01/15 13:58:56 josh Exp $ */

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
 * $Log: snmp.h,v $
 * Revision 1.4  2003/01/15 13:58:56  josh
 * directory structure shifting
 *
 * Revision 1.3  2001/11/06 21:50:40  josh
 * second (and hopefully final) pass of new path hacking
 *
 * Revision 1.2  2001/11/06 21:35:04  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.5  2001/01/19 22:22:06  paul
 * Update copyright.
 *
 * Revision 9.4  2000/03/17 19:48:39  meister
 * Update copyright notice
 *
 * Revision 9.3  1999/10/29 21:05:42  josh
 * The infrastructure lock tag is needed by Epilogue subagents as well
 *
 * Revision 9.2  1999/10/18 22:35:52  sar
 * Add free_lock
 * add init_lock to the block used if locks aren't defined
 *
 * Revision 9.1  1999/09/14 19:45:12  josh
 * added a new lock -- SNMP_V3_Retrans_Lock
 *
 * Revision 9.0  1998/10/16 22:10:18  sar
 * Update version stamp to match release
 *
 * Revision 8.5  1998/05/29 17:12:34  josh
 * a few ifdefs for cleanliness
 *
 * Revision 8.4  1998/05/29 17:10:02  josh
 * new locking macros added.
 *
 * Revision 8.3  1998/05/28 03:41:35  sar
 * Added extern for boots lock
 *
 * Revision 8.2  1998/04/16 15:45:07  josh
 * Declaring a separate lock if necessary
 *
 * Revision 8.1  1998/02/25 04:50:44  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:27  sar
 * Updated revision to 8.0
 *
 * Revision 7.5  1997/10/22 03:07:30  sar
 * Mofied the agentx and older subagent schemes install options
 *
 * Revision 7.4  1997/10/16 00:25:18  sar
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
 * Revision 7.3  1997/08/21 17:23:44  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 7.2  1997/03/20 06:48:16  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:59:45  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/02  23:29:47  sar
 * added some semi-colons in the lock macros.
 *
 * Revision 5.3  1995/04/18  20:41:28  sar
 * Added some new macros and modified the old macros so we don't require
 * ifdefs in all of the mainline code.  Instead we use the new macros
 * which will be defined to nothgin if locks aren't installed.
 *
 * Revision 5.2  1995/03/20  23:11:33  sar
 * Redo the defaults for the lock scheme.  They macros are now
 * defined to look like "good" returns so the main line code does not
 * have to deal with if defs.  Some places that have large blocks of
 * lock code may still choose to use if defs.
 *
 * Revision 5.1  1994/10/12  20:11:47  sar
 * Added an ifdef around the def for the coarse lock
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.6  1993/05/28  18:01:05  sar
 * added the define as nothing macros for snmp_trace_input and output.
 *
 * Revision 1.5  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 1.4  1993/04/30  22:58:26  sar
 * Added mechanism for minimal proxies and coarse grained locks.
 *
 * Revision 1.3  1993/04/26  20:33:14  sar
 * Added bitstrings, and have deletion of parties or contexts delete acls.
 *
 * Revision 1.2  1993/03/03  20:06:55  dab
 * If the config file doesn't set SNMP_SILLY_MARKETING_MACRO
 * then set it to something that wan't make a mess here.
 *
 * Revision 1.1  1993/02/26  17:08:46  dab
 * Initial revision
 *
 * 
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(snmp_inc))
#define snmp_inc

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#include <wrn/wm/common/glue.h>
#include <envoy.h>

/* Define some macros as nothing if the user hasn't defined them */
#if !defined(SNMP_TRACE_INPUT)
#define SNMP_TRACE_INPUT(pkt)
#endif

#if !defined(SNMP_TRACE_OUTPUT)
#define SNMP_TRACE_OUTPUT(pkt)
#endif

/* If we are using the lock code specify coarse lock for everybody to use.
   We also specify a larger macro that gets a lock and if it can't
   executes it's other arguments */
#if INSTALL_ENVOY_SNMP_LOCK

#ifndef ENVOY_SNMP_INIT_LOCK_TAG
#define ENVOY_SNMP_INIT_LOCK_TAG(name, tag) ENVOY_SNMP_INIT_LOCK((name))
#endif

#ifndef ENVOY_SNMP_FREE_LOCK_TAG
#define ENVOY_SNMP_FREE_LOCK_TAG(lock, tag)
#endif

#ifndef SNMP_COARSELOCK_TAG
#define SNMP_COARSELOCK_TAG "COAR"
#endif

#ifndef SNMP_TIMERLOCK_TAG
#define SNMP_TIMERLOCK_TAG "TIMR"
#endif

#ifndef SNMP_INFRASTRUCTURELOCK_TAG
#define SNMP_INFRASTRUCTURELOCK_TAG "INFR"
#endif

#if INSTALL_ENVOY_SNMP_PROXY
#ifndef SNMP_PROXYLOCK_TAG
#define SNMP_PROXYLOCK_TAG "PROX"
#endif
#endif

#if INSTALL_ENVOY_CONTINUE_REENTRANT
#ifndef ENVOY_CONTINUELOCK_TAG
#define ENVOY_CONTINUELOCK_TAG "CONT"
#endif
#endif

#if INSTALL_ENVOY_AGENTX
#ifndef ENVOY_AXLISTLOCK_TAG
#define ENVOY_AXLISTLOCK_TAG "AXLS"
#endif

#ifndef ENVOY_AXCOARSELOCK_TAG
#define ENVOY_AXCOARSELOCK_TAG "AXCO"
#endif
#endif

#if INSTALL_ENVOY_SNMP_VERSION_3
#ifndef ENVOY_V3RETRANSLOCK_TAG
#define ENVOY_V3RETRANSLOCK_TAG "RETR"
#endif
#ifndef ENVOY_V3BOOTSLOCK_TAG
#define ENVOY_V3BOOTSLOCK_TAG "V3BT"
#endif
#endif

ENVOY_SNMP_LOCK_EXTERN(SNMP_CoarseLock);
ENVOY_SNMP_LOCK_EXTERN(SNMP_infrastructure_lock);

#if INSTALL_ENVOY_SNMP_VERSION_3
ENVOY_SNMP_LOCK_EXTERN(SNMP_V3_Retrans_Lock);
ENVOY_SNMP_LOCK_EXTERN(SNMP_V3_Boots_Lock);
#endif

#define ENVOY_SNMP_LOCK_AND_1ARGS(get, lock, com1) \
	if (get(lock)) { com1; }
#define ENVOY_SNMP_LOCK_AND_2ARGS(get, lock, com1, com2) \
	if (get(lock)) { com1; com2; }
#define ENVOY_SNMP_LOCK_AND_3ARGS(get, lock, com1, com2, com3) \
	if (get(lock)) { com1; com2; com3; }
#define ENVOY_SNMP_LOCK_AND_4ARGS(get, lock, com1, com2, com3, com4) \
	if (get(lock)) { com1; com2; com3; com4; }
#else /* INSTALL_ENVOY_SNMP_LOCK */
/* Don't use the lock stuff if the user doesn't define it */
#define ENVOY_SNMP_LOCK(name)
#define ENVOY_SNMP_LOCK_EXTERN(name);
#define ENVOY_SNMP_INIT_LOCK(name)
#define ENVOY_SNMP_INIT_LOCK_TAG(name, tag)
#define ENVOY_SNMP_FREE_LOCK_TAG(lock, tag)
#define ENVOY_SNMP_GET_READ_LOCK(name) 0
#define ENVOY_SNMP_RELEASE_READ_LOCK(name)
#define ENVOY_SNMP_GET_WRITE_LOCK(name) 0
#define ENVOY_SNMP_RELEASE_WRITE_LOCK(name)
#define ENVOY_SNMP_LOCK_AND_1ARGS(get, lock, com1)
#define ENVOY_SNMP_LOCK_AND_2ARGS(get, lock, com1, com2)
#define ENVOY_SNMP_LOCK_AND_3ARGS(get, lock, com1, com2, com3)
#define ENVOY_SNMP_LOCK_AND_4ARGS(get, lock, com1, com2, com3, com4)
#endif /* INSTALL_ENVOY_SNMP_LOCK */

#if (INSTALL_ENVOY_AGENTX_MASTER && INSTALL_ENVOY_SNMP_LOCK)
#define ENVOY_AX_MA_RELEASE_READ_LOCK(name)  ENVOY_SNMP_RELEASE_READ_LOCK(name)
#define ENVOY_AX_MA_RELEASE_WRITE_LOCK(name) ENVOY_SNMP_RELEASE_WRITE_LOCK(name)
#else
#define ENVOY_AX_MA_RELEASE_READ_LOCK(name)
#define ENVOY_AX_MA_RELEASE_WRITE_LOCK(name)
#endif

#endif /* snmp_inc */

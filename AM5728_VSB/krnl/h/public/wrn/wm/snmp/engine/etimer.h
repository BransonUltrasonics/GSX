/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/etimer.h,v 1.2 2001/11/06 21:35:02 josh Exp $ */

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
 * $Log: etimer.h,v $
 * Revision 1.2  2001/11/06 21:35:02  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.5  2001/01/19 22:22:04  paul
 * Update copyright.
 *
 * Revision 9.4  2000/03/17 19:48:34  meister
 * Update copyright notice
 *
 * Revision 9.3  2000/03/09 20:58:33  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.2  2000/03/09 16:55:57  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.1  2000/02/29 20:40:48  josh
 * #ifdefs so we don't have semicolons sitting on lines alone in include
 * files (the ARM compiler hates that).
 *
 * Revision 9.0  1998/10/16 22:10:10  sar
 * Update version stamp to match release
 *
 * Revision 8.3  1998/08/12 04:52:15  sar
 * revamp the initialization routines to limit the amount of
 * code that might get pulled in.
 *
 * Revision 8.2  1998/06/28 21:48:54  sar
 * Added ENVOY_TIMER_32_T for functions that want 32 bit cookies
 * and need to allocate space on the fly.
 *
 * Revision 8.1  1998/02/25 04:50:34  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:25  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:10  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:42  sra
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


#if (!defined(etimer_inc))
#define	etimer_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined snmp_inc)
#include <wrn/wm/snmp/engine/snmp.h>
#endif

/* timer structure. Timer memory allocation is expected to be managed by
 * code which calls into the timer system.
 *
 * Software using timers must call tm_init() on any timer before calling
 * any other tm_ functions on it.
 */

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct ENVOY_TIMER_S {
	struct ENVOY_TIMER_S *link;	/* list link */
	bits32_t	    expire;	/* relative expiration time in ms */
	void (*handler) __((struct ENVOY_TIMER_S *, void *));
					/* expiration handler */
	void		   *cookie;	/* handler cookie */
	} ENVOY_TIMER_T;

/* structure that contains a 32 bit id so the memory can be allocated
   as a block, done here so that multiple routines can use it */
typedef struct ENVOY_TIMER_32_S {
	struct ENVOY_TIMER_S timer;
	bits32_t             id;
	} ENVOY_TIMER_32_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

/* timer calls
 */

void envoy_tm_init	__((ENVOY_TIMER_T *));		/* init timer */
int  envoy_tm_set	__((ENVOY_TIMER_T *, bits32_t));/* set timer */
int  envoy_tm_cancel	__((ENVOY_TIMER_T *));		/* cancel timer */

/* global root pointer and init routine */
#if INSTALL_ENVOY_SNMP_LOCK
ENVOY_SNMP_LOCK_EXTERN(SNMP_TimerLock);
#endif
extern ENVOY_TIMER_T *envoy_timer_list;
void envoy_timer_init   __((void));

#ifdef __cplusplus
}
#endif

#endif /* etimer_inc */

/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/snarklib.h,v 1.3 2003/01/15 13:58:30 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 2000-2001 Wind River Systems, Inc.
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snarklib.h,v $
 * Revision 1.3  2003/01/15 13:58:30  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:27:45  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:37  tneale
 * Tornado shuffle
 *
 * Revision 2.17  2001/01/19 22:23:36  paul
 * Update copyright.
 *
 * Revision 2.16  2000/10/16 19:21:48  paul
 * Restore sockets and mempool code.
 *
 * Revision 2.15  2000/03/17 00:12:13  meister
 * Update copyright message
 *
 * Revision 2.14  2000/03/13 21:22:00  paul
 * Removed some code that we are no longer working on.
 *
 * Revision 2.13  1999/02/18 04:41:31  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 2.12  1998/10/28 18:50:50  josh
 * marging in courier-sep98 branch
 *
 * Revision 2.11.6.1  1998/09/12 19:27:32  sra
 * Minor kludges to make this development snapshot of snark compile with
 * the released versions of the Attache, Envoy, and Emissary products.
 * These changes will need to be un-done when this branch is merged back
 * into the main code base.
 *
 * Revision 2.11.10.2  1998/11/13 18:17:22  wes
 * Make SNARK_*LOCK*() calls into the "real" attache global lock
 * ATTACHE_*LOCK*() and make it live in attache-land, not snark-land.
 *
 * Revision 2.11.10.1  1998/09/23 19:17:12  wes
 * Merge socket-branch-1 changes to socket-branch-2
 *
 * Revision 2.11.8.2  1998/09/11 21:32:02  wes
 * Rename EPI_ prefix thread primitives to ETC_ prefix
 *
 * Revision 2.11.8.1  1998/08/19 13:26:14  wes
 * Merge sockets-pthreads work to shared branch
 *
 * Revision 2.11.2.2  1998/08/12 21:02:46  wes
 * alternate spelling of SNARK_ASSERT_LOCK() because i'm lame..
 *
 * Revision 2.11.2.1  1998/07/28 18:30:41  wes
 * pthreads goop
 *
 * Revision 2.11  1998/06/11 21:40:31  wes
 * Track dhcp invocations
 *
 * Revision 2.10  1998/02/25 04:57:17  sra
 * Update copyrights.
 *
 * Revision 2.9  1997/08/21 17:23:47  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 2.8  1997/07/17 03:38:31  alan
 * Add: snark_reset_read_fdproc & snark_reset_write_fdproc.  Like
 * snark_set_fdprocs, but only makes you mess with the procedure you're
 * interested in.
 *
 * Revision 2.7  1997/05/21 00:18:09  alan
 * Expose declaration of snark_set_fdprocs in all environments.
 *
 * Revision 2.6  1997/05/19 04:26:50  alan
 * Let others have access to the basic file descriptor
 * select() loop in the task dispatcher.
 *
 * Revision 2.5  1997/03/20 06:53:23  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 2.4  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 2.3  1997/02/24 00:56:59  sra
 * Support for run-time configuration of BUG() macros.
 *
 * Revision 2.2  1997/02/19 08:10:29  sra
 * More fun merging snmptalk into snark, general snark cleanup.
 *
 * Revision 2.1  1996/03/22 10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.4  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1993/07/29  04:24:47  sra
 * PTCP_DNS_H changed to ATTACHE_DNS_H for release 2.0.
 *
 * Revision 1.1  1993/07/05  21:53:06  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_SNARK_H_
#define	_SNARK_H_

#ifndef COMMON_CONFIG_H
#include <wrn/wm/common/config.h>
#endif

#include <snark.h>

#include <wrn/wm/demo/keyboard.h>
#include <wrn/wm/demo/parse.h>
#include <wrn/wm/demo/sty.h>
#include <wrn/wm/demo/tasks.h>
#include <wrn/wm/demo/main.h>
#include <wrn/wm/demo/keyword.h>

#ifndef COMMON_GLUE_H
#include <wrn/wm/common/glue.h>
#endif

#if INSTALL_SNARK_BUG_CONF
#include <wrn/wm/demo/bug_conf.h>
#endif

#ifdef INSTALL_SNARK_THREADS
extern void task_kick(void);
#endif

extern void snark_init (void);
extern void snark_shutdown (void);

/* Not all Snark ports support this feature. */
typedef void (*fdproc_t)(int, void *);
extern int snark_set_fdprocs(int, void *, fdproc_t, fdproc_t);
extern int snark_reset_read_fdproc(int, fdproc_t);
extern int snark_reset_write_fdproc(int, fdproc_t);

/*
 * netconfig() sets this up from the config file, it's available
 * for use by applications.
 */
#if INSTALL_ATTACHE_DNS
#include <wrn/wm/attache/dns.h>
extern struct dns_config domain_config;
#endif

#if INSTALL_ATTACHE_DHCP
#include <wrn/wm/attache/dhcp.h>

extern dhcp_stateblk_t *snark_dhcp_find(net_if *net);

typedef int (*snark_dhcpwalk_fn_t)(void *, dhcp_stateblk_t *, net_if *);

extern int snark_dhcp_walk(snark_dhcpwalk_fn_t callback, void *cookie);

extern void snark_dhcp_add (net_if *net, dhcp_stateblk_t *);

extern int snark_dhcp_set_punt (dhcp_stateblk_t *, int);

extern int snark_dhcp_get_punt (dhcp_stateblk_t *);

extern void snark_dhcp_remove (dhcp_stateblk_t *);

#endif

#endif /* _SNARK_H_ */

/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/bsd44/emissary.h,v 1.3 2003/01/15 14:00:26 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1996-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* 
 * $Log: emissary.h,v $
 * Revision 1.3  2003/01/15 14:00:26  josh
 * directory structure shifting
 *
 * Revision 1.2  2002/02/26 18:22:52  josh
 * updating paths in port files
 *
 * Revision 1.1.1.1  2001/11/05 17:47:08  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:22:54  paul
 * Update copyright.
 *
 * Revision 1.6  2000/10/16 19:21:44  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.5  2000/03/17 00:08:56  meister
 * Update copyright message
 *
 * Revision 1.4  2000/03/13 21:21:53  paul
 * Removed some code that we are no longer working on.
 *
 * Revision 1.3  1999/04/09 15:43:11  wes
 * Call a kludge a kludge..
 *
 * Revision 1.2  1999/02/18 04:41:25  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.1.10.2  1998/11/17 20:12:57  wes
 * one more SNARK->ATTACHE_THREADS change..
 *
 * Revision 1.1.10.1  1998/09/23 19:16:57  wes
 * Merge socket-branch-1 changes to socket-branch-2
 *
 * Revision 1.1.8.1  1998/08/19 13:25:51  wes
 * Merge sockets-pthreads work to shared branch
 *
 * Revision 1.1.2.1  1998/08/04 19:47:39  wes
 * XXX XXX work around bug in mit-pthreads on NetBSD
 *
 * Revision 1.1  1998/02/25 04:56:47  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:11  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:57:59  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/23 00:18:21  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/


#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/file.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

#if INSTALL_EMISSARY_THREAD_KLUDGE
/*
 * Work around bug in mit-pthreads on NetBSD which requires
 * threads library to be inited even if they're not in use..
 */
#include <wrn/wm/common/thread.h>
#define EMISSARY_OS_DEPENDENT_INIT(argc,argv) GLUE_THREAD_INIT()
#endif

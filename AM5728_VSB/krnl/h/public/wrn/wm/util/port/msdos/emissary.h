/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/emissary.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1992-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: emissary.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:03  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:27  meister
 * Update copyright message
 *
 * Revision 1.2  1998/10/08 19:31:45  paul
 * small syntax cleanup
 *
 * Revision 1.1  1998/02/25 04:56:57  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:20  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.4  1997/03/20 06:57:48  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.2  1996/10/28 17:42:54  mrf
 * undefine MEM_AVAIL if we are on msvc4.0 (which has no _memavl())
 *
 * Revision 0.1  1996/10/23  00:12:55  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>

/*
 * WATCOM C/C++ 9.5 warns about constructs like "defined(foo) && foo",
 * so explicitly zero all the compiler install tests we know it will find.
 */

#ifndef INSTALL_on_bcc
#define INSTALL_on_bcc 0
#endif

#ifndef INSTALL_on_microsoft
#define INSTALL_on_microsoft 0
#endif

#ifndef INSTALL_on_microsoft
#define INSTALL_on_microsoft 0
#endif

#ifndef INSTALL_on_msvc40
#define INSTALL_on_msvc40 0
#endif

#ifndef INSTALL_on_ztc
#define INSTALL_on_ztc 0
#endif

#if INSTALL_on_bcc
#include <memory.h>
#include <alloc.h>
#define	MEM_AVAIL() ((unsigned long) coreleft())
#endif

#if INSTALL_on_microsoft
#include <memory.h>
#include <malloc.h>
#define	MEM_AVAIL() ((unsigned long) _memavl())
#endif

/* No _memavl() on msvc40 */
#if INSTALL_on_msvc40
#undef MEM_AVAIL
#endif

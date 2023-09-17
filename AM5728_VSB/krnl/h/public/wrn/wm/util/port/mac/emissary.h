/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/mac/emissary.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

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
 * Revision 1.3  2001/01/19 22:23:01  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:10:15  meister
 * Update copyright message
 *
 * Revision 1.1  1998/02/25 04:56:52  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:16  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:57:56  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/28 20:35:56  sar
 * Initial version
 *
 */

/* [clearcase]
modification history
-------------------
*/



#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <console.h>
#include <assert.h>
#include <limits.h>

/*
 * Pathname separator character.
 */
#define	EMISSARY_PATH_SEPARATOR	':'

#if __option(profile)
#include <profile.h>
#endif

/*
 * I don't understand how the Macintosh works well enough to know
 * if these are the right values in all cases.  Change them if not.
 *
 * If you don't want the file type and file creator stuff at all,
 * define these macros to expand to nothing.
 */
#if 1
#define	EMISSARY_MACINTOSH_SET_FILE_TYPE \
  { extern long _ftype; _ftype = 'TEXT'; }
#define	EMISSARY_MACINTOSH_SET_FILE_CREATOR \
  { extern long _fcreator; _fcreator = 'MPS '; }
#else
#define	EMISSARY_MACINTOSH_SET_FILE_TYPE
#define	EMISSARY_MACINTOSH_SET_FILE_CREATOR
#endif

/*
 * OS-specific initialization code for the Macintosh with Think C.
 * This is ugly, if you can think of a better way to do it that
 * doesn't embed Mac-specific code in the guts of the MIB compiler,
 * tell us.
 */

#if __option(profile)
#define EMISSARY_MACINTOSH_INITPROFILE	InitProfile(512, 200)
#else
#define EMISSARY_MACINTOSH_INITPROFILE
#endif

#define	EMISSARY_OS_DEPENDENT_INIT(argc,argv) \
  do { \
    extern struct __copt console_options; \
    console_options.nrows = 25; \
    argc = ccommand(&argv); \
    EMISSARY_MACINTOSH_INITPROFILE; \
    EMISSARY_MACINTOSH_SET_FILE_TYPE; \
    EMISSARY_MACINTOSH_SET_FILE_CREATOR; \
  } while (0)

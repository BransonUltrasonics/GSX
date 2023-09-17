/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/sunos/snark.h,v 1.1.1.1 2001/11/05 17:47:16 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snark.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:18  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:11:01  meister
 * Update copyright message
 *
 * Revision 1.2  1999/05/21 16:45:24  sra
 * vsnprintf() => VSNPRINTF().
 *
 * Revision 1.1  1998/02/25 04:57:11  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:24  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.6  1997/05/17 23:08:53  sra
 * Clean up some SunOS/gcc warnings.
 *
 * Revision 0.5  1997/03/20 06:57:54  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.4  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.3  1997/02/24 00:56:59  sra
 * Support for run-time configuration of BUG() macros.
 *
 * Revision 0.2  1997/02/19 08:10:29  sra
 * More fun merging snmptalk into snark, general snark cleanup.
 *
 * Revision 0.1  1996/10/23 00:14:56  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

#define	SNARK_ATTACHE_MAXLNH		20

#ifndef GLUE_ALLOC
#define	GLUE_ALLOC(x)			((void *) malloc(x))
#endif

#ifndef GLUE_FREE
#define GLUE_FREE(x)			free((char *) (x))
#endif

#ifndef MEMCPY
#define MEMCPY(x, y, z)			memcpy(x, y, z)
#endif

#ifndef MEMCMP
#define MEMCMP(x, y, z)			memcmp(x, y, z)
#endif

#ifndef MEMSET
#define MEMSET(x, y, z)			memset(x, y, z)
#endif

#ifndef STRCMP
#define STRCMP(x, y)			strcmp(x, y)
#endif

#ifndef STRCPY
#define STRCPY(x, y)			strcpy(x, y)
#endif

#ifndef STRLEN
#define STRLEN(x)			strlen(x)
#endif

#ifndef STRNICMP
#define	STRNICMP(x, y, z)		strncasecmp(x, y, z)
#endif

#ifndef STRICMP
#define	STRICMP(x, y)			strcasecmp(x, y)
#endif

#define stricmp				STRICMP
#define strnicmp			STRNICMP

/* This stuff all should be defined in some system header file or another, but I don't see it. */
extern int strcasecmp(), strncasecmp(), gettimeofday(), ioctl(), bzero(), select();
extern int setlinebuf(), sendto(), random(), recv(), socket(), bind();

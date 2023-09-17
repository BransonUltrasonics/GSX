/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/mach386/emissary.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

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
 * Revision 1.3  2001/01/19 22:23:02  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:10:19  meister
 * Update copyright message
 *
 * Revision 1.1  1998/02/25 04:56:53  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:17  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:58:06  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/28 20:30:07  sar
 * Initial version
 *
 */

/* [clearcase]
modification history
-------------------
*/



/*
 * The headers for this system are really screwed up, particularly
 * when dealing with two different versions of gcc in ANSI mode.
 * This will never be pretty, just grit your teeth.
 */

#define	__GCC2__ (defined(__GNUC__) && __GNUC__ >= 2)

#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>

#if !__GCC2__
#include <libc.h>
#endif

#include <ctype.h>

#if !__GCC2__
#include <memory.h>
#endif

#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

/*
 * This is a pain, but the Mach386 header files don't declare this stuff
 * if you're compiling with -traditional, and we want to avoid getting
 * compiler warnings. It seems to be kind of arbitrary as to which functions
 * are actually declared in the header files in this case.
 */
#if !__STDC__
extern int fflush(FILE*);
extern void exit(int);
extern int ungetc(int, FILE*);
extern int atoi(char *);
extern void perror(char *);
extern int fclose(FILE*);
extern int printf(char*,...);
extern int access(char *, int);
extern int _filbuf(FILE*);
extern void free(void *);
#endif /* !__STDC__ */

/*
 * Definitions to suppress warnings from dumb system header files.
 */
#define	MEMCPY(x,y,z) memcpy((char *) (x), (const char *) (y), (int) (z))
#define MEMSET(x,y,z) memset((char *) (x), (int) (y), (int) (z))
#define MEMCMP(x,y,z) memcmp((const char *) (x), (const char *) (y), (int) (z))
#define MEMCHR(x,y,z) memchr((const char *) (x), (int) (y), (int) (z))

/*
 * These should be in the files <stddef.h>, <stdlib.h>, and <unistd.h>,
 * but the versions of those that gcc-2.5.5 generated on Mach386 have
 * everything conditionalized for C++ only, so they're useless.  Sigh.
 */
#if __GCC2__
extern void *memchr(const void *, int, long unsigned int);
extern int memcmp(const void *, const void *, long unsigned int);
extern void *memcpy(void *, const void *, long unsigned int);
extern void *memset(void *, int, long unsigned int);
extern char *getenv (const char *);
extern int atoi(const char *);
extern void *malloc(long unsigned int);
extern void free(void *);
extern void *realloc(void *, long unsigned int);
#endif

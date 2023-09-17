/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/sunos/emissary.h,v 1.1.1.1 2001/11/05 17:47:16 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:18  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:11:00  meister
 * Update copyright message
 *
 * Revision 1.1  1998/02/25 04:57:08  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:23  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:57:52  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/23 00:14:56  sar
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
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>


/*
 * The following declarations shouldn't be necessary, particularly the ones
 * that are stdio internal functions, but GCC whines without them.  Should
 * be harmless in any case.
 */
extern char *getenv (const char *);
extern int _filbuf (FILE *);
extern int _flsbuf (unsigned int, FILE *);
extern int fclose (FILE *);
extern int fprintf (FILE *, const char *, ...);
extern int fputs (const char *, FILE *);
extern int fseek (FILE *, long int, int);
extern int tolower (int);
extern int ungetc (int, FILE *);
extern long time (long *);
extern unsigned long fwrite (const void *,unsigned long,unsigned long,FILE *);
extern void perror (const char *);

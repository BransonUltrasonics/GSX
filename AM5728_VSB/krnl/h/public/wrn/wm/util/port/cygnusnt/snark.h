/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/cygnusnt/snark.h,v 1.1.1.1 2001/11/05 17:47:09 tneale Exp $ */

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
 * $Log: snark.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/07/10 15:55:15  mrf
 * Add configuration for Cygnus NT environment with associated changes.
 *
 * Revision 1.3  2000/03/17 00:08:57  meister
 * Update copyright message
 *
 * Revision 1.2  1998/02/25 04:56:49  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/08/21 17:23:45  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 * Revision 1.0  1997/08/08 22:11:13  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.4  1997/03/20 06:58:00  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/03/09 18:37:03  sra
 * Add STRICMP().
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


#include <string.h>
#include <stdlib.h>

#ifndef GLUE_ALLOC
#define	GLUE_ALLOC(x)			((void *) malloc(x))
#endif
#ifndef GLUE_FREE
#define GLUE_FREE(x)			free((char *) (x))
#endif

#ifndef STRICMP
#define	STRICMP(x, y)			strcasecmp(x, y)
#endif
#ifndef STRNICMP
#define	STRNICMP(x, y, z)		strncasecmp(x, y, z)
#endif

#define NO_ETHADDR_STRUCT 1

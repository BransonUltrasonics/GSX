/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/cygnusnt/common.h,v 1.1.1.1 2001/11/05 17:47:08 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: common.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:08  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/07/10 15:55:14  mrf
 * Add configuration for Cygnus NT environment with associated changes.
 *
 * Revision 1.3  2000/03/17 00:08:56  meister
 * Update copyright message
 *
 * Revision 1.2  1998/02/25 04:56:46  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/08/21 17:23:45  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 */

/* [clearcase]
modification history
-------------------
*/


/*
 * Common definitions for all Epilogue products on 4.4-BSD derived systems.
 */

#ifdef CONFIG_PREAMBLE

#include <string.h>

#ifndef STRICMP
#define	STRICMP(x, y)			strcasecmp(x, y)
#endif

#ifndef STRNICMP
#define	STRNICMP(x, y, z)		strncasecmp(x, y, z)
#endif

#else /* CONFIG_PREAMBLE */

#ifndef GLUE_NOW
#define	GLUE_NOW()			glue_now()
extern bits32_t glue_now (void);
#endif

#ifndef GLUE_SEED_PRNG
#define	GLUE_SEED_PRNG(x,y)		glue_seed_prng(x,y)
extern unsigned glue_seed_prng(bits8_t *, unsigned);
#endif

#endif /* CONFIG_PREAMBLE */

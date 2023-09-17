/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/common.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:23:03  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:10:27  meister
 * Update copyright message
 *
 * Revision 1.5  1998/06/21 21:43:51  sar
 * Dos compilers seem to use stricmp and strnicmp instead of casecmp
 *
 * Revision 1.4  1998/02/25 04:56:57  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/12/02 16:15:15  mrf
 * add GLUE_NOW definition to make prng.c build on DOS
 *
 * Revision 1.2  1997/10/29 20:32:33  sar
 * Added memory.h for some compilers to get prototypes for mem functions.
 *
 * Revision 1.1  1997/08/21 17:23:46  sra
 * Begin moving configuration stuff that's common to all products to common.h
 * Minor cleanups to common/lib/prng.c.  Add pnrg seed function to snarkbsd.
 *
 */

/* [clearcase]
modification history
-------------------
*/



/*
 * NIY....
 */

#ifdef CONFIG_PREAMBLE

/* zortech, watcom and intel 960 don't have a memory.h so away it goes */
#if (!defined(INSTALL_on_ztc) && !defined(INSTALL_on_watcom) && !defined(INSTALL_on_i960))
#include <memory.h>
#endif /* INSTALL_on_ztc */

#ifndef STRICMP
#define STRICMP(x, y)  stricmp(x, y)
#endif

#ifndef STRNICMP
#define STRNICMP(x, y, z) strnicmp(x, y, z)
#endif

#else /* CONFIG_PREAMBLE */

#ifndef GLUE_NOW
#define	GLUE_NOW()			glue_now()
extern bits32_t glue_now (void);
#endif

#endif /* CONFIG_PREAMBLE */










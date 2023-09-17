/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/common.h,v 1.1.1.1 2001/11/05 17:47:15 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:15  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/06/25 19:43:11  markadam
 * Remove the gcc only inline code for MEMCMP. This forces the compiler to
 * use the definition in glue.h, which aliases a call to etc_memcmp.
 *
 * Revision 1.4  2001/01/19 22:23:15  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:53  meister
 * Update copyright message
 *
 * Revision 1.2  1998/07/29 20:54:47  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/



/*
 * Common definitions for all Epilogue products on Solaris.
 */

#ifdef CONFIG_PREAMBLE

#include <string.h>
#include <memory.h>

/* things that should be in some system header file but don't seem to be */
extern int strncasecmp();

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

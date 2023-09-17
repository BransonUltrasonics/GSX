/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/sunos/common.h,v 1.1.1.1 2001/11/05 17:47:16 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:23:17  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:11:00  meister
 * Update copyright message
 *
 * Revision 1.5  1998/06/19 19:28:46  sar
 * added a memcmp macro to deal with the fact that the sunos memcmp
 * uses signed comparisons.  This is the macro from attache.h, it is
 * added here so other products can use it.
 *
 * Revision 1.4  1998/06/10 16:39:04  sar
 * Added the memory.h include file
 *
 * Revision 1.3  1998/02/25 04:57:07  sra
 * Update copyrights.
 *
 * Revision 1.2  1997/08/22 16:30:45  sra
 * Flesh out SunOS-specific stuff.
 *
 * Revision 1.1  1997/08/21 17:23:47  sra
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
#include <memory.h>

/* things that should be in some system header file but don't seem to be */
extern int strncasecmp();

#ifndef STRICMP
#define	STRICMP(x, y)			strcasecmp(x, y)
#endif

#ifndef STRNICMP
#define	STRNICMP(x, y, z)		strncasecmp(x, y, z)
#endif

/*
 * SunOS 4.1.4's memcmp() does signed character comparision, so replace it.
 * This inline code will only work with gcc, but this particular port file
 * is only used for testing and demos anyway, so what the heck.
 */
#ifndef MEMCMP
#define MEMCMP(x, y, z)	\
  ({ \
    register unsigned char *_x_ = (void *) (x), *_y_ = (void *) (y); \
    register unsigned _z_ = (z); \
    int _r_ = 0; \
    while (_z_-- > 0) \
      if ((_r_ = (*_x_++) - (*_y_++)) != 0) \
	break; \
    _r_; \
  })
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

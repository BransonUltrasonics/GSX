/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/sunos/oemtypes.h,v 1.1.1.1 2001/11/05 17:47:16 tneale Exp $ */

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
 * $Log: oemtypes.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:18  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:11:00  meister
 * Update copyright message
 *
 * Revision 1.2  1998/06/10 16:37:21  sar
 * Added stdtypes to get the size_t construct
 *
 * Revision 1.1  1998/02/25 04:57:10  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:24  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:57:54  sra
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



/* get the size_t construct */
#include <sys/stdtypes.h>

/* The sun compiler doesn't understand the `signed' keyword */

#define __TYPES_HAVE_sbits8_t_ 1
typedef char	sbits8_t;	/* signed 8 bits */
#define __TYPES_HAVE_sbits16_t_ 1
typedef short	sbits16_t;	/* signed 16 bits */
#define __TYPES_HAVE_sbits32_t_ 1
typedef long	sbits32_t;	/* signed 32 bits */


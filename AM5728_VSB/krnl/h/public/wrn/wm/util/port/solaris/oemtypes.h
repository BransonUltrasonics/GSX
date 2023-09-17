/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/oemtypes.h,v 1.1.1.1 2001/11/05 17:47:15 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:15  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:16  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:54  meister
 * Update copyright message
 *
 * Revision 1.2  1998/07/29 20:54:49  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* get the size_t construct */
#include <sys/types.h>

#if 0
/* The sun compiler doesn't understand the `signed' keyword */

#define __TYPES_HAVE_sbits8_t_ 1
typedef char	sbits8_t;	/* signed 8 bits */
#define __TYPES_HAVE_sbits16_t_ 1
typedef short	sbits16_t;	/* signed 16 bits */
#define __TYPES_HAVE_sbits32_t_ 1
typedef long	sbits32_t;	/* signed 32 bits */
#endif

#define __TYPES_HAVE_boolean_t_ 1

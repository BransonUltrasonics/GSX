/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/vxworks/oemtypes.h,v 1.1.1.1 2001/11/05 17:47:17 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: oemtypes.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/01/27 18:25:29  josh
 * initial checkin of porting files for vxWorks
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/



#include <vxWorks.h>
#include <sys/types.h>

/* The sun compiler doesn't understand the `signed' keyword */

#define __TYPES_HAVE_sbits8_t_ 1
typedef char    sbits8_t;       /* signed 8 bits */
#define __TYPES_HAVE_sbits16_t_ 1
typedef short   sbits16_t;      /* signed 16 bits */
#define __TYPES_HAVE_sbits32_t_ 1
typedef int    sbits32_t;      /* signed 32 bits */


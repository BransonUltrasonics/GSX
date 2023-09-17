/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/keyboard.h,v 1.1.1.1 2001/11/05 17:47:32 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * Declarations for system-independent low-level keyboard handler
 * and associated output device (ie, the local console, if any).
 */

/*
 * $Log: keyboard.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 2.6  2001/01/19 22:23:35  paul
 * Update copyright.
 *
 * Revision 2.5  2000/03/17 00:12:12  meister
 * Update copyright message
 *
 * Revision 2.4  1998/02/25 04:57:14  sra
 * Update copyrights.
 *
 * Revision 2.3  1997/03/20 06:53:20  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 2.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 2.1  1996/03/22 10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.4  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1993/07/31  23:25:22  sra
 * Add keyboard_write().
 *
 * Revision 1.1  1993/07/05  21:53:06  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_SNARK_KEYBOARD_H_
#define	_SNARK_KEYBOARD_H_

extern void keyboard_handler (void (*)(unsigned char *, size_t));
extern void keyboard_write (unsigned char *, size_t);

#endif

/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/dlpi.h,v 1.1 2001/11/08 16:00:57 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: dlpi.h,v $
 * Revision 1.1  2001/11/08 16:00:57  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.3  2001/01/19 22:23:40  paul
 * Update copyright.
 *
 * Revision 2.2  2000/03/17 00:12:38  meister
 * Update copyright message
 *
 * Revision 2.1  1998/07/29 20:54:51  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 * Revision 1.1.2.1  1998/07/24 21:45:43  wes
 * Initial implementation
 *
 *
 */

/*
 * DLPI interface to ethernet for Attache testing under Solaris 2.x
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_DLPI_H_
#define	_DLPI_H_

extern int dlpi_open (char *, unsigned, unsigned char *);
extern int dlpi_write (int, unsigned char *, unsigned);
extern void dlpi_close (int);
extern void dlpi_find (void (*)(char *, void *), void *);
extern int dlpi_read (int, unsigned,
		     void (*)(unsigned char *, unsigned, unsigned, void *),
		     void *);

#endif	/* _DLPI_H_ */

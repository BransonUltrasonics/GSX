/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/nit.h,v 1.1 2001/11/08 16:00:57 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: nit.h,v $
 * Revision 1.1  2001/11/08 16:00:57  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.8  2001/01/19 22:23:46  paul
 * Update copyright.
 *
 * Revision 2.7  2000/03/17 00:12:40  meister
 * Update copyright message
 *
 * Revision 2.6  1998/07/31 19:45:46  wes
 * Put packet filtering into NIT driver...
 *
 * Revision 2.5  1998/02/25 04:57:28  sra
 * Update copyrights.
 *
 * Revision 2.4  1997/05/02 02:45:50  sra
 * Support NIT in promiscuous mode, for IPv6 multicast.
 *
 * Revision 2.3  1997/03/20 06:52:59  sra
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
 * Revision 1.3  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.2  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.1  1994/01/09  23:57:29  sra
 * Add support for NIT under SunOS 4.1.2.
 *
 */

/*
 * NIT interface to ethernet for Attache testing under SunOS 4.1.2.
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_NIT_H_
#define	_NIT_H_

extern int nit_open (char *, unsigned, unsigned char *);
extern int nit_write (int, unsigned char *, unsigned);
extern void nit_close (int);
extern void nit_find (void (*)(char *, void *), void *);
extern int nit_read (int, unsigned,
		     void (*)(unsigned char *, unsigned, unsigned, void *),
		     void *);

#endif	/* _NIT_H_ */

/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/bpf.h,v 1.1 2001/11/08 16:00:56 tneale Exp $ */

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
 * $Log: bpf.h,v $
 * Revision 1.1  2001/11/08 16:00:56  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.7  2001/01/19 22:23:39  paul
 * Update copyright.
 *
 * Revision 2.6  2000/03/17 00:12:36  meister
 * Update copyright message
 *
 * Revision 2.5  1998/02/25 04:57:21  sra
 * Update copyrights.
 *
 * Revision 2.4  1997/04/25 01:02:26  sra
 * Fix code to set explicit MAC addresses for BPF, allow non-promiscuous mode.
 *
 * Revision 2.3  1997/03/20 06:52:51  sra
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
 * Revision 1.1  1993/07/05  21:53:30  sra
 * Initial revision
 *
 */

/*
 * BPF interface to ethernet for attache testing under 386BSD/NetBSD/FreeBSD.
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_BPF_H_
#define	_BPF_H_

extern int bpf_open (char *, unsigned, unsigned char *, int);
extern int bpf_write (int, unsigned char *, unsigned);
extern void bpf_close (int);
extern void bpf_find (void (*)(char *, void *), void *);
extern int bpf_read (int, unsigned,
		     void (*)(unsigned char *, unsigned, unsigned, void *),
		     void *);

#endif	/* _BPF_H_ */

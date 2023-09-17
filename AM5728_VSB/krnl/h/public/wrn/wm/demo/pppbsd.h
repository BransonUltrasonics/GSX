/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/pppbsd.h,v 1.1 2001/11/08 16:00:59 tneale Exp $ */

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
 * $Log: pppbsd.h,v $
 * Revision 1.1  2001/11/08 16:00:59  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.4  2001/01/19 22:23:50  paul
 * Update copyright.
 *
 * Revision 2.3  2000/03/17 00:12:43  meister
 * Update copyright message
 *
 * Revision 2.2  1998/02/25 04:57:35  sra
 * Update copyrights.
 *
 * Revision 2.1  1997/10/28 00:51:55  sra
 * Convert PPP code to use BUG() macro.
 * Bang on CHAP and PAP code some more.
 * Create test driver for PPP under Snark/BSD.
 *
 */

/*
 * Point-to-Point Protocol (PPP) interface for snark under BSD-derived systems.
 *
 * This code has only been tested under FreeBSD 2.x.  It should work on
 * any BSD-derived system with minor modifications.
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _PPPBSD_H_
#define _PPPBSD_H_

extern void ppp_driver_find
  (void (*config)(char *, struct driver *, int, bits16_t, unsigned, bits32_t));

#endif /* _PPPBSD_H_ */

/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/snoopdrv.h,v 1.1 2001/11/08 16:00:59 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 2000-2001 Wind River Systems, Inc.
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snoopdrv.h,v $
 * Revision 1.1  2001/11/08 16:00:59  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 1.6  2001/01/19 22:23:53  paul
 * Update copyright.
 *
 * Revision 1.5  2000/03/17 00:12:46  meister
 * Update copyright message
 *
 * Revision 1.4  1998/02/25 04:57:41  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/03/20 06:53:13  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 1.1  1996/11/13 15:06:46  mrf
 * Initial revision
 *
 * Revision 2.1  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.3  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.2  1994/09/04  06:13:38  sra
 * Clean up antique type names and install macros.
 *
 * Revision 1.1  1994/01/09  23:57:29  sra
 * Add support for NIT under SunOS 4.1.2.
 *
 */

/*
 * Attache code for interfacing to ethernet via SNOOP on Irix.
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _SNOOPDRV_H_
#define _SNOOPDRV_H_

extern void snoop_driver_find
  (void (*config)(char *, struct driver *, int, bits16_t, unsigned, bits32_t));

#endif /* _SNOOPDRV_H_ */

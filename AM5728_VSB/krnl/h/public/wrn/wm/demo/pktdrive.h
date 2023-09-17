/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/pktdrive.h,v 1.1 2001/11/08 16:00:58 tneale Exp $ */

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
 *****************************************************************************/

/*
 * $Log: pktdrive.h,v $
 * Revision 1.1  2001/11/08 16:00:58  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.9  2001/01/19 22:23:49  paul
 * Update copyright.
 *
 * Revision 2.8  2000/03/17 00:12:43  meister
 * Update copyright message
 *
 * Revision 2.7  1998/02/25 04:57:34  sra
 * Update copyrights.
 *
 * Revision 2.6  1997/07/23 01:42:34  mrf
 * remove redundant constants
 *
 * Revision 2.5  1997/07/22 20:33:47  mrf
 * put constants in correct include file
 *
 * Revision 2.4  1997/07/01 02:23:00  mrf
 * packet drivers still need packet types byteswapped.
 *
 * Revision 2.3  1997/03/20 06:53:08  sra
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
 * Revision 1.2  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.1  1993/07/30  02:41:24  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _PKTDRIVE_H_
#define _PKTDRIVE_H_

extern void pktd_driver_find(boolean_t (*config)(struct net *));

#endif /* _PKTDRIVE_H_ */

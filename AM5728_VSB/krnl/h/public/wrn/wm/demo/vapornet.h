/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/vapornet.h,v 1.1 2001/11/08 16:01:00 tneale Exp $ */

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
 *  Copyright 1995-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: vapornet.h,v $
 * Revision 1.1  2001/11/08 16:01:00  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.6  2001/01/19 22:23:56  paul
 * Update copyright.
 *
 * Revision 2.5  2000/03/17 00:12:49  meister
 * Update copyright message
 *
 * Revision 2.4  1998/02/25 04:57:45  sra
 * Update copyrights.
 *
 * Revision 2.3  1997/03/20 06:53:18  sra
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
 * Revision 1.1  1995/03/26  02:29:19  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

/*
 * Fake network built on BSD "unix domain" sockets.  Intended primarily
 * to allow building a complex enough network to do some testing of router
 * code in a relatively sane debugging environment.
 */

#ifndef _VAPORNET_H_
#define _VAPORNET_H_

extern void vp_driver_find
  (void (*config)(char *, struct driver *, int, bits16_t, unsigned, bits32_t));

#endif /* _VAPORNET_H_ */

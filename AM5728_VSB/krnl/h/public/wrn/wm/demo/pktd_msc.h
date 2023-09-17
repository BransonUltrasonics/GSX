/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/pktd_msc.h,v 1.1 2001/11/08 16:00:58 tneale Exp $ */

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
 * $Log: pktd_msc.h,v $
 * Revision 1.1  2001/11/08 16:00:58  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.7  2001/01/19 22:23:49  paul
 * Update copyright.
 *
 * Revision 2.6  2000/03/17 00:12:42  meister
 * Update copyright message
 *
 * Revision 2.5  1998/02/25 04:57:31  sra
 * Update copyrights.
 *
 * Revision 2.4  1997/03/20 06:53:06  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 2.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 2.2  1996/10/30 16:29:13  sra
 * Conditionalize definition of MK_FP.  I no longer remember why we were
 * defining this ourselves in the first place.
 *
 * Revision 2.1  1996/03/22  10:05:39  sra
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

/*
 * This file contains system and compiler specific information so that the
 * source doesn't have to be ifdef'd for all the various compilers
 * we try and support.
 *
 * This file is for Microsoft C++.
 */

/*
 * Compiler include files - these are development-system dependent
 */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

/*
 * Each compiler defines what its saveregs structure looks like.  The packet
 * driver code requires that it have fields named like below.
 */
struct saveregs {
  unsigned int es;
  unsigned int ds;
  unsigned int di;
  unsigned int si;
  unsigned int bp;
  unsigned int sp;
  unsigned int bx;
  unsigned int dx;
  unsigned int cx;
  unsigned int ax;
};

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef MK_FP
#define MK_FP(seg,ofs) \
  ((void far *)( (((unsigned long)seg) << 16) + (unsigned long)ofs))
#endif

/*
 * This macro expands into a routine which can be the upcalled routine from
 * a packet driver.  It should save all its registers on entry into a structure
 * and pass that structure, along with a handle argument up to an upcall
 * routine argument.
 */
#define PD_UPCALL(routine_name, handle, upcall) \
  void far _saveregs _loadds routine_name(struct saveregs r) \
{ \
    struct saveregs far *rp; \
    unsigned char far *ret; \
\
    rp = (struct saveregs far *) \
      ((char *)(&r) - (sizeof(struct saveregs) + 4)); \
    ret = upcall(rp, handle); \
    rp->es = FP_SEG(ret); \
    rp->di = FP_OFF(ret); \
}

/*
 * This typedef must define PD_UPCALL_RTN_T to be a routine of the same type
 * as the PD_UPCALL macro generates.
 */
typedef void far _saveregs _loadds PD_UPCALL_RTN_T(struct saveregs r);

/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/dlpidrv.h,v 1.1 2001/11/08 16:00:57 tneale Exp $ */

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
 * $Log: dlpidrv.h,v $
 * Revision 1.1  2001/11/08 16:00:57  tneale
 * Adding header files originally from snark/lib
 *
 * Revision 2.4  2001/01/19 22:23:41  paul
 * Update copyright.
 *
 * Revision 2.3  2000/03/17 00:12:38  meister
 * Update copyright message
 *
 * Revision 2.2  1998/09/14 20:13:43  wes
 * make DLPI driver filenames 8.3 safe..
 *
 * Revision 2.1  1998/07/29 20:54:52  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 * Revision 1.1.2.1  1998/07/24 21:45:46  wes
 * Initial implementation
 *
 */

/*
 * Attache code for interfacing to ethernet via the Data Link Protocol Interface
 * on Solaris 2.x
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _DLPIDRV_H_
#define _DLPIDRV_H_

extern void dlpi_driver_find
  (void (*config)(char *, struct driver *, int, bits16_t, unsigned, bits32_t));

#endif /* _DLPIDRV_H_ */

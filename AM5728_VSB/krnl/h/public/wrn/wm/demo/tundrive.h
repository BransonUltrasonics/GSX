/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/tundrive.h,v 1.1 2001/11/08 16:01:00 tneale Exp $ */

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

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

extern void tun_driver_find
  (void (*config)(char *, struct driver *, int, bits16_t, unsigned, bits32_t));

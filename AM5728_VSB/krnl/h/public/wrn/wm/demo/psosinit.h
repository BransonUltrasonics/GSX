/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/psosinit.h,v 1.1.1.1 2001/11/05 17:47:36 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1999 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: psosinit.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:36  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:36  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:12:13  meister
 * Update copyright message
 *
 * Revision 1.1  1999/04/09 20:56:48  wes
 * declare some functions exported by psostty.c for root.c to get at..
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

extern void term_setup (void);
extern unsigned long CreateKbdTask (unsigned long priority,
                             unsigned long kbd_queue_id, 
                             unsigned long attache_task_id);

extern int StartKbdTask (void);





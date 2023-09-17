/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/tasks.h,v 1.1.1.1 2001/11/05 17:47:37 tneale Exp $ */

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
 * $Log: tasks.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:37  tneale
 * Tornado shuffle
 *
 * Revision 2.8  2001/01/19 22:23:37  paul
 * Update copyright.
 *
 * Revision 2.7  2000/03/17 00:12:14  meister
 * Update copyright message
 *
 * Revision 2.6  1998/07/02 06:55:34  sra
 * Make Snark restartable under pSOS, and other minor cleanups.
 *
 * Revision 2.5  1998/02/25 04:57:18  sra
 * Update copyrights.
 *
 * Revision 2.4  1997/03/20 06:53:24  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 2.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 2.2  1997/02/19 08:10:29  sra
 * More fun merging snmptalk into snark, general snark cleanup.
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
 * Revision 1.1  1993/07/05  21:53:06  sra
 * Initial revision
 *
 */

/*
 * Definitions for snark's tasking package.
 *
 * This is based in large part on Romkey's simtask package, with
 * some changes suggested by Bridgham and different modularization
 * to fit into snark properly.
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_SNARK_TASKS_H_
#define	_SNARK_TASKS_H_

/*
 * The task structure.
 */
struct task {
  char *name;
  void (*func) __((struct task *, void *));
  void *cookie;
  struct task *next;
};

/*
 * Tasking functions common to all implementations.
 * These handle all the locking crud via glue_intlock().
 */
extern void task_ini (struct task *);
extern void task_enq (struct task *);
extern struct task *task_deq (void);
extern void task_delq (struct task *);
extern void task_add (struct task *, char *,
		      void (*)(struct task *, void *), void *);
extern int task_idle (void);

/*
 * The task scheduler itself, provided by each port of snark.
 * It should get tasks to run by calling task_deq().
 * If it ever returns, main() will exit with the same value.
 */
extern int tasks_run (void);

/*
 * Initialize static variables for the tasking package.
 */
extern void tasks_init (void);

#endif /* _SNARK_TASKS_H_ */

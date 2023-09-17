/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/main.h,v 1.1.1.1 2001/11/05 17:47:32 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: main.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 1.6  2001/01/19 22:23:35  paul
 * Update copyright.
 *
 * Revision 1.5  2000/03/17 00:12:12  meister
 * Update copyright message
 *
 * Revision 1.4  1998/02/25 04:57:15  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/03/20 06:53:21  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.2  1997/02/24 00:56:59  sra
 * Support for run-time configuration of BUG() macros.
 *
 * Revision 1.1  1997/02/19 08:13:05  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef SNARK_MAIN_H
#define SNARK_MAIN_H

/*
 * snark_main() is Snark's equivilent to the C main() function:
 * once the snark universe has been initialized, snark calls
 * a function by this name, which must exist, and assumes that
 * this function is the starting point of the real application.
 */

extern void snark_main(struct task *initial_task, void *cookie);

/*
 * snark_exit() is like exit() but it first does a clean shutdown
 * of all the Snark stuff, which might matter if you're running under Purify
 * and care about memory leaks, or if you're running under DOS and ever want
 * to use your machine again without rebooting it first.
 */

extern void snark_exit(int value);

/*
 * Default argblock format for snark_main().  You can override this
 * by defining one of the user-exits SNARK_PRE_INIT_HOOK() or
 * SNARK_POST_INIT_HOOK().
 */

struct snark_main_args {
  int argc;
  char **argv;
};

/*
 * Internal variables used by snark_exit() and tasks_run().
 */
extern int snark_exit_flag, snark_exit_value;

#endif /* SNARK_MAIN_H */

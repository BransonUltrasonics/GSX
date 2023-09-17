/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/telnet.h,v 1.1.1.1 2001/11/05 17:47:16 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
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
 * $Log: telnet.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:16  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:16  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:54  meister
 * Update copyright message
 *
 * Revision 1.2  1998/07/29 20:54:50  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/



/*
 * Use '\n' for local newline convention in both directions.
 */

#define TN_LOCAL_NEWLINE '\n'

/*
 * All the windowing stuff for server telnet connections is controlled
 * by the sty code, so shunt the decisions off to the sty_tn interface module.
 *
 * For client telnet connections we're not buffering output, so we just
 * return a constant window size.
 */

#define	TN_INITIAL_ACTIVE_WINDOW_SIZE	(4096)
#define	TN_INITIAL_PASSIVE_WINDOW_SIZE	(stytn_space_initial)

#define	GLUE_TN_WINDOW_SIZE(_tn_) \
  (tn_get_flags((_tn_), TN_FLG_ACT_AS_SERVER) \
   ? stytn_space_left((_tn_)) \
   : TN_INITIAL_ACTIVE_WINDOW_SIZE)

extern unsigned stytn_space_initial, stytn_space_left();

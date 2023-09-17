/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/telnet.h,v 1.1.1.1 2001/11/05 17:47:13 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
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
 * $Log: telnet.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:13  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:04  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:10:28  meister
 * Update copyright message
 *
 * Revision 1.1  1998/02/25 04:57:00  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:21  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.3  1997/03/20 06:57:46  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/23 00:12:55  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* Things which MAY be defined in here,
 * the defaults are in attache/h/tnconfig.h.
 *
 * TN_LOCAL_NEWLINE
 * TN_RECV_LOCAL_NEWLINE
 * TN_SEND_LOCAL_NEWLINE
 * TN_INITIAL_ACTIVE_WINDOW_SIZE
 * TN_INITIAL_PASSIVE_WINDOW_SIZE
 * GLUE_TN_WINDOW_SIZE
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

extern unsigned stytn_space_initial;
extern unsigned stytn_space_left (struct tn_connection *);

/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/sty.h,v 1.1.1.1 2001/11/05 17:47:37 tneale Exp $ */

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
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: sty.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:37  tneale
 * Tornado shuffle
 *
 * Revision 2.8  2001/01/19 22:23:37  paul
 * Update copyright.
 *
 * Revision 2.7  2000/03/17 00:12:14  meister
 * Update copyright message
 *
 * Revision 2.6  1998/11/23 20:47:29  wes
 * If building with GCC, use a gcc extension to declare sty_printf() as
 * being printf-like so sty_printf args can be sanity-checked at build
 * time.  Most visible with stuff relating to the BUG macro..
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
 * Software terminal definitions for Snark.
 * Based on ideas in Bridgham's doskbd code.
 * Keymaps not implemented yet, maybe some day.
 */

/* [clearcase]
modification history
-------------------
01b,10may05,job  increase maximum input line size
01a,20apr05,job  update copyright notices
*/


#ifndef	_SNARK_STY_H_
#define	_SNARK_STY_H_

/*
 * GCC has a mode which lets it catch a number of common errors
 * made involving printf-like format strings.
 * It also has a language extension which lets you declare a function as
 * taking a printf-like format string.
 * If we're using a suitably new version of GCC (2.5 or newer), use
 * this extension.
 */

#if !defined(__GNUC__) || __GNUC__ < 2 || \
	(__GNUC__ == 2 && __GNUC_MINOR__ < 5)
#define	STY_ATTRIBUTE(x)	/* delete __attribute__ if non-gcc or gcc1 */
#else
#define STY_ATTRIBUTE(x) __attribute__(x)
#endif

#define	STY_RING_SIZE		1024
#define	STY_LINE_SIZE		2048
#define	STY_HISTORY_SIZE	20

struct sty;
struct sty_driver;

struct sty {
  unsigned flags;			/* state flags */
  unsigned char ring_buf[STY_RING_SIZE]; /* input character ring buffer */
  unsigned ring_start, ring_end;	/* portion of ring buffer in use */
  char line_buf[STY_LINE_SIZE];		/* input line buffer */
  unsigned line_len;			/* length of current line */
  char *history[STY_HISTORY_SIZE];	/* command line history */
  fastint_t history_ptr;		/* current command history line */
  struct sty_driver *driver;		/* device driver */
  void *driver_cookie;			/* cookie for driver code */
  void *app_cookie;			/* cookie for application */
  void (*app_receiver)(struct sty *, char *, void *);
  char *prompt;				/* prompt string, null ok if none */
};

#define	STY_FLAG_ECHOING	0x1
#define	STY_FLAG_BLOCKED	0x2
#define	STY_FLAG_ABORTED	0x4
#define	STY_FLAG_INTERRUPTED	0x8
#define	STY_FLAG_RAWMODE	0x10

struct sty_driver {
  void (*write)(struct sty *, unsigned char *, size_t, void *);
  void (*close)(struct sty *, void *);
};



/*
 * The "console", which might be a keyboard/display pair, a log file,
 * a null sty driver, a telnet session, a serial line, or something weirder.
 *
 * In general, it is NOT a good idea to just assume that the cty is the
 * right sty to use for interactive commands.  The cty is primarily for
 * use by code which has no sty context (eg, the BUG() macros), and for
 * the initial startup code.
 *
 * By default, the cty is a keyboard/display pair.
 */
extern struct sty *cty;

#ifndef CTY_SETUP
#define	CTY_SETUP() stykb_open()
#endif



/*
 * Declare listener to catch newly opened stys.
 * Returns old listener.
 */
typedef void (*sty_listener_t)(struct sty *);
extern sty_listener_t sty_listen(sty_listener_t);

/*
 * Open an sty.  Caller provides memory.
 * Installs driver, flags, and cookie, then calls sty listener, if any.
 */
extern void sty_open(struct sty *, struct sty_driver *, unsigned, void *);

/*
 * Close an sty.  Calls driver close routine, clears driver.
 */
extern void sty_close(struct sty *);

/*
 * Initialize sty system.
 */
extern void sty_init(void);

/*
 * Request a "line" of text from the sty.  At the moment a "line"
 * really is a line, because EOL is the only break sequence.
 * This could change if/when keymaps are implemented.
 */
extern void sty_read
  (struct sty *, void (*)(struct sty *, char *, void *), char *, void *);

/*
 * Output routines.
 */
extern void sty_write (struct sty *, char *, size_t);
extern void sty_putc (struct sty *, int);
extern void sty_puts (struct sty *, char *);
extern int sty_printf (struct sty *, char *, ...)
     STY_ATTRIBUTE((__format__(__printf__, 2, 3)));


/*
 * Hand some text from driver to low-level sty code.
 */
extern void sty_receive (struct sty *, unsigned char *, unsigned);

/*
 * Ask how much buffer space this sty has left (for TELNET, mostly).
 */
extern unsigned sty_window (struct sty *);

/*
 * Clear sty input buffer.
 */
extern void sty_discard (struct sty *);

/*
 * Install a new cty.
 */
extern void cty_set(struct sty *sty);

/*
 * This should really go somewhere else, no good place for it though.
 */
extern void stykb_open (void);

#endif /* _SNARK_STY_H_ */

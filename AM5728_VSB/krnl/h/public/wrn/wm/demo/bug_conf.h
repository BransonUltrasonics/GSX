/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/bug_conf.h,v 1.3 2003/01/15 13:58:29 josh Exp $ */

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

/* Run-time configuration of the BUG() macro and friends. */

/*
 * $Log: bug_conf.h,v $
 * Revision 1.3  2003/01/15 13:58:29  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:27:43  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 1.12  2001/01/19 22:23:34  paul
 * Update copyright.
 *
 * Revision 1.11  2000/10/16 19:21:48  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.10  2000/03/17 00:12:11  meister
 * Update copyright message
 *
 * Revision 1.9  2000/03/13 21:21:59  paul
 * Removed some code that we are no longer working on.
 *
 * Revision 1.8  1999/02/18 04:41:30  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.7.12.1  1998/10/01 21:45:19  wes
 * sockets port: add new configurable bug action: yield the processor
 *
 * Revision 1.7  1998/02/25 15:21:44  sra
 * Finish moving types.h, bug.h, and bugdef.h to common/h/.
 *
 * Revision 1.6  1998/02/25 04:57:14  sra
 * Update copyrights.
 *
 * Revision 1.5  1997/05/15 07:54:05  sra
 * Add IDENT option.
 *
 * Revision 1.4  1997/03/20 06:53:19  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.3  1997/03/19 04:11:35  sra
 * BUG() now common to all products, so move the associated .h files.
 *
 * Revision 1.2  1997/02/24 00:56:59  sra
 * Support for run-time configuration of BUG() macros.
 *
 * Revision 1.1  1997/02/23 21:58:43  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef SNARK_BUG_CONF_H
#define SNARK_BUG_CONF_H

/*
 * Notes on the following weirdness:
 *
 * If we're going to define BUG(), we have to
 * do it before bug.h is included.
 *
 * "sty_printf z ;" is not a typo.
 */

#if INSTALL_SNARK_BUG_CONF

#ifndef BUG_OUT
#define BUG_OUT cty
#endif

#define	BUG(_code_, _continuable_, _cookie_, _format_) \
  do {\
    unsigned _flags_ = bug_flags[(int)(_code_)];\
    int _fatal_ = ((_continuable_) == BUG_FATAL);\
    if ((_flags_ & BUG_FLAG_PRINT) != 0) {\
      bug_timestamp(BUG_OUT);\
      if ((_flags_ & BUG_FLAG_IDENT) != 0)\
        sty_printf(BUG_OUT, "%s:%d: ", __FILE__, __LINE__);\
      sty_printf _format_ ;\
      sty_putc(BUG_OUT, '\n');\
    }\
    if ((_flags_ & BUG_FLAG_FATAL) != 0)\
      _fatal_ = !_fatal_;\
    if ((_flags_ & BUG_FLAG_BREAK) != 0)\
      _fatal_ = bug_breakpoint((_code_), (_cookie_), _fatal_);\
    if ((_flags_ & BUG_FLAG_YIELD) != 0)\
       bug_yield((_code_), __FILE__, __LINE__); \
    if (_fatal_) {\
      sty_puts(BUG_OUT, "Fatal error, aborting.\n");\
      abort();\
    }\
  } while (0)

#endif /* INSTALL_SNARK_BUG_CONF */

#ifndef EPILOGUE_BUG_H
#include <wrn/wm/common/bug.h>
#endif

#ifndef _SNARK_STY_H_
#include <wrn/wm/demo/sty.h>
#endif

#define	BUG_FLAG_PRINT	0x01
#define	BUG_FLAG_BREAK	0x02
#define	BUG_FLAG_SYSLOG	0x04
#define	BUG_FLAG_FATAL	0x08
#define BUG_FLAG_IDENT	0x10
#define BUG_FLAG_YIELD	0x20

extern unsigned bug_flags[];
extern int bug_breakpoint(enum bug_codes, void *, int);
extern void bug_config(void);
extern void bug_timestamp(struct sty *);
extern void bug_yield(enum bug_codes, char *, int);

#endif /* SNARK_BUG_CONF_H */

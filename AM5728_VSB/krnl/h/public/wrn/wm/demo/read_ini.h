/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/read_ini.h,v 1.3 2003/01/15 13:58:30 josh Exp $ */

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
 * $Log: read_ini.h,v $
 * Revision 1.3  2003/01/15 13:58:30  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:27:44  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:36  tneale
 * Tornado shuffle
 *
 * Revision 2.8  2001/01/19 22:23:36  paul
 * Update copyright.
 *
 * Revision 2.7  2000/03/17 00:12:13  meister
 * Update copyright message
 *
 * Revision 2.6  1998/06/28 20:09:37  sra
 * Add support for SNARK_READ_INI_FROM_MEMORY.
 *
 * Revision 2.5  1998/02/25 04:57:16  sra
 * Update copyrights.
 *
 * Revision 2.4  1997/03/20 06:53:22  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 2.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 2.2  1996/03/22 10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.1  1995/07/31  13:54:55  sra
 * OSPF now working well enough to let somebody else test it.
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

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef	_READ_INI_H_
#define	_READ_INI_H_

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif

struct ini_handle {
#if INSTALL_SNARK_READ_INI_FROM_MEMORY
  unsigned long current_pos;
#else
  FILE *file;
#endif
  char *section_name;
  unsigned long section_start;
  char *iter_variable;
  char *buffer;
  size_t buflen;
};

struct ini_handle_saved {
  char *section_name;
  unsigned long section_start;
  char *iter_variable;
  unsigned long current_pos;
};

#if INSTALL_SNARK_READ_INI_FROM_MEMORY

/*
 * In-memory configuration table is an array of tag-value pairs.
 * A null value indicates that the tag is a section label.
 * A null tag indicates the end of the array.
 */

struct ini_table {
  char *tag;
  char *value;
};

extern struct ini_table etc_ini_table[];

#endif

extern struct ini_handle *ini_open (size_t);
extern void ini_close (struct ini_handle *);
extern char *ini_lookup (struct ini_handle *, char *, char *);
extern char *ini_iter_start (struct ini_handle *, char *, char *);
extern char *ini_iter_next (struct ini_handle *);
extern void ini_save (struct ini_handle *, struct ini_handle_saved *);
extern void ini_restore (struct ini_handle *, struct ini_handle_saved *);

#endif

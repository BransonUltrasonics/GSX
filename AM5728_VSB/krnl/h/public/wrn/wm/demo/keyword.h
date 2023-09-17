/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/keyword.h,v 1.1.1.1 2001/11/05 17:47:32 tneale Exp $ */

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
 * $Log: keyword.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:23:35  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:12:12  meister
 * Update copyright message
 *
 * Revision 1.5  1998/02/25 04:57:15  sra
 * Update copyrights.
 *
 * Revision 1.4  1997/03/20 06:53:20  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.3  1997/03/09 18:39:25  sra
 * Fix some compiler warnings, change from compile-time to run-time init
 * macros for keyword tables.
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


#ifndef	SNARK_KEYWORD_H
#define	SNARK_KEYWORD_H

enum keyword_error {
  KEYWORD_ERROR_OK,
  KEYWORD_ERROR_NOT_FOUND,
  KEYWORD_ERROR_AMBIGUOUS
};
  
struct keyword_table {
  void *table;
  size_t table_length;
  size_t entry_size;
  enum keyword_error error;
  void *result;
  unsigned flags;
};

#define	KEYWORD_ERROR(_table_) \
  ((_table_)->error)

#define KEYWORD_INIT(_table_, _array_) \
  keyword_init((_table_), (_array_), \
	       sizeof(_array_)/sizeof(*(_array_)), \
	       sizeof(*(_array_)))

extern void keyword_init(struct keyword_table *, void *, size_t, size_t);
extern void *keyword_find(struct keyword_table *, const char *);

#endif /* SNARK_KEYWORD_H */

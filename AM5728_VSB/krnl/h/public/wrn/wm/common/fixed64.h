/* $Header: /usr/cvsroot/target/h/wrn/wm/common/fixed64.h,v 1.3 2003/01/16 18:20:11 josh Exp $ */

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
 * Fixed-point 64-bit math routines, intended primarily for time conversion
 * between Attache milliseconds and pSOSystem "ticks".  These routines
 * are in the common library because they may be useful for other
 * applications, but these functions are not a full-fleged 64-bit math
 * library.
 */

/*
 * $Log: fixed64.h,v $
 * Revision 1.3  2003/01/16 18:20:11  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:09  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:21:27  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:16:36  meister
 * Update copyright message
 *
 * Revision 1.2  2000/03/09 17:15:39  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.1  1999/03/01 20:08:56  sra
 * Add fixed-point 64-bit math routines; these are primarily intended for
 * converting between Attache and pSOSystem clock values, but the
 * functions themselves aren't specific to that.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef COMMON_FIXED64_H
#define COMMON_FIXED64_H

#ifdef __cplusplus
extern"C" {
#endif

#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif

extern bits32_t fixed64_mul(bits32_t, bits32_t, bits32_t, bits32_t);
extern void fixed64_div(bits32_t, bits32_t, bits32_t *, bits32_t *);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_FIXED64_H */

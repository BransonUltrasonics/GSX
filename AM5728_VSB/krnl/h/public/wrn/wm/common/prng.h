/* $Header: /usr/cvsroot/target/h/wrn/wm/common/prng.h,v 1.3 2003/01/16 18:20:13 josh Exp $ */


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
 * $Log: prng.h,v $
 * Revision 1.3  2003/01/16 18:20:13  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:10  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.9  2001/01/19 22:21:28  paul
 * Update copyright.
 *
 * Revision 1.8  2000/03/17 00:16:39  meister
 * Update copyright message
 *
 * Revision 1.7  2000/03/09 17:15:41  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.6  1998/02/25 04:43:26  sra
 * Update copyrights.
 *
 * Revision 1.5  1998/02/05 22:37:35  josh
 * Changing references to types.h so we include common/h/types.h instead
 *
 * Revision 1.4  1997/10/30 16:44:29  sra
 * Fix copyright date.
 *
 * Revision 1.3  1997/08/14 16:04:38  lowell
 * boilerplate
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

#ifndef COMMON_PRNG_H
#define COMMON_PRNG_H

#ifdef __cplusplus
extern"C" {
#endif

#ifndef	EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif /* EPILOGUE_TYPES_H */

void prng_get_seed(bits8_t **buffer, int * bufsize);
void prng_random(bits8_t *buffer, int length);
void prng_inject_random8(bits8_t newdata );
bits8_t prng_random8(void );
void prng_init(void );

#ifdef __cplusplus
}
#endif

#endif /* COMMON_PRNG_H */

/* $Header: /usr/cvsroot/target/h/wrn/wm/common/md4.h,v 1.3 2003/01/16 18:20:12 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: md4.h,v $
 * Revision 1.3  2003/01/16 18:20:12  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:10  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:21:28  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:16:38  meister
 * Update copyright message
 *
 * Revision 1.2  2000/03/09 17:15:40  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.1  1999/08/11 21:16:57  bala
 * MD4 functions
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD4 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD4 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.

*/

#ifndef __MD4_INCLUDE__
#define __MD4_INCLUDE__

#ifdef __cplusplus
extern"C" {
#endif

/* get the Epilogue types */
#ifndef	EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif	/* EPILOGUE_TYPES_H */

/* MD4 context. */
typedef struct {
  bits32_t state[4];                                   /* state (ABCD) */
  bits32_t count[2];        /* number of bits, modulo 2^64 (lsb first) */
  bits8_t  buffer[64];                         /* input buffer */
} MD4_CTX;

void MD4Init __((MD4_CTX *));
void MD4Update __((MD4_CTX *, bits8_t *, bits32_t));
void MD4Final __((bits8_t *, MD4_CTX *));

#ifdef __cplusplus
}
#endif

#endif /* __MD4_INCLUDE__ */

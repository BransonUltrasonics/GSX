/* $Header: /usr/cvsroot/target/h/wrn/wm/common/md5.h,v 1.3 2003/01/16 18:20:13 josh Exp $ */

/*
 * Copyright (C) 1999-2005,2007 Wind River Systems, Inc.
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
 * $Log: md5.h,v $
 * Revision 1.3  2003/01/16 18:20:13  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:10  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.8  2001/01/19 22:21:28  paul
 * Update copyright.
 *
 * Revision 1.7  2000/03/17 00:16:38  meister
 * Update copyright message
 *
 * Revision 1.6  2000/03/09 17:15:40  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.5  1998/02/25 04:43:26  sra
 * Update copyrights.
 *
 * Revision 1.4  1998/02/05 22:37:35  josh
 * Changing references to types.h so we include common/h/types.h instead
 *
 * Revision 1.3  1997/10/30 16:44:29  sra
 * Fix copyright date.
 *
 * Revision 1.2  1997/08/14 16:04:38  lowell
 * boilerplate
 *
 */

/* [clearcase]
modification history
-------------------
01b,31aug07,jbl  fix symbol conflicts
01a,20apr05,job  update copyright notices
*/

/*
 ***********************************************************************
 ** md5.h -- header file for implementation of MD5                    **
 ** RSA Data Security, Inc. MD5 Message-Digest Algorithm              **
 ** Created: 2/17/90 RLR                                              **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version               **
 ** Revised (for MD5): RLR 4/27/91                                    **
 **   -- G modified to have y&~z instead of y&z                       **
 **   -- FF, GG, HH modified to add in last register done             **
 **   -- Access pattern: round 2 works mod 5, round 3 works mod 3     **
 **   -- distinct additive constant for each step                     **
 **   -- round 4 added, working mod 7                                 **
 ***********************************************************************
 */

/*
 ***********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved.  **
 **                                                                   **
 ** License to copy and use this software is granted provided that    **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message-     **
 ** Digest Algorithm" in all material mentioning or referencing this  **
 ** software or this function.                                        **
 **                                                                   **
 ** License is also granted to make and use derivative works          **
 ** provided that such works are identified as "derived from the RSA  **
 ** Data Security, Inc. MD5 Message-Digest Algorithm" in all          **
 ** material mentioning or referencing the derived work.              **
 **                                                                   **
 ** RSA Data Security, Inc. makes no representations concerning       **
 ** either the merchantability of this software or the suitability    **
 ** of this software for any particular purpose.  It is provided "as  **
 ** is" without express or implied warranty of any kind.              **
 **                                                                   **
 ** These notices must be retained in any copies of any part of this  **
 ** documentation and/or software.                                    **
 ***********************************************************************
 */

#ifndef __MD5_INCLUDE__

#ifdef __cplusplus
extern"C" {
#endif

/* get the Epilogue types */
#ifndef	EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif	/* EPILOGUE_TYPES_H */

/* Data structure for MD5 (Message-Digest) computation */
typedef struct {
  bits32_t i[2];                   /* number of _bits_ handled mod 2^64 */
  bits32_t buf[4];                                    /* scratch buffer */
  bits8_t in[64];                              /* input buffer */
  bits8_t digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void WrsnmpCommonMD5Init __((MD5_CTX *));
void WrsnmpCommonMD5Update __((MD5_CTX *, bits8_t *, bits32_t));
void WrsnmpCommonMD5Final __((MD5_CTX *));

#define MD5Init WrsnmpCommonMD5Init
#define MD5Update WrsnmpCommonMD5Update
#define MD5Final WrsnmpCommonMD5Final

#ifdef __cplusplus
}
#endif

#define __MD5_INCLUDE__
#endif /* __MD5_INCLUDE__ */

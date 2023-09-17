/* $Header: /usr/cvsroot/target/h/wrn/wm/common/sha.h,v 1.3 2003/01/16 18:20:13 josh Exp $ */

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
 *  Copyright 1998 Integrated Systems Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: sha.h,v $
 * Revision 1.3  2003/01/16 18:20:13  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:11  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:21:29  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:16:40  meister
 * Update copyright message
 *
 * Revision 1.3  2000/03/09 17:15:41  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.2  1998/06/16 05:12:27  sar
 * Clean up some type questions
 *
 * Revision 1.1  1998/05/22 17:59:58  sar
 * Adding include files for hamc and sha
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

/* NIST Secure Hash Algorithm */
/* heavily modified by Uwe Hollerbach <uh@alumni.caltech edu> */
/* from Peter C. Gutmann's implementation as found in */
/* Applied Cryptography by Bruce Schneier */

/* This code is in the public domain */

#ifndef EPILOGUE_SHA_H
#define EPILOGUE_SHA_H

#ifdef __cplusplus
extern"C" {
#endif

/* get the installation options */
#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif /* EPILOGUE_INSTALL_H */

/* get the Epilogue types */
#ifndef	EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif	/* EPILOGUE_TYPES_H */

#define SHA_BLOCKSIZE		64
#define SHA_DIGESTSIZE		20

typedef struct {
    bits32_t count_lo, count_hi;  /* 64-bit bit count */
    bits32_t buf[5];		  /* scratch buffer */
    bits8_t  data[SHA_BLOCKSIZE]; /* SHA data (input) buffer */
    bits8_t  digest[20];	  /* message digest after sha_final call */
    size_t   local;		  /* unprocessed amount in data */
} SHA_CTX;

void sha_init   __((SHA_CTX *));
void sha_update __((SHA_CTX *, bits8_t *, bits32_t));
void sha_final  __((SHA_CTX *));

#ifdef __cplusplus
}
#endif

#endif /* EPILOGUE_SHA_H */


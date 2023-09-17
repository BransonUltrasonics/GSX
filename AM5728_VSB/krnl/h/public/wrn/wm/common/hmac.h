/* $Header: /usr/cvsroot/target/h/wrn/wm/common/hmac.h,v 1.3 2003/01/16 18:20:12 josh Exp $ */

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
 * $Log: hmac.h,v $
 * Revision 1.3  2003/01/16 18:20:12  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:10  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:21:28  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:16:37  meister
 * Update copyright message
 *
 * Revision 1.3  2000/03/09 17:15:39  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.2  1998/06/18 04:11:24  sar
 * clean up some type info, make the lengths be bits32_t
 *
 * Revision 1.1  1998/05/22 17:59:58  sar
 * Adding include files for hmac and sha
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef HMAC_H
#define HMAC_H 1

#ifdef __cplusplus
extern"C" {
#endif

/* get the installation options */
#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif /* EPILOGUE_INSTALL_H */

/* get the Epilogue types */
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif /* EPILOGUE_TYPES_H */

/* Data structure for HMAC computation */

#define HMAC_STATESIZ_MAX 128       
#define HMAC_HASHSIZ_MAX  24        /* keep them 64-bit aligned */

typedef void (*hashfcn_init_t)(void *state);
typedef void (*hashfcn_update_t)(void *state, bits8_t *data, bits32_t datalen);
typedef void (*hashfcn_final_t)(void *state, bits8_t *output);

typedef struct {
  bits8_t          hmac_state [HMAC_STATESIZ_MAX];
  bits8_t          hmac_tmp   [HMAC_HASHSIZ_MAX];
  hashfcn_init_t   hmac_hashinit;
  hashfcn_update_t hmac_hashupdate;
  hashfcn_final_t  hmac_hashfinal;
  bits32_t         hmac_hashlen;
} HMAC_CTX;

/* Initialize an HMAC context. Needs the three hash function interface calls, 
   the size of the output hash, and the size of the state block needed by the 
   underlying hash function. */
 
void InitHmacCTX __((HMAC_CTX         *ctx, 
		     hashfcn_init_t    init,
		     hashfcn_update_t  update, 
		     hashfcn_final_t   final,
		     bits32_t          hashsiz));

void hmac_init   __((HMAC_CTX *ctx, bits8_t *key,  bits32_t keylen));
void hmac_update __((HMAC_CTX *ctx, bits8_t *data, bits32_t datalen));
void hmac_final  __((HMAC_CTX *ctx, bits8_t *key,  bits32_t keylen,
		     bits8_t  *output));

#ifdef __cplusplus
}
#endif

#endif /* HMAC_H */


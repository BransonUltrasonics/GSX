/* $Header: /usr/cvsroot/target/h/wrn/wm/common/weakprng.h,v 1.3 2003/01/16 18:20:14 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/****************************************************************************
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* 
 * $Log: weakprng.h,v $
 * Revision 1.3  2003/01/16 18:20:14  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:11  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:21:29  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:16:42  meister
 * Update copyright message
 *
 * Revision 1.5  2000/03/09 17:15:43  tneale
 * Added #idef C++ to declare extern C if needed
 *
 * Revision 1.4  1998/02/25 22:08:28  mrf
 * Correct copyright dates.
 *
 * Revision 1.3  1998/02/25 04:43:27  sra
 * Update copyrights.
 *
 * Revision 1.2  1998/02/21 23:38:33  sra
 * Minor changes to WeakPRNG module.
 *
 * Revision 1.1  1998/02/21 22:05:35  mrf
 * Added weak random number generator.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/

#ifdef __cplusplus
extern"C" {
#endif

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif

#ifndef COMMON_CONFIG_H
#include <wrn/wm/common/config.h>
#endif

#ifndef	COMMON_GLUE_H
#include <wrn/wm/common/glue.h>
#endif

/* 
 * Include file for cryptographically weak random number generator
 * found in common/lib/weakprng.c.
 */
/* Function prototypes */
void ep_seed_weak_prng(bits8_t *, size_t);
bits32_t ep_weak_prng(bits32_t);


#ifdef __cplusplus
}
#endif



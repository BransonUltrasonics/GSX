/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/snark.h,v 1.1.1.1 2001/11/05 17:47:13 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snark.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:13  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:23:04  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:10:27  meister
 * Update copyright message
 *
 * Revision 1.3  1999/05/21 16:45:28  sra
 * vsnprintf() => VSNPRINTF().
 *
 * Revision 1.2  1998/02/25 04:56:59  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/11/10 23:58:17  sar
 * ifdef the include for memory.h to make some compilers happy
 *
 * Revision 1.0  1997/08/08 22:11:21  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/07/11 12:59:07  mrf
 * make definitions of GLUE_FREE in attache.h and snark.h match exactly
 *
 * Revision 0.4  1997/03/20 06:57:49  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/03/09 18:37:16  sra
 * Add STRICMP().
 *
 * Revision 0.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/23 00:12:55  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* zortech, watcom and intel 960 don't have a memory.h so away it goes */
#if (!defined(INSTALL_on_ztc) && !defined(INSTALL_on_watcom) && !defined(INSTALL_on_i960))
#include <memory.h>
#endif /* INSTALL_on_ztc */

#include <string.h>
#include <stdlib.h>

#ifndef GLUE_ALLOC
#define	GLUE_ALLOC(len)			((void *) malloc(len))
#endif
#ifndef GLUE_FREE
#define GLUE_FREE(mem)			free((char *)(mem))
#endif

#ifndef MEMCPY
#define MEMCPY(x, y, z)			memcpy(x, y, z)
#endif
#ifndef MEMCMP
#define MEMCMP(x, y, z)			memcmp(x, y, z)
#endif
#ifndef MEMSET
#define MEMSET(x, y, z)			memset(x, y, z)
#endif
#ifndef STRCMP
#define STRCMP(x, y)			strcmp(x, y)
#endif
#ifndef STRCPY
#define STRCPY(x, y)			strcpy(x, y)
#endif
#ifndef STRLEN
#define STRLEN(x)			strlen(x)
#endif
#ifndef STRICMP
#define	STRICMP(x, y)			stricmp(x, y)
#endif
#ifndef STRNICMP
#define	STRNICMP(x, y, z)		strnicmp(x, y, z)
#endif

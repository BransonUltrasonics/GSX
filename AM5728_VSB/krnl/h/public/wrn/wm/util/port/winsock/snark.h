/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/winsock/snark.h,v 1.1 2002/03/20 16:43:52 tneale Exp $ */

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
 * Revision 1.1  2002/03/20 16:43:52  tneale
 * To start with this is the same as snark.h in port/msdos
 *
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

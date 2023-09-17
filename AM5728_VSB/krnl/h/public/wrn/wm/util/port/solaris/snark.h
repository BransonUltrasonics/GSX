/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/snark.h,v 1.1.1.1 2001/11/05 17:47:15 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snark.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:15  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:23:16  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:10:54  meister
 * Update copyright message
 *
 * Revision 1.3  1999/05/21 16:45:25  sra
 * vsnprintf() => VSNPRINTF().
 *
 * Revision 1.2  1998/07/29 20:54:50  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 */

/* [clearcase]
modification history
-------------------
*/



#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

#define	SNARK_ATTACHE_MAXLNH		20

#ifndef GLUE_ALLOC
#define	GLUE_ALLOC(x)			((void *) malloc(x))
#endif

#ifndef GLUE_FREE
#define GLUE_FREE(x)			free((char *) (x))
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

#ifndef STRNICMP
#define	STRNICMP(x, y, z)		strncasecmp(x, y, z)
#endif

#ifndef STRICMP
#define	STRICMP(x, y)			strcasecmp(x, y)
#endif

#define stricmp				STRICMP
#define strnicmp			STRNICMP



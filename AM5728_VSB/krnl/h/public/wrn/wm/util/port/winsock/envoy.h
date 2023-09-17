/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/winsock/envoy.h,v 1.1.1.1 2001/11/05 17:47:17 tneale Exp $ */

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
 * Configuration for Envoy on MSDOS with Attache.
 */

/*
 * modification history
 * --------------------
 * 01a,25jun03,ps  remove bad format clearcase history comment
 */

/*
 * $Log: envoy.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:23:19  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:11:05  meister
 * Update copyright message
 *
 * Revision 1.5  1998/02/25 04:57:12  sra
 * Update copyrights.
 *
 * Revision 1.4  1997/10/29 01:31:30  sar
 * Updated the master agent installation option.
 *
 * Revision 1.3  1997/03/20 06:58:11  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 1.1  1996/10/30 17:32:19  mrf
 * Initial revision
 *
 * Revision 1.1  1996/07/02  19:55:38  mrf
 * Initial revision
 *
 * Revision 7.0  1996/03/18  20:25:57  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/11/06  20:05:58  sar
 * removed validate_community's dependency on install v1
 *
 * Revision 6.0  1995/05/31  22:00:34  sra
 * Release 6.0.
 *
 * Revision 5.4  1995/05/22  19:39:49  sar
 * Modified the lock code to add some new macros.
 *
 * Revision 5.3  1995/05/05  22:30:05  sar
 * Added some default macros for the timer code.
 *
 * Revision 5.2  1995/05/01  21:57:42  sar
 * Put the example lock macros in an if def block.
 *
 * Revision 5.1  1994/10/25  20:03:13  sar
 * Added some simple lock code for testing and as an example, also
 * added a definition for snmpresetsysuptime.
 *
 * Revision 5.0  1994/05/16  17:24:02  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:47:40  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 1.1  1993/06/09  20:37:06  sar
 * Initial revision
 */

/* [clearcase]
modification history
-------------------
*/



#if defined(CONFIG_PREAMBLE)

/*****************************************************************
 *
 * System Includes:
 *
 *     The SNMP uses memcpy(), memset(), and strlen().  The appropriate
 * include file should be put here to see that these routines are
 * properly declared.  If no such include file exists on the system
 * then declare memcpy() and memset() here directly.
 *****************************************************************/
/* zortech, watcom and intel 960 don't have a memory.h so away it goes */
#if (!defined(INSTALL_on_ztc) && !defined(INSTALL_on_watcom) && !defined(INSTALL_on_i960))
#include <memory.h>
#endif /* INSTALL_on_ztc */

#include <string.h>

/*****************************************************************
 * Memory allocation routines.  The SNMP needs to allocate memory to
 * decode the packet into and also to store the response data for GET's
 * and GET-NEXT's.
 *****************************************************************/

/* Fix up mach 386's improper declaration of the mem* routines */
#if defined(INSTALL_on_mach386)
#define MEMCPY(dst, src, n) memcpy((char *)(dst), (char *)(src), n)
#define MEMSET(dst, val, n) memset((char *)(dst), val, n)
#define MEMCMP(dst, src, n) memcmp((char *)(dst), (char *)(src), n)
#endif

/*----------------------------------------------------------------
 * Make sure that the routines used by the SNMP_memory_alloc() and
 * SNMP_memory_free() macros are declared.  If system memory routines
 * are used (e.g. malloc() and free()) then include the appropriate
 * system header file here.
 ----------------------------------------------------------------*/

/* The memory allocation macros */
#if defined(INSTALL_on_unix)
extern void *malloc __((unsigned));
extern void free __((void *));
#endif
#if defined(INSTALL_on_msdos)
#include <stdlib.h>
#endif

#define SNMP_memory_alloc(need)	malloc(need)
#define SNMP_memory_free(buf)	free(buf)

#include <wrn/wm/util/port/generic/envoy.h>

#else /* CONFIG_PREAMBLE */

#include <wrn/wm/util/port/generic/envoy.h>

#endif /* CONFIG_PREAMBLE */

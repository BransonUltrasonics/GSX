/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/mach386/envoy.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

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
 * $Log: envoy.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 1.7  2001/01/19 22:23:02  paul
 * Update copyright.
 *
 * Revision 1.6  2000/03/17 00:10:19  meister
 * Update copyright message
 *
 * Revision 1.5  1998/06/21 21:53:05  sar
 * Moved the porting stuff that is common to all of our demos to generic/envoy.h
 * and included that file here.
 *
 * Revision 1.4  1998/06/20 18:29:20  sar
 * Update for use with v3, also made it similar to other envoy.h files
 *
 * Revision 1.3  1998/02/25 04:56:54  sra
 * Update copyrights.
 *
 * Revision 1.2  1997/11/04 00:29:36  sar
 * Changed the agentx sub agent timeout period from 10000 (10 seconds)
 * which was used for testing to 1000 (1 second).
 *
 * Revision 1.1  1997/10/29 01:31:29  sar
 * Updated the master agent installation option.
 *
 * Revision 1.0  1997/08/08 22:11:17  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.4  1997/03/20 06:58:07  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.2  1997/01/15 22:13:32  sar
 * moved the location of nvutils.h to snark/snmptalk
 *
 * Revision 0.1  1996/10/28  20:30:07  sar
 * Initial version
 *
 * Revision 3.2  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.1  1995/09/27  21:21:17  lowell
 * Ifdeffed the example locking code so it won't conflict with the defaults
 * if we don't have locking turned on.
 * (per sar's changes to the msdos example code)
 *
 * Revision 3.0  1995/05/10  22:37:24  sra
 * Release 3.0.
 *
 * Revision 2.9  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 2.8  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 2.7  1994/12/20  21:19:11  lowell
 * worked around problems between Mach's memory.h and
 * the builtin versions provided by GCC...
 *
 * Revision 2.6  1994/11/01  18:02:55  sar
 * Updated *_add_mod to include a flags argument.
 *
 * Revision 2.5  1994/10/18  15:52:03  sar
 * Modfied the calls for the new nvutils routines.
 *
 * Revision 2.4  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 2.3  1994/05/12  17:28:21  sar
 * moved the include for nvutils.h so it is available to all who need it.
 *
 * Revision 2.2  1994/05/10  20:43:24  sar
 * Moved some view stuff around so it is available in other than v2 settings.
 *
 * Revision 2.1  1993/08/06  22:03:40  sra
 * Release 2.0.
 *
 * Revision 1.8  1993/08/02  22:05:36  dab
 * don't use #ifdef for INSTALL_ options
 *
 * Revision 1.7  1993/06/02  23:24:44  dab
 * make MEMCMP identical to attache.h so the compiler doesn't complain.
 *
 * Revision 1.6  1993/05/13  22:27:54  sar
 * added MEMCMP
 *
 * Revision 1.5  1993/05/13  18:47:44  sar
 * Added macros for managing file impelementation of non volatile memory.
 *
 * Revision 1.4  1993/04/22  22:58:03  sar
 * Got rid of pread and pwrite and some other macros, modified TIME
 * to be SNMP_PARTYTIME
 *
 * Revision 1.3  1993/03/01  18:38:08  dab
 * Added MEMSET and MEMCPY macros.
 *
 * Revision 1.2  1993/03/01  17:20:43  dab
 * Added include of <string.h> to get strlen() declaration.
 *
 * Revision 1.1  1993/02/26  22:06:21  dab
 * Initial revision
 *
 * Revision 1.1  1993/02/26  17:26:21  dab
 * Initial revision
 *
 * 
 */

/* [clearcase]
modification history
-------------------
*/



/*****************************************************************

  This is the platform specific part of envoy.h for use with the
  example code.  It contains only the platform specific pieces of
  the port code.  The common porting code is in generic/envoy.h.

  In general customers will have one envoy.h file per platform
  that combines this file and the platform specific information,
  we use two files as there is a lot of shared code between our
  demos.

*****************************************************************/

#ifdef CONFIG_PREAMBLE

/*****************************************************************
 
  System Includes:
 
  Envoy uses memcpy(), memset(), and strlen().  The appropriate
  include file should be put here to see that these routines are
  properly declared.  If no such include file exists on the system
  then declare memcpy() and memset() here directly.  Envoy also
  uses min() and max().  Include the system's definition or use the
  macros supplied below.
*****************************************************************/

void * memset(void *, int, size_t);
#include <string.h>

/*****************************************************************
  Memory allocation routines.  Envoy needs to allocate memory to
  decode the packet into and also to store the response data

  Make sure that the routines used by the SNMP_memory_alloc() and
  SNMP_memory_free() macros are declared.  If system memory routines
  are used (e.g. malloc() and free()) then include the appropriate
  system header file here.

  On MACH 386 the system include files don't pass gcc's -pedentic
  switch.  This is just too painful to watch so I just declare malloc
  and free myself. 
*****************************************************************/

extern void *malloc (unsigned);
extern void  free   (void *);

#define SNMP_memory_alloc(need)	malloc(need)
#define SNMP_memory_free(buf)	free(buf)

/*****************************************************************
  These macros fix up Mach 386's improper declaration of the mem*
  routines.  If you're using Envoy with Attache then these same macros
  need to be defined in attache.h as well.  Make sure they are identical
  otherwise the C-preprocessor will complain about redefining them. 
*****************************************************************/
#define MEMCPY(dst, src, n) memcpy((char *)(dst), (char *)(src), n)
#define MEMSET(dst, val, n) memset((char *)(dst), val, n)
#define MEMCMP(dst, src, n) memcmp((char *)(dst), (char *)(src), n)

#include <port/generic/envoy.h>

#else /* config preamble */

#include <port/generic/envoy.h>

#endif /* CONFIG_PREAMBLE */

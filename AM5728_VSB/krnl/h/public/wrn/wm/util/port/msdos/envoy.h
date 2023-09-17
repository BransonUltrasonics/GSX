/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/envoy.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

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
 * Revision 1.6  2001/01/19 22:23:04  paul
 * Update copyright.
 *
 * Revision 1.5  2000/03/17 00:10:27  meister
 * Update copyright message
 *
 * Revision 1.4  1998/06/21 21:53:06  sar
 * Moved the porting stuff that is common to all of our demos to generic/envoy.h
 * and included that file here.
 *
 * Revision 1.3  1998/02/25 04:56:58  sra
 * Update copyrights.
 *
 * Revision 1.2  1997/11/04 00:29:36  sar
 * Changed the agentx sub agent timeout period from 10000 (10 seconds)
 * which was used for testing to 1000 (1 second).
 *
 * Revision 1.1  1997/10/29 01:29:59  sar
 * Added the agentx options and modified the older sub agent options
 *
 * Revision 1.0  1997/08/08 22:11:20  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.6  1997/03/20 06:57:49  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.5  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.4  1997/01/15 22:14:49  sar
 * moved the location of nvutils.h to snark/snmptalk
 *
 * Revision 0.3  1996/10/30  04:41:43  sra
 * Fix nested comment.
 *
 * Revision 0.2  1996/10/25  21:05:12  sar
 * corrected install_envoy_proxy to install_envoy_snmp_proxy
 *
 * Revision 0.1  1996/10/23  00:12:55  sar
 * *** empty log message ***
 */

/* [clearcase]
modification history
-------------------
*/



/* previous revision history from attache/port/msdos/envoy.h
 * 
 * Revision 3.6  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.5  1996/01/26  22:07:27  sar
 * changed partytime to usectime and ifdeffed it off of version_usec
 *
 * Revision 3.4  1995/11/10  23:58:18  sar
 * stopped using the pre & post set user exits for the nvview stuff
 *
 * Revision 3.3  1995/11/06  20:03:42  sar
 * Removed validate_community's dependency on install v1
 * Remvoed validate_address & validate_version
 *
 * Revision 3.2  1995/10/20  23:19:42  sar
 * removed party, context and acl macros
 * modifed the install options for the view scheme inclusions.
 *
 * Revision 3.1  1995/05/22  19:38:28  sar
 * Modified the lock code to add some new macros.
 *
 * Revision 3.0  1995/05/10  22:37:24  sra
 * Release 3.0.
 *
 * Revision 2.10  1995/05/01  20:54:06  sar
 * Added undo macros for the party mib for use with the async scheme.
 *
 * Revision 2.9  1995/03/24  16:39:14  sar
 * Ifdeffed the example locking code so it won't conflict with the defaults
 * if we don't have locking turned on.
 * Added the envoy timer support routines.
 *
 * Revision 2.8  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 2.7  1994/11/01  18:00:30  sar
 * *_add_mod now takes a flag as well as a data pointer.
 *
 * Revision 2.6  1994/10/18  15:54:01  sar
 * Modified some macros for use with the new nvutils routines.
 *
 * Revision 2.5  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 2.4  1994/05/12  17:25:20  sar
 * moved include for nvutils.h so it is availble when needed.
 *
 * Revision 2.3  1994/05/10  20:22:11  sar
 * moved view defines around so they are availabel in other than v2 settings.
 *
 * Revision 2.2  1994/05/09  21:07:17  sar
 * added define for SNMP_RESETSYSUPTIME to be nothing.
 *
 * Revision 2.1  1993/08/06  22:03:43  sra
 * Release 2.0.
 *
 * Revision 1.8  1993/08/02  22:06:06  dab
 * don't use #ifdef for INSTALL_ optios
 *
 * Revision 1.7  1993/05/20  20:22:21  sar
 * Removed memory.h if the compiler is watcom or intel 960.
 * checked to see if config_preamble is defined rather than what it is.
 *
 * Revision 1.6  1993/05/17  20:11:55  sar
 * Added an ifdef for zortech to remove the include of memory.h and
 * to add the define of NEEDS_STR{DUP ICMP NICMP} as they weren't
 * available.
 *
 * Revision 1.5  1993/05/13  18:51:39  sar
 * Added macros for managing the file implementation of non volatile memory.
 *
 * Revision 1.4  1993/04/22  22:59:03  sar
 * Got rid of pread and pwrite and some other macros, modified TIME
 * to be SNMP_PARTYTIME and TIME_T to be PARTYTIME_T.
 *
 * Revision 1.3  1993/03/05  03:59:57  dab
 * function declarations to keep the compiler happy
 *
 * Revision 1.2  1993/03/01  17:21:07  dab
 * Added include of <string.h> to get strlen() declaration.
 *
 * Revision 1.1  1993/02/27  17:27:42  dab
 * Initial revision
 *
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

/*****************************************************************
 
  System Includes:
 
  Envoy uses memcpy(), memset(), and strlen().  The appropriate
  include file should be put here to see that these routines are
  properly declared.  If no such include file exists on the system
  then declare memcpy() and memset() here directly.  Envoy also
  uses min() and max().  Include the system's definition or use the
  macros supplied below.
*****************************************************************/

/* zortech, watcom and intel 960 don't have a memory.h so away it goes */
#if (!defined(INSTALL_on_ztc) && !defined(INSTALL_on_watcom) && !defined(INSTALL_on_i960))
#include <memory.h>
#endif /* INSTALL_on_ztc */

#include <string.h>

/*****************************************************************
  Memory allocation routines.  Envoy needs to allocate memory to
  decode the packet into and also to store the response data

  Make sure that the routines used by the SNMP_memory_alloc() and
  SNMP_memory_free() macros are declared.  If system memory routines
  are used (e.g. malloc() and free()) then include the appropriate
  system header file here.

*****************************************************************/

PTR_T SNMP_mem_alloc (unsigned int);
void SNMP_mem_free (PTR_T);

#define SNMP_memory_alloc(need)	SNMP_mem_alloc(need)
#define SNMP_memory_free(buf)	SNMP_mem_free(buf)

#include <port/generic/envoy.h>

#else /* CONFIG_PREAMBLE */

#include <port/generic/envoy.h>

#endif /* CONFIG_PREAMBLE */

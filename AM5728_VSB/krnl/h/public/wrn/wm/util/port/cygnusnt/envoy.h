/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/cygnusnt/envoy.h,v 1.1.1.1 2001/11/05 17:47:09 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/07/10 15:55:14  mrf
 * Add configuration for Cygnus NT environment with associated changes.
 *
 * Revision 1.10  2000/03/17 00:08:57  meister
 * Update copyright message
 *
 * Revision 1.9  1998/06/21 21:53:04  sar
 * Moved the porting stuff that is common to all of our demos to generic/envoy.h
 * and included that file here.
 *
 * Revision 1.8  1998/06/17 04:58:43  sar
 * Restore the 1445 view macos
 * Tidy up the rest of the file to bring it in line with the
 * other envoy.h files
 *
 * Revision 1.7  1998/06/07 02:05:34  sar
 * Remove attache specific define for ENVOY_NOW, instead we expect
 * envoy_now() to be defined, if using attache just make it call
 * glue_now().
 *
 * Revision 1.6  1998/06/03 20:20:19  sar
 * Moved nvutils.h to snark/h to allow it to be shared.
 * Changed the style for the string routines.  They are now hidden
 * behind a macro that the user can define.  If the user doesn't
 * have them they can install them in the common library using makmak
 *
 * Revision 1.5  1998/05/28 18:39:28  sar
 * Added options for v3
 *
 * Revision 1.4  1998/02/25 04:56:47  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/11/04 00:29:35  sar
 * Changed the agentx sub agent timeout period from 10000 (10 seconds)
 * which was used for testing to 1000 (1 second).
 *
 * Revision 1.2  1997/10/22 03:01:21  sar
 * Changed ENVOY_MASTER_AGENT to ENVOY_EPI_MASTER for clarity,
 *
 * Started using ENVOY_AGENTX_MASTER & ENVOY_AGENTX_SUB for better
 * granularity
 *
 * Revision 1.1  1997/10/21 03:00:28  sar
 * Added agentx macro defintions
 *
 * Revision 1.0  1997/08/08 22:11:12  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/07/24 21:15:13  alan
 * Decorum now has: DECORUM_MAX_PACKET_SIZE, DECORUM_DEFAULT_RECEIVE_BUFLEN
 *
 * Revision 0.4  1997/03/20 06:57:59  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/03/18 23:30:10  sar
 * Made use of glue_now or envoy_now depend on Attache being
 * installed.
 *
 * Revision 0.2  1997/02/25  10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.1  1996/10/23 00:18:21  sar
 * *** empty log message ***
 *
 */

/* previous revision history when this was netbsd/envoy.h
 *
 * Revision 2.6  1996/10/16  00:10:00  sar
 * Corrected proxy_release_private to use a pointer.
 *
 * Revision 2.5  1996/10/15  20:13:59  sar
 * Added declaration for proxy release
 *
 * Revision 2.4  1996/10/15  17:37:53  sar
 * Added proxy release and timer routine definitions.
 *
 * Revision 2.3  1996/03/22  10:05:39  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 2.2  1996/02/27  00:04:18  sar
 * removed validate_address, made v2 use validate_community and
 * updated partytime to be usectime.
 *
 * Revision 2.1  1995/05/23  23:22:33  sar
 * Added some more lock macros.
 *
 * Revision 2.0  1995/05/10  22:38:15  sra
 * Attache release 3.0.
 *
 * Revision 1.10  1995/05/03  01:00:11  sar
 * Added default envoy timer macros.
 *
 * Revision 1.9  1995/05/01  21:27:13  sar
 * Put ifdef statements around the example lock macros
 *
 * Revision 1.8  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PREAMBLE with #ifdef, not #if.
 *
 * Revision 1.7  1995/04/26  08:03:12  sra
 * Back out to pre-NVutil version until the NVutil changes
 * compile correctly in this configuration.  Sorry, SAR.
 *
 * Revision 1.6  1995/03/24  17:01:35  sar
 * Updated to have definitions for the nvutils code and the timer code.
 *
 * Revision 1.5  1995/01/06  00:52:48  sra
 * Update copyright notice for 2.1 release.
 *
 * Revision 1.4  1994/10/20  19:21:21  sar
 * Added resetsysuptime and a simple lock scheme to allow the lock code
 * to be compiled.
 *
 * Revision 1.3  1994/09/04  23:55:50  sra
 * Get rid of most of the ancient NO_PP cruft.
 *
 * Revision 1.2  1993/08/03  22:03:39  sra
 * Don't use defined() to test install options.
 *
 * Revision 1.1  1993/07/05  21:54:24  sra
 * Initial revision
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

#include <string.h>

/*****************************************************************
  Memory allocation routines.  Envoy needs to allocate memory to
  decode the packet into and also to store the response data

  Make sure that the routines used by the SNMP_memory_alloc() and
  SNMP_memory_free() macros are declared.  If system memory routines
  are used (e.g. malloc() and free()) then include the appropriate
  system header file here.
*****************************************************************/

/* Get malloc and free declarations */
#include <stdlib.h>

#define SNMP_memory_alloc(need)	malloc(need)
#define SNMP_memory_free(buf)	free(buf)

#include <port/generic/envoy.h>

#else /* config preamble */

#include <port/generic/envoy.h>

#endif /* CONFIG_PREAMBLE */

/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/envoy.h,v 1.2 2001/11/07 16:55:52 tneale Exp $ */

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
 * Revision 1.2  2001/11/07 16:55:52  tneale
 * Added path to target/h
 *
 * Revision 1.1.1.1  2001/11/05 17:47:15  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:15  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:53  meister
 * Update copyright message
 *
 * Revision 1.2  1998/07/29 20:54:48  wes
 * First cut of the Solaris port.
 * (DLPI support, termios, minor tweaks to libraries and port header files)
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* previous revision history when this was envoy/port/sunos/envoy.h
 * with rev and copyright updates removed
 *
 * Revision 6.5  1996/02/27  00:03:03  sar
 * Updated partytime to be usectime
 *
 * Revision 6.4  1995/11/14  22:37:59  sar
 * If proxy is installed release_private is proxy_release_private
 *
 * Revision 6.3  1995/11/10  23:59:08  sar
 * stopped using the pre & post set user exits for the nvview stuff
 *
 * Revision 6.2  1995/11/06  20:05:22  sar
 * Removed validate_community's dependency on install v1
 * removed validate_address & validate_vesion
 *
 * Revision 6.1  1995/10/20  23:15:55  sar
 * removed party, context and acl macros
 *
 * Revision 5.6  1995/05/22  19:39:24  sar
 * Modified the lock code to add some new macros.
 *
 * Revision 5.5  1995/05/02  23:39:31  sar
 * Added the envoy timer macros.
 *
 * Revision 5.4  1995/05/01  21:58:15  sar
 * Put the example lock macros in an ifdef block
 *
 * Revision 5.3  1995/05/01  20:59:29  sra
 * Have to test CONFIG_PPREAMBLE with #ifdef, not #if.
 *
 * Revision 5.2  1994/11/01  17:59:04  sar
 * *_add_mod routines now take a flag as well as an data block.
 *
 * Revision 5.1  1994/09/29  20:39:39  sar
 * added macros for a simple locking scheme to test the compiliation of
 * our code
 * Modified the macros for use with the non volatile storage stuff to
 * use the new (as of fall 94) scheme.
 *
 * Revision 4.3  1994/05/12  17:24:05  sar
 * moved the include for nvutils.h to make it available for everybody
 * that needs it.
 *
 * Revision 4.2  1994/05/10  20:21:45  sar
 * moved view defines aroudn so they are available in other than v2 settings.
 *
 * Revision 4.1  1994/05/09  21:06:41  sar
 * Added define for SNMP_RESETSYSUPTIME to be nothing.
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

#include <memory.h>
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

#include <wrn/wm/util/port/generic/envoy.h>

#else /* config preamble */

#include <wrn/wm/util/port/generic/envoy.h>

#endif /* CONFIG_PREAMBLE */

/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/msdos/snmptalk.h,v 1.1.1.1 2001/11/05 17:47:13 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1995-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snmptalk.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:13  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:04  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:10:28  meister
 * Update copyright message
 *
 * Revision 1.1  1998/02/25 04:57:00  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:21  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.5  1997/03/20 06:57:50  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.4  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.3  1996/10/30 22:09:46  sar
 * removed bsd_release
 *
 * Revision 0.2  1996/10/28  17:50:30  mrf
 * add winsock definitions if we're building for winsock.
 *
 * Revision 0.1  1996/10/23  00:12:55  sar
 * *** empty log message ***
 *
 */

/* [clearcase]
modification history
-------------------
*/



/* System include files or function declarations for MS-DOS. */

#include <stdlib.h>
#include <string.h>

#define PTCP_STDFUNC_H
#define ATTACHE

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) remove(fname)

/* defines for missing string items */
/* currently only needed for zortech */
#if defined(INSTALL_on_ztc)
#define NEEDS_STRDUP 1
#define NEEDS_STRICMP 1
#define NEEDS_STRNICMP 1
#endif /* INSTALL_on_ztc */

#if defined(INSTALL_on_winsock)
#define DESIRED_WINSOCK_VERSION 0x0101
#define MINIMUM_WINSOCK_VERSION 0x0001
#define SOCKET_TIMEOUT          20L     /* time to wait to for selects */
#endif /* INSTALL_on_winsock */

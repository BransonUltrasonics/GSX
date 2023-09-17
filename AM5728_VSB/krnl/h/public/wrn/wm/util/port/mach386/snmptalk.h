/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/mach386/snmptalk.h,v 1.1.1.1 2001/11/05 17:47:12 tneale Exp $ */

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
 * $Log: snmptalk.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:12  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:02  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:10:19  meister
 * Update copyright message
 *
 * Revision 1.1  1998/02/25 04:56:55  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:18  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.4  1997/03/20 06:58:08  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 0.2  1996/10/30 22:08:57  sar
 * removed bsd_release
 *
 * Revision 0.1  1996/10/28  20:30:07  sar
 * Initial version
 *
 */

/* [clearcase]
modification history
-------------------
*/


#include <strings.h>

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) unlink(fname)

/* Mach doesn't have these routines so we provide them. */
#define NEEDS_STRDUP
#define NEEDS_STRTOL
#define NEEDS_STRTOUL

#define strnicmp strncasecmp
#define stricmp strcasecmp

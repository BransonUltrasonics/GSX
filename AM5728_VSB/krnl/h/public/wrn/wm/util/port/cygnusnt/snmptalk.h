/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/cygnusnt/snmptalk.h,v 1.1.1.1 2001/11/05 17:47:09 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/07/10 15:55:15  mrf
 * Add configuration for Cygnus NT environment with associated changes.
 *
 * Revision 1.4  2000/03/17 00:08:57  meister
 * Update copyright message
 *
 * Revision 1.3  1998/06/07 02:06:34  sar
 * Add include of unistd.h
 *
 * Revision 1.2  1998/06/03 20:20:20  sar
 * Moved nvutils.h to snark/h to allow it to be shared.
 * Changed the style for the string routines.  They are now hidden
 * behind a macro that the user can define.  If the user doesn't
 * have them they can install them in the common library using makmak
 *
 * Revision 1.1  1998/02/25 04:56:49  sra
 * Update copyrights.
 *
 * Revision 1.0  1997/08/08 22:11:14  meister
 * Bumped revision number to 1.0 to hopfully get rid of the spurious
 * added files weirdness
 *
 * Revision 0.4  1997/03/20 06:58:01  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 0.3  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 */

/* [clearcase]
modification history
-------------------
*/



#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) unlink(fname)

#include <sys/param.h>

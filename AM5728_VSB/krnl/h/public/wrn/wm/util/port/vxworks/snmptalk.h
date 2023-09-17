/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/vxworks/snmptalk.h,v 1.1.1.1 2001/11/05 17:47:17 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: snmptalk.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/02/04 21:49:31  josh
 * modified header files for clean builds of vxWorks demo
 *
 *
 */

/* [clearcase]
modification history
-------------------
*/




#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) unlink(fname)


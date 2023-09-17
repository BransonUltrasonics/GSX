/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/solaris/snmptalk.h,v 1.1.1.1 2001/11/05 17:47:15 tneale Exp $ */

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
 * Revision 1.1.1.1  2001/11/05 17:47:15  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:16  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:10:54  meister
 * Update copyright message
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




#include <stdlib.h>
#include <strings.h>

/* defines for nvutils for file manipulation */
#define SNMP_READONLY "r"
#define SNMP_READPLUS "r+"
#define SNMP_WRITEPLUS "w+"
#define SNMP_REMOVE_FILE(fname) unlink(fname)

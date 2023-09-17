/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/vxworks/snark.h,v 1.1.1.1 2001/11/05 17:47:17 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: snark.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.1  2000/01/27 18:25:30  josh
 * initial checkin of porting files for vxWorks
 *
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/

#include <string.h>

#define vsnprintf(w, x, y, z)           vsprintf(w, y, z)


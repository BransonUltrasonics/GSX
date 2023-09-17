/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/strdup.h,v 1.1.1.1 2001/11/05 17:47:37 tneale Exp $ */

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
 * $Log: strdup.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:37  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:23:37  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:12:14  meister
 * Update copyright message
 *
 * Revision 1.1  1998/06/03 20:28:47  sar
 * Local version of the strdup function so we don't have to
 * depend on the system for it.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


/* This used to be part of snmptalk/stdf.h it has been moved to
   the snark library so other demo code can use it.  The other
   stdf functions were moved to the common library. */

char *etc_strdup      (char *str);
void  etc_strdup_free (char *str);





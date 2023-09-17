/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/isisnmp/common.h,v 1.1.1.1 2001/11/05 17:47:09 tneale Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: common.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:09  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2001/01/19 22:22:57  paul
 * Update copyright.
 *
 * Revision 1.2  2000/03/17 00:10:00  meister
 * Update copyright message
 *
 * Revision 1.1  1998/06/28 21:18:27  josh
 * updating/bugfixing for SNMp
 *
 */

/* [clearcase]
modification history
-------------------
*/


/*
 * NIY....
 */

#ifdef CONFIG_PREAMBLE

#if INSTALL_BUG
#define BUG_OUT stdout
#define BUG(w,x,y,z) fprintf z; fprintf (stdout, "\n");
#endif

#else /* CONFIG_PREAMBLE */

#endif /* CONFIG_PREAMBLE */



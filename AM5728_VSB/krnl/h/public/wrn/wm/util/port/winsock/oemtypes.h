/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/winsock/oemtypes.h,v 1.1.1.1 2001/11/05 17:47:17 tneale Exp $ */

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
 * $Log: oemtypes.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.6  2001/01/19 22:23:19  paul
 * Update copyright.
 *
 * Revision 1.5  2000/03/17 00:11:05  meister
 * Update copyright message
 *
 * Revision 1.4  1998/02/25 04:57:12  sra
 * Update copyrights.
 *
 * Revision 1.3  1997/03/20 06:58:10  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 1.2  1997/02/25 10:58:16  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 1.1  1996/10/30 17:32:19  mrf
 * Initial revision
 *
 * Revision 1.1  1996/07/02  19:55:38  mrf
 * Initial revision
 *
 * Revision 7.0  1996/03/18  20:25:57  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:51:00  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  17:24:02  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  17:47:40  sar
 * Updated rev to 4.0 and copyright to 93
 *
 * Revision 1.1  1993/06/09  20:37:06  sar
 * Initial revision
 */

/* [clearcase]
modification history
-------------------
*/



/*
 * We need to supply size_t.  Everything else in <types.h> will
 * work fine on with MSDOS compilers.
 */

#if defined(INSTALL_on_unix)
#include <sys/types.h>
#endif
#if defined(INSTALL_on_msdos)
#include <stddef.h>
#endif

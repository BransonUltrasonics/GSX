/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/relay.h,v 1.3 2003/01/15 13:58:30 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 2000-2001 Wind River Systems, Inc.
 *  Copyright 1990-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: relay.h,v $
 * Revision 1.3  2003/01/15 13:58:30  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:27:45  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:36  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:23:36  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/17 00:12:13  meister
 * Update copyright message
 *
 * Revision 1.3  1998/02/25 15:21:44  sra
 * Finish moving types.h, bug.h, and bugdef.h to common/h/.
 *
 * Revision 1.2  1998/02/25 04:57:17  sra
 * Update copyrights.
 *
 * Revision 1.1  1997/04/25 05:43:21  sra
 * Still hacking PCL-725 relay boards after all these years.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef _SNARK_RELAY_H_
#define _SNARK_RELAY_H_

#ifndef	EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif

extern bits8_t relay_read (void);
extern void relay_write (bits8_t bits);
extern void relay_start (bits8_t state1, bits32_t delay, bits8_t state2);

#endif

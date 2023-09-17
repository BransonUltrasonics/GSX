/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/smi.h,v 1.1.1.1 2001/11/05 17:47:23 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: smi.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.2  2001/01/19 22:22:06  paul
 * Update copyright.
 *
 * Revision 9.1  2000/03/17 19:48:38  meister
 * Update copyright notice
 *
 * Revision 9.0  1998/10/16 22:10:18  sar
 * Update version stamp to match release
 *
 * Revision 1.1  1998/05/23 19:51:56  sar
 * Defines for various items that are in the standard smi
 * and might be used by our code.  For example row status
 * and storage tags.
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(smi_inc))
#define smi_inc

#define ETC_STO_OTHER  1
#define ETC_STO_VOL    2
#define ETC_STO_NONVOL 3
#define ETC_STO_PERM   4
#define ETC_STO_RONLY  5

#define ETC_RS_RSNE    0
#define ETC_RS_ACTIVE  1
#define ETC_RS_NIS     2
#define ETC_RS_NREADY  3
#define ETC_RS_CAGO    4
#define ETC_RS_CAWAIT  5
#define ETC_RS_DESTROY 6

#endif

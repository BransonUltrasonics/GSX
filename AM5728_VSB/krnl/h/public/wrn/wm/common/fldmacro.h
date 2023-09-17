/* $Header: /usr/cvsroot/target/h/wrn/wm/common/fldmacro.h,v 1.2 2003/01/16 18:20:12 josh Exp $ */

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
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* 
 * $Log: fldmacro.h,v $
 * Revision 1.2  2003/01/16 18:20:12  josh
 * directory structure shifting
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:21:27  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:16:37  meister
 * Update copyright message
 *
 * Revision 1.2  1998/12/15 05:45:58  sra
 * Get new NI interface code working on pSOS/X86.
 *
 * Revision 1.1  1998/12/09 21:48:44  sra
 * Initial revision.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef COMMON_FLDMACRO_H
#define COMMON_FLDMACRO_H

/*
 * Primative macros to get and set bitfields in an array of integers.
 * Should work for either signed or unsigned integers, which means that
 * these macros have to be very careful about right-shift operations.
 *
 * Parameters:
 *
 * _a_:		"array"		(name of array)
 * _i_:		"index"		(offset into array)
 * _w_:		"width"		(width in bits of the field)
 * _p_:		"position"	(bit offset of field)
 * _n_:		"new_value"	(replacement value, for sets)
 *
 * Macros:
 *
 * FLD_MSK()	Construct mask _w_ bits wide, left shifted _p_ bits
 * FLD_GET()	Extract field to right justified value
 * FLD_SET()	Replace field content with right-justified value
 *
 * I would have preferred the index-and-mask mechanism we used back
 * when all real programmers rode dinosaurs (bareback), because the
 * masks are easier to sight-read than this position-and-width values,
 * but the mask stuff is just too horrid when coded for the ANSI C
 * preprocessor.  I miss MIDAS....
 *
 * These macros are intended primarily for use with IPC mechanisms like
 * pSOS message queues.  For network packets, see the layout directory.
 */

#define FLD_MAX_WIDTH 32

#define FLD_MSK(_w_, _p_) ((~((~0UL) << (_w_))) << (_p_))

#define FLD_GET(_m_, _i_, _w_, _p_) \
  (((_w_) == FLD_MAX_WIDTH) \
   ? ((_m_)[_i_]) \
   : ((((_m_)[_i_]) >> (_p_)) & FLD_MSK(_w_, 0)))

#define FLD_SET(_m_, _i_, _w_, _p_, _n_) \
  (((_m_)[_i_]) = (((_w_) == FLD_MAX_WIDTH) \
		   ? (_n_) \
		   : ((((_m_)[_i_]) & ~FLD_MSK(_w_, _p_)) | \
		      (((_n_) & FLD_MSK(_w_, 0)) << (_p_)))))

#endif /* COMMON_FLDMACRO_H */

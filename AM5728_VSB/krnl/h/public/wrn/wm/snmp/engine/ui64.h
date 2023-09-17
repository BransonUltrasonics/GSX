/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/ui64.h,v 1.2 2001/11/06 21:35:05 josh Exp $ */

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
 * $Log: ui64.h,v $
 * Revision 1.2  2001/11/06 21:35:05  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.4  2001/01/19 22:22:10  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:42  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:35  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 16:59:11  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:21  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:52  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:32  sar
 * Updated revision to 8.0
 *
 * Revision 7.3  1997/10/26 01:44:43  sar
 * Always define the 64 bit counter and include it in the vb defintion
 * instead of forgetting about it if we don't have v2 or v2 types installed.
 *
 * Revision 7.2  1997/03/20 06:48:23  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:56  sra
 * Release 6.0.
 *
 * Revision 5.2  1994/11/29  21:58:53  sar
 * Added a missing : to UI64_Compare.
 *
 * Revision 5.1  1994/09/29  18:31:12  sar
 * Rearrange some items to deal with removing v2 types from v1 settings if
 * they aren't installed.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.4  1993/06/17  23:30:45  sar
 * remvoed the ENVOY_ from UI64 routines and macros.
 *
 * Revision 1.3  1993/06/07  18:31:09  sar
 * Added a set macro to set a 64 bitter from two 32 bitters.
 *
 * Revision 1.2  1993/04/28  22:00:38  sar
 * Moved the definition of UINT_64_T to ui64.h and update others
 *
 * Revision 1.1  1993/04/22  20:14:13  sar
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01b,01apr09,y_t  fix the big endian & little endian issue
01a,19apr05,job  update copyright notices
*/


/* define some macros for manipulating unsinged 64 bit quantities */
#if !defined(ui64_inc)
#define ui64_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

/****************************************************************************
UINT_64_T
****************************************************************************/

/* The following structure is used to hold 64 bit unsigned int.  It is
   targeted for COUNTER64 but for generality it is called UINT_64_T. */

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

#if (_BYTE_ORDER == _BIG_ENDIAN)

typedef struct UINT_64_S
	{
	UINT_32_T  high;	/* the high (msb) 4 bytes of the uint */
	UINT_32_T  low;		/* the low (lsb) 4 bytes of the uint  */
	} UINT_64_T;

#endif

#if (_BYTE_ORDER == _LITTLE_ENDIAN)

typedef struct UINT_64_S
	{
	UINT_32_T  low;	    /* the low (lsb) 4 bytes of the uint */
	UINT_32_T  high;    /* the high (msb) 4 bytes of the uint  */
	} UINT_64_T;

#endif


#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

/****************************************************************************

NAME:	UI64_Set

PURPOSE:  Set a UINT_64_T from two UINT_32_Ts

PARAMETERS:  UINT_64_T * the structure to use
             UINT_32_T   the high bits
	     UINT_32_T   the low bits

RETURNS:  nothing

RESTRICTIONS:

BUGS:
****************************************************************************/
#define UI64_Set(N, H, L) ((N)->high = (H), (N)->low = (L))

/****************************************************************************

NAME:	UI64_High

PURPOSE:  Get the high order 32 bits

PARAMETERS:  UINT_64_T * the structure to use

RETURNS:  UINT_32_T

RESTRICTIONS:

BUGS:
****************************************************************************/
#define UI64_High(N) (N)->high

/****************************************************************************

NAME:	UI64_Low

PURPOSE:  Get the low order 32 bits

PARAMETERS:  UINT_64_T *  Pointer to the object

RETURNS:  UINT_32_T 

RESTRICTIONS:

BUGS:
****************************************************************************/
#define UI64_Low(N) (N)->low

/****************************************************************************

NAME:	UI64_Zero

PURPOSE:  Zero a UINT_64_T object

PARAMETERS:  UINT_64_T *  Pointer to the object to be zeroed.

RETURNS:  

RESTRICTIONS:

BUGS:
****************************************************************************/
#define UI64_Zero(N) (N)->high = (N)->low = 0L

/****************************************************************************

NAME:	UI64_Increment

PURPOSE:  Increment (by one) a UINT_64_T object

PARAMETERS:  UINT_64_T *  Pointer to the object to be incremented.

RETURNS:

RESTRICTIONS:  This routine will wrap the object if necessary and not give
	       any warning about having done so.

BUGS:
****************************************************************************/
#define UI64_Increment(N) if (++((N)->low) == 0) (N)->high++

/****************************************************************************

NAME:	UI64_Add32

PURPOSE:  Add a UINT_32_T to a UINT_64_T

PARAMETERS:  UINT_64_T *  pointer to the object to be updated
	     UINT_32_T    the quantity to add in.

RETURNS:

RESTRICTIONS:  This routine will wrap the object if necessary and not give
	       any warning about having done so.

BUGS:
****************************************************************************/
#define UI64_Add32(N, I) (N)->low += I; if ((N)->low < I) (N)->high++

/****************************************************************************

NAME:	UI64_Add64

PURPOSE:  Add two UINT_64_Ts

PARAMETERS:  UINT_64_T * The object to update
	     UINT_64_T * The object to add in

RETURNS:  Doesn't return anything but the first UINT_64_T is updated.

RESTRICTIONS:  This routine will wrap the object if necessary and not give
	       any warning about having done so.

BUGS:
****************************************************************************/
#define UI64_Add64(N, M) (N)->low += (M)->low; (N)->high += (M)->high; \
			  if ((N)->low < (M)->low) (N)->high++

/****************************************************************************

NAME:	UI64_Sub64

PURPOSE:  Subtract two UINT_64_Ts

PARAMETERS:  UINT_64_T * the result goes here
	     UINT_64_T * Subtract from this object
	     UINT_64_T * the object to subtract

RETURNS:  Doesn't return anything but the first UINT_64_T is filled in.

RESTRICTIONS:  This routine will wrap the object if necessary and not give
	       any warning about having done so.

BUGS:
****************************************************************************/
#define UI64_Sub64(X, Y, Z)  (X)->low = (Y)->low - (Z)->low; \
				   (X)->high = (Y)->high - (Z)->high; \
				   if ((X)->low > (Y)->low) (X)->high--	

/****************************************************************************

NAME:	UI64_Copy

PURPOSE:  Copy one UINT_64_T to another

PARAMETERS:  UINT_64_T * the result goes here
	     UINT_64_T * copy from this object

RETURNS:  Doesn't return anything but the first UINT_64_T is filled in.

RESTRICTIONS:

BUGS:
****************************************************************************/
#define UI64_Copy(X, Y)  (X)->low = (Y)->low; (X)->high = (Y)->high

/****************************************************************************

NAME:	UI64_Compare

PURPOSE:  Compare two UINT_64_Ts

PARAMETERS:  UINT_64_T * The two objects to compare
	     UINT_64_T * 

RETURNS:  < 0 if the first is smaller than the second
	  > 0 if the first is greater than the second
	    0 if they are equal.

RESTRICTIONS:

BUGS:
****************************************************************************/
#define UI64_Compare(X, Y) \
(((X)->high == (Y)->high) ? \
 (((X)->low == (Y)->low)  ? (0) : (((X)->low > (Y)->low) ? (1) : (-1))) : \
 (((X)->high > (Y)->high) ? (1) : (-1)))

#ifdef __cplusplus
}
#endif

#endif /* ui64.h defined */

/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/decode.h,v 1.2 2001/11/06 21:35:02 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1986-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: decode.h,v $
 * Revision 1.2  2001/11/06 21:35:02  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.3  2001/01/19 22:22:04  paul
 * Update copyright.
 *
 * Revision 9.2  2000/03/17 19:48:33  meister
 * Update copyright notice
 *
 * Revision 9.1  2000/03/09 16:55:35  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:09  sar
 * Update version stamp to match release
 *
 * Revision 8.3  1998/05/29 03:53:55  sar
 * Add and use defines for v2 types and v2 protocol pieces.  This allows
 * us to include the types and pieces for other versions, such as v3,
 * without having to touch all of the seperate files that use the
 * types or pieces.
 *
 * Revision 8.2  1998/05/23 20:02:36  sar
 * Added A_DecodeOctetWTC
 *
 * Revision 8.1  1998/02/25 04:50:32  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:24  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:08  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * removed bitstring & uinteger32 options
 *
 * Revision 6.0  1995/05/31  21:46:40  sra
 * Release 6.0.
 *
 * Revision 5.1  1994/09/29  18:31:12  sar
 * Rearrange some items to deal with removing v2 types from v1 settings if
 * they aren't installed.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.1  1994/05/02  17:03:55  sar
 * Added a new decode error (AE_WRONG_TYPE) for use with some new routines
 * that do type checking while pulling a value out.
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.5  1993/06/17  23:30:45  sar
 * Modifed decode of bit strings as it was wrong.
 *
 * Revision 3.4  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 3.3  1993/04/28  22:00:38  sar
 * Moved the definition of UINT_64_T to ui64.h and update others
 *
 * Revision 3.2  1993/04/26  20:33:14  sar
 * Added bitstrings, and have deletion of parties or contexts delete acls.
 *
 * Revision 3.1  1993/03/25  21:15:21  sar
 * Added types for uint 64 (counter64) and nsapaddress (translates to string)
 *
 * Revision 3.0  1992/04/03  19:52:27  dab
 * Release 3.0
 *
 * Revision 2.101  92/02/05  18:49:08  dab
 * Fixed up casting in A_DecodeTypeClass for MSC.
 * 
 * Revision 2.100  91/08/09  14:09:45  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:23  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:26
 * Release 2.00
 * 
 *    Rev 1.3   23 Mar 1989 11:55:58
 * Merged the decode helper into the one routine that used it.
 * 
 *    Rev 1.2   18 Mar 1989 11:57:26
 * Revised prototypes for A_DecodeOctetStringData and A_DecodeOctetString
 * 
 *    Rev 1.1   19 Sep 1988 17:27:04
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:14
 * Initial revision.
*/

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(decode_inc))
#define decode_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(localio_inc))
#include <wrn/wm/snmp/engine/localio.h>
#endif

#if (!defined(buffer_inc))
#include <wrn/wm/snmp/engine/buffer.h>
#endif

#if (!defined(ui64_inc))
#include <wrn/wm/snmp/engine/ui64.h>
#endif

/****************************************************************************
NAME:  A_DecodeTypeClass

PURPOSE:  Decode the Class and Form bits from an ASN.1 type field.
	  The data stream is read using the local I/O package.
	  The stream pointer is NOT advanced, leaving it pointing to the
	  start of the type field, ready for a subsequent call to
	  A_DecodeTypeValue.

CAVEAT:	  The user should call this return *BEFORE* calling
	  A_DecodeTypeValue.

PARAMETERS:  LCL_FILE *	    A stream descriptor (already open)

RETURNS:  OCTET_T	    The Class and Form bits

RESTRICTIONS:  It is assumed that the stream is not at EOF.
****************************************************************************/

#define A_DecodeTypeClass(L) ((OCTET_T) (Lcl_Peekc((LCL_FILE *)(L)) &	\
					 A_IDCF_MASK))

/****************************************************************************
Decoding errors
****************************************************************************/
#define	AE_PREMATURE_END		1
#define	AE_INDEFINITE_LENGTH		2
#define AE_WRONG_VALUE			3
#define AE_WRONG_TYPE			4
#define AE_WRONG_LENGTH			5
#define AE_ALLOC_FAILURE		6

extern	ATVALUE_T	A_DecodeTypeValue	__((LCL_FILE *, int *));
extern	ALENGTH_T	A_DecodeLength		__((LCL_FILE *, int *));
extern	void		A_DecodeOctetStringData __((LCL_FILE *, ALENGTH_T,
						    EBUFFER_T *, int *));
extern	void		A_DecodeOctetString	__((LCL_FILE *, EBUFFER_T *,
						    int *));
extern	void		A_DecodeOctetStringWTC	__((LCL_FILE *, EBUFFER_T *,
						    int *, ATVALUE_T,
						    OCTET_T));
extern	bits8_t		A_DecodeOctetWTC	__((LCL_FILE *, int *,
						    ATVALUE_T, bits8_t));
extern	INT_32_T	A_DecodeIntegerData	__((LCL_FILE *, ALENGTH_T,
						    int *));
extern	INT_32_T	A_DecodeInteger		__((LCL_FILE *, int *));
extern	INT_32_T	A_DecodeIntegerWTC	__((LCL_FILE *, int *,
						    ATVALUE_T, OCTET_T));
extern	void		A_DecodeObjectIdData	__((LCL_FILE *, ALENGTH_T,
						    OBJ_ID_T *, int *));
extern	void		A_DecodeObjectId	__((LCL_FILE *, OBJ_ID_T *,
						    int *));
extern	void		A_DecodeObjectIdWTC	__((LCL_FILE *, OBJ_ID_T *,
						    int *, ATVALUE_T,
						    OCTET_T));

/* We only install some functions if their types are installed */
#if (ENVOY_USE_V2_TYPES)
extern	void		A_DecodeInteger64Data	__((LCL_FILE *, ALENGTH_T, 
					  	    UINT_64_T *, int *));
extern	void		A_DecodeInteger64	__((LCL_FILE *, UINT_64_T *,
						    int *));
#endif

#ifdef __cplusplus
}
#endif

#endif /* decode_inc */

/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/encode.h,v 1.2 2001/11/06 21:35:02 josh Exp $ */

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
 * $Log: encode.h,v $
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
 * Revision 9.1  2000/03/09 16:55:43  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:10  sar
 * Update version stamp to match release
 *
 * Revision 8.2  1998/05/29 03:53:55  sar
 * Add and use defines for v2 types and v2 protocol pieces.  This allows
 * us to include the types and pieces for other versions, such as v3,
 * without having to touch all of the seperate files that use the
 * types or pieces.
 *
 * Revision 8.1  1998/02/25 04:50:33  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:24  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:09  sra
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
 * Revision 6.0  1995/05/31  21:46:41  sra
 * Release 6.0.
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
 * Revision 2.101  91/08/12  12:53:01  dab
 * Added EHELPER_T so as to declare the arguments to the helper function.
 * 
 * Revision 2.100  91/08/09  14:09:46  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:24  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:24
 * Release 2.00
 * 
 *    Rev 1.1   19 Sep 1988 17:27:06
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:16
 * Initial revision.
*/

/* [clearcase]
modification history
-------------------
01b,03jun09,y_t  update for requirement WIND00161876
01a,19apr05,job  update copyright notices
*/


#if (!defined(encode_inc))
#define encode_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(buffer_inc))
#include <wrn/wm/snmp/engine/buffer.h>
#endif

#if (!defined(ui64_inc))
#include <wrn/wm/snmp/engine/ui64.h>
#endif

typedef ALENGTH_T	(EHELPER_T)	__((PTR_T, OCTET_T *, ALENGTH_T));
extern	ALENGTH_T	A_SizeOfInt		__((INT_32_T));
extern	ALENGTH_T	A_SizeOfUnsignedInt	__((UINT_32_T));
extern	ALENGTH_T	A_SizeOfObjectId	__((OBJ_ID_T *));

/* We only install some functions if their types are installed */
#if (ENVOY_USE_V2_TYPES)
extern	ALENGTH_T	A_SizeOfUnsignedInt64	__((UINT_64_T *));
#endif

extern	void		A_EncodeType		__((ATVALUE_T, OCTET_T,
						    EHELPER_T *, OCTET_T *));
extern	void		A_EncodeLength		__((ALENGTH_T, EHELPER_T *,
						    OCTET_T *));
extern	void		A_EncodeInt		__((ATVALUE_T, OCTET_T,
						    INT_32_T,  EHELPER_T *,
						    OCTET_T *));
extern	void		A_EncodeUnsignedInt	__((ATVALUE_T, OCTET_T,
						    UINT_32_T, EHELPER_T *,
						    OCTET_T *));
extern	void		A_EncodeOctetString	__((ATVALUE_T, OCTET_T,
						    OCTET_T *, ALENGTH_T,
						    EHELPER_T *, OCTET_T *));
extern	void		A_EncodeObjectId	__((ATVALUE_T, OCTET_T,
						    OBJ_ID_T *, EHELPER_T *,
						    OCTET_T *));

/* We only install some functions if their types are installed */
#if (ENVOY_USE_V2_TYPES)
extern	void		A_EncodeUnsignedInt64	__((ATVALUE_T, OCTET_T,
						    UINT_64_T , EHELPER_T *,
						    OCTET_T *));
#endif

extern	EHELPER_T	A_EncodeHelper;

#ifdef __cplusplus
}
#endif

#endif	/* encode_inc */


/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/buildpkt.h,v 1.2 2001/11/06 21:35:02 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: buildpkt.h,v $
 * Revision 1.2  2001/11/06 21:35:02  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:22  tneale
 * Tornado shuffle
 *
 * Revision 9.7  2001/01/19 22:22:04  paul
 * Update copyright.
 *
 * Revision 9.6  2000/03/17 19:48:32  meister
 * Update copyright notice
 *
 * Revision 9.5  2000/03/09 16:55:22  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.4  2000/03/09 13:51:30  josh
 * a shell function has been created to allow SNMPv3 requests
 * to be created while we're holding the coarse lock.
 *
 * Revision 9.3  1999/09/21 21:30:20  josh
 * correcting VBL_Bind_* macros
 *
 * Revision 9.2  1999/09/14 19:44:11  josh
 * prototypes and macros for VBL_Bind_*() functions/macros
 *
 * Revision 9.1  1999/04/09 20:37:00  josh
 * Obsoleting ENVOY_SNMP_VERSON_USEC and ENVOY_40_VB_FREE_COMPAT
 *
 * Revision 9.0  1998/10/16 22:10:08  sar
 * Update version stamp to match release
 *
 * Revision 8.5  1998/07/20 02:01:23  sar
 * Removed dynamic/static tags from v3 packet creation routine
 *
 * Revision 8.4  1998/06/18 04:42:02  sar
 * Modified bflags in ebuffer to be unsigned ints and updated
 * calling routines accordingly
 *
 * Revision 8.3  1998/05/29 03:53:54  sar
 * Add and use defines for v2 types and v2 protocol pieces.  This allows
 * us to include the types and pieces for other versions, such as v3,
 * without having to touch all of the seperate files that use the
 * types or pieces.
 *
 * Revision 8.2  1998/05/23 20:04:07  sar
 * Added creation function for v3 requests
 *
 * Revision 8.1  1998/02/25 04:50:30  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:23  sar
 * Updated revision to 8.0
 *
 * Revision 7.4  1997/03/20 06:48:06  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.3  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.2  1996/12/04 18:42:08  sar
 * Added a definition for SNMP_Bind_OBJECT_ID to make older versions
 * of Emissary happy.
 *
 * Revision 7.1  1996/10/15  14:28:01  sar
 * Added create_internal_request
 *
 * Revision 7.0  1996/03/18  20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.2  1996/01/10  16:23:25  sar
 * USEC updates
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * remvoed nsapaddress function
 * removed create functions for party system
 * remvoed bitstring function
 *
 * Revision 6.0  1995/05/31  21:46:38  sra
 * Release 6.0.
 *
 * Revision 5.3  1995/03/20  23:11:33  sar
 * Moved index_to_vbp to snmpdefs.h and included snmpdefs.h for backward
 * compatibility.
 *
 * Revision 5.2  1994/10/03  21:20:27  sar
 * Added the def for SNMP_Create_V2_Half_Trap
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
 * Revision 3.6  1993/05/17  14:04:13  sar
 * changed SNMP_Create_Party_Request to be SNMP_Create_V2_Request
 *
 * Revision 3.5  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 3.4  1993/04/27  21:06:48  sar
 * Removed index_to_vbp it is now in buildpkt.h
 *
 * Revision 3.3  1993/04/26  20:33:14  sar
 * Added bitstrings, and have deletion of parties or contexts delete acls.
 *
 * Revision 3.2  1993/03/25  21:15:21  sar
 * Added types for uint 64 (counter64) and nsapaddress (translates to string)
 *
 * Revision 3.1  1993/02/17  20:47:28  sar
 * Added SNMP_Create_Party_Request - definition: create a v2 packet.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.100  91/08/09  14:09:45  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:22  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:20
 * Release 2.00
 * 
 *    Rev 1.4   25 Jan 1989 11:55:50
 * Corrected definitions of macros SNMP_Bind_Counter, SNMP_Bind_Gauge, and
 * SNMP_Bind_Timeticks -- each of these had the type mis-defined.
 * Also added a new macro -- SNMP_Bind_Opaque.
 * 
 *    Rev 1.3   11 Jan 1989 12:10:10
 * Buildpkt.c split into multiple files to prevent linker from picking up
 * unused routines.
 * 
 *    Rev 1.2   22 Sep 1988 18:38:14
 * Added definitions for SNMP_Bind_Null().
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


#if (!defined(buildpkt_inc))
#define buildpkt_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(snmp_inc))
#include <wrn/wm/snmp/engine/snmp.h>
#endif

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#define SNMP_Bind_Counter(P,I,C,L,V)	SNMP_Bind_Unsigned_Integer(P, I, C, \
							L, VT_COUNTER, V)

#define SNMP_Bind_Gauge(P,I,C,L,V)	SNMP_Bind_Unsigned_Integer(P, I, C, \
							L, VT_GAUGE, V)

#define SNMP_Bind_Timeticks(P,I,C,L,V)	SNMP_Bind_Unsigned_Integer(P, I, C, \
							L, VT_TIMETICKS, V)

#define SNMP_Bind_Opaque(P,I,C,L,W,S,D)	SNMP_Bind_String(P, I, C, L, \
							VT_OPAQUE, W, S, D)

#define VBL_Bind_Counter(P,I,C,L,V)	VBL_Bind_Unsigned_Integer(P, I, C, \
							   L, VT_COUNTER, V)

#define VBL_Bind_Gauge(P,I,C,L,V)	VBL_Bind_Unsigned_Integer(P, I, C, \
							L, VT_GAUGE, V)

#define VBL_Bind_Timeticks(P,I,C,L,V)	VBL_Bind_Unsigned_Integer(P, I, C, \
							L, VT_TIMETICKS, V)

#define VBL_Bind_Opaque(P,I,C,L,W,S,D)	VBL_Bind_String(P, I, C, L, \
							VT_OPAQUE, W, S, D)

/* Defined to clean up an error in some versions of Emissary */
#define SNMP_Bind_OBJECT_ID             SNMP_Bind_Object_ID


/* We only install some functions if their types are installed */
#if (ENVOY_USE_V2_TYPES)
#define SNMP_Bind_Counter64(P,I,C,L,H,LO) SNMP_Bind_64_Unsigned_Integer(P, I,\
						C, L, VT_COUNTER64, H, LO)
#define VBL_Bind_Counter64(P,I,C,L,H,LO) VBL_Bind_64_Unsigned_Integer(P, I,\
						C, L, VT_COUNTER64, H, LO)
#endif

#if (INSTALL_ENVOY_SNMP_VERSION_3)
extern	SNMP_PKT_T * SNMP_Create_Request_V3_Lockable __((int, int, bits32_t,
							 bits32_t, bits8_t, 
							 bits32_t, bits8_t *, 
							 ALENGTH_T, bits8_t *,
							 ALENGTH_T, bits8_t *,
							 ALENGTH_T, bits8_t *,
							 ALENGTH_T, sbits32_t,
							 int, int, int, int));
extern	SNMP_PKT_T * SNMP_Create_Request_V3 __((int, int, bits32_t,
						bits32_t, bits8_t, bits32_t,
						bits8_t *, ALENGTH_T,
						bits8_t *, ALENGTH_T,
						bits8_t *, ALENGTH_T,
						bits8_t *, ALENGTH_T,
						sbits32_t, int, int, int));
#endif

extern	SNMP_PKT_T *	SNMP_Create_Request2	__((int, int, int, char *,
						    INT_32_T, int, int, int));
#define SNMP_Create_Request(PDU, VER, CLEN, CSTR, RID, VBS) \
        SNMP_Create_Request2(PDU, VER, CLEN, CSTR, RID, VBS, 0, 0)

extern SNMP_PKT_T *SNMP_Create_Internal_Request __((int, int, int, sbits8_t *,
						    unsigned short int,
						    INT_32_T, int, int, int,
						    ALENGTH_T, IO_COMPLETE_T *,
						    ERR_COMPLETE_T *, PTR_T));

extern	SNMP_PKT_T *	SNMP_Create_Trap	__((int, int, char *, int,
						    OIDC_T *, OCTET_T *, int,
						    INT_32_T, UINT_32_T, int));

extern	int	SNMP_Bind_Integer	   __((SNMP_PKT_T *, int, int,
					       OIDC_T *, INT_32_T));
extern	int	SNMP_Bind_Unsigned_Integer __((SNMP_PKT_T *, int, int, 
					       OIDC_T *, OCTET_T, UINT_32_T));
extern	int	SNMP_Bind_IP_Address	   __((SNMP_PKT_T *, int, int,
					       OIDC_T *, OCTET_T *));
extern	int	SNMP_Bind_Object_ID	   __((SNMP_PKT_T *, int, int,
					       OIDC_T *, int, OIDC_T *));
extern	int	SNMP_Bind_String	   __((SNMP_PKT_T *, int, int,
					       OIDC_T *, OCTET_T,
					       int, OCTET_T *, int));
extern	int	SNMP_Bind_Null		__((SNMP_PKT_T *, int, int, OIDC_T *));

extern	int	VBL_Bind_Integer	   __((VBL_T *, int, int,
					       OIDC_T *, INT_32_T));
extern	int	VBL_Bind_Unsigned_Integer __((VBL_T *, int, int, 
					      OIDC_T *, OCTET_T, UINT_32_T));
extern	int	VBL_Bind_IP_Address	   __((VBL_T *, int, int,
					       OIDC_T *, OCTET_T *));
extern	int	VBL_Bind_Object_ID	   __((VBL_T *, int, int,
					       OIDC_T *, int, OIDC_T *));
extern	int	VBL_Bind_String	   __((VBL_T *, int, int,
				       OIDC_T *, OCTET_T,
				       int, OCTET_T *, int));
extern	int	VBL_Bind_Null		__((VBL_T *, int, int, OIDC_T *));

/* We only install some functions if their types are installed */
#if (ENVOY_USE_V2_TYPES)
extern	int	SNMP_Bind_64_Unsigned_Integer __((SNMP_PKT_T *, int, int,
						  OIDC_T *, OCTET_T,
						  UINT_32_T, UINT_32_T));

extern	int	VBL_Bind_64_Unsigned_Integer __((VBL_T *, int, int,
						 OIDC_T *, OCTET_T,
						 UINT_32_T, UINT_32_T));
#endif

#ifdef __cplusplus
}
#endif

#endif	/* buildpkt_inc */


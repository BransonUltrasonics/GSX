/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/objectid.h,v 1.2 2001/11/06 21:35:03 josh Exp $ */

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
 * $Log: objectid.h,v $
 * Revision 1.2  2001/11/06 21:35:03  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.3  2001/01/19 22:22:05  paul
 * Update copyright.
 *
 * Revision 9.2  2000/03/17 19:48:36  meister
 * Update copyright notice
 *
 * Revision 9.1  2000/03/09 16:57:20  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:16  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:42  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:27  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:14  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:48  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/03/20  23:11:33  sar
 * Minor cleanup.
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
 * Revision 3.3  1993/04/27  21:06:48  sar
 * added copy_object_id which does a straight copy, beware free one copy
 * while using the other.
 *
 * Revision 3.2  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.1  1993/02/17  20:49:21  sar
 * Added no_pp definition (#define __(x) x stuff)
 * Added init_object_id - macro: init an OBJ_ID_T to 0 components and 0 list.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 2.100  91/08/09  14:09:47  dab
 * Update version.
 * 
 * Revision 1.1  91/07/30  02:24:29  romkey
 * Initial revision
 * 
 * 
 *    Rev 2.0   31 Mar 1990 15:11:20
 * Release 2.00
 * 
 *    Rev 1.2   11 Jan 1989 12:46:46
 * Moved Clean_Obj_ID() to objectid.c
 * 
 *    Rev 1.1   19 Sep 1988 17:27:08
 * Made changes to make the Sun C compiler happy.
 * 
 *    Rev 1.0   12 Sep 1988 10:46:20
 * Initial revision.
*/

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(objectid_inc))
#define objectid_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#define	clone_object_id(O, N) build_object_id(	\
				((OBJ_ID_T *)(O))->num_components,	\
				((OBJ_ID_T *)(O))->component_list, N)

#define init_object_id(O) ((OBJ_ID_T *)(O))->num_components = 0; \
			  ((OBJ_ID_T *)(O))->component_list = 0;

#define	copy_object_id(O, N) \
(((OBJ_ID_T *)(N))->num_components = ((OBJ_ID_T *)(O))->num_components, \
 ((OBJ_ID_T *)(N))->component_list = ((OBJ_ID_T *)(O))->component_list)

extern	int		build_object_id	__((int, OIDC_T *, OBJ_ID_T *));
extern	void		Clean_Obj_ID	__((OBJ_ID_T *));

#ifdef __cplusplus
}
#endif

#endif	/* objectid_inc */

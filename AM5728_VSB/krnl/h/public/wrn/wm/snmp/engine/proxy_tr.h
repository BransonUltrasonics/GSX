/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/proxy_tr.h,v 1.2 2001/11/06 21:35:03 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1995-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: proxy_tr.h,v $
 * Revision 1.2  2001/11/06 21:35:03  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.4  2001/01/19 22:22:06  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:37  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 20:58:34  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.1  2000/03/09 16:57:46  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:17  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:43  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:27  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:15  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 1.1  1995/11/14  22:41:44  sar
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(proxy_tr_inc))
#define proxy_tr_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#if (!defined(buffer_inc))
#include <wrn/wm/snmp/engine/buffer.h>
#endif

#if (INSTALL_ENVOY_SNMP_PROXY)
typedef INT_32_T TRANSPORT_ROUTINE_T __((EBUFFER_T *, PTR_T));

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct PROXY_BLOCK_S
	{
	EBUFFER_T	     community;
	TRANSPORT_ROUTINE_T *transport_routine;
	PTR_T		     transport_block;
        } PROXY_BLOCK_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

extern INT_32_T SNMP_Proxy_V1_Translation __((SNMP_PKT_T *));

extern INT_32_T SNMP_Proxy_Trap_V1_To_V2  __((SNMP_PKT_T *));
extern INT_32_T SNMP_Proxy_V2_Translation __((SNMP_PKT_T *));

#endif /* #if (INSTALL_ENVOY_SNMP_PROXY) */

#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(proxy_tr_inc)) */


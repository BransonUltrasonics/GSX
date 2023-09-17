/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/proxy.h,v 1.3 2001/11/06 21:50:40 josh Exp $ */

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
 * $Log: proxy.h,v $
 * Revision 1.3  2001/11/06 21:50:40  josh
 * second (and hopefully final) pass of new path hacking
 *
 * Revision 1.2  2001/11/06 21:35:03  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.6  2001/01/19 22:22:05  paul
 * Update copyright.
 *
 * Revision 9.5  2000/03/17 19:48:37  meister
 * Update copyright notice
 *
 * Revision 9.4  2000/03/09 20:58:34  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.3  2000/03/09 16:57:34  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.2  2000/02/29 20:40:49  josh
 * #ifdefs so we don't have semicolons sitting on lines alone in include
 * files (the ARM compiler hates that).
 *
 * Revision 9.1  1999/11/01 19:56:52  josh
 * cleaning up initializations to keep extra files out of code when not
 * in use
 *
 * Revision 9.0  1998/10/16 22:10:16  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/08/12 04:52:14  sar
 * revamp the initialization routines to limit the amount of
 * code that might get pulled in.
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(proxy_ini_inc))
#define proxy_ini_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(snmp_inc))
#include <wrn/wm/snmp/engine/snmp.h>
#endif

#if INSTALL_ENVOY_SNMP_LOCK
ENVOY_SNMP_LOCK_EXTERN(SNMP_ProxyLock);
#endif

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef struct PROXY_CB_S
	{
	struct PROXY_CB_S	*next;
	UINT_32_T		 indx;
	UINT_32_T		 reqid;
	EBUFFER_T		 community;
	SNMPADDR_T		 loc_addr;
	SNMPADDR_T		 for_addr;
	IO_COMPLETE_T		*io_comp;
	ERR_COMPLETE_T		*err_comp;
	PTR_T			 async_cookie;
	ALENGTH_T		 maxpkt;
	ATVALUE_T		 pdu_type;
	INT_32_T		 original_version;
	} PROXY_CB_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif

extern void envoy_proxy_init __((void));

#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(proxy_ini_inc)) */


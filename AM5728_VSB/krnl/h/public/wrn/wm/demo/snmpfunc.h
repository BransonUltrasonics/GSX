/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/snmpfunc.h,v 1.3 2003/01/15 13:58:30 josh Exp $ */

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
 * $Log: snmpfunc.h,v $
 * Revision 1.3  2003/01/15 13:58:30  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:27:45  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:37  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:23:37  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/17 00:12:13  meister
 * Update copyright message
 *
 * Revision 1.2  1998/06/07 02:29:36  sar
 * Add trap community
 *
 * Revision 1.1  1998/06/07 02:12:01  sar
 * Gather a group of common snmp functions in snark/lib so
 * we don't have to rewrite them for each new demo.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef snark_snmpfunc_ins
#define snark_snmpfunc_ins

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif

#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif

#include <wrn/wm/snmp/engine/snmpdefs.h>
#include <wrn/wm/snmp/engine/buffer.h>
#include <wrn/wm/snmp/engine/proxy_tr.h>

#define SNMP_COMM_MAX 32

#if (INSTALL_ENVOY_SNMP_PROXY)
typedef struct TRANS_STUFF_S {
	PROXY_BLOCK_T  pblockp;
	bits16_t       port;
	bits8_t       *addr_str;
	ptr_t         *pkt;
	} TRANS_STUFF_T;
#endif

extern int validate_SNMP_community  __((SNMP_PKT_T *, SNMPADDR_T *,
					SNMPADDR_T *));

extern int validate_SNMP_V3_Address __((SNMP_PKT_T *, SNMPADDR_T *,
					SNMPADDR_T *));

extern sbits32_t attache_proxy_send_rtn __((EBUFFER_T *, ptr_t));

extern char snmp_get_request_community[];
extern char snmp_set_request_community[];
extern char snmp_trap_community[];

#endif

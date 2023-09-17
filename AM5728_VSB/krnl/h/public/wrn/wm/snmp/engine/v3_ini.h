/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/v3_ini.h,v 1.3 2001/11/06 21:50:41 josh Exp $ */

/*
 * Copyright (C) 1999-2012, 2015 Wind River Systems, Inc.
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
 * Portions of this file are subject to additional licensing terms found in
 * the COPYING-SNMP.txt file that is included with this distribution.
 */

/*
 * $Log: v3_ini.h,v $
 * Revision 1.3  2001/11/06 21:50:41  josh
 * second (and hopefully final) pass of new path hacking
 *
 * Revision 1.2  2001/11/06 21:35:06  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:24  tneale
 * Tornado shuffle
 *
 * Revision 9.3  2001/01/19 22:22:11  paul
 * Update copyright.
 *
 * Revision 9.2  2000/03/17 19:48:45  meister
 * Update copyright notice
 *
 * Revision 9.1  2000/03/09 17:00:12  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.0  1998/10/16 22:10:25  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/08/12 04:52:16  sar
 * revamp the initialization routines to limit the amount of
 * code that might get pulled in.
 *
 */

/* [clearcase]
modification history
-------------------
21may15,r_w  change to _WRS_CONFIG_OPENSSL_FIPS for FIPS
20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support.
16jun11,y_t  update for requirement WIND00242199
19apr05,job  update copyright notices
*/


#if (!defined(v3_ini_inc))
#define v3_ini_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(snmp_inc))
#include <wrn/wm/snmp/engine/snmp.h>
#endif

extern void envoy_v3_init __((void));

#ifdef _WRS_CONFIG_OPENSSL_FIPS
#define OPENSSL_FIPS
#endif
#ifdef _WRS_CONFIG_SNMP_USE_TLS_DTLS_TRANSPORT
#define SNMP_TLS_DTLS_TRANSPORT_ENABLE
#endif

#ifdef _WRS_CONFIG_SNMP_USE_SSH_TRANSPORT
#define SNMP_SSH_TRANSPORT_ENABLE
#endif


#ifdef __cplusplus
}
#endif

#endif /* #if (!defined(v3_ini_inc)) */




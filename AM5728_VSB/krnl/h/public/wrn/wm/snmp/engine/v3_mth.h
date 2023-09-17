/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/v3_mth.h,v 1.2 2001/11/06 21:35:06 josh Exp $ */

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
 * $Log: v3_mth.h,v $
 * Revision 1.2  2001/11/06 21:35:06  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:24  tneale
 * Tornado shuffle
 *
 * Revision 9.4.4.1  2001/07/16 18:02:13  josh
 * a first-round commit adding infrastructure support
 * for the SNMPv3 Coexistence RFC (2576)
 *
 * Revision 9.4  2001/01/19 22:22:12  paul
 * Update copyright.
 *
 * Revision 9.3  2000/03/17 19:48:45  meister
 * Update copyright notice
 *
 * Revision 9.2  2000/03/09 17:00:24  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.1  1999/10/15 17:44:50  josh
 * added prototypes for method routines for target and notify MIBs
 *
 * Revision 9.0  1998/10/16 22:10:25  sar
 * Update version stamp to match release
 *
 * Revision 1.2  1998/07/03 16:54:53  sar
 * Remove many engine install option, moved this engine's information
 * into an engine structure
 *
 * Revision 1.1  1998/05/23 19:49:40  sar
 * Include files for use with v3 code.  They define the structures
 * and entry points for the various pieces.
 * acc: access and group information
 * auth & priv: authentication and privacy
 * con: context
 * eng: engine ids, both this agents and any others it knows
 * mth: method routines, for use with mib control files
 * user: user info
 *
 */

/* [clearcase]
modification history
-------------------
01b,21apr05,AJS  Diffie-Hellman work
01a,19apr05,job  update copyright notices
01a,13apr05,AJS  Diffie-Hellman work
*/


#if (!defined(v3_mth_inc))
#define v3_mth_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1_inc))
#include <wrn/wm/snmp/engine/asn1.h>
#endif

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#if (!defined(vb_inc))
#include <wrn/wm/snmp/engine/vbdefs.h>
#endif

/* for our engine information */
extern void get_snmpEngineID     __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void get_snmpEngineBoots  __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void get_snmpEngineTime   __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
/* for the targetaddr table */
extern void snmpTargetAddrEntry_test       __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));
extern void snmpTargetAddrEntry_set        __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));
extern void snmpTargetAddrEntry_get        __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));
extern void snmpTargetAddrEntry_next       __((OIDC_T, int, OIDC_T *,
                                               SNMP_PKT_T *, VB_T *));

/* for the targetparams table */
extern void snmpTargetParamsEntry_test       __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));
extern void snmpTargetParamsEntry_set        __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));
extern void snmpTargetParamsEntry_get        __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));
extern void snmpTargetParamsEntry_next       __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));

/* for the notify table */
extern void snmpNotifyEntry_test       __((OIDC_T, int, OIDC_T *,
                                           SNMP_PKT_T *, VB_T *));
extern void snmpNotifyEntry_set        __((OIDC_T, int, OIDC_T *,
                                           SNMP_PKT_T *, VB_T *));
extern void snmpNotifyEntry_get        __((OIDC_T, int, OIDC_T *,
                                           SNMP_PKT_T *, VB_T *));
extern void snmpNotifyEntry_next       __((OIDC_T, int, OIDC_T *,
                                           SNMP_PKT_T *, VB_T *));

/* for the notifyfilterprofile table */
extern void snmpNotifyFilterProfileEntry_test       __((OIDC_T, int, OIDC_T *,
                                                        SNMP_PKT_T *, VB_T *));
extern void snmpNotifyFilterProfileEntry_set        __((OIDC_T, int, OIDC_T *,
                                                        SNMP_PKT_T *, VB_T *));
extern void snmpNotifyFilterProfileEntry_get        __((OIDC_T, int, OIDC_T *,
                                                        SNMP_PKT_T *, VB_T *));
extern void snmpNotifyFilterProfileEntry_next       __((OIDC_T, int, OIDC_T *,
                                                        SNMP_PKT_T *, VB_T *));

/* for the notifyfilter table */
extern void snmpNotifyFilterEntry_test       __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));
extern void snmpNotifyFilterEntry_set        __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));
extern void snmpNotifyFilterEntry_get        __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));
extern void snmpNotifyFilterEntry_next       __((OIDC_T, int, OIDC_T *,
                                                 SNMP_PKT_T *, VB_T *));

/* for the proxy table */
extern void snmpProxyEntry_test       __((OIDC_T, int, OIDC_T *,
                                          SNMP_PKT_T *, VB_T *));
extern void snmpProxyEntry_set        __((OIDC_T, int, OIDC_T *,
                                          SNMP_PKT_T *, VB_T *));
extern void snmpProxyEntry_get        __((OIDC_T, int, OIDC_T *,
                                          SNMP_PKT_T *, VB_T *));
extern void snmpProxyEntry_next       __((OIDC_T, int, OIDC_T *,
                                          SNMP_PKT_T *, VB_T *));

/* for the user table */
extern void usertable_test       __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void usertable_set        __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void usertable_get        __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void usertable_next       __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
                                     
/* Diffie-Hellman usmDHParameters object */

extern void usmDHPublicObjects_get  __((OIDC_T, int, OIDC_T *,
                                        SNMP_PKT_T *, VB_T *));
extern void usmDHPublicObjects_next __((OIDC_T, int, OIDC_T *,
                                        SNMP_PKT_T *, VB_T *));
extern void usmDHPublicObjects_test __((OIDC_T, int, OIDC_T *,
                                        SNMP_PKT_T *, VB_T *));
extern void usmDHPublicObjects_set  __((OIDC_T, int, OIDC_T *,
                                        SNMP_PKT_T *, VB_T *));

/* for the context table, read only so don't have test or set */
extern void contexttable_get     __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void contexttable_next    __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));


/* for the security to group and access tables */
extern void  grouptable_test     __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  grouptable_set      __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  grouptable_get      __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  grouptable_next     __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  accesstable_test    __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  accesstable_set     __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  accesstable_get     __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
extern void  accesstable_next    __((OIDC_T, int, OIDC_T *,
                                     SNMP_PKT_T *, VB_T *));
/* for the community table */
extern void snmpCommunityEntry_test       __((OIDC_T, int, OIDC_T *,
                                              SNMP_PKT_T *, VB_T *));
extern void snmpCommunityEntry_set        __((OIDC_T, int, OIDC_T *,
                                              SNMP_PKT_T *, VB_T *));
extern void snmpCommunityEntry_get        __((OIDC_T, int, OIDC_T *,
                                              SNMP_PKT_T *, VB_T *));
extern void snmpCommunityEntry_next       __((OIDC_T, int, OIDC_T *,
                                              SNMP_PKT_T *, VB_T *));

#ifdef __cplusplus
}
#endif

#endif

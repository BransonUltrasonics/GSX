/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/vxagent/snmpdLib.h,v 1.4 2001/12/20 21:41:07 meister Exp $ */

/* snmpdLib.h - VxWorks SNMP Agent */

/*
 * Copyright (C) 1999-2012, 2014, 2016 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/* 
 * Portions of this file are subject to additional licensing terms found in
 * the COPYING-SNMP.txt file that is included with this distribution.
 */

/*
modification history
--------------------
23nov16,r_w  move #include <netinet/in.h> outside extern "C" block (V7MAN-224)
30may14,bbz  resolve snmp engine's dependence on snmp agent(V7MAN-82)
19dec12,bbz  add routine for installing pre-set hook(WIND00392673)
20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
             TLS,DTLS,SSH transport model support. 
10jun10,y_t  update for LP64
16sep08,y_t  update for snmpdMemoryAlloc redefine
19apr05,job  update copyright notices
25nov03,ps  copyright update
05nov03,ps  merge from latest on ickthwa_devel_branch.nh
02oct98,ann  added missing function prototypes (SPR# 7017)
30apr98,ann  changed snmpVbPrivFree prototype to avoid 
             conflict with master-subagent.
07jun96,rjc  removed extra } #ifdef'd under _cplusplus
16may96,rjc  rewritten for snmp v1/v2c product
18feb94,elh  written. 
*/

/*
 * $Log: snmpdLib.h,v $
 * Revision 1.4  2001/12/20 21:41:07  meister
 * Add some more prototypes
 *
 * Revision 1.2  2001/11/06 21:50:42  josh
 * second (and hopefully final) pass of new path hacking
 *
 * Revision 1.1.1.1  2001/11/05 17:47:25  tneale
 * Tornado shuffle
 *
 * Revision 1.4  2001/01/19 22:25:08  paul
 * Update copyright.
 *
 * Revision 1.3  2000/03/08 20:09:17  josh
 * Adding V3 support to vxWorks port
 *
 * Revision 1.2  2000/03/07 20:58:38  josh
 * putting CVS headers and logs into files that were lacking
 *
 */

#ifndef __INCsnmpdLibh
#define __INCsnmpdLibh

#include "vxWorks.h"
#include <netinet/in.h>      /* for htonl, ntohl etc */

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/snmp.h>
#include <wrn/wm/snmp/engine/snmpdefs.h>


#define MAX_OID_LEN   128    /* Max no of components in an OID */

#define SNMP_ERROR      1    /* error messages */
#define SNMP_WARN       2    /* warning messages */
#define SNMP_INFO       3    /* information messages */

extern  int snmpTraceLevel;         /* Trace level for agent logs */

struct Timer_Block {
unsigned int   when;
void          (*what)();
};

#define S_snmpdLib_VIEW_CREATE_FAILURE    (M_snmpdLib | 1)
#define S_snmpdLib_VIEW_INSTALL_FAILURE   (M_snmpdLib | 2)
#define S_snmpdLib_VIEW_MASK_FAILURE      (M_snmpdLib | 3)
#define S_snmpdLib_VIEW_DEINSTALL_FAILURE (M_snmpdLib | 4)
#define S_snmpdLib_VIEW_LOOKUP_FAILURE    (M_snmpdLib | 5)
#define S_snmpdLib_MIB_ADDITION_FAILURE   (M_snmpdLib | 6)
#define S_snmpdLib_NODE_NOT_FOUND         (M_snmpdLib | 7)
#define S_snmpdLib_INVALID_SNMP_VERSION   (M_snmpdLib | 8)
#define S_snmpdLib_TRAP_CREATE_FAILURE    (M_snmpdLib | 9)
#define S_snmpdLib_TRAP_BIND_FAILURE      (M_snmpdLib | 10)
#define S_snmpdLib_TRAP_ENCODE_FAILURE    (M_snmpdLib | 11)
#define S_snmpdLib_INVALID_OID_SYNTAX     (M_snmpdLib | 12)

extern MIBNODE_T *p_mib_root_node;
extern MIBNODE_T mib_root_node;

/* function declarations */
 
#if defined(__STDC__) || defined(__cplusplus)

extern void     snmpdPktProcess (int, char *, void *, int, void *);
extern void     snmpdLog (int, char *);
extern STATUS   snmpdTreeAdd (char *, MIBNODE_T *);
extern void     snmpdTreeRemove (char *);
extern STATUS   snmpdViewEntrySet (OIDC_T *, int, UINT_16_T, uchar_t *, int, 
                                  int);
extern void     snmpdViewEntryRemove (OIDC_T *, int, UINT_16_T);
extern STATUS   snmpdView2275EntrySet (OIDC_T *, int, uchar_t *, ALENGTH_T,
				       uchar_t *, int, int);
extern void     snmpdView2275EntryRemove (OIDC_T *, int, uchar_t *, ALENGTH_T);

extern int      snmpOidToIpHostOrder (int, OIDC_T *, u_int *);

extern void     snmpdTrapSend (void *, int, void **, void *, int, char *,
                              OIDC_T *, int, u_int *, int, int, int, int (*)(SNMP_PKT_T *, void *), 
                              void *); 
extern STATUS   snmpNextIndex (int, const OIDC_T *, OIDC_T * , const OIDC_T *, int); 
extern void     snmpdGroupByGetprocAndInstance (SNMP_PKT_T *, VB_T *, int, OIDC_T *);
extern void     snmpdContinue (SNMP_PKT_T *);
extern void     snmpNextError (SNMP_PKT_T *, VB_T *);
extern void     snmpVbPrivFree (VB_T *);
extern int      snmpOidStrToArray (char *, OIDC_T *);
extern void     snmpdInitFinish (void (*)(SNMP_PKT_T  *), int (*)(SNMP_PKT_T  *), int (*)(SNMP_PKT_T  *), int (*)(SNMP_PKT_T  *),
			    int (*)(SNMP_PKT_T  *));
extern void     snmpdSetUserHook (void (*)(SNMP_PKT_T  *),
                int (*)(SNMP_PKT_T  *), int (*)(SNMP_PKT_T  *),
                int (*)(SNMP_PKT_T  *), int (*)(SNMP_PKT_T  *));
extern STATUS   snmpdPktLockGet (SNMP_PKT_T *);
extern void     initStartSnmpd (void); 
extern void     snmpdDyncfgInit (void);
extern void     snmpdInitInstalledComponentsValue (void);
extern void     snmpdShutdown (void);

#else   /* __STDC__ */

extern void     snmpdPktProcess ();
extern void     snmpdLog ();
extern STATUS   snmpdTreeAdd ();
extern void     snmpdTreeRemove ();
extern int      snmpdViewEntrySet (); 
extern void     snmpdViewEntryRemove (); 
extern int      snmpdView2275EntrySet (); 
extern void     snmpdView2275EntryRemove (); 
extern int      snmpOidToIpHostOrder ();
extern void     snmpdTrapSend ();
extern int      snmpNextIndex ();
extern void     groupVarBinds (); 
extern void     snmpNextError ();
extern void     snmpVbPrivFree ();
extern int      snmpOidStrToArray ();
extern void     snmpdInitFinish ();
extern void     snmpdSetUserHook ();
extern STATUS   snmpdPktLockGet ();
extern void     snmpdGroupByGetprocAndInstance ();
extern void     snmpdContinue ();
extern void     initStartSnmpd (); 
extern void     snmpdDyncfgInit ();
extern void     snmpdInitInstalledComponentsValue ();
extern void     snmpdShutdown ();



#endif  /* __STDC__ */


#ifdef __cplusplus
}
#endif


#endif /* __INCsnmpdLibh */



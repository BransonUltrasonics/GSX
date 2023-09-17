/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1994-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: vxprotos.h,v $
 * Revision 1.5  2002/03/19 19:32:01  josh
 * add a prototype for function to dynamically add AgentX
 * MIB to MIB tree
 *
 * Revision 1.4  2002/01/04 22:31:55  tneale
 * Added prototype for ax_sub_dyncfg_init
 *
 * Revision 1.3  2001/12/20 21:41:08  meister
 * Add some more prototypes
 *
 * Revision 1.2  2001/11/29 18:31:40  meister
 * added prototype for _nvhard_read_v3proxy
 *
 * Revision 1.1.1.1  2001/11/05 17:47:25  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.2  2001/10/16 19:53:33  meister
 * Add initStartSnmpd()
 *
 * Revision 1.1.2.1  2001/10/16 19:25:35  meister
 * added this file as a place for prototypes for functions specific to
 * the vxWorks snmp agent
 *
 */

/* [clearcase]
modification history
-------------------
01b,21apr05,AJS  Diffie-Hellman work
01a,19apr05,job  update copyright notices
01a,13apr05,AJS  Diffie-Hellman work
*/

#ifndef WMSNMP_VXPROTO_H
# define WMSNMP_VXPROTO_H 1

/*
 * vxprotos.h - this file contains prototypes for functions in the vxWorks 
 *              snmp agent.
 */

#include <wrn/wm/snmp/engine/entlogic.h>

#if (INSTALL_SNMP_VXWORKS_VIRTUAL_STACK)
extern STATUS usrVsSnmpUtilStart(void);
extern STATUS vsSnmpUtilVSRemove(int vsnum);

#if (INSTALL_ENVOY_SNMP_VERSION_3)
extern int _nvhard_vs_add(int vsnum);
extern int _nvhard_vs_remove(int vsnum);
extern void _vs_v3engine_set(ENT_LOGICAL_T *);
#endif /* INSTALL_ENVOY_SNMP_VERSION_3 */

#endif /* INSTALL_SNMP_VXWORKS_VIRTUAL_STACK */

extern int _nvhard_read_v3myid          (char *eng_id);
extern int _nvhard_read_v3users         (void);
extern int _nvhard_read_v3groups        (void);
extern int _nvhard_read_v3access        (void);
extern int _nvhard_read_v3contexts      (void);
extern int _nvhard_read_v3taddr         (void);
extern int _nvhard_read_v3tparam        (void);
extern int _nvhard_read_v3notify        (void);
extern int _nvhard_read_v3nprof         (void);
extern int _nvhard_read_v3nfilt         (void);
extern int _nvhard_read_v3proxy         (void);
extern int _nvhard_read_community       (void);
extern int _nvhard_read_usmDHparams     (void);

extern void v3_add_v3_mibs              (void);
extern void v3_user_dyncfg_init         (void);
extern void ax_add_ax_mibs              (void);
extern void ax_master_dyncfg_init       (void);
extern void ax_sub_dyncfg_init          (void);
extern void snmpCleanAgentXMaster       (void);
extern void snmpCleanAgentXSubagent     (void);

extern void axSockClose                 (void);

extern void initStartSnmpd              (void);

extern void snmpCleanUserTable          (void);
extern void snmpCleanSecurityToGroupTable (void);
extern void snmpCleanAccessTable        (void);
extern void snmpCleanContextTable       (void);
extern void snmpCleanV3Tree             (void);
extern void snmpCleanTargetTables       (void);
extern void snmpCleanNotifyTables       (void);
extern void snmpCleanCommunityTable     (void);
extern void snmpCleanProxyTable         (void);
extern int  snmpDHrand                  (void);

#endif /* WMSNMP_VXPROTO_H */

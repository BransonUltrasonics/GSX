/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
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
 * $Log: vxcmpnts.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:25  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.3  2001/11/02 23:37:02  meister
 * prototype for usrSnmpInit
 *
 * Revision 1.1.2.2  2001/10/01 18:52:56  meister
 * Add some prototypes.
 *
 * Revision 1.1.2.1  2001/08/29 15:47:08  meister
 * declarations for vxworks snmpd component initialation functions
 * and task priorities
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#ifndef _ENVOY_VXCMPNTS_H
# define _ENVOY_VXCMPNTS_H 1

/*
 * vxcmpnts.h - this file contains labels for each configurable component
 *              for the SNMPD running under VxWorks. Each one is a bit in 
 *              in a bit-mask
 */

#define SNMPD_CORE_COMPONENT             0x1
#define SNMPD_VERSION_3_COMPONENT        (0x1 << 1)
#define SNMPD_AGENTX_MASTER_COMPONENT    (0x1 << 2)
#define SNMPD_AGENTX_SUBAGENT_COMPONENT  (0x1 << 3)

/*
 * delcarations for component init routines
 */

void initAxMasterComponent (void);
void initAxSubagentComponent (void);
void initSnmpV3Component (void);
void snmpdInitConfigMsgQ (void);
void initSnmpd(void);

void determineSnmpdComponents (void);
void envoy_dyncfg_init (void);

/*
 * priorities for the component startup tasks, if not specified by the user
 */

#ifndef ENVOY_VXWORKS_SNMPD_INIT_PRIORITY     
# define ENVOY_VXWORKS_SNMPD_INIT_PRIORITY             94
#endif

#ifndef ENVOY_VXWORKS_SNMPD_COMP_INIT_PRIORITY
# define ENVOY_VXWORKS_SNMPD_COMP_INIT_PRIORITY        95
#endif

#ifndef ENVOY_VXWORKS_SNMPD_PRIORITY          
# define ENVOY_VXWORKS_SNMPD_PRIORITY                  96
#endif

#endif /* _ENVOY_VXCMPNTS_H */

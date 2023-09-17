/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/vxagent/axCfg.h,v 1.9 2002/06/13 16:03:10 tneale Exp $ */

/*
 * Copyright (C) 1999-2010 Wind River Systems, Inc.
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
modification history
--------------------
01e,10jun10,y_t  update for LP64
01d,19apr05,job  update copyright notices
01c,25nov03,ps  copyright update
01b,05dec2000,ten   Added prototypes for subagent routines.
01a,03oct2000,josh  written.
*/

#ifndef __INCaxCfgh
#define __INCaxCfgh

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h>
#include <sockLib.h>
#include <ioLib.h>
#include <wrn/wm/snmp/engine/snmpdefs.h>
#include <wrn/wm/snmp/engine/buffer.h>
#include <wrn/wm/snmp/engine/agentx.h>
#include <inetLib.h>

#if INSTALL_ENVOY_AGENTX_MASTER

    /* defines */

    #if ((CPU_FAMILY==I960) && (defined __GNUC__))
    #pragma align 1                 /* tell gcc960 not to optimize  alignments */
    #endif  /* CPU_FAMILY==I960 */

    typedef struct AX_CONN_S
        {
        int               sockNum;      /* Socket being used */
        int               refCount;     /* Number of handlers touching this block */
        ENVOY_AX_CHUNK_T  chunk;        /* AgentX chunk for collecting a PDU */
        bits32_t          connectID;    /* Connection ID */
        bits32_t          createTime;   /* Socket creation time */
        struct AX_CONN_S *next;         /* Next block */
        } AX_CONN_T;

    #if ((CPU_FAMILY==I960) && (defined __GNUC__))
    #pragma align 0                 /* turn off alignment requirement */
    #endif  /* CPU_FAMILY==I960 */

    /* Externs */

    extern STATUS    axMasterIoInit (void);
    extern void      snmpAxMonitorSpawn (void);
    extern void      masterAxCleanup (void);
    extern bits32_t  envoyGetSysUpTime (EBUFFER_T *);

/* 
 * Use the following statement to turn off AgentX master code in the
 * VxWorks agent when the SNMP library was built with AgentX master
 * installed.  This should only be used when dynamic configuration
 * (using the Tornado project facility) is disabled.  To turn off
 * AgentX master support set the following to "0"
 */
    #define INSTALL_AXMASTER 1

#else

    #define INSTALL_AXMASTER 0

#endif


#if INSTALL_ENVOY_AGENTX_SUB

    /* defines */

    /* This is the address the subagent will use to communicate with an AgentX master */
    /* The default address is equivalent to INADDR_ANY and can be used when master */
    /* and subagent are are on the same target */
    #define WMSNMP_AXMAST_IP_ADDRESS	"0.0.0.0"

    /* This is the OID subtree the subagent will register at boot-up */
    /* Remember to terminate the component list with a zero.  This zero is not */
    /* included in the component count */
    #define WMSNMP_AXSUB_REGOID_LIST {1, 3, 6, 1, 4, 1, 0}
    #define WMSNMP_AXSUB_REGOID_LEN 6

    /* Externs */

    extern int    envoy_ax_sa_mib_root_node (ptr_t cookie, SNMP_PKT_T *pktp);
    extern STATUS axSubagentIoInit ( void );
    extern void   snmpAxSubagentSpawn (void);
    extern void   subagentAxCleanup (void);
	extern void   snmpCleanAgentXSubagent(void);

/* 
 * Use the following statement to turn off AgentX subagent code in the
 * VxWorks agent when the SNMP library was built with AgentX subagent
 * installed.  This should only be used when dynamic configuration
 * (using the Tornado project facility) is disabled.  To turn off
 * AgentX subagent support set the following to "0"
 */
    #define INSTALL_AXSUB 1

#else

    #define INSTALL_AXSUB 0

#endif

#ifdef __cplusplus
}
#endif

#endif  /* __INCipcLibh */

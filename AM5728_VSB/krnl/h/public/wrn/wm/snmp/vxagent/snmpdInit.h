/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/vxagent/snmpdInit.h,v 1.1.1.1 2001/11/05 17:47:27 tneale Exp $ */

/* snmpInit.h - VxWorks SNMP Agent system initialization interface */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/*
modification history
--------------------
01g,19apr05,job  update copyright notices
01f,25nov03,ps  copyright update
01e,30apr98,ann Added pragma aligns for I960
01d,11nov96,jag Cleaned up file header. Changed loglevel to traceLevel in
		snmpdInit().
01c,12aug96,rjc removed memory partition options from snmpdInit ();
01b,10jun96,rjc removed extra { #ifdef'd under _cplusplus
01a,01may96,rjc Added copyright header
*/

/*
 * $Log: snmpdInit.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:27  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:25:09  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/09 17:42:28  josh
 * setting up prototypes and includes where appropriate to eliminate
 * build warnings
 *
 * Revision 1.3  2000/03/07 21:17:35  josh
 * fixing log comments
 *
 * Revision 1.2  2000/03/07 20:58:39  josh
 * putting CVS headers and logs into files that were lacking
 *
 */

#ifndef __INCsnmpdInith
#define __INCsnmpdInith

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "vxWorks.h"

#if ((CPU_FAMILY==I960) && (defined __GNUC__))
#pragma align 1                 /* tell gcc960 not to optimize  alignments */
#endif  /* CPU_FAMILY==I960 */

/* module structure */ 

typedef struct mib_module
    {
    FUNCPTR	mibInitialize;		/* initialization routine */
    FUNCPTR	mibTerminate;		/* termination routine */
    } MIB_MODULE;

#if ((CPU_FAMILY==I960) && (defined __GNUC__))
#pragma align 0                 /* turn off alignment requirement */
#endif  /* CPU_FAMILY==I960 */

/* function declarations */
 
 #if defined(__STDC__) || defined(__cplusplus)

extern STATUS  snmpdInit (MIB_MODULE * pModules, int traceLevel, int prio, 
                          int stackSize);
extern void    snmpdExit (void);
extern void    generateTrap (int ifTrapType, int interfaceIndex, 
                              void * pM2TrapRtnArg);


#else   /* __STDC__ */

extern STATUS   snmpdInit ();
extern void     snmpdExit ();
extern void     generateTrap ();

#endif  /* __STDC__ */


#ifdef __cplusplus
}
#endif


#endif /* __INCsnmpdInith */


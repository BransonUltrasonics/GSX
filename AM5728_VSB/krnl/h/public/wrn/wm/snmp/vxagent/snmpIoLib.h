/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/vxagent/snmpIoLib.h,v 1.1.1.1 2001/11/05 17:47:27 tneale Exp $ */

/* snmpdLib.h - VxWorks SNMP Agent */

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
01f,19apr05,job  update copyright notices
01e,25nov03,ps  copyright update
01d,30apr98,ann Changed copyright
01c,11nov96,jag Cleaned up file header.
01b,01may96,rjc Added copyright header
01a,10mar96,rjc written. 
*/


/*
 * $Log: snmpIoLib.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:27  tneale
 * Tornado shuffle
 *
 * Revision 1.5  2001/01/19 22:25:08  paul
 * Update copyright.
 *
 * Revision 1.4  2000/03/09 17:42:28  josh
 * setting up prototypes and includes where appropriate to eliminate
 * build warnings
 *
 * Revision 1.3  2000/03/07 21:17:34  josh
 * fixing log comments
 *
 * Revision 1.2  2000/03/07 20:58:38  josh
 * putting CVS headers and logs into files that were lacking
 *
 *
 */

#ifndef __INCsnmpIoh
#define __INCsnmpIoh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <vxWorks.h>



/* function declarations */
 
#if defined(__STDC__) || defined(__cplusplus)

extern STATUS        snmpIoInit (void);
extern void          snmpIoMain (void);
extern void          snmpIoClose (void);
extern void          snmpIoWrite (void *, char *, int, void *, void *);
void                 snmpIoTrapSend (int, int);
void                 snmpIoViewInit (void);

#else   /* __STDC__ */

extern STATUS        snmpIoInit ();
extern void          snmpIoMain ();
extern void          snmpIoClose ();
extern void          snmpIoWrite ();
void                 snmpIoTrapSend ();
void                 snmpIoViewInit ();
#endif  /* __STDC__ */

#ifdef __cplusplus
}
#endif


#endif /* __INCsnmpIoh */


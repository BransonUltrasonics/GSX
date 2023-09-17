/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/vxagent/tae_sa.h,v 1.2 2001/11/06 21:35:16 josh Exp $ */
/* tae_sa.h  - default IO routines for the SNMP AgentX subagent */

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
-------------------------
01c,19apr05,job  update copyright notices
01b,25nov03,ps  copyright update
01a,08Jan2001,tneale Created for TAE subagent

*/

/*
 * $Log: tae_sa.h,v $
 * Revision 1.2  2001/11/06 21:35:16  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:27  tneale
 * Tornado shuffle
 *
 * Revision 1.2  2001/04/16 19:38:45  josh
 * merging the kingfisher branch onto the trunk
 *
 * Revision 1.1.2.2  2001/03/12 22:11:45  tneale
 * Updated copyright
 *
 * Revision 1.1.2.1  2001/02/09 22:51:05  tneale
 * Support for non-kernel device management in TAE using AgentX
 *
 *
 */

#ifndef __INCtae_sah
#define __INCtae_sah

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <wrn/wm/snmp/engine/snmpdefs.h>
#include <wrn/wm/snmp/engine/asn1conf.h>


#if defined(__STDC__) || defined(__cplusplus)

extern void usrTaeSaInit (void);

#else /* __STDC__ */

extern void usrTaeSaInit ();

#endif


#ifdef __cpluplus
}
#endif


#endif /* __INCtae_sah */


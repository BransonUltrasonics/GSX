/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/wkobj.h,v 1.1.1.1 2001/11/05 17:47:25 tneale Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: wkobj.h,v $
 * Revision 1.1.1.1  2001/11/05 17:47:25  tneale
 * Tornado shuffle
 *
 * Revision 9.2  2001/01/19 22:22:14  paul
 * Update copyright.
 *
 * Revision 9.1  2000/03/17 19:48:49  meister
 * Update copyright notice
 *
 * Revision 9.0  1998/10/16 22:10:32  sar
 * Update version stamp to match release
 *
 * Revision 8.1  1998/02/25 04:50:58  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:34  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:28  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.0  1995/05/31  21:46:47  sra
 * Release 6.0.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 1.2  1993/05/26  21:30:55  sar
 * updated to work with the objid defined in the rfcs.
 *
 * Revision 1.1  1993/04/23  23:36:14  sar
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
01a,19apr05,job  update copyright notices
*/


#if (!defined(wkobj_inc))
#define wkobj_inc

#define SNMP_INTERNET		1, 3, 6, 1

#define SNMP_DIRECTORY		SNMP_INTERNET, 1
#define SNMP_MGMT		SNMP_INTERNET, 2
#define SNMP_MIB2		SNMP_MGMT, 1

#define SNMP_EXPERIMENTAL	SNMP_INTERNET, 3
#define SNMP_PRIVATE		SNMP_INTERNET, 4
#define SNMP_ENTERPRISES	SNMP_PRIVATE, 1

#define SNMP_SECURITY		SNMP_INTERNET, 5
#define SNMP_SNMPV2		SNMP_INTERNET, 6

/*#define SNMP_SNMPMAPPINGS	SNMP_SNMPV2, 1*/
/*#define SNMP_SNMPMODULES	SNMP_SNMPV2, 4*/

/*#define SNMP_SNMPDOMAINS	SNMP_SNMPMAPPINGS, 1*/
#define SNMP_SNMPDOMAINS	SNMP_SNMPV2, 1
#define SNMP_UDPDOMAIN		SNMP_SNMPDOMAINS,  1
#define SNMP_CLNSDOMAIN		SNMP_SNMPDOMAINS,  2
#define SNMP_CONSDOMAIN		SNMP_SNMPDOMAINS,  3
#define SNMP_DDPDOMAIN		SNMP_SNMPDOMAINS,  4
#define SNMP_IPXDOMAIN		SNMP_SNMPDOMAINS,  5

/*#define SNMP_SNMPPROXYS		SNMP_SNMPMAPPINGS, 2*/
#define SNMP_SNMPPROXYS		SNMP_SNMPV2, 2
#define SNMP_RFC1157PROXY	SNMP_SNMPPROXYS,   1
#define SNMP_RFC1157DOMAIN	SNMP_RFC1157PROXY, 1
#define SNMP_RFC1157NOAUTH	SNMP_RFC1157PROXY, 2

#define SNMP_SNMPMODULES	SNMP_SNMPV2, 3
#define SNMP_SNMPMIB		SNMP_SNMPMODULES, 1
#define SNMP_SNMPMIBOBJECTS	SNMP_SNMPMIB,	  1

#define SNMP_SNMPTRAP		SNMP_SNMPMIBOBJECTS, 4
#define SNMP_SNMPTRAPOID	SNMP_SNMPTRAP,       1
#define SNMP_SNMPTRAPENTERPRISE	SNMP_SNMPTRAP,       3

#define SNMP_SNMPTRAPS		SNMP_SNMPMIBOBJECTS, 5
#define SNMP_COLDSTART		SNMP_SNMPTRAPS,      1
#define SNMP_WARMSTART		SNMP_SNMPTRAPS,      2
#define SNMP_LINKDOWN		SNMP_SNMPTRAPS,      3
#define SNMP_LINKUP		SNMP_SNMPTRAPS,      4
#define SNMP_AUTHENTICATIONFAILURE   SNMP_SNMPTRAPS, 5
#define SNMP_EGPNEIGHBORLOSS	SNMP_SNMPTRAPS,      6

#define SNMP_PARTYMIB		SNMP_SNMPMODULES, 3
#define SNMP_PARTYADMIN		SNMP_PARTYMIB,	  1

#define SNMP_PARTYPROTOCOLS	SNMP_PARTYADMIN,  1
#define SNMP_NOAUTH		SNMP_PARTYPROTOCOLS, 1
#define SNMP_NOPRIV		SNMP_PARTYPROTOCOLS, 2
#define SNMP_DESPRIV		SNMP_PARTYPROTOCOLS, 3
#define SNMP_MD5AUTH		SNMP_PARTYPROTOCOLS, 4

#define SNMP_TEMPORALDOMAINS	SNMP_PARTYADMIN, 2
#define SNMP_CURRENTTIME	SNMP_TEMPORALDOMAINS, 1
#define SNMP_RESTARTTIME	SNMP_TEMPORALDOMAINS, 2
#define SNMP_CACHETIME		SNMP_TEMPORALDOMAINS, 3

#define SNMP_INITIALPARTYID	SNMP_PARTYADMIN, 3
#define SNMP_INITIALCONTEXTID	SNMP_PARTYADMIN, 4

#endif /* wkobj_inc */

/* $Header: /usr/cvsroot/target/h/wrn/wm/snmp/engine/snmpstat.h,v 1.2 2001/11/06 21:35:04 josh Exp $ */

/*
 * Copyright (C) 1999-2012 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1991-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* 
 * Portions of this file are subject to additional licensing terms found in
 * the COPYING-SNMP.txt file that is included with this distribution.
 */

/*
 * $Log: snmpstat.h,v $
 * Revision 1.2  2001/11/06 21:35:04  josh
 * new revised path structure, first pass.  Also removing useless
 * shell include files.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:23  tneale
 * Tornado shuffle
 *
 * Revision 9.5  2001/01/19 22:22:10  paul
 * Update copyright.
 *
 * Revision 9.4  2000/03/17 19:48:41  meister
 * Update copyright notice
 *
 * Revision 9.3  2000/03/09 20:58:34  tneale
 * All structure definitions are now bracketted with a test that
 * potentially sets an alignment pragma.  This is to maintain compatibility
 * with the code base that had been distributed by Wind River.
 *
 * Revision 9.2  2000/03/09 16:58:33  tneale
 *  Added #ifdef for C++ to declare extern C if needed
 *
 * Revision 9.1  1999/04/09 20:37:02  josh
 * Obsoleting ENVOY_SNMP_VERSON_USEC and ENVOY_40_VB_FREE_COMPAT
 *
 * Revision 9.0  1998/10/16 22:10:20  sar
 * Update version stamp to match release
 *
 * Revision 8.3  1998/06/08 19:42:11  sar
 * Create a define to tell us if we need the stats group in general
 *
 * Revision 8.2  1998/05/23 20:06:17  sar
 * Added some stats variables for SNMPv3
 *
 * Revision 8.1  1998/02/25 04:50:48  sra
 * Update copyrights.
 *
 * Revision 8.0  1997/11/18 00:56:28  sar
 * Updated revision to 8.0
 *
 * Revision 7.2  1997/03/20 06:48:19  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 7.1  1997/02/25 10:49:26  sra
 * Update copyright notice, dust under the bed.
 *
 * Revision 7.0  1996/03/18 20:17:33  sar
 * Updated rev to 7.0 and copyright to 96
 *
 * Revision 6.3  1996/01/10  16:23:25  sar
 * USEC updates
 *
 * Revision 6.2  1995/11/14  22:42:26  sar
 * renamed proxydrops & silentdrops to match the mib definitions.
 *
 * Revision 6.1  1995/10/20  22:53:35  sar
 * moved some stats into and out of the ifdef blocks
 * removed the v2 classic stats
 * added some v2 community stats
 *
 * Revision 6.0  1995/05/31  21:46:53  sra
 * Release 6.0.
 *
 * Revision 5.1  1995/03/20  23:11:33  sar
 * Moved the deinition of the statistics macros here so there is one
 * central point for them.
 *
 * Revision 5.0  1994/05/16  15:49:04  sar
 * Updated revision to 5.0 and copyright to include 1994
 *
 * Revision 4.0  1993/06/24  16:05:41  sar
 * Updated rev to 4.0 and coypright to 93
 *
 * Revision 3.5  1993/06/02  23:05:55  dab
 * Changed #ifdef's to #if's for things from install.h
 *
 * Revision 3.4  1993/05/03  17:54:14  sar
 * Modified #include <foo> to be #include <envoy/h/foo.h> where it was correct.
 *
 * Revision 3.3  1993/04/22  20:14:13  sar
 * Updating of macros and defines to use new naming scheme and get the
 * names to be more useful in general.  (use INSTALL_ENVOY_SNMP_VERSION_1
 * for example).
 *
 * Revision 3.2  1993/02/18  20:43:26  sar
 * Added DestPartyMismatches and UnknownContexts to the v2 snmpstat group.
 *
 * Revision 3.1  1993/02/17  20:55:10  sar
 * Updated stats to deal with v1 and v2 options.
 *
 * Revision 3.0  92/04/03  19:52:27  dab
 * Release 3.0
 * 
 * Revision 1.1  91/10/30  20:45:25  dab
 * Initial revision
 * 
 */

/* [clearcase]
modification history
-------------------
01c,20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support.
01b,15jun10,y_t  update for LP64
01a,19apr05,job  update copyright notices
*/


#if (!defined(snmpstat_inc))
#define snmpstat_inc

#ifdef __cplusplus
extern"C" {
#endif

#if (!defined(asn1conf_inc))
#include <wrn/wm/snmp/engine/asn1conf.h>
#endif

#if (!defined(snmpdefs_inc))
#include <wrn/wm/snmp/engine/snmpdefs.h>
#endif

#if (!defined(snmp_inc))
#include <wrn/wm/snmp/engine/snmp.h>
#endif

#if (ENVOY_NEED_STATS)

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 1
#endif

typedef	struct	SNMP_STATS_S
	{
	unsigned int	snmpInPkts;
	unsigned int	snmpInBadVersions;
	unsigned int	snmpInBadCommunityNames;
	unsigned int	snmpInBadCommunityUses;
	unsigned int	snmpInASNParseErrs;
	unsigned short	snmpEnableAuthTraps;
#if INSTALL_ENVOY_SNMP_GROUP_V1
	unsigned int	snmpOutPkts;
	unsigned int	snmpInBadTypes;
	unsigned int	snmpInTooBigs;
	unsigned int	snmpInNoSuchNames;
	unsigned int	snmpInBadValues;
	unsigned int	snmpInReadOnlys;
	unsigned int	snmpInGenErrs;
	unsigned int	snmpInTotalReqVars;
	unsigned int	snmpInTotalSetVars;
	unsigned int	snmpInGetRequests;
	unsigned int	snmpInGetNexts;
	unsigned int	snmpInSetRequests;
	unsigned int	snmpInGetResponses;
	unsigned int	snmpInTraps;
	unsigned int	snmpOutTooBigs;
	unsigned int	snmpOutNoSuchNames;
	unsigned int	snmpOutBadValues;
	unsigned int	snmpOutReadOnlys;
	unsigned int	snmpOutGenErrs;
	unsigned int	snmpOutGetRequests;
	unsigned int	snmpOutGetNexts;
	unsigned int	snmpOutSetRequests;
	unsigned int	snmpOutGetResponses;
	unsigned int	snmpOutTraps;
#endif /* INSTALL_ENVOY_SNMP_GROUP_V1 */
#if INSTALL_ENVOY_SNMP_GROUP_V2
	unsigned int	snmpSilentDrops;
	unsigned int	snmpProxyDrops;
#endif /* INSTALL_ENVOY_SNMP_GROUP_V2 */
#if INSTALL_ENVOY_SNMP_VERSION_3
	unsigned int    snmpUnknownSecurityModels;
	unsigned int    snmpInvalidMsgs;
	unsigned int    snmpUnknownPDUHandlers;
	unsigned int    snmpUnavailableContexts;
	unsigned int    snmpUnknownContexts;
	unsigned int    usmStatsUnsupportedSecLevels;
	unsigned int    usmStatsNotInTimeWindows;
	unsigned int    usmStatsUnknownUserNames;
	unsigned int    usmStatsUnknownEngineIDs;
	unsigned int    usmStatsWrongDigests;
	unsigned int    usmStatsDecryptionErrors;

	unsigned int    snmpTsmUsePrefix;

	unsigned int    tsmStatsInvalidCaches;
	unsigned int    tsmStatsInadequateSecurityLevels;
	unsigned int    tsmStatsUnknownPrefixes;
	unsigned int    tsmStatsInvalidPrefixes;

	unsigned int    tlsStatsSessionOpens;
	unsigned int    tlsStatsSessionClientCloses;
	unsigned int    tlsStatsSessionOpenErrors;
	unsigned int    tlsStatsSessionAccepts;
	unsigned int    tlsStatsSessionServerCloses;
	unsigned int    tlsStatsSessionNoSessions;
	unsigned int    tlsStatsSessionInvalidClientCertficates;
	unsigned int    tlsStatsSessionUnknownServerCertficate;
	unsigned int    tlsStatsSessionInvalidServerCertficates;
	unsigned int	tlsStatsSessionInvalidCaches;

	unsigned int	sshStatsSessionOpens;
	unsigned int	sshStatsSessionCloses;
	unsigned int    sshStatsSessionOpenErrors;
	unsigned int	sshStatsSessionUserAuthFailures;
	unsigned int	sshStatsSessionNoChannels;
	unsigned int	sshStatsSessionNoSubsystems;
	unsigned int	sshStatsSessionNoSessions;
	unsigned int	sshStatsSessionInvalidCaches;
#endif /* INSTALL_ENVOY_SNMP_VERSION_3 */
	} SNMP_STATS_T;

#ifdef WINDNET_STRUCT_ALIGN
#pragma align 0
#endif


extern	SNMP_STATS_T	snmp_stats;

#endif	/* ENVOY_NEED_STATS */


/* Some macros to make the mainline code a little cleaner to read */
#if (INSTALL_ENVOY_SNMP_GROUP_V1)
#define SGRPv1_INC_COUNTER(x) inc_counter(x)
#define SGRPv1_ADD_COUNTER(x, y) add_counter(x, y)
#else
#define SGRPv1_INC_COUNTER(x)
#define SGRPv1_ADD_COUNTER(x, y)
#endif

#if (INSTALL_ENVOY_SNMP_GROUP_V2)
#define SGRPv2_INC_COUNTER(x) inc_counter(x)
#else
#define SGRPv2_INC_COUNTER(x)
#endif

#if (INSTALL_ENVOY_SNMP_GROUP_V1 || INSTALL_ENVOY_SNMP_GROUP_V2)
#define SGRPv1v2_INC_COUNTER(x) inc_counter(x)
#else
#define SGRPv1v2_INC_COUNTER(x)
#endif

#if (INSTALL_ENVOY_SNMP_VERSION_3)
#define SGRPv3_INC_COUNTER(x) inc_counter(x)
#else
#define SGRPv3_INC_COUNTER(x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* snmpstat_inc */


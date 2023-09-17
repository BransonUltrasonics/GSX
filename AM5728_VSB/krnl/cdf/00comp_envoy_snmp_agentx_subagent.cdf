/* 00comp_envoy_snmp_agentx_subagent.cdf - Component configuration file */


/*
 *  Copyright 2001-2004 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/* 
 * $Log: 00comp_envoy_snmp_agentx_subagent.cdf,v $
 * Revision 1.3  2009/09/15 07:21:45  ytang
 * snmp cdf files
 *
 * Revision 1.2  2002/01/07 22:35:48  meister
 * add PROTOTYPE fields for t2 components
 *
 * Revision 1.1.1.1  2001/11/05 17:47:54  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.2  2001/10/31 18:56:19  meister
 * Fold common library into snmo core.
 * Fold des library into V3 core.
 *
 * Revision 1.1.2.5  2001/10/24 20:17:38  meister
 * core init routines now split off from user init routines
 *
 * Revision 1.1.2.4  2001/10/23 17:17:26  meister
 * There is a limitation in cdf files that disallows modules appearing
 * in more than one CDF. this has forced a rework of the agentx cdfs, into
 * 3 component modules.
 *
 * Revision 1.1.2.3  2001/10/18 19:59:52  meister
 * ENVOY --> WINDMANAGE
 *
 * Revision 1.1.2.2  2001/10/12 20:33:57  meister
 * shuffling around some modules into user components instead of system
 * components
 *
 * Revision 1.1.2.1  2001/09/20 17:18:16  meister
 * component description files for vxworks snmpd components (tae)
 *
 */

/*
modification history
--------------------
01b,02dec03,job  update copyright information
01a,09sep2001,meister written
*/

/*
DESCRIPTION
  This file contains descriptions for the envoy AgentX subagent component

*/

/*
 * as a quickie, just export all global symbls as entry points for now.
 * FIXME Fix this later.
 */

Component INCLUDE_WINDMANAGE_SNMP_AGENTX_SUBAGENTCORE
	{
	NAME		Wind River SNMP AgentX Subagent Core
	SYNOPSIS	Wind River SNMP AgentX Subagent Core
	MODULES 	ax_sa.o dyncfgas.o
	INIT_RTN	ax_sub_dyncfg_init ();
	PROTOTYPE	void ax_sub_dyncfg_init (void);
        REQUIRES	INCLUDE_WINDMANAGE_SNMP_AGENTX_COMMONCORE
	}


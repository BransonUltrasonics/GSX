/* 00comp_envoy_snmp_agentx_subagent.cdf - Component configuration file */

/*
 *  Copyright 1984-2004 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/* 
 * $Log: 00comp_envoy_snmp_agentx_common.cdf,v $
 * Revision 1.3  2009/09/15 07:21:45  ytang
 * snmp cdf files
 *
 * Revision 1.2  2002/01/07 22:35:48  meister
 * add PROTOTYPE fields for t2 components
 *
 * Revision 1.1.1.1  2001/11/05 17:47:54  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.1  2001/11/02 20:30:41  meister
 * agentx component split into 3 parts
 *
 */

/*
modification history
--------------------
01b,02dec03,job  update copyright information
01a,09sep01,meister written
*/

/*
DESCRIPTION
  This file contains descriptions for the snmp AgentX component

*/

/*
 * as a quickie, just export all global symbls as entry points for now.
 * FIXME Fix this later.
 */

Component INCLUDE_WINDMANAGE_SNMP_AGENTX_COMMONCORE
	{
	NAME		Wind River SNMP AgentX Common Core 
	SYNOPSIS	Wind River SNMP AgentX Common Core 
	MODULES 	ax_saini.o ax_sa_cr.o ax_chunk.o ax_core.o dyncfgax.o
	INIT_RTN	ax_dyncfg_init();
	PROTOTYPE	void ax_dyncfg_init(void);
	}


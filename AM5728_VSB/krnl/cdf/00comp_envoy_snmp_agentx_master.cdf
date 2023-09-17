/* 00comp_envoy_snmp_lib1.cdf - Component configuration file */


/*
 *  Copyright 1984-2004 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/* 
 * $Log: 00comp_envoy_snmp_agentx_master.cdf,v $
 * Revision 1.3  2009/09/15 07:21:45  ytang
 * snmp cdf files
 *
 * Revision 1.4  2002/06/21 20:08:39  tneale
 * Fixed empty "REQUIRES" line
 *
 * Revision 1.3  2002/03/18 21:06:48  josh
 * updating cdf files so that they require the correct objects --
 * ax_mth.o is obsolete, axmth.o and axapi.o are new.
 *
 * Revision 1.2  2002/01/07 22:35:48  meister
 * add PROTOTYPE fields for t2 components
 *
 * Revision 1.1.1.1  2001/11/05 17:47:54  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.2  2001/10/31 18:56:18  meister
 * Fold common library into snmo core.
 * Fold des library into V3 core.
 *
 */

/*
modification history
--------------------
01a,01dec03,job  update copyright information
*/

/*
DESCRIPTION
  This file contains descriptions for the envoy AgentX subagent.

*/

/*
 * as a quickie, just export all global symbls as entry points for now.
 * FIXME Fix this later.
 */

Component INCLUDE_WINDMANAGE_SNMP_AGENTX_MASTERCORE
	{
	NAME		Wind River SNMP AgentX Master Agent Core 
	SYNOPSIS	Wind River SNMP AgentX Master Agent Core 
	MODULES 	ax_maini.o ax_index.o ax_ma.o ax_mhlpf.o \
			ax_mmutl.o axmth.o axapi.o dyncfgam.o
        INIT_RTN	ax_master_dyncfg_init ();
        PROTOTYPE	void ax_master_dyncfg_init (void);
        REQUIRES        INCLUDE_WINDMANAGE_SNMP_AGENTX_COMMONCORE 
	}

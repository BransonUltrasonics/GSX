/*
 *  Copyright 1984-2004, 2006-2007 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/*
 * $Log: 00folder_windmanage_snmp.cdf,v $
 * Revision 1.4  2010/09/21 11:09:56  ulf
 * Rearrangement of networking components in .cdf files as part of REQ:WIND00141200 CVS_NOTIFIER:YES
 *
 * Revision 1.3  2009-09-15 07:21:45  ytang
 * snmp cdf files
 *
 * Revision 1.1  2001/12/06 23:38:36  meister
 * this file makes the patching of 00network.cdf somewhat easier
 *
 */

/*
modification history
--------------------
01b,16aug07,mze  change name of FOLDER
01a,02dec03,job  update copyright information
*/

Folder FOLDER_SNMPD
	{
	NAME		Wind River SNMP libraries
	SYNOPSIS	Simple network management protocol daemon
	CHILDREN	INCLUDE_WINDMANAGE_SNMP_CORE \
			INCLUDE_WINDMANAGE_SNMP_V3CORE \
			INCLUDE_WINDMANAGE_SNMP_AGENTX_COMMONCORE \
			INCLUDE_WINDMANAGE_SNMP_AGENTX_MASTERCORE \
			INCLUDE_WINDMANAGE_SNMP_AGENTX_SUBAGENTCORE
	DEFAULTS	INCLUDE_WINDMANAGE_SNMP_CORE \
			INCLUDE_WINDMANAGE_SNMP_V3CORE \
			INCLUDE_WINDMANAGE_SNMP_AGENTX_MASTERCORE \
			INCLUDE_WINDMANAGE_SNMP_AGENTX_COMMONCORE \
			INCLUDE_WINDMANAGE_SNMP_AGENTX_SUBAGENTCORE 
	}

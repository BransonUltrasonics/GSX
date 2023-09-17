/* 00comp_envoy_snmp_core.cdf - Component configuration file */

/*
 *  Copyright 1984-2007, 2014, 2016 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/*
 * $Log: 00comp_envoy_snmp_core.cdf,v $
 * Revision 1.3  2009/09/15 07:21:45  ytang
 * snmp cdf files
 *
 * Revision 1.1  2002/06/17 16:16:08  meister
 * versions of these files for Tornado 2.2, which does not have an
 * INCLUDE_ANSI_STDLIB_ATOL component
 *
 */

/* 
modification history 
--------------------
18sep16,r_w  add parameter SNMP_DEBUG_ENABLE (V7MAN-198)
13jul14,bbz  vip build error when SNMP component is added(V7MAN-107)
13may14,bbz  add configuration in vip for SNMP community name(V7MAN-3)
08jul08,y_t  Removed REQUIRES INCLUDE_IPNET_PACKET_POOL_3
06may08,zhr  Added SNMP_USE_M2LIB for m2Lib migration to ipnet  
16aug06,mze  reduce MODULES to wrsnmp_config.o from the previous list
07jul06,msa  Add RFC4293 support and remove obsolete files.
02dec03,job  update copyright information
*/

/* 
 * Old revision history, from similar file in ../t2_cdf:
 * Revision 1.5  2002/06/04 17:54:56  josh
 * none of the vxWorks builds require COMMON_STRICMP or
 * COMMON_STRNICMP to be installed
 *
 * Revision 1.4  2002/01/07 22:35:48  meister
 * add PROTOTYPE fields for t2 components
 *
 * Revision 1.3  2001/12/04 16:27:08  meister
 * remove epilogue subagent from CDF
 *
 * Revision 1.2  2001/11/13 16:47:20  meister
 * Get rid of obsolete cdf's that came back in the shuffle.
 *
 * Revision 1.1.1.1  2001/11/05 17:47:54  tneale
 * Tornado shuffle
 *
 * Revision 1.1.2.2  2001/10/31 18:56:19  meister
 * Fold common library into snmo core.
 * Fold des library into V3 core.
 *
 * Revision 1.1.2.5  2001/10/23 17:19:37  meister
 * there is a limitation that modules can only appear in a single CDF file.
 * this forces the SNMP core to be slpit into two component modules.
 *
 * Revision 1.1.2.4  2001/10/18 19:59:52  meister
 * ENVOY --> WINDMANAGE
 *
 * Revision 1.1.2.3  2001/09/27 01:04:00  meister
 * More rearrangement ; splitting out coexistence/proxy
 *
 * Revision 1.1.2.2  2001/09/20 19:43:56  meister
 * Fix typo
 *
 * Revision 1.1.2.1  2001/09/20 17:18:18  meister
 * component description files for vxworks snmpd components (tae)
 *
 */

/*

DESCRIPTION
  This file contains descriptions for the envoy AgentX subagent.

*/

/*
 * as a quickie, just export all global symbls as entry points for now.
 * FIXME Fix this later. 
 */

Component INCLUDE_WINDMANAGE_SNMP_CORE {
	NAME		Wind River SNMP Core Library
	SYNOPSIS	Wind River SNMP Core Library
	REQUIRES    INCLUDE_SOCKLIB INCLUDE_IPNET
    CONFIGLETTES    usrSnmp.c
	INIT_RTN	usrSnmpCfgInit();
/*	MODULES 	wrsnmp_config.o */
	PROTOTYPE	void initSnmpd(void);
    CFG_PARAMS  SNMP_USE_M2LIB \
            SNMP_DEBUG_ENABLE  \
            SNMP_GET_COMMUNITY_1 \
            SNMP_GET_COMMUNITY_2 \
            SNMP_GET_COMMUNITY_3 \
            SNMP_GET_COMMUNITY_4 \
            SNMP_GET_COMMUNITY_5 \
            SNMP_SET_COMMUNITY_1 \
            SNMP_SET_COMMUNITY_2
}

Parameter SNMP_USE_M2LIB {
        NAME            SNMP uses m2Lib 
        SYNOPSIS        static rfc1213 mib2 supported by m2Lib is used 
        TYPE            BOOL 
        DEFAULT         FALSE 
}

Parameter SNMP_DEBUG_ENABLE {
        NAME        enable debugging
        SYNOPSIS    Set the value with 1 to debug the task
        TYPE        int
        DEFAULT     0
}

Parameter SNMP_GET_COMMUNITY_1 {
        NAME            SNMP get community string 1
        SYNOPSIS        SNMP get community string 1
        TYPE            string
        DEFAULT         "public"
}

Parameter SNMP_GET_COMMUNITY_2 {
        NAME            SNMP get community string 2
        SYNOPSIS        SNMP get community string 2
        TYPE            string
        DEFAULT         "pub"
}
Parameter SNMP_GET_COMMUNITY_3 {
        NAME            SNMP get community string 3
        SYNOPSIS        SNMP get community string 3
        TYPE            string
        DEFAULT         "private"
}
Parameter SNMP_GET_COMMUNITY_4 {
        NAME            SNMP get community string 4
        SYNOPSIS        SNMP get community string 4
        TYPE            string
        DEFAULT         "priv"
}
Parameter SNMP_GET_COMMUNITY_5 {
        NAME            SNMP get community string 5
        SYNOPSIS        SNMP get community string 5
        TYPE            string
        DEFAULT         "icmp"
}
Parameter SNMP_SET_COMMUNITY_1 {
        NAME            SNMP set community string 1
        SYNOPSIS        SNMP set community string 1
        TYPE            string
        DEFAULT         "private"
}
Parameter SNMP_SET_COMMUNITY_2 {
        NAME            SNMP set community string 2
        SYNOPSIS        SNMP set community string 2
        TYPE            string
        DEFAULT         "priv"
}

 /* MODULES 	asy_util.o \
			buildpkt.o \
			snmpEBuffer.o \
			cmn_init.o \
			snmpBerDecode.o \
			dyncfgvx.o \
			snmpBerEncode.o \
			env_init.o \
			etimerin.o \
			etimer.o \
			localio.o \
			mibutils.o \
			mib_next.o \
			mib_fon.o \
			null_tas.o \
			null_sas.o \
			objectid.o \
			oidcmp2.o \
			row_get.o \
			tree.o \
			vb_rtns.o \
			view_chk.o \
                 	asy_back.o \
			bindint.o \
			bindip.o \
			bindnull.o \
			bindoid.o \
			bindstr.o \
			bindui64.o \
			binduint.o \
			creatint.o \
			creatrap.o \
			creatreq.o \
			etimerin.o \
			exist_as.o \
			exists.o \
			gas_incr.o \
			gas_int.o \
			gas_ip.o \
			gas_oid.o \
			gas_str.o \
			gas_ui64.o \
			gas_uint.o \
			gas_ulng.o \
			get_int.o \
			get_ip.o \
			get_oid.o \
			get_str.o \
			get_ui64.o \
			get_uint.o \
			get_ulng.o \
			getpdu.o \
			ip2llist.o \
			ip2rlist.o \
			llistcmp.o \
			localio.o \
			nextpdu.o \
			null_gas.o \
			null_get.o \
			null_nas.o \
			null_nxt.o \
			null_set.o \
			null_tst.o \
			oid2ip.o \
			oid2str.o \
			oidcmp.o \
			oidorder.o \
			rcv_pkt.o \
			row.o \
			rowloose.o \
			rpktasy.o \
			sas_incr.o \
			sas_int.o \
			sas_ui64.o \
			sas_uint.o \
			scan_loc.o \
			serial.o \
			set_int.o \
			set_ui64.o \
			set_uint.o \
			setpdu.o \
			snmp_d.o \
			snmp_e.o \
			view_rts.o \
			vmth2275.o \
			weakprng.o \
			fixed64.o \
			icmp4293_mib.o \
			icmp4293_mth.o \
			if_mib.o \
			if_mth.o \
			ip4293_mib.o \
			ip4293_mth.o \
			iprt_mth.o \
			tcp_mib.o \
			tcp_mth.o \
			udp_mib.o \
			udp_mth.o \
			wrsnmp_config.o
                        
*/

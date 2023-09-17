/*
 *  Copyright 1984-2006 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/* 
 * $Log: 00comp_envoy_snmp_v3.cdf,v $
 * Revision 1.5  2010/09/09 08:55:24  ulf
 * Update of the networking stack to MNT technology. This includes an extensive redesign of the stack internally, but does not affect socket applications. REQ:WIND00195382,WIND00195386 CVS_NOTIFIER:YES:QUICK
 *
 * Revision 1.4.4.1  2010-09-01 13:13:33  ulf
 * Pre-merge of mnt to HEAD. CVS_NOTIFIER:NO
 *
 * Revision 1.3.34.1  2010-06-14 09:06:35  jhorteli
 * CVS_NOTIFIER:YES Definitions, merge of random and more forwarder declarations
 *
 * Revision 1.4  2010-06-08 06:07:47  ytang
 * fix for WIND00215980.CVS_NOTIFIER
 *
 * Revision 1.3  2009-09-15 07:21:45  ytang
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
01b,08jun10,y_t  fix for WIND00215980
01a,02dec03,job  update copyright information
*/

/*
 * as a quickie, just export all global symbls as entry points for now.
 * FIXME Fix this later.
 */

Component INCLUDE_WINDMANAGE_SNMP_V3CORE
        {
        NAME        Wind River SNMP V3 core support component
        SYNOPSIS    This component provides SNMPv3 functionality in the SNMP engine. \
                    The component includes support for SNMPv3 proxy transfers, VACM, and \
                    SNMPv3 database operations conforming to SNMPv3 standards.
        MODULES     copyvb.o \
                    ntfy_chk.o \
                    sendntfy.o \
                    v3_acc.o \
                    v3_auth.o \
                    v3_ber.o \
                    v3_con.o \
                    v3_creq.o \
                    v3_eng.o \
                    v3_ini.o \
                    v3_ntfy.o \
                    v3_priv.o \
                    v3_trgt.o \
                    v3_user.o \
                    v3mt_acc.o \
                    v3mt_con.o \
                    v3mt_nfy.o \
                    v3mt_tgt.o \
                    v3mt_usr.o \
                    commlkup.o \
                    pxy3_chk.o \
                    v3_comm.o \
                    v3mt_pxy.o \
                    v3_proxy.o \
                    v3mt_com.o
        REQUIRES    INCLUDE_WINDMANAGE_SNMP_CORE
        }


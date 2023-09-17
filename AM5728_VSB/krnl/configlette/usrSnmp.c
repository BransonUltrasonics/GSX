/* usrSnmp.c - SNMP agent initialization */

/*
 *  Copyright 2003-2012, 2014, 2016 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/* 
 * Portions of this file are subject to additional licensing terms found in
 * the COPYING-SNMP.txt file that is included with this distribution.
 */

/*
modification history
--------------------
18sep16,r_w  add parameter SNMP_DEBUG_ENABLE (V7MAN-198)
13may14,bbz  add configuration in vip for SNMP community name(V7MAN-3)
22oct12,rjq  implemented SNMP manager over DTLS SSH
20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support.
08jul10,y_t  fix for WIND00208622
25jun08,zhr  clean up  
22jun08,zhr  Fixed wrsnmp_configure prototype issue  
08jun08,zhr  updated for m2lib dynmaically loading 
08mar08,zhr  updated for m2lib migration
20sep07,jbl  fix an unprototyped function call
05sep06,jbl  add config command for NOR method routines
28jun05,job  need some locking controls around socket creation
19apr05,job  update copyright notices
01apr05,job  add function prototypes
02dec03,job  update copyright information
15aug03,ps  add agentX init routines
11aug03,ten  Clean up: function was returning OK, should be void
05aug03,ggg  Created
*/

/*
DESCRIPTION

SNMP agent initialization code
*/

#include <semLib.h>
#include "wrn/wm/snmp/engine/snmpdefs.h"

extern void initSnmpV3Component(void);
extern void initAxMasterComponent(void);
extern void initAxSubagentComponent(void);
#ifdef INCLUDE_SNMP_UDP_TRANSPORT
extern void snmp_udp_init(void);
#endif
#ifdef INCLUDE_SNMP_TLS_TRANSPORT
extern void snmp_tls_init(void);
#endif
#ifdef INCLUDE_SNMP_DTLS_TRANSPORT
extern void snmp_dtls_init(void);
#endif
#ifdef INCLUDE_SNMP_SSH_TRANSPORT
extern void snmp_ssh_init(void);
#endif
extern void initStartSnmpd(void);
extern int wrsnmp_configure(int);
extern void initSnmpd(void);

#if defined(INCLUDE_SNMP_MANAGER)
extern void snmptalk_init(void); 
#endif
SEM_ID snmpdSocketControlSem;
static int  usrSnmpInit_common_flag = 0;
int g_snmp_debug_enable = 0;


extern CMNTY_TO_VIEW_T  getCmntyViewTbl [];
extern CMNTY_TO_VIEW_T  setCmntyViewTbl [];

#if defined(INCLUDE_SNMP_TLS_TRANSPORT) || defined(INCLUDE_SNMP_DTLS_TRANSPORT)
extern char *snmp_tls_key_file;
extern char *snmp_tls_certificate_file;
extern char *snmp_tls_CA_certifcate_file;
extern char *snmp_tls_key_id;
extern char *snmp_tls_cert_secname;
extern int   snmp_tlstm_init_certTable(void);
#endif

/*****************************************************************************
* usrSnmpInit_common - initializes the SNMP common config
*
* This function initializes the SNMP common config
*
* RETURNS: 
*/
void usrSnmpInit_common()
{
    if(usrSnmpInit_common_flag == 1)
        return;
    /* register snmp transport models */
#ifdef INCLUDE_SNMP_UDP_TRANSPORT
    snmp_udp_init();
#endif

#ifdef INCLUDE_SNMP_TLS_TRANSPORT
    snmp_tls_init();
#endif

#ifdef INCLUDE_SNMP_DTLS_TRANSPORT
    snmp_dtls_init();
#endif

#ifdef INCLUDE_SNMP_SSH_TRANSPORT
    snmp_ssh_init();
#endif
    usrSnmpInit_common_flag = 1;
}



/*****************************************************************************
* usrSnmpInit - initializes the SNMP agent
*
* This function initializes the SNMP agent
*
* RETURNS: ERROR if initialization problems occur
* ERRNO: N/A
*/
void usrSnmpCfgInit() 
    {

    snmpdSocketControlSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);        
                                          
    initSnmpd();
	int i=0;

#ifdef INCLUDE_WINDMANAGE_SNMP_V3CORE
    initSnmpV3Component();
#endif

#ifdef INCLUDE_WINDMANAGE_SNMP_AGENTX_MASTERCORE
    initAxMasterComponent();
#endif

#ifdef INCLUDE_WINDMANAGE_SNMP_AGENTX_SUBAGENTCORE
    initAxSubagentComponent();
#endif

g_snmp_debug_enable = SNMP_DEBUG_ENABLE;

getCmntyViewTbl[0].Cmnty = SNMP_GET_COMMUNITY_1;
getCmntyViewTbl[1].Cmnty = SNMP_GET_COMMUNITY_2;
getCmntyViewTbl[2].Cmnty = SNMP_GET_COMMUNITY_3;
getCmntyViewTbl[3].Cmnty = SNMP_GET_COMMUNITY_4;
getCmntyViewTbl[4].Cmnty = SNMP_GET_COMMUNITY_5;
setCmntyViewTbl[0].Cmnty = SNMP_SET_COMMUNITY_1;
setCmntyViewTbl[1].Cmnty = SNMP_SET_COMMUNITY_2;

#if INSTALL_ENVOY_SNMP_RFC2275_VIEWS
	for (i=0;i<SNMP_GET_COMMUNITY_TABLE_LENGTH;i++) {
		getCmntyViewTbl[i].viewIndex = "two";
		}
	for (i=0;i<SNMP_SET_COMMUNITY_TABLE_LENGTH;i++) {
		setCmntyViewTbl[i].viewIndex = "two";
		}
#else /* If we aren't using 2275 we are using 1445 */
	for (i=0;i<SNMP_GET_COMMUNITY_TABLE_LENGTH;i++) {
		getCmntyViewTbl[i].viewIndex = 2;
		}
	for (i=0;i<SNMP_SET_COMMUNITY_TABLE_LENGTH;i++) {
		setCmntyViewTbl[i].viewIndex = 2;
		}
#endif



#if defined(INCLUDE_SNMP_TLS_TRANSPORT) || defined(INCLUDE_SNMP_DTLS_TRANSPORT)
    snmp_tls_key_file           = SNMP_TLS_KEY_FILE;
    snmp_tls_certificate_file   = SNMP_TLS_CERT_FILE;
    snmp_tls_CA_certifcate_file = SNMP_TLS_CA_CERT_FILE;
#ifdef SNMP_TLS_KEY_ID
    snmp_tls_key_id             = SNMP_TLS_KEY_ID;
#else
    snmp_tls_key_id             = NULL;
#endif
    snmp_tls_cert_secname       = SNMP_TLS_CERT_SECNAME;
    (void)snmp_tlstm_init_certTable();
#endif

    usrSnmpInit_common();

 
   if (SNMP_USE_M2LIB) 
       {
       wrsnmp_configure(1);
       }
    else 
       wrsnmp_configure(0);
    
   initStartSnmpd();

   }
#ifdef INCLUDE_SNMP_MANAGER
/*****************************************************************************
* usrSnmptalkInit - initializes the SNMP manager
*
* This function initializes the SNMP manager
*
* RETURNS: 
*/
void usrSnmptalkInit() 
{
    usrSnmpInit_common();
    snmptalk_init();
}
#endif


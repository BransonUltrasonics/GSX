/* snmp_tlstm_certTable.h - VxWorks SNMP TLSTM header file */

/*
 * Copyright (C) 2012 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/* 
 * Portions of this file are subject to additional licensing terms found in
 * the COPYING-SNMP.txt file that is included with this distribution.
 */

/*
modification history
--------------------
01a,20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support. 
*/

#ifndef _SNMP_TLSTM_CERTTABLE_H
#define _SNMP_TLSTM_CERTTABLE_H

#ifdef __cplusplus
extern          "C" {
#endif

#define SNMPTLSTMCERTTOTSN_FINGERPRINT_MAX_SIZE          255
#define SNMPTLSTMCERTTOTSN_MAPTYPE_MAX_SIZE              128
#define SNMPTLSTMCERTTOTSN_DATA_MAX_SIZE                 1024

/*
 * Typical data structure for a row entry 
 */
typedef struct STRUCT_snmpTlstmCertToTSNEntry {
    /*
     * Index values 
     */
    unsigned int          tlstmCertToTSNID;

    /*
     * Column values 
     */
    char            fingerprint[SNMPTLSTMCERTTOTSN_FINGERPRINT_MAX_SIZE];
    unsigned int    fingerprint_len;
    char            data[SNMPTLSTMCERTTOTSN_DATA_MAX_SIZE];
    unsigned int    data_len;
    int             mapType;
    int             storageType;
    int             rowStatus;
	struct STRUCT_snmpTlstmCertToTSNEntry  *oldentry;   /* will be used in update */
} snmpTlstmCertToTSNEntry;


typedef int (* SnmptlstmCertTableCb) (snmpTlstmCertToTSNEntry *entry, void *user);

int Iterate_snmptlstmCertToTSNTable(SnmptlstmCertTableCb func, void *user);
int snmp_tlstm_add_certTable(snmpTlstmCertToTSNEntry *entry);
int snmp_tlstm_update_certTable(snmpTlstmCertToTSNEntry *entry, snmpTlstmCertToTSNEntry *oldentry);
int snmp_tlstm_remove_certTable(unsigned int tlstmCertToTSNID);

int snmp_tlstm_init_certTable(void);


#ifdef __cplusplus
}
#endif
#endif

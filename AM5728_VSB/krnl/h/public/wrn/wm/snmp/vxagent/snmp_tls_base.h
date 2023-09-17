/* snmp_tls_base.h - VxWorks SNMP TLS transport base header file */

/*
 * Copyright (C) 2012, 2014 Wind River Systems, Inc.
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
30may14,bbz  resolve snmp engine's dependence on snmp agent(V7MAN-82)
22oct12,rjq  implemented SNMP manager over DTLS SSH
20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support. 
*/

#ifndef _SNMP_TLS_BASE_H
#define _SNMP_TLS_BASE_H

#ifdef __cplusplus
extern          "C" {
#endif

/* OpenSSL Includes */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/*
 * data structures
 */
typedef struct snmpTLSBaseData_s {
   int						  flags;
   snmp_addr_pair       	  addr_pair;
   SSL_CTX					 *ssl_context;
   SSL						 *ssl;
   char 					 *securityName;
   char                      *local_cert_file;
   char                      *local_key_file;
   char 					 *their_cert_file;
   char 					 *their_fingerprint;
   char                      *their_hostname;
   char 					 *trust_cert;
   void                      *verify_info;
} snmpTLSBaseData;



/*  macro defines */
#define SNMP_TLSBASE_IS_SERVER         0x0
#define SNMP_TLSBASE_IS_CLIENT         0x01
#define SNMP_TLSBASE_CERT_FP_VERIFIED  0x02

#define SNMP_VERIFIED_FINGERPRINT          0
#define SNMP_NO_FINGERPRINT_AVAILABLE      1
#define SNMP_FAILED_FINGERPRINT_VERIFY     2

#define SNMP_ERR_TLS_NO_CERTIFICATE       -11

/* functions */
int snmp_tlsbase_verify_server_cert(SSL *ssl, snmpTLSBaseData *tlsdata);
int snmp_tlsbase_verify_client_cert(SSL *ssl, snmpTLSBaseData *tlsdata);

snmpTLSBaseData *snmp_tlsbase_allocate_tlsdata(int isserver);
void             snmp_tlsbase_free_tlsdata(snmpTLSBaseData *tlsbase);

int  snmp_tlsbase_wrapup_recv(snmp_tmStateReference *tmStateRef,
                                snmpTLSBaseData *tlsdata,
                                void **opaque, int *olength);

int snmp_tlsbase_config(snmp_transport *t, const char *token, const char *value);
int snmp_tls_get_verify_info_index(void);

SSL_CTX *snmp_sslctx_client_setup(const SSL_METHOD *,  snmpTLSBaseData *tlsbase);
SSL_CTX *snmp_sslctx_server_setup(const SSL_METHOD *);



#ifdef __cplusplus
}
#endif
#endif/*_SNMPTLSBASEDOMAIN_H*/

/* snmp_openssl.h - VxWorks SNMP openssl header file */

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

#ifndef SNMP_OPENSSL_H
#define SNMP_OPENSSL_H

#ifdef __cplusplus
extern          "C" {
#endif

#include <openssl/ssl.h>
#include <openssl/x509.h>


/*************************************************************************
 *
 * snmp_cert structures, defines and function definitions
 *
 *************************************************************************/

/** uses */
#define NS_CERT_IDENTITY       0x0001
#define NS_CERT_REMOTE_PEER    0x0002
#define NS_CERT_RESERVED1      0x0004
#define NS_CERT_CA             0x0008

/** source */
#define NS_CERTKEY_DEFAULT       0x000 /* get default from DS store */
#define NS_CERTKEY_FILE          0x001 /* filename/full path */
#define NS_CERTKEY_FINGERPRINT   0x002 /* public key fingerprint */
#define NS_CERTKEY_CA            0x004 /* trusted CA */
#define NS_CERTKEY_SAN_RFC822    0x008 /* subj alt name: rfc822 */
#define NS_CERTKEY_SAN_DNS       0x010 /* subj alt name: DNS */
#define NS_CERTKEY_SAN_IPADDR    0x020 /* subj alt name: IP address */
#define NS_CERTKEY_COMMON_NAME   0x040 /* common name */
#define NS_CERTKEY_TARGET_PARAM  0x080 /* tlstmParamsTable */
#define NS_CERTKEY_TARGET_ADDR   0x100 /* tlstmAddrTable */
#define NS_CERTKEY_MULTIPLE      0x200 /* try multiple sources */

/** RFC 5246 hash algorithms (Section 7.4.1.4.1) */
#define NS_HASH_NONE        0
#define NS_HASH_MD5         1
#define NS_HASH_SHA1        2
#define NS_HASH_SHA224      3
#define NS_HASH_SHA256      4
#define NS_HASH_SHA384      5
#define NS_HASH_SHA512      6
#define NS_HASH_MAX         NS_HASH_SHA512

/** SNMP-TLS-TM-MIB */
#define SNMPTLSFINGERPRINT_MAX_LEN   255

/*************************************************************************
 *
 *  certificate to Transport Security Name mapping (snmp_cert_map)
 *
 *************************************************************************/

#define TSNM_tlstmCertSpecified                 1
#define TSNM_tlstmCertSANRFC822Name             2
#define TSNM_tlstmCertSANDNSName                3
#define TSNM_tlstmCertSANIpAddress              4
#define TSNM_tlstmCertSANAny                    5
#define TSNM_tlstmCertCommonName                6
#define TSNM_tlstmCert_MAX                      TSNM_tlstmCertCommonName

typedef struct snmp_cert_map_s {
    unsigned int    priority;
    char           *fingerprint;
	unsigned int    fingerprint_len;
    char           *data;
	unsigned int    data_len;
    int             mapType;
	int             storageType;
	int             rowStatus;
	
    int             hashType;
    int             flags;

    X509           *ocert;
	struct snmp_cert_map_s   *next;
} snmp_cert_map;

typedef struct snmp_cert_map_list_s {
	snmp_cert_map   *head;
	snmp_cert_map   *tail;
	int              num;
} snmp_cert_map_list;


/*************************************************************************
 * snmp_cert function definitions
 *************************************************************************/

char *snmp_openssl_cert_get_commonName(X509 *, char **buf, int *len);
void  snmp_openssl_cert_dump_extensions(X509 *ocert);
char *snmp_openssl_cert_get_fingerprint(X509 *ocert, int alg);
int   snmp_openssl_cert_get_hash_type(X509 *ocert);
char *snmp_openssl_extract_secname(snmp_cert_map *cert_map, snmp_cert_map *peer_cert);
void  snmp_openssl_log_error(int rc, SSL *con, const char *location);
void  snmp_openssl_null_checks(SSL *ssl, int *nullAuth, int *nullCipher);

snmp_cert_map_list *snmp_openssl_get_cert_chain(SSL *ssl);
snmp_cert_map *snmp_cert_map_find(snmp_cert_map *map);
snmp_cert_map *snmp_cert_map_alloc(X509 *ocert);
void           snmp_cert_map_free(snmp_cert_map *cert_map);
int            snmp_cert_map_add(snmp_cert_map *map);
int            snmp_cert_map_remove(snmp_cert_map *map);
snmp_cert_map *snmp_cert_map_find(snmp_cert_map *map);

snmp_cert_map_list *snmp_cert_map_list_init(void);
void snmp_cert_map_list_free(snmp_cert_map_list *cert_map_list);


#ifdef __cplusplus
}
#endif
#endif                          /* SNMP_OPENSSL_H */

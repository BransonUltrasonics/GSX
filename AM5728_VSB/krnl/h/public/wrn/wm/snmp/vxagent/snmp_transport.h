/* snmp_transport.h - VxWorks SNMP transport header file */

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
01b,22oct12,rjq  implemented SNMP manager over DTLS SSH
01a,20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support. 
*/

#ifndef _SNMP_TRANSPORT_H
#define _SNMP_TRANSPORT_H


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "vxWorks.h"
#include <sys/socket.h>
#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/snmp.h>
#include <wrn/wm/snmp/engine/snmpdefs.h>
#include <wrn/wm/snmp/vxagent/snmpdLib.h>


/*  Some transport-type constants.  */
#define SNMP_TM_MAX_SECNAME 256


/*  Some transport-type flags.  */
#define     SNMP_TRANSPORT_FLAG_STREAM       0x01
#define     SNMP_TRANSPORT_FLAG_LISTEN       0x02
#define     SNMP_TRANSPORT_FLAG_TUNNELED     0x04
#define     SNMP_TRANSPORT_FLAG_TMSTATE      0x08  /* indicates opaque is a TSM tmStateReference */
#define     SNMP_TRANSPORT_FLAG_OPENED       0x10  /* f_open called or it is a client transport */
#define     SNMP_TRANSPORT_FLAG_ACCEPTED     0x20  /* accepted transport */
#define     SNMP_TRANSPORT_FLAG_DELETE       0x100 /* transport need to be deleted from transport list.
                                                      Now it is used by SNMP over SSH */

/* define SNMPD listenning socket port */
#define SNMP_PORT                         161
#define SNMP_TRAP_PORT                    162

#define SNMP_TLS_TCP_PORT                 10161
#define SNMP_TLS_TCP_TRAP_PORT            10162

#define SNMP_DTLS_UDP_PORT                10161
#define SNMP_DTLS_UDP_TRAP_PORT           10162

#define SNMP_SSH_PORT                     5161
#define SNMP_SSH_TRAP_PORT                5162



/*  The standard SNMP domains.  */

/* Structure which stores transport security model specific parameters */

/* contents documented in draft-ietf-isms-transport-security-model
   Section 3.2 */
/* note: VACM only allows <= 32 so this is overkill till another ACM comes */


typedef union snmp_sockaddr_storage_u {
    struct sockaddr     sa;
    struct sockaddr_in  sin;
#if INSTALL_INETV6
    struct sockaddr_in6 sin6;
#endif
} snmp_sockaddr_storage;

typedef struct snmp_addr_pair_s {
   snmp_sockaddr_storage remote_addr;
   snmp_sockaddr_storage local_addr;
} snmp_addr_pair;


typedef struct snmp_tmStateReference_s {
   OIDC_T transportDomain[MAX_OID_LEN];
   int    transportDomainLen;
   char   securityName[SNMP_TM_MAX_SECNAME];
   size_t securityNameLen;
   int    requestedSecurityLevel;
   int    transportSecurityLevel;
   char   sameSecurity;
   char   sessionID[8];
   
   char   have_addresses;
   snmp_addr_pair  addresses;

   void *otherTransportOpaque; /* XXX: May have mem leak issues */
} snmp_tmStateReference;


typedef struct snmp_transport_s {
    /*  The transport domain object identifier.  */
    const OIDC_T   *domain;
    int             domain_length;  /*  In sub-IDs, not octets.  */

    /*  Local transport address.  */
	snmp_sockaddr_storage  localAddr;

    /*  Remote transport address.  */
	snmp_sockaddr_storage  remoteAddr;

    /*  The actual socket.  */
    int             sock;

    /*  Flags (see #definitions above).  */
    unsigned int    flags;

    /*  Protocol-specific opaque data pointer.  */
    void           *data;
    int             data_length;

	/*  Transport-specific opaque data. Temporary use, it will pass to rp  */
    void           *transport_data;
    int             transport_data_length;

    /*  Maximum size of PDU that can be sent/received by this transport.  */
    size_t          msgMaxSize;

	char           *packet;
    size_t          packet_len;
	size_t          packet_size;

    /*  routine */
    int             (*f_recv)   (struct snmp_transport_s *, void *,
				 int, void **, int *);
    int             (*f_send)   (struct snmp_transport_s *, void *,
				 int, void **, int *);
    int             (*f_close)  (struct snmp_transport_s *);

    /* Optional: opening can occur during creation if more appropriate */
    int             (*f_open)   (struct snmp_transport_s *);

    /*  This callback is only necessary for stream-oriented transports.  */
    int             (*f_accept) (struct snmp_transport_s *);

    /*  Optional callback to support extra configuration token/value pairs */
    /*  return non-zero on error. It will be used by configuring (D)TLS or SSH */
    int            (*f_config)(struct snmp_transport_s *, const char *,
                               const char *);

    /*  Optional callback that is called after the first transport is
        cloned to the second */
    int            (*f_copy)(struct snmp_transport_s *,
                             struct snmp_transport_s *);

    /* allocated host name identifier; used by configuration system
       to load localhost.conf for host-specific configuration */
    unsigned char         *identifier; /* udp:localhost:161 -> "localhost" */

    /* tunneled transports */
    struct snmp_transport_s     *base_transport;
    /* next transport model */
	struct snmp_transport_s     *next;
} snmp_transport;

typedef struct snmp_domain_s {
    const OIDC_T      *name;
    int                name_length;
    const char       **prefix;
    snmp_transport   *(*f_create_transport) (const char *, int, int);    /* first parameter is input address string, 
                                                                            second is port number and the last is session type */
    void             (*f_deinit)(void);      /* This callback routine will be called after SNMPD is shutdown. */
    struct snmp_domain_s *next;
} snmp_domain;


/*  Some utility functions.  */
int     snmp_transport_init(void);
void    snmp_transport_deinit(void);
snmp_transport *snmp_transport_alloc(void);
void            snmp_transport_free(snmp_transport *t); /*  Free an snmp_transport.  */
/*
 * Return an exact (deep) copy of t, or NULL if there is a memory allocation
 * problem (for instance).
 */
snmp_transport *snmp_transport_copy(snmp_transport *t);

/*
 * If the passed oid (in_oid, in_len) corresponds to a supported transport
 * domain, return 1; if not return 0.  If out_oid is not NULL and out_len is
 * not NULL, then the "internal" oid which should be used to identify this
 * domain (e.g. in pdu->tDomain etc.) is written to *out_oid and its length to
 * *out_len.
 */
void            snmp_domain_register(snmp_domain *domain);

void  snmp_domain_deinit(void);
int   snmp_domain_search(OIDC_T *transportDomain, int transportDomainLen, char *prefix, int prefix_len);

void  snmp_create_transport();
void  snmp_add_transport(snmp_transport *t);
void  snmp_delete_transport(snmp_transport *t);
void  snmp_delete_all_transport();
int   snmp_transport_fd_set(fd_set *ready);
int   snmp_transport_packet_process(fd_set *fd);

/* global data */
extern int snmp_transport_selected_fd_update;
extern snmp_domain          *snmp_domain_list;
extern void                 *snmptalk_ssh_transport;

#ifdef __cplusplus
}
#endif
#endif/*_SNMP_TRANSPORT_H*/

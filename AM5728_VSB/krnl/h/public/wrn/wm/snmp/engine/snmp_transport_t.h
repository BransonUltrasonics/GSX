/* snmp_transport_t.h - VxWorks SNMP transport header file */

/*
 * Copyright (C) 2015 Wind River Systems, Inc.
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
04aug15,r_w  created for defect V7MAN-102
*/

#ifndef _SNMP_TRANSPORT_T_H
#define _SNMP_TRANSPORT_T_H


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "vxWorks.h"
#include <sys/socket.h>
#include <netinet/in.h> 
#include <snmpdLib.h>
#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/snmp.h>
#include <wrn/wm/snmp/engine/snmpdefs.h>


#define     SNMP_TRANSPORT_FLAG_OPENED       0x10  /* f_open called or it is a client transport */

/*  Some transport-type constants.  */
#define SNMP_TM_MAX_SECNAME 256

/*  The standard SNMP domains.  */

/* Structure which stores transport security model specific parameters */

/* contents documented in draft-ietf-isms-transport-security-model
   Section 3.2 */
/* note: VACM only allows <= 32 so this is overkill till another ACM comes */


typedef union snmp_sockaddr_storage_u_t {
    struct sockaddr     sa;
    struct sockaddr_in  sin;
#if INSTALL_INETV6
    struct sockaddr_in6 sin6;
#endif
} snmp_sockaddr_storage_t;

typedef struct snmp_addr_pair_s_t {
   snmp_sockaddr_storage_t remote_addr;
   snmp_sockaddr_storage_t local_addr;
} snmp_addr_pair_t;

typedef struct snmp_tmStateReference_s_t {
   OIDC_T transportDomain[MAX_OID_LEN];
   int    transportDomainLen;
   char   securityName[SNMP_TM_MAX_SECNAME];
   size_t securityNameLen;
   int    requestedSecurityLevel;
   int    transportSecurityLevel;
   char   sameSecurity;
   char   sessionID[8];
   
   char   have_addresses;
   snmp_addr_pair_t  addresses;

   void *otherTransportOpaque; /* XXX: May have mem leak issues */
} snmp_tmStateReference_t;

typedef struct snmp_transport_s_t {
    /*  The transport domain object identifier.  */
    const OIDC_T   *domain;
    int             domain_length;  /*  In sub-IDs, not octets.  */

    /*  Local transport address.  */
	snmp_sockaddr_storage_t  localAddr;

    /*  Remote transport address.  */
	snmp_sockaddr_storage_t  remoteAddr;

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
    int             (*f_recv)   (struct snmp_transport_s_t *, void *,
				 int, void **, int *);
    int             (*f_send)   (struct snmp_transport_s_t *, void *,
				 int, void **, int *);
    int             (*f_close)  (struct snmp_transport_s_t *);

    /* Optional: opening can occur during creation if more appropriate */
    int             (*f_open)   (struct snmp_transport_s_t *);

    /*  This callback is only necessary for stream-oriented transports.  */
    int             (*f_accept) (struct snmp_transport_s_t *);

    /*  Optional callback to support extra configuration token/value pairs */
    /*  return non-zero on error. It will be used by configuring (D)TLS or SSH */
    int            (*f_config)(struct snmp_transport_s_t *, const char *,
                               const char *);

    /*  Optional callback that is called after the first transport is
        cloned to the second */
    int            (*f_copy)(struct snmp_transport_s_t *,
                             struct snmp_transport_s_t *);

    /* allocated host name identifier; used by configuration system
       to load localhost.conf for host-specific configuration */
    unsigned char         *identifier; /* udp:localhost:161 -> "localhost" */

    /* tunneled transports */
    struct snmp_transport_s_t     *base_transport;
    /* next transport model */
	struct snmp_transport_s_t     *next;
} snmp_transport_t;


#ifdef __cplusplus
}
#endif
#endif/*_SNMP_TRANSPORT_T_H*/

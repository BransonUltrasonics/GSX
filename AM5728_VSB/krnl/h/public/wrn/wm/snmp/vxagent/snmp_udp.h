/* snmp_udp.h - VxWorks SNMP UDP transport header file */

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

#ifndef _SNMP_UDP_H
#define _SNMP_UDP_H


#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "vxWorks.h"
#include <wrn/wm/snmp/engine/asn1.h>
#include <wrn/wm/snmp/engine/snmp.h>

#if INSTALL_INETV4
snmp_transport *snmp_udp_transport(const char *addr_string, int port, int isserver);
#endif
#if INSTALL_INETV6
snmp_transport *snmp_udpipv6_transport(const char *addr_string, int port, int isserver);
#endif


#ifdef __cplusplus
}
#endif
#endif/*_SNMP_UDP_H*/

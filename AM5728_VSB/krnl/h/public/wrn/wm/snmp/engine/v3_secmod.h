
/* v3_secmod.h - v3_secmod.h routines */

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

/* [clearcase]
modification history
-------------------
01a,20jul12,rjq  implement RFC5590-5593, add TSM secmodel and
                 TLS,DTLS,SSH transport model support.
*/

#ifndef _V3_SECMOD_H
#define _V3_SECMOD_H

#ifdef __cplusplus
extern"C" {
#endif


#define SNMP_TM_SAME_SECURITY_NOT_REQUIRED 0
#define SNMP_TM_USE_SAME_SECURITY		   1



SNMP_PKT_T *SNMP_Decode_V3_USM_Packet(SNMP_PKT_T *    rp,
                            LCL_FILE   *    in_stream,
                            SNMPADDR_T *    pktsrc,
                            SNMPADDR_T *    pktdst,
                            ALENGTH_T       asn1leng,
                            int        *    error_ret);
SNMP_PKT_T *SNMP_Decode_V3_TSM_Packet(SNMP_PKT_T *    rp,
                            LCL_FILE   *    in_stream,
                            SNMPADDR_T *    pktsrc,
                            SNMPADDR_T *    pktdst,
                            ALENGTH_T       asn1leng,
                            int        *    error_ret);
int SNMP_Encode_V3_USM_Packet(SNMP_PKT_T *rp, EBUFFER_T  *ebuffp, bits8_t  *start_ptr, ALENGTH_T start_len);
int SNMP_Encode_V3_TSM_Packet(SNMP_PKT_T *rp, EBUFFER_T  *ebuffp);

#ifdef __cplusplus
}
#endif

#endif /* v3_secmod_inc */

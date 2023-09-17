
/* v3_dh.h - v3_dh.h routines */

/*
 * Copyright (C) 2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/* [clearcase]
modification history
-------------------
01c,04may05,asl  Diffie-Hellman work
01b,02may05,job  fix superfluous definitions
01a,28apr05,job  remove extraneous prototype
*/

#if (!defined(v3_dh_inc))
#define v3_dh_inc

#ifdef __cplusplus
extern"C" {
#endif

#if INSTALL_SNMP_V3_DIFFIE_HELLMAN

#include <openssl/bn.h>

#define DH_MAX_FIELD_LEN    2
#define DH_LONG_FORM        2
#define DH_MIN_PARAMS_LEN   256
#define DH_SEQID            0x30
#define DH_MINKEYCHG        4

#define DH_SEED_PRIME       211
#define DH_MAX_SEED_BYTES   256

#ifndef SNMP_DH_RAND
#define SNMP_DH_RAND()
#endif

#define  DH_PRIMELEN  BN_num_bytes (SNMP_DH_params.prime)

extern BN_CTX *SNMP_DH_bnCtx;
extern SNMP_DH_PARAMS_T SNMP_DH_params;

int SNMP_DH_encodeParams (BIGNUM *prime, int base, int pvl, EBUFFER_T *result);
int SNMP_DH_decodeParams (EBUFFER_T *BUFFER, BIGNUM *prime, int *base, int *pvl);
int SNMP_set_DH_params (int primeLen, bits8_t *prime, int base, int pvl);
int SNMP_get_DH_params (int *primeLen, bits8_t **prime, int *base, int *pvl);
int SNMP_bn2bin (BIGNUM *bignum, bits8_t **binVal);

#endif /* INSTALL_SNMP_V3_DIFFIE_HELLMAN */

#ifdef __cplusplus
}
#endif

#endif /* v3_dh_inc */

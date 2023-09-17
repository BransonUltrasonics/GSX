/*
 * Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
10dec16,chm  eliminate the dependency between INCLUDE_IPFREESCALE
             and INCLUDE_SHELL (F8092)
04mar16,dee  add definition for ipcrypto_times()
25aug14,ljl  Complete openssl layer and cleanup crypto/ssl, US44321
22may14,jsh  created for moving lock/init support from ipcrypto
             to openSSL layer - V7SEC-9
*/

#ifndef IPCRYPTO_UTIL_H
#define IPCRYPTO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Operation timed out and failed. */
#define IPCRYPTO_ERR_TIMEOUT             -1120

/* Operation failed because blocking is illegal. */
#define IPCRYPTO_ERR_WOULDBLOCK          -1121

/* Operation was interrupted */
#define IPCRYPTO_ERR_INTERRUPT           -1122

typedef void *mutex_t;
/* Block type  */
typedef void * Ipcrypto_block;

struct sockaddr;
struct timeval;
struct tms;

int mutex_create(mutex_t *mutex);
int mutex_delete(mutex_t *mutex);
void mutex_lock(mutex_t mutex);
void mutex_unlock(mutex_t mutex);
int ipcrypto_block_create(Ipcrypto_block *block);
int ipcrypto_block_delete(Ipcrypto_block *block);
int ipcrypto_block_wait(Ipcrypto_block block, int milli_seconds);
void ipcrypto_block_post(Ipcrypto_block block);
void ipcrypto_microtime(struct timeval *tv);
clock_t ipcrypto_times(struct tms *buf);

#ifdef __cplusplus
extern "C" {
#endif

#endif /*IPCRYPTO_UTIL_H*/

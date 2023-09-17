/* httpMemP.h -  HTTP server private memory pool header */

/* Copyright (c) 1997-1999, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01e,09sep99,csr  merging for zlib
01d,09jun99,km   merge of Wind Web Server 2.0 code.
01c,07dec98,mb   alignment set to HTTP_MCB_ALIGN.
01b,13may97,mb   osal introduced.
01a,03mar97,ck   written.
*/

#ifndef __INChttpMemPh
#define __INChttpMemPh

/* includes */

#include "httpSem.h"

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define HTTP_MCB_FREE      0x0C0FFEE0
#define HTTP_MCB_OCCUPIED  0xDEADBEEF
#define HTTP_MCB_DELIMITER 0xACE0BACE
#define HTTP_MCB_ALIGN     4  /* align memory control block to 4 byte boundry */

/* typedefs */

typedef struct http_mem_part_mcb
    {
    unsigned long               ulUsage;
    struct http_mem_part_mcb *  pPoolNext;
    unsigned long               ulBlockSize;  /* full size of memory block */
    HTTP_SEM_ID                 semLockBlock; /* currently unused */
    } HTTP_MEM_PART_MCB;

#ifndef DEFINED_HTTP_MEM_PART_DESC
#define DEFINED_HTTP_MEM_PART_DESC

typedef struct http_mem_part_desc
    {
    HTTP_MEM_PART_MCB *         pPoolStart;
    unsigned long               ulPoolSize;
    HTTP_SEM_ID                 semLockPool;
    unsigned long               ulWatermarkFree;
    } HTTP_MEM_PART_DESC;

typedef HTTP_MEM_PART_DESC * MEM_POOL_ID;

#endif /* DEFINED_HTTP_MEM_PART_DESC */


#ifdef __cplusplus
}
#endif

#endif /* __INChttpMemPh */


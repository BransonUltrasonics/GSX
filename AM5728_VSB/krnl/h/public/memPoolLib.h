/* memPoolLib.h -  memory pool management header for RCOS version */

/* Copyright 2004, 2010  Wind River Systems, Inc. */

/*
modification history
--------------------
02b,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
02b.20feb04,jws  added copyright
02a,16may01,jc	 renamed to memPool and ported to RCOS
01c,09sep99,csr  merging for zlib
01c,09jun99,km   merge of Wind Web Server 2.0 code.
01b,15may97,mb   osal introduced.
01a,03mar97,ck   written.
*/

#ifndef __INCmemPoolLibh
#define __INCmemPoolLibh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* typedefs */

#ifndef DEFINED_MEM_POOL_DESC
#define DEFINED_MEM_POOL_DESC

/*
 * Default (opaque) definition of MEM_POOL_ID; include
 * http/private/memPoolLibP.h for the full definition.
 */

typedef struct mem_pool_desc  	MEM_POOL_DESC;
typedef MEM_POOL_DESC *  		MEM_POOL_ID;

#else

#include "memPoolLibP.h"

#endif /* DEFINED_MEM_POOL_DESC */

/* function declarations */

MEM_POOL_ID 	 memPoolNew     (char * pPool,
                                 unsigned long ulPoolSize
#ifdef __ENABLE_SHARED_MEMORY__
                                ,
                                BOOL shared
#endif
                                );

#ifdef __ENABLE_SHARED_MEMORY__
#define memPoolCreate(pPool, size) 	memPoolNew(pPool, size, FALSE)
#else
#define memPoolCreate(pPool, size) 	memPoolNew(pPool, size)
#endif

STATUS      	 memPoolCleanup (MEM_POOL_ID poolId);
STATUS      	 memPoolCheck   (MEM_POOL_ID poolId);
char *           memPoolDelete  (MEM_POOL_ID poolId);
void *           memPoolAlloc   (MEM_POOL_ID poolId, unsigned long ulBytes);
STATUS      	 memPoolFree    (MEM_POOL_ID poolId, void * pBlock);
unsigned long    memPoolAvail   (MEM_POOL_ID poolId, short sMode);
unsigned long    memPoolStringCopy (MEM_POOL_ID pool, char * pString);
long             memPoolUsed    (MEM_POOL_ID poolId);

char *           memPoolDataGet    (MEM_POOL_ID pool, unsigned long offset);
unsigned long    memPoolOffset     (MEM_POOL_ID pool, unsigned long size);

#ifdef __ENABLE_MEMMGR_DEBUG__
sbyte4 memPoolBlockSize (MEM_POOL_ID poolId, void * pBlock);
#endif /* __ENABLE_MEMMGR_DEBUG__ */

#ifdef __ENABLE_SHARED_MEMORY__
BOOL memPoolShared ( MEM_POOL_ID pool);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCmemPoolLibh */



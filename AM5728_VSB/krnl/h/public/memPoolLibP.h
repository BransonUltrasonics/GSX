/* memPoolLibP.h -  private memory pool header */

/* Copyright 1997-1999 Wind River Systems, Inc. */

/*
modification history
--------------------
02b,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
02a,16may01,jc	 renamed to memPool and ported to RCOS
01c,09sep99,csr  merging for zlib
01d,09jun99,km   merge of Wind Web Server 2.0 code.
01c,07dec98,mb   alignment set to HTTP_MCB_ALIGN.
01b,13may97,mb   osal introduced.
01a,03mar97,ck   written.
*/

#ifndef __INCmemPoolLibPh
#define __INCmemPoolLibPh


#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define MCB_FREE      0x0C0FFEE0
#define MCB_OCCUPIED  0xDEADBEEF
#define MCB_DELIMITER 0xACE0BACE
#define MCB_ALIGN     4  /* align memory control block to 4 byte boundry */

/* typedefs */

typedef struct mem_pool_mcb
    {
    unsigned long          		ulUsage;
    struct mem_pool_mcb *  		pPoolNext;
    unsigned long          		ulBlockSize;  /* full size of memory block */
    OS_SEM_T               		semLockBlock; /* currently unused */
    } MEM_POOL_MCB;


typedef struct mem_pool_desc
    {
    MEM_POOL_MCB *  	        pPoolStart;
    unsigned long               ulPoolSize;
    unsigned long               memUsed;
    OS_SEM_T	                semLockPool;
    unsigned long               ulWatermarkFree;
#ifdef __ENABLE_SHARED_MEMORY__
    BOOL                        shared;
#endif
    } MEM_POOL_DESC;

typedef MEM_POOL_DESC * MEM_POOL_ID;



#ifdef __cplusplus
}
#endif

#endif /* __INCmemPoolLibPh */


/* wmLib.h - header file for WindManagerlibrary */

/* Copyright 2000-2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01f,09mar10,m_z  correct 64bit compile error
01e,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01d,04aug03,jws  Eliminate __DISABLE_MEMORY_POOLS__
01c,01aug03,jws  wmError() prototype changed; SPR 88783
01b,01aug01,pns  adjusted.
01a,04mar01,kw   Written.
*/

/*
 * DESCRIPTION:
 *   This file contains the definitions for constants, data structures,
 *   enumerations and function prototypes used to implement RCB2.
 *
 *   This header defines only external structures, defines, and enums that
 *   are required for RCB2 consumers. The producers must also include the
 *   wmbLibP.h header files for internal structures, defines, and enums
 *   statments.
 */

#ifndef __INCwmLibh
#define __INCwmLibh


/* Thread Pool Support */

typedef enum
    {
    WM_THREAD_SUSPENDED = 0,
    WM_THREAD_ACTIVE,
    WM_THREAD_SHUTDOWN
    }
    WM_THREAD_STATUS_T;

typedef struct WM_THREAD_POOL_S
    {
    OS_THREAD_T         threadId;        /* Async thread id                  */
    WM_THREAD_STATUS_T  status;
    VOIDFUNCPTR         threadFunction;
    void                *threadParams;
    }
    WM_THREAD_POOL_T;


/* safety macros */

#define memberAssign(x, y, z)   ((x)->y = z)
#define memberValue(x, y, z)    ((NULL == x) ? z : ((x)->y))
#define memberGet(x, y)         memberValue(x, y, NULL)
#define memberSet(x, y, z)      { memberAssign(x, y, z);}

#define wmAssignInt(src,dest)      *((long *) dest) = *((long *) src)
#define wmAssignPtr(src,dest)      dest = src

/* errors ultimately only OK or ERROR */

/*
#ifdef __WM_DEBUG__
#define wmError                 wmErrorThrow
#else
#define wmError(x,y)            (OK == x ? OK : ERROR)
#endif
*/

/* debugging aids */

#if defined(__WM_DEBUG__) && ! defined(__DEBUG_MESSAGES_OFF__)
#  define DEBUG_MSG_0(msg)          wmLogPrintf(msg)
#  define DEBUG_MSG_1(msg, x)       wmLogPrintf(msg, x)
#  define DEBUG_MSG_2(msg, x, y)    wmLogPrintf(msg, x, y)
#  define DEBUG_MSG_3(msg, x, y, z) wmLogPrintf(msg, x, y, z)
#else
#  define DEBUG_MSG_0(msg)
#  define DEBUG_MSG_1(msg, x)
#  define DEBUG_MSG_2(msg, x, y)
#  define DEBUG_MSG_3(msg, x, y, z)
#endif /* __WM_DEBUG__ ! __DEBUG_MESSAGES_OFF__ */


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __OS_MALLOC_PROVIDED__

#if 0 /* maybe we can! */
/* can't debug memory we don't own... */

#undef __ENABLE_MEMMGR_DEBUG__
#endif

#define wmMalloc    wmosMalloc
#define wmCalloc    wmosCalloc
#define wmFree      wmosFree

#define wmPoolAlloc         memPoolAlloc
#define wmPoolFree          memPoolFree

#else /* __OS_MALLOC_PROVIDED__ */

#ifndef __ENABLE_MEMMGR_DEBUG__

#ifndef _WRS_CONFIG_LP64
void *  wmMalloc (ubyte4 memSize);
void *  wmCalloc (ubyte4 num, Length size);
void    wmFree   (void * pBuffer);
#else
void *  wmMalloc (ubyte8 memSize);
void *  wmCalloc (ubyte8 num, ubyte8 size);
void    wmFree   (void * pBuffer);
#endif

#define wmPoolAlloc         memPoolAlloc
#define wmPoolFree          memPoolFree

#else /* __ENABLE_MEMMGR_DEBUG__ */

#define wmMalloc(x)	        wmMallocDebug       (x,    __FILE__, __LINE__)
#define wmCalloc(x,y)       wmCallocDebug       (x, y, __FILE__, __LINE__)
#define wmFree(x)		    wmFreeDebug         (x,    __FILE__, __LINE__)

#define wmPoolAlloc(x,y)    wmPoolAllocDebug    (x, y, __FILE__, __LINE__)
#define wmPoolFree(x,y)     wmPoolFreeDebug     (x, y, __FILE__, __LINE__)

#ifndef _WRS_CONFIG_LP64
void *  wmMallocDebug   (ubyte4 memSize, sbyte * pFile, int lineNum);
void    wmFreeDebug     (void * pBuffer, sbyte * pFile, int lineNum);
void *  wmCallocDebug   (ubyte4 num, Length size, sbyte * pFile, int lineNum );
void *  wmPoolAllocDebug(MEM_POOL_ID pool, ubyte4 size, sbyte * pFile, int lineNum);
void    wmPoolFreeDebug (MEM_POOL_ID pool, void * pData, sbyte * pFile, int lineNum);
#else
void *  wmMallocDebug   (ubyte8 memSize, sbyte * pFile, long lineNum);
void    wmFreeDebug     (void * pBuffer, sbyte * pFile, long lineNum);
void *  wmCallocDebug   (ubyte8 num, Length size, sbyte * pFile, long lineNum );
void *  wmPoolAllocDebug(MEM_POOL_ID pool, ubyte8 size, sbyte * pFile, long lineNum);
void    wmPoolFreeDebug (MEM_POOL_ID pool, void * pData, sbyte * pFile, long lineNum);
#endif

#endif /* __ENABLE_MEMMGR_DEBUG__ */
#endif /* __OS_MALLOC_PROVIDED__  */


STATUS  wmStart             (void);
STATUS  wmStop              (void);

#ifndef _WRS_CONFIG_LP64
void    wmDumpHex           (sbyte * pMsg, sbyte * pBuf, sbyte4 bufLen);
#else
void    wmDumpHex           (sbyte * pMsg, sbyte * pBuf, sbyte8 bufLen);
#endif
STATUS  wmError             (int error, sbyte * pMsg);
int     wmErrNo             (void);
#ifndef _WRS_CONFIG_LP64
int     wmMax               (int a, int b);
int     wmMin               (int a, int b);
#else
long    wmMax               (long a, long b);
long    wmMin               (long a, long b);
#endif

#ifndef _WRS_CONFIG_LP64
sbyte4  wmMemoryUsed        (void);
#else
sbyte8  wmMemoryUsed        (void);
#endif
void    wmMemoryDebug       (BOOL enable);
BOOL    wmMemoryDebugGet    (void);
void    wmMemoryDebugStdout (BOOL enable);
void    wmMemoryFailEnable  (BOOL enable);
MEM_POOL_ID wmMemoryGet (void);

void    wmThreadPool     (WM_THREAD_POOL_T * pPool);
STATUS  wmThreadStart    (VOIDFUNCPTR threadFunction, void * threadParams);
STATUS  wmThreadPoolStart(void);
STATUS  wmThreadPoolStop (void);

char * wmGetErrMsg (sbyte4);

#ifdef __WM_DEBUG__
void    wmTaskBegin(OS_THREAD_T tid);
void    wmTaskEnd(void);
#else
#define wmTaskBegin(x)
#define wmTaskEnd()
#endif

#define wmThreadPoolInit    wmThreadPoolStart

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCwmbLibh */

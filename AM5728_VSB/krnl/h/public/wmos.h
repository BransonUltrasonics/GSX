/* wmos.h  -  header file for the WindManage OS abstraction layer */

/*
 * Copyright (c) 2004-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,07jan11,m_z correct compiling error on linux
                CQID: Fix WIND00249657
01h,28jul10,m_z  eliminate compile warning
01g,09jul10,m_z  remove compile warning
01f,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01e,24dec08,y_z  delete kWmosThreadCreateOptions' definition, it will
                 defined by MCE
01d,28aug08,q_z  fix for mud tasks spawned with option (WIND00130158)
01c,07mar07,ps   fixes for RTP builds. defect WIND00044864
01b,16feb06,ps   changes for webcli on linux
01a,20feb04,jws  added copyright and mod history
*/

/*
 * DESCRIPTION
 * This header file should be included in all the modules that intend to use
 * the Rapid Control OS abstraction layer.  In order to port a module
 * to any supported OS you need to:
 *    0. #define the appropriate OS flag (i.e. __VXWORKS_OS__)
 *    1. Include this header in your module
 *    2. Replace all the OS system calls with wmos* calls (i.e. wmosSemTake())
 *    3. Link to the corresponding wmos[OS name].c module (i.e. wmosVxWorks.c)
 *    4. Sit back, compile and relax
 *
 */

#ifndef INCwmosh
#define INCwmosh

#ifndef DEFAULT_THREAD_STACK_SIZE_K
#define DEFAULT_THREAD_STACK_SIZE_K     5000
#endif

#ifndef DEFAULT_THREAD_PRIORITY_K
#define DEFAULT_THREAD_PRIORITY_K       101
#endif

#if ! (defined(__VXWORKS_OS__)  || defined(__VXWORKS_AE_OS__))
#define SEM_Q_FIFO     0
#define SEM_Q_PRIORITY 1
#define SEM_EMPTY      0
#define SEM_FULL       1
#endif

#if defined(__CUSTOMER_SPECIFIC_OS__)

/* constants */
#define OS_PIPE_PREFIX_K "/pipe/"

/* typedefs */
typedef int     OS_THREAD_T;
typedef int     OS_SEM_T;
typedef int     OS_MSGQ_T;
typedef int     OS_PIPE_T;
typedef int     OS_TIME_T;
typedef int     SHARED_DATA_T;
typedef void    *OS_ARG_T;


/*__CUSTOMER_SPECIFIC_OS__*/

#elif defined(__POSIX_OS__)

/* #include <sys/time_impl.h> */

#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <stdio.h>

#include "linux_sem_port.h"

#define NO_WAIT_K       (0)

typedef pthread_t       OS_THREAD_T;

/*
typedef int             OS_SEM_T;
*/

#if 0
/* PSERVITA DEBUG */
typedef struct OS_SEM_P
    {
    int  sem;
    BOOL binary;
    }
    OS_SEM_P;
#else
typedef struct linux_sem OS_SEM_P;
#endif

typedef OS_SEM_P *      OS_SEM_T;
typedef OS_SEM_T        OS_MUTEX_T;
typedef int             OS_MSGQ_T;
typedef int             OS_PIPE_T;
typedef time_t          OS_TIME_T;
typedef int             SHARED_DATA_T;
typedef void            *OS_ARG_T;

#define OS_PIPE_PREFIX_K    "/tmp/"
#define WAIT_FOREVER_K      -1

#define taskIdSelf pthread_self
#define ERROR WM_ERROR_CODE


#elif defined(__SOLARIS_OS__)

/*
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE     2
*/

/**/
#include <sys/time_impl.h>
#include <pthread.h>
/**/

#include <sys/types.h>
#include <time.h>

/*
#include <sched.h>
#include <thread.h>
*/
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <stdio.h>

#define NO_WAIT_K       (0)

typedef pthread_t       OS_THREAD_T;

/*
typedef int             OS_SEM_T;
*/

typedef struct OS_SEM_P
    {
    int  sem;
    BOOL binary;
    }
    OS_SEM_P;

typedef OS_SEM_P *      OS_SEM_T;
typedef OS_SEM_T        OS_MUTEX_T;
typedef int             OS_MSGQ_T;
typedef int             OS_PIPE_T;
typedef time_t          OS_TIME_T;
typedef int             SHARED_DATA_T;
typedef void            *OS_ARG_T;

#define OS_PIPE_PREFIX_K    "/tmp/"
#define WAIT_FOREVER_K      -1

#define taskIdSelf pthread_self
#elif defined __BSD_OS__

#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/semaphore.h>
#include <stdio.h>
#include <errno.h>
#include "semLib.h"

typedef pthread_t       OS_THREAD_T;
typedef SEM_ID          OS_SEM_T;
typedef SEM_ID          OS_MUTEX_T;
typedef int             OS_MSGQ_T;
typedef int             OS_PIPE_T;
typedef time_t          OS_TIME_T;
typedef int             SHARED_DATA_T;
typedef int             OS_ARG_T;

#define OS_PIPE_PREFIX_K    "/tmp/"
#define NO_WAIT_K           (0)
#define WAIT_FOREVER_K      (-1)
#define taskIdSelf          pthread_self

#elif defined(__VXWORKS_OS__) || defined(__VXWORKS_AE_OS__)

/* includes where the redefined types live */

#include <taskLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <time.h>
#include <errno.h>

/* constants */

#define OS_PIPE_PREFIX_K    "/pipe/"
#define WAIT_FOREVER_K      WAIT_FOREVER
#define NO_WAIT_K           NO_WAIT

/* typedefs */

typedef TASK_ID         OS_THREAD_T;
typedef SEM_ID          OS_SEM_T;
typedef SEM_ID          OS_MUTEX_T;
typedef MSG_Q_ID        OS_MSGQ_T;
typedef int             OS_PIPE_T;
typedef time_t          OS_TIME_T;
#ifndef _WRS_CONFIG_LP64
typedef int             OS_ARG_T;
#else
typedef long            OS_ARG_T;
#endif
#ifndef _WRS_CONFIG_LP64
typedef unsigned int    OS_CLOCK_T;
#else
typedef unsigned long   OS_CLOCK_T;
#endif
#ifdef __VXWORKS_AE_OS__
#include <sdLib.h>
typedef SD_ID           SHARED_DATA_T;
#else
#ifndef _WRS_CONFIG_LP64
typedef int             SHARED_DATA_T;
#else
typedef long            SHARED_DATA_T;
#endif
#endif

/*__VXWORKS_OS__*/

#elif  defined(__CHORUS_OS__)

#elif defined(__SYSTEMV_OS__)

#elif defined(__WIN32_OS__)

#undef  ERROR  /* Windows uses ERROR = 0, not compatible with RC */

#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>

#undef NO_ERROR /* conflicts with envoy */
#undef MEM_FREE /* windows uses it too */

/* we don't want Windows' ERROR */

#undef  ERROR
#define ERROR   WM_ERROR_CODE

/* constants */

#define OS_PIPE_PREFIX_K    "\\\\.\\pipe\\"
#define WAIT_FOREVER_K      INFINITE
#define NO_WAIT_K           0

/* typedefs */

typedef HANDLE          OS_THREAD_T;
typedef void *          OS_SEM_T;
typedef OS_SEM_T        OS_MUTEX_T;
typedef HANDLE          OS_PIPE_T;
typedef struct _timeb   OS_TIME_T;
typedef HANDLE          SHARED_DATA_T;
typedef void            *OS_ARG_T;

#define taskIdSelf  GetCurrentThreadId


/* Win32 msgQ model doesn't fit us, use our own */

#ifndef __WM_QUEUE_ENABLED__
#define __WM_QUEUE_ENABLED__
#endif

/*__WIN32_OS__ */

#else

#error "You must select an operating system!"

#endif /* End of OS selection */

#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */


#ifdef __WM_QUEUE_ENABLED__
#  ifdef __WM_QUEUE_PRIVATE__
      typedef struct WM_MSGQ_T * OS_MSGQ_T;
#  else
      typedef int OS_MSGQ_T;
#  endif
#  include "wmQueueLib.h"
#endif


/* Typedefs common to all ports */

typedef enum os_sem_opt_tag
    {
    SEM_TYPICAL_K = 0,
    SEM_SHARED_MEM_K
    } OS_SEM_OPTIONS_T;


typedef enum os_file_mode_tag
    {
    OS_PIPE_IN = 1,
    OS_PIPE_OUT,
    OS_PIPE_BOTH
    } OS_PIPE_MODE_T;


typedef struct OS_MEM_SHARE_T
    {
    SHARED_DATA_T   key;    /* key to open shared memory    */
#ifndef _WRS_CONFIG_LP64
    int             size;   /* size of shared memory        */
#else
    long            size;   /* size of shared memory        */
#endif
    void *          pool;   /* memory pool id               */
    void *          pData;  /* pointer to shared memory     */
    } OS_MEM_SHARE_T;


/*------------------ Prototypes ------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

void wmosInit(void);
void wmosFini(void);

/* Semaphores */

STATUS  wmosSemCreate      (OS_SEM_T *pSem, sbyte4 count, OS_SEM_OPTIONS_T opt);
STATUS  wmosSemDestroy     (OS_SEM_T sem);
STATUS  wmosSemTake        (OS_SEM_T sem, ubyte4 timeout);
STATUS  wmosSemMCreate     (OS_SEM_T *pSem,OS_SEM_OPTIONS_T opt);
STATUS  wmosSemBinaryCreate(OS_SEM_T *pSem, OS_SEM_OPTIONS_T opt);
STATUS  wmosSemBinaryTake  (OS_SEM_T sem, ubyte4 timeout);
STATUS  wmosSemGive        (OS_SEM_T sem);
STATUS  wmosSemFlush       (OS_SEM_T sem);

#if ! (defined(__VXWORKS_OS__) || defined(__VXWORKS_AE_OS__) || defined(__SOLARIS_OS__))

OS_SEM_T    semCCreate  (int options, int semCount);
OS_SEM_T    semCSmCreate(int options, int semCount);
OS_SEM_T    semBCreate  (int options, int initial);
STATUS      semFlush    (OS_SEM_T mutex);
STATUS      semTake     (OS_SEM_T mutex, int timeout);
STATUS      semGive     (OS_SEM_T mutex);
STATUS      semDelete   (OS_SEM_T mutex);

void        taskDelay   (int ticks);
STATUS      taskDelete  (OS_THREAD_T tid);
OS_THREAD_T taskSpawn   (char * name, int prio, int options, int stackSize,
                         FUNCPTR pHandlerFcn, OS_ARG_T arg1, OS_ARG_T arg2,
                         OS_ARG_T arg3, OS_ARG_T arg4, OS_ARG_T arg5,
                         OS_ARG_T arg6, OS_ARG_T arg7, OS_ARG_T arg8,
                         OS_ARG_T arg9, OS_ARG_T arg10);
STATUS   taskSuspend    (OS_THREAD_T tid);
STATUS   taskResume     (OS_THREAD_T tid);

#endif

/* Thread Management */

void    wmosYield           (void);
void    wmosTimeSleep       (ubyte4 mSecs);
void    wmosThreadCreateError(void);

STATUS  wmosThreadCreate    (VOIDFUNCPTR pHandlerFcn, sbyte * pTaskName,
                             void * pArg, sbyte4 priority,
                             size_t stackSize, OS_THREAD_T * tid  );

STATUS  wmosThreadKill      (OS_THREAD_T tid);
STATUS  wmosThreadSuspend   (OS_THREAD_T tid);
STATUS  wmosThreadResume    (OS_THREAD_T tid);
STATUS  wmosThreadDataInit  (void);
STATUS  wmosThreadDataGet   (void ** ppData);
STATUS  wmosThreadDataSet   (void ** ppData);
STATUS  wmosThreadDataExit  (void);

/* WARNING:  Pipes are not supported under WIN32!! */

#ifndef _WRS_CONFIG_LP64
STATUS wmosNamedPipeCreate  (sbyte * pName, ubyte4 maxMsgs, ubyte4 msgSize);
#else
STATUS wmosNamedPipeCreate  (sbyte * pName, ubyte4 maxMsgs, ubyte8 msgSize);
#endif
STATUS wmosNamedPipeDelete  (sbyte * pName);
STATUS wmosNamedPipeOpen    (sbyte * pName, OS_PIPE_MODE_T mode, BOOL noBlock,
                             OS_PIPE_T * pipe);
#ifndef _WRS_CONFIG_LP64
STATUS wmosNamedPipeWrite   (OS_PIPE_T pipe, sbyte * buffer, ubyte4 bufSize,
                             ubyte4 * bytesWritten);
#else
STATUS wmosNamedPipeWrite   (OS_PIPE_T pipe, sbyte * buffer, ubyte8 bufSize,
                             ubyte8* bytesWritten);
#endif
#ifndef _WRS_CONFIG_LP64
STATUS wmosNamedPipeRead    (OS_PIPE_T pipe, sbyte * buffer, ubyte4 bufSize,
                             ubyte4 * bytesRead);
#else
STATUS wmosNamedPipeRead    (OS_PIPE_T pipe, sbyte * buffer, ubyte8 bufSize,
                             ubyte8* bytesRead);
#endif
STATUS wmosNamedPipeClose   (OS_PIPE_T pipe);

/* Message Queues */

#ifndef _WRS_CONFIG_LP64
STATUS wmosQueueCreate  (ubyte4 maxMsg, ubyte4 msgSize, sbyte4 flags, OS_MSGQ_T * pQID);
#else
STATUS wmosQueueCreate  (ubyte8 maxMsg, ubyte8 msgSize, sbyte4 flags, OS_MSGQ_T * pQID);
#endif
STATUS wmosQueueDelete  (OS_MSGQ_T qID);
#ifndef _WRS_CONFIG_LP64
STATUS wmosQueueReceive (OS_MSGQ_T qID, sbyte * pBuffer, ubyte4 buffSize,
                         ubyte4 timeout, sbyte4 * pBytesReceived);
#else
STATUS wmosQueueReceive (OS_MSGQ_T qID, sbyte * pBuffer, ubyte8 buffSize,
                         ubyte4 timeout, sbyte8 * pBytesReceived);
#endif
#ifndef _WRS_CONFIG_LP64
STATUS wmosQueueSend    (OS_MSGQ_T qID, sbyte * pMsg, ubyte4 msgSize,
                         ubyte4 timeout, sbyte4 priority);
#else
STATUS wmosQueueSend    (OS_MSGQ_T qID, sbyte * pMsg, ubyte8 msgSize,
                         ubyte4 timeout, sbyte4 priority);
#endif

/* Memory Management */

/*
 * NOTE:  These functions abstract the OS memory management functions.
 * You may want to use your own memory manager or WindManage memory
 * management library instead
 */

#ifndef _WRS_CONFIG_LP64
void *  wmosMalloc          (ubyte4 memSize);
#else
void *  wmosMalloc          (ubyte8 memSize);
#endif
#ifndef _WRS_CONFIG_LP64
void *  wmosCalloc          (ubyte4 elemCount, ubyte4 elemSize);
#else
void *  wmosCalloc          (ubyte8 elemCount, ubyte8 elemSize);
#endif
void    wmosFree            (void * pBuffer);

/* shared memory */

#ifndef _WRS_CONFIG_LP64
void *  wmosCalloc          (ubyte4 elemCount, ubyte4 elemSize);
STATUS wmosSharedMemoryCreate (sbyte * pName, sbyte4 size, OS_MEM_SHARE_T * pMemory);
#else
STATUS wmosSharedMemoryCreate (sbyte * pName, sbyte4 size, OS_MEM_SHARE_T * pMemory);
#endif
STATUS wmosSharedMemoryAttach(OS_MEM_SHARE_T * pMemory);
STATUS wmosSharedMemoryDestroy(OS_MEM_SHARE_T * pMemory);

/* Environment Variables */
#ifndef _WRS_CONFIG_LP64
ubyte4   wmosTimeGet(void);   /* seconds since 12:00 a.m. 1/1/1970 */
#else
ubyte8   wmosTimeGet(void);   /* seconds since 12:00 a.m. 1/1/1970 */
#endif
         
#if defined(__VXWORKS_OS__) || defined(__VXWORKS_AE_OS__)
OS_CLOCK_T wmosClock  (void);   /* time running in ticks or microseconds */
#endif
/* Error Log Interface */
STATUS wmosError(sbyte4 errorLevel, sbyte * pErrorMessage);


#ifdef __cplusplus
}
#endif

#endif /* INCwmosh */

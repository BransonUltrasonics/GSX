/* blkXbd.h - Generic Storage block driver header */

/*
 * Copyright (c) 2012, 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06sep15,hma  add request transfer routine(V7STO-469).
07may14,e_d  add vxbus Gen2 model support. (US29286)
14nov12,sye  Written.
*/

#ifndef __INCblkXbdh
#define __INCblkXbdh

/* includes */

#include <drv/xbd/xbd.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

/* XBD service task name */

#define BLK_XBD_SVC_TASK_NAME           "XbdSvc"
#define BLK_XBD_REQ_SVC_TASK_NAME       "ReqSvc"

#define BLK_XBD_SVC_NAME_LEN            6

/* XBD service task priority */

#define BLK_XBD_SVC_TASK_PRI            50

/* XBD service task stack size */

#define BLK_XBD_SVC_TASK_STACK          8192

#define BLK_XBD_MAX_XFER_BLKS           2048
/* XBD service task delete flag */

#define BLK_XBD_SVC_TASK_DELETE_TIMEOUT 20

typedef struct xbdData
    {
    UINT8 *     buffer;         /* data buffer for transfer */
    UINT32      numBlks;        /* number of blocks to be transfered */
    sector_t    blkNum;         /* start block number for transfer */
    } XBD_DATA;

typedef STATUS (* BLK_READ_FUNC) (void * pDev, XBD_DATA * pData);
typedef STATUS (* BLK_WRITE_FUNC) (void * pDev, XBD_DATA * pData);
typedef STATUS (* BLK_DUMP_FUNC) (void * pDev, XBD_DATA * pData);
typedef int (* BLK_IOCTL_FUNC) (void * pDev, int cmd, void * arg);
typedef BOOL (* BLK_XBD_TEST_FUNC) (void * pDev);

/*  ops define */

typedef struct blkXbdOps
    {
    BLK_READ_FUNC     blkRead;   /* block read */
    BLK_WRITE_FUNC    blkWrite;  /* block write */
    BLK_DUMP_FUNC     blkDump;   /* core dump */
    BLK_IOCTL_FUNC    blkIoctl;  /* block ioctl */
    FUNCPTR           xferReq;            /* xfer request routine */
    } BLK_XBD_OPS;

typedef struct blkXbdDev
    {
    XBD             xbd;                /* xbd device */
    devname_t       devName;            /* name of xbd device */
    TASK_ID         svcTaskId;          /* service task id */
    SEM_ID          bioReady;           /* wake up bio service task */
    SEM_ID          bioMutex;           /* lock bio queue */
    SEM_ID          xbdSemId;           /* used for ERF interaction */
    SEM_ID          reqMutex;           /* lock req list */
    SEM_ID          reqReadySem;        /* request queue counting semaphore */
    struct bio *    bioHead;            /* head of bio queue */
    struct bio *    bioTail;            /* tail of bio queue */
    struct bio *    bioCurr;            /* currently bio pointer */
    BOOL            xbdInserted;        /* inserted or removed */
    BOOL            xbdInstantiated;    /* TRUE if stack init is complete */
    BOOL            directModeFlag;     /* direct BIO mode flag */
    int             xbdSvcTskPri;       /* service task priority */
    BLK_XBD_OPS     xbdOps;             /* storage XBD routines */
    void        *   pDev;               /* pointer to host device struct */
    LIST            reqList;            /* request use xbdMuteSem to protect */
    TASK_ID         reqSevcTask;        /* request service task */
    } BLK_XBD_DEV ;

typedef struct blkXbdParams
    {
    devname_t     devName;              /* xbd device name */
    BLK_XBD_OPS   xbdOps;              /* storage XBD routines */
    sector_t      numBlks;              /* number of blocks for the device */
    UINT32        blkSize;              /* block size of the device */
    int           svcTskPri;            /* bio service task priority */
    BOOL          directModeFlag;       /* direct BIO mode flag */
    void        * pDev;                 /* pointer to host device struct */
    UINT32        maxActiveReqs;        /* max active request */
    UINT32        maxBiosPerReq;        /* max bios per request */
    UINT32        maxXferBlks;          /* max transfer blocks */
    } BLK_XBD_PARAMS;

IMPORT device_t blkXbdDevCreate (BLK_XBD_DEV * pXbd, BLK_XBD_PARAMS * pParams);
IMPORT STATUS blkXbdDevDelete (BLK_XBD_DEV * pXbd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCblkXbdh */

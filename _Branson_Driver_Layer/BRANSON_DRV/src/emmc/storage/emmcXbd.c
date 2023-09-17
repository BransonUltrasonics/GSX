/* emmcXbd.c - Generic Storage XBD block driver */

/*
 * Copyright (c) 2012 - 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26jan16,myt  add support for direct bio mode (V7STO-572)
18nov15,myt  undef bio schedule flag (V7STO-520)
16sep15,j_x  cleanup build warnings
06sep15,hma  add request transfer routine(V7STO-469).
09sep14,m_y  modify to remove compile warning.
07may14,e_d  add vxbus Gen2 model support. (US29286)
29oct13,e_d  fix some prevent issue. (WIND00440964)
22oct13,m_y  modify to remove compile warning.
14nov12,sye  Written.
*/

/*
DESCRIPTION

This is the generic XBD block driver used by storage drivers to interface
with upper level file system.
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <sysLib.h>
#include <ioLib.h>
#include <logLib.h>
#include <semLib.h>
#include <taskLib.h>
#include <errnoLib.h>
#include <drv/erf/erfLib.h>

#include <emmcXbd.h>

/* define */

#ifndef EMMC_XBD_DBG_ON

#   ifdef  LOCAL
#       undef  LOCAL
#       define LOCAL
#   endif /* LOCAL */

#   define EMMC_XBD_DBG_INIT          0x00000001
#   define EMMC_XBD_DBG_RW            0x00000002
#   define EMMC_XBD_DBG_IOCTL         0x00000004
#   define EMMC_XBD_DBG_ALL           0xffffffff
#   define EMMC_XBD_DBG_OFF           0x00000000

UINT32 emmcXbdDbgMask = EMMC_XBD_DBG_OFF;

#   define EMMC_XBD_DBG(mask, string, a, b, c, d, e, f)                   \
        if ((emmcXbdDbgMask & mask) || (mask == EMMC_XBD_DBG_ALL))         \
            {                                                            \
            if (_func_logMsg != NULL)                                    \
                _func_logMsg(string, (_Vx_usr_arg_t)a, (_Vx_usr_arg_t)b, \
                            (_Vx_usr_arg_t)c, (_Vx_usr_arg_t)d,          \
                            (_Vx_usr_arg_t)e, (_Vx_usr_arg_t)f);         \
            }
#else
#   define EMMC_XBD_DBG(mask, string, a, b, c, d, e, f)
#endif  /* blk_XBD_DBG_ON */

/* forward declarations */

LOCAL void emmcXbdExecBio (EMMC_XBD_DEV * pXbd, struct bio * bio);
LOCAL void emmcXbdBioSvcTask (EMMC_XBD_DEV * pXbd);
LOCAL void emmcXbdCreateSyncHandler (UINT16 category, UINT16 type,
                                              void * eventData, void * userData
                                             );
LOCAL int emmcXbdIoctl (struct xbd * self, int cmd, void * arg);
LOCAL int emmcXbdStrategy (struct xbd * self, struct bio *);
LOCAL int emmcXbdDump (struct xbd * self, sector_t pos, void * data,
                            size_t size);
LOCAL int emmcXbdGetBioPrio (EMMC_XBD_DEV * pXbd);
LOCAL struct bio * emmcXbdGetNextBio (EMMC_XBD_DEV * pXbd);

#undef USE_XBD_BIO_SCHED
#ifdef USE_XBD_BIO_SCHED
LOCAL int emmcXbdXferRequest
    (
    XBD *                   pXbd, /* pointer to XBD */
    XBD_REQUEST *           pReq  /* pointer to XBD_REQUEST */
    );
LOCAL VOID emmcXbdReqSevTask
    (
    EMMC_XBD_DEV * pBlkXbd
    );

#endif

/* locals */

LOCAL struct xbd_funcs emmcXbdFuncs =
    {
    emmcXbdIoctl,
    emmcXbdStrategy,
    emmcXbdDump,
    #ifdef USE_XBD_BIO_SCHED
    emmcXbdXferRequest
    #endif
    };

/* imports */

IMPORT int sysClkRateGet(void);

#ifndef _WRS_CONFIG_SMP
IMPORT volatile UINT64 vxAbsTicks;
#else
IMPORT UINT64 vxAbsTicksGet (void);
#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* emmcXbdGetNextBio - get the next bio in the bio chain
*
* This routine gets the next bio in the bio chain.
*
* RETURNS: pointer to the next bio or NULL
* ERRNO: N/A
*/

LOCAL struct bio * emmcXbdGetNextBio
    (
    EMMC_XBD_DEV * pXbd
    )
    {
    struct bio * pBio;

    pXbd->bioCurr = pXbd->bioHead;
    pBio = pXbd->bioHead;

    if (pBio != NULL)
        {
        pXbd->bioHead = pBio->bio_chain;
        pBio->bio_chain = NULL;

        if (pXbd->bioHead == NULL)
            pXbd->bioTail = NULL;
        }
    return (pBio);
    }

/*******************************************************************************
*
* emmcXbdExecBio - execute a bio option
*
* This routine executes a bio option.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void emmcXbdExecBio
    (
    EMMC_XBD_DEV *  pXbd,
    struct bio  *  bio
    )
    {
    XBD_DATA data;
    UINT32   blkSize = 0;
    sector_t blkNum;
    UINT32   size;
    STATUS   rc = ERROR;
    
    blkSize = pXbd->xbd.xbd_blocksize;
    blkNum  = pXbd->xbd.xbd_nblocks;

    size = bio->bio_bcount;
    data.numBlks = size / blkSize;

    EMMC_XBD_DBG (EMMC_XBD_DBG_RW, "blkNo (%d) numBlks (%d) buf (0x%08x)\n",
                (long)bio->bio_blkno, (long)data.numBlks, (long)bio->bio_data, 0, 0, 0);

    /* if our starting block number is bad, done with error */

    if (bio->bio_blkno >= blkNum)
        {
        bio->bio_bcount = 0;
        bio_done (bio, ENOSPC);
        return;
        }

    /* if we overrun the end of the device, truncate the block number */

    if (bio->bio_blkno + data.numBlks > blkNum)
        data.numBlks = (UINT32)(blkNum - (bio->bio_blkno));

    /* calculate the real size */

    size = data.numBlks * blkSize;
    bio->bio_bcount = size;

    /* if we have less than 1 block, done it */

    if (data.numBlks == 0)
        {
        bio->bio_bcount = 0;
        bio_done (bio, 0);
        return;
        }

    data.buffer = bio->bio_data;
    data.blkNum = bio->bio_blkno;

    if (bio->bio_flags & BIO_READ)
        {
        rc = pXbd->xbdOps.blkRead (pXbd->pDev, &data);
        }
    else if (bio->bio_flags & BIO_WRITE)
        {
        rc = pXbd->xbdOps.blkWrite (pXbd->pDev, &data);
        }

    if (rc == OK)
        {
        bio_done (bio, 0);
        EMMC_XBD_DBG (EMMC_XBD_DBG_RW, "xbd block read/write success\n",
                     0, 0, 0, 0, 0, 0);
        }
    else
        {
        bio->bio_bcount = 0;
        bio_done(bio, errnoGet());
        EMMC_XBD_DBG (EMMC_XBD_DBG_RW, "xbd block read/write failed\n",
                     0, 0, 0, 0, 0, 0);
        }
    }

/*******************************************************************************
*
* emmcXbdGetBioPrio - Get the highest priority in the bio list.
*
* This routine get highest prority in the bio list.
*
* RETURN: highest priority value
*
* ERRNO: N/A
*/

LOCAL int emmcXbdGetBioPrio
    (
    EMMC_XBD_DEV * pXbd
    )
    {
    int highestPrio;
    struct bio * nextBio;

    highestPrio = pXbd->xbdSvcTskPri;

    if (pXbd->bioHead != NULL)
        {
        highestPrio = pXbd->bioHead->bio_priority;
        for(nextBio = pXbd->bioHead->bio_chain; nextBio != NULL;
            nextBio = nextBio->bio_chain)
            {
            highestPrio = (highestPrio < nextBio->bio_priority) ?
                           highestPrio:nextBio->bio_priority;
            }
        }
    EMMC_XBD_DBG(EMMC_XBD_DBG_RW, "[emmcXbdGetBioPrio]: highestPrio is %d \n",
               (int)highestPrio, 0, 0, 0, 0, 0);

    return highestPrio;
    }

/*******************************************************************************
*
* emmcXbdBioSvcTask - blkrage XBD bio handle task
*
* This routine is the task loop to handle bio operations.
*
* RETURN: N/A
*
* ERRNO: N/A
*/

LOCAL void emmcXbdBioSvcTask
    (
    EMMC_XBD_DEV * pXbd
    )
    {
    struct bio * bio;
    STATUS semSts;
    int highestPrio;

    FOREVER
        {

        /* wait semaphore from strategy routine */

        semSts = semTake (pXbd->bioReady, WAIT_FOREVER);
        if(semSts != OK)
            {

            /* return from semTake is not needed */

            (void)semTake (pXbd->bioMutex, WAIT_FOREVER);
            while ((bio = emmcXbdGetNextBio (pXbd)) != NULL)
                bio_done (bio, ENXIO);

            /* return from semGive is not needed */

            (void)semGive (pXbd->bioMutex);
            printf ("Take bioReady issue, xbd server task exit now\n");
            return;
            }

        /* process bio queue */

        EMMC_XBD_DBG(EMMC_XBD_DBG_RW, "emmcXbdBioSvcTask begin run emmcXbdExecBio\n",
                    0, 0, 0, 0, 0, 0);
        while (1)
            {

            /* return from semTake is not needed */

            (void)semTake (pXbd->bioMutex, WAIT_FOREVER);

            highestPrio = emmcXbdGetBioPrio (pXbd);

            /* if service task priority is high */

            if (pXbd->xbdSvcTskPri < highestPrio)
                {
                pXbd->xbdSvcTskPri = highestPrio;

                /* lower service task prio based on left BIOs */

                /* return from taskPrioritySet is not needed */

                (void)taskPrioritySet(0, highestPrio);
                }
            
            bio = emmcXbdGetNextBio (pXbd);

            /* return from semGive is not needed */

            (void)semGive (pXbd->bioMutex);
            if (bio == NULL)
                break;
            emmcXbdExecBio (pXbd, bio);
            }
        }
    }

/*******************************************************************************
*
* emmcXbdCreateSyncHandler - XBD creation handler
*
* This routine is the XBD creation handler.
*
* RETURN: N/A
*
* ERRNO: N/A
*/

LOCAL void emmcXbdCreateSyncHandler
    (
    UINT16  category,
    UINT16  type,
    void *  eventData,
    void *  userData
    )
    {
    EMMC_XBD_DEV * pXbd = (EMMC_XBD_DEV *)userData;

    if (pXbd == NULL)
        {
        EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdCreateSyncHandler() xbd == NULL\n",
                    0, 0, 0, 0, 0, 0);
        return;
        }

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdCreateSyncHandler() xbd == %p\n",
                pXbd, 0, 0, 0, 0, 0);
    if ((category != xbdEventCategory) || (type != xbdEventInstantiated))
        return;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdCreateSyncHandler() category == 0x%x   type == 0x%x\n",
                category,type, 0, 0, 0, 0);

    (void)erfHandlerUnregister (xbdEventCategory, xbdEventInstantiated,
                                emmcXbdCreateSyncHandler, userData);

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdCreateSyncHandler() erfHandlerUnregister success\n",
                0, 0, 0, 0, 0, 0);

    /* return from semGive is not needed */

    (void)semGive (pXbd->xbdSemId);
    }

/*******************************************************************************
*
* emmcXbdDevCreate - create an XBD device for blk
*
* This routine creates an XBD device for blk.
*
* RETURN: XBD device id
*
* ERRNO: N/A
*/

device_t emmcXbdDevCreate
    (
    EMMC_XBD_DEV    * pXbd,
    EMMC_XBD_PARAMS * pParams
    )
    {
    char taskName[MAX_DEVNAME + EMMC_XBD_SVC_NAME_LEN];
    char reqTaskName[MAX_DEVNAME + EMMC_XBD_SVC_NAME_LEN];
    device_t dev;
    STATUS rc;

    if ((pXbd == NULL) || (pParams == NULL))
        return NULLDEV;

    if (pParams->pDev == NULL)
        return NULLDEV;

    /* zero capacity is not allowed currently */

    if ((pParams->numBlks == 0) || (pParams->blkSize == 0))
        return NULLDEV;

    strncpy (pXbd->devName, pParams->devName, MAX_DEVNAME - 1);
    strncpy (taskName, pXbd->devName,  MAX_DEVNAME);
    strncat (taskName, EMMC_XBD_SVC_TASK_NAME, EMMC_XBD_SVC_NAME_LEN);
    strncpy (reqTaskName, pXbd->devName,  MAX_DEVNAME);
    strncat (reqTaskName, EMMC_XBD_REQ_SVC_TASK_NAME, EMMC_XBD_SVC_NAME_LEN);

    pXbd->pDev = pParams->pDev;
    pXbd->directModeFlag = pParams->directModeFlag;

    /*
     * Block read and block write are necessary.
     * If hotplug is supported, xbd test should not be NULL
     */

    if ((pParams->xbdOps.blkRead == NULL) ||
        (pParams->xbdOps.blkWrite == NULL))
        return NULLDEV;

    pXbd->xbdOps.blkRead    = pParams->xbdOps.blkRead;
    pXbd->xbdOps.blkWrite   = pParams->xbdOps.blkWrite;
    pXbd->xbdOps.blkDump    = pParams->xbdOps.blkDump;
    pXbd->xbdOps.blkIoctl   = pParams->xbdOps.blkIoctl;

    pXbd->bioMutex = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE |
                                SEM_DELETE_SAFE);

    if (pXbd->bioMutex == NULL)
        return NULLDEV;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "stoXbdDevCreate() Create bioMutex success\n", 
                0, 0, 0, 0, 0, 0);

    pXbd->bioReady = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    if (pXbd->bioReady == NULL)
        goto error;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdDevCreate() Create bioReady success\n",
                0, 0, 0, 0, 0, 0);

    pXbd->xbdSemId = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    if (pXbd->xbdSemId == NULL)
        goto error;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdDevCreate() Create xbdSemId success\n",
                0, 0, 0, 0, 0, 0);

    /* spawn service task */

    if ((pParams->svcTskPri <= 0) || (pParams->svcTskPri >= 255))
        pXbd->xbdSvcTskPri = EMMC_XBD_SVC_TASK_PRI;
    else
        pXbd->xbdSvcTskPri = pParams->svcTskPri;

    if (!(pXbd->directModeFlag))
        {
        pXbd->svcTaskId = taskSpawn (taskName, pXbd->xbdSvcTskPri,
                                    0, EMMC_XBD_SVC_TASK_STACK,
                                    (FUNCPTR)emmcXbdBioSvcTask, (_Vx_usr_arg_t)pXbd,
                                    (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                    (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                    (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                    (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                    (_Vx_usr_arg_t)0);
        if (pXbd->svcTaskId == TASK_ID_ERROR)
            goto error;
        }
#ifdef USE_XBD_BIO_SCHED
    /*  Set the xfer routine */
    pXbd->xbdOps.xferReq    = pParams->xbdOps.xferReq;

    /* Init the request list */

    lstInit(&pXbd->reqList);

    /* To protect the request list */

    pXbd->reqMutex = semMCreate(SEM_Q_PRIORITY |
                               SEM_DELETE_SAFE |
                               SEM_INVERSION_SAFE);
    if (pXbd->reqMutex == NULL)
        goto error;

    /* To signale the request service task */

    pXbd->reqReadySem = semCCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    if (pXbd->reqReadySem == SEM_ID_NULL)
        goto error;

    /* Create req service task */

    pXbd->reqSevcTask = taskSpawn (reqTaskName,
                                  pXbd->xbdSvcTskPri,
                                 0, EMMC_XBD_SVC_TASK_STACK,
                                 (FUNCPTR)emmcXbdReqSevTask,
                                 (_Vx_usr_arg_t)pXbd,
                                 (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                 (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                 (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                 (_Vx_usr_arg_t)0, (_Vx_usr_arg_t)0,
                                 (_Vx_usr_arg_t)0);
     if (pXbd->reqSevcTask == TASK_ID_ERROR)
        goto error;
#endif

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT,
                 "emmcXbdDevCreate() Create emmcXbdSvc success\n",
                 0,0,0,0,0,0);

    /* register new event handler for base device synchronization */

    rc = erfHandlerRegister (xbdEventCategory, xbdEventInstantiated,
                             emmcXbdCreateSyncHandler, pXbd, 0);
    if (rc != OK)
    {
        goto error;
    }

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdDevCreate() erfregister success\n",
                0, 0, 0, 0, 0, 0);

    /* attach XBD */

    pXbd->xbdInserted = TRUE;
    rc = xbdAttach (&pXbd->xbd, &emmcXbdFuncs, pXbd->devName, pParams->blkSize,
                    pParams->numBlks, &dev);

    if (rc != OK)
        goto error;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdDevCreate() xbdAttach success\n",
                0, 0, 0, 0, 0, 0);

#ifdef USE_XBD_BIO_SCHED
    xbdAttachSchecdPolicyById(&pXbd->xbd, BIO_SCHED_NOOP);

    /* Set the max bios per request */

    if (pParams->maxBiosPerReq > 0)
        xbdBioSchedMaxBiosPerReqSet(&pXbd->xbd, pParams->maxBiosPerReq);
    else
        xbdBioSchedMaxBiosPerReqSet(&pXbd->xbd, 1);

    if (pParams->maxActiveReqs > 0)
        xbdBioSchedMaxActiveReqsSet(&pXbd->xbd, pParams->maxActiveReqs);
    else
        xbdBioSchedMaxActiveReqsSet(&pXbd->xbd, 1);

    if (pParams->maxXferBlks > 0)
        xbdBioSchedMaxBlksPerReqSet(&pXbd->xbd, pParams->maxXferBlks);
    else
        xbdBioSchedMaxBlksPerReqSet(&pXbd->xbd, EMMC_XBD_MAX_XFER_BLKS);
#endif

    /* raise an insert event */

    rc = erfEventRaise (xbdEventCategory, xbdEventPrimaryInsert, ERF_ASYNC_PROC,
                        (void *)((long)dev), NULL);
    if (rc != OK)
        goto error;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "erfEventRaise() success\n",
                0, 0, 0, 0, 0, 0);

    /* wait for FSM to send xbdEventInstantiated event */

    rc = semTake (pXbd->xbdSemId, WAIT_FOREVER);
    if (rc != OK)
        goto error;

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdDevCreate() Take xbdSemId success\n",
                0, 0, 0, 0, 0, 0);

    return (dev);

error:

    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "emmcXbdDevCreate() xbdDevCreate failed\n",
                0, 0, 0, 0, 0, 0);

    /* return from semDelete is not needed */

    (void)semDelete (pXbd->bioMutex);
    (void)semDelete (pXbd->bioReady);
    (void)semDelete (pXbd->xbdSemId);

    if ((pXbd->svcTaskId != TASK_ID_ERROR) && (pXbd->svcTaskId != TASK_ID_NULL))
        (void)taskDelete (pXbd->svcTaskId);

#ifdef USE_XBD_BIO_SCHED
    if (pXbd->reqMutex != SEM_ID_NULL)
        (void)semDelete (pXbd->reqMutex);
    if (pXbd->reqReadySem != SEM_ID_NULL)
        (void)semDelete (pXbd->reqReadySem);
    if ((pXbd->reqSevcTask != TASK_ID_ERROR) &&
        (pXbd->reqSevcTask != TASK_ID_NULL))
        (void)taskDelete (pXbd->reqSevcTask);
#endif

    if (pXbd->xbdInserted == TRUE)
        pXbd->xbdInserted = FALSE;

    return (NULLDEV);
    }

/*******************************************************************************
*
* emmcXbdDevDelete - delete an XBD device for blk
*
* This routine deletes an XBD device for blk.
*
* RETURN: OK or ERROR
*
* ERRNO: N/A
*/

STATUS emmcXbdDevDelete
    (
    EMMC_XBD_DEV * pXbd
    )
    {
    struct bio * bio;
    STATUS       rc;
    UINT64       start, end;

    if ((pXbd == NULL) || (!pXbd->xbdInstantiated))
        return  ERROR;

    pXbd->xbdInserted = FALSE;

    /* wait till the service task is pending before deleting */

#ifdef _WRS_CONFIG_SMP
    start = vxAbsTicksGet();
#else
    start = vxAbsTicks;
#endif /* _WRS_CONFIG_SMP */

    while (pXbd->svcTaskId != 0)
        {
        (void)taskCpuLock ();
        if (taskIsPended (pXbd->svcTaskId))
            {
            rc = taskDelete (pXbd->svcTaskId);
            if (rc == ERROR)
                {
                (void)taskCpuUnlock();
                (void)taskDelay(1);
                continue;
                }
            pXbd->svcTaskId = 0;
            }
        (void)taskCpuUnlock ();
        (void)taskDelay (1);
#ifdef _WRS_CONFIG_SMP
        end = vxAbsTicksGet();
#else
        end = vxAbsTicks;
#endif /* _WRS_CONFIG_SMP */

        if ((end - start) > (UINT64)(sysClkRateGet() * EMMC_XBD_SVC_TASK_DELETE_TIMEOUT))
           {
           printf (" delete XBD server task fault \n");
           return (ERROR);
           }
        }

    /* raise a removal event */

    rc = erfEventRaise (xbdEventCategory, xbdEventRemove, ERF_ASYNC_PROC,
                        (void *)((long)pXbd->xbd.xbd_dev.dv_dev), NULL);

    /* detach XBD */

    xbdDetach (&pXbd->xbd);
    pXbd->xbdInstantiated = FALSE;

    (void)devUnmap ((struct device *)&pXbd->xbd);

    /* done any pending bio */

    /* return from semTake is not needed */

    (void)semTake (pXbd->bioMutex, WAIT_FOREVER);
    pXbd->bioHead = pXbd->bioCurr;
    while ((bio = emmcXbdGetNextBio (pXbd)) != NULL)
        bio_done (bio, ENXIO);

    /* return from semGive is not needed */

    (void)semGive (pXbd->bioMutex);

    /* delete semaphores */

    /* return from semDelete is not needed */

    (void)semDelete (pXbd->bioMutex);
    (void)semDelete (pXbd->bioReady);
    (void)semDelete (pXbd->xbdSemId);

    return (rc);
    }

/*******************************************************************************
*
* emmcXbdIoctl - XBD block device ioctl routine
*
* This routine handles a yet to completed list of ioctl calls.
*
* RETURN: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL int emmcXbdIoctl
    (
    struct xbd *    self,
    int             cmd,
    void *          arg
    )
    {
    EMMC_XBD_DEV * pXbd;
    int rc;

    pXbd = (EMMC_XBD_DEV *)self;

    switch ((UINT32)cmd)
        {
        case XBD_GETGEOMETRY:
            {
            XBD_GEOMETRY * geo;
            EMMC_XBD_DBG (EMMC_XBD_DBG_IOCTL, 
                         "[xbdIoctl]: xbd (%p) XBD_GETGEOMETRY\n",
                         pXbd, 0, 0, 0, 0, 0);

            geo = (XBD_GEOMETRY *)arg;
            if (pXbd->xbdOps.blkIoctl != NULL)
                {
                rc = pXbd->xbdOps.blkIoctl(pXbd->pDev, XBD_GETGEOMETRY, (void *)arg);
                if (rc != ENOTSUP)
                    break;
                }
            geo->total_blocks = pXbd->xbd.xbd_nblocks;
            geo->blocksize = pXbd->xbd.xbd_blocksize;

            /* setup dummy CHS information */

            geo->heads = 16;
            geo->secs_per_track = 63;
            geo->cylinders = (geo->total_blocks * geo->blocksize) / (16 * 63);
            rc = OK;
            break;
            }

        case XBD_SOFT_EJECT:
        case XBD_HARD_EJECT:
            {
            UINT16 reType;

            EMMC_XBD_DBG(EMMC_XBD_DBG_IOCTL, "[xbdIoctl]: xbd (%p) %s\n",
                        pXbd,
                        (long)((cmd == XBD_HARD_EJECT) ?
                        "XBD_HARD_EJECT" : "SOFT_HARD_EJECT"), 0, 0, 0, 0);

            /* Need not check return status at here */

            (void)erfEventRaise (xbdEventCategory, xbdEventRemove, ERF_ASYNC_PROC,
                                (void *)((long)pXbd->xbd.xbd_dev.dv_dev), NULL);

            reType = (UINT16)((cmd == XBD_HARD_EJECT) ?
                      xbdEventPrimaryInsert : xbdEventSoftInsert);

            /* Need not check return status at here */

            (void)erfEventRaise (xbdEventCategory, reType, ERF_ASYNC_PROC,
                                (void *)((long)pXbd->xbd.xbd_dev.dv_dev), NULL);

            rc = OK;
            break;
            }

        case XBD_GETBASENAME:
            {
            EMMC_XBD_DBG(EMMC_XBD_DBG_IOCTL, "[xbdIoctl]: xbd (%p) XBD_GETBASENAME\n",
                        pXbd, 0, 0, 0, 0, 0);

            rc = devName (pXbd->xbd.xbd_dev.dv_dev, arg);

            break;
            }

        case XBD_TEST:
            {
            EMMC_XBD_DBG(EMMC_XBD_DBG_IOCTL, "[xbdIoctl]: xbd (%p) XBD_TEST\n", 
                        pXbd, 0, 0, 0, 0, 0);

            if (pXbd->xbdOps.blkIoctl != NULL)
                {
                rc = pXbd->xbdOps.blkIoctl(pXbd->pDev, XBD_TEST, arg);
                }
            else
                {
                if (pXbd->xbdInserted)
                    rc = OK;
                else
                    rc = ERROR;
                }
            break;
            }

        case XBD_STACK_COMPLETE:
            {
            EMMC_XBD_DBG (EMMC_XBD_DBG_IOCTL, "[xbdIoctl]: xbd (%p) XBD_STACK_COMPLETE\n",
                         pXbd, 0, 0, 0, 0, 0);

            if (!pXbd->xbdInstantiated)
                {

                /* Need not check return status at here */

                (void)erfEventRaise (xbdEventCategory, xbdEventInstantiated,
                                     ERF_ASYNC_PROC,
                                     pXbd->xbd.xbd_dev.dv_xname, NULL);
                pXbd->xbdInstantiated = TRUE;
                }

            rc = OK;
            break;
            }

        case XBD_GETINFO:
            {
            EMMC_XBD_DBG(EMMC_XBD_DBG_IOCTL, "[xbdIoctl]: xbd (%p) XBD_GETINFO\n",
                        pXbd, 0, 0, 0, 0, 0);

            if (pXbd->xbdOps.blkIoctl != NULL)
                {
                rc = pXbd->xbdOps.blkIoctl(pXbd->pDev, XBD_GETINFO, arg);
                }
            else
                rc = ENOTSUP;
            break;
            }

        default:
            {
            if (pXbd->xbdOps.blkIoctl != NULL)
                rc = pXbd->xbdOps.blkIoctl (pXbd->pDev, cmd, (void *)arg);
            else
                rc = ENOTSUP;
            break;
            }
        }

    return (rc);
    }

/*******************************************************************************
*
* emmcXbdStrategy - XBD block device strategy routine
*
* This routine handles the data transaction.
*
* The information in the bio is used by the device driver to transfer data.
* This is an asynchronous function in that once the data in the bio is used
* to setup the data transfer, or queued until the device is ready,
* the function returns.
*
* RETURN: OK or ERROR
*
* ERRNO: N/A
*/

LOCAL int emmcXbdStrategy
    (
    struct xbd *    self,
    struct bio *    bio
    )
    {
    EMMC_XBD_DEV * pXbd;
    struct bio  * nextBio = NULL;
    STATUS        rc;

    if (self == NULL || bio == NULL)
        return (ERROR);

    EMMC_XBD_DBG (EMMC_XBD_DBG_INIT, "[xbdStrategy]: xbd (0x%08x) %s\n",
               (long)self,
               (long)((bio->bio_flags == BIO_READ) ? "read" : "write"),
               0, 0, 0, 0);

    pXbd = (EMMC_XBD_DEV *)self;

    /* if device is hot plugged out, finish BIO with S_ioLib_DISK_NOT_PRESENT */

    if (!pXbd->xbdInserted)
        {
        while (bio != NULL)
            {
            bio_done (bio, S_ioLib_DISK_NOT_PRESENT);
            bio = bio->bio_chain;
            }
        return (ERROR);
        }

    rc = semTake (pXbd->bioMutex, WAIT_FOREVER);
    if (rc != OK)
        return (ERROR);

    if ((pXbd->directModeFlag == FALSE) && 
        (pXbd->xbdSvcTskPri > bio->bio_priority))
        {
        pXbd->xbdSvcTskPri = bio->bio_priority;

        /* raise service task priority */

        /* return from taskPrioritySet is not needed */

        (void)taskPrioritySet(pXbd->svcTaskId, bio->bio_priority);

        EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "[xbdStrategy]: xbdSvcTskPri is set %d \n",
                   (int)pXbd->xbdSvcTskPri, 0, 0, 0, 0, 0);
        }

    /*
     * Direct BIO Mode:
     * Call the block read/write function for this BIO directly
     */

    if (pXbd->directModeFlag)
        {
        emmcXbdExecBio (pXbd, bio);
        }
    else
        {

        /* iterate through the chain, running each bio as we get it */

        for (; bio != NULL; bio = nextBio)
            {

            /* make sure that we can get the next one */

            nextBio = bio->bio_chain;

            /* add this bio as the tail of the queue */

            bio->bio_chain = NULL;

            if (pXbd->bioTail)
                {
                pXbd->bioTail->bio_chain = bio;
                }
            else
                {
                pXbd->bioHead = bio;
                }
            pXbd->bioTail = bio;
            }

        /* tap our service task */

        /* return from semGive is not needed */

        (void)semGive (pXbd->bioReady);
        }

    (void)semGive (pXbd->bioMutex);
    EMMC_XBD_DBG(EMMC_XBD_DBG_INIT, "[xbdStrategy]: semGive success \n",
                0, 0, 0, 0, 0, 0);
    return (OK);
    }

/*******************************************************************************
*
* emmcXbdDump - XBD block device dump routine
*
* This routine handles the data transaction with interrupts off.
*
* This call is similar to the xf_strategy call, only it is executed with
* interrupts off. This is intended to allow a filesystem to dump data to
* the device prior to a system crash.
*
* RETURN: OK or EINVAL if not supported.
*
* ERRNO: N/A
*/

LOCAL int emmcXbdDump
    (
    struct xbd *    self,
    sector_t        pos,
    void *          data,
    size_t          size
    )
    {
    EMMC_XBD_DEV * pXbd;

    if ((self == NULL) || (data == NULL) || (size == 0))
        return (EINVAL);

    pXbd = (EMMC_XBD_DEV *)self;
    if (pXbd->xbdOps.blkDump != NULL)
        return (pXbd->xbdOps.blkDump (pXbd->pDev, (XBD_DATA *)data));
    else
        return (EINVAL);
    }

#ifdef USE_XBD_BIO_SCHED

/*******************************************************************************
*
* emmcXbdReqDone - complete function for the xbd request
*
* This routine is the complete function for the xbd request
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void emmcXbdReqDone
    (
    XBD_REQUEST *  pXbdReq,
    unsigned       perStatus
    )
    {
    struct bio *  pBio;
    struct bio *  pNextBio;

    pBio = pXbdReq->pBioHead;

    while (pBio != NULL)
         {
         /* Get the next bio first, after bio_done, this bio may be delete */

         pNextBio = pBio->bio_chain;
         bio_done(pBio, perStatus);
         pBio = pNextBio;
         }

    xbdReqSchedComplete(pXbdReq);
    return;
    }

/*******************************************************************************
*
* blkDevXferReq - transfer request
*
* This routine transfers request.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void blkDevXferReq
    (
    EMMC_XBD_DEV *  pBlkXbd,
    XBD_REQUEST *  pXbdReq
    )
    {
    struct bio  *  pBio;
    struct bio  *  pNextBio;
    FUNCPTR        pFunc;
    sector_t       blkNum;

    blkNum  = pBlkXbd->xbd.xbd_nblocks;

    /* Valid parameter */

    if ((pXbdReq->reqFirstBlk > blkNum) ||
        ((pXbdReq->reqFirstBlk + pXbdReq->reqNumBlks) > blkNum))
        {
        emmcXbdReqDone(pXbdReq, ENOSPC);
        return;
        }

    /* Get the xferReq routine */

    pFunc = pBlkXbd->xbdOps.xferReq;

    if (pFunc != NULL)
        {
        pFunc(pBlkXbd, pXbdReq);
        }
    else
        {
        /* If driver doesn't support xfer request, we will exec bio one by one */

        pBio = pXbdReq->pBioHead;
        while(pBio != NULL)
            {
            pNextBio = pBio->bio_chain;
            emmcXbdExecBio(pBlkXbd, pBio);
            pBio = pNextBio;
            }

        xbdReqSchedComplete(pXbdReq);
        return;
        }
    }

/*******************************************************************************
 *
 * emmcXbdXferRequest - routine to xfer the request
 *
 * This routine used to xfer the request.
 *
 * RETURNS: N/A
 *
 * ERRNO: N/A
 *
 * \NOMANUAL
 */

LOCAL int emmcXbdXferRequest
    (
    XBD *                   pXbd,
    XBD_REQUEST *           pXbdReq
    )
    {
    int            priority;
    STATUS         rc;
    EMMC_XBD_DEV *  pBlkXbd;

    /* Valid parameter */

    if (pXbd == NULL || pXbdReq == NULL)
        return (ERROR);

    EMMC_XBD_DBG (EMMC_XBD_DBG_RW, "[emmcXbdXferRequest]: pBlkXbd (0x%08x) %s req %p\n",
                 (long)pXbd,
                 (long)((pXbdReq->reqflags == BIO_READ) ? "read" : "write"),
                 pXbdReq, 0, 0, 0);

    EMMC_XBD_DBG(EMMC_XBD_DBG_RW, "firstBlk %d, numBlk %d, numBio %d \n",
    pXbdReq->reqFirstBlk, pXbdReq->reqNumBlks, pXbdReq->numBios, 4, 5, 6);

    pBlkXbd = (EMMC_XBD_DEV *)pXbd;
    if (!pBlkXbd->xbdInserted)
        {
        EMMC_XBD_DBG(EMMC_XBD_DBG_RW, "[emmcXbdXferRequest]: pBlkXbd (0x%08x) %x return ENXIO\n",
                     (long)pBlkXbd, 0, 0, 0, 0, 0);
        emmcXbdReqDone (pXbdReq, S_ioLib_DISK_NOT_PRESENT);
        return (ERROR);
        }

    rc = semTake (pBlkXbd->reqMutex, WAIT_FOREVER);
    if (rc != OK)
        {
        emmcXbdReqDone (pXbdReq, EIO);
        return (ERROR);
        }

    /* Get the current req service task priority */

    (void)taskPriorityGet(pBlkXbd->reqSevcTask, &priority);

    /* If the reqPriority is higher (small number means high priority) */

    if (priority > pXbdReq->reqPriority)
        {
        priority = pXbdReq->reqPriority;

        /* raise service task priority */
        /* return from taskPrioritySet is not needed */

        (void)taskPrioritySet(pBlkXbd->reqSevcTask, pXbdReq->reqPriority);
        }

    EMMC_XBD_DBG(EMMC_XBD_DBG_RW, "[emmcXbdXferRequest]: sataServTskPri is set %d \n",
                 (int)priority, 0, 0, 0, 0, 0);

    /*
     * Add the request to the Driver list tail and Signal the service task
     * to handle the pending request
     */

    lstAdd(&pBlkXbd->reqList, &pXbdReq->listNode);
    (void)semGive (pBlkXbd->reqReadySem);
    (void)semGive (pBlkXbd->reqMutex);
    return (OK);
    }

/*******************************************************************************
*
* blkNextReqGet - get the next request waiting to handle
*
* This routine gets the next request waiting to handle
*
* RETURNS: pointer to the XBD_REQUEST, or NULL if fail
*
* ERRNO: N/A
*
* \NOMANUAL
*/

pXBD_REQUEST blkNextReqGet
    (
    EMMC_XBD_DEV * pBlkXbd
    )
    {
    NODE *         pNode;
    XBD_REQUEST *  pXbdReq = NULL;

    if (semTake (pBlkXbd->reqMutex, WAIT_FOREVER) == ERROR)
        return NULL;

    pNode = lstFirst(&pBlkXbd->reqList);
    if (pNode != NULL)
        {
        pXbdReq = LIST_NODE_TO_XBD_REQUEST(pNode);
        lstDelete(&pBlkXbd->reqList, &pXbdReq->listNode);
        }

    (void)semGive (pBlkXbd->reqMutex);
    return pXbdReq;
    }

/*******************************************************************************
*
* blkReqHigPriGet - Get the higest priority of the request list
*
* This routine gets the higest priority of the request list
*
* RETURNS: higest priority of the request list
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int blkReqHigPriGet
    (
    EMMC_XBD_DEV * pBlkXbd
    )
    {
    int highestPrio = pBlkXbd->xbdSvcTskPri;
    NODE *         pNode;
    XBD_REQUEST *  pXbdReq = NULL;

    pNode = lstFirst(&pBlkXbd->reqList);
    while (pNode != NULL)
        {
        pXbdReq = LIST_NODE_TO_XBD_REQUEST(pNode);
        highestPrio = (highestPrio < pXbdReq->reqPriority) ?
                       highestPrio:pXbdReq->reqPriority;
        pNode = lstNext(pNode);
        }

    EMMC_XBD_DBG(EMMC_XBD_DBG_RW,
                 "[blkReqHigPriGet]: highestPrio is %d \n",
                 (int)highestPrio, 0, 0, 0, 0, 0);

    return highestPrio;
    }

/*******************************************************************************
*
* emmcXbdReqSevTask - service task routine to handle the request
*
* This routine is the service task function used to handle the request.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

LOCAL VOID emmcXbdReqSevTask
    (
    EMMC_XBD_DEV * pBlkXbd
    )
    {
    int            higPri;
    int            taskPri;
    XBD_REQUEST *  pXbdReq;

    /* Service task to handle request */

    while (TRUE)
        {
        /* Wait semaphore from strategy routine */

        if (semTake(pBlkXbd->reqReadySem, WAIT_FOREVER) == ERROR)
            return;

        higPri = blkReqHigPriGet(pBlkXbd);

        /* Get the current priority */

        (void)taskPriorityGet(0, &taskPri);

        /* if service task priority is high */

        if (taskPri < higPri)
            {
            /* lower service task prio based on left BIOs */
            /* return from taskPrioritySet is not needed */

            (void)taskPrioritySet(0, higPri);
            }

        while ((pXbdReq = blkNextReqGet(pBlkXbd)) != NULL)
            {
            blkDevXferReq(pBlkXbd, pXbdReq);
            }
        }
    }

#endif

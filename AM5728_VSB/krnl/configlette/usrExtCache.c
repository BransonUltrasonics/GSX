/* usrExtCache.c - ARM external L2 cache configlette */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26aug13,c_t  written.
*/

/*
DESCRIPTION
Provide usrExtL2CacheInit routine to initial external L2 cache
when VxWorks bing up.

NOMANUAL
*/

/* includes */

#include <vxWorks.h>
#include <cacheLib.h>
#include <spinLockLib.h>
#include <arch/arm/cacheArmLib.h>

/* defines */

/* typedefs */

/* locals */

#ifdef _WRS_CONFIG_COMPAT69_ARM_ARCH
LOCAL struct extCache extcc;
#endif /* _WRS_CONFIG_COMPAT69_ARM_ARCH */

/* globals */

#ifdef _WRS_CONFIG_COMPAT69_ARM_ARCH

/* these variables used by vx6x bsps, implemented in sysL2Cache.c */

void (*_pSysL2CacheEnable)(CACHE_TYPE cacheType)  = NULL;
void (*_pSysL2CacheDisable)(CACHE_TYPE cacheType) = NULL;
void (*_pSysL2CacheInvFunc)(CACHE_TYPE cacheType, void * start, void * end) = NULL;
void (*_pSysL2CacheFlush)(CACHE_TYPE cacheType, void * start, void * end)   = NULL;
void (*_pSysL2CacheClear)(CACHE_TYPE cacheType, void * start, void * end)   = NULL;
void (*_pSysL2CachePipeFlush)(void) = NULL;

/* the global variable to represent the L2 cache enable state */

BOOL sysL2CacheEnabled = FALSE; /* L2 cache is disabled by default */

#endif /* _WRS_CONFIG_COMPAT69_ARM_ARCH */

/* externs */

/***************************************************************************
*
* usrExtL2CacheInit - external L2 cache initial routine
*
* Calling this routine to install L2 cache call back funtions.
*
* RETURNS: OK (always)
*
* ERRNO: N/A
*/

STATUS usrExtL2CacheInit
    (
    void
    )
    {
#ifdef _WRS_CONFIG_COMPAT69_ARM_ARCH
    extcc.enableRtn     = _pSysL2CacheEnable;
    extcc.disableRtn    = _pSysL2CacheDisable;
    extcc.flushRtn      = _pSysL2CacheFlush;
    extcc.clearRtn      = _pSysL2CacheClear;
    extcc.invalidateRtn = _pSysL2CacheInvFunc;
    extcc.pipeFlushRtn  = _pSysL2CachePipeFlush;

    if (sysL2CacheEnabled)
        return (OK);

    cacheExternInstall (&extcc);

    sysL2CacheEnabled = TRUE;
#endif

    /* new style BSP init L2 in the PL310 driver */

    return OK;
    }

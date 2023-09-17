/* 30arch.cdf - ARCH component description file */

/*
 * Copyright (c) 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15dec16,cww  moved INCLUDE_EXC_SIGNALS to core
16aug16,g_x  added INCLUDE_ARM_CTXR_MPU (F6411)
18may16,pmr  added prototype for excSigLibInit()
22dec15,gls  created

DESCRIPTION

This file contains descriptions for the ARMV7AR architecture.  Entries
here may override those in armbase/cdf/arch.cdf.
*/

#ifdef _WRS_CONFIG_ARMV7_CTX_R
Component INCLUDE_ARM_CTXR_MPU {
        SYNOPSIS        ARM Cortex-R MPU initialization
        _INIT_ORDER     usrInit
        INIT_RTN        armCtxRMpuInit(USER_MPU_I_CACHE_MODE, \
                                       USER_MPU_D_CACHE_MODE);
        INIT_AFTER      INCLUDE_CACHE_SUPPORT
        INIT_BEFORE     INCLUDE_EXC_HANDLING
        CFG_PARAMS      USER_MPU_I_CACHE_MODE USER_MPU_D_CACHE_MODE
        PROTOTYPE       IMPORT void armCtxRMpuInit(CACHE_MODE inst, \
                                                   CACHE_MODE data);
        _REQUIRES       INCLUDE_KERNEL_ARCH
        _CHILDREN       FOLDER_NOT_VISIBLE
        HDR_FILES       cacheLib.h
}

Parameter USER_MPU_I_CACHE_MODE {
        NAME            ARM Cortex-R MPU instruction cache mode
        DEFAULT         (INCLUDE_CACHE_ENABLE)::(USER_I_CACHE_MODE) \
                        (CACHE_WRITETHROUGH)
}

Parameter USER_MPU_D_CACHE_MODE {
        NAME            ARM Cortex-R MPU data cache mode
        DEFAULT         (INCLUDE_CACHE_ENABLE)::(USER_D_CACHE_MODE) \
                        (CACHE_COPYBACK)
}

#endif /* _WRS_CONFIG_ARMV7_CTX_R */

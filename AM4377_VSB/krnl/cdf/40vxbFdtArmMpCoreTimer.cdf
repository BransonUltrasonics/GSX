/* 40vxbFdtArmMpCoreTimer.cdf - Component configuration file */

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
23oct13,sye  written
*/

Component DRV_TIMER_FDT_ARM_MPCORE {
        NAME            FDT ARM MPCORE Timer Driver
        SYNOPSIS        FDT ARM Cortex-A9 MPCORE Private Timer Driver
        MODULES         vxbFdtArmMpCoreTimer.o
        LINK_SYMS       vxbFdtArmMpCoreTimerDrv
        REQUIRES        INCLUDE_VXBUS \
                        INCLUDE_TIMER_SYS
        _CHILDREN       FOLDER_NOT_VISIBLE
}

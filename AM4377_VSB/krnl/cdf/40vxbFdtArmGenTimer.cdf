/* 40vxbFdtArmGenTimer.cdf - Component configuration file */

/*
 * Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20jun16,swu  rename vxbFdtArmv7GenTimer to vxbFdtArmGenTimer. (F5261)
09oct14,yya  Created. (US47371)
*/

Component DRV_ARM_GEN_TIMER
    {
    NAME        ARM generic timer
    SYNOPSIS    Provides the ARM generic timer driver.
    MODULES     vxbFdtArmGenTimer.o
    LINK_SYMS   armGenTimerDrv
    REQUIRES    DRV_BUS_FDT_ROOT        \
                INCLUDE_TIMER_SYS
    _CHILDREN   FOLDER_DRIVERS
    }

#ifdef _WRS_CONFIG_CPU_ARMARCH7
Component DRV_ARMV7_GEN_TIMER
    {
    NAME        (Deprecated) ARM generic timer for ARMv7
    SYNOPSIS    (Deprecated) Provides the ARM generic timer driver for ARMv7. \
                Use the DRV_ARM_GEN_TIMER component instead.
    REQUIRES    DRV_ARM_GEN_TIMER
    _CHILDREN   FOLDER_DRIVERS
    }
#endif

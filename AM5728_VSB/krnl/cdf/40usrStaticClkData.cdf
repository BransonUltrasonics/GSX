/* 40usrStaticClkData.cdf -  static clock data configlettes */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
04jun14,sye  created. (V7PRO-935)
*/

Component INCLUDE_USR_STATIC_CLK_DATA {
    NAME                Static clock data for clock library
    SYNOPSIS            User defined static clock data for clock library.
    CONFIGLETTES        usrClockData.c
    _INIT_ORDER         usrRoot
    INIT_RTN            usrStaticClkDataInit ();
    _CHILDREN           FOLDER_NOT_VISIBLE
    INCLUDE_WHEN        DRV_CLK_FDT_AM3
    INIT_BEFORE         INCLUDE_VXBUS
    INIT_AFTER          INCLUDE_DEVCLK_SYS
}    

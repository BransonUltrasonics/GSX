/* 40vxbAm3Rtc.cdf  - AM3XXX Real Time Clock */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
13oct13,c_t  created
*/

Component   DRV_FDT_RTC_AM3 {
    NAME        AM3 series RTC vxBus driver
    SYNOPSIS    AM3 series RTC vxBus driver
    MODULES     vxbAm3Rtc.o
    LINK_SYMS   vxbAm3RtcDrv
    _CHILDREN   FOLDER_DRIVERS
    REQUIRES    INCLUDE_VXBUS \
                DRV_BUS_FDT_ROOT
}

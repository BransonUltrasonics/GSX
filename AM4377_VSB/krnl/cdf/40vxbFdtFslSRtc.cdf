/* 40vxbFdtFslSRtc.cdf  - Freescale Secure Real Time Clock */

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
29aug14,yjw  created (US40550)
*/

Component   DRV_SRTC_FDT_FSL {
    NAME        Freescale SRTC vxBus driver
    SYNOPSIS    Freescale SRTC vxBus driver
    MODULES     vxbFdtFslSRtc.o
    LINK_SYMS   vxbFslSRtcDrv
    _CHILDREN   FOLDER_DRIVERS
    REQUIRES    INCLUDE_VXBUS \
                DRV_BUS_FDT_ROOT
}

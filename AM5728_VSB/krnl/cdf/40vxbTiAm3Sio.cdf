/* 40vxbTiAm3Sio.cdf - Ti AM33xx UART Driver */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,25jun14,c_t  written.(US40510)
*/

Component DRV_SIO_TI_AM3 {
    NAME            UART support for Ti Am33xx devices
    SYNOPSIS        UART support for Ti Am33xx devices
    MODULES         vxbTiAm3Sio.o
    LINK_SYMS       tiAm3SioDrv
    REQUIRES        DRV_BUS_FDT_ROOT  \
                    INCLUDE_ISR_DEFER \
                    INCLUDE_SIO
    _CHILDREN       FOLDER_DRIVERS
}

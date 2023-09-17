/* 40vxbFslEspi.cdf - Component Definition file for Freescale ESPI */

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
01a,23may13,y_y  created
*/

Component       DRV_FSL_ESPI {
    NAME        freescale Espi controller Driver
    SYNOPSIS    vxBus Driver for freescale Espi controller
    MODULES     vxbFslEspi.o
    LINK_SYMS   vxbOfFslEspiCtlrDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}

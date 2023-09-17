/* 40vxbFdtFslQspi.cdf - Component configuration file */

/*
 * Copyright (c) 2014, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
09feb17,mw1  rename vxbFdtFslQspiFlash to vxbFdtFslQspi (US89250)
18sep14,m_y  added QSPI NOR flash support for Freescale (US46192)
*/

Component DRV_FDT_FSLQSPI {
    NAME            NXP/Freescale QSPI controller driver
    SYNOPSIS        Shared driver for NXP/Freescale QSPI controller \
                    integrated in various SoCs.
    MODULES         vxbFdtFslQspi.o
    LINK_SYMS       vxbFslQspiDrv
    _INIT_ORDER     usrRoot
    REQUIRES        INCLUDE_VXBUS
    _CHILDREN       FOLDER_FLASH
}


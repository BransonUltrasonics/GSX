/* 40vxbTiClkAm4.cdf - TI Sitara clock system control */

/*
 * Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
04may16,m_w  update clock system to support AM57xx.(F7158)
03jul14,m_w  written.(US40646)
*/

Component DRV_CLK_FDT_AM4 {
        NAME            FDT TI Sitara Clock driver
        SYNOPSIS        FDT TI Sitara Clock driver
        MODULES         vxbFdtTiClkAm4.o
        LINK_SYMS       tiClkTreeDrv            \
                        tiClkDomainDrv          \
                        tiFixedClkDrv           \
                        tiFixedFactorClkDrv     \
                        tiMuxClkDrv             \
                        tiDividerClkDrv         \
                        tiGateClkDrv            \
                        tiModGateClkDrv         \
                        tiDpllClkDrv            \
                        tiApllClkDrv            \
                        tiDpllX2ClkDrv     
        REQUIRES        DRV_BUS_FDT_ROOT        \
                        INCLUDE_DEVCLK_SYS
        _CHILDREN       FOLDER_DRIVERS

}

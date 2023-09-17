/* 40vxbZynqGemEnd.cdf - Xilinx zynq end */

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
27dec16,swu  rename 40vxbZynq7kGemEnd.cdf to 40vxbZynqGemEnd.cdf (F5261)
12dec16,g_x  added INCLUDE_DEVCLK_SYS requires (F7695)
09aug16,sye  set _CHILDREN to FOLDER_NET_END_DRV (V7PRO-3187)
11feb14,y_c  created.(US34090)
*/

Component DRV_END_FDT_XLNX_ZYNQ7K {
    NAME            (Deprecated) Xilinx Zynq-7000 GEM Enhanced Network Driver
    SYNOPSIS        (Deprecated) Xilinx Zynq-7000 GEM Enhanced Network Driver
    HELP            (Deprecated) It has been deprecated in favour of the  \
                    DRV_END_FDT_XLNX_ZYNQ component.
    REQUIRES        DRV_END_FDT_XLNX_ZYNQ
    _CHILDREN       FOLDER_NET_END_DRV
}

Component DRV_END_FDT_XLNX_ZYNQ {
    NAME            Xilinx Zynq GEM Enhanced Network Driver
    SYNOPSIS        Xilinx Zynq GEM Enhanced Network support.
    LINK_SYMS       zynqGemDrv
    REQUIRES        INCLUDE_DMA_SYS \
                    INCLUDE_END \
                    INCLUDE_GENERICPHY \
                    DRV_BUS_FDT_ROOT \
                    INCLUDE_DEVCLK_SYS
    _CHILDREN       FOLDER_NET_END_DRV
}

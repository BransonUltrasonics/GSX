/* 10dma.cdf - Component configuration file */

/*
 * Copyright (c) 2013, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------
15arp16,l_z  fix typo for FOLDER_SUBSYSTEM. (V7PRO-3016)
16jul13,l_z  created for VxBus GEN2
*/

Folder FOLDER_SUB_DMA {                    
    NAME            DMA support subsystem  
    SYNOPSIS        DMA support subsystem  
    _CHILDREN       FOLDER_SUBSYSTEM       
}

Component INCLUDE_DMA_SYS {
    NAME            vxBus DMA subsystem
    SYNOPSIS        vxBus DMA subsystem
    PROTOTYPE       STATUS vxbDmaLibInit (void);
    INIT_RTN        vxbDmaLibInit();
    _INIT_ORDER     usrRoot
    INIT_BEFORE     INCLUDE_VXBUS
    REQUIRES        INCLUDE_VXBUS
    _CHILDREN       FOLDER_SUB_DMA 
}

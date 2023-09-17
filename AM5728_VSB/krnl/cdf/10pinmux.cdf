/* 10pinmux.cdf - Component configuration file */

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
03jan14,sye  added prototype.
23sep13,c_t  created for VxBus GEN2
*/

Folder FOLDER_SUB_PINMUX {                    
    NAME            PINMUX support subsystem  
    SYNOPSIS        PINMUX support subsystem  
    _CHILDREN       FOLDER_SUBSYSTEM       
}

Component INCLUDE_PINMUX_SYS {
    NAME            vxBus pinmux subsystem
    SYNOPSIS        vxBus pinmux subsystem
    PROTOTYPE       STATUS vxbPinMuxLibInit (void);    
    INIT_RTN        vxbPinMuxLibInit();
    _INIT_ORDER     usrRoot
    INIT_BEFORE     INCLUDE_VXBUS
    REQUIRES        INCLUDE_VXBUS
    _CHILDREN       FOLDER_SUB_PINMUX
}

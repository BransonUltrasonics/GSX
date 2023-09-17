/* 10flash.cdf - flash subsystem Component configuration file */

/*
 * Copyright (c) 2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------
15arp16,l_z  fix typo for FOLDER_SUBSYSTEM. (V7PRO-3016)
23feb16,ffu  add vxbflashLibInit function to called. (V7STO-337)
16jan14,ywu  created for VxBus GEN2
*/

Folder FOLDER_SUB_FLASH {                    
    NAME            flash support subsystem  
    SYNOPSIS        flash support subsystem  
    _CHILDREN       FOLDER_SUBSYSTEM       
}

Component INCLUDE_FLASH_SYS {
    NAME            vxBus flash subsystem
    SYNOPSIS        vxBus flash subsystem
    PROTOTYPE       STATUS vxbFlashLibInit (void);
    INIT_RTN        vxbflashLibInit();
    INIT_BEFORE     INCLUDE_VXBUS
    REQUIRES        INCLUDE_VXBUS
    _CHILDREN       FOLDER_SUB_FLASH
}


/* 10devclk.cdf - Device Clock Component configuration file */

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
31aug16,l_z  disable INCLUDE_DEVCLK_SYS when FDT is excluded. (V7PRO-3296)
15arp16,l_z  fix typo for FOLDER_SUBSYSTEM. (V7PRO-3016)
23sep13,c_t  created for VxBus GEN2
*/

#ifdef _WRS_CONFIG_FDT
Folder FOLDER_SUB_CLK {
    NAME            clock support subsystem
    SYNOPSIS        clock support subsystem
    _CHILDREN       FOLDER_SUBSYSTEM
}

Component INCLUDE_DEVCLK_SYS {
    NAME            vxBus device clock subsystem
    SYNOPSIS        vxBus device clock subsystem
    PROTOTYPE       void vxbClkLibInit(void);
    INIT_RTN        vxbClkLibInit();
    _INIT_ORDER     usrRoot
    INIT_BEFORE     INCLUDE_VXBUS
    REQUIRES        INCLUDE_VXBUS
    _CHILDREN       FOLDER_SUB_CLK
}
#endif /* _WRS_CONFIG_FDT */

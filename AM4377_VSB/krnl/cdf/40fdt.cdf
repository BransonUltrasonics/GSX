/* 40fdt.cdf - Flat Device Tree component description file */

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04jun16,l_z  fix the SYNOPSIS truncated issue. (V7PRO-3179)
21apr16,emj  CERT: Update to produce partially linked objects (US79607)
06apr16,l_z  fixes INCLUDE_FDT_SHOW init order. (V7PRO-2986)
20apr15,yya  fixed component dependency issue (V7PRO-1988)
11mar15,yya  added DTB self-patch to support legacy bootloader (US55212)
20nov14,ylu  LP64 adaptation to support PPC64.(F2518)
27mar14,cww  added STACK_SAVE and RAM_LOW_ADRS to INCLUDE_FDT_LIB
23may13,x_z  created
*/

Folder  FOLDER_FDT {
        NAME        Flat Device Tree
        SYNOPSIS    Flat Device Tree
        _CHILDREN   FOLDER_BSP_CONFIG
}

Component INCLUDE_FDT_LIB {
    NAME            FDT library
    SYNOPSIS        Flat Device Tree library
    CONFIGLETTES    usrFdt.c
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
    MODULES         vxFdtLib.o
#else
                        _REQUIRES INCLUDE_KERNEL
#endif
    PROTOTYPE       void usrFdtInit (void *, int);
    INIT_RTN        usrFdtInit ((void*)DTB_RELOC_ADDR, (int)DTB_MAX_LEN);
    _INIT_ORDER     usrInit
    INIT_AFTER      INCLUDE_SYS_START
    INIT_BEFORE     INCLUDE_BOARD_LIB_INIT
    _CHILDREN       FOLDER_FDT
    CFG_PARAMS      DTB_RELOC_ADDR	\
                    DTB_MAX_LEN		\
		    STACK_SAVE		\
		    RAM_LOW_ADRS
}

Parameter DTB_RELOC_ADDR {
    NAME            DTB relocateds address
    SYNOPSIS        DTB(Device Tree Blob) need be relocated to one safe address \
                    to avoid be overwritten, so it should be below RAM_LOW_ADRS \
                    and the reserved start region, and enough for the entire \
                    DTB.
    TYPE            void *
    DEFAULT         (LOCAL_MEM_LOCAL_ADRS + 0x5000)
}

Parameter DTB_MAX_LEN {
    NAME            DTB maximum length
    SYNOPSIS        DTB(Device Tree Blob) need be relocated to one safe address \
                    to avoid be overwritten, so it should be below RAM_LOW_ADRS \
                    and the reserved start region, and enough for the entire \
                    DTB.
    TYPE            int
    DEFAULT         (RAM_LOW_ADRS - STACK_SAVE- DTB_RELOC_ADDR)
}

Component INCLUDE_FDT_SHOW {
    NAME            FDT show routines
    SYNOPSIS        Flat Device Tree show routines
    MODULES         vxFdtShow.o
    PROTOTYPE       void vxFdtShowInit ();
    INIT_RTN        vxFdtShowInit ();
    _INIT_ORDER     usrInit
    INIT_BEFORE     INCLUDE_BOARD_LIB_INIT
    INIT_AFTER      INCLUDE_FDT_LIB
    _CHILDREN       FOLDER_FDT
    REQUIRES        INCLUDE_FDT_LIB
}

/*
This component only patches standalone DTB to inject vxWorks-6.x style bootline. 
For separate DTB, the bootloader is responsible to prepare everything and the
OS just uses it as it is.
*/

Component INCLUDE_PATCH_STANDALONE_DTB {
    NAME            Patch built-in DTB to inject bootline
    SYNOPSIS        Self-patch the built-in DTB. This function overwrites the \
                    bootline in STANALONE_DTB with the bootline passed from the \
                    BOOTAPP, to make the OS be compatible with legacy bootloder \
                    that does not support DTB and instead passes bootline to \
                    vxWorks image.
    _CHILDREN       FOLDER_FDT
}

/* 00boardLib.cdf - Board library component description file */

/*
 * Copyright (c) 2012-2014, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
27oct16,c_l  added missing call sysIntEnableFlagSet (V7PRO-3373)
21apr16,emj  CERT: Update to produce partially linked objects (US79607)
03jul14,j_l  US41956 - Implement GEN2 bootApp network boot
21may14,cww  added missing folders from previous change (V7COR-1176)
27mar14,cww  added required components from 00bsp.cdf (V7COR-907)
06mar14,j_l  US18115 - additional changes for IA
24feb14,x_z  added KERNEL_LOAD_ADRS. (VXW7-1416)
20feb14,x_z  removed unused VSB option.
09oct13,x_z  cleaned.
23may13,x_z  created
*/

Folder  FOLDER_BOARD_LIB {
        NAME        Board library
        SYNOPSIS    board subsystem to supply interfaces to BSP \
                    for other OS components
        _CHILDREN   FOLDER_KERNEL
}

Folder  FOLDER_HARDWARE {
    NAME        Hardware
    SYNOPSIS    Hardware components
    CHILDREN    FOLDER_MEMORY       \
                FOLDER_PERIPHERALS  \
                FOLDER_BSP_CONFIG
    DEFAULTS    FOLDER_MEMORY       \
                FOLDER_BSP_CONFIG
}

Folder  FOLDER_BSP_CONFIG {
    NAME            BSP configuration variants
    CHILDREN        INCLUDE_BSP_MACROS
    DEFAULTS        INCLUDE_BSP_MACROS
}

Folder FOLDER_PERIPHERALS {
    NAME        Peripherals
    SYNOPSIS    Peripheral components
}

Folder FOLDER_FPP {
    NAME        Floating point libraries
	SYNOPSIS	This folder contains floating point libraries.
    CHILDREN    INCLUDE_SW_FP INCLUDE_HW_FP
    _CHILDREN   FOLDER_PERIPHERALS
}

Component INCLUDE_BOARD_LIB_INIT {
    NAME            Initialize board library
    SYNOPSIS        Initialize board subsystem to supply BSP access APIs
    CONFIGLETTES    usrBoardLibInit.c
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
    MODULES         boardLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
    HDR_FILES       boardLib.h
    PROTOTYPE       void usrBoardLibInit (void );
    INIT_RTN        usrBoardLibInit();
    _CHILDREN       FOLDER_BOARD_LIB
    REQUIRES        INCLUDE_BOARD_INIT
    _REQUIRES       INCLUDE_KERNEL
    _INIT_ORDER     usrInit
    INIT_AFTER      INCLUDE_SYS_START
    INIT_BEFORE     INCLUDE_CPU_INIT
}

Component INCLUDE_BOARD_INIT {
    NAME        initialize board
    SYNOPSIS	call the boardInit() routine during system startup
    INIT_RTN	boardInit();
    HDR_FILES	sysLib.h
    _CHILDREN   FOLDER_BOARD_LIB
    _REQUIRES       INCLUDE_KERNEL
    _INIT_ORDER     usrRoot
#if (defined _WRS_CONFIG_ARCH_i86)
    INIT_AFTER      INCLUDE_SYSHW_INIT_1
#else
    INIT_AFTER      INCLUDE_TIMER_STUB
#endif /* (defined _WRS_CONFIG_ARCH_i86) */
    INIT_BEFORE     INCLUDE_VXBUS
}

Component INCLUDE_BSP_MACROS {
    NAME            General BSP macros
    SYNOPSIS        Placeholder component for general BSP macros
    CFG_PARAMS      DEFAULT_BOOT_LINE
    INCLUDE_WHEN    INCLUDE_KERNEL
}

Component INCLUDE_MEMORY_CONFIG {
    NAME            BSP Memory Configuration
    SYNOPSIS        Memory configuration parameter for BSP
    INCLUDE_WHEN    INCLUDE_KERNEL
    CFG_PARAMS      LOCAL_MEM_LOCAL_ADRS    \
                    LOCAL_MEM_SIZE          \
                    LOCAL_MEM_AUTOSIZE      \
                    LOCAL_MEM_PHYS_ADRS     \
                    VEC_BASE_ADRS           \
                    EXC_MSG_OFFSET          \
                    EXC_MSG_ADRS            \
                    BOOT_LINE_SIZE          \
                    BOOT_LINE_ADRS          \
                    BOOT_LINE_OFFSET        \
                    RESERVED                \
                    FREE_RAM_ADRS           \
                    ROM_WARM_ADRS           \
                    STACK_SAVE              \
                    RAM_HIGH_ADRS           \
                    RAM_LOW_ADRS            \
                    ROM_BASE_ADRS           \
                    ROM_TEXT_ADRS           \
                    ROM_SIZE
}

#ifdef _WRS_CONFIG_FDT
Parameter KERNEL_LOAD_ADRS {
    NAME        Runtime kernel load address
    SYNOPSIS    The address for VxWorks kernel load address of uVxWorks image 
    DEFAULT     0x00100000
    _CFG_PARAMS INCLUDE_MEMORY_CONFIG 
}
#endif /* _WRS_CONFIG_FDT */

Parameter STACK_SAVE {
    NAME        stack size
    SYNOPSIS    Amount of stack to reserve (bytes) for rom code
    DEFAULT     0x40
}

Parameter RAM_LOW_ADRS {
    NAME        RAM low Address
    SYNOPSIS    The address at which to load VxWorks
    DEFAULT     0x01000
}

Parameter EXC_MSG_OFFSET {
    NAME        exception message offset
    SYNOPSIS    Exception message offset
    DEFAULT     0x800
}

Parameter EXC_MSG_ADRS {
    NAME        exception message address
    SYNOPSIS    Exception message address
    DEFAULT     (LOCAL_MEM_LOCAL_ADRS + EXC_MSG_OFFSET)
}

/* Components to support post-kernel hardware init */

#if (defined _WRS_CONFIG_ARCH_i86)
Component INCLUDE_DEFERRED_INTERRUPT_ENABLE {
    NAME            Deferred Interrupt Enable
    SYNOPSIS        Enable interrupts at appropriate point in root task
    _CHILDREN       FOLDER_NOT_VISIBLE
    HDR_FILES       private/intLibP.h
#ifndef _WRS_CONFIG_VXBUS_LEGACY
    PROTOTYPE void sysIntEnableFlagSet();
    INIT_RTN        intStartupUnlock (); sysIntEnableFlagSet();
#else /* _WRS_CONFIG_VXBUS_LEGACY */
    INIT_RTN        intStartupUnlock ();
#endif /* !_WRS_CONFIG_VXBUS_LEGACY */
    INCLUDE_WHEN    INCLUDE_KERNEL
}

Component INCLUDE_SYSHW_INIT_1 {
    NAME            Post-kernel BSP hardware initialization
    SYNOPSIS        call the BSPs sysHwInit1 routine during system startup
    _CHILDREN       FOLDER_NOT_VISIBLE
    HDR_FILES       sysLib.h
    INIT_RTN        sysHwInit1 ();
    INCLUDE_WHEN    INCLUDE_KERNEL
}
#endif /* (defined _WRS_CONFIG_ARCH_i86)     */

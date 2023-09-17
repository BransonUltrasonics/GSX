/* 20bsp.cdf - BSP component description file */

/*
 * Copyright (c) 2004-2006, 2008-2010, 2012-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
18aug16,mpc  added ARM64 support (F5261)
29sep16,g_x  adapt MMULESS for INCLUDE_CACHE_ARCH (F6411)
21apr16,emj  CERT: Update to produce partially linked objects (US79607)
15jan16,yya  added parameter of core0's CPUID. (US66795)
21nov15,l_z  Add fast warm boot require for PM. (F3387)
20oct14,cfm  fixed a typo in USER_D_CACHE_MODE (V7PRO-1313)
22aug14,sye  added system interrupt flag set for
             INCLUDE_DEFERRED_INTERRUPT_ENABLE. (V7PRO-1197)
25apr14,cww  moved SYS_MEM_DESC_NUM to boardlib
17feb14,cfm  removed DTB_START and added DTS_FILE
27jan14,yjw  add component INCLUDE_DEFERRED_INTERRUPT_ENABLE
20jan14,c_t  move some parameter from configAll.h to here.
22nov13,sye  added bsp common parameters.
15oct13,c_t  remove TGT_DIR
26aug13,cfm  code clean for vx7
09jan13,jdw  Add missing GNU intrinsics for VFP (WIND00397849)
14dec12,jdw  Modify INCLUDE_SW_FP component for backwards compatibility
             (WIND00391096)
21sep12,jdw  Use kernel math libraries for ARMARCH7
14mar12,cwl  remove component INCLUDE_VFP from ARM v4/v5. (WIND00274266)
06mar12,jdw  Remove dependancies on INCLUDE_SW_FP for Thumb2
16feb12,m_h  remove cacheAimArch6DClearDisable.o cacheAimArch6IMB.o
             from INCLUDE_CACHE_ARCH_ARMARCH7 (WIND00334017)
31aug10,fao  Add ARMARCH7_T2 support.
23aug10,fao  Add ARMARCH7 support.
13jan10,cwl  Remove the dummy function aimMmuShowInit() from the component
             INCLUDE_AIM_MMU_SHOW. (WIND00184839)
12aug09,cww  Updated aimMmuShowInit location in usrRoot
31may09,mdo  Add FASTMATH
04sep08,gls  divided task hook component
26aug08,jpb  Removed LAYER from components.
22nov06,j_b  add page table optimization support
17mar06,h_k  added INCLUDE_VFP. (SPR #118676)
03feb06,h_k  removed fpconst.o from SW_FP. (SPR #112232)
13oct05,yvp  INCLUDE_SW_SP gets pulled in for ANSI_STRTOD and ANSI_ATOF.
06oct05,h_k  changed INCLUDE_WHEN to _REQUIRES attribute in INCLUDE_SW_FP.
             (SPR #113387) added FP_TYPE as a _REQUIRES comp for SW_FP.
             (SPR #113210)
14sep05,h_k  moved ANSI math modules to ANSI_MATH. (SPR #112320)
08sep05,h_k  fixed missing dependency between MMU and Cache. (SPR #112080)
             removed cacheAim1136jfALib.o. (SPR #112139)
30aug05,h_k  added SELECT_MEM_ALLOC to CACHE_ARCH and MMU_ARCH_ARM1136.
12aug05,h_k  added BSP specific components for scalability.
22apr05,dr   SPR108100:adding aimFppLib.o and fppArchLib.o to INCLUDE_HW_FP
05nov04,rec  created

DESCRIPTION

This file contains updated descriptions for some vxWorks components
defined in 00bsp.cdf. It updates those definitions with new ones needed
to support the coprocessor abstraction.
*/

Component INCLUDE_COPROCESSOR {
        NAME            Coprocessor
        SYNOPSIS        Generalized coprocessor support
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         coprocLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        INIT_RTN        coprocLibInit();
        CONFIGLETTES    usrCoprocs.c
        _CHILDREN       FOLDER_PERIPHERALS
        REQUIRES        INCLUDE_KERNEL \
                        INCLUDE_TASK_CREATE_HOOKS \
                        INCLUDE_TASK_SWAP_HOOKS   \
                        INCLUDE_MEM_MGR_FULL    /* for calloc */
        _INIT_ORDER     usrIosCoreInit
}

Component INCLUDE_HW_FP {
        NAME            Hardware fpp support
        SYNOPSIS        Modified definitions for generalized coprocessor
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         fppArchLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        INIT_RTN
        HDR_FILES
        REQUIRES        INCLUDE_COPROCESSOR
        _CHILDREN       FOLDER_NOT_VISIBLE
}

#ifndef ARMARCH8A
/*
 * In order to allow projects from earlier releases of vxWorks to
 * migrate forward we need to manage backwards compatibility of the
 * INCLUDE_SW_FP component. The math library structure changed for
 * ARMARCH7 and ARMARCH7_T2 and no longer includes the dependencies
 * for INCLUDE_SW_FP when built with a VFP tool chain. To allow
 * projects from previous releases which still include the INCLUDE_SW_FP
 * component we define dependencies so they are only required
 * when the VSB selects a software FP toolchain. For ARMARCH7 and
 * ARMARCH7_T2 we also clear the inherited dependencies for VFP builds on
 * the INCLUDE_SW_FP component.
 */

Component INCLUDE_SW_FP {
        NAME            Software fpp support
        SYNOPSIS        ARM architecture dependent SW_FP
#if defined(_WRS_CONFIG_FP_soft)
        MODULES         mathSoftLib.o dccMathLib.o
        REQUIRES        INCLUDE_ARM_DIAB_INTRINSIC_SWFP_MATH \
                        INCLUDE_ARM_GNU_INTRINSIC_SWFP_MATH \
                        INCLUDE_IEEE_STATUS
#else /* defined(_WRS_CONFIG_FP_soft) */
        MODULES
        INIT_RTN
        HDR_FILES
#endif /* defined(_WRS_CONFIG_FP_soft) */
}
#endif /* ARMARCH8A */

Folder  FOLDER_VFP {
        NAME            Vector floating point support
        SYNOPSIS        Vector floating point support
        _CHILDREN       FOLDER_FPP
        CHILDREN        INCLUDE_VFP
        REQUIRES        INCLUDE_COPROCESSOR
}

Component INCLUDE_VFP {
        NAME            Vector floating point
        SYNOPSIS        High level math using VFP instructions
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         fppArchLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        LINK_SYMS       fppSave fppRestore
#ifndef ARMARCH8A
        REQUIRES        INCLUDE_COPROCESSOR INCLUDE_IEEE_STATUS \
                        INCLUDE_ARM_INTRINSIC_VFP_MATH
#else
	REQUIRES        INCLUDE_COPROCESSOR
#endif
}

Folder FOLDER_FPP {
     NAME        Floating point libraries

#ifndef ARMARCH8A
     CHILDREN    INCLUDE_SW_FP FOLDER_VFP
#else
     CHILDREN    FOLDER_VFP
#endif
}

Component INCLUDE_CACHE_ARCH {
        NAME            Cache function libraries
        SYNOPSIS        Architecture dependent portion of cache common libraries
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         cacheArchLib.o cacheALib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
#ifndef _WRS_CONFIG_MMULESS_KERNEL
        REQUIRES        SELECT_MEM_ALLOC        \
                        INCLUDE_CACHE_SUPPORT   \
                        INCLUDE_MMU_GLOBAL_MAP
#else
        REQUIRES        SELECT_MEM_ALLOC        \
                        INCLUDE_CACHE_SUPPORT

#endif
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_MMU_ARCH {
        NAME            MMU function librares
        SYNOPSIS        Architecture dependent portion of MMU common libraries
        REQUIRES        INCLUDE_MMU_GLOBAL_MAP  \
                        INCLUDE_CACHE_SUPPORT
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Parameter DATA_SEG_ALIGN {
        NAME            Kernel data segment alignment
        _CFG_PARAMS     INCLUDE_MEMORY_CONFIG
        DEFAULT         (INCLUDE_LOCK_TEXT_SECTION)::(0x00080000) \
                        1
}

Parameter LOCAL_MEM_SIZE {
        NAME        Local memory size
        SYNOPSIS    Fixed (static) memory size
#ifndef _WRS_CONFIG_COMPAT69_ADR_SPACE
        DEFAULT     KERNEL_SYS_MEM_RGN_SIZE
#else /* _WRS_CONFIG_COMPAT69_ADR_SPACE */
        DEFAULT     0x00400000
#endif /* !_WRS_CONFIG_COMPAT69_ADR_SPACE */    
}

/* Add INCLUDE_PAGE_SIZE_OPTIMIZATION to FOLDER_MMU */

Component INCLUDE_PAGE_SIZE_OPTIMIZATION {
        NAME            Optimize page tables at startup
        CONFIGLETTES    usrMmuOptimize.c
        INIT_RTN        usrMmuOptimize ();
        REQUIRES        INCLUDE_MMU_BASIC INCLUDE_MMU_OPTIMIZE
        HDR_FILES       vmLib.h
        _CHILDREN       FOLDER_MMU
}

Component INCLUDE_DEFERRED_INTERRUPT_ENABLE {
    NAME            Deferred interrupt enable
    SYNOPSIS        Enable interrupts at appropriate point in root task
    _CHILDREN       FOLDER_NOT_VISIBLE
#ifndef _WRS_CONFIG_VXBUS_LEGACY
    HDR_FILES       intLib.h subsys/int/vxbIntLib.h
    INIT_RTN        intCpuUnlock(0); \
                    sysIntEnableFlagSet ();
#else /* _WRS_CONFIG_VXBUS_LEGACY */
    HDR_FILES       intLib.h
    INIT_RTN        intCpuUnlock(0);
#endif /* !_WRS_CONFIG_VXBUS_LEGACY */
    INCLUDE_WHEN    INCLUDE_KERNEL
    _INIT_ORDER     usrRoot
#ifndef _WRS_CONFIG_VXBUS_LEGACY
    INIT_BEFORE     usrClkInit
#else /* _WRS_CONFIG_VXBUS_LEGACY */
    INIT_BEFORE     INCLUDE_SYSCLK_INIT
#endif /* !_WRS_CONFIG_VXBUS_LEGACY */
}

Parameter CONSOLE_BAUD_RATE {
        NAME            Baud rate of console port
        DEFAULT         115200
}

Parameter VEC_BASE_ADRS {
    NAME        Vector base address
    SYNOPSIS    Vector base address
    DEFAULT     (LOCAL_MEM_LOCAL_ADRS + 0x20000)
}

Component INCLUDE_EARLY_MMU {
    NAME        Early MMU support
    SYNOPSIS    Early MMU support before OS starts up.        
    CFG_PARAMS  STATIC_MMU_TABLE_BASE           \
                IMA_SIZE
    _CHILDREN   FOLDER_BSP_CONFIG
}

Parameter IMA_SIZE {
    NAME        IMA(Initial Mapped Area) size
    SYNOPSIS    Initial mapped area size.       
    DEFAULT     0x8000000
}

Parameter STATIC_MMU_TABLE_BASE {
    NAME        Static MMU table base address
    SYNOPSIS    Static MMU table base address for early MMU.
    TYPE        uint
    DEFAULT     (LOCAL_MEM_PHYS_ADRS + 0x00030000)
}

/*
 * DTB could be built into VxWorks kernel image by adding
 * INCLUDE_STANDALONE_DTB component. For this mode:
 *
 *  1. DTB couldn't be fixed by U-Boot automatically;
 *  2. vxWorks.bin should be used with "go <load address>" of U-Boot command to
 *  load the VxWorks kernel image;
 *  3. The load address should be KERNEL_LOAD_ADRS;
 *  4. DTS_FILE must be defined by BSP.
 */

Component INCLUDE_STANDALONE_DTB {
    NAME        Built-in DTB mode
    SYNOPSIS    Build DTB into VxWorks kernel image.
    CFG_PARAMS  DTS_FILE    
    _CHILDREN   FOLDER_BSP_CONFIG
}

Parameter EXC_MSG_OFFSET {
    NAME        Exception message offset
    SYNOPSIS    Exception message offset
    DEFAULT     0x1200
}

Parameter BOOT_LINE_SIZE {
    NAME        Boot line size (bytes)
    SYNOPSIS    Boot line size (bytes)
    DEFAULT     255
}

Parameter BOOT_LINE_OFFSET {
    NAME        Boot line offset
    SYNOPSIS    Boot line offset
    DEFAULT     0x1100
}
Parameter RESERVED {
    NAME        Reserved
    SYNOPSIS    Reserved
    DEFAULT     0
}
Parameter STACK_SAVE {
    NAME        Stack size
    SYNOPSIS    Amount of stack to reserve (bytes) for rom code
    DEFAULT     512
}

Parameter USER_D_CACHE_MODE {
    NAME        Data cache mode
    DEFAULT     CACHE_COPYBACK
}

Parameter USER_I_CACHE_MODE {
    NAME        Instruction cache mode
    DEFAULT     CACHE_COPYBACK
}

#ifndef _WRS_CONFIG_COMPAT69_ARM_ARCH
Component INCLUDE_EDR_PM {
    REQUIRES += INCLUDE_WARM_BOOT
}
#endif /* _WRS_CONFIG_COMPAT69_ARM_ARCH */

/*
 * CPUID of core0, this cannot be read from DTS because core0's ID
 * is referenced early in the boot process, before DTS is ready. 
 * CPUIDs for apcores are read from DTS.
 */

#if defined (_WRS_CONFIG_SMP)
Parameter	ARM_CPU0_INDEX {
    NAME        CPUID of core0
    SYNOPSIS    ID of core0 used in CPU identification function.
	_CFG_PARAMS INCLUDE_KERNEL
    DEFAULT	    0x0
}
#endif


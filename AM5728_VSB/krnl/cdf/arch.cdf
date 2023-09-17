/* arch.cdf - architecture component description file */

/*
 * Copyright (c) 2001, 2004-2008, 2010-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17jan17,lau  added scalb, cabs, copysign, logb, finite and drem
             under INCLUDE_ANSI_MATH (V7COR-4642)
16dec16,cfm  removed X mmu attribute in SYS_DMA32_MEM_ATTR and moved
             INCLUDE_PROTECT_INTERRUPT_STACK to FOLDER_KERNEL_HARDENING (F7142)
18aug16,mpc  added ARM64 support (F5261)
18aug16,cfm  invoking routines by pointers before early MMU enabled (V7PRO-3207)
21apr16,emj  CERT: Update to produce partially linked objects (US79607)
04jan16,gls  removed sigCtxLib.o from INCLUDE_SIGNAL_ARCH
14dec15,clt  Added asin for CERT (V7SP-223)
07sep15,yya  added monitor stub for warm reboot. (US63670)
10jul15,pfl  excluded __floatdidf and __floatdisf from cert (US63144)
08jul15,pfl  excluded rtpSigArchLib.o and vxLib.o from cert
12jun15,gls  removed routines from INCLUDE_ANSI_MATH for CERT
22jul15,v_r  Removed sinh/cosh/tanh from ANSI_MATH LINK_SYMS since they are not
             shipped (V7SP-276).
11mov14,yjw  added MMULESS_KERNEL support (US40548)
30oct14,cfm  added INCLUDE_ADJUST_DTB (V7PRO-1430)
23may14,yya  correct typo (V7PRO-881)
16may14,rlp  Removed WDB references (WB4-1735).
18oct13,c_t  remove windPwrLib for new vxbus.
15oct13,c_t  remove TGT_DIR
02sep13,zgl  added System DMA32 Memory Descriptor Attribute and mask
21sep12,jdw  Use kernel math libraries for ARMARCH7
06mar12,jdw  Remove dependancy on INCLUDE_SW_FP for Thumb2
24jan12,m_h  Force INCLUDE_VFP if VSB builds with vector floating point
             (WIND00277983)
16jan12,j_b  Remove RTP_OVERLAPPED_ADDRESS_SPACE as default for
             ARMv4/5/XScale due to performance (WIND00246765)
15dec11,rec  WIND00188552 - fix _CHILDREN line in INCLUDE_SIGNAL_ARCH
             WIND00313570 - Disable SW_MMU_ENABLE
28sep10,jdw  Fix INCLUDE_ANSI_MATH for Thumb2 default VSB
15sep10,jdw  Update INCLUDE_ANSI_MATH for Thumb2
26aug08,jpb  Removed LAYER from components.
14may07,zl   add ffsALib, bAlib and dllALib for source builds.
27jun06,pcs  Updated to add obj/class lib support in MKL.
30nov05,yvp  INCLUDE_SIGNAL_ARCH needs INCLUDE_ANSI_LONGJMP (SPR 115285)
11oct05,bpn  Removed dependency between the kernel module loader and the
             RTP loader component (SPR#113593).
29sep05,pee  Removed PROFILE_DEFAULT
14sep05,yvp  Added INCLUDE_SIGNAL_ARCH.
14sep05,h_k  added arch specific ANSI_MATH. (SPR #112320)
08sep05,h_k  excluded INCLUDE_GNU_INTRINSICS from PROFILE_DEFAULT.
             (SPR #112080)
07sep05,h_k  removed INCLUDE_BLIB_ARCH. (SPR #111769)
01sep05,h_k  added PROFILE_DEFAULT.
31aug05,h_k  added SELECT_MEM_ALLOC in KERNEL_ARCH.
23aug05,jmt  Move windPwrLib.o from INCLUDE_KERNEL to INCLUDE_KERNEL_ARCH
11aug05,h_k  added arch specific components for lower layer.
10aug05,pee  added KERNEL_ARCH and BLIB_ARCH placeholder
29mar05,rec  Change TASK_STACK_OVERFLOW_SIZE to
             TASK_USER_EXC_STACK_OVERFLOW_SIZE
30aug04,job  Renamed INCLUDE_LOADER to INCLUDER_LOADER_CODE (SPR#101151)
08nov01,jn   Switched to ELF loader, added modification history and
             WindRiver headers
*/

/*
DESCRIPTION
The Tornado Project Facility sources all component description files (CDF)
in a particular order.  The CDF files in these directories are processed in
the order in which they are presented: comps/vxWorks,
comps/vxWorks/arch/<arch>, config/<bsp>, and the project directory.  If
two CDF files describe the same component, the file read last overrides
all earlier files.  As a result, CDF files processed later by the Project
Facility are said to have a higher precedence than those processed
earlier.  Given such functionality, this file contains
architecture-specific VxWorks components and component definitions which
override "generic" components defined in comps/vxWorks.

If a new component is released, a new .cdf file should be deposited
in the appropriate directory, rather than modifying existing files.

For more information, see the .I "Tornado BSP Developer's Kit for VxWorks
User's Guide, Tornado 2.0: Components".
*/

/*
This cdf file defines the ARM arch specific component units.
*/

Component INCLUDE_OSM {
        NAME            Overflow Stopgap Mode
        SYNOPSIS        Handler for Exception Stack Overflow
        PROTOTYPE       void excOsmInit (int, int);
        INIT_RTN        excOsmInit (TASK_USER_EXC_STACK_OVERFLOW_SIZE, VM_PAGE_SIZE);
        _CHILDREN       FOLDER_NOT_VISIBLE
        REQUIRES        INCLUDE_MMU_BASIC
        INCLUDE_WHEN    INCLUDE_PROTECT_TASK_STACK
        _INIT_ORDER     usrRoot
        INIT_AFTER      INCLUDE_MMU_BASIC
        INIT_BEFORE     INCLUDE_PROTECT_TASK_STACK
}

Component INCLUDE_PROTECT_INTERRUPT_STACK {
        _CHILDREN       FOLDER_KERNEL_HARDENING
        INCLUDE_WHEN    INCLUDE_KERNEL_HARDENING
        _INIT_ORDER     usrRoot
        REQUIRES        INCLUDE_OSM
        INIT_AFTER      INCLUDE_OSM
        INIT_BEFORE     INCLUDE_PROTECT_TASK_STACK
}

Component INCLUDE_LOADER_CODE {
        SYNOPSIS        ELF loader
        MODULES         loadLib.o loadElfLib.o
        INIT_RTN        loadElfInit ();
        HDR_FILES       loadElfLib.h
}

Component INCLUDE_RTP_ARCH {
        NAME            Arch-specific component for RTPs
        _CHILDREN       FOLDER_NOT_VISIBLE
        REQUIRES        INCLUDE_RTP
#ifndef _WRS_CONFIG_CERT
        MODULES         syscallArchLib.o syscallALib.o \
                        rtpALib.o rtpSigArchLib.o
#else
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         syscallArchLib.o syscallALib.o \
                        rtpALib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
#endif
}

Component INCLUDE_KERNEL_ARCH {
        NAME            kernel
        SYNOPSIS        architecture dependent portion of the kernel component
#ifndef _WRS_CONFIG_COMPAT69_ARM_ARCH
#ifndef _WRS_CONFIG_CERT
        MODULES         ffsALib.o intALib.o intArchLib.o excALib.o \
                        taskArchLib.o vxLib.o vxALib.o windALib.o \
                        ffsLib.o
#else
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         ffsALib.o intALib.o intArchLib.o excALib.o \
                        taskArchLib.o vxALib.o windALib.o ffsLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
#endif
#else
        MODULES         ffsALib.o intALib.o intArchLib.o excALib.o \
                        taskArchLib.o vxLib.o vxALib.o windALib.o \
                        ffsLib.o windPwrLib.o

#endif
        REQUIRES        SELECT_MEM_ALLOC
        _CHILDREN       FOLDER_NOT_VISIBLE
}

#ifndef _WRS_CONFIG_COMPAT69_ARM_ARCH
Component INCLUDE_CPU_INIT_ARCH {
        SYNOPSIS        ARM arch cpu initialization
        _INIT_ORDER     usrInit
#ifndef _WRS_CONFIG_MMULESS_KERNEL
#ifndef ARMARCH8A
        INIT_RTN          \
                        { \
                        void (* cpuInit) (void) = vxCpuInit; \
                        void (* mmuEarlyInit) (UINT32, UINT32, UINT32) = vxMmuEarlyInit; \
                        cpuInit = (void (*) (void)) ((uintptr_t) cpuInit + LOCAL_MEM_PHYS_ADRS); \
                        mmuEarlyInit = (void (*) (UINT32, UINT32, UINT32)) ((uintptr_t) mmuEarlyInit + LOCAL_MEM_PHYS_ADRS); \
                        cpuInit (); \
                        mmuEarlyInit (STATIC_MMU_TABLE_BASE, LOCAL_MEM_LOCAL_ADRS, IMA_SIZE); \
                        excVecBaseSet ((UINT32)VEC_BASE_ADRS); \
                        }
#else /* ARMARCH8A */
	INIT_RTN        vxCpuInit ();   \
                        vxMmuEarlyInit (STATIC_MMU_TABLE_BASE, LOCAL_MEM_LOCAL_ADRS, IMA_SIZE);
#endif /* !ARMARCH8A */

#else  /* !_WRS_CONFIG_MMULESS_KERNEL */
#ifndef ARMARCH8A
        INIT_RTN        vxCpuInit ();   \
                        excVecBaseSet ((UINT32)VEC_BASE_ADRS);
#else /* ARMARCH8A */
        INIT_RTN        vxCpuInit ();
#endif /* !ARMARCH8A */
#endif /* _WRS_CONFIG_MMULESS_KERNEL */
        INIT_BEFORE     INCLUDE_MONITOR_STUB
#ifndef ARMARCH8A
        PROTOTYPE       IMPORT void vxCpuInit (void);   \
                        IMPORT VOID vxMmuEarlyInit (UINT32 baseAddr, UINT32 linkAddr, UINT32 length); \
                        IMPORT void excVecBaseSet (UINT32 base);
#else
	PROTOTYPE       IMPORT void vxMmuEarlyInit (ULONG baseAddr, ULONG linkAddr, ULONG); \
			IMPORT void vxCpuInit (void);  
#endif
        _CHILDREN       FOLDER_NOT_VISIBLE
        _REQUIRES       INCLUDE_KERNEL_ARCH
}

Component INCLUDE_MONITOR_STUB {
        SYNOPSIS        monitor stub (ARCH internal usage)
        _INIT_ORDER     usrInit
        INIT_RTN        armMonitorStub(startType);
        CONFIGLETTES    armMonitorStub.c
        INIT_AFTER      INCLUDE_CPU_INIT_ARCH
        INIT_BEFORE     INCLUDE_SYS_START
        _CHILDREN       FOLDER_NOT_VISIBLE
        _REQUIRES       INCLUDE_KERNEL_ARCH
}

Component INCLUDE_ADJUST_DTB {
        SYNOPSIS        adjust the dtb address (ARCH internal usage)
        _INIT_ORDER     usrInit
        INIT_RTN        usrAdjustDtb ();
        CONFIGLETTES    usrAdjustDtb.c
        INIT_AFTER      INCLUDE_CPU_INIT_ARCH
        INIT_BEFORE     INCLUDE_FDT_LIB
        _CHILDREN       FOLDER_NOT_VISIBLE
        _REQUIRES       INCLUDE_KERNEL_ARCH
}

#endif

/*
 * SW emulated MMU is not a choice for ARM.  If the board has an MMU, it
 * must be used to get cache support.  By default, SW_MMU_ENABLE is FALSE.
 * Boards that require SW_MMU_ENABLE (such as boards with an MPU) should
 * define this macro in config.h.
 */

Component INCLUDE_MMU_BASIC {
        CFG_PARAMS      -= SW_MMU_ENABLE
}

#ifndef ARMARCH8A
Component INCLUDE_DLL_ARCH {
        NAME            arch dependent portion of doubly linked lists
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         dllALib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        REQUIRES        INCLUDE_DLL
        _REQUIRES       INCLUDE_DLL
        _CHILDREN       FOLDER_NOT_VISIBLE
}
#endif  /* ARMARCH8A */

Component INCLUDE_BLIB_ARCH {
        NAME            arch dependent portion of buffer manipulation library
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         bALib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        REQUIRES        INCLUDE_BLIB
        _REQUIRES       INCLUDE_BLIB
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_OBJ_LIB_ARCH {
        NAME            object management
        SYNOPSIS        architecture dependent portion of OBJ_LIB
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         objALib.o objArchLib.o
#else
        _REQUIRES INCLUDE_KERNEL
#endif
        REQUIRES        INCLUDE_OBJ_LIB
        INCLUDE_WHEN    INCLUDE_OBJ_LIB
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_ANSI_MATH {
        NAME            ANSI math
        SYNOPSIS        ANSI mathematical functions
#ifndef _WRS_CONFIG_CERT
        LINK_SYMS       acos asin atan atan2 cabs ceil copysign cos cosh drem \
                        exp fabs finite floor fmod frexp ldexp log log10 \
                        logb modf pow scalb sin sinh sqrt tan tanh
#else /* _WRS_CONFIG_CERT */
        LINK_SYMS       acos asin atan atan2 ceil cos exp fabs floor fmod frexp     \
                        ldexp log log10 modf pow sin sqrt tan
#endif /* _WRS_CONFIG_CERT */
        HELP            ansiMath
}

/*
 * Make sure project is built with vector floating point
 * if VSB is built with vector floating point
 */

#ifdef _WRS_CONFIG_FP_vector
Component INCLUDE_KERNEL_ARCH {
        REQUIRES += INCLUDE_VFP
}
#endif

Component INCLUDE_SIGNAL_ARCH {
        NAME            architecture dependent code for signals
        SYNOPSIS        architecture dependent code for signals
#ifndef _WRS_CONFIG_CERT_KERNEL_OBJECT
        MODULES         sigCtxALib.o
#endif
        REQUIRES        INCLUDE_KERNEL \
                        INCLUDE_ANSI_LONGJMP
        INCLUDE_WHEN    INCLUDE_SIGNALS
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Parameter INT_LOCK_LEVEL {
        NAME            INT lock level
        SYNOPSIS        Interrupt lock level
        DEFAULT         0x0
}

Parameter ROOT_STACK_SIZE {
        NAME            Root stack size
        SYNOPSIS        Root task stack size (bytes)
        DEFAULT         0x4000
}

Parameter SHELL_STACK_SIZE {
        NAME            Shell stack size
        SYNOPSIS        Shell stack size (bytes)
        DEFAULT         0x10000
}

Parameter ROM_WARM_ADRS {
        NAME            ROM warm boot address
        SYNOPSIS        ROM warm boot address
        DEFAULT         (ROM_TEXT_ADRS + 4)
}

Parameter VM_PAGE_SIZE {
        NAME            VM page size
        SYNOPSIS        Virtual memory page size (bytes)
        DEFAULT         4096
}

Parameter SYS_DMA32_MEM_ATTR {
        NAME            System DMA32 Memory Descriptor Attribute
        SYNOPSIS        VM attribute for DMA32 memory descriptors used to provide
                        DMA memory block below 4G physical address if
                        INCLUDE_DMA32_MEM is defined.
        DEFAULT         MMU_ATTR_VALID | MMU_ATTR_SUP_RW | \
                        MMU_ATTR_CACHE_OFF | MMU_ATTR_CACHE_GUARDED
        _CFG_PARAMS     INCLUDE_MEMORY_CONFIG
}

Parameter SYS_DMA32_MEM_ATTR_MASK {
        NAME            System DMA32 Memory Descriptor Attribute Mask
        SYNOPSIS        VM attribute mask for DMA32 memory descriptors used to
                        provide DMA memory block below 4G physical address if
                        INCLUDE_DMA32_MEM is defined.
        DEFAULT         MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | \
                        MMU_ATTR_CACHE_MSK
        _CFG_PARAMS     INCLUDE_MEMORY_CONFIG
}

#ifndef __llvm__
Component INCLUDE_ARM_GNU_INTRINSIC_SWFP_MATH {
        NAME            GNU Intrinsic SW FP functions
        SYNOPSIS        GNU Intrinsic SW FP functions
        LINK_SYMS       __adddf3 __subdf3 __muldf3 __divdf3 \
                        __negdf2 __eqdf2 __nedf2 __gtdf2 __gedf2 __ltdf2 \
                        __ledf2 __addsf3 __subsf3 __mulsf3 __divsf3 __negsf2 \
                        __eqsf2 __nesf2 __gtsf2 __gesf2 __ltsf2 __lesf2 \
                        __extendsfdf2 __truncdfsf2 __fixdfsi __fixsfsi \
                        __fixdfdi __fixsfdi __fixunsdfsi __fixunssfsi \
                        __fixunsdfdi __fixunssfdi __floatsidf __floatsisf \
                        __floatdidf __floatdisf
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_ARM_DIAB_INTRINSIC_SWFP_MATH {
        NAME            Diab Intrinsic SW FP functions
        SYNOPSIS        Diab Intrinsic SW FP functions
        LINK_SYMS       _d_add _d_sub _d_mul _d_div _f_add _f_sub \
                        _f_mul _f_div _d_feq _d_fne _d_fgt _d_fge \
                        _d_flt _d_fle _f_feq _f_fne _f_fgt _f_fge \
                        _f_flt _f_fle _f_ftod _d_dtof _d_itod _d_utod \
                        _d_dtoi _d_dtou _f_itof _f_utof _f_ftoi \
                        _f_ftou _d_lltod _d_ulltod _d_dtoll _d_dtoull \
                        _f_lltof _f_ulltof _f_ftoll _f_ftoull _d_fnlt \
                        _d_fngt _d_fnle _d_fnge _f_fnlt _f_fngt _f_fnle \
                        _f_fnge
       _CHILDREN       FOLDER_NOT_VISIBLE
}

/*
 * These intrinsic functions are required to support the GNU compiler when
 * the kernel is built with VFP. The GNU APIs are provided as wrappers around
 * the equivalent Diab APIs.
 *
 * In the future the GNU library provided by the compiler should support these
 * APIs and they can be removed from here.
 */

Component INCLUDE_ARM_INTRINSIC_VFP_MATH {
        NAME            Kernel Intrinsic FP functions
        SYNOPSIS        Intrinsic FP functions provided by the kernel
#ifndef ARMARCH7M
#ifndef _WRS_CONFIG_CERT
        LINK_SYMS       _d_lltod _f_lltof  \
                        __floatdidf __floatdisf
#else
        LINK_SYMS       _d_lltod _f_lltof
#endif
#endif
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_IEEE_STATUS {
        NAME            IEEE Status
        SYNOPSIS        API to get FP calculation status
        LINK_SYMS       __ieee_status
        _CHILDREN       FOLDER_NOT_VISIBLE
}
#endif /* !__llvm__ */


/* mmuArmLib.h - ARM common MMU library header file */

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
16dec16,cfm  added PTE_ATTR_XN (F7142)
08oct16,wqi  added the cortex-a15 erratum 798181 workaround (V7PRO-3345)
31mar16,cfm  corrected the defination of MMU_STATE_SECTION_DEVICE_SHARED (V7PRO-2581)
29oct15,cfm  add MMU_STATE_STRONGLY_ORDERED_SHARED definition and use for device memory
09oct15,cfm  corrected MMU_STATE_MASK_VALID (V7PRO-2464)
12aug15,cfm  dynamically get TTBR flags and page table attributes for generic
             cpu (V7PRO-2305)
12may15,m_w  updated attribute of MMU_ATTR_CACHE_OFF from normal none-cache
             to strongly-order. 
             added MMU_ATTR_SPL_1 for supportting device type. (V7PRO-2032)
03sep14,yjw  fixed translate table cache attribute is incorrect for 
             cortex-a5 and cortex-a9 (US40545)
07jul14,c_t  fixed MMU_STATE_INVALID_STATE defining error. (V7PRO-1013)
15may14,cfm  fixed a performance issue for cortex-A8 (V7PRO-820)
05may14,cfm  corrected cortex a8 page table attributes (V7PRO-772)
24apr14,cfm  removed USE_OPTIMIZE_API and ENABLE_MINI_HEAP. (V7PRO-679)
17apr14,cfm  updated the TTBR/PTE flags (VXW7-2982)
30dec13,yjw  remove unsupported arch
13nov13,zgl  derived from mmuArmArch6PalLib.h
*/

#ifndef __INCmmuArmLibh
#define __INCmmuArmLibh

/* includes */

#include <private/vmLibP.h>
#include <memLib.h>
#include <semLib.h>

#ifdef __cplusplus
     extern "C" {
#endif

/* defines */

/*
 * The page size we will use. Ignore the sub-page, Large Page and Tiny
 * Page features.
 */

#define MMU_PAGE_SIZE 4096
#define MMU_PAGE_SIZE_SHIFT 12

#define EARLY_MMU_TABLE_SIZE 0x4000 /* Early MMU table is 16 KBytes */

/*
 * See the Virtual Memory System Architecture for ARMv6 (VMSAv6) section
 * in the ARM Architecture Reference Manual (ARM DDI 0100) for MMU details.
 */

#define PAGE_SIZE            MMU_PAGE_SIZE

#undef INCLUDE_MMU_APXBIT_SUPPORT
#define INCLUDE_MMU_APXBIT_SUPPORT

#undef INCLUDE_MMU_XNBIT_SUPPORT
#define INCLUDE_MMU_XNBIT_SUPPORT

#undef INCLUDE_MMU_EXTD_TYPE_SUPPORT
#define INCLUDE_MMU_EXTD_TYPE_SUPPORT

#undef INCLUDE_MMU_SBIT_SUPPORT
#define INCLUDE_MMU_SBIT_SUPPORT

#undef INCLUDE_MMU_TEX_SUPPORT
#define INCLUDE_MMU_TEX_SUPPORT

/*
 * The amount described by a Level 1 Descriptor, which equals the smallest
 * amount of VM allocatable in VxWorks.
 */

#define MMU_PAGE_BLOCK_SIZE 0x100000    /* 1 MByte */

#define MMU_ADRS_TO_L1D_INDEX_SHIFT 20

#define MMU_PTE_INDEX_MASK  0x000FF000  /* extract PTE index from Virt Addr */
#define MMU_PTE_INDEX_SHIFT 12          /* shift to make that into PTE index */

#define MMU_L1D_TO_BASE_SHIFT 10            /* to turn L1D to PT base addr */
#define MMU_L1DSECTION_TO_BASE_SHIFT 20     /* L1D section to PT base addr */
#define MMU_L1DSECTION_BA_MASK  0xFFF00000  /* phys bits 20-31 */

#define MMU_L1DSSECTIONLO_TO_BASE_SHIFT 24  /* bits 24-31 of supersection */
#define MMU_L1DSSECTIONHI_TO_BASE_SHIFT 32  /* bits 32-35 of supersection */
#define MMU_L1DSSECTIONHI2_TO_BASE_SHIFT 5  /* bits 36-39 of supersection */
#define MMU_L1DSSECTIONLO_BA_MASK  0xFF000000 /* phys bits 24-31 */
#define MMU_L1DSSECTIONHI_BA_MASK  0x00F00000 /* phys bits 32-35 */
#define MMU_L1DSSECTIONHI2_BA_MASK 0x000001E0 /* phys bits 36-39 */

/* optional extended physical address bit fields */

#define MMU_L1DSSECTION_EXTD_BA_MASK  \
            (MMU_L1DSSECTIONHI_BA_MASK | MMU_L1DSSECTIONHI2_BA_MASK)

/* supersection base address packing */

#define MMU_L1DSSECTION_BA_SET(physAddr, L1desc) \
        (L1desc = ((L1desc & ~MMU_L1DSSECTION_EXTD_BA_MASK) | \
            (unsigned long)((((unsigned long long)(physAddr)) & \
                             MMU_L1DSSECTIONLO_BA_MASK) | \
                            ((((unsigned long long)(physAddr)) >> 12) & \
                             MMU_L1DSSECTIONHI_BA_MASK) | \
                            ((((unsigned long long)(physAddr)) >> 31) & \
                             MMU_L1DSSECTIONHI2_BA_MASK))))

/* supersection base address unpacking */

#define MMU_L1DSSECTION_BA_GET(ssectDesc)                                     \
        ((unsigned long long)                                                 \
        ((unsigned long long)(ssectDesc & MMU_L1DSSECTIONLO_BA_MASK) |        \
        ((unsigned long long)(ssectDesc & MMU_L1DSSECTIONHI_BA_MASK) << 12) | \
        ((unsigned long long)(ssectDesc & MMU_L1DSSECTIONHI2_BA_MASK)<< 31)))

/* Address translation masks */

#define MMU_ADDR_PI_MASK     0x00000FFF /* extract page index from Virt Addr */
#define MMU_PTE_BA_MASK      0xFFFFF000 /* extract phys base address from PTE */
#define MMU_ADDR_SI_MASK     0x000FFFFF /* extr section index from Virt Addr */
#define MMU_PTE_SECA_MASK    0xFFF00000 /* extract section base from L1 desc */

#define MMU_ADDR_SSI_MASK    0x00FFFFFF /* supersection index from virt */
#define MMU_PTE_SSECA_MASK   0xFF000000 /* supersection base from L1 desc */
#define MMU_PTE_SSECAX_MASK  0x00F00000 /* supersection extended bits L1 */
#define MMU_PTE_SSECAX_SHIFT 12         /* supersection extended bits shift */

/* Level 1 descriptor bit fields */

#define MMU_L1PD_TYPE_MASK   0x00000003 /* type bits [0:1] */
#define MMU_L1PD_CB_MASK     0x0000000C /* CB bits [3:2] */
#define MMU_L1PD_XN_MASK     0x00000010 /* eXecute Never bit [4] */
#define MMU_L1PD_DOMAIN_MASK 0x000001E0 /* domain bits [8:5] */
#define MMU_L1PD_P_MASK      0x00000200 /* P bit [9] */
#define MMU_L1PD_AP_MASK     0x00000C00 /* AP bits [10:11] */
#define MMU_L1PD_TEX_MASK    0x00007000 /* TEX bits [14:12] */
#define MMU_L1PD_APX_MASK    0x00008000 /* APX bit [15] */
#define MMU_L1PD_S_MASK      0x00010000 /* Shared bit [16] */
#define MMU_L1PD_NG_MASK     0x00020000 /* not-Global bit [17] */
#define MMU_L1PD_PTE_MASK    0xFFFFFC00 /* extract phys L2 PTE addr from L1PD */
#define MMU_L1PD_S           MMU_L1PD_S_MASK

/* # of shifts (right) to get fields in Level 1 descriptor */

#define MMU_L1PD_CB_SHIFT      2
#define MMU_L1PD_XN_SHIFT      4
#define MMU_L1PD_DOMAIN_SHIFT  5
#define MMU_L1PD_P_SHIFT       9
#define MMU_L1PD_AP_SHIFT      10
#define MMU_L1PD_TEX_SHIFT     12
#define MMU_L1PD_APX_SHIFT     15
#define MMU_L1PD_S_SHIFT       16
#define MMU_L1PD_NG_SHIFT      17

/* Level 2 descriptor bit fields */

#define MMU_PTE_TYPE_MASK       0x00000003 /* PTE type bits [0:1] */
#define MMU_PTE_SECT_XN_MASK    0x00000001 /* eXecute Never bit [0] */
#define MMU_PTE_SECT_CB_MASK    0x0000000C /* CB bits [3:2] */
#define MMU_PTE_SECT_AP_MASK    0x00000030 /* AP bits [5:4] */
#define MMU_PTE_SECT_TEX_MASK   0x000001C0 /* TEX bits [8:6] */
#define MMU_PTE_SECT_APX_MASK   0x00000200 /* APX bit [9] */
#define MMU_PTE_SECT_S_MASK     0x00000400 /* Shared bit [10] */
#define MMU_PTE_SECT_NG_MASK    0x00000800 /* not-Global bit [11] */

/* adjustment to translate fields: Level 1 <-> Level 2 descriptors */

#define MMU_L1L2_SECT_NG_SHIFT   6 /* nG-bit shift convert L1 to L2 */
#define MMU_L1L2_SECT_S_SHIFT    6 /* S-bit shift convert L1 to L2 */
#define MMU_L1L2_SECT_TEX_SHIFT  6 /* TEX shift convert L1 to L2 */
#define MMU_L1L2_SECT_AP_SHIFT   6 /* AP, APX shift convert L1 to L2 */
#define MMU_L1L2_SECT_XN_SHIFT   4 /* XN-bit shift convert L1 to L2 */
#define MMU_L1L2_SECT_CB_SHIFT   0 /* CB shift convert L1 to L2 */

/* Section and Supersection type and size macros */

#define MMU_PTE_SSECT_TYPE          0x00040000 /* supersection bit [18] */
#define MMU_PTE_SECT_TYPE_MASK      0x00040000 /* which type of section? */

#define MMU_SECT_SIZE               0x100000    /* section = 1MB */
#define MMU_SSECT_SIZE              0x1000000   /* supersection = 16MB */

#define MMU_NUM_VM_PAGES_IN_A_SECT  0x100       /* 1MB = 256 * 4KB pages */
#define MMU_NUM_VM_PAGES_IN_A_SSECT 0x1000      /* 16MB = 4096 * 4KB pages */
#define MMU_NUM_SECTS_IN_A_SSECT    0x10        /* 16 sections/supersection */

/*
 * The Domain Access Control Register specifies the way in which access
 * rights are treated for each of sixteen domains. For each domain the
 * current state can be:
 *
 *   00 no access: all accesses generate aborts
 *   01 client:    access rights specified in page tables are checked.
 *   10 reserved
 *   11 manager:  accesses not checked against page tables: all access allowed.
 */

#define MMU_DACR_VAL_NORMAL         1       /* client of 0 */
#define MMU_DACR_VAL_UNPROTECT      0x03    /* manager of 0 */

/* Level 1 Descriptor types */

#define MMU_DESC_TYPE_FAULT         0       /* any access will cause a fault */
#define MMU_DESC_TYPE_PAGE          1
#define MMU_DESC_TYPE_SECTION       2

#define MMU_DEF_L1_PAGE     0x00000001      /* domain zero, Page descriptor */

/*
 * Section descriptors, such as might be used to set up an intermediate
 * set of page tables on processors such as SA-1100/SA-1500 where this
 * needs to be done from BSP initialization, before vm(Base)Lib sets up
 * a proper set of page tables.
 */

#define MMU_RAM_SECT_DESC       0xC1E   /* R/W cacheable bufferable domain 0 */
#define MMU_MINICACHE_SECT_DESC 0xC1A   /* R/W C+ B- domain 0 */
#define MMU_OTHER_SECT_DESC 0xC12   /* R/W C- B- domain 0 */

/* Level 2 Descriptor or Page Table Entry (PTE) types */

#define MMU_PTE_TYPE_FAULT   0              /* any access will cause a fault */
#define MMU_PTE_TYPE_LG_PAGE 1              /* Large page descriptor */
#define MMU_PTE_TYPE_SM_PAGE 2              /* Small page descriptor */
#define MMU_PTE_TYPE_EX_PAGE 3              /* Extended page descriptor */

/*
 * The Translation Table Base register (TTBR) points to a table of Level 1
 * Descriptors. these are either Invalid Section descriptors, Section
 * Descriptors, or Page Table Descriptors. If Page Table Descriptors, they
 * each point to a table of Level 2 Page Descriptors, or Page Table Entries
 * (PTEs).
 * The 32-bit (virtual) address space allows for 4096 M. Each Level 1
 * Descriptor describes a 1 M area of memory. There are therefore 4096 Level
 * 1 Descriptors, and each table of 256 Level 2 Page Descriptors (PTEs)
 * describes 256 4 kbyte pages.
 */

#define MMU_NUM_L1_DESCS 4096
#define MMU_NUM_L2_DESCS 256
#define MMU_L1_TABLE_SIZE (MMU_NUM_L1_DESCS * sizeof(MMU_LEVEL_1_DESC))

/* No. of pages a Level 1 Descriptor table takes up */

#define MMU_L1_DESC_PAGES (MMU_L1_TABLE_SIZE / MMU_PAGE_SIZE)

/* Size of a Page Table */

#define MMU_PAGE_TABLE_SIZE (MMU_NUM_L2_DESCS * sizeof(PTE))

/* Number of possible ASIDs */

#define MMU_NUM_ASIDS        256
#define VXWORKS_KERNEL_ASID  0    /* reserved ASID for VxWorks kernel */

/* Section and supersection L1 definitions follow */

#undef INCLUDE_MMU_SUPERSECTION_SUPPORT  /* support supersection descriptors */
#define INCLUDE_MMU_SUPERSECTION_SUPPORT

#undef MMU_DEF_L1_SECTION_PAGE
#define MMU_DEF_L1_SECTION_PAGE 0x00000002  /* Section page descriptor */

#undef MMU_DEF_L1_SUPERSECTION_PAGE
#define MMU_DEF_L1_SUPERSECTION_PAGE 0x00040002  /* supersection */

#undef MMU_DEF_L1_SECTION_PAGE_MASK
#define MMU_DEF_L1_SECTION_PAGE_MASK 0x00040003  /* mask */

#undef MMU_STATE_SECTION_MASK_WRITABLE
#define MMU_STATE_SECTION_MASK_WRITABLE 0x00000030      /* All APn bits */

#define MMU_STATE_SECTION_SUP_RW 0x00000400 /* AP[2]:0 AP[1:0]:01 */
#define MMU_STATE_SECTION_WRITEBACK 0x0000000C
#define MMU_STATE_SECTION_NONCACHE 0x00001000
#define MMU_STATE_SECTION_STRONGLY_ORDERED 0x0
#define MMU_STATE_SECTION_DEVICE_SHARED 0x00000004

/*
 * Architecture-dependent MMU states. These are states settable for pages and
 * here they correspond to settings in the Page Table Entries (PTEs).
 *
 * We set Valid/Invalid by setting a Fault second-level descriptor rather
 * than by using the Access Permissions within a small page second-level
 * descriptor. This is because we will want to use the Access Permissions to
 * control supervisor mode access and we cannot then use the AP bits
 * to disallow access as the SR bits in the MMU Control register must be set
 * to 10 in order to control read/write access from Supervisor mode.
 */

#define MMU_STATE_INVALID_STATE     0xFFFFFFFF

#define MMU_STATE_MASK_VALID        0x00000002  /* 2nd lvl desc bit 1:0 */

#define MMU_STATE_MASK_WRITABLE     0x00000230  /* All APn bits */

#define MMU_STATE_MASK_CACHEABLE    0x000005CC  /* S, TEX[0:2] + CB bits */

#define MMU_STATE_VALID             MMU_PTE_TYPE_SM_PAGE /* set to page type */
#define MMU_STATE_VALID_NOT         MMU_PTE_TYPE_FAULT  /* set to type fault */

#define MMU_STATE_MASK_PROTECTION   0x00000231      /* APX + APn + XN bits */

#define MMU_STATE_SUP_RO            0x00000210      /* APX: 1 AP: 01 */
#define MMU_STATE_SUP_RW            0x00000010      /* APX: 0 AP: 01 */
#define MMU_STATE_SUP_RO_USR_RO     0x00000220      /* APX: 1 AP: 10 */
#define MMU_STATE_SUP_RW_USR_RO     0x00000020      /* APX: 0 AP: 10 */
#define MMU_STATE_SUP_RW_USR_RW     0x00000030      /* APX: 0 AP: 11 */

#define MMU_STATE_WRITABLE          MMU_STATE_SUP_RW

#define MMU_STATE_WRITABLE_NOT      MMU_STATE_SUP_RO

#define MMU_STATE_AP_SUP_RO         0x1      /* AP field: 01 */
#define MMU_STATE_AP_SUP_RW         0x1      /* AP field: 01 */
#define MMU_STATE_AP_SUP_RO_USR_RO  0x2      /* AP field: 10 */
#define MMU_STATE_AP_SUP_RW_USR_RO  0x2      /* AP field: 10 */
#define MMU_STATE_AP_SUP_RW_USR_RW  0x3      /* AP field: 11 */

#define MMU_STATE_AP_USR_RO_SUP_RO  MMU_STATE_AP_SUP_RO_USR_RO
#define MMU_STATE_AP_USR_RO_SUP_RW  MMU_STATE_AP_SUP_RW_USR_RO
#define MMU_STATE_AP_USR_RW_SUP_RW  MMU_STATE_AP_SUP_RW_USR_RW

/* The Execute-Never bit is an extra permission check on readable addresses */

#define MMU_STATE_EXE_NOT           0x00000001  /* Execute-Never, XN bit */

#define MMU_STATE_SUP_RO_XN        (MMU_STATE_SUP_RO | MMU_STATE_EXE_NOT)
#define MMU_STATE_SUP_RW_XN        (MMU_STATE_SUP_RW | MMU_STATE_EXE_NOT)
#define MMU_STATE_SUP_RO_USR_RO_XN (MMU_STATE_SUP_RO_USR_RO | MMU_STATE_EXE_NOT)
#define MMU_STATE_SUP_RW_USR_RO_XN (MMU_STATE_SUP_RW_USR_RO | MMU_STATE_EXE_NOT)
#define MMU_STATE_SUP_RW_USR_RW_XN (MMU_STATE_SUP_RW_USR_RW | MMU_STATE_EXE_NOT)

/*
 * Memory Region Attributes -
 * cache access, write buffer access, coherency control
 */

#define MMU_STATE_STRONGLY_ORDERED  0x0        /* strongly ordered, shared */
#define MMU_STATE_STRONGLY_ORDERED_SHARED       \
         MMU_STATE_STRONGLY_ORDERED | MMU_STATE_SHARED

#define MMU_STATE_DEVICE_SHARED     0x00000004 /* device type, shared */
                                               /* (bufferable, not cacheable) */

/* outer & inner write-thru cachable, no write alloc */

#define MMU_STATE_NORMAL_WRITETHROUGH     0x00000008
#define MMU_STATE_CACHEABLE_WRITETHROUGH  0x00000008  /* backward compatible */

#define MMU_STATE_NORMAL_WRITEBACK        0x0000000C  /* write-back, no alloc */
#define MMU_STATE_CACHEABLE_COPYBACK      0x0000000C  /* backward compatible */

#define MMU_STATE_NORMAL_NONCACHEABLE     0x00000040  /* non-cached, sharable */

#define MMU_STATE_DEVICE_NONSHARED        0x00000080  /* device, non-shared */

#define MMU_STATE_COPYBACK_ALLOC          0x0000004C  /* write-back, sharable */

#define MMU_STATE_CACHEABLE_NOT           MMU_STATE_NORMAL_NONCACHEABLE

/*
 * The following outer/inner noncacheable policy is also used as
 * the basis for the rest of the outer/inner cache policies.
 * (See the MMU_ATTR_OUT_x_IN_x macros for SPL_4 below.)
 */

#define MMU_STATE_OUT_OFF_IN_OFF          0x00000100  /* out/inner non-cached */

/* outer cache policy (TEX[1:0]) >> shift, relative to MMU_ATTR_OUT_x_IN_x */

#define MMU_STATE_OUTER_CACHE_POLICY_SHIFT  24

/* inner cache policy (CB) >> shift, relative to MMU_ATTR_OUT_x_IN_x */

#define MMU_STATE_INNER_CACHE_POLICY_SHIFT  26

/*
 * Set the default state to be copyback. CACHEABLE_WRITETHROUGH can also be
 * selected on those cache designs that permit it.
 */

#define MMU_STATE_CACHEABLE         MMU_STATE_CACHEABLE_COPYBACK

/*
 * Shared memory attribute -
 * accessible by multiple processors or bus masters and must be kept coherent
 */

#define MMU_STATE_SHARED            0x00000400      /* Shared bit */

#define MMU_STATE_WRITETHROUGH_SHARED  \
                          (MMU_STATE_CACHEABLE_WRITETHROUGH | MMU_STATE_SHARED)
#define MMU_STATE_COPYBACK_SHARED  \
                          (MMU_STATE_CACHEABLE_COPYBACK | MMU_STATE_SHARED)
#define MMU_STATE_NONCACHEABLE_SHARED  \
                          (MMU_STATE_NORMAL_NONCACHEABLE | MMU_STATE_SHARED)

/*
 * Global memory attribute -
 * not-Global causes ASID to be associated with address translation
 */

#define MMU_STATE_NOT_GLOBAL        0x00000800      /* not-Global bit */

/*
 * Architecture-specific attributes
 *
 * NOTE: MMU_ATTR_CACHE_GUARDED is used as MMU_ATTR_CACHE_IO (its former name).
 */

#ifdef _WRS_CONFIG_COMPAT69_ARM_ARCH

/* Strongly Ordered - same as MMU_ATTR_CACHE_OFF for backward compatibility */

#define MMU_ATTR_STRONGLY_ORDERED  (MMU_ATTR_CACHE_OFF)
#endif

/*
 * Special Purpose Attributes:
 * When setting any of these attributes, it is necessary to:
 *
 * 1. specify MMU_ATTR_SPL_MSK ORred with MMU_ATTR_CACHE_MSK in the mask
 *    argument to the routine, and:
 * 2. specify the appropriate cache state ORed in to the state argument
 *    to the routine, unless the cache state is implied.
 *
 * So, to set a page as normal, noncacheable memory, it is necessary to
 * specify both MMU_ATTR_SPL_0 and specify MMU_ATTR_CACHE_OFF in the
 * state argument.
 *
 * To "unset" the special attributes, specify MMU_ATTR_CACHE_MSK in
 * the mask parameter (without MMU_ATTR_SPL_MSK) and specify the
 * cacheability required in the state argument.
 */

/* Normal, Noncacheable memory */

#define MMU_ATTR_NORMAL_NONCACHEABLE_MSK (MMU_ATTR_SPL_MSK | MMU_ATTR_CACHE_MSK)
#define MMU_ATTR_NORMAL_NONCACHEABLE     (MMU_ATTR_SPL_0 | MMU_ATTR_CACHE_OFF)
#define MMU_ATTR_NORMAL_NONCACHEABLE_SHARED (MMU_ATTR_NORMAL_NONCACHEABLE | \
                                             MMU_ATTR_CACHE_COHERENCY)

/* Device memory */

#define MMU_ATTR_DEVICE_NONSHARED_MSK   (MMU_ATTR_SPL_MSK | MMU_ATTR_CACHE_MSK)
#define MMU_ATTR_DEVICE_NONSHARED       (MMU_ATTR_SPL_1 | MMU_ATTR_CACHE_OFF)

#define MMU_ATTR_DEVICE_SHARED_MSK      (MMU_ATTR_SPL_MSK | MMU_ATTR_CACHE_MSK)
#define MMU_ATTR_DEVICE_SHARED          (MMU_ATTR_SPL_1 | MMU_ATTR_CACHE_OFF |\
                                         MMU_ATTR_CACHE_COHERENCY)

#define MMU_ATTR_BUFFERABLE     MMU_ATTR_DEVICE_SHARED /* backward compatible */

/*
 * Outer and Inner Cache Write-Back, Write Allocate
 * -- this can also be ORed with MMU_ATTR_CACHE_COHERENCY to set as Shared
 */

#define MMU_ATTR_WRITEALLOCATE_MSK    (MMU_ATTR_SPL_MSK | MMU_ATTR_CACHE_MSK)
#define MMU_ATTR_WRITEALLOCATE        (MMU_ATTR_SPL_3 | MMU_ATTR_CACHE_COPYBACK)
#define MMU_ATTR_WRITEALLOCATE_SHARED (MMU_ATTR_WRITEALLOCATE | \
                                       MMU_ATTR_CACHE_COHERENCY)

/*
 * Individually Specified Outer and Inner Cache Policies
 *  -- ORed with TEX[1:0] and CB encodings for each, shifted to prevent
 *     conflicts with common MMU_ATTR_ protection, cache, valid and SPL bits:
 *  OFF - Noncacheable, Unbuffered
 *  WBWA - Write-Back cached, Write Allocate, Buffered
 *  WTNA - Write-Through cached, No Allocate on Write, Buffered
 *  WBNA - Write-Back cached, No Allocate on Write, Buffered
 *
 * -- these can also be ORed with MMU_ATTR_CACHE_COHERENCY to set as Shared,
 *    but since MMU_ATTR_CACHE_COHERENCY is invalid by itself
 *    MMU_ATTR_CACHE_OFF is added to all of them (and will be disregarded)
 */

#define MMU_ATTR_CACHE_POLICY_SHIFT     28  /* cache policy encoding << shift */
#define MMU_ATTR_CACHE_OUTER_POLICY_MSK 0xC0000000  /* outer policy code bits */
#define MMU_ATTR_CACHE_INNER_POLICY_MSK 0x30000000  /* inner policy code bits */

#define MMU_ATTR_OUT_X_IN_Y_MSK         (MMU_ATTR_SPL_MSK | MMU_ATTR_CACHE_MSK)
#define MMU_ATTR_OUT_OFF_IN_OFF         (MMU_ATTR_SPL_4 | MMU_ATTR_CACHE_OFF)
#define MMU_ATTR_OUT_OFF_IN_WBWA        (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x1 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_OFF_IN_WTNA        (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x2 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_OFF_IN_WBNA        (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x3 << MMU_ATTR_CACHE_POLICY_SHIFT))

#define MMU_ATTR_OUT_WBWA_IN_OFF        (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x4 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WBWA_IN_WBWA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x5 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WBWA_IN_WTNA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x6 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WBWA_IN_WBNA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x7 << MMU_ATTR_CACHE_POLICY_SHIFT))

#define MMU_ATTR_OUT_WTNA_IN_OFF        (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x8 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WTNA_IN_WBWA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0x9 << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WTNA_IN_WTNA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0xA << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WTNA_IN_WBNA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0xB << MMU_ATTR_CACHE_POLICY_SHIFT))

#define MMU_ATTR_OUT_WBNA_IN_OFF        (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0xC << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WBNA_IN_WBWA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0xD << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WBNA_IN_WTNA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0xE << MMU_ATTR_CACHE_POLICY_SHIFT))
#define MMU_ATTR_OUT_WBNA_IN_WBNA       (MMU_ATTR_OUT_OFF_IN_OFF | \
                                          (0xF << MMU_ATTR_CACHE_POLICY_SHIFT))

/* MMU Control Register bit allocations */

#define MMUCR_M_ENABLE   (1<<0)  /* MMU enable */
#define MMUCR_A_ENABLE   (1<<1)  /* Address alignment fault enable */
#define MMUCR_C_ENABLE   (1<<2)  /* (data) cache enable */
#define MMUCR_W_ENABLE   (1<<3)  /* write buffer enable */
#define MMUCR_PROG32     (1<<4)  /* PROG32 */
#define MMUCR_DATA32     (1<<5)  /* DATA32 */
#define MMUCR_L_ENABLE   (1<<6)  /* Late abort on earlier CPUs */
#define MMUCR_BIGEND     (1<<7)  /* Big-endian (=1), little-endian (=0) */
#define MMUCR_SYSTEM     (1<<8)  /* System bit, modifies MMU protections */
#define MMUCR_ROM        (1<<9)  /* ROM bit, modifies MMU protections */
#define MMUCR_F          (1<<10) /* Should Be Zero */
#define MMUCR_Z_ENABLE   (1<<11) /* Branch prediction enable on 810 */
#define MMUCR_I_ENABLE   (1<<12) /* Instruction cache enable */
#define MMUCR_V_ENABLE   (1<<13) /* Exception vectors remap to 0xFFFF0000 */
#define MMUCR_ALTVECT    MMUCR_V_ENABLE /* alternate vector select */
#define MMUCR_RR_ENABLE  (1<<14) /* Round robin cache replacement enable */
#define MMUCR_ROUND_ROBIN MMUCR_RR_ENABLE  /* round-robin placement */
#define MMUCR_DISABLE_TBIT   (1<<15) /* disable TBIT */
#define MMUCR_ENABLE_DTCM    (1<<16) /* Enable Data TCM */
#define MMUCR_ENABLE_ITCM    (1<<18) /* Enable Instruction TCM */
#define MMUCR_UNALIGNED_ENABLE (1<<22) /* Enable unaligned access */
#define MMUCR_EXTENDED_PAGE (1<<23)  /* Use extended PTE format */
#define MMUCR_VECTORED_INTERRUPT (1<<24) /* Enable VIC Interface */
#define MMUCR_EE                 (1<<25) /* Exception Endianness bit */
#define MMUCR_THUMB_EXCEPTIONS (1<<30) /* Exceptions are Thumb */

#define ADDR_TO_PAGE 12     /* shift phys address to PTE page base address*/

/* MMU TTBR register fields */

#define TTBR_MASK_BASE                  0xFFFFC000
#define TTBR_MASK_NOS                   0x00000020
#define TTBR_MASK_RGN                   0x00000018
#define TTBR_MASK_IMP                   0x00000004
#define TTBR_MASK_S                     0x00000002
#define TTBR_MASK_C                     0x00000001 /* no-SMP */
#define TTBR_MASK_IRGN                  0x00000041 /* with-SMP */

/* S & NOS fields */

#define TTBR_ATTR_S                     0x00000002 /* Inner Shared */
#define TTBR_ATTR_NOS                   0x00000020 /* Not Outer Shared */

/* RGN (Outer Cacheability) field bits */

#define TTBR_ATTR_OC_NML_NC             0x00000000 /* Normal, outer not cached */
#define TTBR_ATTR_OC_NML_WB_WA          0x00000008 /* Normal, outer write-back,
                                                      Write-Allocate cached */
#define TTBR_ATTR_OC_NML_WT             0x00000010 /* Normal, outer write-thru cached */
#define TTBR_ATTR_OC_NML_WB             0x00000018 /* Normal, outer write-back,
                                                      no write-allocate cached */

/* C Bit fields for an implementation that does not include SMP (A8) */

#define TTBR_ATTR_C_NML_NC              0x00000000 /* Normal, not cached */
#define TTBR_ATTR_C_NML_C               0x00000001 /* Normal, cached */

/* IRGN (Inner Cacheability) field bits, implementation that include SMP */

#define TTBR_ATTR_IC_NML_NC             0x00000000 /* Normal, inner not cached */
#define TTBR_ATTR_IC_NML_WT             0x00000001 /* Normal, inner write-thru cached */
#define TTBR_ATTR_IC_NML_WB_WA          0x00000040 /* Normal, inner write-back,
                                                      write-allocate cached */
#define TTBR_ATTR_IC_NML_WB             0x00000041 /* Normal, inner write-back,
                                                      no write-allocate cached */

#define PTE_ATTR_NONCACHE               MMU_STATE_NORMAL_NONCACHEABLE
#define PTE_ATTR_SHARED                 MMU_STATE_SHARED
#define PTE_ATTR_SUP_RO                 MMU_STATE_SUP_RO
#define PTE_ATTR_XN                     MMU_STATE_EXE_NOT
#define PTE_ATTR_WBWA                   MMU_STATE_COPYBACK_ALLOC
#define PTE_ATTR_WT                     MMU_STATE_NORMAL_WRITETHROUGH

#define MMU_STATE_PT_FLAGS              pageTableAttr

/*
 *  Mask for Write protecting TLB pages.
 */

#define MMU_STATE_MASK_PT               (MMU_STATE_MASK_PROTECTION  | \
                                         MMU_STATE_MASK_CACHEABLE)

/*
 * The default pages type for a PTE entry
 */

#define MMU_ARM_DEFAULT_SMALL_PAGE_TYPE MMU_PTE_TYPE_SM_PAGE

/*
 * MMU_ATTR_SPL_7 is used for MMU_STATE_ISR_CALLABLE, not supported
 * by ARM MMUs.  We ignore this attribute.
 */

#define ARM_MMU_ATTR_SPL_MSK (MMU_ATTR_SPL_MSK & ~MMU_ATTR_SPL_7)

/* Cortex-A Specific definitions follow */

/*
 * Values to be used when mmuEnable() is called. This will be after the MMU has
 * been initialised by sysInit()/romInit() and after cacheLib has set whatever
 * cache enable settings have been chosen.
 * See "ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition"
 * (ARM DDI 0406) and "Cortex-A8 Processor Technical Reference Manual"
 * (ARM DDI 0344) for details.
 *
 * M     1 Enable MMU
 * A     0 Disable address alignment fault
 * C     X ((D-)Cache Enable) Controlled by cacheLib
 * -     1 (Write Buffer - obsolete) Reserved; SBO
 * -     1 (PROG32 - obsolete) Reserved; SBO
 * -     1 (DATA32 - obsolete) Reserved; SBO
 * -     1 (Late abort - obsolete) Reserved; SBO
 * B     0 (Big/Little-endian - obsolete) Reserved; SBZ
 * -     0 (System - obsolete) Reserved; SBZ
 * -     0 (ROM - obsolete) Reserved; SBZ
 * SW    0 (SWP/SWPB Enable - unsupported) SBZ; Reserved for Multiprocessing
 * Z     1 (Branch prediction enable)
 * I     X (I-cache enable) Controlled by cacheLib
 * V     0 (Vector table location: Normal/High Address)
 * RR    0 (Random/Round-Robin cache replacement - unsupported) SBZ
 * -     0 (Load instructions to PC set T bit - obsolete) SBZ
 * -     1 (Data TCM enable - obsolete) SBO
 * HA    0 (Hardware Access flag enable - unsupported) SBZ
 * -     1 (Instruction TCM enable - obsolete) SBO
 * -     0 Reserved; SBZ
 * -     0 Reserved; SBZ
 * FI    0 (Fast Interrupt enable - unsupported) SBZ
 * U     1 (Unaligned access enable); SBO
 * -     1 (extended PTE format enable - obsolete) SBO
 * VE    0 (VEctored Interrupt - unsupported) SBZ
 * EE    0 Exception Endian set to Little-Endian
 * -     0 Reserved; SBZ
 * NMFI  0 Disable NMFI behavior for FIQs
 * TRE   0 Disable TEX Remap
 * AFE   0 Disable Access Flag
 * TE    X if Thumb-2 enable Thumb Exceptions
 * -     0 Reserved; SBZ
 *
 * For time being, do not enable the address alignment fault, as GCC
 * currently generates unaligned accesses in its code, and switching this
 * on will cause immediate faults. So, do not put it into the enable
 * mask.
 *
 * We used to clear all the Reserved/Should Be Zero bits when the
 * MMU/MPU was enabled, by including one bits for them in the definition
 * of MMU_ENABLE_MASK. We no longer do this, as CPU designers may extend
 * the definitions of the bits within the MMU Control Register. The MMU
 * Control register is initialised within romInit()/sysInit(), as the BSP
 * is the only place where the particular details of the CPU/MMU are
 * actually known. In general, it will be appropriate for the BSP to
 * initialise the MMUCR Reserved/Should Be Zero bits to zero. When the
 * MMU is enabled, we will only change the bits we are particularly
 * concerned about, by using a Read-Modify-Write strategy.
 */

#define MMU_ENABLE_MASK     (MMUCR_M_ENABLE)

#define MMU_ENABLE_VALUE    (MMUCR_M_ENABLE)

#if (ARM_THUMB2)
#define MMU_EXCEPTION_MODE   MMUCR_THUMB_EXCEPTIONS
#else /* (ARM_THUMB2) */
#define MMU_EXCEPTION_MODE   0x0
#endif /* (ARM_THUMB2) */

/* Control Register SBO bits */

#define MMU_MUST_SET_VALUE  (MMUCR_PROG32 | MMUCR_DATA32 | MMUCR_L_ENABLE | \
                             MMUCR_W_ENABLE | MMUCR_ENABLE_DTCM | \
                             MMUCR_ENABLE_ITCM | MMUCR_UNALIGNED_ENABLE | \
                             MMUCR_EXTENDED_PAGE | MMU_EXCEPTION_MODE)

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define MMU_INIT_VALUE  (MMU_MUST_SET_VALUE | MMUCR_Z_ENABLE)
#else
#define MMU_INIT_VALUE  (MMU_MUST_SET_VALUE | MMUCR_Z_ENABLE | MMUCR_EE)
#endif

#define MMU_STATE_CACHE_DFLT_LOCAL_INDEX   16

#ifdef _WRS_CONFIG_SMP
#define MMU_STATE_CACHE_DFLT_LOCAL  \
        (mmuCacheStateTransTbl[ \
         mmuCacheStateTransTbl[MMU_STATE_CACHE_DFLT_LOCAL_INDEX]] \
         | MMU_STATE_SHARED)
#else /*_WRS_CONFIG_SMP*/
#define MMU_STATE_CACHE_DFLT_LOCAL  \
        mmuCacheStateTransTbl[ \
         mmuCacheStateTransTbl[MMU_STATE_CACHE_DFLT_LOCAL_INDEX]]
#endif /*_WRS_CONFIG_SMP*/

#if defined (_WRS_CONFIG_SMP) && defined (_WRS_CONFIG_ARM_ERRATA_798181)
#define    MMU_MIDR_RXPX_MASK           0xff0ffff0
#define    MMU_MIDR_A15                 0x410fc0f0
#define    MMU_MIDR_A15_R3P2            0x413fc0f2
#define    MMU_REVIDE_BIT4              (0x1 << 4)
#define    MMU_REVIDE_BIT9              (0x1 << 9)
#define    MMU_ERRATA_798181_NULL       0x0
#define    MMU_ERRATA_798181_DUMMY_INV  0x1  /* execute a dummy TLBIMVAIS */
#define    MMU_ERRATA_798181_CPC        0x2  /* execute DMB on all cores  */
#define    MMU_CPU_VER_A15_R3P3         0x00300003
#endif /* _WRS_CONFIG_SMP && _WRS_CONFIG_ARM_ERRATA_798181 */

#ifndef _ASMLANGUAGE

/* typedefs */

#if (_BYTE_ORDER == _LITTLE_ENDIAN)

/* little-endian */

/* First level page descriptors */

typedef struct
    {
    UINT type   : 2;        /* descriptor type, 1 => page */
    UINT pad1   : 2;        /* SBZ */
    UINT pad2   : 1;        /* SBO */
    UINT domain : 4;        /* domain number */
    UINT pbit   : 1;        /* 'P' bit */
    UINT addr   : 22;       /* base address of page table */
    } PAGE_DESC_FIELD;

/* First level "section" descriptor */

typedef struct
    {
    UINT type   : 2;        /* descriptor type, 2 => section */
    UINT bbit   : 1;        /* bufferable-bit */
    UINT cbit   : 1;        /* cacheable-bit */
    UINT xnbit  : 1;        /* XN-bit */
    UINT domain : 4;        /* domain number */
    UINT pbit   : 1;        /* XSCALE 'P' bit */
    UINT ap     : 2;        /* AP */
    UINT tex    : 3;        /* TEX */
    UINT apx    : 1;        /* APX */
    UINT sbit   : 1;        /* S-bit */
    UINT nG     : 1;        /* nG-bit */
    UINT super  : 1;        /* 1 => supersection */
    UINT pad1   : 1;        /* SBZ */
    UINT addr   : 12;       /* base address of page table */
    } SECTION_DESC_FIELD;

/* First level "supersection" descriptor */

typedef struct
    {
    UINT type   : 2;        /* descriptor type, 2 => section */
    UINT bbit   : 1;        /* bufferable-bit */
    UINT cbit   : 1;        /* cacheable-bit */
    UINT xnbit  : 1;        /* XN-bit */
    UINT addrHi2: 4;        /* phys address 39:36 */
    UINT pbit   : 1;        /* XSCALE 'P' bit */
    UINT ap     : 2;        /* AP */
    UINT tex    : 3;        /* TEX */
    UINT apx    : 1;        /* APX */
    UINT sbit   : 1;        /* S-bit */
    UINT nG     : 1;        /* nG-bit */
    UINT super  : 1;        /* 1 => supersection */
    UINT pad1   : 1;        /* SBZ */
    UINT addrHi : 4;        /* phys address 35:32 */
    UINT addrLo : 8;        /* phys address 31:24 */
    } SUPERSECTION_DESC_FIELD;

/* Layout of Page Table Entries (PTEs), actually small page descriptors */

typedef struct
    {
    UINT type   : 2;            /* page type, 3 => extended small page */
    UINT cb     : 2;            /* cacheable/bufferable bits */
    UINT ap     : 2;            /* access permission */
    UINT tex    : 3;            /* type extension field */
    UINT apx    : 1;            /* access permission extension field */
    UINT sbz    : 2;            /* should be zero */
    UINT addr   : 20;           /* page base address */
    } PTE_FIELD;

#else /* (_BYTE_ORDER == _LITTLE_ENDIAN) */

/* big-endian */

/* First level page descriptors */

typedef struct
    {
    UINT addr   : 22;       /* base address of page table */
    UINT pbit   : 1;        /* XSCALE 'P' bit */
    UINT domain : 4;        /* domain number */
    UINT pad2   : 1;        /* SBO */
    UINT pad1   : 2;        /* SBZ */
    UINT type   : 2;        /* descriptor type, 1 => page */
    } PAGE_DESC_FIELD;

/* First level "section" descriptor */

typedef struct
    {
    UINT addr   : 12;       /* base address of page table */
    UINT pad1   : 1;        /* SBZ */
    UINT super  : 1;        /* 1 => supersection */
    UINT nG     : 1;        /* nG-bit */
    UINT sbit   : 1;        /* S-bit */
    UINT apx    : 1;        /* APX */
    UINT tex    : 3;        /* AP */
    UINT ap     : 2;        /* AP */
    UINT pbit   : 1;        /* XSCALE 'P' bit */
    UINT domain : 4;        /* domain number */
    UINT xnbit  : 1;        /* XN-bit */
    UINT cbit   : 1;        /* C-bit */
    UINT bbit   : 1;        /* B-bit */
    UINT type   : 2;        /* descriptor type, 2 => section */
    } SECTION_DESC_FIELD;

/* First level "supersection" descriptor */

typedef struct
    {
    UINT addrLo : 8;        /* phys address 31:24 */
    UINT addrHi : 4;        /* phys address 35:32 */
    UINT pad1   : 1;        /* SBZ */
    UINT super  : 1;        /* 1 => supersection */
    UINT nG     : 1;        /* nG-bit */
    UINT sbit   : 1;        /* S-bit */
    UINT apx    : 1;        /* APX */
    UINT tex    : 3;        /* AP */
    UINT ap     : 2;        /* AP */
    UINT pbit   : 1;        /* XSCALE 'P' bit */
    UINT addrHi2: 4;        /* phys address 39:36 */
    UINT xnbit  : 1;        /* XN-bit */
    UINT cbit   : 1;        /* C-bit */
    UINT bbit   : 1;        /* B-bit */
    UINT type   : 2;        /* descriptor type, 2 => section */
    } SUPERSECTION_DESC_FIELD;

/* Layout of Page Table Entries (PTEs), actually small page descriptors */

typedef struct
    {
    UINT addr   : 20;           /* page base address */
    UINT sbz    : 2;            /* should be zero */
    UINT apx    : 1;            /* access permission extension field */
    UINT tex    : 3;            /* type extension field */
    UINT ap     : 2;            /* access permission */
    UINT cb     : 2;            /* cacheable/bufferable bits */
    UINT type   : 2;            /* page type, 3 => extended small page */
    } PTE_FIELD;

#endif /* (_BYTE_ORDER == _LITTLE_ENDIAN) */

/* First level descriptor access */

typedef union
    {
    PAGE_DESC_FIELD         fields;
    SECTION_DESC_FIELD      sectionFields;
    SUPERSECTION_DESC_FIELD supersectionFields;
    UINT32                  bits;
    } MMU_LEVEL_1_DESC;

/* Second level descriptor access */

typedef union
    {
    PTE_FIELD fields;
    UINT32 bits;
    } PTE;

/*
 * Structure for MMU translation table access - normally used to get a pointer
 * to the first-level translation table.
 * To support page table optimization, a second first-level table and a
 * semaphore are needed.
 * The initial, unoptimized set of page tables will be set as the active
 * translation tables (via pLevel1Table) while optimization or deoptimization
 * is in progress using the inactive second translation table (pAltLevel1Table).
 * After each page table optimization or deoptimization operation has
 * completed, the second translation table will be set as the active
 * translation table.  The initial set of page tables retain their
 * links to level 2 pages, which may otherwise be lost after optimization.
 * Any non-optimization changes to the active first-level table will also be
 * made to the inactive first-level table.
 * The semaphore is used to prevent corruption of the tables.
 */

typedef struct mmuTransTblStruct
    {
    MMU_LEVEL_1_DESC *pLevel1Table;     /* active MMU table */
    MMU_LEVEL_1_DESC *pAltLevel1Table;  /* inactive MMU table */
    VX_MUTEX_SEMAPHORE(mmuTransTblSem); /* MMU tables' semaphore */
    UINT32      lockCnt;                /* lock count */
    atomic_t    lockOwner;              /* lock owner */
    UINT8       asid;                   /* ASID/context id */
    } MMU_TRANS_TBL;

/* function declarations */

IMPORT UINT32 mmuReadId (void);
IMPORT PHYS_ADDR mmuVirtToPhys (VIRT_ADDR addr);
IMPORT VIRT_ADDR mmuPhysToVirt (PHYS_ADDR addr);

IMPORT UINT32   (* mmuCrGet) (void);
IMPORT void     (* mmuModifyCr) (UINT32 value, UINT32 mask);
IMPORT UINT32   mmuHardCrGet (void);
IMPORT UINT32   mmuSoftCrGet (void);
IMPORT void     mmuModifyHardCr (UINT32 value, UINT32 mask);
IMPORT void     mmuModifySoftCr (UINT32 value, UINT32 mask);

IMPORT UINT32   (* _func_mmuFaultAddrGet) (void);
IMPORT UINT32   (* _func_mmuFaultStatusGet) (void);
IMPORT UINT32   (* _func_mmuIFaultStatusGet) (void);
IMPORT UINT32   mmuIFsrGet (void);      /* CP15_IFSR only available with MMUs */
IMPORT UINT32   mmuFarGet (void);       /* CP15_FAR only available with MMUs */
IMPORT UINT32   mmuFsrGet (void);       /* CP15_FSR only available with MMUs */

IMPORT  void    mmuLibInstall (VIRT_ADDR(physToVirt) (PHYS_ADDR),
                                       PHYS_ADDR(virtToPhys) (VIRT_ADDR));
IMPORT  void    mmuTtbrSet (MMU_LEVEL_1_DESC *pTable, UINT32 asid);
IMPORT  MMU_LEVEL_1_DESC *  mmuCortexA8TtbrGet (void);
IMPORT  void    mmuTtbrSetAll (UINT32 ttbr, UINT32 asid);
IMPORT  UINT32  mmuTtbrGetAll (void);
IMPORT  void    mmuDacrSet (UINT32 dacrVal);
IMPORT  void    mmuTLBIDFlushEntry (VIRT_ADDR addr);
IMPORT  void    mmuTLBIDFlushAll (void);
IMPORT  void    mmuAEnable (UINT32 cacheState);
IMPORT  void    mmuADisable (void);
IMPORT  STATUS  mmuPBitSet (VIRT_ADDR virtAddr, UINT32 size);
IMPORT  STATUS  mmuPBitClear (VIRT_ADDR virtAddr, UINT32 size);
IMPORT  STATUS  mmuPBitGet (VIRT_ADDR virtAddr);
IMPORT  UINT32  mmuAcrGet (void);
IMPORT  void    mmuAcrSet (UINT32 acrVal);
IMPORT  STATUS  mmuITcmrGet (void);
IMPORT  void    mmuITcmrSet (UINT32 acrVal);
IMPORT  STATUS  mmuDTcmrGet (void);
IMPORT  void    mmuDTcmrSet (UINT32 acrVal);

IMPORT void vxMmuEarlyInit (UINT32 baseAddr, UINT32 linkAddr, UINT32 length);
IMPORT VIRT_ADDR vxMmuEarlyRegMap (PHYS_ADDR physAdrs, ULONG length);
IMPORT void vxMmuEarlyEnable (void);
IMPORT void vxMmuEarlyUnmapAll (void);

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif  /* __INCmmuArmLibh */

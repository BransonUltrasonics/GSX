/* mpuArmLib.h - ARM common MPU library header file */

/*
 * Copyright (c) 2016 - 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02jan17,g_x  remove unused function declarations (V7PRO-3484)
02aug16,g_x  created (F6411)
*/

#ifndef __INCmpuArmLibh
#define __INCmpuArmLibh

#ifdef __cplusplus
     extern "C" {
#endif

/* defines */

/* AP[2:0], Starting at Bit 8 */
 
#define MPU_AP_SUP_NOACCESS_USR_NOACCESS        (0x0 << 8)
#define MPU_AP_SUP_RW_USR_NOACCESS              (0x1 << 8)
#define MPU_AP_SUP_RW_USR_R                     (0x2 << 8)
#define MPU_AP_SUP_RW_USR_RW                    (0x3 << 8)
#define MPU_AP_SUP_R_USR_NOACCESS               (0x5 << 8)
#define MPU_AP_SUP_R_USR_R                      (0x6 << 8)

/* Execute Never bit */

#define MPU_XN_BIT                              (0x01 << 12)

/* TEX[2:0] S C B */

#define MPU_ATTR_STRONGLY_ORDERED               0x00
#define MPU_ATTR_SHARED_DEV                     0x01
#define MPU_ATTR_WT_NOWA                        0x02
#define MPU_ATTR_WB_NOWA                        0x03
#define MPU_ATTR_NOCACHEABLE                    0x08
#define MPU_ATTR_WB_WA                          0x0b
#define MPU_ATTR_NONSHARED_DEV                  0x10

#define MPU_SHAREABLE_BIT                       (0x01 << 2)

/*
 * TCMs always behave as Non-cacheable Non-shared Normal memory, irrespective
 * of the memory type attributes defined in the MPU for a memory region 
 * containing addresses held in the TCM. Access permissions for TCM accesses
 * are the same as the permission attributes that the MPU assigns to the same
 * address. 
 */

#define MPU_TCM_DEF_AP_ATTR         (MPU_AP_SUP_RW_USR_RW |             \
                                     MPU_ATTR_NOCACHEABLE)
     
#define MPU_DEV_DEF_AP_ATTR         (MPU_AP_SUP_RW_USR_NOACCESS |       \
                                     MPU_ATTR_STRONGLY_ORDERED)

#define MPU_DMA_MEM_DEF_AP_ATTR     (MPU_AP_SUP_RW_USR_NOACCESS |       \
                                     MPU_ATTR_STRONGLY_ORDERED)

/* normal memory when cache mode is configured as writethrough */

#define MPU_MEM_WT_DEF_AP_ATTR      (MPU_AP_SUP_RW_USR_NOACCESS |       \
                                     MPU_ATTR_WT_NOWA)

/* normal memory when cache mode is configured as copyback */
     
#define MPU_MEM_WB_DEF_AP_ATTR      (MPU_AP_SUP_RW_USR_NOACCESS |       \
                                     MPU_ATTR_WB_WA)

/* background region permission and attribution: no access and  non-cacheable */

#define MPU_BG_DEF_AP_ATTR         (MPU_AP_SUP_NOACCESS_USR_NOACCESS |  \
                                     MPU_ATTR_NOCACHEABLE)
     
#define MPU_REG_ENABLE              0x01 /* region enable bit in RSR register */
     
#define MPU_RSR_SIZE_OFF            1
#define MPU_RSR_SIZE_MASK           (0x1f << 1)
     
/* MPUIR definitions */

#define MPUIR_NU_BIT                 0x01 /* not unified MPU bit */
#define MPUIR_DREG_BIT_OFF           8
#define MPUIR_IREG_BIT_OFF           16
#define MPUIR_DREG_MASK              (0xff << MPUIR_DREG_BIT_OFF)
#define MPUIR_IREG_MASK              (0xff << MPUIR_IREG_BIT_OFF)

/* Cortex-R Specific definitions follow */

/*
 * Values to be used when mpuEnable() is called. This will be after the MPU has
 * been initialised by sysInit()/romInit() and after cacheLib has set whatever
 * cache enable settings have been chosen.
 * See "ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition"
 * (ARM DDI 0344) for details.
 *
 * M     1 Enable MPU
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
 * of MPU_ENABLE_MASK. We no longer do this, as CPU designers may extend
 * the definitions of the bits within the MPU Control Register. The MPU
 * Control register is initialized within romInit()/sysInit(), as the BSP
 * is the only place where the particular details of the CPU/MPU are
 * actually known. In general, it will be appropriate for the BSP to
 * initialize the MPUCR Reserved/Should Be Zero bits to zero. When the
 * MPU is enabled, we will only change the bits we are particularly
 * concerned about, by using a Read-Modify-Write strategy.
 */
     
/* MPU Control Register bit allocations */

#define MPUCR_M_ENABLE   (1<<0)  /* MPU enable */
#define MPUCR_A_ENABLE   (1<<1)  /* Address alignment fault enable */
#define MPUCR_C_ENABLE   (1<<2)  /* (data) cache enable */
#define MPUCR_W_ENABLE   (1<<3)  /* write buffer enable */
#define MPUCR_PROG32     (1<<4)  /* PROG32 */
#define MPUCR_DATA32     (1<<5)  /* DATA32 */
#define MPUCR_L_ENABLE   (1<<6)  /* Late abort on earlier CPUs */
#define MPUCR_BIGEND     (1<<7)  /* Big-endian (=1), little-endian (=0) */
#define MPUCR_SYSTEM     (1<<8)  /* System bit, modifies MPU protections */
#define MPUCR_ROM        (1<<9)  /* ROM bit, modifies MPU protections */
#define MPUCR_F          (1<<10) /* Should Be Zero */
#define MPUCR_Z_ENABLE   (1<<11) /* Branch prediction enable on 810 */
#define MPUCR_I_ENABLE   (1<<12) /* Instruction cache enable */
#define MPUCR_V_ENABLE   (1<<13) /* Exception vectors remap to 0xFFFF0000 */
#define MPUCR_ALTVECT    MPUCR_V_ENABLE /* alternate vector select */
#define MPUCR_RR_ENABLE  (1<<14) /* Round robin cache replacement enable */
#define MPUCR_ROUND_ROBIN MPUCR_RR_ENABLE  /* round-robin placement */
#define MPUCR_BR_ENABLE  (1<<17)  /* default map used as background regions */
#define MPUCR_UNALIGNED_ENABLE (1<<22) /* Enable unaligned access */
#define MPUCR_VECTORED_INTERRUPT (1<<24) /* Enable VIC Interface */
#define MPUCR_EE                 (1<<25) /* ExceptioMPUndianness bit */
#define MPUCR_THUMB_EXCEPTIONS (1<<30) /* Exceptions are Thumb */

#define MPU_ENABLE_MASK     (MPUCR_M_ENABLE)
#define MPU_ENABLE_VALUE    (MPUCR_M_ENABLE)
     
#define MPU_NORMAL_MEM_REG_BASE         0x00000000
#define MPU_NORMAL_MEM_REG_SIZE         31

#ifndef _ASMLANGUAGE
typedef enum _MPU_REGION_TYPE
    {
    MPU_DATA_REGION = 0,
    MPU_INSTRUCTION_REGION
    } MPU_REGION_TYPE;
#endif /* _ASMLANGUAGE */

#if (ARM_THUMB2)
#define MPU_EXCEPTION_MODE   MPUCR_THUMB_EXCEPTIONS
#else /* (ARM_THUMB2) */
#define MPU_EXCEPTION_MODE   0x0
#endif /* (ARM_THUMB2) */

/* Control Register SBO bits */

#define MPU_MUST_SET_VALUE  (MPUCR_PROG32 | MPUCR_DATA32 | MPUCR_L_ENABLE | \
                             MPUCR_W_ENABLE | MPUCR_ENABLE_DTCM | \
                             MPUCR_ENABLE_ITCM | MPUCR_UNALIGNED_ENABLE | \
                             MPUCR_EXTENDED_PAGE | MPU_EXCEPTION_MODE)

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define MPU_INIT_VALUE  (MPU_MUST_SET_VALUE | MPUCR_Z_ENABLE)
#else
#define MPU_INIT_VALUE  (MPU_MUST_SET_VALUE | MPUCR_Z_ENABLE | MPUCR_EE)
#endif

#ifndef _ASMLANGUAGE

/* function declarations */

IMPORT UINT32   (* _func_mmuFaultAddrGet) (void);
IMPORT UINT32   (* _func_mmuFaultStatusGet) (void);
IMPORT UINT32   (* _func_mmuIFaultStatusGet) (void);
IMPORT UINT32   (* mmuCrGet) (void);
IMPORT void     (* mmuModifyCr) (UINT32 value, UINT32 mask);

IMPORT UINT32   mmuReadId (void);
IMPORT UINT32   mpuIFsrGet (void);
IMPORT UINT32   mpuFarGet (void);
IMPORT UINT32   mpuFsrGet (void);
IMPORT STATUS   armMpuRegionAdd (MPU_REGION_TYPE type, UINT32 base, 
                                 UINT32 size, UINT32 attr);
IMPORT void     armMpuRegionDel (MPU_REGION_TYPE type, UINT32 base, UINT32 size);
#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif  /* __INCmpuArmLibh */

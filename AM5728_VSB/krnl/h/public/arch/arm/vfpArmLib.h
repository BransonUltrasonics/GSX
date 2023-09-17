/* vfpArmLib.h - Arm Version of Vector Floating-Point Include File */

/*
 * Copyright (c) 2005, 2009-2012, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
28mar16,yya  Correct function prototype (V7PRO-2813)
09oct12,jdw  Add support to configure Round To Zero mode in FPSCR
25jul11,jdw  Add support for VFPv3-D32 for ARMARCH7
29apr10,pad  Moved extern C statement after include statements.
15may09,mdo  Assembler is fixed
29jun05,scm  VFP written
*/

#ifndef __INCvfpArmLibh
#define __INCvfpArmLibh

#include <vxWorks.h>
#include <regs.h>
#include <esf.h>

#ifdef __cplusplus
extern "C" {
#endif

/* VFP System Registers */

/* FPSID (Floating Point System ID Register) bit definitions */
/* READ ONLY register, RESET State:0x410120b3, Access Mode:Any */

#define FPSID_IMPLEMENTOR_MSK  0xFF000000
#define FPSID_ARM_LTD          0x41000000  /* ARM Limited */

#define FPSID_SW_MSK           (1<<23)
#define FPSID_SW               FPSID_SW_MSK
#define FPSID_HW               0x00000000

#define FPSID_FORMATR_MSK      0x00600000
#define FPSID_STNDRD_F1        0x00000000
#define FPSID_STNDRD_F2        (1<<21)
#define FPSID_RSRVD_F          (1<<22)
#define FPSID_NONSTDRD_F       0x00600000

#define FPSID_SNG_MSK          (1<<20)
#define FPSID_SNG_ONLY         FPSID_SNG_MSK

#define FPSID_ARCH_MSK         0x000F0000
#define FPSID_VFP1             0x00000000
#define FPSID_VFP2             0x00010000  /* VFPv2 */

#define FPSID_PRTNUM_MSK       0x0000FF00
#define FPSID_VFP11            0x00002000  /* VFP11 */

#define FPSID_VARIANT_MSK      0x000000F0
#define FPSID_ARM11_VFP        0x000000B0  /* ARM 11 VFP */

#define FPSID_REV_MSK          0x0000000F

/* FPSCR (Floating Status and Control Register) bit definitions */
/* READ/WRITE register, RESET State:0x00000000, Access Mode:Any */

/*
 * At the time of implementation bits 27-25,19,15-13, and 7-5,
 * are described as DNM (DO NOT MODIFY), one must employ
 * read/modify/write techniques when handling the FPSCR in order
 * to ensure that these bits are not modified...
 */

#define FPSCR_N_BIT        (1<<31) /* less than result */
#define FPSCR_Z_BIT        (1<<30) /* equal result */
#define FPSCR_C_BIT        (1<<29) /* equal, greater than, or unordered result */
#define FPSCR_V_BIT        (1<<28) /* unordered result */

#define FPSCR_DN_BIT       (1<<25) /* Default NaN mode control */
#define FPSCR_DN_MSK       FPSCR_DN_BIT
#define FPSCR_DN_ENABLE    FPSCR_DN_BIT
#define FPSCR_DN_DISABLE   0x00000000

#define FPSCR_FZ_BIT       (1<<24) /* flush to zero mode control */
#define FPSCR_FZ_MSK       FPSCR_FZ_BIT
#define FPSCR_FZ_ENABLE    FPSCR_FZ_BIT
#define FPSCR_FZ_DISABLE   0x00000000

/* Trap Enable Bits */
#define FPSCR_IDE_BIT      (1<<15) /* Input Subnormal */
#define FPSCR_IXE_BIT      (1<<12) /* Inexact */
#define FPSCR_UFE_BIT      (1<<11) /* Underflow */
#define FPSCR_OFE_BIT      (1<<10) /* Overflow */
#define FPSCR_DZE_BIT      (1<<9)  /* Division By Zero */
#define FPSCR_IOE_BIT      (1<<8)  /* Invalid Operation */

/* Cumulative Exception Bits */
#define FPSCR_IDC_BIT      (1<<7)  /* Input Subnormal */
#define FPSCR_IXC_BIT      (1<<4)  /* Inexact */
#define FPSCR_UFC_BIT      (1<<3)  /* Underflow */
#define FPSCR_OFC_BIT      (1<<2)  /* Overflow */
#define FPSCR_DZC_BIT      (1<<1)  /* Division By Zero */
#define FPSCR_IOC_BIT      (1<<0)  /* Invalid Operation */

#define FPSCR_RMODES_MSK   0x00C00000
#define FPSCR_RN           0x00000000
#define FPSCR_RP           0x00400000
#define FPSCR_RM           0x00800000
#define FPSCR_RZ           0x00C00000

#define FPSCR_STRIDE_MSK   0x00300000
#define FPSCR_LEN_MSK      0x00070000

/* FPEXC (Floating Exception Register) bit definitions */
/* READ/WRITE register, RESET State:0x00000000, Access Mode:Privileged */

#define FPEXC_EX_BIT       (1<<31) /* status bit */
#define FPEXC_EN_BIT       (1<<30) /* global enable bit */

#define FPEXC_FP2V_BIT     (1<<28) /* FPINST2 instruction valid bit */

/* Vector Iteration */
#define FPEXC_VECITR_MSK   0x00000700
#define FPEXC_VECITR_1     0x00000000
#define FPEXC_VECITR_2     0x00000100
#define FPEXC_VECITR_3     0x00000200
#define FPEXC_VECITR_4     0x00000300
#define FPEXC_VECITR_5     0x00000400
#define FPEXC_VECITR_6     0x00000500
#define FPEXC_VECITR_7     0x00000600
#define FPEXC_VECITR_8     0x00000700

#define FPEXC_INV_BIT      (1<<7)  /* Input Exception bit */

#define FPEXC_UFC_BIT      (1<<3)  /* Potential Underflow bit */
#define FPEXC_OFC_BIT      (1<<2)  /* Potential Overflow bit */

#define FPEXC_IOC_BIT      (1<<0)  /* Potential Invalid Operation bit */

/* VFP11 Specific System Registers */

/* FPINST (Floating Point Instruction Register) bit definitions */
/* READ/WRITE register, RESET State:0xEE000A00, Access Mode:Privileged */
/*
 * FPINST register contains the exceptional instruction
 */

/* FPINST2 (Floating Point Instruction Register 2) bit definitions */
/* READ/WRITE register, RESET State:Unpredictable, Access Mode:Privileged */
/*
 * FPINST2 register contains the instruction that was issued to the VFP11
 * coprocessor before the exception was detected
 */

/* MFVFR0 (Media and VFP Feature Register 0) bit definitions */
/* READ ONLY register, RESET State:0x11111111, Access Mode:Any */

/* MFVFR1 (Media and VFP Feature Register 1) bit definitions */
/* READ ONLY register, RESET State:0x00000000, Access Mode:Any */

/* initialization parameters */

/*
 * The VFP coprocessor allows the Round To Zero mode to be used to perform
 * faster VFP computations, but is not strictly IEEE-754 compliant. The
 * default for ARM is to enable Round To Zero mode. See the ARM Architecture
 * Reference Manual for more information.
 */
#ifdef _WRS_CONFIG_DISABLE_VFP_ROUND_TO_ZERO
#define FPSCR_INIT         FPSCR_DN_MSK
#else /* _WRS_CONFIG_DISABLE_VFP_ROUND_TO_ZERO */
#define FPSCR_INIT         (FPSCR_DN_MSK | FPSCR_FZ_BIT)
#endif /* _WRS_CONFIG_DISABLE_VFP_ROUND_TO_ZERO */

#define _VFP_ALIGN_SIZE    8                                   /* 64 bit */

#define VFPREG_SET         VFP_CONTEXT

/* FP_CONTEXT structure offsets */
#define VFP_FPSID          0x00
#define VFP_FPSCR          0x04
#define VFP_FPEXC          0x08
#define VFP_FPINST         0x0C
#define VFP_FPINST2        0x10
#define VFP_MVVFR0         0x14
#define VFP_MVVFR1         0x18
#define VFP_PAD            -
#define VFP_GPR            0x20    /* OFFSET(FP_CONTEXT, vfp_gpr[0]) */
#define VFP_GPR_OFFSET(n)  (VFP_GPR + (n)*4)         /* vfp_gpr[n]) */

/* VFP_REG_SET - ARM VFP register set */

#ifdef _WRS_CONFIG_ENABLE_VFPV3_D32

/*
 * VFPv3-D32 / NEON
 * 64 single precision or 32 double precision general purpose
 */

#define VFP_DGPR_NUM       32 /* 32-double precision general purpose */
#define VFP_SGPR_NUM       64 /* 64-single precision general purpose */

#else    /* _WRS_CONFIG_ENABLE_VFPV3_D32 */

/*
 * VFPv3-D16
 * 32 single precision or 16 double precision general purpose
 */

#define VFP_DGPR_NUM       16 /* 16-double precision general purpose */
#define VFP_SGPR_NUM       32 /* 32-single precision general purpose */

#endif /* _WRS_CONFIG_ENABLE_VFPV3_D32 */

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
# define  HREG  1
# define  LREG  0
#else   /* _BYTE_ORDER != _LITTLE_ENDIAN */
# define  HREG  0
# define  LREG  1
#endif  /* _BYTE_ORDER == _LITTLE_ENDIAN */

#ifndef _ASMLANGUAGE

typedef union
    {
     double u64;
     UINT32 u32[2];
    } VFP_UNION64D;

typedef struct vfpContext       /* VFP Context */
 {
  UINT32 fpsid;                 /* system ID register              :   4 */
  UINT32 fpscr;                 /* status and control register     :   4 */
  UINT32 fpexc;                 /* exception register              :   4 */
  UINT32 fpinst;                /* instruction register            :   4 */
  UINT32 fpinst2;               /* instruction register 2          :   4 */
  UINT32 mfvfr0;                /* media and VFP feature Register 0:   4 */
  UINT32 mfvfr1;                /* media and VFP feature Register 1:   4 */
  UINT32 pad;
  UINT32 vfp_gpr[VFP_SGPR_NUM]; /* general purpose registers       : 128 or 256 */
                                /* TOTAL                           : 156 or 284 */
 } WRS_PACK_ALIGN(_VFP_ALIGN_SIZE) VFP_CONTEXT;

/* function declarations */

IMPORT void          vfpArchInit (void);
IMPORT void          vfpArchTaskCreateInit (VFP_CONTEXT *pDspContext);

IMPORT void          vfpSave (VFP_CONTEXT *);
IMPORT void          vfpRestore (VFP_CONTEXT *);

IMPORT STATUS        vfpProbe (void);
IMPORT void          vfpProbeTrap (ESF *, REG_SET *);
IMPORT STATUS        vfpEnable (void);
IMPORT STATUS        vfpDisable (void);

IMPORT VFP_CONTEXT * vfpCtxCreate (int);
IMPORT STATUS        vfpCtxDelete (VFP_CONTEXT *);
IMPORT void          vfpCtxShow (VFP_CONTEXT *);

void                 vfpRegsToCtx (VFPREG_SET *, VFP_CONTEXT *);
void                 vfpCtxToRegs (VFP_CONTEXT *, VFPREG_SET *);

#ifdef  _WRS_HW_VFP_SUPPORT
IMPORT int           vfpIsEnabled (void);
IMPORT int           vfpHasException (void);
IMPORT STATUS        vfpProbeSup (void);
IMPORT UINT32        vfpFpSidGet (void);
IMPORT UINT32        vfpFpScrGet (void);
IMPORT void          vfpFpScrSet (UINT32);
#endif

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCvfpArmLibh */

/* vfpSpArmLib.h - Single Precise Vector Floating-Point Include File */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
07may13,c_t  VFPv4 SP written based on standard version
*/

#ifndef __INCvfpArmLibSph
#define __INCvfpArmLibSph

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * for cortex M the following register are memory mapped and the addresses 
 * are architectural defined.
 */
/* Coprocessor Access Control Register, CPACR, enable CP10 & CP11 for VFP */

#define CPACR               (0xE000ED88)
#define CPACR_CP_FULL(x)    (3<<(2*(x)))  /* 0b11 : Full access for this cp */

/* Floating-Point Context Control Register, FPCCR */
#define FPCCR               (0xE000EF34)
#define FPCCR_ASPEN         (1<<31)  /* enable auto FP context stacking */
#define FPCCR_LSPEN         (1<<30)  /* enable lazy FP context stacking */
#define FPCCR_MONRDY        (1<<8)
#define FPCCR_BFRDY         (1<<6)
#define FPCCR_MMRDY         (1<<5)
#define FPCCR_HFRDY         (1<<4)
#define FPCCR_THREAD        (1<<3)   /* allocated FP stack in thread */
#define FPCCR_USER          (1<<1)   /* allocated FP stack in priviledged */
#define FPCCR_LSPACT        (1<<0)   /* lazy stacking is active */

/* Floating-Point Context Address Register, FPCAR */
/* this register contains the FP context address in the exception stack */

#define FPCAR               (0xE000EF38)

/* Floating-Point Default Status Control Register, FPDSCR */
/* 
 * set the initial value of FPSCR, however we can set it manually to FPSCR 
 * when creating the VFP task, so let it as is at this time.
 */

#define FPDSCR              (0xE000EF3C)
#define FPDSCR_AHP          (1<<26)
#define FPDSCR_DN           (1<<25)
#define FPDSCR_FZ           (1<<24)
#define FPDSCR_RMODE        0x00C00000
#define FPDSCR_RN           0x00000000   /* round to nearest */
#define FPDSCR_RP           0x00400000   /* round toward plus infinity */
#define FPDSCR_RM           0x00800000   /* round toward minus infinity */
#define FPDSCR_RZ           0x00C00000   /* round toward zero */

/* Media and FP Feature Register 0, MVFR0 & Register 1, MVFR1*/
/* these read only registers are using to detect feature that supported */

#define MVFR0               (0xE000EF40)
#define MVFR0_SP_MSK        (0x000000F0) /* bit 4~7 = 0x2, support sp */
#define MVFR0_SP_EN         (0x20)       /* bit 4~7 = 0x2, support sp */
#define MVFR1               (0xE000EF44)

/* FPSCR (Floating Status and Control Register) bit definitions for VFPv4SP */

/*
 * At the time of implementation bits 27, 21-8, and 6-5,
 * are described as reserved, one must employ
 * read/modify/write techniques when handling the FPSCR in order
 * to ensure that these bits are not modified...
 */

#define FPSCR_N_BIT         (1<<31) /* less than result */
#define FPSCR_Z_BIT         (1<<30) /* equal result */
#define FPSCR_C_BIT         (1<<29) /* equal, greater than, or unordered result */
#define FPSCR_V_BIT         (1<<28) /* unordered result */

#define FPSCR_AHP           (1<<26)

#define FPSCR_DN_BIT        (1<<25) /* Default NaN mode control */
#define FPSCR_DN_MSK        FPSCR_DN_BIT
#define FPSCR_DN_ENABLE     FPSCR_DN_BIT
#define FPSCR_DN_DISABLE    0x00000000

#define FPSCR_FZ_BIT        (1<<24) /* flush to zero mode control */
#define FPSCR_FZ_MSK        FPSCR_FZ_BIT
#define FPSCR_FZ_ENABLE     FPSCR_FZ_BIT
#define FPSCR_FZ_DISABLE    0x00000000

/* Cumulative Exception Bits */

#define FPSCR_IDC_BIT       (1<<7)  /* Input Subnormal */
#define FPSCR_IXC_BIT       (1<<4)  /* Inexact */
#define FPSCR_UFC_BIT       (1<<3)  /* Underflow */
#define FPSCR_OFC_BIT       (1<<2)  /* Overflow */
#define FPSCR_DZC_BIT       (1<<1)  /* Division By Zero */
#define FPSCR_IOC_BIT       (1<<0)  /* Invalid Operation */

/* Round Mode Control Bits */

#define FPSCR_RMODES_MSK    0x00C00000  /* round mode */
#define FPSCR_RN            0x00000000  /* round to nearest */
#define FPSCR_RP            0x00400000  /* round toward plus infinity */
#define FPSCR_RM            0x00800000  /* round toward minus infinity */
#define FPSCR_RZ            0x00C00000  /* round toward zero */

#ifdef _WRS_CONFIG_DISABLE_VFP_ROUND_TO_ZERO
#define FPSCR_INIT          FPSCR_DN_MSK
#else /* _WRS_CONFIG_DISABLE_VFP_ROUND_TO_ZERO */
#define FPSCR_INIT          (FPSCR_DN_MSK | FPSCR_FZ_BIT)
#endif /* _WRS_CONFIG_DISABLE_VFP_ROUND_TO_ZERO */

#define VFPREG_SET          VFP_CONTEXT

#define _VFP_ALIGN_SIZE     8           /* Not sure if this is needed */

/* vfpv4 sp only support D16/S32. */

#define VFP_DGPR_NUM        16          /* 16-double precision general purpose */
#define VFP_SGPR_NUM        32          /* 32-single precision general purpose */

/* FP_CONTEXT structure offsets */

#define VFP_FPSCR           0x00
#define VFP_PAD             -
#define VFP_GPR             0x08        /* OFFSET(FP_CONTEXT, vfp_gpr[0]) */
#define VFP_GPR_OFFSET(n)   (VFP_GPR + (n)*4)   /* vfp_gpr[n]) */

#ifndef _ASMLANGUAGE

/* standard interface to fp lib. */

/* vfpv4 sp context is different with standard vfp. */

typedef struct vfpContext           /* VFP Context */
    {
    UINT32 fpscr;                   /* status and control register      */
    UINT32 pad;                     /* pad to match 8 byte alignment    */
    UINT32 vfp_gpr [VFP_SGPR_NUM];  /* general purpose registers TOTAL  */
    } WRS_PACK_ALIGN (_VFP_ALIGN_SIZE) VFP_CONTEXT;

/* function declarations */

IMPORT void          vfpArchInit (void);
IMPORT void          vfpArchTaskCreateInit (VFP_CONTEXT *pDspContext);

IMPORT void          vfpSave (VFP_CONTEXT *);
IMPORT void          vfpRestore (VFP_CONTEXT *);

IMPORT STATUS        vfpProbe (void);
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

#endif /* __INCvfpArmLibSph */


/* usrSmcProtocol.c - ARM simple SMC call protocol */

/*
 * Copyright (c) 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21oct16,yya  refactored secure mode interface (F6283)
10nov15,cfm  written (US69494)
*/

/*
DESCRIPTION
Provide vxActlrSet, vxActlrGet, (vxL2ActlrSet, and vxL2ActlrGet) routines. Some
boards need SMC instruction some boards don't. So supply these routines as
configlette so the ASP can be the same for all those boards.
Note: these routines are still arch-private routines, *NOT* APIs.

NOMANUAL
*/

/* includes */

#include <vxWorks.h>

/* defines */

#define L2_AUX_CTL_NS_INT_ACCESS        (1 << 27)       /* Non-secure interrupt access control */

#ifdef INCLUDE_SECURE_CONFIG_TISM
#define INCLUDE_ACTLR_NEED_SMC
#define INCLUDE_L2ACTLR_NEED_SMC
#define INCLUDE_PL310_NEED_SMC
#endif /* INCLUDE_SECURE_CONFIG_TISM */

#ifdef __DCC__

/* instruction codes for SMC instruction, remove when diab supported SMC */

#ifdef ARMBE8
#if ARM_THUMB2
#define SMC(n)          .half 0x0080, 0xf0f7 | n << 8
#else /* ARM_THUMB2 */
#define SMC(n)          .word 0x700060e1 | (n << 24)
#endif /* ARM_THUMB2 */
#else /* ARMBE8 */
#if ARM_THUMB2
#define SMC(n)          .half 0xf7f0 | n, 0x8000
#else /* ARM_THUMB2 */
#define SMC(n)          .word 0xe1600070 | n
#endif /* ARM_THUMB2 */
#endif /* ARMBE8 */

/********************************/
/* access ACTRL register        */                             
/********************************/

__asm volatile void writeActlr (UINT32 value)
{
% reg value
#ifdef INCLUDE_ACTLR_NEED_SMC
! "r0", "r12"
    MOV    r0, value
    PUSH   {r4-r11}
    LDR    r12, =SMC_ACTLR_VAL
    SMC    (SMC_ACTLR_IMM)
    POP    {r4-r11}
#else /* INCLUDE_ACTLR_NEED_SMC */
    mcr    p15, 0, value, c1, c0, 1
#endif /* INCLUDE_ACTLR_NEED_SMC */
}

__asm volatile UINT32 readActlr (void)
{
! "r0"
    mrc     p15, 0, r0, c1, c0, 1
}

/********************************/
/* access L2 AUX CTRL registers */
/* only present on Cortex-A15   */                             
/********************************/

#if defined (_WRS_CONFIG_ARMV7_CORE_GENERIC) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15)
__asm volatile void writeL2Actlr (UINT32 value)
{
% reg value
#ifdef INCLUDE_L2ACTLR_NEED_SMC
! "r0", "r12"
    MOV     r0, value
    PUSH    {r4-r11}
    LDR     r12, =SMC_L2ACTLR_VAL
    SMC     (SMC_L2ACTLR_IMM)
    POP     {r4-r11}
#else /* INCLUDE_L2ACTLR_NEED_SMC */
    MCR     p15, 1, value, c15, c0, 0
#endif /* INCLUDE_L2ACTLR_NEED_SMC */
}

__asm volatile UINT32 readL2Actlr (void)
{
! "r0"
    MRC     p15, 1, r0, c15, c0, 0
}
#endif /* _WRS_CONFIG_ARMV7_CORE_GENERIC || _WRS_CONFIG_ARMV7_CORE_CTX_A15 */

/**************************/
/* access PL310 registers */                             
/**************************/

#ifdef INCLUDE_PL310_NEED_SMC
__asm volatile void pl310ClearDisable (UINT32 addr, UINT32 ass)
#else /* INCLUDE_PL310_NEED_SMC */
__asm volatile void pl310ClearDisable (UINT32 addr, UINT32 ass, UINT32 ctrl)
#endif /* INCLUDE_PL310_NEED_SMC */
{
#ifdef INCLUDE_PL310_NEED_SMC
% reg addr, ass; lab wait;
! "r0", "r3", "r12"
    PUSH    {r4-r11}
#else /* INCLUDE_PL310_NEED_SMC */
% reg addr, ass, ctrl;
! "r0"
#endif /* INCLUDE_PL310_NEED_SMC */
    STR     ass, [addr]
    DSB
    ISB
wait:
    LDR     r3, [addr]
    ANDS    r3, r3, ass
    BNE     wait

    MOV     r0, #0
#ifdef INCLUDE_PL310_NEED_SMC
    LDR     r12, =SMC_PL310_CTRL_VAL
    SMC     (SMC_PL310_CTRL_IMM)
    POP     {r4-r11}
#else /* INCLUDE_PL310_NEED_SMC */
    STR     r0, [ctrl]
    DSB
    ISB
#endif /* INCLUDE_PL310_NEED_SMC */
}

#ifdef INCLUDE_PL310_NEED_SMC
__asm volatile void pl310CtrlWrite (UINT32 value)
#else /* INCLUDE_PL310_NEED_SMC */
__asm volatile void pl310CtrlWrite (UINT32 addr, UINT32 value)
#endif /* INCLUDE_PL310_NEED_SMC */
{
#ifdef INCLUDE_PL310_NEED_SMC
% reg value;
! "r0", "r12"
    PUSH    {r4-r11}
    MOV     r0, value
    LDR     r12, =SMC_PL310_CTRL_VAL
    SMC     (SMC_PL310_CTRL_IMM)
    POP     {r4-r11}
#else /* INCLUDE_PL310_NEED_SMC */
% reg addr, value;
    STR     value, [addr]
    DSB
    ISB
#endif /* INCLUDE_PL310_NEED_SMC */
}

#ifdef INCLUDE_PL310_NEED_SMC
__asm volatile void pl310AuxCtrlWrite (UINT32 value)
#else /* INCLUDE_PL310_NEED_SMC */
__asm volatile void pl310AuxCtrlWrite (UINT32 addr, UINT32 value)
#endif /* INCLUDE_PL310_NEED_SMC */
{
#ifdef INCLUDE_PL310_NEED_SMC
% reg value;
! "r0", "r12"
    PUSH    {r4-r11}
    MOV     r0, value
    LDR     r12, =SMC_PL310_AUXCTRL_VAL
    SMC     (SMC_PL310_AUXCTRL_IMM)
    POP     {r4-r11}
#else /* INCLUDE_PL310_NEED_SMC */
% reg addr, value;
    STR     value, [addr]
    DSB
    ISB
#endif /* INCLUDE_PL310_NEED_SMC */
}

#ifdef INCLUDE_PL310_NEED_SMC
__asm volatile void pl310LatCtrlWrite (UINT32 tagLat, UINT32 datLat)
#else /* INCLUDE_PL310_NEED_SMC */
__asm volatile void pl310LatCtrlWrite (UINT32 tagLat, UINT32 datLat,
                                       UINT32 tagLatCtrl, UINT32 datLatCtrl)
#endif /* INCLUDE_PL310_NEED_SMC */
{
#ifdef INCLUDE_PL310_NEED_SMC
% reg tagLat, datLat;
! "r0", "r1", "r12"
    PUSH    {r4-r11}
    MOV     r0, tagLat
    MOV     r1, datLat
    LDR     r12, =SMC_PL310_LATCTRL_VAL
    SMC     (SMC_PL310_LATCTRL_IMM)
    POP     {r4-r11}
#else /* INCLUDE_PL310_NEED_SMC */
% reg tagLat, datLat, tagLatCtrl, datLatCtrl;
    STR     tagLat, [tagLatCtrl]
    STR     datLat, [datLatCtrl]
    DSB
    ISB
#endif /* INCLUDE_PL310_NEED_SMC */
}


#else /* __DCC__ */

/********************************/
/* access ACTRL register        */                             
/********************************/

#ifdef INCLUDE_ACTLR_NEED_SMC
#define writeActlr(value) __asm__ volatile (            \
    ".arch_extension sec\n\t"                           \
    "PUSH   {r4-r11}\n\t"                               \
    "MOV    r0, %0\n\t"                                 \
    "MOV    r12, %1\n\t"                                \
    "SMC    %2\n\t"                                     \
    "POP    {r4-r11}\n\t"                               \
    :                                                   \
    : "r" (value),                                      \
      "i" (SMC_ACTLR_VAL), "i" (SMC_ACTLR_IMM)          \
    : "r0", "r12"                                       \
    )
#else /* INCLUDE_ACTLR_NEED_SMC */
#define writeActlr(value) __asm__ volatile (            \
    "MCR    p15, 0, %0, c1, c0, 1" : : "r" (value)      \
    )
#endif /* INCLUDE_ACTLR_NEED_SMC */

#define readActlr()                                     \
    ({                                                  \
    UINT32 value;                                       \
    __asm__ volatile (                                  \
        "MRC     p15, 0, %0, c1, c0, 1\n\t"             \
        : "=r" (value));                                \
    value;                                              \
    })

/********************************/
/* access L2 AUX CTRL registers */
/* only present on Cortex-A15   */                             
/********************************/

#if defined (_WRS_CONFIG_ARMV7_CORE_GENERIC) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15)
#ifdef INCLUDE_L2ACTLR_NEED_SMC
#define writeL2Actlr(value) __asm__ volatile (          \
    ".arch_extension sec\n\t"                           \
    "PUSH   {r4-r11}\n\t"                               \
    "MOV    r0, %0\n\t"                                 \
    "MOV    r12, %1\n\t"                                \
    "SMC    %2\n\t"                                     \
    "POP    {r4-r11}\n\t"                               \
    :                                                   \
    : "r" (value),                                      \
      "i" (SMC_L2ACTLR_VAL), "i" (SMC_L2ACTLR_IMM)      \
    : "r0", "r12"                                       \
    )
#else /* INCLUDE_L2ACTLR_NEED_SMC */
#define writeL2Actlr(value) __asm__ volatile (          \
    "MCR    p15, 1, %0, c15, c0, 0" : : "r" (value)     \
    )
#endif /* INCLUDE_L2ACTLR_NEED_SMC */

#define readL2Actlr()                                   \
    ({                                                  \
    UINT32 value;                                       \
    __asm__ volatile (                                  \
        "MRC     p15, 1, %0, c15, c0, 0\n\t"            \
        : "=r" (value));                                \
    value;                                              \
    })
#endif /* _WRS_CONFIG_ARMV7_CORE_GENERIC || _WRS_CONFIG_ARMV7_CORE_CTX_A15 */

/**************************/
/* for PL310 registers    */                             
/**************************/

#ifdef INCLUDE_PL310_NEED_SMC
#define pl310ClearDisable(clear, ass)                   \
    __asm volatile (                                    \
        ".arch_extension sec\n\t"                       \
        "PUSH   {r4-r11}\n\t"                           \
        "STR    %1, [%0]\n\t"                           \
        "DSB\n\t"                                       \
        "ISB\n\t"                                       \
        "0:\n\t"                                        \
        "LDR    r3, [%0]\n\t"                           \
        "ANDS   r3, r3, %1\n\t"                         \
        "BNE    0b\n\t"                                 \
        "MOV    r0, #0\n\t"                             \
        "MOVW   r12, %2\n\t"                            \
        "SMC    %3\n\t"                                 \
        "POP    {r4-r11}\n\t"                           \
        : : "r" (clear), "r" (ass),                     \
            "i" (SMC_PL310_CTRL_VAL),                   \
            "i" (SMC_PL310_CTRL_IMM)                    \
        : "r0", "r3", "r12", "cc")
#else /* INCLUDE_PL310_NEED_SMC */
#define pl310ClearDisable(clear, ass, ctrl)             \
    __asm volatile (                                    \
        "STR    %1, [%0]\n\t"                           \
        "DSB\n\t"                                       \
        "ISB\n\t"                                       \
        "0:\n\t"                                        \
        "LDR    r3, [%0]\n\t"                           \
        "ANDS   r3, r3, %1\n\t"                         \
        "BNE    0b\n\t"                                 \
        "MOV    r0, #0\n\t"                             \
        "STR    r0, [%2]\n\t"                           \
        "DSB\n\t"                                       \
        "ISB\n\t"                                       \
        : : "r" (clear), "r" (ass), "r" (ctrl)          \
        : "r0", "r3", "cc")
#endif /* INCLUDE_PL310_NEED_SMC */

#ifdef INCLUDE_PL310_NEED_SMC
#define pl310CtrlWrite(value)                           \
    __asm volatile (                                    \
        ".arch_extension sec\n\t"                       \
        "PUSH   {r4-r11}\n\t"                           \
        "MOV    r0, %0\n\t"                             \
        "MOVW   r12, %1\n\t"                            \
        "SMC    %2\n\t"                                 \
        "POP    {r4-r11}\n\t"                           \
        : : "r" (value),                                \
            "i" (SMC_PL310_CTRL_VAL),                   \
            "i" (SMC_PL310_CTRL_IMM)                    \
        : "r0", "r12", "cc")
#else /* INCLUDE_PL310_NEED_SMC */
#define pl310CtrlWrite(ctrl, value)                     \
    __asm volatile (                                    \
        "STR    %1, [%0]\n\t"                           \
        "DSB\n\t"                                       \
        "ISB\n\t"                                       \
        : : "r" (ctrl), "r" (value)                     \
        )
#endif /* INCLUDE_PL310_NEED_SMC */

#ifdef INCLUDE_PL310_NEED_SMC
#define pl310AuxCtrlWrite(value)                        \
    __asm volatile (                                    \
        ".arch_extension sec\n\t"                       \
        "PUSH   {r4-r11}\n\t"                           \
        "MOV    r0, %0\n\t"                             \
        "MOVW   r12, %1\n\t"                            \
        "SMC    %2\n\t"                                 \
        "POP    {r4-r11}\n\t"                           \
        : : "r" (value),                                \
            "i" (SMC_PL310_AUXCTRL_VAL),                \
            "i" (SMC_PL310_AUXCTRL_IMM)                 \
        : "r0", "r12", "cc")
#else /* INCLUDE_PL310_NEED_SMC */
#define pl310AuxCtrlWrite(auxctrl, value)               \
    __asm volatile (                                    \
        "STR    %1, [%0]\n\t"                           \
        "DSB\n\t"                                       \
        "ISB\n\t"                                       \
        : : "r" (auxctrl), "r" (value)                  \
        )
#endif /* INCLUDE_PL310_NEED_SMC */

#ifdef INCLUDE_PL310_NEED_SMC
#define pl310LatCtrlWrite(tagLat, datLat)               \
    __asm volatile (                                    \
        ".arch_extension sec\n\t"                       \
        "PUSH   {r4-r11}\n\t"                           \
        "MOV    r0, %0\n\t"                             \
        "MOV    r1, %1\n\t"                             \
        "MOVW   r12, %2\n\t"                            \
        "SMC    %3\n\t"                                 \
        "POP    {r4-r11}\n\t"                           \
        : : "r" (tagLat), "r" (datLat),                 \
            "i" (SMC_PL310_AUXCTRL_VAL),                \
            "i" (SMC_PL310_AUXCTRL_IMM)                 \
        : "r0", "r1", "r12", "cc")
#else /* INCLUDE_PL310_NEED_SMC */
#define pl310LatCtrlWrite(tagLat, datLat, tagLatCtrl,   \
                          datLatCtrl)                   \
    __asm volatile (                                    \
        "STR    %0, [%2]\n\t"                           \
        "STR    %1, [%3]\n\t"                           \
        "DSB\n\t"                                       \
        "ISB\n\t"                                       \
        : : "r" (tagLat), "r" (datLat),                 \
            "r" (tagLatCtrl), "r" (datLatCtrl)          \
        )
#endif /* INCLUDE_PL310_NEED_SMC */

#endif /* __DCC__ */

/* typedefs */

/* locals */

/* globals */

/* externs */

/******************************************************************************
*
* vxActlrSet - ACTLR register set
*
* This routine sets the ACTLR register.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxActlrSet
    (
    UINT32 value
    )
    {
    writeActlr (value);
    }

/******************************************************************************
*
* vxActlrGet - ACTLR register get
*
* This routine gets the ACTLR register content.
*
* RETURNS: current ACTLR value
*
* ERRNO: N/A
*
* \NOMANUAL
*/

UINT32 vxActlrGet
    (
    void
    )
    {
    return readActlr ();
    }

#if defined (_WRS_CONFIG_ARMV7_CORE_GENERIC) || \
    defined (_WRS_CONFIG_ARMV7_CORE_CTX_A15)

/******************************************************************************
*
* vxL2ActlrSet - L2ACTLR register set
*
* This routine sets the L2ACTLR register.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxL2ActlrSet
    (
    UINT32 value
    )
    {
    writeL2Actlr (value);
    }

/******************************************************************************
*
* vxL2ActlrGet - L2ACTLR register get
*
* This routine gets the L2ACTLR register content.
*
* RETURNS: current L2ACTLR value
*
* ERRNO: N/A
*
* \NOMANUAL
*/

UINT32 vxL2ActlrGet
    (
    void
    )
    {
    return readL2Actlr ();
    }

#endif /* _WRS_CONFIG_ARMV7_CORE_GENERIC || _WRS_CONFIG_ARMV7_CORE_CTX_A15 */

/******************************************************************************
*
* vxPl310ClearDisable - clear and disable L2 cache
*
* This routine clears and disables PL310 L2 cache. It is writen in inline ASM
* code due to between the clear and disable, stack push should be avoid.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxPl310ClearDisable
    (
    UINT32 addr, UINT32 ass, UINT32 ctrl
    )
    {
#ifdef INCLUDE_PL310_NEED_SMC
    pl310ClearDisable (addr, ass);
#else /* INCLUDE_PL310_NEED_SMC */
    pl310ClearDisable (addr, ass, ctrl);
#endif /* INCLUDE_PL310_NEED_SMC */
    }

/******************************************************************************
*
* vxPl310CtrlWrite - PL310 L2 cache control register write
*
* This routine sets the PL310 L2 cache control register.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxPl310CtrlWrite
    (
    UINT32 addr,
    UINT32 value
    )
    {
#ifdef INCLUDE_PL310_NEED_SMC
    pl310CtrlWrite (value);
#else /* INCLUDE_PL310_NEED_SMC */
    pl310CtrlWrite (addr, value);
#endif /* INCLUDE_PL310_NEED_SMC */
    }

/******************************************************************************
*
* vxPl310CtrlWrite - PL310 L2 cache auxiliary control register write
*
* This routine sets the PL310 L2 cache auxiliary control register.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxPl310AuxCtrlWrite
    (
    UINT32 addr,
    UINT32 value
    )
    {
#ifdef INCLUDE_PL310_NEED_SMC
    pl310AuxCtrlWrite (value | L2_AUX_CTL_NS_INT_ACCESS);
#else /* INCLUDE_PL310_NEED_SMC */
    pl310AuxCtrlWrite (addr, value);
#endif /* INCLUDE_PL310_NEED_SMC */
    }

/******************************************************************************
*
* vxPl310LatCtrlWrite - PL310 L2 cache latency control register write
*
* This routine sets the PL310 L2 cache latency control register.
*
* RETURNS: NA
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxPl310LatCtrlWrite
    (
    UINT32 tagLat,
    UINT32 datLat,
    UINT32 tagLatCtrl,
    UINT32 datLatCtrl)
    {
#ifdef INCLUDE_PL310_NEED_SMC
    pl310LatCtrlWrite (tagLat, datLat);
#else /* INCLUDE_PL310_NEED_SMC */
    pl310LatCtrlWrite (tagLat, datLat, tagLatCtrl, datLatCtrl);
#endif /* INCLUDE_PL310_NEED_SMC */
    }

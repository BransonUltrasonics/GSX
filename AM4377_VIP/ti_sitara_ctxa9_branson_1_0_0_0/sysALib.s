/* sysALib.s - generic ARMv7 system entry */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01jul14,swu  created (US40646)
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly language.  
It contains the entry code, sysInit(), for VxWorks images that start running 
from RAM, such as 'vxWorks'.  These images are loaded into memory by some 
external program (e.g., a boot ROM) and then started.  The routine sysInit() 
must come first in the text segment. Its job is to perform the minimal setup 
needed to call the generic C routine usrInit().

sysInit() masks interrupts in the processor and the interrupt controller, also 
sets the initial stack pointer. Other hardware and device initialization is 
performed later in the usrInit().

NOTE
The routines in this module don't use the "C" frame pointer %r11@ ! or establish
a stack frame.
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
#include <asm.h>
#include <regs.h>
#include <arch/arm/arm.h>
#include "prjParams.h"

        /* defines */

#define ARM_GIC_BASE                    (0x48240000)
#define GIC_DIST                        (ARM_GIC_BASE + 0x1000)
#define GIC_CPU                         (ARM_GIC_BASE + 0x0100)
#define ARM_GIC_CPU_CONTROL             (GIC_CPU)
#define ARM_GIC_DIST_CONTROL            (GIC_DIST)
#define GIC_CPU_Control                 (GIC_CPU      + 0x0000)

        /* exports */

        FUNC_EXPORT (sysInit)
        DATA_IMPORT (gpDtbInit)

        /* externals */

        FUNC_IMPORT (usrInit)

#ifdef INCLUDE_STANDALONE_DTB

        .data

#define TO_STRING(exp)                  #exp
#define DTS_ASM_FILE(file)              TO_STRING (file.s)

#include DTS_ASM_FILE (DTS_FILE)

#endif /* INCLUDE_STANDALONE_DTB */

#if defined (__GNUC__)
        .section .text.entry, "ax"
#elif defined(__DCC__)
        .section .text.entry, 4, "rx"
#else
#error "TOOL not supported!"
#endif

/*******************************************************************************
*
* sysInit - start after boot
*
* This routine is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up
* the stack, and jumps to the C routine usrInit().
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* SYNOPSIS
* \ss
* VOID sysInit
*     (
*     void * pDtb,  /@ physical pointer for device-tree in RAM  @/
*     )
* \se
*
* RETURNS: N/A
*
* sysInit ()              /@ THIS IS NOT A CALLABLE ROUTINE @/
*
*/

_ARM_FUNCTION (sysInit)

#if ARM_THUMB2
        .code   32
#endif

        /* disable interrupts in CPU */

        MRS     r1, cpsr
        ORR     r1, r1, #(I_BIT | F_BIT)
        MSR     cpsr, r1

        /* set initial stack pointer so stack grows down from start of code */

        ADR     sp, sysInit
        MOV     fp, #0     /* initialize frame pointer */

        LDR     r1, =sysInit
        ADR     r6, sysInit
        SUB     r6, r6, r1

#ifdef INCLUDE_STANDALONE_DTB
        LDR     r0, =dt_blob_start
#endif /* INCLUDE_STANDALONE_DTB */

        LDR     r1, =gpDtbInit
        ADD     r1, r1, r6
        STR     r0, [r1]

        /* disable interrupts to CPU */

        LDR     r0, =ARM_GIC_CPU_CONTROL
        MOV     r1, #0
        STR     r1, [r0]

        /* disable interrupts for distributor */

        LDR     r0, =ARM_GIC_DIST_CONTROL
        STR     r1, [r0]

2:
        LDR     r1, =usrInit
        ADD     r1, r1, r6
        MOV     r0, #2      /* legacy BOOT_COLD */
        BX      r1
FUNC_END (sysInit)

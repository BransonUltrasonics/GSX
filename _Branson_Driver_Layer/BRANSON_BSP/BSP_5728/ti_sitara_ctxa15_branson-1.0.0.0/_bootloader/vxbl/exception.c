/* exception.c - exception handling of BootStrap for ARM */

/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15mar16,jnl  created (US75898)
*/

/*
DESCRIPTION
This file contains the exception handling code of ARMv7 CPU.
*/

#include <vxboot.h>
#include <stdio.h>
#include <exc.h>

struct arch_regs
    {
    uint32_t r[13];
    uint32_t sp;
    uint32_t lr;
    uint32_t pc;
    uint32_t cpsr;
    };

/*******************************************************************************
*
* dumpRegs - dump register contents
*
* This routine dumps register contents
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

static void dumpRegs
    (
    struct arch_regs *regs
    )
    {
    int i;
    (void)printf("cpsr = 0x%08x    pc   = 0x%08x\n", regs->cpsr, regs->pc);
    (void)printf("lr   = 0x%08x    sp   = 0x%08x\n", regs->lr  , regs->sp);
    for (i = 0; i < 13; i++)
        {
        (void)printf("r[%2d]= 0x%08x    ", i, regs->r[i]);
        if (i % 2 != 0)
            (void)printf("\n");
        }
    (void)printf("\n\n");
    }

/*******************************************************************************
*
* handleException - handle ARM exception
*
* This routine handles ARM exception
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void handleException
    (
    int type,
    struct arch_regs *regs
    )
    {
    (void)printf("**** Exception Occurred ****\n\n");

    switch (type)
        {
        case EXC_UNDEF:
            (void)printf("undefined instruction\n");
            break;
        case EXC_SVCCALL:
            (void)printf("svc/swi call\n");
            break;
        case EXC_PABT:
            (void)printf("prefetch abort\n");
            break;
        case EXC_DABT:
            (void)printf("data abort\n");
            break;
        case EXC_HYPTRAP:
            (void)printf("hyper trap\n");
            break;
        case EXC_IRQ:
            (void)printf("irq exception\n");
            break;
        case EXC_FIQ:
            (void)printf("fiq exception\n");
            break;
        default:
            (void)printf("unknown exception\n");
        }

    (void)printf("\n");

    dumpRegs(regs);

    (void)printf("**** Please Reset Board ****\n\n");
    for(;;);
    }


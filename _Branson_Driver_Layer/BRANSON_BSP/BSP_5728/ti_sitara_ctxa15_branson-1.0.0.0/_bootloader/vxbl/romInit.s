/* start.s - startup routine for ti_sitara_ctxa15 */

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
This file contains the entry code of ti_sitara_ctxa8 bootloader.
*/

#define _ASMLANGUAGE
#include "ti_am572x.h"
#include "exc.h"

    .extern wrs_kernel_bss_end
    .globl _romInit

    .text
    .balign 4

_romInit:

    .word wrs_kernel_bss_end - _romInit - AM572X_ROM_HEADER_SIZE
    .word AM572X_NANDBOOT_RAM_ADRS

    ldr pc, TEXT_ENTRY

TEXT_ENTRY:
    .word __reset
    .fill AM572X_INIT_STACK_SIZE - AM572X_ROM_HEADER_SIZE, 1, 0

__reset:

    /* disable interrupts in CPU and switch to SVC32 mode */

    mrs    r0, cpsr
    bic    r0, r0, #0x1f
    orr    r0, r0, #0xd3
    msr    cpsr, r0

    /* Invalidate L1 I/D */
    mov    r0, #0
    mcr    p15, 0, r0, c8, c7, 0
    mcr    p15, 0, r0, c7, c5, 0
    mcr    p15, 0, r0, c7, c5, 6
    mcr    p15, 0, r0, c7, c10, 4
    mcr    p15, 0, r0, c7, c5, 4

    /* disable MMU stuff and caches */
    mrc    p15, 0, r0, c1, c0, 0
    bic    r0, r0, #0x00002000
    bic    r0, r0, #0x00000007

    /* normal exception vector selected */

    bic    r0, r0, #0x00002000
    ORR    r0, r0, #0x00000002
    ORR    r0, r0, #0x00000800
    ORR    r0, r0, #0x00001000
    mcr    p15, 0, r0, c1, c0, 0

    _CORTEX_AR_ENTIRE_DATA_CACHE_OP(c14)    /* flush+invalidate all D-cache */

    mrc    p15, 0, r0, c1, c0, 1
    bic    r0, r0, #0x2
    mcr    p15, 0, r0, c1, c0, 1

    /* setup exception vector */

    ldr    r0, _vector
    mcr    p15, 0, r0, c12, c0, 0

    /* load stack and run C routines */

    ldr    sp, SRAM_STACK

    bl     sysHwInit

    /* deadloop, in case we return from errors.*/

dead:
    b   dead            /* loop here forever */

    .fill 32,1,0

__vector:
    ldr pc, _reset
    ldr pc, _undef
    ldr pc, _svc_call
    ldr pc, _prefetch_abort
    ldr pc, _data_abort
    ldr pc, _hyp_trap
    ldr pc, _irq
    ldr pc, _fiq
    b .

_vector:
    .word __vector
_reset:
    .word __reset
_undef:
    .word __undef
_svc_call:
    .word __svc_call
_prefetch_abort:
    .word __prefetch_abort
_data_abort:
    .word __data_abort
_hyp_trap:
    .word __hyp_trap
_irq:
    .word __irq
_fiq:
    .word __fiq

  /*
   ----------
   | spsr   |
   ----------
   | pc     |
   ----------
   | lr_svc |
   ----------
   | sp_svc |
   ---------- <---- r4
   | r12    |
   ----------
   | r11    |
   ----------
   | ...    |
   ----------
   | r0     |
   ---------- <--- sp_und
   */


__undef:
  ldr sp, _exceptionStackTop  /* load the exception stack top */
  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_UNDEF   /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #4       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .


__svc_call:
  /*
   * svc/swi call doesn't need to load sp here,
   * as it won't change the privilege mode
   *
   */

  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_SVCCALL /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #4       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .

__prefetch_abort:
  ldr sp, _exceptionStackTop  /* load the exception stack top */
  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_PABT    /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #4       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .

__data_abort:
  ldr sp, _exceptionStackTop  /* load the exception stack top */
  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_DABT    /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #8       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .

__hyp_trap:
  ldr sp, _exceptionStackTop  /* load the exception stack top */
  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_PABT    /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #4       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .

__irq:
  ldr sp, _exceptionStackTop  /* load the exception stack top */
  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_IRQ    /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #4       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .

__fiq:
  ldr sp, _exceptionStackTop  /* load the exception stack top */
  sub sp, sp, #16
  mov r4, sp

  stmdb sp!, {r0-r12}  /* store r0-r12 */

  /* now r0-r12 are free to use */

  mov r0, #EXC_FIQ     /* set the exception type */
  mov r1, sp           /* save the stack pointer */

  mov r2, lr           /* r2 = return address */
  sub r2, lr, #4       /* save pc, r2 = lr - 4   */
  str r2, [r4, #8]

  mrs r3, spsr         /* save the spsr in this mode */
  str r3, [r4, #12]

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  b __exc_common

  b .

__exc_common:

  /*
   * entry to the common exception hander:
   *    r0 = exception type
   *    r1 = exception stack pointer 
   *    r2 = exception pc (already saved on stack)
   *    r3 = exception sp 
   *    r4 = sp + sizeof(struct regs)
   */

  /* back to svc32 mode */

  mrs r5, cpsr
  bic r5, r5, #0x1f
  orr r5, r5, #0x13
  msr cpsr, r5

  /* now in svc32 mode */

  str lr, [r4, #4]  /* store lr_svc */
  str sp, [r4, #0]  /* store sp_svc */

  /*
   * call c handler:
   *     void handleException(int type, struct regs *reg)
   */

  bl handleException

  /* should never be here */

  b .

SRAM_STACK:
    .word (AM572X_NANDBOOT_RAM_ADRS + AM572X_INIT_STACK_SIZE)

_exceptionStackTop:
    .word __exceptionStackTop

__exceptionStackBottom:
    .fill 1024,1,0
__exceptionStackTop:

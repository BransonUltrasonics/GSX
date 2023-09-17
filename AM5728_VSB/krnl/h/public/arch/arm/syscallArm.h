/* syscallArm.h - ARM specific System Call Infrastructure header */

/*
 * Copyright (c) 2003, 2005, 2007, 2009, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12dec16,zli  Fix arm64 building (US90845)
16apr16,h_k  added ARM64 support (F5261)
21may14,c_t  adjust stack (V7PRO-869)
10may14,cfm  added a new macro: SYSCALL_USER_STATE_GET (V7PRO-791)
29apr14,c_t  add fp in syscall entry (V7PRO-747).
02apr14,gvm  Fix for VXW7-2874
20feb14,cfm  updated copyright
03dec09,cww  Moved ARM specific macros from syscallLibP.h
04may07,zl   updated portable library build configuration.
04mar05,scm  _SYSCALL_USER_SP_CHECK break signals alt stack option...
23feb05,scm  re-assign "spare" to user link reg...
19nov03,cjj  replaced pStackLimit by pStackEnd in _SYSCALL_USER_SP_CHECK
14oct03,scm  enhanced, flushed out, and debugging.
03oct03,jb   written.
*/

/*
DESCRIPTION
This header contains ARM specific definitions and constants used by
the System Call Infrastructure library.

*/

#ifndef __INCsyscallArmh
#define __INCsyscallArmh

#ifndef _ASMLANGUAGE
#include <taskLib.h>
#endif

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#include <arch/arm/regsArm.h>
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */

#ifdef __cplusplus
extern "C" {
#endif

    /* defines */

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#define SYSCALL_ENTRY_FRAME_SIZE        (16 * ARM_REG_SIZE)
#else /* !_WRS_CONFIG_CPU_ARMARCH8A */
#define SYSCALL_ENTRY_FRAME_SIZE        60
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */

#define _SYSCALL_USER_SP_CHECK(sp)

    /* typedefs */

#ifndef _ASMLANGUAGE

    /*
     * The SYSCALL_ENTRY_STATE structure defines the saved machine state
     * when the system call trap is taken. This information is architecture
     * specific, and is used by the system call dispatcher to restart system
     * calls that have been interrupted by the occurance of signals.
     * System call restart is achieved by restoring the saved state at the
     * time of the system call trap, and re-issuing the system call.
     * Arguments to the system call are saved on the kernel stack, and
     * the address of that array is passed as an argument to the dispatcher.
     * The layout of this structure must exactly match the ordering of members
     * of the system call entry frame in src/arch/arm/syscallALib.s.
     */

typedef struct syscall_entry_state
    {
    ULONG    args[8];   /* argument list (r0-r3 and sp[0]-sp[3]) for armv7 */
                        /* argument list (r0-r7)  for armv8 */
    ULONG    scn;       /* System Call Number (SCN) in r12 */
    ULONG    statusReg; /* saved Status Register (spsr) */
    INSTR    *pc;       /* Trap return address (lr) */
    ULONG    *pUStack;  /* user-mode stack pointer */
    ULONG    usrLnk;    /* user-mode link register */
    ULONG    tlsBase;   /* user-mode Thread Local Storage Area base register */
    ULONG    fp;        /* frame pointer (r11 for armv7 and r29 for armv8) */
#ifdef _WRS_CONFIG_CPU_ARMARCH8A
    ULONG    reserved;  /* reserved to be aligned on stack alignment */
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */
    } SYSCALL_ENTRY_STATE;

#ifdef _WRS_CONFIG_CPU_ARMARCH8A
#define SYSCALL_USER_STATE_GET(tid)                                       \
        ((SYSCALL_ENTRY_STATE *) ((((WIND_TCB *) (tid))->pExcStackBase) - \
                                  SYSCALL_ENTRY_FRAME_SIZE))

#define ARM_RTP_STACK_OFFSET     24  /* ARM pushes r2, SCn, lr onto stack 
                                        prior to signal handling */
#else /* !_WRS_CONFIG_CPU_ARMARCH8A */
#define ARM_RTP_STACK_OFFSET     12  /* ARM pushes r2, SCn, lr onto stack 
                                        prior to signal handling */

#define SYSCALL_USER_STATE_GET(tid)                                       \
        ((SYSCALL_ENTRY_STATE *) ((((WIND_TCB *) (tid))->pExcStackBase) - \
                                  (sizeof (SYSCALL_ENTRY_STATE)         + \
                                   ARM_RTP_STACK_OFFSET)))
#endif /* _WRS_CONFIG_CPU_ARMARCH8A */

#define SYSCALL_USER_SP_GET(tid) (char *)                                 \
        (SYSCALL_USER_STATE_GET (tid)->pUStack)

#define SYSCALL_USER_PC_GET(tid) (void *)                                 \
        (SYSCALL_USER_STATE_GET (tid)->pc)

#define SYSCALL_USER_SCN_GET(tid) (int)                                   \
        (SYSCALL_USER_STATE_GET (tid)->scn)

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCsyscallArmh */

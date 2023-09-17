/* esfArm.h - ARM exception stack frames */

/*
 * Copyright (c) 1996, 2004, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04jun14,cfm  add tlsbase to ESF, so structs REG_SET & ESF overlap correctly
09sep04,scm  add ttbase to ESF, so structs REG_SET & ESF overlap correctly
09may96,cdp  created
*/

#ifndef __INCesfArmh
#define __INCesfArmh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

/*
 * The Exception Stack Frame
 * On the ARM, this is faked by a stub and the pointer
 * passed back points to the PC and CPSR at the end of the
 * register structure. If that changes, so must this.
 *
 * I.E. REG_SET struct & ESf struct 'overlap' on last 3
 * elements of REG_SET (pc,cpsr,ttbase)...
 */

typedef struct
    {
    INSTR * pc;         /* program counter */
    ULONG   cpsr;       /* current PSR */
    ULONG   ttbase;     /* Trans Table Base */
    ULONG   tlsbase;    /* tls Base */
    UINT32  vecAddr;    /* address of exception vector => type */
    } ESF;

#endif  /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif  /* __INCesfArmh */

/* smcArm.h - ARM SMC Call Convention (SMCCC) header file */

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
15jul16,yya  created (F6283)
*/

#ifndef __INCsmcArmh
#define __INCsmcArmh

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The SMC call convention is defined in the following document:
 * SMC CALLING CONVENTION System Software on ARM Platforms
 * Document number: ARM DEN 0028A
 */

#define ARM_SMCCC_STD_CALL              0
#define ARM_SMCCC_FAST_CALL             1
#define ARM_SMCCC_TYPE_SHIFT            31

#define ARM_SMCCC_SMC_32                0
#define ARM_SMCCC_SMC_64                1
#define ARM_SMCCC_CALL_CONV_SHIFT       30

#define ARM_SMCCC_OWNER_MASK            0x3F
#define ARM_SMCCC_OWNER_SHIFT           24

#define ARM_SMCCC_FUNC_MASK             0xFFFF

#define ARM_SMCCC_IS_FAST_CALL(smc_val)    \
    ((smc_val) & (ARM_SMCCC_FAST_CALL << ARM_SMCCC_TYPE_SHIFT))
#define ARM_SMCCC_IS_64(smc_val) \
    ((smc_val) & (ARM_SMCCC_SMC_64 << ARM_SMCCC_CALL_CONV_SHIFT))
#define ARM_SMCCC_FUNC_NUM(smc_val)    ((smc_val) & ARM_SMCCC_FUNC_MASK)
#define ARM_SMCCC_OWNER_NUM(smc_val) \
    (((smc_val) >> ARM_SMCCC_OWNER_SHIFT) & ARM_SMCCC_OWNER_MASK)

#define ARM_SMCCC_CALL_VAL(type, calling_convention, owner, func_num) \
    (((type) << ARM_SMCCC_TYPE_SHIFT) | \
    ((calling_convention) << ARM_SMCCC_CALL_CONV_SHIFT) | \
    (((owner) & ARM_SMCCC_OWNER_MASK) << ARM_SMCCC_OWNER_SHIFT) | \
    ((func_num) & ARM_SMCCC_FUNC_MASK))

#define ARM_SMCCC_OWNER_ARCH            0
#define ARM_SMCCC_OWNER_CPU             1
#define ARM_SMCCC_OWNER_SIP             2
#define ARM_SMCCC_OWNER_OEM             3
#define ARM_SMCCC_OWNER_STANDARD        4
#define ARM_SMCCC_OWNER_TRUSTED_APP     48
#define ARM_SMCCC_OWNER_TRUSTED_APP_END 49
#define ARM_SMCCC_OWNER_TRUSTED_OS      50
#define ARM_SMCCC_OWNER_TRUSTED_OS_END  63

/*
 * OP-TEE SMC request definition.
 * These should agree with the definition of OP-TEE.
 */

#define VX_OPTEE_SMC_FUNCID_BOOT_SECONDARY      12
#define VX_OPTEE_SMC_BOOT_SECONDARY  \
    ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL, ARM_SMCCC_SMC_32, \
                ARM_SMCCC_OWNER_TRUSTED_OS, VX_OPTEE_SMC_FUNCID_BOOT_SECONDARY)

#define VX_OPTEE_SMC_FUNCID_PLATFORM_DEFINED    0x100
#define VX_OPTEE_SMC_PLATFORM_DEFINED \
    ARM_SMCCC_CALL_VAL(ARM_SMCCC_FAST_CALL, ARM_SMCCC_SMC_32, \
                ARM_SMCCC_OWNER_TRUSTED_OS, VX_OPTEE_SMC_FUNCID_PLATFORM_DEFINED)

/* smc call arguments are in register r0-r7 */

typedef struct arm_smc_regs
    {
    int     a0;
    int     a1;
    int     a2;
    int     a3;
    int     a4;
    int     a5;
    int     a6;
    int     a7;
    } ARM_SMC_REGS;

/* smc function prototype */

void armSmcCall(ARM_SMC_REGS * input, ARM_SMC_REGS * output);

#ifdef __cplusplus
}
#endif

#endif /* __INCsmcArmh */

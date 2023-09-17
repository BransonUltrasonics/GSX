/* secArm.h - ARM processor secure mode configuration header file */

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

#ifndef __INCsecArmh
#define __INCsecArmh

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

#endif /* _ASMLANGUAGE */

#define ARM_SECURE_CONFIG_STANDARD  0
#define ARM_SECURE_CONFIG_OPTEE     1
#define ARM_SECURE_CONFIG_TISM      2

#ifndef _ASMLANGUAGE

IMPORT UINT32 armSecureConfigGet();

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* __INCsecArmh */

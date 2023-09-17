/* usrTrustZone.c - TrustZone support */

/*
 * Copyright (c) 2016 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15jul16,yya  created (F6283)
*/

/* includes */

#include <vxWorks.h>
#include <prjParams.h>
#include <arch/arm/secArm.h>

/*****************************************************************************
*
* armSecureConfigGet - get ARM processor secure configuration
*
* This function returns the secure configuration of the ARM processor.
* Currently following configuration are supported:
*
*   Standard: VxWorks is running in S world; NS is not used;
*   OP-TEE  : VxWorks rs running in NS; Secure monitor is OP-TEE;
*   TISM    : VxWorks is running is NS; Secure monitor is TI secure monitor
*
* Typically user of this function is ARCH code which depends on the security
* state, for example, vxCpuInit() would avoid writing to ACTLR if OS is running
* in NS, as ACTLR is not accessbile in NS state.
*
* Device driver may also query this, for example, the PL310 driver's cache
* enable/disable behavior depends on whether it is running in Secure or NS
* world.
*
* RETURNS: index of the secure configuration as described above.
*
* \NOMANUAL
*/

UINT32 armSecureConfigGet()
    {
#if defined(INCLUDE_SECURE_CONFIG_STANDARD)
    return ARM_SECURE_CONFIG_STANDARD;
#elif defined(INCLUDE_SECURE_CONFIG_OPTEE)
    return ARM_SECURE_CONFIG_OPTEE;
#elif defined(INCLUDE_SECURE_CONFIG_TISM)
    return ARM_SECURE_CONFIG_TISM;
#else
#error "Unknown secure config" 
#endif
    }


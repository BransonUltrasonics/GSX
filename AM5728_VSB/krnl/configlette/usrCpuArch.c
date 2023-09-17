/* usrCpuArchInit.c - Architecture specific CPU initialization */

/*
 * Copyright (c) 2013, 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17oct16,wqi  added cpuIndexMap only when clusters and smp enabled
18aug16,mpc  added ARM64 support (F5261)
15jan16,yya  added cpuIndexMap for parsing CPUs in clusters. (US66795)
20dec13,cww  created
*/

/*
DESCRIPTION
This file contains architecture specific code used to initialize the CPU.  It
is included by the generic usrAimCpuInit.c
*/

/* externals */

extern STATUS cpuArmLibInit(void);

#if defined(_WRS_CONFIG_SMP) && defined(_WRS_CONFIG_MULTI_CLUSTERS)
UINT32 cpuIndexMap[VX_MAX_SMP_CPUS] = {ARM_CPU0_INDEX};
#endif /* _WRS_CONFIG_SMP && _WRS_CONFIG_MULTI_CLUSTERS */

/*******************************************************************************
*
* usrCpuArchInit - initialize primary CPU
*
* This routine initializes various features of the primary CPU.
*
* It is only called from usrAimCpuInit() for primary CPU initialization.
*
* RETURNS: NONE
*
* \NOMANUAL
*/

STATUS usrCpuArchInit (void)
    {
    return cpuArmLibInit();
    }


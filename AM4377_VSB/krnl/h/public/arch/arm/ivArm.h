/* ivArm.h - ARM interrupt vectors */

/*
 * Copyright (c) 1996, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20feb14,cfm  updated copyright
09may96,cdp  created
*/

#ifndef __INCivArmh
#define __INCivArmh

#ifdef __cplusplus
extern "C" {
#endif

/* macros to convert interrupt vectors <-> interrupt numbers */

#define IVEC_TO_INUM(intVec)    ((int) (intVec))
#define INUM_TO_IVEC(intNum)    ((VOIDFUNCPTR *) (intNum))

#ifdef __cplusplus
}
#endif

#endif  /* __INCivArmh */

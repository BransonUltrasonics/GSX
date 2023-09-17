/* coprocArm.h - coprocessor management library header */

/*
 * Copyright (c) 2004-2007, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
15may12,jmp  CQID: Fix WIND00309977 : Added VX_COPROCS_ALL_TASK
             definition.
28apr10,pad  Moved extern C statement after include statements.
02oct07,jb   Move VFP to FP equivilent, part of fix for WIND00093069
30jun05,scm  add support for ARM VFP...
04aug04,scm  fix permissions...
03aug04,scm  written based on coprocI86.h 01a.
*/

#ifndef __INCcoprocArmh
#define __INCcoprocArmh

/* includes */

#include <taskLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define VX_FP_TASK         VX_COPROC1_TASK
#define VX_VFP_TASK        VX_FP_TASK

#define VX_COPROCS_ALL_TASK  VX_FP_TASK

#ifdef __cplusplus
}
#endif

#endif /* __INCcoprocArmh */

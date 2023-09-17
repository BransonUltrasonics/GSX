/* exc.h - arm exc handling header file */

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

#ifndef __INCexch__
#define __INCexch__

#include <vxboot.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXC_UNDEF   0x1
#define EXC_SVCCALL 0x2
#define EXC_PABT    0x4
#define EXC_DABT    0x8
#define EXC_HYPTRAP 0x10
#define EXC_IRQ     0x20
#define EXC_FIQ     0x40

#ifdef __cplusplus
}
#endif

#endif

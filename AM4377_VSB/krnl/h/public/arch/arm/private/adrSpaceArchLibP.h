/* adrSpaceArchLibP.h - address space header file for ARM */

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
18aug16,mpc  added LP64 support
02sep13,zgl  created based on PPC32 version 01b.
*/

#ifndef __INCadrSpaceArchLibPh
#define __INCadrSpaceArchLibPh

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WRS_CONFIG_LP64

/* virtual memory region definitions */

#define KERNEL_SYS_MEM_RGN_BASE         0xffffffff80000000
#define KERNEL_SYS_MEM_RGN_SIZE         0x0000000080000000

#define KERNEL_VIRT_POOL_RGN_BASE       0xffff800000000000
#define KERNEL_VIRT_POOL_RGN_SIZE       0x0000080000000000

#define KERNEL_DIRECT_MAP_RGN_BASE      0xfffff00000000000
#define KERNEL_DIRECT_MAP_RGN_SIZE      0x0000080000000000

#ifdef	_WRS_CONFIG_RTP

#define RTP_PRIVATE_RGN_BASE            0x0000000000200000UL
#define RTP_PRIVATE_RGN_SIZE            0x000007ffffe00000UL

#endif	/* _WRS_CONFIG_RTP */

#define SHARED_RGN_BASE                 0x0000780000000000
#define SHARED_RGN_SIZE                 0x0000080000000000

#define NON_CANONICAL_RGN_BASE          0x0001000000000000UL
#define NON_CANONICAL_RGN_SIZE          0xfffe000000000000UL

#else /* !LP64 */    

/* virtual memory region definitions */

#ifdef _WRS_CONFIG_RTP
#define KERNEL_SYS_MEM_RGN_BASE     0x00000000
#define KERNEL_SYS_MEM_RGN_SIZE     0x20000000

#define KERNEL_VIRT_POOL_RGN_BASE   0x20000000
#define KERNEL_VIRT_POOL_RGN_SIZE   0x60000000

#define RTP_PRIVATE_RGN_BASE        0x80000000
#define RTP_PRIVATE_RGN_SIZE        0x60000000

#define SHARED_RGN_BASE             0xE0000000
#define SHARED_RGN_SIZE             0x20000000
#else /* !_WRS_CONFIG_RTP */
#define KERNEL_SYS_MEM_RGN_BASE     0x00000000
#define KERNEL_SYS_MEM_RGN_SIZE     0x20000000

#define KERNEL_VIRT_POOL_RGN_BASE   0x20000000
#define KERNEL_VIRT_POOL_RGN_SIZE   0xC0000000

#define RTP_PRIVATE_RGN_BASE        0
#define RTP_PRIVATE_RGN_SIZE        0

#define SHARED_RGN_BASE             0xE0000000
#define SHARED_RGN_SIZE             0x20000000
#endif /* _WRS_CONFIG_RTP */

#endif /* LP64 */
#ifdef __cplusplus
}
#endif

#endif /* __INCadrSpaceArchLibPh */


/* am4xxx.h - header files for TI AM5XXX processor */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
08sep15,yjw  created (US64528)
*/

#ifndef __INCam5xxxh
#define __INCam5xxxh

#ifdef __cplusplus
extern "C" {
#endif

/* Handy sizes */

#define SZ_4K                       (0x00001000)
#define SZ_8K                       (0x00002000)
#define SZ_16K                      (0x00004000)
#define SZ_32K                      (0x00008000)
#define SZ_64K                      (0x00010000)
#define SZ_128K                     (0x00020000)
#define SZ_256K                     (0x00040000)
#define SZ_512K                     (0x00080000)

#define SZ_1M                       (0x00100000)
#define SZ_2M                       (0x00200000)
#define SZ_4M                       (0x00400000)
#define SZ_8M                       (0x00800000)
#define SZ_16M                      (0x01000000)
#define SZ_32M                      (0x02000000)
#define SZ_64M                      (0x04000000)
#define SZ_128M                     (0x08000000)
#define SZ_256M                     (0x10000000)
#define SZ_512M                     (0x20000000)

#define SZ_1G                       (0x40000000)
#define SZ_2G                       (0x80000000)

#ifndef BIT
#define BIT(x)                      (1 << (x))
#endif

/* GIC */

#define GICD_CTRL                   (0x0)
#define GICD_ISENABLER0             (0x100)
#define GICD_ICENABLER0             (0x180)
#define GICD_ISPENDR0               (0x200)
#define GICD_ICPENDR0               (0x280)
#define GICD_ISACTIVER0             (0x300)
#define GICD_ICACTIVER0             (0x380)

#define GICC_CTRL                   (0x0)
#define GICC_PMR                    (0x4)
#define GICC_EOIR                   (0x10)

#define INT_LVL_MPCORE_CPC          (0)

#define SGI_INT_MAX                 (16)
#define ARM_GIC_IPI_COUNT           (16)    /* MPCore IPI count         */
#define SPI_START_INT_NUM           (32)    /* SPI start at ID32        */
#define PPI_START_INT_NUM           (16)    /* PPI start at ID16        */
#define GIC_INT_MAX_NUM             (1020)  /* GIC max interrupts count */

/* accessors */

#ifndef sysRead32
#define sysRead32(a)             (*(volatile unsigned int *)(a))
#endif /* sysRead32 */
#ifndef sysWrite32
#define sysWrite32(a, v)         (*(volatile unsigned int *)(a) = (v))
#endif /* sysWrite32 */
#ifndef sysRead16
#define sysRead16(a)             (*(volatile unsigned short *)(a))
#endif /* sysRead16 */
#ifndef sysWrite16
#define sysWrite16(a, v)         (*(volatile unsigned short *)(a) = (v))
#endif /* sysWrite16 */
#ifndef sysRead8
#define sysRead8(a)              (*(volatile unsigned char *)(a))
#endif /* sysRead8 */
#ifndef sysWrite8
#define sysWrite8(a, v)          (*(volatile unsigned char *)(a) = (v))
#endif /* sysWrite8 */

/* functions */

BOOL   am5xxxProbe     (char * compat);
void   am5xxxInit      (void);
void   am5xxxEarlyInit (void);
char * am5xxxModel     (void);
void   am5xxxUsDelay   (int us);
void   am5xxxReset     (int startType);
#ifdef _WRS_CONFIG_SMP
UINT32  am5xxxCpuAvail(void);
STATUS  am5xxxCpuEnable(UINT32 cpuId, WIND_CPU_STATE * cpuState);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCam5xxxh */

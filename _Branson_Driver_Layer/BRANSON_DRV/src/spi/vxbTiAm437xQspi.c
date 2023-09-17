/*
 * xyz.c
 *
 *  Created on: Jun 13, 2017
 *      Author: emerson
 */
/**
 * \file       qspi.c
 *
 * \brief      This file contains the function definitions for the device
 *             abstraction layer for QSPI.
 */


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtSpiLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <tickLib.h>
#include <types.h>
#include <vxbTiAm437xQspi.h>

#include <vxbAm437xQspiLib.h>
#include <hw_am437xqspi.h>

/* ========================================================================== */
/*                          Macros and Typedefs                               */
/* ========================================================================== */
/*#define  SPI_DBG_ON*/ 	/* turn on/off debug messages */
#ifdef  SPI_DBG_ON

#define DEBUG_PRINT 1
/* turning local symbols into global symbols */

#define SPI_DBG_OFF            0x00000000
#define SPI_DBG_ISR            0x00000001
#define SPI_DBG_RW             0x00000002
#define SPI_DBG_ERR            0x00000004
#define SPI_DBG_RTN            0x00000008
#define SPI_DBG_INFO		   0x0000000C	
#define SPI_DBG_ALL            0xffffffff

LOCAL UINT32 spiDbgMask = SPI_DBG_ALL;

#define SPI_DBG(mask, ...)                                              \
    do                                                                  \
        {                                                               \
        if ((spiDbgMask & mask) || (mask == SPI_DBG_ALL))               \
            {                                                           \
            if (_func_kprintf != NULL)                                  \
                {                                                       \
                (* _func_kprintf)(__VA_ARGS__);                         \
                }                                                       \
            }                                                           \
        }                                                               \
    while ((FALSE))
#else
#undef SPI_DBG
#define SPI_DBG(...)
#endif  /* SPI_DBG_ON */

/* Device default freq, 48M */
#define AM437X_SPI_CLK_DEFAULT         48000000

#define TI_AM437X_SPI_NAME	"ti,Am437xSpi"



/**
 *  Macro to extract Register Base Address. This macro extracts the base address
 *         from a driver id (which contains the driver structure ,which further contains the register base).
 *
 *  driver id (VXB_DEV_ID)
 */
#define MC_SPI_BAR(p)       (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regBase)
#define MMAP_SPI_BAR(p)       (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regFlashBase)

/**
 *  Macro to extract Register Handle. This macro extracts the register handle
 *         from a driver id (which contains the driver structure ,which further contains the register handle).
 *
 * p         driver id (VXB_DEV_ID)
 */
#define MC_SPI_HANDLE(p)    (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regHandle)
#define MMAP_SPI_HANDLE(p)    (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regFlashHandle)

/**
 * Macro to extract a field value. This macro extracts the field value
 *         from a 32-bit variable (which contains the register value).
 *         This macro does not read from actual register address, and only
 *         extracts the field from a variable.
 *
 * regVal         32-bit variable containing the register value.
 * REG_FIELD      Peripheral register bit field name, whose value has to
 *                        be extracted.
 */
#define CS_GET_FIELD(regVal, REG_FIELD)                                        \
    (((regVal) & (uint32_t) REG_FIELD##_MASK) >> (uint32_t) REG_FIELD##_SHIFT)

/**
 * Macro to write a specific field value. This macro first clears the
 *         specified field value and then performs "OR" of the field value which
 *         is shifted and masked. This will set the field value at its
 *         desired position.
 *
 * regVal         32-bit variable containing the register value.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_SET_FIELD(regVal, REG_FIELD, fieldVal)                              \
    ((regVal) = ((regVal) & (uint32_t) (~(uint32_t) REG_FIELD##_MASK)) |       \
                    ((((uint32_t) fieldVal) << (uint32_t) REG_FIELD##_SHIFT) & \
                    (uint32_t) REG_FIELD##_MASK))

/**
 * This macro calls read-modify-write API for 32 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_WRITE_FIELD32(pDev, regAddr, REG_FIELD, fieldVal)                            \
    (CS_WRITE_FIELD32_RAW(pDev, regAddr, (uint32_t) REG_FIELD##_MASK,                   \
                          (uint32_t) REG_FIELD##_SHIFT, (uint32_t) fieldVal))

/**
 * This macro calls read-modify-write API for 16 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_WRITE_FIELD16(pDev, regAddr, REG_FIELD, fieldVal)                            \
    (CS_WRITE_FIELD16_RAW(pDev, regAddr, (uint16_t) REG_FIELD##_MASK,                   \
                          (uint32_t) REG_FIELD##_SHIFT, (uint16_t) fieldVal))

/**
 * This macro calls read-modify-write API for 8 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_WRITE_FIELD8(pDev, regAddr, REG_FIELD, fieldVal)                             \
    (CS_WRITE_FIELD8_RAW(pDev, regAddr, (uint8_t) REG_FIELD##_MASK,                     \
                         (uint32_t) REG_FIELD##_SHIFT, (uint8_t) fieldVal))

/**
 * This macro calls read field API for 32 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, from which specified
 *                        bit-field value has to be read.
 *  retval Value of the bit-field
 */
#define CS_READ_FIELD32(pDev, regAddr, REG_FIELD)                                      \
    (CS_READ_FIELD32_RAW(pDev, regAddr, (uint32_t) REG_FIELD##_MASK,                   \
                          (uint32_t) REG_FIELD##_SHIFT))

/**
 *  This macro calls read field API for 16 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 * retval Value of the bit-field
 */
#define CS_READ_FIELD16(pDev, regAddr, REG_FIELD)                                      \
    (CS_READ_FIELD16_RAW(pDev, regAddr, (uint16_t) REG_FIELD##_MASK,                   \
                          (uint32_t) REG_FIELD##_SHIFT))

/**
 * This macro calls read field API for 8 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 * retval Value of the bit-field
 */
#define CS_READ_FIELD8(pDev, regAddr, REG_FIELD)                                       \
    (CS_READ_FIELD8_RAW(pDev, regAddr, (uint8_t) REG_FIELD##_MASK,                     \
                         (uint32_t) REG_FIELD##_SHIFT))


/**
 * This function reads a 32-bit value from a hardware register
 *           and returns the value.
 *
 * addr    Address of the memory mapped hardware register.
 *
 * Unsigned 32-bit value read from a register.
 */
static inline uint32_t CS_READ_REG32(VXB_DEV_ID pDev, uint32_t addr);

/**
 * This function writes a 32-bit value to a hardware register.
 *
 * addr    Address of the memory mapped hardware register.
 * value   unsigned 32-bit value which has to be written to the
 *                   register.
 */
static inline void CS_WRITE_REG32(VXB_DEV_ID pDev, uint32_t addr, uint32_t value);

/**
 * This function reads a 16-bit value from a hardware register
 *           and returns the value.
 *
 * addr    Address of the memory mapped hardware register.
 *
 * Unsigned 16-bit value read from a register.
 */
static inline uint16_t CS_READ_REG16(VXB_DEV_ID pDev, uint32_t addr);

/**
 * This function writes a 16-bit value to a hardware register.
 *
 * addr    Address of the memory mapped hardware register.
 * value   unsigned 16-bit value which has to be written to the
 *                   register.
 */
static inline void CS_WRITE_REG16(VXB_DEV_ID pDev, uint32_t addr, uint16_t value);

/**
 * This function reads a 8-bit value from a hardware register
 *           and returns the value.
 *
 * addr    Address of the memory mapped hardware register.
 *
 * Unsigned 8-bit value read from a register.
 */
static inline uint8_t CS_READ_REG8(VXB_DEV_ID pDev, uint32_t addr);

/**
 * This function writes a 8-bit value to a hardware
 *           register.
 *
 * addr    Address of the memory mapped hardware register.
 * value   unsigned 8-bit value which has to be written to the
 *                   register.
 */
static inline void CS_WRITE_REG8(VXB_DEV_ID pDev, uint32_t addr, uint8_t value);

/**
 * This function reads a 32 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD32_RAW(VXB_DEV_ID pDev,
									 uint32_t addr,
                                     uint32_t mask,
                                     uint32_t shift,
                                     uint32_t value);

/**
 * This function reads a 16 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 uint32_t addr,
                                     uint16_t mask,
                                     uint32_t shift,
                                     uint16_t value);

/**
 * This function reads a 8 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	uint32_t addr,
                                    uint8_t mask,
                                    uint32_t shift,
                                    uint8_t value);

/**
 * This function reads a 32 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 *
 * retval  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint32_t CS_READ_FIELD32_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint32_t mask,
                                         uint32_t shift);

/**
 * This function reads a 16 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 *
 * retval  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint16_t CS_READ_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint16_t mask,
                                         uint32_t shift);

/**
 * This function reads a 8 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 *
 * retval  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint8_t CS_READ_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	   uint32_t addr,
                                       uint8_t mask,
                                       uint32_t shift);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline uint32_t CS_READ_REG32(VXB_DEV_ID pDev, uint32_t addr)
{
	uint32_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead32 (MMAP_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));
	
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline void CS_WRITE_REG32(VXB_DEV_ID pDev, uint32_t addr, uint32_t value)
{
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite32 (MMAP_SPI_HANDLE(pDev),               \
						(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), value);
	
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline uint16_t CS_READ_REG16(VXB_DEV_ID pDev, uint32_t addr)
{
    uint16_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline void CS_WRITE_REG16(VXB_DEV_ID pDev, uint32_t addr, uint16_t value)
{
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), value);
	
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline uint8_t CS_READ_REG8(VXB_DEV_ID pDev, uint32_t addr)
{
    uint8_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline void CS_WRITE_REG8(VXB_DEV_ID pDev, uint32_t addr, uint8_t value)
{
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), value);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline void CS_WRITE_FIELD32_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 uint32_t addr,
                                     uint32_t mask,
                                     uint32_t shift,
                                     uint32_t value)
{
    uint32_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead32 (MMAP_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite32 (MMAP_SPI_HANDLE(pDev),               \
						(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), regVal);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline void CS_WRITE_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 uint32_t addr,
                                     uint16_t mask,
                                     uint32_t shift,
                                     uint16_t value)
{
    uint16_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), regVal);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline void CS_WRITE_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	uint32_t addr,
                                    uint8_t mask,
                                    uint32_t shift,
                                    uint8_t value)
{
    uint8_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), regVal);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline uint32_t CS_READ_FIELD32_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint32_t mask,
                                         uint32_t shift)
{
    uint32_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead32 (MMAP_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline uint16_t CS_READ_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint16_t mask,
                                         uint32_t shift)
{
    uint16_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline uint8_t CS_READ_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	   uint32_t addr,
                                       uint8_t mask,
                                       uint32_t shift)
{
    uint8_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}













LOCAL STATUS    am437xSpiProbe (VXB_DEV_ID);
LOCAL STATUS    am437xSpiAttach (VXB_DEV_ID);



/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

LOCAL VXB_DRV_METHOD am437xSpiMethods[] = {
    { VXB_DEVMETHOD_CALL (vxbDevProbe),  (FUNCPTR)am437xSpiProbe},
    { VXB_DEVMETHOD_CALL (vxbDevAttach), (FUNCPTR)am437xSpiAttach},
    /*{ VXB_DEVMETHOD_CALL (vxbFdtDevGet), (FUNCPTR)am437xSpiFdtDevGet},
    { VXB_DEVMETHOD_CALL (vxbResourceFree),  (FUNCPTR)am437xSpiResFree},
    { VXB_DEVMETHOD_CALL (vxbResourceAlloc), (FUNCPTR)am437xSpiResAlloc},*/
	VXB_DEVMETHOD_END
};

VXB_DRV vxbOfAm437xSpiDrv =
    {
    { NULL } ,
    TI_AM437X_SPI_NAME,                 /* Name */
    "TI AM437x SPI controller",         /* Description */
    VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
    (VXB_DRV_METHOD*)&am437xSpiMethods  /* Method table */
    };

VXB_DRV_DEF (vxbOfAm437xSpiDrv)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY am437xSpiMatch[] =
    {
        {
        "ti,am437x-spi", /* compatible */
        (void *)NULL
        },
        {} /* Empty terminated list */
    };

/**
 * This function finds and compare the compatible field from the dts file.
 * if found then success , else fail.
 */

LOCAL STATUS am437xSpiProbe
    (
    struct vxbDev * pDev
    )
    {
    return vxbFdtDevMatch (pDev, am437xSpiMatch, NULL);
    }
/**
 * This function allocates driver structure, registers and map it to
 * system.It always does the basic hardware and software initialization 
 * needed for driver and hardware functioning. 
 */
LOCAL STATUS am437xSpiAttach
    (
    VXB_DEV_ID  pDev
    )
    {
    TI_AM437X_SPI_CTRL *    pDrvCtrl;
    VXB_RESOURCE_ADR *      pResAdr = NULL;
    VXB_RESOURCE *          pRes    = NULL;
    VXB_RESOURCE_ADR *      pFlashResAdr = NULL;
    VXB_RESOURCE *          pFlashRes    = NULL;
    STATUS                  ret     = ERROR;
    VXB_FDT_DEV *       	pFdtDev;
    int                		offset;
    VXB_DEV_ID          	pCur;
  //  VXB_SPI_DEV_INFO *  	pSpiDevInfo;
    
    //VXB_QSPI_FLASH_FUNC * pQspiFunc = NULL;
    int result, len;
    
    if (pDev == NULL)
        {
        return ERROR;
        }
    
	pFdtDev = vxbFdtDevGet (pDev);
	if (pFdtDev == NULL)
		{
		return ERROR;
		}
	/* create controller driver context structure for core */
    result = vxFdtPropGet(pFdtDev->offset, "reg", &len);
    pDrvCtrl = (TI_AM437X_SPI_CTRL *) vxbMemAlloc (sizeof (TI_AM437X_SPI_CTRL));
    if (pDrvCtrl == NULL)
        {
        return ERROR;
        }
       
    if (vxbPinMuxEnable (pDev) != OK)
        {
        goto attach_error;
        }
    /* save pDrvCtrl in VXB_DEVICE structure */

    vxbDevSoftcSet (pDev, pDrvCtrl);
    /* save instance ID */

    pDrvCtrl->pDev = pDev;
    pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
    
    if (pRes == NULL)
        {
        goto attach_error;
        }
    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    
    if (pResAdr == NULL)
        {
        goto attach_error;
        }
    pDrvCtrl->regHandle = pResAdr->pHandle;
    pDrvCtrl->regBase = (void *)pResAdr->virtual;
//    kprintf("(%s) control register P=0x%08x,V=0x%08x\n",
//                pFdtDev->name,(UINT32)pResAdr->start,(UINT32)pResAdr->virtual);
//    kprintf(" handle bar P=0x%08x,V=0x%08x\n",
//    				MC_SPI_HANDLE(pDev),MC_SPI_BAR(pDev));
    pFlashRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 1);
    
    if (pFlashRes == NULL)
        {
        goto attach_error;
        }
    pFlashResAdr = (VXB_RESOURCE_ADR *)pFlashRes->pRes;

    if (pFlashResAdr == NULL)
        {
        goto attach_error;
        }
    pDrvCtrl->regFlashHandle = pFlashResAdr->pHandle;
    pDrvCtrl->regFlashBase = (void *)pFlashResAdr->virtual;
//    kprintf("(%s) control register P=0x%08x,V=0x%08x\n",
//                    pFdtDev->name,(UINT32)pFlashResAdr->start,(UINT32)pFlashResAdr->virtual);
//    kprintf(" handle bar P=0x%08x,V=0x%08x\n",
//    				MMAP_SPI_HANDLE(pDev),MMAP_SPI_BAR(pDev));
    
    pDrvCtrl->clkFrequency = AM437X_SPI_CLK_DEFAULT;
    QSPIRegister(pDrvCtrl);
    /* SPI controller init */
    
    QSPILibInit();
   
    return OK;

attach_error:

    if (pDrvCtrl->semSync != SEM_ID_NULL)
        {
        (void)semDelete (pDrvCtrl->semSync);
        }

    if (pDrvCtrl->intRes != NULL)
        {
        (void)vxbResourceFree (pDev, pDrvCtrl->intRes);
        }

    if (pRes != NULL)
        {
        (void)vxbResourceFree (pDev, pRes);
        }

    (void)vxbClkDisableAll (pDev);
    (void)vxbPinMuxDisable (pDev);
    vxbMemFree (pDrvCtrl);
    //vxbMemFree (pQspiFunc);
    return ERROR;
    }


/**
 * This function configures the idle mode of QSPI
 * By definition, target can handle read/write transaction as
 * long as it is out of IDLE state
 *
 *    baseAddr      Base Address of the QSPI Module Registers.
 *    idleMode      Idle mode to be configured.
 *                        It is of type enum #qspiSysConfigIdleMode_t.
 *
 */
void QSPISetIdleMode(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t idleMode)
{
	CS_WRITE_FIELD32(pDev, baseAddr + QSPI_SYSCONFIG, QSPI_SYSCONFIG_IDLE_MODE, idleMode);   
}
/**
 *   Set the QSPI clock register divider value.
 *
 *   This function sets the QSPI clock control register
 *          with serial data clock divider ratio (DCLK_DIV)
 *          according to the input clock provided and the output clock
 *          required.
 *          DCLK_DIV = ((input clock) / (output clock)) - 1.
 *          This function also enables the clock for QSPI module.
 *          This can only be done if QSPI module is not busy.
 *
 *  baseAddr      Base Address for QSPI module registers.
 *  clkDividerVal clock divider value to be set.
 */
void QSPISetPreScaler(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t clkDividerVal)
{
    /* Read the value of Clock control register */
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_CLOCK_CNTRL_REG);
    //kprintf("Read Prescalar Val:0x%08x\n",regVal);
    /* wait for QSPI to be idle */
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Set the QSPI clock divider bit field value*/
    CS_SET_FIELD(regVal, QSPI_CLOCK_CNTRL_REG_DCLK_DIV, clkDividerVal);

    /* Enable the QSPI data clock */
    CS_SET_FIELD(regVal, QSPI_CLOCK_CNTRL_REG_CLKEN,
                QSPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_ON);
    /* Set the value of QSPI clock control register */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_CLOCK_CNTRL_REG, regVal);
}

/**
 * This function configures the QSPI to work in different clock modes
 * for each output chip select.
 *
 * The combination of Clock phase
 *   baseAddr        Base Address of QSPI module registers.
 *   chipSelect      chipSelect for which the mode is to be configured.
 *                         Supported values according to enum
 *                         #qspiChipSelect_t.
 *   clkMode       Mode required to be selected.
 *                         Supported values given by enum #qspiClkMode_t
 */
void QSPISetClkMode(VXB_DEV_ID pDev,uint32_t baseAddr,
                    uint32_t chipSelect,
                    uint32_t clkMode)
{
    /* read value of the device control register */
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_DC_REG);

    /* Set the values of clock phase and clock polarity in the device
     * Control register for chip Select passed as the parameter to function.
     */
    /* The clock mode for different chip select are at a separation of
     * 8 bits in the device control register.
     * So 8U has been multiplied to the chip select value.
     */
    regVal &= ~(QSPI_CLK_MODE_3 << (8U * chipSelect));
    regVal |= (clkMode << (8U * chipSelect));
    /* wait for QSPI to be idle */
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Set clock phase and clock polarity for required chip select
     * based on the mode selected
     */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_DC_REG, regVal);
}

/**
 * This function configures the chip select polarity for
 * a selected chip select.This can only be done if QSPI module is not busy.
 *
 * baseAddr            Base Address for QSPI module registers.
 * chipSelect          chip select for which polarity is to be
 *                              configured. Supported values are
 *                              according to enum
 *                              #qspiChipSelect_t.
 * csPol               Polarity value of the chip select
 *                              selected.
 *                              Supported values are given by the enum
 *                              #qspiCsPol_t
 */
void QSPISetCsPol(VXB_DEV_ID pDev, uint32_t baseAddr,
          uint32_t chipSelect,
                  uint32_t csPol)
{
    /* Read value of device control register */
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_DC_REG);
    /* wait for QSPI to be idle */
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /* Set the chip select polarity for the required chip Select */
    /* The polarity for different chip select are at a separation of
     * 8 bits in the device control register.
     * So 8U has been multiplied to the chip select value.
     */
    regVal &= ~(QSPI_CS_POL_ACTIVE_HIGH << (QSPI_DC_REG_CSP0_SHIFT +
                                     (8U * chipSelect)));
    regVal |= (csPol << (QSPI_DC_REG_CSP0_SHIFT +
                                     (8U * chipSelect)));
    /* Set the value of the device control register to configure chip
     * polarity
     */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_DC_REG, regVal);
}

/**
 * This function configures the Data delay for each chip select.
 *          This can only be done if QSPI module is not busy.
 *
 * baseAddr     Base Address for QSPI module registers.
 * chipSelect   chip select for which polarity is to be configured.
 *                       Supported values are according to enum
 *                       #qspiChipSelect_t.
 * dataDelay    Value of Data Delay.
 *                       Supported values are
 *                       given by enum #qspiDataDelay_t
 */
void QSPISetDataDelay(VXB_DEV_ID pDev, uint32_t baseAddr,
                      uint32_t chipSelect,
                      uint32_t dataDelay)
{
    /* Read the value of device control register */
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_DC_REG);
    /* wait for QSPI to be idle */
    while (TRUE == QSPIIsBusy(pDev, baseAddr));

    /** Set the Data Delay value for the required chip Select */
    /* The Data delay for different chip select are at a separation of
     * 8 bits in the device control register.
     * So 8U has been multiplied to the chip select value.
     */
    regVal &= ~(QSPI_DATA_DELAY_3 << (QSPI_DC_REG_DD0_SHIFT +
                            (8U * chipSelect)));
    regVal |= (dataDelay << (QSPI_DC_REG_DD0_SHIFT +
                            (8U * chipSelect)));

    /* Set the value of the device control register to configure data
       delay */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_DC_REG, regVal);
}
/**
 * Set number of address bytes for memory mapped mode.
 *
 * This function configures the number af Address Bytes to be
 *          sent for memory mapped protocol translator for selected chip
 *          select.By default, the device uses 3 address bytes.
 *          This can only be done if QSPI module is not busy.
 *
 * baseAddr           Base Address for QSPI module registers.
 * chipSelect         chip select for which polarity is to be
 *                             configured.
 *                             Supported values are according to enum
 *                             #qspiChipSelect_t.
 * numAddrBytes    Number of Address bytes to be sent for
 *                          memory mapped protocol translator.
 *                          Supported values are :
 *                          for each chip select given by enum
 *                          #qspiMemMapNumAddrBytes_t.
 */
void QSPISetMemMapNumAddrBytes(VXB_DEV_ID pDev, uint32_t baseAddr,
                               uint32_t chipSelect,
                               uint32_t numAddrBytes)
{
    uint32_t addr = baseAddr + QSPI_SETUP_REG(chipSelect);
    CS_WRITE_FIELD32(pDev, addr, QSPI_SETUP_REG_NUM_A_BYTES, numAddrBytes);
}
/**
 * Set Number of dummy bits for memory mapped mode.
 *
 * This function configures the number of Dummy Bits to be
 *          sent for memory mapped protocol translator for selected chip
 *          select for fast read mode.
 *          This can only be done if QSPI module is not busy.
 *
 * baseAddr        Base Address for QSPI module registers.
 * chipSelect      chip select for which polarity is to be configured.
 *                          Supported values are according to enum
 *                          #qspiChipSelect_t.
 * numDummyBits    Number of Dummy Bits to be sent for
 *                          memory mapped protocol translator.
 *                          User can pass any value from 0 to 32 bits.
 */
void QSPISetMemMapNumDummyBits(VXB_DEV_ID pDev, uint32_t baseAddr,
                               uint32_t chipSelect,
                               uint32_t numDummyBits)
{
    uint32_t addr = baseAddr + QSPI_SETUP_REG(chipSelect);
    /* Read the value of setup register for given chip select */
    uint32_t regVal = CS_READ_REG32(pDev, addr);
    uint32_t numDummyBytes;
    /* set number of dummy bits or bytes in setup register for transfer of
     * data in memory mapped mode
     */
    numDummyBytes = numDummyBits / 8U;
    numDummyBits = numDummyBits % 8U;
    CS_SET_FIELD(regVal, QSPI_SETUP_REG_NUM_D_BITS, numDummyBits);
    CS_SET_FIELD(regVal,
                QSPI_SETUP_REG_NUM_D_BYTES,
                numDummyBytes);
    CS_WRITE_REG32(pDev, addr, regVal);
}
/**
 * Set read type in memory mapped mode.
 *
 * This function configures the read type for QSPI in memory mapped
 *          mode for selected chip select.Determines if the read command
 *          is a single, dual or quad read mode.
 *          This can only be done if QSPI module is not busy.
 *
 * baseAddr       Base Address for QSPI module registers.
 * chipSelect     chip select for which polarity is to be configured.
 *                         Supported values are according to enum
 *                         #qspiChipSelect_t.
 * readType       Read type to be chosen.
 *                         Supported values are given by enum
 *                         #qspiMemMapReadType_t.
 */
void QSPISetMemMapReadType(VXB_DEV_ID pDev, uint32_t baseAddr,
                           uint32_t chipSelect,
                           uint32_t readType)
{
    uint32_t addr = baseAddr + QSPI_SETUP_REG(chipSelect);
    CS_WRITE_FIELD32(pDev, addr, QSPI_SETUP_REG_READ_TYPE, readType);
}
/**
 * Set write command in memory mapped mode.
 *
 * This function configures the write command for memory
 *           mapped protocol translator in memory mapped SPI setup register.
 *           The command send is according to the slave device connected.
 *           By default, the device uses a write command of 0x2.
 *           This can only be done if QSPI module is not busy.
 *
 * baseAddr      Base Address for QSPI module registers.
 * chipSelect    chip select for which polarity is to be configured.
 *                         Supported values are according to enum
 *                         #qspiChipSelect_t.
 * writeCmd      Write Command to be sent to the flash device.
 *                         This command is specific to the flash device being
 *                         used.
 */
void QSPISetMemMapWriteCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
                           uint32_t chipSelect,
                           uint32_t writeCmd)
{
    uint32_t addr = baseAddr + QSPI_SETUP_REG(chipSelect);
    CS_WRITE_FIELD32(pDev, addr, QSPI_SETUP_REG_WCMD, writeCmd);
}

/**
 * Set Read Command in memory mapped mode.
 *
 * This function configures the read command for memory
 *           mapped protocol translator in memory mapped SPI setup register.
 *           The command send is according to the slave device connected.
 *           By default, the device uses a read command of 0x3.
 *           This can only be done if QSPI module is not busy.
 *
 * baseAddr      Base Address for QSPI module registers.
 * chipSelect    chip select for which polarity is to be configured.
 *                         Supported values are according to enum
 *                         #qspiChipSelect_t.
 * readCmd       Read Command to be sent to the flash device.
 *                         This command is specific to the flash device being
 *                         used.
 */
void QSPISetMemMapReadCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
                          uint32_t chipSelect,
                          uint32_t readCmd)
{
    uint32_t addr = baseAddr + QSPI_SETUP_REG(chipSelect);
    CS_WRITE_FIELD32(pDev, addr, QSPI_SETUP_REG_RCMD, readCmd);
}
/**
 * This function writes to data register
 *            (data, data1, data2, data3).
 *
 * baseAddr   Base Address for QSPI module registers.
 * pData      pointer to source address.
 * length     number of words ro be written - accepted values from 1
 *                       to 4.
 *                       The length of data read is in multiples of 4 bytes.
 *                       1 means 4 bytes would be written.
 *                       4 means 16 bytes would be written.
 */
void QSPIWriteData(VXB_DEV_ID pDev, uint32_t baseAddr, const uint32_t *pData, uint32_t length)
{
    /* Data is written to the DATA register 0 */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_DATA_REG, *pData);
    pData++;
    length--;
    if (length > 0U)
    {
        /* Data is written to data register 1 */
        CS_WRITE_REG32(pDev, baseAddr + QSPI_DATA_REG_1, *pData);
        length--;
        pData++;
    }
    if (length > 0U)
    {
        /* Data is written to data register 2 */
        CS_WRITE_REG32(pDev, baseAddr + QSPI_DATA_REG_2, *pData);
        length--;
        pData++;
    }
    if (length > 0U)
    {
        /* Data is written to data register 3 */
        CS_WRITE_REG32(pDev, baseAddr + QSPI_DATA_REG_3, *pData);
    }
}
/**
 * This function reads value of data register
 *            (data, data1, data2, data3)
 *
 * baseAddr   Base Address for QSPI module registers.
 * pData      pointer to destination address.
 * length     number of words to be read - accepted values from 1
 *                       to 4.
 *                       The length of data read is in multiples of 4 bytes.
 *                       1 means 4 bytes would be read.
 *                       4 means 16 bytes would be read.
 */
void QSPIReadData(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t *pData, uint32_t length)
{
    /* Data is read from the QSPI data register 0*/
    *pData = CS_READ_REG32(pDev, baseAddr + QSPI_DATA_REG);
    pData++;
    length--;
    if (length > 0U)
    {
        /* Data is read from QSPI data register 1*/
        *pData = CS_READ_REG32(pDev, baseAddr + QSPI_DATA_REG_1);
        pData++;
        length--;
    }
    if (length > 0U)
    {
        /* Data is read from QSPI data register 2 */
        *pData = CS_READ_REG32(pDev, baseAddr + QSPI_DATA_REG_2);
        pData++;
        length--;
    }
    if (length > 0U)
    {
        /* Data is read from QSPI data register 3 */
        *pData = CS_READ_REG32(pDev, baseAddr + QSPI_DATA_REG_3);
    }
}
/**
 * This function is blocking and waits till busy
 *            bit in status register is reset.
 *
 * baseAddr   Base Address for QSPI module registers.
 *
 * status     value of busy bit of QSPI status register
 *                       TRUE : QSPI is busy
 *                       FALSE : QSPI is not busy
 */
uint32_t QSPIIsBusy(VXB_DEV_ID pDev, uint32_t baseAddr)
{
    uint32_t retval = FALSE;
    /* Read busy bit of QSPI status register */
    uint32_t busyStatus = CS_READ_FIELD32(pDev, (baseAddr + QSPI_STS_REG), QSPI_STS_REG_BUSY);
    if (1U == busyStatus)
    {
        retval = TRUE;
    }
    else
    {
        retval = FALSE;
    }
    return retval;
}

/**
 * This function returns Word count value of Status register.
 *
 * baseAddr   Base Address for QSPI module registers.
 *
 * wordCount  Word count field in Status register
 */
uint32_t QSPIGetStatusWordCount(VXB_DEV_ID pDev, uint32_t baseAddr)
{
    /* Read QSPI status register */
     return(CS_READ_FIELD32(pDev, baseAddr + QSPI_STS_REG, QSPI_STS_REG_WDCNT));
}
/**
 * This Function enables only specified QSPI interrupts
 *
 * baseAddr   Base Address for QSPI module registers.
 * intrMask    mask value for the interrupts to be enabled.
 *                       Supported values are given by the enum
 *                       #qspiIntrMask_t.
 */
void QSPIIntrEnable(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t intrFlag)
{
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_INTR_EN_SET_REG);
    regVal |= intrFlag;
    /** Set specified interrupts */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_INTR_EN_SET_REG, regVal);
}
/**
 * This Function disables only specified QSPI interrupts
 *
 * baseAddr   Base Address for QSPI module registers.
 * intrMask    mask value for the interrupts to be disabled.
 *                       Supported values are given by enum
 *                       #qspiIntrMask_t.
 */

void QSPIIntrDisable(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t intrFlag)
{
    uint32_t regVal =CS_READ_REG32(pDev, baseAddr + QSPI_INTR_EN_CLR_REG);
    regVal |= intrFlag;
    CS_WRITE_REG32(pDev, baseAddr + QSPI_INTR_EN_CLR_REG, regVal);
}
/**
 * This function returns the status of interrupts.
 *            It specifies whether an interrupt is active or inactive.
 *            After an interrupt is serviced, the software must set to 0
 *            the corresponding flag in the interrupt status register.
 *
 * baseAddr   Base Address for QSPI module registers.
 *
 * status     All the interrupt status.
 *                       The return status can take value from the enum
 *                       #qspiIntrMask_t.
 */
uint32_t QSPIIntrStatus(VXB_DEV_ID pDev, uint32_t baseAddr)
{
    return (CS_READ_REG32(pDev, baseAddr + QSPI_INTR_STS_EN_CLR));
}
/**
 * This Function clears the status of specified interrupts.
 *
 * baseAddr   Base Address for QSPI module registers.
 * intrMask    mask value for the interrupts to be cleared.
 *                       - given by enum - #qspiIntrMask_t.
 */
void QSPIIntrClear(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t intrFlag)
{
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_INTR_STS_EN_CLR);
    regVal |= intrFlag;
    CS_WRITE_REG32(pDev, baseAddr + QSPI_INTR_STS_EN_CLR, regVal);
}
/**
 * This function returns the raw status of interrupts.
 *
 * baseAddr   Base Address for QSPI module registers.
 *
 * status     All the raw interrupt status.
 *                       The return status can take the value from the
 *                       enum #qspiIntrMask_t.
 */
uint32_t QSPIIntrRawStatus(VXB_DEV_ID pDev, uint32_t baseAddr)
{
    return (CS_READ_REG32(pDev, baseAddr + QSPI_INTR_STS_RAW_SET));
}
/**
 * Set Memory address space
 *
 * This function selects the memory mapped mode in
 *           SPI switch register and configures mapped address
 *           for different chip select in control module.
 *           It provides a functionality for
 *           remapping the memory mapped address space to one of
 *           the four supported chip selects or to the
 *           configuration registers.
 *
 * baseAddr             Base Address for QSPI module registers
 * memMapPortSwitch  Switch for selection of memory mapped port
 *                             and configuration port.
 *                             Supported values are given by enum
 *                             #qspiMemMapPortSel_t.
 * chipSelect    chip select for which polarity is to be configured.
 *                         Supported values given by #qspiChipSelect_t.
 */
void QSPISetMemAddrSpace(VXB_DEV_ID pDev, uint32_t baseAddr,
                         uint32_t chipSelect,
                         uint32_t memMappedPortSwitch)
{
    CS_WRITE_FIELD32(pDev, baseAddr + QSPI_SWITCH_REG, QSPI_SWITCH_REG_MMPT_S, memMappedPortSwitch);
}
/**
 * Set Transfer Command
 *
 * This function sets the transfer command in the command
 *           for transfer of data in configuration mode.
 *
 * baseAddr         Base Address of QSPI module registers
 * qspiTxCmd        The transfer commnand to be set for initiating the
 *                            qspi transfer in config mode.
 *                            Value given by structure #qspiTxCmd_t
 */
void QSPISetCfgModeTxCmd(VXB_DEV_ID pDev, uint32_t baseAddr, qspiTxCmd_t qspiTxCmd)
{
    /* wait for QSPI to be idle */
    while (TRUE == QSPIIsBusy(pDev, baseAddr));
    /* Read value of the qspi command register */
    uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_CMD_REG);

    /* Set value of all the fields for qspi command */
    CS_SET_FIELD(regVal, QSPI_CMD_REG, qspiTxCmd.txCmd);
    CS_SET_FIELD(regVal, QSPI_CMD_REG_FIRQ, qspiTxCmd.frameCntIntrVal);
    CS_SET_FIELD(regVal, QSPI_CMD_REG_WIRQ, qspiTxCmd.wordCntIntrVal);
    CS_SET_FIELD(regVal, QSPI_CMD_REG_WLEN, qspiTxCmd.wordLength - 1);
    CS_SET_FIELD(regVal, QSPI_CMD_REG_FLEN, qspiTxCmd.frameLength - 1);
    CS_SET_FIELD(regVal, QSPI_CMD_REG_CSNUM, qspiTxCmd.chipSelect);

    /* Set the value of QSPI command in the command register */
    CS_WRITE_REG32(pDev, baseAddr + QSPI_CMD_REG, regVal);
}






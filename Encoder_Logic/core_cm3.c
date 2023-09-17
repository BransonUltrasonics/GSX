// $Header:   D:/databases/VMdb/archives/EN13849/core_cm3.c_v   1.2   20 Jul 2011 16:55:54   ASharma6  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

 Cortex-M3 Core Peripheral Access Layer Source File.
 Module name: core_cm3.c
 File name: core_cm3.c
 -------------------------- TECHNICAL NOTES --------------------------------

 ---------------------------- REVISIONS ------------------------------------
 $Log:   D:/databases/VMdb/archives/EN13849/core_cm3.c_v  $
 * 
 *    Rev 1.2   20 Jul 2011 16:55:54   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.1   22 Jun 2011 18:17:32   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

 --------------------------------------------------------------------------*/

#include "stdint.h"


/* define compiler specific symbols */
#define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */
#define __INLINE         inline                                     /*!< inline keyword for GNU Compiler       */
/* -------------------  Compiler specific Intrinsics  --------------------- */

/* ---------------------------  GNU Compiler  ----------------------------- */
/* GNU gcc specific functions */

/***************************************************************************
 * @brief  Return the Process Stack Pointer
 *
 * @return ProcessStackPointer
 *
 * Return the actual process stack pointer
 ***************************************************************************/
uint32_t __get_PSP(void) __attribute__( ( naked ) );
uint32_t __get_PSP(void)
{
	uint32_t result = 0;

	__ASM volatile ("MRS %0, psp\n\t"
			"MOV r0, %0 \n\t"
			"BX  lr     \n\t" : "=r" (result) );
	return (result);
}

/***************************************************************************
 * @brief  Set the Process Stack Pointer
 *
 * @param  topOfProcStack  Process Stack Pointer
 *
 * Assign the value ProcessStackPointer to the MSP
 *
 * (process stack pointer) Cortex processor register
 ***************************************************************************/
void __set_PSP(uint32_t topOfProcStack) __attribute__( ( naked ) );
void __set_PSP(uint32_t topOfProcStack)
{
	__ASM volatile ("MSR psp, %0\n\t"
			"BX  lr     \n\t" : : "r" (topOfProcStack) );
}

/***************************************************************************
 * @brief  Return the Main Stack Pointer
 *
 * @return Main Stack Pointer
 *
 *  Return the current value of the MSP (main stack pointer)
 *  Cortex processor register
 ***************************************************************************/

uint32_t __get_MSP(void) __attribute__( ( naked ) );
uint32_t __get_MSP(void)
{
	uint32_t result = 0;

	__ASM volatile ("MRS %0, msp\n\t"
			"MOV r0, %0 \n\t"
			"BX  lr     \n\t" : "=r" (result) );
	return (result);
}

/***************************************************************************
 * @brief  Set the Main Stack Pointer
 *
 * @param  topOfMainStack  Main Stack Pointer
 *
 * Assign the value mainStackPointer to the MSP
 * (main stack pointer) Cortex processor register
 ***************************************************************************/
void __set_MSP(uint32_t topOfMainStack) __attribute__( ( naked ) );
void __set_MSP(uint32_t topOfMainStack)
{
	__ASM volatile ("MSR msp, %0\n\t"
			"BX  lr     \n\t" : : "r" (topOfMainStack) );
}

/***************************************************************************
 * @brief  Return the Base Priority value
 *
 * @return BasePriority
 *
 * Return the content of the base priority register
 ***************************************************************************/
uint32_t __get_BASEPRI(void)
{
	uint32_t result = 0;

	__ASM volatile ("MRS %0, basepri_max" : "=r" (result) );
	return (result);
}

/***************************************************************************
 * @brief  Set the Base Priority value
 *
 * @param  basePri  BasePriority
 *
 * Set the base priority register
 */
void __set_BASEPRI(uint32_t value)
{
	__ASM volatile ("MSR basepri, %0" : : "r" (value) );
}

/***************************************************************************
 * @brief  Return the Priority Mask value
 *
 * @return PriMask
 *
 * Return state of the priority mask bit from the priority mask register
 ***************************************************************************/
uint32_t __get_PRIMASK(void)
{
	uint32_t result = 0;

	__ASM volatile ("MRS %0, primask" : "=r" (result) );
	return (result);
}

/***************************************************************************
 * @brief  Set the Priority Mask value
 *
 * @param  priMask  PriMask
 *
 * Set the priority mask bit in the priority mask register
 ***************************************************************************/
void __set_PRIMASK(uint32_t priMask)
{
	__ASM volatile ("MSR primask, %0" : : "r" (priMask) );
}

/***************************************************************************
 * @brief  Return the Fault Mask value
 *
 * @return FaultMask
 *
 * Return the content of the fault mask register
 ***************************************************************************/
uint32_t __get_FAULTMASK(void)
{
	uint32_t result = 0;

	__ASM volatile ("MRS %0, faultmask" : "=r" (result) );
	return (result);
}

/***************************************************************************
 * @brief  Set the Fault Mask value
 *
 * @param  faultMask  faultMask value
 *
 * Set the fault mask register
 ***************************************************************************/
void __set_FAULTMASK(uint32_t faultMask)
{
	__ASM volatile ("MSR faultmask, %0" : : "r" (faultMask) );
}

/***************************************************************************
 * @brief  Return the Control Register value
 *
 *  @return Control value
 *
 * Return the content of the control register
 ***************************************************************************/
uint32_t __get_CONTROL(void)
{
	uint32_t result = 0;

	__ASM volatile ("MRS %0, control" : "=r" (result) );
	return (result);
}

/***************************************************************************
 * @brief  Set the Control Register value
 *
 * @param  control  Control value
 *
 * Set the control register
 ***************************************************************************/
__INLINE void __set_CONTROL(uint32_t control)
{
	__ASM volatile ("MSR control, %0" : : "r" (control) );
}

/***************************************************************************
 * @brief  Reverse byte order in integer value
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse byte order in integer value
 ***************************************************************************/
uint32_t __REV(uint32_t value)
{
	uint32_t result = 0;

	__ASM volatile ("rev %0, %1" : "=r" (result) : "r" (value) );
	return (result);
}

/***************************************************************************
 * @brief  Reverse byte order in unsigned short value
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse byte order in unsigned short value
 ***************************************************************************/
uint32_t __REV16(uint16_t value)
{
	uint32_t result = 0;

	__ASM volatile ("rev16 %0, %1" : "=r" (result) : "r" (value) );
	return (result);
}

/***************************************************************************
 * @brief  Reverse byte order in signed short value with sign extension to integer
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse byte order in signed short value with sign extension to integer
 ***************************************************************************/
int32_t __REVSH(int16_t value)
{
	uint32_t result = 0;

	__ASM volatile ("revsh %0, %1" : "=r" (result) : "r" (value) );
	return (result);
}

/***************************************************************************
 * @brief  Reverse bit order of value
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse bit order of value
 ***************************************************************************/
uint32_t __RBIT(uint32_t value)
{
	uint32_t result = 0;

	__ASM volatile ("rbit %0, %1" : "=r" (result) : "r" (value) );
	return (result);
}

/***************************************************************************
 * @brief  LDR Exclusive (8 bit)
 *
 * @param  *addr  address pointer
 * @return        value of (*address)
 *
 * Exclusive LDR command for 8 bit value
 ***************************************************************************/
uint8_t __LDREXB(uint8_t *addr)
{
	uint8_t result = 0;

	__ASM volatile ("ldrexb %0, [%1]" : "=r" (result) : "r" (addr) );
	return (result);
}

/***************************************************************************
 * @brief  LDR Exclusive (16 bit)
 *
 * @param  *addr  address pointer
 * @return        value of (*address)
 *
 * Exclusive LDR command for 16 bit values
 ***************************************************************************/
uint16_t __LDREXH(uint16_t *addr)
{
	uint16_t result = 0;

	__ASM volatile ("ldrexh %0, [%1]" : "=r" (result) : "r" (addr) );
	return (result);
}

/***************************************************************************
 * @brief  LDR Exclusive (32 bit)
 *
 * @param  *addr  address pointer
 * @return        value of (*address)
 *
 * Exclusive LDR command for 32 bit values
 ***************************************************************************/
uint32_t __LDREXW(uint32_t *addr)
{
	uint32_t result = 0;

	__ASM volatile ("ldrex %0, [%1]" : "=r" (result) : "r" (addr) );
	return (result);
}

/***************************************************************************
 * @brief  STR Exclusive (8 bit)
 *
 * @param  value  value to store
 * @param  *addr  address pointer
 * @return        successful / failed
 *
 * Exclusive STR command for 8 bit values
 ***************************************************************************/
uint32_t __STREXB(uint8_t value, uint8_t *addr)
{
	uint32_t result = 0;

	__ASM volatile ("strexb %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
	return (result);
}

/***************************************************************************
 * @brief  STR Exclusive (16 bit)
 *
 * @param  value  value to store
 * @param  *addr  address pointer
 * @return        successful / failed
 *
 * Exclusive STR command for 16 bit values
 ***************************************************************************/
uint32_t __STREXH(uint16_t value, uint16_t *addr)
{
	uint32_t result = 0;

	__ASM volatile ("strexh %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
	return (result);
}

/***************************************************************************
 * @brief  STR Exclusive (32 bit)
 *
 * @param  value  value to store
 * @param  *addr  address pointer
 * @return        successful / failed
 *
 * Exclusive STR command for 32 bit values
 ***************************************************************************/
uint32_t __STREXW(uint32_t value, uint32_t *addr)
{
	uint32_t result = 0;

	__ASM volatile ("strex %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
	return (result);
}

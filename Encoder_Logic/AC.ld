/* $Header: \Branson/AC/AC.ld,v 1.4 2011/06/28 17:46:32 satya Exp $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 1995,96, 2009        */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------
 LPC17xx .Contains all functions support for ADC firmware library on LPC17xx
 Module name: lpc17xx_adc.c
 File name: lpc17xx_adc.c
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log: AC.ld,v $
Revision 1.4  2011/06/28 17:46:32  satya
Updated for review comments

Revision 1.3  2011/03/25 15:01:50  Noor
Added File Header.

-------------------------------------------------------------------------*/

MEMORY
{
	FLASH		(rx)	: ORIGIN = 0x00000000, LENGTH = (512K)
	SRAM		(rw)	: ORIGIN = 0x10000000, LENGTH = 32K
	BOOT_ROM	(rx)	: ORIGIN = 0x1FFF0000, LENGTH = 8K
	AHB_RAM0	(rw)	: ORIGIN = 0x2007C000, LENGTH = 16K
	AHB_RAM1	(rw)	: ORIGIN = 0x20080000, LENGTH = 16K
}

_vRamTop = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)
	} > FLASH

	/* for exception handling/unwind - some Newlib functions (in common with C++ and STDC++) use this. */
	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
	. = ALIGN(4);
	__exidx_end = .;
	_etext = .;

	.data : AT (__exidx_end)
	{
		_data = .;
		*(vtable)
		*(.data*)
		. = ALIGN(4);
		_edata = .;
	} > SRAM

	/* zero initialized data */
	.bss :
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > SRAM

	_vStackTop = _vRamTop - 16;
}

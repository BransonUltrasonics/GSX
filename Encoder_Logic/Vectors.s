/* $Header:   D:/databases/VMdb/archives/EN13849/Vectors.s_v   1.10   20 Jul 2011 16:59:50   ASharma6  $
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

This file is for the Interrupt Handling routines.

Module name: Vectors

Filename:    Vectors.s

--------------------------- TECHNICAL NOTES -------------------------------

This code contains Interrupt Service routine vector table.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Vectors.s_v  $
; 
;    Rev 1.10   20 Jul 2011 16:59:50   ASharma6
; Modifications for review comments
; 
;    Rev 1.9   05 Jul 2011 11:01:54   ASharma6
; ULS, CM changes
; 
;    Rev 1.6   22 Jun 2011 18:17:46   ASharma6
; LE Fault reset allowed in horn up state
; Part contact lost made more resilient
; Cross monitoring detects other processor was reset and resets itself
; SBeam diagnostics is more resilient
; 
;    Rev 1.3   01 Apr 2011 11:15:36   PDwivedi
; Fixes after code review.
; 
;    Rev 1.2   25 Mar 2011 10:18:54   ygupta
; Removed unused SSP interrupt handler from Vector table.
; 
;    Rev 1.1   25 Mar 2011 09:12:52   ygupta
; Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

		.section ".isr_vector"

		.long	_vStackTop
		.long	ResetHandler+1
		.long	0 @		NMI_Handler, @ The NMI handler
		.long	0 @		HardFault_Handler, @ The hard fault handler
		.long	0 @	MemManage_Handler, @ The MPU fault handler
		.long	0 @	BusFault_Handler, @ The bus fault handler
		.long	0 @	UsageFault_Handler, @ The usage fault handler
		.long	0 @ Reserved
		.long	0 @ Reserved
		.long	0 @ Reserved
		.long	0 @ Reserved
		.long	0 @		SVC_Handler, @ SVCall handler
		.long	0 @	DebugMon_Handler, @ Debug monitor handler
		.long	0 @ Reserved
		.long	0 @	PendSV_Handler, @ The PendSV handler
		.long	SysTick_Handler @ The SysTick handler

		@ Chip Level - LPC17
		.long	0@	WDT_IRQHandler, @ 16, 0x40 - WDT
		.long	0@	TIMER0_IRQHandler, @ 17, 0x44 - TIMER0
		.long	0@	TIMER1_IRQHandler, @ 18, 0x48 - TIMER1
		.long	0@	TIMER2_IRQHandler, @ 19, 0x4c - TIMER2
		.long	0@	TIMER3_IRQHandler, @ 20, 0x50 - TIMER3
		.long	0@	UART0_IRQHandler, @ 21, 0x54 - UART0
		.long	0@	UART1_IRQHandler, @ 22, 0x58 - UART1
		.long	0@	UART2_IRQHandler, @ 23, 0x5c - UART2
		.long	0@	UART3_IRQHandler, @ 24, 0x60 - UART3
		.long	0@PWM1_IRQHandler @	PWM1_IRQHandler, @ 25, 0x64 - PWM1
		.long	0@	I2C0_IRQHandler, @ 26, 0x68 - I2C0
		.long	0@	I2C1_IRQHandler, @ 27, 0x6c - I2C1
		.long	0@	I2C2_IRQHandler, @ 28, 0x70 - I2C2
		.long	0@	SPI_IRQHandler, @ 29, 0x74 - SPI
		.long	0@  SSP0_IRQHandler @ 30, 0x78 - SSP0
		.long	0@   SSP1_IRQHandler @ 31, 0x7c - SSP1
		.long	0@	PLL0_IRQHandler, @ 32, 0x80 - PLL0 (Main PLL)
		.long	RTC_IRQHandler @ 33, 0x84 - RTC
		.long	0@	EINT0_IRQHandler, @ 34, 0x88 - EINT0
		.long	0@	EINT1_IRQHandler, @ 35, 0x8c - EINT1
		.long	0@	EINT2_IRQHandler, @ 36, 0x90 - EINT2
		.long		EINT3_IRQHandler@, @ 37, 0x94 - EINT3
		.long	ADC_IRQHandler 	@ 38, 0x98 - ADC
		.long	0@	BOD_IRQHandler, @ 39, 0x9c - BOD
		.long	0@	USB_IRQHandler, @ 40, 0xA0 - USB
		.long	0@	CAN_IRQHandler, @ 41, 0xa4 - CAN
		.long	0@	DMA_IRQHandler, @ 42, 0xa8 - GP DMA
		.long	0@	I2S_IRQHandler, @ 43, 0xac - I2S
		.long	0@	ENET_IRQHandler, @ 44, Ethernet.
		.long	0@	RIT_IRQHandler, @ 45, 0xb4 - RITINT
		.long	0@	MCPWM_IRQHandler, @ 46, 0xb8 - Motor Control PWM
		.long	0@	QEI_IRQHandler, @ 47, 0xbc - Quadrature Encoder
		.long	0 @	PLL1_IRQHandler, @ 48, 0xc0 - PLL1 (USB PLL)

@		.long	Checksum

		.text
		.thumb
		.global MemoryInit
		.thumb_func
MemoryInit:
		ldr		r0,=__exidx_end		@ flash start
		ldr		r1,=_data			@ ram start
		ldr		r2,=_edata			@ ram end
DataLoop:
		ldr		r3,[r0]
		str		r3,[r1]
		add		r0,#4
		add		r1,#4
		cmp		r1,r2
		bne		DataLoop

		ldr		r1,=_bss			@ bss start
		ldr		r2,=_ebss			@ bss end
		ldr		r3,=0
BssLoop:
		str		r3,[r1]
		add		r1,#4
		cmp		r1,r2
		bne		BssLoop

		bx		lr

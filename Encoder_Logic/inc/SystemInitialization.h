// $Header:   D:/databases/VMdb/archives/EN13849/inc/SystemInitialization.h_v   1.6   20 Jul 2011 16:48:34   ASharma6  $
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
 Module name: SystemInitialization
 File name: SystemInitialization.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/SystemInitialization.h_v  $
 * 
 *    Rev 1.6   20 Jul 2011 16:48:34   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.5   05 Jul 2011 10:57:36   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.2   22 Jun 2011 18:16:38   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.0   31 Mar 2011 12:03:30   PDwivedi
 * Initial revision.
 *
-------------------------------------------------------------------------*/

#ifndef SYSINIT_H_
#define SYSINIT_H_

#include "Global.h"

/* PLL0STAT register
Table 22. PLL Status register (PLL0STAT - address 0x400F C088) bit description
*/
// Fields offset, other fields are unused/reserved
#define MSEL0Ofst     0
#define NSEL0Ofst     16
#define PLLE0STATOfst 24
#define PLLC0STATOfst 25
#define PLOCK0Ofst    26

#define MSEL0Mask        0x7FFF
#define NSEL0Mask        0xFF
#define PLLEnCnChkMask   0x03

#define PLOCK0Mask       (1<<26)
#define PLLEnbldNConnctd 0x03

/* Clock Source Select register (CLKSRCSEL - 0x400F C10C)
The CLKSRCSEL register contains the bits that select the clock source for PLL0.
*/

#define CLKSRCOfst 0
#define CLKSRCMask 0x03

#define CLKSRCRC       0 // Internal RC Oscillator
#define CLKSRCOSC      1 // Main Oscillator
#define CLKSRCRTCOSC   2 // RTC Oscillator
#define CLKSRRES       3 // Reserved

/* CCLKCFG Clock Configuration divisor register */
#define CCLKSELMask   0xFF // 8-bit divisor, divisor = Value+1

/* System Controls and Status register (SCS - 0x400F C1A0)
There are only 3 bits in this register, rest all are reserved */
#define OSCRANGEMask (1<<4)
#define OSCENMask    (1<<5)
#define OSCSTATMask  (1<<6)
#define OSCNotRdy    0
#define OSCRdy       (1<<6)

/* PLL0 Control register (PLL0CON - 0x400F C080)
Bits 2:31 are reserved */
#define PLLE0Enable     1 // PLL0Enable
#define PLLC0Connect    2 // PLL0Connect

/* PLL1 Control register (PLL1CON - 0x400F C0A0)
Bits 2:31 are reserved */
#define PLLE1Enable     1 // PLL0Enable
#define PLLC1Connect    2 // PLL0Connect

/* PLL0 Feed register (PLL0FEED - 0x400F C08C) */
#define PLL0FEEDVal1 0xAA
#define PLL0FEEDVal2 0x55

/* PLL1 Feed register (PLL1FEED - 0x400F C0AC) */
#define PLL1FEEDVal1 0xAA
#define PLL1FEEDVal2 0x55

/* PLL1 Status register (PLL1STAT - 0x400F C0A8) */
// Fields offset, other fields are unused/reserved
#define MSEL1Ofst     0
#define PSEL1Ofst     5
#define PLLE0STATOfst 24
#define PLLC0STATOfst 25
#define PLOCK0Ofst    26

#define MSEL1Mask      0x1F
#define PSEL1Mask      0x60
#define PLLE1STATMask  (1<<8)
#define PLLC1STATMask  (1<<9)
#define PLOCK1Mask     (1<<10)

/* Flash Accelerator Configuration register (FLASHCFG - 0x400F C000) */
#define FLASHCFG_Val          0x00004000
#define FLASHCFG_Mask         0x0000F000

/* Other defines */
#define CLOCK_SETUP           1
#define SCS_Val               0x00000020
#define CLKSRCSEL_Val         0x00000001
#define PLL0_SETUP            1
#define PLL0CFG_Val           0x00050063
#define PLL1_SETUP            0
#define PLL1CFG_Val           0x00000023
#define CCLKCFG_Val           0x00000003
#define USBCLKCFG_Val         0x00000000
#define PCLKSEL0_Val          0x00000000
#define PCLKSEL1_Val          0x00000000
#define PCONP_Val             0x042887DE
#define CLKOUTCFG_Val         0x00000000
#define FLASH_SETUP           1


/* Function prototypes */
void SystemInitialization(void);

#endif

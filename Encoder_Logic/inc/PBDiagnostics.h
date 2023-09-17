// $Header:   D:/databases/VMdb/archives/EN13849/inc/PBDiagnostics.h_v   1.6   20 Jul 2011 16:47:58   ASharma6  $
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
This module has  the list of all the Global variables used for PBDiagnostics.

Module name: PBDiagnostics

Filename:    PBDiagnostics.h

--------------------------- TECHNICAL NOTES -------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/PBDiagnostics.h_v  $
 * 
 *    Rev 1.6   20 Jul 2011 16:47:58   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.5   05 Jul 2011 10:57:06   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.2   22 Jun 2011 18:14:48   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

-------------------------------------------------------------------------*/

#ifndef PBDIAGNOSTICS_H_
#define PBDIAGNOSTICS_H_
#include "lpc_types.h"
#define PWM_CYCLE       5000//Pulses of 100usec
#define PWM_OFFSET      200

#define MR0_INT         1 << 0
#define MR1_INT         1 << 1
#define MR2_INT         1 << 2
#define MR3_INT         1 << 3
#define MR4_INT         1 << 8
#define MR5_INT         1 << 9
#define MR6_INT         1 << 10

#define TCR_CNT_EN      0x00000001
#define TCR_RESET       0x00000002
#define TCR_PWM_EN      0x00000008

#define PWMMR0I         1 << 0
#define PWMMR0R         1 << 1
#define PWMMR0S         1 << 2
#define PWMMR1I         1 << 3
#define PWMMR1R         1 << 4
#define PWMMR1S         1 << 5
#define PWMMR2I         1 << 6
#define PWMMR2R         1 << 7
#define PWMMR2S         1 << 8
#define PWMMR3I         1 << 9
#define PWMMR3R         1 << 10
#define PWMMR3S         1 << 11
#define PWMMR4I         1 << 12
#define PWMMR4R         1 << 13
#define PWMMR4S         1 << 14
#define PWMMR5I         1 << 15
#define PWMMR5R         1 << 16
#define PWMMR5S         1 << 17
#define PWMMR6I         1 << 18
#define PWMMR6R         1 << 19
#define PWMMR6S         1 << 20

#define PWMSEL2         1 << 2
#define PWMSEL3         1 << 3
#define PWMSEL4         1 << 4
#define PWMSEL5         1 << 5
#define PWMSEL6         1 << 6
#define PWMENA1         1 << 9
#define PWMENA2         1 << 10
#define PWMENA3         1 << 11
#define PWMENA4         1 << 12
#define PWMENA5         1 << 13
#define PWMENA6         1 << 14

#define LER0_EN         1 << 0
#define LER1_EN         1 << 1
#define LER2_EN         1 << 2
#define LER3_EN         1 << 3
#define LER4_EN         1 << 4
#define LER5_EN         1 << 5
#define LER6_EN         1 << 6

#define PB2_NO_MASK        (1UL << PINSEL_PIN_25)//P1
#define PB1_NO_MASK        (1UL << PINSEL_PIN_26)//P1
#define PB1_NC_MASK        (1UL << PINSEL_PIN_29)//P1
#define PB2_NC_MASK        (1UL << PINSEL_PIN_28)//P1
#define PBInt_Mask      (1UL << PINSEL_PIN_11)//P2
#define PB1Drv_Mask     (1UL << PINSEL_PIN_7)//P2 For 963 board
#define PB2Drv_Mask     (1UL << PINSEL_PIN_8)//P2 For 963 board
#define RisingEdgeCondition 0x00000001
#define PBDriveEqualCount 5
#define PBErrorCount 24

extern void PWM_Init( tUINT32 cycle );
extern void PWM_Set(  tUINT32 cycle, tUINT32 offset );
extern void PWM_Start(void);
extern void PWM_Stop(void);
#endif


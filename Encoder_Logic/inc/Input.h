// $Header:   D:/databases/VMdb/archives/EN13849/inc/Input.h_v   1.10   19 Sep 2011 10:35:38   ASharma6  $
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

This is the module to check if the other microprocessor is in sync with
this microprocessor.

Module name: Input

Filename:    Input.h

--------------------------- TECHNICAL NOTES -------------------------------

Function prototypes for Inputs module


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/Input.h_v  $
 * 
 *    Rev 1.10   19 Sep 2011 10:35:38   ASharma6
 * For IW+ only: After ULS, the Trigger switch is not looked at for declaring part contact until the horn goes 1/8 of an inch past the ULS.
 * 
 *    Rev 1.8   20 Jul 2011 16:47:16   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.7   07 Jul 2011 12:21:08   ASharma6
 * TRS Diagnostics for IW+ machine configuration is modified
 * 
 *    Rev 1.6   05 Jul 2011 10:56:40   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:14:18   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

-------------------------------------------------------------------------*/

#ifndef INPUT_H_
#define INPUT_H_

extern tUINT32 NoTRSDiagCntr;

/* Function prototypes */
void GetInputs(void);
void InitInputs(void);
void CheckMachineConfiguration(void);

#define U2RESET_MASK   (1UL << PINSEL_PIN_0)//P0
#define TRS_MASK        ( 1UL << PINSEL_PIN_1)//P0
#define DRIVEHIGHFAIL_MASK    (1UL << PINSEL_PIN_4)//P0
#define DRIVELOWFAIL_MASK    (1UL << PINSEL_PIN_5)//P0
#define SONICSHIGHFAIL_MASK  (1UL << PINSEL_PIN_21)//P0
#define SONICSLOWFAIL_MASK	(1UL << PINSEL_PIN_22)//P0
#define SV2HIGHFAIL_MASK    (1UL << PINSEL_PIN_16)//P1
#define SV2LOWFAIL_MASK    (1UL << PINSEL_PIN_17)//P1
#define SV1REQ_MASK     (1UL << PINSEL_PIN_10)//P0
#define SV2REQ_MASK     (1UL << PINSEL_PIN_9)//P1
#define ESTOPLOW_MASK  (1UL << PINSEL_PIN_28)//P0
#define ESTOPHI_MASK   (1UL << PINSEL_PIN_19)//P1
#define ESTOP1_MASK    (1UL << PINSEL_PIN_21)//P1
#define ESTOP2_MASK    (1UL << PINSEL_PIN_22)//P1  
#define ULS_MASK       (1UL << PINSEL_PIN_24)//P1
#define UFAIL24_MASK   (1UL << PINSEL_PIN_28)//P4
/*Enum structure for value of ULS depending on the Machine Configuration*/
enum ULSPOSITION
{
   AWAYPOSITION,
   HOMEPOSITION
};

#endif /* INPUT_H_ */

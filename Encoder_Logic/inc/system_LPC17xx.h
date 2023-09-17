// $Header:   D:/databases/VMdb/archives/EN13849/inc/system_LPC17xx.h_v   1.7   20 Jul 2011 16:48:30   ASharma6  $
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
CMSIS Cortex-M3 Device Peripheral Access Layer Header File
for the NXP LPC17xx Device Series

 Module name: system_LPC17xx
 File name: system_LPC17xx.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/system_LPC17xx.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:48:30   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:57:34   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:15:22   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   01 Apr 2011 11:30:48   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:20:38   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef __SYSTEM_LPC17xx_H
#define __SYSTEM_LPC17xx_H

#include <stdint.h>
#include "Global.h"
#ifdef __cplusplus
extern "C" {
#endif

extern tUINT32 SystemFrequency; /*!< System Clock Frequency (Core Clock)  */
extern tUINT32 SystemCoreClock; /*!< System Clock Frequency (Core Clock)*/

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
extern void SystemInit(void);
extern void InitSystemControlBlock(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_LPC17xx_H */

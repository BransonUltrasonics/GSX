// $Header:   D:/databases/VMdb/archives/EN13849/inc/SetOutputs.h_v   1.5   20 Jul 2011 16:48:18   ASharma6  $
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

Module name: SetOutputs

Filename:    SetOutputs.h

--------------------------- TECHNICAL NOTES -------------------------------

Function prototypes for SetOutputs module


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/SetOutputs.h_v  $
 * 
 *    Rev 1.5   20 Jul 2011 16:48:18   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.4   05 Jul 2011 10:57:22   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.1   22 Jun 2011 18:16:34   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient

-------------------------------------------------------------------------*/

#ifndef SETOUTPUTS_H_
#define SETOUTPUTS_H_


/* Function prototypes */
void PINSEL_SetOutput(tSINT32 port, tSINT32 pin, tSINT32 state);
void SetOutputs(void);
void SetPBs(void);

#endif /* SETOUTPUTS_H_ */

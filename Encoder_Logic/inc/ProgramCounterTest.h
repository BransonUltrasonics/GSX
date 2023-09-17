// $Header:   D:/databases/VMdb/archives/EN13849/inc/ProgramCounterTest.h_v   1.7   20 Jul 2011 16:48:06   ASharma6  $
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
This module has the code that verifies the proper functioning of the Program
counter.

 Module name:  ProgramCounterTest
 File name:  ProgramCounterTest.c
 -------------------------- TECHNICAL NOTES --------------------------------
Updates the status of the detection of the part contact.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/ProgramCounterTest.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:48:06   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:57:08   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:14:50   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   01 Apr 2011 11:29:58   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:18:52   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef PROGRAMCOUNTERTEST_H_
#define PROGRAMCOUNTERTEST_H_

enum
{
    Function1_Return = 1,
    Function2_Return,
    Function3_Return,
    Function4_Return,
    Function5_Return
} ;

enum PCStatus
{
    PCOK,
    PC_FAULT
};

#endif /* PROGRAMCOUNTERTEST_H_ */

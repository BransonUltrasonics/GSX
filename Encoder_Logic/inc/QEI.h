// $Header:   D:/databases/VMdb/archives/EN13849/inc/QEI.h_v   1.9   20 Jul 2011 16:48:08   ASharma6  $
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

 Module name: QEI
 File name: QEI.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/QEI.h_v  $
 * 
 *    Rev 1.9   20 Jul 2011 16:48:08   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.8   05 Jul 2011 10:57:12   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.5   22 Jun 2011 18:14:52   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 11:57:56   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:19:02   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef QEI_H_
#define QEI_H_

#include "stdint.h"
#include "Global.h"

#define MAXQEIPOS      0xffffffff
#define QEITESTVALUE   32767
#define QEIPhaseErrorCount   100
//#define QEITESTVALUE2   100

void QuadratureEncoderInit(tSINT32 maxPos);
void QEI_IRQHandler(void);
void QEI_GetPosition(void);
//void QEI_ResetPosition(void);
inline void QEI_ResetPosition(void);
#endif /* QEI_H_ */

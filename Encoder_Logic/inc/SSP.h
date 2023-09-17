// $Header:   D:/databases/VMdb/archives/EN13849/inc/SSP.h_v   1.10   20 Jul 2011 16:48:22   ASharma6  $
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
 Module name: SSP
 File name: SSP.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/SSP.h_v  $
 * 
 *    Rev 1.10   20 Jul 2011 16:48:22   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.9   05 Jul 2011 10:57:24   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.6   22 Jun 2011 18:15:12   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   01 Apr 2011 11:30:22   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.2   30 Mar 2011 18:35:52   ASharma6
 * Fixed review comments
 *
 *    Rev 1.1   25 Mar 2011 09:19:52   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef SSP_H_
#define SSP_H_

#define SSP0_PATTERN    0xAA55
#define MAXPORTPIN      16
#define  PINCONFIGFAC   2
#define CLK_RATE        1000000
#define MAXSSPPASSCOUNT 3
/* Data */
extern SSP_DATA_SETUP_Type ssp0;
extern SSP_DATA_SETUP_Type ssp1;
extern tUINT16 ssp0TxMsg, ssp0RxMsg, ssp1TxMsg, ssp1RxMsg;

/* Functions */
extern void InitSSPChl0Tx(void);
extern void InitSSPChl1Rx(void);
extern void InitSSP(void);
extern void SSPSyncUp(void);
extern tSINT32 SSPTxChl0(void);
extern void SSPRxChl1(void);
extern tSINT32 SSP0_Transmit(void);

#endif

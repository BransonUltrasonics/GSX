// $Header:   D:/databases/VMdb/archives/EN13849/inc/CrossMonitoring.h_v   1.14   09 Apr 2015 17:17:12   ewomack  $
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

Module name: CrossMonitoring

Filename:    CrossMonitoring.c

--------------------------- TECHNICAL NOTES -------------------------------

This code modifies the values of the Receive and Transmit status flags coming
from the second microprocessor


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/CrossMonitoring.h_v  $
 * 
 *    Rev 1.14   09 Apr 2015 17:17:12   ewomack
 * Fixed VerifyMask so it ignores checksum.
 * 
 *    Rev 1.13   20 Jul 2011 16:47:04   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.12   05 Jul 2011 10:56:26   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:14:04   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.4   01 Apr 2011 11:28:08   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.3   30 Mar 2011 11:22:02   ASharma6
 * Converted tabs to 3 spaces each
 *
 *    Rev 1.2   29 Mar 2011 19:19:04   ASharma6
 * Modifications done for compliance to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:15:56   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef CROSSMONITORING_H_
#define CROSSMONITORING_H_


#define SSPTxTOThres          1000
#define SSPRxTOThres          1000
#define SSPRxMSGFaultThres    1000

extern tUINT32 U2Fault;

/* -------------------------------------------------
Cross Monitoring message
** Description:
**                Bits     Meaning
**                0..2     State
**                3        SV
**                4        EStop
**                5        PartContact
**                6..7     2Hand Status
**                8..11    SW Revision number (LS nibble)
**                12..15   XOR of ls 3 nibbles
*/
/* Masks */
#define StatesMask   0x07
#define SVMask       0x01
#define ESTOPMask    0x01
#define PCMask       0x01
#define PBMask       0x03
#define SWRevMask    0x0F

/* Bit Positions */
#define StatesBitPosn   0
#define SVBitPosn       3
#define ESTOPBitPosn    4
#define PCBitPosn       5
#define PBBitPosn       6
#define SWRevPosn       8
#define XOEBitPosn      12

/* For XORing nibbles */
#define NibbleMask   0x0F
#define Nibble0Shift 0
#define Nibble1Shift 4
#define Nibble2Shift 8
#define Nibble3Shift 12

/* For verifying message, states are not considered */
#define VerifyMask (0x0FFF - (StatesMask << StatesBitPosn) - (PCMask << PCBitPosn))

#define ChkSumOK     0  // XORing the CheckSum with data should give 0

/* Clearing timeout values */
#define TimeOutClr   0

/* If the other processor doesn't transmit anything, message rec'd shall be 0 */
#define NoMsgRxd1  0x0000
#define NoMsgRxd2  0xFFFF

enum CROSSMONITORINGWDSTATUS
{
   OTHERPROCNOK,
   OTHERPROCOK
};

enum OTHERPROCUP
{
   OTHERPROCISDOWN,
   OTHERPROCISUP
};

enum SSPTXFAULTSTATUS
{
   SSPTxOK,
   SSPTxNOK,
   SSPTxFault
};

enum SSPRXFAULTSTATUS
{
   SSPRxOK,
   SSPRxNOK,
   SSPRxFault
};

enum RXMSGSANITY
{
   SSPRXMSGOK,
   SSPRXMSGNOK,
   SSPRXRESET
};

enum  // If msg is received OK
{
   SSPRXOK,
   SSPRXNOK
};

/* Functions */
void PrepareSSPMsg(void);

void CrossMonitoring(void);
void CrossMonitoringTx(void);
void CrossMonitoringRx(void);
void CrossMonitoringWD(void);


#endif /* CROSSMONITORING_H_ */

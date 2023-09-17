// $Header:   D:/databases/VMdb/archives/EN13849/CrossMonitoring.c_v   1.21   20 Jul 2011 16:56:08   ASharma6  $
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

This is the module for the CrossMonitoring the second processor state.

Module name: CrossMonitoring

Filename:    CrossMonitoring.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Cross monitors the states of second processor through SSP communication.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/CrossMonitoring.c_v  $
 * 
 *    Rev 1.21   20 Jul 2011 16:56:08   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.20   12 Jul 2011 18:33:58   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 *
 *    Rev 1.19   05 Jul 2011 10:59:12   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.14   22 Jun 2011 18:08:58   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.5   01 Apr 2011 11:12:38   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.4   30 Mar 2011 11:03:30   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.3.1.0   30 Mar 2011 11:03:08   ASharma6
 * Tabs converted to 3 spaces each
 *
 *    Rev 1.3   30 Mar 2011 08:12:58   ygupta
 * Corrected Typo.
 *
 *    Rev 1.2   29 Mar 2011 19:21:20   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:03:48   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "TestMacros.h"
#include "LPC17xx.h"
#include "SSP.h"
#include "CrossMonitoring.h"
#include "Global.h"
#include "StateMachine.h"
#include "DeviceDiagnostics.h"

UT_VARIABLE(tSINT32 OtherProcStatusIO)
UT_VARIABLE(tSINT32 crossMonitoring)
UT_VARIABLE(tSINT32 crossMonitoringRx)
UT_VARIABLE(tSINT32 crossMonitoringTx)
UT_VARIABLE(tSINT32 DiagVerifyRxdMsg)
UT_VARIABLE(tSINT32 DiagPrepareSSPMsg)

/*-------------------------- Local Variables ------------------------------*/

tSINT32 SSPTxTimeOut;
tSINT32 SSPTxFaultStatus;
tSINT32 SSPRxTimeOut;
tSINT32 SSPRxFaultStatus;
tSINT32 SSPRxMsgFaultCtr;
tSINT32 RxMsgSanity;
tSINT32 RxMsgRxd;
tSINT32 CrossMonitoringErrCnt;
tUINT32 CMFault = FALSE;
tUINT32 U2Fault = FALSE;

/*--------------------------------------------------------------------------*/

/**************************************************************************//**
* \brief -    PrepareSSPMsg
*
* \brief -
*               Bits     Meaning
*                0..2     State
*                3        SV
*                4        EStop
*                5        PartContact
*                6..7     2Hand Status
*                8..11    SW Revision number (LS nibble)
*                12..15   XOR of ls 3 nibbles
*
* \param -    none
* 
* \return -   none
*
******************************************************************************/
void PrepareSSPMsg(void)
{
   tSINT32 Message;
   tSINT32 CheckSum;
   FUNCTION1(DiagPrepareSSPMsg, "PrepareSSPMsg()");
   CHECKPOINT(DiagPrepareSSPMsg, BRANCH_1);
   Message = (State & StatesMask) << StatesBitPosn;   // Insert State
   Message |= ((SV1 & SVMask) << SVBitPosn);           // SV/STO
   Message |= ((EStop & ESTOPMask) << ESTOPBitPosn);   // EStop
   Message |= ((PartContactStatus & PCMask) << PCBitPosn); // PartContact
   Message |= ((BothHandStatus & PBMask) << PBBitPosn);  // Two-hand Status
   Message |= ((SWREVISION & SWRevMask) << SWRevPosn);     // SW Revision number, ls nibble only
   CheckSum =  (Message >> Nibble0Shift) & NibbleMask;
   CheckSum ^= (Message >> Nibble1Shift) & NibbleMask;
   CheckSum ^= (Message >> Nibble2Shift) & NibbleMask;
   Message |=  (CheckSum << Nibble3Shift);
   ssp0TxMsg = Message;
}

/**************************************************************************//**
* \brief -    VerifyRxdMsg
*
* \brief -     Verify the received message by comparing against the sent message.
*
* \param -    none
* 
* \return -   Result SSPRXMSGOK/SSPRXMSGNOK
*
******************************************************************************/
tSINT32 VerifyRxdMsg(void)
{
   tSINT32 Message;
   tSINT32 Result = 0;
   Message = ssp1RxMsg;

   FUNCTION1(DiagVerifyRxdMsg, "VerifyRxdMsg()");
   CHECKPOINT(DiagVerifyRxdMsg, BRANCH_1);
   if (RxMsgSanity == SSPRXMSGNOK)
   {
      CHECKPOINT(DiagVerifyRxdMsg, BRANCH_2);
      Result = SSPRXMSGNOK;
   }
   else
   {
      CHECKPOINT(DiagVerifyRxdMsg, BRANCH_3);
      if (ssp1RxMsg == SSP0_PATTERN)
      {
         /*
          * If the other processor is reset, then this processor shall
          * stop feeding the watchdog timer, so that it can be reset.
         */
         CHECKPOINT(DiagVerifyRxdMsg, BRANCH_4);
         Result = SSPRXRESET;
      }
      else
      {
         CHECKPOINT(DiagVerifyRxdMsg, BRANCH_5);
         if ((ssp0TxMsg & VerifyMask) == (ssp1RxMsg & VerifyMask))
         {
            CHECKPOINT(DiagVerifyRxdMsg, BRANCH_6);
            Result = SSPRXMSGOK;
         }
         else
         {
            CHECKPOINT(DiagVerifyRxdMsg, BRANCH_7);
            Result = SSPRXMSGNOK;
            if ((((ssp1RxMsg & StatesMask) == FAULT) || ((ssp1RxMsg & StatesMask) == FAULTRESET)))
            {
               CHECKPOINT(DiagVerifyRxdMsg, BRANCH_8);
               if (((ssp0TxMsg & StatesMask) != FAULT) && ((ssp0TxMsg & StatesMask) != FAULTRESET))
               {
                 CHECKPOINT(DiagVerifyRxdMsg, BRANCH_9);
                  NotMyFault = TRUE;
               }
               else
               {
                  CHECKPOINT(DiagVerifyRxdMsg, BRANCH_10);
                  NotMyFault = FALSE;
               }
            }
            else
            {
               CHECKPOINT(DiagVerifyRxdMsg, BRANCH_11);
               // I am in Fault State, but the other CPU is not
               NotMyFault = FALSE;
            }
         }
      }
   }
   return (Result);
}

/**************************************************************************//**
* \brief -    CrossMonitoringTx
*
* \brief -     Send the SSP message to second processor. Retry sending
*                   again if not successfully send and declare Fault in case
*                   of run out of retries.
*
* \param -		none
* 
* \return -   	none
*
******************************************************************************/
void CrossMonitoringTx(void)
{
   tSINT32 result;

   FUNCTION1(crossMonitoringTx, "CrossMonitoringTx()");
   CHECKPOINT(crossMonitoringTx, BRANCH_1);

   result = SSPTxChl0();
   CHECKPOINT(crossMonitoringTx, BRANCH_2);
   if (result == BytesToTx)
   {
      // Success
      CHECKPOINT(crossMonitoringTx, BRANCH_4);
      PrepareSSPMsg(); // For next time
      CHECKPOINT(crossMonitoringTx, BRANCH_5);
      SSPTxFaultStatus = SSPTxOK;
      SSPTxTimeOut = TimeOutClr;
      if (FaultReset == TRUE)
      {
         CHECKPOINT(crossMonitoringTx, BRANCH_6);
         SSPTxTimeOut = TimeOutClr;
         SSPTxFaultStatus = SSPTxOK;
         CMFault = FALSE;
      }
      else
         CHECKPOINT(crossMonitoringTx, BRANCH_7);
   }
   else
   {
      // Transmission was not OK
      CHECKPOINT(crossMonitoringTx, BRANCH_3);
      SSPTxTimeOut++;
      if (SSPTxTimeOut >= SSPTxTOThres)
      {
         // Time to raise fault
         CHECKPOINT(crossMonitoringTx, BRANCH_9);
         CMFault = TRUE;
         SSPTxFaultStatus = SSPTxFault;
      }
      else
      {
         CHECKPOINT(crossMonitoringTx, BRANCH_8);
         SSPTxFaultStatus = SSPTxNOK;
      }
   }
}

/**************************************************************************//**
* \brief -    CrossMonitoringRx
*
* \brief -     Receive the SSP message from second processor. Increase
*                   SSPRxMsgFaultCtr counter in case of Incorrect message and
*                   SSPRxTimeOut counter in case of message not received.
*                   declare Fault in case of run out of retries.
*
* \param -		none
* 
* \return -   	none
*
******************************************************************************/
void CrossMonitoringRx(void)
{
   tSINT32 result;

   FUNCTION1(crossMonitoringRx, "CrossMonitoringRx()");
   CHECKPOINT(crossMonitoringRx, BRANCH_1);

   SSPRxChl1();
   CHECKPOINT(crossMonitoringRx, BRANCH_2);

   if ((ssp1RxMsg == NoMsgRxd1) || (ssp1RxMsg == NoMsgRxd2))
   {
      CHECKPOINT(crossMonitoringRx, BRANCH_3);
      RxMsgRxd = SSPRXNOK;
   }
   else
   {
      CHECKPOINT(crossMonitoringRx, BRANCH_4);
      RxMsgRxd = SSPRXOK;
   }

   if (RxMsgRxd == SSPRXOK)
   {
      // Reception OK
      CHECKPOINT(crossMonitoringRx, BRANCH_5);
      SSPRxTimeOut = TimeOutClr;
      result = VerifyRxdMsg();
      CHECKPOINT(crossMonitoringRx, BRANCH_6);
      if (result == SSPRXMSGOK)
      {
         // Message received correctly
         U2Fault = FALSE;
         CHECKPOINT(crossMonitoringRx, BRANCH_7);
         SSPRxFaultStatus = SSPRxOK;
         SSPRxMsgFaultCtr = TimeOutClr;
         if (FaultReset == TRUE)
         {
            CHECKPOINT(crossMonitoringRx, BRANCH_8);
            SSPRxTimeOut = TimeOutClr;
            SSPRxMsgFaultCtr = TimeOutClr;
            SSPRxFaultStatus = SSPRxOK;
            CMFault = FALSE;
         }
         else
         {
            CHECKPOINT(crossMonitoringRx, BRANCH_9);
         }
      }
      else if (result == SSPRXMSGNOK)
      {
         // Message received but was not correct
         CHECKPOINT(crossMonitoringRx, BRANCH_10);
        // CMprinted = 0;
         SSPRxMsgFaultCtr++;
         if (SSPRxMsgFaultCtr >= SSPRxMSGFaultThres)
         {
            // Time to raise fault
            SSPRxFaultStatus = SSPRxFault;
            CMFault = TRUE;
            CHECKPOINT(crossMonitoringRx, BRANCH_11);
         }
         else
         {
            // Msg not OK, but don't raise fault yet
            SSPRxFaultStatus = SSPRxNOK;
            CHECKPOINT(crossMonitoringRx, BRANCH_12);
         }
      }
      else if (result == SSPRXRESET)
      {
         // Message received but was not correct
         CHECKPOINT(crossMonitoringRx, BRANCH_13);
         // CMprinted = 0;
         SSPRxMsgFaultCtr++;
         if (SSPRxMsgFaultCtr >= 10)
         {
            // Time to raise fault
            U2Fault = TRUE;
            CHECKPOINT(crossMonitoringRx, BRANCH_14);
         }
         else
         {
            // Msg not OK, but don't raise fault yet
            CHECKPOINT(crossMonitoringRx, BRANCH_15);
         }
      }
   }
   else
   {
      CHECKPOINT(crossMonitoringRx, BRANCH_16);
      // Message not received
      SSPRxTimeOut++;
      if (SSPRxTimeOut >= SSPRxTOThres)
      {
         // Time to raise fault
         CHECKPOINT(crossMonitoringRx, BRANCH_17);
         CMFault = TRUE;
         SSPRxFaultStatus = SSPRxFault;
      }
      else
      {
         // Not yet
         CHECKPOINT(crossMonitoringRx, BRANCH_18);
         SSPRxFaultStatus = SSPRxNOK;
      }
   }
}

/**************************************************************************//**
* \brief -    CrossMonitoring
*
* \brief -     Transmit and Receive the SSP message to and from second
*                   processor. Cross monitor the WD status also.
*                   declare Fault in case of any Fault.
*
* \param -    none
* 
* \return -   none
*
******************************************************************************/
void CrossMonitoring(void)
{
   FUNCTION1(crossMonitoring, "CrossMonitoring()");
   CHECKPOINT(crossMonitoring, BRANCH_1);
   CrossMonitoringTx();
   CHECKPOINT(crossMonitoring, BRANCH_2);

   CrossMonitoringRx();
   CHECKPOINT(crossMonitoring, BRANCH_3);
   if (CMFault == TRUE)
   {
      CHECKPOINT(crossMonitoring, BRANCH_4);
      FaultFlag |= CMFaultFlag;
      printf("CM Fault %4x %4x\n", ssp0TxMsg, ssp1RxMsg);
   }
   else
   {
      CHECKPOINT(crossMonitoring, BRANCH_5);
   }
}

#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
* \brief -    ResetHandler
*
* \brief -     ResetHandler is the entry for CrossMonitoring unit testing.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   tSINT32 PassCnt;
   tSINT32 Result;

   DiagInit();
   InitSSP();
   ssp0TxMsg = SSP0_PATTERN;
   BytesToTx = 2;
   SSPSyncUp();

   // Reset the board before starting the tests
   // VerifyRxdMsg white box testing
   DiagVerifyRxdMsg = 1;

   // To traverse path 1,2
   RxMsgSanity = SSPRXMSGNOK;
   Result = VerifyRxdMsg();
   DiagPrintInt(Result, SSPRXMSGNOK);

   // To traverse path 1,3,4
   RxMsgSanity = SSPRXMSGOK;
   ssp1RxMsg = SSP0_PATTERN;
   Result = VerifyRxdMsg();
   DiagPrintInt(Result, SSPRXRESET);

   // To traverse path 1,3,5,6
   ssp1RxMsg = 0xBABA;
   ssp0TxMsg = 0xBABA;
   Result = VerifyRxdMsg();
   DiagPrintInt(Result, SSPRXMSGOK);

   // To traverse path 1,3,5,7,8,9
   ssp1RxMsg = FAULT;
   ssp0TxMsg = 0xFFF1;
   Result = VerifyRxdMsg();
   DiagPrintInt(Result, SSPRXMSGNOK);

   // To traverse path 1,3,5,7,8,10
   ssp0TxMsg = 0xFFF3;
   Result = VerifyRxdMsg();
   DiagPrintInt(Result, SSPRXMSGNOK);

   // To traverse path 1,3,5,7,11
   ssp1RxMsg = READY;
   Result = VerifyRxdMsg();
   DiagPrintInt(Result, SSPRXMSGNOK);
   DiagVerifyRxdMsg = 0;

   /*Diagnostics of CrossMonitoringRx() function*/
   crossMonitoringTx = 1; // Turn ON diagnostic messages for Tx
   crossMonitoringRx = 1; // Turn ON diagnostic messages for Rx

   /* Transmit message 0x0000 */
   // For traverse paths Rx 1,2,3,16,18
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      ssp0TxMsg = NoMsgRxd1;
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(SSPTxFaultStatus, SSPTxOK);
   DiagPrintInt(SSPRxFaultStatus, SSPRXNOK);

   // For traverse paths Rx 1,2,3,16,18
   ssp0TxMsg = NoMsgRxd1;
   SSPRxTimeOut = SSPRxTOThres;
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(SSPRxFaultStatus, SSPRxFault);

   // Start transmitting non-zero message
   State = 1;
   SV1 = 1;
   EStop = 1;
   BothHandStatus = 1;
   PartContactStatus = 0;
   PrepareSSPMsg();

   // For traverse paths Rx 1,2,4,5,6,7,8
   FaultReset = TRUE;
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   // For traverse paths Rx 1,2,4,5,6,7,9
   FaultReset = FALSE;
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   // For traverse paths 1,2,4,10,12
   RxMsgSanity = SSPRXMSGNOK;
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(SSPRxFaultStatus, SSPRxNOK);

   // For traverse paths 1,2,4,10,11
   SSPRxMsgFaultCtr = SSPRxMSGFaultThres;
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(SSPRxFaultStatus, SSPRxFault);

   // Start transmitting non-zero message
   RxMsgSanity = SSPRXMSGOK;

   // For traverse paths Rx 1,2,4,5,6,7,8 - this is only to clear fault
   FaultReset = TRUE;
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   // For traverse paths Rx 1,2,4,5,6,13,15
   FaultReset = FALSE;
   U2Fault = FALSE;
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      ssp0TxMsg = SSP0_PATTERN;
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(U2Fault, FALSE);

   // For traverse paths Rx 1,2,4,5,6,13,14
   SSPRxMsgFaultCtr = 10;
   ssp0TxMsg = SSP0_PATTERN;
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(U2Fault, TRUE);

   // For traverse paths Rx 1,2,4,5,6,7,8 - this is only to clear fault
   FaultReset = TRUE;
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   /* -------------------------------------------------------------------------------- */
   // Now do the tests for Tx
   // Fake the result expected for Tx to start getting errors

   BytesToTx = 3;

   // Will test paths 1,3,5,7 for Tx
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(SSPTxFaultStatus, SSPTxNOK);
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   // FaultReset should clear all the faults. Path 1,3,4 for Tx
   FaultReset = TRUE;
   BytesToTx = 2;
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(SSPTxFaultStatus, SSPTxOK); // FaultReset clears this
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   FaultReset = FALSE; // Back to normal
   BytesToTx = 3;
   // Will test transmit paths 1,3,5,7 and 1,3,5,6
   SSPTxTimeOut = SSPTxTOThres - 1;
   PassCnt = SSPTxTimeOut;
   do
   {
      PassCnt++;
      CrossMonitoringTx();
      CrossMonitoringRx();
      if (PassCnt >= SSPTxTOThres)
      {
         DiagPrintInt(SSPTxFaultStatus, SSPTxFault);
      }
      else
      {
         DiagPrintInt(SSPTxFaultStatus, SSPTxNOK);
      }
      DiagPrintInt(SSPRxFaultStatus, SSPRxOK);
   } while (SSPTxFaultStatus == SSPTxNOK);

   FaultReset = TRUE;
   BytesToTx = 2;
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(SSPTxFaultStatus, SSPTxOK); // FaultReset clears this
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);

   // Remove the errors tSINT32roduced to restore normalcy in tests and results
   BytesToTx = 2;
   FaultReset = FALSE;
   CrossMonitoringTx();
   CrossMonitoringRx();
   DiagPrintInt(SSPTxFaultStatus, SSPTxOK);
   DiagPrintInt(SSPRxFaultStatus, SSPRxOK);
   /*Diagnostics of*/
   crossMonitoringTx = 0; // Turn OFF diagnostic messages for Tx
   crossMonitoringRx = 0; // Turn OFF diagnostic messages for Rx
   crossMonitoring = 1; // Turn ON diagnostic messages for CrossMonitoring
   FaultFlag = FALSE;
   CrossMonitoring();
   DiagPrintInt(FaultFlag, FALSE);
   // For traverse paths Rx 1,2,3,16,18
   for (PassCnt = 0; PassCnt < 5; PassCnt++)
   {
      ssp0TxMsg = NoMsgRxd1;
      CrossMonitoringTx();
      CrossMonitoringRx();
   }
   SSPRxTimeOut = SSPRxTOThres;
   CrossMonitoring();
   DiagPrintInt(FaultFlag, CMFaultFlag);
   DiagSummary();
}

#endif

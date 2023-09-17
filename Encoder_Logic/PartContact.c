// $Header:   D:/databases/VMdb/archives/EN13849/PartContact.c_v   1.25   09 Apr 2015 17:12:54   ewomack  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11            */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

 This module implements part Contact Algorithm.

 Module name: PartContact

 Filename:    PartContact.c

 --------------------------- TECHNICAL NOTES -------------------------------



 ------------------------------ REVISIONS ---------------------------------
 $Log:   D:/databases/VMdb/archives/EN13849/PartContact.c_v  $
 * 
 *    Rev 1.25   09 Apr 2015 17:12:54   ewomack
 * DetectPartContact2000X redesigned.  Using velocity algorithm from 2000X.  Allow force to build while verifying part contact.
 * 
 *    Rev 1.24   02 Jan 2015 14:46:36   ewomack
 * Verify Part Contact Algorithm Fix
 * 
 *    Rev 1.22   20 Sep 2011 14:01:32   ASharma6
 * Unit testing code modified for Part contact.
 * 
 *    Rev 1.17   03 Aug 2011 10:32:52   ASharma6
 * For machine configuration C, the part contact lost algorithm shall declare lost 150ms after trigger goes away
 * 
 *    Rev 1.16   20 Jul 2011 16:58:18   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.15   12 Jul 2011 18:34:40   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 *
 *    Rev 1.14   05 Jul 2011 11:00:32   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.11   22 Jun 2011 18:10:34   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.4   01 Apr 2011 11:13:28   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:24   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:12   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 11:29:26   ygupta
 * Added File and Function Headers.
 -------------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "TestMacros.h"
#include "stdio.h"
#include "DeviceDiagnostics.h"
#include "Global.h"
#include "Input.h"
#include "PartContact.h"
#include "ProgramCounterTest.h"
#include "ADCTest.h"
#include "StateMachine.h"

UT_VARIABLE(tSINT32 partContact)
UT_VARIABLE(tSINT32 averageForce)
UT_VARIABLE(tSINT32 calculateSpeed)
UT_VARIABLE(tSINT32 partcontactTriggerSwitch)
UT_VARIABLE(tSINT32 partcontactTest)
UT_VARIABLE(tSINT32 detectPartContactGSXEorP)
UT_VARIABLE(tSINT32 verifyPartContactGSXEorP)
UT_VARIABLE(tSINT32 detectPartContactGSXI)
UT_VARIABLE(tSINT32 verifyPartContactGSXI)
UT_VARIABLE(tSINT32 VelocityEstimatorDiag)
UT_VARIABLE(tSINT32 BubbleSortDiag)
/*-------------------------- Global Variables ------------------------------*/
tSINT32 ForceDistindex = 0;
tSINT32 forceData[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
tSINT32 forceDataCopy[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
tSINT32 Distancedata[MAXSAMPLES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
tUINT32 DebugForceData[300];
tSINT32 DebugPositionData[300];
tSINT32 DebugIndx;
tSINT32 LostCondition = 0;
tSINT32 ForceSamples = 0;
tUINT32 PCStepsCtr;
tSINT32 OldVelocity;
tSINT32 CurrVelocity;
tSINT32 prevForce;
tSINT32 prevdiffForce;
tSINT32 currdiffForce;
tSINT32 OldPosition;
tSINT32 CurrPosition;
tSINT32 OldPressure;
tSINT32 CurrPressure;
tSINT32 PeakVelocity;
tSINT32 PCForce;
tSINT32 PCSpeed;
tSINT32 PCPosition;
tUINT32 ULSLEReading;
tSINT32 PartContactCounter;
tSINT32 PCLostIndex1 = 0;
tSINT32 PCLostIndex2 = 0;
tSINT32 PCLostIndex3 = 0;
tSINT32 Distance = 0;
tSINT32 Sbeam = 0;
tSINT32 distIndex;
tSINT32 PCForceDelay = 0;



int OldTRS;

tSINT32 ForceOffset = 0, OldSVReq = OFF;
tSINT32 FailureData[10];
extern tUINT32 FSum;

static tUINT32 VelBuffer[9] = {10000,10000,10000,10000,10000,0,0,0,0};
tSINT32 NewCurrVelocity = 0;

#define SPEEDINDEX 6
/**************************************************************************//**
 * \brief - 	BubbleSort
 * 
 * \brief -  	Basic bubblesort algorithm to sort data from the lower to higher value
 * 
 * \param - 	tSINT32 array[]: Data array to sort
 * 
 * \param - 	tUINT32 len: Data length
 * 
 * \return -	none
*****************************************************************************/
void BubbleSort(tSINT32 array[], tUINT32 len)
{
	tSINT32 ii, jj, temp;
   FUNCTION1(BubbleSortDiag, "BubbleSort()");
   CHECKPOINT(BubbleSortDiag, BRANCH_1);
	for (ii = 0; ii < len - 1; ii++)
	{
		CHECKPOINT(BubbleSortDiag, BRANCH_2);
		for (jj = 1; jj < len; jj++)
		{
			CHECKPOINT(BubbleSortDiag, BRANCH_3);
			if (array[jj] < array[jj-1])
			{
				CHECKPOINT(BubbleSortDiag, BRANCH_4);
				temp = array[jj-1];
				array[jj-1] = array[jj];
				array[jj] = temp;
			}
		}
	}
	CHECKPOINT(BubbleSortDiag, BRANCH_5);
	ChkFunReturn = 1;
}

/**************************************************************************//**
 * \brief -		VelocityEstimator
 * 
 * \brief -		Estimate velocity during horn downstroke from encoder position readings
 * 				taken at 1ms intervals.  The velocity estimator is called from PartContact.c.
 * 				The filter coefficients are [-1/16,-1/4,-3/8,-1/4,0,1/4,3/8,1/4,1/16]/4.
 * 	
 * \param -		tSINT32 NewEncoderPosition
 * 
 * \return -	none
 * 
*****************************************************************************/
tSINT32 VelocityEstimator(tSINT32 NewEncoderPosition)
{
   tSINT32 Temp,Out;
   FUNCTION1(VelocityEstimatorDiag, "VelocityEstimator()");
   CHECKPOINT(VelocityEstimatorDiag, BRANCH_1);

   VelBuffer[0] = VelBuffer[1];
   VelBuffer[1] = VelBuffer[2];
   VelBuffer[2] = VelBuffer[3];
   VelBuffer[3] = VelBuffer[4];
   VelBuffer[4] = VelBuffer[5];
   VelBuffer[5] = VelBuffer[6];
   VelBuffer[6] = VelBuffer[7];
   VelBuffer[7] = VelBuffer[8];
   VelBuffer[8] = NewEncoderPosition;
   Out = (VelBuffer[8] - VelBuffer[0]) >>2;
   Out = (Out + VelBuffer[7] + VelBuffer[5] - VelBuffer[3] - VelBuffer[1]) >>2;
   Temp = VelBuffer[6] - VelBuffer[2];
   CHECKPOINT(VelocityEstimatorDiag, BRANCH_2);
   return ( Out + (Temp>>1) - (Temp>>3) ) >>2;
}

/**************************************************************************//**
* \brief -	AverageForceReading
*
* \brief -	This function calculates average force getting values from ADC.
* 
* \param -	none
* 
* \return -   none
* 
*****************************************************************************/
void AverageForceandDistanceReading(void)
{
   FUNCTION1(averageForce, "AverageForceReading()");
   CHECKPOINT(averageForce, BRANCH_1);
   tSINT32 i = 0;
   tSINT32 force = 0;

   NewCurrVelocity = VelocityEstimator(QEIPosition * 1000 / 254);
   CHECKPOINT(averageForce, BRANCH_13);
   // Distance array is filled from QEI.
   if (ForceDistindex >= SENSING_INTERVAL)
   {
      CHECKPOINT(averageForce, BRANCH_3);
      // After ten reading this array of Distance samples is shifted one position up will store
      // 11th onwards reading in  the last index.
      while (i < DISTANCE_INTERVAL)
      {
         CHECKPOINT(averageForce, BRANCH_4);
         Distancedata[i] = Distancedata[i + 1];
         i++;
      }
      CHECKPOINT(averageForce, BRANCH_5);
      Distancedata[i] = QEIPosition;
   }
   else
   {
      CHECKPOINT(averageForce, BRANCH_2);
      Distancedata[ForceDistindex++] = QEIPosition;
   }

   if(MachineConfiguration != GSXI)
   {
	   if (ForceSamples < SENSING_INTERVAL)
	   {
		   CHECKPOINT(averageForce, BRANCH_6);
		   ForceSamples++;
	   }
	   else
	   {
		   CHECKPOINT(averageForce, BRANCH_7);
	   }

	   tSINT32 forceCopy[MAX_FORCE_SAMPLE];
	   //Make a copy of the data
	   tSINT32 xx, yy;
	   for (xx = 0; xx < MAX_FORCE_SAMPLE; xx++)
	   {
		   CHECKPOINT(averageForce, BRANCH_8);
		   forceCopy[xx] = forceData[xx];
	   }

	   BubbleSort(forceCopy, 14);
	   CHECKPOINT(averageForce, BRANCH_9);

	   //Take middle 4 values, disregarding the others
	   force += forceCopy[5];
	   force += forceCopy[6];
	   force += forceCopy[7];
	   force += forceCopy[8];
	   force *= FORCE_MULFAC;
	   force /= 4;
	   CurrForce = force;// Average force is stored in CurrForce.
   }
   	
   ChkFunReturn = 1;
}

/**************************************************************************//**
* \brief -	CalculateSpeed
*
* \brief -	This function calculates speed from taking difference between
*			consecutive distance readings.
*
* \param -	none
* 
* \return -	none
* 
*****************************************************************************/
void CalculateSpeed(void)
{
   FUNCTION1(calculateSpeed, "CalculateSpeed()");
   CHECKPOINT(calculateSpeed, BRANCH_1);
   ChkFunReturn = 1;
   tSINT32 Diffdistance = 0;
   // Speed will remain position until samples less than SENSING_INTERVAL that is 10.
   if (ForceSamples < SENSING_INTERVAL)
   {
      CHECKPOINT(calculateSpeed, BRANCH_2);
      Diffdistance = 0;//Distancedata[ForceSamples - 1];
      CurrPosition =Distancedata[ForceSamples - 1];//Diffdistance;
   }
   else
   // Speed will be difference of first and sixth position of the distance samples and
   // Currposition will be position at the sixth position values.
   {
      CHECKPOINT(calculateSpeed, BRANCH_3);
      //distance difference readings. multiply by 100 to get decimal points.
      Diffdistance = (Distancedata[distIndex] - Distancedata[0])*100/distIndex;
      CurrPosition = Distancedata[distIndex];
   }
   CurrVelocity = Diffdistance ;
}

/**************************************************************************//**
* \brief  -        This function detects part contact algorithm for GSXE or GSXP.
* 
* \param  - none
* 
* \return  - none
*****************************************************************************/
void DetectPartContactGSXEorP(void)
{
   FUNCTION1(detectPartContactGSXEorP, "DetectPartContactGSXEorP()");
   CHECKPOINT(detectPartContactGSXEorP, BRANCH_1);

   if (CurrVelocity > PeakVelocity && CurrPosition > MINIMAL_POSITION)
   {
      CHECKPOINT(detectPartContactGSXEorP, BRANCH_2);
      PeakVelocity = CurrVelocity;
      PartContactCounter = 0;
   }
      CHECKPOINT(detectPartContactGSXEorP, BRANCH_3);
      if (NewCurrVelocity < 12 && NewCurrVelocity > -12)   //Checking if actuator is stopped
      {
         CHECKPOINT(detectPartContactGSXEorP, BRANCH_4);
         if(CurrPosition > (QEIPositionOld + MINIMAL_POSITION))
         {
            /* This is for GSXEorP */
            CHECKPOINT(detectPartContactGSXEorP, BRANCH_7);
            if ((CurrForce - ForceOffset) > SMALL_FORCE)
            {
               CHECKPOINT(detectPartContactGSXEorP, BRANCH_9);
               PartContactCounter++;
               if ((CurrForce - ForceOffset) > LARGE_FORCE)
               {
            	   CHECKPOINT(detectPartContactGSXEorP, BRANCH_14);
            	   PartContactCounter += PCFOUND_COUNTER;
               }
               if (PartContactCounter > PCFOUND_COUNTER)
               {
                  CHECKPOINT(detectPartContactGSXEorP, BRANCH_11);
                  PCForce = CurrForce - ForceOffset;
                  if (State == DRIVEON)
                  {  
                     CHECKPOINT(detectPartContactGSXEorP, BRANCH_13);
                     PartContactStatus = DETECTED;
                     PCPosition = CurrPosition;
                  }
                  else
                  {
                     CHECKPOINT(detectPartContactGSXEorP, BRANCH_12);
                     PartContactStatus = NOTDETECTED;
                  }
                  PCLostIndex1 = 0;
                  PCLostIndex2 = 0;
                  PCLostIndex3 = 0;
               }
               else
               {
                  CHECKPOINT(detectPartContactGSXEorP, BRANCH_10);
                  PartContactStatus = NOTDETECTED;
               }
            }
            else
            {
               CHECKPOINT(detectPartContactGSXEorP, BRANCH_8);
               PartContactStatus = NOTDETECTED;
            }
         }
         else
         {
            CHECKPOINT(detectPartContactGSXEorP, BRANCH_6);
            PartContactStatus = NOTDETECTED;
         }
      }
      else
      {
         PartContactCounter = 0;
         PartContactStatus = NOTDETECTED;
         CHECKPOINT(detectPartContactGSXEorP, BRANCH_5);
      }
    ChkFunReturn = 1;
    return;
 }

/**************************************************************************//**
* \brief  -        This function verify part contact algorithm for GSXE and GSXP.
* 
* \param  - none
* 
* \return  - none
*****************************************************************************/
void VerifyPartContactGSXEorP(void)
{
   FUNCTION1(verifyPartContactGSXEorP, "VerifyPartContactGSXEorP()");
   CHECKPOINT(verifyPartContactGSXEorP, BRANCH_1);
   /* For GSXE or GSXP */
   /* There are three conditions to detect part contact lost.
    * Each condition has a separate counter, so if one condition exists for
    * more than the threshold, part contact is declared as "Lost"
   */

   if (PCForceDelay++ < 15 && (CurrForce - ForceOffset) > PCForce)
   {
	   CHECKPOINT(verifyPartContactGSXEorP, BRANCH_18);
	   PCForce = CurrForce - ForceOffset ;
   }
   /*
    * If both hands are pressed, then continue to be in part contact state
    * irrespective of whether part contact is lost.
    */
   if (BothHandStatus == BOTHHANDS_ON)//Both PBs are pressed.
   {
	  CHECKPOINT(verifyPartContactGSXEorP, BRANCH_2);
      PCPosition = CurrPosition;
   }
   else
   {
	  CHECKPOINT(verifyPartContactGSXEorP, BRANCH_3);
      /* The first condition for losing part contact
       * If velocity rises to greater than a third of the peak velocity
      */
      if (((CurrVelocity * SPEED_FACTOR) >> 1) > PeakVelocity)
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_4);
         PCLostIndex1++;
         if (PCLostIndex1 > PCLOST_COUNTER)
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_5);
            PartContactStatus = NOTDETECTED;
            printf("PCLost1\n");
         }
         else
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_6);
         }
      }
      else
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_7);
         PCLostIndex1 = 0;
      }

      /* The second condition for part contact lost
       * If the collapse after part contact is more than 1/4th of an inch and force is less than 25lbs
       */
      if ((CurrPosition > (PCPosition + COLLAPSEDISTGSXE_OR_P)) && ((CurrForce - ForceOffset) < FORCE_25LBS))
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_8);
         PCLostIndex2++;
         if (PCLostIndex2 > PCLOST_COUNTER)
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_9);
            PartContactStatus = NOTDETECTED;
            printf("PCLost2\n");
         }
         else
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_10);
         }
      }
      else
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_11);
         PCLostIndex2 = 0;
      }

   #if 0
      if (CurrVelocity < MIMIMAL_VELOCITY)
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_6);
         PCLostIndex2++;
         if (PCLostIndex2 > PCLOST_COUNTER)
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_7);
            PartContactStatus = NOTDETECTED;
            printf("PCLost2\n");
         }
      }
      else
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_8);
         PCLostIndex2 = 0;
      }
   #endif
      /* The third condition for losing part contact:
       * Force is less than 1/2 the part contact force OR
       * Horn retracts 1/4th of an inch from the part contact position
       */

      if (((FORCE_FACTOR * (CurrForce - ForceOffset)/SCALING_FACTOR)< (PCForce)) ||
    		  (CurrPosition < (PCPosition - RETRACTDISTGSXE_OR_P)))
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_12);
         PCLostIndex3++;
         if (PCLostIndex3 > PCLOST_COUNTER)
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_13);
            PartContactStatus = NOTDETECTED;
            printf("PCLost3\n");
         }
         else
         {
            CHECKPOINT(verifyPartContactGSXEorP, BRANCH_14);
         }
      }
      else
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_15);
         PCLostIndex3 = 0;
      }

      if (PartContactStatus == NOTDETECTED)
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_16);
         PeakVelocity = CurrVelocity; // Resetting peak Velocity with current velocity.
         PCForce = 0; // Resetting PCForce.
      }
      else
      {
         CHECKPOINT(verifyPartContactGSXEorP, BRANCH_17);
      }
   }
   return;
}
/**************************************************************************//**
* \brief -        This function detects part contact algorithm  on GSXI
* 				  machine.
* 						
* \param  - none
* 
* \return - none
*****************************************************************************/
void DetectPartContactGSXI(void)
{
	FUNCTION1(detectPartContactGSXI, "detectPartContactGSXI()");
	CHECKPOINT(detectPartContactGSXI, BRANCH_1);
	if ((OldULSPosition == HOMEPOSITION) && ((ULSPosition == AWAYPOSITION)))
	{
	   CHECKPOINT(detectPartContactGSXI, BRANCH_2);
	   ULSLEReading = CurrPosition;
	}
	else
	{
	   CHECKPOINT(detectPartContactGSXI, BRANCH_3);
	}
	if (CurrPosition > (ULSLEReading + BOUNCYDISTGSXI))
	{
	   CHECKPOINT(detectPartContactGSXI, BRANCH_4);
      if (TRS == TRSON)
      {
         CHECKPOINT(detectPartContactGSXI, BRANCH_5);
         PartContactStatus = DETECTED;
         PCLostIndex1 = PCLOST_COUNTER;
         PCPosition = CurrPosition;
      }
      else
      {
         CHECKPOINT(detectPartContactGSXI, BRANCH_6);
         PartContactStatus = NOTDETECTED;
         }
      }
	else
	{
	   CHECKPOINT(detectPartContactGSXI, BRANCH_7);
	}
   return;
}

/**************************************************************************//**
* \brief -	This function verify part contact algorithm based on
* 			ADC channel 1 value.
* 
* \param  - none
* 
* \return - none
*****************************************************************************/
void VerifyPartContactGSXI(void)
{
   FUNCTION1(verifyPartContactGSXI, "verifyPartContactGSXI()");
   CHECKPOINT(verifyPartContactGSXI, BRANCH_1);
   /*
    * If both hands are pressed, then continue to be in part contact state
    * irrespective of whether part contact is lost.
    */
   if (BothHandStatus == BOTHHANDS_ON)//Both PBs are pressed.
   {
      /*
       * This becomes a new reference for PCPosition until hands are released.
       */
      CHECKPOINT(verifyPartContactGSXI, BRANCH_2);
      PCPosition = CurrPosition;
   }
   else
   {
      CHECKPOINT(verifyPartContactGSXI, BRANCH_3);
      if ((TRS == TRSOFF) || (CurrPosition > (PCPosition + COLLAPSEDISTGSXI)))
      {
         CHECKPOINT(verifyPartContactGSXI, BRANCH_4);
         if (PCLostIndex1 > 0)
         {
            CHECKPOINT(verifyPartContactGSXI, BRANCH_5);
            PCLostIndex1--;
         }
         else
         {
            CHECKPOINT(verifyPartContactGSXI, BRANCH_6);
            PartContactStatus = NOTDETECTED;
         }
      }
      else
      {
         CHECKPOINT(verifyPartContactGSXI, BRANCH_7);
         PCLostIndex1 = PCLOST_COUNTER; // In case trigger is lost for a short while
      }
   }
}

/**************************************************************************//**
* 	\brief  -        This function implements part contact algorithm.
* 
* 	\param  - none
* 
*	\return  - none
*****************************************************************************/
void PartContact(void)
{
   static BOOL firstTime = TRUE;
   tSINT32 X = 0;

   FUNCTION1(partContact, "PartContact()");
   CHECKPOINT(partContact, BRANCH_1);
   // Calculates Average force from force readings and get position.
   AverageForceandDistanceReading();
   CHECKPOINT(partContact, BRANCH_2);
   // Calculates speed from difference two consecutive position in 1 ms.
   CalculateSpeed();
   CHECKPOINT(partContact, BRANCH_3);
   
   
   //Measure Static offset of S-Beam reading to compensate while detecting PC lost
   if ((BothHandStatus == BOTHHANDS_ON) && OldSVReq == OFF)
   {
	   CHECKPOINT(partContact, BRANCH_14);
	   ForceOffset = FSum * 2;
   }

   OldSVReq = SV1_REQ;

   // Initializing PrevForce, Oldposition, PeakVelocity, PartContactStatus,PartContactCounter,
   // distIndex.
   if (firstTime == TRUE)
   {
      prevForce = CurrForce;
      firstTime = FALSE;
      OldPosition = CurrPosition;
      PeakVelocity = CurrVelocity;
      PartContactStatus = NOTDETECTED;
      PartContactCounter = 0;
      distIndex = SPEEDINDEX - 1; /*Index to calculate speed from difference of
       * Position at SPEEDINDEX and Position at SPEEDINDEX - 1
       * SPEEDINDEX is set at sixth position.*/
      PCStepsCtr = 0;
   }
   X = CurrPosition;
   currdiffForce = CurrForce - prevForce; //Force difference from Current and previous
   if (PartContactStatus == DETECTED)
   {
      CHECKPOINT(partContact, BRANCH_4);
      if (MachineConfiguration == GSXI)
      {
        CHECKPOINT(partContact, BRANCH_5);
        VerifyPartContactGSXI();
        CHECKPOINT(partContact, BRANCH_6);
      }
      else
      {
        CHECKPOINT(partContact, BRANCH_7);
        VerifyPartContactGSXEorP();
        prevdiffForce = currdiffForce;
        prevForce = CurrForce;
        CHECKPOINT(partContact, BRANCH_8);
      }
   }
   else
   {
	   PCForceDelay = 0;
      CHECKPOINT(partContact, BRANCH_9);
      if (MachineConfiguration == GSXI)
      {
        CHECKPOINT(partContact, BRANCH_10);
        DetectPartContactGSXI();
        CHECKPOINT(partContact, BRANCH_11);
      }
      else
      {
        CHECKPOINT(partContact, BRANCH_12);
        DetectPartContactGSXEorP();
        prevdiffForce = currdiffForce;
        prevForce = CurrForce;
        CHECKPOINT(partContact, BRANCH_13);
      }
   }
   // Updating the values of OLDVelocity,PrevdiffFOrce,PrevForce and OLD Position.
   OldVelocity = CurrVelocity;
   OldPosition = X;
   OldTRS = TRS;
   ChkFunReturn = 1;
}

/**************************************************************************//**
 * \brief  -	PCDummy4
 *
 * \brief  -	This function is used for PC Counter test.
 *
 * \param  -	none
 * 
 * \return  -	Function2_Return predefined value 4.
 *
 *****************************************************************************/
tSINT32 PCDummy4(void)
{
   return Function4_Return;
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for PartContact test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
 void ResetHandler(void) {
   tSINT32 i = 0;
   tSINT32 prevPartContactStatus = 0;
   DiagInit();
   averageForce = 1;
   ChkFunReturn = 0;

   // Test sample taken for testing only
   tSINT32 forceData1[MAXSAMPLES] = { TESTFORCESAMPLEVALUE1, TESTFORCESAMPLEVALUE2,
         TESTFORCESAMPLEVALUE3, TESTFORCESAMPLEVALUE4, TESTFORCESAMPLEVALUE5,
         TESTFORCESAMPLEVALUE6, TESTFORCESAMPLEVALUE7, TESTFORCESAMPLEVALUE8,
         TESTFORCESAMPLEVALUE9, TESTFORCESAMPLEVALUE10, TESTFORCESAMPLEVALUE11 };
   tSINT32 distData1[MAXSAMPLES] = {TESTDISTSAMPLEVALUE1, TESTDISTSAMPLEVALUE2,
	         TESTDISTSAMPLEVALUE3, TESTDISTSAMPLEVALUE4, TESTDISTSAMPLEVALUE5,
	         TESTDISTSAMPLEVALUE6, TESTDISTSAMPLEVALUE7, TESTDISTSAMPLEVALUE8,
	         TESTDISTSAMPLEVALUE9, TESTDISTSAMPLEVALUE10, TESTDISTSAMPLEVALUE11};
   for (i = 0; i < MAXSAMPLES; i++) {
      forceData[i] = forceData1[i];
      Distancedata[i] = distData1[i];
   }
   ForceDistindex = 0;
   ForceSamples = 0;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 1;
   ForceSamples = 1;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 2;
   ForceSamples = 2;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 3;
   ForceSamples = 3;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 4;
   ForceSamples = 4;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 5;
   ForceSamples = 5;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 6;
   ForceSamples = 6;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 7;
   ForceSamples = 7;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 8;
   ForceSamples = 8;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 9;
   ForceSamples = 9;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   ForceDistindex = 10;
   ForceSamples = 10;
   AverageForceandDistanceReading();
   DiagPrintInt(ChkFunReturn, 1);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   //**Calculate Speed
   //Test 1
   calculateSpeed = 1;
   ForceSamples = 1;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 2;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 3;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 4;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 5;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 6;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 7;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 8;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 9;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   ForceSamples = 10;
   CalculateSpeed();
   DiagPrintInt(ChkFunReturn, 1);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   /************************************PartContact***************************************************/

   ULSPosition = AWAYPOSITION;
   partContact = 1;
   detectPartContactGSXEorP = 0;
   verifyPartContactGSXEorP = 0;
   detectPartContactGSXI = 0;
   verifyPartContactGSXI = 0;
   averageForce = 0;
   calculateSpeed = 0;

   //GSXI ONLY
//   PartContactStatus = NOTDETECTED;
//   MachineConfiguration = GSXI;
//   PartContact();
//   DiagPrintInt(ChkFunReturn, 1);
 ////////////////////////////////////////  

   PartContactStatus = NOTDETECTED;
   MachineConfiguration = GSXE;
   PartContact();
   DiagPrintInt(ChkFunReturn, 1);
   
   //GSXI ONLY
//   PartContactStatus = DETECTED;
//   MachineConfiguration = GSXI;
//   PartContact();
//   DiagPrintInt(ChkFunReturn, 1);
   //////////////////////////////////

   PartContactStatus = DETECTED;
   MachineConfiguration = GSXE;
   PartContact();
   DiagPrintInt(ChkFunReturn, 1);

//   ULSPosition = HOMEPOSITION;
//   SV1_REQ = ON;
//   OldSVReq = OFF;
//   PartContactStatus = DETECTED;
//   MachineConfiguration = GSXE;
//   PartContact();
//   DiagPrintInt(ChkFunReturn, 1);

   PartContactStatus = NOTDETECTED;
   MachineConfiguration = GSXE;
   OldSVReq = OFF;
   PartContact();
   DiagPrintInt(ChkFunReturn, 1);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   partContact = 1;
   ForceSamples = 10;
   ForceDistindex = 10;

   /************************************DetectPartContactGSXE***************************************************/

   //Test 1
   detectPartContactGSXEorP = 1;
   State = DRIVEON;
   CurrVelocity = TESTVELOCITYTEST1;
   PeakVelocity = TESTPEAKVELOCITYTEST1;
   NewCurrVelocity = TESTNEWVELOCITY1;
   CurrPosition = TESTQEIPOSITION1;
   DetectPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   //Test 2
   CurrVelocity = TESTVELOCITYTEST2;
   PeakVelocity = TESTPEAKVELOCITYTEST2;
   NewCurrVelocity = TESTNEWVELOCITY2;
   DetectPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   //Test 3
   CurrVelocity=TESTVELOCITYTEST8;
   PeakVelocity=TESTPEAKVELOCITYTEST8;
   CurrForce = TESTAVGFORCETENSAMPLES8;
   ForceOffset = CurrForce;
   currdiffForce = TESTCURRDIFFFORCE8;
   prevForce = TESTPREVFORCE8;
   prevdiffForce = TESTPREVDIFFFORCE8;
   CurrPosition = TESTQEIPOSITION0;
   DetectPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   //Test 4
   CurrVelocity=TESTVELOCITYTEST8;
   PeakVelocity=TESTPEAKVELOCITYTEST8;
   CurrForce = TESTAVGFORCETENSAMPLES8;
   PartContactCounter = 0;
   ForceOffset = CurrForce - SMALL_FORCE - 1;
   currdiffForce = TESTCURRDIFFFORCE8;
   prevForce = TESTPREVFORCE8;
   prevdiffForce = TESTPREVDIFFFORCE8;
   CurrPosition = TESTQEIPOSITION8;
   ADC0Value[1] = ADC0VOLT_200X;
   DetectPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   //Test 5
   CurrVelocity = TESTVELOCITYTEST7;
   PeakVelocity = CurrVelocity - 1;
   CurrForce = TESTAVGFORCETENSAMPLES7;
   ForceOffset = CurrForce - LARGE_FORCE - 1;
   prevForce = TESTPREVFORCE7;
   currdiffForce = TESTCURRDIFFFORCE7;
   prevdiffForce = TESTPREVDIFFFORCE7;
   CurrPosition = TESTQEIPOSITION7;
   ADC0Value[1] = ADC0VOLT_200X;
   PartContactCounter = TESTPARTCONTACTCOUNTER7;
   DetectPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);

//   CurrVelocity = TESTVELOCITYTEST6;
//   PeakVelocity = TESTPEAKVELOCITYTEST6;
//   CurrForce = TESTAVGFORCETENSAMPLES6;
//   currdiffForce = TESTCURRDIFFFORCE6;
//   prevForce = TESTPREVFORCE6;
//   prevdiffForce = TESTPREVDIFFFORCE6;
//   CurrPosition = TESTQEIPOSITION6;
//   ADC0Value[1] = ADC0VOLT_200X;
//   PartContactCounter = TESTPARTCONTACTCOUNTER6;
   State = READY;
   DetectPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   /*CurrVelocity = TESTVELOCITYTEST6;
   PeakVelocity = TESTPEAKVELOCITYTEST6;
   CurrForce = TESTAVGFORCETENSAMPLES6;
   currdiffForce = TESTCURRDIFFFORCE6;
   prevForce = TESTPREVFORCE6;
   prevdiffForce = TESTPREVDIFFFORCE6;
   CurrPosition = TESTQEIPOSITION6;
   ADC0Value[1] = ADC0VOLT_200X;
   PartContactCounter = PARTCONTACT_COUNTER;
   State = READY;
   DetectPartContact2000X();
   DiagPrintInt(PartContactStatus, NOTDETECTED);
   State = SVON;
   DetectPartContact2000X();
   DiagPrintInt(PartContactStatus, DETECTED);*/
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();
   /*------------------Verify Part Contact2000X-----------*/
   ForceOffset = 0;
   PCForceDelay = 30;
   verifyPartContactGSXEorP = 1;
   BothHandStatus = BOTHHANDS_ON;
   PartContactStatus = DETECTED;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);

   //Test 2
   ForceOffset = 0;
   BothHandStatus = BOTHHANDS_OFF;
   CurrForce = TESTAVGFORCETENSAMPLES12;
   CurrPosition = TESTQEIPOSITION12;
   CurrVelocity = TESTVELOCITYTEST12;
   ADC0Value[1] = ADC0VOLT_200X;
   PeakVelocity = TESTPEAKVELOCITYTEST12;
   PCLostIndex3 = TESTPCINDEX12;
   PartContactCounter = TESTPARTCONTACTCOUNTER12;
   currdiffForce = TESTCURRDIFFFORCE12;
   prevdiffForce = TESTPREVDIFFFORCE12;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);

   //Test 3
   prevPartContactStatus = PartContactStatus;
   PartContactStatus = NOTDETECTED;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);
   PartContactStatus = prevPartContactStatus;

   //Test 4
   CurrForce = 0;
   ForceOffset = 0;
   PCForce = 1;
   CurrPosition = TESTQEIPOSITION9;
   CurrVelocity = TESTVELOCITYTEST9;
   ADC0Value[1] = ADC0VOLT_200X;
   PeakVelocity = TESTPEAKVELOCITYTEST9;
   PCLostIndex1 = PCLOST_COUNTER - 1;
   PCLostIndex3 = PCLOST_COUNTER - 1;
   PartContactCounter = TESTPARTCONTACTCOUNTER9;
   currdiffForce = TESTCURRDIFFFORCE9;
   prevdiffForce = TESTPREVDIFFFORCE9;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);


   //Test 5
   PartContactStatus = DETECTED;
   CurrPosition = (PCPosition + COLLAPSEDISTGSXE_OR_P);
   CurrForce = TESTAVGFORCETENSAMPLES12;
   CurrVelocity = TESTVELOCITYTEST12;
   ADC0Value[1] = ADC0VOLT_200X;
   PeakVelocity = TESTPEAKVELOCITYTEST12;
   PCLostIndex1 = 0;
   PCLostIndex3 = 0;
   currdiffForce = TESTCURRDIFFFORCE12;
   prevdiffForce = TESTPREVDIFFFORCE12;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);

   //Test 6
   PCLostIndex2 = PCLOST_COUNTER - 1;
   CurrPosition ++;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);

   //Test 7
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   //Test 8
   PartContactStatus = DETECTED;
   CurrVelocity = TESTVELOCITYTEST12;
   PeakVelocity = TESTPEAKVELOCITYTEST12;
   CurrPosition = (PCPosition - RETRACTDISTGSXE_OR_P - 1);
   PCForce = ((FORCE_FACTOR * CurrForce) + 1);
   PCLostIndex3 = PCLOST_COUNTER - 1;
   PCLostIndex2 = 0;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, DETECTED);

   //Test 9
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   //Test 10
   PCForceDelay = 0;
   PCForce = 1;
   CurrForce = 2;
   VerifyPartContactGSXEorP();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   /************************************DetectPartContactIW***************************************************/
   detectPartContactGSXI = 1;
   CurrPosition = 0;
   PartContactStatus = NOTDETECTED;
   TRS = TRSON;
   OldULSPosition = HOMEPOSITION;
   ULSPosition = AWAYPOSITION;
   ULSLEReading = 0;
   DetectPartContactGSXI();
   DiagPrintInt(PartContactStatus, NOTDETECTED);
   TRS = TRSON;
   ULSPosition = HOMEPOSITION;
   CurrPosition = ULSLEReading + 2*BOUNCYDISTGSXI;
   DetectPartContactGSXI();
   DiagPrintInt(PartContactStatus, DETECTED);
   TRS = TRSOFF;
   DetectPartContactGSXI();
   DiagPrintInt(PartContactStatus, NOTDETECTED);
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   /************************************VerifyPartContactIW***************************************************/
   verifyPartContactGSXI = 1;
   BothHandStatus = BOTHHANDS_ON;
   PartContactStatus = DETECTED;
   VerifyPartContactGSXI();
   DiagPrintInt(PartContactStatus, DETECTED);
   BothHandStatus = BOTHHANDS_OFF;
   CurrPosition = PCPosition + COLLAPSEDISTGSXI;
   TRS = TRSON;
   VerifyPartContactGSXI();
   DiagPrintInt(PartContactStatus, DETECTED);
   TRS = TRSOFF;
   VerifyPartContactGSXI();
   DiagPrintInt(PartContactStatus, DETECTED);
   CurrPosition++;
   PCLostIndex1 = 0;
   VerifyPartContactGSXI();
   DiagPrintInt(PartContactStatus, NOTDETECTED);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();
   /**********************************VelocityEstimator*****************************************************/
   VelocityEstimatorDiag = 1;
   VelocityEstimator(0);
   DiagPrintInt(ChkFunReturn, 1);
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

   /**********************************BubbleSort*****************************************************/
   tSINT32 array[] = {2, 1, 3, 0, 6, -1};
   BubbleSortDiag = 1;
   BubbleSort(array, 4);
   DiagPrintInt(array[0], 0);
   BubbleSort(array, 6);
   DiagPrintInt(array[5], 6);
   DiagSummary();
   for (;;) {
   }
}
#endif

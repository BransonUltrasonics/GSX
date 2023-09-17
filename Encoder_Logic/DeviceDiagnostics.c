// $Header:   D:/databases/VMdb/archives/EN13849/DeviceDiagnostics.c_v   1.16   20 Jul 2011 16:56:16   ASharma6  $
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

This is the module for the DeviceDiagnostics for all the devices on the Actuator controller board.

Module name: DeviceDiagnostics

Filename:    DeviceDiagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Diagnos the  states of all the external devices connected to the controller.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/DeviceDiagnostics.c_v  $
 * 
 *    Rev 1.16   20 Jul 2011 16:56:16   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.15   05 Jul 2011 10:59:16   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.12   22 Jun 2011 18:09:02   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.9   15 Jun 2011 17:44:20   ygupta
 * EStop and LE diagnostics fix
 *
 *    Rev 1.8   08 Jun 2011 07:45:50   pdwivedi
 * Added changes for phase 8.
 *
 *    Rev 1.7   06 May 2011 11:39:34   pdwivedi
 * Added changes for Setoutputs, Inputs and SystemInitialization.
 *
 *    Rev 1.6   04 May 2011 08:57:14   pdwivedi
 * Added changes for cross monitoring(module level test cases), Device Diagnostics(channged) and SBeam Diagnostics(modified).
 *
 *    Rev 1.5   08 Apr 2011 08:33:34   PDwivedi
 * Added after sanity check.
 *
 *    Rev 1.4   01 Apr 2011 11:12:50   PDwivedi
 * Fixes after code review.
 *
 *    Rev 1.3   30 Mar 2011 10:57:04   ASharma6
 * Tabs coverted to 3 spaces
 *
 *    Rev 1.2   29 Mar 2011 19:21:18   ASharma6
 * Modifications done to comply to coding standards
 *
 *    Rev 1.1   25 Mar 2011 09:04:06   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/


#include "TestMacros.h"
#include "DeviceDiagnostics.h"
#include "Global.h"

UT_VARIABLE(tSINT32 DeviceDiagEnable)
UT_VARIABLE(tSINT32 DriveStatusFillOK)
UT_VARIABLE(tSINT32 DriveStatusFillFault)
UT_VARIABLE(tSINT32 SonicsEnStatusFillOK)
UT_VARIABLE(tSINT32 SonicsEnStatusFillFault)
UT_VARIABLE(tSINT32 EStopResultFillOK)
UT_VARIABLE(tSINT32 EStopResultFillFault)
UT_VARIABLE(tSINT32 LEStatusFillOK)
UT_VARIABLE(tSINT32 LEStatusFillFault)
UT_VARIABLE(tSINT32 TRSStatusFillOK)
UT_VARIABLE(tSINT32 TRSStatusFillFault)
UT_VARIABLE(tSINT32 SBeamStatusFillOK)
UT_VARIABLE(tSINT32 SBeamStatusFault)
UT_VARIABLE(tSINT32 UFAILStatusFillOK)
UT_VARIABLE(tSINT32 UFAILStatusFault)
UT_VARIABLE(tSINT32 PBStatusFillOK)
UT_VARIABLE(tSINT32 PBStatusFault)
UT_VARIABLE(tSINT32 TestWDDiag)


/**************************************************************************//**
* \brief -    DeviceDiagnostics
*
* \brief -     Verify that all the devices connected to the controller
*                   are working as expected.
*
* \param -		none
* 
* \return -   none
*
******************************************************************************/
void DeviceDiagnostics(void)
{
	
   FUNCTION1(DeviceDiagEnable, "DeviceDiagnostics()");
   CHECKPOINT(DeviceDiagEnable, BRANCH_1);
  
   if(LUTestStatus == OK)
   {  
	   UFAIL24VDiagnostics();
	   CHECKPOINT(DeviceDiagEnable, BRANCH_2);
	   FAULT(UFAILStatusFillOK, {UFAIL24Status = OK;});
	   FAULT(UFAILStatusFault, {UFAIL24Status = NOK;});
	   if (UFAIL24Status == OK)
	   {
		  CHECKPOINT(DeviceDiagEnable, BRANCH_3);
		  EStopDiagnostics();
		  CHECKPOINT(DeviceDiagEnable, BRANCH_5);
		  FAULT(EStopResultFillOK, {EStopStatus = OK;});
		  FAULT(EStopResultFillFault, {EStopStatus = NOK;});
		  if (EStopStatus != NOK)
		  {
			 /* It takes 2 seconds to declare a fault on Estop diagnostics
			  * if the two poles are read same. Until then, the hazardous
			  * motions shall be disabled.
			  * So, EStop diagnostics has 3 states now as compared to others:
			  * OK
			  * NOK
			  * INTERMEDIATE
			  * Hence, the test is equivalent to
			  * ((EStop == OK) || (EStopStatus == INTERMEDIATE))
			  */
			 CHECKPOINT(DeviceDiagEnable, BRANCH_6);
			 DriveDiagnostics();
			 CHECKPOINT(DeviceDiagEnable, BRANCH_8);
			 FAULT(DriveStatusFillOK, {DriveStatus = OK;});
			 FAULT(DriveStatusFillFault, {DriveStatus = NOK;});
			 if (DriveStatus == OK)
			 {
				CHECKPOINT(DeviceDiagEnable, BRANCH_9);
				SonicsEnDiagnostics();
				CHECKPOINT(DeviceDiagEnable, BRANCH_11);
				FAULT(SonicsEnStatusFillOK, {SonicsEnStatus = OK;});
				FAULT(SonicsEnStatusFillFault, {SonicsEnStatus = NOK;});
				if(SonicsEnStatus == OK)
				{
					CHECKPOINT(DeviceDiagEnable, BRANCH_12);
					ULSLEDiagnostics();
					CHECKPOINT(DeviceDiagEnable, BRANCH_14);
					FAULT(LEStatusFillOK, {LEStatus = OK;});
					FAULT(LEStatusFillFault, {LEStatus = NOK;});
					if (LEStatus == OK)
					{
						CHECKPOINT(DeviceDiagEnable, BRANCH_15);
						TRSDiagnostics();
						CHECKPOINT(DeviceDiagEnable, BRANCH_17);
						FAULT(TRSStatusFillOK, {TRSStatus = OK;});
						FAULT(TRSStatusFillFault, {TRSStatus = NOK;});
						if (TRSStatus == OK)
						{
							CHECKPOINT(DeviceDiagEnable, BRANCH_18);
							SBeamDiagnostics();
							CHECKPOINT(DeviceDiagEnable, BRANCH_20);
							FAULT(SBeamStatusFillOK, {SBeamStatus = OK;});
							FAULT(SBeamStatusFault, {SBeamStatus = NOK;});
							if (SBeamStatus == OK)
							{
								CHECKPOINT(DeviceDiagEnable, BRANCH_21);
								FAULT(PBStatusFillOK, {PBStatus = OK;});
								FAULT(PBStatusFault, {PBStatus = NOK;});
								if(PBStatus == OK)
								{
									CHECKPOINT(DeviceDiagEnable, BRANCH_23);
											  
								}
								else
								{
									CHECKPOINT(DeviceDiagEnable, BRANCH_24);
									FaultFlag |= PBFaultFlag;
									ALARM = FirstBitFoundPos(PBFaultFlag);
								}
						
							}
							else
							{
								CHECKPOINT(DeviceDiagEnable, BRANCH_22);
								FaultFlag |= SBeamFaultFlag;
								ALARM = FirstBitFoundPos(SBeamFaultFlag);
							}
						}
						else
						{
							CHECKPOINT(DeviceDiagEnable, BRANCH_19);
							FaultFlag |= TRSFaultFlag;
							ALARM = FirstBitFoundPos(TRSFaultFlag);
						}
					}
					else
					{
						CHECKPOINT(DeviceDiagEnable, BRANCH_16);
						FaultFlag |= LEFaultFlag;
						ALARM = FirstBitFoundPos(LEFaultFlag);
					}
				}
				else
				{
					CHECKPOINT(DeviceDiagEnable, BRANCH_13);
					FaultFlag |= SonicsEnFaultFlag;
					ALARM = FirstBitFoundPos(SonicsEnFaultFlag);
				}
				
			 }
			 else
			 {
				CHECKPOINT(DeviceDiagEnable, BRANCH_10);
				FaultFlag |= DriveFaultFlag;
				ALARM = FirstBitFoundPos(DriveFaultFlag);
			 }
		  }
		  else
		  {
			 CHECKPOINT(DeviceDiagEnable, BRANCH_7);
			 FaultFlag |= EStopFaultFlag;
			 ALARM = FirstBitFoundPos(EStopFaultFlag);
		  }
	   }
	   else
	   {
		  CHECKPOINT(DeviceDiagEnable, BRANCH_4);
		  FaultFlag |= UFAIL24FaultFlag;
		  ALARM = FirstBitFoundPos(UFAIL24FaultFlag);
	   }
   }
   else
   {
	   CHECKPOINT(DeviceDiagEnable, BRANCH_25);
	   FaultFlag |= LUFaultFlag;
	   ALARM = FirstBitFoundPos(LUFaultFlag);
   }
   if((NotMyFault == TRUE)&&(FaultFlag != FALSE))
   {
      CHECKPOINT(DeviceDiagEnable, BRANCH_26);
      NotMyFault = FALSE;
   }
   else
      CHECKPOINT(DeviceDiagEnable, BRANCH_27);
}

/**************************************************************************//**
*
* 	\brief -     This function returns the position of the first bit found                
*
* 	\param  -  Data: Data that will be scanned to get the first bit position
* 
* 	\return - first bit position
*
******************************************************************************************/
tUINT16 FirstBitFoundPos(tUINT32 Data)
{
	tUINT32 Counter = 0;
	BOOL FirstBitFound = false;
	tUINT16 BitPos = 0;
	if(Data != 0)
	{
		for(Counter = 0 ; (FirstBitFound == false);Counter++)
		{
			if(((Data>>Counter) & 1) == true)
			{
				FirstBitFound= true;
				BitPos = Counter +1;
			}
		}
	}
	
	return (BitPos);
}
#if UNIT_TESTING
/**************************************************************************//**
* \brief -    	ResetHandler
*
* \brief -     ResetHandler is the entry for DeviceDiagnostics unit testing.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   DeviceDiagEnable = 1;
   NotMyFault = FALSE;
   //DeviceDiagnostics test
   // test case 1
   LUTestStatus = OK;
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   LEStatusFillOK = 0;
   LEStatusFillFault = 1;
   TRSStatusFillOK = 0;
   TRSStatusFillFault = 1;
   SBeamStatusFillOK = 0;
   SBeamStatusFault = 1;
   PBStatusFillOK = 0;
   PBStatusFault = 1;
   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, FALSE);

   // test case 2
   FaultFlag = FALSE;
   UFAILStatusFillOK = 1;
   UFAILStatusFault = 0;
   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, UFAIL24FaultFlag);

   // test case 3
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 1;
   EStopResultFillFault = 0;

   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, EStopFaultFlag);

   // test case 4
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 1;
   DriveStatusFillFault = 0;

   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, DriveFaultFlag);
   
   //test case 5
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 1;
   SonicsEnStatusFillFault = 0;
   
   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, SonicsEnFaultFlag);

   // test case 6
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   LEStatusFillOK = 1;
   LEStatusFillFault = 0;

   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, LEFaultFlag);

   // test case 7
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   LEStatusFillOK = 0;
   LEStatusFillFault = 1;
   TRSStatusFillOK = 1;
   TRSStatusFillFault = 0;

   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, TRSFaultFlag);

   // test case 8
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   LEStatusFillOK = 0;
   LEStatusFillFault = 1;
   TRSStatusFillOK = 0;
   TRSStatusFillFault = 1;
   SBeamStatusFillOK = 1;
   SBeamStatusFault = 0;

   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, SBeamFaultFlag);

   // test case 9
    FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   LEStatusFillOK = 0;
   LEStatusFillFault = 1;
   TRSStatusFillOK = 0;
   TRSStatusFillFault = 1;
   SBeamStatusFillOK = 0;
   SBeamStatusFault = 1;
   PBStatusFillOK = 1;
   PBStatusFault = 0;
   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, PBFaultFlag);

   // test case 10
   FaultFlag = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   LEStatusFillOK = 0;
   LEStatusFillFault = 1;
   TRSStatusFillOK = 0;
   TRSStatusFillFault = 1;
   SBeamStatusFillOK = 0;
   SBeamStatusFault = 1;
   PBStatusFillOK = 1;
   PBStatusFault = 0;
   NotMyFault = TRUE;
   DeviceDiagnostics();
   DiagPrintInt(NotMyFault, FALSE);

   // test case 11
   FaultFlag = FALSE;
   NotMyFault = FALSE;
   UFAILStatusFillOK = 0;
   UFAILStatusFault = 1;
   EStopResultFillOK = 0;
   EStopResultFillFault = 1;
   DriveStatusFillOK = 0;
   DriveStatusFillFault = 1;
   SonicsEnStatusFillOK = 0;
   SonicsEnStatusFillFault = 1;
   LEStatusFillOK = 0;
   LEStatusFillFault = 1;
   TRSStatusFillOK = 0;
   TRSStatusFillFault = 1;
   SBeamStatusFillOK = 0;
   SBeamStatusFault = 1;

   DeviceDiagnostics();
   DiagPrintInt(FaultFlag, FALSE);

   DiagSummary();
}

#endif

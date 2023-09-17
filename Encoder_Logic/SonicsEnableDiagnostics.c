/*
 * SonicsEnableDiagnostics.c
 *
 *  Created on: Sep 24, 2018
 *      Author: eguajardo
 */


#include "inc/SonicsEnableDiagnostics.h"
#include "LPC17xx.h"
#include "Global.h"
#include "TestMacros.h"
#include "Diagnostics.h"

/*-------------------------- Global Variables ------------------------------*/
tSINT32 SonicsEnCount;

UT_VARIABLE(tSINT32 CheckSonicsEnDiagnostics)
/*--------------------------------------------------------------------------*/

/**************************************************************************//**
* \brief -    	SonicsEnDiagnostics
*
* \brief -     Diagnose the Sonics Enable signal.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void SonicsEnDiagnostics(void)
{
   FUNCTION1(CheckSonicsEnDiagnostics, "SonicsEnDiagnostics()");
   CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_1);

   if (EStop == !ESTOPPRESSED)
   {
      CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_2);
   }
   else
   {
      /* EStop is not pressed */
      CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_3);
      if (SONICS_FAIL == SONICS_EN)
      {
    	  SonicsEnCount = 0;
        /* Relay state is Drive */
         CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_8);
         if (FaultReset == TRUE)
         {
        	 SonicsEnStatus = OK;
            CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_10);
         }
         else
         {
            CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_9);
         }
      }
      else
      {
         /* Relay state is not Drive */
         CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_4);
         SonicsEnCount++;
         if (SonicsEnCount > SONICSEN_COUNT)
         {
            CHECKPOINT(CheckSonicsEnDiagnostics,BRANCH_6);
            SonicsEnStatus = NOK;
         }
         else
         {
            CHECKPOINT(CheckSonicsEnDiagnostics, BRANCH_5);
         }
      }
   }
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -    ResetHandler
*
* \brief -    ResetHandler is the entry for SonicsEn Diagnostics unit testing.
*
* \param -    none
* 
* \return -   none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
   CheckSonicsEnDiagnostics = 1;
   SonicsEnCount = 0;

   // Test with EStops not pressed and Relay state as Drive, Fault Reset, expect it to pass
   EStop = ESTOPPRESSED;
   SONICS_FAIL = SONICS_EN;
   FaultReset = TRUE;
   SonicsEnDiagnostics();
   DiagPrintInt(SonicsEnStatus, OK);

   // Test with EStops not pressed and Relay state as Drive, No Fault Reset, expect it to pass
   EStop = ESTOPPRESSED;
   SONICS_FAIL = SONICS_EN;
   FaultReset = FALSE;
   SonicsEnDiagnostics();
   DiagPrintInt(SonicsEnStatus, OK);

   // Test with EStops not pressed and Relay state as not Drive, DriveCount is less than threshold,expect it to pass
   EStop = ESTOPPRESSED;
   SONICS_FAIL = ~SONICS_EN;
   SonicsEnCount = 2;
   SonicsEnDiagnostics();
   DiagPrintInt(SonicsEnStatus, OK);

   // Test with EStop1 not Pressed and Relay state not drive and count > Threshold, expect it to pass
   EStop = ESTOPPRESSED;
   SONICS_FAIL = ~SONICS_EN;
   SonicsEnCount = SONICSEN_COUNT+1;
   SonicsEnDiagnostics();
   DiagPrintInt(SonicsEnStatus, NOK);

   // Test with EStop2 Pressed and Relay state not drive and count > Threshold, expect it to pass
   EStop = !ESTOPPRESSED;
   SonicsEnDiagnostics();
   DiagPrintInt(SonicsEnStatus,NOK);

   DiagSummary();
}

#endif

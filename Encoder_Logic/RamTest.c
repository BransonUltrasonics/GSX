// $Header:   D:/databases/VMdb/archives/EN13849/RamTest.c_v   1.11   09 Apr 2015 17:13:38   ewomack  $
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

 This is the module for RAM Test.

 Module name: RamTest

 Filename:    RamTest.c

 --------------------------- TECHNICAL NOTES -------------------------------

 This code Handles the RAM Test.

 ------------------------------ REVISIONS ---------------------------------
 $Log:   D:/databases/VMdb/archives/EN13849/RamTest.c_v  $
 * 
 *    Rev 1.11   09 Apr 2015 17:13:38   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.10   20 Jul 2011 16:58:38   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.9   05 Jul 2011 11:00:48   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.6   22 Jun 2011 18:10:54   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:13:08   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:07:52   ygupta
 * Initial Version with File and Function Headers. 
 -------------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "TestMacros.h"
#include "ProgramCounterTest.h"
#include "Global.h"
#define PATTERN 0x55555555
#define MARCHLOOP 2
UT_VARIABLE(tSINT32 ramTest)
UT_VARIABLE(tSINT32 marchFill)
UT_VARIABLE(tSINT32 marchIncr)
UT_VARIABLE(tSINT32 marchDecr)
UT_VARIABLE(tSINT32 ramTestFault)
UT_VARIABLE(tSINT32 marchFillFault)
UT_VARIABLE(tSINT32 marchIncrFault)
UT_VARIABLE(tSINT32 marchDecrFault)

//#if UNIT_TESTING
//tUINT32 * START_ADDRESS;
//tUINT32 * END_ADDRESS;
/*#else
#define START_ADDRESS   ((tUINT32 *) 0x10000000)
#define END_ADDRESS     ((tUINT32 *) 0x10008000)
#endif*/

/**************************************************************************//**
* \brief -	MarchFill
*
* \brief -	Fill the Pattern on RAM Area.
*
* \param -	START_ADDRESS: Start Address of RAM.
*      		END_ADDRESS: End Address of RAM.
*       	pattern:Pattern to be written.
*                      
* \return -	none
*
*****************************************************************************/
void MarchFill(tUINT32 * START_ADDRESS,tUINT32 * END_ADDRESS,tUINT32 pattern)
{
   tUINT32 * pntr;

   FUNCTION(marchFill, "MarchFill(%08lX)", pattern);
   CHECKPOINT(marchFill, BRANCH_1);
   for (pntr = START_ADDRESS; pntr < END_ADDRESS; pntr++)
   {
      CHECKPOINT(marchFill, BRANCH_2);
      *pntr = pattern;
      FAULT(marchFillFault, {*pntr = ~pattern;});
   }
   CHECKPOINT(marchFill, BRANCH_3);
}

/**************************************************************************//**
* \brief - 		MarchIncr
*
* \brief - 		Test the range of memory in increasing order.The Requested
*          		pattern will be written to a location for up to 'write' times
*           	and verified that it is written before writing the inverse of
*             	the pattern in the same location. This newly written inverse
*              	pattern will now be checked by reading the same location 'read'
*             	times. Repeat the process for whole area.
*
* \param -   	START_ADDRESS: Start Address of RAM.
*             	END_ADDRESS: End Address of RAM.
*              	pattern:Pattern to be written.
*             	read: number of times to be read.
*             	write: number of times to be written.
*                      
* \return -      PASS/FAIL.
*
*****************************************************************************/
tSINT32 MarchIncr(tUINT32 * START_ADDRESS,tUINT32 * END_ADDRESS,tUINT32 pattern, tSINT32 read, tSINT32 write)
{
   tUINT32 * pntr;
   tSINT32 rd, wr;
   tSINT32 i, Result = FAIL;

   FUNCTION(marchIncr, "MarchIncr(%08lX, %ld, %ld): ", pattern, read, write);
   CHECKPOINT(marchIncr, BRANCH_1);

   for (i = 0; i < MARCHLOOP; i++) // Alternet address in a loop.
   {
      CHECKPOINT(marchIncr, BRANCH_3);
      for (pntr = START_ADDRESS + i; pntr < END_ADDRESS; pntr += 2)
      {
         CHECKPOINT(marchIncr, BRANCH_4);
         for (wr = 0; wr < write; wr++)
         {
            CHECKPOINT(marchIncr, BRANCH_5);
            *pntr = pattern;
         }
         CHECKPOINT(marchIncr, BRANCH_6);
         FAULT(marchIncrFault, {*pntr = ~pattern;});
         if (*pntr != pattern)
         {
            CHECKPOINT(marchIncr, BRANCH_7);
            goto Done;
         }
         CHECKPOINT(marchIncr, BRANCH_8);
         *pntr = ~pattern;
			for (rd = 0; rd < read; rd++)
			{
				 CHECKPOINT(marchIncr, BRANCH_9);
				 FAULT(marchIncrFault, {*pntr = pattern;});
				 if (*pntr != (~pattern))
				 {
					  CHECKPOINT(marchIncr, BRANCH_10);
					  goto Done;
				 }
				 CHECKPOINT(marchIncr, BRANCH_13);
			}
          CHECKPOINT(marchIncr, BRANCH_12);
      }
      CHECKPOINT(marchIncr, BRANCH_11);
   }
    CHECKPOINT(marchIncr, BRANCH_2);
    Result = PASS;
Done:
    return Result;
}

/**************************************************************************//**
* \brief -   	MarchDecr
*
* \brief -   	Test the range of memory in decreasing order.The Requested
*            	pattern will be written to a location for up to 'write' times
*         		and verified that it is written before writing the inverse of
*              	the pattern in the same location. This newly written inverse
*              	pattern will now be checked by reading the same location 'read'
*             	times. Repeat the process for whole area.
*
* \param -  	START_ADDRESS: Start Address of RAM.
*             	END_ADDRESS: End Address of RAM.
*             	pattern:Pattern to be written.
*             	read: number of times to be read.
*              	write: number of times to be written.
*              	
* \return -    	PASS/FAIL.
*
*****************************************************************************/
tSINT32 MarchDecr(tUINT32 * START_ADDRESS,tUINT32 * END_ADDRESS,tUINT32 pattern, tSINT32 read, tSINT32 write)
{
    tUINT32 *pntr;
    tSINT32 i, rd, wr, Result = FAIL;

    FUNCTION(marchDecr, "MarchDecr(%08lX, %ld, %ld): ", pattern, read, write);
    CHECKPOINT(marchDecr, BRANCH_1);

    for (i = 0; i < MARCHLOOP; i++)
    {
        CHECKPOINT(marchDecr, BRANCH_3);
        for (pntr = END_ADDRESS - 2 + i; pntr >= START_ADDRESS; pntr -= 2)
        {
            CHECKPOINT(marchDecr, BRANCH_4);
            for (wr = 0; wr < write; wr++)
            {
                *pntr = pattern;
                CHECKPOINT(marchDecr, BRANCH_5);
            }
            CHECKPOINT(marchDecr, BRANCH_6);
            FAULT(marchDecrFault, {*pntr = ~pattern;});
            if (*pntr != pattern)
            {
                CHECKPOINT(marchDecr, BRANCH_7);
                goto DONE;
            }
            CHECKPOINT(marchDecr, BRANCH_8);
            *pntr = ~pattern;
            for (rd = 0; rd < read; rd++)
            {
                CHECKPOINT(marchDecr, BRANCH_9);
                FAULT(marchDecrFault, {*pntr = pattern;});
                if (*pntr != ~pattern)
                {
                    CHECKPOINT(marchDecr, BRANCH_10);
                    goto DONE;
                }
                CHECKPOINT(marchDecr, BRANCH_13);
            }
            CHECKPOINT(marchDecr, BRANCH_12);
        }
        CHECKPOINT(marchDecr, BRANCH_11);
    }
    CHECKPOINT(marchDecr, BRANCH_2);
    Result = PASS;
DONE:
    return Result;
}

/**************************************************************************//**
* \brief -   	RamTest
*
* \brief - 		Test the range of memory in for direct coupling and stuck at faults.
*
* \param -		START_ADDRESS: Start Address of RAM.
*          		END_ADDRESS: End Address of RAM.
*                      
* \return -  	PASS/FAIL.
*
****************************************************************************/
tSINT32 RamTest(tUINT32 *START_ADDRESS,tUINT32 * END_ADDRESS)
{
    tSINT32 result = PASS;

    FUNCTION1(ramTest, "RamTest()");
    CHECKPOINT(ramTest, BRANCH_1);
    /** MARCH 0 *****************************************/
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    /** MARCH 1 *****************************************/
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    CHECKPOINT(ramTest, BRANCH_2);
    FAULT(ramTestFault, {result = 0;});
    if (result == FAIL)
    {
        CHECKPOINT(ramTest, BRANCH_3);
    }
    else
    {
        CHECKPOINT(ramTest, 4);
        /** MARCH 2 *****************************************/
        result = MarchDecr(START_ADDRESS,END_ADDRESS,~PATTERN, 5, 0);
        CHECKPOINT(ramTest, BRANCH_5);
        FAULT(ramTestFault, {result = 0;});
        if (result == FAIL)
        {
            CHECKPOINT(ramTest, BRANCH_6);
        }
        else
        {
            CHECKPOINT(ramTest, BRANCH_7);
            /** MARCH 3 *****************************************/
            result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
            CHECKPOINT(ramTest, BRANCH_8);
            FAULT(ramTestFault, {result = 0;});
            if (result == FAIL)
            {
                CHECKPOINT(ramTest, BRANCH_9);
            }
            else
            {
                CHECKPOINT(ramTest, BRANCH_10);
                /** MARCH 4 *****************************************/
                result = MarchIncr(START_ADDRESS,END_ADDRESS,~PATTERN, 0, 0);
                CHECKPOINT(ramTest, BRANCH_11);
                FAULT(ramTestFault, {result = 0;});
                if (result == FAIL)
                {
                    CHECKPOINT(ramTest, BRANCH_12);
                }
                else
                {
                    CHECKPOINT(ramTest, BRANCH_13);
                    /** MARCH 5 *****************************************/
                    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 5, 0);
                    CHECKPOINT(ramTest, BRANCH_14);
                    FAULT(ramTestFault, {result = 0;});
                    if (result == FAIL)
                    {
                        CHECKPOINT(ramTest, BRANCH_15);
                    }
                    else
                    {
                        CHECKPOINT(ramTest, BRANCH_16);
                        /** MARCH 6 *****************************************/
                        result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 1);
                        CHECKPOINT(ramTest, BRANCH_17);
                        FAULT(ramTestFault, {result = 0;});
                        if (result == FAIL)
                        {
                            CHECKPOINT(ramTest, BRANCH_18);
                        }
                        else
                        {
                            CHECKPOINT(ramTest, BRANCH_19);
                            /** MARCH 7 *****************************************/
                            result = MarchDecr(START_ADDRESS,END_ADDRESS,~PATTERN, 0, 0);
                            CHECKPOINT(ramTest, BRANCH_20);
                            FAULT(ramTestFault, {result = 0;});
                            if (result == FAIL)
                            {
                                CHECKPOINT(ramTest, BRANCH_21);
                            }
                            else
                            {
                                CHECKPOINT(ramTest, BRANCH_22);
                                /** MARCH 8 *****************************************/
                                result = MarchIncr(START_ADDRESS,END_ADDRESS,~PATTERN, 0, 1);
                                CHECKPOINT(ramTest, BRANCH_23);
                                FAULT(ramTestFault, {result = 0;});
                                if (result == FAIL)
                                {
                                    CHECKPOINT(ramTest, BRANCH_24);
                                }
                                else
                                {
                                    CHECKPOINT(ramTest, BRANCH_25);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

/**************************************************************************//**
* \brief -  	PCDummy1
*
* \brief -  	Dummy function used for Program counter test.
*
* \param -		none
* 
* \return -		Function1_Return.
*
****************************************************************************/
tSINT32 PCDummy1(void)
{
    return Function1_Return;
}

#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for RamTest test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    tSINT32 result = 1;
    tSINT32 test;
    tUINT32 * START_ADDRESS = 0;
    tUINT32 * END_ADDRESS = 0;
    DiagInit();
    marchFill = 1;
    marchIncr = 1;
    marchDecr = 1;
    marchFillFault = -1;
    marchIncrFault = -1;
    marchDecrFault = -1;
    START_ADDRESS = (tUINT32 *) 0x2007C000;
    END_ADDRESS = (tUINT32 *) 0x2007C010;
    // First, test MarchFill and MarchIncr, expect it to pass
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    DiagPrint(result, 1);
    marchFill = 0;
    SerialOut("\n");
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 1);
    // MarchIncr inverted the pattern, so next MarchIncr will fail
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Next, same for MarchDecr
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 1);
    // MarchDecr inverted the pattern, so next MarchIncr will fail
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Insert a fault in memory fill initialization, expect to fail. Location 1
    // is faulty, so it will be detected at 3rd test because test order is
    // 0 2 1 3.
    marchFillFault = 1;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Same, different location. Will be detected in 2nd test in order 0 2 1 3.
    marchFillFault = 2;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Insert a fault in memory fill initialization, expect to fail. Location 1
    // is faulty, so it will be detected at 4th test because test order is
    // 2 0 3 1.
    marchFillFault = 1;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Same, different location. Will be detected in 1st test in order 2 0 3 1.
    marchFillFault = 2;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Next, passing test for MarchIncr where we read X times and write X times
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 2, 0);
    DiagPrint(result, 1);
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 2);
    DiagPrint(result, 1);
    // Same, MarchDecr
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 2, 0);
    DiagPrint(result, 1);
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 2);
    DiagPrint(result, 1);
    // More testing for MarchIncr, insert a fault around checkpoint 7. Results
    // are same as for MarchFill fault. 3rd check at 6 fails.
    marchIncrFault = 2;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    SerialOut("\n");
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Now insert a fault at checkpoint 10. Read two times, location 0 and 2,
    // then read 2 times location 1, on second pass there will be a fault.
    marchIncrFault = 7;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchIncr(START_ADDRESS,END_ADDRESS,PATTERN, 2, 0);
    DiagPrint(result, 0);
    // Same tests for MarchDecr
    marchDecrFault = 2;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 0, 0);
    DiagPrint(result, 0);
    // Now insert a fault at checkpoint 10. Read two times, location 0 and 2,
    // then read 2 times location 1, on second pass there will be a fault.
    marchDecrFault = 7;
    MarchFill(START_ADDRESS,END_ADDRESS,PATTERN);
    result = MarchDecr(START_ADDRESS,END_ADDRESS,PATTERN, 2, 0);
    DiagPrint(result, 0);
    // With MarchIncr and MarchDecr tested, it's time for the RamTest tests.
    // Test the RAM, assuming MarchIncr and MarchDecr work properly.
    ramTest = 1;
    marchDecr = 0;
    marchIncr = 0;
    marchFill = 0;
    ramTestFault = -1;
    result = RamTest(START_ADDRESS,END_ADDRESS);
    DiagPrint(result, 1);
    // Last but not least, while running full RAM test, inject faults into each
    // MarchIncr/MarchDecr results to verify RamTest error handling.
    for (test = 0; test < 8; test++) {
        ramTestFault = test;
        result = RamTest(START_ADDRESS,END_ADDRESS);
        DiagPrint(result, 0);
    }
    DiagSummary();
    for (;;) {
    }
}

#endif

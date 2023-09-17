// $Header:   D:/databases/VMdb/archives/EN13849/ProgramCounterTest.c_v   1.7   20 Jul 2011 16:58:30   ASharma6  $
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

This is the module for ProgramCounter Test.

Module name: ProgramCounter

Filename:    ProgramCounter.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Test the ProgramCounter register by calling different functions placed
at different memory location with known return values.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/ProgramCounterTest.c_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:58:30   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 11:00:42   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:10:46   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:12:44   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:07:18   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "TestMacros.h"
#include "ProgramCounterTest.h"
#include "LPC17xx.h"
#include "Global.h"

UT_VARIABLE(tSINT32 PCDiagnostics)
UT_VARIABLE(tSINT32 EnableFaultvalue)

/*****************************************************************************
** Function name:       PCDummy3
**
** Descriptions:        Dummy function used for Program counter test.
**
** parameters:          void
** Returned value:      Function3_Return.
**
*****************************************************************************/
tSINT32 PCDummy3 (void)
{
    return Function3_Return;
}

/**************************************************************************//**
* \brief  -	ProgramCounterTest
*
* \brief  -	Program counter test. Calls 5 different located
*        	functions those returns different known values.Compare
*        	return values with expected and retun result.
*
* \param  -	none
* 
* \return  -	result PCOK or PC_FAULT.
*
*****************************************************************************/
tSINT32 ProgramCounterTest(void)
{
    BOOL FunctionFailFlag = FALSE;
    tSINT32 result = PCOK;
    tSINT32 i;
    tSINT32 FunReturnValue;

    FUNCTION1(PCDiagnostics, "ProgramCounterTest()");
    CHECKPOINT(PCDiagnostics, BRANCH_1);

    for(i=1;((i<=Function5_Return) && (FunctionFailFlag == FALSE));i++)
    {
        switch(i)
        {
            case Function1_Return:
                CHECKPOINT(PCDiagnostics, BRANCH_2);
                FunReturnValue = PCDummy1();
                FAULT(EnableFaultvalue,{FunReturnValue = 0;});
                if( FunReturnValue == Function1_Return)
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_3);
                    FunctionFailFlag = FALSE;
                }
                else
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_4);
                    FunctionFailFlag = TRUE;
                    result = PC_FAULT;
                }
            break;

            case Function2_Return:
                CHECKPOINT(PCDiagnostics, BRANCH_5);
                FunReturnValue = PCDummy2();
                FAULT(EnableFaultvalue,{FunReturnValue = 0;});
                if(FunReturnValue == Function2_Return)
                {
                    CHECKPOINT(PCDiagnostics,BRANCH_6);
                    FunctionFailFlag = FALSE;
                }
                else
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_7);
                    FunctionFailFlag = TRUE;
                    result = PC_FAULT;
                }
            break;

            case Function3_Return:
                CHECKPOINT(PCDiagnostics, BRANCH_8);
                FunReturnValue = PCDummy3();
                FAULT(EnableFaultvalue,{FunReturnValue = 0;});
                if(FunReturnValue == Function3_Return)
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_10);
                    FunctionFailFlag = FALSE;
                }
                else
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_9);
                    FunctionFailFlag = TRUE;
                    result = PC_FAULT;
                }
            break;
            case Function4_Return:
                CHECKPOINT(PCDiagnostics, BRANCH_11);
                FunReturnValue = PCDummy4();
                FAULT(EnableFaultvalue,{FunReturnValue = 0;});
                if(FunReturnValue == Function4_Return)
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_13);
                    FunctionFailFlag = FALSE;
                }
                else
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_12);
                    FunctionFailFlag = TRUE;
                    result = PC_FAULT;
                }
            break;
            case Function5_Return:
                CHECKPOINT(PCDiagnostics, BRANCH_14);
                FunReturnValue = PCDummy5();
                FAULT(EnableFaultvalue,{FunReturnValue = 0;});
                if (FunReturnValue == Function5_Return)
                {
                    CHECKPOINT(PCDiagnostics, BRANCH_16);
                    FunctionFailFlag = FALSE;
                }
                else
                {
                    CHECKPOINT(PCDiagnostics,BRANCH_15);
                    FunctionFailFlag = TRUE;
                    result = PC_FAULT;
                }
            break;
        }
    }
    CHECKPOINT(PCDiagnostics, BRANCH_17);
    return result;
}


#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for ProgramCounterTest test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{

    tSINT32 result;
    DiagInit();
    PCDiagnostics = 1;

    EnableFaultvalue = 10;
    result = ProgramCounterTest();
    DiagPrintInt(result, 0);

    //Fail in 1 function call
    EnableFaultvalue = 0;
    result = ProgramCounterTest();
    DiagPrintInt(result, 1);

    //Fail in 2 function call
    EnableFaultvalue = 1;
    result = ProgramCounterTest();
    DiagPrintInt(result, 1);

    //Fail in 3 function call
    EnableFaultvalue = 2;
    result = ProgramCounterTest();
    DiagPrintInt(result, 1);

    //Fail in 4 function call
    EnableFaultvalue = 3;
    result = ProgramCounterTest();
    DiagPrintInt(result, 1);

    //Fail in 5 function call
    EnableFaultvalue = 4;
    result = ProgramCounterTest();
    DiagPrintInt(result, 1);

    // Print Summary
    DiagSummary();
}

#endif

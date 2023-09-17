// $Header:   D:/databases/VMdb/archives/EN13849/Diagnostics.c_v   1.10   09 Apr 2015 17:06:44   ewomack  $
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

This is the module for the Diagnostics messages on serial port.

Module name: Diagnostics

Filename:    Diagnostics.c

--------------------------- TECHNICAL NOTES -------------------------------

This code is used during UNIT Test to send the diagnostics results and messages
on serial port.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Diagnostics.c_v  $
 * 
 *    Rev 1.10   09 Apr 2015 17:06:44   ewomack
 * Enable printf for diagnostic messages.
 * 
 *    Rev 1.9   02 Jan 2015 14:34:42   ewomack
 * Modified Diagnostic Serial Port for non Breadboard hardware
 * 
 *    Rev 1.8   20 Jul 2011 16:56:18   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.7   05 Jul 2011 10:59:18   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.4   22 Jun 2011 18:09:04   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   01 Apr 2011 11:13:04   PDwivedi
 * Fixes after code review.
 * 
 *    Rev 1.2   31 Mar 2011 12:11:00   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:04:18   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "system_LPC17xx.h"
#include "core_cm3.h"
#include "Serial.h"
#include <stdio.h>
#include "Global.h"

static tSINT32 diagTests;
static tSINT32 diagErrors;

extern void CPUInit(void);

/**************************************************************************//**
* \brief -		DiagInit
*
* \brief -		Function is called from unit test code to initialize
* 				the CPU and serial port output.
*
* \param - 		none
* 
* \return -    	none
*
*****************************************************************************/
void DiagInit(void)
{
    CPUInit();
    SerialInit(0);
    PrintfEnabled = TRUE;
    SerialOut("\n\n Diagnostic output initialized\n\n");
    diagTests = 0;
    diagErrors = 0;
}

/**************************************************************************//**
* \brief -       DiagCheckpoint
*
* \brief -        Diagnostics checkpoint output. Function assumes that
* 					checkpoint number is two digits or less for simplicity.
*
* \param -		  number: Number to be sent to Serial output.
* 
* \return -      none
*
*****************************************************************************/
void DiagCheckpoint(tUINT32 number)
{
    if (number == 0)
        SerialOut(" 0");
    else
    {
        char ascii[4];
        tSINT32 i;
        ascii[0] = ' ';
        ascii[3] = 0;
        for (i = 2; i > 0; i--)
        {
            ascii[i] = '0' + number % 10;
            number /= 10;
        }
        i = 0;
        if (ascii[1] == '0')
        {
            i = 1;
            ascii[1] = ' ';
        }
        SerialOut(ascii + i);
    }
}

/**************************************************************************//**
* \brief -       	DiagPrint
*
* \brief -        	Diagnostics result output.
*
* \param -			result: Result of a Unit test.
*					expected: Expected result.
*                      
* \return -      	none
*
*****************************************************************************/
void DiagPrint(tUINT32 result, tUINT32 expected)
{
    SerialOut(result ? " - Passed, " : " - Failed, ");
    SerialOut(result == expected ? "correct\n" : "error\n");
    diagTests++;
    if (result != expected)
        diagErrors++;
}

/**************************************************************************//**
* \brief -		DiagPrintInt
*
* \brief -		Diagnostics result output with Expected and actual result.
*
* \param -		result: Result of a Unit test.
*				expected: Expected result.
*				
* \return -	none
*
*****************************************************************************/
void DiagPrintInt(tUINT32 result, tUINT32 expected)
{
	PrintfEnabled = true;
    if (result == expected)
    {
        printf(" - Passed\n");
    }
    else
    {
        printf(" - Failed(got %ld, expected %ld)\n", result, expected);
    }
    diagTests++;
    if (result != expected)
        diagErrors++;
    PrintfEnabled = false;
}

/**************************************************************************//**
* \brief -		DiagSummary
*
* \brief -		Diagnostics result summary.
*
* \param -		void
* 
* \return -	void.
*
*****************************************************************************/
void DiagSummary(void)
{
	PrintfEnabled = true;
    printf("Diagnostics summary: %ld tests, %ld errors\n\n", diagTests,
            diagErrors);
    PrintfEnabled = false;
}
/**************************************************************************//**
* \brief -		ResetdiagTests
*
* \brief -		Reset Diagnostics test numbers.
*
* \param -		void
* 
* \return -	void.
*
*****************************************************************************/
void ResetdiagTests(void)
{
    diagTests = 0;
}

/**************************************************************************//**
* \brief -		ResetdiagErrors
*
* \brief -		Reset Diagnostics Errors count.
*
* \param -		void
* 
* \return -	void.
*
*****************************************************************************/
void ResetdiagErrors(void)
{
    diagErrors = 0;
}

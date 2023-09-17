// $Header:   D:/databases/VMdb/archives/EN13849/Serial.c_v   1.9   09 Apr 2015 17:13:58   ewomack  $
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

This module is used for configuring Serial port.

Module name: Serial

Filename:    Serial.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for initializing and sending character on serial port.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Serial.c_v  $
 * 
 *    Rev 1.9   09 Apr 2015 17:13:58   ewomack
 * Compiler warning cleanup.
 * 
 *    Rev 1.8   20 Jul 2011 16:59:00   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.7   05 Jul 2011 11:01:08   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.4   22 Jun 2011 18:17:44   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:13:44   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:09:10   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/


#include "Serial.h"
#include "LPC17xx.h"
#include "TestMacros.h"

UT_VARIABLE(tSINT32 SerialInitDiagnostics)

LPC_UART_TypeDef * uart;

/*********************************************************************//**
* \brief -  	SerialInit
*
* \brief -  	This Functions is used to a initialization of serial port.
*
* \param - 	port - port number.
* 
* \return -	none
*
****************************************************************************/
void SerialInit(tSINT32 port)
{
    FUNCTION1(SerialInitDiagnostics, "SerialInit()");
    CHECKPOINT(SerialInitDiagnostics, 1);

    switch (port)
    {
        case 0:
            CHECKPOINT(SerialInitDiagnostics, 2);
            //TXD0 and RXD0 , Function when 01
            LPC_PINCON->PINSEL0 = (LPC_PINCON->PINSEL0 & SERIALP02P03MASK) | SERIALTXD0RXD0_FUN01;
            uart = (LPC_UART_TypeDef *) LPC_UART0;
            ChkFunReturn = 1;
        break;
        case 1:
      	  //TXD1 and RXD1 , Function when 10
            CHECKPOINT(SerialInitDiagnostics, 3);
            LPC_PINCON->PINSEL4 = (LPC_PINCON->PINSEL4 & SERIALP20P21MASK) | SERIALTXD1RXD1_FUN10;
            uart = (LPC_UART_TypeDef *) LPC_UART1;
            ChkFunReturn = 1;
        break;
        default:
            CHECKPOINT(SerialInitDiagnostics, 4);
            ChkFunReturn = 0;
            break;
    }
    uart->LCR = 0x83; // 8-N-1, switch to divisor latch
    // 115200, PCLK = 25MHz
    // DLest = 13.56, DLest = 8, FRest = 1.692, DIVADDVAL = 9, MULVAL = 13
    // error = +0.18%
    uart->FDR = 0xD9;
    uart->DLM = 0;
    uart->DLL = 8;
    uart->LCR = 0x03; // 8-N-1, switch to regular operation
}

/*********************************************************************//**
* \brief -  	SerialOutChar
*
* \brief -  	This Functions is used to a character on
* 				Serial port.
*
* \param - 	c - character to be displayed.
* 
* \return -  	none
*
****************************************************************************/
void SerialOutChar(tSINT32 c)
{
    while (!(uart->LSR & 0x20))
        ;
    uart->THR = c;
}

/*********************************************************************//**
* \brief -   	SerialOut
*
* \brief -   	This Functions is used to write character string on
* 				Serial port.
*
* \param -    	string to be displayed.
* 
* \return -  	none
*
****************************************************************************/
void SerialOut(char * string)
{
    while (*string != 0)
    {
        if (*string == '\n')
            SerialOutChar('\r');
        SerialOutChar(*string++);
    }
}

#if UNIT_TESTING

/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for Serial test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    DiagInit();

    
       
    //Not Used, only for prototype board
//    ChkFunReturn = 0;
//    SerialInit(1);
//    DiagPrintInt(ChkFunReturn, 1);

    SerialInitDiagnostics = 1; 
    ChkFunReturn = 0;
    SerialInit(0);
    DiagPrintInt(ChkFunReturn, 1);

    SerialInitDiagnostics  = 0;
    
    //Not Used, only for prototype board
//    SerialInit(1);
//
//    SerialInitDiagnostics = 1;
//    ChkFunReturn = 1;
//    SerialInit(2);
//    DiagPrintInt(ChkFunReturn, 0);
//
//    SerialInitDiagnostics  = 0;
//    SerialInit(1);

    // Print Summary
    DiagSummary();
}

#endif

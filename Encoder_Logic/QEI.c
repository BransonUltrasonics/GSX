// $Header:   D:/databases/VMdb/archives/EN13849/QEI.c_v   1.12   29 Aug 2011 12:11:20   ASharma6  $
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

This is the module for Quadrature linear Encoder.

Module name: QEI

Filename:    QEI.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Handles the Initialization, Interrupt and reading the QEI.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/QEI.c_v  $
 * 
 *    Rev 1.12   29 Aug 2011 12:11:20   ASharma6
 * Unit testing code modified for Part contact and Quadrature encoder modules for the modifications done in the application code for revisions 25 and 26.
 * 
 *    Rev 1.9   20 Jul 2011 16:58:32   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.8   05 Jul 2011 11:00:46   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.5   22 Jun 2011 18:10:50   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:12:56   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:07:36   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "core_cm3.h"
#include "stdio.h"
#include "TestMacros.h"
#include "QEI.h"
#include "DeviceDiagnostics.h"

UT_VARIABLE(tSINT32 QEInitDiag)
UT_VARIABLE(tSINT32 QEIHandler)
UT_VARIABLE(tSINT32 QEIRead)
UT_VARIABLE(tSINT32 QEIHandlerFault)

tSINT32 QEIPhaseErrorCnt;

/**************************************************************************//**
* \brief -		QuadratureEncoderInit
*
* \brief -		Initialize Quadrature Encoder Interface
*
* \param -		maxPos : Maximum Position
* 
* \return -		none
*
*****************************************************************************/
void QuadratureEncoderInit(tSINT32 maxPos)
{
    FUNCTION1(QEInitDiag, "QuadratureEncoderInit()");
    CHECKPOINT(QEInitDiag, BRANCH_1);

    // Enable power to this block
    LPC_SC->PCONP |= (PCONP_PCQEI_MASK);

    // Enable clock to this module
    LPC_SC->PCLKSEL1 &= ~(PCLKSEL1_PCLK_QEI_MASK);
    LPC_SC->PCLKSEL1 |= (PCLKSEL1_PCLK_QEI_FUN1); // CCLK/1

    // Configure the pins now
    LPC_PINCON->PINSEL3 &= ~(PHASEADISABLEMASK); // Phase A - P1.20
    LPC_PINCON->PINSEL3 |= (PINSEL3_MCI0);
    LPC_PINCON->PINSEL3 &= ~(PHASEBDISABLEMASK); // Phase B - P1.23
    LPC_PINCON->PINSEL3 |= (PINSEL3_MCI1);
    LPC_PINCON->PINSEL3 &= ~(INDEXDISABLEMASK); // Index - P1.24
    LPC_PINCON->PINSEL3 |= (PINSEL3_MCI2);
    LPC_PINCON->PINMODE3 &= ~(PHASEADISABLEMASK); // Phase A - internal pull-up
    LPC_PINCON->PINMODE3 |= (ENABLE_PHAHEA_PULLUP);
    LPC_PINCON->PINMODE3 &= ~(PHASEBDISABLEMASK); // Phase B - internal pull-up
    LPC_PINCON->PINMODE3 |= (ENABLE_PHAHEB_PULLUP);
    LPC_PINCON->PINMODE3 &= ~(INDEXDISABLEMASK); // Index - internal pull-up
    LPC_PINCON->PINMODE3 |= (ENABLE_INDEX_PULLUP);

    // Control Register
    LPC_QEI->QEICON = QEICON_RESP_ENABLE; // reset the position counter

    // Configuration Configuration Register
    LPC_QEI->QEICONF = 0 | (QEICONF_DIRINV_ENABLE) // Direction Invert Bit
            // PhA and PhB are quadrature encoder inputs
            | (QEICONF_CAPMODE_ENABLE); // Both PhA & PhB edges are counted (4X)
   

    // Max position register
    LPC_QEI->QEIMAXPOS = maxPos;

    // Digital Filter Sampling Delay
    LPC_QEI->FILTER = 0;

    // Enable the interrupts
    LPC_QEI->QEIIES = 0
            | (QEIIES_ERR_EN_ENABLE); // encoder phase error detect
    

    // Set the priority of the interrupt now
    NVIC_SetPriority(QEI_IRQn, NVIC_EncodePriority(6, 1, 2));
//    CHECKPOINT(QEInitDiag, BRANCH_2);

    // Enable the interrupt now
    NVIC_EnableIRQ(QEI_IRQn);
//    CHECKPOINT(QEInitDiag, BRANCH_3);
}
/**
 *    Resets the position counter to 0
 */
inline void QEI_ResetPosition(void)
{
   LPC_QEI->QEICON = 1 << 0; // reset the position counter
}
/**************************************************************************//**
* \brief -		QEI_GetPosition
*
* \brief -		Reads the Current Encoder Position and update the variable. If the count is negative resets it to 0.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void QEI_GetPosition(void)
{
    FUNCTION1(QEIRead, "QEI_GetPosition()");
    CHECKPOINT(QEIRead, BRANCH_1);
    QEIPosition = LPC_QEI->QEIPOS;
    if(0 > QEIPosition)
    {
    	CHECKPOINT(QEIRead, BRANCH_2);
    	QEI_ResetPosition();
    	QEIPosition = 0;
    }

    ChkFunReturn = 1;//For Diagnostics
    return;
}

/**************************************************************************//**
* \brief -		QEI_IRQHandler
*
* \brief -		QEI interrupt handler
*
* \param -		none
* 
* \return -		none
*
******************************************************************************/
void QEI_IRQHandler(void)
{
    tUINT32 status = LPC_QEI->QEIINTSTAT;

    FAULT(QEIHandlerFault, {status = (1<<0);});
    FAULT(QEIHandlerFault, {status = (1<<3);});
    FAULT(QEIHandlerFault, {status = (1<<4);});
    FUNCTION1(QEIHandler, "QEI_IRQHandler()");

    CHECKPOINT(QEIHandler, BRANCH_1);
    // Change of Direction was detected
    if (status & (QEIINTSTAT_DIR_IntENABLE))
    {
        CHECKPOINT(QEIHandler, BRANCH_2);
        printf("Direction changed \n");
        // Clear the interrupt now
        LPC_QEI->QEICLR |= (QEIINTSTAT_DIR_IntENABLE);
    }
    else
    {
        CHECKPOINT(QEIHandler, BRANCH_3);
    }
    // Encoder phase error was detected
    if (status & (QEIINTSTAT_ERR_IntENABLE))
    {
        CHECKPOINT(QEIHandler, BRANCH_4);
        printf("Phase Error\n");
        QEIPhaseErrorCnt++;
        // Clear the interrupt now
        LPC_QEI->QEICLR |= (QEIINTSTAT_ERR_IntENABLE);
    }
    else
    {
    	QEIPhaseErrorCnt = 0;
    	LEStatus = OK;
        CHECKPOINT(QEIHandler, BRANCH_5);
    }
    
    if(QEIPhaseErrorCnt >= QEIPhaseErrorCount)
    {
    	LEStatus = NOK;
    	 CHECKPOINT(QEIHandler, BRANCH_6);
    }
    else
    {
    	CHECKPOINT(QEIHandler, BRANCH_7);
    }
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for QEI test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
	QEInitDiag = 0;
	QEIHandler = 0;
	QEIRead = 0;

	DiagInit();

	QEInitDiag = 1;
	QuadratureEncoderInit(QEITESTVALUE);
	DiagPrintInt(LPC_QEI->QEIMAXPOS, QEITESTVALUE);
	QEInitDiag = 0;

	DiagSummary();
	ResetdiagTests();
	ResetdiagErrors();

	QEIRead = 1;
	ChkFunReturn = 0;
//	MachineConfiguration = IWPLUS;
//	QEI_GetPosition();
//	DiagPrintInt(ChkFunReturn, 1);
//	MachineConfiguration = NOTIWPLUS;
	QEI_GetPosition();
	DiagPrintInt(ChkFunReturn, 1);
	QEIRead = 0;

	DiagSummary();
	ResetdiagTests();
	ResetdiagErrors();

	QEIHandler = 1;
	QEIHandlerFault = 0;
	QEI_IRQHandler();
	DiagPrintInt((LPC_QEI->QEICLR & (1 << 0)), (1 << 0));
	QEIHandlerFault = 1;
	QEI_IRQHandler();
	DiagPrintInt((LPC_QEI->QEICLR & (1 << 3)), (1 << 3));
	QEIHandlerFault = 2;
	QEI_IRQHandler();
	DiagPrintInt((LPC_QEI->QEICLR & (1 << 4)), (1 << 4));
	QEIHandler = 0;

	DiagSummary();
}

#endif


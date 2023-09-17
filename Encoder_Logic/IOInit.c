// $Header:   D:/databases/VMdb/archives/EN13849/IOInit.c_v   1.11   20 Jul 2011 16:58:08   ASharma6  $
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

This is the module for Initializing the Inputs and Outputs.

Module name: IOInit

Filename:    IOInit.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Initializes the Inputs and Outputs.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/IOInit.c_v  $
 * 
 *    Rev 1.11   20 Jul 2011 16:58:08   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.10   05 Jul 2011 11:00:22   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:10:18   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.2   31 Mar 2011 12:11:58   PDwivedi
 * Fixes after review.
 *
 *    Rev 1.1   25 Mar 2011 09:05:54   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "Global.h"
#include "StateMachine.h"
#include "PBDiagnostics.h"
#include "DeviceDiagnostics.h"
#include "SetOutputs.h"

UT_VARIABLE(tSINT32 InputInit)
UT_VARIABLE(tSINT32 OutputInit)
UT_VARIABLE(tSINT32 InitIO)

/**************************************************************************//**
* \brief -		InitializeInputs
*
* \brief -		Function Initializes all the Input
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void InitializeInputs(void)
{
    FUNCTION1(InputInit, "InitializeInputs()");
    CHECKPOINT(InputInit, BRANCH_1);
    //PIN SELECT configuration for all the inputs
    LPC_PINCON->PINSEL0 = LPC_PINCON->PINSEL0 & FIOPINSEL_P0FUN0_MASK;  //P0:0 U_RESET
    LPC_PINCON->PINSEL0 = LPC_PINCON->PINSEL0 & FIOPINSEL_P1FUN0_MASK;  //P0:1 TRS
    LPC_PINCON->PINSEL0 = LPC_PINCON->PINSEL0 & FIOPINSEL_P4FUN0_MASK;  //P0:4 DRIVE_HIGHFAIL
    LPC_PINCON->PINSEL0 = LPC_PINCON->PINSEL0 & FIOPINSEL_P5FUN0_MASK;  //P0:5 DRIVE_LOWFAIL
    LPC_PINCON->PINSEL0 = LPC_PINCON->PINSEL0 & FIOPINSEL_P10FUN0_MASK; //P0:10 SV1_REQ
    LPC_PINCON->PINSEL1 = LPC_PINCON->PINSEL1 & FIOPINSEL_P21FUN0_MASK; //P0:21 SONICS_HIGHFAIL
    LPC_PINCON->PINSEL1 = LPC_PINCON->PINSEL1 & FIOPINSEL_P22FUN0_MASK; //P0:22 SONICS_LOWFAIL
    LPC_PINCON->PINSEL2 = LPC_PINCON->PINSEL2 & FIOPINSEL_P9FUN0_MASK; // P1:9 SV2_REQ 
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P16FUN0_MASK; //P1:16 SV2_HIGHFAIL 
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P17FUN0_MASK; //P1:17 SV2_LOWFAIL 
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P21FUN0_MASK; //P1:21 ESTOP1
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P22FUN0_MASK; //P1:22 ESTOP2
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P24FUN0_MASK; //P1:24 ULS
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P25FUN0_MASK; //P1:25 PB2_NO_IN
    LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P26FUN0_MASK; //P1:26 PB1_NO_IN
    LPC_PINCON->PINSEL9 = LPC_PINCON->PINSEL9 & FIOPINSEL_P28FUN0_MASK; //P4:28 UFAIL24V
    LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P7FUN0_MASK; //P2:7 PB2-Drv
    LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P8FUN0_MASK;//P2:8 PB1-Drv

    //Data direction setting for all the inputs
    LPC_GPIO0->FIODIR   = LPC_GPIO0->FIODIR & (~(FIODIR_D0_MASK
                                                |FIODIR_D1_MASK
                                                |FIODIR_D4_MASK
                                                |FIODIR_D5_MASK
                                                |FIODIR_D10_MASK
												|FIODIR_D21_MASK
												|FIODIR_D22_MASK
                                                |FIODIR_D28_MASK));

    LPC_GPIO1->FIODIR   = LPC_GPIO1->FIODIR & (~(FIODIR_D19_MASK
                                                |FIODIR_D21_MASK
                                                |FIODIR_D22_MASK
                                                |FIODIR_D24_MASK
                                                |FIODIR_D25_MASK
                                                |FIODIR_D26_MASK
												|FIODIR_D9_MASK 
												|FIODIR_D16_MASK 
												|FIODIR_D17_MASK));

    LPC_GPIO4->FIODIR   = LPC_GPIO4->FIODIR & (~(FIODIR_D28_MASK));
    LEStatus = OK;
    TRSStatus = OK;
    DriveStatus = OK;
    EStopStatus = OK;
    UFAIL24Status = OK;
    SBeamStatus = OK;
    PBStatus = OK;
    SonicsEnStatus = OK;
    BytesToTx = 2;
    ChkFunReturn = 1; // For diagnostics.
}

/**************************************************************************//**
* \brief -		InitializeOutput
*
* \brief -		Function Initializes all the Outputs
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void  InitializeOutput(void)
{
   FUNCTION1(OutputInit, "InitializeOutput()");
   CHECKPOINT(OutputInit, BRANCH_1);
   //PIN SELECT configuration for all the outputs
   LPC_PINCON->PINSEL0 = LPC_PINCON->PINSEL0 & FIOPINSEL_P11FUN0_MASK; //P0:11 BZ
   LPC_PINCON->PINSEL1 = LPC_PINCON->PINSEL1 & FIOPINSEL_P18FUN0_MASK; //P1:18 U_WDO
   LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P27FUN0_MASK; //P1:27 DRIVE
   LPC_PINCON->PINSEL2 = LPC_PINCON->PINSEL2 & FIOPINSEL_P10FUN0_MASK; //P1:10 SV2 
   LPC_PINCON->PINSEL2 = LPC_PINCON->PINSEL2 & FIOPINSEL_P15FUN0_MASK; //P1:15 SONICS_EN 
   LPC_PINCON->PINSEL3 = LPC_PINCON->PINSEL3 & FIOPINSEL_P31FUN0_MASK; //P1:31 PB_OUT
   LPC_PINCON->PINSEL2 = LPC_PINCON->PINSEL2 & FIOPINSEL_P0FUN0_MASK;//P1:0 TP8
   LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P0FUN0_MASK;//P2:0 EN_ALARM_3 
   LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P1FUN0_MASK;//P2:1 EN_ALARM_2 
   LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P2FUN0_MASK;//P2:2 EN_ALARM_1 
   LPC_PINCON->PINSEL4 = LPC_PINCON->PINSEL4 & FIOPINSEL_P4FUN0_MASK;//P2:4 EN_ALARM_0 
   
   
   //Data direction setting for all the outputs
   LPC_GPIO0->FIODIR   = LPC_GPIO0->FIODIR | FIODIR_D11_MASK;
   LPC_GPIO1->FIODIR   = LPC_GPIO1->FIODIR | FIODIR_D18_MASK | FIODIR_D27_MASK | FIODIR_D31_MASK | FIODIR_D0_MASK | FIODIR_D10_MASK | FIODIR_D15_MASK;
   LPC_GPIO2->FIODIR   = LPC_GPIO2->FIODIR | FIODIR_D0_MASK  | FIODIR_D1_MASK  | FIODIR_D2_MASK  | FIODIR_D4_MASK; 
   PINSEL_SetOutput(PINSEL_PORT_1, PINSEL_PIN_31,PB_OUT);      //PB_OUT:Active low
   PB_OUT = PB_DISABLED;//PB are disabled to Weld Controller.
   SetPBs();
   CHECKPOINT(OutputInit, BRANCH_2);
   ChkFunReturn =1 ; //For diagnostics.
}

/**************************************************************************//**
* \brief -		IOInit
*
* \brief -		Function calls the Initialization functions for Input
* 				and output.
*
* \param -		void
* 
* \return -	void.
*
*****************************************************************************/
void IOInit(void)
{
    FUNCTION1(InitIO, "IOInit()");
    CHECKPOINT(InitIO, BRANCH_1);
    InitializeInputs();
    CHECKPOINT(InitIO, BRANCH_2);
    InitializeOutput();
    CHECKPOINT(InitIO, BRANCH_3);
    ChkFunReturn = 1; //For Diagnostics
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for IOInit test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    DiagInit();
    InputInit = 1;
    ChkFunReturn = 0;
    InitializeInputs();
    DiagPrintInt(ChkFunReturn, 1);
    InputInit = 0;
    OutputInit = 1;
    ChkFunReturn = 0;
    InitializeOutput();
    DiagPrintInt(ChkFunReturn, 1);
    OutputInit = 0;
    InitIO = 1;
    ChkFunReturn = 0;
    IOInit();
    DiagPrintInt(ChkFunReturn, 1);
    InitIO = 0;
    DiagSummary();
}

#endif

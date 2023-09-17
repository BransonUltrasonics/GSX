// $Header:   D:/databases/VMdb/archives/EN13849/Input.c_v   1.16   24 Aug 2011 11:24:16   ASharma6  $
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

This is the module for All the Inputs.

Module name: Input

Filename:    Input.c

--------------------------- TECHNICAL NOTES -------------------------------

This code handle all the Digital and Analog Inputs as well as the Debouncing of Digital inputs..


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/Input.c_v  $
 * 
 *    Rev 1.16   24 Aug 2011 11:24:16   ASharma6
 * For IW+ only - if in part contact state, if palm buttons are still pressed then stay in that state and the horn position becomes the new reference for collapse until the hands are released.
 * 
 *    Rev 1.14   20 Jul 2011 16:57:00   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.13   05 Jul 2011 10:59:36   ASharma6
 * ULS, CM changes
Revision 1.45  2011/06/28 17:46:32  satya
Updated for review comments

 * 
 *    Rev 1.10   22 Jun 2011 18:09:26   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.3   31 Mar 2011 12:11:34   PDwivedi
 * Fixes after review.
 *
 *    Rev 1.2   25 Mar 2011 10:51:04   ygupta
 * Modified ReadDigitalInputs function to store current input in TempCurrInputs structure.
 *
 *    Rev 1.1   25 Mar 2011 09:05:20   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "Input.h"
#include "Diagnostics.h"
#include "Global.h"
#include "ADCTest.h"
#include "QEI.h"
#include "EStopDiagnostics.h"
#include "inc/DriveDiagnostics.h"

#define DEBOUNCE_TIME 10
#define OFF 0
#define ON 1

UT_VARIABLE(tSINT32 input)
UT_VARIABLE(tSINT32 digitalInput)
UT_VARIABLE(tSINT32 UpdateInput)
UT_VARIABLE(tSINT32 debounce)
UT_VARIABLE(tSINT32 GetData)
UT_VARIABLE(tSINT32 digitalInputFault)
UT_VARIABLE(tSINT32 UpdateInputFault)
UT_VARIABLE(tSINT32 inputFault)
UT_VARIABLE(tSINT32 MachineCon)
UT_VARIABLE(tSINT32 MachineConFault)
UT_VARIABLE(tSINT32 UpdateULSPositionDiag)

static tUINT32 InputValues[DEBOUNCE_TIME] =
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static tUINT16 DebounceIndex = 0;


//union Input lastInputs;
Input CurrInputs;//Will hold the current debounced input values.
Input TempCurrInputs;//Will hold the current values only.

/**************************************************************************//**
* \brief -		ReadDigitalInputs
*
* \brief -		Read all the digital input value.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void ReadDigitalInputs(void)
{
    FUNCTION1(digitalInput, "ReadDigitalInputs()");
    CHECKPOINT(digitalInput, BRANCH_1);

    TempCurrInputs.InputBits.U2Reset = (LPC_GPIO0->FIOPIN & U2RESET_MASK)?1:0;
    TempCurrInputs.InputBits.TRS = (LPC_GPIO0->FIOPIN & TRS_MASK)?1:0; 

#ifdef PROTOTYPEBOARD
    if(IsThisCPU1)
    {
      CHECKPOINT(digitalInput, BRANCH_2);
      TempCurrInputs.InputBits.DriveFail = (LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK)?1:0;
      TempCurrInputs.InputBits.SV2Fail = (LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK)?1:0; 
      TempCurrInputs.InputBits.SONICSFail = (LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK)?1:0; 
    }
    else
    {
      CHECKPOINT(digitalInput, BRANCH_3);
      TempCurrInputs.InputBits.DriveFail = (LPC_GPIO0->FIOPIN & DRIVELOWFAIL_MASK)?1:0;
      TempCurrInputs.InputBits.SV2Fail = (LPC_GPIO1->FIOPIN & SV2LOWFAIL_MASK)?1:0; 
      TempCurrInputs.InputBits.SONICSFail = (LPC_GPIO0->FIOPIN & SONICSLOWFAIL_MASK)?1:0; 
    }
#else
    CHECKPOINT(digitalInput, BRANCH_3);
    TempCurrInputs.InputBits.DriveFail = (LPC_GPIO0->FIOPIN & DRIVEHIGHFAIL_MASK)?1:0;
    TempCurrInputs.InputBits.SV2Fail = (LPC_GPIO1->FIOPIN & SV2HIGHFAIL_MASK)?1:0; 
    TempCurrInputs.InputBits.SONICSFail = (LPC_GPIO0->FIOPIN & SONICSHIGHFAIL_MASK)?1:0;
#endif

    if (ADC0IntDone == 1)
    {
       CHECKPOINT(digitalInput, BRANCH_4);
       ADC0IntDone = 0;
       EnableReadADCInterrupt(0);
       CHECKPOINT(digitalInput, BRANCH_6);
    }
    else
    {
       CHECKPOINT(digitalInput, BRANCH_5);
    }
    TempCurrInputs.InputBits.SV1Req = (LPC_GPIO0->FIOPIN & SV1REQ_MASK)?1:0;
    TempCurrInputs.InputBits.SV2Req = (LPC_GPIO1->FIOPIN & SV2REQ_MASK)?1:0; 
    TempCurrInputs.InputBits.EStop1 = (LPC_GPIO1->FIOPIN & ESTOP1_MASK)?1:0;
    TempCurrInputs.InputBits.EStop2 = (LPC_GPIO1->FIOPIN & ESTOP2_MASK)?1:0;
    TempCurrInputs.InputBits.ULS = (LPC_GPIO1->FIOPIN & ULS_MASK)?1:0;
    TempCurrInputs.InputBits.PB2_NO = PB2_NO_INPUT;
    TempCurrInputs.InputBits.PB1_NO = PB1_NO_INPUT;
    TempCurrInputs.InputBits.UFail24V = (LPC_GPIO4->FIOPIN & UFAIL24_MASK)?1:0;
    CHECKPOINT(digitalInput, BRANCH_7);
    CheckMachineConfiguration();
    CHECKPOINT(digitalInput, BRANCH_8);
    InputValues[DebounceIndex++] = TempCurrInputs.AllInputs;
    DebounceIndex %= DEBOUNCE_TIME;

    FAULT(digitalInputFault, {CurrInputs.AllInputs = 0x0F;});
}
/**************************************************************************//**
* \brief -		DoDebouncing
*
* \brief -		Debouncing the digital input value.
*
* \param -		void
* 
* \return -	void
*
*****************************************************************************/
void DoDebouncing(void)
{
    FUNCTION1(debounce, "DoDebouncing()");
    CHECKPOINT(debounce, BRANCH_1);
    tUINT32 ANDMask;
    tUINT32 ORMask;
    tUINT32 Index;
    tUINT32 Result;

    /* Now do debouncing */
    // Initialize the masks
    ANDMask = 0xFFFFFFFF;
    ORMask  = 0x00000000;

    // Initialize with data from last time around
    Result = CurrInputs.AllInputs;
    for (Index = 0; Index < DEBOUNCE_TIME; Index++)
    {
        CHECKPOINT(debounce, BRANCH_2);
        ANDMask &= InputValues[Index];
        ORMask  |= InputValues[Index];
    }
    CHECKPOINT(debounce, BRANCH_3);

    Result &= ORMask;
    Result |= ANDMask;
    CurrInputs.AllInputs = Result;
}
/**************************************************************************//**
* \brief -		CheckMachineConfiguration
*
* \brief -		To check what kind of machine is it. GSX-E/P or GSXI.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void CheckMachineConfiguration(void)
{
   FUNCTION1(MachineCon, "CheckMachineConfiguration()");
   CHECKPOINT(MachineCon, BRANCH_1);
   FAULT(MachineConFault, {ADC0Value[1] = 100;});
   FAULT(MachineConFault, {ADC0Value[1] = 0;});
   
   if ((ADC0Value[1] > ADC_0VOLT_RANGE) && (ADC0Value[0] > ADC_0VOLT_RANGE))
   {
      CHECKPOINT(MachineCon, BRANCH_2);
      MachineConfiguration = GSXE; //Servo
   }
   else
   {
      CHECKPOINT(MachineCon, BRANCH_3);
      if((ADC0Value[1] < ADC_0VOLT_RANGE) && (ADC0Value[0] < ADC_0VOLT_RANGE))
      {
    	  CHECKPOINT(MachineCon, BRANCH_4);
    	  MachineConfiguration = GSXP; //Pneumatic
      }
      else
      {
    	  CHECKPOINT(MachineCon, BRANCH_5);
    	  if((ADC0Value[1] > ADC_0VOLT_RANGE) && (ADC0Value[0] < ADC_0VOLT_RANGE))
    	  {
    		  CHECKPOINT(MachineCon, BRANCH_6);
    		  MachineConfiguration = GSXI; //IW
    	  }
      }
   }
}
/**************************************************************************//**
* \brief - 		UpdateULSPosition
*
* \brief -		This function is called to update the value of the ULS
*           	depending on the Machine Configuration
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void UpdateULSPosition(void)
{
   FUNCTION1(UpdateULSPositionDiag, "UpdateULSPosition()");
   CHECKPOINT(UpdateULSPositionDiag, BRANCH_1);
   if((MachineConfiguration == GSXE)||(MachineConfiguration == GSXP))
   {
      CHECKPOINT(UpdateULSPositionDiag, BRANCH_2);
   }
   else
   {
      CHECKPOINT(UpdateULSPositionDiag, BRANCH_5);
      if(ULS == TRUE)
      {
         CHECKPOINT(UpdateULSPositionDiag, BRANCH_6);
         ULSPosition = HOMEPOSITION;
      }
      else
      {
         CHECKPOINT(UpdateULSPositionDiag, BRANCH_7);
         ULSPosition = AWAYPOSITION;
      }
   }
}
/**************************************************************************//**
* \brief -		UpdateInputGlobalVars
*
* \brief -		Function Updates globals variables according to the current input value.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void UpdateInputGlobalVars(void)
{
    FUNCTION1(UpdateInput, "UpdateInputGlobalVars()");
    CHECKPOINT(UpdateInput, BRANCH_1);

    //U2RESET = !CurrInputs.InputBits.U2Reset;    //Active low
    //TRS = CurrInputs.InputBits.TRS;
    TRS = (LPC_GPIO0->FIOPIN & TRS_MASK)?1:0;
    DRIVE_FAIL = !CurrInputs.InputBits.DriveFail;     //Active low
    SV2_FAIL = !CurrInputs.InputBits.SV2Fail;     //Active low 
    SONICS_FAIL = !CurrInputs.InputBits.SONICSFail; //Active low
    
    SV1_REQ = CurrInputs.InputBits.SV1Req;        //Active high 
    SV2_REQ = CurrInputs.InputBits.SV2Req;        //Active high 

    ESTOP1 = CurrInputs.InputBits.EStop1;
    ESTOP2 = CurrInputs.InputBits.EStop2;
    ULS = CurrInputs.InputBits.ULS;
    PB2_NO_IN = CurrInputs.InputBits.PB2_NO;
    PB1_NO_IN = CurrInputs.InputBits.PB1_NO;
    UFAIL24V = CurrInputs.InputBits.UFail24V;

    CHECKPOINT(UpdateInput, BRANCH_2);
    //  Read and store current Linear Encoder reading
    QEI_GetPosition();
    CHECKPOINT(UpdateInput, BRANCH_3);
    FAULT(UpdateInputFault, {ESTOP1 = ESTOP1PRESSEDCPU1; ESTOP2 = ESTOP2PRESSEDCPU1;});
    FAULT(UpdateInputFault, {ESTOP1 = ESTOP1NOTPRESSEDCPU1; ESTOP2 = ESTOP2NOTPRESSEDCPU1;});
    FAULT(UpdateInputFault, {ESTOP1 = ESTOP1PRESSEDCPU2; ESTOP2 = ESTOP2PRESSEDCPU2;});
    FAULT(UpdateInputFault, {ESTOP1 = ESTOP1NOTPRESSEDCPU2; ESTOP2 = ESTOP2NOTPRESSEDCPU2;});
    if(IsThisCPU1)
    {
       CHECKPOINT(UpdateInput, BRANCH_4);
       if ((ESTOP1 == ESTOP1PRESSEDCPU1) && (ESTOP2 == ESTOP2PRESSEDCPU1)) // Both are pressed for CPU1
       {
          CHECKPOINT(UpdateInput, BRANCH_5);
          EStop = true;
       }
       else if((ESTOP1 == ESTOP1NOTPRESSEDCPU1) && (ESTOP2 == ESTOP2NOTPRESSEDCPU1))
       {
          CHECKPOINT(UpdateInput, BRANCH_6);
          EStop = false;
       }
       else
       {
          CHECKPOINT(UpdateInput, BRANCH_7);
       }
    }
    else
    {
       CHECKPOINT(UpdateInput, BRANCH_8);
       if ((ESTOP1 == ESTOP1PRESSEDCPU2) && (ESTOP2 == ESTOP2PRESSEDCPU2)) // Both are pressed for CPU2
       {
          CHECKPOINT(UpdateInput, BRANCH_9);
          EStop = true;
       }
       else if((ESTOP1 == ESTOP1NOTPRESSEDCPU2) && (ESTOP2 == ESTOP2NOTPRESSEDCPU2))
       {
          CHECKPOINT(UpdateInput, BRANCH_10);
          EStop = false;
       }
       else
       {
          CHECKPOINT(UpdateInput, BRANCH_11);
       }
    }
    // Check Machine Configuration Type
    CHECKPOINT(UpdateInput, BRANCH_12);
    UpdateULSPosition();
    CHECKPOINT(UpdateInput, BRANCH_13);
}
/**************************************************************************//**
* \brief -		GetInputs
*
* \brief -		Function called every millisecond to read all inputs.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void GetInputs(void)
{
    FUNCTION1(GetData, "GetInputs()");
    CHECKPOINT(GetData, BRANCH_1);
    /* First read all inputs */
    ReadDigitalInputs();
    CHECKPOINT(GetData, BRANCH_2);
    /* Do Debouncing */
    DoDebouncing();
    CHECKPOINT(GetData, BRANCH_3);
    /* Populate the global variables */
    UpdateInputGlobalVars();
    CHECKPOINT(GetData, BRANCH_4);
    ChkFunReturn = 1;
}

/**************************************************************************//**
* \brief -		InitInputs
*
* \brief -		Function called at first time after Input initialized so that
*  				InputValues buffer can be filled up by valid values.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void InitInputs(void)
{
    tSINT32 Index;
    FUNCTION1(input, "InitInputs()");
    CHECKPOINT(input, BRANCH_1);
    ReadDigitalInputs();
    ADC0IntDone = 1;
    CHECKPOINT(input, BRANCH_2);
    for (Index = 0; Index < DEBOUNCE_TIME; Index++)
    {
        CHECKPOINT(input, BRANCH_3);
        InputValues[Index] = TempCurrInputs.AllInputs;
    }
    CHECKPOINT(input, BRANCH_4);
    UpdateInputGlobalVars();
    CHECKPOINT(input, BRANCH_5);
    FAULT(inputFault, {CurrInputs.AllInputs = 0xF;});
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for Inputs test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    tSINT32 index = 0;
    DiagInit();

    input = 0;
    digitalInput = 0;
    UpdateInput = 0;
    MachineCon = 0;
    debounce = 0;
    GetData = 0;
    /* Configuration for ADC :
     *  Frequency at 1Mhz
     */
    ADCInit(ADC_CLK);
    QuadratureEncoderInit(QEITESTVALUE);
    //Test DoDebouncing()
    debounce = 1;
    for (index = 0; index < 10; index++) {
        InputValues[index] = 0x0E;
    }
    DoDebouncing();
    DiagPrintInt(CurrInputs.AllInputs, 0x0E);
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    //Test ReadDigitalInputs()
   debounce = 0;
   digitalInput = 1;
   digitalInputFault = OK;
   IsThisCPU1 = TRUE;
   ReadDigitalInputs();
   DiagPrintInt(CurrInputs.AllInputs, 0x0F);

   digitalInputFault = OK;
   ADC0IntDone = 1;
   IsThisCPU1 = FALSE;
   ReadDigitalInputs();
   DiagPrintInt(CurrInputs.AllInputs, 0x0F);

   digitalInput = 0;
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();

    //CheckMachineConfiguration()
    MachineCon = 1;
    MachineConFault = 1;
    CheckMachineConfiguration();
    DiagPrintInt(MachineConfiguration, GSXP);
    MachineConFault = 0;
    CheckMachineConfiguration();
    DiagPrintInt(MachineConfiguration, GSXI);
    MachineConFault = 0;
    ADC0Value[0] = 100;
    CheckMachineConfiguration();
    DiagPrintInt(MachineConfiguration, GSXE);	
    MachineCon = 0;
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();
    //UpdateULSPosition()
    UpdateULSPositionDiag = 1;
    MachineConfiguration = GSXI;
    ULS = FALSE;
    UpdateULSPosition();
    DiagPrintInt(ULSPosition, AWAYPOSITION);
    ULS = TRUE;
    UpdateULSPosition();
    DiagPrintInt(ULSPosition, HOMEPOSITION);
    MachineConfiguration = GSXE;
    UpdateULSPosition();
    DiagPrintInt(ULSPosition, HOMEPOSITION);
    ULS = FALSE;
    UpdateULSPosition();
    DiagPrintInt(ULSPosition, HOMEPOSITION);

    UpdateULSPositionDiag = 0;
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    //Test UpdateInputGlobalVars()
    IsThisCPU1 = TRUE;
    UpdateInput = 1;
    UpdateInputFault = -1;
    UpdateInputGlobalVars();
    DiagPrintInt(EStop, false);
    UpdateInputFault = 0;
    UpdateInputGlobalVars();
    DiagPrintInt(EStop, true);
    UpdateInputFault = 1;
    UpdateInputGlobalVars();
    DiagPrintInt(EStop, false);
    IsThisCPU1 = FALSE;
    UpdateInput = 1;
    UpdateInputFault = -1;
    UpdateInputGlobalVars();
    DiagPrintInt(EStop, false);
    UpdateInputFault = 2;
    UpdateInputGlobalVars();
    DiagPrintInt(EStop, true);
    UpdateInputFault = 3;
    UpdateInputGlobalVars();
    DiagPrintInt(EStop, false);
    UpdateInput = 0;
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    //Test InitInputs()
    input = 1;
    inputFault = OK;
    InitInputs();
    DiagPrintInt(CurrInputs.AllInputs, 0x0F);
    input = 0;
    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    //Test GetInput()
    GetData = 1;
    GetInputs();
    GetData = 0;
    DiagPrintInt(ChkFunReturn, 1);
    DiagSummary();

}
#endif

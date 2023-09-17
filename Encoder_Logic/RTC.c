// $Header:   D:/databases/VMdb/archives/EN13849/RTC.c_v   1.9   20 Jul 2011 16:58:50   ASharma6  $
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

This is the module for RTC.

Module name: RTC

Filename:    RTC.c

--------------------------- TECHNICAL NOTES -------------------------------

This code Handles the CPU RTC.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/RTC.c_v  $
 * 
 *    Rev 1.9   20 Jul 2011 16:58:50   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.8   05 Jul 2011 11:01:02   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.5   22 Jun 2011 18:11:08   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:13:30   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:08:38   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "RTC.h"
#include "lpc17xx_clkpwr.h"
#include "SSP.h"
#include "Diagnostics.h"
#include "TestMacros.h"
#include "Global.h"
#include "lpc17xx_libcfg_default.h"

void (*RTC_Callback)(void) = 0;
void (*SysTick_Callback)(void) = 0;

UT_VARIABLE(tSINT32 RTCConfigureDiagnostic)
UT_VARIABLE(tSINT32 RTCSettimeDiagnostic)
UT_VARIABLE(tSINT32 RTCDiagnostic)
UT_VARIABLE(tSINT32 RTCModuleDiagnostic)

/**************************************************************************//**
* \brief -		RTCModule
*
* \brief -		FUnction Initializes and configure the RTC module.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void RTCModule(void)
{
    FUNCTION1(RTCModuleDiagnostic, "RTCModule()");
    CHECKPOINT(RTCModuleDiagnostic, BRANCH_1);

    RTC_Init(LPC_RTC);
    CHECKPOINT(RTCModuleDiagnostic, BRANCH_2);
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
    CHECKPOINT(RTCModuleDiagnostic, BRANCH_3);
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_SECOND, 1);
    CHECKPOINT(RTCModuleDiagnostic, BRANCH_4);
}

/********************************************************************//**
* \brief -		Initializes the RTC peripheral.
* 
* \param -		RTCx RTC peripheral selected, should be RTC
* 
* \return -		none
*********************************************************************/
void RTC_Init(LPC_RTC_TypeDef *RTCx)
{
    CHECK_PARAM(PARAM_RTCx(RTCx));

    /* Set up clock and power for RTC module */
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCRTC, ENABLE);

    FUNCTION1(RTCDiagnostic, "RTC_Init()");
    CHECKPOINT(RTCDiagnostic, BRANCH_1);

    // Clear all register to be default
    RTCx->ILR = RTC_ILR_BITMASK;
    RTCx->CCR = RTC_CCR_CLKEN;
    RTCx->CIIR = 0;
    RTCx->AMR = RTC_AMR_BITMASK;
    RTCx->CALIBRATION = 0x00;
    NVIC_EnableIRQ(RTC_IRQn);
}

/*********************************************************************//**
* \brief - 		Enable/Disable Counter increment interrupt for each time type
*              	in RTC peripheral
*              
* \param -   	RTCx RTC peripheral selected, should be RTC
* 
* \param -   	Interrupt_Type: Counter Increment Interrupt type,
*              	an increment of this type value below will generates
*              	an interrupt, should be:
*              	- RTC_TIMETYPE_SECOND
*              	- RTC_TIMETYPE_MINUTE
*              	- RTC_TIMETYPE_HOUR
*              	- RTC_TIMETYPE_DAYOFWEEK
*              	- RTC_TIMETYPE_DAYOFMONTH
*              	- RTC_TIMETYPE_DAYOFYEAR
*              	- RTC_TIMETYPE_MONTH
*              	- RTC_TIMETYPE_YEAR
*              
* \param -   	NewState New State of this function, should be:
*              	- ENABLE: Counter Increment interrupt for this
*            	time type are enabled
*              	- DISABLE: Counter Increment interrupt for this
*           	time type are disabled
*                  
* \return -  	none
* 
**********************************************************************/
void RTC_Configure(LPC_RTC_TypeDef *RTCx, tUINT32 Interrupt_Type,
        FunctionalState NewState)
{
    CHECK_PARAM(PARAM_RTCx(RTCx));
    CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
    CHECK_PARAM(PARAM_RTC_TIMETYPE(Interrupt_Type));
    FUNCTION1(RTCConfigureDiagnostic, "RTC_Configure()");
    CHECKPOINT(RTCConfigureDiagnostic, BRANCH_1);

    switch (Interrupt_Type)
    {
        case RTC_TIMETYPE_SECOND:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_2);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMSEC;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_11);
                ChkFunReturn =1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMSEC) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_12);
                ChkFunReturn = 1;
            }

            break;
        case RTC_TIMETYPE_MINUTE:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_3);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMMIN;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_13);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMMIN) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_14);
                ChkFunReturn =1;
            }
            break;
        case RTC_TIMETYPE_HOUR:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_4);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMHOUR;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_15);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMHOUR) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_16);
                ChkFunReturn = 1;
            }

            break;
        case RTC_TIMETYPE_DAYOFWEEK:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_5);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMDOW;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_17);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMDOW) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_18);
                ChkFunReturn = 1;
            }

            break;
        case RTC_TIMETYPE_DAYOFMONTH:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_6);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMDOM;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_19);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMDOM) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_20);
                ChkFunReturn = 1;
            }
            break;
        case RTC_TIMETYPE_DAYOFYEAR:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_7);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMDOY;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_21);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMDOY) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_22);
                ChkFunReturn = 1;
            }
            break;
        case RTC_TIMETYPE_MONTH:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_8);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMMON;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_23);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMMON) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_24);
                ChkFunReturn = 1;
            }
            break;
        case RTC_TIMETYPE_YEAR:
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_9);
            if (NewState == ENABLE)
            {
                RTCx->CIIR |= RTC_CIIR_IMYEAR;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_25);
                ChkFunReturn = 1;
            }
            else
            {
                RTCx->CIIR &= (~RTC_CIIR_IMYEAR) & RTC_CIIR_BITMASK;
                CHECKPOINT(RTCConfigureDiagnostic, BRANCH_26);
                ChkFunReturn = 1;
            }
            break;
        default :
            CHECKPOINT(RTCConfigureDiagnostic, BRANCH_10);
            ChkFunReturn = 0;
            break;
    }
}

/*********************************************************************//**
* \brief - 	Set current time value for each time type in RTC peripheral
* 
* \param -   	RTCx RTC peripheral selected, should be RTC
* 
* \param -   	Timetype: Time Type, should be:
*              - RTC_TIMETYPE_SECOND
*              - RTC_TIMETYPE_MINUTE
*              - RTC_TIMETYPE_HOUR
*              - RTC_TIMETYPE_DAYOFWEEK
*              - RTC_TIMETYPE_DAYOFMONTH
*              - RTC_TIMETYPE_DAYOFYEAR
*              - RTC_TIMETYPE_MONTH
*              - RTC_TIMETYPE_YEAR
*              
* \param -   	TimeValue Time value to set
* 
* \return - 	none
* 
**********************************************************************/
void RTC_SetTime(LPC_RTC_TypeDef *RTCx, tUINT32 Timetype, tUINT32 TimeValue)
{
    CHECK_PARAM(PARAM_RTCx(RTCx));
    CHECK_PARAM(PARAM_RTC_TIMETYPE(Timetype));
    FUNCTION1(RTCSettimeDiagnostic, "RTC_SetTime()");
    CHECKPOINT(RTCSettimeDiagnostic, BRANCH_1);

    switch (Timetype)
    {
        case RTC_TIMETYPE_SECOND:
            CHECK_PARAM(TimeValue < RTC_SECOND_MAX);
            RTCx->SEC = TimeValue & RTC_SEC_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_2);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_MINUTE:
            CHECK_PARAM(TimeValue < RTC_MINUTE_MAX);
            RTCx->MIN = TimeValue & RTC_MIN_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_3);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_HOUR:
            CHECK_PARAM(TimeValue < RTC_HOUR_MAX);
            RTCx->HOUR = TimeValue & RTC_HOUR_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_4);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_DAYOFWEEK:
            CHECK_PARAM(TimeValue < RTC_DAYOFWEEK_MAX);
            RTCx->DOW = TimeValue & RTC_DOW_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_5);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_DAYOFMONTH:
            CHECK_PARAM((TimeValue < RTC_DAYOFMONTH_MAX)
                    && (TimeValue > RTC_DAYOFMONTH_MIN));
            RTCx->DOM = TimeValue & RTC_DOM_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_6);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_DAYOFYEAR:
            CHECK_PARAM((TimeValue > RTC_DAYOFYEAR_MIN)
                    && (TimeValue < RTC_DAYOFYEAR_MAX));
            RTCx->DOY = TimeValue & RTC_DOY_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_7);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_MONTH:
            CHECK_PARAM((TimeValue > RTC_MONTH_MIN)
                    && (TimeValue < RTC_MONTH_MAX));
            RTCx->MONTH = TimeValue & RTC_MONTH_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_8);
            ChkFunReturn = 1;
            break;

        case RTC_TIMETYPE_YEAR:
            CHECK_PARAM(TimeValue < RTC_YEAR_MAX);
            RTCx->YEAR = TimeValue & RTC_YEAR_MASK;
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_9);
            ChkFunReturn = 1;
            break;
        default:
            CHECKPOINT(RTCSettimeDiagnostic, BRANCH_10);
            ChkFunReturn = 0;
    }
}

/*********************************************************************//**
* \brief -		RTC_IRQHandler
*
* \brief -		This is RTC Interrupt handler.
*
* \param -  	none
* 
* \return - 	none
*
****************************************************************************/
void RTC_IRQHandler(void)
{
    LPC_RTC->ILR = RTC_IRL_RTCCIF;
    if (RTC_Callback)
    {
        RTC_Callback();
        MsInterruptCounter = 0;
    }
}

/*********************************************************************//**
* \brief -  	SysTick Interrupt handler
*
* \brief -  	This is SysTick Interrupt handler.
*
* \param -  	none
* 
* \return -		none
*
****************************************************************************/
void SysTick_Handler(void)
{
    if (SysTick_Callback)
        SysTick_Callback();

}

/**
 * @}
 */
#if UNIT_TESTING

#include "Serial.h"
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for RTC test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    DiagInit();

    RTCDiagnostic =  1;
    ChkFunReturn = 0;
    RTC_Init(LPC_RTC);
    DiagPrintInt(1, 1);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    RTCDiagnostic = 0;
    RTCConfigureDiagnostic = 1;

    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_MINUTE, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_MINUTE, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_HOUR, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_HOUR, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_DAYOFWEEK, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_DAYOFWEEK, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_DAYOFYEAR, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_DAYOFYEAR, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_MONTH, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_MONTH, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_YEAR, ENABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_Configure(LPC_RTC, RTC_TIMETYPE_YEAR, DISABLE);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 1;
    RTC_Configure(LPC_RTC, 100, ENABLE);
    DiagPrintInt(ChkFunReturn, 0);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    RTCConfigureDiagnostic = 0;
    RTCSettimeDiagnostic = 1;

    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_SECOND, 1);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MINUTE, 1);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_HOUR, 1);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_DAYOFWEEK, 1);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 2);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_DAYOFYEAR, 3);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MONTH, 2);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 0;
    RTC_SetTime(LPC_RTC, RTC_TIMETYPE_YEAR, 1);
    DiagPrintInt(ChkFunReturn, 1);
    ChkFunReturn = 1;
    RTC_SetTime(LPC_RTC, 100, 1);
    DiagPrintInt(ChkFunReturn, 0);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();

    RTCSettimeDiagnostic = 0;
    RTCModuleDiagnostic = 1;
	 RTCModule();
	 DiagPrintInt(1, 1);

	 DiagSummary();

	 RTCModuleDiagnostic = 0;
}

#endif

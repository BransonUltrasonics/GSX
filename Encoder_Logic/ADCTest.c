// $Header:   D:/databases/VMdb/archives/EN13849/ADCTest.c_v   1.14   09 Apr 2015 17:05:40   ewomack  $
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

This is the module for the ADC Handling.

Module name: ADCTest

Filename:    ADCTest.c

--------------------------- TECHNICAL NOTES -------------------------------

This code modifies the values of the registers that are related to the
ADC of LPC 1786 microprocessor also Initializes, Read and test the ADC.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/ADCTest.c_v  $
 * 
 *    Rev 1.14   09 Apr 2015 17:05:40   ewomack
 * Added filtering to ADC Channel 2 to resolve noise issues.
 * 
 *    Rev 1.13   20 Jul 2011 16:55:52   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.12   05 Jul 2011 10:59:04   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.9   22 Jun 2011 18:08:24   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   01 Apr 2011 11:12:24   PDwivedi
 * Fixes after code review.
 * 
 *    Rev 1.2   31 Mar 2011 12:10:50   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 06:56:22   ygupta
 * Initial Version.Added File and Function Headers.
-------------------------------------------------------------------------*/

#include "lpc17xx.h"
#include "ADCTest.h"
#include "TestMacros.h"
#include "Global.h"
#include "PartContact.h"
#include "DeviceDiagnostics.h"
#include "Input.h"
#include "StateMachine.h"

UT_VARIABLE(tSINT32 ADCInitDiag)
UT_VARIABLE(tSINT32 EnableReadADCIntDiag)
UT_VARIABLE(tSINT32 ADC_TestDiag)
UT_VARIABLE(tSINT32 ADC_IRQHandlerDiag)
UT_VARIABLE(tSINT32 ADC_HandlerFault)
UT_VARIABLE(tSINT32 ThreePtFilterDiag)

#define ADC_2_5VOLT_MINRANGE 3000//2400
#define ADC_2_5VOLT_MAXRANGE 3200//3500

#define ADC_FORCE_MIN 39 // 150lb tension
#define ADC_FORCE_MAX 4095 //1500lb compression

tUINT32 ADC0Value[ADC_NUM];
tSINT32 ADCErrorCnt = 0;
tUINT16 FOBuff[500], FOCount = 0, FFull = 0;
tUINT32 FSum = 0;

extern tSINT32 forceData[MAX_FORCE_SAMPLE];
extern tSINT32 ULSPosition;
tSINT32 NumBadReadings = 0;
#define Filter
#define Tolerance
//#define Limits
static tUINT32 ADCBuf[MAX_ADCCHANNEL_USED][3], bufIdx = 0;

/*
 test 7
 */

/**************************************************************************//**
* \brief -       ThreePtFilter
*
* \brief -        Remember last three ADC readings and puts the middle
* 						value into ADC0Value array for specified channel.
*
* \param  -          ADC Channel Number
* 
* \return - none
* 
******************************************************************************/
void ThreePtFilter(tUINT8 channelNum)
{
    FUNCTION1(ThreePtFilterDiag, "ThreePtFilter()");
    CHECKPOINT(ThreePtFilterDiag, BRANCH_1);
    ChkFunReturn = 0;

    if (ADCBuf[channelNum][0] > ADCBuf[channelNum][1])
    {
    	CHECKPOINT(ThreePtFilterDiag, BRANCH_2);
    	if (ADCBuf[channelNum][0] < ADCBuf[channelNum][2])
    	{
    		CHECKPOINT(ThreePtFilterDiag, BRANCH_3);
    		ADC0Value[channelNum] = ADCBuf[channelNum][0];
    	}
    	else 
    	{
    		CHECKPOINT(ThreePtFilterDiag, BRANCH_4);
    		if (ADCBuf[channelNum][2] > ADCBuf[channelNum][1])
    		{
    			CHECKPOINT(ThreePtFilterDiag, BRANCH_5);
    			ADC0Value[channelNum] = ADCBuf[channelNum][2];
    		}
    		else
    		{
    			CHECKPOINT(ThreePtFilterDiag, BRANCH_6);
    			ADC0Value[channelNum] = ADCBuf[channelNum][1];
    		}
    	}
    }
    else
    {
    	CHECKPOINT(ThreePtFilterDiag, BRANCH_7);
    	if (ADCBuf[channelNum][0] > ADCBuf[channelNum][2])
    	{
    		CHECKPOINT(ThreePtFilterDiag, BRANCH_8);
    		ADC0Value[channelNum] = ADCBuf[channelNum][0];
    	}
    	else 
    	{
    		CHECKPOINT(ThreePtFilterDiag, BRANCH_9);
    		if (ADCBuf[channelNum][2] < ADCBuf[channelNum][1])
    		{
    			CHECKPOINT(ThreePtFilterDiag, BRANCH_10);
    			ADC0Value[channelNum] = ADCBuf[channelNum][2];
    		}
    		else
    		{
    			CHECKPOINT(ThreePtFilterDiag, BRANCH_11);
    			ADC0Value[channelNum] = ADCBuf[channelNum][1];
    		}
    	}
    }
    ChkFunReturn = 1;
    CHECKPOINT(ThreePtFilterDiag, BRANCH_12);
}

/**************************************************************************//**
* \brief -		ADC_IRQHandler
*
* \brief -		ADC interrupt handler. Read the ADC value and fill the
*                      Global Array forceData for further use.
*
* \param -		none
* 
* \return -    none
*
******************************************************************************/
void  ADC_IRQHandler (void)
{
    tUINT32 regVal;
    static tSINT32 SampleIndex = 0;
    tSINT32 channel = ADC_CH0;
    FUNCTION1(ADC_IRQHandlerDiag, "ADC_IRQHandler()");
    CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_1);
    static tUINT32 retry = 1;

    regVal = LPC_ADC->ADSTAT;       /* Read ADC will clear the interrupt */
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH0 );});//0
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH0 | ADC_ADINT);});//1
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH1 | ADC_ADINT);});//2
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH2 | ADC_ADINT);SampleIndex = 0;ADCBuf[2][bufIdx]=0;FFull=1;ULSPosition=HOMEPOSITION;SV1_REQ=OFF;});//3
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH2 | ADC_ADINT);SampleIndex = 14;ADCBuf[2][bufIdx]=1000;FFull=0;FOCount=500;});//4
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH3 | ADC_ADINT);});//5
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH4 | ADC_ADINT);});//6
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH5 | ADC_ADINT);});//7
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH6 | ADC_ADINT);});//8
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH7 | ADC_ADINT);});//9
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH8 | ADC_ADINT);});//10
    FAULT(ADC_HandlerFault, {regVal = (ADC_CH2 | ADC_ADINT);ADCBuf[2][bufIdx]=1000;FOBuff[FOCount]=1000;});//11

    if ( regVal & ADC_ADINT ) //If ADC interrupt
    {
        CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_3);
        switch ( regVal & 0xFF )    /* check DONE bit */
        {
            case ADC_CH0: // Channel 0 tied to GND.
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_4);
                bufIdx = ++bufIdx % 3;
                ADCBuf[0][bufIdx] = ( LPC_ADC->ADDR0 >> ADC_RESERVED_BITS ) & 0xFFF;

#ifdef Filter
                ThreePtFilter(0);
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_15);
#else
                ADC0Value[0] = ADC0Buf[bufIdx];
#endif
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                LPC_ADC->ADCR &= ADCR_CHSEL_MASK;
#if UNIT_TESTING
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
#else
               	LPC_ADC->ADCR |= (ADCR_START_MASK) | (ADC_CH1); //Enable ADC for next channel.
#endif
                ChkFunReturn = 1;
                channel = ADC_CH1;
            break;
            case ADC_CH1: //Channel 1 Tied to 2.5V ref.
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_5);
                ADCBuf[1][bufIdx] = ( LPC_ADC->ADDR1 >> ADC_RESERVED_BITS ) & 0xFFF;
#ifdef Filter
                ThreePtFilter(1);
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_16);
#else
                ADC0Value[1] = ADC1Buf[bufIdx];
#endif
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                LPC_ADC->ADCR &= ADCR_CHSEL_MASK;
#if UNIT_TESTING
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
#else
                LPC_ADC->ADCR |= (ADCR_START_MASK) | (ADC_CH2); //Enable ADC for next channel.
#endif
                ChkFunReturn = 1;
                channel = ADC_CH2;
            break;
            case ADC_CH2:  // Channel 2 Tied to Force
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_6);
#if UNIT_TESTING == 0
                //Don't set this value from register if unit testing
                ADCBuf[2][bufIdx] = ( LPC_ADC->ADDR2 >> ADC_RESERVED_BITS ) & 0xFFF;
#endif
#ifdef Limits
                if (ADCBuf[2][bufIdx] > ADC_FORCE_MAX || ADCBuf[2][bufIdx] < ADC_FORCE_MIN)
                {
                	CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_17);
                	ADCBuf[2][bufIdx] = ADC0Value[2];
                }
#endif

#ifdef Filter
                ThreePtFilter(2);
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_18);
#else
                ADC0Value[2] = ADC2Buf[bufIdx];
#endif

#ifdef Tolerance
                if (retry == 1)
                {
                	//This code has a bug to be fixed (or not fixed) in the future
                	//The comparison isn't against the previous good value, but it
                	//is against the last value at the home position so if a large
                	//force is being exerted on the part, this code will force
                	//duplicate ADC conversions for no reason.
                	CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_19);
					tUINT32 upper, lower;
					upper = FOBuff[FOCount] * 105;
					upper /= 100;
					lower = FOBuff[FOCount] * 95;
					lower /= 100;
					if ((ADC0Value[2] > upper || ADC0Value[2] < lower))
					{
						CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_20);
						retry = 0;
		                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
#if UNIT_TESTING == 0
						LPC_ADC->ADCR |= (ADCR_START_MASK) | (ADC_CH2);
#endif
						ChkFunReturn = 1;
						break;
					}
                }
#endif
                if (State == READY && ((SV1_REQ == OFF)))
                {
                	CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_21);
					tUINT16 fTemp = 0;
					//Save Current and Last Reading
					fTemp = FOBuff[FOCount];
					FOBuff[FOCount] = ADC0Value[2];
					if (FFull == 1)
					{
						CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_22);
						FSum -= fTemp;
					}
					FSum += FOBuff[FOCount];
					if (++FOCount >= 500)
					{
						CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_23);
						FOCount = 0;
						FFull = 1;
					}
                }

				if(SampleIndex >= MAX_FORCE_SAMPLE)
				{
					CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_8);
					SampleIndex = 0;
					forceData[SampleIndex] = ADC0Value[2];
				}
				else
				{
					CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_7);
					forceData[SampleIndex] = ADC0Value[2];
				}

                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                LPC_ADC->ADCR &= ADCR_CHSEL_MASK;
#if UNIT_TESTING
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
#else
                LPC_ADC->ADCR |= (ADCR_START_MASK) | (ADC_CH3); //Enable ADC for next channel.
#endif
                channel = ADC_CH3;
                
                ChkFunReturn = 1;
				SampleIndex++;
				retry = 1;
            break;
            case ADC_CH3: //Channel 3 Not used
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_9);
#if UNIT_TESTING == 0
                ADCBuf[3][bufIdx] = (LPC_ADC->ADDR3 >> ADC_RESERVED_BITS ) & 0xFFF;
#endif
#ifdef Filter
                ThreePtFilter(3);
#else
                ADC0Value[3] = ADC3Buf[bufIdx];
#endif
                
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                LPC_ADC->ADCR &= ADCR_CHSEL_MASK;
#if UNIT_TESTING
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
#else
                LPC_ADC->ADCR |= (ADCR_START_MASK) | (ADC_CH4); //Enable ADC for next channel.
#endif
                ChkFunReturn = 1;
            break;
            case ADC_CH4:  //Channel 4 Not used
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_10);
#if UNIT_TESTING == 0
                ADCBuf[4][bufIdx] = ( LPC_ADC->ADDR4 >> ADC_RESERVED_BITS ) & 0xFFF;
#endif
#ifdef Filter
                ThreePtFilter(4);
#else
                ADC0Value[4] = ADC4Buf[bufIdx];
#endif
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                
                ADC0IntDone = 1;
                ChkFunReturn = 1;
            break;
            case ADC_CH5:  //Channel 5 Not used
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_11);
                ADC0Value[5] = ( LPC_ADC->ADDR5 >> ADC_RESERVED_BITS ) & 0xFFF;
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                ChkFunReturn = 1;
            break;
            case ADC_CH6:  //Channel 6 Not used
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_12);
                ADC0Value[6] = ( LPC_ADC->ADDR6 >> ADC_RESERVED_BITS ) & 0xFFF;
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                ChkFunReturn = 1;
            break;
            case ADC_CH7: //Channel 7 Not used
                CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_13);
                ADC0Value[7] = ( LPC_ADC->ADDR7 >> ADC_RESERVED_BITS ) & 0xFFF;
                LPC_ADC->ADCR &= ADCR_STOP_MASK;    /* stop ADC now */
                ChkFunReturn = 1;
            break;
            default: //Default case. Should not come here.
            CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_14);
            ChkFunReturn = 0;
            break;
        }
    }
    else
    {
        ChkFunReturn = 1;
        CHECKPOINT(ADC_IRQHandlerDiag, BRANCH_2);
    }
}


/**************************************************************************//**
* \brief -		ADCInit
*
* \brief -		initialize ADC channel
*
* \param -		ADC clock rate
* 
* \return -		none
*
*****************************************************************************/
void ADCInit( tUINT32 ADC_Clk )
{
    tUINT32 pclk;

    FUNCTION1(ADCInitDiag, "ADCInit()");
    CHECKPOINT(ADCInitDiag, BRANCH_1);

    /* Enable CLOCK into ADC controller */
    LPC_SC->PCONP |= PCONP_PCADC_MASK;

    /* all the related pins are set to ADC inputs, AD0.0~7 */
    /* P0.23~25, A0.0~2, function 01 */
    LPC_PINCON->PINSEL1 &= FIOPINSEL_P23FUN0_MASK & FIOPINSEL_P24FUN0_MASK & FIOPINSEL_P25FUN0_MASK & FIOPINSEL_P26FUN0_MASK;
    LPC_PINCON->PINSEL1 |= FIOPINSEL_P23FUN1_MASK | FIOPINSEL_P24FUN1_MASK |FIOPINSEL_P25FUN1_MASK | FIOPINSEL_P26FUN1_MASK;
    
    LPC_PINCON->PINSEL3 &= FIOPINSEL_P30FUN0_MASK;
    LPC_PINCON->PINSEL3 |= FIOPINSEL_P30FUN3_MASK;

    /* By default, the PCLKSELx value is zero, */
    pclk = SystemCoreClock/4;
    pclk = pclk  / ADC_Clk - 1 ;
    LPC_ADC->ADCR = ((( ADC_CH0 ) |     /* SEL=1,select channel 0 on ADC0 */
        ( ( pclk ) << 8 ) |     /* CLKDIV = Fpclk / ADC_Clk - 1 */
        ( ADC_NO_BURST) |       /* BURST = 0, no BURST, software controlled */
        ( ADC_NO_PDN ) |        /* PDN = 1, normal operation */
        ( ADC_RISE_EDGE ))&     /* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */
        ( ADCR_STOP_MASK )) ;   /* START = 0 A/D conversion stops */


    LPC_ADC->ADINTEN = 0x1FF;       /* Enable all interrupts */
    NVIC_EnableIRQ(ADC_IRQn);
    CHECKPOINT(ADCInitDiag, BRANCH_2);
    ChkFunReturn = 1;
}

/**************************************************************************//**
* \brief -		ADC0Read
* 
* \brief -		Read ADC0 channel
*
* \param -		Channel number
* 
* \return -	none
*
*****************************************************************************/
void EnableReadADCInterrupt( tUINT8 channelNum )
{

    FUNCTION1(EnableReadADCIntDiag, "EnableReadADCInterrupt()");
    CHECKPOINT(EnableReadADCIntDiag, BRANCH_1);

    /* channel number is 0 through 7 */
    if ( channelNum >= MAX_ADCCHANNEL_USED - 1 )
    {
        CHECKPOINT(EnableReadADCIntDiag, BRANCH_2);
        channelNum = 0;     /* reset channel number to 0 */
    }
    else
    {
        CHECKPOINT(EnableReadADCIntDiag, BRANCH_3);
    }
    LPC_ADC->ADCR &= ADCR_CHSEL_MASK;
    LPC_ADC->ADCR |= (ADCR_START_MASK) | (1 << channelNum);

    ChkFunReturn = channelNum;
}

/**************************************************************************//**
* \brief -		ADC_Test
*
* \brief -		Read ADC0.3 and ADC0.4 channel and compare the value and
*				See if they are in acceptable limits.ADC0.4 is tied to Ground
*				and ADC0.3 is tied to 2.5Volt.
*
* \param -		none
* 
* \return 		none
*
*****************************************************************************/
void ADC_Test(void)
{
	FUNCTION1(ADC_TestDiag, "ADC_Test()");
	CHECKPOINT(ADC_TestDiag, BRANCH_1);
	ADCStatus = OK;


	if((ADC0Value[3] > ADC_2_5VOLT_MINRANGE)&&(ADC0Value[3] < ADC_2_5VOLT_MAXRANGE)
			&&(ADC0Value[4] < ADC_0VOLT_RANGE))
	{
		ADCErrorCnt = 0;
		CHECKPOINT(ADC_TestDiag, BRANCH_2);
	}
	else
	{
		ADCErrorCnt++;
		CHECKPOINT(ADC_TestDiag, BRANCH_3);
	}
	
	
	if(ADCErrorCnt > ADCErrorCount)
	{
	   CHECKPOINT(ADC_TestDiag, BRANCH_4);
      ADCStatus = NOK;
	}
	else
	{
	   CHECKPOINT(ADC_TestDiag, BRANCH_5);
      ADCStatus = OK;
	}
}
/*********************************************************************************
**                            End Of File
*********************************************************************************/

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for ADC test unit testing.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void ResetHandler(void)
{
	 DiagInit();
    ADCInitDiag = 1;
    tSINT32 channelNum = 0;
    ADC_IRQHandlerDiag = 0;
    /*-------------ADCInit Test-------------*/
    ChkFunReturn = 0;
    ADCInit(ADC_CLK);
    DiagPrintInt(ChkFunReturn, 1);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();
    /*------------------EnableReadADCInterrupt Test---------*/
    EnableReadADCIntDiag = 1;
    ChkFunReturn = 0;
    EnableReadADCInterrupt(  channelNum );
    DiagPrintInt(ChkFunReturn, 0);

    channelNum = TESTCHNUMBERVAL1;
    EnableReadADCInterrupt(  channelNum );
    DiagPrintInt(ChkFunReturn, 1);

    channelNum = TESTCHNUMBERVAL2;
    EnableReadADCInterrupt(  channelNum );
    DiagPrintInt(ChkFunReturn, 0);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();
     /*------------------ADC_IRQHandlerTest---------*/
    ADC_IRQHandlerDiag = 1;
    ADC_HandlerFault = 0;
    ChkFunReturn = 0;
    int ii, jj;
    for (ii = 0; ii < 3; ii++)
    {
    	for (jj = 0; jj < 3; jj++)
    	{
    		ADCBuf[ii][jj] = 1000;
    	}
    }
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);

    ADC_HandlerFault = 1;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 2;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 3;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 4;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 5;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 6;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 7;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 8;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 9;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);
    ADC_HandlerFault = 10;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 0);
    ADC_HandlerFault = 11;
    ADC_IRQHandler();
    DiagPrintInt(ChkFunReturn, 1);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();
  /*------------------ADC_Test---------*/
	ADC_TestDiag = 1;
	ADCErrorCnt = 0;
	ADC0Value[3] = TESTADCTESTVALCH3_0; //  Within Acceptable Range
	ADC0Value[4] = TESTADCTESTVALCH4_0; //  Within Acceptable Range but result is ok because count is only one.
	ADC_Test();
	DiagPrintInt(ADCStatus, OK);

	ADC0Value[3] = TESTADCTESTVALCH3_1; // Not Within Acceptable Range
	ADC0Value[4] = TESTADCTESTVALCH4_1; // Within Acceptable Range but result is ok because count is only one.
	ADC_Test();
	DiagPrintInt(ADCStatus, OK);

	ADC0Value[3] = TESTADCTESTVALCH3_2;//  Within Acceptable Range
	ADC0Value[4] = TESTADCTESTVALCH4_2; // Not Within Acceptable Range but result is ok because count is only one.
	ADC_Test();
	DiagPrintInt(ADCStatus, OK);

	ADCErrorCnt = 0;
    ADCErrorCnt = ADCErrorCount + 1;
	ADC0Value[3] = TESTADCTESTVALCH3_3; // Not Within Acceptable Range
	ADC0Value[4] = TESTADCTESTVALCH4_3; // Not Within Acceptable Range
	ADC_Test();
	DiagPrintInt(ADCStatus, NOK);

   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();
   /*-------------ThreePtFilter-------------------------*/
   ThreePtFilterDiag = 1;
   //Test 1
   ADCBuf[0][0] = 3;
   ADCBuf[0][1] = 2;
   ADCBuf[0][2] = 1;
   ThreePtFilter(0);
   DiagPrintInt(ADC0Value[0], 2);
   //Test 2
   ADCBuf[0][0] = 2;
   ADCBuf[0][1] = 1;
   ADCBuf[0][2] = 3;
   ThreePtFilter(0);
   DiagPrintInt(ADC0Value[0], 2);
   //Test 3
   ADCBuf[0][0] = 3;
   ADCBuf[0][1] = 1;
   ADCBuf[0][2] = 2;
   ThreePtFilter(0);
   DiagPrintInt(ADC0Value[0], 2);
   //Test 4
   ADCBuf[0][0] = 1;
   ADCBuf[0][1] = 2;
   ADCBuf[0][2] = 3;
   ThreePtFilter(0);
   DiagPrintInt(ADC0Value[0], 2);
   //Test 5
   ADCBuf[0][0] = 2;
   ADCBuf[0][1] = 3;
   ADCBuf[0][2] = 1;
   ThreePtFilter(0);
   DiagPrintInt(ADC0Value[0], 2);
   //Test 6
   ADCBuf[0][0] = 1;
   ADCBuf[0][1] = 3;
   ADCBuf[0][2] = 2;
   ThreePtFilter(0);
   DiagPrintInt(ADC0Value[0], 2);
   DiagSummary();
}

#endif

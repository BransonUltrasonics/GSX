// $Header:   D:/databases/VMdb/archives/EN13849/inc/ADCTest.h_v   1.13   09 Apr 2015 17:16:38   ewomack  $
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

This is the module for testing the ADC as part of Logic Unit Testing.

Module name: ADCTest

Filename:    ADCTest.c

--------------------------- TECHNICAL NOTES -------------------------------

This code modifies the values of the registers that are related to the
ADC of LPC 1786 microprocessor.


------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/ADCTest.h_v  $
 * 
 *    Rev 1.13   09 Apr 2015 17:16:38   ewomack
 * Changed unit test conditions.  Cut 0V ADC range in half.
 * 
 *    Rev 1.12   20 Jul 2011 16:46:58   ASharma6
 * Modifications for review comments
 *
 *    Rev 1.11   05 Jul 2011 10:56:22   ASharma6
 * ULS, CM changes
 *
 *    Rev 1.8   22 Jun 2011 18:13:58   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 *
 *    Rev 1.3   01 Apr 2011 11:27:44   PDwivedi
 * Fixex after review.
 *
 *    Rev 1.2   31 Mar 2011 11:56:58   PDwivedi
 * Fixes after review.
 *
 *    Rev 1.1   25 Mar 2011 09:15:32   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef __ADC_H
#define __ADC_H
#include "lpc_types.h"

#define ADC_ADINT    0x00010000

#define ADC_NUM      8        /* for LPCxxxx */
#define ADC_CLK      1000000     /* set to 1Mhz */
#define MAX_ADCCHANNEL_USED   5
#define ADC_CH0      0x01
#define ADC_CH1      0x02
#define ADC_CH2      0x04
#define ADC_CH3      0x08
#define ADC_CH4      0x10
#define ADC_CH5      0x20
#define ADC_CH6      0x40
#define ADC_CH7      0x80
#define ADC_CH8      0x100 // For Module testing only
#define ADC_0VOLT_RANGE 25
#define ADCErrorCount 10
extern tUINT32 ADC0Value[ADC_NUM];

// for Testing
#define TESTCHNUMBERVAL1   1
#define TESTCHNUMBERVAL2   8

#define  TESTADCTESTVALCH0_0   5
#define  TESTADCTESTVALCH1_0   5

#define  TESTADCTESTVALCH0_1   3100
#define  TESTADCTESTVALCH1_1   3100

#define  TESTADCTESTVALCH0_2   3100
#define  TESTADCTESTVALCH1_2   5

#define  TESTADCTESTVALCH0_3   5
#define  TESTADCTESTVALCH1_3   3100

#define TESTADCTESTVALCH3_0	  3100
#define TESTADCTESTVALCH4_0   5

#define TESTADCTESTVALCH3_1	  5
#define TESTADCTESTVALCH4_1   5

#define TESTADCTESTVALCH3_2	  3100
#define TESTADCTESTVALCH4_2	  3100

#define TESTADCTESTVALCH3_3	  5
#define TESTADCTESTVALCH4_3   3100



void ADCInit( tUINT32 ADC_Clk );
void EnableReadADCInterrupt( tUINT8 channelNum );
void ADC_Test(void);
#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

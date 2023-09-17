// $Header:   D:/databases/VMdb/archives/EN13849/SystemInitialization.c_v   1.10   20 Jul 2011 16:59:24   ASharma6  $
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

This is the module for Initialization of CPU and peripherals.

Module name: SystemInitialization

Filename:    SystemInitialization.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for initialization of CPU and system core frequency.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/SystemInitialization.c_v  $
 * 
 *    Rev 1.10   20 Jul 2011 16:59:24   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.9   12 Jul 2011 18:35:16   ASharma6
 * Revision 19 with modifications in Part contact lost algorithm - Loss of force alongwith horn retraction.
 * 
 *    Rev 1.8   07 Jul 2011 12:25:54   ASharma6
 * TRS Diagnostics for IW+ machine configuration is modified
 * 
 *    Rev 1.7   05 Jul 2011 11:01:34   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.4   22 Jun 2011 18:11:46   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.1.1.0   31 Mar 2011 12:14:18   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:11:18   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "SystemInitialization.h"
#include "Global.h"
#include "ADCTest.h"
#include "QEI.h"
#include "PBDiagnostics.h"
#include "SSP.h"

UT_VARIABLE(tSINT32 SysinitDiag)
UT_VARIABLE(tSINT32 PeripherialDiag)
UT_VARIABLE(tSINT32 CPUDiag)
UT_VARIABLE(tSINT32 SyncUpFault)
void MemoryInit(void);
/*----------------------------------------------------------------------------
 Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL        (12000000UL)        /* Oscillator frequency               */
#define OSC_CLK     (      XTAL)        /* Main oscillator frequency          */
#define RTC_CLK     (   32000UL)        /* RTC oscillator frequency           */
#define IRC_OSC     ( 4000000UL)        /* Internal RC oscillator frequency   */

/* F_cco0 = (2 * M * F_in) / N  */
#define __M               (((PLL0CFG_Val      ) & 0x7FFF) + 1)
#define __N               (((PLL0CFG_Val >> 16) & 0x00FF) + 1)
#define __FCCO(__F_IN)    ((2 * __M * __F_IN) / __N)
#define __CCLK_DIV        (((CCLKCFG_Val      ) & 0x00FF) + 1)

/* Determine core clock frequency according to settings */
#if (PLL0_SETUP)
#if   ((CLKSRCSEL_Val & 0x03) == 1)
#define __CORE_CLK (__FCCO(OSC_CLK) / __CCLK_DIV)
#elif ((CLKSRCSEL_Val & 0x03) == 2)
#define __CORE_CLK (__FCCO(RTC_CLK) / __CCLK_DIV)
#else
#define __CORE_CLK (__FCCO(IRC_OSC) / __CCLK_DIV)
#endif
#else
#if   ((CLKSRCSEL_Val & 0x03) == 1)
#define __CORE_CLK (OSC_CLK         / __CCLK_DIV)
#elif ((CLKSRCSEL_Val & 0x03) == 2)
#define __CORE_CLK (RTC_CLK         / __CCLK_DIV)
#else
#define __CORE_CLK (IRC_OSC         / __CCLK_DIV)
#endif
#endif


/*----------------------------------------------------------------------------
 Clock Variable definitions
 *----------------------------------------------------------------------------*/
tUINT32 SystemFrequency = IRC_OSC; /*!< System Clock Frequency (Core Clock)  */

/*----------------------------------------------------------------------------
 Clock Variable definitions
 *----------------------------------------------------------------------------*/
tUINT32 SystemCoreClock = __CORE_CLK;/*!< System Clock Frequency (Core Clock)*/

/*----------------------------------------------------------------------------
 Clock functions
 *----------------------------------------------------------------------------*/

/**************************************************************************//**
* \brief -    	SystemCoreClockUpdate
*
* \brief -     This function will update System Core Clock variable.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void SystemCoreClockUpdate(void)
{
   /* Get Core Clock Frequency      */
   /* Determine clock frequency according to clock register values             */
   if (((LPC_SC->PLL0STAT >> PLLE0STATOfst) & PLLEnCnChkMask) == PLLEnbldNConnctd)
   { /* If PLL0 enabled and connected */
      switch (LPC_SC->CLKSRCSEL & CLKSRCMask)
      {
         case CLKSRCRC: /* Int. RC oscillator => PLL0    */
         case CLKSRRES: /* Reserved, default to Int. RC  */
            SystemCoreClock = (IRC_OSC * ((2
            * ((LPC_SC->PLL0STAT & MSEL0Mask) + 1))) / (((LPC_SC->PLL0STAT
            >> NSEL0Ofst) & NSEL0Mask) + 1) / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1));
         break;
         case CLKSRCOSC: /* Main oscillator => PLL0       */
            SystemCoreClock = (OSC_CLK * ((2
            * ((LPC_SC->PLL0STAT & MSEL0Mask) + 1))) / (((LPC_SC->PLL0STAT
            >> NSEL0Ofst) & NSEL0Mask) + 1) / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1));
         break;
         case CLKSRCRTCOSC: /* RTC oscillator => PLL0        */
            SystemCoreClock = (RTC_CLK * ((2
            * ((LPC_SC->PLL0STAT & MSEL0Mask) + 1))) / (((LPC_SC->PLL0STAT
            >> NSEL0Ofst) & NSEL0Mask) + 1) / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1));
         break;
      }
   }
   else
   {
      switch (LPC_SC->CLKSRCSEL & CLKSRCMask)
      {
         case CLKSRCRC: /* Int. RC oscillator => PLL0    */
         case CLKSRRES: /* Reserved, default to Int. RC  */
            SystemCoreClock = IRC_OSC / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1);
         break;
         case CLKSRCOSC: /* Main oscillator => PLL0       */
            SystemCoreClock = OSC_CLK / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1);
         break;
         case CLKSRCRTCOSC: /* RTC oscillator => PLL0        */
            SystemCoreClock = RTC_CLK / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1);
         break;
      }
   }

}

/**************************************************************************//**
* \brief -		CPUInit
*
* \brief -		Setup the Microcontroller system.Initialize the System
*           	and update the SystemFrequency variable.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void CPUInit(void)
{
   MemoryInit();
#if (CLOCK_SETUP)                       /* Clock Setup                        */
   LPC_SC->SCS = SCS_Val;
   if (SCS_Val & OSCENMask)
   { /* If Main Oscillator is enabled      */
      while ((LPC_SC->SCS & OSCSTATMask) == OSCNotRdy)
         ;/* Wait for Oscillator to be ready    */
   }

   LPC_SC->CCLKCFG = CCLKCFG_Val; /* Setup Clock Divider                */

   LPC_SC->PCLKSEL0 = PCLKSEL0_Val; /* Peripheral Clock Selection         */
   LPC_SC->PCLKSEL1 = PCLKSEL1_Val;

   LPC_SC->CLKSRCSEL = CLKSRCSEL_Val; /* Select Clock Source for PLL0       */

#if (PLL0_SETUP)
   LPC_SC->PLL0CFG = PLL0CFG_Val;
   LPC_SC->PLL0CON = PLLE0Enable; /* PLL0 Enable                        */
   LPC_SC->PLL0FEED = PLL0FEEDVal1;
   LPC_SC->PLL0FEED = PLL0FEEDVal2;
   while (!(LPC_SC->PLL0STAT & PLOCK0Mask))
      ;/* Wait for PLOCK0                    */

   LPC_SC->PLL0CON = (PLLE0Enable | PLLC0Connect); /* PLL0 Enable & Connect              */
   LPC_SC->PLL0FEED = PLL0FEEDVal1;
   LPC_SC->PLL0FEED = PLL0FEEDVal2;
#endif

#if (PLL1_SETUP)
   LPC_SC->PLL1CFG = PLL1CFG_Val;
   LPC_SC->PLL1CON = PLLE1Enable; /* PLL1 Enable                        */
   LPC_SC->PLL1FEED = PLL1FEEDVal1;
   LPC_SC->PLL1FEED = PLL1FEEDVal2;
   while (!(LPC_SC->PLL1STAT & PLOCK1Mask));/* Wait for PLOCK1                    */

   LPC_SC->PLL1CON = (PLLE1Enable | PLLE1Connect); /* PLL1 Enable & Connect              */
   LPC_SC->PLL1FEED = PLL1FEEDVal1;
   LPC_SC->PLL1FEED = PLL1FEEDVal2;
#else
   LPC_SC->USBCLKCFG = USBCLKCFG_Val; /* Setup USB Clock Divider            */
#endif

   LPC_SC->PCONP = PCONP_Val; /* Power Control for Peripherals      */

   LPC_SC->CLKOUTCFG = CLKOUTCFG_Val; /* Clock Output Configuration         */
#endif

   /* Determine clock frequency according to clock register values             */
   if (((LPC_SC->PLL0STAT >> PLLE0STATOfst) & PLLEnCnChkMask) == PLLEnbldNConnctd)
   {
      /* If PLL0 enabled and connected      */
      switch (LPC_SC->CLKSRCSEL & CLKSRCMask)
      {
         case CLKSRCRC: /* Internal RC oscillator => PLL0     */
         case CLKSRRES: /* Reserved, default to Internal RC   */
            SystemFrequency = (IRC_OSC * ((2
            * ((LPC_SC->PLL0STAT & MSEL0Mask) + 1))) / (((LPC_SC->PLL0STAT
            >> NSEL0Ofst) & NSEL0Mask) + 1) / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1));
         break;
         case CLKSRCOSC: /* Main oscillator => PLL0            */
            SystemFrequency = (OSC_CLK * ((2
            * ((LPC_SC->PLL0STAT & MSEL0Mask) + 1))) / (((LPC_SC->PLL0STAT
            >> NSEL0Ofst) & NSEL0Mask) + 1) / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1));
         break;
         case CLKSRCRTCOSC: /* RTC oscillator => PLL0             */
            SystemFrequency = (RTC_CLK * ((2
            * ((LPC_SC->PLL0STAT & MSEL0Mask) + 1))) / (((LPC_SC->PLL0STAT
            >> NSEL0Ofst) & NSEL0Mask) + 1) / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1));
         break;
      }
   }
   else
   {
      switch (LPC_SC->CLKSRCSEL & 0x03)
      {
         case CLKSRCRC: /* Int. RC oscillator => PLL0    */
         case CLKSRRES: /* Reserved, default to Int. RC  */
            SystemCoreClock = IRC_OSC / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1);
         break;
         case CLKSRCOSC: /* Main oscillator => PLL0       */
            SystemCoreClock = OSC_CLK / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1);
         break;
         case CLKSRCRTCOSC: /* RTC oscillator => PLL0        */
            SystemCoreClock = RTC_CLK / ((LPC_SC->CCLKCFG & CCLKSELMask) + 1);
         break;
      }
   }

#if (FLASH_SETUP == 1)                  /* Flash Accelerator Setup            */
   LPC_SC->FLASHCFG = (LPC_SC->FLASHCFG & ~FLASHCFG_Mask) | FLASHCFG_Val;
#endif
   SystemCoreClockUpdate();
}

/**************************************************************************//**
* \brief -    	InitPeripherals
*
* \brief -     	Initialize System Peripherals.
*
* \param -		none
* 
* \return -  	none
*
*****************************************************************************/
void InitPeripherals(void)
{

   FUNCTION1(PeripherialDiag, "InitPeripherals()");
#if UNIT_TESTING
#else
//#ifdef PROTOTYPEBOARD
//   SerialInit(1);
//#else
   SerialInit(0);
//#endif
#endif
   CHECKPOINT(PeripherialDiag, BRANCH_1);
   IOInit();
   CHECKPOINT(PeripherialDiag, BRANCH_2);
   printf("CPU Initialized.\n");
#if UNIT_TESTING
#else
   TestWD();
   printf("Initializing Peripherals \n");
#endif
   CHECKPOINT(PeripherialDiag, BRANCH_3);
   QuadratureEncoderInit(MAXQEIPOS);
   CHECKPOINT(PeripherialDiag, BRANCH_4);
   InitSSP();
   CHECKPOINT(PeripherialDiag, BRANCH_5);
   SSPSyncUp();
   CHECKPOINT(PeripherialDiag, BRANCH_6);
   ADCInit(ADC_CLK);
   CHECKPOINT(PeripherialDiag, BRANCH_7);
   RTCModule();
   CHECKPOINT(PeripherialDiag, BRANCH_8);
   PWM_Init(50000);
   CHECKPOINT(PeripherialDiag, BRANCH_9);
   PWM_Start();
   CHECKPOINT(PeripherialDiag, BRANCH_10);

}

/**************************************************************************//**
* \brief -    	SystemInitialization
*
* \brief -     Initialize CPU and System Peripherals.
*
* \param -		none
* 
* \return -   	none
*
*****************************************************************************/
void SystemInitialization(void)
{
   FUNCTION1(SysinitDiag, "SystemInitialization()");
   CHECKPOINT(SysinitDiag, BRANCH_1);
#if UNIT_TESTING
#else
   CPUInit();
#endif
   CHECKPOINT(SysinitDiag, BRANCH_2);
   InitPeripherals();
   CHECKPOINT(SysinitDiag, BRANCH_3);
   printf("System Initialized.\n");
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -    	ResetHandler
*
* \brief -     	ResetHandler is the entry point for System Initialization
*          		unit testing.
*
* \param - 		none
* 
* \return -   	none
*
*****************************************************************************/
void ResetHandler(void)
{
   DiagInit();
#ifndef UNIT_TESTING
   CPUDiag = 1;
   CPUInit();
   DiagPrintInt(1, 1);
   CPUDiag = 0;
   DiagSummary();
   ResetdiagTests();
   ResetdiagErrors();
#endif

   PeripherialDiag = 1;
   SyncUpFault = 0;
   InitPeripherals();
   DiagPrintInt(1, 1);
   PeripherialDiag = 0;

   SysinitDiag = 1;
   SystemInitialization();
   DiagPrintInt(1, 1);
   SysinitDiag = 0;

   DiagSummary();
}

#endif

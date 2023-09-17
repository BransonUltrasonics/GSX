// $Header:   D:/databases/VMdb/archives/EN13849/SSP.c_v   1.11   20 Jul 2011 16:59:08   ASharma6  $
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

This is the module contains all functions support for SSP firmware
library on LPC17xx.

Module name: SSP

Filename:    SSP.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for sending and receiving SSP message.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/SSP.c_v  $
 * 
 *    Rev 1.11   20 Jul 2011 16:59:08   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.10   05 Jul 2011 11:01:16   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.7   22 Jun 2011 18:11:26   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   01 Apr 2011 11:14:08   PDwivedi
 * Fixes after code review.
 * 
 *    Rev 1.2   30 Mar 2011 18:36:32   ASharma6
 * Fixed after review comments
 *
 *    Rev 1.1   25 Mar 2011 09:09:54   ygupta
 * Initial Version with File and Function Headers.
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include <stdio.h>
#include "Diagnostics.h"
#include "SSP.h"
#include "TestMacros.h"
#include "Global.h"
#include "SystemInitialization.h"

UT_VARIABLE(tSINT32 sspInit)
UT_VARIABLE(tSINT32 sspTxInit)
UT_VARIABLE(tSINT32 sspRxInit)
UT_VARIABLE(tSINT32 sspSyncUp)
UT_VARIABLE(tSINT32 sspTx)
UT_VARIABLE(tSINT32 sspRx)
UT_VARIABLE(tSINT32 sspPinConfig)
UT_VARIABLE(tSINT32 ssp0Tx)

SSP_DATA_SETUP_Type ssp0;
SSP_DATA_SETUP_Type ssp1;
tUINT16 ssp0TxMsg, ssp0RxMsg, ssp1TxMsg, ssp1RxMsg;

/** @brief SSP device configuration structure type */
typedef struct
{
   tSINT32 dataword; /* Current data word: 0 - 8 bit; 1 - 16 bit */
   tUINT32 txrx_setup; /* Transmission setup */
} SSP_CFG_T;

/* SSP configuration data */
SSP_CFG_T sspdat[2];

/**************************************************************************//**
* \brief -    	PIN_Config
*
* \brief -     	This function is used for  configuring PIN of Microcontroller.
*
* \param - 		port - 0 for Port 0, 1 for Port 1, etc
*      			pin  - 0..31 for pin within a port
*           	func - a 2-bit field for default/alternate functions of a pin
*           	mode - pullup/tristate/pulldown
*            	openDrain - normal/open drain configuration
*                   
* \return -   none
*
******************************************************************************/
void PIN_Config(tSINT32 port, tSINT32 pin, tSINT32 func, tSINT32 mode, tSINT32 openDrain)
{
	FUNCTION1(sspPinConfig, "\n PIN_Config()");
	CHECKPOINT(sspPinConfig, BRANCH_1);
	/* pinFunc */
   tUINT32 pinnum_t = pin;
   tUINT32 pinselreg_idx = (PINCONFIGFAC) * port;
   tUINT32 pinmodereg_idx = (PINCONFIGFAC) * port;
   tUINT32 *pPinFunc = (tUINT32 *) &LPC_PINCON->PINSEL0;
   tUINT32 *pPinMode = (tUINT32 *) &LPC_PINCON->PINMODE0;
   tUINT32 *pPinOpen = (tUINT32 *) &LPC_PINCON->PINMODE_OD0;

   if (pinnum_t >= MAXPORTPIN)
   {
      pinnum_t -= MAXPORTPIN;
      pinselreg_idx++;
      pinmodereg_idx++;
   }
   /* Function */
   *(tUINT32 *) (pPinFunc + pinselreg_idx) &= ~(0x03UL << (pinnum_t * (PINCONFIGFAC)));
   *(tUINT32 *) (pPinFunc + pinselreg_idx) |= ((tUINT32) func) << (pinnum_t * (PINCONFIGFAC));

   /* Mode */
   *(tUINT32 *) (pPinMode + pinmodereg_idx) &= ~(0x03UL << (pinnum_t * (PINCONFIGFAC)));
   *(tUINT32 *) (pPinMode + pinmodereg_idx) |= ((tUINT32) mode) << (pinnum_t * (PINCONFIGFAC));

   /* Open Drain Mode */
   if (openDrain == PINSEL_PINMODE_OPENDRAIN)
   {
      *(tUINT32 *) (pPinOpen + port) |= (0x01UL << pinnum_t);
   }
   else
   {
      *(tUINT32 *) (pPinOpen + port) &= ~(0x01UL << pinnum_t);
   }
   CHECKPOINT(sspPinConfig, BRANCH_2);
   ChkFunReturn = 1;
}

/**************************************************************************//**
* \brief -   	SSP0_Transmit
*
* \brief -     This function is used for  transmitting SSP message
* 				and called from SSPTxChl0 function.
*
* \param -		none
* 
* \return -   	result of transmission.If it's equal to no of bytes
*        		transmitted, it means success.
*
******************************************************************************/
tSINT32 SSP0_Transmit(void)
{
	FUNCTION1(ssp0Tx, "\nSSP0_Transmit()");
	CHECKPOINT(ssp0Tx, BRANCH_1);
	tUINT16 *rdata16 = 0;
   tUINT16 *wdata16 = 0;
   tUINT32 stat;
   tUINT32 tmp;
   tSINT32 result;

   result = 0;
   ssp0.rx_cnt = 0;
   ssp0.tx_cnt = 0;
   ssp0.status = 0;

   /* Clear all remaining data in RX FIFO */
   while (LPC_SSP0->SR & SSP_SR_RNE)
   {
      tmp = (tUINT32) (SSP_DR_BITMASK(LPC_SSP0->DR));
   }

   // Clear status
   LPC_SSP0->ICR = SSP_ICR_BITMASK;

   // Polling mode ----------------------------------------------------------------------
   rdata16 = (tUINT16 *) ssp0.rx_data;
   wdata16 = (tUINT16 *) ssp0.tx_data;

   while ((ssp0.tx_cnt != ssp0.length) || (ssp0.rx_cnt
         != ssp0.length))
   {
      if ((LPC_SSP0->SR & SSP_SR_TNF) && (ssp0.tx_cnt != ssp0.length))
      {
         // Write data to buffer, 2 bytes at a time
         LPC_SSP0->DR = SSP_DR_BITMASK(*wdata16);
         wdata16++;
         ssp0.tx_cnt += 2;
      }

      // Check overrun error
      if ((stat = LPC_SSP0->RIS) & SSP_RIS_ROR)
      {
         // save status
         ssp0.status = stat | SSP_STAT_ERROR;
         result = -1;
      }

      if (result == 0) // if no overrun error
      {
         // Check for any data available in RX FIFO
         while ((LPC_SSP0->SR & SSP_SR_RNE) && (ssp0.rx_cnt
               != ssp0.length))
         {
            // Read data from SSP data
            tmp = ((tUINT16) (SSP_DR_BITMASK(LPC_SSP0->DR)));
            result = ssp0.tx_cnt;

            // Store data to destination
            if (ssp0.rx_data != NULL)
            {
               *(rdata16) = (tUINT16) tmp;
               rdata16++;
            }
            // Increase counter
            ssp0.rx_cnt += 2;
         }
      }
   }

   if (result != ssp0.tx_cnt) // if didn't transmit
   {
      // save status
      ssp0.status = SSP_STAT_DONE;

      if (ssp0.tx_data != NULL)
      {
         result = ssp0.tx_cnt;
      }
      else if (ssp0.rx_data != NULL)
      {
         result = ssp0.rx_cnt;
      }
      else
      {
         result = 0;
      }
   }
   CHECKPOINT(ssp0Tx, BRANCH_2);
   ChkFunReturn = 1;
   return result;
}

/**************************************************************************//**
* \brief -    	SSPTxChl0
*
* \brief -     	This function is used for  transmitting SSP message
* 				on channel 10.
*
* \param -    	none
* 
* \return -  	Result of the transmission. If it's equal to no of bytes
*          		transmitted, it means success.
*
******************************************************************************/
tSINT32 SSPTxChl0(void)
{
   tSINT32 result;
   FUNCTION1(sspTx, "\nSSPTxChl0()");
   CHECKPOINT(sspTx, BRANCH_1);
   result = SSP0_Transmit();
   CHECKPOINT(sspTx, BRANCH_2);
   return result;
}

/**************************************************************************//**
* \brief -    		SSPRxChl1
*
* \brief -     	This function is used for  receiving SSP message
* 					on channel 11.
*
* \param -       	none
* 
* \return -   		none
*
******************************************************************************/
void SSPRxChl1(void)
{
   FUNCTION1(sspRx, "\nSSPRxChl1()");
   CHECKPOINT(sspRx, BRANCH_1);

   ssp1RxMsg = ((tUINT16) (SSP_DR_BITMASK(LPC_SSP1->DR)));
   CHECKPOINT(sspRx, BRANCH_2);
}

/**************************************************************************//**
* \brief -     SSPSyncUp
*
* \param -		none
* 
* \return -    none
*
******************************************************************************/
void SSPSyncUp(void)
{
   FUNCTION1(sspSyncUp, "\nSSPSyncUp()");
   CHECKPOINT(sspSyncUp, BRANCH_1);
   do
   {
      SSPTxChl0();
      CHECKPOINT(sspSyncUp, BRANCH_2);
      SSPRxChl1();
      CHECKPOINT(sspSyncUp, BRANCH_3);
   }
   while (ssp1RxMsg != SSP0_PATTERN);

   CHECKPOINT(sspSyncUp, BRANCH_4);
}

/**************************************************************************//**
* \brief -    	InitSSPChl0Tx
*
* \brief -		This function is for Initialization of SSP channel 10
* 				for Transmitting message.
*
* \param -  	none
* 
* \return -   	none
*
******************************************************************************/
void InitSSPChl0Tx(void)
{
   tUINT32 tmp;
   SSP_CFG_Type SSP_InitStruct;
   tUINT32 prescale, cr0_div, cmp_clk, ssp_clk, target_clock;

   FUNCTION1(sspTxInit, "\nInitSSPChl0Tx()");
   CHECKPOINT(sspTxInit, BRANCH_1);

   // SSI0
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_15, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_16, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_17, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_18, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);

   CHECKPOINT(sspTxInit, BRANCH_2);
   LPC_SSP0->CR1 &= (~SSP_CR1_SSP_EN) & SSP_CR1_BITMASK;

   /* Config Init */
   SSP_InitStruct.CPHA = SSP_CPHA_SECOND;
   SSP_InitStruct.CPOL = SSP_CPOL_LO;
   SSP_InitStruct.ClockRate = CLK_RATE;
   SSP_InitStruct.Databit = SSP_DATABIT_16;
   SSP_InitStruct.Mode = SSP_MASTER_MODE;
   SSP_InitStruct.FrameFormat = SSP_FRAME_SPI;

   /* Set up clock and power for SSP0 module */
   LPC_SC->PCONP |= CLKPWR_PCONP_PCSSP0 & CLKPWR_PCONP_BITMASK;
   /* Configure SSP, interrupt is disable, LoopBack mode is disable,
    * SSP is disable, Slave output is disable as default
    */
   tmp = ((SSP_InitStruct.CPHA) | (SSP_InitStruct.CPOL)
         | (SSP_InitStruct.FrameFormat) | (SSP_InitStruct.Databit))
         & SSP_CR0_BITMASK;
   // write back to SSP control register
   LPC_SSP0->CR0 = tmp;
   sspdat[0].dataword = 1;

   tmp = SSP_MASTER_MODE & SSP_CR1_BITMASK;
   // Write back to CR1
   LPC_SSP0->CR1 = tmp;

   /* The SSP clock is derived from the (main system oscillator / 2),
    so compute the best divider from that clock */
   ssp_clk = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_SSP0);

   /* Find closest divider to get at or under the target frequency.
    Use smallest prescale possible and rely on the divider to get
    the closest target frequency */
   target_clock = CLK_RATE;
   cr0_div = 0;
   cmp_clk = 0xFFFFFFFF;
   prescale = 2;
   while (cmp_clk > target_clock)
   {
      cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
      if (cmp_clk > target_clock)
      {
         cr0_div++;
         if (cr0_div > 0xFF)
         {
            cr0_div = 0;
            prescale += 2;
         }
      }
   }

   /* Write computed prescaler and divider back to register */
   LPC_SSP0->CR0 &= (~SSP_CR0_SCR(0xFF)) & SSP_CR0_BITMASK;
   LPC_SSP0->CR0 |= (SSP_CR0_SCR(cr0_div)) & SSP_CR0_BITMASK;
   LPC_SSP0->CPSR = prescale & SSP_CPSR_BITMASK;

   LPC_SSP0->CR1 |= SSP_CR1_SSP_EN;
   ssp0.tx_data = &ssp0TxMsg;
   ssp0.rx_data = &ssp0RxMsg;
   ssp0.length = sizeof(ssp0TxMsg);
   CHECKPOINT(sspTxInit, BRANCH_3);
   ChkFunReturn = 1;
}

/**************************************************************************//**
* \brief -	InitSSPChl1Rx
*
* \brief - 	This function is for Initialization of SSP channel 11
* 			for receiving message.
*
* \param -	none
* 
* \return -	none
*
******************************************************************************/
void InitSSPChl1Rx(void)
{
   tUINT32 tmp;
   SSP_CFG_Type SSP_InitStruct;
   tUINT32 prescale, cr0_div, cmp_clk, ssp_clk, target_clock;

   FUNCTION1(sspRxInit, "\nInitSSPChl1Rx()");
   CHECKPOINT(sspRxInit, BRANCH_1);

   /* Initialise SSP */
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_6, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_7, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_8, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_9, PINSEL_FUNC_2,
         PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);

   CHECKPOINT(sspRxInit, BRANCH_2);
   LPC_SSP1->CR1 &= (~SSP_CR1_SSP_EN) & SSP_CR1_BITMASK;

   /* Config Init */
   SSP_InitStruct.CPHA = SSP_CPHA_SECOND;
   SSP_InitStruct.CPOL = SSP_CPOL_LO;
   SSP_InitStruct.ClockRate = CLK_RATE;
   SSP_InitStruct.Databit = SSP_DATABIT_16;
   SSP_InitStruct.Mode = SSP_SLAVE_MODE;
   SSP_InitStruct.FrameFormat = SSP_FRAME_SPI;

   /* Set up clock and power for SSP1 module */
   LPC_SC->PCONP |= CLKPWR_PCONP_PCSSP1 & CLKPWR_PCONP_BITMASK;

   /* Configure SSP, interrupt is disable, LoopBack mode is disable,
    * SSP is disable, Slave output is disable as default
    */
   tmp = ((SSP_InitStruct.CPHA) | (SSP_InitStruct.CPOL)
         | (SSP_InitStruct.FrameFormat) | (SSP_InitStruct.Databit))
         & SSP_CR0_BITMASK;
   // write back to SSP control register
   LPC_SSP1->CR0 = tmp;
   sspdat[1].dataword = 1;

   tmp = SSP_SLAVE_MODE & SSP_CR1_BITMASK;
   // Write back to CR1
   LPC_SSP1->CR1 = tmp;

   /* The SSP clock is derived from the (main system oscillator / 2),
    so compute the best divider from that clock */
   ssp_clk = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_SSP1);

   /* Find closest divider to get at or under the target frequency.
    Use smallest prescale possible and rely on the divider to get
    the closest target frequency */
   target_clock = CLK_RATE;
   cr0_div = 0;
   cmp_clk = 0xFFFFFFFF;
   prescale = 2;
   while (cmp_clk > target_clock)
   {
      cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
      if (cmp_clk > target_clock)
      {
         cr0_div++;
         if (cr0_div > 0xFF)
         {
            cr0_div = 0;
            prescale += 2;
         }
      }
   }

   /* Write computed prescaler and divider back to register */
   LPC_SSP1->CR0 &= (~SSP_CR0_SCR(0xFF)) & SSP_CR0_BITMASK;
   LPC_SSP1->CR0 |= (SSP_CR0_SCR(cr0_div)) & SSP_CR0_BITMASK;
   LPC_SSP1->CPSR = prescale & SSP_CPSR_BITMASK;

   LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;

   // SSP1 slave
   ssp1.tx_data = &ssp1TxMsg;
   ssp1.rx_data = &ssp1RxMsg;
   ssp1.length = sizeof(ssp1TxMsg);

   ssp1RxMsg = 0; // Nothing's received yet
   CHECKPOINT(sspRxInit, BRANCH_3);
   ChkFunReturn = 1;
}

/**************************************************************************//**
* \brief -    	InitSSP
*
* \brief -     This function is for Initialization of SSP Module.
*
* \param -		none
* 
* \return -   	none
*
******************************************************************************/
void InitSSP()
{
   FUNCTION1(sspInit, "\nInitSSP()");
   CHECKPOINT(sspInit, BRANCH_1);
   ssp0TxMsg = SSP0_PATTERN;
   InitSSPChl0Tx();
   CHECKPOINT(sspInit, BRANCH_2);
   InitSSPChl1Rx();
   CHECKPOINT(sspInit, BRANCH_3);
   ChkFunReturn = 1;
}

#if UNIT_TESTING

/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for SSP test unit testing.
*
* \param -		none
* 
* \return -		none
*
*****************************************************************************/
void ResetHandler(void)
{
   tSINT32 PassCnt;

   DiagInit();

   ssp0TxMsg = SSP0_PATTERN;  // Any non-zero pattern
   ssp1RxMsg = 0; // Nothing's received yet

   sspPinConfig = 1;
   ChkFunReturn = 0;
   PIN_Config(PINSEL_PORT_0, PINSEL_PIN_15, PINSEL_FUNC_2,
            PINSEL_PINMODE_PULLUP, PINSEL_PINMODE_NORMAL);
   DiagPrintInt(ChkFunReturn, 1);
   sspPinConfig = 0;

   sspTxInit = 1;
   ChkFunReturn = 0;
   InitSSPChl0Tx();
   DiagPrintInt(ChkFunReturn, 1);
   sspRxInit = 1;
   ChkFunReturn = 0;
   InitSSPChl1Rx();
   DiagPrintInt(ChkFunReturn, 1);

   ChkFunReturn = 0;
   sspInit = 1;
   sspTxInit = 0;
   sspRxInit = 0;
   InitSSP();
   DiagPrintInt(ChkFunReturn, 1);
   sspInit = 0;

   ssp0Tx = 1;
   ChkFunReturn = 0;
   SSP0_Transmit();
   DiagPrintInt(ChkFunReturn, 1);
   ssp0Tx = 0;

   sspTx = 1;
   sspRx = 1;

   ssp1RxMsg = 0;
   PassCnt = 0;
   do
   {
      PassCnt++;
      SSPTxChl0();
      SSPRxChl1();
   }
   while (PassCnt < MAXSSPPASSCOUNT);

   DiagPrintInt(ssp1RxMsg, SSP0_PATTERN);

   sspTx = 0;
   sspRx = 0;
   sspSyncUp = 1;
   SSPSyncUp();
   sspSyncUp = 0;
   DiagPrintInt(ssp1RxMsg, SSP0_PATTERN);

   DiagSummary();
}

#endif


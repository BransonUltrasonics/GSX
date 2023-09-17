// $Header:   D:/databases/VMdb/archives/EN13849/inc/lpc17xx_libcfg_default.h_v   1.7   20 Jul 2011 16:47:44   ASharma6  $
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
 LPC17xx .  Default Library configuration header file

 Module name: lpc17xx_libcfg_default
 File name: lpc17xx_libcfg_default.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/lpc17xx_libcfg_default.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:47:44   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:56:52   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:14:32   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   01 Apr 2011 11:29:08   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:17:46   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef LPC17XX_LIBCFG_DEFAULT_H_
#define LPC17XX_LIBCFG_DEFAULT_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc_types.h"

/* Public Macros -------------------------------------------------------------- */

/************************** DEBUG MODE DEFINITIONS *********************************/
/* Un-comment the line below to compile the library in DEBUG mode, this will expanse
 the "CHECK_PARAM" macro in the FW library code */

//#define DEBUG    1


/******************* PERIPHERAL FW LIBRARY CONFIGURATION DEFINITIONS ***********************/

/* Comment the line below to disable the specific peripheral inclusion */

/* GPIO ------------------------------- */
#define _GPIO   1

/* UART ------------------------------- */
#define _UART   1
#define _UART0  1
#define _UART1  1
#define _UART2  1
#define _UART3  1

/* SPI ------------------------------- */
#define _SPI    1

/* SSP ------------------------------- */
#define _SSP    1
#define _SSP0   1
#define _SSP1   1

/* I2C ------------------------------- */
#define _I2C    1
#define _I2C0   1
#define _I2C1   1
#define _I2C2   1

/* TIMER ------------------------------- */
#define _TIM    1

/* WDT ------------------------------- */
#define _WDT    1

/* GPDMA ------------------------------- */
#define _GPDMA  1

/* DAC ------------------------------- */
#define _DAC    1

/* DAC ------------------------------- */
#define _ADC    1

/* PWM ------------------------------- */
#define _PWM    1
#define _PWM1   1

/* RTC ------------------------------- */
#define _RTC    1

/* I2S ------------------------------- */
#define _I2S    1

/* USB device ------------------------------- */
#define _USBDEV     1
#define _USB_DMA    1

/* QEI ------------------------------- */
#define _QEI    1

/* MCPWM ------------------------------- */
#define _MCPWM  1

/* CAN--------------------------------*/
#define _CAN    1

/* RIT ------------------------------- */
#define _RIT    1

/* EMAC ------------------------------ */
#define _EMAC   1

/************************** GLOBAL/PUBLIC MACRO DEFINITIONS *********************************/

#ifdef  DEBUG
/*******************************************************************************
 * @brief       The CHECK_PARAM macro is used for function's parameters check.
 *              It is used only if the library is compiled in DEBUG mode.
 * @param[in]   expr - If expr is false, it calls check_failed() function
 *                      which reports the name of the source file and the source
 *                      line number of the call that failed.
 *                    - If expr is true, it returns no value.
 * @return      None
 *******************************************************************************/
#define CHECK_PARAM(expr) ((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
#else
#define CHECK_PARAM(expr) do { } while (0)
#endif /* DEBUG */

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup LIBCFG_DEFAULT_Public_Functions
 * @{
 */

#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line);
#endif

/**
 * @}
 */

#endif /* LPC17XX_LIBCFG_DEFAULT_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

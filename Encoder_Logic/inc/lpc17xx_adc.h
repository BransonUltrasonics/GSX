// $Header:   D:/databases/VMdb/archives/EN13849/inc/lpc17xx_adc.h_v   1.7   20 Jul 2011 16:47:38   ASharma6  $
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
 LPC17xx .Contains all macro definitions and function prototypes support for
 ADC firmware library on LPC17xx
 Module name: lpc17xx_adc
 File name: lpc17xx_adc.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/lpc17xx_adc.h_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:47:38   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 10:56:46   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:14:26   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   01 Apr 2011 11:28:44   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:17:22   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef LPC17XX_ADC_H_
#define LPC17XX_ADC_H_
/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************//**
 * Macro defines for ADC  control register
 **********************************************************************/
/**  Selects which of the AD0.0:7 pins is (are) to be sampled and converted */
#define ADC_CR_CH_SEL(n)    ((1UL << n))
/**  The APB clock (PCLK) is divided by (this value plus one)
* to produce the clock for the A/D */
#define ADC_CR_CLKDIV(n)    ((n<<8))
/**  Repeated conversions A/D enable bit */
#define ADC_CR_BURST        ((1UL<<16))
/**  ADC convert in power down mode */
#define ADC_CR_PDN          ((1UL<<21))
/**  Start mask bits */
#define ADC_CR_START_MASK   ((7UL<<24))
/**  Select Start Mode */
#define ADC_CR_START_MODE_SEL(SEL)  ((SEL<<24))
/**  Start conversion now */
#define ADC_CR_START_NOW    ((1UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on P2.10/EINT0 */
#define ADC_CR_START_EINT0  ((2UL<<24))
/** Start conversion when the edge selected by bit 27 occurs on P1.27/CAP0.1 */
#define ADC_CR_START_CAP01  ((3UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT0.1 */
#define ADC_CR_START_MAT01  ((4UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT0.3 */
#define ADC_CR_START_MAT03  ((5UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT1.0 */
#define ADC_CR_START_MAT10  ((6UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT1.1 */
#define ADC_CR_START_MAT11  ((7UL<<24))
/**  Start conversion on a falling edge on the selected CAP/MAT signal */
#define ADC_CR_EDGE         ((1UL<<27))

/*********************************************************************//**
 * Macro defines for ADC Global Data register
 **********************************************************************/
/** When DONE is 1, this field contains result value of ADC conversion */
#define ADC_GDR_RESULT(n)       (((n>>4)&0xFFF))
/** These bits contain the channel from which the LS bits were converted */
#define ADC_GDR_CH(n)           (((n>>24)&0x7))
/** This bit is 1 in burst mode if the results of one or
 * more conversions was (were) lost */
#define ADC_GDR_OVERRUN_FLAG    ((1UL<<30))
/** This bit is set to 1 when an A/D conversion completes */
#define ADC_GDR_DONE_FLAG       ((1UL<<31))

/** This bits is used to mask for Channel */
#define ADC_GDR_CH_MASK     ((7UL<<24))
/*********************************************************************//**
 * Macro defines for ADC Interrupt register
 **********************************************************************/
/** These bits allow control over which A/D channels generate
 * interrupts for conversion completion */
#define ADC_INTEN_CH(n)         ((1UL<<n))
/** When 1, enables the global DONE flag in ADDR to generate an interrupt */
#define ADC_INTEN_GLOBAL        ((1UL<<8))

/*********************************************************************//**
 * Macro defines for ADC Data register
 **********************************************************************/
/** When DONE is 1, this field contains result value of ADC conversion */
#define ADC_DR_RESULT(n)        (((n>>4)&0xFFF))
/** These bits mirror the OVERRRUN status flags that appear in the
 * result register for each A/D channel */
#define ADC_DR_OVERRUN_FLAG     ((1UL<<30))
/** This bit is set to 1 when an A/D conversion completes. It is cleared
 * when this register is read */
#define ADC_DR_DONE_FLAG        ((1UL<<31))

/*********************************************************************//**
 * Macro defines for ADC Status register
**********************************************************************/
/** These bits mirror the DONE status flags that appear in the result
 * register for each A/D channel */
#define ADC_STAT_CH_DONE_FLAG(n)        ((n&0xFF))
/** These bits mirror the OVERRRUN status flags that appear in the
 * result register for each A/D channel */
#define ADC_STAT_CH_OVERRUN_FLAG(n)     (((n>>8)&0xFF))
/** This bit is the A/D interrupt flag */
#define ADC_STAT_INT_FLAG               ((1UL<<16))

/*********************************************************************//**
 * Macro defines for ADC Trim register
**********************************************************************/
/** Offset trim bits for ADC operation */
#define ADC_ADCOFFS(n)      (((n&0xF)<<4))
/** Written to boot code*/
#define ADC_TRIM(n)         (((n&0xF)<<8))

/*********************************************************************//**
 * @brief ADC enumeration
 **********************************************************************/
/** @brief Channel Selection */
typedef enum
{
    ADC_CHANNEL_0  = 0, /*!<  Channel 0 */
    ADC_CHANNEL_1,      /*!<  Channel 1 */
    ADC_CHANNEL_2,      /*!<  Channel 2 */
    ADC_CHANNEL_3,      /*!<  Channel 3 */
    ADC_CHANNEL_4,      /*!<  Channel 4 */
    ADC_CHANNEL_5,      /*!<  Channel 5 */
    ADC_CHANNEL_6,      /*!<  Channel 6 */
    ADC_CHANNEL_7       /*!<  Channel 7 */
}ADC_CHANNEL_SELECTION;


/** @brief Type of start option */

typedef enum
{
    ADC_START_CONTINUOUS =0,    /*!< Continuous mode */
    ADC_START_NOW,              /*!< Start conversion now */
    ADC_START_ON_EINT0,         /*!< Start conversion when the edge selected
                                 * by bit 27 occurs on P2.10/EINT0 */
    ADC_START_ON_CAP01,         /*!< Start conversion when the edge selected
                                 * by bit 27 occurs on P1.27/CAP0.1 */
    ADC_START_ON_MAT01,         /*!< Start conversion when the edge selected
                                 * by bit 27 occurs on MAT0.1 */
    ADC_START_ON_MAT03,         /*!< Start conversion when the edge selected
                                 * by bit 27 occurs on MAT0.3 */
    ADC_START_ON_MAT10,         /*!< Start conversion when the edge selected
                                  * by bit 27 occurs on MAT1.0 */
    ADC_START_ON_MAT11          /*!< Start conversion when the edge selected
                                  * by bit 27 occurs on MAT1.1 */
} ADC_START_OPT;


/** @brief Type of edge when start conversion on the selected CAP/MAT signal */

typedef enum
{
    ADC_START_ON_RISING = 0,    /*!< Start conversion on a rising edge
                                *on the selected CAP/MAT signal */
    ADC_START_ON_FALLING        /*!< Start conversion on a falling edge
                                *on the selected CAP/MAT signal */
} ADC_START_ON_EDGE_OPT;

/** @brief* ADC type interrupt enum */
typedef enum
{
    ADC_ADINTEN0 = 0,       /*!< Interrupt channel 0 */
    ADC_ADINTEN1,           /*!< Interrupt channel 1 */
    ADC_ADINTEN2,           /*!< Interrupt channel 2 */
    ADC_ADINTEN3,           /*!< Interrupt channel 3 */
    ADC_ADINTEN4,           /*!< Interrupt channel 4 */
    ADC_ADINTEN5,           /*!< Interrupt channel 5 */
    ADC_ADINTEN6,           /*!< Interrupt channel 6 */
    ADC_ADINTEN7,           /*!< Interrupt channel 7 */
    ADC_ADGINTEN            /*!< Individual channel/global flag done generate an interrupt */
}ADC_TYPE_INT_OPT;

/** Macro to determine if it is valid interrupt type */
#define PARAM_ADC_TYPE_INT_OPT(OPT) ((OPT == ADC_ADINTEN0)||(OPT == ADC_ADINTEN1)\
||(OPT == ADC_ADINTEN2)||(OPT == ADC_ADINTEN3)\
||(OPT == ADC_ADINTEN4)||(OPT == ADC_ADINTEN5)\
||(OPT == ADC_ADINTEN6)||(OPT == ADC_ADINTEN7)\
||(OPT == ADC_ADGINTEN))


/** @brief ADC Data  status */
typedef enum
{
    ADC_DATA_BURST = 0,     /*Burst bit*/
    ADC_DATA_DONE        /*Done bit*/
}ADC_DATA_STATUS;


#define PARAM_ADC_START_ON_EDGE_OPT(OPT)    ((OPT == ADC_START_ON_RISING)||(OPT == ADC_START_ON_FALLING))

#define PARAM_ADC_DATA_STATUS(OPT)  ((OPT== ADC_DATA_BURST)||(OPT== ADC_DATA_DONE))

#define PARAM_ADC_FREQUENCY(FRE) (FRE <= 13000000 )

#define PARAM_ADC_CHANNEL_SELECTION(SEL)     ((SEL == ADC_CHANNEL_0)||(ADC_CHANNEL_1)\
||(SEL == ADC_CHANNEL_2)|(ADC_CHANNEL_3)\
||(SEL == ADC_CHANNEL_4)||(ADC_CHANNEL_5)\
||(SEL == ADC_CHANNEL_6)||(ADC_CHANNEL_7))

#define PARAM_ADC_START_OPT(OPT)    ((OPT == ADC_START_CONTINUOUS)||(OPT == ADC_START_NOW)\
||(OPT == ADC_START_ON_EINT0)||(OPT == ADC_START_ON_CAP01)\
||(OPT == ADC_START_ON_MAT01)||(OPT == ADC_START_ON_MAT03)\
||(OPT == ADC_START_ON_MAT10)||(OPT == ADC_START_ON_MAT11))

#define PARAM_ADC_TYPE_INT_OPT(OPT) ((OPT == ADC_ADINTEN0)||(OPT == ADC_ADINTEN1)\
||(OPT == ADC_ADINTEN2)||(OPT == ADC_ADINTEN3)\
||(OPT == ADC_ADINTEN4)||(OPT == ADC_ADINTEN5)\
||(OPT == ADC_ADINTEN6)||(OPT == ADC_ADINTEN7)\
||(OPT == ADC_ADGINTEN))

#define PARAM_ADCx(n)   (((uint32_t *)n)==((uint32_t *)LPC_ADC))

/* Public Functions ----------------------------------------------------------- */

void ADC_Init(LPC_ADC_TypeDef *ADCx, uint32_t ConvFreq);
void ADC_DeInit(LPC_ADC_TypeDef *ADCx);
void ADC_BurstCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_PowerdownCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState);
void ADC_StartCmd(LPC_ADC_TypeDef *ADCx, uint8_t start_mode);
void ADC_EdgeStartConfig(LPC_ADC_TypeDef *ADCx, uint8_t EdgeOption);
void ADC_IntConfig (LPC_ADC_TypeDef *ADCx, ADC_TYPE_INT_OPT IntType, FunctionalState NewState);
void ADC_ChannelCmd (LPC_ADC_TypeDef *ADCx, uint8_t Channel, FunctionalState NewState);
uint16_t ADC_ChannelGetData(LPC_ADC_TypeDef *ADCx, uint8_t channel);
FlagStatus ADC_ChannelGetStatus(LPC_ADC_TypeDef *ADCx, uint8_t channel, uint32_t StatusType);
uint16_t ADC_GlobalGetData(LPC_ADC_TypeDef *ADCx, uint8_t channel);
FlagStatus  ADC_GlobalGetStatus(LPC_ADC_TypeDef *ADCx, uint32_t StatusType);


#ifdef __cplusplus
}
#endif


#endif /* LPC17XX_ADC_H_ */

/* --------------------------------- End Of File ------------------------------ */

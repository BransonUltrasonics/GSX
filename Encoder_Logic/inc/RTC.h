// $Header:   D:/databases/VMdb/archives/EN13849/inc/RTC.h_v   1.8   20 Jul 2011 16:48:10   ASharma6  $
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
 RTC Driver.
 Module name: RTC
 File name: RTC.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/inc/RTC.h_v  $
 * 
 *    Rev 1.8   20 Jul 2011 16:48:10   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.7   05 Jul 2011 10:57:14   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.4   22 Jun 2011 18:14:56   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.3   01 Apr 2011 11:30:10   PDwivedi
 * Fixex after review.
 * 
 *    Rev 1.2   31 Mar 2011 11:58:08   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:19:16   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/

#ifndef LPC17XX_RTC_H_
#define LPC17XX_RTC_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/** @defgroup RTC_Private_Macros
 * @{
 */

/** @defgroup RTC_REGISTER_BIT_DEFINITIONS
 * @{
 */

/* Miscellaneous register group --------------------------------------------- */

/**********************************************************************
* ILR register definitions
**********************************************************************/
/** ILR register mask */
#define RTC_ILR_BITMASK			((0x00000003))
/** Bit inform the source interrupt is counter increment*/
#define RTC_IRL_RTCCIF			((1<<0))
/** Bit inform the source interrupt is alarm match*/
#define RTC_IRL_RTCALF			((1<<1))


/**********************************************************************
* CCR register definitions
**********************************************************************/
/** CCR register mask */
#define RTC_CCR_BITMASK			((0x00000013))
/** Clock enable */
#define RTC_CCR_CLKEN			((1<<0))
/** Clock reset */
#define RTC_CCR_CTCRST			((1<<1))
/** Calibration counter enable */
#define RTC_CCR_CCALEN			((1<<4))


/**********************************************************************
* CIIR register definitions
**********************************************************************/
/** Counter Increment Interrupt bit for second */
#define RTC_CIIR_IMSEC			((1<<0))
/** Counter Increment Interrupt bit for minute */
#define RTC_CIIR_IMMIN			((1<<1))
/** Counter Increment Interrupt bit for hour */
#define RTC_CIIR_IMHOUR			((1<<2))
/** Counter Increment Interrupt bit for day of month */
#define RTC_CIIR_IMDOM			((1<<3))
/** Counter Increment Interrupt bit for day of week */
#define RTC_CIIR_IMDOW			((1<<4))
/** Counter Increment Interrupt bit for day of year */
#define RTC_CIIR_IMDOY			((1<<5))
/** Counter Increment Interrupt bit for month */
#define RTC_CIIR_IMMON			((1<<6))
/** Counter Increment Interrupt bit for year */
#define RTC_CIIR_IMYEAR			((1<<7))
/** CIIR bit mask */
#define RTC_CIIR_BITMASK		((0xFF))

/**********************************************************************
* AMR register definitions
**********************************************************************/
/** Counter Increment Select Mask bit for second */
#define RTC_AMR_AMRSEC			((1<<0))
/** Counter Increment Select Mask bit for minute */
#define RTC_AMR_AMRMIN			((1<<1))
/** Counter Increment Select Mask bit for hour */
#define RTC_AMR_AMRHOUR			((1<<2))
/** Counter Increment Select Mask bit for day of month */
#define RTC_AMR_AMRDOM			((1<<3))
/** Counter Increment Select Mask bit for day of week */
#define RTC_AMR_AMRDOW			((1<<4))
/** Counter Increment Select Mask bit for day of year */
#define RTC_AMR_AMRDOY			((1<<5))
/** Counter Increment Select Mask bit for month */
#define RTC_AMR_AMRMON			((1<<6))
/** Counter Increment Select Mask bit for year */
#define RTC_AMR_AMRYEAR			((1<<7))
/** AMR bit mask */
#define RTC_AMR_BITMASK			((0xFF))

/**********************************************************************
* RTC_AUX register definitions
**********************************************************************/
/** RTC Oscillator Fail detect flag */
#define RTC_AUX_RTC_OSCF		((1<<4))

/**********************************************************************
* RTC_AUXEN register definitions
**********************************************************************/
/** Oscillator Fail Detect interrupt enable*/
#define RTC_AUXEN_RTC_OSCFEN	((1<<4))


/* Consolidated time register group ----------------------------------- */
/** Consolidated Time Register 0 */
#define RTC_CTIME0_SECONDS_MASK		((0x3F))
#define RTC_CTIME0_MINUTES_MASK		((0x3F00))
#define RTC_CTIME0_HOURS_MASK		((0x1F0000))
#define RTC_CTIME0_DOW_MASK			((0x7000000))
/** Consolidated Time Register 1 */
#define RTC_CTIME1_DOM_MASK			((0x1F))
#define RTC_CTIME1_MONTH_MASK		((0xF00))
#define RTC_CTIME1_YEAR_MASK		((0xFFF0000))
/** Consolidated Time Register 2 */
#define RTC_CTIME2_DOY_MASK			((0xFFF))


/* Time Counter Group and Alarm register group ----------------------------- */
/** SEC register mask */
#define RTC_SEC_MASK			(0x0000003F)
/** MIN register mask */
#define RTC_MIN_MASK			(0x0000003F)
/** HOUR register mask */
#define RTC_HOUR_MASK			(0x0000001F)
/** DOM register mask */
#define RTC_DOM_MASK			(0x0000001F)
/** DOW register mask */
#define RTC_DOW_MASK			(0x00000007)
/** DOY register mask */
#define RTC_DOY_MASK			(0x000001FF)
/** MONTH register mask */
#define RTC_MONTH_MASK			(0x0000000F)
/** YEAR register mask */
#define RTC_YEAR_MASK			(0x00000FFF)

#define RTC_SECOND_MAX		59 /*!< Maximum value of second */
#define RTC_MINUTE_MAX		59 /*!< Maximum value of minute*/
#define RTC_HOUR_MAX		23 /*!< Maximum value of hour*/
#define RTC_MONTH_MIN		1 /*!< Minimum value of month*/
#define RTC_MONTH_MAX		12 /*!< Maximum value of month*/
#define RTC_DAYOFMONTH_MIN 	1 /*!< Minimum value of day of month*/
#define RTC_DAYOFMONTH_MAX 	31 /*!< Maximum value of day of month*/
#define RTC_DAYOFWEEK_MAX	6 /*!< Maximum value of day of week*/
#define RTC_DAYOFYEAR_MIN	1 /*!< Minimum value of day of year*/
#define RTC_DAYOFYEAR_MAX	366 /*!< Maximum value of day of year*/
#define RTC_YEAR_MAX		4095 /*!< Maximum value of year*/

/* Calibration register */
/** Calibration value */
#define RTC_CALIBRATION_CALVAL_MASK		((0x1FFFF))
/** Calibration direction */
#define RTC_CALIBRATION_LIBDIR			((1<<17))
/** Calibration max value */
#define RTC_CALIBRATION_MAX				((0x20000))

/** @brief Time structure definitions for easy manipulate the data */
typedef struct {
	uint32_t SEC; 		/*!< Seconds Register */
	uint32_t MIN; 		/*!< Minutes Register */
	uint32_t HOUR; 		/*!< Hours Register */
	uint32_t DOM;		/*!< Day of Month Register */
	uint32_t DOW; 		/*!< Day of Week Register */
	uint32_t DOY; 		/*!< Day of Year Register */
	uint32_t MONTH; 	/*!< Months Register */
	uint32_t YEAR; 		/*!< Years Register */
} RTC_TIME_Type;

/** @brief RTC interrupt source */
typedef enum {
	RTC_INT_COUNTER_INCREASE = RTC_IRL_RTCCIF, 	/*!<  Counter Increment Interrupt */
	RTC_INT_ALARM = RTC_IRL_RTCALF, 				/*!< The alarm interrupt */
} RTC_INT_OPT;

#define PARAM_RTC_INT(n)	((n==RTC_INT_COUNTER_INCREASE) || (n==RTC_INT_ALARM))

/** @brief RTC time type option */
typedef enum {
	RTC_TIMETYPE_SECOND = 0, 		/*!< Second */
	RTC_TIMETYPE_MINUTE = 1, 		/*!< Month */
	RTC_TIMETYPE_HOUR = 2, 			/*!< Hour */
	RTC_TIMETYPE_DAYOFWEEK = 3, 	/*!< Day of week */
	RTC_TIMETYPE_DAYOFMONTH = 4, 	/*!< Day of month */
	RTC_TIMETYPE_DAYOFYEAR = 5, 	/*!< Day of year */
	RTC_TIMETYPE_MONTH = 6, 		/*!< Month */
	RTC_TIMETYPE_YEAR = 7, 			/*!< Year */
} RTC_TIMETYPE_Num;

#define PARAM_RTC_TIMETYPE(n)	((n==RTC_TIMETYPE_SECOND) || (n==RTC_TIMETYPE_MINUTE) \
|| (n==RTC_TIMETYPE_HOUR) || (n==RTC_TIMETYPE_DAYOFWEEK) \
|| (n==RTC_TIMETYPE_DAYOFMONTH) || (n==RTC_TIMETYPE_DAYOFYEAR) \
|| (n==RTC_TIMETYPE_MONTH) || (n==RTC_TIMETYPE_YEAR))


/* Public Macros -------------------------------------------------------------- */
/** Macro to determine if it is valid RTC peripheral */
#define PARAM_RTCx(x)	(((uint32_t *)x)==((uint32_t *)LPC_RTC))

/** Calibration definitions */
#define RTC_CALIB_DIR_FORWARD	((uint8_t)(0))
#define RTC_CALIB_DIR_BACKWARD	((uint8_t)(1))

#define PARAM_RTC_CALIB_DIR(n)	((n==RTC_CALIB_DIR_FORWARD) || (n==RTC_CALIB_DIR_BACKWARD))
#define PARAM_RTC_GPREG_CH(n)	((n>=0) && (n<=4))

#define PARAM_RTC_CALIBRATION_DIR(n)

/* Public Functions ----------------------------------------------------------- */
void RTC_Init (LPC_RTC_TypeDef *RTCx);
void RTC_Configure(LPC_RTC_TypeDef *RTCx, tUINT32 Interrupt_Type,\
		FunctionalState NewState);
void RTC_SetTime(LPC_RTC_TypeDef *RTCx, tUINT32 Timetype, \
		tUINT32 TimeValue);
void RTCModule(void);

extern void (*RTC_Callback)(void);
extern void (*SysTick_Callback)(void);


#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_RTC_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

/* vxbI2cPcfRtc.c - VxBus PCF RTC driver */

/*
 * Copyright (c) 2012-2015, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05jan17,wch  clarified RTC alarm is not supported (F6376)
17aug15,sye  set and clear stop bit when setting the rtc time. (V7PRO-2099)
29jun15,xms  fixed compiler warning. (F2518)
23sep14,l_z  fix memory leak issue. (V7PRO-1271)
08aug14,x_z  fixed compiling wranings. (US40508)
19may14,mpc  clean warnings (V7PRO-837)
01jan14,mpc  updated for VxBus GEN2 (US22546)
11apr13,y_y  clean the gun build warnings.
02jul12,fao  remove ivPpc.h. (WIND00359801)
24jun12,fao  cleaned up
14jun12,fao  initial creation.
*/

/*
DESCRIPTION
This is the driver for the PCF I2C serial Real-Time Clock.

This driver supports the PCF8564 I2C RTC device. Other I2C real-time
clock devices with identical registers such as PCF8563 should also be able to
use this driver. Due to no hardware validation, the alarm function is not
supported.

The PCF8564 RTC devices use 7 distinct registers, addressable as follows to
define the date and time:

\cs
                                ADDRESS FUNCTION        RANGE
#define RTC_SEC_REG             0x2     Seconds         00-59
#define RTC_MIN_REG             0x3     Minutes         00-59
#define RTC_HOUR_REG            0x4     Hours           00-23
#define RTC_DAY_REG             0x5     Day             01-31
#define RTC_WEEKDAY_REG         0x6     weekDays        01-7
#define RTC_MONTH_REG           0x7     Month           01-12
#define RTC_YEAR_REG            0x8     Year            00-99
\ce

To get or set the time, this driver uses one I2C transfer to get or set all
the second, minute, hour, day, date, month and year registers. To make the RTC
starts from the time set more accurate, this driver stops the RTC before setting
and starts again the RTC after setting the time registers.

To add the driver to the vxWorks image, add the following component to the
kernel configuration:

\cs
vxprj component add DRV_I2C_PCFRTC
\ce

This I2c RTC device should be bound to a device tree node which requires
below properties:

\cs
compatible:     Specify the programming model for the device.
                It should be set to "nxp,pcf8564" and is used
                by vxbus GEN2 for device driver selection.

reg:            Specify the address of the device's resources within
                the address space defined by its parent bus.
\ce

Below is an example:

\cs
        rtc@51
            {
            compatible = "nxp,pcf8564";
            reg = <0x51>;
            };
\ce

INCLUDE FILES: vxBus.h vxbI2cLib.h vxbRtcLib.h
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <private/timeP.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbI2cLib.h>
#include "vxbI2cPcfRtc.h"
#include <hwif/drv/resource/vxbRtcLib.h>

/* define */
#define PCFRTC_DEBUG
//#undef PCFRTC_DEBUG
#ifdef  PCFRTC_DEBUG
#undef LOCAL
#define LOCAL
#include <private/kwriteLibP.h>     /* _func_kprintf */

#define RTC_DBG_ERR         0x00000001
#define RTC_DBG_INFO        0x00000002
#define RTC_DBG_ALL         0xffffffff
#define RTC_DBG_OFF         0x00000000

LOCAL UINT32 rtcDbgMask = RTC_DBG_ERR;

#undef PCFRTC_MSG
#define PCFRTC_MSG(mask, ...)                                   \
    do                                                          \
        {                                                       \
        if ((rtcDbgMask & (mask)) || ((mask) == RTC_DBG_ALL))   \
            {                                                   \
            if (_func_kprintf != NULL)                          \
                {                                               \
                (* _func_kprintf)(__VA_ARGS__);                 \
                }                                               \
            }                                                   \
        }                                                       \
    while ((FALSE))
#else
#undef PCFRTC_MSG
#define PCFRTC_MSG(...)
#endif  /* PCFRTC_DEBUG */

/* typedef */

UINT32 rtc_low_volt = 0;

typedef struct i2cRtcCtrl
    {
    VXB_DEV_ID  pDev;
    UINT16      i2cAddr;
    }I2C_RTC_CTRL;

/* forward declarations */

LOCAL STATUS vxbI2cPcfRtcTimeGet (VXB_DEV_ID, struct tm * rtcTime);
LOCAL STATUS vxbI2cPcfRtcTimeSet (VXB_DEV_ID, struct tm * rtcTime);
LOCAL STATUS vxbI2cPcfRtcProbe (struct vxbDev * pDev);
LOCAL STATUS vxbI2cPcfRtcAttach (struct vxbDev * pDev);
LOCAL STATUS vxbI2cPcfRtcAlarmSet
    (
    VXB_DEV_ID     pInst, 
    UINT8          alarmNum, 
    struct tm    * rtc_time,
    RTC_ALARM_FUNC rtcAlarmFunc,
    void         * pArg
    );
LOCAL STATUS vxbI2cPcfRtcAlarmGet
    (
    VXB_DEV_ID     pInst, 
    UINT8          alarmNum,
    struct tm *    rtc_time
    );

LOCAL VXB_DRV_METHOD vxbI2cPcfRtc_methods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe), vxbI2cPcfRtcProbe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), vxbI2cPcfRtcAttach },
    VXB_DEVMETHOD_END
};

VXB_DRV vxbFdtI2cPcfRtcDrv =
    {
    { NULL },
    PCF_RTC_NAME,         /* Name */
    "NXP PCF8564 RTC",    /* Description */
    VXB_BUSID_FDT,        /* Class */
    0,                    /* Flags */
    0,                    /* Reference count */
    vxbI2cPcfRtc_methods  /* Method table */
    };

LOCAL const VXB_FDT_DEV_MATCH_ENTRY i2cRtcMatch[] =
    {
        {
        "nxp,pcf8564",  /* compatible */
        NULL            /* no configuration */
        },
        {} /* Empty terminated list */
    };

/* month days in one year */

LOCAL const int month_days[12] = 
    {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

VXB_DRV_DEF(vxbFdtI2cPcfRtcDrv)

/******************************************************************************
*
* vxbI2cPcfRtcProbe - probe for device presence at specific address
*
* Check for PCF RTC contoller (or compatible) device at the specified
* base address. We assume one is present at that address, but we need to verify.
*
* RETURNS: OK if probe passes and assumed a valid PCF RTC
* (or compatible) device. ERROR otherwise.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcProbe
    (
    struct vxbDev * pDev /* Device information */
    )
    {
    return vxbFdtDevMatch (pDev, i2cRtcMatch, NULL);
    }

/******************************************************************************
*
* vxbI2cPcfRtcAttach - attach the PCF RTC device
*
* This is the PCF RTC initialization routine.
*
* RETURNS: OK, or ERROR if the initialization failed.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcAttach
    (
    VXB_DEV_ID          pDev
    )
    {
    VXB_I2C_RTC_FUNC *  i2cRtc  = NULL;
    VXB_RESOURCE_ADR *  pResAdr = NULL;
    VXB_RESOURCE *      pRes    = NULL;
    I2C_RTC_CTRL *      pDrvCtrl;

    pDrvCtrl = (I2C_RTC_CTRL *) vxbMemAlloc (sizeof (I2C_RTC_CTRL));
    if (pDrvCtrl == NULL)
        return ERROR;

    /* retrive the i2cAddress */

    pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
    if (pRes == NULL)
        {
        goto errOut;
        }
    pResAdr = (VXB_RESOURCE_ADR *) pRes->pRes;
    if (pResAdr == NULL)
        {
        goto errOut;
        }
    pDrvCtrl->i2cAddr = (UINT16)pResAdr->virtual;

    i2cRtc = vxbMemAlloc (sizeof (VXB_I2C_RTC_FUNC));
    if (i2cRtc == NULL)
        {
	goto errOut;
	}

    pDrvCtrl->pDev = pDev;
    vxbDevSoftcSet (pDev, pDrvCtrl);

    i2cRtc->rtcGet = vxbI2cPcfRtcTimeGet;
    i2cRtc->rtcSet = vxbI2cPcfRtcTimeSet;
    i2cRtc->alarmGet = vxbI2cPcfRtcAlarmGet;
    i2cRtc->alarmSet = vxbI2cPcfRtcAlarmSet;
    i2cRtc->alarmNum = 1;
    i2cRtc->alarmCap[0] = RTC_ALARM_SEC_SUPPORT;
    rtcRegister (pDev, i2cRtc);
    return OK;

errOut:

    if (i2cRtc != NULL)
        vxbMemFree (i2cRtc);
    if (pRes != NULL)
        (void)vxbResourceFree (pDev, pRes);
    vxbMemFree (pDrvCtrl);
    return ERROR;
    }

/******************************************************************************
*
* vxbI2cPcfRtcAlarmGet - get alarm time
*
* This feature is not supported, so just return ERROR. 
*
* RETURNS: always ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcAlarmGet
    (
    VXB_DEV_ID      pDev, 
    UINT8           alarmNum,
    struct tm     * pAlarmTime
    )
    {
    PCFRTC_MSG (RTC_DBG_INFO, "RTC alarm function is not supported\n");
    return ERROR;
    }

/******************************************************************************
*
* vxbI2cPcfRtcAlarmSet - set alarm time
*
* This feature is not supported, so just return ERROR. 
*
* RETURNS: always ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcAlarmSet
    (
    VXB_DEV_ID      pDev, 
    UINT8           alarmNum, 
    struct tm *     pAlarmTime,
    RTC_ALARM_FUNC  rtcAlarmFunc,
    void      *     pArg
    )
    {
    PCFRTC_MSG (RTC_DBG_INFO, "RTC alarm function is not supported\n");
    return ERROR;
    }

/*******************************************************************************
*
* vxbI2cPcfRtcRead - read from RTC PCF8564
*
* This routine performs the I2C read operation for RTC PCF8564.
*
* RETURNS: OK, or ERROR if read failed.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcRead
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
    I2C_RTC_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg[2];
    UINT8               msgbuf;

    memset (msg, 0, sizeof (I2C_MSG) * 2);

    msgbuf      = offset;
    msg[0].addr = pDrvCtrl->i2cAddr;
    msg[0].scl  = FAST_MODE;
    msg[0].buf  = &msgbuf;
    msg[0].len  = 1;

    msg[1].addr = pDrvCtrl->i2cAddr;
    msg[1].scl  = FAST_MODE;
    msg[1].buf  = buf;
    msg[1].len  = length;
    msg[1].flags  = I2C_M_RD;
    msg[1].wrTime = I2C_RTC_WRDELAY;

    return (vxbI2cDevXfer (pDev, msg, 2));
    }

/******************************************************************************
*
* vxbI2cPcfRtcWrite - write into RTC PCF8564
*
* This routine performs the I2C write operation for RTC PCF8564.
*
* RETURNS: OK, or ERROR if write failed.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcWrite
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
    I2C_RTC_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg;
    UINT8               writeBuf[16];

    memset (&msg, 0, sizeof (I2C_MSG));

    msg.addr    = pDrvCtrl->i2cAddr;
    msg.buf     = writeBuf;
    msg.scl     = FAST_MODE;
    msg.flags   = 0;
    msg.buf[0]  = offset;
    msg.len     = length + 1;
    msg.wrTime  = I2C_RTC_WRDELAY;
    memcpy (&msg.buf[1], buf, length);

    return (vxbI2cDevXfer (pDev, &msg, 1));
    }

/*******************************************************************************
*
* vxbI2cPcfRtcDateCheck - check whether date/time values are valid
*
* This routine tests the validity of the values in the tm structure.
*
* RETURNS: OK, or ERROR if any values are invalid.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcDateCheck
    (
    VXB_DEV_ID  pDev,
    struct tm * rtcTime         /* pointer to time keeping structure */
    )
    {
    int         monthday = 0;

    if (NULL == rtcTime)
        return ERROR;

    /* Check validity of year value */

    if (rtcTime->tm_year < MIN_YEAR || rtcTime->tm_year > MAX_YEAR)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "ERROR: year should be between 100 and 200\n");
        return (ERROR);
        }

    /* Check validity of month value */

    if (rtcTime->tm_mon + 1 < JANUARY || rtcTime->tm_mon + 1 > DECEMBER)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "ERROR: month should be between 1 and 12\n");
        return (ERROR);
        }

    /* Check validity of day of month value */

    if (IS_LEAP_YEAR (rtcTime->tm_year + 1900) &&
	(FEBRUARY == rtcTime->tm_mon + 1))
        monthday = 29;
    else
        monthday = month_days[rtcTime->tm_mon];

    if (rtcTime->tm_mday < 1 || rtcTime->tm_mday > monthday)
        {
        PCFRTC_MSG (RTC_DBG_ERR,
                    "ERROR: mDay should be between 1 and %d\n", monthday);
        return (ERROR);
        }

    /* Check validity of hours value */

    if (rtcTime->tm_hour > 23)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "ERROR: hour should be less than 24\n");
        return (ERROR);
        }

    /* Check validity of minutes value */

    if (rtcTime->tm_min > 59)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "ERROR: minute should be less than 60\n");
        return (ERROR);
        }

    /* Check validity of seconds value */
    if (rtc_low_volt != 0)
	{    	
    	rtcTime->tm_sec &= ~(RTC_SEC_REG_VL);
        PCFRTC_MSG (RTC_DBG_INFO, "RTC RtcDateCheck\n");
	}

    if (rtcTime->tm_sec > 59)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "ERROR: second should be less than 60\n");
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* vxbI2cPcfRtcConvert - convert the year and month to days
*
* This routine allows the caller to convert the year and month to days.
*
* RETURNS: the days converted.
*
* ERRNO: N/A
*/

LOCAL int vxbI2cPcfRtcConvert
    (
    int startYear,
    int endYear,
    int month
    )
    {
    int i = 0;
    int leapyear = 0;
    int dayspriormonths = 0;

    /* count leap years since epoch */

    for (i = startYear; i < endYear; i++)
        {
        if (IS_LEAP_YEAR (i))
            leapyear++;
        }

    /* count days between start of year and end of prior month */

    for (i = 1; i < month; i++)
        dayspriormonths += month_days[i - 1];

    /* leapday */

    if (IS_LEAP_YEAR (endYear) && (FEBRUARY < month))
        dayspriormonths++;

    return ((dayspriormonths + leapyear) + (endYear - startYear) * DAYS_IN_YEAR);
    }

/*******************************************************************************
*
* vxbI2cPcfRtcTimeSet - set the RTC's date/time per caller's values
*
* This routine allows the caller to set the RTC time and date. The caller
* must allocate space for an tm structure, fill the structure
* with the desired time and date, and call this routine.
*
* RETURNS: OK, or ERROR if failed to set the date/time.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcTimeSet
    (
    VXB_DEV_ID          pDev,
    struct tm *         rtcTime   /* pointer to time keeping structure */
    )
    {
    I2C_RTC_CTRL *      pDrvCtrl;
    UINT8               i2cBuff[7];
    int                 day = 0;
    UINT8               ctrl;

    if (pDev == NULL)
	return ERROR;

    pDrvCtrl = (I2C_RTC_CTRL *) vxbDevSoftcGet (pDev);
    if (pDrvCtrl == NULL)
	return ERROR;

    /* Determine whether date/time values are valid */

    if (vxbI2cPcfRtcDateCheck (pDev, rtcTime) != OK)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "vxbI2cPcfRtcDateCheck failed\n");
        return ERROR;
        }

    /* stops the RTC before setting */

    ctrl = RTC_CTR1_REG_STOP;
    if (vxbI2cPcfRtcWrite (pDev, &ctrl, RTC_CTR1_REG, 1) != OK)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "stops the RTC failed\n");
        return ERROR;
        }

    /* debug info */

    PCFRTC_MSG (RTC_DBG_INFO, "vxbI2cPcfRtcTimeSet:\n");
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_year = %04d\n", rtcTime->tm_year);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_mon  = %02d\n", rtcTime->tm_mon);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_mday = %02d\n", rtcTime->tm_mday);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_hour = %02d\n", rtcTime->tm_hour);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_min  = %02d\n", rtcTime->tm_min);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_sec  = %02d\n", rtcTime->tm_sec);

    i2cBuff[0] = BIN_TO_BCD (rtcTime->tm_sec);     /* seconds */
    /*clear bit VL, low voltage */
    if (rtc_low_volt != 0)
	{    	
    	i2cBuff[0] &= ~(RTC_SEC_REG_VL);
    	rtc_low_volt = 0;
        PCFRTC_MSG (RTC_DBG_INFO, "RTC RtcTimeSet\n");
	}

    i2cBuff[1] = BIN_TO_BCD (rtcTime->tm_min);     /* minutes */
    i2cBuff[2] = BIN_TO_BCD (rtcTime->tm_hour);    /* hours   */
    i2cBuff[3] = BIN_TO_BCD (rtcTime->tm_mday);    /* dayOfMon */
    i2cBuff[5] = BIN_TO_BCD (rtcTime->tm_mon + 1); /* month   */

    /* For RTC, the years range are 0-99 */

    i2cBuff[6] = BIN_TO_BCD (rtcTime->tm_year - 100); /* year */

    /* conver the previous year and month to days */

    day = vxbI2cPcfRtcConvert (2000, rtcTime->tm_year + 1900,
	                       rtcTime->tm_mon + 1) + rtcTime->tm_mday;

    /* starts from 2000/1/1, the init weekday is Saturday */

    i2cBuff[4] = (UINT8)((((day) % DAY_IN_WEEK) + INIT_WEEKDAY) % DAY_IN_WEEK);
    

    /* write 7 bytes */

    if (vxbI2cPcfRtcWrite (pDev, i2cBuff, RTC_SEC_REG, 7) != OK)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "starts the RTC failed\n");
        return ERROR;
        }

    /* starts the RTC after setting */

    ctrl = 0;
    if (vxbI2cPcfRtcWrite (pDev, &ctrl, RTC_CTR1_REG, 1) != OK)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "starts the RTC failed\n");
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* vxbI2cPcfRtcTimeGet - get the current RTC date/time
*
* This routine allows the caller to obtain the current RTC time and date.
* The caller must allocate space for an tm structure, then call
* this routine.
*
* RETURNS: OK, or ERROR if unable to retrieve the current RTC date and time.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbI2cPcfRtcTimeGet
    (
    VXB_DEV_ID  pDev,
    struct tm * rtcTime   /* pointer to time keeping structure */
    )
    {
    UINT8       i2cBuff[7] = {0};

    PCFRTC_MSG (RTC_DBG_INFO, "vxbI2cPcfRtcTimeGet:\n");

    /* read 7 bytes */

    if (vxbI2cPcfRtcRead (pDev, i2cBuff, RTC_SEC_REG, 7) != OK)
        {
        PCFRTC_MSG (RTC_DBG_ERR, "vxbI2cPcfRtcRead failed\n");
        return ERROR;
        }

//    if ((i2cBuff[RTC_SEC_REG] & RTC_SEC_REG_VL) != 0)
    if ((i2cBuff[0] & RTC_SEC_REG_VL) != 0)
        {
    	
    	rtc_low_volt = 1;
        PCFRTC_MSG (RTC_DBG_INFO, "RTC RtcTimeGet\n");

    	
        PCFRTC_MSG (RTC_DBG_ERR,
                    "vxbI2cPcfRtcTimeGet: Low voltage detected, the"
                    "RTC date/time is not reliable!\n");
        }

    rtcTime->tm_sec  = BCD_TO_BIN (i2cBuff[0] & 0x7F);
    rtcTime->tm_min  = BCD_TO_BIN (i2cBuff[1] & 0x7F);
    rtcTime->tm_hour = BCD_TO_BIN (i2cBuff[2] & 0x3F);
    rtcTime->tm_mday = BCD_TO_BIN (i2cBuff[3] & 0x3F);
    rtcTime->tm_wday = i2cBuff[4] & 0x07;

    /* The range of tm_mon is 0-11 but in RTC, the range is 1-12 */

    rtcTime->tm_mon  = BCD_TO_BIN (i2cBuff[5] & 0x1F) - 1;

    /*
     * Assumes 20YY not 19YY, but ansi-time years since 1900, so add 100.
     * Ignores CENTURY Bit in month register for general.
     */

    rtcTime->tm_year = BCD_TO_BIN (i2cBuff[6]) + 100;

    /* __jullday needs years since TM_YEAR_BASE */

    rtcTime->tm_yday = __julday (rtcTime->tm_year,
                                 rtcTime->tm_mon, rtcTime->tm_mday);

    /* debug info */

    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_year = %04d\r\n",
                rtcTime->tm_year + TM_YEAR_BASE);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_mon  = %02d\r\n",
                rtcTime->tm_mon + 1);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_mday = %02d\r\n", rtcTime->tm_mday);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_hour = %02d\r\n", rtcTime->tm_hour);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_min  = %02d\r\n", rtcTime->tm_min);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_sec  = %02d\r\n", rtcTime->tm_sec);
    PCFRTC_MSG (RTC_DBG_INFO, "rtcTime->tm_wday = %02d\r\n", rtcTime->tm_wday);

    return (OK);
    }

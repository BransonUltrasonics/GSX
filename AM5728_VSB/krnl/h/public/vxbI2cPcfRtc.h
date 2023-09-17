/* vxbI2cPcfRtc.h - header file for I2C PCF RTC driver */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
17jul15,sye  set and clear stop bit when setting the rtc time. (V7PRO-2099)
08aug14,x_z  fixed compiling wranings. (US40508)
01mar14,mpc  created from 01b (US22546)
*/

#ifndef __INCvxI2cPcfRtch
#define __INCvxI2cPcfRtch

#ifdef __cplusplus
extern "C" {
#endif

#define PCF_RTC_NAME            "pcf-rtc"

/* RTC register addresses */

#define RTC_CTR1_REG            0x0
#define RTC_CTR2_REG            0x1
#define RTC_SEC_REG             0x2
#define RTC_MIN_REG             0x3
#define RTC_HOUR_REG            0x4
#define RTC_DAY_REG             0x5
#define RTC_WEEKDAY_REG         0x6
#define RTC_MONTH_REG           0x7
#define RTC_YEAR_REG            0x8

#define RTC_MIN_ALARM_REG       0x9
#define RTC_HOUR_ALARM_REG      0xA
#define RTC_DAY_ALARM_REG       0xB
#define RTC_WEEKDAY_ALARM_REG   0xC

#define RTC_CTR1_REG_STOP       (1 << 5)
#define RTC_SEC_REG_VL          (1 << 7)

/*
 * The year register only can save 00-99, so the BASE_YEAR is needed,
 * Currently, setting the BASE_YEAR as 2000.
 */

#define MIN_YEAR                100
#define MAX_YEAR                199
#define INIT_WEEKDAY            5
#define I2C_RTC_WRDELAY         2       /* tBuf 1.3 us */

/* misc defines */

#define DAYS_IN_YEAR             365
#define DAY_IN_WEEK              7
#define JANUARY                  1
#define FEBRUARY                 2
#define DECEMBER                 12
#define IS_LEAP_YEAR(year)      (((year % 4 == 0) && (year % 100 != 0)) \
                                || year % 400 == 0)

/*
 * the following macros convert from BCD to binary and back.
 * Be careful that the arguments are chars, only char width returned.
 */

#define BCD_TO_BIN(bcd) (( ((((bcd) & 0xf0) >> 4) * 10) + ((bcd) & 0xf) ) & 0xff)
#define BIN_TO_BCD(bin) ((UINT8)(( (((bin) / 10) << 4) + ((bin) % 10) ) & 0xff))

#ifdef __cplusplus
}
#endif

#endif /* __INCvxI2cPcfRtch */

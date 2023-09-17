/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Common Data

***************************************************************************/
#ifndef COMMON_H
#define COMMON_H
#define SINT8  	char
#define SINT32 	long int
#define UINT16 quint16
/* *************** Macros for Configuration values ****************** */

#define SERVO		0
#define PNEUMATIC	1
#define NEPTUNE     1
#define DHD 		1
#define CHD			0
#define INTERNAL 	0
#define EXTERNAL	1
#define ENABLED		1
#define DISABLED	0
#define DASHBOARD	1
#define ENGLISH		0
#define METRIC		1
#define CONTINUE	1
#define EXIT		0
#define SCAN        0
#define SEEK        1
//Common Images
#define CUST_NAME_PART_ICON_IMG  "qrc:/images/image/part_icon_u135.png"

// Units defined commonly
#define CUST_NAME_TIME  QObject::tr("Time")
#define CUST_NAME_ENERGY  QObject::tr("Energy")
#define CUST_NAME_PEAK  QObject::tr("Peak power")
#define CUST_NAME_SCRUB  QObject::tr("Scrub Time")
#define CUST_NAME_ABSOLUTE  QObject::tr("Absolute Distance")
#define CUST_NAME_COLLAPSE  QObject::tr("Collapse Distance")
#define CUST_NAME_DYNAMIC  QObject::tr("Dynamic")
#define CUST_NAME_MULTI_MODE  QObject::tr("Multi Mode")

#define CUST_NAME_UINTSEC "s"
#define CUST_NAME_UINTMilliMeter "mm"
#define CUST_NAME_UINTINCHES "in"
#define CUST_NAME_UINTLBS "lbs"
#define CUST_NAME_UINTJOULES "J"
#define CUST_NAME_UINTWATTS "W"
#define CUST_NAME_MMPERSECOND "mm/s"
#define CUST_NAME_MMPERSECONDSQU "mm/s2"
#define CUST_NAME_UINIINSEC "in/s"
#define CUST_NAME_UINIINSECSQU "in/s2"
#define CUST_NAME_UNIT_NEWTON "N"

#define WELDMODE_TIME           "1"
#define WELDMODE_ENERGY         "2"
#define WELDMODE_PEAK_POWER     "3"
#define WELDMODE_GROUND_DETECT  "4"
#define WELDMODE_ABS_DISTANCE   "5"
#define WELDMODE_COL_DISTANCE   "6"
#define WELDMODE_DYNAMIC        "7"


#define INCHES "in"
#define MICROMETRE "mm"
#define SECONDS "s"
#define MINUTES "min"
#define NEWTONS "N"
#define LECIBALS "lbs"
#define PERCENTAGE "%"
#define KILOHERTZ "KHz"
#define KILO "K"

#define INCHPERSECOND "in/s"
#define JOULE "J"
#define WATT "W"
#define HERTZ "Hz"
#define DEGREE "Deg"

#define PRIVATE_KEY_SIZE 33
#define INVALID_KEY     QObject::tr("Invalid Key")

#endif // COMMON_H

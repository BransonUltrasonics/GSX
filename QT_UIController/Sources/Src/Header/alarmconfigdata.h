/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Alarm Configuration Data

***************************************************************************/
#ifndef ALARMCONFIGDATA_H
#define ALARMCONFIGDATA_H

struct AlarmActions                     /* Alarm Actions to be performed */
{
    quint8  ResetRequired       :      1;
    quint8  LogAlarm            :      1;
    quint8  GeneralAlarm        :      1;
    quint8  CustomAlarmOutput   :      1;
    quint8  CycleCounter        :      1;
    quint8  Reserved            :      3;

};

struct AlarmManagementData
{
    AlarmActions  OverloadsAlarmActions;
    AlarmActions  CycleModifiedAlarmActions;
    AlarmActions  WarningsAlarmActions;
    AlarmActions  SuspectAlarmActions;
    AlarmActions  RejectAlarmActions;
    AlarmActions  NoCycleAlarmActions;
    AlarmActions  HardwareFailureAlarmActions;
    AlarmActions  NonCycleOverloadsAlarmActions;
};

#endif // ALARMCONFIGDATA_H

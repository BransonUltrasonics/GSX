/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Alarm Configuration

***************************************************************************/
#ifndef ALARMCONFIGURATION_H
#define ALARMCONFIGURATION_H
#include <QObject>
#include <QList>
#include <QQmlContext>

#include "Header/alarmconfigcolumn.h"
#define ALARM_CONFIG_RESET_REQUIRED            QObject::tr("Reset\nRequired")
#define ALARM_CONFIG_LOG_ALARM                 QObject::tr("Log\nAlarm")
#define ALARM_CONFIG_GENERAL_ALARM             QObject::tr("General\nAlarm")
#define ALARM_CONFIG_CUSTOM_ALARM_REQUIRED     QObject::tr("Custom\nAlarm\nOutput")
#define ALARM_CONFIG_CYCLE_COUNTER             QObject::tr("Cycle\nCounter")
#define ALARM_CONFIG_OVERLOADS                 QObject::tr("Overloads")
#define ALARM_CONFIG_CYCLE_MODIFIED            QObject::tr("Cycle Modified")
#define ALARM_CONFIG_WARNINGS                  QObject::tr("Warnings")
#define ALARM_CONFIG_SUSPECT                   QObject::tr("Suspect")
#define ALARM_CONFIG_REJECT                    QObject::tr("Reject")
#define ALARM_CONFIG_NO_CYCLE                  QObject::tr("No Cycle")
#define ALARM_CONFIG_HARDWARE_FAILURE          QObject::tr("Hardware Failure")
#define ALARM_CONFIG_NO_CYCLE_OVERLOADS        QObject::tr("Non-Cycle Overloads")

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

enum AlarmActionsNumber
{
    OverloadsAlarmActions_ResetRequired = 0,
    OverloadsAlarmActions_LogAlarm,
    OverloadsAlarmActions_GeneralAlarm,
    CycleModifiedAlarmActions_ResetRequired,
    CycleModifiedAlarmActions_LogAlarm,
    CycleModifiedAlarmActions_GeneralAlarm,
    WarningsAlarmActions_ResetRequired,
    WarningsAlarmActions_LogAlarm,
    WarningsAlarmActions_GeneralAlarm,
    SuspectAlarmActions_ResetRequired,
    SuspectAlarmActions_LogAlarm,
    SuspectAlarmActions_GeneralAlarm,
    RejectAlarmActions_ResetRequired,
    RejectAlarmActions_LogAlarm,
    RejectAlarmActions_GeneralAlarm,
    NoCycleAlarmActions_ResetRequired,
    NoCycleAlarmActions_LogAlarm,
    NoCycleAlarmActions_GeneralAlarm,
    HardwareFailureAlarmActions_ResetRequired,
    HardwareFailureAlarmActions_LogAlarm,
    HardwareFailureAlarmActions_GeneralAlarm,
    NonCycleOverloadsAlarmActions_ResetRequired,
    NonCycleOverloadsAlarmActions_LogAlarm,
    NonCycleOverloadsAlarmActions_GeneralAlarm,
};

class AlarmConfiguration : public QObject
{
    Q_OBJECT

public:
    QList<QObject*> m_columnslist;
    QList<QObject*> m_columnslistdummy;
    QList<QObject*> m_rowslist;
    QList<QObject*> m_rowslistdummy;
    QList<QObject*> m_statuslist;
    QList<QObject*> m_statuslistdummy;
    char m_saveResult;

    explicit AlarmConfiguration(QObject *parent = 0);
    ~AlarmConfiguration();

    QString getAlarmRows();
    QString getAlarmcolums();
    void createalarmcolumns();
    void createAlarmRows();
    void structureCopy(AlarmManagementData);
    bool getStructureValue(int);
    Q_INVOKABLE bool saveToBL();
    Q_INVOKABLE void saveAlarmconfigDetails(int,bool);
    Q_INVOKABLE void resetDefaultValues();
    Q_INVOKABLE void readAlarmconfigDetails();
    Q_INVOKABLE void cancelCurrentDetails();

private:
    AlarmManagementData datatoBl;
    AlarmManagementData datatoBldummy;
    AlarmManagementData readstructure;
    AlarmManagementData readstructuredummy;
    AlarmManagementData tempStructure;
    AlarmManagementData tempStructureDummy;
    bool saveStatus;
};

#endif // ALARMCONFIGURATION_H

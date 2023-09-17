/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Alarm Configuration

***************************************************************************/
#ifndef ALARMCONFIGCOLUMN_H
#define ALARMCONFIGCOLUMN_H
#include <QObject>

class Alarmconfigcolumn : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString RowName READ getRowName WRITE setRowName NOTIFY rowNameChanged)
    Q_PROPERTY(bool CheckboxStatus READ getAlarmCheckboxStatus WRITE setCheckboxStatus NOTIFY checkboxStatusChanged)

public:
    QString m_ParameterName;
    QString m_RowName;
    bool m_checkboxstatus;

    Alarmconfigcolumn(QObject *parent=0);
    Alarmconfigcolumn(const QString paramName, const QString rowName,bool status ,QObject *parent=0);

    QString getParameterName() const;
    QString getRowName() const;
    void setParameterName(const QString parameter);
    void setRowName(const QString parameter);
    void setCheckboxStatus(bool parameter);
    bool getAlarmCheckboxStatus();

signals:
    void parameterNameChanged();
    void rowNameChanged();
    void checkboxStatusChanged(); 
};

#endif // ALARMCONFIGCOLUMN_H

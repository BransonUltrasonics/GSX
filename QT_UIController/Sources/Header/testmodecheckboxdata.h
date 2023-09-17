/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Mode Checkbox Model

***************************************************************************/

#ifndef TESTMODECHECKBOXDATA_H
#define TESTMODECHECKBOXDATA_H

#include <QObject>

class TestModeCheckBoxData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString CheckBoxColor READ getCheckBoxColor  WRITE setCheckBoxColor)
    Q_PROPERTY(bool CheckBoxStatus READ getCheckBoxStatus  WRITE setCheckBoxStatus)
    Q_PROPERTY(QString ParamName READ getParamName  WRITE setParamName)
    Q_PROPERTY(QString ParamValue READ getParamValue  WRITE setParamValue)
    Q_PROPERTY(QString ParamUnit READ getParamUnit  WRITE setParamUnit)

public:
    explicit TestModeCheckBoxData( QObject *parent = nullptr);

    QString getCheckBoxColor() const;
    bool getCheckBoxStatus() const;
    QString getParamName() const;
    QString getParamValue() const;
    QString getParamUnit() const;

    TestModeCheckBoxData(QString paramName
                         , QString checkBoxColor
                         , bool checkBoxStatus
                         , QString paramValue
                         , QString paramUnit,QObject *parent=nullptr);



public slots:
    void setCheckBoxColor(const QString& boxColor);
    void setCheckBoxStatus(bool boxStatus);
    void setParamName(const QString& paramName);
    void setParamValue(const QString& paramValue);
    void setParamUnit(const QString& paramUnit);

private:
    QString m_ParamName;
    QString m_CheckBoxColor;
    bool m_CheckBoxStatus;
    QString m_ParamValue;
    QString m_ParamUnit;
};

#endif // TESTMODECHECKBOXDATA_H

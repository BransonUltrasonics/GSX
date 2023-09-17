/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Test Mode Progressbar Model

***************************************************************************/

#ifndef TESTMODEPROGRESSBAR_H
#define TESTMODEPROGRESSBAR_H

#include <QObject>

class TestModeProgressBar : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ParamName READ getParamName  WRITE setParamName NOTIFY paramNameChanged)
    Q_PROPERTY(double  ParamValue READ getParamValue WRITE setParamValue NOTIFY paramValueChanged)
    Q_PROPERTY(QString ParamUnit  READ getParamUnit WRITE setParamUnit NOTIFY paramUnitChanged)

public:
    explicit TestModeProgressBar(QObject *parent=nullptr);

    TestModeProgressBar(QString paramName
                        , double paramValue
                        , QString paramUnit,QObject *parent=nullptr);

public slots:
    void setParamName(const QString& paramName);
    void setParamValue(double paramValue);
    void setParamUnit(const QString& paramUnit);

public:
    QString getParamName() const;
    double getParamValue() const;
    QString getParamUnit() const;

signals:
    void paramNameChanged();
    void paramValueChanged();
    void paramUnitChanged();

private:
    QString m_ParamName;
    double m_ParamValue;
    QString m_ParamUnit;
};

#endif // TESTMODEPROGRESSBAR_H

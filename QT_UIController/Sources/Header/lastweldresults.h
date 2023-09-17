/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Last Weld Results

***************************************************************************/

#ifndef LASTWELDRESULTS_H
#define LASTWELDRESULTS_H
#include <QObject>

class LastWeldResults : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterValue READ getParameterValue WRITE setParameterValue NOTIFY parameterValueChanged)
    Q_PROPERTY(QString ParameterUnit READ getParameterUnit WRITE setParameterUnit NOTIFY parameterUnitChanged)

public:
    QString m_ParameterName;
    QString m_ParameterValue;
    QString m_ParameterUnit;

    LastWeldResults(QObject *parent=0);
    LastWeldResults(const QString name,const QString value,const QString unit, QObject *parent=0);

    QString getParameterName() const;
    void setParameterName(const QString name);
    QString getParameterValue() const;
    void setParameterValue(const QString value);
    QString getParameterUnit() const;
    void setParameterUnit(const QString unit);

signals:
    void parameterNameChanged();
    void parameterValueChanged();
    void parameterUnitChanged();
};
#endif // LASTWELDRESULTS_H

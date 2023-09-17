/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Pressure Parameter

***************************************************************************/

#ifndef PRESSUREPARAMETER_H
#define PRESSUREPARAMETER_H

#include <QObject>

class PressureParameter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)


public:
    PressureParameter(QObject *parent=0);
    PressureParameter(const QString name, QObject *parent=0);

    QString m_ParameterName;
    QString getParameterName() const;

    void setParameterName(const QString name);

signals:
    void parameterNameChanged();

};

#endif // PRESSUREPARAMETER_H


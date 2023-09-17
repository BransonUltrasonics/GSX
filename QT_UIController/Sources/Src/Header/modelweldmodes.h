/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Weld Mode Model

***************************************************************************/
#ifndef MODELWELDMODES_H
#define MODELWELDMODES_H

#include <QObject>

class WeldModes : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterLocaleName READ getParameterLocaleName WRITE setParameterLocaleName NOTIFY parameterLocaleNameChanged)


public:
    WeldModes(QObject *parent=0);
    WeldModes(const QString name, QObject *parent=0);

    QString m_ParameterName;
    QString getParameterName() const;
    void setParameterName(const QString parameter);
    QString m_ParameterLocaleName;
    QString getParameterLocaleName() const;
    void setParameterLocaleName(const QString parameter);

signals:
    void parameterNameChanged();
    void parameterLocaleNameChanged();

};

#endif // MODELWELDMODES_H

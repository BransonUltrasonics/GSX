/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Menu Limits

***************************************************************************/
#ifndef MENULIMITS_H
#define MENULIMITS_H
#include <QObject>

class MenuLimits : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ParameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString ParameterLocaleName READ getParameterLocaleName WRITE setParameterLocaleName NOTIFY parameterLocaleNameChanged)

public:
    QString m_ParameterName;
    QString m_ParameterLocaleName;

    MenuLimits(QObject *parent=0);
    MenuLimits(const QString name, const QString Localename,QObject *parent=0);

    QString getParameterName() const;
    void setParameterName(const QString parameter);

    QString getParameterLocaleName() const;
    void setParameterLocaleName(const QString parameter);

signals:
    void parameterNameChanged();
    void parameterLocaleNameChanged();
};

#endif // MENULIMITS_H

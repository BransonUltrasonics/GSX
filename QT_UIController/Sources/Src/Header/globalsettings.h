/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Global Settings

***************************************************************************/

#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>

class GlobalSettings : public QObject
{
    Q_OBJECT

 public:
    explicit GlobalSettings(QObject *parent = nullptr);

    Q_PROPERTY(QString passwordExpiryInDays READ passwordExpiryInDays WRITE setPasswordExpiryInDays NOTIFY passwordExpiryInDaysChanged)
    Q_PROPERTY(QString idealLogoffTime READ idealLogoffTime WRITE setIdealLogoffTime NOTIFY idealLogoffTimeChanged)

    Q_INVOKABLE void handleGlobalSettings();
    Q_INVOKABLE void updateGlobalSettings(QString passwordExpiry, QString idealLogoffTime);
    void setPasswordExpiryInDays(QString passwrodExpiry);
    void setIdealLogoffTime(QString idealLogoffTime);
    QString passwordExpiryInDays() const;
    QString idealLogoffTime() const;

signals:
    void passwordExpiryInDaysChanged();
    void idealLogoffTimeChanged();

private:
    QString m_passwordExpiry;
    QString m_idealLogoffTime;
};

#endif // GLOBALSETTINGS_H

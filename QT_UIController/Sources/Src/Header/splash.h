/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Splash Screen

***************************************************************************/
#ifndef SPLASH_H
#define SPLASH_H
#include <QObject>

class Splash:public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString SystemStatus MEMBER m_SystemReadyStatus NOTIFY SystemReadyStatusChanged )
public:
    Splash(QObject *parent = nullptr);
    Q_INVOKABLE int getSystemReadyStatus();
    Q_INVOKABLE int getSCStatus();
public slots:
    void systemReadyStatusChanged(QString a_ReqBuffer);

private:
    QString m_SystemReadyStatus;
    int m_SCStatus;

};

#endif // SPLASH_H

/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

IP Configuration

***************************************************************************/
#ifndef IPCONFIGURATION_H
#define IPCONFIGURATION_H

#include <QObject>
#include <QString>
#include <QQmlContext>

struct IP_Configuraion
{
    char IPAddr[4];
    char SubNetmask[4];
    char GatewayAddr[4];
};

class IpConfiguration: public QObject
{
    Q_OBJECT
public:
    IP_Configuraion stIPDetails;
    IP_Configuraion stIPDetailsDummy;
    bool bIPFormat;
    bool bSubNetFormat;

    explicit IpConfiguration(QObject *parent = nullptr);

    Q_INVOKABLE int saveIPv4Details();
    Q_INVOKABLE void saveChangedValue(QString qstrAddressText,int index);
    Q_INVOKABLE void readIPv4Details();
    Q_INVOKABLE QString getIP();
    Q_INVOKABLE QString getSubnetMask();
    Q_INVOKABLE QString getGatewayAddr();
};

#endif // IPCONFIGURATION_H

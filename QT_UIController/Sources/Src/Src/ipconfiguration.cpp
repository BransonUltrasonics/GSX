/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

IP Configuration

***************************************************************************/
#include <QQuickView>
#include <QQmlContext>
#include <QString>
#include "Header/configuration.h"
#include "Header/communicationinterface.h"
#include "Header/ipconfiguration.h"

extern CommunicationInterface CIObj;

/**
 * @brief IpConfiguration::IpConfiguration : Initializtion class member and create meomory
 * @param parent
 */
IpConfiguration::IpConfiguration(QObject *parent) : QObject(parent)
{
    bIPFormat=false;
    bSubNetFormat=false;
}

/**
 * @brief IpConfiguration::saveChangedValue : Validating and save the IP address value
 * @param qstrAddressText
 * @param index
 */
void IpConfiguration::saveChangedValue(QString qstrAddressText,int index)
{
    QStringList temp = qstrAddressText.split(".");

    if(index == 0)
    {
        if(temp.size()<4)
        {
            bIPFormat = false;
            return;
        }
        bIPFormat = true;
        stIPDetails.IPAddr[0]=temp[0].toInt();
        stIPDetails.IPAddr[1]=temp[1].toInt();
        stIPDetails.IPAddr[2]=temp[2].toInt();
        stIPDetails.IPAddr[3]=temp[3].toInt();
    }
    else if(index == 1)
    {
        if(temp.size()<4)
        {
            bSubNetFormat = false;
            return;
        }
        bSubNetFormat = true;
        stIPDetails.SubNetmask[0]=temp[0].toInt();
        stIPDetails.SubNetmask[1]=temp[1].toInt();
        stIPDetails.SubNetmask[2]=temp[2].toInt();
        stIPDetails.SubNetmask[3]=temp[3].toInt();
    }
    else
    {
        if(temp.size()<4)
        {
            bSubNetFormat = false;
            return;
        }
        bSubNetFormat = true;
        stIPDetails.GatewayAddr[0]=temp[0].toInt();
        stIPDetails.GatewayAddr[1]=temp[1].toInt();
        stIPDetails.GatewayAddr[2]=temp[2].toInt();
        stIPDetails.GatewayAddr[3]=temp[3].toInt();
    }
}

/**
 * @brief IpConfiguration::saveIPv4Details
 * @return
 */
int IpConfiguration::saveIPv4Details()
{
    if(!bSubNetFormat || !bSubNetFormat)
    {
        return 0;
    }
    char *m_data=reinterpret_cast<char*>(&stIPDetails);
    CIObj.sendMessage(SCBL_IP_CONFIGURATION_WRITE_REQ,UIC_IP_CONFIGURATION_WRITE_RES,m_data,sizeof(stIPDetails));
    char ch[8]={'\0'};
    CIObj.recivedMessage(UIC_IP_CONFIGURATION_WRITE_RES,ch,7);
    return 1;
}

/**
 * @brief IpConfiguration::readIPv4Details
 */
void IpConfiguration::readIPv4Details()
{
    char ch[4]={'\0'};
    stIPDetails = {};
    char *m_data=reinterpret_cast<char*>(&stIPDetails);
    CIObj.sendMessage(SCBL_IP_CONFIGURATION_READ_REQ,UIC_IP_CONFIGURATION_READ_RES,ch,0);
    CIObj.recivedMessage(UIC_IP_CONFIGURATION_READ_RES,m_data,sizeof(stIPDetails));
}

/**
 * @brief IpConfiguration::getIP : Return IP address
 * @return
 */
QString IpConfiguration::getIP()
{
    UINT8 num = stIPDetails.IPAddr[0];
    QString retString = QString::number(num);
    retString.append(".");
    num = stIPDetails.IPAddr[1];
    retString.append(QString::number(num));
    retString.append(".");
    num = stIPDetails.IPAddr[2];
    retString.append(QString::number(num));
    retString.append(".");
    num = stIPDetails.IPAddr[3];
    retString.append(QString::number(num));
    return retString;
}

/**
 * @brief IpConfiguration::getSubnetMask : Return subnet mask
 * @return
 */
QString IpConfiguration::getSubnetMask()
{
    UINT8 num = stIPDetails.SubNetmask[0];
    QString retString = QString::number(num);
    retString.append(".");
    num = stIPDetails.SubNetmask[1];
    retString.append(QString::number(num));
    retString.append(".");
    num = stIPDetails.SubNetmask[2];
    retString.append(QString::number(num));
    retString.append(".");
    num = stIPDetails.SubNetmask[3];
    retString.append(QString::number(num));
    return retString;
}

/**
 * @brief IpConfiguration::getGatewayAddr : Return gateway address
 * @return
 */
QString IpConfiguration::getGatewayAddr()
{
    UINT8 num = stIPDetails.GatewayAddr[0];
    QString retString = QString::number(num);
    retString.append(".");
    num = stIPDetails.GatewayAddr[1];
    retString.append(QString::number(num));
    retString.append(".");
    num = stIPDetails.GatewayAddr[2];
    retString.append(QString::number(num));
    retString.append(".");
    num = stIPDetails.GatewayAddr[3];
    retString.append(QString::number(num));
    return retString;
}

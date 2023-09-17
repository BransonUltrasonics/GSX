/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to process firmware upgrade commands to BL

***************************************************************************/

#include "Header/firmwareupgrade.h"
#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

extern CommunicationInterface CIObj;
extern QQmlContext* context;
extern QObject * object;
extern bool g_bFirmwareUpgradeInProgress;

/**
 * @brief FirmwareUpgrade::FirmwareUpgrade : Constructor, Initialization of class members and memory for object
 * @param parent
 */
FirmwareUpgrade::FirmwareUpgrade(QObject *parent) : QObject(parent)
{
    qstrACUpgradeFilenames.clear();
    qstrPCUpgradeFilenames.clear();
    qstrSCUpgradeFilenames.clear();
    g_bFirmwareUpgradeInProgress = false;

    context->setContextProperty("firmwareUpgradeObj",this);
    QObject::connect(&CIObj , &CommunicationInterface::upgradeProgress , this , &FirmwareUpgrade::upgradeProgressStatusData);
    QObject::connect(&CIObj , &CommunicationInterface::upgradeResponse , this , &FirmwareUpgrade::upgradeResponseStatus);

    QObject::connect(&CIObj , &CommunicationInterface::firmwareUpgradeOverEthernet , this , &FirmwareUpgrade::upgradePopup);

    emit upgradeProgressDetail(Firmwareupgradetype::ACTUATOR_CONTROLLER,0);
    emit upgradeProgressDetail(Firmwareupgradetype::POWER_CONTROLLER,0);
    emit upgradeProgressDetail(Firmwareupgradetype::SUPERVISORY_CONTROLLER,0);

    qstrACUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
    qstrPCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
    qstrSCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);

}

/**
 * @brief FirmwareUpgrade::readUSB : Read Usb request will be sent to BL, when user clicks on read usb in
 *                                   software upgrade screen
 * @return
 */
int FirmwareUpgrade::readUSB()
{
    int nReadUSb = 0;
    int nFilesCounter = 0;
    int nSubFilesCounter = 0;
    QString Firmware_readusb_response;
    QString Firmware_readusb_request;


    emit upgradeProgressDetail(Firmwareupgradetype::ACTUATOR_CONTROLLER,0);
    emit upgradeProgressDetail(Firmwareupgradetype::POWER_CONTROLLER,0);
    emit upgradeProgressDetail(Firmwareupgradetype::SUPERVISORY_CONTROLLER,0);

    qstrACUpgradeFilenames.clear();
    qstrPCUpgradeFilenames.clear();
    qstrSCUpgradeFilenames.clear();

    /* Send message to BL */
    CIObj.sendMessage(SCBL_FW_UPGRADE_REQ,UIC_FW_UPGRADE_RES,Firmware_readusb_request);
    CIObj.recivedMessage(UIC_FW_UPGRADE_RES,Firmware_readusb_response);

    //    Sample response for testing //AC/PC/SC order
    //Firmware_readusb_response= QString("AC_UWBLV400.bin||AC_UWBLV500.bin||AC_UWBLV100.bin||SC_UWBLV600.bin||SC_UWBLV700.bin");
    //Firmware_readusb_response= QString("2");

    /* Error case */
    if(Firmware_readusb_response.length()==1)
    {
        nReadUSb = Firmware_readusb_response.toInt();

        //2 Means No files found in attached Device, in this case add the none option to the UI
        //if(nReadUSb==2)
        {
            qstrACUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
            qstrPCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
            qstrSCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        }

    }
    else
    {
        /* Read files from usb and seperates AC,PC,SC files and assigns to respective lists */
        nReadUSb = Firmware_readusb_response.length();
        if(nReadUSb > 0)
        {
            QStringList lstAllReceivedFiles= Firmware_readusb_response.split(FILEDELIMITER, QString::SkipEmptyParts);
            for(nFilesCounter=0;nFilesCounter<lstAllReceivedFiles.count();nFilesCounter++)
            {
                if( lstAllReceivedFiles[nFilesCounter].startsWith("AC_"))
                {
                    QStringList lstACfiles = lstAllReceivedFiles[nFilesCounter].split(FILESEPERATOR);
                    for(nSubFilesCounter=0;nSubFilesCounter<lstACfiles.count();nSubFilesCounter++)
                    {
                        qstrACUpgradeFilenames.append(lstACfiles[nSubFilesCounter]);
                    }
                }
                if( lstAllReceivedFiles[nFilesCounter].startsWith("PC_"))
                {
                    QStringList lstPCfiles = lstAllReceivedFiles[nFilesCounter].split(FILESEPERATOR);
                    for(nSubFilesCounter=0;nSubFilesCounter<lstPCfiles.count();nSubFilesCounter++)
                    {
                        qstrPCUpgradeFilenames.append(lstPCfiles[nSubFilesCounter]);
                    }
                }
                if( lstAllReceivedFiles[nFilesCounter].startsWith("SC_"))
                {
                    QStringList lstSCfiles = lstAllReceivedFiles[nFilesCounter].split(FILESEPERATOR);
                    for(nSubFilesCounter=0;nSubFilesCounter<lstSCfiles.count();nSubFilesCounter++)
                    {
                        qstrSCUpgradeFilenames.append(lstSCfiles[nSubFilesCounter]);
                    }
                }
            }//end of all files for loop
            //Sorting of files and reverse order to show the latest file on top
            qstrACUpgradeFilenames.sort();
            qstrPCUpgradeFilenames.sort();
            qstrSCUpgradeFilenames.sort();
            std::reverse(qstrACUpgradeFilenames.begin(), qstrACUpgradeFilenames.end());
            std::reverse(qstrPCUpgradeFilenames.begin(), qstrPCUpgradeFilenames.end());
            std::reverse(qstrSCUpgradeFilenames.begin(), qstrSCUpgradeFilenames.end());

            qstrACUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
            qstrPCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
            qstrSCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        }
    }

    return nReadUSb;
}

/**
 * @brief FirmwareUpgrade::getfileslist : Returns AC, PC and SC files list
 * @param nIdentifier
 * @return
 */
QStringList FirmwareUpgrade::getfileslist(int nIdentifier)
{
    QStringList strDefaultEmptyList;
    Firmwareupgradetype currentupgradetype=static_cast<FirmwareUpgrade::Firmwareupgradetype>(nIdentifier);
    switch (currentupgradetype)
    {
    case Firmwareupgradetype::ACTUATOR_CONTROLLER:
        if(qstrACUpgradeFilenames.count() ==0)
            qstrACUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        return qstrACUpgradeFilenames;
        break;
    case Firmwareupgradetype::POWER_CONTROLLER:
        if(qstrPCUpgradeFilenames.count() ==0)
            qstrPCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        return qstrPCUpgradeFilenames;
        break;
    case Firmwareupgradetype::SUPERVISORY_CONTROLLER:
        if(qstrSCUpgradeFilenames.count() ==0)
            qstrSCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        return qstrSCUpgradeFilenames;
        break;
    default:
        return strDefaultEmptyList;
        break;
    }
    return strDefaultEmptyList;
}

/**
 * @brief FirmwareUpgrade::upgradeFirmwares : Selected files from UI will be sent to BL
 *                                            to upgrade the firmware
 * @param nscIndex
 * @param nacIndex
 * @param npcIndex
 * @param upgradeType
 * @return
 */
QString FirmwareUpgrade::upgradeFirmwares(int nscIndex,int nacIndex,int npcIndex,int upgradeType)
{
    QString strReturn;
    QString strUpgradeRequest;
    strUpgradeRequest+= QString::number((!(qstrACUpgradeFilenames.at(nacIndex)==NO_FIRMWARE_UPGRADE))) + FILEDELIMITER;
    strUpgradeRequest+= QString::number((!(qstrPCUpgradeFilenames.at(npcIndex)==NO_FIRMWARE_UPGRADE))) + FILEDELIMITER;
    strUpgradeRequest+= QString::number((!(qstrSCUpgradeFilenames.at(nscIndex)==NO_FIRMWARE_UPGRADE))) + FILEDELIMITER;

    strReturn = strUpgradeRequest;
    strUpgradeRequest+=qstrACUpgradeFilenames.at(nacIndex) + FILEDELIMITER;
    strUpgradeRequest+=qstrPCUpgradeFilenames.at(npcIndex) + FILEDELIMITER;
    strUpgradeRequest+=qstrSCUpgradeFilenames.at(nscIndex) + FILEDELIMITER;

    emit upgradeProgressDetail(Firmwareupgradetype::ACTUATOR_CONTROLLER,0);
    emit upgradeProgressDetail(Firmwareupgradetype::POWER_CONTROLLER,0);
    emit upgradeProgressDetail(Firmwareupgradetype::SUPERVISORY_CONTROLLER,0);
    if(!upgradeType)
        CIObj.sendMessage(SCBL_FW_UPGRADE_CMD_REQ,SCBL_FW_UPGRADE_CMD_REQ,strUpgradeRequest);
    else
        CIObj.sendMessage(SCBL_FW_UPGRADE_ETHER_REQ,SCBL_FW_UPGRADE_ETHER_REQ,strUpgradeRequest);
    g_bFirmwareUpgradeInProgress = true;
    return strReturn;
}

/**
 * @brief FirmwareUpgrade::upgradeProgressStatusData : Updates the progress of AC, PC and SC
 *                                                     received from BL in UI
 * @param qstrBuffer
 */
void FirmwareUpgrade::upgradeProgressStatusData(QString qstrBuffer)
{
    if((qstrBuffer.length() > 0) && (qstrBuffer.length()==2))
    {
        QString strModule = qstrBuffer.at(0);
        QString strprogress = qstrBuffer.at(1);

        int nModule = strModule.toInt();
        int nProgress = strprogress.toInt();

        //Format of response is : 11 12 13 21 22 23 31 32 33
        //1 for AC 2 for PC 3 for PC
        emit upgradeProgressDetail(nModule,nProgress);
    }
}

/**
 * @brief FirmwareUpgrade::upgradeResponseStatus : Updates the status of upgrade, whether
 *                                                 upgrade is in progress or not
 * @param qstrResponseBuffer
 */
void FirmwareUpgrade::upgradeResponseStatus(QString qstrResponseBuffer)
{
    int nUpgradeResponse = qstrResponseBuffer.toInt();
    g_bFirmwareUpgradeInProgress = false;
    emit upgradeCompletionStatus(nUpgradeResponse);
}

/**
 * @brief FirmwareUpgrade::upgradePopup : This function is called, when upgrade over ethernet req
 *                                        comes to UI and opens an upgrade popup
 * @param upgradeFiles
 */
void FirmwareUpgrade::upgradePopup(QString upgradeFiles)
{
    qstrACUpgradeFilenames.clear();
    qstrPCUpgradeFilenames.clear();
    qstrSCUpgradeFilenames.clear();
    if(!upgradeFiles.isEmpty())
    {
        QStringList upgradeFilesList = upgradeFiles.split(",");
        for(int dwListCounter = 0; dwListCounter < upgradeFilesList.size(); dwListCounter++)
        {
            if(upgradeFilesList[dwListCounter].startsWith("AC_"))
            {
                qstrACUpgradeFilenames.append(upgradeFilesList[dwListCounter]);
            }
            else if(upgradeFilesList[dwListCounter].startsWith("PC_"))
            {
                qstrPCUpgradeFilenames.append(upgradeFilesList[dwListCounter]);
            }
            else if(upgradeFilesList[dwListCounter].startsWith("SC_"))
            {
                qstrSCUpgradeFilenames.append(upgradeFilesList[dwListCounter]);
            }
        }
        if(qstrACUpgradeFilenames.size() == 0)
        {
            qstrACUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        }
        if(qstrPCUpgradeFilenames.size() == 0)
        {
            qstrPCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        }
        if(qstrSCUpgradeFilenames.size() == 0)
        {
            qstrSCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        }
    }
    else
    {
        qstrACUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        qstrPCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
        qstrSCUpgradeFilenames.append(NO_FIRMWARE_UPGRADE);
    }
    QMetaObject::invokeMethod(object,"upgradePopupOpen");
}

/**
 * @brief FirmwareUpgrade::clearfilelist : Clears all the filesnames lists
 */
void FirmwareUpgrade::clearfilelist()
{
    qstrACUpgradeFilenames.clear();
    qstrPCUpgradeFilenames.clear();
    qstrSCUpgradeFilenames.clear();
}

/**
 * @brief FirmwareUpgrade::getUpgradeStatus : Returns upgrade status, whether upgrade is in progress or not
 * @return
 */
bool FirmwareUpgrade::getUpgradeStatus()
{
    return g_bFirmwareUpgradeInProgress;
}


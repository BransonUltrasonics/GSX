/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Communication Interface

***************************************************************************/
#include "Header/communicationinterface.h"
//#include "Header/clientsocket.h"
#include <QDebug>
#include <QDateTime>
#include <QTime>
#include "Header/configuration.h"
#include "Header/sysconfigdata.h"
#include "Header/customeventfilter.h"
#include "Header/helperenums.h"
#include "Header/splash.h"
#include <QFile>
#include <QTextStream>

extern CustomEventFilter *filter;
extern QObject *object;
extern Splash *splash;
extern CommunicationInterface CIObj;
static const int BACK_GROUND_THREAD_RUNNING = 1112;

/**
 * @brief CommunicationInterface::CommunicationInterface : Constructor, Initialization of class members and memory for object
 */
CommunicationInterface::CommunicationInterface()
{
    m_isConnected = false;
    dwWeldCounter = 0;
    SysConfigData::getInstance();
    start();
}

/**
 * @brief CommunicationInterface::~CommunicationInterface : Destructor
 */
CommunicationInterface::~CommunicationInterface()
{
    terminate();
}

/**
 * @brief CommunicationInterface::sendMessage
 * @param reqMessageID
 * @param resMessageID
 * @param Message
 */
int CommunicationInterface::sendMessage(int reqMessageID,int resMessageID, QString Message)
{
    if(!m_isConnected)
        return 1;
    if(reqMessageID>0)
        qDebug()<<"Request MessageID:" << reqMessageID;
    int dwLen = Message.size();
    char *ch = (char*)malloc(dwLen+10);
    memcpy(ch,&reqMessageID,4);
    memcpy(ch+4,&dwLen,4);
    memcpy(ch+8,Message.toStdString().c_str(),dwLen);
    dwLen = dwLen + 8;
    if(reqMessageID != resMessageID)
    {
        RecivedMsg stRecivedMsg;
        RecvBuffMap[resMessageID] = stRecivedMsg;
    }
    m_socket.Send(ch,dwLen);
    free(ch);
    return 0;
}

/**
 * @brief CommunicationInterface::sendMessage
 * @param reqMessageID
 * @param resMessageID
 * @param chMessage
 * @param length
 */
int CommunicationInterface::sendMessage(int reqMessageID,int resMessageID, char* chMessage, int length)
{
    if(!m_isConnected)
        return 1;
    if(reqMessageID>0)
        qDebug()<<"Request MessageID:" << reqMessageID;

    Message sendBuff;
    sendBuff.length = length;
    sendBuff.msgID = reqMessageID;
    memset(sendBuff.Buffer,0,512);
    memcpy(sendBuff.Buffer,chMessage,length);
    if(reqMessageID != resMessageID)
    {
        RecivedMsg stRecivedMsg;
        RecvBuffMap[resMessageID] = stRecivedMsg;
    }
    char *ch = reinterpret_cast<char*>(&sendBuff);
    if(length == 0)
        m_socket.Send(ch,8);
    else
        m_socket.Send(ch,4 + 4 + length);
    return 0;
}

/**
 * @brief CommunicationInterface::recivedMessage
 * @param MessageID
 * @param Message
 */
bool CommunicationInterface::recivedMessage(int MessageID, QString &Message)
{
    bool bRetVal = false;
    int dwTimeout = 500; //5 sec 5*1000/10 10 for every 10 mil sec loop is going onnn;
    if(MessageID == RES_GET_ALARM_LOG)
    {
        dwTimeout = 1000;
    }
    else if(MessageID == RES_GET_WELD_SIGNATURE_DATA)
    {
        dwTimeout = 2000; // Increasing to 20 sec as timeout for weld signature
    }
    else if (MessageID==RES_GET_LAST_WELD_HISTORY_DATA)
    {
        dwTimeout = 3000;
    }
    else if(MessageID == RES_GET_ANALYTICS_ALARM)
    {
        dwTimeout = 2000;
    }
    else if(MessageID == RES_COPY_DB)
    {
        dwTimeout = 0xFFFFF; //around 2hrs timeout
    }
    else if(MessageID == UIC_FW_UPGRADE_RES)
    {
        dwTimeout = 6000; //Increasing to 60 sec as timeout to read AC, PC, SC file names for upgrade
    }
    int dwReadyState = splash->getSystemReadyStatus();

    if((dwReadyState != 1 && MessageID != UIC_SYSCONFIG_READ_RES && MessageID != UIC_FEATURERUN_READ_RES && MessageID != RES_GET_ANALYTICS_ALARM && MessageID != RES_GET_USERIO_DATA) || m_isConnected == false)
    {
        dwTimeout = 50;
    }
    while(1)
    {
        if(RecvBuffMap[MessageID].isBuffReceived)
        {
            RecvBuffMap[MessageID].isBuffReceived = false;
            char *chTemp = RecvBuffMap[MessageID].pchBuff;
            Message = chTemp;
            RecvBuffMap[MessageID].pchBuff=NULL;
            free(chTemp);
            RecvBuffMap.remove(MessageID);
            bRetVal = true;
            break;
        }
        else
        {
            Sleep(10);
//            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            dwTimeout--;
        }
        if(dwTimeout)
            continue;
        else
        {
            RecvBuffMap.remove(MessageID);
            break;//timeout need to be handle
        }
    }
    if(MessageID>0)
        qDebug()<<"Received MessageID:" << MessageID;
    return bRetVal;
}

/**
 * @brief CommunicationInterface::recivedMessage
 * @param MessageID
 * @param Message
 * @param length
 */
bool CommunicationInterface::recivedMessage(int MessageID, char* Message, int length)
{
    bool bRetVal = false;
    int dwTimeout = 500; //5 sec 5*1000/10 10 for every 10 mil sec loop is going onnn;
    int dwReadyState = splash->getSystemReadyStatus();
    qDebug()<<"dwReadyState:::"<<dwReadyState;
    if((dwReadyState != 1 && MessageID != UIC_SYSCONFIG_READ_RES && MessageID != UIC_FEATURERUN_READ_RES && MessageID != UIC_ALARM_CONFIG_SAVE_RSP) || m_isConnected == false)
    {
        dwTimeout = 10;
    }
    if(MessageID == UIC_FW_UPGRADE_RES)
    {
        dwTimeout = 6000; //Increasing to 60 sec as timeout to read AC, PC, SC file names for upgrade
    }
    if(MessageID == UIC_TEST_ABORT_CMD_RES)
    {
        dwTimeout = 1000; //Increasing timeout to 10 sec so SC can have time to save data on DB before respinding to UI.
    }
    while(1)
    {
        if(RecvBuffMap[MessageID].isBuffReceived)
        {
            RecvBuffMap[MessageID].isBuffReceived = false;
            char *chTemp = RecvBuffMap[MessageID].pchBuff;
            memcpy(Message,chTemp,RecvBuffMap[MessageID].dwBuffLen);
            length = RecvBuffMap[MessageID].dwBuffLen;
            Message[length]=0;
            free(chTemp);
            RecvBuffMap[MessageID].pchBuff=NULL;
            RecvBuffMap.remove(MessageID);
            bRetVal = true;
            break;
        }
        else
        {
            Sleep(10);
            dwTimeout--;
        }
        if(dwTimeout)
            continue;
        else
        {
            RecvBuffMap.remove(MessageID);
            break;//timeout need to be handle
        }
    }
    if(MessageID>0)
        qDebug()<<"Received MessageID:" << MessageID;
    return bRetVal;
}

/**
 * @brief CommunicationInterface::run
 */
void CommunicationInterface::run()
{
    heartBeatObj.stratHeartBeat();
    partsPerMinObj.startPartsPerMin();
    QByteArray qByteResBuff ;
    while(1)
    {
        try
        {
            char buff[8]={'\0'};
            int dwReadLen = m_socket.ReadExact(buff,4);

            if(dwReadLen == 0)
            {
                continue; //Timeout or No data available on the socket
            }
            else if(dwReadLen < 0)
            {
                m_socket.Close();
                m_isConnected = false;
                qDebug()<<"Socket Closed";

                auto activePageIndex = Configuration::getInstance()->getActivePage();
                
                if (!(activePageIndex == (int)OpenedScreenEnums::OpenedScreen::SPLASH_SCREEN ||
                      activePageIndex == (int)OpenedScreenEnums::OpenedScreen::LOGIN_SCREEN))
                {
                    QMetaObject::invokeMethod(object,"setFirstTimeFlag");
                    if(SysConfigData::getInstance()->getAuthorityCheck())
                    {
                        //AuthCheck=ON
                        qDebug()<<"Logging out";
                        QMetaObject::invokeMethod(object,"callLogoutFunction");
                    }
                    else
                    {
                        //AuthCheck=OFF
                        QMetaObject::invokeMethod(object,"showServerFailPopup");
                    }
                }
                
                qDebug()<<"Sleeping";
                Sleep(1000);
                qDebug()<<"Creating socket";
                m_socket.Create();
                qDebug()<<"socket Created";
                QString qstrIP= Configuration::getInstance()->getIP();
                QString qstrPort = Configuration::getInstance()->getPort();
                auto ip = qstrIP.toStdString().c_str();
                auto port = qstrPort.toInt();
                
                if(!m_socket.Connect(ip, port))
                {
                    m_socket.Close();
                }
                else
                {
                    m_isConnected = true;
                    QMetaObject::invokeMethod(object,"closeServerFailPopup");
                }
                continue;
            }
            
            int reqId,ReqValueLen;
            memcpy(&reqId,buff,4);
            memset(buff,0,4);
            m_socket.Read(buff,4);
            memcpy(&ReqValueLen,buff,4);
            if(reqId>0)
            {
                qDebug()<<"Received MessageID: "<<reqId;
                qDebug()<<"Received Length: "<<ReqValueLen;
            }
            char *chTemp;
            if(ReqValueLen)
            {
                char *ReqBuff = NULL;
                ReqBuff=(char *)malloc(ReqValueLen+10);
                if(!ReqBuff)
                    continue;
                memset(ReqBuff,0,ReqValueLen+10);
                m_socket.ReadExact(ReqBuff,ReqValueLen);
                ReqBuff[ReqValueLen]=0;
                QString qstrReceivedBuff="";
                int dwActivePageIndex=0;
//                QString filename = "UICWeldResult.txt";
//                QFile file(filename);
                switch(reqId)
                {
                case REQ_PROGRESS_HORN_SCAN:
                case RES_START_HORN_SCAN:
                    qByteResBuff.clear();
                    qByteResBuff.append(ReqBuff , ReqValueLen);
                    free(ReqBuff);
                    emit hornScanMsgData(reqId , qByteResBuff);
                    break;
                case UIC_SEEK_CMD_RES:
                    qByteResBuff.clear();
                    qByteResBuff.append(ReqBuff , ReqValueLen);
                    free(ReqBuff);
                    emit seekScanMsgData(reqId , qByteResBuff);
                    break;
                case UIC_ALARM_DATA_INDICATION:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit alarmRaised(qstrReceivedBuff);
                    break;
                case UIC_CLEAR_NON_RESET_ALARMS:
                    free(ReqBuff);
                    emit clearNonResetAlarms();
                    break;
                case SCBL_GEN_SINGLE_REPORT_RSP:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit generateReportRes(qstrReceivedBuff);
                    break;
                case UIC_OPTIMIZE_DATABASE_RESP:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit vacummReportRes(qstrReceivedBuff);
                    break;
                case SCBL_SETUP_RES:
                    qstrReceivedBuff = ReqBuff;
                    ActuatorData stActuatorData;
                    chTemp=reinterpret_cast<char*>(&stActuatorData);
                    memcpy(chTemp,ReqBuff,sizeof(stActuatorData));
                    qstrReceivedBuff = QString::number(stActuatorData.AbsolutePosition);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.ActualDistance);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.ActualForce);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.ExcpetedPartContact);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.PrePartContact);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.PrePartContactVelocity);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.ResponseStatus);
                    qstrReceivedBuff += "," + QString::number(stActuatorData.ResponseType);
                    free(ReqBuff);
                    emit setupIndication(qstrReceivedBuff);
                    break;
                case UIC_EMERGENCY_STOP_ACTIVE_IND:
                    free(ReqBuff);
                    emit emergencyPopupStart();
                    break;
                case UIC_EMERGENCY_STOP_RESET_IND:
                    free(ReqBuff);
                    emit emergencyPopupStop();
                    break;
                case UIC_TEST_PROGRESS_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit setProgressData(qstrReceivedBuff);
                    break;
                case SCBL_SYS_READY_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit systemStatusData(qstrReceivedBuff);
                    break;
                case SCBL_GEN_REPORT_REMIND_LATER_RSP:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit dBStorageStatusRes(qstrReceivedBuff);
                    break;
                case UIC_WELD_PROGRESS_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    dwActivePageIndex = Configuration::getInstance()->getActivePage();
                    if(dwActivePageIndex == (int)OpenedScreenEnums::OpenedScreen::PRODUTCION)
                    {
                        emit productionPowerUpdate(qstrReceivedBuff);
                    }
                    break;
                case SCBL_WELD_FINISH_IND:
                    free(ReqBuff);
                    dwActivePageIndex = Configuration::getInstance()->getActivePage();
                    if(dwActivePageIndex == (int)OpenedScreenEnums::OpenedScreen::DASHBORAD)
                    {
                        emit dashboardUpdate(reqId);
                    }
                    else if(dwActivePageIndex == (int)OpenedScreenEnums::OpenedScreen::PRODUTCION)
                    {
                        emit productionUpdate();
                    }
                    else if(dwActivePageIndex == (int)OpenedScreenEnums::OpenedScreen::WELDGRAPH ||
                            dwActivePageIndex == (int)OpenedScreenEnums::OpenedScreen::ANALYTICS)
                    {
                        emit updateWeldHistory();
                    }
                    break;
                case UIC_WELD_DATA_DELETE_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit weldDataDeleteInd(qstrReceivedBuff);
                    break;
                case UIC_FW_UPGRADE_PROGRESS_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit upgradeProgress(qstrReceivedBuff);
                    break;
                case UIC_FW_UPGRADE_CMD_RES:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit upgradeResponse(qstrReceivedBuff);
                    break;
                case UIC_BARCODE_RECIPE_RECALL_REQ:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit barcodeRecipeRecall(qstrReceivedBuff);
                    break;
                case UIC_BARCODE_RECIPE_RECALL_AUTHENTICATION_CHECK_REQ:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit barcodeRecipeRecallAuthCheck(qstrReceivedBuff);
                    break;
                case UIC_BARCODE_SCANNER_STATUS_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit barcodescannerconnectionstatus(qstrReceivedBuff);
                    break;
                case UIC_BARCODE_PART_ID_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit partidstatus(qstrReceivedBuff);
                    break;
                case UIC_BARCODE_RECIPE_RECALL_FAIL_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit barcodeProcessStatus(qstrReceivedBuff);
                    break;
                case RES_COPY_DB:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit copyDBRes(qstrReceivedBuff);
                    break;
                case UIC_FW_UPGRADE_ETHER_IND:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);
                    emit firmwareUpgradeOverEthernet(qstrReceivedBuff);
                    break;
                case UIC_WEBSERVICE_UI_VERSION_REQ:
                	qstrReceivedBuff = ReqBuff;
                	free(ReqBuff);
                	emit webserviceVersionRequest(qstrReceivedBuff);
                	break;
                case UIC_WEBSERVICE_USER_LOGGED_INFO:
                	qstrReceivedBuff = ReqBuff;
                	free(ReqBuff);
                    emit webserviceUserLoggedInfo(qstrReceivedBuff);
                	break;
                case UIC_WEBSERVICE_REFRESH_SCREEN:
                    qstrReceivedBuff = ReqBuff;
                    free(ReqBuff);

                    emit ChangeFromWSSignal(qstrReceivedBuff);
                    break;
                case 0:
                    qstrReceivedBuff = ReqBuff;
                    dwWeldCounter = qstrReceivedBuff.toInt();
                    free(ReqBuff);
                    break;
                default:
                    if(RecvBuffMap.contains(reqId))
                    {
                        RecvBuffMap[reqId].isBuffReceived=true;
                        RecvBuffMap[reqId].dwBuffLen=ReqValueLen;
                        RecvBuffMap[reqId].pchBuff=ReqBuff;
                        //ReqBuff is not getting freed here because of undesirable behaviour of the system
                    }
                    else
                    {
                        if(reqId>0)
                        {
                            qDebug()<<"Ulsol Response ID:"<<reqId;
                            qDebug()<<"Ulsol Response len:"<<ReqValueLen;
                            qDebug()<<"Ulsol Response buff:"<<ReqBuff;
                            free(ReqBuff);
                        }
                    }
                }
                if(reqId > 0)
                    QCoreApplication::postEvent(filter,new QEvent(QEvent::Type(BACK_GROUND_THREAD_RUNNING)));
            }
            else
            {
                RecvBuffMap[reqId].isBuffReceived = true;
                RecvBuffMap[reqId].dwBuffLen = 0;
                RecvBuffMap[reqId].pchBuff=(char*)"";
            }
        }
        catch(...)
        {
            qDebug()<<"Crash Found";
        }
    }
}

/**
 * @brief CommunicationInterface::raiseEmergencyStop
 */
void CommunicationInterface::raiseEmergencyStop()
{
    emit emergencyPopupStart();
}

/**
 * @brief HeartBeatUIBL::HeartBeatUIBL
 */
HeartBeatUIBL::HeartBeatUIBL()
{
}

/**
 * @brief HeartBeatUIBL::~HeartBeatUIBL
 */
HeartBeatUIBL::~HeartBeatUIBL()
{
    terminate();
}

/**
 * @brief HeartBeatUIBL::stratHeartBeat
 */
void HeartBeatUIBL::stratHeartBeat()
{
    start();
}

/**
 * @brief HeartBeatUIBL::stopHeartBeat
 */
void HeartBeatUIBL::stopHeartBeat()
{
    terminate();
}

/**
 * @brief HeartBeatUIBL::run
 */
void HeartBeatUIBL::run()
{
    while(1)
    {
        if(CIObj.m_isConnected)
        {
        	CIObj.sendMessage(0,0,"");
        }

        Sleep(1000);
    }
}


PartsPerMin::PartsPerMin()
{

}
PartsPerMin::~PartsPerMin()
{
    terminate();
}

void PartsPerMin::startPartsPerMin()
{
    start();
}

void PartsPerMin::run()
{
    CIObj.queueWelds.clear();
    //CIObj.queueWelds.enqueue(0);
    while (1)
    {
        if(CIObj.m_isConnected == false)
        {
            Sleep(5000);
            continue;
        }
        if(CIObj.queueWelds.size() > 0 && CIObj.queueWelds.last() > CIObj.dwWeldCounter)
        {
            CIObj.queueWelds.clear();
            CIObj.queueWelds.enqueue(CIObj.dwWeldCounter);
            Sleep(5000);
            continue;
        }
        if(CIObj.queueWelds.size()<12)
            CIObj.queueWelds.enqueue(CIObj.dwWeldCounter);
        else
        {
            CIObj.queueWelds.dequeue();
            CIObj.queueWelds.enqueue(CIObj.dwWeldCounter);
        }
        Sleep(5000);
    }
}

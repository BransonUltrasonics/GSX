/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek History Data

***************************************************************************/
#ifndef HISTORYDATA_H
#include "Header/seekhistorydata.h"
#endif

#include <QFile>
#include <QQmlContext>
using namespace std ;

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief SeekHistoryItem::SeekHistoryItem : Initialization of class members and memory for object
 * @param a_scanId
 * @param a_dateTime
 */
SeekHistoryItem :: SeekHistoryItem(const QString &a_scanId , const QString &a_dateTime)
{
    m_SeekscanId = a_scanId;
    m_SeekdateTime = formatDateTime(a_dateTime , true);
}

/**
 * @brief SeekHistoryItem::~SeekHistoryItem
 */
SeekHistoryItem :: ~SeekHistoryItem()
{
}

/**
 * @brief SeekHistoryItem::getSeekScanId :Return seek scan ID
 * @return
 */
QString SeekHistoryItem :: getSeekScanId()
{
    return m_SeekscanId ;
}

/**
 * @brief SeekHistoryItem::getSeekDateTime : Return Date&Time
 * @return
 */
QString SeekHistoryItem :: getSeekDateTime()
{
    return m_SeekdateTime ;
}

/**
 * @brief SeekHistoryItem::setScanId : Set new scan ID
 * @param a_newScanId
 */
void SeekHistoryItem :: setScanId(QString a_newScanId)
{
    m_SeekscanId = a_newScanId ;
}

/**
 * @brief SeekHistoryItem::formatDateTime : Convert the dateformat
 * @param a_dateTime
 * @param a_formatDateTime
 * @return
 */
QString SeekHistoryItem :: formatDateTime(QString a_dateTime , bool a_formatDateTime)
{
    QStringList date ;
    QStringList formattedDate;
    QString tmpString ;
    QString formattedString ;
    QStringList tmpDateTime ;
    int ind = 0 ;

    if(true == a_formatDateTime)
    {
        tmpDateTime = a_dateTime.split(' ');
        tmpString = tmpDateTime.at(0);
        date = tmpString.split('/');
        formattedDate.reserve(3);

        for(ind = 2 ; ind >= 0 ; ind--)
        {
            formattedDate.append(date[ind]);
        }

        tmpDateTime[0] = formattedDate.join('/');
        formattedString = tmpDateTime.join(' ');
    }

    return formattedString ;
}

/**
 * @brief SeekHistoryData::SeekHistoryData
 */
SeekHistoryData :: SeekHistoryData()
{
}

/**
 * @brief SeekHistoryData::clearSeekHistoryData : String list for date and time
 */
void SeekHistoryData :: clearSeekHistoryData()
{
    /* Clear the member string list for date and time , history ID and RecipeId */
    SeekHistoryItem* tmpHistoryItem ;
    int ind = 0;

    for(ind = 0 ; ind < m_seekhistoryItemList.count(); ind++)
    {
        tmpHistoryItem = dynamic_cast<SeekHistoryItem*>(m_seekhistoryItemList.at(ind));

        delete tmpHistoryItem ;
        tmpHistoryItem = NULL;
    }

    m_seekhistoryItemList.clear();
}

/**
 * @brief SeekHistoryData::updateSeekHistoryDataId : Update the model from list
 * @param a_positionInd
 * @param updatedId
 * @return value from BL success response
 */
int SeekHistoryData :: updateSeekHistoryDataId(unsigned int a_positionInd , QString updatedId)
{
    int status = OPERATION_SUCCESS ;
    QStringList updateHistoryBuff;
    QString updateHistoryMsg;

    QString histIdToModify ;

    SeekHistoryItem* tmpHistory ;

    context->setContextProperty("seekhistoryListCtrlModel" , QVariant::fromValue(m_dummyHistoryItemList));

    /* Check if position Ind is within the history List */
    if(a_positionInd > (unsigned int)m_seekhistoryItemList.length())
    {
        status = PARAM_OUT_OF_RANGE ;
    }
    /* Check if updatedId from user is not part of the history List */
    else if(SCANID_FOUND == checkSeekScanIdRepeat(updatedId))
    {

        status = SCANID_REPEAT ;
    }

    if(OPERATION_SUCCESS == status)
    {
        tmpHistory = dynamic_cast<SeekHistoryItem*>(m_seekhistoryItemList.at(a_positionInd));

        histIdToModify = tmpHistory->getSeekScanId();

        updateHistoryBuff.append(histIdToModify);
        updateHistoryBuff.append(updatedId);

        updateHistoryMsg = updateHistoryBuff.join(",");

        CIObj.sendMessage(REQ_SET_SEEK_SCAN_ID, RES_SET_SEEK_SCAN_ID, updateHistoryMsg);

        CIObj.recivedMessage(RES_SET_SEEK_SCAN_ID , updateHistoryMsg);

        updateHistoryBuff = updateHistoryMsg.split(',');

        if(1 >= updateHistoryBuff.size())
        {
            status = OPERATION_FAILURE ;
        }
        else
        {
           status = stoi(updateHistoryBuff.at(0).toStdString());

           if(MESSGE_OP_SUCCESS != status)
           {
               status = OPERATION_FAILURE ;
           }
           else
           {
               tmpHistory->setScanId(updatedId);
           }
        }
    }

    context->setContextProperty("seekhistoryListCtrlModel" , QVariant::fromValue(m_seekhistoryItemList));

    return status ;
}

/**
 * @brief SeekHistoryData::getSeekHistoryDataFromDb : Get the seek history data form database checking the values and appending in to List
 * @param a_dateTime
 * @param a_countReq
 * @return value from BL
 */
int SeekHistoryData :: getSeekHistoryDataFromDb(QString a_dateTime , QString a_countReq)
{
    int retVal = OPERATION_SUCCESS ;
    QStringList historyDataBuff ;

    QString historyDataMsg;
    QString historyDataResMsg;

    QString tmpHistoryId, tmpDateTime;

    QStringList historyDataResBuff;

    int ind = 0;
    int length = 0 ;

    /* Generate the history data request send message to DB */
    historyDataBuff.append(a_dateTime);
    historyDataBuff.append(a_countReq);
    historyDataMsg = historyDataBuff.join(",");

    context->setContextProperty("seekhistoryListCtrlModel" , QVariant::fromValue(m_dummyHistoryItemList));

    CIObj.sendMessage(REQ_GET_SEEK_SCAN_HISTORY,RES_GET_SEEK_SCAN_HISTORY, historyDataMsg);

    CIObj.recivedMessage(RES_GET_SEEK_SCAN_HISTORY , historyDataResMsg);

    historyDataResBuff = historyDataResMsg.split(",");


    length = historyDataResBuff.size();

    if((1 == length) || (0 != (length % SEEK_PARAMETERS_PER_RECORD)))
    {
        retVal = INCORRECT_PARAM ;
    }
    else
    {
        /* Clear the history data */
        clearSeekHistoryData();

        /* Add the new history items to the list */
        for(ind = 0; ind < length / SEEK_PARAMETERS_PER_RECORD ; ind++)
        {
            tmpHistoryId = historyDataResBuff.at(ind * SEEK_PARAMETERS_PER_RECORD);
            tmpDateTime = historyDataResBuff.at(ind * SEEK_PARAMETERS_PER_RECORD + 1);

            m_seekhistoryItemList.append(new SeekHistoryItem(tmpHistoryId , tmpDateTime));
        }

        retVal = length / SEEK_PARAMETERS_PER_RECORD ;
    }

    /* Update the History UI */
    context->setContextProperty("seekhistoryListCtrlModel" , QVariant::fromValue(m_seekhistoryItemList));

    return retVal ;
}

/**
 * @brief SeekHistoryData::checkSeekScanIdRepeat : Scan the Seek ID
 * @param a_scanIdVal
 * @return
 */
int SeekHistoryData :: checkSeekScanIdRepeat(QString a_scanIdVal)
{
    SeekHistoryItem* tmpHistoryItem ;
    int ind = 0;
    int status = SCANID_NOT_FOUND ;

    /* Search the complete list of scan IDs for the scanIdVal */
    for(ind=0 ; ind < m_seekhistoryItemList.length(); ind++)
    {
        tmpHistoryItem = dynamic_cast<SeekHistoryItem*>(m_seekhistoryItemList.at(ind));
        if(a_scanIdVal == tmpHistoryItem->getSeekScanId())
        {
            status = SCANID_FOUND ;
            break ;
        }
    }

    return status ;
}

/**
 * @brief SeekHistoryData::performSeekScan : Perform seek scan
 * @return
 */
int SeekHistoryData :: performSeekScan()
{
    QString startSeekScanMsg ;

    /* Initiate a message to the UI task to start the horn scan operation */
    CIObj.sendMessage(SCBL_SEEK_CMD_REQ , SCBL_SEEK_CMD_REQ ,startSeekScanMsg);

    m_opInProgress = true ;

    /* Reset the horn resonant points */
    //m_seekResultData.resetSeekResonantPoints();

    return OPERATION_SUCCESS;
}

/**
 * @brief SeekHistoryData::receiveScUiseekReqMsg : Receive request message
 * @param reqId
 * @param ScUiMsg
 */
void SeekHistoryData :: receiveScUiseekReqMsg(int reqId , QByteArray ScUiMsg)
{

    int status = (int)ScUiMsg.at(0);
    switch(reqId)
    {
    case UIC_SEEK_CMD_RES:
        receiveStartSeekScanRes(status);
        break;
    case REQ_PROGRESS_HORN_SCAN:
        receiveSeekScanStatus(status);
        break;
    }
}

/**
 * @brief SeekHistoryData::~SeekHistoryData
 */
SeekHistoryData :: ~SeekHistoryData()
{
    clearSeekHistoryData();
}

/**
 * @brief SeekHistoryData::receiveStartSeekScanRes : Receive scan resoponse and emit the signal
 * @param status
 */
void SeekHistoryData :: receiveStartSeekScanRes(int status)
{
    /* Reset the "operation in progress" flag */
    m_opInProgress = false ;
    emit seekScanCompleted(status);
}

/**
 * @brief SeekHistoryData::getSeekOperationStatus
 * @return
 */
bool SeekHistoryData :: getSeekOperationStatus()
{
    return m_opInProgress;
}

/**
 * @brief SeekHistoryData::getSeekIndSelected
 * @return
 */
unsigned int SeekHistoryData :: getSeekIndSelected()
{
    return m_currentSeekInd;
}

/**
 * @brief SeekHistoryData::setSeekIndSelected
 * @param a_hornInd
 */
void SeekHistoryData :: setSeekIndSelected(unsigned int a_hornInd)
{
    m_currentSeekInd = a_hornInd;
}

/**
 * @brief SeekHistoryData::receiveSeekScanStatus : Percentage status
 * @param progressPercentage
 */
void SeekHistoryData :: receiveSeekScanStatus(int progressPercentage)
{
    /* Calculate the progress from the total samples */
    qreal progressStatus = 0.0;

    progressStatus = (qreal)progressPercentage / (qreal)100 ;

    /* Send signal to the progress bar to update the UI */
    {
        emit changeSeekProgress(progressStatus);
    }
}

/**
 * @brief SeekHistoryData::getSeekGraphDataFromDb : Get seek data from Database and return value
 * @param a_historyId
 * @return
 */
int SeekHistoryData :: getSeekGraphDataFromDb(QString a_historyId)
{
    int retVal = OPERATION_SUCCESS ;

    /* Retrieve the horn signature information from DB */
    retVal = m_seekScanData.getSeekSignatureData(a_historyId) ;

    /* Retrieve horn result information from DB */
    if(OPERATION_SUCCESS == retVal)
    {
        //retVal = m_seekResultData.getSeekResultDbRes(a_historyId);
    }

    return retVal ;
}

/**
 * @brief SeekHistoryData::refreshSeekHistory : refresh the model to changed the updated status
 * @return
 */
int SeekHistoryData :: refreshSeekHistory()
{
    int retVal = OPERATION_SUCCESS ;
    SeekHistoryItem* tmpHistory ;
    QString scanId;

    /* Retrieve the history information from the DB */
    retVal = getSeekHistoryDataFromDb("999912302359" , "100" );

    if(retVal > 0)
    {
        /* Retrieve the latest scan ID from the history list */
        tmpHistory = dynamic_cast<SeekHistoryItem*>(m_seekhistoryItemList.at(0));

        scanId = tmpHistory->getSeekScanId();

        /* Retrieve the horn signature and horn the result information for the latest data */

        retVal = getSeekGraphDataFromDb(scanId);
    }

    return retVal ;
}

/**
 * @brief SeekHistoryData::retrieveSeekSigPoints
 * @param a_series
 * @param a_type
 * @return
 */
int SeekHistoryData :: retrieveSeekSigPoints(QAbstractSeries *a_series , quint8 a_type)
{
    m_seekScanData.replaceSeekSample(a_series , a_type);

    return OPERATION_SUCCESS ;
}

/**
 * @brief SeekHistoryData::seekAxisMaxValues
 * @return
 */
QList<qreal> SeekHistoryData :: seekAxisMaxValues()
{
    return m_seekScanData.getAxisMaxParameters();
}

/**
 * @brief SeekHistoryData::seekAxisMinValues
 * @return
 */
QList<qreal> SeekHistoryData :: seekAxisMinValues()
{
    return m_seekScanData.getAxisMinParameters();
}

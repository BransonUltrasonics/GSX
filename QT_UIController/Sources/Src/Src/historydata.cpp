/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

History Data

***************************************************************************/
#ifndef HISTORYDATA_H
#include "Header/historydata.h"
#endif

#include <QFile>
#include <QQmlContext>

using namespace std ;

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief HistoryItem::HistoryItem : Initialization of class members and memory for object
 * @param a_scanName
 * @param a_scanId
 * @param a_dateTime
 */
HistoryItem :: HistoryItem(const QString &a_scanName,const QString &a_scanId , const QString &a_dateTime)
{
    m_scanName = a_scanName;
    m_scanId = a_scanId;
    m_dateTime = formatDateTime(a_dateTime , true);
}

/**
 * @brief HistoryItem::~HistoryItem
 */
HistoryItem :: ~HistoryItem()
{
}

/**
 * @brief HistoryItem::getScanName : Return Scan name
 * @return
 */
QString HistoryItem :: getScanName()
{
    return m_scanName ;
}

/**
 * @brief HistoryItem::getScanId : Return Scan ID
 * @return
 */
QString HistoryItem :: getScanId()
{
    return m_scanId ;
}

/**
 * @brief HistoryItem::getDateTime : Return Date and Time
 * @return
 */
QString HistoryItem :: getDateTime()
{
    return m_dateTime ;
}

/**
 * @brief HistoryItem::getRecipeId : Return recipe id
 * @return
 */
quint16 HistoryItem :: getRecipeId()
{
    return m_recipeId;
}

/**
 * @brief HistoryItem::setScanName : Set Scan name
 * @param a_newScanName
 */
void HistoryItem :: setScanName(QString a_newScanName)
{
    m_scanName = a_newScanName;
}

/**
 * @brief HistoryItem::formatDateTime : Convert the date format
 * @param a_dateTime
 * @param a_formatDateTime
 * @return
 */
QString HistoryItem :: formatDateTime(QString a_dateTime , bool a_formatDateTime)
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
 * @brief HistoryData::HistoryData : This function clears the data stored in history in BackEnd when required
 */
HistoryData :: HistoryData() {
}

/**
 * @brief HistoryData::clearHistoryData : This function clears the data stored in history in BackEnd when required
 */
void HistoryData :: clearHistoryData()
{
    /* Clear the member string list for date and time , history ID and RecipeId */
    HistoryItem* tmpHistoryItem ;
    int ind = 0;

    for(ind = 0 ; ind < m_historyItemList.count(); ind++)
    {
        tmpHistoryItem = dynamic_cast<HistoryItem*>(m_historyItemList.at(ind));

        delete tmpHistoryItem;
        tmpHistoryItem = NULL;
    }

    m_historyItemList.clear();
}

/**
 * @brief HistoryData::updateHistoryDataId : This function is called to update the History ID
 * @param a_positionInd : a_positionInd - index of the history list for which
*                       history ID to be updated
*
 * @param updatedId : updatedId: New ID that is to be updated in the DB
 * @return : int - to return status of the request
 */
int HistoryData :: updateHistoryDataId(unsigned int a_positionInd , QString updatedId)
{
    int status = OPERATION_SUCCESS ;
    QStringList updateHistoryBuff;
    QString updateHistoryMsg;

    QString histIdToModify ;

    HistoryItem* tmpHistory ;


    /* Check if position Ind is within the history List */
    if(a_positionInd > (unsigned int)m_historyItemList.length())
    {
        status = PARAM_OUT_OF_RANGE ;
    }
    /* Check if updatedId from user is not part of the history List */
    else if(SCANID_FOUND == checkScanIdRepeat(updatedId))
    {
        status = SCANID_REPEAT ;
    }
    context->setContextProperty("historyListCtrlModel" , QVariant::fromValue(m_dummyHornHistoryItemList));

    if(OPERATION_SUCCESS == status)
    {
        tmpHistory = dynamic_cast<HistoryItem*>(m_historyItemList.at(a_positionInd));

        histIdToModify = tmpHistory->getScanId();

        updateHistoryBuff.append(histIdToModify);
        updateHistoryBuff.append(updatedId);

        updateHistoryMsg = updateHistoryBuff.join(",");

        CIObj.sendMessage(REQ_SET_HORN_SCAN_ID, RES_SET_HORN_SCAN_ID, updateHistoryMsg);
        updateHistoryMsg="";
        CIObj.recivedMessage(RES_SET_HORN_SCAN_ID , updateHistoryMsg);

        updateHistoryBuff = updateHistoryMsg.split(',');

        if(updateHistoryMsg == "")
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
                tmpHistory->setScanName(updatedId);
            }
        }
    }

    context->setContextProperty("historyListCtrlModel" , QVariant::fromValue(m_historyItemList));

    return status ;
}

/**
 * @brief HistoryData::getHistoryDataFromDb : This function is called to retrieve the history from DB
 * @param a_dateTime : a_dateTime - Time and Date from which history to update
*
 * @param a_countReq : a_countReq - number of records to retrieve
 * @return : int - to return status of the request
 */
int HistoryData :: getHistoryDataFromDb(QString a_dateTime , QString a_countReq)
{
    int retVal = OPERATION_SUCCESS ;
    QStringList historyDataBuff ;

    QString historyDataMsg;
    QString historyDataResMsg;

    QString tmpHistoryName,tmpHistoryId, tmpRecipeID, tmpDateTime;

    QStringList historyDataResBuff;

    int ind = 0;
    int length = 0 ;

    context->setContextProperty("historyListCtrlModel" , QVariant::fromValue(m_dummyHornHistoryItemList));

    /* Generate the history data request send message to DB */
    historyDataBuff.append(a_dateTime);
    historyDataBuff.append(a_countReq);
    historyDataMsg = historyDataBuff.join(",");

    CIObj.sendMessage(REQ_GET_HORN_SCAN_HISTORY,RES_GET_HORN_SCAN_HISTORY, historyDataMsg);

    CIObj.recivedMessage(RES_GET_HORN_SCAN_HISTORY , historyDataResMsg);

    historyDataResBuff = historyDataResMsg.split(",");


    length = historyDataResBuff.size();

    if(historyDataResMsg=="" || (0 != (length % PARAMETERS_PER_RECORD)))
    {
        retVal = INCORRECT_PARAM ;
    }
    else
    {
        /* Clear the history data */
        clearHistoryData();

        /* Add the new history items to the list */
        for(ind = 0; ind < length / PARAMETERS_PER_RECORD ; ind++)
        {
            tmpHistoryName = historyDataResBuff.at(ind * PARAMETERS_PER_RECORD);
            tmpHistoryId = historyDataResBuff.at(ind * PARAMETERS_PER_RECORD + 1);
            tmpDateTime = historyDataResBuff.at(ind * PARAMETERS_PER_RECORD + 2);
            m_historyItemList.append(new HistoryItem(tmpHistoryName, tmpHistoryId, tmpDateTime));
        }

        retVal = length / PARAMETERS_PER_RECORD ;
    }

    /* Update the History UI */
    context->setContextProperty("historyListCtrlModel" , QVariant::fromValue(m_historyItemList));

    return retVal ;
}

/**
 * @brief HistoryData::checkScanIdRepeat : This function is called to check if the scan ID is part of the
 *                       history items
 * @param a_scanIdVal
 * @return
 */
int HistoryData :: checkScanIdRepeat(QString a_scanIdVal)
{
    HistoryItem* tmpHistoryItem ;
    QString tmpUpdatedScanIdVal = a_scanIdVal.toLower() ;
    QString tmpHistoryScanId ;
    int ind = 0;
    int status = SCANID_NOT_FOUND ;

    /* Search the complete list of scan IDs for the scanIdVal */
    for(ind=0 ; ind < m_historyItemList.length(); ind++)
    {
        tmpHistoryItem = dynamic_cast<HistoryItem*>(m_historyItemList.at(ind));
        tmpHistoryScanId = tmpHistoryItem->getScanId() ;
        if(tmpUpdatedScanIdVal == tmpHistoryScanId.toLower())
        {
            status = SCANID_FOUND ;
            break ;
        }
    }

    return status ;
}

/**
 * @brief HistoryData::performHornScan : This function called to send start horn scan request to SC
 * @return
 */
int HistoryData :: performHornScan()
{
    QString startHornScanMsg ;

    /* Initiate a message to the UI task to start the horn scan operation */
    CIObj.sendMessage(REQ_START_HORN_SCAN , REQ_START_HORN_SCAN ,startHornScanMsg);

    m_opInProgress = true ;

    /* Reset the horn resonant points */
    m_hornResultData.resetHornResonantPoints();

    return OPERATION_SUCCESS;
}

/**
 * @brief HistoryData::receiveScUiReqMsg : This function is slot to the signal from communication Interface
*                       socket on reception of horn scan progress or horn scan response message
 * @param reqId
 * @param ScUiMsg
 */
void HistoryData :: receiveScUiReqMsg(int reqId , QByteArray ScUiMsg)
{

    int status = (int)ScUiMsg.at(0);
    switch(reqId)
    {
    case RES_START_HORN_SCAN:
        receiveStartHornScanRes(status);
        break;
    case REQ_PROGRESS_HORN_SCAN:
        receiveHornScanStatus(status);
        break;
    }
}

/**
 * @brief HistoryData::~HistoryData
 */
HistoryData :: ~HistoryData()
{
    HistoryItem* tmpHistoryItem ;
    int ind = 0 ;

    for(ind = 0 ; ind < m_historyItemList.length() ; ind++ )
    {
        tmpHistoryItem = dynamic_cast<HistoryItem*>(m_historyItemList.at(ind));
        delete tmpHistoryItem ;
        tmpHistoryItem = NULL;
    }
    m_historyItemList.clear();
}

/**
 * @brief HistoryData::receiveStartHornScanRes : This function processes the start horn scan response message
 * @param status : - horn scan operation status code
 */
void HistoryData :: receiveStartHornScanRes(int status)
{
    /* Reset the "operation in progress" flag */
    m_opInProgress = false ;
    emit hornScanCompleted(status);
}

/**
 * @brief HistoryData::getHornOperationStatus : Return horn operation status
 * @return
 */
bool HistoryData :: getHornOperationStatus()
{
    return m_opInProgress;
}

/**
 * @brief HistoryData::getHornIndSelected : Return horn selected indication
 * @return
 */
unsigned int HistoryData :: getHornIndSelected()
{
    return m_currentHornInd;
}

/**
 * @brief HistoryData::setHornIndSelected : Set horn selected indication
 * @param a_hornInd
 */
void HistoryData :: setHornIndSelected(unsigned int a_hornInd)
{
    m_currentHornInd = a_hornInd;
}

/**
 * @brief HistoryData::receiveHornScanStatus : This function updates the progress bar of Horn Scan UI
 * @param progressPercentage : progressPercentage - Progress in percentage received from SC
 */
void HistoryData :: receiveHornScanStatus(int progressPercentage)
{
    /* Calculate the progress from the total samples */
    qreal progressStatus = 0.0;

    progressStatus = (qreal)progressPercentage / (qreal)100 ;

    /* Send signal to the progress bar to update the UI */
    {
        emit changeProgress(progressStatus);
    }
}

/**
 * @brief HistoryData::abortHornScan : Sending horn scan abort request to BL
 * @return
 */
int HistoryData :: abortHornScan()
{
    char abortScanResMsg[4] = {0};
    int status = OPERATION_FAILURE;

    CIObj.sendMessage(REQ_ABORT_HORN_SCAN , RES_ABORT_HORN_SCAN , "");

    CIObj.recivedMessage(RES_ABORT_HORN_SCAN , abortScanResMsg , 1);

    status = abortScanResMsg[0]-48;

    m_opInProgress = false ;

    return status;

}

/**
 * @brief HistoryData::getHornGraphDataFromDb : This function called to retrieve horn sig and horn
*                      resonant point data for a history ID.
 * @param a_historyId : QString - History ID for which horn graph data to be retrieved
 * @return
 */
int HistoryData :: getHornGraphDataFromDb(QString a_historyId)
{
    int retVal = OPERATION_SUCCESS ;

    /* Retrieve the horn signature information from DB */
    retVal = m_hornScanData.getHornSignatureData(a_historyId) ;


    /* Retrieve horn result information from DB */
    if(OPERATION_SUCCESS == retVal)
    {
        retVal = m_hornResultData.getHornResultDbRes(a_historyId);
    }

    return retVal ;
}

/**
 * @brief HistoryData::refreshHistory : This function refreshes the history information and
*                      retrieves the horn signature and horn result for history ID
 * @return
 */
int HistoryData :: refreshHistory()
{
    int retVal = OPERATION_SUCCESS ;
    HistoryItem* tmpHistory ;
    QString scanId;
    
    /* Retrieve the history information from the DB */
    retVal = getHistoryDataFromDb("999912302359" , "100" );

    if(retVal > 0 )
    {
        /* Retrieve the latest scan ID from the history list */
        tmpHistory = dynamic_cast<HistoryItem*>(m_historyItemList.at(0));

        scanId = tmpHistory->getScanId();

        /* Retrieve the horn signature and horn the result information for the latest data */

        retVal = getHornGraphDataFromDb(scanId);
    }
    return retVal ;
}

/**
 * @brief HistoryData::retrieveHornSigPoints : This function called to retrieve horn sig and horn
*                      resonant point data for from the UI
 * @param a_series :
 * @param a_type : QString - History ID for which horn graph data to be retrieved
 * @return
 */
int HistoryData :: retrieveHornSigPoints(QAbstractSeries *a_series , quint8 a_type)
{
    m_hornScanData.replaceSample(a_series , a_type);

    return OPERATION_SUCCESS ;
}

/**
 * @brief HistoryData::hornAxisMaxValues : Return horn Axis maximum value
 * @return
 */
QList<qreal> HistoryData :: hornAxisMaxValues()
{
    return m_hornScanData.getAxisMaxParameters();
}

/**
 * @brief HistoryData::hornAxisMinValues : Return horn Axis minimum value
 * @return
 */
QList<qreal> HistoryData :: hornAxisMinValues()
{
    return m_hornScanData.getAxisMinParameters();
}

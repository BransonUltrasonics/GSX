/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek Result Data

***************************************************************************/
#ifndef SEEKRESULTDATA_H
    #include "Header/seekresultdata.h"
#endif

#include <fstream>
#include <iostream>
#include <QFile>
#include <QQmlContext>

using namespace std ;
extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief SeekResonantPoints::SeekResonantPoints : Initialization of class members and memory for object
 * @param a_labelName
 * @param a_xAxisVal
 * @param a_custColor
 */
SeekResonantPoints :: SeekResonantPoints(const QString &a_labelName , const int &a_xAxisVal, const QString &a_custColor)
{
    m_labelName = a_labelName ;
    m_timePos = a_xAxisVal;
    m_custColor = a_custColor ;
}

/**
 * @brief SeekResonantPoints::~SeekResonantPoints
 */
SeekResonantPoints :: ~SeekResonantPoints()
{
}

/**
 * @brief SeekResonantPoints::getLabelName : Return the Name
 * @return
 */
QString SeekResonantPoints :: getLabelName()
{
    return m_labelName;
}

/**
 * @brief SeekResonantPoints::getXAxisPos : Return the position
 * @return
 */
int SeekResonantPoints :: getXAxisPos()
{
    return m_timePos ;
}

/**
 * @brief SeekResonantPoints::getCustColor : Return the color
 * @return
 */
QString SeekResonantPoints :: getCustColor()
{
    return m_custColor ;
}

/**
 * @brief SeekResonantPoints::setXAxisPos : Return the XAxis position
 * @param a_val
 */
void SeekResonantPoints :: setXAxisPos(int a_val)
{
    m_timePos = a_val;
}

/* Purpose: Open file containing the result information and read the result into BackEnd */
/**
 * @brief SeekResultData::SeekResultData : Containing the result information and read the result
 */
SeekResultData :: SeekResultData()
{
    context->setContextProperty("resonantPointModel" , QVariant::fromValue(m_dummyresonantPoints));

    m_SeekresonantPoints.append(new SeekResonantPoints("P1", -1, "#68a4eb"));
    m_SeekresonantPoints.append(new SeekResonantPoints("P2", -1, "#c3c3c3"));
    m_SeekresonantPoints.append(new SeekResonantPoints("P3", -1, "#c3c3c3"));
    m_SeekresonantPoints.append(new SeekResonantPoints("S1", -1, "#c3c3c3"));
    m_SeekresonantPoints.append(new SeekResonantPoints("S2", -1, "#c3c3c3"));
    m_SeekresonantPoints.append(new SeekResonantPoints("S3", -1, "#c3c3c3"));

    context->setContextProperty("resonantPointModel" , QVariant :: fromValue(m_SeekresonantPoints));
}

/**
 * @brief SeekResultData::resetSeekResonantPoints : Reset the old values
 */
void SeekResultData :: resetSeekResonantPoints()
{
    SeekResonantPoints* tmpResonantPoint ;
    unsigned int ind = 0;
    unsigned int length = m_SeekresonantPoints.length();

    for(ind = 0 ; ind < length ; ind++)
    {
        tmpResonantPoint = dynamic_cast<SeekResonantPoints*>(m_SeekresonantPoints.at(ind));
        tmpResonantPoint->setXAxisPos(-1);
    }

    return ;
}

/**
 * @brief SeekResultData::getSeekResultDbRes : Update the model and send the request to BL
 * @param a_historyId
 * @return
 */
int SeekResultData :: getSeekResultDbRes(QString a_historyId)
{
    QString seekResultMsg;
    QString wordRead ;
    QStringList seekResultBuff;
    SeekResonantPoints *tmpResonantPoint;
    int ind = 0;
    int length = 0 ;
    int retVal = OPERATION_SUCCESS ;

    context->setContextProperty("resonantPointModel" , QVariant::fromValue(m_dummyresonantPoints));

    /* Send Message requesting seek result data */
    CIObj.sendMessage(REQ_GET_SEEK_SCAN_RESULTS ,RES_GET_SEEK_SCAN_RESULTS , a_historyId);

    /* Receive message requesting seek result response data */

    CIObj.recivedMessage(RES_GET_SEEK_SCAN_RESULTS , seekResultMsg);

    seekResultBuff = seekResultMsg.split(',');

    length = seekResultBuff.size();

    if((1 >= length) || (COUNT_RESONANT_POINTS != length))
    {
        retVal = OPERATION_FAILURE ;

        /* In case of failure scenario reset the resonant points to -1 xAxis pos so that it does not display on screen */
        for(ind = 0 ; ind < COUNT_RESONANT_POINTS ; ind++)
        {
            tmpResonantPoint = dynamic_cast<SeekResonantPoints*>(m_SeekresonantPoints.at(ind));
            tmpResonantPoint->setXAxisPos(-1);
        }

    }
    else
    {
        for(ind = 0 ; ind < COUNT_RESONANT_POINTS ; ind++)
        {
            tmpResonantPoint = dynamic_cast<SeekResonantPoints*>(m_SeekresonantPoints.at(ind));
            wordRead = seekResultBuff.at(ind);
            tmpResonantPoint->setXAxisPos(stoi(wordRead.toStdString()));
        }
    }
    context->setContextProperty("resonantPointModel" , QVariant :: fromValue(m_SeekresonantPoints));

    return retVal ;
}

/**
 * @brief SeekResultData::~SeekResultData
 */
SeekResultData :: ~SeekResultData()
{
    SeekResonantPoints* tmpHornResonantPoint ;
    int length = 0 ;
    int ind = 0 ;

    length = m_SeekresonantPoints.length();

    for(ind = 0 ; ind < length ; ind++ )
    {
        tmpHornResonantPoint = dynamic_cast<SeekResonantPoints*>(m_SeekresonantPoints.at(ind));

        delete tmpHornResonantPoint ;
        tmpHornResonantPoint = NULL;
    }

    m_SeekresonantPoints.clear();
}

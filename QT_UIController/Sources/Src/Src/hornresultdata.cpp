/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Result Data

***************************************************************************/
#ifndef HORNRESULTDATA_H
#include "Header/hornresultdata.h"
#endif

#include <fstream>
#include <iostream>
#include <QFile>
#include <QQmlContext>

using namespace std ;

extern QQmlContext* context;
extern CommunicationInterface CIObj;

/**
 * @brief HornResonantPoints::HornResonantPoints
 * @param a_labelName
 * @param a_xAxisVal
 * @param a_custColor
 */
HornResonantPoints :: HornResonantPoints(const QString &a_labelName , const int &a_xAxisVal, const QString &a_custColor)
{
    m_labelName = a_labelName ;
    m_timePos = a_xAxisVal;
    m_custColor = a_custColor ;
}

/**
 * @brief HornResonantPoints::~HornResonantPoints
 */
HornResonantPoints :: ~HornResonantPoints()
{

}

/**
 * @brief HornResonantPoints::getLabelName : Return horn scan result name
 * @return
 */
QString HornResonantPoints :: getLabelName()
{
    return m_labelName;
}

/**
 * @brief HornResonantPoints::getXAxisPos : Return XAxis position
 * @return
 */
int HornResonantPoints :: getXAxisPos()
{
    return m_timePos ;
}

/**
 * @brief HornResonantPoints::getCustColor : Return customize color
 * @return
 */
QString HornResonantPoints :: getCustColor()
{
    return m_custColor ;
}

/**
 * @brief HornResonantPoints::setXAxisPos : Set XAxis position
 * @param a_val
 */
void HornResonantPoints :: setXAxisPos(int a_val)
{
    m_timePos = a_val;
}

/**
 * @brief HornResultData::HornResultData : Updating horn scan resonant points
 */
HornResultData :: HornResultData(){
    HornResonantPoints* tmpHornResonantPoints ;
    context->setContextProperty("resonantPointModel" , QVariant :: fromValue(m_dummyResonantPoints));


    tmpHornResonantPoints = new HornResonantPoints("P1", -1, "#68a4eb");
    m_resonantPoints.append(tmpHornResonantPoints);
    
    tmpHornResonantPoints = new HornResonantPoints("P2", -1, "#c3c3c3");
    m_resonantPoints.append(tmpHornResonantPoints);
    
    
    tmpHornResonantPoints = new HornResonantPoints("P3", -1, "#c3c3c3");
    m_resonantPoints.append(tmpHornResonantPoints);
    
    
    tmpHornResonantPoints = new HornResonantPoints("S1", -1, "#c3c3c3");
    m_resonantPoints.append(tmpHornResonantPoints);
    
    
    tmpHornResonantPoints = new HornResonantPoints("S2", -1, "#c3c3c3");
    m_resonantPoints.append(tmpHornResonantPoints);
    
    
    tmpHornResonantPoints = new HornResonantPoints("S3", -1, "#c3c3c3");
    m_resonantPoints.append(tmpHornResonantPoints);
    
    context->setContextProperty("resonantPointModel" , QVariant :: fromValue(m_resonantPoints));
}

/**
 * @brief HornResultData::resetHornResonantPoints : Reset the values
 */
void HornResultData :: resetHornResonantPoints()
{
    HornResonantPoints* tmpResonantPoint ;
    unsigned int ind = 0;
    unsigned int length = m_resonantPoints.length();

    for(ind = 0 ; ind < length ; ind++)
    {
        tmpResonantPoint = dynamic_cast<HornResonantPoints*>(m_resonantPoints.at(ind));

        tmpResonantPoint->setXAxisPos(-1);
    }

    return ;
}

/**
 * @brief HornResultData::getHornResultDbRes : Return the horn scan result infromation and update the model
 * @param a_historyId
 * @return int response from BL
 */
int HornResultData :: getHornResultDbRes(QString a_historyId)
{
    QString hornResultMsg;
    QString wordRead ;
    QStringList hornResultBuff;
    HornResonantPoints *tmpResonantPoint;
    int ind = 0;
    int length = 0 ;
    int retVal = OPERATION_SUCCESS ;

    context->setContextProperty("resonantPointModel" , QVariant :: fromValue(m_dummyResonantPoints));

    /* Send Message requesting horn result data */
    CIObj.sendMessage(REQ_GET_HORN_SCAN_RESULTS ,RES_GET_HORN_SCAN_RESULTS , a_historyId);

    /* Receive message requesting horn result response data */

    CIObj.recivedMessage(RES_GET_HORN_SCAN_RESULTS , hornResultMsg);

    hornResultBuff = hornResultMsg.split(',');

    length = hornResultBuff.size();

    if((1 >= length) || (COUNT_RESONANT_POINTS != length))
    {
        retVal = OPERATION_FAILURE ;

        /* In case of failure scenario reset the resonant points to -1 xAxis pos so that it does not display on screen */
        for(ind = 0 ; ind < COUNT_RESONANT_POINTS ; ind++)
        {
            tmpResonantPoint = dynamic_cast<HornResonantPoints*>(m_resonantPoints.at(ind));
            wordRead = hornResultBuff.at(ind);
            tmpResonantPoint->setXAxisPos(-1);
        }

    }
    else
    {
        for(ind = 0 ; ind < COUNT_RESONANT_POINTS ; ind++)
        {
            tmpResonantPoint = dynamic_cast<HornResonantPoints*>(m_resonantPoints.at(ind));
            wordRead = hornResultBuff.at(ind);
            tmpResonantPoint->setXAxisPos(stoi(wordRead.toStdString()));
        }
    }
    context->setContextProperty("resonantPointModel" , QVariant :: fromValue(m_resonantPoints));

    return retVal ;
}

/**
 * @brief HornResultData::~HornResultData : Clearing the ploted points
 */
HornResultData :: ~HornResultData()
{
    HornResonantPoints* tmpHornResonantPoint ;
    int length = 0 ;
    int ind = 0 ;

    length = m_resonantPoints.length();

    for(ind = 0 ; ind < length ; ind++ )
    {
        tmpHornResonantPoint = dynamic_cast<HornResonantPoints*>(m_resonantPoints.at(ind));

        delete tmpHornResonantPoint ;
        tmpHornResonantPoint = NULL ;
    }

    m_resonantPoints.clear();
}

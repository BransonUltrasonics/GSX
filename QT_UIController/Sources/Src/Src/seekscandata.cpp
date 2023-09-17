/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek Scan Data

***************************************************************************/
#ifndef SEEKSCANDATA_H
#include "Header/seekscandata.h"
#endif

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QtMath>
#include <QFile>

QT_CHARTS_USE_NAMESPACE
using namespace std ;

extern CommunicationInterface CIObj;

/**
 * @brief SeekScanData::SeekScanData
 */
SeekScanData :: SeekScanData()
{
}

/**
 * @brief SeekScanData::init
 */
void SeekScanData :: init()
{
}

/**
 * @brief SeekScanData::getSeekSignatureData : Retreive parameters of hornSignatureData and send request message to BL
 * @param a_historyId
 * @return
 */
int SeekScanData :: getSeekSignatureData(QString a_historyId)
{
    /* Retreive parameters of hornSignatureData and retrieve the points */
    int ind=0;
    int totalSampleCountFromDb = 0 ;
    QString word ;
    QString seekSignatureMsg ;
    QStringList a_seekSignatureData;

    int retVal = OPERATION_SUCCESS ;

    QList<qreal> axisMinValues ;
    QList<qreal> axisMaxValues;

    for(ind = 0 ; ind <  6 ; ind++)
    {
        axisMaxValues.append(0);
        axisMinValues.append(0);
    }

    axisMinValues.replace(SEEK_AMP_SCAN_PARAM_IND , 0);
    axisMinValues.replace(SEEK_FREQ_SCAN_PARAM_IND , 0);
    axisMinValues.replace(SEEK_POWER_SCAN_PARAM_IND , 0);
    axisMinValues.replace(SEEK_CURRENT_SCAN_PARAM_IND , 0);
    axisMinValues.replace(SEEK_PHASE_SCAN_PARAM_IND , -180);
    axisMinValues.replace(SEEK_TIME_SCAN_PARAM_IND , 0);

    axisMaxValues.replace(SEEK_AMP_SCAN_PARAM_IND , 125);
    axisMaxValues.replace(SEEK_FREQ_SCAN_PARAM_IND , 125);
    axisMaxValues.replace(SEEK_POWER_SCAN_PARAM_IND , 10);
    axisMaxValues.replace(SEEK_CURRENT_SCAN_PARAM_IND , 10);
    axisMaxValues.replace(SEEK_PHASE_SCAN_PARAM_IND , 180);
    axisMaxValues.replace(SEEK_TIME_SCAN_PARAM_IND , 1);

    /* Send request message for the seek signature data */
    CIObj.sendMessage(REQ_GET_SEEK_SIGNATURE_DATA , RES_GET_SEEK_SIGNATURE_DATA , a_historyId);

    /* Receive response message of the seek signature data */

    CIObj.recivedMessage(RES_GET_SEEK_SIGNATURE_DATA , seekSignatureMsg);

    a_seekSignatureData = seekSignatureMsg.split(',');

    totalSampleCountFromDb  = a_seekSignatureData.size();

    /* Clear points in vector */
    m_AmpPoints.clear();
    m_FreqPoints.clear();
    m_PowerPoints.clear();
    m_CurrPoints.clear();
    m_PhasePoints.clear();

    if((1 >= totalSampleCountFromDb) || (0 != (totalSampleCountFromDb % 6)))
    {
        retVal = OPERATION_FAILURE ;
    }
    else
    {
        /* Read the file for all the samples */

        for(ind = 0 ; ind < totalSampleCountFromDb ; )
        {
            double dTime,dAmp,dFreq,dPower,dCurrent,dPhase;
            /* Populate the member variables */
            word = a_seekSignatureData.at(ind );
            dTime = word.toDouble()/1000;

            word = a_seekSignatureData.at(ind + 1 );
            dAmp = word.toDouble();

            word = a_seekSignatureData.at(ind + 2 );
            dFreq = word.toDouble();

            word = a_seekSignatureData.at(ind + 3 );
            dPower = word.toDouble();

            word = a_seekSignatureData.at(ind + 4 );
            dCurrent = word.toDouble();

            word = a_seekSignatureData.at(ind + 5 );
            dPhase = word.toDouble();

            ind = ind + 6;

            calculateLargest(axisMaxValues[SEEK_AMP_SCAN_PARAM_IND] , (qreal)dAmp);
            calculateSmallest(axisMinValues[SEEK_AMP_SCAN_PARAM_IND] , (qreal)dAmp);

            calculateLargest(axisMaxValues[SEEK_FREQ_SCAN_PARAM_IND] , (qreal)dFreq);
            calculateSmallest(axisMinValues[SEEK_FREQ_SCAN_PARAM_IND] , (qreal)dFreq);

            calculateLargest(axisMaxValues[SEEK_POWER_SCAN_PARAM_IND] , (qreal)dPower);
            calculateSmallest(axisMinValues[SEEK_POWER_SCAN_PARAM_IND] , (qreal)dPower);

            calculateLargest(axisMaxValues[SEEK_CURRENT_SCAN_PARAM_IND] , (qreal)dCurrent);
            calculateSmallest(axisMinValues[SEEK_CURRENT_SCAN_PARAM_IND] , (qreal)dCurrent);

            calculateLargest(axisMaxValues[SEEK_PHASE_SCAN_PARAM_IND] , (qreal)dPhase);
            calculateSmallest(axisMinValues[SEEK_PHASE_SCAN_PARAM_IND] , (qreal)dPhase);

            calculateLargest(axisMaxValues[SEEK_TIME_SCAN_PARAM_IND] , (qreal)dTime);
            calculateSmallest(axisMinValues[SEEK_TIME_SCAN_PARAM_IND] , (qreal)dTime);

            /* Append the points to vector */
            m_AmpPoints.append(QPointF(dTime , dAmp));
            m_FreqPoints.append(QPointF(dTime , dFreq));
            m_PowerPoints.append(QPointF(dTime , dPower));
            m_CurrPoints.append(QPointF(dTime , dCurrent));
            m_PhasePoints.append(QPointF(dTime , dPhase));
        }
        setAxisMaxParameters(axisMaxValues);
        setAxisMinParameters(axisMinValues);
    }
    return retVal ;
}

/**
 * @brief SeekScanData::setAxisMaxParameters : Set Axis Maximum value
 * @param a_axisVal
 */
void SeekScanData :: setAxisMaxParameters(QList<qreal> &a_axisVal)
{
    m_axisMaxParameters.clear();

    m_axisMaxParameters.append(a_axisVal);
}

/**
 * @brief SeekScanData::setAxisMinParameters : Set Axis minimum value
 * @param a_axisVal
 */
void SeekScanData :: setAxisMinParameters(QList<qreal> &a_axisVal)
{
    m_axisMinParameters.clear();

    m_axisMinParameters.append(a_axisVal);
}

/**
 * @brief SeekScanData::getAxisMinParameters : Return minimum value
 * @return
 */
QList<qreal> SeekScanData :: getAxisMinParameters()
{
    return m_axisMinParameters ;
}

/**
 * @brief SeekScanData::getAxisMaxParameters : Return maximum value
 * @return
 */
QList<qreal> SeekScanData :: getAxisMaxParameters()
{
    return m_axisMaxParameters ;
}

/**
 * @brief SeekScanData::replaceSeekSample : Updating the ploted points based on types
 * @param a_series
 * @param a_type
 */
void SeekScanData :: replaceSeekSample(QAbstractSeries *a_series , quint8 a_type)
{
    QXYSeries *xySeries = static_cast<QXYSeries *>(a_series);
    switch(a_type)
    {
    case 1:
        xySeries->replace(m_AmpPoints);
        break ;
    case 2:
        xySeries->replace(m_FreqPoints);
        break ;
    case 3:
        xySeries->replace(m_PowerPoints);
        break ;
    case 4:
        xySeries->replace(m_CurrPoints);
        break ;
    case 5:
        xySeries->replace(m_PhasePoints);
        break ;

    default:
        break ;
    }
}

/**
 * @brief SeekScanData::~SeekScanData
 */
SeekScanData :: ~SeekScanData()
{

}

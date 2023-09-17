/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Scan Data

***************************************************************************/
#ifndef HornScanData_H
#include "Header/hornscandata.h"
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
 * @brief HornScanData::HornScanData
 */
HornScanData :: HornScanData()
{
}

/**
 * @brief HornScanData::init
 */
void HornScanData :: init()
{
}

/**
 * @brief HornScanData::getHornSignatureData : Calculate the minimumm and maximun axis and Return the ploting points
 * @param a_historyId
 * @return ploting points data
 */
int HornScanData :: getHornSignatureData(QString a_historyId)
{
    /* Retreive parameters of hornSignatureData and retrieve the points */
    int ind=0;
    int totalSampleCountFromDb = 0 ;
    QString word ;
    QString hornSignatureMsg ;
    QStringList a_hornSignatureData;
    unsigned long tmpFreq = 0 ;
    unsigned int tmpAmp = 0 ;
    float tmpPhase = 0 ;
    unsigned int tmpImpedance = 0;
    unsigned int tmpCurrent = 0 ;

    int retVal = OPERATION_SUCCESS ;

    /* Time, Freq, Phase, Amp, Current, Power, Energy*/
    QList<qreal> axisMinValues ;
    QList<qreal> axisMaxValues;

    for(ind = 0 ; ind <=  PARAMETER_COUNT_PER_SIGNATURE_SAMPLE ; ind++)
    {
        axisMaxValues.append(0);
        axisMinValues.append(0);
    }

    axisMinValues.replace(AMP_SCAN_PARAM_IND , PARAM_MAX_VAL);
    axisMinValues.replace(CURRENT_SCAN_PARAM_IND , PARAM_MAX_VAL);
    axisMinValues.replace(PHASE_SCAN_PARAM_IND , PHASE_MAX_VAL);
    axisMinValues.replace(IMPEDANCE_SCAN_PARAM_IND , PARAM_MAX_VAL);
    axisMinValues.replace(FREQ_SCAN_PARAM_IND , PARAM_MAX_VAL);

    axisMaxValues.replace(AMP_SCAN_PARAM_IND , PARAM_MIN_VAL);
    axisMaxValues.replace(CURRENT_SCAN_PARAM_IND , PARAM_MIN_VAL);
    axisMaxValues.replace(PHASE_SCAN_PARAM_IND , PHASE_MIN_VAL);
    axisMaxValues.replace(IMPEDANCE_SCAN_PARAM_IND , PARAM_MIN_VAL);
    axisMaxValues.replace(FREQ_SCAN_PARAM_IND , PARAM_MIN_VAL);

    /* Send request message for the horn signature data */
    CIObj.sendMessage(REQ_GET_HORN_SIGNATURE_DATA , RES_GET_HORN_SIGNATURE_DATA , a_historyId);

    /* Receive response message of the horn signature data */

    CIObj.recivedMessage(RES_GET_HORN_SIGNATURE_DATA , hornSignatureMsg);

    a_hornSignatureData = hornSignatureMsg.split(',');

    totalSampleCountFromDb  = a_hornSignatureData.size();

    if((1 >= totalSampleCountFromDb) || (0 != (totalSampleCountFromDb % PARAMETER_COUNT_PER_SIGNATURE_SAMPLE)))
    {
        retVal = OPERATION_FAILURE ;
    }
    else
    {
        /* Clear points in vector */
        m_AmpPoints.clear();
        m_CurrentPoints.clear();
        m_PhasePoints.clear();
        m_ImpedancePoints.clear();

        /* Read the file for all the samples */

        for(ind = 0 ; ind < totalSampleCountFromDb ; )
        {
            /* Populate the member variables */
            word = a_hornSignatureData.at(ind );
            tmpFreq = stoul(word.toStdString()) ;

            word = a_hornSignatureData.at(ind + CURRENT_SCAN_DATA_INDEX );
            tmpCurrent = stoi(word.toStdString()) ;

            word = a_hornSignatureData.at(ind + AMPTITUDE_SCAN_DATA_INDEX);
            tmpAmp = stoi(word.toStdString()) ;

            word = a_hornSignatureData.at(ind + PHASE_SCAN_DATA_INDEX );
            tmpPhase = stof(word.toStdString()) ;
            ind = ind + 4;
            if(0 == tmpCurrent)
            {
                tmpImpedance = ((float)tmpAmp) ;
            }
            else
            {
                tmpImpedance = ((float)tmpAmp / (float)tmpCurrent);
            }

            calculateLargest(axisMaxValues[AMP_SCAN_PARAM_IND] , (qreal)tmpAmp);
            calculateSmallest(axisMinValues[AMP_SCAN_PARAM_IND] , (qreal)tmpAmp);

            calculateLargest(axisMaxValues[CURRENT_SCAN_PARAM_IND] , (qreal)tmpCurrent);
            calculateSmallest(axisMinValues[CURRENT_SCAN_PARAM_IND] , (qreal)tmpCurrent);

            calculateLargest(axisMaxValues[PHASE_SCAN_PARAM_IND] , (qreal)tmpPhase);
            calculateSmallest(axisMinValues[PHASE_SCAN_PARAM_IND] , (qreal)tmpPhase);

            calculateLargest(axisMaxValues[IMPEDANCE_SCAN_PARAM_IND] , (qreal)tmpImpedance);
            calculateSmallest(axisMinValues[IMPEDANCE_SCAN_PARAM_IND] , (qreal)tmpImpedance);

            calculateLargest(axisMaxValues[FREQ_SCAN_PARAM_IND] , (qreal)tmpFreq);
            calculateSmallest(axisMinValues[FREQ_SCAN_PARAM_IND] , (qreal)tmpFreq);

            /* Append the points to vector */
            /* Amplitude plot */
            m_AmpPoints.append(QPointF(tmpFreq , tmpAmp));

            /* Current Plot */
            m_CurrentPoints.append(QPointF(tmpFreq , tmpCurrent));

            /* Phase Plot */
            m_PhasePoints.append(QPointF(tmpFreq , tmpPhase));

            m_ImpedancePoints.append(QPointF(tmpFreq , tmpImpedance));

            /* TBD : Check if sorting of the points would be needed */
        }

        setAxisMaxParameters(axisMaxValues);
        setAxisMinParameters(axisMinValues);
    }

    return retVal ;
}

/**
 * @brief HornScanData::setAxisMaxParameters : Set maximum Axis parameter
 * @param a_axisVal
 */
void HornScanData :: setAxisMaxParameters(QList<qreal> &a_axisVal)
{
    m_axisMaxParameters.clear();

    m_axisMaxParameters.append(a_axisVal);
}

/**
 * @brief HornScanData::setAxisMinParameters : Set minimum Axis parameter
 * @param a_axisVal
 */
void HornScanData :: setAxisMinParameters(QList<qreal> &a_axisVal)
{
    m_axisMinParameters.clear();

    m_axisMinParameters.append(a_axisVal);
}

/**
 * @brief HornScanData::getAxisMinParameters : Return minimum Axis Parameter
 * @return
 */
QList<qreal> HornScanData :: getAxisMinParameters()
{
    return m_axisMinParameters ;
}

/**
 * @brief HornScanData::getAxisMaxParameters : Return Maximum Axis parameter
 * @return
 */
QList<qreal> HornScanData :: getAxisMaxParameters()
{
    return m_axisMaxParameters ;
}

/**
 * @brief HornScanData::replaceSample : Update the ploted points data and refresh based on graph selected
 * @param a_series
 * @param a_type
 */
void HornScanData :: replaceSample(QAbstractSeries *a_series , quint8 a_type)
{
    QXYSeries *xySeries = static_cast<QXYSeries *>(a_series);
    switch(a_type)
    {
    case 1: /* Amplitude Graph */
    {
        xySeries->replace(m_AmpPoints);
    }
        break ;
    case 2: /* Current Graph */
    {
        xySeries->replace(m_CurrentPoints);

    }
        break ;

    case 3: /* Plot Graph */
    {
        xySeries->replace(m_PhasePoints);

    }
        break;

    case 4: /* Impedance Graph */
    {
        xySeries->replace(m_ImpedancePoints);
    }
        break;

    default:
        break ;

    }
}

/**
 * @brief HornScanData::~HornScanData
 */
HornScanData :: ~HornScanData()
{
}

/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Weld Graph

***************************************************************************/
#ifndef HornScanData_H
#include "Header/WeldGraph.h"
#endif

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif
#include "Header/configuration.h"

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QtMath>
#include <QFile>
#include <QQmlContext>
#include <QDateTime>
#include <QTime>
#include <iostream>
#include <iomanip>
#include "Header/checkbox.h"
#include "Header/utility.h"
//#define TEST
#define DEBUG

QT_CHARTS_USE_NAMESPACE
using namespace std ;
extern QQmlContext* context;
extern CommunicationInterface CIObj;
extern int dwCurrentUnit;
extern checkbox *obj_checkbox;

/**
 * @brief WeldResonantPoints::WeldResonantPoints : Get the Points and XValues
 * @param a_labelName
 * @param a_xAxisVal
 */
WeldResonantPoints :: WeldResonantPoints(const QString &a_labelName , const double &a_xAxisVal)
{
    m_labelName = a_labelName ;
    m_timePos = a_xAxisVal;
}

/**
 * @brief WeldResonantPoints::~Weld Resonant Points : Destructor method
 */
WeldResonantPoints :: ~WeldResonantPoints()
{
}

/**
 * @brief WeldResonantPoints::getLabelName : Return Resonant Name
 * @return
 */
QString WeldResonantPoints :: getLabelName()
{
    return m_labelName;
}

/**
 * @brief WeldResonantPoints::getXAxisPos : Return XAxis Position
 * @return
 */
double WeldResonantPoints :: getXAxisPos()
{
    return m_timePos ;
}

/**
 * @brief WeldResonantPoints::setXAxisPos : Set XAxis Position
 * @param a_val
 */
void WeldResonantPoints :: setXAxisPos(double a_val)
{
    m_timePos = a_val;
}

/**
 * @brief WeldGraphData::WeldGraphData : Plot Weld Graph Data
 */
WeldGraphData :: WeldGraphData()
{
    QList<qreal> axisMinPoints , axisMaxPoints ;
    unsigned int ind = 0;
    m_GraphPointsCount = 0;
    /* Creating and initializing the resonant points*/
    m_weldPoints.append(new WeldResonantPoints("T", 0));
    m_weldPoints.append(new WeldResonantPoints("W", 0));
    m_weldPoints.append(new WeldResonantPoints("H", 0));

    context->setContextProperty("weldPointModel",QVariant::fromValue(m_weldPoints));

    /* Initializing axis parameters*/
    for(ind = 0; ind < PARAMETER_COUNT_PER_GRAPH_SAMPLE ; ind++)
    {
        axisMinPoints.append(0);
        axisMaxPoints.append(100);
    }
}

/**
 * @brief WeldGraphData::getTriggerPoint : Return Trigger Point
 * @return
 */
double WeldGraphData::getTriggerPoint()
{
    WeldResonantPoints *tmpResonantPoints = dynamic_cast<WeldResonantPoints*>(m_weldPoints.at(0));
    double dTriggerPoint = tmpResonantPoints->getXAxisPos();
    return dTriggerPoint;
}

/**
 * @brief WeldGraphData::receiveWeldGraphData : Replace the Plotted Points in Weld Graph
 * @param a_weldResultId
 * @return
 */
int WeldGraphData :: receiveWeldGraphData(QString a_weldResultId)
{
    int ind = 0 ;
    QString weldGraphResMsg;
    QString wordRead ;
    QStringList weldGraphResBuff;
    int totalParamsEntries = 0;
    int retVal = 0;
    int totalEntriesPerParams = 0;
    double tmpTime ;
    double tmpFreq ;
    double tmpPower ;
    double tmpCurrent ;
    double tmpAmp ;
    double tmpEnergy;
    double tmpPhase ;
    double tmpForce;
    double tmpVelocity;
    double tmpAbsoluteDist;
    double tmpCollapseDist;

    /* Time, Freq, Phase, Amp, Current, Power, Energy*/
    QList<qreal> axisMinValues ;
    QList<qreal> axisMaxValues;

    for(ind = 0; ind < PARAMETER_COUNT_PER_GRAPH_SAMPLE ; ind++)
    {
        axisMinValues.append(0);
        axisMaxValues.append(100);
    }
    //axisMaxValues.append(0);
    //axisMaxValues.append(0) ;

    axisMinValues.replace(AMP_PARAM_IND , MAX_PERCENTAGE);
    axisMinValues.replace(CURRENT_PARAM_IND , MAX_PERCENTAGE);
    axisMinValues.replace(POWER_PARAM_IND , MAX_PERCENTAGE);
    axisMinValues.replace(FREQ_PARAM_IND , (uint32_t)-1);
    axisMinValues.replace(ENERGY_PARAM_IND , (uint32_t)-1) ;
    axisMinValues.replace(PHASE_PARAM_IND , 180);
    axisMinValues.replace(FORCE_PARAM_IND , (uint32_t)-1) ;
    axisMinValues.replace(VELOCITY_PARAM_IND , (uint32_t)-1) ;
    axisMinValues.replace(ABSOLUTE_DIST_PARAM_IND , (uint32_t)-1) ;
    axisMinValues.replace(COLLAPSE_DIST_PARAM_IND , (uint32_t)-1) ;
    axisMinValues.replace(TIME_PARAM_IND , (uint32_t)-1) ;

    axisMaxValues.replace(AMP_PARAM_IND , 0);
    axisMaxValues.replace(CURRENT_PARAM_IND , 0);
    axisMaxValues.replace(POWER_PARAM_IND , 0);
    axisMaxValues.replace(FREQ_PARAM_IND , 0);
    axisMaxValues.replace(ENERGY_PARAM_IND , 0) ;
    axisMaxValues.replace(PHASE_PARAM_IND , -180);
    axisMaxValues.replace(FORCE_PARAM_IND , 0) ;
    axisMaxValues.replace(VELOCITY_PARAM_IND , 0) ;
    axisMaxValues.replace(ABSOLUTE_DIST_PARAM_IND , 0) ;
    axisMaxValues.replace(COLLAPSE_DIST_PARAM_IND , 0) ;
    axisMaxValues.replace(TIME_PARAM_IND , 0) ;

    /* Send request for weld Graph data */
    CIObj.sendMessage(REQ_GET_WELD_SIGNATURE_DATA, RES_GET_WELD_SIGNATURE_DATA , a_weldResultId);

    /* Poll for weld graph result from DB */
    CIObj.recivedMessage(RES_GET_WELD_SIGNATURE_DATA , weldGraphResMsg);

    /* Populate the QList points class members */
    weldGraphResBuff = weldGraphResMsg.split(',');

    totalParamsEntries = weldGraphResBuff.size();

    clearGraph();
    try
    {
        if(weldGraphResMsg == "" || 0 != (totalParamsEntries % PARAMETER_COUNT_PER_GRAPH_SAMPLE))
        {
            retVal = -1 ;
        }
        else
        {
            totalEntriesPerParams = totalParamsEntries / PARAMETER_COUNT_PER_GRAPH_SAMPLE;

            WeldResonantPoints *tmpResonantPoints = dynamic_cast<WeldResonantPoints*>(m_weldPoints.at(0));
            double dStartingPoint = tmpResonantPoints->getXAxisPos();
            tmpResonantPoints = dynamic_cast<WeldResonantPoints*>(m_weldPoints.at(2));
            double dEndPoint = tmpResonantPoints->getXAxisPos();

            clearGraph();
            m_GraphPointsCount = totalEntriesPerParams;
            //double samePointCheck = -1;
            for(ind = 0 ; ind < totalEntriesPerParams ; ind++)
            {
                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE);
                float tmpTime1 = stof(wordRead.toStdString());
                std::setprecision(5);
                QString a = QString::number(tmpTime1/1000, 'f', 5);
                tmpTime = a.toDouble();
                if((dStartingPoint-0.1) > tmpTime)
                    continue;
                if((dEndPoint+0.1) < tmpTime)
                    break;
                tmpTime = tmpTime - dStartingPoint;
                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 1);
                tmpFreq = wordRead.toDouble();

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 2);
                tmpPower = wordRead.toDouble();

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 3);
                tmpCurrent = wordRead.toDouble();

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 4);
                tmpAmp = wordRead.toDouble();

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 5);
                tmpPhase = wordRead.toDouble();

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 6);
                tmpEnergy =  Utility::convertMilliJoulesToJoules(wordRead.toDouble());

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 7);
                if(dwCurrentUnit)
                {
                    wordRead = Utility::convertNtolbs(wordRead);
                    /*To display one decimal value for imperial unit*/
                    wordRead.setNum(wordRead.toDouble(),'f',1);
                    tmpForce = wordRead.toDouble();
                }
                else
                    tmpForce = wordRead.toDouble();

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 8);
                if(dwCurrentUnit)
                    tmpVelocity = Configuration::getInstance()->convertmicrometretoinches(wordRead.toDouble());
                else
                    tmpVelocity = wordRead.toDouble()/(double)1000;

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 9);
                if(dwCurrentUnit)
                    tmpAbsoluteDist = Configuration::getInstance()->convertmicrometretoinches(wordRead.toDouble());
                else
                    tmpAbsoluteDist = wordRead.toDouble()/(double)1000;

                wordRead = weldGraphResBuff.at(ind * PARAMETER_COUNT_PER_GRAPH_SAMPLE + 10);
                if(dwCurrentUnit)
                    tmpCollapseDist = Configuration::getInstance()->convertmicrometretoinches(wordRead.toDouble());
                else
                    tmpCollapseDist = wordRead.toDouble()/(double)1000;

                calculateLargest(axisMaxValues[TIME_PARAM_IND] , (qreal)tmpTime);
                calculateSmallest(axisMinValues[TIME_PARAM_IND] , (qreal)tmpTime);

                calculateLargest(axisMaxValues[FREQ_PARAM_IND] , (qreal)tmpFreq);
                calculateSmallest(axisMinValues[FREQ_PARAM_IND] , (qreal)tmpFreq);

                calculateLargest(axisMaxValues[PHASE_PARAM_IND] , (qreal)tmpPhase);
                calculateSmallest(axisMinValues[PHASE_PARAM_IND] , (qreal)tmpPhase);

                calculateLargest(axisMaxValues[AMP_PARAM_IND] , (qreal)tmpAmp);
                calculateSmallest(axisMinValues[AMP_PARAM_IND] , (qreal)tmpAmp);

                calculateLargest(axisMaxValues[CURRENT_PARAM_IND] , (qreal)tmpCurrent);
                calculateSmallest(axisMinValues[CURRENT_PARAM_IND] , (qreal)tmpCurrent);

                calculateLargest(axisMaxValues[POWER_PARAM_IND] , (qreal)tmpPower);
                calculateSmallest(axisMinValues[POWER_PARAM_IND] , (qreal)tmpPower);

                calculateLargest(axisMaxValues[ENERGY_PARAM_IND] , (qreal)tmpEnergy);
                calculateSmallest(axisMinValues[ENERGY_PARAM_IND] , (qreal)tmpEnergy);

                calculateLargest(axisMaxValues[FORCE_PARAM_IND] , (qreal)tmpForce);
                calculateSmallest(axisMinValues[FORCE_PARAM_IND] , (qreal)tmpForce);

                calculateLargest(axisMaxValues[VELOCITY_PARAM_IND] , (qreal)tmpVelocity);
                calculateSmallest(axisMinValues[VELOCITY_PARAM_IND] , (qreal)tmpVelocity);

                calculateLargest(axisMaxValues[ABSOLUTE_DIST_PARAM_IND] , (qreal)tmpAbsoluteDist);
                calculateSmallest(axisMinValues[ABSOLUTE_DIST_PARAM_IND] , (qreal)tmpAbsoluteDist);

                calculateLargest(axisMaxValues[COLLAPSE_DIST_PARAM_IND] , (qreal)tmpCollapseDist);
                calculateSmallest(axisMinValues[COLLAPSE_DIST_PARAM_IND] , (qreal)tmpCollapseDist);

                m_FreqPoints.append(QPointF(tmpTime , tmpFreq));
                m_PowerPoints.append(QPointF(tmpTime , tmpPower));
                m_CurrentPoints.append(QPointF(tmpTime , tmpCurrent));
                m_AmpPoints.append(QPointF(tmpTime , tmpAmp));
                m_PhasePoints.append(QPointF(tmpTime , tmpPhase));
                m_EnergyPoints.append(QPointF(tmpTime , tmpEnergy));
                m_ForcePoints.append(QPointF(tmpTime , tmpForce));
                m_VelocityPoints.append(QPointF(tmpTime , tmpVelocity));
                m_AbsoluteDistPoints.append(QPointF(tmpTime , tmpAbsoluteDist));
                m_CollapseDistPoints.append(QPointF(tmpTime , tmpCollapseDist));
            }

            setAxisMaxParameters(axisMaxValues);
            setAxisMinParameters(axisMinValues);
        }
    }
    catch(...)
    {
        qDebug("Crash Occured while reading or assigning the graph data");
    }

    return retVal;
}

/**
 * @brief WeldGraphData::receiveWeldPoints : Return the plotted points
 * @param a_weldResultId
 * @return
 */
int WeldGraphData::receiveWeldPoints(QString a_weldResultId)
{
    QString weldPointsResMsg;
    QStringList weldPointsResBuff;
    QString wordRead ;
    int retVal = 0;
    WeldResonantPoints* tmpResonantPoints;
    //a_weldResultId="";
    /* Send request for weld Graph data */
    CIObj.sendMessage(REQ_GET_WELD_TWH_DATA, RES_GET_WELD_TWH_DATA, a_weldResultId);
    CIObj.recivedMessage(RES_GET_WELD_TWH_DATA , weldPointsResMsg);

    //weldPointsResMsg = "200,600,710";

    weldPointsResBuff = weldPointsResMsg.split(',');

    if(COUNT_WELD_POINTS != weldPointsResBuff.size())
    {
        retVal = -1;
    }
    else
    {
        context->setContextProperty("weldPointModel",QVariant::fromValue(m_weldPointsDummy));
        tmpResonantPoints = dynamic_cast<WeldResonantPoints*>(m_weldPoints.at(0));
        wordRead = weldPointsResBuff.at(0);
        tmpResonantPoints->setXAxisPos(wordRead.toDouble()/(double)1000);

        tmpResonantPoints = dynamic_cast<WeldResonantPoints*>(m_weldPoints.at(1));
        wordRead = weldPointsResBuff.at(1);
        tmpResonantPoints->setXAxisPos(wordRead.toDouble()/(double)1000);

        tmpResonantPoints = dynamic_cast<WeldResonantPoints*>(m_weldPoints.at(2));
        wordRead = weldPointsResBuff.at(2);
        tmpResonantPoints->setXAxisPos(wordRead.toDouble()/(double)1000);

        context->setContextProperty("weldPointModel",QVariant::fromValue(m_weldPoints));
    }

    return retVal ;
}

/**
 * @brief WeldGraphData::appendSamples : All Graph Plot has to be combined
 * @param a_series
 * @param a_type
 * @return
 */
int WeldGraphData :: appendSamples(CustomXYSeries *a_series , quint8 a_type)
{
    //Qa_series *a_series = static_cast<Qa_series *>(a_series);

    int length = 0 ;
    /* Switch Case to determine the type*/
    switch(a_type)
    {
    case FREQ_PARAM_IND: /* Amplitude Graph */
    {
        a_series->replace(m_FreqPoints);
        length = m_FreqPoints.count();
    }
        break ;
    case AMP_PARAM_IND: /* Current Graph */
    {
        a_series->replace(m_AmpPoints);
        length = m_AmpPoints.count();
    }
        break ;
    case CURRENT_PARAM_IND:/* Plot Graph */
    {
        a_series->replace(m_CurrentPoints);
        length = m_CurrentPoints.count();
    }
        break;
    case POWER_PARAM_IND: /* Impedance Graph */
    {
        a_series->replace(m_PowerPoints);
        length = m_PowerPoints.count();
    }
        break;
    case PHASE_PARAM_IND:
    {
        a_series->replace(m_PhasePoints);
        length = m_PhasePoints.count();
    }
        break ;
    case ENERGY_PARAM_IND:
        a_series->replace(m_EnergyPoints);
        length = m_EnergyPoints.count();
        break;
    case FORCE_PARAM_IND:
        a_series->replace(m_ForcePoints);
        length = m_ForcePoints.count();
        break;
    case VELOCITY_PARAM_IND:
        a_series->replace(m_VelocityPoints);
        length = m_VelocityPoints.count();
        break;
    case ABSOLUTE_DIST_PARAM_IND:
        a_series->replace(m_AbsoluteDistPoints);
        length = m_AbsoluteDistPoints.count();
        break;
    case COLLAPSE_DIST_PARAM_IND:
        a_series->replace(m_CollapseDistPoints);
        length = m_CollapseDistPoints.count();
        break;
    default:
        break ;
    }

    return length;
}

/**
 * @brief WeldGraphData::setAxisMinParameters : Minimun Ranges to set Axis
 * @param a_axisVal
 */
void WeldGraphData :: setAxisMinParameters(QList<qreal> &a_axisVal)
{
    m_axisMinParameters.clear();

    m_axisMinParameters.append(a_axisVal);
}

/**
 * @brief WeldGraphData::getAxisMinParameters : Return Axis Minimum Range
 * @return
 */
QList<qreal> WeldGraphData :: getAxisMinParameters()
{
    return m_axisMinParameters ;
}

/**
 * @brief WeldGraphData::setAxisMaxParameters : Maximum Ranges to set Axis
 * @param a_axisVal
 */
void WeldGraphData :: setAxisMaxParameters(QList<qreal> &a_axisVal)
{
    m_axisMaxParameters.clear();
    m_axisMaxParameters.append(a_axisVal);
}

/**
 * @brief WeldGraphData::getAxisMaxParameters : Return Axis Maximum Ranges
 *
 * @return
 */
QList<qreal> WeldGraphData :: getAxisMaxParameters()
{
    return m_axisMaxParameters ;
}

/**
 * @brief WeldGraphData::~WeldGraphData
 */
WeldGraphData :: ~WeldGraphData()
{
}

/**
 * @brief WeldGraphData::getCount : Total count of Plotted points
 * @return
 */
int WeldGraphData::getCount()
{
    return m_GraphPointsCount;
}

/**
 * @brief WeldGraphData::clearGraph : Clear the Plotted Points
 */
void WeldGraphData::clearGraph()
{
    m_GraphPointsCount = 0;
    m_FreqPoints.clear();
    m_PowerPoints.clear();
    m_PhasePoints.clear();
    m_CurrentPoints.clear();
    m_AmpPoints.clear();
    m_EnergyPoints.clear();
    m_ForcePoints.clear();
    m_VelocityPoints.clear();
    m_AbsoluteDistPoints.clear();
    m_CollapseDistPoints.clear();
}

/**
 * @brief WeldGraphData::GetClosestIndex : Getting the Closest Range Values while sliding
 * @param qstrXAxisValue
 * @return
 */
int WeldGraphData::GetClosestIndex(QString qstrXAxisValue)
{
    for(int dwIndex=0;dwIndex<m_GraphPointsCount;dwIndex++)
    {
        QPointF pointTemp = m_FreqPoints.at(dwIndex);
        if(pointTemp.x() >= qstrXAxisValue.toDouble())
        {
            return dwIndex;
        }
    }
    return 0;
}

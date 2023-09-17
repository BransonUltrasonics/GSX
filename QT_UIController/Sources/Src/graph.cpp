/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Plots graph for test mode

***************************************************************************/
#include "Header/graph.h"
#include <Header/communicationinterface.h>
#include <Header/qtcommon.h>
#include <QQmlContext>
#include <algorithm>
extern CommunicationInterface CIObj;
extern QQmlContext*context;


#define GRAPH_FREQ_AXIS         "FrequencyAxis"
#define GRAPH_AMP_AXIS          "AmpAxis"
#define GRAPH_POWER_AXIS        "PowerAxis"
#define GRAPH_CURRENT_AXIS      "CurrentAxis"
#define GRAPH_PHASE_AXIS        "PhaseAxis"
#define GRAPH_TIME_AXIS         "TimeAxis"

#define GRAPH_FREQ_PLOT         "FrequencyPlot"
#define GRAPH_AMP_PLOT          "AmpPlot"
#define GRAPH_POWER_PLOT        "PowerPlot"
#define GRAPH_CURRENT_PLOT      "CurrentPlot"
#define GRAPH_PHASE_PLOT        "PhasePlot"

enum
{
    AMP_SCAN_PARAM_IND = 0,
    CURRENT_SCAN_PARAM_IND,
    PHASE_SCAN_PARAM_IND,
    IMPEDANCE_SCAN_PARAM_IND,
    FREQ_SCAN_PARAM_IND
};

/**
 * @brief Graph :constructor
 * @param QObject *ptr
 */
Graph::Graph(QObject *parent) : QObject(parent)
{
    m_XAxisValue=0;
    addAxes();
    addGraph();
    context ->setContextProperty("graph", this);
}

/**
 * @brief Graph::copy : copy QSplineSeries *a_spline to m_splineList
 * @param a_spline
 * @return
 */
QAbstractSeries* Graph::copy(QSplineSeries *a_spline)
{
    MySplineSeries*spline;;
    spline=static_cast<MySplineSeries*>(a_spline);
    spline->setUseOpenGL(true);
    m_splineList.append(spline);
    return spline;
}

/**
 * @brief addAxes : adds basic information of graph axes
 */
void Graph::addAxes()
{
    if(m_axisProperties.size()>0)
    {
        for(int index=0;index<m_axisProperties.size();index++)
        {
            GraphProperties* temp =dynamic_cast<GraphProperties*>(m_axisProperties.at(index));
            delete temp;
            temp = NULL;
        }
    }
    m_axisProperties.clear();
    m_axisProperties.append(new GraphProperties(GRAPH_FREQ_AXIS,"#4285f4"));
    m_axisProperties.append(new GraphProperties(GRAPH_AMP_AXIS,"#f03e3e"));
    m_axisProperties.append(new GraphProperties(GRAPH_POWER_AXIS,"#a82ba8"));
    m_axisProperties.append(new GraphProperties(GRAPH_CURRENT_AXIS,"#009587"));
    m_axisProperties.append(new GraphProperties(GRAPH_PHASE_AXIS,"black"));
    m_axisProperties.append(new GraphProperties(GRAPH_TIME_AXIS,"black"));
}

/**
 * @brief addGraph : adds basic information of graph

 */
void Graph::addGraph()
{
    if(m_graphProperties.size()>0)
    {
        for(int index=0;index<m_graphProperties.size();index++)
        {
            GraphProperties* temp =dynamic_cast<GraphProperties*>(m_graphProperties.at(index));
            delete temp;
            temp = NULL;
        }
    }
    m_graphProperties.clear();
    m_graphProperties.append(new GraphProperties(GRAPH_FREQ_PLOT,"#4285f4"));
    m_graphProperties.append(new GraphProperties(GRAPH_AMP_PLOT,"#f03e3e"));
    m_graphProperties.append(new GraphProperties(GRAPH_POWER_PLOT,"#a82ba8"));
    m_graphProperties.append(new GraphProperties(GRAPH_CURRENT_PLOT,"#009587"));
    m_graphProperties.append(new GraphProperties(GRAPH_PHASE_PLOT,"black"));
}

/**
 * @brief getXAxisValue:returns value xAxisVal in Test.qml
 * @return double
 */
double Graph::getXAxisValue()
{
    return m_XAxisValue;
}

/**
 * @brief setXAxisValue:saves value xAxisVal in Test.qml
 */
void Graph::setXAxisValue(double a_XAxisValue)
{
    if(a_XAxisValue!=m_XAxisValue)
    {
        m_XAxisValue=a_XAxisValue;
        XAxisValueChanged();
    }
}

/**
 * @brief getGraphTime:gets value of timeLabel in Test.qml
 * @return QString
 */
QString Graph::getGraphTime()
{
    return m_GraphTime;
}

/**
 * @brief Graph::setGraphTime
 * @param a_GraphTime
 */
void Graph::setGraphTime(QString a_GraphTime)
{
    if(a_GraphTime!=m_GraphTime){

        m_GraphTime=a_GraphTime;
        GraphTimeChanged();
    }
}

/**
 * @brief colorAtAxis : color of axis
 * @param index of axis
 */
QString Graph::colorAtAxis(int a_index)
{
    return m_axisProperties.at(a_index)->m_color;
}

/**
 * @brief nameAtAxis : name of axis
 * @param index of axis
 */
QString Graph::nameAtAxis(int a_index)
{
    return m_axisProperties.at(a_index)->m_name;
}

/**
 * @brief nameAtGraph : name of graph
 * @param index of graph
 */
QString Graph::colorAtGraph(int a_index)
{
    return m_graphProperties.at(a_index)->m_color;
}

/**
 * @brief nameAtGraph : name of graph
 * @param index of graph
 */
QString Graph::nameAtGraph(int a_index)
{
    return m_graphProperties.at(a_index)->m_name;
}

/**
 * @brief noOfGraphs : number of graph
 * @return number of graph

 */
int Graph::noOfGraphs()
{
    return m_graphProperties.size();
}

/**
 * @brief GraphProperties::getName
 * @return
 */
QString GraphProperties::getName()
{
    return m_name;
}

/**
 * @brief GraphProperties::getColor
 * @return
 */
QString GraphProperties::getColor()
{
    return m_color;
}

/**
 * @brief GraphProperties :constructor
 * @param name ,color properties and QObject *ptr
 */
GraphProperties::GraphProperties(QString a_name, QString a_color, QObject *ptr):QObject(ptr)
{
    m_name=a_name;
    m_color=a_color;
}

/**
 * @brief getSignatureData :gets graph points from Database
 * @param id of data in database
 */
int Graph :: getSignatureData()
{
    /* Retreive parameters of SignatureData and retrieve the points */
    int ind=0;
    int totalSampleCountFromDb = 0 ;
    QString word ;
    QString SignatureMsg ;
    QStringList a_SignatureData;
    double tmpFrequency = 0 ;
    double tmpAmp = 0 ;
    double tmpPhase = 0 ;
    double tmpPower = 0;
    double tmpCurrent = 0 ;
    double tmpTime = 0 ;

    int retVal = OPERATION_SUCCESS ;

    /* Send request message for the  signature data */
    CIObj.sendMessage(REQ_GET_TEST_MODE_DATA, RES_GET_TEST_MODE_DATA , "");

    /* Receive response message of the  signature data */
    CIObj.recivedMessage(RES_GET_TEST_MODE_DATA , SignatureMsg);

    a_SignatureData = SignatureMsg.split(',');

    totalSampleCountFromDb  = a_SignatureData.count();

    if((1 >= totalSampleCountFromDb) || (0 != (totalSampleCountFromDb % 6)))
    {
        retVal = OPERATION_FAILURE ;
    }
    else
    {
        /* Clear points in vector */
        for(int index=0;index<m_splineList.size();index++)
        {
            m_splineList.at(index)->m_splinePoints.clear();
        }

        for(ind = 0 ; ind < totalSampleCountFromDb ; )
        {
            /* Populate the member variables */

            word = a_SignatureData.at(ind );
            tmpTime = word.toDouble()/1000.0 ;

            word = a_SignatureData.at(ind + 1);
            tmpAmp = word.toDouble();

            word = a_SignatureData.at(ind + 2 );
            tmpFrequency = word.toDouble();

            word = a_SignatureData.at(ind + 3 );
            tmpPower = word.toDouble();

            word = a_SignatureData.at(ind + 4 );
            tmpCurrent = word.toDouble();

            word = a_SignatureData.at(ind + 5 );
            tmpPhase = word.toDouble();

            ind = ind + 6;;
            /* Append the points to vector */
            m_splineList.at(0)->m_splinePoints.append(QPointF(tmpTime , tmpFrequency));
            m_splineList.at(1)->m_splinePoints.append(QPointF(tmpTime , tmpAmp));

            m_splineList.at(2)->m_splinePoints.append(QPointF(tmpTime , tmpPower));
            m_splineList.at(3)->m_splinePoints.append(QPointF(tmpTime , tmpCurrent));
            m_splineList.at(4)->m_splinePoints.append(QPointF(tmpTime , tmpPhase));
        }
    }
    return retVal ;
}

/**
 * @brief getMin :gets minimum point
 * @param QSplineSeries graph
 */
QPointF Graph::getMin(QSplineSeries *a_splineseries)
{
    return a_splineseries->at(0);
}

/**
 * @brief getMax :gets maximum point
 * @param QSplineSeries graph
 */
QPointF Graph::getMax(QSplineSeries *a_splineseries)
{
    return a_splineseries->at(a_splineseries->count()-1);
}

/**
 * @brief getMax :sets visibilty of axes and graph
 * @param QSplineSeries graph
 * @param QAbstractAxis axis
 * @param visibility true or false
 */
void Graph::setVisibility(QSplineSeries *a_splineseries,QAbstractAxis* a_abstractaxis, bool a_isVisible)
{
    a_splineseries->setVisible(a_isVisible);
    a_abstractaxis->setVisible(a_isVisible);
}

/**
 * @brief getAtIndex :gets point of QSplineSeries graph at index
 * @param QSplineSeries graph
 * @param a_index
 * @return point
 */
QPointF Graph:: getAtIndex(QSplineSeries *a_splineseries,int a_index)
{
    return a_splineseries->at(a_index);
}

/**
 * @brief getMinPoint :gets point of QSplineSeries graph at index for which y is min

 * @param a_index
 * @return point
 */
QPointF Graph::getMinPoint(int a_index)
{
    return *std::min_element(m_splineList.at(a_index)->m_splinePoints.begin(),m_splineList.at(a_index)->m_splinePoints.end(),[]( QPointF &a_p1, QPointF &a_p2)
    {
        return a_p1.y()<a_p2.y()  ;
    });

}

/**
 * @brief getMaxPoint :gets point of QSplineSeries graph at index for which y is max

 * @param a_index
 * @return point
 */
QPointF Graph::getMaxPoint(int a_index)
{
    return *std::max_element(m_splineList.at(a_index)->m_splinePoints.begin(),m_splineList.at(a_index)->m_splinePoints.end(),[](QPointF &a_p1,QPointF &a_p2) {

        return a_p1.y()<a_p2.y()  ;

    });

}

/**
 * @brief setMin_Max :sets min and max value for axis
 * @param QAbstractAxis axis
 * @param min value
 * @param max value
 */
void Graph::setMin_Max(QAbstractAxis *a_abstractaxis, qreal a_min, qreal a_max)
{
    if(a_min==a_max)
    {
        if(a_min==0.0f)
        {
            a_abstractaxis->setMax(QVariant(1));
            a_abstractaxis->setMin(QVariant(0));
        }
        else if(a_min<0)
        {
            a_abstractaxis->setMax(QVariant(0));
            a_abstractaxis->setMin(QVariant(a_min));
        }
        else if(a_min>0)
        {
            a_abstractaxis->setMax(QVariant(a_min));
            a_abstractaxis->setMin(QVariant(0));
        }
    }
    else
    {
        int dwDiff = a_max-a_min;
        if(a_max-a_min < 4)
        {
            switch(dwDiff)
            {
            case 0:
                a_min = a_min - 2;
                a_max = a_max + 2;
                break;
            case 1:
                a_min = a_min - 2;
                a_max = a_max + 1;
                break;
            case 2:
                a_min = a_min - 1;
                a_max = a_max + 1;
                break;
            case 3:
                a_max = a_max + 1;
                break;
            }
        }
        a_abstractaxis->setMax(QVariant(a_max));
        a_abstractaxis->setMin(QVariant(a_min));
    }
}

/**
 * @brief clearLists : clears m_splineList ,m_axisProperties ,m_graphProperties;
 */
void Graph::clearLists()
{
    if(m_graphProperties.size()>0)
    {
        for(int index=0;index<m_graphProperties.size();index++)
        {
            GraphProperties* temp =dynamic_cast<GraphProperties*>(m_graphProperties.at(index));
            delete temp;
            temp = NULL;
        }
    }
    m_graphProperties.clear();

    if(m_axisProperties.size()>0)
    {
        for(int index=0;index<m_axisProperties.size();index++)
        {
            GraphProperties* temp =dynamic_cast<GraphProperties*>(m_axisProperties.at(index));
            delete temp;
            temp = NULL;
        }
    }
    m_axisProperties.clear();

    m_splineList.clear();
}

/**
 * @brief getXAxisValue:returns value xAxisVal in Test.qml
 * @return double
 */
void Graph::addAxesAndSplines()
{
    addAxes();
    addGraph();
}

/**
 * @brief replaceSample :replaces points of a graph
 * @param index of spline graph
 */
void Graph :: replaceSample(int a_index)
{
    if(a_index>=0)
    {
        QXYSeries *xySeries = static_cast<QXYSeries *>(m_splineList.at(a_index));
        xySeries->replace(m_splineList.at(a_index)->m_splinePoints);
    }
}

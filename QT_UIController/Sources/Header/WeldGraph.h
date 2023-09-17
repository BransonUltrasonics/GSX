/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Weld Graph

***************************************************************************/
#ifndef WELDGRAPH_H
#define WELDGRAPH_H


#include <QObject>
#include <QString>
#include <QFile>
#include<QTextStream>

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>
#include "Header/customxyseries.h"


#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

#define PARAMETER_COUNT_PER_GRAPH_SAMPLE 11
#define COUNT_WELD_POINTS 3
#define MAX_PERCENTAGE 100

/* Indices for the Graph Parameters */
enum
{
    AMP_PARAM_IND = 0 ,
    POWER_PARAM_IND ,
    FREQ_PARAM_IND ,
    FORCE_PARAM_IND,
    COLLAPSE_DIST_PARAM_IND,
    ABSOLUTE_DIST_PARAM_IND,
    VELOCITY_PARAM_IND ,
    CURRENT_PARAM_IND ,
    PHASE_PARAM_IND ,
    ENERGY_PARAM_IND ,
    TIME_PARAM_IND
};

QT_CHARTS_USE_NAMESPACE

//! A weldResonantPoint Class
/*!
  This class derived from QObject, holds the weld points to be plotted on the weld Graph UI.
*/
class WeldResonantPoints : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString labelName READ getLabelName NOTIFY labelNameChanged)
    Q_PROPERTY(double xAxisPos READ getXAxisPos NOTIFY xAxisPosChanged)

public :
    WeldResonantPoints(const QString &a_labelName , const double &a_xAxisVal);
    ~WeldResonantPoints();
    QString m_labelName;
    double m_timePos;
    QString getLabelName();
    double getXAxisPos();

    void setXAxisPos(double val);

signals:
    void labelNameChanged();
    void xAxisPosChanged();

};

//!  A weldGraphData class.
/*!
  This class derived from QObject, handles the back end interface to hold the
 weld graph data and for plotting the graph.
*/
class WeldGraphData : public QObject
{
    Q_OBJECT

public:

    // constructor.
    /*
      This Constructor initializes and creates WeldGraphData object.
       * */
    explicit WeldGraphData();
    // Destructor.
    /*
      This Destructor destroys WeldGraphData object.
       * */
    ~WeldGraphData();
    
    //Q_INVOKABLE int appendSamples(QAbstractSeries *a_series , quint8 a_type);
    Q_INVOKABLE int appendSamples(CustomXYSeries *a_series , quint8 a_type);

    Q_INVOKABLE int receiveWeldGraphData(QString a_weldResultId);

    Q_INVOKABLE int receiveWeldPoints(QString a_weldResultId);

    Q_INVOKABLE void setAxisMinParameters(QList<qreal> &a_axisVal);
    Q_INVOKABLE void setAxisMaxParameters(QList<qreal> &a_axisVal);

    Q_INVOKABLE QList<qreal> getAxisMinParameters();
    Q_INVOKABLE QList<qreal> getAxisMaxParameters();

    Q_INVOKABLE int getCount();
    Q_INVOKABLE void clearGraph();
    Q_INVOKABLE int GetClosestIndex(QString qstrXAxisValue);
//    Q_INVOKABLE QString updateParameterVal(int dwIndex);
    Q_INVOKABLE double getTriggerPoint();

    inline void calculateLargest(qreal &a_axisVal , qreal a_val)
    {
        if(a_axisVal < a_val)
        {
            a_axisVal = a_val;
        }
    }


    inline void calculateSmallest(qreal &a_axisVal , qreal a_val)
    {
        if(a_axisVal > a_val)
        {
            a_axisVal = a_val;
        }
    }

private:
    int m_GraphPointsCount;

    //QVector<QPointF> m_TimePoints ;
    //QVector<QPointF> m_FreqPoints ;
    //QVector<QPointF> m_PowerPoints ;
    //QVector<QPointF> m_CurrentPoints ;
    //QVector<QPointF> m_AmpPoints;
    //QVector<QPointF> m_PhasePoints ;
    //QVector<QPointF> m_EnergyPoints;
    //QVector<QPointF> m_ForcePoints;
    //QVector<QPointF> m_VelocityPoints;
    //QVector<QPointF> m_AbsoluteDistPoints;
    //QVector<QPointF> m_CollapseDistPoints;

    QList<QPointF> m_TimePoints ;
    QList<QPointF> m_FreqPoints ;
    QList<QPointF> m_PowerPoints ;
    QList<QPointF> m_CurrentPoints ;
    QList<QPointF> m_AmpPoints;
    QList<QPointF> m_PhasePoints ;
    QList<QPointF> m_EnergyPoints;
    QList<QPointF> m_ForcePoints;
    QList<QPointF> m_VelocityPoints;
    QList<QPointF> m_AbsoluteDistPoints;
    QList<QPointF> m_CollapseDistPoints;

    /* Resonant Points */
    QList<QObject*> m_weldPoints;
    QList<QObject*> m_weldPointsDummy;

    /* Axis Parameters */
    QList<qreal> m_axisMinParameters ;
    QList<qreal> m_axisMaxParameters;
};
#endif // WELDGRAPH_H

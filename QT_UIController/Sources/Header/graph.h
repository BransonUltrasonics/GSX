/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Graph in Test Mode

***************************************************************************/
#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QList>

QT_CHARTS_USE_NAMESPACE
//Macros for Graph
#define MINIMUM_SAMPLE_COUNT 100
#define PARAMETER_COUNT_PER_SIGNATURE_SAMPLE 4

#define POINT_NOT_FOUND     -1
#define POINT_FOUND         0
#define PARAM_MAX_VAL       ((unsigned int)-1)
#define PARAM_MIN_VAL       0

#define PHASE_MAX_VAL       180
#define PHASE_MIN_VAL       -180

class MySplineSeries;
class GraphProperties;
/**
 * @brief The Graph class : handles graph in test mode
 */
class Graph:public QObject
{
    Q_OBJECT
public:
    QList <MySplineSeries*> m_splineList;
    QList <GraphProperties*> m_axisProperties;
    QList <GraphProperties*> m_graphProperties;
    QString m_GraphTime ;

    Graph(QObject*ptr=0);

    Q_PROPERTY(double XAxisValue  READ getXAxisValue WRITE setXAxisValue NOTIFY XAxisValueChanged)
    Q_PROPERTY(QString GraphTime  READ getGraphTime WRITE setGraphTime NOTIFY GraphTimeChanged)

    Q_INVOKABLE QAbstractSeries* copy(QSplineSeries*);
    Q_INVOKABLE void addAxes();
    Q_INVOKABLE void addGraph();
    Q_INVOKABLE QString colorAtAxis(int );
    Q_INVOKABLE QString nameAtAxis(int );
    Q_INVOKABLE QString colorAtGraph(int );
    Q_INVOKABLE QString nameAtGraph(int );
    Q_INVOKABLE int noOfGraphs();
    Q_INVOKABLE int getSignatureData();
    Q_INVOKABLE QPointF getMin(QSplineSeries * );
    Q_INVOKABLE QPointF getMax(QSplineSeries * );
    Q_INVOKABLE void setVisibility(QSplineSeries *,QAbstractAxis* , bool );
    Q_INVOKABLE QPointF getAtIndex(QSplineSeries * ,int );
    Q_INVOKABLE  QPointF getMinPoint(int);
    Q_INVOKABLE QPointF getMaxPoint(int );
    Q_INVOKABLE void setMin_Max(QAbstractAxis* ,qreal ,qreal );
    Q_INVOKABLE void clearLists();
    Q_INVOKABLE void addAxesAndSplines();
    QString getGraphTime();
    void setGraphTime(QString a_GraphTime);
    void setXAxisValue(double a_XAxisValue);
    double m_XAxisValue;
    double getXAxisValue();

    ~Graph()
    {
    }

public slots:
    void replaceSample(int index);

signals:
    void XAxisValueChanged();
    void GraphTimeChanged();
};

class MySplineSeries:public QSplineSeries
{
public:
    QList< QPointF> m_splinePoints;
    ~MySplineSeries()
    {
    }
};

class GraphProperties:public QObject
{
    Q_OBJECT
public:
    GraphProperties(QString a_name,QString a_color,QObject *ptr=nullptr);

    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString color READ getColor)

    QString getName();
    QString getColor();
    QString m_name;
    QString m_color;
};
#endif // GRAPH_H

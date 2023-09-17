/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Horn Scan Data

***************************************************************************/
#ifndef HORNSCANDATA_H
#define HORNSCANDATA_H

#include <QObject>
#include <QString>
#include <QFile>
#include<QTextStream>

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>

#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#ifndef HORNRESULTDATA_H
#include "Header/hornresultdata.h"
#endif

#define PARAM_MAX_VAL ((unsigned int)-1)
#define PARAM_MIN_VAL 0

#define PHASE_MAX_VAL 180
#define PHASE_MIN_VAL -180

const int AMPTITUDE_SCAN_DATA_INDEX = 1;
const int CURRENT_SCAN_DATA_INDEX = 2;
const int PHASE_SCAN_DATA_INDEX = 3;

enum
{
    AMP_SCAN_PARAM_IND = 0,
    CURRENT_SCAN_PARAM_IND,
    PHASE_SCAN_PARAM_IND,
    IMPEDANCE_SCAN_PARAM_IND,
    FREQ_SCAN_PARAM_IND
};

struct t_points
{
    int amplitude ;
    int current;
    float phase;
    int impedance;
};

#ifndef COMMUNICATIONINTERFACE_H
#include "Header/communicationinterface.h"
#endif

QT_CHARTS_USE_NAMESPACE

#define MINIMUM_SAMPLE_COUNT 100
#define PARAMETER_COUNT_PER_SIGNATURE_SAMPLE 4

#define POINT_NOT_FOUND -1
#define POINT_FOUND 0

//#define DEBUG

using namespace std ;

//!  A hornScanData class.
/*!
  This class derived from QObject, handles the back end interface to hold the
 scan data for plotting the graph.
*/
class HornScanData : public QObject
{
    Q_OBJECT

public:

    // constructor.
    /*
      This Constructor initializes and creates HornScanData object.
       * */
    explicit HornScanData();
    // Destructor.
    /*
      This Destructor destroys HornScanData object.
       * */
    ~HornScanData();

    Q_INVOKABLE int getHornSignatureData(QString a_historyId);
    Q_INVOKABLE void init();

public slots:
    void replaceSample(QAbstractSeries *a_series , quint8 a_type);

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

    void setAxisMinParameters(QList<qreal> &a_axisVal);
    void setAxisMaxParameters(QList<qreal> &a_axisVal);
    QList<qreal> getAxisMinParameters();
    QList<qreal> getAxisMaxParameters();

private:

    QList<QPointF> m_AmpPoints ;
    QList<QPointF> m_PhasePoints ;
    QList<QPointF> m_CurrentPoints ;
    QList<QPointF> m_ImpedancePoints;

    /* Axis Parameters */
    QList<qreal> m_axisMinParameters ;
    QList<qreal> m_axisMaxParameters;

};


#endif // HORNSCANDATA_H

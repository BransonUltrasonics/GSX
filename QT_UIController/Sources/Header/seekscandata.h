/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Seek Scan Data

***************************************************************************/
#ifndef SEEKSCANDATA_H
#define SEEKSCANDATA_H

#include <QObject>
#include <QString>
#include <QFile>
#include<QTextStream>

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>


#ifndef QTCOMMON_H
#include "Header/qtcommon.h"
#endif

#ifndef SEEKRESULTDATA_H
#include "Header/seekresultdata.h"
#endif

enum
{
    SEEK_AMP_SCAN_PARAM_IND = 0,
    SEEK_FREQ_SCAN_PARAM_IND,
    SEEK_POWER_SCAN_PARAM_IND,
    SEEK_CURRENT_SCAN_PARAM_IND,
    SEEK_PHASE_SCAN_PARAM_IND,
    SEEK_TIME_SCAN_PARAM_IND
};

struct t_seekpoints
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
#define POINT_NOT_FOUND -1
#define POINT_FOUND 0

//#define DEBUG

using namespace std ;

//!  A SeekScanData class.
/*!
  This class derived from QObject, handles the back end interface to hold the
 scan data for plotting the graph.
*/
class SeekScanData : public QObject
{
    Q_OBJECT

public:

    // constructor.
    /*
      This Constructor initializes and creates SeekScanData object.
       * */
    explicit SeekScanData();
    // Destructor.
    /*
      This Destructor destroys SeekScanData object.
       * */
    ~SeekScanData();

    Q_INVOKABLE int getSeekSignatureData(QString a_historyId);
    Q_INVOKABLE void init();

public slots:
    void replaceSeekSample(QAbstractSeries *a_series , quint8 a_type);

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


signals:

private:

    QList<QPointF> m_AmpPoints ;
    QList<QPointF> m_FreqPoints ;
    QList<QPointF> m_PowerPoints ;
    QList<QPointF> m_CurrPoints ;
    QList<QPointF> m_PhasePoints ;

    QList<qreal> m_axisMinParameters ;
    QList<qreal> m_axisMaxParameters;
};


#endif // SEEKSCANDATA_H



#ifndef TRENDSGRAPH_H
#define TRENDSGRAPH_H

#include <QObject>
#include <QQmlContext>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QList>
#include <Header/communicationinterface.h>
#include <Header/qtcommon.h>
QT_CHARTS_USE_NAMESPACE

class TrendsGraph : public QObject
{
    Q_OBJECT
public:
    TrendsGraph(QObject *parent = nullptr);

    QList<QObject*> m_list;
    QList<QObject*> m_listdummy;

    int m_countValueX;
    int m_tickYCount;

    int m_suspectLimitHigh;
    int m_rejectLimitHigh;
    int m_suspectLimitLow;
    int m_rejectLimitLow;

    int m_timeSuspectLimitHigh;
    int m_timeRejectLimitHigh;
    int m_timeSuspectLimitLow;
    int m_timeRejectLimitLow;
    int m_energySuspectLimitHigh;
    int m_energyRejectLimitHigh;
    int m_energySuspectLimitLow;
    int m_energyRejectLimitLow;
    int m_peakPowerSuspectLimitHigh;
    int m_peakPowerRejectLimitHigh;
    int m_peakPowerSuspectLimitLow;
    int m_peakPowerRejectLimitLow;
    int m_absoluteDistSuspectLimitHigh;
    int m_absoluteDistRejectLimitHigh;
    int m_absoluteDistSuspectLimitLow;
    int m_absoluteDistRejectLimitLow;
    int m_frequencySuspectLimitHigh;
    int m_frequencyRejectLimitHigh;
    int m_frequencySuspectLimitLow;
    int m_frequencyRejectLimitLow;
    int m_maxWeldForceSuspectLimitHigh;
    int m_maxWeldForceRejectLimitHigh;
    int m_maxWeldForceSuspectLimitLow;
    int m_maxWeldForceRejectLimitLow;

    int m_minWeldForceYValue;
    int m_maxWeldForceYValue;
    int m_minHoldForceYValue;
    int m_maxHoldForceYValue;
    int m_minTotalAbsoluteYValue;
    int m_maxTotalAbsoluteYValue;
    int m_minPartContactPosYValue;
    int m_maxPartContactPosYValue;
    int m_minWeldTimeYValue;
    int m_maxWeldTimeYValue;
    int m_minWeldEnergyYValue;
    int m_maxWeldEnergyYValue;
    int m_minWeldPeakPowerYValue;
    int m_maxWeldPeakPowerYValue;
    int m_minStartFrequencyYValue;
    int m_maxStartFrequencyYValue;
    int m_minFrequencyChangeYValue;
    int m_maxFrequencyChangeYValue;

    Q_PROPERTY(int countValueX READ getXCount WRITE setXCount NOTIFY valueXChanged)
    Q_PROPERTY(int tickCountY READ getTickCountY WRITE setTickCountY NOTIFY tickCountYChanged)

    Q_PROPERTY(int timeSuspectLimitHigh READ getTimeSuspectLimitHigh WRITE setTimeSuspectLimitHigh NOTIFY timeSuspectLimitHighChanged)
    Q_PROPERTY(int timeRejectLimitHigh READ getTimeRejectLimitHigh WRITE setTimeRejectLimitHigh NOTIFY timeRejectLimitHighChanged)
    Q_PROPERTY(int timeRejectLimitLow READ getTimeRejectLimitLow WRITE setTimeRejectLimitLow NOTIFY timeRejectLimitLowChanged)
    Q_PROPERTY(int timeSuspectLimitLow READ getTimeSuspectLimitLow WRITE setTimeSuspectLimitLow NOTIFY timeSuspectLimitLowChanged)

    Q_PROPERTY(int energySuspectLimitHigh READ getEnergySuspectLimitHigh WRITE setEnergySuspectLimitHigh NOTIFY energySuspectLimitHighChanged)
    Q_PROPERTY(int energyRejectLimitHigh READ getEnergyRejectLimitHigh WRITE setEnergyRejectLimitHigh NOTIFY energyRejectLimitHighChanged)
    Q_PROPERTY(int energyRejectLimitLow READ getEnergyRejectLimitLow WRITE setEnergyRejectLimitLow NOTIFY energyRejectLimitLowChanged)
    Q_PROPERTY(int energySuspectLimitLow READ getEnergySuspectLimitLow WRITE setEnergySuspectLimitLow NOTIFY energySuspectLimitLowChanged)

    Q_PROPERTY(int peakPowerSuspectLimitHigh READ getPeakPowerSuspectLimitHigh WRITE setPeakPowerSuspectLimitHigh NOTIFY peakPowerSuspectLimitHighChanged)
    Q_PROPERTY(int peakPowerRejectLimitHigh READ getPeakPowerRejectLimitHigh WRITE setPeakPowerRejectLimitHigh NOTIFY peakPowerRejectLimitHighChanged)
    Q_PROPERTY(int peakPowerRejectLimitLow READ getPeakPowerRejectLimitLow WRITE setPeakPowerRejectLimitLow NOTIFY peakPowerRejectLimitLowChanged)
    Q_PROPERTY(int peakPowerSuspectLimitLow READ getPeakPowerSuspectLimitLow WRITE setPeakPowerSuspectLimitLow NOTIFY peakPowerSuspectLimitLowChanged)

    Q_PROPERTY(int absoluteDistSuspectLimitHigh READ getAbsoluteDistSuspectLimitHigh WRITE setAbsoluteDistSuspectLimitHigh NOTIFY absoluteDistSuspectLimitHighChanged)
    Q_PROPERTY(int absoluteDistRejectLimitHigh READ getAbsoluteDistRejectLimitHigh WRITE setAbsoluteDistRejectLimitHigh NOTIFY absoluteDistRejectLimitHighChanged)
    Q_PROPERTY(int absoluteDistRejectLimitLow READ getAbsoluteDistRejectLimitLow WRITE setAbsoluteDistRejectLimitLow NOTIFY absoluteDistRejectLimitLowChanged)
    Q_PROPERTY(int absoluteDistSuspectLimitLow READ getAbsoluteDistSuspectLimitLow WRITE setAbsoluteDistSuspectLimitLow NOTIFY absoluteDistSuspectLimitLowChanged)

    Q_PROPERTY(int frequencySuspectLimitHigh READ getFrequencySuspectLimitHigh WRITE setFrequencySuspectLimitHigh NOTIFY frequencySuspectLimitHighChanged)
    Q_PROPERTY(int frequencyRejectLimitHigh READ getFrequencyRejectLimitHigh WRITE setFrequencyRejectLimitHigh NOTIFY frequencyRejectLimitHighChanged)
    Q_PROPERTY(int frequencyRejectLimitLow READ getFrequencyRejectLimitLow WRITE setFrequencyRejectLimitLow NOTIFY frequencyRejectLimitLowChanged)
    Q_PROPERTY(int frequencySuspectLimitLow READ getFrequencySuspectLimitLow WRITE setFrequencySuspectLimitLow NOTIFY frequencySuspectLimitLowChanged)

    Q_PROPERTY(int maxWeldForceSuspectLimitHigh READ getMaxWeldForceSuspectLimitHigh WRITE setMaxWeldForceSuspectLimitHigh NOTIFY maxWeldForceSuspectLimitHighChanged)
    Q_PROPERTY(int maxWeldForceRejectLimitHigh READ getMaxWeldForceRejectLimitHigh WRITE setMaxWeldForceRejectLimitHigh NOTIFY maxWeldForceRejectLimitHighChanged)
    Q_PROPERTY(int maxWeldForceRejectLimitLow READ getMaxWeldForceRejectLimitLow WRITE setMaxWeldForceRejectLimitLow NOTIFY maxWeldForceRejectLimitLowChanged)
    Q_PROPERTY(int maxWeldForceSuspectLimitLow READ getMaxWeldForceSuspectLimitLow WRITE setMaxWeldForceSuspectLimitLow NOTIFY maxWeldForceSuspectLimitLowChanged)

    Q_PROPERTY(int suspectLimitHigh READ getSuspectLimitHigh WRITE setSuspectLimitHigh NOTIFY suspectLimitHighChanged)
    Q_PROPERTY(int rejectLimitHigh READ getRejectLimitHigh WRITE setRejectLimitHigh NOTIFY rejectLimitHighChanged)
    Q_PROPERTY(int rejectLimitLow READ getRejectLimitLow WRITE setRejectLimitLow NOTIFY rejectLimitLowChanged)
    Q_PROPERTY(int suspectLimitLow READ getSuspectLimitLow WRITE setSuspectLimitLow NOTIFY suspectLimitLowChanged)

    Q_PROPERTY(int minWeldForceYValue READ getMinWeldForceYValue WRITE setMinWeldForceYValue NOTIFY minWeldForceYValueChanged)
    Q_PROPERTY(int maxWeldForceYValue READ getMaxWeldForceYValue WRITE setMaxWeldForceYValue NOTIFY maxWeldForceYValueChanged)

    Q_PROPERTY(int minHoldForceYValue READ getMinHoldForceYValue WRITE setMinHoldForceYValue NOTIFY minHoldForceYValueChanged)
    Q_PROPERTY(int maxHoldForceYValue READ getMaxHoldForceYValue WRITE setMaxHoldForceYValue NOTIFY maxHoldForceYValueChanged)

    Q_PROPERTY(int minTotalAbsoluteYValue READ getMinTotalAbsoluteYValue WRITE setMinTotalAbsoluteYValue NOTIFY minTotalAbsoluteYValueChanged)
    Q_PROPERTY(int maxTotalAbsoluteYValue READ getMaxTotalAbsoluteYValue WRITE setMaxTotalAbsoluteYValue NOTIFY maxTotalAbsoluteYValueChanged)

    Q_PROPERTY(long minPartContactPosYValue READ getMinPartContactPosYValue WRITE setMinPartContactPosYValue NOTIFY minPartContactPosYValueChanged)
    Q_PROPERTY(long maxPartContactPosYValue READ getMaxPartContactPosYValue WRITE setMaxPartContactPosYValue NOTIFY maxPartContactPosYValueChanged)

    Q_PROPERTY(int minWeldTimeYValue READ getMinWeldTimeYValue WRITE setMinWeldTimeYValue NOTIFY minWeldTimeYValueChanged)
    Q_PROPERTY(int maxWeldTimeYValue READ getMaxWeldTimeYValue WRITE setMaxWeldTimeYValue NOTIFY maxWeldTimeYValueChanged)

    Q_PROPERTY(int minWeldEnergyYValue READ getMinWeldEnergyYValue WRITE setMinWeldEnergyYValue NOTIFY minWeldEnergyYValueChanged)
    Q_PROPERTY(int maxWeldEnergyYValue READ getMaxWeldEnergyYValue WRITE setMaxWeldEnergyYValue NOTIFY maxWeldEnergyYValueChanged)

    Q_PROPERTY(int minWeldPeakPowerYValue READ getMinWeldPeakPowerYValue WRITE setMinWeldPeakPowerYValue NOTIFY minWeldPeakPowerYValueChanged)
    Q_PROPERTY(int maxWeldPeakPowerYValue READ getMaxWeldPeakPowerYValue WRITE setMaxWeldPeakPowerYValue NOTIFY maxWeldPeakPowerYValueChanged)

    Q_PROPERTY(int minStartFrequencyYValue READ getMinStartFrequencyYValue WRITE setMinStartFrequencyYValue NOTIFY minStartFrequencyYValueChanged)
    Q_PROPERTY(int maxStartFrequencyYValue READ getMaxStartFrequencyYValue WRITE setMaxStartFrequencyYValue NOTIFY maxStartFrequencyYValueChanged)

    Q_PROPERTY(int minFrequencyChangeYValue READ getMinFrequencyChangeYValue WRITE setMinFrequencyChangeYValue NOTIFY minFrequencyChangeYValueChanged)
    Q_PROPERTY(int maxFrequencyChangeYValue READ getMaxFrequencyChangeYValue WRITE setMaxFrequencyChangeYValue NOTIFY maxFrequencyChangeYValueChanged)

    QList<QPointF> weldForceList;
    QList<QPointF> holdForceList;
    QList<QPointF> totalAbsoluteList;
    QList<QPointF> partContactPositionList;
    QList<QPointF> weldTimeList;
    QList<QPointF> weldEnergyList;
    QList<QPointF> weldPeakPowerList;
    QList<QPointF> startFrequencyList;
    QList<QPointF> frequencyChangeList;

    QList<int> weldForceListYValue;
    QList<int> holdForceListYValue;
    QList<int> totalAbsoluteListYValue;
    QList<int> partContactPositionListYValue;
    QList<int> weldTimeListYValue;
    QList<int> weldEnergyListYValue;
    QList<int> weldPeakPowerListYValue;
    QList<int> startFrequencyListYValue;
    QList<int> frequencyChangeListYValue;
    QList<int> cycleCounterList;
    QList<int> alarmFlagList;

    Q_INVOKABLE QPointF getGraphValueByIndex(int gIndex,int typeIndex);

    Q_INVOKABLE void trendsGraphData(QString recipeNumber);

    Q_INVOKABLE int getTrendsGraphSignatureData(QString a_historyId);

    Q_INVOKABLE int getYAxisMaxValues(int);

    Q_INVOKABLE int getYAxisMinValues(int);

    Q_INVOKABLE void getGraphParametersName();

    Q_INVOKABLE void getCycleConterValues();

    Q_INVOKABLE int getSuspectRejectParamLimits(QString);

    Q_INVOKABLE int getXCount();

    Q_INVOKABLE int getAlarmFlag(int index);

    Q_INVOKABLE void getTopScaleModel();

    Q_INVOKABLE QString  getBrushColorForAxis();

    void setXCount(int);
    int getTickCountY();
    void setTickCountY(int);

    int getMinWeldForceYValue();
    void setMinWeldForceYValue(int);
    int getMaxWeldForceYValue();
    void setMaxWeldForceYValue(int);

    int getMinHoldForceYValue();
    void setMinHoldForceYValue(int);
    int getMaxHoldForceYValue();
    void setMaxHoldForceYValue(int);

    int getMinTotalAbsoluteYValue();
    void setMinTotalAbsoluteYValue(int);
    int getMaxTotalAbsoluteYValue();
    void setMaxTotalAbsoluteYValue(int);

    int getMinPartContactPosYValue();
    void setMinPartContactPosYValue(int);
    int getMaxPartContactPosYValue();
    void setMaxPartContactPosYValue(int);

    int getMinWeldTimeYValue();
    void setMinWeldTimeYValue(int);
    int getMaxWeldTimeYValue();
    void setMaxWeldTimeYValue(int);

    int getMinWeldEnergyYValue();
    void setMinWeldEnergyYValue(int);
    int getMaxWeldEnergyYValue();
    void setMaxWeldEnergyYValue(int);

    int getMinWeldPeakPowerYValue();
    void setMinWeldPeakPowerYValue(int);
    int getMaxWeldPeakPowerYValue();
    void setMaxWeldPeakPowerYValue(int);

    int getMinStartFrequencyYValue();
    void setMinStartFrequencyYValue(int);
    int getMaxStartFrequencyYValue();
    void setMaxStartFrequencyYValue(int);

    int getMinFrequencyChangeYValue();
    void setMinFrequencyChangeYValue(int);
    int getMaxFrequencyChangeYValue();
    void setMaxFrequencyChangeYValue(int);

    int getSuspectLimitHigh();
    void setSuspectLimitHigh(int);
    int getRejectLimitHigh();
    void setRejectLimitHigh(int);
    int getRejectLimitLow();
    void setRejectLimitLow(int);
    int getSuspectLimitLow();
    void setSuspectLimitLow(int);

    int getTimeSuspectLimitHigh();
    void setTimeSuspectLimitHigh(int);
    int getTimeRejectLimitHigh();
    void setTimeRejectLimitHigh(int);
    int getTimeRejectLimitLow();
    void setTimeRejectLimitLow(int);
    int getTimeSuspectLimitLow();
    void setTimeSuspectLimitLow(int);

    int getEnergySuspectLimitHigh();
    void setEnergySuspectLimitHigh(int);
    int getEnergyRejectLimitHigh();
    void setEnergyRejectLimitHigh(int);
    int getEnergyRejectLimitLow();
    void setEnergyRejectLimitLow(int);
    int getEnergySuspectLimitLow();
    void setEnergySuspectLimitLow(int);

    int getPeakPowerSuspectLimitHigh();
    void setPeakPowerSuspectLimitHigh(int);
    int getPeakPowerRejectLimitHigh();
    void setPeakPowerRejectLimitHigh(int);
    int getPeakPowerRejectLimitLow();
    void setPeakPowerRejectLimitLow(int);
    int getPeakPowerSuspectLimitLow();
    void setPeakPowerSuspectLimitLow(int);

    int getAbsoluteDistSuspectLimitHigh();
    void setAbsoluteDistSuspectLimitHigh(int);
    int getAbsoluteDistRejectLimitHigh();
    void setAbsoluteDistRejectLimitHigh(int);
    int getAbsoluteDistRejectLimitLow();
    void setAbsoluteDistRejectLimitLow(int);
    int getAbsoluteDistSuspectLimitLow();
    void setAbsoluteDistSuspectLimitLow(int);

    int getFrequencySuspectLimitHigh();
    void setFrequencySuspectLimitHigh(int);
    int getFrequencyRejectLimitHigh();
    void setFrequencyRejectLimitHigh(int);
    int getFrequencyRejectLimitLow();
    void setFrequencyRejectLimitLow(int);
    int getFrequencySuspectLimitLow();
    void setFrequencySuspectLimitLow(int);

    int getMaxWeldForceSuspectLimitHigh();
    void setMaxWeldForceSuspectLimitHigh(int);
    int getMaxWeldForceRejectLimitHigh();
    void setMaxWeldForceRejectLimitHigh(int);
    int getMaxWeldForceRejectLimitLow();
    void setMaxWeldForceRejectLimitLow(int);
    int getMaxWeldForceSuspectLimitLow();
    void setMaxWeldForceSuspectLimitLow(int);
    ~TrendsGraph(){
    }
signals:
    void valueXChanged();
    void tickCountYChanged();

    void suspectLimitHighChanged();
    void rejectLimitHighChanged();
    void rejectLimitLowChanged();
    void suspectLimitLowChanged();

    void timeSuspectLimitHighChanged();
    void timeRejectLimitHighChanged();
    void timeRejectLimitLowChanged();
    void timeSuspectLimitLowChanged();

    void energySuspectLimitHighChanged();
    void energyRejectLimitHighChanged();
    void energyRejectLimitLowChanged();
    void energySuspectLimitLowChanged();

    void peakPowerSuspectLimitHighChanged();
    void peakPowerRejectLimitHighChanged();
    void peakPowerRejectLimitLowChanged();
    void peakPowerSuspectLimitLowChanged();

    void absoluteDistSuspectLimitHighChanged();
    void absoluteDistRejectLimitHighChanged();
    void absoluteDistRejectLimitLowChanged();
    void absoluteDistSuspectLimitLowChanged();

    void frequencySuspectLimitHighChanged();
    void frequencyRejectLimitHighChanged();
    void frequencyRejectLimitLowChanged();
    void frequencySuspectLimitLowChanged();

    void maxWeldForceSuspectLimitHighChanged();
    void maxWeldForceRejectLimitHighChanged();
    void maxWeldForceRejectLimitLowChanged();
    void maxWeldForceSuspectLimitLowChanged();

    void minWeldForceYValueChanged();
    void maxWeldForceYValueChanged();
    void minHoldForceYValueChanged();
    void maxHoldForceYValueChanged();
    void minTotalAbsoluteYValueChanged();
    void maxTotalAbsoluteYValueChanged();
    void minPartContactPosYValueChanged();
    void maxPartContactPosYValueChanged();
    void minWeldTimeYValueChanged();
    void maxWeldTimeYValueChanged();
    void minWeldEnergyYValueChanged();
    void maxWeldEnergyYValueChanged();
    void minWeldPeakPowerYValueChanged();
    void maxWeldPeakPowerYValueChanged();
    void minStartFrequencyYValueChanged();
    void maxStartFrequencyYValueChanged();
    void minFrequencyChangeYValueChanged();
    void maxFrequencyChangeYValueChanged();

private:
    void clearModel();

};

#endif // TRENDSGRAPH_H

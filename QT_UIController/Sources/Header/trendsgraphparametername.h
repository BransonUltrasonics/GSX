#ifndef TRENDSGRAPHPARAMETERNAME_H
#define TRENDSGRAPHPARAMETERNAME_H

#include <QObject>

class Trendsgraphparametername : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString parameterName READ getParameterName WRITE setParameterName NOTIFY parameterNameChanged)
    Q_PROPERTY(QString cycleCounterVal READ getCycleCounterVal WRITE setCycleCounterVal NOTIFY cycleCounterValChanged)


public:
    QString m_ParameterName;
    QString m_CycleCounterVal;
    Trendsgraphparametername(QObject *parent=0);
    Trendsgraphparametername(const QString name, QObject *parent=0);
    QString getParameterName() const;
    QString getCycleCounterVal() const;
    void setParameterName(const QString parameter);
    void setCycleCounterVal(const QString parameter);

signals:
    void parameterNameChanged();
    void cycleCounterValChanged();

};

class TrendsGraphCycleCounterVal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cycleCountName READ getCycleCountName NOTIFY cycleCountNameChanged)
    Q_PROPERTY(int alarmFlag READ getAlarmFlag NOTIFY alarmFlagChanged)

public :
    TrendsGraphCycleCounterVal(const QString &a_cycleCountName, const int &a_alarmFlag);
    ~TrendsGraphCycleCounterVal();

    QString getCycleCountName();
    int getAlarmFlag();

signals:
    void cycleCountNameChanged();
    void alarmFlagChanged();

public:
    QString m_cycleCountName;
    int m_alarmFlag;
};

#endif // TRENDSGRAPHPARAMETERNAME_H

#ifndef ANALYTICSPRODUCTION_H
#define ANALYTICSPRODUCTION_H
#include <QObject>

#define PRODUCTION_OVR_SPLITTER             ","
#define PRODUCTION_OVR_NAME_WELD            "Weld"
#define PRODUCTION_OVR_NAME_ALARMS          "Alarms"
#define PRODUCTION_OVR_NAME_RATE            "Rate"
#define PRODUCTION_OVR_UNIT_RLIMITS         "Reject Limits"
#define PRODUCTION_OVR_UNIT_ERRORS          "Errors"
#define PRODUCTION_OVR_UNIT_PARTPERMIN      "Part/Min"
#define PRODUCTION_OVR_BAR_COLOR_WELD       "#8BC24A"
#define PRODUCTION_OVR_BAR_COLOR_ALARMS     "#F59F00"
#define PRODUCTION_OVR_BAR_COLOR_RATE       "#68A4EB"

class AnalyticsProdcution: public QObject
{
    Q_OBJECT
public:
    QList<QObject*> m_productionList;
    QString m_productionName,m_productionUnit,m_productionBarColor,m_avgValue;
    QString m_sunVal,m_monVal,m_tueVal,m_wedVal,m_thrVal,m_friVal,m_satVal;

    AnalyticsProdcution(QString productionName, QString sunVal, QString monVal, QString tueVal, QString wedVal, QString thrVal ,QString friVal ,QString satVal ,QString productionUnit, QString productionBarColor,QString avgValue);
    explicit AnalyticsProdcution(QObject *parent = 0);

    Q_PROPERTY(QString productionName READ getproductionName NOTIFY OnproductionName)
    Q_PROPERTY(QString sunVal READ getsunVal NOTIFY OnsunVal)
    Q_PROPERTY(QString monVal READ getmonVal NOTIFY OnmonVal)
    Q_PROPERTY(QString tueVal READ gettueVal NOTIFY OntueVal)
    Q_PROPERTY(QString wedVal READ getwedVal NOTIFY OnwedVal)
    Q_PROPERTY(QString thrVal READ getthrVal NOTIFY OnthrVal)
    Q_PROPERTY(QString friVal READ getfriVal NOTIFY OnfriVal)
    Q_PROPERTY(QString satVal READ getsatVal NOTIFY OnsatVal)
    Q_PROPERTY(QString productionUnit READ getproductionUnit NOTIFY OnproductionUnit)
    Q_PROPERTY(QString productionBarColor READ getproductionBarColor NOTIFY OnproductionBarColor)
    Q_PROPERTY(QString avgVal READ getavgVal NOTIFY OnavgVal)

    Q_INVOKABLE void setAnalyticsDateProductionList(QString ,QString);
    Q_INVOKABLE QString startDateofCurrentWeek();
    Q_INVOKABLE QString endDateofCurrentWeek();
    Q_INVOKABLE QString startDateofWeek(QString);
    Q_INVOKABLE QString endDateofWeek(QString);
    Q_INVOKABLE QString clickNextWeek(QString);
    Q_INVOKABLE QString clickPreviousWeek(QString);
    QString sendDateParams(QString buffer);
    void updateAnalyticsProductiondetails(QString);
    void defaultAnalyticsProductiondetails();

    QString getproductionName();
    QString getsunVal();
    QString getmonVal();
    QString gettueVal();
    QString getwedVal();
    QString getthrVal();
    QString getfriVal();
    QString getsatVal();
    QString getproductionUnit();
    QString getproductionBarColor();
    QString getavgVal();

signals:
    void OnproductionName();
    void OnsunVal();
    void OnmonVal();
    void OntueVal();
    void OnwedVal();
    void OnthrVal();
    void OnfriVal();
    void OnsatVal();
    void OnproductionUnit();
    void OnproductionBarColor();
    void OnavgVal();

private:
    void clearList();
};

#endif // ANALYTICSPRODUCTION_H

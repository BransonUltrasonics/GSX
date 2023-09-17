#ifndef DATAWIZARD_H
#define DATAWIZARD_H

#include <QObject>
#include <QQmlEngine>
#include <QDebug>
#include <QQmlContext>
#include <QDateTime>
#include "Header/alarmconfiguration.h"

#define PRINTENABLE 1

#define DATAREPORTTYPE_URI_NAME "com.branson.datawizard"
#define DATAREPORTTYPE_MAJOR_VERSION    1
#define DATAREPORTTYPE_MINIOR_VERSION   0
#define DATAREPORTTYPE_NAME "DataReportType"

#define DATAWIZARD_REPORT_CONTINOUS_STR                 QObject::tr("Continous Report")
#define DATAWIZARD_REPORT_SINGLE_STR                    QObject::tr("Single Report")
#define DATAWIZARD_FREQUENCY_TIME_STR                   QObject::tr("Time")
#define DATAWIZARD_FREQUENCY_CYCLE_STR                  QObject::tr("Cycles")
#define DATAWIZARD_FREQUENCY_TIME_PERIOD_DAILY_STR      QObject::tr("Daily")
#define DATAWIZARD_FREQUENCY_TIME_PERIOD_MONTHLY_STR    QObject::tr("Monthly")
#define DATAWIZARD_FREQUENCY_TIME_PERIOD_WEEKLY_STR     QObject::tr("Weekly")
#define DATAWIZARD_DATAFILTER_WELD_DATA_STR             QObject::tr("Weld Data")
#define DATAWIZARD_DATAFILTER_ALARM_DATA_STR            QObject::tr("Alarm Data")
#define DATAWIZARD_DATAFILTER_USER_DATA_STR             QObject::tr("User Data")
#define DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA_STR   QObject::tr("System Settings")
#define DATAWIZARD_DATAFILTER_EVENT_DATA_STR            QObject::tr("Event Data")
#define DATAWIZARD_DATAFILTER_RECIPE_DATA_STR           QObject::tr("Recipe Data")
#define DATA_WIZARD_DATAFILTER_WELDDATA_RESULTS         QObject::tr("Weld Results")
#define DATA_WIZARD_DATAFILTER_WELD_SIGNATURE           QObject::tr("Weld Signature")
#define DATA_WIZARD_DATAFILTER_WELDDATA_GRAPH           QObject::tr("Weld Graph")
#define DATA_WIZARD_DATAFILTER_WELD_GRAPH_DATA          QObject::tr("Weld Graph Data")
#define DATA_WIZARD_DATAFILTER_OTHER_OPTION             QObject::tr("Other Option")
#define DATA_WIZARD_DATAFILTER_DELETE_DB_RECORDS        QObject::tr("Delete DB Records")
#define DATA_WIZARD_DATAFILTER_WELDDATA_RECIPES         QObject::tr("Recipes")
#define DATA_WIZARD_DATAFILTER_WELDDATA_PARAMETER       QObject::tr("Parameter")
#define DATA_WIZARD_DATAFILTER_WELDDATA_OTHER           QObject::tr("Other")
#define DATAWIZARD_STORAGE_USB_STR                      QObject::tr("USB")
#define DATAWIZARD_STORAGE_CLOUD_STR                    QObject::tr("Cloud")
#define DATAWIZARD_STORAGE_BLUETOOTH_STR                QObject::tr("Bluetooth")
#define DATAWIZARD_STORAGE_LOCALSTORAGE_STR             QObject::tr("Local Storage")
#define DATAWIZARD_SUMMARY_FORMAT                       QObject::tr("Format")

#define WELD_DATA                                       QObject::tr("Weld Data")
#define ALARM_DATA                                      QObject::tr("Alarm Data")
#define SYSTEM_DATA                                     QObject::tr("System Settings")
#define EVENT_DATA                                      QObject::tr("Event Data")
namespace DataWizardspace
{
Q_NAMESPACE
enum DataWizardStep
{
    DATAWIZARD_STEP_TYPE = 1,
    //DATAWIZARD_STEP_FREQUENCY = 2,
    DATAWIZARD_STEP_OUTPUT = 2,
    DATAWIZARD_STEP_STORAGEOPTIONS = 3,
    DATAWIZARD_STEP_DATAFILTER = 4,
    DATAWIZARD_STEP_SUMMARY = 5

};
Q_ENUM_NS(DataWizardStep)
}
struct WeldDataStruct
{
   int resultExport;
   int resultDelete;
   int recipeExport;
   int recipeDelete;
   int graphExport;
   int graphDelete;
};
struct AlarmDataStruct
{
   int overloadExport;
   int overloadDelete;
   int cyclemodifyExport;
   int cyclemodifyDelete;
   int warningExport;
   int warningDelete;
   int suspectExport;
   int suspectDelete;
   int rejectExport;
   int rejectDelete;
   int nocycleExport;
   int nocycleDelete;
   int hardwareFailExport;
   int hardwareFailDelete;
   int nocycleOverloadExport;
   int nocycleOverloadDelete;
};
struct SystemSettingStruct
{
   int systemExport;
   int systemDelete;

};
struct EventDataStruct
{
   int eventExport;
   int eventDelete;

};
class DataWizard : public QObject
{
    Q_OBJECT
public:
    Q_ENUMS(DataWizardStep)
    Q_ENUMS(DataWizardEnums)
    Q_ENUMS(DataWizardDataFilters)
    Q_PROPERTY(DataWizardEnums SelectedReportType READ getSelectedReportType WRITE setSelectedReportType NOTIFY selectedReportTypeChanged)
    Q_PROPERTY(DataWizardEnums SelectedFrequencyType READ getSelectedFrequencyType WRITE setSelectedFrequencyType NOTIFY selectedFrequencyTypeChanged)
    Q_PROPERTY(DataWizardEnums SelectedFrequencyPeriodType READ getSelectedFrequencyPeriodType WRITE setSelectedFrequencyPeriodType NOTIFY selectedFrequencyPeriodTypeChanged)
    Q_PROPERTY(QDateTime SelectedFrequencyPeriodDate READ getSelectedFrequencyPeriodDate WRITE setSelectedFrequencyPeriodDate NOTIFY selectedFrequencyPeriodDateChanged)
    Q_PROPERTY(QString SelectedFrequencyPeriodTime READ getSelectedFrequencyPeriodTime WRITE setSelectedFrequencyPeriodTime NOTIFY selectedFrequencyPeriodTimeChanged)
    Q_PROPERTY(QString SelectedOutputTime READ getSelectedOutputTime WRITE setSelectedOutputTime NOTIFY selectedOutputTimeChanged)

    Q_PROPERTY(int SelectedCycles READ getSelectedCycles WRITE setSelectedCycles NOTIFY selectedCyclesChanged)
    Q_PROPERTY(QString cyclesText READ getCyclesText WRITE setCyclesText NOTIFY cyclesTextChanged)
    Q_PROPERTY(QString frequencyDailyTime READ getFrequencyDailyTime WRITE setFrequencyDailyTime NOTIFY frequencyDailyTimeChanged)

    explicit DataWizard(QObject *parent = nullptr);

    enum DataWizardEnums
    {
        DATAWIZARD_REPORT_CONTINOUS = 0,
        DATAWIZARD_REPORT_SINGLE = 1,
        DATAWIZARD_FREQUENCY_TIME = 2,
        DATAWIZARD_FREQUENCY_CYCLE = 3,

        DATAWIZARD_FREQUENCY_TIME_PERIOD_DAILY = 4,
        DATAWIZARD_FREQUENCY_TIME_PERIOD_WEEKLY = 5,
        DATAWIZARD_FREQUENCY_TIME_PERIOD_MONTHLY = 6,

        //        DATAWIZARD_FREQUENCY_SHIFT
    };
    enum DataWizardDataFilters
    {
        DATAWIZARD_DATAFILTER_WELD_DATA = 0,
        DATAWIZARD_DATAFILTER_ALARM_DATA = 1,
        DATAWIZARD_DATAFILTER_SYSTEMSETTINGS_DATA = 2,
        DATAWIZARD_DATAFILTER_EVENT_DATA = 3,
        DATAWIZARD_DATAFILTER_USER_DATA = 4,
        DATAWIZARD_DATAFILTER_RECIPE_DATA = 5,
        DATAWIZARD_DATAFILTER_WELD_GRAPH_DATA = 6,
        DATAWIZARD_DATAFILTER_RESULT_AND_SIGNATURE = 7,
        DATAWIZARD_DATAFILTER_CSV_DELETE_DB = 8
    };

    enum DataWizardOutputEnums
    {
        DATAWIZARD_OUTPUT_CSV = 0,
        DATAWIZARD_OUTPUT_PDF = 1,
    };

    static void registerQmlType()
    {
        //qmlRegisterType<DataWizard>(DATAREPORTTYPE_URI_NAME, DATAREPORTTYPE_MAJOR_VERSION, DATAREPORTTYPE_MINIOR_VERSION, DATAREPORTTYPE_NAME);
        qmlRegisterUncreatableType<DataWizard>(DATAREPORTTYPE_URI_NAME, DATAREPORTTYPE_MAJOR_VERSION, DATAREPORTTYPE_MINIOR_VERSION, DATAREPORTTYPE_NAME,"Enums of report");

    }
    Q_INVOKABLE DataWizardEnums getSelectedReportType();
    Q_INVOKABLE void setSelectedReportType(DataWizardEnums SelectedReportType);

    Q_INVOKABLE DataWizardEnums getSelectedFrequencyType();
    Q_INVOKABLE void setSelectedFrequencyType(DataWizardEnums SelectedFrequencyType);

    Q_INVOKABLE DataWizardEnums getSelectedFrequencyPeriodType();
    Q_INVOKABLE void setSelectedFrequencyPeriodType(DataWizardEnums SelectedFrequencyPeriodType);

    Q_INVOKABLE QString getSelectedIndexString(DataWizard::DataWizardEnums);
    Q_INVOKABLE int getSelectedStringIndex(QString strSelectedString);

    Q_INVOKABLE QDateTime getSelectedFrequencyPeriodDate();
    Q_INVOKABLE void setSelectedFrequencyPeriodDate(QDateTime currenttime);


    Q_INVOKABLE QString getSelectedFrequencyPeriodTime();
    Q_INVOKABLE void setSelectedFrequencyPeriodTime(QString currenttime);

    Q_INVOKABLE QString getSelectedOutputTime();
    Q_INVOKABLE void setSelectedOutputTime(QString str);

    Q_INVOKABLE int getSelectedCycles();
    Q_INVOKABLE void setSelectedCycles(int CurrentCycle);

    Q_INVOKABLE QString getCyclesText();
    Q_INVOKABLE void setCyclesText(QString cycles);

    Q_INVOKABLE void getSelectedForSingleReport();

    //    Q_INVOKABLE QString getSelectedShifts();
    //    Q_INVOKABLE void setSelectedShifts(int,bool);

    Q_INVOKABLE void getCurrentStatusofValues();
    QString getFilterName(int nIndex);
    QStringList getAlarmRows();
    Q_INVOKABLE void outputRefresh();
    Q_INVOKABLE void setOutputType(int currentOuputIndex, int currentSubOutputIndex);

    Q_INVOKABLE QString getcysclesContinousReport();
    Q_INVOKABLE void setcysclesContinousReport(QString str);

    Q_INVOKABLE QString getFrequencyDailyTime();
    Q_INVOKABLE void setFrequencyDailyTime(QString str);
    void createModelforDataFilter();
    Q_INVOKABLE void storeDataFilters(QString dataFilterName,int currentIndex,bool exportdata, bool deletedata);
    void defaultDatafilterValues();
    Q_INVOKABLE void storeReportSummary(QString);

signals:
    void selectedReportTypeChanged();
    void selectedFrequencyTypeChanged();
    void selectedFrequencyPeriodTypeChanged();
    void selectedFrequencyPeriodDateChanged();
    void selectedFrequencyPeriodTimeChanged();
    void selectedOutputTimeChanged();
    void selectedCyclesChanged();
    void selectedShiftsChanged();
    void cyclesTextChanged();
    void frequencyDailyTimeChanged();

public slots:
private:
    QMap<int,QString> m_DataFilters;
    DataWizardEnums m_SelectedReportType;
    DataWizardEnums m_SelectedFrequencyType;
    DataWizardEnums m_SelectedFrequencyPeriodType;
    QMap<int,QString> m_DataWizardStrings;
    QStringList m_FrequencyPeriods;
    QStringList m_FrequencyPeriods_Time_Weekly;
    QStringList m_FrequencyPeriods_Time_Monthly;
    QStringList m_StorageList;
    QStringList m_OutputList;
    int m_OutputType;
    QDateTime m_FrequencyPeriodDate;
    QString m_FrequencyPeriodsTime;

    int m_Cycles;
    int index;
    QList<QObject*> m_DataFiltersList;
    QList<QObject*> m_DataFiltersListSingleReport;
    QList<QObject*> m_OutputsList;
    QStringList stroutputs;
    QString cysclesContinousReport;
    QString m_NoOfcycles;

    QString m_FrequencyDailyTime;
    QString m_OutputTime;
    QList<QObject*>m_weldDataList;
    QList<QObject*> m_alarmDataList;
    QList<QObject*> m_SystemDataList;
    QList<QObject*> m_eventDataList;

    WeldDataStruct weldDataFilter;
    AlarmDataStruct alarmDataFilter;
    SystemSettingStruct systemFilter;
    EventDataStruct eventFilter;

};


#endif // DATAWIZARD_H

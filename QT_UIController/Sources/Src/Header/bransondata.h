#ifndef BRANSONDATA_H
#define BRANSONDATA_H

#include <QObject>
#include <QQmlContext>

#include <Header/communicationinterface.h>
#include "Header/welduitobl.h"

extern QQmlContext* context;
class BransonData : public QObject
{
    Q_OBJECT

public:
    explicit BransonData(QObject *parent = 0);
    ~BransonData();

    FeatureFlag m_runTimeFeatureStr;
    FeatureFlag tempRTFStrObj;
    FeatureFlag tempRTFStrObjDummy;
    FeatureFlag cancelRTF;
    FeatureFlag cancelRTFDummy;

    /**List for Branson Features**/
    QList<QObject*> m_ScreensList;
    QList<QObject*> m_WeldModeList;
    QList<QObject*> m_CycleModifyList;
    QList<QObject*> m_LoggingList;
    QList<QObject*> m_StorageList;
    QList<QObject*> m_DataExtractList;
    QList<QObject*> m_ReportingList;
    QList<QObject*> m_ParamList;
    QList<QObject*> m_ParamListDummy;

    Q_INVOKABLE void updateScreensCheckboxStatus(int,bool);
    Q_INVOKABLE void updateWeldModeCheckboxStatus(int index, bool status);
    Q_INVOKABLE void updateCycleModifyCheckboxStatus(int index, bool status);
    Q_INVOKABLE void updateLoggingCheckboxStatus(int index, bool status);
    Q_INVOKABLE void updateExtraParamCheckboxStatus(int index, bool status);
    Q_INVOKABLE void updateReportingCheckboxStatus(int index, bool status);
    Q_INVOKABLE void updateExtractionCheckboxStatus(int index, bool status);
    Q_INVOKABLE void updateStorageCheckboxStatus(int index, bool status);
    Q_INVOKABLE void cancelCurrentChanges();
    Q_INVOKABLE void resetDefaultsValues();
    Q_INVOKABLE bool saveRunTimeFeatures();
    Q_INVOKABLE void createBransonFeaturesModel();
    Q_INVOKABLE void readRunTimeFeatureData();

    Q_INVOKABLE void saveWebServiceOptions(bool a_webServiceStaus,bool a_foeStatus);
    Q_INVOKABLE void getWebServiceOptions();
    Q_INVOKABLE bool getWebServiceStatus();
    Q_INVOKABLE bool getFOEStatus();
    void createScreenModel();
    void createWeldModeModel();
    void createLoggingModel();
    void createDataStorageModel();
    void createCycleModifyModel();
    void createDataExtractionModel();
    void createReportingModel();
    void createExtraParamModel();
    bool getCheckBoxStatus(QString paramName);

    bool savedStatus;
    bool bIsWebServices;
    bool bIsFOE;
};

#endif // BRANSONDATA_H

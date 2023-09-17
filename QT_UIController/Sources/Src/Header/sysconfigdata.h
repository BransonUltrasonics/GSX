/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

System Configuration

***************************************************************************/
#ifndef SYSCONFIGDATA_H
#define SYSCONFIGDATA_H
#include <QObject>
#include <QRegularExpression>
#include <QQmlContext>
#include <qdatetime.h>

#include <Header/clientsocket.h>
#include <Header/communicationinterface.h>
#include <Header/SysConfig.h>
#include <Header/newpassword.h>

/* Macros for String Constants */

//Ps Power On Options
#define S_SCAN QObject::tr("SCAN")
#define S_SEEK QObject::tr("SEEK")
#define S_NONE QObject::tr("NONE")

//Units
#define S_METRIC QObject::tr("Metric")
#define S_IMPERIAL QObject::tr("Imperial")

//Actuators Types
#define S_SERVO QObject::tr("Servo")
#define S_PNEUMATIC QObject::tr("Pneumatic")

//Control Levels
#define S_NEPTUNE QObject::tr("Neptune")
#define S_MERCURY QObject::tr("Mercury")

//Start Screen Options
#define S_DASHBOARD QObject::tr("Dashboard")
#define S_PRODUCTION QObject::tr("Production")
#define S_RECIPE     QObject::tr("Recipes")
#define S_ANALYTICS QObject::tr("Analytics")

//Ps Types
#define S_STANDARD QObject::tr("STANDARD")

//System Types
#define S_GSX_E1 "GSX-E1"
/*System Type ComboBox model names*/
#define S_ASX "ASX"

//Memory Full Options
#define S_STOP QObject::tr("STOP")
#define S_CONTINUE QObject::tr("CONTINUE")

//Authority Check Options
#define S_DISABLED QObject::tr("DISABLED")
#define S_MINIMAL QObject::tr("MINIMAL")
#define S_FULL QObject::tr("FULL")

class DateAndTime: public QThread
{
    Q_OBJECT
public:
    DateAndTime();
    ~DateAndTime();

    void startTimer();
    void stopTimer();
    void run();
};

struct welddataextraction
{
    UINT8 reportId;
    UINT8 outputFormat;
    char storageType[5];
    UINT8 deleteID;
};

enum{
    SC_INVALID_STATE=0,
    USB_STORAGE_NOT_PRESENT = 16,
    USB_STORAGE_FULL,
    USB_INCOMPLETE_WRITE
};


//! Class for System Configuration Data class.
/*!
  This class derived from QObject, handles communication with QML.
*/
class SysConfigData : public QObject
{
    Q_OBJECT

    QList<QObject*> m_listSysConfigGen;
    QList<QObject*> m_listdummy;

    QList<QObject*> m_BransonSystemMenuList;
    QList<QObject*> m_dummyBransonSystemMenuList;

    QList<QObject*> m_serviceRightParamList;
    QList<QObject*> m_serviceRightParamListdummy;
    QList<QObject*> m_modelList;

    QList<QObject*> m_SCBransonSystemMenuList;
    QList<QObject*> m_PCBransonSystemMenuList;
    QList<QObject*> m_ACBransonSystemMenuList;

    Q_PROPERTY(QDateTime TitleBarDate READ getDateAndTime WRITE setDateAndTime NOTIFY DateAndTimeChanged)
    Q_PROPERTY(int reportRes READ getReportRes WRITE setReportRes NOTIFY responseResChaned)
    Q_PROPERTY(bool bransonKeyGlobalFlag READ getBransonKeyGlobalFlag WRITE setBransonKeyGlobalFlag NOTIFY bransonKeyGlobalFlagChanged)
    Q_PROPERTY(bool bransonSecondLevelFlag READ getBransonSecondLevelKeyFlag WRITE setBransonSecondLevelKeyFlag NOTIFY bransonSecondLevelFlagChanged)
    Q_PROPERTY(bool globalCheckFlag READ getGlobalCheckFlag WRITE setGlobalCheckFlag NOTIFY globalCheckFlagChanged)
    Q_PROPERTY(QString emmcUsage READ getEMMCUsage WRITE updateEMMCUsage NOTIFY emmcUsageChanged)
    Q_PROPERTY(QString vacuumRes READ getVacuumRes WRITE setVacuumRes NOTIFY vacuumResChaned)

public:
    BransonKey bransonKeyStrObj;
    BransonKey bransonKeyStrObjDummy;
    ServiceParams serviceParameterObj;
    ServiceParams serviceParameterObjDummy;

    BransonKey tempBransonKeyStrObj;
    BransonKey tempBransonKeyStrObjDummy;

    Q_INVOKABLE void generateReport(int index, int subIndex, int outputformat,int deleteID);
    Q_INVOKABLE void sendReportAction(QString status);

    Q_INVOKABLE void startVacuum();

    /**< system configuration getter functions ,to show system configuration parameters to UI*/
    Q_INVOKABLE QString getSystemConfigGeneralParam();
    Q_INVOKABLE void createSystemConfigGeneralParamList();
    QString getBransonSystemConfigList();
    Q_INVOKABLE void createBransonSystemConfigList();
    Q_INVOKABLE bool getStatusOfSwitch(QString);
    Q_INVOKABLE QString getNameOfTextEdit(QString);
    Q_INVOKABLE int getCurrentNameOfComboBox(QString);
    Q_INVOKABLE void initializeComboxModelsName();
    Q_INVOKABLE void updateToggleSwitchState(QString paramText,bool toogleState);
    Q_INVOKABLE void updateComboBoxState(QString paramText,int currentState);
    Q_INVOKABLE void updateLineEditState(QString paramText,QString currentText);
    Q_INVOKABLE int compareBransonKey(QString keyValue);
    Q_INVOKABLE int compareBransonSecondLevelKey(QString keyValue);
    Q_INVOKABLE QString ReadEMMCWareUsage();
    Q_INVOKABLE QString getEMMCUsage();

    Q_INVOKABLE int getLanguage();
    Q_INVOKABLE QString getLanguageCode();
    Q_INVOKABLE QString getMachineName();
    Q_INVOKABLE int getAuthorityCheck();
    Q_INVOKABLE void setAuthorityCheck();
    Q_INVOKABLE void incrementTimeBySec();
    void setDefaultValues();

    /**
     * @brief getInstance - use to get SysConfigData class object
     * @return pointer to single object of class
     */
    static SysConfigData *getInstance();

    QQmlContext* context;

    Q_INVOKABLE bool deleteDB();
    Q_INVOKABLE void copyDB();

    Q_INVOKABLE void Init();
    Q_INVOKABLE int getReportRes() const;
    Q_INVOKABLE QString getVacuumRes() const;
    void setReportRes(int reportRes);
    void setVacuumRes(QString reportRes);
    Q_INVOKABLE bool getBransonKeyGlobalFlag();
    void setBransonKeyGlobalFlag(bool flag);

    Q_INVOKABLE bool getGlobalCheckFlag() const;
    void setGlobalCheckFlag(bool GlobalCheckFlag);

    Q_INVOKABLE int getDbmemoryStatus() const;
    void setDbmemoryStatus(int dbmemoryStatus);
    Q_INVOKABLE int getQTLanguageId(int currentIndex);
    Q_INVOKABLE QString getQTLanguageName(int lRealLanguageId);
    Q_INVOKABLE QString getQTNativeLanguageName(int lRealLanguageId);

    void getBransonKeyDetails();
    bool getBransonSecondLevelKeyFlag() const;
    void setBransonSecondLevelKeyFlag(bool bransonSecondLevelKeyFlag);

    Q_INVOKABLE bool sendfactoryQuery();

    QList<QObject*> m_serviceNotes;
    Q_INVOKABLE void createServiceNotesList(QString servicenoteText);
    Q_INVOKABLE void updateServiceNotesFromFile();
    QString validatingDelimiter(QString);
    Q_INVOKABLE void createServiceRightModel();
    QStringList getServiceRightModel();
    Q_INVOKABLE void setServiceSwitchStatus(int index, bool status);
    Q_INVOKABLE bool getServiceSwitchStatus(QString paramName);
    Q_INVOKABLE void resetServiceSwitchStatus();
    Q_INVOKABLE void saveServiceData(QString);
    Q_INVOKABLE void stopTimerThread();
    Q_INVOKABLE void startTimerThread();
    Q_INVOKABLE int getCopyDBRes() const;
    void setCopyDBRes(int copyDBRes);
    QString getSCBransonSystemConfigList();
    QString getPCBransonSystemConfigList();
    void createBransonPCSystemConfigList();
    QString getACBransonSystemConfigList();
    void createBransonACSystemConfigList();
    void updateEMMCUsage(QString EMMCUsageValue);

public slots:
    /**< system configuration setter functions ,to set system configuration parameters from UI*/

    Q_INVOKABLE void setMachineName(QString a_MachineName);
    Q_INVOKABLE void setLanguage(int a_language);
    Q_INVOKABLE void setSysConfigData();
    Q_INVOKABLE void setPartContactDistance(QString a_PartContactDist);
    Q_INVOKABLE int saveSysConfigData();
    Q_INVOKABLE void sendSetupReq();
    Q_INVOKABLE QString recvSetupRes();
    Q_INVOKABLE QString setWaitingStatus();
    Q_INVOKABLE void setSetupModel();
    Q_INVOKABLE void setPresetBarcode_RecallRecipe(QString cPresetBarcodeRecallRecipe);
    Q_INVOKABLE QString getPresetBarcode_RecallRecipe();
    Q_INVOKABLE void setPartIdScanRequired(bool bPartIdScanRequired);
    Q_INVOKABLE bool getPartIdScanRequired();
    Q_INVOKABLE void getTimeAndDateFromSC();
    Q_INVOKABLE void setDateAndTime(QDateTime a_dateAndTime);
    Q_INVOKABLE QDateTime getDateAndTime();
    Q_INVOKABLE QString convertDateAndTimeToString();
    Q_INVOKABLE bool sendDateAndTimeToSC(QString dateTime);

    void generateReportResPopup(QString res);
    void vacuumReportResPopup(QString res);
    void dBMemoryStatusPopup(QString res);
    void copyDBToUSBRes(QString res);

signals:
    void LanguageChanged();
    void DateAndTimeChanged();
    void responseResChaned();
    void vacuumResChaned();
    void dbStatusChanged();
    void bransonKeyGlobalFlagChanged();
    void bransonSecondLevelFlagChanged();
    void globalCheckFlagChanged();
    void emmcUsageChanged();

private:
    QString m_PartContactDist;//Part Contact Distance
    QString m_WaitingStatus;
    int m_reportRes;
    QString m_vacuumRes;
    int m_copyDBRes;
    int m_dbmemoryStatus;
    bool m_bransonKeyGlobalFlag;
    bool m_bransonSecondLevelKeyFlag;
    QList<QObject*> m_SetUpData;
    QList<QObject*> m_dummySetupData;
    QDateTime m_TitleBarDate;
    NewPassword m_NewPassword;
    DateAndTime dateTimeTimer;
    QString m_EMMCUsage;
	
    static SysConfigData *sysConfigPtr;///<Pointer to the single object of the class
    explicit SysConfigData(QObject *parent = 0);
public:
    bool m_GlobalCheckFlag;

};

#endif // SYSCONFIGDATA_H







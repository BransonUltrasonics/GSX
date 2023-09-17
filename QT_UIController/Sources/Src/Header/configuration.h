/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     handle configuration data

***************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QSettings>
#include <QString>
#include <Header/SysConfig.h>
#include "Header/welduitobl.h"

#define FILESEPERATOR       ","
#define NO_PRIVATE_KEY      "None"
enum FeatureFlagIndexes
{
    MULTI_MODE = 1,
    MODE_TIME,
    MODE_ENERGY,
    MODE_POWER,
    MODE_COLLAPSE_DISTANCE,
    MODE_ABSOLUTE_DISTANCE,
    MODE_GROUND_DETECT,
    MODE_CONTINOUS,
    PRE_TRIGGER,
    AFTER_BURST,
    ENERGY_BRAKE,
    AMPLITUDE_STEP,
    FORCE_STEP,
    ALARM_LOG,
    EVENT_LOG,
    WELD_LOG,
    REPORTING,
    CRACKED_HORN,
    USB,
    BINARY,
    WEB_SERVICES,
    OPC_UA,
    USER_IO,
    ACTUATOR_IO,
    CYCLE_COUNTER,
    BATCH_COUNTER,
    POWER_MATCH_CURVE,
    SMART_CONVERTER,
    CONTROL_SPEED,
    SEQUENCING,
    AUTO_CONVERTER_COOLING,
    FIELD_BUS,
    DASHBOARD_SCREEN,
    PASSWORD_SCREEN,
    MOBILE,
    OVERLAY,
    TRENDS,
    OPTITUNE,
    LIMITS,
    HD_MODE,
    TEACH_MODE,
    ADV_COMM,
    EXTRA_DATA_STORAGE,
    TOUBLE_SHOOT,
    CFR,
    HOUR_COUNTER,
    CSV,
    PDF,
    IODIAG,
    MODE_DYNAMIC,
    RESERVED_B50,
    RESERVED_B51
};

enum HMILOGIN
{
    NO_HMI = 0,
    HMI_LOGIN
};

/**
 * @brief The Configuration class: Configuration details are stored in this singleton  class
 */
class Configuration : public QObject
{
    Q_OBJECT
public:
signals:
    void bransonKeyValueChanged();

public:
    Q_INVOKABLE void init();
    Q_INVOKABLE QString CheckDBVersion();
    Q_INVOKABLE QString getUICVersion();
    Q_INVOKABLE QString getSCFirmwareVersion();
    Q_INVOKABLE QString getACFirmwareVersion();
    Q_INVOKABLE QString getPCFirmwareVersion();
    Q_INVOKABLE int getFeatureFlagStatus(int dwIndex);
    Q_INVOKABLE void setActivePage(int dwPageIndex);
    Q_INVOKABLE int getActivePage();
    Q_INVOKABLE bool readRuntimeFlagInformation();
    //Q_INVOKABLE void shutDownWindows();
    Q_INVOKABLE void allowAltTab();
    Q_INVOKABLE void blockAltTab();

    Q_INVOKABLE void setConigValues(const QString& group, const QString& key, const QString& value);
    QString getConfigValues(const QString& group, const QString& key);
    QString getIP();
    QString getPort();
    Q_INVOKABLE int getStatusDisplayTime();
private:
    Configuration();///<constructor of Configuration class
    ~Configuration();

    void readFirmwareVersion();

    QString m_IPaddr;///<IP address
    QString m_Port;///< Port Number
    QString m_UserID;///< User ID
    QString m_SCFV;
    QString m_ACFV;
    QString m_PCFV;
    QString m_UICV;
    QString m_DBV;
    bool    m_SkipLogin;///<Skip Login
    int     m_dwActivePage;
    int     m_ScreenActiveValue;
    int     m_nStatusDisplayTime; //Timer for displaying text in global status bar

    static Configuration *configPtr;///<Pointer to the single object of the class

    QSettings m_settings;

    QStringList qstrListPrivateKeyFilenames;
public:
    /**
     * @brief getInstance - use to get Configuration class object
     * @return pointer to single object of class
     */
    static Configuration *getInstance();
    /**
     * @brief readSysConfigData - get system configuration data from socket
     * @return void
     */
    Q_INVOKABLE bool readSysConfigData();
    intf_SysConfig_t m_systemData;///<object of system configuration which is set from socket
    intf_SysConfig_t m_systemDataDummy;
    FeatureFlag m_runTimeData;///<object of system configuration which is set from socket
    FeatureFlag m_runTimeDataDummy;
    Q_INVOKABLE QString getEventTableOrder();
    Q_INVOKABLE void setEventTableOrder(QString qstrOrder);
    Q_INVOKABLE QString getAnalyticsTableOrder();
    Q_INVOKABLE void setAnalyticsTableOrder(QString qstrOrder);
    Q_INVOKABLE void setScreenActiveValue(unsigned int a_ScreenActiveValue);
    Q_INVOKABLE unsigned int getScreenActiveValue();
    Q_INVOKABLE int getActiveScreen(int a_screenIndex);
    Q_INVOKABLE void changingUnits();
    Q_INVOKABLE int checkWebServices();
    Q_INVOKABLE int readPrivateKeyFiles();
    Q_INVOKABLE QStringList getPrivateKeyFilesList();
    Q_INVOKABLE void clearPrivateKeyFilesList();
    Q_INVOKABLE int uploadAuthenticationKey(int index);
    Q_INVOKABLE void setHMILogonStatus(bool status);
    void setSystemLanguage();

    double convertSecToMsec(double);
    double convertMsecToSec(double);
    double convertMicrometreToMillimetre(double);
    double convertMillimetreToMicrometre(double);
    double convertinchestomicrometre(double);
    double convertmicrometretoinches(double);

    double convertInchesToMillimetre(double);
    double convertMillimeterToInches(double);
    QString getDatabaseVersion();

};
#endif // CONFIGURATION_H

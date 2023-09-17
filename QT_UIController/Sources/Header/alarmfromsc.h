/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Alarm Notification from SC

***************************************************************************/

#ifndef ALARMFROMSC_H
#define ALARMFROMSC_H

#include <QString>
#include <QObject>

#define ALARM_PHASE_OVERLOAD_ID					1  // base number for weld overloads
#define ALARM_CURR_OVERLOAD_ID	 				2
#define ALARM_FREQ_OVERLOAD_ID					3
#define ALARM_POWER_OVERLOAD_ID					4
#define ALARM_VOLT_OVERLOAD_ID					5
#define ALARM_TEMP_OVERLOAD_ID					6
#define ALARM_EB_PHASE_OVERLOAD_ID				17  // base number for Energy Brake overloads
#define ALARM_EB_CURR_OVERLOAD_ID	 			18
#define ALARM_EB_FREQ_OVERLOAD_ID				19
#define ALARM_EB_POWER_OVERLOAD_ID				20
#define ALARM_EB_VOLT_OVERLOAD_ID				21
#define ALARM_EB_TEMP_OVERLOAD_ID				22
#define ALARM_AB_PHASE_OVERLOAD_ID				33  // base number for Afterburst overloads
#define ALARM_AB_CURR_OVERLOAD_ID	 			34
#define ALARM_AB_FREQ_OVERLOAD_ID				35
#define ALARM_AB_POWER_OVERLOAD_ID				36
#define ALARM_AB_VOLT_OVERLOAD_ID				37
#define ALARM_AB_TEMP_OVERLOAD_ID				38
#define ALARM_POST_SEEK_PHASE_OVERLOAD_ID		49  // base number for Post Weld Seek overloads
#define ALARM_POST_SEEK_CURR_OVERLOAD_ID		50
#define ALARM_POST_SEEK_FREQ_OVERLOAD_ID		51
#define ALARM_POST_SEEK_POWER_OVERLOAD_ID		52
#define ALARM_POST_SEEK_VOLT_OVERLOAD_ID		53
#define ALARM_POST_SEEK_TEMP_OVERLOAD_ID		54

/* No Cycle Alarm names */
#define ALARM_NOCYCLE_EXTERNAL_SONIC_ID         1795
#define ALARM_NOCYCLE_MISSING_PART_ID           1798
#define ALARM_NOCYCLE_PART_CONTACT_ID           1800
#define ALARM_NOCYCLE_EXTERNAL_CYCLE_ID         1802
#define ALARM_NOCYCLE_ABSOLUTE_DISTANCE_ID      1804
#define ALARM_NOCYCLE_EXTRA_TOOLING_ID          1812
#define ALARM_NOCYCLE_EXTRA_TIMEOUT_ID          1813
#define ALARM_NOCYCLE_PART_PRESENT_ID           1814
#define ALARM_NOCYCLE_SERVO_MOTOR_ID            1815
#define ALARM_NOCYCLE_READY_POS_ID              1816
#define ALARM_NOCYCLE_RECIPE_NOTVALID_ID        1817
#define ALARM_NOCYCLE_BATCH_COUNT_ID            1818
#define ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_ID  1819
#define ALARM_NOCYCLE_PC_ASS_MISMATCH_ID        1824
#define ALARM_NOCYCLE_AC_ASS_MISMATCH_ID        1825
#define ALARM_NOCYCLE_STACK_ASS_MISMATCH_ID     1826
#define ALARM_NOCYCLE_TRIGGER_TIMEOUT_ID        1827

/* Hardware failure alarm names */
#define ALARM_HW_START_SWITCH_FAIL_ID           1537
#define ALARM_HW_ULS_ACTIVE_ID                  1538
#define ALARM_HW_ULS_NONACTIVE_ID               1540
#define ALARM_HW_GD_BEFORE_TRIGGER_ID           1541
#define ALARM_HW_START_SWITCH_LOST_ID           1545
#define ALARM_HW_ALARM_LOG_ID                   1553
#define ALARM_HW_EVENT_LOG_ID                   1554
#define ALARM_HW_WELD_RESULT_ID                 1555
#define ALARM_HW_WELD_GRAPH_ID                  1556
#define ALARM_HW_HORN_SCAN_ID                   1557
#define ALARM_HW_PRETRIGGER_TIMEOUT_ID          1568
#define ALARM_HW_ENCODER_FAIL_ID                1569
#define ALARM_HW_DATA_ERROR_ID                  1572
#define ALARM_HW_HOME_RETURN_TIMEOUT_ID         1573
#define ALARM_HW_ACTUATOR_NOVARM_ID             1574
#define ALARM_HW_PS_NOVARM_ID                   1575
#define ALARM_HW_START_SWITCH_TIME_ID           1576
#define ALARM_HW_MEM_FULL_ID                    1577
#define ALARM_HW_INTERNAL_STORAGE_FAIL_ID		1578
#define ALARM_HW_RECALIBRATE_ACT_ID             1583
#define ALARM_HW_ACT_CLR_FUN_ID                 1584
#define ALARM_HW_EXTRA_TOOLING_ACTIVE_ID        1585
#define ALARM_HW_ACT_TYPE_CHANGED_ID            1586
#define ALARM_HW_SYS_PRESSURE_INCORRECT_ID      1587
#define ALARM_HW_SYS_PART_PRESENT_ACTIVE_ID     1588
#define ALARM_HW_USB_MEM_LOST_ID                1589
#define ALARM_INTERNAL_COMM_ENET                1592
#define ALARM_HW_INTERNAL_BUS_LOST_ID           1593
#define ALARM_HW_CABLE_FAIL_ID                  1594
#define ALARM_HW_PROFINET_OR_IP_NOT_ID          1595
#define ALARM_HW_AC_LINE_LOST_ID                1596
#define ALARM_HW_TRIGGER_ACTIVE_READY_ID        1597
#define ALARM_HW_INTERNAL_COMM_FAIL_ID          1598
#define ALARM_HW_SC_COMPONENT_FAIL_ID           1599
#define ALARM_HW_RTC_LOW_BATT_ID       			1092

/* Cycle modified alarm names */
#define ALARM_CYCLE_GROUND_DETECT_ABORT_ID      771
#define ALARM_CYCLE_MAXTIMEOUT_ID               772
#define ALARM_CYCLE_NOFORCESTEP_ID              774
#define ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_ID 789	//not E1
#define ALARM_CYCLE_LL_NOT_REACHED_ID           791
#define ALARM_CYCLE_PEAKPOWER_CUTOFF_ID         1051
#define ALARM_CYCLE_ABSOLUTE_CUTOFF_ID          1052
#define ALARM_CYCLE_COLLAPSE_CUTOFF_ID          1055
#define ALARM_CYCLE_ULTRASONICS_DISABLED_ID     1057
#define ALARM_CYCLE_CUSTOM1_CUTOFF_ID           1060
#define ALARM_CYCLE_CUSTOM2_CUTOFF_ID           1061
#define ALARM_CYCLE_FREQLOW_CUTOFF_ID           1062
#define ALARM_CYCLE_FREQHIGH_CUTOFF_ID          1063
#define ALARM_CYCLE_CUST_DIGITAL_CUTOFF_ID      1064
#define ALARM_CYCLE_ENERGY_CUTOFF_ID            1065
#define ALARM_CYCLE_GD_CUTOFF_ID                1066
#define ALARM_CYCLE_TIME_CUTOFF_ID              1067
#define ALARM_CYCLE_NO_PRESSURE_STEP_ID         1081

/* Suspect alarms */
#define ALARM_ENERGY_SLLIMIT_ID                 1361
#define ALARM_ENERGY_SHLIMIT_ID                 1362
#define ALARM_POWER_SLLIMIT_ID                  1363
#define ALARM_POWER_SHLIMIT_ID                  1364
#define ALARM_COLLAPSE_SLLIMIT_ID               1365
#define ALARM_COLLAPSE_SHLIMIT_ID               1366
#define ALARM_ABSOLUTE_SLLIMIT_ID               1367
#define ALARM_ABSOLUTE_SHLIMIT_ID               1368
#define ALARM_TRIGGER_SLLIMIT_ID                1369
#define ALARM_TRIGGER_SHLIMIT_ID                1370
#define ALARM_WELD_FORCE_SLLIMIT_ID             1371
#define ALARM_WELD_FORCE_SHLIMIT_ID             1372
#define ALARM_TIME_SLLIMIT_ID                   1373
#define ALARM_TIME_SHLIMIT_ID                   1374
#define ALARM_VELOCITY_SLLIMITS_ID              1376
#define ALARM_VELOCITY_SHLIMITS_ID              1377
#define ALARM_FREQ_SLLIMITS_ID                  1378
#define ALARM_FREQ_SHLIMITS_ID                  1379

/* Reject alarms */
#define ALARM_VELOCITY_RLLIMITS_ID              1281
#define ALARM_VELOCITY_RHLIMITS_ID              1282
#define ALARM_ENERGY_RLLIMIT_ID                 1287
#define ALARM_ENERGY_RHLIMIT_ID                 1288
#define ALARM_POWER_RLLIMIT_ID                  1283
#define ALARM_POWER_RHLIMIT_ID                  1284
#define ALARM_COLLAPSE_RLLIMIT_ID               1289
#define ALARM_COLLAPSE_RHLIMIT_ID               1290
#define ALARM_ABSOLUTE_RLLIMIT_ID               1291
#define ALARM_ABSOLUTE_RHLIMIT_ID               1292
#define ALARM_TRIGGER_RLLIMIT_ID                1293
#define ALARM_TRIGGER_RHLIMIT_ID                1294
#define ALARM_WELD_FORCE_RLLIMIT_ID             1295
#define ALARM_WELD_FORCE_RHLIMIT_ID             1296
#define ALARM_TIME_RLLIMIT_ID                   1285
#define ALARM_TIME_RHLIMIT_ID                   1286
#define ALARM_FREQ_RLLIMITS_ID                  1298
#define ALARM_FREQ_RHLIMITS_ID                  1299
#define ALARM_POWER_MATCH_CURVE_RLLIMITS_ID     1300
#define ALARM_POWER_MATCH_CURVE_RHLIMITS_ID     1301

/* Warnings */
#define ALARM_WARNING_TRIGGERLOST_ID            1025
#define ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD_ID  1047
#define ALARM_WARNING_ACTU_RECAL_ID             1054
#define ALARM_WARNING_USBMEMORY_ID              1058
#define ALARM_WARNING_DISK_MEMORY_ID            1059
#define ALARM_WARNING_ALARM_LOG_ID              1093
#define ALARM_WARNING_EVENT_LOG_ID              1094
#define ALARM_WARNING_WELD_RESULT_ID            1095
#define ALARM_WARNING_WELD_GRAPH_ID             1096
#define ALARM_WARNING_HORN_SCAN_ID              1097
#define ALARM_WARNING_EEPROM_CORRUPT_ID			1104

#define ALARM_WARNING_RECIPE_WELD_CORRUPTED_ID		1105
#define ALARM_WARNING_RECIPE_LIMITS_CORRUPTED_ID	1106
#define ALARM_WARNING_RECIPE_SETUP_CORRUPTED_ID		1107
#define ALARM_WARNING_RECIPE_STACK_CORRUPTED_ID		1108

/*  No Cycle Overloads - Overloads that happen before trigger or outside the weld cycle */
#define ALARM_TEST_OVERLOAD_PHASE_ID                  2113	// base number for Test overloads
#define ALARM_TEST_OVERLOAD_CURRENT_ID                2114
#define ALARM_TEST_OVERLOAD_FREQUENCY_ID              2115
#define ALARM_TEST_OVERLOAD_POWER_ID                  2116
#define ALARM_TEST_OVERLOAD_VOLTAGE_ID                2117
#define ALARM_TEST_OVERLOAD_TEMPERATURE_ID            2118
#define ALARM_PRETRIGGER_OVERLOAD_PHASE_ID            2129	// base number for Pretrigger overloads
#define ALARM_PRETRIGGER_OVERLOAD_CURRENT_ID          2130
#define ALARM_PRETRIGGER_OVERLOAD_FREQUENCY_ID        2131
#define ALARM_PRETRIGGER_OVERLOAD_POWER_ID            2132
#define ALARM_PRETRIGGER_OVERLOAD_VOLTAGE_ID          2133
#define ALARM_PRETRIGGER_OVERLOAD_TEMPERATURE_ID      2134
#define ALARM_SEEK_OVERLOAD_PHASE_ID           		  2145	// base number for Seek overloads
#define ALARM_SEEK_OVERLOAD_CURRENT_ID       		  2146
#define ALARM_SEEK_OVERLOAD_FREQUENCY_ID    		  2147
#define ALARM_SEEK_OVERLOAD_POWER_ID      		      2148
#define ALARM_SEEK_OVERLOAD_VOLTAGE_ID   		      2149
#define ALARM_SEEK_OVERLOAD_TEMPERATURE_ID  	      2150
#define ALARM_PRE_SEEK_PHASE_OVERLOAD_ID			  2177  // base number for Pre Weld Seek overloads
#define ALARM_PRE_SEEK_CURR_OVERLOAD_ID			  	  2178
#define ALARM_PRE_SEEK_FREQ_OVERLOAD_ID				  2179
#define ALARM_PRE_SEEK_POWER_OVERLOAD_ID			  2180
#define ALARM_PRE_SEEK_VOLT_OVERLOAD_ID				  2181
#define ALARM_PRE_SEEK_TEMP_OVERLOAD_ID				  2182

/* EN Faults */
#define ALARM_MULTIPLE_FAULTS_ID                3824
#define ALARM_PALM_BUTTON_FAULT_ID              3825
#define ALARM_24V_FAULT_ID                      3826
#define ALARM_ESTOP_FAULT_ID                    3827
#define ALARM_LINEAR_ENCODER_FAULT_ID           3828
#define ALARM_S_BEAM_FAULT_ID                   3829
#define ALARM_TRIGGER_SWITCH_FAULT_ID           3830
#define ALARM_DRIVE_FAULT_ID                    3831
#define ALARM_CROSS_MONITORING_FAULT_ID         3832
#define ALARM_LOGIC_FAULT_ID                    3833
#define ALARM_SONICS_ENABLE_FAULT_ID            3834

class AlarmFromSC : public QObject
{
    Q_OBJECT
public:
    bool loginStatus = false;
    bool bAlarmPopupStatusFlag = false;

    explicit AlarmFromSC(QObject *parent = 0);

    Q_PROPERTY(int Reset READ getReset WRITE setReset NOTIFY OnResetChanged)
    Q_PROPERTY(QString AlarmID READ getAlarmId WRITE setAlarmID NOTIFY OnAlarmIDChanged)
    Q_PROPERTY(QString AlarmTime READ getAlarmTime WRITE setAlarmTime NOTIFY OnAlarmTimeChanged)
    Q_PROPERTY(QString AlarmDate READ getAlarmDate WRITE setAlarmDate NOTIFY OnAlarmDateChanged)
    Q_PROPERTY(QString ErrorDesc READ getErrorDesc WRITE setErrorDesc NOTIFY OnErrorDescChanged)
    Q_PROPERTY(QString ErrorName READ getErrorName WRITE setErrorName NOTIFY OnErrorNameChanged)
signals:
public:
    Q_INVOKABLE void readAlarmDetails(QString a_data);
    Q_INVOKABLE void emitKey(Qt::Key key);
    Q_INVOKABLE void dbFullRemindReq();
    Q_INVOKABLE void uicStatus(bool a_status);
    Q_INVOKABLE void alarmPopupStatus(bool a_status);
    Q_INVOKABLE int sendAlarmToSCToReset(QString);
    Q_INVOKABLE int sendResetAllRequestToSC();
    Q_INVOKABLE int getReset();
    Q_INVOKABLE int sendResetEmergencyReq();
    Q_INVOKABLE QString getAlarmId();
    Q_INVOKABLE QString getAlarmTime();
    Q_INVOKABLE QString getAlarmDate();
    Q_INVOKABLE QString getErrorDesc();
    Q_INVOKABLE QString getErrorName();
    Q_INVOKABLE QString errorNameforAlarm(QString a_Id);
    Q_INVOKABLE QString errorDescforAlarm(QString a_Id);

public slots:
    void setAlarmID(QString alarmID);
    void setAlarmTime(QString alarmTime);
    void setAlarmDate(QString alarmDate);
    void setReset(int a_reset);
    void setErrorDesc(QString errorDesc);
    void setErrorName(QString errorName);
    void emergencyStart();
    void emergencyStop();
    Q_INVOKABLE void loadAlarmPopup(QString a_data);

signals:
    void OnAlarmIDChanged();
    void OnAlarmTimeChanged();
    void OnAlarmDateChanged();
    void OnResetChanged();
    void OnErrorDescChanged();
    void OnErrorNameChanged();

private:
    QString m_AlarmId;
    QString m_AlarmDate;
    QString m_AlarmTime;
    QString m_errorDesc;
    QString m_errorName;
    int m_resetStatus;
    bool m_isAlarmRaised = false;
};

#endif // ALARMFROMSC_H

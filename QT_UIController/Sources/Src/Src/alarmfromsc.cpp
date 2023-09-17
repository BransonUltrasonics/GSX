/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    It is used to process alarm recived to from BL and sends reset req to BL

***************************************************************************/
#include <QString>
#include <QObject>
#include <QQmlContext>
#include <QDate>
#include <QTime>
#include "Header/alarmfromsc.h"
#include "Header/alarmnotification.h"
#include "Header/communicationinterface.h"
#include "Header/sysconfigdata.h"

extern CommunicationInterface CIObj;
extern QQmlContext* context;
extern AlarmNotification *alarmNotify;
extern QObject * object;

QStringList alarmDataList;
QString date;
QString alarmTime;
#define ANALY_ALARM_PART_ERROR  QObject::tr("Part Error :")
#define ANALY_ALARM_MACHINE_ERROR  QObject::tr("Machine Error :")
#define ANALY_ALARM_NEW  QObject::tr("Unresolved")
#define ANALY_ALARM_RESET  QObject::tr("Resolved")

/* No Cycle Alarm names */
#define ALARM_NOCYCLE_EXTERNAL_SONIC                QObject::tr("External Sonics Delay Timeout")
#define ALARM_NOCYCLE_MISSING_PART                  QObject::tr("Part Window Abort")
#define ALARM_NOCYCLE_PART_CONTACT                  QObject::tr("Part Contact before Pretrigger")
#define ALARM_NOCYCLE_EXTERNAL_CYCLE                QObject::tr("External Cycle Abort")
#define ALARM_NOCYCLE_ABSOLUTE_DISTANCE             QObject::tr("Invalid Part Contact Distance")
#define ALARM_NOCYCLE_EXTRA_TOOLING                 QObject::tr("External Tooling Input Lost")
#define ALARM_NOCYCLE_EXTRA_TIMEOUT                 QObject::tr("External Tooling Input Timeout")
#define ALARM_NOCYCLE_PART_PRESENT                  QObject::tr("Part Present Input Lost")
#define ALARM_NOCYCLE_SERVO_MOTOR                   QObject::tr("Actuation Drive Error")
#define ALARM_NOCYCLE_READY_POS                     QObject::tr("Ready Position Timeout")
#define ALARM_NOCYCLE_RECIPE_NOTVALID               QObject::tr("Recipe Not Valid")
#define ALARM_NOCYCLE_BATCH_COUNT                   QObject::tr("Batch Count Complete")
#define ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID         QObject::tr("Active Recipe not Validated")
#define ALARM_NOCYCLE_PC_ASS_MISMATCH               QObject::tr("Power Supply Assembly Component Mismatch")
#define ALARM_NOCYCLE_AC_ASS_MISMATCH               QObject::tr("Actuator Assembly Component Mismatch")
#define ALARM_NOCYCLE_STACK_ASS_MISMATCH            QObject::tr("Stack Assembly Component Mismatch")
#define ALARM_NOCYCLE_TRIGGER_TIMEOUT               QObject::tr("Trigger Timeout")

/* No Cycle Alarm Descriptions */
#define ALARM_NOCYCLE_EXTERNAL_SONIC_DESC           QObject::tr("Trigger Delay has been turned on, but the assigned input did not become inactive within the 30 seconds allowed.")
#define ALARM_NOCYCLE_MISSING_PART_DESC             QObject::tr("The Missing Part Minimum Distance has not been reached before Trigger occurred or the Maximum Distance has been exceeded before Trigger has occurred.")
#define ALARM_NOCYCLE_PART_CONTACT_DESC             QObject::tr("The Part Contact Distance has been met before the defined Pretrigger Distance.")
#define ALARM_NOCYCLE_EXTERNAL_CYCLE_DESC           QObject::tr("The Cycle Abort Digital Input has been activated before trigger occurred.")
#define ALARM_NOCYCLE_ABSOLUTE_DISTANCE_DESC        QObject::tr("The Part Contact Distance is invalid or not set.")
#define ALARM_NOCYCLE_EXTRA_TOOLING_DESC            QObject::tr("The External Tooling Input became inactive before Hold Time ended.")
#define ALARM_NOCYCLE_EXTRA_TIMEOUT_DESC            QObject::tr("The External Tooling Input did not become active within the Tooling Delay Input time after the External Tooling Output became active.")
#define ALARM_NOCYCLE_PART_PRESENT_DESC             QObject::tr("The Part Present Input became inactive before the end of Hold Time.")
#define ALARM_NOCYCLE_SERVO_MOTOR_DESC              QObject::tr("The Actuator did not reach the target defined position or Actuation is being prevented.")
#define ALARM_NOCYCLE_READY_POS_DESC                QObject::tr("The Actuator did not return to the Ready Position within 4 seconds from end of Hold Time.")
#define ALARM_NOCYCLE_RECIPE_NOTVALID_DESC          QObject::tr("The External Recipe # is not valid through the I/O or Barcode scanner.")
#define ALARM_NOCYCLE_BATCH_COUNT_DESC              QObject::tr("The target number of welds for this Batch has been met.\n\nNavigate to the Recipe Production Screen to reset the count.")
#define ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_DESC    QObject::tr("The Operator is attempting to run a recipe that has not been Validated.")
#define ALARM_NOCYCLE_PC_ASS_MISMATCH_DESC          QObject::tr("The Power Supply Component Name defined in the System Configuration do not match the Name stored with this Recipe.")
#define ALARM_NOCYCLE_AC_ASS_MISMATCH_DESC          QObject::tr("The Actuator Component Name defined in the System Configuration do not match the Name stored with this Recipe.")
#define ALARM_NOCYCLE_STACK_ASS_MISMATCH_DESC       QObject::tr("The Ultrasonic Stack Component Name defined in the System Configuration do not match the Name stored with this Recipe.")
#define ALARM_NOCYCLE_TRIGGER_TIMEOUT_DESC          QObject::tr("The Trigger Force was not achieved within the 10 second allowed time.")

/* Hardware failure alarm names */
#define ALARM_HW_START_SWITCH_FAIL                  QObject::tr("Start Switch Still Active")
#define ALARM_HW_ULS_ACTIVE                         QObject::tr("ULS Still Active")
#define ALARM_HW_ULS_NONACTIVE                      QObject::tr("ULS Not Active After Homing")
#define ALARM_HW_GD_BEFORE_TRIGGER                  QObject::tr("Ground Detect Before Trigger")
#define ALARM_HW_START_SWITCH_LOST                  QObject::tr("Start Switch Lost")
#define ALARM_HW_ALARM_LOG                          QObject::tr("Alarm Log Capacity Reached")
#define ALARM_HW_EVENT_LOG                          QObject::tr("Event Log Capacity Reached")
#define ALARM_HW_WELD_RESULT                        QObject::tr("Weld Result Capacity Reached")
#define ALARM_HW_WELD_GRAPH                         QObject::tr("Weld Graph Capacity Reached")
#define ALARM_HW_HORN_SCAN                          QObject::tr("Horn Scan Graph Capacity Reached")
#define ALARM_HW_PRETRIGGER_TIMEOUT                 QObject::tr("Pretrigger Timeout")
#define ALARM_HW_ENCODER_FAIL                       QObject::tr("Encoder Failure")
#define ALARM_HW_DATA_ERROR                         QObject::tr("Data Error")
#define ALARM_HW_HOME_RETURN_TIMEOUT                QObject::tr("Actuator Return Timeout")
#define ALARM_HW_ACTUATOR_NOVARM                    QObject::tr("Actuator Novram")
#define ALARM_HW_PS_NOVARM                          QObject::tr("P/S Novram")
#define ALARM_HW_START_SWITCH_TIME                  QObject::tr("Start Switch Time")
#define ALARM_HW_MEM_FULL                           QObject::tr("Data Storage Full")
#define ALARM_HW_INTERNAL_STORAGE_FAIL				QObject::tr("Internal Failure")
#define ALARM_HW_RECALIBRATE_ACT                    QObject::tr("Recalibrate Actuator")
#define ALARM_HW_ACT_CLR_FUN                        QObject::tr("Actuator Clear Function")
#define ALARM_HW_EXTRA_TOOLING_ACTIVE               QObject::tr("Ext Tooling Active")
#define ALARM_HW_ACT_TYPE_CHANGED                   QObject::tr("Actuator Type Changed")
#define ALARM_HW_SYS_PRESSURE_INCORRECT             QObject::tr("System Pressure Incorrect")
#define ALARM_HW_SYS_PART_PRESENT_ACTIVE            QObject::tr("Part Present Active")
#define ALARM_HW_USB_MEM_LOST                       QObject::tr("USB Memory Lost")
#define ALARM_CONNECTION_LOST                       QObject::tr("Connection Lost")
#define ALARM_HW_INTERNAL_BUS_LOST                  QObject::tr("Internal Bus Scan Failure")
#define ALARM_HW_CABLE_FAIL                         QObject::tr("Cable Failure")
#define ALARM_HW_PROFINET_OR_IP_NOT                 QObject::tr("Profinet or Ethernet/IP not responding")
#define ALARM_HW_AC_LINE_LOST                       QObject::tr("AC Line Voltage Lost")
#define ALARM_HW_TRIGGER_ACTIVE_READY               QObject::tr("Trigger active in Ready")
#define ALARM_HW_INTERNAL_COMM_FAIL                 QObject::tr("HMI Connection Lost")
#define ALARM_HW_SC_COMPONENT_FAIL                  QObject::tr("Internal Component Failure")

/* Hardware failure alarm descriptions */
#define ALARM_HW_START_SWITCH_FAIL_DESC             QObject::tr("The Start Switches are still active 6 seconds after the end of the Cycle.")
#define ALARM_HW_ULS_ACTIVE_DESC                    QObject::tr("The ULS has not become inactive after Trigger or Pretrigger has been reached.")
#define ALARM_HW_ULS_NONACTIVE_DESC                 QObject::tr("The ULS is not active after an Estop or Alarm.")
#define ALARM_HW_GD_BEFORE_TRIGGER_DESC             QObject::tr("The Ground Detect Input has become active before Trigger occurred.")
#define ALARM_HW_START_SWITCH_LOST_DESC             QObject::tr("The Start Switches became inactive before Trigger occurred.")
#define ALARM_HW_ALARM_LOG_DESC                     QObject::tr("Alarm Log Storage is Full.")
#define ALARM_HW_EVENT_LOG_DESC                     QObject::tr("Event Log Storage is Full.")
#define ALARM_HW_WELD_RESULT_DESC                   QObject::tr("Weld Result Storage is Full.")
#define ALARM_HW_WELD_GRAPH_DESC                    QObject::tr("Weld Graph Storage is Full.")
#define ALARM_HW_HORN_SCAN_DESC                     QObject::tr("Horn Scan Graph Storage is Full.")
#define ALARM_HW_PRETRIGGER_TIMEOUT_DESC            QObject::tr("Pre-trigger has not occurred within 10 seconds of ULS going inactive.")
#define ALARM_HW_ENCODER_FAIL_DESC                  QObject::tr("No distance after part contact is made.")
#define ALARM_HW_DATA_ERROR_DESC                    QObject::tr("Corrupted data in the Recipe checked at power up.")
#define ALARM_HW_HOME_RETURN_TIMEOUT_DESC           QObject::tr("The carriage has not returned home in 4 seconds.")
#define ALARM_HW_ACTUATOR_NOVARM_DESC               QObject::tr("The actuator Novram has corrupted data. This is checked at power up.")
#define ALARM_HW_PS_NOVARM_DESC                     QObject::tr("The power supply Novram has corrupted data. This is checked at power up.")
#define ALARM_HW_START_SWITCH_TIME_DESC             QObject::tr("Both Start Switches were not pressed within the allowed time frame.")
#define ALARM_HW_MEM_FULL_DESC                      QObject::tr("Internal Storage Device is full. Any operation which requires Data Storage will not be allowed.")
#define ALARM_HW_INTERNAL_STORAGE_FAIL_DESC			QObject::tr("Contact Branson Service for Assistance.")
#define ALARM_HW_RECALIBRATE_ACT_DESC               QObject::tr("The force calibration values loaded into the system are invalid.")
#define ALARM_HW_ACT_CLR_FUN_DESC                   QObject::tr("ULS is active before actuator clear condition was met.")
#define ALARM_HW_EXTRA_TOOLING_ACTIVE_DESC          QObject::tr("The External Tooling Input has not become inactive for more than 4 seconds after the end of a cycle.")
#define ALARM_HW_ACT_TYPE_CHANGED_DESC              QObject::tr("The actuator type detected at power up is different from power down or after an ESTOP.")
#define ALARM_HW_SYS_PRESSURE_INCORRECT_DESC        QObject::tr("The set air pressure is not achieved.")
#define ALARM_HW_SYS_PART_PRESENT_ACTIVE_DESC       QObject::tr("The Part Present Input is configured and is still active for more than 4 seconds after the end of a cycle.")
#define ALARM_HW_USB_MEM_LOST_DESC                  QObject::tr("The USB memory stick has been removed or it not functional. Since weld data was configured to be saved on the USB stick welding must be stopped until either the USB stick is functional or weld data is no longer required to be saved.")
#define ALARM_INTERNAL_COMM_FAIL                    QObject::tr("Communication between the HMI and the welder has been disconnected.")
#define ALARM_HW_INTERNAL_BUS_LOST_DESC             QObject::tr("Internal Bus scan timeout occurred. Please check all power connections or contact Branson Service for assistance.")
#define ALARM_HW_CABLE_FAIL_DESC                    QObject::tr("If Cable Detect is configured and the pin goes inactive.")
#define ALARM_HW_PROFINET_OR_IP_NOT_DESC            QObject::tr("No Description available.")
#define ALARM_HW_AC_LINE_LOST_DESC                  QObject::tr("230V input to the Power Supply is not ON.")
#define ALARM_HW_TRIGGER_ACTIVE_READY_DESC          QObject::tr("Trigger force is detected in the Ready State.")
#define ALARM_HW_INTERNAL_COMM_FAIL_DESC            QObject::tr("Internal Communication failure. Contact Branson Service.")
#define ALARM_HW_SC_COMPONENT_FAIL_DESC             QObject::tr("There is an internal failure. Contact Branson Service")
#define ALARM_HW_RTC_LOW_BATT_DESC					QObject::tr("System time may not be reliable. Set system time.")

/* Cycle modified alarm names */
#define ALARM_CYCLE_GROUND_DETECT_ABORT             QObject::tr("Ground Detect Abort")
#define ALARM_CYCLE_MAXTIMEOUT                      QObject::tr("Max Weld Time Exceeded")
#define ALARM_CYCLE_NOFORCESTEP                     QObject::tr("No Force Step")
#define ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE        QObject::tr("Trigger Force greater than End Force")
#define ALARM_CYCLE_LL_NOT_REACHED                  QObject::tr("LL Not reached")
#define ALARM_CYCLE_PEAKPOWER_CUTOFF                QObject::tr("Peak Power Cutoff")
#define ALARM_CYCLE_ABSOLUTE_CUTOFF                 QObject::tr("Absolute Distance Cutoff")
#define ALARM_CYCLE_COLLAPSE_CUTOFF                 QObject::tr("Collapse Distance Cutoff")
#define ALARM_CYCLE_ULTRASONICS_DISABLED            QObject::tr("Ultrasonics disabled")
#define ALARM_CYCLE_CUSTOM1_CUTOFF                  QObject::tr("Custom Input 1 Cutoff")
#define ALARM_CYCLE_CUSTOM2_CUTOFF                  QObject::tr("Custom Input 2 Cutoff")
#define ALARM_CYCLE_FREQLOW_CUTOFF                  QObject::tr("Frequency Low Cutoff")
#define ALARM_CYCLE_FREQHIGH_CUTOFF                 QObject::tr("Frequency High Cutoff")
#define ALARM_CYCLE_CUST_DIGITAL_CUTOFF             QObject::tr("Custom Digital Cutoff")
#define ALARM_CYCLE_ENERGY_CUTOFF                   QObject::tr("Energy Cutoff")
#define ALARM_CYCLE_GD_CUTOFF                       QObject::tr("Ground Detect Cutoff")
#define ALARM_CYCLE_TIME_CUTOFF                     QObject::tr("Time Cutoff")
#define ALARM_CYCLE_NO_PRESSURE_STEP                QObject::tr("No Pressure Step")

/* Cycle modified alarm descriptions */
#define ALARM_CYCLE_GROUND_DETECT_ABORT_DESC        QObject::tr("The Ground Detect Input has become active and the Cycle has been aborted.")
#define ALARM_CYCLE_MAXTIMEOUT_DESC                 QObject::tr("The maximum time allowed for Ultrasonic energy to be applied to the part has been reached.")
#define ALARM_CYCLE_NOFORCESTEP_DESC                QObject::tr("The force step trigger was not reached.")
#define ALARM_CYCLE_NO_PRESSURE_STEP_DESC           QObject::tr("The pressure step trigger was not reached.") // Not in E1
#define ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_DESC	QObject::tr("The force at the end of the cycle was greater than the trigger force.") // Not in E1
#define ALARM_CYCLE_LL_NOT_REACHED_ID_DESC			QObject::tr("No Description available")	// Not in E1
#define ALARM_CYCLE_PEAKPOWER_CUTOFF_DESC			QObject::tr("The Peak Power Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_ABSOLUTE_CUTOFF_DESC			QObject::tr("The Absolute Distance Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_COLLAPSE_CUTOFF_DESC			QObject::tr("The Collapse Distance Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_ULTRASONICS_DISABLED_DESC		QObject::tr("The Ultrasonics Disable user input has been enabled during this cycle.")
#define ALARM_CYCLE_CUSTOM1_CUTOFF_DESC				QObject::tr("No Description available")	// Not in E1
#define ALARM_CYCLE_CUSTOM2_CUTOFF_DESC				QObject::tr("No Description available") 	// Not in E1
#define ALARM_CYCLE_FREQLOW_CUTOFF_DESC				QObject::tr("The Frequency Low Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_FREQHIGH_CUTOFF_DESC			QObject::tr("The Frequency High Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_CUST_DIGITAL_CUTOFF_DESC		QObject::tr("No Description available") // Not in E1
#define ALARM_CYCLE_ENERGY_CUTOFF_DESC				QObject::tr("The Energy Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_GD_CUTOFF_DESC					QObject::tr("The Ground Detect Cutoff has triggered during the weld.")
#define ALARM_CYCLE_TIME_CUTOFF_DESC				QObject::tr("The Time Cutoff value has been exceeded during the weld.")

/* Suspect Alarms */
#define ALARM_SUSPECT_ENERGY_LIMIT                  QObject::tr("Energy Suspect Limit")
#define ALARM_SUSPECT_POWER_LIMIT                   QObject::tr("Peak Power Suspect Limit")
#define ALARM_SUSPECT_COLLAPSE_LIMIT                QObject::tr("Collapse Distance Suspect Limit")
#define ALARM_SUSPECT_ABSOLUTE_LIMIT                QObject::tr("Absolute Distance Suspect limit")
#define ALARM_SUSPECT_TRIGGER_LIMIT                 QObject::tr("Trigger Distance Suspect Limit")
#define ALARM_SUSPECT_WELD_FORCE_LIMIT              QObject::tr("Weld Force Suspect  Limit")
#define ALARM_SUSPECT_TIME_LIMIT                    QObject::tr("Time Suspect Limit")
#define ALARM_SUSPECT_VELOCITY_LIMITS               QObject::tr("Velocity Suspect Limit")
#define ALARM_SUSPECT_FREQ_LIMITS                   QObject::tr("Frequency Suspect Limit")

#define ALARM_LOW_ENERGY_LIMIT_DESC                 QObject::tr("The total Weld Energy did not exceed the lower limit value")
#define ALARM_HIGH_ENERGY_LIMIT_DESC                QObject::tr("The total Weld Energy exceeded the upper limit value")
#define ALARM_LOW_POWER_LIMIT_DESC                  QObject::tr("The Peak Power did not exceed the lower limit value")
#define ALARM_HIGH_POWER_LIMIT_DESC                 QObject::tr("The Peak Power exceeded the upper limit value")
#define ALARM_LOW_COLLAPSE_LIMIT_DESC               QObject::tr("The total Collapse Distance did not exceed the lower limit value")
#define ALARM_HIGH_COLLAPSE_LIMIT_DESC              QObject::tr("The total Collapse Distance exceeded the upper limit value")
#define ALARM_LOW_ABSOLUTE_LIMIT_DESC               QObject::tr("The total Absolute Distance did not exceed the lower limit value")
#define ALARM_HIGH_ABSOLUTE_LIMIT_DESC              QObject::tr("The total Absolute Distance exceeded the upper limit value")
#define ALARM_LOW_TRIGGER_LIMIT_DESC                QObject::tr("The Trigger Distance did not exceed the lower limit value")
#define ALARM_HIGH_TRIGGER_LIMIT_DESC               QObject::tr("The Trigger Distance exceeded the upper limit value")
#define ALARM_LOW_WELD_FORCE_LIMIT_DESC             QObject::tr("The total Weld Force did not exceed the lower limit value")
#define ALARM_HIGH_WELD_FORCE_LIMIT_DESC            QObject::tr("The total Weld Force exceeded the upper limit value")
#define ALARM_LOW_TIME_LIMIT_DESC                   QObject::tr("The total Weld Time did not exceed the lower limit value")
#define ALARM_HIGH_TIME_LIMIT_DESC                  QObject::tr("The total Weld Time exceeded the upper limit value")
#define ALARM_LOW_VELOCITY_LIMITS_DESC              QObject::tr("The Weld Velocity did not exceed the lower limit value")
#define ALARM_HIGH_VELOCITY_LIMITS_DESC             QObject::tr("The Weld Velocity exceeded the upper limit value")
#define ALARM_LOW_FREQ_LIMITS_DESC                  QObject::tr("The Weld Frequency did not exceed the lower limit value")
#define ALARM_HIGH_FREQ_LIMITS_DESC                 QObject::tr("The Weld Velocity exceeded the upper limit value")

/* Reject Alarms */
#define ALARM_REJECT_ENERGY_LIMIT                   QObject::tr("Energy Reject Limit")
#define ALARM_REJECT_POWER_LIMIT                    QObject::tr("Peak Power Reject Limit")
#define ALARM_REJECT_COLLAPSE_LIMIT                 QObject::tr("Collapse Distance Reject Limit")
#define ALARM_REJECT_ABSOLUTE_LIMIT                 QObject::tr("Absolute Distance Reject limit")
#define ALARM_REJECT_TRIGGER_LIMIT                  QObject::tr("Trigger Reject Limit")
#define ALARM_REJECT_WELD_FORCE_LIMIT               QObject::tr("Weld Force Reject Limit")
#define ALARM_REJECT_TIME_LIMIT                     QObject::tr("Time Reject Limit")
#define ALARM_REJECT_VELOCITY_LIMITS                QObject::tr("Velocity Reject Limit")
#define ALARM_REJECT_FREQ_LIMITS                    QObject::tr("Frequency Reject Limit")

#define ALARM_POWER_MATCH_CURVE_LIMITS				QObject::tr("Power Match Curve Limits")

/* Warnings */
#define ALARM_WARNING_TRIGGERLOST                   QObject::tr("Trigger Force lost in Weld")
#define ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD         QObject::tr("Actuator clear not reached")
#define ALARM_WARNING_ACTU_RECAL                    QObject::tr("Actuator Recal suggested")
#define ALARM_WARNING_USBMEMORY                     QObject::tr("USB Memory Nearly Full(80%)")
#define ALARM_WARNING_DISK_MEMORY                   QObject::tr("Internal Storage Capacity Warning")
#define ALARM_WARNING_ALARM_LOG                     QObject::tr("Alarm Log Capacity Warning")
#define ALARM_WARNING_EVENT_LOG                     QObject::tr("Event Log Capacity Warning")
#define ALARM_WARNING_WELD_RESULT                   QObject::tr("Weld Result Capacity Warning")
#define ALARM_WARNING_WELD_GRAPH                    QObject::tr("Weld Graph Capacity Warning")
#define ALARM_WARNING_HORN_SCAN                     QObject::tr("Horn Scan Graph Capacity Warning")
#define ALARM_WARNING_EEPROM_CORRUPT                QObject::tr("Possible Data Error")
#define ALARM_WARNING_RECIPE_CORRUPTED				QObject::tr("Possible Recipe Record Corruption")

#define ALARM_WARNING_TRIGGERLOST_DESC              QObject::tr("The applied Force dropped below the minimum Trigger Force during the Cycle.")
#define ALARM_WARNING_DISK_MEMORY_DESC              QObject::tr("Internal Storage is greater than 80% Full. Please consider extracting data to USB to avoid loss of data.")
#define ALARM_WARNING_ALARM_LOG_DESC                QObject::tr("Alarm Log Storage is greater than 80% Full.")
#define ALARM_WARNING_EVENT_LOG_DESC                QObject::tr("Event Log Storage is greater than 80% Full.")
#define ALARM_WARNING_WELD_RESULT_DESC              QObject::tr("Weld Result Storage is greater than 80% Full.")
#define ALARM_WARNING_WELD_GRAPH_DESC               QObject::tr("Weld Graph Storage is greater than 80% Full.")
#define ALARM_WARNING_HORN_SCAN_DESC                QObject::tr("Horn Scan Graph Storage is greater than 80% Full.")
#define ALARM_WARNING_EEPROM_CORRUPT_DESC			QObject::tr("System Configuration potentially incorrect. Double check the System Information")

#define ALARM_WARNING_RECIPE_WELD_CORRUPT_DESC		QObject::tr("Weld Recipe Record may be Corrupted. The last default record has been restored.")
#define ALARM_WARNING_RECIPE_LIMITS_CORRUPT_DESC 	QObject::tr("Suspect and Limits Recipe Record may be Corrupted. The last default record has been restored.")
#define ALARM_WARNING_RECIPE_SETUP_CORRUPT_DESC		QObject::tr("Setup Recipe Record may be Corrupted. The last default record has been restored.")
#define ALARM_WARNING_RECIPE_STACK_CORRUPT_DESC		QObject::tr("Stack Recipe Record may be Corrupted. The last default record has been restored.")

/* Overloads */
#define ALARM_PHASE_OVERLOAD						QObject::tr("Phase Overload")
#define ALARM_CURR_OVERLOAD							QObject::tr("Current Overload")
#define ALARM_FREQ_OVERLOAD							QObject::tr("Frequency Overload")
#define ALARM_POWER_OVERLOAD						QObject::tr("Power Overload")
#define ALARM_VOLT_OVERLOAD							QObject::tr("Voltage Overload")
#define ALARM_TEMP_OVERLOAD							QObject::tr("Temperature Overload")

#define ALARM_PHASE_OVERLOAD_DESC					QObject::tr("Phase Overload occurred during the weld")
#define ALARM_CURR_OVERLOAD_DESC					QObject::tr("Current Overload occurred during the weld")
#define ALARM_FREQ_OVERLOAD_DESC					QObject::tr("Frequency Overload occurred during the weld")
#define ALARM_POWER_OVERLOAD_DESC					QObject::tr("Power Overload occurred during the weld")
#define ALARM_VOLT_OVERLOAD_DESC					QObject::tr("Voltage Overload occurred during the weld")
#define ALARM_TEMP_OVERLOAD_DESC					QObject::tr("Temperature Overload occurred during the weld")
#define ALARM_EB_PHASE_OVERLOAD_DESC				QObject::tr("Phase Overload occurred during Energy Braking")
#define ALARM_EB_CURR_OVERLOAD_DESC					QObject::tr("Current Overload occurred during Energy Braking")
#define ALARM_EB_FREQ_OVERLOAD_DESC					QObject::tr("Frequency Overload occurred during Energy Braking")
#define ALARM_EB_POWER_OVERLOAD_DESC				QObject::tr("Power Overload occurred during Energy Braking")
#define ALARM_EB_VOLT_OVERLOAD_DESC					QObject::tr("Voltage Overload occurred during Energy Braking")
#define ALARM_EB_TEMP_OVERLOAD_DESC					QObject::tr("Temperature Overload occurred during Energy Braking")
#define ALARM_AB_PHASE_OVERLOAD_DESC				QObject::tr("Phase Overload occurred during Afterburst")
#define ALARM_AB_CURR_OVERLOAD_DESC					QObject::tr("Current Overload occurred during Afterburst")
#define ALARM_AB_FREQ_OVERLOAD_DESC					QObject::tr("Frequency Overload occurred during Afterburst")
#define ALARM_AB_POWER_OVERLOAD_DESC				QObject::tr("Power Overload occurred during Afterburst")
#define ALARM_AB_VOLT_OVERLOAD_DESC					QObject::tr("Voltage Overload occurred during Afterburst")
#define ALARM_AB_TEMP_OVERLOAD_DESC					QObject::tr("Temperature Overload occurred during Afterburst")
#define ALARM_PRETRIG_PHASE_OVERLOAD_DESC			QObject::tr("Phase Overload occurred during Pretrigger")
#define ALARM_PRETRIG_CURR_OVERLOAD_DESC			QObject::tr("Current Overload occurred during Pretrigger")
#define ALARM_PRETRIG_FREQ_OVERLOAD_DESC			QObject::tr("Frequency Overload occurred during Pretrigger")
#define ALARM_PRETRIG_POWER_OVERLOAD_DESC			QObject::tr("Power Overload occurred during Pretrigger")
#define ALARM_PRETRIG_VOLT_OVERLOAD_DESC			QObject::tr("Voltage Overload occurred during Pretrigger")
#define ALARM_PRETRIG_TEMP_OVERLOAD_DESC			QObject::tr("Temperature Overload occurred during Pretrigger")
#define ALARM_PRE_SEEK_PHASE_OVERLOAD_DESC			QObject::tr("Phase Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_CURR_OVERLOAD_DESC			QObject::tr("Current Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_FREQ_OVERLOAD_DESC			QObject::tr("Frequency Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_POWER_OVERLOAD_DESC			QObject::tr("Power Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_VOLT_OVERLOAD_DESC			QObject::tr("Voltage Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_TEMP_OVERLOAD_DESC			QObject::tr("Temperature Overload occurred during Pre Weld Seek")
#define ALARM_POST_SEEK_PHASE_OVERLOAD_DESC			QObject::tr("Phase Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_CURR_OVERLOAD_DESC			QObject::tr("Current Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_FREQ_OVERLOAD_DESC			QObject::tr("Frequency Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_POWER_OVERLOAD_DESC			QObject::tr("Power Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_VOLT_OVERLOAD_DESC			QObject::tr("Voltage Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_TEMP_OVERLOAD_DESC			QObject::tr("Temperature Overload occurred during Post Weld Seek")
#define ALARM_SEEK_PHASE_OVERLOAD_DESC				QObject::tr("Phase Overload occurred during Seek")
#define ALARM_SEEK_CURR_OVERLOAD_DESC				QObject::tr("Current Overload occurred during Seek")
#define ALARM_SEEK_FREQ_OVERLOAD_DESC				QObject::tr("Frequency Overload occurred during Seek")
#define ALARM_SEEK_POWER_OVERLOAD_DESC				QObject::tr("Power Overload occurred during Seek")
#define ALARM_SEEK_VOLT_OVERLOAD_DESC				QObject::tr("Voltage Overload occurred during Seek")
#define ALARM_SEEK_TEMP_OVERLOAD_DESC				QObject::tr("Temperature Overload occurred during Seek")
#define ALARM_TEST_PHASE_OVERLOAD_DESC				QObject::tr("Phase Overload occurred during Test")
#define ALARM_TEST_CURR_OVERLOAD_DESC				QObject::tr("Current Overload occurred during Test")
#define ALARM_TEST_FREQ_OVERLOAD_DESC				QObject::tr("Frequency Overload occurred during Test")
#define ALARM_TEST_POWER_OVERLOAD_DESC				QObject::tr("Power Overload occurred during Test")
#define ALARM_TEST_VOLT_OVERLOAD_DESC				QObject::tr("Voltage Overload occurred during Test")
#define ALARM_TEST_TEMP_OVERLOAD_DESC				QObject::tr("Temperature Overload occurred during Test")

/*  No Cycle Overloads - Overloads that happen before trigger or outside the weld cycle */
#define ALARM_SEEK_OVERLOAD                         QObject::tr("Seek Overload")
#define ALARM_TEST_OVERLOAD                         QObject::tr("Test Overload")
#define ALARM_PRETRIGGER_OVERLOAD                   QObject::tr("Pretrigger Overload")
#define ALARM_PREWELDSEEK_OVERLOAD                  QObject::tr("Pre-Weld Seek")
#define ALARM_THERMAL_OVERLOAD                      QObject::tr("Thermal Overload")
#define ALARM_THERMAL_OVERLOAD_DESC                 QObject::tr("Power supply temperature above the maximum operating temperature.")

/* EN Faults */
#define ALARM_MULTIPLE_FAULTS                       QObject::tr("Multiple Faults")
#define ALARM_MULTIPLE_FAULTS_DESC                  QObject::tr("Start Switches activated when system not Ready")
#define ALARM_PALM_BUTTON_FAULT                     QObject::tr("Palm Button Fault")
#define ALARM_24V_FAULT                             QObject::tr("24V Fault")
#define ALARM_ESTOP_FAULT                           QObject::tr("E-Stop Fault")
#define ALARM_LINEAR_ENCODER_FAULT                  QObject::tr("Linear Encoder Fault ")
#define ALARM_LINEAR_ENCODER_FAULT_DESC             QObject::tr("No Description available")
#define ALARM_S_BEAM_FAULT                          QObject::tr("S-Beam Fault")
#define ALARM_TRIGGER_SWITCH_FAULT                  QObject::tr("Trigger Switch Fault")
#define ALARM_DRIVE_FAULT                           QObject::tr("Drive Fault")
#define ALARM_CROSS_MONITORING_FAULT                QObject::tr("Cross Monitoring Fault")
#define ALARM_LOGIC_FAULT                           QObject::tr("Logic Unit Fault ")
#define ALARM_SONICS_ENABLE_FAULT                   QObject::tr("Sonics Enable Fault")

// As per the TOM's comment in the Bug:2676, Unknown alarm should be replaced with empty
#define ALARM_UNKNOW_ALARM                          QObject::tr("")
#define ALARM_NO_DESC                               QObject::tr("No Description available")

#define NOT_AVAIABLE                                QObject::tr("NA")

/**
 * @brief AlarmFromSC::AlarmFromSC : Constructor
 * @param : QObject parent
 */
AlarmFromSC::AlarmFromSC(QObject *parent) : QObject(parent)
{
    context ->setContextProperty("alarmDetails", this);

    /* Connecting a signal alarmRaised from CommunicationInterface.cpp to loadAlarmPopup slot */
    QObject::connect(&CIObj , &CommunicationInterface::alarmRaised , this ,&AlarmFromSC :: loadAlarmPopup );

    /* Connecting a signal emergencyPopupStart from CommunicationInterface.cpp to emergencyStart slot */
    QObject::connect(&CIObj , &CommunicationInterface::emergencyPopupStart , this ,&AlarmFromSC :: emergencyStart );

    /* Connecting a signal emergencyPopupStop from CommunicationInterface.cpp to emergencyStop slot */
    QObject::connect(&CIObj , &CommunicationInterface::emergencyPopupStop , this ,&AlarmFromSC :: emergencyStop );
}

/**
 * @brief AlarmFromSC::dbFullRemindReq : Sends DB full reminder to BL
 */
void AlarmFromSC::dbFullRemindReq()
{
    char ch[8]={'\0'};
    CIObj.sendMessage(UIC_GEN_DB_REPORT_REMIND_LATER_REQ,SCBL_GEN_REPORT_REMIND_LATER_RSP,ch);
}

/**
 * @brief AlarmFromSC::readAlarmDetails : Accepts alarm data and process to UI
 * @param a_alarmData
 */
void AlarmFromSC::readAlarmDetails(QString a_alarmData)
{
    QString alarmId,time,date;
    char reset;
    SysConfigData *sysconfig = SysConfigData::getInstance();
    /* Proceeds if alarm data is not empty*/
    if( !a_alarmData.isEmpty())
    {
        QStringList alarmList = a_alarmData.split(",");
        reset = QString(alarmList[0]).toStdString()[0];
        m_resetStatus = reset - 48;
        for(int dwIndex = 1; dwIndex < alarmList[0].count();dwIndex++)
        {
            alarmId.append(alarmList[0][dwIndex]);
        }
        m_AlarmId = alarmId;
        m_AlarmDate = sysconfig->getDateAndTime().toString("yyyy/MM/dd");
        m_AlarmTime = sysconfig->getDateAndTime().toString("hh:mm:ss");
        for(int i = 1 ;i < alarmList.size();i++)
        {
            reset = QString(alarmList[i]).toStdString()[0] - 48;
            alarmId.clear();
            for(int j = 1; j < alarmList[i].count();j++)
            {
                alarmId.append(alarmList[i][j]);
            }
            date = sysconfig->getDateAndTime().toString("yyyy/MM/dd");
            time = sysconfig->getDateAndTime().toString("hh:mm:ss");
            alarmNotify->fillNotificationList(alarmId,time,date,reset);
        }
    }
    else
    {
        m_AlarmId = NOT_AVAIABLE;
        m_AlarmDate = NOT_AVAIABLE;
        m_AlarmTime = NOT_AVAIABLE;
        m_resetStatus = 0;
        m_errorDesc = NOT_AVAIABLE;
    }
    errorNameforAlarm(m_AlarmId);
    errorDescforAlarm(m_AlarmId);
}

/**
 * @brief AlarmFromSC::errorDescforAlarm
 * @param alarmId
 * @return
 */
QString AlarmFromSC::errorDescforAlarm(QString alarmId)
{
    bool status;
    int hex = alarmId.toInt(&status, 16);
    switch (hex)
    {
    case ALARM_NOCYCLE_EXTERNAL_SONIC_ID:
        m_errorDesc = ALARM_NOCYCLE_EXTERNAL_SONIC_DESC;
        break;
    case ALARM_NOCYCLE_MISSING_PART_ID:
        m_errorDesc = ALARM_NOCYCLE_MISSING_PART_DESC;
        break;
    case ALARM_NOCYCLE_PART_CONTACT_ID:
        m_errorDesc = ALARM_NOCYCLE_PART_CONTACT_DESC;
        break;
    case ALARM_NOCYCLE_EXTERNAL_CYCLE_ID:
        m_errorDesc = ALARM_NOCYCLE_EXTERNAL_CYCLE_DESC;
        break;
    case ALARM_NOCYCLE_ABSOLUTE_DISTANCE_ID:
        m_errorDesc = ALARM_NOCYCLE_ABSOLUTE_DISTANCE_DESC;
        break;
    case ALARM_NOCYCLE_EXTRA_TOOLING_ID:
        m_errorDesc = ALARM_NOCYCLE_EXTRA_TOOLING_DESC;
        break;
    case ALARM_NOCYCLE_EXTRA_TIMEOUT_ID:
        m_errorDesc = ALARM_NOCYCLE_EXTRA_TIMEOUT_DESC;
        break;
    case ALARM_NOCYCLE_PART_PRESENT_ID:
        m_errorDesc = ALARM_NOCYCLE_PART_PRESENT_DESC;
        break;
    case ALARM_NOCYCLE_SERVO_MOTOR_ID:
        m_errorDesc = ALARM_NOCYCLE_SERVO_MOTOR_DESC;
        break;
    case ALARM_NOCYCLE_READY_POS_ID:
        m_errorDesc = ALARM_NOCYCLE_READY_POS_DESC;
        break;
    case ALARM_NOCYCLE_RECIPE_NOTVALID_ID:
        m_errorDesc = ALARM_NOCYCLE_RECIPE_NOTVALID_DESC;
        break;
    case ALARM_NOCYCLE_BATCH_COUNT_ID:
        m_errorDesc = ALARM_NOCYCLE_BATCH_COUNT_DESC;
        break;
    case ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_ID:
        m_errorDesc = ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_DESC;
        break;
    case ALARM_NOCYCLE_PC_ASS_MISMATCH_ID:
        m_errorDesc = ALARM_NOCYCLE_PC_ASS_MISMATCH_DESC;
        break;
    case ALARM_NOCYCLE_AC_ASS_MISMATCH_ID:
        m_errorDesc = ALARM_NOCYCLE_AC_ASS_MISMATCH_DESC;
        break;
    case ALARM_NOCYCLE_STACK_ASS_MISMATCH_ID:
        m_errorDesc = ALARM_NOCYCLE_STACK_ASS_MISMATCH_DESC;
        break;
    case ALARM_HW_START_SWITCH_FAIL_ID:
        m_errorDesc = ALARM_HW_START_SWITCH_FAIL_DESC;
        break;
    case ALARM_HW_ULS_ACTIVE_ID:
        m_errorDesc = ALARM_HW_ULS_ACTIVE_DESC;
        break;
    case ALARM_HW_ULS_NONACTIVE_ID:
        m_errorDesc = ALARM_HW_ULS_NONACTIVE_DESC;
        break;
    case ALARM_HW_GD_BEFORE_TRIGGER_ID:
        m_errorDesc = ALARM_HW_GD_BEFORE_TRIGGER_DESC;
        break;
    case ALARM_HW_START_SWITCH_LOST_ID:
        m_errorDesc = ALARM_HW_START_SWITCH_LOST_DESC;
        break;
    case ALARM_HW_ALARM_LOG_ID:
        m_errorDesc = ALARM_HW_ALARM_LOG_DESC;
        break;
    case ALARM_HW_EVENT_LOG_ID:
        m_errorDesc = ALARM_HW_EVENT_LOG_DESC;
        break;
    case ALARM_HW_WELD_RESULT_ID:
        m_errorDesc = ALARM_HW_WELD_RESULT_DESC;
        break;
    case ALARM_HW_WELD_GRAPH_ID:
        m_errorDesc = ALARM_HW_WELD_GRAPH_DESC;
        break;
    case ALARM_HW_HORN_SCAN_ID:
        m_errorDesc = ALARM_HW_HORN_SCAN_DESC;
        break;
    case ALARM_HW_PRETRIGGER_TIMEOUT_ID:
        m_errorDesc = ALARM_HW_PRETRIGGER_TIMEOUT_DESC;
        break;
    case ALARM_HW_ENCODER_FAIL_ID:
        m_errorDesc = ALARM_HW_ENCODER_FAIL_DESC;
        break;
    case ALARM_HW_DATA_ERROR_ID:
        m_errorDesc = ALARM_HW_DATA_ERROR_DESC;
        break;
    case ALARM_HW_HOME_RETURN_TIMEOUT_ID:
        m_errorDesc = ALARM_HW_HOME_RETURN_TIMEOUT_DESC;
        break;
    case ALARM_HW_ACTUATOR_NOVARM_ID:
        m_errorDesc = ALARM_HW_ACTUATOR_NOVARM_DESC;
        break;
    case ALARM_HW_PS_NOVARM_ID:
        m_errorDesc = ALARM_HW_PS_NOVARM_DESC;
        break;
    case ALARM_HW_START_SWITCH_TIME_ID:
        m_errorDesc = ALARM_HW_START_SWITCH_TIME_DESC;
        break;
    case ALARM_HW_MEM_FULL_ID:
        m_errorDesc = ALARM_HW_MEM_FULL_DESC;
        break;
    case ALARM_HW_INTERNAL_STORAGE_FAIL_ID:
    	m_errorDesc = ALARM_HW_INTERNAL_STORAGE_FAIL_DESC;
    	break;
    case ALARM_HW_RECALIBRATE_ACT_ID:
        m_errorDesc = ALARM_HW_RECALIBRATE_ACT_DESC;
        break;
    case ALARM_HW_ACT_CLR_FUN_ID:
        m_errorDesc = ALARM_HW_ACT_CLR_FUN_DESC;
        break;
    case ALARM_HW_EXTRA_TOOLING_ACTIVE_ID:
        m_errorDesc = ALARM_HW_EXTRA_TOOLING_ACTIVE_DESC;
        break;
    case ALARM_HW_ACT_TYPE_CHANGED_ID:
        m_errorDesc = ALARM_HW_ACT_TYPE_CHANGED_DESC;
        break;
    case ALARM_HW_SYS_PRESSURE_INCORRECT_ID:
        m_errorDesc = ALARM_HW_SYS_PRESSURE_INCORRECT_DESC;
        break;
    case ALARM_HW_SYS_PART_PRESENT_ACTIVE_ID:
        m_errorDesc = ALARM_HW_SYS_PART_PRESENT_ACTIVE_DESC;
        break;
    case ALARM_HW_USB_MEM_LOST_ID:
        m_errorDesc = ALARM_HW_USB_MEM_LOST_DESC;
        break;
    case ALARM_INTERNAL_COMM_ENET:
        m_errorDesc = ALARM_INTERNAL_COMM_FAIL;
        break;
    case ALARM_HW_INTERNAL_BUS_LOST_ID:
        m_errorDesc = ALARM_HW_INTERNAL_BUS_LOST_DESC;
        break;
    case ALARM_HW_CABLE_FAIL_ID:
        m_errorDesc = ALARM_HW_CABLE_FAIL_DESC;
        break;
    case ALARM_HW_PROFINET_OR_IP_NOT_ID:
        m_errorDesc = ALARM_HW_PROFINET_OR_IP_NOT_DESC;
        break;
    case ALARM_HW_AC_LINE_LOST_ID:
        m_errorDesc = ALARM_HW_AC_LINE_LOST_DESC;
        break;
    case ALARM_HW_TRIGGER_ACTIVE_READY_ID:
        m_errorDesc = ALARM_HW_TRIGGER_ACTIVE_READY_DESC;
        break;
    case ALARM_HW_INTERNAL_COMM_FAIL_ID:
        m_errorDesc = ALARM_HW_INTERNAL_COMM_FAIL_DESC;
        break;
    case ALARM_HW_SC_COMPONENT_FAIL_ID:
        m_errorDesc = ALARM_HW_SC_COMPONENT_FAIL_DESC;
        break;
    case ALARM_HW_RTC_LOW_BATT_ID:
        m_errorDesc = ALARM_HW_RTC_LOW_BATT_DESC;
        break;
    case ALARM_CYCLE_GROUND_DETECT_ABORT_ID:
        m_errorDesc = ALARM_CYCLE_GROUND_DETECT_ABORT_DESC;
        break;
    case ALARM_CYCLE_MAXTIMEOUT_ID:
        m_errorDesc = ALARM_CYCLE_MAXTIMEOUT_DESC;
        break;
    case ALARM_CYCLE_NOFORCESTEP_ID:
        m_errorDesc = ALARM_CYCLE_NOFORCESTEP_DESC;
        break;
    case ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_ID:
        m_errorDesc = ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_DESC;
        break;
    case ALARM_CYCLE_LL_NOT_REACHED_ID:	// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_CYCLE_PEAKPOWER_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_PEAKPOWER_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_ABSOLUTE_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_ABSOLUTE_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_COLLAPSE_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_COLLAPSE_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_ULTRASONICS_DISABLED_ID:
        m_errorDesc = ALARM_CYCLE_ULTRASONICS_DISABLED_DESC;
        break;
    case ALARM_CYCLE_CUSTOM1_CUTOFF_ID:	// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_CYCLE_CUSTOM2_CUTOFF_ID: // Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_CYCLE_FREQLOW_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_FREQLOW_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_FREQHIGH_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_FREQHIGH_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_CUST_DIGITAL_CUTOFF_ID:	// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_CYCLE_ENERGY_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_ENERGY_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_GD_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_GD_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_TIME_CUTOFF_ID:
        m_errorDesc = ALARM_CYCLE_TIME_CUTOFF_DESC;
        break;
    case ALARM_CYCLE_NO_PRESSURE_STEP_ID:		// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_ENERGY_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_ENERGY_LIMIT_DESC;
        break;
    case ALARM_ENERGY_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_ENERGY_LIMIT_DESC;
        break;
    case ALARM_POWER_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_POWER_LIMIT_DESC;
        break;
    case ALARM_POWER_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_POWER_LIMIT_DESC;
        break;
    case ALARM_COLLAPSE_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_COLLAPSE_LIMIT_DESC;
        break;
    case ALARM_COLLAPSE_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_COLLAPSE_LIMIT_DESC;
        break;
    case ALARM_ABSOLUTE_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_ABSOLUTE_LIMIT_DESC;
        break;
    case ALARM_ABSOLUTE_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_ABSOLUTE_LIMIT_DESC;
        break;
    case ALARM_TRIGGER_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_TRIGGER_LIMIT_DESC;
        break;
    case ALARM_TRIGGER_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_TRIGGER_LIMIT_DESC;
        break;
    case ALARM_WELD_FORCE_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_WELD_FORCE_LIMIT_DESC;
        break;
    case ALARM_WELD_FORCE_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_WELD_FORCE_LIMIT_DESC;
        break;
    case ALARM_TIME_SLLIMIT_ID:
        m_errorDesc = ALARM_LOW_TIME_LIMIT_DESC;
        break;
    case ALARM_TIME_SHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_TIME_LIMIT_DESC;
        break;
    case ALARM_VELOCITY_SLLIMITS_ID:
        m_errorDesc = ALARM_LOW_VELOCITY_LIMITS_DESC;
        break;
    case ALARM_VELOCITY_SHLIMITS_ID:
        m_errorDesc = ALARM_HIGH_VELOCITY_LIMITS_DESC;
        break;
    case ALARM_FREQ_SLLIMITS_ID:
        m_errorDesc = ALARM_LOW_FREQ_LIMITS_DESC;
        break;
    case ALARM_FREQ_SHLIMITS_ID:
        m_errorDesc = ALARM_HIGH_FREQ_LIMITS_DESC;
        break;
    case ALARM_ENERGY_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_ENERGY_LIMIT_DESC;
        break;
    case ALARM_ENERGY_RHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_ENERGY_LIMIT_DESC;
        break;
    case ALARM_POWER_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_POWER_LIMIT_DESC;
        break;
    case ALARM_POWER_RHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_POWER_LIMIT_DESC;
        break;
    case ALARM_COLLAPSE_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_COLLAPSE_LIMIT_DESC;
        break;
    case ALARM_COLLAPSE_RHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_COLLAPSE_LIMIT_DESC;
        break;
    case ALARM_ABSOLUTE_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_ABSOLUTE_LIMIT_DESC;
        break;
    case ALARM_ABSOLUTE_RHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_ABSOLUTE_LIMIT_DESC;
        break;
    case ALARM_TRIGGER_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_TRIGGER_LIMIT_DESC;
        break;
    case ALARM_TRIGGER_RHLIMIT_ID:
		m_errorDesc = ALARM_HIGH_TRIGGER_LIMIT_DESC;
		break;
    case ALARM_WELD_FORCE_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_WELD_FORCE_LIMIT_DESC;
        break;
    case ALARM_WELD_FORCE_RHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_WELD_FORCE_LIMIT_DESC;
        break;
    case ALARM_TIME_RLLIMIT_ID:
        m_errorDesc = ALARM_LOW_TIME_LIMIT_DESC;
        break;
    case ALARM_TIME_RHLIMIT_ID:
        m_errorDesc = ALARM_HIGH_TIME_LIMIT_DESC;
        break;
    case ALARM_VELOCITY_RLLIMITS_ID:
        m_errorDesc = ALARM_LOW_VELOCITY_LIMITS_DESC;
        break;
    case ALARM_VELOCITY_RHLIMITS_ID:
        m_errorDesc = ALARM_HIGH_VELOCITY_LIMITS_DESC;
        break;
    case ALARM_FREQ_RLLIMITS_ID:
        m_errorDesc = ALARM_LOW_FREQ_LIMITS_DESC;
        break;
    case ALARM_FREQ_RHLIMITS_ID:
        m_errorDesc = ALARM_HIGH_FREQ_LIMITS_DESC;
        break;
    case ALARM_POWER_MATCH_CURVE_RLLIMITS_ID:	// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_POWER_MATCH_CURVE_RHLIMITS_ID:	// Not in E1
        m_errorDesc = ALARM_NO_DESC;    //** Reject
        break;
    case ALARM_WARNING_TRIGGERLOST_ID:
        m_errorDesc = ALARM_WARNING_TRIGGERLOST_DESC;
        break;
    case ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD_ID:	// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_WARNING_ACTU_RECAL_ID:			// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_WARNING_USBMEMORY_ID:			// Not in E1
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_WARNING_DISK_MEMORY_ID:
        m_errorDesc = ALARM_WARNING_DISK_MEMORY_DESC; //**warning
        break;
    case ALARM_WARNING_ALARM_LOG_ID:
        m_errorDesc = ALARM_WARNING_ALARM_LOG_DESC; //**warning
        break;
    case ALARM_WARNING_EVENT_LOG_ID:
        m_errorDesc = ALARM_WARNING_EVENT_LOG_DESC; //**warning
        break;
    case ALARM_WARNING_WELD_RESULT_ID:
        m_errorDesc = ALARM_WARNING_WELD_RESULT_DESC; //**warning
        break;
    case ALARM_WARNING_WELD_GRAPH_ID:
        m_errorDesc = ALARM_WARNING_WELD_GRAPH_DESC; //**warning
        break;
    case ALARM_WARNING_HORN_SCAN_ID:
        m_errorDesc = ALARM_WARNING_HORN_SCAN_DESC; //**warning
        break;
    case ALARM_WARNING_EEPROM_CORRUPT_ID:
        m_errorDesc = ALARM_WARNING_EEPROM_CORRUPT_DESC; //**warning
        break;
        
    case ALARM_WARNING_RECIPE_WELD_CORRUPTED_ID:
        m_errorDesc = ALARM_WARNING_RECIPE_WELD_CORRUPT_DESC;
        break;
    case ALARM_WARNING_RECIPE_LIMITS_CORRUPTED_ID:
        m_errorDesc = ALARM_WARNING_RECIPE_LIMITS_CORRUPT_DESC;
        break;
    case ALARM_WARNING_RECIPE_SETUP_CORRUPTED_ID:
        m_errorDesc = ALARM_WARNING_RECIPE_SETUP_CORRUPT_DESC;
        break;
    case ALARM_WARNING_RECIPE_STACK_CORRUPTED_ID:
        m_errorDesc = ALARM_WARNING_RECIPE_STACK_CORRUPT_DESC;
        break;
        
    case ALARM_PHASE_OVERLOAD_ID:					//** Overloads
    	m_errorDesc = ALARM_PHASE_OVERLOAD_DESC;
    	break;
    case ALARM_CURR_OVERLOAD_ID:
        m_errorDesc = ALARM_CURR_OVERLOAD_DESC;
        break;
    case ALARM_FREQ_OVERLOAD_ID:
        m_errorDesc = ALARM_FREQ_OVERLOAD_DESC;
        break;
    case ALARM_POWER_OVERLOAD_ID:
        m_errorDesc = ALARM_POWER_OVERLOAD_DESC;
        break;
    case ALARM_VOLT_OVERLOAD_ID:
        m_errorDesc = ALARM_VOLT_OVERLOAD_DESC;
        break;
    case ALARM_TEMP_OVERLOAD_ID:
        m_errorDesc = ALARM_TEMP_OVERLOAD_DESC;
        break;
    case ALARM_EB_PHASE_OVERLOAD_ID:					//** Energy Brake Overloads
		m_errorDesc = ALARM_EB_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_EB_CURR_OVERLOAD_ID:
		m_errorDesc = ALARM_EB_CURR_OVERLOAD_DESC;
		break;
	case ALARM_EB_FREQ_OVERLOAD_ID:
		m_errorDesc = ALARM_EB_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_EB_POWER_OVERLOAD_ID:
		m_errorDesc = ALARM_EB_POWER_OVERLOAD_DESC;
		break;
	case ALARM_EB_VOLT_OVERLOAD_ID:
		m_errorDesc = ALARM_EB_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_EB_TEMP_OVERLOAD_ID:
		m_errorDesc = ALARM_EB_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_AB_PHASE_OVERLOAD_ID:					//** Afterburst Overloads
		m_errorDesc = ALARM_AB_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_AB_CURR_OVERLOAD_ID:
		m_errorDesc = ALARM_AB_CURR_OVERLOAD_DESC;
		break;
	case ALARM_AB_FREQ_OVERLOAD_ID:
		m_errorDesc = ALARM_AB_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_AB_POWER_OVERLOAD_ID:
		m_errorDesc = ALARM_AB_POWER_OVERLOAD_DESC;
		break;
	case ALARM_AB_VOLT_OVERLOAD_ID:
		m_errorDesc = ALARM_AB_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_AB_TEMP_OVERLOAD_ID:
		m_errorDesc = ALARM_AB_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_POST_SEEK_PHASE_OVERLOAD_ID:					//** Post Weld Seek Overloads
		m_errorDesc = ALARM_POST_SEEK_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_POST_SEEK_CURR_OVERLOAD_ID:
		m_errorDesc = ALARM_POST_SEEK_CURR_OVERLOAD_DESC;
		break;
	case ALARM_POST_SEEK_FREQ_OVERLOAD_ID:
		m_errorDesc = ALARM_POST_SEEK_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_POST_SEEK_POWER_OVERLOAD_ID:
		m_errorDesc = ALARM_POST_SEEK_POWER_OVERLOAD_DESC;
		break;
	case ALARM_POST_SEEK_VOLT_OVERLOAD_ID:
		m_errorDesc = ALARM_POST_SEEK_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_POST_SEEK_TEMP_OVERLOAD_ID:
		m_errorDesc = ALARM_POST_SEEK_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_SEEK_OVERLOAD_PHASE_ID:					//** Seek Overloads
		m_errorDesc = ALARM_SEEK_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_SEEK_OVERLOAD_CURRENT_ID:
		m_errorDesc = ALARM_SEEK_CURR_OVERLOAD_DESC;
		break;
	case ALARM_SEEK_OVERLOAD_FREQUENCY_ID:
		m_errorDesc = ALARM_SEEK_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_SEEK_OVERLOAD_POWER_ID:
		m_errorDesc = ALARM_SEEK_POWER_OVERLOAD_DESC;
		break;
	case ALARM_SEEK_OVERLOAD_VOLTAGE_ID:
		m_errorDesc = ALARM_SEEK_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_SEEK_OVERLOAD_TEMPERATURE_ID:
		m_errorDesc = ALARM_SEEK_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_TEST_OVERLOAD_PHASE_ID:					//** Test Overloads
		m_errorDesc = ALARM_TEST_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_TEST_OVERLOAD_CURRENT_ID:
		m_errorDesc = ALARM_TEST_CURR_OVERLOAD_DESC;
		break;
	case ALARM_TEST_OVERLOAD_FREQUENCY_ID:
		m_errorDesc = ALARM_TEST_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_TEST_OVERLOAD_POWER_ID:
		m_errorDesc = ALARM_TEST_POWER_OVERLOAD_DESC;
		break;
	case ALARM_TEST_OVERLOAD_VOLTAGE_ID:
		m_errorDesc = ALARM_TEST_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_TEST_OVERLOAD_TEMPERATURE_ID:
		m_errorDesc = ALARM_TEST_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_PRETRIGGER_OVERLOAD_PHASE_ID:					//** Pretrigger Overloads
		m_errorDesc = ALARM_PRETRIG_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_CURRENT_ID:
		m_errorDesc = ALARM_PRETRIG_CURR_OVERLOAD_DESC;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_FREQUENCY_ID:
		m_errorDesc = ALARM_PRETRIG_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_POWER_ID:
		m_errorDesc = ALARM_PRETRIG_POWER_OVERLOAD_DESC;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_VOLTAGE_ID:
		m_errorDesc = ALARM_PRETRIG_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_TEMPERATURE_ID:
		m_errorDesc = ALARM_PRETRIG_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_PRE_SEEK_PHASE_OVERLOAD_ID:					//** Pre Weld Seek Overloads
		m_errorDesc = ALARM_PRE_SEEK_PHASE_OVERLOAD_DESC;
		break;
	case ALARM_PRE_SEEK_CURR_OVERLOAD_ID:
		m_errorDesc = ALARM_PRE_SEEK_CURR_OVERLOAD_DESC;
		break;
	case ALARM_PRE_SEEK_FREQ_OVERLOAD_ID:
		m_errorDesc = ALARM_PRE_SEEK_FREQ_OVERLOAD_DESC;
		break;
	case ALARM_PRE_SEEK_POWER_OVERLOAD_ID:
		m_errorDesc = ALARM_PRE_SEEK_POWER_OVERLOAD_DESC;
		break;
	case ALARM_PRE_SEEK_VOLT_OVERLOAD_ID:
		m_errorDesc = ALARM_PRE_SEEK_VOLT_OVERLOAD_DESC;
		break;
	case ALARM_PRE_SEEK_TEMP_OVERLOAD_ID:
		m_errorDesc = ALARM_PRE_SEEK_TEMP_OVERLOAD_DESC;
		break;
    case ALARM_MULTIPLE_FAULTS_ID:
        m_errorDesc = ALARM_MULTIPLE_FAULTS_DESC;
        break;
    case ALARM_PALM_BUTTON_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_24V_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_ESTOP_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_LINEAR_ENCODER_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_S_BEAM_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_TRIGGER_SWITCH_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_DRIVE_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_CROSS_MONITORING_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_LOGIC_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    case ALARM_SONICS_ENABLE_FAULT_ID:
        m_errorDesc = ALARM_NO_DESC;
        break;
    default:
        m_errorDesc = ALARM_UNKNOW_ALARM;
        break;
    }
    return m_errorDesc;
}

/**
 * @brief AlarmFromSC::errorNameforAlarm : Assigning error description to the particular alarm
 * @param : QString alarmId
 * @return
 */
QString AlarmFromSC::errorNameforAlarm(QString alarmId)
{    
    bool status;
    int hex = alarmId.toInt(&status, 16);

    switch (hex)
    {
    case ALARM_NOCYCLE_EXTERNAL_SONIC_ID:
        m_errorName = ALARM_NOCYCLE_EXTERNAL_SONIC;
        break;
    case ALARM_NOCYCLE_MISSING_PART_ID:
        m_errorName = ALARM_NOCYCLE_MISSING_PART;
        break;
    case ALARM_NOCYCLE_PART_CONTACT_ID:
        m_errorName = ALARM_NOCYCLE_PART_CONTACT;
        break;
    case ALARM_NOCYCLE_EXTERNAL_CYCLE_ID:
        m_errorName = ALARM_NOCYCLE_EXTERNAL_CYCLE;
        break;
    case ALARM_NOCYCLE_ABSOLUTE_DISTANCE_ID:
        m_errorName = ALARM_NOCYCLE_ABSOLUTE_DISTANCE;
        break;
    case ALARM_NOCYCLE_EXTRA_TOOLING_ID:
        m_errorName = ALARM_NOCYCLE_EXTRA_TOOLING;
        break;
    case ALARM_NOCYCLE_EXTRA_TIMEOUT_ID:
        m_errorName = ALARM_NOCYCLE_EXTRA_TIMEOUT;
        break;
    case ALARM_NOCYCLE_PART_PRESENT_ID:
        m_errorName = ALARM_NOCYCLE_PART_PRESENT;
        break;
    case ALARM_NOCYCLE_SERVO_MOTOR_ID:
        m_errorName = ALARM_NOCYCLE_SERVO_MOTOR;
        break;
    case ALARM_NOCYCLE_READY_POS_ID:
        m_errorName = ALARM_NOCYCLE_READY_POS;
        break;
    case ALARM_NOCYCLE_RECIPE_NOTVALID_ID:
        m_errorName = ALARM_NOCYCLE_RECIPE_NOTVALID;
        break;
    case ALARM_NOCYCLE_BATCH_COUNT_ID:
        m_errorName = ALARM_NOCYCLE_BATCH_COUNT;
        break;
    case ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_ID:
        m_errorName = ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID;
        break;
    case ALARM_NOCYCLE_PC_ASS_MISMATCH_ID:
        m_errorName = ALARM_NOCYCLE_PC_ASS_MISMATCH;
        break;
    case ALARM_NOCYCLE_AC_ASS_MISMATCH_ID:
        m_errorName = ALARM_NOCYCLE_AC_ASS_MISMATCH;
        break;
    case ALARM_NOCYCLE_STACK_ASS_MISMATCH_ID:
        m_errorName = ALARM_NOCYCLE_STACK_ASS_MISMATCH;
        break;
    case ALARM_HW_START_SWITCH_FAIL_ID:
        m_errorName = ALARM_HW_START_SWITCH_FAIL;
        break;
    case ALARM_HW_ULS_ACTIVE_ID:
        m_errorName = ALARM_HW_ULS_ACTIVE;
        break;
    case ALARM_HW_ULS_NONACTIVE_ID:
        m_errorName = ALARM_HW_ULS_NONACTIVE;
        break;
    case ALARM_HW_GD_BEFORE_TRIGGER_ID:
        m_errorName = ALARM_HW_GD_BEFORE_TRIGGER;
        break;
    case ALARM_HW_START_SWITCH_LOST_ID:
        m_errorName = ALARM_HW_START_SWITCH_LOST;
        break;
    case ALARM_HW_ALARM_LOG_ID:
        m_errorName = ALARM_HW_ALARM_LOG;
        break;
    case ALARM_HW_EVENT_LOG_ID:
        m_errorName = ALARM_HW_EVENT_LOG;
        break;
    case ALARM_HW_WELD_RESULT_ID:
        m_errorName = ALARM_HW_WELD_RESULT;
        break;
    case ALARM_HW_WELD_GRAPH_ID:
        m_errorName = ALARM_HW_WELD_GRAPH;
        break;
    case ALARM_HW_HORN_SCAN_ID:
        m_errorName = ALARM_HW_HORN_SCAN;
        break;
    case ALARM_HW_PRETRIGGER_TIMEOUT_ID:
        m_errorName = ALARM_HW_PRETRIGGER_TIMEOUT;
        break;
    case ALARM_HW_ENCODER_FAIL_ID:
        m_errorName = ALARM_HW_ENCODER_FAIL;
        break;
    case ALARM_HW_DATA_ERROR_ID:
        m_errorName = ALARM_HW_DATA_ERROR;
        break;
    case ALARM_HW_HOME_RETURN_TIMEOUT_ID:
        m_errorName = ALARM_HW_HOME_RETURN_TIMEOUT;
        break;
    case ALARM_HW_ACTUATOR_NOVARM_ID:
        m_errorName = ALARM_HW_ACTUATOR_NOVARM;
        break;
    case ALARM_HW_PS_NOVARM_ID:
        m_errorName = ALARM_HW_PS_NOVARM;
        break;
    case ALARM_HW_START_SWITCH_TIME_ID:
        m_errorName = ALARM_HW_START_SWITCH_TIME;
        break;
    case ALARM_HW_MEM_FULL_ID:
        m_errorName = ALARM_HW_MEM_FULL;
        break;
    case ALARM_HW_INTERNAL_STORAGE_FAIL_ID:
		m_errorName = ALARM_HW_INTERNAL_STORAGE_FAIL;
		break;
    case ALARM_HW_RECALIBRATE_ACT_ID:
        m_errorName = ALARM_HW_RECALIBRATE_ACT;
        break;
    case ALARM_HW_ACT_CLR_FUN_ID:
        m_errorName = ALARM_HW_ACT_CLR_FUN;
        break;
    case ALARM_HW_EXTRA_TOOLING_ACTIVE_ID:
        m_errorName = ALARM_HW_EXTRA_TOOLING_ACTIVE;
        break;
    case ALARM_HW_ACT_TYPE_CHANGED_ID:
        m_errorName = ALARM_HW_ACT_TYPE_CHANGED;
        break;
    case ALARM_HW_SYS_PRESSURE_INCORRECT_ID:
        m_errorName = ALARM_HW_SYS_PRESSURE_INCORRECT;
        break;
    case ALARM_HW_SYS_PART_PRESENT_ACTIVE_ID:
        m_errorName = ALARM_HW_SYS_PART_PRESENT_ACTIVE;
        break;
    case ALARM_HW_USB_MEM_LOST_ID:
        m_errorName = ALARM_HW_USB_MEM_LOST;
        break;
    case ALARM_INTERNAL_COMM_ENET:
        m_errorName = ALARM_CONNECTION_LOST;
        break;
    case ALARM_HW_INTERNAL_BUS_LOST_ID:
        m_errorName = ALARM_HW_INTERNAL_BUS_LOST;
        break;
    case ALARM_HW_CABLE_FAIL_ID:
        m_errorName = ALARM_HW_CABLE_FAIL;
        break;
    case ALARM_HW_PROFINET_OR_IP_NOT_ID:
        m_errorName = ALARM_HW_PROFINET_OR_IP_NOT;
        break;
    case ALARM_HW_AC_LINE_LOST_ID:
        m_errorName = ALARM_HW_AC_LINE_LOST;
        break;
    case ALARM_HW_TRIGGER_ACTIVE_READY_ID:
        m_errorName = ALARM_HW_TRIGGER_ACTIVE_READY;
        break;
    case ALARM_HW_INTERNAL_COMM_FAIL_ID:
        m_errorName = ALARM_HW_INTERNAL_COMM_FAIL;
        break;
    case ALARM_HW_SC_COMPONENT_FAIL_ID:
        m_errorName = ALARM_HW_SC_COMPONENT_FAIL;
        break;
    case ALARM_HW_RTC_LOW_BATT_ID:
        m_errorName = ALARM_HW_SC_COMPONENT_FAIL;
        break;
    case ALARM_CYCLE_GROUND_DETECT_ABORT_ID:
        m_errorName = ALARM_CYCLE_GROUND_DETECT_ABORT;
        break;
    case ALARM_CYCLE_MAXTIMEOUT_ID:
        m_errorName = ALARM_CYCLE_MAXTIMEOUT;
        break;
    case ALARM_CYCLE_NOFORCESTEP_ID:
        m_errorName = ALARM_CYCLE_NOFORCESTEP;
        break;
    case ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_ID:
        m_errorName = ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE;
        break;
    case ALARM_CYCLE_LL_NOT_REACHED_ID:
        m_errorName = ALARM_CYCLE_LL_NOT_REACHED;
        break;
    case ALARM_CYCLE_PEAKPOWER_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_PEAKPOWER_CUTOFF;
        break;
    case ALARM_CYCLE_ABSOLUTE_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_ABSOLUTE_CUTOFF;
        break;
    case ALARM_CYCLE_COLLAPSE_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_COLLAPSE_CUTOFF;
        break;
    case ALARM_CYCLE_ULTRASONICS_DISABLED_ID:
        m_errorName = ALARM_CYCLE_ULTRASONICS_DISABLED;
        break;
    case ALARM_CYCLE_CUSTOM1_CUTOFF_ID:		// Not in E1
        m_errorName = ALARM_CYCLE_CUSTOM1_CUTOFF;
        break;
    case ALARM_CYCLE_CUSTOM2_CUTOFF_ID:		// Not in E1
        m_errorName = ALARM_CYCLE_CUSTOM2_CUTOFF;
        break;
    case ALARM_CYCLE_FREQLOW_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_FREQLOW_CUTOFF;
        break;
    case ALARM_CYCLE_FREQHIGH_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_FREQHIGH_CUTOFF;
        break;
    case ALARM_CYCLE_CUST_DIGITAL_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_CUST_DIGITAL_CUTOFF;
        break;
    case ALARM_CYCLE_ENERGY_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_ENERGY_CUTOFF;
        break;
    case ALARM_CYCLE_GD_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_GD_CUTOFF;
        break;
    case ALARM_CYCLE_TIME_CUTOFF_ID:
        m_errorName = ALARM_CYCLE_TIME_CUTOFF;
        break;
    case ALARM_CYCLE_NO_PRESSURE_STEP_ID:
        m_errorName = ALARM_CYCLE_NO_PRESSURE_STEP;
        break;
    case ALARM_ENERGY_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_ENERGY_LIMIT;
        break;
    case ALARM_ENERGY_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_ENERGY_LIMIT;
        break;
    case ALARM_POWER_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_POWER_LIMIT;
        break;
    case ALARM_POWER_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_POWER_LIMIT;
        break;
    case ALARM_COLLAPSE_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_COLLAPSE_LIMIT;
        break;
    case ALARM_COLLAPSE_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_COLLAPSE_LIMIT;
        break;
    case ALARM_ABSOLUTE_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_ABSOLUTE_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_TRIGGER_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_TRIGGER_LIMIT;
        break;
    case ALARM_TRIGGER_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_TRIGGER_LIMIT;
        break;
    case ALARM_WELD_FORCE_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_WELD_FORCE_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_TIME_SLLIMIT_ID:
        m_errorName = ALARM_SUSPECT_TIME_LIMIT;
        break;
    case ALARM_TIME_SHLIMIT_ID:
        m_errorName = ALARM_SUSPECT_TIME_LIMIT;
        break;
    case ALARM_VELOCITY_SLLIMITS_ID:
        m_errorName = ALARM_SUSPECT_VELOCITY_LIMITS;
        break;
    case ALARM_VELOCITY_SHLIMITS_ID:
        m_errorName = ALARM_SUSPECT_VELOCITY_LIMITS;
        break;
    case ALARM_FREQ_SLLIMITS_ID:
        m_errorName = ALARM_SUSPECT_FREQ_LIMITS;
        break;
    case ALARM_FREQ_SHLIMITS_ID:
        m_errorName = ALARM_SUSPECT_FREQ_LIMITS;
        break;
    case ALARM_ENERGY_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_ENERGY_LIMIT;
        break;
    case ALARM_ENERGY_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_ENERGY_LIMIT;
        break;
    case ALARM_POWER_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_POWER_LIMIT;
        break;
    case ALARM_POWER_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_POWER_LIMIT;
        break;
    case ALARM_COLLAPSE_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_COLLAPSE_LIMIT;
        break;
    case ALARM_COLLAPSE_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_COLLAPSE_LIMIT;
        break;
    case ALARM_ABSOLUTE_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_ABSOLUTE_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_TRIGGER_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_TRIGGER_LIMIT;
        break;
    case ALARM_TRIGGER_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_TRIGGER_LIMIT;
        break;
    case ALARM_WELD_FORCE_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_WELD_FORCE_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_TIME_RLLIMIT_ID:
        m_errorName = ALARM_REJECT_TIME_LIMIT;
        break;
    case ALARM_TIME_RHLIMIT_ID:
        m_errorName = ALARM_REJECT_TIME_LIMIT;
        break;
    case ALARM_FREQ_RLLIMITS_ID:
        m_errorName = ALARM_REJECT_FREQ_LIMITS;
        break;
    case ALARM_FREQ_RHLIMITS_ID:
        m_errorName = ALARM_REJECT_FREQ_LIMITS;
        break;
    case ALARM_POWER_MATCH_CURVE_RLLIMITS_ID:
        m_errorName = ALARM_POWER_MATCH_CURVE_LIMITS;
        break;
    case ALARM_POWER_MATCH_CURVE_RHLIMITS_ID:
        m_errorName = ALARM_POWER_MATCH_CURVE_LIMITS;
        break;
    case ALARM_WARNING_TRIGGERLOST_ID:
        m_errorName = ALARM_WARNING_TRIGGERLOST;
        break;
    case ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD_ID:
        m_errorName = ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD;
        break;
    case ALARM_WARNING_ACTU_RECAL_ID:
        m_errorName = ALARM_WARNING_ACTU_RECAL;
        break;
    case ALARM_WARNING_USBMEMORY_ID:
        m_errorName = ALARM_WARNING_USBMEMORY;
        break;
    case ALARM_WARNING_DISK_MEMORY_ID:
        m_errorName = ALARM_WARNING_DISK_MEMORY;
        break;
    case ALARM_WARNING_ALARM_LOG_ID:
        m_errorName = ALARM_WARNING_ALARM_LOG;
        break;
    case ALARM_WARNING_EVENT_LOG_ID:
        m_errorName = ALARM_WARNING_EVENT_LOG;
        break;
    case ALARM_WARNING_WELD_RESULT_ID:
        m_errorName = ALARM_WARNING_WELD_RESULT;
        break;
    case ALARM_WARNING_WELD_GRAPH_ID:
        m_errorName = ALARM_WARNING_WELD_GRAPH;
        break;
    case ALARM_WARNING_HORN_SCAN_ID:
        m_errorName = ALARM_WARNING_HORN_SCAN;
        break;
    case ALARM_WARNING_EEPROM_CORRUPT_ID:
        m_errorName = ALARM_WARNING_EEPROM_CORRUPT; //**warning
        break;
        
    case ALARM_WARNING_RECIPE_WELD_CORRUPTED_ID:
    case ALARM_WARNING_RECIPE_LIMITS_CORRUPTED_ID:
    case ALARM_WARNING_RECIPE_SETUP_CORRUPTED_ID:
    case ALARM_WARNING_RECIPE_STACK_CORRUPTED_ID:
    	m_errorName = ALARM_WARNING_RECIPE_CORRUPTED;
        break;
        
    case ALARM_PHASE_OVERLOAD_ID:					//** Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
    	break;
    case ALARM_CURR_OVERLOAD_ID:
    	m_errorName = ALARM_CURR_OVERLOAD;
        break;
    case ALARM_FREQ_OVERLOAD_ID:
    	m_errorName = ALARM_FREQ_OVERLOAD;
        break;
    case ALARM_POWER_OVERLOAD_ID:
    	m_errorName = ALARM_POWER_OVERLOAD;
        break;
    case ALARM_VOLT_OVERLOAD_ID:
    	m_errorName = ALARM_VOLT_OVERLOAD;
        break;
    case ALARM_TEMP_OVERLOAD_ID:
    	m_errorName = ALARM_TEMP_OVERLOAD;
        break;
    case ALARM_EB_PHASE_OVERLOAD_ID:					//** Energy Brake Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_EB_CURR_OVERLOAD_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_EB_FREQ_OVERLOAD_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_EB_POWER_OVERLOAD_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_EB_VOLT_OVERLOAD_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_EB_TEMP_OVERLOAD_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_AB_PHASE_OVERLOAD_ID:					//** Afterburst Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_AB_CURR_OVERLOAD_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_AB_FREQ_OVERLOAD_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_AB_POWER_OVERLOAD_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_AB_VOLT_OVERLOAD_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_AB_TEMP_OVERLOAD_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_POST_SEEK_PHASE_OVERLOAD_ID:					//** Post Weld Seek Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_POST_SEEK_CURR_OVERLOAD_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_POST_SEEK_FREQ_OVERLOAD_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_POST_SEEK_POWER_OVERLOAD_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_POST_SEEK_VOLT_OVERLOAD_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_POST_SEEK_TEMP_OVERLOAD_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_SEEK_OVERLOAD_PHASE_ID:					//** Seek Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_CURRENT_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_FREQUENCY_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_POWER_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_VOLTAGE_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_TEMPERATURE_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_TEST_OVERLOAD_PHASE_ID:					//** Test Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_CURRENT_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_FREQUENCY_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_POWER_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_VOLTAGE_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_TEMPERATURE_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_PRETRIGGER_OVERLOAD_PHASE_ID:					//** Pretrigger Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_CURRENT_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_FREQUENCY_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_POWER_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_VOLTAGE_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_TEMPERATURE_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_PRE_SEEK_PHASE_OVERLOAD_ID:					//** Pre Weld Seek Overloads
    	m_errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_CURR_OVERLOAD_ID:
		m_errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_FREQ_OVERLOAD_ID:
		m_errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_POWER_OVERLOAD_ID:
		m_errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_VOLT_OVERLOAD_ID:
		m_errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_TEMP_OVERLOAD_ID:
		m_errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_MULTIPLE_FAULTS_ID:
        m_errorName = ALARM_MULTIPLE_FAULTS;
        break;
    case ALARM_PALM_BUTTON_FAULT_ID:
        m_errorName = ALARM_PALM_BUTTON_FAULT;
        break;
    case ALARM_24V_FAULT_ID:
        m_errorName = ALARM_24V_FAULT;
        break;
    case ALARM_ESTOP_FAULT_ID:
        m_errorName = ALARM_ESTOP_FAULT;
        break;
    case ALARM_LINEAR_ENCODER_FAULT_ID:
        m_errorName = ALARM_LINEAR_ENCODER_FAULT;
        break;
    case ALARM_S_BEAM_FAULT_ID:
        m_errorName = ALARM_S_BEAM_FAULT;
        break;
    case ALARM_TRIGGER_SWITCH_FAULT_ID:
        m_errorName = ALARM_TRIGGER_SWITCH_FAULT;
        break;
    case ALARM_DRIVE_FAULT_ID:
        m_errorName = ALARM_DRIVE_FAULT;
        break;
    case ALARM_CROSS_MONITORING_FAULT_ID:
        m_errorName = ALARM_CROSS_MONITORING_FAULT;
        break;
    case ALARM_LOGIC_FAULT_ID:
        m_errorName = ALARM_LOGIC_FAULT;
        break;
    case ALARM_SONICS_ENABLE_FAULT_ID:
        m_errorName = ALARM_SONICS_ENABLE_FAULT;
        break;
    default:
        m_errorName = ALARM_UNKNOW_ALARM;
        break;
    }
    return m_errorName;
}

/**
 * @brief AlarmFromSC::sendAlarmToSCToReset : Sends alarm reset request to BL
 * @param : QString a_errorName
 * @return : int
 */
int AlarmFromSC::sendAlarmToSCToReset(QString a_errorName)
{
    int status;
    char *buff;
    char resetStatus[4]={'\0'};
    std::string fname = a_errorName.toStdString();
    buff = new char [fname.size()+1];
    strcpy( buff, fname.c_str() );

    /* Send message to BL */
    CIObj.sendMessage(SCBL_ALARM_RESET_REQ ,UIC_ALARM_REST_RES,buff,sizeof(a_errorName));
    CIObj.recivedMessage(UIC_ALARM_REST_RES,resetStatus,1);

    status = resetStatus[0]-48;
    delete []buff;
    buff = NULL;
    return status;
}

/**
 * @brief AlarmFromSC::sendResetAllRequestToSC : Sends reset all alarms request to BL
 * @return : int
 */
int AlarmFromSC::sendResetAllRequestToSC()
{
    int status;
    char *buff = NULL;
    char resetStatus[4]={'\0'};

    /* Send message to BL */
    CIObj.sendMessage(SCBL_ALARM_RESET_ALL_REQ ,UIC_ALARM_REST_ALL_RES,buff,0);
    CIObj.recivedMessage(UIC_ALARM_REST_ALL_RES,resetStatus,1);

    status = resetStatus[0]-48;
    return status;
}

/**
 * @brief AlarmFromSC::getAlarmId : Returns alarm ID
 * @return : QString
 */
QString AlarmFromSC::getAlarmId()
{
    return m_AlarmId;
}

/**
 * @brief AlarmFromSC::getAlarmTime : Returns alarm time
 * @return : QString
 */
QString AlarmFromSC::getAlarmTime()
{
    return m_AlarmTime;
}

/**
 * @brief AlarmFromSC::getAlarmDate : Returns alarm date
 * @return : QString
 */
QString AlarmFromSC::getAlarmDate()
{
    return m_AlarmDate;
}

/**
 * @brief AlarmFromSC::getReset : Returns alarm reset status
 * @return : int
 */
int AlarmFromSC::getReset()
{
    return m_resetStatus;
}

/**
 * @brief AlarmFromSC::getErrorDesc : Returns alarm error description
 * @return : QString
 */
QString AlarmFromSC::getErrorDesc()
{
    return m_errorDesc;
}

/**
 * @brief AlarmFromSC::getErrorName : Returns alarm name
 * @return : QString
 */
QString AlarmFromSC::getErrorName()
{
    return m_errorName;
}

/**
 * @brief AlarmFromSC::setAlarmID : Sets the alarm ID
 * @param : QString a_AlarmName
 */
void AlarmFromSC::setAlarmID(QString a_AlarmName)
{
    if(m_AlarmId != a_AlarmName)
    {
        m_AlarmId = a_AlarmName;
        emit OnAlarmIDChanged();
    }
}

/**
 * @brief AlarmFromSC::setAlarmTime : Sets the alarm time
 * @param : QString a_AlarmTime
 */
void AlarmFromSC::setAlarmTime(QString a_AlarmTime)
{
    if(m_AlarmTime != a_AlarmTime)
    {
        m_AlarmTime = a_AlarmTime;
        emit OnAlarmTimeChanged();
    }
}

/**
 * @brief AlarmFromSC::setAlarmDate : Sets the alarm date
 * @param : QString a_AlarmDate
 */
void AlarmFromSC::setAlarmDate(QString a_AlarmDate)
{
    if(m_AlarmDate != a_AlarmDate)
    {
        m_AlarmDate = a_AlarmDate;
        emit OnAlarmDateChanged();
    }
}

/**
 * @brief AlarmFromSC::setReset : Sets the alarm reset status
 * @param : int a_resetStatus
 */
void AlarmFromSC::setReset(int a_resetStatus)
{
    if(m_resetStatus != a_resetStatus)
    {
        m_resetStatus = a_resetStatus;
        emit OnResetChanged();
    }
}

/**
 * @brief AlarmFromSC::setErrorDesc : Sets alarm description
 * @param : QString errorDesc
 */
void AlarmFromSC::setErrorDesc(QString errorDesc)
{
    if(m_errorDesc != errorDesc)
    {
        m_errorDesc = errorDesc;
        emit OnErrorDescChanged();
    }
}

/**
 * @brief AlarmFromSC::setErrorName : Sets alarm name
 * @param errorName
 */
void AlarmFromSC::setErrorName(QString errorName)
{
    if(m_errorName != errorName)
    {
        m_errorName = errorName;
        emit OnErrorNameChanged();
    }
}

/**
 * @brief AlarmFromSC::uicStatus : Updates uic status whether user logged or not
 * @param : bool a_status
 */
void AlarmFromSC::uicStatus(bool a_status)
{
    loginStatus = a_status;
}

/**
 * @brief AlarmFromSC::alarmPopupStatus : Updates alarm popup opened or not
 * @param : bool a_status
 */
void AlarmFromSC::alarmPopupStatus(bool a_status)
{
    bAlarmPopupStatusFlag = a_status;
}

/**
 * @brief AlarmFromSC::loadAlarmPopup : Opens alarm popup or if user not logged in
 *                                      or alarm popup is already opened then details
 *                                      will be stored in notifications
 *
 * @param : QString a_alarmData
 */
void AlarmFromSC::loadAlarmPopup(QString a_alarmData)
{
    if((loginStatus == true) && (bAlarmPopupStatusFlag == false))
    {

        if(alarmNotify->checkBusScanAlarmInNotificationList(a_alarmData))
        {
            readAlarmDetails(a_alarmData);
            /* Invokes a qml function raiseAlarmPopup */
            QMetaObject::invokeMethod(object, "raiseAlarmPopup");
        }
    }
    else
    {
        alarmNotify->storeAlarmDetails(a_alarmData);

        /* Invokes a qml function alarmRaisedWithoutLogin */
        QMetaObject::invokeMethod(object, "alarmRaisedWithoutLogin");
    }
}

/**
 * @brief AlarmFromSC::sendResetEmergencyReq : Sends emergency reset request to BL
 * @return : int
 */
int AlarmFromSC::sendResetEmergencyReq()
{
    int status;
    char requestMessage[] = "";
    char resetStatus[4]={'\0'};

    /* Send message to BL*/
    CIObj.sendMessage(UIC_EMERGENCY_STOP_HORN_HOME_REQ ,UIC_EMERGENCY_STOP_HORN_HOME_RES,requestMessage,0);
    CIObj.recivedMessage(UIC_EMERGENCY_STOP_HORN_HOME_RES,resetStatus,1);

    status = resetStatus[0]-48;
    return status;
}

/**
 * @brief AlarmFromSC::emergencyStart : Opens emergency popup
 */
void AlarmFromSC::emergencyStart()
{
    /* Invokes a qml function emergencyStart */
    QMetaObject::invokeMethod(object,"emergencyStart");
}

/**
 * @brief AlarmFromSC::emergencyStop : Closes emergency popup
 */
void AlarmFromSC::emergencyStop()
{
    /* Invokes a qml function emergencyStop */
    QMetaObject::invokeMethod(object,"emergencyStop");
}

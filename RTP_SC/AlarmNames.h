#include <string>

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
#define ALARM_HW_INTERNAL_BUS_SCAN_FAILURE_ID   1593
#define ALARM_HW_CABLE_FAIL_ID                  1594
#define ALARM_HW_PROFINET_OR_IP_NOT_ID          1595
#define ALARM_HW_AC_LINE_LOST_ID                1596
#define ALARM_HW_TRIGGER_ACTIVE_READY_ID        1597
#define ALARM_HW_INTERNAL_COMM_FAIL_ID          1598
#define ALARM_HW_SC_COMPONENT_FAIL_ID           1599

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

#define ALARM_RECIPE_WELD_CORRUPTED_ID			1105
#define ALARM_RECIPE_LIMITS_CORRUPTED_ID		1106
#define ALARM_RECIPE_SETUP_CORRUPTED_ID			1107
#define ALARM_RECIPE_STACK_CORRUPTED_ID			1108

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
#define ALARM_SEEK_OVERLOAD_PHASE_ID           		  2145	// base number for Pretrigger overloads
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

/* No Cycle Alarm names */
#define ALARM_NOCYCLE_EXTERNAL_SONIC                "External Sonics Delay Timeout"
#define ALARM_NOCYCLE_MISSING_PART                  ("Part Window Abort")
#define ALARM_NOCYCLE_PART_CONTACT                  ("Part Contact before Pretrigger")
#define ALARM_NOCYCLE_EXTERNAL_CYCLE                ("External Cycle Abort")
#define ALARM_NOCYCLE_ABSOLUTE_DISTANCE             ("Invalid Part Contact Distance")
#define ALARM_NOCYCLE_EXTRA_TOOLING                 ("External Tooling Input Lost")
#define ALARM_NOCYCLE_EXTRA_TIMEOUT                 ("External Tooling Input Timeout")
#define ALARM_NOCYCLE_PART_PRESENT                  ("Part Present Input Lost")
#define ALARM_NOCYCLE_SERVO_MOTOR                   ("Actuation Drive Error")
#define ALARM_NOCYCLE_READY_POS                     ("Ready Position Timeout")
#define ALARM_NOCYCLE_RECIPE_NOTVALID               ("Recipe Not Valid")
#define ALARM_NOCYCLE_BATCH_COUNT                   ("Batch Count Complete")
#define ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID         ("Active Recipe not Validated")
#define ALARM_NOCYCLE_PC_ASS_MISMATCH               ("Power Supply Assembly Component Mismatch")
#define ALARM_NOCYCLE_AC_ASS_MISMATCH               ("Actuator Assembly Component Mismatch")
#define ALARM_NOCYCLE_STACK_ASS_MISMATCH            ("Stack Assembly Component Mismatch")

/* No Cycle Alarm Descriptions */
#define ALARM_NOCYCLE_EXTERNAL_SONIC_DESC           ("Trigger Delay has been turned on, but the assigned input did not become inactive within the 30 seconds allowed.")
#define ALARM_NOCYCLE_MISSING_PART_DESC             ("The Missing Part Minimum Distance has not been reached before Trigger occurred or the Maximum Distance has been exceeded before Trigger has occurred.")
#define ALARM_NOCYCLE_PART_CONTACT_DESC             ("The Part Contact Distance has been met before the defined Pretrigger Distance.")
#define ALARM_NOCYCLE_EXTERNAL_CYCLE_DESC           ("The Cycle Abort Digital Input has been activated before trigger occurred.")
#define ALARM_NOCYCLE_ABSOLUTE_DISTANCE_DESC        ("The Part Contact Distance is invalid or not set.")
#define ALARM_NOCYCLE_EXTRA_TOOLING_DESC            ("The External Tooling Input became inactive before Hold Time ended.")
#define ALARM_NOCYCLE_EXTRA_TIMEOUT_DESC            ("The External Tooling Input did not become active within the Tooling Delay Input time after the External Tooling Output became active.")
#define ALARM_NOCYCLE_PART_PRESENT_DESC             ("The Part Present Input became inactive before the end of Hold Time.")
#define ALARM_NOCYCLE_SERVO_MOTOR_DESC              ("The Actuator did not reach the target defined position or Actuation is being prevented.")
#define ALARM_NOCYCLE_READY_POS_DESC                ("The Actuator did not return to the Ready Position within 4 seconds from end of Hold Time.")
#define ALARM_NOCYCLE_RECIPE_NOTVALID_DESC          ("The External Recipe # is not valid through the I/O or Barcode scanner.")
#define ALARM_NOCYCLE_BATCH_COUNT_DESC              ("The target number of welds for this Batch has been met.\n\nNavigate to the Recipe Production Screen to reset the count.")
#define ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_DESC    ("The Operator is attempting to run a recipe that has not been Validated.")
#define ALARM_NOCYCLE_PC_ASS_MISMATCH_DESC          ("The Power Supply Component Name defined in the System Configuration do not match the Name stored with this Recipe.")
#define ALARM_NOCYCLE_AC_ASS_MISMATCH_DESC          ("The Actuator Component Name defined in the System Configuration do not match the Name stored with this Recipe.")
#define ALARM_NOCYCLE_STACK_ASS_MISMATCH_DESC       ("The Ultrasonic Stack Component Name defined in the System Configuration do not match the Name stored with this Recipe.")

/* Hardware failure alarm names */
#define ALARM_HW_START_SWITCH_FAIL                  ("Start Switch Still Active")
#define ALARM_HW_ULS_ACTIVE                         ("ULS Still Active")
#define ALARM_HW_ULS_NONACTIVE                      ("ULS Not Active After Homing")
#define ALARM_HW_GD_BEFORE_TRIGGER                  ("Ground Detect Before Trigger")
#define ALARM_HW_START_SWITCH_LOST                  ("Start Switch Lost")
#define ALARM_HW_ALARM_LOG                          ("Alarm Log Capacity Reached")
#define ALARM_HW_EVENT_LOG                          ("Event Log Capacity Reached")
#define ALARM_HW_WELD_RESULT                        ("Weld Result Capacity Reached")
#define ALARM_HW_WELD_GRAPH                         ("Weld Graph Capacity Reached")
#define ALARM_HW_HORN_SCAN                          ("Horn Scan Graph Capacity Reached")
#define ALARM_HW_PRETRIGGER_TIMEOUT                 ("Pretrigger Timeout")
#define ALARM_HW_ENCODER_FAIL                       ("Encoder Failure")
#define ALARM_HW_DATA_ERROR                         ("Data Error")
#define ALARM_HW_HOME_RETURN_TIMEOUT                ("Actuator Return Timeout")
#define ALARM_HW_ACTUATOR_NOVARM                    ("Actuator Novram")
#define ALARM_HW_PS_NOVARM                          ("P/S Novram")
#define ALARM_HW_START_SWITCH_TIME                  ("Start Switch Time")
#define ALARM_HW_MEM_FULL                           ("Data Storage Full")
#define ALARM_HW_INTERNAL_STORAGE_FAIL				("Internal Failure")
#define ALARM_HW_RECALIBRATE_ACT                    ("Recalibrate Actuator")
#define ALARM_HW_ACT_CLR_FUN                        ("Actuator Clear Function")
#define ALARM_HW_EXTRA_TOOLING_ACTIVE               ("Ext Tooling Active")
#define ALARM_HW_ACT_TYPE_CHANGED                   ("Actuator Type Changed")
#define ALARM_HW_SYS_PRESSURE_INCORRECT             ("System Pressure Incorrect")
#define ALARM_HW_SYS_PART_PRESENT_ACTIVE            ("Part Present Active")
#define ALARM_HW_USB_MEM_LOST                       ("USB Memory Lost")
#define ALARM_CONNECTION_LOST                       ("Connection Lost")
#define ALARM_HW_INTERNAL_BUS_SCAN_FAILURE          ("Internal Bus Scan Failure")
#define ALARM_HW_CABLE_FAIL                         ("Cable Failure")
#define ALARM_HW_PROFINET_OR_IP_NOT                 ("Profinet or Ethernet/IP not responding")
#define ALARM_HW_AC_LINE_LOST                       ("AC Line Voltage Lost")
#define ALARM_HW_TRIGGER_ACTIVE_READY               ("Trigger active in Ready")
#define ALARM_HW_INTERNAL_COMM_FAIL                 ("HMI Connection Lost")
#define ALARM_HW_SC_COMPONENT_FAIL                  ("Internal Component Failure")

/* Hardware failure alarm descriptions */
#define ALARM_HW_START_SWITCH_FAIL_DESC             ("The Start Switches are still active 6 seconds after the end of the Cycle.")
#define ALARM_HW_ULS_ACTIVE_DESC                    ("The ULS has not become inactive after Trigger or Pretrigger has been reached.")
#define ALARM_HW_ULS_NONACTIVE_DESC                 ("The ULS is not active after an Estop or Alarm.")
#define ALARM_HW_GD_BEFORE_TRIGGER_DESC             ("The Ground Detect Input has become active before Trigger occurred.")
#define ALARM_HW_START_SWITCH_LOST_DESC             ("The Start Switches became inactive before Trigger occurred.")
#define ALARM_HW_ALARM_LOG_DESC                     ("Alarm Log Storage is Full.")
#define ALARM_HW_EVENT_LOG_DESC                     ("Event Log Storage is Full.")
#define ALARM_HW_WELD_RESULT_DESC                   ("Weld Result Storage is Full.")
#define ALARM_HW_WELD_GRAPH_DESC                    ("Weld Graph Storage is Full.")
#define ALARM_HW_HORN_SCAN_DESC                     ("Horn Scan Graph Storage is Full.")
#define ALARM_HW_PRETRIGGER_TIMEOUT_DESC            ("Pre-trigger has not occurred within 10 seconds of ULS going inactive.")
#define ALARM_HW_ENCODER_FAIL_DESC                  ("No distance after part contact is made.")
#define ALARM_HW_DATA_ERROR_DESC                    ("Corrupted data in the Recipe checked at power up.")
#define ALARM_HW_HOME_RETURN_TIMEOUT_DESC           ("The carriage has not returned home in 4 seconds.")
#define ALARM_HW_ACTUATOR_NOVARM_DESC               ("The actuator Novram has corrupted data. This is checked at power up.")
#define ALARM_HW_PS_NOVARM_DESC                     ("The power supply Novram has corrupted data. This is checked at power up.")
#define ALARM_HW_START_SWITCH_TIME_DESC             ("Both Start Switches were not pressed within the allowed time frame.")
#define ALARM_HW_MEM_FULL_DESC                      ("Internal Storage Device is full. Any operation which requires Data Storage will not be allowed.")
#define ALARM_HW_INTERNAL_STORAGE_FAIL_DESC			("Contact Branson Service for Assistance.")
#define ALARM_HW_RECALIBRATE_ACT_DESC               ("The force calibration values loaded into the system are invalid.")
#define ALARM_HW_ACT_CLR_FUN_DESC                   ("ULS is active before actuator clear condition was met.")
#define ALARM_HW_EXTRA_TOOLING_ACTIVE_DESC          ("The External Tooling Input has not become inactive for more than 4 seconds after the end of a cycle.")
#define ALARM_HW_ACT_TYPE_CHANGED_DESC              ("The actuator type detected at power up is different from power down or after an ESTOP.")
#define ALARM_HW_SYS_PRESSURE_INCORRECT_DESC        ("The set air pressure is not achieved.")
#define ALARM_HW_SYS_PART_PRESENT_ACTIVE_DESC       ("The Part Present Input is configured and is still active for more than 4 seconds after the end of a cycle.")
#define ALARM_HW_USB_MEM_LOST_DESC                  ("The USB memory stick has been removed or it not functional. Since weld data was configured to be saved on the USB stick welding must be stopped until either the USB stick is functional or weld data is no longer required to be saved.")
#define ALARM_INTERNAL_COMM_FAIL                    ("Communication between the HMI and the welder has been disconnected.")
#define ALARM_HW_ETH_LINK_LOST_DESC                 ("The Ethernet link has been lost between the Supervisor, actuator, and power supply modules.")
#define ALARM_HW_CABLE_FAIL_DESC                    ("If Cable Detect is configured and the pin goes inactive.")
#define ALARM_HW_PROFINET_OR_IP_NOT_DESC            ("No Description available.")
#define ALARM_HW_AC_LINE_LOST_DESC                  ("230V input to the Power Supply is not ON.")
#define ALARM_HW_TRIGGER_ACTIVE_READY_DESC          ("Trigger force is detected in the Ready State.")
#define ALARM_HW_INTERNAL_COMM_FAIL_DESC            ("Internal Communication failure. Contact Branson Service.")
#define ALARM_HW_SC_COMPONENT_FAIL_DESC             ("There is an internal failure. Contact Branson Service")

/* Cycle modified alarm names */
#define ALARM_CYCLE_GROUND_DETECT_ABORT             ("Ground Detect Abort")
#define ALARM_CYCLE_MAXTIMEOUT                      ("Max Weld Time Exceeded")
#define ALARM_CYCLE_NOFORCESTEP                     ("No Force Step")
#define ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE        ("Trigger Force greater than End Force")
#define ALARM_CYCLE_LL_NOT_REACHED                  ("LL Not reached")
#define ALARM_CYCLE_PEAKPOWER_CUTOFF                ("Peak Power Cutoff")
#define ALARM_CYCLE_ABSOLUTE_CUTOFF                 ("Absolute Distance Cutoff")
#define ALARM_CYCLE_COLLAPSE_CUTOFF                 ("Collapse Distance Cutoff")
#define ALARM_CYCLE_ULTRASONICS_DISABLED            ("Ultrasonics disabled")
#define ALARM_CYCLE_CUSTOM1_CUTOFF                  ("Custom Input 1 Cutoff")
#define ALARM_CYCLE_CUSTOM2_CUTOFF                  ("Custom Input 2 Cutoff")
#define ALARM_CYCLE_FREQLOW_CUTOFF                  ("Frequency Low Cutoff")
#define ALARM_CYCLE_FREQHIGH_CUTOFF                 ("Frequency High Cutoff")
#define ALARM_CYCLE_CUST_DIGITAL_CUTOFF             ("Custom Digital Cutoff")
#define ALARM_CYCLE_ENERGY_CUTOFF                   ("Energy Cutoff")
#define ALARM_CYCLE_GD_CUTOFF                       ("Ground Detect Cutoff")
#define ALARM_CYCLE_TIME_CUTOFF                     ("Time Cutoff")
#define ALARM_CYCLE_NO_PRESSURE_STEP                ("No Pressure Step")

/* Cycle modified alarm descriptions */
#define ALARM_CYCLE_GROUND_DETECT_ABORT_DESC        ("The Ground Detect Input has become active and the Cycle has been aborted.")
#define ALARM_CYCLE_MAXTIMEOUT_DESC                 ("The maximum time allowed for Ultrasonic energy to be applied to the part has been reached.")
#define ALARM_CYCLE_NOFORCESTEP_DESC                ("The force step trigger was not reached.")
#define ALARM_CYCLE_NO_PRESSURE_STEP_DESC           ("The pressure step trigger was not reached.") // Not in E1
#define ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_DESC	("The force at the end of the cycle was greater than the trigger force.") // Not in E1
#define ALARM_CYCLE_LL_NOT_REACHED_ID_DESC			("No Description available")	// Not in E1
#define ALARM_CYCLE_PEAKPOWER_CUTOFF_DESC			("The Peak Power Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_ABSOLUTE_CUTOFF_DESC			("The Absolute Distance Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_COLLAPSE_CUTOFF_DESC			("The Collapse Distance Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_ULTRASONICS_DISABLED_DESC		("The Ultrasonics Disable user input has been enabled during this cycle.")
#define ALARM_CYCLE_CUSTOM1_CUTOFF_DESC				("No Description available")	// Not in E1
#define ALARM_CYCLE_CUSTOM2_CUTOFF_DESC				("No Description available") 	// Not in E1
#define ALARM_CYCLE_FREQLOW_CUTOFF_DESC				("The Frequency Low Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_FREQHIGH_CUTOFF_DESC			("The Frequency High Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_CUST_DIGITAL_CUTOFF_DESC		("No Description available") // Not in E1
#define ALARM_CYCLE_ENERGY_CUTOFF_DESC				("The Energy Cutoff value has been exceeded during the weld.")
#define ALARM_CYCLE_GD_CUTOFF_DESC					("The Ground Detect Cutoff has triggered during the weld.")
#define ALARM_CYCLE_TIME_CUTOFF_DESC				("The Time Cutoff value has been exceeded during the weld.")

/* Suspect Alarms */
#define ALARM_SUSPECT_ENERGY_LIMIT                  ("Energy Suspect Limit")
#define ALARM_SUSPECT_POWER_LIMIT                   ("Peak Power Suspect Limit")
#define ALARM_SUSPECT_COLLAPSE_LIMIT                ("Collapse Distance Suspect Limit")
#define ALARM_SUSPECT_ABSOLUTE_LIMIT                ("Absolute Distance Suspect limit")
#define ALARM_SUSPECT_TRIGGER_LIMIT                 ("Trigger Distance Suspect Limit")
#define ALARM_SUSPECT_WELD_FORCE_LIMIT              ("Weld Force Suspect  Limit")
#define ALARM_SUSPECT_TIME_LIMIT                    ("Time Suspect Limit")
#define ALARM_SUSPECT_VELOCITY_LIMITS               ("Velocity Suspect Limit")
#define ALARM_SUSPECT_FREQ_LIMITS                   ("Frequency Suspect Limit")

#define ALARM_LOW_ENERGY_LIMIT_DESC                 ("The total Weld Energy did not exceed the lower limit value")
#define ALARM_HIGH_ENERGY_LIMIT_DESC                ("The total Weld Energy exceeded the upper limit value")
#define ALARM_LOW_POWER_LIMIT_DESC                  ("The Peak Power did not exceed the lower limit value")
#define ALARM_HIGH_POWER_LIMIT_DESC                 ("The Peak Power exceeded the upper limit value")
#define ALARM_LOW_COLLAPSE_LIMIT_DESC               ("The total Collapse Distance did not exceed the lower limit value")
#define ALARM_HIGH_COLLAPSE_LIMIT_DESC              ("The total Collapse Distance exceeded the upper limit value")
#define ALARM_LOW_ABSOLUTE_LIMIT_DESC               ("The total Absolute Distance did not exceed the lower limit value")
#define ALARM_HIGH_ABSOLUTE_LIMIT_DESC              ("The total Absolute Distance exceeded the upper limit value")
#define ALARM_LOW_TRIGGER_LIMIT_DESC                ("The Trigger Distance did not exceed the lower limit value")
#define ALARM_HIGH_TRIGGER_LIMIT_DESC               ("The Trigger Distance exceeded the upper limit value")
#define ALARM_LOW_WELD_FORCE_LIMIT_DESC             ("The total Weld Force did not exceed the lower limit value")
#define ALARM_HIGH_WELD_FORCE_LIMIT_DESC            ("The total Weld Force exceeded the upper limit value")
#define ALARM_LOW_TIME_LIMIT_DESC                   ("The total Weld Time did not exceed the lower limit value")
#define ALARM_HIGH_TIME_LIMIT_DESC                  ("The total Weld Time exceeded the upper limit value")
#define ALARM_LOW_VELOCITY_LIMITS_DESC              ("The Weld Velocity did not exceed the lower limit value")
#define ALARM_HIGH_VELOCITY_LIMITS_DESC             ("The Weld Velocity exceeded the upper limit value")
#define ALARM_LOW_FREQ_LIMITS_DESC                  ("The Weld Frequency did not exceed the lower limit value")
#define ALARM_HIGH_FREQ_LIMITS_DESC                 ("The Weld Velocity exceeded the upper limit value")

/* Reject Alarms */
#define ALARM_REJECT_ENERGY_LIMIT                   ("Energy Reject Limit")
#define ALARM_REJECT_POWER_LIMIT                    ("Peak Power Reject Limit")
#define ALARM_REJECT_COLLAPSE_LIMIT                 ("Collapse Distance Reject Limit")
#define ALARM_REJECT_ABSOLUTE_LIMIT                 ("Absolute Distance Reject limit")
#define ALARM_REJECT_TRIGGER_LIMIT                  ("Trigger Reject Limit")
#define ALARM_REJECT_WELD_FORCE_LIMIT               ("Weld Force Reject Limit")
#define ALARM_REJECT_TIME_LIMIT                     ("Time Reject Limit")
#define ALARM_REJECT_VELOCITY_LIMITS                ("Velocity Reject Limit")
#define ALARM_REJECT_FREQ_LIMITS                    ("Frequency Reject Limit")

#define ALARM_POWER_MATCH_CURVE_LIMITS				("Power Match Curve Limits")

/* Warnings */
#define ALARM_WARNING_TRIGGERLOST                   ("Trigger Force lost in Weld")
#define ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD         ("Actuator clear not reached")
#define ALARM_WARNING_ACTU_RECAL                    ("Actuator Recal suggested")
#define ALARM_WARNING_USBMEMORY                     ("USB Memory Nearly Full(80%)")
#define ALARM_WARNING_DISK_MEMORY                   ("Internal Storage Capacity Warning")
#define ALARM_WARNING_ALARM_LOG                     ("Alarm Log Capacity Warning")
#define ALARM_WARNING_EVENT_LOG                     ("Event Log Capacity Warning")
#define ALARM_WARNING_WELD_RESULT                   ("Weld Result Capacity Warning")
#define ALARM_WARNING_WELD_GRAPH                    ("Weld Graph Capacity Warning")
#define ALARM_WARNING_HORN_SCAN                     ("Horn Scan Graph Capacity Warning")
#define ALARM_WARNING_EEPROM_CORRUPT                ("Possible Data Error")
#define ALARM_WARNING_RECIPE_CORRUPTED				("Possible Recipe Record Corruption")

#define ALARM_WARNING_TRIGGERLOST_DESC              ("The applied Force dropped below the minimum Trigger Force during the Cycle.")
#define ALARM_WARNING_DISK_MEMORY_DESC              ("Internal Storage is greater than 80% Full. Please consider extracting data to USB to avoid loss of data.")
#define ALARM_WARNING_ALARM_LOG_DESC                ("Alarm Log Storage is greater than 80% Full.")
#define ALARM_WARNING_EVENT_LOG_DESC                ("Event Log Storage is greater than 80% Full.")
#define ALARM_WARNING_WELD_RESULT_DESC              ("Weld Result Storage is greater than 80% Full.")
#define ALARM_WARNING_WELD_GRAPH_DESC               ("Weld Graph Storage is greater than 80% Full.")
#define ALARM_WARNING_HORN_SCAN_DESC                ("Horn Scan Graph Storage is greater than 80% Full.")
#define ALARM_WARNING_EEPROM_CORRUPT_DESC			("System Configuration potentially incorrect. Double check the System Information")

/* Overloads */
#define ALARM_PHASE_OVERLOAD						("Phase Overload")
#define ALARM_CURR_OVERLOAD							("Current Overload")
#define ALARM_FREQ_OVERLOAD							("Frequency Overload")
#define ALARM_POWER_OVERLOAD						("Power Overload")
#define ALARM_VOLT_OVERLOAD							("Voltage Overload")
#define ALARM_TEMP_OVERLOAD							("Temperature Overload")

#define ALARM_PHASE_OVERLOAD_DESC					("Phase Overload occurred during the weld")
#define ALARM_CURR_OVERLOAD_DESC					("Current Overload occurred during the weld")
#define ALARM_FREQ_OVERLOAD_DESC					("Frequency Overload occurred during the weld")
#define ALARM_POWER_OVERLOAD_DESC					("Power Overload occurred during the weld")
#define ALARM_VOLT_OVERLOAD_DESC					("Voltage Overload occurred during the weld")
#define ALARM_TEMP_OVERLOAD_DESC					("Temperature Overload occurred during the weld")
#define ALARM_EB_PHASE_OVERLOAD_DESC				("Phase Overload occurred during Energy Braking")
#define ALARM_EB_CURR_OVERLOAD_DESC					("Current Overload occurred during Energy Braking")
#define ALARM_EB_FREQ_OVERLOAD_DESC					("Frequency Overload occurred during Energy Braking")
#define ALARM_EB_POWER_OVERLOAD_DESC				("Power Overload occurred during Energy Braking")
#define ALARM_EB_VOLT_OVERLOAD_DESC					("Voltage Overload occurred during Energy Braking")
#define ALARM_EB_TEMP_OVERLOAD_DESC					("Temperature Overload occurred during Energy Braking")
#define ALARM_AB_PHASE_OVERLOAD_DESC				("Phase Overload occurred during Afterburst")
#define ALARM_AB_CURR_OVERLOAD_DESC					("Current Overload occurred during Afterburst")
#define ALARM_AB_FREQ_OVERLOAD_DESC					("Frequency Overload occurred during Afterburst")
#define ALARM_AB_POWER_OVERLOAD_DESC				("Power Overload occurred during Afterburst")
#define ALARM_AB_VOLT_OVERLOAD_DESC					("Voltage Overload occurred during Afterburst")
#define ALARM_AB_TEMP_OVERLOAD_DESC					("Temperature Overload occurred during Afterburst")
#define ALARM_PRETRIG_PHASE_OVERLOAD_DESC			("Phase Overload occurred during Pretrigger")
#define ALARM_PRETRIG_CURR_OVERLOAD_DESC			("Current Overload occurred during Pretrigger")
#define ALARM_PRETRIG_FREQ_OVERLOAD_DESC			("Frequency Overload occurred during Pretrigger")
#define ALARM_PRETRIG_POWER_OVERLOAD_DESC			("Power Overload occurred during Pretrigger")
#define ALARM_PRETRIG_VOLT_OVERLOAD_DESC			("Voltage Overload occurred during Pretrigger")
#define ALARM_PRETRIG_TEMP_OVERLOAD_DESC			("Temperature Overload occurred during Pretrigger")
#define ALARM_PRE_SEEK_PHASE_OVERLOAD_DESC			("Phase Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_CURR_OVERLOAD_DESC			("Current Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_FREQ_OVERLOAD_DESC			("Frequency Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_POWER_OVERLOAD_DESC			("Power Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_VOLT_OVERLOAD_DESC			("Voltage Overload occurred during Pre Weld Seek")
#define ALARM_PRE_SEEK_TEMP_OVERLOAD_DESC			("Temperature Overload occurred during Pre Weld Seek")
#define ALARM_POST_SEEK_PHASE_OVERLOAD_DESC			("Phase Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_CURR_OVERLOAD_DESC			("Current Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_FREQ_OVERLOAD_DESC			("Frequency Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_POWER_OVERLOAD_DESC			("Power Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_VOLT_OVERLOAD_DESC			("Voltage Overload occurred during Post Weld Seek")
#define ALARM_POST_SEEK_TEMP_OVERLOAD_DESC			("Temperature Overload occurred during Post Weld Seek")
#define ALARM_SEEK_PHASE_OVERLOAD_DESC				("Phase Overload occurred during Seek")
#define ALARM_SEEK_CURR_OVERLOAD_DESC				("Current Overload occurred during Seek")
#define ALARM_SEEK_FREQ_OVERLOAD_DESC				("Frequency Overload occurred during Seek")
#define ALARM_SEEK_POWER_OVERLOAD_DESC				("Power Overload occurred during Seek")
#define ALARM_SEEK_VOLT_OVERLOAD_DESC				("Voltage Overload occurred during Seek")
#define ALARM_SEEK_TEMP_OVERLOAD_DESC				("Temperature Overload occurred during Seek")
#define ALARM_TEST_PHASE_OVERLOAD_DESC				("Phase Overload occurred during Test")
#define ALARM_TEST_CURR_OVERLOAD_DESC				("Current Overload occurred during Test")
#define ALARM_TEST_FREQ_OVERLOAD_DESC				("Frequency Overload occurred during Test")
#define ALARM_TEST_POWER_OVERLOAD_DESC				("Power Overload occurred during Test")
#define ALARM_TEST_VOLT_OVERLOAD_DESC				("Voltage Overload occurred during Test")
#define ALARM_TEST_TEMP_OVERLOAD_DESC				("Temperature Overload occurred during Test")

/*  No Cycle Overloads - Overloads that happen before trigger or outside the weld cycle */
#define ALARM_SEEK_OVERLOAD                         ("Seek Overload")
#define ALARM_TEST_OVERLOAD                         ("Test Overload")
#define ALARM_PRETRIGGER_OVERLOAD                   ("Pretrigger Overload")
#define ALARM_PREWELDSEEK_OVERLOAD                  ("Pre-Weld Seek")
#define ALARM_THERMAL_OVERLOAD                      ("Thermal Overload")
#define ALARM_THERMAL_OVERLOAD_DESC                 ("Power supply temperature above the maximum operating temperature.")

/* EN Faults */
#define ALARM_MULTIPLE_FAULTS                       ("Multiple Faults")
#define ALARM_MULTIPLE_FAULTS_DESC                  ("Start Switches activated when system not Ready")
#define ALARM_PALM_BUTTON_FAULT                     ("Palm Button Fault")
#define ALARM_24V_FAULT                             ("24V Fault")
#define ALARM_ESTOP_FAULT                           ("E-Stop Fault")
#define ALARM_LINEAR_ENCODER_FAULT                  ("Linear Encoder Fault ")
#define ALARM_LINEAR_ENCODER_FAULT_DESC             ("No Description available")
#define ALARM_S_BEAM_FAULT                          ("S-Beam Fault")
#define ALARM_TRIGGER_SWITCH_FAULT                  ("Trigger Switch Fault")
#define ALARM_DRIVE_FAULT                           ("Drive Fault")
#define ALARM_CROSS_MONITORING_FAULT                ("Cross Monitoring Fault")
#define ALARM_LOGIC_FAULT                           ("Logic Unit Fault ")
#define ALARM_SONICS_ENABLE_FAULT                   ("Sonics Enable Fault")

// As per the TOM's comment in the Bug:2676, Unknown alarm should be replaced with empty
#define ALARM_UNKNOW_ALARM                          ("")
#define ALARM_NO_DESC                               ("No Description available")

#define NOT_AVAIABLE                                ("NA")


class AlarmNames
{
public:
	static std::string errorNameforAlarm(const char *);
};

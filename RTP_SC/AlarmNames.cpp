#include "AlarmNames.h"
#include <iostream>
#include <stdlib.h>

std::string AlarmNames::errorNameforAlarm(const char * alarmId)
{    
	std::string errorName;
    int hex = std::strtol(alarmId, 0, 16);
    
    switch (hex)
    {
    case ALARM_NOCYCLE_EXTERNAL_SONIC_ID:
        errorName = ALARM_NOCYCLE_EXTERNAL_SONIC;
        break;
    case ALARM_NOCYCLE_MISSING_PART_ID:
        errorName = ALARM_NOCYCLE_MISSING_PART;
        break;
    case ALARM_NOCYCLE_PART_CONTACT_ID:
        errorName = ALARM_NOCYCLE_PART_CONTACT;
        break;
    case ALARM_NOCYCLE_EXTERNAL_CYCLE_ID:
        errorName = ALARM_NOCYCLE_EXTERNAL_CYCLE;
        break;
    case ALARM_NOCYCLE_ABSOLUTE_DISTANCE_ID:
        errorName = ALARM_NOCYCLE_ABSOLUTE_DISTANCE;
        break;
    case ALARM_NOCYCLE_EXTRA_TOOLING_ID:
        errorName = ALARM_NOCYCLE_EXTRA_TOOLING;
        break;
    case ALARM_NOCYCLE_EXTRA_TIMEOUT_ID:
        errorName = ALARM_NOCYCLE_EXTRA_TIMEOUT;
        break;
    case ALARM_NOCYCLE_PART_PRESENT_ID:
        errorName = ALARM_NOCYCLE_PART_PRESENT;
        break;
    case ALARM_NOCYCLE_SERVO_MOTOR_ID:
        errorName = ALARM_NOCYCLE_SERVO_MOTOR;
        break;
    case ALARM_NOCYCLE_READY_POS_ID:
        errorName = ALARM_NOCYCLE_READY_POS;
        break;
    case ALARM_NOCYCLE_RECIPE_NOTVALID_ID:
        errorName = ALARM_NOCYCLE_RECIPE_NOTVALID;
        break;
    case ALARM_NOCYCLE_BATCH_COUNT_ID:
        errorName = ALARM_NOCYCLE_BATCH_COUNT;
        break;
    case ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID_ID:
        errorName = ALARM_NOCYCLE_ACTIVERECIPE_NOTVALID;
        break;
    case ALARM_NOCYCLE_PC_ASS_MISMATCH_ID:
        errorName = ALARM_NOCYCLE_PC_ASS_MISMATCH;
        break;
    case ALARM_NOCYCLE_AC_ASS_MISMATCH_ID:
        errorName = ALARM_NOCYCLE_AC_ASS_MISMATCH;
        break;
    case ALARM_NOCYCLE_STACK_ASS_MISMATCH_ID:
        errorName = ALARM_NOCYCLE_STACK_ASS_MISMATCH;
        break;
    case ALARM_HW_START_SWITCH_FAIL_ID:
        errorName = ALARM_HW_START_SWITCH_FAIL;
        break;
    case ALARM_HW_ULS_ACTIVE_ID:
        errorName = ALARM_HW_ULS_ACTIVE;
        break;
    case ALARM_HW_ULS_NONACTIVE_ID:
        errorName = ALARM_HW_ULS_NONACTIVE;
        break;
    case ALARM_HW_GD_BEFORE_TRIGGER_ID:
        errorName = ALARM_HW_GD_BEFORE_TRIGGER;
        break;
    case ALARM_HW_START_SWITCH_LOST_ID:
        errorName = ALARM_HW_START_SWITCH_LOST;
        break;
    case ALARM_HW_ALARM_LOG_ID:
        errorName = ALARM_HW_ALARM_LOG;
        break;
    case ALARM_HW_EVENT_LOG_ID:
        errorName = ALARM_HW_EVENT_LOG;
        break;
    case ALARM_HW_WELD_RESULT_ID:
        errorName = ALARM_HW_WELD_RESULT;
        break;
    case ALARM_HW_WELD_GRAPH_ID:
        errorName = ALARM_HW_WELD_GRAPH;
        break;
    case ALARM_HW_HORN_SCAN_ID:
        errorName = ALARM_HW_HORN_SCAN;
        break;
    case ALARM_HW_PRETRIGGER_TIMEOUT_ID:
        errorName = ALARM_HW_PRETRIGGER_TIMEOUT;
        break;
    case ALARM_HW_ENCODER_FAIL_ID:
        errorName = ALARM_HW_ENCODER_FAIL;
        break;
    case ALARM_HW_DATA_ERROR_ID:
        errorName = ALARM_HW_DATA_ERROR;
        break;
    case ALARM_HW_HOME_RETURN_TIMEOUT_ID:
        errorName = ALARM_HW_HOME_RETURN_TIMEOUT;
        break;
    case ALARM_HW_ACTUATOR_NOVARM_ID:
        errorName = ALARM_HW_ACTUATOR_NOVARM;
        break;
    case ALARM_HW_PS_NOVARM_ID:
        errorName = ALARM_HW_PS_NOVARM;
        break;
    case ALARM_HW_START_SWITCH_TIME_ID:
        errorName = ALARM_HW_START_SWITCH_TIME;
        break;
    case ALARM_HW_MEM_FULL_ID:
        errorName = ALARM_HW_MEM_FULL;
        break;
    case ALARM_HW_INTERNAL_STORAGE_FAIL_ID:
		errorName = ALARM_HW_INTERNAL_STORAGE_FAIL;
		break;
    case ALARM_HW_RECALIBRATE_ACT_ID:
        errorName = ALARM_HW_RECALIBRATE_ACT;
        break;
    case ALARM_HW_ACT_CLR_FUN_ID:
        errorName = ALARM_HW_ACT_CLR_FUN;
        break;
    case ALARM_HW_EXTRA_TOOLING_ACTIVE_ID:
        errorName = ALARM_HW_EXTRA_TOOLING_ACTIVE;
        break;
    case ALARM_HW_ACT_TYPE_CHANGED_ID:
        errorName = ALARM_HW_ACT_TYPE_CHANGED;
        break;
    case ALARM_HW_SYS_PRESSURE_INCORRECT_ID:
        errorName = ALARM_HW_SYS_PRESSURE_INCORRECT;
        break;
    case ALARM_HW_SYS_PART_PRESENT_ACTIVE_ID:
        errorName = ALARM_HW_SYS_PART_PRESENT_ACTIVE;
        break;
    case ALARM_HW_USB_MEM_LOST_ID:
        errorName = ALARM_HW_USB_MEM_LOST;
        break;
    case ALARM_INTERNAL_COMM_ENET:
        errorName = ALARM_CONNECTION_LOST;
        break;
    case ALARM_HW_INTERNAL_BUS_SCAN_FAILURE_ID:
        errorName = ALARM_HW_INTERNAL_BUS_SCAN_FAILURE;
        break;
    case ALARM_HW_CABLE_FAIL_ID:
        errorName = ALARM_HW_CABLE_FAIL;
        break;
    case ALARM_HW_PROFINET_OR_IP_NOT_ID:
        errorName = ALARM_HW_PROFINET_OR_IP_NOT;
        break;
    case ALARM_HW_AC_LINE_LOST_ID:
        errorName = ALARM_HW_AC_LINE_LOST;
        break;
    case ALARM_HW_TRIGGER_ACTIVE_READY_ID:
        errorName = ALARM_HW_TRIGGER_ACTIVE_READY;
        break;
    case ALARM_HW_INTERNAL_COMM_FAIL_ID:
        errorName = ALARM_HW_INTERNAL_COMM_FAIL;
        break;
    case ALARM_HW_SC_COMPONENT_FAIL_ID:
        errorName = ALARM_HW_SC_COMPONENT_FAIL;
        break;
    case ALARM_CYCLE_GROUND_DETECT_ABORT_ID:
        errorName = ALARM_CYCLE_GROUND_DETECT_ABORT;
        break;
    case ALARM_CYCLE_MAXTIMEOUT_ID:
        errorName = ALARM_CYCLE_MAXTIMEOUT;
        break;
    case ALARM_CYCLE_NOFORCESTEP_ID:
        errorName = ALARM_CYCLE_NOFORCESTEP;
        break;
    case ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE_ID:
        errorName = ALARM_CYCLE_TRIGGER_GREATER_ENDFORCE;
        break;
    case ALARM_CYCLE_LL_NOT_REACHED_ID:
        errorName = ALARM_CYCLE_LL_NOT_REACHED;
        break;
    case ALARM_CYCLE_PEAKPOWER_CUTOFF_ID:
        errorName = ALARM_CYCLE_PEAKPOWER_CUTOFF;
        break;
    case ALARM_CYCLE_ABSOLUTE_CUTOFF_ID:
        errorName = ALARM_CYCLE_ABSOLUTE_CUTOFF;
        break;
    case ALARM_CYCLE_COLLAPSE_CUTOFF_ID:
        errorName = ALARM_CYCLE_COLLAPSE_CUTOFF;
        break;
    case ALARM_CYCLE_ULTRASONICS_DISABLED_ID:
        errorName = ALARM_CYCLE_ULTRASONICS_DISABLED;
        break;
    case ALARM_CYCLE_CUSTOM1_CUTOFF_ID:		// Not in E1
        errorName = ALARM_CYCLE_CUSTOM1_CUTOFF;
        break;
    case ALARM_CYCLE_CUSTOM2_CUTOFF_ID:		// Not in E1
        errorName = ALARM_CYCLE_CUSTOM2_CUTOFF;
        break;
    case ALARM_CYCLE_FREQLOW_CUTOFF_ID:
        errorName = ALARM_CYCLE_FREQLOW_CUTOFF;
        break;
    case ALARM_CYCLE_FREQHIGH_CUTOFF_ID:
        errorName = ALARM_CYCLE_FREQHIGH_CUTOFF;
        break;
    case ALARM_CYCLE_CUST_DIGITAL_CUTOFF_ID:
        errorName = ALARM_CYCLE_CUST_DIGITAL_CUTOFF;
        break;
    case ALARM_CYCLE_ENERGY_CUTOFF_ID:
        errorName = ALARM_CYCLE_ENERGY_CUTOFF;
        break;
    case ALARM_CYCLE_GD_CUTOFF_ID:
        errorName = ALARM_CYCLE_GD_CUTOFF;
        break;
    case ALARM_CYCLE_TIME_CUTOFF_ID:
        errorName = ALARM_CYCLE_TIME_CUTOFF;
        break;
    case ALARM_CYCLE_NO_PRESSURE_STEP_ID:
        errorName = ALARM_CYCLE_NO_PRESSURE_STEP;
        break;
    case ALARM_ENERGY_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_ENERGY_LIMIT;
        break;
    case ALARM_ENERGY_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_ENERGY_LIMIT;
        break;
    case ALARM_POWER_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_POWER_LIMIT;
        break;
    case ALARM_POWER_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_POWER_LIMIT;
        break;
    case ALARM_COLLAPSE_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_COLLAPSE_LIMIT;
        break;
    case ALARM_COLLAPSE_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_COLLAPSE_LIMIT;
        break;
    case ALARM_ABSOLUTE_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_ABSOLUTE_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_TRIGGER_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_TRIGGER_LIMIT;
        break;
    case ALARM_TRIGGER_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_TRIGGER_LIMIT;
        break;
    case ALARM_WELD_FORCE_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_WELD_FORCE_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_TIME_SLLIMIT_ID:
        errorName = ALARM_SUSPECT_TIME_LIMIT;
        break;
    case ALARM_TIME_SHLIMIT_ID:
        errorName = ALARM_SUSPECT_TIME_LIMIT;
        break;
    case ALARM_VELOCITY_SLLIMITS_ID:
        errorName = ALARM_SUSPECT_VELOCITY_LIMITS;
        break;
    case ALARM_VELOCITY_SHLIMITS_ID:
        errorName = ALARM_SUSPECT_VELOCITY_LIMITS;
        break;
    case ALARM_FREQ_SLLIMITS_ID:
        errorName = ALARM_SUSPECT_FREQ_LIMITS;
        break;
    case ALARM_FREQ_SHLIMITS_ID:
        errorName = ALARM_SUSPECT_FREQ_LIMITS;
        break;
    case ALARM_ENERGY_RLLIMIT_ID:
        errorName = ALARM_REJECT_ENERGY_LIMIT;
        break;
    case ALARM_ENERGY_RHLIMIT_ID:
        errorName = ALARM_REJECT_ENERGY_LIMIT;
        break;
    case ALARM_POWER_RLLIMIT_ID:
        errorName = ALARM_REJECT_POWER_LIMIT;
        break;
    case ALARM_POWER_RHLIMIT_ID:
        errorName = ALARM_REJECT_POWER_LIMIT;
        break;
    case ALARM_COLLAPSE_RLLIMIT_ID:
        errorName = ALARM_REJECT_COLLAPSE_LIMIT;
        break;
    case ALARM_COLLAPSE_RHLIMIT_ID:
        errorName = ALARM_REJECT_COLLAPSE_LIMIT;
        break;
    case ALARM_ABSOLUTE_RLLIMIT_ID:
        errorName = ALARM_REJECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_ABSOLUTE_RHLIMIT_ID:
        errorName = ALARM_REJECT_ABSOLUTE_LIMIT;
        break;
    case ALARM_TRIGGER_RLLIMIT_ID:
        errorName = ALARM_REJECT_TRIGGER_LIMIT;
        break;
    case ALARM_TRIGGER_RHLIMIT_ID:
        errorName = ALARM_REJECT_TRIGGER_LIMIT;
        break;
    case ALARM_WELD_FORCE_RLLIMIT_ID:
        errorName = ALARM_REJECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_WELD_FORCE_RHLIMIT_ID:
        errorName = ALARM_REJECT_WELD_FORCE_LIMIT;
        break;
    case ALARM_TIME_RLLIMIT_ID:
        errorName = ALARM_REJECT_TIME_LIMIT;
        break;
    case ALARM_TIME_RHLIMIT_ID:
        errorName = ALARM_REJECT_TIME_LIMIT;
        break;
    case ALARM_FREQ_RLLIMITS_ID:
        errorName = ALARM_REJECT_FREQ_LIMITS;
        break;
    case ALARM_FREQ_RHLIMITS_ID:
        errorName = ALARM_REJECT_FREQ_LIMITS;
        break;
    case ALARM_POWER_MATCH_CURVE_RLLIMITS_ID:
        errorName = ALARM_POWER_MATCH_CURVE_LIMITS;
        break;
    case ALARM_POWER_MATCH_CURVE_RHLIMITS_ID:
        errorName = ALARM_POWER_MATCH_CURVE_LIMITS;
        break;
    case ALARM_WARNING_TRIGGERLOST_ID:
        errorName = ALARM_WARNING_TRIGGERLOST;
        break;
    case ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD_ID:
        errorName = ALARM_WARNING_ACTUATOR_CLR_NOT_RCHD;
        break;
    case ALARM_WARNING_ACTU_RECAL_ID:
        errorName = ALARM_WARNING_ACTU_RECAL;
        break;
    case ALARM_WARNING_USBMEMORY_ID:
        errorName = ALARM_WARNING_USBMEMORY;
        break;
    case ALARM_WARNING_DISK_MEMORY_ID:
        errorName = ALARM_WARNING_DISK_MEMORY;
        break;
    case ALARM_WARNING_ALARM_LOG_ID:
        errorName = ALARM_WARNING_ALARM_LOG;
        break;
    case ALARM_WARNING_EVENT_LOG_ID:
        errorName = ALARM_WARNING_EVENT_LOG;
        break;
    case ALARM_WARNING_WELD_RESULT_ID:
        errorName = ALARM_WARNING_WELD_RESULT;
        break;
    case ALARM_WARNING_WELD_GRAPH_ID:
        errorName = ALARM_WARNING_WELD_GRAPH;
        break;
    case ALARM_WARNING_HORN_SCAN_ID:
        errorName = ALARM_WARNING_HORN_SCAN;
        break;
    case ALARM_WARNING_EEPROM_CORRUPT_ID:
        errorName = ALARM_WARNING_EEPROM_CORRUPT; //**warning
        break;
    case ALARM_RECIPE_WELD_CORRUPTED_ID:
    case ALARM_RECIPE_LIMITS_CORRUPTED_ID:
    case ALARM_RECIPE_SETUP_CORRUPTED_ID:
    case ALARM_RECIPE_STACK_CORRUPTED_ID:
        errorName = ALARM_WARNING_RECIPE_CORRUPTED; //**warning
        break;
    case ALARM_PHASE_OVERLOAD_ID:					//** Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
    	break;
    case ALARM_CURR_OVERLOAD_ID:
    	errorName = ALARM_CURR_OVERLOAD;
        break;
    case ALARM_FREQ_OVERLOAD_ID:
    	errorName = ALARM_FREQ_OVERLOAD;
        break;
    case ALARM_POWER_OVERLOAD_ID:
    	errorName = ALARM_POWER_OVERLOAD;
        break;
    case ALARM_VOLT_OVERLOAD_ID:
    	errorName = ALARM_VOLT_OVERLOAD;
        break;
    case ALARM_TEMP_OVERLOAD_ID:
    	errorName = ALARM_TEMP_OVERLOAD;
        break;
    case ALARM_EB_PHASE_OVERLOAD_ID:					//** Energy Brake Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_EB_CURR_OVERLOAD_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_EB_FREQ_OVERLOAD_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_EB_POWER_OVERLOAD_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_EB_VOLT_OVERLOAD_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_EB_TEMP_OVERLOAD_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_AB_PHASE_OVERLOAD_ID:					//** Afterburst Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_AB_CURR_OVERLOAD_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_AB_FREQ_OVERLOAD_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_AB_POWER_OVERLOAD_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_AB_VOLT_OVERLOAD_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_AB_TEMP_OVERLOAD_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_POST_SEEK_PHASE_OVERLOAD_ID:					//** Post Weld Seek Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_POST_SEEK_CURR_OVERLOAD_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_POST_SEEK_FREQ_OVERLOAD_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_POST_SEEK_POWER_OVERLOAD_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_POST_SEEK_VOLT_OVERLOAD_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_POST_SEEK_TEMP_OVERLOAD_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_SEEK_OVERLOAD_PHASE_ID:					//** Seek Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_CURRENT_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_FREQUENCY_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_POWER_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_VOLTAGE_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_SEEK_OVERLOAD_TEMPERATURE_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_TEST_OVERLOAD_PHASE_ID:					//** Test Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_CURRENT_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_FREQUENCY_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_POWER_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_VOLTAGE_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_TEST_OVERLOAD_TEMPERATURE_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_PRETRIGGER_OVERLOAD_PHASE_ID:					//** Pretrigger Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_CURRENT_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_FREQUENCY_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_POWER_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_VOLTAGE_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_PRETRIGGER_OVERLOAD_TEMPERATURE_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_PRE_SEEK_PHASE_OVERLOAD_ID:					//** Pre Weld Seek Overloads
    	errorName = ALARM_PHASE_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_CURR_OVERLOAD_ID:
		errorName = ALARM_CURR_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_FREQ_OVERLOAD_ID:
		errorName = ALARM_FREQ_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_POWER_OVERLOAD_ID:
		errorName = ALARM_POWER_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_VOLT_OVERLOAD_ID:
		errorName = ALARM_VOLT_OVERLOAD;
		break;
	case ALARM_PRE_SEEK_TEMP_OVERLOAD_ID:
		errorName = ALARM_TEMP_OVERLOAD;
		break;
    case ALARM_MULTIPLE_FAULTS_ID:
        errorName = ALARM_MULTIPLE_FAULTS;
        break;
    case ALARM_PALM_BUTTON_FAULT_ID:
        errorName = ALARM_PALM_BUTTON_FAULT;
        break;
    case ALARM_24V_FAULT_ID:
        errorName = ALARM_24V_FAULT;
        break;
    case ALARM_ESTOP_FAULT_ID:
        errorName = ALARM_ESTOP_FAULT;
        break;
    case ALARM_LINEAR_ENCODER_FAULT_ID:
        errorName = ALARM_LINEAR_ENCODER_FAULT;
        break;
    case ALARM_S_BEAM_FAULT_ID:
        errorName = ALARM_S_BEAM_FAULT;
        break;
    case ALARM_TRIGGER_SWITCH_FAULT_ID:
        errorName = ALARM_TRIGGER_SWITCH_FAULT;
        break;
    case ALARM_DRIVE_FAULT_ID:
        errorName = ALARM_DRIVE_FAULT;
        break;
    case ALARM_CROSS_MONITORING_FAULT_ID:
        errorName = ALARM_CROSS_MONITORING_FAULT;
        break;
    case ALARM_LOGIC_FAULT_ID:
        errorName = ALARM_LOGIC_FAULT;
        break;
    case ALARM_SONICS_ENABLE_FAULT_ID:
        errorName = ALARM_SONICS_ENABLE_FAULT;
        break;
    default:
        errorName = ALARM_UNKNOW_ALARM;
        break;
    }
    return errorName;
}

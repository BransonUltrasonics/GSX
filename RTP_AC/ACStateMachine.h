#pragma once
#include "Ready.h"
#include "ReturnStroke.h"
#include "ForceControl.h"
#include "DownStroke.h"
#include "FindPartContact.h"
#include "Homeing.h"
#include "Common.h"
#include "Commons.h"
#include<iostream>
#include "ioLib.h"
#include "AcDgtInput.h"
#include "Alarm.h"
#include "ForceCalibration.h"
#include "JoggingUp.h"
#include "JoggingDown.h"
#include "Estop.h"
#include "AcDgtOutput.h"
#include "GoingToPosition.h"
#include "ServoReset.h"

using namespace std;

//#define DEMO
#define LINEAR 0
#define ROTARY 2
#define AC_OUTPUT_MASK 0x1f



class ACStateMachine {
	friend class ACState;
public:
	ACStateMachine();
	~ACStateMachine();
	void RunStateMachine();

	static bool alarm; //placeholder for servo amplifier alarms
	static bool moveDone;
	static bool partContactFound;
	static bool homed; //indicates the actuator has an accurate absolute position because the homing routine has completed and there have been no estop or EN faults since then
	static UINT32 foundPartContactPos;

	static void PrintMasterState();
	void PrintCurrentState();
	static bool GetEstop();

protected:
	Ready ready;
	ReturnStroke returnStroke;
	ForceControl forceControl;
	DownStroke downStroke;
	FindPartContact findPartContact;
	Homeing homeing;	
	Alarm acAlarm;
	ForceCalibration forceCalibration;
	JoggingUp joggingUp;
	JoggingDown joggingDown;
	Estop estop;
	GoingToPosition goingToPosition;
	ServoReset servoReset;

	
	void ChangeState(ACSTATES);
private:	
	ACState * currentState;
	UINT16 digOut, unknownFaultCount, servoAlarmCount;
	static bool Estop;
};

//TODO: temporary storage of object dictionary to be replaced when integrating with ethercat implementation
namespace ObjectDictionary {
S_AC_CMD_PDO rpdo;
S_AC_STATUS_PDO tpdo;
AcCtrltaskWeldRecipe AcWeldRecipe;
CalibrationReadings calReadings;
AcFirmwareUpgradeInfo AcFwUpgrade;
}

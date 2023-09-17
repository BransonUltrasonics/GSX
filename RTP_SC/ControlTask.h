/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef CONTROLTASK_H_
#define CONTROLTASK_H_

#include <taskLib.h>
#include  "Common.h"
#include  "CommonProperty.h"
#include  "SC_Common.h"
#include  "ControlTaskCommon.h"
#include "IOManager.h"


enum OPERATION
{
	NO_OPERATION	= 0,
	WELD,		 	
	SEEK,
	FIND_PART_CONTACT_OP,
	JOG_UP_OP,
	JOG_DOWN_OP,
	JOG_READY_POSITION,
	HORN_DOWN_CLAMP_ON,
	HORN_DOWN_CLAMP_OFF,
	HORN_RETRACT,
	ALARM,
	RESET_ALARM,
	SCAN,
	ABORT,
	CALIBRATION,
	TEST,
	STOPTEST,
	ABORT_TEST,
	RESET_ALL_ALARM,
	AC_GO_HOME,
	AC_GO_READY
};

#define CALIBRATION_FINISH 	3
#define CALIBRATION_ABORT	4

typedef enum ACTUATOR_SETUP
{
	ABS_POSITION,
	PRE_PART_CONTACT,
	VELOCITY,
	EXP_PART_CONTACT,
	ACTUAL_FORCE,
	ACTUAL_DISTANCE
}ACTUATOR_SETUP;

/* The structure used for sending response to UIC, While performing any of actuator setup operations */
typedef struct ActuatorSetup
{
	UINT8	ResponseType;			/* FIND_PART_CONTACT_OP, JOG-UP/DOWN, HORN_DOWN, HORN_RETRACT, READY POSITION */
	UINT8	ResponseStatus;			/* Indicates operation status - Success or fail */		
	UINT32 	AbsolutePosition;
	UINT16 	PrePartContactVelocity;
	UINT32 	PrePartContact;
	UINT32 	ExpectedPartContact;
	UINT16	ActualForce;
	UINT32  ActualDistance;
}ActuatorSetup;

class ControlTask
{
    friend class Ready;
    friend class StartSwitch; 
    friend class WaitForReadyPosition;
    friend class WeldSonicOn;
    friend class Hold;
    friend class WaitForTrigger;
    friend class SeekSonicOn;
    friend class Alarm;
    
public:
	ControlTask();
	~ControlTask();

	bool				bIsTaskRunStatus();
	static OPERATION 	GetOperationType();
	static OPERATION 	GetPrevOperationType();
	static OPERATION 	GetNextOperationType();
	static UINT8	 	GetPostWeldDoneFlag();
	static UINT8	 	GetPreWeldDoneFlag();
	static UINT8	 	GetWeldOpFlag();
	static UINT8		GetCalibrationStep();
	static void			SetCalibrationStep(UINT8 step);
	static void			TriggerConsoleTask();
	void				StateMachineHandler();
	static bool			GetLimitExceedAlarm();
	static void			ResetLimitExceedAlarm(bool bIsVal);
	static void			SetActuatorSetupData(ACTUATOR_SETUP typeofData, UINT32 data);
	void				ProcessCommandMessage();
	static void			SetResultData(bool bVal);
	static EC_MASTER_STATE 	EcatGetMasterStateSysCall();
	static void			SetOperationType(OPERATION typeofOp);
	static void 		SetPrevOperationType(OPERATION typeofOp);
	static void			SetPostWeldDoneFlag(UINT8 TempPostWeldDoneFlag);
	static void			SetPreWeldDoneFlag(UINT8 TempPreWeldDoneFlag);
	static void			SetWeldOpFlag(UINT8 TempWeldOpFlag);
	void				SendDataToDataBaseQueue(UINT8 typeOfResult);
	void				PrevOperationHandler(OPERATION prevOpType);
	void				DeCoder(char *recvBuffer,Message& msgDecoder);
	STATUS				SendToMsgQ(Message& msgBuffer, const MSG_Q_ID &msgQID);
	STATUS				StoreWeldResultToEEPROM();
	static void			SetCycleCounter(const UINT32 cycleCounter);
	static void			UpdateCycleCounter();
	static UINT32		GetPartsPerMinCounter();
	static bool			IsWeldAllowed ();
	
	static UINT8		GetEmmcWearLevel ();
	static bool			CheckEmmcWearLevel ();
	
public:
	static bool		m_bIsIndicateUIC;
	static bool		m_bIsHornClampOn;
	
private:
	void			UpdateWeldStatus();
	void			SwitchToNextOp();
	void			UpdateActuatorSetup(UINT8 respType, UINT8 respMsg);
	void			DecideNextOperationType(char *pRecv_Msg, SCSTATES state);
	void			CheckNewActiveRecipeAvailability();
	void			UpdatePartsPerMinCounter();
	
private:
	MSG_Q_ID 		DB_MSG_Q_ID;
	MSG_Q_ID 		SDO_MSG_Q_ID;
	MSG_Q_ID 		CTRL_MSG_Q_ID;
	MSG_Q_ID 		UI_MSG_Q_ID;
	MSG_Q_ID 		ALARM_MSG_Q_ID;
	MSG_Q_ID		UI_DB_MSG_Q_ID;
	MSG_Q_ID		WEBSERVICE_MSG_Q_ID;
	
	static	UINT8	emmcWearLevel;
	static 	TASK_ID	Console_TID;
	TASK_ID  		MAIN_TID;
	UINT32 			events;
	static OPERATION nextOpType;
	static OPERATION OpType, PrevOpType;
	static bool		bIsResultConfig;
	static UINT8	calibrationStep;	
	static UINT8 	PostWeldDoneFlag;
	static UINT8 	PreWeldDoneFlag;
	static UINT8 	WeldOpFlag;
	static UINT32	m_CycleCounter;
	bool			bIsAlarmData;
	bool			bIsPowerup;
	static bool		m_bIsWeldAllowed;
	static bool		m_bIsLimitExceed;
	char 			RecvMsgBuffer[MAX_SIZE_OF_MSG_LENGTH];
	TASK_ID			IO_Image_TID, Mail_Box_TID;
	Message 		message;
	CommonProperty 	*CP;
	DigitalIOMgr 	digitalIOMgr;
	UINT8			m_Setup;
	static ActuatorSetup	m_SetupData;
	static UINT32	m_PartsPerMinCounter;	
	
	float 	startTime, endTime;
};


#endif /* CONTROL_TASK_H_ */

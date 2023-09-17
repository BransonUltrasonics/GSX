/*
 * PCStateMachine.h
 *
 *  Created on: May 23, 2017
 *      Author: eguajardo
 */

#ifndef PCSTATEMACHINE_H_
#define PCSTATEMACHINE_H_


#include "Ready.h"
#include "SeekRun.h"
#include "Sonics.h"
#include "WeldRun.h"
#include "Scan.h"
#include "Alarm.h"
#include "Estop.h"


		
#ifdef TEST_MODE
#include "TestRun.h"			
#endif //TEST_MODE		


#define DEMO


class PCStateMachine
{ 
	friend class PCState;
public:
	PCStateMachine();
	~PCStateMachine();
	  void RunStateMachine();
	  
#ifdef DEMO
	static UINT8 alarm;
	static UINT8 moveDone;
	static SCSTATES masterState;
	static void PrintMasterState();
	void PrintCurrentState();
#endif // DEMO
	
protected:
	void ChangeState(PCSTATES);
	Ready ReadyObj;
	WeldRun WeldRunObj;
	SeekRun SeekRunObj;
	Scan ScanRunObj;
	Alarm AlarmObj;			
	Estop EstopObj;
#ifdef TEST_MODE
	TestRun TestRunObj;			
#endif //TEST_MODE		 

	
private:
		PCState * currentState;
};

//TODO: temporary storage of object dictionary to be replaced when integrating with ethercat implementation
namespace ObjectDictionary {
S_PC_CMD_PDO rpdo;
S_PC_STATUS_PDO tpdo;
PcFirmwareUpgradeInfo PcFwUpgrade;
}


#endif /* PCSTATEMACHINE_H_ */

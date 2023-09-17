/*
 * SCState.h
 *
 *  Created on: May 22, 2017
 *      Author: lzhu
 */

#ifndef SRC_INC_SCSTATE_H_
#define SRC_INC_SCSTATE_H_

#include "stdio.h"
#include <iostream>
#include "state.h"
#include "Common.h"
#include "CommonProperty.h"

using namespace std;

UINT32 AccumulatedEnergy;
UINT32 DistanceStart;
UINT32 Energy;
UINT16 NewMaxWeldForce;
UINT16 NewMaxHoldForce;
UINT16 NewEndHoldForce;

class SCState : public state
{
	friend class SCStateMachine;
public:
	SCState();
	virtual ~SCState();
	void UpdatePDO(void *ODobject, UINT16 ui16Index, UINT8 ui8SubIndex);
	void ReadPDO(void *ODobject, UINT16 ui16Index, UINT8 ui8SubIndex);
    struct SDOStructure SDO;
    UINT32 getCycleCounter();
    void   ResetCycleTimeCounter();
    		
protected:	
    SCSTATES stateType;
	virtual void Enter()=0;
	virtual void Loop()=0;
	virtual void Exit()=0;
	
	virtual void CaptureWeldData();	/* member function used for signature data */
	virtual void ProcessAlarm (UINT8 origin, UINT16 alarmID = 0);
	virtual void SendToMsgQ(MSG_Q_ID msgQID,Message& msg);

	
	void 		ChangeState	(SCSTATES);
	SCSTATES 	GetStateType ();
	
	CommonProperty *CP;
};

#endif /* SRC_INC_SCSTATE_H_ */

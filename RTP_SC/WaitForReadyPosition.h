/*
 * WaitForReadyPosition.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_WAITFORREADYPOSITION_H_
#define INC_WAITFORREADYPOSITION_H_

#include <stdio.h>
#include "SCState.h"

#define MAX_AFTERBURST_DELAY_IN_US 		(2*1000*1000)	//2seconds
#define MAX_AFTERBURST_TIME_IN_US 		(2*1000*1000)	//2seconds
#define MAX_AFTERBURST_DELAY_COUNTER 	(MAX_AFTERBURST_DELAY_IN_US/ECAT_CYCLE_TIME_IN_US)
#define MAX_AFTERBURST_TIME_COUNTER 	(MAX_AFTERBURST_TIME_IN_US/ECAT_CYCLE_TIME_IN_US)

class WaitForReadyPosition :public SCState
{
	friend class SCStateMachine;
	friend class ControlTask;
	
public:
	WaitForReadyPosition();
	~WaitForReadyPosition();
	UINT32 WaitForReadyPosTimeCounter;
	
protected:
	void Enter();
	void  Loop();
	void Exit();
	
private:
	enum AFTERBURST_STATE { AFTERBURST_SETUP, AFTERBURST_SONICS_DELAY, AFTERBURST_SONICS_ON, AFTERBURST_SONICS_TIME, AFTERBURST_SONICS_OFF, AFTERBURST_FINISH };
	AFTERBURST_STATE AfterburstState;
	UINT32  AfterburstTimeCounter;
	UINT32  AfterburstDelayCounter;
	UINT16  AfterburstFlag;
	UINT16  AfterburstAmplitude;
	UINT32  AfterburstTime;
	UINT32  AfterburstDelay;
	UINT32	WaitforReadyTimeout;
	bool 	PostWeldSeek;
	void AfterburstSetup();
	void AfterburstSonicsDelay();
	void AfterburstSonicsON();
	void AfterburstSonicsTime();
	void AfterburstSonicsOFF();
	void AfterburstFinish();
	void CheckSonicsON();
	void ProcessLoop (INT32 cmdID);
	/* virtual member function used for signature data */
	void CaptureWeldData();
	void UpdateHornDown();
};




#endif /* INC_WAITFORREADYPOSITION_H_ */

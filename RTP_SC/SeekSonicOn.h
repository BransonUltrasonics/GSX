/*
 * SeekSonicOn.h
 *
 *  Created on: May 31, 2017
 *      Author: lzhu
 */

#ifndef SRC_INC_SEEKSONICON_H_
#define SRC_INC_SEEKSONICON_H_

#include <stdio.h>
#include <iostream>
#include "SonicOn.h"
#include "EcSDOTask.h" 
#include "DataBaseManager.h"

using namespace std;

class SeekSonicOn : public SonicOn
{
	friend class SCStateMachine;
public:
	SeekSonicOn();
	~SeekSonicOn();
	UINT32 EnergyinJourles;
	UINT32 NewMaxWeldForce;
	UINT32 SeekTime;
	UINT32 SeekTimeCounter;

protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	void UpdateMemOffSet();
	STATUS EventLog (LOG_EVENT eventID, UINT32 prevVal, UINT32 newVal);
	UINT32 EndFrequency;
	INT32 MemOffset;
};


#endif /* SRC_INC_SEEKSONICON_H_ */

/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef TESTSONICSON_H_
#define TESTSONICSON_H_

#include "SonicOn.h"

#define NUM_OF_SCAN_POINTS 				5000
#define MICROSEC_TO_MILLISEC 			0.001
#define HALF_SECOND_TIMER_COUNTER 		2000

class TestSonicOn : public SonicOn
{
	friend class SCStateMachine;
public:
	TestSonicOn();
	~TestSonicOn();
	
	UINT32 uiTestLoopCounter;
	UINT32 TestCounter;	

protected:
	void Enter();
	void Loop();
	void Exit();
	
private:
	UINT32 TestGraphPointCounter;
	UINT32 NumOfScanPoints;
	UINT32 TestTimeCounter;
	UINT32 testTime;
	MSG_Q_ID UI_Q_ID;
	deque<TestSignature> testSignatureDeQue;		/* Accumulates latest 5 seconds signature data */
	bool m_MaxTestSign;
	UINT32 SeekTime;
	UINT32 SeekTimeCounter;
	UINT32 EndFrequency;
	INT32 MemOffset;
	void UpdateTestProgressData();
	void UpdateMemOffSet();
};



#endif /* TESTSONICSON_H_ */

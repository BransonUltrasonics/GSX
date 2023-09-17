/*
 * Hold.h
 *
 *  Created on: May 19, 2017
 *      Author: lzhu
 */

#ifndef INC_HOLD_H_
#define INC_HOLD_H_

#include <stdio.h>
#include "SCState.h"
#include "SysConfigEeprom.hpp"

class Hold :public SCState
{
	friend class SCStateMachine;
public:
	Hold();
	~Hold();
	UINT32 HoldTimeCounter;
	UINT32 HoldTime;
	UINT32 HoldTimeOutEnd;
protected:
	void Enter();
	void Loop();
	void Exit();
private:
	WELDMODES 	weldMode;
	bool		_flagGndDetect;
	SystemConfiguration SysConfig;
	intf_SysConfig_t 	sysConfigData;
	void ProcessLoop ();
	void ProcessAlarm (UINT8 org, UINT16 alarmID = 0);
	/* virtual member function used for signature data */
	void CaptureWeldData();
	void LimitsValidation(); 
};

#endif /* INC_HOLD_H_ */

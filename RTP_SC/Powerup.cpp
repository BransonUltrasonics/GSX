/*
 * Powerup.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: DShilonie
 *      
 *      This state precedes the PREREADY state as it handles all things power-up related
 *      that are done once i.e. password override plug check and override sequence etc.
 */

#include "Common.h"
#include "Commons.h"
#include "Powerup.h"
#include "IOManager.h" 
#include "ControlTask.h"
#include <stdio.h>
#include <iostream>
#include "AlarmManager.h"


extern "C"
{
	#include <customSystemCall.h>
}


// private parameters
#define OVERRIDE_HALF_CYCLE_TIME_IN_US 	2000
#define OVERRIDE_MAX_CYCLE_COUNT 		4

// local private variables
static 	Message msg;

enum
{
	OVERRIDE_STEP1  = 1,
	OVERRIDE_STEP2
};

/**************************************************************************//**
* 
* \brief   - Class constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Powerup::Powerup (): _tCount(0),_cCount(0),_hcTime(0),_sCount(0),_override(false)
{
	stateType = POWER_UP;
}


/**************************************************************************//**
* 
* \brief   - Class destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Powerup::~Powerup ()
{
}


/**************************************************************************//**
* 
* \brief   - This method will be executed when entering this state. It initializes
* 			counters and flags needed to perform this power-up sequence.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Powerup::Enter ()
{
	const INT32 month_days[12] = 
	{
	    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	_tCount = 1;
	_cCount = (UINT16) OVERRIDE_MAX_CYCLE_COUNT;
	_hcTime = OVERRIDE_HALF_CYCLE_TIME_IN_US/ECAT_CYCLE_TIME_IN_US;
	_sCount = (UINT16) OVERRIDE_STEP1;
	_override = false;
	INT32       tm_lowPwr;
	INT32       monthday = 0;
	RTCtimerStr rTime;
	RTCtimerStr SysTime;

	// clear message buffer
	memset(msg.Buffer, 0x00, sizeof(msg.Buffer));

	// set output
	DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_PSWD_OVERRIDE, DigitalIO::LINE_HIGH);
	
#ifndef DEBUG
	 printf ("\nEnter Powerup\n");
#endif

//#define RTC_LV_TEST	 
	 	 
	rTime.tm_year = 0;
	rTime.tm_mon = 0;
	rTime.tm_mday = 0;
	rTime.tm_hour = 0;
	rTime.tm_min = 0; 
	rTime.tm_sec = 0;

	tm_lowPwr = GetRTClowPwr(&rTime);
	MsDelay(3);
	 
	GetRTC(&rTime);
	MsDelay(3);
	 
	if(tm_lowPwr != 0)
	{
         // send it to the alarm
		CP->ind_RTC_LowBatt = true;
		 
		SysTime.tm_year = rTime.tm_year;
		SysTime.tm_mon = rTime.tm_mon;
		SysTime.tm_mday = rTime.tm_mday;
		SysTime.tm_hour = rTime.tm_hour;
		SysTime.tm_min = rTime.tm_min;
		SysTime.tm_sec = rTime.tm_sec;
		
		tm_lowPwr = 0;  //Clear low battery bit
		 
	    //Check Date range	
		if (SysTime.tm_year < MIN_YEAR || SysTime.tm_year > MAX_YEAR)
		{
			SysTime.tm_year = MIN_YEAR;
		}

		// Check validity of month value 
		if (SysTime.tm_mon + 1 < JANUARY || SysTime.tm_mon + 1 > DECEMBER)
		{
			 SysTime.tm_mon = JANUARY;
		}

		// Check validity of day of month value 
		if (IS_LEAP_YEAR (SysTime.tm_year + 1900) &&
		(FEBRUARY == SysTime.tm_mon + 1))
			monthday = 29;
		else
			monthday = month_days[SysTime.tm_mon];

		if (SysTime.tm_mday < 1 || SysTime.tm_mday > monthday)
		{
			SysTime.tm_mday = 1;
		}

		// Check validity of hours value 
		if (SysTime.tm_hour > 23)
		{
			SysTime.tm_hour = 0;
		}

		// Check validity of minutes value 
		if (SysTime.tm_min > 59)
		{
			SysTime.tm_min = 0;
		}


		if (SysTime.tm_sec > 59)
		{
			SysTime.tm_sec = 0;
		}

	    SetRTC(SysTime);  //which clear RTC low battery bit
	    MsDelay(3);
	     
#ifdef RTC_LV_TEST	 //Test codes begin
	     	 RTCtimerStr SysTime;
	     	 
	     	 rTime.tm_year = 0;
	     	 rTime.tm_mon = 0;
	     	 rTime.tm_mday = 0;
	     	 rTime.tm_hour = 0;
	     	 rTime.tm_min = 0; 
	     	 rTime.tm_sec = 0;

	     	 tm_lowPwr = GetRTClowPwr(&rTime);
	     	 MsDelay(3);
	     	 
	     	 GetRTC(&rTime);
	     	 MsDelay(3);
	     	 //taskDelay(ONE_SEC_DELAY);
	     	 rTime.tm_year = 1900 + rTime.tm_year;
	     	 rTime.tm_mon  = 1 + rTime.tm_mon;

	     	 printf("\nGetRTC before :%d/%02d/%02d %02d:%02d:%02d:%02d \n",rTime.tm_year,rTime.tm_mon,rTime.tm_mday,rTime.tm_hour,rTime.tm_min,rTime.tm_sec,tm_lowPwr);
	     	 	     	 //if(rTime.tm_lowPwr != 0)
	     	 if(tm_lowPwr != 0)
	     	 {
	              // send it to the alarm
	     		 CP->ind_RTC_LowBatt = true;
	     		 
	     		 SysTime.tm_year = rTime.tm_year - 1900;
	     		 SysTime.tm_mon = rTime.tm_mon - 1;
	     		 SysTime.tm_mday = rTime.tm_mday;
	     		 SysTime.tm_hour = rTime.tm_hour;
	     		 SysTime.tm_min = rTime.tm_min;
	     		 SysTime.tm_sec = rTime.tm_sec;
	     		 tm_lowPwr = 0;

	     		 //taskDelay(ONE_SEC_DELAY);
	     		 
	     		 if (SysTime.tm_year < MIN_YEAR || SysTime.tm_year > MAX_YEAR)
	     		 {
	     			printf("\nYEAR :%04d \n",SysTime.tm_year);
	     			SysTime.tm_year = MIN_YEAR;
	     		 }

	     		/* Check validity of month value */

	     		 if (SysTime.tm_mon + 1 < JANUARY || SysTime.tm_mon + 1 > DECEMBER)
	     		 {
	     			 printf("\nMon :%02d \n",SysTime.tm_mon);
	     			 SysTime.tm_mon = JANUARY;
	     		 }

	     		/* Check validity of day of month value */

	     		 if (IS_LEAP_YEAR (SysTime.tm_year + 1900) &&
	     		 (FEBRUARY == SysTime.tm_mon + 1))
	     			monthday = 29;
	     		 else
	     			monthday = month_days[SysTime.tm_mon];

	     		 if (SysTime.tm_mday < 1 || SysTime.tm_mday > monthday)
	     		 {
	     			SysTime.tm_mday = 1;
	     		 }

	     		/* Check validity of hours value */

	     		 if (SysTime.tm_hour > 23)
	     		 {
	     			 printf("\nHr :%02d \n",SysTime.tm_hour);
	     			 SysTime.tm_hour = 0;
	     		 }

	     		/* Check validity of minutes value */

	     		 if (SysTime.tm_min > 59)
	     		 {
	     			 printf("\nMin :%02d \n",SysTime.tm_min);
	     			 SysTime.tm_min = 0;
	     		 }


	     		 if (SysTime.tm_sec > 59)
	     		 {
	     			 printf("\nSec :%02d \n",SysTime.tm_sec);
	     			 SysTime.tm_sec = 0;
	     		 }

	     	     SetRTC(SysTime);  //which clear RTC low battery bit
	     	     MsDelay(3);
	     	     
	     		 GetRTClowPwr(&rTime);
	     	     MsDelay(3);
	     		 printf("After set: LowPwr: %02d\n",tm_lowPwr);
	     	     
	     		 rTime.tm_year = 0;
	     		 rTime.tm_mon = 0;
	     		 rTime.tm_mday = 0;
	     		 rTime.tm_hour = 0;
	     		 rTime.tm_min = 0; 
	     		 rTime.tm_sec = 0;

	     	     GetRTC(&rTime);
	     	     MsDelay(3);
	     		 SysTime.tm_year = 1900 + SysTime.tm_year;
	     		 ysTime.tm_mon  = 1 + SysTime.tm_mon;

	     	     printf("\nGetRTC after RTCSet: %04d/%02d/%02d %02d:%02d:%02d\n",SysTime.tm_year,SysTime.tm_mon,SysTime.tm_mday,SysTime.tm_hour,SysTime.tm_min,SysTime.tm_sec);	     	     	    	     
#endif //RTC_LV_TEST
	 }	 
	 else
	 {
		 //Sync System Time equal to RTC Time at every power up.
		 SetRTC(rTime); 
	 }
}


/**************************************************************************//**
* 
* \brief   - This method will be executed every 250us. It will executes all
* 			power-up operations in an interlaced fashion. Will exit when all
* 			operations are complete. For example, actuate the password
* 			override signals until the sequence is complete.  
* 			   
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Powerup::Loop ()
{
	_hcTime = _hcTime - 1;
	
	if (_hcTime == 0)
	{
		switch (_sCount)
		{
		case OVERRIDE_STEP1:
			
			DigitalIOMgr::ReadInputs ();
			
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PSWD_OVERRIDE) == DigitalIO::INPUT_ASSERTED)
				_tCount++;
			
			// reset output
			DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_PSWD_OVERRIDE, DigitalIO::LINE_LOW);
			
			_sCount = (UINT16) OVERRIDE_STEP2;
			_hcTime = OVERRIDE_HALF_CYCLE_TIME_IN_US/ECAT_CYCLE_TIME_IN_US;
			break;
		
		case OVERRIDE_STEP2:
			
			DigitalIOMgr::ReadInputs ();
			
			if (DigitalIOMgr::GetInputFromFunction (DigitalIO::INPUT_PSWD_OVERRIDE) == DigitalIO::INPUT_NOT_ASSERTED)
				_tCount++;
			
			_cCount--;
			
			if (_cCount > 0)
			{
				// set output
				DigitalIOMgr::SetOutputFromFunction (DigitalIO::OUTPUT_PSWD_OVERRIDE, DigitalIO::LINE_HIGH);

				_sCount = (UINT16) OVERRIDE_STEP1;
				_hcTime = OVERRIDE_HALF_CYCLE_TIME_IN_US/ECAT_CYCLE_TIME_IN_US;
			}
			else
			{
				if (_tCount == (UINT16) (2 * OVERRIDE_MAX_CYCLE_COUNT))
				    _override = true;
				
			    ProcessOverride ();
				ChangeState (PREREADY);
			}
			
			break;
		}
	}
}


/**************************************************************************//**
* 
* \brief   - This method will be executed when exiting this state.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Powerup::Exit ()
{
	// check eMMC wear level
	ControlTask::CheckEmmcWearLevel ();

	// check for early alarms
	if (CP->ind_RTC_LowBatt)
			AlarmMgr::EnterAlarmEvent (ALARM_SC, ALARM_RTC_LOW_BATT_WRA);

#ifndef DEBUG
	 printf ("\nExit Powerup: _override: %s\n", _override ? "TRUE" : "FALSE");
#endif

}


/**************************************************************************//**
* 
* \brief   - Relays the final outcome after running the override sequence
* 			using the related I/O signals, to the database manager.  
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
void Powerup::ProcessOverride ()
{
	// send it to the DB task
	memcpy (&msg.Buffer, &_override, sizeof (_override));
	msg.msgID = TO_DB_TASK_PASSWORD_OVERRIDE;
	msgQSend(CommonProperty::getInstance()->GetMsgQId (cTaskName[DB_T]), reinterpret_cast<char*>(&msg), sizeof (msg), NO_WAIT,MSG_PRI_NORMAL);
}

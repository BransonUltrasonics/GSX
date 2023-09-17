/*
 * TimerInterval.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: eguajardo
 */
#include "TimerInterval.h"
#include "DynamicSystemCall.h"
#include"PCObjDictionary.h"
#include <fstream>
#include <string>
#include <iostream>
#include "PCTimer.h"

TimerIntervalDataParam TimerInterval::TmrInterval;


/**************************************************************************//**
* \brief  - Constructor
*			
* \param  - none
*
* \return - none
*
******************************************************************************/
TimerInterval::TimerInterval()
{

}

/**************************************************************************//**
* \brief  - This function reads the timer interval from OD
*			
*			
* \param  - none
*
* \return - none
*
******************************************************************************/
void TimerInterval::ReadTimerIntervalfromOD()
{
		OBJCONST TOBJECT OBJMEM * pObjEntry;
		UINT32 TimerIntervalODLength=0;
		UINT8 Status;
		UINT8 SubIndex=0;
		fstream fp;
		
				fp.open(TIMER_INTERVAL_RECIPE_PATH, ios::out);
				if(fp.is_open())
				{
					memset(&TmrInterval,0,sizeof(TimerIntervalDataParam));
					pObjEntry = obj_getobjecthandleSc(INDEX_TIMER_INTERVAL);
					if(pObjEntry == NULL)
					{
						LOG("   Failed to get the pointer to  TimerInterval OBJ\n");
						exit(-1);
					}
					TimerIntervalODLength = obj_getobjectlengthSc(INDEX_TIMER_INTERVAL, SubIndex, pObjEntry, COMPLETE_ACCESS);
					Status= obj_readSc(INDEX_TIMER_INTERVAL, SubIndex, TimerIntervalODLength, pObjEntry, (UINT16 MBXMEM *) Buffer, COMPLETE_ACCESS );
				
					UINT8* pBuffer = (UINT8 *)Buffer;
					if(Status==SUCCESS)
						{	
							memcpy(&TmrInterval,pBuffer,sizeof(TimerIntervalDataParam));
							fp <<"TimerIntervalPC:\t"<< TmrInterval.TmrIntervalPC<<endl;
						#ifdef PRINT
							LOG("--------------------Timer Interval----------------------------------\n");
							LOG("   TimerIntervalPC        = %d\n",TmrInterval.TmrIntervalPC);
						#endif
							
						}
					else
					{
						LOG("   Failed to read Timer Interval from OD\n");
					}
					fp.close();
					PCTimer::Timer_Init();
				}
				else
				{
					#ifdef ERROR_PRINT
					LOG("Error reading TimerInterval from text file\n");
					#endif
				
				}
				fp.close();
			memset(Buffer,'\0',GLOBAL_BUFFER_SIZE);	
}

/******************************************************************************
* \brief  - Gets timer interval to set the sample rate
*				
* \param  - none
*
* \return - none
*
******************************************************************************/
UINT32 TimerInterval::GetTimerInterval()
{
	return(TmrInterval.TmrIntervalPC);
}

/******************************************************************************
* \brief  - Gets timer interval from txt file
*					
* \param  - none
*
* \return - none
*
******************************************************************************/
void TimerInterval::ReadTimerIntervalfromtxt()
{
	string TempStr;
	string TmrIntervalTxt;
	fstream fp;
	
	fp.open(TIMER_INTERVAL_RECIPE_PATH, ios::in);
	
	if(fp.is_open())
	{
		getline(fp,TempStr,'\0');
		{
			TmrIntervalTxt = TempStr.substr(TempStr.find("TimerIntervalPC:"));	
			TmrIntervalTxt = TmrIntervalTxt.substr(TmrIntervalTxt.find(':')+1);
			TmrInterval.TmrIntervalPC = atoi(TmrIntervalTxt.c_str());
		}
	#ifdef PRINT
		LOG("Timer Interval PC:%d\n",TmrInterval.TmrIntervalPC);
	#endif
	}
	fp.close();
}

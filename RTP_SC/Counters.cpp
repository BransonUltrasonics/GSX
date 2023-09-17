/*
 * Counters.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: slurie
 */

#include <string>
#include  <fstream>
#include "Counters.h"
#include "Eeprom.h"
#include "Common.h"
#include "CommonProperty.h"

using namespace std;

UINT32 Counters::overloads = 0;
UINT32 Counters::welds = 0;
UINT32 Counters::actuations = 0;
UINT32 Counters::alarms = 0;

/**************************************************************************//**
* \brief   - Default constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Counters::Counters() {
}


/**************************************************************************//**
* \brief   - Default destructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Counters::~Counters() {
}


/**************************************************************************//**
* \brief   - Get a counter value
*
* \param   - type - Which counter type
*
* \return  - counter vaule.
*
******************************************************************************/
UINT32 Counters::getCounter(CounterType type)
{
	UINT32 value;
	switch (type)
	{
	case LIFETIME_OVERLOADS:
		value = overloads;
		break;

	case LIFETIME_WELDS:
		value = welds;
		break;

	case LIFETIME_ACTUATIONS:
		value = actuations;
		break;

	case LIFETIME_ALARMS:
		value = alarms;
		break;
		
	default:
		break;
	}
	return value;
}


/**************************************************************************//**
* \brief   - Add one to a counter
*
* \param   - type - which counter
*
* \return  - None.
*
******************************************************************************/
void Counters::incrementCounter(CounterType type)
{
	bool save = true;
	
	switch (type)
	{
	case LIFETIME_OVERLOADS:
		overloads++;
		break;

	case LIFETIME_WELDS:
		welds++;
		break;

	case LIFETIME_ACTUATIONS:
		actuations++;
		break;

	case LIFETIME_ALARMS:
		alarms++;
		break;
		
	default:
		save = false;
		break;
	}
	
	if (save)
		saveCounters();
}


/**************************************************************************//**
* \brief   - store all counters to a file (to be called after every weld)
*
* \param   - None.
*
* \return  - bool written
*
******************************************************************************/
bool Counters::saveCounters()
{
	char counters[MAX_SIZE_OF_MSG_BUFF];
	bool written = false;
	fstream fp;
	string sequence;

	fp.open (LIFETIME_COUNTERS_PATH, fstream::in | fstream::out | fstream::trunc);
	
	if(fp.is_open())
	{
		snprintf (counters, sizeof (counters), "%d\n%d\n%d\n%d\n", welds, overloads, actuations, alarms);
		sequence = counters;
		
		fp << sequence;
		fp.close();
		written = true;
	}
	else
		LOGERR("Counters::saveCounters: file error",0,0,0);
	
	return written;
}


/**************************************************************************//**
* \brief   - Load counters from a file or EEPROM (to be called only once at bootup)
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
bool Counters::loadCounters()
{
	Eeprom epclass;
	char eeprom_sig;
	UINT32 results=0;
	bool read = false;
	string counter;
	fstream fp;

	if(CommonProperty::getInstance()->IsFileExist(LIFETIME_COUNTERS_PATH))
	{
		fp.open(LIFETIME_COUNTERS_PATH, ios::in);

		if(fp.is_open())
		{
			fp.seekp(0);
			
			getline (fp,counter);
			welds = atoi (counter.c_str());
			getline (fp,counter);
			overloads = atoi (counter.c_str());
			getline (fp,counter);
			actuations = atoi (counter.c_str());
			getline (fp,counter);
			alarms = atoi (counter.c_str());
			
			fp.close();
			read = true;
		}
		else
		{
			overloads = 0; welds =  0; actuations = 0; alarms = 0;	
			LOGERR("Counters::loadCounters: file error",0,0,0);
		}
	}
	else
	{
		memset(&welds,0x0,sizeof(welds));
		epclass.Read((char *)&welds,sizeof(welds),COUNTER_ADDR);
		
		memset(&overloads,0x0,sizeof(overloads));
		epclass.Read((char *)&overloads,sizeof(overloads),OVERLOAD_ADDR);

		memset(&actuations,0x0,sizeof(actuations));
		epclass.Read((char *)&actuations,sizeof(actuations),ACTUATION_ADDR);

		memset(&alarms,0x0,sizeof(alarms));
		epclass.Read((char *)&alarms,sizeof(alarms),ALARMS_ADDR);
		
		if (welds < 0 || overloads < 0 || actuations < 0 || alarms < 0)
			{overloads = 0; welds =  0; actuations = 0; alarms = 0;}
		
		read = saveCounters();
	}

	return read;
}

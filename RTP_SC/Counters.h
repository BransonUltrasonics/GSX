/*
 * Counters.h
 *
 *  Created on: Dec 11, 2018
 *      Author: slurie
 */

#ifndef COUNTERS_H_
#define COUNTERS_H_

#include "Common.h"

typedef enum  {LIFETIME_OVERLOADS, LIFETIME_WELDS, LIFETIME_ACTUATIONS, LIFETIME_ALARMS} CounterType;

class Counters {
public:
	Counters();
	virtual ~Counters();
	
	static UINT32	getCounter			(CounterType);
	static void 	incrementCounter	(CounterType);
	static bool 	saveCounters		();
	static bool 	loadCounters		();
	
private:
	static UINT32 overloads, welds, actuations, alarms;
};

#endif /* COUNTERS_H_ */

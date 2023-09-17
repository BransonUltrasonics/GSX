/*
 * TestResults.h
 *
 *  Created on: Mar 21, 2018
 *      
 */
 
#ifndef TESTRESULTS_H_
#define TESTRESULTS_H_

#include "Common.h"
#include "WeldRecipe.h"
 
typedef enum  
{
	TEST_TIME,	
	TEST_PEAK_POWER,
	RESONANT_FREQ,
	FREQUENCY_CHANGE,
} TEST_RESULT_IDX;

class TestResults
{
public:
	TestResults();
	//TestResults(INT32 Amplitude, INT32 Frequency, INT32 Memory, INT32 Power, INT32 RecipeNumber);
	~TestResults();
	
	void SetData(unsigned char *RecvBuffer);
	
	UINT32 	Get		(TEST_RESULT_IDX idx);
	void 	Set		(TEST_RESULT_IDX idx, UINT32 data);
	
public:
	static TEST_RESULT_IDX testResultIdx;
	
private:
	TestResult testResult;
};

#endif /* TESTRESULTS_H_ */

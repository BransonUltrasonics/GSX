/*
 * TestResults.cpp
 *
 *  Created on: Mar 21, 2018
 *     
 */

#include "TestResults.h"
#include "DataBaseManager.h"



TEST_RESULT_IDX TestResults::testResultIdx;

/**************************************************************************//**
* \brief  - Sets parameters at start up (needed for static initialization)
*
* \param  - All results parameters to initialize
*
*
* \return  - None
*
******************************************************************************/
TestResults :: TestResults () 
{
#ifdef DEBUG_TEST
	testResult.TestTime = 655355;
	testResult.TestPeakPower = 65535;
	testResult.ResonantFreq = 655356;
	testResult.FrequencyChange = 6555;
#endif	//DEBUG_TEST
	
	#ifdef DEBUG
		logMsg("DB_T : -----------Constructor called of TestResult------------\n",0,0,0,0,0,0);
	#endif
}


/**************************************************************************//**
* \brief  - Gets any parameter from the test results.
*
* \param  - idx - index of specific result to get
*
*
* \return  - Specific result's value
*
******************************************************************************/
UINT32 TestResults :: Get(TEST_RESULT_IDX idx)
{
	UINT32 result;	
	switch (idx)
	{
		case TEST_TIME:
			result = 		testResult.TestTime;
			break;
		case TEST_PEAK_POWER:
			result = 		testResult.TestPeakPower;
			break;
		case RESONANT_FREQ:
			result = 		testResult.ResonantFreq;
			break;
		case FREQUENCY_CHANGE:
			result = 		testResult.FrequencyChange;
			break;
		default:
			result = 		0;
			break;
	}	
	return result;
}


/**************************************************************************//**
* \brief  - Saves a specific result from the test results.
*
* \param  - idx - 	index of specific result to save
* \param  - data - 	value to set specific result to
*
*
* \return  - None
*
******************************************************************************/
void TestResults :: Set(TEST_RESULT_IDX idx, UINT32 data)
{
#ifdef DEBUG
	printf("idx : %d, data : %d\n",idx,data);
#endif
	switch (idx)
	{
		case TEST_TIME:
			testResult.TestTime = data;
			break;
		case TEST_PEAK_POWER:
			testResult.TestPeakPower = data;
			break;
		case RESONANT_FREQ:
			testResult.ResonantFreq = data;
			break;
		case FREQUENCY_CHANGE:
			testResult.FrequencyChange = data;
			break;					
	}
}


/*
******************************************************************************

*      Function    :   	SetData
*      Description :   	Assign TestResult values to member data. 
*      Input       :   	unsigned char *RecvBuffer
*      Output      :   	void
*      Called by   :  	ControlTask::SendDataToDataBaseQ

*******************************************************************************
*/
void TestResults::SetData(unsigned char *RecvBuffer)
{	
}

/*
******************************************************************************

*      Function    :   	~TestResults
*      Description :   	Memory release of object.
*      Input       :   	void
*      Output      :   	void
*      Called by   :  	Object Destruction 

*******************************************************************************
*/
TestResults::~TestResults()
{
	#ifdef DEBUG
		printf("---------Destructor called of TestResult------------\n");
	#endif
}

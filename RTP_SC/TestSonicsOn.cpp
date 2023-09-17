/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Sonics Test feature functionality of SC for different kinds of Test operations
 
**********************************************************************************************************/

#include "TestSonicsOn.h"
#include "Common.h"
#include "ControlTask.h"
#include "MainTask.h"
#include "AlarmManager.h"

/**************************************************************************//**
* \brief  - Constructor.
* \brief  - TestSonicOn Class constructor
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
TestSonicOn::TestSonicOn()
{
	stateType = TEST_SONICS_ON;
}


/**************************************************************************//**
* \brief  - Destructor.
* \brief  - TestSonicOn Class Destructor
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
TestSonicOn::~TestSonicOn()
{
}

/**************************************************************************//**
* \brief  - Execute The Enter function to the TestSonicsOn state.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
void TestSonicOn::Enter()
{
	LOGDBG("enter TEST  on",0,0,0);
	NumOfScanPoints 		= NUM_OF_SCAN_POINTS;	
	CP->testSignature 		= new TestSignature[NumOfScanPoints + 1];  		/* SIZE_SCANBUFFER */	
	UI_Q_ID 				= CP->GetMsgQId(cTaskName[UI_T]);
	TestGraphPointCounter 	= 0;							// To ensure sonics stay on for at least 1500 samples
	TestTimeCounter 		= 0;
	TestCounter 			= 0;
	m_MaxTestSign 			= false;
	UINT32 SeekTimeRecipe 	= CP->m_SeekRecipeObj.GetSeekTime();
	SeekTime 				= SeekTimeRecipe*(1000/ECAT_CYCLE_TIME_IN_US);
	SeekTimeCounter			= 0;
	EndFrequency			= 0;
	MemOffset				= 0;
	LOGDBG("SeekSonic : SeekTime : %d",SeekTime,0,0);
	testSignatureDeQue.clear();
}

/**************************************************************************//**
* \brief  - Execute The Loop function to the TestSonicsOn state.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
void TestSonicOn::Loop()
{
	if(PC_TX->PCState == PC_ALARM)
	{
		ProcessAlarm (ALARM_PC);
			
	}
	else if(SeekTimeCounter < SeekTime)
	{
		SeekTimeCounter = SeekTimeCounter + 1;
	}
	else
	{
		PC_RX->Amplitudechange = CP->m_TestRecipeObj.GetTestAmplitude();
		PC_RX->MasterEvents |= BIT_MASK(CTRL_WELDTEST);
	}
	
	TestTimeCounter = TestTimeCounter + 1;			
	if(ABORT_TEST != ControlTask::GetOperationType())
	{										
		TestCounter = TestCounter + 1;
		CP->testSignature[TestGraphPointCounter].Time	 	= 		(float)TestCounter/4;
		CP->testSignature[TestGraphPointCounter].Frequency 	= 		PC_TX->Frequency/10;			
		CP->testSignature[TestGraphPointCounter].Amplitude 	= 		PC_TX->Amplitude;		
		CP->testSignature[TestGraphPointCounter].Power 		= 		PC_TX->Power;
		CP->testSignature[TestGraphPointCounter].Current 	= 		PC_TX->Current/100;
		CP->testSignature[TestGraphPointCounter].Phase 		= 		PC_TX->Phase/10;
		
		//Make sure first sample point has "Time" value as 0.0
		if(TestTimeCounter <= 4)
		{
			CP->testSignature[TestGraphPointCounter].Time	 	=	0.0;
		}
		
		/* Write Progress bar data to UIC every 500ms */
		if(0 == (TestTimeCounter % HALF_SECOND_TIMER_COUNTER))
		{
			UpdateTestProgressData();					
		}	
		
		if(TestTimeCounter % 4 == 0)//reduce by 4 
		{
			/* Maintain the latest signature data if it goes beyond 5 seconds(5,000 samples) */
			if( (TestGraphPointCounter >= NUM_OF_SCAN_POINTS) || (m_MaxTestSign == true) )
			{				
				m_MaxTestSign = true;
				testSignatureDeQue.pop_front();						 
				testSignatureDeQue.push_back(CP->testSignature[TestGraphPointCounter]);
				TestGraphPointCounter = 0;						/* To ensure the signature array doesn't overflow */			
			}		
			else		
			{				
				testSignatureDeQue.push_back(CP->testSignature[TestGraphPointCounter]);
			}				
			TestGraphPointCounter = TestGraphPointCounter + 1;					//Condition to stop Test Sonic after certain iterations, just a stub.
		}
	}			
	else if(ABORT_TEST == ControlTask :: GetOperationType())
	{											
		LOGDBG("--------------- TestSonicOn Operation completed ---------------",0,0,0);
		ControlTask::SetResultData(true);		/* To collect test results */
		ChangeState(PREREADY);
	}	
}
	
/**************************************************************************//**
* \brief  - Execute The Exit function to the TestSonicsOn state.
*
* \param  - none.
*			
* \return  - none.
*
******************************************************************************/
void TestSonicOn::Exit()
{
	UINT32 NewStartFrequency;

	PC_RX->MasterEvents &= ~BIT_MASK(CTRL_WELDTEST);
	
	/* Calculate the Test time based on TestTimeCounter */										
	testTime = (TestTimeCounter/4);		
	/* Get the signature samples */		
	if(m_MaxTestSign == true)
	{	
		LOGWARN("------------Test Operation was performed beyond 5 seconds------------",0,0,0);
		UINT16 testSignIndex = 0;
		deque<TestSignature> :: iterator testSignIterator = testSignatureDeQue.begin();
		CP->NoOfSamples = testSignatureDeQue.size();				
		LOGDBG("------------Number of Samples: ------------ %d",CP->NoOfSamples,0,0);
		while(testSignIterator != testSignatureDeQue.end())		
		{			
			CP->testSignature[testSignIndex] = *testSignIterator++;
			testSignIndex++;
		}		
	}	
	else
	{
		CP->NoOfSamples = TestGraphPointCounter;
		LOGDBG("------------Number of Samples: ------------ : %d",CP->NoOfSamples,0,0);
	}
					
	if((TestGraphPointCounter > 0) && (NULL != CP->testSignature))
	{
		LOGDBG("Exit TestSonicOn",0,0,0);			
		/* To store test result into DB */						//Writing some random values. only a stub. 		
		CP->testResult.Set(TestResults :: testResultIdx = TEST_TIME, testTime);
		CP->testResult.Set(TestResults :: testResultIdx = TEST_PEAK_POWER, CP->testSignature[TestGraphPointCounter].Power);
		CP->testResult.Set(TestResults :: testResultIdx = RESONANT_FREQ, CP->testSignature[TestGraphPointCounter].Frequency);
		CP->testResult.Set(TestResults :: testResultIdx = FREQUENCY_CHANGE, (CP->testSignature[TestGraphPointCounter].Frequency - 1));			
	}	
	
	if(PC_TX->PCState != PC_ALARM)
	{	
		UpdateMemOffSet();	
		
		if (CP->stackRecipe.GetData(StackRecipe::INTERNAL_OFFSET_FLAG)==1)
	    {
			NewStartFrequency = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET)+CP->stackRecipe.GetData(StackRecipe::INTERNAL_FREQ_OFFSET);
	    }
		else
		{
			NewStartFrequency = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE)+CP->stackRecipe.GetData(StackRecipe::MEMORY_OFFSET);
		}
				
		CP->recipe["StartFrequency"] = NewStartFrequency;
	}
}

/**************************************************************************//**
* \brief   - Member function will update progress data to UIC 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void TestSonicOn::UpdateTestProgressData()
{
	char signatureData[MSG_SIZE] = {0x00};
	Message Send;
	
	memset(Send.Buffer, 0x00, sizeof(Send.Buffer));
	sprintf(signatureData,"%d,%d,%d,%d",CP->testSignature[TestGraphPointCounter].Frequency,CP->testSignature[TestGraphPointCounter].Frequency,CP->testSignature[TestGraphPointCounter].Amplitude,CP->testSignature[TestGraphPointCounter].Power);	
	Send.msgID = TO_UI_TASK_TEST_PROGRESS_IND;
	memcpy(Send.Buffer, signatureData, sizeof(signatureData));
	
	if(msgQSend(UI_Q_ID,reinterpret_cast<char*>(&Send), sizeof(Send),NO_WAIT, MSG_PRI_NORMAL) != OK)
	{		
		LOGERR("TestSonicOn : -----------Error on msgQSend----------",0,0,0);
	}
}

/**************************************************************************//**
* 
* \brief   - This function updates the MemOffSet for the system. 
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
void TestSonicOn::UpdateMemOffSet()
{
	UINT16 Count;   
	UINT32 Memory;
	
  	EndFrequency= PC_TX->Frequency;

	if (PC_TX->PCState!=PC_ALARM)
	{
		MemOffset = EndFrequency - (CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE));  
	}
	else
	{
		MemOffset = 0;
	}

	CP->stackRecipe.SetData(StackRecipe::MEMORY_OFFSET, MemOffset);
	Memory = CP->stackRecipe.GetData(StackRecipe::DIGITAL_TUNE) + MemOffset;
	LOGDBG("--------------this is memoryvalue = %d, = %d", Memory, MemOffset,0);
	CP->stackRecipe.SetData(StackRecipe::MEMORY, Memory);
	CP->recipe["MemoryOffset"] = MemOffset;
	CP->m_RecipePC.UpdateMemoryOffset(MemOffset);
	CP->m_RecipePC.UpdateResonantFreq(EndFrequency);

#ifdef DEBUG
	LOGDBG("MemOffset = %d", MemOffset,0,0);
#endif
}


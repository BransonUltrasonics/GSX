/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef TESTRECIPE_H_
#define TESTRECIPE_H_

#include  <sys/types.h>
#include  <iostream>

#define TEST_PHASELIMITTIME 		1000
#define TEST_PHASELIMIT 		 	1000
#define TEST_AMPLITUDE_DEFAULT		50

typedef struct testRecipe
{
	INT32  AmplitudeLoopC1;	
	INT32  AmplitudeLoopC2;
	INT32  PhaseLoopI;
	INT32  PhaseLoopCF;
	INT32  FrequencyHigh;
	INT32  FrequencyLow;
	UINT16 TestAmplitude;	
}testRecipe;

class TestRecipe
{
public:
	TestRecipe();
	~TestRecipe();
	
	INT32  GetAmplitudeLoopC1() const;
	INT32  GetAmplitudeLoopC2() const;
	INT32  GetPhaseLoopI() const;
	INT32  GetPhaseLoopCF() const;
	INT32  GetFrequencyHigh() const;
	INT32  GetFrequencyLow() const;
	UINT16 GetTestAmplitude() const;
	void   UpdateTestRecipe(testRecipe& recipe);
	
private:
	testRecipe m_recipe;
};

#endif /* TESTRECIPE_H_ */

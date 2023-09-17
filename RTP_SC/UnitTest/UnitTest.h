/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef UNITTEST_H_
#define UNITTEST_H_

#include "EthernetIP.h"

class UnitTest
{
public:
	EthernetIP * TestObject;
	UnitTest();
	~UnitTest();
	
	void UnitTestFunctionInput();
	void UnitTestFunctionOutput();
	void PrintBuffer(UINT8* expectedResult);
	
	UINT8 TestVariable1[MAX_FBEXCHANGESIZE];
	UINT8 TestVariable2[MAX_FBEXCHANGESIZE];
	UINT8 TestVariable3[MAX_FBEXCHANGESIZE];
	
	bool InOut;

};

#endif /* UNITTEST_H_ */

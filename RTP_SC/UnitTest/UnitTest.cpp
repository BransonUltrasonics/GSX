/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2012
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

 
 
**********************************************************************************************************/

#include "UnitTest.h"
#include "EthernetIP.h"
#include <stdio.h>

UnitTest::UnitTest()
{
	TestObject = EthernetIP::Getinstance();
	
	UINT64 val1 = 135652151861;
	UINT64 val2 = 316846854544444;
	UINT64 val3 = 8569785684846846444;
	
	memset(TestVariable1 , 0 , sizeof(TestVariable1));
	memset(TestVariable2 , 0 , sizeof(TestVariable2));
	memset(TestVariable3 , 0 , sizeof(TestVariable3));
	
	for(int i = 0; i < sizeof(val1); i++)
		TestVariable1[i] = (UINT8)(val1 >> (8*i));
		
	for(int i = 0; i < sizeof(val2); i++)
		TestVariable2[i] = (UINT8)(val2 >> (8*i));
	
	for(int i = 0; i <sizeof(val3); i++)
		TestVariable3[i] = (UINT8)(val3 >> (8*i));
}

UnitTest::~UnitTest()
{

}

void UnitTest::UnitTestFunctionInput()
{	
	static long x = 0;
	
	switch(x)
	{
	case 10000:
		printf("\n\n%sInput test 1 start%s\n",KRED,KNRM);
		
		InOut = 1;
		
		TestObject->UpdateInputCyclic(TestVariable1);
		
		PrintBuffer(TestVariable1);
		
		break;
		
	case 20000:
		printf("\n\n%sInput test 2 start%s\n",KRED,KNRM);
		
		InOut = 1;
		
		TestObject->UpdateInputCyclic(TestVariable2);
		
		PrintBuffer(TestVariable2);
		
		break;
		
	case 30000:
		printf("\n\n%sInput test 3 start%s\n",KRED,KNRM);
		
		InOut = 1;
		
		TestObject->UpdateInputCyclic(TestVariable3);
		
		PrintBuffer(TestVariable3);
		
		break;
		
	default:
		//printf("Default");
		break;
	}
	x++;
}

void UnitTest::UnitTestFunctionOutput()
{
	static long x = 0;
	
	switch(x)
		{
		case 10001:
			printf("\n\n%sOutput test 1 start%s\n",KCYN,KNRM);
			
			InOut = 0;
			
			memset(TestVariable1, 0, sizeof(TestVariable1));
			memcpy(TestObject->FBOutputs, TestObject->FBInputs, sizeof(TestObject->FBInputs));
			
			TestObject->UpdateOutputCyclic(TestVariable1);
			
			PrintBuffer(TestVariable1);
			
			break;
			
		case 20001:
			printf("\n\n%sOutput test 2 start%s\n",KCYN,KNRM);
			
			InOut = 0;
			
			memset(TestVariable2, 0, sizeof(TestVariable2));
			memcpy(TestObject->FBOutputs, TestObject->FBInputs, sizeof(TestObject->FBInputs));
			
			TestObject->UpdateOutputCyclic(TestVariable2);
			
			PrintBuffer(TestVariable2);
			
			break;
			
		case 30001:
			printf("\n\n%sOutput test 3 start%s\n",KCYN,KNRM);
			
			InOut = 0;
			
			memset(TestVariable3, 0, sizeof(TestVariable3));
			memcpy(TestObject->FBOutputs, TestObject->FBInputs, sizeof(TestObject->FBInputs));
			
			TestObject->UpdateOutputCyclic(TestVariable3);
			
			PrintBuffer(TestVariable3);
			
			break;
			
		default:
			//printf("Default");
			break;
		}
		x++;
		
	return;
}

void UnitTest::PrintBuffer(UINT8* Buffer)
{
	UINT8 localBuffer[FB_INPUT_BYTES], curByte = 0;
	
	memset(localBuffer , 0 , sizeof(localBuffer));
	memcpy(localBuffer, Buffer, sizeof(localBuffer));
	
	for(INT8 i = 0; i < MAX_FBEXCHANGESIZE; i++)
	{
		if(i == 0)
		{
			if(InOut)
				printf("\n Actual result\n");
			else
				printf("\n Expected result\n");
		}
		
		printf("%d",TestObject->FBInputs[i].value);
		
		if((i + 1) % 4 == 0)
			printf(" ");
	}
	
	printf("\n");
	
	for(INT8 i = 0 ; i < 8; i++)
	{
		if(i == 0)
		{
			if(InOut)
				printf("\n Expected result\n");
			else
				printf("\n Actual result\n");
		}
		
		curByte = localBuffer[i];
		
		for(INT8 bit = 0 ; bit < 8; bit++)
		{	
			if(curByte & BIT_MASK(bit))
				printf("1");
			else
				printf("0");
			
			if((bit + 1) % 4 == 0)
				printf(" ");
		}
	}
}

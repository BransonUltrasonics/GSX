// $Header:   D:/databases/VMdb/archives/EN13849/LUTest.c_v   1.13   09 Apr 2015 17:11:42   ewomack  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

This is the module for PowerUp Logic Unit Test.

Module name: Led

Filename:    Led.c

--------------------------- TECHNICAL NOTES -------------------------------

This code does the test for All Logic units at power up.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/LUTest.c_v  $
 * 
 *    Rev 1.13   09 Apr 2015 17:11:42   ewomack
 * Use all 4 FlashTest functions.
 * 
 *    Rev 1.12   20 Jul 2011 16:58:12   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.11   05 Jul 2011 11:00:26   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.8   22 Jun 2011 18:10:22   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:12:20   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:06:30   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/

#include "LPC17xx.h"
#include "TestMacros.h"
#include "Diagnostics.h"
#include "DeviceDiagnostics.h"
#include "Global.h"
#include "StateMachine.h"
#include "ProgramCounterTest.h"

#define NVIC_ICER0_DISABLE_INTERRUPTS 0xFFFFFFFF
#define NVIC_ICER1_DISABLE_INTERRUPTS 0x00000001

#define SAFE_STARTADDR   0x10004000
#define SAFE_ENDADDR     0x10004400

#define STACK_STARTADDR  0x10007C00
#define STACK_ENDADDR    0x10008000

#define DATA_STARTADDR   0x10000000
#define DATA_ENDADDR     0x10000400

UT_VARIABLE(tSINT32 UnitTest)
UT_VARIABLE(tSINT32 RamUnitTest)
UT_VARIABLE(tSINT32 RegTestPass)
UT_VARIABLE(tSINT32 RegTestFail)
UT_VARIABLE(tSINT32 FlashTestPass)
UT_VARIABLE(tSINT32 FlashTestFail)
UT_VARIABLE(tSINT32 RamTestPass)
UT_VARIABLE(tSINT32 RamTestFail)
UT_VARIABLE(tSINT32 PCTestPass)
UT_VARIABLE(tSINT32 PCTestFail)
UT_VARIABLE(tSINT32 INTTestPass)
UT_VARIABLE(tSINT32 INTTestFail)
UT_VARIABLE(tSINT32 ADCStatusFillOK)
UT_VARIABLE(tSINT32 ADCStatusFault)

UT_VARIABLE(tSINT32 RamSafeTestPass)
UT_VARIABLE(tSINT32 RamSafeTestFail)
UT_VARIABLE(tSINT32 RamStackTestPass)
UT_VARIABLE(tSINT32 RamStackTestFail)
UT_VARIABLE(tSINT32 RamDataTestPass)
UT_VARIABLE(tSINT32 RamDataTestFail)

void ADC_Test(void);
tSINT32 FlashTest1(void);
tSINT32 FlashTest2(void);
tSINT32 FlashTest3(void);
tSINT32 FlashTest4(void);
tSINT32 RamTest(tUINT32 *START_ADDRESS,tUINT32 * END_ADDRESS);
tSINT32 ProgramCounterTest(void);
tSINT32 TestInterruptTimer(tSINT32 TimerIntCount);


/**************************************************************************//**
* \brief -		RegisterTest
*
* \brief -		Register Test at PowerUP.
*
* \param -		none
* 
* \return -	PASS or FAIL.
*
*****************************************************************************/
tSINT32 RegisterTest(void)
{
    __asm(  "push   {lr} \n"
    "push   {r0-r12,r14} \n"
    "ldr    r0,=RegTest \n"
    "blx    r0 \n"
    "ldr    r0,=Reg1Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg2Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg3Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg4Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg5Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg6Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg7Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg8Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg9Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg10Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg11Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg12Test \n"
    "blx    r0 \n"
    "ldr    r0,=Reg14Test \n"
    "blx    r0 \n"
    "ldr    r0,=MSPRegTest \n"
    "blx    r0 \n"
    "ldr    r0,=PSPRegTest \n"
    "blx    r0 \n"
    "ldr    r0,=APSRRegTest \n"
    "blx    r0 \n"
    "ldr    r0,=PRIMASKRegTest \n"
    "blx    r0 \n"
    "ldr    r0,=FAULTMASKRegTest \n"
    "blx    r0 \n"
    "ldr    r0,=BASEPRIREGTest \n"
    "blx    r0 \n"
    "pop    {r0-r12,r14} \n"
    "pop    {lr}");
    return PASS;
}

/**************************************************************************//**
* \brief -		RAM_SAFEAREA_TEST
*
* \brief -		RAM_SAFEAREA_TEST is used to test the RAM area which we
* 				will need to store Stack and Data section of RAM during
* 				RAM Stack and data section test.
* 				Following steps are done during this test:
* 				1.> Disable Interrupts.
* 				2.> Test the Safe Area.
* 				3.> Enable Interrupts and return result.
*
* \param -		START_ADDRESS: Start Address of RAM.
*     			END_ADDRESS: End Address of RAM.
*                      
* \return -	tUINT32 result  PASS/FAIL.
*
*****************************************************************************/
tUINT32 RAM_SAFEAREA_TEST(tUINT32 StartAddr,tUINT32 EndAddr)
{
	tUINT32 result  = FAIL;
	tUINT32 * START_ADDRESS;
	tUINT32 * END_ADDRESS;
	START_ADDRESS = (tUINT32 *) StartAddr;//0x10004000; // StartAddress of Safe Area.
	END_ADDRESS = (tUINT32 *) EndAddr;//0x10004400;   // End Address of SafeArea.
	__disable_irq(); // Disable all interrupts.
	result = RamTest(START_ADDRESS,END_ADDRESS); // Test the RAM Safe Area.
	__enable_irq(); // Enable all interrupts again.
	return result;
}

/**************************************************************************//**
* \brief -		RAM_STACKAREA_TEST
*
* \brief -		RAM_STACKAREA_TEST is used to test the RAM stack area.
* 				Following steps are done during this test:
* 				1.> Disable Interrupts.
* 				2.> Copy the contents of Stack area first to SafeArea.
* 				3.> Set Stack pointer to the SafeArea.
* 				4.> Test the Stack Area.
* 				5.> Restore the Stack Area contents.
* 				6.> Restore Stack Pointer.
* 				7.> Enable Interrupts and return result.
*
* \param -		START_ADDRESS: Start Address of RAM.
*            	END_ADDRESS: End Address of RAM.
*            	
* \return -		tUINT32 result  PASS/FAIL.
*
*****************************************************************************/
tUINT32 RAM_STACKAREA_TEST(tUINT32 StartAddr,tUINT32 EndAddr)
{
	tUINT32 result  = FAIL;
	tUINT32 RamTestresult = 0;
	tUINT32 i = 0;
	unsigned char * source = 0, *dest = 0;
	tUINT32 * START_ADDRESS;
	tUINT32 * END_ADDRESS;
	tUINT32 Len = EndAddr - StartAddr;
	// RAM Stack Area Addresses.
	START_ADDRESS = (tUINT32 *) StartAddr;//0x10007C00;
	END_ADDRESS = (tUINT32 *) EndAddr;//0x10008000;

	__disable_irq();// Disable All Interrupts.
	source = (unsigned char*) (StartAddr);//(0x10007C00);
	dest = (unsigned char*) (SAFE_STARTADDR);//(SafeAreaStart);//
	// Store Current Stack pointer value.
	result = __get_MSP();
	// Copy Stack Area contents to Safe Area.
	for (i = 0; i < Len; i++)
	{
		*dest++ = *source++;
	}
	// Set Stack pointer to safe Area.
	__set_MSP(result - (StartAddr - SAFE_STARTADDR));//(0x10007C00 - 0x10004000));
	// We have saved the data and moved the stack pointer to safe area, we can
	//	do the RAM test for Stack area now.
	RamTestresult = RamTest(START_ADDRESS,END_ADDRESS);

	// Restore Stack area contents back to original place.
	dest = (unsigned char *) (StartAddr);//(0x10007C00);
	source = (unsigned char *) (SAFE_STARTADDR);//(SafeAreaStart);
	for (i = 0; i < Len; i++)
	{
		*dest++ = *source++;
	}
	// Set the Stack pointer back to where it was before we moved it.
	result = __get_MSP();
	__set_MSP(result + (StartAddr - SAFE_STARTADDR));//(0x10007C00 - 0x10004000));
	__enable_irq();// Enable Interrupts again.
	return RamTestresult;
}


/**************************************************************************//**
* 	\brief -	RAM_DATAAREA_TEST
*
* 	\brief -	RAM_DATAAREA_TEST is used to test the RAM Data area.
* 				Following steps are done during this test:
* 				1.> Disable Interrupts.
* 				2.> Copy the contents of Data area first to SafeArea.
* 				3.> Test the Data Area.
* 				4.> Restore the Data Area contents.
* 				5.> Enable Interrupts and return result.
*
* 	\param -	START_ADDRESS: Start Address of RAM.
*              	END_ADDRESS: End Address of RAM.
*                     
*	\return -      tUINT32 result  PASS/FAIL.
*
*****************************************************************************/
tUINT32 RAM_DATAAREA_TEST(tUINT32 StartAddr,tUINT32 EndAddr)
{
	tUINT32 RamTestresult = FAIL;
	tUINT32 i = 0;
	unsigned char * source = 0, *dest = 0;
	tUINT32 * START_ADDRESS;
	tUINT32 * END_ADDRESS;
	tUINT32 Len = EndAddr - StartAddr;
	// RAM Data Area Addresses.
	START_ADDRESS = (tUINT32 *) StartAddr;//0x10000000;
	END_ADDRESS = (tUINT32 *) EndAddr;//0x10000400;

	__disable_irq();// Disable Interrupts.
	// Copy Data Area contents to Safe Area.
	source = (unsigned char*) (StartAddr);//(0x10000000);
	dest = (unsigned char*) (SAFE_STARTADDR);//(SafeAreaStart);
	for (i = 0; i < Len; i++)
	{
		*dest++ = *source++;
	}
	// We have saved the data to safe area, we can
	//	do the RAM test for Data area now.
	RamTestresult = RamTest(START_ADDRESS,END_ADDRESS);
	// Restore Data area contents back to original place.
	dest = (unsigned char *) (StartAddr);//(0x10000000);
	source = (unsigned char *) (SAFE_STARTADDR);//(SafeAreaStart);
	for (i = 0; i < Len; i++)
	{
		*dest++ = *source++;
	}
	__enable_irq();//Enable Interrupts again.
	return RamTestresult;
}

static tUINT32 CurrentTest = BIST_REGISTER_TEST;// First Test is register test.

/**************************************************************************//**
* 	\brief -	BIST
*
* 	\brief -	BIST is used to test the Register, RAM, Program Counters,
* 				ADC and Test at run time.
* 				Total 16ms is required to complete one round of these tests.
* 				One by one test will be done on every or every other ms during these 16ms
* 				to minimize the CPU load.
*
* 	\param -	none
* 	
* 	\return -	tUINT32 result  PASS/FAIL.
*
*****************************************************************************/
tUINT32 BIST()
{
	tUINT32 result  = PASS;
	FUNCTION1(UnitTest, "BIST()");
	CHECKPOINT(UnitTest, BRANCH_1);
	switch(CurrentTest)
	{
		case BIST_REGISTER_TEST: // CPU Register test.
			CHECKPOINT(UnitTest, BRANCH_2);
			__disable_irq();// Disable Interrupts.
			result = RegisterTest();
			__enable_irq();//Enable Interrupts again.
			if(result == FAIL)
			{
				printf("REGISTER Test ");
				LUTestErrMask |= (1<<BIST_REGISTER_TEST);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_REGISTER_TEST);
			}
			break;
		case BIST_RAMSAFEAREA_TEST: // Test the RAM safe Area.
			CHECKPOINT(UnitTest, BRANCH_3);
			result = RAM_SAFEAREA_TEST(SAFE_STARTADDR,SAFE_ENDADDR);
			if(result == FAIL)
			{
				printf("RAM1 Test ");
				LUTestErrMask |= (1<<BIST_RAMSAFEAREA_TEST);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_RAMSAFEAREA_TEST);
			}
			break;
		case BIST_PC_TEST: // Program Counter Test.
			CHECKPOINT(UnitTest, BRANCH_4);
			result = ProgramCounterTest();
			if(result == PC_FAULT)
			{
				result = FAIL;
				LUTestErrMask |= (1<<BIST_PC_TEST);
			}
			else
			{
				result = PASS;
				LUTestErrMask &= ~(1<<BIST_PC_TEST);
			}
			if(result == FAIL)
				printf("ProgramCounter Test ");
			break;
		case BIST_RAMDATAAREA_TEST: // RAM Data Area Test.
			CHECKPOINT(UnitTest, BRANCH_5);
			result = RAM_DATAAREA_TEST(DATA_STARTADDR,DATA_ENDADDR);
			if(result == FAIL)
			{
				printf("RAM2 Test ");
				LUTestErrMask |= (1<<BIST_RAMDATAAREA_TEST);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_RAMDATAAREA_TEST);
			}
			break;
		case BIST_ADC_TEST: // ADC Test.
			CHECKPOINT(UnitTest, BRANCH_6);
			ADC_Test();
			if(ADCStatus == NOK)
			{
				result = FAIL;
				LUTestErrMask |= (1<<BIST_ADC_TEST);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_ADC_TEST);
			}
			if(result == FAIL)
				printf("ADC Test ");
			break;
		case BIST_RAMSTACKAREA_TEST: // RAM Stack Area Test.
			CHECKPOINT(UnitTest, BRANCH_7);
			result = RAM_STACKAREA_TEST(STACK_STARTADDR,STACK_ENDADDR);
			if(result == FAIL)
			{
				printf("RAM3 Test ");
				LUTestErrMask |= (1<<BIST_RAMSTACKAREA_TEST);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_RAMSTACKAREA_TEST);
			}
			break;
		case BIST_FLASH_TEST1: // FLASH Test.
			CHECKPOINT(UnitTest, BRANCH_8);
			result = FlashTest1();
			if(result == FAIL)
			{
				printf("FLASH Test ");
				LUTestErrMask |= (1<<BIST_FLASH_TEST1);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_FLASH_TEST1);
			}
			break;
		case BIST_FLASH_TEST2: // FLASH Test.
			CHECKPOINT(UnitTest, BRANCH_8);
			result = FlashTest2();
			if(result == FAIL)
			{
				printf("FLASH Test ");
				LUTestErrMask |= (1<<BIST_FLASH_TEST2);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_FLASH_TEST2);
			}
			break;
		case BIST_FLASH_TEST3: // FLASH Test.
			CHECKPOINT(UnitTest, BRANCH_8);
			result = FlashTest3();
			if(result == FAIL)
			{
				printf("FLASH Test ");
				LUTestErrMask |= (1<<BIST_FLASH_TEST3);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_FLASH_TEST3);
			}
			break;
		case BIST_FLASH_TEST4: // FLASH Test.
			CHECKPOINT(UnitTest, BRANCH_8);
			result = FlashTest4();
			if(result == FAIL)
			{
				printf("FLASH Test ");
				LUTestErrMask |= (1<<BIST_FLASH_TEST4);
			}
			else
			{
				LUTestErrMask &= ~(1<<BIST_FLASH_TEST4);
			}
			break;
		default: // Place Holder between two tests.
			CHECKPOINT(UnitTest, BRANCH_9);
			result = PASS;
			break;
	}
	if(LUTestErrMask !=0)
	{
		LUTestStatus = NOK;
	}
	else
	{
		LUTestStatus = OK;
	}
	CHECKPOINT(UnitTest, BRANCH_10);
	if((CurrentTest & 0xFF) >= BIST_END)
	{
		CHECKPOINT(UnitTest, BRANCH_12);
		CurrentTest = BIST_REGISTER_TEST;
	}
	else
	{
		CHECKPOINT(UnitTest, BRANCH_11);
		CurrentTest++;
	}

	return result;
}


/**************************************************************************//**
* \brief -		RamLUTest
*
* \brief -		RAM LOgic Unit Test at PowerUP.
*
* \param -		none
* 
* \return -	PASS or FAIL.
*
*****************************************************************************/
tSINT32 RamLUTest(void)
{
    tSINT32 result = PASS;
//#if UNIT_TESTING
//#else
    FUNCTION1(RamUnitTest, "RamLUTest()");
    CHECKPOINT(RamUnitTest, BRANCH_1);
    result = RAM_SAFEAREA_TEST(SAFE_STARTADDR,SAFE_ENDADDR);
    CHECKPOINT(RamUnitTest, BRANCH_2);
    FAULT(RamSafeTestPass,{result = PASS;});
    FAULT(RamSafeTestFail,{result = FAIL;});
    if(result == PASS)
    {
    	CHECKPOINT(RamUnitTest, BRANCH_4);
    	result = RAM_STACKAREA_TEST(STACK_STARTADDR,STACK_ENDADDR);
    	CHECKPOINT(RamUnitTest, BRANCH_5);
    	FAULT(RamStackTestPass,{result = PASS;});
    	FAULT(RamStackTestFail,{result = FAIL;});
    	if(result == PASS)
    	{
    		CHECKPOINT(RamUnitTest, BRANCH_7);
    		result = RAM_DATAAREA_TEST(DATA_STARTADDR,DATA_ENDADDR);
    		CHECKPOINT(RamUnitTest, BRANCH_8);
    		FAULT(RamDataTestPass,{result = PASS;});
    		FAULT(RamDataTestFail,{result = FAIL;});
       	if(result == PASS)
       	{
       		CHECKPOINT(RamUnitTest, BRANCH_10);
       	}
       	else
       		CHECKPOINT(RamUnitTest, BRANCH_9);
    	}
    	else
    		CHECKPOINT(RamUnitTest, BRANCH_6);
    }
    else
    {
    	CHECKPOINT(RamUnitTest, BRANCH_3);
    }
// #endif
    return result;
}

/**************************************************************************//**
* \brief -		LogicUnitTest
*
* \brief -		Logic Unit Test at PowerUP.
*
* \param -		none
* 
* \return -	PASS or FAIL.
*
*****************************************************************************/
tSINT32 LogicUnitTest(void)
{
    tSINT32 result = FAIL;
    FUNCTION1(UnitTest, "LogicUnitTest()");
    CHECKPOINT(UnitTest, BRANCH_1);
    result = RegisterTest();
    CHECKPOINT(UnitTest, BRANCH_2);
    FAULT(RegTestPass,{result = PASS;});
    FAULT(RegTestFail,{result = FAIL;});
    if(result)
    {
        CHECKPOINT(UnitTest, BRANCH_3);
        FlashTest1();
        FlashTest2();
        FlashTest3();
        result = FlashTest4();
        CHECKPOINT(UnitTest, BRANCH_5);
        FAULT(FlashTestPass,{result = PASS;});
        FAULT(FlashTestFail,{result = FAIL;});
        if (result)
        {
            CHECKPOINT(UnitTest, BRANCH_6);
            result = RamLUTest();
            CHECKPOINT(UnitTest, BRANCH_8);
            FAULT(RamTestPass,{result = PASS;});
            FAULT(RamTestFail,{result = FAIL;});
            if (result)
            {
                CHECKPOINT(UnitTest, BRANCH_9);
                result = ProgramCounterTest();
                CHECKPOINT(UnitTest, BRANCH_11);
                FAULT(PCTestPass,{result = PCOK;});
                FAULT(PCTestFail,{result = PC_FAULT;});
                if (result == PCOK)
                {
                    CHECKPOINT(UnitTest, BRANCH_12);
                    result = TestInterruptTimer(999);
                    CHECKPOINT(UnitTest, BRANCH_14);
                    FAULT(INTTestPass,{result = PASS;});
                    FAULT(INTTestFail,{result = FAIL;});
                    if(result)
                    {
                        CHECKPOINT(UnitTest, BRANCH_15);
                        ADC_Test();
                        CHECKPOINT(UnitTest, BRANCH_17);
                        FAULT(ADCStatusFillOK,{ADCStatus = OK;});
                        FAULT(ADCStatusFault,{ADCStatus = NOK;});
                        if(ADCStatus == OK)
                        {
                            CHECKPOINT(UnitTest, BRANCH_18);
                            result = PASS;
                        }
                        else
                        {
                            CHECKPOINT(UnitTest, BRANCH_19);
                            BEEPER = BEEPING;
                            NVIC->ICER[0] = NVIC_ICER0_DISABLE_INTERRUPTS;
                            NVIC->ICER[1] = NVIC_ICER1_DISABLE_INTERRUPTS;
                            result = FAIL;
                        }
                    }
                    else
                    {
                        CHECKPOINT(UnitTest, BRANCH_16);
                        BEEPER = BEEPING;
                        NVIC->ICER[0] = NVIC_ICER0_DISABLE_INTERRUPTS;
                        NVIC->ICER[1] = NVIC_ICER1_DISABLE_INTERRUPTS;
                        result = FAIL;
                    }
                }
                else
                {
                    CHECKPOINT(UnitTest, BRANCH_13);
                    BEEPER = BEEPING;
                    NVIC->ICER[0] = NVIC_ICER0_DISABLE_INTERRUPTS;
                    NVIC->ICER[1] = NVIC_ICER1_DISABLE_INTERRUPTS;
                    result = FAIL;
                }
            }
            else
            {
                CHECKPOINT(UnitTest, BRANCH_10);
                BEEPER = BEEPING;
                NVIC->ICER[0] = NVIC_ICER0_DISABLE_INTERRUPTS;
                NVIC->ICER[1] = NVIC_ICER1_DISABLE_INTERRUPTS;
                result = FAIL;
            }
        }
        else
        {
            CHECKPOINT(UnitTest, BRANCH_7);
            BEEPER = BEEPING;
            NVIC->ICER[0] = NVIC_ICER0_DISABLE_INTERRUPTS;
            NVIC->ICER[1] = NVIC_ICER1_DISABLE_INTERRUPTS;
            result = FAIL;
        }
    }
    else
    {
        CHECKPOINT(UnitTest, BRANCH_4);
        BEEPER = BEEPING;
        NVIC->ICER[0] = NVIC_ICER0_DISABLE_INTERRUPTS;
        NVIC->ICER[1] = NVIC_ICER1_DISABLE_INTERRUPTS;
        result = FAIL;
    }
    return result;
}

#if UNIT_TESTING
/**************************************************************************//**
* \brief -		ResetHandler
*
* \brief -		ResetHandler is the entry point for LUTest test unit testing.
*
* \param -		none
* 
* \return -	none
*
*****************************************************************************/
void ResetHandler(void)
{
    tSINT32 result = 0;
    int i,X;
    DiagInit();
    RamUnitTest = 1;
    UnitTest = 1;
    //Start RAMLUTest
    RamSafeTestPass = OK;
    RamSafeTestFail = NOK;
    RamStackTestPass = OK;
    RamStackTestFail = NOK;
    RamDataTestPass = OK;
    RamDataTestFail = NOK;
    result = RamLUTest();
    DiagPrintInt(result, TRUE);

    RamSafeTestPass = NOK;
    RamSafeTestFail = OK;
    result = RamLUTest();
    DiagPrintInt(result, FALSE);

    RamSafeTestPass = OK;
    RamSafeTestFail = NOK;
    RamStackTestPass = NOK;
    RamStackTestFail = OK;
    result = RamLUTest();
    DiagPrintInt(result, FALSE);

    RamSafeTestPass = OK;
	 RamSafeTestFail = NOK;
	 RamStackTestPass = OK;
	 RamStackTestFail = NOK;
	 RamDataTestPass = NOK;
	 RamDataTestFail = OK;
	 result = RamLUTest();
	 DiagPrintInt(result, FALSE);

    DiagSummary();
    ResetdiagTests();
    ResetdiagErrors();
    // End RAMLUTest

    //Start BIST Test
	 X = 0;
	 for(i = 0; i <= BIST_END; i++)
	 {
		BIST();
		X++;
		if(X > BIST_END)
			X = 0;
		DiagPrintInt(CurrentTest, X);
	 }
	 DiagSummary();
	 ResetdiagTests();
	 ResetdiagErrors();
	//End BIST Test

	 RamUnitTest = 0;
	 //Logic Unit Test pass 1
    RegTestPass = OK;
    RegTestFail = NOK;
    FlashTestPass = OK;
    FlashTestFail = NOK;
    RamTestPass = OK;
    RamTestFail = NOK;
    PCTestPass = OK;
    PCTestFail = NOK;
    INTTestPass = OK;
    INTTestFail = NOK;
    ADCStatusFillOK = OK;
    ADCStatusFault = NOK;
    result = LogicUnitTest();
    DiagPrintInt(result, TRUE);

    //Logic Unit Test pass 2
    RegTestPass = NOK;
    RegTestFail = OK;
    result = LogicUnitTest();
    DiagPrintInt(result, FALSE);

    //Logic Unit Test pass 3
    RegTestPass = OK;
    RegTestFail = NOK;
    FlashTestPass = NOK;
    FlashTestFail = OK;
    result = LogicUnitTest();
    DiagPrintInt(result, FALSE);

    //Logic Unit Test pass 4
    RegTestPass = OK;
    RegTestFail = NOK;
    FlashTestPass = OK;
    FlashTestFail = NOK;
    RamTestPass = NOK;
    RamTestFail = OK;
    result = LogicUnitTest();
    DiagPrintInt(result, FALSE);

    //Logic Unit Test pass 5
    RegTestPass = OK;
    RegTestFail = NOK;
    FlashTestPass = OK;
    FlashTestFail = NOK;
    RamTestPass = OK;
    RamTestFail = NOK;
    PCTestPass = NOK;
    PCTestFail = OK;
    result = LogicUnitTest();
    DiagPrintInt(result, FALSE);

    //Logic Unit Test pass 6
	 RegTestPass = OK;
	 RegTestFail = NOK;
	 FlashTestPass = OK;
	 FlashTestFail = NOK;
	 RamTestPass = OK;
	 RamTestFail = NOK;
	 PCTestPass = OK;
	 PCTestFail = NOK;
	 INTTestPass = NOK;
	 INTTestFail = OK;
	 result = LogicUnitTest();
	 DiagPrintInt(result, FALSE);

    //Logic Unit Test pass 7
    RegTestPass = OK;
    RegTestFail = NOK;
    FlashTestPass = OK;
    FlashTestFail = NOK;
    RamTestPass = OK;
    RamTestFail = NOK;
    PCTestPass = OK;
    PCTestFail = NOK;
    INTTestPass = OK;
    INTTestFail = NOK;
    ADCStatusFillOK = NOK;
    ADCStatusFault = OK;
    result = LogicUnitTest();
    DiagPrintInt(result, FALSE);

    //Logic Unit Test pass 8
//    RegTestPass = OK;
//    RegTestFail = NOK;
//    FlashTestPass = OK;
//    FlashTestFail = NOK;
//    RamTestPass = OK;
//    RamTestFail = NOK;
//    PCTestPass = OK;
//    PCTestFail = NOK;
//    INTTestPass = OK;
//    INTTestFail = NOK;
//    ADCStatusFillOK = OK;
//    ADCStatusFault = NOK;
//    result = LogicUnitTest();
//    DiagPrintInt(result, TRUE);

    DiagSummary();
}
#endif

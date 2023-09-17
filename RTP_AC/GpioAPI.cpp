
#include <vxWorks.h>        //for vxworks main
#include <iostream>

extern "C"
{
	#include <customSystemCall.h>
}

#include "GpioAPI.h"

using namespace std;

/****************************************************************************//**
*      Function    :   GpioInit
*      Description :   This function is to initialize GPIO
*      Input       :   
*      Output      :   GPIO_OK or GPIO_ERROR
*      Called by   :   
*******************************************************************************/
INT16 GpioInit(void)
{	
	INT16 stat = GPIO_ERROR;
	INT16 stat1 = GPIO_ERROR;
	UINT16 val;
	
	//They will be removed after clean out device tree.
	GpioFree(GPIO_4_16);
	GpioFree(GPIO_4_20);
	
	//Initial GPIO reset line
	stat1 = GpioAlloc(GPIO_1_5);
	stat1 |= GpioAlloc(GPIO_1_6);
	stat1 |= GpioAlloc(GPIO_1_7);
	
	stat1 |= GpioAlloc(GPIO_3_17);
	stat1 |= GpioAlloc(GPIO_3_20);
	
	stat1 |= GpioAlloc(GPIO_4_9);
	stat1 |= GpioAlloc(GPIO_4_10);
	stat1 |= GpioAlloc(GPIO_4_11);
	stat1 |= GpioAlloc(GPIO_4_12);
	stat1 |= GpioAlloc(GPIO_4_16);
	stat1 |= GpioAlloc(GPIO_4_17);
	stat1 |= GpioAlloc(GPIO_4_20);
	stat1 |= GpioAlloc(GPIO_4_24);
	stat1 |= GpioAlloc(GPIO_4_25);
	stat1 |= GpioAlloc(GPIO_4_28);
	stat1 |= GpioAlloc(GPIO_4_29);
	
	stat1 |= GpioAlloc(GPIO_5_4);
	stat1 |= GpioAlloc(GPIO_5_5);
	stat1 |= GpioAlloc(GPIO_5_6);
	stat1 |= GpioAlloc(GPIO_5_7);
	stat1 |= GpioAlloc(GPIO_5_9);
	stat1 |= GpioAlloc(GPIO_5_12);
	stat1 |= GpioAlloc(GPIO_5_19);
	stat1 |= GpioAlloc(GPIO_5_20);
	stat1 |= GpioAlloc(GPIO_5_23);
//	stat1 |= GpioAlloc(GPIO_5_25);
//	stat1 |= GpioAlloc(GPIO_5_26);
	stat1 |= GpioAlloc(GPIO_0_23);
	stat1 |= GpioAlloc(GPIO_0_22);
		
	if(GPIO_OK == stat1)
	{
		stat1 = GpioSetDir(GPIO_1_5, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_1_6, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_3_17, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_4_9, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_4_10, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_4_11, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_4_12, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_4_28, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_4_29, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_4, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_5, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_6, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_7, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_9, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_12, GPIO_DIR_INPUT);
		stat1 |= GpioSetDir(GPIO_5_23, GPIO_DIR_INPUT);
		
		stat1 |= GpioSetDir(GPIO_1_7, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_3_20, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_4_16, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_4_17, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_4_20, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_4_24, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_4_25, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_5_19, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_5_20, GPIO_DIR_OUTPUT);
//		stat1 |= GpioSetDir(GPIO_5_25, GPIO_DIR_OUTPUT);
//		stat1 |= GpioSetDir(GPIO_5_26, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_0_23, GPIO_DIR_OUTPUT);
		stat1 |= GpioSetDir(GPIO_0_22, GPIO_DIR_OUTPUT);
	}
	
	stat = stat1;

	return stat;
}

/****************************************************************************//**
*      Function    :   GpioSetVal
*      Description :   This function set the value for a GPIO output pin
*      Input       :   pinNum: which pin
*                  :   value:  High or Low
*      Output      :   GPIO_OK or GPIO_ERROR
*      Called by   :   
*******************************************************************************/
INT16 GpioSetVal(UINT16 pinNum, UINT16 value) 
{
	INT16 stat = GPIO_ERROR;
		
	if(GPIO_VALUE_HIGH == value)
	{
		stat = GpioSetValue(pinNum, GPIO_VALUE_HIGH);
	}
	else if(GPIO_VALUE_LOW == value)
	{
		stat = GpioSetValue(pinNum, GPIO_VALUE_LOW);		
	}
	
	return stat;
}

/****************************************************************************//**
*      Function    :   GpioGetVal
*      Description :   This function get the value from a GPIO input pin
*      Input       :   pinNum: which pin
*                  :   Pointer to val:  val is High or Low
*      Output      :   GPIO_OK or GPIO_ERROR
*      Called by   :   
*******************************************************************************/
INT16 GpioGetVal(UINT16 pinNum, UINT16* val) 
{
	INT16 stat = GPIO_ERROR;
	UINT16 inVal;
	
	inVal = GpioGetValue(pinNum);
	
	if((GPIO_VALUE_LOW == inVal)||(GPIO_VALUE_HIGH == inVal))
	{
		*val = inVal;		
		stat = GPIO_OK;
	}
	
	return stat;
}

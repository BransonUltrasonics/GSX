

#include <iostream>
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <vxWorks.h>        //for vxworks main
#include "GPIO.h"

extern "C"
{
	#include <customSystemCall.h>
}

using namespace std;

/**************************************************************************//**
* \brief   - This function is to be responsible for turning on Blue LED drove by GPIO 
*                  :   Need call GpioInit() first before call this function.
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
void BlueLedOn(void)
{		
	//initialize control to Blue LED
	GpioSetValue(GPIO_4_11, GPIO_VALUE_HIGH); 
}

/**************************************************************************//**
* \brief   - This function is to be responsible for turning off Blue LED drove by GPIO 
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
void BlueLedOff(void)
{	
	GpioSetValue(GPIO_4_11, GPIO_VALUE_LOW); 	
}

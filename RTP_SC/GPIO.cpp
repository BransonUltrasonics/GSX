
#include <vxWorks.h>        //for vxworks main
#include <iostream>
//#include <taskLib.h>        //for taskdelay()
//#include <subsys/gpio/vxbGpioLib.h> 
//#include "McSPI.h"
//#include <subsys/gpio/vxbGpioLib.h> 
//#include "McSPI.h"

extern "C"
{
	#include <customSystemCall.h>
}

#include "GPIO.h"

using namespace std;

/**************************************************************************//**
* \brief   - This function is to initialize GPIO lines.
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
INT16 GpioInit(void)
{	
	INT16 stat = GPIO_ERROR;
	UINT16 val;
	
	//Initial GPIO reset line
	//The followings are used in DKM, not RTP
//	stat = GpioAlloc(GPIO_4_0);
//	stat |= GpioAlloc(GPIO_4_1);
//	stat |= GpioAlloc(GPIO_4_2);
//	stat |= GpioAlloc(GPIO_4_3);
//	stat |= GpioAlloc(GPIO_4_4);
//	stat |= GpioAlloc(GPIO_4_5);
//	stat |= GpioAlloc(GPIO_4_6);
//	stat |= GpioAlloc(GPIO_4_7);
//	stat = GpioFree(GPIO_4_9);
	stat = GpioAlloc(GPIO_4_8);    //E4, O, /GPIO_IO_RESET
	stat = GpioAlloc(GPIO_4_11);    //D3, O, GPIO drive blue led
	stat |= GpioAlloc(GPIO_6_19);   //B26, I, /GPIO_INP_INT
	
	if (GPIO_OK == stat)
	{
		//The followings are used in DKM, not RTP
//		stat = GpioSetDir(GPIO_4_0, GPIO_DIR_INPUT);
//		stat |= GpioSetDir(GPIO_4_1, GPIO_DIR_INPUT);
//		stat |= GpioSetDir(GPIO_4_2, GPIO_DIR_INPUT);
//		stat |= GpioSetDir(GPIO_4_3, GPIO_DIR_INPUT);

//		stat |= GpioSetDir(GPIO_4_4, GPIO_DIR_OUTPUT);
//		stat |= GpioSetDir(GPIO_4_5, GPIO_DIR_OUTPUT);
//		stat |= GpioSetDir(GPIO_4_6, GPIO_DIR_OUTPUT);
//		stat |= GpioSetDir(GPIO_4_7, GPIO_DIR_OUTPUT);
		stat = GpioSetDir(GPIO_4_8, GPIO_DIR_OUTPUT);
//		stat |= GpioSetDir(GPIO_4_9, GPIO_DIR_OUTPUT);  //F5, O, GPIO_EXT_PWR
		stat |= GpioSetDir(GPIO_4_11, GPIO_DIR_OUTPUT); //Blue Led
		stat |= GpioSetDir(GPIO_6_19, GPIO_DIR_INPUT);  //B26, I, /GPIO_INP_INT
	}
	
	return stat;
}

/**************************************************************************//**
* \brief   - This function is to disable /IO_RESET pin,
* 			 which will be enable the devices connected
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
INT16 GpioIOResetDis(void)
{	
	INT16 stat = GPIO_ERROR;
	UINT16 val;

	val = GpioGetDir(GPIO_4_8);
	if(GPIO_DIR_OUTPUT == val)
	{
		//Disable /IO_RESET pin, which will be enable the devices connected
		stat = GpioSetValue(GPIO_4_8, GPIO_VALUE_HIGH); 
	}
	
	return stat;
}

/**************************************************************************//**
* \brief   - This function is to enable /IO_RESET pin,
* 			 which will be reset the devices connected
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
INT16 GpioIOResetEn(void)
{	
	INT16 stat = GPIO_ERROR;
	UINT16 val;

	val = GpioGetDir(GPIO_4_8);
	if(GPIO_DIR_OUTPUT == val)
	{
		//Enable /IO_RESET pin, which will reset the devices connected
		stat = GpioSetValue(GPIO_4_8, GPIO_VALUE_LOW); 
	}
	
	return stat;
}	

/**************************************************************************//**
* \brief   - This function is to get status of /IO_RESET pin
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
UINT16 GpioIOIsReset(void)
{	
	INT16 stat = GPIO_ERROR;
	UINT16 val;

	val = GpioGetValue(GPIO_4_8);
	if(LINE_LOW == val)
	{
		stat = GPIO_IS_RESET; 
	}
	else
	{
		stat = GPIO_IS_NOT_RESET; 	
	}
	
	return stat;
}	

/**************************************************************************//**
* \brief   - This function is to  enable powers v5_0EXT and V12_0EXT
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
INT16 GpioExtPwrEn(void)
{	
	INT16 stat = GPIO_ERROR;
	UINT16 val;

	val = GpioGetDir(GPIO_4_9);
	if(GPIO_DIR_OUTPUT == val)
	{
		//Enable powers v5_0EXT and V12_0EXT
		stat = GpioSetValue(GPIO_4_9, GPIO_VALUE_LOW); 
	}
	
	return stat;
}	

/**************************************************************************//**
* \brief   - This function is to disable powers v5_0EXT and V12_0EXT
*
* \param   - None.
*
* \return  - GPIO_OK or GPIO_ERROR.
*
******************************************************************************/
INT16 GpioExtPwrDis(void)
{	
	INT16 stat = GPIO_ERROR;
	UINT16 val;

	val = GpioGetDir(GPIO_4_9);
	if(GPIO_DIR_OUTPUT == val)
	{
		//Disable powers v5_0EXT and V12_0EXT
		stat = GpioSetValue(GPIO_4_9, GPIO_VALUE_HIGH); 
	}
	
	return stat;
}	

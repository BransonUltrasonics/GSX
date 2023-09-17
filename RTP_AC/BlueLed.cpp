

#include <iostream>
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <vxWorks.h>        //for vxworks main
#include "GPIOAPI.h"

extern "C"
{
	#include <customSystemCall.h>
}

using namespace std;


/****************************************************************************//**
*      Function    :   BlueLedTest
*      Description :   This function is responsible for testing Blue LED drove by GPIO 
*                  :   Need call GpioInit() first before call this function.
*      Input       :   void
*      Output      :   void
*      Called by   :   
*******************************************************************************/
void BlueLedTest(void)

{		
	//stat1 =  GpioInit();
		
	//Test Blue LED
	GpioSetVal(LATCH_OE, GPIO_VALUE_HIGH); //Enable 74HC574
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_LOW);
	GpioSetVal(LATCH_CLK, GPIO_VALUE_LOW); 
	MsDelay(1);
	GpioSetVal(LATCH_OE, GPIO_VALUE_LOW); //Enable 74HC574
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_HIGH); 
	while(1)
	{
	MsDelay(1);
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_HIGH); 
	GpioSetVal(LATCH_CLK, GPIO_VALUE_HIGH); //Load 74HC574 register 
	MsDelay(1);
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_LOW); //Enable 74HC574
	GpioSetVal(LATCH_CLK, GPIO_VALUE_LOW); //Load 74HC574 register 
	MsDelay(1);
	GpioSetVal(LATCH_CLK, GPIO_VALUE_HIGH); //Load 74HC574 register 
	MsDelay(1);
	GpioSetVal(LATCH_CLK, GPIO_VALUE_LOW); //Enable 74HC574
	}
}

/****************************************************************************//**
*      Function    :   BlueLedInit
*      Description :   This function is responsible for init Blue LED drove by GPIO 
*                  :   Need call GpioInit() first before call this function.
*      Input       :   void
*      Output      :   void
*      Called by   :   
*******************************************************************************/
void BlueLedInit(void)
{		
	//initialize control to Blue LED
	GpioSetVal(LATCH_OE, GPIO_VALUE_HIGH);   //Disable 74HC574
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_LOW);
	GpioSetVal(LATCH_CLK, GPIO_VALUE_LOW); 
}

/****************************************************************************//**
*      Function    :   BlueLedOn
*      Description :   This function is responsible for turning on Blue LED drove by GPIO 
*                  :   Need call GpioInit() first before call this function.
*      Input       :   void
*      Output      :   void
*      Called by   :   
*******************************************************************************/
void BlueLedOn(void)

{		
	GpioSetVal(LATCH_OE, GPIO_VALUE_LOW);    //Enable 74HC574
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_HIGH);//Set ON value
	GpioSetVal(LATCH_CLK, GPIO_VALUE_HIGH);  //Latch in
}

/****************************************************************************//**
*      Function    :   BlueLedOff
*      Description :   This function is responsible for turning off Blue LED drove by GPIO 
*      Input       :   void
*      Output      :   void
*      Called by   :   
*******************************************************************************/

void BlueLedOff(void)
{		
	//initialize control to Blue LED
	GpioSetVal(LATCH_CLK, GPIO_VALUE_LOW); 
	GpioSetVal(BLUE_LED_EN, GPIO_VALUE_LOW);
	GpioSetVal(LATCH_CLK, GPIO_VALUE_HIGH);  //Latch in
	GpioSetVal(LATCH_OE, GPIO_VALUE_HIGH);   //Disable 74HC574
}

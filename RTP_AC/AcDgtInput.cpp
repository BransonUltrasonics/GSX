

#include <iostream>
#include <vxWorks.h>        //for vxworks main
#include "SPI.h"
#include "AcDgtInput.h"
#include "AcControlTask.h"		
#include "GpioAPI.h"
#include "AcDgtOutput.h"

extern "C"
{
	#include <customSystemCall.h>
}

using namespace std;

//#define AC_DO_TEST

/* GPIO pin value definitions */
//#define GPIO_3_1 ID
#define GPIO_3_1             (0x61)   //B14, SPI1_LD0
#define GPIO_PIN_VAL_LOW       (0U)
#define GPIO_PIN_VAL_HIGH      (1U)
#define CH_DIN					1

/// temporary defines, may require to change names, since it is already presetn in vxbGpioLib.h
#define GPIO_DIR_INPUT          (0)
#define GPIO_DIR_OUTPUT         (1)
#define GPIO_VALUE_LOW          (0)
#define GPIO_VALUE_HIGH         (1)
#define GPIO_VALUE_INVALID      (0xff)
///

static void SmallDelay(UINT32 delayValue);
static void LoadData(void);
static void setItHigh(void);
static void setItLow(void);

static UINT8 txBuf[1]=  {0xff};
// Buffer containing the received data
static UINT8 rxBuf[1] = {0x0};
// Transfer length 
UINT32 transferLength;
//Hold Digital IO Input reading value
UINT8 dio_input_value;

#ifdef NEW_AC
/****************************************************************************//**
*      Function    :   DgtGetDinPort
*      Description :   This function is responsible for receiving Industrial digital IO inputs 
*      Input       :   
*      Output      :   the 16 bit port value
*      Called by   :   
*******************************************************************************/
INT16 DgtGetDinPort(UINT8* val)
{	
	INT16 stat = 1; //DIO_INPUTS_ERROR;
//	volatile UINT8 rxBuf1[1] = {0x0};
	
	//Get 5 channel inputs
	//Receive the serial output
	stat = McSPI_SnglTrnsfr_DIOInputs((UINT8 *)&txBuf[0], (UINT8 *)&rxBuf[0], sizeof(rxBuf));
		
	if (SPI_OK == stat)
	{
		//Don want whole port value, not the single line value
		*val = rxBuf[0];
//			printf("*val: 0x%x \n", *val);			
	}	
	else
		printf("SPI DIO Transfer \n");	
	
	return stat;
}

/*
**********************************************************************************

*      Function    :   AcDgtInputTask
*      Description :   AC Digital IO Inputs Task for receive Digital inputs
*      Input       :   No arguments
*      Output      :   No return type
*      Called by   :   AcMain

**********************************************************************************
*/

void AcDgtInputTask()
{
	INT16 stat = ERROR;
	UINT8      Val;

	#ifdef PRINT	
		LOG("------------------------ DgtInput Task spawned ------------------------\n\n");			
	#endif

	AcDgtInputTaskSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	McSPI::AcDgtInOutTaskMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

	while(TRUE)	
	{	
		// polling Digital IO Inputs each ms
		semTake(AcDgtInputTaskSem, WAIT_FOREVER);
		
		semTake(McSPI::AcDgtInOutTaskMutex, WAIT_FOREVER);
		
		stat = DgtGetDinPort(&Val);            //Digital Inputs receive the data from Digital inputs lines

		semGive(McSPI::AcDgtInOutTaskMutex);

		if (ERROR == stat)
		{
			LOG("Get DgtInput failed!!!\n");
		}
		else
		{
			AcSetDinValue(Val);
		}
	}			
}

/**************************************************************************//**
* \brief   - This function is responsible for setting input value.
*
* \param   - input value, high or low.
*
* \return  - none.
*
******************************************************************************/

void AcSetDinValue(UINT8 in_val)
{			
    dio_input_value = in_val;
}

/**************************************************************************//**
* \brief   - This function is responsible for getting input value.
*
* \param   - none.
*
* \return  - input value, high or low.
*
******************************************************************************/

UINT8 AcGetDinValue(void)
{			
    return dio_input_value;
}

#endif  //NEW_AC

/******************************************************************************
*      Function    :   AcDgtCtlInTask
*      Description :   This function is responsible for receiving Industrial digital IO input 
*      Input       :   void
*      Output      :   void
*      Called by   :   VxWorks OS scheduler
*******************************************************************************/
void AcDgtCtlInTask(void)
{		
	GpioAlloc(GPIO_3_1);    
	GpioSetDir(GPIO_3_1, GPIO_DIR_OUTPUT);

	SPI::Init();		

	setItHigh();
	SmallDelay(1);
	SPI::SingleTransfer(CH_DIN, (UINT8 *)&txBuf[0], (UINT8 *)&rxBuf[0], 1);
	setItLow();
}

/******************************************************************************
*      Function    :   SmallDelay
*      Description :   This function is responsible for small delaying 
*      Input       :   the number to delay
*      Output      :   void
*      Called by   :   The function of AcDgtCtlInTask()
*******************************************************************************/

void SmallDelay(UINT32 delayValue)
{
    volatile UINT32 delay1 = delayValue*100;
    while (delay1--) ;
    return;
}

/******************************************************************************
*      Function    :   LoadData
*      Description :   This function is responsible for setting up LD signal 
*      Input       :   void
*      Output      :   void
*      Called by   :   The function of AcDgtCtlInTask()
*******************************************************************************/
void LoadData(void)
{
    //set up LD signal high
	GpioSetValue(GPIO_3_1, GPIO_VALUE_LOW);
    SmallDelay(2);
    
    //set up LD signal low
    GpioSetValue(GPIO_3_1, GPIO_VALUE_HIGH);
    return;
}

UINT8 GetDigitalIn() {
	LoadData();
	SPI::SingleTransfer(CH_DIN, (UINT8 *)&txBuf[0], (UINT8 *)&rxBuf[0], 1);  
	return rxBuf[0];
}

void setItHigh() {
	
	GpioSetValue(GPIO_3_1, GPIO_VALUE_HIGH);
}

void setItLow() {

	GpioSetValue(GPIO_3_1, GPIO_VALUE_LOW);
}

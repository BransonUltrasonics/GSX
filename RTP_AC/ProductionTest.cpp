

#include <iostream>
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <vxWorks.h>        //for vxworks main
#include "SPI.h"
#include "GpioAPI.h"
#include "AcDgtInput.h"
#include "AcDgtOutput.h"
#include "BlueLed.h"
#include "ProductionTest.h"

extern "C"
{
	#include <customSystemCall.h>
}

using namespace std;

#ifdef DEBUG_SPI1
static void SmallDelay(UINT32 delayValue);
#endif


static UINT8 SpiDin_txBuf[1] = {0x0};
static UINT8 SpiDin_rxBuf[1] = {0x5A};
static UINT16 SpiDout_txBuf[1] = {0x0};
static UINT16 SpiDout_rxBuf[1] = {0x55AA};

//Digital IO loop test
static UINT16 Dout_tx[3] = {0xFEAA, 0xFD55, 0xFEAA};  //010101010B, 101010101B, 010101010B

static UINT8 Din_rx1[5] = {0x0, 0x0, 0x0, 0x0, 0x0};  



// Transfer length 
UINT32 transferLength;

/****************************************************************************//**
*      Function    :   McSPITestTask
*      Description :   This function is responsible for receiving Industrial digital IO input 
*      Input       :   void
*      Output      :   void
*      Called by   :   VxWorks OS scheduler
*******************************************************************************/
void ProductionTestTask(void)

{	
	INT16      stat = DIO_OUTPUTS_ERROR;
	UINT16      lineNum;                  //Which line is read 
	UINT8      inVal;
	int         i = 0, j = 0;
	UINT16 k = 1;

	stat =  GpioInit();
	stat = GpioSetVal(IO_POWER_EN, GPIO_VALUE_LOW);

#define DEBUG_SPI2

#ifdef DEBUG_SPI2
//Digital IO Output loop test
	GpioSetVal(UP_DIGIO_RESET, GPIO_VALUE_HIGH); //Enable the chips Max14913 and PCA9704
	MsDelay(1);
	
//	stat = McSPI_Init_DIOOut();
	stat = McSPI_Init_DIO();

	
	k++;
	
	while(1)  
	{
		MsDelay(1000);
		
		if (k <= 0x10)
		{
			AcDgtOutput::AcDgtSetDoutValue(k);
			k=k<<1;
		}
		else
		{
			k = 1;
		}				
	}
#endif	
	
	
//	BlueLedTest();
	BlueLedInit();
	MsDelay(1);
	BlueLedOn();	

//	MsDelay(100000);
	//BlueLedOff();	
	
#ifdef NEW_AC
	//Digital IO loop test	
	GpioSetVal(UP_DIGIO_RESET, GPIO_VALUE_LOW); //Disable the chips Max14913 and PCA9704
	MsDelay(1);
	GpioSetVal(UP_DIGIO_RESET, GPIO_VALUE_HIGH); //Enable the chips Max14913 and PCA9704

//	stat = McSPI_Init_DIOIn();
	stat = McSPI_Init_DIO();
	//SPI::Init();	
	
while(k<2)  //Four test patterns
{				
	DgtGetDinPort(&inVal);            //Digital Inputs receive the data from Digital Outputs lines
	Din_rx1[k] = inVal;
	printf("inVal: 0x%x \n", Din_rx1[k]);	
	k++;
}

#endif  //NEW_AC
    
#if(0)    
	//Digital Outputs Pattern 000000001B << i	
	i = 0;
	while(i < 11)
	{
		if (i < 9)
		{
		    DgtSetDoutValue(i, LINE_HIGH);   //SPI2 xmit data to Digital Outputs 
		}
		else  	//Digital Output line 9 loop to Digital Inputs line 10 and line 11
		{
			DgtSetDoutValue(8, LINE_HIGH);   //SPI2 xmit data to Digital Outputs 
		}
		
		DgtGetDinPort(&inVal);            //Digital Inputs receive the data from Digital Outputs lines
		Din_rx2[i] = inVal;
		i++;	
	}

	printf("Din_rx2[]: ");
	for (i = 0; i < 11; i++)
	{
		printf(" 0x%x ", Din_rx2[i]);
	}

//#endif
	
#ifdef DEBUG_SPI4
//Analog IO loop test
	GpioIOResetDis();   //Enable the chip TLV5604

	while(j < 10)  //Ten test patterns
	{
		while(i < 2)
		{
			AnlgDacInit();      //Initialize DAC TLV5604
			AnlgSetDoutValue(i, Aout_tx[i][j]);
			
			AnlgAdcInit();
			AnlgGetAinValue(i, &inVal);
			Ain_rx[i][j] = inVal;
			
			i++;	
		}
		j++;
	}	
	
    for (i = 0; i < 2; i++)
	{
    	printf("Ain_rx[][%d]: ", i);
        for (j = 0; j < 10; j++)
    	{
        	printf(" 0x%x ", Ain_rx[j][i]);
    	}
    	printf("\n");
	}
#endif	
	
#ifdef DEBUG_SPI1
	GpioIOResetDis();
	McSPI_Init_EthrntIP();
		
	while(i++ < 1)
	{
		McSPI_SnglTrnsfr_EthrntIP((UINT8 *)&SpiEip_txBuf[0], (UINT8 *)&SpiEip_rxBuf[0], sizeof(SpiEip_rxBuf));
	}
#endif
	
#ifdef DEBUG_SPI2
#ifdef DEBUG_SPI_DIGITAL_INPUTS
	SpiDin_txBuf[0] = 0xFFFF; //Enable 9 inputs
	//vxbGpioSetValue(GPIO_4_8, GPIO_VALUE_HIGH);   //Enable GPIO.
	GpioSetValue(GPIO_4_8, GPIO_VALUE_HIGH);   //Enable GPIO.

	McSPI_Init_DIOIn();
	
	//Setup 9 channel inputs
	//McSPI::SingleTransfer(SPI_PORT_2, SPI_DIGITAL_INPUTS_CH, (UINT8 *)&SpiDin_txBuf[0], (UINT8 *)&SpiDin_rxBuf[0], sizeof(SpiDin_rxBuf));	
	//Reset the shift register by writting 0x00 via SDIN, also disable all of interrupts.
	McSPI_SnglTrnsfr_DIOInputs((UINT8 *)&SpiDin_txBuf[0], (UINT8 *)&SpiDin_rxBuf[0], sizeof(SpiDin_rxBuf));
	while(i < 2)
	{
		//Receive the serial output
		McSPI_SnglTrnsfr_DIOInputs((UINT8 *)&SpiDin_txBuf[0], (UINT8 *)&SpiDin_rxBuf[0], sizeof(SpiDin_rxBuf));
		i++;
		
		inVal = SpiDin_rxBuf[0] & SC_DIN_MASK;
		inVal = inVal & 1 << lineNum;
	}
	//vxbGpioSetValue(GPIO_4_8, GPIO_VALUE_LOW);   //Reset IO, should in the program Initial routine.
	GpioSetValue(GPIO_4_8, GPIO_VALUE_LOW);   //Reset IO, should in the program Initial routine.
	
#endif

#ifdef DEBUG_SPI_DIGITAL_OUTPUTS
	//vxbGpioSetValue(GPIO_4_8, GPIO_VALUE_HIGH);   //Enable GPO .
	GpioSetValue(GPIO_4_8, GPIO_VALUE_HIGH);   //Enable GPO .
	
	McSPI_Init_DIOOut();

	//Tx 16 bits
	while(i++ < 1)
	{
		//McSPI::SingleTransfer(SPI_PORT_2, SPI_DIGITAL_OUTPUTS_CH, (UINT8 *)&txBuf2[0], (UINT8 *)&rxBuf2[0], sizeof(rxBuf2));	
		McSPI_SnglTrnsfr_DIOOutputs((UINT8 *)&SpiDout_txBuf[0], (UINT8 *)&SpiDout_rxBuf[0], sizeof(SpiDout_txBuf));
	}
#endif
#endif
	
#ifdef DEBUG_SPI4	
#ifdef DEBUG_SPI_ANALOG_INPUTS
	McSPI_Init_AIOIn();	
	while(i++ < 1)
	{
		McSPI_SnglTrnsfr_AIOInputs((UINT8 *)&SpiAin_txBuf[0], (UINT8 *)&SpiAin_rxBuf[0], sizeof(SpiAin_rxBuf));
	}
#endif

#ifdef DEBUG_SPI_ANALOG_OUTPUTS
	McSPI_Init_AIOOut();	

	while(i++ < 1)
	{
		McSPI_SnglTrnsfr_AIOOutputs((UINT8 *)&SpiAout_txBuf[0], (UINT8 *)&SpiAout_rxBuf[0], sizeof(SpiAout_txBuf));
	}
#endif	
#endif	
#endif	
}

#ifdef DEBUG_SPI1
/****************************************************************************//**
*      Function    :   SmallDelay
*      Description :   This function is responsible for small delaying 
*      Input       :   the number to delay
*      Output      :   void
*      Called by   :   The function of AcDgtCtlInTask()
*******************************************************************************/
//This is only for Am5728EVM board
void SmallDelay(UINT32 delayValue)
{
    volatile UINT32 delay1 = delayValue*10000;
    while (delay1--) ;
    return;
}
#endif

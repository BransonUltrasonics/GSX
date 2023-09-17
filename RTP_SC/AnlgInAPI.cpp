
#include <vxWorks.h>        //for vxworks main
#include <iostream>
//#include <taskLib.h>        //for taskdelay()
//#include <subsys/gpio/vxbGpioLib.h> 
//#include "McSPI.h"
//#include <subsys/gpio/vxbGpioLib.h> 
#include "McSPI.h"
#include "GPIO.h"

extern "C"
{
	#include <customSystemCall.h>
}

#include "AnlgInAPI.h"

using namespace std;

#ifdef DEBUG_SPI_ANALOG_INPUTS

#define ADC_CONFIG_CH_0    0x00
#define ADC_CONFIG_CH_1    0x40

enum
{
	ADC_CH_0 = 0,
	ADC_CH_1,
	ADC_CH_MAX
};

UINT8 Adc_Config[3] = {0x6, 0x0, 0x0};
UINT8 Adc_Reading[3] = {0x0, 0x0, 0x0};

static UINT16 SpiAin_txBuf[1] = {0xFFFF};
static UINT16 SpiAin_rxBuf[1] = {0x55AA};

/**************************************************************************//**
* \brief   - This function is responsible for initial configurate ADC
*
* \param   - None
*
* \return  - UINT16
*
*****************************************************************************/
INT16 AnlgAdcInit(void)
{	
	INT16 stat = AIO_INPUTS_ERROR;
	
	stat = McSPI_Init_AIOIn();

	if (SPI_OK == stat)
	{
		stat = McSPI_SnglTrnsfr_AIOInputs((UINT8 *)&Adc_Config[0], (UINT8 *)&Adc_Reading[0], sizeof(Adc_Config));
	}

	if (SPI_OK == stat)
	{
		stat = AIO_INPUTS_OK;		
	}

	return stat;
}

/**************************************************************************//**
* \brief   - This function is responsible for receiving Industrial Analog IO inputs
*
* \param   - line - which channel
*
* \return  - INT16
*
*****************************************************************************/
INT16 AnlgGetAinValue(UINT16 line, UINT16* val)
{	
	INT16 stat = AIO_INPUTS_ERROR;
	
	UINT16      inVal;

//	stat = AnlgAdcInit();

//	if (SPI_OK == stat)
//	{
		if ((SPI_OK == stat)&&(line < ADC_CH_MAX))
		{
			//Read ADC channel 
			if (ADC_CH_0 == line)
			{
				Adc_Config[1] = ADC_CONFIG_CH_0;  //config ADC channel 0						
			}
			else
			{
				Adc_Config[1] = ADC_CONFIG_CH_1;  //config ADC channel 1			
			}

			stat = McSPI_SnglTrnsfr_AIOInputs((UINT8 *)&Adc_Config[0], (UINT8 *)&Adc_Reading[0], sizeof(Adc_Reading));
			
			
			if (SPI_OK == stat)
			{
				inVal = Adc_Reading[1] & 0xf;
				inVal <<= 8;
				inVal |= Adc_Reading[2];
				*val = inVal;
			}			
		}
//	}

	if (SPI_OK == stat)
	{
		stat = AIO_INPUTS_OK;		
	}

	return stat;
}

#endif
   


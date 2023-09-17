
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

#include "AnlgOutAPI.h"

using namespace std;

#ifdef DEBUG_SPI_ANALOG_OUTPUTS

#define DAC_CONFIG_CH_A    0x0000
#define DAC_CONFIG_CH_B    0x4000
#define DAC_CONFIG_MASK    0x3FFF
#define DAC_CONFIG_PWR_NORM 0x0000
#define DAC_CONFIG_PWR_DOWN 0x2000
#define DAC_CONFIG_SPD_SLOW 0x0000
#define DAC_CONFIG_SPD_FAST 0x1000
#define DAC_MAX_VALUE       0x3FF

enum
{
	DAC_CH_A = 0,
	DAC_CH_B,
	DAC_CH_MAX
};

static UINT16 SpiAout_txBuf = 0;
static UINT16 SpiAout_rxBuf = 5555;
static UINT16 Dac_config    = 0;

/**************************************************************************//**
* \brief   - This function is responsible for initial configuration of DAC
*
* \param   - None
*
* \return  - UINT16
*
*****************************************************************************/
INT16 AnlgDacInit(void)
{	
	INT16 stat = AIO_OUTPUTS_ERROR;
	
	stat = McSPI_Init_AIOOut();

	if (SPI_OK == stat)
	{
		Dac_config = DAC_CONFIG_CH_A | DAC_CONFIG_PWR_NORM | DAC_CONFIG_SPD_FAST;
		SpiAout_txBuf |= Dac_config;
		stat = McSPI_SnglTrnsfr_AIOOutputs((UINT8 *)&SpiAout_txBuf, (UINT8 *)&SpiAout_rxBuf, sizeof(SpiAout_txBuf));
	}

	if (SPI_OK == stat)
	{
		stat = AIO_OUTPUTS_OK;		
	}

	return stat;
}

/**************************************************************************//**
* \brief   - This function is responsible for transmitting Industrial analog IO outputs
*
* \param   - line -- which channel
* 			 val -- what value to set
*
* \return  - UINT16
*
*****************************************************************************/
INT16 AnlgSetDoutValue(UINT16 line, UINT16 val)
{	
	INT16 stat = AIO_OUTPUTS_ERROR;

//	stat = AnlgDacInit();

//	if ((SPI_OK == stat)&&(line < DAC_CH_MAX)&&(val <= DAC_MAX_VALUE))
	if ((line < DAC_CH_MAX)&&(val <= DAC_MAX_VALUE))
	{
		//Set DAC channel 
		if (DAC_CH_A == line)
		{
			Dac_config = DAC_CONFIG_CH_A;  //config DAC channel A						
		}
		else
		{
			Dac_config = DAC_CONFIG_CH_B;  //config DAC channel B			
		}

		Dac_config |= DAC_CONFIG_PWR_NORM | DAC_CONFIG_SPD_FAST;
		SpiAout_txBuf = val << 2;
		SpiAout_txBuf |= Dac_config;
		stat = McSPI_SnglTrnsfr_AIOOutputs((UINT8 *)&SpiAout_txBuf, (UINT8 *)&SpiAout_rxBuf, sizeof(SpiAout_txBuf));					
	}

	if (SPI_OK == stat)
	{
		stat = AIO_OUTPUTS_OK;		
	}

	return stat;
}

#endif
   


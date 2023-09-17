#include <iostream>
#include <vxWorks.h>        //for vxworks main
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <string.h>         //for strcmp()
#include <taskLib.h>        //for taskdelay()
#include "SPI.h"
extern "C" 
{
#include <customSystemCall.h>
}


using namespace std;

SPI_HARDWARE* devConfig_FPGA ;
SPI_HARDWARE* devConfig_DIN ;
SPI_HARDWARE* devConfig_ADC ;
SPI_HARDWARE* devConfig;


SPI_TRANSFER* pPkg;
UINT8 FlagTest = FALSE;


#define MAX_STRING_LENGTH 			32
#define NUM_BITS_PERTRANSFER_16 	16
#define NUM_BITS_PERTRANSFER_8		8
#define NUM_BITS_PERTRANSFER_32		32
#define NUM_BITS_PERTRANSFER_64		64
#define SPI_ADC_SCK_FREQ			30000000
#define SPI_FPGA_CLK_FREQ			12000000   //12 Mhz 
#define SPI_DIN_CLK_FREQ			10000
#define SPI_DATA_LINES				1


	
/******************************************************************************
* \brief  - SPI Constructor
* 			This is the constructor of SPI class.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
SPI::SPI()
{
	
}


/******************************************************************************
* \brief  - SPI Destructor
* 			This is the destructor of SPI class.
*
* \param  - none
*
* \return - none
*
******************************************************************************/
SPI::~SPI()
{
	
}


/******************************************************************************
* \brief  - void SPI::Init
* 			Here the configuration of the SPI devices (FPGA,ADC and Digital inputs) 
*    		This function is called by PCControlTask
*
* \param  - none
*
* \return - none
*
******************************************************************************/
void SPI::Init(void)
{
	pPkg  = (SPI_TRANSFER *) MemAlloc (sizeof (SPI_TRANSFER));
	devConfig_FPGA  = (SPI_HARDWARE *) MemAlloc (sizeof (SPI_HARDWARE));
	devConfig_DIN  	= (SPI_HARDWARE *) MemAlloc (sizeof (SPI_HARDWARE));
	devConfig_ADC  	= (SPI_HARDWARE *) MemAlloc (sizeof (SPI_HARDWARE));
	
	devConfig  = (SPI_HARDWARE *) MemAlloc (sizeof (SPI_HARDWARE));
	
	//SPI configuration to FPGA
	devConfig_FPGA->bitWidth	=	NUM_BITS_PERTRANSFER_8;
	devConfig_FPGA->chipSelect	=	CS_0;
	devConfig_FPGA->devFreq		=	SPI_FPGA_CLK_FREQ;
	devConfig_FPGA->mode		= 	SPI_MODE_3| SPI_FULL_DUPLEX;   //SPI_CKPHA = 1 , SPI_CKPOL = 1
	devConfig_FPGA->dataLines	=	SPI_DATA_LINES;
	
	//SPI configuration to DIN
	devConfig_DIN->bitWidth		=	NUM_BITS_PERTRANSFER_16;
	devConfig_DIN->chipSelect	=	CS_0;
	devConfig_DIN->devFreq		=	SPI_DIN_CLK_FREQ;
	devConfig_DIN->mode			= 	SPI_MODE_0;
	devConfig_DIN->dataLines	=	SPI_DATA_LINES;
	
	//SPI configuration to ADC
	devConfig_ADC->bitWidth		=	NUM_BITS_PERTRANSFER_16;
	devConfig_ADC->chipSelect	=	CS_0;  
	devConfig_ADC->devFreq		=	SPI_ADC_SCK_FREQ;
	devConfig_ADC->mode			= 	SPI_FULL_DUPLEX;
	devConfig_ADC->dataLines	=	SPI_DATA_LINES;

}




/******************************************************************************
* \brief  - SPI SingleTransfer
*			This function sends and receives data from SPI port
*			
* \param  - Channel	: Indicates the Channel used to SPI the transfer 
* \param  - DataOut	: Contains the data the will be sent from uP to the external slave device
* \param  -	DataIn	: Holds the data sent from the external slave device to the uP
* \param  -	Length	: Size of the data will be sent
*
*
* \return - none
*
******************************************************************************/

void SPI::SingleTransfer(UINT8 Channel, UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{
	
		switch (Channel)
		{
			case FPGA_SPI_CHANNEL:
				devConfig = devConfig_FPGA;
				break;
			case DIN_SPI_CHANNEL:
				devConfig = devConfig_DIN;
				break;
			case ADC_SPI_CHANNEL:
				devConfig = devConfig_ADC;
				break;
			default:
				//Error
				break;
		}
		
		pPkg->rxLen= Length;
		pPkg->txLen= Length;
	
	
		pPkg->usDelay=1;   //1 microsecond
		pPkg->txBuf= (UINT8*)DataOut;
		pPkg->rxBuf= (UINT8*)DataIn; 
		McSpiTrans(devConfig, pPkg, Channel); 
}

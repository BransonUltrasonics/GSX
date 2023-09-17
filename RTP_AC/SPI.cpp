
#include <vxWorks.h>        //for vxworks main
#include <iostream>
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
SPI_HARDWARE* devConfig_ADC ;
SPI_HARDWARE* devConfig_DIN ;
SPI_HARDWARE* devConfig_DIO ;  //Digital IO, Slave 0 spi0_ss0 for Digital Inputs, Slave 3 spi0_ss3 for Digital Outputs

static SPI_TRANSFER* pPkg;
SEM_ID McSPI::AcDgtInOutTaskMutex = 0;
/*  SPI Constructor
 *
 *  Purpose:
 *   This is the constructor of SPI class.
 *
 *   Entry condition: None.
 *
 *   Exit condition: None.
 */
SPI::SPI()
{
	
}

/*  SPI Destructor
 *
 *  Purpose:
 *   This is the destructor of SPI class.
 *
 *   Entry condition: None.
 *
 *   Exit condition: None.
 */
SPI::~SPI()
{
	
}

/*  void SPI::Init(void)
 *
 *  Purpose :
 *    Here the configuration of the SPI devices (FPGA,ADC and Digital inputs) 
 *    This function is called by PCControlTask
 *    
 *  Entry Condition : None
 *
 *  Exit Condition : None
 */	
void SPI::Init(void)
{
	pPkg  = (SPI_TRANSFER *) vxbMemAlloc (sizeof (SPI_TRANSFER));

	devConfig_DIN  	= (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));

	devConfig_DIN->bitWidth		=	NUM_BITS_PERTRANSFER_8;
	devConfig_DIN->chipSelect	=	0;       	//Slave 0
	devConfig_DIN->devFreq		=	5000000;
	devConfig_DIN->mode			= 	1 | SPI_FULL_DUPLEX;
	devConfig_DIN->dataLines    =   1;	
}

/*  void SPI::SingleTransfer(UINT8 CS, UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
 *
 *  Purpose :
 *    This function sends and receives data from SPI port 
 *    
 *  Entry Condition : 
 *  	CS: Indicates the Chip select used to SPI transfer 
 *  	DataOut	: Contains the data the will be sent from uP to the external slave device
 *  	DataIn	: Holds the data sent from the external slave device to the uP
 *  	Length	: Size of the data will be sent
 *  	
 *  	This function is called by the FPGA, ADC and Digital inputs functions
 *
 *  Exit Condition : None
 */	
void SPI::SingleTransfer(UINT8 CS, UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{	
	pPkg->rxLen = Length;
	pPkg->txLen = Length;                                    //This forces to go to Rx in SPI device drivers
	pPkg->usDelay=1;
	
	pPkg->txBuf = (UINT8*)DataOut;  //Not useful for DigitalIO Inputs
    pPkg->rxBuf = (UINT8*)DataIn;  //Not useful for DigitalIO Inputs

	
    McSpiTrans(devConfig_DIN, pPkg, CS);
//	printf("txBuf: 0x%x ", pPkg->txBuf[0]);
//	printf("rxBuf: 0x%x\n\n ", pPkg->rxBuf[0]);
}

#ifdef NEW_AC
/**************************************************************************//**
* \brief    - This function is responsible for configure SPI port.
*
* \param    - Port: Indicates which SPI port is used to SPI transfer.
*
* \return   - None
*
******************************************************************************/
INT16 McSPI::Init(UINT8 Port)
{   
	INT16 stat = SPI_ERROR;

	pPkg = (SPI_TRANSFER *) vxbMemAlloc (sizeof (SPI_TRANSFER));

	switch(Port)
	{
		case SPI_DIGITAL_IO:
			devConfig_DIO = (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));
		
			devConfig_DIO->dataLines   =   1;	
			stat = OK;
			break;
	
		default:
			stat = SPI_ERROR;
			break;
	}
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of SPI::Init() for Digital IO.
*
* \param    - None.
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_Init_DIO(void)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::Init(SPI_DIGITAL_IO);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is responsible for configure SPI port.
*
* \param    - Port    : Indicates which SPI port is used to SPI transfer
* 			  \Slave   : Which slave in the port 
* 			  \DataOut : pointer to the data the will be sent from uP to the external slave device
* 			  \DataIn  : pointer to the data sent from the external slave device to the uP
* 			  \Length  : Size of the data will be sent
*
* \return   - None
*
******************************************************************************/
INT16 McSPI::SingleTransfer(UINT8 Port, UINT8 Slave, UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{	
	INT16 stat = SPI_ERROR;
	UINT16 SizeOfPackage=0;
    
	pPkg->rxLen = Length;
	pPkg->txLen = Length;                                    //This forces to go to Rx in SPI device drivers
	pPkg->usDelay=1;
	
	pPkg->txBuf = (UINT8*)DataOut;  //Not useful for DigitalIO Inputs
    pPkg->rxBuf = (UINT8*)DataIn;  //Not useful for DigitalIO Inputs
    
	switch(Port)
	{
		case SPI_DIGITAL_IO:
			if(SPI_DIGITAL_INPUTS_CH == Slave)
			{
				devConfig_DIO->chipSelect	= SPI_DIGITAL_INPUTS_CH;       //Slave 0, Digital Inputs
				devConfig_DIO->devFreq		= 5000000;
				devConfig_DIO->mode		= SPI_CLK_MODE1 | SPI_FULL_DUPLEX;
			    devConfig_DIO->bitWidth	    =	NUM_BITS_PERTRANSFER_8;
			}
			else if(SPI_DIGITAL_OUTPUTS_CH == Slave)
			{
				devConfig_DIO->chipSelect	= SPI_DIGITAL_OUTPUTS_CH;       //Slave 1, Digital Outputs	
				devConfig_DIO->devFreq		= 10000000;
				devConfig_DIO->mode		= SPI_CLK_MODE0 | SPI_FULL_DUPLEX;
			    devConfig_DIO->bitWidth	    =	NUM_BITS_PERTRANSFER_16;
//				printf("txBuf[1]: 0x%x\n ", pPkg->txBuf[1]);	
//				printf("txBuf[0]: 0x%x\n ", pPkg->txBuf[0]);	
//				printf("rxBuf[1]: 0x%x\n ", pPkg->rxBuf[1]);					   
//				printf("rxBuf[0]: 0x%x\n ", pPkg->rxBuf[0]);					   
			}
			
			stat = McSpiTrans(devConfig_DIO, pPkg, Port);

			break;

		default:
			stat = SPI_ERROR;
			break;
	}
	
	return stat;	
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::SingleTransfer() for Digital IO inputs.
*
* \param    - DataIn  : pointer to the data sent from the external slave device to the uP 
* 			  \Length  : Size of the data will be sent 
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_SnglTrnsfr_DIOInputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::SingleTransfer(SPI_DIGITAL_IO, SPI_DIGITAL_INPUTS_CH, DataOut, DataIn, Length);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::SingleTransfer() for Digital IO outputs.
*
* \param    - DataIn  : pointer to the data sent from the external slave device to the uP  
* 			  \Length  : Size of the data will be sent   
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_SnglTrnsfr_DIOOutputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::SingleTransfer(SPI_DIGITAL_IO, SPI_DIGITAL_OUTPUTS_CH, DataOut, DataIn, Length);
	
	return stat;
}

#endif  //#ifdef NEW_AC

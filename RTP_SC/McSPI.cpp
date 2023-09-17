
#include <iostream>
#include <vxWorks.h>        //for vxworks main
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <taskLib.h>        //for taskdelay()
#include "McSPI.h"

extern "C"
{
	#include <customSystemCall.h>
}

using namespace std;

SPI_HARDWARE* devConfig_SPI1 ;  //For EthernetIP
SPI_HARDWARE* devConfig_DIO ;   //Slave 0 cs0_2 for Digital Inputs, Slave 1 cs1_2 for Digital Outputs
SPI_HARDWARE* devConfig_SPI4 ;  //Slave 0 cs0_4 for Analog Inputs, Slave 1 cs1_4 for Analog Outputs

static SPI_TRANSFER* pPkg;
SEM_ID McSPI::ScDgtInOutTaskMutex = 0;
SEM_ID McSPI::ScDgtInTaskSem = 0;
SEM_ID McSPI::ScDgtOutTaskSem = 0;

/**************************************************************************//**
* \brief    - This is the constructor of SPI class, todo.
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
McSPI::McSPI()
{
}

/**************************************************************************//**
* \brief    - This is the destructor of SPI class, todo.
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
McSPI::~McSPI()
{
	
}

/**************************************************************************//**
* \brief    - This function is responsible for configure SPI port.
*
* \param    - Port: Indicates which SPI port is used to SPI transfer.
*
* \return   - None
*
******************************************************************************/
INT16 McSPI::Init(UINT8 Port, UINT8 Slave)
{   
	INT16 stat = SPI_ERROR;

	pPkg = (SPI_TRANSFER *) vxbMemAlloc (sizeof (SPI_TRANSFER));

	switch(Port)
	{
		case SPI_PORT_1:
			devConfig_SPI1 = (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));
		
			devConfig_SPI1->bitWidth	=	NUM_BITS_PERTRANSFER_8;
			devConfig_SPI1->chipSelect	=	SPI_SLAVE_0;       //Slave 0
			devConfig_SPI1->devFreq		=	10000000;   //50MHz for netX52
			devConfig_SPI1->mode		= 	SPI_CLK_MODE3 | SPI_FULL_DUPLEX;
			devConfig_SPI1->dataLines   =   1;	
			stat = SPI_OK;
			break;
			
		case SPI_DIGITAL_IO:
			devConfig_DIO = (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));
		
			devConfig_DIO->dataLines   =   1;	
			devConfig_DIO->bitWidth	   =	NUM_BITS_PERTRANSFER_16;
			break;
							
		case SPI_PORT_4:
			devConfig_SPI4 = (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));
		
			devConfig_SPI4->bitWidth	=	NUM_BITS_PERTRANSFER_8;
			devConfig_SPI4->mode		= 	SPI_FULL_DUPLEX;
			devConfig_SPI4->dataLines   =   1;

			if(SPI_ANALOG_INPUTS_CH == Slave)
			{
				devConfig_SPI4->bitWidth	=	NUM_BITS_PERTRANSFER_8;
				devConfig_SPI4->chipSelect	=	SPI_SLAVE_0;       //Slave 0, Analog Inputs
				devConfig_SPI4->devFreq		=	1000000;
				stat = SPI_OK;
			}
			else if(SPI_ANALOG_OUTPUTS_CH == Slave)
			{
				devConfig_SPI4->bitWidth	=	NUM_BITS_PERTRANSFER_16;
				devConfig_SPI4->chipSelect	=	SPI_SLAVE_1;       //Slave 1, Analog Outputs
				devConfig_SPI4->devFreq		=	20000000;
				stat = SPI_OK;
			}
			break;
	
		default:
			stat = SPI_ERROR;
			break;
	}
	
	// initialize synchronization semaphores
	ScDgtInOutTaskMutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
	ScDgtInTaskSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	ScDgtOutTaskSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

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
    
#ifdef DEBUG_SPI1    
    static int i = 0;
    printf("txBuf: ");
    for (i = 0; i < 8; i++)
	{
		printf(" 0x%x ", pPkg->txBuf[i]);
	}
	printf("\n");
#endif

	switch(Port)
	{
		case SPI_PORT_1:
			if(SPI_EHERNETIP_CH == Slave)
			{
				devConfig_SPI1->chipSelect	= SPI_EHERNETIP_CH;       
				stat = McSpiTrans(devConfig_SPI1, pPkg, Port);				
			}
			break;
		    
		case SPI_DIGITAL_IO:
			if(SPI_DIGITAL_INPUTS_CH == Slave)
			{
				devConfig_DIO->chipSelect	= SPI_DIGITAL_INPUTS_CH;       //Slave 0, Digital Inputs
				devConfig_DIO->devFreq		= 5000000;
				devConfig_DIO->mode		= SPI_CLK_MODE1 | SPI_FULL_DUPLEX;
			}
			else if(SPI_DIGITAL_OUTPUTS_CH == Slave)
			{
				devConfig_DIO->chipSelect	= SPI_DIGITAL_OUTPUTS_CH;       //Slave 1, Digital Outputs	
				devConfig_DIO->devFreq		= 10000000;
				devConfig_DIO->mode		= SPI_CLK_MODE0 | SPI_FULL_DUPLEX;
				}
			
			    stat = McSpiTrans(devConfig_DIO, pPkg, Port);
//#define SC_DIO_TEST
#ifdef SC_DIO_TEST				    
				printf("txBuf[1]: 0x%x\n ", pPkg->txBuf[1]);	
				printf("txBuf[0]: 0x%x\n ", pPkg->txBuf[0]);	
				printf("rxBuf[1]: 0x%x\n ", pPkg->rxBuf[1]);					   
				printf("rxBuf[0]: 0x%x\n ", pPkg->rxBuf[0]);					   
#endif //SC_DIO_TEST

			break;
			
			
		case SPI_PORT_4:
			if(SPI_ANALOG_INPUTS_CH == Slave)
			{
				devConfig_SPI4->chipSelect	= SPI_ANALOG_INPUTS_CH;       //Slave 0, Analog Inputs	
				devConfig_SPI4->devFreq		=	1000000;
				devConfig_SPI4->mode		= SPI_CLK_MODE0 | SPI_FULL_DUPLEX;
				stat = McSpiTrans(devConfig_SPI4, pPkg, Port);
			}
			else if(SPI_ANALOG_OUTPUTS_CH == Slave)
			{
				devConfig_SPI4->chipSelect	= SPI_ANALOG_OUTPUTS_CH;       //Slave 1, Analog Outputs				
				devConfig_SPI4->devFreq		=	20000000;
				devConfig_SPI4->mode		= SPI_CLK_MODE1 | SPI_FULL_DUPLEX;
				stat = McSpiTrans(devConfig_SPI4, pPkg, Port);
			}
			break;

		default:
			stat = SPI_ERROR;
			break;
	}
#ifdef DEBUG_SPI1
    static int j = 0;
    printf("rxBuf: ");
	//for (j = 0; j < 1560; i++)
	for (j = 0; j < 15; i++)
	{
		printf(" 0x%x ", pPkg->rxBuf[j]);
	}
	printf("\n");
#endif
#ifdef DEBUG_SPI   
	printf("rxBuf: 0x%x\n ", pPkg->rxBuf[i]);	
  
	printf("txBuf: 0x%x\n ", pPkg->txBuf[i]);	
#endif	
	
	return stat;	
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::Init() for EthernetIP.
*
* \param    - None.
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_Init_EthrntIP(void)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::Init(SPI_PORT_1, SPI_EHERNETIP_CH);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::Init() for Digital IO.
*
* \param    - None.
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_Init_DIO(void)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::Init(SPI_DIGITAL_IO, SPI_DIGITAL_INPUTS_CH);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::Init() for Analog IO Inputs.
*
* \param    - None.
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_Init_AIOIn(void)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::Init(SPI_PORT_4, SPI_ANALOG_INPUTS_CH);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::Init() for Analog IO outputs.
*
* \param    - None.
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_Init_AIOOut(void)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::Init(SPI_PORT_4, SPI_ANALOG_OUTPUTS_CH);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::SingleTransfer() for EthernetIP.
*
* \param    - DataIn  : pointer to the data sent from the external slave device to the uP
* 			  \Length  : Size of the data will be sent 
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_SnglTrnsfr_EthrntIP(UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::SingleTransfer(SPI_PORT_1, SPI_EHERNETIP_CH, DataOut, DataIn, Length);
	
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

/**************************************************************************//**
* \brief    - This function is API of McSPI::SingleTransfer() for Analog IO inputs.
*
* \param    - DataIn  : pointer to the data sent from the external slave device to the uP  
* 			  \Length  : Size of the data will be sent     
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_SnglTrnsfr_AIOInputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::SingleTransfer(SPI_PORT_4,SPI_ANALOG_INPUTS_CH, DataOut, DataIn, Length);
	
	return stat;
}

/**************************************************************************//**
* \brief    - This function is API of McSPI::SingleTransfer() for Analog IO outputs.
*
* \param    - DataIn  : pointer to the data sent from the external slave device to the uP   
* 			  \Length  : Size of the data will be sent t     
*
* \return   - Status: SPI_ERROR or SPI_OK
*
******************************************************************************/
INT16 McSPI_SnglTrnsfr_AIOOutputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{   
	INT16 stat = SPI_ERROR;

	stat = McSPI::SingleTransfer(SPI_PORT_4,SPI_ANALOG_OUTPUTS_CH, DataOut, DataIn, Length);
	
	return stat;
}


/**************************************************************************//**
* \brief    - 	Gives a semaphore for exclusive access to a shared resource
*				between the digital output and input tasks
*
* \param    - 	None
*
* \return   - 	None
*
******************************************************************************/
void McSPI::ScDgtInOutMutexGive()
{
	semGive (ScDgtInOutTaskMutex);
}


/**************************************************************************//**
* \brief    - 	Askes for a semaphore for exclusive access to a shared resource
*				between the digital output and input tasks
*
* \param    - 	None
*
* \return   - 	None
*
******************************************************************************/
void McSPI::ScDgtInOutMutexTake()
{
	semTake (ScDgtInOutTaskMutex, WAIT_FOREVER);
}


/**************************************************************************//**
* \brief    - 	Gives a semaphore to time the execution to access a shaared
*				resource between the digital output and input tasks.
*
* \param    - 	taskIndex task
*
* \return   - 	None
*
******************************************************************************/
void McSPI::ScDgtInOutSemGive(taskIndex task)
{
	switch (task)
	{
	case DGTIN_T:
		semGive(ScDgtInTaskSem);
		break;
	case DGTOUT_T:
		semGive(ScDgtOutTaskSem);
		break;
	default:
		break;
	}
}


/**************************************************************************//**
* \brief    - 	Asks for a semaphore to time the execution to access a shaared
*				resource between the digital output and input tasks.
*
* \param    - 	taskIndex task
*
* \return   - 	None
*
******************************************************************************/
void McSPI::ScDgtInOutSemTake(taskIndex task)
{
	switch (task)
	{
	case DGTIN_T:
		semTake(ScDgtInTaskSem, WAIT_FOREVER);
		break;
	case DGTOUT_T:
		semTake(ScDgtOutTaskSem, WAIT_FOREVER);
		break;
	default:
		break;
	}
}

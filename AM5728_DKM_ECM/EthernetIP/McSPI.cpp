
#include <iostream>
#include <vxWorks.h>        //for vxworks main
#include <stdio.h>          //for printf(),scanf(),fgets()
#include <string.h>         //for strcmp()
#include <taskLib.h>        //for taskdelay()
#include <usrLib.h>         //for adding demo in vxworks as a command in ui
#include <vxbMcSpiLib.h> 
#include <hwif/buslib/vxbSpiLib.h>		
#include <subsys/timer/vxbTimerLib.h> 
#include "McSPI.h"

using namespace std;

#define NUM_BITS_PERTRANSFER_16 16
#define NUM_BITS_PERTRANSFER_8 8
#define NUM_BITS_PERTRANSFER_32 32
//#define NUM_BITS_PERTRANSFER_8 7

SPI_HARDWARE* devConfig_SPI1 ;
SPI_HARDWARE* devConfig_SPI2 ;

static SPI_TRANSFER* pPkg;
	
/*  SPI Constructor
 *
 *  Purpose:
 *   This is the constructor of SPI class.
 *
 *   Entry condition: None.
 *
 *   Exit condition: None.
 */
McSPI::McSPI()
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
McSPI::~McSPI()
{
	
}

/*  void McSPI::Init(void)
 *
 *  Purpose :
 *    Here the configuration of the SPI devices (FPGA,ADC and Digital inputs) 
 *    This function is called by PCControlTask
 *    
 *  Entry Condition : None
 *
 *  Exit Condition : None
 */	
void McSPI::Init(void)
{
	pPkg  = (SPI_TRANSFER *) vxbMemAlloc (sizeof (SPI_TRANSFER));

	devConfig_SPI1  	= (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));

	devConfig_SPI1->bitWidth		=	NUM_BITS_PERTRANSFER_8;
	devConfig_SPI1->chipSelect	=	0;       //Slave 0
	//devConfig_SPI1->devFreq		=	10000;
	devConfig_SPI1->devFreq		=	10000000;   //50MHz for netX52
	devConfig_SPI1->mode		= 	3 | SPI_FULL_DUPLEX;
	devConfig_SPI1->dataLines    =   1;	
	
	devConfig_SPI2  	= (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));

	devConfig_SPI2->bitWidth		=	NUM_BITS_PERTRANSFER_8;
	devConfig_SPI2->chipSelect	=	0;       //Slave 0
	devConfig_SPI2->devFreq		=	10000;
	devConfig_SPI2->mode		= 	3 | SPI_FULL_DUPLEX;
	devConfig_SPI2->dataLines    =   1;	

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
void McSPI::SingleTransfer(UINT8 CS, UINT8 * DataOut, UINT8* DataIn, UINT16 Length)
{	
	UINT16 SizeOfPackage=0;
    static int i = 0;
    
	pPkg->rxLen = Length;
	pPkg->txLen = Length;                                    //This forces to go to Rx in SPI device drivers
	pPkg->usDelay=1;
	
	pPkg->txBuf = (UINT8*)DataOut;  //Not useful for DigitalIO Inputs
    pPkg->rxBuf = (UINT8*)DataIn;  //Not useful for DigitalIO Inputs
    
#ifdef DEBUG_SPI    
    printf("txBuf: ");
    for (i = 0; i < 8; i++)
	{
		printf(" 0x%x ", pPkg->txBuf[i]);
	}
	printf("\n");
#endif
	
	if(1==CS)
	{
		 vxbMcSpiTrans(devConfig_SPI1, pPkg, CS);
	}
	else if(2==CS)
		{
			 vxbMcSpiTrans(devConfig_SPI2, pPkg, CS); 
		}
#ifdef DEBUG_SPI
    printf("rxBuf: ");
	//for (i = 0; i < 1560; i++)
	for (i = 0; i < 15; i++)
	{
		printf(" 0x%x ", pPkg->rxBuf[i]);
	}
	printf("\n");
#endif	
}


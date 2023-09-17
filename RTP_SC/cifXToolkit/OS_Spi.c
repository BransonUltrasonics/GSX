/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: OS_SPICustom.c 12222 2018-07-26 07:03:23Z Robert $:

  Description:
    Implementation of the custom SPI abstration layer

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2018-07-26  Added return value to OS_SpiInit() and changed compile #error to
                #warning
    2014-08-27  created

**************************************************************************************/

/*****************************************************************************/
/*! \file OS_SPICustom.c
*    Sample SPI abstraction layer. Implementation must be done
*    according to used target system                                         */
/*****************************************************************************/

#include "OS_Spi.h"
#include "ToolkitSample.h"
#include "customSystemCall.h"
#include <vxworks.h>
#include <vxbus.h>
#include <subsys/gpio/vxbGpioLib.h>
#include <stdlib.h>
#include <stdio.h>
#include "cifXErrors.h"
#include <Logger.h>
/*****************************************************************************/
/*!  \addtogroup CIFX_TK_OS_ABSTRACTION Operating System Abstraction
*    \{                                                                      */
/*****************************************************************************/
//#define SPI_DEBUG

#define SPI_OK           0
#define SPI_ERROR       -1

#define vxbMemAlloc(size) calloc(1,size)

#define SPI_SLAVE_0       0

#define SPI_CLK_MODE0     0     // POL = 0, PHA = 0
#define SPI_CLK_MODE1     1     // POL = 0, PHA = 1
#define SPI_CLK_MODE2     2     // POL = 1, PHA = 0
#define SPI_CLK_MODE3     3     // POL = 1, PHA = 1

#define SPI_FULL_DUPLEX     0x20

#define NUM_BITS_PERTRANSFER_32 32
#define NUM_BITS_PERTRANSFER_16 16
#define NUM_BITS_PERTRANSFER_8 8

#define SPI_PORT_1        1

#define GPIO_CS  0xCA

static SPI_HARDWARE* devConfig_SPI1;
static SPI_TRANSFER* pPkg;
/*****************************************************************************/
/*! Initialize SPI components
*   \param pvOSDependent OS Dependent parameter passed by reference
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
INT32 OS_SpiInit(void** pvOSDependent)
{	
	INT32 lRet= CIFX_GENERAL_ERROR;
#ifdef SPI_DEBUG
    printf("OS_SpiInit\n");
#endif
  /* initialize SPI device */
    devConfig_SPI1 = (SPI_HARDWARE *) vxbMemAlloc (sizeof (SPI_HARDWARE));
    devConfig_SPI1->bitWidth	= NUM_BITS_PERTRANSFER_8;
    devConfig_SPI1->chipSelect	= SPI_SLAVE_0;       //Slave 0
    devConfig_SPI1->devFreq     = 10000000;   //50MHz for netX52
    devConfig_SPI1->mode	= SPI_CLK_MODE3 | SPI_FULL_DUPLEX;
    devConfig_SPI1->dataLines   = 1;	
    pPkg = (SPI_TRANSFER *) vxbMemAlloc (sizeof (SPI_TRANSFER));
    
    GpioAlloc(GPIO_CS);
    GpioSetDir(GPIO_CS, GPIO_DIR_OUTPUT);
//    GpioSetValue(GPIO_CS, GPIO_VALUE_HIGH);
//    while(1)
//    {
//	OS_Sleep(500);
//	GpioSetValue(GPIO_CS, GPIO_VALUE_LOW);
//	OS_Sleep(500);
//	GpioSetValue(GPIO_CS, GPIO_VALUE_HIGH);
//    };

    // Mutex creation for spi communication 
	if (NULL == (*pvOSDependent = OS_CreateMutex()))
	{	LOGERR("pvEIP_Mutex Invalid pointer\n", 0, 0, 0);
		lRet= CIFX_INVALID_POINTER;
	}
	else
	{
		lRet= CIFX_NO_ERROR;
	}

	return lRet;
}

/*****************************************************************************/
/*! Assert chip select
*   \param pvOSDependent OS Dependent parameter to identify card             */
/*****************************************************************************/
void OS_SpiAssert(void* pvOSDependent)
{
    GpioSetValue(GPIO_CS, GPIO_VALUE_LOW);
}

/*****************************************************************************/
/*! Deassert chip select
*   \param pvOSDependent OS Dependent parameter to identify card             */
/*****************************************************************************/
void OS_SpiDeassert(void* pvOSDependent)
{
    GpioSetValue(GPIO_CS, GPIO_VALUE_HIGH);
}

/*****************************************************************************/
/*! Lock the SPI bus
*   \param pvOSDependent OS Dependent parameter                              */
/*****************************************************************************/
void OS_SpiLock(void* pvOSDependent)
{
  /* lock access to SPI device */
	if ( 0 == OS_WaitMutex( pvOSDependent, WAIT_FOREVER))
		LOGERR("Fail OS_WaitMutex\n", 0, 0, 0);
}

/*****************************************************************************/
/*! Unlock the SPI bus
*   \param pvOSDependent OS Dependent parameter                              */
/*****************************************************************************/
void OS_SpiUnlock(void* pvOSDependent)
{
  /* unlock access to SPI device */
	OS_ReleaseMutex(pvOSDependent);
}

/*****************************************************************************/
/*! Transfer byte stream via SPI
*   \param pvOSDependent OS Dependent parameter to identify card
*   \param pbSend        Send buffer (NULL for polling)
*   \param pbRecv        Receive buffer (NULL if discard)
*   \param ulLen         Length of SPI transfer                              */
/*****************************************************************************/
void OS_SpiTransfer(void* pvOSDependent, uint8_t* pbSend, uint8_t* pbRecv, uint32_t ulLen)
{
    INT16 stat = SPI_ERROR;
#ifdef SPI_DEBUG
    printf("OS_SpiTransfer\n");
#endif
    pPkg->rxLen = ulLen;
    pPkg->txLen = ulLen;                                    //This forces to go to Rx in SPI device drivers
    pPkg->usDelay = 1;
    pPkg->txBuf = (UINT8*)pbSend;  //Not useful for DigitalIO Inputs
    pPkg->rxBuf = (UINT8*)pbRecv;  //Not useful for DigitalIO Inputs
    devConfig_SPI1->chipSelect	= SPI_SLAVE_0;       
    stat = McSpiTrans(devConfig_SPI1, pPkg, SPI_PORT_1);
    if(stat != SPI_OK)
    {
	printf("Spi transfer failed\n");
    }
    else
    {
#ifdef SPI_DEBUG
	if(pbSend != NULL)
	{
	    printf("Tx: ");
	    for(int i = 0; i < ulLen; i++)
	    {
		printf("%.2X ", pbSend[i]);
	    }
	    printf("\n");
	}
        if(pbRecv != NULL)
        {
	    printf("Rx: ");
	    for(int i = 0; i < ulLen; i++)
	    {
		printf("%.2X ", pbRecv[i]);
	    }
	    printf("\n");
        }
#endif
    }
}
/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

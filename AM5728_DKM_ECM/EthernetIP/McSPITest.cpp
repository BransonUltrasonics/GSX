

#include <iostream>
#include <vxWorks.h>        //for vxworks main
//#include <taskLib.h>        //for taskdelay()
//#include <subsys/gpio/vxbGpioLib.h> 
#include "McSPI.h"

using namespace std;

#ifdef DEBUG_SPI
static void SmallDelay(UINT32 delayValue);

//static UINT8 txBuf1[8]=  {0x80, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
static UINT32 txBuf1[2]=  {0x04000080, 0x00000000};
// Buffer containing the received data: 11 00 00 3F 6E 65 74 58
//static UINT32 rxBuf1[2] = {0x55555555,0x55555555};
static UINT32 rxBuf1[16] = {0x55555555,0x55555555};
#else
static UINT32 txBuf1[1560];
static UINT32 rxBuf1[1560];
#endif

// Transfer length 
UINT32 transferLength;

/******************************************************************************
*      Function    :   AcDgtCtlInTask
*      Description :   This function is responsible for receiving Industrial digital IO input 
*      Input       :   void
*      Output      :   void
*      Called by   :   VxWorks OS scheduler
*******************************************************************************/


//void spi(void)
void McSPITestTask(void)

{		
	int i=0;

	McSPI::Init();	

	while(i++ < 1)
	{
		McSPI::SingleTransfer(1, (UINT8 *)&txBuf1[0], (UINT8 *)&rxBuf1[0], sizeof(rxBuf1));
//		SmallDelay(1);	
	}
}

#ifdef DEBUG_SPI
/******************************************************************************
*      Function    :   SmallDelay
*      Description :   This function is responsible for small delaying 
*      Input       :   the number to delay
*      Output      :   void
*      Called by   :   The function of AcDgtCtlInTask()
*******************************************************************************/
void SmallDelay(UINT32 delayValue)
{
    volatile UINT32 delay1 = delayValue*10000;
    while (delay1--) ;
    return;
}
#endif

/*
 * SPI.h
 *
 *  Created on: Jun 16, 2017
 *      Author: hasanchez
 */

#ifndef SPI_H_
#define SPI_H_
#include "ProductionTest.h"

//These error definitions are compatible with the one of VxWorks SPI driver 
#define SPI_OK           0
#define SPI_ERROR       -1

#define SPI_PORT_0        0
#define SPI_PORT_1        1
#define SPI_PORT_2        2
#define SPI_DIGITAL_IO   SPI_PORT_0
#define SPI_SPARE        SPI_PORT_1
#define SPI_ADC          SPI_PORT_2

#define SPI_SLAVE_0       0
#define SPI_SLAVE_1       1
#define SPI_SLAVE_2       2
#define SPI_SLAVE_3       3

#define SPI_DIGITAL_INPUTS_CH  SPI_SLAVE_0  //slave 0
#define SPI_DIGITAL_OUTPUTS_CH SPI_SLAVE_3  //slave 3

#define SPI_CLK_MODE0     0     // POL = 0, PHA = 0
#define SPI_CLK_MODE1     1     // POL = 0, PHA = 1
#define SPI_CLK_MODE2     2     // POL = 1, PHA = 0
#define SPI_CLK_MODE3     3     // POL = 1, PHA = 1

#define SPI_FULL_DUPLEX     0x20
#define vxbMemAlloc(size) calloc(1,size)
#define NUM_BITS_PERTRANSFER_32 32
#define NUM_BITS_PERTRANSFER_16 16
#define NUM_BITS_PERTRANSFER_8 8

/* clock phase */
#define SPI_CKPHA           0x01
/* clock polarity */
#define SPI_CKPOL           0x02
#define SPI_MODE_0          (0 | 0)
#define SPI_MODE_1          (0 | SPI_CKPHA)
#define SPI_MODE_2          (SPI_CKPOL | 0)
#define SPI_MODE_3          (SPI_CKPOL | SPI_CKPHA)

class SPI
{
	public:
	
				SPI(); //Constructor
		virtual ~SPI(); //Destructor 
	
		static void Init();
		static void SingleTransfer(UINT8 CS, UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
};

#ifdef NEW_AC
class McSPI
{
	public:
	
				McSPI(); //Constructor
		virtual ~McSPI(); //Destructor 
	
		static INT16 Init(UINT8 Port);
		static INT16 SingleTransfer(UINT8 Port, UINT8 Slave, UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
		
		static SEM_ID AcDgtInOutTaskMutex;
};

extern INT16 McSPI_Init_DIO(void);
extern INT16 McSPI_SnglTrnsfr_DIOInputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
extern INT16 McSPI_SnglTrnsfr_DIOOutputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
#endif  //#ifdef NEW_AC

#endif /* SPI_H_ */

/*
 * SPI.h
 *
 *  Created on: Jun 16, 2017
 *      Author: hasanchez
 */

#ifndef MCSPI_H_
#define MCSPI_H_

// includes
#include "CommonProperty.h"

//These error definitions are compatible with the one of VxWorks SPI driver 
#define SPI_OK           0
#define SPI_ERROR       -1

//#define DEBUG_SPI
//#define DEBUG_SPI1
//#define DEBUG_SPI2
//#define DEBUG_SPI4

#define SPI_PORT_1        1
#define SPI_PORT_2        2
#define SPI_PORT_3        3
#define SPI_PORT_4        4

#define SPI_DIGITAL_IO   SPI_PORT_2

#define SPI_SLAVE_0       0
#define SPI_SLAVE_1       1
#define SPI_SLAVE_2       2
#define SPI_SLAVE_3       3
#define SPI_EHERNETIP_CH       SPI_SLAVE_0  //slave 0
#define SPI_DIGITAL_INPUTS_CH  SPI_SLAVE_0  //slave 0
#define SPI_DIGITAL_OUTPUTS_CH SPI_SLAVE_1  //slave 1
#define SPI_ANALOG_INPUTS_CH   SPI_SLAVE_0  //slave 0
#define SPI_ANALOG_OUTPUTS_CH  SPI_SLAVE_1  //slave 1

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

/* Task queue message IDs */
#define TO_OUTPUT_TASK_SET_OUTPUT 1

class McSPI
{
	public:
	
				McSPI(); //Constructor
		virtual ~McSPI(); //Destructor 
	
		static INT16 Init(UINT8 Port, UINT8 Slave);
		static INT16 SingleTransfer(UINT8 Port, UINT8 Slave, UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
		
		// Input output task synchronization
		static void ScDgtInOutMutexGive ();
		static void ScDgtInOutMutexTake ();
		static void ScDgtInOutSemGive 	(taskIndex task);
		static void ScDgtInOutSemTake 	(taskIndex task);
		
	private:
		// Input output task synchronization
		static SEM_ID ScDgtInOutTaskMutex;
		static SEM_ID ScDgtInTaskSem;
		static SEM_ID ScDgtOutTaskSem;
};

//APIs for SPI drivers
extern INT16 McSPI_Init_EthrntIP(void);
extern INT16 McSPI_Init_DIO(void);
extern INT16 McSPI_Init_AIOIn(void);
extern INT16 McSPI_Init_AIOOut(void);
extern INT16 McSPI_SnglTrnsfr_EthrntIP(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
extern INT16 McSPI_SnglTrnsfr_DIOInputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
extern INT16 McSPI_SnglTrnsfr_DIOOutputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
extern INT16 McSPI_SnglTrnsfr_AIOInputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
extern INT16 McSPI_SnglTrnsfr_AIOOutputs(UINT8 * DataOut, UINT8* DataIn, UINT16 Length);

#endif /* MCSPI_H_ */

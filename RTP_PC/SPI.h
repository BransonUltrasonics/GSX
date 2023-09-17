/*
 * SPI.h
 *
 *  Created on: Jun 16, 2017
 *      Author: hasanchez
 */

#ifndef SPI_H_
#define SPI_H_

#include<sys/types.h>
//Chip select defines
#define CS_0  		0
#define CS_1 		1
#define CS_2 		2
#define CS_3		3

/* clock polarity */
#define SPI_CKPHA           0x01
#define SPI_CKPOL           0x02

#define SPI_FULL_DUPLEX     0x20
#define SPI_MODE_0          (0 | 0)
#define SPI_MODE_3          (SPI_CKPOL | SPI_CKPHA)
#define MemAlloc(size) calloc(1, size)

//SPI channels
#define FPGA_SPI_CHANNEL	0
#define DIN_SPI_CHANNEL		1
#define ADC_SPI_CHANNEL		2


#define MAX_STRING_LENGTH 32
class SPI
{
	public:
	
				SPI(); //Constructor
		virtual ~SPI(); //Destructor 
		
		static void Init();
		static void SingleTransfer(UINT8 Channel, UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
		static SPI * ThisPtr;

};


#endif /* SPI_H_ */

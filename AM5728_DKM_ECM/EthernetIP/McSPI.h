/*
 * SPI.h
 *
 *  Created on: Jun 16, 2017
 *      Author: hasanchez
 */

#ifndef MCSPI_H_
#define MCSPI_H_

//#define SPI_DEBUG

class McSPI
{
	public:
	
				McSPI(); //Constructor
		virtual ~McSPI(); //Destructor 
	
		static void Init();
		static void SingleTransfer(UINT8 CS, UINT8 * DataOut, UINT8* DataIn, UINT16 Length);
};


#endif /* MCSPI_H_ */

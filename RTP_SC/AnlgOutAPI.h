/*
 * DgtOutAPI.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef ANLG__OUTPUTS_API_H
#define ANLG_OUTPUTS_API_H

#define DEBUG_SPI_ANALOG_OUTPUTS

#ifdef DEBUG_SPI_ANALOG_OUTPUTS

#define AIO_OUTPUTS_OK           0
#define AIO_OUTPUTS_ERROR       -1

INT16 AnlgDacInit(void);
INT16 AnlgSetDoutValue(UINT16 line, UINT16 val);

#endif  //#ifdef DEBUG_SPI_ANALOG_OUTPUTS

#endif  //ANLG_OUTPUTS_API_H


/*
 * DgtInAPI.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef ANLG_INPUTS_API_H
#define ANLG_INPUTS_API_H

#define DEBUG_SPI_ANALOG_INPUTS

#ifdef DEBUG_SPI_ANALOG_INPUTS
#define AIO_INPUTS_OK           0
#define AIO_INPUTS_ERROR       -1

INT16 AnlgAdcInit(void);
INT16 AnlgGetAinValue(UINT16 line, UINT16* val);

#endif

#endif  //ANLG_INPUTS_OUTPUTS_API_H


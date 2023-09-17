/*
 * ScDgtInput.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef DGT_INPUTS_API_H
#define DGT_INPUTS_API_H

#define DEBUG_SPI_DIGITAL_INPUTS
#ifdef DEBUG_SPI_DIGITAL_INPUTS

#define DIO_INPUTS_OK           0
#define DIO_INPUTS_ERROR       -1

INT16 ScDgtGetDinPort(UINT16* val);
void ScDgtSaveDinValue(UINT16 in_val);
UINT16 ScDgtGetDinValue(void);
void ScDgtInputTask(void);

#endif

#endif  //DGT_INPUTS_API_H


/*
 * ScDgtOutput.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef DGT_OUTPUTS_API_H
#define DGT_OUTPUTS_API_H

#define DIO_OUTPUTS_OK           0
#define DIO_OUTPUTS_ERROR       -1

#define  SC_DIN_MASK   0xff         //9 bits inputs

#define  DOUT_LINE_MIN                 1          //the line starts from 1, can't be 0 	
#define  DOUT_LINE_MAX                 8          //the last line is 8  	
#define  SC_DOUT_MASK               0xff          //8 bits outputs
#define  SC_DOUT_CONFIG_SET         0xff          //Push-Pull mode  

INT16 	ScDgtSetDout		(UINT16 val);
UINT16 	ScDoutGetValue		(void);
void 	ScDgtOutputTask		(void);

//For IO manager
INT16  	ScDgtSetDoutValue	(UINT16 line, UINT16 val);
UINT16 	ScDoutGetFault		(void);
UINT16 	ScDoutGetLevel		(void);

// output intertask synchronization
void 	ScDgtOutputMutexGive 	();
void 	ScDgtOutputMutexTake 	();


#endif  //DGT_OUTPUTS_API_H

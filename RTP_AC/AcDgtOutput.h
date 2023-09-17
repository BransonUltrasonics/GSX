/*
 * AcDgtOutAPI.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef AC_DGT_OUTPUTS_API_H
#define AC_DGT_OUTPUTS_API_H

#define DIO_OUTPUTS_OK           0
#define DIO_OUTPUTS_ERROR       -1

#define  DOUT_LINE_MIN            1          //the line starts from 1, can't be 0 	
#define  DOUT_LINE_MAX            5          //the last line is 5  	

#define  AC_DOUT_MASK          0x1f          //5 bits outputs
#define  AC_DOUT_CONFIG_SET    0x1f          //Push-Pull mode  


class AcDgtOutput
{
public:
	static INT16  AcDgtSetDoutValue(UINT16 val);
	static UINT16 AcDoutGetFault(void);
	static UINT16 AcDoutGetLevel(void);
	static void AcDgtOutputTask();	
	
private:
	static SEM_ID DgtOutputTaskSem;
	static UINT16 SpiDout_tx[1];
	static UINT16 SpiDout_rx[1];

	static UINT16 dio_output_value;
       
	static INT16 AcDgtSetDout(UINT16 val);
};


#endif  //AC_DGT_OUTPUTS_API_H

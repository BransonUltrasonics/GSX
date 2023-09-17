/*
 * GPIOAPI.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef GPIO_API_H
#define GPIO_API_H

#define GPIO_OK           0
#define GPIO_ERROR       -1

#define GPIO_PIN_VAL_LOW       (0U)
#define GPIO_PIN_VAL_HIGH      (1U)

#define GPIO_DIR_INPUT          (0)
#define GPIO_DIR_OUTPUT         (1)
#define GPIO_VALUE_LOW          (0)
#define GPIO_VALUE_HIGH         (1)

// GPIO pin value definitions 
//GPIO1
#define GPIO_1_4   (0x24)       //GPIO1[4]	B7	INPUT
#define GPIO_1_5   (0x25)		//GPIO1[5]	A7	INPUT	
#define GPIO_1_6   (0x26)		//GPIO1[6]	C8	INPUT	
#define GPIO_1_7   (0x27)		//GPIO1[7]	B8	OUTPUT
#define	GPIO_PC_15V_LOW         GPIO_1_4    //PC GPIO1_4 pin allocated for interrupt to Shutdown 15V
#define	GPIO_PC_5V_LOW          GPIO_1_5    //PC GPIO1_5 pin allocated for interrupt to Shutdown 5V
#define	GPIO_PC_24V_LOW         GPIO_1_6    //PC GPIO1_6 pin allocated for interrupt to Shutdown 24V
#define	GPIO_PC_FAN_EN          GPIO_1_6    //PC GPIO1_7 pin allocated for Fan Enable

//GPIO4	 	 	 	 	 	 
#define GPIO_4_20   (0x94)		//GPIO4[20]	AD23	OUTPUT		
#define GPIO_4_16   (0x90)		//GPIO4[16]	AD21	OUTPUT	 
#define GPIO_4_17   (0x91)		//GPIO4[17]	AE22	OUTPUT		 
#define GPIO_4_10   (0x8A)		//GPIO4[10]	AC23	INPUT		 
#define GPIO_4_11   (0x8B)		//GPIO4[11]	AE21	INPUT	
#define GPIO_4_12   (0x8C)		//GPIO4[12]	AC25	INPUT		 
#define GPIO_4_9    (0x89)		//GPIO4[9]	AD25	INPUT	
#define GPIO_4_24   (0x98)		//GPIO4[24]	Y18	
#define GPIO_4_28   (0x9C)		//GPIO4[28]	AE20	INPUT		 
#define GPIO_4_29   (0x9D)		//GPIO4[29]	AD20	INPUT		 
						
//GPIO5	 	 	 	 	 	 
#define GPIO_5_19   (0xB3)		//GPIO5[19]	AE18	OUTPUT		
#define GPIO_5_12   (0xAC)		//GPIO5[12]	E25	INPUT		 
#define GPIO_5_9    (0xA9)		//GPIO5[9]	F24	INPUT		 
#define GPIO_5_20   (0xB4)		//GPIO5[20]	AB18	OUTPUT	
//#define GPIO_5_25   (0xB9)		//GPIO5[25]	A10	OUTPUT		 
//#define GPIO_5_26   (0xBA)		//GPIO5[26]	B10	OUTPUT		 
#define GPIO_0_23   (0x17)		//GPIO0[23]	A10	OUTPUT		 
#define GPIO_0_22   (0x16)		//GPIO0[22]	B10	OUTPUT		 
#define GPIO_5_23   (0xB7)		//GPIO5[23]	D11	INPUT		 
#define GPIO_5_4    (0xA4)		//GPIO5[4]	P25	INPUT		 
#define GPIO_5_5    (0xA5)		//GPIO5[5]	R24	INPUT		 
#define GPIO_5_6    (0xA6)		//GPIO5[6]	P24	INPUT		 
#define GPIO_5_7    (0xA7)		//GPIO5[7]	N25	INPUT		 

//GPIO1	


						
//GPIO5	 	 	 	 	 	 
#define LED5            GPIO_5_19   //GPIO5[19]	AE18	OUTPUT	\LED5	
#define FAN_EN			GPIO_1_7
//GPIO5[7]	N25	INPUT	\ENLOG_ALARM3	 


INT16 GpioInit(void);
INT16 GpioSetVal(UINT16 pinNum, UINT16 value);
INT16 GpioGetVal(UINT16 pinNum, UINT16* val); 

#endif  //GPIO_API_H


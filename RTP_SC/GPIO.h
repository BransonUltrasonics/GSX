/*
 * GPIO.h
 *
 *  Created on: Nov. 30, 2017
 *      Author: FLi
 */


#ifndef GPIO_H
#define GPIO_H

#define GPIO_OK                (0)
#define GPIO_ERROR             (-1)

#define GPIO_IS_RESET          (1)
#define GPIO_IS_NOT_RESET      (0)

#define GPIO_PIN_VAL_LOW       (0U)
#define GPIO_PIN_VAL_HIGH      (1U)
#define GPIO_DIR_INPUT          (0)
#define GPIO_DIR_OUTPUT         (1)
#define GPIO_VALUE_LOW          (0)
#define GPIO_VALUE_HIGH         (1)
#define GPIO_VALUE_INVALID   (0xff)

typedef enum
	{
		LINE_LOW,
		LINE_HIGH
	} LINE_VALUE;

// GPIO pin value definitions 
#define GPIO_4_8             (0x68)   //E4, O, /GPIO_IO_RESET
#define GPIO_4_9             (0x69)   //F5, O, /GPIO_EXT_PWR	
#define GPIO_4_11            (0x6B)   //D3, O, GPIO drive blue led
#define GPIO_6_19            (0xB3)   //B26, I, /GPIO_INP_INT

	//The followings are used in DKM, not RTP
//#define GPIO_4_0             (0x80)   //G6, I, GPIO_POWERGOOD
//#define GPIO_4_1             (0x81)   //F2, I, GPIO_24V_LOW
//#define GPIO_4_2             (0x82)   //F3, I, GPIO_USB3_FLT
//#define GPIO_4_3             (0x83)   //D1, I, GPIO_USB2_FLT
//#define GPIO_4_4             (0x84)   //E2, O, GPIO_MEM_SEL
//#define GPIO_4_5             (0x85)   //D2, O, GPIO_ECAT_RESET
//#define GPIO_4_6             (0x86)   //F4, O, GPIO_ENET_RESET
//#define GPIO_4_7             (0x87)   //C1, O, GPIO_EMMC_RST
//#define GPIO_4_9             (0x89)   //F5, O, GPIO_EXT_PWR
//#define GPIO_POWERGOOD       GPIO_4_0     //G6, I, GPIO_POWERGOOD
//#define GPIO_24V_LOW         GPIO_4_1     //F2, I, GPIO_24V_LOW
//#define GPIO_USB3_FLT        GPIO_4_2     //F3, I, GPIO_USB3_FLT
//#define GPIO_USB2_FLT        GPIO_4_3     //D1, I, GPIO_USB2_FLT
//#define GPIO_MEM_SEL         GPIO_4_4     //E2, O, GPIO_MEM_SEL
//#define GPIO_ECAT_RESET      GPIO_4_5     //D2, O, GPIO_ECAT_RESET
//#define GPIO_ENET_RESET      GPIO_4_6     //F4, O, GPIO_ENET_RESET
//#define GPIO_EMMC_RST        GPIO_4_7     //C1, O, GPIO_EMMC_RST
#define GPIO_IO_RESET        GPIO_4_8       //E4, O, GPIO_IO_RESET      
#define GPIO_EXT_PWR         GPIO_4_9       //F5, O, GPIO_EXT_PWR
#define GPIO_BLUE_LED        GPIO_4_11      //D3, O, GPIO drive blue led	
#define GPIO_INP_INT         GPIO_6_19      //B26, I, Digital IO Inputs interrupt output

INT16 GpioInit(void);
INT16 GpioIOResetDis(void);
INT16 GpioIOResetEn(void);
UINT16 GpioIOIsReset(void);
INT16 GpioExtPwrEn(void);
INT16 GpioExtPwrDis(void);

#endif  //GPIO_H


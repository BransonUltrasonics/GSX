/*
 * FPGA.h
 *
 *  Created on: Jun 21, 2017
 *      Author: hasanchez
 */

#ifndef FPGA_H_
#define FPGA_H_

#include "Common.h"
#include "PowerUpRecipe.h"
#include<sys/types.h>
//FPGA GPO
#define NO_OUT					0
#define OVERLOADRESET_OUT		1
#define SONIC_ON_OUT			2

// FPGA inputs
#define FPGA_SIGN      4
#define OL_TEMPERATURE 8
#define OL_POWER       16
#define OL_VOLTAGE     32
#define OL_CURRENT     64
#define OVL_FUNCTION   128

//Additional defines for HD FPGA code
//FPGA control register bit definitions
#define FPGA_CTRL_BTM        BIT(0)//board test
#define FPGA_CTRL_BIT1       BIT(1)
#define FPGA_CTRL_BIT2       BIT(2)
#define FPGA_CTRL_BIT3       BIT(3)

#define FPGA_CTRL_INIT_DONE  BIT(4)
#define FPGA_CTRL_RUN        BIT(5)
#define FPGA_CTRL_BIT6       BIT(6)
#define FPGA_CTRL_BIT7       BIT(7)

#define FPGA_CTRL_BIT8       BIT(8)
#define FPGA_CTRL_BIT9       BIT(9)
#define FPGA_CTRL_BIT10      BIT(10)
#define FPGA_CTRL_BIT11      BIT(11)

#define FPGA_CTRL_BIT12      BIT(12)
#define FPGA_CTRL_BIT13      BIT(13)
#define FPGA_CTRL_BIT14      BIT(14)
#define FPGA_CTRL_BIT15      BIT(15)

//FPGA status register bit definitions

#define FPGA_STAT_BTM         BIT(0)
#define FPGA_STAT_BIT1        BIT(1)
#define FPGA_STAT_BIT2        BIT(2)
#define FPGA_STAT_BIT3        BIT(3)

#define FPGA_STAT_INIT_DONE   BIT(4)
#define FPGA_STAT_RUN         BIT(5)
#define FPGA_STAT_PWM_ZERO    BIT(6)
#define FPGA_STAT_SONIC_SAFE  BIT(7)

#define FPGA_STAT_FB_ERROR    BIT(8)
#define FPGA_STAT_ADC_ERROR   BIT(9)
#define FPGA_STAT_BIT10       BIT(10)
#define FPGA_STAT_BIT11       BIT(11)

#define FPGA_STAT_BIT12       BIT(12)
#define FPGA_STAT_BIT13       BIT(13)
#define FPGA_STAT_BIT14       BIT(14)
#define FPGA_STAT_BIT15       BIT(15)

//HD control reg
#define FPGA_HDCTRL_SONICON  BIT(0)
#define FPGA_HDCTRL_SEEK BIT(8)
#define FPGA_HDCTRL_SCAN BIT(9)
#define FPGA_HDCTRL_TEST_SEEKRAMP BIT(10)
#define FPGA_HDCTRL_TEST_WELDRAMP BIT(11)
#define FPGA_HDCTRL_WELD BIT(12)
//HD status reg
#define FPGA_HDSTAT_STDMODE_SONICSON  BIT(0)
#define FPGA_HDSTAT_SYNCMODE_SONICSON	BIT(1)
#define FPGA_HDSTAT_STDDDS_ERROR	BIT(4)
#define FPGA_HDSTAT_SYNCDDS_ERROR	BIT(5)
#define FPGA_HDSTAT_COMMON_ERROR	BIT(7)
#define FPGA_HDSTAT_ERROR1	BIT(8)
#define FPGA_HDSTAT_ERROR2	BIT(9)

#define MAX_PRE_INIT_PARAM 10
#define MAX_INIT_PARAM		15

#define FPGA_VERSION_1_0 		"3.xx1_bt"
#define FPGA_VERSION_DIGITS		4
#define MAX_FPGA_VERSION		255

#define FPGA_BASEADDRESS 0x0000

//FPGA map addresses
#define FPGA_TEST_ADDRESS 						(FPGA_BASEADDRESS + 0x00)
#define FPGA_TEST_INV_ADDRESS 					(FPGA_BASEADDRESS + 0x04)
#define FPGA_VER1_ADDRESS 						(FPGA_BASEADDRESS + 0x10)
#define FPGA_VER2_ADDRESS 						(FPGA_BASEADDRESS + 0x14)
#define FPGA_BRDTEST_ADDRESS 					(FPGA_BASEADDRESS + 0x22)
#define FPGA_CTRL_ADDRESS 						(FPGA_BASEADDRESS + 0x32)
#define FPGA_STAT_ADDRESS 						(FPGA_BASEADDRESS + 0x3A)
#define FPGA_HD_MODE_ADDRESS 					(FPGA_BASEADDRESS + 0x200)
#define FPGA_HD_BLIND_TIME_SEEK_ADDRESS 		(FPGA_BASEADDRESS + 0x204)
#define FPGA_HD_BLIND_TIME_WELD_ADDRESS 		(FPGA_BASEADDRESS + 0x208)
#define FPGA_HD_SWITCH_TIME_ADDRESS 			(FPGA_BASEADDRESS + 0x20C)
#define FPGA_HD_WELD_AMP_ADDRESS 				(FPGA_BASEADDRESS + 0x210)
#define FPGA_HD_PWM_PERCENT_ADDRESS 			(FPGA_BASEADDRESS + 0x214)
#define FPGA_HD_HW_DELAY_ADDRESS 				(FPGA_BASEADDRESS + 0x218)
#define FPGA_HD_FSYNC_DPLL_ADDRESS 				(FPGA_BASEADDRESS + 0x21C)
#define FPGA_HD_LTFREQ_CRTL1_ADDRESS 			(FPGA_BASEADDRESS + 0x220)
#define FPGA_HD_LTFREQ_CRTL2_ADDRESS 			(FPGA_BASEADDRESS + 0x224)
#define FPGA_HD_LTFREQ_CRTL3_ADDRESS 			(FPGA_BASEADDRESS + 0x228)
#define FPGA_HD_LTFREQ_CRTL4_ADDRESS 			(FPGA_BASEADDRESS + 0x22C)
#define FPGA_HD_NOT_USED1_ADDRESS 				(FPGA_BASEADDRESS + 0x230)
#define FPGA_HD_NOT_USED2_ADDRESS 				(FPGA_BASEADDRESS + 0x234)
#define FPGA_HD_HCTEST_ADDRESS 					(FPGA_BASEADDRESS + 0x238)
#define FPGA_GPO_ADDRESS 						(FPGA_BASEADDRESS + 0x30B)
#define FPGA_CTRL_HDX_ADDRESS 					(FPGA_BASEADDRESS + 0x312)
#define FPGA_FREQ_VAL_IN_ADDRESS 				(FPGA_BASEADDRESS + 0x320)
#define FPGA_PHASE_VAL_IN_ADDRESS 				(FPGA_BASEADDRESS + 0x326)
#define FPGA_PWM_VAL_ADDRESS 					(FPGA_BASEADDRESS + 0x32A)
#define FPGA_GPI_ADDRESS 						(FPGA_BASEADDRESS + 0x38B)
#define FPGA_READ_STAT_ADDRESS 					(FPGA_BASEADDRESS + 0x38E)
#define FPGA_STAT_HDX_ADDRESS 					(FPGA_BASEADDRESS + 0x392)
#define FPGA_FREQ_VAL_OUT_ADDRESS 				(FPGA_BASEADDRESS + 0x3A0)
#define FPGA_PHASE_VAL_OUT_ADDRESS 				(FPGA_BASEADDRESS + 0x3A6)
#define FPGA_PREINIT_PARAM0_ADDRESS 			(FPGA_BASEADDRESS + 0x100)
#define FPGA_PREINIT_PARAM1_ADDRESS 			(FPGA_BASEADDRESS + 0x104)
#define FPGA_PREINIT_PARAM2_ADDRESS 			(FPGA_BASEADDRESS + 0x108)
#define FPGA_PREINIT_PARAM3_ADDRESS 			(FPGA_BASEADDRESS + 0x10C)
#define FPGA_PREINIT_PARAM4_ADDRESS 			(FPGA_BASEADDRESS + 0x110)
#define FPGA_PREINIT_PARAM5_ADDRESS 			(FPGA_BASEADDRESS + 0x114)
#define FPGA_PREINIT_PARAM6_ADDRESS 			(FPGA_BASEADDRESS + 0x118)
#define FPGA_PREINIT_PARAM7_ADDRESS 			(FPGA_BASEADDRESS + 0x11C)
#define FPGA_PREINIT_PARAM8_ADDRESS 			(FPGA_BASEADDRESS + 0x120)
#define FPGA_PREINIT_PARAM9_ADDRESS 			(FPGA_BASEADDRESS + 0x124)
#define FPGA_DUMMY_ADDRESS 						(FPGA_BASEADDRESS + 0xFFF)

//Pre-init parameters default values

//20KHZ
#define PREINITPARA1_20KHZ_DEFAULT   	48
#define PREINITPARA2_20KHZ_DEFAULT   	1388
#define PREINITPARA3_20KHZ_DEFAULT   	1136
#define PREINITPARA4_20KHZ_DEFAULT   	950
#define PREINITPARA5_20KHZ_DEFAULT   	0
#define PREINITPARA6_20KHZ_DEFAULT		0	
#define PREINITPARA7_20KHZ_DEFAULT		0
#define PREINITPARA8_20KHZ_DEFAULT		0
#define PREINITPARA9_20KHZ_DEFAULT		0
#define PREINITPARA10_20KHZ_DEFAULT		0
//30KHZ
#define PREINITPARA1_30KHZ_DEFAULT   	48
#define PREINITPARA2_30KHZ_DEFAULT   	925
#define PREINITPARA3_30KHZ_DEFAULT   	757
#define PREINITPARA4_30KHZ_DEFAULT   	1024
#define PREINITPARA5_30KHZ_DEFAULT   	0
#define PREINITPARA6_30KHZ_DEFAULT		0	
#define PREINITPARA7_30KHZ_DEFAULT		0
#define PREINITPARA8_30KHZ_DEFAULT		0
#define PREINITPARA9_30KHZ_DEFAULT		0
#define PREINITPARA10_30KHZ_DEFAULT		0
//40KHZ
#define PREINITPARA1_40KHZ_DEFAULT   	48
#define PREINITPARA2_40KHZ_DEFAULT   	694
#define PREINITPARA3_40KHZ_DEFAULT   	568
#define PREINITPARA4_40KHZ_DEFAULT   	1024
#define PREINITPARA5_40KHZ_DEFAULT   	0
#define PREINITPARA6_40KHZ_DEFAULT		0	
#define PREINITPARA7_40KHZ_DEFAULT		0
#define PREINITPARA8_40KHZ_DEFAULT		0
#define PREINITPARA9_40KHZ_DEFAULT		0
#define PREINITPARA10_40KHZ_DEFAULT		0

//FPGA params define for HD
//Mode
#define HD_MODE_DEFAULT 0
//blind time seek ms
#define HD_BLIND_TIME_SEEK_DEFAULT 0			//10
//blind time weld ms
#define HD_BLIND_TIME_WELD_DEFAULT 0			//10
//dds switch time ms
#define HD_SWITCH_TIME_20K_DEFAULT	500
#define HD_SWITCH_TIME_30K_DEFAULT 	200
#define HD_SWITCH_TIME_40K_DEFAULT	200


//Weld Amplitude percent
#define HD_WELD_AMP_DEFAULT 100
//fixed PWM % for weld
#define HD_PWM_DEFAULT 30

//Mode
#define HD_MODE_MIN 0
//blind time seek ms
#define HD_BLIND_TIME_SEEK_MIN 0
//blind time weld ms
#define HD_BLIND_TIME_WELD_MIN 0
//dds switch time ms
#define HD_SWITCH_TIME_MIN 0
//Weld Amplitude(copy of preset WeldAmplitude)
#define HD_WELD_AMP_MIN 10
//fixed PWM % for weld
#define HD_PWM_MIN 10


//Mode
#define INITPARA1_MAX 3
//blind time seek ms
#define INITPARA2_MAX  1000
//blind time weld ms
#define INITPARA3_MAX 1000
//dds switch time ms
#define INITPARA4_MAX 10000
//Weld Amplitude(Copy of preset WeldAmplitude)
#define INITPARA5_MAX 100
//fixed PWM % for weld
#define INITPARA6_MAX 100

#define HD_HW_DELAY_20K_DEFAULT			0
#define HD_HW_DELAY_30K_DEFAULT			0
#define HD_HW_DELAY_40K_DEFAULT			180

#define HD_FSYNC_DPLL_20K_DEFAULT 		0
#define HD_FSYNC_DPLL_30K_DEFAULT 		2
#define HD_FSYNC_DPLL_40K_DEFAULT 		2

#define HD_LT_FREQ_CTRL1_20K_DEFAULT		800
#define HD_LT_FREQ_CTRL1_30K_DEFAULT		1200
#define HD_LT_FREQ_CTRL1_40K_DEFAULT		1600

#define HD_LT_FREQ_CTRL2_20K_DEFAULT		1000
#define HD_LT_FREQ_CTRL2_30K_DEFAULT		1000
#define HD_LT_FREQ_CTRL2_40K_DEFAULT		1000

#define HD_LT_FREQ_CTRL3_20K_DEFAULT		200
#define HD_LT_FREQ_CTRL3_30K_DEFAULT		200
#define HD_LT_FREQ_CTRL3_40K_DEFAULT		200

#define HD_LT_FREQ_CTRL4_20K_DEFAULT		5
#define HD_LT_FREQ_CTRL4_30K_DEFAULT		5
#define HD_LT_FREQ_CTRL4_40K_DEFAULT		10

#define HD_NOT_USED1_20K_DEFAULT		0
#define HD_NOT_USED1_30K_DEFAULT		0
#define HD_NOT_USED1_40K_DEFAULT		0

#define HD_NOT_USED2_20K_DEFAULT		0
#define HD_NOT_USED2_30K_DEFAULT		0
#define HD_NOT_USED2_40K_DEFAULT		0

#define HD_HC_TEST_DEFAULT		928

#define INIT_DONE_MAX_RETRIES	100
#define TEST_REGISTER_PATTERN	0x5A5A5A5A

enum FPGA_INX
{
	INX_TEST = 0,
	INX_TEST_INV,
	INX_FPGA_VER1,
	INX_FPGA_VER2,
	INX_FPGA_BRDTEST,
	INX_FPGA_CTRL,
	INX_FPGA_STAT,
	INX_PWRUP_DUMMY,
	
	INX_HD_MODE,
	INX_HD_BLIND_TIME_SEEK,
	INX_HD_BLIND_TIME_WELD,
	INX_HD_SWITCH_TIME,
	INX_HD_WELD_AMP,
	INX_HD_PWM_PERCENT,
	INX_HD_HW_DELAY,
	INX_HD_FSYNC_DPLL,
	INX_HD_LTFREQ_CRTL1,
	INX_HD_LTFREQ_CRTL2,
	INX_HD_LTFREQ_CRTL3,
	INX_HD_LTFREQ_CRTL4,
	INX_HD_NOT_USED1,
	INX_HD_NOT_USED2,
	INX_HD_HCTEST,
	
	INX_GPO,
	INX_CTRL_HDX,
	INX_FREQ_VAL_IN_HI,
	INX_PHASE_VAL_IN,
	INX_PWM_VAL,
	
	INX_GPI,
	INX_RFPGA_STAT,
	INX_STAT_HDX,
	INX_FREQ_VAL_OUT,
	INX_PHASE_VAL_OUT,
	
	INX_PREINIT_PARAM0,
	INX_PREINIT_PARAM1,
	INX_PREINIT_PARAM2,
	INX_PREINIT_PARAM3,
	INX_PREINIT_PARAM4,
	INX_PREINIT_PARAM5,
	INX_PREINIT_PARAM6,
	INX_PREINIT_PARAM7,
	INX_PREINIT_PARAM8,
	INX_PREINIT_PARAM9,
	
	INX_DUMMY
};

typedef struct 
{
									//TestRegister, Write,- ReadBack RW                                          
	UINT32 TestRegister;         //Address = 0, Data =0x5A5A
							//TestRegisterInv, Write,- Inverted ReadBack RW
	UINT32 TestRegisterInv;      // Address = 4, Data = A5A5
										//FpgaVersion, FPGA-Code-Version RO
	UINT32 FpgaVersion1;         // Address = 0x10                                           
	UINT32 FpgaVersion2;         // Address = 0x14
    							        //BoardTestMode1, BoardTestMode RW
	UINT16 BoardTestMode;        // Address = 0x22,                
											//FpgaCtrl, FPGA Control Register  RW
	UINT16 FPGACtrl;             // Address = 0x32,  At power up and start of a cycle          
										//FpgaStat, FPGA Status Register   RO
	UINT16 FPGAStat;            // Address = 0x3A,checked at power up and when the initialization is done
} FPGA_POWERUP;


typedef union 
{
	 struct 
	{
		UINT32 Mode;  
		UINT32 BlindTimeSeek;
		UINT32 BlindTimeWeld;
		UINT32 SwitchTime;
		UINT32 WeldAmplitude;
		UINT32 PwmPercentage;
		UINT32 HwDelay;
		UINT32 FsyncDpll;
		UINT32 LtFreqCtrl1;
		UINT32 LtFreqCtrl2;
		UINT32 LtFreqCtrl3;
		UINT32 LtFreqCtrl4;
		UINT32 NotUsed1;
		UINT32 NotUsed2;
		UINT32 HcTest;
		UINT16 Dummy;
		
	}HDParam;
	
	UINT32 Parameter[MAX_INIT_PARAM];
}FPGA_PARAMS;



typedef struct
{
	UINT8 GPO;				// Address 0x30C                           
	UINT16 CtrlHDX;			// Address 0x313                 
	UINT32 FrequencyValIn;	// Address  0x321   
	UINT16 PhaseValIn;		// Address 0x327   
	UINT16 PWMVal;			// Address 0x32B
}STWCYC;                                


typedef struct
{
	UINT8 GPI;                  //RO- FPGAinput Address 0x38B
	UINT16 FPGAStat;           //RO Address 0x38E
	UINT16 StatHDX  ;          // RO Address 0x392
	UINT32 FrequencyValOut;  // RO Address 0x39F
	UINT16 PhaseValOut;         // RO Address 0x3a2
}STRCYC;                        

	

typedef struct
{
	UINT16 Index;		
	UINT16 Address;	  	//Address to write the parameter to FPGA	
	UINT16 Length;	  	//Size of the parameter	
	UINT8 * Source;   	//Pointer to current parameter 
} FPGA_MAP;



typedef union {
  UINT32 TxPkg[WORDS_ON_SIZE64];
  UINT8 TxPkgByte[BYTES_ON_SIZE64];
  struct {            
	UINT32 Parity       :8;
	UINT32 TxAddr       :12;
	UINT32 RxAddr       :12;
    UINT32 Data;   
  } Tx;
} MOSI;


typedef union {
  UINT32 RxPkg[WORDS_ON_SIZE64];
  UINT8  RxPkgByte[BYTES_ON_SIZE64];
  struct {  
	UINT32  LeadZero    :16;
	UINT32 	Error      	:8;
	UINT32 	Parity      :8;
    UINT32 	Data;   
  } Rx;
} MISO;

enum
{
	STOP = 0,
	START,
	RUN,
	SCAN
};

class FPGA
{
	public:
		FPGA();
		virtual ~FPGA();
		
		static void Init();
		static void UpdateFPGAInitParameters(PowerUpDataParam& powerUpRecipe);
		//static void UpdateFPGAInitParameters();
		static void StartSonic(void);
		static void StopSonic(void);	
		static void  SPITransfer(FPGA_INX IndexRx, FPGA_INX IndexTx);
		static UINT8 PowerUpSequence(void);
		static void EnableSpiXfer(void);
		static void DisableSpiXfer(void);
		static UINT8 Re_Init();
		static UINT8 VerifyFPGAComm();
		
		//Set functions
		static void SetFrequency(UINT32 Freq);
		static void SetPhase(UINT16 Phase);
		static void SetPwmVal(UINT16 Pwm);
		static void SetCtrlHD(UINT16 CtrlHd);
		static void SetParameterHD(UINT16 Id,UINT32 Param);
		static void ResetFPGAOverload();
		static void FPGA::ResetFPGAOverloadClear();
				//Get functions
		static UINT32 GetFrequency(void);
		static UINT16 GetStatus();
		static UINT8 GetGPI();
		static UINT32 GetParameterHD(UINT16 Id);
		
		static void  SetSonicCommand(UINT8 SonicCommand);
		static UINT8 GetSonicCommand();
		 
		 
		
	private:
		static FPGA_POWERUP FPGA_PowerUp;   //Power up parameters
		static FPGA_PARAMS  FPGA_Params;	//Parameters
		static UINT32 FPGA_PreInitPara[MAX_PRE_INIT_PARAM]; 
		static STRCYC stRCyc ;				// Read parameters
		static STWCYC stWCyc;				//Write parameters
		static  MOSI Mosi;
		static  MISO Miso;
		static FPGA_MAP FPGA_Map[];
		static UINT8 Spi_XferEnableFlag;
		static UINT8 FPGASonicCommand;
		
		static UINT8 CheckParity(UINT8 *RxPkg);
		static void WriteParity( UINT8 *TxPkg);
		static void SwapByte(UINT32* Data);
		static void ProcessVersionFPGA();
};

#endif /* FPGA_H_ */

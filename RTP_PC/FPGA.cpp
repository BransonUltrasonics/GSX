/*
 * FPGA.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: hasanchez
 */

#include <vxWorks.h>        //for vxworks main
#include "FPGA.h"
#include "SPI.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include "versions.h"
#include "Common.h"

#include"PCObjDictionary.h"

using namespace std;


FPGA_POWERUP FPGA::FPGA_PowerUp;
FPGA_PARAMS  FPGA::FPGA_Params;
UINT32 FPGA::FPGA_PreInitPara[MAX_PRE_INIT_PARAM];
STRCYC FPGA::stRCyc;
STWCYC FPGA::stWCyc;
MOSI FPGA::Mosi;
MISO FPGA::Miso;
UINT8 FPGA::Spi_XferEnableFlag;
UINT8 FPGA::FPGASonicCommand;

#define RX_PARITY_BYTE_INDX		4
#define TX_PARITY_BYTE_INDX		0
#define RX_LENGTH_WITH_NO_LEAD_ZERO_BYTES	6

FPGA_MAP FPGA::FPGA_Map[]=
{ 
		//Index						Address									MOSI parameter size										MOSI parameter																		
		
		//FPGA Power Up parameters
		{INX_TEST, 					FPGA_TEST_ADDRESS, 						sizeof(FPGA_PowerUp.TestRegister),				(UINT8*)&FPGA_PowerUp.TestRegister},
		{INX_TEST_INV, 				FPGA_TEST_INV_ADDRESS, 					sizeof(FPGA_PowerUp.TestRegisterInv),			(UINT8*)&FPGA_PowerUp.TestRegisterInv},
		{INX_FPGA_VER1, 			FPGA_VER1_ADDRESS, 						sizeof(FPGA_PowerUp.FpgaVersion1),				(UINT8*)&FPGA_PowerUp.FpgaVersion1},
		{INX_FPGA_VER2, 			FPGA_VER2_ADDRESS, 						sizeof(FPGA_PowerUp.FpgaVersion2),				(UINT8*)&FPGA_PowerUp.FpgaVersion2},
		{INX_FPGA_BRDTEST, 			FPGA_BRDTEST_ADDRESS, 					sizeof(FPGA_PowerUp.BoardTestMode),				(UINT8*)&FPGA_PowerUp.BoardTestMode},
		{INX_FPGA_CTRL, 			FPGA_CTRL_ADDRESS, 						sizeof(FPGA_PowerUp.FPGACtrl),					(UINT8*)&FPGA_PowerUp.FPGACtrl},
		{INX_FPGA_STAT, 			FPGA_STAT_ADDRESS, 						sizeof(FPGA_PowerUp.FPGAStat), 					(UINT8*)&FPGA_PowerUp.FPGAStat},
		{INX_PWRUP_DUMMY, 			FPGA_DUMMY_ADDRESS, 					sizeof(FPGA_Params.HDParam.Dummy), 			(UINT8*)&FPGA_Params.HDParam.Dummy},
		
		//FPGA parameters
		{INX_HD_MODE, 				FPGA_HD_MODE_ADDRESS, 					sizeof(FPGA_Params.HDParam.Mode), 			(UINT8*)&FPGA_Params.HDParam.Mode},
		{INX_HD_BLIND_TIME_SEEK, 	FPGA_HD_BLIND_TIME_SEEK_ADDRESS, 		sizeof(FPGA_Params.HDParam.BlindTimeSeek), 	(UINT8*)&FPGA_Params.HDParam.BlindTimeSeek},
		{INX_HD_BLIND_TIME_WELD, 	FPGA_HD_BLIND_TIME_WELD_ADDRESS, 		sizeof(FPGA_Params.HDParam.BlindTimeWeld), 	(UINT8*)&FPGA_Params.HDParam.BlindTimeWeld},
		{INX_HD_SWITCH_TIME, 		FPGA_HD_SWITCH_TIME_ADDRESS, 			sizeof(FPGA_Params.HDParam.SwitchTime), 	(UINT8*)&FPGA_Params.HDParam.SwitchTime},
		{INX_HD_WELD_AMP, 			FPGA_HD_WELD_AMP_ADDRESS, 				sizeof(FPGA_Params.HDParam.WeldAmplitude), 	(UINT8*)&FPGA_Params.HDParam.WeldAmplitude},
		{INX_HD_PWM_PERCENT, 		FPGA_HD_PWM_PERCENT_ADDRESS, 			sizeof(FPGA_Params.HDParam.PwmPercentage), 	(UINT8*)&FPGA_Params.HDParam.PwmPercentage},
		{INX_HD_HW_DELAY, 			FPGA_HD_HW_DELAY_ADDRESS, 				sizeof(FPGA_Params.HDParam.HwDelay), 		(UINT8*)&FPGA_Params.HDParam.HwDelay}, 
		{INX_HD_FSYNC_DPLL, 		FPGA_HD_FSYNC_DPLL_ADDRESS, 			sizeof(FPGA_Params.HDParam.FsyncDpll), 		(UINT8*)&FPGA_Params.HDParam.FsyncDpll},	
		{INX_HD_LTFREQ_CRTL1, 		FPGA_HD_LTFREQ_CRTL1_ADDRESS, 			sizeof(FPGA_Params.HDParam.LtFreqCtrl1), 	(UINT8*)&FPGA_Params.HDParam.LtFreqCtrl1},
		{INX_HD_LTFREQ_CRTL2, 		FPGA_HD_LTFREQ_CRTL2_ADDRESS, 			sizeof(FPGA_Params.HDParam.LtFreqCtrl2), 	(UINT8*)&FPGA_Params.HDParam.LtFreqCtrl2},
		{INX_HD_LTFREQ_CRTL3, 		FPGA_HD_LTFREQ_CRTL3_ADDRESS, 			sizeof(FPGA_Params.HDParam.LtFreqCtrl3), 	(UINT8*)&FPGA_Params.HDParam.LtFreqCtrl3},
		{INX_HD_LTFREQ_CRTL4, 		FPGA_HD_LTFREQ_CRTL4_ADDRESS, 			sizeof(FPGA_Params.HDParam.LtFreqCtrl4), 	(UINT8*)&FPGA_Params.HDParam.LtFreqCtrl4},
		{INX_HD_NOT_USED1, 			FPGA_HD_NOT_USED1_ADDRESS, 				sizeof(FPGA_Params.HDParam.NotUsed1), 		(UINT8*)&FPGA_Params.HDParam.NotUsed1},
		{INX_HD_NOT_USED2, 			FPGA_HD_NOT_USED2_ADDRESS, 				sizeof(FPGA_Params.HDParam.NotUsed2), 		(UINT8*)&FPGA_Params.HDParam.NotUsed2},
		{INX_HD_HCTEST, 			FPGA_HD_HCTEST_ADDRESS, 				sizeof(FPGA_Params.HDParam.HcTest), 		(UINT8*)&FPGA_Params.HDParam.HcTest},
		
		//Cycle write parameters uP->FPGA
		{INX_GPO, 					FPGA_GPO_ADDRESS, 						sizeof(stWCyc.GPO), 							(UINT8*)&stWCyc.GPO	},
		{INX_CTRL_HDX, 				FPGA_CTRL_HDX_ADDRESS, 					sizeof(stWCyc.CtrlHDX), 						(UINT8*)&stWCyc.CtrlHDX	},
		{INX_FREQ_VAL_IN_HI, 		FPGA_FREQ_VAL_IN_ADDRESS , 				sizeof(stWCyc.FrequencyValIn), 					(UINT8*)(&stWCyc.FrequencyValIn)},
		{INX_PHASE_VAL_IN, 			FPGA_PHASE_VAL_IN_ADDRESS, 				sizeof(stWCyc.PhaseValIn), 						(UINT8*)&stWCyc.PhaseValIn	},
		{INX_PWM_VAL, 				FPGA_PWM_VAL_ADDRESS, 					sizeof(stWCyc.PWMVal), 							(UINT8*)&stWCyc.PWMVal	},
		
		//Cycle read parameters FPGA->uP
		{INX_GPI, 					FPGA_GPI_ADDRESS,						sizeof(stRCyc.GPI), 							(UINT8*)&stRCyc.GPI	},
		{INX_RFPGA_STAT, 			FPGA_READ_STAT_ADDRESS,					sizeof(stRCyc.FPGAStat), 						(UINT8*)&stRCyc.FPGAStat	},
		{INX_STAT_HDX, 				FPGA_STAT_HDX_ADDRESS,					sizeof(stRCyc.StatHDX), 						(UINT8*)&stRCyc.StatHDX	},
		{INX_FREQ_VAL_OUT, 			FPGA_FREQ_VAL_OUT_ADDRESS, 				sizeof(stRCyc.FrequencyValOut), 				(UINT8*)&stRCyc.FrequencyValOut	},
		{INX_PHASE_VAL_OUT, 		FPGA_PHASE_VAL_OUT_ADDRESS, 			sizeof(stRCyc.PhaseValOut), 					(UINT8*)&stRCyc.PhaseValOut	},
		
		//FPGA pre-initialization parameters
		{INX_PREINIT_PARAM0, 		FPGA_PREINIT_PARAM0_ADDRESS, 			sizeof(FPGA_PreInitPara[0]), 					(UINT8*)&FPGA_PreInitPara[0]	},
		{INX_PREINIT_PARAM1, 		FPGA_PREINIT_PARAM1_ADDRESS, 			sizeof(FPGA_PreInitPara[1]), 					(UINT8*)&FPGA_PreInitPara[1]	},
		{INX_PREINIT_PARAM2, 		FPGA_PREINIT_PARAM2_ADDRESS, 			sizeof(FPGA_PreInitPara[2]), 					(UINT8*)&FPGA_PreInitPara[2]	},
		{INX_PREINIT_PARAM3, 		FPGA_PREINIT_PARAM3_ADDRESS, 			sizeof(FPGA_PreInitPara[3]), 					(UINT8*)&FPGA_PreInitPara[3]	},
		{INX_PREINIT_PARAM4, 		FPGA_PREINIT_PARAM4_ADDRESS, 			sizeof(FPGA_PreInitPara[4]), 					(UINT8*)&FPGA_PreInitPara[4]	},
		{INX_PREINIT_PARAM5, 		FPGA_PREINIT_PARAM5_ADDRESS, 			sizeof(FPGA_PreInitPara[5]), 					(UINT8*)&FPGA_PreInitPara[5]	},
		{INX_PREINIT_PARAM6, 		FPGA_PREINIT_PARAM6_ADDRESS, 			sizeof(FPGA_PreInitPara[6]), 					(UINT8*)&FPGA_PreInitPara[6]	},
		{INX_PREINIT_PARAM7, 		FPGA_PREINIT_PARAM7_ADDRESS, 			sizeof(FPGA_PreInitPara[7]), 					(UINT8*)&FPGA_PreInitPara[7]	},
		{INX_PREINIT_PARAM8, 		FPGA_PREINIT_PARAM8_ADDRESS, 			sizeof(FPGA_PreInitPara[8]), 					(UINT8*)&FPGA_PreInitPara[8]	},
		{INX_PREINIT_PARAM9, 		FPGA_PREINIT_PARAM9_ADDRESS, 			sizeof(FPGA_PreInitPara[9]), 					(UINT8*)&FPGA_PreInitPara[9]	},
		
		{INX_DUMMY, 				FPGA_DUMMY_ADDRESS, 					sizeof(FPGA_Params.HDParam.Dummy), 			(UINT8*)&FPGA_Params.HDParam.Dummy	},
	
};

/**************************************************************************//**
* \brief  - Constructor.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
FPGA::FPGA()
{
	//Constructor
	
}

/**************************************************************************//**
* \brief  - Destructor.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
FPGA::~FPGA()
{
	//Destructor
}


/**************************************************************************//**
* \brief  - Initialization of the FPGA parameters.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
void FPGA::Init()
{
	UINT16 Indx = 0;
	Spi_XferEnableFlag = TRUE;
	FPGA_PowerUp.FpgaVersion1 = 0;
	FPGA_PowerUp.FpgaVersion2 = 0;
	FPGA_PowerUp.FPGACtrl = FPGA_CTRL_RUN | FPGA_CTRL_INIT_DONE;
	FPGA_PowerUp.BoardTestMode = 0; 
	FPGA_PowerUp.FPGAStat = 0;
	FPGA_PowerUp.TestRegister = TEST_REGISTER_PATTERN;
	
	stWCyc.GPO = 0;
	stWCyc.CtrlHDX = 0;
	stWCyc.PWMVal = 0;
	stWCyc.PhaseValIn = 0;

	LOG("FPGA::Init\n");

#ifdef POWERUP_FILE_READ
	ifstream Fp("/sd0:1/PowerupRecipefromsc.txt");
	string PowerUpParamStr;
	string TempStr;

	if(Fp.is_open())     //Check if file is opened
	{
		getline(Fp,PowerUpParamStr,'@');                //Reads all content of the text file until end character '@'
		
		TempStr = PowerUpParamStr.substr(0,PowerUpParamStr.find("Power"));    //Extracts one string from the beginning until "Power" string
		TempStr = TempStr.substr(TempStr.find(':')+1);						  //in order to read the PS Frequency	
		PSFrequency=  atoi(TempStr.c_str());
#ifdef PRINT
		LOG("   Frequency 		 =  %d\n",PSFrequency);
#endif	
		TempStr = PowerUpParamStr.substr(PowerUpParamStr.find("Power"),PowerUpParamStr.find("SYSTEMTYPE")-PowerUpParamStr.find("Power")); // Reads the PS power
		TempStr = TempStr.substr(TempStr.find(':')+1);
		PSPower = atoi(TempStr.c_str());
#ifdef PRINT
		LOG("   Power 		 =  %d\n",PSPower);
#endif		 
	}
	Fp.close();    //Closes the text file
#endif
	
	PSFrequency = FREQ_20KHZ;	/* Initialize FPGA with 20 KHz system configuration and later updates as per user configuration */ 
	PSPower		= 4000;
	
#ifndef PRINT
		LOG("   Frequency 	=  %d\n",PSFrequency);
		LOG("   Power 		=  %d\n",PSPower);
#endif

	switch (PSFrequency)
	{
		case FREQ_40KHZ:
			FPGA_PreInitPara[Indx++] = PREINITPARA1_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA2_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA3_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA4_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			stWCyc.FrequencyValIn = 400000;
			break;
			
		case FREQ_30KHZ:
			FPGA_PreInitPara[Indx++] = PREINITPARA1_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA2_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA3_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA4_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			stWCyc.FrequencyValIn = 300000;
			break;
			
		case FREQ_20KHZ:
		default:
			FPGA_PreInitPara[Indx++] = PREINITPARA1_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA2_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA3_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA4_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			stWCyc.FrequencyValIn = 199500;
			break;
		}
	
	//20khz HD default values
	FPGA_Params.HDParam.Mode = HD_MODE_MIN ;
	FPGA_Params.HDParam.BlindTimeSeek = HD_BLIND_TIME_SEEK_DEFAULT;
	FPGA_Params.HDParam.BlindTimeWeld = HD_BLIND_TIME_WELD_DEFAULT;
	FPGA_Params.HDParam.SwitchTime = HD_SWITCH_TIME_20K_DEFAULT;
	FPGA_Params.HDParam.WeldAmplitude = HD_WELD_AMP_DEFAULT;
	FPGA_Params.HDParam.PwmPercentage =HD_PWM_DEFAULT;
	FPGA_Params.HDParam.HwDelay = HD_HW_DELAY_20K_DEFAULT;
	FPGA_Params.HDParam.FsyncDpll = HD_FSYNC_DPLL_20K_DEFAULT;
	FPGA_Params.HDParam.LtFreqCtrl1 = HD_LT_FREQ_CTRL1_20K_DEFAULT;
	FPGA_Params.HDParam.LtFreqCtrl2 = HD_LT_FREQ_CTRL2_20K_DEFAULT;
	FPGA_Params.HDParam.LtFreqCtrl3 = HD_LT_FREQ_CTRL3_20K_DEFAULT; 
	FPGA_Params.HDParam.LtFreqCtrl4 = HD_LT_FREQ_CTRL4_20K_DEFAULT;
	FPGA_Params.HDParam.NotUsed1  = HD_NOT_USED1_20K_DEFAULT ;
	FPGA_Params.HDParam.NotUsed2  = HD_NOT_USED2_20K_DEFAULT;
	FPGA_Params.HDParam.HcTest = HD_HC_TEST_DEFAULT;


}

/**************************************************************************//**
* \brief  - Updates the FPGA init parameters every request of SDO.
*
* \param  - none
*
*
* \return  - none
*
******************************************************************************/
//void FPGA::UpdateFPGAInitParameters()
void FPGA::UpdateFPGAInitParameters(PowerUpDataParam& powerUpRecipe)
{
	UINT16 Indx = 0;
	
	LOG("FPGA::UpdateFPGAInitParameters\n");

#ifdef POWERUP_FILE_READ
	ifstream Fp("/sd0:1/PowerupRecipefromsc.txt");
	string PowerUpParamStr;
	string TempStr;

	if(Fp.is_open())			//Check if file is opened
	{
		getline(Fp,PowerUpParamStr,'@');		 //Reads all content of the text file until end character '@'

		TempStr = PowerUpParamStr.substr(0,PowerUpParamStr.find("Power"));	//Extracts one string from the beginning until "Power" string
		TempStr = TempStr.substr(TempStr.find(':')+1);						//in order to read the PS Frequency	
		PSFrequency=  atoi(TempStr.c_str());
#ifdef PRINT
		LOG("   PSFrequency 		 =  %d\n",PSFrequency);
#endif	
				TempStr = PowerUpParamStr.substr(PowerUpParamStr.find("Power"),PowerUpParamStr.find("SYSTEMTYPE")-PowerUpParamStr.find("Power")); // Reads the PS power
				TempStr = TempStr.substr(TempStr.find(':')+1);
				PSPower = atoi(TempStr.c_str());
#ifdef PRINT
		LOG("   Power 		 =  %d\n",PSPower);
#endif					

	}
	Fp.close();		//Closes the text file
#endif

	PSFrequency = powerUpRecipe.Frequency;	/* SDO recipe from Supervisory Controller */ 
	PSPower		= powerUpRecipe.Power;

#ifndef PRINT
	LOG("   Frequency 	=  %d\n",PSFrequency);
	LOG("   Power 		=  %d\n",PSPower);
#endif

	switch ( PSFrequency )
	{
		case FREQ_40KHZ:
			FPGA_PreInitPara[Indx++] = PREINITPARA1_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA2_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA3_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA4_40KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			break;
	
		case FREQ_30KHZ:
			FPGA_PreInitPara[Indx++] = PREINITPARA1_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA2_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA3_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA4_30KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			break;
	
		case FREQ_20KHZ:
		default:
			FPGA_PreInitPara[Indx++] = PREINITPARA1_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA2_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA3_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = PREINITPARA4_20KHZ_DEFAULT;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			FPGA_PreInitPara[Indx++] = 0;
			break;
	}

	FPGA_Params.HDParam.Mode 			= WeldRecipeObj.PCWeldRecipe.ControlMode;
	FPGA_Params.HDParam.BlindTimeSeek 	= WeldRecipeObj.PCWeldRecipe.BlindtimeSeek;
	FPGA_Params.HDParam.BlindTimeWeld 	= WeldRecipeObj.PCWeldRecipe.Blindtimeweld;
	FPGA_Params.HDParam.SwitchTime 		= WeldRecipeObj.PCWeldRecipe.DDSSwitchtime;
	FPGA_Params.HDParam.WeldAmplitude 	= HD_WELD_AMP_DEFAULT;
	FPGA_Params.HDParam.PwmPercentage	= HD_PWM_DEFAULT;
	FPGA_Params.HDParam.HwDelay 		= HD_HW_DELAY_20K_DEFAULT;
	FPGA_Params.HDParam.FsyncDpll 		= HD_FSYNC_DPLL_20K_DEFAULT;
	FPGA_Params.HDParam.LtFreqCtrl1 	= HD_LT_FREQ_CTRL1_20K_DEFAULT;
	FPGA_Params.HDParam.LtFreqCtrl2 	= HD_LT_FREQ_CTRL2_20K_DEFAULT;
	FPGA_Params.HDParam.LtFreqCtrl3 	= HD_LT_FREQ_CTRL3_20K_DEFAULT; 
	FPGA_Params.HDParam.LtFreqCtrl4 	= HD_LT_FREQ_CTRL4_20K_DEFAULT;
	FPGA_Params.HDParam.NotUsed1  		= HD_NOT_USED1_20K_DEFAULT ;
	FPGA_Params.HDParam.NotUsed2  		= HD_NOT_USED2_20K_DEFAULT;
	FPGA_Params.HDParam.HcTest 			= HD_HC_TEST_DEFAULT;

			Sonics::SetDefaultFreqAndFFactor();
	
}

/**************************************************************************//**
* \brief  - Function of the FPGA transfer to send and receive the parameters from the FPGA Map.
*
* \param  - FpgaInfoRx: Includes the information of the data to receive according to the FPGA parameter map. 
* 			FpgaInfoTx: Includes the information of the data to transfer according to the FPGA parameter map.
*
* \return  - none
*
******************************************************************************/
void FPGA::SPITransfer(FPGA_INX IndexRx, FPGA_INX IndexTx)
{
	UINT16 Indx = 0;

	//Fills the frame with the Tx,Rx addresses and Data content to be sent to the FPGA
	if(Spi_XferEnableFlag == TRUE)
		{
			Mosi.Tx.RxAddr = FPGA_Map[IndexRx].Address;       
			Mosi.Tx.TxAddr = FPGA_Map[IndexTx].Address;
			memcpy(&Mosi.Tx.Data,FPGA_Map[IndexTx].Source,sizeof(Mosi.Tx.Data));

			//Put the parity result into the B4 of the frame
			WriteParity((UINT8*)&Mosi.TxPkg);
	
			SwapByte(&Mosi.TxPkg[Indx]);  //Swapping the first 32 bits
			Indx++;
			SwapByte(&Mosi.TxPkg[Indx]);  //Swapping the second 32 bits
	


			SPI::SingleTransfer(FPGA_SPI_CHANNEL, (UINT8*)&Mosi.TxPkg, (UINT8*)&Miso.RxPkg, sizeof(Mosi.TxPkg));   //Sends the whole frame of 64 bits to the FPGA
																							 //and receives the response in the Rx frame
			SwapByte(&Miso.Rx.Data);		//Swapping the response of FPGA
	
	
	
			if (Miso.Rx.Error == 0)    //No error found
			{
				if (CheckParity((UINT8 *)&Miso.RxPkgByte) == TRUE)
				{
					memcpy(FPGA_Map[IndexRx].Source, &Miso.Rx.Data, FPGA_Map[IndexRx].Length );
				}
				else
				{
					//Do something is case of no parity
				}
			}
			else
			{
				//Do something is case an error

			}
		}
}

/**************************************************************************//**
* \brief  - Calculates the parity between the bytes and stores the result in B4 of the TxPkg.
*
* \param  - *TxPkg: Pointer to the MOSI message will be sent to FPGA. 
*
* \return  - none
*
******************************************************************************/
void FPGA::WriteParity( UINT8 *TxPkg)
{
	UINT8 Parity = 0;
	UINT8 Len = sizeof(Mosi.TxPkg);
	for (UINT8 Indx= 0; Indx < Len ; Indx++)
	{
		if (Indx != TX_PARITY_BYTE_INDX)      //Parity operation to all bytes except B4
			Parity ^= TxPkg[Indx]; 
	}
	TxPkg[TX_PARITY_BYTE_INDX]=Parity;      //Assigns the parity result to B4
}

/**************************************************************************//**
* \brief  - Parity check to confirm if the message was received well from FPGA.
*
* \param  - *RxPkg: Pointer to the MISO message will be received from FPGA.
*
* \return  - TestParity: Returns true if the message received matches to the parity sent previously.
*
******************************************************************************/
UINT8 FPGA::CheckParity( UINT8 *RxPkg)
{
	UINT8 TestParityFlag= FALSE;
	UINT8 Parity = 0;
	UINT8 Len = sizeof(Miso.RxPkg);//RX_LENGTH_WITH_NO_LEAD_ZERO_BYTES;      
	
	for (UINT8 Indx= 0; Indx < Len ; Indx++)
	{
		if (Indx != RX_PARITY_BYTE_INDX)
			Parity ^= RxPkg[Indx]; 
	}
	
	if (Parity == RxPkg[RX_PARITY_BYTE_INDX])
		TestParityFlag= TRUE;
	
	return (TestParityFlag);
}

/**************************************************************************//**
* \brief  - Setup the FPGA to store some parameters during the powerup
*			it will enter to this function during the power up
*			It will exit until get the status "init done" from the FPGA
*
* \param  - none.
*
* \return  - none.
*
******************************************************************************/
UINT8 FPGA::PowerUpSequence(void)
{
	UINT32 Init_Done_Retries = 0;
	UINT8 RetVal = TRUE;
	
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM0 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM1 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM2 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM3 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM4 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM5 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM6 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM7 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM8 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM9 );
	SPITransfer( INX_DUMMY,  INX_PWM_VAL );
	SPITransfer( INX_DUMMY,  INX_FPGA_CTRL );
	
	stWCyc.GPO |= OVERLOADRESET_OUT;
	
	do
	{		
		SPITransfer( INX_FPGA_VER1,  INX_PWM_VAL );
		SPITransfer( INX_RFPGA_STAT,  INX_FREQ_VAL_IN_HI );
		SPITransfer( INX_FPGA_VER2,  INX_FPGA_CTRL );
		SPITransfer( INX_RFPGA_STAT,  INX_GPO );
		
		if(Init_Done_Retries++ >= INIT_DONE_MAX_RETRIES)
		{
			RetVal = FALSE;
		}
				
	}while (((stRCyc.FPGAStat & (FPGA_STAT_INIT_DONE) == 0) || (stRCyc.FPGAStat & (FPGA_STAT_RUN) == 0)) && (RetVal == TRUE));
	
	/*Process FPGA version into a 32bit integer (8 bits per digit)*/
	ProcessVersionFPGA();
			
	return (RetVal);
		
}

/**************************************************************************//**
* \brief  - Swaping between the bytes of the message.
*
* \param  - *Data: Pointer to Data will be swapped.
*
* \return  - none.
*
******************************************************************************/
void FPGA::SwapByte(UINT32* Data )
{
	UINT32 Dummy;
	memcpy(&Dummy,Data,sizeof(Dummy));
	*Data=0;
	
	for(UINT8 Indx=0; Indx < sizeof(UINT32); Indx++)
	{
		*Data = (*Data<<BITS_ON_BYTE) | (Dummy & BYTE_MASK);
		Dummy>>=BITS_ON_BYTE;
	}
}

/**************************************************************************//**
* \brief  - This function sends SONIC_ON_OUT to FPGA in order to start sonics.
*
* \param  - none.
*
* \return  - none.
*
******************************************************************************/
void FPGA::StartSonic()
{
   stWCyc.GPO = SONIC_ON_OUT;
}

/**************************************************************************//**
* \brief  - This function sends OVERLOADRESET_OUT to FPGA in order to reset
* 			The alarms detected by the FPGA
*
* \param  - none.
*
* \return  - none.
*
******************************************************************************/
void FPGA::ResetFPGAOverload()
{
   stWCyc.GPO |= OVERLOADRESET_OUT;
}

void FPGA::ResetFPGAOverloadClear()
{
   stWCyc.GPO &= ~OVERLOADRESET_OUT;
}

/**************************************************************************//**
* \brief  - This function sends NO_OUT to FPGA to stops sonics.
*
* \param  - none.
*
* \return  - none.
*
******************************************************************************/
void FPGA::StopSonic()
{
	stWCyc.GPO &= ~SONIC_ON_OUT;
}

/**************************************************************************//**
* \brief  - Sets the Frequency to be sent to the FPGA.
*
* \param  - Freq: Frequency value to send to the FPGA.
*
* \return  - none.
*
******************************************************************************/
void FPGA::SetFrequency(UINT32 Freq)
{
	stWCyc.FrequencyValIn = Freq;
}

/**************************************************************************//**
* \brief  - Gets the frequency that is sent to the FPGA.
*
* \param  - none.
*
* \return  - FrequencyValIn: Frequency of the FPGA.
*
******************************************************************************/
UINT32 FPGA::GetFrequency()
{
	return(stWCyc.FrequencyValIn);
}

/**************************************************************************//**
* \brief  - Sets the PWM value to the FPGA.
*
* \param  - Pwm: Pwm value to be sent to the FPGA.
*
* \return  - none.
*
******************************************************************************/
void FPGA::SetPwmVal(UINT16 Pwm)
{
	stWCyc.PWMVal = Pwm;
}

/**************************************************************************//**
* \brief  - Gets the digital inputs signals from FPGA.
*
* \param  - none.
*
* \return  - Overload signals and sign bit of the phase.
*
******************************************************************************/
UINT8 FPGA::GetGPI()
{
	return (stRCyc.GPI);
}

/**************************************************************************//**
* \brief  - Sets the FPGA phase value.
*
* \param  - Phase:  Phase value to be sent to the FPGA. This value is taken from ADC phase.
*
* \return  - none.
*
******************************************************************************/
void FPGA::SetPhase(UINT16 Phase)
{
	stWCyc.PhaseValIn = Phase;
}

/**************************************************************************//**
* \brief  - Gets the current status of FPGA.
*
* \param  - none.
*
* \return  - FPGAStat: Status of FPGA.
*
******************************************************************************/
UINT16 FPGA::GetStatus()
{
	return(stRCyc.FPGAStat);
}

/**************************************************************************//**
* \brief  - Sets the control HD command to FPGA.
*
* \param  - CtrlHd: HD command.
*
* \return  - none.
*
******************************************************************************/
void FPGA::SetCtrlHD(UINT16 CtrlHd)
{
	stWCyc.CtrlHDX = CtrlHd;
}

/**************************************************************************//**
* \brief  - Sets the control HD command to FPGA.
*
* \param  - Id: ID of the parameter.
*			Param: HD parameter.
*			
* \return  - none.
*
******************************************************************************/
void FPGA::SetParameterHD(UINT16 Id,UINT32 Param)
{
	if(Id >= INX_HD_MODE && Id <= INX_HD_HCTEST)
	{
		FPGA_Params.Parameter[Id] = Param;
	}
}

/**************************************************************************//**
* \brief  - Gets the value of the HD parameter.
*
* \param  - Id: ID of the parameter.
*			
* \return  - HdParam: HD parameter.
*
******************************************************************************/
UINT32 FPGA::GetParameterHD(UINT16 Id)
{
	UINT32 HdParam = 0;
	
	if(Id >= INX_HD_MODE && Id <= INX_HD_HCTEST)
		{
			HdParam = FPGA_Params.Parameter[Id];
		}
	return (HdParam);
}

/**************************************************************************//**
* \brief  - Set the Sonic command to be read by the FPGA CommFPGATask
*
* \param  - SonicCommand: Sonic command to be set
*			
* \return  - none
*
******************************************************************************/
void FPGA::SetSonicCommand(UINT8 SonicCommand)
{
	FPGASonicCommand = SonicCommand;
}

/**************************************************************************//**
* \brief  - Gets the actual Sonic command
*
* \param  - none
*			
* \return  - FPGASonicCommand: Actual sonic command
*
******************************************************************************/
UINT8 FPGA::GetSonicCommand()
{
	return FPGASonicCommand;
}

/**************************************************************************//**
* \brief  - Enables communication with FPGA.
*
* \param  - none
*			
* \return  - none
*
******************************************************************************/
void FPGA::EnableSpiXfer()
{
	Spi_XferEnableFlag = TRUE;
}

/**************************************************************************//**
* \brief  - Disables communication with FPGA.
*
* \param  - none
*			
* \return  - none
*
******************************************************************************/
void FPGA::DisableSpiXfer()
{
	Spi_XferEnableFlag = FALSE;
}

/**************************************************************************//**
* \brief  - Re-Initialization of FPGA.
*
* \param  - none
*			
* \return  - True if re-initialization successfully
*
******************************************************************************/
UINT8 FPGA::Re_Init()
{
	UINT16 Indx = 0;
	Spi_XferEnableFlag = TRUE;
	FPGA_PowerUp.FpgaVersion1 = 0;
	FPGA_PowerUp.FpgaVersion2 = 0;
	FPGA_PowerUp.FPGACtrl = FPGA_CTRL_RUN | FPGA_CTRL_INIT_DONE;
	FPGA_PowerUp.BoardTestMode = 0; 
	FPGA_PowerUp.FPGAStat = 0;
	
	stWCyc.CtrlHDX = 0;
	stWCyc.PWMVal = 0;
	stWCyc.PhaseValIn = 0;
	
	stWCyc.GPO = OVERLOADRESET_OUT;
			
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM0 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM1 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM2 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM3 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM4 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM5 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM6 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM7 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM8 );
	SPITransfer( INX_DUMMY,  INX_PREINIT_PARAM9 );
	
	SPITransfer( INX_FPGA_VER1,  INX_PWM_VAL );
	SPITransfer( INX_FPGA_STAT,  INX_FREQ_VAL_IN_HI );
	SPITransfer( INX_FPGA_VER2,  INX_FPGA_CTRL );
	SPITransfer( INX_FPGA_STAT,  INX_GPO );
	
	if((FPGA_PowerUp.FPGAStat & (FPGA_STAT_INIT_DONE) == 0) || (stRCyc.FPGAStat & (FPGA_STAT_RUN) == 0))
	{
		Spi_XferEnableFlag = FALSE;
	}
	else
	{
		ProcessVersionFPGA();
	}
	
	return Spi_XferEnableFlag;
}

/**************************************************************************//**
* \brief  - Communication verification.
*
* \param  - none
*			
* \return  - true if the communication is still alive
*
******************************************************************************/
UINT8 FPGA::VerifyFPGAComm()
{
	UINT8 RetVal = TRUE;
	Spi_XferEnableFlag = TRUE;			//Enabling communication just for this command to be sent to FPGA
	FPGA_PowerUp.TestRegister = TEST_REGISTER_PATTERN;
	SPITransfer(INX_DUMMY, INX_TEST);
	FPGA_PowerUp.TestRegister = 0;		//To be sure that Test Register receives the pattern from FPGA
	SPITransfer(INX_TEST,INX_DUMMY);
	Spi_XferEnableFlag = FALSE;			//Disabling communication again
	
	if(FPGA_PowerUp.TestRegister != TEST_REGISTER_PATTERN)
	{
		RetVal = FALSE;
	}
	return RetVal;
}

/**************************************************************************//**
* \brief  - Process the FPGA version sent by the FPGA and sets the value
* 			on the OD.
*
* \param  - None
*			
* \return - None
*
******************************************************************************/
void FPGA::ProcessVersionFPGA()
{
	VersionInfoSDO versionInfo;
	UINT8 FPGAVersionDigits = FPGA_VERSION_DIGITS;
	UINT32 FPGA_Version_SDO = 0;
	UINT64 iFPGAVersion = 0;
	char strFPGAVersion[sizeof(iFPGAVersion) + 1] = {0}; //+1 for the end of string character
	std::string strTemp;
	std::stringstream sstrFPGAVersion;
	std::vector<string> v_FPGAVersion;
	
	//Concatenate the whole FPGA version into one single variable
	iFPGAVersion = FPGA_PowerUp.FpgaVersion1;
	iFPGAVersion = iFPGAVersion << 32;
	iFPGAVersion |= FPGA_PowerUp.FpgaVersion2;
	
	//Cast the integer value into a C string type 
	std::copy(static_cast<const char*> (static_cast<const void*>(&iFPGAVersion)), static_cast<const char*> (static_cast<const void*>(&iFPGAVersion)) + sizeof(iFPGAVersion), strFPGAVersion);
	
	//Reverse the array excluding the end of string character
	reverse(strFPGAVersion, strFPGAVersion + sizeof(strFPGAVersion) - 1);
	
	//Unique exception for the first ever release FPGA version
	if(strcmp(strFPGAVersion, FPGA_VERSION_1_0) == 0)
	{
		strcpy(strFPGAVersion, "1.0.0.0");
	}
	
	sstrFPGAVersion << strFPGAVersion;
	
	//Separate the version into single numbers divided by decimal point and cast version string digits into UINT32 format (8 bits per version digit)
	while (getline(sstrFPGAVersion, strTemp,'.'))
	{
	    FPGA_Version_SDO = FPGA_Version_SDO << BITS_ON_BYTE;
	    
	    if (std::atoi(strTemp.c_str()) <= MAX_FPGA_VERSION)
		{
			FPGA_Version_SDO |= std::atoi(strTemp.c_str());
		}
		else
		{
			FPGA_Version_SDO = 0;
			LOG("\nFPGA Version does not meet the correct format, setting it to 0\n");
			break;
		}
	    
		strTemp.clear();
		FPGAVersionDigits--;
	}
	
	FPGA_Version_SDO = FPGA_Version_SDO << BITS_ON_BYTE*FPGAVersionDigits;
	
	//Save the value into the FwInfo SDO
	FWInfo::Set(FW_INFO_PC,FPGA_Version_SDO);
	
	versionInfo.WriteFirmwareInfoToOD();
}

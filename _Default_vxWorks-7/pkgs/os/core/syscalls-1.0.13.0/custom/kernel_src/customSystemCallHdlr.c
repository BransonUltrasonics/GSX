/*
 * customSystemCallHdlr.c
 */

/* Header files */
#include <vxWorks.h>
#include <string.h>
#include <errno.h>
#include <syscallArgs.h>
#include <scMemVal.h>
#include <vxbMcSpiLib.h>
#include <timerDev.h>
#include <customSystemCallHdlr.h>
#include <signal.h>
#include <eventLib.h>
#include <subsys/gpio/vxbGpioLib.h>
#include <hwif/drv/resource/vxbRtcLib.h>
#include "ioLib.h"			//for open(),create(),write(),close() for timer rtc
#include <logLib.h>
#include <vxbEhrPwmLib.h>
#include <vxbEqepLib.h>
#include <fioLib.h>
#include <ifLib.h>
#include <ipProto.h>
#include <net/utils/routeCmd.h>
#include <net/utils/ifconfig.h>
#include <vxbQspiInterface.h>
#include <vxbSp25QspiFlash.h>
#include <vxbeMmcLib.h>


#define MAX_LOG_LEVELS 4 //Maximum logging levels
#define DISABLE_LOGS 0 //disables logs
#define MAX_LOG_FMT_BUFSIZE 512 //maximum formatted buffer size
// RUN time feature
#define EEPROM_PATH	"/eeprom/0" // eeprom path

//Ip Configuration
#define IFF_UP 0x1
#define INTERFACE "cpsw"
#define INTERFACE_NAME "cpsw1"
#define UNIT_NUMBER 1
#define SUCCESS 1
#define FAILURE 0
#define GATEWAY_CMD_BUFSIZE 100

#define	GPIO_PC_15V_LOW_PIN 0x24 //PC GPIO1_4 pin allocated for interrupt to Shutdown 15V
#define	GPIO_PC_5V_LOW_PIN 0x25 //PC GPIO1_5 pin allocated for interrupt to Shutdown 5V
#define	GPIO_PC_24V_LOW_PIN 0x26 //PC GPIO1_6 pin allocated for interrupt to Shutdown 24V
#define	GPIO_24V_LOW_PIN 0x61 //GPIO4_1 pin allocated for Shutdown Activity

/* Global Variables */
TASK_ID taskId, ctrlTaskId;
TASK_ID taskIdServo;

UINT8 logFmtBuffer[MAX_LOG_FMT_BUFSIZE]; //Buffer to hold formatted string
static UINT16 globalLogCount, logCount1; //holds the no. of line count
MSG_Q_ID loggerMsgQId;	

// RUN time feature start 
INT32 EepromFd; // Eeprom file descriptor
UINT8 featureNeptune[FEATUREFLAG_SIZE]			=	FEATURE_NEPTUNE;  		//Neptune model default values
UINT8 featureP1Pneumatic[FEATUREFLAG_SIZE]		=	FEATURE_P1_PNEUMATIC;  	//p1pneumatic model default values
UINT8 featureP2Pneumatic[FEATUREFLAG_SIZE]		=	FEATURE_P2_PNEUMATIC;  	//p2pneumatic model default values
UINT8 featureIIW[FEATUREFLAG_SIZE]				=	FEATURE_I_IW;  			//i(iw)model default values
UINT8 featureMercury[FEATUREFLAG_SIZE]			=	FEATURE_MERCURY; 		//mercury model default values
UINT8 featureSaturnP1[FEATUREFLAG_SIZE]			=	FEATURE_SATURNP1;  		//saturnp1 model default values
UINT8 featureSaturnP2[FEATUREFLAG_SIZE]			=	FEATURE_SATURNP2;  		//saturnp2 model default values
UINT8 featureMars[FEATUREFLAG_SIZE]				=	FEATURE_MARS;			//mars model default values
UINT8 featureJuno[FEATUREFLAG_SIZE]				=	FEATURE_JUNO;  			//juno model default values
UINT8 featureVenus[FEATUREFLAG_SIZE]			=	FEATURE_VENUS;			//venus model default values
UINT8 featureVenusHH[FEATUREFLAG_SIZE]			=	FEATURE_VENUSHH;  		//Venus HH model default values

extern EST_LIFE Dvr_EmmcEstLife;
extern UINT32 rtc_low_volt;

// Aux Clock Timer ISR
void AuxIsr(INT32 tid)
{
	if(eventSend(taskId, VXEV01) == ERROR)	
		kprintf("Error in sending an Timer Event\n");
}

/* Custom system call definitions */

// Aux clock connect timer ISR
INT32 AuxClkConnectSc(struct AuxClkConnectScArgs *isr)
{
	taskId = taskOpen((UINT8 *)"/TimerEvent0", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);	
	return (INT32)sysAuxClkConnect((FUNCPTR)AuxIsr, isr->task_id);
}

// Enable Aux clock 
void AuxClkEnableSc(void)
{
	sysAuxClkEnable();
}

// Disable Aux clock 
void AuxClkDisableSc(void)
{
	sysAuxClkDisable();
}

// Get Aux clock rate 
INT32 AuxClkRateGetSc(void)
{
	return (INT32)sysAuxClkRateGet();
}

// Set Aux clock rate
INT32 AuxClkRateSetSc(struct AuxClkRateSetScArgs *rate)
{
	return (INT32)sysAuxClkRateSet(rate->ticksPerSecond);
}

// Custom system call Set RTC
STATUS SetRTCSc (struct SetRTCScArgs *pArgs)
{
	STATUS status = SUCCESS;
	RTCtimerStr timer;
	timer = pArgs->time;	
	struct tm rtcTime;
	struct timespec stSystemTime;
	rtcTime.tm_year = timer.tm_year;
	rtcTime.tm_mon = timer.tm_mon;
	rtcTime.tm_mday = timer.tm_mday;
	rtcTime.tm_hour = timer.tm_hour;
	rtcTime.tm_min = timer.tm_min;
	rtcTime.tm_sec = timer.tm_sec;
	rtcTime.tm_isdst = 0; //Is DST on ?  0 = no, 1 = yes, -1 = unknown			
	if( vxbRtcSet(&rtcTime) == ERROR)		//Sets the RTC
	{
		kprintf("\nError in Setting the RTC Time!!!\n");
		status = FAILURE;
	}
	else
	{
		stSystemTime.tv_sec = mktime(&rtcTime);
		stSystemTime.tv_nsec = 0;
		if (clock_settime(CLOCK_REALTIME, &stSystemTime) == ERROR) //Sets the System-wide realtime clock	
		{
			kprintf("\nError in Setting the System Time!!!\n");
		}
	}
	return status;
}

// Custom system call Get RTC
STATUS GetRTCSc (struct GetRTCScArgs *pArgs)
{
	STATUS status = SUCCESS;
	RTCtimerStr *pTime;
	pTime = pArgs->pTime;
	struct tm rtcTime;
	//sysRtcGet(1);		// 1: full show string in time format DD:MM:YYYY (May require, hence commented out and kept)	 
	if( vxbRtcGet(&rtcTime) == ERROR)		//Sets the RTC
	{
		kprintf("\nError in Getting the RTC Time!!!\n");
		status = FAILURE;
	}
	else
	{
		pTime->tm_year = rtcTime.tm_year;
		pTime->tm_mon = rtcTime.tm_mon;
		pTime->tm_mday = rtcTime.tm_mday;
		pTime->tm_hour = rtcTime.tm_hour;
		pTime->tm_min = rtcTime.tm_min;
		pTime->tm_sec = rtcTime.tm_sec;
	}
	return status;
} 

// Custom system call Get RTC Low Voltage bit
INT32 GetRTClowPwrSc (struct GetRTClowPwrScArgs *pArgs)
{
	RTCtimerStr *pTime;
	pTime = pArgs->pTime;
	struct tm rtcTime;
	INT32 tm_lowPwr;
		 
	if( vxbRtcGet(&rtcTime) == ERROR)		//Gets the RTC
	{
		kprintf("\nError in Detecting the RTC low Power!!!\n");
	}
	else
	{
	    if (rtc_low_volt != 0)
		{    	
	    	tm_lowPwr = 1;
			kprintf("\nRTC low Power!!!\n");
		}
	    else
		{
	    	tm_lowPwr = 0;
			kprintf("\nthe RTC Power OK\n");
		}
	}
	return tm_lowPwr;
} 

// Allocate specific Gpio pin
INT32 GpioAllocSc(struct GpioAllocScArgs *id)
{
	return (int)vxbGpioAlloc(id->id);
}

// Get Gpio pin's value
UINT32 GpioGetValueSc(struct GpioGetValueScArgs *id)
{
	return (UINT32)vxbGpioGetValue(id->id);
}

// Set Gpio pin's value
INT32 GpioSetValueSc(struct GpioSetValueScArgs *id)
{
	return (INT32)vxbGpioSetValue(id->id, id->value);
}

// Set Gpio pin's direction
INT32 GpioSetDirSc(struct GpioSetDirScArgs *id)
{
	return (INT32)vxbGpioSetDir(id->id, id->value);
}

// Get Gpio pin's value
UINT32 GpioGetDirSc(struct GpioGetValueScArgs *id)
{
	return (UINT32)vxbGpioGetDir(id->id);
}


// PWM Timer ISR
void pwmISR()
{	
	if(eventSend(taskIdServo, VXEV04) == ERROR)
		kprintf("Error in sending an event\n");
}


// Get the value of the Time base Status Register [TBSTS]
INT32 EhrPwmRegisterISRSc(struct EhrPwmRegisterISRScArgs *pArgs)
{
	taskIdServo = taskOpen((UINT8 *)"/ServoISR", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);	
	return (INT32)vxbEhrPwmRegisterISR((EHRPWMDRV_IRQ_HANDLER)pwmISR, pArgs->ehrpwmHandlerArgs, pArgs->ehrpwm_inst, pArgs->period, pArgs->mode);
}


// set Software Force & also set Action when One-Time Software Force A & B Is invoked
STATUS EhrPwmSetAQSFRCSc(struct EhrPwmSetAQSFRCScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetAQSFRC(pArgs->value, pArgs->inst);	
}

// set the divisor of the time base clock
STATUS EhrPwmSetTbClkSc(struct EhrPwmSetTbClkScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbClk(pArgs->tb, pArgs->ehrpwm_inst);	
}

// set the actions to take when TBCNT reach 0, TBPRD, CMPA and CMPB for eHRPWM0A
STATUS EhrPwmSetTbAQConfASc(struct EhrPwmSetTbAQConfAScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbAQConfA(pArgs->tb, pArgs->ehrpwm_inst);	
}

// set the actions to take when TBCNT reach 0, TBPRD, CMPA and CMPB for eHRPWM0B
STATUS EhrPwmSetTbAQConfBSc(struct EhrPwmSetTbAQConfBScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbAQConfB(pArgs->tb, pArgs->ehrpwm_inst);	
}

// set the value of the current PWM Counter Direction [CTRMODE] & Maximum Period Counter Value(PWM Frequency[TBPRD]
STATUS EhrPwmSetTbPWMCfgSc(struct EhrPwmSetTbPWMCfgScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbPWMCfg(pArgs->freq, pArgs->dir, pArgs->ehrpwm_inst);	
}

// set the maximum counter value
INT32 EqepSetMaxCountSc(struct EqepSetMaxCountScArgs *pArgs)
{
	return (INT32)vxbEqepSetMaxCount(pArgs->eqep, pArgs->count);
}

// get the current position counter value
INT32 EqepGetPosCountSc(struct EqepGetPosCountScArgs *pArgs)
{
	return (INT32)vxbEqepGetPosCount(pArgs->eqep);
}

// delay for 'delayTime' milliseconds
void MsDelaySc(struct MsDelayScArgs *pArgs)
{
	return vxbMsDelay(pArgs->delayTime);
}

// Spi Transfer Function
STATUS McSpiTransSc(struct McSpiTransScArgs * pArgs)
{
	return (INT32)vxbMcSpiTrans(pArgs->devInfo, pArgs->pPkg, pArgs->channel);
}
INT32 GpioFreeSc(struct GpioFreeScArgs *pArgs)
{
	return (INT32)vxbGpioFree(pArgs->id);
}
INT32 SioPollInputSc(struct SioPollInputScArgs *pArgs)
{
	return (INT32)sioPollInput(pArgs->pChan, pArgs->pChar);
}

INT32 SioPollOutputSc(struct SioPollOutputScArgs *pArgs)
{
	return (INT32)sioPollOutput(pArgs->pChan, pArgs->outChar);
}
/**************************************************LOGGING LEVELS*****************************************************************/

/*This function gets the time and convert this broken time into string format*/
STATUS getTimeStamp(UINT8 *timebuff)
{
	struct tm rtcTimePar;
	UINT8 timeBuf[sizeof(RTCtimerStr)+1]; //Buffer for date and time
	
	if( vxbRtcGet(&rtcTimePar) == ERROR)
	{
		kprintf("\nError in Getting the RTC Time!!!\n");
		return ERROR;
	}
	bzero(timeBuf, sizeof(timeBuf));
	
	if(asctime_r (&rtcTimePar, timeBuf)==NULL)
	{
		kprintf("Error in Converting broken time into String format\n");
		return ERROR;
	}

	memcpy(timebuff, (char *)timeBuf, strlen(timeBuf));
	timebuff[strlen(timeBuf)-1]='\0';
	return OK;
}

/*This function writes formatted string output into the output buffer*/
STATUS printBuf(UINT8 *inbuf, size_t nbytes, UINT8 **outbuf)
{
	bcopy (inbuf, *outbuf, (size_t)nbytes);     
    *outbuf += nbytes;
    if(nbytes>MAX_LOG_FMT_BUFSIZE)
    	return ERROR;
	return OK;
}

/*This function converts unformatted string to formatted string*/
STATUS bufPrintf(UINT8 *buff,  const UINT8 *  fmt,  ...)	 /*fmt- format string to write */
{
	INT32 nChars;
	va_list vaList; /* traverses argument list */
	
	va_start (vaList, fmt);
	nChars = fioFormatV (fmt, vaList, (FIOFORMATV_OUTPUT_FUNCPTR) printBuf, (_Vx_usr_arg_t)&buff);
	va_end(vaList);
	return (nChars);
}

/*This Custom SystemCall shows all the information of all critical issues */
STATUS logCRITSc(struct logCRITScArgs *logInf) 
{
	char logFmtBuffer[LOGDATA_SIZE];
	UINT8 timeBuf[sizeof(RTCtimerStr)+1], *strBuf="%s"; //Buffer for date and time
	LogData logData;
	LogMessage loggerMsg;
	logData.LogLevel=LOG_CRITICAL;
	
	if(logInf->logLevel==DISABLE_LOGS)
		return ERROR;
	else if((logInf->logLevel>=logData.LogLevel) && (logInf->logLevel <= MAX_LOG_LEVELS)) //Enables for all log levels
	{
		strcpy(logData.STATUS,"< C >");
		globalLogCount++;
		memset(timeBuf, 0x00, sizeof(timeBuf));
		getTimeStamp(timeBuf); //It will get the time stamp
				
		memset(logData.LogingDataBuf, 0x00, sizeof(logData.LogingDataBuf));
		sprintf (logData.LogingDataBuf, "<LC: %d> ~ < %s > ~ %s < Line No: %d > < File: %s > : %s \n",globalLogCount,strBuf, logData.STATUS, logInf->lineNo, logInf->fileName,  (char *)logInf->logData);
		
		memset(logFmtBuffer, 0x00, sizeof(logFmtBuffer));
		if(bufPrintf(logFmtBuffer,logData.LogingDataBuf, timeBuf, logInf->arg1, logInf->arg2, logInf->arg3)<0) //This function converts the unformatted string into formatted string and print into the buffer
		{
			kprintf("logCRITSc: Error in Formatting String\n");
			return ERROR;
		}

		loggerMsg.LogLevel=logData.LogLevel;
		loggerMsg.FileFlag=logInf->logToFile;
		memset(loggerMsg.Buffer, 0x00, sizeof(loggerMsg.Buffer));
		memcpy(loggerMsg.Buffer, logFmtBuffer, strlen(logFmtBuffer)+1);
		
		loggerMsgQId = msgQOpen("/LOG_Task_Q", 0, 0, 0, 0, 0);
		
		if (loggerMsgQId == MSG_Q_ID_NULL)
		{
			kprintf("logCRITSc: Error in opening Message Queue\n");
			return ERROR;
		}
		
		if(msgQSend(loggerMsgQId, (char *)&loggerMsg, sizeof(loggerMsg), NO_WAIT,  MSG_PRI_URGENT)==ERROR)
		{
			kprintf("logCRITSc: Error in sending Message\n");
			return ERROR;
		}
				
	
	}
	
	return OK;
}

/*This Custom SystemCall gives Information of Errors */
STATUS logERRSc(struct logERRScArgs *logInf)
{
	char logFmtBuffer[LOGDATA_SIZE];
	LogData logData;
	LogMessage loggerMsg;
	logData.LogLevel=LOG_ERROR;
	if(logInf->logLevel==DISABLE_LOGS)
		return ERROR;
	else if((logInf->logLevel>=logData.LogLevel) && (logInf->logLevel <= MAX_LOG_LEVELS)) //Enables for all log levels
	{
		strcpy(logData.STATUS,"< E >");
		globalLogCount++;
		
		memset(logData.LogingDataBuf, 0x00,  sizeof(logData.LogingDataBuf));
		sprintf (logData.LogingDataBuf, "<LC: %d> ~ %s < Line No: %d > < File: %s > : %s \n",globalLogCount, logData.STATUS, logInf->lineNo, logInf->fileName, (char *)logInf->logData);
		
		memset(logFmtBuffer, 0x00, sizeof(logFmtBuffer));
		if(bufPrintf(logFmtBuffer,logData.LogingDataBuf, logInf->arg1, logInf->arg2, logInf->arg3)<0)
		{
			kprintf("logERRSc: Error in Formatting String\n");
			return ERROR;
		}
		
		loggerMsg.LogLevel=logData.LogLevel;
		loggerMsg.FileFlag=logInf->logToFile;
		memset(loggerMsg.Buffer, 0x00, sizeof(loggerMsg.Buffer));
		memcpy(loggerMsg.Buffer, logFmtBuffer, strlen(logFmtBuffer)+1);
		
		loggerMsgQId = msgQOpen("/LOG_Task_Q", 0, 0, 0, 0, 0);
		
		if (loggerMsgQId == MSG_Q_ID_NULL)
		{
			kprintf("logERRSc: Error in opening Message Queue\n");
			return ERROR;
		}
		
		if(msgQSend(loggerMsgQId, (char *)&loggerMsg, sizeof(loggerMsg), NO_WAIT,  MSG_PRI_URGENT)==ERROR)
		{
			kprintf("logERRSc: Error in sending Message\n");
			return ERROR;
		}
			
	}
	return OK;
}

/*This Custom SystemCall shows all Warnings */
STATUS logWARNSc(struct logWARNScArgs *logInf)
{
	char logFmtBuffer[LOGDATA_SIZE];
	LogData logData;
	LogMessage loggerMsg;
	logData.LogLevel=LOG_WARNING;
	
	if(logInf->logLevel==DISABLE_LOGS)
		return ERROR;
	else if((logInf->logLevel>=logData.LogLevel) && (logInf->logLevel <= MAX_LOG_LEVELS)) //Enables logs till level 3
	{	
		strcpy(logData.STATUS,"< W >");
		globalLogCount++;
		
		memset(logData.LogingDataBuf, 0x00, sizeof(logData.LogingDataBuf));
		sprintf (logData.LogingDataBuf, "<LC: %d> ~ %s < Line No: %d > < File: %s > : %s \n",globalLogCount, logData.STATUS, logInf->lineNo, logInf->fileName, (char *)logInf->logData);
		
		memset(logFmtBuffer, 0x00, sizeof(logFmtBuffer));
		
		if(bufPrintf(logFmtBuffer,logData.LogingDataBuf, logInf->arg1, logInf->arg2, logInf->arg3)<0)
		{
			kprintf("logWARNSc: Error in Formatting String\n");
			return ERROR;
		}
		
		loggerMsg.LogLevel=logData.LogLevel;
		loggerMsg.FileFlag=0;
		memset(loggerMsg.Buffer, 0x00, sizeof(loggerMsg.Buffer));
		memcpy(loggerMsg.Buffer, logFmtBuffer, strlen(logFmtBuffer)+1);
		
		loggerMsgQId = msgQOpen("/LOG_Task_Q", 0, 0, 0, 0, 0);
		
		if (loggerMsgQId == MSG_Q_ID_NULL)
		{
			kprintf("logWARNSc: Error in opening Message Queue\n");
			return ERROR;
		}
		
		if(msgQSend(loggerMsgQId, (char *)&loggerMsg, sizeof(loggerMsg), NO_WAIT,  MSG_PRI_NORMAL)==ERROR)
		{
			kprintf("logWARNSc: Error in sending Message\n");
			return ERROR;
		}
	}
	return OK;
}

/*This Custom SystemCall gives Debugging Information */
STATUS logDBGSc(struct logDBGScArgs *logInf)
{
	char logFmtBuffer[LOGDATA_SIZE];
	LogData logData;
	LogMessage loggerMsg;
	logData.LogLevel=LOG_DEBUG;
	
	if(logInf->logLevel==DISABLE_LOGS)
		return ERROR;
	else if(logInf->logLevel==logData.LogLevel) //Enables all log levels (i.e upto level 4)
	{
		strcpy(logData.STATUS,"< D >");
		globalLogCount++;
		
		memset(logData.LogingDataBuf, 0x00, sizeof(logData.LogingDataBuf));
		sprintf (logData.LogingDataBuf, "<LC: %d> ~ %s < Line No: %d > < File: %s > : %s \n",globalLogCount, logData.STATUS, logInf->lineNo, logInf->fileName, (char *)logInf->logData);
		
		memset(logFmtBuffer, 0x00, sizeof(logFmtBuffer));
		if(bufPrintf(logFmtBuffer,logData.LogingDataBuf, logInf->arg1, logInf->arg2, logInf->arg3)<0)
		{
			kprintf("logDBGSc: Error in Formatting String\n");
			return ERROR;
		}
		
		loggerMsg.LogLevel=logData.LogLevel;
		loggerMsg.FileFlag=0;
		memset(loggerMsg.Buffer, 0x00, sizeof(loggerMsg.Buffer));
		memcpy(loggerMsg.Buffer, logFmtBuffer, strlen(logFmtBuffer)+1);
		
		loggerMsgQId = msgQOpen("/LOG_Task_Q", 0, 0, 0, 0, 0);
		
		if (loggerMsgQId == MSG_Q_ID_NULL)
		{
			kprintf("logDBGSc: Error in opening Message Queue\n");
			return ERROR;
		}
		
		if(msgQSend(loggerMsgQId, (char *)&loggerMsg, sizeof(loggerMsg), NO_WAIT,  MSG_PRI_NORMAL)==ERROR)
		{
			kprintf("logDBGSc: Error in sending Message\n");
			return ERROR;
		}
	}
	return OK;
}
// PWM Timer ISR
void Am5728pwmISR()
{	
	if(eventSend(taskIdServo, VXEV04) == ERROR)
		kprintf("Error in sending an event\n");
}


// Get the value of the Time base Status Register [TBSTS]
INT32 Am5728EhrPwmRegisterISRSc(struct EhrPwmRegisterISRScArgs *pArgs)
{
	taskIdServo = taskOpen((UINT8 *)"/ServoISR", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);	
	return (INT32)vxbEhrPwmRegisterISR((EHRPWMDRV_IRQ_HANDLER)Am5728pwmISR, pArgs->ehrpwmHandlerArgs, pArgs->ehrpwm_inst, pArgs->period, pArgs->mode);
}


// set Software Force & also set Action when One-Time Software Force A & B Is invoked
STATUS Am5728EhrPwmSetAQSFRCSc(struct EhrPwmSetAQSFRCScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetAQSFRC(pArgs->value, pArgs->inst);	
}

// set the divisor of the time base clock
STATUS Am5728EhrPwmSetTbClkSc(struct EhrPwmSetTbClkScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbClk(pArgs->tb, pArgs->ehrpwm_inst);	
}

// set the actions to take when TBCNT reach 0, TBPRD, CMPA and CMPB for eHRPWM0A
STATUS Am5728EhrPwmSetTbAQConfASc(struct EhrPwmSetTbAQConfAScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbAQConfA(pArgs->tb, pArgs->ehrpwm_inst);	
}

// set the actions to take when TBCNT reach 0, TBPRD, CMPA and CMPB for eHRPWM0B
STATUS Am5728EhrPwmSetTbAQConfBSc(struct EhrPwmSetTbAQConfBScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbAQConfB(pArgs->tb, pArgs->ehrpwm_inst);	
}

// set the value of the current PWM Counter Direction [CTRMODE] & Maximum Period Counter Value(PWM Frequency[TBPRD]
STATUS Am5728EhrPwmSetTbPWMCfgSc(struct EhrPwmSetTbPWMCfgScArgs *pArgs)
{
	return (INT32)vxbEhrPwmSetTbPWMCfg(pArgs->freq, pArgs->dir, pArgs->ehrpwm_inst);	
}

/**************************************************RUNTIME FEATURE*****************************************************************/

/******************************************************************************
*
* CheckEepromModelEntry 
*
* This routine  is for checking eeprom whether model
* is present or not.
*
* RETURNS: TRUE if model present, OK if not
*
* ERRNO: N/A
*/

STATUS CheckEepromModelEntry(INT32 Offset,INT32 length)
{
	char EepromBuf;
	int status=TRUE;
	
	if ((EepromFd = open(EEPROM_PATH, O_RDWR, 0)) == ERROR)
	{
		kprintf("CheckEepromModelEntry: Failed to open the fd..\n");
	}
	
	ioctl (EepromFd, FIOSEEK, Offset);
	read(EepromFd, (char *)&EepromBuf, BYTESIZE);
	
	if(EepromBuf==RUNTIMEDATA)
	{
		status=OK;
	}
	
	return status;
		
}

/******************************************************************************
*
* EepromWrite 
*
* This routine  is for writing to eeprom
*
* RETURNS:TRUE on successful write else returns error
*
* ERRNO: N/A
*/

STATUS EepromWrite(INT32 Offset,INT32 length, UINT8* FeaturesFlag)
{
	INT32 status,writeSize;
	
	if((status=ioctl (EepromFd, FIOSEEK,Offset))==ERROR)
	{	
		kprintf(" EepromWrite: Error in setting Address\n");
		return ERROR;
	}	
	
	writeSize=write(EepromFd,(UINT8 *)FeaturesFlag,length);
	
	return TRUE;
}

/******************************************************************************
*
* EepromRead 
*
* This routine  is for reading from eeprom
*
* RETURNS: TRUE on successful read else returns error
*
* ERRNO: N/A
*/

STATUS EepromRead(INT32 Offset,INT32 length, void* FeatureBuf)
{
	INT32 status,readSize;
	
	if((status=ioctl (EepromFd, FIOSEEK,Offset))==ERROR)
	{
		kprintf(" EepromRead: Error in IOCTL setting of Eeprom address\n");
		return ERROR;
	}
	
	if((readSize=read(EepromFd,(UINT8 *)FeatureBuf,length))==ERROR)
	{
		kprintf(" EepromRead: Error in eeprom read \n");
		return ERROR;	
	}
	
	return TRUE;	
}

/******************************************************************************
*
* CheckModel 
*
* This routine  is for checking the feature present in the eeprom
*
* RETURNS: Model present in eeprom
*
* ERRNO: N/A
*/

ModelID CheckModel(INT32 Offset)
{
	INT32	ReadStatus;
	UINT8 ModelBuffer,modelID;
	
	EepromRead((Offset+FEATUREFLAG_SIZE-1),sizeof(UINT8),&ModelBuffer);
	modelID= (ModelBuffer & MODELID_MASK);
	
	return modelID;
}

/************************************************
 * getDefaultModel
 * 
 * This routine reads the default model
 *  
 * RETURN :TRUE on finding default model
 * 
 */

STATUS getDefaultModel(UINT8 modelID,UINT8 *FeatureArray)
{
	
	if(modelID==NEPTUNE_ID)
	{
		memcpy(FeatureArray,featureNeptune,FEATUREFLAG_SIZE);
	}
	else if(modelID==P1_PNEUMATIC_ID)
	{
		memcpy(FeatureArray,featureP1Pneumatic,FEATUREFLAG_SIZE);
	}
	else if(modelID==P2_PNEUMATIC_ID)
	{
		memcpy(FeatureArray,featureP2Pneumatic,FEATUREFLAG_SIZE);
	}
	else if(modelID==I_IW_ID)
	{
		memcpy(FeatureArray,featureIIW,FEATUREFLAG_SIZE);
	}
	else if(modelID==MERCURY_ID)
	{
		memcpy(FeatureArray,featureMercury,FEATUREFLAG_SIZE);
	}
	else if(modelID==SATURNP1_ID)
	{
		memcpy(FeatureArray,featureSaturnP1,FEATUREFLAG_SIZE);
	}
	else if(modelID==SATURNP2_ID)
	{
		memcpy(FeatureArray,featureSaturnP2,FEATUREFLAG_SIZE);
	}
	else if(modelID==MARS_ID)
	{
		memcpy(FeatureArray,featureMars,FEATUREFLAG_SIZE);
	}
	else if(modelID==JUNO_ID)
	{
		memcpy(FeatureArray,featureJuno,FEATUREFLAG_SIZE);
	}
	else if(modelID==VENUS_ID)
	{
		memcpy(FeatureArray,featureVenus,FEATUREFLAG_SIZE);
	}
	else if(modelID==VENUSHH_ID)
	{
		memcpy(FeatureArray,featureVenusHH,FEATUREFLAG_SIZE);
	}
	else
	{
		return ERROR;
	}
	
	return TRUE;
	
}

/******************************************************************************
*
* changeModelWrite 
*
* This routine  is for  writing the modified Featureflag into eeprom
*
* RETURNS: TRUE on write
*
* ERRNO: ERROR
*/

STATUS changeModelWrite()
{
	INT32 	index1,index2,statusValue,ArrayIndex,ReadStatus,DefaultValue,WriteStatus;	
	UINT8  	ModelFeatureList[FEATUREFLAG_SIZE],modelID,DefaultFeature[FEATUREFLAG_SIZE],featureStatus=0;

	if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModelFeatureList))==ERROR)
	{		
		printf(" changeModelWrite : Error in eeprom read\n");
		return ERROR;
	}
	
	modelID=CheckModel(EEPROM_MODEL_ADDRESS);
	getDefaultModel(modelID,DefaultFeature);

	for(ArrayIndex=0;ArrayIndex<RTF_LIST1_SIZE;ArrayIndex++)
		{
			for(index1=0;index1<NUMFLAG_INBYTES;index1++)
			{
				index2=NUMBITS_PERFLAG*index1;
				statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
				DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;
				
				if(statusValue==RTF_NA && (DefaultValue==RTF_NA))
				{
					statusValue=DISABLE;
				}
				else if(statusValue==RTF_BASE && (DefaultValue==RTF_BASE))
				{
					statusValue=ENABLE;
				} 
				else if(statusValue==RTF_LTD && (DefaultValue==RTF_LTD))
				{
					statusValue=ENABLE;		
				}
				else if((statusValue==RTF_OPT) || (DefaultValue==RTF_OPT))
				{
					if(statusValue==RTF_OPT)
					{
						statusValue=DISABLE;	
					}
				}	
				featureStatus= featureStatus | (statusValue<<index2);
			}
			ModelFeatureList[ArrayIndex]=featureStatus;
			featureStatus=0;
		}
		
		for(ArrayIndex=RTF_LIST1_SIZE;ArrayIndex<(FEATUREFLAG_SIZE-1);ArrayIndex++)
		{
			for(index1=0;index1<(FEATUREFLAG_SIZE/3);index1++)
			{
				index2=NUMBITS_PERFLAG*index1;
				statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
				DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;
				if(statusValue==RTF_NA && (DefaultValue==RTF_NA))
				{
					statusValue=DISABLE;
				}
				else if(statusValue==RTF_BASE && (DefaultValue==RTF_BASE))
				{
					statusValue=ENABLE;
				}
				else if(statusValue==RTF_LTD && (DefaultValue==RTF_LTD))
				{
					statusValue=ENABLE;
				}
				else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
				{
					if(statusValue==3)
					{
						statusValue=DISABLE;
					}
				}
				featureStatus= featureStatus | (statusValue<<index2);
			}
			
			ModelFeatureList[ArrayIndex]=featureStatus;
			featureStatus=0;
		}
		
		for(index1=0;index1<(RTF_LIST2_SIZE/2);index1++)
		{
			index2=NUMBITS_PERFLAG*index1;
			statusValue=(ModelFeatureList[(FEATUREFLAG_SIZE-1)]>>index2) & FLAGMASK;
			DefaultValue=(DefaultFeature[FEATUREFLAG_SIZE-1]>>index2) & FLAGMASK;
			if(statusValue==RTF_NA && (DefaultValue==RTF_NA))
			{
				statusValue=DISABLE;
			}
			else if(statusValue==RTF_BASE && (DefaultValue==RTF_BASE))
			{
				statusValue=ENABLE;
			}
			else  if(statusValue==RTF_LTD && (DefaultValue==RTF_LTD))
			{
				statusValue=ENABLE;
			}
			else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
			{
				if(statusValue==3)
				{
					statusValue=DISABLE;
				}
			}
			
			featureStatus= featureStatus | (statusValue<<index2);
		} 
		
		ModelFeatureList[FEATUREFLAG_SIZE-1]=featureStatus;
		featureStatus=0;
		
		for(index1=NUMFLAG_INBYTES;index1<RTF_LIST1_SIZE;index1++)
		{
			statusValue=(DefaultFeature[FEATUREFLAG_SIZE-1]>>index1) & MODELBIT_MASK;
			featureStatus= featureStatus | (statusValue<<index1);
		} 
		
		ModelFeatureList[FEATUREFLAG_SIZE-1]=(ModelFeatureList[FEATUREFLAG_SIZE-1]|featureStatus);
		
		if((WriteStatus=EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModelFeatureList))==ERROR)
		{
			kprintf(" changeModelWrite : ERROR in EEPROM Write \n");
		}
		
		return TRUE;
}
 
/******************************************************************************
*
* RunTimeFeatureDisplay 
*
* This routine  is for  displaying the features of the particular model.
*
* RETURNS: TRUE on display
*
* ERRNO: ERROR
*/

STATUS  RunTimeFeatureDisplay(int Offset,int length )
{
	UINT8 Feature_Name [][30]={	"MULTIMODE           ", "TIME                ", "ENERGY              ", "PEAK POWER          ", 
								"COLLAPSE DISTANCE   ", "ABSOLUTE DISTANCE   ", "GROUND DETECT       ",	"CONTINOUS           ",	
								"PRETRIGGER          ",	"AFTERBURST          ", 	"ENERGYBRAKE         ","AMPLITUDE STEP      ",
								"FORCE STEP          ",	"ALARM LOG           ", 	"EVENT LOG           ",  "WELD DATA LOG       ",
								"REPORTING           ",		"CRACKED HORN        ",		"USB                 ",		 "BINARY              ",
								"WEB SERVICES        ",		"OPC UA              ",		"USER I/O            ",		"ACTUATOR I/O        ",
								"CYCLE COUNTER       ",		"BATCH COUNTER       ",		"POWER MATCH CURVE   ",		"SMARTCONVERTER      ",
								"CONTROL SPEED       ",		"SEQUENCING          ",		"AUTOCONVERTERCOOLING",		"FIELD BUS           ",
								"DASHBOARD           ",		"PASSWORD            ",		"MOBILE              ",		"OVERLAY             ",
								"TRENDS              ",		"OPTITUNE            ","LIMITS              ",	"HD MODE             ",
								"TEACH MODE          ",		"ADV COMM            ",	"EXTRA DATA STORAGE  ",		"TROUBLE SHOOT       ",
								"CFR                  ","HOUR COUNTER         "	};

	INT32 index1,index2,statusValue,ArrayIndex,FeatureIndex=0,ReadStatus,DefaultValue;	
	UINT8  ModelFeatureList[FEATUREFLAG_SIZE],FeatureCategory[FEATURECATEGORY_SIZE],modelID,DefaultFeature[FEATUREFLAG_SIZE];
	
	if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModelFeatureList))==ERROR)
	{		
		kprintf(" RunTimeFeatureDisplay : Error in eeprom read\n");
		return ERROR;
	}
	
	modelID=CheckModel(EEPROM_MODEL_ADDRESS);
	getDefaultModel(modelID,DefaultFeature);
	
	kprintf("---------------------------------------------------------------------------------\n");
	kprintf(" \t FEATURES \t\t |\tFeature Category   |  Feature Status\t| \n");
	kprintf("---------------------------------------------------------------------------------\n");
	
	for(ArrayIndex=0;ArrayIndex<RTF_LIST1_SIZE;ArrayIndex++)
	{
		for(index1=0;index1<NUMFLAG_INBYTES;index1++,FeatureIndex++)
		{
			index2=NUMBITS_PERFLAG*index1;
			statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
			DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;
			
			if(DefaultValue==RTF_NA)
			{
				strcpy(FeatureCategory,"NOT SUPPORTED");	
			}
			else if(DefaultValue==RTF_BASE)
			{
				strcpy(FeatureCategory,"BASE         ");
			}
			else if(DefaultValue==RTF_LTD)
			{
				strcpy(FeatureCategory,"LIMITED");
				statusValue=ENABLE;
			}
			else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
			{
				strcpy(FeatureCategory,"OPTIONAL");
				if(statusValue==RTF_OPT)
				{
					statusValue=DISABLE;
				}
			}
			kprintf("  %d )	%s\t |\t %s \t   |         %d  \t|\n",FeatureIndex,Feature_Name[FeatureIndex],FeatureCategory,statusValue);
		}
	}
	
	for(ArrayIndex=RTF_LIST1_SIZE;ArrayIndex<(FEATUREFLAG_SIZE-1);ArrayIndex++)
	{
		for(index1=0;index1<(FEATUREFLAG_SIZE/3);index1++,FeatureIndex++)
		{
			index2=NUMBITS_PERFLAG*index1;
			statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
			DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;
			
			if (DefaultValue==RTF_NA)
			{
				strcpy(FeatureCategory,"NOT SUPPORTED");
			}
			else if(DefaultValue==RTF_BASE)
			{
				strcpy(FeatureCategory,"BASE         ");
			}
			else if(DefaultValue==RTF_LTD)
			{
				strcpy(FeatureCategory,"LIMITED");
				statusValue=ENABLE;
			}
			else if(DefaultValue==RTF_OPT)
			{
				strcpy(FeatureCategory,"OPTIONAL");
				if(statusValue==RTF_OPT)
				{
					statusValue=DISABLE;
				}
			}
			kprintf("  %d )	%s\t |\t %s \t   |         %d  \t|\n",FeatureIndex,Feature_Name[FeatureIndex],FeatureCategory,statusValue);
		}
	}
	
	for(index1=0;index1<(RTF_LIST2_SIZE/2);index1++,FeatureIndex++)
	{
		index2=NUMBITS_PERFLAG*index1;
		statusValue=(ModelFeatureList[(FEATUREFLAG_SIZE-1)]>>index2) & FLAGMASK;
		DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;
		
		if(DefaultValue==RTF_NA)
		{
			strcpy(FeatureCategory,"NOT SUPPORTED");
		}
		else if(DefaultValue==RTF_BASE)
		{
			strcpy(FeatureCategory,"BASE         ");
		}
		else if(DefaultValue==RTF_LTD)
		{
			strcpy(FeatureCategory,"LIMITED");
			statusValue=ENABLE;
		}
		else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
		{
			strcpy(FeatureCategory,"OPTIONAL");
			if(statusValue==RTF_OPT)
			{
				statusValue=DISABLE;
			}
		}
		kprintf("  %d )	%s\t |\t %s \t   |         %d  \t|\n",FeatureIndex,Feature_Name[FeatureIndex],FeatureCategory,statusValue);
	} 
	
	kprintf("---------------------------------------------------------------------------------\n");	
	kprintf("\n"); 
	return TRUE;
}

/******************************************************************************
*
* DisplayModel 
*
* This routine  is for checking and displaying the model present in the eeprom.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

STATUS DisplayModel(int Offset)
{
	INT32 ReadStatus;
	UINT8 ModelBuffer,modelID;
	
	if((ReadStatus=EepromRead((Offset+FEATUREFLAG_SIZE-1),BYTESIZE,(UINT8 *)&ModelBuffer))==ERROR)
	{		
	kprintf(" DisplayModel : Error in eeprom read\n");
	return ERROR;
	}
	
	modelID= (ModelBuffer & MODELID_MASK);
	
	if(modelID==NEPTUNE_ID)
	{
		kprintf("			******* MODEL IS NEPTUNE ******* \n\n");
	}
	else if(modelID==P1_PNEUMATIC_ID)
	{
		kprintf("			******* MODEL IS P1 PNEUMATIC ******* \n\n");
	}
	else if(modelID==P2_PNEUMATIC_ID)
	{
		kprintf("			******* MODEL IS P2 PNEUMATIC ******* \n\n");
	}
	else if(modelID==I_IW_ID)
	{
		kprintf("			******* MODEL IS I(IW) ******* \n\n");
	}
	else if(modelID==MERCURY_ID)
	{
		kprintf("			******* MODEL IS MERCURY ******* \n\n");
	}
	else  if(modelID==SATURNP1_ID)	
	{		
		kprintf("			******* MODEL IS SATURNP1 ******* \n\n ");
	} 
	else  if(modelID==SATURNP2_ID)	
	{
		kprintf("			******* MODEL IS SATURN P2 ******* \n\n ");	
	} 
	else if(modelID==MARS_ID)			
	{		
		kprintf("			******* MODEL IS MARS ******* \n\n ");
	}
	else if(modelID==JUNO_ID)
	{
		kprintf("			******* MODEL IS JUNO ******* \n\n");
	}
	else  if(modelID==VENUS_ID)	
	{		
		kprintf("			******* MODEL IS VENUS ******* \n\n ");
	}
	else  if(modelID==VENUSHH_ID)	
	{		
		kprintf("			******* MODEL IS VENUS-HH ******* \n\n ");
	}
	else
	{
		kprintf("			******* WRONG MODEL ******* \n \n");	
	}
	return TRUE;
}

/******************************************************************************
*
* ModifyFeature -Modify the feature of model
*
* This routine  is for modifying  the feature flags for model present
* in EEPROM
*
* RETURNS: Modified FeaturesFlag
*
* ERRNO: N/A
*/

STATUS ModifyFeature(int Offset,int length)
{
	INT32 value,UserChoice,FeatureChoice,ReadStatus;
	UINT8  Feature[FEATUREFLAG_SIZE],ModelBuffer,modelID,FeatureArray[FEATUREFLAG_SIZE],ModelFeatureList[FEATUREFLAG_SIZE];
	FeaturesFlag ModifiedFeature={0};
	
	if((ReadStatus=EepromRead( (Offset+FEATUREFLAG_SIZE-1),BYTESIZE,(UINT8 *)&ModelBuffer))==ERROR)
	{
		kprintf(" ModifyFeature : EEPROM read in Modify Feature\n");
		return ERROR;
	}
	
	modelID= (ModelBuffer & MODELID_MASK);
	
	if((ReadStatus=EepromRead(Offset ,FEATUREFLAG_SIZE,(UINT8 *)&Feature))==ERROR)
	{
		kprintf(" ModifyFeature : EEPROM read in Modify Feature\n");
		return ERROR;
	}

	memcpy(&ModifiedFeature,&Feature,length);
	
	// Logic to Enable/Disable features
	do
	{
		kprintf(" press '1' to enable/disable feature else press '0'\n");
		scanf("%d",&UserChoice);
		
		if(UserChoice==1)
		{
			kprintf("enter feature number (0-45)  \n");
			scanf("%d",&FeatureChoice);

			if(modelID==NEPTUNE_ID)
			{
				if ((FeatureChoice == FIELDBUS) || (FeatureChoice == HD_MODE) || (FeatureChoice == ADV_COMM))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					if((FeatureChoice == FIELDBUS))
						{
							ModifiedFeature.RtFeature1.FieldBus_Flag_B31=value;
						}
					else if((FeatureChoice == HD_MODE))	
						{
							ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
						}
						else if((FeatureChoice == ADV_COMM))
						{
							ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
						}
				}
				else if (FeatureChoice ==CONTINOUS) 
				{
					kprintf("FEATURE NOT SUPPORTED \n\n");	
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
				}	
			}
			
			if(modelID==P1_PNEUMATIC_ID)
			{
				if ((FeatureChoice ==REPORTING) || (FeatureChoice == AUTOCONVERTERCOOLING) || (FeatureChoice == FIELDBUS) ||  
					(FeatureChoice == OVERLAY) ||(FeatureChoice == TRENDS) || (FeatureChoice == HD_MODE) || 
					(FeatureChoice == TEACH_MODE) ||(FeatureChoice == ADV_COMM) || (FeatureChoice == EXTRADATASTORAGE))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					if((FeatureChoice == REPORTING))
					{
						ModifiedFeature.RtFeature1.Reporting_Flag_B16=value;
					}
					else if((FeatureChoice ==AUTOCONVERTERCOOLING ))
					{
						ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
					}
					else if((FeatureChoice == FIELDBUS))
					{
						ModifiedFeature.RtFeature1.FieldBus_Flag_B31=value;
					}
					else if((FeatureChoice == OVERLAY))	
					{
						ModifiedFeature.RtFeature2.Overlay_Flag_B35 =value;
					}
					else if((FeatureChoice == TRENDS))
					{
						ModifiedFeature.RtFeature2.Trends_Flag_B36 =value;
					}
					else if((FeatureChoice == HD_MODE))
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
					}
					else if((FeatureChoice == TEACH_MODE))
					{
						ModifiedFeature.RtFeature2.TeachMode_Flag_B40 =value;
					}
					else if(FeatureChoice == ADV_COMM)
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
					}
					else if((FeatureChoice == EXTRADATASTORAGE))
					{
						ModifiedFeature.RtFeature2.ExtraDataStorage_Flag_B42 =value;
					}
				}
				else if ((FeatureChoice == CONTINOUS) ||  (FeatureChoice == FORCESTEP) ||  (FeatureChoice == POWERMATCHCURVE) ||
						(FeatureChoice == SMART_CONVERTER)||  (FeatureChoice == CONTROLSPEED)||  (FeatureChoice == OPTITIUNE)||  
						(FeatureChoice == CFR)) 
				{
					kprintf("FEATURE NOT SUPPORTED \n");	
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n \n");
				}	
			}
			
			if(modelID==P2_PNEUMATIC_ID)
			{
				if ((FeatureChoice == AUTOCONVERTERCOOLING)||(FeatureChoice == FIELDBUS)||(FeatureChoice == HD_MODE)||(FeatureChoice ==ADV_COMM)) 
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
				
					if(FeatureChoice == AUTOCONVERTERCOOLING)
					{
						ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
					}
					else if(FeatureChoice == FIELDBUS)
					{
						ModifiedFeature.RtFeature1.FieldBus_Flag_B31=value;
					}
					else if(FeatureChoice == HD_MODE)
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
					}
					else if(FeatureChoice == ADV_COMM)
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
					}
				}
				else if (FeatureChoice ==CONTINOUS) 
				{
					kprintf("FEATURE NOT SUPPORTED \n");	
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
				}		
			}
			
			if(modelID==I_IW_ID)
			{
				if ((FeatureChoice == ENERGY) || (FeatureChoice == COLLLAPSEDIST) ||(FeatureChoice == ABSOLUTEDIST))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
				
					if((FeatureChoice == ENERGY))
					{
						ModifiedFeature.RtFeature1.Energy_Flag_B2=value;
					}
					else if(FeatureChoice == COLLLAPSEDIST)
					{
						ModifiedFeature.RtFeature1.CollapseDist_Flag_B4=value;
					}
					else if((FeatureChoice == ABSOLUTEDIST))
					{
						ModifiedFeature.RtFeature1.AbsoluteDist_Flag_B5 =value;
					}
				}
				else if ((FeatureChoice == TIMEMODE) ||(FeatureChoice == GROUND_DETECT) || (FeatureChoice == PRETRIGGER) || 
						(FeatureChoice == AFTERBURST) ||  (FeatureChoice == ALARMLOG) || (FeatureChoice == EVENTLOG)|| 
						(FeatureChoice == WELDDATALOG)||(FeatureChoice == CRACKEDHORN)|| (FeatureChoice ==USER_IO)|| 
						(FeatureChoice == ACTUATOR_IO)||  (FeatureChoice == CYCLECOUNTER)|| (FeatureChoice == PASSWORD)|| 
						(FeatureChoice == LIMITS)|| (FeatureChoice == HD_MODE))
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");		
				}
				else
				{
					kprintf("FEATURE NOT SUPPORTED \n");
				}	
			}
			
			if(modelID==MERCURY_ID)
			{
				if ((FeatureChoice == DASHBOARD)|| (FeatureChoice == HD_MODE)||(FeatureChoice == ADV_COMM))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					if((FeatureChoice == DASHBOARD))
					{
						ModifiedFeature.RtFeature2.DashBoard_Flag_B32=value;
					}
					else if((FeatureChoice == HD_MODE))					
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
					}
					else if((FeatureChoice == ADV_COMM))
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
					}
				}
				else if ((FeatureChoice == 28) ||  (FeatureChoice == OPTITIUNE))
				{
					kprintf("FEATURE NOT SUPPORTED \n");	
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
				}
			}
			
			if(modelID==SATURNP1_ID)
			{
				if ((FeatureChoice == AUTOCONVERTERCOOLING)||(FeatureChoice == DASHBOARD)||(FeatureChoice == HD_MODE)|| 
					(FeatureChoice == REPORTING)||(FeatureChoice == TEACH_MODE)||(FeatureChoice == ADV_COMM)||(FeatureChoice == EXTRADATASTORAGE))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					
					if(FeatureChoice == REPORTING)
					{
						ModifiedFeature.RtFeature1.Reporting_Flag_B16=value;
					}
					else if((FeatureChoice == AUTOCONVERTERCOOLING))
					{
						ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
					}
					else if(FeatureChoice == DASHBOARD)			
					{
						ModifiedFeature.RtFeature2.DashBoard_Flag_B32=value;
					}
					else if(FeatureChoice == HD_MODE)
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
					}
					else if(FeatureChoice == TEACH_MODE)
					{
						ModifiedFeature.RtFeature2.TeachMode_Flag_B40=value;
					}
					else if((FeatureChoice == ADV_COMM))
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
					}
					else if(FeatureChoice == EXTRADATASTORAGE)
					{
						ModifiedFeature.RtFeature2.ExtraDataStorage_Flag_B42=value;
					}
				}
				else if ((FeatureChoice == CONTROLSPEED) || (FeatureChoice == OPTITIUNE)|| (FeatureChoice == CFR))			
				{
					kprintf("FEATURE NOT SUPPORTED \n");		
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
				}
			}
			
			if(modelID==SATURNP2_ID)
			{
				if ((FeatureChoice == AUTOCONVERTERCOOLING)||(FeatureChoice == DASHBOARD)||(FeatureChoice == HD_MODE) ||(FeatureChoice == ADV_COMM))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					
					if(FeatureChoice == AUTOCONVERTERCOOLING)
					{
						ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
					}
					else if(FeatureChoice == DASHBOARD)
					{
						ModifiedFeature.RtFeature2.DashBoard_Flag_B32=value;
					}
					else if(FeatureChoice ==HD_MODE)
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
					}
					else if((FeatureChoice == ADV_COMM))
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
					}
				}	
				else if ((FeatureChoice == CONTROLSPEED) || (FeatureChoice == OPTITIUNE))
				{
					kprintf("FEATURE NOT SUPPORTED \n");		
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
				}
			}
			
			if(modelID==MARS_ID)
			{
				if ((FeatureChoice == COLLLAPSEDIST) || (FeatureChoice == ABSOLUTEDIST))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					if(FeatureChoice == COLLLAPSEDIST)
					{
						ModifiedFeature.RtFeature1.CollapseDist_Flag_B4=value;
					}
					else if(FeatureChoice ==ABSOLUTEDIST)
					{
						ModifiedFeature.RtFeature1.AbsoluteDist_Flag_B5=value;
					}					
				}
				else if ((FeatureChoice == TIMEMODE) ||  (FeatureChoice == ENERGY) ||  (FeatureChoice == GROUND_DETECT) ||
						(FeatureChoice == CONTINOUS) || (FeatureChoice == PRETRIGGER) ||  (FeatureChoice == AFTERBURST) ||
						(FeatureChoice == ENERGYBRAKE) || (FeatureChoice == ALARMLOG)||(FeatureChoice == EVENTLOG) ||  
						(FeatureChoice == WELDDATALOG)|| (FeatureChoice == CRACKEDHORN)||(FeatureChoice ==USER_IO)|| 
						(FeatureChoice == ACTUATOR_IO) || (FeatureChoice == CYCLECOUNTER) ||  (FeatureChoice == PASSWORD) ||(FeatureChoice == LIMITS))
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");	
				}
				else
				{
					kprintf("FEATURE NOT SUPPORTED \n");	
				}	
			}
			
			if(modelID==JUNO_ID)
			{
				if ((FeatureChoice == ADV_COMM)  ||  (FeatureChoice == HD_MODE))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					if(FeatureChoice == ADV_COMM)
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
					}
					else if(FeatureChoice == HD_MODE)	
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
					}
				}
				else if ((FeatureChoice == COLLLAPSEDIST) ||  (FeatureChoice == ABSOLUTEDIST)||  (FeatureChoice == AFTERBURST)||
						(FeatureChoice == ENERGYBRAKE)||(FeatureChoice == FORCESTEP)|| (FeatureChoice == ALARMLOG)||(FeatureChoice == WELDDATALOG)
						||(FeatureChoice ==BATCHCOUNTER)||(FeatureChoice == AUTOCONVERTERCOOLING)||  (FeatureChoice == TEACH_MODE)||
						(FeatureChoice == CFR)||(FeatureChoice == HOURCOUNTER))
				{
					kprintf("FEATURE NOT SUPPORTED \n");		
				}
				else
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");	
				}
			}	
			
			if(modelID==VENUS_ID)
			{
				if ((FeatureChoice == REPORTING)  ||  (FeatureChoice == OVERLAY) ||  (FeatureChoice == TRENDS) || 
						(FeatureChoice == HD_MODE)||  (FeatureChoice == ADV_COMM))
				{
					kprintf(" 1. ENABLE\n");
					kprintf(" 0. DISABLE\n");
					scanf("%d",&value);
					if(FeatureChoice == REPORTING)
					{
						ModifiedFeature.RtFeature1.Reporting_Flag_B16=value;
					}
					else if(FeatureChoice == OVERLAY)
					{
						ModifiedFeature.RtFeature2.Overlay_Flag_B35=value;
					}
					else if(FeatureChoice == TRENDS)						
					{
						ModifiedFeature.RtFeature2.Trends_Flag_B36=value;
					}
					else if(FeatureChoice == HD_MODE)
					{
						ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
					}
					else if(FeatureChoice == ADV_COMM)					
					{
						ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
					}
				}
				else if ((FeatureChoice == TIMEMODE) ||(FeatureChoice == ENERGY) ||(FeatureChoice == PEAKPOWER) ||(FeatureChoice == GROUND_DETECT)
						||(FeatureChoice == CONTINOUS)|| (FeatureChoice == EVENTLOG) ||  (FeatureChoice == CRACKEDHORN) ||
						(FeatureChoice == USER_IO)||(FeatureChoice == CYCLECOUNTER) ||(FeatureChoice == MOBILE))
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");			
				}
				else
				{
					kprintf("FEATURE NOT SUPPORTED \n");
				}	
			}
			
			if(modelID==VENUSHH_ID)
			{
				if ((FeatureChoice == TIMEMODE) || (FeatureChoice == ENERGY)||(FeatureChoice == PEAKPOWER)||(FeatureChoice == GROUND_DETECT)
				||(FeatureChoice == CONTINOUS)||(FeatureChoice == USER_IO) ||(FeatureChoice == CYCLECOUNTER) ||(FeatureChoice ==PASSWORD) ||
				(FeatureChoice == LIMITS))
				{
					kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");	
				}
				else
				{
					kprintf("FEATURE NOT SUPPORTED \n");	
				}	
			}
		}	
	}while(UserChoice);
	
	EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModifiedFeature);
	changeModelWrite();
	kprintf(" \n");
	
	return TRUE;
}

/******************************************************************************
*
* ChangeModel - Change model
*
* This routine  is for changing the required model
*
* RETURNS: OK on successful change 
*
* ERRNO: N/A
*/

STATUS ChangeModel(FeaturesFlag Flag)
{
	INT32	iChoice,ReadStatus;
	UINT8 modelID,FeatureArray[FEATUREFLAG_SIZE];
	FeaturesFlag CommonFeature;
	
	if((ReadStatus=EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&Flag))==ERROR)
	{		
		kprintf(" ChangeModel : Error in eeprom read\n");
		return ERROR;
	}
	
	do
	{
		kprintf(" 1. To Change Model \n");
		kprintf(" 0. EXIT \n ");	
		scanf(" %d",&iChoice);
		
		if(iChoice==1)
		{
			kprintf("	1.  NEPTUNE 			\n ");
			kprintf("	2.  P1_PNEUMATIC 	\n ");
			kprintf("	3.  P2_PNEUMATIC 	\n ");
			kprintf("	4.  I(IW) 			\n ");
			kprintf("	5.  MERCURY 		\n ");
			kprintf("	6.  SATURN P1		\n ");
			kprintf("	7.  SATURN P2		\n ");
			kprintf("	8.  MARS 			\n ");
			kprintf("	9.  JUNO 			\n ");
			kprintf("	10. VENUS 			\n ");
			kprintf("	11. VENUS HH 		\n ");
			kprintf(" enter choice\n");
			scanf(" %d",&iChoice);
			
			if(iChoice==NEPTUNE)
			{
				kprintf("			******* MODEL IS NEPTUNE ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==NEPTUNE_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureNeptune,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
				}
			}
			else if(iChoice==P1_PNEUMATIC)
			{
				kprintf("			******* MODEL IS P1 PNEUMATIC ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==P1_PNEUMATIC_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureP1Pneumatic,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
				}
			}
			else if(iChoice==P2_PNEUMATIC)
			{
				kprintf("			******* MODEL IS P2 PNEUMATIC ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==P2_PNEUMATIC_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureP2Pneumatic,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
				}
			}
			else if(iChoice==I_IW)
			{
				kprintf("			******* MODEL IS I(IW)****** \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==I_IW_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureIIW,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
				}
			}
			else if(iChoice==MERCURY)
			{
				kprintf("			******* MODEL IS MERCURY ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==MERCURY_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureMercury,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
				}
			}
			else if(iChoice==SATURNP1)
			{
				kprintf("			******* MODEL IS SATURN P1 ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==SATURNP1_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureSaturnP1,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
				}	
			}
			else if(iChoice==SATURNP2)
			{
				kprintf("			******* MODEL IS SATURN P2 ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==SATURNP2_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureSaturnP2,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);	
				}
					
			}
			else if(iChoice==MARS)
			{
				kprintf("			******* MODEL IS MARS ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==MARS_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureMars,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
			}
			else if(iChoice==JUNO)
			{
				kprintf("			******* MODEL IS JUNO ******* \n\n");
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==JUNO_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureJuno,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				} 
			}
			else if(iChoice==VENUS)
			{
				kprintf("			******* MODEL IS VENUS ******* \n\n");	
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==VENUS_ID)
					{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					}
				else
					{
					memcpy(&CommonFeature,featureVenus,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					}
			}
			else if(iChoice==VENUSHH)
			{
				kprintf("			******* MODEL IS VENUS HH******* \n\n");	
				modelID=CheckModel(EEPROM_MODEL_ADDRESS);
				
				if(modelID==VENUSHH_ID)
				{
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);		
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
				else
				{
					memcpy(&CommonFeature,featureVenusHH,FEATUREFLAG_SIZE);
					EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
					changeModelWrite();
					RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
					ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
				}
			} 
		}		
	}while(iChoice);
	
	return TRUE;	
}

/******************************************************************************
*
* RunTimeFeatureSc - Custom System Call for RunTime feature configuration
*
* This custom system call is for RunTime feature configuration.
*
* RETURNS: TRUE 
*
* ERRNO: N/A
*/

STATUS RunTimeFeatureSc(void)
{
	
	INT32	EepromStatus,changeStatus,ReadStatus,WriteStatus;
	char Flag[FEATUREFLAG_SIZE],RunTimeData=RUNTIMEDATA;
	FeaturesFlag FeatureBits;

	EepromStatus=CheckEepromModelEntry(EEPROM_RUNTIMEENTRY_ADDRESS,BYTESIZE); 
	
	if(EepromStatus==TRUE)  //default write when eeprom has no model
	{	
		memcpy(&FeatureBits,&featureNeptune,FEATUREFLAG_SIZE); 
		kprintf("					EEPROM IS EMPTY \n\n");
		kprintf("			******* DEFAULT MODEL WRITTEN IS NEPTUNE ******* \n\n");
		
		EepromWrite(EEPROM_RUNTIMEENTRY_ADDRESS,BYTESIZE,(char *)&RunTimeData);
		EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&FeatureBits);
	    changeModelWrite();
	   	RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
		
		if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&Flag))==ERROR)
		{
			kprintf(" RunTimeFeatureSc : ERROR in EEPROM Read \n");
		}
		
		memcpy(&FeatureBits,&Flag,FEATUREFLAG_SIZE);
		
		if((changeStatus=ChangeModel(FeatureBits))!=TRUE)
		{	
			kprintf(" RunTimeFeatureSc : Error in Changing Model\n");
		}
	}
	else 
	{
		DisplayModel(EEPROM_MODEL_ADDRESS);
		RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
		
		if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&Flag))==ERROR)
		{
			kprintf(" RunTimeFeatureSc : ERROR in EEPROM Read \n");
		}
		
		memcpy(&FeatureBits,&Flag,FEATUREFLAG_SIZE);
		
		if((changeStatus=ChangeModel(FeatureBits))!=TRUE)
		{	
			kprintf(" RunTimeFeatureSc : Error in Changing Model\n");
		}
	} 
	return TRUE; 	
}

/*-----------------------IpConfiguration----------------------*/

/*
*ipConfigSc - Custom System Call for IP configuration
*
* This custom system call is for Internet protocol configuration
* for the second Interface of SC.
*
* RETURNS: SUCCESS, or FAILURE if not configured
*
* ERRNO: N/A
*/
STATUS IPConfigurationSc(struct IPConfigurationScArgs *ipArgs)
{
	uint8_t status = SUCCESS;
	char gatewayCmd[100]={0x00};
	
	sprintf(gatewayCmd, "add -net -netmask %s %s %s",ipArgs->subnetAddr,ipArgs->ipAddr,ipArgs->gatewayAddr);

	if(routec(gatewayCmd)==ERROR)
	{
		kprintf("IPConfigurationSc : Error in setting Gateway\n");
		status=FAILURE;
	}
	
	return status;
}

/*-----------------------QSPI Custom system Calls For AM5728 MASTER----------------------*/

/******************************************************************************
*QSPIFlashEraseSc - Custom System Call for QSPI erase
*
* This custom system call is for block by block erase of QSPI
*
* RETURNS: N/A
*/
void QSPIFlashEraseSc(struct QSPIFlashEraseScArgs * pArgs)
{
   S25FL_Handle flashHandle;
   uint32_t blockCount;
   uint32_t startBlockNumber, endBlockNumber;
   flashHandle = vxbQspiLibSF25FLOpen();
   startBlockNumber = (pArgs->offset / QSPI_DEVICE_BLOCK_SIZE);
   endBlockNumber = ( (pArgs->offset + pArgs->length) / QSPI_DEVICE_BLOCK_SIZE);
   for (blockCount = startBlockNumber; blockCount <= endBlockNumber; blockCount++)
    {
    	vxbQspiLibS25FLFlashBlockErase(flashHandle, blockCount);
    }
    vxbQspiLibSF25FLClose(flashHandle);
   return;

}

/******************************************************************************
*QSPIFlashWriteSc - Custom System Call for QSPI Write.
*
* This custom system call is to write into the QSPI flash.
*
* RETURNS: Returns SUCCESS on Write success and FAIL on write fail.
*/
STATUS QSPIFlashWriteSc(struct QSPIFlashWriteScArgs * pArgs)
{
    int retval=0;
    S25FL_Transaction flashTransaction;
    S25FL_Handle flashHandle;
  
    /* Update transaction parameters */
    flashTransaction.data       = pArgs->src;
    flashTransaction.address    = pArgs->offset;
    flashTransaction.dataSize   = pArgs->length;  /* In bytes */
    
    flashHandle = vxbQspiLibSF25FLOpen();
    /* Write buffer to flash */
    retval = vxbQspiLibSF25FLBufferWrite(flashHandle, &flashTransaction);
    vxbQspiLibSF25FLClose(flashHandle);
	
	return retval;
}

/******************************************************************************
*QSPIFlashReadSc - Custom System Call for QSPI read.
*
* This custom system call is to read from the QSPI flash.
*
* RETURNS: Returns SUCCESS on read success and FAIL on read fail.
*/
STATUS QSPIFlashReadSc(struct QSPIFlashReadScArgs * pArgs)
{
	S25FL_Handle flashHandle;
    int retval=0;
    S25FL_Transaction flashTransaction;

    /* Update transaction parameters */
    flashTransaction.data       = pArgs->dest;
    flashTransaction.address    = pArgs->offset;
    flashTransaction.dataSize   = pArgs->length;  /* In bytes */
    
    flashHandle = vxbQspiLibSF25FLOpen();

    /* Write buffer to flash */
    retval = vxbQspiLibSF25FLBufferRead(flashHandle, &flashTransaction);
    vxbQspiLibSF25FLClose(flashHandle);

    return retval;
}

/*-----------------------QSPI Custom system Calls For AM437x Slaves----------------------*/

/******************************************************************************
* QspiLibInitSc - This function calls the QSPILibInit() to initialize the QSPI 
* 
* RETURNS: N/A
*/
void QspiLibInitSc()
{
	QSPILibInit();
}

/******************************************************************************
* QspiLibPrintIdSc - This function calls the QSPILibPrintId()  
* 
* RETURNS: N/A
*/
void QspiLibPrintIdSc()
{
	QSPILibPrintId();
}

/******************************************************************************
* QspiLibBlockEraseSc - This function calls the erases the particular block of
*                      	flash memory     
* 
* RETURNS: N/A
*/
void QspiLibBlockEraseSc(struct QspiLibBlockEraseScArgs *pArgs)
{
	QSPILibBlockErase(pArgs->block);
}

/******************************************************************************
* QspiLibWriteSc - This function calls the QSPI flash library to write 
*                  into the flash memory   
* 
* RETURNS: int
*/
int QspiLibWriteSc(struct QspiLibWriteScArgs *pArgs)
{
	printf("\nIn Custom system calls write \n pArgs->dstOffsetAddr = %x \n pArgs->srcAddr = %x \np Args->length = %x ",pArgs->dstOffsetAddr,pArgs->srcAddr,pArgs->length);
	
	return(QSPILibWrite(pArgs->dstOffsetAddr,pArgs->srcAddr,pArgs->length));
}

/******************************************************************************
* QspiLibReadSc - This function calls the QSPI flash library to read 
*                 from the flash memory    
* 
* RETURNS: int
*/
int QspiLibReadSc(struct QspiLibReadScArgs *pArgs)
{
	printf("\nIn Custom system calls read \n pArgs->srcOffsetAddr = %x \n pArgs->srcAddr = %x \np Args->length = %x ",pArgs->srcOffsetAddr,pArgs->dstAddr,pArgs->length);
	return(QSPILibRead(pArgs->srcOffsetAddr,pArgs->dstAddr,pArgs->length));
}

/******************************************************************************
* TimeStampSc -   This function calls the system time stamp and returns ticks.
* 
* RETURNS: float
*/
float TimeStampSc()
 {
	return (float)sysTimestamp()/sysTimestampFreq();
 }
void PC24VIsr(INT32 tid)
{
	if(eventSend(taskId, VXEV05) == ERROR)	
		kprintf("Error in sending an PC 24V Event\n");
}
INT32 GpioIntConnectSc(struct GpioIntConnectScArgs *pArgs)
{
	taskId = taskOpen((UINT8 *)"/PC24VEvent5", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);	
	return (int)vxbGpioIntConnect(pArgs->id, (VOIDFUNCPTR)pArgs->pIsr, pArgs->ptr);
}
INT32 GpioIntEnableSc(struct GpioIntEnableScArgs *id)
{
	return (INT32)vxbGpioIntEnable(id->id, (VOIDFUNCPTR)id->pIsr, (void *)id->pArg);
}
INT32 GpioIntConfigSc(struct GpioIntConfigScArgs *intConfig)
{
	return (INT32)vxbGpioIntConfig(intConfig->id, intConfig->trig, intConfig->pol);
}
/*
*Gpio24vLowShutdownIsr - Shutdown Interrupt Service Routine
* 
* This ISR triggers when power goes off.
**
* ERRNO: N/A
*/
void Gpio24vLowShutdownIsr(void)
{
	if(eventSend(ctrlTaskId, VXEV08) == ERROR)	
	{
		kprintf("Error in sending an GPIO24VLow event\n");
	}
}


/*
*Gpio24VLowShutdownSc - Custom System Call for Shutdown event
* 
* This Custom System call triggers event to RTP Application to save the critical data when power goes off.
**
** RETURNS: SUCCESS, or FAILURE if not configured
*
* ERRNO: N/A
*/
STATUS Gpio24VLowShutdownSc(void)
{
	int8_t status=SUCCESS;
	
	ctrlTaskId = taskOpen((char *)"/Ctrl_Task", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);
	
	/*Setting GPIO4_1 pin as input direction*/
	if (ERROR == vxbGpioSetDir (GPIO_24V_LOW_PIN, GPIO_DIR_INPUT))
	{
		(void)vxbGpioFree (GPIO_24V_LOW_PIN);
		kprintf("Gpio24VLowSc : Error in setting GPIO4_1 Input Direction\n");
		status=FAILURE;
	}
	
	/*Configuring GPIO4_1 pin as interrupt pin for poweroff*/
	if (ERROR == vxbGpioIntConfig (GPIO_24V_LOW_PIN, INTR_TRIGGER_EDGE, INTR_POLARITY_LOW))
	{
		(void)vxbGpioFree (GPIO_24V_LOW_PIN);
		kprintf("Gpio24VLowSc : Error in Configuring GPIO4_1 pin as Interrupt\n");
		status=FAILURE;
	}
	
	/*Registering ISR for GPIO4_1 pin*/
	if (ERROR == vxbGpioIntConnect (GPIO_24V_LOW_PIN, (VOIDFUNCPTR)Gpio24vLowShutdownIsr, (void *)NULL))
	{
		kprintf("Gpio24VLowSc : Error in Registering GPIO4_1 ISR\n");
		status=FAILURE;
	}
	
	/*Enabling GPIO4_1 interrupt pin*/
	if (ERROR == vxbGpioIntEnable (GPIO_24V_LOW_PIN, (VOIDFUNCPTR)Gpio24vLowShutdownIsr, (void *)NULL))
	{
		(void)vxbGpioIntDisconnect (GPIO_24V_LOW_PIN, (VOIDFUNCPTR)Gpio24vLowShutdownIsr, (void *)NULL);
		kprintf("Gpio24VLowSc : Error in Enabling GPIO4_1 Interrupt\n");
		status=FAILURE;
	}
	
	return status;
}

//For PC 15V Monitor
void PcGpio15vLowIsr(void)
{
	if(eventSend(ctrlTaskId, VXEV06) == ERROR)	
		kprintf("Error in sending an PC GPIO15VLow event\n");
}

STATUS PcGpio15VLowSc(void)
{
	int8_t status=SUCCESS;

	ctrlTaskId = taskOpen((char *)"/ControlTask", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);	
	
	/*Setting GPIO1_4 pin as input direction*/
	if (ERROR == vxbGpioSetDir (GPIO_PC_15V_LOW_PIN, GPIO_DIR_INPUT))
	{
		(void)vxbGpioFree (GPIO_PC_15V_LOW_PIN);
		kprintf("PcGpio15VLowSc : Error in setting GPIO1_4 Input Direction\n");
		status=FAILURE;
	}
	
	/*Configuring GPIO1_4 pin as interrupt pin for poweroff*/
	if (ERROR == vxbGpioIntConfig (GPIO_PC_15V_LOW_PIN, INTR_TRIGGER_EDGE, INTR_POLARITY_LOW))
	{
		(void)vxbGpioFree (GPIO_PC_15V_LOW_PIN);
		kprintf("PcGpio15VLowSc : Error in Configuring GPIO1_4 pin as Interrupt\n");
		status=FAILURE;
	}
	
	/*Registering ISR for GPIO1_4 pin*/
	if (ERROR == vxbGpioIntConnect (GPIO_PC_15V_LOW_PIN, (VOIDFUNCPTR)PcGpio15vLowIsr, (void *)NULL))
	{
		kprintf("PcGpio15VLowSc : Error in Registering GPIO4_1 ISR\n");
		status=FAILURE;
	}
	
	/*Enabling GPIO1_4 interrupt pin*/
	if (ERROR == vxbGpioIntEnable (GPIO_PC_15V_LOW_PIN, (VOIDFUNCPTR)PcGpio15vLowIsr, (void *)NULL))
	{
		(void)vxbGpioIntDisconnect (GPIO_PC_15V_LOW_PIN, (VOIDFUNCPTR)PcGpio15vLowIsr, (void *)NULL);
		kprintf("PcGpio15VLowSc : Error in Enabling GPIO1_4 Interrupt\n");
		status=FAILURE;
	}
	
	return status;
}

//For PC 5V Monitor
void PcGpio5vLowIsr(void)
{
	if(eventSend(ctrlTaskId, VXEV07) == ERROR)	
		kprintf("Error in sending an PC GPIO5VLow event\n");
}

STATUS PcGpio5VLowSc(void)
{
	int8_t status=SUCCESS;

	ctrlTaskId = taskOpen((char *)"/ControlTask", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);	
	
	/*Setting GPIO1_5 pin as input direction*/
	if (ERROR == vxbGpioSetDir (GPIO_PC_5V_LOW_PIN, GPIO_DIR_INPUT))
	{
		(void)vxbGpioFree (GPIO_PC_5V_LOW_PIN);
		kprintf("PcGpio5VLowSc : Error in setting GPIO1_5 Input Direction\n");
		status=FAILURE;
	}
	
	/*Configuring GPIO1_5 pin as interrupt pin for poweroff*/
	if (ERROR == vxbGpioIntConfig (GPIO_PC_5V_LOW_PIN, INTR_TRIGGER_EDGE, INTR_POLARITY_LOW))
	{
		(void)vxbGpioFree (GPIO_PC_5V_LOW_PIN);
		kprintf("PcGpio5VLowSc : Error in Configuring GPIO1_5 pin as Interrupt\n");
		status=FAILURE;
	}
	
	/*Registering ISR for GPIO1_5 pin*/
	if (ERROR == vxbGpioIntConnect (GPIO_PC_5V_LOW_PIN, (VOIDFUNCPTR)PcGpio5vLowIsr, (void *)NULL))
	{
		kprintf("PcGpio5VLowSc : Error in Registering GPIO5_1 ISR\n");
		status=FAILURE;
	}
	
	/*Enabling GPIO1_5 interrupt pin*/
	if (ERROR == vxbGpioIntEnable (GPIO_PC_5V_LOW_PIN, (VOIDFUNCPTR)PcGpio5vLowIsr, (void *)NULL))
	{
		(void)vxbGpioIntDisconnect (GPIO_PC_5V_LOW_PIN, (VOIDFUNCPTR)PcGpio5vLowIsr, (void *)NULL);
		kprintf("PcGpio5VLowSc : Error in Enabling GPIO1_5 Interrupt\n");
		status=FAILURE;
	}
	
	return status;
}


STATUS EmmcExtCsdDecodeSc(struct EmmcExtCsdDecodeScArgs *pArgs)
{
    STATUS status = ERROR;
    
	vxbDecodeExtCSD();

	pArgs->emmc_est_life->LifeTimeEstA = Dvr_EmmcEstLife.LifeTimeEstA;
	pArgs->emmc_est_life->LifeTimeEstB = Dvr_EmmcEstLife.LifeTimeEstB;
	pArgs->emmc_est_life->PreEOLInfo =   Dvr_EmmcEstLife.PreEOLInfo;
    
    status = SUCCESS;
    
	return status;
}

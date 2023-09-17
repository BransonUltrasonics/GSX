/*
 * customSystemCallHdlr.c
 *
 *  Created on: Oct 16, 2017
 *      Author: EE208214
 */

#ifndef __INCcustomSystemCallh
#define __INCcustomSystemCallh

/* Header files */
#include <vxWorks.h>
#include <subsys/timer/vxbTimerLib.h> 
#include <hwif/buslib/vxbSpiLib.h>	
#include <sioLib.h>
#include <subsys/int/vxbIntLib.h>
typedef void (*EHRPWMDRV_IRQ_HANDLER)(void);


/** Memory size 1 byte */
#define MEM_SIZE_1B               			 (1U)
/** Memory size 1 kilo-byte */
#define MEM_SIZE_KB                			(1024U * MEM_SIZE_1B)
/** Memory size 1 gega-byte */
#define MEM_SIZE_MB                			(1024U * MEM_SIZE_KB)

/** QSPI device specific definitions */
#define QSPI_INSTANCE                       (1U)
#define QSPI_OFFSET                         (4U)
#define QSPI_DEVICE_SIZE                    (32 * MEM_SIZE_MB)
#define QSPI_DEVICE_BLOCK_SIZE              (64 * MEM_SIZE_KB)

#define LOGGER_T_STACK_SIZE					(1024 * 8)
#define MAX_SIZE_OF_MSG_LENGTH 				512 //maximum message length
#define LOGDATA_SIZE 						512
#define STATUS_SIZE 						5

#define MAX_MSG								1000 
#define MAX_MSG_LEN							1024 
#define MSG_Q_FIFO							0x00
#define MSG_Q_PRIORITY						0x01

#define EEPROM_MODEL_ADDRESS				0x110
#define EEPROM_RUNTIMEENTRY_ADDRESS			0x100
#define RUNTIMEDATA							0X52

#define RTF_NA								0
#define RTF_BASE							1
#define RTF_LTD								2
#define	RTF_OPT								3

#define ENABLE 								1
#define DISABLE								0


#define MODELID_MASK						0xF0
#define FLAGMASK							3
#define MODELBIT_MASK						1
#define BYTESIZE							1
#define NUMBITS_PERFLAG						2
#define FEATURECATEGORY_SIZE				15				


//Model IDs defined
#define NEPTUNE_ID                           0x10
#define P1_PNEUMATIC_ID                      0X20                                                                                                     
#define P2_PNEUMATIC_ID                      0X30                                                                                                    
#define I_IW_ID                              0X40                                                                                                                     
#define MERCURY_ID                           0x50                                                            
#define SATURNP1_ID                          0x60                                                            
#define SATURNP2_ID                          0x70                                                            
#define MARS_ID                              0x80                                                                  
#define JUNO_ID                              0X90                                                                                                                  
#define VENUS_ID                             0xA0                                                                 
#define VENUSHH_ID                           0xB0    

//model choice
#define NEPTUNE                             1
#define P1_PNEUMATIC                        2                                                                                                     
#define P2_PNEUMATIC                        3                                                                                                   
#define I_IW                               	4                                                                                                                    
#define MERCURY                             5                                                            
#define SATURNP1                            6                                                            
#define SATURNP2                            7                                                            
#define MARS                                8                                                                 
#define JUNO                                9                                                                                                                 
#define VENUS                               10                                                                
#define VENUSHH                             11    

//Model default feature settings in bits
#define FEATURE_NEPTUNE                     {0X55,0X15,0X55,0X55,0X55,0X55,0X55,0XD5,0X55,0XD5,0X5D,0X15}                                                                                             
#define FEATURE_P1_PNEUMATIC                {0X55,0X15,0X55,0XA8,0X57,0X55,0X05,0XF4,0XD5,0XD3,0X7F,0X24} 
#define FEATURE_P2_PNEUMATIC                {0X55,0X15,0X55,0X55,0X55,0X55,0X55,0XF5,0X55,0XD5,0X5D,0X35} 
#define FEATURE_I_IW                       	{0X34,0X1F,0X05,0XA8,0X04,0X50,0X01,0X00,0X08,0X20,0X00,0X40}                                                                                                 
#define FEATURE_MERCURY                     {0X55,0X55,0X55,0X55,0X55,0X55,0X55,0X54,0X57,0XD1,0X5D,0X55}                                                                                         
#define FEATURE_SATURNP1                    {0X55,0X55,0X55,0XA9,0X57,0X55,0X55,0X74,0X57,0XD1,0X7F,0X64}                                                                        
#define FEATURE_SATURNP2                    {0X55,0X55,0X55,0X55,0X55,0X55,0X55,0X74,0X57,0XD1,0X5D,0X75}                                                                       
#define FEATURE_MARS                        {0X14,0X5F,0X15,0XA8,0X04,0X50,0X01,0X00,0X08,0X20,0X00,0X80}                                                                                 
#define FEATURE_JUNO                        {0X55,0X50,0X40,0X10,0X55,0X55,0X51,0X45,0X56,0XE5,0X5C,0X90}                                                                                              
#define FEATURE_VENUS                       {0X54,0X50,0X00,0X10,0X07,0X10,0X01,0X00,0XDA,0XE3,0X0C,0XA0}                                                                                             
#define FEATURE_VENUSHH                     {0X54,0X50,0X00,0X00,0X00,0X10,0X01,0X00,0X08,0X20,0X00,0XB0} 

typedef char ModelID;


#define	MULTIMODE							0										
#define	TIMEMODE							1                       						
#define ENERGY								2                    						
#define PEAKPOWER							3                  						
#define COLLLAPSEDIST						4               						
#define ABSOLUTEDIST						5               						
#define	GROUND_DETECT						6                   					
#define CONTINOUS							7              							
#define PRETRIGGER							8                   					
#define AFTERBURST							9                   					
#define ENERGYBRAKE							10                   					
#define AMPLITUDESTEP						11                   				
#define FORCESTEP							12                   					
#define ALARMLOG							13                   					
#define EVENTLOG							14                   					
#define WELDDATALOG							15                   					
#define REPORTING							16                   					
#define CRACKEDHORN							17                   					
#define USB									18                   							
#define BINARY								19                   						
#define WEBSERVICES							20                   					
#define OPC_UA								21                   						
#define USER_IO								22                   						
#define ACTUATOR_IO							23                   					
#define CYCLECOUNTER						24                   				
#define BATCHCOUNTER						25                   				
#define POWERMATCHCURVE						26                   				
#define SMART_CONVERTER						27                   				
#define CONTROLSPEED						28                   				
#define SEQUENCING							29                   					
#define AUTOCONVERTERCOOLING				30                   		
#define FIELDBUS							31
#define DASHBOARD							32                   				
#define PASSWORD							33                   					
#define MOBILE								34                   					
#define OVERLAY								35                   					
#define TRENDS								36                   					
#define OPTITIUNE							37                   				
#define LIMITS								38                   					
#define	HD_MODE								39                   					
#define	TEACH_MODE							40                   				
#define	ADV_COMM							41                   					
#define EXTRADATASTORAGE					42                   		
#define TROUBLESHOOT						43                   	
#define	CFR									44                     					
#define	HOURCOUNTER							45                				

typedef struct
 {
	
	UINT8	Multimode_Flag_B0										:2;
	UINT8	Time_Flag_B1                       						:2;
	UINT8  	Energy_Flag_B2                    						:2;
	UINT8  	PeakPower_Flag_B3                  						:2;
	UINT8  	CollapseDist_Flag_B4               						:2;
	UINT8  	AbsoluteDist_Flag_B5               						:2;
	UINT8 	GroundDetect_Flag_B6                   					:2;
	UINT8  	Continous_Flag_B7              							:2;
	UINT8  	PreTrigger_Flag_B8                   					:2;
	UINT8  	AfterBurst_Flag_B9                   					:2;
	UINT8  	EnergyBrake_Flag_B10                   					:2;
	UINT8  	AmplitudeStep_Flag_B11                   				:2;
	UINT8  	ForceStep_Flag_B12                   					:2;
	UINT8 	AlarmLog_Flag_B13                   					:2;
	UINT8  	EventLog_Flag_B14                   					:2;
	UINT8  	WeldDataLog_Flag_B15                   					:2;
	UINT8  	Reporting_Flag_B16                   					:2;
	UINT8  	CrackedHorn_Flag_B17                   					:2;
	UINT8  	USB_Flag_B18                   							:2;
	UINT8 	Binary_Flag_B19                   						:2;
	UINT8  	WebServices_Flag_B20                   					:2;
	UINT8  	OPC_UA_FLAG_B21                   						:2;
	UINT8  	UserIO_Flag_B22                   						:2;
	UINT8  	ActuatorIO_Flag_B23                   					:2;
	UINT8  	CycleCounter_Flag_B24                   				:2;
	UINT8  	BatchCounter_Flag_B25                   				:2;
	UINT8  	PowerMatchCurve_Flag_B26                   				:2;
	UINT8  	SmartConverter_Flag_B27                   				:2;
	UINT8  	ControlSpeed_Flag_B28                   				:2;
	UINT8  	Sequencing_Flag_B29                   					:2;
	UINT8  	AutoConverterCooling_Flag_B30                   		:2;
	UINT8  	FieldBus_Flag_B31                   					:2;

}RTF_LIST1;


typedef struct 
{
	
		UINT8  DashBoard_Flag_B32                   				:2;
		UINT8  Password_Flag_B33                   					:2;
		UINT8  Mobile_Flag_B34                   					:2;
		UINT8  Overlay_Flag_B35                   					:2;
		UINT8  Trends_Flag_B36                   					:2;
		UINT8  Optitune_Flag_B37                   					:2;
		UINT8  Limits_Flag_B38                   					:2;
		UINT8  HDMode_Flag_B39                   					:2;
		UINT8  TeachMode_Flag_B40                   				:2;
		UINT8  AdvComm_Flag_B41                   					:2;
		UINT8  ExtraDataStorage_Flag_B42                   			:2;
		UINT8  TroubleShoot_Suggestion_Flag_B43                   	:2;
		UINT8  CFR_Flag_B44                     					:2;
		UINT8  HourCounter_Flag_B45                					:2;
		UINT8  MODELID_Flag_B1	                   					:1;
		UINT8  MODELID_Flag_B2    	               					:1;
		UINT8  MODELID_Flag_B3        	           					:1;
		UINT8  MODELID_Flag_B4            	       					:1;
}RTF_LIST2;


typedef struct
{
	RTF_LIST1 RtFeature1;
	RTF_LIST2 RtFeature2;
	
}FeaturesFlag;

#define FEATUREFLAG_SIZE sizeof(FeaturesFlag)
#define RTF_LIST1_SIZE sizeof(RTF_LIST1)
#define RTF_LIST2_SIZE sizeof(RTF_LIST2)


#define NUMFLAG_INBYTES FEATUREFLAG_SIZE/3

/* Global Variables */    
struct timer_isr
{
	int task_id;
};

// Structure used to Set the time
typedef struct
{
	int tm_sec;         /* seconds after the minute     - [0, 59] */
	int tm_min;         /* minutes after the hour       - [0, 59] */
	int tm_hour;        /* hours after midnight         - [0, 23] */
	int tm_mday;        /* day of the month             - [1, 31] */
	int tm_mon;         /* months since January         - [0, 11] */
	int tm_year;        /* years since 1900     */
	int tm_wday;        /* days since Sunday            - [0, 6] */
	int tm_yday;        /* days since January 1         - [0, 365] */
	int tm_isdst;       /* Daylight Saving Time flag */	//int tm_lowPwr;      /* RTC low Power flag, 1: power is 0.9v - 1.0v  */
}RTCtimerStr;

typedef struct 
{
	uint8_t	LogLevel;
	uint8_t FileFlag;
	char	Buffer[MAX_SIZE_OF_MSG_LENGTH];	
}LogMessage;

typedef enum {
LOG_CRITICAL=1,LOG_ERROR,LOG_WARNING,LOG_DEBUG
}LOG_LEVEL_LIST;

typedef struct {
LOG_LEVEL_LIST LogLevel;
char STATUS[ STATUS_SIZE];
char LogingDataBuf[LOGDATA_SIZE];
}LogData;

typedef struct 
	{
	 UINT32 LifeTimeEstA;
     UINT32 LifeTimeEstB;
     UINT32 PreEOLInfo;
   } EST_LIFE;


#endif/* __INCcustomSystemCallh */

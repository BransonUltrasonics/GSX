/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef ETHERNET_IP_H_
#define ETHERNET_IP_H_
#include  "IOFunctions.h"
#include "FieldBus.h"
#include "IOImageHandler.h"
#include "MailBoxHandler.h"
#include "cifXUser.h"
#include "eip_eif_packetdefinitions_packets.h"
#include "eip_eif_packetdefinitions_commands.h"
#include "eip_tcp.h"
#include "CommunicateInterface.h"

#define MAILBOX_PACKET_TIMEOUT 		1000//non real-time packet transfer/acknowledgment timeout with Hilscher slave card.

#define FB_INPUT_BITS 64
#define FB_OUTPUT_BITS 64

#define FB_INPUT_BYTES FB_INPUT_BITS/16
#define FB_OUTPUT_BYTES FB_INPUT_BITS/16

// Vendor specific ID's: 0x64 - 0xC7  & 0x300 - 0x4FF
// 0x64 - 0x7F Reserved for DCX
#define ETHIP_VENDORSPECIFIC_DCX_START	0x64
#define ETHIP_VENDORSPECIFIC_DCX_END	0x7F
// 0x80 - 0x9F Reserved for GSX
#define ETHIP_VENDORSPECIFIC_GSX_START	0x80 
#define ETHIP_VENDORSPECIFIC_GSX_END	0x9F

#define ETHIP_VENDORSPECIFIC_GSX_MAXINSTANCE 32
#define MAX_SERVICEDATA_LEN 100

#define ETHIP_VENDORSPECIFIC_GSX_TIMEOUT	1000
#define ETHIP_VENDORSPECIFIC_GSX_RETRY_TIME	10

#define ETHIP_PROTOCOL_STACK_DESTINATION 0x00000020 
#define STATUS_MESSAGES_LENGTH 100

//<! EthernetIP standard class and instance for TCP/IP configuration.
#define TCPIP_INTERFACEOBJECT_CLASS                        0xF5
#define TCPIP_INTERFACEOBJECT_INSTANCE                     0x01

//<! Attribute to configure the IP, SubnetMask and Gateway.
#define TCPIP_INTERFACEOBJECT_INTERFACECONFIG_ATTRIBUTE    0x05

//<! Added for the QoS attribute to check.
#define EIP_QOS_MIN_INSTANCE_ATTR 1

#define IP_ADDRESS(a,b,c,d) (uint32_t)(((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

#define LE16_CONVERT(a)   ( (((a) & 0x00FF) << 8) | \
                                (((a) & 0xFF00) >> 8) )

#define WELD_RECIPE_BIT_NUM_OFFSET 5
extern UINT32 ServiceDataSizeIn, ServiceDataSizeOut;

//Fieldbus IO

typedef	enum
{
	WELD_RECIPE_OBJECT = ETHIP_VENDORSPECIFIC_GSX_START,
	WELD_RESULTS_OBJECT,
	SUSPECT_REJECT_OBJECT,
	STACK_RECIPE_OBJECT,
	SEEK_SCAN_TEST_RECIPE_OBJECT,
	SEEK_SCAN_TEST_RESULTS_OBJECT,
	ALARM_OBJECT,
	SYSTEM_INFO_OBJECT,
	LAST_WELD_RESULTS_OBJECT,
	VERSION_RTC_COUNTERS_OBJECT,
	ETHIP_VENDOR_OBJECT_LAST_USED, // Always left these two ID's at the end of the enum
	LAST_VENDOR_OBJECT= ETHIP_VENDORSPECIFIC_GSX_END
} FB_VENDOR_SPECIFIC_OBJECTS;
#define ETHIP_VENDORSPECIFIC_GSX_SIZE	(ETHIP_VENDOR_OBJECT_LAST_USED - ETHIP_VENDORSPECIFIC_GSX_START)

typedef	enum
{
	LINE_LOW_EIP,
	LINE_HIGH_EIP,
	
	LINE_NONE_EIP

} FB_LINE_VALUE;

typedef	enum
{
	STATE_ACTIVE_LOW_EIP,
	STATE_ACTIVE_HIGH_EIP,

	STATE_NONE
} FB_LINE_STATE;

typedef struct{

	FB_LINE_VALUE value;
	FB_LINE_STATE logic;

}DigIOProperty;

typedef enum{
	US_DISABLE_EIP, //GSX Only
	E_STOP_EIP,
	I_HOME_POSITION_EIP, //GSX Only
	I_READY_POSITION_EIP, //GSX Only
	I_STACK_PRESET_0_EIP,
	I_STACK_PRESET_1_EIP,
	I_STACK_PRESET_2_EIP,
	I_STACK_PRESET_3_EIP,
	
	I_WELD_PRESET_0_EIP,
	I_WELD_PRESET_1_EIP,
	I_WELD_PRESET_2_EIP,
	I_WELD_PRESET_3_EIP,
	I_WELD_PRESET_4_EIP,
	HORN_DELAY_EIP, //GSX Only
	I_MANUAL_AUTO_EIP,
	PART_PRESENT_EIP, //GSX Only
	
	WELD_FUNCTION_EIP,
	STACK_FUNCTION_EIP,
	STACK_FUNCTION_0_EIP,
	STACK_FUNCTION_1_EIP,
	STACK_FUNCTION_2_EIP,
	I_RESERVED_0_EIP,
	MEMORY_CLEAR_EIP,
	I_RESERVED_1_EIP,
	
	RESET_EIP,
	RUN_SONICS_EIP,
	I_RESERVED_2_EIP,
	I_RESERVED_3_EIP,
	GORUND_DETECT_EIP,
	AMPLITUDE_PROFILE_EIP,
	I_RESERVED_4_EIP,
	I_RESERVED_5_EIP,
	
	ACTUATOR_FUNCTION_EIP, //GSX Only
	ACTUATOR_FUNCTION_0_EIP, //GSX Only
	ACTUATOR_FUNCTION_1_EIP, //GSX Only
	ACTUATOR_FUNCTION_2_EIP, //GSX Only
	I_RESERVED_6_EIP,
	I_WELD_PRESET_5_EIP, //GSX Only
	I_WELD_PRESET_6_EIP, //GSX Only
	I_WELD_PRESET_7_EIP,//GSX Only
	
	I_WELD_PRESET_8_EIP, //GSX Only
	I_WELD_PRESET_9_EIP, //GSX Only
	I_RESERVED_7_EIP,
	I_RESERVED_8_EIP,
	I_RESERVED_9_EIP,
	I_RESERVED_10_EIP,
	I_RESERVED_11_EIP,
	I_RESERVED_12_EIP,
	
	INPUTS_END_EIP
	
}EIP_INPUTS;

typedef enum{
	NON_CYCLE_OVL_GROUP_B_EIP,
	E_STOP_ACTIVE_EIP,
	TEMP_ERROR_EIP, //Professional compact only
	RF_SWITCH_ERROR_EIP, //Professional compact only
	O_STACK_PRESET_0_EIP,
	O_STACK_PRESET_1_EIP,
	O_STACK_PRESET_2_EIP,
	O_STACK_PRESET_3_EIP,
	
	O_WELD_PRESET_0_EIP,
	O_WELD_PRESET_1_EIP,
	O_WELD_PRESET_2_EIP,
	O_WELD_PRESET_3_EIP,
	O_WELD_PRESET_4_EIP,
	PRESET_CHANGE_COMPLETE_EIP,
	O_MANUAL_AUTO_EIP,
	OVL_GROUP_0_EIP,
	
	SETUP_ALARM_GROUP_2_EIP,
	CYCLE_MODIFIED_ALARM_GROUP_3_EIP,
	WARNING_ALARM_GROUP_4_EIP,
	EQUIPMENT_FAILURE_ALARM_GROUP_6_EIP,
	NO_CYCLE_ALARM_GROUP_7_EIP,
	COMM_ALARM_GROUP_8_EIP,
	HARDWARE_ALARM_GROUP_A_EIP,
	CUTOFF_ALARM_GROUP_1_EIP,
	
	TEMPERATURE_ALARM_EIP, //Professional compact only
	STOP_MODE_EIP, //Professional compact only
	SONICS_OFF_EIP, //READY?
	SONICS_ACTIVE_EIP,
	CYCLE_OK_EIP,
	LIMIT_ALARM_GROUP_5_EIP,
	O_MEMORY_CLEAR_EIP,
	WAIT_FOR_PART_ID_EIP,
	
	O_WELD_PRESET_5_EIP, //GSX Only
	O_WELD_PRESET_6_EIP, //GSX Only
	O_WELD_PRESET_7_EIP,//GSX Only
	O_WELD_PRESET_8_EIP, //GSX Only
	O_WELD_PRESET_9_EIP, //GSX Only
	CYCLE_RUNNING_EIP,
	HORN_SEEK_EIP,
	WELD_ACTIVE_EIP,
	
	HOLD_ACTIVE_EIP,
	O_HOME_POSITION_EIP,
	O_READY_POSITION_EIP,
	PB_RELEASE_EIP,
	O_RESERVED_0_EIP,
	O_RESERVED_1_EIP,
	O_RESERVED_2_EIP,
	O_RESERVED_3_EIP,
	
	OUTPUTS_END_EIP
	
}EIP_OUTPUTS;
typedef enum{
		NO_B,	//NOT USED
		ES,		//ESTOP
		TEE,	//NOT USED
		HFSE,	//NOT USED
		HFS0,	//NOT USED
		HFS1,	//NOT USED
		HFS2,	//NOT USED
		HFS3,	//NOT USED
		RCPB0,	//NOT USED
		RCPB1,	//NOT USED
		RCPB2,	//NOT USED
		RCPB3,	//NOT USED
		RCPB4,	//NOT USED
		PSCA ,	//NOT USED
		MA,		//NOT USED
		OL_0 ,	// msb
} StatusWord1;
typedef enum 
{
		SE_2,		//NOT USED
		CM_3 ,		//NOT USED
		WA_4,		//NOT USED
		EQ_6 ,		//NOT USED
		NC_7,		//NOT USED
		CF_8 ,		//NOT USED
		HW_A,		//NOT USED
		CU_1,		//NOT USED
		TP_9,		//NOT USED
		SM,			//NOT USED
		READY_ST,	//OUTPUT_SYSTEM_READY
		SONIC_ON ,	//OUTPUT_ULTRASONIC
		WELD_OK ,	//OUTPUT_CYCLE_OK
		LM_5,		//NOT USED
		MCLR,		//NOT USED
		RES,		//NOT USED
} StatusWord2;

typedef enum
{
		CYRU ,	//OUTPUT_CYCLE_RUN
		H_SEEK,	//OUTPUT_HORN_SEEK
		WACT,	//OUTPUT_WELD_ACTIVE
		HACT,	//OUTPUT_HOLD_ACTIVE
		HPOS,	//OUTPUT_AC_HOME
		RPOS,	//OUTPUT_AC_READY
		PBR,	//OUTPUT_PB_RELEASE	
		
		/*PENDING*/
		GR_AL,	//general alarm
		STP_MD,	//setup mode
		VAL_R,	//validated recipe
		S_AL,	//Suspect alarm
		R_AL,	//Reject alarm
		ABRST,	//Afterburst active
		RES7,	//NOT USED
		RES8,	//NOT USED
		RES9,	//NOT USED

} StatusWord3;

typedef enum
{
	CW1_USDIS,	//U/S Disable
	CW1_ES,		//DCX use only
	CW1_HPOS,	//Home Position	
	CW1_RPOS,	//Ready Position 
	CW1_HFS0,	//DCX use only
	CW1_HFS1,	//DCX use only
	CW1_HFS2,	//DCX use only
	CW1_HFS3,	//DCX use only
	CW1_RCPB0,	//Weld Recipe Bit 0
	CW1_RCPB1,	//Weld Recipe Bit 1
	CW1_RCPB2,	//Weld Recipe Bit 2
	CW1_RCPB3,	//Weld Recipe Bit 3
	CW1_RCPB4,	//Weld Recipe Bit 4
	CW1_HDEL,	//Hold Delay
	CW1_MA,		//Manual/Auto
	CW1_RES0,	//Reserved

}ControlWord1;		
 
typedef enum
{
	CW2_FCT,	//Weld Function
	CW2_SFCT,	//Stack Function
	CW2_SFCT0,	//Stack Function 0
	CW2_SFCT1,	//Stack Function 1
	CW2_SFCT2,	//Stack Function 2
	CW2_RES0,	//Reserved
	CW2_MCLR,	//Memory Clear
	CW2_RES1,	//Reserved
	CW2_RST,	//Reset
	CW2_ON,		//Run Ultrasonics
	CW2_CYAB,	//Cycle Abort
	CW2_RES2,	//Reserved
	CW2_GNDDT,	//Ground Detect
	CW2_APROF,	//Amplitude Profile
	CW2_RES3,	//Reserved
	CW2_RES4,	//Reserved
}ControlWord2;	

typedef enum
{
	CW3_ACFT,	//Actuator Function
	CW3_ACFT0,	//Actuator Function 0
	CW3_ACFT1,	//Actuator Function 1
	CW3_ACFT2,	//Actuator Function 2
	CW3_RES0,	//Reserved
	CW3_GSXR0,	//Weld Preset Number 0
	CW3_GSXR1,	//Weld Preset Number 1
	CW3_GSXR2,	//Weld Preset Number 2
	CW3_GSXR3,	//Weld Preset Number 3
	CW3_GSXR4,	//Weld Preset Number 4
	CW3_GSXR5,	//Weld Preset Number 5
	CW3_GSXR6,	//Weld Preset Number 6
	CW3_GSXR7,	//Weld Preset Number 7
	CW3_GSXR8,	//Weld Preset Number 8
	CW3_GSXR9,	//Weld Preset Number 9
	CW3_RES1,	//Reserved
}ControlWord3;	

typedef struct ImplicitOutputs
{
	UINT16	
		Reserved1,
		Reserved2;
	UINT16
		StatusWord1,
		StatusWord2,
		StatusWord3;
	UINT16	
		Amplitude,
		Current,
		Power;
	INT16
		Phase;
	UINT16
		PWM,
		Frequency,
		Force,
		Collapse,
		Absolute,
		Velocity,
		PresetNumber;
	
} ImplicitOutputs;

typedef struct ImplicitInputs
{
	UINT16
		ControlWord1,
		ControlWord2,
		ControlWord3;
} ImplicitInputs;

enum StatusWordNum
{
	SW1=1,
	SW2,
	SW3
};

enum ServiceCode
{
	GET_ATTRIBUTE_SINGLE = 0x0E,
	SET_ATTRIBUTE_SINGLE = 0x10,
};

//EthernetIP class (singleton class).
class EthernetIP: public FieldBus, public IOImageHandler, public MailBoxHandler
{
public:	
	
	static EthernetIP* Getinstance();
	bool GetWarmStartParams();
	void ReadExtendedStatus();
	INT32 ProtocolSpecificInit(CIFXHANDLE HChannel);
	void GetFaultData();
	INT32 UpdateInputCyclic(UINT8* InputPtr);
	INT32 UpdateOutputCyclic(UINT8* OutputPtr);
	virtual bool ProcessMailBox(CIFX_PACKET *MBXPkt);
	void RegisterForMSNSChange();
	void RegisterAssembly();
	void RegisterService();
	INT32 GSXE2RegisterVendorSpecificObjects(CIFXHANDLE HChannel);
	INT32 RegisterVendorSpecificClass(CIFXHANDLE HChannel, UINT32 VendorObjClass);
	void HandleResetServiceIndication();
	void CheckConnIndicationDetails();
	void GetIOSignalState();
	void SetIOSignalState();
	void SendACDInformation();
	void SetStaticIPAddr();
	INT32 SetAttribute(UINT32 ulClass, UINT32 ulInstance, UINT32 ulAttribute, EIP_TCP_INTERFACE_CONFIG_Ttag &value, CIFXHANDLE HChannel);
	void GetAttribute(UINT32 ulClass, UINT32 ulInstance, UINT32 ulAttribute);
	UINT32 GetConnectionState();
	
	void SetOutputToIOImage (DigitalIO::LINE_FUNCTION function, DigitalIO::LINE_VALUE value);
	void SetOutputToSWIOImage (INT32 StatusWordNum, INT32 function, INT32 value);
private:
	EthernetIP();
	~EthernetIP();	
	static EthernetIP *m_objEthIP;
	ImplicitOutputs ImplicitOutputImage;
	ImplicitInputs  ImplicitInputImage;
	DigIOProperty FBInputs[FB_INPUT_BITS];
	DigIOProperty FBOutputs[FB_OUTPUT_BITS];
	
	UINT32 ConnectionState;
	UINT8  ConnectionType;
	
	char AcdInfoBuffer[CIFX_MAX_PACKET_SIZE];
	
	CommonProperty 	*CP;
	CommunicateInterface *Interface;
protected:
	EIP_OBJECT_PACKET_CL3_SERVICE_IND_T *Cl3Pkt;
	EIP_OBJECT_PACKET_CL3_SERVICE_IND_T  Cl3IndPkt;
	EIP_OBJECT_PACKET_CL3_SERVICE_RES_T *Cl3Res;
	EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_IND_T* PktInd;
	EIP_OBJECT_PACKET_CIP_OBJECT_CHANGE_RES_T* PktRes;

	EIP_TCP_INTERFACE_CONFIG_T Config;
	EIP_TCP_ACD_LAST_CONFLICT_T *AcdInfo;
	EIP_TCP_INST_ATTR_STORED_T *EthipData;
};

#endif /* ETHERNET_IP_H_ */

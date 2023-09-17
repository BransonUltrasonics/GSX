/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef FIELDBUS_H_
#define FIELDBUS_H_

#include "cifXHWFunctions.h"
#include "CommonProperty.h"

#define HOSTSTATE_TIMEOUT	100

//FieldBus Abstract class.
class FieldBus
{
public:
	FieldBus();
	virtual ~FieldBus();
	
	static bool GetFieldBusInitialised();
	static void SetFieldBusInitialised();
	void SendWarmStart();
	UINT32 GetHilModuleInfo(); 
	INT32 HilscherInit();
	void configuration();
	//<!added for test purpose to set HOST and BUS flags.
	void InitIOCommunication();

	INT32 BasicConfigurationPacketSet();
	void RegisterApp();
	void ReadCommonStatus();
	void cifXErrorDescription(INT32 lRetErr);
	virtual void ProtocolSpecificInit();
	virtual void GetFaultData();
	virtual bool GetWarmStartParams() = 0;
	virtual void ReadExtendedStatus() = 0;
	
	SYSTEM_CHANNEL_SYSTEM_INFO_BLOCK tSysInfo;
	CHANNEL_INFORMATION tChannelInfo;
	
	//Toolkit driver handle
	static CIFXHANDLE HDriver;
		
	//Hilscher's DPM Communication channel handle
	static CIFXHANDLE HChannel;
	
	
private:
	static bool m_bisInitialised;
	
};

#endif /* FIELDBUS_H_ */

/*
 * RunTimeFeature.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: ramuks
 */




#include "IPConfig.h"
#include "Eeprom.h"
#include "AlarmFunctions.h"
#include "DataBaseManager.h"
#include  <iostream>
#include  <fstream>


using namespace std;

/**************************************************************************//**
* \brief   -   IPConfiguration constructor
*
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
IPConfig::IPConfig()
{
		
}

/**************************************************************************//**
* \brief   -   IPConfiguration destructor
*
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
IPConfig::~IPConfig()
{
		
}

/**************************************************************************//**
* \brief   -  Reading EEPROM for the IP configuration
*
* \param   -  None
*
* \return  -  STATUS - SUCCESS or FAILURE
*
******************************************************************************/
STATUS  IPConfig::GetIPParameters(char *pIPConfigObj)
{
	UINT8 ret = SUCCESS;
	
	if(EEPROM_IPCONF.Read(pIPConfigObj, IPCONF_PARAM_LENGTH, EEPROM_IPCONF_OFFSET) == ERROR)//Reading the IP Configuration parameters
	{
		cout<<"IPConfig : ERROR IN GetIPParameters"<<endl;
		ret = FAILURE;
	}
	
	for(UINT8 idx = 0; idx < IPCONF_PARAM_LENGTH; idx++)
	{
		printf("UI_T : Byte%d : data %u\n",idx,pIPConfigObj[idx]);
	}
	
	return ret;
}

/**************************************************************************//**
* \brief   - Setting the IP address, gateway and SubNetMask addresses
* 			in EEPROM. This info is then taking effect in boot time
*
* \param   - char *pIPConfigObj
*
* \return  -  STATUS
*
******************************************************************************/
STATUS IPConfig::SetIPParameters(char *pIPConfigObj)
{
	UINT8 status = FAILURE;
	UINT8 IPByteIndex, IPConfLen;
	INT32 SubNetMask=0;
	char IPAddr[IP_STRING_LENGTH]={0x00}, GatewayAddr[IP_STRING_LENGTH]={0x00}, SubnetAddr[IP_STRING_LENGTH]={0x00};

	char oldIPAddr[IP_STRING_LENGTH]={0x00}, oldGatewayAddr[IP_STRING_LENGTH]={0x00}, oldSubnetAddr[IP_STRING_LENGTH]={0x00};
	char oldDetails[16]={0};
	string details;
	char event[100];
	
	// read old IP configuration first
	GetIPParameters(oldDetails);

	sprintf(oldIPAddr,"%d.%d.%d.%d",oldDetails[INET_ADDR_LEN-4],  oldDetails[INET_ADDR_LEN-3], oldDetails[INET_ADDR_LEN-2], oldDetails[INET_ADDR_LEN-1]); 
	sprintf(oldSubnetAddr,"%d.%d.%d.%d", oldDetails[NETMASK_LEN], oldDetails[NETMASK_LEN+1], oldDetails[NETMASK_LEN+2], oldDetails[NETMASK_LEN+3]);
	sprintf(oldGatewayAddr,"%d.%d.%d.%d",oldDetails[IPCONF_PARAM_LENGTH-4],  oldDetails[IPCONF_PARAM_LENGTH-3], oldDetails[IPCONF_PARAM_LENGTH-2], oldDetails[IPCONF_PARAM_LENGTH-1]);

	EEPROM_IPCONF.Write(pIPConfigObj, IPCONF_PARAM_LENGTH+1, EEPROM_IPCONF_OFFSET); //updating the latest IP Configuration parameters into EEPROM
	
	for(IPByteIndex=(IP_LEN-IPBYTE), IPConfLen=NETMASK_LEN; IPConfLen<(IPCONF_PARAM_LENGTH-NETMASK_LEN); IPByteIndex=IPByteIndex-IPBYTE, IPConfLen++)
	{
		SubNetMask |= (pIPConfigObj[IPConfLen]<<IPByteIndex);
	}
		
	// load new IP configuration
	sprintf(IPAddr,"%d.%d.%d.%d",pIPConfigObj[INET_ADDR_LEN-4],  pIPConfigObj[INET_ADDR_LEN-3], pIPConfigObj[INET_ADDR_LEN-2], pIPConfigObj[INET_ADDR_LEN-1]); 
	sprintf(SubnetAddr,"%d.%d.%d.%d", pIPConfigObj[NETMASK_LEN], pIPConfigObj[NETMASK_LEN+1], pIPConfigObj[NETMASK_LEN+2], pIPConfigObj[NETMASK_LEN+3]);
	sprintf(GatewayAddr,"%d.%d.%d.%d",pIPConfigObj[IPCONF_PARAM_LENGTH-4],  pIPConfigObj[IPCONF_PARAM_LENGTH-3], pIPConfigObj[IPCONF_PARAM_LENGTH-2], pIPConfigObj[IPCONF_PARAM_LENGTH-1]);
	
	// although, there is no error checking in this method,
	// we will keep this for future error control 
	status = SUCCESS;
	
	if (status == SUCCESS)
	{
		// send it to the event log
		if (strcmp (IPAddr, oldIPAddr))
		{
			snprintf (event, sizeof(event), "Identifier:IP Address:From:%s:To:%s:", oldIPAddr, IPAddr);
			details = event;
			DataBaseManager::EnterEventLog (WEB_SERVICES, details);
		}
		if (strcmp (SubnetAddr, oldSubnetAddr))
		{
			snprintf (event, sizeof(event), "Identifier:Subnet Address:From:%s:To:%s:", oldSubnetAddr, SubnetAddr);
			details = event;
			DataBaseManager::EnterEventLog (WEB_SERVICES, details);
		}
		if (strcmp (GatewayAddr, oldGatewayAddr))
		{
			snprintf (event, sizeof(event), "Identifier:Gateway Address:From:%s:To:%s:", oldGatewayAddr, GatewayAddr);
			details = event;
			DataBaseManager::EnterEventLog (WEB_SERVICES, details);
		}
#if IPCONFIG_DBG
		printf ("\n%sIPConfig::SetIPParameters: IPAddr: %s SubnetAddr: %s GatewayAddr: %s %s\n", KBLU, IPAddr, SubnetAddr, GatewayAddr, KNRM);
#endif
	}
	
	return status;
}






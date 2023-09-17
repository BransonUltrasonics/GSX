/*
 * IpConfig.h
 *
 *  Created on: Jan 24, 2018
 *      Author: ramuks
 */

#ifndef IPCONFIG_HPP_
#define IPCONFIG_HPP_


#include <vxWorks.h>
#include <iostream>
#include <stdio.h>
#include "common.h"
#include "Eeprom.h"

#define	INET_ADDR_LEN	4
#define NETMASK_LEN 4
#define IPCONF_PARAM_LENGTH 12
#define IP_STRING_LENGTH 18
#define IP_LEN 32
#define IPBYTE 8

#define SUCCESS 1
#define FAILURE	0

class IPConfig
{
	public:	
		IPConfig();
		~IPConfig();
		STATUS GetIPParameters(char *pIPConfigObj);
		STATUS SetIPParameters(char *pIPConfigObj);
		
	private:
		Eeprom EEPROM_IPCONF;
};

#endif /* IPCONFIG_H_ */

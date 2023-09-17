/*
   * version.cpp
 *
 *  Created on: Sep 6, 2017
 *      Author: DShilonie
 */
#include  <sys/types.h>
#include  <cstdlib>
#include  <cstdio>
#include  <iostream>
#include  <string>
#include  <cstring>
#include  "versions.h"
#include "version_common.h"

using namespace std;


/*
 *  initialize this with your own version base on your FW_VER_IDX
 *  and default for others.
 *  
 */
UINT16 FWVersion::versions[FW_VERSION_MAX][VERSION_FIELD_MAX] = {
		{SC_FW_VERSION_MAJOR, SC_FW_VERSION_MINOR, SC_FW_VERSION_BUILD},
		{0,0,0},
		{0,0,0},
};

UINT16 FWCrc::crc[FW_CRC_MAX] = {0,0,0};

UINT32 FWInfo::info[FW_INFO_MAX] = {0,0,0,0};

FWVersion::FWVersion ()
{	
}

FWCrc::FWCrc ()
{	
}

FWInfo::FWInfo ()
{	
}


/**************************************************************************//**
* \brief  - gets the version for the requested component's field
*
* \param  - idx - 	index of specific component version to get
* \param  - field - index of specific field to get
*
*
* \return  - value of component version
*
******************************************************************************/
UINT16 FWVersion::Get(FW_VER_IDX idx, VER_FIELD_IDX field)
{
	return versions[idx][field];
}


/**************************************************************************//**
* \brief  - sets the version for the requested component's field
*
* \param  - idx - 	index of specific component's version to set
* \param  - field - index of specific field to set
* \param  - val - 	value of specific component's version to set
*
*
* \return  - nothing
*
******************************************************************************/
void FWVersion::Set(FW_VER_IDX idx, VER_FIELD_IDX field, UINT16 val)
{
	if ((idx < FW_VERSION_MAX) && (field < VERSION_FIELD_MAX))
		versions[idx][field] = val;
}


/**************************************************************************//**
* \brief  - sets the firmware's CRC for the requested component
*
* \param  - idx - 	index of specific component
* \param  - val - 	value of specific component's CRC
*
*
* \return  - nothing
*
******************************************************************************/
void FWCrc::Set(FW_CRC_IDX idx, UINT16 val)
{
	if (idx < FW_CRC_MAX)
		crc[idx] = val;
}


/**************************************************************************//**
* \brief  - gets the firmware's CRC for the requested component
*
* \param  - idx - 	index of specific component's CRC to get
*
*
* \return  - value of component's CRC
*
******************************************************************************/
UINT16 FWCrc::Get(FW_CRC_IDX idx)
{
	return crc[idx];
}


/**************************************************************************//**
* \brief  - sets the firmware's info for the requested component
*
* \param  - idx - 	index of specific component
* \param  - val - 	value of specific component's info
*
*
* \return  - nothing
*
******************************************************************************/
void FWInfo::Set(FW_INFO_IDX idx, UINT32 val)
{
	if (idx < FW_INFO_MAX)
		info[idx] = val;
}


/**************************************************************************//**
* \brief  - gets the firmware's info for the requested component
*
* \param  - idx - 	index of specific component's info to get
*
*
* \return  - value of component's info
*
******************************************************************************/
UINT32 FWInfo::Get(FW_INFO_IDX idx)
{
	return info[idx];
}

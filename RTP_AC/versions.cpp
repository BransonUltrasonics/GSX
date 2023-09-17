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
UINT32 FWVersion::versions[FW_VERSION_MAX][VERSION_FIELD_MAX] = {
		{0,0,0},
		{AC_FW_VERSION_MAJOR, AC_FW_VERSION_MINOR, AC_FW_VERSION_BUILD},
		{0,0,0},
};

/*Changed by MOUNIKA*/
UINT16 FWCrc::crc[FW_CRC_MAX] = {0,0,0};

UINT32 FWInfo::info[FW_INFO_MAX] = {0,0,0};

FWVersion::FWVersion ()
{	
}

FWCrc::FWCrc ()
{	
}

FWInfo::FWInfo ()
{	
}

/*
 *  gets the version info for the requested board and field
 */
UINT32 FWVersion::Get(FW_VER_IDX idx, VER_FIELD_IDX field)
{
	return versions[idx][field];
}

/*
 * sets the version info for the requested slave and field (SC only)
 */
void FWVersion::Set(FW_VER_IDX idx, VER_FIELD_IDX field, UINT32 val)
{
	if (!(idx > FW_VERSION_MAX) && !(field > VERSION_FIELD_MAX))
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



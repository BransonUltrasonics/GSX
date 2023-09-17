/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Eeprom class provides easy to use interface to interact with the NV Storage on boards
 
**********************************************************************************************************/


/* includes */

#include "Eeprom.h"

using namespace std;

/**************************************************************************//**
* \brief   -  constructor - Intializes the file descriptor variable and  
* 			  opens the device
*
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
Eeprom::Eeprom()
{
	fd=0;
	if ((fd = open(EEPROM_PATH, O_RDWR, 0)) == ERROR)
	{
		printf("EepromOpen: Failed to open the fd..\n");
	}	
}

/**************************************************************************//**
* \brief   -  destructor - Closes the file descriptor
*
* \param   -  None
*
* \return  -  None
*
******************************************************************************/
Eeprom :: ~Eeprom()
{
	close(fd);
}

/**************************************************************************//**
* \brief   -  Writes at the beginning of the eeeprom
*
* \param   -  char *pBuf,ssize_t length
*
* \return  -  ssize_t (Number of bytes written ) 
*
******************************************************************************/
ssize_t Eeprom :: Write(char *pBuf,ssize_t length)
{
	return write(fd, pBuf, length);		
}

/**************************************************************************//**
* \brief   -  Writing to the eeprom at the given offset. 
*
* \param   -  char *pBuf,ssize_t length,ssize_t offset
*
* \return  -  ssize_t (Number of bytes written /ERROR) 
*
******************************************************************************/
ssize_t Eeprom :: Write(char *pBuf,ssize_t length,ssize_t offset)
{
	int status;
	
	if((status = SetCurOffset(offset)) == ERROR)
		return status;
	
	return write(fd, pBuf, length);	
}

/**************************************************************************//**
* \brief   -  Reads the eeprom from the starting and for the given length 
*
* \param   -  char *pBuf,ssize_t length
*
* \return  -  ssize_t Number of bytes read
*
******************************************************************************/
ssize_t Eeprom :: Read(char *pBuf,ssize_t length)
{	
	return read(fd, pBuf, length);
}

/**************************************************************************//**
* \brief   -  Reads the eeprom from the given offset and for  the given length 
*
* \param   -  char *pBuf,ssize_t length,ssize_t offset
*
* \return  -  ssize_t - ERROR / Number of bytes read
*
******************************************************************************/
ssize_t Eeprom :: Read(char *pBuf,ssize_t length,ssize_t offset)
{
	int status;
	
	if((status = SetCurOffset(offset)) == ERROR)
		return status;
	
	return read(fd, pBuf, length);	
}

/**************************************************************************//**
* \brief   -   Sets the file descriptor for the given  offset 
*
* \param   -  ssize_t offset
*
* \return  -  ssize_t - value (0 to 32768 - 1) 
*
******************************************************************************/
ssize_t Eeprom :: SetCurOffset(ssize_t offset)
{
	return ioctl (fd, FIOSEEK, offset);
}

/**************************************************************************//**
* \brief   -   Returns the current file descriptor position 
*
* \param   -  None
*
* \return  -  ssize_t - value (0 to 32768 - 1)
*
******************************************************************************/
ssize_t Eeprom :: GetCurOffset()
{
	return ioctl(fd,FIOWHERE,0);
}

/**************************************************************************//**
* \brief   -   Returns the number of bytes present form current file position
* 			   to end of the file
*
* \param   -  None
*
* \return  -  ssize_t count
*
******************************************************************************/
ssize_t Eeprom :: SizeAvlbl()
{
	ssize_t count =0;
	int status;
		
	if((status = ioctl(fd, FIONREAD, &count)) == ERROR)
		return status;
	
	return count;
}


/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef EEPROM_H_
#define EEPROM_H_

#include <vxWorks.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ioLib.h>

/* Macros */
#define MAX_EEPROM_SIZE 32768
#define EEPROM_PATH	"/eeprom/0"

class Eeprom
{
	public:	
		Eeprom();
		ssize_t Write(char *pBuf, ssize_t length);
		ssize_t Write(char *pBuf, ssize_t length, ssize_t offset);
		ssize_t Read(char *pBuf, ssize_t length);
		ssize_t Read(char *pBuf, ssize_t length, ssize_t offset);
		ssize_t SetCurOffset(ssize_t offset);
		ssize_t GetCurOffset();
		ssize_t SizeAvlbl();
		~Eeprom();
		
	private:	
		INT32    fd;
};

#endif /* EEPROM_H_ */

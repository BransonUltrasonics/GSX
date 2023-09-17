/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     RecipeZeroValidate class  for Recipe zero file validation.  
 
**********************************************************************************************************/


#include "FirmwareUpgrade.h"
#include "RecipeZeroValidate.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "Logger.h"
using namespace std;

extern "C"
{
	#include "customSystemCall.h"	
}

/**************************************************************************//**
* 
* \brief   - Class constructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
RecipeZeroValidate::RecipeZeroValidate()
{

}

/**************************************************************************//**
* 
* \brief   - Class destructor
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
RecipeZeroValidate::~RecipeZeroValidate()
{
	
}

/**************************************************************************//**
* 
* \brief   - Calculate CRC of RecipeZero file .
*
* \param   - Offset- It is an integer value which represents the offset in the stream’s buffer, relative to the dir parameter.
*			 filePath- RecipeZro file path.
* \return  - CRC (2 bytes)
*
******************************************************************************/
unsigned short RecipeZeroValidate::CalCrcRecipeZeroFile(INT8 offset,char* filePath)
{
	
	fstream fp;
	UINT32 recipeZeroFileSize=0;
	unsigned short recipeZeroCrc=0;
    fp.open(filePath, ios::in|ios::binary);
	if(fp.is_open())
	{	
		fp.seekg(offset, ios::end);
		recipeZeroFileSize=fp.tellg();
		fp.seekg(0, ios::beg);
		
		/* Allocate memory */
		char *tmpBuf =new (std::nothrow)char[recipeZeroFileSize];
		if (tmpBuf== NULL)
		{
			LOGERR("RZ_T : MEMMORY ALLOCATION FAILED in CalCrcRecipeZeroFile",0,0,0);
			return 0;
		}
		/* Read file contents into  Buffer */
		if(fp.read(tmpBuf,recipeZeroFileSize))
		{
			recipeZeroCrc = crc16Ccitt((char *) tmpBuf, recipeZeroFileSize);
		}
		delete[] tmpBuf;
		tmpBuf=NULL;
		fp.seekg(0, ios::beg);
		fp.close();
	}
	return recipeZeroCrc;
}

/**************************************************************************//**
* 
* \brief   - Insert  CRC at the end of RecipeZero file .
*
* \param   - CRC value.
*			
* \return  - Result(0 or 1)
*
******************************************************************************/
bool RecipeZeroValidate::InsertCrcInRecipeZeroFile(unsigned short recipeZeroCrc )
{
	fstream fp;
	bool status=false;
	char *tmpBuf =new (std::nothrow)char[CRCBYTESLEN+1];
	if (tmpBuf== NULL)
	{
		LOGERR("RZ_T : MEMMORY ALLOCATION FAILED in InsertCrcInRecipeZeroFile",0,0,0);
		return false;
	}
	sprintf(tmpBuf,"0x%X",recipeZeroCrc);
	fp.open(RECIPE_ZERO_PATH, ios::out|ios::app|ios::binary);
	if(fp.is_open())
	{
		fp.write(tmpBuf,strlen(tmpBuf));
		fp.seekg(0, ios::beg);
		fp.close();
		status=true;
	}
	delete[] tmpBuf;
	tmpBuf=NULL;
	return status;
}

/**************************************************************************//**
* 
* \brief   - Check if  RecipeZero file is empty or not. .
*
* \param   - filepath of recipe zero file.
*			
* \return  - Result(0 or 1)
*
******************************************************************************/
bool RecipeZeroValidate::IsFileEmpty(char* filePath)
{
	fstream fp;UINT32 recipeZeroFileSize=0;
	bool status=false;
    fp.open(filePath, ios::in|ios::binary);
	if(fp.is_open())
	{
		fp.seekg(0, ios::end);
		recipeZeroFileSize=fp.tellg();
		if(recipeZeroFileSize < CRCBYTESLEN)
		{
			status=true;
		}else
		{
			status=false;
		}
		fp.close();
	}
	return status;
}

/**************************************************************************//**
* 
* \brief   - Validate RecipeZero file.
*
* \param   - filepath of recipe zero file.
*			
* \return  - Result(0 or 1)
*
******************************************************************************/
bool RecipeZeroValidate::ValidateRecipeZeroFile(char* filePath)
{
	
	unsigned short calRecipeZeroCrc=0;
	fstream fp;
	bool status=false;
	char calCrcBuf[CRCBYTESLEN+1];
	
	if(IsFileEmpty(filePath))
	{
		status=false;
	}
	else
	{
		char *recipeZeroCrcBuf =new (std::nothrow)char[CRCBYTESLEN+1];
		if (recipeZeroCrcBuf== NULL)
		{
			LOGERR("RZ_T : MEMMORY ALLOCATION FAILED in ValidateRecipeZeroFile",0,0,0);
			return false;
		}
		calRecipeZeroCrc=CalCrcRecipeZeroFile(-CRCBYTESLEN,filePath);
		sprintf(calCrcBuf,"0x%X",calRecipeZeroCrc);
		
		fp.open(filePath, ios::in|ios::binary);
		if(fp.is_open())
		{
			fp.seekg(-CRCBYTESLEN, ios::end);
			fp.read(recipeZeroCrcBuf,CRCBYTESLEN);
			if(strncmp(calCrcBuf,recipeZeroCrcBuf,CRCBYTESLEN)==0)
			{
				status=true;
			}
			else
			{
				status=false;
			}
			fp.seekg(0, ios::beg);
			fp.close();
		}
		delete[] recipeZeroCrcBuf;
		recipeZeroCrcBuf=NULL;
	}
	return status;
}

/**************************************************************************//**
 * \brief   - Calculates CRC of RecipeZero File
 *
 * \param   - const void *dataBuffer, UINT32 buffLen
 *
 * \return  - UINT16 - CRC value
 *
 ******************************************************************************/
unsigned short RecipeZeroValidate::crc16Ccitt(const void *buf, int len)
{

	int counter;
	unsigned short crc = 0;
	for (counter = 0; counter < len; counter++) 
	{
		crc = (crc << CRC_BIT_SHIFT)
			^ crc16tab[((crc >> CRC_BIT_SHIFT) ^ *(char *) buf) & 0x00FF];
		buf = ((char *) buf) + 1;
	}
	return crc;
}

/**************************************************************************//**
* 
* \brief   - Copying file.
*
* \param   - srceFile-Path of source file.
*			 destFile-Path of destination file.
* \return  - None
*
******************************************************************************/
void RecipeZeroValidate::CopyFile( const char* srceFile, const char* destFile )
{
    std::ifstream srce( srceFile, std::ios::binary ) ;
    std::ofstream dest( destFile, std::ios::binary ) ;
    dest << srce.rdbuf() ;
}


/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/


#ifndef RECIPEZEROVALIDATE_H_
#define RECIPEZEROVALIDATE_H_

#include "Common.h"
#include "CommonProperty.h"

#define CRCBYTESLEN     6

class RecipeZeroValidate
{

public:
	RecipeZeroValidate();
    ~RecipeZeroValidate();
    
    unsigned short CalCrcRecipeZeroFile(INT8,char* );
    bool InsertCrcInRecipeZeroFile(unsigned short );
    bool ValidateRecipeZeroFile(char* );
    void CopyFile( const char* srceFile, const char* destFile );
private:
    unsigned short crc16Ccitt(const void *buf, int len);
    bool IsFileEmpty(char* filePath);
  
};

#endif /* RECIPEZEROVALIDATE_H_ */

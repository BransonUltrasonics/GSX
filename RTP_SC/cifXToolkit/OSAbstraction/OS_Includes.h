/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: OS_Includes.h 12297 2018-08-09 12:16:12Z Robert $:

  Description:
    Headerfile for specific operating system includes

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2018-08-09  changed file header
    2010-04-25  initial version

**************************************************************************************/

#ifndef __OS_INCLUDES_H_
#define __OS_INCLUDES_H_

#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* Ignore redundant declarations to suppress warning caused by 
   redundant declaration in cifXToolkit.h and cifXHWFunctions.h */
//#pragma GCC diagnostic ignored "-Wredundant-decls"

#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a<b)?b:a)
#define UNREFERENCED_PARAMETER(x) (x=x)
#define APIENTRY

extern void HAL_GetSystime       ( uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser );
extern void HAL_SetSystimeBorder ( uint32_t ulBorder, void* pvUser );

#endif /* __OS_INCLUDES_H_ */

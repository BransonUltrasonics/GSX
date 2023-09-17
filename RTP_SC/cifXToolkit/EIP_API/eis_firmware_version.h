/**************************************************************************************
  Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
  $Id:$

  Description:
    EtherNet/IP Version

**************************************************************************************/
#ifndef EIS_FIRMWARE_VERSION_H_
#define EIS_FIRMWARE_VERSION_H_

#define EIS_FIRMWARE_VERSION_MAJOR        3
#define EIS_FIRMWARE_VERSION_MINOR        7
#define EIS_FIRMWARE_VERSION_BUILD        0
#define EIS_FIRMWARE_VERSION_REVISION     10

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if !defined(EIS_FIRMWARE_VERSION_MAJOR) || !defined(EIS_FIRMWARE_VERSION_MINOR) || !defined(EIS_FIRMWARE_VERSION_BUILD) || !defined(EIS_FIRMWARE_VERSION_REVISION)
#error "Missing version define"
#endif

#define EIS_VERSION "EtherNet/IP V" TOSTRING(EIS_FIRMWARE_VERSION_MAJOR) "." TOSTRING(EIS_FIRMWARE_VERSION_MINOR) "." TOSTRING(EIS_FIRMWARE_VERSION_BUILD) "." TOSTRING(EIS_FIRMWARE_VERSION_REVISION)


#endif

/*************************************************************************************/
/*-----------------------------------------------------------------------------------*/
/*-------------- EOF ----------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*************************************************************************************/

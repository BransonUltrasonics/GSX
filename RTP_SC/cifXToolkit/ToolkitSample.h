/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: ToolkitSample.h 14195 2021-09-02 12:24:48Z AMinor $:

  Description:
    Toolkit sample header

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2021-09-01  Change prototype of ToolkitSample() and added new defines
    2018-07-24  Added function prototype
    2011-10-03  Initial version derived from windows example

**************************************************************************************/

#ifndef __CIFXSAMPLE_TASK
#define __CIFXSAMPLE_TASK

#include <cifxToolkit.h>

/*****************************************************************************/
/*! nonOS toolkit device instance                                            */
/*****************************************************************************/
typedef struct NONOS_DEVICEINSTANCE_TTag
{
  DEVICEINSTANCE tDevice;
  uint32_t       ulSerLock;

} NONOS_DEVICEINSTANCE_T;

#define SAMPLE_TYPE_SPM  1
#define SAMPLE_TYPE_DPM  2

/* Function prototype */
void ToolkitSample(uint32_t ulType, uint8_t* pbDPM);

#endif /* __CIFXSAMPLE_TASK */

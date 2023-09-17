/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: hifdpm_hwif.c 14195 2021-09-02 12:24:48Z AMinor $:

  Description:
    HWIF functions for HIFDPM

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2021-09-01  Initial version

**************************************************************************************/
#include <cifxToolkit.h>

/*****************************************************************************/
/*! Read a number of bytes from DPM
*   \param pvDevInstance  Device Instance
*   \param pvAddr         Address offset in DPM to read data from
*   \param pvData         Buffer to store read data
*   \param ulLen          Number of bytes to read                            */
/*****************************************************************************/
void* hifdpm_read( void* pvDevInstance, void* pvAddr, void* pvData, uint32_t ulLen)
{
  return memcpy(pvData, pvAddr, ulLen);
}

/*****************************************************************************/
/*! Write a number of bytes to DPM
*   \param pvDevInstance  Device Instance
*   \param pvAddr         Address offset in DPM to write data to
*   \param pvData         Data to write
*   \param ulLen          Number of bytes to write                           */
/*****************************************************************************/
void* hifdpm_write( void* pvDevInstance, void* pvAddr, void* pvData, uint32_t ulLen)
{
  return memcpy(pvAddr, pvData, ulLen);
}

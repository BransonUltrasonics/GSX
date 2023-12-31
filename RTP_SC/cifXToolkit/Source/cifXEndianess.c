/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXEndianess.c 13253 2019-10-11 10:57:04Z LuisContreras $:

  Description:
    Little/Big Endian conversion

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2019-10-11  Change prototype of endianess conversion function
    2018-10-10  - Updated header and definitions to new Hilscher defines
                - Derived from cifX Toolkit V1.6.0.0

**************************************************************************************/

/*****************************************************************************/
/*! \file cifXEndianess.c
*   Little/Big Endian conversion                                             */
/*****************************************************************************/

#include "cifXToolkit.h"
#include "cifXErrors.h"
#include "cifXEndianess.h"

/*****************************************************************************/
/*! Convert a buffer from / to host endianess
*   this structure is used for automatically transforming a structure (which
*   is described by this structure) from/to host endianess                   */
/*****************************************************************************/
int32_t cifXConvertEndianess(unsigned int uiOffset, void* pvBuffer, int iBufferLen,
                             const CIFX_ENDIANESS_ENTRY_T* atConv, int iConvLen)
{
/* Conversion needs only be done, it host and dpm endianess differ */
#ifdef CIFX_TOOLKIT_BIGENDIAN
  /* We need to make sure, that we can handle odd offsets, so we are using
     a byte pointer here and build the appropriate data type byte by byte.
     Compilers provide packing/unaligned access in different ways, and we
     are not able to generalize it */
  uint8_t* pbBuffer      = (uint8_t*)pvBuffer;
  int      iActConvEntry;
  int      iActElement;

#ifdef CIFX_TOOLKIT_PARAMETER_CHECK
  if((NULL == pvBuffer) || (NULL == atConv))
    return CIFX_INVALID_POINTER;
#endif /* CIFX_TOOLKIT_PARAMETER_CHECK */

  /* On Big-endian targets, the given block shall only be read from start */
  if(0 != uiOffset)
    return CIFX_INVALID_PARAMETER;

  /* Iterate over complete user table */
  for(iActConvEntry = 0; iActConvEntry < iConvLen; ++iActConvEntry)
  {
    /* Iterate over all elements given from this entry. (From iOffset to iOffset + iElementCnt * size) */
    for(iActElement = 0; iActElement < atConv[iActConvEntry].iElementCnt; ++iActElement)
    {
      switch(atConv[iActConvEntry].eWidth)
      {
      case eCIFX_ENDIANESS_WIDTH_8BIT:
        /* nothing to do for 8 bit */
        break;

      case eCIFX_ENDIANESS_WIDTH_16BIT:
        {
          uint16_t usValue;
          int      iOffset = atConv[iActConvEntry].iOffset + iActElement * 2;

          if( (iOffset + 2) < iBufferLen)
          {
            OS_Memcpy(&usValue, pbBuffer + iOffset, 2);
            usValue = LE16_TO_HOST(usValue);
            OS_Memcpy(pbBuffer + iOffset, &usValue, 2);
          }
        }
        break;

      case eCIFX_ENDIANESS_WIDTH_32BIT:
        {
          uint32_t ulValue;
          int      iOffset = atConv[iActConvEntry].iOffset + iActElement * 4;

          if( (iOffset + 4) < iBufferLen)
          {
            OS_Memcpy(&ulValue, pbBuffer + iOffset, 4);
            ulValue = LE32_TO_HOST(ulValue);
            OS_Memcpy(pbBuffer + iOffset, &ulValue, 4);
          }
        }
        break;

      case eCIFX_ENDIANESS_WIDTH_64BIT:
        {
          uint64_t ullValue;
          int      iOffset = atConv[iActConvEntry].iOffset + iActElement * 8;

          if( (iOffset + 8) < iBufferLen)
          {
            OS_Memcpy(&ullValue, pbBuffer + iOffset, 8);
            ullValue = LE64_TO_HOST(ullValue);
            OS_Memcpy(pbBuffer + iOffset, &ullValue, 8);
          }
        }
        break;

      default:
        /* This should never happen */
        break;
      }
    }
  }
  return CIFX_NO_ERROR;
#else
  UNREFERENCED_PARAMETER(uiOffset);
  UNREFERENCED_PARAMETER(pvBuffer);
  UNREFERENCED_PARAMETER(iBufferLen);
  UNREFERENCED_PARAMETER(atConv);
  UNREFERENCED_PARAMETER(iConvLen);
  return CIFX_NO_ERROR; /*lint !e438 : unused variables */
#endif /* CIFX_TOOLKIT_BIGENDIAN */
}

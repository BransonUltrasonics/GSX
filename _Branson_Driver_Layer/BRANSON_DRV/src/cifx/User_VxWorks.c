/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: User_VxWorks.c 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    USER implemented functions called by the cifX Toolkit.

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  created from VxWorks driver V6.x revision 5

**************************************************************************************/

/*****************************************************************************/
/*! \file User_VxWorks.c
*   USER implemented functions called by the cifX Toolkit.                   */
/*****************************************************************************/

#include "cifXToolkit/cifXToolkit.h"
#include "cifXToolkit/WarmstartFile.h"
#include "cifXToolkit/cifXErrors.h"
#include <dirent.h>

char* g_szDriverBaseDir                     = NULL;
BOOL  g_fSingleDir                          = FALSE;

static const char* DEVICE_CONF_ALIAS_KEY    = "alias=";
static const char* DEVICE_CONF_IRQ_KEY      = "irq=";

#ifdef CIFX_TOOLKIT_DMA
static const char* DEVICE_CONF_DMA_KEY      = "dma=";
#endif

#define CIFx_2NDSTAGE_LOADERFILE_NETX50   "NETX50-BSL.bin"
#define CIFx_2NDSTAGE_LOADERFILE_NETX51   "NETX51-BSL.bin"
#define CIFx_2NDSTAGE_LOADERFILE_NETX52   "NETX52-BSL.bin"
#define CIFx_2NDSTAGE_LOADERFILE_NETX100  "NETX100-BSL.bin"

/*****************************************************************************/
/* Internally used helper functions                                          */
/*****************************************************************************/

/*****************************************************************************/
/*!  \addtogroup CIFX_TK_USER User specific implementation
*    \{                                                                      */
/*****************************************************************************/

/*****************************************************************************/
/*! Gets the name of a file in a specified directory
*     \param szDirectoy    Directory to search for the file
*     \param fFirmwareFile !=0 to get firmware filename
*     \param ulFileNo      Number of file in the directory
*     \param ptFileInfo    Returns short and full file name
*     \return Returns 1 if the specified file is in the directory            */
/*****************************************************************************/
LOCAL int GetFileName(char* szDirectory, int fFirmwareFile, uint32_t ulFileNo, CIFX_FILE_INFORMATION* ptFileInfo)
{
  int           fRet          = 0;
  uint32_t      ulCurrentFile = 0;
  DIR*          hDirList      = opendir(szDirectory);
  if(NULL != hDirList)
  {
    struct dirent* ptDir;
    do
    {
      ptDir = readdir(hDirList);
      if(NULL != ptDir)
      {
        int iStrLen  = OS_Strlen(ptDir->d_name);
        int fProcess = 0;

        if(iStrLen <= 4)
        {
          /* The file does not have an extension with 3 chars,
           * so it does not need to be checked */
          continue;
        }

        /* We assume 8.3 format here */
        if(fFirmwareFile)
        {
          if( (OS_Strnicmp(&ptDir->d_name[iStrLen - 4], ".NXO", 4) == 0) ||
              (OS_Strnicmp(&ptDir->d_name[iStrLen - 4], ".NXF", 4) == 0) )
          {
            fProcess = 1;
          }

        } else
        {
          if( (OS_Strnicmp(&ptDir->d_name[iStrLen - 4], ".NXD", 4) == 0) ||
              (OS_Strnicmp(&ptDir->d_name[iStrLen - 4], ".DBM", 4) == 0) )
          {
            fProcess = 1;
          }
        }

        if(fProcess)
        {
          if(ulCurrentFile == ulFileNo)
          {
            /* this is the requested file */
            OS_Strncpy(ptFileInfo->szShortFileName, ptDir->d_name, sizeof(ptFileInfo->szFullFileName));
            snprintf(ptFileInfo->szFullFileName,
                     sizeof(ptFileInfo->szFullFileName),
                     "%s/%s",
                     szDirectory,
                     ptDir->d_name);
            fRet = 1;
            break;

          } else
          {
            ++ulCurrentFile;
          }
        }
      }
    } while(NULL != ptDir);

    closedir(hDirList);
  }
  return fRet;
}

/*****************************************************************************/
/*! Counts the number of files with a specific file extension in a directory
*     \param szDirectoy  Directory to search for the files
*     \param szExtension Only files with this extension are counted
*     \return Number of files                                                */
/*****************************************************************************/
LOCAL uint32_t GetNumberOfFiles(char* szDirectory, char* szExtension)
{
  uint32_t ulRet    = 0;
  DIR*     hDirList = opendir(szDirectory);
  if(NULL != hDirList)
  {
    struct dirent* ptDir;
    do
    {
      ptDir = readdir(hDirList);
      if(NULL != ptDir)
      {
        int iStrLen = OS_Strlen(ptDir->d_name);

        /* We assume 8.3 format here */
        if( (4 <= iStrLen)                                                 &&
            (0 == OS_Strnicmp(&ptDir->d_name[iStrLen - 4], szExtension, 4))  )
        {
          ++ulRet;
        }
      }
    } while(NULL != ptDir);
    closedir(hDirList);
  }

  return ulRet;
}

#define PARSER_BUFFER_SIZE  1024

/*****************************************************************************/
/*! Reads a value from an config file (ini-file style)
*     \param szFile   Path to the ini file
*     \param szKey    Key to search for (including trailing '=')
*     \param szValue  Pointer to returned value (needs to be free'd by caller)
*     \return !=0 on success                                                 */
/*****************************************************************************/
LOCAL int GetDeviceConfigString(char* szFile, const char* szKey, char** szValue)
{
  int   iRet = 0;
  FILE* fd  = fopen(szFile, "r");

  if(NULL != fd)
  {
    /* File is open */
    char* pbBuffer = OS_Memalloc(PARSER_BUFFER_SIZE);

    /* Read file line by line */
    while(NULL != fgets(pbBuffer, PARSER_BUFFER_SIZE, fd))
    {
      char* key;

      /* '#' marks a comment line in the device.conf file */
      if(pbBuffer[0] == '#')
        continue;

      /* Search for key in the input buffer */
      key = strstr(pbBuffer, szKey);

      if(NULL != key)
      {
        /* We've found the key */
        int   allocsize  = OS_Strlen(key + OS_Strlen(szKey)) + 1;
        int   valuelen;
        char* tempstring = (char*)OS_Memalloc(allocsize);

        strcpy(tempstring, key + OS_Strlen(szKey));
        valuelen = OS_Strlen(tempstring);

        /* strip all trailing whitespaces */
        while( (tempstring[valuelen - 1] == '\n') ||
               (tempstring[valuelen - 1] == '\r') ||
               (tempstring[valuelen - 1] == ' ') )
        {
          tempstring[valuelen - 1] = '\0';
          --valuelen;
        }

        *szValue = tempstring;
        iRet = 1;
        break;
      }
    }

    OS_Memfree(pbBuffer);
    fclose(fd);
  }

  return iRet;
}

/*****************************************************************************/
/*! Internal helper function returning the path to a channel directory
*   on the given device (e.g. /opt/cifx/deviceconfig/1250100/20004/)
*     \param szPath         Pointer to returned path
*     \param iPathLen       Length of the buffer passed in szPath
*     \param ptDevInfo      Device information (DeviceNr, SerialNr, ChannelNr)*/
/*****************************************************************************/
LOCAL void GetDeviceDir(char* szPath, size_t iPathLen,PCIFX_DEVICE_INFORMATION ptDevInfo)
{
  if (g_fSingleDir)
  {
    /* Use single firmware directory */
  snprintf(szPath, iPathLen, "%s/FW/",
       g_szDriverBaseDir);

  } else if (0 == ptDevInfo->ptDeviceInstance->ulSlotNumber)
  {
    /* Use the serial and device number for the Device directory path */
  snprintf(szPath, iPathLen, "%s/%d_%d/",
       g_szDriverBaseDir,
       (unsigned int)ptDevInfo->ulDeviceNumber,
       (unsigned int)ptDevInfo->ulSerialNumber);
  } else
  {
    /* Use the Slot number for the Device directory path */
  snprintf(szPath, iPathLen, "%s/Slot_%d/",
       g_szDriverBaseDir,
       (unsigned int)ptDevInfo->ptDeviceInstance->ulSlotNumber);
  }
}

/*****************************************************************************/
/*! Internal helper function returning the path to a channel directory
*   on the given device (e.g. /opt/cifx/deviceconfig/1250100/20004/channel0/)
*     \param szPath         Pointer to returned path
*     \param iPathLen       Length of the buffer passed in szPath
*     \param ptDevInfo      Device information (DeviceNr, SerialNr, ChannelNr)*/
/*****************************************************************************/
LOCAL void GetChannelDir(char* szPath, size_t iPathLen,PCIFX_DEVICE_INFORMATION ptDevInfo)
{
  GetDeviceDir(szPath, iPathLen, ptDevInfo);

  snprintf(szPath, iPathLen, "%schannel%d/",
         szPath,
           (unsigned int)ptDevInfo->ulChannel);
}

/*****************************************************************************/
/*! Returns the number of firmware files associated with the card/channel,
*   passed as argument.
*   \param ptDevInfo DeviceInfo including channel number, for which the
*                    firmware file count should be read
*   \return number for firmware files, to download. The returned value will
*           be used as maximum value for ulIdx in calls to
*           USER_GetFirmwareFile                                             */
/*****************************************************************************/
uint32_t USER_GetFirmwareFileCount(PCIFX_DEVICE_INFORMATION ptDevInfo)
{
  uint32_t ulRet = 0;

  if(NULL != g_szDriverBaseDir)
  {
    char szDir[CIFX_MAX_FILE_NAME_LENGTH];

    GetChannelDir(szDir, sizeof(szDir), ptDevInfo);

    /* count all .NXF, .NXO files */
    ulRet = GetNumberOfFiles(szDir, ".NXO") +
            GetNumberOfFiles(szDir, ".NXF");
  }

  return ulRet;
}

/*****************************************************************************/
/*! Returns firmware file information for the given device/channel and Idx
*   passed as argument.
*   \param ptDevInfo  DeviceInfo including channel number, for which the
*                     firmware file should be delivered
*   \param ulIdx      Index of the returned file
*                     (0..USER_GetFirmwareFileCount() - 1)
*   \param ptFileInfo Short and full file name of the firmware. Used in
*                     calls to OS_OpenFile()
*   \return !=0 on success                                                   */
/*****************************************************************************/
int USER_GetFirmwareFile(PCIFX_DEVICE_INFORMATION ptDevInfo, uint32_t ulIdx, PCIFX_FILE_INFORMATION ptFileInfo)
{
  int iRet = 0;

  if(NULL != g_szDriverBaseDir)
  {
    char szDir[CIFX_MAX_FILE_NAME_LENGTH];

    GetChannelDir(szDir, sizeof(szDir), ptDevInfo);

    iRet = GetFileName(szDir, 1, ulIdx, ptFileInfo);
  }

  return iRet;
}

/*****************************************************************************/
/*! Returns the number of configuration files associated with the card/
*   channel, passed as argument.
*   \param ptDevInfo DeviceInfo including channel number, for which the
*                    configuration file count should be read
*   \return number for confgiuration files, to download. The returned value
*           will be used as maximum value for ulIdx in calls to
*           USER_GetConfgirationFile                                         */
/*****************************************************************************/
uint32_t USER_GetConfigurationFileCount(PCIFX_DEVICE_INFORMATION ptDevInfo)
{
  uint32_t ulRet = 0;

  if(NULL != g_szDriverBaseDir)
  {
    char szDir[CIFX_MAX_FILE_NAME_LENGTH];

    GetChannelDir(szDir, sizeof(szDir), ptDevInfo);

    /* count all .NXD, .DBM files */
    ulRet = GetNumberOfFiles(szDir, ".NXD") +
            GetNumberOfFiles(szDir, ".DBM");
  }

  return ulRet;
}

/*****************************************************************************/
/*! Returns configuration file information for the given device/channel and
*   Idx passed as argument.
*   \param ptDevInfo  DeviceInfo including channel number, for which the
*                     configuration file should be delivered
*   \param ulIdx      Index of the returned file
*                     (0..USER_GetConfigurationFileCount() - 1)
*   \param ptFileInfo Short and full file name of the configuration. Used in
*                     calls to OS_OpenFile()
*   \return !=0 on success                                                   */
/*****************************************************************************/
int USER_GetConfigurationFile(PCIFX_DEVICE_INFORMATION ptDevInfo, uint32_t ulIdx, PCIFX_FILE_INFORMATION ptFileInfo)
{
  int iRet = 0;

  if(NULL != g_szDriverBaseDir)
  {
    char szDir[CIFX_MAX_FILE_NAME_LENGTH];

    GetChannelDir(szDir, sizeof(szDir), ptDevInfo);

    iRet = GetFileName(szDir, 0, ulIdx, ptFileInfo);
  }

  return iRet;
}

/*****************************************************************************/
/*! Retrieve the full file name of the cifX Romloader binary image
*   \param ptDevInstance  Pointer to the device instance
*   \param ptFileInfo Short and full file name of the bootloader. Used in
*                     calls to OS_OpenFile()
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
void USER_GetBootloaderFile(PDEVICEINSTANCE ptDevInstance, PCIFX_FILE_INFORMATION ptFileInfo)
{
  if(NULL == g_szDriverBaseDir)
    return;

  switch(ptDevInstance->eChipType)
  {
  case eCHIP_TYPE_NETX500:
  case eCHIP_TYPE_NETX100:
  snprintf(ptFileInfo->szFullFileName, sizeof(ptFileInfo->szFullFileName),
       "%s/%s", g_szDriverBaseDir, CIFx_2NDSTAGE_LOADERFILE_NETX100);
  strncpy(ptFileInfo->szShortFileName, CIFx_2NDSTAGE_LOADERFILE_NETX100,
      sizeof(ptFileInfo->szShortFileName));
    break;

  case eCHIP_TYPE_NETX51:
    /* This is a netX51 device */
  snprintf(ptFileInfo->szFullFileName, sizeof(ptFileInfo->szFullFileName),
       "%s/%s", g_szDriverBaseDir, CIFx_2NDSTAGE_LOADERFILE_NETX51);
  strncpy(ptFileInfo->szShortFileName, CIFx_2NDSTAGE_LOADERFILE_NETX51,
      sizeof(ptFileInfo->szShortFileName));
    break;

  case eCHIP_TYPE_NETX52:
    /* This is a netX52 device */
  snprintf(ptFileInfo->szFullFileName, sizeof(ptFileInfo->szFullFileName),
       "%s/%s", g_szDriverBaseDir, CIFx_2NDSTAGE_LOADERFILE_NETX52);
  strncpy(ptFileInfo->szShortFileName, CIFx_2NDSTAGE_LOADERFILE_NETX52,
      sizeof(ptFileInfo->szShortFileName));
    break;

  case eCHIP_TYPE_NETX50:
    /* new bootloader for netX50 will default to NETX50-BSL.bin (which is the default toolkit delivery) */
  snprintf(ptFileInfo->szFullFileName, sizeof(ptFileInfo->szFullFileName),
       "%s/%s", g_szDriverBaseDir, CIFx_2NDSTAGE_LOADERFILE_NETX50);
  strncpy(ptFileInfo->szShortFileName, CIFx_2NDSTAGE_LOADERFILE_NETX50,
      sizeof(ptFileInfo->szShortFileName));
    break;

  default:
    /* This should never happen */
    break;
  }
}

/*****************************************************************************/
/*! Returns OS file information for the given device/channel and Idx
*   passed as argument.
*   \param ptDevInfo  DeviceInfo including channel number, for which the
*                     firmware file should be delivered
*   \param ptFileInfo Short and full file name of the firmware. Used in
*                     calls to OS_OpenFile()
*   \return != 0 on success                                                   */
/*****************************************************************************/
int USER_GetOSFile(PCIFX_DEVICE_INFORMATION ptDevInfo, PCIFX_FILE_INFORMATION ptFileInfo)
{
  int   iRet     = 0;

  if(NULL != g_szDriverBaseDir)
  {
    char szDir[CIFX_MAX_FILE_NAME_LENGTH];

    GetDeviceDir(szDir, sizeof(szDir), ptDevInfo);

    iRet = GetFileName(szDir, 1, 0, ptFileInfo);
  }

  return iRet;
}

/*****************************************************************************/
/*! Retrieve the alias name of a card. This name can be alternatively used
*   by a application in a call to xChannelOpen. This name must be unique
*     \param ptDevInfo  Device information (DeviceNr, SerialNr)
*     \param ulMaxLen   Maximum Length of alias
*     \param szAlias    Pointer to returned alias name (empty = no alias)    */
/*****************************************************************************/
void USER_GetAliasName(PCIFX_DEVICE_INFORMATION ptDevInfo, uint32_t ulMaxLen, char* szAlias)
{
  char  szFile[CIFX_MAX_FILE_NAME_LENGTH];
  char* szTempAlias = NULL;

  GetDeviceDir(szFile, sizeof(szFile), ptDevInfo);
  strcat(szFile, "/device.conf");

  /* Read alias from file */
  if(GetDeviceConfigString(szFile, DEVICE_CONF_ALIAS_KEY, &szTempAlias))
  {
    OS_Strncpy(szAlias, szTempAlias, ulMaxLen);
    OS_Memfree(szTempAlias);
  } else
  {
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(ptDevInfo->ptDeviceInstance,
                 TRACE_LEVEL_ERROR,
                 "No Alias defined for '%s'",
                 ptDevInfo->ptDeviceInstance->szName);
    }
  }
}

/*****************************************************************************/
/*! Read the warmstart data from a given warmstart file
*   \param ptDevInfo Device- and Serial number of the card
*   \param ptPacket  Buffer for the warmstart packet                         */
/*****************************************************************************/
int USER_GetWarmstartParameters(PCIFX_DEVICE_INFORMATION ptDevInfo, CIFX_PACKET* ptPacket)
{
  int fRet = 0;

  if(NULL != g_szDriverBaseDir)
  {
    char     szFile[CIFX_MAX_FILE_NAME_LENGTH];
    void*    pvFile;
    uint32_t ulFileLen;

    GetChannelDir(szFile, sizeof(szFile), ptDevInfo);
    strcat(szFile, "/warmstart.dat");

    if ( (NULL == (pvFile = OS_FileOpen(szFile, &ulFileLen))) ||
         (ulFileLen < sizeof(CIFX_WS_FILEHEADER)) )
    {
      if (g_ulTraceLevel & TRACE_LEVEL_INFO)
      {
        USER_Trace(ptDevInfo->ptDeviceInstance,
                    TRACE_LEVEL_INFO,
                    "No warm start file available!");
      }
    } else
    {
      CIFX_WS_FILEHEADER tHeader = {0};

      /* Read file header */
      if (OS_FileRead( pvFile, 0, sizeof(tHeader), &tHeader) != sizeof(tHeader))
      {
        if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace(ptDevInfo->ptDeviceInstance,
                      TRACE_LEVEL_ERROR,
                      "Error reading from warm start file!");
        }
      } else if( tHeader.ulCookie != CIFX_WS_WARMSTART_FILE_COOKIE)
      {
        if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace(ptDevInfo->ptDeviceInstance,
                     TRACE_LEVEL_ERROR,
                     "Invalid warm start file cookie!");
        }
      } else if( tHeader.ulDataLen > sizeof(*ptPacket))
      {
        if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace(ptDevInfo->ptDeviceInstance,
                     TRACE_LEVEL_ERROR,
                     "Invalid warm start file length!");
        }
      /* Read file data */
      } else if (OS_FileRead( pvFile, sizeof(tHeader), tHeader.ulDataLen, (void*)ptPacket) != tHeader.ulDataLen)
      {
        if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
        {
          USER_Trace(ptDevInfo->ptDeviceInstance,
                     TRACE_LEVEL_ERROR,
                     "Error reading user data from warm start file!");
        }
      } else
      {
        fRet = 1;
      }
      OS_FileClose(pvFile);
    }
  }

  return fRet;
}

/*****************************************************************************/
/*! User trace function
*   right while cifXTKitAddDevice is being processed
*   \param ptDevInstance  Device instance
*   \param ulTraceLevel   Trace level
*   \param szFormat       Format string                                      */
/*****************************************************************************/
void USER_Trace(PDEVICEINSTANCE ptDevInstance, uint32_t ulTraceLevel, const char* szFormat, ...)
{
  va_list vaList;

  if(g_ulTraceLevel & ulTraceLevel)
  {
    printf("cifXDrv: ");

    va_start(vaList, szFormat);
    vprintf(szFormat, vaList);
    va_end(vaList);

    printf("\n");
  }

  UNREFERENCED_PARAMETER(ptDevInstance);
}

/*****************************************************************************/
/*! Check if interrupts should be enabled for this device
*   \param ptDevInfo  Device Infotmation
*   \return !=0 to enable interrupts                                         */
/*****************************************************************************/
int USER_GetInterruptEnable(PCIFX_DEVICE_INFORMATION ptDevInfo)
{
  char  szFile[CIFX_MAX_FILE_NAME_LENGTH];
  char* szTempIrq    = NULL;
  int   iRet         = 0;

  GetDeviceDir(szFile, sizeof(szFile), ptDevInfo);
  strcat(szFile, "/device.conf");

  /* Read IRQ enable from file */
  if (GetDeviceConfigString(szFile, DEVICE_CONF_IRQ_KEY, &szTempIrq))
  {
    if (0 == OS_Strnicmp("yes", szTempIrq, OS_Strlen("yes")))
    {
      iRet = 1;
    }
    OS_Memfree(szTempIrq);
  }

  if (g_ulTraceLevel & TRACE_LEVEL_INFO)
  {
    if (iRet)
      USER_Trace(ptDevInfo->ptDeviceInstance, TRACE_LEVEL_INFO, "Using interrupt mode");
    else
      USER_Trace(ptDevInfo->ptDeviceInstance, TRACE_LEVEL_INFO, "Using polling mode");
  }

  return iRet;
}

/*****************************************************************************/
/*! Check for DMA enable
*   \param ptDevInfo  Device Infotmation
*   \return eDMA_MODE_LEAVE, eDMA_MODE_ON, eDMA_MODE_OFF                     */
/*****************************************************************************/
#ifdef CIFX_TOOLKIT_DMA
int USER_GetDMAMode (PCIFX_DEVICE_INFORMATION ptDevInfo)
{
  char  szFile[CIFX_MAX_FILE_NAME_LENGTH];
  char* szTempDMA    = NULL;
  int   iRet         = eDMA_MODE_OFF;

  GetDeviceDir(szFile, sizeof(szFile), ptDevInfo);
  strcat(szFile, "/device.conf");

  /* Read DMA enable from file */
  if (GetDeviceConfigString(szFile, DEVICE_CONF_DMA_KEY, &szTempDMA))
  {
    if (0 == OS_Strnicmp("leave", szTempDMA, OS_Strlen("leave")))
      iRet = eDMA_MODE_LEAVE;
    else if (0 == OS_Strnicmp("on", szTempDMA, OS_Strlen("on")))
      iRet = eDMA_MODE_ON;
    else if (0 == OS_Strnicmp("off", szTempDMA, OS_Strlen("off")))
      iRet = eDMA_MODE_OFF;

    OS_Memfree(szTempDMA);
  }

  if (g_ulTraceLevel & TRACE_LEVEL_INFO)
  {
    switch (iRet)
    {
      case eDMA_MODE_LEAVE:
        USER_Trace(ptDevInfo->ptDeviceInstance, TRACE_LEVEL_INFO, "DMA mode leave");
        break;
      case eDMA_MODE_ON:
        USER_Trace(ptDevInfo->ptDeviceInstance, TRACE_LEVEL_INFO, "DMA mode on");
        break;
      case eDMA_MODE_OFF:
        USER_Trace(ptDevInfo->ptDeviceInstance, TRACE_LEVEL_INFO, "DMA mode off");
        break;
    }
  }

  return iRet;
}
#endif

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

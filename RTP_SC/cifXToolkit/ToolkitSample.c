/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: ToolkitSample.c 14195 2021-09-02 12:24:48Z AMinor $:

  Description:
    Toolkit sample implementation

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2021-09-01  Add handling for HIFDPM
    2018-08-09  Fixed pc-lint warnings
    2018-07-24  Changed header and fixed pc-lint warnings
    2014-07-23  Updated SerDPMIfWrite/SerDPMRead function parameters
                according to cifXToolkit (PFN_HWIF_MEMCPY) parameters
    2011-10-03  Initial version derived from windows example

**************************************************************************************/

/* Standard includes */
#include <stdio.h>
#include <unistd.h>

/* Toolkit includes */
#include "cifXToolkit.h"
#include "CIFXErrors.h"
#include "SerialDPM/SerialDPMInterface.h"
#include "ToolkitSample.h"

#define DPM_TEST_CYCLES     500

#define PERF_TESTCASE_READ  0
#define PERF_TESTCASE_WRITE 1
#define PERF_TESTCASE_RW    2

extern void* hifdpm_read  ( void* pvDevInstance, void* pvAddr, void* pvData, uint32_t ulLen );
extern void* hifdpm_write ( void* pvDevInstance, void* pvAddr, void* pvData, uint32_t ulLen );

extern uint32_t g_ulTraceLevel;

static uint8_t s_pabData[3584];

/*****************************************************************************/
/*! Displays a hex dump on the debug console (16 bytes per line)
*   \param pbData     Pointer to dump data
*   \param ulDataLen  Length of data dump                                    */
/*****************************************************************************/
static void DumpData(uint8_t* pbData, uint32_t ulDataLen)
{
  uint32_t ulIdx = 0;
  for(ulIdx = 0; ulIdx < ulDataLen; ++ulIdx)
  {
    if(0 == (ulIdx % 16))
      printf("\r\n");

    printf("%02X ", pbData[ulIdx]);
  }
  printf("\r\n");
}

/*****************************************************************************/
/*! Dumps a rcX packet to debug console
*   \param ptPacket Pointer to packed being dumped                           */
/*****************************************************************************/
static void DumpPacket(CIFX_PACKET* ptPacket)
{
  printf("Dest   : 0x%08lX      ID   : 0x%08lX\r\n", ptPacket->tHeader.ulDest,   ptPacket->tHeader.ulId);
  printf("Src    : 0x%08lX      Sta  : 0x%08lX\r\n", ptPacket->tHeader.ulSrc,    ptPacket->tHeader.ulState);
  printf("DestID : 0x%08lX      Cmd  : 0x%08lX\r\n", ptPacket->tHeader.ulDestId, ptPacket->tHeader.ulCmd);
  printf("SrcID  : 0x%08lX      Ext  : 0x%08lX\r\n", ptPacket->tHeader.ulSrcId,  ptPacket->tHeader.ulExt);
  printf("Len    : 0x%08lX      Rout : 0x%08lX\r\n", ptPacket->tHeader.ulLen,    ptPacket->tHeader.ulRout);

  printf("Data:");
  DumpData(ptPacket->abData, ptPacket->tHeader.ulLen);
}

/*****************************************************************************/
/*! Function to demonstrate the board/channel enumeration
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t EnumBoardDemo(void)
{
  CIFXHANDLE hDriver = NULL;
  int32_t lRet    = xDriverOpen(&hDriver);

  printf("\r\n---------- Board/Channel enumeration demo ----------\r\n");

  if(CIFX_NO_ERROR == lRet)
  {
    /* Driver/Toolkit successfully opened */
    uint32_t          ulBoard    = 0;
    BOARD_INFORMATION tBoardInfo = {0};

    /* Iterate over all boards */
    while(CIFX_NO_ERROR == xDriverEnumBoards(hDriver, ulBoard, sizeof(tBoardInfo), &tBoardInfo))
    {
      printf("Found Board %s\r\n", tBoardInfo.abBoardName);
      if(strlen( (char*)tBoardInfo.abBoardAlias) != 0)
        printf(" Alias        : %s\r\n", tBoardInfo.abBoardAlias);

      printf(" DeviceNumber : %lu\r\n", tBoardInfo.tSystemInfo.ulDeviceNumber);
      printf(" SerialNumber : %lu\r\n", tBoardInfo.tSystemInfo.ulSerialNumber);
      printf(" Board ID     : %lu\r\n", tBoardInfo.ulBoardID);
      printf(" System Error : 0x%08lX\r\n", tBoardInfo.ulSystemError);
      printf(" Channels     : %lu\r\n", tBoardInfo.ulChannelCnt);
      printf(" DPM Size     : %lu\r\n", tBoardInfo.ulDpmTotalSize);

      uint32_t            ulChannel    = 0;
      CHANNEL_INFORMATION tChannelInfo = {{0}};

      /* iterate over all channels on the current board */
      while(CIFX_NO_ERROR == xDriverEnumChannels(hDriver, ulBoard, ulChannel, sizeof(tChannelInfo), &tChannelInfo))
      {
        printf(" - Channel %lu:\r\n", ulChannel);
        printf("    Firmware : %s\r\n", tChannelInfo.abFWName);
        printf("    Version  : %u.%u.%u build %u\r\n",
               tChannelInfo.usFWMajor,
               tChannelInfo.usFWMinor,
               tChannelInfo.usFWRevision,
               tChannelInfo.usFWBuild);
        printf("    Date     : %02u/%02u/%04u\r\n",
               tChannelInfo.bFWMonth,
               tChannelInfo.bFWDay,
               tChannelInfo.usFWYear);

        ++ulChannel;
      }

      ++ulBoard;
    }

    /* close previously opened driver */
    (void)xDriverClose(hDriver);
  }

  printf(" State = 0x%08lX\r\n", lRet);
  printf("----------------------------------------------------\r\n");

  return lRet;
}

/*****************************************************************************/
/*! Function to demonstrate system channel functionality (PacketTransfer)
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t SysdeviceDemo()
{
  CIFXHANDLE hDriver = NULL;
  int32_t    lRet    = xDriverOpen(&hDriver);

  printf("\r\n---------- System Device handling demo ----------\r\n");

  if(CIFX_NO_ERROR == lRet)
  {
    /* Driver/Toolkit successfully opened */
    CIFXHANDLE hSys = NULL;
    lRet = xSysdeviceOpen(hDriver, "cifX0", &hSys);

    if(CIFX_NO_ERROR != lRet)
    {
      printf("Error opening SystemDevice!\r\n");

    } else
    {
      SYSTEM_CHANNEL_SYSTEM_INFO_BLOCK tSysInfo = {{0}};

      /* System channel successfully opened, try to read the System Info Block */
      if( CIFX_NO_ERROR != (lRet = xSysdeviceInfo(hSys, CIFX_INFO_CMD_SYSTEM_INFO_BLOCK, sizeof(tSysInfo), &tSysInfo)))
      {
        printf("Error querying system information block\r\n");
      } else
      {
        printf("System Channel Info Block:\r\n");
        printf("DPM Size         : %lu\r\n", tSysInfo.ulDpmTotalSize);
        printf("Device Number    : %lu\r\n", tSysInfo.ulDeviceNumber);
        printf("Serial Number    : %lu\r\n", tSysInfo.ulSerialNumber);
        printf("Manufacturer     : %u\r\n", tSysInfo.usManufacturer);
        printf("Production Date  : %u\r\n", tSysInfo.usProductionDate);
        printf("Device Class     : %u\r\n", tSysInfo.usDeviceClass);
        printf("HW Revision      : %u\r\n", tSysInfo.bHwRevision);
        printf("HW Compatibility : %u\r\n", tSysInfo.bHwCompatibility);
      }

      uint32_t ulSendPktCount = 0;
      uint32_t ulRecvPktCount = 0;

      /* Do a simple Packet exchange via system channel */
      (void)xSysdeviceGetMBXState(hSys, &ulRecvPktCount, &ulSendPktCount);
      printf("System Mailbox State: MaxSend = %lu, Pending Receive = %lu\r\n",
             ulSendPktCount, ulRecvPktCount);

      CIFX_PACKET tSendPkt = {{0}};
      CIFX_PACKET tRecvPkt = {{0}};

      if(CIFX_NO_ERROR != (lRet = xSysdevicePutPacket(hSys, &tSendPkt, 10)))
      {
        printf("Error sending packet to device!\r\n");
      } else
      {
        printf("Send Packet:\r\n");
        DumpPacket(&tSendPkt);

        (void)xSysdeviceGetMBXState(hSys, &ulRecvPktCount, &ulSendPktCount);
        printf("System Mailbox State: MaxSend = %lu, Pending Receive = %lu\r\n",
              ulSendPktCount, ulRecvPktCount);

        if(CIFX_NO_ERROR != (lRet = xSysdeviceGetPacket(hSys, sizeof(tRecvPkt), &tRecvPkt, 20)) )
        {
          printf("Error getting packet from device!\r\n");
        } else
        {
          printf("Received Packet:\r\n");
          DumpPacket(&tRecvPkt);

          (void)xSysdeviceGetMBXState(hSys, &ulRecvPktCount, &ulSendPktCount);
          printf("System Mailbox State: MaxSend = %lu, Pending Receive = %lu\r\n",
               ulSendPktCount, ulRecvPktCount);
        }
      }

      if(CIFX_NO_ERROR != (lRet = xSysdeviceReset(hSys, 1000)))
      {
        printf("Error on reset device lRet = 0x%08x!\r\n", (unsigned int)lRet);
      } else

      (void)xSysdeviceClose(hSys);
    }

    (void)xDriverClose(hDriver);
  }

  printf(" State = 0x%08lX\r\n", lRet);
  printf("----------------------------------------------------\r\n");

  return lRet;
}

/*****************************************************************************/
/*! Function to demonstrate communication channel functionality
*   Packet Transfer and I/O Data exchange
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t ChannelDemo()
{
  CIFXHANDLE  hDriver = NULL;
  int32_t     lRet    = xDriverOpen(&hDriver);

  printf("\r\n---------- Communication Channel demo ----------\r\n");

  if(CIFX_NO_ERROR == lRet)
  {
    /* Driver/Toolkit successfully opened */
    CIFXHANDLE hChannel = NULL;
    lRet = xChannelOpen(NULL, "cifX0", 0, &hChannel);

    if(CIFX_NO_ERROR != lRet)
    {
      printf("Error opening Channel!");

    } else
    {
      CHANNEL_INFORMATION tChannelInfo = {{0}};
      uint32_t ulCycle = 0;

      /* Channel successfully opened, so query basic information */
      if( CIFX_NO_ERROR != (lRet = xChannelInfo(hChannel, sizeof(CHANNEL_INFORMATION), &tChannelInfo)))
      {
        printf("Error querying system information block\r\n");
      } else
      {
        printf("Communication Channel Info:\r\n");
        printf("Device Number    : %lu\r\n", tChannelInfo.ulDeviceNumber);
        printf("Serial Number    : %lu\r\n", tChannelInfo.ulSerialNumber);
        printf("Firmware         : %s\r\n", tChannelInfo.abFWName);
        printf("FW Version       : %u.%u.%u build %u\r\n",
                tChannelInfo.usFWMajor,
                tChannelInfo.usFWMinor,
                tChannelInfo.usFWRevision,
                tChannelInfo.usFWBuild);
        printf("FW Date          : %02u/%02u/%04u\r\n",
                tChannelInfo.bFWMonth,
                tChannelInfo.bFWDay,
                tChannelInfo.usFWYear);

        printf("Mailbox Size     : %lu\r\n", tChannelInfo.ulMailboxSize);
      }

      CIFX_PACKET tSendPkt = {{0}};
      CIFX_PACKET tRecvPkt = {{0}};

      /* Do a basic Packet Transfer */
      if(CIFX_NO_ERROR != (lRet = xChannelPutPacket(hChannel, &tSendPkt, 10)))
      {
        printf("Error sending packet to device!\r\n");
      } else
      {
        printf("Send Packet:\r\n");
        DumpPacket(&tSendPkt);

        if(CIFX_NO_ERROR != (lRet = xChannelGetPacket(hChannel, sizeof(tRecvPkt), &tRecvPkt, 20)) )
        {
          printf("Error getting packet from device!\r\n");
        } else
        {
          printf("Received Packet:\r\n");
          DumpPacket(&tRecvPkt);
        }
      }

      /* Read and write I/O data (32Bytes). Output data will be incremented each cyle */
      uint8_t abSendData[32] = {0};
      uint8_t abRecvData[32] = {0};

      for( ulCycle = 0; ulCycle < 10; ++ulCycle)
      {
        if(CIFX_NO_ERROR != (lRet = xChannelIORead(hChannel, 0, 0, sizeof(abRecvData), abRecvData, 10)))
        {
          printf("Error reading IO Data area!\r\n");
          break;
        } else
        {
          printf("IORead Data:");
          DumpData(abRecvData, sizeof(abRecvData));

          if(CIFX_NO_ERROR != (lRet = xChannelIOWrite(hChannel, 0, 0, sizeof(abSendData), abSendData, 10)))
          {
            printf("Error writing to IO Data area!\r\n");
            break;
          } else
          {
            printf("IOWrite Data:");
            DumpData(abSendData, sizeof(abSendData));
            memset(abSendData, ulCycle + 1, sizeof(abSendData));
          }
        }
      }

      (void)xChannelClose(hChannel);
    }

    (void)xDriverClose(hDriver);
  }

  printf(" State = 0x%08lX\r\n", lRet);
  printf("----------------------------------------------------\r\n");

  return lRet;

}

/*****************************************************************************/
/*! Function to demonstrate control/status block functionality
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t BlockDemo( void)
{
  CIFXHANDLE hDriver = NULL;
  int32_t    lRet    = xDriverOpen(&hDriver);

  printf("\n--- Read / Write Block Information ---\r\n");

  if(CIFX_NO_ERROR == lRet)
  {
    /* Driver/Toolkit successfully opened */
    /* Open channel */
    CIFXHANDLE hDevice = NULL;
    lRet = xChannelOpen(NULL, "cifX0", 0, &hDevice);
    if(lRet != CIFX_NO_ERROR)
    {
      printf("Error opening Channel!\r\n");
    } else
    {
      uint8_t abBuffer[4] = {0};

      /* Read / Write control block */
      printf("Read CONTROL Block \r\n");
      memset( abBuffer, 0, sizeof(abBuffer));
      lRet = xChannelControlBlock( hDevice, CIFX_CMD_READ_DATA, 0, 4, &abBuffer[0]);

      DumpData(abBuffer, 4);

      printf("Write CONTROL Block \r\n");
      lRet = xChannelControlBlock( hDevice, CIFX_CMD_WRITE_DATA, 0, 4, &abBuffer[0]);

      printf("Read COMMON Status Block \r\n");
      memset( abBuffer, 0, sizeof(abBuffer));
      lRet = xChannelCommonStatusBlock( hDevice, CIFX_CMD_READ_DATA, 0, 4, &abBuffer[0]);

      DumpData(abBuffer, 4);

      printf("Write COMMON Status Block \r\n");
      lRet = xChannelCommonStatusBlock( hDevice, CIFX_CMD_WRITE_DATA, 0, 4, &abBuffer[0]);

      /* this is expected to fail, as this block must not be written by Host */
      if(CIFX_NO_ERROR != lRet)
        printf("Error writing to common status block. lRet = 0x%08lx\r\n", lRet);

      printf("Read EXTENDED Status Block \r\n");
      memset( abBuffer, 0, sizeof(abBuffer));
      lRet = xChannelExtendedStatusBlock( hDevice, CIFX_CMD_READ_DATA, 0, 4, &abBuffer[0]);
      DumpData(abBuffer, 4);

      printf("Write EXTENDED Status Block \r\n");
      lRet = xChannelExtendedStatusBlock( hDevice, CIFX_CMD_WRITE_DATA, 0, 4, &abBuffer[0]);

      /* this is expected to fail, as this block must not be written by Host */
      if(CIFX_NO_ERROR != lRet)
        printf("Error writing to extended status block. lRet = 0x%08lx\r\n", lRet);

      (void)xChannelClose(hDevice);
    }

    (void)xDriverClose(hDriver);
  }

  return lRet;
}

/*****************************************************************************/
/*! IO Mirror demo
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
static int32_t IOMirrorDemo(void)
{
  uint8_t     abRecvData[240] = {0};
  CIFXHANDLE  hChannel        = NULL;
  CIFXHANDLE  hDriver         = NULL;
  int32_t     lRet            = CIFX_NO_ERROR;

  printf("\n--- IO Mirror Demo ---\r\n");

  if ( (CIFX_NO_ERROR != (lRet = xDriverOpen(&hDriver))) ||
       (CIFX_NO_ERROR != (lRet = xChannelOpen(NULL, "cifX0", 0, &hChannel))) )
  {
    printf("Error opening Channel! (lRet = 0x%08lx)\r\n", lRet);

  } else
  {
    uint32_t ulCycle    = 0;
    uint32_t ulErrorCnt = 0;

    while (1)
    {
      if(CIFX_NO_ERROR != (lRet = xChannelIORead(hChannel, 0, 0, sizeof(abRecvData), abRecvData, 10)))
      {
        ulErrorCnt++;
      } else if(CIFX_NO_ERROR != (lRet = xChannelIOWrite(hChannel, 0, 0, sizeof(abRecvData), abRecvData, 10)))
      {
        ulErrorCnt++;
      }

      if (++ulCycle % 500 == 0)
      {
        printf("%lu cycles passed with %lu error(s)!\r", ulCycle, ulErrorCnt);
        (void)fflush(stdout);
      }
    }
  }

  return lRet;
}


/*****************************************************************************/
/*! Function to measure performance of I/O data exchange
*   \param fTestcase Read/Write/Readwrite performance test
*   \param fCompact  TRUE for compact display of results
*   \return CIFX_NO_ERROR on success                                         */
/*****************************************************************************/
int32_t IOPerformance(int fTestcase, int fCompact)
{
  CIFXHANDLE  hDriver     = NULL;
  int32_t     lRet        = CIFX_NO_ERROR;

  switch(fTestcase)
  {
    case PERF_TESTCASE_READ:
      printf("\n--- I/O Read Performance test ---\r\n");
      break;
    case PERF_TESTCASE_WRITE:
      printf("\n--- I/O Write Performance test ---\r\n");
      break;
    case PERF_TESTCASE_RW:
      printf("\n--- I/O Write/Read Performance test ---\r\n");
      break;
  }

  if (fCompact)
  {
    printf("Size[bytes]\tMin[us]\tAvg[us]\tMax[us]\t[kBit/s]/[kB/s]\r\n");
    printf("-------------------------------------------------------\r\n");
  }

  if(CIFX_NO_ERROR == (lRet = xDriverOpen(&hDriver)))
  {
    /* Driver/Toolkit successfully opened */
    CIFXHANDLE hChannel = NULL;
    lRet = xChannelOpen(NULL, "cifX0", 0, &hChannel);

    if(CIFX_NO_ERROR != lRet)
    {
      printf("Error opening Channel!\r\n");

    } else
    {
      CHANNEL_IO_INFORMATION tIoInformation = {0};
      uint16_t abIOSizeMap[] = {1, 2, 4, 8, 10, 16, 20, 32, 50, 64, 100, 128, 150, 200, 240, 256, 512, 1024, 3584};
      int      iIdx          = 0;
      int      iIOMax        = 0;

      /* Check maximum I/O area size */
      (void)xChannelIOInfo(hChannel, CIFX_IO_INPUT_AREA, 0, sizeof(tIoInformation), &tIoInformation);
      iIOMax = tIoInformation.ulTotalSize;
      (void)xChannelIOInfo(hChannel, CIFX_IO_OUTPUT_AREA, 0, sizeof(tIoInformation), &tIoInformation);
      iIOMax = min( (unsigned long)iIOMax, tIoInformation.ulTotalSize);

      for (iIdx = 0; iIdx < (int)(sizeof(abIOSizeMap)/sizeof(abIOSizeMap[0])); iIdx++)
      {
        uint32_t ulErrors = 0;
        uint32_t ulCycle  = 0;
        uint32_t ulMin    = ~0;
        uint32_t ulMax    = 0;
        uint32_t ulSum    = 0;

        if (abIOSizeMap[iIdx] > iIOMax)
          break;

        for( ulCycle = 0; ulCycle < DPM_TEST_CYCLES; ++ulCycle)
        {
          uint32_t   ulTmp;
          uint32_t   ulTimeStampBegin;
          uint32_t   ulTimeStampEnd;
          uint32_t   ulDiff = 0;

          HAL_GetSystime(&ulTmp, &ulTimeStampBegin, NULL);

          switch(fTestcase)
          {
            case PERF_TESTCASE_READ:
              lRet = xChannelIORead(hChannel, 0, 0, abIOSizeMap[iIdx], s_pabData, 10);
              break;

            case PERF_TESTCASE_WRITE:
              lRet = xChannelIOWrite(hChannel, 0, 0, abIOSizeMap[iIdx], s_pabData, 10);
              break;

            case PERF_TESTCASE_RW:
              lRet  = xChannelIOWrite(hChannel, 0, 0, abIOSizeMap[iIdx], s_pabData, 10);
              lRet |= xChannelIORead(hChannel, 0, 0, abIOSizeMap[iIdx], s_pabData, 10);
              break;
          }

          HAL_GetSystime(&ulTmp, &ulTimeStampEnd, NULL);

          if (ulTimeStampBegin > ulTimeStampEnd)
            ulDiff = ulTimeStampEnd+(0xFFFFFFFF-ulTimeStampBegin);
          else
            ulDiff = ulTimeStampEnd-ulTimeStampBegin;

          if (lRet != CIFX_NO_ERROR)
          {
            if (++ulErrors > 100)
            {
              printf("I/O Performance test failed with error 0x%08x!\r\n", (unsigned int)lRet);
              return lRet;
            }
            --ulCycle;
            continue;
          }

          ulMin = min(ulMin, ulDiff);
          ulMax = max(ulMax, ulDiff);
          ulSum+= ulDiff;
        }

        if (fCompact)
        {
          uint32_t ulAvgThp = 0;
          uint64_t ulDiv    = 1024 * ((uint64_t)ulSum / DPM_TEST_CYCLES);
          if(  ulDiv != 0)
          {
            ulAvgThp = (uint64_t)abIOSizeMap[iIdx] * 1000 * 1000 * 1000 / ulDiv;
            printf("%d\t\t%ld\t%ld\t%ld\t%4ld/%4ld\r\n",abIOSizeMap[iIdx], ulMin/1000, (ulSum/DPM_TEST_CYCLES)/1000, ulMax/1000, 8 * ulAvgThp, ulAvgThp);
          }

        } else
        {
          uint64_t ulAvgThp = 0;
          uint64_t ulDiv    = 1024 * ((uint64_t)ulSum / DPM_TEST_CYCLES);
          if(  ulDiv != 0)
          {
            ulAvgThp = (uint64_t)abIOSizeMap[iIdx] * 1000 * 1000 * 1000 / ulDiv;
          }

          printf("-------------\r\n");
          printf(" Cycles                 : %u\r\n", DPM_TEST_CYCLES);
          printf(" Bytes per Cycle        : %u\r\n", abIOSizeMap[iIdx]);
          printf(" Total Time             : %ld ms\r\n", ulSum / 1000);
          printf(" I/O Time (min/avg/max) : %ld/%ld/%ld us\r\n", ulMin/1000, (ulSum/DPM_TEST_CYCLES)/1000, ulMax/1000);
          printf(" Avg. Throughput        : %lld kB/s\r\n", (uint64_t)ulAvgThp);
          printf("-------------\r\n\r\n");
        }
      }
      (void)xChannelClose(hChannel);
    }

    (void)xDriverClose(hDriver);
  }

  printf("----------------------------------------------------\r\n");
  return lRet;
}

/*****************************************************************************/
/*! Toolkit initialization function
 *  \param  ulType  DPM type to use (SAMPLE_TYPE_SPM, SAMPLE_TYPE_DPM)
 *  \param  pbDPM   Pointer to start of DPM                                  */
/*****************************************************************************/
void ToolkitSample(uint32_t ulType, uint8_t* pbDPM)
{
  int32_t lTkRet = CIFX_NO_ERROR;

  /* Adjust border of system timer */
  //HAL_SetSystimeBorder(0xFFFFFFFF, NULL);

  /* First of all initialize toolkit */
  lTkRet = cifXTKitInit();

  if(CIFX_NO_ERROR == lTkRet)
  {
    NONOS_DEVICEINSTANCE_T* ptDevInstance = (NONOS_DEVICEINSTANCE_T*)OS_Memalloc(sizeof(*ptDevInstance));
    unsigned int            ulSerDPMType;

    if( NULL == ptDevInstance)
      lTkRet = CIFX_INVALID_POINTER;
    else
    {
      /* Set trace level of toolkit */
      g_ulTraceLevel = TRACE_LEVEL_ERROR   |
                       TRACE_LEVEL_WARNING |
                       TRACE_LEVEL_INFO    |
                       TRACE_LEVEL_DEBUG;

      OS_Memset(ptDevInstance, 0, sizeof(*ptDevInstance));

      ptDevInstance->tDevice.pvOSDependent = ptDevInstance;
      ptDevInstance->tDevice.ulDPMSize     = 0x10000;
      ptDevInstance->tDevice.pbDPM         = pbDPM;
      (void)OS_Strncpy(ptDevInstance->tDevice.szName, "cifX0", sizeof(ptDevInstance->tDevice.szName));

      if (SAMPLE_TYPE_SPM == ulType)
      {
        if (SERDPM_UNKNOWN == (ulSerDPMType = SerialDPM_Init(&ptDevInstance->tDevice)))
        {
          printf("Serial DPM protocol could not be recognized!)\r\n");
          lTkRet = CIFX_FUNCTION_FAILED;
        } else
        {
          char* szSerDPM[] = {"", "NETX10", "NETX50", "NETX51", "NETX100"};
          printf("Serial DPM protocol detected: %s\r\n", szSerDPM[ulSerDPMType]);
        }
      } else
      {
        /* Usually we don't need to set the HWIF functions for HIFDPM, but the Toolkit was
         * compiled with CIFX_TOOLKIT_HWIF enabled to support the serial DPM interface.
         * In this case the HWIF read/write functions have to be set for HIFDPM, too. */
        ptDevInstance->tDevice.pfnHwIfRead  = hifdpm_read;
        ptDevInstance->tDevice.pfnHwIfWrite = hifdpm_write;
      }

      if (CIFX_NO_ERROR == lTkRet)
      {
        /* Add the device to the toolkits handled device list */
        lTkRet = cifXTKitAddDevice(&ptDevInstance->tDevice);

        /* If it succeeded do device tests */
        if(CIFX_NO_ERROR != lTkRet)
        {
            printf("cifXTKitAddDevice failed");
          /* Uninitialize Toolkit, this will remove all handled boards from the toolkit and
             deallocate the device instance */
          cifXTKitDeinit();
        } else
        {
          /* Toolkit demonstration */
          (void)EnumBoardDemo();
          (void)SysdeviceDemo();
          (void)ChannelDemo();
          (void)BlockDemo();
          (void)IOPerformance(PERF_TESTCASE_READ,  1);
          (void)IOPerformance(PERF_TESTCASE_WRITE, 1);
          (void)IOPerformance(PERF_TESTCASE_RW,    1);
          (void)IOMirrorDemo();
        }
      }

      OS_Memfree(ptDevInstance);
    }
  }
} /*lint !e438: Last value assigned to lTkRet not used */

//Main commented to avoid compilation issues, ToolkitSample() can be executed from RTP if needed
/*int main()
{
	ToolkitSample(SAMPLE_TYPE_SPM, NULL);
}*/

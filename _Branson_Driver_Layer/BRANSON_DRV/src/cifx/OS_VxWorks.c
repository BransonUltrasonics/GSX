/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: OS_VxWorks.c 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    VxWorks "Abstraction Layer" implementation

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2017-10-12  FL, fixed build error
    2015-06-08  created from VxWorks 6x driver version 5
                - Adapated OS_ReadPCIConfig()/OS_WritePCIConfig() to VxWorks 7
                - Adapated OS_EnableInterrupts()/OS_DisableInterrupts() to VxWorks 7

**************************************************************************************/

/*****************************************************************************/
/*! \file OS_xWorks.c
*    VxWorks OS Abstraction Layer implementation                             */
/*****************************************************************************/

#include "cifXToolkit/OS_Dependent.h"
#include "OS_Includes.h"
#include "cifXVxWorks_internal.h"

/*****************************************************************************/
/*!  \addtogroup CIFX_TK_OS_ABSTRACTION Operating System Abstraction
*    \{                                                                      */
/*****************************************************************************/

uint64_t  g_ullUnalignedMemcpyCnt = 0;

typedef struct VXWMEMORY_INFOMATION_Ttag
{
  void*         pvMemID;
  unsigned long ulMemSize;

} VXWMEMORY_INFOMATION_T;

/*****************************************************************************/
/*! Memory allocation function
*   \param ulSize    Length of memory to allocate
*   \return Pointer to allocated memory                                      */
/*****************************************************************************/
void* OS_Memalloc(uint32_t ulSize)
{
  return malloc(ulSize);
}

/*****************************************************************************/
/*! Memory freeing function
*   \param pvMem Memory block to free                                        */
/*****************************************************************************/
void OS_Memfree(void* pvMem)
{
  free(pvMem);
}

/*****************************************************************************/
/*! Memory reallocating function (used for resizing dynamic toolkit arrays)
*   \param pvMem     Memory block to resize
*   \param ulNewSize new size of the memory block
*   \return pointer to the resized memory block                              */
/*****************************************************************************/
void* OS_Memrealloc(void* pvMem, uint32_t ulNewSize)
{
  return realloc(pvMem, ulNewSize);
}

/*****************************************************************************/
/*! Memory setting
*   \param pvMem     Memory block
*   \param bFill     Byte to use for memory initialization
*   \param ulSize    Memory size for initialization)                         */
/*****************************************************************************/
void OS_Memset(void* pvMem, unsigned char bFill, uint32_t ulSize)
{
  memset(pvMem, bFill, ulSize);
}

/*****************************************************************************/
/*! Copy memory from one block to another
*   \param pvDest    Destination memory block
*   \param pvSrc     Source memory block
*   \param ulSize    Copy size in bytes                                      */
/*****************************************************************************/
void OS_Memcpy(void* pvDest, void* pvSrc, uint32_t ulSize)
{
  uint32_t ulDestAlignment   = (uint32_t)((uint64_t)pvDest & 0x03);
  uint32_t ulSourceAlignment = (uint32_t)((uint64_t)pvSrc  & 0x03);

  /* Pericom bridges generate a READ_LINE or READ_MULTIPLE command on PCI if accesses > 32
   bits are performed. This happens internally in most memcpy routines, so we need a memcpy
   routine that only performs 32bit accesses and must hope that the compiler does not optimize
   it */
  if( (0 == ulDestAlignment)   &&
      (0 == ulSourceAlignment) &&
      (ulSize >= sizeof(uint32_t)) )
  {
    uint32_t* pulDest = (uint32_t*)pvDest;
    uint32_t* pulSrc  = (uint32_t*)pvSrc;

    do
    {
      *pulDest++ = *pulSrc++;
      ulSize   -= (uint32_t)sizeof(uint32_t);

    } while(ulSize >= sizeof(uint32_t));

    pvSrc  = pulSrc;
    pvDest = pulDest;
  }

  /* If Destination or Source is unaligned, we need to perform a byte
   copy, so that no PCI Bridge (like pericom) will perform a memory read line command
   which does not work on netX */
  {
    uint8_t* pbDest = (uint8_t*)pvDest;
    uint8_t* pbSrc  = (uint8_t*)pvSrc;

    while(ulSize-- > 0)
    {
      *pbDest++ = *pbSrc++;
    }
  }

  if( (0 != ulDestAlignment)   ||
      (0 != ulSourceAlignment) )
  {
    ++g_ullUnalignedMemcpyCnt;
  }
}

/*****************************************************************************/
/*! Compare two memory blocks
*   \param pvBuf1    First memory block
*   \param pvBuf2    Second memory block
*   \param ulSize    Compare size in bytes
*   \return 0 if both buffers are equal                                      */
/*****************************************************************************/
int OS_Memcmp(void* pvBuf1, void* pvBuf2, uint32_t ulSize)
{
  return memcmp(pvBuf1, pvBuf2, ulSize);
}

/*****************************************************************************/
/*! Move memory
*   \param pvDest    Destination memory
*   \param pvSrc     Source memory
*   \param ulSize    Size in byte to move                                    */
/*****************************************************************************/
void OS_Memmove(void* pvDest, void* pvSrc, uint32_t ulSize)
{
  memmove(pvDest, pvSrc, ulSize);
}

/*****************************************************************************/
/*! Sleep for a specific time
*   \param ulSleepTimeMs  Time in ms to sleep for                            */
/*****************************************************************************/
void OS_Sleep(uint32_t ulSleepTimeMs)
{
  int iTicks = ulSleepTimeMs * sysClkRateGet() / 1000;

  taskDelay(iTicks);
}

/*****************************************************************************/
/*! Retrieve a counter based on millisecond used for timeout monitoring
*   \return Current counter value (resolution of this value will influence
*           timeout monitoring in driver/toolkit functions(                  */
/*****************************************************************************/
uint32_t OS_GetMilliSecCounter(void)
{
  uint32_t ulTicksPerSec = 0;
  uint32_t ulTickCount   = 0;

  /* Get ticks per millisecond */
  ulTicksPerSec = (uint32_t)sysClkRateGet();
  /* Read actual tick count */
  ulTickCount   = (uint32_t)tickGet();

  return ((uint32_t)((ulTickCount * 1000)/ulTicksPerSec));
}

/*****************************************************************************/
/*! Create an auto reset event
*   \return handle to the created event                                      */
/*****************************************************************************/
void* OS_CreateEvent(void)
{
  SEM_ID hSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY );

  return hSemaphore;
}

/*****************************************************************************/
/*! Set an event
*   \param pvEvent Handle to event being signalled                           */
/*****************************************************************************/
void OS_SetEvent(void* pvEvent)
{
  SEM_ID hSemaphore = (SEM_ID)pvEvent;
  semGive(hSemaphore);
}

/*****************************************************************************/
/*! Reset an event
*   \param pvEvent Handle to event being reset                               */
/*****************************************************************************/
void OS_ResetEvent(void* pvEvent)
{
  OS_WaitEvent(pvEvent, 0);
}

/*****************************************************************************/
/*! Delete an event
*   \param pvEvent Handle to event being deleted                             */
/*****************************************************************************/
void OS_DeleteEvent(void* pvEvent)
{
  SEM_ID hSemaphore = (SEM_ID)pvEvent;

  semDelete(hSemaphore);
}

/*****************************************************************************/
/*! Wait for the signalling of an event
*   \param pvEvent   Handle to event being wait for
*   \param ulTimeout Timeout in ms to wait for event
*   \return 0 if event was signalled                                         */
/*****************************************************************************/
uint32_t OS_WaitEvent(void* pvEvent, uint32_t ulTimeout)
{
  SEM_ID   hSemaphore = (SEM_ID)pvEvent;
  uint32_t ulRet      = CIFX_EVENT_TIMEOUT;

  STATUS status;
  if (WAIT_FOREVER == ulTimeout)
  {
    status = semTake(hSemaphore, WAIT_FOREVER);
  } else
  {
    int iTimeout = ulTimeout * sysClkRateGet() / 1000;
    status = semTake(hSemaphore, iTimeout);
  }

  if(OK == status)
  {
    ulRet = CIFX_EVENT_SIGNALLED;
  }

  return ulRet;
}

/*****************************************************************************/
/*! Compare two ASCII string
*   \param pszBuf1   First buffer
*   \param pszBuf2   Second buffer
*   \return 0 if strings are equal                                           */
/*****************************************************************************/
int OS_Strcmp(const char* pszBuf1, const char* pszBuf2)
{
  return strcmp(pszBuf1, pszBuf2);
}

/*****************************************************************************/
/*! Compare characters of two strings without regard to case
*   \param pszBuf1   First buffer
*   \param pszBuf2   Second buffer
*   \param ulLen     Number of characters to compare
*   \return 0 if strings are equal                                           */
/*****************************************************************************/
int OS_Strnicmp(const char* pszBuf1, const char* pszBuf2, uint32_t ulLen)
{
  return strncasecmp(pszBuf1, pszBuf2, ulLen);
}

/*****************************************************************************/
/*! Query the length of an ASCII string
*   \param szText    ASCII string
*   \return character count of szText                                        */
/*****************************************************************************/
int OS_Strlen(const char* szText)
{
  return (int)strlen(szText);
}

/*****************************************************************************/
/*! Copy one string to another monitoring the maximum length of the target
*   buffer.
*   \param szDest    Destination buffer
*   \param szSource  Source buffer
*   \param ulLength  Maximum length to copy
*   \return pointer to szDest                                                */
/*****************************************************************************/
char* OS_Strncpy(char* szDest, const char* szSource, uint32_t ulLength)
{
  return strncpy(szDest, szSource, ulLength);
}

/*****************************************************************************/
/*! Create an interrupt safe locking mechanism (Spinlock/critical section)
*   \return handle to the locking object                                     */
/*****************************************************************************/
void* OS_CreateLock(void)
{
  SEM_ID hLock = semMCreate(SEM_Q_PRIORITY     |
                            SEM_INVERSION_SAFE |
                            SEM_DELETE_SAFE);

  return hLock;
}

/*****************************************************************************/
/*! Enter a critical section/spinlock
*   \param pvLock Handle to the locking object                               */
/*****************************************************************************/
void OS_EnterLock(void* pvLock)
{
  SEM_ID hLock = (SEM_ID)pvLock;
  semTake(hLock, WAIT_FOREVER);
}

/*****************************************************************************/
/*! Leave a critical section/spinlock
*   \param pvLock Handle to the locking object                               */
/*****************************************************************************/
void OS_LeaveLock(void* pvLock)
{
  SEM_ID hLock = (SEM_ID)pvLock;
  semGive(hLock);
}

/*****************************************************************************/
/*! Delete a critical section/spinlock object
*   \param pvLock Handle to the locking object being deleted                 */
/*****************************************************************************/
void OS_DeleteLock(void* pvLock)
{
  SEM_ID hLock = (SEM_ID)pvLock;
  semDelete(hLock);
}

/*****************************************************************************/
/*! Create an Mutex object for locking code sections
*   \return handle to the mutex object                                       */
/*****************************************************************************/
void* OS_CreateMutex(void)
{
  SEM_ID hMutex = semMCreate(SEM_Q_PRIORITY     |
                             SEM_INVERSION_SAFE |
                             SEM_DELETE_SAFE);

  return hMutex;
}

/*****************************************************************************/
/*! Wait for mutex
*   \param pvMutex    Handle to the Mutex locking object
*   \param ulTimeout  Wait timeout
*   \return !=0 on succes                                                    */
/*****************************************************************************/
int OS_WaitMutex(void* pvMutex, uint32_t ulTimeout)
{
  SEM_ID hMutex   = (SEM_ID)pvMutex;
  int    iRet     = 0;

  STATUS status;
  if (WAIT_FOREVER == ulTimeout)
  {
    status = semTake(hMutex, WAIT_FOREVER);
  } else
  {
    int iTimeout = ulTimeout * sysClkRateGet() / 1000;
    status = semTake(hMutex, iTimeout);
  }

  if(OK == status)
    iRet = 1;

  return iRet;
}

/*****************************************************************************/
/*! Release a mutex section section
*   \param pvMutex Handle to the locking object                              */
/*****************************************************************************/
void OS_ReleaseMutex(void* pvMutex)
{
  SEM_ID hMutex = (SEM_ID)pvMutex;
  semGive(hMutex);
}

/*****************************************************************************/
/*! Delete a Mutex object
*   \param pvMutex Handle to the mutex object being deleted                  */
/*****************************************************************************/
void OS_DeleteMutex(void* pvMutex)
{
  SEM_ID hMutex = (SEM_ID)pvMutex;
  semDelete(hMutex);
}

/*****************************************************************************/
/*! Opens a file in binary mode
*   \param szFile     Full file name of the file to open
*   \param pulFileLen Returned length of the opened file
*   \return handle to the file, NULL mean file could not be opened           */
/*****************************************************************************/
void* OS_FileOpen(char* szFile, uint32_t* pulFileLen)
{
  FILE* hFile = fopen(szFile, "r");
  if(NULL != hFile)
  {
    fseek(hFile, 0, SEEK_END);
    if (NULL != pulFileLen)
    {
      *pulFileLen = (uint32_t)ftell(hFile);
    }
    fseek(hFile, 0, SEEK_SET);
  }

  return hFile;
}

/*****************************************************************************/
/*! Closes a previously opened file
*   \param pvFile Handle to the file being closed                            */
/*****************************************************************************/
void OS_FileClose(void* pvFile)
{
  FILE* hFile = (FILE*)pvFile;
  fclose(hFile);
}

/*****************************************************************************/
/*! Read a specific amount of bytes from the file
*   \param pvFile   Handle to the file being read from
*   \param ulOffset Offset inside the file, where read starts at
*   \param ulSize   Size in bytes to be read
*   \param pvBuffer Buffer to place read bytes in
*   \return number of bytes actually read from file                          */
/*****************************************************************************/
uint32_t OS_FileRead(void* pvFile, uint32_t ulOffset, uint32_t ulSize, void* pvBuffer)
{
  uint32_t ulRet = 0;
  FILE*    hFile = (FILE*)pvFile;

  if( 0 == fseek(hFile, ulOffset, SEEK_SET))
  {
    ulRet = (uint32_t)fread(pvBuffer, 1, ulSize, hFile);
  }

  return ulRet;
}

/*****************************************************************************/
/*! OS specific initialization (if needed), called during cifXTKitInit()     */
/*****************************************************************************/
int32_t OS_Init()
{
  return 0;
}

/*****************************************************************************/
/*! OS specific de-initialization (if needed), called during cifXTKitInit()  */
/*****************************************************************************/
void OS_Deinit()
{
}

/*****************************************************************************/
/*! This functions is called for PCI cards in the toolkit. It is expected to
* write back all BAR's (Base address registers), Interrupt and Command
* Register. These registers are invalidated during cifX Reset and need to be
* re-written after the reset has succeeded
*   \param pvOSDependent OS Dependent Variable passed during call to
*                        cifXTKitAddDevice
*   \param pvPCIConfig   Configuration returned by OS_ReadPCIConfig
*                        (implementation dependent)                          */
/*****************************************************************************/
void OS_WritePCIConfig(void* pvOSDependent, void* pvPCIConfig)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDev      = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;
  UINT32           ulOffset   = 0;
  UINT32*                    pulBuffer  = (UINT32*)&ptDev->tPciInfo.tPCIConfig;

  /* Read the complete PCI_HEADER_DEVICE structure */
  for ( ulOffset = 0; ulOffset < sizeof (PCI_HEADER_DEVICE); ulOffset += (UINT32)sizeof(UINT32))
  {
    VXB_PCI_BUS_CFG_WRITE (ptDev->tDeviceEntry.pDev, ulOffset,  sizeof(uint32_t), *pulBuffer);
    pulBuffer++;
  }
}

/*****************************************************************************/
/*! This functions is called for PCI cards in the toolkit. It is expected to
* read all BAR's (Base address registers), Interrupt and Command Register.
* These registers are invalidated during cifX Reset and need to be
* re-written after the reset has succeeded
*   \param pvOSDependent OS Dependent Variable passed during call to
*                        cifXTKitAddDevice
*   \return pointer to stored register copies (implementation dependent)     */
/*****************************************************************************/
void* OS_ReadPCIConfig(void* pvOSDependent)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDev        = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;
  UINT32           ulOffset     = 0;
  UINT32*                    pulBuffer    = (UINT32*)&ptDev->tPciInfo.tPCIConfig;

  /* Read the complete PCI_HEADER_DEVICE structure */
  for ( ulOffset = 0; ulOffset < sizeof (PCI_HEADER_DEVICE); ulOffset += (UINT32)sizeof(UINT32))
  {
    VXB_PCI_BUS_CFG_READ (ptDev->tDeviceEntry.pDev, ulOffset,  sizeof(uint32_t), *pulBuffer);
    pulBuffer++;
  }

  return &ptDev->tPciInfo.tPCIConfig;
}

/*****************************************************************************/
/*! This function Maps a DPM pointer to a user application if needed.
*   \param pvDriverMem   Pointer to be mapped
*   \param ulMemSize     Size to be mapped
*   \param ppvMappedMem  Returned mapped pointer (usable by application)
*   \param pvOSDependent OS Dependent variable passed during call to
*                        cifXTKitAddDevice
*   \return Handle that is needed for unmapping NULL is a mapping failure    */
/*****************************************************************************/
void* OS_MapUserPointer(void* pvDriverMem, uint32_t ulMemSize, void** ppvMappedMem, void* pvOSDependent)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDevInst = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;
  void* pvRet = NULL;
  uint64_t ullOffset = 0;

  /* retrieve offset to DPM beginning to map the exact physical memory */
  ullOffset = (uint64_t*)pvDriverMem - (uint64_t*)(ptDevInst->tDeviceEntry.pvDPMAddress);

  pvRet = pmapPrivateMap ( ptDevInst->tDeviceEntry.ulPhysicalAddress + ullOffset,     /* physical address */
                           ulMemSize,                                                 /* number of bytes to map */
                           MMU_ATTR_CACHE_OFF |
                           MMU_ATTR_PROT_SUP_READ | MMU_ATTR_PROT_SUP_WRITE |
                           MMU_ATTR_PROT_USR_READ | MMU_ATTR_PROT_USR_WRITE   );      /* MMU attributes */

  if (PMAP_FAILED == pvRet)
  {
    pvRet = NULL;
  }else
  {
    VXWMEMORY_INFOMATION_T* ptVxMemInfo = (VXWMEMORY_INFOMATION_T*)OS_Memalloc( (uint32_t)sizeof(VXWMEMORY_INFOMATION_T));

    if( NULL != ptVxMemInfo)
    {
      /* return the mapped memory pointer for the application */
      *ppvMappedMem = pvRet;

      /* store information for un-mapping */
      ptVxMemInfo->pvMemID    = pvRet;
      ptVxMemInfo->ulMemSize  = ulMemSize;

      /* return the memory information structure as the memory ID */
      pvRet = ptVxMemInfo;
    }else
    {
      /* Failed to allocate return structure */
      pmapPrivateUnmap ( pvRet,
                         ulMemSize);
      pvRet = NULL;
    }
  }

  return pvRet;
}

/*****************************************************************************/
/*! This function unmaps a previously mapped user application pointer
*   \param pvMemoryInfo   Handle that was returned by OS_MapUserPointer
*   \param pvOSDependent  OS Dependent variable passed during call to
*                         cifXTKitAddDevice
*   \return !=0 on success                                                   */
/*****************************************************************************/
int OS_UnmapUserPointer(void* pvMemoryInfo, void* pvOSDependent)
{
  int iRet = 0;
  UNREFERENCED_PARAMETER(pvOSDependent);

  VXWMEMORY_INFOMATION_T* ptVxMemInfo = pvMemoryInfo;

  if( NULL != ptVxMemInfo)
  {
    if ( ERROR != pmapPrivateUnmap( ptVxMemInfo->pvMemID,      /* base virtual address */
                  ptVxMemInfo->ulMemSize))  /* number of bytes to unmap */
      iRet = 1;

    /* remove the VXMEMORY structure */
    OS_Memfree( ptVxMemInfo);
  }

  return iRet;
}

/*****************************************************************************/
/*! Interrupt Service Thread (IST)
 *   \param pContext    IST Context
 *****************************************************************************/
void OS_InterruptServiceRoutine ( void* pContext)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)pContext;
  int iISRRet                      = CIFX_TKIT_IRQ_OTHERDEVICE;

  /* Handle PLX Interrupts differently */
  if( TRUE == ptDev->tPciInfo.fPLXBridge )
  {
    uint8_t bIntState = ptDev->tPciInfo.pbPLXRegisters[PLX_PCI_INTCTRLSTS_REG];

    if(0 == (bIntState & PLX_HW_INTERRUPT_STATE))
    {
      /* This was not our interrupt */
      return;
    } else
    {
      /* It was our Interrupt, so proceed with processing */
    }
  }

  iISRRet = cifXTKitISRHandler(&ptDev->tDeviceInst, 1);

  switch(iISRRet)
  {
    case CIFX_TKIT_IRQ_DSR_REQUESTED:
      OS_SetEvent(ptDev->hDeferredServiceEvent);
      break;

    case CIFX_TKIT_IRQ_HANDLED:
      /* Everything was done by ISR, no need to call DSR */
      break;

    case CIFX_TKIT_IRQ_OTHERDEVICE:
    default:
      /* This is an interrupt from another device. pciIntConnect creates a linked
         list of ISRs that are sharing the same interrupt line. When the interrupt
         occurs, it sequentially invokes all the ISRs connected to it.  */
      break;
  }
}

/*****************************************************************************/
/*! Thread for processing deferred interupts
*   \param pvOSDependent OS Dependent Variable passed during call to
*                        cifXTKitAddDevice
*   \return 0                                                                */
/*****************************************************************************/
static int OS_DeferredServiceThread( void* pContext)
{
  PVXW_CIFXDRV_DEVINSTANCE_T ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)pContext;

  ptDev->fDeferredServiceThreadRunning = 1;

  while(ptDev->fDeferredServiceThreadRunning)
  {
    if ( (CIFX_EVENT_SIGNALLED == OS_WaitEvent(ptDev->hDeferredServiceEvent, WAIT_FOREVER)) &&
         (1                    == ptDev->fDeferredServiceThreadRunning)                        )
    {
      if( !ptDev->fDeferredServiceThreadSuspended)
      {
        cifXTKitDSRHandler(&ptDev->tDeviceInst);
      }
    }
  }

  return 0;
}

/*****************************************************************************/
/*! This function enables the interrupts for the device physically
*   \param pvOSDependent OS Dependent Variable passed during call to
*                        cifXTKitAddDevice                                   */
/*****************************************************************************/
void OS_EnableInterrupts(void* pvOSDependent)
{
  PVXW_CIFXDRV_DEVINSTANCE_T  ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;
  int                         iRet = 0;
  STATUS                  	  ret  = ERROR;

#ifdef CIFX_TOOLKIT_ENABLE_DSR_LOCK
  spinLockIsrInit(&ptDev->tIsrLock, 0);
  ptDev->ftIsrLockInitialized = 1;
#endif

  /* Check if we have already a service thread */
  if( NULL == ptDev->iDeferredServiceThread)
  {
    /* Create an interrupt service thread */
    ptDev->hDeferredServiceEvent          = OS_CreateEvent();
    ptDev->fDeferredServiceThreadSuspended= 0;
    ptDev->iDeferredServiceThread         = taskSpawn("DeferredServiceThread",
                                                      99,
                                                      0,
                                                      0x1000,
                                                      OS_DeferredServiceThread,
                                                      (_Vx_usr_arg_t)ptDev,0,0,0,0,0,0,0,0,0);
    if( NULL == ptDev->hDeferredServiceEvent)
    {
      if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace(&ptDev->tDeviceInst,
                   TRACE_LEVEL_ERROR,
                   "Failed to create DSR service event!");
      }
    }

    if( TASK_ID_ERROR == ptDev->iDeferredServiceThread)
    {
      /* Set to NULL for next try */
      ptDev->iDeferredServiceThread          = NULL;
      ptDev->fDeferredServiceThreadSuspended = 0;
      ptDev->fDeferredServiceThreadRunning   = 0;

      if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
      {
        USER_Trace(&ptDev->tDeviceInst,
                   TRACE_LEVEL_ERROR,
                   "Failed to create DSR service thread!");
      }
    }

  }else
  {
  /* Check if the thread is suspended and let it run again */
  if( 0 != ptDev->fDeferredServiceThreadSuspended)
    ptDev->fDeferredServiceThreadSuspended = 0;
  }

  /* Connect interrupt */
  iRet = vxbIntConnect(ptDev->tDeviceEntry.pDev, ptDev->tDeviceEntry.tCifXPciData.pResIRQ, OS_InterruptServiceRoutine, (void*)&ptDev->tDeviceInst);

  if (ERROR == iRet)
  {
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(&ptDev->tDeviceInst,
                 TRACE_LEVEL_ERROR,
                 "Failed to connect interrupt!, lRet = 0x%08X", iRet);
    }
  }

  /* Enable interrupt */
  ret = vxbIntEnable(ptDev->tDeviceEntry.pDev, ptDev->tDeviceEntry.tCifXPciData.pResIRQ);
	if (ret != OK)
		{
		(void)vxbIntDisconnect (ptDev->tDeviceEntry.pDev, ptDev->tDeviceEntry.tCifXPciData.pResIRQ);
		return ERROR;
		}

  /* Handle PLX Interrupts differently */
#if 0
  if( TRUE   == ptDev->tPciInfo.fPLXBridge )
  {
    /* Enable PLX Interrupts  */
    uint8_t bData = ptDev->tPciInfo.pbPLXRegisters[PLX_PCI_INTCTRLSTS_REG];

    bData |= PLX_HW_INTERRUPT_ENABLE;

    /* Write new data to port */
    ptDev->tPciInfo.pbPLXRegisters[PLX_PCI_INTCTRLSTS_REG] = bData;
  }
#endif
}

/*****************************************************************************/
/*! This function disables the interrupts for the device physically
*   \param pvOSDependent OS Dependent Variable passed during call to
*                        cifXTKitAddDevice                                   */
/*****************************************************************************/
void OS_DisableInterrupts(void* pvOSDependent)
{
  PVXW_CIFXDRV_DEVINSTANCE_T  ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;
  int                         iRet = 0;

  /* Disable Interrupt */
  iRet = vxbIntDisable(ptDev->tDeviceEntry.pDev, ptDev->tDeviceEntry.tCifXPciData.pResIRQ);
  if (ERROR == iRet)
  {
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(&ptDev->tDeviceInst,
                 TRACE_LEVEL_ERROR,
                 "Failed to disable interrupt!, iRet = 0x%08X", iRet);
    }
  }

  /* Disconnect interrupt */
  iRet = vxbIntDisconnect(ptDev->tDeviceEntry.pDev, ptDev->tDeviceEntry.tCifXPciData.pResIRQ);
  if (ERROR == iRet)
  {
    if (g_ulTraceLevel & TRACE_LEVEL_ERROR)
    {
      USER_Trace(&ptDev->tDeviceInst,
                 TRACE_LEVEL_ERROR,
                 "Failed to disconnect interrupt!, lRet = 0x%08X", iRet);
    }
  }

  /* Handle PLX Interrupts differently */
#if 0
  if( TRUE   == ptDev->tPciInfo.fPLXBridge )
  {
    /* Disable PLX Interrupts  */
    uint8_t bData = ptDev->tPciInfo.pbPLXRegisters[PLX_PCI_INTCTRLSTS_REG];

    bData &= ~PLX_HW_INTERRUPT_ENABLE;

    /* Write new data to port */
    ptDev->tPciInfo.pbPLXRegisters[PLX_PCI_INTCTRLSTS_REG] = bData;
  }
#endif

#if 0
  /* Remove the interrupt service thread */
  if( 0 != ptDev->iDeferredServiceThread)
  {
  /* Signal thread stop */
    ptDev->fDeferredServiceThreadRunning = 0;
    /* Signal the thread because it may wait for an event */
    OS_SetEvent(ptDev->hDeferredServiceEvent);

    if(OK != taskWait(ptDev->iDeferredServiceThread, WAIT_FOREVER))
    {
      OS_Sleep(100);
      taskDelete(ptDev->iDeferredServiceThread);
    }
    ptDev->iDeferredServiceThread = NULL;
  }

  OS_DeleteEvent(ptDev->hDeferredServiceEvent);
#endif

  /* Suspend the the interrupt service thread */
  if( NULL != ptDev->iDeferredServiceThread)
  {
  /* Signal thread suspend */
    ptDev->fDeferredServiceThreadSuspended = 1;

    /* Signal the thread because it may wait for an event */
    OS_SetEvent(ptDev->hDeferredServiceEvent);
  /* Wait until thread was running */
    OS_Sleep(10);
  }
}

#ifdef CIFX_TOOLKIT_ENABLE_DSR_LOCK
/*****************************************************************************/
/*! This functions needs to provide a lock against the interrupt service
*   routine of the device. The easiest way is an IRQ lock but some operating
*   systems provide a way to lock against a specific interrupt
*     \param pvODDependent OS Dependent parameter in DEVICEINSTANCE          */
/*****************************************************************************/
void OS_IrqLock(void* pvOSDependent)
{
  PVXW_CIFXDRV_DEVINSTANCE_T  ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;

  if((NULL != pvOSDependent) && (ptDev->ftIsrLockInitialized == 1))
  {
    spinLockIsrTake(&ptDev->tIsrLock);
  }
}

/*****************************************************************************/
/*! This function re-enables the device's interrupt service routine.
*     \param pvODDependent OS Dependent parameter in DEVICEINSTANCE          */
/*****************************************************************************/
void OS_IrqUnlock(void* pvOSDependent)
{
  PVXW_CIFXDRV_DEVINSTANCE_T  ptDev = (PVXW_CIFXDRV_DEVINSTANCE_T)pvOSDependent;

  if((NULL != pvOSDependent) && (ptDev->ftIsrLockInitialized == 1))
  {
    spinLockIsrGive(&ptDev->tIsrLock);
  }
}
#endif

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

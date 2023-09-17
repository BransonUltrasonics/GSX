/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: OS_Specific.c 7061 2015-07-21 10:04:03Z LuisContreras $:

  Description:
    VxWorks specific OS function implementation

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  created

**************************************************************************************/

/*****************************************************************************/
/*! \file OS_Specific.c
*    VxWorks specific OS function implementation                             */
/*****************************************************************************/

#include "OS_Includes.h"

/*****************************************************************************/
/*!  \addtogroup CIFX_TK_OS_ABSTRACTION Operating System Abstraction
*    \{                                                                      */
/*****************************************************************************/

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
  uint32_t ulDestAlignment   = (uint32_t)(*((unsigned int*)pvDest) & (unsigned int)0x03);
  uint32_t ulSourceAlignment = (uint32_t)(*((unsigned int*)pvSrc)  & (unsigned int)0x03);

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
uint32_t OS_Gettickcount(void)
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
*   \param szName Name for event
*   \return handle to the created event                                      */
/*****************************************************************************/
void* OS_CreateNamedEvent(char* szName)
{
  SEM_ID hSemaphore = semOpen (szName, /* name of semaphore */
                                SEM_TYPE_BINARY,/* type of semaphore */
                                SEM_EMPTY, /* initial state or initial count */
                                SEM_Q_PRIORITY, /* semaphore options */
                                OM_CREATE | OM_EXCL | OM_DELETE_ON_LAST_CLOSE, /* OM_CREATE, ... */
                                NULL); /* context value. Not used in VxWorks */

  return hSemaphore;
}

/*****************************************************************************/
/*! Delete an event
*   \param pvEvent Handle to event being deleted                             */
/*****************************************************************************/
void OS_DeleteNamedEvent(void* pvEvent)
{
  SEM_ID hSemaphore = (SEM_ID)pvEvent;

  semClose(hSemaphore);
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
/*! Copies one string to another monitoring the maximum length of the target
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
/*! Checks the console for keyboard input.
*   \return Nonzero value if a key has been pressed. Otherwise, it returns 0.*/
/*****************************************************************************/
int OS_KbHit(void)
{
  static int iCnt = 10;

  if (0 == --iCnt)
  {
    iCnt = 10;
    return 1;
  }

  return 0;
}

/*****************************************************************************/
/*! Returns the character read.
*   \return Returns the character read.                                      */
/*****************************************************************************/
int OS_GetChar(void)
{
  return 'Y';/* toupper (getchar()); */
}

/*****************************************************************************/
/*! Retrieves the last-error code value.
*   \return Error code value                                                 */
/*****************************************************************************/
int OS_GetLastError(void)
{
 return 0;
}

/*****************************************************************************/
/*! Global Lock                                                              */
/*****************************************************************************/
SEM_ID g_hLock;

/*****************************************************************************/
/*! Acquire a lock (TCP Server)
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
int OS_Lock(void)
{
  semTake(g_hLock, WAIT_FOREVER);
  return 1;
}

/*****************************************************************************/
/*! Release a lock (TCP Server)
*     \param pvLock Handle to lock                                           */
/*****************************************************************************/
void OS_Unlock(int iLock)
{
  semGive(g_hLock);
}


/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

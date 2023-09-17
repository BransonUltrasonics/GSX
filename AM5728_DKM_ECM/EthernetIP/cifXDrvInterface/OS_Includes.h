/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: OS_Includes.h 7061 2015-07-21 10:04:03Z LuisContreras $:

  Description:
    Operating system specific function definition file

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-12  created

**************************************************************************************/

/*****************************************************************************/
/*!  \addtogroup CIFX_TK_OS_ABSTRACTION Operating System Abstraction
*    \{                                                                      */
/*****************************************************************************/

#ifndef __OS_INCLUDES__H
#define __OS_INCLUDES__H

#include <vxworks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WRS_KERNEL
#include <strings.h>        /* strncasecmp() (DKM) */
#endif /* _WRS_KERNEL */
#include <ioLib.h>          /* open, ioctl, close */
#include <semLib.h>         /* semaphore functions (OS_*Lock(), OS_*Event() implementation */
#include <lstLib.h>         /* linked list implementation for directory entries */
#include <sysLib.h>
#include <taskLibCommon.h>
#include <ctype.h>
#include <tickLib.h>

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UNREFERENCED_PARAMETER
  #define UNREFERENCED_PARAMETER(a) (a=a)
#endif

#define CIFX_EVENT_SIGNALLED  0
#define CIFX_EVENT_TIMEOUT    1

void*    OS_Memalloc     (uint32_t ulSize);
void     OS_Memfree      (void* pvMem);
void     OS_Memset       (void* pvMem, uint8_t bFill, uint32_t ulSize);
void     OS_Memcpy       (void* pvDest, void* pvSrc, uint32_t ulSize);
int      OS_Memcmp       (void* pvBuf1, void* pvBuf2, uint32_t ulSize);
void     OS_Memmove      (void* pvDest, void* pvSrc, uint32_t ulSize);

int      OS_Strcmp       (const char* pszBuf1, const char* pszBuf2);
int      OS_Strnicmp     (const char* pszBuf1, const char* pszBuf2, uint32_t ulLen);
int      OS_Strlen       (const char* szText);
char*    OS_Strncpy      (char* szDest, const char* szSource, uint32_t ulLength);

void*    OS_FileOpen     (char* szFile, uint32_t* pulFileLen);
void     OS_FileClose    (void* pvFile);
uint32_t OS_FileRead     (void* pvFile, uint32_t ulOffset, uint32_t ulSize, void* pvBuffer);

void     OS_Sleep        (uint32_t ulSleepTimeMs);

void*    OS_CreateLock   (void);
void     OS_EnterLock    (void* pvLock);
void     OS_LeaveLock    (void* pvLock);
void     OS_DeleteLock   (void* pvLock);

int      OS_KbHit        (void);
int      OS_GetChar      (void);
int      OS_GetLastError (void);

void*    OS_CreateNamedEvent  (char* szName);
void     OS_DeleteNamedEvent  (void* pvEvent);
void     OS_SetEvent          (void* pvEvent);
uint32_t OS_WaitEvent         (void* pvEvent, uint32_t ulTimeout);

/* Added for Marshaller */
uint32_t OS_Gettickcount      (void);
int      OS_Lock              (void);
void     OS_Unlock            (int iLock);

#define OS_MALLOC       OS_Memalloc
#define OS_MEMCPY       OS_Memcpy
#define OS_FREE         OS_Memfree
#define OS_STRNCPY      OS_Strncpy
#define OS_STRNICMP     OS_Strnicmp
#define OS_STRLEN       OS_Strlen
#define OS_GETTICKCOUNT OS_Gettickcount
#define OS_LOCK         OS_Lock
#define OS_UNLOCK       OS_Unlock

#ifdef __cplusplus
}
#endif

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

#endif /* __OS_INCLUDES__H */

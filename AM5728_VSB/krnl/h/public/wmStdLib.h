/* wmStdLib.h - standard C library replacement */

/* Copyright 2003-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01c,03aug04,jws  fix ISWSPACE definitions
01b,05may04,jws  Macro and function name rework.
01a,07oct03,jws  First attempt at adding coding standards related text.
*/

/*
 * DESCRIPTION:
 *
 * To be added...
 */

#ifndef __INCwmStdLibh
#define __INCwmStdLibh

/* The following definition clarifies the meaning of __USE_LOCAL_ANSI_LIB__ */

#ifndef __USE_LOCAL_ANSI_LIB__
#define __USE_WM_ANSI_C_IMPLEMENTATION__
#endif

/* started of with mem mgr in backplane */

#define wmbMalloc   wmMalloc
#define wmbCalloc   wmCalloc
#define wmbFree     wmFree

/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * These functions duplicate standard library functions,
 * and are are usually not compiled.
 */

sbyte * wmStrtok_reentrant( sbyte *pStr, sbyte *pDelimiter, sbyte **ppPrevStr);
sbyte4  wmStricmp ( sbyte *pStr1, sbyte *pStr2                    );
#ifndef _WRS_CONFIG_LP64
sbyte4  wmStrnicmp( sbyte *pStr1, sbyte *pStr2, sbyte4 Len        );
#else
sbyte4  wmStrnicmp( sbyte *pStr1, sbyte *pStr2, sbyte8 Len        );
#endif
sbyte * wmStrichr ( const sbyte *pStr, sbyte4 c                   );

BOOL    wmIslower (sbyte4 c);
BOOL    wmIsdigit (sbyte4 c);
BOOL    wmIsspace (sbyte4 c);
BOOL    wmIswspace(sbyte4 c);
sbyte4  wmAtoi    (const sbyte *pStr);
#ifndef _WRS_CONFIG_LP64
sbyte4  wmAtol    (const sbyte *pStr);
#else
sbyte8  wmAtol    (const sbyte *pStr);
#endif
BOOL    wmIsalpha (sbyte4 x);
BOOL    wmIsalnum (sbyte4 c);

void * wmMemset  ( void *pDest, sbyte4 value, Length size         );
void*    wmMemcpy  ( void *pDest, const void *pSrc, Length size    );
#ifndef _WRS_CONFIG_LP64
sbyte4   wmMemcmp  ( const void *pBuf1, const void *pBuf2, Length Len );
#else
sbyte8   wmMemcmp  ( const void *pBuf1, const void *pBuf2, Length Len );
#endif 
#ifndef _WRS_CONFIG_LP64
ubyte4   wmStrlen  ( const sbyte *pStr                             );
#else
ubyte8   wmStrlen  ( const sbyte *pStr                             );
#endif 

ubyte4   wmStrcmp  ( sbyte *pStr1, sbyte *pStr2                    );
#ifndef _WRS_CONFIG_LP64
ubyte4   wmStrncmp ( sbyte *pStr1, sbyte *pStr2, sbyte4 Len        );
#else
ubyte4   wmStrncmp ( sbyte *pStr1, sbyte *pStr2, sbyte8 Len        );
#endif

sbyte*   wmStrcpy  ( sbyte *pDest, const sbyte *pSrc               );
#ifndef _WRS_CONFIG_LP64
sbyte*   wmStrncpy ( sbyte *pDest, const sbyte *pSrc, ubyte4 num   );
#else
sbyte*   wmStrncpy ( sbyte *pDest, const sbyte *pSrc, ubyte8 num   );
#endif
sbyte*   wmStrcat  ( sbyte *pDest, const sbyte *pSrc               );
#ifndef _WRS_CONFIG_LP64
sbyte*   wmStrncat ( sbyte *pDest, const sbyte *pSrc, ubyte4 bufLen);
#else
sbyte*   wmStrncat ( sbyte *pDest, const sbyte *pSrc, ubyte8 bufLen);
#endif
sbyte*   wmStrchr  ( const sbyte *pStr, sbyte4 c                   );
sbyte*   wmStrrchr ( const sbyte *pStr, sbyte4 c                   );
#ifndef _WRS_CONFIG_LP64
sbyte4   wmStrspn  ( sbyte *pStr, sbyte *pSampleChars              );
#else
sbyte8   wmStrspn  ( sbyte *pStr, sbyte *pSampleChars              );
#endif
#ifndef _WRS_CONFIG_LP64
sbyte4   wmStrcspn ( sbyte *pStr, sbyte *pReject                   );
#else
sbyte8   wmStrcspn ( sbyte *pStr, sbyte *pReject                   );
#endif
sbyte*   wmStrpbrk ( sbyte *pStr, sbyte *pSampleStr                );

/* First, some non-ansi lib stuff */

#ifndef _WRS_CONFIG_LP64
STATUS  RC_Replace(sbyte *pInput, sbyte *pOutput, sbyte4 outLen,
                           sbyte *pFind, sbyte *pReplace, BOOL    word  );
#else
STATUS  RC_Replace(sbyte *pInput, sbyte *pOutput, sbyte8 outLen,
                           sbyte *pFind, sbyte *pReplace, BOOL    word  );
#endif

/* Time routine and debug macro */

void wmAscTime (ubyte4 TimeInSec, char *pTimeBuffer);

#ifdef __WMB_DEBUG__
#define DEBUG_TIME(time, buffer)   wmAscTime(time, buffer)
#else
#define DEBUG_TIME(time, buffer)
#endif

/* General Tests */

#define MIN_M(x, y)   ( (x < y) ? (x) : (y) )
#define MAX_M(x, y)   ( (x > y) ? (x) : (y) )
#define IN_RANGE_M(low,value,high)     ((low <= value) && (value <= high))

/*
 * Now, alternate definitions for the ansi related macros.
 * The default is, or should be, to not use the WM functions.
 */

#ifdef __USE_WM_ANSI_C_IMPLEMENTATION__

#define ASCTIME  wmAscTime

#define ISLOWER  wmIslower
#define ISDIGIT  wmIsdigit
#define ISSPACE  wmIsspace
#define ISWSPACE wmIsspace
#define ATOI     wmAtoi
#define ATOL     wmAtol
#define ISALPHA  wmIsalpha
#define ISALNUM  wmIsalnum

/* Memory Routines. */

#define MEMSET wmMemset
#define MEMCPY wmMemcpy
#define MEMCMP wmMemcmp

/* String functions. */

#define STRLEN           wmStrlen
#define STRCMP           wmStrcmp
#define STRNCMP          wmStrncmp
#define STRCPY           wmStrcpy
#define STRNCPY          wmStrncpy
#define STRCAT           wmStrcat
#define STRNCAT          wmStrncat
#define STRCHR           wmStrchr
#define STRRCHR          wmStrrchr
#define STRSPN           wmStrspn
#define STRCSPN          wmStrcspn
#define STRPBRK          wmStrpbrk
#define STRTOK_REENTRANT wmStrtok_reentrant
#define STRNICMP         wmStrnicmp
#define STRICHR          wmStrichr
#define STRICMP          wmStricmp

/* other stuff */

#define TOUPPER(x)  ( (('a' <= x) && ('z' >= x)) ? (x - 'a' + 'A') : x )
#define TOLOWER(x)  ( (('A' <= x) && ('Z' >= x)) ? (x - 'A' + 'a') : x )

#else   /* use the OS versions of these macros/functions */

#include <stdlib.h>
#include <string.h>

/*
 * There are a few macros with no VxWorks implementation
 */

#ifndef ASCTIME
#define ASCTIME wmAscTime
#endif

#ifndef STRTOK_REENTRANT
#define STRTOK_REENTRANT wmStrtok_reentrant
#endif

#ifndef STRNICMP
#define STRNICMP wmStrnicmp
#endif

#ifndef STRICHR
#define STRICHR wmStrichr
#endif

#ifndef STRICMP
#define STRICMP wmStricmp
#endif

/* Memory Routines. */

#define MEMSET      memset
#define MEMCPY      memcpy
#define MEMCMP      memcmp

/* String functions. */

#define STRCMP      strcmp
#define STRNCMP     strncmp
#define STRLEN      strlen
#define STRCPY      strcpy
#define STRNCPY     strncpy
#define STRCAT      strcat
#define STRNCAT     strncat
#define STRCHR      strchr
#define STRRCHR     strrchr
#define STRSPN      strspn
#define STRCSPN     strcspn
#define STRPBRK     strpbrk

/* String Conversion routines */

#define ATOI        atoi
#define ATOL        atol

#define ISLOWER     islower
#define ISDIGIT     isdigit
#define ISSPACE     isspace
#define ISWSPACE    isspace
#define TOUPPER     toupper
#define TOLOWER     tolower
#define ISALPHA     isalpha
#define ISALNUM     isalnum

#endif /* __USE_WM_ANSI_C_IMPLEMENTATION__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCwmStdLibh */

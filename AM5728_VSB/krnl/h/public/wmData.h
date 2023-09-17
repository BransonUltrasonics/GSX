/* wmData.h - header for WindManage data types and macros */

/*
 * Copyright (c) 2004-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21jul16,r_w  fix gcc 4.3.3.2 compiler warning
02dec11,f_f  SNMP DateAndTime type support
17may12,h_l  remove NO_STRING macro (WIND00032212)
17dec10,m_z  part of APIs changed
                 CQID: Fix WIND00247096
15mar10,m_z  VxWorks 64bit support (WIND00193649)
17jul08,q_z  clear the warning of gnu compiler
08mar06,ps   BYTE_ORDER macros handled differently on Linux
08jan04,jws  Require _BYTE_ORDER macro.
                 Add modification history.
*/

/*
 * DESCRIPTION:
 *
 * To be added...
 */

#ifndef __INCwmDatah
#define __INCwmDatah

#ifdef MAX_EXTENDED_TYPES_K
#define BASE_DATATYPE_K MAX_EXTENDED_TYPES_K + 1
#else
#define BASE_DATATYPE_K 1
#endif

/* give customer 0 to x range */

typedef enum dTypes
    {
    DATATYPE_INVALID_K,
    DATATYPE_UNKNOWN_K = BASE_DATATYPE_K,
    DATATYPE_BOOL_K,
    DATATYPE_ABSOLUTE_K,
    DATATYPE_OID_K,
    DATATYPE_STRING_K,
    DATATYPE_IPV4_K,
    DATATYPE_IPV6_K,
    DATATYPE_INTEGER_K,
    DATATYPE_CHAR_K,
    DATATYPE_SHORT_K,
    DATATYPE_LONG_K,
    DATATYPE_INT64_K,
    DATATYPE_UINTEGER_K,
    DATATYPE_UCHAR_K,
    DATATYPE_USHORT_K,
    DATATYPE_ULONG_K,
    DATATYPE_UINT64_K,
    DATATYPE_VOID_K,
    DATATYPE_NULL_K,
    DATATYPE_MACADDR_K,
    DATATYPE_ENUM_K,
    DATATYPE_LIST_K,
    DATATYPE_ACCESS_K,
    DATATYPE_FUNCPTR_K,
    DATATYPE_VOIDFUNCPTR_K,
    DATATYPE_OCTETS_K,      /* binary snmp string                            */
    DATATYPE_TIME_K,        /* time  is int representing seconds since epoch */
    DATATYPE_TICKS_K,       /* ticks is int representing seconds duration    */
    DATATYPE_COUNTER64_K,
    DATATYPE_ROW_STATUS_K,
    DATATYPE_COUNTER_K,
    DATATYPE_GAUGE_K,
    DATATYPE_OPAQUE_K,
    DATATYPE_PORT_LIST_K,
    DATATYPE_BRIDGE_ID_K,
    DATATYPE_DATEANDTIME_K
    }
    DATATYPE_T;


/******* WARNING!!!! use last entry of DATATYPE_T here *****************/

#define DATATYPE_MAX_K   DATATYPE_BRIDGE_ID_K

/* system flags */

#define WM_FLAG_FREE_NAME_K     0x00010000 /* free memory for name on destruct     */
#define WM_FLAG_FREE_VALUE_K    0x00020000 /* free memory for value on destruct    */


#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    ( ! FALSE)
#endif

#ifndef NULL
#define NULL    ((void*)0)
#endif

#ifdef __WIN32_OS__
#define PATH_CHAR_K  '\\'
#else
#define PATH_CHAR_K  '/'
#endif

#define WORD_BITS_K     (sizeof(ubyte4) * 8)

/* !!!!!! ATTENTION: compiler dependent !!!!!!!! */

#ifndef INITIAL_DATA_K
#ifndef _WRS_CONFIG_LP64
#define INITIAL_DATA_K  0xcccccccc
#else
#define INITIAL_DATA_K  0xcccccccccccccccc
#endif
#endif

#ifndef MY_BAD_K
#define MY_BAD_K        0xcaUL
#endif

#ifndef _WRS_CONFIG_LP64
#define OUR_BAD_K      0xcacacaca 
#else
#define OUR_BAD_K      0xcacacacacacacaca
#endif

/* structs are blessed at their creation and tainted at destruction */


#ifndef __WM_NOT_STRICT__
#define wmInvalid(x)            (NULL == x || (void *)INITIAL_DATA_K == (void *) x || (void *)OUR_BAD_K == (void *) x)
#define wmInvalidate(x)         memset(x, MY_BAD_K, sizeof(*(x)))
#else
#define wmInvalid(x)            (NULL == x)
#define wmInvalidate(x)
#endif

#ifndef __NO_MAGIC__
#define wmMagicBless(x, m)      (x)->magic = m
#define wmMagicBad(x, m)        (wmInvalid(x) || (x)->magic != (m))
#define wmMagicGood(x, m)       (! wmMagicBad(x, m))
#else
#define wmMagicBless(x, m)
#define wmMagicBad(x, m)        wmInvalid(x)
#define wmMagicGood(x, m)       (! wmInvalid(x))
#endif

/* placeholder for real nvm code -- remove when that is added */

#define nvmSet(value, size)

/* WIN32 defines */

#ifdef __WIN32_OS__
typedef char            char_t;
typedef long            long_t;
typedef unsigned short  ushort_t;
typedef unsigned char   uchar_t;
typedef unsigned long   ulong_t;
#endif

/* Standard Types */

typedef sbyte   Path;
typedef ubyte4  Flags;
#ifndef _WRS_CONFIG_LP64
typedef ubyte4  Length;
typedef ubyte4  Counter;
#else
typedef ubyte8  Length;
typedef ubyte8  Counter;
#endif
typedef int     CompareType;
typedef ubyte2  UniqId;
typedef ubyte4  EnumType;
typedef sbyte4  EditType;
typedef ubyte4  Cmd;
typedef long    State;
typedef sbyte2  ID;
typedef ubyte4  IPV4;


typedef struct WM_ACCESS_T
    {
    Access          read;
    Access          write;
    }
    WM_ACCESS_T;


/* Custom Type Support */

typedef struct DTEnumInfo
    {
    sbyte *     EnumString;
    EnumType    EnumValue;
    ubyte4      EnumAccess;
    } DTEnumInfo;


typedef struct DTTypeInfo
    {
    sbyte       *pHelpStr;
    void        *pHelpHandler;
    DATATYPE_T  baseType;
    sbyte       *pValidateStr;
#ifdef x__WINDMANAGE__
    WM_ACCESS_T nAccessLevel;
#else
    Access      nAccessLevel;
#endif
    sbyte       *pDefaultStr;
    void        *pCustomValidate;
/*
    STATUS      (*pCustomValidate)(RCB_SESSION_T *, sbyte *, struct DTTypeInfo *, BOOL);
*/
    DTEnumInfo  *pEnumTable;
    } DTTypeInfo;


typedef struct TABLE_HEAD_S
    {
    sbyte * pTitle;
    sbyte4  width;
    } TABLE_HEAD_T;


#define WM_ERROR_CODE   -1

#if defined(__VXWORKS_OS__) || defined(__VXWORKS_AE_OS__)
#include "vxWorks.h"
#elif defined (__UNIX__)
/**/
#include "vxDefs.h"
/**/
#define LOCAL   static
#define IMPORT  extern
typedef void    (*VOIDFUNCPTR)(void *);
#else

#define LOCAL   static
#define IMPORT  extern

#ifndef OK
#define OK    0
#endif

#ifndef ERROR
#define ERROR   WM_ERROR_CODE
#endif

typedef int     STATUS;
typedef int     BOOL;

typedef void *  (*FUNCPTR)(void *);
typedef void    (*VOIDFUNCPTR)(void *);

#endif /* __VXWORKS_OS__ */

/* backwards compatability for RapidControl 3.x */

typedef STATUS RLSTATUS;

/* windmanage name alias delimiters */

#define WM_ALIAS_START_K   '<'
#define WM_ALIAS_STOP_K    '>'

#define ALIAS_DELIM(x)  (WM_ALIAS_START_K == *x || WM_ALIAS_STOP_K  == *x)
#define NAME_OK(x)      (0 != *x && ! ALIAS_DELIM(x))


/* generic backplane function pointer w/ returned status */

typedef STATUS  (*WMBFUNCPTR)(void *);

/* macros for transforming to and from shared memory */

#define wmOffset(base, data)           (void *) ((char *) (data) - (char *) (base))
#define wmAdjust(base, offset)         (void *) ((char *) (base) + (int) (offset))

/*
 * nullAlways - make sure a null address is not taken for a 0 offset
 * returns NULL if input NULL
 */

#define nullAlways(x, y)               (void *) (NULL == x ? NULL : y)

#define wmPtrGet(base, data)           nullAlways (data, wmAdjust(base, data))
#define wmPtrSet(base, data)           nullAlways (data, wmOffset(base, data))

/* "test" is shared memory test resolving as a BOOL */

#define wmDataGet(test, base, offset)  (void *) ((test) ? wmPtrGet(base, offset) : offset)
#define wmDataSet(test, base, data)    (void *) ((test) ? wmPtrSet(base, data)   : data)



/*  MACROS  */

#ifndef WEBCLI_ON_LINUX

#ifndef _BYTE_ORDER
#error _BYTE_ORDER must be defined!!
#endif

#if (_BYTE_ORDER == _BIG_ENDIAN)

#define HTON2(x) (x)
#define NTOH2(x) (x)
#define HTON4(x) (x)
#define NTOH4(x) (x)

#endif

#if (_BYTE_ORDER == _LITTLE_ENDIAN)

#define HTON2(x) ((((x) << 8) & 0xFF00) | (((x) >> 8) & 0x00FF))
#define NTOH2(x) HTON2(x)
#define HTON4(x) (   (( (x) << 24 ) & 0xFF000000) | (( (x) << 8 ) & 0x00FF0000) \
                   | (( (x) >> 8 ) & 0x0000FF00) | (( (x) >> 24 ) & 0x000000FF) )
#define NTOH4(x) HTON4(x)

#endif

#else /* WEBCLI_ON_LINUX */

# ifdef __LITTLE_ENDIAN_SYSTEM__
# define HTON2(x) ((((x) << 8) & 0xFF00) | (((x) >> 8) & 0x00FF))
# define NTOH2(x) HTON2(x)
# define HTON4(x) (  (( (x) << 24 ) & 0xFF000000) | (( (x) << 8 ) & 0x00FF0000) \
                   | (( (x) >> 8 ) & 0x0000FF00) | (( (x) >> 24 ) & 0x000000FF) )
# define NTOH4(x) HTON4(x)
# endif

#ifdef __BIG_ENDIAN_SYSTEM__
# define HTON2(x) (x)
# define NTOH2(x) (x)
# define HTON4(x) (x)
# define NTOH4(x) (x)
#endif

#endif /* WEBCLI_ON_LINUX */

/* Free and null pointer to allocated memory */

#define FREEMEM(x)              {if (NULL != x) {wmFree(x); x = NULL;}}

#define wmNew(ptr)              (NULL != ((ptr) = wmCalloc(1, sizeof(*(ptr)))))
#define wmNewMany(ptr, count)   (NULL != ((ptr) = wmMalloc(sizeof(*(ptr)) * count)))
#define wmDelete(ptr)           wmFree(ptr)

/* misc handy tools */

#define NOT_EMPTY(x)            ((NULL != x) && ('\0' != *x))
#define NULL_STRING(x)          ((NULL == x) || ('\0' == *x))
#define HAS_STRING(x)           ((NULL != x) && ('\0' != *x))
#define STRING_EQUAL(x,y)       (0 == strncmp(x, y, strlen(y)))

#define ARRAY_SIZE(x)           (sizeof(x)/sizeof(x[0]))
#define DIGIT_TO_CHAR(Digit)    ((sbyte)(Digit - '0'))

/* #define NULLCHECK(x) */

#define FLAG_SET(x, y)          ((NULL == x) ? FALSE : (((x)->flags & (y)) == (y)))
#define FLAG_CLEAR(x, y)        (!FLAG_SET(x, y))
#define CLEAR_FLAG(x, y)        ((x)->flags &= ~y)
#define SET_FLAG(x, y)          ((x)->flags |=  y)

#define wmZero(x)               memset(x, 0, sizeof(*(x)))

#define DATA_ZERO_M             wmZero

/* data debugging */

#define DATA_DEBUG_SHOW_DATA_K
#define DATA_DEBUG_SHOW_ERROR_K
#define DATA_DEBUG_SHOW_FILEINFO_K

#ifdef  DATA_DEBUG_SHOW_FILEINFO_K
#define DATA_MSG_FILEINFO_M     printf("File: %s Line: %d ", __FILE__, __LINE__)
#else
#define DATA_MSG_FILEINFO_M     TRUE
#endif

#ifdef  DATA_DEBUG_SHOW_DATA_K
#define DATA_MSG_VALUE_M(x)     printf("value %x = %x\n", x, *x)
#else
#define DATA_MSG_VALUE_M(x)     TRUE
#endif

#define DATA_MSG_NULL_M(x)      printf("value is NULL\n")
#define DATA_MSG_NO_INIT_M(x)   printf("value uninitialized")

#ifdef  DATA_DEBUG_SHOW_ERROR_K
#define DATA_MSG_BAD_DATA_M(x)  ((NULL == x) ? DATA_MSG_NULL_M(x) : DATA_MSG_NO_INIT_M(x))
#else
#define DATA_MSG_BAD_DATA_M(x)  TRUE
#endif

#define NULL_TEST_M(x)          (DATA_MSG_FILEINFO_M && (NULL != x) && (((void *) INITIAL_DATA_K) != x))

#define DATA_DEBUG_MSG_M(x)     NULL_TEST_M(x) ? DATA_MSG_VALUE_M(x) : DATA_MSG_BAD_DATA_M(x)

#define NOT_NULL_M(x)           (DATA_DEBUG_MSG_M(x) && NULL_TEST_M(x))
#define DATA_VALID_1_M(x, y)    (NOT_NULL_M(x) && (z) ? TRUE : FALSE)
#define DATA_VALID_2_M(x, y, z) ((NOT_NULL_M(x) && NOT_NULL_M(y)) && (z) ? TRUE : FALSE)

#endif /* ! __INCwmDatah */

/* wmbCommon.h -  Wind River Standard Definitions */

/* Copyright (c) 2003 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01c,12oct05,wdz  '#ifdef __cplusplus' statements added
                 for SPR#98444
01b,07jul04,jws  remove IS_RAPIDMARK macro defs
01a,19dec03,jws  add mod history and copyright, SPR92723
                 move DTNAMES and DTLEN defs to wmConvertLib.c
*/

#ifndef __INCwmbCommonh
#define __INCwmbCommonh

#ifdef __cplusplus
#include "wmBufferLib.h"
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* constants */

typedef struct DTYPE
    {
	sbyte	*    pHelpStr;
 	void    *    pHelpHandler;
	DATATYPE_T   baseType;
	sbyte   *    pValidateStr;
	Access	     accessLevel;
	sbyte   *    pDefaultStr;
/*
	STATUS   (*pfn_Validate)(struct WMB_OBJECT_T *, sbyte *, struct DTYPE *, BOOL   );
*/
/*
	DTEnumInfo * pEnumTable;
*/
    } *PDTYPE, DTYPE;



typedef struct WMB_DESTRUCTOR_S
    {
    int         domain;
    FUNCPTR *   destructor;
    void *      pData;
    } WMB_DESTRUCTOR_T;


typedef struct METAVALUE_S
    {
    ubyte   domain;         /* domain where data lives  */
    ubyte   type_obj;       /* type currently in object */
    ubyte   type_native;    /* type native storage      */
    ubyte   size;           /* data size                */
    } METAVALUE_T;


typedef struct WMB_DOUBLE_S
    {
    ubyte4   high;
    ubyte4   low;
    } WMB_DOUBLE_T;

/*
typedef struct WMB_VALUE_S
    {
    METAVALUE_T info;
    union
        {
        sbyte4        longInt;
        ubyte4        longUns;
        WMB_BUFFER_T  bufChr;
        WMB_OBJID_T   bufInt;
        WMB_DOUBLE_T  doubleUns;
        sbyte * pPtr;
        } value;
    } WMB_VALUE_T;

*/

typedef WM_BUFFER_T    WMB_VALUE_T;

#define WMB_INSTANCE_MAX_LEN_K  64


/* more defines... */

/* for the CompareType type */

#ifndef EQUAL
#define EQUAL           0
#endif

#ifndef GREATER_THAN
#define GREATER_THAN    1
#endif

#ifndef LESS_THAN
#define LESS_THAN       -1
#endif

#define kMAX_UCHAR      ((unsigned char)((char)(-1)))
#define kMIN_UCHAR      0
#define kMAX_CHAR       (kMAX_UCHAR >> 1)
#define kMIN_CHAR       (~kMAX_CHAR)

#define kMAX_UNSIGNED   ((unsigned int)((int)(-1)))
#define kMIN_UNSIGNED   0
#define kMAX_INT        (kMAX_UNSIGNED >> 1)
#define kMIN_INT        (~kMAX_INT)

#define kMAX_USHORT     ((unsigned short)((short)(-1)))
#define kMIN_USHORT     0
#define kMAX_SHORT      (kMAX_USHORT >> 1)
#define kMIN_SHORT      (~kMAX_SHORT)

#define kMAX_ULONG      ((unsigned long)((long)(-1)))
#define kMIN_ULONG      0
#define kMAX_LONG       (kMAX_ULONG >> 1)
#define kMIN_LONG       (~kMAX_LONG)

#define kErrorChange    -1
#define kNoChange       0
#define kChange         1

#define kMaxTimeString  32

#ifndef kCRLF
#define kCRLF       "\x0D\x0A"
#define kCRLFSize   2
#endif

#ifndef kCR
#define kCR     ((char)(0x0d))
#endif

#ifndef kLF
#define kLF     ((char)(0x0a))
#endif

#ifndef kBS
#define kBS     ((char)(0x08))
#endif

#ifndef kDEL
#define kDEL    ((char)(0x7F))
#endif

#ifndef kTAB
#define kTAB    ((char)(0x09))
#endif

#ifndef kHELP
#define kHELP   ((char)(0x3F))
#endif

#ifndef kESC
#define kESC    ((char)(0x1b))
#endif

#ifndef kCHAR_NULL
#define kCHAR_NULL ((char)(0x00))
#endif

#ifndef kEOF
#define kEOF -1
#endif

#define wmbNew          wmNew
#define wmbDelete       wmDelete

/*
 *  These last definitions are out of date and should not be here in the
 *  first place.  There are definitions in rcc_db_p.h.
 */
#if 0
#define IS_RAPIDMARK_START(x)  \
((kMagicMarkupStartChar0 == *x) && (kMagicMarkupStartChar1 == *(x + 1)))

#define IS_RAPIDMARK_END(x)  \
((kMagicMarkupEndChar0 == *x) && (kMagicMarkupEndChar1 == *(x + 1)))
#endif


#ifdef __cplusplus 
}
#endif /* __cplusplus */

#endif /* __INCwmbCommonh */


/* wmConvertLib.h */

/* Copyright (c) 2004 2010, 2012 Wind River Systems, Inc. 
 *      
 * The right to copy, distribute, modify, or otherwise make use 
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,08nov12,shi  add declaration of wmConvertEnumFromStr() and
                 wmConvertIntToStr(). (WIND00386819)
01e,17dec10,m_z part of APIs changed
                CQID: Fix WIND00247096
01d,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01c,30jun04,jws  remove __RCC_CASE_INSENSITIVE__ reference
01b,13may04,???  To correct lint warnings
01a,20feb04,jws  added copyright and mod history
*/


#ifndef __INCwmConvertLibh
#define __INCwmConvertLibh

#if 0
# ifdef __RCC_CASE_INSENSITIVE__
#  define COMPARE  STRICMP
#  define NCOMPARE STRNICMP
# else
#  define COMPARE  STRCMP
#  define NCOMPARE STRNCMP
# endif /* __RCC_CASE_INSENSITIVE__ */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes */

STATUS wmConvertEnum
    (
    sbyte *  pValidation,
    sbyte4   number,
    sbyte * * ppEnum,
    sbyte4 * pLength
    );

STATUS wmConvertStrToOid
    (
    sbyte *     pAsciiOid,
    void *      pBuf,
    sbyte4      bufLen,
    sbyte4 *    pBytesUsed
    );


sbyte4  wmConvertSecsToStr (ubyte4 timeInSec, sbyte * pTimeBuffer, sbyte4 bufferSize);

STATUS  wmConvert (void * pSrc,  ssize_t srcSize,  DATATYPE_T srcType,
                   void * pDest, ssize_t destSize, DATATYPE_T destType,
                   ssize_t * pSize);
DATATYPE_T wmConvertDataTypeByName (sbyte * pName);
BOOL    wmConvertIsNumber (sbyte *pBuf);
sbyte4  wmConvertOctetCount (sbyte * pAsciiOid);
STATUS  wmConvertStrTo (sbyte *pString, void *pDataObject, ssize_t bufSize, DATATYPE_T DType);
STATUS  wmConvertToStr (void *pDataObject, sbyte *pString, sbyte4 bufSize, DATATYPE_T DType);
STATUS  wmConvertStrSize (void * pData, ssize_t srcLen, DATATYPE_T type, ssize_t * pSize);
STATUS  wmConvertStrTypeTo (sbyte *pString, void *pDataObject, DTTypeInfo *pTypeInfo);
sbyte  * wmConvertDataTypeName (DATATYPE_T type);
sbyte  * wmConvertGetDTProtoType (DATATYPE_T type);
int      wmConvertGetEnumCount (DTTypeInfo *pTypeInfo);
STATUS  wmConvertValid (sbyte *pString, DATATYPE_T DType);
void    wmConvertHexString (sbyte4 *hex, sbyte *pBuf);
DTTypeInfo * wmConvertTypeToInfo (DATATYPE_T DType);
STATUS  wmConvertEnumStr (DTTypeInfo *pTypeInfo, sbyte *pOutput,
                          sbyte4 bufLen, sbyte *sep);
STATUS  wmConvertListToStr (EnumType nEnum, sbyte *pString, DTTypeInfo *pTypeInfo);
void    wmConvertSuffix (DTTypeInfo *pTypeInfo, sbyte *what, sbyte **start, sbyte4 *length);
sbyte4  wmConvertSizeOf (DATATYPE_T DType, void * pData);
int     wmConvertStrNumCmp (sbyte * aptr, sbyte * bptr);
sbyte4  wmConvertIntToString (int data, sbyte * pString, sbyte4 strLen);
STATUS  wmConvertValidate (void * pString, DTTypeInfo * pTypeInfo, BOOL errorMsg);
STATUS  wmConvertOidToString (ubyte4 * pData, sbyte4 oidLen, sbyte * pString,
                              sbyte4 strLen, sbyte4 * pWritten);
STATUS  wmConvertNumberToStr (unsigned long number, sbyte * pString, ssize_t bufferSize, BOOL isSigned,
                              ssize_t * pWritten);
STATUS wmConvertIntToStr(int Int, sbyte *pString);
STATUS wmConvertEnumFromStr(sbyte *pValidation, sbyte *pText, sbyte4 *pResult);
#ifdef __cplusplus
}
#endif


#endif /* __INCwmConvertLibh */

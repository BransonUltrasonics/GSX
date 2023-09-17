/* compareLib.h - comparison tests library */

/* Copyright 2004-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
12oct05,wdz      changed '"C"' to 'extern "C"'
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCcompareLibh__
#define __INCcompareLibh__

/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif

/* specific comparison functions */
BOOL compareUchars       (void *pUchar,   void *pUchar1);
BOOL compareIP           (void *pIPaddr,  void *pIPaddr1);
BOOL compareInts         (void *pInt,     void *pInt1);
BOOL compareUnsignedInts (void *pUnsigned,void *pUnsigned1);
BOOL compareShorts       (void *pShort,   void *pShort1);
BOOL compareUshorts      (void *pUshort,  void *pUshort1);
BOOL compareLongs        (void *pLong,    void *pLong1);
BOOL compareUlongs       (void *pUlong,   void *pUlong1);
BOOL compareStrings      (void *pStr,     void *pStr1);

/* general function */
BOOL  compareValues      (void *pDataObj, void *pDataObj1, DATATYPE_T DType);

#ifdef __cplusplus
}
#endif

#endif /* __INCcompareLibh__ */

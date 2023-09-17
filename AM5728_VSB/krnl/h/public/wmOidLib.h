/* wmOidLib.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmOidLibh
#define __INCwmOidLibh

typedef WM_BUFFER_T * WM_OID_T;

#define wmOidBad    wmBufferBad

#ifdef __cplusplus
extern "C" {
#endif

STATUS      wmOidAppend     (WM_OID_T pDestOid, WM_OID_T pAddOid);
int         wmOidCmp        (WM_OID_T first, WM_OID_T second);
BOOL        wmOidEqual      (WM_OID_T first, WM_OID_T second);
WM_OID_T    wmOidFromString (sbyte * pString, int max);
int         wmOidMax        (WM_OID_T oid);
int         wmOidLen        (WM_OID_T oid);
void        wmOidPrint      (WM_OID_T oid, sbyte * pMsg);
BOOL        wmOidScaler     (WM_OID_T oid);
void        wmOidScalerSet  (WM_OID_T oid);
STATUS      wmOidStringSet  (WM_OID_T oid, sbyte * pString, BOOL append);
STATUS      wmOidStringGet  (WM_OID_T oid, sbyte * pString, sbyte4 strLen);
WM_OID_T    wmOidSuffix     (WM_OID_T pBase, WM_OID_T pFull);
STATUS      wmOidTruncate   (WM_OID_T oid, sbyte4 length);
STATUS      wmOidValid      (WM_OID_T oid);

#ifdef __WM_DEBUG__
void        wmOidDebug      (WM_OID_T oid, sbyte * pMsg);
void        wmOidDebugEnable(BOOL enable);
#else
#define     wmOidDebug(x,y)
#define     wmOidDebugEnable(x)
#endif /* __WM_DEBUG__ */

#ifdef __cplusplus
}
#endif

#endif /* ! __INCwmOidLibh */

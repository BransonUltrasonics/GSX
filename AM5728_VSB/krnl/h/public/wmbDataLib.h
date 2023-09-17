/*  - header file for RCB library */

/* Copyright 2002, 2010 Wind River Systems, Inc. */

/*
modification history
--------------------
00c,17dec10,m_z part of APIs changed
                CQID: Fix WIND00247096
00b,30mar10,shi VxWorks 64bit support (WIND00193649)
00a,08aug02,pns created.
*/

/*
 * DESCRIPTION:
 *   wmbDataDataLib.h contains prototypes used to access backplane data.
 */

#ifndef __INCwmbDataLibh
#define __INCwmbDataLibh


#ifdef __cplusplus
extern "C" {
#endif


STATUS  wmbDataType (sbyte * pName, DATATYPE_T * pType);


STATUS  wmbDataStringGet (WMB_SESSION_T * pSession, sbyte * pName,
                          void * pInstance, sbyte4 instanceLength,
                          sbyte *  pValue, ssize_t bufferLength,
                          ssize_t * pWritten);

STATUS wmbDataStringGetNext (WMB_SESSION_T * pSession, sbyte * pName,
						  void * pInstance, sbyte4 * pInstanceLength,
						  sbyte4 maxInstanceLength, DATATYPE_T instanceType,
						  sbyte * pBuffer, ssize_t bufferLength,
						  ssize_t * pWritten );

STATUS  wmbDataStringSet (WMB_SESSION_T * pSession, sbyte * pName,
                          void * pInstance, sbyte4 instanceLength,
                          sbyte * pValue, ssize_t valueLength);

STATUS  wmbDataNativeGet (WMB_SESSION_T * pSession, sbyte * pName,
                          void * pInstance, sbyte4 instanceLength,
                          void * pValue, ssize_t bufferLength,
                          ssize_t * pRead);

STATUS  wmbDataNativeGetNext (WMB_SESSION_T * pSession, sbyte * pName,
						  void * pInstance, sbyte4 * pInstanceLength,
						  sbyte4 maxInstanceLength, DATATYPE_T instanceType,
						  void * pBuffer, ssize_t bufferLength,
						  ssize_t * pWritten );

STATUS  wmbDataNativeSet (WMB_SESSION_T * pSession, sbyte * pName,
                          void * pInstance, sbyte4 instanceLength,
                          void * pValue, ssize_t valueLength);

STATUS  wmbDataTrustedGet (sbyte * pName, void * pInstance,
                           sbyte4 instanceLength, void * pValue,
                           ssize_t valueLength);

STATUS  wmbDataTrustedSet (sbyte * pName, void * pInstance,
                           sbyte4 instanceLength, void * pValue,
                           ssize_t valueLength);

STATUS  wmbDataTrustedGetNext (sbyte * pName,
						  void * pInstance, sbyte4 * pInstanceLength,
						  sbyte4 maxInstanceLength, DATATYPE_T instanceType,
						  void * pBuffer, ssize_t bufferLength,
						  ssize_t * pWritten );

STATUS  wmbDataTrustedNvmSet (sbyte * pName, void * pInstance, sbyte4 instanceLength,
                       void * pValue, ssize_t valueLength);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCwmbDataLibh */

/* rcc_util.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      changed '"C"' to 'extern "C"'
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history.
*/

#ifndef __RCC_UTIL_H__
#define __RCC_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

STATUS  RCC_Grep(WMB_SESSION_T * pCliEnv, sbyte * pSearch, BOOL exclude);


DTTypeInfo *  RCC_UTIL_GetAccess(void);
STATUS  RCC_UTIL_Disable(WMB_SESSION_T * pCliEnv);
STATUS  RCC_UTIL_Enable(WMB_SESSION_T * pCliEnv);
STATUS  RCC_UTIL_ErrorThrow(STATUS status);
STATUS  RCC_UTIL_FileClose(WMB_SESSION_T * pCliEnv);
STATUS  RCC_UTIL_FileOpen(WMB_SESSION_T * pCliEnv, sbyte * pName);
STATUS  RCC_UTIL_FileWrite(WMB_SESSION_T * pCliEnv, sbyte * pInBuf, sbyte4 size);
STATUS  RCC_UTIL_Init(WMB_SESSION_T * pCliEnv);
BOOL    RCC_UTIL_IsQuoted(sbyte * pBuffer, sbyte * pTestCase);
void    RCC_UTIL_RapidPrompt(WMB_SESSION_T * pCliEnv, BOOL print);
STATUS  RCC_UTIL_SecuredAccess(WMB_SESSION_T * pCliEnv, sbyte * login, sbyte * password);
STATUS  RCC_UTIL_SetAccess(WMB_SESSION_T * pCliEnv, sbyte * pAccessStr);
STATUS  RCC_UTIL_SetAccessLevel(WMB_SESSION_T * pCliEnv, sbyte4 level);
STATUS  RCC_UTIL_SetAccessGroups(WMB_SESSION_T * pCliEnv, sbyte * pGroups);
void    RCC_UTIL_SetPrompt(WMB_SESSION_T * pCliEnv, sbyte * promptText);
void    RCC_UTIL_SetPromptTail(WMB_SESSION_T * pCliEnv, sbyte * pText);
void    RCC_UTIL_UpdatePrompt(WMB_SESSION_T * pCliEnv);

STATUS  RCC_ReplaceRapidMarkData(WMB_SESSION_T * pCliEnv, sbyte * pSrc,
                                 sbyte * pDest, sbyte4 bufLen);

BOOL RCC_AUTH_DefaultCallback(OS_SOCKET_T sock, sbyte *login, sbyte *password, Access *AccLvl );

#ifdef __cplusplus
}
#endif


#endif /* __RCC_UTIL_H__ */

/*  rcc_alias.h : Supports CLI aliasing  */

/*  Copyright (C) 1999-2004 Wind River. */

/*
modification history
--------------------
12oct05,wdz      changed 'C' to 'extern "C"'
                 for SPR#98444
01a,20jan04,gsr  support for alias SPR 83988
*/


#ifndef __RCC_ALIAS_HEADER__
#define __RCC_ALIAS_HEADER__

#define kSTRING_DELIM   "\""
#define kSPACECHAR      " "

#define kDoubleQuote '\"'
#define kSingleQuote '\''
#define kNoQuote     '\0'
#define kSpaceChar   ' '
#define kBackSlash   '\\'

#ifdef __cplusplus
extern "C" {
#endif

STATUS  RCC_ALIAS_AliasDump(WMB_SESSION_T * pCliEnv, sbyte *pDumpBuffer, sbyte4 bufferSize);
STATUS  RCC_ALIAS_AliasRestore(WMB_SESSION_T * pCliEnv, sbyte *pDumpBuffer, sbyte4 bufferSize);
void    RCC_ALIAS_Apply(WMB_SESSION_T *  pCliEnv, sbyte *pBuffer);
STATUS  RCC_ALIAS_CreateAlias(WMB_SESSION_T * pCliEnv, sbyte * pAliasName, sbyte * pAliasText);
STATUS  RCC_ALIAS_DeleteAlias(WMB_SESSION_T * pCliEnv, sbyte * pAliasName);
void    RCC_ALIAS_DisplayAliases(WMB_SESSION_T * pCliEnv);

#ifdef __cplusplus
}
#endif

#ifdef __NO_ALIASING__
#define RCC_ALIAS_CreateAlias(x,y,z)    OK
#define RCC_ALIAS_DeleteAlias(x, y)     OK
#define RCC_ALIAS_DisplayAliases(x)
#endif

#endif /* __RCC_ALIAS_HEADER__ */

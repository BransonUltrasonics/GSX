/* rcc_db.h - rcc_db.c related defines */

/*
 * Copyright (c) 2003-2004, 2006-2007, 2011-2012 Wind River Systems, Inc.
  *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01h,04may12,lan  add prototypes for DB_ErrorAccessDeny (CQ:WIND00346995)
01g,27mar12,lan  add prototypes for DB_ErrorStringTooLong (WIND00337945)
01f,18jan12,f_f  add prototypes for RCC_DB_OrderedIndex
01e,05jan12,r_w  remove gDebugFlags variable 
                 CQID:Fix WIND00321029
01d,27dec11,r_w  if there is no matches,do not show anyting
                 CQID:Fix WIND00325478
01c.29jun11,r_w  Add prototypes for RCC_DB_PrintSubRange
                 CQID: defect WIND00272292				 
01b,02apr07,msa  Add prototypes for DB_ErrorType and Range.
01a,07jul06,msa  SPR#119076 (or defect 37786 if you please)
12oct05,wdz      changed 'C' to 'extern "C"'
                 for SPR#98444
01b,20jan04,gsr  support for alias SPR 83988
01a,24feb03,jws  Started mod history.
                 Backed out sixth parameter to RCC_DB_ParamValue
*/


#ifndef __RCC_DB_HEADER__
#define __RCC_DB_HEADER__

#define kSTRING_DELIM   "\""
#define kSPACECHAR      " "

#define kTREE_FLAG_SYNTAX   0x0001

#define kDoubleQuote '\"'
#define kSingleQuote '\''
#define kNoQuote     '\0'
#define kSpaceChar   ' '
#define kBackSlash   '\\'

#ifdef __RCC_DEBUG__
BOOL debugFeature      (ubyte4 feature);
void    debugFeatureSet   (ubyte4 feature);
void    debugFeatureClear (ubyte4 feature);
void    debugExecShow     (cmdNode * pNode);
#else
#define debugFeature(feature)       FALSE
#define debugFeatureSet(feature)
#define debugFeatureClear(feature)
#define debugExecShow(pNode)
#endif /* __RCC_DEBUG__ */

#ifdef __cplusplus
extern "C" {
#endif

STATUS  RCC_DB_AccessValidate(WMB_SESSION_T * pCliEnv, sbyte *pString,
                                      DTTypeInfo *pInfo, BOOL errMsg);
void    RCC_DB_AppendErrorText(WMB_SESSION_T * pCliEnv, sbyte *pText,
                                       sbyte4 length);
STATUS  RCC_DB_AssignFrom(WMB_SESSION_T * pCliEnv, sbyte *pInBuf, sbyte4 size);
sbyte * RCC_DB_BadParam(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_BuildParamList(WMB_SESSION_T * pCliEnv, BOOL exec);
STATUS  RCC_DB_CommandComplete(WMB_SESSION_T * pCliEnv);
sbyte4  RCC_DB_Count(WMB_SESSION_T * pCliEnv);
handlerDefn * RCC_DB_CurrentHandler(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_EnvironmentCreate(WMB_SESSION_T * *ppCliEnv, OS_SOCKET_T sock,
                                         sbyte *pBuffer, sbyte4 size);
void    RCC_DB_EnvironmentDestroy(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_EnvironmentCopy(WMB_SESSION_T * pSrc, WMB_SESSION_T * * ppDest);
STATUS  RCC_DB_EnvironmentReset(WMB_SESSION_T * pCliEnv);
void    RCC_DB_ErrorLine(WMB_SESSION_T * pCliEnv);
BOOL 	RCC_DB_ErrorOnLastToken (WMB_SESSION_T * pCliEnv);
void    RCC_DB_ExecReset(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_Exec (WMB_SESSION_T * pCliEnv, sbyte *pBuffer, sbyte4 bufferLen, BOOL echo);
STATUS  RCC_DB_ExecFile(WMB_SESSION_T * pCliEnv, sbyte *pFileName, BOOL echo);
STATUS  RCC_DB_ExecRM(WMB_SESSION_T * pCliEnv, sbyte *pRapidMark, BOOL echo);

STATUS  RCC_DB_Execute(sbyte *pCmd, sbyte *pOutput,
                               sbyte4 outputBuffSize, sbyte4 *outputSize);
STATUS  RCC_DB_ExecuteCommand(WMB_SESSION_T * pCliEnv, sbyte *pCmd);
STATUS  RCC_DB_Exit(WMB_SESSION_T * pCliEnv, BOOL exitAll);
STATUS  RCC_DB_ExpandToken(WMB_SESSION_T * pCliEnv, BOOL sameLine);
void    RCC_DB_FreeTasks(void);
sbyte * RCC_DB_FullEnum(paramDefn *pParam, sbyte *pData);
sbyte4  RCC_DB_GetArraySize(paramDescr *pParamDesc);
sbyte * RCC_DB_GetArrayElement(paramList  *pParamList,
                                       paramDescr *pParamDesc, sbyte4 offset);
cmdNode * RCC_DB_GetCommandNode(WMB_SESSION_T * pCliEnv);
paramDescr * RCC_DB_GetParamDesc(paramList * pList, sbyte4 offset);
sbyte4  RCC_DB_GetParamCount(WMB_SESSION_T * pCliEnv, cmdNode *pNode);
cmdNode * RCC_DB_GetParentNode(WMB_SESSION_T * pCliEnv, cmdNode *pNode);
cmdNode * RCC_DB_GetRootNode(void);
paramDefn * RCC_DB_GetNextParam(WMB_SESSION_T * pCliEnv, sbyte4 *pOffset);
BOOL    RCC_DB_HandlerOk(WMB_SESSION_T * pCliEnv);
HistInfo * RCC_DB_History(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_InitTasks(void);
void    RCC_DB_InvalidParam(WMB_SESSION_T * pCliEnv, BitMask paramMask);
STATUS  RCC_DB_Invert(paramList * pParams);
BOOL    RCC_DB_IsAssigned(WMB_SESSION_T * pCliEnv, void *item);
BOOL    RCC_DB_IsNoCommand(paramList  *pParamList);
BOOL    RCC_DB_IsNo(WMB_SESSION_T * pCliEnv);
paramDefn * RCC_DB_IncompleteParam(WMB_SESSION_T * pCliEnv);
sbyte * RCC_DB_LastToken(WMB_SESSION_T * pCliEnv);
BOOL    RCC_DB_LeafOnly(WMB_SESSION_T * pCliEnv);
paramDefn * RCC_DB_OrderedParam(WMB_SESSION_T * pCliEnv, sbyte4 index);
STATUS  RCC_DB_OrderedIndex(WMB_SESSION_T *pCliEnv, sbyte4 paramNum, sbyte4 *orderedIndex);
STATUS  RCC_DB_Parameterize(WMB_SESSION_T * pCliEnv, BOOL final);
BOOL    RCC_DB_ParamAccess(WMB_SESSION_T * pCliEnv, paramDefn *pParam);
STATUS  RCC_DB_ParamValue(WMB_SESSION_T * pCliEnv, BitMask bitmask,
                                  sbyte *pArgs, sbyte *pRapidMark, void * pOutput);
BOOL	RCC_DB_ParamPossible(WMB_SESSION_T * pCliEnv, cmdNode *pNode, paramDefn *pParam);
BOOL    RCC_DB_ParamRequired(WMB_SESSION_T * pCliEnv, paramDefn *pParam);
STATUS  RCC_DB_ParseCmdString(WMB_SESSION_T * pCliEnv);

BOOL    RCC_DB_ParseFlag(WMB_SESSION_T * pCliEnv, ubyte4 flag);
BOOL    RCC_DB_Possibilities(WMB_SESSION_T * pCliEnv);
void    RCC_DB_PrintType(WMB_SESSION_T * pCliEnv, paramDefn *pParam);
void    RCC_DB_PrintSubRange(WMB_SESSION_T * pCliEnv, paramDefn *pParam);
void    RCC_DB_PrintRM(WMB_SESSION_T * pCliEnv, sbyte *pRapidMark);
STATUS  RCC_DB_Process_CLI(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_ReadPipe(WMB_SESSION_T * pCliEnv, sbyte *pBuffer,
                                sbyte4 buffSize, sbyte4 *pWritten);
STATUS  RCC_DB_RetrieveParam(paramList *pParamList, sbyte *pKeyword,
                                     paramID id, paramDescr **ppParamDescr);
sbyte * RCC_DB_Rvalue(WMB_SESSION_T * pCliEnv, paramList *pParamList);
void    RCC_DB_SetCommand(WMB_SESSION_T * pCliEnv, sbyte *pString, sbyte4 length);
void    RCC_DB_SetErrorPos(WMB_SESSION_T * pCliEnv, sbyte4 position);
void    RCC_DB_ShowCommand(WMB_SESSION_T * pCliEnv, cmdNode *pNode);
void    RCC_DB_ShowNode(WMB_SESSION_T * pCliEnv);
void    RCC_DB_ShowInput(WMB_SESSION_T * pCliEnv, BOOL showPrompt);
void    RCC_DB_ShowParameters(WMB_SESSION_T * pCliEnv);
void    RCC_DB_ShowParentNodes(WMB_SESSION_T * pCliEnv);
void    RCC_DB_ShowTree(WMB_SESSION_T * pCliEnv, cmdNode *pNode,
                                sbyte4 depth, ubyte4 mask, ubyte4 flags);
STATUS  RCC_DB_Syntax(WMB_SESSION_T * pCliEnv);
void    RCC_DB_SystemSettings(WMB_SESSION_T * pCliEnv);
STATUS  RCC_DB_SystemOrder(WMB_SESSION_T * pCliEnv, ubyte4 order);
BOOL    RCC_DB_PathValid(WMB_SESSION_T * pCliEnv, sbyte * pPath);
STATUS  RCC_DB_VarDelete(WMB_SESSION_T * pCliEnv, sbyte * pName);
void    RCC_DB_VarDump(WMB_SESSION_T * pCliEnv);
sbyte * RCC_DB_VarGet(WMB_SESSION_T * pCliEnv, sbyte * pName);
STATUS  RCC_DB_VarSet(WMB_SESSION_T * pCliEnv, sbyte * pName,
                              sbyte * pValue, sbyte4 length);
void    DB_ErrorType (WMB_SESSION_T *   pCliEnv, paramDefn * pParam);
void    DB_ErrorRange (WMB_SESSION_T *   pCliEnv, paramDefn * pParam);
void    DB_ErrorStringTooLong(WMB_SESSION_T *   pCliEnv, paramDefn * pParam);
void    DB_ErrorAccessDeny(WMB_SESSION_T *   pCliEnv, paramDefn * pParam);

#ifdef __cplusplus
}
#endif

/* remap old calls */

#define RCC_DB_CreateAlias          RCC_ALIAS_CreateAlias
#define RCC_DB_DeleteAlias          RCC_ALIAS_DeleteAlias
#define RCC_DB_DisplayAliases       RCC_ALIAS_DisplayAliases
#define RCC_DB_DestroyEnvironment   RCC_DB_EnvironmentDestroy
#define RCC_DB_InitEnvironment      RCC_DB_EnvironmentCreate

#endif /* __RCC_DB_HEADER__ */


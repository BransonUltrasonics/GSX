/* wmcLib.h - WindManage CLI Library header */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,27apr04,dlr  removed define for CLI_CONSOLE_ENABLED_K
01a,20feb04,jws  added copyright and mod history.
*/

#ifndef __INCwmcLibh
#define __INCwmcLibh

#ifndef WMC_PORTFOLIO_K
#define WMC_PORTFOLIO_K NULL
#endif

typedef struct CLI_MEM_T
    {
    cmdNode *       pCmdNode;
    paramDefn *     pParam;
    handlerDefn *   pHandler;
    paramEntry *    pEntry;
    sbyte *         pStrings;
    }
    CLI_MEM_T;


#ifdef __cplusplus
extern "C" {
#endif

STATUS  wmcDefaultHandler (WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T command);

void    wmcDeregisterSelf (void);

void    wmcListener (WMB_EVENT_T * pEvent);

STATUS  cliTreeAdd (WMB_ID_T backplane, WMB_CID_T component,
                    cmdNode * pRootCmdNode, CLI_MEM_T * pMem, sbyte * pPath);

STATUS  cliTreeDelete (WMB_ID_T backplane, WMB_CID_T component, sbyte * pPath);

STATUS  wmcCommandHandler (WMB_TID_T tid, WMB_OBJ_T obj, COMMAND_T command);

WMB_CID_T wmcComponentGet(void);

void    wmcComponentSet(WMB_CID_T component);

STATUS  wmcShutdown (void * pData);

cmdNode * cliSibling(WMB_SESSION_T * pSession, sbyte4 index);

STATUS  wmcPortfolioLoad (WMB_ID_T backplane, WMB_CID_T component, sbyte * pFileName);

STATUS  cliStart
    (
    sbyte *         pBackplane,
    sbyte *         pName,
    cmdNode *       pRootCmdNode,
    WMB_ENTRY_T *   pHandlers,
    sbyte4          handlerCount,
    ubyte2          port,
    sbyte4          timeout,
    sbyte4          tasks,
    sbyte4          stackSize,
    sbyte4          priority,
    BOOL            console
    );

#define wmcStartup  cliStart

STATUS cliTelnet
    (
    WMB_CID_T   component,  /* component registered for CLI */
    ubyte2      port,       /* port to listen to            */
    sbyte4      timeOut,    /* timeout in seconds           */
    sbyte4      maxTasks,   /* maximum number of sessions   */
    sbyte4      stackSize,  /* stack size for each session  */
    ubyte       priority    /* default session priority     */
    );

STATUS wmcRMGet
    (
    WMB_SESSION_T * pSession,
    paramList *     pParamList,
    paramDescr  *   pParamDescr
    );

STATUS wmcRMSet
    (
    WMB_SESSION_T * pSession,
    paramList *     pParamList,
    paramDescr  *   pParamDescr
    );

cmdNode * wmcRootNodeGet(void);

#ifdef __cplusplus
}
#endif




#endif /* __INCwmcLibh */

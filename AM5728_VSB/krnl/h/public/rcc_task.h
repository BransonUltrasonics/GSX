/* rcc_task.h */

/* 
 * Copyright 2004, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,30nov12,f_f  add the prototype of RCC_TASK_CancelRead (REQ: WIND00364534) 
01c,22may12,shi  support cli stdout redirection. (req:WIND00270599)
01b,16may12,f_f  support interrupting the CLI command with escape sequence
01a,20feb04,jws  added copyright and mod history.
*/

#ifndef __RCC_TASK_H__
#define __RCC_TASK_H__

#define TASK_NONE_K     0x0000
#define TASK_RETRY_K    0x0001
#define TASK_NOMSG_K    0x0002

typedef struct ErrorTable
    {
    ubyte4    flags;
    STATUS    status;
    sbyte *   pErrorText;
    } ErrorTable;


#ifdef __cplusplus
extern "C" {
#endif

void    RCC_TASK_Cleanup(WMB_SESSION_T * pCliEnv);
BOOL    RCC_TASK_LineEval(WMB_SESSION_T * pCliEnv, BOOL haveInput);
void    RCC_TASK_PrintPrompt(WMB_SESSION_T * pCliEnv);
void    RCC_TASK_PrintError(WMB_SESSION_T * pCliEnv, STATUS status);
void    RCC_TASK_Readline(WMB_SESSION_T * pCliEnv);
STATUS  RCC_TASK_Shell(WMB_SESSION_T * pCliEnv);
BOOL    RCC_TASK_ValidateLogin(OS_SOCKET_T sock, sbyte *login,
                                sbyte *password, Access *AccLvl);
#ifndef __DISABLE_CLI_INTERRUPT__
STATUS  RCC_TASK_ExecHandler(WMB_SESSION_T *pCliEnv, handlerDefn *pHandler,
                            paramList *pParams, sbyte *pAuxBuf, intExecTypes type);
void    RCC_TASK_CancelRead(WMB_SESSION_T *   pCliEnv);
#endif

#ifdef  __RCC_STDIO_REDIRECTION__
sbyte4 	RCC_TASK_Redirect(OS_SOCKET_T sock, BOOL isEolOpen);
#endif

#ifdef __cplusplus
}
#endif

#define RCC_TASK_EvalError  RCC_SYSTEM_EvalError

#endif /* __RCC_TASK_H__ */

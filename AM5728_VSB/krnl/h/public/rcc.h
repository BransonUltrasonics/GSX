/* rcc.h */

/*
 * Copyright (c) 2004, 2008, 2012-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
27may14,r_w  Eliminate dependency for V7MAN-62
16may13,shi  add declaration for eliminating build warning
                 (WIND00416628)
22may12,shi  support cli stdout redirection. (req:WIND00270599)
16may12,f_f  support interrupting the CLI command with escape sequence
17dec08,m_z  dynamically set cli session max number when initial
                 (WIND00127186)
20feb04,jws  added copyright and mod history.
*/

#define __WINDMANAGE__
#ifdef __WINDMANAGE__
#define __DISABLE_STRUCTURES__
#include "cliCfg.h"
#define cli_env     WMB_SESSION_T
#define environment WMB_SESSION_T
#else

#ifndef __VXWORKS_OS__
typedef void *  (*FUNCPTR)(void *);
typedef void    (*VOIDFUNCPTR)(void *);
#endif

#endif /* __WINDMANAGE__ */



#ifndef __RCC_H__
#define __RCC_H__

#include <stdio.h>

#ifdef __SNMP_API_ENABLED__
#ifdef __ENVOY_STACK__
#include "rcm_envoy.h"
#include "rcm_ev_cnv.h"
#include "rcm_snmp.h"
#endif
#endif /* __SNMP_API_ENABLED__ */

#include "rcc_custom.h"
#include "rcc_structs.h"
#include "rcc_cmd.h"
#include "rcc_error.h"
#include "rcc_hist.h"
#include "rcc_telnetd.h"
#include "rcc_db.h"
#include "rcc_ext.h"
/*
#include "rcc_rcb.h"
*/
#include "rcc_task.h"
#include "rcc_telnet.h"
#include "rcc_util.h"
#include "rcc_help.h"
#include "rcc_alias.h"

#ifdef __WINDMANAGE__
#include "wmcLib.h"
#endif

#ifdef __RCC_DEBUG__
#include "rcc_log.h"
#define RCC_ERROR_THROW(status)     RCC_UTIL_ErrorThrow(status)
#else
#define RCC_ERROR_THROW(status)     status
#endif

#ifdef RCC_STATUS_LINE_FN
void    RCC_STATUS_LINE_FN(WMB_SESSION_T *pCliEnv);
#else
#define RCC_STATUS_LINE_FN(pCliEnv)
#endif

#define STDIN 0

#ifndef CONSOLE_SESSION_FN
#define CONSOLE_SESSION_FN  RCC_TELNETD_ConsoleSession
#endif

#ifndef TELNET_SEND_FN
#define TELNET_SEND_FN      RCC_TELNET_Send
#endif

#ifndef TELNET_RECV_FN
#define TELNET_RECV_FN      RCC_TELNET_Recv
#endif

#ifdef __RCC_PARAMETER_ORDER_NONE__
#define HANDLER_PARAMS(x)      NULL
#define HANDLER_PARAM_COUNT(x) 0
#else
#define HANDLER_PARAMS(x)      x->pParams
#define HANDLER_PARAM_COUNT(x) x->paramCount
#endif

#ifdef __RCC_DEBUG__
#define RCC_ASSERT(x)   \
if (FALSE==(x)) printf("Assertion failed. File:%s Line:%d\n", __FILE__, __LINE__)
#else
#define RCC_ASSERT(x)
#endif

#ifndef CLI_SESSION_AUTH
#define CLI_SESSION_AUTH RCC_UTIL_SampleAuth
#endif

#if defined(__CLI_SCRIPTING__) && defined(__RCC_PARAMETER_ORDER_NONE__)
#error Scripting support requires parameter ordering
#endif

/* prefixes for rapidmarks and session variables */

#ifdef  kRCC_RM_PREFIX
#define IS_RM_PREFIX(x) (NULL_STRING(x) ? FALSE : (kRCC_RM_PREFIX == *x))
#else
#define IS_RM_PREFIX(x) FALSE
#endif

#ifdef  kRCC_VAR_PREFIX
#define IS_VAR_PREFIX(x) (NULL_STRING(x) ? FALSE : (kRCC_VAR_PREFIX == *x))
#else
#define IS_VAR_PREFIX(x) FALSE
#endif

#ifdef __RCC_CASE_INSENSITIVE__
#define COMPARE  STRICMP
#define NCOMPARE STRNICMP
#else
#define COMPARE  STRCMP
#define NCOMPARE STRNCMP
#endif /* __RCC_CASE_INSENSITIVE__ */

#ifndef CLI_NAME_K
#define CLI_NAME_K "CLI"
#endif

#ifdef  __RCC_STDIO_REDIRECTION__
#ifdef  __FILTER_IO__
#define CLI_EOL_DEV_DIR	"/cliEol"
#endif
#endif

ubyte4   RCC_SYSTEM_EvalError(STATUS status, sbyte **pErrorMsg);
STATUS   RCC_SYSTEM_EditHelp(WMB_SESSION_T *pCliEnv);
BOOL     cliDone(void);

STATUS cliServerStart
    (
    WMB_CID_T           component,      /* component registered for CLI */
    ubyte2              port,           /* port to listen to            */
    sbyte4              timeOut,        /* timeout in seconds           */
    sbyte4              maxTasks,       /* maximum number of sessions   */
    sbyte4              stackSize,      /* stack size for each session  */
    ubyte               priority        /* default session priority     */
    );

STATUS cliConsoleStart(void);
STATUS cliConsoleStop(void);
BOOL cliConsoleStatus(void);

#ifndef __DISABLE_CLI_INTERRUPT__
STATUS cliIntExecCreate(cliIntExec **pNewCliIntExec);
STATUS cliIntExecDestroy(cliIntExec *pCliIntExec);
#endif

#endif /* __RCC_H__       */


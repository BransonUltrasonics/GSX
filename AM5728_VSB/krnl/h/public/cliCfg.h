/* cliCfg.h */

/* Copyright(c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
29apr15,f_f  disable the paged output with HV defined (V7MAN-151)
20jun14,kaj  modified config params to match changes made in common.vxconfig
04jun14,h_l  expand possible thread stack size range (V7MAN-85)
29may14,f_f  add SKIP_LOGIN config property (V7MAN-68)
28may14,f_f  add FILTER_IO and INTERRUPT_EXEC config properties (US35920)
26may14,r_w  written
*/

#ifndef __CLI_CFG_H_HEADER__
#define __CLI_CFG_H_HEADER__

#include <vsbConfig.h>
#include "commonCfg.h"

#undef kRCC_MAX_LOGIN_LEN
#undef kRCC_MAX_PASSWORD_LEN
#undef kRCC_MAX_PROMPT_LEN
#undef kRCC_ALIAS_BUFFER_SIZE
#undef kRCC_ALIAS_COUNT
#undef kRCC_MAX_CMD_LEN
#undef kOUTPUT_BUFFER_SIZE
#undef __SSH_ONLY__
#undef __SSH_LOGIN__
#undef __RCC_PARAMETER_ORDER_NONE__
#undef __RCC_PARAMETER_ORDER_UNNAMED__
#undef __RCC_PARAMETER_ORDER_FULL__
#undef __RCC_SERIAL_CONSOLE__
#undef __RCC_STDIO_REDIRECTION__
#undef __CLI_SCRIPTING__
#undef __DISABLE_PAGED_OUTPUT__

#ifndef _WRS_CONFIG_WEBCLI_RCC_MAX_LOGIN_LEN
#define  kRCC_MAX_LOGIN_LEN 16
#else
#define  kRCC_MAX_LOGIN_LEN _WRS_CONFIG_WEBCLI_RCC_MAX_LOGIN_LEN
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_MAX_PASSWORD_LEN
#define  kRCC_MAX_PASSWORD_LEN 16
#else
#define  kRCC_MAX_PASSWORD_LEN _WRS_CONFIG_WEBCLI_RCC_MAX_PASSWORD_LEN
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_MAX_PROMPT_LEN
#define  kRCC_MAX_PROMPT_LEN 64
#else
#define  kRCC_MAX_PROMPT_LEN _WRS_CONFIG_WEBCLI_RCC_MAX_PROMPT_LEN
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_ALIAS_BUFFER_SIZE
#define  kRCC_ALIAS_BUFFER_SIZE 1024
#else
#define  kRCC_ALIAS_BUFFER_SIZE _WRS_CONFIG_WEBCLI_RCC_ALIAS_BUFFER_SIZE
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_ALIAS_COUNT
#define  kRCC_ALIAS_COUNT 3
#else
#define  kRCC_ALIAS_COUNT _WRS_CONFIG_WEBCLI_RCC_ALIAS_COUNT
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_MAX_CMD_LEN
#define  kRCC_MAX_CMD_LEN 256
#else
#define  kRCC_MAX_CMD_LEN _WRS_CONFIG_WEBCLI_RCC_MAX_CMD_LEN
#endif

#define kOUTPUT_BUFFER_SIZE             kMagicMarkupBufferSize

#ifdef _WRS_CONFIG_WEBCLI_TELNET_LOGIN_N
#undef __SSH_ONLY__
#undef __SSH_LOGIN__
#endif

#ifdef _WRS_CONFIG_WEBCLI_SSH_LOGIN_N
#define __SSH_ONLY__
#define __SSH_LOGIN__
#endif

#ifdef _WRS_CONFIG_WEBCLI_TELNET_SSH_LOGIN_N
#undef __SSH_ONLY__
#define __SSH_LOGIN__
#endif

#ifdef _WRS_CONFIG_WEBCLI_RCC_SKIP_LOGIN
#define __SKIP_LOGIN__
#else
#undef __SKIP_LOGIN__
#endif

#ifdef _WRS_CONFIG_WEBCLI_RCC_PARAMETER_ORDER_NONE_N
#define   __RCC_PARAMETER_ORDER_NONE__
#undef  __RCC_PARAMETER_ORDER_UNNAMED__
#undef   __RCC_PARAMETER_ORDER_FULL__
#endif

#ifdef _WRS_CONFIG_WEBCLI_RCC_PARAMETER_ORDER_UNNAMED_N
#undef   __RCC_PARAMETER_ORDER_NONE__
#define  __RCC_PARAMETER_ORDER_UNNAMED__
#undef   __RCC_PARAMETER_ORDER_FULL__
#endif

#ifdef _WRS_CONFIG_WEBCLI_RCC_PARAMETER_ORDER_FULL_N
#undef   __RCC_PARAMETER_ORDER_NONE__
#undef  __RCC_PARAMETER_ORDER_UNNAMED__
#define   __RCC_PARAMETER_ORDER_FULL__
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_SERIAL_CONSOLE
#undef __RCC_SERIAL_CONSOLE__
#else
#define  __RCC_SERIAL_CONSOLE__
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_STDIO_REDIRECTION
#undef   __RCC_STDIO_REDIRECTION__
#else
#define  __RCC_STDIO_REDIRECTION__
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_FILTER_IO
#undef   __FILTER_IO__
#else
#define  __FILTER_IO__
#endif

#ifndef _WRS_CONFIG_WEBCLI_RCC_INTERRUPT_EXEC
#define  __DISABLE_CLI_INTERRUPT__   
#else
#undef  __DISABLE_CLI_INTERRUPT__
#endif

#ifndef _WRS_CONFIG_WEBCLI_CLI_SCRIPTING
#undef __CLI_SCRIPTING__
#else
#define  __CLI_SCRIPTING__
#endif

#ifdef _WRS_CONFIG_HYPERVISOR
#define __DISABLE_PAGED_OUTPUT__
#endif

extern size_t         global_kRCC_THREAD_STACK_SIZE;
extern unsigned short global_kRCC_TIMEOUT;
extern unsigned short kRCC_TASK_MAX;
extern unsigned short global_kRCC_MAX_LOGIN_ATTEMPTS;
extern char *         global_kRCC_LOGIN_PROMPT;
extern char *         global_kRCC_PASSWORD_PROMPT;
extern char *         g_kRCC_DEFAULT_PROMPT;
extern char *         g_kRCC_DEFAULT_PROMPT_TAIL;
extern unsigned short global_kRCC_HISTORY_BUFFER_SIZE;
extern unsigned short   g_HISTORY_MODAL;
extern unsigned short   g_HISTORY_NO_RING_BUFFER;
extern unsigned short   g_CLI_STDIO;
extern unsigned short   g_RCC_CONSOLE_ENABLED;
extern unsigned short   g_EXEC_LEAF_NODE_ONLY;
extern unsigned short   g_NO_INTERMEDIATE_MODE;
extern unsigned short   g_NO_MEANS_NO;
extern unsigned short   g_RCC_DISABLE_RETRIES;
extern unsigned short   g_HIDE_PRIVILEGED_CMD_PARAMS;
extern unsigned short   g_DISABLE_VT_ESCAPES;
extern unsigned short   g_RCC_ENABLE_HARD_WRAP;
extern unsigned short   g_DISABLE_PAGED_OUTPUT;
extern unsigned short   g_RCC_SCROLL_LONG_LINES;
extern unsigned short   g_RCC_DASHED_ERROR_LINE;
extern unsigned short   g_HELP_ALL_PARAMS;
extern unsigned short   g_HELP_DISABLE_FIXED_WIDTH;
extern unsigned short   g_RCC_SHOW_NO;
extern unsigned short   g_RCC_HIDE_BLANK_HELP;
extern unsigned short   g_HELP_MORE_THAN_NEXT_PARAM;
extern unsigned short   g_HELP_SHOWS_EXECUTABLE_NODES;
extern unsigned short   g_HELP_SKIP_NODE;
extern unsigned short   g_ENABLE_THIS_HELP;
extern unsigned short   g_ENABLE_ERROR_HELP;
extern unsigned short   g_HELP_LEAF_SUBNODE;
extern char * g_kRCC_DEFAULT_PATH;
extern char * g_kRCC_AUTOEXEC_FILE;
extern int (*pRCC_AUTH_CALLBACK_FN)(int sock, char *login, char *password, unsigned short *AccLvl);

#endif /* __CLI_CFG_H_HEADER__ */


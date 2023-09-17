/* webCliCliInit.c */

/* Copyright (c) 2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
22may14,r_w  create file
*/

#include "wmc_options.h"
#include "cliCfg.h"
#include "wm.h"
#include "wmb.h"
#include "rcc.h"

void webcli_cli_init(void)
{
 global_kRCC_THREAD_STACK_SIZE = kRCC_THREAD_STACK_SIZE;
 global_kRCC_TIMEOUT = kRCC_TIMEOUT;
 kRCC_TASK_MAX = kRCC_MAX_CLI_TASK; 
 global_kRCC_MAX_LOGIN_ATTEMPTS = kRCC_MAX_LOGIN_ATTEMPTS;
 global_kRCC_LOGIN_PROMPT = kRCC_LOGIN_PROMPT;
 global_kRCC_PASSWORD_PROMPT = kRCC_PASSWORD_PROMPT;
 global_kRCC_HISTORY_BUFFER_SIZE = kRCC_HISTORY_BUFFER_SIZE; 
 global_kRCC_LOGIN_PROMPT = kRCC_LOGIN_PROMPT;
 g_kRCC_DEFAULT_PROMPT_TAIL = kRCC_DEFAULT_PROMPT_TAIL;

#ifdef __HISTORY_MODAL__
 g_HISTORY_MODAL = 1;
#else
 g_HISTORY_MODAL = 0;
#endif

#ifdef __HISTORY_NO_RING_BUFFER__
 g_HISTORY_NO_RING_BUFFER = 1;
#else
 g_HISTORY_NO_RING_BUFFER = 0;
#endif

#ifdef __CLI_STDIO__
 g_CLI_STDIO = 1;
#else
 g_CLI_STDIO = 0;
#endif

#ifdef __RCC_CONSOLE_ENABLED__
 g_RCC_CONSOLE_ENABLED = 1;
#else
 g_RCC_CONSOLE_ENABLED = 0;
#endif


#ifdef __EXEC_LEAF_NODE_ONLY__
 g_EXEC_LEAF_NODE_ONLY = 1;
#else
 g_EXEC_LEAF_NODE_ONLY = 0;
#endif


#ifdef __NO_INTERMEDIATE_MODE__
 g_NO_INTERMEDIATE_MODE = 1;
#else
 g_NO_INTERMEDIATE_MODE = 0;
#endif

#ifdef __HIDE_PRIVILEGED_CMD_PARAMS__
 g_HIDE_PRIVILEGED_CMD_PARAMS = 1;
#else
 g_HIDE_PRIVILEGED_CMD_PARAMS = 0;
#endif

#ifdef __RCC_DISABLE_RETRIES__
 g_RCC_DISABLE_RETRIES = 1;
#else
 g_RCC_DISABLE_RETRIES = 0;
#endif
 
#ifdef __DISABLE_VT_ESCAPES__
 g_DISABLE_VT_ESCAPES = 1;
#else
 g_DISABLE_VT_ESCAPES = 0;
#endif

#ifdef __RCC_ENABLE_HARD_WRAP__
 g_RCC_ENABLE_HARD_WRAP = 1;
#else
 g_RCC_ENABLE_HARD_WRAP = 0;
#endif

#ifdef __DISABLE_PAGED_OUTPUT__
 g_DISABLE_PAGED_OUTPUT = 1;
#else
 g_DISABLE_PAGED_OUTPUT = 0;
#endif

#ifdef __RCC_SCROLL_LONG_LINES__
 g_RCC_SCROLL_LONG_LINES = 1;
#else
 g_RCC_SCROLL_LONG_LINES = 0;
#endif

#ifdef __RCC_DASHED_ERROR_LINE__
 g_RCC_DASHED_ERROR_LINE = 1;
#else
 g_RCC_DASHED_ERROR_LINE = 0;
#endif

#ifdef __HELP_ALL_PARAMS__
 g_HELP_ALL_PARAMS = 1;
#else
 g_HELP_ALL_PARAMS = 0;
#endif

#ifdef __HELP_DISABLE_FIXED_WIDTH__
 g_HELP_DISABLE_FIXED_WIDTH = 1;
#else
 g_HELP_DISABLE_FIXED_WIDTH = 0;
#endif

#ifdef __RCC_SHOW_NO__
 g_RCC_SHOW_NO = 1;
#else
 g_RCC_SHOW_NO = 0;
#endif

#ifdef __RCC_HIDE_BLANK_HELP__
 g_RCC_HIDE_BLANK_HELP = 1;
#else
 g_RCC_HIDE_BLANK_HELP = 0;
#endif

#ifdef __HELP_MORE_THAN_NEXT_PARAM__
 g_HELP_MORE_THAN_NEXT_PARAM = 1;
#else
 g_HELP_MORE_THAN_NEXT_PARAM = 0;
#endif

#ifdef __HELP_SHOWS_EXECUTABLE_NODES__
 g_HELP_SHOWS_EXECUTABLE_NODES = 1;
#else
 g_HELP_SHOWS_EXECUTABLE_NODES = 0;
#endif

#ifdef __ENABLE_THIS_HELP__
 g_ENABLE_THIS_HELP = 1;
#else
 g_ENABLE_THIS_HELP = 0;
#endif

#ifdef __ENABLE_ERROR_HELP__
 g_ENABLE_ERROR_HELP = 1;
#else
 g_ENABLE_ERROR_HELP = 0;
#endif

#ifdef __HELP_LEAF_SUBNODE__
 g_HELP_LEAF_SUBNODE = 1;
#else
 g_HELP_LEAF_SUBNODE = 0;
#endif

#ifdef kRCC_DEFAULT_PATH
 g_kRCC_DEFAULT_PATH = kRCC_DEFAULT_PATH;
#else
 g_kRCC_DEFAULT_PATH = ".";
#endif

#ifdef kRCC_AUTOEXEC_FILE
 g_kRCC_AUTOEXEC_FILE = kRCC_AUTOEXEC_FILE;
#else
 g_kRCC_AUTOEXEC_FILE = NULL;
#endif

#ifdef RCC_AUTH_CALLBACK_FN
 pRCC_AUTH_CALLBACK_FN = RCC_AUTH_CALLBACK_FN;
#else
 pRCC_AUTH_CALLBACK_FN = NULL;
#endif
}


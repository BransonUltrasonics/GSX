/* rcc_custom.h */

/* Copyright (c) 2004-2005, 2012 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,12jan12,r_w  add '. ' right after "String too long". CQID: WIND00328015
01d,06jan12,shi  allow some macros can be defined in elsewhere. Defect:WIND00148071
01c,21mar05,gsr  change timeout message from minutes to seconds SPR#106558
01b,05may04,gsr  added text messages for SNMP errors SPR 90299
01a,20feb04,jws  added copyright and mod history.
*/

/*
 *  should eventually be generated in rc_options.h
 */

#ifndef __RCC_CUSTOM_HEADER__
#define __RCC_CUSTOM_HEADER__

/* various system options */
#define kRCC_BRANCH_SIZE            256     /* max width of "tree" display      */
#define kRCC_CHAR_COMMENT           '#'     /* ignore all text after this       */
#define kRCC_CMD_BANG               '!'     /* history with optional index      */
#define kRCC_CMD_NO                 "no"    /* "no" command                     */
#define kRCC_CMD_ASSIGN             "="     /* scripting assignment operator    */
#define kRCC_COL_SEPARATOR          "\t"    /* used by RCC_RCB_PrintTable       */
#define kRCC_CONSOLE_NAME           "Console"

#ifndef kRCC_DEFAULT_ACCESS
#define kRCC_DEFAULT_ACCESS         "10"    /* default access level for dev.    */
#endif

#define kRCC_DEFAULT_HEIGHT         25      /* default assumed screen height    */

#ifndef kRCC_DEFAULT_LOGIN
#define kRCC_DEFAULT_LOGIN          "admin" /* default login for development    */
#endif
#ifndef kRCC_DEFAULT_PASSWORD
#define kRCC_DEFAULT_PASSWORD       "admin" /* default password for development */
#endif
#ifndef kRCC_DEFAULT_PATH
#define kRCC_DEFAULT_PATH           "."     /* default path for exec'ing files  */
#endif

#ifndef kRCC_DEFAULT_PROMPT_TAIL
#define kRCC_DEFAULT_PROMPT_TAIL    "# "    /* always appended to prompt        */
#endif

#define kRCC_DEFAULT_WIDTH          80      /* default assumed screen width     */
#define kRCC_ERROR_TEXT_SIZE        64
#define kRCC_HELP_DELIMITER         " - "   /* put between cmd and help         */
#define kRCC_HELP_KEYS_WIDTH        45      /* tab stop for edit keys labels    */
#define kRCC_HELP_PREFIX            " "     /* precedes help text               */
#define kRCC_HELP_SYNTAX            "Syntax: "
/*
#define kRCC_HELP_TITLE " configuration commands:"
*/
#define kRCC_HELP_WIDTH              20     /* width of help item area          */
#define kRCC_INTERMEDIATE_PROMPT    ">"     /* after line continuation char     */
#define kRCC_MATCH_COL_WIDTH        20      /* column width for display matches */
#define kRCC_MATCH_LIST_SIZE        20      /* maximum number of token matches  */
#define kRCC_MAX_LOG_STRING         64
#define kRCC_MAX_OPT_HANDLED        32      /* buffer for telnet options        */
#define kRCC_MAX_PROMPT_TAIL_LEN    8       /* max size of tail-end of prompt   */
#define kRCC_MORE_LINE             '\r'     /* character to skip line           */
#define kRCC_MORE_CANCEL           'Q'      /* character to cancel printing     */
#define kRCC_MORE_TEXT              "Press any key to continue (Q to quit)"
#define kRCC_OPT_BUF_SIZE           40      /* buffer for telnet suboptions     */
#define kRCC_PARAM_BRACKETS         "<>"    /* displaying parameters            */
#define kRCC_PARAM_LEFT_BRACKET     "<"     /* displaying parameters            */
#define kRCC_PARAM_RIGHT_BRACKET    ">"     /* displaying parameters            */
#define kRCC_PROMPT_DELIMITER       "-"     /* separator in intermediate mode   */
#define kRCC_ROW_SEPARATOR          "\r\n"  /* used by RCC_RCB_PrintTable       */
#define kRCC_TASK_STACK_SIZE        8       /* depth of command queueing stack  */
#define kRCC_TERM_TYPE_SIZE         16      /* size for storing terminal name   */
#define kRCC_ENUM_MSG_SIZE          128     /* buffer size for displaying enum  */
#if 0
#define kRCC_IGNORE_CHARS           "{}="   /* treat as white space             */
#define kRCC_MOTD                   "motd"  /* RapidMark - message of the day   */
#endif

#define kRCC_RM_PREFIX              '%'     /* treat parameter as WindMark      */
#define kRCC_VAR_PREFIX             '$'     /* treat item as session variable   */

/* help and error messages */

#define kRCC_MSG_CHILDHELP    "Possible children:"
#define kRCC_MSG_COMMANDS     "\r\nAvailable internal commands:\r\n"
#define kRCC_MSG_DEPENDENCIES "Dependencies:"
#define kRCC_MSG_DISABLED	  "Disabled"
#define kRCC_MSG_ENABLED	  "Enabled"
#define kRCC_MSG_ERROR_PREFIX "Error: "
#define kRCC_MSG_FAIL         "Error: unable to open session"
#define kRCC_MSG_KEYSTROKES   "\r\nAvailable editing keystrokes\r\n"
#define kRCC_MSG_LOGIN_FAILED " Login failed for: "
#define kRCC_MSG_LOGIN_OKAY   " Login succeded for: "
#define kRCC_MSG_LOGOUT_IDLE  "\r\nThis terminal has been idle for %d seconds.\r\n"
#define kRCC_MSG_LOGOUT_NOW   "Log out by the system"
#define kRCC_MSG_LOGOUT_LEFT  "It will be logged out if it remains idle for another %d seconds.\r\n"
#define kRCC_MSG_NO_HELP      "No help available for: "
#define kRCC_MSG_NO_INSTANCE  "Could not retrieve instance of: "
#define kRCC_MSG_NO_PARAM     "Expected mandatory parameter missing"
#define kRCC_MSG_NO_STRUCTS   "Structures need to be enabled to make this work"
#define kRCC_MSG_NULL_PARAM   "Parameter is null!"
#define kRCC_MSG_PARAMS_AVAIL "Available parameters:"
#define kRCC_MSG_SNMPAUTO     "SNMP auto commits: "
#define kRCC_MSG_TYPE         " Must be of type: "
#define kRCC_MSG_VALID_FROM   "Valid range is from: "
#define kRCC_MSG_VALID_TO     " - "
#define kRCC_MSG_BROADCAST    "Broadcast message from "

/* error message text */
#define kMSG_ERROR_RCC_INVALID_PARAM      "Invalid parameter. "
#define kMSG_ERROR_RCC_INVALID_VALUE      "Invalid value"
#define kMSG_ERROR_RCC_AMBIGUOUS_PARAM    "Ambiguous parameter"
#define kMSG_ERROR_RCC_AMBIGUOUS_COMMAND  "Ambiguous command"
#define kMSG_ERROR_RCC_NO_PARAM_DATA      "Missing parameter data"
#define kMSG_ERROR_RCC_MISSING_PARAM      "Missing parameter"
#define kMSG_ERROR_RCC_DEFAULT            "Bad command"
#define kMSG_ERROR_RCC_BAD_COMMAND        "Bad command"
#define kMSG_ERROR_RCC_NO_HANDLER         "Incomplete command"
#define kMSG_ERROR_RCC_INVALID_USER       "Unknown user"
#define kMSG_ERROR_RCC_NO_HELP            "No help available"
#define kMSG_ERROR_RCC_INVALID_HISTORY    "Event not found"
#define kMSG_ERROR_GENERAL_ACCESS_DENIED  "Access denied"
#define kMSG_ERROR_GENERAL_DATA_AMBIG     "Ambiguous data"
#define kMSG_ERROR_CONVERSION_TOO_LONG    "String too long. "
#define kMSG_ERROR_INVALID_RAPIDMARK      "Invalid RapidMark: "
#define kMSG_ERROR_GENERAL_OUT_OF_RANGE   "Out of range. Valid range is: "
#define kMSG_ERROR_GENERAL_FILE_NOT_FOUND "File not found"
#define kMSG_ERROR_RCC_INVALID_NO         "'no' is not applicable"
#define kMSG_ERROR_GENERAL_NOT_FOUND      "Not found: "
#define kMSG_ERROR_RCC_EXTRA_PARAMS       "Too many parameters"
#define kMSG_ERROR_RCC_BAD_PARAM_ORDER    "Invalid parameter order"
#define kMSG_ERROR_GENERAL_INVALID_PATH   "Invalid directory"
#define kMSG_ERROR_RCC_KEYWORD_AS_PARAM   "Command names may not be used as arguments"
#define kMSG_ERROR_RCC_CUSTOM_RETRY        NULL
#define kMSG_ERROR_RCC_CUSTOM_NO_RETRY     NULL
#define kMSG_ERROR_RCC_NO_CHAIN           "Command chaining not allowed"
#define kMSG_ERROR_RCC_MISSING_NO         "This command uses the \"no\" prefix"
#define kMSG_ERROR_RCC_MAX_ALIASES        "Can not add alias -- table full"

#define kMSG_ERROR_GENERAL_NO_ACCESS              "No Access"
#define kMSG_ERROR_GENERAL_INCONSISTENT_VALUE     "Inconsistent value"
#define kMSG_ERROR_GENERAL_INCONSISTENT_NAME      "Inconsistent name"
#define kMSG_ERROR_GENERAL_RESOURCE_UNAVAILABLE   "Resource unavailable"
#define kMSG_ERROR_GENERAL_WRONG_VALUE            "Wrong value"
#define kMSG_ERROR_GENERAL_NO_CREATION            "No creation"
#define kMSG_ERROR_GENERAL_COMMIT_FAILED          "Commit failed"
#define kMSG_ERROR_GENERAL_UNDO_FAILED            "Undo failed"
#define kMSG_ERROR_GENERAL_WRONG_LENGTH           "Wrong length"
#define kMSG_ERROR_GENERAL_TOO_BIG                "Too big"
#define kMSG_ERROR_GENERAL_NO_SUCH_NAME           "No such name"
#define kMSG_ERROR_GENERAL_BAD_VALUE              "Bad value"
#define kMSG_ERROR_GENERAL_READ_ONLY              "Read only"
#define kMSG_ERROR_GENERAL_WRONG_TYPE             "Wrong type"
#define kMSG_ERROR_GENERAL_WRONG_ENCODING         "Wrong encoding"
#define kMSG_ERROR_GENERAL_AUTHORIZATION_ERROR    "Authorization error"
#define kMSG_ERROR_GENERAL_NOT_WRITABLE           "Not writable"




#endif /* __RCC_CUSTOM_HEADER__ */

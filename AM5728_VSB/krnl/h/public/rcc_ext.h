/* rcc_ext.h */

/*
 * Copyright (c) 2004, 2005, 2007, 2012-2013  Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,15aug13,shi  add flag kRCC_FLAG_REALTIME_INPUTECHO (WIND00429285)
01e,22nov12,f_f  defined the new control key kKEY_ESCAPE (REQ: WIND00364534)
01d,19apr12,lan  new flag kRCC_SPCH_REPLACEVAR  (REQ:WIND00303792)
01c,12dec07,pas  New flag kRCC_TERM_SSH
01b,02oct05,wdz  changed 'C' to 'extern "C"' for SPR#98444
01a,20feb04,jws  added copyright and mod history.
*/

#ifndef __RCC_EXT_H__
#define __RCC_EXT_H__

/* terminal settings flags */

#define kRCC_TERM_FILE          0x00000001 /* output to file                    */
#define kRCC_TERM_ECHO          0x00000002 /* enables output                    */
#define kRCC_TERM_STATUS        0x00000004 /* display status line               */
#define kRCC_TERM_RAW           0x00000020 /* output w/o updating screen info   */
#define kRCC_TERM_HARDWRAP      0x00000400 /* use hard wrap in output           */
#define kRCC_TERM_MORE          0x00010000 /* paginate output                   */
#define kRCC_TERM_SCROLL        0x00020000 /* scroll long horizontal lines      */
#define kRCC_TERM_NOPRINT       0x00040000 /* output aborted at "more" prompt   */
#define kRCC_TERM_ASSIGN        0x00080000 /* output is directed to var assign  */
#define kRCC_TERM_DASHES        0x00100000 /* uses dashes in error line         */
#define kRCC_TERM_CONTINUE      0x00200000 /* allow '\' to continue extra lines */
#if defined(__VXWORKS_OS__) && defined(__SSH_LOGIN__)
#define kRCC_TERM_SSH           0x00400000 /* this is an ssh connection over telnet */
#endif

/* System state flags */

#define kRCC_FLAG_NEWMODE       0x00000001 /* entered/exited current mode       */
#define kRCC_FLAG_PRIVILEGED    0x00000002 /* in privileged mode                */
#define kRCC_FLAG_MODE          0x00000004 /* disallow intermediate modes       */
#define kRCC_FLAG_HISTORY       0x00000008 /* capture script to history buffer  */
#define kRCC_FLAG_CONVERT       0x00000010 /* convert escaped chars to value    */
#define kRCC_FLAG_NO_MEANS_NO   0x00000020 /* no cmd requires 'no' handler      */
#define kRCC_FLAG_LEAF          0x00000040 /* execute leaf node only            */
#define kRCC_FLAG_LINES         0x00000080 /* show line number in script        */

#define kRCC_FLAG_SNMPAUTO      0x00000100 /* auto-commit snmp changes          */
#define kRCC_FLAG_RETRIES       0x00000200 /* reprint input w/ cursor at error  */
#define kRCC_FLAG_DASHES        0x00000400 /* dashes - backwards compat KILL */
#define kRCC_FLAG_ALIASED       0x00000800 /* input has alias command - reparse */
#define kRCC_FLAG_MYPROMPT      0x00001000 /* custom prompt is in effect        */
#define kRCC_FLAG_INPUT         0x00002000 /* in input mode (for logging)       */
#define kRCC_FLAG_FINAL         0x00004000 /* parsing is for execution          */
#define kRCC_FLAG_HIST_LIST     0x00008000 /* history does not use a ring buffer*/

#define kRCC_FLAG_NO_TO_ALL     0x00010000 /* subnodes inherit 'no' status      */
#define kRCC_FLAG_NO_CHAINING   0x00020000 /* disable command chaining          */
#define kRCC_FLAG_NO_EXEC       0x00040000 /* do not execute handler code       */
#define kRCC_FLAG_LOG_INPUT     0x00080000 /* enable logging input              */
#define kRCC_FLAG_LOG_OUTPUT    0x00100000 /* enable logging output             */
#define kRCC_FLAG_LOG_IO        0x00200000 /* enable logging output             */
#define kRCC_FLAG_LAST_INPUT    0x00400000 /* recycle input buffer              */
#define kRCC_FLAG_EXEC          0x00800000 /* script is running                 */

#define kRCC_FLAG_HIST_FILTER   0x01000000 /* filter history (repeats, etc)     */
#define kRCC_FLAG_RM_PROMPT     0x02000000 /* prompt contains rapidmark         */
#define kRCC_FLAG_ORDER_NONE    0x04000000 /* no parameter ordering             */
#define kRCC_FLAG_ORDER_SOME    0x08000000 /* unnamed parameter ordering        */
#define kRCC_FLAG_ORDER_FULL    0x10000000 /* full parameter ordering           */
#define kRCC_FLAG_WARNING       0x20000000 /* timeout message has fired off     */
#define kRCC_FLAG_ERROR_SET     0x40000000 /* only need one error notification  */
#define kRCC_FLAG_HIST_MODAL    0x80000000 /* different history stack per mode  */

/* debugging flags */
#define kRCC_DEBUG_MEMORY       0x00000001 /* show system memory used           */
#define kRCC_DEBUG_SHOWEXEC     0x00000002 /* dump exec name to console         */
#define kRCC_DEBUG_STACK        0x00000004 /* dump parent nodes to console      */

/*handle special character */
#define kRCC_SPCHAR_REPLACEVAR   0x00000001 /* replace session variable with real value    */


#define kRCC_FLAG_REALTIME_INPUTECHO   0x00000010 /*falg for input echo*/

#define kRCC_FLAG_ORDER_MASK (kRCC_FLAG_ORDER_NONE | \
                              kRCC_FLAG_ORDER_SOME | \
                              kRCC_FLAG_ORDER_FULL)

/*
   vt100 cursor movement escape codes
   - abstract out to support other term types!
*/
#define kRCC_VTTERM_UP      "\x1B[%dA"
#define kRCC_VTTERM_DN      "\x1B[%dB"
#define kRCC_VTTERM_RT      "\x1B[%dC"
#define kRCC_VTTERM_LT      "\x1B[%dD"

/* terminal screen commands */

#define kRCC_TELNET_BACKSPACE   "\b \b"
#define kRCC_VT100_CLEAR        "\33[2J"
#define kRCC_VT100_HOME         "\033[H"

/* xterm */

#define kRCC_XTERM_CLEAR        "\033\154"

/*
   cursor/keyboard escape codes
   -- is there an rfc for these? is this just vt100?
*/
#define kRCC_ESC_CURSOR   0x5b
#define kRCC_CURSOR_UP    0x41
#define kRCC_CURSOR_DOWN  0x42
#define kRCC_CURSOR_RIGHT 0x43
#define kRCC_CURSOR_LEFT  0x44

/* IOS-like escape codes */
#define kRCC_WORD_PREV               'b'
#define kRCC_WORD_NEXT               'f'
#define kRCC_WORD_UPPERCASE_TO_END   'c'
#define kRCC_WORD_DELETE_TO_END      'd'
#define kRCC_WORD_LOWERCASE_TO_END   'l'

#define CONTROL_KEY(x)          ((x < 'a')   ? (x + 'a' - 1) : (x - 'a' + 1))
#define ESCAPE_KEY(key)         ((key > 127) ? key - 128 : key + 128)

#define kKEY_HELP               ((char) '?')
#define kKEY_DELETE             ((char)(0x7F))

#define kKEY_LINE_START         CONTROL_KEY('a')
#define kKEY_MOVE_LEFT          CONTROL_KEY('b')
#define kKEY_BREAK              CONTROL_KEY('c')
#define kKEY_DELETE_CHAR        CONTROL_KEY('d')
#define kKEY_LINE_END           CONTROL_KEY('e')
#define kKEY_MOVE_RIGHT         CONTROL_KEY('f')
#define kKEY_DELETE_TO_END      CONTROL_KEY('k')
#define kKEY_REFRESH_DISPLAY    CONTROL_KEY('l')
#define kKEY_MOVE_DOWN          CONTROL_KEY('n')
#define kKEY_MOVE_UP            CONTROL_KEY('p')
#define kKEY_TRANSPOSE          CONTROL_KEY('t')
#define kKEY_DELETE_FROM_START  CONTROL_KEY('u')
#define kKEY_DELETE_WORD_START  CONTROL_KEY('w')
#define kKEY_ESCAPE             CONTROL_KEY('x')
#define kKEY_END_OF_ENTRY       CONTROL_KEY('z')

#define kKEY_WORD_PREV          ESCAPE_KEY(kRCC_WORD_PREV)
#define kKEY_WORD_NEXT          ESCAPE_KEY(kRCC_WORD_NEXT)
#define kKEY_UPPERCASE          ESCAPE_KEY(kRCC_WORD_UPPERCASE_TO_END)
#define kKEY_DELETE_WORD_END    ESCAPE_KEY(kRCC_WORD_DELETE_TO_END)
#define kKEY_LOWERCASE          ESCAPE_KEY(kRCC_WORD_LOWERCASE_TO_END)

#define INDENT_SET_HERE(pEnv)   HELP_SET(pEnv, HELP_FLAG_INDENT)  ? \
                                MEDIT_SetIndent(pEnv, MEDIT_GetXPos(pEnv)) : FALSE

/* how are lines terminated? */

#undef kEOL

#ifdef __EOL_USES_LF__
# define kEOL "\n"
#endif

#ifdef __EOL_USES_CR__
# define kEOL "\r"
#endif

#ifndef kEOL
# define __EOL_USES_CRLF__
# define kEOL "\r\n"
#endif

#define kEOL_SIZE (sizeof(kEOL) - 1)


/* if defined, allows multi-line editing */
/**
#define kKEY_CONTINUE           '\\'
**/

/* not supported yet - for multiple commands on one line */
/**
  #define kKEY_SEPARATOR         ';'
**/

#ifdef __cplusplus
extern "C" {
#endif

void    RCC_EXT_AppendText(WMB_SESSION_T * pCliEnv, sbyte *pString,
                                   sbyte4 length, BOOL print);
void    RCC_EXT_CommandLineInit(WMB_SESSION_T * pCliEnv);
EditType RCC_EXT_GetWidth(WMB_SESSION_T * pCliEnv);
EditType RCC_EXT_GetHeight(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_SetWidth(WMB_SESSION_T * pCliEnv, EditType width);
void    RCC_EXT_SetHeight(WMB_SESSION_T * pCliEnv, EditType height);
void    RCC_EXT_GetScreenCoord(WMB_SESSION_T * pCliEnv, sbyte4 *xPos, sbyte4 *yPos);
void    RCC_EXT_DeleteChar(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_EnableScroll(WMB_SESSION_T * pCliEnv, BOOL enable);
void    RCC_EXT_EraseLine(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_EnablePrint(WMB_SESSION_T * pCliEnv, BOOL enable);
void    RCC_EXT_FlushOutput(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_FreeOutput(WMB_SESSION_T * pEnv);
sbyte   RCC_EXT_GetPriorChar(WMB_SESSION_T * pCliEnv);
STATUS  RCC_EXT_Gets(WMB_SESSION_T * pCliEnv, sbyte *pBuffer, sbyte4 buffSize,
                sbyte4 *byteCount, BOOL echo);
STATUS  RCC_EXT_OutputCreate(WMB_SESSION_T * pEnv, sbyte *pBuffer, sbyte4 size);
void    RCC_EXT_OutputReset(WMB_SESSION_T * pEnv);
void    RCC_EXT_InsertText(WMB_SESSION_T * pCliEnv, sbyte *text, sbyte4 length);
void    RCC_EXT_LineEnd(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_LineStart(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_LocalEcho(WMB_SESSION_T * pCliEnv, BOOL enable);
void    RCC_EXT_MoveCursor(WMB_SESSION_T * pCliEnv, EditType offset);
STATUS  RCC_EXT_ParseInput(WMB_SESSION_T * pCliEnv, sbyte * pData,
                                   sbyte4 len, BOOL extended);
void    RCC_EXT_PrintString(WMB_SESSION_T * pCliEnv, sbyte *pString, sbyte4 length, sbyte fill);
STATUS  RCC_EXT_Put(WMB_SESSION_T * pCliEnv, sbyte *pBuf, sbyte4 bufLen);
STATUS  RCC_EXT_PutStr(WMB_SESSION_T * pCliEnv, sbyte *pBuf);
STATUS  RCC_EXT_PutStrLine(WMB_SESSION_T * pCliEnv, sbyte *pBuf);
STATUS  RCC_EXT_ReadCmd(WMB_SESSION_T * pCliEnv, BOOL extended);
void    RCC_EXT_SetCursor(WMB_SESSION_T * pCliEnv, EditType position);
STATUS  RCC_EXT_Settings(WMB_SESSION_T * pCliEnv);
void    RCC_EXT_StringPrint(WMB_SESSION_T * pEnv, sbyte *string, sbyte4 length, sbyte fill);
STATUS  RCC_EXT_Write(WMB_SESSION_T * pCliEnv, sbyte *pBuf, sbyte4 bufLen);
STATUS  RCC_EXT_WriteStr(WMB_SESSION_T * pCliEnv, sbyte *pBuf);
STATUS  RCC_EXT_WriteStrLine(WMB_SESSION_T * pCliEnv, sbyte *pBuf);

#ifdef __cplusplus
}
#endif

#endif /* __RCC_EXT_H__ */


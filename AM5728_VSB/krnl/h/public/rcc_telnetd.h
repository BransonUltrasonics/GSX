/* rcc_telnetd.h */

/* Copyright(c) 2004-2005, 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
27may14,r_w  Eliminate dependency for V7MAN-62
12oct05,wdz  changed '"C"' to 'extern "C"' for SPR#98444
07apr04,gsr  RCC_TELNETD_ConsoleInput() for SPR#94980 and SPR#91986
20feb04,jws  added copyright and mod history.
*/

#ifndef __RCC_TELNETD_H__
#define __RCC_TELNETD_H__

#include <stdio.h>
/*-----------------------------------------------------------------------*/

/* Defaults */

#define kRCC_CONN_TELNET    1
#define kRCC_CONN_CONSOLE   2
#define kRCC_CONN_SERIAL    3
#define kRCC_CONN_EXTERNAL  4

#define kTELNET_TIMEOUT_IN_MINUTES		(global_kRCC_TIMEOUT/60)

/* number of CliChannel objects in array (# TCP threads) */
#define kTELNET_QUEUE_SIZE 10

/*-----------------------------------------------------------------------*/

/* DOS console key codes */
/* to convert to signed char */
#define kRCC_DOSKEY_ESC         0xE0
#define kRCC_DOSKEY_UP          0x48
#define kRCC_DOSKEY_DN          0x50
#define kRCC_DOSKEY_RT          0x4D
#define kRCC_DOSKEY_LT          0x4B
#define kRCC_DOSKEY_PAGE_UP     0x49
#define kRCC_DOSKEY_PAGE_DN     0x51
#define kRCC_DOSKEY_DEL         0x53
/*-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void    RCC_TELNETD(void);
STATUS  RCC_TELNETD_AddSession(WMB_SESSION_T * pCliEnv);
void    RCC_TELNETD_AdjustTerminalWindow(WMB_SESSION_T * pCliEnv);
STATUS  RCC_TELNETD_Broadcast(WMB_SESSION_T * pCliEnv, sbyte * pMessage);
STATUS  RCC_TELNETD_BroadcastMessage(sbyte *pMessage, Access authLevel);
void    RCC_TELNETD_ConsoleSession(void);
BOOL    RCC_TELNETD_CreateSession(OS_SOCKET_T sock, SessionInit * pSessionInit,
                                  ReadHandle * pReadHandle,
                                  WriteHandle * pWriteHandle,
                                  ubyte4 connectionType, sbyte * pName);
void    RCC_TELNETD_ConsoleClose(void);
void    RCC_TELNETD_ConsoleSpeed(int bps);
#ifdef __RCC_SERIAL_CONSOLE__
STATUS  RCC_TELNETD_ConsoleInput(WMB_SESSION_T * pEnv, cliChar * charRead);
#endif
void    RCC_TELNETD_DelSession(WMB_SESSION_T * pCliEnv);
WMB_SESSION_T * RCC_TELNETD_GetSession(sbyte4 index);
void    RCC_TELNETD_Kill(void);
BOOL    RCC_TELNETD_Done(void);
STATUS  RCC_TELNETD_SessionCreate(OS_SOCKET_T sock,
                                          SessionInit *pSessionInit,
                                          ReadHandle *pReadHandle,
                                          WriteHandle *pWriteHandle,
                                          ubyte4 connectionType,
                                          sbyte *pName,
                                          WMB_SESSION_T * * ppCliEnv);

void     RCC_TELNETD_SessionDestroy(WMB_SESSION_T * pCliEnv);

#ifdef __cplusplus
}
#endif


#endif /*  __RCC_TELNETD_H__ */


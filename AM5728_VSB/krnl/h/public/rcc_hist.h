/* rcc_hist.h - header file for cli history */

/* Copyright 2000-2003 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      changed 'C' to 'extern "C"'
                 for SPR#98444
00b,03Jan03,pns updated.
00a,04mar00,pns Written.
*/

/*
 * DESCRIPTION:
 *
 *   rcc_hist.h contains the prototypes for cli history.
 *
 */

#ifndef _RCC_HIST_H_
#define _RCC_HIST_H_

#ifdef __cplusplus
extern "C" {
#endif

sbyte * RCC_HIST_HistBuffPtr(HistInfo * pHistory, sbyte4 index);
STATUS  RCC_HIST_Clear(WMB_SESSION_T * pCliEnv);
STATUS  RCC_HIST_DispHistBuff (WMB_SESSION_T * pCliEnv);
STATUS  RCC_HIST_DispHistLine(WMB_SESSION_T * pCliEnv, BOOL enable);
STATUS  RCC_HIST_Enable(WMB_SESSION_T * pCliEnv, BOOL enable);
void    RCC_HIST_Free(HistInfo * pHistory);
void    RCC_HIST_AddHistLine (WMB_SESSION_T * pCliEnv);
void    RCC_HIST_Scroll(WMB_SESSION_T * pCliEnv, sbyte4 index);
HistInfo * RCC_HIST_Create(WMB_SESSION_T *  pCliEnv);
STATUS  RCC_HIST_ResizeHistBuff(WMB_SESSION_T * pCliEnv, sbyte4 NumCmds);
STATUS  RCC_HIST_ExecHistory(WMB_SESSION_T * pCliEnv);
sbyte * HistBuffPtr(HistInfo * pHistory, sbyte4 index);
STATUS  RCC_HIST_Filter(WMB_SESSION_T * pCliEnv, BOOL repeat);
STATUS  RCC_HIST_Modal(WMB_SESSION_T * pCliEnv, BOOL modal);
STATUS  RCC_HIST_Ring(WMB_SESSION_T * pCliEnv, BOOL ring);
STATUS  RCC_HIST_Info(WMB_SESSION_T * pCliEnv);

#ifdef __cplusplus
}
#endif

#endif /* _RCC_HIST_H_ */

/* sampleLiveCtrlApp.h - example application to demonstrate form evaluation */

/*
 * Copyright (c) 1996-2004, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
02c,09aug12,r_w  standardization for Requirement WIND00347266
02b,28jun04,rws  added globals and comments
02a,20oct03,dlr  Port to WM Web
01a,31oct96,ck   written

*/

/*
DESCRIPTION

NOMANUAL
*/


/* global vars used the sample Live Control app */

extern BOOL        g_liveCtrl_fFlag;
extern short       g_liveCtrl_sRadioIndex;
extern char        g_liveCtrl_szString[101];
extern float       g_liveCtrl_fFloat;
extern short       g_liveCtrl_sListIndex;
extern char        g_liveCtrl_szAreaBuffer [1024];

extern long        g_liveCtrl_myLiveCounter;
extern long        g_liveCtrl_lLongTest;
extern short       g_liveCtrl_sShortTest;
extern char        g_liveCtrl_cCharTest;
extern BOOL        g_liveCtrl_fBoolTest;
extern float       g_liveCtrl_myFloatTest;
extern double      g_liveCtrl_myDoubleTest;


/* live control sample application prototypes */
STATUS  userApplicationStart (void);
STATUS  userApplicationStop(void);

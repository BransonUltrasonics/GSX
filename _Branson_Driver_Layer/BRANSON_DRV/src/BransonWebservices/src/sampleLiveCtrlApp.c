/* sampleLiveCtrlApp.c - example application to demonstrate form evaluation */

/*
 * Copyright (c) 1996-2003, 2012-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
05may14,r_w  fix static analysis issues(US35919)
09aug12,r_w  add httpLCNotifyChange() demo
20oct03,dlr  Port to WM Web
31oct96,ck   written
*/

/*
DESCRIPTION
The sample application is implemented in this file sampleLiveCtrlApp.c.
Make a call to userApplicationStart() to launch the sample Live Control application. 
Make a call to userApplicationStop() to stop the sample Live Control application when you want to stop it.

NOMANUAL
*/


/* includes */
#include "vxWorks.h"
#include "string.h"
#include "wmw_httpconf.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER

#include "httpLib.h"
#include "httpSym.h"
#include "httpRpms.h"
#include "taskLib.h"
#include "sysLib.h"
#include "sampleLiveCtrlApp.h"

#ifdef INCLUDE_HTTP_LIVE_CONTROL
/* public variables which control the behaviour of your application */

BOOL        g_liveCtrl_fFlag=0;
short       g_liveCtrl_sRadioIndex=1;
char        g_liveCtrl_szString[101]="<str>";
float       g_liveCtrl_fFloat=0.0;
short       g_liveCtrl_sListIndex=1;
char        g_liveCtrl_szAreaBuffer [1024]="<buffer>";
long        g_liveCtrl_myLiveCounter=0;
long        g_liveCtrl_lLongTest = 0x12345678;
short       g_liveCtrl_sShortTest = 0x1234;
char        g_liveCtrl_cCharTest = 0;
BOOL        g_liveCtrl_fBoolTest = HTTP_FALSE;
float       g_liveCtrl_myFloatTest = 0.0;
double      g_liveCtrl_myDoubleTest = 0.0;
static BOOL  g_liveCtrl_bStart = 0;

extern HTTP_STATUS    httpLCNotifyChange (char * szName, void * szValue, unsigned int nValueSize);

/****************************************************************************
* testAppl - application thread to test continuous Live Control messaging
*
* NOMANUAL
*/
static void testAppl ( void * pArg )
    {
    STATUS          rc;
    WMB_SESSION_T * pSession;
    WMB_CID_T       cid;

    (void)wmbBackplaneComponentGet(wmbBackplaneGet(), &cid);
    rc = wmbSessionCreate(cid, 0, 0, &pSession);
    if (OK != rc)
        return;


    while ( g_liveCtrl_bStart )
		{
    	g_liveCtrl_myLiveCounter += 1;

		/* Update WindMark */
		if (g_liveCtrl_myLiveCounter%2)
		{
			(void) wmbDataNativeSet (pSession, "LiveCounter",
                          NULL, 0,
                          &g_liveCtrl_myLiveCounter, sizeof(g_liveCtrl_myLiveCounter));
		} else
		{
			(void) httpLCNotifyChange("LiveCounter",NULL,0);
		}

		(void)taskDelay ( (int)(sysClkRateGet() * 1) );
		}
    }

/****************************************************************************
* userApplicationStart - initialize the demonstration Application
*
* Call this function once if you want to set up all data needed.
*
* RETURNS: HTTP_OK
*/

STATUS userApplicationStart
    (
    void
    )
    {
    /* predefine some variables */

    strcpy (g_liveCtrl_szString, "Default setting");
    strcpy (g_liveCtrl_szAreaBuffer, "This is the setting\n for the TEXTAREA");

    g_liveCtrl_sRadioIndex = 1;
    g_liveCtrl_fFlag       = HTTP_FALSE;
    g_liveCtrl_fFloat      = (float)(42.13);
    g_liveCtrl_sListIndex  = 1;

    g_liveCtrl_myLiveCounter = 0;
    g_liveCtrl_bStart = 1;

    (void) taskSpawn("testappl",
				220, 0, 10000,
    			(FUNCPTR) testAppl,
				(long) ((void *) ("LiveCounter")),
				1, 2, 3, 4, 5, 6, 7, 8, 9 );

	return (HTTP_OK);
	}

/****************************************************************************
* userApplicationStop - stop the demonstration Application
*
* Call this function once if you want to stop it.
*
* RETURNS: HTTP_OK
*/

STATUS userApplicationStop
    (
    void
    )
    {
    strcpy (g_liveCtrl_szString, "<str>");
    strcpy (g_liveCtrl_szAreaBuffer, "<buffer>");

    g_liveCtrl_sRadioIndex = 1;
    g_liveCtrl_fFlag       = HTTP_FALSE;
    g_liveCtrl_fFloat      = (float)(42.13);
    g_liveCtrl_sListIndex  = 1;
    g_liveCtrl_myLiveCounter = 0;

    g_liveCtrl_bStart = 0;
    
	return (HTTP_OK);
	
    }
#endif
#endif


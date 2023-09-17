/* httpWmb.c - establish WMB connection */

/* Copyright 2003-2007,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,02feb10,h_y   support for LP64 
01b,23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
01a,19feb04,jws  copyright and mod history added
*/

/*
DESCRIPTION

This module starts the backplane if needed and registers the http server
as a component.

INCLUDE FILES: httpLib.h

*/

#include "httpLib.h"
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER

#if defined(INCLUDE_HTTP_WMB_INTERFACE) || defined(INCLUDE_WMB_INTERFACE)

static WMB_COMPONENT_T * httpWmbComponentId = NULL;

static void httpWmbEventListener (WMB_EVENT_T * pEvent);

/**************************************************************************
*
* httpGetWmbComp - Get the server's component ID for WMB
*
* The component id is initialized by httpWmbInit() at start up, and is
* needed to start WMB sessions.
*
* RETURNS: the component id or null
*
*/
WMB_COMPONENT_T * httpGetWmbComp
    (
    void
    )
    {
    return httpWmbComponentId;
    }


/**************************************************************************
*
* httpWmbEventListener - Receive events from the WMB.
*
* RETURNS: OK
*
*
* NOMANUAL
*/

static void httpWmbEventListener
    (
    WMB_EVENT_T * pEvent
    )
    {
    char *  pName;
    char *  pType;
    WMB_PORTFOLIO_T *portfolio;

    pType = wmbEventTypeNameGet (pEvent->type);
    pName = wmbComponentNameGet (pEvent->cid);
    printf("Component name: %s Event type: %s\n", pName, pType);
    if (WMB_PORTFOLIO_CREATE == pEvent->eventNumber)
        {
        if (OK == wmbPortfolioGet("wmw", "xml", &portfolio))
            {
            httpPortfolioRead(portfolio->fileName);
            }
        }
    }

/*******************************************************************************
*
* httpWmbInit - Start the backplane if needed, and register with it.
*
* Start the backplane if needed, and register with it.
*
* RETURNS: OK or an error
*
*/

STATUS httpWmbInit
    (
    void
    )
    {
    return wmbComponentRegister
        (
        WMB_NAME_K,             /* WMB backplane name               */
        "HTTPserver",           /* Name to be referred by           */
        NULL,                   /* called when component is exiting */
	    NULL,                   /* deregistration state / cookie    */
        httpWmbEventListener,   /* component event handler          */
        &httpWmbComponentId     /* returned component               */
        );
    }

#endif /* INCLUDE_HTTP_WMB_INTERFACE || INCLUDE_WMB_INTERFACE */
#endif


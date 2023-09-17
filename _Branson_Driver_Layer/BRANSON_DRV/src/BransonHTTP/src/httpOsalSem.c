/* httpOsalSem.c - OSAL semaphore translation for http server */

/* Copyright 2004-2007,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de
 */

/*
modification history
-------------------- 
01e,02feb10,h_y   support for LP64 
01d,23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
01c,15jan04,gsr  fixed httpOsalSemMCreate for mutex SPR 93009
01b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01a,13may97,mb   initial creation.
*/

/*
DESCRIPTION
This module encapsulates the semaphores used in the http server

INCLUDES FILES: http/httpType.h
                http/private/httpOsalSem.h
                http/httpSem.h
*/


#include <stdio.h>

#include "httpLib.h"
#include "private/httpOsalSem.h"
#include "commonCfg.h"

long httpOsalSemCountShow ( void );

/* statics */
static long lSemCount;  /* keeps track of how many semaphores are used */


/*******************************************************************************
*
* httpOsalSemInit - initialize the OSAL semaphore package
*
* This routine initializes the OSAL semaphore package.
*
* RETURNS: HTTP_OK, or
*          HTTP_ERROR if initialization failed.
* NOMANUAL
*/

HTTP_STATUS httpOsalSemInit (void)
    {
    lSemCount = 0;
    return HTTP_OK;
    }


/*******************************************************************************
*
* httpOsalSemTake - take a semaphore os independent
*
* This routine performs the take operation on a specified semaphore.
* Depending on the type of semaphore, the state of the semaphore and the
* calling task may be affected.
*
* RETURNS: HTTP_OK, or
*          HTTP_ERROR if the semaphore ID is invalid.
* NOMANUAL
*/

HTTP_STATUS httpOsalSemTake
    (
    HTTP_SEM_ID semId
    )
    {
    return ((HTTP_STATUS)((wmosSemTake (semId, WAIT_FOREVER_K) == OK) ?
                                HTTP_OK : HTTP_ERROR));
    }


/*******************************************************************************
* httpOsalSemGive - give a semaphore os independent
*
* This routine performs the give operation on a specified semaphore.
* Depending on the type of semaphore, the state of the semaphore and the
* pending tasks may be affected.
*
* RETURNS: HTTP_OK, or
*          HTTP_ERROR if the semaphore ID is invalid.
* NOMANUAL
*/

HTTP_STATUS httpOsalSemGive
    (
    HTTP_SEM_ID semId
    )
    {
    return ((HTTP_STATUS)( OK != wmosSemGive (semId) ? HTTP_ERROR : HTTP_OK));
    }


/*******************************************************************************
*
* httpOsalSemMCreate - create and initialize a mutex semaphore os independent
*
* This routine allocates and initializes a mutual-exclusion semaphore.
* The semaphore state is initialized to full.
*
* The options parameter is currently not used, but it should be set to 0 for
* future compatibility.
*
* RETURNS: The semaphore ID, or NULL if memory cannot be allocated.
* NOMANUAL
*/

HTTP_SEM_ID httpOsalSemMCreate
    (
    long options  /* currently not used, set to 0 */
    )
    {
    OS_SEM_T semId = NULL;

    if ( OK == wmosSemMCreate ( &semId, options ))
        {
        lSemCount++;
#ifdef DEBUG_INFO
        printf ("httpOsalSemMCreate: New Semaphore created (MEMORY ALLOCATED!), total number: %ld\n", lSemCount);
        }
    else
        {
        printf ( "httpOsalSemMCreate: No new semaphore created!\n" );
#endif
        }
    return ( semId );
    }


/*******************************************************************************
*
* httpOsalSemDelete - delete a semaphore OS independent
*
* This routine deletes a semaphore with an OS independent interface.
*
* This routine terminates and deallocates any  memory  associated
* with a specified semaphore. Any pended tasks will unblock and
* return HTTP_ERROR.
*
* RETURNS: HTTP_OK or HTTP_ERROR if the semaphore ID is invalid.
*
* NOMANUAL
*/

HTTP_STATUS httpOsalSemDelete
    (
    HTTP_SEM_ID semId  /* semaphore to delete */
    )
    {
    if ( OK == wmosSemDestroy ( semId ))
    {
    lSemCount--;
    return HTTP_OK;
    }
    return ( HTTP_ERROR );
    }


/*******************************************************************************
*
* httpOsalSemCountShow - show how many semaphores are used by HTTP server
*
* This routine is just for debugging and should help to find semaphore
* leakages.
*
* RETURNS: number of semaphores currently used by HTTP server.
* NOMANUAL
*/

long httpOsalSemCountShow ( void )
    {
    return lSemCount;
    }


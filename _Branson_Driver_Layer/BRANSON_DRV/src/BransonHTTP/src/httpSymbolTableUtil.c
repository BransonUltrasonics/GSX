/* httpSymbolTableUtil.c - symbol table package */

/* Copyright 1999-2007,2010 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/* Project: Wind Web Server, Version 2.0
 *
 * Copyright (c) 1996/97, 3Soft GmbH
 *
 * 3Soft GmbH, Wetterkreuz 19a
 * D-91058 Erlangen, Germany
 * infohttp@dreisoft.de, http://www.dreisoft.de/
 */

/*
modification history
-------------------- 
02a,02feb10,h_y   support for LP64 
01l,23feb07,msa  Correct the apigen comments so they match the actual
                 routines.
01k,13feb07,msa  Fix comments to remove APIgen errors.
01j,21dec99,csr  reverting to orginal API of httpSymTabAdd
01i,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01h,08dec98,mb   changed offset for double hashing used in httpSymTabCreate().
01g,17nov98,mb   bugfix: httpSymValtoStringCopy allocated buffer to store 
                 strings BEFORE calling GET functions -> memory corruption.
01f,28apr98,mb   bugfix: httpSymTabIterator() returned symbol name instead 
                 of HTTP_SYM_ID.
01e,02apr98,mb   symbol type handler concept introduced
01d,01sep97,mb   bugfix: callSymFunc splitted into callSymGetFunc() 
                 and callSymSetFunc.
01c,26may97,mb   httpSymTabCreate may be initialized to multiples of 17.
01b,13may97,mb   osal introduced.
01a,15jan97,mb   initial release.
*/

/*
DESCRIPTION
This module provides a symbol table using symbols with type and description 
information.

INCLUDE FILES: http/httpLib.h
	       http/httpHash.h
	       http/httpSym.h
	       http/httpSem.h
	       http/httpUtil.h
	       http/httpList.h

NOMANUAL
*/

#include <stdio.h>
#include "stdlib.h"
#include <string.h>

#include "httpLib.h"
#include "httpHash.h"
#include "httpSym.h"
#include "httpSem.h"
#include "httpUtil.h"
#include "httpList.h"
#include "commonCfg.h"


/* Structure of a symbol table. */
struct http_sym_tab
    {
	unsigned long       ulMaxSymbols;
	HTTP_HASH_TABLE_ID  hashTabId;
	HTTP_SEM_ID         semLock;
	MEM_POOL_ID    partId;
	unsigned long       ulOptions;
    };


/* static vars */
static HTTP_GEN_LIST_ID     httpSymTypeListId;
static HTTP_SYM_ACCESS_ID   httpSymAccessDef;
static HTTP_SYM_NOTIFY_FUNC httpSymNotifyFunc;

/*******************************************************************************
*
* httpSymTabInit - initialize the symbol table
*
* This routine initializes the symbol table.
* Call this routine only once during system startup.
*
* RETURNS: HTTP_OK if initialization was successful, HTTP_ERROR if not.
*/

HTTP_STATUS httpSymTabInit
    (
    unsigned long ulMaxTypes
    )
    {
    httpSymAccessDef = HTTP_SYM_ACCESS_DEF;
    httpSymNotifyFunc = NULL;
    if ( ( httpSymTypeListId = httpGenListCreate ( httpGlobalMemPartIdGet() , 
		                    (unsigned short) ulMaxTypes , HTTP_TRUE ) ) != NULL )
	{
	logInfo ("httpSymTabInit: symbol table is initialized.");
	return HTTP_OK;
	}
    logError ( "httpSymTabInit: initialization failed!" );
    return HTTP_ERROR;
    }


/*******************************************************************************
*
* httpSymTabLock - lock the symbol table
*
* This routine uses a mutual exclusion semaphore to prevent access to a
* symbol table and its contents.
*
* RETURNS: N/A
*/

HTTP_STATUS httpSymTabLock
    (
    HTTP_SYM_TAB_ID symTabId  /* symbol table to lock */
    )
    {
    if (symTabId == NULL)
	logInfo ("httpSymTabLock: symId invalid!");
    else
	return httpOsalSemTake ( symTabId->semLock );
    return HTTP_ERROR;
    }


/*******************************************************************************
*
* httpSymTabUnlock - unlock the symbol table
*
* This routine uses a mutual exclusion semaphore to provide access to a
* symbol table and its contents.
*
* RETURNS: N/A
*/

HTTP_STATUS httpSymTabUnlock
    (
    HTTP_SYM_TAB_ID symTabId  /* symbol table to unlock */
    )
    {
    if (symTabId == NULL)
	logInfo ("httpSymTabUnlock: symId invalid!");
    else
	return httpOsalSemGive (symTabId->semLock);
    return HTTP_ERROR;
    }


/*******************************************************************************
*
* httpSymLock - locks a symbol
*
* This routine uses a mutual exclusion semaphore to lock access to a symbol 
* and its contents.
*
* RETURNS: HTTP_OK if locking was successful, or
*          HTTP_ERROR otherwise.
*
* SEE ALSO: httpSymUnlock(), httpSymTabAdd()
*/

HTTP_STATUS httpSymLock
    (
    HTTP_SYM_ID symId  /* symbol to lock */
    )
    {
    if (symId == NULL)
	logInfo ("httpSymLock: symId invalid!");
    else
	if ( symId->semId != NULL )
	    return httpOsalSemTake ( symId->semId );
    return HTTP_ERROR;
    }


/*******************************************************************************
*
* httpSymUnlock - unlocks a symbol
*
* This routine uses a mutual exclusion semaphore to unlock access to a symbol 
* and its contents.
*
* RETURNS: N/A
*
* SEE ALSO: httpSymLock(), httpSymTabAdd()
*/

HTTP_STATUS httpSymUnlock
    (
    HTTP_SYM_ID symId  /* symbol to unlock */
    )
    {
    if (symId == NULL)
	logInfo ("httpSymUnlock: symId invalid!");
    else
	if ( symId->semId != NULL )
	    return httpOsalSemGive (symId->semId);
    return HTTP_ERROR;
    }


/*******************************************************************************
*
* httpSymTabCreate - create a new symbol table
*
* This routine creates a new symbol table with a maximum of <ulNumEntries>
* entries.  Note that as this symbol table package uses double hashing, thus
* performance problems may occur if the symbol table is nearly
* full.
*
* NOTE: Use prime numbers for the size of the symbol table to get
* good distribution of symbols in the hash table.
*
* This routine now supports locking with fine granularity. 
* This feature can be enabled setting ulOptions to HTTP_SYM_OPTION_USELOCKS
* when creating a symbol table. If enabled, any API function reading 
* or writing to symbols will lock/unlock the specified symbol during operation.
* Please note that httpSymTabAdd() creates a mutex semaphore for each symbol 
* if this feature is used!
*
* ATTENTION: Do not use multiples of 19997 for size, as this is the offset for
* the double hashing mechanism.
*
* RETURNS: HTTP_SYM_TAB_ID or NULL.
*/

HTTP_SYM_TAB_ID httpSymTabCreate
    (
    MEM_POOL_ID  partId,       /* where to get the memory from   */
    unsigned long     ulNumEntries, /* max number of symbols to store */
    unsigned long     ulOptions     /* option bits                    */
    )
    {
    HTTP_SYM_TAB_ID symTabId;

    if ( (symTabId = (HTTP_SYM_TAB_ID) memPoolAlloc ( partId,  sizeof (HTTP_SYM_TAB))) != NULL )
	{
	symTabId->ulMaxSymbols  = ulNumEntries;
	symTabId->partId        = partId;
	symTabId->ulOptions     = ulOptions;
	if ( (symTabId->hashTabId = httpHashCreate (partId,  ulNumEntries , NULL , 19997 )) != NULL )
	    {
	    if ( ( symTabId->semLock = httpOsalSemMCreate ( HTTP_SEM_Q_PRIORITY ) ) != NULL)
		return symTabId;
	    }
	memPoolFree ( partId, (char *) symTabId );
	}
    return NULL;
    }


/*******************************************************************************
*
* httpSymAccessDefSet - set the default permissions of new symbols being created
*
* This routine adjusts the default permissions for accessing symbol values.
* Available arguments (permissions) to httpSymAccessDefSet() are:
*
* #define HTTP_SYM_ACCESS_NONE    do not grant any accesses to the symbol value 
* #define HTTP_SYM_ACCESS_R       symbol value may be read
* #define HTTP_SYM_ACCESS_W       symbol value may be changed (written)
* #define HTTP_SYM_ACCESS_RW      both read and write accesses are permitted
* #define HTTP_SYM_ACCESS_DEF     global default permissions used when symbol
*                                 table is initialized
*
* The permissions set are used only for subsequent calls to httpSymTabCreate().
* Symbols already existing will not be changed.
*
* RETURNS: N/A
*
* SEE ALSO: httpSymAccessDefGet(), httpSymTabCreate(), httpSymAccessRead(),
*           httpSymAccessWrite()
*/

void httpSymAccessDefSet
    (
    HTTP_SYM_ACCESS_ID accessId  /* permission code to set */
    )
    {
    httpSymAccessDef = accessId;
    }


/*******************************************************************************
*
* httpSymAccessDefGet - get the default permissions
*
* This routine provides the currently active default permissions.
*
* RETURNS: current default access permissions
*
* SEE ALSO: httpSymAccessDefSet(), httpSymTabCreate(), httpSymAccessRead(),
*           httpSymAccessWrite()
*/

HTTP_SYM_ACCESS_ID httpSymAccessDefGet(void)
    {
    return httpSymAccessDef;
    }


/*******************************************************************************
*
* httpSymAccessSet - set the permissions of the specified symbol
*
* This function adjusts the access permissions of the specified
* symbol.
* Available arguments (permissions) to httpSymAccessSet() are:
*
* #define HTTP_SYM_ACCESS_NONE    do not grant any accesses to the symbol value 
* #define HTTP_SYM_ACCESS_R       symbol value may be read
* #define HTTP_SYM_ACCESS_W       symbol value may be changed (written)
* #define HTTP_SYM_ACCESS_RW      both read and write accesses are permitted
* #define HTTP_SYM_ACCESS_DEF     global default permissions used when symbol
*                                 table is initialized
*
* The permissions set are used for subsequent calls to httpSymValGet(),
* httpSymValSet(), httpSymValToString() and httpSymValSetByString().
*
* RETURNS: N/A
*
* SEE ALSO: httpSymAccessDefGet(), httpSymTabCreate(), httpSymAccessRead(),
*           httpSymAccessWrite(), httpSymAccessGet(), httpSymAccessLive()
*/

void httpSymAccessSet
    (
    HTTP_SYM_ID        symId,    /* symbol id */
    HTTP_SYM_ACCESS_ID accessId  /* permission code to set */
    )
    {
    httpSymLock ( symId );
    symId->accessId = accessId;
    httpSymUnlock ( symId );
    }


/*******************************************************************************
*
* httpSymAccessGet - get a symbol's permissions
*
* This routine provides the currently active access permissions of the
* specified symbol.
*
* RETURNS: current access permissions
*
* SEE ALSO: httpSymAccessDefSet(), httpSymTabCreate(), httpSymAccessRead(),
*           httpSymAccessWrite(), httpSymAccessSet(), httpSymAccessLive()
*/

HTTP_SYM_ACCESS_ID httpSymAccessGet
    (
    HTTP_SYM_ID symId
    )
    {
    HTTP_SYM_ACCESS_ID accessId;

    httpSymLock ( symId );
    accessId = symId->accessId;
    httpSymUnlock ( symId );
    
    return accessId;
    }


/*******************************************************************************
*
* httpSymAccessRead - check if read access permitted
*
* This routine determines whether read access to the specified symbol is
* permitted or not.
*
* RETURNS: HTTP_TRUE if read access is permitted,
*          HTTP_FALSE if not.
*
* SEE ALSO: httpSymAccessDefGet(), httpSymAccessDefSet(), httpSymTabCreate(),
*           httpSymAccessWrite(), httpSymAccessLive()
*/

HTTP_BOOL httpSymAccessRead
    (
    HTTP_SYM_ID symId
    )
    {
    return ( ( ( symId->accessId & HTTP_SYM_ACCESS_R ) == HTTP_SYM_ACCESS_R ) ? HTTP_TRUE : HTTP_FALSE );
    }


/*******************************************************************************
*
* httpSymAccessWrite - check if write access permitted
*
* This routine determines whether write access to the specified symbol
* is permitted or not.
*
* RETURNS: HTTP_TRUE if write access is permitted,
*          HTTP_FALSE if not.
*
* SEE ALSO: httpSymAccessDefGet(), httpSymAccessDefSet(), httpSymTabCreate(),
*            httpSymAccessRead(), httpSymAccessLive()
*/

HTTP_BOOL httpSymAccessWrite
    (
    HTTP_SYM_ID symId
    )
    {
    return ( ( ( symId->accessId & HTTP_SYM_ACCESS_W ) == HTTP_SYM_ACCESS_W ) ? HTTP_TRUE : HTTP_FALSE );
    }


/*******************************************************************************
*
* httpSymAccessLive - check if access from LiveControl is permitted
*
* This routine determines whether symbol may be accessed using
* LiveControl.
*
* RETURNS: HTTP_TRUE if access is permitted,
*          HTTP_FALSE if not.
*
* SEE ALSO: httpSymAccessDefGet(), httpSymAccessDefSet(), httpSymTabCreate(),
*           httpSymAccessWrite()
*/

HTTP_BOOL httpSymAccessLive
    (
    HTTP_SYM_ID symId
    )
    {
    return ( ( ( symId->accessId & HTTP_SYM_ACCESS_LIVE ) == HTTP_SYM_ACCESS_LIVE ) ? HTTP_TRUE : HTTP_FALSE );
    }


/*******************************************************************************
*
* httpSymTabAdd - add a new entry to a symbol table
*
* This routine adds a new symbol to a symbol table.
* The <pAddr> argument may be NULL if the symbol has a dynamic location.
*
* See httpSymLib.h for valid symbol types.
*
* NOTE: This routine now supports locking with fine granularity. This feature
* can be enabled using HTTP_SYM_OPTION_USELOCKS when creating a symbol table.
* If enabled, any API function reading or writing to symbols will
* lock/unlock the specified symbol during operation. Please note that 
* httpSymTabAdd() creates a mutex semaphore for each symbol if this feature 
* is used!
*
* ATTENTION:
* Symbol types which do not implicitly define the size of a value may
* cause the HTTP server to trash memory.
* Please use httpSymValMaxSizeSet() to specify the memory consumption in
* these cases. Examples for dynamic length types are "string"
* and "genlist".
*
* RETURNS: HTTP_SYM_ID or NULL.
*/

HTTP_SYM_ID httpSymTabAdd
    (
    HTTP_SYM_TAB_ID  symTabId,          /* ID of symbol table                                  */
    char *           szName,            /* name of the symbol to add                           */
    char *           szDescription,     /* description of the symbol to add                           */
    HTTP_SYM_TYPE_ID typeId,            /* type of the symbol                                  */
    unsigned long    ulNumItems,        /* #variables of type typeId are referenced via symbol */
    void *           pAddr              /* pointer to the variable the symbol represents       */
    )
    {
    HTTP_SYM_ID symId;

    if ( ( typeId != NULL ) && ( szName != NULL ) )
	{
	httpSymTabLock( symTabId );
	if ( (symId = memPoolAlloc ( symTabId->partId, sizeof (HTTP_SYM))) != 0)
	    {
	    symId->pAddr = pAddr;
	    symId->szName = szName;
	    symId->szDesc = szDescription;
	    symId->typeId = typeId;
	    symId->ulNumItems = ulNumItems;
	    symId->accessId = httpSymAccessDef;
	    if ( (symTabId->ulOptions & HTTP_SYM_OPTION_USELOCKS ) == HTTP_SYM_OPTION_USELOCKS )
		if ( ( symId->semId = httpOsalSemMCreate ( HTTP_SEM_Q_PRIORITY ) ) == NULL )
		    {
		    memPoolFree ( symTabId->partId, (char *) symId );
		    httpSymTabUnlock( symTabId );
		    return ( NULL );
		    }
	    if ( httpHashAdd (symTabId->hashTabId, szName, symId ) == HTTP_OK )
		{
		symId->ulMaxSize = httpSymValSize ( symId );
		httpSymTabUnlock( symTabId );
		return ( symId );
		}
	    if ( symId->semId != NULL )
		httpOsalSemDelete ( symId->semId );
	    memPoolFree ( symTabId->partId, (char *) symId );
	    }
	httpSymTabUnlock( symTabId );
	}
    return ( NULL );
    }


/*******************************************************************************
*
* httpSymDescriptionSet - set a text description for a symbol
*
* This routine sets a text description for a symbol.
*
* RETURNS: HTTP_TRUE if description was set, or
*          HTTP_FALSE otherwise.
*/

HTTP_BOOL httpSymDescriptionSet
    (
    HTTP_SYM_ID symId,         /* symbol id                 */
    char *      szDescription  /* description string to set */
    )
    {
    if ( symId != NULL )
	{
	symId->szDesc = szDescription;
        return HTTP_TRUE;
	}
    return HTTP_FALSE;
    }


/*******************************************************************************
*
* httpSymDescriptionGet - get the text description for a symbol
*
* This routine gets the text description for a symbol.
*
* RETURNS: pointer to description string, or
*          NULL otherwise.
*/

char * httpSymDescriptionGet
    (
    HTTP_SYM_ID symId /* symbol id */
    )
    {
    if ( symId != NULL )
      return symId->szDesc;
    return NULL;
    }


/*******************************************************************************
*
* httpSymTabFindByNameAndType - find a symbol specified by name and type
*
* This routine does not make any use of types yet, so it
* has the same effect as httpSymTabFindByName().
*
* RETURNS: HTTP_SYM_ID or NULL if the described symbol does not exist
*
* NOMANUAL
*/

HTTP_SYM_ID httpSymTabFindByNameAndType
    (
    HTTP_SYM_TAB_ID symTabId,  /* symbol table to search in */
    char *          szName,    /* name of the needed symbol */
    HTTP_SYM_TYPE_ID   typeId  /* type of the needed symbol */
    )
    {
    HTTP_SYM_ID symId;

    httpSymTabLock (symTabId);
    symId = httpHashGet (symTabId->hashTabId, szName);
    httpSymTabUnlock (symTabId);

    return ( httpSymTypeGet(symId) == typeId ? symId : NULL );
    }


/*******************************************************************************
*
* httpSymTabFindByName - find a symbol specified by name
*
* This routine finds a symbol with a specified name.  If there are several
* symbols with the same name, the resulting symbol is undefined.
*
* RETURNS: HTTP_SYM_ID, or NULL if the symbol does not exist.
*/

HTTP_SYM_ID httpSymTabFindByName
    (
    HTTP_SYM_TAB_ID symTabId,   /* symbol table to search in */
    char *          szName      /* name of the needed symbol */
    )
    {
    HTTP_SYM_ID symId;

    httpSymTabLock (symTabId);
    symId = httpHashGet (symTabId->hashTabId, szName);
    httpSymTabUnlock (symTabId);
    return symId;
    }


/*******************************************************************************
*
* httpSymTabIterator - iterate through a symbol table
*
* This routine allows iteration through a
* symbol table, entry by entry.
*
* NOTE: Lock the symbol table before calling this routine.
*
* RETURNS: HTTP_SYM_ID, or NULL if the described symbol does not exist.
*/

HTTP_SYM_ID httpSymTabIterator
    (
    HTTP_SYM_TAB_ID symTabId,
    unsigned long * pulIndex
    )
    {
    char * szName = httpHashIterator (symTabId->hashTabId, pulIndex);
    if ( szName != NULL )
	return httpSymTabFindByName ( symTabId, szName );
    return NULL;
    }


/*******************************************************************************
*
* httpSymValCmp - compare the value of a symbol with another value
*
* This routine compares a symbol's value with a specified value and returns zero
* if the values are equal.
* The comparison is performed bit by bit using a size
* dependant on the symbol's type, so signed and unsigned types
* can be compared by the same code.
*
* RETURNS: Zero for equal values (see memcmp()).
*/

short httpSymValCmp
    (
    HTTP_SYM_ID symId ,   /* reference symbol                 */
    void * pValue         /* pointer to value to compare with */
    )
    {
    void * pSymVal;
    short  sCmp = 0;

    if ( symId != NULL )
	{
	httpSymLock ( symId );
	if ( ( pSymVal = httpSymValGet ( symId ) ) != NULL )
	    sCmp = memcmp (pSymVal, pValue, httpSymValSize (symId) * httpSymNumItemsGet( symId ));
	httpSymUnlock( symId );
	}
    return sCmp;
    }


/*******************************************************************************
*
* httpSymNameGet - get the name of a symbol
*
* This routine gets the name of a symbol,  based on the symbol ID.
*
* RETURNS: A string pointer to the symbol's name.
*/

char * httpSymNameGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    return symId->szName;
    }


/*******************************************************************************
*
* httpSymTypeGet - get a symbol's type
*
* This routine gets a symbol's type, based the symbol ID.
*
* RETURNS: The symbol's type.
*/

HTTP_SYM_TYPE_ID httpSymTypeGet
    (
    HTTP_SYM_ID symId
    )
    {
    return symId->typeId;
    }


/*******************************************************************************
*
* httpSymValGet - get the value of a symbol
*
* This routine gets the current value of a symbol. If a function is attached 
* to the get operation of the symbol, the function will be executed
* before.
*
* RETURNS: pointer to the value of a symbol,
*          or NULL if there is no value yet.
*/

void * httpSymValGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    httpSymLock ( symId );
    if ( httpSymAccessRead ( symId ) == HTTP_FALSE )
	{
	httpSymUnlock ( symId );
	logInfoMsg ( "httpSymValGet: symbol %s is not readable!" , symId->szName );
	return NULL;
	}
    
    logInfoMsg ("Get the value of %s", symId->szName);
    
    if ( symId->pGetFunc != NULL)
        {
	logInfoMsg ("Call get func of %s", symId->szName);
	symId->pGetFunc (symId, symId->pGetUserArg); /* ToDo: analyze return code XXX */
	}
    httpSymUnlock ( symId );
    return symId->pAddr;
    }


/*******************************************************************************
*
* httpSymValSet - set a symbols actual value
*
* This routine sets the current value of a symbol. If a function is attached 
* to the set operation of the symbol, the function will be executed after
* setting the new symbol value.
*
* RETURNS: N/A
*/

HTTP_STATUS httpSymValSet
    (
    HTTP_SYM_ID symId,		/* symbol id               */
    void * pValue		/* pointer to value to set */
    )
    {
    httpSymLock ( symId );
    if ( httpSymAccessWrite ( symId ) == HTTP_FALSE )
	{
	httpSymUnlock ( symId );
	logInfoMsg ( "httpSymValSet: symbol %s is not writable!" , symId->szName );
	return HTTP_ERROR;
	}

    symId->pAddr = pValue;
    
    logInfoMsg ("Set the value of %s", symId->szName);
    
    if (symId->pSetFunc != NULL)
	{
	logInfoMsg ("Call set func of %s", symId->szName);
	symId->pSetFunc (symId, symId->pSetUserArg); /* ToDo: analyze return code XXX */
	}
    if ( httpSymNotifyFunc != NULL )
	httpSymNotifyFunc ( symId );
    httpSymUnlock ( symId );
    return HTTP_OK;
    }


/*******************************************************************************
*
* httpSymNotifyFuncSet - set a hook function call if symbol values change
*
* This routine sets a hook function call if symbol values change.
*
* RETURNS: N/A
*/

void httpSymNotifyFuncSet
    ( 
    HTTP_SYM_NOTIFY_FUNC pFunc
    )
    {
    httpSymNotifyFunc = pFunc;
    }


/*******************************************************************************
*
* httpSymGetFuncGet - get the routine attached to the symbol's GET operation
*
* This routine returns the GET routine for a specified symbol, which is
* the routine called when the symbol's value is read.
*
* RETURNS: The symbol's get routine.
*
* SEE ALSO: httpSymGetFuncSet()
*/

HTTP_SYM_FUNC httpSymGetFuncGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    HTTP_SYM_FUNC pFunc;

    httpSymLock ( symId );
    pFunc = symId->pGetFunc;
    httpSymUnlock ( symId );

    return pFunc;
    }


/*******************************************************************************
*
* httpSymGetArgGet - get the user argument associated with the GET routine
*
* This routine returns the user argument associated with the GET
* routine for the given symbol.
*
* RETURNS: The user argument for the symbol's GET routine.
*
* SEE ALSO: httpSymGetFuncSet()
*/

void * httpSymGetArgGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    void * pArg;

    httpSymLock ( symId );
    pArg = symId->pGetUserArg;
    httpSymUnlock ( symId );

    return pArg;
    }


/*******************************************************************************
*
* httpSymGetArgSet - set the user argument associated with the GET routine
*
* This routine sets the user argument associated with the GET routine for 
* the given symbol.
*
* RETURNS: N/A
*
* SEE ALSO: httpSymGetArgGet()
*/

void httpSymGetArgSet
    (
    HTTP_SYM_ID symId,   /* symbol id */
    void *      pArg
    )
    {
    httpSymLock ( symId );
    symId->pGetUserArg = pArg;
    httpSymUnlock ( symId );
    }


/*******************************************************************************
*
* httpSymGetFuncSet - associate a routine with a symbol's GET operation
*
* This routine associates a routine with a symbol's read operation.
* This routine is executed each time httpSymValGet() is called.
* The routine prototype for GET routines is:
*
* .CS
*     HTTP_STATUS (* HTTP_SYM_FUNC)(HTTP_SYM_ID, void * );
* .CE
*
* The GET routine receives the symbol ID and a pointer to user-specific
* data <pUserArg>.
*
* If <fOwnTask> is HTTP_TRUE, the read routine is executed asynchronously
* using its own task, which are created dynamically.
*
* RETURNS: N/A
*/

void httpSymGetFuncSet
    (
    HTTP_SYM_ID    symId,     /* symbol id                     */
    HTTP_SYM_FUNC symFunc,   /* symbol write function         */
    void *         pUserArg,  /* pointer to user specific data */
    HTTP_BOOL      fOwnTask   /* HTTP_TRUE for own task spawn  */
    )
    {

    httpSymLock ( symId );
    if ( symFunc != NULL )
	{
	symId->pGetFunc = symFunc;
	symId->pGetUserArg = pUserArg;
	/* symId->typeId |= HTTP_SYM_TYPE_FUNCTION | HTTP_SYM_TYPE_FUNCTION_UNIFY | ( (fOwnTask == HTTP_TRUE ) ? HTTP_SYM_TYPE_OWNTASK_GET : 0 ); */ /* XXX  how to clear owntask setup ?? */
	}
    httpSymUnlock ( symId );
    }


/*******************************************************************************
*
* httpSymSetFuncGet - get the routine associated with the symbol's SET operation
*
* This routine returns the SET routine for a symbol, which is the routine
* called when the symbol's value is written.
*
* RETURNS: The symbol's set routine.
*
* SEE ALSO: httpSymSetFuncSet()
*/

HTTP_SYM_FUNC httpSymSetFuncGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    HTTP_SYM_FUNC pFunc;

    httpSymLock ( symId );
    pFunc = symId->pSetFunc;
    httpSymUnlock ( symId );

    return pFunc;
    }


/*******************************************************************************
*
* httpSymSetArgGet - get the user argument for the SET operation
*
* This routine returns the user argument associated with the SET
* routine for a symbol.
*
* RETURNS: The symbol's set routine's user argument.
*
* SEE ALSO: httpSymSetFuncSet()
*/

void * httpSymSetArgGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    void * pArg;

    httpSymLock ( symId );
    pArg = symId->pSetUserArg;
    httpSymUnlock ( symId );

    return pArg;
    }


/*******************************************************************************
*
* httpSymSetArgSet - set the user argument of the SET operation
*
* This routine sets the user argument associated with the SET
* routine for the given symbol.
*
* RETURNS: N/A
*
* SEE ALSO: httpSymSetArgGet()
*/

void httpSymSetArgSet
    (
    HTTP_SYM_ID symId,   /* symbol id */
    void *      pArg
    )
    {
    httpSymLock ( symId );
    symId->pSetUserArg = pArg;
    httpSymUnlock ( symId );
    }


/*******************************************************************************
*
* httpSymSetFuncSet - associate a routine with a symbol's SET operation
*
* This routine associates a routine with a symbol's write operation.
* This routine is executed each time httpSymValSet() is called.
* The routine prototype for SET routines is:
*
* .CS
*     HTTP_STATUS (* HTTP_SYM_FUNC)(HTTP_SYM_ID, void * );
* .CE
*
* The SET routine receives the symbol ID and a pointer to user-specific
* data <pUserArg>.
*
* If <fOwnTask> is HTTP_TRUE, the read routine will be executed asynchronously
* using its own task, which will be created dynamically.
*
* RETURNS: N/A
*/

void httpSymSetFuncSet 
    ( 
    HTTP_SYM_ID   symId,     /* symbol id */
    HTTP_SYM_FUNC symFunc,   /* set function to register */
    void *        pUserArg,  /* custom argument for set function */
    HTTP_BOOL     fOwnTask   /* HTTP_TRUE for own task to execute set function */
    ) 
    {
    httpSymLock ( symId );
    if ( symFunc != NULL )
	{
	symId->pSetFunc = symFunc;
	symId->pSetUserArg = pUserArg ;
	}
    httpSymUnlock ( symId );
    }


/*******************************************************************************
*
* httpSymFuncUnify - control access to a symbol routine
*
* This routine controls the invocation of symbol read/write
* routines.  If <fSet> is HTTP_TRUE,
* only one call to a symbol's read/write routine is allowed at a
* time.
* Any read/write operation attempted while another is in progress
* are skipped.
*
* RETURNS: N/A
*/

void httpSymFuncUnify
    (
    HTTP_SYM_ID symId,   /* symbol id */
    HTTP_BOOL   fSet     /* HTTP_TRUE for unification, HTTP_FALSE for free access */
    )
    {
    logError ( "Symbol unification currently not supported!" );
    }


/*******************************************************************************
*
* httpSymNumItemsGet - get the number of array items stored in a symbol
*
* This routine provides the actual number of items stored in
* the symbol specified by <symId>.
*
* RETURNS: Number of items stored in <symId>.
*/

unsigned long httpSymNumItemsGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    return symId->ulNumItems;
    }


/*******************************************************************************
*
* httpSymNumItemsSet - set the number of array items stored in a symbol
*
* This routine sets the actual number of items stored in
* the symbol specifided by <symId>.
*
* RETURNS: N/A
*/

void httpSymNumItemsSet
    (
    HTTP_SYM_ID symId,            /* symbol id */
    unsigned long ulNumItems      /* number of items */
    )
    {
    symId->ulNumItems = ulNumItems;
    }


/*******************************************************************************
*
* httpSymValCpy - copy the value of a symbol
*
* This routine copies the value of a symbol to preallocated memory
* pointed to by <pCopy>.
*
* RETURNS: N/A
*/

HTTP_BOOL httpSymValCpy
    (
    HTTP_SYM_ID symId,   /* symbol id */
    void * pCopy         /* pointer to preallocated memory to copy value to */
    )
    {
    void *    pSymVal = NULL;

    if ( symId != NULL )
	{
	httpSymLock ( symId );
	if ( ( pSymVal = httpSymValGet ( symId ) ) != NULL )
	    memcpy ( pCopy , pSymVal , httpSymValSize ( symId ) * httpSymNumItemsGet ( symId ) );
	httpSymUnlock ( symId );
	}
    return (pSymVal == NULL ? HTTP_FALSE : HTTP_TRUE );
    }


/*******************************************************************************
*
* httpSymValSize - determine the size of a symbol's value
*
* This routine determines the size of a symbol's value in bytes, rounded up.
*
* RETURNS: The size of a symbol value in bytes.
*/

unsigned long httpSymValSize
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    HTTP_SYM_TYPE_HANDLER_ID pHandler;
    unsigned long            ulSize = 0;

    if ( symId != NULL )
	{
	httpSymLock ( symId );
	if ( ( pHandler = httpGenListDataGet ( httpSymTypeGet (symId) ) ) != NULL )
	    ulSize =  pHandler->pSymValSize ( symId );
	httpSymUnlock ( symId );
	}
    return ulSize;
    }


/*******************************************************************************
*
* httpSymValStringSizeGet - get the size in bytes of a symbol's value
*
* This routines determines the (max) size in bytes of a symbol value in 
* string representation. This number depends on the symbol type and is 
* provided by the symbol type handler.
*
* RETURNS: size of symbol value in bytes.
*/

unsigned long httpSymValStringSizeGet
    (
    HTTP_SYM_ID symId   /* symbol id */
    )
    {
    HTTP_SYM_TYPE_HANDLER_ID pHandler;
    unsigned long            ulSize = 0;

    if ( symId != NULL )
	{
	httpSymLock ( symId );
	if ( ( pHandler = httpGenListDataGet ( httpSymTypeGet (symId) ) ) != NULL )
	    ulSize =  pHandler->pSymValStringSize ( symId );
	httpSymUnlock ( symId );
	}
    return ulSize;
    }


/*******************************************************************************
*
* httpSymValMaxSizeSet - set the maximum size of a symbol value in bytes
*
* This routine sets the maximum size of a symbol value in bytes. Some symbol 
* types do have variable content length, e.g. strings. To be able to perform 
* a suitable range checking, it is neccessary to know the maximum memory 
* consumption of the symbol value.
*
* RETURNS: N/A
*/

void httpSymValMaxSizeSet
    (
    HTTP_SYM_ID   symId,
    unsigned long ulMaxSize
    )
    {
    httpSymLock ( symId );
    if ( httpSymValSize ( symId ) <= ulMaxSize )
	{
	symId->ulMaxSize = ulMaxSize;
	}
#ifdef DEBUG_INFO
    else
	{
	logInfoMsg ( "httpSymValMaxSizeSet: symbol %s max size < current size!",httpSymNameGet(symId) );
	}
#endif
    httpSymUnlock ( symId );
    }


/*******************************************************************************
*
* httpSymValMaxSizeGet - get the maximum size of symbol value
*
* This routine gets the maximum size in bytes a symbol value may consume.
* This is useful for symbol types with dynamic length.
*
* RETURNS: maximum size of symbol value in bytes.
*/

unsigned long httpSymValMaxSizeGet
    (
    HTTP_SYM_ID   symId
    )
    {
    unsigned long ulMaxSize;

    httpSymLock ( symId );
    ulMaxSize = symId->ulMaxSize;
    httpSymUnlock ( symId );
    return ulMaxSize;
    }


/*******************************************************************************
*
* httpSymValToString - convert a symbol value to a string
*
* This routine converts a symbol's value to a string
* and stores the result in <szBuffer>.  The <ulItem> argument determines
* which item of
* the symbol value is converted.
*
* RETURNS: <szBuffer> if the conversion succeeded, or NULL otherwise.
*/

char * httpSymValToString
    (
    HTTP_SYM_ID   symId,      /* symbol id                               */
    char *        szBuffer,   /* buffer to store results in              */
    unsigned long ulItem      /* first item to be converted (normally 0) */
    )
    {
    HTTP_SYM_TYPE_HANDLER_ID pHandler;
    char *                   szResult = NULL;

    if ( szBuffer != NULL )
	szBuffer[0] = 0;

    if ( symId != NULL )
	{
	httpSymLock ( symId );
	if ( httpSymAccessRead ( symId ) == HTTP_TRUE )
	    if ( ( pHandler = httpGenListDataGet ( httpSymTypeGet (symId) ) ) != NULL )
		{
		/* XXX add support to multi-item symbols! */
		httpSymValGet ( symId );
		szResult = pHandler->pSymValToString ( symId , ulItem , szBuffer );
		}
	httpSymUnlock ( symId );
	}
    return szResult;
    }


/*******************************************************************************
*
* httpSymValToStringCopy - convert a symbol's value to a string
*
* This routine converts a symbol's value to a string. For network transmissions 
* of symbols you need to convert the binary type information to a string to 
* get architecture independent. This routine does this conversion using 
* httpSymValToString(), but also handles memory requirements for the 
* resulting string.
*
* NOTE: httpSymValToStringCopy() allocates memory from the request memory pool
* for the result, so please free this memory after use!
*
* RETURNS: pointer to szBuffer or NULL if an incorrect type was passed
*
* SEE ALSO: httpSymValToString()
*/

char * httpSymValToStringCopy
    (
    HTTP_REQ_ID   reqId,  /* request id of this HTTP server thread   */
    HTTP_SYM_ID   symId,  /* symbol id                               */
    unsigned long ulItem  /* first item to be converted (normally 0) */
    )
    {
    char *                   szTmp;
    HTTP_SYM_TYPE_HANDLER_ID pHandler;
    char *                   szBuffer = NULL;
    
    if ( symId != NULL )
	{
	httpSymLock ( symId );
	logInfoMsg ( "httpSymValToStringCopy: alloc %l bytes",httpSymValStringSizeGet ( symId ));
	if ( httpSymAccessRead ( symId ) == HTTP_TRUE )
	    if ( ( pHandler = httpGenListDataGet ( httpSymTypeGet (symId) ) ) != NULL )
		{
		httpSymValGet ( symId );
		if ( ( szBuffer = memPoolAlloc( httpReqMemPartIdGet (reqId) , httpSymValStringSizeGet ( symId ) ) ) != NULL )
		    {
		    /* XXX add support to multi-item symbols! */
		    if ( ( szTmp = pHandler->pSymValToString ( symId , ulItem , szBuffer ) ) == NULL )
			{
			memPoolFree( httpReqMemPartIdGet (reqId) , szBuffer );
			szBuffer = NULL;
			}
		    }
		}
	httpSymUnlock (symId);
	logInfoMsg ( "httpSymValToStringCopy: >%s<",szBuffer );
	}
    return szBuffer;
    }


/*******************************************************************************
*
* httpSymValSetByString - set a symbol's value using a string
*
* This routine sets a symbol's value using a string for the new value.
* Depending on the symbol type, the string is translated into the
* appropriate binary representation, such as an integer or float.
*
* NOTE: The new symbol value overwrites the existing symbol
* value. Therefore, ensure that there is already some value
* registered to the symbol.
*
* RETURNS: A pointer to the binary representation of the symbol value.
*/

void * httpSymValSetByString
    (
    HTTP_SYM_ID   symId,   /* symbol id                         */
    char *        szValue, /* new value of the symbol as string */
    unsigned long ulItem   /* first item to be set              */
    )
    {
    HTTP_SYM_TYPE_HANDLER_ID pHandler;
    void *                   pReturn = NULL;

    if ( (symId != NULL) && (szValue != NULL) )
	if ( ( pHandler = httpGenListDataGet ( httpSymTypeGet (symId) ) ) != NULL )
		{
		httpSymLock ( symId );
		if ( httpSymAccessWrite ( symId ) == HTTP_TRUE )
		    if ( symId->pAddr != NULL )
			/* XXX multi-item symbol support to be done! */
			if ( pHandler->pSymValSetByString( symId , ulItem , szValue ) == HTTP_TRUE )
			    if ( httpSymValSet ( symId , symId->pAddr ) == HTTP_OK )
				pReturn = symId->pAddr;
		httpSymUnlock ( symId );
		}
    return pReturn;
    }


/*******************************************************************************
*
* httpSymTypeHandlerAdd - register a new symbol type handler package
*
* This routine registers new symbol type handlers which can be identified
* by <szSymType>. The identifier must be unique.
*
* NOTE:
* Please note that handlers must be registered during initialization,
* as there is no synchronization support like semaphores!
*
* The signatures of the handler functions are defined as follows:
*
*         generic function to determine a symbol types storage size in bytes
* typedef unsigned long (* HTTP_SYM_VAL_SIZE_HANDLER_FUNC)(HTTP_SYM_ID symId);
*         generic function to determine a symbol types string size in bytes
* typedef unsigned long (* HTTP_SYM_VAL_STRING_SIZE_HANDLER_FUNC)(HTTP_SYM_ID symId);
*         generic function to transform symbol value to string
* typedef char * const (* HTTP_SYM_VAL_TO_STRING_HANDLER_FUNC)(HTTP_SYM_ID symId,unsigned long ulItem,char * szBuffer);
*         generic function to set the symbol value using strings
* typedef HTTP_BOOL (* HTTP_SYM_VAL_SET_BY_STRING_HANDLER_FUNC)(HTTP_SYM_ID symId,unsigned long ulItem,char * szValue);
*
*         generic function to print the symbol value using efficient encoding
* typedef HTTP_STATUS (* HTTP_SYM_VAL_TO_NET_PRINT_HANDLER_FUNC)(HTTP_OSAL_IO_HNDL_ID ioHandleId,HTTP_SYM_ID symId);
*         generic function to determine a symbol types network encoding size in bytes
* typedef unsigned long (* HTTP_SYM_VAL_NET_SIZE_HANDLER_FUNC)(HTTP_SYM_ID symId);
*         generic function to set the symbol value using strings
* typedef HTTP_BOOL (* HTTP_SYM_VAL_SET_BY_NET_HANDLER_FUNC)(HTTP_SYM_ID symId,char * szValue);
*
* RETURNS: HTTP_SYM_TYPE_ID of successfully registered handler, or
*          NULL if handler list was already full
*
* SEE ALSO: httpSymTypeHandlerFind()
*/
HTTP_SYM_TYPE_ID httpSymTypeHandlerAdd
    (
    char *                                   szSymType,
    HTTP_SYM_VAL_SIZE_HANDLER_FUNC           pSymValSize,
    HTTP_SYM_VAL_STRING_SIZE_HANDLER_FUNC    pSymValStringSize,
    HTTP_SYM_VAL_TO_STRING_HANDLER_FUNC      pSymValToString,
    HTTP_SYM_VAL_SET_BY_STRING_HANDLER_FUNC  pSymValSetByString,
    HTTP_SYM_VAL_NET_SIZE_HANDLER_FUNC       pSymValNetSize,
    HTTP_SYM_VAL_TO_NET_PRINT_HANDLER_FUNC   pSymValToNetPrint,
    HTTP_SYM_VAL_SET_BY_NET_HANDLER_FUNC     pSymValSetByNet
    )
    {
    HTTP_GEN_LIST_ENTRY_ID  typeId;
    HTTP_SYM_TYPE_HANDLER * pHandler;

    if ( (szSymType!=NULL) && (pSymValSize!=NULL) && (pSymValStringSize!=NULL) && (pSymValToString!=NULL) && (pSymValSetByString!=NULL) )
	{
	if ( ( pHandler = memPoolAlloc ( httpGlobalMemPartIdGet(), sizeof (HTTP_SYM_TYPE_HANDLER)) ) != NULL )
	    {
	    pHandler->pSymValSize = pSymValSize;
	    pHandler->pSymValStringSize = pSymValStringSize;
	    pHandler->pSymValToString = pSymValToString;
	    pHandler->szSymType = szSymType;
	    pHandler->pSymValSetByString = pSymValSetByString;
	    
	    pHandler->pSymValToNetPrint = pSymValToNetPrint;
	    pHandler->pSymValNetSize = pSymValNetSize;
	    pHandler->pSymValSetByNet = pSymValSetByNet;

	    if ( ( typeId = httpGenListDataAppend ( httpSymTypeListId, szSymType, pHandler ) ) != NULL )
		return typeId;
	    
	    logErrorMsg ( "httpSymTypeHandlerAdd: can not add %s, handler list full!" , szSymType );
	    memPoolFree ( httpGlobalMemPartIdGet() , pHandler );
	    }
	logErrorMsg ( "httpSymTypeHandlerAdd: can not add %s, no memory!" , szSymType );
	}
    logErrorMsg ( "httpSymTypeHandlerAdd: %s - invalid arguments!" , szSymType );
    return NULL;
    }

    
/*******************************************************************************
*
* httpSymTypeHandlerFind - find a symbol type handler by name
*
* This routine performs a search in the list of registered symbol type handlers
* and returns the id of the handler found.
*
* RETURNS: HTTP_SYM_TYPE_ID of handler found, or
*          NULL if not such handler exists.
*
* SEE ALSO: httpSymTypeHandlerAdd()
*/

HTTP_SYM_TYPE_ID httpSymTypeHandlerFind
    (
    char * szType /* string identifier used for httpSymTypeHandlerAdd() */
    )
    {
    return httpGenListFind ( httpSymTypeListId, szType );
    }


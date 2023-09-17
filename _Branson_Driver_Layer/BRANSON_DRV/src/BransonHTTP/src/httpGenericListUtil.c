/* httpGenericListUtil.c - Utilities for the configuration lists */

/*
 * Copyright (c) 1996-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01q,26sep14,r_w  fix static CHECKED_RETURN issue.
01p,12jan11,m_z validate memory before using
                CQID: Fix WIND00250286
01o,25jun08,q_z  implementation of config file save and load(WIND00042066)
01n,22feb07,ps   apigen reformatting
01m,13jan05,gsr  added memset to zero out alloced memory in 
                 httpGenListCreate() SPR 100500 and SPR 100501
01l,13jan04,gsr  removed unnecessary lock for SPR 93009
01k,07nov03,adb  added httpGenListLock () input pointer validity check
01j,11aug03,jws  HTTP_BOOL -> BOOL and minor tweaks
01i,10feb00,csr  updating documentation API
01h,20dec99,csr  adding strlen test to httpGenListFind() and
                 httpGenListCaseFind()
01g,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
02g,03dec98,mb   extension: case insensitive search function
                 httpGenListCaseFind() added.
02f,20apr98,mb   bugfix: httpGenListFind() did not find free list entries.
02e,14apr98,mb   generic lists preserve entry order even after multiple
                 append/remove cycles.
02d,27jun97,mb   bugfix: added missing httpOsalSemDelete() to
                 httpGenListDestroy().
02c,14may97,mb   osal introduced.
02b,13mar97,mb   bugfix: httpGenListRemove did not remove items in some cases.
02a,10mar97,mb   replaced table administration by generic list package.
01a,18oct96,ck   written.
*/

/*
DESCRIPTION
This module provides generic list management utilities.

INCLUDE FILES:
	wm.h
	memPoolLib.h
 	httpType.h
	httpSem.h
	httpList.h
	httpLib.h

*/

#include "stdlib.h"
#include <string.h>

#include "wm.h"
#include "memPoolLib.h"

#include "httpType.h"
#include "httpSem.h"
#include "httpList.h"
#include "httpLib.h"




/**************************************************************************
*
* httpGenListPredecessorFind - Find entry ID before the specified entry.
*
* This routine is for internal use only and will find the entry residing in
* the list before another specified entry.
* If the specified entry id was invalid (e.g. NULL), the id of the last entry
* will be returned.
*
* CAUTION: listId MUST NOT be NULL !
*
* NOMANUAL
*
* RETURNS: entry id or
*          NULL if list was empty
*
*/

LOCAL HTTP_GEN_LIST_ENTRY_ID httpGenListPredecessorFind
    (
    HTTP_GEN_LIST_ID       listId,
    HTTP_GEN_LIST_ENTRY_ID entryId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID nextId;
    HTTP_GEN_LIST_ENTRY_ID predId;

    if  (NULL == listId)
        {
        return NULL;
        }

    nextId = predId = listId->firstId;

    while ( ( nextId != entryId ) && ( nextId != NULL ) )
        {
        predId = nextId;
        nextId = nextId->nextId;
        }

    return predId;
    }


/**************************************************************************
*
* httpGenListFreeFind - Find a free list entry.
*
* This routine finds an unused entry.
*
* NOTE: Any list locking must be done before this function is called.
*
* NOMANUAL
*
* RETURNS: pointer to free entry, or
*          NULL if list is full.
*
*/

LOCAL HTTP_GEN_LIST_ENTRY_ID httpGenListFreeFind
    (
    HTTP_GEN_LIST_ID listId
    )
    {
    long usIndex = -1;

    if  (NULL == listId)
        {
        return NULL;
        }

    if ( listId->ulMax > listId->ulUsed )
        {
        while ( ++usIndex < listId->ulMax )
            {
            if ( listId->pData[usIndex].szId == NULL )
                return &listId->pData[usIndex];
            }
        }

    return NULL;
    }


/**************************************************************************
*
* httpGenListAppend - Append an entry to the list.
*
* This routine appends an entry to the list.
*
* NOTE: Any list locking must be done before this function is called.
*
* NOMANUAL
*
* RETURNS: Pointer to the appended entry, or
*          NULL if list is full.
*
*/

LOCAL HTTP_GEN_LIST_ENTRY_ID httpGenListAppend
    (
    HTTP_GEN_LIST_ID listId,
    char *           szId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID newEntryId;
    HTTP_GEN_LIST_ENTRY_ID lastEntryId;

    if  ( szId == NULL )
        {
        return NULL;
        }

    newEntryId = httpGenListFreeFind(listId);
    if ((newEntryId != NULL) && (listId != NULL))
        {
        newEntryId->szId = szId;
        newEntryId->nextId = NULL;
        listId->ulUsed++;

        if  ( listId->firstId != NULL )
            {
            /* add non-first element */
            lastEntryId = httpGenListPredecessorFind ( listId , NULL );
            lastEntryId->nextId = newEntryId;
            }
        else
            {
            /* add first element */
            listId->firstId = newEntryId;
            }
        }

    return newEntryId;
    }


/**************************************************************************
*
* httpGenListUnlock - Unlock a list.
*
* This routine unlocks a list that was locked.
*
* RETURNS: HTTP_OK if the list initialized for locking the return of
* httpOsalSemGive() (HTTP_OK or HTTP_ERROR), otherwise.
*
* SEE ALSO: httpGenListLock()
*/

HTTP_STATUS httpGenListUnlock
    (
    HTTP_GEN_LIST_ID listId   /* List identifier */
    )
    {
    if  (NULL == listId)
        {
        return (HTTP_ERROR);
        }

    return ((HTTP_STATUS)((listId->semLock == NULL ) ?
             HTTP_OK : httpOsalSemGive ( listId->semLock )) );
    }


/**************************************************************************
*
* httpGenListLock - Lock a list by mutual exclusion.
*
* This routine locks a list to avoid conflicts between tasks.
*
* RETURNS: HTTP_OK if the list initialized for locking the return of
* httpOsalSemTake() (HTTP_OK or HTTP_ERROR), otherwise.
*
* SEE ALSO: httpGenListUnlock()
*/

HTTP_STATUS httpGenListLock
    (
    HTTP_GEN_LIST_ID listId    /* List identifier */
    )
    {
    if  (NULL == listId)
        {
        return (HTTP_ERROR);
        }

    return ((HTTP_STATUS)((listId->semLock == NULL ) ?
            HTTP_OK : httpOsalSemTake ( listId->semLock ) ));
    }


/**************************************************************************
*
* httpGenListCreate - Create and initialize a list.
*
* This routine creates and initializes a list. If memory allocation fails or if
* <usMaxItems> is equal to 0, the routine returns NULL. Also, the
* routine tries to allocate the required memory for all future items in the list.
* If this operation fails, NULL is returned.
*
* RETURNS: ID of the new generic list which is a pointer to an HTTP_GEN_LIST
* structure or NULL on failure.
*
*/

HTTP_GEN_LIST_ID httpGenListCreate
    (
    MEM_POOL_ID memPartId,      /* pool to allocate memory from            */
    ubyte2      usMaxItems,     /* maximum number of items in  list        */
    BOOL        fUseSemaphore   /* TRUE if list should use mutex semaphore */
    )
    {
    HTTP_GEN_LIST_ID listId;

    if ( usMaxItems > 0 )
        {

        /* create the list header */

        listId = memPoolAlloc( memPartId, sizeof(HTTP_GEN_LIST));
        if (listId != NULL)
            {

            /* allocate space for the entries */

            listId->pData = memPoolAlloc (memPartId,
                                          (ubyte4) usMaxItems *
                                          sizeof(HTTP_GEN_LIST_ENTRY));


            if (listId->pData != NULL )
                {
                memset (listId->pData,0,(ubyte4) usMaxItems *
                                           sizeof(HTTP_GEN_LIST_ENTRY));

                listId->ulMax     = usMaxItems;
                listId->memPartId = memPartId;
                listId->ulUsed    = 0;
                listId->firstId   = NULL;

                /* allocate a semaphore, if needed */

                if (fUseSemaphore)
                    {
                    listId->semLock = httpOsalSemMCreate(HTTP_SEM_Q_PRIORITY);
                    if (listId->semLock == NULL)
                        {
                        (void)memPoolFree ( memPartId, listId->pData);
                        (void)memPoolFree ( memPartId, listId);
                        return NULL;
                        }
                    }
                else
                    listId->semLock = NULL;

                return listId;   /* only non-error return */
                }
            else
                {
                (void)memPoolFree ( memPartId, listId );
                }
            }
        }

    return NULL;
    }


/**************************************************************************
*
* httpGenListFind - Find a list entry by ID string.
*
* This routine finds an entry in list, <listId>, identified by <szKey>.
* If a NULL pointer is used as <szKey>, the next available position in
* the table will be returned.  An HTTP_GEN_LIST_ENTRY_ID is returned on success.
* This is a pointer to an HTTP_GEN_LIST_ENTRY structure.
*
* NOTE: The list is not locked by this function.
*
* RETURNS: A pointer to the entry found, or NULL if the entry does
* not exist; or if <szKey> is NULL, a pointer to next free entry,
* or NULL if the table is full.
*
* SEE ALSO: httpGenListCaseFind()
*/

HTTP_GEN_LIST_ENTRY_ID httpGenListFind
    (
    HTTP_GEN_LIST_ID listId,    /* the ID of the HTTP_GEN_LIST to search */
    char *           szKey      /* the key of the entry to find or NULL  */
    )
    {
    unsigned long ulKeyLen;
    HTTP_GEN_LIST_ENTRY_ID entryId;

    if ( listId != NULL )
    if ( szKey != NULL )
        {
        ulKeyLen = strlen(szKey);
        for ( entryId = listId->firstId ; entryId != NULL ; entryId = entryId->nextId )
            {
            if ( (entryId->szId != NULL) && (strlen(entryId->szId) == ulKeyLen) )
                {
                if ( ( entryId->szId == szKey ) || strcmp ( entryId->szId , szKey ) == 0 )
                    {
                    return ( entryId );
                    }
                }
            }
        }

    /* XXX - what about szKey == NULL processing?? */

    return NULL;
    }


/**************************************************************************
*
* httpGenListCaseFind - Find a list entry by ID string which is case insensitive.
*
* This routine finds an entry in a list identified by <szKey> ignoring case.
* If a NULL pointer is used as <szKey>, the next available position in
* the table will be returned.
*
* NOTE: List locking is not performed by this function.
*
* RETURNS: A pointer to the entry found, or NULL if the entry does
* not exist; or if <szKey> is NULL,
* a pointer to next free entry, or NULL if the table is full.
*
* SEE ALSO: httpGenListFind()
*/

HTTP_GEN_LIST_ENTRY_ID httpGenListCaseFind
    (
    HTTP_GEN_LIST_ID listId,     /* the ID of the HTTP_GEN_LIST structure  */
                                 /* to search                              */
    char *           szKey       /* the key of the entry to find or NULL   */
    )
    {
    unsigned long ulKeyLen;
    HTTP_GEN_LIST_ENTRY_ID entryId;

    if ( listId != NULL )
    if ( szKey != NULL )
        {
        ulKeyLen = strlen ( szKey );
        for ( entryId = listId->firstId ; entryId != NULL ; entryId = entryId->nextId )
                {
                if ( (entryId->szId != NULL) && (strlen(entryId->szId) == ulKeyLen) )
                    {
            if ( httpStrNCaseCmp ( entryId->szId , szKey , ulKeyLen ) == 0 )
            {
            return ( entryId );
            }
                    }
                }
        }

    return NULL;
    }


/*******************************************************************************
*
* httpGenListFuncAppend - Add a new entry for a function pointer.
*
* This routine stores an identification string and a function pointer, <pFunc>
* together in <listId>.  If a pointer to common data must be stored, use
* httpGenListDataAppend() instead (some hardware platforms use different sizes
* for data and function pointers).
*
* NOTE: <szId> must not be NULL.
*
* RETURNS: HTTP_GEN_LIST_ID of the new list entry, or
* NULL if there is insufficient room in the list for more entries.
*
* SEE ALSO: httpGenListFuncGet()
*/

HTTP_GEN_LIST_ENTRY_ID httpGenListFuncAppend
    (
    HTTP_GEN_LIST_ID listId,       /* the ID of the HTTP_GEN_LIST to add the */
                                   /* function pointer                       */
    char * szId ,                  /* the key to be used as a future         */
                                   /* reference to <pFunc>                   */
    HTTP_FUNCPTR pFunc             /* the function pointer to store          */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID newEntryId;

    if ( listId == NULL )
    {
    logError ( "httpGenListFuncAppend: list id is NULL!" );
    return NULL;
    }

    (void) httpGenListLock ( listId );

    if ( ( newEntryId = httpGenListAppend ( listId , szId ) ) != NULL )
    newEntryId->listItem.pFunc = pFunc;

    (void) httpGenListUnlock ( listId );
    return newEntryId;
    }


/*******************************************************************************
*
* httpGenListFuncGet - Get a function pointer from a list.
*
* This routine returns the function pointer stored in a
* list entry.
*
* ATTENTION:
* Do not call httpGenListFuncGet() with an <entryId>
* created by httpGenListDataAppend() or vice versa. Attempting
* dereference the function handle with a data pointer could
* cause a runtime exception.
*
* RETURNS: The function pointer stored by httpGenListFuncAppend(),
* or NULL if <entryId> is NULL.
*
* SEE ALSO: httpGenListFuncAppend()
*/

HTTP_FUNCPTR httpGenListFuncGet
    (
    HTTP_GEN_LIST_ENTRY_ID entryId     /* the entry containing the function */
                                       /* pointer to get                    */
    )
    {
    if ( entryId != NULL )
    return entryId->listItem.pFunc;
    return NULL;
    }


/*******************************************************************************
*
* httpGenListDataAppend - Create a new entry in a list for a data pointer.
*
* This routine stores an identification string with a data pointer in list
* <listId>.  If a pointer to a function should be stored, use
* httpGenListFuncAppend() instead (some hardware platforms use different
* sizes for data and function pointers).
*
* NOTE:
* <pData> may be a NULL pointer and still be valid, but <szId> must not be
* NULL.
*
* RETURNS: HTTP_GEN_LIST_ID of the new list entry, or
* NULL if there is insufficient room in the list for more entries.
*
* SEE ALSO: httpGenListDataGet()
*/

HTTP_GEN_LIST_ENTRY_ID httpGenListDataAppend
    (
    HTTP_GEN_LIST_ID listId,     /* the ID of the HTTP_GEN_LIST where to */
                                 /* store the data                       */
    char * szId,                 /* the key to use as future reference   */
                                 /* for the data                         */
    void * pData                 /* the data to store                    */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID newEntryId;

    if ( listId == NULL )
    {
    logError ( "httpGenListDataAppend: list id is NULL!" );
    return NULL;
    }

    (void) httpGenListLock ( listId );
    if (( newEntryId = httpGenListAppend ( listId , szId ) ) != NULL )
    newEntryId->listItem.pData = pData;
    (void) httpGenListUnlock ( listId );

    return newEntryId;
    }


/*******************************************************************************
*
* httpGenListDataGet - Get a data pointer out of a list entry.
*
* This routine returns a data pointer from a list entry.  The pointer returned
* will be NULL if no data was added to <entryId> using httpGenListDataAppend().
*
*
* ATTENTION:
* Do not call httpGenListDataGet() with an <entryId>
* created by httpGenListFuncAppend() or vice versa. Attempting
* to dereference the data with a function pointer could
* cause a runtime exception.
*
* RETURNS: The data pointer previously added with httpGenListDataAppend(),
* or NULL if <entryId> was NULL.
*
* SEE ALSO: httpGenListDataAppend()
*/

void * httpGenListDataGet
    (
    HTTP_GEN_LIST_ENTRY_ID entryId    /* the entry containing the data */
                                      /* pointer to return             */
    )
    {
    if ( entryId != NULL )
    return entryId->listItem.pData;
    return NULL;
    }


/*******************************************************************************
*
* httpGenListIdGet - Get the ID string of a list entry.
*
* This routine returns the string used to identify a list entry.  The string can
* then be used to find <entryId> using httpGenListFind() or
* httpGenListCaseFind().
*
* RETURNS: The identification string of the list entry, or
* NULL if <entryId> was NULL.
*/

char * httpGenListIdGet
    (
    HTTP_GEN_LIST_ENTRY_ID entryId    /* the ID of the HTTP_GEN_LIST_ENTRY */
                                      /* whose key will be returned        */
    )
    {
    if ( entryId != NULL )
    return entryId->szId;
    return NULL;
    }


/*******************************************************************************
*
* httpGenListItemsGet - Get the number of items currently stored in a list.
*
* This routine gets the number of items currently stored in the HTTP_GEN_LIST
* referred to by <listId>. This will be less then or equal to the maximum
* number of items in the list.
*
* ATTENTION: <listId> must NOT be NULL.
*
* RETURNS: The number of items currently stored in the list or 0 if the list is
* empty.
*/

unsigned short httpGenListItemsGet
    (
    HTTP_GEN_LIST_ID listId     /* the ID of the HTTP_GEN_LIST whose number */
                                /* of used items will be returned           */
    )
    {
    if  (NULL == listId)
        {
        return 0;
        }

    return listId->ulUsed;
    }


/*******************************************************************************
*
* httpGenListMemPartIdGet - Get the memory partition ID of the list header.
*
* This routine gets the memory partition ID of the specified list, <listId>.
* The memory partition ID is that used by httpGenListCreate() to initialize
* <listId>.
*
* ATTENTION: <listId> must NOT be NULL.
*
* RETURNS: The memory partition ID of the list.
*/

MEM_POOL_ID httpGenListMemPartIdGet
    (
    HTTP_GEN_LIST_ID listId
    )
    {
    if  (NULL == listId)
        {
        return NULL;
        }

    return listId->memPartId;
    }


/*******************************************************************************
*
* httpGenListRemove - Remove an entry from a list.
*
* This routine removes an entry from a list.  If either <listId> or <entryId>
* are NULL, nothing is done.  The HTTP_GEN_LIST, pointed to by <listId>, is
* locked with httpGenListLock() during the removal operation. The memory of the
* HTTP_GEN_LIST_ENTRY_ID is freed using memset(), but the data stored in
* <entryId> is NOT freed.
*
* NOTE:
* The order of the remaining items is not changed by this operation.
*
* RETURNS: N/A
*/

void httpGenListRemove
    (
    HTTP_GEN_LIST_ID        listId, /* list to remove entry from */
    HTTP_GEN_LIST_ENTRY_ID  entryId /* entry to remove from list */
    )
    {
    HTTP_GEN_LIST_ENTRY_ID predId;

    if ( ( listId != NULL ) && ( entryId != NULL ) )
    {
    (void) httpGenListLock ( listId );
    logInfoMsg ( "httpGenListRemove: remove item %s\n",entryId->szId);
    if ( listId->firstId == entryId )
        {
        /* remove first member of list */
        listId->firstId = entryId->nextId;
        }
    else
        {
        /* remove non-first member */
        predId = httpGenListPredecessorFind ( listId , entryId );
        if ( predId->nextId == entryId )
        predId->nextId = entryId->nextId;
        }
    --(listId->ulUsed);
    memset ( entryId , 0 , sizeof ( HTTP_GEN_LIST_ENTRY ) );
    (void) httpGenListUnlock( listId );
    }
    else
    {
    logError ( "httpGenListRemove: invalid arguments!" );
    }
    }


/*******************************************************************************
*
* httpGenListDestroy - Destroys a list and all of its entries.
*
* This routine deletes all list entries and then destroys the list
* itself.  After this call, the <listId> is invalid.
*
* ATTENTION:
* The list is locked with httpGenListLock(), but it is never unlocked. In fact,
* the semaphore is deleted with httpOsalSemDelete(). So, be
* sure that no other tasks are blocking on httpGenListLock() before calling this
* routine.
*
* RETURNS: N/A
*
* SEE ALSO: httpGenListRemove(), httpOsalSemDelete()
*/

void httpGenListDestroy
    (
    HTTP_GEN_LIST_ID listId
    )
    {
    HTTP_GEN_LIST_ENTRY_ID entryId;
    MEM_POOL_ID       memPartId;

    if  (NULL == listId)
        {
        return;
        }

    logInfo ( "httpGenListDestroy: removing list." );

    (void) httpGenListLock (listId);
    while ( ( entryId = listId->firstId ) != NULL )
    httpGenListRemove ( listId , entryId );

    if  (NULL != listId->semLock)
        {
        (void) httpOsalSemDelete ( listId->semLock );
        listId->semLock = NULL;
        }

    memPartId = listId->memPartId;
    (void)memPoolFree (memPartId, (void *) (listId->pData));
    (void)memPoolFree (memPartId, (void *) listId);

    return;

    } /* httpGenListDestroy */

/*******************************************************************************
*
* httpGenListIterator - Iterate through all entries of a list.
*
* This routine is called with an HTTP_GEN_LIST_ID, <listId>, and a pointer to
* an HTTP_GEN_LIST_ENTRY_ID. If the <pEntryId> is not NULL, then the next
* HTTP_GEN_LIST_ENTRY_ID in the list is returned (which might also be NULL if
* <pEntryId> refers to the last HTTP_GEN_LIST_ENTRY_ID in the list).
*
* On a subsequent call, with the return of this routine, the next
* HTTP_GEN_LIST_ENTRY_ID will be returned.
*
* EXAMPLE:
*    HTTP_GEN_LIST_ID entryId = NULL;
*    int i = 0;
*
*    while (httpGenListIterator(someListId, &entryId) != NULL)
*        {
*        printf ("ENTRY(%d)'s key is %s\n", i++, httpGenListIdGet(entryId));
*        }
* NOTE:
* If <pEntryId>[0] does not index a valid entry (i.e., it is NULL), it
* is set to the first entry in the HTTP_GEN_LIST referred to by <listId>. Also
* note that no locking is performed.
*
* RETURNS: The entry specified with pulIndex[0] or
* NULL if pulIndex[0] was greater than the actual number
* of entries stored in the list.
*/

HTTP_GEN_LIST_ENTRY_ID httpGenListIterator
    (
    HTTP_GEN_LIST_ID         listId,  /* list id                     */
    HTTP_GEN_LIST_ENTRY_ID * pEntryId /* pointer to current entry id */
    )
    {
    if  ( pEntryId != NULL )
        {
        if  ( pEntryId[0] == NULL )
            {
            if  (NULL == listId)
                {
                return NULL;
                }
            pEntryId[0] = listId->firstId;
            }
        else
            {
            pEntryId[0] = pEntryId[0]->nextId;
            }
        return pEntryId[0];
        }
    return NULL;
    }


/*******************************************************************************
*
* httpGenListIndexIterator - Iterate through all entries of a list.
*
* The iterator function returns the entry indexed by the value
* contained in <pusIndex>.  If the <pusIndex> points to a value greater
* than the number of items in <listId>, the routine will return
* NULL. The short pointed to by <pusIndex> will continue to be decremented
* until it becomes zero. At which point, either NULL or a valid
* HTTP_GEN_LIST_ENTRY_ID will be returned.
*
* If the index is 0, the first entry ID of the specified list is returned.
*
* This function performs no locking of the list it is working on.
*
* NOTE: This function calls httpGenListIterator internally and is rather slow
*       if used with long lists. <pusIndex> must NOT be NULL.
*
* RETURNS: HTTP_GEN_LIST_ID in (*pusIndex)'s position or
*          NULL if the last element had been already accessed.
*
* SEE ALSO: httpGenListIterator()
*/
HTTP_GEN_LIST_ENTRY_ID httpGenListIndexIterator
    (
    HTTP_GEN_LIST_ID   listId,  /* list id                  */
    unsigned short   * pusIndex /* pointer to current index */
    )
    {
    unsigned short         usIndex;
    HTTP_GEN_LIST_ENTRY_ID entryId = NULL;

    if  (NULL == pusIndex)
        {
        return NULL;
        }
    usIndex = *pusIndex;
    logInfoMsg ( "httpGenListIndexIterator: usIndex=%hd",usIndex );
    while ( ( httpGenListIterator (listId,&entryId) != NULL ) && ( usIndex != 0 ) )
    usIndex--;

    if ( *pusIndex < httpGenListItemsGet(listId) )
    *pusIndex = (unsigned short)(*pusIndex + 1);

    return entryId;
    }


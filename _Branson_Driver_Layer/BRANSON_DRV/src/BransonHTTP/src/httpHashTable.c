/* httpHashTable.c - hash table package */

/* Copyright 1999 - 2007,2010 Wind River Systems, Inc. */
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
01d,22feb07,ps  apigen reformatting
01c,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01b,14may97,mb   osal introduced.
01a,26nov96,mb   initial creation.
*/

/*
DESCRIPTION
This utility package implements hash lists using double hashing.

INCLUDE FILES: http/httpType.h
               http/httpHash.h

NOMANUAL
*/


#include "stdlib.h"
#include <string.h>

#include "wm.h"

#include "wm.h"

#include "httpType.h"
#include "httpHash.h"
#include "memPoolLib.h"


/*******************************************************************************
*
* httpHashStdFunc - standard hashing routine
*
* This routine is the default hashing routine if you provide NULL
* as function pointer to httpHashCreate().  It uses Horner's method and an
* alphabet size of 8 bits to compute the hash index from the whole
* key <szKey>.
*
* NOMANUAL
*
* RETURNS: A position in the hash table.
*/

static unsigned long httpHashStdFunc
    (
    HTTP_HASH_TABLE_ID httpHashTabId,  /* the hash table to index into */
    char *        szKey       /* the key to hash */
    )
    {
    short         i = -1;
    unsigned long ulIndex = 0;

    while ( szKey[++i] != 0 )
        ulIndex = ( (ulIndex << 8) + szKey[i] ) % httpHashTabId->ulMax;

    return ulIndex;
    }


/*******************************************************************************
*
* secondHttpHashStdFunc - standard double-hash hashing routine
*
* This routine provides a standard double-hash hashing function.
*
* NOMANUAL
*
* RETURNS: A position in the hash table.
*/

static unsigned long secondHttpHashStdFunc
    (
    HTTP_HASH_TABLE_ID hashTabId,   /* the hash table to index into */
    unsigned long ulIndex
    )
    {
    return ( hashTabId->sHashOffset + ulIndex ) % hashTabId->ulMax;
    }


/*******************************************************************************
*
* httpHashInit - initialize the hash table package
*
* This routine initialized the hash table feature.  It should be called
* once, during system startup.
*
* NOMANUAL
*
* RETURNS: HTTP_OK if initialization was successful, HTTP_ERROR otherwise.
*/

HTTP_STATUS httpHashInit
    (
    void
    )
    {
    return HTTP_OK;
    }


/*******************************************************************************
*
* httpHashCreate - initialize a new hash table with a specified number of hash entries
*
* This routine initializes a new hash table with <ulEntries> entries.
* Note that there may no more than <ulEntries> entries,
* and that all hashing operations will slow down if the
* table nears capacity.
*
* <pHashFunc> is a function pointer to a hashing routine or NULL for
* the standard built-in hashing routine.
*
* NOMANUAL
*
* RETURNS: A pointer to the hash table header.
*/

HTTP_HASH_TABLE_ID httpHashCreate
    (
    MEM_POOL_ID  memPartId,  /* Pool where to get the memory from       */
    long              ulEntries,  /* number of entries in the table          */
    HTTP_HASH_FUNC    pHashFunc,  /* function to use to generate a hash      */
                  /* value -- NULL to use a default function */
    unsigned long     sHashOffset /* offset value                            */
    )
    {
    HTTP_HASH_TABLE * pTab;

    if ( (pTab = memPoolAlloc (memPartId, (unsigned long) sizeof (HTTP_HASH_TABLE))) != NULL )
    {
    pTab->ulMax     = ulEntries;
    pTab->memPartId = memPartId;
    if (pHashFunc == NULL)
        {
        pTab->pHashFunc = (HTTP_HASH_FUNC)httpHashStdFunc;
        }
    else
        {
        pTab->pHashFunc = pHashFunc;
        }

    pTab->sHashOffset = sHashOffset;
    if ( (pTab->pTable = memPoolAlloc (memPartId, (ulEntries + 1) * (unsigned long) sizeof (HTTP_HASH))) == NULL )
        {
        memPoolFree (memPartId, (void *) pTab);
        return NULL;
        }
    }

    return pTab;
    }


/*******************************************************************************
*
* httpHashAdd - add an entry with a specified key to the hash table
*
* This routine adds a new hash list entry via a double-hashing
* algorithm.
* 
* NOMANUAL
*
* RETURNS: HTTP_OK if the entry was added successfully, HTTP_ERROR if not.
*/

HTTP_STATUS httpHashAdd
    (
    HTTP_HASH_TABLE_ID hashTabId,   /* hash table to add to */
    char * szKey,       /* key for the new entry */
    void *  pData           /* data to associate with the key */
    )
    {
    unsigned long ulIndex = hashTabId->pHashFunc (hashTabId,szKey);
    HTTP_HASH *        pHash;
    unsigned long i = 0;

    while ( (pHash = &hashTabId->pTable[ulIndex])->szKey != NULL)
    {
    ulIndex = secondHttpHashStdFunc (hashTabId,ulIndex);
    if ( ++i >= hashTabId->ulMax )
        return HTTP_ERROR;
    }

    pHash->pData = (void *)pData;
    pHash->szKey = (char *)szKey;

    return HTTP_OK;
    }


/********************************************************************************
* httpHashGet - get the first entry of a specified key from the hash table
*
* This routine returns the data entered into the hash table with
* the key <szKey>.
*
* NOMANUAL
*
* RETURNS: A pointer to the data, or NULL if nothing was found.
*/

void * httpHashGet
    (
    HTTP_HASH_TABLE_ID hashTabId,   /* hash table to look up */
    char * szKey        /* key to search for */
    )
    {
    unsigned long ulIndex = hashTabId->pHashFunc (hashTabId,szKey);
    HTTP_HASH *        pHash;
    unsigned long i = 0;

    while ( ( (pHash = &hashTabId->pTable[ulIndex])->szKey != NULL) &&
        ++i <= hashTabId->ulMax )
    {
    if (strcmp (pHash->szKey,szKey) == 0)
        return pHash->pData;

    ulIndex = secondHttpHashStdFunc (hashTabId, ulIndex);
    }

    return NULL;
    }


/*******************************************************************************
*
* httpHashIterator - step through a hash table
*
* This routine runs through the hash table beginning with the index
* <pulIndex> and returns the key of the next entry if it is not empty.
*
* NOMANUAL
*
* RETURNS: A pointer to the next key, or NULL if the end of the table
* has been reached.
*/

char * httpHashIterator
    (
    HTTP_HASH_TABLE_ID hashTabId,   /* hash table to iterate over  */
    unsigned long *    pulIndex         /* pointer to the stored index */
    )
    {
    while ( *pulIndex < hashTabId->ulMax )
    {
    if (hashTabId->pTable[*pulIndex].szKey != NULL)
        return hashTabId->pTable[(*pulIndex)++].szKey;
    *pulIndex += 1;
    }

    *pulIndex = hashTabId->ulMax;
    return NULL;
    }


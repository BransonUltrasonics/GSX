/* httpFileRegistry.c - Registry routines for files that need special handling. */


/* Copyright 1999-2007 Wind River Systems, Inc. */

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
01a,22feb07,ps  apigen reformatting
02f,02mar05,???  Rebranding part 2..
02e,13sep04,dkg  added new API's to set & get file access level to support
                 JavaScript Authentication feature
02d,28apr04,vmk  added customTimeGet to httpFileRegister(SPR#92408)
02c,14jan04,gsr  cleanup routine for SPR 93009
02b,06oct03,jws  remove (unneeded) inclusion of zlib.h
02a,07aug03,adb  fixed SPR 89575: memory pool free did not match malloc
01c,05feb02,rs   added refgen comments
01b,09jun99,km   merge of Wind Web Server 2.0 code drop from 3Soft.
01a,25sep96,ck   written.
*/

/*
DESCRIPTION:
This module implements registry routines for files that need special handling.
This will typically be all files using the file system type CUSTOM_FS, and any
files in other file systems that require non-default access routines.

The default file access behavior of the web server is to first check the file
registry. Then, if the requested file is not found in the registry, the server
will check the default file system type as specified in httpConf.c. For more
information on file system options see the Wind River CLI, Web, MIBway Programmer's Guide.

Use httpFileRegister() to register any file that has special (non default) file
access routines. Every file that uses the file system type CUSTOM_FS, and any
other file that requires non-default access routines must be registered.



INCLUDE FILES: httpLib.h
	wmLinkListLib.h
	httpFileRegistry.h
*/



#include <stdio.h>
#include <string.h>

#include "httpLib.h"
#include "wmLinkListLib.h"
#include "httpFileRegistry.h"


#define kInitFlag           0xD0B71270
static  OS_SEM_T            mMutex;
static  Counter             mNumReaders;
static  HTTP_GEN_LIST_ID    mplst_Registry;
static  ubyte4              mRegInitFlag;
static  MEM_POOL_ID         mMemPool;

void filesShow(void);
extern void REGISTRY_ReaderEnter(void);
extern void REGISTRY_ReaderExit(void);

/*******************************************************************************
*
* httpFileRegInit - Web Server file registry initialization.
*
* File registry initialization. This is normally called by the http server when
* the server is initialized. You should not need to explicitly call it.
*
* RETURNS: OK
*		ERROR_MEMMGR_NO_MEMORY
*		ERROR_GENERAL
*
*/
extern STATUS httpFileRegInit
	(
	ubyte2 ulMaxItems
	)
    {
    STATUS status;

    if ( ulMaxItems > 0 )
	    {
	    mNumReaders = 0;

        mMemPool = httpGlobalMemPartIdGet();

        if (NULL == mMemPool)
            return wmError(ERROR_MEMMGR_NO_MEMORY,"no mem pool");

        mplst_Registry = httpGenListCreate (mMemPool, ulMaxItems, HTTP_FALSE);
        if (NULL == mplst_Registry)
            return wmError(ERROR_MEMMGR_NO_MEMORY,"no mem");

        status = wmosSemCreate(&mMutex,1,SEM_TYPICAL_K); /* initially available */
        if (OK != status)
            return wmError(status,"sem create error");

        mRegInitFlag = kInitFlag;
	    }

    return OK;

}

/*******************************************************************************
*
* httpFileRegCleanup - cleanup routine for httpFileRegCleanInit
*
* File registry cleanup routine. This is normally called by the http server when
* the server is killed. You should not need to explicitly call it.
*
* RETURNS: OK or ERROR
*/

extern STATUS httpFileRegCleanup
        (
        void
        )
        {
        STATUS rs = OK;

        if  (NULL != mplst_Registry)
            {
            httpGenListDestroy (mplst_Registry);

            mplst_Registry = NULL;
            }

        if  (NULL != mMutex)
            {
            if  (OK != (rs = (wmosSemDestroy (mMutex))))
                {
                wmError (ERROR_GENERAL, "semaphore deletion failed");
                }

            mMutex = NULL;
            }

        return (rs);

        } /* httpFileRegCleanup */

/*-----------------------------------------------------------------------*/
/***************************************************************
*
*  REGISTRY_Lock - 
*
* NOMANUAL
*
*/

LOCAL void REGISTRY_Lock(void)
    {
    wmosSemTake(mMutex, WAIT_FOREVER_K);

    }



/*-----------------------------------------------------------------------*/
/***************************************************************
*
*  REGISTRY_Unlock - 
*
* NOMANUAL
*
*/
LOCAL void REGISTRY_Unlock(void)
    {
    (void)wmosSemGive(mMutex);

    }




/*******************************************************************************
*
* REGISTRY_ReaderEnter - File registry reader enter.
*
* File registry reader enter.
*
* RETURNS: status
* NOMANUAL
*/
extern void REGISTRY_ReaderEnter()
    {
    REGISTRY_Lock();

    mNumReaders++;

    REGISTRY_Unlock();

    }



/*******************************************************************************
*
* REGISTRY_ReaderExit -  File registry reader exit.
*
* File registry reader exit.
*
* RETURNS: n/a
* NOMANUAL
*/
extern void REGISTRY_ReaderExit()
{
    REGISTRY_Lock();

    mNumReaders--;

    REGISTRY_Unlock();

}



/*******************************************************************************
*
* httpFileRegister - Registers a file with the web server file registry.
*
* Use this routine to register any file that requires <content type> or
* access routines <other> than the default values for the file's <fs> type.
*
* In the case of CUSTOM_FS, every file needs to be registered. Since CUSTOM_FS
* does not specify default content type or access routines, each file in
* CUSTOM_FS needs these values specified.
*
* Each file requiring special (non-default) handling must specify, <content type>,
* the file system, <fs>, in which the file exists, and the following six ANSI-like
* access routines.
* \ml
* \m customOpen()	customRead()
* \m customWrite()	customSeek()
* \m customClose()	customSize()
* \me
*
*
* RETURNS: OK
* 	ERROR_MEMMGR_NO_MEMORY if memory is insufficient.
* 	ERROR_GENERAL if the registry is not initialized, or if the system can't
*	register the file.
*
*/
extern  STATUS  httpFileRegister
    (
    sbyte       *pFileName,
    enum fs_e   fs,
    sbyte *     contentType,
    void *      (*customOpen)  (void * , void *),
    unsigned long      (*customRead)  (void * , unsigned long, unsigned long, void *),
    unsigned long      (*customWrite) (void * , unsigned long, unsigned long, void *),
    unsigned long      (*customSeek)  (void *,  long , long),
    unsigned long      (*customTimeGet) (void *,  sbyte* , void *),
    unsigned long      (*customClose) (void *),
    unsigned long      (*customSize)  (void *)
    )
    {
    STATUS  status    = OK;
    RegDes  *p_regNew   = memPoolAlloc(mMemPool, sizeof(RegDes));

    if (NULL == p_regNew)
        return wmError(ERROR_MEMMGR_NO_MEMORY,"no mem");

    if (kInitFlag != mRegInitFlag)
        {
        return wmError(status,"registry not initialized");
        }

    REGISTRY_Lock();

    while (mNumReaders != 0)
        {
        REGISTRY_Unlock();

        wmosYield();

        REGISTRY_Lock();
        }

    /* make sure it doesn't already exist! */
    if (NULL == httpFileRegHandle (pFileName))
        {
        p_regNew->pFileName          = pFileName;
        p_regNew->fs                 = fs;
        p_regNew->customOpen         = customOpen;
        p_regNew->customRead         = customRead;
        p_regNew->customWrite        = customWrite;
        p_regNew->customSeek         = customSeek;
        p_regNew->customTimeGet      = customTimeGet;
        p_regNew->customClose        = customClose;
        p_regNew->customSize         = customSize;
        p_regNew->contentType        = contentType;

        if (NULL == httpGenListDataAppend (mplst_Registry, pFileName,
                                           (void *)p_regNew))
            {
            (void)memPoolFree(mMemPool, p_regNew);
            return wmError(ERROR_GENERAL, "unable to register file");
            }
        }

    else 
        (void)memPoolFree(mMemPool, p_regNew);

    REGISTRY_Unlock();

    return status;

    }   /* httpFileRegister */

#if 0
/*******************************************************************************
*
* REGISTRY_FindObject - Find a registered object.
*
* Find a registered object.
*
* RETURNS: TRUE
*	FALSE
* NOMANUAL
*/
static BOOL REGISTRY_FindObject
	(
	void *pFileName, void *p_regObject
	)
{
    if ((NULL==pFileName) || (NULL==p_regObject))
    {
        return FALSE;
    }

#ifdef __CASE_INSENSITIVE_FILENAMES__
    if (0 == STRICMP(((RegDes *)p_regObject)->pFileName, pFileName))
#else
    if (0 == strcmp(((RegDes *)p_regObject)->pFileName, pFileName))
#endif
        return TRUE;

    return FALSE;
}

#endif

/*******************************************************************************
*
* httpFileRegHandle - Find a files registry handle.
*
* This routine returns the registry handle (registry entry) for <pFileName>.
*
* RETURNS: The registry description for <pFileName>.
*
*
*/
extern RegDes *httpFileRegHandle
	(
	sbyte *pFileName
	)
    {

    HTTP_GEN_LIST_ENTRY_ID listEntry = httpGenListFind (mplst_Registry,
                                                        pFileName);
    if (NULL != listEntry)
        return (RegDes *) httpGenListDataGet (listEntry);
    return NULL;
    }


/*******************************************************************************
*
* REGISTRY_PrivGetNumReaders - Get number of registry readers.
*
* Get number of registry readers.
*
*
* RETURNS: Number of registry readers.
* NOMANUAL
*/
extern Counter REGISTRY_PrivGetNumReaders
	(
	void
	)
{
    return mNumReaders;
}


/* Registry_Remove added by tya 5.08.2000 */

#if 0
/*-----------------------------------------------------------------------*/
static BOOL Registry_Compare(void *p_RegDelete, void *p_ListObject)
{
    BOOL RetVal = ((RegDes *)p_RegDelete == (RegDes *)p_ListObject);

    return RetVal;
}
#endif
/*******************************************************************************
*
* httpFileRegRemove - Remove a file's registry entry.
*
* This routine removes the registry entry for the file registered at <pUrl>.
*
* RETURNS: OK
*	ERROR_GENERAL_NOT_FOUND if the registry entry is not found.
*
*/
extern STATUS httpFileRegRemove
	(
	sbyte *pUrl
	)
{

    HTTP_GEN_LIST_ENTRY_ID listEntryToDelete;
    listEntryToDelete = httpGenListFind (mplst_Registry, pUrl);

    if(listEntryToDelete == NULL)
        return wmError(ERROR_GENERAL_NOT_FOUND,"not found object to remove");

    REGISTRY_Lock();

    while (mNumReaders != 0)
    {
        REGISTRY_Unlock();
        wmosYield();
        REGISTRY_Lock();
    }

    httpGenListRemove (mplst_Registry, listEntryToDelete);

    REGISTRY_Unlock();

    return OK;

}

/*******************************************************************************
*
* httpFileRegGetFd - Get's a file descriptor.
*
* This routine gets a registry entry's file descriptor.
*
* RETURNS: OK
*
*/
extern STATUS httpFileRegGetFd
	(
	RegDes *ph_regObject, generic_fd_t * fd
	)
{
    fd->mimeType = ph_regObject->contentType;
    if (CUSTOM_FS == (fd->fs = ph_regObject->fs))
        {
        fd->pfOpen   = (custom_fopen_t )ph_regObject->customOpen;
        fd->pfRead   = (custom_fread_t )ph_regObject->customRead;
        fd->pfWrite  = (custom_fwrite_t)ph_regObject->customWrite;
        fd->pfSeek   = (custom_fseek_t)ph_regObject->customSeek;
        fd->pfTimeGet = (custom_ftimeget_t)ph_regObject->customTimeGet;
        fd->pfClose  = (custom_fclose_t)ph_regObject->customClose;
        fd->pfSize   = (custom_fsize_t)ph_regObject->customSize;
        }
    return OK;
}


/*-----------------------------------------------------------------------*/

/*

>>>******************************************************************
*
* REGISTRY_ModifyTemplate - 
*
* NOMANUAL
*
*<<<


>>> a template for creating additional write registry methods <<<

void REGISTRY_ModifyTemplate()
{
    REGISTRY_Lock();

    while (mNumReaders != 0)
    {
        REGISTRY_Unlock();

        OS_SPECIFIC_YIELD();

        REGISTRY_Lock();
    }

    >>> modify the registry here <<<

    REGISTRY_Unlock();
}

*/


/***************************************************************
*
* httpFileAccessSet - 
*
* NOMANUAL
*
*/

extern STATUS httpFileAccessSet
    (
    char    *fileName,    /* name of file */
    Access  readAccess,   /* read access */
    Access  writeAccess   /* write access */
    )
{
    RegDes * pRegistryEntry = NULL;
    if ( NULL != ( pRegistryEntry = httpFileRegHandle ( fileName )))
        {
        pRegistryEntry->readAccess  = readAccess;
        pRegistryEntry->writeAccess = writeAccess;
        return OK;
        }
    else
        return ERROR_GENERAL;
}

/***************************************************************
*
* httpFileAccessGet - 
*
* NOMANUAL
*
*/

extern STATUS httpFileAccessGet
    (
    char    *fileName,     /* name of file */
    Access  *readAccess,   /* pointer to get read access */
    Access  *writeAccess   /* pointer to get write access */
    )
{
    RegDes * pRegistryEntry = NULL;
    if ( NULL != ( pRegistryEntry = httpFileRegHandle ( fileName )))
        {
        *readAccess  = pRegistryEntry->readAccess;
        *writeAccess = pRegistryEntry->writeAccess;
        return OK;
        }
    else
        return ERROR_GENERAL;
}


/*******************************************************************************
*
* filesShow - Show the list of registered files.
*
* NOMANUAL
*
* RETURNS: OK
*	ERROR_GENERAL_NOT_FOUND if the registry entry is not found.
*
*/

void filesShow(void)
	{
	HTTP_GEN_LIST_ENTRY_ID entryId = NULL;
	int i = 0;

	while (httpGenListIterator(mplst_Registry, &entryId) != NULL)
		{
		printf ("File (%3d): '%s'\n", i++, httpGenListIdGet(entryId));
		}
	}

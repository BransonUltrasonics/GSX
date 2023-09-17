/* httpFileMgr.c - file manager for the HTTP server */

/*
 * Copyright (c) 2003-2008,2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
02jul14,r_w  support Etag on NVM file system US32703
05may14,r_w  fix static analysis issues(US35919)
08apr13,r_w  fix static analysis issues for defect WIND00411995
16oct12,f_f  validate the pointer before calling the custom handler pfGets
             WIND00377389
11oct12,r_w  add fseek for CUSTOM_FS in httpFileSeek. defect WIND00376252
             add pfTimeGet for CUSTOM_FS in httpFileBiTimeGet. defect WIND00376276
10sep12,r_w  fix file system search order issue
             defect WIND00374786
07jun11,m_z  add get binary file m_time function WIND00203773
             CQID: WIND00203773
2feb10,h_y   support for LP64 
29aug07,ps   add CMP_MEM_FS case to httpFileTimeGet()
22feb07,ps   apigen reformatting
13feb07,msa  Fix comments to remove APIgen errors.
25oct04,gsr  enhanced httpFileOpen to meet file uploads SPR 102705
15jul04,jws  fix SPR 97985
26apr04,vmk  add httpFileTimeGet functionality
16jan04,jws  add references to external func ptrs
14Jan04,gsr  cleanup routine for SPR 93009
15dec03,adb  merged in SPR 90310 fix
*/

/*
DESCRIPTION
This module provides a unified file access API that can read ANSI files
as well as non volatile files generated with Rombuild.  It can be easily
expanded to support custom file systems.


INCLUDE FILES:
    httpLib.h
    httpNvFileMgr.h

*/


#include <string.h>
#include "time.h"
#include "httpLib.h"
#include "httpNvFileMgr.h"

static enum fs_e defaultFileSystem = CMP_ANSI_FS;
static time_t  gst_webFsStartTime = 0;

static ubyte4 ansiFileSize(FILE * f);
static ssize_t gzipFileSize(gzFile f);
static BOOL nextReadFs(generic_fd_t * f);
static BOOL nextWriteFs(generic_fd_t * f);

/***************************************************************************
*
* httpFileMngrInit - Initialize the server file manager.
*
*
* RETURNS: OK always
*
*/
STATUS httpFileMngrInit (void)
    {
    
     if (pFILEMGR_Construct)  pFILEMGR_Construct();
    
     (void)time(&gst_webFsStartTime);

     return OK;
    }

/***************************************************************************
*
* httpFileMngrCleanup - Cleanup the server file manager.
*
*
* RETURNS: OK or ERROR
*
*/
STATUS httpFileMngrCleanup (void)
    {

    if (pFILEMGR_DeConstruct)  pFILEMGR_DeConstruct();

    return OK;
    }

/***************************************************************************
*
* httpFileRead - Read a file in the file system--compressed or not.
*
* This routine reads a file in the file system. Its behavior should
* be exactly as defined by the ANSI C library function read(), except
* for the double indirection of the read buffer.
*
* RETURNS: The number of bytes read--between 1 and <maxbytes>, or
* 0 if end of  file--or HTTP_ERROR if the file descriptor does not exist.
*/
STATUS httpFileRead
    (
    generic_fd_t * f,
    sbyte *        cpBuffer,
    ubyte4         lMaxBytes,
    ubyte4 *       pBytesRead
    )
    {
    /*void * retValVoid;*/
    ubyte4 avBytes;
    switch (f->fs)
        {
        case CUSTOM_FS:
            (*pBytesRead) = f->pfRead ((sbyte *) cpBuffer, 1,
                                       lMaxBytes, f->fd.customFd);
            break;
        case ANSI_FS:
            (*pBytesRead) = (ubyte4)fread (cpBuffer, 1, lMaxBytes, f->fd.ansiFd);
            break;
        case CMP_ANSI_FS:
            (*pBytesRead) = (ubyte4)gzread (f->fd.cmpAnsiFd, cpBuffer, lMaxBytes);
            break;
        case MEM_FS:
            /* prevent overruns since this is a raw memcpy... */
             avBytes = (f->size < f->offset) ? 0 : f->size - f->offset;
            (*pBytesRead) = (avBytes < lMaxBytes) ? avBytes : lMaxBytes;
            memcpy(cpBuffer,
                   (sbyte *) f->fd.memFd + f->offset, *pBytesRead);
            f->offset += (*pBytesRead);
            break;
        case CMP_MEM_FS:
            if (pMemGzRead)
                {
                *pBytesRead = lMaxBytes;
                pMemGzRead (f->fd.cmpMemFd, cpBuffer, pBytesRead);
                }
            else
                *pBytesRead = 0;

            break;
        default:
            return wmError(ERROR_GENERAL_ILLEGAL_VALUE,"invalid file type");
        }
    return OK;
    }

/***************************************************************************
*
* httpFileWrite - Write to a file in the file system.
*
* This routine writes to a file (ONLY ANSI files supported for writing). Its
* behavior should be exactly as defined by the ANSI C library function write().
*
*
* RETURNS: OK or ERROR
*/
STATUS httpFileWrite
    (
    generic_fd_t * f,
    const sbyte *   cpBuffer,
    sbyte4          lNbytes,
    ubyte4 *        bytesWritten
    )
    {
    switch (f->fs)
        {
        case CUSTOM_FS:
            (*bytesWritten) = (ubyte4)f->pfWrite ((sbyte *) cpBuffer, 1,
                                          (ubyte4)lNbytes, f->fd.customFd);
            break;
        case ANSI_FS:
            (*bytesWritten) = (ubyte4)fwrite((sbyte *) cpBuffer, 1,
                                     (ubyte4)lNbytes, f->fd.ansiFd);
            break;
        case CMP_ANSI_FS:
            (*bytesWritten) = (ubyte4)gzwrite (f->fd.cmpAnsiFd,
                                       (sbyte *) cpBuffer, (ubyte4)lNbytes);
            break;
        default:
            return wmError(ERROR_GENERAL_ILLEGAL_VALUE,"invalid file type");
        }
    return OK;
    }

/***************************************************************************
*
* httpFileOpen - Open a distinct file in the file system.
*
* This routine opens a file for reading, writing, or updating,
* and  returns a file descriptor for that file.  The arguments
* to open() are the filename and the type of access (same as
* for ANSI fopen, that is "r", "w", "a", "rb", "wb", "ab",
* "r+", "w+", "a+", "rb+"/"r+b", "wb+"/"w+b" or "ab+"/"a+b".
*
* Write accesses are not supported on the custom or server-memory
* file systems.
*
* The function will attempt to open the file in the following
* file systems:
*
*   1. If the file is in the registry, use the fs specified there.
*   2. If the file is not found, look in the defaultFileSystem.
*   3. If the file is still not found, look in the nextFs().
*   4. Give up after looking in MEM_FS.
*
* RETURNS: OK or ERROR
*
*/
STATUS httpFileOpen
    (
    sbyte *        szName,     /* name of the file to open             */
    sbyte *        mode,
    generic_fd_t * f
    )
    {
    sbyte cmpMode[5];
    RegDes * pRegistryEntry;
    
    BOOL bAreReads;
    BOOL bAreWrites;
    
    BOOL isRegistry = FALSE;
    BOOL isRetry = FALSE;

    bAreReads  =   (NULL != strchr(mode,'r'));
    bAreWrites =  ( (NULL != strchr(mode,'w')) || (NULL != strchr(mode,'a')) );

    if (!bAreReads && !bAreWrites)
	return wmError(ERROR_GENERAL, "invalid parameter");

    f->fd.anyFd = NULL;
    f->offset = 0;
    f->fs = defaultFileSystem;
    f->mimeType = NULL;

    /* check if file is in the registry */
    if (NULL != (pRegistryEntry = httpFileRegHandle (szName)))
        {
	/* fill out the file system field (f->fs), and for CUSTOM_FS, the
	   file access functions (f->pfWhatever.customWhatever)    */
        (void) httpFileRegGetFd(pRegistryEntry, f);
        isRegistry = TRUE;
        }

    /* fsys = IsFileInRegistry(szName); */

    do 
    {
    switch (f->fs)
	{
	   case CUSTOM_FS:
	    if (NULL != pRegistryEntry)
	        f->fd.customFd = f->pfOpen(szName, mode);
	   break;
       case ANSI_FS:
	       f->fd.ansiFd = fopen (szName, mode);
	   break;
       case CMP_ANSI_FS:
		   strncpy(cmpMode, mode, 4);
		   cmpMode[4] = '\0';
	       strcat(cmpMode, "9");       /* maximum compression */
	       f->fd.cmpAnsiFd = gzopen (szName, cmpMode);
	   break;
       case MEM_FS:
           if (bAreWrites) /* this is a READ_ONLY file system */
               break;                

           f->fd.memFd = NULL;
           if (pFILEMGR_RetrieveFile)
               f->fd.memFd = pFILEMGR_RetrieveFile((int *)(&f->size), szName);
           else
	       f->fd.memFd = NULL;
	   break;
       case CMP_MEM_FS:
           if (bAreWrites) /* this is a READ_ONLY file system */
               break;

           if (pMemGzOpen)
               pMemGzOpen(szName, &f->fd.cmpMemFd, &f->size);
           else
               f->fd.cmpMemFd = NULL;
	    break;
        default:
           return wmError(ERROR_GENERAL,"invalid filesystem");
       }
    
    if(isRegistry)
        {
    	if(NULL == f->fd.anyFd)
    	    {
    		isRetry = TRUE;
    	   	f->fs = defaultFileSystem;
    	   	isRegistry = FALSE;
    	    }
        }
    else
        {
    	isRetry = (NULL == f->fd.anyFd) && ((bAreWrites) ? nextWriteFs(f) : nextReadFs(f)); 
        }
    }    while (isRetry);
    if (NULL == f->fd.anyFd) return wmError(ERROR_GENERAL,"file not found");
    return OK;
    }

/***************************************************************************
*
* httpFileClose - Close a file in the file system.
*
* This routine closes a file in the file system.
*
* RETURNS: OK if file could be closed successfully, or
*          an error if the file descriptor is invalid.
*/
STATUS  httpFileClose
    (
    generic_fd_t * f
    )
    {
    switch (f->fs)
        {
        case CUSTOM_FS:
            return f->pfClose(f->fd.customFd);
        case MEM_FS:
            break;
        case CMP_MEM_FS:
            if (pMemGzClose)
                pMemGzClose(f->fd.cmpMemFd);
            break;
        case CMP_ANSI_FS:
            (void) gzclose(f->fd.cmpAnsiFd);
            break;
        case ANSI_FS:
            (void) fclose(f->fd.ansiFd);
            break;
        default:
            return wmError(ERROR_GENERAL,"invalid filesystem");
        }
    return OK;
    }

/***************************************************************************
*
* httpFileGets - Read from a file until a newline is seen.
*
* RETURNS: The number of bytes written (if not equal to <nbytes>,
* an error has occurred), HTTP_ERROR if the file descriptor does not exist.
*/
STATUS httpFileGets
    (
    generic_fd_t * f,
    sbyte *        cpBuffer,
    sbyte4         lNbytes
    )
    {
    switch (f->fs)
        {
        case CUSTOM_FS: /* Reserve it for expansion */
            if (NULL != f->pfGets)
                f->pfGets ((sbyte *) cpBuffer, (ubyte4)lNbytes, f->fd.customFd);
            break;
        case ANSI_FS:
            fgets ((sbyte *) cpBuffer, lNbytes, f->fd.ansiFd);
            break;
        case CMP_ANSI_FS:
            gzgets(f->fd.cmpAnsiFd, (sbyte*) cpBuffer, lNbytes);
            break;
        case CMP_MEM_FS:
        case MEM_FS:
            return wmError(ERROR_GENERAL,
                             "httpFileGets not implemented for this filesystem");
        default:
            return wmError(ERROR_GENERAL_ILLEGAL_VALUE,"invalid file type");
        }
    return OK;
    }

/***************************************************************************
*
* httpFileSize - Return the size of a file.
*
* RETURNS: the size of a file in bytes.  For compressed files, it
* returns the size of the uncompressed file.
*
* NOTE:  A file must be open to get its size.
*/
ssize_t httpFileSize
    (
    generic_fd_t * f
    )
    {
    if (NULL == f) return 0;
    switch (f->fs)
        {
        case CUSTOM_FS:
            return (ssize_t)(f->pfSize(f->fd.customFd));
        case ANSI_FS:
            return (ssize_t)(ansiFileSize (f->fd.ansiFd));
            /*break;*/
        case CMP_ANSI_FS:
            return (ssize_t)(gzipFileSize (f->fd.cmpAnsiFd));
           /* break;*/
        case MEM_FS:
            return (ssize_t)(f->size);
        case CMP_MEM_FS:
            return (ssize_t)(f->size); /* uncompressed size */
        default:
            return 0;
        }
    }

/***************************************************************************
*
* httpFileDelete - Delete a file from the selected file system.
*
* This routine deletes a file in the file system.
*
* If the file system is ALL_FS, the function will attempt to remove
* the file from all the file systems that support deletion.
*
* RETURNS: OK if file was deleted, ERROR otherwise
*/
STATUS httpFileDelete
    (
    sbyte * szName,
    enum fs_e fs
    )
    {
    sbyte4 notRemoved;
    switch (fs)
        {
        case MEM_FS:            /* fall through */
        case CMP_MEM_FS:
            return OK;      /* not supported for ***MEM_FS */
        case ALL_FS:
        case CMP_ANSI_FS:       /* fall through */
        case ANSI_FS:
            notRemoved = remove(szName);
            if (notRemoved)
                return wmError(ERROR_GENERAL_FILE_NOT_FOUND, "file not found");
            return OK;
        default:
            return wmError(ERROR_GENERAL,"invalid filesystem");
        }
    }

/***************************************************************************
*
* httpFileSeek - Set an offset for the next read or write operation.
*
* The new position is calculated from the origin of the file.
*
* RETURNS: OK or ERROR
*/
STATUS httpFileSeek
    (
    generic_fd_t * f,
    ubyte4 offset
    )
    {
    switch (f->fs)
        {
        case CUSTOM_FS:
            f->pfSeek(f->fd.customFd, offset, SEEK_SET);
            return OK;
        case MEM_FS:
            f->offset = offset;
            return OK;
        case CMP_MEM_FS:
            if (pMemGzSeek)
                pMemGzSeek(f->fd.cmpMemFd, offset);
            return OK;
        case CMP_ANSI_FS:
            (void) gzseek(f->fd.cmpAnsiFd, offset, SEEK_SET);
            return OK;
        case ANSI_FS:
            (void)fseek(f->fd.ansiFd, offset, SEEK_SET);
            return OK;
        default:
            return wmError(ERROR_GENERAL,"invalid filesystem");
        }
    }

/***************************************************************************
*
* httpFileContentTypeGet - Read the content type for the file.
*
* This routine returns the content type of the file if it was provided by the
* user.  Normally the file Content-Type will be guessed by the server based on
* the extension of the requested file.  But if the file was registered using
* a different Content-Type, this function will return it.
*
* RETURNS: A pointer to the Content-Type string or NULL.
*/
sbyte * httpFileContentTypeGet
    (
    generic_fd_t * f
    )
    {
    if (NULL != f) return f->mimeType;
    return NULL;
    }

/***************************************************************************
*
* httpFileSetDefaultFs - Sets the file system to be accessed first.
*
* This function will set the default file system that will be used in subsequent
* file operations.
* In order to choose the appropriate input parameter, you should first answer the
* following questions about the desired default file manager behavior:
*
* 1. Where should the file manager look for files?
*    a) First in the ANSI file system, and if the file is not found, fall back
*       to the non-volatile memory file system.
*    b) Go directly to the non-volatile file system.
*
* 2. Should the file manager uncompress gzipped files?
*    c) Yes
*    d) No
*
* Based on your answers, you should set defaultFs to
* ac: CMP_ANSI_FS  ad: ANSI_FS  bc: CMP_MEM_FS bd: MEM_FS
*
* If a file is registered using REGISTRY_FileRegister, the default behavior
* is ignored.
*
* RETURNS: OK or ERROR
*/
void httpFileSetDefaultFs
    (
    enum fs_e defaultFs
    )
    {
    defaultFileSystem = defaultFs;
    }

/***************************************************************************
*
* httpFileGetDefaultFs -  Gets the initialized default file system.
*
* This function will get the default file system that will be used in subsequent
* file operations.
*
* SEE ALSO: httpFileSetDefaultFs( )
*
* RETURNS: Enumeration value for the default file system
*/
enum fs_e  httpFileGetDefaultFs(void)
    {
    return defaultFileSystem;
    }

/***************************************************************************
*
* nextReadFs - Selects the next file system to attempt to open a file from 
* for read.
*
* see httpFileSetDefaultFs for details.
*
* CMP_ANSI_FS -> CMP_MEM_FS \
*                            >  MEM_FS
*                   ANSI_FS /
*
* NOMANUAL
*
* RETURNS: TRUE or FALSE
*/
static BOOL nextReadFs
    (
    generic_fd_t * f
    )
    {
    switch (f->fs)
        {
        case CUSTOM_FS:   (f->fs) = defaultFileSystem; break;
        case CMP_ANSI_FS: (f->fs) = CMP_MEM_FS;        break;
        case ANSI_FS:     (f->fs) = MEM_FS;            break;
        case CMP_MEM_FS:  (f->fs) = MEM_FS;            break;
        case MEM_FS:      return FALSE;
        default:
            wmError(ERROR_GENERAL_ILLEGAL_VALUE, "getNextFs error");
        }
    return TRUE;
    }

/***************************************************************************
*
* nextWriteFs - Selects the next upload file system to open a file from
* for write.
*
* see httpFileSetDefaultFs for details.
*
* CMP_MEM_FS \
*             > -> CUSTOM_FS  \
*     MEM_FS /                 > ANSI_FS
*                CMP_ANSI_FS  /
*
* NOMANUAL
*
* RETURNS: TRUE or FALSE
*/
static BOOL nextWriteFs
    (
    generic_fd_t * f
    )
    {
    switch (f->fs)
        {
        case CMP_MEM_FS:  (f->fs) = CUSTOM_FS; break;
        case MEM_FS:      (f->fs) = CUSTOM_FS; break;
        case CUSTOM_FS:   (f->fs) = ANSI_FS;   break;
        case CMP_ANSI_FS: (f->fs) = ANSI_FS;   break;
        case ANSI_FS:     return FALSE;       /* break;*/
        default:
            wmError(ERROR_GENERAL_ILLEGAL_VALUE, "getNextFs error");
        }
    return TRUE;
    }


/*******************************************************************************
*
* httpFileTimeGet - Get modification time of a file.
*
* Retrieve modification time for the file. The buffer to store the time
* information, has to be provided.It has to be at least 32 characters long.
*
* An ERROR return is not expected to be fatal because some file systems do not
* have time information.  In general, <errno> will not contain useful information
* after an ERROR return.
*
* RETURNS: OK if time info was obtained, or
*          ERROR if the file descriptor is invalid or time not available.
*/

STATUS  httpFileTimeGet
    (
    generic_fd_t * f,
    sbyte *        szName,     /* file name to collect file status  */
    sbyte *        szDateBuffer
    )
    {
    struct stat     stBuffer;
    STATUS          rc;
	time_t          st_mtime = 0;

	/* init stat buffer */

    bzero( (char *) &stBuffer, sizeof(stBuffer));

    switch (f->fs)
        {
        case CUSTOM_FS:

            rc = f->pfTimeGet(f, szName, &st_mtime);
            if ((OK == rc) && (0 != st_mtime))
                {

                /* Convert to string and get rid of newline. */
                ctime_r (&st_mtime, szDateBuffer);

                szDateBuffer [strlen(szDateBuffer) - 1] = '\0';
                }							
            break;

	case CMP_MEM_FS:
	case MEM_FS:

		    /* This will change if time info gets added to MEM_FS. */
		    ctime_r (&gst_webFsStartTime, szDateBuffer);
        
            szDateBuffer [strlen(szDateBuffer) - 1] = '\0';        		   
        
            rc = OK;
		    
		    break;

        case ANSI_FS:

            rc = stat (szName, &stBuffer);
            if ((OK == rc) && (0 != stBuffer.st_mtime))
                {

                /* Convert to string and get rid of newline. */

                ctime_r (&stBuffer.st_mtime, szDateBuffer);

                szDateBuffer [strlen(szDateBuffer) - 1] = '\0';
                }
            break;

        default:

            rc = wmError(ERROR_GENERAL,"Filesystem not supported");
            break;
  	     };

    return rc;
    }

/*******************************************************************************
*
* httpFileBiTimeGet - Get file time in binary mode.
*
* Retrieve modification time for the file in binary mode 
*
* An ERROR return is not expected to be fatal because some file systems do not
* have time information.  In general, <errno> will not contain useful information
* after an ERROR return.
*
* RETURNS: OK if time info was obtained, or
*          ERROR if the file descriptor is invalid or time not available.
*/

STATUS  httpFileBiTimeGet
    (
    generic_fd_t * f,
    sbyte *        szName,     /* file name to collect file status  */
    unsigned long * mtime
    )
    {
    struct stat     stBuffer;
    STATUS          rc;
	time_t          st_mtime = 0;

	/* init stat buffer */

    bzero( (char *) &stBuffer, sizeof(stBuffer));

    switch (f->fs)
        {
        case ANSI_FS:

            rc = stat (szName, &stBuffer);
            if (OK == rc)
                {
                *mtime = (unsigned long)stBuffer.st_mtime;
                }
            break;

        case CUSTOM_FS:
            rc = f->pfTimeGet(f, szName, &st_mtime);
            if (OK == rc)
                {
                *mtime = (unsigned long)st_mtime;
                }
            break;
			
	    case CMP_MEM_FS:
	    case MEM_FS:
	    	
	    	   *mtime = (unsigned long)gst_webFsStartTime;
	    	   rc = OK;
	    	   
	    	break;
	    	
        default:

            rc = wmError(ERROR_GENERAL,"Filesystem not supported");
            break;
  	     };

    return rc;
    }


/* kludging functions */
/* the following functions are not part of the specific file system, but
   they are needed to provide the functionality that filemngr requires.
 */

/* ansiFileSize */

/**********************************************************************
*
* ansiFileSize - 
*
* NOMANUAL
*/

static ubyte4 ansiFileSize
    (
    FILE * f
    )
    {
    ubyte4 fileSize;

    (void)fseek(f,0L,SEEK_END);
    if (-1 == (sbyte4)(fileSize = (ubyte4)ftell(f))) return 0;
    (void)fseek(f, 0L, SEEK_SET);
    return fileSize;
    }

/* gzipFileSize */

/**********************************************************************
*
* gzipFileSize - 
*
* NOMANUAL
*/

static ssize_t gzipFileSize
    (
    gzFile file
    )
    {
    ssize_t fileSize;
    ubyte  aByte;
    while (gzread(file, &aByte, 1));
    fileSize = (ssize_t)gztell(file);
    (void) gzseek(file, 0L, SEEK_SET);
    return fileSize;
    }


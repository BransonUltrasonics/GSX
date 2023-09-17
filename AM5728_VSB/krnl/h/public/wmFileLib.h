/* wmFileLib.h - ansi file system for threads */

/* Copyright 2004-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01c,30aug07,msa  Update DIRECTORY_BUFFER_SIZE_K to use kDIRECTORY_BUFFER_SIZE
                 or 256
01b,07mar07,ps  fixes for RTP builds. defect WIND00044864
01a,20feb04,jws  added copyright and mod history
*/


#ifndef __INCwmFileLibh
#define __INCwmFileLibh

#include <stdio.h>

#ifdef __WIN32_OS__
#include <direct.h>
#include <io.h>
#define CLOSEDIR    _findclose

#else /* __WIN32_OS__ */

#ifdef __BSD_OS__
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif /* __POSIX_OS__ */

#if defined(__POSIX_OS__) || defined(__SOLARIS_OS__)

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif /* __POSIX_OS__ */

#if defined(__VXWORKS_OS__) || defined(__VXWORKS_AE_OS__)
#include <dirent.h>
#include <sys/stat.h>
#endif /* __VXWORKS_OS__ */


#endif /* ! __WIN32_OS__ */

#ifndef CLOSEDIR
#define CLOSEDIR    closedir
#endif

#define wmDir(x)    ((DIR_HANDLE *) x)

#define wmDirBad(x) (NULL == x)


/* backwards compatability */

#define kFileSeparator  PATH_CHAR_K

/* file attribute flags -- expand */

#define FS_DIRECTORY_K  0x0001
#define FS_READ_ONLY_K  0x0002


#ifndef DIRECTORY_BUFFER_SIZE_K
#ifdef kDIRECTORY_BUFFER_SIZE
#define DIRECTORY_BUFFER_SIZE_K     kDIRECTORY_BUFFER_SIZE
#else
#define DIRECTORY_BUFFER_SIZE_K     256
#endif
#endif


#ifdef __WIN32_OS__

typedef struct WM_DIR_T
    {
    BOOL    found;
    long    handle;
    struct  _finddata_t fileInfo;
    sbyte   cwd[DIRECTORY_BUFFER_SIZE_K];
    }
    WM_DIR_T;

#else

typedef struct WM_DIR_T
    {
    DIR *    handle;
    sbyte4   nameLen;
    sbyte    fileMask[64];
    sbyte    fileDir[64];
    sbyte    cwd[DIRECTORY_BUFFER_SIZE_K];
    }
    WM_DIR_T;

#endif /* __WIN32_OS__ */

#ifdef __cplusplus
extern "C" {
#endif

STATUS   wmFileInit             (sbyte * pPath);
#ifndef _WRS_CONFIG_LP64
void *   wmFileReadFile         (WM_DIR_T * pDir, sbyte4 * pLen, sbyte * pId);
#else
void *   wmFileReadFile         (WM_DIR_T * pDir, sbyte8 * pLen, sbyte * pId);
#endif
#ifndef _WRS_CONFIG_LP64
void *   wmFileRetrieveFile     (sbyte4 * pLen, sbyte * pId);
#else
void *   wmFileRetrieveFile     (sbyte8 * pLen, sbyte * pId);
#endif
void     wmFileReleaseFile      (void * pObject);
#ifndef _WRS_CONFIG_LP64
void *   wmFileRetrieveResource (sbyte4 * pLen, sbyte * pId);
#else
void *   wmFileRetrieveResource (sbyte8 * pLen, sbyte * pId);
#endif
void     wmFileReleaseResource  (void * pObject);

STATUS   wmFileChdir    (WM_DIR_T * pDir, sbyte * pPath);
STATUS   wmFileOpenDir  (WM_DIR_T * pDir, sbyte * pFile);
#ifndef _WRS_CONFIG_LP64
sbyte*   wmFileNextDir  (WM_DIR_T * pDir, sbyte4 * pSize, ubyte4 * pFlags);
#else
sbyte*   wmFileNextDir  (WM_DIR_T * pDir, sbyte8 * pSize, ubyte4 * pFlags);
#endif
void     wmFileCloseDir (WM_DIR_T * pDir);

#ifdef __cplusplus
}
#endif

#endif /* ! __INCwmFileLibh               */


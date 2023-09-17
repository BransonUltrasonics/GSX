/* httpNvFileMgr.c - file manager for filesystems generated with rombuild */

/* Copyright 2002-2005,2010,2014 Wind River Systems, Inc. */

/*
modification history
-------------------- 
26sep14,r_w  fix static CHECKED_RETURN issue.
02feb10,h_y   support for LP64 
11oct05,msa  add file version 3 with 256 byte filenames. abstract file access
07jun05,dlr  fetch integers with scanLong(), to avoid alignment dependancies
16jan04,jws  remove dependency on __NO_ROMBUILD_IMAGE__
07oct03,jws  remove include of zlib.h
17jan01,jc   created
*/

/*
DESCRIPTION
This module provides the functions needed to access non-volatile filesystems
generated with rombuild.  This module can only access non-compressed files
in the non-volatile image.  In order to access compressed files, use
memgzio.c.

NOMANUAL
*/

#include <string.h>
#include "wm.h"
#include "httpLib.h"

extern void FILEMGR_ReleaseFile(void *pFile);
extern void FILEMGR_ReleaseResource(void *pFile);
extern void FILEMGR_Construct(void);
extern void FILEMGR_DeConstruct(void);
extern void *FILEMGR_RetrieveFile(int *pFileLen, sbyte *fileName);
extern void *FILEMGR_RetrieveResource(int *pFileLen, sbyte *fileName);
int  httpDoNvmShow(int nPartNumber, sbyte *mFileMgrDirectory);
extern int httpNvmShow(void);

/*-----------------------------------------------------------------------*/

/* #ifndef __NO_ROMBUILD_IMAGE__ */

#if 1

#define __RLI_PARTITION_FULL__
#ifdef __RLI_PARTITION_FULL__

/* Setup tables for NEW multiple file arrays */
sbyte4 nPartitionCount;
extern sbyte *rc_filearray[];

#else /* !__RLI_PARTITION_FULL__ */

/* Setup tables for multiple file arrays if any */
#ifndef __RLI_FILEMGR_PARTITION_COUNT__
#define nPartitionCount  1
extern char rc_filesys[];
static char *rc_filearray[] =
{   kNVRamStart     /* this is usually set to rc_filesys in options.h */
};

#else /* nPartitionCount */

/* Setup the search table that will automatically link to the requested file arrays */
#define nPartitionCount  __RLI_FILEMGR_PARTITION_COUNT__
extern char rc_filesys1[];
#if (__RLI_FILEMGR_PARTITION_COUNT__ > 1)
extern char rc_filesys2[];
#else
#define     rc_filesys2 NULL
#endif
#if (__RLI_FILEMGR_PARTITION_COUNT__ > 2)
extern char rc_filesys3[];
#else
#define     rc_filesys3 NULL
#endif
#if( __RLI_FILEMGR_PARTITION_COUNT__ > 3)
extern char rc_filesys4[];
#else
#define     rc_filesys4 NULL
#endif
#if( __RLI_FILEMGR_PARTITION_COUNT__ > 4)
extern char rc_filesys5[];
#else
#define     rc_filesys5 NULL
#endif
#if( __RLI_FILEMGR_PARTITION_COUNT__ > 5)
extern char rc_filesys6[];
#else
#define     rc_filesys6 NULL
#endif
#if( __RLI_FILEMGR_PARTITION_COUNT__ > 6)
extern char rc_filesys7[];
#else
#define     rc_filesys7 NULL
#endif
#if( __RLI_FILEMGR_PARTITION_COUNT__ > 7)
extern char rc_filesys8[];
#else
#define     rc_filesys8 NULL
#endif
static sbyte *rc_filearray[] =
{   rc_filesys1,
    rc_filesys2,
    rc_filesys3,
    rc_filesys4,
    rc_filesys5,
    rc_filesys6,
    rc_filesys7,
    rc_filesys8,
    NULL
};
#endif  /* nPartitionCount */

#endif /* __RLI_PARTITION_FULL__ */

/*-----------------------------------------------------------------------*/

/* If the File System is compiled as a C source file, then the File Manager
 * and the compiler need to know where to look for it. */
#ifdef __RLI_PARTITION_FULL__
static  sbyte **mFlashMemoryStartArray = NULL;
static  sbyte **mFileMgrDirectoryArray = NULL;
static  ubyte4 *mFileMgrSizeArray      = NULL;
#else
static  sbyte *mFlashMemoryStartArray[nPartitionCount];
static  sbyte *mFileMgrDirectoryArray[nPartitionCount];
static  ubyte4 mFileMgrSizeArray[     nPartitionCount];
#endif

    /* Note: This is not re-entrant... All file system versions must currently
       be of the same type. */
#define kFileSystemVersionBad       -1
#define kFileSystemVersion1         1
#define kFileSystemVersion2         2
#define kFileSystemVersion3         3

#define kUniqueSignatureLength      16
#define kVer1And2MaxFileNameLen     64
#define kVer3MaxFileNameLen         256

    /* Placeholder for the pointer to the real Ver_n_DirEntry structure. */
#define DIRECTORY_ENTRY             void

/* Note: these integers are in network order for version 2 */
typedef struct  Ver1And2DirEntry        /* (basename:  dir) */
{
    sbyte   fileName[kVer1And2MaxFileNameLen];
    sbyte4  fileSize;
    sbyte4  fileOffset;                 /* from the base addr of the file system */
    sbyte4  fileCheckSum;
} Ver1And2DirEntry;

/* Note: these integers are in network order */
typedef struct  Ver3DirEntry            /* (basename:  dir) */
{
    sbyte   fileName[kVer3MaxFileNameLen];
    sbyte4  fileSize;
    sbyte4  fileOffset;                 /* from the base addr of the file system */
    sbyte4  fileCheckSum;
} Ver3DirEntry;

/* This routine will translate integers from network-ordered byte array */
/* This explicitly fetches its individual bytes to avoid a long's alignment restrictions */
static sbyte4 scanLong(void *bufPtr)
{
	char  *sPtr;
	sbyte4 nLong;
	int   i;

	sPtr = (char *) bufPtr;

	/* convert network order (HighEndian) long to host long */
	for (i=0,nLong=0; (i<4) ; i++)
		{
		nLong = (nLong << 8) + (((int) sPtr[i]) & 0x0ff);
		}

	return (nLong);
}

/*-----------------------------------------------------------------------*/

/* The DIR_* and DIRENT_* routines handle abstraction of the directory
   structure access. */
static sbyte4 DIR_fileSystemVersionNumGet (sbyte *pFileMgrDir)
{
    sbyte4 *        pFileDirectory = NULL;
    static sbyte4   fileSystemVersionNum = kFileSystemVersionBad;

        /* We know the version number must be the same for all partitions... */
    if ((pFileMgrDir == NULL)
            || (fileSystemVersionNum != kFileSystemVersionBad))

        return (fileSystemVersionNum);

    pFileDirectory = (sbyte4 *)(&(pFileMgrDir [2 * kUniqueSignatureLength]));

     	/* Get the version number: we do not know endian yet */
	fileSystemVersionNum = scanLong(&pFileDirectory[0]);

	if ((1 == (fileSystemVersionNum & 0x0ff))
            || (1 == ((fileSystemVersionNum >> 24) & 0x0ff)))

		fileSystemVersionNum = kFileSystemVersion1;

	if ((2 == (fileSystemVersionNum & 0x0ff))
            || (2 == ((fileSystemVersionNum >> 24) & 0x0ff)))

		fileSystemVersionNum = kFileSystemVersion2;

	if ((3 == (fileSystemVersionNum & 0x0ff))
            || (3 == ((fileSystemVersionNum >> 24) & 0x0ff)))

		fileSystemVersionNum = kFileSystemVersion3;

    return (fileSystemVersionNum);
}

/*-----------------------------------------------------------------------*/

static sbyte4 DIR_numFilesGet (sbyte *pFileMgrDir)
{
    sbyte4 *    pFileDirectory = NULL;

    if (pFileMgrDir == NULL)
        return (0);

    pFileDirectory = (sbyte4 *)(&(pFileMgrDir [2 * kUniqueSignatureLength]));

    return (scanLong (&pFileDirectory [1]));
}

/*-----------------------------------------------------------------------*/

static DIRECTORY_ENTRY *DIR_dirEntryGet (sbyte *pFileMgrDir, int index)
{
    sbyte4 *            pFileDirectory = NULL;
    Ver1And2DirEntry *  pV1Or2DirDesc = NULL;
    Ver3DirEntry *      pV3DirDesc = NULL;

    if (pFileMgrDir == NULL)
        return (NULL);

    pFileDirectory = (sbyte4 *)(&(pFileMgrDir [2 * kUniqueSignatureLength]));

    switch (DIR_fileSystemVersionNumGet (pFileMgrDir))
    {
        case kFileSystemVersion1:
        case kFileSystemVersion2:
            pV1Or2DirDesc = (Ver1And2DirEntry *)(&pFileDirectory [2]);
            return ((DIRECTORY_ENTRY *)&(pV1Or2DirDesc [index]));
            /*break;*/

        case kFileSystemVersion3:
            pV3DirDesc = (Ver3DirEntry *)(&pFileDirectory [2]);
            return ((DIRECTORY_ENTRY *)&(pV3DirDesc [index]));
           /* break;*/

        default:
            return (NULL);
           /* break;*/
    }
}

/*-----------------------------------------------------------------------*/

static sbyte4 DIRENT_fileOffsetGet (DIRECTORY_ENTRY *pDirEntry)
{
    Ver1And2DirEntry *  pV1Or2DirDesc = (Ver1And2DirEntry *)pDirEntry;
    Ver3DirEntry *      pV3DirDesc = (Ver3DirEntry *)pDirEntry;

    if (NULL == pDirEntry)
        return (0);

    switch (DIR_fileSystemVersionNumGet (NULL))
    {
        case kFileSystemVersion1:
        case kFileSystemVersion2:
            return (scanLong (&(pV1Or2DirDesc->fileOffset)));
            /*break;*/

        case kFileSystemVersion3:
            return (scanLong (&(pV3DirDesc->fileOffset)));
            /*break;*/

        default:
            return (0);
           /* break;*/
    }
}

/*-----------------------------------------------------------------------*/

static sbyte4 DIRENT_fileSizeGet (DIRECTORY_ENTRY *pDirEntry)
{
    Ver1And2DirEntry *  pV1Or2DirDesc = (Ver1And2DirEntry *)pDirEntry;
    Ver3DirEntry *      pV3DirDesc = (Ver3DirEntry *)pDirEntry;

    if (NULL == pDirEntry)
        return (0);

    switch (DIR_fileSystemVersionNumGet (NULL))
    {
        case kFileSystemVersion1:
        case kFileSystemVersion2:
            return (scanLong (&(pV1Or2DirDesc->fileSize)));
            /*break;*/

        case kFileSystemVersion3:
            return (scanLong (&(pV3DirDesc->fileSize)));
           /* break;*/

        default:
            return (0);
            /*break;*/
    }
}

/*-----------------------------------------------------------------------*/

static sbyte *DIRENT_fileNameGet (DIRECTORY_ENTRY *pDirEntry)
{
    static char *       emptyName = "";
    Ver1And2DirEntry *  pV1Or2DirDesc = (Ver1And2DirEntry *)pDirEntry;
    Ver3DirEntry *      pV3DirDesc = (Ver3DirEntry *)pDirEntry;

    if (NULL == pDirEntry)
        return (emptyName);

    switch (DIR_fileSystemVersionNumGet (NULL))
    {
        case kFileSystemVersion1:
        case kFileSystemVersion2:
            return (pV1Or2DirDesc->fileName);
           /* break;*/

        case kFileSystemVersion3:
            return (pV3DirDesc->fileName);
           /* break;*/

        default:
            return (emptyName);
           /*break;*/
    }
}

/*-----------------------------------------------------------------------*/

static BOOL FILEMGR_CheckUniqueSignature(sbyte *pMessageCheck)
{
    if (0 == strncmp(pMessageCheck, kUniqueSignature, kUniqueSignatureLength))
        return TRUE;

    return FALSE;
}


/*-----------------------------------------------------------------------*/

extern int size_of_ocsys;

static void FILEMGR_doConstruct(sbyte* RamStart, sbyte4 nTableIndex)
{
    sbyte*  StartFlashMemory;
    sbyte*  FileMgrBaseAddr;
    BOOL FoundSig = FALSE;
    sbyte4          NumFilesInDir;
    DIRECTORY_ENTRY * p_dirDesc;

    StartFlashMemory = RamStart;
    FileMgrBaseAddr  = RamStart;

    mFlashMemoryStartArray[nTableIndex] = StartFlashMemory;

    while (FileMgrBaseAddr < (StartFlashMemory + kNVRamSize))
    {
        if (TRUE == FILEMGR_CheckUniqueSignature(FileMgrBaseAddr))
            if (TRUE == (FoundSig = FILEMGR_CheckUniqueSignature(&FileMgrBaseAddr[kUniqueSignatureLength])))
            {
                mFileMgrDirectoryArray[nTableIndex] = FileMgrBaseAddr;
                break;
            }

        FileMgrBaseAddr += kSectorSize;
    }

    if (TRUE != FoundSig)
    {
        mFileMgrDirectoryArray[nTableIndex] = NULL;
    } else {
            /* Calculate true size of this array buffer for correct bounds
               check in FileRelease() */
        if (DIR_fileSystemVersionNumGet (FileMgrBaseAddr)
                == kFileSystemVersionBad)

            NumFilesInDir = 0;
        else
            NumFilesInDir = DIR_numFilesGet (FileMgrBaseAddr);

        if (0<NumFilesInDir)
        {
            p_dirDesc = DIR_dirEntryGet (FileMgrBaseAddr, NumFilesInDir - 1);
            mFileMgrSizeArray[nTableIndex] = (ubyte4)DIRENT_fileOffsetGet (p_dirDesc)
                    + (ubyte4)DIRENT_fileSizeGet (p_dirDesc);
        }
        else
        {
            p_dirDesc = NULL;
            mFileMgrSizeArray[nTableIndex] = 0;
        }
    }
}



/*-----------------------------------------------------------------------*/

static DIRECTORY_ENTRY *GetFileRecord(sbyte *fileName, sbyte *mFileMgrDirectory)
{
    sbyte4              NumFilesInDir;
    DIRECTORY_ENTRY *   p_dirDesc;
    int                 index;

    if ((NULL == fileName) || (NULL == mFileMgrDirectory))
        return NULL;

    if (DIR_fileSystemVersionNumGet (mFileMgrDirectory)
            == kFileSystemVersionBad)
    {
        /* not able to recognize the version, exit */
        return NULL;
    }

    NumFilesInDir = DIR_numFilesGet (mFileMgrDirectory);

    for (index = 0; index < NumFilesInDir; index++)
    {
        p_dirDesc = DIR_dirEntryGet (mFileMgrDirectory, index);
#ifdef __CASE_INSENSITIVE_FILENAMES__
        if (0 == STRICMP(DIRENT_fileNameGet (p_dirDesc), fileName))
#else
        if (0 == strcmp(DIRENT_fileNameGet (p_dirDesc), fileName))
#endif
            return p_dirDesc;
    }

    return NULL;

}  /* GetFileRecord */



/*-----------------------------------------------------------------------*/

static void *FILEMGR_doRetrieveFile(int *pFileLen, sbyte *fileName, sbyte *mFileMgrDirectory)
{
    DIRECTORY_ENTRY *   p_dirDesc;
    void*               pFile = NULL;

    *pFileLen    = 0;

    if (NULL == (p_dirDesc = GetFileRecord(fileName,mFileMgrDirectory)))
        return NULL;

    pFile = (void *)(&mFileMgrDirectory [DIRENT_fileOffsetGet (p_dirDesc)]);
    *pFileLen = DIRENT_fileSizeGet (p_dirDesc);

    return pFile;
} /* FILEMGR_doRetrieveFile */

/*-----------------------------------------------------------------------*/

extern void FILEMGR_ReleaseFile(void *pFile)
{
}

/*-----------------------------------------------------------------------*/

extern void FILEMGR_ReleaseResource(void *pFile)
{
    return;
}

/*-----------------------------------------------------------------------
 *
 * Methods for handling Multiple File Array Partitions
 *
 */

#ifdef __PSOS_OS__
extern const unsigned char rc_filesys[];
#endif

extern void FILEMGR_Construct(void)
{   int  i;
    ubyte4  memSize;

#ifdef __PSOS_OS__
    if (NULL == rc_filearray[0])
    {
        rc_filearray[0] = (void *) rc_filesys;
        rc_filearray[1] = NULL;
    }
#endif

#ifdef __RLI_PARTITION_FULL__
    /* Find the partition count */
    for (nPartitionCount = 0;rc_filearray[nPartitionCount];nPartitionCount++);

    memSize = (ubyte4)((ubyte4)nPartitionCount * sizeof(sbyte *));
    mFlashMemoryStartArray = (sbyte **) wmosMalloc(memSize);
    memset(mFlashMemoryStartArray,0,memSize);

    mFileMgrDirectoryArray = (sbyte **) wmosMalloc(memSize);
    memset(mFileMgrDirectoryArray,0,memSize);

    memSize = (ubyte4)((ubyte4)nPartitionCount * sizeof(ubyte4 ));
    mFileMgrSizeArray = (ubyte4 *) wmosMalloc(memSize);
    memset(mFileMgrSizeArray,0,memSize);

#endif

    for (i=0;i<nPartitionCount;i++)
    {
        FILEMGR_doConstruct(rc_filearray[i],i);
    }

}

extern void FILEMGR_DeConstruct(void)
{
#ifdef __RLI_PARTITION_FULL__
    /* Delete the partition arrays */
    if (mFlashMemoryStartArray) wmosFree(mFlashMemoryStartArray);
    if (mFileMgrDirectoryArray) wmosFree(mFileMgrDirectoryArray);
    if (mFileMgrSizeArray     ) wmosFree(mFileMgrSizeArray     );
#endif

#if 0
        /* Old code left here for reference purposes. */
    for (int i=0;i<nPartitionCount;i++)
    {
        FILEMGR_deConstruct(rc_filearray[i],i);
    }
#endif
}

extern void *FILEMGR_RetrieveFile(int *pFileLen, sbyte *fileName)
{   int  i;
    void *ret = NULL;

    for (i=0;i<nPartitionCount;i++)
    {
        if (NULL != (ret=FILEMGR_doRetrieveFile(pFileLen,fileName,mFileMgrDirectoryArray[i])))
            return(ret);
    }
    return(ret);
}

extern void *FILEMGR_RetrieveResource(int *pFileLen, sbyte *fileName)
{
		/* The original function of this routine is lost in the mists of time.
		 * This is left as a routine and not a define because it is external
		 * and someone might be using it. */
    return (FILEMGR_RetrieveFile (pFileLen, fileName));
}

/*-----------------------------------------------------------------------*/

#ifdef HTTP_SHOW

int  httpDoNvmShow(int nPartNumber, sbyte *mFileMgrDirectory)
{
    sbyte4              NumFilesInDir;
    sbyte4              index;
    DIRECTORY_ENTRY *   p_dirDesc;
    sbyte4              fileSystemVersionNum;

    if (NULL == mFileMgrDirectory)
        return ERROR;

    fileSystemVersionNum = DIR_fileSystemVersionNumGet (mFileMgrDirectory);

    if (fileSystemVersionNum == kFileSystemVersionBad)
    {
        printf ("%d:***                Unknown file version number\n",
                nPartNumber);
        return ERROR;
    }

    NumFilesInDir = DIR_numFilesGet (mFileMgrDirectory);

    for (index = 0; index < NumFilesInDir; index++)
    {
        p_dirDesc = DIR_dirEntryGet (mFileMgrDirectory, index);

		printf("%d:%3d %016lx %5d '%s'\n",
			nPartNumber,
			index + 1,
			(unsigned long) p_dirDesc,
			DIRENT_fileSizeGet (p_dirDesc),
			(char *)(DIRENT_fileNameGet (p_dirDesc)));
    }

    return OK;

}  /* GetFileRecord */

extern int httpNvmShow(void)
{
    int  i;
    sbyte4          fileSystemVersionNum;

        /* Since all partitions have the same version, we can use the
         * first. */
    fileSystemVersionNum = DIR_fileSystemVersionNumGet
            (mFileMgrDirectoryArray [0]);

	printf("=== ROM File Array, Version #%d ", fileSystemVersionNum);

    switch (fileSystemVersionNum)
    {
        case kFileSystemVersion1:
            printf ("{User-def endian, 64 byte file names}");
            break;

        case kFileSystemVersion2:
            printf ("{Network Order, 64 byte file names}");
            break;

        case kFileSystemVersion3:
            printf ("{Network Order, 256 byte file names}");
            break;

        default:
            printf ("{Unknown}");
            break;
    }

    printf (" ===\n");

	printf("Index Address  Size  File \n");
	printf("----- -------- ----- ------------------------------\n");


    for (i=0;i<nPartitionCount;i++)
    {
        (void) httpDoNvmShow(i,mFileMgrDirectoryArray[i]);
	}

    return(OK);
}

#endif   /* HTTP_SHOW */

/*-----------------------------------------------------------------------*/

#endif /* __NO_ROMBUILD_IMAGE__ */


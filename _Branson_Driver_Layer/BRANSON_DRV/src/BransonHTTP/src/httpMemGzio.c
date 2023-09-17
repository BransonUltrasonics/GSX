/* httpMemGzio.c  -  read a gzip file from memory */

/* Copyright(c) 2004 - 2007, 2010, 2012 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01f,01mar12,r_w  remove compile warning for defect WIND00335205
01e,02feb10,h_y  support for LP64 
01d,22feb07,ps   apigen reformatting
01c,13feb07,msa  Fix comments to remove APIgen errors.
01b,19feb04,jws  add copyright
01a,16jan04,jws  add references to globals
                 add modification history
*/

/*
DESCRIPTION:
File access functions for the ROMBUILD filemanager compressed files.


*/

#include <stdio.h>


#include "wm.h"
#include "httpNvFileMgr.h"
#include "zlib.h"
#include "httpMemGzio.h"


#if !defined(__NO_ROMBUILD_IMAGE__) || defined(COMMON_IS_COMPONENT)

/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

static const sbyte  gz_magic[2] = {'\37', '\213'}; /* gzip magic header 0x1f8b */
static const sbyte4 gzipHeaderTail = 8; /* trailing bytes after payload */

/* prototypes */
static ubyte4 memGzGetCrc(const sbyte *pGzipFile, ubyte4 fileLen);
static sbyte * gzipStripHeader (const sbyte *, ubyte4 *);
static ubyte4 memGzGetLen (const sbyte *pGzipFile, ubyte4 fileLen);

/*******************************************************************************
*
* memGzOpen - open a compressed file from the ROMBUILD filemanager.
*
* If the file is not found or it turns out to be not compressed, the
* returned pMemGzFile is NULL.
*
* RETURNS OK or ERROR, and the (uncompressed) file size
*/

STATUS memGzOpen
    (
    sbyte * filename,
    memGzFile_t ** ppFile,
    ubyte4 * size
    )
    {
    void * pGzFileInMem;
    int cmpSize;
    ubyte4 gzipHeaderSize = 0;
    z_stream * pZ;

    (*ppFile) = (memGzFile_t *) wmosMalloc(sizeof(memGzFile_t));
    if (NULL == *ppFile) return wmError(ERROR_MEMMGR_NO_MEMORY, "no mem");
    pGzFileInMem = FILEMGR_RetrieveFile (&cmpSize, filename);
    if (NULL == pGzFileInMem)
        {
        wmosFree(*ppFile);
        (*ppFile) = NULL;
        return OK;
        }
    (*size) = memGzGetLen(pGzFileInMem, cmpSize);
    if (0 == (*size))  /* file not compressd */
        {
        wmosFree(*ppFile);
        (*ppFile) = NULL;
        return OK;
        }
    (*ppFile)->startpos = gzipStripHeader(pGzFileInMem, &gzipHeaderSize);
    (*ppFile)->crc = (ubyte4)crc32(0L, Z_NULL, 0);
    (*ppFile)->cmpSize = cmpSize;
    (*ppFile)->size = (*size);
    (*ppFile)->savedCrc = memGzGetCrc(pGzFileInMem, cmpSize);
    (*ppFile)->zstrsize = cmpSize - gzipHeaderSize - gzipHeaderTail;
    pZ = &(*ppFile)->stream;
    pZ->next_in = (ubyte *) (*ppFile)->startpos;
    pZ->avail_in = (*ppFile)->zstrsize;
    pZ->next_out = Z_NULL;
    pZ->avail_out = Z_NULL;
    pZ->zalloc = Z_NULL;
    pZ->zfree = Z_NULL;
    pZ->opaque = Z_NULL;
    /* see gz_read on why inflateInit2 is called instead of inflateInit */
    if (Z_OK != inflateInit2(pZ, -MAX_WBITS))
        return wmError(ERROR_GENERAL, "unable to init inflate");
    return OK;
    }

/*******************************************************************************
*
* memGzClose - Close a compressed file from the ROMBUILD filemanager.
*
* If the file is not found or it turns out to be not compressed, the
* returned pMemGzFile is NULL.
*
* RETURNS OK or ERROR
*/
STATUS memGzClose
    (
    memGzFile_t * pFile
    )
    {
    if (NULL != pFile)
        {
        if (Z_OK != inflateEnd(&pFile->stream))
            return wmError(ERROR_GENERAL, "error cleanup after decompressing");
        wmosFree(pFile);
        }
    return OK;
    }

/*******************************************************************************
*
* memGzRead - Read a compressed file from the ROMBUILD filemanager.
*
* Read a compressed file from the ROMBUILD filemanager. If the file
* is not found or it turns out to be not compressed, the  returned
* pMemGzFile is NULL.
*
* RETURNS OK or ERROR, and the (uncompressed) file size
*/
STATUS memGzRead
    (
    memGzFile_t * pFile,
    sbyte *       pDecompBuffer, /* buffer to copy uncompressed output */
    ubyte4 *      pDecompSize    /* in: bytes to read  out: bytes read */
    )
    {
    sbyte4 retVal;

    if (0 == (*pDecompSize)) return OK;
    pFile->stream.next_out = (ubyte *) pDecompBuffer;
    pFile->stream.avail_out = (*pDecompSize);

    if (-1 == (retVal = inflate(&pFile->stream,Z_NO_FLUSH)))
        return wmError(ERROR_GENERAL, "error decompressing from mem");

    pFile->crc = (ubyte4)crc32(pFile->crc, (const ubyte *) pDecompBuffer,
                           (unsigned int)(pFile->stream.next_out - (ubyte *)pDecompBuffer));
    if (Z_STREAM_END == retVal)
        {
        if (pFile->crc != pFile->savedCrc)
            return wmError(ERROR_GENERAL, "bad crc on compressed file");
        pFile->crc = (ubyte4)crc32(pFile->crc, NULL, 0L);
        }
    return OK;
    }

/*******************************************************************************
*
* memGzSeek - set the next read position at an offset of the file origin.
*
* This function is emulated, so it may be very slow...
*
* RETURNS OK or ERROR, and the (uncompressed) file size
*/
STATUS memGzSeek
    (
    memGzFile_t * pFile,
    ubyte4 offset)
    {
#define SEEK_BUF_SIZE 64
    sbyte  buffer[SEEK_BUF_SIZE];
    ubyte4 bytesLeft = offset;
    ubyte4 bytesRead = 0;
    STATUS s = OK;

    /* reset stream */
    if (Z_OK != inflateEnd(&pFile->stream))
        return wmError(ERROR_GENERAL, "error cleanup after decompressing");
    if (Z_OK != inflateInit2(&pFile->stream, -MAX_WBITS))
        return wmError(ERROR_GENERAL, "unable to init inflate");
    pFile->crc = (ubyte4)crc32(0L, Z_NULL, 0);
    pFile->stream.next_in = (ubyte *) pFile->startpos;
    pFile->stream.avail_in = pFile->zstrsize;
    /* read and descard an offset amount of bytes */
    while ((OK == s) && (0 != bytesLeft))
        {
        bytesRead = (SEEK_BUF_SIZE < bytesLeft) ? SEEK_BUF_SIZE : bytesLeft;
        s = memGzRead(pFile, buffer, &bytesRead);
        bytesLeft -= bytesRead;
        }
    return s;
    }


/*-----------------------------------------------------------------------*/
/******************************************************************
* 
* memGzGetLen - 
*
* NOMANUAL
*/

static ubyte4 memGzGetLen
    (
    const sbyte *pGzipFile, /* starting memory position of the gzip file */
    ubyte4 fileLen          /* compressed file length                    */
    )
    {
    ubyte   *pDataLenLocation;
    ubyte4  decompressedLen = 0;

    if ((pGzipFile[0] == gz_magic[0]) && (pGzipFile[1] == gz_magic[1]))
        {
        pDataLenLocation = ((ubyte*)pGzipFile + fileLen) - 4;
        decompressedLen =  (pDataLenLocation[3] << 24)
                         | (pDataLenLocation[2] << 16)
                         | (pDataLenLocation[1] << 8 )
                         | (pDataLenLocation[0] );

        }
    return decompressedLen;
    }

/******************************************************************
* 
* memGzGetCrc - 
*
* NOMANUAL
*/

static ubyte4 memGzGetCrc
    (
    const sbyte *pGzipFile,
    ubyte4 fileLen
    )
    {
    ubyte   *pCrcLocation;
    ubyte4  crcVal;

    if ((pGzipFile[0] == gz_magic[0]) && (pGzipFile[1] == gz_magic[1]))
        {
        pCrcLocation = ((ubyte*)pGzipFile + fileLen) - 8;
        crcVal          =  (pCrcLocation[3] << 24)
                         | (pCrcLocation[2] << 16)
                         | (pCrcLocation[1] << 8 )
                         | (pCrcLocation[0] );

        return crcVal;
        }
    else
        return 0;
    }

/*-----------------------------------------------------------------------*/
/******************************************************************
* 
* gzipStripHeader - 
*
* NOMANUAL
*/

static sbyte * gzipStripHeader
    (
    const sbyte * memFile,
    ubyte4 * headerSize
    )
    {
    sbyte  method; /* method byte */
    sbyte  flags;  /* flags byte */
    ubyte4 len;
    sbyte  c;
    sbyte * startFile = (sbyte *) memFile;

    /* Check the gzip magic header */
    for (len = 0; len < 2; len++)
        if ((*memFile++) != gz_magic[len]) return NULL; /* invalid gzip */

    method = *memFile++;
    flags = *memFile++;
    if (method != Z_DEFLATED || (flags & RESERVED) != 0)
        return NULL;  /* invalid format */

    /* Discard time, xflags and OS code: */
    memFile += 6;

    if ((flags & EXTRA_FIELD) != 0)  /* skip the extra field */
        {
        len  =  *memFile++;
        len  +=  (*memFile++)<<8;
        /* len is garbage if EOF but the loop below will quit anyway */
        while (len-- != 0 && (*memFile++) != (sbyte) EOF);
        }
    if ((flags & ORIG_NAME) != 0)
        while ((c = (*memFile++)) != 0 && c != (sbyte) EOF);

    if ((flags & COMMENT) != 0)  /* skip the .gz file comment */
        while ((c = (*memFile++)) != 0 && c != (sbyte) EOF) ;

    if ((flags & HEAD_CRC) != 0)   /* skip the header crc */
        for (len = 0; len < 2; len++) (void)(*memFile++);

    (*headerSize) = (ubyte4)(memFile - startFile);
    return (sbyte *) memFile;
    }

#endif /* __NO_ROMBUILD_IMAGE__ */

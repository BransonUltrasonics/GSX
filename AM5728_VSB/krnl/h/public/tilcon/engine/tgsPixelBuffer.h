/* tgsPixelBuffer.h - pixel buffer header */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
14mar16,jnl  support window surface (F6570)
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines Tilcon's pixel buffer.
*/

#ifndef _INCLUDE_tgsPixelBuffer_H_
#define _INCLUDE_tgsPixelBuffer_H_

#include "tilcon/tilcon.h"

class TPixelBuffer
    {
    friend class TBrush;
    friend class TPainter;
    friend class TVectorPainter;
public:

    TPixelBuffer();
    TPixelBuffer(TRT_S32 w, TRT_S32 h);
    TPixelBuffer(const TPixelBuffer &);
    TPixelBuffer(const TImage &);
    ~TPixelBuffer();

    TPixelBuffer & operator = (const TPixelBuffer &);
    TPixelBuffer & Duplicate(const TPixelBuffer &);

    TRT_S32 GetWidth() const;
    TRT_S32 GetHeight() const;

    void Blit(const TPoint &, const TPoint &, const TSize &);

    void Read(const TPoint & offset, const TSize & size,
        TRT_BYTE * buffer, ColorFormat bufferFormat, TRT_S32 bufferStride) const;

    void Write(const TPoint & offset, const TSize & size,
        const TRT_BYTE * buffer, ColorFormat bufferFormat, TRT_S32 bufferStride);

/*******************************************************************************
*
* GetHandle - get the platform specific object handle
*
* SYNOPSIS
* \ss
* TRT_HANDLE GetHandle() const
* \se
*
* DESCRIPTION
* This function returns the object handle.
*
* RETURNS: object handle.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_HANDLE GetHandle() const
        {
        return data->handle;
        }

private:
    class Data
        {
    public:
        Data(TRT_PBUFFER_HANDLE h);
        ~Data();
        TRT_PBUFFER_HANDLE  handle;
        };

    TSharedPtr<Data>   data;
    };

#endif /* _INCLUDE_tgsPixelBuffer_H_ */


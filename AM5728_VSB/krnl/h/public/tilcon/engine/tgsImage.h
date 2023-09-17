/* tgsImage.h - Tilcon image class */

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
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's image class.

TImage class is used to load image from permanent storage. Supported image
type includes BMP, PNG, JPEG.

INCLUDE FILES: tilcon/engine/tgsImage.h
*/

#ifndef _INCLUDE_tgsImage_H_
#define _INCLUDE_tgsImage_H_

#include "tilcon/tilcon.h"

/* internal data for TImage */

struct ImageData
    {
public:
    ImageData();
    ImageData(TRT_BYTE * _buffer,
               TRT_S32 _width, TRT_S32 _height, TRT_S32 _stride,
               ColorFormat _format);
    ~ImageData();

    const TRT_BYTE *    buffer;
    TRT_S32             width;
    TRT_S32             height;
    TRT_S32             stride;
    ColorFormat         format;
    };

class TPixelBuffer;

/* image class */

class TImage
    {
public:
    /* image type */
    enum ImageType
        {
        Type_BMP = 0,
        Type_PNG,
        Type_JPEG,
        };

public:

/*******************************************************************************
*
* TImage - construct an image
*
* SYNOPSIS
* \ss
* void TImage()
* \se
*
* DESCRIPTION
* This function constructs a null image.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TImage() :
        imageData(defaultImageData)
        {
        }

/*******************************************************************************
*
* TImage - construct an image from a buffer.
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     TRT_BYTE * address,           /@ address of the buffer @/
*     TRT_S32 width,                /@ width of the buffer @/
*     TRT_S32 height,               /@ height of the buffer @/
*     TRT_S32 stride,               /@ stride of the buffer @/
*     ColorOrder order              /@ color order of the buffer @/
*     ColorFormat desiredFormat
*         = CF_ARGB8888             /@ desired format @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a image instance with specified color buffer.
* The color buffer is filled with color with specified channel order.
*
* Parameter:
* \is
* \i <address>
* the address of the buffer;
* \i <width>
* the width of the buffer;
* \i <height>
* the height of the buffer;
* \i <stride>
* the stride of the buffer;
* \i <order>
* the color order of the buffer;
* \i <desiredFormant>
* the desired color format for the loaded buffer. If it is not the same with
* the color format in the image file, this function converts the format to the
* desired format;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TImage
        (
        TRT_BYTE * address,
        TRT_S32 width,
        TRT_S32 height,
        TRT_S32 stride,
        ColorOrder order,
        ColorFormat desiredFormat = CF_ARGB8888
        );

/*******************************************************************************
*
* TImage - construct an image from a buffer.
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     TRT_BYTE * address,           /@ address of the buffer @/
*     TRT_S32 width,                /@ width of the buffer @/
*     TRT_S32 height,               /@ height of the buffer @/
*     TRT_S32 stride,               /@ stride of the buffer @/
*     ColorFormat format            /@ color order of the buffer @/
*     ColorFormat desiredFormat
*         = CF_ARGB8888             /@ desired format @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a image instance with specified color buffer.
* The color buffer is filled with color with specified format.
*
* Parameter:
* \is
* \i <address>
* the address of the buffer;
* \i <width>
* the width of the buffer;
* \i <height>
* the height of the buffer;
* \i <stride>
* the stride of the buffer;
* \i <format>
* the color format of the buffer;
* \i <desiredFormant>
* the desired color format for the loaded buffer. If it is not the same with
* the color format in the image file, this function converts the format to the
* desired format;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TImage
        (
        TRT_BYTE * address,
        TRT_S32 width,
        TRT_S32 height,
        TRT_S32 stride,
        ColorFormat format,
        ColorFormat desiredFormat = CF_ARGB8888
        );


/*******************************************************************************
*
* TImage - construct an image as a copy of another image
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     const TImage & image  /@ image to copy @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a image by copying from another image.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TImage(const TImage & image)
        : imageData(image.imageData)
        {
        }

/*******************************************************************************
*
* TImage - construct an image from a pixmap
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     const TPixmap & pixmap  /@ pixmap to construct image @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a image from a pixmap.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    explicit TImage(const TPixelBuffer &);

/*******************************************************************************
*
* ~TImage - destruct the image
*
* This function destructs the image instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TImage()
        {
        }

/*******************************************************************************
*
* LoadFromFile - load the image content from file.
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     ImageType type,               /@ type of the image file @/
*     TRT_LPCSTR pathname,          /@ pathname of the image file @/
*     ColorFormat desiredFormat
*         = CF_ARGB8888             /@ desired format @/
*     )
* \se
*
* DESCRIPTION
* This function loads the image content from file.
*
* Parameter:
* \is
* \i <type>
* the type of image to be loaded (BMP, PNG, JPEG;
* \i <pathname>
* the pathname of the image file;
* \i <desiredFormant>
* the desired color format for the loaded buffer. If it is not the same with
* the color format in the image file, this function converts the format to the
* desired format;
* \ie
*
* RETURNS: TRT_STATUS_OK if loading successful; otherwise TRT_STATUS_ERROR;
*
* ERRNO: N/A
*
* SEE ALSO: LoadFromBuffer(), Unload()
*/
    TRT_STATUS LoadFromFile
        (
        ImageType _type,
        TRT_LPCSTR _pathname,
        ColorFormat desiredFormat = CF_ARGB8888
        );

/*******************************************************************************
*
* LoadFromBuffer - load the image content from a memory location.
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     TRT_BYTE * address,               /@ address of the buffer @/
*     TRT_S32 width,                    /@ width of the buffer @/
*     TRT_S32 height,                   /@ height of the buffer @/
*     TRT_S32 stride,                   /@ stride of the buffer @/
*     ColorOrder order                  /@ color order of the buffer @/
*     ColorFormat desiredFormat
*         = CF_ARGB8888                 /@ desired format @/
*     )
* \se
*
* DESCRIPTION
* This function loads the image content from a memory location. The memory
* location is filled with colors of known channel order.
*
* Parameter:
* \is
* \i <address>
* the address of the buffer;
* \i <width>
* the width of the buffer;
* \i <height>
* the height of the buffer;
* \i <stride>
* the stride of the buffer;
* \i <order>
* the channel order of the buffer;
* \i <desiredFormant>
* the desired color format for the loaded buffer. If it is not the same with
* the color format in the image file, this function converts the format to the
* desired format;
* \ie
*
* RETURNS: TRT_STATUS_OK if loading successful; otherwise TRT_STATUS_ERROR;
*
* ERRNO: N/A
*
* SEE ALSO: LoadFromFile(), Unload()
*/
    TRT_STATUS LoadFromBuffer
        (
        const TRT_BYTE * _address,
        TRT_S32 _width,
        TRT_S32 _height,
        TRT_S32 _stride,
        ColorOrder _order,
        ColorFormat desiredFormat = CF_ARGB8888
        );

/*******************************************************************************
*
* LoadFromBuffer - load the image content from a memory location.
*
* SYNOPSIS
* \ss
* void TImage
*     (
*     TRT_BYTE * address,               /@ address of the buffer @/
*     TRT_S32 width,                    /@ width of the buffer @/
*     TRT_S32 height,                   /@ height of the buffer @/
*     TRT_S32 stride,                   /@ stride of the buffer @/
*     ColorFormat format                /@ color format of the buffer @/
*     ColorFormat desiredFormat
*         = CF_ARGB8888                 /@ desired format @/
*     )
* \se
*
* DESCRIPTION
* This function loads the image content from a memory location. The memory
* location is filled with colors of known format.
*
* Parameter:
* \is
* \i <address>
* the address of the buffer;
* \i <width>
* the width of the buffer;
* \i <height>
* the height of the buffer;
* \i <stride>
* the stride of the buffer;
* \i <format>
* the color format of the buffer;
* \i <desiredFormant>
* the desired color format for the loaded buffer. If it is not the same with
* the color format in the image file, this function converts the format to the
* desired format;
* \ie
*
* RETURNS: TRT_STATUS_OK if loading successful; otherwise TRT_STATUS_ERROR;
*
* ERRNO: N/A
*
* SEE ALSO: LoadFromFile(), Unload()
*/
    TRT_STATUS LoadFromBuffer
        (
        const TRT_BYTE * _address,
        TRT_S32 _width,
        TRT_S32 _height,
        TRT_S32 _stride,
        ColorFormat _format,
        ColorFormat desiredFormat = CF_ARGB8888
        );

/*******************************************************************************
*
* Unload - unload any previously loaded image.
*
* SYNOPSIS
* \ss
* void Unload()
* \se
*
* DESCRIPTION
* This function unloads any previously loaded image.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: LoadFromFile(), LoadFromBuffer();
*/
    void Unload();

/*******************************************************************************
*
* GetWidth - get the width of the image.
*
* SYNOPSIS
* \ss
* TRT_S32 GetWidth() const
* \se
*
* DESCRIPTION
* This function gets the width of the image.
*
* RETURNS: the width of the image;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetWidth() const;

/*******************************************************************************
*
* GetHeight - get the height of the image.
*
* SYNOPSIS
* \ss
* TRT_S32 GetHeight() const
* \se
*
* DESCRIPTION
* This function gets the height of the image.
*
* RETURNS: the height of the image;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetHeight() const;

/*******************************************************************************
*
* GetStride - get the stride of the image.
*
* SYNOPSIS
* \ss
* TRT_S32 GetStride() const
* \se
*
* DESCRIPTION
* This function gets the stride of the image.
*
* RETURNS: the stride of the image;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetStride() const;

/*******************************************************************************
*
* GetBuffer - get the buffer address of the image.
*
* SYNOPSIS
* \ss
* const TRT_BYTE * GetBuffer() const
* \se
*
* DESCRIPTION
* This function gets the buffer address of the image.
*
* RETURNS: the buffer address of the image;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    const TRT_BYTE * GetBuffer() const;

/*******************************************************************************
*
* GetBuffer - get the buffer address with an offset.
*
* SYNOPSIS
* \ss
* const TRT_BYTE * GetBuffer
*   (
*   TRT_S32 x,  /@ x offset @/
*   TRT_S32 y   /@ y offset @/
*   ) const
* \se
*
* DESCRIPTION
* This function gets the buffer address with an offset.
*
* RETURNS: the buffer address with an offset;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    const TRT_BYTE * GetBuffer(TRT_S32 x, TRT_S32 y) const;

/*******************************************************************************
*
* GetFormat - get the color format of the image.
*
* SYNOPSIS
* \ss
* ColorFormat GetFormat() const
* \se
*
* DESCRIPTION
* This function gets the color format of the image.
*
* RETURNS: the color format of the image;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    ColorFormat GetFormat() const;

/*******************************************************************************
*
* GetColor - get pixel color at specified coordinate.
*
* SYNOPSIS
* \ss
* const TColor GetColor
*     (
*     TRT_S32 x,    /@ x coordinate of the pixel @/
*     TRT_S32 y     /@ y coordinate of the pixel @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets pixel color at specified coordinate.
*
* RETURNS: pixel color at specified coordinate
*
* ERRNO: N/A
*
* SEE ALSO: SetColor()
*/
    const TColor GetColor
        (
        TRT_S32 x,
        TRT_S32 y
        ) const;

/*******************************************************************************
*
* IsNull - get whether the image is a null image.
*
* SYNOPSIS
* \ss
* TRT_BOOL IsNull() const
* \se
*
* DESCRIPTION
* This function gets whether the image is a null image (buffer == 0);
*
* RETURNS: whether the image is a null image
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_BOOL IsNull() const;

/*******************************************************************************
*
* Convert - convert a source color buffer to destination color buffer
*
* SYNOPSIS
* \ss
* static void Convert
*     (
*     TRT_BYTE * dstBuf,            /@ destiny buffer address @/
*     TRT_S32 dstStride,            /@ destiny buffer stride @/
*     ColorFormat dstFormat,/@ destiny buffer format @/
*     const TRT_BYTE * srcBuf,      /@ source buffer address @/
*     TRT_S32 srcStride,            /@ source buffer stride @/
*     ColorFormat srcFormat,/@ source buffer format @/
*     TRT_S32 width,                /@ width of both buffers @/
*     TRT_S32 height                /@ height of both buffers @/
*     )
* \se
*
* DESCRIPTION
* This function converts a source color buffer to destination color buffer.
* The tow buffer shall have same dimension, i.e., the same pixel width and the
* same pixel height;
*
* Parameters:
* \is
* \i <dstBuf>
* the destiny buffer's address;
* \i <dstStride>
* the destiny buffer's stride;
* \i <dstFormat>
* the destiny buffer's color format;
* \i <srcBuf>
* the source buffer's address;
* \i <srcStride>
* the source buffer's stride;
* \i <srcFormat>
* the source buffer's format;
* \i <width>
* the width for both buffers;
* \i <height>
* the height for both buffers;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    static void Convert
        (
        TRT_BYTE * dstBuf,
        TRT_S32 dstStride,
        ColorFormat dstFormat,
        const TRT_BYTE * srcBuf,
        TRT_S32 srcStride,
        ColorFormat srcFormat,
        TRT_S32 width,
        TRT_S32 height
        );

    void Dump() const;

private:
    TRT_STATUS LoadBMPImage(TRT_LPCSTR, ColorFormat);
    TRT_STATUS LoadPNGImage(TRT_LPCSTR, ColorFormat);
    TRT_STATUS LoadJPEGImage(TRT_LPCSTR, ColorFormat);

    TSharedPtr<ImageData> imageData;

    static TSharedPtr<ImageData> defaultImageData;
    };

#endif /* _INCLUDE_tgsImage_H_ */


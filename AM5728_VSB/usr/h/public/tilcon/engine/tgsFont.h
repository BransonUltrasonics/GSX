/* tgsFont.h - Tilcon font class */

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

This file defines the Tilcon's font class.

INCLUDE FILES: tilcon/engine/tgsFont.h
*/

#ifndef _INCLUDE_tgsFont_H_
#define _INCLUDE_tgsFont_H_

#include "tilcon/tilcon.h"

class TFont
    {
    friend class TPainter;
public:
    enum FontType
        {
        Type_TTF = 0,
        Type_BMF,
        };

public:
/*******************************************************************************
*
* TFont - construct a font
*
* SYNOPSIS
* \ss
* TFont()
* \se
*
* DESCRIPTION
* This function constructs a default font.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TFont();

/*******************************************************************************
*
* TFont - construct a font with specified face name and point size
*
* SYNOPSIS
* \ss
* TFont
*     (
*     TRT_LPCSTR faceName,  /@ face name of the font @/
*     TRT_U32 pointSize     /@ point size of the font @/
*     )
* \se
*
* DESCRIPTION
* This function construct a font with specified face name and point size.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TFont
        (
        TRT_LPCSTR  faceName,
        TRT_U32     pointSize
        );

/*******************************************************************************
*
* ~TFont - destruct the font
*
* This function destructs the font.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TFont();

/*******************************************************************************
*
* operator = - copy from another font
*
* SYNOPSIS
* \ss
* TFont & operator =
*     (
*     const TFont & font    /@ font to be copied @/
*     );
* \se
*
* DESCRIPTION
* This function copies attributes of this font from another font.
*
* RETURNS: reference to this font.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TFont & operator =
        (
        const TFont & font
        );

/*******************************************************************************
*
* operator == - compare if the font is equal with another font
*
* SYNOPSIS
* \ss
* TRT_BOOL operator ==
*     (
*     const TFont & font    /@ font to be compared @/
*     );
* \se
*
* DESCRIPTION
* This function compares the attributes of this font with another font.
*
* RETURNS: TRT_TURE if equal, TRT_FALSE if not equal
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_BOOL operator ==
        (
        const TFont & font
        ) const;

/*******************************************************************************
*
* operator != - compare if the font is not equal with another font
*
* SYNOPSIS
* \ss
* TRT_BOOL & operator !=
*     (
*     const TFont & font    /@ font to be compared @/
*     );
* \se
*
* DESCRIPTION
* This function compares the attributes of this font with another font.
*
* RETURNS: TRT_TURE if not equal, TRT_FALSE if equal
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_BOOL operator !=
        (
        const TFont & font
        ) const;

/*******************************************************************************
*
* GetType - get the type of the font
*
* SYNOPSIS
* \ss
* TFont::FontType GetType() const
* \se
*
* DESCRIPTION
* This function gets the type of the font.
*
* RETURNS: type of the font
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TFont::FontType GetType() const;

/*******************************************************************************
*
* GetPointSize - get the point size of the font
*
* SYNOPSIS
* \ss
* TRT_S32 GetPointSize() const
* \se
*
* DESCRIPTION
* This function gets the point size of the font.
*
* RETURNS: point size of the font
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetPointSize() const;

/*******************************************************************************
*
* GetFaceName - get the face name of the font
*
* SYNOPSIS
* \ss
* const TString & GetFaceName() const
* \se
*
* DESCRIPTION
* This function gets the face name of the font.
*
* RETURNS: face name of the font
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    const TString & GetFaceName() const;

/*******************************************************************************
*
* GetMetrics - get the metrics of this font
*
* SYNOPSIS
* \ss
* void GetMetrics
*     (
*     FontMetrics & metrics  /@ struct to hold metrix data @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the metrics of the font.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextWidth(), GetTextBound()
*/
    void GetMetrics(FontMetrics & metrics) const;

/*******************************************************************************
*
* GetTextWidth - get the width of a string rendered using this font
*
* SYNOPSIS
* \ss
* TRT_S32 GetTextWidth
*     (
*     const TWString & str     /@ the string to be measured @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the width of a string rendered using this font on the
* specified paint device.
*
* Parameters:
* \is
* \i <str>
* the string to be measured;
* \ie
*
* RETURNS: the width of the string rendered using this font; 0 if specified
* C-style string is not valid.
*
* RETURNS: the measured width.
*
* ERRNO: N/A
*
* SEE ALSO: GetMetrics(), GetTextBound()
*/
    TRT_S32 GetTextWidth
        (
        const TWString & str
        ) const
        {
        return GetTextWidth(str.Text(), str.Len());
        }

/*******************************************************************************
*
* GetTextWidth - get the width of a string rendered using this font
*
* SYNOPSIS
* \ss
* TRT_S32 GetTextWidth
*     (
*     TRT_LPCWSTR text,         /@ start address of the C-style string @/
*     TRT_U32 len               /@ length of the C-style string @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the width of a string rendered using this font on the
* specific device.
*
* Parameters:
* \is
* \i <text>
* the start address of the C-style string;
* \i <len>
* the length of the C-style string;
* \ie
*
* RETURNS: the width of the string rendered using this font; 0 if specified
* C-style string is not valid.
*
* ERRNO: N/A
*
* SEE ALSO: GetMetrics(), GetTextBound()
*/
    TRT_S32 GetTextWidth
        (
        TRT_LPCWSTR text,
        TRT_U32 len
        ) const;

/*******************************************************************************
*
* GetTextBound - get the bounding rectangle to render string using this font
*
* SYNOPSIS
* \ss
* TRect GetTextBound
*     (
*     const TRect & rect,               /@ target rectangle @/
*     TextHorizontalAlignment horAlign, /@ horizontal alignment @/
*     TextVerticalAlignment verAlign,   /@ vertical alignment @/
*     const TWString & str              /@ string to be measured @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the bounding rectangle to render a string using this font.
* This function provides a rectangle from the target area to render the text,
* the horizontal and vertical alignment, and the text content; the function
* returns the bounding rectangle of the rendered text within the area;
*
* Parameters:
* \is
* \i <rect>
* the the target rectangle inside which the text is to be rendered;
* \i <horAlign>
* the horizontal alignment of the text;
* \i <verAlign>
* the vertical alignments of the text;
* \i <str>
* the TWString of which the width is to be calculated;
* \ie
*
* RETURNS: the bounding rectangle to render a string using this font.
* TRect::nullRect if the input parameters are not valid.
*
* ERRNO: N/A
*
* SEE ALSO: GetTextWidth(), GetMetrics(), CalculateTextBound()
*/
    TRect GetTextBound
        (
        const TRect & rect,
        TextHorizontalAlignment horAlign,
        TextVerticalAlignment verAlign,
        const TWString & str
        ) const
        {
        return GetTextBound(rect, horAlign, verAlign,
            str.Text(), str.Len());
        }

/*******************************************************************************
*
* GetTextBound - get the bounding rectangle to render string using this font
*
* SYNOPSIS
* \ss
* TRect GetTextBound
*     (
*     const TRect & rect,               /@ target rectangle @/
*     TextHorizontalAlignment horAlign, /@ horizontal alignment @/
*     TextVerticalAlignment verAlign,   /@ vertical alignment @/
*     TRT_LPCWSTR text,                 /@ start of the C-style string @/
*     TRT_U32 len                       /@ length of the C-style string @/
*     )
* \se
*
* DESCRIPTION
* This function gets the bounding rectangle to render a string using this font.
* This function provides a rectangle from the target area to render the text,
* the horizontal and vertical alignment, and the text content; the function
* returns the bounding rectangle of the rendered text within the area;
*
* Parameters:
* \is
* \i <rect>
* the the target rectangle inside which the text is to be rendered;
* \i <horAlign>
* the horizontal alignment of the text;
* \i <verAlign>
* the vertical alignments of the text;
* \i <text>
* the start address of the C-style string;
* \i <len>
* the length of the C-style string;
* \ie
*
* RETURNS: the bounding rectangle to render a string using this font.
* TRect::nullRect if the input parameters are not valid.
*
* ERRNO: N/A
*
* SEE ALSO: GetTextWidth(), GetMetrics(), CalculateTextBound()
*/
    TRect GetTextBound
        (
        const TRect & rect,
        TextHorizontalAlignment horAlign,
        TextVerticalAlignment verAlign,
        TRT_LPCWSTR text,
        TRT_U32 len
        ) const;

/*******************************************************************************
*
* WidthToIndex - get string index where the length is nearest to specified width
*
* SYNOPSIS
* \ss
* TRT_S32 WidthToIndex
*     (
*     TRT_S32 width,                /@ the target width @/
*     TRT_S32 & actualWidth,        /@ the actual width @/
*     const TWString & str          /@ string to be measured @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the index which defines a sub string of which the width is
* nearest to the specified width. This function is typically used to calculate
* the text selection by mouse dragging. The actual width of the sub string is
* also returned.
*
* Parameters:
* \is
* \i <width>
* the targeted width against which the sub string is to be found;
* \i <actaulWidth>
* the actual width of the sub string found;
* \i <str>
* the TWString of which the width is to be calculated;
* \ie
*
* RETURNS: the end-index of the calculated sub string.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 WidthToIndex
        (
        TRT_S32 width,
        TRT_S32 & actualWidth,
        const TWString & str
        ) const
        {
        return WidthToIndex(width, actualWidth, str.Text(), str.Len());
        }

/*******************************************************************************
*
* WidthToIndex - get string index where the length is nearest to specified width
*
* SYNOPSIS
* \ss
* TRT_S32 WidthToIndex
*     (
*     TRT_S32 width,                /@ the target width @/
*     TRT_S32 & actualWidth,        /@ the actual width @/
*     TRT_LPCWSTR text,             /@ start of the C-style string @/
*     TRT_U32 len                  /@ length of the C-style string @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the index which defines a sub string of which the width is
* nearest to the specified width. This function is typically used to calculate
* the text selection by mouse dragging. The actual width of the sub string is
* also returned.
*
* Parameters:
* \is
* \i <width>
* the targeted width against which the sub string is to be found;
* \i <actaulWidth>
* the actual width of the sub string found;
* \i <text>
* the start address of the C-style string;
* \i <len>
* the length of the C-style string;
* \ie
*
* RETURNS: the end-index of the calculated sub string.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 WidthToIndex
        (
        TRT_S32 width,
        TRT_S32 & actualWidth,
        TRT_LPCWSTR text,
        TRT_S32 len
        ) const;

/*******************************************************************************
*
* CalculateTextBound - helper to calculate text bounding rectangle
*
* SYNOPSIS
* \ss
* TRect CalculateTextBound
*     (
*     const TRect & rect,               /@ target rectangle @/
*     const TSize & size,               /@ width and height of text @/
*     TextHorizontalAlignment horAlign, /@ horizontal alignment @/
*     TextVerticalAlignment verAlign    /@ vertical alignment @/
*     )
* \se
*
* DESCRIPTION
* This function calculates the bounding rectangle based on bounding rect, width
* and height of text, and horizontal and vertical alignment policy;
*
* Parameters:
* \is
* \i <rect>
* the the target rectangle inside which the text is to be rendered;
* \i <size>
* the width and height of text to be aligned;
* \i <horAlign>
* the horizontal alignment of the text;
* \i <verAlign>
* the vertical alignments of the text;
* \ie
*
* RETURNS: the bounding rectangle to render a string using this font.
*
* ERRNO: N/A
*
* SEE ALSO: GetTextBound()
*/
    static TRect CalculateTextBound(const TRect & rect,
        const TSize & size,
        TextHorizontalAlignment horAlign,
        TextVerticalAlignment verAlign);

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
    struct FontData
        {
    public:
        FontData(TString _faceName, TRT_U32 _pointSize,
            TFont::FontType _type, TRT_HANDLE _handle) :
            faceName(_faceName),
            pointSize(_pointSize),
            type(_type),
            handle(_handle)
                {
                }
    public:
        TString         faceName;
        TRT_U32         pointSize;
        TFont::FontType type;
        TRT_HANDLE      handle;
        };

    TSharedPtr<const FontData>          data;
    };

class TImage;
class TTtfFont
    {
public:
    TTtfFont(const TString &);
    ~TTtfFont();
    TRT_BOOL operator == (const TTtfFont & ttf) const;
    TRT_BOOL IsNull() const;
    TRT_FLOAT GetHeight() const;
    void GetMetrics(FontMetrics &) const;
    TRT_STATUS GetGlyphImage(TRT_WCHAR, TImage &) const;
    TRT_STATUS GetGlyphAdvance(TRT_WCHAR, TPointF &) const;
    TRT_STATUS GetGlyphPath(TRT_WCHAR, TArray<TRT_BYTE> &, TArray<TRT_FLOAT> &, TPointF &) const;
private:
    TRT_HANDLE font;
    };

class TBmfFont
    {
public:
    struct Glyph
        {
        TRT_BYTE    width;
        TRT_BYTE    height;
        TRT_BYTE    ascent;
        TRT_BYTE    bits[1];
        };
public:
    TBmfFont(const TString &, TRT_S32);
    ~TBmfFont();
    TRT_BOOL operator == (const TBmfFont & bmf) const;
    TRT_BOOL IsNull() const;
    void GetMetrics(FontMetrics &) const;
    const Glyph * GetGlyph(TRT_WCHAR) const;
    TRT_STATUS GetGlyphAdvance(TRT_WCHAR, TPoint &) const;

private:
    TRT_HANDLE font;
    };

#endif /* _INCLUDE_tgsFont_H_ */


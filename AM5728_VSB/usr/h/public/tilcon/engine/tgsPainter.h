/* tgsPainter.h - Tilcon painter class */

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
28dec16,yya  fixed draw arc incorrectly (V7GFX-351)
14mar16,jnl  support window surface (F6570)
09jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines the Tilcon's painter class. TPainter is used to draw things
on paint devices.

INCLUDE FILES: tilcon/engine/tgsPainter.h
*/

#ifndef _INCLUDE_tgsPainter_H_
#define _INCLUDE_tgsPainter_H_

#include "tilcon/tilcon.h"
#include "tilcon/engine/tgsVectorPainter.h"
#include "tilcon/engine/tgsFont.h"
#include "tilcon/engine/tgsPath.h"
#include "tilcon/engine/tgsImage.h"

class TPainter;
class TWindow;
class TDisplay;
struct TextProperty;

/* pen class */

class TPen
    {
    friend class TPainter;

public:
    /* Line style */
    enum LineStyle
        {
        LS_empty = 0,       /* empty line */
        LS_solid,           /* solid line  */
        LS_dashed           /* dashed line */
        };

public:
/*******************************************************************************
*
* TPen - construct a default pen
*
* SYNOPSIS
* \ss
* TPen()
* \se
*
* DESCRIPTION
* This function constructs a TPen instance. The default values for class
* members are as below:
* color is set to black (ARGB = 0xff, 0x0, 0x0, 0x0);
* width is set to 1.0f;
* line style is set to solid line (LS_solid);
* joint style is set to bevel (JS_bevel);
* cap style is set to butt (CS_butt);
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPen() :
        color (TRT_COLOR_BLACK),
        width(1),
        style(LS_solid)
        {
        }

/*******************************************************************************
*
* TPen - construct a pen with specified color and width
*
* SYNOPSIS
* \ss
* TPen
*     (
*     const TColor & color, /@ color of the pen @/
*     TRT_S32 width         /@ width of the pen @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPen instance with specified color and line width.
* other members are the same to the value as the default constructor.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPen
        (
        const TColor & _color,
        TRT_S32 _width,
        LineStyle _style = LS_solid
        ) :
        color (_color),
        width(_width),
        style(_style)
        {
        }

/*******************************************************************************
*
* TPen - construct a pen by copying from another pen
*
* SYNOPSIS
* \ss
* TPen
*     (
*     const TPen & pen  /@ pen to copy @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPen instance by copying from another TPen
* instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPen
        (
        const TPen & pen
        ) :
        color (pen.color),
        width(pen.width),
        style(LS_solid)
        {
        }

/*******************************************************************************
*
* TPen - construct a pen with specified line style
*
* SYNOPSIS
* \ss
* TPen
*     (
*     LineStyle _style, /@ line style of the pen @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPen instance with specified line style.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    TPen
        (
        LineStyle _style
        ) :
        color (TRT_COLOR_BLACK),
        width(1),
        style(_style)
        {
        }

/*******************************************************************************
*
* ~TPen - destruct the pen
*
* This function destructs the TPen instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TPen()
        {
        }

/*******************************************************************************
*
* SetLineStyle - set the line style
*
* SYNOPSIS
* \ss
* void SetLineStyle
*     (
*     LineStyle value     /@ new line style to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the line style of the pen.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetLineStyle
        (
        LineStyle value
        )
        {
        style = value;
        }

/*******************************************************************************
*
* GetLineStyle - get the line style
*
* SYNOPSIS
* \ss
* LineStyle GetLineStyle() const
* \se
*
* DESCRIPTION
* This function gets the line style of the pen.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    LineStyle GetLineStyle() const
        {
        return style;
        }

/*******************************************************************************
*
* SetWidth - set the line width
*
* SYNOPSIS
* \ss
* void SetWidth
*     (
*     TRT_S32 value     /@ new width to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the line width of the pen.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetWidth()
*/
    void SetWidth
        (
        TRT_S32 value
        )
        {
        width = value;
        }

/*******************************************************************************
*
* GetWidth - get the line width
*
* SYNOPSIS
* \ss
* TRT_S32 GetWidth() const
* \se
*
* DESCRIPTION
* This function gets the line width of the pen.
*
* RETURNS: line width of the pen.
*
* ERRNO: N/A
*
* SEE ALSO: SetWidth()
*/
    TRT_S32 GetWidth() const
        {
        return width;
        }

/*******************************************************************************
*
* SetColor - set the color
*
* SYNOPSIS
* \ss
* void SetColor
*     (
*     const TColor & value  /@ new color to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the color of the pen.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetColor()
*/
    void SetColor
        (
        const TColor & value
        )
        {
        color  = value;
        }

/*******************************************************************************
*
* GetColor - get the color
*
* SYNOPSIS
* \ss
* const TColor & GetColor() const
* \se
*
* DESCRIPTION
* This function gets the color of the pen.
*
* RETURNS: color of the pen.
*
* ERRNO: N/A
*
* SEE ALSO: SetColor()
*/
    const TColor & GetColor() const
        {
        return color;
        }

/*******************************************************************************
*
* operator == - compare if the pen is equal with another pen
*
* SYNOPSIS
* \ss
* TRT_BOOL operator ==
*     (
*     const TPen & pen  /@ pen to compare @/
*     ) const
* \se
*
* DESCRIPTION
* This function compares the attributes of this pen with another pen.
*
* RETURNS: TRT_TURE if equal, TRT_FALSE if not equal
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_BOOL operator ==
        (
        const TPen & pen
        ) const
        {
        return color == pen.color &&
            width == pen.width;
        }

/*******************************************************************************
*
* operator != - compare if the pen is not equal with another pen
*
* SYNOPSIS
* \ss
* TRT_BOOL operator !=
*     (
*     const TPen & pen  /@ pen to compare @/
*     ) const
* \se
*
* DESCRIPTION
* This function compares the attributes of this pen with another pen.
*
* RETURNS: TRT_TURE if not equal, TRT_FALSE if equal
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_BOOL operator != (const TPen & pen) const
        {
        return !(*this == pen);
        }

private:
    TColor          color;
    TRT_S32         width;
    LineStyle       style;
    };

/* brush class */

class TBrush
    {
    friend class TPainter;
public:
    /* brush style */
    enum BrushStyle
        {
        BS_empty = 0,       /* empty brush */
        BS_solid,           /* solid color fill */
        BS_pattern          /* pattern image fill */
        };

public:
/*******************************************************************************
*
* TBrush - construct a default brush
*
* SYNOPSIS
* \ss
* TBrush()
* \se
*
* DESCRIPTION
* This function constructs a default empty TBrush instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TBrush() :
        style(TBrush::BS_solid),
        color (TRT_COLOR_BLACK)
        {
        }

/*******************************************************************************
*
* TBrush - construct a solid fill brush with specified solid color
*
* SYNOPSIS
* \ss
* TBrush
*     (
*     const TColor & color /@ color of the brush @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a solid fill TBrush instance, with the specified
* color as solid fill color.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TBrush
        (
        const TColor & _color
        ) :
        style(TBrush::BS_solid),
        color(_color)
        {
        }

/*******************************************************************************
*
* TBrush - construct a brush with specified style
*
* SYNOPSIS
* \ss
* TBrush
*     (
*     BrushStyle value /@ style of the brush @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TBrush instance, with the specified style.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TBrush
        (
        BrushStyle value
        ) :
        style(value),
        color (TRT_COLOR_BLACK)
        {
        }

/*******************************************************************************
*
* TBrush - construct a pattern brush with specified image
*
* SYNOPSIS
* \ss
* TBrush
*     (
*     const TPixelBuffer & image /@ pattern image @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a pattern fill TBrush instance, with the specified
* image as the pattern.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TBrush
        (
        const TPixelBuffer & image
        ) :
        style(BS_pattern),
        pattern(image)
        {
        }

/*******************************************************************************
*
* TBrush - construct a brush by copying from another brush
*
* SYNOPSIS
* \ss
* TBrush
*     (
*     const TBrush & brush /@ brush to copy @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TBrush instance as a copy of another TBrush
* instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TBrush
        (
        const TBrush & brush
        ) :
        style (brush.style),
        color (brush.color),
        pattern (brush.pattern)
        {
        }

/*******************************************************************************
*
* ~TBrush - destruct the brush
*
* This function destructs the TBrush instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TBrush()
        {
        }

/*******************************************************************************
*
* SetStyle - set the style of the brush
*
* SYNOPSIS
* \ss
* void SetColor
*     (
*     BrushStyle value /@ new brush style to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the style of the brush.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetStyle()
*/
    void SetStyle
        (
        BrushStyle value
        )
        {
        style = value;
        }

/*******************************************************************************
*
* GetStyle - get the style of the brush
*
* SYNOPSIS
* \ss
* BrushStyle GetStyle() const
* \se
*
* DESCRIPTION
* This function gets the style of the brush.
*
* RETURNS: style of the brush.
*
* ERRNO: N/A
*
* SEE ALSO: SetStyle()
*/
    BrushStyle GetStyle() const
        {
        return style;
        }

/*******************************************************************************
*
* SetColor - set the color of the brush
*
* SYNOPSIS
* \ss
* void SetColor
*     (
*     const TColor & value /@ new color to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the color of the brush. The specified color will take
* effect when the style of the brush is set to BS_solid.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetColor()
*/
    void SetColor
        (
        const TColor & value
        )
        {
        color = value;
        }

/*******************************************************************************
*
* GetColor - get the color of the brush
*
* SYNOPSIS
* \ss
* const TColor & GetColor() const
* \se
*
* DESCRIPTION
* This function gets the color of the brush.
*
* RETURNS: color of the brush.
*
* ERRNO: N/A
*
* SEE ALSO: SetColor()
*/
    const TColor & GetColor() const
        {
        return color;
        }

/*******************************************************************************
*
* SetPatternImage - set the pattern of the brush
*
* SYNOPSIS
* \ss
* void SetPatternImage
*     (
*     const TPixelBuffer & value /@ new pattern to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the pattern image of the brush. The specified pattern will
* take effect when the style of the brush is set to BS_pattern.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetPatternImage()
*/
    void SetPatternImage
        (
        const TPixelBuffer & value
        )
        {
        pattern = value;
        }

/*******************************************************************************
*
* GetPatternImage - get the pattern image of the brush
*
* SYNOPSIS
* \ss
* const TPixelBuffer & GetPatternImage() const
* \se
*
* DESCRIPTION
* This function gets the pattern image of the brush.
*
* RETURNS: pattern image.
*
* ERRNO: N/A
*
* SEE ALSO: SetPatternImage()
*/
    const TPixelBuffer & GetPatternImage() const
        {
        return pattern;
        }

private:
    BrushStyle          style;
    TColor              color;
    TPixelBuffer        pattern;
    };

/* Tilcon's painter class */

class TPainter
    {
    friend class TPen;
    friend class TBrush;
    friend class TFont;
    friend class TVectorPainter;

public:
/*******************************************************************************
*
* TPainter - construct a painter
*
* This function constructs a default painter.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPainter();

/*******************************************************************************
*
* ~TPainter - destruct the painter
*
* This function destructs the painter.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TPainter();

/*******************************************************************************
*
* BeginPaint - prepare the context for painting
*
* This function prepares the context for painting. This function shall be called
* before any rendering operation is carried out.
*
* SYNOPSIS
* \ss
* TRT_STATUS BeginPaint
*     (
*     TPaintDevice & device     /@ device to draw on @/
*     )
* \se
*
* DESCRIPTION
* Parameters:
* \is
* \i <device>
* the device onto which to paint;
* \ie
*
* RETURNS: TRT_STATUS_OK is the painting context is successfully prepared;
* TRT_STATUS_ERROR otherwise;
*
* ERRNO: N/A
*
* SEE ALSO: EndPaint()
*/
    TRT_STATUS BeginPaint
        (
        TPixelBuffer & device
        );

    TRT_STATUS BeginPaint
        (
        TWindow & window
        );

/*******************************************************************************
*
* EndPaint - free the painting context and end painting
*
* SYNOPSIS
* \ss
* EndPaint()
* \se
*
* DESCRIPTION
* This function frees the painting painter-> This function is called after all
* rendering operations are finished. If it is not called explicitly, the
* destructor of the TPainter will call it implicitly.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: BeginPaint()
*/
    void EndPaint();

/*******************************************************************************
*
* Reset - reset the state of the painter
*
* SYNOPSIS
* \ss
* void Reset()
* \se
*
* DESCRIPTION
* This function resets the state of the painter, include the viewport, clipping,
* pen, brush, font, transformation, fill rule and blending mode.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Reset();

/*******************************************************************************
*
* SetViewport - set the viewport of the painter
*
* SYNOPSIS
* \ss
* void SetViewport
*   (
*   const TPoint & origin = TPoint(),
*   const TRect & bound = TRect::maxRect
*   )
* \se
*
* DESCRIPTION
* This function sets the viewport of the painter. The coordination of all
* drawing function is relative to the viewport's origin, and is clipped by the
* viewport's bounding rectangle.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetViewport(const TPoint & origin = TPoint(),
        const TRect & bound = TRect::maxRect);

/*******************************************************************************
*
* SetClipRect - set the clip rectangle
*
* SYNOPSIS
* \ss
* void SetClipRect
*     (
*     const TRect & rect    /@ clip rectangle to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the clip rectangle; the following up painting output will
* be restricted within the clip rectangle. Clip rectangle can be removed by
* ClearClipRect.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ClearClipRect()
*/
    void SetClipRect
        (
        const TRect & clipRect
        );

/*******************************************************************************
*
* ClearClipRect - clear the clip rectangle
*
* SYNOPSIS
* \ss
* void ClearClipRect()
* \se
*
* DESCRIPTION
* This function removes the clip rectangle.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetClipRect()
*/
    void ClearClipRect();

/*******************************************************************************
*
* SetPen - set the pen used for primitive drawing
*
* SYNOPSIS
* \ss
* void SetPen
*     (
*     const TPen & pen     /@ new pen to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the pen for primitive drawing, which determines the width
* and color of lines drawn by the painter's primitives drawing interfaces.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: DrawLine()
*/
    void SetPen
        (
        const TPen & pen
        );

/*******************************************************************************
*
* SetBrush - set a new brush to be used for primitive drawing
*
* SYNOPSIS
* \ss
* void SetBrush
*     (
*     const TBrush & brush    /@ new brush to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets a new brush to be used for primitive drawing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ResetBrush()
*/
    void SetBrush
        (
        const TBrush & brush
        );

/*******************************************************************************
*
* SetFont - set a new font to be used by the painter
*
* SYNOPSIS
* \ss
* void SetFont
*     (
*     const TFont & font    /@ new font to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets a new font to be used by the painter.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ResetFont()
*/
    void SetFont
        (
        const TFont & font
        );

/*******************************************************************************
*
* SetTextColor - set the text color of the painter
*
* SYNOPSIS
* \ss
* void SetTextColor
*     (
*     const TColor & value    /@ new text color to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text color of the painter.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: DrawUCSText()
*/
    void SetTextColor
        (
        const TColor & value
        );

/*******************************************************************************
*
* SetRasterMode - set the raster operation of the painter
*
* SYNOPSIS
* \ss
* void SetRasterMode
*     (
*     RasterMode value    /@ new raster mode to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the raster mode of the painter.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void SetRasterMode
        (
        RasterMode value
        );

/*******************************************************************************
*
* ResetBrush - resset the brush
*
* SYNOPSIS
* \ss
* void ResetBrush()
* \se
*
* DESCRIPTION
* This function resets the brush.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void ResetBrush();

/*******************************************************************************
*
* ResetFont - reset the font
*
* SYNOPSIS
* \ss
* void ResetFont()
* \se
*
* DESCRIPTION
* This function resets the font of the painter.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetFont()
*/
    void ResetFont();

/*******************************************************************************
*
* DrawLine - draw a line between two points
*
* SYNOPSIS
* \ss
* void Line
*     (
*     const TPoint & p1,    /@ point 1 @/
*     const TPoint & p2     /@ point 2 @/
*     )
* \se
*
* DESCRIPTION
* This function draws a line between two points. The line's attribute is based
* on the currently selected TPen.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawLine
        (
        const TPoint & p1,
        const TPoint & p2
        );

/*******************************************************************************
*
* Line - draw a line between two points
*
* SYNOPSIS
* \ss
* void Line
*     (
*     TRT_S32 x1,       /@ x coordinate of point 1 @/
*     TRT_S32 y1,       /@ y coordinate of point 1 @/
*     TRT_S32 x2,       /@ x coordinate of point 2 @/
*     TRT_S32 y2        /@ y coordinate of point 2 @/
*     )
* \se
*
* DESCRIPTION
* This function draws a line between two points. The line's attribute is based
* on the currently selected TPen.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawLine
        (
        TRT_S32 x1,
        TRT_S32 y1,
        TRT_S32 x2,
        TRT_S32 y2
        )
        {
        DrawLine(TPoint(x1, y1), TPoint(x2, y2));
        }

/*******************************************************************************
*
* DrawCircle - draws a circle
*
* SYNOPSIS
* \ss
* void DrawCircle
*     (
*     const TRect & rect /@ specifies a rectangle that bounds the circle @/
*     )
* \se
*
* DESCRIPTION
* This routine draws a circle. The parameter <rect> specifies a rectangle
* that bounds the circle. The graphics context specifies the details of how the
* circle is to be drawn. The line's attribute is based on the currently
* selected TPen, and fill's attribute is based on the currently selected TBrush.
*
* If an outlined circle is not desired, the line width should be set to zero.
* If the line width is not zero the inner fill will be drawn to adjust for the
* line width whether or not it is transparent.  The circle is not filled
* if the brush style is BS_empty.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void DrawCircle
        (
        const TRect & rect
        );

/*******************************************************************************
*
* DrawEllipse - draws an ellipse
*
* SYNOPSIS
* \ss
* void DrawEllipse
*     (
*     TRT_S32 left,     /@ left of ellipse @/
*     TRT_S32 top,      /@ top of ellipse @/
*     TRT_S32 right,    /@ right of ellipse @/
*     TRT_S32 bottom    /@ bottom of ellipse @/
*     )
* \se
*
* DESCRIPTION
* This routine draws an ellipse. The parameters specifie a rectangle that bounds
* the ellipse. The graphics context specifies the details of how the ellipse is
* to be drawn. The line's attribute is based on the currently selected TPen, and
* fill's attribute is based on the currently selected TBrush.
*
* If an outlined ellipse is not desired, the line width should be set to zero.
* If the line width is not zero the inner fill will be drawn to adjust for the
* line width whether or not it is transparent. The ellipse is not filled
* if the brush style is BS_empty.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void DrawEllipse
        (
        TRT_S32 left,
        TRT_S32 top,
        TRT_S32 right,
        TRT_S32 bottom
        );

/*******************************************************************************
*
* DrawEllipse - draws an ellipse
*
* SYNOPSIS
* \ss
* void DrawEllipse
*     (
*     const TRect & rect /@ specifies a rectangle that bounds the ellipse @/
*     )
* \se
*
* DESCRIPTION
* This routine draws an ellipse. The parameter <rect> specifies a rectangle
* that bounds the ellipse. The graphics context specifies the details of how the
* ellipse is to be drawn. The line's attribute is based on the currently
* selected TPen, and fill's attribute is based on the currently selected TBrush.
*
* If an outlined ellipse is not desired, the line width should be set to zero.
* If the line width is not zero the inner fill will be drawn to adjust for the
* line width whether or not it is transparent. The ellipse is not filled
* if the brush style is BS_empty.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawEllipse
        (
        const TRect & rect
        );

/*******************************************************************************
*
* DrawArc - draws an arc
*
* SYNOPSIS
* \ss
* void DrawArc
*     (
*     const TRect  & rect, /@ specifies a rectangle that bounds the arc @/
*     const TPoint & start,/@ the starting angle for an arc @/
*     const TPoint & end   /@ the ending angle for an arc @/
*     )
* \se
*
* DESCRIPTION
* This routine draws an arc.
* The parameter <rect> specifies a rectangle that bounds the arc.
* The parameter <start> is the starting angle for an arc.
* The parameter <end> is the ending angle for an arc.
*
* The arc proceeds clockwise in coordinate systems with
* positive Y going up and positive X going right, but counter-clockwise on
* the screen (positive Y going down).
*
* The graphics context specifies the details of how the arc is
* to be drawn. The line's attribute is based on the currently selected TPen.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawArc
        (
        const TRect  & rect,
        const TPoint & start,
        const TPoint & end
        );

/*******************************************************************************
*
* DrawArc - draws a pie slice
*
* SYNOPSIS
* \ss
* void DrawPie
*     (
*     const TRect  & rect, /@ specifies a rectangle that bounds the pie @/
*     const TPoint & start,/@ the starting angle for a pie @/
*     const TPoint & end   /@ the ending angle for a pie @/
*     )
* \se
*
* DESCRIPTION
* This routine draws a pie slice.
* The parameter <rect> specifies a rectangle that bounds the pie. A radial
* line draw from the center of the ellipse through the point defined by
* parameter <start> is the starting angle for a pie slice. A
* radial line draw from the center of the ellipse through the point
* defined by parameter <end> is the ending angle for a pie slice.
*
* The pie proceeds clockwise in coordinate systems with
* positive Y going up and positive X going right, but counter-clockwise on
* the screen (positive Y going down).
*
* The graphics context specifies the details of how the pie slice is
* to be drawn. The line's attribute is based on the currently
* selected TPen, and fill's attribute is based on the currently selected TBrush.
* The pie slice is not filled if the brush style is BS_empty.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawPie
        (
        const TRect  & rect,
        const TPoint & start,
        const TPoint & end
        );

/*******************************************************************************
*
* DrawRect - draw a rectangle
*
* SYNOPSIS
* \ss
* void DrawRect
*     (
*     const TRect & rect    /@ rectangle to draw @/
*     )
* \se
*
* DESCRIPTION
* This function draws a rectangle using the selected pen and fills the rectangle
* using the selected brush if the brush is not empty.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void DrawRect
        (
        const TRect & rect
        );

/*******************************************************************************
*
* DrawRect - draw a rectangle
*
* SYNOPSIS
* \ss
* void DrawRect
*     (
*     TRT_S32 left,     /@ left of rectangle @/
*     TRT_S32 top,      /@ top of rectangle @/
*     TRT_S32 right,    /@ right of rectangle @/
*     TRT_S32 bottom    /@ bottom of rectangle @/
*     )
* \se
*
* DESCRIPTION
* This function draws a rectangle using the selected pen and fills the rectangle
* using the selected brush if the brush is not empty.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void DrawRect
        (
        TRT_S32 left,
        TRT_S32 top,
        TRT_S32 right,
        TRT_S32 bottom
        );

/*******************************************************************************
*
* FillRect - fill a rectangle using the selected brush
*
* SYNOPSIS
* \ss
* void FillRect
*     (
*     const TRect & rect    /@ rectangle to fill @/
*     )
* \se
*
* DESCRIPTION
* This function fills a rectangle using the selected brush.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void FillRect
        (
        const TRect & rect
        );

/*******************************************************************************
*
* FillRect - fill a rectangle using the selected brush
*
* SYNOPSIS
* \ss
* void FillRect
*     (
*     TRT_S32 left,     /@ left of rectangle @/
*     TRT_S32 top,      /@ top of rectangle @/
*     TRT_S32 right,    /@ right of rectangle @/
*     TRT_S32 bottom    /@ bottom of rectangle @/
*     )
* \se
*
* DESCRIPTION
* This function fills a rectangle using the selected brush.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/

    void FillRect
        (
        TRT_S32 left,
        TRT_S32 top,
        TRT_S32 right,
        TRT_S32 bottom
        );

/*******************************************************************************
*
* DrawUCSText - draw text
*
* SYNOPSIS
* \ss
* void DrawUCSText
*     (
*     const TWString & string,      /@ string to be drawn @/
*     TRT_S32 x,                    /@ x position @/
*     TRT_S32 y                     /@ y position @/
*     )
* \se
*
* DESCRIPTION
* This function draws text at specified coordinates.
*
* Parameters:
* \is
* \i <string>
* the string to be drawn;
* \i <x>, <y>
* the upper left position to draw the text
* \ie
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawUCSText
        (
        const TWString & str,
        TRT_S32 x,
        TRT_S32 y
        );

/*******************************************************************************
*
* DrawUCSText - draw text
*
* SYNOPSIS
* \ss
* void DrawUCSText
*     (
*     TRT_LPCWSTR text,             /@ address of the C-style string @/
*     TRT_S32 len,                  /@ length of the C-style string @/
*     TRT_S32 x,                    /@ x position @/
*     TRT_S32 y                     /@ y position @/
*     )
* \se
*
* DESCRIPTION
* This function draws text at specified coordinates.
*
* Parameters:
* \is
* \i <text>
* the start address of the C-style string;
* \i <len>
* the length of the C-style string;
* \i <x>, <y>
* the upper left position to draw the text
* \ie
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawUCSText
        (
        TRT_LPCWSTR text,
        TRT_S32 len,
        TRT_S32 x,
        TRT_S32 y
        );

/*******************************************************************************
*
* DrawUCSText - draw text into rectangle with alignments
*
* SYNOPSIS
* \ss
* void DrawUCSText
*     (
*     const TWString &          str,        /@ text to be drawn @/
*     const TRect &             rect,       /@ destination rectangle @/
*     TextHorizontalAlignment   horAlign,   /@ horizontal alignment @/
*     TextVerticalAlignment     verAlign    /@ vertical alignment @/
*     )
* \se
*
* DESCRIPTION
* This function draws text at specified rectangle and alignments.
*
* Parameters:
* \is
* \i <str>
* the string to be drawn;
* \i <rect>
* the destination rectangle to place the text into;
* \i <horAlign>
* the horizontal alignment;
* \i <verAlign>
* the vertical alignment;
* \ie
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawUCSText
        (
        const TWString & str,
        const TRect & rect,
        TextHorizontalAlignment horAlign,
        TextVerticalAlignment verAlign
        );

/*******************************************************************************
*
* DrawUCSText - draw text into rectangle with alignments
*
* SYNOPSIS
* \ss
* void DrawUCSText
*     (
*     TRT_LPCWSTR               text,       /@ wide char buffer @/
*     TRT_S32                   len,        /@ wide char buffer length @/
*     const TRect &             rect,       /@ destination rectangle @/
*     TextHorizontalAlignment   horAlign,   /@ horizontal alignment @/
*     TextVerticalAlignment     verAlign    /@ vertical alignment @/
*     )
* \se
*
* DESCRIPTION
* This function draws text at specified rectangle and alignments.
*
* Parameters:
* \is
* \i <text>
* the wide char buffer containing the text to be drawn;
* \i <len>
* the length of the wide char buffer;
* \i <rect>
* the destination rectangle to place the text into;
* \i <horAlign>
* the horizontal alignment;
* \i <verAlign>
* the vertical alignment;
* \ie
*
* RETURNS: N/A;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawUCSText
        (
        TRT_LPCWSTR text,
        TRT_S32 len,
        const TRect & rect,
        TextHorizontalAlignment horAlign,
        TextVerticalAlignment verAlign
        );

/*******************************************************************************
*
* DrawImage - draw image
*
* SYNOPSIS
* \ss
* void DrawImage
*     (
*     const TImage & image, /@ image to be drawn @/
*     TRT_S32 x,            /@ target x coordinate @/
*     TRT_S32 y             /@ target y coordinate @/
*     )
* \se
*
* DESCRIPTION
* This function draws an image at the specified position, with its original
* size.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawImage
        (
        const TImage & image,
        TRT_S32 x,
        TRT_S32 y
        );

/*******************************************************************************
*
* DrawPixelBuffer - draw pixel buffer
*
* SYNOPSIS
* \ss
* void DrawPixelBuffer
*     (
*     const TPixelBuffer & pbuffer, /@ pixel buffer to be drawn @/
*     TRT_S32 x,                    /@ target x coordinate @/
*     TRT_S32 y                     /@ target y coordinate @/
*     )
* \se
*
* DESCRIPTION
* This function draws a pixel buffer at the specified position, with its
* original size.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DrawPixelBuffer
        (
        const TPixelBuffer & pbuffer,
        TRT_S32 x,
        TRT_S32 y
        );

/*******************************************************************************
*
* CopyToPixelBuffer - copy the content of the paint target to a pixel buffer
*
* SYNOPSIS
* \ss
* void CopyToPixelBuffer
*     (
*     TPixelBuffer & pbuffer    /@ pixel buffer to copy to @/
*     TRT_S32 pbufferX,         /@ x coordinate of the pixel buffer @/
*     TRT_S32 pbufferY,         /@ y coordinate of the pixel buffer @/
*     TRT_S32 surfaceX,         /@ x coordinate of the copy source @/
*     TRT_S32 surfaceY,         /@ y coordinate of the copy source @/
*     TRT_S32 w,                /@ width of the copy source @/
*     TRT_S32 h,                /@ height of the copy source @/
*     )
* \se
*
* DESCRIPTION
* This function copies the content of the paint target, specified by <x>, <y>,
* <w> and <h> to a pixel buffer. The copying area is specified in the paint
* target's coordinate, and is not affected by transformation and clipping.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: CopyFromPixelBuffer()
*/
    void CopyToPixelBuffer(TPixelBuffer & pbuffer,
        TRT_S32 pbufferX, TRT_S32 pbufferY, TRT_S32 surfaceX, TRT_S32 surfaceY,
        TRT_S32 w, TRT_S32 h);

/*******************************************************************************
*
* CopyFromPixelBuffer - copy pixel buffer to the paint target
*
* SYNOPSIS
* \ss
* void CopyFromPixelBuffer
*     (
*     TPixelBuffer & pbuffer    /@ pixel buffer to copy from @/
*     TRT_S32 pbufferX,         /@ x coordinate of the pixel buffer @/
*     TRT_S32 pbufferY,         /@ y coordinate of the pixel buffer @/
*     TRT_S32 surfaceX,         /@ x coordinate of the copy destination @/
*     TRT_S32 surfaceY,         /@ y coordinate of the copy destination @/
*     TRT_S32 w,                /@ width of the copy destination @/
*     TRT_S32 h,                /@ height of the copy destination @/
*     )
* \se
*
* DESCRIPTION
* This function copies the pixel buffer to an area in the paint target,
* specified by <x>, <y>, <w> and <h> to a pixel buffer. The copying area is
* specified in the paint target's coordinate, and is not affected by
* transformation and clipping.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: CopyToPixelBuffer()
*/
    void CopyFromPixelBuffer(const TPixelBuffer & pbuffer,
        TRT_S32 pbufferX, TRT_S32 pbufferY, TRT_S32 surfaceX, TRT_S32 surfaceY,
        TRT_S32 w, TRT_S32 h);


/*******************************************************************************
*
* VG - return the Vector Graphics interface of the painter
*
* SYNOPSIS
* \ss
* TVectorPainter & VG()
* \se
*
* DESCRIPTION
* This function return the Vector Graphics interface of the painter.
*
* RETURNS: TVectorPainter interface
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TVectorPainter & VG()
        {
        return vectorPainter;
        }

    TDisplay &          engine;
    TRT_HANDLE          context;
    TVectorPainter      vectorPainter;
    TBrush              defaultBrush;
    TFont               defaultFont;
    };

#endif /* _INCLUDE_tgsPainter_H_ */


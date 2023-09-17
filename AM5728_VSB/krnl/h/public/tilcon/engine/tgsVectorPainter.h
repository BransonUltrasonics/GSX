/* tgsVectorPainter.h - Tilcon vector paint class */

/*
 * Copyright (c) 2015-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
14mar16,jnl  support window surface (F6570)
04aug15,yya  created. (V7GFX-247)
*/

/*
DESCRIPTION

This file defines the Tilcon vector painter class.

TGradient - helper class to define gradient paint;
TBrush - the paint of Tilcon's painter;

INCLUDE FILES: tilcon/engine/tgsBrush.h
*/

#ifndef _INICLUDE_tgsVectorPainter_H_
#define _INICLUDE_tgsVectorPainter_H_

#include "tilcon/engine/tgsPath.h"
#include "tilcon/engine/tgsImage.h"
#include "tilcon/engine/tgsPixelBuffer.h"

struct LinearGradientData;
struct RadialGradientData;
struct ColorStop;
struct ColorStopData;


/* color stop */

struct ColorStop
    {
    ColorStop() : u(0.0f)
        {}
    ColorStop(TRT_FLOAT _u, const TColor & _c) :
        u(_u), color(_c)
        {}
    TRT_FLOAT   u;
    TColor      color;
    };

/* gradient */

class TGradient
    {
public:
    /* gradient type */
    enum GradientType
        {
        GT_linear = 0,
        GT_radial,
        };

    /* gradient spread mode */
    enum SpreadMode
        {
        SM_pad = 0,
        SM_repeat,
        SM_reflect,
        };

public:
/*******************************************************************************
*
* TGradient - construct a gradient
*
* This function constructs a default gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGradient();

/*******************************************************************************
*
* TGradient - construct a gradient with specific type
*
* This function constructs a gradient of specific type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGradient
        (
        GradientType type
        );

/*******************************************************************************
*
* TGradient - construct a linear gradient
*
* This function constructs a linear gradient with parameters.
*
* <x0>, <y0>, <x1>, <y1> are parameters to define the linear gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGradient
        (
        TRT_FLOAT x0,
        TRT_FLOAT y0,
        TRT_FLOAT x1,
        TRT_FLOAT y1,
        TRT_U32 n,
        const TColor * colors,
        const TRT_FLOAT * stops
        );

/*******************************************************************************
*
* TGradient - construct a radial gradient
*
* This function constructs a radial gradient with parameter.
*
* <cx>, <cy>, <fx>, <fy>, <r> are parameters to define the radial gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGradient
        (
        TRT_FLOAT cx,
        TRT_FLOAT cy,
        TRT_FLOAT fx,
        TRT_FLOAT fy,
        TRT_FLOAT r,
        TRT_U32 n,
        const TColor * colors,
        const TRT_FLOAT * stops
        );

/*******************************************************************************
*
* ~TGradient - destruct the gradient
*
* This function destructs the gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TGradient();

/*******************************************************************************
*
* GetType - get the type of the gradient
*
* This function gets the type of the gradient.
*
* RETURNS: Type of the gradient
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    GradientType GetType() const
        {
        return type;
        }

/*******************************************************************************
*
* SetLinearData - set parameter for linear gradient
*
* This function changes parameters for linear gradient. This function only
* takes effect for gradient that is of linear gradient type.
*
* <x0>, <y0>, <x1>, <y1> are parameters to define the linear gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetLinearData()
*/
    void SetLinearData
        (
        TRT_FLOAT x0,
        TRT_FLOAT y0,
        TRT_FLOAT x1,
        TRT_FLOAT y1
        );

/*******************************************************************************
*
* GetLinearData - get linear gradient data of the gradient
*
* This function gets linear gradient data of the gradient.
*
* RETURNS: the linear gradient data of the gradient
*
* ERRNO: N/A
*
* SEE ALSO: SetLinearData()
*/
    const LinearGradientData & GetLinearData() const;

/*******************************************************************************
*
* SetRadialData - set parameter for radial gradient
*
* This function changes parameters for radial gradient. This function only
* takes effect for gradient that is of radial gradient type.
*
* <cx>, <cy>, <fx>, <fy>, <r> are parameters to define the radial gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRadialData()
*/
    void SetRadialData
        (
        TRT_FLOAT cx,
        TRT_FLOAT cy,
        TRT_FLOAT fx,
        TRT_FLOAT fy,
        TRT_FLOAT r
        );

/*******************************************************************************
*
* GetRadialData - get radial gradient data of the gradient
*
* This function gets radial gradient data of the gradient.
*
* RETURNS: the radial gradient data of the gradient
*
* ERRNO: N/A
*
* SEE ALSO: SetRadialData()
*/
    const RadialGradientData & GetRadialData() const;

/*******************************************************************************
*
* SetSpreadMode - set the spread mode of the gradient
*
* This function sets the spread mode of the gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetSpreadMode()
*/
    void SetSpreadMode
        (
        SpreadMode value
        )
        {
        spread = value;
        }

/*******************************************************************************
*
* GetSpreadMode - get the spread mode of the gradient
*
* This function gets the spread mode of the gradient.
*
* RETURNS: the spread mode of the gradient
*
* ERRNO: N/A
*
* SEE ALSO: SetSpreadMode()
*/
    SpreadMode GetSpreadMode() const
        {
        return spread;
        }

/*******************************************************************************
*
* ClearColorStops - clear all color stop data of the gradient
*
* This function clears all color stop data of the gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddColorStop()
*/
    void ClearColorStops();

/*******************************************************************************
*
* SetColorStop - set color stop data of the gradient
*
* This function sets color stop data of the gradient.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetColorStop()
*/
    void SetColorStop
        (
        const ColorStopData &
        );

/*******************************************************************************
*
* GetColorStop - get color stop data of the gradient
*
* This function gets color stop data of the gradient.
*
* RETURNS: the color stop data of the gradient
*
* ERRNO: N/A
*
* SEE ALSO: SetColorStop()
*/
    const ColorStopData & GetColorStop() const;

private:
    friend class TApplication;

    static TSharedPtr<LinearGradientData> defaultLinearGradientData;
    static TSharedPtr<RadialGradientData> defaultRadialGradientData;
    static TSharedPtr<ColorStopData>      defaultColorStopData;

    GradientType                                type;
    SpreadMode                                  spread;
    TSharedPtr<LinearGradientData>        linearData;
    TSharedPtr<RadialGradientData>        radialData;
    TSharedPtr<ColorStopData>             colorStopData;
    };

struct LinearGradientData
    {
    LinearGradientData();
    LinearGradientData(TRT_FLOAT, TRT_FLOAT, TRT_FLOAT, TRT_FLOAT);

    TRT_FLOAT           x0, y0;
    TRT_FLOAT           x1, y1;
    TRT_FLOAT           dx, dy;
    TRT_FLOAT           q;
    };

struct RadialGradientData
    {
    RadialGradientData();
    RadialGradientData(TRT_FLOAT, TRT_FLOAT, TRT_FLOAT, TRT_FLOAT, TRT_FLOAT);
    TRT_FLOAT           cx, cy;
    TRT_FLOAT           fx, fy;
    TRT_FLOAT           r;
    };

struct ColorStopData
    {
    ColorStopData()
        {}
    ColorStopData(const TArray<ColorStop> & s) :
        stops(s)
        {}
    ColorStopData(TRT_U32, const TColor *, const TRT_FLOAT *);
    TArray<ColorStop>   stops;
    };

/* paint class in Vector Graphics */

class TPaint
    {
    friend class TVectorPainter;
public:
    /* paint style */
    enum PaintStyle
        {
        PS_color = 0,       /* color paint */
        PS_gradient,        /* gradient paint */
        PS_pattern,         /* pattern paint */
        };

public:
/*******************************************************************************
*
* TPaint - construct a default paint
*
* SYNOPSIS
* \ss
* TPaint()
* \se
*
* DESCRIPTION
* This function constructs a default TPaint instance, with color paint type,
* and color is opaque white (ARGB = 0xfffffff).
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPaint() :
        style(PS_color),
        color(TRT_COLOR_WHITE)
        {
        }

/*******************************************************************************
*
* TPaint - construct a solid color paint with specified color
*
* SYNOPSIS
* \ss
* TPaint
*     (
*     const TColor & color /@ color of the paint @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a color TPaint instance with the specified color.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPaint
        (
        const TColor & _color
        ) :
        style(PS_color),
        color(_color)
        {
        }

/*******************************************************************************
*
* TPaint - construct a gradient paint with specified gradient
*
* SYNOPSIS
* \ss
* TPaint
*     (
*     const TGradient & gradient /@ gradient of the paint @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a gradient TPaint instance, with the specified
* gradient.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPaint
        (
        const TGradient & _gradient
        ) :
        style(PS_gradient),
        gradient(_gradient)
        {
        }

/*******************************************************************************
*
* TPaint - construct a pattern paint with specified image
*
* SYNOPSIS
* \ss
* TPaint
*     (
*     const TPixelBuffer & image /@ pattern image @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a pattern TPaint instance, with the specified image.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPaint
        (
        const TPixelBuffer & image
        ) :
        style(PS_pattern),
        pattern(image)
        {
        }

/*******************************************************************************
*
* TPaint - construct a paint by copying from another paint
*
* SYNOPSIS
* \ss
* TPaint
*     (
*     const TPaint & paint /@ paint to copy @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPaint instance as a copy of another TPaint
* instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPaint
        (
        const TPaint & paint
        ) :
        style (paint.style),
        color (paint.color),
        gradient (paint.gradient),
        pattern (paint.pattern)
        {
        }

/*******************************************************************************
*
* ~TPaint - destruct the paint
*
* This function destructs the TPaint instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TPaint()
        {
        }

/*******************************************************************************
*
* SetStyle - set the style of the paint
*
* SYNOPSIS
* \ss
* void SetColor
*     (
*     BrushStyle value /@ new paint style to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the style of the paint.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetStyle()
*/
    void SetStyle
        (
        PaintStyle value
        )
        {
        style = value;
        }

/*******************************************************************************
*
* GetStyle - get the style of the paint
*
* SYNOPSIS
* \ss
* BrushStyle GetStyle() const
* \se
*
* DESCRIPTION
* This function gets the style of the paint.
*
* RETURNS: style of the paint.
*
* ERRNO: N/A
*
* SEE ALSO: SetStyle()
*/
    PaintStyle GetStyle() const
        {
        return style;
        }

/*******************************************************************************
*
* SetColor - set the color of the paint
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
* This function sets the color of the paint. The specified color will take
* effect when the style of the paint is set to BS_solid.
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
* GetColor - get the color of the paint
*
* SYNOPSIS
* \ss
* const TColor & GetColor() const
* \se
*
* DESCRIPTION
* This function gets the color of the paint.
*
* RETURNS: color of the paint.
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
* SetGradient - set the gradient of the paint
*
* SYNOPSIS
* \ss
* void SetGradient
*     (
*     const TGradient & value /@ new gradient to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the gradient of the paint. The specified gradient will
* take effect when the style of the paint is set to BS_gradient.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetGradient()
*/
    void SetGradient(const TGradient & value)
        {
        gradient = value;
        }

/*******************************************************************************
*
* GetGradient - get the gradient of the paint
*
* SYNOPSIS
* \ss
* const TGradient & GetGradient() const
* \se
*
* DESCRIPTION
* This function gets the gradient of the paint.
*
* RETURNS: gradient of the paint.
*
* ERRNO: N/A
*
* SEE ALSO: SetGradient()
*/
    const TGradient & GetGradient() const
        {
        return gradient;
        }

/*******************************************************************************
*
* SetPatternImage - set the pattern image of the paint
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
* This function sets the pattern image of the paint. The specified pattern will
* take effect when the style of the paint is set to BS_pattern.
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
        pattern = TPixelBuffer(value);
        }

/*******************************************************************************
*
* GetPatternImage - get the pattern image of the paint
*
* SYNOPSIS
* \ss
* const TPixelBuffer & GetPatternImage() const
* \se
*
* DESCRIPTION
* This function gets the pattern of the paint.
*
* RETURNS: pattern image of the paint.
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
    PaintStyle          style;
    TColor              color;
    TGradient           gradient;
    TPixelBuffer        pattern;
    };

/* vector paint class */

class TPainter;
class TVectorPainter
    {
    friend class TPainter;
private:
    TVectorPainter(TPainter * p) :
        painter(*p)
        {}
public:
/*******************************************************************************
*
* SetDashStyle - set the line style
*
* SYNOPSIS
* \ss
* void SetDashStyle
*     (
*     DashStyle value  /@ new line style to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the line style of the stroked path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetDashStyle
        (
        DashStyle value
        );

/*******************************************************************************
*
* SetJointStyle - set the joint style
*
* SYNOPSIS
* \ss
* void SetJointStyle
*     (
*     JointStyle value  /@ new joint style to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the joint style of the stroked path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetJointStyle
        (
        JointStyle value
        );

/*******************************************************************************
*
* SetCapStyle - set the cap style
*
* SYNOPSIS
* \ss
* void SetCapStyle
*     (
*     CapStyle value  /@ new cap style to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the cap style of the stroked path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetCapStyle
        (
        CapStyle value
        );

/*******************************************************************************
*
* SetFillPaint - set the paint used in filling path
*
* SYNOPSIS
* \ss
* void SetFillPaint
*     (
*     const TPaint & paint  /@ fill paint @/
*     )
* \se
*
* DESCRIPTION
* This function sets the paint used in filling path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: SetStrokePaint()
*/
    void SetFillPaint(const TPaint &);

/*******************************************************************************
*
* SetStrokePaint - set the paint used in stroking path
*
* SYNOPSIS
* \ss
* void SetStrokePaint
*     (
*     const TPaint & paint  /@ stroke paint @/
*     )
* \se
*
* DESCRIPTION
* This function sets the paint used in stroking path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: SetFillPaint()
*/
    void SetStrokePaint(const TPaint &);

/*******************************************************************************
*
* SetPathWidth - set the width of the stroked path
*
* SYNOPSIS
* \ss
* void SetPathWidth
*     (
*     TRT_FLOAT value     /@ new width to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the width of the stroked path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetPathWidth()
*/
    void SetPathWidth
        (
        TRT_FLOAT value
        );

/*******************************************************************************
*
* TranslateMatrix - translate the vector graphics coordinates system
*
* SYNOPSIS
* \ss
* void TranslateMatrix
*     (
*     TRT_FLOAT x,  /@ translate applied to x axis @/
*     TRT_FLOAT y   /@ translate applied to y axis @/
*     )
* \se
*
* DESCRIPTION
* This function Translates the vector graphics coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ScaleMatrix(), RotateMatrix(), SetPathMartrix(),
* ResetMatrix()
*/
    void TranslateMatrix
        (
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* ScaleMatrix - Scale the vector graphics coordinates system
*
* SYNOPSIS
* \ss
* void ScaleMatrix
*     (
*     TRT_FLOAT sx, /@ scale applied to x axis @/
*     TRT_FLOAT sy  /@ scale applied to y axis @/
*     )
* \se
*
* DESCRIPTION
* This function scales the vector graphics coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslateMatrix(), RotateMatrix(), SetPathMartrix(),
* ResetMatrix()
*/
    void ScaleMatrix
        (
        TRT_FLOAT sx,
        TRT_FLOAT sy
        );

/*******************************************************************************
*
* RotateMatrix - rotate the vector graphics coordinates system
*
* SYNOPSIS
* \ss
* void RotateMatrix
*     (
*     TRT_FLOAT degree /@ degree to rotate @/
*     )
* \se
*
* DESCRIPTION
* This function rotates the vector graphics coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslateMatrix(), ScaleMatrix(), SetPathMartrix(),
* ResetMatrix()
*/
    void RotateMatrix
        (
        TRT_FLOAT degree
        );

/*******************************************************************************
*
* SetMatrix - set the transform of the vector graphics coordinates system
*
* SYNOPSIS
* \ss
* void SetMatrix
*     (
*     const TMatrix3x3 & matrix /@ new matrix to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the transform of vector graphics coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslateMatrix(), ScaleMatrix(), RotateMatrix(),
* ResetMatrix()
*/
    void SetMatrix
        (
        const TMatrix3x3 & matrix
        );

/*******************************************************************************
*
* ResetMatrix - reset the vector graphics coordinates system
*
* SYNOPSIS
* \ss
* void ResetMatrix()
* \se
*
* DESCRIPTION
* This function sets the transform of vector graphics coordinates system to the
* identity matrix;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslateMatrix(), ScaleMatrix(), RotateMatrix(),
* SetMatrix()
*/
    void ResetMatrix();

/*******************************************************************************
*
* TranslatePaintMatrix - translate the paint's coordinates system
*
* SYNOPSIS
* \ss
* void TranslatePaintMatrix
*     (
*     TRT_FLOAT x,  /@ translate applied to x axis @/
*     TRT_FLOAT y   /@ translate applied to y axis @/
*     )
* \se
*
* DESCRIPTION
* This function Translates the paint's coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ScalePaintMatrix(), RotatePaintMatrix(), SetBrushMartrix(),
* ResetPaintMatrix()
*/
    void TranslatePaintMatrix
        (
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* ScalePaintMatrix - scale the paint's drawing coordinates system
*
* SYNOPSIS
* \ss
* void ScalePaintMatrix
*     (
*     TRT_FLOAT sx, /@ scale applied to x axis @/
*     TRT_FLOAT sy  /@ scale applied to y axis @/
*     )
* \se
*
* DESCRIPTION
* This function scales the paint's coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslatePaintMatrix(), RotatePaintMatrix(), SetBrushMartrix(),
* ResetPaintMatrix()
*/
    void ScalePaintMatrix
        (
        TRT_FLOAT sx,
        TRT_FLOAT sy
        );

/*******************************************************************************
*
* RotatePaintMatrix - rotate the paint's coordinates system
*
* SYNOPSIS
* \ss
* void RotatePaintMatrix
*     (
*     TRT_FLOAT degree /@ degree to rotate @/
*     )
* \se
*
* DESCRIPTION
* This function rotates the paint's coordinates system
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslatePaintMatrix(), ScalePaintMatrix(), SetBrushMartrix(),
* ResetPaintMatrix()
*/
    void RotatePaintMatrix
        (
        TRT_FLOAT degree
        );

/*******************************************************************************
*
* SetPaintMatrix - set the transform matrix for the paint of the painter
*
* SYNOPSIS
* \ss
* void SetPaintMatrix
*     (
*     const TMatrix3x3 & matrix /@ new matrix to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the transform matrix for the paint of the painter;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslatePaintMatrix(), ScalePaintMatrix(), RotatePaintMatrix(),
* ResetPaintMatrix()
*/
    void SetPaintMatrix
        (
        const TMatrix3x3 & matrix
        );

/*******************************************************************************
*
* ResetPaintMatrix - reset the transform paint of the painter paint
*
* SYNOPSIS
* \ss
* void ResetPaintMatrix()
* \se
*
* DESCRIPTION
* This function resets the transform matrix of the painter paint to the
* identity matrix;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: TranslatePaintMatrix(), ScalePaintMatrix(), RotatePaintMatrix(),
* SetPaintMatrix()
*/
    void ResetPaintMatrix();

/*******************************************************************************
*
* SetFillRule - set the fill rule of path drawing
*
* SYNOPSIS
* \ss
* void SetFillRule
*     (
*     FillRule value    /@ new fill rule to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the fill rule of path drawing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetFillRule
        (
        FillRule value
        );

/*******************************************************************************
*
* SetBlendMode - set the blend mode of path drawing
*
* SYNOPSIS
* \ss
* void SetBlendMode
*     (
*     BlendMode value    /@ new blend mode to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the blend mode of path drawing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetBlendMode (BlendMode value);

/*******************************************************************************
*
* FillPath - fill the interior of the specified path
*
* SYNOPSIS
* \ss
* void FillPath
*     (
*     const TPath & path,   /@ path to be drawn @/
*     )
* \se
*
* DESCRIPTION
* This function fill the interior of the specified path. If the specified path
* is not closed, it is implicitly closed before filling the interior. The fill's
* attribute is based on the currently selected TBrush.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: StrokePath()
*/
    void FillPath (const TPath & path);

/*******************************************************************************
*
* FillPath - fill the interior enclosed by the specified lines
*
* SYNOPSIS
* \ss
* void FillPath
*     (
*     const TPointF * pts,  /@ points to defines lines @/
*     TRT_U32 count         /@ number of points specified @/
*     )
* \se
*
* DESCRIPTION
* This function fill the interior enclosed by the specified lines.
* The fill's attribute is based on the currently selected TBrush.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: StrokePath()
*/
    void FillPath (const TPointF * pts, TRT_U32 count);

/*******************************************************************************
*
* StrokePath - stroke the specified path
*
* SYNOPSIS
* \ss
* void StrokePath
*     (
*     const TPath & path,   /@ path to be drawn @/
*     )
* \se
*
* DESCRIPTION
* This function strokes the specified path, i.e., draw a line defined by the
* path. The line's attribute is based on the currently selected TPen.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: FillPath()
*/
    void StrokePath (const TPath & path);

/*******************************************************************************
*
* StrokePath - draw consecutive lines defined by array of points
*
* SYNOPSIS
* \ss
* void StrokePath
*     (
*     const TPointF * pts,          /@ points defining the lines @/
*     TRT_U32 num,                  /@ number of points @/
*     TRT_BOOL closed               /@ whether the line is closed or not @/
*     )
* \se
*
* DESCRIPTION
* This function draws draw consecutive lines defined by array of points.
* The line's attribute is based on the currently selected TPen.
*
* Parameters:
* \is
* \i <pts>
* the points defining the lines;
* \i <num>
* the number of points in <pts> array;
* \i <closed>
* whether the stroked line is closed or not;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void StrokePath
        (
        const TPointF * pts,
        TRT_U32 num,
        TRT_BOOL closed
        );

private:
    TPainter &      painter;
    TMatrix3x3      pathMatrix;
    TMatrix3x3      paintMatrix;
    };

#endif /* _INICLUDE_tgsVectorPainter_H_ */


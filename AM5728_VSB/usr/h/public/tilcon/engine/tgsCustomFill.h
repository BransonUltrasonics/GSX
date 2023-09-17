/* tgsCustomFill.h - Tilcon custom fill class */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's custom fill object, which is used to apply
customized appearance to UI controls.

INCLUDE FILES: tilcon/engine/tgsCustomFill.h
*/

#ifndef _INCLUDE_tgsCustomFill_H_
#define _INCLUDE_tgsCustomFill_H_

#include "tilcon/engine/tgsPainter.h"
#include "tilcon/engine/tgsImage.h"
#include "tilcon/engine/tgsPixelBuffer.h"

class TEmptyFill;
class TSolidFill;
class TPatternFill;
class TImageFill;
class TPainter;

/* base class of custom fill */
class TCustomFill
    {
    friend class TSolidFill;
    friend class TImageFill;
public:
    class TCustomFillAdaptor
        {
        };
public:

    TCustomFill();
    TCustomFill(const TCustomFill & fill);
    TCustomFill(const TEmptyFill & fill);
    TCustomFill(const TSolidFill & fill);
    TCustomFill(const TPatternFill & fill);
    TCustomFill(const TImageFill & fill);
    ~TCustomFill();

    TRT_BOOL IsDefualtVisual() const;
    TCustomFill & operator = (const TCustomFill &);
    TCustomFill & operator = (const TEmptyFill &);
    TCustomFill & operator = (const TSolidFill &);
    TCustomFill & operator = (const TImageFill &);
    TCustomFill & operator = (const TPatternFill &);

    void FillRect(TPainter & painter, const TRect & rect) const;

private:
    enum CustomFillType
        {
        CF_default = 0,
        CF_empty,
        CF_static,
        CF_pattern,
        CF_image
        };

    void DeleteAdaptor();
    void CopyAdaptor(CustomFillType, const TCustomFillAdaptor *);

    CustomFillType          type;
    TCustomFillAdaptor *    adaptor;
    };

class TEmptyFill : public TCustomFill::TCustomFillAdaptor
    {
    friend class TCustomFill;
public:

/*******************************************************************************
*
* TEmptyFill - construct an empty fill
*
* SYNOPSIS
* \ss
* TEmptyFill()
* \se
*
* DESCRIPTION
* This function constructs a TEmptyFill instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEmptyFill();

/*******************************************************************************
*
* ~TEmptyFill - destruct the empty fill
*
* This function destructs the TEmptyFill instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TEmptyFill();
    };

class TSolidFill : public TCustomFill::TCustomFillAdaptor
    {
    friend class TCustomFill;
public:

/*******************************************************************************
*
* TSolidFill - construct a solid fill
*
* SYNOPSIS
* \ss
* TSolidFill
*     (
*     const TColor & color  /@ color of the fill @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TSolidFill instance with specified color as the
* solid fill color.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TSolidFill
        (
        const TColor & color
        );

/*******************************************************************************
*
* ~TSolidFill - destruct the solid fill
*
* This function destructs the TSolidFill instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TSolidFill();

    void FillRect(TPainter & painter, const TRect & rect) const;

private:
    TCustomFillAdaptor * dup() const;

private:
    const TColor color;
    };

class TPatternFill : public TCustomFill::TCustomFillAdaptor
    {
    friend class TCustomFill;
public:

/*******************************************************************************
*
* TPatternFill - construct a pattern fill
*
* SYNOPSIS
* \ss
* TPatternFill
*     (
*     const TPixelBuffer & pattern  /@ pattern to fill @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a pattern fill.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPatternFill
        (
        const TPixelBuffer & image
        );

/*******************************************************************************
*
* ~TPatternFill - destruct the pattern fill
*
* This function destructs the pattern fill.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TPatternFill();

    void FillRect(TPainter & painter, const TRect & rect) const;

private:
    TCustomFillAdaptor * dup() const;

    const TPixelBuffer  pattern;
    };

class TImageFill : public TCustomFill::TCustomFillAdaptor
    {
    friend class TCustomFill;
public:
/*******************************************************************************
*
* TImageFill - construct an image fill
*
* SYNOPSIS
* \ss
* TImageFill
*     (
*     const TPixelBuffer & image,     /@ image content @/
*     )
* \se
*
* DESCRIPTION
* This constructor creates a TImageFill instance with a specified image. The
* TImageFill class will have a copy of the specified image, so the specified
* image can be destroyed after the caller to this constructor.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TImageFill
        (
        const TPixelBuffer & image
        );

/*******************************************************************************
*
* TImageFill - construct an image fill by copying from another image fill
*
* SYNOPSIS
* \ss
* TImageFill
*     (
*     const TImageFill & image,     /@ image fill to copy from @/
*     )
* \se
*
* DESCRIPTION
* This constructor creates a TImageFill instance by copy from another image
* fill instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TImageFill(const TImageFill &);

/*******************************************************************************
*
* ~TImageFill - destruct the image fill
*
* This function destructs the image fill.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TImageFill();

    void FillRect(TPainter & painter, const TRect & rect) const;

private:
    TCustomFillAdaptor * dup() const;
    TPixelBuffer    buffer;
    };

#endif /* _INCLUDE_tgsCustomFill_H_ */


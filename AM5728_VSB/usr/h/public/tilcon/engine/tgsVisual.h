/* tgsVisual.h - Tilcon visual class */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's TVisual class. TVisual represents the visible
elements among Tilcon's UI. One Tilcon control may contains one or several
visible elements, for example, a button has only one visible element which is
its foreground area, while a check box has four visible elements: in its checked
state, there are one visible element for its indicator and one for the entire
background area, and there are other two for the unchecked state.

The TVisual class has a default type, which is visible element type that the
TVisual class is going to represent, for example, the check box indicator's
TVisual is of type FT_check_indicator_check. The type of TVisual is designed to
be used by the TVisualEngine class to render different type of visible elements.

User can overwrite the way to draw the TVisual object by attaching the
TCustomVisual data to it. If there is a TCustomVisual attached, the TVisual
object draws the element accordingly to TCustomVisual, and will not use the
TVisualEngine to draw the element in the default way.

TCustomVisual contains seven of TCustomFill objects as data member, each of them
is for the appearance of one visual state. To apply custom visual, user shall
first create a TCustomVisual object, then change its TCustomFill members as
needed, and finally call TVisual's SetCustomVisual method to attach the custom
visual to the TVisual.

The seven TCustomFill objects are corresponding to visual state as following.

normal:                 node is in normal state;
focused:                node has the input focus;
dimmed:                 node is disabled;
highlighted:            node is under mouse pointer;
active:                 node is active(i.e., being pressed);
highlighted with focus: focused node is under mouse pointer,
active with focus:      focused node is active(i.e., being pressed);

INCLUDE FILES: tilcon/engine/tgsVisual.h
*/

#ifndef _INCLUDE_tgsFrameVisual_H_
#define _INCLUDE_tgsFrameVisual_H_

#include "tilcon/engine/tgsCustomFill.h"

class TPainter;

/* data structure to hold custom fills */
struct TCustomVisual
    {
public:
    TCustomVisual()
        {
        }

    TCustomVisual(const TCustomFill & fill) :
        normalFill(fill),
        highlightedFill(fill),
        activeFill(fill),
        focusedFill(fill),
        focusedHighlightedFill(fill),
        focusedActiveFill(fill),
        dimmedFill(fill)
        {
        }

    TCustomVisual(const TCustomFill & n, const TCustomFill & h, const TCustomFill & a,
        const TCustomFill & f, const TCustomFill & fh,const TCustomFill & fa,
        const TCustomFill & d) :
        normalFill(n),
        highlightedFill(h),
        activeFill(a),
        focusedFill(f),
        focusedHighlightedFill(fh),
        focusedActiveFill(fa),
        dimmedFill(d)
        {
        }

    TCustomFill normalFill;             /* normal */
    TCustomFill highlightedFill;        /* normal highlight */
    TCustomFill activeFill;             /* normal active */
    TCustomFill focusedFill;            /* focused */
    TCustomFill focusedHighlightedFill; /* focused highlight */
    TCustomFill focusedActiveFill;      /* focused active */
    TCustomFill dimmedFill;             /* dimmed */
    };

class TVisual
    {
public:

/*******************************************************************************
*
* TVisual - construct a visual instance
*
* SYNOPSIS
* \ss
* TVisual()
*     (
*     FrameType type = FT_none /@ frame type of the visual @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TVisual, with a specified frame type
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TVisual
        (
        FrameType type = FT_none
        );

/*******************************************************************************
*
* TVisual - destruct the visual instance
*
* This function destructs the TVisual
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TVisual();

/*******************************************************************************
*
* Draw - draw the TVisual
*
* SYNOPSIS
* \ss
* void Draw
*     (
*     TPainter & painter,           /@ painter used to draw @/
*     const VisualState & state,    /@ visual state of the visual @/
*     const TRect & rect            /@ bounding rectangle of the visual @/
*     ) const
* \se
*
* DESCRIPTION
* This function draws the TVisual.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Draw
        (
        TPainter & painter,
        const VisualState & state,
        const TRect & rect
        ) const;

/*******************************************************************************
*
* SetCustomVisual - set the custom visual
*
* SYNOPSIS
* \ss
* void SetCustomVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function set the custom visual to the TVisual instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: GetCustomVisual()
*/
    void SetCustomVisual
        (
        const TCustomVisual & value
        )
        {
        customVisual = value;
        }

/*******************************************************************************
*
* GetCustomVisual - get the custom visual
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetCustomVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual attached to the TVisual instance.
*
* RETURNS: the custom visual attached to the TVisual instance.
*
* ERRNO: N/A
*
* SEE ALSO: SetCustomVisual()
*/
    const TCustomVisual & GetCustomVisual() const
        {
        return customVisual;
        }

/*******************************************************************************
*
* SetFrameType - set the type of the visual
*
* SYNOPSIS
* \ss
* void SetFrameType
*     (
*     FrameType value /@ new frame type @/
*     )
* \se
*
* DESCRIPTION
* This function sets the type of the TVisual.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetFrameType()
*/
    void SetFrameType(FrameType value)
        {
        frameType = value;
        }

/*******************************************************************************
*
* GetFrameType - get the type of the visual
*
* SYNOPSIS
* \ss
* FrameType GetFrameType() const
* \se
*
* DESCRIPTION
* This function returns the type of the TVisual.
*
* RETURNS: type of the visual.
*
* ERRNO: N/A
*
* SEE ALSO: SetFrameType()
*/
    FrameType GetFrameType() const
        {
        return frameType;
        }

private:
    TRT_BOOL DrawCustom(TPainter & painter, const VisualState & state, const TRect & rect) const;

    FrameType       frameType;
    TCustomVisual   customVisual;
    };

#endif /* _INCLUDE_tgsFrameVisual_H_ */


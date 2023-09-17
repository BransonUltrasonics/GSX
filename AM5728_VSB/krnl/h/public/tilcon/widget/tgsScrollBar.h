/* tgsScrollBar.h - Tilcon scroll bar widget */

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

This file defines Tilcon's scroll bar widget.

The scroll bar is usually used to scroll an area under a view port; The scroll
area are defined by two values, the pages size and the total size. The page size
stands for the amount that can be shown inside the view port; the total size is
the amount of the whole content that is to be shown. For example, there are 100
lines of text to be shown, and each screen can shown 20 lines, then the page
size is 20 and total size is 100; the scrollbar can be scroll 80 steps from the
top of the text content to the bottom. Scroll bar's page and total size can be
set at construction time, or by the SetScrollPos() and SetScrollData() methods.

When the scrollbar is scrolled, either by dragging the thumb or clicking the
arrow buttons, it generates the Scroll Event. User can use the ScrollTo() and
ScrollBy() method to simulate the scrolling and generate Scroll Event.

The Scroll bar has four customizable visual parts, the track, the thumb, and the
two button at each end. Use SetTrackVisual(), SetThumbVisual(),
SetNegativeArrowVisual() and SetPositiveArrowVisual() to customize them.

INCLUDE FILES: tilcon/widget/tgsScrollBar.h
*/

#ifndef _INCLUDE_tgsScrollBar_H_
#define _INCLUDE_tgsScrollBar_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/widget/tgsPushButton.h"
#include "tilcon/widget/tgsScrollBarNode.h"

class TScrollBar : public TControl
    {
    friend class TScrollArea;
public:
    /* scroll event parameter */
    struct ScrollEventParam : public ControlEventParam
        {
        TRT_S32 pos;
        TRT_S32 delta;
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TRT_S32             trackWidth;
        TRT_S32             buttonWidth;

        TCustomVisual       trackVisual;
        TCustomVisual       thumbVisual;
        TCustomVisual       negativeArrowVisual;
        TCustomVisual       positiveArrowVisual;
        };
public:
/*******************************************************************************
*
* TScrollBar - construct a scroll bar
*
* SYNOPSIS
* \ss
* TScrollBar
*     (
*     TRT_S32 x = 0,                                /@ x position @/
*     TRT_S32 y = 0,                                /@ y position @/
*     TRT_S32 w = 0,                                /@ width @/
*     TRT_S32 h = 0,                                /@ height @/
*     OrientationType orientation = OT_horizontal,  /@ orientation @/
*     TRT_S32 page = 10,                            /@ page size @/
*     TRT_S32 total = 100,                          /@ total size @/
*     TRT_S32 position = 0                          /@ initial position @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a scroll bar widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of scroll bar widget;
* \i <orientation>
* horizontal scroll bar or vertical scroll bar;
* \i <page>, <total>, <position>
* the page size, total size and initial position of the scroll bar;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TScrollBar(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0,
        OrientationType _orientation = OT_horizontal,
        TRT_S32 _page = 10, TRT_S32 _total = 100, TRT_S32 _pos = 0);

/*******************************************************************************
*
* ~TScrollBar - destructs the scroll bar
*
* This function destructs the scroll bar widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TScrollBar();

/*******************************************************************************
*
* ScrollEvent - get the Scroll Event of the scroll bar
*
* SYNOPSIS
* \ss
* TEvent & CheckEvent()
* \se
*
* DESCRIPTION
* This function returns the reference to the Scroll Event. The Scroll Event is
* emitted whenever the scroll bar is scrolled, via dragging the scroll thumb or
* clicking the arrow buttons at each end of the scroll bar. The event parameter
* is of type ScrollEventParam, which contains the scroll's new position and the
* increment/decrement value to previous position:
*
* class ScrollEventParam : public EventParam
*     {
* public:
*     TRT_S32 pos;      /@ new scroll position @/
*     TRT_S32 delta;    /@ increment/decrement to previous position @/
*     };
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Scroll Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & ScrollEvent()
        {
        return scrollEvent;
        }

/*******************************************************************************
*
* SetProperty - change the property of scroll bar
*
* SYNOPSIS
* \ss
* void SetProperty
*     (
*     const Property & property /@ new property to apply @/
*     )
* \se
*
* DESCRIPTION
* This function changes the property of scroll bar. This function is used to
* change multiple property items in one call. Frequently used property can be
* individually changed by dedicated APIs, e.g., label content can be changed by
* SetText(), and label font can be changed by SetFont().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetProperty(const Property &);

/*******************************************************************************
*
* GetProperty - get the property of scroll bar
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of scroll bar.
*
* RETURNS: the Property structure representing the scroll bar's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SetOrientation - set the orientation of the scroll bar
*
* SYNOPSIS
* \ss
* void SetOrientation
*     (
*     OrientationType value /@ new orientation @/
*     )
* \se
*
* DESCRIPTION
* This function sets the orientation of the scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetOrientation()
*/
    void SetOrientation(OrientationType value);

/*******************************************************************************
*
* GetOrientation - get the orientation of the scroll bar
*
* SYNOPSIS
* \ss
* OrientationType GetOrientation() const
* \se
*
* DESCRIPTION
* This function gets the orientation of the scroll bar.
*
* RETURNS: the orientation of the scroll bar
*
* ERRNO: N/A
*
* SEE ALSO: SetOrientation()
*/
    OrientationType GetOrientation() const
        {
        return boundNode.GetOrientation();
        }

/*******************************************************************************
*
* SetStepSize - set the step size of scroll bar
*
* SYNOPSIS
* \ss
* void SetStepSize
*     (
*     TRT_S32 value /@ new step size @/
*     )
* \se
*
* DESCRIPTION
* This function set the steps size of scroll bar. Step size is the increment or
* decrement applied when using arrow buttons to move scroll bar;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetStepSize()
*/
    void SetStepSize(TRT_S32 value)
        {
        boundNode.SetStepSize(value);
        }

/*******************************************************************************
*
* GetStepSize - get the step size of scroll bar
*
* SYNOPSIS
* \ss
* TRT_S32 GetStepSize() const
* \se
*
* DESCRIPTION
* This function gets the steps size of scroll bar.
*
* RETURNS: the step size of scroll bar
*
* ERRNO: N/A
*
* SEE ALSO: SetStepSize()
*/
    TRT_S32 GetStepSize() const
        {
        return boundNode.GetStepSize();
        }

/*******************************************************************************
*
* SetScrollPos - set the scroll position of the scroll bar
*
* SYNOPSIS
* \ss
* SetScrollPos
*   (
*   TRT_S32 pos     /@ new scroll position @/
*   )
* \se
*
* DESCRIPTION
* This function sets the scroll position of the scroll bar.
* This function does not generate the Scroll Event. Use ScrollTo() and
* ScrollBy() to move the scroll with Scroll Event generation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetScrollPos(), ScrollTo(), ScrollBy()
*/
    void SetScrollPos(TRT_S32 value)
        {
        boundNode.SetScrollPos(value);
        }

/*******************************************************************************
*
* GetScrollPos - get the scroll position of the scroll bar
*
* SYNOPSIS
* \ss
* TRT_S32 GetScrollPos() const
* \se
*
* DESCRIPTION
* This function gets the scroll position of the scroll bar.
*
* RETURNS: the scroll position of the scroll bar
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollPos()
*/
    TRT_S32 GetScrollPos() const
        {
        return boundNode.GetScrollPos();
        }

/*******************************************************************************
*
* SetScrollData - set the scroll page size, total size and position
*
* SYNOPSIS
* \ss
* SetScrollData
*   (
*   TRT_S32 page,   /@ page size @/
*   TRT_S32 total,  /@ total size @/
*   TRT_S32 pos     /@ scroll position @/
*   )
* \se
*
* DESCRIPTION
* This function sets the page size, total size and scroll position of the scroll
* bar. This function to does not generate the Scroll Event. Use ScrollTo() and
* ScrollBy() to move the scroll with Scroll Event generation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollPos(), ScrollTo(),ScrollBy()
*/
    void SetScrollData(TRT_S32 page, TRT_S32 total, TRT_S32 pos)
        {
        boundNode.SetScrollData(page, total, pos);
        }

/*******************************************************************************
*
* GetScrollPageSize - get the scroll page size of the scroll bar
*
* SYNOPSIS
* \ss
* TRT_S32 GetScrollPageSize() const
* \se
*
* DESCRIPTION
* This function gets the scroll page size of the scroll bar.
*
* RETURNS: the scroll page size of the scroll bar
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollData()
*/
    TRT_S32 GetScrollPageSize() const
        {
        return boundNode.GetScrollPageSize();
        }

/*******************************************************************************
*
* GetScrollTotalSize - get the scroll total size of the scroll bar
*
* SYNOPSIS
* \ss
* TRT_S32 GetScrollTotalSize() const
* \se
*
* DESCRIPTION
* This function gets the scroll total size of the scroll bar.
*
* RETURNS: the scroll total size of the scroll bar
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollData()
*/
    TRT_S32 GetScrollTotalSize() const
        {
        return boundNode.GetScrollTotalSize();
        }

/*******************************************************************************
*
* ScrollTo - scroll the scroll bar to new position
*
* SYNOPSIS
* \ss
* void ScrollTo
*     (
*     TRT_S32 position /@ position to scroll to @/
*     )
* \se
*
* DESCRIPTION
* This function scrolls the scroll bar to new position. The specified position
* will be clamped so that it falls within valid range according to the scroll
* page and scroll total size.
* If the scrolled position is different than the current position, the Scroll
* Event is generated.
* Use SetScrollPos() and SetScrollData() to change the scroll's position without
* generating the Scroll Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollPos(), SetScrollData(), ScrollBy()
*/
    void ScrollTo(TRT_S32 pos)
        {
        boundNode.ScrollTo(pos);
        }

/*******************************************************************************
*
* ScrollBy - scroll the scroll bar by the specified amount
*
* SYNOPSIS
* \ss
* void ScrollBy
*     (
*     TRT_S32 offset /@ amount to scroll by @/
*     )
* \se
*
* DESCRIPTION
* This function scrolls the scroll bar by the specified amount. The scroll bar
* will not be scrolled to position outside the valid range according to the
* scroll page size and scroll total size.
* If the scrolled position is different than the current position, the Scroll
* Event is generated.
* Use SetScrollPos() and SetScrollData() to change the scroll's position without
* generating the Scroll Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetScrollPos(), SetScrollData(), ScrollTo()
*/
    void ScrollBy(TRT_S32 offset)
        {
        boundNode.ScrollBy(offset);
        }

/*******************************************************************************
*
* SetTrackVisual - set the custom visual for the track area of the scroll bar
*
* SYNOPSIS
* \ss
* void SetTrackVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the track area of the
* scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTrackVisual()
*/
    void SetTrackVisual(const TCustomVisual & value)
        {
        trackVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetTrackVisual - get the custom visual for the track area of the scroll bar
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetTrackVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the track area of the
* scroll bar.
*
* RETURNS: reference to TCustomVisual object for the track area
*
* ERRNO: N/A
*
* SEE ALSO: SetTrackVisual()
*/
    const TCustomVisual & GetTrackVisual() const
        {
        return trackVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetThumbVisual - set the custom visual for the thumb area of the scroll bar
*
* SYNOPSIS
* \ss
* void SetThumbVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the thumb area of the
* scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetThumbVisual()
*/
    void SetThumbVisual(const TCustomVisual & value)
        {
        thumbVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetThumbVisual - get the custom visual for the thumb area of the scroll bar
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetThumbVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the thumb area of the
* scroll bar.
*
* RETURNS: reference to TCustomVisual object for the thumb area
*
* ERRNO: N/A
*
* SEE ALSO: SetThumbVisual()
*/
    const TCustomVisual & GetThumbVisual() const
        {
        return thumbVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetPositiveArrowVisual - set the custom visual for the positive button
*
* SYNOPSIS
* \ss
* void SetPositiveArrowVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the positive arrow button
* (down / right button) of the scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetPositiveArrowVisual()
*/
void SetPositiveArrowVisual(const TCustomVisual & value)
    {
    positiveArrowVisual.SetCustomVisual(value);
    Update();
    }

/*******************************************************************************
*
* GetPositiveArrowVisual - get the custom visual for the positive button
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetPositiveArrowVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the positive arrow button
* (down / right button) of the scroll bar.
*
* RETURNS: reference to TCustomVisual object for positive button
*
* ERRNO: N/A
*
* SEE ALSO: SetPositiveArrowVisual()
*/
    const TCustomVisual & GetPositiveArrowVisual() const
        {
        return positiveArrowVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetNegativeArrowVisual - set the custom visual for the negative button
*
* SYNOPSIS
* \ss
* void SetNegativeArrowVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the negative arrow button
* (up / left button) of the scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetNegativeArrowVisual()
*/
void SetNegativeArrowVisual(const TCustomVisual & value)
    {
    negativeArrowVisual.SetCustomVisual(value);
    Update();
    }

/*******************************************************************************
*
* GetNegativeArrowVisual - get the custom visual for the negative button
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetPositiveArrowVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the negative arrow button
* (up / left button) of the scroll bar.
*
* RETURNS: reference to TCustomVisual object for negative button
*
* ERRNO: N/A
*
* SEE ALSO: SetNegativeArrowVisual()
*/
    const TCustomVisual & GetNegativeArrowVisual() const
        {
        return negativeArrowVisual.GetCustomVisual();
        }

private:

    void ScrollHandler(EventParam &);

    TEvent              scrollEvent;

    TVisual             trackVisual;
    TVisual             thumbVisual;
    TVisual             negativeArrowVisual;
    TVisual             positiveArrowVisual;

    TScrollBarNode      boundNode;
    };

#endif /* _INCLUDE_tgsScrollBar_H_ */


/* tgsSlider.h - Tilcon slider widget */

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

This file defines Tilcon's slider widget.

The Slider is used to represents a value range; when the user slides the thumb
on the slider, it generates Slide Event. The value range of the slider is set
at construction time, and change be changed by the SetRange() method.

The Slider has three customizable visual parts, the track, the thumb, and the
background area. Use SetThumbVisual() and SetTrackVisual() methods to change
the thumb and track's appearance; the background's appearance is changed by the
TControl base class's SetFrameVisual() method.

INCLUDE FILES: tilcon/widget/tgsSlider.h
*/

#ifndef _INCLUDE_tgsSlider_H_
#define _INCLUDE_tgsSlider_H_

#include "tilcon/engine/tgsControl.h"

class TSlider : public TControl
    {
public:
    /* slider event parameter */
    struct SlideEventParam : public ControlEventParam
        {
        TRT_S32 pt1;
        TRT_S32 pt2;
        TRT_S32 pos;
        };
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TRT_S32             trackWidth;
        TRT_S32             thumbWidth;
        TRT_S32             thumbHeight;
        TRT_S32             thumbOffset;

        TCustomVisual       trackVisual;
        TCustomVisual       thumbVisual;
        TCustomVisual       frameVisual;
        };

public:
/*******************************************************************************
*
* TSlider - construct a slider widget
*
* SYNOPSIS
* \ss
* TSlider
*     (
*     TRT_S32 x = 0,                                /@ x position @/
*     TRT_S32 y = 0,                                /@ y position @/
*     TRT_S32 w = 0,                                /@ width @/
*     TRT_S32 h = 0,                                /@ height @/
*     OrientationType orientation = OT_horizontal,  /@ orientation @/
*     TRT_S32 pt1 = 0,                              /@ start value @/
*     TRT_S32 pt2 = 100,                            /@ end value @/
*     TRT_S32 position = 0                          /@ initial position @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a slider widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the slider widget;
* \i <orientation>
* horizontal or vertical slider;
* \i <pt1>, <pt2>, <position>
* the start value, end value and initial position of the slider;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TSlider(TRT_S32 x = 0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0,
        OrientationType _dir = OT_horizontal,
        TRT_S32 pt1 = 0, TRT_S32 pt2 = 100, TRT_S32 pos = 0);

/*******************************************************************************
*
* ~TSlider - destructs the slider widget
*
* This function destructs the slider widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TSlider();

/*******************************************************************************
*
* SlideEvent - get the Slide Event of the slide
*
* SYNOPSIS
* \ss
* TEvent & SlideEvent()
* \se
*
* DESCRIPTION
* This function returns the reference to the Slide Event. The Slide Event is
* emitted whenever the Slider is moved, via dragging the slider thumb. The event
* parameter is of type SlideEventParam, which contains the slider's range and
* current position:
*
* class SlideEventParam : public ControlEventParam
*     {
* public:
*     TRT_S32 pt1;  /@ start value, corresponding to thumb at left/top @/
*     TRT_S32 pt2;  /@ end value, corresponding to thumb at right/bottom @/
*     TRT_S32 pos;  /@ current value @/
*     };
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Slide Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & SlideEvent()
        {
        return slideEvent;
        }

/*******************************************************************************
*
* SetProperty - change the property of slider
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
* This function changes the property of slider. This function is used to
* change multiple property items in one call. Frequently used property can be
* individually changed by dedicated APIs, e.g., text content can be changed by
* SetText(), and font can be changed by SetFont().
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
* GetProperty - get the property of slider
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of slider.
*
* RETURNS: the Property structure representing the slider's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SetOrientation - set the orientation of the slider
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
* This function sets the orientation of the slider.
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
* GetOrientation - get the orientation of the slider
*
* SYNOPSIS
* \ss
* OrientationType GetOrientation() const
* \se
*
* DESCRIPTION
* This function gets the orientation of the slider.
*
* RETURNS: the orientation of the slider
*
* ERRNO: N/A
*
* SEE ALSO: SetOrientation()
*/
    OrientationType GetOrientation() const
        {
        return orientation;
        }

/*******************************************************************************
*
* SetRange - set the slider's range
*
* SYNOPSIS
* \ss
* SetRange
*   (
*   TRT_S32 pt1,    /@ start value of the range @/
*   TRT_S32 pt2     /@ end value of the range @/
*   )
* \se
*
* DESCRIPTION
* This function sets the range of the slider widget. After the range is changed,
* the slider will adjust the position to place it within the valid range.
* However, change of the position here will not incur the Slider Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRange()
*/
    void SetRange(TRT_S32 pt1, TRT_S32 pt2);

/*******************************************************************************
*
* GetRange - get the slider's range
*
* SYNOPSIS
* \ss
* const TLinear & GetRange() const
* \se
*
* DESCRIPTION
* This function gets the range of the slider widget.
*
* RETURNS: the range of the slider
*
* ERRNO: N/A
*
* SEE ALSO: SetRange()
*/
    const TLinear & GetRange() const
        {
        return scrollRange;
        }

/*******************************************************************************
*
* SetValue - set the slider's value
*
* SYNOPSIS
* \ss
* SetValue
*   (
*   TRT_S32 pos     /@ new slider value @/
*   )
* \se
*
* DESCRIPTION
* This function sets the value of the slider widget.
* This function does not generate the Slider Event. Use SlideTo() and SlideBy()
* to move the slider with Slide Event generation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetValue(), SlideTo(),SlideBy()
*/
    void SetValue(TRT_S32 pos);

/*******************************************************************************
*
* GetValue - get the slider's value
*
* SYNOPSIS
* \ss
* TRT_S32 GetValue() const
* \se
*
* DESCRIPTION
* This function gets the value of the slider widget.
*
* RETURNS: the position of the slider
*
* ERRNO: N/A
*
* SEE ALSO: SetValue()
*/
    TRT_S32 GetValue() const
        {
        return scrollPos;
        }

/*******************************************************************************
*
* SlideTo - move the slider to new position
*
* SYNOPSIS
* \ss
* void SlideTo
*     (
*     TRT_S32 position /@ position to slide to @/
*     )
* \se
*
* DESCRIPTION
* This function slides the slider to new position. The specified position
* will be clamped so that it falls within valid range of the slider.
* If the new position is different than the current position, the Slide Event
* is generated. Use SetValue() to change the slider's position without
* generating the Slide Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetValue(), SlideBy()
*/
    void SlideTo(TRT_S32);

/*******************************************************************************
*
* SlideBy - move the slider by the specified amount
*
* SYNOPSIS
* \ss
* void SlideBy
*     (
*     TRT_S32 position /@ amount to slide by @/
*     )
* \se
*
* DESCRIPTION
* This function slides the slider by the specified amount. The target position
* will be clamped so that it falls within valid range of the slider.
* If the new position is different than the current position, the Slide Event
* is generated. Use SetValue() to change the slider's position without
* generating the Slide Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetValue(), SlideTo()
*/
    void SlideBy(TRT_S32);

/*******************************************************************************
*
* SetTrackWidth - set the slider track's width
*
* SYNOPSIS
* \ss
* SetTrackWidth
*   (
*   TRT_S32 value     /@ new track width @/
*   )
* \se
*
* DESCRIPTION
* This function sets the slider track's width.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTrackWidth()
*/
    void SetTrackWidth(TRT_S32 value)
        {
        if (trackWidth != value)
            {
            trackWidth = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetTrackWidth - get the slider track's width
*
* SYNOPSIS
* \ss
* TRT_S32 GetTrackWidth() const
* \se
*
* DESCRIPTION
* This function gets the slider track's width.
*
* RETURNS: the slider track's width
*
* ERRNO: N/A
*
* SEE ALSO: SetTrackWidth()
*/
    TRT_S32 GetTrackWidth() const
        {
        return trackWidth;
        }

/*******************************************************************************
*
* SetThumbSize - set the slider thumb's size
*
* SYNOPSIS
* \ss
* SetThumbSize
*   (
*   const TSize & value     /@ new thumb size @/
*   )
* \se
*
* DESCRIPTION
* This function sets the slider thumb's size.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetThumbSize()
*/
    void SetThumbSize(const TSize & value)
        {
        if (thumbSize != value)
            {
            thumbSize = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetThumbSize - get the slider thumb's size
*
* SYNOPSIS
* \ss
* const TSize & GetThumbSize() const
* \se
*
* DESCRIPTION
* This function gets the slider thumb's size.
*
* RETURNS: the slider thumb's size
*
* ERRNO: N/A
*
* SEE ALSO: SetThumbSize()
*/
    const TSize & GetThumbSize() const
        {
        return thumbSize;
        }

/*******************************************************************************
*
* SetThumbOffset - set the slider thumb's offset
*
* SYNOPSIS
* \ss
* SetThumbOffset
*   (
*   TRT_S32 value     /@ new thumb offset @/
*   )
* \se
*
* DESCRIPTION
* This function sets the slider thumb's offset.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetThumbOffset()
*/
    void SetThumbOffset(TRT_S32 value)
        {
        if (thumbOffset != value)
            {
            thumbOffset = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetThumbOffset - get the slider thumb's offset
*
* SYNOPSIS
* \ss
* TRT_S32 GetThumbOffset() const
* \se
*
* DESCRIPTION
* This function gets the slider thumb's offset.
*
* RETURNS: the slider thumb's offset
*
* ERRNO: N/A
*
* SEE ALSO: SetThumbOffset()
*/
    TRT_S32 GetThumbOffset() const
        {
        return thumbOffset;
        }

/*******************************************************************************
*
* SetTrackVisual - set the custom visual for the slider track
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
* This function sets the custom visual used to show the slider track.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTrackVisual()
*/
    void SetTrackVisual(const TCustomVisual & visual)
        {
        trackVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetTrackVisual - get the custom visual for the slider track
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetTrackVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the slider track.
*
* RETURNS: reference to TCustomVisual object for the slider track
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
* SetThumbVisual - set the custom visual for the slider thumb
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
* This function sets the custom visual used to show the slider thumb.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetThumbVisual()
*/
    void SetThumbVisual(const TCustomVisual & visual)
        {
        thumbVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetThumbVisual - get the custom visual for the slider thumb
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetThumbVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the slider thumb.
*
* RETURNS: reference to TCustomVisual object for the slider thumb
*
* ERRNO: N/A
*
* SEE ALSO: SetThumbVisual()
*/
    const TCustomVisual & GetThumbVisual() const
        {
        return thumbVisual.GetCustomVisual();
        }

private:
    void NodeHandler(EventParam &);
    void ResizeHandler(EventParam &);
    void ThumbMouseHandler(EventParam &);
    void TrackPaintHandler(EventParam &);
    void ThumbPaintHandler(EventParam &);
    void AdjustThumb();
    void UpdateLayout(const TSize &);

    TEvent              slideEvent;

    OrientationType     orientation;

    TLinear             scrollRange;
    TRT_S32             scrollPos;
    TLinear             thumbRange;
    TPoint              dragBeginThumbPt;
    TPoint              dragBeginMousePt;
    TRT_S32             dragBeginDragPos;

    TSize               thumbSize;
    TRT_S32             thumbOffset;
    TRT_S32             trackWidth;
    TRect               trackRect;

    TVisual             trackVisual;
    TVisual             thumbVisual;

    TNodeEventDelegate  eventDelegate;
    TNodeArray          boundNode;
    TNode               thumb;
    };

#endif /* _INCLUDE_tgsSlider_H_ */


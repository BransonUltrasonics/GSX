/* tgsProgressBar.h - Tilcon progress bar widget */

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

This file defines Tilcon's progress bar.

The progress bar is used to show the status of a processes. The progress is
described by the start value, end value and current value; progress usually
begins at start value, and gradually increments to the end value.

Progress bar' has two types of animation mode to represent the increment, one
looks as if the bar is moving towards the end; the other mode gradually reveals
the bar to represent the increment. The bar's appearance can be customized by
the SetBarVisual() method, while the progress bar's track can be customized
by the SetTrackBackgroundVisual() and SetTrackBorderVisual(). The track's
background is the area behind the bar, while the track's border is the area that
is in front of the bar.

INCLUDE FILES: tilcon/widget/tgsProgressBar.h
*/

#ifndef _INCLUDE_tgsProgressBar_H_
#define _INCLUDE_tgsProgressBar_H_

#include "tilcon/engine/tgsControl.h"

class TProgressBar : public TControl
    {
public:
    struct ProgressEventParam : public ControlEventParam
        {
        TRT_S32 progress;   /* the current progress */
        };

    enum BarAnimationMode
        {
        BAM_reveal = 0,
        BAM_move
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TMargin             contentMargin;

        TRT_S32             trackWidth;
        TMargin             trackMargin;
        BarAnimationMode    barAnimationMode;

        TRT_BOOL            showText;
        TColor              textForegroundColor;
        TColor              textBackgroundColor;
        TextProperty        textProperty;

        TCustomVisual       trackBackgroundVisual;
        TCustomVisual       trackBorderVisual;
        TCustomVisual       barVisual;
        TCustomVisual       frameVisual;
        };
public:
/*******************************************************************************
*
* TProgressBar - construct a progress bar
*
* SYNOPSIS
* \ss
* TProgressBar
*     (
*     const TString & id = "",                      /@ identifier @/
*     TRT_S32 x = 0,                                /@ x position @/
*     TRT_S32 y = 0,                                /@ y position @/
*     TRT_S32 w = 0,                                /@ width @/
*     TRT_S32 h = 0,                                /@ height @/
*     OrientationType orientation = OT_horizontal,  /@ orientation @/
*     TRT_S32 start = 0,                            /@ start value @/
*     TRT_S32 end = 100,                            /@ end value @/
*     TRT_S32 current = 0                           /@ initial value @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a progress bar widget.
*
* Parameters:
* \is
* \i <id>
* the identifier of the progress bar widget;
* \i <x>, <y>, <w>, <h>
* the position and size of the progress bar widget;
* \i <orientation>
* horizontal or vertical progress bar;
* \i <start>, <end>, <current>
* the start value, end value and current value of the progress bar;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TProgressBar(TRT_S32 x = 0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0,
        OrientationType _orientation = OT_horizontal,
        TRT_S32 _start = 0, TRT_S32 _end = 100, TRT_S32 _pos = 0);

/*******************************************************************************
*
* ~TProgressBar - destructs the progress bar
*
* This function destructs the progress bar widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TProgressBar();

/*******************************************************************************
*
* ProgressEvent - get the Progress Event of the progress bar
*
* SYNOPSIS
* \ss
* TEvent & ProgressEvent()
* \se
*
* DESCRIPTION
* This function gets the reference to the Progress Event. Progress Event is
* emitted when the progress value is changed. The event parameter is of type
* ProgressEventParam, which contains the current progress value:
*
* \cs
* struct ProgressEventParam : public ControlEventParam
*     {
*     TRT_S32 progress;   /@ the current progress @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Progress Event of progress bar.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & ProgressEvent()
        {
        return progressEvent;
        }

/*******************************************************************************
*
* SetProperty - change the property of progress bar
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
* This function changes the property of progress bar. This function is used to
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
* GetProperty - get the property of progress bar
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of progress bar.
*
* RETURNS: the Property structure representing the progress bar's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SetOrientation - set the orientation of the progress bar
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
* This function sets the orientation of the progress bar.
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
* GetOrientation - get the orientation of the progress bar
*
* SYNOPSIS
* \ss
* OrientationType GetOrientation() const
* \se
*
* DESCRIPTION
* This function gets the orientation of the progress bar.
*
* RETURNS: the orientation of the progress bar
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
* SetRange - set the progress bar's start and end value
*
* SYNOPSIS
* \ss
* SetRange
*   (
*   TRT_S32 start,   /@ start value @/
*   TRT_S32 end      /@ end value @/
*   )
* \se
*
* DESCRIPTION
* This function sets the range of the progress bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRange()
*/
    void SetRange(TRT_S32 min, TRT_S32 max);

/*******************************************************************************
*
* GetRange - get the progress bar's start and end value
*
* SYNOPSIS
* \ss
* TLinear GetRange() const
* \se
*
* DESCRIPTION
* This function gets the start and end value of the progress bar.
*
* RETURNS: the range of the progress bar
*
* ERRNO: N/A
*
* SEE ALSO: SetRange()
*/
    TLinear GetRange() const
        {
        return TLinear(start, end);
        }

/*******************************************************************************
*
* ChangeProgress - set the progress bar's current progress
*
* SYNOPSIS
* \ss
* ChangeProgress
*   (
*   TRT_S32 value   /@ new progress value @/
*   )
* \se
*
* DESCRIPTION
* This function change the current progress of the progress bar. This function
* will trigger the Progress Event is the new progress value is different.
* If the intention is to change the progress value without Event invocation,
* use the the SetProgress() method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetProgress()
*/
    void ChangeProgress(TRT_S32 value);

/*******************************************************************************
*
* SetProgress - set the progress bar's current progress
*
* SYNOPSIS
* \ss
* SetProgress
*   (
*   TRT_S32 value   /@ new progress value @/
*   )
* \se
*
* DESCRIPTION
* This function sets the current progress of the progress bar. This function
* does not trigger the Progress Event. If the intention is to trigger the
* Progress Event when progress value is change, use the ChangeProgress() API.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetProgress(), ChangeProgress()
*/
    void SetProgress(TRT_S32 value);

/*******************************************************************************
*
* GetProgress - get the progress bar's current progress
*
* SYNOPSIS
* \ss
* TRT_S32 GetProgress() const
* \se
*
* DESCRIPTION
* This function gets the progress bar's current progress.
*
* RETURNS: the progress bar's current progress
*
* ERRNO: N/A
*
* SEE ALSO: SetProgress()
*/
    TRT_S32 GetProgress() const
        {
        return progress;
        }

/*******************************************************************************
*
* SetTrackWidth - set the slider track's width
*
* SYNOPSIS
* \ss
* SetTrackWidth
*   (
*   const TSize & value     /@ new track width @/
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
            }
        UpdateBoundRect();
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
* SetBarAnimationMode - set the animation mode of the bar
*
* SYNOPSIS
* \ss
* void SetBarAnimationMode
*     (
*     BarAnimationMode value /@ new animation mode @/
*     )
* \se
*
* DESCRIPTION
* This function sets the animation mode of the bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetBarAnimationMode()
*/
    void SetBarAnimationMode(BarAnimationMode value)
        {
        if (barAnimationMode != value)
            {
            barAnimationMode = value;
            Update();
            }
        }

/*******************************************************************************
*
* GetBarAnimationMode - get the animation mode of the bar
*
* SYNOPSIS
* \ss
* BarAnimationMode GetBarAnimationMode() const
* \se
*
* DESCRIPTION
* This function gets the animation mode of the bar.
*
* RETURNS: the animation mode of the bar
*
* ERRNO: N/A
*
* SEE ALSO: SetBarAnimationMode()
*/
    BarAnimationMode GetBarAnimationMode() const
        {
        return barAnimationMode;
        }

/*******************************************************************************
*
* GetShowText - get whether to show text on progress bar
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowText() const
* \se
*
* DESCRIPTION
* This function gets whether to show text on progress bar.
*
* RETURNS: whether to show text on progress bar
*
* ERRNO: N/A
*
* SEE ALSO: SetShowText()
*/
    TRT_BOOL GetShowText() const
        {
        return showText;
        }

/*******************************************************************************
*
* SetShowText - set whether to show text on progress bar
*
* SYNOPSIS
* \ss
* void SetShowText
*     (
*     TRT_BOOL value /@ whether to show text @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to show text on progress bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowText()
*/
    void SetShowText(TRT_BOOL show)
        {
        if (showText != show)
            {
            showText = show;
            Update();
            }
        }

/*******************************************************************************
*
* SetBarVisual - set the custom visual of the bar
*
* SYNOPSIS
* \ss
* void SetBarVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetBarVisual()
*/
    void SetBarVisual(const TCustomVisual & visual)
        {
        barVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetBarVisual - get the custom visual of the bar
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetBarVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the bar.
*
* RETURNS: reference to TCustomVisual object for the bar
*
* ERRNO: N/A
*
* SEE ALSO: SetBarVisual()
*/
    const TCustomVisual & GetBarVisual() const
        {
        return barVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetTrackBackgroundVisual - set the custom visual of the track background
*
* SYNOPSIS
* \ss
* void SetTrackBackgroundVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the track background.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTrackBackgroundVisual()
*/
    void SetTrackBackgroundVisual(const TCustomVisual & visual)
        {
        trackBackgroundVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetTrackBackgroundVisual - get the custom visual of the track background
*
* SYNOPSIS
* \ss
* const GetTrackBackgroundVisual & GetBarVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the track background.
*
* RETURNS: reference to TCustomVisual object for the track background
*
* ERRNO: N/A
*
* SEE ALSO: SetTrackBackgroundVisual()
*/
    const TCustomVisual & GetTrackBackgroundVisual() const
        {
        return trackBackgroundVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetTrackBorderVisual - set the custom visual of the track border
*
* SYNOPSIS
* \ss
* void SetTrackBorderVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual used to show the track border.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTrackBorderVisual()
*/
    void SetTrackBorderVisual(const TCustomVisual & visual)
        {
        trackBorderVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetTrackBorderVisual - get the custom visual of the track border
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetTrackBorderVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual used to show the track border.
*
* RETURNS: reference to TCustomVisual object for the track border
*
* ERRNO: N/A
*
* SEE ALSO: SetTrackBorderVisual()
*/
    const TCustomVisual & GetTrackBorderVisual() const
        {
        return trackBorderVisual.GetCustomVisual();
        }
private:

    void NodeHandler(EventParam &);
    void PaintHandler(EventParam &);
    void DrawProgressBar(TPainter &);

    TEvent              progressEvent;

    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;

    TRT_S32             trackWidth;
    TMargin             trackMargin;
    OrientationType     orientation;
    BarAnimationMode    barAnimationMode;

    TRT_BOOL            showText;
    TColor              textBackgroundColor;
    TColor              textForegroundColor;

    TVisual             trackBackgroundVisual;
    TVisual             trackBorderVisual;
    TVisual             barVisual;

    TRT_S32             start;
    TRT_S32             end;
    TRT_S32             progress;
    };

#endif /* _INCLUDE_tgsProgressBar_H_ */


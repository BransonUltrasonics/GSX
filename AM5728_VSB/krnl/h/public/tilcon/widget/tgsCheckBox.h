/* tgsCheckBox.h - Tilcon check box widget */

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

This file defines Tilcon's check box widget

The check box has two states: checked and unchecked; when user click the check
box, it toggles the state, and at the same time, generate the Check Event with
parameter indicating whether its state becomes checked or unchecked.

The state of the check box can be queried by GetChecked(), and can be changed
by SetChecked() and ChangeChecked(); the difference between SetChecked() and
ChangedChecked() is that SetChecked() modifies the state without generating
Check Event, while ChangeChecked() will generate Check Event if checked state
is modified.

The check box has a text label displayed on it; use SetText() to change the
label content. The text's attribute can be change by methods of TControl class:
SetFont(), SetTextColor(), SetTextHorizontalAlignment() and
SetTextVerticalAlignment().

The check box contains four customizable visual parts: the background area and
the check mark indicator; Each of the two visual parts is represented by two
TVisual objects, one for checked state and one for unchecked state. Use
SetCheckedFrameVisual(), SetUncheckedFrameVisual(), SetCheckedIndicatorVisual()
and SetUncheckedIndicatorVisual() methods to customize them.

INCLUDE FILES: tilcon/widget/tgsCheckBox.h
*/

#ifndef _INCLUDE_tgsCheckBox_H_
#define _INCLUDE_tgsCheckBox_H_

#include "tilcon/engine/tgsControl.h"


/* check box class */
class TCheckBox : public TControl
    {
public:
    /* check event parameter */
    struct CheckEventParam : public ControlEventParam
        {
        TRT_BOOL checked;   /* TRT_TURE if check box in checked;
                               TRT_FALSE if check box in unchecked; */
        };

    /* check box's property */
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        /* layout */
        TMargin                 contentMargin;
        TSize                   indicatorSize;
        TRT_BOOL                indicatorAtLeft;

        /* text */
        TMargin                 textMargin;
        TextProperty            textProperty;

        /* visual */
        TCustomVisual           uncheckFrameVisual;
        TCustomVisual           uncheckedIndicatorVisual;
        TCustomVisual           checkFrameVisual;
        TCustomVisual           checkedIndicatorVisual;
        };

public:

/*******************************************************************************
*
* TCheckBox - construct a check box
*
* SYNOPSIS
* \ss
* TCheckBox
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     const TWString & text = L""   /@ label text @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a check box widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the check box;
* \i <text>
* The label text of the check box;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TCheckBox
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0,
        const TWString & text = L""
        );

/*******************************************************************************
*
* ~TCheckBox - destruct the check box
*
* This function destructs the check box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TCheckBox();

/*******************************************************************************
*
* CheckEvent - get the Check Event of the check box
*
* SYNOPSIS
* \ss
* TEvent & CheckEvent()
* \se
*
* DESCRIPTION
* This function gets the reference to the Check Event. Check Event is emitted
* when the user clicks the check box to check or uncheck it. The event parameter
* is of type CheckEventParam, which contains whether the check box is toggled to
* checked state or to unchecked state:
*
* \cs
* struct CheckEventParam : public EventParam
*     {
* public:
*     TRT_BOOL checked;  /@ TRT_TRUE if toggled to checked state;
*                           TRT_FALSE if toggled unchecked state; @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Check Event of check box.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & CheckEvent()
        {
        return checkEvent;
        }

/*******************************************************************************
*
* SetProperty - set the property of check box
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
* This function changes the property of check box. This function is used to
* change multiple property items in one call. Frequently used property can be
* individually changed by dedicated APIs, e.g., label content can be changed by
* SetText(), and label font can be changed by SetFont().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetProperty()
*/
    void SetProperty
        (
        const Property &
        );

/*******************************************************************************
*
* GetProperty - get the property of check box
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of check box.
*
* RETURNS: the Property structure representing the check box's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* ChangeChecked - change the checked state of check box
*
* SYNOPSIS
* \ss
* void ChangeChecked
*     (
*     TRT_BOOL checked /@ new checked state @/
*     )
* \se
*
* DESCRIPTION
* This function changes the checked state of check box. This function generates
* the Check Event if the new state is different than the current state.
* Use SetChecked() method to change the checked state without generating the
* Check Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetChecked()
*/
    void ChangeChecked(TRT_BOOL value);

/*******************************************************************************
*
* SetChecked - set the checked state of check box
*
* SYNOPSIS
* \ss
* void SetChecked
*     (
*     TRT_BOOL checked /@ new checked state @/
*     )
* \se
*
* DESCRIPTION
* This function sets the checked state of check box. This function does not
* generate the Check Event. Use ChangeChecked() method to change the checked
* state with corresponding Check Event generation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetChecked(), ChangeChecked()
*/
    void SetChecked
        (
        TRT_BOOL value
        )
        {
        if (checked != value)
            {
            checked = value;
            Update();
            }
        }

/*******************************************************************************
*
* GetChecked - get the checked state of check box
*
* SYNOPSIS
* \ss
* TRT_BOOL GetChecked() const
* \se
*
* DESCRIPTION
* This function gets the checked state of check box.
*
* RETURNS: the checked state of check box
*
* ERRNO: N/A
*
* SEE ALSO: SetChecked()
*/
    TRT_BOOL GetChecked() const
        {
        return checked;
        }

/*******************************************************************************
*
* SetText - set label content of check box
*
* SYNOPSIS
* \ss
* void SetText
*     (
*     const TWString & string /@ new text to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the label content of check box. This label content is used
* in both checked and unchecked states.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetText()
*/
    void SetText
        (
        const TWString & value
        )
        {
        text = value;
        UpdateBoundRect();
        }

/*******************************************************************************
*
* GetText - get label content of check box
*
* SYNOPSIS
* \ss
* const TWString & GetText() const
* \se
*
* DESCRIPTION
* This function gets the label content of check box.
*
* RETURNS: TWString as the label content
*
* ERRNO: N/A
*
* SEE ALSO: SetText()
*/
    const TWString & GetText() const
        {
        return text;
        }

/*******************************************************************************
*
* SetTextMargin - set the label margin of check box
*
* SYNOPSIS
* \ss
* void SetTextMargin
*     (
*     const TMargin & margin /@ new margin to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the label margin of check box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextMargin()
*/
    void SetTextMargin
        (
        const TMargin & value
        )
        {
        if (textMargin != value)
            {
            textMargin = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetTextMargin - get the label margin of check box
*
* SYNOPSIS
* \ss
* const TMargin & GetTextMargin() const
* \se
*
* DESCRIPTION
* This function gets the label margin of check box.
*
* RETURNS: margin of the label
*
* ERRNO: N/A
*
* SEE ALSO: SetTextMargin()
*/
    const TMargin & GetTextMargin() const
        {
        return textMargin;
        }

/*******************************************************************************
*
* SetCheckedIndicatorVisual - set custom visual for checked indicator
*
* SYNOPSIS
* \ss
* void SetCheckedIndicatorVisual
*     (
*     const TCustomVisual & visual /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual object that is used to draw the check
* indicator in checked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetCheckedIndicatorVisual()
*/
    void SetCheckedIndicatorVisual(const TCustomVisual & value)
        {
        checkedIndicatorVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetCheckedIndicatorVisual - get custom visual for checked indicator
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetCheckedIndicatorVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual object that is used to draw the check
* indicator in checked state.
*
* RETURNS: reference to the TCustomVisual object used to draw the check
* indicator in checked state
*
* ERRNO: N/A
*
* SEE ALSO: SetCheckedIndicatorVisual()
*/
    const TCustomVisual & GetCheckedIndicatorVisual() const
        {
        return checkedIndicatorVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetUncheckedIndicatorVisual - set custom visual for unchecked indicator
*
* SYNOPSIS
* \ss
* void SetUncheckedIndicatorVisual
*     (
*     const TCustomVisual & visual /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual object that is used to draw the check
* indicator in unchecked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetUncheckedIndicatorVisual()
*/
    void SetUncheckedIndicatorVisual(const TCustomVisual & value)
        {
        uncheckedIndicatorVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetUncheckedIndicatorVisual - get custom visual for unchecked indicator
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetUncheckedIndicatorVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual object that is used to draw the check
* indicator in unchecked state.
*
* RETURNS: reference to the TCustomVisual object used to draw the check
* indicator in unchecked state
*
* ERRNO: N/A
*
* SEE ALSO: SetUncheckedIndicatorVisual()
*/
    const TCustomVisual & GetUncheckedIndicatorVisual() const
        {
        return uncheckedIndicatorVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetCheckedFrameVisual - set custom visual for checked background
*
* SYNOPSIS
* \ss
* void SetCheckedFrameVisual
*     (
*     const TCustomVisual & visual /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets custom visual object that is used to draw the control
* background in checked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetCheckedFrameVisual()
*/
    void SetCheckedFrameVisual(const TCustomVisual & value)
        {
        checkFrameVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetCheckedFrameVisual - get custom visual for checked background
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetCheckedFrameVisual() const
* \se
*
* DESCRIPTION
* This function gets custom visual object that is used to draw the control
* background in checked state.
*
* RETURNS: reference to TCustomVisual object used to draw the control
* background in checked states
*
* ERRNO: N/A
*
* SEE ALSO: SetCheckedFrameVisual()
*/
    const TCustomVisual & GetCheckedFrameVisual() const
        {
        return checkFrameVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetUncheckedFrameVisual - set custom visual for unchecked background
*
* SYNOPSIS
* \ss
* void SetUncheckedFrameVisual
*     (
*     const TCustomVisual & visual /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets TCustomVisual object that is used to show the control
* background in unchecked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetUncheckedFrameVisual()
*/
    void SetUncheckedFrameVisual(const TCustomVisual & value)
        {
        uncheckFrameVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetUncheckedFrameVisual - get custom visual for unchecked background
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetUncheckedFrameVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual object that is used to draw the control
* background in unchecked state.
*
* RETURNS: reference to TCustomVisual object used to draw the control
* background in unchecked states
*
* ERRNO: N/A
*
* SEE ALSO: SetUnheckedFrameVisual()
*/
    const TCustomVisual & GetUncheckedFrameVisual() const
        {
        return uncheckFrameVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetIndicatorSize - set the size of the check box indicator
*
* SYNOPSIS
* \ss
* void SetIndicatorSize
*     (
*     const TSize & value /@ new indicator size to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the size of the check box indicator.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetIndicatorSize()
*/
    void SetIndicatorSize(const TSize & value)
        {
        if (indicatorSize != value)
            {
            indicatorSize = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetIndicatorSize - get the size of the check box indicator
*
* SYNOPSIS
* \ss
* const TSize & GetIndicatorSize() const
* \se
*
* DESCRIPTION
* This function gets the size of the check box indicator.
*
* RETURNS: the size of the check box indicator
*
* ERRNO: N/A
*
* SEE ALSO: SetIndicatorSize()
*/
    const TSize & GetIndicatorSize() const
        {
        return indicatorSize;
        }

/*******************************************************************************
*
* SetIndicatorAtLeft - set whether the check box indicator is at left side
*
* SYNOPSIS
* \ss
* void SetIndicatorAtLeft
*     (
*     TRT_BOOL value /@ whether to show indicator at left @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether the check box indicator is at left side.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetIndicatorAtLeft()
*/
    void SetIndicatorAtLeft(TRT_BOOL value)
        {
        if (indicatorAtLeft != value)
            {
            indicatorAtLeft = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetIndicatorAtLeft - get whether the check box indicator is at left side
*
* SYNOPSIS
* \ss
* TRT_BOOL GetIndicatorAtLeft() const
* \se
*
* DESCRIPTION
* This function gets whether the check box indicator is at left side.
*
* RETURNS: TRT_TRUE is indicator is at left side, TRT_FALSE if at right side
*
* ERRNO: N/A
*
* SEE ALSO: SetIndicatorAtLeft()
*/
    TRT_BOOL GetIndicatorAtLeft() const
        {
        return indicatorAtLeft;
        }

private:
    void NodeHandler(EventParam &);
    void OnMouse(TNode::MouseEventParam &);
    void OnPaint(TNode::PaintEventParam &);

    TMargin         textMargin;
    TSize           indicatorSize;
    TRT_BOOL        indicatorAtLeft;

    TWString        text;
    TRT_BOOL        checked;

    TVisual         uncheckedIndicatorVisual;
    TVisual         checkedIndicatorVisual;
    TVisual         uncheckFrameVisual;
    TVisual         checkFrameVisual;

    TEvent              checkEvent;
    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;
    };

#endif /* _INCLUDE_tgsCheckBox_H_ */


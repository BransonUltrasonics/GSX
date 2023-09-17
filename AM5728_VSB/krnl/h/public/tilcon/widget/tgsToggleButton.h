/* tgsToggleButton.h - Tilcon toggle button widget */

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

This file defines Tilcon's toggle button widget

The toggle button is a push-button-like widget with two states: OUT and IN.
Clicking the toggle button makes it to toggle between OUT and IN state, and at
the same time generates Toggle Event.

The toggle button's state can be queried by GetToggled(), and can be changed by
SetToggled() and ChangeToggled(); the difference between SetToggled() and
ChangeToggled() is that SetToggled() modifies the state without generating
Toggle Event, while ChangeToggled() will generate Toggle Event if toggled state
is modified.

As there are two states for toggle button, its visible area is represented by
two TVisual objects: one for OUT state and one for IN state. Use the
SetOutStateVisual() and SetInStateVisual() method to customize them.

Toggle button is able to show different text for OUT and IN states; use the
SetOutText() and SetInText() methods to set label content of the two states
respectively. The labels of the two states share the same font and alignment,
which can be changed by SetFont(), SetTextHorizontalAlignment() and
SetVerticalAlignment().

INCLUDE FILES: tilcon/widget/tgsToggleButton.h
*/

#ifndef _INCLUDE_tgsToggleButton_H_
#define _INCLUDE_tgsToggleButton_H_

/* include */

#include "tilcon/engine/tgsControl.h"


/* toggle button class */
class TToggleButton : public TControl
    {
public:
    /* toggle event parameter */
    struct ToggleEventParam : public ControlEventParam
        {
        TRT_BOOL toggled; /* TRT_TRUE for IN state; TRT_FALSE for OUT state */
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TRT_BOOL                showText;
        TMargin                 textMargin;
        TextProperty            textProperty;
        TCustomVisual           outFrameVisual;
        TCustomVisual           inFrameVisual;
        TCustomVisual           frameVisual;
        };

public:

/*******************************************************************************
*
* TToggleButton - construct a toggle button
*
* SYNOPSIS
* \ss
* TToggleButton
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0                 /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a push button widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the toggle button;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TToggleButton
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0
        );

/*******************************************************************************
*
* ~TToggleButton - destruct the toggle button
*
* This function destructs the toggle button widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TToggleButton();

/*******************************************************************************
*
* ToggleEvent - get the Toggle Event of the toggle button
*
* SYNOPSIS
* \ss
* TEvent & CheckEvent()
* \se
*
* DESCRIPTION
* This function gets the reference to the Toggle Event. Toggle Event is emitted
* when the user clicks the button to toggle it. The event parameter is of
* type TCheckMessaage, which contains whether the button is toggled to IN state
* or to OUT state:
*
* \cs
* class ToggleEventParam : public ControlEventParam
*     {
* public:
*     TRT_BOOL toggled; /@ TRT_TRUE for IN state; TRT_FALSE for OUT state @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Toggle Event of toggle button.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & ToggleEvent()
        {
        return toggleEvent;
        }

/*******************************************************************************
*
* SetProperty - change the property of toggle button
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
* This function changes the property of toggle button. This function is used to
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
    void SetProperty
        (
        const Property &
        );

/*******************************************************************************
*
* GetProperty - get the property of toggle button
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of toggle button.
*
* RETURNS: the Property structure representing the toggle button's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* ChangeToggled - change the state of toggle button
*
* SYNOPSIS
* \ss
* void ChangeToggled
*     (
*     TRT_BOOL checked /@ new checked state @/
*     )
* \se
*
* DESCRIPTION
* This function changes the toggled state of toggle button. This function
* generates the Toggle Event if the new toggle state is different than the
* current toggle state. Use SetToggled() method to change the state without
* generating the Toggle Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetToggled()
*/
    void ChangeToggled(TRT_BOOL value);

/*******************************************************************************
*
* SetToggled - set the state of toggle button
*
* SYNOPSIS
* \ss
* void SetToggled
*     (
*     TRT_BOOL value /@ the new toggle state. TRT_TRUE for IN state,
*                       TRT_FALSE for OUT state @/
*     )
* \se
*
* DESCRIPTION
* This function sets the state of toggle button. This function does not
* generate the Toggle Event. Use ChangeToggled() method to change the toggle
* state with corresponding Toggle Event generation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ChangeToggled(), GetToggled()
*/
    void SetToggled
        (
        TRT_BOOL value
        )
        {
        if (toggled != value)
            {
            toggled = value;
            Update();
            }
        }

/*******************************************************************************
*
* GetToggled - get the state of toggle button
*
* SYNOPSIS
* \ss
* TRT_BOOL GetToggled() const
* \se
*
* DESCRIPTION
* This function gets the state of toggle button.
*
* RETURNS: the state of toggle button
*
* ERRNO: N/A
*
* SEE ALSO: SetToggled()
*/
    TRT_BOOL GetToggled() const
        {
        return toggled;
        }

/*******************************************************************************
*
* SetText - sets the label text for both in and out state
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
* This function sets the text content for both in and out state, after this
* function, the text of in state and out state will be the same. The text of in
* and out state can be independently set by SetInText() and SetOutText().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetOutText(), SetInText(), GetInText(), GetOutText()
*/
    void SetText
        (
        const TWString & value
        )
        {
        outText = value;
        inText = value;
        /* text change requires layout update */
        UpdateBoundRect();
        }

/*******************************************************************************
*
* SetOutText - sets the label text for OUT state
*
* SYNOPSIS
* \ss
* void SetOutText
*     (
*     const TWString & string /@ new text to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text content to be shown in OUT state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetText(), SetInText(), GetOutText(), GetInText()
*/
    void SetOutText
        (
        const TWString & value
        )
        {
        outText = value;
        /* text change requires layout update */
        UpdateBoundRect();
        }

/*******************************************************************************
*
* SetOutText - sets the label text for IN state
*
* SYNOPSIS
* \ss
* void SetInText
*     (
*     const TWString & string /@ new text to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text content to be shown in IN state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetText(), SetOutText(), GetOutText(), GetInText()
*/
    void SetInText
        (
        const TWString & value
        )
        {
        inText = value;
        /* text change requires layout update */
        UpdateBoundRect();
        }

/*******************************************************************************
*
* GetOutText - gets the label text for OUT state
*
* SYNOPSIS
* \ss
* const TWString & GetOutText() const
* \se
*
* DESCRIPTION
* This function gets the text content to be shown in OUT state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetText(), SetOutText(), SetInText(), GetInText()
*/
    const TWString & GetOutText() const
        {
        return outText;
        }

/*******************************************************************************
*
* GetInText - set the label text for IN state
*
* SYNOPSIS
* \ss
* const TWString & GetInText() const
* \se
*
* DESCRIPTION
* This function sets the text content to be shown in IN state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetText(), SetOutText(), SetInText(), GetOutText()
*/
    const TWString & GetInText() const
        {
        return inText;
        }

/*******************************************************************************
*
* SetShowText - set whether to show the toggle button label
*
* SYNOPSIS
* \ss
* void SetShowText
*     (
*     TRT_BOOL show /@ whether to show text label @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to show the toggle button label.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowText()
*/
    void SetShowText
        (
        TRT_BOOL show
        )
        {
        if (showText != show)
            {
            showText = show;
            Update();
            }
        }

/*******************************************************************************
*
* GetShowText - get whether to show the toggle button label
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowText() const
* \se
*
* DESCRIPTION
* This function gets whether to show the toggle button label.
*
* RETURNS: whether to show the text label on toggle button.
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
* SetTextMargin - set the text margin of button label
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
* This function sets the label margin of toggle button.
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
* GetTextMargin - get the text margin of button label
*
* SYNOPSIS
* \ss
* const TMargin & GetTextMargin() const
* \se
*
* DESCRIPTION
* This function gets the text margin of toggle button's label.
*
* RETURNS: text margin of the label
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
* SetOutStateVisual - set custom visual for OUT state
*
* SYNOPSIS
* \ss
* void SetOutStateVisual
*     (
*     const TCustomVisual & visual /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual object for the toggle button's foreground
* when the toggle button is in the OUT state. This function copies the passed
* TCustomVisual object, so the caller can discard the passed visual after the
* call to this function.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetOutStateVisual()
*/
    void SetOutStateVisual
        (
        const TCustomVisual & value
        )
        {
        outFrameVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetOutStateVisual - get custom visual for OUT state
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetOutStateVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual object for the toggle button's foreground
* when the toggle button is in the OUT state.
*
* RETURNS: reference to the custom visual object of the toggle button's
* foreground for the OUT state
*
* ERRNO: N/A
*
* SEE ALSO: SetOutStateVisual()
*/
    const TCustomVisual & GetOutStateVisual() const
        {
        return outFrameVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetInStateVisual - get custom visual for IN state
*
* SYNOPSIS
* \ss
* void SetInStateVisual
*     (
*     const TCustomVisual & visual /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual object for the toggle button's foreground
* when the toggle button is in the IN state. This function copies the passed
* TCustomVisual object, so the caller can discard the passed visual after the
* call to this function.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetInStateVisual()
*/
    void SetInStateVisual
        (
        const TCustomVisual & value
        )
        {
        inFrameVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetInStateVisual - get custom visual for IN state
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetInStateVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual object for the toggle button's foreground
* when the toggle button is in the IN state.
*
* RETURNS: reference to the custom visual object of the toggle button's
* foreground for the IN state
*
* ERRNO: N/A
*
* SEE ALSO: SetInStateVisual()
*/
    const TCustomVisual & GetInStateVisual() const
        {
        return inFrameVisual.GetCustomVisual();
        }

private:
    TEvent  toggleEvent;

    void NodeHandler(EventParam &);
    void OnMouse(TNode::MouseEventParam &);
    void OnPaint(TNode::PaintEventParam &);

    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;

    TRT_BOOL        toggled;

    TWString        outText;
    TWString        inText;
    TRT_BOOL        showText;
    TMargin         textMargin;

    TVisual         outFrameVisual;
    TVisual         inFrameVisual;
    };

#endif /* _INCLUDE_tgsToggleButton_H_ */


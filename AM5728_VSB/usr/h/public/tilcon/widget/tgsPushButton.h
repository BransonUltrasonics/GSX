/* tgsPushButton.h - Tilcon push button widget */

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
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines Tilcon's push button widget

The push button is a common used widget in UI, it emits Click Event when user
clicks it.

By default, the Click Event is generated on the rising edge of the click (i.e.,
when the mouse pressing is released); user may change this behaviour by the
SetClickWhenMouseUp() method, to generate the Click Event when mouse is pressed
down. The push button can also be configured to generate repeated Click Events
when the user keeps pressing down the button; the initial delay, the repeat
delay after the initial delay, and the repeat times can all be configured.

The push button has a text label displayed on it. Use SetShowText() method to
change whether to show the label or not; use SetText() to change the label
content. Other attributes of the label can be change by methods of the TControl
base class: SetFont(), SetTextColor(), SetTextBkColor(),
SetTextHorizontalAlignment() and SetTextVerticalAlignment().

The push button's appearance is represented by a single visual object; use the
TControl's SetFrameVisual() method to customize the appearance of push button.

INCLUDE FILES: tilcon/widget/tgsPushButton.h
*/

#ifndef _INCLUDE_tgsPushButton_H_
#define _INCLUDE_tgsPushButton_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/widget/tgsPushButtonNode.h"


/* push button class */
class TPushButton : public TControl
    {
public:
    /* click event parameter */
    struct ClickEventParam : public ControlEventParam
        {
        TRT_BOOL isRepeatedClick;       /* TRT_TRUE for repeated click */
        TRT_S32  repeatedTimes;         /* repeated times */
        TRT_S32  elapseSinceMouseDown;  /* elapsed time since mouse down */
        };

    /* push button's property */
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        /* text */
        TRT_BOOL                showText;
        TMargin                 textMargin;
        TextProperty            textProperty;

        /* click repeat */
        TRT_BOOL                clickRepeat;
        TRT_S32                 clickRepeatInitialDelay;
        TRT_S32                 clickRepeatReloadDelay;
        TRT_S32                 clickRepeatTimes;

        /* click edge */
        TRT_S32                 clickWhenMouseUp;

        /* custom visual */
        TCustomVisual           buttonVisual;
        };

public:

/*******************************************************************************
*
* TPushButton - construct a push button
*
* SYNOPSIS
* \ss
* TPushButton
*     (
*     TRT_S32 x = 0,              /@ x position @/
*     TRT_S32 y = 0,              /@ y position @/
*     TRT_S32 w = 0,              /@ width @/
*     TRT_S32 h = 0               /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a push button widget, with specified identification
* and geometry information.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the widget.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPushButton(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TPushButton - destruct the push button
*
* This function destructs the push button widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TPushButton();

/*******************************************************************************
*
* ClickEvent - get the Click Event of the push button
*
* SYNOPSIS
* \ss
* TEvent & ClickEvent()
* \se
*
* DESCRIPTION
* This function returns the reference to the Click Event of the push button.
* The Click Event is emitted whenever the push button is clicked; the event is
* also emitted for any automatic repeated clicks. The event parameter is of
* type ClickEventParam, which contains the source of click, and extra
* information regarding automatic repeated clicks:
*
* \cs
* class ClickEventParam : public EventParam
*     {
* public:
*     TRT_BOOL isRepeatedClick;       /@ TRT_TRUE for repeated click @/
*     TRT_S32  repeatedTimes;         /@ repeated times @/
*     TRT_S32  elapseSinceMouseDown;  /@ elapsed time since mouse down @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Click Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & ClickEvent()
        {
        return clickEvent;
        }

/*******************************************************************************
*
* Click - apply a click to the push button
*
* SYNOPSIS
* \ss
* void Click();
* \se
*
* DESCRIPTION
* This function applies a click to the push button, i.e, generates a Click
* Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Click();

/*******************************************************************************
*
* SetProperty - set the property of push button
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
* This function set the property of push button. This function is used to
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
    void SetProperty(const Property & property);

/*******************************************************************************
*
* GetProperty - get the property of push button
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function returns a Property structure containing the property of the
* push button.
*
* RETURNS: a Property structure containing the push button's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SetText - set the label content of push button
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
* This function sets the label content of push button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetText()
*/
    void SetText(const TWString & value)
        {
        pushButtonNode.SetText(value);
        }

/*******************************************************************************
*
* GetText - get label content of push button
*
* SYNOPSIS
* \ss
* const TWString & GetText() const
* \se
*
* DESCRIPTION
* This function gets the label content of push button.
*
* RETURNS: the label content of the push button
*
* ERRNO: N/A
*
* SEE ALSO: SetText()
*/
    const TWString & GetText() const
        {
        return pushButtonNode.GetText();
        }



/*******************************************************************************
*
* SetShowText - set whether to show the text label on push button
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
* This function sets whether to show the text label on push button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowText()
*/
    void SetShowText(TRT_BOOL show)
        {
        pushButtonNode.SetShowText(show);
        }

/*******************************************************************************
*
* GetShowText - get whether to show the text label on push button
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowText() const
* \se
*
* DESCRIPTION
* This function gets whether to show the text label on push button.
*
* RETURNS: whether to show the text label on push button
*
* ERRNO: N/A
*
* SEE ALSO: SetShowText()
*/
    TRT_BOOL GetShowText() const
        {
        return pushButtonNode.GetShowText();
        }

/*******************************************************************************
*
* SetTextMargin - set the margin of push button label
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
* This function sets the margin of push button label.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetTextMargin()
*/
    void SetTextMargin(const TMargin & value)
        {
        pushButtonNode.SetTextMargin(value);
        }

/*******************************************************************************
*
* GetTextMargin - get the margin of push button label
*
* SYNOPSIS
* \ss
* const TMargin & GetTextMargin() const
* \se
*
* DESCRIPTION
* This function gets the margin of push button label.
*
* RETURNS: the margin of push button label
*
* ERRNO: N/A
*
* SEE ALSO: SetTextMargin()
*/
    const TMargin & GetTextMargin() const
        {
        return pushButtonNode.GetTextMargin();
        }

/*******************************************************************************
*
* SetClickWhenMouseUp - set whether to generate Click Event at mouse up
*
* SYNOPSIS
* \ss
* void SetClickWhenMouseUp
*     (
*     TRT_BOOL value  /@ whether to generate Click Event at mouse up @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to generate Click Event at mouse up.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetClickWhenMouseUp()
*/
    void SetClickWhenMouseUp(TRT_BOOL value)
        {
        pushButtonNode.SetClickWhenMouseUp(value);
        }

/*******************************************************************************
*
* GetClickWhenMouseUp - get whether to generate Click Event at mouse up
*
* SYNOPSIS
* \ss
* TRT_BOOL GetClickWhenMouseUp() const
* \se
*
* DESCRIPTION
* This function gets whether to generate Click Event at mouse up.
*
* RETURNS: TRT_TRUE if click event is generated at mouse up;
* TRT_FALSE if click event is generated at mouse down;
*
* ERRNO: N/A
*
* SEE ALSO: SetClickWhenMouseUp()
*/
    TRT_BOOL GetClickWhenMouseUp() const
        {
        return pushButtonNode.GetClickWhenMouseUp();
        }

/*******************************************************************************
*
* SetClickRepeat - set whether to generate repeated Click Events
*
* SYNOPSIS
* \ss
* void SetClickRepeat
*     (
*     TRT_BOOL value /@ whether to enable click repeat @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to generate repeated Click Events when the button
* is being kept pressing down. The delay to generate the first repeated
* Click Event is set by SetClickRepeatInitDelay(); the delay to generate
* subsequent repeated Click Events after the first one is set by
* SetClickRepeatReloadDelay(), and the repeat times is set by
* SetClickRepeatTimes().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetClickRepeate(), SetClickRepeatInitDelay(),
* SetClickRepeatReloadDelay(), SetClickRepeatTimes()
*/
    void SetClickRepeat(TRT_BOOL value)
        {
        pushButtonNode.SetClickRepeat(value);
        }

/*******************************************************************************
*
* GetClickRepeat - get whether to generate repeated Click Events
*
* SYNOPSIS
* \ss
* TRT_BOOL GetClickRepeat() const
* \se
*
* DESCRIPTION
* This function gets whether to generate repeated Click Events when the button
* is being kept pressing down.
*
* RETURNS: whether to generate repeated click events
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat()
*/
    TRT_BOOL GetClickRepeat() const
        {
        return pushButtonNode.GetClickRepeat();
        }

/*******************************************************************************
*
* SetClickRepeatInitDelay - set the delay to the first repeated Click Event
*
* SYNOPSIS
* \ss
* void SetClickRepeatInitDelay
*     (
*     TRT_S32 delay   /@ the delay to be applied @/
*     )
* \se
*
* DESCRIPTION
* This function sets the delay time to generate the first repeated click event.
* The time is specified is mini-second.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat(), GetClickRepeatInitDelay()
*/
    void SetClickRepeatInitDelay(TRT_S32 value)
        {
        pushButtonNode.SetClickRepeatInitDelay(value);
        }

/*******************************************************************************
*
* GetClickRepeatInitDelay - get the delay to the first repeated Click Event
*
* SYNOPSIS
* \ss
* TRT_S32 GetClickRepeatInitDelay() const
* \se
*
* DESCRIPTION
* This function get the delay time to generate the first repeated click event.
* The time is specified in mini-second.
*
* RETURNS: the initial delay time to the first repeatedly click event.
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat(), SetClickRepeatInitDelay()
*/
    TRT_S32 GetClickRepeatInitDelay() const
        {
        return pushButtonNode.GetClickRepeatInitDelay();
        }

/*******************************************************************************
*
* SetClickRepeatReloadDelay - set the time period between repeated Click Events
*
* SYNOPSIS
* \ss
* void SetClickRepeatReloadDelay
*     (
*     TRT_S32 delay   /@ the delay to be applied @/
*     )
* \se
*
* DESCRIPTION
* This function sets the time period to generate the repeated Click Events after
* the first one. The time is specified in mini-second.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat(), GetClickRepeatReloadDelay()
*/
    void SetClickRepeatReloadDelay(TRT_S32 value)
        {
        pushButtonNode.SetClickRepeatReloadDelay(value);
        }

/*******************************************************************************
*
* GetClickRepeatReloadDelay - get the time period between repeated Click Events
*
* SYNOPSIS
* \ss
* TRT_S32 GetClickRepeatReloadDelay() const
* \se
*
* DESCRIPTION
* This function gets the time period to generate the repeated Click Event after
* the first one. The time is specified in mini-second.
*
* RETURNS: time period between repeated Click Events
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat(), SetClickRepeatReloadDelay()
*/
    TRT_S32 GetClickRepeatReloadDelay() const
        {
        return pushButtonNode.GetClickRepeatReloadDelay();
        }

/*******************************************************************************
*
* SetClickRepeatTimes - set the repeated times of the repeated Click Events
*
* SYNOPSIS
* \ss
* void SetClickRepeatTimes
*     (
*     TRT_S32 times /@ repeat times @/
*     )
* \se
*
* DESCRIPTION
* This function sets the repeated times of the repeated Click Events.
* The default repeat times is infinite.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat(), GetClickRepeatTimes()
*/
    void SetClickRepeatTimes(TRT_S32 value)
        {
        pushButtonNode.SetClickRepeatTimes(value);
        }

/*******************************************************************************
*
* GetClickRepeatTimes - get the repeated times of the repeated Click Events
*
* SYNOPSIS
* \ss
* TRT_S32 GetClickRepeatTimes() const
* \se
*
* DESCRIPTION
* This function gets the repeated times of the repeated Click Events.
*
* RETURNS: repeated times of the repeated Click Events
*
* ERRNO: N/A
*
* SEE ALSO: SetClickRepeat(), SetClickRepeatTimes()
*/
    TRT_S32 GetClickRepeatTimes() const
        {
        return pushButtonNode.GetClickRepeatTimes();
        }

/*******************************************************************************
*
* SetButtonVisual - set the custom visual of the push button
*
* SYNOPSIS
* \ss
* void SetButtonVisual
*       (
*       const TCustomVisual & value     /@ new visual to apply @/
*       )
* \se
*
* DESCRIPTION
* This function sets custom visual of the push button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetButtonVisual()
*/
    void SetButtonVisual(const TCustomVisual & value)
        {
        buttonVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetButtonVisual - get the custom visual of the push button
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetButtonVisual() const
* \se
*
* DESCRIPTION
* This function gets the custom visual of the push button.
*
* RETURNS: the custom visual of the push button.
*
* ERRNO: N/A
*
* SEE ALSO: SetButtonVisual()
*/
    const TCustomVisual & GetButtonVisual() const
        {
        return buttonVisual.GetCustomVisual();
        }

protected:
    TEvent  clickEvent;

    void NodeClickHandler(EventParam &);

    TVisual         buttonVisual;
    TPushButtonNode pushButtonNode;
    };

#endif /* _INCLUDE_tgsPushButton_H_ */


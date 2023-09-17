/* tgsMultiStateButton.h - Tilcon multi-state button widget */

/*
 * Copyright (c) 2014-2015, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
06jan17,jnl code clean
23oct15,c_l IA64 warning reduction
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines Tilcon's multi-state button widget

INCLUDE FILES: tilcon/widget/tgsMultiStateButton.h
*/

#ifndef _INCLUDE_tgsMultiStateButton_H_
#define _INCLUDE_tgsMultiStateButton_H_

#include "tilcon/engine/tgsControl.h"


class TMultiStateButton : public TControl
    {
public:
    /* state change event parameter */
    struct StateChangeEventParam : public ControlEventParam
        {
        TRT_S32     previousState;
        TRT_S32     currentState;
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TRT_BOOL                showText;
        TRT_BOOL                stateChangeWhenMouseUp;
        TMargin                 textMargin;
        TextProperty            textProperty;
        };
public:
/*******************************************************************************
*
* TMultiStateButton - construct a multi-state button
*
* SYNOPSIS
* \ss
* TMultiStateButton
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     TRT_S32 c = 1                 /@ number of states @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a multi-state button.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the multi-state button;
* \i <c>
* the number of states of the multi-state button; If <c> is less than 1, the
* default value of 1 will be used;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TMultiStateButton(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0, TRT_S32 count = 1);

/*******************************************************************************
*
* ~TMultiStateButton - destruct the multi-state button
*
* This function destructs the multi-state button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TMultiStateButton();

/*******************************************************************************
*
* StateChangeEvent - get the State Change Event of the multi-state button
*
* SYNOPSIS
* \ss
* TEvent & StateChangeEvent()
* \se
*
* DESCRIPTION
* This function gets the reference to the State Change Event. State Change Event
* is emitted when the state of the widget is changed. The event parameter is of
* type StateChangeEventParam, which contains the previous state and the new state
* the widget is changing to:
*
* \cs
* class StateChangeEventParam : public ControlEventParam
*     {
* public:
*     TRT_S32     previousState;
*     TRT_S32     currentState;
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the State Change Event of the multi-state button.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & StateChangeEvent()
        {
        return stateChangeEvent;
        }

/*******************************************************************************
*
* SetProperty - set the property of multi-state button
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
* This function set the property of multi-state button. This function is used to
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
* GetProperty - get the property of multi-state button
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function returns a Property structure containing the property of the
* multi-state button.
*
* RETURNS: a Property structure containing the multi-state button's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* ChangeState - change the state of multi-state button
*
* SYNOPSIS
* \ss
* void ChangeState
*     (
*     TRT_S32 state /@ new state @/
*     )
* \se
*
* DESCRIPTION
* This function changes the state of multi-state button. This function
* generates the State Change Event if the new state is different than the
* current state. Use SetState() method to change the state without generating
* the State Change Event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetState()
*/
    void ChangeState(TRT_S32);

/*******************************************************************************
*
* SetState - set the state of multi-state button
*
* SYNOPSIS
* \ss
* void SetState
*     (
*     TRT_S32 state /@ new state @/
*     )
* \se
*
* DESCRIPTION
* This function sets the state of multi-state button. This function does not
* generate the State Change Event. Use ChangeState() method to change the state
* with corresponding State Change Event generation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ChangeState(), GetState()
*/
    void SetState(TRT_S32);

/*******************************************************************************
*
* GetState - get the state of multi-state button
*
* SYNOPSIS
* \ss
* TRT_BOOL GetState() const
* \se
*
* DESCRIPTION
* This function gets the state of multi-state button.
*
* RETURNS: the state of multi-state button
*
* ERRNO: N/A
*
* SEE ALSO: SetState()
*/
    TRT_S32 GetState() const;

/*******************************************************************************
*
* GetStateCount - get the number of states in the multi-state button
*
* SYNOPSIS
* \ss
* TRT_S32 GetStateCount() const
* \se
*
* DESCRIPTION
* This function gets the number of states in the multi-state button.
*
* RETURNS: the number of states in the multi-state button
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetStateCount() const
        {
        return states.Count();
        }

/*******************************************************************************
*
* SetStateChangeWhenMouseUp - set whether to change state at mouse up
*
* SYNOPSIS
* \ss
* void SetStateChangeWhenMouseUp
*     (
*     TRT_BOOL value  /@ whether to change state at mouse up @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to change state at mouse up.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetStateChangeWhenMouseUp()
*/
    void SetStateChangeWhenMouseUp(TRT_BOOL value)
        {
        stateChangeWhenMouseUp = (value & 0x1U) ? !!1 : !!0;
        }

/*******************************************************************************
*
* GetStateChangeWhenMouseUp - get whether to change state at mouse up
*
* SYNOPSIS
* \ss
* TRT_BOOL GetStateChangeWhenMouseUp() const
* \se
*
* DESCRIPTION
* This function gets whether to change state at mouse up.
*
* RETURNS: whether to change state at mouse up;
*
* ERRNO: N/A
*
* SEE ALSO: SetStateChangeWhenMouseUp()
*/
    TRT_BOOL GetStateChangeWhenMouseUp() const
        {
        return stateChangeWhenMouseUp;
        }

/*******************************************************************************
*
* SetShowText - set whether to show the multi-state button label
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
* This function sets whether to show the multi-state button label.
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
        if (showText != ((show) ? 1 : 0))
            {
            showText = (show & 0x1U) ? !!1 : !!0;
            Update();
            }
        }

/*******************************************************************************
*
* GetShowText - get whether to show the multi-state button label
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowText() const
* \se
*
* DESCRIPTION
* This function gets whether to show the multi-state button label.
*
* RETURNS: whether to show the text label on multi-state button.
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
* SetTextMargin - set the text margin of multi-state label
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
* This function sets the label margin of multi-state button.
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
* GetTextMargin - get the text margin of multi-state label
*
* SYNOPSIS
* \ss
* const TMargin & GetTextMargin() const
* \se
*
* DESCRIPTION
* This function gets the text margin of multi-state button's label.
*
* RETURNS: text margin of the multi-state label
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
* SetStateText - set the text content for the specified state
*
* SYNOPSIS
* \ss
* void SetStateText
*     (
*     TRT_S32 state,            /@ state index @/
*     const TWString & text     /@ new text to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the text content for the specified state. This function
* throws TException of type ET_index if the specified index is out of range.
* This function also throws std::bad_alloc on memory failure.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetStateText()
*/
    void SetStateText(TRT_S32, const TWString &);

/*******************************************************************************
*
* GetStateText - get the text content for the specified state
*
* SYNOPSIS
* \ss
* const TWString & GetStateText
*     (
*     TRT_S32 state,                /@ state index @/
*     )
* \se
*
* DESCRIPTION
* This function gets the text content for the specified state. This function
* throws TException of type ET_index if the specified index is out of range.
*
* RETURNS: the text content for the specified state
*
* ERRNO: N/A
*
* SEE ALSO: SetStateText()
*/
    const TWString & GetStateText(TRT_S32) const;

/*******************************************************************************
*
* SetStateVisual - set the custom visual for the specified state
*
* SYNOPSIS
* \ss
* void SetStateVisual
*     (
*     TRT_S32 state,                /@ state index @/
*     const TCustomVisual & visual  /@ new visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the custom visual for the specified state. This function
* throws TException of type ET_index if the specified index is out of range.
* This function also throws std::bad_alloc on memory failure.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetStateVisual()
*/
    void SetStateVisual(TRT_S32, const TCustomVisual &);

/*******************************************************************************
*
* GetStateVisual - get the custom visual for the specified state
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetStateVisual
*     (
*     TRT_S32 state,                /@ state index @/
*     )
* \se
*
* DESCRIPTION
* This function gets the custom visual for the specified state. This function
* throws TException of type ET_index if the specified index is out of range.
*
* RETURNS: the custom visual for the specified state
*
* ERRNO: N/A
*
* SEE ALSO: SetStateVisual()
*/
    const TCustomVisual & GetStateVisual(TRT_S32) const;

private:
    struct State
        {
        State() : visual(FT_push_button)
            {}
        TWString    text;
        TVisual     visual;
        };
    void NodeHandler(EventParam &);
    void PaintHandler(EventParam &);
    void MouseHandler(EventParam &);
    void StepState();

    TEvent              stateChangeEvent;
    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;

    TRT_U8              showText                : 1;
    TRT_U8              stateChangeWhenMouseUp  : 1;
    TMargin             textMargin;

    TArray<State>       states;
    TRT_S32             currentState;
    };

#endif /* _INCLUDE_tgsMultiStateButton_H_ */


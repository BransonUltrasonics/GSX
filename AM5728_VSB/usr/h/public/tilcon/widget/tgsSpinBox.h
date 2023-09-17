/* tgsSpinBox.h - Tilcon spin box widget */

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

This file defines Tilcon's spin box widget.

INCLUDE FILES: tilcon/widget/tgsSpinBox.h
*/

#ifndef _INCLUDE_tgsSpinBox_H_
#define _INCLUDE_tgsSpinBox_H_

#include "tilcon/widget/tgsSpinLayout.h"
#include "tilcon/widget/tgsPushButtonNode.h"
#include "tilcon/widget/tgsSingleLineEditNode.h"

class TSpinBox : public TSpinLayout
    {
public:
    /* spin event parameter */
    struct SpinEventParam : public ControlEventParam
        {
        TRT_S32 value;
        };

    /* property of spin box */
    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TMargin                 margin;
        TextProperty            textProperty;

        ButtonPlacement         buttonPlacement;
        TRT_S32                 buttonWidth;

        TCustomVisual           frameVisual;
        TCustomVisual           editVisual;
        TCustomVisual           upButtonVisual;
        TCustomVisual           downButtonVisual;
        };
public:
/*******************************************************************************
*
* TSpinBox - construct a spin box
*
* SYNOPSIS
* \ss
* TSpinBox
*     (
*     TRT_S32 x = 0,            /@ x position @/
*     TRT_S32 y = 0,            /@ y position @/
*     TRT_S32 w = 0,            /@ width @/
*     TRT_S32 h = 0             /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a spin box widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the spin box;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TSpinBox(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TSpinBox - destructs the spin box
*
* This function destructs the spin box widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TSpinBox();

/*******************************************************************************
*
* SetProperty - set the property of the spin box widget
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
* This function changes the property of the spin box. This function is used to
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
    void SetProperty(const Property &);

/*******************************************************************************
*
* GetProperty - get the property of the spin box widget
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of the spin box.
*
* RETURNS: the Property structure representing the spin box's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SpinEvent - get the Spin Event of the spin box
*
* This function returns the reference to the Spin Event of the spin box.
* The Spin Event is emitted whenever the user uses the up or down button to spin
* the value of spin box. The event parameter is of type SpinEventParam, which
* contains the new spin value:
*
* \cs
* class SpinEventParam : public EventParam
*     {
* public:
*     TRT_S32  value;  /@ new value @/
*     };
* \ce
*
* This function is usually used to access the event so as to attach handler to
* the event.
*
* RETURNS: the reference to the Spin Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & SpinEvent()
        {
        return spinEvent;
        }

/*******************************************************************************
*
* SetMaxValue - set maximum value of spin box
*
* SYNOPSIS
* \ss
* void SetMaxValue
*     (
*     TRT_S32 value   /@ new maximum value to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets maximum value of spin box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetMaxValue()
*/
    void SetMaxValue(TRT_S32 value);

/*******************************************************************************
*
* GetMaxValue - get maximum value of spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetMaxValue() const
* \se
*
* DESCRIPTION
* This function sets maximum value of spin box
*
* RETURNS: maximum value of spin box
*
* ERRNO: N/A
*
* SEE ALSO: SetMaxValue()
*/
    TRT_S32 GetMaxValue() const
        {
        return max;
        }

/*******************************************************************************
*
* SetMinValue - set minimum value of spin box
*
* SYNOPSIS
* \ss
* void SetMaxValue
*     (
*     TRT_S32 value   /@ new minimum value to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets minimum value of spin box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetMinValue()
*/
    void SetMinValue(TRT_S32 value);

/*******************************************************************************
*
* GetMinValue - get minimum value of spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetMinValue() const
* \se
*
* DESCRIPTION
* This function sets minimum value of spin box
*
* RETURNS: minimum value of spin box
*
* ERRNO: N/A
*
* SEE ALSO: SetMinValue()
*/
    TRT_S32 GetMinValue() const
        {
        return min;
        }

/*******************************************************************************
*
* SetIncrement - set increment value of spin box
*
* SYNOPSIS
* \ss
* void SetIncrement
*     (
*     TRT_S32 value   /@ new increment value to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets increment value of spin box.
*
* RETURNS:
*
* ERRNO: N/A
*
* SEE ALSO: GetIncrement()
*/
    void SetIncrement(TRT_S32 value);

/*******************************************************************************
*
* GetIncrement - get increment value of spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetIncrement() const
* \se
*
* DESCRIPTION
* This function gets increment value of spin box.
*
* RETURNS: increment value of spin box
*
* ERRNO: N/A
*
* SEE ALSO: SetIncrement()
*/
    TRT_S32 GetIncrement() const
        {
        return increment;
        }

/*******************************************************************************
*
* ChangeValue - change the spin box value
*
* SYNOPSIS
* \ss
* void ChangeValue
*     (
*     TRT_S32 value/@ new value @/
*     )
* \se
*
* DESCRIPTION
* This function changes the spin box value. This function generates the Spin
* Event if the new value is different than the current value. If the event
* generation is not needed, use SetValue() instead.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetValue()
*/
    void ChangeValue(TRT_S32 value);

/*******************************************************************************
*
* SetValue - set current value of spin box
*
* SYNOPSIS
* \ss
* void SetValue
*     (
*     TRT_S32 value   /@ new value to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets current value of spin box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ChangeValue(), GetValue()
*/
    void SetValue(TRT_S32 value);

/*******************************************************************************
*
* GetValue - get current value of spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetValue() const
* \se
*
* DESCRIPTION
* This function gets current value of spin box.
*
* RETURNS: current value of spin box
*
* ERRNO: N/A
*
* SEE ALSO: SetValue()
*/
    TRT_S32 GetValue() const
        {
        return number;
        }

/*******************************************************************************
*
* SetArrowPlacement - set the placement method of spin buttons in spin box
*
* SYNOPSIS
* \ss
* void SetArrowPlacement
*     (
*     ButtonPlacement value /@ new placement @/
*     )
* \se
*
* DESCRIPTION
* This function sets the placement method of spin buttons in spin box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetArrowPlacement()
*/
    void SetArrowPlacement(ButtonPlacement value)
        {
        if (buttonPlacement != value)
            {
            buttonPlacement = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetArrowPlacement - get the placement method of spin buttons in spin box
*
* SYNOPSIS
* \ss
* ButtonPlacement GetArrowPlacement() const
* \se
*
* DESCRIPTION
* This function gets the placement method of spin buttons in spin box.
*
* RETURNS: the placement method of spin buttons in spin box
*
* ERRNO: N/A
*
* SEE ALSO: SetArrowPlacement()
*/
    ButtonPlacement GetArrowPlacement() const
        {
        return buttonPlacement;
        }

/*******************************************************************************
*
* SetButtonWidth - set the width of spin buttons in spin box
*
* SYNOPSIS
* \ss
* void SetButtonWidth
*     (
*     TRT_S32 value /@ new width @/
*     )
* \se
*
* DESCRIPTION
* This function sets the width of spin buttons in spin box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetButtonWidth()
*/
    void SetButtonWidth(TRT_S32 value)
        {
        if (buttonWidth != value)
            {
            buttonWidth = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetButtonWidth - get the width of spin buttons in spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetButtonWidth() const
* \se
*
* DESCRIPTION
* This function gets the width of spin buttons in spin box.
*
* RETURNS: the width of spin buttons in spin box
*
* ERRNO: N/A
*
* SEE ALSO: SetButtonWidth()
*/
    TRT_S32 GetButtonWidth() const
        {
        return buttonWidth;
        }

/*******************************************************************************
*
* SetEditVisual - set custom visual for the edit area of spin widget
*
* SYNOPSIS
* \ss
* void SetEditVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets custom visual for the edit area of spin widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetEditVisual()
*/
    void SetEditVisual(const TCustomVisual & value)
        {
        editVisual.SetCustomVisual(value);
        Update();
        }

/*******************************************************************************
*
* GetEditVisual - get TCustomVisual for edit area of spin widget
*
* SYNOPSIS
* \ss
* TRT_S32 GetButtonWidth() const
* \se
*
* DESCRIPTION
* This function gets TCustomVisual for edit area of spin widget.
*
* RETURNS: reference to TCustomVisual object for edit area of the spin widget
*
* ERRNO: N/A
*
* SEE ALSO: SetEditVisual()
*/
    const TCustomVisual & GetEditVisual() const
        {
        return editVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetUpButtonVisual - set TCustomVisual for the up button of spin widget
*
* SYNOPSIS
* \ss
* void SetUpButtonVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets TCustomVisual for the up button of spin widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetUpButtonVisual()
*/
    void SetUpButtonVisual(const TCustomVisual & visual)
        {
        upButtonVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetUpButtonVisual - get TCustomVisual object for the up button of spin widget
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetUpButtonVisual() const
* \se
*
* DESCRIPTION
* This function gets TCustomVisual for the up button  of spin widget.
*
* RETURNS: reference to TCustomVisual object for the up button of spin widget
*
* ERRNO: N/A
*
* SEE ALSO: SetUpButtonVisual()
*/
    const TCustomVisual & GetUpButtonVisual() const
        {
        return upButtonVisual.GetCustomVisual();
        }

/*******************************************************************************
*
* SetDownButtonVisual - set TCustomVisual for the down button of spin widget
*
* SYNOPSIS
* \ss
* void SetDownButtonVisual
*     (
*     const TCustomVisual & value /@ new custom visual @/
*     )
* \se
*
* DESCRIPTION
* This function sets TCustomVisual for the down button of spin widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetDownButtonVisual()
*/
    void SetDownButtonVisual(const TCustomVisual & visual)
        {
        downButtonVisual.SetCustomVisual(visual);
        Update();
        }

/*******************************************************************************
*
* GetDownButtonVisual - get TCustomVisual for the down button of spin widget
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetDownButtonVisual() const
* \se
*
* DESCRIPTION
* This function gets TCustomVisual for the down button of spin widget.
*
* RETURNS: reference to TCustomVisual for the up button of spin widget
*
* ERRNO: N/A
*
* SEE ALSO: SetDownButtonVisual()
*/
    const TCustomVisual & GetDownButtonVisual() const
        {
        return downButtonVisual.GetCustomVisual();
        }

private:
    TEvent  spinEvent;

    void UpdateValue();
    void ArrowClickHandler(EventParam &);

    TRT_S32                 max;
    TRT_S32                 min;
    TRT_S32                 increment;
    TRT_S32                 number;

    TMargin                 editMargin;
    TVisual                 editVisual;
    TVisual                 upButtonVisual;
    TVisual                 downButtonVisual;
    TEditNode               edit;
    TPushButtonNode         upArrow;
    TPushButtonNode         downArrow;
    };

#endif /* _INCLUDE_tgsSpinBox_H_ */


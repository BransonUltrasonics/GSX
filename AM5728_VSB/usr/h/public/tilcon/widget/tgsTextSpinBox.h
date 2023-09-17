/* tgsTextSpinBox.h - Tilcon text spin box widget */

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

This file defines Tilcon's text spin box widget.

INCLUDE FILES: tilcon/widget/tgsTextSpinBox.h
*/

#ifndef _INCLUDE_tgsTextSpinBox_H_
#define _INCLUDE_tgsTextSpinBox_H_

#include "tilcon/widget/tgsSpinLayout.h"
#include "tilcon/widget/tgsPushButtonNode.h"
#include "tilcon/widget/tgsLabelNode.h"

class TTextSpinBox : public TSpinLayout
    {
public:
    /* spin event parameter */
    struct SpinEventParam : public ControlEventParam
        {
        SpinEventParam(TRT_S32 _selection, const TWString & _text) :
            selection(_selection), text(_text)
            {
            }
        TRT_S32             selection;
        const TWString &    text;
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
* TTextSpinBox - construct a text spin box
*
* SYNOPSIS
* \ss
* TTextSpinBox
*     (
*     TRT_S32 x = 0,            /@ x position @/
*     TRT_S32 y = 0,            /@ y position @/
*     TRT_S32 w = 0,            /@ width @/
*     TRT_S32 h = 0             /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a text spin box widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the text spin box;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TTextSpinBox(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TTextSpinBox - destructs the text spin box
*
* This function destructs the text spin box widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TTextSpinBox();

/*******************************************************************************
*
* SetProperty - set the property of the text spin box
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
* This function changes the property of the text spin box. This function is used
* to change multiple property items in one call. Frequently used property can be
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
* TextSpinEvent - get the Spin Event of the text spin box
*
* This function returns the reference to the Spin Event of the text spin box.
* The Spin Event is emitted whenever the user uses the up or down button to spin
* the value of spin box. The event parameter is of type SpinEventParam, which
* contains the new text selection and reference to the text content.
*
* \cs
* class SpinEventParam : public EventParam
*     {
* public:
*     TRT_S32  value;       /@ new text selection @/
*     const TWString & text;/@ reference to text content of the selection @/
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
    TEvent & TextSpinEvent()
        {
        return textSpinEvent;
        }

/*******************************************************************************
*
* AddItem - add a new item in the text spin box selections
*
* SYNOPSIS
* \ss
* void AddItem
*     (
*     const TWString & item /@ text content for the new item @/
*     )
* \se
*
* DESCRIPTION
* This function adds a new item in the text spin box selections.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ClearItems()
*/
    void AddItem(const TWString &);

/*******************************************************************************
*
* ClearItems - removes all items in the text spin box selections
*
* SYNOPSIS
* \ss
* void ClearItems()
* \se
*
* DESCRIPTION
* This function removes all items in the text spin box selections.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddItem()
*/
    void ClearItems();

/*******************************************************************************
*
* GetItemCount - get the number of items in the text spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetItemCount() const
* \se
*
* DESCRIPTION
* This function gets the number of items in the text spin box.
*
* RETURNS: item count
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetItemCount() const;

/*******************************************************************************
*
* ChangeSelection - change the current selection of text spin box
*
* SYNOPSIS
* \ss
* void ChangeSelection
*     (
*     TRT_S32 sel /@ new selection @/
*     )
* \se
*
* DESCRIPTION
* This function changes the current selection of text spin box. This function
* generates the Text Spin Event if the new selection is different than the
* current selection. If the event generation is not needed, use SetSelection()
* to change the selection.
* If the specified selection is out of valid range, this function throws
* TException with type ET_index;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetSelection()
*/
    void ChangeSelection(TRT_S32);

/*******************************************************************************
*
* SetSelection - set the current selection of text spin box
*
* SYNOPSIS
* \ss
* void SetSelection
*     (
*     TRT_S32 sel /@ new selection @/
*     )
* \se
*
* DESCRIPTION
* This function sets the current selection of text spin box. This function
* changes the spin selection without generating the Text Spin Event. Use the
* ChangeSelection() method to change the selection with event generation.
* If the specified selection is out of valid range, this function throws
* TException with type ET_index;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ChangeSelection(), GetSelection()
*/
    void SetSelection(TRT_S32);

/*******************************************************************************
*
* GetSelection - get the current selection of text spin box
*
* SYNOPSIS
* \ss
* TRT_S32 GetSelection() const
* \se
*
* DESCRIPTION
* This function gets the current selection of text spin box.
*
* RETURNS: index of the current selection of text spin box.
*
* ERRNO: N/A
*
* SEE ALSO: SetSelection()
*/
    TRT_S32 GetSelection() const;

/*******************************************************************************
*
* GetSelectionText - get the text content of to the current selection
*
* SYNOPSIS
* \ss
* const TWString & GetSelectionText() const
* \se
*
* DESCRIPTION
* This function gets the text content of to the current selection. If the spin
* box contains no item, the function return a reference to an empty string.
*
* RETURNS: the text content of to the current selection.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    const TWString & GetSelectionText() const;

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
    TEvent  textSpinEvent;

    void ArrowClickHandler(EventParam &);

    TRT_S32                 selection;
    TArray<TWString>        items;
    TWString                emptyText;

    TMargin                 editMargin;
    TVisual                 editVisual;
    TVisual                 upButtonVisual;
    TVisual                 downButtonVisual;
    TLabelNode              label;
    TPushButtonNode         upArrow;
    TPushButtonNode         downArrow;
    };

#endif /* _INCLUDE_tgsTextSpinBox_H_ */


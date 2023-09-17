/* tgsRadioButton.h - Tilcon radio button widget */

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

This file defines Tilcon's radio button widget

The radio button provides multiple mutual-exclusive choices for user to choose
from. When the user changes item selection in the radio button, it generates
the Radio Event, which contains information regarding the previous selected
item and the newly selected item.

The selection of the radio button can be queried by GetSelection() method, and
can be changed by SetSelection() and ChangeSelection() methods; the difference
between SetSelection() and ChangedSelection() is that SetSelection() changes the
selection without generating Radio Event, while ChangeSelection() will generate
the Radio Event if the selection is actually changed.

The radio button's items is added by AddItem() method, and can be removed by the
RemoveAllItem() method.

The radio button's text attribute can be change by the TControl base class's
methods: SetFont(), SetTextColor(), SetTextBkColor(),
SetTextHorizontalAlignment() and SetTextVerticalAlignment().

The radio button's entire background appearance can be customized by changing
the control's frame visual, via TControl's SetFrameVisual() method.

The radio button's item contains four customizable visual object: two for the
item's background in selected and unselected states, and two for the item's
indicator in selected and unselected states. APIs to change these custom visual
objects are provided.

INCLUDE FILES: tilcon/widget/tgsRadioButton.h
*/

#ifndef _INCLUDE_tgsRadioButton_H_
#define _INCLUDE_tgsRadioButton_H_

#include "tilcon/engine/tgsControl.h"

class TRadioButton : public TControl
    {
public:
    /* radio event parameter */
    struct RadioEventParam : public ControlEventParam
        {
        TRT_S32 selected;   /* entry index selected */
        TRT_S32 deSelected; /* entry index previous selected(lose selection)*/
        };

    enum RowSizeMethod
        {
        RSM_fill_all_height = 0,
        RSM_fixed_height
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        TMargin                 frameMargin;
        TCustomVisual           frameVisual;

        RowSizeMethod           rowSizeMethod;
        TRT_S32                 rowHeight;
        TRT_S32                 itemsPerRow;
        TSize                   indicatorSize;

        TCustomVisual           uncheckedIndicatorVisual;
        TCustomVisual           checkedIndicatorVisual;
        TCustomVisual           uncheckedFrameVisual;
        TCustomVisual           checkedFrameVisual;

        TMargin                 textMargin;
        TextProperty            textProperty;
        };
public:
/*******************************************************************************
*
* TRadioButton - construct a radio button
*
* SYNOPSIS
* \ss
* TCheckBox
*     (
*     const TString & id = "",      /@ identifier @/
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0                 /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a radio button widget.
*
* Parameters:
* \is
* \i <id>
* The identifier of the radio button;
* \i <x>, <y>, <w>, <h>
* The position and size of the radio button;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRadioButton(TRT_S32 x = 0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TRadioButton - destruct the radio button
*
* This function destructs the radio button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TRadioButton();

/*******************************************************************************
*
* RadioEvent - get the Radio Event of the radio button
*
* SYNOPSIS
* \ss
* TEvent & RadioEvent()
* \se
*
* DESCRIPTION
* This function gets the reference to the Radio Event. Radio Event is emitted
* when the user changes the radio button selection. The event parameter is of
* type RadioEventParam, which contains the previous selection and the new
* selection of the radio button:
*
* \cs
* class RadioEventParam : public ControlEventParam
*     {
* public:
*     TRT_S32 selected;   /@ entry index selected @/
*     TRT_S32 deSelected; /@ entry index previous selected @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Radio Event of radio button.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & RadioEvent()
        {
        return radioEvent;
        }

/*******************************************************************************
*
* SetProperty - set the property of radio button
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
* This function changes the property of radio button. This function is used to
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
* GetProperty - get the property of radio button
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of radio button.
*
* RETURNS: the Property structure representing the radio button's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* AddItem - add an item in the radio button
*
* SYNOPSIS
* \ss
* void AddItem
*     (
*     const TWString & text /@ text content for the new item @/
*     )
* \se
*
* DESCRIPTION
* This function adds an item in the radio button. This function throws
* std::bad_alloc on memory failure.
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
* ClearItems - remove all items in the radio button
*
* SYNOPSIS
* \ss
* void ClearItems();
* \se
*
* DESCRIPTION
* This function removes all items in the radio button.
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
* GetItemCount - get the number of items in the radio button
*
* SYNOPSIS
* \ss
* TRT_S32 GetItemCount() const
* \se
*
* DESCRIPTION
* This function gets the number of items in the radio button.
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
* ChangeSelection - change the selection of radio button
*
* SYNOPSIS
* \ss
* void ChangeSelection
*     (
*     TRT_S32 index /@ new selection @/
*     )
* \se
*
* DESCRIPTION
* This function changes the selection of radio button. This function generates
* the Radio Event if the new selection is different than the current selection.
* Use SetSelection() method to change the selection without generating the
* Radio Event.
* This function throws ET_index if specified index is out of valid range.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetSelection()
*/
    void ChangeSelection(TRT_S32 index);

/*******************************************************************************
*
* SetSelection - set the selection of the radio button
*
* SYNOPSIS
* \ss
* void SetSelection
*     (
*     TRT_S32 index /@ new selection @/
*     )
* \se
*
* DESCRIPTION
* This function sets the selection of the radio button. This function does not
* generate the Radio Event when selection is changed. Use ChangeSelection()
* method to change the selection with corresponding Radio Event generation.
* This function throws ET_index if specified index is out of valid range.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ChangeSelection(), GetSelection()
*/
    void SetSelection(TRT_S32 index);

/*******************************************************************************
*
* GetSelection - get the selection of the radio button
*
* SYNOPSIS
* \ss
* TRT_S32 GetSelection() const
* \se
*
* DESCRIPTION
* This function gets the selection of the radio button.
*
* RETURNS: index of the current selection.
*
* ERRNO: N/A
*
* SEE ALSO: SetSelection()
*/
    TRT_S32 GetSelection() const;

/*******************************************************************************
*
* SetItemsPerRow - set how many items are shown in one row
*
* SYNOPSIS
* \ss
* void SetItemsPerRow
*     (
*     TRT_S32 value /@ new value to apply @/
*     )
* \se
*
* DESCRIPTION
* This function set how many items are shown in one row.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetItemsPerRow()
*/
    void SetItemsPerRow(TRT_S32 value);

/*******************************************************************************
*
* GetItemsPerRow - get how many items are shown in one row
*
* SYNOPSIS
* \ss
* TRT_S32 GetItemsPerRow() const
* \se
*
* DESCRIPTION
* This function get how many items are shown in one row.
*
* RETURNS: number of items shown in one row
*
* ERRNO: N/A
*
* SEE ALSO: SetItemsPerRow()
*/
    TRT_S32 GetItemsPerRow() const
        {
        return itemsPerRow;
        }

/*******************************************************************************
*
* SetShowIndicator - set whether to show the selection indicator
*
* SYNOPSIS
* \ss
* void SetShowIndicator
*     (
*     TRT_BOOL value /@ whether to show the selection indicator @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to show the selection indicator.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowIndicator()
*/
    void SetShowIndicator(TRT_BOOL value)
        {
        if (showIndicator != value)
            {
            showIndicator = value;
            Update();
            }
        }

/*******************************************************************************
*
* GetShowIndicator - get whether to show the selection indicator
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowIndicator()
* \se
*
* DESCRIPTION
* This function gets whether to show the selection indicator.
*
* RETURNS: whether to show the selection indicator
*
* ERRNO: N/A
*
* SEE ALSO: SetShowIndicator()
*/
    TRT_BOOL GetShowIndicator() const
        {
        return showIndicator;
        }

/*******************************************************************************
*
* SetShowText - set whether to show the radio item text
*
* SYNOPSIS
* \ss
* void SetShowText
*     (
*     TRT_BOOL show /@ whether to show the radio item text @/
*     )
* \se
*
* DESCRIPTION
* This function sets whether to show the radio item text.
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
* GetShowText - get whether to show the radio item text
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowText() const
* \se
*
* DESCRIPTION
* This function gets whether to show the radio item text.
*
* RETURNS: whether to show the radio item text.
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
* SetUncheckedIndicatorVisual - set radio item's indicator visual
*
* SYNOPSIS
* \ss
* void SetUncheckedIndicatorVisual
* (
* TRT_S32 index,                /@ item index @/
* const TCustomVisual & visual  /@ visual to apply @/
* );
* \se
*
* DESCRIPTION
* This function sets the radio item's indicator visual in unchecked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetUncheckedIndicatorVisualToAllItems()
*/
    void SetUncheckedIndicatorVisual(TRT_S32 index, const TCustomVisual & visual);

/*******************************************************************************
*
* SetCheckedIndicatorVisual - set radio item's indicator visual
*
* SYNOPSIS
* \ss
* void SetCheckedIndicatorVisual
* (
* TRT_S32 index,                /@ item index @/
* const TCustomVisual & visual  /@ visual to apply @/
* );
* \se
*
* DESCRIPTION
* This function sets the radio item's indicator visual in checked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetCheckedIndicatorVisualToAllItems()
*/
    void SetCheckedIndicatorVisual(TRT_S32 index, const TCustomVisual & visual);

/*******************************************************************************
*
* SetUncheckedFrameVisual - set radio item's background visual
*
* SYNOPSIS
* \ss
* void SetUncheckedFrameVisual
* (
* TRT_S32 index,                /@ item index @/
* const TCustomVisual & visual  /@ visual to apply @/
* );
* \se
*
* DESCRIPTION
* This function sets the radio item's background visual in unchecked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetUncheckedFrameVisualToAllItems()
*/
    void SetUncheckedFrameVisual(TRT_S32 index, const TCustomVisual & visual);

/*******************************************************************************
*
* SetCheckedFrameVisual - set radio item's background visual
*
* SYNOPSIS
* \ss
* void SetCheckedFrameVisual
* (
* TRT_S32 index,                /@ item index @/
* const TCustomVisual & visual  /@ visual to apply @/
* );
* \se
*
* DESCRIPTION
* This function sets the radio item's background visual in checked state.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetCheckedFrameVisualToAllItems()
*/
    void SetCheckedFrameVisual(TRT_S32 index, const TCustomVisual & visual);

/*******************************************************************************
*
* SetUncheckedIndicatorVisualToAllItems - set indicator visual to all items
*
* SYNOPSIS
* \ss
* void SetUncheckedIndicatorVisualToAllItems
*     (
*     const TCustomVisual & visual  /@ visual to apply @/
*     );
* \se
*
* DESCRIPTION
* This function sets the indicator visual for the unchecked state to all items
* in the radio button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetUncheckedIndicatorVisual()
*/
    void SetUncheckedIndicatorVisualToAllItems(const TCustomVisual & visual);

/*******************************************************************************
*
* SetCheckedIndicatorVisualToAllItems - set indicator visual to all items
*
* SYNOPSIS
* \ss
* void SetCheckedIndicatorVisualToAllItems
*     (
*     const TCustomVisual & visual  /@ visual to apply @/
*     );
* \se
*
* DESCRIPTION
* This function sets the indicator visual for the checked state to all items
* in the radio button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetCheckedIndicatorVisual()
*/
    void SetCheckedIndicatorVisualToAllItems(const TCustomVisual & visual);

/*******************************************************************************
*
* SetUncheckedFrameVisualToAllItems - set frame visual to all items
*
* SYNOPSIS
* \ss
* void SetUncheckedFrameVisualToAllItems
*     (
*     const TCustomVisual & visual  /@ visual to apply @/
*     );
* \se
*
* DESCRIPTION
* This function sets the frame visual for the unchecked state to all items
* in the radio button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetUncheckedFrameVisual()
*/
    void SetUncheckedFrameVisualToAllItems(const TCustomVisual & visual);

/*******************************************************************************
*
* SetCheckedIndicatorVisualToAllItems - set frame visual to all items
*
* SYNOPSIS
* \ss
* void SetCheckedFrameVisualToAllItems
*     (
*     const TCustomVisual & visual  /@ visual to apply @/
*     );
* \se
*
* DESCRIPTION
* This function sets the frame visual for the checked state to all items
* in the radio button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetCheckedFrameVisual()
*/
    void SetCheckedFrameVisualToAllItems(const TCustomVisual & visual);

private:
    class TRadioItem : public TNode
        {
    public:
        TRadioItem(TEventDelegate *, const TWString & str = TWString());
        virtual ~TRadioItem();
        TRadioItem & operator = (const TRadioItem &);

        TWString    text;
        TVisual     uncheckedIndicatorVisual;
        TVisual     checkedIndicatorVisual;
        TVisual     uncheckedFrameVisual;
        TVisual     checkedFrameVisual;
        };

    void NodeHandler(EventParam &);
    void OnItemMouse(TNode::MouseEventParam &);
    void OnResize(TNode::ResizeEventParam &);

    void ChangeSelectedItem(TRadioItem * item);
    void SetSelectedItem(TRadioItem * item);
    void DrawRadioItem(TPainter &, TRadioItem *);

    TEvent              radioEvent;
    TNodeEventDelegate  eventDelegate;
    TNodeContainer      items;
    TRadioItem *        selectedItem;

    /* property */
    RowSizeMethod       rowSizeMethod;
    TRT_S32             rowHeight;
    TRT_S32             itemsPerRow;
    TRT_BOOL            showIndicator;
    TRT_BOOL            showText;
    TSize               indicatorSize;
    TMargin             textMargin;
    };

#endif /* _INCLUDE_tgsRadioButton_H_ */


/* tgsComboBox.h - Tilcon combo box widget */

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

This file defines Tilcon's combo box widget.

The combo box widget lets the use to select one item from multiple choices.
The combo box contains a text area and a button; the text area displays the
current selected item, and the button is used to open the drop down list which
lists all available selections. New selection is added to the list by the
AddItem() method.

When selection is changed, the combo box widget generates the Combo Event, with
information regarding the previous selection and the new selection. User can
use the ChangeSelection() method to simulate selection change and generate the
corresponding Combo Event. User can also use SetSelection() to change the
current selection, but this function does not generate the Combo Event.

The appearance of the text area and the button can be customized by the
SetEditVisual() and SetButtonVisual() methods.

INCLUDE FILES: tilcon/widget/tgsComboBox.h
*/

#ifndef _INCLUDE_tgsComboBox_H_
#define _INCLUDE_tgsComboBox_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsWindow.h"
#include "tilcon/engine/tgsGridLayout.h"
#include "tilcon/widget/tgsSingleLineEdit.h"
#include "tilcon/widget/tgsListBox.h"

class TComboBox : public TControl
    {
public:
    class ComboEventParam : public ControlEventParam
        {
    public:
        TRT_S32 selected;   /* index of newly selected item */
        TRT_S32 deselected; /* index of previously selected item */
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        /* combo layout */

        TMargin                     frameMargin;
        TRT_S32                     buttonWidth;
        TRT_S32                     listVisibleItems;
        TMargin                     listFrameMargin;
        TRT_S32                     listLineSpace;
        TScrollBarPair::ShowPolicy  listHorBarShowPolicy;
        TScrollBarPair::ShowPolicy  listVerBarShowPolicy;

        /* combo visual */

        TCustomVisual               frameVisual;
        TCustomVisual               buttonVisual;
        TCustomVisual               editVisual;
        TCustomVisual               listFrameVisual;
        TCustomVisual               listContentVisual;
        TCustomVisual               listLeftArrowVisual;
        TCustomVisual               listRightArrowVisual;
        TCustomVisual               listHorBarTrackVisual;
        TCustomVisual               listHorBarThumbVisual;
        TCustomVisual               listUpArrowVisual;
        TCustomVisual               listDownArrowVisual;
        TCustomVisual               listVerBarTrackVisual;
        TCustomVisual               listVerBarThumbVisual;

        /* text */

        TextProperty                editTextProperty;
        TextProperty                listTextProperty;
        };

public:
/*******************************************************************************
*
* TComboBox - construct a combo box
*
* SYNOPSIS
* \ss
* TComboBox
*     (
*     TRT_S32 x = 0,              /@ x position @/
*     TRT_S32 y = 0,              /@ y position @/
*     TRT_S32 w = 0,              /@ width @/
*     TRT_S32 h = 0               /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function construct a scroll bar widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the combo box;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TComboBox(TRT_S32 x = 0, TRT_S32 y = 0,
            TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TComboBox - destruct the combo box
*
* DESCRIPTION
* This function destructs the combo box widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TComboBox();

/*******************************************************************************
*
* ComboBoxEvent - get the Event of the combo box
*
* SYNOPSIS
* \ss
* TEvent & ComboBoxEvent()
* \se
*
* DESCRIPTION
* This function returns the reference to the ComboBox Event. The ComboBox Event
* is emitted whenever the user changes the selected item of the ComboBox, via
* the drop-down list or other methods from UI. The event parameter is of type
* ComboEventParam, which contains the index of newly selected item and the
* previously selected item:
*
* class ComboEventParam : public EventParam
*     {
* public:
*     TRT_S32 selected;     /@ index of newly selected item @/
*     TRT_S32 deselected;   /@ index of previously selected item @/
*     };
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the ComboBox Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & ComboBoxEvent()
        {
        return comboBoxEvent;
        }

/*******************************************************************************
*
* SetProperty - change the property of combo box
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
* This function changes the property of combo box. This function is used to
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
* GetProperty - get the property of combo box
*
* SYNOPSIS
* \ss
* Property GetProperty() const
* \se
*
* DESCRIPTION
* This function gets the property of combo box.
*
* RETURNS: the Property structure representing the widget's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* AddItem - add a new item in the combo box selections
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
* This function adds a new item in the combo box selections.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ClearItems()
*/
    void AddItem(const TWString & item)
        {
        list.AddRow(item);
        }

/*******************************************************************************
*
* ClearItems - removes all items in the combo box selections
*
* SYNOPSIS
* \ss
* void ClearItems()
* \se
*
* DESCRIPTION
* This function removes all items in the combo box selections.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddItem()
*/
    void ClearItems()
        {
        list.ClearRows();
        edit.ClearText();
        selection = -1;
        }

/*******************************************************************************
*
* SetFont - set the font of all text components in combo box
*
* SYNOPSIS
* \ss
* void SetFont
*     (
*     const TFont & font  /@ new font to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the font of all text components in combo box, including
* the edit field and the drop-down list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void SetFont(const TFont & font)
        {
        TControl::SetFont(font);
        list.SetFont(font);
        }

/*******************************************************************************
*
* SetEditFont - set the font of edit field in combo box
*
* SYNOPSIS
* \ss
* void SetEditFont
*     (
*     const TFont & font  /@ new font to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the font of edit field in combo box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetEditFont()
*/
    void SetEditFont(const TFont & font)
        {
        TControl::SetFont(font);
        }

/*******************************************************************************
*
* GetEditFont - get the font of edit field in combo box
*
* SYNOPSIS
* \ss
* const TFont & GetEditFont() const
* \se
*
* DESCRIPTION
* This function gets the font of edit field in combo box
*
* RETURNS: font of edit field in combo box.
*
* ERRNO: N/A
*
* SEE ALSO: SetEditFont()
*/
    const TFont & GetEditFont() const
        {
        return TControl::GetFont();
        }

/*******************************************************************************
*
* SetListFont - set the font of drop down list in combo box
*
* SYNOPSIS
* \ss
* void SetListFont
*     (
*     const TFont & font  /@ new font to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the font of drop-down list in combo box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetListFont()
*/
    void SetListFont(const TFont & font)
        {
        list.SetFont(font);
        }

/*******************************************************************************
*
* GetListFont - get the font of drop-down list in combo box
*
* SYNOPSIS
* \ss
* const TFont & GetListFont() const
* \se
*
* DESCRIPTION
* This function gets the font of drop-down list in combo box
*
* RETURNS: font of drop-down list in combo box.
*
* ERRNO: N/A
*
* SEE ALSO: SetListFont()
*/
    const TFont & GetListFont() const
        {
        return list.GetFont();
        }

/*******************************************************************************
*
* SetSelection - set the current selection of combo box
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
* This function sets the current selection of combo box.
* If the specified selection is out of valid range, this function throws
* TException with type ET_index;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetSelection()
*/
    void SetSelection(TRT_S32);

/*******************************************************************************
*
* GetSelection - get the current selection of combo box
*
* SYNOPSIS
* \ss
* TRT_S32 GetSelection() const
* \se
*
* DESCRIPTION
* This function gets the current selection of combo box.
*
* RETURNS: index of the current selection of combo box.
*
* ERRNO: N/A
*
* SEE ALSO: SetSelection()
*/
    TRT_S32 GetSelection() const
        {
        return selection;
        }

/*******************************************************************************
*
* GetText - get the current selection's text content
*
* SYNOPSIS
* \ss
* const TWString & GetText() const
* \se
*
* DESCRIPTION
* This function gets the current selection's text content.
*
* RETURNS: current selection's text content.
*
* ERRNO: N/A
*
* SEE ALSO: SetSelection()
*/
    const TWString & GetText() const
        {
        return edit.GetText();
        }

/*******************************************************************************
*
* SetButtonWidth - set the width of drop-down button
*
* SYNOPSIS
* \ss
* void SetButtonWidth
*     (
*     TRT_S32 width   /@ new width to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the width of drop-down button.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetButtonWidth()
*/
    void SetButtonWidth(TRT_S32 width)
        {
        buttonWidth = width;
        layout.SetColumnSize(1, TGridLayout::LM_fixed, buttonWidth);
        UpdateBoundRect();
        }

/*******************************************************************************
*
* GetButtonWidth - get the width of dropdown button
*
* SYNOPSIS
* \ss
* TRT_S32 GetButtonWidth() const
* \se
*
* DESCRIPTION
* This function gets the width of drop-down button.
*
* RETURNS: the width of drop-down button
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
* SetEditVisual - set custom visual object for the edit field
*
* SYNOPSIS
* \ss
* void SetEditVisual
*     (
*     const TCustomVisual & value /@ custom visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the TCustomVisual object that is used to show the edit
* field in the combo box.
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
* GetEditVisual - get custom visual object for the edit field
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetEditVisual() const
* \se
*
* DESCRIPTION
* This function gets the TCustomVisual object that is used to show the edit
* field in the combo box.
*
* RETURNS: reference to TCustomVisual object for edit field
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
* SetButtonVisual - set custom visual object for the drop-down button
*
* SYNOPSIS
* \ss
* void SetButtonVisual
*     (
*     const TCustomVisual & value /@ new custom visual to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the TCustomVisual object that is used to show the drop-down
* button in the combo box.
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
* GetButtonVisual - get custom visual object for the drop-down button
*
* SYNOPSIS
* \ss
* const TCustomVisual & GetButtonVisual() const
* \se
*
* DESCRIPTION
* This function gets the TCustomVisual object that is used to show the drop-down
* button in the combo box.
*
* RETURNS: reference to TCustomVisual object for drop-down button
*
* ERRNO: N/A
*
* SEE ALSO: SetButtonVisual()
*/
    const TCustomVisual & GetButtonVisual() const
        {
        return buttonVisual.GetCustomVisual();
        }

private:
    TEvent  comboBoxEvent;

    void NodeHandler(EventParam &);
    void ResizeHandler(EventParam &);
    void MouseHandler(EventParam &);
    void PaintHandler(EventParam &);
    void DetachHandler(EventParam &);

    void ButtonMouseHandler(EventParam &);
    void ButtonPaintHandler(EventParam &);
    void ItemClickHandler(EventParam &);

    void OpenDropdown();
    void CloseDropdown();

    TListBox                list;
    TWindow                 dropdown;

    TRT_BOOL                dropdownShown;
    TRT_S32                 selection;

    TMargin                 editMargin;
    TVisual                 editVisual;
    TVisual                 buttonVisual;

    TRT_S32                 buttonWidth;
    TRT_S32                 listVisibleItems;

    TGridLayout             layout;
    TNodeEventDelegate      eventDelegate;
    TNodeArray              boundNode;
    TEditNode               edit;
    TPushButtonNode         button;
    };

#endif /* _INCLUDE_tgsComboBox_H_ */


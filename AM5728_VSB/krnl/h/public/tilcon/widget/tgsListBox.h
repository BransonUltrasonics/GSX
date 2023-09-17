/* tgsListBox.h - Tilcon list box class */

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
01jan14,yya created (US15550)
*/

/*
DESCRIPTION

This file defines the Tilcon's list box class.

INCLUDE FILES: tilcon/engine/tgsListBox.h
*/

#ifndef _INCLUDE_tgsListBox_H_
#define _INCLUDE_tgsListBox_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/widget/tgsScrollBarPair.h"
#include "tilcon/widget/tgsScrollHelper.h"


class TListBox : public TControl
    {
public:
    enum ColumnWidthMode
        {
        CWM_fixed = 0,
        CWM_relative,
        CWM_from_text
        };

    enum RowHeightMode
        {
        RHM_fixed = 0,
        RHM_from_text
        };

    struct ItemSelectEventParam : public ControlEventParam
        {
        TRT_S32   selection; /* selection index */
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        /* layout */

        TMargin                     frameMargin;
        TScrollBarPair::ShowPolicy  horBarShowPolicy;
        TScrollBarPair::ShowPolicy  verBarShowPolicy;

        TRT_S32                     rowSpace;
        TRT_BOOL                    showColumnHeader;
        TRT_BOOL                    showGrid;
        TRT_BOOL                    showMouseHover;
        ColumnWidthMode             columnWidthMode;

        /* visual */

        TCustomVisual               frameVisual;
        TCustomVisual               contentVisual;
        TCustomVisual               leftArrowVisual;
        TCustomVisual               rightArrowVisual;
        TCustomVisual               horBarTrackVisual;
        TCustomVisual               horBarThumbVisual;
        TCustomVisual               upArrowVisual;
        TCustomVisual               downArrowVisual;
        TCustomVisual               verBarTrackVisual;
        TCustomVisual               verBarThumbVisual;

        /* font */

        TextProperty                textProperty;
        };

    struct ColHeaderInfo
        {
        TWString    text;
        TRT_S32     width;
        TRT_U32     proportion;
        TRT_S32     effectiveWidth;
        };
public:

/*******************************************************************************
*
* TListBox - construct a list box
*
* SYNOPSIS
* \ss
* TListBox
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     TRT_S32 cols = 1              /@ number of columns @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a list box.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the widget;
* \i <cols>
* number of columns in the list, should be greater than 0; if 0 or negative is
* specified here, default value will be used (default column number is 1);
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TListBox(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0, TRT_S32 cols = 1);

/*******************************************************************************
*
* ~TListBox - destructs the list box
*
* This function destructs the list box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TListBox();

/*******************************************************************************
*
* ItemSelectEvent - get the Item Select Event of the list box
*
* SYNOPSIS
* \ss
* TEvent & ItemSelectEvent()
* \se
*
* DESCRIPTION
* This function returns the reference to the Item Select Event of the list box.
* The Item Select Event is emitted whenever the item selection is changed in the
* list box as a result of user interaction, e.g., mouse click, or key press.
* The event parameter is of type ItemSelectEventParam, which contains the row
* index of the new selection.
*
* \cs
* class ItemSelectEventParam : public EventParam
*     {
* public:
*     TRT_S32   selection;  /@ index of the row which is selected @/
*     };
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the Item Select Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & ItemSelectEvent()
        {
        return itemSelectEvent;
        }

/*******************************************************************************
*
* SetProperty - set the property of the list box
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
* This function changes the property of the list box. This function is used to
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
* GetProperty - get the property of the list box
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of the list box.
*
* RETURNS: the Property structure representing the list box's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* ResetScroll - reset scroll of list box
*
* SYNOPSIS
* \ss
* void ResetScroll()
* \se
*
* DESCRIPTION
* This function resets scroll of list box (i.e., move to top of the list).
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ScrollToRow()
*/
    void ResetScroll();

/*******************************************************************************
*
* ScrollToRow - scroll the list content to specified row
*
* SYNOPSIS
* \ss
* void ScrollToRow
*    (
*    TRT_S32 index,     /@ row index to scroll to @/
*    )
* \se
*
* DESCRIPTION
* This function scrolls the list content to specified row. this function does
* nothing if the specified row index is out of range.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ResetScroll()
*/
    void ScrollToRow(TRT_S32 index);

/*******************************************************************************
*
* AddRow - add a new row to the list box
*
* SYNOPSIS
* \ss
* void AddRow
*    (
*    const TWString & text,     /@ text content of the new row @/
*    TRT_BOOL update = TRT_TRUE /@ whether to update list box after addition @/
*    )
* \se
*
* DESCRIPTION
* This function adds a new row in the list box. The new row is added at the
* last of the list.
*
* Parameters:
* \is
* \i <text>
* the text content of the new row. This text content is shown in the first
* column of the added row. To set text content of other columns, use the
* SetItemText() method after the row is added.
* \i <update>
* whether to update the list layout after adding the row; if multiple rows are
* to be added, the good practice is to only update the list after the last
* addition, which avoid unnecessary intermediate updates.
* \ie
*
* This function throws std::bad_alloc on memory failure;
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: RemoveRow(), ClearRows()
*/
    void AddRow(const TWString & text, TRT_BOOL update = TRT_TRUE);

/*******************************************************************************
*
* RemoveRow - remove the specified row in the list box
*
* SYNOPSIS
* \ss
* void RemoveRow
*    (
*    TRT_S32 row,               /@ index of row to be removed @/
*    TRT_BOOL update = TRT_TRUE /@ whether to update list box after remove @/
*    )
* \se
*
* DESCRIPTION
* This function removes the specified row in the list box.
*
* Parameters:
* \is
* \i <row>
* index of the row to be removed; this function does nothing if the specified
* row index is out of range.
* \i <update>
* update the list layout after removing the row; if multiple rows are to be
* removed, the good practice is to only update the list after the last
* operation, which avoid unnecessary intermediate updates.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: ClearRow(), AddRow()
*/
    void RemoveRow(TRT_S32 row, TRT_BOOL update = TRT_TRUE);

/*******************************************************************************
*
* ClearRows - remove all rows in the list box
*
* SYNOPSIS
* \ss
* void ClearRows()
* \se
*
* DESCRIPTION
* This function removes all rows in the list box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: RemoveRow(), AddRow()
*/
    void ClearRows();

/*******************************************************************************
*
* GetRowCount - get the number of rows in the list box
*
* SYNOPSIS
* \ss
* TRT_S32 GetRowCount() const
* \se
*
* DESCRIPTION
* This function gets the number of rows in the list box.
*
* RETURNS: the number of rows in the list box
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 GetRowCount() const
        {
        return items.Count();
        }

/*******************************************************************************
*
* SetItemText - set the text content of specific list item
*
* SYNOPSIS
* \ss
* void SetItemText
*    (
*    TRT_S32 row,           /@ index of row @/
*    TRT_S32 col,           /@ index of column @/
*    const TWString & text  /@ text to be set @/
*    )
* \se
*
* DESCRIPTION
* This function sets the text of list item. This function throws TException of
* type ET_index if the specified row index is out of range. This function also
* throws std::bad_alloc on memory failure.
*
* Parameters:
* \is
* \i <row>
* row index of the item;
* \i <col>
* column index of the item;
* \i <text>
* text to be set;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetItemText()
*/
    void SetItemText(TRT_S32 row, TRT_S32 col, const TWString & text);

/*******************************************************************************
*
* GetItemText - get the text of list item
*
* SYNOPSIS
* \ss
* const TWString & GetItemText
*    (
*    TRT_S32 row,           /@ index of row @/
*    TRT_S32 col,           /@ index of column @/
*    ) const
* \se
*
* DESCRIPTION
* This function sets the text of list item. This function throws TException of
* type ET_index if the specified row index is out of range.
*
* Parameters:
* \is
* \i <row>
* row index of the item;
* \i <col>
* column index of the item;
* \ie
*
* RETURNS: text of the item
*
* ERRNO: N/A
*
* SEE ALSO: SetItemText
*/
    const TWString & GetItemText(TRT_S32 row, TRT_S32 col) const;

/*******************************************************************************
*
* SetColumnHeaderText - set the text of column header
*
* SYNOPSIS
* \ss
* void SetColumnHeaderText
*    (
*    TRT_S32 col,           /@ index of column @/
*    const TWString & text  /@ text to be set @/
*    )
* \se
*
* DESCRIPTION
* This function sets the text of column header. This function throws TException
* of type ET_index if the specified row index is out of range. This function
* also throws std::bad_alloc on memory failure.
*
* Parameters:
* \is
* \i <col>
* index of the column;
* \i <text>
* new header text of the column;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetColumnHeaderText()
*/
    void SetColumnHeaderText(TRT_S32 col, const TWString & text);

/*******************************************************************************
*
* GetColumnHeaderText - get the text of column header
*
* SYNOPSIS
* \ss
* void GetColumnHeaderText
*    (
*    TRT_S32 col            /@ index of column @/
*    )
* \se
*
* DESCRIPTION
* This function gets the text of column header. This function throws TException
* of type ET_index if the specified row index is out of range
*
* RETURNS: text of the column header;
*
* ERRNO: N/A
*
* SEE ALSO: SetColumnHeaderText()
*/
    const TWString & GetColumnHeaderText(TRT_S32 col) const;

/*******************************************************************************
*
* SetColumnWidthMode - set the method for determining column width
*
* SYNOPSIS
* \ss
* void SetColumnWidthMode
*     (
*     ColumnWidthMode value /@ new mode to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the method for determining column width.
* \is
* \i CWM_relative
* column width is based on relative proportion between columns. The proportion
* of individual column can be changed by the SetProportionColumnWidth() method.
* \i CWM_fixed
* column width is based on absolute value, specified by the
* SetFixedColumnWidth() method.
* \i CWM_from_text
* column width is based on the text content;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetColumnWidthMode()
*/
    void SetColumnWidthMode(ColumnWidthMode value)
        {
        if (columnWidthMode != value)
            {
            columnWidthMode = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetColumnWidthMode - get the method for determining column width
*
* SYNOPSIS
* \ss
* ColumnWidthMode GetColumnWidthMode() const
* \se
*
* DESCRIPTION
* This function gets the method for determining column width.
*
* RETURNS: the method for determining column width
*
* ERRNO: N/A
*
* SEE ALSO: SetColumnWidthMode()
*/
    ColumnWidthMode GetColumnWidthMode() const
        {
        return columnWidthMode;
        }

/*******************************************************************************
*
* SetFixedColumnWidth - set the width of the specified column
*
* SYNOPSIS
* \ss
* void SetFixedColumnWidth
*    (
*    TRT_S32 col,           /@ index of column @/
*    TRT_U32 width          /@ new width to apply @/
*    )
* \se
*
* DESCRIPTION
* This function sets the width of the specified column. This function throws
* TException of type ET_index if the specified row index is out of range.
*
* Parameters:
* \is
* \i <col>
* column index of the column;
* \i <width>
* new width of the column;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetFixedColumnWidth()
*/
    void SetFixedColumnWidth(TRT_S32 col, TRT_S32 width);

/*******************************************************************************
*
* GetFixedColumnWidth - get the width of the specified column
*
* SYNOPSIS
* TRT_U32 GetFixedColumnWidth
*    (
*    TRT_S32 col,           /@ index of column @/
*    )
* \se
*
* DESCRIPTION
* This function gets the width of the specified column. This function throws
* TException of type ET_index if the specified row index is out of range
*
* RETURNS: width of the specified column;
*
* ERRNO: N/A
*
* SEE ALSO: SetFixedColumnWidth()
*/
    TRT_S32 GetFixedColumnWidth(TRT_S32 col) const;

/*******************************************************************************
*
* SetProportionColumnWidth - set the relative width of the specified column
*
* SYNOPSIS
* \ss
* void SetProportionColumnWidth
*    (
*    TRT_S32 col,   /@ index of column @/
*    TRT_U32 value  /@ new proportion to apply @/
*    )
* \se
*
* DESCRIPTION
* This function sets the relative width of the specified column.
* This function throws TException of type ET_index if the specified row index
* is out of range.
* Proportion width is only used when the Column Width Mode is CWM_relative.
* The relative width is based on the proportion value between columns.
* The default proportion value for all column is 100.
*
* Parameters:
* \is
* \i <col>
* the column index of the column;
* \i <value>
* the new proportion to apply;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetProportionColumnWidth()
*/
    void SetProportionColumnWidth(TRT_S32 col, TRT_U32 proportion);

/*******************************************************************************
*
* GetProportionColumnWidth - get the relative width of the specified column
*
* SYNOPSIS
* \ss
* TRT_U32 SetProportionColumnWidth
*    (
*    TRT_S32 col,   /@ index of column @/
*    )
* \se
*
* DESCRIPTION
* This function gets the relative width of the specified column.
*
* RETURNS: the relative width of the specified column
*
* ERRNO: N/A
*
* SEE ALSO: SetProportionColumnWidth()
*/
    TRT_U32 GetProportionColumnWidth(TRT_S32 col) const;

/*******************************************************************************
*
* GetActualColumnWidth - get the actual col width of the list box
*
* SYNOPSIS
* \ss
* TRT_S32 GetActualColumnWidth
*     (
*     TRT_S32 col /@ col index @/
*     )
* \se
*
* DESCRIPTION
* This function gets the width of the specified column in the list box.
* If the Column Width Mode is set to CWM_fixed, the width of column is the fixed
* value specified by the GetFixedColumnWidth() method; if the Column Width Mode
* is CWM_relative, the column width is based on the relative proportion set by
* the SetProportionColumnWidth() method. If the Column Width Mode is CWM_from_text,
* the actual width of column is based on the width of text content inside the
* specified column.
*
* RETURNS: the height of row in list
*
* ERRNO: N/A
*
* SEE ALSO: GetFixedColumnWidth(), SetProportionColumnWidth(), SetColumnWidthMode()
*/
    TRT_S32 GetActualColumnWidth(TRT_S32 col) const;

/*******************************************************************************
*
* SetRowHeightMode - set the method for determining row height
*
* SYNOPSIS
* \ss
* void SetRowHeightMode
*     (
*     RowHeightMode value /@ new mode to set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the method for determining row height.
* \is
* \i RHM_fixed
* row height is specified by SetRowHeight() method.
* \i RHM_from_text
* row height is based on height of text font, plus the space value specified by
* the SetRowSpace() method;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRowHeightMode()
*/
    void SetRowHeightMode(RowHeightMode value)
        {
        if (rowHeightMode != value)
            {
            rowHeightMode = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetRowHeightMode - get the method for determining row height
*
* SYNOPSIS
* \ss
* RowHeightMode GetRowHeightMode() const
* \se
*
* DESCRIPTION
* This function gets the method for determining row height.
*
* RETURNS: the method for determining row height
*
* ERRNO: N/A
*
* SEE ALSO: SetRowHeightMode()
*/
    RowHeightMode GetRowHeightMode() const
        {
        return rowHeightMode;
        }

/*******************************************************************************
*
* SetFixedRowHeight - set the fixed row height of the list box
*
* SYNOPSIS
* \ss
* void SetFixedRowHeight
*    (
*    TRT_S32 value  /@ the fixed row height @/
*    )
* \se
*
* DESCRIPTION
* This function sets the fixed row height of the list box.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetFixedRowHeight()
*/
    void SetFixedRowHeight(TRT_S32 value)
        {
        if (fixedRowHeight != value)
            {
            fixedRowHeight = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetFixedRowHeight - get the fixed row height of the list box
*
* SYNOPSIS
* \ss
* TRT_S32 GetFixedRowHeight() const
* \se
*
* DESCRIPTION
* This function gets the fixed row height of the list box.
*
* RETURNS: the fixed row height of the list box
*
* ERRNO: N/A
*
* SEE ALSO: SetFixedRowHeight()
*/
    TRT_S32 SetFixedRowHeight() const
        {
        return fixedRowHeight;
        }

/*******************************************************************************
*
* SetRowSpace - set the space between rows in list
*
* SYNOPSIS
* \ss
* void SetShowColumnHeader
*    (
*    TRT_S32 value  /@ the space between rows @/
*    )
* \se
*
* DESCRIPTION
* This function sets the space between rows in list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRowSpace()
*/
    void SetRowSpace(TRT_S32 value)
        {
        if (rowSpace != value)
            {
            rowSpace = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetRowSpace - get the space between rows in list
*
* SYNOPSIS
* \ss
* TRT_S32 GetRowSpace() const
* \se
*
* DESCRIPTION
* This function gets the space between rows in list.
*
* RETURNS: the space between rows in list
*
* ERRNO: N/A
*
* SEE ALSO: SetRowSpace()
*/
    TRT_S32 GetRowSpace() const
        {
        return rowSpace;
        }

/*******************************************************************************
*
* GetActualRowHeight - get the actual row height of the list box
*
* SYNOPSIS
* \ss
* TRT_S32 GetActualRowHeight() const
* \se
*
* DESCRIPTION
* This function gets the height of row in list. If the Row Height Mode is set
* to RHM_fixed, the height of row is the fixed value set by the SetRowHeight()
* method; if the Row Height Mode is RHM_from_text, the actual height of row is
* based on the text font being used and the row space set by the SetRowSpace()
* method.
*
* RETURNS: the height of row in list
*
* ERRNO: N/A
*
* SEE ALSO: SetFont(), SetRowSpace(), SetRowHeight(), SetRowHeightMode()
*/
    TRT_S32 GetActualRowHeight() const
        {
        return rowHeight;
        }

/*******************************************************************************
*
* SetShowColumnHeader - set whether to show the column header
*
* SYNOPSIS
* \ss
* void SetShowColumnHeader
*    (
*    TRT_BOOL show  /@ whether to show the column header @/
*    )
* \se
*
* DESCRIPTION
* This function sets whether to show the column header.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowColumnHeader()
*/
    void SetShowColumnHeader(TRT_BOOL value)
        {
        if (showColumnHeader != value)
            {
            showColumnHeader = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetShowColumnHeader - get whether to show the column header
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowColumnHeader() const
* \se
*
* DESCRIPTION
* This function gets whether to show the column header.
*
* RETURNS: whether to show the column header
*
* ERRNO: N/A
*
* SEE ALSO: SetShowColumnHeader()
*/
    TRT_BOOL GetShowColumnHeader() const
        {
        return showColumnHeader;
        }

/*******************************************************************************
*
* SetShowGrid - set whether to show the list grid
*
* SYNOPSIS
* \ss
* void SetShowGrid
*    (
*    TRT_BOOL show  /@ whether to show the list grid @/
*    )
* \se
*
* DESCRIPTION
* This function sets whether to show the list grid.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowGrid()
*/
    void SetShowGrid(TRT_BOOL value)
        {
        if (showGrid != value)
            {
            showGrid = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetShowGrid - get whether to show the list grid
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowGrid() const
* \se
*
* DESCRIPTION
* This function gets whether to show the list grid.
*
* RETURNS: whether to show the list grid
*
* ERRNO: N/A
*
* SEE ALSO: SetShowGrid()
*/
    TRT_BOOL GetShowGrid() const
        {
        return showGrid;
        }

/*******************************************************************************
*
* SetShowMouseHover - set whether to show the mouse hover highlight
*
* SYNOPSIS
* \ss
* void SetShowMouseHover
*    (
*    TRT_BOOL show  /@ whether to show the mouse hover highlight @/
*    )
* \se
*
* DESCRIPTION
* This function sets whether to show the mouse hover highlight.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowMouseHover()
*/
    void SetShowMouseHover(TRT_BOOL value)
        {
        if (showMouseHover != value)
            {
            showMouseHover = value;
            Update();
            }
        }

/*******************************************************************************
*
* GetShowMouseHover - get whether to show the mouse hover highlight
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowMouseHover() const
* \se
*
* DESCRIPTION
* This function gets whether to show the mouse hover highlight.
*
* RETURNS: whether to show the mouse hover highlight
*
* ERRNO: N/A
*
* SEE ALSO: SetShowMouseHover
*/
    TRT_BOOL GetShowMouseHover() const
        {
        return showMouseHover;
        }

/*******************************************************************************
*
* SetHorBarPolicy - set the show policy of the horizontal scroll bar
*
* SYNOPSIS
* \ss
* void SetHorBarPolicy
*     (
*     TScrollBarPair::ShowPolicy    value /@ new policy to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the show policy of the horizontal scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetHorBarPolicy()
*/
    void SetHorBarPolicy(TScrollBarPair::ShowPolicy value)
        {
        boundNode.SetHorBarPolicy(value);
        }

/*******************************************************************************
*
* GetHorBarPolicy - get the show policy of the horizontal scroll bar
*
* SYNOPSIS
* \ss
* TScrollBarPair::ShowPolicy GetHorBarPolicy() const
* \se
*
* DESCRIPTION
* This function gets the show policy of the horizontal scroll bar.
*
* RETURNS: the show policy of the horizontal scroll bar
*
* ERRNO: N/A
*
* SEE ALSO:setHorBarPolicy()
*/
    TScrollBarPair::ShowPolicy GetHorBarPolicy() const
        {
        return boundNode.GetHorBarPolicy();
        }

/*******************************************************************************
*
* SetVerBarPolicy - set the show policy of the vertical scroll bar
*
* SYNOPSIS
* \ss
* void SetVerBarPolicy
*     (
*     TScrollBarPair::ShowPolicy    value /@ new policy to apply @/
*     )
* \se
*
* DESCRIPTION
* This function sets the show policy of the vertical scroll bar.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetVerBarPolicy()
*/
    void SetVerBarPolicy(TScrollBarPair::ShowPolicy value)
        {
        boundNode.SetVerBarPolicy(value);
        }

/*******************************************************************************
*
* GetVerBarPolicy - set the show policy of the vertical scroll bar
*
* SYNOPSIS
* \ss
* TScrollBarPair::ShowPolicy GetVerBarPolicy() const
* \se
*
* DESCRIPTION
* This function sets the show policy of the vertical scroll bar.
*
* RETURNS: the show policy of the vertical scroll bar
*
* ERRNO: N/A
*
* SEE ALSO: SetVerBarPolicy()
*/
    TScrollBarPair::ShowPolicy GetVerBarPolicy() const
        {
        return boundNode.GetVerBarPolicy();
        }

private:
    TEvent itemSelectEvent;

    TEvent scrollContentChangeEvent;
    TEvent scrollBarChangeEvent;

    void NodeHandler(EventParam &);

    void ScrollContentChangeHandler(EventParam &);
    void ScrollBarChangeHandler(EventParam &);

    void ClientResizeHandler(EventParam &);

    void ListPaintHandler(EventParam &);
    void ListMouseHandler(EventParam &);
    void HeaderPaintHandler(EventParam &);
    void HeaderMouseHandler(EventParam &);

    void HScrollHandler(EventParam &);
    void VScrollHandler(EventParam &);

    void GainFocusHandler(EventParam &);
    void LoseFocusHandler(EventParam &);
    void MouseLeaveHandler(EventParam &);

    void CursorTimerHandler(EventParam &);
    void DragTimerHandler(EventParam &);
    void TScrollViewportResizeHandler(EventParam &);

    void DrawList(TPainter &);
    void DrawHeader(TPainter &);
    void UpdateViewLayout(const TSize & size);
    void UpdateAfterSizeChange();
    void UpdateLayout()
        {
        boundNode.UpdateLayout();
        }

    TRT_S32             firstItemInView;
    TRT_S32             itemsInView;
    TRT_S32             hoverIndex;
    TRT_S32             rowHeight;
    TScrollHelper       scrollHelper;

    ColHeaderInfo *     headerInfo;
    TArray<TWString *>  items;

    TRT_S32             columns;

    /* property */

    TRT_BOOL            showColumnHeader;
    TRT_BOOL            showGrid;
    TRT_BOOL            showMouseHover;
    ColumnWidthMode     columnWidthMode;
    TRT_S32             rowSpace;
    TRT_S32             fixedRowHeight;
    RowHeightMode       rowHeightMode;

    TVisual             contentVisual;
    TVisual             headerVisual;
    TextProperty        headerTextProperty;

    TNodeEventDelegate  eventDelegate;
    TScrollBarPair      boundNode;
    TNodeArray          clientNode;
    TNode               header;
    TNode               list;
    };

#endif /* _INCLUDE_tgsListBox_H_ */


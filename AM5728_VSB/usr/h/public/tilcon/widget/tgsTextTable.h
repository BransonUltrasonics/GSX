/* tgsTextTable.h - Tilcon text table widget */

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

This file defines the Tilcon's text table widget.

INCLUDE FILES: tilcon/engine/tgsTextTable.h
*/

#ifndef _INCLUDE_tgsTextTable_H_
#define _INCLUDE_tgsTextTable_H_

#include "tilcon/engine/tgsGridLayout.h"
#include "tilcon/widget/tgsScrollBarPair.h"
#include "tilcon/widget/tgsScrollHelper.h"

class TTextTable : public TControl
    {
public:
    enum ColumnWidthMode
        {
        CWM_fixed = 0,
        CWM_relative
        };

    struct Property : public ControlProperty
        {
        Property();
        ~Property();

        ColumnWidthMode             columnWidthMode;
        TRT_BOOL                    showGrid;
        TRT_BOOL                    showRowHeader;
        TRT_BOOL                    showColumnHeader;

        TMargin                     frameMargin;
        TScrollBarPair::ShowPolicy  horBarShowPolicy;
        TScrollBarPair::ShowPolicy  verBarShowPolicy;

        TCustomVisual               frameVisual;
        TCustomVisual               rowHeaderVisual;
        TCustomVisual               colHeaderVisual;

        TCustomVisual               leftArrowVisual;
        TCustomVisual               rightArrowVisual;
        TCustomVisual               horBarTrackVisual;
        TCustomVisual               horBarThumbVisual;
        TCustomVisual               upArrowVisual;
        TCustomVisual               downArrowVisual;
        TCustomVisual               verBarTrackVisual;
        TCustomVisual               verBarThumbVisual;

        TextProperty                textProperty;
        };

    struct ColHeader
        {
        TWString    text;
        TRT_S32     width;
        TRT_U32     proportion;
        TRT_S32     effectiveWidth;
        };

public:
/*******************************************************************************
*
* TTextTable - construct a text table
*
* SYNOPSIS
* \ss
* TTextTable
*     (
*     TRT_S32 x = 0,                /@ x position @/
*     TRT_S32 y = 0,                /@ y position @/
*     TRT_S32 w = 0,                /@ width @/
*     TRT_S32 h = 0,                /@ height @/
*     TRT_S32 r = 1,                /@ number of rows in the table @/
*     TRT_S32 c = 1                 /@ number of columns in the table @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a text table.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the widget;
* \i <r>, <col>
* number of rows and columns in the text table, which should be greater than 0;
* if 0 or negative are specified here, default value of 1will be used;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TTextTable(TRT_S32 x = 0, TRT_S32 y = 0,
        TRT_S32 w = 0, TRT_S32 h = 0, TRT_S32 r = 1, TRT_S32 c = 1);

/*******************************************************************************
*
* ~TTextTable - destructs the text table
*
* This function destructs the text table.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TTextTable();

/*******************************************************************************
*
* SetProperty - set the property of the text table
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
* This function changes the property of the text table. This function is used to
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
* GetProperty - get the property of the text table
*
* SYNOPSIS
* \ss
* Property GetProperty() const;
* \se
*
* DESCRIPTION
* This function gets the property of the text table.
*
* RETURNS: the Property structure representing the text table's property
*
* ERRNO: N/A
*
* SEE ALSO: SetProperty()
*/
    Property GetProperty() const;

/*******************************************************************************
*
* SetCellText - set the text of table cell
*
* SYNOPSIS
* \ss
* void SetCellText
*    (
*    TRT_S32 row,           /@ index of row @/
*    TRT_S32 col,           /@ index of column @/
*    const TWString & text  /@ text to be set @/
*    )
* \se
*
* DESCRIPTION
* This function sets the text of table cell. This function throws TException of
* type ET_index if the specified row index is out of range. This function
* throws std::bad_alloc on memory failure.
*
* Parameters:
* \is
* \i <row>
* row index of the cell;
* \i <col>
* column index of the cell;
* \i <text>
* text to be set;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetCellText()
*/
    void SetCellText(TRT_S32 row, TRT_S32 col, const TWString & text);

/*******************************************************************************
*
* GetCellText - get the text of table cell
*
* SYNOPSIS
* \ss
* const TWString & GetCellText
*    (
*    TRT_S32 row,           /@ index of row @/
*    TRT_S32 col,           /@ index of column @/
*    ) const
* \se
*
* DESCRIPTION
* This function sets the text of table cell. This function throws TException of
* type ET_index if the specified row index is out of range.
*
* Parameters:
* \is
* \i <row>
* row index of the cell;
* \i <col>
* column index of the cell;
* \ie
*
* RETURNS: text of the item otherwise;
*
* ERRNO: N/A
*
* SEE ALSO: SetCellText()
*/
    const TWString & GetCellText(TRT_S32 row, TRT_S32 col) const;

/*******************************************************************************
*
* SetRowHeaderText - set the text of row header
*
* SYNOPSIS
* \ss
* void SetRowHeaderText
*    (
*    TRT_S32 row,           /@ index of row @/
*    const TWString & text  /@ text to be set @/
*    )
* \se
*
* DESCRIPTION
* This function sets the text of row header. This function throws TException
* of type ET_index if the specified row index is out of range. This function
* throws std::bad_alloc on memory failure.
*
* Parameters:
* \is
* \i <row>
* index of the row;
* \i <text>
* new header text of the row;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRowHeaderText()
*/
    void SetRowHeaderText(TRT_S32 row, const TWString & text);

/*******************************************************************************
*
* GetRowHeaderText - get the text of row header
*
* SYNOPSIS
* \ss
* void GetRowHeaderText
*    (
*    TRT_S32 row            /@ index of row @/
*    )
* \se
*
* DESCRIPTION
* This function gets the text of row header. This function throws TException
* of type ET_index if the specified row index is out of range
*
* RETURNS: text of the row header;
*
* ERRNO: N/A
*
* SEE ALSO: SetRowHeaderText()
*/
    const TWString & GetRowHeaderText(TRT_S32 row) const;

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
* TRT_S32 GetFixedColumnWidth
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
* SetShowRowHeader - set whether to show the row header
*
* SYNOPSIS
* \ss
* void SetShowRowHeader
*    (
*    TRT_BOOL show  /@ whether to show the row header @/
*    )
* \se
*
* DESCRIPTION
* This function sets whether to show the row header.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetShowRowHeader()
*/
    void SetShowRowHeader(TRT_BOOL value)
        {
        if (showRowHeader != value)
            {
            showRowHeader = value;
            UpdateBoundRect();
            }
        }

/*******************************************************************************
*
* GetShowRowHeader - get whether to show the row header
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowRowHeader() const
* \se
*
* DESCRIPTION
* This function gets whether to show the row header.
*
* RETURNS: whether to show the row header
*
* ERRNO: N/A
*
* SEE ALSO: SetShowRowHeader()
*/
    TRT_BOOL GetShowRowHeader() const
        {
        return showRowHeader;
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
* SetShowGrid - set whether to show the table grid
*
* SYNOPSIS
* \ss
* void SetShowGrid
*    (
*    TRT_BOOL show  /@ whether to show the table grid @/
*    )
* \se
*
* DESCRIPTION
* This function sets whether to show the table grid.
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
* GetShowGrid - get whether to show the table grid
*
* SYNOPSIS
* \ss
* TRT_BOOL GetShowGrid() const
* \se
*
* DESCRIPTION
* This function gets whether to show the table grid.
*
* RETURNS: whether to show the table grid
*
* ERRNO: N/A
*
* SEE ALSO: SetShowGrid()
*/
    TRT_BOOL GetShowGrid() const
        {
        return showGrid;
        }

private:
    TEvent scrollContentChangeEvent;
    TEvent scrollBarChangeEvent;

    void NodeHandler(EventParam &);
    void ScrollBarChangeHandler(EventParam &);
    void ScrollContentChangeHandler(EventParam &);

    void HScrollHandler(EventParam &);
    void VScrollHandler(EventParam &);
    void ViewportResizeHandler(EventParam &);
    void ClientResizeHandler(EventParam &);
    void ColHeaderPaintHandler(EventParam &);
    void RowHeaderPaintHandler(EventParam &);
    void CellAreaPaintHandler(EventParam &);

    void UpdateLayout(const TSize &);

    TRT_S32                 rowCnt;
    TRT_S32                 colCnt;
    TRT_S32                 rowHeight;
    TRT_S32                 rowHeaderWidth;
    TRT_S32                 colHeaderHeight;
    TArray<TWString>        rowLabels;
    TArray<ColHeader>       colHeaders;
    TArray<TWString>        contents;

    TRT_S32                 firstRowInView;
    TRT_S32                 rowsInView;

    ColumnWidthMode         columnWidthMode;
    TRT_BOOL                showGrid;
    TRT_BOOL                showRowHeader;
    TRT_BOOL                showColumnHeader;

    TVisual                 rowHeaderVisual;
    TVisual                 colHeaderVisual;

    TNodeEventDelegate      eventDelegate;
    TGridLayout             layout;
    TScrollBarPair          boundNode;
    TNodeArray              clientNode;
    TNode                   rowHeaderViewport;
    TNodeArray              colHeaderViewport;
    TNode                   colHeaderArea;
    TNodeArray              cellViewport;
    TNode                   cellArea;
    TScrollHelper           scrollHelper;
    };

#endif /* _INCLUDE_tgsTextTable_H_ */


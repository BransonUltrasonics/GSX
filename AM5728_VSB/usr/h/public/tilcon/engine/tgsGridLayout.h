/* tgsGridLayout.h - Tilcon grid layout class */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's grid layout class.

TGridlayout arranges the interface in grids, formed by rows and columns. Each
child on the interface will be placed in one cell or in the rectangle defined
by collection of cells in consecutive rows or columns.

The height/width of each row/column in the grid can be individually changed.
There are two sizing methods available to arrange row and column in the grid
layout: one is to use absolute size(LM_fixed), the other is to used relative
size(LM_proportion). When doing layout, the logic first goes through all
rows/columns and give the ones with absolute sizing method the size they
request, then the remaining size is divided among those rows/columns with
relative sizing method, based on the proportion value of each of them.

The default sizing method is relative (LM_proportion), and the default
proportion value is 100 for all rows/columns; so in default, all rows/columns in
the grid layout equally divide the available space.

INCLUDE FILES: tilcon/engine/tgsGridLayout.h
*/

#ifndef _INCLUDE_tgsGridLayout_H_
#define _INCLUDE_tgsGridLayout_H_

#include "tilcon/tilcon.h"

/* grid layout class */
class TGridLayout
    {
    friend class TGridPanel;
public:
    enum LayoutMethod
        {
        LM_proportion = 0,
        LM_fixed,
        };

    struct LayoutInfo
        {
        LayoutMethod    method;
        TRT_S32         fix;
        TRT_S32         proportion;
        TRT_S32         start;
        TRT_S32         end;

        LayoutInfo()
            {
            method = LM_proportion;
            fix = 0;
            proportion = 100;
            start = 0;
            end = 0;
            }
        };

public:
/*******************************************************************************
*
* TGridLayout - construct a grid layout
*
* SYNOPSIS
* \ss
* TGridLayout
*     (
*     TRT_S32 row,  /@ number of rows in the grid @/
*     TRT_S32 col   /@ number of columns in the grid @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a grid layout with specified rows and columns.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGridLayout
        (
        TRT_S32 row,
        TRT_S32 col
        );

/*******************************************************************************
*
* ~TGridLayout - destruct the grid layout
*
* This function destructs the grid layout instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TGridLayout();

/*******************************************************************************
*
* DoLayout - carry out the layout
*
* SYNOPSIS
* \ss
* void DoLayout
*     (
*     const TRect & rect    /@ bounding rectangle to conduct the layout @/
*     )
* \se
*
* DESCRIPTION
* This function conducts the layout against the specified rectangle.
* It updates the width of each column and height of each row in the grid.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void DoLayout
        (
        const TRect & rect
        );

/*******************************************************************************
*
* GetCellRect - calculate the placement of the specified cell(s)
*
* SYNOPSIS
* \ss
* TRect GetCellRect
*     (
*     const GridCell & cell     /@ cell of which to get placement information @/
*     )
* \se
*
* DESCRIPTION
* This function calculates the placement of the specified cell(s).
*
* RETURNS: the bounding rectangle of the specified cell(s)
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRect GetCellRect
        (
        const GridCell & cell
        ) const;

/*******************************************************************************
*
* SetRowSize - set the sizing method and value of row
*
* SYNOPSIS
* \ss
* void SetRowSize
*     (
*     TRT_S32 row,          /@ index of the row @/
*     LayoutMethod method,  /@ sizing method @/
*     TRT_S32 value         /@ size of the row if the <method> is LM_fixed.
*                              proportion of the row if the <method> is
*                              LM_proportion @/
*     )
* \se
*
* DESCRIPTION
* This function sets the sizing method of row.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetFixedRowSize(), SetProportionRowSize(),
*/
    void SetRowSize
        (
        TRT_S32 row,
        LayoutMethod method,
        TRT_S32 value
        )
        {
        if (row < 0 || row >= rows.Count())
            {
            return;
            }
        switch (method)
            {
            default:
            case LM_proportion:
                rows[row].proportion = value;
                break;
            case LM_fixed:
                rows[row].fix = value;
                break;
            }
        rows[row].method = method;
        }

/*******************************************************************************
*
* SetColumnSize - set the sizing method and value of column
*
* SYNOPSIS
* \ss
* void SetColumnSize
*     (
*     TRT_S32 col,          /@ index of the column @/
*     LayoutMethod method,  /@ sizing method @/
*     TRT_S32 value         /@ size of the column if the <method> is LM_fixed.
*                              proportion of the column if the <method> is
*                              LM_proportion @/
*     )
* \se
*
* DESCRIPTION
* This function sets the sizing method of column.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetFixedColumnSize(), SetProportionColumnSize(),
*/
    void SetColumnSize
        (
        TRT_S32 col,
        LayoutMethod method,
        TRT_S32 value
        )
        {
        if (col < 0 || col >= cols.Count())
            {
            return;
            }
        switch (method)
            {
            default:
            case LM_proportion:
                cols[col].proportion = value;
                break;
            case LM_fixed:
                cols[col].fix = value;
                break;
            }
        cols[col].method = method;
        }

/*******************************************************************************
*
* SetFixedRowSize - set the fixed size of row
*
* SYNOPSIS
* \ss
* void SetFixedRowSize
*     (
*     TRT_S32 row,          /@ index of the row @/
*     TRT_S32 value         /@ new fixed size of the row @/
*     )
* \se
*
* DESCRIPTION
* This function sets the fixed size of row. It is a short-cut call to
* SetRowSize() with sizing method to be LM_fixed. This function implicitly sets
* the row to use the LM_fixed sizing method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetRowSize(), SetProportionRowSize()
*/
    void SetFixedRowSize
        (
        TRT_S32 row,
        TRT_S32 value
        )
        {
        SetRowSize(row, LM_fixed, value);
        }

/*******************************************************************************
*
* SetFixedColumnSize - set the fixed size of column
*
* SYNOPSIS
* \ss
* void SetFixedColumnSize
*     (
*     TRT_S32 col,          /@ index of the column @/
*     TRT_S32 value         /@ new fixed size of the column @/
*     )
* \se
*
* DESCRIPTION
* This function sets the fixed size of column. It is a short-cut call to
* SetColumnSize() with sizing method to be LM_fixed. This function implicitly
* sets the column to use the LM_fixed sizing method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetColumnSize(), SetProportionColumnSize()
*/
    void SetFixedColumnSize
        (
        TRT_S32 col,
        TRT_S32 value
        )
        {
        SetColumnSize(col, LM_fixed, value);
        }

/*******************************************************************************
*
* SetProportionRowSize - set the proportion of row
*
* SYNOPSIS
* \ss
* void SetProportionRowSize
*     (
*     TRT_S32 row,          /@ index of the row @/
*     TRT_S32 value         /@ new proportional size of the row @/
*     )
* \se
*
* DESCRIPTION
* This function sets the proportion of row. It is a short-cut call to
* SetRowSize() with sizing method to be LM_proportion. This function implicitly
* sets the row to use the LM_proportion sizing method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetRowSize(), SetFixedColumnSize()
*/
    void SetProportionRowSize
        (
        TRT_S32 row,
        TRT_S32 value
        )
        {
        SetRowSize(row, LM_proportion, value);
        }

/*******************************************************************************
*
* SetProportionColumnSize - set the proportion of column
*
* SYNOPSIS
* \ss
* void SetProportionColumnSize
*     (
*     TRT_S32 col,          /@ index of the column @/
*     TRT_S32 value         /@ new proportional size of the column @/
*     )
* \se
*
* DESCRIPTION
* This function sets the proportion of column. It is a short-cut call to
* SetColumnSize() with sizing method to be LM_proportion. This function
* implicitly sets the column to use the LM_proportion sizing method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetColumnSize(), SetFixedColumnSize()
*/
    void SetProportionColumnSize
        (
        TRT_S32 col,
        TRT_S32 value
        )
        {
        SetColumnSize(col, LM_proportion, value);
        }

/*******************************************************************************
*
* SetSpacing - set the spacing among cells in the grid
*
* SYNOPSIS
* \ss
* void SetSpacing
*     (
*     const TMargin & value /@ the new spacing to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the spacing among cells in the grid.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetSpacing
*/
    void SetSpacing
        (
        const TMargin & value
        )
        {
        spacing = value;
        }

/*******************************************************************************
*
* GetSpacing - get the spacing among cells in the grid
*
* SYNOPSIS
* \ss
* const TMargin & GetSpacing() const
* \se
*
* DESCRIPTION
* This function gets the spacing among cells in the grid.
*
* RETURNS: spacing among cells in the grid
*
* ERRNO: N/A
*
* SEE ALSO: SetSpacing
*/
    const TMargin & GetSpacing() const
        {
        return spacing;
        }

private:
    TArray<LayoutInfo>  rows;
    TArray<LayoutInfo>  cols;
    TMargin             spacing;
    };

#endif /* _INCLUDE_tgsGridLayout_H_ */


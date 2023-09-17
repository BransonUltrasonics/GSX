/* tgsGridPanel.h - Tilcon grid panel class */

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
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's grid panel class.

TGridPanel is a container class which arranges the children layout in grids.

TGridPanel provides the AddControl(), AddControlAtTop(), AddControlAtBottom(),
RemoveControl() and RemoveAllControls() methods to manage add/remove the child.
TGridPanel contains a TGridLayout class to help it manage the layout; the grid
layout can be accessed via the GetLayout() method. See TGridLayout's reference
for more information on how to layout interface in grid.

INCLUDE FILES: tilcon/engine/tgsGridPanel.h
*/

#ifndef _INCLUDE_tgsGridPanel_H_
#define _INCLUDE_tgsGridPanel_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsGridLayout.h"


class TGridPanel : public TControl
    {
public:

/*******************************************************************************
*
* TGridPanel - construct a grid panel
*
* SYNOPSIS
* \ss
* TGridPanel
*     (
*     TRT_S32 x = 0,              /@ x position @/
*     TRT_S32 y = 0,              /@ y position @/
*     TRT_S32 w = 0,              /@ width @/
*     TRT_S32 h = 0,              /@ height @/
*     TRT_S32 row = 1,            /@ row number of the grid @/
*     TRT_S32 col = 1             /@ columns number of the grid @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a grid panel widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* The position and size of the grid panel;
* \i <row>, <col>
* row and column number of the grid panel;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGridPanel
        (
        TRT_S32 x = 0,
        TRT_S32 y = 0,
        TRT_S32 w = 0,
        TRT_S32 h = 0,
        TRT_S32 row = 1,
        TRT_S32 col = 1
        );

/*******************************************************************************
*
* ~TGridPanel - destruct the grid panel
*
* This function destructs a grid panel widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TGridPanel();

/*******************************************************************************
*
* AddControl - add child control at specified Z-order position
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControl
*     (
*     TControl *          child,  /@ child to be added @/
*     TRT_S32             index,  /@ z-order of the added child @/
*     const GridCell &    cell    /@ where to place the added child @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control at specified Z-order, and with specified
* placement information in the grid.
*
* Parameters:
* \is
* \i <child>
* child to be added;
* \i <index>
* the insertion position. Possible value for <index> is between 0 and the number
* of children in the container. insertion position of 0 means the child will be
* added at bottom most Z-order; insertion position of the children count means
* the child will be added at the top most Z-order (it is OK to pass -1 as index,
* the function treats -1 as the top most Z-order); insertion value in between
* means to add the child at other Z-order between the bottom most and top most.
* \i <cell>
* specify which cell(s) the newly added child will take, for example:
* GridCell(2, 3) means the child is placed in the grid at row 2 and column 3;
* GridCell(2, 3, 4, 5) means the child is placed beginning at row 2 column 3,
* and takes the next 4 consecutive rows and 5 consecutive columns.
* \ie
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: AddControlAtTop(), AddControlAtBottom(), RemoveControl(),
* RemoveAllControls()
*/
    TRT_STATUS AddControl(TControl * child, TRT_S32 index, const GridCell &);

/*******************************************************************************
*
* AddControlAtTop - add child control at top most Z-order
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControlAtTop
*     (
*     TControl *          child,  /@ child to be added @/
*     const GridCell &    cell    /@ where to place the added child @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control at top most Z-order, with specified
* placement information in the grid.
*
* Parameters:
* \is
* \i <child>
* child to be added;
* \i <cell>
* specify which cell(s) the newly added child will take, for example:
* GridCell(2, 3) means the child is placed in the grid at row 2 and column 3;
* GridCell(2, 3, 4, 5) means the child is placed beginning at row 2 column 3,
* and takes the next 4 consecutive rows and 5 consecutive columns.
* \ie
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtBottom(), RemoveControl(),
* RemoveAllControls()
*/
    TRT_STATUS AddControlAtTop(TControl * child, const GridCell &);

/*******************************************************************************
*
* AddControlAtBottom - add child control at bottom most Z-order
*
* SYNOPSIS
* \ss
* TRT_STATUS AddControlAtBottom
*     (
*     TControl *          child,  /@ child to be added @/
*     const GridCell &    cell    /@ where to place the added child @/
*     )
* \se
*
* DESCRIPTION
* This function adds child control at bottom most Z-order, with specified
* placement information in the grid.
*
* Parameters:
* \is
* \i <child>
* child to be added;
* \i <cell>
* specify which cell(s) the newly added child will take, for example:
* GridCell(2, 3) means the child is placed in the grid at row 2 and column 3;
* GridCell(2, 3, 4, 5) means the child is placed beginning at row 2 column 3,
* and takes the next 4 consecutive rows and 5 consecutive columns.
* \ie
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), RemoveControl(),
* RemoveAllControls()
*/
    TRT_STATUS AddControlAtBottom(TControl * child, const GridCell & cell);

/*******************************************************************************
*
* RemoveControl - remove a specific control from the container
*
* SYNOPSIS
* \ss
* void RemoveControl
*     (
*     TControl * child    /@ child to be removed @/
*     )
* \se
*
* DESCRIPTION
* This function removes a specific control from the container. if the specified
* control is not a child of the container, this function does nothing.
*
* Parameters:
* \is
* \i <child>
* control to be removed.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), AddControlAtBottom(),
* RemoveAllControls()
*/
    void RemoveControl(TControl *);

/*******************************************************************************
*
* RemoveAllControls - remove all child controls from the container
*
* SYNOPSIS
* \ss
* void RemoveAllControls()
* \se
*
* DESCRIPTION
* This function removes all child controls from the container.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), AddControlAtBottom(),
* RemoveControl()
*/
    void RemoveAllControls();

/*******************************************************************************
*
* Count - get the number of child controls in the container
*
* SYNOPSIS
* \ss
* TRT_S32 Count() const
* \se
*
* DESCRIPTION
* This function gets the number of child controls in the container.
*
* RETURNS: the number of child controls in the container
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 Count() const
        {
        return cells.Count();
        }

/*******************************************************************************
*
* operator - get the pointer to child control at specific Z-order index
*
* SYNOPSIS
* \ss
* TControl * operator []
*     (
*     TRT_S32 index   /@ Z-order index @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the pointer to child control at specific Z-order index.
* Z-order increases from screen towards outside, i.e., Z-order 0 means
* bottom-most, and Z-order of last child index means top-most.
*
* Parameters:
* \is
* \i <index>
* the Z-order index, valid value is between 0 and last child index;
* \ie
*
* RETURNS: pointer to child control; throws exception if index is out of range.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TControl * operator [] (TRT_S32 index) const
        {
        return cells[index].child;
        }

/*******************************************************************************
*
* IndexOf - get the Z-order index of specified child
*
* SYNOPSIS
* \ss
* TRT_S32 IndexOf
*     (
*     const TControl * child    /@ of which to get the index @/
*     ) const
* \se
*
* DESCRIPTION
* This function gets the Z-order index of specified child.
*
* Parameters:
* \is
* \i <child>
* the child whose Z-order index is to be retrieved;
* \ie
*
* RETURNS: the index of the child; if the child does not belong to this
* container, the return value is -1;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_S32 IndexOf(const TControl * child) const;

/*******************************************************************************
*
* SetChildGridCell - set the grid cell information for a child
*
* SYNOPSIS
* \ss
* TRT_STATUS SetChildGridCell
*     (
*     TControl * child,       /@ of which the cell is to be set @/
*     const GridCell & cell   /@ new cell information @/
*     )
* \se
*
* DESCRIPTION
* This function sets the grid cell information for a child.
*
* Parameters:
* \is
* \i <child>
* child whose placement is to be set;
* \i <cell>
* the new placement information for the child, for example:
* GridCell(2, 3) means the child is placed in the grid at row 2 and column 3;
* GridCell(2, 3, 4, 5) means the child is placed beginning at row 2 column 3,
* and takes the next 4 consecutive rows and 5 consecutive columns.
* \ie
*
* RETURNS: TRT_STATUS_ERROR if the specified control is not a child of the
* container; otherwise TRT_STATUS_OK;
*
* ERRNO: N/A
*
* SEE ALSO: AddControl(), AddControlAtTop(), AddControlAtBottom()
*/
    TRT_STATUS SetChildGridCell(TControl * child, const GridCell & cell);

/*******************************************************************************
*
* GetLayout - get the layout helper of the container
*
* SYNOPSIS
* \ss
* TGridLayout & GetLayout()
* \se
*
* DESCRIPTION
* This function gets the layout helper of the container. This function is used
* to access the layout helper so that customer is able to change layout
* attributes (row height, column width, spacing, etc.).
*
* RETURNS: reference to the TGridLayout instance;
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TGridLayout & GetLayout()
        {
        return layout;
        }

protected:
    TChildrenDelegate * GetBranchDelegate()
        {
        return &childrenDelegate;
        };

private:
    struct GridUnit
        {
        GridUnit() :
            child(0)
            {}
        GridUnit(TControl * c, const GridCell & g, const TRect & r) :
            child(c), cell(g), rect(r)
            {}
        TControl *  child;
        GridCell    cell;
        TRect       rect;
        };

    class GridChildrenDelegate : public TChildrenDelegate
        {
        friend class TGridPanel;
    public:
        GridChildrenDelegate(TGridPanel & p) :
            panel(p)
            {}
        virtual TRT_S32 GetCount() const;
        virtual TNode & GetNode(TRT_S32);

    private:
        TGridPanel & panel;
        };

    void NodeHandler(EventParam &);
    void OnResize();
    void OnChildBoundChange(TControl *);

    TNodeEventDelegate      eventDelegate;
    GridChildrenDelegate    childrenDelegate;
    TNode                   boundNode;
    TGridLayout             layout;
    TArray<GridUnit>        cells;
    };

#endif /* _INCLUDE_tgsGridPanel_H_ */


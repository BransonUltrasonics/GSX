/* tgsSpinLayout.h - spin layout header */

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

This file defines the spin layout.
*/


#ifndef _INCLUDE_tgsSpinLayout_H_
#define _INCLUDE_tgsSpinLayout_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsGridLayout.h"

class TSpinLayout : public TControl
    {
public:
    /* placement of spin button */
    enum ButtonPlacement
        {
        BP_horizontal = 0,
        BP_vertical
        };

public:
    TSpinLayout(TRT_S32 x, TRT_S32 y,
        TRT_S32 w, TRT_S32 h, TNode &, TNode &, TNode &);
    ~TSpinLayout();

protected:
    void NodeHandler(EventParam &);
    void ResizeHandler(EventParam &);

    TRT_BOOL                around;

    ButtonPlacement         buttonPlacement;
    TRT_S32                 buttonWidth;

    TNodeEventDelegate      eventDelegate;
    TGridLayout             layout;
    TNodeArray              boundNode;
    TNodeArray              clientNode;
    TNode &                 editNode;
    TNode &                 upButtonNode;
    TNode &                 downButtonNode;
    };


#endif /* _INCLUDE_tgsSpinLayout_H_ */


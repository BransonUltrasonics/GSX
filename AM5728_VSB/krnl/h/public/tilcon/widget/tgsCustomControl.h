/* tgsCustomControl.h - custom control header */

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

This file defines the TCustomControl class.
*/


#ifndef _INCLUDE_tgsCustomControl_H_
#define _INCLUDE_tgsCustomControl_H_

#include "tilcon/engine/tgsControl.h"

class TCustomControl : public TControl, private TEventDelegate
    {
public:
/*******************************************************************************
*
* TCustomControl - construct a custom widget
*
* SYNOPSIS
* \ss
* TCustomControl
*     (
*     TRT_S32 x = 0,            /@ x position @/
*     TRT_S32 y = 0,            /@ y position @/
*     TRT_S32 w = 0,            /@ width @/
*     TRT_S32 h = 0             /@ height @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a custom widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the custom widget;
* \i <image>
* TImage to be shown;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TCustomControl(TRT_S32 x = 0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0);

/*******************************************************************************
*
* ~TCustomControl - destructs the custom widget
*
* This function destructs the custom widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    virtual ~TCustomControl();

    TEvent & ResizeEvent()
        {
        return resizeEvent;
        }

    TEvent & PaintEvent()
        {
        return paintEvent;
        }
    TEvent & MouseEvent()
        {
        return mouseEvent;
        }
    TEvent & KeyboardEvent()
        {
        return keyboardEvent;
        }
    TNode & GetBoundNode()
        {
        return boundNode;
        }

private:

    TEvent  mouseEvent;
    TEvent  keyboardEvent;
    TEvent  paintEvent;
    TEvent  resizeEvent;
    TEvent  attachEvent;
    TEvent  detachEvent;
    TNode   boundNode;

    virtual void OnEvent(EventParam &);
    };

#endif /* _INCLUDE_tgsCustomControl_H_ */


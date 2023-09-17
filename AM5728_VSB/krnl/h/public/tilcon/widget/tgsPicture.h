/* tgsPicture.h - Tilcon picture widget */

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
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines Tilcon's picture widget.

The picture which is used to show static images. The image content is set at
the construction time, or set by the SetImage() method.

INCLUDE FILES: tilcon/widget/tgsPicture.h
*/

#ifndef _INCLUDE_tgsPicture_H_
#define _INCLUDE_tgsPicture_H_

#include "tilcon/engine/tgsControl.h"


class TPicture : public TControl
    {
public:
/*******************************************************************************
*
* TPicture - construct a picture widget
*
* SYNOPSIS
* \ss
* TPicture
*     (
*     TRT_S32 x = 0,            /@ x position @/
*     TRT_S32 y = 0,            /@ y position @/
*     TRT_S32 w = 0,            /@ width @/
*     TRT_S32 h = 0,            /@ height @/
*     const TImage & image      /@ image @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a picture widget.
*
* Parameters:
* \is
* \i <x>, <y>, <w>, <h>
* the position and size of the picture widget;
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
    TPicture(TRT_S32 x = 0, TRT_S32 y = 0, TRT_S32 w = 0, TRT_S32 h = 0,
        const TImage & image = TImage());

/*******************************************************************************
*
* ~TPicture - destructs the picture widget
*
* This function destructs the picture widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TPicture();

/*******************************************************************************
*
* SetImage - set the image content of the picture widget
*
* SYNOPSIS
* \ss
* void SetImage
*     (
*     const TImage & image /@ new image to show @/
*     )
* \se
*
* DESCRIPTION
* This function sets the image content of the picture widget.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetImage()
*/
    void SetImage(const TImage &);

/*******************************************************************************
*
* GetImage - get the image content of the picture widget
*
* SYNOPSIS
* \ss
* const TImage & GetImage() const;
* \se
*
* DESCRIPTION
* This function gets the image content of the picture widget.
*
* RETURNS: the image content of the picture widget
*
* ERRNO: N/A
*
* SEE ALSO: SetImage()
*/
    const TImage & GetImage() const;

private:
    void NodeHandler(EventParam &);
    void PaintHandler(EventParam &);

    TNodeEventDelegate  eventDelegate;
    TNode               boundNode;
    TRT_BOOL            showBackground;
    TImage              image;
    };

#endif /* _INCLUDE_tgsPicture_H_ */

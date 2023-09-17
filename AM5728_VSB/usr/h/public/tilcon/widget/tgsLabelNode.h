/* tgsLabelNode.h - label node header */

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


#ifndef _INCLUDE_tgsLabelNode_H_
#define _INCLUDE_tgsLabelNode_H_

#include "tilcon/engine/tgsControl.h"

/* push button class */
class TLabelNode : public TNode
    {
public:
    TLabelNode(TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h, const TWString &, TMargin &, TVisual &, TextProperty &);
    virtual ~TLabelNode();

    void SetText(const TWString & value)
        {
        text = value;
        Update();
        }

    const TWString & GetText() const
        {
        return text;
        }

private:
    void NodeHandler(EventParam &);
    TNodeEventDelegate  eventDelegate;

    TWString            text;

    TMargin &           textMargin;
    TextProperty &      textProperty;
    TVisual &           labelVisual;
    };

#endif /* _INCLUDE_tgsLabelNode_H_ */


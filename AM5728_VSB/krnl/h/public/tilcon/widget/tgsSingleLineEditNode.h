/* tgsSingleLineEditNode.h - single line edit node header */

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
15may14,yya fix V7GFX-5
01jan14,yya created (US15550)
*/

/*
DESCRIPTION

This file defines the single line edit node.
*/


#ifndef _INCLUDE_tgsSingleLineEditNode_H_
#define _INCLUDE_tgsSingleLineEditNode_H_

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsTimer.h"

class TEditNode : public TNodeContainer
    {
public:
    TEditNode(TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32,
        TMargin & _margin, TVisual & _boundVisual, TextProperty & _textProperty,
        const TWString & wstr = L"");
    virtual ~TEditNode();

    void SetText(const TWString & value);
    const TWString & GetText() const;
    void ClearText();

    void SetAllowEdit(TRT_BOOL value)
        {
        allowEdit = value;
        }

    TRT_BOOL GetAllowEdit() const
        {
        return allowEdit;
        }

    void SetMaxTextLength(TRT_S32 value);
    TRT_S32 GetMaxTextLength() const
        {
        return maxTextLen;
        }

private:
    struct SelectInfo
        {
        TRT_S32 start;
        TRT_S32 end;
        TRT_S32 startPos;
        TRT_S32 endPos;
        TRT_BOOL cursorOn;
        };
    void NodeHandler(EventParam &);
    void CursorTimerHandler(EventParam &);
    void DragTimerHandler(EventParam &);

    void TextResizeHandler(EventParam &);
    void TextPaintHandler(EventParam &);
    void TextMouseHandler(EventParam &);
    void KeyboardHandler(EventParam &);

    void GainFocusHandler(EventParam &);
    void LoseFocusHandler(EventParam &);

    void DrawBlinkingCursor(TPainter & painter) const;
    void MoveCursorIntoView();
    void ResetSelInfo();

    void SelectAll();
    void AddChar(TRT_WCHAR);
    void Backspace();
    void Delete();
    void MoveCursorHome();
    void MoveCursorEnd();
    void MoveCursorLeft();
    void MoveCursorRight();

    void LayoutText();

    TTimer              cursorTimer;
    TTimer              dragTimer;

    TNodeEventDelegate  eventDelegate;
    TNodeContainer      textViewBound;
    TNode               textView;
    TNode               textControl;

    TWString            text;
    TRT_S32             maxTextLen;
    TRT_BOOL            allowEdit;

    TPoint              mousePt;
    TRT_BOOL            inDrag;
    SelectInfo          selectInfo;

    TMargin &           margin;
    TVisual &           frameVisual;
    TextProperty &      textProperty;
    };

#endif /* _INCLUDE_tgsSingleLineEditNode_H_ */


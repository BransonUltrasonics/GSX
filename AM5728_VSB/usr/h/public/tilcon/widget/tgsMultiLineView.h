/* tgsMultiLineView.h - multiline view header */

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

This file defines the multiline view.
*/


#ifndef _INCLUDE_tgsMultiLineView_H_
#define _INCLUDE_tgsMultiLineView_H_

#include "tilcon/widget/tgsScrollHelper.h"
#include "tilcon/widget/tgsScrollBarPair.h"

#define MOUSE_DRAG_HOR_MASK     0xf0
#define MOUSE_DRAG_VER_MASK     0x0f

#define MOUSE_DRAG_DIR_UP       0x01
#define MOUSE_DRAG_DIR_DOWN     0x02
#define MOUSE_DRAG_DIR_LEFT     0x10
#define MOUSE_DRAG_DIR_RIGHT    0x20


class TTextDataAdaptor
    {
public:
    TTextDataAdaptor(){}
    virtual ~TTextDataAdaptor(){};

    virtual TRT_S32 Count() const = 0;
    virtual const TWString & Item(TRT_S32) const = 0;
    };

class TMultiLineView : public TScrollBarPair
    {
    friend class TMultiLineEdit;
public:
    enum WrapMode
        {
        WM_none = 0,
        WM_after_letter,
        WM_after_word
        };

    struct MultiLineProperty : public ControlProperty
        {
        MultiLineProperty();
        ~MultiLineProperty();
        };

private:
    TMultiLineView(TRT_S32, TRT_S32, TRT_S32, TRT_S32,
        TMargin &, TVisual &, TextProperty &, TTextDataAdaptor *);
    virtual ~TMultiLineView();

    TEvent scrollContentChangeEvent;
    TEvent scrollBarChangeEvent;

    struct LineInfo
        {
        TRT_S32 startRow;
        TRT_S32 endRow;
        };
    struct RowInfo
        {
        TRT_S32 lineNumber;
        TRT_S32 startChar;
        TRT_S32 len;
        TRT_S32 width;
        };
    struct SelectInfo
        {
        TRT_S32 startRow;
        TRT_S32 startCol;
        TRT_S32 endRow;
        TRT_S32 endCol;

        TRT_S32 startLineIndex;
        TRT_S32 startLineOffset;
        TRT_S32 endLineIndex;
        TRT_S32 endLineOffset;

        TRT_S32 startColPos;
        TRT_S32 endColPos;

        TRT_S32 cursorOn;
        };

    TTextDataAdaptor * GetListData()
        {
        return listData;
        }
    void SetWrapMode(WrapMode newWrapMode);

private:

    TRT_S32 GetHighlightStartRow(const SelectInfo & info) const
        {
        return info.startRow < info.endRow ? info.startRow : info.endRow;
        }

    TRT_S32 GetHighlightEndRow(const SelectInfo & info) const
        {
        return info.startRow < info.endRow ? info.endRow : info.startRow;
        }

    TRT_S32 GetHighlightStartCol(const SelectInfo & info) const
        {
        if (info.startRow < info.endRow)
            {
            return info.startCol;
            }
        else if (info.startRow > info.endRow)
            {
            return info.endCol;
            }
        else
            {
            return info.startCol < info.endCol ? info.startCol : info.endCol;
            }
        }

    TRT_S32 GetHighlightEndCol(const SelectInfo & info) const
        {
        if (info.startRow < info.endRow)
            {
            return info.endCol;
            }
        else if (info.startRow > info.endRow)
            {
            return info.startCol;
            }
        else
            {
            return info.startCol < info.endCol ? info.endCol : info.startCol;
            }
        }

    void ClearSelectInfo(SelectInfo & info)
        {
        memset(&info, 0, sizeof(SelectInfo));
        }

    void ResetSelection()
        {
        ClearSelectInfo(selectInfo);
        }
    void ResetEditPosition()
        {
        scrollHelper.XScrollTo(0, &scrollBarChangeEvent);
        scrollHelper.YScrollTo(0, &scrollBarChangeEvent);
        }

    void NodeHandler(EventParam &);
    void HScrollHandler(EventParam &);
    void VScrollHandler(EventParam &);

    void CursorTimerHandler(EventParam &);
    void DragTimerHandler(EventParam &);
    void TScrollViewportResizeHandler(EventParam &);

    void ScrollContentChangeHandler(EventParam &);
    void ScrollBarChangeHandler(EventParam &);

    void EditPaintHandler(EventParam &);
    void EditMouseMsgHandler(EventParam &);
    void ListPaintHandler(EventParam &);
    void ListMouseMsgHandler(EventParam &);

    void GainFocusHandler(EventParam &);
    void LoseFocusHandler(EventParam &);
    void MouseLeaveHandler(EventParam &);

    void SetEditSize(const TSize &);
    void UpdateContent();
    void ParseAll();

    void OnLineAdded(TRT_S32, TRT_S32);
    void OnLineRemoved(TRT_S32, TRT_S32);

    void ConvertLineToRow(TRT_S32 lineIndex, TRT_S32 lineOffset, TRT_S32 & rowIndex, TRT_S32 & rowOffset);
    void ConvertRowToLine(TRT_S32 rowIndex, TRT_S32 rowOffset, TRT_S32 & lineIndex, TRT_S32 & lineOffset);

    void ScrollTextView(TRT_S32, TRT_S32);
    void ConvertPosToRowCol(const TPoint & pt, TRT_S32 & row, TRT_S32 & col, TRT_S32 & actualWidth);
    void MoveCursorIntoView();

    void DrawBlinkingCursor(TPainter & painter) const;
    void SetTextAreaSize(const TSize & size);
    void RemoveLineAndRowInfo(TRT_S32, TRT_S32);
    void AddLineAndRowInfo(TRT_S32, const TArray<LineInfo> &, const TArray<RowInfo> &);
    void ParseLineIntoRows(const TWString & string, LineInfo & newLine, TArray<RowInfo> & newRows);

    TArray<LineInfo>    lineInfoList;
    TArray<RowInfo>     rowInfoList;
    SelectInfo          selectInfo;

    TRT_S32             numberOfVisiableLines;
    TRT_S32             firstVisibleLine;
    TRT_S32             mouseHoverIndex;
    TRT_S32             lineHeight;

    TPoint              mousePt;

    TTextDataAdaptor *  listData;

    /* property */

    TRT_S32             rowSpace;
    WrapMode            wrapMode;
    TextProperty &      textProperty;

    /* nodes */

    TTimer              cursorTimer;
    TTimer              dragTimer;

    TNodeEventDelegate  eventDelegate;
    TNodeArray          viewport;
    TNode               edit;
    TScrollHelper       scrollHelper;
    };
#endif /* _INCLUDE_tgsMultiLineView_H_ */


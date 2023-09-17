/* tgsMultiLineEdit.h - multiline edit implementation */

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

This file defines the multiline edit widget.
*/


#ifndef _INCLUDE_tgsMultiLineEdit_H_
#define _INCLUDE_tgsMultiLineEdit_H_

#include "tilcon/widget/tgsMultiLineView.h"

class TMultiLineEdit : public TControl
    {
private:
    class TMultiLineData : public TTextDataAdaptor
        {
        friend class TMultiLineEdit;
    public:
        TMultiLineData();
        virtual ~TMultiLineData();

        virtual TRT_S32 Count() const;
        virtual const TWString & Item(TRT_S32) const;
    private:
        TArray<TWString> content;
        };

public:
    TMultiLineEdit(TRT_S32, TRT_S32, TRT_S32, TRT_S32);
    virtual ~TMultiLineEdit();

    void SetWrapMode(TMultiLineView::WrapMode mode)
        {
        view.SetWrapMode(mode);
        }

    void SetContent(TRT_LPCSTR);
    void AddLine(const TWString &);
    void ClearContent();
    void UpdateContent();
    void SetFont(const TFont &);

private:
    TMultiLineView  view;
    TMultiLineData  data;
    };

#endif /* _INCLUDE_tgsMultiLineEdit_H_ */


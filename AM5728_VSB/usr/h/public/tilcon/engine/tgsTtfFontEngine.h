/* tgsFontEngine.h - tilcon font engine */

/*
 * Copyright (c) 2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
15jan15,yya  created
*/

#ifndef _INCLUDE_tgsTtfFont_H_
#define _INCLUDE_tgsTtfFont_H_

#include "tilcon/engine/tgsFont.h"
#include "tilcon/engine/tgsImage.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

class TTtfFontEngine
    {
public:
    TTtfFontEngine();
    ~TTtfFontEngine();

    TRT_STATUS RegisterTtfFont(const TTF_FONT_DESC &);
    TRT_STATUS RegisterBmfFont(const UGL_BMF_FONT_DESC &);

    TRT_HANDLE AcquireFont(const TString &);
    void ReleaseFont(TRT_HANDLE);

    TRT_FLOAT GetHeight(TRT_HANDLE handle) const;
    void GetMetrics(TRT_HANDLE handle, FontMetrics & m) const;
    TRT_STATUS GetGlyphImage(TRT_HANDLE, TRT_WCHAR, TImage &) const;
    TRT_STATUS GetGlyphAdvance(TRT_HANDLE, TRT_WCHAR, TPointF &) const;
    TRT_STATUS GetGlyphPath(TRT_HANDLE, TRT_WCHAR, TArray<TRT_BYTE> &, TArray<TRT_FLOAT> &, TPointF &) const;

private:

    struct FONT
        {
        FONT(const TString & _name) :
            name(_name),
            ascender(0),
            descender(0),
            height(0),
            face(0)
            {}

        TString     name;
        TRT_S32     ascender;
        TRT_S32     descender;
        TRT_S32     height;

        FT_Face     face;
        };

    struct FuncParam
        {
        FuncParam(TArray<TRT_BYTE> & _cmds, TArray<TRT_FLOAT> & _coords) :
            cmds(_cmds), coords(_coords)
            {}
        TArray<TRT_BYTE>    & cmds;
        TArray<TRT_FLOAT>   & coords;
        };
    static int  AddCubicTo(const FT_Vector*, const FT_Vector*, const FT_Vector*, FuncParam*);
    static int  AddLineTo(const FT_Vector*, FuncParam*);
    static int  AddMoveTo(const FT_Vector*, FuncParam*);
    static int  AddQuadTo(const FT_Vector*, const FT_Vector*, FuncParam*);
    static const FT_Outline_Funcs outlineFuncs;

    FONT * CreateTtfFont(const TTF_FONT_DESC &);
    void DestroyTtfFont(FONT *);

    TArray<FONT *>  fonts;

    /* freetype2 */
    FT_Library  library;
    };

#endif /* _INCLUDE_tgsTtfFont_H_ */

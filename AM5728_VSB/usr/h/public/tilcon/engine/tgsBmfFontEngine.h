/* tgsBmfFontEngine.h - tilcon BMF font engine */

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

#ifndef _INCLUDE_tgsBmfFontEngine_H_
#define _INCLUDE_tgsBmfFontEngine_H_

#include "tilcon/engine/tgsFont.h"


class TBmfFontEngine
    {
public:
    TBmfFontEngine();
    ~TBmfFontEngine();

    TRT_STATUS RegisterBmfFont(const UGL_BMF_FONT_DESC &);

    TRT_HANDLE AcquireFont(const TString &, TRT_S32);
    void ReleaseFont(TRT_HANDLE);

    void GetMetrics(TRT_HANDLE handle, FontMetrics & m) const;
    const TBmfFont::Glyph * GetGlyph(TRT_HANDLE, TRT_WCHAR) const;
    TRT_STATUS GetGlyphAdvance(TRT_HANDLE, TRT_WCHAR, TPoint &) const;

private:

    struct Font
        {
        struct GlyphPage
            {
            const TBmfFont::Glyph *   glyphs[256];
            };

        struct PageTable
            {
            GlyphPage * pages[256];
            };

        Font(const UGL_BMF_FONT_DESC &);
        ~Font();

        TString             name;
        TRT_S32             size;
        TRT_S32             ascender;
        TRT_S32             descender;
        PageTable           pageTable;
        };

    Font * CreateBmfFont();
    void DestroyBmfFont(Font *);

    TArray<Font *>  fonts;
    };

#endif /* _INCLUDE_tgsBmfFontEngine_H_ */

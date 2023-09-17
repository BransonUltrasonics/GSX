/* tgsTextUtility.h - Tilcon text utility */

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

This file defines the Tilcon's text utility.

INCLUDE FILES: tilcon/engine/tgsTextUtility.h
*/

#ifndef _INCLUDE_textUtility_H_
#define _INCLUDE_textUtility_H_

#include "tilcon/tilcon.h"

class TFont;

class LineInfo
    {
 public:
    TRT_S32 start;
    TRT_S32 len;
    LineInfo()
        {
        start = 0;
        len = 0;
        }
    LineInfo(TRT_S32 _start, TRT_S32 _len)
        {
        start = _start;
        len = _len;
        }
    };

TRT_S32 FindParagraphEnd(const TWString & string, TRT_S32 index);
TRT_S32 IndexToLine(const TArray<LineInfo> & lineList, TRT_S32 index);
TRT_S32 GetLineEndIndex(const TFont & font, const TWString & string, TRT_S32 start, TRT_S32 lineWidth, TRT_S32 startLen);
void ParseTextIntoLines(const TFont & font, const TWString & string, TRT_S32 start, TRT_S32 end, TRT_S32 lineWidth, TRT_S32 startLen, TArray<LineInfo> & lineList);
TRT_S32 TextWidthToIndex(const TFont & font, TRT_LPCWSTR text, TRT_S32 maxLen, TRT_S32 width, TRT_S32 & actualWidth);
void AnsiToWide(TRT_WCHAR * dst, const TRT_CHAR * src, TRT_S32 len);

#endif /* _INCLUDE_textUtility_H_ */


/* tgsColor.h - tilcon color header */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
14mar16,jnl  support window surface (F6570)
23oct15,c_l  IA64 warning reduction
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created
*/

/*
DESCRIPTION

This file defines the tilcon's color class.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tgsColor_H_
#define _INCLUDE_tgsColor_H_

/* color */

#define TRT_MAKE_ARGB(a, r, g, b)           ((a << 24) | (r <<16) | (g<<8) | b)
#define TRT_MAKE_RGB(r, g, b)               TRT_MAKE_ARGB(0xff, r, g, b)
#define TRT_GET_ALPHA(c)                    ((c>>24) & 0x000000ff)
#define TRT_GET_RED(c)                      ((c>>16) & 0x000000ff)
#define TRT_GET_GREEN(c)                    ((c>>8) & 0x000000ff)
#define TRT_GET_BLUE(c)                     (c & 0xff)

#define TRT_COLOR_WHITE                     TRT_MAKE_RGB(0xff, 0xff, 0xff)
#define TRT_COLOR_GREY                      TRT_MAKE_RGB(0x80, 0x80, 0x80)
#define TRT_COLOR_BLACK                     TRT_MAKE_RGB(0, 0, 0)
#define TRT_COLOR_RED                       TRT_MAKE_RGB(0xff, 0, 0)
#define TRT_COLOR_GREEN                     TRT_MAKE_RGB(0, 0xff, 0)
#define TRT_COLOR_BLUE                      TRT_MAKE_RGB(0, 0, 0xff)

#define TRT_COLOR_HIGHLIGHT                 TRT_MAKE_RGB(0x20, 0x20, 0x80)

#define TRT_COLOR_FRAME_FILL                TRT_MAKE_RGB(0xE0, 0xE0, 0xE0)
#define TRT_COLOR_FRAME_FILL_FOCUSED        TRT_MAKE_RGB(0xC0, 0xC0, 0xC0)
#define TRT_COLOR_FRAME_FILL_HIGHLIGHTED    TRT_MAKE_RGB(0x40, 0x80, 0xF0)
#define TRT_COLOR_FRAME_FILL_ACTIVE         TRT_MAKE_RGB(0xA0, 0xA0, 0xA0)
#define TRT_COLOR_FRAME_FILL_DIMMED         TRT_MAKE_RGB(0x80, 0x80, 0x80)

#define TRT_COLOR_FIELD_HIGHLIGHTED         TRT_MAKE_RGB(0x40, 0x80, 0xF0)
#define TRT_COLOR_FIELD_ACTIVE              TRT_MAKE_RGB(0xA0, 0xA0, 0xA0)

#define TRT_COLOR_SCROLL_TRACK              TRT_MAKE_RGB(0xF0, 0xF0, 0xF0)

#define TRT_COLOR_BORDER_3D1TONE_BRIGHT     TRT_MAKE_RGB(0xC0, 0xC0, 0xC0)
#define TRT_COLOR_BORDER_3D2TONE_BRIGHT     TRT_MAKE_RGB(0xF0, 0xF0, 0xF0)

#define TRT_COLOR_BORDER_3D1TONE_DULL       TRT_MAKE_RGB(0x80, 0x80, 0x80)
#define TRT_COLOR_BORDER_3D2TONE_DULL       TRT_MAKE_RGB(0x40, 0x40, 0x40)

#define TRT_COLOR_TEXT                      TRT_COLOR_BLACK
#define TRT_COLOR_TEXT_BK                   TRT_COLOR_WHITE
#define TRT_COLOR_TEXT_HIGHLIGHT            TRT_COLOR_WHITE
#define TRT_COLOR_TEXT_BK_HIGHLIGHT         TRT_COLOR_HIGHLIGHT

enum ColorOrder
    {
    /* 24bit */
    CO_R8G8B8 = 0,
    CO_B8G8R8,

    /* 32bit */
    CO_A8R8G8B8,
    CO_B8G8R8A8
    };

enum ColorFormat
    {
    CF_XRGB8888 = 0,
    CF_ARGB8888
    };

class TColor
    {
public:
    TColor() : blue(0), green(0), red(0), alpha(0xff)
        {}
    TColor(TRT_U8 _red, TRT_U8 _green, TRT_U8 _blue) :
        blue(_blue), green(_green), red(_red), alpha(0xff)
        {}
    TColor(TRT_U8 _red, TRT_U8 _green, TRT_U8 _blue, TRT_U8 _alpha) :
        blue(_blue), green(_green), red(_red), alpha(_alpha)
        {}
    TColor(TRT_U32 color) : blue((TRT_UCHAR)(TRT_GET_BLUE(color))), green((TRT_UCHAR)(TRT_GET_GREEN(color))),
        red((TRT_UCHAR)(TRT_GET_RED(color))), alpha((TRT_UCHAR)(TRT_GET_ALPHA(color)))
        {}
    TColor(const TColor & color) : blue(color.blue), green(color.green),
        red(color.red), alpha(color.alpha)
        {}
    ~TColor()
        {}

    TColor Invert() const
        {
        return TColor((TRT_UCHAR)(~red), (TRT_UCHAR)(~green), (TRT_UCHAR)(~blue), (TRT_UCHAR)(alpha));
        }

    TRT_BOOL operator == (const TColor & c) const
        {
        return (red == c.red && green == c.green &&
                blue == c.blue && alpha == c.alpha);
        }

    TRT_BOOL operator != (const TColor & c) const
        {
        return (red != c.red || green != c.green ||
                blue != c.blue || alpha != c.alpha);
        }

    TRT_BOOL IsOpague() const {return alpha == 0xff;}

    void Interpolate(TRT_FLOAT, const TColor &);

    void Blend(const TColor &);
    void Blend(TRT_S32, const TColor &);
    void Blend256(TRT_S32, const TColor &);

    TRT_U32 Pack32(ColorFormat) const;
    void Unpack32(TRT_U32, ColorFormat);

    TRT_UCHAR Alpha() const {return alpha;}
    TRT_UCHAR Red() const {return red;}
    TRT_UCHAR Green() const {return green;}
    TRT_UCHAR Blue() const {return blue;}

    TRT_FLOAT AlphaF() const {return (TRT_FLOAT)alpha/255.0f;}
    TRT_FLOAT RedF() const {return (TRT_FLOAT)red/255.0f;}
    TRT_FLOAT GreenF() const {return (TRT_FLOAT)green/255.0f;}
    TRT_FLOAT BlueF() const {return (TRT_FLOAT)blue/255.0f;}

    static TColor Fade256(TRT_S32 alpha, const TColor &);

private:

    TRT_UCHAR blue;
    TRT_UCHAR green;
    TRT_UCHAR red;
    TRT_UCHAR alpha;
    };

#endif /* _INCLUDE_tgsColor_H_ */

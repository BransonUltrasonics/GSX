/* tgsTypes.h - tilcon data type definition */

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
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION
This file defines the tilcon's data types.
*/

#ifndef _INCLUDE_tgsTypes_H_
#define _INCLUDE_tgsTypes_H_

struct ColorFormatDescriptor
    {
    TRT_U8 bpp;

    TRT_U8 alphaBits;
    TRT_U8 redBits;
    TRT_U8 greenBits;
    TRT_U8 blueBits;

    TRT_U8 alphaOffset;
    TRT_U8 redOffset;
    TRT_U8 greenOffset;
    TRT_U8 blueOffset;
    };

#define TRT_APP_MSG_UNKNOWN     (0)
#define TRT_APP_MSG_PTR         (1)
#define TRT_APP_MSG_KBD         (2)
#define TRT_APP_MSG_TIMER       (3)
#define TRT_APP_MSG_QUIT        (4)
#define TRT_APP_MSG_CALLBACK    (5)

typedef void (*CallbackFunc)(TRT_PVOID);

typedef struct _trt_drag_msg
    {
    TRT_S32     ox;
    TRT_S32     oy;
    DragState   state;
    } TRT_DRAG_MSG;

typedef struct _trt_mouse_msg
    {
    TRT_S32 x;
    TRT_S32 y;

    union
        {
        struct
            {
            TRT_U8 lbdown   : 1;
            TRT_U8 mbdown   : 1;
            TRT_U8 rbdown   : 1;
            TRT_U8 lbup     : 1;
            TRT_U8 mbup     : 1;
            TRT_U8 rbup     : 1;
            } bits;
        TRT_U8 value;
        } event;

    union
        {
        struct
            {
            TRT_U8 lbutton  : 1;
            TRT_U8 mbutton  : 1;
            TRT_U8 rbutton  : 1;
            TRT_U8 shift    : 1;
            TRT_U8 ctrl     : 1;
            TRT_U8 alt      : 1;
            } bits;
        TRT_U8  value;
        } state;
    } TRT_MOUSE_MSG;

typedef struct _trt_keyboard_msg
    {
    TRT_U16 code;       /* key code */
    TRT_U8  modifier;   /* modifier(ctrl, shift, alt) */
    } TRT_KEYBOARD_MSG;

typedef struct _trt_timer_msg
    {
    TRT_S32 id;         /* timer id */
    TRT_S32 stamp;      /* time stamp in ms */
    TRT_S32 stamp0;     /* initial time stamp in ms */
    } TRT_TIMER_MSG;

typedef struct _trt_quit_msg
    {
    TRT_S32 code;       /* exit code */
    } TRT_QUIT_MSG;

typedef struct _trt_callback_msg
    {
    CallbackFunc    func;
    TRT_PVOID       param;
    } TRT_CALLBACK_MSG;

typedef struct _app_msg
    {
    TRT_S32 type;

    union CONTENT
        {
        TRT_MOUSE_MSG       mouse;
        TRT_KEYBOARD_MSG    keyboard;
        TRT_TIMER_MSG       timer;
        TRT_QUIT_MSG        quit;
        TRT_CALLBACK_MSG    callback;
        } content;
    } TRT_APP_MSG;

/* prototype of keyboard hook function */

struct EventParam
    {
    };

struct ControlProperty
    {
    ControlProperty(ControlType);
    ~ControlProperty();

    ControlType Type() const
        {
        return type;
        }
private:
    ControlType     type;
    };

/* used to place widget in containers */
struct AlignmentPolicy
    {
    AlignmentPolicy() : ha(HA_fixed), va(VA_fixed)
        {}
    AlignmentPolicy(HorizontalAlignment h, VerticalAlignment v) : ha(h), va(v)
        {}
    HorizontalAlignment   ha;
    VerticalAlignment     va;
    };

/* visual state */
struct VisualState
    {
    TRT_U8  focused         : 1; /* edit focus */
    TRT_U8  dimmed          : 1; /* disabled */
    TRT_U8  highlighted     : 1; /* mouse hover */
    TRT_U8  active          : 1; /* active */
    TRT_U8  f_mask          : 1;
    TRT_U8  d_mask          : 1;
    TRT_U8  h_mask          : 1;
    TRT_U8  a_mask          : 1;
    };

struct GridCell
    {
    TRT_S32 startRow;
    TRT_S32 startCol;
    TRT_S32 rowSize;
    TRT_S32 colSize;

    GridCell() :
        startRow(0),
        startCol(0),
        rowSize(1),
        colSize(1)
        {
        }

    GridCell(TRT_S32 _startRow, TRT_S32 _startCol) :
        startRow(_startRow),
        startCol(_startCol),
        rowSize(1),
        colSize(1)
        {
        }

    GridCell(TRT_S32 _startRow, TRT_S32 _startCol, TRT_S32 _rowSize, TRT_S32 _colSize) :
        startRow(_startRow),
        startCol(_startCol),
        rowSize(_rowSize),
        colSize(_colSize)
        {
        }
    };

struct FontMetrics
    {
    TRT_S32 ascender;
    TRT_S32 descender;
    TRT_S32 height;
    };

/* font */

/* following definition is copied from WindML's UGL Font definition,
 * in order to be able to read the BMF font source file */

#define UGL_FONT_BOLD_OFF       0
#define UGL_FONT_BOLD_MIN       1
#define UGL_FONT_BOLD_LIGHT     25
#define UGL_FONT_BOLD           50
#define UGL_FONT_BOLD_HEAVY     75
#define UGL_FONT_BOLD_MAX       100

#define UGL_FONT_UPRIGHT        0
#define UGL_FONT_ITALIC         1

#define UGL_FONT_MONO_SPACED    0
#define UGL_FONT_PROPORTIONAL   1

#define UGL_FONT_SERIF          0
#define UGL_FONT_SANS_SERIF     1

#define UGL_FONT_BITMAPPED      0
#define UGL_FONT_TRUE_TYPE      1
#define UGL_FONT_FCO            2

/* Encoding code below adopted from freetype.h from the FreeType project */

#define UGL_FONT_CHARSET_TAG( value, a, b, c, d )   \
          value = ( ( (unsigned int)(a) << 24 ) |     \
                    ( (unsigned int)(b) << 16 ) |     \
                    ( (unsigned int)(c) <<  8 ) |     \
                      (unsigned int)(d)         )


typedef enum  ugl_font_charset
  {
    UGL_FONT_CHARSET_TAG( UGL_FONT_NONE, 0, 0, 0, 0 ),

    UGL_FONT_CHARSET_TAG( UGL_FONT_MS_SYMBOL,  's', 'y', 'm', 'b' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_UNICODE,    'u', 'n', 'i', 'c' ),

    UGL_FONT_CHARSET_TAG( UGL_FONT_SJIS,    's', 'j', 'i', 's' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_GB2312,  'g', 'b', ' ', ' ' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_BIG5,    'b', 'i', 'g', '5' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_WANSUNG, 'w', 'a', 'n', 's' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_JOHAB,   'j', 'o', 'h', 'a' ),

    UGL_FONT_CHARSET_TAG( UGL_FONT_ADOBE_STANDARD, 'A', 'D', 'O', 'B' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_ADOBE_EXPERT,   'A', 'D', 'B', 'E' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_ADOBE_CUSTOM,   'A', 'D', 'B', 'C' ),
    UGL_FONT_CHARSET_TAG( UGL_FONT_ADOBE_LATIN_1,  'l', 'a', 't', '1' ),

    UGL_FONT_CHARSET_TAG( UGL_FONT_OLD_LATIN_2, 'l', 'a', 't', '2' ),

    UGL_FONT_CHARSET_TAG( UGL_FONT_APPLE_ROMAN, 'a', 'r', 'm', 'n' ),

    UGL_FONT_ISO_8859_1 = UGL_FONT_UNICODE

  } UGL_FONT_CHARSET;

/* BMF font descriptor */

struct UGL_BMF_FONT_DESC
    {
    /* Standard Header */
    struct
        {
        struct
            {
            int min;
            int max;
            }pixelSize;         /* average size of font in pixels */
        struct
            {
            int min;
            int max;
            }weight;            /* weight is a bold setting from 0 - 100 */
        int italic;             /* Italic is usually either on or off */
        int spacing;            /* Mono spaced or proportional */
        int charSet;            /* ISO 8859-1, Unicode, etc */
        char faceName[128];     /* face name of font */
        char familyName[128];   /* family name of font */
        } header;
    int leading;
    int maxAscent;
    int maxDescent;
    int maxAdvance;
    const unsigned char * const * pageData;
    };

/* TTF font descriptor */

struct TTF_FONT_DESC
    {
    TRT_LPCSTR              faceName;
    const unsigned char *   addr;
    long                    len;
    };

/* utility function */

TRT_S32 trt_stricmp(TRT_LPCSTR s,  TRT_LPCSTR t);
TRT_S32 round_int(TRT_FLOAT  r);
TRT_S32 endian();
TRT_U32 get_u32(TRT_BYTE * c, TRT_S32 endian);
TRT_U16 get_u16(TRT_BYTE * c, TRT_S32 endian);

#endif /* _INCLUDE_tgsTypes_H_ */


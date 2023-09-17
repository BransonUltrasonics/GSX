/* tgsBasicTypes.h - tilcon basic data type definition */

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
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION
This file defines the tilcon's primitive data types.
*/

#ifndef _INCLUDE_tgsBasicTypes_H_
#define _INCLUDE_tgsBasicTypes_H_

/* data type */

typedef long long TRT_S64;
typedef int TRT_S32;
typedef short TRT_S16;
typedef char TRT_S8;

typedef unsigned int TRT_U32;
typedef unsigned short TRT_U16;
typedef unsigned char TRT_U8;

typedef float TRT_FLOAT;
typedef int TRT_BOOL;

typedef void *      TRT_PVOID;
typedef TRT_PVOID   TRT_ID;
typedef TRT_PVOID   TRT_HANDLE;

typedef TRT_S32 TRT_STATUS;

typedef char            TRT_CHAR;
typedef unsigned char   TRT_BYTE;
typedef unsigned char   TRT_UCHAR;
typedef wchar_t         TRT_WCHAR;

typedef TRT_CHAR * TRT_LPSTR;
typedef const TRT_CHAR * TRT_LPCSTR;
typedef TRT_WCHAR * TRT_LPWSTR;
typedef const TRT_WCHAR * TRT_LPCWSTR;

typedef TRT_HANDLE  TRT_FONT_HANDLE;
typedef TRT_HANDLE  TRT_PATH_HANDLE;
typedef TRT_HANDLE  TRT_PBUFFER_HANDLE;
typedef TRT_HANDLE  TRT_SURFACE_HANDLE;

#define TRT_STATUS_OK               (0)
#define TRT_STATUS_ERROR            (-1)

#define TRT_TRUE                    (1)
#define TRT_FALSE                   (0)

#define TRT_NULL                    (0)

/* debug output */

#define TILCON_ENBLE_DEBUG_INFO

#define DEBUG_OUT(...)  (void)printf(__VA_ARGS__)

#ifdef TILCON_ENBLE_DEBUG_INFO
#define TRT_DEBUG_INFO(...)    DEBUG_OUT( __VA_ARGS__)
#else
#define TRT_DEBUG_INFO(...)
#endif

#ifdef TILCON_ENBLE_DEBUG_WARNING
#define TRT_DEBUG_WARNING(...)   DEBUG_OUT(__VA_ARGS__)
#else
#define TRT_DEBUG_WARNING(...)
#endif

/* key code */

#define TRT_KBD_BASE                        0xE000

#define TRT_KBD_HOME_KEY                    (TRT_KBD_BASE + 0)
#define TRT_KBD_END_KEY                     (TRT_KBD_BASE + 1)
#define TRT_KBD_INSERT_KEY                  (TRT_KBD_BASE + 2)
#define TRT_KBD_PAGE_UP_KEY                 (TRT_KBD_BASE + 3)
#define TRT_KBD_PAGE_DOWN_KEY               (TRT_KBD_BASE + 4)
#define TRT_KBD_LEFT_ARROW_KEY              (TRT_KBD_BASE + 5)
#define TRT_KBD_RIGHT_ARROW_KEY             (TRT_KBD_BASE + 6)
#define TRT_KBD_UP_ARROW_KEY                (TRT_KBD_BASE + 7)
#define TRT_KBD_DOWN_ARROW_KEY              (TRT_KBD_BASE + 8)
#define TRT_KBD_PRINT_SCREEN_KEY            (TRT_KBD_BASE + 9)
#define TRT_KBD_PAUSE_KEY                   (TRT_KBD_BASE + 10)
#define TRT_KBD_CAPS_KEY                    (TRT_KBD_BASE + 11)
#define TRT_KBD_NUM_KEY                     (TRT_KBD_BASE + 12)
#define TRT_KBD_SCR_KEY                     (TRT_KBD_BASE + 13)
#define TRT_KBD_LEFT_SHIFT_KEY              (TRT_KBD_BASE + 14)
#define TRT_KBD_RIGHT_SHIFT_KEY             (TRT_KBD_BASE + 15)
#define TRT_KBD_LEFT_CTRL_KEY               (TRT_KBD_BASE + 16)
#define TRT_KBD_RIGHT_CTRL_KEY              (TRT_KBD_BASE + 17)
#define TRT_KBD_LEFT_ALT_KEY                (TRT_KBD_BASE + 18)
#define TRT_KBD_RIGHT_ALT_KEY               (TRT_KBD_BASE + 19)
#define TRT_KBD_LEFT_GUI_KEY                (TRT_KBD_BASE + 20)
#define TRT_KBD_RIGHT_GUI_KEY               (TRT_KBD_BASE + 21)
#define TRT_KBD_MENU_KEY                    (TRT_KBD_BASE + 22)

#define TRT_KBD_FUNC_KEY_BASE               (TRT_KBD_BASE + 0x0F00)

#define TRT_KBD_F1_KEY                      (TRT_KBD_FUNC_KEY_BASE + 1)
#define TRT_KBD_F2_KEY                      (TRT_KBD_FUNC_KEY_BASE + 2)
#define TRT_KBD_F3_KEY                      (TRT_KBD_FUNC_KEY_BASE + 3)
#define TRT_KBD_F4_KEY                      (TRT_KBD_FUNC_KEY_BASE + 4)
#define TRT_KBD_F5_KEY                      (TRT_KBD_FUNC_KEY_BASE + 5)
#define TRT_KBD_F6_KEY                      (TRT_KBD_FUNC_KEY_BASE + 6)
#define TRT_KBD_F7_KEY                      (TRT_KBD_FUNC_KEY_BASE + 7)
#define TRT_KBD_F8_KEY                      (TRT_KBD_FUNC_KEY_BASE + 8)
#define TRT_KBD_F9_KEY                      (TRT_KBD_FUNC_KEY_BASE + 9)
#define TRT_KBD_F10_KEY                     (TRT_KBD_FUNC_KEY_BASE + 10)
#define TRT_KBD_F11_KEY                     (TRT_KBD_FUNC_KEY_BASE + 11)
#define TRT_KBD_F12_KEY                     (TRT_KBD_FUNC_KEY_BASE + 12)

/* math */

#define TRT_MAX(a,b)    ((a)>(b)?(a):(b))
#define TRT_MIN(a,b)    ((a)>(b)?(b):(a))

#define TRT_MOD(a, b)   (((TRT_U32)(a))%(b))
#define TRT_DIV(a, b)   (((a)-TRT_MOD(a,b))/(b))

#define TRT_PI  (3.1415926f)
#define TRT_ANGLE_TO_DEGREE(a) ((TRT_FLOAT)(a) * 180.0f / TRT_PI)
#define TRT_DEGREE_TO_ANGLE(d) ((TRT_FLOAT)(d) * TRT_PI / 180.0f)

/* vector path command */

#define PATH_CLOSE                      (0)
#define PATH_MOVE_TO_ABS                (2  | 0)
#define PATH_MOVE_TO_REL                (2  | 1)
#define PATH_LINE_TO_ABS                (4  | 0)
#define PATH_LINE_TO_REL                (4  | 1)
#define PATH_HLINE_TO_ABS               (6  | 0)
#define PATH_HLINE_TO_REL               (6  | 1)
#define PATH_VLINE_TO_ABS               (8  | 0)
#define PATH_VLINE_TO_REL               (8  | 1)
#define PATH_QUAD_TO_ABS                (10 | 0)
#define PATH_QUAD_TO_REL                (10 | 1)
#define PATH_CUBIC_TO_ABS               (12 | 0)
#define PATH_CUBIC_TO_REL               (12 | 1)
#define PATH_SQUAD_TO_ABS               (14 | 0)
#define PATH_SQUAD_TO_REL               (14 | 1)
#define PATH_SCUBIC_TO_ABS              (16 | 0)
#define PATH_SCUBIC_TO_REL              (16 | 1)
#define PATH_SCCWARC_TO_ABS             (18  | 0)
#define PATH_SCCWARC_TO_REL             (18  | 1)
#define PATH_SCWARC_TO_ABS              (20  | 0)
#define PATH_SCWARC_TO_REL              (20  | 1)
#define PATH_LCCWARC_TO_ABS             (22  | 0)
#define PATH_LCCWARC_TO_REL             (22  | 1)
#define PATH_LCWARC_TO_ABS              (24  | 0)
#define PATH_LCWARC_TO_REL              (24  | 1)

/* misc */

#define TRT_LITTLE_ENDIAN 0
#define TRT_BIG_ENDIAN    1

/* platform specific */

#ifdef _MSC_VER
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
/* ignore MSVC warning: function or variable may be unsafe */
#pragma warning(disable : 4996)
/* ignore MSVC warning: 'this' used in base member initializer list */
#pragma warning(disable : 4355)
#endif

/* enums */

enum ControlType
    {
    CT_unknown = 0,

    CT_tws_window,
    CT_tws_root,

    CT_push_button = 0x100,
    CT_toggle_button,
    CT_multistate_button,
    CT_check_box,
    CT_combo_box,
    CT_radio_button,
    CT_spin_box,
    CT_text_spin_box,
    CT_progress_bar,
    CT_slider,
    CT_scroll_bar,
    CT_picture,
    CT_label,
    CT_single_line_edit,
    CT_multi_line_edit,
    CT_panel,
    CT_group_box,
    CT_grid_panel,
    CT_row_panel,
    CT_scroll_area,
    CT_tab,
    CT_list,
    CT_text_table,

    /* add user defined control here */
    CT_user_control = 0x200,
    };

enum DirectionType
    {
    DT_left = 0,
    DT_right,
    DT_up,
    DT_down,
    };

enum OrientationType
    {
    OT_horizontal = 0,
    OT_vertical,
    };

enum SizeSource
    {
    SS_fixed = 0,
    SS_auto,
    };

enum HorizontalAlignment
    {
    HA_fixed = 0,
    HA_fill,
    HA_left,
    HA_center,
    HA_right,
    };

enum VerticalAlignment
    {
    VA_fixed = 0,
    VA_fill,
    VA_top,
    VA_center,
    VA_bottom,
    };

enum TextHorizontalAlignment
    {
    THA_left,
    THA_center,
    THA_right,
    };

enum TextVerticalAlignment
    {
    TVA_top,
    TVA_center,
    TVA_bottom,
    };

/* raster operation for 2D painter */

enum RasterMode
    {
    RM_copy = 0,
    RM_alpha_blend,
    RM_alpha_key /* TODO: not supported yet */
    };

/* blend mode for VG painter */

enum BlendMode
    {
    BM_src = 0,
    BM_src_over_dst
    };

/* Line style */

enum DashStyle
    {
    DS_solid = 0,
    DS_dash,
    DS_dot,
    };

/* style for joint where two lines connect*/

enum JointStyle
    {
    JS_bevel = 0,
    JS_miter,
    JS_round,
    };

/* style for end point of line*/

enum CapStyle
    {
    CS_butt = 0,
    CS_round,
    CS_square,
    };

enum FillRule
    {
    FR_even_odd = 0,
    FR_non_zero
    };

enum DragState
    {
    DS_none = 0,
    DS_begin,
    DS_dragging,
    DS_end
    };

enum FrameType
    {
    FT_none = 0,
    FT_panel_with_border,
    FT_panel_without_border,
    FT_panel_border_only,
    FT_field_with_border,
    FT_field_without_border,
    FT_field_border_only,

    FT_group_box,
    FT_group_box_header,

    FT_push_button,

    FT_toggle_button_out,
    FT_toggle_button_in,

    FT_check_indicator_check,
    FT_check_indicator_uncheck,

    FT_radio_indicator_check,
    FT_radio_indicator_uncheck,

    FT_combobox_button,
    FT_combobox_edit,

    FT_spinbox_button_up,
    FT_spinbox_button_down,

    FT_scrollbar_track_horizontal,
    FT_scrollbar_track_vertical,
    FT_scrollbar_thumb_horizontal,
    FT_scrollbar_thumb_vertical,

    FT_scrollbar_button_up,
    FT_scrollbar_button_down,
    FT_scrollbar_button_left,
    FT_scrollbar_button_right,

    FT_slider_track_horizontal,
    FT_slider_track_vertical,
    FT_slider_thumb_horizontal,
    FT_slider_thumb_vertical,

    FT_progressbar_track_background_horizontal,
    FT_progressbar_track_background_vertical,
    FT_progressbar_track_border_horizontal,
    FT_progressbar_track_border_vertical,
    FT_progressbar_bar_horizontal,
    FT_progressbar_bar_vertical,

    FT_list_column_header,

    FT_table_column_header,
    FT_table_row_header,
    };

#endif /* _INCLUDE_tgsBasicTypes_H_ */

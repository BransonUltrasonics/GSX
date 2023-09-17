/* twdFile.h - twd file */

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
12aug15,c_l Remove 64-bit compiler warning.
01jan14,yya created
*/


#ifndef _INCLUDE_twdFile_H_
#define _INCLUDE_twdFile_H_


#include "tilcon/tilcon.h"

#undef ENABLE_TWD_PARSE_OUTPUT

#ifdef ENABLE_TWD_PARSE_OUTPUT
#define TWD_INFO(...)   DEBUG_OUT(__VA_ARGS__)
#else
#define TWD_INFO(...)
#endif

class TControl;
class TWindow;
class TImage;
struct TextProperty;

class TWDFile
    {
public:
    enum TWDWidgetType
        {
        TWD_unknown = 0,
        TWD_button,
        TWD_check_box,
        TWD_label,
        TWD_radio_button,
        TWD_combo_box,
        TWD_list,
        TWD_edit,
        TWD_number_box,
        TWD_slider,
        TWD_panel,
        TWD_scroll_area,
        TWD_tab,
        TWD_tab_page,
        TWD_picture,
        TWD_chart,
        TWD_multi_state,
        };

public:
    TWDFile();
    virtual ~TWDFile();

    TRT_STATUS Load(const TString & filename);
    void Unload();

    TWDWidgetType GetTypeByName(const TString &) const;
    TControl * GetControlByName(const TString &) const;
    TWindow * GetTwdWindow() const {return twdHost;}
private:
    struct TwdHeader
        {
        TRT_S32 len;
        TRT_S32 len1;
        TRT_S32 version;
        TRT_S32 convertEndian;
        };

    struct TwdSegment
        {
        TRT_S32     type;
        TRT_S32     len;
        TRT_S32     id_len;
        TRT_S32     parent_id_len;
        TRT_S32     parameter_len;
        TRT_CHAR *  id;
        TRT_CHAR *  parent_id;
        TRT_BYTE *  parameters;
        };

    struct FontAttribute
        {
        FontAttribute();
        TRT_S32     type;
        TRT_S32     size;
        TRT_BOOL    bold;
        TRT_BOOL    italic;
        TRT_S32     horJust;
        TRT_S32     verJust;
        TRT_U32     color;
        TRT_U32     fillColor;
        };

    struct PositionAttribute
        {
        PositionAttribute();
        TRT_S32     x1;
        TRT_S32     y1;
        TRT_S32     x2;
        TRT_S32     y2;
        };

    struct ImageAttribute
        {
        ImageAttribute();
        TRT_S32     loadMode;
        TRT_S32     imageType;
        TRT_S32     angle;
        TPoint      anchor;

        TRT_S32     len;
        TRT_BYTE *  buf;
        TString     fileName;
        TRT_U8      maintainAspect : 1;
        };

    struct BorderAttribute
        {
        BorderAttribute();

        TRT_U32 fillColor;
        TRT_U32 topShadowColor;
        TRT_U32 bottomShadowColor;
        TRT_S32 xMargin;
        TRT_S32 yMargin;

        TRT_S32 shape;
        TRT_S32 type;
        TRT_S32 inOut;
        TRT_S32 enclosed;
        TRT_S32 antiAlias;
        TRT_S32 depth;
        };

    struct RealObjAttribute : public PositionAttribute
        {
        RealObjAttribute();
        TRT_U8          hide            : 1;
        TRT_U8          dimmed          : 1;
        TRT_U8          preventFocus    : 1;

        TString         id;
        };

    struct WindowAttribute : public RealObjAttribute
        {
        WindowAttribute();
        FontAttribute   font;
        TRT_S32         minWidth;
        TRT_S32         minHeight;
        TRT_S32         maxWidth;
        TRT_S32         maxHeight;

        TRT_U8          topMost : 1;

        ImageAttribute  image;
        TRT_S32         currentImage;
        };

    struct PanelAttribute : public WindowAttribute
        {
        PanelAttribute();
        };

    struct ScrollBarAttribute
        {
        ScrollBarAttribute();
        TRT_S32         whichImage;
        TRT_S32         show;
        TRT_S32         inc;
        TRT_S32         largeInc;
        TRT_S32         initPos;
        TRT_S32         pos;
        TRT_S32         range;
        };

    struct ScrollAreaAttribute : public WindowAttribute
        {
        ScrollAreaAttribute();
        TRT_S32             whichBar;
        ScrollBarAttribute  horBar;
        ScrollBarAttribute  verBar;
        };

    struct LabelAttribute : public RealObjAttribute
        {
        LabelAttribute();
        FontAttribute   font;
        TRT_CHAR *      text;
        };

    struct ButtonAttribute : public RealObjAttribute
        {
        ButtonAttribute();
        TRT_S32         type;
        BorderAttribute border;
        FontAttribute   font;
        TRT_S32         currentImage;
        ImageAttribute  dimImage;
        ImageAttribute  inImage;
        ImageAttribute  outImage;
        ImageAttribute  downImage;
        ImageAttribute  focusedInImage;
        ImageAttribute  focusedOutImage;
        ImageAttribute  hoverImage;
        TRT_CHAR *      inText;
        TRT_CHAR *      outText;
        TRT_S32         inColor;
        TRT_S32         outColor;
        TRT_S32         onColor;
        TRT_S32         offColor;
        TMargin         margin;
        };

    struct RadioItemAttribute
        {
        RadioItemAttribute();

        ImageAttribute  dimImage;
        ImageAttribute  inImage;
        ImageAttribute  outImage;
        ImageAttribute  downImage;
        ImageAttribute  focusedInImage;
        ImageAttribute  focusedOutImage;
        ImageAttribute  hoverImage;
        TRT_CHAR *      text;
        };

    struct RadioButtonAttribute : public RealObjAttribute
        {
        RadioButtonAttribute();

        TRT_S32                     currentItem;
        TRT_S32                     currentImage;
        TRT_S32                     columns;
        FontAttribute               font;
        TArray<RadioItemAttribute>  items;
        };

    struct EditAttribute : public RealObjAttribute
        {
        EditAttribute();

        BorderAttribute border;
        FontAttribute   font;
        TRT_CHAR *      text;
        };

    struct ListItemAttribute
        {
        ListItemAttribute();

        TRT_CHAR *  id;
        TRT_CHAR *  text;
        TRT_S32     hide;
        TRT_U32     textColor;
        TRT_U32     fillColor;
        };

    struct ListAttribute : public ScrollAreaAttribute
        {
        ListAttribute();
        ~ListAttribute();

        FontAttribute   font;

        TRT_S32     whichComponent;
        TRT_S32     whichItem;
        TRT_S32     whichCol;
        TRT_S32     visibleRows;
        TRT_S32     visibleCols;
        TRT_S32 *   colWidths;
        TRT_S32     rowMargin;
        TRT_S32     colMargin;
        TRT_S32     labelAlignment;
        TRT_S32     imageMargin;
        TRT_S32     indexWidth;

        TRT_S32     allowNoSelection;
        TRT_S32     highlightMode;
        TRT_S32     selectMode;
        TRT_S32     sortMode;
        TRT_S32     colorListBox;

        TRT_S32     highlightTextColor;
        TRT_S32     highlightBkColor;

        TArray<ListItemAttribute> items;
        };

    struct ComboBoxAttribute : public RealObjAttribute
        {
        ComboBoxAttribute();

        EditAttribute       edit;
        ListAttribute       list;

        TRT_S32 whichBorder;
        TRT_S32 whichArrow;
        TRT_S32 whichComponent;
        };

    struct NumberBoxAttribute : public RealObjAttribute
        {
        NumberBoxAttribute();

        BorderAttribute border;
        FontAttribute   font;

        TRT_S32 whichArrow;

        TRT_CHAR *          label;

        TRT_S32 decimalDigits;
        TRT_S32 labelPlacement;
        TRT_S32 labelEnclosed;
        TRT_S32 labelSeperated;
        TRT_S32 decimalMode;
        TRT_S32 arrowMode;
        TRT_S32 arrowHide;

        TRT_FLOAT increment;
        TRT_FLOAT max;
        TRT_FLOAT min;
        TRT_FLOAT value;

        TRT_S32 splitLineColor;

        ImageAttribute downImageOut;
        ImageAttribute downImageIn;
        ImageAttribute downImageDim;
        ImageAttribute upImageOut;
        ImageAttribute upImageIn;
        ImageAttribute upImageDim;
        };

    struct SliderAttribute : public RealObjAttribute
        {
        SliderAttribute();

        TRT_S32 throughHeight;
        TRT_S32 thumbOffset;
        TRT_S32 thumbWidth;
        TRT_S32 thumbHeight;
        TRT_BOOL vertical;

        TRT_FLOAT min;
        TRT_FLOAT max;
        TRT_FLOAT value;
        TRT_FLOAT smallInc;
        TRT_FLOAT largeInc;

        TRT_S32 whichBorder;
        BorderAttribute frameBorder;
        BorderAttribute throughBorder;
        BorderAttribute thumbBorder;
        ImageAttribute  thumbImage;
        };

    struct TabAttribute : public RealObjAttribute
        {
        TabAttribute();
        ~TabAttribute();
        FontAttribute   font;
        TRT_CHAR**      tabId;
        TRT_CHAR**  tabLabel;
        TRT_S32     tabIndex;
        TRT_S32     tabNumber;
        TRT_S32     tabsPerRow;
        TRT_S32     initTab;
        TRT_S32     tabHeight;
        TRT_S32     tabLocation;
        TRT_S32     tabType;
        TRT_S32     tabOffset;
        TRT_U32     highlightFillColor;
        TRT_U32     highlightTextColor;
        };

    struct PictureAttribute : public RealObjAttribute
        {
        ImageAttribute  image;
        };

    struct ChartAttribute : public RealObjAttribute
        {
        BorderAttribute     border;
        FontAttribute       font;
        };

    struct MultiStateItemAttribute
        {
        ImageAttribute  image;
        };

    struct MultiStateAttribute : public RealObjAttribute
        {
        MultiStateAttribute();

        TRT_S32                         activeState;
        TRT_S32                         currentItem;
        TArray<MultiStateItemAttribute> items;
        };

    struct ControlInfo
        {
        TString         id;
        TString         parentId;
        TWDWidgetType   type;
        TControl *      control;
        TRT_S32         page;
        };

    TString             twdHostId;
    TWindow *             twdHost;
    TArray<ControlInfo> controlList;

    TRT_STATUS CheckFileVersion(TRT_BYTE *);
    TRT_S32 ParseHeader(TRT_BYTE *);
    TRT_STATUS ParseBody(TRT_BYTE *);

    TRT_STATUS ParseSegment(TwdSegment &);

    TRT_STATUS ParseButton(TRT_BYTE * ptr, TRT_S32 size, ButtonAttribute & att);
    TRT_STATUS ParseLabel(TRT_BYTE * ptr, TRT_S32 size, LabelAttribute & att);
    TRT_STATUS ParseRadioButton(TRT_BYTE * ptr, TRT_S32 size, RadioButtonAttribute & att);
    TRT_STATUS ParseComboBox(TRT_BYTE * ptr, TRT_S32 size, ComboBoxAttribute & att);
    TRT_STATUS ParseNumberBox(TRT_BYTE * ptr, TRT_S32 size, NumberBoxAttribute & att);
    TRT_STATUS ParseSlider(TRT_BYTE * ptr, TRT_S32 size, SliderAttribute & att);
    TRT_STATUS ParseEdit(TRT_BYTE * ptr, TRT_S32 size, EditAttribute & att);
    TRT_STATUS ParseList(TRT_BYTE * ptr, TRT_S32 size, ListAttribute & att);
    TRT_STATUS ParsePanel(TRT_BYTE * ptr, TRT_S32 size, PanelAttribute & att);
    TRT_STATUS ParseWindow(TRT_BYTE * ptr, TRT_S32 size, WindowAttribute & att);
    TRT_STATUS ParseScrollArea(TRT_BYTE * ptr, TRT_S32 size, ScrollAreaAttribute & att);
    TRT_STATUS ParseTab(TRT_BYTE * ptr, TRT_S32 size, TabAttribute & att);
    TRT_STATUS ParsePicture(TRT_BYTE * ptr, TRT_S32 size, PictureAttribute & att);
    TRT_STATUS ParseChart(TRT_BYTE * ptr, TRT_S32 size, ChartAttribute & att);
    TRT_STATUS ParseMultiState(TRT_BYTE * ptr, TRT_S32 size, MultiStateAttribute & att);

    TRT_BOOL BranchPosition(TRT_S32 fid, TRT_S32 * lp, PositionAttribute & att);
    TRT_BOOL BranchFont(TRT_S32 fid, TRT_S32 * lp, FontAttribute & att);
    TRT_BOOL BranchImage(TRT_S32 fid, TRT_S32 * lp, ImageAttribute & att);
    TRT_BOOL BranchBorder(TRT_S32 fid, TRT_S32 * lp, BorderAttribute & att);
    TRT_BOOL BranchRealObj(TRT_S32 fid, TRT_S32 * lp, RealObjAttribute & att);
    TRT_BOOL BranchButton(TRT_S32 fid, TRT_S32 * lp, ButtonAttribute & att);
    TRT_BOOL BranchLabel(TRT_S32 fid, TRT_S32 * lp, LabelAttribute & att);
    TRT_BOOL BranchRadioButton(TRT_S32 fid, TRT_S32 * lp, RadioButtonAttribute & att);
    TRT_BOOL BranchComboBox(TRT_S32 fid, TRT_S32 * lp, ComboBoxAttribute & att);
    TRT_BOOL BranchNumberBox(TRT_S32 fid, TRT_S32 * lp, NumberBoxAttribute & att);
    TRT_BOOL BranchSlider(TRT_S32 fid, TRT_S32 * lp, SliderAttribute & att);
    TRT_BOOL BranchEdit(TRT_S32 fid, TRT_S32 * lp, EditAttribute & att);
    TRT_BOOL BranchListItem(TRT_S32 fid, TRT_S32 * lp, ListItemAttribute & att);
    TRT_BOOL BranchList(TRT_S32 fid, TRT_S32 * lp, ListAttribute & att);
    TRT_BOOL BranchPanel(TRT_S32 fid, TRT_S32 * lp, PanelAttribute & att);
    TRT_BOOL BranchWindow(TRT_S32 fid, TRT_S32 * lp, WindowAttribute & att);
    TRT_BOOL BranchScrollBar(TRT_S32 fid, TRT_S32 * lp, ScrollBarAttribute & att);
    TRT_BOOL BranchScrollBarPair(TRT_S32 fid, TRT_S32 * lp, ScrollAreaAttribute & att);
    TRT_BOOL BranchScrollArea(TRT_S32 fid, TRT_S32 * lp, ScrollAreaAttribute & att);
    TRT_BOOL BranchTab(TRT_S32 fid, TRT_S32 * lp, TabAttribute & att);
    TRT_BOOL BranchMultiState(TRT_S32 fid, TRT_S32 * lp, MultiStateAttribute & att);

    void LoadImage(TRT_BYTE *, TRT_S32, TImage &);

    TRT_STATUS CreateButtonFromAtt(const TwdSegment &);
    TRT_STATUS CreateLabelFromAtt(const TwdSegment &);
    TRT_STATUS CreateRadioButtonFromAtt(const TwdSegment &);
    TRT_STATUS CreateComboBoxFromAtt(const TwdSegment &);
    TRT_STATUS CreateNumberBoxFromAtt(const TwdSegment &);
    TRT_STATUS CreateSliderFromAtt(const TwdSegment &);
    TRT_STATUS CreateEditFromAtt(const TwdSegment &);
    TRT_STATUS CreateListFromAtt(const TwdSegment &);
    TRT_STATUS CreatePanelFromAtt(const TwdSegment &);
    TRT_STATUS CreateWindowFromAtt(const TwdSegment &);
    TRT_STATUS CreateScrollAreaFromAtt(const TwdSegment &);
    TRT_STATUS CreateTabFromAtt(const TwdSegment &);
    TRT_STATUS CreatePictureFromAtt(const TwdSegment &);
    TRT_STATUS CreateChartFromAtt(const TwdSegment &);
    TRT_STATUS CreateMultiStateFromAtt(const TwdSegment &);

    void ApplyRealObjAttribute(TControl &, const RealObjAttribute &);
    void ApplyTextAttribute(TextProperty &, const FontAttribute &);
    TColor GetTWDColor(TRT_U32);

    const ControlInfo * GetInfoByName(const TString & name) const;
    TRT_STATUS AddControlInfo(const ControlInfo & info);

    TwdHeader twdHeader;
    };

#endif /* _INCLUDE_twdFile_H_*/


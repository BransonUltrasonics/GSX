/* tgsScrollHelper.h - scroll helper implementation */

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

This file defines the scroll helper class.
*/


#ifndef _INCLUDE_tgsScrollHelper_H_
#define _INCLUDE_tgsScrollHelper_H_

#include "tilcon/engine/tgsControl.h"

TRT_S32 GetValidScrollPos(TRT_S32 page, TRT_S32 total, TRT_S32 pos);

class TScrollHelper
    {
public:
    struct ScrollChangeEventParam : public EventParam
        {
        TSize   page;
        TSize   total;
        TPoint  pos;
        };

    TScrollHelper(const TSize & _page = TSize(0, 0), const TSize & _total = TSize(0, 0));
    virtual ~TScrollHelper();

    void SetTotalSize(const TSize &, TEvent *);
    void SetPageSize(const TSize &, TEvent *);
    void SetInfo(const TSize &, const TSize &, TEvent *);
    void SetInfo(const TSize &, const TSize &, const TPoint &, TEvent *);
    void XScrollTo(TRT_S32, TEvent *);
    void YScrollTo(TRT_S32, TEvent *);
    void ScrollTo(const TPoint &, TEvent *);

    const TSize & TotalSize() const {return total;}
    const TSize & PageSize() const {return page;}
    const TPoint & ScrollPos() const {return pos;}

private:
    void Adjust(TEvent *);
    TPoint  pos;
    TSize   page;
    TSize   total;
    };

#endif /* _INCLUDE_tgsScrollHelper_H_ */


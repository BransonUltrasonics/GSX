/* tgsPrimitive.cpp - tilcon primitive classes header */

/*
 * Copyright (c) 2014-2015, 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
06jan17,jnl code clean
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created (US15550)
*/

/*
DESCRIPTION

This file declares the tilcon's primitive classes.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tgsPrimitive_H_
#define _INCLUDE_tgsPrimitive_H_

class TNonCopyable
    {
protected:
    TNonCopyable(){};
    ~TNonCopyable(){};
private:
    TNonCopyable (TNonCopyable const &){};
    TNonCopyable & operator = (TNonCopyable const &){return *this;};
    };

class TException
    {
public:
    enum Type
        {
        ET_general = 0,
        ET_parameter,
        ET_memory_alloc,
        ET_index,
        ET_node_relationship,
        ET_platform_init,
        ET_no_application_instance,
        ET_second_application_instance
        };

    TException(Type t) : type(t)
        {
        }
    virtual ~TException()
        {
        }
    Type GetType() const
        {
        return type;
        }
private:
    Type type;
    };

template<typename T>
class _TLinear
    {
public:
    T pt1;
    T pt2;

    _TLinear() : pt1(0), pt2(1)
        {
        };

    _TLinear(T _pt1, T _pt2) : pt1(_pt1), pt2(_pt2)
        {
        };

    ~_TLinear()
        {
        };

    T Span() const
        {
        return pt2 - pt1;
        }

    TRT_BOOL IsValid() const
        {
        return pt2 != pt1;
        }

    TRT_BOOL operator == (const _TLinear<T> & linear) const
        {
        return (pt1 == linear.pt1) && (pt2 == linear.pt2);
        }

    TRT_BOOL operator != (const _TLinear<T> & linear) const
        {
        return (pt1 != linear.pt1) || (pt2 != linear.pt2);
        }

    T Clamp(T value) const
        {
        T range_min = pt1 < pt2 ? pt1 : pt2;
        T range_max = pt1 > pt2 ? pt1 : pt2;

        if (value <= range_min)
            return range_min;
        if (value >= range_max)
            return range_max;
        return value;
        }

    TRT_FLOAT GetRatio(T value) const
        {
        if (TRT_FALSE == IsValid())
            {
            return 0.0f;
            }
        return (TRT_FLOAT)((TRT_FLOAT)(value - pt1) / (TRT_FLOAT)Span());
        }

    TRT_FLOAT TranslateFrom(TRT_FLOAT ratio)
        {
        return ratio * Span() + (TRT_FLOAT)pt1;
        }

    TRT_FLOAT TranslateFrom(const _TLinear<T> & linear, T value) const
        {
        return (TRT_FLOAT)((TRT_FLOAT)linear.GetRatio(value) * (TRT_FLOAT)Span()
                           + (TRT_FLOAT)pt1);
        }
    };

template<typename T>
class _TPoint
    {
public:
    T x;
    T y;

    _TPoint() : x(0),y(0)
        {
        }

    _TPoint(T _x, T _y) : x(_x), y(_y)
        {
        }

    ~_TPoint()
        {
        }

    TRT_BOOL operator == (const _TPoint & pt) const
        {
        return (x == pt.x) && (y == pt.y);
        }

    TRT_BOOL operator != (const _TPoint & pt) const
        {
        return (x != pt.x) || (y != pt.y);
        }

    const _TPoint & operator = (const _TPoint & pt)
        {
        x = pt.x;
        y = pt.y;
        return *this;
        }

    const _TPoint & operator += (const _TPoint & pt)
        {
        x += pt.x;
        y += pt.y;
        return *this;
        }

    const _TPoint & operator -= (const _TPoint & pt)
        {
        x -= pt.x;
        y -= pt.y;
        return *this;
        }

    const _TPoint operator + (const _TPoint & pt) const
        {
        return _TPoint(x+pt.x, y+pt.y);
        }

    const _TPoint operator - (const _TPoint & pt) const
        {
        return _TPoint(x-pt.x, y-pt.y);
        }
    };

template<typename T>
class _TSize
    {
public:
    T w;
    T h;

    _TSize() : w(0), h(0)
        {
        }

    _TSize(T _w, T _h) : w(_w), h(_h)
        {
        }

    ~_TSize()
        {
        }

    TRT_BOOL operator == (const _TSize & size) const
        {
        return (w == size.w) && (h == size.h);
        }

    TRT_BOOL operator != (const _TSize & size) const
        {
        return (w != size.w) || (h != size.h);
        }

    const _TSize & operator = (const _TSize & size)
        {
        w = size.w;
        h = size.h;
        return *this;
        }

    const _TSize & operator += (const _TSize & size)
        {
        w += size.w;
        h += size.h;
        return *this;
        }

    const _TSize & operator -= (const _TSize & size)
        {
        w -= size.w;
        h -= size.h;
        return *this;
        }

    const _TSize operator + (const _TSize & size)
        {
        return _TSize(w + size.w, h + size.h);
        }

    const _TSize operator - (const _TSize & size)
        {
        return _TSize(w - size.w, h - size.h);
        }
    };

template<typename T>
class _TMargin
    {
public:
    T left;
    T top;
    T right;
    T bottom;

    _TMargin() : left(0), top(0), right(0), bottom(0)
        {}
    _TMargin(T width) : left(width), top(width), right(width), bottom(width)
        {}
    _TMargin(const _TMargin & margin) : left(margin.left), top(margin.top),
        right(margin.right), bottom(margin.bottom)
        {}
    _TMargin(T _left, T _top, T _right, T _bottom) :
        left(_left), top(_top), right(_right), bottom(_bottom)
        {}

    TRT_BOOL operator == (const _TMargin<T> & m) const
        {
        return (left == m.left && right == m.right &&
                top == m.top && bottom == m.bottom);
        }

    TRT_BOOL operator != (const _TMargin<T> & m) const
        {
        return (left != m.left || right != m.right ||
                top != m.top || bottom != m.bottom);
        }
    };

template<typename T>
class _TRect
    {
public:
    T left;
    T top;
    T right;
    T bottom;

    _TRect() : left(nullRect.left), top(nullRect.top), right(nullRect.right), bottom(nullRect.bottom)
        {}
    _TRect(const _TRect & rect) : left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom)
        {}
    _TRect(T _left, T _top, T _right, T _bottom) :
        left(_left), top(_top), right(_right), bottom(_bottom){}
    _TRect(const _TSize<T> & size) :
        left(0), top(0), right(size.w-1), bottom(size.h-1){}
    _TRect(const _TPoint<T> & pt, const _TSize<T> & size) :
        left(pt.x), top(pt.y), right(pt.x+size.w-1), bottom(pt.y+size.h-1){}

    _TRect(const _TPoint<T> & pt1, const _TPoint<T> & pt2) :
        left(TRT_MIN(pt1.x, pt2.x)),
        top(TRT_MIN(pt1.y, pt2.y)),
        right(TRT_MAX(pt1.x, pt2.x)),
        bottom(TRT_MAX(pt1.y, pt2.y)){}

    ~_TRect(){};

    T Width() const
        {
        return (right - left + 1);
        }

    T Height() const
        {
        return (bottom - top + 1);
        }

    _TPoint<T> LeftTop() const
        {
        return _TPoint<T>(left, top);
        }

    _TPoint<T> RightTop() const
        {
        return _TPoint<T>(right, top);
        }

    _TPoint<T> LeftBottom() const
        {
        return _TPoint<T>(left, bottom);
        }

    _TPoint<T> RightBottom() const
        {
        return _TPoint<T>(right, bottom);
        }

    _TRect Normalize() const
        {
        return *this - LeftTop();
        }

    _TRect Expand(T value) const
        {
        return _TRect(left - value, top - value, right + value, bottom + value);
        }

    _TRect Shrink(T value) const
        {
        return _TRect(left + value, top + value, right - value, bottom - value);
        }

    _TRect Expand(const _TSize<T> & size) const
        {
        return _TRect(left, top, right + size.w, bottom + size.h);
        }

    _TRect Shrink(const _TSize<T> & size) const
        {
        return _TRect(left, top, right - size.w, bottom - size.h);
        }

    _TRect Adjust(T ax1, T ay1, T ax2, T ay2) const
        {
        return _TRect(left + ax1, top + ay1, right + ax2, bottom + ay2);
        }

    _TRect Adjust(const _TMargin<T> & margin) const
        {
        return _TRect(left + margin.left, top + margin.top, right - margin.right, bottom - margin.bottom);
        }

    _TRect Combine(const _TRect & rect) const
        {
        return _TRect(TRT_MIN(left, rect.left), TRT_MIN(top, rect.top),
                     TRT_MAX(right, rect.right), TRT_MAX(bottom, rect.bottom));
        }

    _TRect operator + (const _TPoint<T> & pt) const
        {
        return _TRect(left + pt.x, top + pt.y, right + pt.x, bottom + pt.y);
        }

    _TRect operator - (const _TPoint<T> & pt) const
        {
        return _TRect(left - pt.x, top - pt.y, right - pt.x, bottom - pt.y);
        }

    _TRect Intersect(const _TRect & rect) const
        {
        _TRect intersect_rect;

        intersect_rect.left = left > rect.left ? left : rect.left;
        intersect_rect.top = top > rect.top ? top : rect.top;
        intersect_rect.right = right < rect.right ? right : rect.right;
        intersect_rect.bottom = bottom < rect.bottom ? bottom : rect.bottom;

        return intersect_rect;
        }

    TRT_BOOL IsValid() const
        {
        return (left <= right && top <= bottom);
        }

    TRT_BOOL Overlaps(const _TRect & rect) const
        {
        return Intersect(rect).IsValid();
        }

    TRT_BOOL Contains(const _TPoint<T> & pt) const
        {
        return (left <= pt.x && right >= pt.x && top <= pt.y && bottom >= pt.y);
        }

    _TSize<T> ToSize() const
        {
        return _TSize<T>(Width(), Height());
        }

    TRT_BOOL operator == (const _TRect & rect) const
        {
        return (left == rect.left &&
                top == rect.top &&
                right == rect.right &&
                bottom == rect.bottom);
        }

    TRT_BOOL operator != (const _TRect & rect) const
        {
        return (left != rect.left ||
                top != rect.top ||
                right != rect.right ||
                bottom != rect.bottom);
        }

    _TRect & operator += (const _TPoint<T> & pt)
        {
        left += pt.x;
        top += pt.y;
        right += pt.x;
        bottom += pt.y;
        return *this;
        }

    _TRect & operator -= (const _TPoint<T> & pt)
        {
        left -= pt.x;
        top -= pt.y;
        right -= pt.x;
        bottom -= pt.y;
        return *this;
        }

    static const _TRect nullRect;
    static const _TRect maxRect;
    };

typedef _TLinear<TRT_S32>   TLinear;
typedef _TLinear<TRT_FLOAT> TLinearF;

typedef _TPoint<TRT_S32>    TPoint;
typedef _TPoint<TRT_FLOAT>  TPointF;

typedef _TSize<TRT_S32>     TSize;
typedef _TSize<TRT_FLOAT>   TSizeF;

typedef _TMargin<TRT_S32>   TMargin;
typedef _TMargin<TRT_FLOAT> TMarginF;

typedef _TRect<TRT_S32>     TRect;
typedef _TRect<TRT_FLOAT>   TRectF;

class TVector2
    {
public:
    TVector2() : v0(0.0f), v1(0.0f)
        {}
    TVector2(TRT_FLOAT f0, TRT_FLOAT f1) : v0(f0), v1(f1)
        {}
    ~TVector2()
        {}

    TVector2 PerpendicularCW() const
        {
        return TVector2(v1, -v0);
        }
    TVector2 PerpendicularCCW() const
        {
        return TVector2(-v1, v0);
        }
    TVector2 & Normalize();

    TVector2 operator - (const TVector2 & v) const
        {
        return TVector2(v0 - v.v0, v1 - v.v1);
        }

    TVector2 operator + (const TVector2 & v) const
        {
        return TVector2(v0 + v.v0, v1 + v.v1);
        }

    TRT_BOOL operator == (const TVector2 & vector) const
        {
        return (v0 == vector.v0 &&
            v1 == vector.v1);
        }

    bool operator != (const TVector2 & vector) const
        {
        return !(*this == vector);
        }

    TVector2 operator * (TRT_FLOAT f) const
        {
        return TVector2(v0 * f, v1 * f);
        }

    TRT_FLOAT v0, v1;
    };

class TMatrix3x3
    {
public:
    TMatrix3x3() :
        v00(1.0f), v01(0.0f), v02(0.0f),
        v10(0.0f), v11(1.0f), v12(0.0f),
        v20(0.0f), v21(0.0f), v22(1.0f)
        {
        }

    TMatrix3x3(const TMatrix3x3 & m) :
        v00(m.v00), v01(m.v01), v02(m.v02),
        v10(m.v10), v11(m.v11), v12(m.v12),
        v20(m.v20), v21(m.v21), v22(m.v22)
        {
        }

    TMatrix3x3( TRT_FLOAT f00, TRT_FLOAT f01, TRT_FLOAT f02,
                TRT_FLOAT f10, TRT_FLOAT f11, TRT_FLOAT f12,
                TRT_FLOAT f20, TRT_FLOAT f21, TRT_FLOAT f22) :
        v00(f00), v01(f01), v02(f02),
        v10(f10), v11(f11), v12(f12),
        v20(f20), v21(f21), v22(f22)
        {
        }

    ~TMatrix3x3()
        {}

    TMatrix3x3 & operator = (const TMatrix3x3 & m)
        {
        v00 = m.v00;
        v01 = m.v01;
        v02 = m.v02;
        v10 = m.v10;
        v11 = m.v11;
        v12 = m.v12;
        v20 = m.v20;
        v21 = m.v21;
        v22 = m.v22;
        return *this;
        }

    TRT_BOOL operator == (const TMatrix3x3 & m) const
        {
        return (
            v00 == m.v00 &&
            v01 == m.v01 &&
            v02 == m.v02 &&
            v10 == m.v10 &&
            v11 == m.v11 &&
            v12 == m.v12 &&
            v20 == m.v20 &&
            v21 == m.v21 &&
            v22 == m.v22
                );
        }

    bool operator != (const TMatrix3x3 & m) const
        {
        return !(*this == m);
        }

    TVector2 operator * (const TVector2 & vi) const
        {
        TVector2 t;
        t.v0 = vi.v0 * v00 + vi.v1 * v01 + 1.0f * v02;
        t.v1 = vi.v0 * v10 + vi.v1 * v11 + 1.0f * v12;
        return t;
        }

    TMatrix3x3 operator * (const TMatrix3x3 & m ) const
        {
        return TMatrix3x3(
            v00 * m.v00 + v01 * m.v10 + v02 * m.v20,
            v00 * m.v01 + v01 * m.v11 + v02 * m.v21,
            v00 * m.v02 + v01 * m.v12 + v02 * m.v22,
            v10 * m.v00 + v11 * m.v10 + v12 * m.v20,
            v10 * m.v01 + v11 * m.v11 + v12 * m.v21,
            v10 * m.v02 + v11 * m.v12 + v12 * m.v22,
            v20 * m.v00 + v21 * m.v10 + v22 * m.v20,
            v20 * m.v01 + v21 * m.v11 + v22 * m.v21,
            v20 * m.v02 + v21 * m.v12 + v22 * m.v22);
        }

    TMatrix3x3 operator * (TRT_FLOAT f)
        {
        TMatrix3x3 t(*this);
        t *= f;
        return t;
        }

    TMatrix3x3 operator + (const TMatrix3x3 & m)
        {
        TMatrix3x3 t(*this);
        t += m;
        return t;
        }

    TMatrix3x3 operator - (const TMatrix3x3 & m)
        {
        TMatrix3x3 t(*this);
        t -= m;
        return t;
        }

    TMatrix3x3 Transposed()
        {
        TMatrix3x3 t(*this);
        t.Transpose();
        return t;
        }

    TMatrix3x3 Inverted()
        {
        TMatrix3x3 t(*this);
        t.Invert();
        return t;
        }

    void operator *= (const TMatrix3x3 & m)
        {
        *this = *this * m;
        }

    void operator *= (TRT_FLOAT f)
        {
        v00 *= f;
        v01 *= f;
        v02 *= f;
        v10 *= f;
        v11 *= f;
        v12 *= f;
        v20 *= f;
        v21 *= f;
        v22 *= f;
        }

    void operator += (const TMatrix3x3 & m)
        {
        v00 += m.v00;
        v01 += m.v01;
        v02 += m.v02;
        v10 += m.v10;
        v11 += m.v11;
        v12 += m.v12;
        v20 += m.v20;
        v21 += m.v21;
        v22 += m.v22;
        }

    void operator -= (const TMatrix3x3 & m)
        {
        v00 -= m.v00;
        v01 -= m.v01;
        v02 -= m.v02;
        v10 -= m.v10;
        v11 -= m.v11;
        v12 -= m.v12;
        v20 -= m.v20;
        v21 -= m.v21;
        v22 -= m.v22;
        }

    TMatrix3x3 operator - () const
        {
        return TMatrix3x3( -v00,-v01,-v02, -v10,-v11,-v12, -v20,-v21,-v22);
        }

    void LoadIdentity()
        {
        v00 = 1.0f;
        v01 = 0.0f;
        v02 = 0.0f;
        v10 = 0.0f;
        v11 = 1.0f;
        v12 = 0.0f;
        v20 = 0.0f;
        v21 = 0.0f;
        v22 = 1.0f;
        }

    TRT_BOOL IsIdentity() const
        {
        return (
            v00 == 1.0f &&
            v01 == 0.0f &&
            v02 == 0.0f &&
            v10 == 0.0f &&
            v11 == 1.0f &&
            v12 == 0.0f &&
            v20 == 0.0f &&
            v21 == 0.0f &&
            v22 == 1.0f
                );
        }

    void Transpose()
        {
        TRT_FLOAT t = v10;
        v10 = v01;
        v01 = t;

        t = v20;
        v20 = v02;
        v02 = t;

        t = v21;
        v21 = v12;
        v12 = t;
        }

    void Invert();

    TRT_FLOAT Det() const
        {
        return v00 * (v11*v22 - v21*v12) + v01 * (v20*v12 - v10*v22) + v02 * (v10*v21 - v20*v11);
        }

    TRT_BOOL IsAffine() const
        {
        if (v20 == 0.0f && v21 == 0.0f && v22 == 1.0f)
            return TRT_TRUE;
        return TRT_FALSE;
        }

    TMatrix3x3 & Translate(TRT_FLOAT x, TRT_FLOAT y)
        {
        *this *= TMatrix3x3( 1.0f, 0.0f, x,
                            0.0f, 1.0f, y,
                            0.0f, 0.0f, 1.0f);
        return *this;
        }

    TMatrix3x3 & Scale(TRT_FLOAT sx, TRT_FLOAT sy)
        {
        *this *= TMatrix3x3( sx, 0.0f,  0.0f,
                            0.0f,  sy, 0.0f,
                            0.0f,  0.0f,  1.0f );
        return *this;
        }

    TMatrix3x3 & Rotate(TRT_FLOAT degree);

    TRT_FLOAT V00() const {return v00;}
    TRT_FLOAT V01() const {return v01;}
    TRT_FLOAT V02() const {return v02;}
    TRT_FLOAT V10() const {return v10;}
    TRT_FLOAT V11() const {return v11;}
    TRT_FLOAT V12() const {return v12;}
    TRT_FLOAT V20() const {return v20;}
    TRT_FLOAT V21() const {return v21;}
    TRT_FLOAT V22() const {return v22;}

private:
    TRT_FLOAT   v00, v01, v02, v10, v11, v12, v20, v21, v22;
    };

#endif /* _INCLUDE_tgsPrimitive_H_ */


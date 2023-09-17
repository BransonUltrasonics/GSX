/* tgsSmartPointer.h - tilcon shared data pointer template */

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
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the tilcon's smart pointer template.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tgsSmartPointer_H_
#define _INCLUDE_tgsSmartPointer_H_

template<typename T>
class TAutoPtr
    {
public:
    /* the managed pointer shall be created by operator new */
    TAutoPtr(T * _ptr) : ptr(_ptr)
        {
        }

    ~TAutoPtr()
        {
        if (ptr)
            delete ptr;
        }

    void Reset(T * _ptr)
        {
        if (ptr)
            {
            delete ptr;
            }
        ptr = _ptr;
        }

    T & operator *()
        {
        return *ptr;
        }

    T * operator ->()
        {
        return ptr;
        }

private:
    /* not copyable */
    TAutoPtr<T> & operator = (const TAutoPtr<T> &)
        {
        return *this;
        }

    T * ptr;
    };

template<typename T>
class TSharedPtr
    {
public:

    TSharedPtr() :
        ptr(TRT_NULL),
        ref(TRT_NULL)
        {
        }

    explicit TSharedPtr(T * _ptr) :
        ptr( _ptr),
		ref(TRT_NULL)
        {
        if (ptr)
            {
            ref = new int(0);
            Ref();
            }
        }

    TSharedPtr(const TSharedPtr& sp) :
        ptr(sp.ptr),
		ref(sp.ref)
        {
        if (ptr)
            {
            Ref();
            }
        }

    ~TSharedPtr()
        {
        if (ptr)
            {
            Deref();
            }
        }

    TRT_BOOL IsNull() const
        {
        return ptr == (T*)0;
        }

    void Reset()
        {
        if (ptr)
            {
            Deref();
            }
        ptr = 0;
        ref = 0;
        }

    TSharedPtr<T> & operator = (T* _ptr)
        {
        if (ptr)
            {
            Deref();
            }
        ptr = _ptr;
        if (ptr)
            {
		    ref = new int(0);
            Ref();
            }
        return *this;
        }

    TSharedPtr & operator = (const TSharedPtr & sp)
        {
        if(ptr != sp.ptr)
            {
            if (ptr)
                {
                Deref();
                }
            ptr = sp.ptr;
            ref = sp.ref;
            if (ptr)
                {
                Ref();
                }
            }
        return *this;
        }

    bool operator == (const TSharedPtr & sp)
        {
        return ptr == sp.ptr;
        }

    T & operator *()
        {
        return *ptr;
        };

    const T & operator *() const
        {
        return *ptr;
        };

    T * operator ->()
        {
        return ptr;
        };

    const T * operator ->() const
        {
        return ptr;
        };

    template<typename C>
    TSharedPtr<C> Cast() const
        {
        C * c = static_cast<C *>(ptr);
        return TSharedPtr<C>(c);
        }

private:

	void Ref() const
        {
        (*ref)++;
        }
	void Deref() const
        {
        if (0 == --(*ref))
            {
            delete ptr;
            delete ref;
            }
        }

    T*  ptr;
	mutable int * ref;
    };

#endif /* _INCLUDE_tgsSmartPointer_H_ */


/* tgsArray.h - tilcon array template */

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
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the tilcon's array template.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tgsArray_H_
#define _INCLUDE_tgsArray_H_

template<typename T>
class TArray
    {
public:
    TArray()
        {
        capacity = 0;
        count = 0;
        buffer = 0;
        }
    TArray(TRT_S32 _count)
        {
        capacity = 0;
        count = 0;
        buffer = 0;
        ExpandAt(0, _count);
        }
    TArray(const TArray<T> & list)
        {
        capacity = 0;
        count = 0;
        buffer = 0;
        CopyFrom(list);
        }

    virtual ~TArray()
        {
        if (buffer)
            {
            delete[](buffer);
            buffer = 0;
            }
        }

    void InsertAt(TRT_S32 index, const T & item);
    void InsertLast(const T & item)
        {
        InsertAt(Count(), item);
        }
    void InsertFirst(const T & item)
        {
        InsertAt(0, item);
        }
    void RemoveAt(TRT_S32 index);
    void InsertAt(TRT_S32 index, const TArray<T> &);
    void RemoveAt(TRT_S32 index, TRT_S32 size);
    void Remove(const T & item);
    TRT_BOOL Contains(const T & item) const;
    TRT_S32 IndexOf(const T & item) const;
    TRT_S32 Diff(const TArray<T> &);
    TArray<T> & operator = (const TArray<T> list);
    void Clear();
    T & operator[](TRT_S32 index) const;
    TRT_S32 Count() const
        {
        return count;
        }

protected:
    void CopyFrom(const TArray<T> &);
    void ExpandAt(TRT_S32 index, TRT_S32 size);
    void ShrinkAt(TRT_S32 index, TRT_S32 size);
    TRT_S32 RoundCapacity(TRT_S32);
    TRT_S32 count;
    TRT_S32 capacity;
    T * buffer;
    };

template<typename T>
void TArray<T>::InsertAt(TRT_S32 index, const T & item)
    {
    if (index == -1)
        {
        index = count;
        }
    if ((index < 0) || (index > count))
        {
        throw TException(TException::ET_index);
        }
    ExpandAt(index, 1);
    buffer[index] = item;
    }

template<typename T>
void TArray<T>::InsertAt(TRT_S32 index, const TArray<T> & list)
    {
    if (index == -1)
        {
        index = count;
        }
    if ((index < 0) || (index > count))
        {
        throw TException(TException::ET_index);
        }
    ExpandAt(index, list.Count());
    for (TRT_S32 i=0; i< list.Count(); i++)
        {
        buffer[i+index] = list[i];
        }
    }

template<typename T>
void TArray<T>::RemoveAt(TRT_S32 index)
    {
    if (index == -1)
        {
        index = count - 1;
        }
    if ((index < 0) || (index >= count))
        {
        throw TException(TException::ET_index);
        }
    ShrinkAt(index, 1);
    }

template<typename T>
void TArray<T>::RemoveAt(TRT_S32 index, TRT_S32 size)
    {
    if (index == -1)
        {
        index = count - 1;
        }
    if ((index < 0) || (index >= count))
        {
        throw TException(TException::ET_index);
        }
    ShrinkAt(index, size);
    }

template<typename T>
void TArray<T>::Remove(const T & item)
    {
    TRT_S32 index = IndexOf(item);
    if (index >= 0)
        {
        RemoveAt(index);
        }
    }

template<typename T>
TRT_BOOL TArray<T>::Contains(const T & item) const
    {
    return (IndexOf(item) != -1);
    }

template<typename T>
TRT_S32 TArray<T>::IndexOf(const T & item) const
    {
    for(TRT_S32 i=0; i < count; i++)
        {
        if ( buffer[i] == item)
            return i;
        }
        return -1;
    }

template<typename T>
TRT_S32 TArray<T>::Diff(const TArray<T> & list)
    {
    TRT_S32 diff = Count() < list.Count() ? Count() : list.Count();
    for (TRT_S32 i = 0; i<diff;i++)
        {
        if (buffer[i] != list[i])
            {
            diff = i;
            break;
            }
        }
    return diff;
    }

template<typename T>
TArray<T> & TArray<T>::operator = (const TArray<T> list)
    {
    CopyFrom(list);

    return *this;
    }
template<typename T>
void TArray<T>::Clear()
    {
    if(buffer)
        {
        delete[](buffer);
        buffer = 0;
        }
    capacity = 0;
    count = 0;
    }

template<typename T>
T & TArray<T>::operator[](TRT_S32 index) const
    {
    if ((index < 0) || (index >= count))
        {
        throw TException(TException::ET_index);
        }
    return buffer[index];
    }

template<typename T>
void TArray<T>::CopyFrom(const TArray<T> & list)
    {
    TRT_S32 new_capacity = list.capacity;

    T * new_buffer = new T[new_capacity];
    if (NULL == new_buffer)
        {
        throw TException(TException::ET_memory_alloc);
        }
    for (TRT_S32 i=0; i< list.Count(); i++)
        {
        new_buffer[i] = list[i];
        }
    delete[](buffer);
    buffer = new_buffer;
    capacity = new_capacity;
    count = list.Count();
    }

template<typename T>
void TArray<T>::ExpandAt(TRT_S32 index, TRT_S32 size)
    {
    TRT_S32 new_capacity = capacity;
    if ((count + size) > capacity)
        {
        new_capacity = RoundCapacity(count + size);
        }

    if (new_capacity != capacity)
        {
        T * new_buffer = new T[new_capacity];
        if (NULL == new_buffer)
            {
            throw TException(TException::ET_memory_alloc);
            }
        if (buffer)
            {
            for (TRT_S32 i=0; i< index; i++)
                {
                new_buffer[i] = buffer[i];
                }
            for (TRT_S32 i=count-1; i >= index; i--)
                {
                new_buffer[i + size] = buffer[i];
                }
            delete[](buffer);
            }
        buffer = new_buffer;
        capacity = new_capacity;
        }
    else
        {
        if (buffer)
            {
            for (TRT_S32 i=count-1; i >= index; i--)
                {
                buffer[i + size] = buffer[i];
                }
            }
        }
    count += size;
    }

template<typename T>
void TArray<T>::ShrinkAt(TRT_S32 index, TRT_S32 size)
    {
    TRT_S32 new_capacity = capacity;
    if ((count - size) < (capacity >> 1))
        {
        new_capacity = RoundCapacity(count - size);
        }

    if (new_capacity != capacity)
        {
        T * new_buffer = new T[new_capacity];
        if (NULL == new_buffer)
            {
            throw TException(TException::ET_memory_alloc);
            }
        if (buffer)
            {
            for (TRT_S32 i=0; i< index; i++)
                {
                new_buffer[i] = buffer[i];
                }
            for (TRT_S32 i=index; i < count - size; i++)
                {
                new_buffer[i] = buffer[i + size];
                }
            }
        delete [] buffer;
        buffer = new_buffer;
        capacity = new_capacity;
        }
    else
        {
        if (buffer)
            {
            for (TRT_S32 i=index; i < count - size; i++)
                {
                buffer[i] = buffer[i + size];
                }
            }
        }
    count -= size;
    }

template<typename T>
TRT_S32 TArray<T>::RoundCapacity(TRT_S32 _size)
    {
    if (_size <= 2)
        {
        return 2;
        }
    else
        {
        TRT_S32 new_capacity = 2;
        while (new_capacity < _size)
            {
            new_capacity = new_capacity << 1;
            }
        return new_capacity;
        }
    }

#endif /* _INCLUDE_tgsArray_H_ */


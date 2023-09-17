/* tgsPairArray.h - tilcon pair array template */

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

This file defines the tilcon's pair array template.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tgsPairArray_H_
#define _INCLUDE_tgsPairArray_H_

template<typename K, typename V>
class Pair
    {
    public:
        K key;
        V value;
    public:
        Pair(){};
        Pair(K _key, V _value) : key(_key), value(_value){};
        ~Pair(){};

        TRT_BOOL operator == (const Pair & pair)
            {return pair.key == key;}
     };

template<typename K, typename V>
class TPairArray : public TArray<Pair<K, V> >
{

public:
    TPairArray()
        {
        };
    virtual ~TPairArray()
        {
        };

    int IndexOf(const K & key);
};

template<typename K, typename V>
int TPairArray<K, V>::IndexOf(const K & key)
    {
    for (int i=0; i < TArray<Pair<K, V> >::count; i++)
        {
        if (key == TArray<Pair<K, V> >::buffer[i].key)
            {
            return i;
            }
        }
    return -1;
    }

#endif /* _INCLUDE_tgsPairArray_H_ */


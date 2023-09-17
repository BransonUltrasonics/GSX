/* tgsWideString.h - tilcon wide string header */

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

This file defines the tilcon's wide string type.

INCLUDE FILES: tilcon/basic/tgsBasic.h
*/

#ifndef _INCLUDE_tgsWideString_H_
#define _INCLUDE_tgsWideString_H_

class TWString
    {
public:
    TWString()
        {
        len = 0;
        buf = (TRT_WCHAR *)new TRT_WCHAR[len + 1];
        buf[0] = '\0';
        }

    TWString(const TRT_WCHAR * text)
        {
        if (TRT_NULL == text)
            {
            throw TException(TException::ET_parameter);
            }
        TRT_S32 text_len = textlen(text);
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';
        textncpy(text_buf, text, text_len);

        buf = text_buf;
        len = text_len;
        }

    TWString(const TRT_WCHAR * text, TRT_S32 text_len)
        {
        if (TRT_NULL == text)
            {
            throw TException(TException::ET_parameter);
            }
        text_len = text_len < textlen(text) ? text_len : textlen(text);
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';
        textncpy(text_buf, text, text_len);

        buf = text_buf;
        len = text_len;
        }

    TWString(const TWString & string)
        {
        TRT_S32 text_len = string.Len();
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';
        textncpy(text_buf, string.Text(), text_len);

        buf = text_buf;
        len = text_len;
        }

    TWString(const TString & string)
        {
        TRT_S32 text_len = string.Len();
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';

        for (int i = 0; i < text_len; i++)
            {
            text_buf[i] = string.Text()[i];
            }
        buf = text_buf;
        len = text_len;
        }

    ~TWString()
        {
        delete [] buf;
        }

     TWString & operator = (const TWString & string)
        {
        if (string.buf != buf)
            {
            TRT_S32 text_len = string.Len();
            TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
            text_buf[text_len] = '\0';
            textncpy(text_buf, string.Text(), text_len);

            delete [] buf;
            buf = text_buf;
            len = text_len;
            }
        return *this;
        }

    void SetText(const TRT_WCHAR * text)
        {
        if (TRT_NULL == text)
            {
            throw TException(TException::ET_parameter);
            }

        TRT_S32 text_len = textlen(text);
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';
        textncpy(text_buf, text, text_len);

        delete [] buf;
        buf = text_buf;
        len = text_len;
        }

    TWString Left(TRT_S32 count) const
        {
        if (count > Len())
            {
            count = Len();
            }
        return TWString(Text(), count);
        }

    void Replace(TRT_S32 index, TRT_S32 count, const TWString & str)
        {
        if (index < 0)
            {
            index = 0;
            }

        if (index > Len())
            {
            index = Len();
            }

        if (count > Len() - index)
            {
            count = Len() - index;
            }

        TRT_S32 text_len = Len() + str.Len() - count;
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';
        textncpy(text_buf, Text(), index);
        textncpy(text_buf + index, str.Text(), str.Len());
        textncpy(text_buf + index + str.Len(), Text() + index + count, Len() - (index + count));

        delete [] buf;
        buf = text_buf;
        len = text_len;
        }

    TWString & Remove(TRT_S32 index, TRT_S32 count)
        {
        if (0 == Len())
            {
            return *this;
            }

        if (index < 0)
            {
            index = 0;
            }
        if (index > Len())
            {
            index = Len();
            }

        if (count > Len() - index)
            {
            count = Len() - index;
            }

        if (Len() > 0 && count > 0)
            {
            TRT_S32 head_cnt = index;
            TRT_S32 tail_index = index + count;
            TRT_S32 tail_cnt = Len() - tail_index;

            TRT_S32 text_len = Len() - count;
            TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
            text_buf[text_len] = '\0';
            textncpy(text_buf, Text(), head_cnt);
            textncpy(text_buf + head_cnt, Text() + tail_index, tail_cnt);

            delete [] buf;
            buf = text_buf;
            len = text_len;
            }
        return * this;
        }

     TWString & Add(const TWString & string)
        {
        TRT_S32 text_len = Len() + string.Len();
        TRT_WCHAR * text_buf = (TRT_WCHAR *)new TRT_WCHAR[text_len + 1];
        text_buf[text_len] = '\0';
        textncpy(text_buf, Text(), Len());
        textncpy(text_buf + Len(), string.Text(), string.Len());

        delete [] buf;
        buf = text_buf;
        len = text_len;

        return *this;
        }

    TRT_BOOL operator != (const TWString & string) const
        {
        if (Len() != string.Len())
            {
            return TRT_TRUE;
            }
        for (int i=0; i<Len(); i++)
            {
            if ((*this)[i] != string[i])
                {
                return TRT_TRUE;
                }
            }
        return TRT_FALSE;
        }

    TRT_BOOL operator == (const TWString & string) const
        {
        if (Len() != string.Len())
            {
            return TRT_FALSE;
            }
        for (int i=0; i<Len(); i++)
            {
            if ((*this)[i] != string[i])
                {
                return TRT_FALSE;
                }
            }
        return TRT_TRUE;
        }

     TWString & operator += (const TWString & string)
        {
        return Add(string);
        }

     TWString operator + (const TWString & string)
        {
        TWString ret = TWString(*this);
        return ret.Add(string);
        }

     TRT_S32 Find(TRT_S32 index, TRT_WCHAR dst) const
        {
        for (int i = index; i<Len(); i++)
            {
            if (dst == (*this)[i])
                {
                return i;
                }
            }
        return -1;
        }

     TRT_WCHAR operator[] (TRT_S32 index) const
        {
        return Text()[index];
        }

    TRT_S32 Len() const
        {
        return len;
        }

    const TRT_WCHAR * Text() const
        {
        return buf;
        }

private:
    static TRT_S32 twcslen(TRT_LPCWSTR str)
        {
        TRT_S32 len = 0;
        while (*(str+len))
            {
            len++;
            }
        return len;
        }

    static TRT_LPWSTR twcsncpy(TRT_LPWSTR dst, TRT_LPCWSTR src, TRT_S32 n)
        {
        return (TRT_LPWSTR)memcpy(dst, src, n * sizeof(TRT_WCHAR));
        }

    static TRT_S32 textlen(TRT_LPCWSTR text)
        {
        return twcslen(text);
        }

    static TRT_LPWSTR textncpy(TRT_LPWSTR dst, TRT_LPCWSTR src, TRT_S32 n)
        {
        return twcsncpy(dst, src, n);
        }

    TRT_WCHAR * buf;
    TRT_S32     len;
    };

#endif /* _INCLUDE_tgsWideString_H_ */


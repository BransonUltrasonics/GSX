/* tgsAnsiString.h - tilcon ansi string header */

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
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the tilcon's ansi string type.

INCLUDE FILES:
*/

#ifndef _INTRT_BYTELUDE_clsString_H_
#define _INTRT_BYTELUDE_clsString_H_

class TWString;
class TString
    {
public:
    TString();
    TString(const TRT_CHAR * text);
    TString(const TRT_CHAR * text, TRT_S32 text_len);
    TString(const TString & string);
    TString(const TWString & string);

    ~TString()
        {
        if (buf)
            {
            free(buf);
            }
        }

    TString & operator = (const TString & string);
    void SetText(const TRT_CHAR * text);
    void Replace(TRT_S32 index, TRT_S32 count, const TString & str);
    TString Left(TRT_S32 count);
    TString & Remove(TRT_S32 index, TRT_S32 count);
    TString & Add(const TString & string);
    TRT_BOOL operator != (const TString & string) const;
    TRT_BOOL operator == (const TString & string) const;

    TString & operator += (const TString & string)
        {
        return Add(string);
        }

    TString operator + (const TString & string)
        {
        TString ret = TString(*this);
        return ret.Add(string);
        }

    TRT_S32 Find(TRT_S32 index, TRT_CHAR dst) const;
    void Format(TRT_S32 n, const TRT_CHAR * format, ...);

     TRT_CHAR operator[] (TRT_S32 index) const
        {
        return buf[index];
        }

    TRT_S32 Len() const
        {
        return len;
        }

    const TRT_CHAR * Text() const
        {
        return buf;
        }
private:
    static TRT_S32 textlen(TRT_LPCSTR text)
        {
        return (TRT_S32)strlen(text);
        }

    static TRT_LPSTR textncpy(TRT_LPSTR dst, TRT_LPCSTR src, TRT_S32 n)
        {
        return strncpy(dst, src, n);
        }

    TRT_CHAR * buf;
    TRT_S32 len;
    };

#endif /* _INTRT_BYTELUDE_clsString_H_ */


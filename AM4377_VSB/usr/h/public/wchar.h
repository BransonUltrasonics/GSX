/* wchar.h standard header */
#ifndef _WCHAR
#define _WCHAR
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_STD_BEGIN

#ifndef _NO_WINDRIVER_MODIFICATIONS
#include <stdarg.h>
#if !defined(__VA_LIST)
#define __VA_LIST
typedef va_list _Va_list;
#endif
#if defined(_C_IN_NS)
/* should <cstdarg> retrieve va_list from :: or from std::? */
#define _WR_VA_LIST_DEFINED_IN_STD 1
#endif /* _C_IN_NS */
#endif /* _NO_WINDRIVER_MODIFICATIONS */

		/* MACROS */
#ifndef NULL
 #define NULL	_NULL
#endif /* NULL */

#define WCHAR_MIN	_WCMIN
#define WCHAR_MAX	_WCMAX
#define WEOF	((_CSTD wint_t)(-1))

#ifndef _Mbstinit

 #ifdef __cplusplus
#define _Mbstinit(x)	_Mbstatet x

 #else /* __cplusplus */
#define _Mbstinit(x)	_Mbstatet x = {0}
 #endif /* __cplusplus */

#endif /* _Mbstinit */

		/* TYPE DEFINITIONS */
#ifndef _MBSTATET
 #define _MBSTATET
typedef struct _Mbstatet
	{	/* state of a multibyte translation */
	unsigned long _Wchar;
	unsigned short _Byte, _State;

 #ifdef __cplusplus
	_Mbstatet()
		: _Wchar(0), _Byte(0), _State(0)
		{	// default construct
		}

	_Mbstatet(const _Mbstatet& _Right)
		: _Wchar(_Right._Wchar), _Byte(_Right._Byte),
			_State(_Right._State)
		{	// copy construct
		}

	_Mbstatet& operator=(const _Mbstatet& _Right)
		{	// assign
		_Wchar = _Right._Wchar;
		_Byte = _Right._Byte;
		_State = _Right._State;
		return (*this);
		}
 #endif /* __cplusplus */
	} _Mbstatet;
#endif /* _MBSTATET */

typedef _Mbstatet mbstate_t;
_C_STD_END

#include <b_std_size_t.h>

_C_STD_BEGIN
struct tm;
struct _Dnk_filet;

 #ifndef _FILET
  #define _FILET
typedef struct _Dnk_filet _Filet;
 #endif /* _FILET */

 #if !defined(_WCHART) && !defined(_WCHAR_T_DEFINED)
  #define _WCHART
  #define _WCHAR_T_DEFINED
typedef _Wchart wchar_t;
 #endif /* _WCHART etc. */

 #if 1200 <= _MSC_VER
 #ifndef _WCTYPE_T_DEFINED
  #define _WCTYPE_T_DEFINED
  #ifndef _WCTYPET
   #define _WCTYPET
typedef _Sizet wctype_t;
  #endif /* _WCTYPET */

  #ifndef _WINTT
   #define _WINTT
typedef _Wintt wint_t;
  #endif /* _WINTT */

 #endif /* _WCTYPE_T_DEFINED */

 #else /* 1200 <= _MSC_VER */
 #ifndef _WINTT
  #define _WINTT
typedef _Wintt wint_t;
 #endif /* _WINTT */

 #endif /* 1200 <= _MSC_VER */

_C_LIB_DECL
		/* stdio DECLARATIONS */
wint_t fgetwc(_Filet *);
wchar_t *fgetws(wchar_t *_Restrict, int,
	_Filet *_Restrict);
wint_t fputwc(wchar_t, _Filet *);
int fputws(const wchar_t *_Restrict,
	_Filet *_Restrict);
int fwide(_Filet *, int);
int fwprintf(_Filet *_Restrict,
	const wchar_t *_Restrict, ...);
int fwscanf(_Filet *_Restrict,
	const wchar_t *_Restrict, ...);
wint_t getwc(_Filet *);
wint_t getwchar(void);
wint_t putwc(wchar_t, _Filet *);
wint_t putwchar(wchar_t);
int swprintf(wchar_t *_Restrict, size_t,
	const wchar_t *_Restrict, ...);
int swscanf(const wchar_t *_Restrict,
	const wchar_t *_Restrict, ...);
wint_t ungetwc(wint_t, _Filet *);
int vfwprintf(_Filet *_Restrict,
	const wchar_t *_Restrict, _Va_list);
int vswprintf(wchar_t *_Restrict, size_t,
	const wchar_t *_Restrict, _Va_list);
int vwprintf(const wchar_t *_Restrict, _Va_list);
int wprintf(const wchar_t *_Restrict, ...);
int wscanf(const wchar_t *_Restrict, ...);

 #if _HAS_C9X
int vfwscanf(_Filet *_Restrict,
	const wchar_t *_Restrict, _Va_list);
int vswscanf(const wchar_t *_Restrict,
	const wchar_t *_Restrict, _Va_list);
int vwscanf(const wchar_t *_Restrict, _Va_list);
 #endif /* _IS_C9X */

		/* stdlib DECLARATIONS */
size_t mbrlen(const char *_Restrict,
	size_t, mbstate_t *_Restrict);
size_t mbrtowc(wchar_t *_Restrict, const char *,
	size_t, mbstate_t *_Restrict);
size_t mbsrtowcs(wchar_t *_Restrict,
	const char **_Restrict, size_t, mbstate_t *_Restrict);
int mbsinit(const mbstate_t *);
size_t wcrtomb(char *_Restrict,
	wchar_t, mbstate_t *_Restrict);
size_t wcsrtombs(char *_Restrict,
	const wchar_t **_Restrict, size_t, mbstate_t *_Restrict);
long wcstol(const wchar_t *_Restrict,
	wchar_t **_Restrict, int);

 #if _HAS_C9X
_Longlong wcstoll(const wchar_t *_Restrict,
	wchar_t **_Restrict, int);
_ULonglong wcstoull(const wchar_t *_Restrict,
	wchar_t **_Restrict, int);
 #endif /* _IS_C9X */

		/* string DECLARATIONS */
wchar_t *wcscat(wchar_t *_Restrict, const wchar_t *_Restrict);
int wcscmp(const wchar_t *, const wchar_t *);
wchar_t *wcscpy(wchar_t *_Restrict, const wchar_t *_Restrict);
size_t wcslen(const wchar_t *);
int wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wcsncpy(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);

int wcscoll(const wchar_t *, const wchar_t *);
size_t wcscspn(const wchar_t *, const wchar_t *);
wchar_t *wcsncat(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);
size_t wcsspn(const wchar_t *, const wchar_t *);
wchar_t *wcstok(wchar_t *_Restrict, const wchar_t *_Restrict,
	wchar_t **_Restrict);
size_t wcsxfrm(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);
int wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wmemcpy(wchar_t *_Restrict,
	const wchar_t *_Restrict, size_t);
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);

		/* time DECLARATIONS */
size_t wcsftime(wchar_t *_Restrict, size_t,
	const wchar_t *_Restrict, const struct tm *_Restrict);

wint_t _Btowc(int);
int _Wctob(wint_t);
double _WStod(const wchar_t *, wchar_t **, long);
float _WStof(const wchar_t *, wchar_t **, long);
long double _WStold(const wchar_t *, wchar_t **, long);
unsigned long _WStoul(const wchar_t *, wchar_t **, int);
_END_C_LIB_DECL

 #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
		// INLINES AND OVERLOADS, FOR C++
  #define _WConst_return const

_C_LIB_DECL
const wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
_END_C_LIB_DECL

extern "C++" {
inline wchar_t *wmemchr(wchar_t *_Str, wchar_t _Ch, size_t _Num)
	{	// call with const first argument
	return ((wchar_t *)wmemchr((const wchar_t *)_Str, _Ch, _Num));
	}
}	// extern "C++"

 #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
  #define _WConst_return

_C_LIB_DECL
wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
_END_C_LIB_DECL
 #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */

 #include <xwcstod.h>
 #include <xwstr.h>

 #if __STDC_WANT_LIB_EXT1__
_C_LIB_DECL

  #if !defined(_ERRNO_T_DEFINED)
   #define _ERRNO_T_DEFINED
typedef int errno_t;
  #endif /* _ERRNO_T_DEFINED */

  #if !defined(_RSIZE_T_DEFINED)
   #define _RSIZE_T_DEFINED
typedef size_t rsize_t;
  #endif /* _RSIZE_T_DEFINED */

int fwprintf_s(_Filet *_Restrict,
	const wchar_t *_Restrict, ...);
int fwscanf_s(_Filet *_Restrict,
	const wchar_t *_Restrict, ...);
int snwprintf_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict, ...);
int swprintf_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict, ...);
int swscanf_s(const wchar_t *_Restrict,
	const wchar_t *_Restrict, ...);
int vfwprintf_s(_Filet *_Restrict,
	const wchar_t *_Restrict,
	_Va_list);
int vfwscanf_s(_Filet *_Restrict,
	const wchar_t *_Restrict,
	_Va_list);
int vsnwprintf_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict,
	_Va_list);
int vswprintf_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict,
	_Va_list);
int vswscanf_s(const wchar_t *_Restrict,
	const wchar_t *_Restrict,
	_Va_list);
int vwprintf_s(const wchar_t *_Restrict,
	_Va_list);
int vwscanf_s(const wchar_t *_Restrict,
	_Va_list);
int wprintf_s(const wchar_t *_Restrict, ...);
int wscanf_s(const wchar_t *_Restrict, ...);

errno_t wcscpy_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict);
errno_t wcsncpy_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict, rsize_t);
errno_t wmemcpy_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict, rsize_t);
errno_t wmemmove_s(wchar_t *, rsize_t,
	const wchar_t *, rsize_t);
errno_t wcscat_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict);
errno_t wcsncat_s(wchar_t *_Restrict, rsize_t,
	const wchar_t *_Restrict, rsize_t);
wchar_t *wcstok_s(wchar_t *_Restrict, rsize_t *_Restrict,
	const wchar_t *_Restrict, wchar_t **_Restrict);

size_t wcsnlen_s(const wchar_t *, size_t);

errno_t wcrtomb_s(size_t *_Restrict,
	char *_Restrict, rsize_t,
	wchar_t,
	mbstate_t *_Restrict);
errno_t mbsrtowcs_s(size_t *_Restrict,
	wchar_t *_Restrict, rsize_t,
	const char **_Restrict, rsize_t,
	mbstate_t *_Restrict);
errno_t wcsrtombs_s(size_t *_Restrict,
	char *_Restrict, rsize_t,
	const wchar_t **_Restrict, rsize_t,
	mbstate_t *_Restrict);
_END_C_LIB_DECL
 #endif /* __STDC_WANT_LIB_EXT1__ */
_C_STD_END
#endif /* _WCHAR */

 #if defined(_STD_USING)

  #ifdef _STD_USING_SIZE_T
using _CSTD size_t;
  #endif /* _STD_USING_SIZE_T */

using _CSTD mbstate_t; using _CSTD tm; using _CSTD wint_t;

using _CSTD btowc; using _CSTD fgetwc; using _CSTD fgetws; using _CSTD fputwc;
using _CSTD fputws; using _CSTD fwide; using _CSTD fwprintf;
using _CSTD fwscanf; using _CSTD getwc; using _CSTD getwchar;
using _CSTD mbrlen; using _CSTD mbrtowc; using _CSTD mbsrtowcs;
using _CSTD mbsinit; using _CSTD putwc; using _CSTD putwchar;
using _CSTD swprintf; using _CSTD swscanf; using _CSTD ungetwc;
using _CSTD vfwprintf; using _CSTD vswprintf; using _CSTD vwprintf;
using _CSTD wcrtomb; using _CSTD wprintf; using _CSTD wscanf;
using _CSTD wcsrtombs; using _CSTD wcstol; using _CSTD wcscat;
using _CSTD wcschr; using _CSTD wcscmp; using _CSTD wcscoll;
using _CSTD wcscpy; using _CSTD wcscspn; using _CSTD wcslen;
using _CSTD wcsncat; using _CSTD wcsncmp; using _CSTD wcsncpy;
using _CSTD wcspbrk; using _CSTD wcsrchr; using _CSTD wcsspn;
using _CSTD wcstod; using _CSTD wcstoul; using _CSTD wcsstr;
using _CSTD wcstok; using _CSTD wcsxfrm; using _CSTD wctob;
using _CSTD wmemchr; using _CSTD wmemcmp; using _CSTD wmemcpy;
using _CSTD wmemmove; using _CSTD wmemset; using _CSTD wcsftime;

 #if _HAS_C9X
using _CSTD vfwscanf; using _CSTD vswscanf; using _CSTD vwscanf;
using _CSTD wcstof; using _CSTD wcstold;
using _CSTD wcstoll; using _CSTD wcstoull;
 #endif /* _IS_C9X */

 #if __STDC_WANT_LIB_EXT1__
using _CSTD errno_t;
using _CSTD rsize_t;

using _CSTD fwprintf_s;
using _CSTD fwscanf_s;
using _CSTD snwprintf_s;
using _CSTD swprintf_s;
using _CSTD swscanf_s;
using _CSTD vfwprintf_s;
using _CSTD vfwscanf_s;
using _CSTD vsnwprintf_s;
using _CSTD vswprintf_s;
using _CSTD vswscanf_s;
using _CSTD vwprintf_s;
using _CSTD vwscanf_s;
using _CSTD wprintf_s;
using _CSTD wscanf_s;

using _CSTD wcscpy_s;
using _CSTD wcsncpy_s;
using _CSTD wmemcpy_s;
using _CSTD wmemmove_s;
using _CSTD wcscat_s;
using _CSTD wcsncat_s;
using _CSTD wcstok_s;

using _CSTD wcsnlen_s;

using _CSTD wcrtomb_s;
using _CSTD mbsrtowcs_s;
using _CSTD wcsrtombs_s;
 #endif /* __STDC_WANT_LIB_EXT1__ */

 #if defined(_WR_VA_LIST_DEFINED_IN_STD)
using _CSTD va_list;
 #endif /* _WR_VA_LIST_DEFINED_IN_STD */

 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:1278 */

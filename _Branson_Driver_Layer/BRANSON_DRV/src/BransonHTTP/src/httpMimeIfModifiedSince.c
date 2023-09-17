/* httpMimeIfModifiedSince.c - parse functions for cache-related header */

/* Copyright 2011 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01a,20apr11,m_z initiate file
                CQID: WIND00203773
*/

/*
DESCRIPTION
This file contains especially HTTP_REQ access functions which
are useful for cache implementation.

INCLUDE FILES: http/private/httpReq.h
               http/httpLib.h

NOMANUAL
*/

/* includes */
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "stdlib.h"

#include "httpLib.h"
#include "httpReq.h"

#define imsGetWeekDay(pos, wday, sp) \
    {                                                                       \
    char *imsWDay[] ={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", 0};  \
    int imsWDIndex;                                                         \
    for (imsWDIndex = 0; imsWDIndex < 7;  imsWDIndex++)                     \
        if (                                                                \
            (0 == strncmp(pos, imsWDay[imsWDIndex], 3))                     \
            &&                                                              \
            ((pos)[3] == (sp))                                              \
            )                                                               \
            {                                                               \
            (wday) = imsWDIndex;                                            \
            (pos) += 3;                                                     \
            break;                                                          \
            }                                                               \
    if (!imsWDay[imsWDIndex])                                               \
        broken = 1;                                                         \
    }

#define imsGetMonth(pos, mon, sp)   \
    {                                                                       \
    char *imsMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",           \
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", 0};                       \
    int imsMonIndex;                                                        \
    for (imsMonIndex = 0; imsMonIndex < 12; imsMonIndex++)                  \
        if (                                                                \
            (0 == strncmp(pos, imsMonth[imsMonIndex], 3))                   \
            &&                                                              \
            (   (pos)[3] == (sp))                                           \
            )                                                               \
            {                                                               \
            (mon) = imsMonIndex;                                            \
            (pos) += 3;                                                     \
            break;                                                          \
            }                                                               \
    if (!imsMonth[imsMonIndex])                                             \
        broken = 1;                                                         \
    }

#define imsGetDigit(pos, len, digit, sp)    \
    {                                                                       \
        int imsIdx;                                                         \
        int imsTmp;                                                         \
        for (imsIdx = 0; imsIdx < (len); imsIdx++, pos++)                   \
            if ('0' <= *(pos) && *(pos) <= '9')                             \
                {                                                           \
                imsTmp = (digit);                                           \
                (digit) <<= 3;                                              \
                (digit) += (imsTmp << 1);                                   \
                (digit) += *(pos) - '0';                                    \
                }                                                           \
            else if (*(pos) == (sp) && imsIdx > 0)                          \
                break;                                                      \
            else                                                            \
                {                                                           \
                broken = 1;                                                 \
                break;                                                      \
                }                                                           \
    }

#define imsGetMonthDay(pos, mday, sp)   \
    {                                       \
    imsGetDigit((pos), 2, (mday), sp);      \
    if (*(pos) != (sp))                     \
        broken = 1;                         \
    else if ((mday) > 31 || (mday) < 1)     \
        broken = 1;                         \
    }

#define imsGetYear(pos, year, sp)   \
    {                                       \
    imsGetDigit((pos), 4, (year), (sp));    \
    if (*(pos) != (sp))                     \
        broken = 1;                         \
    }

#define imsGetHour(pos, hour, sp)  \
    {                                       \
    imsGetDigit((pos), 2, (hour), (sp));    \
    if (*(pos) != (sp))                     \
        broken = 1;                         \
    else if ((hour) >= 24)                  \
        broken = 1;                         \
    }
#define imsGetMinute(pos, min, sp)  \
    {                                       \
    imsGetDigit((pos), 2, (min), (sp));     \
    if (*(pos) != (sp))                     \
        broken = 1;                         \
    else if ((min) >= 60)                   \
        broken = 1;                         \
    }
#define imsGetSecond(pos, sec, sp)  \
    {                                       \
    imsGetDigit((pos), 2, (sec), (sp));     \
    if (*(pos) != (sp))                     \
        broken = 1;                         \
    else if ((sec) >= 60)                   \
        broken = 1;                         \
    }

#define imsGetTimezone(pos, tz) \
    {                                       \
    for (; *(pos) != '\0'; pos++)           \
        ;                                   \
    }

/*******************************************************************************
*
* httpMimeIfModifiedSince - extract HTTP If-Modified-Since header
*
* This function is called by the cache header parser to extract
* the "If-Modified-Since" header from the client's HTTP request.
* The cache parser calls httpHdrIfModifiedSince() only if "If-Modified-Since:"
* is found in a request header.
* See httpconfig.c to see how cache parsing should be configured or
* manipulated.
*
* NOTE: Do not modify <szLine>.
*
* RETURNS: OK or ERROR.
*/

HTTP_STATUS httpMimeIfModifiedSince
    (
    HTTP_REQ_ID reqId,      /* Handle of the active request */
    char * szLine           /* line to parse                */
    )
    {
    logInfoMsg ("HEADER IF-MODIFIED-SINCE: %s detected",szLine);
    httpSetEnv ( reqId , "If-Modified-Since" , szLine );
    httpSetEnv ( reqId , "IF-MODIFIED-SINCE" , szLine );
    httpSetEnv ( reqId , "IF_MODIFIED_SINCE" , szLine );

    return (httpMimeHdrSet (reqId, HDR_IN, "If-Modified-Since", szLine));
    }

/*******************************************************************************
*
* httpIfModifiedSinceValueGet - get time value from HTTP If-Modified-Since header
*
* This function is called to translate http date string to break down time
*
* RETURNS: OK or ERROR.
*/


HTTP_STATUS httpIfModifiedSinceValueGet
    (
    const char * src, 
    struct tm* dst 
    )
    {
    const char *pos, *ind;
    int i = 0;
    int broken = 0;
    char * fmt[] = {
        "%a, %b %d %Y %H:%M:%S %Z", 
        "%a, %d %b %Y %H:%M:%S %Z", 
        "%a %d %b %H:%M:%S %Y",
        0
        };
 
    for (i = 0; fmt[i]; i++)
        {
        ind = fmt[i];
        pos = src;
        broken = 0;
        for (; (*pos && *ind && !broken); )
            {
            if (*ind == '%')
                {
                ind++;
                while (*pos == ' ' || *pos == '\t')
                    pos++;
                switch(*ind)
                {
                case 'a':
                    ind++;
                    imsGetWeekDay(pos, dst->tm_wday, *(ind));
                    break;
                case 'b':
                    ind++;
                    imsGetMonth(pos, dst->tm_mon, *(ind));
                    break;
                case 'd':
                    ind++;
                    imsGetMonthDay(pos, dst->tm_mday, *(ind));
                    break;
                case 'Y':
                    ind++;
                    imsGetYear(pos, dst->tm_year, *(ind));
                    break;
                case 'H':
                    ind++;
                    imsGetHour(pos, dst->tm_hour, *(ind));
                    break;
                case 'M':
                    ind++;
                    imsGetMinute(pos, dst->tm_min, *(ind));
                    break;
                case 'S':
                    ind++;
                    imsGetSecond(pos, dst->tm_sec, *(ind));
                    break;
                case 'Z':
                    ind++;
                    imsGetTimezone(pos, 0);
                    break;
                default:
                    broken = 1;
                    break;
                }
                }
            else if (*ind == *pos)
                {
                ind++;
                pos++;
                }
            else if (*ind == ' ' || *ind == '\t')
                ind++;
            else
                broken = 1;
            }
        if (*pos == 0 && *ind == 0)
            break;
        }

    if (broken)
        return (HTTP_ERROR);
    return (HTTP_OK);
    }

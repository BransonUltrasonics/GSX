/*-----------------------------------------------------------------------------
 * EcPrintf.cpp             
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              EtherCAT master formatted print routines
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"



/*-DEFINES-------------------------------------------------------------------*/
#define MAX_MINLEN  200

/*-TYPEDEFS------------------------------------------------------------------*/


/*-LOCALS--------------------------------------------------------------------*/


/********************************************************************************/
/**
\brief Print the hex/dec representation of a number into string.
\return length of string
*/
EC_T_INT
EcSprintVal
(EC_T_CHAR* szDest      // [out]  result string
,EC_T_INT nMaxSize      // [in]   maximum size of the result string
,EC_T_DWORD dwVal       // [in]   value to convert
,EC_T_INT nBase         // [in]   base system (16 or 10)
,EC_T_BOOL bUpperCase   // [in]   EC_TRUE if uppercase letters shall be used
)
{
EC_T_INT nTotLen = 0;
EC_T_INT nCurrLen = 0;
EC_T_INT nRemainSize = nMaxSize;

    OsDbgAssert( (nBase == 10) || (nBase == 16) );
    if( nMaxSize == 0 )
    {
        goto Exit;
    }

    if( nBase == 16 )
    {
        if( (dwVal & ~0xf) ) 
        {
            nCurrLen = EcSprintVal(szDest, nRemainSize, dwVal >> 4, nBase, bUpperCase);
            szDest += nCurrLen;
            nTotLen += nCurrLen;
            nRemainSize -= nCurrLen;
            dwVal &= 0xf;
        }
    }
    else
    {
        if( dwVal >=10 ) 
        {
            nCurrLen = EcSprintVal(szDest, nRemainSize, dwVal/10, nBase, bUpperCase);
            szDest += nCurrLen;
            nTotLen += nCurrLen;
            nRemainSize -= nCurrLen;
            dwVal %= 10;
        }
    }
    OsDbgAssert( nRemainSize >= 0);
    if( nRemainSize <= 0 )
    {
        goto Exit;
    }

    if (dwVal < 10) 
    {
        *szDest = (unsigned char)((unsigned char)dwVal + '0');
        nCurrLen = 1;
    } 
    else 
    {
        OsDbgAssert( nBase == 16 );
        if( bUpperCase )
        {
            *szDest = (unsigned char)((unsigned char)dwVal - 10 + 'A');
        }
        else
        {
            *szDest = (unsigned char)((unsigned char)dwVal - 10 + 'a');
        }
        nCurrLen = 1;
    }
    nTotLen += nCurrLen;
    nRemainSize -= nCurrLen;

Exit:
    return nTotLen;
}



/********************************************************************************/
/** 
\brief formatted string printf with vaList
\return length of string (without terminating zero character)
*/
ATECAT_API EC_T_INT EcVsnprintf
(EC_T_CHAR* szDest
 ,EC_T_INT nMaxSize
 ,const 
  EC_T_CHAR* szFormat
 ,EC_T_VALIST vaList
 )
{
EC_T_CHAR chTmp;
EC_T_INT nTotLen = 0;
EC_T_INT nCurrLen = 0;
EC_T_INT nRemainSize;
EC_T_BOOL bZeroFiller = EC_FALSE;
EC_T_INT nFillLen = 0;
EC_T_INT nMinLen = 0;
EC_T_INT nVal;
EC_T_BOOL bIsNegative;
EC_T_CHAR* szDestSave;
EC_T_CHAR* szStr;
EC_T_WORD* szUniCodeStr;
EC_T_CHAR szTmp[MAX_MINLEN];
#ifdef DEBUG
EC_T_CHAR* szFormatSave = (EC_T_CHAR*)szFormat;
#endif

    nMaxSize--; // NULL terminator
    nRemainSize = nMaxSize;

    while( *szFormat && (nRemainSize>0) )
    {
        bZeroFiller = EC_FALSE;
        nMinLen = 0;
        szDestSave = EC_NULL;
        bIsNegative = EC_FALSE;
        chTmp = *szFormat++;
        switch (chTmp)
        {
        case (unsigned char)'%':
        {
            chTmp = *szFormat++;
            if( chTmp == '0' )
            {
                /* e.g. %02x */
                bZeroFiller = EC_TRUE;
                chTmp = *szFormat++;
            }
            if( (chTmp > '0') && (chTmp <= '9') )
            {
                /* e.g. %7s */
                nMinLen = (EC_T_INT)chTmp - (EC_T_INT)'0';
                chTmp = *szFormat++;
                if( (chTmp >= '0') && (chTmp <= '9') )
                {
                    /* e.g. %12s */
                    nMinLen = nMinLen * 10 + (EC_T_INT)chTmp - (EC_T_INT)'0';
                    chTmp = *szFormat++;
                }
            }
            if( nMinLen > 1 )
            {
                if( nMinLen > nRemainSize )
                {
                    nMinLen = nRemainSize;
                }
                if( nMinLen > MAX_MINLEN )
                {
                    OsDbgAssert( EC_FALSE );
                    nMinLen = MAX_MINLEN;
                }
                if( nMinLen > 1 )
                {
                    /* store formatted result in temporary buffer, save current destination */
                    szDestSave = szDest;
                    szDest = szTmp;
                }
            }
            if( chTmp == 'l' )
            {
                /* ignore %lf, %ld */
                chTmp = *szFormat++;
            }
            switch (chTmp)
            {
            case 'x':
            case 'X':
                nCurrLen = EcSprintVal(szDest, nRemainSize, EC_VAARG(vaList, EC_T_DWORD), 16,(chTmp=='x')?EC_FALSE:EC_TRUE);
                break;
            case 'd':
                nVal = EC_VAARG(vaList, EC_T_INT);
                if( nVal < 0 )
                {
                    *szDest = '-';
                    nTotLen++;
                    szDest += 1;
                    nRemainSize--;
                    nVal = -nVal;
                    bIsNegative = EC_TRUE;
                }
                if( nRemainSize > 0 )
                {
                    nCurrLen = EcSprintVal(szDest, nRemainSize, nVal, 10, EC_FALSE);
                }
                break;
            case 'u':
                nCurrLen = EcSprintVal(szDest, nRemainSize, EC_VAARG(vaList, EC_T_DWORD), 10, EC_FALSE);
                break;
            case 's':
                szStr = (EC_T_CHAR*)EC_VAARG(vaList, EC_T_CHAR*);
                nCurrLen = 0;
                while( (nRemainSize>nCurrLen) && (szStr[nCurrLen]!='\0') )
                {
                    szDest[nCurrLen] = szStr[nCurrLen]; nCurrLen++;
                }
                break;
            case 'S':
                szUniCodeStr = (EC_T_WORD*)EC_VAARG(vaList, EC_T_WORD*);
                nCurrLen = 0;
                while( (nRemainSize>nCurrLen) && (szUniCodeStr[nCurrLen]!= 0) )
                {
                    szDest[nCurrLen] = EC_LOBYTE(szUniCodeStr[nCurrLen]); nCurrLen++;
                }
                break;
            case '%':
                *szDest = '%';
                nCurrLen = 1;
                break;
            case 'c':
                *szDest = (EC_T_CHAR)EC_VAARG(vaList, EC_T_INT);
                nCurrLen = 1;
                break;
            case ' ':
                szDest[0] = '%';
                szDest[1] = ' ';
                nCurrLen = 2;
                break;
            default:
                *szDest = ' ';
                nCurrLen = 1;
#ifdef DEBUG
                printf( "EcVsnprintf: unknown format character '%c'\n", chTmp );
                printf( "             format string \"%s\" (curr at \"%s\")\n", szFormatSave, szFormat );
#endif
                OsDbgAssert( EC_FALSE );
                break;
            } // switch (chTmp)

            /* handle minimum length stuff */
            if( szDestSave != EC_NULL )
            {
                OsDbgAssert( nMinLen > 0 );
                szDest = szDestSave;
                if( nCurrLen < nMinLen )
                {
                    /* result string smaller than minimum length */
                    nFillLen = nMinLen - nCurrLen;
                    if( nFillLen > nRemainSize )
                    {
                        OsDbgAssert( EC_FALSE );
                        nFillLen = nRemainSize;
                    }
                    if( bIsNegative )
                    {
                        if( bZeroFiller )
                        {
                            /* negative number: sign must be in front in case of zero-filler */
                            *szDest++ = '-';
                        }
                        if( nFillLen > 0 )
                        {
                            nFillLen--;
                        }
                    }
                    /* fill with pattern */
                    OsMemset( szDest, bZeroFiller?'0':' ', nFillLen );
                    nTotLen += nFillLen;
                    szDest += nFillLen;
                    nRemainSize -= nFillLen;
                    if( nCurrLen > nRemainSize )
                    {
                        OsDbgAssert( EC_FALSE );
                        nCurrLen = nRemainSize;
                    }
                    if( bIsNegative )
                    {
                        /* note: nCurrLen does not include the '-' character! */
                        if( bZeroFiller )
                        {
                            /* sign already copied */
                            OsMemcpy( szDest, &szTmp[1], nCurrLen );
                        }
                        else
                        {
                            OsMemcpy( szDest, szTmp, nCurrLen+1 );
                            szDest++;   // sign is not included in nCurrLen
                        }
                    }
                    else
                    {
                        OsMemcpy( szDest, szTmp, nCurrLen );
                    }
                } // if( nCurrLen < nMinLen )
                else
                {
                    /* result string at least of minimum length size */
                    if( nCurrLen > nRemainSize )
                    {
                        OsDbgAssert( EC_FALSE );
                        nCurrLen = nRemainSize;
                    }
                    OsMemcpy( szDest, szTmp, nCurrLen );
                }
            } // if( szDestSave != EC_NULL )
            break;
        } // case (unsigned char)'%':
        case '\n':
            OsDbgAssert( nRemainSize > 0 );
            szDest[0] = '\n';   // '\r'
            nCurrLen = 1;
/*
            if( nRemainSize > 1 )
            {
                szDest[1] = '\n';
                nCurrLen = 2;
            }
*/
            break;
        default:
            OsDbgAssert( nRemainSize > 0 );
            *szDest = chTmp;
            nCurrLen = 1;
        } // switch (chTmp)
        if( nCurrLen > nRemainSize )
        {
            OsDbgAssert( EC_FALSE );
            nCurrLen = nRemainSize;
        }
        nTotLen += nCurrLen;
        szDest += nCurrLen;
        nRemainSize -= nCurrLen;
    } // while( *szFormat && (nRemainSize>0) )
    // NULL terminator 
    *szDest = '\0';

    return nTotLen;
}


/********************************************************************************/
/**
\brief formatted string printf with variable arguments
\return length of string (without terminating zero character)
*/
ATECAT_API EC_T_INT EcSnprintf
(EC_T_CHAR* szDest
,EC_T_INT nMaxSize
,const
 EC_T_CHAR* szFormat
,...
)
{
EC_T_VALIST vaList;
EC_T_INT nLen;

    EC_VASTART( vaList, szFormat );
    nLen = EcVsnprintf( szDest, nMaxSize, szFormat, vaList );
    EC_VAEND(vaList);
    return nLen;
}


/*-END OF SOURCE FILE--------------------------------------------------------*/

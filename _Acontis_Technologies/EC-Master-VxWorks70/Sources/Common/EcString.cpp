/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              String function definitions
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "EcCommon.h"
#include "EcCommonPrivate.h"
#include "EcString.h"

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
/*****************************************************************************/
/**
\brief  Get Length of String.
\return Length of String.
*/
EC_T_INT GetStringLen(
    EC_T_STRING* szString   /**< [in]   String Handle */
                     )
{
    if ((EC_NULL == szString) || (0 == szString->dwLen))
    {
        return 0;
    }
    else
    {
        return (EC_T_INT)szString->dwLen - 1;
    }
}

/***************************************************************************************************/
/**
\brief  Create String handle from C-String.

\return New Created String handle, EC_NULL on error.
*/
EC_T_STRING* CreateString(
    const EC_T_CHAR* szString,  /**< [in]   C-String containing data */
    const EC_T_DWORD dwLen,      /**< [in]   C-String length */
    struct _EC_T_MEMORY_LOGGER* pMLog
                         )
{
    EC_T_STRING* pNewString = EC_NULL;

    pNewString = (EC_T_STRING*)OsMalloc(sizeof(EC_T_STRING));
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_STRING, pMLog, "CreateString:pNewString", pNewString, sizeof(EC_T_STRING));
    if( EC_NULL != pNewString )
    {
        OsMemset(pNewString, 0, sizeof(EC_T_STRING));
        pNewString->dwLen = (dwLen+1);
        pNewString->pData = (EC_T_CHAR*)OsMalloc(sizeof(EC_T_CHAR)*(pNewString->dwLen));
        EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_STRING, pMLog, "CreateString:pData", pNewString->pData, sizeof(EC_T_CHAR)*(pNewString->dwLen));

        if( EC_NULL == pNewString->pData )
        {
            EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_STRING, pMLog, "CreateString:pNewString", pNewString, sizeof(EC_T_STRING));
            OsFree(pNewString);
            pNewString = EC_NULL;
        }
        else
        {
            OsMemset(pNewString->pData, 0, pNewString->dwLen);
            OsMemcpy(pNewString->pData, szString, dwLen);
        }
    }

    return pNewString;
}

/*****************************************************************************/
/**
\brief  Creates an uninitialized string object.

Set initial values to 0.
*/
EC_T_VOID CreateUninitializedString( EC_T_STRING& szString )
{
    OsMemset(&szString, 0, sizeof(EC_T_STRING));
}

/*****************************************************************************/
/**
\brief  Initialize string.

Preset Initial values, for static String objects.
*/
EC_T_DWORD InitString( 
    EC_T_STRING&        szString,   /**< [in]   String object reference */
    const EC_T_CHAR*    szInit,     /**< [in]   static initializer (C-String)*/
    const EC_T_DWORD    dwLen,       /**< [in]   Length of szInit */
    struct _EC_T_MEMORY_LOGGER* pMLog
                    )
{
EC_T_DWORD dwRetval = EC_E_NOERROR;

    OsMemset(&szString, 0, sizeof(EC_T_STRING));
    szString.dwLen = (dwLen+1);
    szString.pData = (EC_T_CHAR*)OsMalloc(sizeof(EC_T_CHAR)*szString.dwLen);
    EC_TRACE_ADDMEM_LOG(EC_MEMTRACE_STRING, pMLog, "InitString:szString.pData", szString.pData, sizeof(EC_T_CHAR)*szString.dwLen);

    if(EC_NULL == szString.pData )
    {
        szString.dwLen = 0;
        dwRetval = EC_E_NOMEMORY;
    }
    else
    {
        OsMemset(szString.pData, 0, szString.dwLen);
        OsMemcpy(szString.pData, szInit, dwLen);
    }
    return dwRetval;
}

/*****************************************************************************/
/**
\brief  Cleanup a given String handle.
*/
EC_T_VOID DeleteStatString(
    EC_T_STRING& szString,   /**< [in]   Handle to delete */
    struct _EC_T_MEMORY_LOGGER* pMLog
                          )
{
    if( EC_NULL != szString.pData )
    {
        EC_TRACE_SUBMEM_LOG(EC_MEMTRACE_STRING, pMLog, "DeleteStatString", szString.pData, sizeof(EC_T_CHAR)*szString.dwLen);
        OsFree(szString.pData);
    }
    
    OsMemset(&szString, 0, sizeof(EC_T_STRING));
}

/***************************************************************************************************/
/**
\brief  Return pointer to contained data portion.

\return Pointer to contained data portion, EC_NULL on error.
*/
EC_T_CHAR* GetString( const EC_T_STRING* szString )
{
    EC_T_CHAR* pContData = EC_NULL;

    if( EC_NULL != szString )
    {
        pContData = szString->pData;
    }

    return pContData;
}


/*****************************************************************************/
/**
\brief  Check String equality.

\return EC_TRUE if Strings to match, EC_FALSE otherwise.
*/
EC_T_BOOL IsEqualString( 
    const EC_T_STRING*  szString,   /**< [in]   String A */
    const EC_T_CHAR*    szComp,     /**< [in]   String B */
    const EC_T_DWORD    dwLen       /**< [in]   Length of szComp */
                       )
{
    EC_T_BOOL bMatch = EC_FALSE;

    if( EC_NULL == szString && EC_NULL == szComp )
    {
        bMatch = EC_TRUE;
    }
    else if ( EC_NULL == szString && EC_NULL != szComp )
    {
        bMatch = EC_FALSE;
    }
    else if( EC_NULL != szString && EC_NULL == szComp )
    {
        bMatch = EC_FALSE;
    }
    else if( EC_NULL != szString && EC_NULL != szComp )
    {
        bMatch  = ((szString->dwLen-1) == dwLen);
        bMatch &= (0 == OsMemcmp(szString->pData, szComp, EC_MIN(dwLen, (szString->dwLen-1))));
    }
    else
    {
        bMatch = EC_FALSE;
    }

    return bMatch;
}

/*-END OF SOURCE FILE--------------------------------------------------------*/

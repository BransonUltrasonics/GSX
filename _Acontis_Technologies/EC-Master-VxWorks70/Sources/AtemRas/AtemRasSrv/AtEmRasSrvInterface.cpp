/*-----------------------------------------------------------------------------
 * AtEmRasSrvInterface.cpp  file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Willig, Andreas
 * Description              description of file
 * Date                     2007/5/4::7:30
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/

#include "CAtemRasServer.h"
#include <AtEmRasSrv.h>
#include <AtEmRasSrvVersion.h>

/*-DEFINES-------------------------------------------------------------------*/

/*-TYPEDEFS------------------------------------------------------------------*/

/*-CLASS---------------------------------------------------------------------*/

/*-FUNCTION DECLARATION------------------------------------------------------*/

/*-LOCAL VARIABLES-----------------------------------------------------------*/

/*-GLOBAL VARIABLES-----------------------------------------------------------*/

EC_T_DWORD G_dwVerbosity = 0;

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/

/***************************************************************************************************/
/**
\brief  Version of AtEmRas Server Software.

\return Version Number as DWORD.
*/
ATECAT_API EC_T_DWORD emRasSrvGetVersion( EC_T_VOID )
{
    return ATEMRASSRV_VERSION;
}

/***************************************************************************************************/
/**
\brief  Create RAS Server Instance.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emRasSrvStart( 
    ATEMRAS_T_SRVPARMS  oParms,         /**< [in]   Server start-up parameters */
    EC_T_PVOID*         ppHandle        /**< [out]  Handle to opened instance, used for ctrl access */
                         )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_DWORD      dwRes       = EC_E_ERROR;
    CAtemRasServer* pServer     = EC_NULL;

    if( (EC_NULL == ppHandle) )
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pServer = EC_NEW(CAtemRasServer(oParms));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "emRasSrvStart pServer", pServer, sizeof(CAtemRasServer));
    if( EC_NULL == pServer )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }

    dwRes = pServer->StartServer();
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }

    (*ppHandle) = (EC_T_PVOID)pServer;

    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
} 

/***************************************************************************************************/
/**
\brief  Destroy RAS Server Instance.

If this function returns !EC_E_NOERROR, the Server is shut and deleted anyway, so application needs to call 
emRasSrvStart agn to get a valid RAS Server handle.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD  emRasSrvStop( 
    EC_T_PVOID pvHandle,        /**< [in]   Handle to previously started Server */
    EC_T_DWORD dwTimeout        /**< [in]   Timeout to wait for thread shutdown, real shutdown timeout is
                                  *         this value multiplied with open threads spawned.
                                  */
                        )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    EC_T_DWORD      dwRes       = EC_E_ERROR;
    CAtemRasServer* pServer     = (CAtemRasServer*)pvHandle;

    if( EC_NULL == pvHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRes = pServer->StopServer(dwTimeout);
    if( EC_E_NOERROR != dwRes )
    {
        dwRetVal = dwRes;
        goto Exit;
    }
    
    dwRetVal = EC_E_NOERROR;
Exit:
    /* don't care about errors. if this call is issued, the server instance shall be shut down afterwards,
     * anyhow 
     */
    EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "emRasSrvStop", pServer, sizeof(CAtemRasServer));
    SafeDelete(pServer);

    return dwRetVal;
}

#if (defined INCLUDE_RAS_TRACESUPPORT)
/***************************************************************************************************/
/**
\brief  Enable RAS Server RAW Trace to file.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emRasSrvTraceEnable( 
    EC_T_BOOL   bEnable     /**< [in]   EC_TRUE enable trace, EC_FALSE disable trace */
                              )
{
    EC_T_DWORD dwRetVal = EC_E_ERROR;

    dwRetVal = CEcSocket::TraceEnable(bEnable);

    return dwRetVal;
}

#else
ATECAT_API EC_T_DWORD emRasSrvTraceEnable( 
    EC_T_BOOL   bEnable     /**< [in]   EC_TRUE enable trace, EC_FALSE disable trace */
                              )
{
    EC_UNREFPARM(bEnable);
    
    return EC_E_NOTSUPPORTED;
}
#endif

#if (defined INCLUDE_RAS_SPOCSUPPORT)
/***************************************************************************************************/
/**
\brief  Modify Access level.

Function need to lock down different function calls to realize SPOC.
\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emRasSrvModifyAccessLevel(
    EC_T_PVOID  pvHandle            /**< [in]   Handle to previously started Server */
   ,EC_T_DWORD  dwAccessClass       /**< [in]   Call class (0 = wild card) */
   ,EC_T_DWORD  dwAccessLevel       /**< [in]   New Access level */
                                    )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    CAtemRasServer* pServer     = (CAtemRasServer*)pvHandle;
    
    if( EC_NULL == pvHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }
    
    dwRetVal = pServer->SetAccessLevel(dwAccessClass, dwAccessLevel);
Exit:
    return dwRetVal;
}

/***************************************************************************************************/
/**
\brief  Load Access control configuration List.

\return EC_E_NOERROR on success, error code otherwise.
*/
EC_T_DWORD emRasSrvConfigAccessLevel(
    EC_T_PVOID  pvHandle        /**< [in]   Handle to previously started Server */
   ,EC_T_BYTE*  pbyData         /**< [in]   Configuration data */
   ,EC_T_DWORD  dwLen           /**< [in]   Length of Configuration data */
                                    )
{
    EC_T_DWORD      dwRetVal    = EC_E_ERROR;
    CAtemRasServer* pServer     = (CAtemRasServer*)pvHandle;
    
    if( EC_NULL == pvHandle)
    {
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    dwRetVal = pServer->ConfigureAccessControl(pbyData, dwLen);
Exit:
    return dwRetVal;
}

#else

/***************************************************************************************************/
/**
\brief  Modify Access level.

Function need to lock down different function calls to realize SPOC.
\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emRasSrvModifyAccessLevel(
    EC_T_PVOID  pvHandle            /**< [in]   Handle to previously started Server */
   ,EC_T_DWORD  dwAccessClass       /**< [in]   Call class (0 = wildcard) */
   ,EC_T_DWORD  dwAccessLevel       /**< [in]   New Access level */
                                    )
{
    EC_UNREFPARM(pvHandle);
    EC_UNREFPARM(dwAccessClass);
    EC_UNREFPARM(dwAccessLevel);

    return EC_E_NOTSUPPORTED;
}

/***************************************************************************************************/
/**
\brief  Load Access control configuration List.

\return EC_E_NOERROR on success, error code otherwise.
*/
ATECAT_API EC_T_DWORD emRasSrvConfigAccessLevel(
    EC_T_PVOID  pvHandle        /**< [in]   Handle to previously started Server */
   ,EC_T_BYTE*  pbyData         /**< [in]   Configuration data */
   ,EC_T_DWORD  dwLen           /**< [in]   Length of Configuration data */
                                    )
{
    EC_UNREFPARM(pvHandle);
    EC_UNREFPARM(pbyData);
    EC_UNREFPARM(dwLen);
    return EC_E_NOTSUPPORTED;
}

#endif

/*-END OF SOURCE FILE--------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Paul Bussmann
 * Description              RAS Client connection acceptor thread
 *---------------------------------------------------------------------------*/

/*-INCLUDES------------------------------------------------------------------*/
#include "CAtemRasServer.h"

/*-DEFINES-------------------------------------------------------------------*/

/* The mixing step */
#define mix(a,b,c) \
{ \
    a=a-b;  a=a-c;  a=a^(c>>13); \
    b=b-c;  b=b-a;  b=b^(a<<8);  \
    c=c-a;  c=c-b;  c=c^(b>>13); \
    a=a-b;  a=a-c;  a=a^(c>>12); \
    b=b-c;  b=b-a;  b=b^(a<<16); \
    c=c-a;  c=c-b;  c=c^(b>>5);  \
    a=a-b;  a=a-c;  a=a^(c>>3);  \
    b=b-c;  b=b-a;  b=b^(a<<10); \
    c=c-a;  c=c-b;  c=c^(b>>15); \
}

/*-FUNCTION-DEFINITIONS------------------------------------------------------*/
/*****************************************************************************/
/**
\brief  Cookie Hash Function.

\return 32-Bit Hash Value.
*/
static EC_T_DWORD CookieHash( 
    EC_T_PBYTE k,           /**< [in]   the key */
    EC_T_DWORD dwLength,    /**< [in]   the length of the key in bytes */
    EC_T_DWORD initval      /**< [in]   the previous hash, or an arbitrary value */
                            )
{
    EC_T_DWORD  a = 0,
                b = 0,
                c = 0;  /* the internal state */
    EC_T_DWORD  len;    /* how many key bytes still need mixing */
    
    /* Set up the internal state */
    len = dwLength;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* variable initialization of internal state */
    
    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a=a+(k[0]+((EC_T_DWORD)k[1]<<8)+((EC_T_DWORD)k[2]<<16) +((EC_T_DWORD)k[3]<<24));
        b=b+(k[4]+((EC_T_DWORD)k[5]<<8)+((EC_T_DWORD)k[6]<<16) +((EC_T_DWORD)k[7]<<24));
        c=c+(k[8]+((EC_T_DWORD)k[9]<<8)+((EC_T_DWORD)k[10]<<16)+((EC_T_DWORD)k[11]<<24));
        mix(a,b,c);
        k = k+12; len = len-12;
    }
    
    /*------------------------------------- handle the last 11 bytes */
    c = c+dwLength;
    switch(len)              /* all the case statements fall through */
    {
    case 11: c=c+((EC_T_DWORD)k[10]<<24);
    case 10: c=c+((EC_T_DWORD)k[9]<<16);
    case 9 : c=c+((EC_T_DWORD)k[8]<<8);
        /* the first byte of c is reserved for the length */
    case 8 : b=b+((EC_T_DWORD)k[7]<<24);
    case 7 : b=b+((EC_T_DWORD)k[6]<<16);
    case 6 : b=b+((EC_T_DWORD)k[5]<<8);
    case 5 : b=b+k[4];
    case 4 : a=a+((EC_T_DWORD)k[3]<<24);
    case 3 : a=a+((EC_T_DWORD)k[2]<<16);
    case 2 : a=a+((EC_T_DWORD)k[1]<<8);
    case 1 : a=a+k[0];
        /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */

    return c;
}

EC_T_VOID CAtemRasServer::AcceptorThreadStepWrapper(EC_T_PVOID pvParams)
{
    CAtemRasServer* pServer = (CAtemRasServer*)pvParams;
    pServer->AcceptorThreadStep();
}

/*****************************************************************************/
/**
\brief  RAS Client connection acceptor thread

    Waits for incoming connection requests and dispatches them accordingly. 
    Its job is an acceptor and logon purpose. Any other data transfer is done 
    by the corresponding client threads started after client logged on.
*/
EC_T_VOID   CAtemRasServer::AcceptorThreadStep(EC_T_VOID)
{
CEcSocket*           pSpawnSocket            = EC_NULL;
EC_T_BYTE            abyCookieString[128];
EC_T_DWORD           dwCookieHash            = 0;
EC_T_DWORD           dwRes;
CAtemRasSrvClient*   pNewClient              = EC_NULL;

    /* remove terminated clients from list */
    GarbageCollect();

    /* new socket created, pass this socket to a logon handler, which is re-used as client 
     * thread on a "really" new connection, and is closed agn if cookie tells about a 
     * re-connect */
    /* wait on master port for an incoming connection */
    dwRes = m_pServerAcceptorSocket->Accept(&pSpawnSocket, RAS_CMD_RECV_TIMEOUT);
    switch (dwRes)
    {
    case EC_E_TIMEOUT:
        return;
    case EC_E_NOERROR:
        break;
    default:
        /* prevent endless-loop */
        OsSleep(500);
        return;
    }

    OsMemset(abyCookieString, 0, sizeof(abyCookieString));
    EC_SET_FRM_DWORD(&abyCookieString[0], m_pServerAcceptorSocket->GetAddr());
    EC_SET_FRM_WORD(&abyCookieString[4], m_pServerAcceptorSocket->GetPort());
    EC_SET_FRM_DWORD(&abyCookieString[6], pSpawnSocket->GetAddr());
    EC_SET_FRM_WORD(&abyCookieString[10], pSpawnSocket->GetPort());
    dwCookieHash = CookieHash(abyCookieString, sizeof(abyCookieString), 0);

    pNewClient = EC_NEW(CAtemRasSrvClient(this, pSpawnSocket, dwCookieHash, 
        m_oConfig.dwWDTOLimit, m_oConfig.dwCycleTime,
        m_oConfig.dwClientPrio, m_dwClientStackSize));
    EC_TRACE_ADDMEM(EC_MEMTRACE_RAS, "CAtemRasServer::AcceptorThreadStep", pNewClient, sizeof(CAtemRasSrvClient));
    if (EC_NULL == pNewClient)
    {
        VERBOSEOUT(1, ("CAtemRasServer::AcceptorThreadStep(): Cannot spawn new client instance, out of Memory\n"));
        return;
    }
    dwRes = pNewClient->InitClient();
    if (EC_E_NOERROR != dwRes)
    {
        VERBOSEOUT(1, ("CAtemRasServer::AcceptorThreadStep(): InitClient() failed with %s (0x%lx)!\n", ecatGetText(dwRes), dwRes));

        EC_TRACE_SUBMEM(EC_MEMTRACE_RAS, "CAtemRasServer::AcceptorThreadStep", pNewClient, sizeof(CAtemRasSrvClient));
        SafeDelete(pNewClient);
        return;
    }

    /* enqueue client to list */
    AppendClientEntry(pNewClient);
}

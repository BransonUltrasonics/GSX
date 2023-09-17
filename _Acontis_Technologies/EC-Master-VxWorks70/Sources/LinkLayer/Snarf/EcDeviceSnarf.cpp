/*-----------------------------------------------------------------------------
 * EcDeviceSnarf.cpp        implementation file
 * Copyright                acontis technologies GmbH, Weingarten, Germany
 * Response                 Stefan Zintgraf
 * Description              
 *---------------------------------------------------------------------------*/


/*-INCLUDES------------------------------------------------------------------*/
#include "EcCommon.h"

#include "EthernetServices.h"
#include "EcLink.h"

#include <netinet/in.h>
#if (defined VXWORKS55)
#include <sys/times.h>
#define class u_class
#endif

/* avoid warnings caused from the VxWorks eventLibCommon.h header file */ 
#include <muxTkLib.h>
#include "logLib.h"
#include <errno.h>

#if (defined VXWORKS55)
#undef class
#endif

#if (defined VXWORKS55)
#include "snarfmacros.h"
#endif

/*-DEFINES------------------------------------------------------------------*/

#define MAX_FRAME_SIZE 1516

#define FRAMEDESC_BUFFER_SIZE   20  /* store maximum of 20 frame descriptors for polling */

#define SNARF_MODE_INTERRUPT      1   /* SNARF running in interrupt mode */
#define SNARF_MODE_POLLING        2   /* SNARF emulating polling mode */
#define SNARF_MODE_MUX_POLLING    3   /* SNARF using MUX in polling mode (note: do not use!!) */


/*#define WV_EVENTS*/
#ifdef WV_EVENTS
#include "wvLib.h"
#define WVEVENT(num,buf,len) wvEvent((num),(buf),(len))
#else
#define WVEVENT(num,buf,len)
#endif

#define WVTRACE(offset) \
    WVEVENT((0+offset), NULL, 0)

/*-TYPEDEFS------------------------------------------------------------------*/

typedef struct
{
    EC_T_BOOL       bInUse;
    EC_T_CHAR       szDeviceName[16];
    char            szIfName[16];
    int             nIfNr;
    END_OBJ*        pEnd;
    void*           pvMuxCookie;
    EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback;
    EC_T_VOID*                pvCallbackContext;
    EC_T_LINK_FRAMEDESC         aoFrameDesc[FRAMEDESC_BUFFER_SIZE]; /* frame descriptor buffer for polling mode */
    EC_T_DWORD                  dwFrameDescTailIndex;               /* frame desc buffer tail index (last full) */
    EC_T_DWORD                  dwFrameDescHeadIndex;               /* frame desc buffer head index (next empty) */
    M_BLK_ID                    pTxMblk;                            /* tx mblk buffer when MUX is running in polling mode */
    M_BLK_ID                    pRxMblk;                            /* rx mblk buffer when MUX is running in polling mode */
} EC_SNARF_LINK_DESC;


/*-GLOBALS--------------------------------------------------------------------*/
#define SNARF_POLLING_MODE 
#if defined MUX_POLLING_MODE
int G_nEmSnarfMode = SNARF_MODE_MUX_POLLING;
#elif defined SNARF_POLLING_MODE
int G_nEmSnarfMode = SNARF_MODE_POLLING;
#else
int G_nEmSnarfMode = SNARF_MODE_INTERRUPT;
#endif
int G_nReuseMblk = EC_FALSE;	/* note: EC_TRUE does not work!!! */
int G_nDumpTxFrm = EC_FALSE;
int G_nDumpRxFrm = EC_FALSE;

/*-FORWARD DECLARATIONS-------------------------------------------------------*/
static EC_T_DWORD      EcLinkOpen(EC_T_VOID* pvLinkParms, EC_T_RECEIVEFRAMECALLBACK pfReceiveFrameCallback, EC_T_LINK_NOTIFY pfLinkNotifyCallback, EC_T_VOID* pvContext, EC_T_VOID** ppvInstance );
static EC_T_DWORD      EcLinkClose(EC_T_VOID* pvInstance);
static EC_T_DWORD      EcLinkSendFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
static EC_T_DWORD      EcLinkSendAndFreeFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
static EC_T_DWORD      EcLinkRecvFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
static EC_T_DWORD      EcLinkAllocSendFrame(EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_DWORD dwSize);
static EC_T_VOID       EcLinkFreeSendFrame (EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
static EC_T_VOID       EcLinkFreeRecvFrame (EC_T_VOID* pvInstance, EC_T_LINK_FRAMEDESC* pLinkFrameDesc);
static EC_T_DWORD      EcLinkGetEthernetAddress(EC_T_VOID* pvInstance, EC_T_BYTE* pbyEthernetAddress);
static EC_T_LINKSTATUS EcLinkGetStatus(EC_T_VOID* pvInstance);
static EC_T_DWORD      EcLinkGetSpeed(EC_T_VOID* pvInstance);
static EC_T_LINKMODE   EcLinkGetMode(EC_T_VOID* pvInstance);
static EC_T_DWORD      EcLinkIoctl(EC_T_VOID*   pvInstance, EC_T_DWORD dwCode, EC_T_LINK_IOCTLPARMS* pParms );

/* instance table */
typedef EC_T_BOOL (*EC_PF_LINKOSDBGMSGHK)(const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs);
static EC_PF_LINKOSDBGMSGHK S_pfLinkOsDbgMsgHook    = EC_NULL;

/********************************************************************************/
/** \brief Print a debugging message
*
* \return N/A
*/
static EC_T_VOID LinkOsDbgMsg(const EC_T_CHAR* szFormat, ...)
{
    EC_T_BOOL bPrintMsg = EC_FALSE;
    va_list vaArgs;
    
    va_start(vaArgs, szFormat);
    if( S_pfLinkOsDbgMsgHook != EC_NULL )
    {
        bPrintMsg = (*S_pfLinkOsDbgMsgHook)(szFormat, vaArgs);
    }
    if( bPrintMsg )
    {
        vprintf(szFormat, vaArgs);
    }
    va_end(vaArgs);
}

#ifdef ASSERT_SUSPEND
/********************************************************************************/
/** \brief 
*
* \return
*/
EC_T_VOID OsDbgAssertFunc(EC_T_BOOL bAssertCondition, EC_T_CHAR* szFile, EC_T_DWORD dwLine)
{
    if( !bAssertCondition )
    {
        logMsg( "ASSERTION in file %s, line %d\n", (int)szFile, (int)dwLine, 3,4,5,6 );
        taskSuspend(0);
    }
}
#endif

/********************************************************************************/
/** \brief Add OS Layer Debug Message hook
*
* \return N/A
*/
static EC_T_VOID LinkOsAddDbgMsgHook(EC_PF_LINKOSDBGMSGHK pfOsDbgMsgHook)
{
    S_pfLinkOsDbgMsgHook = pfOsDbgMsgHook;
}

/********************************************************************************/
/** \brief Tracing of a received frame.
*
* \return N/A.
*/
static void emllSnarfDumpFrame
(   EC_T_BOOL bTxFrame,	/* [in]  EC_TRUE if it is a tx frame */
	UINT8*  pbyFrame,   /* [in]  ethernet frame */
    UINT32  dwSize      /* [in]  frame size */
)
{
UINT32 dwCnt;
char* szPrefix;
static UINT32 s_dwTxFrameCnt = 0;
static UINT32 s_dwRxFrameCnt = 0;
UINT32 dwFrameCnt;

	if( bTxFrame )
	{
		s_dwTxFrameCnt++;
		szPrefix="SND";
		dwFrameCnt=s_dwTxFrameCnt;
	}
	else
	{
		s_dwRxFrameCnt++;
		szPrefix="RCV";
		dwFrameCnt=s_dwRxFrameCnt;
	}
	
	printf( "------------------------------------------------------\n" );
	//OsTrcMsg( "%d - RCV[%d]:", OsQueryMsecCount(), dwSize );
	printf( "FRAME-%s[%d](%d):", szPrefix, dwSize, dwFrameCnt );
	for( dwCnt = 0; dwCnt < dwSize; dwCnt++ )
	{
		if( (dwCnt % 16) == 0 )
		{
			printf( "\n[%03X]  ", dwCnt );
		}
		printf( "%02X ", pbyFrame[dwCnt] );
	}
	printf( "\n" );
}

/*-LOCAL VARIABLES-----------------------------------------------------------*/

static EC_T_BOOL muxReceiveFrame(void *, long type, M_BLK_ID pMblk, void *);
#ifdef DEBUG
int nSnarfPollRxFrm = 0;
int nSnarfPollRxFrmEcat = 0;
#endif

/*-IEcDevice-----------------------------------------------------------------*/

extern "C"
{
/********************************************************************************/
/** \brief Register link layer driver.
*
* \return EC_E_NOERROR or error code.
*/
EC_T_DWORD emllRegisterSnarf
(EC_T_LINK_DRV_DESC*  pLinkDrvDesc        /* [in,out] link layer driver descriptor */
,EC_T_DWORD         dwLinkDrvDescSize   /* [in]     size in bytes of link layer driver descriptor */
)
{
EC_T_DWORD dwResult = EC_E_NOERROR;
EC_T_BOOL bInterfaceSupported = EC_FALSE;

    if( pLinkDrvDesc->dwValidationPattern != LINK_LAYER_DRV_DESC_PATTERN )
    {

        LinkOsDbgMsg( "emllRegister: invalid drive descriptor pattern 0x%x instead of 0x%x\n", 
                  pLinkDrvDesc->dwValidationPattern, LINK_LAYER_DRV_DESC_PATTERN );
        dwResult = EC_E_INVALIDPARM;
        goto Exit;
    }

    /* Check the size of the given link layer driver descriptor */
    if( dwLinkDrvDescSize > sizeof(EC_T_LINK_DRV_DESC) )
    {
        LinkOsDbgMsg( "emllRegister: link layer driver descriptor size too large\n" );
        dwResult = EC_E_INVALIDPARM;
        goto Exit;
    }

    bInterfaceSupported = EC_FALSE;

    /* Check if the version of the interface is supported  */
    if(sizeof(EC_T_LINK_DRV_DESC) == dwLinkDrvDescSize  && pLinkDrvDesc->dwInterfaceVersion == LINK_LAYER_DRV_DESC_VERSION)
    {
        bInterfaceSupported = EC_TRUE;
    }

    /* Interface is not supported. */
    if(!bInterfaceSupported)
    {
        LinkOsDbgMsg( "emllRegister: invalid drive descriptor interface version 0x%x instead of 0x%x\n", 
            pLinkDrvDesc->dwInterfaceVersion, LINK_LAYER_DRV_DESC_VERSION );
        dwResult = EC_E_INVALIDPARM;
        goto Exit;
    }

    pLinkDrvDesc->pfEcLinkOpen = EcLinkOpen;
    pLinkDrvDesc->pfEcLinkClose = EcLinkClose;
    pLinkDrvDesc->pfEcLinkSendFrame = EcLinkSendFrame;
    pLinkDrvDesc->pfEcLinkSendAndFreeFrame = EcLinkSendAndFreeFrame;
    pLinkDrvDesc->pfEcLinkRecvFrame = EcLinkRecvFrame;
    pLinkDrvDesc->pfEcLinkAllocSendFrame = EcLinkAllocSendFrame;
    pLinkDrvDesc->pfEcLinkFreeSendFrame  = EcLinkFreeSendFrame ;
    pLinkDrvDesc->pfEcLinkFreeRecvFrame  = EcLinkFreeRecvFrame ;
    pLinkDrvDesc->pfEcLinkGetEthernetAddress = EcLinkGetEthernetAddress;
    pLinkDrvDesc->pfEcLinkGetStatus = EcLinkGetStatus;
    pLinkDrvDesc->pfEcLinkGetSpeed = EcLinkGetSpeed;
    pLinkDrvDesc->pfEcLinkGetMode = EcLinkGetMode;
    pLinkDrvDesc->pfEcLinkIoctl = EcLinkIoctl;

    /* store DBG Message hook */
    LinkOsAddDbgMsgHook(pLinkDrvDesc->pfOsDbgMsgHook);

Exit:
    return dwResult;
}

}


/********************************************************************************/
/** \brief allocate mblk buffer.
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD muxAllocMblk( EC_SNARF_LINK_DESC* pSnarfLinkDesc, M_BLK_ID* ppMblk, EC_T_DWORD dwSize )
{
EC_T_DWORD dwResult = EC_E_NOERROR;
EC_T_DWORD dwEffSize;
M_BLK_ID pMblk;

    WVTRACE(0);
    *ppMblk = EC_NULL;
    pMblk = netTupleGet( pSnarfLinkDesc->pEnd->pNetPool, dwSize, M_DONTWAIT, MT_DATA, TRUE );
    if( pMblk == EC_NULL)
    {
        WVTRACE(1);
        dwResult = EC_E_NOMEMORY;
        goto Exit;
    }
    dwEffSize = dwSize;
#ifdef VXWORKS55
    dwEffSize -= 14;
#endif
    /* pre-fill frame */
    pMblk->mBlkHdr.mFlags |= M_PKTHDR;
    pMblk->mBlkHdr.mLen    = dwEffSize;
    pMblk->mBlkPktHdr.len  = dwEffSize;
    pMblk->mBlkHdr.mData   = pMblk->pClBlk->clNode.pClBuf;
    *ppMblk = pMblk;
    
Exit:
    return dwResult;
}


/********************************************************************************/
/** \brief free mblk buffer.
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_VOID muxFreeMblk( M_BLK_ID pMblk )
{
    WVTRACE(5);
    if( pMblk != EC_NULL )
    {
        WVTRACE(6);
#if (defined VXWORKS55)
        errno = 0;
        netMblkClChainFree( pMblk );
        if( errno == S_netBufLib_MBLK_INVALID )
        {
            WVTRACE(7);
            LinkOsDbgMsg( "muxFreeMblk: invalid mblk 0x%x\n", (unsigned int)pMblk );
#ifdef DEBUG_OFF            
            taskSuspend(0);
#endif            
        }
#else
        netTupleFree( pMblk );
#endif
    }
    
    return;
}

/********************************************************************************/
/** \brief Open Link Layer connection.
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkOpen
(   EC_T_VOID*                  pLinkParms,             /* [in]  link parameters */
    EC_T_RECEIVEFRAMECALLBACK   pfReceiveFrameCallback, /* [in]  pointer to rx callback function */
    EC_T_LINK_NOTIFY            pfLinkNotifyCallback,   /* [in]  pointer to notification callback function */
    EC_T_VOID*                  pvContext,              /* [in]  caller context, to be used in callback functions */
    EC_T_VOID**                 ppvInstance             /* [out] instance handle */
)
{
	EC_T_DWORD dwRetVal = EC_E_ERROR;
	EC_T_LINK_PARMS_SNARF* pLinkParmsAdapter = (EC_T_LINK_PARMS_SNARF*)pLinkParms;
	EC_SNARF_LINK_DESC* pSnarfLinkDesc 	  = EC_NULL;
    STATUS Status;

    /* check parameters */
    if (pfReceiveFrameCallback == EC_NULL)
    {
        goto Exit;
    }
    if( EC_NULL == pLinkParmsAdapter )
    {
    	LinkOsDbgMsg(("Snarf-EcLinkOpen(): Missing Configuration for Snarf Link Layer\n"));
        *ppvInstance = EC_NULL;
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    if( (pLinkParmsAdapter->linkParms.dwSignature) != EC_LINK_PARMS_SIGNATURE_SNARF )
    {
    	LinkOsDbgMsg(("Snarf-EcLinkOpen(): Invalid Configuration for Snarf Link Layer\n"));
        *ppvInstance = EC_NULL;
        dwRetVal = EC_E_INVALIDPARM;
        goto Exit;
    }

    pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)OsMalloc(sizeof(EC_SNARF_LINK_DESC));
    if( pSnarfLinkDesc == EC_NULL )
    {
        dwRetVal = EC_E_NOMEMORY;
        goto Exit;
    }
    *ppvInstance = (EC_T_VOID*)pSnarfLinkDesc;
    OsMemset( pSnarfLinkDesc, 0, sizeof(EC_SNARF_LINK_DESC) );

    pSnarfLinkDesc->pfReceiveFrameCallback = pfReceiveFrameCallback;
    pSnarfLinkDesc->pvCallbackContext = pvContext;

    /* get network adapter */
    OsStrcpy(pSnarfLinkDesc->szDeviceName, (char*)pLinkParmsAdapter->szAdapterName);
    OsStrcpy(pSnarfLinkDesc->szIfName, pSnarfLinkDesc->szDeviceName );
    pSnarfLinkDesc->szIfName[strlen(pSnarfLinkDesc->szDeviceName)-1] = '\0';
    pSnarfLinkDesc->nIfNr = pSnarfLinkDesc->szDeviceName[strlen(pSnarfLinkDesc->szDeviceName)-1] - '0';

    /* get END object */
    pSnarfLinkDesc->pEnd = endFindByName( pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr );
    if (pSnarfLinkDesc->pEnd == EC_NULL)
    {
        LinkOsDbgMsg( "EcLinkOpen(): endFindByName could not find device %s%d\n", pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr );
        goto Exit;
    }
    /* install SNARF protocol */
    if( EC_TRACE_LINK )
        LinkOsDbgMsg("Install SNARF protocol on <%s>, unit %d\n", pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr);
#ifdef VXWORKS55
    pSnarfLinkDesc->pvMuxCookie = muxTkBind (pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr, (BOOL (*) (void *, long int, M_BLK *, void *))muxReceiveFrame, EC_NULL, EC_NULL, EC_NULL, MUX_PROTO_SNARF, "Ecat Master", (void*)pSnarfLinkDesc, EC_NULL, EC_NULL);
#else
    pSnarfLinkDesc->pvMuxCookie = muxTkBind (pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr, (FUNCPTR)muxReceiveFrame, EC_NULL, EC_NULL, EC_NULL, MUX_PROTO_SNARF, "Ecat Master", (void*)pSnarfLinkDesc, EC_NULL, EC_NULL);
#endif
    if( pSnarfLinkDesc->pvMuxCookie == EC_NULL )
    {
        LinkOsDbgMsg( "EcLinkOpen(): muxTkBind could not bind to device %s%d\n", pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr );
        goto Exit;
    }
    if( G_nEmSnarfMode == SNARF_MODE_POLLING ) 
    {
        OsMemset( pSnarfLinkDesc->aoFrameDesc, 0, sizeof(pSnarfLinkDesc->aoFrameDesc) );
        pSnarfLinkDesc->dwFrameDescTailIndex = 0;
        pSnarfLinkDesc->dwFrameDescHeadIndex = 0;
    }

    if( (G_nEmSnarfMode == SNARF_MODE_MUX_POLLING) && G_nReuseMblk ) 
    {
        LinkOsDbgMsg( "EcLinkOpen(): maximum frame size supported = %d\n", MAX_FRAME_SIZE );
        
        /* pre-allocate tx/rx frame buffer */
        dwRetVal = muxAllocMblk( pSnarfLinkDesc, &pSnarfLinkDesc->pTxMblk, MAX_FRAME_SIZE );
        if( dwRetVal != EC_E_NOERROR )
        {
            LinkOsDbgMsg( "EcLinkOpen(): muxAllocMblk Tx failed\n");
            goto Exit;
        }
        dwRetVal = muxAllocMblk( pSnarfLinkDesc, &pSnarfLinkDesc->pRxMblk, MAX_FRAME_SIZE );
        if( dwRetVal != EC_E_NOERROR )
        {
            LinkOsDbgMsg( "EcLinkOpen(): muxAllocMblk Rx failed\n");
            goto Exit;
        }
    } /* if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) */
    else
    {
        pSnarfLinkDesc->pTxMblk = EC_NULL;
        pSnarfLinkDesc->pRxMblk = EC_NULL;
    }
    
    if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {
		/* set driver into polling mode */	
		Status = muxIoctl( pSnarfLinkDesc->pvMuxCookie, EIOCPOLLSTART, NULL );
		if( Status != OK )
		{
			LinkOsDbgMsg( "EcLinkOpen(): muxIoctl(EIOCPOLLSTART) for device %s%d failed with error 0x%x\n", pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr, errno );
			goto Exit;
		}
    }
    
    /* no errors */
    dwRetVal = EC_E_NOERROR; 

Exit:
    if( dwRetVal != EC_E_NOERROR )
    {
        if( pSnarfLinkDesc != EC_NULL )
        {
            if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
            {
                if( pSnarfLinkDesc->pTxMblk != EC_NULL )
                {
                    muxFreeMblk( pSnarfLinkDesc->pTxMblk );
                }
                if( pSnarfLinkDesc->pRxMblk != EC_NULL )
                {
                    muxFreeMblk( pSnarfLinkDesc->pRxMblk );
                }
            }
            OsFree( pSnarfLinkDesc );
        }
    }
    return dwRetVal;
}   


/********************************************************************************/
/** \brief Close Link Layer connection.
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkClose(EC_T_VOID* pvInstance)
{
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;

    if( pSnarfLinkDesc == EC_NULL )
    {
        goto Exit;
    }
    if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {
        STATUS Status;
        if( pSnarfLinkDesc->pTxMblk != EC_NULL )
        {
            muxFreeMblk( pSnarfLinkDesc->pTxMblk );
        }
        if( pSnarfLinkDesc->pRxMblk != EC_NULL )
        {
            muxFreeMblk( pSnarfLinkDesc->pRxMblk );
        }
    
        /* set driver into polling mode */	
        Status = muxIoctl( pSnarfLinkDesc->pvMuxCookie, EIOCPOLLSTOP, NULL );
        if( Status != OK )
        {
            LinkOsDbgMsg( "EcLinkClose(): muxIoctl(EIOCPOLLSTART) for device %s%d failed with error 0x%x\n", pSnarfLinkDesc->szIfName, pSnarfLinkDesc->nIfNr, errno );
        }
    }

    /* uninstall SNARF protocol */
    if( pSnarfLinkDesc->pvMuxCookie != EC_NULL )
    {
        muxUnbind(pSnarfLinkDesc->pvMuxCookie, MUX_PROTO_SNARF, (FUNCPTR)muxReceiveFrame);
        pSnarfLinkDesc->pvMuxCookie = EC_NULL;
    }
    /* cleanup */
    pSnarfLinkDesc->szDeviceName[0] = 0;
    pSnarfLinkDesc->szIfName[0]     = 0;
    pSnarfLinkDesc->nIfNr           = 0;

    pSnarfLinkDesc->pfReceiveFrameCallback = EC_NULL;
    pSnarfLinkDesc->pvCallbackContext      = EC_NULL;
    OsFree( pSnarfLinkDesc );
    
Exit:
    return EC_E_NOERROR;
}


/********************************************************************************/
/** \brief Determine link speed.
*
* \return link speed in MBit/sec
*/
static EC_T_DWORD EcLinkGetSpeed(EC_T_VOID* pvInstance)
{
EC_UNREFPARM(pvInstance);
    return 100;
}


/********************************************************************************/
/** \brief Determine link mode
*
* \return link mode
*/
static EC_T_LINKMODE EcLinkGetMode(EC_T_VOID* pvInstance)
{
EC_UNREFPARM(pvInstance);
    if( G_nEmSnarfMode == SNARF_MODE_POLLING ) 
    {
        return EcLinkMode_POLLING;
    } 
    if( G_nEmSnarfMode == SNARF_MODE_INTERRUPT ) 
    {
        return EcLinkMode_INTERRUPT;
    } 
    if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {
        return EcLinkMode_POLLING;
    } 
    OsDbgAssert(FALSE);
    return EcLinkMode_POLLING;
}


/********************************************************************************/
/** \brief Send data frame
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkSendFrame(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
STATUS     oRes     = ERROR;
EC_T_DWORD dwRetVal = EC_E_ERROR;
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;
M_BLK_ID pMblk;
EC_T_BYTE* pMac;
int nSendProto;
EC_T_DWORD dwEffSize = pLinkFrameDesc->dwSize;
EC_T_BYTE* pbySendFrame = NULL;

    WVTRACE(10);
    if( (G_nEmSnarfMode == SNARF_MODE_MUX_POLLING) && G_nReuseMblk ) 
    {
        pMblk = pSnarfLinkDesc->pTxMblk;
        pbySendFrame = (UINT8*)pMblk->mBlkHdr.mData;
    }
    else
    {
        pMblk = (M_BLK_ID)pLinkFrameDesc->pvContext;
    }

#if (defined VXWORKS55)
    pbySendFrame = (UINT8*)pMblk->mBlkHdr.mData;
#endif

#ifdef VXWORKS55
EC_T_BYTE* pbyData;
EC_T_BYTE abyMac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    pMac = abyMac;
    nSendProto = htons(ETHERNET_FRAME_TYPE_BKHF);
    pbyData = (EC_T_BYTE*)pMblk->mBlkHdr.mData;
    dwEffSize -= 14;
    memcpy( pbySendFrame, &pLinkFrameDesc->pbyFrame[14], dwEffSize );	// ethernet header added internally!
#else
    pMac = EC_NULL;
    nSendProto = MUX_PROTO_SNARF;
    
    /* Todo testen */
    /* force broadcast: set destination mac address */
    memset(&pLinkFrameDesc->pbyFrame[0], 0xff, 6);

    if( (G_nEmSnarfMode == SNARF_MODE_MUX_POLLING) && G_nReuseMblk ) 
    {
        memcpy( pbySendFrame, pLinkFrameDesc->pbyFrame, dwEffSize );
    }
#endif


    if( pSnarfLinkDesc == EC_NULL )
    {
        goto Exit;
    }
    /* check state */
    if (pSnarfLinkDesc->pvMuxCookie == EC_NULL)
    {
        WVTRACE(11);
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }

    /* send frame */
    if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {   
        pMblk->mBlkHdr.mLen    = dwEffSize;
        pMblk->mBlkPktHdr.len  = dwEffSize;
        /* STATUS muxTkPollSend(void* pCookie, M_BLK_ID pNBuff, char* dstMacAddr, USHORT netType, void* pSpareData) */
        WVTRACE(12);
        oRes = muxTkPollSend(pSnarfLinkDesc->pvMuxCookie, pMblk, (char*)pMac, nSendProto, EC_NULL);
    }
    else
    {
        pMblk->mBlkHdr.mLen    = dwEffSize;
        pMblk->mBlkPktHdr.len  = dwEffSize;
        /* check parameters */
        if (pLinkFrameDesc->pvContext == EC_NULL)
        {
            goto Exit;
        }
        /* STATUS muxTkSend(void* pCookie, M_BLK_ID pNBuff, char* dstMacAddr, USHORT netType, void* pSpareData) */
        WVTRACE(13);
        oRes = muxTkSend(pSnarfLinkDesc->pvMuxCookie, pMblk, (char*)pMac, nSendProto, EC_NULL);
    }
    if ( oRes != OK)
    {
        //logMsg( "muxTkSend failed\n", 1,2,3,4,5,6 );
        if( (G_nEmSnarfMode == SNARF_MODE_MUX_POLLING) && G_nReuseMblk ) 
        {
            WVTRACE(14);
        }
        else
        {
        	/* mblk is not reused, free it */
            muxFreeMblk( (M_BLK_ID)pLinkFrameDesc->pvContext );
            pLinkFrameDesc->pvContext = EC_NULL;
        }
        dwRetVal = EC_E_ERROR;
        goto Exit;
    }
    if( G_nDumpTxFrm )
    {
    	emllSnarfDumpFrame( EC_TRUE, pLinkFrameDesc->pbyFrame, pLinkFrameDesc->dwSize );
    }
    
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief Send data frame and free the frame buffer. This function must be 
*          supported if EcLinkAllocSendFrame() is supported
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkSendAndFreeFrame(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
EC_UNREFPARM(pvInstance);
    return EcLinkSendFrame(pvInstance,pLinkFrameDesc);
}

/********************************************************************************/
/** \brief Poll for received frame. This function is called by the ethercat Master 
*          if the function EcLinkGetMode() returns EcLinkMode_POLLING
*
* \return EC_E_NOERROR or error code.
*/
int nNumAllocMblkErr=0;
int nNumNoRxFrm=0;
int nNumEnetDown=0;
int nNumRxErr=0;
int nNumUnknwnRxErr=0;
int nNumNonEcatFrm=0;
int nNumRxOk=0;
static EC_T_DWORD EcLinkRecvFrameMuxPoll(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
STATUS Status;
EC_T_DWORD dwRetval = EC_E_NOERROR;
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;
M_BLK_ID  pMblk = EC_NULL;
EC_T_BYTE* pbyRcvFrame = NULL;

	/* default: nothing received */
	pLinkFrameDesc->pvContext = EC_NULL;
    pLinkFrameDesc->dwSize    = 0;       
    pLinkFrameDesc->pbyFrame  = EC_NULL;

    if( G_nReuseMblk )
    {
        pMblk = pSnarfLinkDesc->pRxMblk;
    }
    else
    {
        dwRetval = muxAllocMblk( pSnarfLinkDesc, &pMblk, MAX_FRAME_SIZE );
        if( dwRetval != EC_E_NOERROR )
        {
        	nNumAllocMblkErr++;
            goto Exit;
        }
        pLinkFrameDesc->pvContext = (EC_T_VOID*)pMblk;
    }
    WVTRACE(20);
    Status = muxTkPollReceive( pSnarfLinkDesc->pvMuxCookie, pMblk, NULL );
    if( Status == EAGAIN )
    {
        /* nothing received */
        WVTRACE(21);
        nNumNoRxFrm++;
        dwRetval = EC_E_NOERROR;
        goto Exit;
    }
    if( Status != OK )
    {
        if( Status == ENETDOWN )
        {
            WVTRACE(22);
            nNumEnetDown++;
            LinkOsDbgMsg( "EcLinkRecvFrame() muxTkPollReceive returned ENETDOWN!\n" );
        }
        else if( Status == ERROR )
        {
            WVTRACE(23);
            nNumRxErr++;
            LinkOsDbgMsg( "EcLinkRecvFrame() muxTkPollReceive returned ERROR!\n" );
        }
        else
        {
        	nNumUnknwnRxErr++;
            LinkOsDbgMsg( "EcLinkRecvFrame() muxTkPollReceive returned 0x%x!\n", Status );
        }
        WVTRACE(24);
        dwRetval = EC_E_ERROR;
        goto Exit;
    }
    pbyRcvFrame = (EC_T_BYTE*)TUP_BUF_GET(pMblk);

    /* check for ethercat frame */
    if (!( (EC_ETHFRM_GET_FRAMETYPE(pbyRcvFrame) == ETHERNET_FRAME_TYPE_BKHF) ||
          ((EC_ETHFRM_GET_FRAMETYPE(pbyRcvFrame) == ETHERNET_FRAME_TYPE_VLAN) && (EC_ETHFRM_GET_FRAMETYPE(pbyRcvFrame + 2) == ETHERNET_FRAME_TYPE_BKHF))
       ))
    {
        //logMsg( "muxReceiveFrame: no ECAT frame\n", 1,2,3,4,5,6 );
        WVTRACE(25);
        nNumNonEcatFrm++;
        dwRetval = EC_E_NOERROR;
        goto Exit;
    }
    //logMsg( "muxReceiveFrame: received ECAT frame\n", 1,2,3,4,5,6 );
#ifdef DEBUG
    nSnarfPollRxFrmEcat++;
#endif
    pLinkFrameDesc->dwSize    = (EC_T_DWORD)TUP_LEN_GET(pMblk);
    pLinkFrameDesc->pbyFrame  = (EC_T_BYTE*)pMblk->mBlkHdr.mData;
    nNumRxOk++;
    WVTRACE(26);

Exit:
	if( dwRetval != EC_E_NOERROR )
	{
		if( pLinkFrameDesc->pvContext != EC_NULL )
		{
			/* release mblk if one was allocated */
	        pMblk = (M_BLK_ID)pLinkFrameDesc->pvContext;
	        if( pMblk != EC_NULL )
	        {
	            muxFreeMblk(pMblk);
	        }
		}
	}
    return dwRetval;
}
static void EcLinkRecvFrameMuxPollShow(void)
{
	printf( "nNumAllocMblkErr = %d\n", nNumAllocMblkErr ); 
	printf( "nNumNoRxFrm = %d\n", nNumNoRxFrm ); 
	printf( "nNumEnetDown = %d\n", nNumEnetDown ); 
	printf( "nNumRxErr = %d\n", nNumRxErr ); 
	printf( "nNumUnknwnRxErr = %d\n", nNumUnknwnRxErr ); 
	printf( "nNumNonEcatFrm = %d\n", nNumNonEcatFrm );
	printf( "nNumRxOk = %d\n", nNumRxOk );
}


/********************************************************************************/
/** \brief Poll for received frame. This function is called by the ethercat Master 
*          if the function EcLinkGetMode() returns EcLinkMode_POLLING
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkRecvFrameNormal(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;
EC_T_LINK_FRAMEDESC* pNextFrameDesc;
EC_T_DWORD dwNextFrameDescTailIndex;

    if( G_nEmSnarfMode == SNARF_MODE_POLLING ) 
    {
        if( pSnarfLinkDesc->dwFrameDescTailIndex == pSnarfLinkDesc->dwFrameDescHeadIndex )
        {
            /* frame buffer is empty */
            pLinkFrameDesc->dwSize = 0;
        }
        else
        {
        	/* get frame descriptor from buffer */
            dwNextFrameDescTailIndex = pSnarfLinkDesc->dwFrameDescTailIndex;
            pNextFrameDesc = &pSnarfLinkDesc->aoFrameDesc[dwNextFrameDescTailIndex];
            pLinkFrameDesc->pvContext = pNextFrameDesc->pvContext;
    	      pLinkFrameDesc->dwSize = pNextFrameDesc->dwSize;
            pLinkFrameDesc->pbyFrame = pNextFrameDesc->pbyFrame;
            dwNextFrameDescTailIndex++;
            if( dwNextFrameDescTailIndex >= FRAMEDESC_BUFFER_SIZE )
            {
                /* wrap case */
                dwNextFrameDescTailIndex = 0;
            }
            /* forward tail */
            pSnarfLinkDesc->dwFrameDescTailIndex = dwNextFrameDescTailIndex;
            WVTRACE(30);
        }
        return EC_E_NOERROR;
    }
    else
    {
        /* nothing to do */
        LinkOsDbgMsg( "EcLinkRecvFrameNormal(): ERROR - called with mode %d!\n", G_nEmSnarfMode );
        return EC_E_NOTSUPPORTED;
    }
}

/********************************************************************************/
/** \brief Poll for received frame. This function is called by the ethercat Master 
*          if the function EcLinkGetMode() returns EcLinkMode_POLLING
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkRecvFrame(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
EC_T_DWORD dwRetval = EC_E_ERROR;

	if( G_nEmSnarfMode == SNARF_MODE_POLLING ) 
    {
		dwRetval = EcLinkRecvFrameNormal(pvInstance,pLinkFrameDesc);
    } 
	else if( G_nEmSnarfMode == SNARF_MODE_INTERRUPT ) 
    {
    	dwRetval = EcLinkRecvFrameNormal(pvInstance,pLinkFrameDesc);
    } 
	else if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {
    	dwRetval = EcLinkRecvFrameMuxPoll(pvInstance,pLinkFrameDesc);
    }
	else
	{
        LinkOsDbgMsg( "EcLinkRecvFrame() invalid mode 0x%x!\n", G_nEmSnarfMode );
	}
    if( (dwRetval == EC_E_NOERROR) && G_nDumpRxFrm )
    {
    	emllSnarfDumpFrame( EC_FALSE, pLinkFrameDesc->pbyFrame, pLinkFrameDesc->dwSize );
    }
    
    return dwRetval; 
}


/********************************************************************************/
/** \brief Allocate a frame buffer used for send 
*
*    If the link layer doesn't support frame allocation, this function must return
*    EC_E_NOTSUPPORTED
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkAllocSendFrame(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc,     /* [in/out]  link frame descriptor */
    EC_T_DWORD           dwSize              /* [in]      size of the frame to allocate */
)
{
M_BLK_ID   pMblk    = EC_NULL;
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;
EC_T_DWORD dwRetVal = EC_E_ERROR;

    if( (G_nEmSnarfMode == SNARF_MODE_MUX_POLLING) && G_nReuseMblk ) 
    {
        WVTRACE(40);
        pLinkFrameDesc->pvContext = EC_NULL;
        dwRetVal = EC_E_NOTSUPPORTED;
    }
    else
    {
        WVTRACE(41);
        if( pSnarfLinkDesc == EC_NULL )
        {
            WVTRACE(42);
            goto Exit;
        }
    
        /* get a frame buffer */
        dwRetVal = muxAllocMblk( pSnarfLinkDesc, &pMblk, dwSize ); 
        if( dwRetVal != EC_E_NOERROR )
        {
            WVTRACE(43);
            pLinkFrameDesc->pvContext = EC_NULL;
            pLinkFrameDesc->pbyFrame  = EC_NULL;
            pLinkFrameDesc->dwSize    = 0;
            goto Exit;
        }
    
        /* no errors */
        pLinkFrameDesc->pvContext = (EC_T_VOID*)pMblk;
        pLinkFrameDesc->pbyFrame  = (EC_T_BYTE*)pMblk->mBlkHdr.mData;
        pLinkFrameDesc->dwSize    = dwSize;
        dwRetVal   = EC_E_NOERROR;
    }
Exit:
    return dwRetVal;
}


/********************************************************************************/
/** \brief Release a frame buffer previously allocated with EcLinkAllocFrame()
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_VOID  EcLinkFreeSendFrame(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;
M_BLK_ID pMblk = EC_NULL;

    if( (G_nEmSnarfMode == SNARF_MODE_MUX_POLLING) && G_nReuseMblk ) 
    {
        WVTRACE(50);
        LinkOsDbgMsg( "EcLinkFreeSendFrame(): FATAL Error, should not be called with MUX_POLLING_MODE enabled if Mblks are reused!\n" );
    }
    else
    {
        WVTRACE(51);
        if( pSnarfLinkDesc == EC_NULL )
        {
            WVTRACE(52);
            goto Exit;
        }
        pMblk = (M_BLK_ID)pLinkFrameDesc->pvContext;
        OsDbgAssert(pMblk != EC_NULL);
    
        if( pMblk != EC_NULL )
        {
            WVTRACE(53);
            muxFreeMblk( pMblk );
        }
        pLinkFrameDesc->pvContext = EC_NULL;
    }
Exit:
    return;
}


/********************************************************************************/
/** \brief Release a frame buffer given to the ethercat master through the receive
*          callback function
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_VOID  EcLinkFreeRecvFrame(
    EC_T_VOID*           pvInstance,
    EC_T_LINK_FRAMEDESC* pLinkFrameDesc      /* [in]  link frame descriptor */
)
{
M_BLK_ID pMblk;
EC_UNREFPARM(pvInstance);
EC_UNREFPARM(pLinkFrameDesc);
EC_T_BOOL bReleaseMblk = EC_FALSE;
/*EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;*/

    if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {
        if( G_nReuseMblk )
        {
        	/*
            pMblk = pSnarfLinkDesc->pRxMblk;
            pMblk->mBlkHdr.mLen    = 0;
            pMblk->mBlkPktHdr.len  = 0;
            */
        }
        else
        {
            bReleaseMblk = EC_TRUE;
        }
    }
    else if( G_nEmSnarfMode == SNARF_MODE_POLLING )
    {
        bReleaseMblk = EC_TRUE;
    }
    else if( G_nEmSnarfMode == SNARF_MODE_INTERRUPT )
    {
        /* nothing to do */
    }
    
    if( bReleaseMblk )
    {
        /* release mblk and frame buffer */
        WVTRACE(60);
        pMblk = (M_BLK_ID)pLinkFrameDesc->pvContext;
        if( pMblk != EC_NULL )
        {
            WVTRACE(61);
            muxFreeMblk(pMblk);
        }
        pLinkFrameDesc->pvContext = EC_NULL;
    }
    return;
}


/********************************************************************************/
/** \brief Determine current link status.
*
* \return Current link status.
*/
static EC_T_LINKSTATUS EcLinkGetStatus(EC_T_VOID* pvInstance)
{
EC_UNREFPARM(pvInstance);
    /* there is no possibility to get this information using SNARF protocol */
    return eLinkStatus_OK;
}


/********************************************************************************/
/** \brief Determine link layer MAC address
*
* \return EC_E_NOERROR or error code.
*/
static EC_T_DWORD EcLinkGetEthernetAddress(   
    EC_T_VOID* pvInstance,
    EC_T_BYTE* pbyEthernetAddress )   /* [out]  Ethernet MAC address */
{
EC_T_DWORD dwRetVal = EC_E_ERROR;
STATUS     oRes     = ERROR;
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvInstance;

    if( pSnarfLinkDesc == EC_NULL )
    {
        goto Exit;
    }
    /* check state */
    if (pSnarfLinkDesc->pvMuxCookie == EC_NULL)
    {
        dwRetVal = EC_E_INVALIDSTATE;
        goto Exit;
    }
    /* get mac address */
    oRes = muxIoctl(pSnarfLinkDesc->pvMuxCookie, EIOCGADDR, (char*)pbyEthernetAddress);
    if (oRes != OK)
    {
        goto Exit;
    }
    /* no errors */
    dwRetVal = EC_E_NOERROR;

Exit:
	return dwRetVal;
}



/********************************************************************************/
/** \brief Multi Purpose LinkLayer IOCTL
*
* \return EC_E_NOERROR or error code.
*/

static EC_T_DWORD EcLinkIoctl(
    EC_T_VOID*              pvInstance, 
    EC_T_DWORD              dwCode, 
    EC_T_LINK_IOCTLPARMS*   pParms 
                           )
{
    EC_T_DWORD                 dwRetVal    = EC_E_ERROR;
    EC_SNARF_LINK_DESC* pAdapter = (EC_SNARF_LINK_DESC*)pvInstance;

    /* check for Type Signature */
    if( EC_NULL == pAdapter )
    {
        goto Exit;
    }

    /* fan out IOCTL functions */
    switch( dwCode )
    {
    case EC_LINKIOCTL_REGISTER_FRAME_CLBK:
        {
            EC_T_LINK_FRM_RECV_CLBK* pFrameRecvClbk = {0};
            
            if(
                (EC_NULL == pParms)
                || (EC_NULL == pParms->pbyInBuf)
                || (sizeof(EC_T_LINK_FRM_RECV_CLBK) != pParms->dwInBufSize)
                )
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            
            pFrameRecvClbk = (EC_T_LINK_FRM_RECV_CLBK*)pParms->pbyInBuf;
            
            pAdapter->pvCallbackContext = pFrameRecvClbk->pvDevice;
            pAdapter->pfReceiveFrameCallback = (EC_T_RECEIVEFRAMECALLBACK)pFrameRecvClbk->pfFrameReceiveCallback;    
            dwRetVal = EC_E_NOERROR;
            break;
        }
    case EC_LINKIOCTL_UNREGISTER_FRAME_CLBK:
        {

            pAdapter->pvCallbackContext = EC_NULL;
            pAdapter->pfReceiveFrameCallback = EC_NULL; 
            dwRetVal = EC_E_NOERROR;
            break;
        }
        
    case EC_LINKIOCTL_GET_ETHERNET_ADDRESS:
        {
            if ((EC_NULL == pParms) || (EC_NULL == pParms->pbyOutBuf) || (pParms->dwOutBufSize < 6))
            {
                dwRetVal = EC_E_INVALIDPARM;
                goto Exit;
            }
            
            dwRetVal = EcLinkGetEthernetAddress(pAdapter, pParms->pbyOutBuf); 
        } break;   

    default:
        {
            dwRetVal = EC_E_NOTSUPPORTED;
            goto Exit;
            break;
        };
    };
    
    /* no error */
    
    dwRetVal = EC_E_NOERROR;
    
Exit:

    return dwRetVal;
}



/********************************************************************************/
/** \brief
*
* \return TRUE if frame is used, FALSE if it is not a ECAT frame.
*/
static EC_T_BOOL muxReceiveFrame(void* pvCookie, long type, M_BLK_ID pMblk, void *)
{
EC_T_LINK_FRAMEDESC* poFrame;
EC_T_DWORD dwCurrentFrameDescHeadIndex;
EC_T_DWORD dwNextFrameDescHeadIndex;
EC_T_BOOL bFrameProcessed  = EC_FALSE;
EC_T_LINK_FRAMEDESC oFrame = {0};
EC_T_BOOL bRetVal          = FALSE;
EC_SNARF_LINK_DESC* pSnarfLinkDesc = (EC_SNARF_LINK_DESC*)pvCookie;
EC_T_BYTE* pbyRcvFrame =EC_NULL;

#ifdef DEBUG
    nSnarfPollRxFrm++;
#endif

    if( G_nEmSnarfMode == SNARF_MODE_MUX_POLLING ) 
    {
        WVTRACE(100);
        LinkOsDbgMsg( "ERROR - muxReceiveFrame() called with MUX_POLLING_MODE enabled!!\n" );
        goto Exit;
    }

    if( pSnarfLinkDesc == EC_NULL )
    {
        //logMsg( "muxReceiveFrame: no descriptor!\n", 1,2,3,4,5,6 );
        WVTRACE(101);
        goto Exit;
    }
    pbyRcvFrame = (EC_T_BYTE*)TUP_BUF_GET(pMblk);

    /* check for ethercat frame */
    if (!( (EC_ETHFRM_GET_FRAMETYPE(pbyRcvFrame) == ETHERNET_FRAME_TYPE_BKHF) ||
          ((EC_ETHFRM_GET_FRAMETYPE(pbyRcvFrame) == ETHERNET_FRAME_TYPE_VLAN) && (EC_ETHFRM_GET_FRAMETYPE(pbyRcvFrame + 2) == ETHERNET_FRAME_TYPE_BKHF))
       ))
    {
        //logMsg( "muxReceiveFrame: no ECAT frame\n", 1,2,3,4,5,6 );
        WVTRACE(102);
        goto Exit;
    }
    //logMsg( "muxReceiveFrame: received ECAT frame\n", 1,2,3,4,5,6 );
#ifdef DEBUG
    nSnarfPollRxFrmEcat++;
#endif
    if( G_nEmSnarfMode == SNARF_MODE_POLLING ) 
    {
        WVTRACE(103);
        dwCurrentFrameDescHeadIndex = pSnarfLinkDesc->dwFrameDescHeadIndex;
        dwNextFrameDescHeadIndex = dwCurrentFrameDescHeadIndex + 1;
        if( dwNextFrameDescHeadIndex >= FRAMEDESC_BUFFER_SIZE )
        {
            /* wrap case */
            WVTRACE(104);
            dwNextFrameDescHeadIndex = 0;
        }
        if( dwNextFrameDescHeadIndex == pSnarfLinkDesc->dwFrameDescTailIndex )
        {
            WVTRACE(105);
            LinkOsDbgMsg( "AT-EM - muxReceiveFrame: frame descriptor buffer overflow (polling rate too slow or buffer size too small)\n" );
    
            /* throw away this frame */
            bRetVal = TRUE; 
            muxFreeMblk(pMblk);
            goto Exit;
        }
    
        /* store frame descriptor info in buffer */
        poFrame = &pSnarfLinkDesc->aoFrameDesc[dwCurrentFrameDescHeadIndex];
        poFrame->pvContext = (EC_T_VOID*)pMblk;
        poFrame->dwSize    = (EC_T_DWORD)TUP_LEN_GET(pMblk);
        poFrame->pbyFrame  = (EC_T_BYTE*)pMblk->mBlkHdr.mData;
        
        /* forward head */
        pSnarfLinkDesc->dwFrameDescHeadIndex = dwNextFrameDescHeadIndex;
    }
    else if( G_nEmSnarfMode == SNARF_MODE_INTERRUPT ) 
    {
        /* ring buffer or synchronization are not needed 
         * because the receive function calls are serialized 
         * through the netTask 
         */
        WVTRACE(106);
        oFrame.pvContext = (EC_T_VOID*)pMblk;
        oFrame.dwSize    = (EC_T_DWORD)TUP_LEN_GET(pMblk);
        oFrame.pbyFrame  = (EC_T_BYTE*)pMblk->mBlkHdr.mData;
    
        /* call upper layer */
        bFrameProcessed = EC_FALSE;
        pSnarfLinkDesc->pfReceiveFrameCallback(pSnarfLinkDesc->pvCallbackContext, &oFrame, &bFrameProcessed);
    
        muxFreeMblk(pMblk);
    }

    /* frame used */
    bRetVal = TRUE;

Exit:

	return bRetVal;
}

#if 0
/********************************************************************************/
/** \brief Simple test application (note: not yet tested!!!)
*
* \return N/A.
*/
EC_T_DWORD emllSnarfTestFrameRcv(EC_T_VOID* pvContext, struct _EC_T_LINK_FRAMEDESC* pLinkFrameDesc, EC_T_BOOL* pbFrameProcessed);
EC_T_DWORD emllSnarfTestNotify(EC_T_DWORD dwCode, struct _EC_T_LINK_NOTIFYPARMS* pParms);
EC_T_BOOL emllSnarfTestMsgHook(const EC_T_CHAR* szFormat, EC_T_VALIST vaArgs);
#define FRAME_DATA_SIZE 60
EC_T_BYTE   S_abyFrameData[FRAME_DATA_SIZE] =
{
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   /* Dest Mac */                            /*0-5*/
        /*0x00, 0x50, 0xc2, 0x0e, 0xcf, 0x26,   / * Src Mac * / */                      /*6-11*/
        //0x00, 0x0e, 0x0c, 0xd9, 0x67, 0xab,   /* Src Mac */                             /*6-11*/
        0x00, 0xd0, 0xb7, 0xf0, 0xc4, 0x71,   /* Src Mac */                             /*6-11*/
        0x88, 0xa4,                           /* Type ECAT */                           /*12-13*/
        0x0e, 0x10,                           /* E88A4 Length 0x0e, Type ECAT (0x1) */  /*14-15*/
        0x01,                                 /* APRD */                                /*16*/
        0x80,                                 /* Index */                               /*17*/
        0x00, 0x00,                           /* Slave Address */                       /*18-19*/
        0x30, 0x01,                           /* Offset Address */                      /*20-21*/
        0x04, 0x00,                           /* Length - Last sub command */           /*22-23*/
        0x00, 0x00,                           /* Interrupt */                           /*24-25*/
        0x00, 0x00,                           /* Data */                                /*26-27*/
        0x00, 0x00,                           /* Data 2 */                              /*28-29*/
        0x00, 0x00                            /* Working Counter */                     /*30-31*/
};
EC_T_BYTE   S_abyFrameDataTxBuff[FRAME_DATA_SIZE];

/***************************************************************************************************/
/**
\brief  Fill common link layer parameters
*/
static EC_T_VOID LinkParmsInit(EC_T_LINK_PARMS* pLinkParms, 
                               const EC_T_DWORD dwSignature, const EC_T_DWORD dwSize, const char* szDriverIdent,
                               const EC_T_DWORD dwInstance, const EC_T_LINKMODE eLinkMode, const EC_T_DWORD dwIstPriority = 0)
{
    OsMemset(pLinkParms, 0, sizeof(EC_T_LINK_PARMS));
    pLinkParms->dwSignature = dwSignature;
    pLinkParms->dwSize = dwSize;
    OsStrncpy(pLinkParms->szDriverIdent, szDriverIdent, MAX_DRIVER_IDENT_LEN - 1);
    pLinkParms->dwInstance = dwInstance;
    pLinkParms->eLinkMode = eLinkMode;
    pLinkParms->dwIstPriority = dwIstPriority;
}

EC_T_VOID emllSnarfTest
(EC_T_CHAR*  szDevName
,int		 nNumFrames
)
{
EC_T_LINK_PARMS_SNARF LinkDevParm;
EC_T_DWORD dwStatus;
EC_T_VOID* pvLLInstance;
EC_T_LINK_FRAMEDESC oFrame = {0};
int nFrameCnt;

    if( szDevName == EC_NULL )
    {
        printf( "emllSnarfTest: device name missing!\n" );
        return;
    }
    
    /* initialization */
    /*S_pfLinkOsDbgMsgHook = emllSnarfTestMsgHook;*/
    sysClkRateSet(1000);
    if(nNumFrames == 0) nNumFrames=1;
    LinkOsAddDbgMsgHook(emllSnarfTestMsgHook);
    LinkDevParm.linkParms.dwSignature = EC_LINK_PARMS_SIGNATURE_SNARF;
    OsMemset(&LinkDevParm, 0, sizeof(EC_T_LINK_PARMS_SNARF));
    LinkParmsInit(&LinkDevParm.linkParms, EC_LINK_PARMS_SIGNATURE_SNARF, sizeof(EC_T_LINK_PARMS_SNARF), EC_LINK_PARMS_IDENT_SNARF, 1, EcLinkMode_POLLING);

    /* get adapter name */
    OsSnprintf(LinkDevParm.szAdapterName, MAX_LEN_SNARF_ADAPTER_NAME, "%s", szDevName);

    dwStatus = EcLinkOpen( &LinkDevParm, emllSnarfTestFrameRcv, emllSnarfTestNotify, EC_NULL, &pvLLInstance );
    if( szDevName == EC_NULL )
    {
        printf( "emllSnarfTest: error 0x%x in EcLinkOpen!\n", (unsigned int)dwStatus );
        return;
    }

    /* get frame buffer */
    memset(&oFrame,0,sizeof(oFrame));
    dwStatus = EcLinkAllocSendFrame( pvLLInstance, &oFrame, FRAME_DATA_SIZE );
    if( dwStatus == EC_E_NOTSUPPORTED )
    {
        // we must provide the send buffer 
        oFrame.pvContext = EC_NULL;
        oFrame.pbyFrame  = S_abyFrameDataTxBuff;
        oFrame.dwSize = FRAME_DATA_SIZE;
    }
    else if( dwStatus != EC_E_NOERROR )
    {
        printf( "EcLinkAllocSendFrame returned error 0x%x\n", (unsigned int)dwStatus );
        return;
    }
    if( oFrame.dwSize != FRAME_DATA_SIZE )
    {
        printf( "EcLinkAllocSendFrame returned invalid frame size %d instead of %d\n", (unsigned int)oFrame.dwSize, FRAME_DATA_SIZE );
        return;
    }
    /* Generate SendData */
    memcpy( oFrame.pbyFrame, S_abyFrameData, FRAME_DATA_SIZE );
    

    for( nFrameCnt = 0; nFrameCnt < nNumFrames; nFrameCnt++ )
    {
		/* send frame */
        memcpy( oFrame.pbyFrame, S_abyFrameData, FRAME_DATA_SIZE );
		dwStatus = EcLinkSendAndFreeFrame( pvLLInstance, &oFrame );
		if( dwStatus != EC_E_NOERROR )
		{
			printf( "EcLinkSendAndFreeFrame returned error 0x%x\n", (unsigned int)dwStatus );
			return;
		}
		
		/* wait one tick, then receive frame  */
		taskDelay( 1 );
		
		dwStatus = EcLinkRecvFrame( pvLLInstance, &oFrame );
		if( dwStatus != EC_E_NOERROR )
		{
			printf( "EcLinkRecvFrame returned error 0x%x\n", (unsigned int)dwStatus );
			return;
		}
		printf( "EcLinkRecvFrame: received %d bytes\n", (int)oFrame.dwSize );
    }
    dwStatus = EcLinkClose( pvLLInstance );
    if( dwStatus != EC_E_NOERROR )
    {
        printf( "EcLinkClose returned error 0x%x\n", (unsigned int)dwStatus );
        return;
    }
}

EC_T_BOOL emllSnarfTestMsgHook
(   const
    EC_T_CHAR* szFormat,
    EC_T_VALIST vaArgs
)
{
	printf("emllSnarfTestMsgHook called with format: %s\n", szFormat);
    return EC_TRUE;
}

EC_T_DWORD emllSnarfTestFrameRcv
(   EC_T_VOID* pvContext,
    struct _EC_T_LINK_FRAMEDESC* pLinkFrameDesc,
    EC_T_BOOL* pbFrameProcessed
)
{
    return EC_E_NOERROR;
}
EC_T_DWORD emllSnarfTestNotify
(   EC_T_DWORD dwCode,
    struct _EC_T_LINK_NOTIFYPARMS* pParms
)
{
    return EC_E_NOERROR;
}
#endif

/*-END OF SOURCE FILE--------------------------------------------------------*/

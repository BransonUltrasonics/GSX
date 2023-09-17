/* httpAuth.c -  Javascript Authentication Support */

/* Copyright(c) 2004-2007, 2010, 2012-2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
28may14,r_w  fix compile warnings(V7MAN-78)
05may14,r_w  fix static analysis issues(US35919)
05nov12,r_w  prevent for NULL pointer access for defect WIND00387001
27jul12,r_w  fix pointer error. defect WIND00365788
18apr12,r_w  move some routines from rc directory to this file
             Requirement: WIND00299859
29jan10,m_z  VxWorks 64-bit support
             (WIND00193649)
07mar07,ps   fixes for RTP builds. defect WIND00044864
24feb07,msa  Correct the apigen comments so they match the actual routines
             and correct any apigen errors that may occur.
01sep06,msa  Change define so debug is not compiled by default.
26jul06,ps   update for IPV6
25apr05,???  Update JS Auth #define ordering
22apr05,dkg  Update from source, 4/22/05
17sep04,dkg  ported from RC3.3 to WM4.4 for JavaScript Authentication
 */


/*
DESCRIPTION
This module contains support routines for Java Script Authentication.

INCLUDE FILES: wm_options.h
 */


/* See if Digest is enabled globally */
#include "commonCfg.h"
#include "httpCfg.h"

#ifdef __JAVASCRIPT_DIGEST_ENABLED__

#include <stdio.h>
#include "httpLib.h"
#include "httpReq.h"
#include "httpAuth.h"
#include "wmMD5.h"

#if 0
#define __DEBUG_AUTH__
#endif

#ifdef __DEBUG_AUTH__
/* for inet_ntop () */
#include <sys/socket.h>
#include <arpa/inet.h> 
#endif

#define __ZERO_TERM_DIGEST_AUTH__	/* add zero terminator to DIGEST */
#define __NONCE_BY_INC_AUTH__		/* add incrementer to nonce, not just seconds uptime */
#define __NONCE_STR_AUTH__		/* Do STRCMP on this string, not MEMCMP */


#define kIllegal_IpAddr     0xffffabab /* was 0 */
#ifndef kHwMaxSimultaneousClients
#define kHwMaxSimultaneousClients 10
#endif
#ifndef kHwIdleTimeoutSecs
#define kHwIdleTimeoutSecs 120
#endif

/* ENVIRONMENT_GetEnvironment constants */
#define kenv_REQUEST_METHOD     0
#define kenv_DOCUMENT_NAME      1
#define kenv_ACCEPT             2
#define kenv_AUTHORIZATION      3
#define kenv_COOKIE             4
#define kenv_CONTENT_LENGTH     5
#define kenv_CONTENT_TYPE       6
#define kenv_CONNECTION         7
#define kenv_HOST               8
#define kenv_LANGUAGE           9
#define kenv_PRAGMA             10
#define kenv_REFERER            11
#define kenv_USER_AGENT         12
#define kenv_QUERY_STRING       13
#define kenv_HTTP_VERSION       14

/*-----------------------------------------------------------------------*/

typedef struct authInfo
{
	WMNET_IP_ADDR ClientIp46Addr;

	WM_ACCESS_T *ClientAccessLevel;
	BOOL     ClientAuthenticated;            /* has the client been authenticated yet? */
	sbyte*      pUserName;

	Counter     ConsecutiveIllegalAttempts;     /* Covert break in attempts */
	Counter     ConsecutiveFailedLogins;        /* Basic login failures */

#ifndef _WRS_CONFIG_LP64
	ubyte4      FirstLoginSecs;                 /* used for keeping track when user first logged in */
	ubyte4      LastAccessSecs;                 /* used for detecting idle timeouts */
#else
	ubyte8      FirstLoginSecs;                 /* used for keeping track when user first logged in */
	ubyte8      LastAccessSecs;                 /* used for detecting idle timeouts */
#endif

	sbyte       SentNonce[kMaxNonceLen];
	sbyte       ReplyDigest[kDigestMesgBufSize];

} authInfo;



/*-----------------------------------------------------------------------*/

static OS_SEM_T    mAuthMutex;
static authInfo    *LoggedInClients = NULL;

void*  (*pwmGetAuthenticationLevelPassword)(void *envVar, char *pUserName, char *pPassword) = NULL;
/*-----------------------------------------------------------------------*/


static void wmClearClientRecords(authInfo *pClientRecord)
{
	MEMSET(pClientRecord, 0x00, sizeof(authInfo));
	pClientRecord->ClientAuthenticated  = FALSE;
	/*
	 * we use a "len" field of zero to indicate an invalid IP address 
	 */
	pClientRecord->ClientIp46Addr.len = 0;  
}

#ifdef __DEBUG_AUTH__

STATUS wmAuthShow()
{
	int clientIndex;
	char *nonce,*user;

	char        ipAddrStr [INET6_ADDRSTRLEN + 1];
	WMNET_IP_ADDR *clientIpAddr;
	WM_ACCESS_T ClientAccessLevel;
	BOOL     ClientAuthenticated;
	Counter     ConsecutiveIllegalAttempts;
	Counter     ConsecutiveFailedLogins;
#ifndef _WRS_CONFIG_LP64
	ubyte4      FirstLoginSecs;
	ubyte4      LastAccessSecs;
	sbyte4      LeftAccessSecs;
#else
	ubyte8      FirstLoginSecs;
	ubyte8      LastAccessSecs;
	sbyte8      LeftAccessSecs;
#endif


	printf("Clients  IpAddr     Access   A FstSec LstSec TimLft Nonce... Name...\n");
	printf("-------- ---------- -------- - ------ ------ ------ -------- --------.\n");

	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
	{    
		nonce = LoggedInClients[clientIndex].SentNonce;
		user  = LoggedInClients[clientIndex].pUserName;
		LeftAccessSecs =
				(sbyte4) kHwIdleTimeoutSecs - (sbyte4)(wmosTimeGet() - LoggedInClients[clientIndex].LastAccessSecs);

		if (NULL != LoggedInClients[clientIndex].ClientAccessLevel)
			ClientAccessLevel.read			= LoggedInClients[clientIndex].ClientAccessLevel->read;
		else
			ClientAccessLevel.read			= 0;
		ClientAuthenticated			= LoggedInClients[clientIndex].ClientAuthenticated;
		ConsecutiveIllegalAttempts      	= LoggedInClients[clientIndex].ConsecutiveIllegalAttempts;
		ConsecutiveFailedLogins		        = LoggedInClients[clientIndex].ConsecutiveFailedLogins;
		FirstLoginSecs				= LoggedInClients[clientIndex].FirstLoginSecs;
		LastAccessSecs				= LoggedInClients[clientIndex].LastAccessSecs;

		clientIpAddr = &(LoggedInClients[clientIndex].ClientIp46Addr);

		inet_ntop (clientIpAddr->family, clientIpAddr->ipAddrBytes, 
				ipAddrStr, INET6_ADDRSTRLEN + 1);

		printf("AUTH[%2d]:%46s,%8d,%1d,%6lu,%6lu,%6lu,%s,%s.\n",
				clientIndex,
				ipAddrStr,
				ClientAccessLevel.read,
				ClientAuthenticated,
				/*	ConsecutiveIllegalAttempts,  */
				/*	ConsecutiveFailedLogins,     */
				(unsigned long) FirstLoginSecs,
				(unsigned long) LastAccessSecs,
				(unsigned long) LeftAccessSecs,
				(NULL != nonce) ? nonce:"<null>",
						(NULL != user ) ? user :"<null>"
		);
	}
	printf(" Time (seconds) Now = %d, Timeout = %d \n",wmosTimeGet(),kHwIdleTimeoutSecs);

	return 0;
}

/* This is a place to breakpoint and explore the environment */
RLSTATUS AUTH_HALT_BAD_IP(WMB_SESSION_T *p_envVar)
{
#if 0
	printf("AUTH_HALT_BAD_IP: ILLEGAL IP (0x%08lx)!!!\n",(unsigned long) p_envVar->IpAddr);
#else
	printf("AUTH_HALT_BAD_IP: ILLEGAL IP \n");
#endif 
	return 0;
}

#endif



/*-----------------------------------------------------------------------*/

extern STATUS wmAUTHConstruct(void)
{
	int clientIndex;

	if (NULL == (LoggedInClients = wmMalloc(sizeof(authInfo) * kHwMaxSimultaneousClients)))
		return SYS_ERROR_NO_MEMORY;

	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
		wmClearClientRecords(&(LoggedInClients[clientIndex]));

	return wmosSemCreate(&mAuthMutex, 1, 0);
}



/*-----------------------------------------------------------------------*/

static void wmAuthLock(void)
{
	wmosSemTake(mAuthMutex, WAIT_FOREVER_K);
}



/*-----------------------------------------------------------------------*/

static void wmAuthUnlock(void)
{
	(void)wmosSemGive(mAuthMutex);
}

static BOOL wmnetIpAddrEqual 
(
		WMNET_IP_ADDR *addr1,
		WMNET_IP_ADDR *addr2
)
{

#ifdef __DEBUG_AUTH__
	char addrstr [64];

	inet_ntop (addr1->family, addr1->ipAddrBytes, addrstr, 64);
	printf ("comparing addr1 %s ", addrstr);
	inet_ntop (addr2->family, addr2->ipAddrBytes, addrstr, 64);
	printf ("vs addr2 %s\n", addrstr);
#endif

	if (    (addr1->family != addr2->family)
			|| (addr1->len    != addr2->len)
			|| (0 != memcmp (addr1->ipAddrBytes, addr2->ipAddrBytes, addr1->len))
	)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


static void wmAuthCopyWmnetIpAddr 
( 
		WMNET_IP_ADDR *from, 
		WMNET_IP_ADDR *to
)
{
	to->family = from->family;
	to->len = from->len;

	/* somewhat paranoid, but extra defensiveness here cant hurt */
	if (from->len == 4 || from->len == 16)
		memcpy (to->ipAddrBytes, from->ipAddrBytes, from->len);

}

/*-----------------------------------------------------------------------*/

static BOOL wmVerifyCookie(WMB_SESSION_T *p_envVar, authInfo *pUser );

static sbyte4 wmFindClientLocation(WMNET_IP_ADDR *IpAddr, WMB_SESSION_T *p_envVar) 
{
	int clientIndex;

	WMNET_IP_ADDR *c_addr;

	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
	{
		c_addr = &(LoggedInClients[clientIndex].ClientIp46Addr);

		if ( (c_addr->len > 0) && wmnetIpAddrEqual (c_addr, IpAddr) )
		{
#ifdef __DEBUG_AUTH__
printf("wmFindClientLocation: wmVerifyCookie[%d].\n",clientIndex);
#endif
if (TRUE == wmVerifyCookie(p_envVar, &LoggedInClients[clientIndex]))
	return clientIndex;
		}
	}
	return ERROR_GENERAL_NOT_FOUND;
}


/*-----------------------------------------------------------------------*/

static sbyte4 wmFindFreeClientLocation(void)
{
	int clientIndex;

	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
       to make the system more sophisticated, we should have a method for logging out
       a lower access level user, to allow a higher user into the device.  JAB */

	/* To optimize the search time consolidate these searches.  JAB */
	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
	{
		if (LoggedInClients[clientIndex].ClientIp46Addr.len == 0)
			return clientIndex;
	}

	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
	{
		if (kHwIdleTimeoutSecs < (wmosTimeGet() - LoggedInClients[clientIndex].LastAccessSecs))
			return clientIndex;
	}

	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
	{
		if (FALSE == LoggedInClients[clientIndex].ClientAuthenticated)
			return clientIndex;

		if (NULL == LoggedInClients[clientIndex].pUserName)
			return clientIndex;
	}

	/* we probably want to keep track of these buggers, so that we can lock them out.  JAB */
	for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
	{
		if (0 < LoggedInClients[clientIndex].ConsecutiveIllegalAttempts)
			return clientIndex;

		if (0 < LoggedInClients[clientIndex].ConsecutiveFailedLogins)
			return clientIndex;
	}

	return ERROR_GENERAL_NOT_FOUND;

} /* wmFindFreeClientLocation */



/*-----------------------------------------------------------------------*/

static BOOL wmIdleTimeoutOccurred(sbyte4 clientIndex)
{
	/* if an invalid clientIndex, return TRUE --- idle timeout has occurred */
	if ((0 > clientIndex) || (kHwMaxSimultaneousClients <= clientIndex))
		return TRUE;

	/* has the user been idle for kHwIdleTimeoutSecs seconds? */
	if (kHwIdleTimeoutSecs < (wmosTimeGet() - LoggedInClients[clientIndex].LastAccessSecs))
		return TRUE;

	/* user is still connected */
	return FALSE;
}



/*-----------------------------------------------------------------------*/

static BOOL wmUserAuthenticated(sbyte4 clientIndex)
{
	return LoggedInClients[clientIndex].ClientAuthenticated;
}



/*-----------------------------------------------------------------------*/

static BOOL wmProperAuthRequest(WMB_SESSION_T *p_envVar, sbyte4 clientIndex)
{
	sbyte Nonce[kMaxNonceLen+1];
	sbyte HtmlDigest[128+1];

	Nonce[kMaxNonceLen]            = '\0';
	HtmlDigest[kDigestMesgBufSize] = '\0';

	if ((OK == CgiLookup(p_envVar, "nonce", Nonce, kMaxNonceLen)) &&
			(OK == CgiLookup(p_envVar, "encoded", HtmlDigest, 128)) )
	{
		sbyte *pNonce = STRCHR(HtmlDigest, ':');

		if (NULL == pNonce)
		{
#ifdef __DEBUG_AUTH__
			printf("wmProperAuthRequest: FAIL - Missing nonce.\n");
#endif
			return FALSE;
		}

		*pNonce = '\0';
		pNonce++;

		if (NULL != LoggedInClients[clientIndex].pUserName)
		{
			wmFree(LoggedInClients[clientIndex].pUserName);
			LoggedInClients[clientIndex].pUserName = NULL;
		}

		if (NULL == (LoggedInClients[clientIndex].pUserName = wmMalloc(STRLEN(HtmlDigest)+1)))
		{
#ifdef __DEBUG_AUTH__
printf("wmProperAuthRequest: FAIL - Out of memory.\n");
#endif
return FALSE;
		}

		STRCPY(LoggedInClients[clientIndex].pUserName, HtmlDigest);
		MEMCPY(LoggedInClients[clientIndex].ReplyDigest, pNonce, kDigestMesgBufSize);

#ifdef __NONCE_STR_AUTH__
		if (0 == STRNCMP(Nonce, LoggedInClients[clientIndex].SentNonce, kMaxNonceLen))
			return TRUE;
#else
		if (0 == MEMCMP(Nonce, LoggedInClients[clientIndex].SentNonce, kMaxNonceLen))
			return TRUE;
#endif
	}

#ifdef __DEBUG_AUTH__
	printf("wmProperAuthRequest: FAIL - Client not found.\n");
	printf("      Nonce=%s, SentNonce=%s. \n",Nonce, LoggedInClients[clientIndex].SentNonce);
	wmAuthShow();
#endif

	return FALSE;
}

/*-----------------------------------------------------------------------*/

#define MostSignificantHexDigit(x) ((x & 0xf0000000) >> 28)
#ifdef _WRS_CONFIG_LP64
#define MostSignificantHexDigit64(x) ((x & 0xf000000000000000) >> 60)
#endif

static sbyte wmBitsToHex(ubyte4 num)
{
	if (10 > num)
		return (sbyte)('0' + num);
	else
		return (sbyte)('a' + (num - 10));
}



/*-----------------------------------------------------------------------*/

static void wmIpAddrToHex8(WMNET_IP_ADDR *ip46Addr, sbyte *pBuf)
{
	sbyte4 i;
	ubyte4 num;
	ubyte4 newDigit;

	memcpy (&num, ip46Addr->ipAddrBytes, 4);

	for(i=0; i<8; i++)
	{
		newDigit = MostSignificantHexDigit(num);
		pBuf[i] = wmBitsToHex(newDigit);
		num = num << 4;
	}

	pBuf[i] = '\0';
}

#ifndef _WRS_CONFIG_LP64
static void wmIntToHex8(ubyte4 num, sbyte *pBuf)
{
	sbyte4 i;
	ubyte4 newDigit;

	for(i=0; i<8; i++)
	{
		newDigit = MostSignificantHexDigit(num);
		pBuf[i] = wmBitsToHex(newDigit);
		num = num << 4;
	}

	pBuf[i] = '\0';
}
#else
	static void wmIntToHex16(ubyte8 num, sbyte *pBuf)
	{
		sbyte4 i;
		ubyte4 newDigit;

		for(i=0; i<16; i++)
		{
			newDigit = (ubyte4)MostSignificantHexDigit64(num);
			pBuf[i] = wmBitsToHex(newDigit);
			num = num << 4;
		}

		pBuf[i] = '\0';
	}
#endif

/*-----------------------------------------------------------------------*/

#ifdef __NONCE_BY_INC_AUTH__
	static int nNonceCounter = 0;
#endif

	static void wmAddNewUserToTable(WMB_SESSION_T *p_envVar, sbyte4 clientIndex)
	{
		/* Preclear the client record */
		wmClearClientRecords(&(LoggedInClients[clientIndex]));

#ifdef __DEBUG_AUTH__
		if (0 == p_envVar->wmnet_IpAddr.len)
		{
			printf("wmAddNewUserToTable: SETTING AN INVALID IP address\n");
			AUTH_HALT_BAD_IP(p_envVar);
		}
#endif

		wmAuthCopyWmnetIpAddr ( &(p_envVar->wmnet_IpAddr),
				&(LoggedInClients[clientIndex].ClientIp46Addr) );

		LoggedInClients[clientIndex].ClientAccessLevel   = NULL;
		LoggedInClients[clientIndex].ClientAuthenticated = FALSE;

		if (NULL != LoggedInClients[clientIndex].pUserName)
		{
			wmFree(LoggedInClients[clientIndex].pUserName);
			LoggedInClients[clientIndex].pUserName = NULL;
		}

		LoggedInClients[clientIndex].ConsecutiveIllegalAttempts = 0;
		LoggedInClients[clientIndex].ConsecutiveFailedLogins    = 0;

		/* !-!-!-!-! Use wmosTimeGet(), since we don't have access to RAND(). JAB */
		LoggedInClients[clientIndex].FirstLoginSecs = wmosTimeGet();
		LoggedInClients[clientIndex].LastAccessSecs = wmosTimeGet();


		wmIpAddrToHex8(&(p_envVar->wmnet_IpAddr), LoggedInClients[clientIndex].SentNonce);

#ifdef __NONCE_BY_INC_AUTH__
		/* Append the seconds uptime, plus an incrementing number per nonce for greater grandularity */
#ifndef _WRS_CONFIG_LP64
		wmIntToHex8((wmosTimeGet() << 8)+(nNonceCounter++ & 0x0ff), &(LoggedInClients[clientIndex].SentNonce[8]));
#else
		wmIntToHex16((wmosTimeGet() << 8)+(nNonceCounter++ & 0x0ff), &(LoggedInClients[clientIndex].SentNonce[8]));
#endif
#else
		/* Append the seconds uptime */
#ifndef _WRS_CONFIG_LP64
		wmIntToHex8(wmosTimeGet(), &(LoggedInClients[clientIndex].SentNonce[8]));
#else
		wmIntToHex16(wmosTimeGet(), &(LoggedInClients[clientIndex].SentNonce[8]));
#endif
#endif


	}



	/*-----------------------------------------------------------------------*/

#define kUsernameLen    16
#define kPasswordLen    16

	static WM_ACCESS_T* wmVerifyUser(WMB_SESSION_T *p_envVar, authInfo *clientInfo,char *pAuthMessage,
			char *pUserName, char *pNonce, WMNET_IP_ADDR *Ip46Addr)
	{
		sbyte Password[kPasswordLen + 1];
		sbyte AssembledNonce[kUsernameLen + 1 + kPasswordLen + 1 + (kDigestMesgBufSize * 2) + 1];
		sbyte DigestedMessage[kDigestMesgBufSize];
		sbyte DigestOutput[128];
		sbyte *pDigestOutput = DigestOutput;
		WM_ACCESS_T *pAccessLevel = NULL;
		sbyte4   k;
		sbyte4   assembledLen = 0;

		/* Lookup password based on username, simulation */    
		if ((NULL == pwmGetAuthenticationLevelPassword)||
				(NULL == (pAccessLevel = (WM_ACCESS_T*) (*pwmGetAuthenticationLevelPassword) (p_envVar, pUserName, Password)))
		)
		{
#ifdef __DEBUG_AUTH__
			printf("wmVerifyUser: CUSTOM_AUTHENTICATION_LEVEL DID NOT PASS.\n");
#endif
			return NULL;
		}
#ifdef __DEBUG_AUTH__
		printf("wmVerifyUser: CUSTOM_AUTHENTICATION_LEVEL Fetched! (%s,%s)(read access: %d, write access :%d).\n",pUserName, Password, pAccessLevel->read,pAccessLevel->write);
#endif

		/*    sprintf(AssembledNonce, "%s:%s:%s", pUserName, Password, pNonce); */
		if(STRLEN(pUserName)< kUsernameLen)
		{
			STRCPY(AssembledNonce, pUserName);
			assembledLen += (sbyte4)STRLEN(pUserName);
		}
		else
		{
			MEMCPY(AssembledNonce, pUserName,kUsernameLen);
			assembledLen += kUsernameLen;
		}
		AssembledNonce[assembledLen++] = ':';
		STRCPY(&AssembledNonce[assembledLen], Password);
		assembledLen += (sbyte4)STRLEN(Password);
		AssembledNonce[assembledLen++] = ':';
		STRCPY(&AssembledNonce[assembledLen], pNonce);

		if (OK > wmMD5MessageDigest(AssembledNonce, STRLEN(AssembledNonce), DigestedMessage))
		{
#ifdef __DEBUG_AUTH__
			printf("wmVerifyUser: wmMD5MessageDigest DID NOT PASS.\n");
#endif
			return NULL;
		}

		for (k = 0; k < kDigestMesgBufSize; k++)
		{
			*pDigestOutput = wmBitsToHex((((ubyte4)DigestedMessage[k]) & 0x0f0) >> 4);
			pDigestOutput++;
			*pDigestOutput = wmBitsToHex((ubyte4)DigestedMessage[k] & 0x0f);
			pDigestOutput++;
		}

#ifdef __ZERO_TERM_DIGEST_AUTH__
		*pDigestOutput = '\0';
#endif

		if (kMD5_Match == wmMD5MessageCompare(pAuthMessage, DigestOutput))
		{
			clientInfo->ClientAuthenticated = TRUE;

#ifdef __DEBUG_AUTH__
			printf("wmVerifyUser: wmMD5MessageCompare passed: We're authenticated!.\n");
#endif
			/* don't want a clear text password floating around on the stack! */
			MEMSET(Password, 0x00, kPasswordLen);
			return pAccessLevel;
		}

		MEMSET(Password, 0x00, kPasswordLen);

#ifdef __DEBUG_AUTH__
		printf("wmVerifyUser: wmMD5MessageCompare DID NOT PASS.\n");
		printf("  pAuthMessage = <%s>\n",pAuthMessage);
		printf("  DigestOutput = <%s>\n",DigestOutput);
#endif
		return NULL;
	}

	/*-----------------------------------------------------------------------*/

	static BOOL wmVerifyCookie(WMB_SESSION_T *p_envVar, authInfo *pUser )
	{
		sbyte *pCookie = pUser->SentNonce;
		sbyte *pSentCookie = NULL;
		HTTP_REQ_ID reqId;

		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
			return wmError(ERROR_GENERAL, "invalid parameters");

		pSentCookie =  httpMimeInCookieGet(reqId,"COOKIE");

		if ( NULL != pSentCookie )
		{
			if (0 == STRCMP(pCookie, pSentCookie))
			{
#ifdef __DEBUG_AUTH__
				printf("wmVerifyCookie: NOTE - Cookie is a match!\n");
#endif
				return TRUE;
			}
		}
		else
			return FALSE;
#ifdef __DEBUG_AUTH__
		printf("wmVerifyCookie: Cookie not same.\n");
#endif
		return FALSE;
	}



	/*-----------------------------------------------------------------------*/

	static BOOL wmIsAuthenticationPost(WMB_SESSION_T *p_envVar)
	{
		sbyte *pTemp = NULL;
		sbyte *p_reqMethod =
				ENVIRONMENT_GetEnvironment(p_envVar, kenv_REQUEST_METHOD);

		if (NULL == p_reqMethod)
			return FALSE;

		pTemp = p_reqMethod;
		for ( ; '\0' != *p_reqMethod; p_reqMethod++)
			*p_reqMethod = (sbyte)(TOUPPER(*p_reqMethod));

		if (0 == STRCMP("POST", pTemp))
		{
			sbyte Nonce[kMaxNonceLen+1];
			if (OK == CgiLookup(p_envVar, "nonce", Nonce, kMaxNonceLen))
				return TRUE;
			return FALSE;
		}
		else
			return FALSE;
	}

	/*-----------------------------------------------------------------------*/

	extern WM_ACCESS_T* wmAUTHCheckAuthentication(WMB_SESSION_T *p_envVar)
	{
		sbyte4  clientIndex;
		HTTP_REQ_ID reqId;
		sbyte   *pSentCookie;
#ifdef __DEBUG_AUTH__
		sbyte   *p_reqMethod;
#endif
		WM_ACCESS_T	*retval;

#if 0
		RegDes* ph_regObject;
		sbyte*  Resource;
		Access readAccess;
		Access writeAccess;
#endif

#ifdef __DEBUG_AUTH__
		sbyte   temp[256];
		int		val;
		char ipAddrStr [INET6_ADDRSTRLEN + 1];
		WMNET_IP_ADDR *clientIpAddr;
#endif

		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
		{
			wmError(ERROR_GENERAL, "invalid parameters");
			return NULL;
		}

		pSentCookie =  httpMimeInCookieGet(reqId,"COOKIE");


#ifdef __DEBUG_AUTH__
		p_reqMethod = ENVIRONMENT_GetEnvironment(p_envVar, kenv_REQUEST_METHOD);
#endif


		wmAuthLock();


#ifdef __DEBUG_AUTH__
		/* Display the basic authentication request environment */
		printf("\nwmAUTHCheckAuthentication: Start : '%s'.\n", p_reqMethod);

		if (NULL != pSentCookie)
			printf("wmAUTHCheckAuthentication: pSentCookie = '%s'.\n", pSentCookie);

		if (OK == (val=CgiLookup(p_envVar, "nonce", temp, sizeof(temp))))
			printf("wmAUTHCheckAuthentication: pSent nonce = '%s' (%d).\n", temp,val);

		if (OK == CgiLookup(p_envVar, "encoded", temp, sizeof(temp)))
			printf("wmAUTHCheckAuthentication: pSnt encoded= '%s'.\n", temp);

#endif

		if (0 > (clientIndex = wmFindClientLocation(&(p_envVar->wmnet_IpAddr), p_envVar)))
		{
			if (0 > (clientIndex = wmFindFreeClientLocation()))
			{
#ifdef __DEBUG_AUTH__
				printf("wmAUTHCheckAuthentication: TOO BUSY\n");
#endif
				wmAuthUnlock();
				wmError(-500, "Server Too Busy");
				return NULL;
			}

#ifdef __DEBUG_AUTH__
			clientIpAddr = &(p_envVar->wmnet_IpAddr);
			inet_ntop (clientIpAddr->family, clientIpAddr->ipAddrBytes, 
					ipAddrStr, INET6_ADDRSTRLEN + 1);

			printf("AUTH_AddNewUserToTable[%2d]: IP=%46s,Nonce=<%s>\n",clientIndex, ipAddrStr, LoggedInClients[clientIndex].SentNonce);
#if 0
			if ((0 == p_envVar->IpAddr) || (kIllegal_IpAddr == p_envVar->IpAddr))
				AUTH_HALT_BAD_IP(p_envVar);
#endif
#endif

			wmAddNewUserToTable(p_envVar, clientIndex);


			p_envVar->clientIndex = clientIndex;


#ifdef __DEBUG_AUTH__
			printf("wmAUTHCheckAuthentication: Able to add new user to logon list.\n");
#endif

			wmAuthUnlock();
			return NULL;                   /* return lowest access level */
		}

		p_envVar->clientIndex = clientIndex;


		if (TRUE == wmIdleTimeoutOccurred(clientIndex))
		{
			wmAddNewUserToTable(p_envVar, clientIndex);

#ifdef __DEBUG_AUTH__
			printf("wmAUTHCheckAuthentication: Idle timeout swap occurred.\n");
#endif

			wmAuthUnlock();

			return NULL;                   /* return lowest access level */
		}

		/* If a user is authenticated, but the cookie we're receiving is NULL,
		 * the user must be logging in again before his original connection
		 * timed out.  So go ahead and create a proper authentication request.
		 */

		/* addendum - we need do an additional check to see if it's a new request for validation - i.e. the
		 * may have already authenticated but is switching to a higher access level.  In this case, we
		 * have to first check to see if it's a POST request, then check to see if it's a JS authentication POST,
		 * then check the auth string in the CGI stream if that's the case.  Otherwise we'd ignore subsequent
		 * authentication requests....
		 */
#ifdef AUTH_PROXY_BEHAVIOR_FIX
		if (( FALSE == wmUserAuthenticated(clientIndex)) &&
				( NULL == pSentCookie ))
#else
			if (( FALSE == wmUserAuthenticated(clientIndex)) ||
					( NULL == pSentCookie ) ||
					( TRUE == wmIsAuthenticationPost(p_envVar)))
#endif
			{
				if (FALSE == wmProperAuthRequest(p_envVar, clientIndex))
				{
					LoggedInClients[clientIndex].ConsecutiveIllegalAttempts++;

#ifdef __DEBUG_AUTH__
					printf("wmAUTHCheckAuthentication: Improper auth request.\n");
#endif

					wmAuthUnlock();

					return NULL;                   /* disconnect user */
				}

#ifdef __DEBUG_AUTH__
				printf("wmAUTHCheckAuthentication: VerifyUser[%d].\n",clientIndex);
#endif

				/* authenticate the user */
				LoggedInClients[clientIndex].ClientAccessLevel =
						wmVerifyUser(p_envVar,
								&LoggedInClients[clientIndex],
								LoggedInClients[clientIndex].ReplyDigest, LoggedInClients[clientIndex].pUserName,
								LoggedInClients[clientIndex].SentNonce,
								&(LoggedInClients[clientIndex].ClientIp46Addr));
#ifdef __DEBUG_AUTH__
				printf("wmAUTHCheckAuthentication: ClientAccessLevel[%d]=", clientIndex);
				if (NULL != LoggedInClients[clientIndex].ClientAccessLevel)
					printf("%d.\n",LoggedInClients[clientIndex].ClientAccessLevel->read);
				else
					printf("NULL Access.\n");
#endif

			}
			else if (FALSE == wmVerifyCookie(p_envVar, &LoggedInClients[clientIndex]))
			{
#ifdef __DEBUG_AUTH__
				printf("wmAUTHCheckAuthentication: Bad cookie.\n");
#endif

				wmAuthUnlock();

				return NULL;
			}

		/* reset the timer for the current entry */
		LoggedInClients[clientIndex].LastAccessSecs = wmosTimeGet();

#ifdef __DEBUG_AUTH__
		if (NULL != LoggedInClients[clientIndex].ClientAccessLevel)
			printf("wmAUTHCheckAuthentication: Granting access (level=%d,", LoggedInClients[clientIndex].ClientAccessLevel->read);
		else
			printf("wmAUTHCheckAuthentication: Granting access (level=NULL Access,");
		printf("Nonce=%s).\n",LoggedInClients[clientIndex].SentNonce);
#endif

		retval = LoggedInClients[clientIndex].ClientAccessLevel;
		wmAuthUnlock();

		return retval;

	} /* wmAUTHCheckAuthentication */


	/*-----------------------------------------------------------------------*/

	extern sbyte *wmAUTHGetSentNonce(sbyte4 clientIndex)
	{
		sbyte *retval = "";

		if ((0 <= clientIndex) && (kHwMaxSimultaneousClients > clientIndex))
		{
			retval = LoggedInClients[clientIndex].SentNonce;
		}

		return retval;
	}



	/*-----------------------------------------------------------------------*/

	extern sbyte *wmAUTHGetClientCookie(sbyte4 clientIndex)
	{
		sbyte *retval = "";

#ifdef __DEBUG_AUTH__
		WMNET_IP_ADDR *clientIpAddr;
		char ipAddrStr [INET6_ADDRSTRLEN + 1];
#endif

		if ((0 <= clientIndex) && (kHwMaxSimultaneousClients > clientIndex))
		{

#ifdef __DEBUG_AUTH__
			clientIpAddr = &(LoggedInClients[clientIndex].ClientIp46Addr);
			inet_ntop (clientIpAddr->family, clientIpAddr->ipAddrBytes, 
					ipAddrStr, INET6_ADDRSTRLEN + 1);

			printf("wmAUTHGetClientCookie[%d]: [nonce=%s,IP=%46s]\n",
					clientIndex,
					LoggedInClients[clientIndex].SentNonce,
					ipAddrStr);
#endif

			retval = LoggedInClients[clientIndex].SentNonce;
		}

		return retval;
	}



	/*-----------------------------------------------------------------------*/

	extern void wmAUTHLogout(WMB_SESSION_T *p_envVar)
	{
		/* make sure we have a valid index */

		if ((0 <= p_envVar->clientIndex) && (kHwMaxSimultaneousClients > p_envVar->clientIndex))
		{
			/* make sure we have a valid pointer */
			if (NULL != LoggedInClients)
			{
				wmAuthLock();

#ifdef __DEBUG_AUTH__
				printf("wmAUTHLogout: [%d] '%s'='%s'.\n",
						p_envVar->clientIndex,
						LoggedInClients[p_envVar->clientIndex].SentNonce,
						LoggedInClients[p_envVar->clientIndex].pUserName);
#endif
				wmClearClientRecords(&LoggedInClients[p_envVar->clientIndex]);

				wmAuthUnlock();
			}
		}
	}

	/*-----------------------------------------------------------------------*/

	extern void wmAUTHLogoutAll(int flag)
	{

		int clientIndex;

		wmAuthLock();

		for (clientIndex = 0; clientIndex < kHwMaxSimultaneousClients; clientIndex++)
		{
			if (0 != (flag & 0x01))
			{
				if (kHwIdleTimeoutSecs > (wmosTimeGet() - LoggedInClients[clientIndex].LastAccessSecs))
					continue;
			}

			wmClearClientRecords(&LoggedInClients[clientIndex]);
		}

		wmAuthUnlock();
	}

	/* This routine come from rc\retrofit.c. "rc" was backward compability directory */
	/**************************************************************************
	 *
	 * CgiLookup - Returns the value of a name passed in the HTTP POST string
	 *
	 * NOTE: in WMW, the value pairs in BOTH the query string and the POST buffer are
	 * placed into the http environment lookup table.
	 * The standard RPM httpRpmPost must be enabled for POSTs for the CGI lookup to
	 * be populated. The query string is parsed as a matter of course.
	 *
	 * See httpParseargs() in httpRequestTranslation.c for more information.
	 *
	 * RETURNS: ERROR, OK
	 *
	 */

	IMPORT STATUS CgiLookup
	(
			WMB_SESSION_T *p_envVar,	/* The WMB session == RC Environment structure */
			char *SearchArgument,	/* name to look for */
			char *pRetBuf,			/* buffer to place value */
			unsigned long retBufLen	/* size of the passed value buffer */
	)
	{
		char * pValue = NULL;
		HTTP_REQ_ID reqId;

		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
			return wmError(ERROR_GENERAL, "invalid parameters");

		pValue = httpGetEnv(reqId,SearchArgument);

		if (NULL == pValue)
			return ERROR_GENERAL_NOT_FOUND;

		if (retBufLen <= strlen(pValue))
		{
			strncpy(pRetBuf,pValue,retBufLen);
			pRetBuf[retBufLen-1] = '\0';
		}
		else
		{
			strcpy( pRetBuf,pValue);
		}

		return OK;
	}

	/* Note: This routine come from rc\retrofit.c. "rc" was backward compability directory */
	/**************************************************************************
	 *
	 * ENVIRONMENT_GetEnvironment - Value of HTTP parameter in RC array
	 *
	 * The RC Web placed HTTP environment values in a string array. In WM Web,
	 * these values are controled by httpSetEnv() and httpGetEnv()
	 *
	 * RETURNS: string value of requested HTTP variable
	 *
	 */

	IMPORT sbyte* ENVIRONMENT_GetEnvironment
	(
			WMB_SESSION_T *p_envVar,	/* The WMB session == RC Environment structure */
			Counter      index		/* Index of requested HTTP variable */
	)
	{
		HTTP_REQ_ID reqId;

		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
		{
			wmError(ERROR_GENERAL, "invalid parameters");
			return NULL;
		}

		switch (index)
		{
		case kenv_REQUEST_METHOD :
			return( httpGetEnv(reqId,"REQUEST_METHOD") );
			break;
		case kenv_DOCUMENT_NAME :
			return( httpRpmDataGet(reqId) );
			break;
		case kenv_ACCEPT :
			return( httpGetEnv(reqId,"ACCEPT") );
			break;
		case kenv_AUTHORIZATION :
			return( httpGetEnv(reqId,"AUTH_TYPE") );
			break;
		case kenv_COOKIE :
			return( httpGetEnv(reqId,"COOKIE") ); /* MIME: "COOKIE"? */
			break;
		case kenv_CONTENT_LENGTH :
			return( httpGetEnv(reqId,"CONTENT-LENGTH") );
			break;
		case kenv_CONTENT_TYPE :
			return( httpGetEnv(reqId,"CONTENT_TYPE") );
			break;
		case kenv_CONNECTION :
			return( httpGetEnv(reqId,"CONNECTION") ); /* MIME: "CONNECTION"? */
			break;
		case kenv_HOST :
			return( httpGetEnv(reqId,"HOST") );
			break;
		case kenv_LANGUAGE :
			return( httpGetEnv(reqId,"HTTP_ACCEPT") ); /* MIME: "ACCESS-LANGUAGE"? */
			break;
		case kenv_PRAGMA :
			return( httpGetEnv(reqId,"PRAGMA") ); /* RC source? */
			break;
		case kenv_REFERER :
			return( httpGetEnv(reqId,"REFERER") ); /* RC source? */
			break;
		case kenv_USER_AGENT :
			return( httpGetEnv(reqId,"USER_AGENT") );
			break;
		case kenv_QUERY_STRING :
			return( httpGetEnv(reqId,"QUERY_STRING") );
			break;
		case kenv_HTTP_VERSION :
			if (0 == reqId->sProtocol)
				return ("HTTP/1.0");
			else if (1 == reqId->sProtocol)
				return ("HTTP/1.1");
			else if (2 == reqId->sProtocol) /* could happen */
				return ("HTTP/1.2");
			break;

			/* Add other RC mappings here as needed                   */
			/* Look for "httpSetEnv()" to find environment variables, */
			/*  particularly in httpRequestTranslation.c              */

		default:
			wmError(ERROR_GENERAL, "invalid parameters");
			break;
		}

		return NULL;
	}

	/* following is from retrofic.c */
#if 0

#ifdef __HTTP_ENABLED__
#include "httpLib.h"
#endif

	/**************************************************************************
	 *
	 * HTTP Specific Functions : these routines relate to RapidControl for Web
	 * support.
	 *
	 */

#ifdef __HTTP_ENABLED__

	/**************************************************************************
	 *
	 * WCTOOLS_ParseQueryString - Returns the value of a name passed in the HTTP
	 * Query string.
	 *
	 * See note in CgiLookup()
	 *
	 * RETURNS: ERROR, OK
	 *
	 */

	/* see evaluatePost() in httpRpmPost.c  */
	IMPORT STATUS WCTOOLS_ParseQueryString
	(
			environment *pEnv, 		/* The WMB session == RC Environment structure */
			char *SearchArgument,	/* name to look for */
			char *pReturnBuffer, 	/* buffer to place value */
			unsigned long ReturnBufferLen		/* size of the passed value buffer */
	)
	{
		return (CgiLookup(pEnv, SearchArgument, pReturnBuffer, ReturnBufferLen));
	}

	/**************************************************************************
	 *
	 * WCTOOLS_RetrieveArgument - This helper routine extracts comma delimited
	 * parameters passed in a normal-formed argument string
	 *
	 * assumes "(0,1,2)" argument structure
	 *
	 * RETURNS: ERROR, OK
	 *
	 */

#define kArgOpen  '('
#define kArgClose ')'
#define kArgDelim ','

	IMPORT RLSTATUS WCTOOLS_RetrieveArgument(
			char *pArgString,		/* source argument string */
			long    Arg,				/* index of argument requested */
			char *pArgRetBuf,		/* buffer for argument value */
			long    ArgRetBufLen		/* size of passed argument value buffer */
	)
	{
#ifdef _WRS_CONFIG_LP64
		sbyte8  stringIndex  = 0;
		sbyte8  stringLength, argWriteIndex;
#else 
		sbyte4  stringIndex  = 0;
		sbyte4  stringLength, argWriteIndex;
#endif
		Counter ArgNum = (Counter)Arg;

		if ((NULL == pArgRetBuf) || (0 > Arg))
			return ERROR_GENERAL_NO_DATA;

		*pArgRetBuf = '\0';

		if (NULL == pArgString)
			return ERROR_GENERAL_NO_DATA;

		if (0 >= (--ArgRetBufLen))
			return ERROR_GENERAL_ACCESS_DENIED;

		stringLength = (long)STRLEN(pArgString);

		/* WMB strips '(' and ')', so if no closing ')' try without */
#ifdef RC30
		/* determine length:  search for closing ')' */
		while ((0 < stringLength) && (kArgClose != pArgString[stringLength]))
			stringLength--;
#endif /* RC30 */

		if (0 == stringLength)
			return ERROR_GENERAL_NO_DATA;

		/* WMB strips '(' and ')', so if no closing ')' try without */
#ifdef RC30
		/* determine start:  skip past leading '(' */
		while (stringIndex < stringLength)
		{
			stringIndex++;

			if (kArgOpen == pArgString[stringIndex - 1])
				break;
		}
#endif /* RC30 */

/* sync to start of arg */
		while (0 < ArgNum)
		{
			while (stringIndex < stringLength)
			{
				stringIndex++;

				if (kArgDelim == pArgString[stringIndex - 1])
					break;
			}

			if (stringIndex == stringLength)
				return OK;

			ArgNum--;
		}

		argWriteIndex = 0;

		while ((stringIndex < stringLength) && (kArgDelim != pArgString[stringIndex]))
		{
			if (argWriteIndex < ArgRetBufLen)
				pArgRetBuf[argWriteIndex++] = pArgString[stringIndex++];
			else break;
		}

		pArgRetBuf[argWriteIndex] = '\0';

		return OK;

	} /* WCTOOLS_RetrieveArgument */


	/**************************************************************************
	 *
	 * RESPONSE_SendResource - this will send the content of a resource, in practice
	 * a named HTML or text file as the responce to a POST.
	 *
	 * RETURNS: ERROR, OK
	 *
	 */

	extern STATUS fileSend (WMB_SESSION_T *, generic_fd_t *);

	IMPORT RLSTATUS RESPONSE_SendResource
	(
			OS_SPECIFIC_SOCKET_HANDLE sock, /* IGNORED - WM gets the socket from the session */
			environment *p_envVar,			/* The WMB session == RC Environment structure */
			char *Resource					/* File name to be sent */
	)
	{
		STATUS       status=OK;
		HTTP_REQ_ID reqId;
		generic_fd_t f;

		/* Get the ReqId */
		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
			return wmError(ERROR_GENERAL, "invalid parameters");

		/* open a file and send it to HTTP */
		if (OK != (status = httpFileOpen(Resource,"r",&f)))
			return status;

		httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
		httpStatusSet (reqId, HTTP_OK /* HTTP_NOT_ACCEPTED */); /* Set the HTTP status code */
		httpHeaderGenerate (reqId);                				/* Send HTTP header         */

		status = fileSend (p_envVar, &f);

		httpFileClose(&f);

		return(status);
	}


	/**************************************************************************
	 *
	 * RESPONSE_SendMessage - this will send a message in the form of a Web page
	 * response to a POST.
	 *
	 * RETURNS: ERROR, OK
	 *
	 */

	IMPORT RLSTATUS RESPONSE_SendMessage
	(
			OS_SPECIFIC_SOCKET_HANDLE sock,	/* IGNORED - WM gets the socket from the session */
			environment *p_envVar,			/* The WMB session == RC Environment structure */
			char *pMessage,					/* message to be sent */
#ifdef _WRS_CONFIG_LP64
			sbyte8 messageLen				/* length of message */
#else
			sbyte4 messageLen				/* length of message */
#endif
	)
	{
		HTTP_REQ_ID reqId;

		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
			return wmError(ERROR_GENERAL, "invalid parameters");

		httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
		httpStatusSet (reqId, HTTP_OK /* HTTP_NOT_ACCEPTED */); /* Set the HTTP status code */
		httpHeaderGenerate (reqId);                				/* Send HTTP header         */

		httpPrintf(reqId, "<HTML><BODY>\n");
		httpPrintf(reqId, "<HR><H2>Message!</H2>\n");
		httpPrintf(reqId, pMessage);
		httpPrintf(reqId, "</HTML></BODY>\n");

		return(OK);
	}

	/*******************************************************************************
	 *
	 * RESPONSE_RedirectReply - direct the browser to request a named Web page in
	 * response to a POST, by redirection
	 *
	 * RETURNS: ERROR, OK
	 *
	 */

	IMPORT RLSTATUS RESPONSE_RedirectReply
	(
			environment *p_envVar,	/* The WMB session == RC Environment structure */
			char *Resource			/* File name to be sent */
	)
	{
		HTTP_REQ_ID reqId;

		sbyte *    redirUrl;

		/* Get the ReqId */
		if ((NULL == p_envVar) ||
				(NULL == (reqId = (HTTP_REQ_ID) wmbSessionStateGet(p_envVar))))
			return wmError(ERROR_GENERAL, "invalid parameters");

		/* in this version of httpPostReply, the browser is redirected to
       a different URL by means of HTTP error 303.
       Use the form 'action' field to let the server know where to redirect
       the post reply in the following way:
       if post is mapped to url substring "/urlpost/", and you want your
       redirection to go to "wmb/doc/jsform.html", set your action field to
       "/urlpost/wmb/doc/jsform.html"
		 */
		redirUrl = httpBufferGet( reqId );

		/* Make a URL local to the device */
		strcpy(redirUrl,"/");
		strcat(redirUrl,Resource);

		httpMimeContentTypeSet(reqId, HDR_OUT, "text/html");
		httpMimeHdrSet (reqId, HDR_OUT, "Location", redirUrl);
		httpStatusSet (reqId, HTTP_SEE_OTHER);/* Set the HTTP status code     */
		httpHeaderGenerate (reqId);           /* Send HTTP header             */
		httpPrintf(reqId, "<HTML><BODY>\n");
		httpPrintf(reqId, "redirected to %s\n", redirUrl);
		httpPrintf(reqId, "click <a href=\"%s\">here </a>", redirUrl);
		httpPrintf(reqId, "if your browser doesn't redirect you automatically\n");
		httpPrintf(reqId, "</BODY></HTML>\n");

		return OK;
	}

	/**************************************************************************
	 *
	 * END of HTTP Specific Functions
	 *
	 */

#endif /* __HTTP_ENABLED__ */
#endif

#endif /* __JAVASCRIPT_DIGEST_ENABLED__ */


/* sampleJSUser.c - a sample file demonstrated how to do a user database */

/* Copyright (c) 2012-2014 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
26sep14,r_w  fix static CHECKED_RETURN issue.
05aug14,lan  include the header file httpCfg.h.
05may14,r_w  fix static analysis issues(US35919)
28aug12,r_w  add support for clear text passwords
29feb12,lan  written
 */

/*
DESCRIPTION
This is a sample file demonstrated how to do a user database that the password is hashed by MD5.
A user can call wmUserDBCreate() at the WMB_COMPONENT_Start function to create the user database, 
call wmRestoreUserDB at the WMB_COMPONENT_Start function to load user's information from a file, 
and call wmAddUserInfo_Hash/wmUpdateUserInfo_Hash at the WMB_COMPONENT_Start function to 
add/modify some user's information. 
A user can call wmAddUserInfo/wmUpdateUserInfo at the kernel shell or cli shell to  add/modify a user's 
information. 
A user can call wmSaveUserDB at the WMB_COMPONENT_Stop function to save the user database into a file,
can call wmUserDBDestry at the WMB_COMPONENT_Stop function to destroy the user database.
 */

/* includes */
#include "wmw_httpconf.h"
#include "httpCfg.h"
#include "wm.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER

#include "wmb.h"
#include "wmMD5.h"
#include <hashLib.h>
#include "sampleJSUser.h"

#ifdef __JAVASCRIPT_DIGEST_ENABLED__

#define WMUSERDBMAGIC             0xCAFEBAAD
#define ACCESSINFO_INIT_VALUE     5

#define kUserDataLen    32
static int   dbUserCount = 0;
static int   userNameCount = 0;
static int   passwordCount = 0;
static int   userLevelCount = 0;
static char **dbUserName;
static char **dbPassword;
static char **dbUserLevel;

typedef struct
{
	HASH_ID hashID;
	OS_MUTEX_T  semID;
} USERINFODB;

static USERINFODB* pUserInfoDB = NULL;

typedef struct			/* NODE_STRING */
{
	HASH_NODE	node;			                     /* linked list node (must be first) */
	const char *	strUserName;			           /* hash node key */
	sbyte DigestedMessage[kDigestMesgBufSize];   /* password hash value */
	WM_ACCESS_T access;                          /* access info */
} H_NODE_USERINFO;


static int hashFunc_Userinfo
(
		int           elements,            /* number of elements in hash table        */
		H_NODE_USERINFO *   pHashNode,     /* hash node to pass through hash function */
		_Vx_usr_arg_t keyArg               /* optional argument to hash function      */
)
{
	const char* p=pHashNode->strUserName;
	int hash = 0;
	for(;*p;p++)
	{
		hash = hash * 33 + *p;
	}
	return ( hash & (elements-1) );
}

static BOOL hashKeyCmp_UserInfo 
(
		H_NODE_USERINFO *   pMatchNode,      /* hash node to match  */
		H_NODE_USERINFO *   pHashNode,       /* hash node in table being compared to */
		_Vx_usr_arg_t keyCmpArg              /* parameter passed to hashTblFind (2)  */
)
{
	return (strcmp(pMatchNode->strUserName, pHashNode->strUserName) == 0);  
}


static USERINFODB* userInfoDBCreate( )
{
	USERINFODB* pUserDB=(USERINFODB*)malloc( sizeof(USERINFODB) );
	if( pUserDB == NULL )
		return NULL;
	pUserDB->semID = semMCreate(SEM_Q_FIFO | SEM_DELETE_SAFE) ;
	if( pUserDB->semID == NULL )
	{
		free ( pUserDB );
		return NULL;
	}
	pUserDB->hashID = hashTblCreate ( 8, (FUNCPTR) hashKeyCmp_UserInfo, 
			(FUNCPTR) hashFunc_Userinfo, 0 );
	if( pUserDB->hashID == NULL )
	{
		(void)semDelete( pUserDB->semID );
		free ( pUserDB );
		return NULL;
	}
	return pUserDB;
}


static BOOL freeUserInfoNode
(
		H_NODE_USERINFO*  pNode,
		_Vx_usr_arg_t  routineArg 
)
{
	(void)hashTblRemove(pUserInfoDB->hashID, &pNode->node);
	free(pNode);
	return TRUE;
}

static BOOL showRecordInfo
(
		H_NODE_USERINFO * pNode,
		_Vx_usr_arg_t  routineArg
)
{
	printf("username=%s access right=%d %d",
			pNode->strUserName, pNode->access.read,pNode->access.write);
	printf(" password hash=");
	int i=0;
	for(;i<kDigestMesgBufSize;i++)
	{
		printf("%02x",(unsigned char)pNode->DigestedMessage[i]);
	}
	printf("\n");
	return TRUE;
}

static BOOL countHashTable
(
		H_NODE_USERINFO * pNode, 
		_Vx_usr_arg_t  routineArg
)
{
	ubyte4* pCount=( ubyte4* )routineArg ;
	++*pCount;
	return TRUE;
}

static BOOL writeRecord2File
(
		H_NODE_USERINFO * pNode,
		_Vx_usr_arg_t  routineArg
)
{
	FILE* fHandle = ( FILE* ) routineArg;
	/* write username length */
	ubyte4 nLen = (ubyte4)strlen ( pNode->strUserName ) ;
	(void) fwrite(&nLen, sizeof(nLen), 1, fHandle );
	(void) fwrite(pNode->strUserName, 1 , nLen , fHandle );
	/* write password hash */
	(void) fwrite(pNode->DigestedMessage, 1 , kDigestMesgBufSize , fHandle );
	/* write access information  */
	(void) fwrite(& pNode->access.read , sizeof(ubyte2) , 1 , fHandle );
	(void) fwrite(& pNode->access.write , sizeof(ubyte2) , 1 , fHandle );
	return TRUE;
}


static STATUS wmAddUserInfo_PasswordHash
(
		const char* pUserName,        /* the user name */
		const sbyte* pPassWordHash,   /* the user password hash*/
		const WM_ACCESS_T* pAccess   /* the user's access information */
)
{
	if( pUserName==NULL
			|| pPassWordHash==NULL 
			|| strlen(pUserName)==0  )
		return ERROR;

	STATUS sResult = ERROR ;
	size_t      length = strlen( pUserName );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*)malloc( sizeof(H_NODE_USERINFO)+ length + 1 ) ;
	if( !pNode )
		return ERROR;
	char* pUser = (char *)pNode + sizeof (H_NODE_USERINFO);
	strncpy(pUser, pUserName, length );
	pUser[length] = 0;
	pNode->strUserName = pUser;

	memcpy( pNode->DigestedMessage, pPassWordHash, kDigestMesgBufSize );

	if( pAccess == NULL )
	{
		pNode->access.read = ACCESSINFO_INIT_VALUE;
		pNode->access.write = ACCESSINFO_INIT_VALUE;
	}
	else
	{
		pNode->access.read = pAccess->read ;
		pNode->access.write = pAccess->write ;
	}
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	HASH_NODE* pNodeFind =  hashTblFind( pUserInfoDB->hashID, &pNode->node, 0 );
	/*the user not exist */
	if( ! pNodeFind )
	{
		sResult = hashTblPut( pUserInfoDB->hashID, & pNode->node );
	}
	(void)semGive( pUserInfoDB->semID );
	if( pNodeFind || sResult != OK )
	{
		free ( pNode );
	}
	return sResult;

}

static void changeHashString2Bytes(const char* pHashString, sbyte* pDigestedMessage)
{
	int i;
	int ii;
	char c;
	sbyte num ;
	for( i=0;i<kDigestMesgBufSize;i++)
	{
		for(  ii=0; ii<2; ii++ )
		{
			c=*pHashString;
			if( ( c>='0') && ( c<='9' ) )
				num = (sbyte)(c - '0');
			else if ( (c>='a') && (c<='f') )
				num = (sbyte)(c - 'a' + 10);
			else if ( (c>='A') && (c<='F') )
				num = (sbyte)(c - 'A' + 10);
			else
				num = 0;
			if( ii == 0 )
				pDigestedMessage[i]= (sbyte)(num*16);
			else
				pDigestedMessage[i] = (sbyte)(pDigestedMessage[i] + num);
			++pHashString;
		}    
	}
}

/****************************************************************************
 *
 * wmUserDBCreate - create a user information hash table
 *
 * This routine create a user information hash table if the hash table doesn't be
 * created.
 *
 * RETURNS: OK, or ERROR if failed to create the hash table
 *
 */

STATUS wmUserDBCreate()
{
	if ( pUserInfoDB == NULL )
	{
		pUserInfoDB = userInfoDBCreate( ) ;
	}
	return (pUserInfoDB == NULL)?ERROR:OK;
}

/****************************************************************************
 *
 * wmUserDBDestry - destroy the user information hash table
 *
 * This routine destroy the user information hash table.
 *
 * RETURNS: N/A
 *
 */

void wmUserDBDestry()
{
	if(pUserInfoDB)
	{
		(void)semTake(pUserInfoDB->semID,WAIT_FOREVER);
		hashTblEach(pUserInfoDB->hashID , (FUNCPTR) freeUserInfoNode, 0 );
		(void) hashTblDelete( pUserInfoDB->hashID );
		(void)semGive(pUserInfoDB->semID);
		(void)semDelete(pUserInfoDB->semID);
		free ( pUserInfoDB );
	}
	pUserInfoDB = NULL;
}

/****************************************************************************
 *
 * wmShowUserInfo - show user information at stand output
 *
 * This routine show user information at stand output.
 *
 * RETURNS: N/A
 *
 */
void  wmShowUserInfo ( )
{
	if(pUserInfoDB)
	{
		(void)semTake(pUserInfoDB->semID,WAIT_FOREVER);
		hashTblEach(pUserInfoDB->hashID , (FUNCPTR) showRecordInfo, 0 );
		(void)semGive(pUserInfoDB->semID);
	}

}

/****************************************************************************
 *
 * wmAddUserInfo - add a user information to the hash table
 *
 * This routine find whether the user name is in the hash table.
 * If the user name is in the hash table, return OK; else add it 
 * to the hash table.
 *
 * RETURNS: OK, or ERROR if failed 
 *
 */

STATUS wmAddUserInfo
(
		const sbyte* pUserName,   /* the user name */
		const sbyte* pPassWord,   /* the user password */
		const WM_ACCESS_T* pAccess  /* the user's access information */
)
{
	if( pUserInfoDB == NULL 
			||	pUserName == NULL
			|| pPassWord == NULL 
			|| strlen(pUserName)==0 
			|| strlen(pPassWord) == 0 )
		return ERROR;

	STATUS      sResult = ERROR ;
	size_t      length = strlen( pUserName );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*)malloc( sizeof(H_NODE_USERINFO)+ length + 1 ) ;
	if( !pNode )
		return ERROR;
	char* pUser = (char *)pNode + sizeof (H_NODE_USERINFO);
	strncpy(pUser, pUserName, length );
	pUser[length] = 0;
	pNode->strUserName = pUser;

	sbyte DigestedMessage[kDigestMesgBufSize];
	if (OK != wmMD5MessageDigest((sbyte*)pPassWord, strlen(pPassWord), DigestedMessage))
	{
		free( pNode );
		return ERROR;
	}
	memcpy( pNode->DigestedMessage, DigestedMessage, kDigestMesgBufSize );

	if( pAccess == NULL )
	{
		pNode->access.read  =  ACCESSINFO_INIT_VALUE;
		pNode->access.write =  ACCESSINFO_INIT_VALUE;
	}
	else
	{
		pNode->access.read = pAccess->read ;
		pNode->access.write = pAccess->write ;
	}
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	HASH_NODE* pNodeFind =  hashTblFind( pUserInfoDB->hashID, &pNode->node, 0 );
	/*the user not exist */
	if( ! pNodeFind )
	{
		sResult = hashTblPut( pUserInfoDB->hashID, & pNode->node );
	}
	(void)semGive( pUserInfoDB->semID );
	if( pNodeFind || sResult != OK )
	{
		free ( pNode );
	}
	return sResult;

}

/****************************************************************************
 *
 * wmGetUserInfo - get a user information from the hash table
 *
 * This routine find whether the user name is in the hash table.
 * If the user name is in the hash table, if the pPasswordHash is not
 *  null, copy the user password hash to the pPasswordHash; if the 
 *  pAccess is not null, copy the user access information to pAccess.  
 *
 * RETURNS: OK, or ERROR if failed 
 *
 */

STATUS wmGetUserInfo
(
		const char* pUserName,   /* a user name */ 
		sbyte* pPassWordHash,    /* where to put the user password hash */
		WM_ACCESS_T* pAccess     /* where to put the user access information */
)
{
	if( pUserInfoDB == NULL
			|| pUserName == NULL 
			|| strlen(pUserName)==0
	)
		return ERROR;

	H_NODE_USERINFO matchNode ;
	matchNode.strUserName = pUserName;
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*) hashTblFind( pUserInfoDB->hashID, & matchNode.node , 0 );
	if( pNode == NULL )
	{
		(void)semGive( pUserInfoDB->semID);
		return ERROR;
	}
	if( pPassWordHash )
		memcpy(pPassWordHash, pNode->DigestedMessage, kDigestMesgBufSize );
	if( pAccess )
	{
		pAccess->read = pNode->access.read;
		pAccess->write = pNode->access.write;
	}
	(void)semGive( pUserInfoDB->semID);
	return OK;
}

/****************************************************************************
 *
 * wmRemoveUserInfo - remove a user information from the hash table
 *
 * This routine find whether the user name is in the hash table.
 * If the user name is in the hash table, remove the user from 
 * the hash table.
 *
 * RETURNS: OK, or ERROR if don't find the user 
 *
 */

STATUS wmRemoveUserInfo
(
		const char* pUserName   /* a user name */ 
)
{
	if( pUserInfoDB == NULL
			|| pUserName==NULL 
			|| strlen(pUserName)==0
	)
		return ERROR;

	STATUS st = ERROR ;
	H_NODE_USERINFO matchNode ;
	matchNode.strUserName = pUserName;
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*) hashTblFind( pUserInfoDB->hashID, & matchNode.node , 0 );
	if( pNode )
	{
		(void)hashTblRemove(pUserInfoDB->hashID, &pNode->node);
		free( pNode );
		st = OK;
	}
	(void)semGive( pUserInfoDB->semID);
	return st;
}

/****************************************************************************
 *
 * wmUpdateUserInfo - update a user information into the hash table
 *
 * This routine find whether the user name is in the hash table.
 * If the user name is in the hash table, if the pPassword is not
 *  null, update the user password hash; if the pAccess is not null, 
 *  update the user access information.  
 *
 * RETURNS: OK, or ERROR if failed 
 *
 */

STATUS wmUpdateUserInfo
(
		const sbyte* pUserName,         /* a user name */ 
		const sbyte* pPassWord,        /*  the user password */
		const WM_ACCESS_T* pAccess     /*  the user access information */
)
{
	if( pUserInfoDB == NULL
			|| pUserName == NULL 
			|| strlen(pUserName)==0
	)
		return ERROR;

	H_NODE_USERINFO matchNode ;
	matchNode.strUserName = pUserName;
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*) hashTblFind( pUserInfoDB->hashID, & matchNode.node , 0 );
	if( pNode == NULL )
	{
		(void)semGive( pUserInfoDB->semID);
		return ERROR;
	}
	if( pPassWord )
	{
		sbyte DigestedMessage[kDigestMesgBufSize];
		if (OK != wmMD5MessageDigest((sbyte*)pPassWord, strlen(pPassWord), DigestedMessage))
		{
			(void)semGive( pUserInfoDB->semID);
			return ERROR;
		}
		memcpy( pNode->DigestedMessage, DigestedMessage, kDigestMesgBufSize );
	}
	if( pAccess )
	{
		pNode->access.read = pAccess->read ;
		pNode->access.write = pAccess->write ;
	}
	(void)semGive( pUserInfoDB->semID);
	return OK;
}

/****************************************************************************
 *
 * wmAddUserInfo_Hash - add a user information to the hash table
 *                      and the password is hash string.
 *
 * This routine find whether the user name is in the hash table.
 * If the user name is in the hash table, return ERROR; else add it 
 * to the hash table. The password hash string is like this:
 * bf114c476511e1a46b08fc05b3fbaf45, and the length is 32.
 *
 * RETURNS: OK, or ERROR if failed 
 *
 */

STATUS wmAddUserInfo_Hash
(
		const char* pUserName,                  /* a user name */ 
		const char* pPassWordHashString,        /*  the user password hash string */
		const WM_ACCESS_T* pAccess              /*  the user access information */
)
{
	/*the password hash stirng length must be 32*/
	if( pUserInfoDB == NULL 
			||	pUserName == NULL
			|| pPassWordHashString == NULL 
			|| strlen(pUserName)==0 
			|| strlen(pPassWordHashString) != kDigestMesgBufSize*2 )
		return ERROR;

	STATUS      sResult = ERROR ;
	size_t      length = strlen( pUserName );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*)malloc( sizeof(H_NODE_USERINFO)+ length + 1 ) ;
	if( !pNode )
		return ERROR;
	char* pUser = (char *)pNode + sizeof (H_NODE_USERINFO);
	strncpy(pUser, pUserName, length );
	pUser[length] = 0;
	pNode->strUserName = pUser;

	changeHashString2Bytes( pPassWordHashString, pNode->DigestedMessage);

	if( pAccess == NULL )
	{
		pNode->access.read  =  ACCESSINFO_INIT_VALUE;
		pNode->access.write =  ACCESSINFO_INIT_VALUE;
	}
	else
	{
		pNode->access.read = pAccess->read ;
		pNode->access.write = pAccess->write ;
	}
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	HASH_NODE* pNodeFind =  hashTblFind( pUserInfoDB->hashID, &pNode->node, 0 );
	/*the user not exist */
	if( ! pNodeFind )
	{
		sResult = hashTblPut( pUserInfoDB->hashID, & pNode->node );
	}
	(void)semGive( pUserInfoDB->semID );
	if( pNodeFind || sResult != OK )
	{
		free ( pNode );
	}
	return sResult;
}

/****************************************************************************
 *
 * wmUpdateUserInfo_Hash - update a user information into the hash table
 *                         and the password is hash string.
 *
 * This routine find whether the user name is in the hash table.
 * If the user name is in the hash table, if the pPasswordHash is not
 *  null, update the user password hash; if the pAccess is not null, 
 *  update the user access information.  The password hash string is like 
 *  this: bf114c476511e1a46b08fc05b3fbaf45, and the length is 32.
 *
 * RETURNS: OK, or ERROR if failed 
 *
 */

STATUS wmUpdateUserInfo_Hash
(
		const char* pUserName,             /* a user name */ 
		const sbyte* pPassWordHashString,        /*  the user password hash string */
		const WM_ACCESS_T* pAccess         /*  the user access information */
)
{
	if( pUserInfoDB == NULL
			|| pUserName == NULL 
			|| strlen(pUserName)==0
	)
		return ERROR;
	if( pPassWordHashString!=NULL 
			&& strlen(pPassWordHashString) != kDigestMesgBufSize*2  )
		return ERROR;

	H_NODE_USERINFO matchNode ;
	matchNode.strUserName = pUserName;
	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	H_NODE_USERINFO* pNode = (H_NODE_USERINFO*) hashTblFind( pUserInfoDB->hashID, & matchNode.node , 0 );
	if( pNode == NULL )
	{
		(void)semGive( pUserInfoDB->semID);
		return ERROR;
	}
	if( pPassWordHashString )
	{
		changeHashString2Bytes( pPassWordHashString, pNode->DigestedMessage);
	}
	if( pAccess )
	{
		pNode->access.read = pAccess->read ;
		pNode->access.write = pAccess->write ;
	}
	(void)semGive( pUserInfoDB->semID);
	return OK;
}

/****************************************************************************
 *
 * saveUserDB - save user information to filesystem
 *
 * This routine save user information to filesystem.   
 *
 * RETURNS: OK, or ERROR if failed
 *
 */

STATUS wmSaveUserDB(const char* file)
{
	if( pUserInfoDB == NULL
			|| file==NULL
			|| strlen(file) == 0 )
		return ERROR;
	FILE* fHandle = fopen(file, "wb");
	if( fHandle == NULL )
		return ERROR;

	(void)semTake( pUserInfoDB->semID, WAIT_FOREVER );
	/* write a magic */
	ubyte4 nMagic = WMUSERDBMAGIC ;
	(void) fwrite(&nMagic, sizeof(nMagic), 1, fHandle );
	/* write  version*/
	ubyte4 nVersion = 1 ;
	(void) fwrite(&nVersion, sizeof(nVersion), 1, fHandle );
	/* count of record */
	ubyte4 nCount=0;
	hashTblEach(pUserInfoDB->hashID , (FUNCPTR) countHashTable, (_Vx_usr_arg_t) &nCount );
	(void) fwrite(&nCount, sizeof(nCount), 1, fHandle );
	/* write records */
	hashTblEach(pUserInfoDB->hashID , (FUNCPTR) writeRecord2File, (_Vx_usr_arg_t) fHandle );
	(void)semGive( pUserInfoDB->semID );

	(void) fclose(fHandle);
	return OK;
}

/****************************************************************************
 *
 * wmRestoreUserDB - restore user information from filesystem
 *
 * This routine restore user information from filesystem.   
 *
 * RETURNS: OK, or ERROR if failed
 *
 */

STATUS wmRestoreUserDB(const char* file)
{
	size_t sRet=0;
	if( pUserInfoDB == NULL
			|| file==NULL
			|| strlen(file) == 0 )
		return ERROR;
	FILE* fHandle = fopen(file, "rb");
	if( fHandle == NULL )
		return ERROR;

	/* read a magic */
	ubyte4 nMagic = 0 ;
	if( 0==fread(&nMagic, sizeof(nMagic), 1, fHandle )
			|| nMagic!=0xCAFEBAAD )
	{
		(void) fclose(fHandle);
		return ERROR;
	}

	/* read  version*/
	ubyte4 nVersion = 0 ;
	sRet=fread(&nVersion, sizeof(nVersion), 1, fHandle );
	if(0==sRet || sRet!=1 || nVersion != 1 )
	{
		(void) fclose(fHandle);
		return ERROR;
	}

	/* count of record */
	ubyte4 nCount=0;
	sRet = fread(&nCount, sizeof(nCount), 1, fHandle );
	if(0==sRet || sRet != 1 )
	{
		(void) fclose(fHandle);
		return ERROR;
	}

	/* read records */

	int i=0;
	ubyte4 nLen = 0;
	char* pUser=NULL;
	sbyte DigestedMessage[kDigestMesgBufSize];  
	WM_ACCESS_T access; 
	for(;i<nCount;i++)
	{
		sRet = fread(&nLen, sizeof(nLen), 1, fHandle );
		if(0 == sRet || sRet != 1 || nLen < 1 )
			break;

		if(nLen < 256)
			pUser = (char*) malloc( nLen+1 );
		if( ! pUser )
			break;
		sRet = fread(pUser, 1 , nLen, fHandle );
		if(0==sRet || sRet!=nLen ) 
			break;
		pUser[nLen]=0;
		sRet = fread(DigestedMessage, 1 , kDigestMesgBufSize, fHandle );
		if(0==sRet || sRet!=kDigestMesgBufSize ) 
			break;
		sRet = fread(& access.read, sizeof(ubyte2) , 1, fHandle );
		if(0==sRet || sRet!=1 ) 
			break;
		sRet = fread(& access.write, sizeof(ubyte2) , 1, fHandle );
		if(0==sRet || sRet!=1 ) 
			break;

		(void) wmAddUserInfo_PasswordHash(pUser,DigestedMessage,&access);

		free(pUser);
		pUser=NULL;
	}
	/* the file is incorrect */
	if( i<nCount )
	{
		if( pUser )
			free(pUser);
		(void) fclose(fHandle);
		return ERROR;
	}
	(void) fclose(fHandle);
	return OK;
}



static WM_ACCESS_T gAccess;

/****************************************************************************
 *
 * allocateMemory - allocate memory for user profiles
 * 
 *
 * RETURNS: void
 *
 */
void allocateMemory()
{
	int userCount;
	/*allocate memory for username , password and access */

	dbUserName = (char **) malloc(sizeof(char*) * dbUserCount);
	dbPassword = (char **) malloc(sizeof(char*) * dbUserCount);
	dbUserLevel = (char **) malloc(sizeof(char*) * dbUserCount);

	for (userCount = 0; userCount < dbUserCount; userCount++)
	{
		dbUserName[userCount] = (char *) malloc(sizeof(char) * kUserDataLen);
		dbPassword[userCount] = (char *) malloc(sizeof(char) * kUserDataLen);
		dbUserLevel[userCount] = (char *) malloc(sizeof(char) * kUserDataLen);
	}
}

/****************************************************************************
 *
 * freeMemory - free memory allocated for user profiles
 * 
 *
 * RETURNS: void
 *
 */
void freeMemory()
{
	int userCount;

	/*free allocated memory for username , password and access */

	for (userCount = 0; userCount < dbUserCount; userCount++)
	{
		free(dbUserName[userCount]);
		free(dbPassword[userCount]);
		free(dbUserLevel[userCount]);
	}
	userNameCount = 0;
	passwordCount = 0;
	userLevelCount = 0;
	dbUserCount = 0;
	free(dbUserName);
	free(dbPassword);
	free(dbUserLevel);
}

extern void wmStoreUserData(char *chBuffer)
{
	int dwLength = strlen(chBuffer);
	int i=0, dwCnt=1;
	char *chTempStart = chBuffer;
	char *chTempEnd = chBuffer;
	for(i=0; i<dwLength; i++)
	{
		if(chBuffer[i] == ',')
			dwCnt++;
	}
	if(dwCnt <= 0 && dwCnt%3 != 0)
	{
		return;
	}

	freeMemory();
	dbUserCount = dwCnt/3;
	allocateMemory();
	printf("\n Actual Buffer::%s",chBuffer); // ADMIN,123456Aa@,Eecutive
	for(i=0; i<dbUserCount; i++)
	{
		chTempEnd = strstr(chTempStart,",");	
		memcpy(dbUserName[userNameCount], chTempStart,chTempEnd - chTempStart);
		dbUserName[userNameCount][chTempEnd - chTempStart] = 0;
		userNameCount++;
		printf("\nwmStoreUserData ::%d::%s",chTempEnd - chTempStart,dbUserName[userNameCount-1]);

		chTempStart = chTempEnd + 1;
		chTempEnd = strstr(chTempStart,",");	
		memcpy(dbPassword[passwordCount], chTempStart,chTempEnd - chTempStart);
		dbPassword[passwordCount][chTempEnd - chTempStart] = 0;
		passwordCount++;
		printf("\nwmStoreUserData ::%d::%s",chTempEnd - chTempStart,dbPassword[passwordCount-1]);

		chTempStart = chTempEnd + 1;
		chTempEnd = strstr(chTempStart,",");	
		if(chTempEnd == NULL)
			chTempEnd = chBuffer+dwLength;
		memcpy(dbUserLevel[userLevelCount], chTempStart,chTempEnd - chTempStart);
		dbUserLevel[userLevelCount][chTempEnd - chTempStart] = 0;
		userLevelCount++;
		printf("\nwmStoreUserData ::%d::%s",chTempEnd - chTempStart,dbUserLevel[userLevelCount-1]);
		printf("\nwmStoreUserData %s::%s::%s",dbUserName[userNameCount-1],dbPassword[passwordCount-1],dbUserLevel[userLevelCount-1]);

		chTempStart = chTempEnd + 1;
		if(userNameCount >= dbUserCount)
			break;
	}
}

/****************************************************************************
 * 
 * USER_AUTH_AuthLevel_EX - a JavaScript authentication routine with password hash
 *
 * This routine get the password hash and read/write access information of a user.
 *
 * RETURNS: the pointer of WM_ACCESS_T(OK), or null
 *
 * ERRNO: N/A
 *
 * Note: wmGetAuthenticationLevelPassword()is a user supplied function which will 
 * be used in http_init() of httpInit.c and will be called by wmVerifyUser() in httpAuth.c.
 * If using USER_AUTH_AuthLevel_EX, USER_AUTH_AuthLevel_EX should
 * be changed name to wmGetAuthenticationLevelPassword.
 */
WM_ACCESS_T* USER_AUTH_AuthLevel_EX(WMB_SESSION_T *envVar, char *pUserName, char *pPasswordHash)
{

	/*
	 * an example of JavaScript authentication routine
	 */
	if( pUserInfoDB == NULL )
	{
		if( OK != wmUserDBCreate() )
			return NULL;
		(void) wmRestoreUserDB("/hostfs/ws/sampleuser.dat");
		(void) wmAddUserInfo("admin","admin",0);
		(void) wmAddUserInfo_Hash("wrs","bf114c476511e1a46b08fc05b3fbaf45",0);  /*the password is wrs */
	}

	if( OK != wmGetUserInfo ( pUserName, pPasswordHash, &gAccess ) )
	{
		return NULL;
	}
	pPasswordHash[16]=0;
	return &gAccess ;
}

#if 1
/*******************************************************************************************
 * Following routines demonstrate how to do a user database that the password is clear text.
 */

static WM_ACCESS_T g_sampleUsrAccess[3]; 

/*******************************************************************************************
 *
 * User routine: returns OK if user name and password are in user database, else
 * returns ERROR 
 */
static STATUS VD_AUTH_GetPassword(sbyte *pUsername, sbyte *pPassword)
{
	int userCount;
	for(userCount = 0 ; userCount < dbUserCount; userCount++)
	{
		if ((0 == strcmp(pUsername, dbUserName[userCount] ))
				&& (0 == strcmp(dbUserLevel[userCount], "Executive")))
		{
			strcpy(pPassword, dbPassword[userCount]);
			return OK;
		}
	}

	return ERROR;
}

/*******************************************************************************************
 *
 * User routine: returns pointer to access record for this user name, else NULL
 * if not found 
 *
 */
static WM_ACCESS_T* VD_AUTH_GetAccessLevel(sbyte *pUsername)
{
	int userCount;
	for(userCount = 0; userCount < dbUserCount; userCount++)
	{
		if ((0 == strcmp(pUsername, dbUserName[userCount] ))
				&& (0 == strcmp(dbUserLevel[userCount], "Executive")))
		{
			g_sampleUsrAccess[0].read = 6;
			g_sampleUsrAccess[0].write = 6;
			return &g_sampleUsrAccess[0];
		}
	}

	return NULL;
}

/*******************************************************************************************
 *
 * User routine: returns pointer to access record for this user name, else NULL if not found 
 * Note: wmGetAuthenticationLevelPassword()is a user supplied function which will be used in 
 * http_init() of httpInit.c and will be called by wmVerifyUser() in httpAuth.c. 
 * This is an example. You must modify this routine to
 * customize the functionality (behavior).
 */
extern WM_ACCESS_T*
wmGetAuthenticationLevelPassword(WMB_SESSION_T *envVar, char *pUserName, char *pPassword)
{
	WM_ACCESS_T *pAccessLevel = NULL;
	STATUS status;

	/* Test the user name and password, return NULL access pointer if no match */

	status = VD_AUTH_GetPassword( pUserName, pPassword );
	if ( OK > status )
		return pAccessLevel;

	/* Return pointer to access record for this validated user */

	pAccessLevel = VD_AUTH_GetAccessLevel( pUserName );

	return pAccessLevel;
}
#endif

#if 0
extern WM_ACCESS_T*
USER_AUTH_AuthLevel(WMB_SESSION_T *envVar, sbyte *pUserName, sbyte *pPassword)
{
	WM_ACCESS_T *pAccess;
	pAccess = (WM_ACCESS_T*) wmMalloc(sizeof(WM_ACCESS_T));
	memset (pAccess,0,sizeof(WM_ACCESS_T));
	if (0 == STRCMP( pUserName, "admin" ))
	{
		STRCPY( pPassword, "admin" );
		pAccess->read  = 3;
		pAccess->write = 3;
		return pAccess;
	}
	else if (0 == STRCMP( pUserName, "guest1" ))
	{
		STRCPY( pPassword, "guest1" );
		pAccess->read  = 3;
		pAccess->write = 0;
		return pAccess;
	}
	else if (0 == STRCMP( pUserName, "guest" ))
	{
		STRCPY( pPassword, "guest" );
		pAccess->read  = 1;
		pAccess->write = 0;
		return pAccess;
	}
	else return pAccess;
}
#endif
#endif
#endif

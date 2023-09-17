/* sampleJSUser.h - a sample file demonstrated how to do a user database */

/* Copyright (c) 2012 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,29feb12,lan   written
*/

/*
DESCRIPTION
This is a sample file demonstrated how to do a user database that the password is hashed by MD5.
*/

/* includes */

#if !defined(_SAMPLE_JS_USER_H_)
#define _SAMPLE_JS_USER_H_

#include "wmData.h"

#ifdef __cplusplus
extern "C" {
#endif

STATUS wmUserDBCreate();
void   wmUserDBDestry();
void   wmShowUserInfo( );
STATUS wmAddUserInfo(const char* pUserName, const char* pPassWord, const WM_ACCESS_T* pAccess );
STATUS wmGetUserInfo(const char* pUserName, sbyte* pPassWordHash, WM_ACCESS_T* pAccess );
STATUS wmRemoveUserInfo(const char* pUserName );
STATUS wmSaveUserDB(const char* file);
STATUS wmUpdateUserInfo(const char* pUserName, const char* pPassWord, const WM_ACCESS_T* pAccess );
STATUS wmRestoreUserDB(const char* file);
STATUS wmAddUserInfo_Hash(const char* pUserName, const char* pPassWordHashString, const WM_ACCESS_T* pAccess );
STATUS wmUpdateUserInfo_Hash(const char* pUserName, const char* pPassWordHashString, const WM_ACCESS_T* pAccess );

#ifdef __cplusplus
}
#endif

#endif

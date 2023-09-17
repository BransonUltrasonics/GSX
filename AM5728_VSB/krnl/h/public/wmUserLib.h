/* wmUserLib.h - WindManage user library header */

/*
 * Copyright (c) 2004, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,22nov12,f_f  added the definition of userInfo here (REQ: WIND00364534)
01b,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmbUserLibh
#define __INCwmbUserLibh

#ifndef USER_LOGIN_LEN_K
#  ifdef  kRCC_MAX_LOGIN_LEN
#    define USER_LOGIN_LEN_K        kRCC_MAX_LOGIN_LEN
#  else
#    define USER_LOGIN_LEN_K        16
#  endif
#endif

#ifndef USER_PASSWORD_LEN_K
#  ifdef  kRCC_MAX_PASSWORD_LEN
#    define USER_PASSWORD_LEN_K     kRCC_MAX_PASSWORD_LEN
#  else
#    define USER_PASSWORD_LEN_K     16
#  endif
#endif

#ifndef USER_LOGIN_LEN_K
#define USER_LOGIN_LEN_K        16
#endif

#ifndef USER_PASSWORD_LEN_K
#define USER_PASSWORD_LEN_K     16
#endif

#ifndef MAX_USERS_K
#define MAX_USERS_K             20
#endif

#ifdef  kRCC_DEFAULT_LOGIN
#define DEFAULT_LOGIN_K         kRCC_DEFAULT_LOGIN
#else
#define DEFAULT_LOGIN_K         "admin"
#endif

#ifdef  kRCC_DEFAULT_PASSWORD
#define DEFAULT_PASSWORD_K      kRCC_DEFAULT_PASSWORD
#else
#define DEFAULT_PASSWORD_K      "admin"
#endif

#ifdef kRCC_DEFAULT_ACCESS
#define DEFAULT_ACCESS_K        kRCC_DEFAULT_ACCESS
#else
#define DEFAULT_ACCESS_K        "10"
#endif

typedef struct userInfo
    {
    sbyte       login[USER_LOGIN_LEN_K];
    sbyte       password[USER_PASSWORD_LEN_K];
    Access      accessLevel;
    } userInfo;

#ifdef __cplusplus
extern "C" {
#endif

BOOL    wmUserValidate (OS_SOCKET_T sock, sbyte * pLogin,
                        sbyte * pPassword, Access * pAccess);
STATUS  wmUserAdd (sbyte * pLogin, sbyte * pAccess);
STATUS  wmUserDelete (sbyte *pLogin);
STATUS  wmUserChangePassword (sbyte * pLogin, sbyte * pPassword);
STATUS  wmUserChangeAccess (sbyte * pLogin, sbyte * pAccess);
void    wmUserShowUsers (void);

#ifdef __cplusplus
}
#endif

#endif /* __INCwmUserLibh */

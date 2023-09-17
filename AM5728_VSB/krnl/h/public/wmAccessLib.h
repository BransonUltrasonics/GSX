/* wmAccessLib.h - RCB access library header */

/* Copyright 2004-2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,09feb10,m_z  group match macro logic issue fix
                 CQID: Fix WIND00199475
01c,19jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01b,13sep04,dkg  added new API's to set & get user access level to support
                 JavaScript Authentication feature
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCwmAccessLibh
#define __INCwmAccessLibh

/* temporary fix for no options.h */

#ifndef __RLI_ACCESS_LEVEL_MASK__
#define __RLI_ACCESS_LEVEL_MASK__ 0x0000ffff
#endif

#ifndef __RLI_ACCESS_OPTION_MASK__
#define __RLI_ACCESS_OPTION_MASK__ 0xffff0000
#endif


#ifdef __RLI_ACCESS_LEVEL_MASK__
#define ACCESS_LEVEL_MASK_K   __RLI_ACCESS_LEVEL_MASK__
#else
#define ACCESS_LEVEL_MASK_K   0
#endif

#ifdef __RLI_ACCESS_OPTION_MASK__
#define ACCESS_GROUP_MASK_K   __RLI_ACCESS_OPTION_MASK__
#else
#define ACCESS_GROUP_MASK_K   0
#endif


#define accessLevel(x)  (x & ACCESS_LEVEL_MASK_K)
#define accessGroup(x)  (x & ACCESS_GROUP_MASK_K)

#if (ACCESS_LEVEL_MASK_K != 0)
#  define wmAccessInvalidLevel(x, y)  (accessLevel(x) > accessLevel(y))
#else
#  define wmAccessInvalidLevel(x,y)   (x > y)
#endif

#if (ACCESS_GROUP_MASK_K != 0)
#  define wmAccessInvalidGroup(x, y)  (0 == accessGroup(x) ? FALSE : 0 == (accessGroup(x) & accessGroup(y)))
#else
#  define wmAccessInvalidGroup(x,y)   FALSE
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Access validation */

DTTypeInfo * wmAccessInfoGet (void);
void    wmAccessInfoSet (DTTypeInfo * pInfo);
BOOL    wmAccessValidIpAddress (ubyte4 ClientIpAddr);
ubyte4 wmAccessLevelGet ( void * what);
ubyte4 wmAccessGroupsGet ( void * what);
  
#ifndef __WMB_VIEW_ACCESS_MODEL__

/* for cli situation -- readable only */

BOOL    wmAccessReadOkay (Access user, Access resource);
BOOL    wmAccessReadAllowed (Access user, Access resource);
BOOL    wmAccessWriteAllowed (Access user, Access resource);

#endif

STATUS  wmAccessSetAccess (WM_ACCESS_T user, sbyte * pAccessStr);

STATUS  wmAccessToStr (Access nAccess, sbyte *pString);
STATUS  wmAccessFromString (sbyte * pString, Access * pAccess);

/* User access level set & get APIs */

Access wmAccessReadGet  ( WM_ACCESS_T pAccess );
Access wmAccessWriteGet ( WM_ACCESS_T pAccess );
STATUS wmAccessReadSet  ( WM_ACCESS_T *pAccess, Access readAccess );
STATUS wmAccessWriteSet ( WM_ACCESS_T *pAccess, Access writeAccess );

#ifdef __cplusplus
}
#endif

#endif /* __INCwmAccessLibh */

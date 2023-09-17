/* namingScopes.h - register naming scopes functions */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/*
modification history
-------------------
01d,19apr05,job  update copyright notices
01c,25nov03,ps  copyright update
01b,18mar03,ism  removed conditional prototypes after code review
01a,05dec02,ism  written
*/

#ifndef __INCnamingScopesh
#define __INCnamingScopesh

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* Defines used for viewType field */

#define VIEW_TYPE_GET 0x01
#define VIEW_TYPE_SET 0x02

#if defined(__STDC__) || defined(__cplusplus)

extern void communityStringInit (bits32_t defaultVsNum);

extern STATUS communityStringRegister (bits8_t * newCommunityString, 
                                       bits32_t  stringLen, 
                                       bits8_t   viewType,
                                       bits8_t * viewName, 
                                       bits32_t  nameLen, 
                                       bits32_t  vsNum);

extern STATUS communityStringDeregister (bits8_t * communityString, 
                                         bits32_t  stringLen,
                                         bits8_t   viewType,
                                         bits32_t  vsNum);

extern bits32_t communityStringFind (EBUFFER_T   searchCommunity,
                                     bits8_t     viewType,
                                     EBUFFER_T * viewName);


#if INSTALL_ENVOY_SNMP_VERSION_3

extern void contextNameInit (bits32_t defaultVsNum);

extern STATUS contextNameRegister (bits8_t * newContextName, 
                                   bits32_t  stringLen, 
                                   bits8_t   viewType,
                                   bits8_t * viewName, 
                                   bits32_t  nameLen, 
                                   bits32_t  vsNum);

extern STATUS contextNameDeregister (bits8_t * contextName, 
                                     bits32_t  stringLen,
                                     bits8_t   viewType,
                                     bits32_t  vsNum);

extern bits32_t contextNameFind (EBUFFER_T   searchContext,
                                 bits8_t     viewType,
                                 EBUFFER_T * viewName);

#endif /* INSTALL_ENVOY_SNMP_VERSION_3 */

#else   /* __STDC__ */

extern void communityStringInit ();
extern STATUS communityStringRegister ();
extern STATUS communityStringDeregister ();
extern bits32_t communityStringFind ();

#if INSTALL_ENVOY_SNMP_VERSION_3

extern void contextNameInit ();
extern STATUS contextNameRegister ();
extern STATUS contextNameDeregister ();
extern bits32_t contextNameFind ();

#endif /* INSTALL_ENVOY_SNMP_VERSION_3 */

#endif  /* __STDC__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCnamingScopesh */

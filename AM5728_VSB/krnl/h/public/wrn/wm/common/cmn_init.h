/* $Header: /usr/cvsroot/target/h/wrn/wm/common/cmn_init.h,v 1.3 2003/01/16 18:20:11 josh Exp $ */


/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/* 
 * $Log: cmn_init.h,v $
 * Revision 1.3  2003/01/16 18:20:11  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 00:07:09  meister
 * Rework pathnames again
 *
 * Revision 1.1.1.1  2001/11/05 17:47:18  tneale
 * Tornado shuffle
 *
 * Revision 1.10  2001/01/19 22:21:27  paul
 * Update copyright.
 *
 * Revision 1.9  2000/03/17 00:16:35  meister
 * Update copyright message
 *
 * Revision 1.8  2000/03/09 19:09:22  josh
 * changes so we don't shoot ourselves in the foot if we're building
 * Emissary or Liaison
 *
 * Revision 1.7  1998/09/11 16:30:40  paul
 * Auto-initialize common library.
 *
 * Revision 1.6  1998/06/19 20:46:29  sra
 * Minor cleanups to crypto code installation options.
 *
 * Revision 1.5  1998/02/25 04:43:25  sra
 * Update copyrights.
 *
 * Revision 1.4  1997/10/30 16:44:29  sra
 * Fix copyright date.
 *
 * Revision 1.3  1997/10/15 19:45:14  sra
 * Cleanup.
 *
 * Revision 1.2  1997/08/14 16:04:37  lowell
 * boilerplate
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif

extern void etc_common_init(void);	/* init the common library */

/*
 * The purpose of the following incantations is to make sure that exactly
 * one Epilogue product initializes the common library.  The decision of
 * which product should do the initialization is made at compile time,
 * according to the following if/else-if/else chain.   The intent is to
 * keep all of the control of how this happens localized in one place.
 */

#if	defined(INSTALL_ATTACHE) && INSTALL_ATTACHE
#define	COMMON_INIT_FROM_ATTACHE()	etc_common_init()
#elif 	defined(INSTALL_ENVOY)   && INSTALL_ENVOY
#define	COMMON_INIT_FROM_ENVOY()	etc_common_init()
#elif 	defined(INSTALL_COURIER) && INSTALL_COURIER
#define	COMMON_INIT_FROM_COURIER()	etc_common_init()
#elif	defined(INSTALL_DECORUM) && INSTALL_DECORUM
#define	COMMON_INIT_FROM_DECORUM()	etc_common_init()
#elif   defined(INSTALL_LIAISON) && INSTALL_LIAISON
#define COMMON_INIT_FROM_LIAISON()      etc_common_init()
#elif   defined(INSTALL_EMISSARY) && INSTALL_EMISSARY
/* We don't care in this case... */
#else
#error	Internal error! Nobody calls etc_common_init()!
#endif

#ifndef COMMON_INIT_FROM_ATTACHE
#define COMMON_INIT_FROM_ATTACHE()
#endif

#ifndef COMMON_INIT_FROM_ENVOY
#define COMMON_INIT_FROM_ENVOY()
#endif

#ifndef COMMON_INIT_FROM_COURIER
#define COMMON_INIT_FROM_COURIER()
#endif

#ifndef COMMON_INIT_FROM_DECORUM
#define COMMON_INIT_FROM_DECORUM()
#endif

#ifndef COMMON_INIT_FROM_LIAISON
#define COMMON_INIT_FROM_LIAISON()
#endif



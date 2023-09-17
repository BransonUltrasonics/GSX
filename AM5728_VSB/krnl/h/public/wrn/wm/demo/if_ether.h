/* $Header: /usr/cvsroot/target/h/wrn/wm/demo/if_ether.h,v 1.3 2003/01/15 13:58:30 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1993-1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: if_ether.h,v $
 * Revision 1.3  2003/01/15 13:58:30  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:27:44  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:32  tneale
 * Tornado shuffle
 *
 * Revision 1.2  2001/01/19 22:23:34  paul
 * Update copyright.
 *
 * Revision 1.1  2000/07/10 15:55:15  mrf
 * Add configuration for Cygnus NT environment with associated changes.
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


/* Include file to include netinet/if_ether.h on machines that
 * have it, not on machines that don't. 
 */

#include <wrn/wm/common/config.h>

#if (!INSTALL_on_ntgcc)
#include <netinet/if_ether.h>
#endif





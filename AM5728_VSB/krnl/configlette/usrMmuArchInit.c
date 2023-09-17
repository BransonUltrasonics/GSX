/* usrMmuArchInit.c - memory management unit initialization */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
25sep13,pcs  Moved the arch specific code from usrMmuInit to here.
*/

/*
DESCRIPTION
This file is used to initialize the memory management unit.  This file is
included by usrMmuInit.c.

SEE ALSO: usrMmuInit.c

NOMANUAL
*/

#ifndef  __INCusrMmuArchInitc
#define  __INCusrMmuArchInitc

/* externals */

IMPORT FUNCPTR sysMmuLibInit;

/*******************************************************************************
*
* usrMmuArchInit - initialize the memory management unit
*
* This routine calls the architecture MMU library initialization routine.
*
* RETURNS: OK, or ERROR if the MMU library initialization failed.
*
* NOMANUAL
*/

STATUS usrMmuArchInit (void)
    {
    if ((sysMmuLibInit == NULL) || (((* sysMmuLibInit) (VM_PAGE_SIZE)) != OK))
	return (ERROR);

    return (OK);
    }

#endif	/* __INCusrMmuArchInitc */

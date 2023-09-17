/* 21shell.cdf - Kernel shell components for ARM */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
31Aug05,pee  fixed type. Space after backslash
17mar05,asa  written

DESCRIPTION

This file contains updated descriptions for some Kernel shell components
defined in 01shell.cdf. It overwrites those definitions with empty ones
as ARM architecture does not support the coprocessor abstraction.
*/

Component INCLUDE_HW_FP_MREGS {
	NAME		Overwrite the floating point registers modification
	SYNOPSIS	Overwrite the floating point register modification module \
                        as ARM does not support the coprocessor abstraction
	MODULES		
	INCLUDE_WHEN	INCLUDE_HW_FP_SHOW \
			INCLUDE_SHELL
	REQUIRES	INCLUDE_HW_FP_SHOW \
			INCLUDE_SHELL
	_CHILDREN 	FOLDER_NOT_VISIBLE
}


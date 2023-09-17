/* sampleCheckbox.h - sample implementation for tutorial */

/* Copyright (c) 2012 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,09aug12,r_w  create file
*/

#ifndef __SAMPLE_CHECK_BOX_H__
#define __SAMPLE_CHECK_BOX_H__

extern sbyte   myCheckbox[32];

STATUS checkboxHandler
     (
     WMB_TID_T  tid,     /* wmb transaction pointer */
     WMB_OBJ_T  obj,     /* wmb object      pointer */
     COMMAND_T  command  /* transaction type (e.g. GET,SET) */
    );

#endif /* __SAMPLE_CHECK_BOX_H__ */


/* eof sampleCheckbox.h */

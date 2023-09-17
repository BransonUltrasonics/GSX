// $Header:   D:/databases/VMdb/archives/EN13849/String.c_v   1.7   20 Jul 2011 16:59:16   ASharma6  $
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

This is the module for returning length of the string, not counting
terminating zero.

Module name: String

Filename:    String.c

--------------------------- TECHNICAL NOTES -------------------------------

The code is for calculating length of string.

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/String.c_v  $
 * 
 *    Rev 1.7   20 Jul 2011 16:59:16   ASharma6
 * Modifications for review comments
 * 
 *    Rev 1.6   05 Jul 2011 11:01:24   ASharma6
 * ULS, CM changes
 * 
 *    Rev 1.3   22 Jun 2011 18:11:34   ASharma6
 * LE Fault reset allowed in horn up state
 * Part contact lost made more resilient
 * Cross monitoring detects other processor was reset and resets itself
 * SBeam diagnostics is more resilient
 * 
 *    Rev 1.2   31 Mar 2011 12:14:08   PDwivedi
 * Fixes after review.
 * 
 *    Rev 1.1   25 Mar 2011 09:10:24   ygupta
 * Initial Version with File and Function Headers. 
-------------------------------------------------------------------------*/
#include "Global.h"

/**************************************************************************//**
* \brief -       strlen
*
* \brief -		Returning length of the string, not counting
* 				terminating zero.
*
* \param -		str The string.
* 
* \return -	length of string.
*
*****************************************************************************/
tSINT32 strlen(const tSINT8 *str)
{
	tSINT32 len = 0;
    if (str != 0)
    {
        while (*str++ != '\0')
            ++len;
    }
    return len;
}


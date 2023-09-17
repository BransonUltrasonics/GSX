// $Header:   D:/databases/VMdb/archives/EN13849/app/TestMacros.h_v   1.2   01 Apr 2011 11:20:32   PDwivedi  $
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
/* ------------------------- MODULE DESCRIPTION ------------------------------
 This is the application include file.

 Module name: TestMacros
 File name: TestMacros.h
 -------------------------- TECHNICAL NOTES --------------------------------

------------------------------ REVISIONS ---------------------------------
$Log:   D:/databases/VMdb/archives/EN13849/app/TestMacros.h_v  $
 * 
 *    Rev 1.2   01 Apr 2011 11:20:32   PDwivedi
 * Fixes after code review.
 * 
 *    Rev 1.1   25 Mar 2011 09:14:10   ygupta
 * Initial Version With File and Function Headers.
-------------------------------------------------------------------------*/


#ifndef TESTMACROS_H_
#define TESTMACROS_H_

#define UNIT_TESTING 0

// All these macros expand to no code in application build, and to executable
// code during unit testing. Refer to TestMacros.h in uut folder for
// documentation.

#define UT_VARIABLE(x)

#define CHECKPOINT(var,number) do { } while(0)

#define FUNCTION(var,format,...)

#define FUNCTION1(var,format)

#define FAULT(var,code)

#endif

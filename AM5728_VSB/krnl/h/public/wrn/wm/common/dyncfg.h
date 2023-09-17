/* $Header: /usr/cvsroot/target/h/wrn/wm/common/dyncfg.h,v 1.2 2003/01/16 18:20:11 josh Exp $ */

/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1988-1997 Epilogue Technology Corporation.
 *  All rights reserved.
 ****************************************************************************/

/*
 * $Log: dyncfg.h,v $
 * Revision 1.2  2003/01/16 18:20:11  josh
 * directory structure shifting
 *
 */

/* [clearcase]
modification history
-------------------
01a,20apr05,job  update copyright notices
*/


#ifndef COMMON_DYNCFG_H
# define COMMON_DYNCFG_H 1

/*
 * see example of usage at end of file
 */

#if INSTALL_COMMON_DYNAMIC_COMP_CONFIG

/*
 * macros for declaring the function pointers which will be used by 
 * the dynamic component code. Each is of the form 
 * DYNCFG_FUNCPTR<n_args>(<function_name>,<return_type>,<argtype>,<argtype>...)
 * 
 * Variants for declaring it to be global, static to file, or extern.
 * Current code which references a function which may be in a component which 
 * may not be loaded, must be changed to call the function via a pointer 
 * dereference, the pointer having been declared via one of these macros.
 * 
 * These macros evaluate to nothing if INSTALL_COMMON_DYNAMIC_COMP_CONFIG
 * is turned off.
 */

#define DYNCFG_FUNCPTR0(X,rval) \
	rval (*DYN_PTR_##X) ();
#define DYNCFG_STATIC_FUNCPTR0(X,rval) \
	static rval (*DYN_PTR_##X) ();
#define DYNCFG_EXTERN_FUNCPTR0(X,rval) \
	extern rval (*DYN_PTR_##X) ();

#define DYNCFG_FUNCPTR1(X,rval,a1) \
	rval (*DYN_PTR_##X) (a1);
#define DYNCFG_STATIC_FUNCPTR1(X,rval,a1) \
	static rval (*DYN_PTR_##X) (a1);
#define DYNCFG_EXTERN_FUNCPTR1(X,rval,a1) \
	extern rval (*DYN_PTR_##X) (a1);

#define DYNCFG_FUNCPTR2(X,rval,a1,a2) \
	rval (*DYN_PTR_##X) (a1, a2);
#define DYNCFG_STATIC_FUNCPTR2(X,rval,a1,a2) \
	static rval (*DYN_PTR_##X) (a1, a2);
#define DYNCFG_EXTERN_FUNCPTR2(X,rval,a1,a2) \
	extern rval (*DYN_PTR_##X) (a1, a2);

#define DYNCFG_FUNCPTR3(X,rval,a1,a2,a3) \
	rval (*DYN_PTR_##X) (a1, a2, a3);
#define DYNCFG_STATIC_FUNCPTR3(X,rval,a1,a2,a3) \
	static rval (*DYN_PTR_##X) (a1, a2, a3);
#define DYNCFG_EXTERN_FUNCPTR3(X,rval,a1,a2,a3) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3);

#define DYNCFG_FUNCPTR4(X,rval,a1,a2,a3, a4) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4);
#define DYNCFG_STATIC_FUNCPTR4(X,rval,a1,a2,a3,a4) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4);
#define DYNCFG_EXTERN_FUNCPTR4(X,rval,a1,a2,a3,a4) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4);

#define DYNCFG_FUNCPTR5(X,rval,a1,a2,a3,a4,a5) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5);
#define DYNCFG_STATIC_FUNCPTR5(X,rval,a1,a2,a3,a4,a5) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5);
#define DYNCFG_EXTERN_FUNCPTR5(X,rval,a1,a2,a3,a4,a5) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5);

#define DYNCFG_FUNCPTR6(X,rval,a1,a2,a3,a4,a5,a6) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6);
#define DYNCFG_STATIC_FUNCPTR6(X,rval,a1,a2,a3,a4,a5,a6) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6);
#define DYNCFG_EXTERN_FUNCPTR6(X,rval,a1,a2,a3,a4,a5,a6) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6);

#define DYNCFG_FUNCPTR7(X,rval,a1,a2,a3,a4,a5,a6,a7) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7);
#define DYNCFG_STATIC_FUNCPTR7(X,rval,a1,a2,a3,a4,a5,a6,a7) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7);
#define DYNCFG_EXTERN_FUNCPTR7(X,rval,a1,a2,a3,a4,a5,a6,a7) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7);

#define DYNCFG_FUNCPTR8(X,rval,a1,a2,a3,a4,a5,a6,a7,a8) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8);
#define DYNCFG_STATIC_FUNCPTR8(X,rval,a1,a2,a3,a4,a5,a6,a7,a8) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8);
#define DYNCFG_EXTERN_FUNCPTR8(X,rval,a1,a2,a3,a4,a5,a6,a7,a8) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8);

#define DYNCFG_FUNCPTR9(X,rval,a1,a2,a3,a4,a5,a6,a7,a8,a9) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8, a9);
#define DYNCFG_STATIC_FUNCPTR9(X,rval,a1,a2,a3,a4,a5,a6,a7,a8,a9) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8, a9);
#define DYNCFG_EXTERN_FUNCPTR9(X,rval,a1,a2,a3,a4,a5,a6,a7,a8,a9) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8, a9);

#define DYNCFG_FUNCPTR10(X, rval, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
	rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
#define DYNCFG_STATIC_FUNCPTR10(X,rval,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
	static rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
#define DYNCFG_EXTERN_FUNCPTR10(X,rval,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
	extern rval (*DYN_PTR_##X) (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);

/*
 * This macro provides the mechanism for calling a function which may be in a
 * non-installed component. If INSTALL_COMMON_DYNAMIC_COMP_CONFIG is not 
 * installed, it evaluates to a normal function call, calling X in the usual 
 * way.
 */

#define DYNCFG_FUNCALL(X) (*DYN_PTR_##X)

/* 
 * This macro returns the actual function pointer itself. 
 */

#define DYNCFG_FUNCPTR(X) DYN_PTR_##X

/*
 * this macro sets a previously declared (by DYNCFG_*_FUNCPTR?()) to its 
 * most likely default, the name of the function we named it after.
 */

#define DYNCFG_FUNCPTR_SET_DEFAULT(X) DYN_PTR_##X = X

/*
 * this macro sets a previously declared (by DYNCFG_*_FUNCPTR?()) to 
 * something other than the probable default
 */

#define DYNCFG_FUNCPTR_SET(X,Y) DYN_PTR_##X = Y


/* 
 * these macros are for declaring variable pointers which will be used by
 * dynamic component code, which may need to reference a variable which will
 * only exist if a specific component is included.
 *
 * Variants for declaring it to be global, static to file, or extern.
 * Current code which references a variable which may be in a component which 
 * may not be loaded, must be changed to reference the variable via a pointer 
 * dereference, the pointer having been declared via one of these macros.
 * 
 * These macros evaluate to nothing if INSTALL_COMMON_DYNAMIC_COMP_CONFIG
 * is turned off.
 */
 
#define DYNCFG_VBLPTR(X,type)               type *DYNCFG_VPTR_##X;
#define DYNCFG_VBLPTR_STATIC(X,type) static type *DYNCFG_VPTR_##X;
#define DYNCFG_VBLPTR_EXTERN(X,type) extern type *DYNCFG_VPTR_##X;

/*
 * This macro provides the mechanism for referring to a variable which may be 
 * in a non-installed component.
 * If INSTALL_COMMON_DYNAMIC_COMP_CONFIG is not 
 * installed, it evaluates to a normal variable reference.
 */

#define DYNCFG_VREF(X)  (*DYNCFG_VPTR_##X)

/*
 * these macros are for delimiting blocks of code which are currently delimited
 * by #if <install_option> directives. They arrange for the block of code to 
 * be enclosed by a if (DYNCFG_VBL_label) { } construct. You then compile the 
 * code for the dynamic component case by turning on both the <install_option>
 * and INSTALL_COMMON_DYNAMIC_COMP_CONFIG. 
 * These macros evaluate to nothing if INSTALL_COMMON_DYNAMIC_COMP_CONFIG is
 * turned off. 
 *
 * DYNCFG_IFCFGVBL_END(X) and DYNCFG_IFNOTCFGVBL_END(X) both take an argument
 * solely to make the code more readable.
 */

#define DYNCFG_IFCFGVBL_BEGIN(X) if (DYNCFG_VBL_##X) {
#define DYNCFG_IFNOTCFGVBL_BEGIN(X) if (!DYNCFG_VBL_##X) {
#define DYNCFG_IFCFGVBL_ELSE(X)     } else {
#define DYNCFG_IFNOTCFGVBL_ELSE(X)  } else {
#define DYNCFG_IFCFGVBL_ELSEDEFAULT(X)  } else { goto default; }
#define DYNCFG_IFCFGVBL_END(X)      }
#define DYNCFG_IFNOTCFGVBL_END(X)   }

/*
 * These macros provide the mechanism to declare the variables which are 
 * used in the above code enclusure macros. They evaluate to nothing if 
 * INSTALL_COMMON_DYNAMIC_COMP_CONFIG is turned off.
 */

#define DYNCFG_VBL_DECLARE_EXTERN(X)  extern int DYNCFG_VBL_##X;
#define DYNCFG_VBL_DECLARE_GLOBAL(X)         int DYNCFG_VBL_##X;
#define DYNCFG_VBL_DECLARE_STATIC(X)  static int DYNCFG_VBL_##X;

/*
 * Use this macro to set one of these config variables to some value "val"
 */

#define DYNCFG_VBL_SET(X,val)  DYNCFG_VBL_##X = val

/*
 * this macro returns the actual variable itself
 */

#define DYNCFG_VBL(X) DYNCFG_VBL_##X

#else

#define DYNCFG_FUNCPTR0(X,rval) 
#define DYNCFG_STATIC_FUNCPTR0(X,rval) 
#define DYNCFG_EXTERN_FUNCPTR0(X,rval) 

#define DYNCFG_FUNCPTR1(X,rval,a1) 
#define DYNCFG_STATIC_FUNCPTR1(X,rval,a1) 
#define DYNCFG_EXTERN_FUNCPTR1(X,rval,a1) 

#define DYNCFG_FUNCPTR2(X,rval,a1,a2) 
#define DYNCFG_STATIC_FUNCPTR2(X,rval,a1,a2) 
#define DYNCFG_EXTERN_FUNCPTR2(X,rval,a1,a2) 

#define DYNCFG_FUNCPTR3(X,rval,a1,a2,a3)
#define DYNCFG_STATIC_FUNCPTR3(X,rval,a1,a2,a3)
#define DYNCFG_EXTERN_FUNCPTR3(X,rval,a1,a2,a3)

#define DYNCFG_FUNCPTR4(X,rval,a1,a2,a3, a4) 
#define DYNCFG_STATIC_FUNCPTR4(X,rval,a1,a2,a3, a4) 
#define DYNCFG_EXTERN_FUNCPTR4(X,rval,a1,a2,a3, a4) 

#define DYNCFG_FUNCPTR5(X,rval,a1,a2,a3, a4, a5) 
#define DYNCFG_STATIC_FUNCPTR5(X,rval,a1,a2,a3, a4, a5) 
#define DYNCFG_EXTERN_FUNCPTR5(X,rval,a1,a2,a3, a4, a5) 

#define DYNCFG_FUNCPTR6(X,rval,a1,a2,a3, a4, a5, a6) 
#define DYNCFG_STATIC_FUNCPTR6(X,rval,a1,a2,a3, a4, a5, a6) 
#define DYNCFG_EXTERN_FUNCPTR6(X,rval,a1,a2,a3, a4, a5, a6) 

#define DYNCFG_FUNCPTR7(X,rval,a1,a2,a3, a4, a5, a6, a7) 
#define DYNCFG_STATIC_FUNCPTR7(X,rval,a1,a2,a3, a4, a5, a6, a7) 
#define DYNCFG_EXTERN_FUNCPTR7(X,rval,a1,a2,a3, a4, a5, a6, a7) 

#define DYNCFG_FUNCPTR8(X,rval,a1,a2,a3, a4, a5, a6, a7, a8) 
#define DYNCFG_STATIC_FUNCPTR8(X,rval,a1,a2,a3, a4, a5, a6, a7, a8) 
#define DYNCFG_EXTERN_FUNCPTR8(X,rval,a1,a2,a3, a4, a5, a6, a7, a8) 

#define DYNCFG_FUNCPTR9(X,rval,a1,a2,a3, a4, a5, a6, a7, a8, a9) 
#define DYNCFG_STATIC_FUNCPTR9(X,rval,a1,a2,a3, a4, a5, a6, a7, a8, a9) 
#define DYNCFG_EXTERN_FUNCPTR9(X,rval,a1,a2,a3, a4, a5, a6, a7, a8, a9) 

#define DYNCFG_FUNCPTR10(X,rval,a1,a2,a3, a4, a5, a6, a7, a8, a9, a10) 
#define DYNCFG_STATIC_FUNCPTR10(X,rval,a1,a2,a3, a4, a5, a6, a7, a8, a9, a10) 
#define DYNCFG_EXTERN_FUNCPTR10(X,rval,a1,a2,a3, a4, a5, a6, a7, a8, a9, a10) 

#define DYNCFG_FUNCALL(X) X

#define DYNCFG_VBLPTR(X,type)        
#define DYNCFG_VBLPTR_STATIC(X,type) 
#define DYNCFG_VBLPTR_EXTERN(X,type) 

#define DYNCFG_VREF(X)  X

#define DYNCFG_IFCFGVBL_BEGIN(X) 
#define DYNCFG_IFNOTCFGVBL_BEGIN(X) 
#define DYNCFG_IFCFGVBL_ELSE(X)     
#define DYNCFG_IFNOTCFGVBL_ELSE(X) 
#define DYNCFG_IFCFGVBL_ELSEDEFAULT(X)
#define DYNCFG_IFCFGVBL_END(X)   
#define DYNCFG_IFNOTCFGVBL_END(X)

#define DYNCFG_VBL_DECLARE_EXTERN(X)  
#define DYNCFG_VBL_DECLARE_GLOBAL(X)
#define DYNCFG_VBL_DECLARE_STATIC(X)

#endif

#endif /* COMMON_DYNCFG_H */

/*
 * Example of usage: code which looked like:
 *    #if INSTALL_ENVOY_SNMP_VERSION_3
 *		case SNMP_VERSION_3:
 *			Call_Func_In_V3_Component (a, b);
 *                      break;
 *    #endif
 * becomes:
 *    #if INSTALL_ENVOY_SNMP_VERSION_3
 *              DYNCFG_IFCFGVBL_BEGIN(snmpv3_component)
 *		case SNMP_VERSION_3:
 *			DYN_FUNCALL(Call_Func_In_V3_Component) (a, b);
 *                      break;
 *              DYNCFG_IFCFGVBL_BEGIN(snmpv3_component)
 *    #endif
 *
 * In order for the above to work, earlier in the same file we must have 
 * declarations for the test variables and function pointers. If 
 *
 * If Call_Function_In_SNMPV3_Component () took 2 args, say, an (SNMP_PKT_T *)
 * and an integer, and returns void:
 *	
 *    DYNCFG_EXTERN_FUNCPTR2(Call_Func_In_V3_Component,void,SNMP_PKT_T *, int)
 *    DYNCFG_VBL_DECLARE_EXTERN(snmpv3_component)
 *
 * Elsewhere in some other module, these must be declared with the non-EXTERN
 * form of the macro, and initialized at startup by come component init 
 * routine.
 */




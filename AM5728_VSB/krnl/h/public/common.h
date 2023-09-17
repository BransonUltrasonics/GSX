/* $Header: /usr/cvsroot/target/h/wrn/wm/util/port/vxworks/common.h,v 1.3 2003/01/15 14:00:28 josh Exp $ */


/*
 * Copyright (C) 1999-2005 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */

/*
 *
 *  Copyright 1997 Epilogue Technology Corporation.
 *  Copyright 1998 Integrated Systems, Inc.
 *  All rights reserved.
 *
 */

/*
 * $Log: common.h,v $
 * Revision 1.3  2003/01/15 14:00:28  josh
 * directory structure shifting
 *
 * Revision 1.2  2001/11/07 14:26:53  tneale
 * Added wm to include path
 *
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.3  2000/02/04 21:49:29  josh
 * modified header files for clean builds of vxWorks demo
 *
 * Revision 1.2  2000/01/28 23:09:25  josh
 * make sure we're really using memcmp() when we intend to
 *
 * Revision 1.1  2000/01/27 18:25:29  josh
 * initial checkin of porting files for vxWorks
 *
 *
 */

/*
modification history
-------------------
01b,19apr05,job  update copyright notices
01a,06oct04,job  draw processor alignment from vxWorks header files
*/

#include <string.h>

#include <vxWorks.h>

#ifdef INSTALL_ON_BIG_ENDIAN
#undef INSTALL_ON_BIG_ENDIAN
#endif
    
#ifdef INSTALL_ON_LITTLE_ENDIAN
#undef INSTALL_ON_LITTLE_ENDIAN
#endif
    
#if (_BYTE_ORDER == _BIG_ENDIAN)
#define INSTALL_ON_BIG_ENDIAN 1
#define INSTALL_ON_LITTLE_ENDIAN 0
#elif (_BYTE_ORDER == _LITTLE_ENDIAN)
#define INSTALL_ON_LITTLE_ENDIAN 1
#define INSTALL_ON_BIT_ENDIAN 0
#endif

#ifdef CONFIG_PREAMBLE

/* This is a sample of how one might use the BUG() macros for simple
 * debugging.  You can almost certainly do better than this, see the
 * comments in common/h/bug.h for details.  This definition suffices
 * for simple (and very verbose) debugging.  
 */

#if 0 && defined(INSTALL_BUG) && INSTALL_BUG

#define BUG_OUT stdout

#define BUG(w,x,y,z) \
  do {\
    fprintf z;\
    fprintf (BUG_OUT, "\n");\
    if ((x) == BUG_FATAL)\
      t_suspend(0L);\
  } while (0)

#include <wrn/wm/common/bug.h>

#endif /* defined(INSTALL_BUG) && INSTALL_BUG */

/*
 * Fun with our new run-time debugging facility.
 * This is still experimental, and defaults to uninstalled.
 */
#if defined(INSTALL_SNARK_BUG_CONF) && INSTALL_SNARK_BUG_CONF
#include <wrn/wm/demo/bug_conf.h>
#endif

#else /* CONFIG_PREAMBLE */

#define GLUE_ALLOC(x)                   ((void *) malloc(x))
#define GLUE_FREE(x)                    free((char *) (x))
#define GLUE_NOW()                      glue_now()

#endif /* CONFIG_PREAMBLE */

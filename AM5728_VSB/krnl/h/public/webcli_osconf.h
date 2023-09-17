/* webcli_osconf.h 
 *
 * handle all os configuration issues and OS header files needed here.
 */

/* Copyright 2007 Wind River Systems, Inc. */

/*
modification history
--------------------
*/


#ifdef WEBCLI_ON_LINUX
# include "vxDefs.h"
#else

#include <vxWorks.h>

#ifdef __WEBCLI_OSCONF_NEED_MSGQLIB__
# include <msgQLib.h>
#endif

#ifdef __WEBCLI_OSCONF_NEED_SEMLIB__
# include <semLib.h>
#endif

#ifdef __WEBCLI_OSCONF_NEED_RINGLIB__
# include <rngLib.h>
#endif

#endif


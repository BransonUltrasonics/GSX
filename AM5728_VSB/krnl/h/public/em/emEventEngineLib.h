/* emEventEngineLib.h - Event Manager event engine methods. */

/* Copyright 1984-2002 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01b,16feb06,ps  changes for webcli on linux
01a,01jul02,ssk  created.
*/

#ifndef __INCemEventEngineLibh
#define __INCemEventEngineLibh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */ 

#include <webcli_osconf.h>

#include "emLib.h"

/* defines */

#define EM_EVENT_ENGINE_PRI_DEFAULT 95

/* function declarartions */

STATUS emEventEngineLibInit(int stackSize);

STATUS emEventSubmitAsync(EM_PRODUCER_CONTEXT * pProducerContext,
                          EM_EVENT_DATA * pEvent, int submitOpts);

STATUS emEventEngineStart(EM_ID em, uint32_t priority, uint32_t acceptorQMax);

STATUS emEventEngineStop(EM_ID em);


#ifdef __cplusplus
}
#endif

#endif /* __INCemEngineLibh */

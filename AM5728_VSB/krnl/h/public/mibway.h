/* mibway.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,20feb04,jws  added copyright and mod history
*/

#ifdef __SNMP_API_ENABLED__

#ifdef ENVOY_STACK_K

#include "wrn/wm/snmp/engine/asn1.h"
#include "wrn/wm/snmp/engine/asn1conf.h"

#include <wrn/wm/snmp/engine/vbdefs.h>

/* prevents glue.h included by snmp.h from redefining STRXXX functions */
#define COMMON_GLUE_H
#include <wrn/wm/snmp/engine/snmp.h>
#include <wrn/wm/snmp/engine/objectid.h>

#include "rcm_envoy.h"
#include "rcm_ev_cnv.h"

#endif /* ENVOY_STACK_K */

#ifdef __CMU_SNMP_V1__
#include "snmp.h"
#include "rcm_cmu.h"
#include "rcm_cmu_cnv.h"
#include "snmp_api.h"
#include "snmp_impl.h"
#include "asn1.h"
#endif

#include "rcm_snmp.h"
#include "rcm_mibway.h"

#define GET_SNMP_ENV_PTR(x)   SNMP_DATA_GET_M(wmbSessionWmm(x))

#endif /* __SNMP_API_ENABLED__ */

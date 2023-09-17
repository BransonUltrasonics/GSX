/* This file was automatically generated by Epilogue Technology's
 * network datastructure layout tool.
 * 
 * DO NOT MODIFY THIS FILE BY HAND.
 * 
 * Source file information:
 *  Id: ip6.ldb,v 1.13 2000/12/02 20:22:20 sar Exp 
 *    --used --
 *  Id: ip.ldb,v 1.18 1999/10/19 20:12:43 sar Exp 
 */

#ifndef EPILOGUE_LAYOUT_IP6_H
#define EPILOGUE_LAYOUT_IP6_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif
#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#include <wrn/wm/util/layout/ldbglue.h>
#endif

/* Definitions for IP6_ND_OPT */

#define SIZEOF_IP6_ND_OPT (48)
#define PTR_IP6_ND_OPT_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_IP6_ND_OPT_TYPE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_IP6_ND_OPT_TYPE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define IP6_ND_OPT_TYPE_is_SOURCE_LINK_LAYER_ADDRESS GLUE_CAST08(1L)
#define IP6_ND_OPT_TYPE_is_TARGET_LINK_LAYER_ADDRESS GLUE_CAST08(2L)
#define IP6_ND_OPT_TYPE_is_PREFIX_INFORMATION GLUE_CAST08(3L)
#define IP6_ND_OPT_TYPE_is_REDIRECTED_HEADER GLUE_CAST08(4L)
#define IP6_ND_OPT_TYPE_is_MTU GLUE_CAST08(5L)
#define PTR_IP6_ND_OPT_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_IP6_ND_OPT_LENGTH(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_IP6_ND_OPT_LENGTH(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_IP6_ND_OPT_LINK_LAYER(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_IP6_ND_OPT_LINK_LAYER (2)
#define PTR_IP6_ND_OPT_LINK_LAYER_ADDR(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define SIZEOF_IP6_ND_OPT_LINK_LAYER_ADDR (0)
#define PTR_IP6_ND_OPT_LINK_LAYER_ADDR_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(2)+(_X0_))
#define GET_IP6_ND_OPT_LINK_LAYER_ADDR_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)+(_X0_))
#define SET_IP6_ND_OPT_LINK_LAYER_ADDR_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2)+(_X0_), GLUE_CAST08(_V_))
#define PTR_IP6_ND_OPT_PREFIX(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_IP6_ND_OPT_PREFIX (32)
#define PTR_IP6_ND_OPT_PREFIX_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_IP6_ND_OPT_PREFIX_LENGTH(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_IP6_ND_OPT_PREFIX_LENGTH(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
/* Can't define PTR_IP6_ND_OPT_PREFIX_ON_LINK_P because:
   Size 1 is not a multiple of 8.   */
#define GET_IP6_ND_OPT_PREFIX_ON_LINK_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) & GLUE_CAST08(0x80L))
#define SET_IP6_ND_OPT_PREFIX_ON_LINK_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(3), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) | GLUE_CAST08(0x80L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) & ~GLUE_CAST08(0x80L)))
/* Can't define PTR_IP6_ND_OPT_PREFIX_AUTONOMOUS_P because:
   Size 1 is not a multiple of 8.   */
#define GET_IP6_ND_OPT_PREFIX_AUTONOMOUS_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) & GLUE_CAST08(0x40L))
#define SET_IP6_ND_OPT_PREFIX_AUTONOMOUS_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(3), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) | GLUE_CAST08(0x40L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) & ~GLUE_CAST08(0x40L)))
#define PTR_IP6_ND_OPT_PREFIX_VALID_LIFETIME(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_IP6_ND_OPT_PREFIX_VALID_LIFETIME(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_IP6_ND_OPT_PREFIX_VALID_LIFETIME(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_IP6_ND_OPT_PREFIX_PREFERRED_LIFETIME(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define GET_IP6_ND_OPT_PREFIX_PREFERRED_LIFETIME(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(8))
#define SET_IP6_ND_OPT_PREFIX_PREFERRED_LIFETIME(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(8), GLUE_CAST32(_V_))
#define PTR_IP6_ND_OPT_PREFIX_PREFIX(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define PTR_IP6_ND_OPT_REDIRECTED(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_IP6_ND_OPT_REDIRECTED (48)
#define PTR_IP6_ND_OPT_REDIRECTED_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_IP6_ND_OPT_MTU(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_IP6_ND_OPT_MTU (8)
#define PTR_IP6_ND_OPT_MTU_MTU(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_IP6_ND_OPT_MTU_MTU(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_IP6_ND_OPT_MTU_MTU(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))

/* Definitions for ICMP6_MSG */

#define SIZEOF_ICMP6_MSG (48)
/* Can't define PTR_ICMP6_MSG_INFO_P because:
   Size 1 is not a multiple of 8.   */
#define GET_ICMP6_MSG_INFO_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))) & GLUE_CAST08(0x80L))
#define SET_ICMP6_MSG_INFO_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))) | GLUE_CAST08(0x80L) : GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0x80L)))
#define PTR_ICMP6_MSG_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_ICMP6_MSG_TYPE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_ICMP6_MSG_TYPE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define ICMP6_MSG_TYPE_is_DESTINATION_UNREACHABLE GLUE_CAST08(1L)
#define ICMP6_MSG_TYPE_is_PACKET_TOO_BIG GLUE_CAST08(2L)
#define ICMP6_MSG_TYPE_is_TIME_EXCEEDED GLUE_CAST08(3L)
#define ICMP6_MSG_TYPE_is_PARAMETER_PROBLEM GLUE_CAST08(4L)
#define ICMP6_MSG_TYPE_is_ECHO_REQUEST GLUE_CAST08(128L)
#define ICMP6_MSG_TYPE_is_ECHO_REPLY GLUE_CAST08(129L)
#define ICMP6_MSG_TYPE_is_GM_QUERY GLUE_CAST08(130L)
#define ICMP6_MSG_TYPE_is_GM_REPORT GLUE_CAST08(131L)
#define ICMP6_MSG_TYPE_is_GM_REDUCTION GLUE_CAST08(132L)
#define ICMP6_MSG_TYPE_is_ROUTER_SOLICITATION GLUE_CAST08(133L)
#define ICMP6_MSG_TYPE_is_ROUTER_ADVERTISEMENT GLUE_CAST08(134L)
#define ICMP6_MSG_TYPE_is_NEIGHBOR_SOLICITATION GLUE_CAST08(135L)
#define ICMP6_MSG_TYPE_is_NEIGHBOR_ADVERTISEMENT GLUE_CAST08(136L)
#define ICMP6_MSG_TYPE_is_REDIRECT GLUE_CAST08(137L)
#define PTR_ICMP6_MSG_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_CHECKSUM(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_ICMP6_MSG_CHECKSUM(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(2))
#define SET_ICMP6_MSG_CHECKSUM(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST16(_V_))
#define PTR_ICMP6_MSG_IDENTIFICATION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_ICMP6_MSG_IDENTIFICATION(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_))+(4))
#define SET_ICMP6_MSG_IDENTIFICATION(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST16(_V_))
#define PTR_ICMP6_MSG_SEQUENCE_NUMBER(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define GET_ICMP6_MSG_SEQUENCE_NUMBER(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(6))
#define SET_ICMP6_MSG_SEQUENCE_NUMBER(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(6), GLUE_CAST16(_V_))
#define PTR_ICMP6_MSG_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_ICMP6_MSG_MINIMUM_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_MINIMUM_HEADER (8)
#define PTR_ICMP6_MSG_DU(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_DU (8)
#define PTR_ICMP6_MSG_DU_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_DU_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_DU_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_DU_CODE_is_NO_ROUTE GLUE_CAST08(0L)
#define ICMP6_MSG_DU_CODE_is_PROHIBITED GLUE_CAST08(1L)
#define ICMP6_MSG_DU_CODE_is_ADDRESS GLUE_CAST08(3L)
#define ICMP6_MSG_DU_CODE_is_PORT GLUE_CAST08(4L)
#define PTR_ICMP6_MSG_PTB(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_PTB (8)
#define PTR_ICMP6_MSG_PTB_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_PTB_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_PTB_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_PTB_MTU(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_ICMP6_MSG_PTB_MTU(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_ICMP6_MSG_PTB_MTU(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_ICMP6_MSG_TE(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_TE (8)
#define PTR_ICMP6_MSG_TE_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_TE_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_TE_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_TE_CODE_is_HOPS GLUE_CAST08(0L)
#define ICMP6_MSG_TE_CODE_is_REASSEMBLY GLUE_CAST08(1L)
#define PTR_ICMP6_MSG_TE_MTU(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_ICMP6_MSG_TE_MTU(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_ICMP6_MSG_TE_MTU(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_ICMP6_MSG_PP(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_PP (8)
#define PTR_ICMP6_MSG_PP_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_PP_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_PP_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_PP_CODE_is_ZERO GLUE_CAST08(0L)
#define ICMP6_MSG_PP_CODE_is_UNREC_HEADER GLUE_CAST08(1L)
#define ICMP6_MSG_PP_CODE_is_UNREC_OPTION GLUE_CAST08(2L)
#define PTR_ICMP6_MSG_PP_POINTER(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_ICMP6_MSG_PP_POINTER(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_ICMP6_MSG_PP_POINTER(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_ICMP6_MSG_ECHO(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_ECHO (8)
#define PTR_ICMP6_MSG_ECHO_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_ECHO_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_ECHO_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_ECHO_CODE_is_ZERO GLUE_CAST08(0L)
#define PTR_ICMP6_MSG_ECHO_DATA(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define SIZEOF_ICMP6_MSG_ECHO_DATA (0)
#define PTR_ICMP6_MSG_ECHO_DATA_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(8)+(_X0_))
#define GET_ICMP6_MSG_ECHO_DATA_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(8)+(_X0_))
#define SET_ICMP6_MSG_ECHO_DATA_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(8)+(_X0_), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_GM(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_GM (24)
#define PTR_ICMP6_MSG_GM_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_GM_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_GM_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_GM_CODE_is_ZERO GLUE_CAST08(0L)
#define PTR_ICMP6_MSG_GM_DELAY(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_ICMP6_MSG_GM_DELAY(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(4))
#define SET_ICMP6_MSG_GM_DELAY(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST16(_V_))
#define PTR_ICMP6_MSG_GM_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_ICMP6_MSG_RS(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_RS (8)
#define PTR_ICMP6_MSG_RS_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_RS_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_RS_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_RS_CODE_is_ZERO GLUE_CAST08(0L)
#define PTR_ICMP6_MSG_RS_OPTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define SIZEOF_ICMP6_MSG_RS_OPTIONS (0)
#define PTR_ICMP6_MSG_RS_OPTIONS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(8)+(_X0_))
#define GET_ICMP6_MSG_RS_OPTIONS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(8)+(_X0_))
#define SET_ICMP6_MSG_RS_OPTIONS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(8)+(_X0_), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_RA(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_RA (16)
#define PTR_ICMP6_MSG_RA_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_RA_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_RA_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_RA_CODE_is_ZERO GLUE_CAST08(0L)
#define PTR_ICMP6_MSG_RA_HOP_LIMIT(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_ICMP6_MSG_RA_HOP_LIMIT(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(4))
#define SET_ICMP6_MSG_RA_HOP_LIMIT(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST08(_V_))
/* Can't define PTR_ICMP6_MSG_RA_MANAGED_P because:
   Size 1 is not a multiple of 8.   */
#define GET_ICMP6_MSG_RA_MANAGED_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(5)) & GLUE_CAST08(0x80L))
#define SET_ICMP6_MSG_RA_MANAGED_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(5), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(5)) | GLUE_CAST08(0x80L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(5)) & ~GLUE_CAST08(0x80L)))
/* Can't define PTR_ICMP6_MSG_RA_OTHER_P because:
   Size 1 is not a multiple of 8.   */
#define GET_ICMP6_MSG_RA_OTHER_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(5)) & GLUE_CAST08(0x40L))
#define SET_ICMP6_MSG_RA_OTHER_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(5), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(5)) | GLUE_CAST08(0x40L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(5)) & ~GLUE_CAST08(0x40L)))
#define PTR_ICMP6_MSG_RA_ROUTER_LIFETIME(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define GET_ICMP6_MSG_RA_ROUTER_LIFETIME(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(6))
#define SET_ICMP6_MSG_RA_ROUTER_LIFETIME(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(6), GLUE_CAST16(_V_))
#define PTR_ICMP6_MSG_RA_REACHABLE_TIME(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define GET_ICMP6_MSG_RA_REACHABLE_TIME(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(8))
#define SET_ICMP6_MSG_RA_REACHABLE_TIME(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(8), GLUE_CAST32(_V_))
#define PTR_ICMP6_MSG_RA_RETRANSMIT_TIME(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define GET_ICMP6_MSG_RA_RETRANSMIT_TIME(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(12))
#define SET_ICMP6_MSG_RA_RETRANSMIT_TIME(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(12), GLUE_CAST32(_V_))
#define PTR_ICMP6_MSG_RA_OPTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define SIZEOF_ICMP6_MSG_RA_OPTIONS (0)
#define PTR_ICMP6_MSG_RA_OPTIONS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(16)+(_X0_))
#define GET_ICMP6_MSG_RA_OPTIONS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(16)+(_X0_))
#define SET_ICMP6_MSG_RA_OPTIONS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(16)+(_X0_), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_NS(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_NS (24)
#define PTR_ICMP6_MSG_NS_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_NS_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_NS_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_NS_CODE_is_ZERO GLUE_CAST08(0L)
#define PTR_ICMP6_MSG_NS_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_ICMP6_MSG_NS_OPTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(24))
#define SIZEOF_ICMP6_MSG_NS_OPTIONS (0)
#define PTR_ICMP6_MSG_NS_OPTIONS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(24)+(_X0_))
#define GET_ICMP6_MSG_NS_OPTIONS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(24)+(_X0_))
#define SET_ICMP6_MSG_NS_OPTIONS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(24)+(_X0_), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_NA(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_NA (24)
#define PTR_ICMP6_MSG_NA_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_NA_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_NA_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_NA_CODE_is_ZERO GLUE_CAST08(0L)
/* Can't define PTR_ICMP6_MSG_NA_ROUTER_P because:
   Size 1 is not a multiple of 8.   */
#define GET_ICMP6_MSG_NA_ROUTER_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) & GLUE_CAST08(0x80L))
#define SET_ICMP6_MSG_NA_ROUTER_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(4), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) | GLUE_CAST08(0x80L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) & ~GLUE_CAST08(0x80L)))
/* Can't define PTR_ICMP6_MSG_NA_SOLICITED_P because:
   Size 1 is not a multiple of 8.   */
#define GET_ICMP6_MSG_NA_SOLICITED_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) & GLUE_CAST08(0x40L))
#define SET_ICMP6_MSG_NA_SOLICITED_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(4), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) | GLUE_CAST08(0x40L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) & ~GLUE_CAST08(0x40L)))
/* Can't define PTR_ICMP6_MSG_NA_OVERRIDE_P because:
   Size 1 is not a multiple of 8.   */
#define GET_ICMP6_MSG_NA_OVERRIDE_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) & GLUE_CAST08(0x20L))
#define SET_ICMP6_MSG_NA_OVERRIDE_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(4), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) | GLUE_CAST08(0x20L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)) & ~GLUE_CAST08(0x20L)))
#define PTR_ICMP6_MSG_NA_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_ICMP6_MSG_NA_OPTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(24))
#define SIZEOF_ICMP6_MSG_NA_OPTIONS (0)
#define PTR_ICMP6_MSG_NA_OPTIONS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(24)+(_X0_))
#define GET_ICMP6_MSG_NA_OPTIONS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(24)+(_X0_))
#define SET_ICMP6_MSG_NA_OPTIONS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(24)+(_X0_), GLUE_CAST08(_V_))
#define PTR_ICMP6_MSG_RE(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ICMP6_MSG_RE (40)
#define PTR_ICMP6_MSG_RE_CODE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_ICMP6_MSG_RE_CODE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_ICMP6_MSG_RE_CODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define ICMP6_MSG_RE_CODE_is_ZERO GLUE_CAST08(0L)
#define PTR_ICMP6_MSG_RE_TARGET_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_ICMP6_MSG_RE_DESTINATION_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(24))
#define PTR_ICMP6_MSG_RE_OPTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(40))
#define SIZEOF_ICMP6_MSG_RE_OPTIONS (0)
#define PTR_ICMP6_MSG_RE_OPTIONS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(40)+(_X0_))
#define GET_ICMP6_MSG_RE_OPTIONS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(40)+(_X0_))
#define SET_ICMP6_MSG_RE_OPTIONS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(40)+(_X0_), GLUE_CAST08(_V_))

/* Definitions for IP6_PSEUDO_HEADER */

#define SIZEOF_IP6_PSEUDO_HEADER (40)
#define PTR_IP6_PSEUDO_HEADER_SOURCE_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define PTR_IP6_PSEUDO_HEADER_DESTINATION_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define PTR_IP6_PSEUDO_HEADER_PAYLOAD_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(32))
#define GET_IP6_PSEUDO_HEADER_PAYLOAD_LENGTH(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(32))
#define SET_IP6_PSEUDO_HEADER_PAYLOAD_LENGTH(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(32), GLUE_CAST32(_V_))
#define PTR_IP6_PSEUDO_HEADER_NEXT_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_))+(39))
#define GET_IP6_PSEUDO_HEADER_NEXT_HEADER(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(39))
#define SET_IP6_PSEUDO_HEADER_NEXT_HEADER(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(39), GLUE_CAST08(_V_))

/* Definitions for IP6_FRAGMENT_HEADER */

#define SIZEOF_IP6_FRAGMENT_HEADER (8)
#define PTR_IP6_FRAGMENT_HEADER_NEXT_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_IP6_FRAGMENT_HEADER_NEXT_HEADER(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_IP6_FRAGMENT_HEADER_NEXT_HEADER(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
/* Can't define PTR_IP6_FRAGMENT_HEADER_OFFSET because:
   Size 13 is not a multiple of 8.   */
#define GET_IP6_FRAGMENT_HEADER_OFFSET(_P_)\
 GLUE_CAST16(GLUE_GB16((GLUE_CAST_PTR(_P_))+(2)) >> 3)
#define SET_IP6_FRAGMENT_HEADER_OFFSET(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(2), (GLUE_GB16((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST16(0xFFF8L)) | ((GLUE_CAST16(_V_) << 3) & GLUE_CAST16(0xFFF8L)))
/* Can't define PTR_IP6_FRAGMENT_HEADER_MORE_P because:
   Size 1 is not a multiple of 8.   */
#define GET_IP6_FRAGMENT_HEADER_MORE_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) & GLUE_CAST08(0x1L))
#define SET_IP6_FRAGMENT_HEADER_MORE_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(3), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) | GLUE_CAST08(0x1L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(3)) & ~GLUE_CAST08(0x1L)))
#define PTR_IP6_FRAGMENT_HEADER_IDENTIFICATION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_IP6_FRAGMENT_HEADER_IDENTIFICATION(_P_)\
 GLUE_GU32((GLUE_CAST_PTR(_P_))+(4))
#define SET_IP6_FRAGMENT_HEADER_IDENTIFICATION(_P_, _V_)\
 GLUE_SU32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))

/* Definitions for IP6_ROUTING_HEADER */

#define SIZEOF_IP6_ROUTING_HEADER (8)
#define PTR_IP6_ROUTING_HEADER_NEXT_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_IP6_ROUTING_HEADER_NEXT_HEADER(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_IP6_ROUTING_HEADER_NEXT_HEADER(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_IP6_ROUTING_HEADER_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_IP6_ROUTING_HEADER_LENGTH(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_IP6_ROUTING_HEADER_LENGTH(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_IP6_ROUTING_HEADER_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_IP6_ROUTING_HEADER_TYPE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_IP6_ROUTING_HEADER_TYPE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define IP6_ROUTING_HEADER_TYPE_is_TYPE0 GLUE_CAST08(0L)
#define PTR_IP6_ROUTING_HEADER_SEGMENTS_LEFT(_P_)\
 ((GLUE_CAST_PTR(_P_))+(3))
#define GET_IP6_ROUTING_HEADER_SEGMENTS_LEFT(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(3))
#define SET_IP6_ROUTING_HEADER_SEGMENTS_LEFT(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(3), GLUE_CAST08(_V_))
#define PTR_IP6_ROUTING_HEADER_STRICT(_P_)\
 ((GLUE_CAST_PTR(_P_))+(5))
#define SIZEOF_IP6_ROUTING_HEADER_STRICT (3)
/* Can't define PTR_IP6_ROUTING_HEADER_STRICT_ELT because:
   Size 1 is not a multiple of 8.   */
/* Can't define (GET_IP6_ROUTING_HEADER_STRICT_ELT SET_IP6_ROUTING_HEADER_STRICT_ELT) because:
   Array elements are not aligned on octet boundaries: ((1 0 23))   */
#define PTR_IP6_ROUTING_HEADER_HOP(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define SIZEOF_IP6_ROUTING_HEADER_HOP (0)
#define PTR_IP6_ROUTING_HEADER_HOP_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(8)+(16)*(_X0_))

/* Definitions for IP6_OPTION */

#define SIZEOF_IP6_OPTION (6)
/* Can't define PTR_IP6_OPTION_ACTION because:
   Size 2 is not a multiple of 8.   */
#define GET_IP6_OPTION_ACTION(_P_)\
 GLUE_CAST08(GLUE_GB08((GLUE_CAST_PTR(_P_))) >> 6)
#define SET_IP6_OPTION_ACTION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), (GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0xC0L)) | ((GLUE_CAST08(_V_) << 6) & GLUE_CAST08(0xC0L)))
#define IP6_OPTION_ACTION_is_IGNORE GLUE_CAST08(0L)
#define IP6_OPTION_ACTION_is_NEVER_COMPLAIN GLUE_CAST08(1L)
#define IP6_OPTION_ACTION_is_ALWAYS_COMPLAIN GLUE_CAST08(2L)
#define IP6_OPTION_ACTION_is_COMPLAIN GLUE_CAST08(3L)
/* Can't define PTR_IP6_OPTION_MAY_CHANGE_P because:
   Size 1 is not a multiple of 8.   */
#define GET_IP6_OPTION_MAY_CHANGE_P(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))) & GLUE_CAST08(0x20L))
#define SET_IP6_OPTION_MAY_CHANGE_P(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))) | GLUE_CAST08(0x20L) : GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0x20L)))
#define PTR_IP6_OPTION_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_IP6_OPTION_TYPE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_IP6_OPTION_TYPE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define IP6_OPTION_TYPE_is_PAD1 GLUE_CAST08(0L)
#define IP6_OPTION_TYPE_is_PADN GLUE_CAST08(1L)
#define IP6_OPTION_TYPE_is_JUMBO GLUE_CAST08(194L)
#define PTR_IP6_OPTION_DATA_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_IP6_OPTION_DATA_LENGTH(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_IP6_OPTION_DATA_LENGTH(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_IP6_OPTION_JUMBO_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_IP6_OPTION_JUMBO_LENGTH(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(2))
#define SET_IP6_OPTION_JUMBO_LENGTH(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST32(_V_))

/* Definitions for IP6_OPTIONS_HEADER */

#define SIZEOF_IP6_OPTIONS_HEADER (8)
#define PTR_IP6_OPTIONS_HEADER_NEXT_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_IP6_OPTIONS_HEADER_NEXT_HEADER(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_IP6_OPTIONS_HEADER_NEXT_HEADER(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_IP6_OPTIONS_HEADER_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_IP6_OPTIONS_HEADER_LENGTH(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_IP6_OPTIONS_HEADER_LENGTH(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_IP6_OPTIONS_HEADER_OPTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define SIZEOF_IP6_OPTIONS_HEADER_OPTIONS (6)
#define PTR_IP6_OPTIONS_HEADER_OPTIONS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(2)+(_X0_))
#define GET_IP6_OPTIONS_HEADER_OPTIONS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)+(_X0_))
#define SET_IP6_OPTIONS_HEADER_OPTIONS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2)+(_X0_), GLUE_CAST08(_V_))

/* Definitions for IP6_HEADER */

#define SIZEOF_IP6_HEADER (40)
/* Can't define PTR_IP6_HEADER_VERSION because:
   Size 4 is not a multiple of 8.   */
#define GET_IP6_HEADER_VERSION(_P_)\
 GLUE_CAST08(GLUE_GB08((GLUE_CAST_PTR(_P_))) >> 4)
#define SET_IP6_HEADER_VERSION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), (GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0xF0L)) | ((GLUE_CAST08(_V_) << 4) & GLUE_CAST08(0xF0L)))
#define IP6_HEADER_VERSION_is_IPV4 GLUE_CAST08(4L)
#define IP6_HEADER_VERSION_is_IPV6 GLUE_CAST08(6L)
/* Can't define PTR_IP6_HEADER_TRAFFIC_CLASS because:
   Offset 4 is not a multiple of 8.   */
#define GET_IP6_HEADER_TRAFFIC_CLASS(_P_)\
 GLUE_CAST08((GLUE_GB16((GLUE_CAST_PTR(_P_))) & GLUE_CAST16(0xFF0L)) >> 4)
#define SET_IP6_HEADER_TRAFFIC_CLASS(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_)), (GLUE_GB16((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST16(0xFF0L)) | ((GLUE_CAST16(_V_) << 4) & GLUE_CAST16(0xFF0L)))
/* Can't define PTR_IP6_HEADER_FLOW_LABEL because:
   Size 20 is not a multiple of 8.   */
#define GET_IP6_HEADER_FLOW_LABEL(_P_)\
 GLUE_CAST32(GLUE_GB32LS24((GLUE_CAST_PTR(_P_))) & GLUE_CAST32(0xFFFFFL))
#define SET_IP6_HEADER_FLOW_LABEL(_P_, _V_)\
 GLUE_SB32LS24((GLUE_CAST_PTR(_P_)), (GLUE_GB32LS24((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST32(0xFFFFFL)) | (GLUE_CAST32(_V_) & GLUE_CAST32(0xFFFFFL)))
#define PTR_IP6_HEADER_PAYLOAD_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_IP6_HEADER_PAYLOAD_LENGTH(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(4))
#define SET_IP6_HEADER_PAYLOAD_LENGTH(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST16(_V_))
#define PTR_IP6_HEADER_NEXT_HEADER(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define GET_IP6_HEADER_NEXT_HEADER(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(6))
#define SET_IP6_HEADER_NEXT_HEADER(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(6), GLUE_CAST08(_V_))
#define PTR_IP6_HEADER_HOP_LIMIT(_P_)\
 ((GLUE_CAST_PTR(_P_))+(7))
#define GET_IP6_HEADER_HOP_LIMIT(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(7))
#define SET_IP6_HEADER_HOP_LIMIT(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(7), GLUE_CAST08(_V_))
#define PTR_IP6_HEADER_SOURCE_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define PTR_IP6_HEADER_DESTINATION_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(24))

/* Definitions for IP6_ADDRESS */

#define SIZEOF_IP6_ADDRESS (16)
#define PTR_IP6_ADDRESS_BYTES(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_IP6_ADDRESS_BYTES (16)
#define PTR_IP6_ADDRESS_BYTES_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(_X0_))
#define GET_IP6_ADDRESS_BYTES_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(_X0_))
#define SET_IP6_ADDRESS_BYTES_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(_X0_), GLUE_CAST08(_V_))

#endif /* EPILOGUE_LAYOUT_IP6_H */
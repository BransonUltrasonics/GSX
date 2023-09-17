/* This file was automatically generated by Epilogue Technology's
 * network datastructure layout tool.
 * 
 * DO NOT MODIFY THIS FILE BY HAND.
 * 
 * Source file information:
 *  Id: agentx.ldb,v 1.10 1998/02/25 15:21:38 sra Exp 
 */

#ifndef EPILOGUE_LAYOUT_AGENTX_H
#define EPILOGUE_LAYOUT_AGENTX_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif
#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#include <wrn/wm/util/layout/ldbglue.h>
#endif

/* Definitions for AGENTX_PDU */

#define SIZEOF_AGENTX_PDU (20)
#define PTR_AGENTX_PDU_VERSION(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_PDU_VERSION(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_PDU_VERSION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_AGENTX_PDU_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_AGENTX_PDU_TYPE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_AGENTX_PDU_TYPE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define AGENTX_PDU_TYPE_is_OPEN GLUE_CAST08(1L)
#define AGENTX_PDU_TYPE_is_CLOSE GLUE_CAST08(2L)
#define AGENTX_PDU_TYPE_is_REG GLUE_CAST08(3L)
#define AGENTX_PDU_TYPE_is_UNREG GLUE_CAST08(4L)
#define AGENTX_PDU_TYPE_is_GET GLUE_CAST08(5L)
#define AGENTX_PDU_TYPE_is_NEXT GLUE_CAST08(6L)
#define AGENTX_PDU_TYPE_is_BULK GLUE_CAST08(7L)
#define AGENTX_PDU_TYPE_is_TEST_SET GLUE_CAST08(8L)
#define AGENTX_PDU_TYPE_is_COMMIT_SET GLUE_CAST08(9L)
#define AGENTX_PDU_TYPE_is_UNDO_SET GLUE_CAST08(10L)
#define AGENTX_PDU_TYPE_is_CLEANUP_SET GLUE_CAST08(11L)
#define AGENTX_PDU_TYPE_is_NOTIFY GLUE_CAST08(12L)
#define AGENTX_PDU_TYPE_is_PING GLUE_CAST08(13L)
#define AGENTX_PDU_TYPE_is_INDEX_ALLOCATE GLUE_CAST08(14L)
#define AGENTX_PDU_TYPE_is_INDEX_DEALLOCATE GLUE_CAST08(15L)
#define AGENTX_PDU_TYPE_is_ADD_AGENT_CAPS GLUE_CAST08(16L)
#define AGENTX_PDU_TYPE_is_REMOVE_AGENT_CAPS GLUE_CAST08(17L)
#define AGENTX_PDU_TYPE_is_RESP GLUE_CAST08(18L)
#define PTR_AGENTX_PDU_FLAGS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_PDU_FLAGS(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_PDU_FLAGS(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define AGENTX_PDU_FLAGS_is_INSTANCE_REGISTRATION GLUE_CAST08(1L)
#define AGENTX_PDU_FLAGS_is_NEW_INDEX GLUE_CAST08(2L)
#define AGENTX_PDU_FLAGS_is_ANY_INDEX GLUE_CAST08(4L)
#define AGENTX_PDU_FLAGS_is_NON_DEFAULT_CONTEXT GLUE_CAST08(8L)
#define AGENTX_PDU_FLAGS_is_NETWORK_BYTE_ORDER GLUE_CAST08(16L)
/* Can't define PTR_AGENTX_PDU_FILL0 because:
   Size 3 is not a multiple of 8.   */
#define GET_AGENTX_PDU_FILL0(_P_)\
 GLUE_CAST08(GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) >> 5)
#define SET_AGENTX_PDU_FILL0(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), (GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST08(0xE0L)) | ((GLUE_CAST08(_V_) << 5) & GLUE_CAST08(0xE0L)))
/* Can't define PTR_AGENTX_PDU_NETWORK_BYTE_ORDER because:
   Size 1 is not a multiple of 8.   */
#define GET_AGENTX_PDU_NETWORK_BYTE_ORDER(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & GLUE_CAST08(0x10L))
#define SET_AGENTX_PDU_NETWORK_BYTE_ORDER(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) | GLUE_CAST08(0x10L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST08(0x10L)))
/* Can't define PTR_AGENTX_PDU_NON_DEFAULT_CONTEXT because:
   Size 1 is not a multiple of 8.   */
#define GET_AGENTX_PDU_NON_DEFAULT_CONTEXT(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & GLUE_CAST08(0x8L))
#define SET_AGENTX_PDU_NON_DEFAULT_CONTEXT(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) | GLUE_CAST08(0x8L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST08(0x8L)))
/* Can't define PTR_AGENTX_PDU_ANY_INDEX because:
   Size 1 is not a multiple of 8.   */
#define GET_AGENTX_PDU_ANY_INDEX(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & GLUE_CAST08(0x4L))
#define SET_AGENTX_PDU_ANY_INDEX(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) | GLUE_CAST08(0x4L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST08(0x4L)))
/* Can't define PTR_AGENTX_PDU_NEW_INDEX because:
   Size 1 is not a multiple of 8.   */
#define GET_AGENTX_PDU_NEW_INDEX(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & GLUE_CAST08(0x2L))
#define SET_AGENTX_PDU_NEW_INDEX(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) | GLUE_CAST08(0x2L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST08(0x2L)))
/* Can't define PTR_AGENTX_PDU_INSTANCE_REGISTRATION because:
   Size 1 is not a multiple of 8.   */
#define GET_AGENTX_PDU_INSTANCE_REGISTRATION(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & GLUE_CAST08(0x1L))
#define SET_AGENTX_PDU_INSTANCE_REGISTRATION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) | GLUE_CAST08(0x1L) : GLUE_GB08((GLUE_CAST_PTR(_P_))+(2)) & ~GLUE_CAST08(0x1L)))
#define PTR_AGENTX_PDU_FILL1(_P_)\
 ((GLUE_CAST_PTR(_P_))+(3))
#define GET_AGENTX_PDU_FILL1(_P_)\
 GLUE_GU08((GLUE_CAST_PTR(_P_))+(3))
#define SET_AGENTX_PDU_FILL1(_P_, _V_)\
 GLUE_SU08((GLUE_CAST_PTR(_P_))+(3), GLUE_CAST08(_V_))
#define PTR_AGENTX_PDU_SESSION_ID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define SIZEOF_AGENTX_PDU_SESSION_ID (4)
#define PTR_AGENTX_PDU_SESSION_ID_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_AGENTX_PDU_SESSION_ID_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_AGENTX_PDU_SESSION_ID_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_SESSION_ID_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_AGENTX_PDU_SESSION_ID_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_))+(4))
#define SET_AGENTX_PDU_SESSION_ID_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_TRANS_ID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define SIZEOF_AGENTX_PDU_TRANS_ID (4)
#define PTR_AGENTX_PDU_TRANS_ID_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define GET_AGENTX_PDU_TRANS_ID_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(8))
#define SET_AGENTX_PDU_TRANS_ID_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(8), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_TRANS_ID_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define GET_AGENTX_PDU_TRANS_ID_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_))+(8))
#define SET_AGENTX_PDU_TRANS_ID_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_))+(8), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_PACKET_ID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define SIZEOF_AGENTX_PDU_PACKET_ID (4)
#define PTR_AGENTX_PDU_PACKET_ID_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define GET_AGENTX_PDU_PACKET_ID_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(12))
#define SET_AGENTX_PDU_PACKET_ID_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(12), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_PACKET_ID_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define GET_AGENTX_PDU_PACKET_ID_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_))+(12))
#define SET_AGENTX_PDU_PACKET_ID_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_))+(12), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_PAYLOAD_LEN(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define SIZEOF_AGENTX_PDU_PAYLOAD_LEN (4)
#define PTR_AGENTX_PDU_PAYLOAD_LEN_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define GET_AGENTX_PDU_PAYLOAD_LEN_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(16))
#define SET_AGENTX_PDU_PAYLOAD_LEN_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(16), GLUE_CAST32(_V_))
#define PTR_AGENTX_PDU_PAYLOAD_LEN_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define GET_AGENTX_PDU_PAYLOAD_LEN_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_))+(16))
#define SET_AGENTX_PDU_PAYLOAD_LEN_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_))+(16), GLUE_CAST32(_V_))

/* Definitions for AGENTX_CLOSE */

#define SIZEOF_AGENTX_CLOSE (4)
#define PTR_AGENTX_CLOSE_REASON(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_CLOSE_REASON(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_CLOSE_REASON(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define AGENTX_CLOSE_REASON_is_OTHER GLUE_CAST08(1L)
#define AGENTX_CLOSE_REASON_is_PARSE_ERROR GLUE_CAST08(2L)
#define AGENTX_CLOSE_REASON_is_PROTOCOL_ERROR GLUE_CAST08(3L)
#define AGENTX_CLOSE_REASON_is_TIMEOUTS GLUE_CAST08(4L)
#define AGENTX_CLOSE_REASON_is_SHUTDOWN GLUE_CAST08(5L)
#define AGENTX_CLOSE_REASON_is_BY_MANAGER GLUE_CAST08(6L)
#define PTR_AGENTX_CLOSE_FILL0(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_AGENTX_CLOSE_FILL0(_P_)\
 GLUE_GU08((GLUE_CAST_PTR(_P_))+(1))
#define SET_AGENTX_CLOSE_FILL0(_P_, _V_)\
 GLUE_SU08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_AGENTX_CLOSE_FILL1(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_CLOSE_FILL1(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_CLOSE_FILL1(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST16(_V_))

/* Definitions for AGENTX_TYPE_CODE */

#define SIZEOF_AGENTX_TYPE_CODE (2)
#define GET_AGENTX_TYPE_CODE(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_TYPE_CODE(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define AGENTX_TYPE_CODE_is_INTEGER GLUE_CAST16(2L)
#define AGENTX_TYPE_CODE_is_OCT_STR GLUE_CAST16(4L)
#define AGENTX_TYPE_CODE_is_NULL GLUE_CAST16(5L)
#define AGENTX_TYPE_CODE_is_OID GLUE_CAST16(6L)
#define AGENTX_TYPE_CODE_is_IP_ADDRESS GLUE_CAST16(64L)
#define AGENTX_TYPE_CODE_is_COUNTER32 GLUE_CAST16(65L)
#define AGENTX_TYPE_CODE_is_GAUGE32 GLUE_CAST16(66L)
#define AGENTX_TYPE_CODE_is_TIME_TICKS GLUE_CAST16(67L)
#define AGENTX_TYPE_CODE_is_OPAQUE GLUE_CAST16(68L)
#define AGENTX_TYPE_CODE_is_COUNTER64 GLUE_CAST16(70L)
#define AGENTX_TYPE_CODE_is_NO_SUCH_OBJECT GLUE_CAST16(128L)
#define AGENTX_TYPE_CODE_is_NO_SUCH_INSTANCE GLUE_CAST16(129L)
#define AGENTX_TYPE_CODE_is_END_OF_MIB_VIEW GLUE_CAST16(130L)

/* Definitions for AGENTX_VARBIND */

#define SIZEOF_AGENTX_VARBIND (8)
#define PTR_AGENTX_VARBIND_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_AGENTX_VARBIND_TYPE (2)
#define PTR_AGENTX_VARBIND_TYPE_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_VARBIND_TYPE_B(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_VARBIND_TYPE_B(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define PTR_AGENTX_VARBIND_TYPE_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_VARBIND_TYPE_L(_P_)\
 GLUE_GL16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_VARBIND_TYPE_L(_P_, _V_)\
 GLUE_SL16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define PTR_AGENTX_VARBIND_FILL0(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_VARBIND_FILL0(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_VARBIND_FILL0(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST16(_V_))
#define PTR_AGENTX_VARBIND_NAME(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))

/* Definitions for AGENTX_UNSIGNED32 */

#define SIZEOF_AGENTX_UNSIGNED32 (4)
#define PTR_AGENTX_UNSIGNED32_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_UNSIGNED32_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_UNSIGNED32_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))
#define PTR_AGENTX_UNSIGNED32_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_UNSIGNED32_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_UNSIGNED32_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))

/* Definitions for AGENTX_UNREG */

#define SIZEOF_AGENTX_UNREG (8)
#define PTR_AGENTX_UNREG_FILL0(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_UNREG_FILL0(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_UNREG_FILL0(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define PTR_AGENTX_UNREG_RANGE_SUBID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_UNREG_RANGE_SUBID(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_UNREG_RANGE_SUBID(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define PTR_AGENTX_UNREG_FILL1(_P_)\
 ((GLUE_CAST_PTR(_P_))+(3))
#define GET_AGENTX_UNREG_FILL1(_P_)\
 GLUE_GU08((GLUE_CAST_PTR(_P_))+(3))
#define SET_AGENTX_UNREG_FILL1(_P_, _V_)\
 GLUE_SU08((GLUE_CAST_PTR(_P_))+(3), GLUE_CAST08(_V_))
#define PTR_AGENTX_UNREG_REGION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))

/* Definitions for AGENTX_UNSIGNED16 */

#define SIZEOF_AGENTX_UNSIGNED16 (2)
#define PTR_AGENTX_UNSIGNED16_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_UNSIGNED16_B(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_UNSIGNED16_B(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define PTR_AGENTX_UNSIGNED16_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_UNSIGNED16_L(_P_)\
 GLUE_GL16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_UNSIGNED16_L(_P_, _V_)\
 GLUE_SL16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))

/* Definitions for AGENTX_UNSIGNED64 */

#define SIZEOF_AGENTX_UNSIGNED64 (8)
#define PTR_AGENTX_UNSIGNED64_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
/* Can't define (GET_AGENTX_UNSIGNED64_B SET_AGENTX_UNSIGNED64_B) because:
   Size 64 is greater than 32.   */
#define PTR_AGENTX_UNSIGNED64_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
/* Can't define (GET_AGENTX_UNSIGNED64_L SET_AGENTX_UNSIGNED64_L) because:
   Size 64 is greater than 32.   */

/* Definitions for AGENTX_OCT_STR */

#define SIZEOF_AGENTX_OCT_STR (4)
#define PTR_AGENTX_OCT_STR_LEN(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_AGENTX_OCT_STR_LEN (4)
#define PTR_AGENTX_OCT_STR_LEN_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_OCT_STR_LEN_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_OCT_STR_LEN_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))
#define PTR_AGENTX_OCT_STR_LEN_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_OCT_STR_LEN_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_OCT_STR_LEN_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))
#define PTR_AGENTX_OCT_STR_OCTETS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define SIZEOF_AGENTX_OCT_STR_OCTETS (0)
#define PTR_AGENTX_OCT_STR_OCTETS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(4)+(_X0_))
#define GET_AGENTX_OCT_STR_OCTETS_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(4)+(_X0_))
#define SET_AGENTX_OCT_STR_OCTETS_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(4)+(_X0_), GLUE_CAST08(_V_))

/* Definitions for AGENTX_BULK */

#define SIZEOF_AGENTX_BULK (4)
#define PTR_AGENTX_BULK_NON_REP(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_AGENTX_BULK_NON_REP (2)
#define PTR_AGENTX_BULK_NON_REP_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_BULK_NON_REP_B(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_BULK_NON_REP_B(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define PTR_AGENTX_BULK_NON_REP_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_BULK_NON_REP_L(_P_)\
 GLUE_GL16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_BULK_NON_REP_L(_P_, _V_)\
 GLUE_SL16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define PTR_AGENTX_BULK_MAX_REP(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define SIZEOF_AGENTX_BULK_MAX_REP (2)
#define PTR_AGENTX_BULK_MAX_REP_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_BULK_MAX_REP_B(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_BULK_MAX_REP_B(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST16(_V_))
#define PTR_AGENTX_BULK_MAX_REP_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_BULK_MAX_REP_L(_P_)\
 GLUE_GL16((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_BULK_MAX_REP_L(_P_, _V_)\
 GLUE_SL16((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST16(_V_))

/* Definitions for AGENTX_OPEN */

#define SIZEOF_AGENTX_OPEN (8)
#define PTR_AGENTX_OPEN_TIMEOUT(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_OPEN_TIMEOUT(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_OPEN_TIMEOUT(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_AGENTX_OPEN_FILL0(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_AGENTX_OPEN_FILL0(_P_)\
 GLUE_GU08((GLUE_CAST_PTR(_P_))+(1))
#define SET_AGENTX_OPEN_FILL0(_P_, _V_)\
 GLUE_SU08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_AGENTX_OPEN_FILL1(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_OPEN_FILL1(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_OPEN_FILL1(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST16(_V_))
#define PTR_AGENTX_OPEN_OID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))

/* Definitions for AGENTX_OID */

#define SIZEOF_AGENTX_OID (4)
#define PTR_AGENTX_OID_N_SUBID(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_OID_N_SUBID(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_OID_N_SUBID(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_AGENTX_OID_PREFIX(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_AGENTX_OID_PREFIX(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_AGENTX_OID_PREFIX(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_AGENTX_OID_INCLUDE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_OID_INCLUDE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_OID_INCLUDE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define PTR_AGENTX_OID_FILL0(_P_)\
 ((GLUE_CAST_PTR(_P_))+(3))
#define GET_AGENTX_OID_FILL0(_P_)\
 GLUE_GU08((GLUE_CAST_PTR(_P_))+(3))
#define SET_AGENTX_OID_FILL0(_P_, _V_)\
 GLUE_SU08((GLUE_CAST_PTR(_P_))+(3), GLUE_CAST08(_V_))
#define PTR_AGENTX_OID_SUBIDS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define SIZEOF_AGENTX_OID_SUBIDS (0)
#define PTR_AGENTX_OID_SUBIDS_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_))
#define SIZEOF_AGENTX_OID_SUBIDS_ELT (4)
#define PTR_AGENTX_OID_SUBIDS_ELT_B(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_))
#define GET_AGENTX_OID_SUBIDS_ELT_B(_P_, _X0_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_))
#define SET_AGENTX_OID_SUBIDS_ELT_B(_P_, _X0_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_), GLUE_CAST32(_V_))
#define PTR_AGENTX_OID_SUBIDS_ELT_L(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_))
#define GET_AGENTX_OID_SUBIDS_ELT_L(_P_, _X0_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_))
#define SET_AGENTX_OID_SUBIDS_ELT_L(_P_, _X0_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_))+(4)+(4)*(_X0_), GLUE_CAST32(_V_))

/* Definitions for AGENTX_ERROR_CODE */

#define SIZEOF_AGENTX_ERROR_CODE (2)
#define GET_AGENTX_ERROR_CODE(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_ERROR_CODE(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define AGENTX_ERROR_CODE_is_OPEN_FAILED GLUE_CAST16(256L)
#define AGENTX_ERROR_CODE_is_NOT_OPEN GLUE_CAST16(257L)
#define AGENTX_ERROR_CODE_is_INDEX_WRONG_TYPE GLUE_CAST16(258L)
#define AGENTX_ERROR_CODE_is_INDEX_ALREADY_ALLOCATED GLUE_CAST16(259L)
#define AGENTX_ERROR_CODE_is_INDEX_NONE_AVAILABLE GLUE_CAST16(260L)
#define AGENTX_ERROR_CODE_is_INDEX_NOT_ALLOCATED GLUE_CAST16(261L)
#define AGENTX_ERROR_CODE_is_UNSUPPORTED_CONTEXT GLUE_CAST16(262L)
#define AGENTX_ERROR_CODE_is_DUPLICATE_REGISTRATION GLUE_CAST16(263L)
#define AGENTX_ERROR_CODE_is_UNKNOWN_REGISTRATION GLUE_CAST16(264L)
#define AGENTX_ERROR_CODE_is_UNKNOWN_AGENT_CAPS GLUE_CAST16(265L)

/* Definitions for AGENTX_RESP */

#define SIZEOF_AGENTX_RESP (8)
#define PTR_AGENTX_RESP_SYS_UPTIME(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_AGENTX_RESP_SYS_UPTIME (4)
#define PTR_AGENTX_RESP_SYS_UPTIME_B(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_RESP_SYS_UPTIME_B(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_RESP_SYS_UPTIME_B(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))
#define PTR_AGENTX_RESP_SYS_UPTIME_L(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_RESP_SYS_UPTIME_L(_P_)\
 GLUE_GL32((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_RESP_SYS_UPTIME_L(_P_, _V_)\
 GLUE_SL32((GLUE_CAST_PTR(_P_)), GLUE_CAST32(_V_))
#define PTR_AGENTX_RESP_ERROR(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define SIZEOF_AGENTX_RESP_ERROR (2)
#define PTR_AGENTX_RESP_ERROR_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_AGENTX_RESP_ERROR_B(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(4))
#define SET_AGENTX_RESP_ERROR_B(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST16(_V_))
#define PTR_AGENTX_RESP_ERROR_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_AGENTX_RESP_ERROR_L(_P_)\
 GLUE_GL16((GLUE_CAST_PTR(_P_))+(4))
#define SET_AGENTX_RESP_ERROR_L(_P_, _V_)\
 GLUE_SL16((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST16(_V_))
#define PTR_AGENTX_RESP_INDEX(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define SIZEOF_AGENTX_RESP_INDEX (2)
#define PTR_AGENTX_RESP_INDEX_B(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define GET_AGENTX_RESP_INDEX_B(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(6))
#define SET_AGENTX_RESP_INDEX_B(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(6), GLUE_CAST16(_V_))
#define PTR_AGENTX_RESP_INDEX_L(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define GET_AGENTX_RESP_INDEX_L(_P_)\
 GLUE_GL16((GLUE_CAST_PTR(_P_))+(6))
#define SET_AGENTX_RESP_INDEX_L(_P_, _V_)\
 GLUE_SL16((GLUE_CAST_PTR(_P_))+(6), GLUE_CAST16(_V_))

/* Definitions for AGENTX_REG */

#define SIZEOF_AGENTX_REG (8)
#define PTR_AGENTX_REG_TIMEOUT(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_AGENTX_REG_TIMEOUT(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_AGENTX_REG_TIMEOUT(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_AGENTX_REG_PRIORITY(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_AGENTX_REG_PRIORITY(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_AGENTX_REG_PRIORITY(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_AGENTX_REG_RANGE_SUBID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_AGENTX_REG_RANGE_SUBID(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_AGENTX_REG_RANGE_SUBID(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define PTR_AGENTX_REG_FILL0(_P_)\
 ((GLUE_CAST_PTR(_P_))+(3))
#define GET_AGENTX_REG_FILL0(_P_)\
 GLUE_GU08((GLUE_CAST_PTR(_P_))+(3))
#define SET_AGENTX_REG_FILL0(_P_, _V_)\
 GLUE_SU08((GLUE_CAST_PTR(_P_))+(3), GLUE_CAST08(_V_))
#define PTR_AGENTX_REG_REGION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))

/* Definitions for AGENTX_PDU_WITH_CONTEXT */

#define SIZEOF_AGENTX_PDU_WITH_CONTEXT (24)
#define PTR_AGENTX_PDU_WITH_CONTEXT_CONTEXT(_P_)\
 ((GLUE_CAST_PTR(_P_))+(20))

#endif /* EPILOGUE_LAYOUT_AGENTX_H */

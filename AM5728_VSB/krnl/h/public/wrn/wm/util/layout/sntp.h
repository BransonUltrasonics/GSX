/* This file was automatically generated by Epilogue Technology's
 * network datastructure layout tool.
 * 
 * DO NOT MODIFY THIS FILE BY HAND.
 * 
 * Source file information:
 */

#ifndef EPILOGUE_LAYOUT_SNTP_H
#define EPILOGUE_LAYOUT_SNTP_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif
#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#include <wrn/wm/util/layout/ldbglue.h>
#endif

/* Definitions for SNTP */

#define SIZEOF_SNTP (48)
/* Can't define PTR_SNTP_LEAP_INDICATOR because:
   Size 2 is not a multiple of 8.   */
#define GET_SNTP_LEAP_INDICATOR(_P_)\
 GLUE_CAST08(GLUE_GB08((GLUE_CAST_PTR(_P_))) >> 6)
#define SET_SNTP_LEAP_INDICATOR(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), (GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0xC0L)) | ((GLUE_CAST08(_V_) << 6) & GLUE_CAST08(0xC0L)))
#define SNTP_LEAP_INDICATOR_is_NOWARNING GLUE_CAST08(0L)
#define SNTP_LEAP_INDICATOR_is_61_IN_LAST_MINUTE GLUE_CAST08(1L)
#define SNTP_LEAP_INDICATOR_is_59_IN_LAST_MINUTE GLUE_CAST08(2L)
#define SNTP_LEAP_INDICATOR_is_ALARM GLUE_CAST08(3L)
/* Can't define PTR_SNTP_VERSION because:
   Size 3 is not a multiple of 8.   */
#define GET_SNTP_VERSION(_P_)\
 GLUE_CAST08((GLUE_GB08((GLUE_CAST_PTR(_P_))) & GLUE_CAST08(0x38L)) >> 3)
#define SET_SNTP_VERSION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), (GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0x38L)) | ((GLUE_CAST08(_V_) << 3) & GLUE_CAST08(0x38L)))
/* Can't define PTR_SNTP_MODE because:
   Size 3 is not a multiple of 8.   */
#define GET_SNTP_MODE(_P_)\
 GLUE_CAST08(GLUE_GB08((GLUE_CAST_PTR(_P_))) & GLUE_CAST08(0x7L))
#define SET_SNTP_MODE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), (GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0x7L)) | (GLUE_CAST08(_V_) & GLUE_CAST08(0x7L)))
#define SNTP_MODE_is_RESERVED GLUE_CAST08(0L)
#define SNTP_MODE_is_SYMMETRIC_ACTIVE GLUE_CAST08(1L)
#define SNTP_MODE_is_SYMMETRIC_PASSIVE GLUE_CAST08(2L)
#define SNTP_MODE_is_CLIENT GLUE_CAST08(3L)
#define SNTP_MODE_is_SERVER GLUE_CAST08(4L)
#define SNTP_MODE_is_BROADCAST GLUE_CAST08(5L)
#define SNTP_MODE_is_RESERVED_NTP GLUE_CAST08(6L)
#define SNTP_MODE_is_RESERVED_PRIVATE GLUE_CAST08(7L)
#define PTR_SNTP_STRATUM(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_SNTP_STRATUM(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_SNTP_STRATUM(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define SNTP_STRATUM_is_UNSPECIFIED GLUE_CAST08(0L)
#define SNTP_STRATUM_is_PRIMARY_REFERENCE GLUE_CAST08(1L)
#define PTR_SNTP_POLL_INTERVAL(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_SNTP_POLL_INTERVAL(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_SNTP_POLL_INTERVAL(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define PTR_SNTP_PRECISION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(3))
#define GET_SNTP_PRECISION(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(3))
#define SET_SNTP_PRECISION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(3), GLUE_CAST08(_V_))
#define PTR_SNTP_ROOT_DELAY(_P_)\
 ((GLUE_CAST_PTR(_P_))+(4))
#define GET_SNTP_ROOT_DELAY(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(4))
#define SET_SNTP_ROOT_DELAY(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(4), GLUE_CAST32(_V_))
#define PTR_SNTP_ROOT_DISPERSION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(8))
#define GET_SNTP_ROOT_DISPERSION(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(8))
#define SET_SNTP_ROOT_DISPERSION(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(8), GLUE_CAST32(_V_))
#define PTR_SNTP_REFERENCE_ID(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define GET_SNTP_REFERENCE_ID(_P_)\
 GLUE_GU32((GLUE_CAST_PTR(_P_))+(12))
#define SET_SNTP_REFERENCE_ID(_P_, _V_)\
 GLUE_SU32((GLUE_CAST_PTR(_P_))+(12), GLUE_CAST32(_V_))
#define PTR_SNTP_REFERENCE_TIMESTAMP_SECONDS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define GET_SNTP_REFERENCE_TIMESTAMP_SECONDS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(16))
#define SET_SNTP_REFERENCE_TIMESTAMP_SECONDS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(16), GLUE_CAST32(_V_))
#define PTR_SNTP_REFERENCE_TIMESTAMP_FRACTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(20))
#define GET_SNTP_REFERENCE_TIMESTAMP_FRACTIONS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(20))
#define SET_SNTP_REFERENCE_TIMESTAMP_FRACTIONS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(20), GLUE_CAST32(_V_))
#define PTR_SNTP_ORIGINATE_TIMESTAMP_SECONDS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(24))
#define GET_SNTP_ORIGINATE_TIMESTAMP_SECONDS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(24))
#define SET_SNTP_ORIGINATE_TIMESTAMP_SECONDS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(24), GLUE_CAST32(_V_))
#define PTR_SNTP_ORIGINATE_TIMESTAMP_FRACTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(28))
#define GET_SNTP_ORIGINATE_TIMESTAMP_FRACTIONS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(28))
#define SET_SNTP_ORIGINATE_TIMESTAMP_FRACTIONS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(28), GLUE_CAST32(_V_))
#define PTR_SNTP_RECEIVE_TIMESTAMP_SECONDS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(32))
#define GET_SNTP_RECEIVE_TIMESTAMP_SECONDS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(32))
#define SET_SNTP_RECEIVE_TIMESTAMP_SECONDS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(32), GLUE_CAST32(_V_))
#define PTR_SNTP_RECEIVE_TIMESTAMP_FRACTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(36))
#define GET_SNTP_RECEIVE_TIMESTAMP_FRACTIONS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(36))
#define SET_SNTP_RECEIVE_TIMESTAMP_FRACTIONS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(36), GLUE_CAST32(_V_))
#define PTR_SNTP_TRANSMIT_TIMESTAMP_SECONDS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(40))
#define GET_SNTP_TRANSMIT_TIMESTAMP_SECONDS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(40))
#define SET_SNTP_TRANSMIT_TIMESTAMP_SECONDS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(40), GLUE_CAST32(_V_))
#define PTR_SNTP_TRANSMIT_TIMESTAMP_FRACTIONS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(44))
#define GET_SNTP_TRANSMIT_TIMESTAMP_FRACTIONS(_P_)\
 GLUE_GB32((GLUE_CAST_PTR(_P_))+(44))
#define SET_SNTP_TRANSMIT_TIMESTAMP_FRACTIONS(_P_, _V_)\
 GLUE_SB32((GLUE_CAST_PTR(_P_))+(44), GLUE_CAST32(_V_))

#endif /* EPILOGUE_LAYOUT_SNTP_H */
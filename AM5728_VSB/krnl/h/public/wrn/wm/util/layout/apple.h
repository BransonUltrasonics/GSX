/* This file was automatically generated by Epilogue Technology's
 * network datastructure layout tool.
 * 
 * DO NOT MODIFY THIS FILE BY HAND.
 * 
 * Source file information:
 *  Id: apple.ldb,v 1.8 1998/02/25 15:21:38 sra Exp 
 */

#ifndef EPILOGUE_LAYOUT_APPLE_H
#define EPILOGUE_LAYOUT_APPLE_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif
#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#include <wrn/wm/util/layout/ldbglue.h>
#endif

/* Definitions for APPLE_LAP */

#define SIZEOF_APPLE_LAP (5)
#define PTR_APPLE_LAP_DESTINATION(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define GET_APPLE_LAP_DESTINATION(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_)))
#define SET_APPLE_LAP_DESTINATION(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), GLUE_CAST08(_V_))
#define PTR_APPLE_LAP_SOURCE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(1))
#define GET_APPLE_LAP_SOURCE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(1))
#define SET_APPLE_LAP_SOURCE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(1), GLUE_CAST08(_V_))
#define PTR_APPLE_LAP_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(2))
#define GET_APPLE_LAP_TYPE(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(2))
#define SET_APPLE_LAP_TYPE(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(2), GLUE_CAST08(_V_))
#define APPLE_LAP_TYPE_is_DDP_SHORT GLUE_CAST08(1L)
#define APPLE_LAP_TYPE_is_DDP_LONG GLUE_CAST08(2L)
#define APPLE_LAP_TYPE_is_ENQ GLUE_CAST08(129L)
#define APPLE_LAP_TYPE_is_ACK GLUE_CAST08(130L)
#define APPLE_LAP_TYPE_is_RTS GLUE_CAST08(132L)
#define APPLE_LAP_TYPE_is_CTS GLUE_CAST08(133L)
/* Can't define PTR_APPLE_LAP_DATA_LENGTH because:
   Size 10 is not a multiple of 8.   */
#define GET_APPLE_LAP_DATA_LENGTH(_P_)\
 GLUE_CAST16(GLUE_GB16((GLUE_CAST_PTR(_P_))+(3)) & GLUE_CAST16(0x3FFL))
#define SET_APPLE_LAP_DATA_LENGTH(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(3), (GLUE_GB16((GLUE_CAST_PTR(_P_))+(3)) & ~GLUE_CAST16(0x3FFL)) | (GLUE_CAST16(_V_) & GLUE_CAST16(0x3FFL)))
#define PTR_APPLE_LAP_DATA(_P_)\
 ((GLUE_CAST_PTR(_P_))+(5))
#define SIZEOF_APPLE_LAP_DATA (0)

#endif /* EPILOGUE_LAYOUT_APPLE_H */

/* This file was automatically generated by Epilogue Technology's
 * network datastructure layout tool.
 * 
 * DO NOT MODIFY THIS FILE BY HAND.
 * 
 * Source file information:
 *  Id: ethernet.ldb,v 1.7 1998/02/25 15:21:40 sra Exp 
 */

#ifndef EPILOGUE_LAYOUT_ETHERNET_H
#define EPILOGUE_LAYOUT_ETHERNET_H

#ifndef EPILOGUE_INSTALL_H
#include <wrn/wm/common/install.h>
#endif
#ifndef EPILOGUE_TYPES_H
#include <wrn/wm/common/types.h>
#endif
#ifndef EPILOGUE_LAYOUT_LDBGLUE_H
#include <wrn/wm/util/layout/ldbglue.h>
#endif

/* Definitions for IEEE802_HEADER */

#define SIZEOF_IEEE802_HEADER (22)
#define PTR_IEEE802_HEADER_DESTINATION_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define PTR_IEEE802_HEADER_SOURCE_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define PTR_IEEE802_HEADER_LENGTH(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define GET_IEEE802_HEADER_LENGTH(_P_)\
 GLUE_GB16((GLUE_CAST_PTR(_P_))+(12))
#define SET_IEEE802_HEADER_LENGTH(_P_, _V_)\
 GLUE_SB16((GLUE_CAST_PTR(_P_))+(12), GLUE_CAST16(_V_))
#define PTR_IEEE802_HEADER_DSAP(_P_)\
 ((GLUE_CAST_PTR(_P_))+(14))
#define GET_IEEE802_HEADER_DSAP(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(14))
#define SET_IEEE802_HEADER_DSAP(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(14), GLUE_CAST08(_V_))
#define PTR_IEEE802_HEADER_SSAP(_P_)\
 ((GLUE_CAST_PTR(_P_))+(15))
#define GET_IEEE802_HEADER_SSAP(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(15))
#define SET_IEEE802_HEADER_SSAP(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(15), GLUE_CAST08(_V_))
#define PTR_IEEE802_HEADER_CONTROL(_P_)\
 ((GLUE_CAST_PTR(_P_))+(16))
#define GET_IEEE802_HEADER_CONTROL(_P_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(16))
#define SET_IEEE802_HEADER_CONTROL(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(16), GLUE_CAST08(_V_))
#define PTR_IEEE802_HEADER_ORGANIZATION(_P_)\
 ((GLUE_CAST_PTR(_P_))+(17))
#define GET_IEEE802_HEADER_ORGANIZATION(_P_)\
 GLUE_CAST32(GLUE_GB32LS24((GLUE_CAST_PTR(_P_))+(16)) & GLUE_CAST32(0xFFFFFFL))
#define SET_IEEE802_HEADER_ORGANIZATION(_P_, _V_)\
 GLUE_SB32LS24((GLUE_CAST_PTR(_P_))+(16), (GLUE_GB32LS24((GLUE_CAST_PTR(_P_))+(16)) & ~GLUE_CAST32(0xFFFFFFL)) | (GLUE_CAST32(_V_) & GLUE_CAST32(0xFFFFFFL)))
#define PTR_IEEE802_HEADER_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(20))
#define GET_IEEE802_HEADER_TYPE(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_))+(20))
#define SET_IEEE802_HEADER_TYPE(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_))+(20), GLUE_CAST16(_V_))

/* Definitions for ETHERNET_HEADER */

#define SIZEOF_ETHERNET_HEADER (14)
#define PTR_ETHERNET_HEADER_DESTINATION_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define PTR_ETHERNET_HEADER_SOURCE_ADDRESS(_P_)\
 ((GLUE_CAST_PTR(_P_))+(6))
#define PTR_ETHERNET_HEADER_TYPE(_P_)\
 ((GLUE_CAST_PTR(_P_))+(12))
#define GET_ETHERNET_HEADER_TYPE(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_))+(12))
#define SET_ETHERNET_HEADER_TYPE(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_))+(12), GLUE_CAST16(_V_))

/* Definitions for ETHERNET_TYPE */

#define SIZEOF_ETHERNET_TYPE (2)
#define GET_ETHERNET_TYPE(_P_)\
 GLUE_GU16((GLUE_CAST_PTR(_P_)))
#define SET_ETHERNET_TYPE(_P_, _V_)\
 GLUE_SU16((GLUE_CAST_PTR(_P_)), GLUE_CAST16(_V_))
#define ETHERNET_TYPE_is_IP GLUE_OPAQUE16(GLUE_CAST16(0x800L))
#define ETHERNET_TYPE_is_CHAOS GLUE_OPAQUE16(GLUE_CAST16(0x804L))
#define ETHERNET_TYPE_is_ARP GLUE_OPAQUE16(GLUE_CAST16(0x806L))
#define ETHERNET_TYPE_is_RARP GLUE_OPAQUE16(GLUE_CAST16(0x8035L))
#define ETHERNET_TYPE_is_APPLETALK GLUE_OPAQUE16(GLUE_CAST16(0x809BL))
#define ETHERNET_TYPE_is_APPLETALK_ARP GLUE_OPAQUE16(GLUE_CAST16(0x80F3L))
#define ETHERNET_TYPE_is_IPV6 GLUE_OPAQUE16(GLUE_CAST16(0x86DDL))
#define ETHERNET_TYPE_is_LOOPBACK GLUE_OPAQUE16(GLUE_CAST16(0x9000L))

/* Definitions for ETHERNET_ADDRESS */

#define SIZEOF_ETHERNET_ADDRESS (6)
/* Can't define PTR_ETHERNET_ADDRESS_MULTICAST because:
   Size 1 is not a multiple of 8.   */
#define GET_ETHERNET_ADDRESS_MULTICAST(_P_)\
 (GLUE_GB08((GLUE_CAST_PTR(_P_))) & GLUE_CAST08(0x1L))
#define SET_ETHERNET_ADDRESS_MULTICAST(_P_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_)), ((_V_) ? GLUE_GB08((GLUE_CAST_PTR(_P_))) | GLUE_CAST08(0x1L) : GLUE_GB08((GLUE_CAST_PTR(_P_))) & ~GLUE_CAST08(0x1L)))
#define PTR_ETHERNET_ADDRESS_BYTES(_P_)\
 ((GLUE_CAST_PTR(_P_)))
#define SIZEOF_ETHERNET_ADDRESS_BYTES (6)
#define PTR_ETHERNET_ADDRESS_BYTES_ELT(_P_, _X0_)\
 ((GLUE_CAST_PTR(_P_))+(_X0_))
#define GET_ETHERNET_ADDRESS_BYTES_ELT(_P_, _X0_)\
 GLUE_GB08((GLUE_CAST_PTR(_P_))+(_X0_))
#define SET_ETHERNET_ADDRESS_BYTES_ELT(_P_, _X0_, _V_)\
 GLUE_SB08((GLUE_CAST_PTR(_P_))+(_X0_), GLUE_CAST08(_V_))

#endif /* EPILOGUE_LAYOUT_ETHERNET_H */

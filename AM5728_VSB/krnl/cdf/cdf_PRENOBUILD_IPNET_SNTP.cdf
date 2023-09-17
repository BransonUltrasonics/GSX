

/* this file is automatically generated */


Layer Layer::IPNET_SNTP {
}




/*********************************
 Component INCLUDE_IPSNTP_COMMON 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 37 50  
**********************************/
Component	INCLUDE_IPSNTP_COMMON {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 37 50 }
}


/*********************************
 Component INCLUDE_IPSNTPC_API 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 79 85  
**********************************/
Component	INCLUDE_IPSNTPC_API {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 79 85 }
}


/*********************************
 Component INCLUDE_IPSNTPS_API 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 87 95  
**********************************/
Component	INCLUDE_IPSNTPS_API {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 87 95 }
}


/*********************************
 Component INCLUDE_IPSNTPC 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 64 77  
**********************************/
Component	INCLUDE_IPSNTPC {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 64 77 }
}


/*********************************
 Component INCLUDE_IPSNTPS 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 52 62  
**********************************/
Component	INCLUDE_IPSNTPS {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 52 62 }
}


/*********************************
 Folder FOLDER_SNTP 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 24 35  
**********************************/
Folder	FOLDER_SNTP {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 24 35 }
}


/*********************************
 Layer Layer::IPNET_SNTP 
 original object sources located at 
  "C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_SNTP.tmp" 13 15  
**********************************/
Layer	Layer::IPNET_SNTP {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/cdf/cdf_PRENOBUILD_IPNET_SNTP.tmp" 13 15 }
}


/*********************************
 Parameter SNTP_LISTENING_PORT 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 100 106  
**********************************/
Parameter	SNTP_LISTENING_PORT {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 100 106 }
}


/*********************************
 Parameter SNTP_BIND_IPV6_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 116 122  
**********************************/
Parameter	SNTP_BIND_IPV6_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 116 122 }
}


/*********************************
 Parameter SNTP_BIND_IPV4_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 108 114  
**********************************/
Parameter	SNTP_BIND_IPV4_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 108 114 }
}


/*********************************
 Parameter SNTP_TASK_PRIORITY 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 280 287  
**********************************/
Parameter	SNTP_TASK_PRIORITY {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 280 287 }
}


/*********************************
 Parameter SNTP_DEBUG_ENABLE 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 290 296  
**********************************/
Parameter	SNTP_DEBUG_ENABLE {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 290 296 }
}


/*********************************
 Parameter SNTPS_STRATUM 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 140 146  
**********************************/
Parameter	SNTPS_STRATUM {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 140 146 }
}


/*********************************
 Parameter SNTPS_PRECISION 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 148 155  
**********************************/
Parameter	SNTPS_PRECISION {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 148 155 }
}


/*********************************
 Parameter SNTPS_MULTICAST_INTERVAL 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 157 164  
**********************************/
Parameter	SNTPS_MULTICAST_INTERVAL {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 157 164 }
}


/*********************************
 Parameter SNTPS_MULTICAST_TTL 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 166 172  
**********************************/
Parameter	SNTPS_MULTICAST_TTL {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 166 172 }
}


/*********************************
 Parameter SNTPS_IPV4_MULTICAST_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 174 180  
**********************************/
Parameter	SNTPS_IPV4_MULTICAST_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 174 180 }
}


/*********************************
 Parameter SNTPS_IPV6_MULTICAST_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 182 188  
**********************************/
Parameter	SNTPS_IPV6_MULTICAST_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 182 188 }
}


/*********************************
 Parameter SNTPS_LEAP_SECOND_FILENAME 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 125 138  
**********************************/
Parameter	SNTPS_LEAP_SECOND_FILENAME {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 125 138 }
}


/*********************************
 Parameter SNTPC_POLL_INTERVAL 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 191 198  
**********************************/
Parameter	SNTPC_POLL_INTERVAL {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 191 198 }
}


/*********************************
 Parameter SNTPC_POLL_COUNT 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 200 206  
**********************************/
Parameter	SNTPC_POLL_COUNT {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 200 206 }
}


/*********************************
 Parameter SNTPC_POLL_TIMEOUT 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 208 214  
**********************************/
Parameter	SNTPC_POLL_TIMEOUT {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 208 214 }
}


/*********************************
 Parameter SNTPC_PRIMARY_IPV4_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 216 222  
**********************************/
Parameter	SNTPC_PRIMARY_IPV4_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 216 222 }
}


/*********************************
 Parameter SNTPC_BACKUP_IPV4_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 224 230  
**********************************/
Parameter	SNTPC_BACKUP_IPV4_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 224 230 }
}


/*********************************
 Parameter SNTPC_MULTICAST_MODE_IF 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 232 238  
**********************************/
Parameter	SNTPC_MULTICAST_MODE_IF {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 232 238 }
}


/*********************************
 Parameter SNTPC_MULTICAST_GROUP_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 240 246  
**********************************/
Parameter	SNTPC_MULTICAST_GROUP_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 240 246 }
}


/*********************************
 Parameter SNTPC_PRIMARY_IPV6_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 248 254  
**********************************/
Parameter	SNTPC_PRIMARY_IPV6_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 248 254 }
}


/*********************************
 Parameter SNTPC_BACKUP_IPV6_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 256 262  
**********************************/
Parameter	SNTPC_BACKUP_IPV6_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 256 262 }
}


/*********************************
 Parameter SNTPC_MULTICAST_MODE_IPV6_IF 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 264 270  
**********************************/
Parameter	SNTPC_MULTICAST_MODE_IPV6_IF {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 264 270 }
}


/*********************************
 Parameter SNTPC_MULTICAST_GROUP_IPV6_ADDR 
 original object sources located at 
  "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 272 278  
**********************************/
Parameter	SNTPC_MULTICAST_GROUP_IPV6_ADDR {
		LAYER += Layer::IPNET_SNTP
		LAYER_SOURCE		{ "$(WIND_BASE)/pkgs/net/ipnet-1.1.1.3/app/sntp-1.0.0.5/cdf/02comp_ipnet_sntp.cdf" 272 278 }
}

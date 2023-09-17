 
/* this file is generated */

Layer Layer::IPNET_SNTP {

LIBRARIES += libSNTP.a

OBJECTS libSNTP.a::ipsntp.o libSNTP.a::ipsntp_cmd_sntp.o libSNTP.a::ipsntp_time.o

}


ObjectFile libSNTP.a::ipsntp.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objSNTP/
}
 
ObjectFile libSNTP.a::ipsntp_cmd_sntp.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objSNTP/
}
 
ObjectFile libSNTP.a::ipsntp_time.o { 
	PATH C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objSNTP/
}




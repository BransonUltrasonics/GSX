
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/ARMARCH7/common
#	LIBDIRBASE = krnl/$(CPU)$(CPU_OPTION_SUFFIX)/$(TOOL_COMMON_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)



include kernel_top_IPNET_NTP_src_diab_libntp.mk 
include kernel_top_IPNET_NTP_src_diab_libisc.mk 
include kernel_top_IPNET_NTP_src_diab_ntpd.mk 
include kernel_top_IPNET_NTP_src_diab_ntpdc.mk 
include kernel_top_IPNET_NTP_src_diab_ntpq.mk 
include kernel_top_IPNET_NTP_src_diab_util.mk



LIBOBJS = $(LIBOBJS_NTP)

# DEP_OBJS = (DEP_OBJS)

 



 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

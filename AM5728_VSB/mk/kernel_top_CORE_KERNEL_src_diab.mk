
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/ARMARCH7/common
#	LIBDIRBASE = krnl/$(CPU)$(CPU_OPTION_SUFFIX)/$(TOOL_COMMON_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)



include kernel_top_CORE_KERNEL_src_diab_aim.mk 
include kernel_top_CORE_KERNEL_src_diab_edr.mk 
include kernel_top_CORE_KERNEL_src_diab_mm.mk 
include kernel_top_CORE_KERNEL_src_diab_multicore.mk 
include kernel_top_CORE_KERNEL_src_diab_posix.mk 
include kernel_top_CORE_KERNEL_src_diab_rtp.mk 
include kernel_top_CORE_KERNEL_src_diab_services.mk 
include kernel_top_CORE_KERNEL_src_diab_util.mk 
include kernel_top_CORE_KERNEL_src_diab_wind.mk 
include kernel_top_CORE_KERNEL_src_diab_tls.mk 
include kernel_top_CORE_KERNEL_src_diab_demo.mk 
include kernel_top_CORE_KERNEL_src_diab_random.mk 
include kernel_top_CORE_KERNEL_src_diab_sysctl.mk



LIBOBJS = 

# DEP_OBJS = (DEP_OBJS)







################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

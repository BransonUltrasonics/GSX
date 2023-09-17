
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



include kernel_top_CORE_KERNEL_src_diab_posix_mm.mk 
include kernel_top_CORE_KERNEL_src_diab_posix_pthread.mk 
include kernel_top_CORE_KERNEL_src_diab_posix_rt.mk 
include kernel_top_CORE_KERNEL_src_diab_posix_signal.mk 
include kernel_top_CORE_KERNEL_src_diab_posix_user_group.mk 
include kernel_top_CORE_KERNEL_src_diab_posix_umask.mk 
include kernel_top_CORE_KERNEL_src_diab_posix_trace.mk



LIBOBJS = 

# DEP_OBJS = (DEP_OBJS)







################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

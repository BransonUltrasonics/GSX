
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
#	LIBDIRBASE = krnl/diab
#	LIBDIRBASE = krnl/$(TOOL_SPECIFIC_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)



include kernel_top_TILCON_KERNEL_src_diab_config.mk 
include kernel_top_TILCON_KERNEL_src_diab_engine.mk 
include kernel_top_TILCON_KERNEL_src_diab_twd.mk 
include kernel_top_TILCON_KERNEL_src_diab_widget.mk



LIBOBJS = $(LIBOBJS_tilcon)

# DEP_OBJS = (DEP_OBJS)

 



 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

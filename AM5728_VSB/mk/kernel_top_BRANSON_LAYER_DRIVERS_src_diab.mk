
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



include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_spi.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_core.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_i2c.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_resource.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_pll.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_pmic.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_ehrpwm.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_eqep.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_pwmss.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_net.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_storage.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_McSPI.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_cifx.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_am5728ehrpwm.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_am5728pwmss.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_emmc.mk 
include kernel_top_BRANSON_LAYER_DRIVERS_src_diab_uart2.mk



LIBOBJS = $(LIBOBJS_branson_drv)

# DEP_OBJS = (DEP_OBJS)

 



 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

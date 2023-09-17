
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






LIBOBJS = $(LIBOBJS_vxbus)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_vxbus_kernel_top_VXBUS_DRV_src_diab_gpio =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxbus/vxbFdtKinetisGpio.o 

arcmdfile_krnl_ARMARCH7_common_vxbus : $(LIBOBJS_krnl_ARMARCH7_common_vxbus_kernel_top_VXBUS_DRV_src_diab_gpio)

LIBOBJS_krnl_ARMARCH7_common_vxbus_VXBUS_DRV += $(LIBOBJS_krnl_ARMARCH7_common_vxbus_kernel_top_VXBUS_DRV_src_diab_gpio)

LIBOBJS_krnl_ARMARCH7_common_vxbus += $(LIBOBJS_krnl_ARMARCH7_common_vxbus_kernel_top_VXBUS_DRV_src_diab_gpio)

__OBJS_TO_CLEAN_VXBUS_DRV += $(LIBOBJS_krnl_ARMARCH7_common_vxbus_kernel_top_VXBUS_DRV_src_diab_gpio) $(LIBOBJS_krnl_ARMARCH7_common_vxbus_kernel_top_VXBUS_DRV_src_diab_gpio:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_vxbus

_DONEBASE_LIB_krnl_ARMARCH7_common_vxbus = TRUE

# LIBBASES += vxbus

__LIBS_TO_BUILD += krnl_ARMARCH7_common_vxbus

__LIBS_TO_BUILD_VXBUS_DRV += krnl_ARMARCH7_common_vxbus

__BUILT_LIBS += krnl_ARMARCH7_common_vxbus

__LIB_krnl_ARMARCH7_common_vxbus = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objvxbus :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libvxbus$(OPT).a libvxbus$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libvxbus.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_vxbus

arcmdfile_krnl_ARMARCH7_common_vxbus :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_vxbus))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a : arcmdfile_krnl_ARMARCH7_common_vxbus
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_vxbus


clean _layer_clean_VXBUS_DRV : _clean_LIB_BASE_krnl_ARMARCH7_common_vxbus

_clean_LIB_BASE_krnl_ARMARCH7_common_vxbus :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a


endif

ifndef _DONEBASE_LIB_VXBUS_DRV_krnl_ARMARCH7_common_vxbus

_DONEBASE_LIB_VXBUS_DRV_krnl_ARMARCH7_common_vxbus = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerVXBUS_DRV_libvxbus.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerVXBUS_DRV_libvxbus.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,VXBUS_DRV,libvxbus.a,$(LIBOBJS_krnl_ARMARCH7_common_vxbus)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libvxbus$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_vxbus)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

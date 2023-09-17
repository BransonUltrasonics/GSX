
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






LIBOBJS = $(LIBOBJS_arch)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_arch_kernel_top_CORE_KERNEL_genh_offset_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objarch/offsets.o 

arcmdfile_krnl_ARMARCH7_common_arch : $(LIBOBJS_krnl_ARMARCH7_common_arch_kernel_top_CORE_KERNEL_genh_offset_diab)

LIBOBJS_krnl_ARMARCH7_common_arch_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_arch_kernel_top_CORE_KERNEL_genh_offset_diab)

LIBOBJS_krnl_ARMARCH7_common_arch += $(LIBOBJS_krnl_ARMARCH7_common_arch_kernel_top_CORE_KERNEL_genh_offset_diab)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_arch_kernel_top_CORE_KERNEL_genh_offset_diab) $(LIBOBJS_krnl_ARMARCH7_common_arch_kernel_top_CORE_KERNEL_genh_offset_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_arch

_DONEBASE_LIB_krnl_ARMARCH7_common_arch = TRUE

# LIBBASES += arch

__LIBS_TO_BUILD += krnl_ARMARCH7_common_arch

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_arch

__BUILT_LIBS += krnl_ARMARCH7_common_arch

__LIB_krnl_ARMARCH7_common_arch = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objarch :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libarch$(OPT).a libarch$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libarch.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_arch

arcmdfile_krnl_ARMARCH7_common_arch :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_arch))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a : arcmdfile_krnl_ARMARCH7_common_arch
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_arch


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_arch

_clean_LIB_BASE_krnl_ARMARCH7_common_arch :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_arch

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_arch = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libarch.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libarch.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libarch.a,$(LIBOBJS_krnl_ARMARCH7_common_arch)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libarch$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_arch)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 


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



include kernel_top_CORE_KERNEL_src_diab_multicore_lock.mk



LIBOBJS = $(LIBOBJS_multicore)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_multicore_kernel_top_CORE_KERNEL_src_diab_multicore =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objmulticore/cpuset.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objmulticore/vxCpuLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objmulticore/vxAtomicScLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objmulticore/vxAtomicLib.o 

arcmdfile_krnl_ARMARCH7_common_multicore : $(LIBOBJS_krnl_ARMARCH7_common_multicore_kernel_top_CORE_KERNEL_src_diab_multicore)

LIBOBJS_krnl_ARMARCH7_common_multicore_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_multicore_kernel_top_CORE_KERNEL_src_diab_multicore)

LIBOBJS_krnl_ARMARCH7_common_multicore += $(LIBOBJS_krnl_ARMARCH7_common_multicore_kernel_top_CORE_KERNEL_src_diab_multicore)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_multicore_kernel_top_CORE_KERNEL_src_diab_multicore) $(LIBOBJS_krnl_ARMARCH7_common_multicore_kernel_top_CORE_KERNEL_src_diab_multicore:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_multicore

_DONEBASE_LIB_krnl_ARMARCH7_common_multicore = TRUE

# LIBBASES += multicore

__LIBS_TO_BUILD += krnl_ARMARCH7_common_multicore

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_multicore

__BUILT_LIBS += krnl_ARMARCH7_common_multicore

__LIB_krnl_ARMARCH7_common_multicore = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objmulticore :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libmulticore$(OPT).a libmulticore$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libmulticore.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_multicore

arcmdfile_krnl_ARMARCH7_common_multicore :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_multicore))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a : arcmdfile_krnl_ARMARCH7_common_multicore
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_multicore


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_multicore

_clean_LIB_BASE_krnl_ARMARCH7_common_multicore :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_multicore

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_multicore = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libmulticore.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libmulticore.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libmulticore.a,$(LIBOBJS_krnl_ARMARCH7_common_multicore)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libmulticore$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_multicore)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
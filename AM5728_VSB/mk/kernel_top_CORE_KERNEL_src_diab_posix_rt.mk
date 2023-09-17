
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






LIBOBJS = $(LIBOBJS_pxrt)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_pxrt_kernel_top_CORE_KERNEL_src_diab_posix_rt =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/clockLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/mqPxLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/mqPxShow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/semPxShow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/semPxLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/timerLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/timerShow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/rtFuncBind.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/timerOpen.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/clockScLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/mqSemPxScLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt/timerScLib.o 

arcmdfile_krnl_ARMARCH7_common_pxrt : $(LIBOBJS_krnl_ARMARCH7_common_pxrt_kernel_top_CORE_KERNEL_src_diab_posix_rt)

LIBOBJS_krnl_ARMARCH7_common_pxrt_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_pxrt_kernel_top_CORE_KERNEL_src_diab_posix_rt)

LIBOBJS_krnl_ARMARCH7_common_pxrt += $(LIBOBJS_krnl_ARMARCH7_common_pxrt_kernel_top_CORE_KERNEL_src_diab_posix_rt)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_pxrt_kernel_top_CORE_KERNEL_src_diab_posix_rt) $(LIBOBJS_krnl_ARMARCH7_common_pxrt_kernel_top_CORE_KERNEL_src_diab_posix_rt:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_pxrt

_DONEBASE_LIB_krnl_ARMARCH7_common_pxrt = TRUE

# LIBBASES += pxrt

__LIBS_TO_BUILD += krnl_ARMARCH7_common_pxrt

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_pxrt

__BUILT_LIBS += krnl_ARMARCH7_common_pxrt

__LIB_krnl_ARMARCH7_common_pxrt = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxrt :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libpxrt$(OPT).a libpxrt$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libpxrt.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_pxrt

arcmdfile_krnl_ARMARCH7_common_pxrt :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_pxrt))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a : arcmdfile_krnl_ARMARCH7_common_pxrt
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_pxrt


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_pxrt

_clean_LIB_BASE_krnl_ARMARCH7_common_pxrt :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_pxrt

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_pxrt = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libpxrt.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libpxrt.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libpxrt.a,$(LIBOBJS_krnl_ARMARCH7_common_pxrt)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxrt$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_pxrt)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

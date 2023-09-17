
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






LIBOBJS = $(LIBOBJS_pxtrace)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_pxtrace_kernel_top_CORE_KERNEL_src_diab_posix_trace =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace/pxTraceStreamBufferLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace/pxTraceStreamLogLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace/pxTraceStreamLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace/pxTraceHashTblLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace/pxTraceEventSetLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace/pxTraceScLib.o 

arcmdfile_krnl_ARMARCH7_common_pxtrace : $(LIBOBJS_krnl_ARMARCH7_common_pxtrace_kernel_top_CORE_KERNEL_src_diab_posix_trace)

LIBOBJS_krnl_ARMARCH7_common_pxtrace_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_pxtrace_kernel_top_CORE_KERNEL_src_diab_posix_trace)

LIBOBJS_krnl_ARMARCH7_common_pxtrace += $(LIBOBJS_krnl_ARMARCH7_common_pxtrace_kernel_top_CORE_KERNEL_src_diab_posix_trace)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_pxtrace_kernel_top_CORE_KERNEL_src_diab_posix_trace) $(LIBOBJS_krnl_ARMARCH7_common_pxtrace_kernel_top_CORE_KERNEL_src_diab_posix_trace:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_pxtrace

_DONEBASE_LIB_krnl_ARMARCH7_common_pxtrace = TRUE

# LIBBASES += pxtrace

__LIBS_TO_BUILD += krnl_ARMARCH7_common_pxtrace

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_pxtrace

__BUILT_LIBS += krnl_ARMARCH7_common_pxtrace

__LIB_krnl_ARMARCH7_common_pxtrace = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpxtrace :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libpxtrace$(OPT).a libpxtrace$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libpxtrace.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_pxtrace

arcmdfile_krnl_ARMARCH7_common_pxtrace :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_pxtrace))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a : arcmdfile_krnl_ARMARCH7_common_pxtrace
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_pxtrace


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_pxtrace

_clean_LIB_BASE_krnl_ARMARCH7_common_pxtrace :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_pxtrace

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_pxtrace = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libpxtrace.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libpxtrace.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libpxtrace.a,$(LIBOBJS_krnl_ARMARCH7_common_pxtrace)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpxtrace$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_pxtrace)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

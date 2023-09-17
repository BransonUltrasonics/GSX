
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






LIBOBJS = $(LIBOBJS_edr)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_edr_kernel_top_CORE_KERNEL_src_diab_edr =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrFuncBind.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrPrintfLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrSysDbgLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/pmLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/sysDbgLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrErrLogLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrShow.o  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr/edrScLib.o 

arcmdfile_krnl_ARMARCH7_common_edr : $(LIBOBJS_krnl_ARMARCH7_common_edr_kernel_top_CORE_KERNEL_src_diab_edr)

LIBOBJS_krnl_ARMARCH7_common_edr_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_edr_kernel_top_CORE_KERNEL_src_diab_edr)

LIBOBJS_krnl_ARMARCH7_common_edr += $(LIBOBJS_krnl_ARMARCH7_common_edr_kernel_top_CORE_KERNEL_src_diab_edr)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_edr_kernel_top_CORE_KERNEL_src_diab_edr) $(LIBOBJS_krnl_ARMARCH7_common_edr_kernel_top_CORE_KERNEL_src_diab_edr:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_edr

_DONEBASE_LIB_krnl_ARMARCH7_common_edr = TRUE

# LIBBASES += edr

__LIBS_TO_BUILD += krnl_ARMARCH7_common_edr

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_edr

__BUILT_LIBS += krnl_ARMARCH7_common_edr

__LIB_krnl_ARMARCH7_common_edr = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objedr :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libedr$(OPT).a libedr$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libedr.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_edr

arcmdfile_krnl_ARMARCH7_common_edr :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_edr))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a : arcmdfile_krnl_ARMARCH7_common_edr
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_edr


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_edr

_clean_LIB_BASE_krnl_ARMARCH7_common_edr :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_edr

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_edr = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libedr.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libedr.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libedr.a,$(LIBOBJS_krnl_ARMARCH7_common_edr)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libedr$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_edr)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

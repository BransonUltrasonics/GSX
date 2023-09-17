
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






LIBOBJS = $(LIBOBJS_coredump)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_coredump_kernel_top_CORE_DUMP_src_diab_arch_arm =  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objcoredump/coreDumpArmLib.o 

arcmdfile_krnl_ARMARCH7_common_coredump : $(LIBOBJS_krnl_ARMARCH7_common_coredump_kernel_top_CORE_DUMP_src_diab_arch_arm)

LIBOBJS_krnl_ARMARCH7_common_coredump_CORE_DUMP += $(LIBOBJS_krnl_ARMARCH7_common_coredump_kernel_top_CORE_DUMP_src_diab_arch_arm)

LIBOBJS_krnl_ARMARCH7_common_coredump += $(LIBOBJS_krnl_ARMARCH7_common_coredump_kernel_top_CORE_DUMP_src_diab_arch_arm)

__OBJS_TO_CLEAN_CORE_DUMP += $(LIBOBJS_krnl_ARMARCH7_common_coredump_kernel_top_CORE_DUMP_src_diab_arch_arm) $(LIBOBJS_krnl_ARMARCH7_common_coredump_kernel_top_CORE_DUMP_src_diab_arch_arm:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_coredump

_DONEBASE_LIB_krnl_ARMARCH7_common_coredump = TRUE

# LIBBASES += coredump

__LIBS_TO_BUILD += krnl_ARMARCH7_common_coredump

__LIBS_TO_BUILD_CORE_DUMP += krnl_ARMARCH7_common_coredump

__BUILT_LIBS += krnl_ARMARCH7_common_coredump

__LIB_krnl_ARMARCH7_common_coredump = C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/objcoredump :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common && $(NM) libcoredump$(OPT).a libcoredump$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libcoredump.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_coredump

arcmdfile_krnl_ARMARCH7_common_coredump :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_coredump))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a : arcmdfile_krnl_ARMARCH7_common_coredump
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_coredump


clean _layer_clean_CORE_DUMP : _clean_LIB_BASE_krnl_ARMARCH7_common_coredump

_clean_LIB_BASE_krnl_ARMARCH7_common_coredump :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_DUMP_krnl_ARMARCH7_common_coredump

_DONEBASE_LIB_CORE_DUMP_krnl_ARMARCH7_common_coredump = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_DUMP_libcoredump.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/layerCORE_DUMP_libcoredump.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_DUMP,libcoredump.a,$(LIBOBJS_krnl_ARMARCH7_common_coredump)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM4377_VSB/krnl/ARMARCH7/common/libcoredump$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_coredump)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 


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






LIBOBJS = $(LIBOBJS_umask)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_umask_kernel_top_CORE_KERNEL_src_diab_posix_umask =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objumask/umaskLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objumask/umaskScLib.o 

arcmdfile_krnl_ARMARCH7_common_umask : $(LIBOBJS_krnl_ARMARCH7_common_umask_kernel_top_CORE_KERNEL_src_diab_posix_umask)

LIBOBJS_krnl_ARMARCH7_common_umask_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_umask_kernel_top_CORE_KERNEL_src_diab_posix_umask)

LIBOBJS_krnl_ARMARCH7_common_umask += $(LIBOBJS_krnl_ARMARCH7_common_umask_kernel_top_CORE_KERNEL_src_diab_posix_umask)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_umask_kernel_top_CORE_KERNEL_src_diab_posix_umask) $(LIBOBJS_krnl_ARMARCH7_common_umask_kernel_top_CORE_KERNEL_src_diab_posix_umask:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_umask

_DONEBASE_LIB_krnl_ARMARCH7_common_umask = TRUE

# LIBBASES += umask

__LIBS_TO_BUILD += krnl_ARMARCH7_common_umask

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_umask

__BUILT_LIBS += krnl_ARMARCH7_common_umask

__LIB_krnl_ARMARCH7_common_umask = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objumask :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libumask$(OPT).a libumask$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libumask.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_umask

arcmdfile_krnl_ARMARCH7_common_umask :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_umask))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a : arcmdfile_krnl_ARMARCH7_common_umask
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_umask


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_umask

_clean_LIB_BASE_krnl_ARMARCH7_common_umask :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_umask

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_umask = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libumask.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libumask.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libumask.a,$(LIBOBJS_krnl_ARMARCH7_common_umask)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libumask$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_umask)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
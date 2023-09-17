
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






LIBOBJS = $(LIBOBJS_pthread)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_pthread_kernel_top_CORE_KERNEL_src_diab_posix_pthread =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread/pthreadFuncBind.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread/pthreadLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread/_pthreadLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread/schedPxLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread/_schedPxLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread/schedPxScLib.o 

arcmdfile_krnl_ARMARCH7_common_pthread : $(LIBOBJS_krnl_ARMARCH7_common_pthread_kernel_top_CORE_KERNEL_src_diab_posix_pthread)

LIBOBJS_krnl_ARMARCH7_common_pthread_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_pthread_kernel_top_CORE_KERNEL_src_diab_posix_pthread)

LIBOBJS_krnl_ARMARCH7_common_pthread += $(LIBOBJS_krnl_ARMARCH7_common_pthread_kernel_top_CORE_KERNEL_src_diab_posix_pthread)

__OBJS_TO_CLEAN_CORE_KERNEL += $(LIBOBJS_krnl_ARMARCH7_common_pthread_kernel_top_CORE_KERNEL_src_diab_posix_pthread) $(LIBOBJS_krnl_ARMARCH7_common_pthread_kernel_top_CORE_KERNEL_src_diab_posix_pthread:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_pthread

_DONEBASE_LIB_krnl_ARMARCH7_common_pthread = TRUE

# LIBBASES += pthread

__LIBS_TO_BUILD += krnl_ARMARCH7_common_pthread

__LIBS_TO_BUILD_CORE_KERNEL += krnl_ARMARCH7_common_pthread

__BUILT_LIBS += krnl_ARMARCH7_common_pthread

__LIB_krnl_ARMARCH7_common_pthread = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objpthread :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libpthread$(OPT).a libpthread$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libpthread.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_pthread

arcmdfile_krnl_ARMARCH7_common_pthread :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_pthread))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a : arcmdfile_krnl_ARMARCH7_common_pthread
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_pthread


clean _layer_clean_CORE_KERNEL : _clean_LIB_BASE_krnl_ARMARCH7_common_pthread

_clean_LIB_BASE_krnl_ARMARCH7_common_pthread :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a


endif

ifndef _DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_pthread

_DONEBASE_LIB_CORE_KERNEL_krnl_ARMARCH7_common_pthread = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libpthread.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerCORE_KERNEL_libpthread.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,CORE_KERNEL,libpthread.a,$(LIBOBJS_krnl_ARMARCH7_common_pthread)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libpthread$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_pthread)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

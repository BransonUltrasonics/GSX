
#
# This file is automatically generated by mk/krnl/defs.fastlibgen.mk .
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : gnu
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = krnl/gnu
#	LIBDIRBASE = krnl/$(TOOL_SPECIFIC_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)



include kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts_demangler.mk



LIBOBJS = $(LIBOBJS_cplus)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_gnu_cplus_kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus/cplusLib.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus/cplusCore.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus/cplusInit.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus/cplusStr.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus/cplusXtors.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus/cplusUsr.o 

arcmdfile_krnl_gnu_cplus : $(LIBOBJS_krnl_gnu_cplus_kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts)

LIBOBJS_krnl_gnu_cplus_LANG_LIB_CPLUS_CPLUS_KERNEL += $(LIBOBJS_krnl_gnu_cplus_kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts)

LIBOBJS_krnl_gnu_cplus += $(LIBOBJS_krnl_gnu_cplus_kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts)

__OBJS_TO_CLEAN_LANG_LIB_CPLUS_CPLUS_KERNEL += $(LIBOBJS_krnl_gnu_cplus_kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts) $(LIBOBJS_krnl_gnu_cplus_kernel_top_LANG_LIB_CPLUS_CPLUS_KERNEL_src_gnu_rts:.o=.d)

ifndef _DONEBASE_LIB_krnl_gnu_cplus

_DONEBASE_LIB_krnl_gnu_cplus = TRUE

# LIBBASES += cplus

__LIBS_TO_BUILD += krnl_gnu_cplus

__LIBS_TO_BUILD_LANG_LIB_CPLUS_CPLUS_KERNEL += krnl_gnu_cplus

__BUILT_LIBS += krnl_gnu_cplus

__LIB_krnl_gnu_cplus = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/objcplus :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus.cdf

# need to pass the lib.a file twice to nmarm to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu && $(NM) libcplus$(OPT).a libcplus$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libcplus.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_gnu_cplus

arcmdfile_krnl_gnu_cplus :
	$(file >$@,$(LIBOBJS_krnl_gnu_cplus))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a : arcmdfile_krnl_gnu_cplus
	C:/WindRiverSR500/compilers/gnu-4.8.1.9/x86-win32/bin/ararm crusv $@ @arcmdfile_krnl_gnu_cplus


clean _layer_clean_LANG_LIB_CPLUS_CPLUS_KERNEL : _clean_LIB_BASE_krnl_gnu_cplus

_clean_LIB_BASE_krnl_gnu_cplus :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a


endif

ifndef _DONEBASE_LIB_LANG_LIB_CPLUS_CPLUS_KERNEL_krnl_gnu_cplus

_DONEBASE_LIB_LANG_LIB_CPLUS_CPLUS_KERNEL_krnl_gnu_cplus = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/layerLANG_LIB_CPLUS_CPLUS_KERNEL_libcplus.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/layerLANG_LIB_CPLUS_CPLUS_KERNEL_libcplus.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,LANG_LIB_CPLUS_CPLUS_KERNEL,libcplus.a,$(LIBOBJS_krnl_gnu_cplus)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/gnu/libcplus$(OPT).a : $(LIBOBJS_krnl_gnu_cplus)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

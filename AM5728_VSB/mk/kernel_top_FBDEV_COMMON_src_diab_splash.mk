
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






LIBOBJS = $(LIBOBJS_gfxSplash)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_gfxSplash_kernel_top_FBDEV_COMMON_src_diab_splash =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxSplash/gfxSplash.o 

arcmdfile_krnl_ARMARCH7_common_gfxSplash : $(LIBOBJS_krnl_ARMARCH7_common_gfxSplash_kernel_top_FBDEV_COMMON_src_diab_splash)

LIBOBJS_krnl_ARMARCH7_common_gfxSplash_FBDEV_COMMON += $(LIBOBJS_krnl_ARMARCH7_common_gfxSplash_kernel_top_FBDEV_COMMON_src_diab_splash)

LIBOBJS_krnl_ARMARCH7_common_gfxSplash += $(LIBOBJS_krnl_ARMARCH7_common_gfxSplash_kernel_top_FBDEV_COMMON_src_diab_splash)

__OBJS_TO_CLEAN_FBDEV_COMMON += $(LIBOBJS_krnl_ARMARCH7_common_gfxSplash_kernel_top_FBDEV_COMMON_src_diab_splash) $(LIBOBJS_krnl_ARMARCH7_common_gfxSplash_kernel_top_FBDEV_COMMON_src_diab_splash:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_gfxSplash

_DONEBASE_LIB_krnl_ARMARCH7_common_gfxSplash = TRUE

# LIBBASES += gfxSplash

__LIBS_TO_BUILD += krnl_ARMARCH7_common_gfxSplash

__LIBS_TO_BUILD_FBDEV_COMMON += krnl_ARMARCH7_common_gfxSplash

__BUILT_LIBS += krnl_ARMARCH7_common_gfxSplash

__LIB_krnl_ARMARCH7_common_gfxSplash = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxSplash :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libgfxSplash$(OPT).a libgfxSplash$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libgfxSplash.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_gfxSplash

arcmdfile_krnl_ARMARCH7_common_gfxSplash :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_gfxSplash))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a : arcmdfile_krnl_ARMARCH7_common_gfxSplash
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_gfxSplash


clean _layer_clean_FBDEV_COMMON : _clean_LIB_BASE_krnl_ARMARCH7_common_gfxSplash

_clean_LIB_BASE_krnl_ARMARCH7_common_gfxSplash :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a


endif

ifndef _DONEBASE_LIB_FBDEV_COMMON_krnl_ARMARCH7_common_gfxSplash

_DONEBASE_LIB_FBDEV_COMMON_krnl_ARMARCH7_common_gfxSplash = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerFBDEV_COMMON_libgfxSplash.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerFBDEV_COMMON_libgfxSplash.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,FBDEV_COMMON,libgfxSplash.a,$(LIBOBJS_krnl_ARMARCH7_common_gfxSplash)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxSplash$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_gfxSplash)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

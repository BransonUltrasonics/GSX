
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






LIBOBJS = $(LIBOBJS_gfxFreeType2)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_kernel_top_FONT_FREETYPE2_src_diab_psaux =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxFreeType2/psaux.o 

arcmdfile_krnl_ARMARCH7_common_gfxFreeType2 : $(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_kernel_top_FONT_FREETYPE2_src_diab_psaux)

LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_FONT_FREETYPE2 += $(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_kernel_top_FONT_FREETYPE2_src_diab_psaux)

LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2 += $(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_kernel_top_FONT_FREETYPE2_src_diab_psaux)

__OBJS_TO_CLEAN_FONT_FREETYPE2 += $(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_kernel_top_FONT_FREETYPE2_src_diab_psaux) $(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2_kernel_top_FONT_FREETYPE2_src_diab_psaux:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_gfxFreeType2

_DONEBASE_LIB_krnl_ARMARCH7_common_gfxFreeType2 = TRUE

# LIBBASES += gfxFreeType2

__LIBS_TO_BUILD += krnl_ARMARCH7_common_gfxFreeType2

__LIBS_TO_BUILD_FONT_FREETYPE2 += krnl_ARMARCH7_common_gfxFreeType2

__BUILT_LIBS += krnl_ARMARCH7_common_gfxFreeType2

__LIB_krnl_ARMARCH7_common_gfxFreeType2 = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxFreeType2 :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libgfxFreeType2$(OPT).a libgfxFreeType2$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libgfxFreeType2.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_gfxFreeType2

arcmdfile_krnl_ARMARCH7_common_gfxFreeType2 :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a : arcmdfile_krnl_ARMARCH7_common_gfxFreeType2
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_gfxFreeType2


clean _layer_clean_FONT_FREETYPE2 : _clean_LIB_BASE_krnl_ARMARCH7_common_gfxFreeType2

_clean_LIB_BASE_krnl_ARMARCH7_common_gfxFreeType2 :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a


endif

ifndef _DONEBASE_LIB_FONT_FREETYPE2_krnl_ARMARCH7_common_gfxFreeType2

_DONEBASE_LIB_FONT_FREETYPE2_krnl_ARMARCH7_common_gfxFreeType2 = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerFONT_FREETYPE2_libgfxFreeType2.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerFONT_FREETYPE2_libgfxFreeType2.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,FONT_FREETYPE2,libgfxFreeType2.a,$(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxFreeType2$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_gfxFreeType2)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 


#
# This file is automatically generated by mk/usr/defs.fastlibgen.mk.
#
# build configuration :
#	VSB : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB
#	CPU : ARMARCH7
#	TOOL : diab
#	FP : vector
# 	ENDIAN : little
#	LIB_DIR_TAG = 
#	LIBDIRBASE = common
#	LIBDIRBASE = $(TOOL_COMMON_DIR)$(LIB_DIR_TAG)






ifeq (,)



endif


# DEP_OBJS = (DEP_OBJS)

# LIB_BASE_NAMES = gfxFreeType2

 
_STATIC_LIBOBJS_common_gfxFreeType2_user_static_top_FONT_FREETYPE2_src_diab_pcf = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objgfxFreeType2/pcf.o

_usrstarcmdfile_common_gfxFreeType2 : $(_STATIC_LIBOBJS_common_gfxFreeType2_user_static_top_FONT_FREETYPE2_src_diab_pcf)

_STATIC_LIBOBJS_common_gfxFreeType2 += $(_STATIC_LIBOBJS_common_gfxFreeType2_user_static_top_FONT_FREETYPE2_src_diab_pcf)

__OBJS_TO_CLEAN_FONT_FREETYPE2 += $(_STATIC_LIBOBJS_common_gfxFreeType2_user_static_top_FONT_FREETYPE2_src_diab_pcf)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.a : $(_STATIC_LIBOBJS_common_gfxFreeType2_user_static_top_FONT_FREETYPE2_src_diab_pcf)

ifndef _DONEBASE_USRST_LIB_common_gfxFreeType2

_DONEBASE_USRST_LIB_common_gfxFreeType2 = TRUE

# LIBBASES += gfxFreeType2

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common:
#	mkdir -p $@

# C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objgfxFreeType2 :
#	mkdir -p $@

usrstlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.a

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.a : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common

__STATIC_BUILT_LIBS += common_gfxFreeType2

__STLIB_common_gfxFreeType2 = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.a

arcmdfiles : _usrstarcmdfile_common_gfxFreeType2

_usrstarcmdfile_common_gfxFreeType2 :
	$(file >$@,$(sort $(_STATIC_LIBOBJS_common_gfxFreeType2)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.a : _usrstarcmdfile_common_gfxFreeType2 | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @_usrstarcmdfile_common_gfxFreeType2

clean  _layer_clean_FONT_FREETYPE2 : _clean_USRST_LIB_BASE_common_gfxFreeType2

_clean_USRST_LIB_BASE_common_gfxFreeType2 :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.a

endif


 

 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 

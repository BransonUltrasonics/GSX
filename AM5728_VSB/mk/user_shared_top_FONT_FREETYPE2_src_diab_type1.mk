
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

 

 
_SHARED_LIBOBJS_common_gfxFreeType2_user_shared_top_FONT_FREETYPE2_src_diab_type1 = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/objgfxFreeType2/type1.sho

_usrsharcmdfile_common_gfxFreeType2 : $(_SHARED_LIBOBJS_common_gfxFreeType2_user_shared_top_FONT_FREETYPE2_src_diab_type1)

_SHARED_LIBOBJS_common_gfxFreeType2 += $(_SHARED_LIBOBJS_common_gfxFreeType2_user_shared_top_FONT_FREETYPE2_src_diab_type1)

__OBJS_TO_CLEAN_FONT_FREETYPE2 += $(_SHARED_LIBOBJS_common_gfxFreeType2_user_shared_top_FONT_FREETYPE2_src_diab_type1)

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.so : $(_SHARED_LIBOBJS_common_gfxFreeType2_user_shared_top_FONT_FREETYPE2_src_diab_type1)

ifndef _DONEBASE_USRSH_LIB_common_gfxFreeType2

_DONEBASE_USRSH_LIB_common_gfxFreeType2 = TRUE

# LIBBASES += gfxFreeType2



usrshlib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libgfxFreeType2.so.1

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libgfxFreeType2.so.1 : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin

__DIR_TARGETS += C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root 

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/root/diab/bin/libgfxFreeType2.so.1 : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.so
	cp C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.so $@

# XXX gfxFreeType2
# false
# 



C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.so : | C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common 

arcmdfiles : _usrsharcmdfile_common_gfxFreeType2

_usrsharcmdfile_common_gfxFreeType2 :
	$(file >$@,$(sort $(_SHARED_LIBOBJS_common_gfxFreeType2)))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.so : | usrstlib
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dplus -tARMCORTEXA15MV:rtp7 -Y I,+C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public/c++03:C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public:C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/../../include/diab -Xansi -WDVSB_DIR=C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB  -W:c++:.CPP  -Xfp-fast  -XO -w   -IC:/WindRiverSR500/vxworks-7/pkgs/ui/font/freetype2/h -D_VX_CPU=_VX_ARMARCH7 -D_VX_TOOL_FAMILY=diab -D_VX_TOOL=diab -DARMEL -DARMEL -DINET -DINET6   -D_VSB_CONFIG_FILE=\"C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/h/config/vsbConfig.h\"  -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/share/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/system -IC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/h/public -I.  -Xpic -Xshared -Xdynamic -soname=libgfxFreeType2.so.1 -o  $@ -Wl,@_usrsharcmdfile_common_gfxFreeType2  -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/diab/PIC -LC:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/PIC  


clean _layer_clean_FONT_FREETYPE2 : _clean_USRSH_LIB_BASE_common_gfxFreeType2

_clean_USRSH_LIB_BASE_common_gfxFreeType2 :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/usr/lib/common/libgfxFreeType2.so

endif


 

################
# rules defined locally in Makefile
##





################

objs : $(LIBOBJS)
 
# this is the end of the file 


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






LIBOBJS = $(LIBOBJS_gfxTiDssFb)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_kernel_top_BRANSON_LAYER_TIDSSFB_src_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxTiDssFb/gfxTiDssIosDrv.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxTiDssFb/gfxTiDss.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxTiDssFb/gfxTiAm35xx.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxTiDssFb/gfxTiAm37xx.o 

arcmdfile_krnl_ARMARCH7_common_gfxTiDssFb : $(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_kernel_top_BRANSON_LAYER_TIDSSFB_src_diab)

LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_BRANSON_LAYER_TIDSSFB += $(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_kernel_top_BRANSON_LAYER_TIDSSFB_src_diab)

LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb += $(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_kernel_top_BRANSON_LAYER_TIDSSFB_src_diab)

__OBJS_TO_CLEAN_BRANSON_LAYER_TIDSSFB += $(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_kernel_top_BRANSON_LAYER_TIDSSFB_src_diab) $(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb_kernel_top_BRANSON_LAYER_TIDSSFB_src_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_ARMARCH7_common_gfxTiDssFb

_DONEBASE_LIB_krnl_ARMARCH7_common_gfxTiDssFb = TRUE

# LIBBASES += gfxTiDssFb

__LIBS_TO_BUILD += krnl_ARMARCH7_common_gfxTiDssFb

__LIBS_TO_BUILD_BRANSON_LAYER_TIDSSFB += krnl_ARMARCH7_common_gfxTiDssFb

__BUILT_LIBS += krnl_ARMARCH7_common_gfxTiDssFb

__LIB_krnl_ARMARCH7_common_gfxTiDssFb = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/objgfxTiDssFb :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common && $(NM) libgfxTiDssFb$(OPT).a libgfxTiDssFb$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libgfxTiDssFb.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_ARMARCH7_common_gfxTiDssFb

arcmdfile_krnl_ARMARCH7_common_gfxTiDssFb :
	$(file >$@,$(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a : arcmdfile_krnl_ARMARCH7_common_gfxTiDssFb
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_ARMARCH7_common_gfxTiDssFb


clean _layer_clean_BRANSON_LAYER_TIDSSFB : _clean_LIB_BASE_krnl_ARMARCH7_common_gfxTiDssFb

_clean_LIB_BASE_krnl_ARMARCH7_common_gfxTiDssFb :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a


endif

ifndef _DONEBASE_LIB_BRANSON_LAYER_TIDSSFB_krnl_ARMARCH7_common_gfxTiDssFb

_DONEBASE_LIB_BRANSON_LAYER_TIDSSFB_krnl_ARMARCH7_common_gfxTiDssFb = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBRANSON_LAYER_TIDSSFB_libgfxTiDssFb.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/layerBRANSON_LAYER_TIDSSFB_libgfxTiDssFb.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,BRANSON_LAYER_TIDSSFB,libgfxTiDssFb.a,$(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/ARMARCH7/common/libgfxTiDssFb$(OPT).a : $(LIBOBJS_krnl_ARMARCH7_common_gfxTiDssFb)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
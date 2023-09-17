
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
#	LIBDIRBASE = krnl/diab
#	LIBDIRBASE = krnl/$(TOOL_SPECIFIC_DIR)$(LIB_DIR_TAG)$(MINOR_OPTION_SUFFIX)






LIBOBJS = $(LIBOBJS_tilconDemo)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_diab_tilconDemo_kernel_top_TILCON_DEMO_basic_diab =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoRandomCurve.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoFont.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconBasic.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoPrimitive.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoWidget.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoAnimation.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconVectorFont.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoUpdateGUI.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconVectorSouthPark.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoVectorGraphics.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconDemoPainter.o  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo/tilconVectorLion.o 

arcmdfile_krnl_diab_tilconDemo : $(LIBOBJS_krnl_diab_tilconDemo_kernel_top_TILCON_DEMO_basic_diab)

LIBOBJS_krnl_diab_tilconDemo_TILCON_DEMO += $(LIBOBJS_krnl_diab_tilconDemo_kernel_top_TILCON_DEMO_basic_diab)

LIBOBJS_krnl_diab_tilconDemo += $(LIBOBJS_krnl_diab_tilconDemo_kernel_top_TILCON_DEMO_basic_diab)

__OBJS_TO_CLEAN_TILCON_DEMO += $(LIBOBJS_krnl_diab_tilconDemo_kernel_top_TILCON_DEMO_basic_diab) $(LIBOBJS_krnl_diab_tilconDemo_kernel_top_TILCON_DEMO_basic_diab:.o=.d)

ifndef _DONEBASE_LIB_krnl_diab_tilconDemo

_DONEBASE_LIB_krnl_diab_tilconDemo = TRUE

# LIBBASES += tilconDemo

__LIBS_TO_BUILD += krnl_diab_tilconDemo

__LIBS_TO_BUILD_TILCON_DEMO += krnl_diab_tilconDemo

__BUILT_LIBS += krnl_diab_tilconDemo

__LIB_krnl_diab_tilconDemo = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilconDemo :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab && $(NM) libtilconDemo$(OPT).a libtilconDemo$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libtilconDemo.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_diab_tilconDemo

arcmdfile_krnl_diab_tilconDemo :
	$(file >$@,$(LIBOBJS_krnl_diab_tilconDemo))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a : arcmdfile_krnl_diab_tilconDemo
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_diab_tilconDemo


clean _layer_clean_TILCON_DEMO : _clean_LIB_BASE_krnl_diab_tilconDemo

_clean_LIB_BASE_krnl_diab_tilconDemo :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a


endif

ifndef _DONEBASE_LIB_TILCON_DEMO_krnl_diab_tilconDemo

_DONEBASE_LIB_TILCON_DEMO_krnl_diab_tilconDemo = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/layerTILCON_DEMO_libtilconDemo.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/layerTILCON_DEMO_libtilconDemo.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,TILCON_DEMO,libtilconDemo.a,$(LIBOBJS_krnl_diab_tilconDemo)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilconDemo$(OPT).a : $(LIBOBJS_krnl_diab_tilconDemo)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 

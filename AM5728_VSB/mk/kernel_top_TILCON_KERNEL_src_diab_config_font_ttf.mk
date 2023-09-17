
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






LIBOBJS = $(LIBOBJS_tilcon)

# DEP_OBJS = (DEP_OBJS)

 
LIBOBJS_krnl_diab_tilcon_kernel_top_TILCON_KERNEL_src_diab_config_font_ttf =  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilcon/BuiltInTtfFont.o 

arcmdfile_krnl_diab_tilcon : $(LIBOBJS_krnl_diab_tilcon_kernel_top_TILCON_KERNEL_src_diab_config_font_ttf)

LIBOBJS_krnl_diab_tilcon_TILCON_KERNEL += $(LIBOBJS_krnl_diab_tilcon_kernel_top_TILCON_KERNEL_src_diab_config_font_ttf)

LIBOBJS_krnl_diab_tilcon += $(LIBOBJS_krnl_diab_tilcon_kernel_top_TILCON_KERNEL_src_diab_config_font_ttf)

__OBJS_TO_CLEAN_TILCON_KERNEL += $(LIBOBJS_krnl_diab_tilcon_kernel_top_TILCON_KERNEL_src_diab_config_font_ttf) $(LIBOBJS_krnl_diab_tilcon_kernel_top_TILCON_KERNEL_src_diab_config_font_ttf:.o=.d)

ifndef _DONEBASE_LIB_krnl_diab_tilcon

_DONEBASE_LIB_krnl_diab_tilcon = TRUE

# LIBBASES += tilcon

__LIBS_TO_BUILD += krnl_diab_tilcon

__LIBS_TO_BUILD_TILCON_KERNEL += krnl_diab_tilcon

__BUILT_LIBS += krnl_diab_tilcon

__LIB_krnl_diab_tilcon = C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/objtilcon :
	$(MKDIR) $@


lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon.cdf

# need to pass the lib.a file twice to C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/ddump -Ng to workaround ddump problem
C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon.nm : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a
	$(if $(wildcard $<),cd C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab && $(NM) libtilcon$(OPT).a libtilcon$(OPT).a > $@, $(info skipping $@ : non existant $<))

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon.cdf : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon.nm 
	$(if $(wildcard $<),$(LIB2CDF) -i $< -a libtilcon.a -o $@, $(info skipping $@ : non existant $<))

arcmdfiles : arcmdfile_krnl_diab_tilcon

arcmdfile_krnl_diab_tilcon :
	$(file >$@,$(LIBOBJS_krnl_diab_tilcon))
	@echo created $@

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a : arcmdfile_krnl_diab_tilcon
	C:/WindRiverSR500/compilers/diab-5.9.6.1/WIN32/bin/dar crusv $@ @arcmdfile_krnl_diab_tilcon


clean _layer_clean_TILCON_KERNEL : _clean_LIB_BASE_krnl_diab_tilcon

_clean_LIB_BASE_krnl_diab_tilcon :
	rm -f C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a


endif

ifndef _DONEBASE_LIB_TILCON_KERNEL_krnl_diab_tilcon

_DONEBASE_LIB_TILCON_KERNEL_krnl_diab_tilcon = TRUE

lib : C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/layerTILCON_KERNEL_libtilcon.cdf

C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/layerTILCON_KERNEL_libtilcon.cdf :  C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a
	$(file >$@, $(call __tmpl_layercdf,TILCON_KERNEL,libtilcon.a,$(LIBOBJS_krnl_diab_tilcon)))

endif


C:/ScriptGSX2-3/GSX2-3Stream/AM5728_VSB/krnl/diab/libtilcon$(OPT).a : $(LIBOBJS_krnl_diab_tilcon)




 

################
# rules defined locally in Makefile
##



################

objs : $(LIBOBJS)
 
# this is the end of the file 
